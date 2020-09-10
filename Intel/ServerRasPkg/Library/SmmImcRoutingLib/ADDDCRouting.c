/** @file
  Implementation of ADDDC library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2020 Intel Corporation. <BR>

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
#include <Library/PlatMemTopologyLib.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PlatSSRLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/MemMcIpRasLib.h>
#include <Library/PclsLib.h>
#include "ImcRoutingPrivate.h"
#include "ADDDCPrivate.h"
#include <Library/SiliconWorkaroundLib.h>

ADDDC_EVENT_STRUCT         mAdddcEventPendingList[MC_MAX_NODE][MAX_EVT_PENDING];
UINT8                      mAdddcPendingIndex[MC_MAX_NODE];
// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#if AMI_RAS_ENHANCEMENTS_AND_HOOKS_SUPPORT
FAILED_DIMM_INFO           mAddcFailedDimmInfo;
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support

VOID
EFIAPI
PrintOutAdddcEvent (
  ADDDC_EVENT_STRUCT *pEvent
  )
{
  RAS_DEBUG ((LEVEL_BASIC_FLOW,
              "\t AdddcEvent%d: \n\t\tType %d, Region %d \
        (PRank %d : PBank %d) - (BRank %d : BBank %d), FailedDevice %d.\n",
              pEvent->Num,
              pEvent->Type,
              pEvent->SparingRegion.RegionNum,
              pEvent->SparingRegion.FailedRank,
              pEvent->SparingRegion.FailedBank,
              pEvent->SparingRegion.NonFailedRank,
              pEvent->SparingRegion.NonFailedBank,
              pEvent->SparingRegion.FailedDevice));
}

/**
  Initialize ADDDC pending list

**/
VOID
EFIAPI
InitAdddcPendingList (
  VOID
  )
{
  ZeroMem (mAdddcEventPendingList, sizeof (mAdddcEventPendingList));
  ZeroMem (mAdddcPendingIndex, sizeof (mAdddcPendingIndex));
}

/**
  Check whether the pending list is empty

  @param  NodeId          - Node ID

  @retval Status          - TRUE: the list is empty
                          - FALSE: not empty

**/
BOOLEAN
EFIAPI
IsAdddcPendingListEmpty (
  IN UINT8              NodeId
  )
{
  return mAdddcPendingIndex[NodeId] == 0;
}

/**
  Check whether the pending list is full

  @param  NodeId          - Node ID

  @retval Status          - TRUE: the list is full
                          - FALSE: not full

**/
BOOLEAN
EFIAPI
IsAdddcPendingListFull (
  IN UINT8              NodeId
  )
{
  return mAdddcPendingIndex[NodeId] == MAX_EVT_PENDING;
}

/**
  Push one event into the pending list

  @param  NodeId          - Node ID
  @param  Event           - Pointer to the popout event

  @retval Status          - EFI_SUCCESS if no failures

**/
EFI_STATUS
EFIAPI
PushEventToPendingList (
  IN UINT8                      NodeId,
  IN ADDDC_EVENT_STRUCT        *Event
  )
{
  EFI_STATUS Status;
  UINT8      Index;

  Status = EFI_OUT_OF_RESOURCES;
  Index = mAdddcPendingIndex[NodeId];

  if (!IsAdddcPendingListFull (NodeId)) {
 //   mAdddcEventPendingList[NodeId][Index] = Event;
     CopyMem( &mAdddcEventPendingList[NodeId][Index],Event, sizeof(ADDDC_EVENT_STRUCT) );
    Index++;
    Status = EFI_SUCCESS;
  }
  mAdddcPendingIndex[NodeId] = Index;

  return Status;
}

/**
  pop one event from the pending list

  @param  NodeId          - Node ID
  @param  Event           - Pointer to the popout event

  @retval Status          - EFI_SUCCESS if no failures

**/
EFI_STATUS
EFIAPI
PopEventFromPendingList (
             IN  UINT8                      NodeId,
             OUT ADDDC_EVENT_STRUCT         *Event
             )
{
  EFI_STATUS Status;
  UINT8      Index;

  Status = EFI_ABORTED;
  Index = mAdddcPendingIndex[NodeId];
  if (!IsAdddcPendingListEmpty (NodeId)) {
    Index--;
//    *Event = mAdddcEventPendingList[NodeId][Index];
    CopyMem(Event, &mAdddcEventPendingList[NodeId][Index], sizeof(ADDDC_EVENT_STRUCT) );
    Status = EFI_SUCCESS;
  }
  mAdddcPendingIndex[NodeId] = Index;

  return Status;
}

/**
  Handle BANK VLS event

  @param  pAdddcRegion    - Pointer to ADDDC region structure
  @retval Status          - EFI_SUCCESS if no failures

**/
EFI_STATUS
EFIAPI
HandleBankVlsEvent (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  )
{
  EFI_STATUS          Status;
  UINT8               NodeId;
  UINT8               ChOnNode;
  ADDDC_EVENT_STRUCT  AdddcEvent0;
  ADDDC_EVENT_STRUCT  AdddcEvent1;

  NodeId    = SparingRegionPtr->NodeId;
  ChOnNode  = SparingRegionPtr->ChOnNode;

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[ADDDC]: HandleBankVlsEvent()... Start\n"));
  ZeroMem (&AdddcEvent0, sizeof (ADDDC_EVENT_STRUCT));
  ZeroMem (&AdddcEvent1, sizeof (ADDDC_EVENT_STRUCT));

  //
  //Get the free region number
  //
  Status = GetAvailableRegion (NodeId,
             ChOnNode,
             &SparingRegionPtr->RegionNum);

  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "[ADDDC]: Can't find available region.\n"));
    return Status;
  }

  Status = GetNewNonFailedRankBank (
             NodeId,
             ChOnNode,
             SparingRegionPtr->FailedRank,
             SparingRegionPtr->FailedBank,
             &SparingRegionPtr->NonFailedRank,
             &SparingRegionPtr->NonFailedBank);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  SparingRegionPtr->Valid = TRUE;
  CopyMem (&AdddcEvent0.SparingRegion, SparingRegionPtr, sizeof(SPARING_REGION_STRUCT));
  AdddcEvent0.Type = BankVlsEvent;
  AdddcEvent0.Num = 0;
  AdddcEvent0.ActionFunc = ActionOnBankVls;
  AdddcEvent0.CombinedSmi = FALSE;
  AdddcEvent0.Valid = TRUE;
  PrintOutAdddcEvent (&AdddcEvent0);

  //AdddcEvent1 = AdddcEvent0;
  CopyMem (&AdddcEvent1, &AdddcEvent0, sizeof(ADDDC_EVENT_STRUCT));
  AdddcEvent1.Num = 1;
  AdddcEvent1.ActionFunc = CleanPreVlsEvent;
  PrintOutAdddcEvent (&AdddcEvent1);

  //
  //Push the cleanup task
  //
  Status = PushEventToPendingList (SparingRegionPtr->NodeId,
             &AdddcEvent1);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  //Push the BANK VLS task
  //
  Status = PushEventToPendingList (SparingRegionPtr->NodeId,
             &AdddcEvent0);

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[ADDDC]: HandleBankVlsEvent()... End\n"));

  return Status;
}

