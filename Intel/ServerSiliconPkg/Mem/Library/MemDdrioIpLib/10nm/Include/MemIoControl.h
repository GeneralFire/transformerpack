/** @file
  MemIoControl.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2019 Intel Corporation. <BR>

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

#include "SysHost.h"
#include <Library/VictimAggresorTableLib.h>

//
// private definitions used in this library
//
#define MAX_PHASE_IN_READ_DQS_ADJUSTMENT 87 // 1.375UI, 64 + 64/4 + 64/8 - 1
#define DEFAULT_EMULATION_ROUND_TRIP     42

//
// Function prototypes that are private to this lib
//

/**
  Read/write PXC DQ attackers for a strobe
  Each DQ pin has 0, 1 or 2 attacker DQs within the same byte.

  @param [in    ] Host       - Pointer to sysHost
  @param [in    ] Socket     - Socket number
  @param [in    ] Ch         - Channel number
  @param [in    ] Strobe     - Strobe number
  @param [in    ] Mode       - Bit-field of different access modes
  @param [in,out] AttackerN0 - PXC_ATTACKER_SEL structure for Nibble 0
  @param [in,out] AttackerN1 - PXC_ATTACKER_SEL structure for Nibble 1

  @retval SUCCESS
**/
UINT32
GetSetPxcAttackSel (
  IN     PSYSHOST         Host,
  IN     UINT8            Socket,
  IN     UINT8            Ch,
  IN     UINT8            Strobe,
  IN     UINT8            Mode,
  IN OUT PXC_ATTACKER_SEL AttackerN0,
  IN OUT PXC_ATTACKER_SEL AttackerN1
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

#endif   // __MEM_IO_CONTROL_H__
