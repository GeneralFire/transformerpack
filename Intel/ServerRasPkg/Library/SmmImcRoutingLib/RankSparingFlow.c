/** @file
  This file implements Rank Sparing code

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#include <Library/RasDebugLib.h>
#include <SiliconSetting.h>
#include <Library/PlatSSRLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/S3MemDataLib.h>
#include <Library/MemMcIpRasLib.h>
#include "ImcRoutingPrivate.h"
#include <Library/PclsLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/SiliconWorkaroundLib.h>

// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#if AMI_RAS_ENHANCEMENTS_AND_HOOKS_SUPPORT
FAILED_DIMM_INFO		mSpareFailedDimmInfo;
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support

/**

  Parse rank status.

  @param McNodeId -- Memory controller ID
  @param Ch -- channel number.
  @param Rank -- logical ran

  @retval none

 Rank Sparing is only possible when operation on 1LM mode.
 his feature is disabled if system has AEP Dimm present and using 2LM mode.


 **/
VOID
EFIAPI
ParseRankState (
  IN UINT8 McNodeId,
  IN UINT8 Ch,
  IN UINT8 Rank
  )
{
  RANK_STATUS_TYPE State;
  UINT32 Rankstate;
  IMC_RAS_STRUCTURE   *ImcRas;
  ImcRas = GetImcRasData ();

  //
  //switch for print parse rank state. Open it for debugging.
  //
  if (FALSE) {
    State.Data = ImcRas->RankStatus[McNodeId][Ch][Rank].Data;
    RAS_DEBUG((LEVEL_REG, "MC:%d Ch:%d Rank:%d----", McNodeId, Ch, Rank));
    if (State.Bits.rank_exist) {
      RAS_DEBUG((LEVEL_REG, "rank exist,   "));
    }

    if (State.Bits.rank_fail) {
      RAS_DEBUG((LEVEL_REG, "fail,   "));
    }

    if (State.Bits.non_spare_rank) {
      RAS_DEBUG((LEVEL_REG, "non-spare rank state:   "));
      Rankstate = State.Bits.non_spare_status;
      switch (Rankstate) {
        case NONSPARESTATUS_SCRUBNEXT:
          RAS_DEBUG((LEVEL_REG, "scrub next"));
          break;
        case NONSPARESTATUS_SCRUBBING:
          RAS_DEBUG((LEVEL_REG, "scrubbing"));
          break;
        case NONSPARESTATUS_DOSPARECOPY:
          RAS_DEBUG((LEVEL_REG, "do spare copying"));
          break;
        case NONSPARESTATUS_SPARECOPYDONE:
          RAS_DEBUG((LEVEL_REG, "spare copy done"));
          break;
        case NONSPARESTATUS_TRY_TAG:
          RAS_DEBUG((LEVEL_REG, "try do tag"));
          break;
        default:
          RAS_DEBUG((LEVEL_REG, "wrong state"));
          break;
      }
    }

    if (State.Bits.spare_rank) {
      RAS_DEBUG((LEVEL_REG, "spare rank state:   "));
      Rankstate = State.Bits.spare_rank_status;
      switch (Rankstate) {
        case SPARERANKSTATUS_AVAILABLE:
          RAS_DEBUG((LEVEL_REG, "available"));
          break;
        case SPARERANKSTATUS_SCRUBBING:
          RAS_DEBUG((LEVEL_REG, "scrubbing"));
          break;
        case SPARERANKSTATUS_INPROGRESS:
          RAS_DEBUG((LEVEL_REG, "do spare copying (in progress)"));
          break;
        case SPARERANKSTATUS_NOTAVAILALBE:
          RAS_DEBUG((LEVEL_REG, "not available"));
          break;
        default:
          RAS_DEBUG((LEVEL_REG, "wrong state"));
          break;
      }
    }

    RAS_DEBUG((LEVEL_REG, "\n "));
  }
}

/**

  get specific rank state

  @param McNodeId -- Memory controller ID
  @param Ch -- channel number.
  @param LogicalRank -- logical rank index.

  @retval rank state

  **/
RANK_STATUS_TYPE
EFIAPI
GetRankState (
  IN UINT8 McNodeId,
  IN UINT8 Ch,
  IN UINT8 LogicalRank
  )
{
  IMC_RAS_STRUCTURE   *ImcRas;
  ImcRas = GetImcRasData ();
  return ImcRas->RankStatus[McNodeId][Ch][LogicalRank];
}

