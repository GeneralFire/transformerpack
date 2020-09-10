/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary    and
  confidential information of Intel Corporation and its suppliers and licensors,
  and is protected by worldwide copyright and trade secret laws and treaty
  provisions. No part of the Material may be used, copied, reproduced, modified,
  published, uploaded, posted, transmitted, distributed, or disclosed in any way
  without Intel's prior express written permission.

  No license under any patent, copyright, trade secret or other intellectual
  property right is granted to or conferred upon you by disclosure or delivery
  of the Materials, either expressly, by implication, inducement, estoppel or
  otherwise. Any license under such intellectual property rights must be
  express and approved by Intel in writing.

  Unless otherwise agreed by Intel in writing, you may not remove or alter
  this notice or any other notice embedded in Materials by Intel or
  Intel's suppliers or licensors in any way.
**/

#ifndef _DDR4_ODT_STRUCTURES_H_
#define _DDR4_ODT_STRUCTURES_H_

#include <Library/MemoryCoreLib.h>

//
// defines for mem ODT
//
#define  SLOT_NOT_PRESENT  0
#define  EMPTY_DIMM        1
#define  SR_DIMM           2
#define  DR_DIMM           3
#define  QR_DIMM           4
#define  LR_DIMM           5
#define  AEP_DIMM          6

//
// ODT equates used to program mode registers
//
#define  MCODT_50        0
#define  MCODT_100       1

#define  MCODT_ACT       1
#define  ODT_ACT         1
#define  NO_ACT          0

typedef union {
  struct{
    UINT8 rttNom:3;      // Rtt_nom value matching JEDEC spec
    UINT8 rsvd_3:1;
    UINT8 rttWr:2;       // Rtt_park value matching JEDEC spec
    UINT8 rsvd_6:2;
  } Bits;
  UINT8 Data;
} ODT_VALUE_RTT_DDR3;

typedef struct odtValueStruct {
  UINT16 config;                            // ODT_VALUE_TABLE_ENTRY_INDEX
  UINT8  mcOdt;                             // bit[0] = 0 for 50 ohms; 1 for 100 ohms
  //SKX hack
  UINT8  dramOdt[MAX_DIMM3][MAX_RANK_DIMM]; // ODT_VALUE_RTT_DDR3
} ODT_VALUE_STRUCT,*PODT_VALUE_STRUCT;

typedef union {
  struct{
    UINT16 freq:5;  // Frequency enum - DDR 800, 1066, 1333, 1600, 1866, 2133, 2400, 2666, 2933, 3200, etc.
    UINT16 rsvd:2;  // Reserved for now
    UINT16 slot0:3; // DIMM slot 0 enum - Not present, Empty, SR, DR, QR, LR
    UINT16 slot1:3; // DIMM slot 1 enum - Not present, Empty, SR, DR, QR, LR
    UINT16 slot2:3; // DIMM slot 2 enum - Not present, Empty, SR, DR, QR, LR
  } Bits;
  UINT16 Data;
} ODT_VALUE_TABLE_ENTRY_INDEX; // DIMM population is the dominant factor to determine which ODT values must be set (so this data is used to "index" the ODT tables, during table lookups)

typedef union {
  struct{
    UINT16 dimmNum:2; // Target DIMM number on the channel
    UINT16 rankNum:2; // Target Rank number on the DIMM
    UINT16 rsvd:3;
    UINT16 slot0:3;   // DIMM slot 0 enum - Not present, Empty, SR, DR, QR, LR
    UINT16 slot1:3;   // DIMM slot 1 enum - Not present, Empty, SR, DR, QR, LR
    UINT16 slot2:3;   // DIMM slot 2 enum - Not present, Empty, SR, DR, QR, LR
  } Bits;
  UINT16 Data;
} ODT_ACTIVATION_TABLE_ENTRY_INDEX; // DIMM population is the dominant factor to determine which ODT values must be set (so this data is used to "index" the ODT tables, during table lookups)

typedef struct ddrOdtActivationTableEntry {
  UINT16 tgtDimmRankPopData;  // ODT_ACTIVATION_TABLE_ENTRY_INDEX
  UINT16 actBits[2];          // Bits[3:0]  = D0_R[3:0]
                              // Bits[7:4]  = D1_R[3:0]
                              // Bits[11:8] = D2_R[3:0]
                              // Bit[15] = MCODT
} ODT_ACT_STRUCT, **PPODT_ACT_STRUCT;