/**
  Handle BANK VLS reverse and rebuddy event

  @param  pAdddcRegion    - Pointer to ADDDC region structure
  @retval Status          - EFI_SUCCESS if no failures

**/
EFI_STATUS
HandleBankVlsRevsEvent (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  )
{
  EFI_STATUS          Status;
  ADDDC_EVENT_STRUCT  AdddcEvent0;
  ADDDC_EVENT_STRUCT  AdddcEvent1;
  ADDDC_EVENT_STRUCT  AdddcEvent2;
  ADDDC_EVENT_STRUCT  AdddcEvent3;
  UINT8               NodeId;
  UINT8               ChOnNode;
  UINT8               RegionNum;
  UINT8               Rank;
  UINT8               BuddyRank;
  UINT8               Bank;
  UINT8               BuddyBank;
  UINT8               FailedDevice;

  NodeId       = SparingRegionPtr->NodeId;
  ChOnNode     = SparingRegionPtr->ChOnNode;
  Rank         = SparingRegionPtr->FailedRank;
  Bank         = SparingRegionPtr->FailedBank;
  FailedDevice = SparingRegionPtr->FailedDevice;

  ZeroMem (&AdddcEvent0, sizeof (ADDDC_EVENT_STRUCT));
  ZeroMem (&AdddcEvent1, sizeof (ADDDC_EVENT_STRUCT));
  ZeroMem (&AdddcEvent2, sizeof (ADDDC_EVENT_STRUCT));

  RAS_DEBUG ( (LEVEL_BASIC_FLOW, "[ADDDC]: HandleBankVlsRevsEvent()... Start\n"));
  //
  //Locate the region which has done VLS for the rank & bank
  //
  Status = FindRegionWithRankBank (NodeId, ChOnNode, Rank, Bank, &RegionNum);
  RAS_DEBUG ( (LEVEL_BASIC_FLOW, "\t FindRegionWithRankBank(Node %d, Ch %d, Rank %d, Bank %d, RegionNum = %d) = %r\n",
              NodeId, ChOnNode, Rank, Bank, RegionNum, Status));
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  //Fill the primary and buddy rank/bank info
  //
  FillRegionStruct (NodeId, ChOnNode, RegionNum, SparingRegionPtr);
  //
  //Step 1: reverse the previous BANK VLS
  //
  SparingRegionPtr->RegionNum = RegionNum; // Keep using this region number
  SparingRegionPtr->ReverseRegion = 1;
  SparingRegionPtr->Valid = TRUE;
  AdddcEvent0.Type = BankVlsReverseEvent;
  AdddcEvent0.Num = 0;
  CopyMem (&AdddcEvent0.SparingRegion, SparingRegionPtr, sizeof(SPARING_REGION_STRUCT));
  AdddcEvent0.CombinedSmi = FALSE;
  AdddcEvent0.ActionFunc = ActionOnBankVlsReverse;
  AdddcEvent0.Valid = TRUE;
  PrintOutAdddcEvent (&AdddcEvent0);

  //
  // Mask the ranks as busy so that we won't choose them as buddy in alternate step 2
  //
  SetRankMode (NodeId,
    ChOnNode,
    AdddcEvent0.SparingRegion.FailedRank,
    RankVlsBusyMode
    );

  SetRankMode (NodeId,
    ChOnNode,
    AdddcEvent0.SparingRegion.NonFailedRank,
    RankVlsBusyMode
    );

  //
  //Alternate Step 2: Try BANK VLS for the primary bank
  //
  SparingRegionPtr->ReverseRegion = 0;
  Rank = AdddcEvent0.SparingRegion.FailedRank;
  Bank = AdddcEvent0.SparingRegion.FailedBank;

  Status = GetNewNonFailedRankBank (NodeId,
             ChOnNode,
             Rank,
             Bank,
             &BuddyRank,
             &BuddyBank
             );
  if (EFI_ERROR (Status)) {
    goto Error;
  }
  //
  //Change the new buddy rank's mode
  //
  SetRankMode (NodeId, ChOnNode, BuddyRank, RankVlsBusyMode);
  //
  //Change the new buddy bank's mode
  //
  SetBankMode (NodeId, ChOnNode, BuddyRank, BuddyBank, BankVlsBuddyMode);

  SparingRegionPtr->RegionNum = RegionNum; // Keep using the previous region used for reversing
  SparingRegionPtr->FailedRank = Rank;
  SparingRegionPtr->FailedBank = Bank;
  SparingRegionPtr->NonFailedRank = BuddyRank;
  SparingRegionPtr->NonFailedBank = BuddyBank;

  AdddcEvent1.Type = BankVlsEvent;
  AdddcEvent1.Num = 1;
  CopyMem (&AdddcEvent1.SparingRegion, SparingRegionPtr, sizeof(SPARING_REGION_STRUCT));
  AdddcEvent1.ActionFunc = ActionOnBankVls;
  AdddcEvent1.CombinedSmi = FALSE;
  AdddcEvent1.Valid = TRUE;
  PrintOutAdddcEvent (&AdddcEvent1);

  //
  //Alternate Step 3: Try BANK VLS for the buddy bank
  //
  Status = GetAvailableRegion (NodeId,
             ChOnNode,
             &SparingRegionPtr->RegionNum);
  if (EFI_ERROR (Status)) {
    //
    // Can't find available region
    //
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "\t: Can't find available region.\n"));
    goto Error;
  }

  Rank = AdddcEvent0.SparingRegion.NonFailedRank;
  Bank = AdddcEvent0.SparingRegion.NonFailedBank;

  //
  // Set the failed rank to NonRankVlsMode firstly.
  // If there is no available pair rank for the failed rank, we have to select itself as buddy rank.
  //
  SetRankMode (NodeId,
    ChOnNode,
    Rank,
    NonRankVlsMode
    );

  Status = GetNewNonFailedRankBank (NodeId,
             ChOnNode,
             Rank,
             Bank,
             &BuddyRank,
             &BuddyBank);
  if (EFI_ERROR (Status)) {
    goto Error;
  }
  SparingRegionPtr->FailedRank = Rank;
  SparingRegionPtr->FailedBank = Bank;
  SparingRegionPtr->NonFailedRank = BuddyRank;
  SparingRegionPtr->NonFailedBank = BuddyBank;
  SparingRegionPtr->FailedDevice = FailedDevice;

  AdddcEvent2.Type = BankVlsEvent;
  AdddcEvent2.Num = 2;
  CopyMem (&AdddcEvent2.SparingRegion, SparingRegionPtr, sizeof(SPARING_REGION_STRUCT));
  AdddcEvent1.CombinedSmi = FALSE;
  AdddcEvent2.ActionFunc = ActionOnBankVls;
  AdddcEvent2.Valid = TRUE;
  PrintOutAdddcEvent (&AdddcEvent2);
  //
  // AdddcEvent3 = AdddcEvent2;
  //
  CopyMem (&AdddcEvent3, &AdddcEvent2, sizeof(ADDDC_EVENT_STRUCT));
  AdddcEvent3.Num = 3;
  AdddcEvent3.ActionFunc = CleanPreVlsEvent;
  AdddcEvent3.CombinedSmi = FALSE;
  AdddcEvent3.Valid = TRUE;
  PrintOutAdddcEvent (&AdddcEvent3);

  Status = PushEventToPendingList (NodeId, &AdddcEvent3);
  if (EFI_ERROR (Status)) {
    goto Error;
  }
  Status = PushEventToPendingList (NodeId, &AdddcEvent2);
  if (EFI_ERROR (Status)) {
    goto Error;
  }
  Status = PushEventToPendingList (NodeId, &AdddcEvent1);
  if (EFI_ERROR (Status)) {
    goto Error;
  }
  Status = PushEventToPendingList (NodeId, &AdddcEvent0);
  if (EFI_ERROR (Status)) {
    goto Error;
  }

  RAS_DEBUG ( (LEVEL_BASIC_FLOW, "[ADDDC]: HandleBankVlsRevsEvent()... End\n"));
  //return Status;
