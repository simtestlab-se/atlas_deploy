#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "bms.h"
#include "bms_config.h"
#include "can_service.h"
#include <bms_cmdlist.h>
#include <bms_datatypes.h>
#include <bms_libWrapper.h>
#include <bms_mcuWrapper.h>
#include <bms_utility.h>
#include <adBms6830Data.h>
#include <adBms6830GenericType.h>
#include <custom_bms.h>
#include <mcu_wrapper.h>
#include "global_db.h"

#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"
#include "algorithm.h"
#include "BmsContactorControl.h"
#include "CellLimitViolation.h"

/* === Global variables === */
float cellValues[TOTAL_CELLS];
float gpio_tempValues[TOTAL_GPIO_CHANNELS];
int16_t cell_values[TOTAL_CELLS];
float pack_voltage = 0;
float temp = 0.0;

bool contactor_req = true;
bool contactor_err_reset_req = false;
bool key_status = true;

float load_current_ch1 = 0.0f;
float load_current_ch2 = 0.0f;

uint8_t cellVReg[CELL_VREG_SIZE];
uint8_t auxVReg[AUX_VREG_SIZE];
uint32_t current_timestamp = 0;

extern bool violation_overvoltage;
extern bool violation_undervoltage;
extern bool violation_overtemperature;
extern bool violation_undertemperature;
extern bool violation_overcurrent;
extern bool violation_cell_imbalance;

uint16_t adc_val_ch1;
float voltage_ch1;
float current_ch1;
uint16_t adc_val_ch2;
float voltage_ch2;
float current_ch2;

float pack_current = 0.0f;
float ntc_temperatures_c[NUM_NTC_CHANNELS] = {0};
uint16_t adc_temp_raw[NUM_NTC_CHANNELS] = {0};
float raw_temp[NUM_NTC_CHANNELS] = {0};
float last_temp[NUM_NTC_CHANNELS] = {0};

ADC_MEASUREMENT_CHANNEL_t* ntc_adc_channels[NUM_NTC_CHANNELS] = {
    &ADC_MEASUREMENT_Channel_B_handle
};

float link_voltage = 0.0f;
uint16_t link_adc_raw= 0;
bool cell_balance = false;
bool Positive_contactor = false;
bool Negative_contactor = false;
bool Precharge_contactor = false;
uint8_t pwm_data[6] = {0};
float soc[TOTAL_CELLS];
float min_soc = 0.0f;
float max_soc = 0.0f;
float average_soc = 0.0f;
float pack_sum_voltage = 0.0f;
uint16_t gpio_values[GPIO_VALUES_SIZE];
uint16_t soc_scaled;

extern can_frame_t tx;
extern can_frame_t rx;


/* === Consolidated Initialization Function === */
bool Bms_Init(void)
{
    bool init_success = true;

    /* Initialize DWT for timing */
    DWT_Init();

    /* Initialize SPI for BMS communication */
    SPI_MASTER_SetMode(&SPI_MASTER_0, XMC_SPI_CH_MODE_STANDARD);
    XMC_USIC_CH_SetFrameLength(XMC_SPI0_CH1, 8);
    SPI_MASTER_SetBaudRate(&SPI_MASTER_0, 2000000U);

    /* Initialize ADC for measurements */
    ADC_MEASUREMENT_StartConversion(&ADC_MEASUREMENT_0);

    /* Initialize BMS configuration */
    init_cfg();
    bms_softReset();
    Delay_ms(200);  // Initialization delay

    /* Initialize BMS chain */
    bms_wakeupChain();
    bms2950_init();     // Initialize BMS configs and send them
    bms_readSid();

    /* Initialize CAN */
    if (CAN_Service_Init(CAN_DEFAULT_BAUD, CAN_DEFAULT_SP_PERCENT, CAN_DEFAULT_SJW_TQ) != CAN_SVC_OK)
    {
        XMC_DEBUG("CAN initialization failed\n");
        init_success = false;
    }

    /* Initialize GPIOs for contactors */
    DIGITAL_IO_SetOutputLow(&POSITIVE_PIN);
    DIGITAL_IO_SetOutputLow(&NEGATIVE_PIN);
    DIGITAL_IO_SetOutputLow(&PRECHARGE_PIN);
    DIGITAL_IO_SetOutputLow(&RELAY_LOAD);

    /* Initialize global variables */
    memset(cellValues, 0, sizeof(cellValues));
    memset(gpio_tempValues, 0, sizeof(gpio_tempValues));
    memset(cell_values, 0, sizeof(cell_values));
    memset(ntc_temperatures_c, 0, sizeof(ntc_temperatures_c));
    memset(soc, 0, sizeof(soc));

    XMC_DEBUG("BMS Initialization %s\n", init_success ? "SUCCESS" : "FAILED");
    return init_success;
}

