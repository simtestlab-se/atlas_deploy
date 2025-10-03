/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
*/

/**
* \addtogroup XMC_ESC XMC_ESC
* @{
*/

/**
\file XMC_ESCObjects
\author ET9300Utilities.ApplicationHandler (Version 1.6.4.0) | EthercatSSC@beckhoff.com

\brief XMC_ESC specific objects<br>
\brief NOTE : This file will be overwritten if a new object dictionary is generated!<br>
*/

#if defined(_XMC__ESC_) && (_XMC__ESC_ == 1)
#define PROTO
#else
#define PROTO extern
#endif
/******************************************************************************
*                    Object 0x1600 : Output mapping 0
******************************************************************************/
/**
* \addtogroup 0x1600 0x1600 | Output mapping 0
* @{
* \brief Object 0x1600 (Output mapping 0) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Reference to 0x7000.1<br>
* SubIndex 2 - Reference to 0x7001.1<br>
* SubIndex 3 - Reference to 0x7001.2<br>
* SubIndex 4 - Reference to 0x7001.3<br>
* SubIndex 5 - Padding entry (4Bit)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1600[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex1 - Reference to 0x7000.1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex2 - Reference to 0x7001.1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex3 - Reference to 0x7001.2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex4 - Reference to 0x7001.3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }}; /* Subindex5 - Padding entry (4Bit) */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1600[] = "Output mapping 0\000"
"SubIndex 001\000"
"SubIndex 002\000"
"SubIndex 003\000"
"SubIndex 004\000"
"SubIndex 005\000\377";
#endif //#ifdef _OBJD_