/**

  Checks if the given rank is a spare rank and returns true if it is

  @param McNodeId -- Memory controller ID
  @param Ch -- channel number.
  @param LogicalRank -- logical rank index.

  @retval spare rank exists or not

  **/
BOOLEAN
EFIAPI
IsSpareRankAvailable (
  IN UINT8 McNodeId,
  IN UINT8 Ch,
  IN UINT8 LogicalRank
  )
{
  IMC_RAS_STRUCTURE   *ImcRas;
  ImcRas = GetImcRasData ();
  return (ImcRas->RankStatus[McNodeId][Ch][LogicalRank].Bits.rank_exist == 1);
}

/**

  set  rank new status. New status is  should downgrade, not upgrade here.

  @param McNodeId -- Memory controller ID
  @param Ch -- channel number.
  @param LogicalRank -- logical rank index.
  @param SpareStatus -- spare rank new status.

  @retval none

  **/
VOID
EFIAPI
SetRankState (
  IN UINT8 McNodeId,
  IN UINT8 Ch,
  IN UINT8 LogicalRank,
  IN UINT8 NewState
  )
{
  IMC_RAS_STRUCTURE   *ImcRas;
  ImcRas = GetImcRasData ();
  RAS_DEBUG((LEVEL_REG, "Set rank state -- Node:%d ch:%d  rank:%d new state:%d\n", McNodeId, Ch, LogicalRank, NewState));
  ParseRankState (McNodeId, Ch, LogicalRank);

  if (ImcRas->RankStatus[McNodeId][Ch][LogicalRank].Bits.rank_exist &&
    ImcRas->RankStatus[McNodeId][Ch][LogicalRank].Bits.spare_rank &&
    (ImcRas->RankStatus[McNodeId][Ch][LogicalRank].Bits.rank_fail == 0)) {
    ImcRas->RankStatus[McNodeId][Ch][LogicalRank].Bits.spare_rank_status = NewState;
  }

  if (ImcRas->RankStatus[McNodeId][Ch][LogicalRank].Bits.rank_exist &&
    ImcRas->RankStatus[McNodeId][Ch][LogicalRank].Bits.non_spare_rank) {
    ImcRas->RankStatus[McNodeId][Ch][LogicalRank].Bits.non_spare_status = NewState;
  }

  ParseRankState (McNodeId, Ch, LogicalRank);

  return;
}

/**

  check if current rank is specific state of  rank.

  @param McNodeId -- Memory controller ID
  @param Ch -- channel number.
  @param LogicalRank -- logical rank.
  @param CheckState  -- check state

  @retval TRUE  -- available state of spare rank.
  @retval FALSE -- not available state of spare rank.

  **/
BOOLEAN
EFIAPI
CheckRankState (
  IN UINT8 McNodeId,
  IN UINT8 Ch,
  IN UINT8 LogicalRank,
  IN UINT8 CheckState
  )
{
  BOOLEAN IsCheckState = FALSE;
  IMC_RAS_STRUCTURE   *ImcRas;
  ImcRas = GetImcRasData ();

  if (ImcRas->RankStatus[McNodeId][Ch][LogicalRank].Bits.rank_exist &&
    ImcRas->RankStatus[McNodeId][Ch][LogicalRank].Bits.spare_rank &&
    (ImcRas->RankStatus[McNodeId][Ch][LogicalRank].Bits.rank_fail == 0)) {
    if (ImcRas->RankStatus[McNodeId][Ch][LogicalRank].Bits.spare_rank_status == CheckState) {
      IsCheckState = TRUE;
    }
  }

  if (ImcRas->RankStatus[McNodeId][Ch][LogicalRank].Bits.rank_exist &&
    ImcRas->RankStatus[McNodeId][Ch][LogicalRank].Bits.non_spare_rank) {
    if (ImcRas->RankStatus[McNodeId][Ch][LogicalRank].Bits.non_spare_status == CheckState) {
      IsCheckState = TRUE;
    }
  }

  ParseRankState (McNodeId, Ch, LogicalRank);
  RAS_DEBUG((LEVEL_REG, " check rank state : check state%d   status:%d\n", CheckState, IsCheckState));

  return IsCheckState;
}

