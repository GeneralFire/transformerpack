/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include <Library/MemoryCoreLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/RcMemAllocLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemRoutingLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemTurnaroundTimingLib.h>

#include <Memory/MemoryCheckpointCodes.h>

#define TURN_AROUND_REDUCTION   1 // 1 DCLK
#define GUARD_BAND              2 // 2 DCLK


/**

  This function run turnaround test to check if there is any test error. It only applies to
  turnaround within the same bank group and turnaround between different bank groups of the
  same rank.

  @param Socket              - Socket number
  @param TurnaroundType      - Turnaround timing type
  @param ChannelDone         - Record which channel has finished turnaround training.
  @param ErrorResult         - Error result of turnaround training per channel.
                               For turnaround within a rank, ErrorResult[Channel#] records
                               the error of trunaround within current rank. For turnaround between
                               different ranks, ErrorResult[Channel#]
                               records the error of trunaround from rank to rank.

  @retval MRC_STATUS_SUCCESS - Turnaround test succeds.
**/
MRC_STATUS
TurnAroundSgSrRunTest (
  IN       UINT8        Socket,
  IN       UINT8        TurnaroundType,
  IN       BOOLEAN      (*ChannelsDone) [MAX_CH],
  IN OUT   BOOLEAN       (*ErrorResult) [MAX_CH]
  )
{
  UINT8               Ch;
  UINT8               SubCh;
  UINT8               Dimm;
  UINT8               Rank;
  UINT32              ChBitmask;
  MRC_STATUS          Status;
  UINT8               ChToCheck;
  MRC_MST             MemSsType;
  UINT8               MaxChDdr;
  MRC_RT              MrcRankList;
  BOOLEAN             RankPresent;
  UINT8               LogicalRank;
  UINT32              BwSerr[MAX_CH][SUB_CH][3];
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  PSYSHOST            Host;
  MRC_TT              TestType;

  TestType = GetCurrentTestType (Socket);

  //
  // Only support turnaround of same bank group or turnaround of same rank.
  //
  if ((TestType != TurnAroundSgTest) && (TestType != TurnAroundSrTest)) {
    return MRC_STATUS_SUCCESS;
  }

  Host      = GetSysHostPointer ();
  MaxChDdr  = GetMaxChDdr ();
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  ChToCheck = 0;

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {


    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

      RankPresent = FALSE;
      LogicalRank = 0;

      ChBitmask = GetChBitmask (Host, Socket, Dimm, Rank);
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled(Socket, Ch) == FALSE) {
          continue;
        }

        if ((*ChannelsDone)[Ch] == TRUE) {
          //
          // Update ChBitmask. Remove channels which has reached to the lowest turnaround value.
          //
          ChBitmask = ChBitmask & (~(UINT32)(1 << Ch));
          continue;
        }


        RankList = GetRankNvList (Host, Socket, Ch, Dimm);

        if (IsRankValid (Socket, Ch, Dimm, Rank, TRUE) == FALSE) {
          continue;
        }

        //
        // Inidicate this rank number is present on at least one channel
        //
        RankPresent = TRUE;

        //
        // Select current rank as CPGC rank for test.
        //
        LogicalRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
        SelectCPGCRanks (Host, Socket, Ch, 1 << LogicalRank, 0, 0);

      } // ch loop

      //
      // Go to the next rank if this rank is not present on any channels
      //
      if (RankPresent == FALSE) {
        continue;
      }

      //
      // Setup test.
      //
      MrcRankList.Dimm = Dimm;
      MrcRankList.Rank = Rank;
      SetupTest (Host, Socket, MemSsType, ChBitmask, 1, &MrcRankList, DdrLevel, GetCurrentTestType (Socket), gsmCsnDelim, 0, FIRST_ITERATION_LOOP);

      //
      // Run test.
      //
      TestControl (Host, Socket, MemSsType, ChBitmask, NO_SUBCH, GetCurrentTestType (Socket), 1, START_TEST, NOT_STOP_TEST);

      //
      // Parse test result.
      //
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }

        //
        // Skip the channel which has been trained.
        //
        if ((*ChannelsDone)[Ch] == TRUE) {
          continue;
        }

        if (IsRankValid (Socket, Ch, Dimm, Rank, TRUE) == FALSE) {
          continue;
        }

        RankList = GetRankNvList (Host, Socket, Ch, Dimm);

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          BwSerr[Ch][SubCh][0] = 0;
          BwSerr[Ch][SubCh][1] = 0;
          BwSerr[Ch][SubCh][2] = 0;
        }

        ChToCheck = 1 << Ch;
        Status = CollectTestResultsDdr5 (Host, Socket, ChToCheck, BwSerr);


        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          if ((BwSerr[Ch][SubCh][0] != 0) || (BwSerr[Ch][SubCh][2] != 0)) {
            //
            // Turnaround timing is per-channel. If any rank on a channel fails, set the channel to fail.
            //
            (*ErrorResult) [Ch] = TRUE;
          }
        } //subCh loop
      } // Ch loop

    } // Rank loop
  } // DIMM loop

  return MRC_STATUS_SUCCESS;
}

