/*
 * adBms6830CmdList.c
 *
 *  Created on: Aug 20, 2025
 *      Author: akil
 */

#include "bms_cmdlist.h"
#include "adBms6830CmdList.h"
#include "DAVE.h"

/* All command bytes are immutable and file-local. */
 uint8_t ADBMS_WRCFGA[2]  = { 0x00U, 0x01U };
 uint8_t ADBMS_WRCFGB[2]  = { 0x00U, 0x24U };
 uint8_t ADBMS_RDCFGA[2]  = { 0x00U, 0x02U };
 uint8_t ADBMS_RDCFGB[2]  = { 0x00U, 0x26U };


/* Read cell voltage result registers commands */
 uint8_t RDCVA[2]         = { 0x00U, 0x04U };
 uint8_t RDCVB[2]         = { 0x00U, 0x06U };
 uint8_t RDCVC[2]         = { 0x00U, 0x08U };
 uint8_t RDCVD[2]         = { 0x00U, 0x0AU };
 uint8_t RDCVE[2]         = { 0x00U, 0x09U };
 uint8_t RDCVF[2]         = { 0x00U, 0x0BU };
 uint8_t RDCVALL[2]       = { 0x00U, 0x0CU };

/* Read average cell voltage result registers commands commands */
 uint8_t RDACA[2]         = { 0x00U, 0x44U };
 uint8_t RDACB[2]         = { 0x00U, 0x46U };
 uint8_t RDACC[2]         = { 0x00U, 0x48U };
 uint8_t RDACD[2]         = { 0x00U, 0x4AU };
 uint8_t RDACE[2]         = { 0x00U, 0x49U };
 uint8_t RDACF[2]         = { 0x00U, 0x4BU };
 uint8_t RDACALL[2]       = { 0x00U, 0x4CU };

/* Read s voltage result registers commands */
 uint8_t RDSVA[2]         = { 0x00U, 0x03U };
 uint8_t RDSVB[2]         = { 0x00U, 0x05U };
 uint8_t RDSVC[2]         = { 0x00U, 0x07U };
 uint8_t RDSVD[2]         = { 0x00U, 0x0DU };
 uint8_t RDSVE[2]         = { 0x00U, 0x0EU };
 uint8_t RDSVF[2]         = { 0x00U, 0x0FU };
 uint8_t RDSALL[2]        = { 0x00U, 0x10U };

/* Read c and s results */
 uint8_t RDCSALL[2]       = { 0x00U, 0x11U };
 uint8_t RDACSALL[2]      = { 0x00U, 0x51U };

/* Read all AUX and all Status Registers */
 uint8_t RDASALL[2]       = { 0x00U, 0x35U };

/* Read filtered cell voltage result registers*/
 uint8_t RDFCA[2]         = { 0x00U, 0x12U };
 uint8_t RDFCB[2]         = { 0x00U, 0x13U };
 uint8_t RDFCC[2]         = { 0x00U, 0x14U };
 uint8_t RDFCD[2]         = { 0x00U, 0x15U };
 uint8_t RDFCE[2]         = { 0x00U, 0x16U };
 uint8_t RDFCF[2]         = { 0x00U, 0x17U };
 uint8_t RDFCALL[2]       = { 0x00U, 0x18U };

/* Read aux results */
 uint8_t RDAUXA[2]        = { 0x00U, 0x19U };
 uint8_t RDAUXB[2]        = { 0x00U, 0x1AU };
//  uint8_t RDAUXC[2]        = { 0x00U, 0x1BU };
//  uint8_t RDAUXD[2]        = { 0x00U, 0x1FU };

/* Read redundant aux results */
 uint8_t RDRAXA[2]        = { 0x00U, 0x1C };
 uint8_t RDRAXB[2]        = { 0x00U, 0x1D };
 uint8_t RDRAXC[2]        = { 0x00U, 0x1E };
 uint8_t RDRAXD[2]        = { 0x00U, 0x25 };

/* Read status registers */
//  uint8_t RDSTATA[2]       = { 0x00U, 0x30U };
//  uint8_t RDSTATB[2]       = { 0x00U, 0x31U };
//  uint8_t RDSTATC[2]       = { 0x00U, 0x32U };
//  uint8_t RDSTATCERR[2]    = { 0x00U, 0x72U };              /* ERR */
//  uint8_t RDSTATD[2]       = { 0x00U, 0x33U };
//  uint8_t RDSTATE[2]       = { 0x00U, 0x34U };

/* Pwm registers commands */
 uint8_t WRPWM1[2]        = { 0x00U, 0x20U };
 uint8_t RDPWM1[2]        = { 0x00U, 0x22U };

 uint8_t WRPWM2[2]        = { 0x00U, 0x21U };
 uint8_t RDPWM2[2]        = { 0x00U, 0x23U };

/* Clear commands */
 uint8_t CLRCELL[2]       = { 0x07U, 0x11U };
 uint8_t CLRAUX [2]       = { 0x07U, 0x12U };
 uint8_t CLRSPIN[2]       = { 0x07U, 0x16U };
 uint8_t CLRFLAG[2]       = { 0x07U, 0x17U };
 uint8_t CLRFC[2]         = { 0x07U, 0x14U };
 uint8_t CLOVUV[2]        = { 0x07U, 0x15U };

/* Poll adc command */
 uint8_t PLADC[2]         = { 0x07U, 0x18U };
 uint8_t PLAUT[2]         = { 0x07U, 0x19U };
 uint8_t PLCADC[2]        = { 0x07U, 0x1CU };
 uint8_t PLSADC[2]        = { 0x07U, 0x1DU };
 uint8_t PLAUX1[2]        = { 0x07U, 0x1EU };
 uint8_t PLAUX2[2]        = { 0x07U, 0x1FU };

/* Diagn command */
 uint8_t DIAGN[2]         = {0x07 , 0x15U};

/* GPIOs Comm commands */
 uint8_t WRCOMM[2]        = { 0x07U, 0x21U };
 uint8_t RDCOMM[2]        = { 0x07U, 0x22U };
 uint8_t STCOMM[13]       = { 0x07U, 0x23U, 0xB9U, 0xE4U ,0x00U ,0x00U ,0x00U ,0x00U ,0x00U ,0x00U ,0x00U ,0x00U ,0x00U};

/* Mute and Unmute commands */
 uint8_t MUTE[2] 	       = { 0x00U, 0x28U };
 uint8_t UNMUTE[2]        = { 0x00U, 0x29U };

 uint8_t RSTCC[2]         = { 0x00U, 0x2EU };
 uint8_t SNAP[2]          = { 0x00U, 0x2DU };
 uint8_t UNSNAP[2]        = { 0x00U, 0x2FU };
 uint8_t SRST[2]          = { 0x00U, 0x27U };

/* Read SID command */
 uint8_t RDSID[2]         = { 0x00U, 0x2CU };