/**

  check if current channel has specific state  rank,

  @param McNodeId -- Memory controller ID
  @param Ch -- channel number.
  @param state -- find state status
  @param pLogicalRank -- pointer to found rank

  @retval TRUE  -- found.
  @retval FALSE -- no found.
  @retval Status

  **/
BOOLEAN
EFIAPI
CheckRankStatePerCh (
  IN UINT8 McNodeId,
  IN UINT8 Ch,
  IN UINT8 state,
  IN OUT UINT8 * pFoundRank
  )
{
  BOOLEAN RankStatusExist = FALSE;
  UINT8 i;

  for (i = 0; i < (MAX_RANK_CH * SUB_CH); i++) {
    if (CheckRankState (McNodeId, Ch, i, state)) {
      RankStatusExist = TRUE;
      if (pFoundRank != NULL) {
        *pFoundRank = i;
      }
      break;
    }
  }

  RAS_DEBUG((LEVEL_REG, "CheckRankStatePerCh    MC:%d   Channel:%d  State:%d  exist:%d \n", McNodeId, Ch, state, RankStatusExist));

  return RankStatusExist;
}

/**

  check if current MC has specific state rank,

  @param McNodeId -- Memory controller ID
  @param Ch -- channel number.
  @param state -- find state status
  @param pLogicalRank -- pointer to found rank

  @retval TRUE  -- found.
  @retval FALSE -- no found.
  @retval Status

  **/
BOOLEAN
EFIAPI
CheckRankStatePerMC (
  IN UINT8 NodeId,
  IN UINT8 state,
  IN OUT UINT8 * FoundCh,
  IN OUT UINT8 * FoundRank
  )
{
  BOOLEAN MCStatusExist = FALSE;
  UINT8   i;

  for (i = 0; i < MAX_MC_CH; i++) {
    if (IsChannelPresent (NodeId,i) == FALSE) {
      continue;
    }

    MCStatusExist = CheckRankStatePerCh (NodeId, i, state, FoundRank);
    if (MCStatusExist) {
      if (FoundCh != NULL) {
        *FoundCh = i;
      }
      break;
    }
  }

  RAS_DEBUG((LEVEL_REG, "CheckRankStatePerMC   MC:%d     State:%d  exist:%d  \n", NodeId, state, MCStatusExist));

  return MCStatusExist;
}

/**

  get the lowest priority spare rank.

  @param McNodeId -- Memory controller ID
  @param Ch -- channel number.
  @param LogicalRank -- pointer to logical rank.

  @retval EFI_STATUS

  Note: invoker should call IsAvailSpareRankExist first.

  **/
VOID
EFIAPI
GetLowestPrioritySpareRank (
  IN UINT8 McNodeId,
  IN UINT8 Ch,
  OUT UINT8* pLogicalRank
  )
{
  UINT8 i;
  UINT8 LowestRank = 0;
  UINT32 LowestSeq = 0;
  UINT32 Seq;
  IMC_RAS_STRUCTURE   *ImcRas;

  ImcRas = GetImcRasData ();

  for (i = 0; i < (MAX_RANK_CH * SUB_CH); i++) {
    if (CheckRankState (McNodeId, Ch, i, SPARERANKSTATUS_AVAILABLE)) {
      Seq = ImcRas->RankStatus[McNodeId][Ch][i].Bits.spare_rank_sequence;
      if (Seq >= LowestSeq) {
        LowestRank = i;
        LowestSeq = Seq;
      }
    }
  }

  *pLogicalRank = LowestRank;

  RAS_DEBUG((LEVEL_REG, "GetLowestPrioritySpareRank--  MC:%d   Ch:%d  low priority rank:%d \n", McNodeId, Ch, LowestRank));

  return;
}

/**

  Update scratch pad register after spare rank is mark dirty.

  SCRATCHPAD2 in each DDR channel
  bits 0-7     one-hot encoding of channel's spare ranks, each bit position represents a logical rank id
  bits 8       sparing enabled

  @param NodeId  -- MC id
  @param Ch -- when value is 0xff, do whole MC; other do specified channel.
  @retval None

  **/