/* Rest of the bms.c file remains the same... */
void Relay_On(void)  { DIGITAL_IO_SetOutputHigh(&RELAY_LOAD); }
void Relay_Off(void) { DIGITAL_IO_SetOutputLow(&RELAY_LOAD); }


/* === Tasks === */

void TwinCATTask(void *pvParameters) {
    while (1) {
        MainLoop();
        vTaskDelay(pdMS_TO_TICKS(TASK_PERIOD_5MS));
    }
}

void TaskBms100ms(void *pvParameters)
{
    while (1) {
        BmsVoltage();
        PackVoltage();
        Check_CellLimitViolations();
        vTaskDelay(pdMS_TO_TICKS(TASK_PERIOD_100MS));
    }
}

void TaskCellBal(void *pvParameters)
{
    while (1) {
        cellbalance();
        vTaskDelay(pdMS_TO_TICKS(TASK_PERIOD_10MS));
    }
}

void ContactorTask(void *pvParameters)
{
    while (1)
    {
        Contactorcntl();
        LinkVoltage();
        vTaskDelay(pdMS_TO_TICKS(TASK_PERIOD_100MS));
    }
}

void TaskBms1s(void *pvParameters)
{
    while (1) {
        NTCTemperature();
        vTaskDelay(pdMS_TO_TICKS(TASK_PERIOD_1000MS));
    }
}

void Task_VehicleLoad(void *pvParameters) {
    (void)pvParameters;

    while (1) {
        bool key_state = key_status;

        if (key_state) {
            Relay_On();
            load_current_ch1 = Read_Current_ch1();
            load_current_ch2 = Read_Current_ch2();
            // TODO: Process current value (log, send via CAN/EtherCAT)
        } else {
            Relay_Off();
        }
        vTaskDelay(pdMS_TO_TICKS(TASK_PERIOD_100MS));
    }
}

void TaskBmsCAN(void *pvParameters)
{
    (void)pvParameters;

    while (1)
    {
        CAN_SendCellVoltages();
        CAN_SendGpioValues();
        CAN_SendSocValues();
        CAN_SendContactors();

        /* Poll RX */
        if (CAN_Service_Rx(&rx) == CAN_SVC_OK)
        {
            /* TODO: Add processing (queue, mailbox, etc.) */
            (void)rx;
        }

        /* Health checks */
        const uint32_t status = CAN_Service_GetNodeStatus();
        const uint8_t  rxerr  = CAN_Service_GetRxErrCnt();
        const uint8_t  txerr  = CAN_Service_GetTxErrCnt();
        app_log_status(status, rxerr, txerr);

        /* Periodicity */
        vTaskDelay(pdMS_TO_TICKS(TASK_PERIOD_500MS));
    }
}

/* === Core functions === */

/**
 * @brief Handles cell and auxiliary voltage measurement.
 *
 * <b>Details of function</b><br>
 * Measures cell voltages and auxiliary voltages, parses and prints the results.
 *
 * @return Always returns 0.
 */
int voltage_loop(void){
    memset(cellVReg, 0, sizeof(cellVReg));
    memset(auxVReg, 0, sizeof(auxVReg));

    pollCellVoltage(cellVReg);
    parse_print_cell_measurement(cellVReg);
    pollAuxVoltage(auxVReg);
    parse_print_gpio_measurement(auxVReg);

    return 0;
}

