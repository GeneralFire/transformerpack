/** @file
  Implementation of SDDC library.

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
#include <IioUniversalData.h>
#include <Library/ImcRoutingLib.h>
#include <Library/RasDebugLib.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/RasGlobalDataLib.h>
#include <Library/PlatSSRLib.h>
#include <Library/MemMcIpRasLib.h>
#include <Library/CpuAndRevisionLib.h>
#include "ImcRoutingPrivate.h"
#include "SDDCPrivate.h"
// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#if AMI_RAS_ENHANCEMENTS_AND_HOOKS_SUPPORT
#include <Library/BaseMemoryLib.h>
FAILED_DIMM_INFO          mSddcFailedDimmInfo;
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support

SDDC_PLUS_ONE_STRUCTURE   *mSDDCPlusOne = NULL;

#define PLUS1_RANK_MCDDC_DP_LIST      {PLUS1_RANK0_MCDDC_DP_REG, PLUS1_RANK1_MCDDC_DP_REG, PLUS1_RANK2_MCDDC_DP_REG, \
                                        PLUS1_RANK3_MCDDC_DP_REG, PLUS1_RANK4_MCDDC_DP_REG, PLUS1_RANK5_MCDDC_DP_REG, \
                                          PLUS1_RANK6_MCDDC_DP_REG, PLUS1_RANK7_MCDDC_DP_REG  }

SDDC_PLUSONE_STATUS2
EFIAPI
GetSDDCState (
  IN  UINT8   Node,
  IN  UINT8   ChOnNode,
  IN  UINT8   Rank
  )
{
  if (mSDDCPlusOne != NULL) {
    return mSDDCPlusOne->SddcPlusOneStatus[Node][ChOnNode][Rank];
  } else {
    //
    //if SDDC+1 is not enabled, still force IsSDDCPlus1Rank return FALSE.
    //
    return SddcPlusOneAvailable;
  }
}

VOID
EFIAPI
SetSDDCState (
  IN   UINT8    Node,
  IN   UINT8    ChOnNode,
  IN   UINT8    Rank,
  IN   SDDC_PLUSONE_STATUS2    State
  )
{
  mSDDCPlusOne->SddcPlusOneStatus[Node][ChOnNode][Rank] = State;
}

BOOLEAN
EFIAPI
CheckSDDCState (
  IN    UINT8                   Node,
  IN    SDDC_PLUSONE_STATUS2    State,
  OUT   UINT8                  *ChOnMc,
  OUT   UINT8                  *Rank
  )
{
  UINT8        c;
  UINT8        r;
  UINT8        Flag = FALSE;

  for (c = 0; c < MAX_MC_CH; c ++) {
    if (Flag != 0) {
      break;
    }

    for (r = 0; r < (MAX_RANK_CH * SUB_CH); r ++) {
      if (GetSDDCState (Node, c, r) == State) {
        *ChOnMc = c;
        *Rank = r;
        Flag = TRUE;
        RAS_DEBUG((LEVEL_FUNC_FLOW, "[SDDC+1]found given state rank-- node:%d ch:%d,  rank:%d\n", Node, *ChOnMc, *Rank));
        break;
      }
    }
  }
  return Flag;
}

BOOLEAN
EFIAPI
FindFailSDDCRank (
  IN    UINT8    Node,
  OUT   UINT8   *FoundCh,
  OUT   UINT8   *FoundRank
  )
{
  BOOLEAN    FailedRankFound = FALSE;
  UINT8      ChOnMc;
  UINT8      Rank;

  for (ChOnMc = 0; ChOnMc < MAX_MC_CH; ChOnMc ++) {
    if (IsChannelPresent (Node,ChOnMc) == FALSE) {
      continue;
    }

    for (Rank = 0; Rank < (MAX_RANK_CH * SUB_CH); Rank++) {
      if ((IsErrorExcdForThisRank(Node, ChOnMc, Rank)) &&
          (!IsDcpmmRank (NODE_TO_SKT (Node), NODECHA_TO_SKTCHA (Node, ChOnMc), Rank))       &&
          (GetSDDCState (Node, ChOnMc, Rank) == SddcPlusOneAvailable)) {
        FailedRankFound = TRUE;
        *FoundCh = ChOnMc;
        *FoundRank = Rank;
        RAS_DEBUG((LEVEL_FUNC_FLOW, "[SDDC+1]found failed rank-- ch:%d,  rank:%d\n", ChOnMc, Rank));
        break;
      }
    }

    if (FailedRankFound) {
      break;
    }
  }

  return FailedRankFound;
}

BOOLEAN
EFIAPI
HandleSddcPlusOneEventDone (
  IN  UINT8   Node
  )
{

  UINT8   ChOnMc;
  UINT8   Rank;
  UINT8   ChOnMcBuddy;
  UINT8   BuddyRank;
  UINT8   Flag = FALSE;

  RAS_DEBUG((LEVEL_BASIC_FLOW,"[SDDC+1] pending event start \n"));

  if (CheckSDDCState (Node, SddcPlusOneInProgress, &ChOnMc, &Rank)) {
    SetSDDCState (Node, ChOnMc, Rank, SddcPlusOneDone);

    if (CheckSDDCState (Node, SddcPlusOneBuddyInProgress, &ChOnMcBuddy, &BuddyRank)) {
      if (ChOnMc != ChOnMcBuddy) {
        RAS_DEBUG((LEVEL_BASIC_FLOW,"[SDDC+1] Error! Primary and Buddy are not at the same channel \n"));
      }
      ClearSparing2LmWa (Node, ChOnMcBuddy);
      SetSDDCState (Node, ChOnMcBuddy, BuddyRank, SddcPlusOneDone);
    }

    PatrolScrubInfoSaveOrRestore (Node, PATROL_RESTORE | GV_C6FLAG);
    SetErrorThresholdAfterCopyComplete (Node, ChOnMc);
    SetNodeProgress (Node, FALSE);
    Flag = TRUE;
  }

// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#if AMI_RAS_ENHANCEMENTS_AND_HOOKS_SUPPORT
  OemReportStatusCode(mSddcFailedDimmInfo, SDDC_COMPLETE);
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support
  RAS_DEBUG((LEVEL_BASIC_FLOW, "[SDDC+1] pending event end \n"));
  return Flag;
}

BOOLEAN
EFIAPI
HandleNewSddcPlusOneEvents (
  IN UINT8     NodeId
  )
{
  BOOLEAN                     IsEventHandled = FALSE;
  UINT8                       Socket;
  UINT8                       Mc;
  UINT8                       ChOnMc;
  UINT8                       ChOnSkt;
  UINT8                       Rank;
  UINT8                       FoundCh;
  UINT8                       FoundRank;
  FAILED_DIMM_INFO            FailedDimmInfo;
  EFI_STATUS                  Status;
  UINT8                       RegionType;
  UINT8                       Plus1Rank = 0;
  UINT8                       PeerRank;
  UINT8                       BuddyCs = 0;
  UINT16                      SktChBitMap;
  BOOLEAN                     BuddyCsEn = FALSE;
#ifndef GNRSRF_HOST // GNRSRF_TODO - 22011066572
  UINT32                      Plus1RankMcddcReg[] = PLUS1_RANK_MCDDC_DP_LIST;
  PLUS1_RANK0_MCDDC_DP_STRUCT Plus1RankOffset;
#endif // !GNRSRF_HOST
  SPARING_REGION_STRUCT       SparingRegion;
  BOOLEAN                     Is14nmCpu;

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[SDDC+1] new event start \n"));
  if (FindFailSDDCRank (NodeId, &FoundCh, &FoundRank)) {
    if ((!IsThereAnyPotentialPairRank (NodeId, FoundCh, FoundRank)) && \
        (FindBankVlsPairRank (NodeId, FoundCh, FoundRank, NULL) == EFI_SUCCESS)) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "[SDDC+1]Found bank VLS within single rank, skip +1\n"));
      return FALSE;
    }

    Socket = NODE_TO_SKT(NodeId);
    Mc = NODE_TO_MC(NodeId);
    SktChBitMap = 0;

    //
    //if multi error happens in one node, clear all errors include the one to be handled.
    //
    for (ChOnMc = 0; ChOnMc < MAX_MC_CH; ChOnMc++) {
      ChOnSkt = NODECHA_TO_SKTCHA (NodeId, ChOnMc);
      if (IsChannelPresentPerSocket (Socket, ChOnSkt) ) {
        SktChBitMap |= 1 << ChOnSkt;
      }
      for (Rank = 0; Rank < (MAX_RANK_CH * SUB_CH); Rank++) {
        ClearRankErrors (Socket, ChOnSkt, Rank);
      }
    }

    Status = GetChannelErrorInfo (NodeId, FoundCh, (UINT64)-1, &FailedDimmInfo);
    ChOnSkt = NODECHA_TO_SKTCHA (NodeId, FoundCh);
    if (!EFI_ERROR (Status)) {
      Is14nmCpu = IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL);
      //
      //remove following check due to silicon bug https://vthsd.sc.intel.com/hsd/skylake_server/default.aspx#bugeco/default.aspx?bugeco_id=312014.
      //CPX always log in buddy rank/bank in retry_rd_err. the rank return from retry_rd_err mismatch to the errcnt/overflow.
      //
      if (Is14nmCpu == FALSE) {
        if (FailedDimmInfo.FailedRank != FoundRank) {
          RAS_DEBUG ((LEVEL_BASIC_FLOW, "\nInvalid FoundRank\n"));
          return FALSE;
         }
      }
// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#if AMI_RAS_ENHANCEMENTS_AND_HOOKS_SUPPORT
        {
            BOOLEAN   SkipEventHandlers = FALSE;
            UINT8     ErrorEvent = ERROR_EVENT_SDDC;
            
            CopyMem (&mSddcFailedDimmInfo, &FailedDimmInfo, sizeof (FAILED_DIMM_INFO));
            OemReadyToTakeAction(ErrorEvent, &mSddcFailedDimmInfo, &SkipEventHandlers); // Oem Hook To decide the Error action based on the Error data
            if (SkipEventHandlers == TRUE) {
                DEBUG((DEBUG_INFO,"OEM request..Skipping SDDC Plus One Event!\n"));
                return IsEventHandled;
            }
        }
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support
      RegionType = FailedDimmInfo.RegionType;
      Plus1Rank = FailedDimmInfo.FailedRank;
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "[SDDC+1]Error region type:%d  Plus1Rank:%d\n", RegionType, Plus1Rank));

#ifndef GNRSRF_HOST // GNRSRF_TODO - 22011066572
      RAS_ASSERT (Plus1Rank < ARRAY_SIZE (Plus1RankMcddcReg));
      if (Plus1Rank >= ARRAY_SIZE (Plus1RankMcddcReg)) {
        RAS_DEBUG ((LEVEL_BASIC_FLOW, "\n Unsupported Rank access\n"));
        return FALSE;
      }
#endif // !GNRSRF_HOST

      SetSDDCState (NodeId, FoundCh, Plus1Rank, SddcPlusOneInProgress);
      if (RegionType == ErrInIndReg) {

        if(IsRankInBankVLS (Socket,ChOnSkt,Plus1Rank,&PeerRank)) {
          //
          //test case 3.d -- error happen at non-VLS region, but this rank has build bank VLS with other ranks.
          //
          BuddyCs = PeerRank;
          BuddyCsEn = TRUE;
          SetSDDCState (NodeId, FoundCh, BuddyCs, SddcPlusOneBuddyInProgress);
        } else {
          //
          //x4 NO-ADDDC case
          //
          BuddyCs = 0;
          BuddyCsEn = FALSE;
        }
      } else if ((RegionType == ErrInSVLSFailureReg) || (RegionType == ErrInSVLSBuddyReg)) {
        //
        //x8 SVL case
        //

        //BuddyCs = (Plus1Rank + 4) % 8;
        //BuddyCsEn = TRUE;
        //SetSDDCState(NodeId,FoundCh,BuddyCs,SddcPlusOneBuddyInProgress);
        RAS_DEBUG((LEVEL_BASIC_FLOW, "Error on SVL Rank. Disable SMI on Rank %d\n", Rank));
        //DisableRankError (NodeId, FoundCh, FoundRank);
        return FALSE; //disable this case as silicon limit.
      } else {
        //
        //error happens at VLS region.
        //
        BuddyCs = FailedDimmInfo.PeerRank;
        BuddyCsEn = TRUE;
        SetSDDCState (NodeId, FoundCh, BuddyCs, SddcPlusOneBuddyInProgress);
      }
      //
      // Program SPARING_CONTROL_SOURCE, SPARING_CONTROL_DESTINATION and SPARING_CONTROL
      //

      SetSparingCtlSrc (Socket, Mc, ChOnSkt, Plus1Rank, 0xff, 0xff, BuddyCs, (UINT8) BuddyCsEn);

      SetSparingCtlDest (Socket, Mc, ChOnSkt, Plus1Rank);

      //SetSpareInterval (Socket, Mc, SktChBitMap);

      SetSparingCtrl (Socket, Mc, ChOnSkt, FALSE, FALSE, TRUE, 1, FALSE, 0xff, 0, 0, 0 );

#ifndef GNRSRF_HOST // GNRSRF_TODO - 22011066572
      //
      // Program Plus1Rank
      //
      Plus1RankOffset.Data = (UINT8) ReadCpuCsr (Socket, ChOnSkt, (Plus1RankMcddcReg[Plus1Rank]));
      Plus1RankOffset.Bits.faildevice = FailedDimmInfo.FailedDev;
      WriteCpuCsr (Socket, ChOnSkt, (Plus1RankMcddcReg[Plus1Rank]), Plus1RankOffset.Data);

      if (BuddyCsEn == TRUE) {
        Plus1RankOffset.Data = (UINT8) ReadCpuCsr (Socket, ChOnSkt, (Plus1RankMcddcReg[BuddyCs]));
        Plus1RankOffset.Bits.faildevice = FailedDimmInfo.FailedDev;
        WriteCpuCsr (Socket, ChOnSkt, (Plus1RankMcddcReg[BuddyCs]), Plus1RankOffset.Data);
      }
#endif // GNRSRF_HOST

      //
      // Program MCMTR.addc_mode = 1
      //
      SetMcmtr (Socket, Mc, ChOnSkt, 1, 0xff);

      SetErrorThresholdBeforeSpareCopyStart (NodeId, FoundCh, MAX_RANK_ERROR_THRESHOLD);
      PatrolScrubInfoSaveOrRestore (NodeId, PATROL_SAVE | GV_C6FLAG);
      SparingRegion.NodeId = NodeId;
      SparingRegion.ChOnNode = FoundCh;
      VlsSparingCopy (&SparingRegion, SddcPlusOne);
    }

// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#if AMI_RAS_ENHANCEMENTS_AND_HOOKS_SUPPORT
    OemReportStatusCode(mSddcFailedDimmInfo, SDDC_START);
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support

    IsEventHandled = TRUE;
    SetNodeProgress (NodeId, TRUE);
  }

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[SDDC+1] new event end \n"));
  return IsEventHandled;
}

/**
  Check if SDDC Plus One is enabled and perform that

  @param NodeId    - Memory controller ID
  @param EventType - NEW_EVENTS or EVENTS_IN_PROGRESS
  @param *IsEventHandled - ptr to buffer to hold event handled status

  @retval status - EFI_SUCCESS if no failures
  @retval non-zero error code for failures

  @retval IsEventHandled - TRUE -  event was handled
  @retval FALSE - event not handled

**/
BOOLEAN
EFIAPI
CheckAndHandleSddcPlusOne (
  IN UINT8       NodeId,
  IN EVENT_TYPE  EventType
  )
{
  BOOLEAN    IsEventHandled = FALSE;

  RAS_DEBUG((LEVEL_BASIC_FLOW, "[SDDC+1] start \n"));

  if (EventType == NEW_EVENTS) {
    IsEventHandled = HandleNewSddcPlusOneEvents (NodeId);
  }

  if (EventType == EVENTS_IN_PROGRESS) {
     IsEventHandled = HandleSddcPlusOneEventDone (NodeId);
  }

  RAS_DEBUG((LEVEL_BASIC_FLOW, "[SDDC+1] end \n"));
  return IsEventHandled;
}


VOID
EFIAPI
InitializeSDDCPlusOne (
  VOID
  )
{
  IMC_RAS_STRUCTURE   *ImcRas;
  BOOLEAN             NewAllocation;

  ImcRas = GetImcRasData ();

  mSDDCPlusOne = GetRasGlobalData (L"SDDCPlusOneData", sizeof(SDDC_PLUS_ONE_STRUCTURE), &NewAllocation);

  if (NewAllocation) {
    //
    //nothing need to be initialized here.
    //GetRasGlobalData will clear SDDC_PLUS_ONE_STRUCTURE,
    //each rank default is SDDC_PLUSONE_STATUS2.SddcPlusOneAvailable. So don't change SddcPlusOneAvailable value!!!
    //
  }

  ImcRas->RasEventHndlrTable[SDDC_INDEX] = CheckAndHandleSddcPlusOne;
}

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
  )
{
  return (GetSDDCState(Node, ChOnNode, Rank) == SddcPlusOneDone) ? TRUE : FALSE;
}