Error:
  if (AdddcEvent0.Valid) {
    //
    //Reverse the occupied ranks in event 0
    //
    SetRankMode (NodeId,
      ChOnNode,
      AdddcEvent0.SparingRegion.FailedRank,
      NonRankVlsMode);

    SetRankMode (NodeId,
      ChOnNode,
      AdddcEvent0.SparingRegion.NonFailedRank,
      NonRankVlsMode);
  }
  if (AdddcEvent1.Valid) {
    //
    //Reverse the occupied rank and bank in event 1
    //
    SetRankMode (NodeId,
      ChOnNode,
      AdddcEvent1.SparingRegion.NonFailedRank,
      NonRankVlsMode);

    SetBankMode (NodeId,
      ChOnNode,
      AdddcEvent1.SparingRegion.NonFailedRank,
      AdddcEvent1.SparingRegion.NonFailedBank,
      NonBankVlsMode);
  }
  return Status;
}

/**
  Promote BANK VLS to RANK VLS

  @param  pAdddcRegion    - Pointer to ADDDC region structure
  @retval Status          - EFI_SUCCESS if no failures

**/
EFI_STATUS
EFIAPI
HandleBank2RankVlsEvent (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  )
{
  EFI_STATUS          Status;
  ADDDC_EVENT_STRUCT  AdddcEvent0;
  ADDDC_EVENT_STRUCT  AdddcEvent1;
  ADDDC_EVENT_STRUCT  AdddcEvent2;
  UINT8               NodeId;
  UINT8               ChOnNode;
  UINT8               RegionNum;
  UINT8               Rank;
  UINT8               Bank;

  // Check whether CPU support Rank VLS or not. Only advanced RAS support Rank VLS
  if (IsRankVlsSupport() == FALSE) {
    return EFI_UNSUPPORTED;
  }

  NodeId    = SparingRegionPtr->NodeId;
  ChOnNode  = SparingRegionPtr->ChOnNode;
  Rank      = SparingRegionPtr->FailedRank;
  Bank      = SparingRegionPtr->FailedBank;

  ZeroMem (&AdddcEvent0, sizeof (ADDDC_EVENT_STRUCT));
  ZeroMem (&AdddcEvent1, sizeof (ADDDC_EVENT_STRUCT));
  ZeroMem (&AdddcEvent2, sizeof (ADDDC_EVENT_STRUCT));
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[ADDDC]: HandleBank2RankVlsEvent()... Start\n"));
  //
  //Locate the region which has done VLS for the rank & bank
  //
  Status = FindRegionWithRank (NodeId, ChOnNode, Rank, &RegionNum);
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "\t FindRegionWithRank(Node %d, Ch %d, Rank %d, RegionNum = %d.) = %r \n",
              NodeId, ChOnNode, Rank, RegionNum, Status));
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  //Fill the primary and buddy rank/bank info
  //
  FillRegionStruct (NodeId, ChOnNode, RegionNum, SparingRegionPtr);
  SparingRegionPtr->ReverseRegion = 1;
  AdddcEvent0.Type = BankVlsReverseEvent;
  AdddcEvent0.Num = 0;
  CopyMem (&AdddcEvent0.SparingRegion, SparingRegionPtr, sizeof(SPARING_REGION_STRUCT));
  AdddcEvent0.ActionFunc = ActionOnBankVlsReverse;
  AdddcEvent0.CombinedSmi = FALSE;
  PrintOutAdddcEvent (&AdddcEvent0);

  //
  //Clear the reverse flag
  //
  SparingRegionPtr->ReverseRegion = 0;
  AdddcEvent1.Type = RankVlsEvent;
  AdddcEvent1.Num = 1;
  CopyMem (&AdddcEvent1.SparingRegion, SparingRegionPtr, sizeof(SPARING_REGION_STRUCT));
  AdddcEvent1.ActionFunc = ActionOnRankVls;
  AdddcEvent1.CombinedSmi = FALSE;
  PrintOutAdddcEvent (&AdddcEvent1);

