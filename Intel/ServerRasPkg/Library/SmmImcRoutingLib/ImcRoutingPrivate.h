/** @file
  Interface of imc routine library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#ifndef __IMC_ROUTING_PRIVATE_H__
#define __IMC_ROUTING_PRIVATE_H__

#include <RcRegs.h>
#ifdef SKX_HOST
#include <Mem/Library/MemMcIpLib14nm/Include/MemMcRegs.h>
#else
#include <Mem/Library/MemMcIpLib/Include/MemMcRegs.h>
#endif
#include <Library/ImcRoutingLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/MemFuncLib.h>
#include <MaxSocket.h>
// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#include <Library/AmiOemRasLib.h>
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support

#define ALERT_POLICY_UNC_EN           BIT7
#define ALERT_POLICY_DPA_ERR_EN       BIT5
#define ALERT_POLICY_AIT_ERR_EN       BIT3
#define ALERT_POLICY_DATAPATH_ERR_EN  BIT8
#define ALERT_POLICY_ILL_ACC_EN       BIT10

#define MAX_RANK_ERROR_THRESHOLD     0x7FFF

//
// Register A
//
typedef struct {
  UINT8 RS : 4;   // Rate Selection Bits
  UINT8 DV : 3;   // Divisor
  UINT8 Uip : 1;  // Update in progress
} RTC_REGISTER_A_BITS;

typedef union {
  RTC_REGISTER_A_BITS Bits;
  UINT8               Data;
} RTC_REGISTER_A;

//
// Register D
//
typedef struct {
  UINT8               Reserved:7; // Read as zero.  Can not be written.
  UINT8               Vrt:1;      // Valid RAM and Time
} RTC_REGISTER_D_BITS;

typedef union {
  RTC_REGISTER_D_BITS Bits;
  UINT8               Data;
} RTC_REGISTER_D;

#define RTC_ADDRESS_HOURS             4   // R/W  Range 1..12 or 0..23 Bit 7 is AM/PM
#define RTC_ADDRESS_DAY_OF_THE_MONTH  7   // R/W  Range 1..31
#define RTC_ADDRESS_MONTH             8   // R/W  Range 1..12
#define RTC_ADDRESS_YEAR              9   // R/W  Range 0..99
#define RTC_ADDRESS_REGISTER_A        10  // R/W[0..6]  R0[7]
#define RTC_ADDRESS_REGISTER_D        13  // RO


//
//more than 1 rank can wait at this status, waiting for SSR free.
//
#define            NONSPARESTATUS_SCRUBNEXT 8
//
//only 1 rank per MC can set NONSPARESTATUS_SCRUBBING and NONSPARESTATUS_DOSPARECOPY at a time
//
#define            NONSPARESTATUS_SCRUBBING 9
#define            NONSPARESTATUS_DOSPARECOPY  10

#define            NONSPARESTATUS_SPARECOPYDONE  11
#define            NONSPARESTATUS_TRY_TAG    12

#define         SPARERANKSTATUS_AVAILABLE         0
//
//only 1 rank per MC can set SPARERANKSTATUS_SCRUBBING and SPARERANKSTATUS_INPROGRESS at a time.
//
#define         SPARERANKSTATUS_SCRUBBING        1
#define         SPARERANKSTATUS_INPROGRESS      2
#define         SPARERANKSTATUS_NOTAVAILALBE  3


#define NON_SPARE_CE    1
#define NON_SPARE_CE_EVENT 2
#define SPARE_UC   3
#define SPARE_CE   4


#define END_OF_TABLE  ((MEM_RAS_EVENT_HNDLR)((UINTN)(-1)))

UINT32
EFIAPI
EfiSmmGetTime (
  VOID
  );

BOOLEAN
EFIAPI
DummyHandler (
  IN UINT8        NodeId,
  IN EVENT_TYPE   EventType
  );

VOID
SetNodeProgress (
  IN  UINT8    NodeId,
  IN  BOOLEAN  ProgressFlag
  );
/**
    Checks if the specified rank has error overflow
    If the CPU error counter overflows clear overflow
    status in CPU; counters are not cleared

  @param NodeId    - Memory controller ID
  @param Ch        - Channel ID
  @param Rank      - Rank ID

  @retval TRUE      - if the specified rank has error overflow
  @retval FALSE     - specified rank has no error overflow

**/
BOOLEAN
EFIAPI
IsErrorExcdForThisRank (
  IN UINT8 Node,
  IN UINT8 NodeCh,
  IN UINT8 Rank
  );

BOOLEAN
EFIAPI
CheckAndHandleRankSparing (
  IN UINT8 NodeId,
  IN EVENT_TYPE EventType
  );

