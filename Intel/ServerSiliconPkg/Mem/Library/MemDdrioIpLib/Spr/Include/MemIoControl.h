/** @file
  MemIoControl.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#ifndef  __MEM_IO_CONTROL_H__
#define  __MEM_IO_CONTROL_H__

#include <ReferenceCodeDataTypes.h>
#include "SysHost.h"
#include <Library/VictimAggresorTableLib.h>

#define SUBCH_OFFSET(SUBCH, OFFSET) (OFFSET - ((SUBCH*2) << 8))

#define STROBE_INDEX_SUBCH(STROBE, SUBCH) (STROBE + SUBCH * (MAX_STROBE_DDR5 / 2))
//
// Adjust for nibble/strobe register addresses bits [15:8] not increasing monotonically
// with strobe#. Address bits [15:8] increase with the strobe number from 0 to 9 and step
// size is 2, then reset at strobe 10 (strob 0 of sub channel B) to a value that is 2*10
// less than that of sub channel A strobe 0.
// SUB_CH:      SUB_CH_A - Strobe 0 to 9
//              SUB_CH_B - Strobe 10 to 19
// STROBE:      Strobe index (0 to 9) within each sub channel.
// BASE_OFFSET: The address of strobe 0 of SUB_CH_A.
//
#define SUBCH_STROBE_OFFSET(SUB_CH, STROBE, BASE_OFFSET) \
        (BASE_OFFSET - SUB_CH * ((2*10) << 8) + ((STROBE * 2) << 8))

//
// SPR iMC channel has two sub channels.
//
#define SUB_CH_NUMBER             2

//
// private definitions used in this library
//
#define MAX_PHASE_IN_READ_DQS_ADJUSTMENT 87 // 1.375UI, 64 + 64/4 + 64/8 - 1
#define DEFAULT_EMULATION_ROUND_TRIP     42


//
// Defines for DQ/DQS Sense Amp and ODT Duration Constant
//
#define DDR_1867_DURATION_CONSTANT          7
#define DDR_2133_DURATION_CONSTANT          8
#define DDR_2400_DURATION_CONSTANT          8
#define DDR_2667_DURATION_CONSTANT          8
#define DDR_2933_DURATION_CONSTANT          9
#define DDR_3200_DURATION_CONSTANT          9
#define DDR_3600_DURATION_CONSTANT          9
#define DDR_4000_DURATION_CONSTANT          10
#define DDR_4400_DURATION_CONSTANT          10
#define DDR_4800_DURATION_CONSTANT          10

//
// LRDIMM Buffer VrefDq structure
// Each buffer includes a VrefDq register for each DQ
//
typedef struct {
  UINT8   VrefDq[MAX_BITS_IN_BYTE];
} LRBUFF_HOST_VREF;

//
// Function prototypes that are private to this lib
//

/**
  Read/write PXC DQ attackers for a strobe
  Each DQ pin has 0, 1 or 2 attacker DQs within the same byte.

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Socket      - Socket number
  @param [in]       Ch          - Channel number
  @param [in]       Strobe      - Strobe number
  @param [in]       Mode        - Bit-field of different access modes
  @param [in,out]   Attacker    - Pointer to array of PXC_ATTACKER_SEL structures

  @retval SUCCESS
**/
UINT32
GetSetPxcAttackSel (
  IN     PSYSHOST          Host,
  IN     UINT8             Socket,
  IN     UINT8             Ch,
  IN     UINT8             Strobe,
  IN     UINT8             Mode,
  IN OUT PXC_ATTACKER_SEL  Attacker[MaxPxcAttacker]
  );

/**

  This function updates the register function block number and offset

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Ch          - Channel number
  @param [in]       LogRank     - Logical rank number in the DDRIO
  @param [in]       Strobe      - Strobe number
  @param [in]       RegOffset   - Register define

  @retval IO Address

**/
UINT32
UpdateIoRegister10nm (
  IN      PSYSHOST    Host,
  IN      UINT8       Ch,
  IN      UINT8       LogRank,
  IN      UINT8       Strobe,
  IN      UINT32      RegOffset
  );

/**

  This function updates the register function block number and offset

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Ch          - Channel number
  @param [in]       Strobe      - Strobe number
  @param [in]       RegOffset   - Register define

  @retval IO Address

**/
UINT32
UpdateIoRegisterCh10nm (
  IN      PSYSHOST    Host,
  IN      UINT8       Ch,
  IN      UINT8       Strobe,
  IN      UINT32      RegOffset
  );

/**

  Issue a comp update

  @param[in] Host   - Pointer to sysHost
  @param[in] Socket - Socket number
  @param[in] McId   - iMC number

  @retval N/A

**/
VOID
DoCompUpdate (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 McId
  );