/**
 * @brief Parses and stores cell voltage measurements from buffer.
 *
 * <b>Details of function</b><br>
 * Extracts cell voltage data from the provided buffer, converts to voltage, and stores in global arrays.
 *
 * @param buff Pointer to buffer containing cell voltage data.
 */
void parse_print_cell_measurement(uint8_t* buff){
    memset(cell_values, 0, sizeof(cell_values));
    memset(cellValues, 0, sizeof(cellValues));
    pack_sum_voltage = 0;
    current_timestamp = xTaskGetTickCount();

    for(int x = 0; x < TOTAL_CELLS; x++){
        cell_values[x] = buff[2*x]|buff[2*x + 1]<<8; // Not using memcpy because I am not sure about endianness
        temp = C_VOLT_CONV(cell_values[x]);
        cellValues[x] = temp;
        pack_sum_voltage += cellValues[x];
        XMC_DEBUG("cellValues%d = %.1f  \n",x+1, cellValues[x]);
    }
}

/**
 * @brief Parses and stores GPIO measurement data from buffer.
 *
 * <b>Details of function</b><br>
 * Extracts GPIO measurement data from the provided buffer, converts to voltage, and stores in global arrays.
 *
 * @param buff Pointer to buffer containing GPIO measurement data.
 */
void parse_print_gpio_measurement(uint8_t* buff){
    for(int x = 0; x < TOTAL_GPIO_CHANNELS; x++){
        gpio_values[x] = buff[2*x]|buff[2*x + 1]<<8; // Not using memcpy because I am not sure about endianness
        temp = C_VOLT_CONV(gpio_values[x]);
        gpio_tempValues[x] = C_VOLT_CONV(gpio_values[x]);
    }
}

void BmsVoltage(void) {
    readSID();
    configBMS();
    readCFG();
    voltage_loop();
    readStatErr();
}

void cellbalance(void)
{
    if(cell_balance)
    {
        for(int i=0;i<6;i++)
        {
            pwm_data[i] = 0xFF;
        }
        PWMforBalancing();     // Enable passive balancing
    }
    else
    {
        for(int i=0;i<6;i++)
        {
            pwm_data[i] = 0x00;
        }
        PWMforBalancing();     // disable passive balancing
    }
}

void PackVoltage(void) {
    bms_wakeupChain();
    bms_startAdcvCont();            // Need to wait 8ms for the average register to fill up
    bms_wakeupChain();
    bms29_setGpo();
    pack_voltage = bms_readVB();
    pack_current = bms29_readCurrent();  // currently commented

    XMC_DEBUG("pack_voltage = %.2f   | pack_current = %.2f  \n", pack_voltage, pack_current);
}

void LinkVoltage(void) {
    ADC_MEASUREMENT_StartConversion(&ADC_MEASUREMENT_0);
    link_adc_raw = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_Channel_A_handle);
    float adc_voltage = (link_adc_raw / ADC_MAX_VALUE) * V_REF;
    link_voltage = adc_voltage * VOLTAGE_DIVIDER_SCALE;
}

void NTCTemperature(void) {
    ADC_MEASUREMENT_StartConversion(&ADC_MEASUREMENT_0);

    for (uint8_t i = 0; i < NUM_NTC_CHANNELS; i++) {
        adc_temp_raw[i] = ADC_MEASUREMENT_GetResult(ntc_adc_channels[i]);
        raw_temp[i] = calculate_temperature_from_adc(adc_temp_raw[i]);
        ntc_temperatures_c[i] = apply_ema_filter(i, raw_temp[i]);
    }
}

void Contactorcntl(void)
{
    /* Update logic before driving hardware */
    ContactorControl_UpdateLogic();

    /* Drive Positive Contactor */
    if (Positive_contactor)
    {
        (void)DIGITAL_IO_SetOutputHigh(&POSITIVE_PIN);
    }
    else
    {
        (void)DIGITAL_IO_SetOutputLow(&POSITIVE_PIN);
    }

    /* Drive Negative Contactor */
    if (Negative_contactor)
    {
        (void)DIGITAL_IO_SetOutputHigh(&NEGATIVE_PIN);
    }
    else
    {
        (void)DIGITAL_IO_SetOutputLow(&NEGATIVE_PIN);
    }

    /* Drive Precharge Contactor */
    if (Precharge_contactor)
    {
        DIGITAL_IO_SetOutputHigh(&PRECHARGE_PIN);
    }
    else
    {
        (void)DIGITAL_IO_SetOutputLow(&PRECHARGE_PIN);
    }
}