/**

  This function runs turnaround test to check if there is any test error. It only applies to
  turnaround between different ranks of a dual-rank DIMM.

  Rank switching within dual-rank DIMM:
  Test1: DIMM0 Rank0 -> DIMM 0 Rank1; DIMM0 Rank1 -> DIMM 0 Rank0
  Test2: DIMM1 Rank0 -> DIMM 1 Rank1; DIMM1 Rank1 -> DIMM 1 Rank0

  @param Socket              - Socket number.
  @param TurnaroundType      - Trunaround timing type.
  @param ChannelDone         - Record which channel has finished turnaround training.
  @param ErrorResult         - Error result of turnaround training.
                               For turnaround within a rank, ErrorResult[Channel#] records
                               the error of trunaround within current rank. For turnaround between
                               different ranks, ErrorResult[Channel#] records the error
                               of trunaround from rank to rank.

  @retval MRC_STATUS_SUCCESS - Turnaround test succeds.
**/
MRC_STATUS
TurnAroundDrRunTest (
  IN       UINT8        Socket,
  IN       UINT8        TurnaroundType,
  IN       BOOLEAN      (*ChannelsDone) [MAX_CH],
  IN OUT   BOOLEAN       (*ErrorResult) [MAX_CH]
  )
{
  UINT8               Ch;
  UINT8               SubCh;
  UINT8               Dimm;
  UINT8               Rank;
  UINT32              ChBitmask;
  MRC_STATUS          Status;
  UINT8               ChToCheck;
  MRC_MST             MemSsType;
  UINT8               MaxChDdr;
  MRC_RT              MrcRankList[1];
  UINT8               RankFirst;
  UINT8               RankSecond;
  UINT32              BwSerr[MAX_CH][SUB_CH][3];
  PSYSHOST            Host;
  MRC_TT              TestType;
  BOOLEAN             DualRankDimmPresent;
  UINT8               Phy2LogicalRankMapping[CPGC_RANK_MAPPING_MAX_NUMBER];

  TestType = GetCurrentTestType(Socket);

  Host      = GetSysHostPointer ();
  MaxChDdr  = GetMaxChDdr ();
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  ChBitmask = 0;
  ChToCheck = 0;

  //
  // For channel there is no dual-rank DIMM present, mark them as "Done".
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    //
    // Stop rank-to-rank(within same DIMM) turnaound training if there is no dual-rank DIMM on the channel.
    //
    if ((IsDualRankOnDimm (Socket, Ch, DIMM0) == FALSE) && (IsDualRankOnDimm (Socket, Ch, DIMM1) == FALSE)) {
      (*ErrorResult) [Ch] = TRUE;
    }
  }

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    DualRankDimmPresent = FALSE;
    RankFirst = 0;
    RankSecond = 0;

    //
    // Returns a bitmask of channels in which the current dimm is dual-rank DIMM.
    //
    ChBitmask = GetChBitmaskDualRank (Host, Socket, Dimm);

    //
    // Rank is used to order CPGC logical ranks. Current rank will be sleceted as the first CPGC logical rank.
    //
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }

        if (((*ChannelsDone)[Ch] == TRUE) || ((*ErrorResult) [Ch] == TRUE)) {
          //
          // Update ChBitmask. Remove channels which has done or turnaround has failed.
          //
          ChBitmask = ChBitmask & (~(UINT32)(1 << Ch));
          continue;
        }

        if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
          continue;
        }

        if (IsRankValid (Socket, Ch, Dimm, Rank, TRUE) == FALSE) {
          continue;
        }

        if (IsDualRankOnDimm(Socket, Ch, Dimm)) {
          //
          // There is dual-rank DIMM on channel.
          //
          DualRankDimmPresent = TRUE;
        }

        //
        // Only for dual-rank DIMM. TurnAroundDrTest is trained in DIMM granularity.
        // All turnaround traffics are between ranks within the current DIMM.
        //
        RankFirst  = GetLogicalRank (Host, Socket, Ch, Dimm, Rank); // Current Rank
        if (Rank == RANK0) {
          RankSecond = GetLogicalRank (Host, Socket, Ch, Dimm, RANK1);
        } else {
          RankSecond = GetLogicalRank (Host, Socket, Ch, Dimm, RANK0);
        }

        if (TurnaroundType == t_RWDR) {
          Phy2LogicalRankMapping[0] = RankFirst;
          Phy2LogicalRankMapping[1] = RankSecond;
          MapCPGCRanks (Host, Socket, Ch, &Phy2LogicalRankMapping, 2, 0, 0);
        } else {
          Phy2LogicalRankMapping[0] = RankFirst;
          Phy2LogicalRankMapping[1] = RankSecond;
          Phy2LogicalRankMapping[2] = RankFirst;
          Phy2LogicalRankMapping[3] = RankSecond;
          Phy2LogicalRankMapping[4] = RankSecond; // This is the second rank, not the first rank.
          Phy2LogicalRankMapping[5] = RankFirst;
          Phy2LogicalRankMapping[6] = RankSecond;
          Phy2LogicalRankMapping[7] = RankFirst;

          MapCPGCRanks (Host, Socket, Ch, &Phy2LogicalRankMapping, 8, 0, 0);
        }
      } // ch loop

      //
      // Go to the next DIMM if there is no dual-rank DIMM on any channels
      //
      if (DualRankDimmPresent == FALSE) {
        continue;
      }

      //
      // Setup test.
      //
      MrcRankList[0].Dimm = Dimm;
      MrcRankList[0].Rank = Rank;
      SetupTest (Host, Socket, MemSsType, ChBitmask, 1, &MrcRankList[0], DdrLevel, GetCurrentTestType (Socket), gsmCsnDelim, 0, FIRST_ITERATION_LOOP);

      //
      // Run test.
      //
      TestControl (Host, Socket, MemSsType, ChBitmask, NO_SUBCH, GetCurrentTestType (Socket), 1, START_TEST, NOT_STOP_TEST);

      //
      // Parse test result.
      //
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }

        //
        // Skip the channel which has been trained.
        //
        if ((*ChannelsDone)[Ch] == TRUE) {
          continue;
        }

        if (IsDualRankOnDimm (Socket, Ch, Dimm) == FALSE) {
          continue;
        }

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          BwSerr[Ch][SubCh][0] = 0;
          BwSerr[Ch][SubCh][1] = 0;
          BwSerr[Ch][SubCh][2] = 0;
        }

        ChToCheck = 1 << Ch;
        Status = CollectTestResultsDdr5 (Host, Socket, ChToCheck, BwSerr);


        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          if ((BwSerr[Ch][SubCh][0] != 0) || (BwSerr[Ch][SubCh][2] != 0)) {
            //
            // Turnaround timing is per-channel. If any rank-rank on a channel fails, set the channel to fail.
            //
            (*ErrorResult) [Ch] = TRUE;
          }
        } //subCh loop
      } // Ch loop
    } // Rank
  } // DIMM loop

  return MRC_STATUS_SUCCESS;
}

