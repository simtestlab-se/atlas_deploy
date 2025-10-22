#ifndef CAN_SERVICE_H
#define CAN_SERVICE_H

#include <stdint.h>
#include <stdbool.h>
#include "bms_config.h"
/* Public constants */
#define CAN_MAX_DLC            (8U)

/* Status codes for the service */
typedef enum
{
    CAN_SVC_OK = 0,
    CAN_SVC_ERR_PARAM,
    CAN_SVC_ERR_HW,
    CAN_SVC_NO_DATA
} can_svc_status_t;

/* Basic RX frame container */
typedef struct
{
    uint16_t id;                  /* Standard 11-bit CAN ID */
    uint8_t  dlc;                 /* Data length code (0..8) */
    uint8_t  data[CAN_MAX_DLC];   /* Payload */
} can_frame_t;


can_frame_t tx;
can_frame_t rx;


/* Init once at startup. Returns CAN_SVC_OK if ready. */
can_svc_status_t CAN_Service_Init(uint32_t baud_hz,
                                  uint16_t sample_point_percent,
                                  uint8_t sjw_tq);

/* Transmit a standard 11-bit frame (non-blocking fire-and-forget). */
can_svc_status_t CAN_Service_Tx(const can_frame_t *frame);

/* Try to fetch one received frame. Returns CAN_SVC_NO_DATA if none. */
can_svc_status_t CAN_Service_Rx(can_frame_t *out_frame);

/* Lightweight health query helpers */
uint8_t  CAN_Service_GetRxErrCnt(void);
uint8_t  CAN_Service_GetTxErrCnt(void);
uint32_t CAN_Service_GetNodeStatus(void);

#endif /* CAN_SERVICE_H */