//From RCD JEDEC SPEC, Output Driver Strengths

#define DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS 0  // RZQ/12 (20 Ohm) driver strength
#define DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS 1  // RZQ/17 (14 Ohm) driver strength
#define DDR5_RCD_OUTPUT_DRIVE_STRENGTH_10_OHMS 2  // RZQ/24 (10 Ohm) driver strength

typedef struct _DDR_BACKSIDE_ODT_VALUE_TABLE_ENTRY {
  UINT32 FreqRawCardAndVrefTypeData;      // 32 bits as defined by ODT_BACKSIDE_VALUE_TABLE_ENTRY_INDEX;
  UINT16 PerDramOdtValues;                // 16 bits as defined by ODT_BACKSIDE_POSITION_TABLE_VALUE;
  INT8   VrefValueTimesTwo;               // Percent Vref value * 2
  UINT8  RcdOutputDriveStrength;          // DDR5_RCD_OUTPUT_DRIVE_STRENGTH_xx_OHMS
} DDR_BACKSIDE_ODT_VALUE_TABLE_ENTRY;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef DDR5_SUPPORT
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef union {
  struct{
    UINT16 rttNomWr:3; // Rtt_Nom_Wr (encoding of these values is defined by JEDEC)
    UINT16 rttNomRd:3; // Rtt_Nom_Rd
    UINT16 rttPrk:3;   // Rtt_park
    UINT16 rttWr:3;    // Rtt_Wr
    UINT16 dqsRttPrk:3;
    UINT16 reserved:1;
  } Bits;
  UINT16 Data;
} ODT_VALUE_TABLE_ENTRY_RTT;

//
// For DDR5, store percentages multiplied by two to support 0.5% precision.
//
#define ODT_TABLE_STORAGE_VREF_PERCENT_FACTOR 2

typedef struct ddrOdtValueTableEntry {
  UINT16 freqPopData;                     // 16 bits as defined by ODT_VALUE_TABLE_ENTRY_INDEX
  UINT8  DramOrDbHostRon;                 // RDIMM DRAM Ron or LRDIMM DB Host Ron (ohm)
  UINT8  mcOdt;                           // bit[0] = 0 for 50 ohms; 1 for 100 ohms
  UINT8  mcVref;                          // Percent Vddq formula by MC spec
                                          // ReadVrefSel = (percent * 192 / Vdd) - 128
  UINT8  dramVref[MAX_DIMM];              // Percent Vddq as defined by JEDEC spec
  UINT8  dramIbtCa[MAX_DIMM];             // DDR5: Ibt CA as provided by KIT (encoding of these values is defined by JEDEC)
  UINT8  dramCaVref[MAX_DIMM];            // DDR5: % CA Vref
  UINT8  dramIbtCs[MAX_DIMM];             // DDR5: Ibt CS
  UINT8  dramCsVref[MAX_DIMM];            // DDR5: % CS Vref
  UINT8  dramIbtClk[MAX_DIMM];            // DDR5: Ibt CLK
  UINT16 dramRttData[MAX_DIMM][MAX_RANK_DIMM]; // 16 bits as defined by ODT_VALUE_TABLE_ENTRY_RTT
} DDR_ODT_VALUE_TABLE_ENTRY;

typedef enum {
  QCS_VREF = 0,                           // Vref associated with QCS signals
  QCA_VREF,                               // Vref associated with QCA signals
  QCK_VREF                                // Vref associated with QCK signals
} BACKSIDE_VREF_TYPE;

typedef union {
  struct{
    UINT32 DimmType:5;                    // RDIMM, LRDIMM
    UINT32 RawCard:5;                     // RAW_CARD_A, RAW_CARD_B, etc..
    UINT32 RawCardRev:3;                  // Raw Card Revision
    UINT32 Frequency:5;                   // Frequency enum - DDR 800, 1066, 1333, 1600, 1866, 2133, 2400, 2666, 2933, 3200, etc.
    UINT32 VrefType:4;                    // Vref Type defined by BACKSIDE_VREF_TYPE
    UINT32 Reserved:10;                    // Reserved for future use
  } Bits;
  UINT32 Data;
} ODT_BACKSIDE_VALUE_TABLE_ENTRY_INDEX; // this data is used to "index" the ODT tables, during table lookups