/**

  This function runs turnaround test to check if there is any test error. It only applies to
  turnaround between ranks on different DIMMs, running on 2DPC population configuration.

  Rank switching combinations:
    DIMM0 Rank0 -> DIMM 1 Rank0;  DIMM 1 Rank0 -> DIMM0 Rank0
    DIMM0 Rank0 -> DIMM 1 Rank1;  DIMM 1 Rank1 -> DIMM0 Rank0
    DIMM0 Rank1 -> DIMM 1 Rank0;  DIMM 1 Rank0 -> DIMM0 Rank1
    DIMM0 Rank1 -> DIMM 1 Rank1;  DIMM 1 Rank1 -> DIMM0 Rank1

  @param Socket              - Socket number.
  @param TurnaroundType      - Turnaround timing type.
  @param ChannelDone         - Record which channel has finished turnaround training.
  @param ErrorResult         - Error result of turnaround training.
                               For turnaround within a rank, ErrorResult[Channel#] records
                               the error of trunaround within current rank. For turnaround between
                               different ranks.

  @retval MRC_STATUS_SUCCESS - Turnaround test succeds.
**/

MRC_STATUS
TurnAroundDdRunTest (
  IN       UINT8        Socket,
  IN       UINT8        TurnaroundType,
  IN       BOOLEAN      (*ChannelsDone) [MAX_CH],
  IN OUT   BOOLEAN       (*ErrorResult) [MAX_CH]
  )
{
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               SubCh;
  UINT32              ChBitmask;
  MRC_STATUS          Status;
  UINT8               ChToCheck;
  MRC_MST             MemSsType;
  UINT8               MaxChDdr;
  MRC_RT              MrcRankList[1];
  UINT32              BwSerr[MAX_CH][SUB_CH][3];
  PSYSHOST            Host;
  MRC_TT              TestType;
  UINT8               RankOnDimm0;
  UINT8               RankOnDimm1;
  UINT8               Phy2LogicalRankMapping[CPGC_RANK_MAPPING_MAX_NUMBER];
  UINT8               RankFirst;
  UINT8               RankSecond;

  TestType = GetCurrentTestType (Socket);

  Host      = GetSysHostPointer ();
  MaxChDdr  = GetMaxChDdr ();
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  ChToCheck = 0;

  for (RankOnDimm0 = 0; RankOnDimm0 < MAX_RANK_DIMM; RankOnDimm0 ++) {

    for (RankOnDimm1 = 0; RankOnDimm1 < MAX_RANK_DIMM; RankOnDimm1 ++) {

      //
      // Dimm is used to order CPGC logical ranks. The rank on current DIMM will be sleceted as the first CPGC logical rank.
      //
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

        ChBitmask = 0x00;
        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

          if (IsChannelEnabled (Socket, Ch) == FALSE) {
            continue;
          }


          if ((*ChannelsDone)[Ch] == TRUE) {
            continue;
          }

          if (IsRankValid (Socket, Ch, DIMM0, RankOnDimm0, TRUE) == FALSE) {
            continue;
          }

          if (IsRankValid (Socket, Ch, DIMM1, RankOnDimm1, TRUE) == FALSE) {
            continue;
          }

          if (DdrDimmsPerChannel (Socket, Ch) != MAX_DIMM) {
            //
            // Skip the channel wich has only one DIMM present.
            //
            (*ErrorResult)[Ch] = TRUE;
            continue;
          }

          if ((*ErrorResult)[Ch] == TRUE) {
            //
            // Ship the channel on which turnaround has failed.
            //
            continue;
          }

          ChBitmask |= 1 << Ch;

          if (Dimm == DIMM0) {
            RankFirst  = GetLogicalRank (Host, Socket, Ch, DIMM0, RankOnDimm0);
            RankSecond = GetLogicalRank (Host, Socket, Ch, DIMM1, RankOnDimm1);
          } else {
            RankFirst  = GetLogicalRank (Host, Socket, Ch, DIMM1, RankOnDimm1);
            RankSecond = GetLogicalRank (Host, Socket, Ch, DIMM0, RankOnDimm0);
          }

          if (TurnaroundType == t_RWDD) {
            Phy2LogicalRankMapping[0] = RankFirst;
            Phy2LogicalRankMapping[1] = RankSecond;
            MapCPGCRanks (Host, Socket, Ch, &Phy2LogicalRankMapping, 2, 0, 0);
          } else {
            Phy2LogicalRankMapping[0] = RankFirst;
            Phy2LogicalRankMapping[1] = RankSecond;
            Phy2LogicalRankMapping[2] = RankFirst;
            Phy2LogicalRankMapping[3] = RankSecond;
            Phy2LogicalRankMapping[4] = RankSecond; // This is the second rank, not the first rank.
            Phy2LogicalRankMapping[5] = RankFirst;
            Phy2LogicalRankMapping[6] = RankSecond;
            Phy2LogicalRankMapping[7] = RankFirst;
            MapCPGCRanks (Host, Socket, Ch, &Phy2LogicalRankMapping, 8, 0, 0);
          }
        } // ch loop

        //
        // Setup test.
        //
        MrcRankList[0].Dimm = DIMM0;
        MrcRankList[0].Rank = RankOnDimm0;
        SetupTest (Host, Socket, MemSsType, ChBitmask, 1, &MrcRankList[0], DdrLevel, GetCurrentTestType (Socket), gsmCsnDelim, 0, FIRST_ITERATION_LOOP);

        //
        // Run test.
        //
        TestControl (Host, Socket, MemSsType, ChBitmask, NO_SUBCH, GetCurrentTestType (Socket), 1, START_TEST, NOT_STOP_TEST);

        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

          if (IsChannelEnabled (Socket, Ch) == FALSE) {
            continue;
          }

          if ((*ChannelsDone)[Ch] == TRUE) {
            continue;
          }

          if (IsRankValid (Socket, Ch, DIMM0, RankOnDimm0, TRUE) == FALSE) {
            continue;
          }
          if (IsRankValid (Socket, Ch, DIMM1, RankOnDimm1, TRUE) == FALSE) {
            continue;
          }

          if (DdrDimmsPerChannel (Socket, Ch) != MAX_DIMM) {
            //
            // Skip the channel wich has only one DIMM present.
            //
            (*ErrorResult)[Ch] = TRUE;
            continue;
          }

          if ((*ErrorResult)[Ch] == TRUE) {
            //
            // Ship the channel on which turnaround has failed.
            //
            continue;
          }

          //
          // Parse test result.
          //
          for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
            BwSerr[Ch][SubCh][0] = 0;
            BwSerr[Ch][SubCh][1] = 0;
            BwSerr[Ch][SubCh][2] = 0;
          }

          ChToCheck = 1 << Ch;
          Status = CollectTestResultsDdr5 (Host, Socket, ChToCheck, BwSerr);

          for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
            if ((BwSerr[Ch][SubCh][0] != 0) || (BwSerr[Ch][SubCh][2] != 0)) {
              //
              // Turnaround timing is per-channel. If any dimm-dimm on a channel fails, set the channel to fail.
              //
              (*ErrorResult)[Ch] = TRUE;
            }
          }
        }
      } // Dimm
    } // RankOnDimm1
  } // RankOnDimm0
  return MRC_STATUS_SUCCESS;
}

