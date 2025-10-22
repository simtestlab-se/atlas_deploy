#include "bms_os.h"
#include "bms.h"
#include "bms_config.h"
#include "algorithm.h"

/* Task handles (optional, if you need to monitor them) */
TaskHandle_t hTaskBms100ms;
TaskHandle_t hTaskContactor;
TaskHandle_t hTaskBmsCAN;
TaskHandle_t hTaskAlgorithm;
TaskHandle_t hTaskCellBal;
TaskHandle_t hTaskTwinCAT;
TaskHandle_t hTaskBms1s;
TaskHandle_t hTask_VehicleLoad;

void bms_OS_Init(void)
{
    /* BMS measurement + limit checks (5 ms loop) */
    xTaskCreate(TwinCATTask,
                "EtherCAT Task",
                TASK_STACK_SIZE_LARGE,
                NULL,
                TASK_PRIORITY_HIGHEST,
                &hTaskTwinCAT);

    xTaskCreate(TaskBms100ms,
                "Bms100ms",
                TASK_STACK_SIZE_MINIMAL,
                NULL,
                TASK_PRIORITY_HIGH,
                &hTaskBms100ms);

    /* CellBal (10 ms loop) */
    xTaskCreate(TaskCellBal,
                "cellbalance",
                TASK_STACK_SIZE_MINIMAL,
                NULL,
                TASK_PRIORITY_MEDIUM,
                &hTaskCellBal);

    /* Contactor and Link voltage (100 ms loop) */
    xTaskCreate(ContactorTask,
                "Contactor",
                TASK_STACK_SIZE_MINIMAL,
                NULL,
                TASK_PRIORITY_LOWEST,
                &hTaskContactor);

    /* CAN transmit/receive (500 ms loop) */
    xTaskCreate(TaskBmsCAN,
                "BmsCAN",
                TASK_STACK_SIZE_MINIMAL,
                NULL,
                TASK_PRIORITY_LOW,
                &hTaskBmsCAN);

    /* SOC Algorithm (200 ms loop) */
    xTaskCreate(bmsalgo_Task,
                "BmsAlgo",
                TASK_STACK_SIZE_MINIMAL,
                NULL,
                TASK_PRIORITY_LOW,
                &hTaskAlgorithm);

    /* NTC temperature (1000 ms loop) */
    xTaskCreate(TaskBms1s,
                "Bms1s",
                TASK_STACK_SIZE_MINIMAL,
                NULL,
                TASK_PRIORITY_LOWEST,
                &hTaskBms1s);

    /* load (100 ms loop) */
    xTaskCreate(Task_VehicleLoad,
                "VehicleLoad",
                TASK_STACK_SIZE_MINIMAL,
                NULL,
                TASK_PRIORITY_LOWEST,
                &hTask_VehicleLoad);
}