typedef union {
  struct{
    UINT16 P1:3;                          // Position 1
    UINT16 P2:3;                          // Position 2
    UINT16 P3:3;                          // Position 3
    UINT16 P4:3;                          // Position 4
    UINT16 P5:3;                          // Position 5
    UINT16 Reserved:1;                    // Reserved for future use
  } Bits;
  UINT16 Data;
} ODT_BACKSIDE_POSITION_TABLE_VALUE;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else // Data structures for DDR4 ODT table entries
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef union {
  struct{
    UINT16 rttNom:3; // Rtt_nom value matching JEDEC spec
    UINT16 rttPrk:3; // Rtt_park value matching JEDEC spec
    UINT16 rttWr:3;  // Rtt_wr value matching JEDEC spec
    UINT16 reserved:7;
  } Bits;
  UINT16 Data;
} ODT_VALUE_TABLE_ENTRY_RTT;

//
// For DDR4, store exact whole-number percentages.
//
#define ODT_TABLE_STORAGE_VREF_PERCENT_FACTOR 1

typedef struct ddrOdtValueTableEntry {
  UINT16 freqPopData;                      // 16 bits as defined by ODT_VALUE_TABLE_ENTRY_INDEX
  UINT8  mcOdt;                            // bit[0] = 0 for 50 ohms; 1 for 100 ohms
  UINT8  CpuRon;                           // CPU RON
  UINT8  mcVref;                           // Percent Vddq formula by MC spec
                                           // ReadVrefSel = (percent * 192 / Vdd) - 128
  UINT8  dramVref[MAX_DIMM];               // Percent Vddq as defined by JEDEC spec
  UINT16 dramRttData[MAX_DIMM][MAX_RANK_DIMM]; // 16 bits as defined by ODT_VALUE_TABLE_ENTRY_RTT
} DDR_ODT_VALUE_TABLE_ENTRY;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // END ODT table entries Data structures
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
  Return pointer to the ODT activation table.
  The table is an array of ODT activation structures.
  Also returns the size of the array.

  @param [OUT] OdtActTablePtr  - Pointer to caller's ODT table pointer
  @param [OUT] OdtActTableSize - Pointer to caller's ODT table size variable.

  @retval N/A
**/
VOID
GetActivationTableOdt (
  OUT struct ddrOdtActivationTableEntry **OdtActTablePtr,
  OUT UINT32                             *OdtActTableSize
  );

/**
  Return pointer to the ODT value table.
  The table is an array of ODT value structures.
  Also returns the size of the array.

  @param [OUT] OdtValueTablePtr  - Pointer to caller's ODT table pointer
  @param [OUT] OdtValueTableSize - Pointer to caller's ODT table size variable.

  @retval N/A
**/
VOID
GetValuesTableOdt (
  OUT struct ddrOdtValueTableEntry **OdtValueTablePtr,
  OUT UINT16                        *OdtValueTableSize
  );

/**
  Return pointer to the Backside ODT value table.
  The table is an array of ODT value structures.
  Also returns the size of the array.

  @param [OUT] OdtValueTablePtr  - Pointer to caller's ODT table pointer
  @param [OUT] OdtValueTableSize - Pointer to caller's ODT table size variable.

  @retval N/A
**/
VOID
GetBacksideValuesTableOdt (
  OUT DDR_BACKSIDE_ODT_VALUE_TABLE_ENTRY **OdtValueTablePtr,
  OUT UINT16                             *OdtValueTableSize
  );

/**
  Return the correct slot designator for LRDIMM and AEP DIMM when walking
  the ODT activation table

  @param [IN] AepIsPresent  - 1 if AEP DIMM is present, 0 otherwise

  @retval LR_DIMM or AEP_DIMM
**/
UINT8
GetSlotTypeForActivationLookupOdt (
  IN UINT8 AepIsPresent
  );

/**
  Get the current DIMM frequency formatted as an "index" for the ODT value table
  (the DIMM slot population fields are set to default values)

  @param [IN] Socket  - Socket number

  @retval The current DIMM frequency formatted as an "index" for the ODT value table
  (the DIMM slot population fields are set to default values)
**/
UINT16
GetCurrentDimmFrequencyOdt (
  IN UINT8 Socket
  );

#endif //ifndef _DDR4_ODT_STRUCTURES_H_
