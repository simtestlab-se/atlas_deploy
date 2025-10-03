// BmsContactorControl.h
#ifndef BMSCONTACTORCONTROL_H
#define BMSCONTACTORCONTROL_H

#include <stdbool.h>

/* Contactor types */
typedef enum {
    CONTACTOR_POSITIVE = 0,     // Main Positive
    CONTACTOR_NEGATIVE,         // Main Negative
    CONTACTOR_PRECHARGE,        // Precharge contactor
} CONTACTOR_TYPE_e;

/* Contactor switch states */
typedef enum {
    CONTACTOR_OPEN = 0,
    CONTACTOR_CLOSE
} CONTACTOR_STATE_e;

/* Initialization function */
void ContactorControl_Init(void);

/* Logic update function – should be called periodically */
void ContactorControl_UpdateLogic(void);

/* Directly request a specific contactor to turn ON/OFF */
void ContactorControl_RequestState(CONTACTOR_TYPE_e type, CONTACTOR_STATE_e requestedState);

/* Read back the requested state for a contactor */
CONTACTOR_STATE_e ContactorControl_GetState(CONTACTOR_TYPE_e type);

/* Reset all contactors to OFF state */
void ContactorControl_ResetAll(void);

/* Get the last precharge error code */
int ContactorControl_GetLastError(void);


#endif /* BMSCONTACTORCONTROL_H */