//  AdddcEvent2 = AdddcEvent1;
  CopyMem (&AdddcEvent2, &AdddcEvent1, sizeof(ADDDC_EVENT_STRUCT));
  AdddcEvent2.Num = 2;
  AdddcEvent2.CombinedSmi = FALSE;
  AdddcEvent2.ActionFunc = CleanPreVlsEvent;
  PrintOutAdddcEvent (&AdddcEvent2);

  Status = PushEventToPendingList (NodeId, &AdddcEvent2);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = PushEventToPendingList (NodeId, &AdddcEvent1);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = PushEventToPendingList (NodeId, &AdddcEvent0);

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[ADDDC]: HandleBank2RankVlsEvent()... End\n"));

  return Status;
}

/**
  Create a Rank level VLS event

  @param  pAdddcRegion    - Pointer to ADDDC region structure
  @retval Status          - EFI_SUCCESS if no failures

**/
EFI_STATUS
EFIAPI
HandleRankVlsEvent (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  )
{
  EFI_STATUS          Status;
  ADDDC_EVENT_STRUCT  AdddcEvent0;
  ADDDC_EVENT_STRUCT  AdddcEvent1;
  UINT8               NodeId;
  UINT8               ChOnNode;

  NodeId    = SparingRegionPtr->NodeId;
  ChOnNode  = SparingRegionPtr->ChOnNode;

  ZeroMem (&AdddcEvent0, sizeof (ADDDC_EVENT_STRUCT));
  ZeroMem (&AdddcEvent1, sizeof (ADDDC_EVENT_STRUCT));

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[ADDDC]: HandleRankVlsEvent()... Start\n"));

  //
  //Get the free region number
  //
  Status = GetAvailableRegion (NodeId,
             ChOnNode,
             &SparingRegionPtr->RegionNum);

  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "[ADDDC]: Can't find available region.\n"));
    return Status;
  }

  Status = GetNewNonFailedRankBank (
             NodeId,
             ChOnNode,
             SparingRegionPtr->FailedRank,
             SparingRegionPtr->FailedBank,
             &SparingRegionPtr->NonFailedRank,
             &SparingRegionPtr->NonFailedBank);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  SparingRegionPtr->Valid = TRUE;
  CopyMem (&AdddcEvent0.SparingRegion, SparingRegionPtr, sizeof(SPARING_REGION_STRUCT));
  //
  //Fill the primary and buddy rank/bank info
  //
  SparingRegionPtr->ReverseRegion = 0;
  AdddcEvent0.Type = RankVlsEvent;
  AdddcEvent0.Num = 0;
  CopyMem (&AdddcEvent0.SparingRegion, SparingRegionPtr, sizeof(SPARING_REGION_STRUCT));
  AdddcEvent0.ActionFunc = ActionOnRankVls;
  AdddcEvent0.CombinedSmi = FALSE;
  PrintOutAdddcEvent (&AdddcEvent0);

  CopyMem (&AdddcEvent1, &AdddcEvent0, sizeof(ADDDC_EVENT_STRUCT));
  AdddcEvent1.Num = 1;
  AdddcEvent1.CombinedSmi = FALSE;
  AdddcEvent1.ActionFunc = CleanPreVlsEvent;
  PrintOutAdddcEvent (&AdddcEvent1);

  Status = PushEventToPendingList (NodeId, &AdddcEvent1);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = PushEventToPendingList (NodeId, &AdddcEvent0);

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[ADDDC]: HandleRankVlsEvent()... End\n"));

  return Status;
}