/* Optional lightweight logging hook (no stdio in hot paths) */
void app_log_status(uint32_t node_status, uint8_t rx_err, uint8_t tx_err)
{
    (void)node_status;
    (void)rx_err;
    (void)tx_err;
    /* Map these to your USB CDC / RTT / LED indicators if needed. */
}

/* === Static helpers === */

/**
 * @brief Calculates temperature from ADC raw value using NTC thermistor.
 *
 * <b>Details of function</b><br>
 * Converts ADC value to voltage, computes NTC resistance, and calculates temperature using the Beta equation.
 *
 * @param adc_raw Raw ADC value.
 * @return Calculated temperature in Celsius.
 */
float calculate_temperature_from_adc(uint32_t adc_raw) {
    float v_adc = ((float)adc_raw / ADC_MAX_VALUE) * V_REF;

    if (v_adc < 0.01f || v_adc > (V_REF - 0.01f)) {
        return -273.15f;  // Invalid or noisy reading
    }

    // Inverted voltage divider formula:
    // NTC is on top (connected to VCC), R_FIXED is at bottom (to GND)
    float r_ntc = (V_REF * R_FIXED / v_adc) - R_FIXED;

    // Steinhart-Hart or Beta Equation
    float temp_k = 1.0f / ((1.0f / T0) + (1.0f / BETA) * logf(r_ntc / R0));
    float temp_c = temp_k - 273.15f;

    return temp_c + CALIBRATION_OFFSET;  // Optional offset tuning
}

/**
 * @brief Applies Exponential Moving Average (EMA) filter to temperature readings.
 *
 * <b>Details of function</b><br>
 * Smooths temperature readings for each channel using EMA.
 *
 * @param i Index of the temperature channel.
 * @param new_temp New temperature reading.
 * @return Smoothed temperature value.
 */
float apply_ema_filter(uint8_t i, float new_temp) {
    if (last_temp[i] == 0.0f) {
        last_temp[i] = new_temp;
    }
    last_temp[i] = ALPHA * new_temp + (1.0f - ALPHA) * last_temp[i];
    return last_temp[i];
}

/* ================= CAN transmit ================= */
void CAN_SendU16(uint32_t id, uint16_t value)
{
    memset(tx.data, 0, sizeof(tx.data));
    memcpy(tx.data, &value, sizeof(uint16_t));
    tx.id  = id;
    tx.dlc = CAN_DLC_2BYTES;
    (void)CAN_Service_Tx(&tx);
    Delay_ms(5);
}

void CAN_SendBool(uint32_t id, bool value)
{
    memset(tx.data, 0, sizeof(tx.data));
    memcpy(tx.data, &value, sizeof(bool));
    tx.id  = id;
    tx.dlc = CAN_DLC_1BYTE;
    (void)CAN_Service_Tx(&tx);
    Delay_ms(5);
}

/* Send cell voltages */
void CAN_SendCellVoltages(void)
{
    uint8_t cell_can_id = CAN_ID_CELL_VOLTAGES_BASE;
    memset(tx.data, 0, sizeof(tx.data));

    for (int i = 0; i < TOTAL_CELLS; i += 4)
    {
        for (int j = 0; j < 4; j++)
        {
            if ((i + j) < TOTAL_CELLS) {
                tx.data[j * 2]     = cell_values[i + j] & 0xFF;
                tx.data[j * 2 + 1] = (cell_values[i + j] >> 8) & 0xFF;
            }
        }
        tx.id  = cell_can_id++;
        tx.dlc = CAN_DLC_8BYTES;
        (void)CAN_Service_Tx(&tx);
        Delay_ms(5);
    }
}