/**

  This function runs turnaround test to check if there is any test error for
  turnaround between sub-ranks in a rank.

  @param Socket              - Socket number.
  @param TurnaroundType      - Turnaround timing type.
  @param ChannelDone         - Record which channel has finished turnaround training.
  @param ErrorResult         - Error result of turnaround training.
                               For turnaround within a rank, ErrorResult[Channel#] records
                               the error of trunaround within current rank. For turnaround between
                               different sub-ranks in a 3DS DIMM rank.

  @retval MRC_STATUS_SUCCESS - Turnaround test succeds.
**/

MRC_STATUS
TurnAroundDsRunTest (
  IN       UINT8        Socket,
  IN       UINT8        TurnaroundType,
  IN       BOOLEAN      (*ChannelsDone) [MAX_CH],
  IN OUT   BOOLEAN       (*ErrorResult) [MAX_CH]
  )
{
  UINT8               Ch;
  UINT8               SubCh;
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               RankInCh;
  UINT32              ChBitmask;
  MRC_STATUS          Status;
  UINT8               ChToCheck;
  MRC_MST             MemSsType;
  UINT8               MaxChDdr;
  MRC_RT              MrcRankList[1];
  UINT8               SubRankFirst;
  UINT8               SubRankSecond;
  UINT32              BwSerr[MAX_CH][SUB_CH][3];
  PSYSHOST            Host;
  MRC_TT              TestType;
  UINT8               Phy2LogicalRankMapping[CPGC_RANK_MAPPING_MAX_NUMBER];
  UINT8               Phy2LogicalSubRankMapping[2];
  struct              dimmNvram (*DimmNvList)[MAX_DIMM];

  TestType = GetCurrentTestType(Socket);

  Host      = GetSysHostPointer ();
  MaxChDdr  = GetMaxChDdr ();
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  ZeroMem((UINT8 *)(Phy2LogicalRankMapping), sizeof(Phy2LogicalRankMapping));

  ChBitmask = 0;
  ChToCheck = 0;

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

    if (Is3dsDimmPresentInChannel (Socket, Ch) == FALSE) {
      //
      // Stop sub-rank to sub-rank(within same DIMM) turnaround training if there is no 3DS DIMM on the channel.
      //
      (*ErrorResult) [Ch] = TRUE;
      continue;
    }

    if (((*ChannelsDone)[Ch] == TRUE) || ((*ErrorResult) [Ch] == TRUE)) {
      //
      // Update ChBitmask. Remove channels which has done or turnaround has failed.
      //
      continue;
    }

    ChBitmask = (UINT32)(1 << Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

      //
      // Test turnaround between sub-ranks of the same rank.
      //
      for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

        if (IsRankValid (Socket, Ch, Dimm, Rank, TRUE) == FALSE) {
          continue;
        }

        SubRankFirst = 0;
        SubRankSecond = 0;

        DimmNvList = GetDimmNvList (Host, Socket, Ch);

        RankInCh  = GetLogicalRank (Host, Socket, Ch, Dimm, Rank); // Current Rank

        for (SubRankFirst = 0; SubRankFirst < (*DimmNvList)[Dimm].dieCount; SubRankFirst++) {

          for (SubRankSecond = (SubRankFirst + 1); SubRankSecond < (*DimmNvList)[Dimm].dieCount; SubRankSecond++) {

            Phy2LogicalRankMapping[0] = RankInCh;
            Phy2LogicalSubRankMapping[0] = SubRankFirst;
            Phy2LogicalSubRankMapping[1] = SubRankSecond;
            MapCPGCRanks (Host, Socket, Ch, &Phy2LogicalRankMapping, 1, 0, &Phy2LogicalSubRankMapping);

            //
            // Setup test.
            //
            MrcRankList[0].Dimm = Dimm;
            MrcRankList[0].Rank = Rank;
            SetupTest (Host, Socket, MemSsType, ChBitmask, 1, &MrcRankList[0], DdrLevel, GetCurrentTestType (Socket), gsmCsnDelim, 0, FIRST_ITERATION_LOOP);

            //
            // Run test.
            //
            TestControl (Host, Socket, MemSsType, ChBitmask, NO_SUBCH, GetCurrentTestType (Socket), 1, START_TEST, NOT_STOP_TEST);

            //
            // Parse test result.
            //
            for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
              BwSerr[Ch][SubCh][0] = 0;
              BwSerr[Ch][SubCh][1] = 0;
              BwSerr[Ch][SubCh][2] = 0;
            }

            ChToCheck = 1 << Ch;
            Status = CollectTestResultsDdr5 (Host, Socket, ChToCheck, BwSerr);

            for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
              if ((BwSerr[Ch][SubCh][0] != 0) || (BwSerr[Ch][SubCh][2] != 0)) {
                //
                // Turnaround timing is per-channel. If any rank-rank on a channel fails, set the channel to fail.
                //
                (*ErrorResult) [Ch] = TRUE;
              }
            } //subCh loop
          } // SubRankSecond loop
        } // SubRankFirst loop
      } // Rank loop

      //
      // For turnaround between different sub-ranks from different ranks of the DIMM.
      //
      if (IsDualRankOnDimm (Socket, Ch, Dimm)) {

        SubRankFirst = 0;
        SubRankSecond = 0;

        DimmNvList = GetDimmNvList (Host, Socket, Ch);

        for (SubRankFirst = 0; SubRankFirst < (*DimmNvList)[Dimm].dieCount; SubRankFirst++) {

          for (SubRankSecond = 0; SubRankSecond < (*DimmNvList)[Dimm].dieCount; SubRankSecond++) {

            Phy2LogicalRankMapping[0] = GetLogicalRank (Host, Socket, Ch, Dimm, RANK0);
            Phy2LogicalRankMapping[0] = GetLogicalRank (Host, Socket, Ch, Dimm, RANK1);
            Phy2LogicalSubRankMapping[0] = SubRankFirst;
            Phy2LogicalSubRankMapping[1] = SubRankSecond;
            MapCPGCRanks (Host, Socket, Ch, &Phy2LogicalRankMapping, DUAL_RANK, 0, &Phy2LogicalSubRankMapping);

            //
            // Setup test.
            //
            MrcRankList[0].Dimm = Dimm;
            MrcRankList[0].Rank = Rank;
            SetupTest (Host, Socket, MemSsType, ChBitmask, 1, &MrcRankList[0], DdrLevel, GetCurrentTestType (Socket), gsmCsnDelim, 0, FIRST_ITERATION_LOOP);

            //
            // Run test.
            //
            TestControl (Host, Socket, MemSsType, ChBitmask, NO_SUBCH, GetCurrentTestType (Socket), 1, START_TEST, NOT_STOP_TEST);

            //
            // Parse test result.
            //
            for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
              BwSerr[Ch][SubCh][0] = 0;
              BwSerr[Ch][SubCh][1] = 0;
              BwSerr[Ch][SubCh][2] = 0;
            }

            ChToCheck = 1 << Ch;
            Status = CollectTestResultsDdr5 (Host, Socket, ChToCheck, BwSerr);

            for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
              if ((BwSerr[Ch][SubCh][0] != 0) || (BwSerr[Ch][SubCh][2] != 0)) {
                //
                // Turnaround timing is per-channel. If any rank-rank on a channel fails, set the channel to fail.
                //
                (*ErrorResult) [Ch] = TRUE;
              }
            } //subCh loop
          } // SubRankSecond loop
        } // SubRankFirst loop
      } // if

    } // Dimm
  } // Ch loop

  return MRC_STATUS_SUCCESS;

}