/**
  Hanle RANK VLS reverse and rebuddy event.

  @param  pAdddcRegion    - Pointer to ADDDC region structure
  @retval Status          - EFI_SUCCESS if no failures

**/
EFI_STATUS
HandleRankVlsRevsEvent (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  )
{
  EFI_STATUS          Status;
  ADDDC_EVENT_STRUCT  AdddcEvent0;
  ADDDC_EVENT_STRUCT  AdddcEvent1;
  ADDDC_EVENT_STRUCT  AdddcEvent2;
  ADDDC_EVENT_STRUCT  AdddcEvent3;
  UINT8               NodeId;
  UINT8               ChOnNode;
  UINT8               RegionNum;
  UINT8               Rank, FailedRank;
  UINT8               BuddyRank;
  UINT8               Bank, FailedBank;
  UINT8               BuddyBank;
  UINT8               CurrentStrikeFailedDevice;

  ZeroMem (&AdddcEvent0, sizeof (ADDDC_EVENT_STRUCT));
  ZeroMem (&AdddcEvent1, sizeof (ADDDC_EVENT_STRUCT));
  ZeroMem (&AdddcEvent2, sizeof (ADDDC_EVENT_STRUCT));
  ZeroMem (&AdddcEvent3, sizeof (ADDDC_EVENT_STRUCT));

  NodeId    = SparingRegionPtr->NodeId;
  ChOnNode  = SparingRegionPtr->ChOnNode;
  CurrentStrikeFailedDevice = SparingRegionPtr->FailedDevice;

  FailedRank = SparingRegionPtr->FailedRank; // The failed rank
  FailedBank = SparingRegionPtr->FailedBank; // The failed bank

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[ADDDC]: HandleRankVlsRevsEvent()... Start\n"));
  //
  //Step1: Rank VLS reverse
  //
  //Locate the region which has done VLS for the rank & bank
  //
  Rank = SparingRegionPtr->FailedRank;
  Status = FindRegionWithRank (NodeId, ChOnNode, Rank, &RegionNum);
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "\t FindRegionWithRank(Node %d, Ch %d, Rank %d, RegionNum = %d.) = %r \n",
              NodeId, ChOnNode, Rank, RegionNum, Status));
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  //Fill the primary and buddy rank/bank info
  //
  FillRegionStruct (NodeId, ChOnNode, RegionNum, SparingRegionPtr);
  SparingRegionPtr->ReverseRegion = 1;
  SparingRegionPtr->RegionNum = RegionNum;

  AdddcEvent0.Type = RankVlsReverseEvent;
  AdddcEvent0.Num = 0;
  CopyMem (&AdddcEvent0.SparingRegion, SparingRegionPtr, sizeof(SPARING_REGION_STRUCT));
  AdddcEvent0.ActionFunc = ActionOnRankVlsReverse;
  AdddcEvent0.CombinedSmi = FALSE;
  AdddcEvent0.Valid = TRUE;

  PrintOutAdddcEvent (&AdddcEvent0);

  //
  //Alternate Step 2: Try RANK VLS for the primary bank
  //
  SparingRegionPtr->ReverseRegion = 0;
  Rank = AdddcEvent0.SparingRegion.FailedRank;
  Bank = AdddcEvent0.SparingRegion.FailedBank;
  //
  // Mask the ranks as busy so that we won't choose them as buddy in step 3
  //
  SetRankMode (NodeId,
    ChOnNode,
    AdddcEvent0.SparingRegion.FailedRank,
    RankVlsBusyMode);

  SetRankMode (NodeId,
    ChOnNode,
    AdddcEvent0.SparingRegion.NonFailedRank,
    RankVlsBusyMode);

  Status = GetNewNonFailedRankBank (NodeId,
             ChOnNode,
             Rank,
             Bank,
             &BuddyRank,
             &BuddyBank);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  //Change the mode of new buddy rank
  //
  SetRankMode (NodeId, ChOnNode, BuddyRank, RankVlsBusyMode);

  SparingRegionPtr->RegionNum = RegionNum; // Keep using the previous region used for reversing

  AdddcEvent1.Type = RankVlsEvent;
  AdddcEvent1.Num = 1;
  CopyMem (&AdddcEvent1.SparingRegion, SparingRegionPtr, sizeof(SPARING_REGION_STRUCT));
  AdddcEvent1.SparingRegion.NonFailedRank = BuddyRank;
  AdddcEvent1.SparingRegion.NonFailedBank = BuddyBank;
  AdddcEvent1.ActionFunc = ActionOnRankVls;
  AdddcEvent1.CombinedSmi = FALSE;
  AdddcEvent1.Valid = TRUE;
  PrintOutAdddcEvent (&AdddcEvent1);

  //
  //Alternate Step 3:Try BANK VLS for the buddy bank
  //
  Status = GetAvailableRegion (NodeId,
             ChOnNode,
             &SparingRegionPtr->RegionNum);
  if (EFI_ERROR (Status)) {
    //
    // Can't find available region
    //
    goto Error;
  }
  Rank = FailedRank;
  Bank = FailedBank;

  Status = GetNewNonFailedRankBank (NodeId,
             ChOnNode,
             Rank,
             Bank,
             &BuddyRank,
             &BuddyBank);
  if (EFI_ERROR (Status)) {
    goto Error;
  }
  SparingRegionPtr->FailedRank = Rank;
  SparingRegionPtr->FailedBank = Bank;
  SparingRegionPtr->NonFailedRank = BuddyRank;
  SparingRegionPtr->NonFailedBank = BuddyBank;
  SparingRegionPtr->FailedDevice = CurrentStrikeFailedDevice;

  AdddcEvent2.Type = BankVlsEvent;
  AdddcEvent2.Num = 2;
  CopyMem (&AdddcEvent2.SparingRegion, SparingRegionPtr, sizeof(SPARING_REGION_STRUCT));
  AdddcEvent2.ActionFunc = ActionOnBankVls;
  AdddcEvent2.CombinedSmi = FALSE;
  AdddcEvent2.Valid = TRUE;
  PrintOutAdddcEvent (&AdddcEvent2);

    //  AdddcEvent3 = AdddcEvent2;
  CopyMem (&AdddcEvent3, &AdddcEvent2, sizeof(ADDDC_EVENT_STRUCT));
  AdddcEvent3.Num = 3;
  AdddcEvent3.ActionFunc = CleanPreVlsEvent;
  AdddcEvent3.Valid = TRUE;
  PrintOutAdddcEvent (&AdddcEvent3);

  Status = PushEventToPendingList (NodeId, &AdddcEvent3);
  if (EFI_ERROR (Status)) {
    goto Error;
  }
  Status = PushEventToPendingList (NodeId, &AdddcEvent2);
  if (EFI_ERROR (Status)) {
    goto Error;
  }
  Status = PushEventToPendingList (NodeId, &AdddcEvent1);
  if (EFI_ERROR (Status)) {
    goto Error;
  }
  Status = PushEventToPendingList (NodeId, &AdddcEvent0);
  if (EFI_ERROR (Status)) {
    goto Error;
  }

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[ADDDC]: HandleRankVlsRevsEvent()... End\n"));
  return Status;
Error:
  if (AdddcEvent1.Valid) {
    SetRankMode (NodeId,
      ChOnNode,
      AdddcEvent1.SparingRegion.NonFailedRank,
      NonRankVlsMode);
  }

  return Status;
}

/**
  Shedule the pending list. Get one event from the pending list and call its action fuction.

  @param  NodeId          - Memory controller ID
  @retval Status          - EFI_SUCCESS if no failures

**/
EFI_STATUS
EFIAPI
SchedulePendingList (
  IN  UINT8                NodeId
  )
{
  EFI_STATUS               Status;
  ADDDC_EVENT_STRUCT       Event;

  Status = EFI_UNSUPPORTED;

  RAS_DEBUG ((LEVEL_BASIC_FLOW,
              "[ADDDC]: ShedulePendingList()... Start\n"));
  while (!IsAdddcPendingListEmpty (NodeId)) {
    //
    //Handle Pending event
    //
    Status = PopEventFromPendingList (NodeId, &Event);
    if (EFI_ERROR (Status)) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW,
                  "[ADDDC]: Failed to get an event from pending list, %r\n", Status));
      return Status;
    }

    if (Event.ActionFunc != NULL) {
      Status = Event.ActionFunc (&Event.SparingRegion);
      RAS_DEBUG ((LEVEL_BASIC_FLOW,
                "[ADDDC]: Event type %d, Action Status %r.\n",
                Event.Type,
                Status));
    }

    if (Event.CombinedSmi) {
      continue;
    } else {
      break;
    }
  }
  RAS_DEBUG ((LEVEL_BASIC_FLOW,
              "[ADDDC]: SchedulePendingList()...End\n"));

  return Status;
}