/**
  This routine will check if the Rank belongs to a DCPMM dimm

  @param NodeId    - Memory controller ID
  @param Ch        - Channel ID
  @param Rank      - Rank ID

  @retval TRUE      - if the specified rank has error overflow
  @retval FALSE     - specified rank has no error overflow

**/
BOOLEAN
EFIAPI
IsDcpmmRank (
  IN UINT8 Socket,
  IN UINT8 ChOnSkt,
  IN UINT8 Rank
  );



VOID
ClearRankErrors (
  IN UINT8 Socket,
  IN UINT8 SktCh,
  IN UINT8 Rank
  );

BOOLEAN
GetNodeProgress (
  IN  UINT8    NodeId
  );

VOID
EFIAPI
DisableRankError (
  IN  UINT8     Node,
  IN  UINT8     ChOnMc,
  IN  UINT8     Rank
  );

/**
    This routine is to check if parcicular rank located in bank VLS region.

    @param Skt          - CPU socket id
    @param ChOnSkt      - channel on socket
    @param Rank         - Rank number

    @retval BOOLEAN     - TRUE if rank is in parcicular rank.

**/
BOOLEAN
EFIAPI
IsRankInBankVLS (
  IN  UINT8  Skt,
  IN  UINT8  ChOnSkt,
  IN  UINT8  Rank,
  OUT UINT8 *Plus1PeerRank
  );


VOID
EFIAPI
CorrectedErrorSMIThrottling (
  IN        UINT8    Node,
  IN        UINT8    ChOnMc,
  IN        UINT8    Rank,
  IN        UINT8    MemCeFloodPolicy
  );

VOID
EFIAPI
ClearShadowReg (
  IN UINT8 Socket,
  IN UINT8 Mc
  );

VOID
EFIAPI
SettingScratchPadReg (
  IN   UINT8     Skt,
  IN   UINT8     SktCh,
  IN   UINT32    ScratchPadValue
  );

/**
  Whether there is potential pair rank except itself for the failed rank in the channel

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number
  @param  FailedRank      - Failed Rank number

  @retval Status          - TRUE - there is potential pair rank for the failed rank
                          - FALSE- no potential pair rank for the failed rank

**/
BOOLEAN
EFIAPI
IsThereAnyPotentialPairRank (
  IN  UINT8         NodeId,
  IN  UINT8         ChOnNode,
  IN  UINT8         FailedRank
  );

/**
  Find out pair rank for bank VLS

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number
  @param  Rank            - Rank number
  @param  pPairRank       - Pointer to pair rank of the failed rank

  @retval status          - EFI_SUCCESS      - found pair rank
                          - EFI_UNSUPPORTED  - non pair rank found

**/
EFI_STATUS
EFIAPI
FindBankVlsPairRank (
  IN  UINT8         NodeId,
  IN  UINT8         ChOnNode,
  IN  UINT8         Rank,
  OUT UINT8         *pPairRank
  );


/**
  Set the corrected error threshold for the rank

  @param Socket    -     Node ID
  @param ChOnNode -      Channel ID on the Node
  @param Rank      -     Rank ID
  @param ErrThreshold  - Threshold value  to be set

  @retval None

**/


/**
  This routine is to check if parcicular rank in Plus one state.

  @param Node         - Memory controller ID
  @param ChOnNode     - Channel number on this node.
  @param Rank         - Rank number

  @retval BOOLEAN     - TRUE if rank in +1 state

**/
BOOLEAN
EFIAPI
IsRankInPlusoneState (
  IN  UINT8  Node,
  IN  UINT8  ChOnNode,
  IN  UINT8  Rank
  );

/**
  Checks if the rank is in ADDDC region

  @param NodeId         - Memory controller ID
  @param ChOnNode       - Channel number
  @param Rank           - Rank number

  @retval TRUE          - Rank is in ADDDC
  @retval TRUE          - Rank not in ADDDC
**/
BOOLEAN
IsRankinAdddc (
  IN  UINT8   NodeId,
  IN  UINT8   ChOnNode,
  IN  UINT8   Rank
  );

/**
  Clears the error counter and update error threshold
  that  was pre-configured for all rank on the channel

  @param Node           - Memory controller ID
  @param ChOnNode       - Channel number on this node.
  @param ErrThreshold   - Threshold value to be set

  @retval N/A
**/
VOID
SetErrorThresholdBeforeSpareCopyStart (
  UINT8    Node,
  UINT8    ChOnNode,
  UINT16   ErrThreshold
);

/**
  This routine changes corrected error threshold for the ranks in
  a channel according to the rank mode.

  @param Node           - Memory controller ID
  @param ChOnNode       - Channel number on this node.

  @retval N/A
**/
VOID
SetErrorThresholdAfterCopyComplete (
  UINT8    Node,
  UINT8    ChOnNode
);
#endif