#ifndef _XMC__ESC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 - Reference to 0x7000.1 */
UINT32 SI2; /* Subindex2 - Reference to 0x7001.1 */
UINT32 SI3; /* Subindex3 - Reference to 0x7001.2 */
UINT32 SI4; /* Subindex4 - Reference to 0x7001.3 */
UINT32 SI5; /* Subindex5 - Padding entry (4Bit) */
} OBJ_STRUCT_PACKED_END
TOBJ1600;
#endif //#ifndef _XMC__ESC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1600 OutputMapping00x1600
#if defined(_XMC__ESC_) && (_XMC__ESC_ == 1)
={5,0x70000101,0x70010101,0x70010201,0x70010301,0x00000004}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1A00 : Input mapping 0
******************************************************************************/
/**
* \addtogroup 0x1A00 0x1A00 | Input mapping 0
* @{
* \brief Object 0x1A00 (Input mapping 0) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Reference to 0x6000.1<br>
* SubIndex 2 - Reference to 0x6000.2<br>
* SubIndex 3 - Reference to 0x6000.3<br>
* SubIndex 4 - Reference to 0x6000.4<br>
* SubIndex 5 - Reference to 0x6000.5<br>
* SubIndex 6 - Reference to 0x6000.6<br>
* SubIndex 7 - Reference to 0x6000.7<br>
* SubIndex 8 - Reference to 0x6000.8<br>
* SubIndex 9 - Reference to 0x6000.9<br>
* SubIndex 10 - Reference to 0x6000.10<br>
* SubIndex 11 - Reference to 0x6000.11<br>
* SubIndex 12 - Reference to 0x6000.12<br>
* SubIndex 13 - Reference to 0x6001.1<br>
* SubIndex 14 - Reference to 0x6002.1<br>
* SubIndex 15 - Reference to 0x6003.1<br>
* SubIndex 16 - Reference to 0x6004.1<br>
* SubIndex 17 - Reference to 0x6004.2<br>
* SubIndex 18 - Reference to 0x6004.3<br>
* SubIndex 19 - Reference to 0x6004.4<br>
* SubIndex 20 - Reference to 0x6004.5<br>
* SubIndex 21 - Reference to 0x6004.6<br>
* SubIndex 22 - Reference to 0x6004.7<br>
* SubIndex 23 - Reference to 0x6004.8<br>
* SubIndex 24 - Reference to 0x6004.9<br>
* SubIndex 25 - Reference to 0x6004.10<br>
* SubIndex 26 - Reference to 0x6004.11<br>
* SubIndex 27 - Reference to 0x6004.12<br>
* SubIndex 28 - Reference to 0x6005.1<br>
* SubIndex 29 - Reference to 0x6005.2<br>
* SubIndex 30 - Reference to 0x6005.3<br>
* SubIndex 31 - Reference to 0x6006.1<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1A00[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex1 - Reference to 0x6000.1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex2 - Reference to 0x6000.2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex3 - Reference to 0x6000.3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex4 - Reference to 0x6000.4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex5 - Reference to 0x6000.5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex6 - Reference to 0x6000.6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex7 - Reference to 0x6000.7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex8 - Reference to 0x6000.8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex9 - Reference to 0x6000.9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex10 - Reference to 0x6000.10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex11 - Reference to 0x6000.11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex12 - Reference to 0x6000.12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex13 - Reference to 0x6001.1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex14 - Reference to 0x6002.1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex15 - Reference to 0x6003.1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex16 - Reference to 0x6004.1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex17 - Reference to 0x6004.2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex18 - Reference to 0x6004.3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex19 - Reference to 0x6004.4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex20 - Reference to 0x6004.5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex21 - Reference to 0x6004.6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex22 - Reference to 0x6004.7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex23 - Reference to 0x6004.8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex24 - Reference to 0x6004.9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex25 - Reference to 0x6004.10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex26 - Reference to 0x6004.11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex27 - Reference to 0x6004.12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex28 - Reference to 0x6005.1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex29 - Reference to 0x6005.2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex30 - Reference to 0x6005.3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }}; /* Subindex31 - Reference to 0x6006.1 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1A00[] = "Input mapping 0\000"
"SubIndex 001\000"
"SubIndex 002\000"
"SubIndex 003\000"
"SubIndex 004\000"
"SubIndex 005\000"
"SubIndex 006\000"
"SubIndex 007\000"
"SubIndex 008\000"
"SubIndex 009\000"
"SubIndex 010\000"
"SubIndex 011\000"
"SubIndex 012\000"
"SubIndex 013\000"
"SubIndex 014\000"
"SubIndex 015\000"
"SubIndex 016\000"
"SubIndex 017\000"
"SubIndex 018\000"
"SubIndex 019\000"
"SubIndex 020\000"
"SubIndex 021\000"
"SubIndex 022\000"
"SubIndex 023\000"
"SubIndex 024\000"
"SubIndex 025\000"
"SubIndex 026\000"
"SubIndex 027\000"
"SubIndex 028\000"
"SubIndex 029\000"
"SubIndex 030\000"
"SubIndex 031\000\377";
#endif //#ifdef _OBJD_

#ifndef _XMC__ESC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 - Reference to 0x6000.1 */
UINT32 SI2; /* Subindex2 - Reference to 0x6000.2 */
UINT32 SI3; /* Subindex3 - Reference to 0x6000.3 */
UINT32 SI4; /* Subindex4 - Reference to 0x6000.4 */
UINT32 SI5; /* Subindex5 - Reference to 0x6000.5 */
UINT32 SI6; /* Subindex6 - Reference to 0x6000.6 */
UINT32 SI7; /* Subindex7 - Reference to 0x6000.7 */
UINT32 SI8; /* Subindex8 - Reference to 0x6000.8 */
UINT32 SI9; /* Subindex9 - Reference to 0x6000.9 */
UINT32 SI10; /* Subindex10 - Reference to 0x6000.10 */
UINT32 SI11; /* Subindex11 - Reference to 0x6000.11 */
UINT32 SI12; /* Subindex12 - Reference to 0x6000.12 */
UINT32 SI13; /* Subindex13 - Reference to 0x6001.1 */
UINT32 SI14; /* Subindex14 - Reference to 0x6002.1 */
UINT32 SI15; /* Subindex15 - Reference to 0x6003.1 */
UINT32 SI16; /* Subindex16 - Reference to 0x6004.1 */
UINT32 SI17; /* Subindex17 - Reference to 0x6004.2 */
UINT32 SI18; /* Subindex18 - Reference to 0x6004.3 */
UINT32 SI19; /* Subindex19 - Reference to 0x6004.4 */
UINT32 SI20; /* Subindex20 - Reference to 0x6004.5 */
UINT32 SI21; /* Subindex21 - Reference to 0x6004.6 */
UINT32 SI22; /* Subindex22 - Reference to 0x6004.7 */
UINT32 SI23; /* Subindex23 - Reference to 0x6004.8 */
UINT32 SI24; /* Subindex24 - Reference to 0x6004.9 */
UINT32 SI25; /* Subindex25 - Reference to 0x6004.10 */
UINT32 SI26; /* Subindex26 - Reference to 0x6004.11 */
UINT32 SI27; /* Subindex27 - Reference to 0x6004.12 */
UINT32 SI28; /* Subindex28 - Reference to 0x6005.1 */
UINT32 SI29; /* Subindex29 - Reference to 0x6005.2 */
UINT32 SI30; /* Subindex30 - Reference to 0x6005.3 */
UINT32 SI31; /* Subindex31 - Reference to 0x6006.1 */
} OBJ_STRUCT_PACKED_END
TOBJ1A00;
#endif //#ifndef _XMC__ESC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1A00 InputMapping00x1A00
#if defined(_XMC__ESC_) && (_XMC__ESC_ == 1)
={31,0x60000110,0x60000210,0x60000310,0x60000410,0x60000510,0x60000610,0x60000710,0x60000810,0x60000910,0x60000A10,0x60000B10,0x60000C10,0x60010120,0x60020110,0x60030110,0x60040120,0x60040220,0x60040320,0x60040420,0x60040520,0x60040620,0x60040720,0x60040820,0x60040920,0x60040A20,0x60040B20,0x60040C20,0x60050120,0x60050220,0x60050320,0x60060120}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1C12 : SyncManager 2 assignment
******************************************************************************/
/**
* \addtogroup 0x1C12 0x1C12 | SyncManager 2 assignment
* @{
* \brief Object 0x1C12 (SyncManager 2 assignment) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1C12[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x1C12[] = "SyncManager 2 assignment\000\377";
#endif //#ifdef _OBJD_

#ifndef _XMC__ESC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[1];  /**< \brief Subindex 1 - 1 */
} OBJ_STRUCT_PACKED_END
TOBJ1C12;
#endif //#ifndef _XMC__ESC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1C12 sRxPDOassign
#if defined(_XMC__ESC_) && (_XMC__ESC_ == 1)
={1,{0x1600}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1C13 : SyncManager 3 assignment
******************************************************************************/
/**
* \addtogroup 0x1C13 0x1C13 | SyncManager 3 assignment
* @{
* \brief Object 0x1C13 (SyncManager 3 assignment) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1C13[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x1C13[] = "SyncManager 3 assignment\000\377";
#endif //#ifdef _OBJD_

#ifndef _XMC__ESC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[1];  /**< \brief Subindex 1 - 1 */
} OBJ_STRUCT_PACKED_END
TOBJ1C13;
#endif //#ifndef _XMC__ESC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1C13 sTxPDOassign
#if defined(_XMC__ESC_) && (_XMC__ESC_ == 1)
={1,{0x1A00}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6000 : CELL_VOLTAGE
******************************************************************************/
/**
* \addtogroup 0x6000 0x6000 | CELL_VOLTAGE
* @{
* \brief Object 0x6000 (CELL_VOLTAGE) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - CELL_001<br>
* SubIndex 2 - CELL_002<br>
* SubIndex 3 - CELL_003<br>
* SubIndex 4 - CELL_004<br>
* SubIndex 5 - CELL_005<br>
* SubIndex 6 - CELL_006<br>
* SubIndex 7 - CELL_007<br>
* SubIndex 8 - CELL_008<br>
* SubIndex 9 - CELL_009<br>
* SubIndex 10 - CELL_010<br>
* SubIndex 11 - CELL_011<br>
* SubIndex 12 - CELL_012<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6000[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_INTEGER16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex1 - CELL_001 */
{ DEFTYPE_INTEGER16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex2 - CELL_002 */
{ DEFTYPE_INTEGER16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex3 - CELL_003 */
{ DEFTYPE_INTEGER16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex4 - CELL_004 */
{ DEFTYPE_INTEGER16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex5 - CELL_005 */
{ DEFTYPE_INTEGER16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex6 - CELL_006 */
{ DEFTYPE_INTEGER16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex7 - CELL_007 */
{ DEFTYPE_INTEGER16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex8 - CELL_008 */
{ DEFTYPE_INTEGER16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex9 - CELL_009 */
{ DEFTYPE_INTEGER16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex10 - CELL_010 */
{ DEFTYPE_INTEGER16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex11 - CELL_011 */
{ DEFTYPE_INTEGER16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }}; /* Subindex12 - CELL_012 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x6000[] = "CELL_VOLTAGE\000"
"CELL_001\000"
"CELL_002\000"
"CELL_003\000"
"CELL_004\000"
"CELL_005\000"
"CELL_006\000"
"CELL_007\000"
"CELL_008\000"
"CELL_009\000"
"CELL_010\000"
"CELL_011\000"
"CELL_012\000\377";
#endif //#ifdef _OBJD_

#ifndef _XMC__ESC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
INT16 CELL_001; /* Subindex1 - CELL_001 */
INT16 CELL_002; /* Subindex2 - CELL_002 */
INT16 CELL_003; /* Subindex3 - CELL_003 */
INT16 CELL_004; /* Subindex4 - CELL_004 */
INT16 CELL_005; /* Subindex5 - CELL_005 */
INT16 CELL_006; /* Subindex6 - CELL_006 */
INT16 CELL_007; /* Subindex7 - CELL_007 */
INT16 CELL_008; /* Subindex8 - CELL_008 */
INT16 CELL_009; /* Subindex9 - CELL_009 */
INT16 CELL_010; /* Subindex10 - CELL_010 */
INT16 CELL_011; /* Subindex11 - CELL_011 */
INT16 CELL_012; /* Subindex12 - CELL_012 */
} OBJ_STRUCT_PACKED_END
TOBJ6000;
#endif //#ifndef _XMC__ESC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6000 CELL_VOLTAGE0x6000
#if defined(_XMC__ESC_) && (_XMC__ESC_ == 1)
={12,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6001 : CELL_CURRENT_DC
******************************************************************************/
/**
* \addtogroup 0x6001 0x6001 | CELL_CURRENT_DC
* @{
* \brief Object 0x6001 (CELL_CURRENT_DC) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - CELL_CURRENT<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6001[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }}; /* Subindex1 - CELL_CURRENT */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x6001[] = "CELL_CURRENT_DC\000"
"CELL_CURRENT\000\377";
#endif //#ifdef _OBJD_

#ifndef _XMC__ESC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
REAL32 CELL_CURRENT; /* Subindex1 - CELL_CURRENT */
} OBJ_STRUCT_PACKED_END
TOBJ6001;
#endif //#ifndef _XMC__ESC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6001 CELL_CURRENT_DC0x6001
#if defined(_XMC__ESC_) && (_XMC__ESC_ == 1)
={1,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6002 : CELL_TEMPERATURE
******************************************************************************/
/**
* \addtogroup 0x6002 0x6002 | CELL_TEMPERATURE
* @{
* \brief Object 0x6002 (CELL_TEMPERATURE) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - TEMP_001<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6002[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_INTEGER16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }}; /* Subindex1 - TEMP_001 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x6002[] = "CELL_TEMPERATURE\000"
"TEMP_001\000\377";
#endif //#ifdef _OBJD_

#ifndef _XMC__ESC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
INT16 TEMP_001; /* Subindex1 - TEMP_001 */
} OBJ_STRUCT_PACKED_END
TOBJ6002;
#endif //#ifndef _XMC__ESC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6002 CELL_TEMPERATURE0x6002
#if defined(_XMC__ESC_) && (_XMC__ESC_ == 1)
={1,0x0000}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6003 : LINK_VOLTAGE_DC
******************************************************************************/
/**
* \addtogroup 0x6003 0x6003 | LINK_VOLTAGE_DC
* @{
* \brief Object 0x6003 (LINK_VOLTAGE_DC) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - LINK_VOLTAGE<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6003[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }}; /* Subindex1 - LINK_VOLTAGE */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x6003[] = "LINK_VOLTAGE_DC\000"
"LINK_VOLTAGE\000\377";
#endif //#ifdef _OBJD_

#ifndef _XMC__ESC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT16 LINK_VOLTAGE; /* Subindex1 - LINK_VOLTAGE */
} OBJ_STRUCT_PACKED_END
TOBJ6003;
#endif //#ifndef _XMC__ESC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6003 LINK_VOLTAGE_DC0x6003
#if defined(_XMC__ESC_) && (_XMC__ESC_ == 1)
={1,0x0000}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6004 : SOC_ALGO
******************************************************************************/
/**
* \addtogroup 0x6004 0x6004 | SOC_ALGO
* @{
* \brief Object 0x6004 (SOC_ALGO) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - SOC_CELL_001<br>
* SubIndex 2 - SOC_CELL_002<br>
* SubIndex 3 - SOC_CELL_003<br>
* SubIndex 4 - SOC_CELL_004<br>
* SubIndex 5 - SOC_CELL_005<br>
* SubIndex 6 - SOC_CELL_006<br>
* SubIndex 7 - SOC_CELL_007<br>
* SubIndex 8 - SOC_CELL_008<br>
* SubIndex 9 - SOC_CELL_009<br>
* SubIndex 10 - SOC_CELL_010<br>
* SubIndex 11 - SOC_CELL_011<br>
* SubIndex 12 - SOC_CELL_012<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6004[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex1 - SOC_CELL_001 */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex2 - SOC_CELL_002 */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex3 - SOC_CELL_003 */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex4 - SOC_CELL_004 */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex5 - SOC_CELL_005 */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex6 - SOC_CELL_006 */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex7 - SOC_CELL_007 */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex8 - SOC_CELL_008 */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex9 - SOC_CELL_009 */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex10 - SOC_CELL_010 */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex11 - SOC_CELL_011 */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }}; /* Subindex12 - SOC_CELL_012 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x6004[] = "SOC_ALGO\000"
"SOC_CELL_001\000"
"SOC_CELL_002\000"
"SOC_CELL_003\000"
"SOC_CELL_004\000"
"SOC_CELL_005\000"
"SOC_CELL_006\000"
"SOC_CELL_007\000"
"SOC_CELL_008\000"
"SOC_CELL_009\000"
"SOC_CELL_010\000"
"SOC_CELL_011\000"
"SOC_CELL_012\000\377";
#endif //#ifdef _OBJD_

#ifndef _XMC__ESC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
REAL32 SOC_CELL_001; /* Subindex1 - SOC_CELL_001 */
REAL32 SOC_CELL_002; /* Subindex2 - SOC_CELL_002 */
REAL32 SOC_CELL_003; /* Subindex3 - SOC_CELL_003 */
REAL32 SOC_CELL_004; /* Subindex4 - SOC_CELL_004 */
REAL32 SOC_CELL_005; /* Subindex5 - SOC_CELL_005 */
REAL32 SOC_CELL_006; /* Subindex6 - SOC_CELL_006 */
REAL32 SOC_CELL_007; /* Subindex7 - SOC_CELL_007 */
REAL32 SOC_CELL_008; /* Subindex8 - SOC_CELL_008 */
REAL32 SOC_CELL_009; /* Subindex9 - SOC_CELL_009 */
REAL32 SOC_CELL_010; /* Subindex10 - SOC_CELL_010 */
REAL32 SOC_CELL_011; /* Subindex11 - SOC_CELL_011 */
REAL32 SOC_CELL_012; /* Subindex12 - SOC_CELL_012 */
} OBJ_STRUCT_PACKED_END
TOBJ6004;
#endif //#ifndef _XMC__ESC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6004 SOC_ALGO0x6004
#if defined(_XMC__ESC_) && (_XMC__ESC_ == 1)
={12,0,0,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6005 : SOC_THRESHOLD
******************************************************************************/
/**
* \addtogroup 0x6005 0x6005 | SOC_THRESHOLD
* @{
* \brief Object 0x6005 (SOC_THRESHOLD) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - MIN_SOC<br>
* SubIndex 2 - MAX_SOC<br>
* SubIndex 3 - AVERAGE_SOC<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6005[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex1 - MIN_SOC */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex2 - MAX_SOC */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }}; /* Subindex3 - AVERAGE_SOC */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x6005[] = "SOC_THRESHOLD\000"
"MIN_SOC\000"
"MAX_SOC\000"
"AVERAGE_SOC\000\377";
#endif //#ifdef _OBJD_

#ifndef _XMC__ESC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
REAL32 MIN_SOC; /* Subindex1 - MIN_SOC */
REAL32 MAX_SOC; /* Subindex2 - MAX_SOC */
REAL32 AVERAGE_SOC; /* Subindex3 - AVERAGE_SOC */
} OBJ_STRUCT_PACKED_END
TOBJ6005;
#endif //#ifndef _XMC__ESC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6005 SOC_THRESHOLD0x6005
#if defined(_XMC__ESC_) && (_XMC__ESC_ == 1)
={3,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6006 : PACK_VOLTAGE_DC
******************************************************************************/
/**
* \addtogroup 0x6006 0x6006 | PACK_VOLTAGE_DC
* @{
* \brief Object 0x6006 (PACK_VOLTAGE_DC) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - PACK_VOLTAGE<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6006[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }}; /* Subindex1 - PACK_VOLTAGE */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x6006[] = "PACK_VOLTAGE_DC\000"
"PACK_VOLTAGE\000\377";
#endif //#ifdef _OBJD_

#ifndef _XMC__ESC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
REAL32 PACK_VOLTAGE; /* Subindex1 - PACK_VOLTAGE */
} OBJ_STRUCT_PACKED_END
TOBJ6006;
#endif //#ifndef _XMC__ESC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6006 PACK_VOLTAGE_DC0x6006
#if defined(_XMC__ESC_) && (_XMC__ESC_ == 1)
={1,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x7000 : CELL_BALANCING
******************************************************************************/
/**
* \addtogroup 0x7000 0x7000 | CELL_BALANCING
* @{
* \brief Object 0x7000 (CELL_BALANCING) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - CELL_BAL<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x7000[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING }}; /* Subindex1 - CELL_BAL */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x7000[] = "CELL_BALANCING\000"
"CELL_BAL\000\377";
#endif //#ifdef _OBJD_

#ifndef _XMC__ESC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
BOOLEAN(CELL_BAL); /* Subindex1 - CELL_BAL */
} OBJ_STRUCT_PACKED_END
TOBJ7000;
#endif //#ifndef _XMC__ESC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ7000 CELL_BALANCING0x7000
#if defined(_XMC__ESC_) && (_XMC__ESC_ == 1)
={1,0x00}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x7001 : CONTACTOR_CONTROL
******************************************************************************/
/**
* \addtogroup 0x7001 0x7001 | CONTACTOR_CONTROL
* @{
* \brief Object 0x7001 (CONTACTOR_CONTROL) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - POSITIVE<br>
* SubIndex 2 - NEGATIVE<br>
* SubIndex 3 - PRECHARGE<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x7001[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING }, /* Subindex1 - POSITIVE */
{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING }, /* Subindex2 - NEGATIVE */
{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING }}; /* Subindex3 - PRECHARGE */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x7001[] = "CONTACTOR_CONTROL\000"
"POSITIVE\000"
"NEGATIVE\000"
"PRECHARGE\000\377";
#endif //#ifdef _OBJD_

#ifndef _XMC__ESC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
BOOLEAN(POSITIVE); /* Subindex1 - POSITIVE */
BOOLEAN(NEGATIVE); /* Subindex2 - NEGATIVE */
BOOLEAN(PRECHARGE); /* Subindex3 - PRECHARGE */
} OBJ_STRUCT_PACKED_END
TOBJ7001;
#endif //#ifndef _XMC__ESC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ7001 CONTACTOR_CONTROL0x7001
#if defined(_XMC__ESC_) && (_XMC__ESC_ == 1)
={3,0x00,0x00,0x00}
#endif
;
/** @}*/







#ifdef _OBJD_
TOBJECT    OBJMEM ApplicationObjDic[] = {
/* Object 0x1600 */
{NULL , NULL ,  0x1600 , {DEFTYPE_PDOMAPPING , 5 | (OBJCODE_REC << 8)} , asEntryDesc0x1600 , aName0x1600 , &OutputMapping00x1600 , NULL , NULL , 0x0000 },
/* Object 0x1A00 */
{NULL , NULL ,  0x1A00 , {DEFTYPE_PDOMAPPING , 31 | (OBJCODE_REC << 8)} , asEntryDesc0x1A00 , aName0x1A00 , &InputMapping00x1A00 , NULL , NULL , 0x0000 },
/* Object 0x1C12 */
{NULL , NULL ,  0x1C12 , {DEFTYPE_UNSIGNED16 , 1 | (OBJCODE_ARR << 8)} , asEntryDesc0x1C12 , aName0x1C12 , &sRxPDOassign , NULL , NULL , 0x0000 },
/* Object 0x1C13 */
{NULL , NULL ,  0x1C13 , {DEFTYPE_UNSIGNED16 , 1 | (OBJCODE_ARR << 8)} , asEntryDesc0x1C13 , aName0x1C13 , &sTxPDOassign , NULL , NULL , 0x0000 },
/* Object 0x6000 */
{NULL , NULL ,  0x6000 , {DEFTYPE_RECORD , 12 | (OBJCODE_REC << 8)} , asEntryDesc0x6000 , aName0x6000 , &CELL_VOLTAGE0x6000 , NULL , NULL , 0x0000 },
/* Object 0x6001 */
{NULL , NULL ,  0x6001 , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x6001 , aName0x6001 , &CELL_CURRENT_DC0x6001 , NULL , NULL , 0x0000 },
/* Object 0x6002 */
{NULL , NULL ,  0x6002 , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x6002 , aName0x6002 , &CELL_TEMPERATURE0x6002 , NULL , NULL , 0x0000 },
/* Object 0x6003 */
{NULL , NULL ,  0x6003 , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x6003 , aName0x6003 , &LINK_VOLTAGE_DC0x6003 , NULL , NULL , 0x0000 },
/* Object 0x6004 */
{NULL , NULL ,  0x6004 , {DEFTYPE_RECORD , 12 | (OBJCODE_REC << 8)} , asEntryDesc0x6004 , aName0x6004 , &SOC_ALGO0x6004 , NULL , NULL , 0x0000 },
/* Object 0x6005 */
{NULL , NULL ,  0x6005 , {DEFTYPE_RECORD , 3 | (OBJCODE_REC << 8)} , asEntryDesc0x6005 , aName0x6005 , &SOC_THRESHOLD0x6005 , NULL , NULL , 0x0000 },
/* Object 0x6006 */
{NULL , NULL ,  0x6006 , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x6006 , aName0x6006 , &PACK_VOLTAGE_DC0x6006 , NULL , NULL , 0x0000 },
/* Object 0x7000 */
{NULL , NULL ,  0x7000 , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x7000 , aName0x7000 , &CELL_BALANCING0x7000 , NULL , NULL , 0x0000 },
/* Object 0x7001 */
{NULL , NULL ,  0x7001 , {DEFTYPE_RECORD , 3 | (OBJCODE_REC << 8)} , asEntryDesc0x7001 , aName0x7001 , &CONTACTOR_CONTROL0x7001 , NULL , NULL , 0x0000 },
{NULL,NULL, 0xFFFF, {0, 0}, NULL, NULL, NULL, NULL}};
#endif    //#ifdef _OBJD_

#undef PROTO

/** @}*/
#define _XMC__ESC_OBJECTS_H_