/**
  The rank is not in VLS, try to handle the error within bank level

  @retval Status   - TRUE - Error existed
                   - FALSE- No error

**/
EFI_STATUS
HandleNonRankVlsStrike (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  )
{
  EFI_STATUS       Status;
  ADDDC_BANK_MODE  BankMode;
  UINT8            BankVlsCnt;
  UINT8            NodeId;
  UINT8            ChOnNode;
  UINT8            Rank;
  UINT8            Bank;
  UINT8            Device;

  Status = EFI_SUCCESS;

  NodeId    = SparingRegionPtr->NodeId;
  ChOnNode  = SparingRegionPtr->ChOnNode;
  Rank      = SparingRegionPtr->FailedRank;
  Bank      = SparingRegionPtr->FailedBank;
  Device    = SparingRegionPtr->FailedDevice;

  RAS_DEBUG ((LEVEL_BASIC_FLOW,
              "[ADDDC]: HandleNonRankVlsStrike() start...\n"));

  BankMode = GetBankMode (NodeId, ChOnNode, Rank, Bank);

  RAS_DEBUG ((LEVEL_BASIC_FLOW,
              "[ADDDC]: NodeId %d, ChOnNode %d, Rank %d, Bank %d, Device %d.\n",
              NodeId, ChOnNode, Rank, Bank, Device));
  RAS_DEBUG ((LEVEL_BASIC_FLOW,
              "[ADDDC]: BankMode is %d.\n", BankMode));
  switch (BankMode) {
    case NonBankVlsMode:
      if (IsErrorExistedInDevice (NodeId, ChOnNode, Rank, Device) && (IsThereAnyPotentialPairRank (NodeId,ChOnNode, Rank))) {
        //
        // Error existed in the device
        //
        RAS_DEBUG ((LEVEL_BASIC_FLOW,
                    "[ADDDC]: Previous error in device %d!\n", Device));
        BankVlsCnt = GetBankVlsCount (NodeId, ChOnNode, Rank);
        RAS_DEBUG ((LEVEL_BASIC_FLOW,
                    "[ADDDC]: BankVlsCnt is %d.\n", BankVlsCnt));
        if (BankVlsCnt < 2) {
          //
          // Try to promote to **RANK** VLS
          //
          RAS_DEBUG ((LEVEL_BASIC_FLOW,
                      "[ADDDC]: Promote BANK to RANK VLS...\n"));
          Status = HandleBank2RankVlsEvent (SparingRegionPtr);
          if (EFI_ERROR (Status)) {
            //
            //Try next memory sparing feature
            //
            RAS_DEBUG ((LEVEL_BASIC_FLOW,
                        "[ADDDC]: HandleBank2RankVlsEvent() Status = %r, try next memory sparing feature...\n", Status));
            Status = StartAdddcPlusOneState (SparingRegionPtr);
          }
        } else {
          //
          //Try next memory sparing feature
          //
          RAS_DEBUG ((LEVEL_BASIC_FLOW,
                      "[ADDDC]: Multiple BANK VLS happened previously, try next memory sparing feature...\n"));
          Status = StartAdddcPlusOneState (SparingRegionPtr);
        }
      } else { // No previous error in the device or single rank ADDDC
        RAS_DEBUG ((LEVEL_BASIC_FLOW,
                    "[ADDDC]: No previous error in device %d.\n", Device));
        RAS_DEBUG ((LEVEL_BASIC_FLOW,
                    "[ADDDC]: Call HandleBankVlsEvent()...\n"));
        Status = HandleBankVlsEvent (SparingRegionPtr);
        if (EFI_ERROR (Status)) {
          //
          //Not enough region, try next memory sparing feature
          //
          RAS_DEBUG ((LEVEL_BASIC_FLOW,
                      "[ADDDC]: HandleBankVlsEvent() Status = %r, try next memory sparing feature...\n", Status));
          Status = StartSddcPlusOneState (SparingRegionPtr);
        }
      }
      break;
    case BankVlsPrimaryMode:
      //
      // Try next memory sparing feature
      //
      Status = StartAdddcPlusOneState (SparingRegionPtr);
      break;
    case BankVlsBuddyMode:
      BankVlsCnt = GetBankVlsCount (NodeId, ChOnNode, Rank);
      RAS_DEBUG ((LEVEL_BASIC_FLOW,
                  "[ADDDC]: BankVlsCnt = %d.\n", BankVlsCnt));
      if (BankVlsCnt < 2) {
        //
        //Try BANK VLS reverse
        //
        RAS_DEBUG ((LEVEL_BASIC_FLOW,
                    "[ADDDC]: Try BANK VLS Reverse...\n"));
        Status = HandleBankVlsRevsEvent (SparingRegionPtr);
        if (EFI_ERROR (Status)) {
          //
          //Try next memory sparing feature
          //
          RAS_DEBUG ((LEVEL_BASIC_FLOW,
                      "[ADDDC]: Status %r, try next memory sparing feature...\n", Status));
          Status = StartAdddcPlusOneState (SparingRegionPtr);
        }
      } else { // more than two bank VLS in the rank
        //
        // SW policy: hard to reverse more than two bank VLS, switch to next memory sparing feature
        //
        RAS_DEBUG ((LEVEL_BASIC_FLOW,
                    "[ADDDC]: Try next memory sparing feature...\n", Status));
        Status = StartAdddcPlusOneState (SparingRegionPtr);
      }
      break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
  }

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[ADDDC]: HandleNonRankVlsStrike() Status %r.\n", Status));

  return Status;
}


/**
  This Function overrides the Intel POR of doing Bank VLS first before attempting
  Rank level VLS. It can be optionally enabled if it is required to not attempt BankVLS operation
  and always choose to perform VLS at a Rank level.

  @retval Status   - EFI_SUCCESS - Successfully created Rank VLS region
                   - EFI_ERROR   - Error in attempt to create Rank VLS region

**/
EFI_STATUS
HandleRankVlsAlways (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  )
{
  EFI_STATUS       Status;
  UINT8            NodeId;
  UINT8            ChOnNode;
  UINT8            Rank;
  UINT8            Bank;
  UINT8            Device;

  // Check whether CPU support Rank VLS or not. Only advanced RAS support Rank VLS
  if (IsRankVlsSupport() == FALSE) {
    return EFI_UNSUPPORTED;
  }

  Status = EFI_SUCCESS;

  NodeId    = SparingRegionPtr->NodeId;
  ChOnNode  = SparingRegionPtr->ChOnNode;
  Rank      = SparingRegionPtr->FailedRank;
  Bank      = SparingRegionPtr->FailedBank;
  Device    = SparingRegionPtr->FailedDevice;

  RAS_DEBUG ((LEVEL_BASIC_FLOW,
              "[ADDDC]: HandleNonRankVlsStrike() start...\n"));

  RAS_DEBUG ((LEVEL_BASIC_FLOW,
              "[ADDDC]: NodeId %d, ChOnNode %d, Rank %d, Bank %d, Device %d.\n",
              NodeId, ChOnNode, Rank, Bank, Device));
  Status = HandleRankVlsEvent (SparingRegionPtr);

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[ADDDC]: HandleNonRankVlsStrike() Status %r.\n", Status));

  return Status;
}