/**

  Optimizes turnaround timings for each channel.

  @param Socket          - Current CPU socket.
  @param TurnaroundType  - Turnaround timing type.
  @retval SUCCESS

**/
MRC_STATUS
TurnAroundOptimize (
  UINT8             Socket,
  UINT8             TurnaroundType
  )
{
  UINT8                             Ch;
  BOOLEAN                           ErrorResult[MAX_CH];
  UINT32                            GoodTurnaround[MAX_CH];
  UINT32                            OrginalTurnaround[MAX_CH];
  BOOLEAN                           ChannelsDone[MAX_CH];
  UINT32                            TurnaroundValue;
  BOOLEAN                           KeepGoing;
  UINT8                             MaxChDdr;
  PSYSHOST                          Host;
  MRC_STATUS                        Status;


  Host      = GetSysHostPointer ();
  MaxChDdr  = GetMaxChDdr ();
  ZeroMem ((UINT8 *) ChannelsDone, sizeof (ChannelsDone));
  ZeroMem ((UINT8 *) GoodTurnaround, sizeof (GoodTurnaround));
  ZeroMem ((UINT8 *) OrginalTurnaround, sizeof (OrginalTurnaround));

  //
  // Return if this socket has any DIMM populated.
  //
  if (IsSocketDimmPopulated (Host, Socket) == FALSE) {
    return SUCCESS;
  }

  //
  // Initialize ChannelDone, and save orignal safe turnaround timings.
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      //
      // Mark disabled channels to be Done.
      //
      ChannelsDone [Ch] = TRUE;
      continue;
    }

    //
    // For channel there is no dual-rank DIMM present.
    //
    if ((TurnaroundType == t_RRDR) || (TurnaroundType == t_RWDR) || (TurnaroundType == t_WRDR) || (TurnaroundType == t_WWDR)) {
      //
      // Skip rank-to-rank(within a DIMM) turnaound training if there is no dual-rank DIMM on the channel.
      //
      if ((IsDualRankOnDimm (Socket, Ch, DIMM0) == FALSE) && (IsDualRankOnDimm (Socket, Ch, DIMM1) == FALSE)) {
        ChannelsDone [Ch] = TRUE;
      }
    }

    //
    // For channel there are less than two DIMMs.
    //
    if ((TurnaroundType == t_RRDD) || (TurnaroundType == t_RWDD) || (TurnaroundType == t_WRDD) || (TurnaroundType == t_WWDD)) {
      //
      // Skip DIMM-to-DIMM turnaround training if the channel has less than two DIMMs.
      //
      if (DdrDimmsPerChannel(Socket, Ch) != MAX_DIMM) {
        ChannelsDone [Ch] = TRUE;
      }
    }

    GetSetSingleTurnaroundSetting (MemTechDdr, Socket, Ch, (GSM_READ_ONLY | GSM_READ_CSR), TurnaroundType, &TurnaroundValue);
    OrginalTurnaround [Ch] = TurnaroundValue;
    GoodTurnaround [Ch] = TurnaroundValue;

  } // ch loop

  //
  // Reduce turnaround on each channel until finding the shortest timing.
  //

  while (TRUE) {

    KeepGoing = FALSE;

    //
    // 1. Reduce channels' turnaround setting by 1 DCLK.
    //
    for (Ch = 0; Ch < MaxChDdr; Ch++) {

      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }

      if (ChannelsDone [Ch] == TRUE) {
        //
        // If a channel has reached to the shortest turnaround setting, skip this channel.
        //
        continue;
      }

      TurnaroundValue = GoodTurnaround [Ch] - TURN_AROUND_REDUCTION;  // Reduce TA by 1 DCK.
      if (TurnaroundValue <= GUARD_BAND) {
        ChannelsDone [Ch] = TRUE;
        continue;
      }
      GetSetSingleTurnaroundSetting (MemTechDdr, Socket, Ch, (GSM_READ_CSR | GSM_FORCE_WRITE), TurnaroundType, &TurnaroundValue);
    } // ch loop

    IO_Reset (Host, Socket);

    //
    // 2. Verify data Rd/Wr under current turnaround setting on all applicable channels
    //    in parallel.
    //
    ZeroMem ((UINT8 *) ErrorResult, sizeof (ErrorResult));
    if ((TurnaroundType == t_RWSG) || (TurnaroundType == t_RWSR)) {
      Status = TurnAroundSgSrRunTest (Socket, TurnaroundType, &ChannelsDone, &ErrorResult);
    } else if ((TurnaroundType == t_RRDR) || (TurnaroundType == t_RWDR) || (TurnaroundType == t_WRDR) || (TurnaroundType == t_WWDR)) {
      Status = TurnAroundDrRunTest (Socket, TurnaroundType, &ChannelsDone, &ErrorResult);
    } else if ((TurnaroundType == t_RRDD) || (TurnaroundType == t_RWDD) || (TurnaroundType == t_WRDD) || (TurnaroundType == t_WWDD)) {
      Status = TurnAroundDdRunTest (Socket, TurnaroundType, &ChannelsDone, &ErrorResult);
    } else if ((TurnaroundType == t_RRDS) || (TurnaroundType == t_RWDS) || (TurnaroundType == t_WRDS) || (TurnaroundType == t_WWDS)) {
      //
      // For 3DS DIMM.
      //
      Status = TurnAroundDsRunTest (Socket, TurnaroundType, &ChannelsDone, &ErrorResult);
    }

    //
    // 3. Check failures on each channel.
    //
    for (Ch = 0; Ch < MaxChDdr; Ch++) {

      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }

      if (ChannelsDone [Ch] == TRUE) {
        //
        // If a channel has reached to the shortest turnaround timing, skip this channel.
        //
        continue;
      }

      if (ErrorResult [Ch] == TRUE) {
         ChannelsDone [Ch] = TRUE;
      }
    } // ch loop

    //
    // 4. If all channels are done, stop. Otherwise, keep going.
    //

    for (Ch = 0; Ch < MaxChDdr; Ch++) {

      if (ChannelsDone [Ch] == FALSE) {
        //
        // Update to shorter turnaround timing which has passed test and keep going.
        //
        GoodTurnaround [Ch] = GoodTurnaround [Ch] - TURN_AROUND_REDUCTION;
        KeepGoing = TRUE;
      }
    }

    if (KeepGoing == FALSE) {
      break;
    }

  } // while loop

  //
  // The optimal turnaround values of all channels have been found. Adjust turnaround timing of each channel to optimal value.
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    TurnaroundValue = GoodTurnaround[Ch];
    GetSetSingleTurnaroundSetting (MemTechDdr, Socket, Ch, (GSM_READ_CSR | GSM_FORCE_WRITE), TurnaroundType, &TurnaroundValue);
  } // ch loop

  IO_Reset (Host, Socket);

  return MRC_STATUS_SUCCESS;
} // RoundTripOptimize