VOID
UpdateScratchPadReg (
  IN UINT8 McNodeId,
  IN UINT8 Ch
  )
{
  UINT8 Skt;
  UINT8 SktCh;
  UINT8 i;
  UINT32 ScratchPadValue;


  ScratchPadValue = 0;
  for (i = 0; i < (MAX_RANK_CH * SUB_CH); i++) {
    if (CheckRankState (McNodeId, Ch, i, SPARERANKSTATUS_AVAILABLE)) {
      ScratchPadValue |= (1 << i);
    }
  }

  Skt = NODE_TO_SKT(McNodeId);
  SktCh = NODECHA_TO_SKTCHA(McNodeId, Ch);
  SettingScratchPadReg (Skt, SktCh, ScratchPadValue);

}

/**

  Mark the rank as dirty flag.
  1. set mSpareRankStatus as RankSpareNotAvailable if it is a spare rank
  2. add it to mFailedRank

  @param McNodeId -- Memory controller ID
  @param Ch -- channel number.
  @param LogicalRank -- logical rank.

  @retval TRUE  -- available spare rank.
  @retval FALSE -- not an spare rank.
  @retval Status

  **/
VOID
EFIAPI
MarkRankDirtyFlag (
  IN UINT8 McNodeId,
  IN UINT8 Ch,
  IN UINT8 LogicalRank
  )
{
  IMC_RAS_STRUCTURE   *ImcRas;
  ImcRas = GetImcRasData ();
  ImcRas->RankStatus[McNodeId][Ch][LogicalRank].Bits.rank_fail = 1;

  RAS_DEBUG((LEVEL_REG, "MarkRankDirtyFlag -  MC:%d  channel:%d,  rank:%d is dirty\n", McNodeId, Ch, LogicalRank));
  return;
}



BOOLEAN
EFIAPI
FindFailRank (
  IN     UINT8      Node,
  IN OUT UINT8     *FoundCh,
  IN OUT UINT8     *FoundRank
  )
{
  BOOLEAN    FailedRankFound = FALSE;
  UINT8      Ch;
  UINT8      Rank;

  for (Ch = 0; Ch < MAX_MC_CH; Ch ++) {
    if (IsChannelPresent( Node,Ch) == FALSE) {
      continue;
    }

    //
    // If this ch doesn't have spare rank, skip to next ch
    //
    if (!CheckRankStatePerCh (Node, Ch, SPARERANKSTATUS_AVAILABLE, NULL)) {
      continue;
    }

    //
    // Check if there is any rank in this channel
    // that has overflow bit set
    //
    for (Rank = 0; Rank < (MAX_RANK_CH * SUB_CH); Rank++) {
      if (IsErrorExcdForThisRank (Node, Ch, Rank) &&
          (!IsDcpmmRank (NODE_TO_SKT (Node), NODECHA_TO_SKTCHA (Node, Ch), Rank))) {
        FailedRankFound = TRUE;
        *FoundCh = Ch;
        *FoundRank = Rank;
        break;
      }
    }

    if (FailedRankFound) {
      break;
    }
  }
  return FailedRankFound;
}

/**

 Handles patrol complete event for a node

 @param NodeId  - Memory controller ID

 @retval EFI_SUCCESS / Error code

 **/