/* Send GPIO values */
void CAN_SendGpioValues(void)
{
    uint8_t gpio_can_id = CAN_ID_GPIO_VALUES_BASE;
    memset(tx.data, 0, sizeof(tx.data));

    for (int i = 0; i < TOTAL_GPIO_CHANNELS; i += 4)
    {
        for (int j = 0; j < 4; j++)
        {
            if ((i + j) < TOTAL_GPIO_CHANNELS) {
                tx.data[j * 2]     = gpio_values[i + j] & 0xFF;
                tx.data[j * 2 + 1] = (gpio_values[i + j] >> 8) & 0xFF;
            }
        }
        tx.id  = gpio_can_id++;
        tx.dlc = CAN_DLC_8BYTES;
        (void)CAN_Service_Tx(&tx);
        Delay_ms(5);
    }
}

/* Send SOC values */
void CAN_SendSocValues(void)
{
    for (int i = 0; i < TOTAL_CELLS; i++)
    {
        uint16_t soc_scaled = (uint16_t)(soc[i] * 10.0f);
        CAN_SendU16(CAN_ID_SOC_VALUES_BASE + i, soc_scaled);
    }

    CAN_SendU16(CAN_ID_PACK_VOLTAGE, (uint16_t)(pack_voltage * 10.0f));
    CAN_SendU16(CAN_ID_PACK_CURRENT, (uint16_t)(pack_current * 10.0f));
    CAN_SendU16(CAN_ID_LINK_VOLTAGE, (uint16_t)(link_voltage * 10.0f));
    CAN_SendU16(CAN_ID_TEMPERATURE, (uint16_t)(ntc_temperatures_c[0] * 10.0f));
    CAN_SendU16(CAN_ID_MIN_SOC, (uint16_t)(min_soc * 10.0f));
    CAN_SendU16(CAN_ID_MAX_SOC, (uint16_t)(max_soc * 10.0f));
    CAN_SendU16(CAN_ID_AVG_SOC, (uint16_t)(average_soc * 10.0f));
}

/* Send Contactor states */
void CAN_SendContactors(void)
{
    CAN_SendBool(CAN_ID_CONTACTOR_POSITIVE, Positive_contactor);
    CAN_SendBool(CAN_ID_CONTACTOR_NEGATIVE, Negative_contactor);
    CAN_SendBool(CAN_ID_CONTACTOR_PRECHARGE, Precharge_contactor);
}

float Read_Current_ch1(void) {
    ADC_MEASUREMENT_StartConversion(&ADC_MEASUREMENT_0);
    adc_val_ch1 = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_Channel_C_handle);

    // Convert ADC → Voltage
    voltage_ch1 = ((float)adc_val_ch1 / ADC_MAX_VALUE) * CUR_SEN_VOLTAGE;

    // Calculate current using the formula:
    // I_P = ( (5 / U_C) * V_out - V_o ) * (1 / G)
    current_ch1 = ( (CUR_SEN_VOLTAGE / CUR_SEN_VOLTAGE) * voltage_ch1 - OFFSET_VOLTAGE ) * (1 / SENSOR_SENS_CH1);
    current_ch1 = current_ch1 - OFFSET_CURRENT_CH1;
    return current_ch1;
}

float Read_Current_ch2(void) {
    ADC_MEASUREMENT_StartConversion(&ADC_MEASUREMENT_0);
    adc_val_ch2 = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_Channel_D_handle);

    // Convert ADC → Voltage
    voltage_ch2 = ((float)adc_val_ch2 / ADC_MAX_VALUE ) * CUR_SEN_VOLTAGE;

    // Calculate current using the formula:
    // I_P = ( (5 / U_C) * V_out - V_o ) * (1 / G)
    current_ch2 = ( (CUR_SEN_VOLTAGE / CUR_SEN_VOLTAGE) * voltage_ch2 - OFFSET_VOLTAGE ) * (1 / SENSOR_SENS_CH2);
    current_ch2 = current_ch2 - OFFSET_CURRENT_CH2;

    return current_ch2;
}
