/*
 * bms_libWrapper.cpp
 *
 *  Created on: Nov 24, 2024
 *      Author: amrlxyz
 */

/*
 * Compatible commands
 * ADBMS2950 == ADBMS6830
 *
 * RDCFGA
 * RDCFGB
 * ADI1     = ADCV
 * ADI2     = ADSV
 * RDI      = RDFCA or RDCVA
 * RDVB     = RDFCB or RDCVB
 * RDIVB1   = RDFCC or RDCVC
 * RDIACC   = RDACA
 * RDVBACC  = RDACB
 * RDIVB1ACC= RDACC
 *
 */

/*
 * Commands Notes
 *
 * -- 6830 --
 * ADCV : Start ADC
 * ADSV : Start redundancy ADC
 * RDCVA: Read Cell Voltage A
 * RDFCA: Read Filtered Cell A
 * RDACA: Read Averaged Cell A
 *
 * -- 2950 --
 * ADIx: Start IxADC and VBxADC
 * RDI : Read Read I1ADC and I2ADC results
 *
 */

#include "bms_cmdlist.h"
#include "bms_datatypes.h"
#include "bms_libWrapper.h"
#include "bms_mcuWrapper.h"
#include "bms_utility.h"
#include "mcu_wrapper.h"
#include "dave.h"
#include <string.h>
#include <stdio.h>




uint8_t  txData[TOTAL_AD_IC][DATA_LEN];
uint8_t  rxData[TOTAL_AD_IC][DATA_LEN];
uint16_t rxPec[TOTAL_AD_IC];
uint8_t  rxCc[TOTAL_AD_IC];

uint32_t packCurrent;
float voltage1 ;
float voltage2 ;

float packCurrent1 ;
float packCurrent2 ;
typedef struct
{
    ad29_cfa_t cfa_Tx;
    ad29_cfa_t cfa_Rx;
    ad29_cfb_t cfb_Tx;
    ad29_cfb_t cfb_Rx;
} ic_ad29_t;


typedef struct
{
    ad68_cfa_t cfa_Tx;
    ad68_cfa_t cfa_Rx;
    ad68_cfb_t cfb_Tx;
    ad68_cfb_t cfb_Rx;

    ad68_pwma_t pwma;
    ad68_pwmb_t pwmb;

    float v_avgCell[16];
    float v_avgCell_sum;
    float v_avgCell_avg;
    float v_avgCell_min;
    float v_avgCell_max;
    float v_avgCell_delta;

    float v_sCell[16];

    float v_tempSens[16];
    float v_segment;

    float temp_cell[16];
    float temp_ic;
} ic_ad68_t;


ic_ad29_t ic_ad29;
ic_ad68_t ic_ad68[TOTAL_AD68];



void bms_resetConfig(void)
{
    // Obtained from RDCFG after reset
    // Flipped due to Little endian
//    uint64_t const ad29_cfaDefault = 0x00 00 00 3F 3F 11;
    uint64_t const ad29_cfaDefault = 0x113F3F000000;
//    uint64_t const ad29_cfbDefault = 0x00 00 00 00 01 F0;
    uint64_t const ad29_cfbDefault = 0xF00100000000;
//    uint64_t const ad68_cfaDefault = 0x01 00 00 FF 03 00;
    uint64_t const ad68_cfaDefault = 0x0003FF000001;
//    uint64_t const ad68_cfbDefault = 0x00 F8 7F 00 00 00;
    uint64_t const ad68_cfbDefault = 0x0000007FF800;

    // Copy defaults to Tx Buffer
    memcpy(&ic_ad29.cfa_Rx, &ad29_cfaDefault, DATA_LEN);
    memcpy(&ic_ad29.cfb_Rx, &ad29_cfbDefault, DATA_LEN);

    for (int ic = 0; ic < TOTAL_AD68; ic++)
    {
        memcpy(&ic_ad68[ic].cfa_Tx, &ad68_cfaDefault, DATA_LEN);
        memcpy(&ic_ad68[ic].cfb_Tx, &ad68_cfbDefault, DATA_LEN);
    }

    ad68_cfa_t ad68_cfaT;
    memcpy(&ad68_cfaT, &ad68_cfaDefault, DATA_LEN);
}