VOID
EFIAPI
HandlePatrolCompleteEventForNode (
  IN   UINT8   Node
  )
{
  UINT8                               SourceRank;
  UINT8                               DestRank;
  UINT8                               Ch;
  UINT8                               Socket;
  UINT8                               Mc;
  UINT8                               ScrubbingRank;
  UINT32                              RegAddr;
  UINT32                              RegValue;
  EFI_STATUS                          Status;
  IMC_RAS_STRUCTURE                   *ImcRas;

  ImcRas = GetImcRasData ();
  Socket = NODE_TO_SKT (Node);
  Mc = NODE_TO_MC (Node);

  RAS_DEBUG((LEVEL_BASIC_FLOW, "[Patrol] Patrol complete SMI entry! Skt:%d Mc:%d\n", Socket, Mc));

  Status = GetScrubbedRank (Socket, Mc, &Ch, &ScrubbingRank);
  ASSERT_EFI_ERROR (Status);

  if (!CheckRankStatePerCh (Node, Ch, NONSPARESTATUS_SCRUBBING, &SourceRank)) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "[Patrol] Error! didn't find the scrubbing non spare rank:%d\n"));
  }

  CheckRankStatePerCh (Node, Ch, SPARERANKSTATUS_SCRUBBING, &DestRank);
  if (ScrubbingRank != DestRank) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR! spare desti rank is not equal to SSR rank\n"));
  }

  RAS_DEBUG((LEVEL_FUNC_FLOW, "[Patrol] srubbing rank:%d   source rank:%d\n", DestRank, SourceRank));

  SetRankState (Node, Ch, DestRank, SPARERANKSTATUS_INPROGRESS);
  SetRankState (Node, Ch, SourceRank, NONSPARESTATUS_DOSPARECOPY);
  //
  // SI WA to down grade patrol error during spare rank scrub.
  // After scrub complete set ign_ptrl_uc to 0
  //
  if ((IsSiliconWorkaroundEnabled ("S2206549949")) && (ImcRas->PatrolErrorDowngradeEn == 0)) {
    SetPatrolErrorDowngrade (Socket, NODECHA_TO_SKTCHA (Node, Ch), FALSE, &RegAddr, &RegValue);
  }
  //
  //switch back to SPA mode, and clear legacy part status, also it will clear patrol scrub smi interrupt status by HW.
  //
  PatrolScrubGoBack (Node, Ch, SourceRank, DestRank);

  //
  //do spare copy operation
  //
  PatrolScrubInfoSaveOrRestore (Node, PATROL_SAVE | GV_C6FLAG);
  RankSparingCopy (Node, Ch, SourceRank, DestRank );

  //
  //update progress info
  //
  SetNodeProgress (Node, TRUE);

  RAS_DEBUG((LEVEL_BASIC_FLOW, "[Patrol] end!\n"));

}

/**

  Handles patrol UC event for a node

  @param NodeId  - Memory controller ID

  @retval EFI_SUCCESS / Error code

  **/

VOID
EFIAPI
HandlePatrolUCEventForNode (
  IN  UINT8 NodeId
  )
{
  UINT8                           Socket;
  UINT8                           Mc;
  UINT8                           ScrubbingCh;
  UINT8                           ScrubbingRank;
  UINT8                           SourceRank;
  UINT8                           NextSpareRank;
  UINT32                          RegAddr;
  UINT32                          RegValue;
  EFI_STATUS                      Status;
  IMC_RAS_STRUCTURE               *ImcRas;

  ImcRas = GetImcRasData ();

  Socket = NODE_TO_SKT(NodeId);
  Mc = NODE_TO_MC(NodeId);
  RAS_DEBUG((LEVEL_BASIC_FLOW, "[Patrol]  this is an spare rank UC\n"));

  Status = GetScrubbedRank (Socket, Mc, &ScrubbingCh, &ScrubbingRank);
  ASSERT_EFI_ERROR (Status);

  //
  //mark this rank as "dirty"
  //
  MarkRankDirtyFlag (NodeId, ScrubbingCh, ScrubbingRank);
  SetRankState (NodeId, ScrubbingCh, ScrubbingRank, SPARERANKSTATUS_NOTAVAILALBE);

  if (!CheckRankStatePerCh (NodeId, ScrubbingCh, NONSPARESTATUS_SCRUBBING, &SourceRank)) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "[Patrol UE] Error! didn't find the scrubbing non spare rank:%d\n"));
  }
  //
  //move to next rank scrub
  //
  if (CheckRankStatePerCh (NodeId, ScrubbingCh, SPARERANKSTATUS_AVAILABLE, NULL)) {
    GetLowestPrioritySpareRank(NodeId, ScrubbingCh, &NextSpareRank);

    SetRankState (NodeId, ScrubbingCh, NextSpareRank, SPARERANKSTATUS_SCRUBBING);
    PatrolScrubSpecificAddress (NodeId, ScrubbingCh, NextSpareRank);
  } else {
    //
    // SI WA to down grade patrol error during spare rank scrub.
    // After scrub complete set ign_ptrl_uc to 0.
    //
    if ((IsSiliconWorkaroundEnabled ("S2206549949")) && (ImcRas->PatrolErrorDowngradeEn == 0)) {
      SetPatrolErrorDowngrade (Socket, NODECHA_TO_SKTCHA (NodeId, ScrubbingCh), FALSE, &RegAddr, &RegValue);
    }
    PatrolScrubGoBack (NodeId, ScrubbingCh, SourceRank, ScrubbingRank);
  }
}