/**

  Reads per-rank register from DDRIO data FUB instance selected by strobe number

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Socket      - Socket number
  @param [in]       Ch          - Channel number
  @param [in]       LogRank     - Logical rank number in the DDRIO
  @param [in]       Strobe      - Strobe number
  @param [in]       Offset      - Register define

  @retval Register value

**/
UINT32
ReadDdrioDataReg (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    LogRank,
  IN      UINT8    Strobe,
  IN      UINT32   Offset
  );

/**

  Reads per-channel register from DDRIO data FUB instance selected by strobe number

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Socket      - Socket number
  @param [in]       Ch          - Channel number
  @param [in]       Strobe      - Strobe number
  @param [in]       Offset      - Register define

  @retval Register value

**/
UINT32
ReadDdrioDataRegCh (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    Strobe,
  IN      UINT32   Offset
  );

/**

  Writes per-rank register to DDRIO data FUB instance selected by strobe number

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Socket      - Socket number
  @param [in]       Ch          - Channel number
  @param [in]       LogRank     - Logical rank number in the DDRIO
  @param [in]       Strobe      - Strobe number
  @param [in]       Offset      - Register define
  @param [in]       Data        - Register value

  @retval N/A

**/
VOID
WriteDdrioDataReg (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    LogRank,
  IN      UINT8    Strobe,
  IN      UINT32   Offset,
  IN      UINT32   Data
  );

/**

  Writes per-channel register to DDRIO data FUB instance selected by strobe number

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Socket      - Socket number
  @param [in]       Ch          - Channel number
  @param [in]       Strobe      - Strobe number
  @param [in]       Offset      - Register define
  @param [in]       Data        - Register value

  @retval N/A

**/
VOID
WriteDdrioDataRegCh (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    Strobe,
  IN      UINT32   Offset,
  IN      UINT32   Data
  );

/**

  Reads DDRIO CC registers. It takes DDRIO lane reversal into consideration.

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Socket      - Socket number
  @param [in]       Ch          - Channel number
  @param [in]       SubCh       - Sub channel number
  @param [in]       RegOffset   - Register define

  @retval Register value

**/
UINT32
ReadDdrioCcRegister (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    SubCh,
  IN      UINT32   RegOffset
  );

/**

  Writes DDRIO CC register. It takes DDRIO lane reversal into consideration.

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Socket      - Socket number
  @param [in]       Ch          - Channel number
  @param [in]       SubCh       - Sub channel number
  @param [in]       Offset      - Register define
  @param [in]       Data        - Register value

  @retval N/A

**/
VOID
WriteDdrioCcRegister (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    SubCh,
  IN      UINT32   RegOffset,
  IN      UINT32   Data
  );

/**

  Retrieves results from HW FSM based Sense Amp Calibration training and stores
  them in NVRAM. Also checks for errors while reading CMD/CTL results.

  @retval EFI_SUCCESS       - No errors detected
  @retval EFI_DEVICE_ERROR  - Error detected in hardware

**/
MRC_STATUS
CacheSenseAmpSettings (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  );

/**

  Restores the Sense Amp Calibration settings for a given channel.

  @param[in] Socket   - Socket number
  @param[in] Channel  - Channel number

  @retval N/A

**/
VOID
RestoreSenseAmpCalib (
  IN UINT8  Socket,
  IN UINT8  Channel
  );

/**

  Get min/max limits for Command, Control and Clock signals

  @param[in]  Socket      - Processor socket within the system (0-based)
  @param[in]  Ch          - Channel number
  @param[in]  Dimm        - DIMM number
  @param[in]  Level       - IO level to access
  @param[in]  Group       - Command, clock or control group to access
  @param[out] *MinLimit   - Minimum delay value allowed
  @param[out] *MaxLimit   - Maximum delay value allowed

  @retval EFI_SUCESS if signal max/mins are returned
          EFI_NOT_FOUND otherwise
**/
EFI_STATUS
GetCmdGroupLimitsCpu (
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  IN    UINT8     Dimm,
  IN    MRC_LT    Level,
  IN    MRC_GT    Group,
  OUT   UINT16    *MinLimit,
  OUT   UINT16    *MaxLimit
  );

/**
  Find register address for DDRIO strobe-based register for DDR4.
  This function does not take into account logical-to-physical
  strobe swizzling.

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Strobe      - Strobe number
  @param [in]       AddressBase - Base address for ch0, rank0, strobe0

  @retval Register address for the swizzled strobe
**/
UINT32
GetDdrIoAddr (
  IN     PSYSHOST  Host,
  IN     UINT8     Strobe,
  IN     UINT32    AddressBase
  );

#endif   // __MEM_IO_CONTROL_H__