/*++

  Turnaround timing training.

  @param[in] Host      - Pointer to Host structure

  @retval EFI_SUCCESS

--*/
UINT32
EFIAPI
TurnaroundTrain (
  IN PSYSHOST  Host
  )
{
  UINT8           Ch;
  UINT8           Socket;
  UINT8           MaxChDdr;
  SYS_SETUP       *Setup;

  MaxChDdr  = GetMaxChDdr ();
  Setup     = GetSysSetupPointer ();
  Socket    = GetCurrentSocketId ();

  if (!IsMemFlowEnabled (TurnaroundTraining)) {
    return SUCCESS;
  }

  if ((Setup->mem.optionsExt & TURNAROUND_OPT_EN) == 0x00) {
    return SUCCESS;
  }

  //
  // Display turnaround timings before training.
  //
  RcDebugPrint (SDBG_MAX, "\nOriginal Turnaround");
  RcDebugPrintLine (SDBG_DEFAULT, 30, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);
  for (Ch = 0; Ch < MaxChDdr; Ch++) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    DisplayTurnaroundTimes (MemTechDdr, Socket, Ch);
  }
  RcDebugPrintLine (SDBG_DEFAULT, 30, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_TURNAROUND, Socket));

  //
  // Optimize turnaround within same bank group.
  //
  SetCurrentTestType (Socket, TurnAroundSgTest);
  TurnAroundOptimize (Socket, t_RWSG);

  //
  // Optimize turnaround between different bank groups of the same rank.
  //
  SetCurrentTestType (Socket, TurnAroundSrTest);
  TurnAroundOptimize (Socket, t_RWSR);

  //
  // Optimize turnaround between different ranks of the same DIMM.
  //

  SetCurrentTestType (Socket, TurnAroundDrDdWwRrWrTest);
  TurnAroundOptimize (Socket, t_RRDR);
  TurnAroundOptimize (Socket, t_WWDR);
  TurnAroundOptimize (Socket, t_WRDR);
  //
  // For 3DS DIMM
  //
  TurnAroundOptimize (Socket, t_RRDS);
  TurnAroundOptimize (Socket, t_WWDS);
  TurnAroundOptimize (Socket, t_WRDS);


  SetCurrentTestType (Socket, TurnAroundDrDdRwTest);
  TurnAroundOptimize (Socket, t_RWDR);
  //
  // For 3DS DIMM
  //
  TurnAroundOptimize (Socket, t_RWDS);

  //
  // Optimize turnaround between different DIMMs.
  //

  SetCurrentTestType (Socket, TurnAroundDrDdWwRrWrTest);
  TurnAroundOptimize (Socket, t_RRDD);
  TurnAroundOptimize (Socket, t_WWDD);
  TurnAroundOptimize (Socket, t_WRDD);

  SetCurrentTestType (Socket, TurnAroundDrDdRwTest);
  TurnAroundOptimize (Socket, t_RWDD);

  //
  // Display turnaround timings after training.
  //
  RcDebugPrint (SDBG_MAX, "\nNew Turnaround Timings.");
  RcDebugPrintLine (SDBG_DEFAULT, 30, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);
  for (Ch = 0; Ch < MaxChDdr; Ch++) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    DisplayTurnaroundTimes (MemTechDdr, Socket, Ch);
  }
  RcDebugPrintLine (SDBG_DEFAULT, 30, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);

  SetCurrentTestType (Socket, 0);
  return SUCCESS;
}