/**

  Checks if patrol has completed on any node and handles it

  @retval EFI_SUCCESS / Error code

  **/
VOID
EFIAPI
CheckAndHandlePatrolEvent (
  IN   UINT8     Socket
  )
{
  UINT8                       Mc;
  UINT8                       Ch;
  UINT8                       Node;
  UINT8                       ChOnSkt;
  SSR_STATE                   SsrState;

  for (Mc = 0; Mc < MAX_IMC; Mc++) {
    Node = SKTMC_TO_NODE(Socket, Mc);
    if (IsNodePresent (Node)) {
      SsrState = GetMcSsrState (Node);
      if (SsrState == PatrolLegacyCPLMode) {
        HandlePatrolCompleteEventForNode (Node);
        if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
          RAS_DEBUG((LEVEL_BASIC_FLOW, "[Patrol] non_spare memory complete event start! \n"));
          ClearPatrolStatus (Socket, Mc, 0); //Socket, Mc, ChOnSkt
        } else {
          for (Ch = 0; Ch < MAX_MC_CH; Ch++) {
            if (IsChannelPresent (Node, Ch) == FALSE) {
              continue;
            }
            ChOnSkt = NODECHA_TO_SKTCHA(Node, Ch);
            RAS_DEBUG((LEVEL_BASIC_FLOW, "[Patrol] non_spare memory complete event start! \n"));
            ClearPatrolStatus (Socket, Mc, ChOnSkt); //Socket, Mc, ChOnSkt
          }
        }
      }
      if (SsrState == PatrolLegacyStoppedMode) {
        RAS_DEBUG((LEVEL_BASIC_FLOW, "[Patrol] spare rank UC event start \n"));
        HandlePatrolUCEventForNode (Node);
      }
      //
      //no need to clear patrol ptl_cmpl/ptl_stopped as it will be cleard by enable patrol scrub.
      //
    }
  } // for Mc
}


/**

  Initialize all rank sparing related data structures
  This routine should be called only once during boot

  @param None
  @retval None

  **/
VOID
EFIAPI
InitializeRankSparing (
  VOID
  )
{
  UINT8                NodeId;
  UINT8                Socket;
  UINT8                Mc;
  UINT8                Ch;
  UINT8                ChOnSkt;
  UINT8                RankIndex;
  UINT8                Rank;
  IMC_RAS_STRUCTURE   *ImcRas;
  MEM_TOPOLOGY        *MemTopology;

  ImcRas = GetImcRasData ();
  MemTopology = GetMemTopology ();

  //
  // It don't init non-spare rank info here, init it at error handler in case error happen.
  //
  for (NodeId = 0; NodeId < MC_MAX_NODE; NodeId++) {

    Socket = NODE_TO_SKT(NodeId);
    Mc     = NODE_TO_MC(NodeId);
    if (IsNodePresent (NodeId) == FALSE) {
      continue;
    }

    for (Ch = 0; Ch < MAX_MC_CH; Ch++) {
      if (IsChannelPresent (NodeId, Ch) == FALSE) {
        continue;
      }
      ChOnSkt = NODECHA_TO_SKTCHA(NodeId, Ch);

      for (RankIndex = 0; RankIndex < (MAX_SPARE_RANK); RankIndex++) {
        if (MemTopology->Socket[Socket].ChannelInfo[ChOnSkt].SpareLogicalRank[RankIndex] != 0xFF) {
          Rank = MemTopology->Socket[Socket].ChannelInfo[ChOnSkt].SpareLogicalRank[RankIndex];
          ImcRas->RankStatus[NodeId][Ch][Rank].Data = 0;
          ImcRas->RankStatus[NodeId][Ch][Rank].Bits.rank_exist = 1;
          ImcRas->RankStatus[NodeId][Ch][Rank].Bits.spare_rank = 1;
          ImcRas->RankStatus[NodeId][Ch][Rank].Bits.spare_rank_sequence = RankIndex;
          RAS_DEBUG((LEVEL_FUNC_FLOW, "[Rank] spare rank from HOB  Node:%d channel:%d  logical rank:%d\n", NodeId, Ch, Rank));
        }
      }  //rank loop
    } // ch loop
  } // node loop

  ImcRas->RasEventHndlrTable[RANK_SPARE_INDEX] = CheckAndHandleRankSparing;
}