/**
  Handle new ADDDC event

  @param  NodeId          - Memory controller ID
  @param  IsEventHandled  - ptr to buffer to hold event handled status

  @retval Status          - EFI_SUCCESS if no failures
                          - non-zero error code for failures

  @retval IsEventHandled  - TRUE  -  event was handled
                          - FALSE - event not handled

**/
EFI_STATUS
EFIAPI
HandleNewAdddcEvent (
  IN  UINT8             NodeId,
  OUT BOOLEAN          *IsEventHandled
  )
{
  EFI_STATUS                    Status;
  UINT8                         ChOnNode;
  UINT8                         Rank;
  UINT8                         Bank;
  ADDDC_RANK_MODE               RankMode;
  SPARING_REGION_STRUCT         SparingRegion;
  MEM_TOPOLOGY                  *MemTopology;
  IMC_RAS_STRUCTURE             *ImcRas;

  Status = EFI_SUCCESS;
  *IsEventHandled = FALSE;

  MemTopology = GetMemTopology ();
  ImcRas      = GetImcRasData ();

  if (MemTopology->RasModesEnabled & (FULL_MIRROR_1LM | PARTIAL_MIRROR_1LM)) {

    if (!GetM2mDefeatures0ScrChkReadDis (NODE_TO_SKT (NodeId), NODE_TO_MC (NodeId)) ) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW,
                  "[ADDDC]: Sighting:5371865   ADDDC + MIRROR, mirror failover has not started yet, exit ADDDC \n"));
      return   EFI_NOT_READY;
    }

  }

  RAS_DEBUG ((LEVEL_BASIC_FLOW,
              "[ADDDC]: HandleNewAdddcEvent() Start...\n"));
  if (!IsAdddcPendingListEmpty (NodeId)) {
    //
    //The previous event is being handled
    //
    RAS_DEBUG ((LEVEL_BASIC_FLOW,
                "[ADDDC]: The pending list is not empty, can't handle new event, return...\n"));
    return EFI_NOT_READY;
  }

  for (ChOnNode = 0; ChOnNode < MAX_MC_CH; ChOnNode++) {
    if (!IsChannelPresent (NodeId, ChOnNode)) {
      continue;
    }
    if (IsSiliconWorkaroundEnabled ("S1707203950")) {
      if (IsAdddcDisabled (NODE_TO_SKT (NodeId), NODECHA_TO_SKTCHA (NodeId, ChOnNode))) {
        continue;
      }
    }

    RAS_DEBUG ((LEVEL_BASIC_FLOW,
                "[ADDDC]: Check Node %d, CH %d...\n", NodeId, ChOnNode));
    ZeroMem (&SparingRegion, sizeof (SparingRegion));
    Status = GetFailedDeviceInfo (NodeId, ChOnNode, &SparingRegion);
    if (EFI_ERROR (Status)) {
      //
      //No failed device was found
      //
      continue;
    }
// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#if AMI_RAS_ENHANCEMENTS_AND_HOOKS_SUPPORT
    {
        BOOLEAN   SkipEventHandlers = FALSE;
        UINT8     ErrorEvent = ERROR_EVENT_ADDDC;
		
        Status = GetChannelErrorInfo (NodeId, ChOnNode, FALSE, &mAddcFailedDimmInfo);
        if (!EFI_ERROR(Status)) {
		    // Oem Hook To decide the Error action based on the Error data
            OemReadyToTakeAction(ErrorEvent, &mAddcFailedDimmInfo, &SkipEventHandlers);
            if (SkipEventHandlers == TRUE) {
                DEBUG((DEBUG_INFO,"OEM request..Skipping ADDDC Sparing Event!\n"));
                return EFI_SUCCESS;
            }
        }
    }
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support
    


    ChOnNode  = SparingRegion.ChOnNode;
    Rank      = SparingRegion.FailedRank;
    Bank      = SparingRegion.FailedBank;

    //
    //RefreshChStatus(NodeId, ChOnNode);
    //
    DumpAdddcStatus (NodeId, ChOnNode);
    RankMode = GetRankMode (NodeId, ChOnNode, Rank);
    switch (RankMode) {
      case NonRankVlsMode:
        RAS_DEBUG ((LEVEL_BASIC_FLOW,
                    "[ADDDC]NonRankVlsMode: Call HandleNonRankVlsStrike()...\n"));

        if (ImcRas->RankVlsAlways) {
          RAS_DEBUG ((LEVEL_BASIC_FLOW,
                      "[ADDDC]NonRankVlsMode: Call HandleRankVlsAlways() \n"));
          Status = HandleRankVlsAlways (&SparingRegion);
        } else {
          if (IsSiliconWorkaroundEnabled ("S1706995555")) {
            RAS_DEBUG ((LEVEL_BASIC_FLOW,
              "[ADDDC]HandleNewAdddcEvent: Silicon WA active no ADDDC VLS is possible \n"));
          } else {
            Status = HandleNonRankVlsStrike (&SparingRegion);
          }
        }
        break;
      case RankVlsPrimaryMode: // The rank is in VLS primary mode
        //
        //Try next memory sparing feature
        //
        RAS_DEBUG ((LEVEL_BASIC_FLOW,
                    "[ADDDC]RankVlsPrimaryMode: Try next memory sparing feature...\n"));
        Status = StartAdddcPlusOneState (&SparingRegion);
        break;
      case RankVlsBuddyMode:   // The rank is in VLS buddy mode
        RAS_DEBUG ((LEVEL_BASIC_FLOW,
                    "[ADDDC]RankVlsBuddyMode: Call HandleRankVlsRevsEvent()...\n"));
        //
        // If PCLS possible attempt reverse and PCLS
        //

        //
        //Try reverse VLS and rebuddy
        //
        Status = HandleRankVlsRevsEvent (&SparingRegion);
        if (EFI_ERROR (Status)) {
          //
          //Try next memory sparing feature
          //
          Status = StartAdddcPlusOneState (&SparingRegion);
        }
        break;
      case SddcPlusOneMode:    // Can't do + 1 twice
        RAS_DEBUG ((LEVEL_BASIC_FLOW,
                    "[ADDDC]Rank is under S mode, return...\n"));

        Status = EFI_UNSUPPORTED;
        break;
      default:                 // Error
        Status = EFI_UNSUPPORTED;
        break;
    }
    if (!EFI_ERROR (Status)) {
      //
      //No error happens, the event was handled
      //
      RAS_DEBUG ((LEVEL_BASIC_FLOW,
                  "[ADDDC]: The event has been successfully handled!\n"));
      *IsEventHandled = TRUE;
      break;
    }
  }

// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#if AMI_RAS_ENHANCEMENTS_AND_HOOKS_SUPPORT
  OemReportStatusCode(mAddcFailedDimmInfo, ADDDC_START);
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support

  if (!EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW,
                "[ADDDC]: Call SchedulePendingList()\n"));

    SetErrorThresholdBeforeSpareCopyStart (NodeId, ChOnNode, MAX_RANK_ERROR_THRESHOLD);
    PatrolScrubInfoSaveOrRestore (NodeId, PATROL_SAVE | GV_C6FLAG);
    Status = SchedulePendingList (NodeId);
  }
  RAS_DEBUG ((LEVEL_BASIC_FLOW,
              "[ADDDC]: HandleNewAdddcEvent() return, Status = %r.\n", Status));
  return Status;
}

/**
  Handle InProgress ADDDC event

  @param NodeId               - Memory controller ID
  @param *IsEventHandled      - ptr to buffer to hold event handled status

  @retval Status              - EFI_SUCCESS if no failures

  @retval IsEventHandled      - TRUE  - event was handled
  @retval                     - FALSE - event not handled

**/
EFI_STATUS
EFIAPI
HandleInProgressAdddcEvent (
  IN  UINT8         NodeId,
  OUT BOOLEAN       *IsEventHandled
  )
{
  EFI_STATUS        Status;

  RAS_DEBUG ((LEVEL_BASIC_FLOW,
              "[ADDDC]: HandleInProgressAdddcEvent() Start...\n"));
  Status = SchedulePendingList (NodeId);
  if (!EFI_ERROR (Status)) {
    *IsEventHandled = TRUE;
  } else {
    *IsEventHandled = FALSE;
  }
  RAS_DEBUG ((LEVEL_BASIC_FLOW,
              "[ADDDC]: HandleInProgressAdddcEvent() Status %r...\n", Status));
// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#if AMI_RAS_ENHANCEMENTS_AND_HOOKS_SUPPORT
  OemReportStatusCode(mAddcFailedDimmInfo, ADDDC_COMPLETE);
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support
  return Status;
}

/**
  Check and handle ADDDC event

  @param  NodeId          - Memory controller ID
  @param  EventType       - NEW_EVENTS or EVENTS_IN_PROGRESS

  @retval IsEventHandled  - TRUE -  event was handled
  @retval                 - FALSE - event not handled

**/

BOOLEAN
EFIAPI
CheckAndHandleAdddcSparing (
  IN  UINT8         NodeId,
  IN  EVENT_TYPE    EventType
  )
{
  BOOLEAN           IsEventHandled;
  EFI_STATUS        Status;

  RAS_DEBUG ((LEVEL_BASIC_FLOW,
              "[ADDDC]: Main handler start...\n"));

  IsEventHandled = FALSE;
  Status = EFI_UNSUPPORTED;
  switch (EventType) {
    case NEW_EVENTS:
      Status = HandleNewAdddcEvent (NodeId, &IsEventHandled);
      break;

    case EVENTS_IN_PROGRESS:
      Status = HandleInProgressAdddcEvent (NodeId, &IsEventHandled);
      break;

    default:
      break;
  }

  RAS_DEBUG ((LEVEL_BASIC_FLOW,
              "[ADDDC]: Main handler return, Status %r.\n", Status));

  return IsEventHandled;
}

/**
  Initialize ADDDC sparing handler
**/
EFI_STATUS
EFIAPI
InitAdddcSparing (
  VOID
  )
{
  IMC_RAS_STRUCTURE       *ImcRas;
  MEM_TOPOLOGY            *MemTopology;

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[ADDDC]: InitAdddcSparing()... Start\n"));

  MemTopology = GetMemTopology ();
  if (MemTopology == NULL) {
    return EFI_UNSUPPORTED;
  }

  if (MemTopology->ExRasModesEnabled & ADDDC_EN) {
    //
    //Initialize pending list
    //
    InitAdddcPendingList ();

    //
    //Register handler into IMC RAS table
    //
    ImcRas = GetImcRasData ();
    if (ImcRas == NULL) {
      return EFI_UNSUPPORTED;
    }
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "[ADDDC]: Register CheckAndHandleAdddcSparing()...\n"));
    ImcRas->RasEventHndlrTable[ADDDC_INDEX] = CheckAndHandleAdddcSparing;
    InitAdddcData ();
  }

  return EFI_SUCCESS;
}

/**
  Initialize ADDDC Error Injection in all enabled channels.

  @param  [in]  Socket    Socket to be programmed.

  @retval  None.

**/
VOID
InitializeAdddcErrInj (
  UINT8 IN  Socket
  )
{
  UINT8  Mc;
  UINT8  SktCh;
  UINT8  Ch;
  MEM_TOPOLOGY  *MemTopology;

  MemTopology = GetMemTopology ();
  for (Mc = 0; Mc < MAX_IMC; Mc++) {
    if (MemTopology->Socket[Socket].imcEnabled[Mc]) {
      for (Ch = 0; Ch < MAX_MC_CH; Ch++) {
        SktCh = NODECHA_TO_SKTCHA (Mc, Ch);
        if (MemTopology->Socket[Socket].ChannelInfo[SktCh].Enabled) {
          SetAdddcErrorInj (Socket, SktCh);
        }
      } // Ch loop
    } // If IMC enabled
  } // IMC loop
}