void bms2950_init(void)
{
    bms_resetConfig();
}


void bms_writeConfigA(void)
{
    // Fill buffer for ad2950 first
    memcpy(txData[0], &ic_ad29.cfa_Tx, DATA_LEN);

    // Fill buffer with the other ad6830 data
    for (int ic = 0; ic < TOTAL_AD68; ic++)
    {
        memcpy(txData[ic+1], &ic_ad68[ic].cfa_Tx, DATA_LEN);
    }

    // write config A
    bms_transmitData(WRCFGA, txData);
}


void bms_writeConfigB(void)
{
    // Fill buffer for ad2950 first
    memcpy(txData[0], &ic_ad29.cfb_Tx, DATA_LEN);

    // Fill buffer with the other ad6830 data
    for (int ic = 0; ic < TOTAL_AD68; ic++)
    {
        memcpy(txData[ic+1], &ic_ad68[ic].cfb_Tx, DATA_LEN);
    }

    // write config B
    bms_transmitData(WRCFGB, txData);
}


void bms_writePwmA(void)
{
    // Fill padding bytes for ad29
    memset(txData[0], 0x00, DATA_LEN);

    // Fill buffer with the other ad6830 data
    for (int ic = 0; ic < TOTAL_AD68; ic++)
    {
        memcpy(txData[ic+1], &ic_ad68[ic].pwma, DATA_LEN);
    }

    // write config A
    bms_transmitData(WRPWM1, txData);
}


void bms_writePwmB(void)
{
    // Fill padding bytes for ad29
    memset(txData[0], 0x00, DATA_LEN);

    // Fill buffer with the other ad6830 data
    for (int ic = 0; ic < TOTAL_AD68; ic++)
    {
        memcpy(txData[ic+1], &ic_ad68[ic].pwmb, DATA_LEN);
    }

    // write config B
    bms_transmitData(WRPWM2, txData);
}


void bms68_setGpo45(uint8_t twoBitIndex)
{
    // GPIO Output: 1 = No pulldown (Default), 0 = Pulldown
    // Only for pin 4 and 5
    for (int ic = 0; ic < TOTAL_AD68; ic++)
    {
        ic_ad68[ic].cfa_Tx.gpo1to8 = ((twoBitIndex) << 3) | (0xFF ^ (0x3 << 3));
    }

    bms_writeConfigA();
}


void bms_printRawData(uint8_t data[TOTAL_AD_IC][DATA_LEN], uint8_t cc[TOTAL_AD_IC])
{
    for (int ic = 0; ic < TOTAL_AD_IC; ic++)
    {
        printf("IC%d: ", ic+1);
        for (int j = 0; j < 6; j++)             // For every byte recieved (6 bytes)
        {
            printf("0x%02X, ", data[ic][j]);    // Print each of the bytes
        }
        printf("CC: %d |   ", cc[ic]);
    }
    printf("\n\n");
}


bool bms_checkRxFault(uint8_t data[TOTAL_AD_IC][DATA_LEN], uint16_t pec[TOTAL_AD_IC], uint8_t cc[TOTAL_AD_IC])
{
    bool faultDetected = false;
    bool errorIndex[TOTAL_AD_IC];

    if (!bms_checkRxPec(data, pec, cc, errorIndex))
    {
        printf("WARNING! PEC ERROR - IC:");
        for(int ic = 0; ic < TOTAL_AD_IC; ic++)
        {
            if (!errorIndex[ic])
            {
                printf(" %d,", ic+1);
            }
        }
        printf("\n");
        faultDetected = true;
    }

    return faultDetected;

    // TODO: Add command counter fault checker
    // TODO: Add fault handler for PEC fault
}


// used mostly for debugging purposes
void bms_readSid(void)
{
    bms_receiveData(RDSID, rxData, rxPec, rxCc);
    printf("SID: \n");
    bms_checkRxFault(rxData, rxPec, rxCc);
    bms_printRawData(rxData, rxCc);
}