/**

 Check and handle ranks sparing done event

 @param NodeId    - Memory controller ID

  @retval TRUE  --  the event has been handled.
  @retval FALSE --  the event has not been handled.

 **/
BOOLEAN
EFIAPI
CheckAndHandleRankSparingDone (
  IN UINT8 NodeId
  )
{
  UINT8            Ch;
  BOOLEAN          RankSpareInProgress;
  UINT8            SpareProgressRank = 0;
  UINT8            SourceCh;
  UINT8            SourceRank;
  BOOLEAN          IsEventHandled = FALSE;
  UINT8            Index = 0;
  MEMRAS_S3_PARAM *S3Data;

  RAS_DEBUG((LEVEL_BASIC_FLOW, "[Rank] pending event start\n"));
  //
  // Check if there is any rank sparing event already in progress
  // At a time only one rank sparing operation can be in progress
  // for a given node.
  //
  RankSpareInProgress = CheckRankStatePerMC (NodeId, SPARERANKSTATUS_INPROGRESS, &Ch, &SpareProgressRank);

  RAS_DEBUG((LEVEL_FUNC_FLOW, " [Rank pending] CheckAndHandleRankSparingDone in progress:%d\n", RankSpareInProgress));
  if (RankSpareInProgress) {
      RAS_DEBUG((LEVEL_FUNC_FLOW, "[RAS pending ] spare copy complete! \n"));
      //
      //update status
      //
      SetRankState (NodeId, Ch, SpareProgressRank, SPARERANKSTATUS_NOTAVAILALBE);
      if (CheckRankStatePerMC (NodeId, NONSPARESTATUS_DOSPARECOPY, &SourceCh, &SourceRank)) {
        if (SourceCh != Ch) {
          RAS_DEBUG((LEVEL_FUNC_FLOW, "[Rank pending] error! the spare copy ch:%d is not equal to found ch:%d", Ch, SourceCh));
        }
        SetRankState (NodeId, SourceCh, SourceRank, NONSPARESTATUS_SPARECOPYDONE);
      }
      //
      //update scratch pad register here
      //
      UpdateScratchPadReg (NodeId, SourceCh);

      ClearSparing2LmWa (NodeId, SourceCh);
      PatrolScrubInfoSaveOrRestore (NodeId, PATROL_RESTORE | GV_C6FLAG );
      SetNodeProgress (NodeId, FALSE);

      S3Data = GetS3RasData ();
      Index = 0;
      if (S3Data->SpareRank[NodeId][Ch].SpareInUse[Index] ==1 ) {
        Index = 1;
      }
      S3Data->SpareRank[NodeId][Ch].SpareInUse[Index] = 1;
      S3Data->SpareRank[NodeId][Ch].OldLogicalRank[Index] = SourceRank;
      S3Data->SpareRank[NodeId][Ch].SpareLogicalRank[Index]  = SpareProgressRank;

      //
      //Reverse PCLS regions, if any, on this failed rank so that PCLS resources can be reused.
      //
      ClearPclsOnRank (NodeId, Ch, SourceRank);

// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#if AMI_RAS_ENHANCEMENTS_AND_HOOKS_SUPPORT
      OemReportStatusCode(mSpareFailedDimmInfo, SPARE_COPY_COMPLETE);
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support
      IsEventHandled = TRUE;
  }
  RAS_DEBUG((LEVEL_BASIC_FLOW, "[Rank] pending event end\n"));
  return IsEventHandled;
}

/**
  Check for new rank sparing events and handle

  @param NodeId    - Memory controller ID
  @retval TRUE  --  the event has been handled.
  @retval FALSE --  the event has not been handled.
  **/
