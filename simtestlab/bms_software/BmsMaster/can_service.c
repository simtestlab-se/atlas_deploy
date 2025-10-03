
#include "DAVE.h"          /* DAVE-generated headers (CAN_NODE_0 etc.) */
#include <string.h>
#include "can_service.h"
/* Static context – module scope only */
static bool g_can_inited = false;

/* Local helpers (internal linkage) */
static bool prv_check_dlc(uint8_t dlc)
{
    return (dlc <= CAN_MAX_DLC);
}

can_svc_status_t CAN_Service_Init(uint32_t baud_hz,
                                  uint16_t sample_point_percent,
                                  uint8_t sjw_tq)
{
    const CAN_NODE_t * const h = &CAN_NODE_0;

    /* Basic parameter checks */
    if ((sample_point_percent > 100U) || (sjw_tq > 3U))
    {
        return CAN_SVC_ERR_PARAM;
    }

    /* Initialize global CAN and node as per DAVE APP flow (once).
       CAN_NODE_Init sets analyzer/loopback, GPIO, allocates MOs, configures bit timing, and
       leaves the node out of INIT. (See DAVE APP implementation.) */
    if (CAN_NODE_Init(h) != CAN_NODE_STATUS_SUCCESS)
    {
        return CAN_SVC_ERR_HW;
    }

    /* If you need to override baud at runtime, do it once here (not each TX/RX). */
    XMC_CAN_NODE_SetInitBit(h->node_ptr);
    CAN_NODE_ConfigBaudrate(h, baud_hz, sample_point_percent, sjw_tq);
    XMC_CAN_NODE_ResetInitBit(h->node_ptr);

    g_can_inited = true;
    return CAN_SVC_OK;
}

can_svc_status_t CAN_Service_Tx(const can_frame_t *frame)
{
    if ((g_can_inited == false) || (frame == NULL))
    {
        return CAN_SVC_ERR_PARAM;
    }

    if ((frame->dlc > CAN_MAX_DLC) || ((frame->id & 0xFFFFF800U) != 0U))
    {
        return CAN_SVC_ERR_PARAM; /* reject invalid DLC or non-11bit ID */
    }

    const CAN_NODE_t * const h = &CAN_NODE_0;
    XMC_CAN_MO_t * const mo = h->lmobj_ptr[0]->mo_ptr; /* TX MO */

    mo->can_mo_type     = XMC_CAN_MO_TYPE_TRANSMSGOBJ;
    mo->can_id_mode     = XMC_CAN_FRAME_TYPE_STANDARD_11BITS;
    mo->can_identifier  = (uint32_t)frame->id;    /* ✅ runtime CAN ID */
    mo->can_data_length = frame->dlc;             /* ✅ runtime DLC */
    mo->can_id_mask = 0x7FF;
    mo->can_priority    = XMC_CAN_ARBITRATION_MODE_IDE_DIR_BASED_PRIO_2;

    CAN_NODE_MO_Init(h->lmobj_ptr[0]);
    uint8_t temp[CAN_MAX_DLC] = {0U};
    (void)memcpy(temp, frame->data, frame->dlc);
    (void)CAN_NODE_MO_UpdateData(h->lmobj_ptr[0], temp);

    if (CAN_NODE_MO_Transmit(h->lmobj_ptr[0]) != CAN_NODE_STATUS_SUCCESS)
    {
        return CAN_SVC_ERR_HW;
    }

    return CAN_SVC_OK;
}


can_svc_status_t CAN_Service_Rx(can_frame_t *out_frame)
{
    if ((g_can_inited == false) || (out_frame == NULL))
    {
        return CAN_SVC_ERR_PARAM;
    }

    CAN_NODE_LMO_t * const rx_lmo = CAN_NODE_0.lmobj_ptr[1]; /* RX MO configured in DAVE */
    XMC_CAN_MO_t * const mo = rx_lmo->mo_ptr;
    CAN_NODE_MO_Init(rx_lmo);
    /* Attempt to fetch any pending frame into the MO struct */
    if (CAN_NODE_MO_Receive(rx_lmo) != CAN_NODE_STATUS_SUCCESS)
    {
        return CAN_SVC_NO_DATA;
    }

    /* Read meta and data */
    out_frame->id  = (uint16_t)(mo->can_identifier & 0x7FFU);
    out_frame->dlc = (uint8_t)mo->can_data_length;

    if (prv_check_dlc(out_frame->dlc) == false)
    {
        /* Defensive: clear any weird value and report error */
        (void)memset(out_frame, 0, sizeof(*out_frame));
        return CAN_SVC_ERR_HW;
    }

    for (uint8_t i = 0U; i < out_frame->dlc; ++i)
    {
        out_frame->data[i] = mo->can_data_byte[i];
    }

    return CAN_SVC_OK;
}

uint8_t CAN_Service_GetRxErrCnt(void)
{
    return XMC_CAN_NODE_GetReceiveErrorCounter(CAN_NODE_0.node_ptr);
}

uint8_t CAN_Service_GetTxErrCnt(void)
{
    return XMC_CAN_NODE_GetTransmitErrorCounter(CAN_NODE_0.node_ptr);
}

uint32_t CAN_Service_GetNodeStatus(void)
{
    return CAN_NODE_GetStatus(&CAN_NODE_0);
}