void bms_readConfigA(void)
{
    bms_receiveData(RDCFGA, rxData, rxPec, rxCc);
    printf("CFGA: \n");
    bms_checkRxFault(rxData, rxPec, rxCc);
    bms_printRawData(rxData, rxCc);
}


void bms_readConfigB(void)
{
    bms_receiveData(RDCFGB, rxData, rxPec, rxCc);
    printf("CFGB: \n");
    bms_checkRxFault(rxData, rxPec, rxCc);
    bms_printRawData(rxData, rxCc);
}


void bms_startAdcvCont(void)
{
    // 6830
    // For DCP = 0
    // If RD = 0 and CONT = 1, PWM discharge is permitted
    // If RD = 1 and CONT = 0, PWM discharge interrupted temporarily until RD conversion finished (8ms typ)
    // If RD = 1 and CONT = 1, PWM discharge stopped

    ADCV.CONT = 1;      // Continuous
    ADCV.RD   = 1;      // Redundant Measurement
    ADCV.DCP  = 0;      // Discharge permitted
    ADCV.RSTF = 0;      // Reset filter
    ADCV.OW   = 0b00;   // Open wire on C-ADCS and S-ADCs

    // Behaviour of 2950 (ADI1 Command)
    //

    bms_transmitCmd((uint8_t *)&ADCV);
}


void bms_stopDischarge(void)
{
    bms_wakeupChain();
    bms_transmitCmd(SRST);      // Put all devices to sleep
    printf("--- SOFT RESET --- \n");
}

void bms_softReset(void)
{
    bms_wakeupChain();
    bms_transmitCmd(SRST);      // Put all devices to sleep
    printf("\n  ---  SOFT RESET  ---  \n");
}
void bms29_setGpo(void)
{
    ic_ad29.cfa_Tx.gpo1c  = 1;      // State control
    ic_ad29.cfa_Tx.gpo1od = 0;      // 1 = Open drain, 0 = push-pull
    ic_ad29.cfa_Tx.gpo2c  = 1;      // State control
    ic_ad29.cfa_Tx.gpo2od = 0;      // 1 = Open drain, 0 = push-pull

    bms_writeConfigA();
}


float bms_readVB(void)
{
    bms_receiveData(RDVB, rxData, rxPec, rxCc);
    bms_checkRxFault(rxData, rxPec, rxCc);
 //   bms_printRawData(rxData, rxCc);
    float vb1 = *((int16_t *)(rxData[0] + 2)) *  0.000100 * 396.604395604;
    float vb2 = *((int16_t *)(rxData[0] + 4)) * -0.000085 * 751;
//    printfDma("VB: %fV, %fV  \n\n", vb1, vb2);
    voltage1 = vb1;
    voltage2 = vb2;

    return voltage2;
}




float bms29_readCurrent(void)
{
    bms_receiveData(RDI, rxData, rxPec, rxCc);
    bms_checkRxFault(rxData, rxPec, rxCc);
//    bms_printRawData(rxData, rxCc);

    // microvolts
    int32_t i1v = 0;
    int32_t i2v = 0;

    i1v = ((uint32_t)rxData[0][0]) | ((uint32_t)rxData[0][1] << 8) | ((int32_t)rxData[0][2] << 16);
    i2v = ((uint32_t)rxData[0][3]) | ((uint32_t)rxData[0][4] << 8) | ((int32_t)rxData[0][5] << 16);

    if (i1v & (UINT32_C(1) << 23)) { i1v |= 0xFF000000; }; // Check the sign bit (24th bit) and extend the sign
    if (i2v & (UINT32_C(1) << 23)) { i2v |= 0xFF000000; };

    const float SHUNT_RESISTANCE = 0.000050; // 50 microOhms

    float current1 = ((float)i1v / 1000000.0f) / SHUNT_RESISTANCE;
    float current2 = ((float)i2v / 1000000.0f) / SHUNT_RESISTANCE;

    packCurrent = (current1 + current2) * 1000 / 2;  // Store in mA for internal use

    packCurrent1 = current1;
    packCurrent2 = current2;

    // Return the average current in Amperes for consistency with pack_current global variable
    return packCurrent2;
//    printfDma("Current: %fA, %fA  \n\n", current1 , current2);
}