BOOLEAN
EFIAPI
CheckAndHandleNewRankSparingEvent (
  IN UINT8 NodeId
  )
{
  UINT8                Ch;
  UINT8                Rank;
  UINT8                Socket;
  UINT8                ChOnSkt;
  UINT8                FoundCh;
  UINT8                FoundRank;
  BOOLEAN              IsEventHandled = FALSE;
  UINT8                NextSpareRank;
  IMC_RAS_STRUCTURE   *ImcRas;

  RAS_DEBUG((LEVEL_BASIC_FLOW, "[Rank] new event start\n"));
  ImcRas = GetImcRasData ();

  Socket = NODE_TO_SKT(NodeId);

  if (FindFailRank (NodeId, &FoundCh, &FoundRank)) {
      
// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#if AMI_RAS_ENHANCEMENTS_AND_HOOKS_SUPPORT
      {
          BOOLEAN   		SkipEventHandlers = FALSE;
          UINT8     		ErrorEvent = ERROR_EVENT_RANK_SPARING;
          EFI_STATUS   	 	Status;
          Status = GetChannelErrorInfo (NodeId, FoundCh, FALSE, &mSpareFailedDimmInfo);
          if (!EFI_ERROR(Status)) {
		      // Oem Hook To decide the Error action based on the Error data
              OemReadyToTakeAction(ErrorEvent, &mSpareFailedDimmInfo, &SkipEventHandlers);
              if (SkipEventHandlers == TRUE) {
                  DEBUG((DEBUG_INFO,"OEM request..Skipping Rank Sparing Event!\n"));
                  return IsEventHandled;
              }
          }
      }
#endif
 // APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support
    //
    //if multi error happens in one node, clear all errors include the one to be handled.
    //
    for (Ch = 0; Ch < MAX_MC_CH; Ch++) {
      ChOnSkt = NODECHA_TO_SKTCHA(NodeId,Ch);
      for (Rank = 0; Rank < (MAX_RANK_CH * SUB_CH); Rank ++) {
        ClearRankErrors (Socket, ChOnSkt, Rank);
      }
    }

    Ch = FoundCh;
    Rank = FoundRank;
    RAS_DEBUG((LEVEL_FUNC_FLOW, "[Rank]  found a rank error on MC:%d   channel:%d,  rank:%d\n", NodeId, FoundCh, FoundRank));

    ImcRas->RankStatus[NodeId][Ch][Rank].Data = 0;
    ImcRas->RankStatus[NodeId][Ch][Rank].Bits.rank_exist = 1;
    ImcRas->RankStatus[NodeId][Ch][Rank].Bits.non_spare_rank = 1;
    ImcRas->RankStatus[NodeId][Ch][Rank].Bits.non_spare_status = NONSPARESTATUS_SCRUBNEXT;

    MarkRankDirtyFlag(NodeId, Ch, Rank);

    GetLowestPrioritySpareRank (NodeId, Ch, &NextSpareRank);
    if (IsPatrolEnabled ()) {
      PatrolScrubSpecificAddress (NodeId, Ch, NextSpareRank);

      SetRankState (NodeId, Ch, NextSpareRank, SPARERANKSTATUS_SCRUBBING);
      SetRankState (NodeId, Ch, Rank, NONSPARESTATUS_SCRUBBING);
    } else {
      PatrolScrubInfoSaveOrRestore (NodeId, PATROL_SAVE | GV_C6FLAG);
      RankSparingCopy (NodeId, Ch, Rank, NextSpareRank);

      SetRankState (NodeId, Ch, NextSpareRank, SPARERANKSTATUS_INPROGRESS);
      SetRankState (NodeId, Ch, Rank, NONSPARESTATUS_DOSPARECOPY);
      SetNodeProgress (NodeId, TRUE);
    }

    IsEventHandled = TRUE;
// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#if AMI_RAS_ENHANCEMENTS_AND_HOOKS_SUPPORT
    OemReportStatusCode(mSpareFailedDimmInfo, SPARE_COPY_START);
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support
  }
  RAS_DEBUG((LEVEL_BASIC_FLOW, "[Rank] new event end\n"));
  return IsEventHandled;
}


/**
  Check if Rank Sparing is possible for this node and
  do Rank Sparing

  @param NodeId    - Memory controller ID
  @param EventType - NEW_EVENTS or EVENTS_IN_PROGRESS

  @retval TRUE  --  the event has been handled.
  @retval FALSE --  the event has not been handled.

  **/
BOOLEAN
EFIAPI
CheckAndHandleRankSparing(
  IN UINT8 NodeId,
  IN EVENT_TYPE EventType
)
{
  BOOLEAN    IsEventHandled = FALSE;

  if (EventType == NEW_EVENTS) {
    IsEventHandled = CheckAndHandleNewRankSparingEvent (NodeId);
  }

  if (EventType == EVENTS_IN_PROGRESS){
    IsEventHandled = CheckAndHandleRankSparingDone (NodeId);
  }

  return IsEventHandled;
}
