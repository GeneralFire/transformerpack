/** @file

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
#include <Library/Ddr5CoreLib.h>
#include <Memory/MemCmdCtlClkCommon.h>
#include "Ddr5Core.h"
#include <Library/MemSweepLib.h>
#include <Library/SiliconWorkaroundLib.h>

#define PATTERN_LENGTH_64      64
#define RX_VREF_MARGIN_OFFSET  63 //RX VREF Magin Offset
#define DEFAULT_MARGIN_OFFSET  48 //Margin Offset for the groups except RX VREF.
#define TX_VREF_MARGIN_OFFSET  125 //TX VREF Margin Offset
#define RX_SAMPLER_MARGIN_OFFSET  31 //RX SAMPLER Margin Offset


/*++
  Collects the results of the previous test

  @param Host         - Pointer to sysHost
  @param socket       - Processor socket to check
  @param chToCheck    - Channel to check
  @param bwSerr       - Bit wise error status

  @retval status      - Non-zero if any failure was detected

--*/
UINT32
CollectTestResultsDdr5 (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN UINT32        ChToCheck,
  IN OUT UINT32    BwSerr[MAX_CH][SUB_CH][3]
  )
{
  UINT8                 Ch;
  UINT8                 SubCh = 0;
  UINT32                Status = SUCCESS;
#ifdef SPR_PO_FLAG
  CPGC_ERROR_STATUS_OLD CpgcErrorStatus[MAX_CH][SUB_CH];
#else
  CPGC_ERROR_STATUS     CpgcErrorStatus[MAX_CH][SUB_CH];
#endif
  struct  channelNvram  (*ChannelNvList) [MAX_CH];
  UINT8                 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  // Collect test results
#ifdef SPR_PO_FLAG
  CpgcAdvTrainingErrorStatusDdr51 (Host, Socket, ChToCheck, CpgcErrorStatus);
#else
  CpgcAdvTrainingErrorStatusDdr5 (Host, Socket, ChToCheck, CpgcErrorStatus);
#endif

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if ((*ChannelNvList) [Ch].enabled == 0) {
      continue;
    }

    // No need to check this channel if it's bit isn't set
    if ((ChToCheck & (1 << Ch))) {
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        BwSerr[Ch][SubCh][0] = CpgcErrorStatus[Ch][SubCh].cpgcErrDat0S | CpgcErrorStatus[Ch][SubCh].cpgcErrDat2S;
        BwSerr[Ch][SubCh][1] = CpgcErrorStatus[Ch][SubCh].cpgcErrDat1S | CpgcErrorStatus[Ch][SubCh].cpgcErrDat3S;
        if (Host->nvram.mem.eccEn) {
          BwSerr[Ch][SubCh][2] = (CpgcErrorStatus[Ch][SubCh].cpgcErrEccS & 0xFF) | ((CpgcErrorStatus[Ch][SubCh].cpgcErrEccS >> 8) & 0xFF);
        } else {
          BwSerr[Ch][SubCh][2] = 0;
        }

        Status |= BwSerr[Ch][SubCh][0];
        Status |= BwSerr[Ch][SubCh][1];
        Status |= BwSerr[Ch][SubCh][2];
      } //subch loop
    }
  } // ch loop

  return  Status;
} // CollectTestResultsDdr5

/*++
  Collects the results of the previous test

  @param[in] Host         - Pointer to sysHost
  @param[in] socket       - Processor socket to check
  @param[in] chToCheck    - Channel to check
  @param[in] bwSerr       - Bit wise error status

  @retval status      - Non-zero if any failure was detected

--*/
UINT32
CollectTestResultsEvenOddDdr5 (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN UINT32        ChToCheck,
  IN OUT UINT32    BwSerr[MAX_CH][SUB_CH][2][3]
  )
{
  UINT8                 Ch;
  UINT8                 SubCh = 0;
  UINT32                Status = SUCCESS;
#ifdef SPR_PO_FLAG
  CPGC_ERROR_STATUS_OLD CpgcErrorStatus[MAX_CH][SUB_CH];
#else
  CPGC_ERROR_STATUS     CpgcErrorStatus[MAX_CH][SUB_CH];
#endif
  struct  channelNvram  (*ChannelNvList) [MAX_CH];
  UINT8                 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  //
  // Collect test results
  //
#ifdef SPR_PO_FLAG
  CpgcAdvTrainingErrorStatusDdr51 (Host, Socket, ChToCheck, CpgcErrorStatus);
#else
  CpgcAdvTrainingErrorStatusDdr5 (Host, Socket, ChToCheck, CpgcErrorStatus);
#endif

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if ((*ChannelNvList) [Ch].enabled == 0) {
      continue;
    }
    //
    // No need to check this channel if it's bit isn't set
    //
    if ((ChToCheck & (1 << Ch))) {
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        //
        // EVEN UI
        //
        BwSerr[Ch][SubCh][0][0] = CpgcErrorStatus[Ch][SubCh].cpgcErrDat0S;
        //
        // ODD UI
        //
        BwSerr[Ch][SubCh][1][0] = CpgcErrorStatus[Ch][SubCh].cpgcErrDat2S;

        if (Host->nvram.mem.eccEn) {
          BwSerr[Ch][SubCh][0][2] = (CpgcErrorStatus[Ch][SubCh].cpgcErrEccS & 0xFF);
          BwSerr[Ch][SubCh][1][2] = ((CpgcErrorStatus[Ch][SubCh].cpgcErrEccS >> 8) & 0xFF);
        } else {
          BwSerr[Ch][SubCh][0][2] = 0;
          BwSerr[Ch][SubCh][1][2] = 0;
        }

        Status |= BwSerr[Ch][SubCh][0][0];
        Status |= BwSerr[Ch][SubCh][0][1];
        Status |= BwSerr[Ch][SubCh][0][2];
        Status |= BwSerr[Ch][SubCh][1][0];
        Status |= BwSerr[Ch][SubCh][1][1];
        Status |= BwSerr[Ch][SubCh][1][2];
      } //subch loop
    } //if
  } // ch loop

  return  Status;
} // CollectTestResultsEvenOddDdr5

/*++

  Write DQDQS Pre DFE 2D Centering

  @param[in] Host      - Pointer to Host structure

  @retval EFI_SUCCESS

--*/

UINT32
EFIAPI
WriteDqDqsPreDfe2DCentering (
  IN PSYSHOST  Host
  )
{

  UINT8 Socket = Host->var.mem.currentSocket;

#ifdef SPR_PO_FLAG
  Host->nvram.mem.socket[Socket].TrainingStepDone.TxVref = 1;
#endif // SPR_PO_FLAG

  if (!IsMemFlowEnabled (WriteDqDqsPreDfe2DCenteringTraining)) {
    return SUCCESS;
  }
  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_WRITE_DQDQS_PRE_DFE_2D_CENTERING, Socket));
  SetCurrentTestType (Socket, WritePreDfe2DCenteringTest);

  DoCompDdrioPerSetupOption (Socket, TRAINING_COMP_EVERY_TRAINING);

  //
  // First do the VREF centering.
  //
#ifndef SPR_PO_FLAG
#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    RcDebugPrint (SDBG_MEM_TRAIN,"Dump TxVref setting before VrefCentering.\n");
    DisplayResultsDdr5 (Host, Socket, TxVref);
    RcDebugPrint (SDBG_MEM_TRAIN,"\n");
  }
#endif // DEBUG_CODE_BLOCK

  VrefCentering (Host, Socket, MEM_CHIP_POLICY_VALUE (Host, TxVrefTrainingMode), TxVref);

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    RcDebugPrint (SDBG_MEM_TRAIN,"Dump TxVref setting after VrefCentering.\n");
    DisplayResultsDdr5 (Host, Socket, TxVref);
    RcDebugPrint (SDBG_MEM_TRAIN,"\n");
  }
#endif // DEBUG_CODE_BLOCK

#endif // SPR_PO_FLAG
  //
  // Do the timing centering
  //
  AdvancedCentering (Host, Socket, TxDqDelay);
  SetCurrentTestType (Socket, 0);
  return SUCCESS;
} // WriteDqDqsPreDfe2DCentering

/*++

  Write DQDQS Post DFE 2D Centering

  @param[in] Host      - Pointer to Host structure

  @retval EFI_SUCCESS

--*/

UINT32
EFIAPI
WriteDqDqsPostDfe2DCentering (
  IN PSYSHOST  Host
  )
{

  UINT8 Socket = Host->var.mem.currentSocket;

  if (!IsMemFlowEnabled (WriteDqDqsPostDfe2DCenteringTraining)) {
    return SUCCESS;
  }
  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_WRITE_DQDQS_POST_DFE_2D_CENTERING, Socket));
  SetCurrentTestType (Socket, WritePostDfe2DCenteringTest);

  DoCompDdrioPerSetupOption (Socket, TRAINING_COMP_EVERY_TRAINING);

  //
  // First do the VREF centering.
  //
  VrefCentering (Host, Socket, MEM_CHIP_POLICY_VALUE (Host, TxVrefTrainingMode), TxVref);
  //
  // Do the timing centering
  //
  AdvancedCentering (Host, Socket, TxDqDelay);
  SetCurrentTestType (Socket, 0);
  return SUCCESS;
} // WriteDqDqsPostDfe2DCentering


/**

  DDR5 DRAM read pattern initialization

  @param[in] Host           - Pointer to sysHost
  @param[in] Socket         - Socket to initialize
  @param[in] PatternMode    - Read test pattern mode
  @param[in] UiPattern      - UI pattern
  @param[in] PatternInvert  - Indicates DQs to invert in the pattern

  @retval N/A

**/
VOID
DramReadPatternInit (
  IN  PSYSHOST            Host,
  IN  UINT8               Socket,
  IN  READ_PATTERN_MODE   PatternMode,
  IN  UINT16              UiPattern,
  IN  UINT16              PatternInvert
  )
{
  UINT8                         Ch;
  UINT8                         MaxChDdr;
  UINT8                         SubCh;
  UINT8                         Dimm;
  UINT8                         Rank;
  UINT8                         NumRanks;
  DDR5_MODE_REGISTER_25_STRUCT  Mr25;
  DDR5_MODE_REGISTER_26_STRUCT  Mr26;
  DDR5_MODE_REGISTER_27_STRUCT  Mr27;
  DDR5_MODE_REGISTER_28_STRUCT  Mr28;
  DDR5_MODE_REGISTER_29_STRUCT  Mr29;
  UINT8                         Mr25Orig;
  UINT8                         Mr26Orig;
  UINT8                         Mr27Orig;
  UINT8                         Mr28Orig;
  UINT8                         Mr29Orig;

  MaxChDdr = GetMaxChDdr ();

  //
  // Initialize Read Training Mode Settings
  //
  Mr25.Data = 0;
  Mr25.Bits.read_training_pattern_format = PatternMode.Bits.PatternFormat;
  Mr25.Bits.lfsr0_pattern_option         = PatternMode.Bits.Lfsr0PatternOption;
  Mr25.Bits.lfsr1_pattern_option         = PatternMode.Bits.Lfsr1PatternOption;

  //
  // Initialize Read Pattern Data0 / LFSR0
  //
  Mr26.Data = 0;
  Mr26.Bits.read_pattern_lfsr_seed_ui_0 = ((UiPattern & BIT0) == 0) ? 0 : 1;
  Mr26.Bits.read_pattern_lfsr_seed_ui_1 = ((UiPattern & BIT1) == 0) ? 0 : 1;
  Mr26.Bits.read_pattern_lfsr_seed_ui_2 = ((UiPattern & BIT2) == 0) ? 0 : 1;
  Mr26.Bits.read_pattern_lfsr_seed_ui_3 = ((UiPattern & BIT3) == 0) ? 0 : 1;
  Mr26.Bits.read_pattern_lfsr_seed_ui_4 = ((UiPattern & BIT4) == 0) ? 0 : 1;
  Mr26.Bits.read_pattern_lfsr_seed_ui_5 = ((UiPattern & BIT5) == 0) ? 0 : 1;
  Mr26.Bits.read_pattern_lfsr_seed_ui_6 = ((UiPattern & BIT6) == 0) ? 0 : 1;
  Mr26.Bits.read_pattern_lfsr_seed_ui_7 = ((UiPattern & BIT7) == 0) ? 0 : 1;

  //
  // Initialize Read Pattern Data1 / LFSR1
  //
  Mr27.Data = 0;
  Mr27.Bits.read_pattern_lfsr_seed_ui_8  = ((UiPattern & BIT8)  == 0) ? 0 : 1;
  Mr27.Bits.read_pattern_lfsr_seed_ui_9  = ((UiPattern & BIT9)  == 0) ? 0 : 1;
  Mr27.Bits.read_pattern_lfsr_seed_ui_10 = ((UiPattern & BIT10) == 0) ? 0 : 1;
  Mr27.Bits.read_pattern_lfsr_seed_ui_11 = ((UiPattern & BIT11) == 0) ? 0 : 1;
  Mr27.Bits.read_pattern_lfsr_seed_ui_12 = ((UiPattern & BIT12) == 0) ? 0 : 1;
  Mr27.Bits.read_pattern_lfsr_seed_ui_13 = ((UiPattern & BIT13) == 0) ? 0 : 1;
  Mr27.Bits.read_pattern_lfsr_seed_ui_14 = ((UiPattern & BIT14) == 0) ? 0 : 1;
  Mr27.Bits.read_pattern_lfsr_seed_ui_15 = ((UiPattern & BIT15) == 0) ? 0 : 1;

  //
  // Initialize Read Pattern Invert DQL7:0 (DQ7:0)
  //
  Mr28.Bits.dq_invert_lower_dq_bits = PatternInvert & 0xFF;

  //
  // Initialize Read Pattern Invert DQU7:0 (DQ15:8)
  //
  Mr29.Bits.dq_invert_upper_dq_bits = (PatternInvert >> 8) & 0xFF;

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

      NumRanks = GetNumberOfRanksOnDimm (Socket, Ch, Dimm);
      for (Rank = 0; Rank < NumRanks; Rank++) {

        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

          ReadDramModeRegCachePerRank (Socket, Ch, SubCh, Dimm, Rank, DDR5_MODE_REGISTER_25_REG, &Mr25Orig);
          ReadDramModeRegCachePerRank (Socket, Ch, SubCh, Dimm, Rank, DDR5_MODE_REGISTER_26_REG, &Mr26Orig);
          ReadDramModeRegCachePerRank (Socket, Ch, SubCh, Dimm, Rank, DDR5_MODE_REGISTER_27_REG, &Mr27Orig);
          ReadDramModeRegCachePerRank (Socket, Ch, SubCh, Dimm, Rank, DDR5_MODE_REGISTER_28_REG, &Mr28Orig);
          ReadDramModeRegCachePerRank (Socket, Ch, SubCh, Dimm, Rank, DDR5_MODE_REGISTER_29_REG, &Mr29Orig);

          if (Mr25Orig != Mr25.Data) {
            DramModeRegWrite (Socket, Ch, 1 << SubCh, Dimm, Rank, DDR5_MODE_REGISTER_25_REG, Mr25.Data);
          }
          if (Mr26Orig != Mr26.Data) {
            DramModeRegWrite (Socket, Ch, 1 << SubCh, Dimm, Rank, DDR5_MODE_REGISTER_26_REG, Mr26.Data);
          }
          if (Mr27Orig != Mr27.Data) {
            DramModeRegWrite (Socket, Ch, 1 << SubCh, Dimm, Rank, DDR5_MODE_REGISTER_27_REG, Mr27.Data);
          }
          if (Mr28Orig != Mr28.Data) {
            DramModeRegWrite (Socket, Ch, 1 << SubCh, Dimm, Rank, DDR5_MODE_REGISTER_28_REG, Mr28.Data);
          }
          if ((Mr29Orig != Mr29.Data) && (IsX16Dimm (Socket, Ch, Dimm))) {
            DramModeRegWrite (Socket, Ch, 1 << SubCh, Dimm, Rank, DDR5_MODE_REGISTER_29_REG, Mr29.Data);
          }

        } // SubCh
      } //Rank
    } //Dimm
  } // Ch

}

/**

  DDR5 read DQ DQS Pre Dfe training Pattern setup.

  @param Host  - Pointer to sysHost

  @retval N/A

**/
VOID
ReadDqDqsPreDfeDramPatternInit (
  PSYSHOST Host
  )
{
  UINT8                   Socket;
  UINT8                   Ch;
  UINT8                   MaxChDdr;
  READ_PATTERN_MODE       PatternMode;
  UINT16                  UiPattern;
  UINT16                  PatternInvert;
  UINT8                   Dimm;
  UINT8                   Rank;
  UINT8                   NumRanks;
  MRC_RT                  MrcRankList;
  UINT32                  ChBitmask;
  MRC_TT                  OriginalTestType;
  MRC_TT                  TestType;
  MRC_MST                 MemSsType;

  MaxChDdr = GetMaxChDdr ();

  Socket = Host->var.mem.currentSocket;
  RcDebugPrint (SDBG_MEM_TRAIN, "ReadDqDqsPreDfeDramPatternInit start\n");

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  //
  // Initialize read test pattern in the DRAM
  //
  PatternMode.Data = 0;
  PatternMode.Bits.PatternFormat = READ_TRAINING_PATTERN_SERIAL;
  //
  // Leverage CPGC pattern definition. But UI pattern in DRAM is only 16-bits.
  //
  UiPattern = (UINT16)(RD_DQ_DQS_PREDFE_DDR5 & 0xFFFF);
  PatternInvert = 0;
  DramReadPatternInit (Host, Socket, PatternMode, UiPattern, PatternInvert);

  OriginalTestType = GetCurrentTestType (Socket);
  TestType = RdDqTest;

  SetCurrentTestType (Socket, TestType);


  //
  // Initialize channel mask and rank list
  //
  ChBitmask = 0;
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

      NumRanks = GetNumberOfRanksOnDimm (Socket, Ch, Dimm);
      for (Rank = 0; Rank < NumRanks; Rank++) {

        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        ChBitmask = 1 << Ch;

        MrcRankList.Dimm = Dimm;
        MrcRankList.Rank = Rank;
        SetupTest (Host, Socket, MemSsType, ChBitmask, 1, &MrcRankList, DdrLevel, TestType, gsmCsnDelim, 0, 0);
      } //Rank loop
    } //Dimm loop
  } // Ch loop

  SetCurrentTestType (Socket, OriginalTestType);

  return;
}

  /**

  DDR5 read DQ DQS Dfe training Pattern setup.

  @param Host  - Pointer to sysHost

  @retval Status

  **/
VOID
ReadDfeDramPatternInit (
  PSYSHOST Host
  )
{
  UINT8                         Socket;
  UINT8                         Ch;
  UINT8                         MaxChDdr;
  DDR5_MODE_REGISTER_25_STRUCT  Mr25;
  DDR5_MODE_REGISTER_26_STRUCT  Mr26;
  DDR5_MODE_REGISTER_27_STRUCT  Mr27;
  DDR5_MODE_REGISTER_28_STRUCT  Mr28;
  DDR5_MODE_REGISTER_29_STRUCT  Mr29;

  UINT8                         Dimm;
  UINT8                         Rank;
  MRC_RT                        MrcRankList;
  struct dimmNvram              (*DimmNvList) [MAX_DIMM];
  struct ddrRank                (*RankList) [MAX_RANK_DIMM];
  UINT32                        ChBitmask = 0;
  Mr28.Data                     = 0;
  Mr29.Data                     = 0;
  MRC_TT                        OriginalTestType;
  MRC_TT                        TestType;
  MRC_MST                       MemSsType;
  MaxChDdr                      = GetMaxChDdr ();

  Socket = Host->var.mem.currentSocket;
  RcDebugPrint (SDBG_MEM_TRAIN,
    "ReadDfeDramPatternInit start\n");

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  //
  // Initialize structures to write during the loop.
  //
  Mr25.Data = 0;
  Mr25.Bits.read_training_pattern_format = 1; // 1B: LFSR
  Mr26.Data = 0;
  Mr26.Bits.read_pattern_lfsr_seed_ui_0 = ((RD_DFE_PATTERN_DDR5 & BIT0) >> 0);
  Mr26.Bits.read_pattern_lfsr_seed_ui_1 = ((RD_DFE_PATTERN_DDR5 & BIT1) >> 1);
  Mr26.Bits.read_pattern_lfsr_seed_ui_2 = ((RD_DFE_PATTERN_DDR5 & BIT2) >> 2);
  Mr26.Bits.read_pattern_lfsr_seed_ui_3 = ((RD_DFE_PATTERN_DDR5 & BIT3) >> 3);
  Mr26.Bits.read_pattern_lfsr_seed_ui_4 = ((RD_DFE_PATTERN_DDR5 & BIT4) >> 4);
  Mr26.Bits.read_pattern_lfsr_seed_ui_5 = ((RD_DFE_PATTERN_DDR5 & BIT5) >> 5);
  Mr26.Bits.read_pattern_lfsr_seed_ui_6 = ((RD_DFE_PATTERN_DDR5 & BIT6) >> 6);
  Mr26.Bits.read_pattern_lfsr_seed_ui_7 = ((RD_DFE_PATTERN_DDR5 & BIT7) >> 7);
  Mr27.Data = 0;
  Mr27.Bits.read_pattern_lfsr_seed_ui_8 = ((RD_DFE_PATTERN_DDR5 & BIT8) >> 8);
  Mr27.Bits.read_pattern_lfsr_seed_ui_9 = ((RD_DFE_PATTERN_DDR5 & BIT9) >> 9);
  Mr27.Bits.read_pattern_lfsr_seed_ui_10 = ((RD_DFE_PATTERN_DDR5 & BIT10) >> 10);
  Mr27.Bits.read_pattern_lfsr_seed_ui_11 = ((RD_DFE_PATTERN_DDR5 & BIT11) >> 11);
  Mr27.Bits.read_pattern_lfsr_seed_ui_12 = ((RD_DFE_PATTERN_DDR5 & BIT12) >> 12);
  Mr27.Bits.read_pattern_lfsr_seed_ui_13 = ((RD_DFE_PATTERN_DDR5 & BIT13) >> 13);
  Mr27.Bits.read_pattern_lfsr_seed_ui_14 = ((RD_DFE_PATTERN_DDR5 & BIT14) >> 14);
  Mr27.Bits.read_pattern_lfsr_seed_ui_15 = ((RD_DFE_PATTERN_DDR5 & BIT15) >> 15);



  OriginalTestType = GetCurrentTestType (Socket);
  TestType = RdDqTest;

  SetCurrentTestType (Socket, TestType);
  //
  // Initialize to no ranks present
  //
  ChBitmask = 0;
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    //
    // Loop for each dimm and each rank
    //
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList) [Dimm].dimmPresent == 0) {
        continue;
      }

      RankList = GetRankNvList (Host, Socket, Ch, Dimm);
      for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
        //
        // Skip if no rank
        //
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        ChBitmask = 1 << Ch;
        //
        // Set the Read Training Pattern Format as serial.
        //
        DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_25_REG, Mr25.Data);
        DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_26_REG, Mr26.Data);
        DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_27_REG, Mr27.Data);
        DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_28_REG, Mr28.Data);
        DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_29_REG, Mr29.Data);

        MrcRankList.Dimm = Dimm;
        MrcRankList.Rank = Rank;
        SetupTest (Host, Socket, MemSsType, ChBitmask, 1, &MrcRankList, DdrLevel, TestType, gsmCsnDelim, 0, 0);
      } //Rank loop
    }//Dimm loop
  }  // Ch loop

  SetCurrentTestType (Socket, OriginalTestType);

  return;
}

/*++

  Read DQDQS Pre DFE 2D Centering

  @param[in] Host      - Pointer to Host structure

  @retval EFI_SUCCESS

**/
UINT32
EFIAPI
ReadDqDqsPreDfe2DCentering (
  IN PSYSHOST  Host
  )
{
  UINT8  MaxChDdr;
  UINT8 Ch = 0;
  MaxChDdr = GetMaxChDdr ();
  UINT8 Socket = Host->var.mem.currentSocket;

  if (!IsMemFlowEnabled (ReadDqDqsPreDfe2DCenteringTraining)) {
    return SUCCESS;
  }
  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_READ_DQDQS_PRE_DFE_2D_CENTERING, Socket));

  DoCompDdrioPerSetupOption (Socket, TRAINING_COMP_EVERY_TRAINING);

  //
  // Pattern Init.
  //
  ReadDqDqsPreDfeDramPatternInit (Host);
  SetCurrentTestType (Socket, ReadPreDfe2DCenteringTest);
  //
  // Vref Centering
  //
  VrefCentering (Host, Socket, MEM_CHIP_POLICY_VALUE (Host, RxVrefTrainingMode), RxVref);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    //
    // Set mpr_train_ddr_on in host side to filter ACT/PRE.
    //
    SetMprModeFilterActPre (Host, Socket, Ch, FILTER_ACT_PRE_ENABLE);
  }  // Ch loop

  SetCurrentTestType (Socket, ReadPreDfe2DCenteringTest);
  AdvancedCentering (Host, Host->var.mem.currentSocket, RxDqsNDelay);
  AdvancedCentering (Host, Host->var.mem.currentSocket, RxDqsPDelay);

  SetCurrentTestType (Socket, 0);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    //
    // Set mpr_train_ddr_on in host side to filter ACT/PRE.
    //
    SetMprModeFilterActPre (Host, Socket, Ch, FILTER_ACT_PRE_DISABLE);
  }  // Ch loop

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    DisplayResultsDdr5 (Host, Socket, RxDqsPDelay);
    DisplayResultsDdr5 (Host, Socket, RxDqsNDelay);
    DisplayResultsDdr5 (Host, Socket, RxDqsBitDelay);
    DisplayResultsDdr5 (Host, Socket, RxDqsNBitDelay);
    DisplayResultsDdr5 (Host, Socket, RxDqsPBitDelay);
    DisplayResultsDdr5 (Host, Socket, RxVref);
    RcDebugPrint (SDBG_MEM_TRAIN,"\n");
  }
#endif // DEBUG_CODE_BLOCK

  return SUCCESS;
} // ReadDqDqsPreDfe2DCentering

/*++

  Read DQDQS Post DFE 2D Centering

  @param[in] Host      - Pointer to Host structure

  @retval EFI_SUCCESS

--*/
UINT32
EFIAPI
ReadDqDqsPostDfe2DCentering (
  IN PSYSHOST  Host
  )
{

  UINT8        MaxChDdr;
  MRC_GT       GroupInput;
  UINT8        Ch = 0;
  MaxChDdr = GetMaxChDdr ();
  UINT8 Socket = Host->var.mem.currentSocket;

  if (!IsMemFlowEnabled (ReadDqDqsPostDfe2DCenteringTraining)) {
    return SUCCESS;
  }
  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_READ_DQDQS_POST_DFE_2D_CENTERING, Socket));
  //
  // Pattern Init.
  //
  ReadDfeDramPatternInit (Host);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    //
    // Set mpr_train_ddr_on in host side to filter ACT/PRE.
    //
    SetMprModeFilterActPre (Host, Socket, Ch, FILTER_ACT_PRE_ENABLE);
  }  // Ch loop


  SetCurrentTestType (Socket, ReadDfeTest);
  DoCompDdrioPerSetupOption (Socket, TRAINING_COMP_EVERY_TRAINING);

  //
  // Vref Centering
  //
  if (IsSiliconWorkaroundEnabled ("S22010823763")) {
    GroupInput = RxVref;
  } else {
    GroupInput = RxSamplerEvenOdd;
  }
  VrefCentering (Host, Socket, MEM_CHIP_POLICY_VALUE (Host, RxVrefTrainingMode), GroupInput);

  AdvancedCentering (Host, Socket, RxDqsNDelay);
  AdvancedCentering (Host, Socket, RxDqsPDelay);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    //
    // Clear mpr_train_ddr_on in host side to enable ACT/PRE.
    //
    SetMprModeFilterActPre (Host, Socket, Ch, FILTER_ACT_PRE_DISABLE);
  }  // Ch loop

  SetCurrentTestType (Socket, 0);
  return SUCCESS;
} // ReadDqDqsPostDfe2DCentering


/**

  Tx DQ and DQS Duty cycle correction

  @param[in] Host       - Pointer to sysHost
  @param[in] socket     - Socket number

  @retval N/A

**/
VOID
EFIAPI
TxDqDqsDutyCycleCorrection (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  struct bitMargin    *ResultsBit;
  UINT8               MaxChDdr;
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               RankIndex;
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  INT16               EwEven;
  INT16               EwOdd;
  INT16               MinEwEven;
  INT16               IdealCenterEven;
  INT16               IdealCenterOdd;
  INT16               StrobeDutyCycleCorrection;
  INT16               TxDqPieOffsetValues[MAX_BITS_IN_BYTE];
  INT16               TxDqDlyTemp[MAX_BITS_IN_BYTE];
  INT16               TxDqsPieOffsetValue;
  INT16               TxDqsDelayValue;
  INT16               TxDqDelayValues[MAX_BITS_IN_BYTE];
  UINT8               Bit;
  UINT8               SubCh;
  UINT8               SubChMSVx4;
  UINT8               SubChMSVx8;
  UINT8               EncodedStrobe;
  UINT8               BitIndex;
  UINT8               Strobe;
  UINT8               MaxStrobe;
  UINT8               StrobeWidth;

  //
  // If the current test type is not supported, return immediately.
  //
  if ((GetCurrentTestType (Socket) != WritePreDfe2DCenteringTest) &&
      (GetCurrentTestType (Socket) != WritePostDfe2DCenteringTest)) {
    return;
  }

  ResultsBit = RcAllocatePool (MaxChunkSides * sizeof (*ResultsBit));
  if (ResultsBit == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return;
  }

  ZeroMem ((UINT8 *) ResultsBit, MaxChunkSides * sizeof (*ResultsBit));
  ZeroMem ((UINT8 *) TxDqPieOffsetValues, sizeof (TxDqPieOffsetValues));
  ZeroMem ((UINT8 *) TxDqDlyTemp, sizeof (TxDqDlyTemp));
  ZeroMem ((UINT8 *) TxDqDelayValues, sizeof (TxDqDelayValues));

  MaxChDdr = GetMaxChDdr ();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  SubChMSVx8 = GetSubChMaxStrobeValid (Host) / 2;

  RcDebugPrintWithDevice (
    SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Tx DQ DQS Duty cycle correction starts\n"
    );

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    RcDebugPrint (SDBG_MEM_TRAIN,"Dump register setting before duty cycle correction.\n");
    DisplayResultsDdr5 (Host, Socket, TxDqDelay);
    DisplayResultsDdr5 (Host, Socket, TxDqBitDelay);
    DisplayResultsDdr5 (Host, Socket, TxDqPieOffset);
    DisplayResultsDdr5 (Host, Socket, TxDqsDelay);
    DisplayResultsDdr5 (Host, Socket, TxDqsPieOffset);
    RcDebugPrint (SDBG_MEM_TRAIN,"\n");
  }
#endif // DEBUG_CODE_BLOCK
  //
  // Do the timing recentering.
  //
  GetAdvDataMargins (
    Socket, DdrLevel, TxDqDelay, DefaultResults,
    NULL, NULL, NULL,
    NULL, ResultsBit, NULL
    );

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      if (IsX4Dimm (Socket, Ch, Dimm)) {
        MaxStrobe = SubChMSVx4;
        StrobeWidth = BITS_PER_NIBBLE;
      } else {
        MaxStrobe = SubChMSVx8;
        StrobeWidth = MAX_BITS_IN_BYTE;
      }

      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        RankIndex = GetRankIndex (Socket, Ch, Dimm, Rank);

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
            //
            // Skip if this is an ECC nibble or strobe when ECC is disabled
            //
            if (IsX4Dimm (Socket, Ch, Dimm)) {
              if (IsNibbleNotValidDdr5 (Host, Strobe)) {
                continue;
              }
            } else {
              if (IsStrobeNotValidDdr5 (Host, Strobe)) {
                continue;
              }
            }

            //
            // Get the "encoded"/"logical" strobe used by the GetSet function
            //
            if (IsX4Dimm (Socket, Ch, Dimm)) {
              EncodedStrobe = GetStrobeFromNibble (Host, Strobe);
            } else {
              EncodedStrobe = Strobe;
            }

            MinEwEven = MAX_INT16;  // Trigger the first bit to update
            StrobeDutyCycleCorrection = 0;
            TxDqsPieOffsetValue = 0;
            for (Bit = 0; Bit < StrobeWidth; Bit++) {
              BitIndex = (SubCh * MaxStrobe + Strobe) * StrobeWidth + Bit;
              //
              // 1. For each DQ in the nibble, for even UI and odd UI,
              //    calculate Eye Widthand Ideal Center.
              //
              EwEven = (ResultsBit[EvenChunkSide].bits[Ch][RankIndex][BitIndex].p - ResultsBit[EvenChunkSide].bits[Ch][RankIndex][BitIndex].n);
              EwOdd = (ResultsBit[OddChunkSide].bits[Ch][RankIndex][BitIndex].p - ResultsBit[OddChunkSide].bits[Ch][RankIndex][BitIndex].n);

              IdealCenterEven = (ResultsBit[EvenChunkSide].bits[Ch][RankIndex][BitIndex].p + ResultsBit[EvenChunkSide].bits[Ch][RankIndex][BitIndex].n) / 2;
              IdealCenterOdd = (ResultsBit[OddChunkSide].bits[Ch][RankIndex][BitIndex].p + ResultsBit[OddChunkSide].bits[Ch][RankIndex][BitIndex].n) / 2;

              //
              // 2. Calculate the TX DQ Duty Cycle Correction for each of the
              //    DQ bits.
              //
              TxDqPieOffsetValues[Bit] = (EwEven - EwOdd) / 2;

              //
              // 3. Calculate the new value of txdq_dly# that results in
              //    centering the ODD UI for each of the DQ bits.
              //
              TxDqDlyTemp[Bit] = IdealCenterOdd - ((EwEven - EwOdd) / 4);

              //
              // 4. For the DQ with the minimum EW Even: Calculate the Duty
              //    Cycle Correction for the Strobe, (use a single DQ results
              //    for the calculation).
              //
              if (EwEven < MinEwEven) {
                MinEwEven = EwEven;
                StrobeDutyCycleCorrection = IdealCenterEven - IdealCenterOdd;
                TxDqsPieOffsetValue = -1 * StrobeDutyCycleCorrection;
              }
              RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, SubCh, Rank, Strobe, Bit,
                "TxDqPieOffsetValues:%2d TxDqDlyTemp:%3d StrobeDutyCycleCorrection:%2d TxDqsPieOffsetValue:%2d   EW:%2d  OW:%2d  EC:%3d OC:%3d EE[%3d %3d]  OE[%3d %3d]\n",
                TxDqPieOffsetValues[Bit],
                TxDqDlyTemp[Bit],
                StrobeDutyCycleCorrection,
                TxDqsPieOffsetValue,
                (ResultsBit[EvenChunkSide].bits[Ch][RankIndex][BitIndex].p - ResultsBit[EvenChunkSide].bits[Ch][RankIndex][BitIndex].n),
                (ResultsBit[OddChunkSide].bits[Ch][RankIndex][BitIndex].p - ResultsBit[OddChunkSide].bits[Ch][RankIndex][BitIndex].n),
                IdealCenterEven,
                IdealCenterOdd,
                ResultsBit[EvenChunkSide].bits[Ch][RankIndex][BitIndex].p,
                ResultsBit[EvenChunkSide].bits[Ch][RankIndex][BitIndex].n,
                ResultsBit[OddChunkSide].bits[Ch][RankIndex][BitIndex].p,
                ResultsBit[OddChunkSide].bits[Ch][RankIndex][BitIndex].n
                );
            } // Bit loop

            //
            // Write values calculated in step 2 within the loop.
            //
            GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedStrobe, ALL_BITS, DdrLevel, TxDqPieOffset, GSM_WRITE_OFFSET, TxDqPieOffsetValues);
            if (!IsX4Dimm (Socket, Ch, Dimm)) {
              GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedStrobe + SubChMSVx8, ALL_BITS, DdrLevel, TxDqPieOffset, GSM_WRITE_OFFSET, &TxDqPieOffsetValues[MAX_BITS_IN_BYTE / 2]);
            }

            //
            // Write value calculated in step 4 within the loop.
            //
            GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedStrobe, 0, DdrLevel, TxDqsPieOffset, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET, &TxDqsPieOffsetValue);
            if (!IsX4Dimm (Socket, Ch, Dimm)) {
               GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedStrobe + SubChMSVx8, 0, DdrLevel, TxDqsPieOffset, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET, &TxDqsPieOffsetValue);
            }

            //
            // 5. Calculate the final txdqs_dly and txdq_dly# to shift Strobes
            //    and DQ bits so that the rising edge of the Strobe retains its
            //    original positioned as trained in Write Leveling
            //

            TxDqsDelayValue = StrobeDutyCycleCorrection;
            GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedStrobe, 0, DdrLevel, TxDqsDelay, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET, &TxDqsDelayValue);
            if (!IsX4Dimm (Socket, Ch, Dimm)) {
              GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedStrobe + SubChMSVx8, 0, DdrLevel, TxDqsDelay, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET, &TxDqsDelayValue);
            }

            for (Bit = 0; Bit < StrobeWidth; Bit++) {
              TxDqDelayValues[Bit] = TxDqDlyTemp[Bit] + StrobeDutyCycleCorrection;
            }
            GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedStrobe, ALL_BITS, DdrLevel, TxDqBitDelay, GSM_UPDATE_CACHE, TxDqDelayValues);
            if (!IsX4Dimm (Socket, Ch, Dimm)) {
              GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedStrobe + SubChMSVx8, ALL_BITS, DdrLevel, TxDqBitDelay, GSM_UPDATE_CACHE, &TxDqDelayValues[MAX_BITS_IN_BYTE / 2]);
            }
          } // Strobe loop
        } // SubCh loop
      } // Rank loop
    } // Dimm loop
  } // Ch loop

  //
  // Add the results display here once the new APL is finalized.
  //

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    DisplayResultsDdr5 (Host, Socket, TxDqDelay);
    DisplayResultsDdr5 (Host, Socket, TxDqBitDelay);
    DisplayResultsDdr5 (Host, Socket, TxDqPieOffset);
    DisplayResultsDdr5 (Host, Socket, TxDqsDelay);
    DisplayResultsDdr5 (Host, Socket, TxDqsPieOffset);
    RcDebugPrint (SDBG_MEM_TRAIN,"\n");
  }
#endif // DEBUG_CODE_BLOCK

  RcFreePool (ResultsBit);

}

/**
  Program the same training parameter setting for all subchannels.
  This is used for parameters that have one setting per-subchannel.

  @param[in]      Param         - Pointer a specific parameter structures
  @param[in]      SettingIndex  - Index into the parameter's setting array

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
EFIAPI
SetParamPerSubCh (
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             SettingIndex
  )
{
  PSYSHOST            Host;
  UINT8               Ch;
  UINT8               SubCh;
  MPT_PT              ParamType;
  MRC_GT              ParamGsm;
  INT16               Setting;
  UINT8               Socket;
  UINT8               MaxChDdr;


  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;
  MaxChDdr = GetMaxChDdr ();

  Setting = Param->SettingList[SettingIndex];
  ParamGsm = Param->ParamGsm;
  ParamType = Param->ParamType;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      GetSetDataGroup (Host, Socket, Ch, SubCh, 0, 0, 0, ALL_BITS, DdrLevel, ParamGsm,
        GSM_UPDATE_CACHE | GSM_FORCE_WRITE, &Setting);
    }
  }
  return MRC_STATUS_SUCCESS;
}

/**
  Program the same training parameter setting for all strobes/bytes in all channels
  This is used for parameters that have one setting per-strobe for each channel.

  @param[in]      Param         - Pointer a specific parameter structures
  @param[in]      SettingIndex  - Index into the parameter's setting array

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetParamPerChAllStrobes (
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             SettingIndex
  )
{
  PSYSHOST            Host;
  UINT8               Socket;
  UINT8               Ch;
  MPT_PT              ParamType;
  MRC_GT              ParamGsm;
  INT16               Setting;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT8               MaxChDdr;


  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  Setting = Param->SettingList[SettingIndex];
  ParamGsm = Param->ParamGsm;
  ParamType = Param->ParamType;

  // Fill in margin entries for strobes for per Ch per Strobe
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    //
    // Set all strobes to the same value
    //
    GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, 0, 0, ALL_STROBES, ALL_BITS, DdrLevel, ParamGsm,
      GSM_UPDATE_CACHE | GSM_FORCE_WRITE, &Setting);
  } // Ch loop

  return MRC_STATUS_SUCCESS;
}


/**
  Sets all Tco DqDqs values of an output structure of just setting values to an input value

  @param[in]  Value            - Value to set
  @param[out] Settings         - Pointer the current Setting Values in the structure

  @retval None
**/
VOID
SetAllTcoDqDqsSettingValues (
  IN      INT16             Value,
     OUT  INT16             (*Settings)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 4]
  )

{
  UINT8               Socket;
  UINT8               Ch;
  UINT8               SubCh;
  UINT8               MaxChDdr;
  UINT8               Strobe;

  Socket = GetCurrentSocketId ();
  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      for (Strobe = 0; Strobe < (MAX_STROBE_DDR5 / 4); Strobe++) {
        (*Settings)[Ch][SubCh][Strobe] = Value;
      }//Strobe
    } //SubCh
  } //Ch
}

/**
  Sets all Dca Tco values of an output structure of just setting values to an input value

  @param[in]  Value            - Value to set
  @param[out] Settings         - Pointer the current Setting Values in the structure

  @retval None
**/
VOID
SetAllTcoSettingValues (
  IN      INT16             Value,
     OUT  INT16             (*Settings)[MAX_CH][SUB_CH]
)
{
  UINT8               Socket;
  UINT8               Ch;
  UINT8               SubCh;
  UINT8               MaxChDdr;

  Socket = GetCurrentSocketId ();
  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      (*Settings)[Ch][SubCh] = Value;
    } //SubCh
  } //Ch
}

/*
  Update the TcoResults structure with input margin data if the input margin data represents smaller non-zero margins

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      EyewidthPerSubCh  Array of per channel/subchannel results to check
  @param[in, out] TcoResults        Array that contains minimum per sub-channel minimum margin widths

  @retval n/a

*/
VOID
UpdateMinimumEyewidthInStructure (
  IN      COMMON_1D_PARAMETERS  *CommonParameters,
  IN      INT16                 (*EyewidthPerSubCh)[MAX_CH][SUB_CH],
  IN OUT  INT16                 (*TcoResults)[MAX_CH][SUB_CH]
)
{
  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT8   MaxChDdr;

  MaxChDdr = CommonParameters->MaxChDdr;

  for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {

    if (!CommonParameters->ChannelEnabledLocal[ChannelIndex]) {
      continue;
    }

    for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {

      if ((*TcoResults)[ChannelIndex][SubChannelIndex] == 0) {

        // If the existing TcoResults structure is 0, just use the input structure value
        (*TcoResults)[ChannelIndex][SubChannelIndex] = (*EyewidthPerSubCh)[ChannelIndex][SubChannelIndex];

      } else {
        // Compare input and use the minimum
        if ((*TcoResults)[ChannelIndex][SubChannelIndex] > (*EyewidthPerSubCh)[ChannelIndex][SubChannelIndex]) {
          (*TcoResults)[ChannelIndex][SubChannelIndex] = (*EyewidthPerSubCh)[ChannelIndex][SubChannelIndex];
        }
      }

    } // SubChannelIndex loop ...

  } // ChannelIndex loop

} //UpdateMinimumEyewidthInStructure

/**
  Compare a given margin results structure with the current best results and update if better

  @param[in]      Param            - Pointer a specific parameter structures
  @param[in]      SettingValue     - Pointer with values of the settings used for the input results structure
  @param[in]      Results          - Pointer that contains the margin results for the SettingValue
  @param[in, out] BestTcoResults   - Pointer to a Tco training results data structure

  @retval None
**/
VOID
UpdateBestTcoPerStrobeResults (
  IN     POWER_TRAIN_PARAM           *Param,
  IN     INT16                       (*SettingValue)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 4],
  IN     INT16                       (*PerParamTcoResults)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 4],
  IN OUT SETTINGVALUE_EYEWIDTH_PAIR  (*BestTcoResults)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 4]
)
{
  PSYSHOST            Host;
  UINT8               Socket;
  UINT8               Ch;
  UINT8               SubCh;
  UINT8               Strobe;
  UINT8               MaxChDdr;
  UINT16              MinLimit;
  UINT16              MaxLimit;
  UINT16              UsDelay;
  INT16               CenterOfRange;


  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;

  //Assuming TCO limits are DdrLevel
  GetDataGroupLimits (Host, DdrLevel, Param->ParamGsm, &MinLimit, &MaxLimit, &UsDelay);

  CenterOfRange = (INT16) ((MaxLimit - MinLimit) / 2);

  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      for (Strobe = 0; Strobe < (MAX_STROBE_DDR5 /4); Strobe++) {
        // if the new results are larger, select those
        if ((*BestTcoResults)[Ch][SubCh][Strobe].EyeWidth < (*PerParamTcoResults)[Ch][SubCh][Strobe]){
          (*BestTcoResults)[Ch][SubCh][Strobe].EyeWidth = (*PerParamTcoResults)[Ch][SubCh][Strobe];
          (*BestTcoResults)[Ch][SubCh][Strobe].SettingValue = (*SettingValue)[Ch][SubCh][Strobe];

          //if the new results are equal, select the result closest to the middle of the range
        } else if (((*BestTcoResults)[Ch][SubCh][Strobe].EyeWidth == (*PerParamTcoResults)[Ch][SubCh][Strobe])) {
          if ((ABS (CenterOfRange - (*SettingValue)[Ch][SubCh][Strobe])) < (ABS (CenterOfRange - (*BestTcoResults)[Ch][SubCh][Strobe].SettingValue))){
            (*BestTcoResults)[Ch][SubCh][Strobe].SettingValue = (*SettingValue)[Ch][SubCh][Strobe];
          }
        }
      } //Strobe
    } //SubCh
  } //Ch
}


/**
  Compare a given margin results structure with the current best results and update if better

  @param[in]      Param            - Pointer a specific parameter structures
  @param[in]      SettingValue     - Pointer with values of the settings used for the input results structure
  @param[in]      Results          - Pointer that contains the margin results for the SettingValue
  @param[in, out] BestTcoResults   - Pointer to a Tco training results data structure

  @retval None
**/
VOID
UpdateBestTcoResults (
  IN     POWER_TRAIN_PARAM           *Param,
  IN     INT16                       (*SettingValue)[MAX_CH][SUB_CH],
  IN     INT16                       (*PerParamTcoResults)[MAX_CH][SUB_CH],
  IN OUT SETTINGVALUE_EYEWIDTH_PAIR  (*BestTcoResults)[MAX_CH][SUB_CH]
)
{
  PSYSHOST            Host;
  UINT8               Socket;
  UINT8               Ch;
  UINT8               SubCh;
  UINT8               MaxChDdr;
  UINT16              MinLimit;
  UINT16              MaxLimit;
  UINT16              UsDelay;
  INT16               CenterOfRange;


  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;

  //Assuming TCO limits are DdrLevel
  GetDataGroupLimits (Host, DdrLevel, Param->ParamGsm, &MinLimit, &MaxLimit, &UsDelay);

  CenterOfRange = (INT16) ((MaxLimit - MinLimit) / 2);

  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      // if the new results are larger, select those
      if ((*BestTcoResults)[Ch][SubCh].EyeWidth < (*PerParamTcoResults)[Ch][SubCh]){
        (*BestTcoResults)[Ch][SubCh].EyeWidth = (*PerParamTcoResults)[Ch][SubCh];
        (*BestTcoResults)[Ch][SubCh].SettingValue = (*SettingValue)[Ch][SubCh];

        //if the new results are equal, select the result closest to the middle of the range
      } else if (((*BestTcoResults)[Ch][SubCh].EyeWidth == (*PerParamTcoResults)[Ch][SubCh])) {
        if ((ABS (CenterOfRange - (*SettingValue)[Ch][SubCh])) < (ABS (CenterOfRange - (*BestTcoResults)[Ch][SubCh].SettingValue))){
          (*BestTcoResults)[Ch][SubCh].SettingValue = (*SettingValue)[Ch][SubCh];
        }
      }
    } //SubCh
  } //Ch
}


/**
  Program the training parameter setting for all subchannels/strobes based
  on input data in a TcoResults structure.

  @param[in]      Param         - Pointer a specific parameter structures
  @param[in]      BestTcoResults   - Pointer to a Tco training results data structure

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetTcoPerStrobeToBestResults (
  IN     POWER_TRAIN_PARAM           *Param,
  IN     SETTINGVALUE_EYEWIDTH_PAIR  (*BestTcoResults)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 4]
  )
{
  PSYSHOST            Host;
  UINT8               Socket;
  UINT8               Ch;
  UINT8               SubCh;
  UINT8               Strobe;
  MPT_PT              ParamType;
  MRC_GT              ParamGsm;
  UINT8               MaxChDdr;
  INT16               Setting;

  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;
  MaxChDdr = GetMaxChDdr ();

  ParamGsm = Param->ParamGsm;
  ParamType = Param->ParamType;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      for (Strobe = 0; Strobe < (MAX_STROBE_DDR5 / 4); Strobe++) {

        Setting = (*BestTcoResults)[Ch][SubCh][Strobe].SettingValue;

        RcDebugPrintWithDeviceInternal (
          SDBG_MAX, NO_SOCKET, Ch, NO_DIMM, SubCh, NO_RANK, Strobe, NO_BIT,
          "SetTcoPerSubChToBestResults=%d (%d)\n",
          Setting,
          GetIpTcoSetting (Setting)
          );

        GetSetDataGroup (Host, Socket, Ch, SubCh, 0, 0, Strobe, ALL_BITS, DdrLevel, ParamGsm,
          GSM_UPDATE_CACHE | GSM_FORCE_WRITE, &Setting);
      } //Strobe
    } //SubCh
  } //Ch
  return MRC_STATUS_SUCCESS;
}


/**
  Program the training parameter setting for all subchannels based
  on input data in a TcoResults structure.

  @param[in]      Param         - Pointer a specific parameter structures
  @param[in]      BestTcoResults   - Pointer to a Tco training results data structure

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetTcoPerSubChToBestResults (
  IN     POWER_TRAIN_PARAM           *Param,
  IN     SETTINGVALUE_EYEWIDTH_PAIR  (*BestTcoResults)[MAX_CH][SUB_CH]
  )
{
  PSYSHOST            Host;
  UINT8               Socket;
  UINT8               Ch;
  UINT8               SubCh;
  MPT_PT              ParamType;
  MRC_GT              ParamGsm;
  UINT8               MaxChDdr;
  INT16               Setting;

  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;
  MaxChDdr = GetMaxChDdr ();

  ParamGsm = Param->ParamGsm;
  ParamType = Param->ParamType;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      Setting = (*BestTcoResults)[Ch][SubCh].SettingValue;

      RcDebugPrintWithDeviceInternal (
        SDBG_MAX, NO_SOCKET, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
        "SetTcoPerSubChToBestResults=%d (%d)\n",
        Setting,
        GetIpTcoSetting (Setting)
        );

      GetSetDataGroup (Host, Socket, Ch, SubCh, 0, 0, 0, ALL_BITS, DdrLevel, ParamGsm,
        GSM_UPDATE_CACHE | GSM_FORCE_WRITE, &Setting);
    }
  }
  return MRC_STATUS_SUCCESS;
}


/**
  Apply the same offset setting for all strobes.
  This is used for parameters that have one setting per-strobe.

  @param[in]      Param         - Pointer a specific parameter structures
  @param[in]      Offset        - Offset to apply

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
ApplyOffsetToParamPerStrobe (
  IN      POWER_TRAIN_PARAM *Param,
  IN      INT16             Offset
  )
{
  PSYSHOST            Host;
  UINT8               Socket;
  UINT8               Ch;
  MPT_PT              ParamType;
  MRC_GT              ParamGsm;
  UINT8               MaxChDdr;

  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;

  MaxChDdr = GetMaxChDdr ();

  ParamGsm = Param->ParamGsm;
  ParamType = Param->ParamType;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, 0, 0, ALL_STROBES, ALL_BITS, DdrLevel, ParamGsm,
      GSM_WRITE_OFFSET | GSM_UPDATE_CACHE | GSM_FORCE_WRITE, &Offset);
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Apply the same offset setting for all subchannels.
  This is used for parameters that have one setting per-subchannel.

  @param[in]      Param         - Pointer a specific parameter structures
  @param[in]      Offset        - Offset to apply

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
ApplyOffsetToParamPerSubCh (
  IN      POWER_TRAIN_PARAM *Param,
  IN      INT16             Offset
)
{
  PSYSHOST            Host;
  UINT8               Socket;
  UINT8               Ch;
  UINT8               SubCh;
  MPT_PT              ParamType;
  MRC_GT              ParamGsm;
  UINT8               MaxChDdr;

  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;

  MaxChDdr = GetMaxChDdr ();

  ParamGsm = Param->ParamGsm;
  ParamType = Param->ParamType;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      GetSetDataGroup (Host, Socket, Ch, SubCh, 0, 0, 0, ALL_BITS, DdrLevel, ParamGsm,
        GSM_WRITE_OFFSET | GSM_UPDATE_CACHE | GSM_FORCE_WRITE, &Offset);
    }
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Adds signed input value to all setting values in the output structure

  @param[in]  Group            - Training group related to the value
  @param[in]  Value            - Value to assign
  @param[out] Settings         - Pointer the current Setting Values in the structure

  @retval None
**/
VOID
OffsetAllTcoSettingValues (
  IN      MRC_GT               Group,
  IN      INT16                Value,
     OUT  INT16                (*Settings)[MAX_CH][SUB_CH]
)
{
  PSYSHOST            Host;
  UINT8               Ch;
  UINT8               SubCh;
  UINT8               MaxChDdr;
  UINT16              MinLimit;
  UINT16              MaxLimit;
  UINT16              UsDelay;
  UINT8               Socket;


  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;

  MaxChDdr = GetMaxChDdr ();

  //Assuming TCO limits are DdrLevel
  GetDataGroupLimits (Host, DdrLevel, Group, &MinLimit, &MaxLimit, &UsDelay);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      if (((*Settings)[Ch][SubCh] + Value) <= MinLimit) {
        (*Settings)[Ch][SubCh] = MinLimit;
      } else if (((*Settings)[Ch][SubCh] + Value) > MaxLimit) {
        (*Settings)[Ch][SubCh] = MaxLimit;
      } else {
        (*Settings)[Ch][SubCh] += Value;
      }
    } //SubCh
  } //Ch
}

/*

  Callback function to configure the engine before start doing the tests

  @param[in]      CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Callback function executed properly

*/
MRC_STATUS
EFIAPI
SetupAdvDqSweep (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
)
{
  //
  // Call the gerneral purpose CPGC advanced training setup function
  //
  SetupSweepAdvance (CommonParameters);


  return MRC_STATUS_SUCCESS;
}

/**
  Adds signed input value to all setting values in the output structure

  @param[in]  Group            - Training group related to the value
  @param[in]  Value            - Value to assign
  @param[out] Settings         - Pointer the current Setting Values in the structure

  @retval None
**/
VOID
OffsetAllTcoDqDqsSettingValues (
  IN      MRC_GT               Group,
  IN      INT16                Value,
     OUT  INT16                (*Settings)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 4]
)
{
  PSYSHOST            Host;
  UINT8               Ch;
  UINT8               SubCh;
  UINT8               MaxChDdr;
  UINT16              MinLimit;
  UINT16              MaxLimit;
  UINT16              UsDelay;
  UINT8               Socket;
  UINT8               Strobe;


  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;

  MaxChDdr = GetMaxChDdr ();

  //Assuming TCO limits are DdrLevel
  GetDataGroupLimits (Host, DdrLevel, Group, &MinLimit, &MaxLimit, &UsDelay);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      for (Strobe = 0; Strobe < (MAX_STROBE_DDR5 / 4); Strobe++) {
        if (((*Settings)[Ch][SubCh][Strobe] + Value) <= MinLimit) {
          (*Settings)[Ch][SubCh][Strobe] = MinLimit;
        } else if (((*Settings)[Ch][SubCh][Strobe] + Value) > MaxLimit) {
          (*Settings)[Ch][SubCh][Strobe] = MaxLimit;
        } else {
          (*Settings)[Ch][SubCh][Strobe] += Value;
        }
      } //Strobe
    } //SubCh
  } //Ch
}



/**

  Search the data collected in Results for the margin width and put eyewidth in a channel/strobe array
  WARNING:
  This function MUST be executed only after context was already created (CreateContextParameters).
  This function MUST be executed only after resources are allocated (AllocatePoolForResults1D || AllocatePoolForResults1DAllRanks).

  @param[in]      CommonParameters  Pointer to the context information
  @param[in]      Results1D         Pointer to 1D results for all ranks
  @param[out]     MarginsPerStrobe  Pointer that contains per sub-channel minimum margin widths

**/
VOID
GetEyewidthPerStrobeFromCurrentResults1D (
  IN        COMMON_1D_PARAMETERS    *CommonParameters,
  IN        RESULTS_1D              Results1D[MAX_RANK_CH],
      OUT   INT16                   (*MarginsPerByte)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 4]
)
{
  UINT8             ChannelIndex;
  UINT8             SubChannelIndex;
  UINT8             DimmIndex;
  UINT8             RankIndex;
  UINT8             StrobeLinear;
  UINT8             StrobeLogical;
  UINT8             MaxChDdr;
  struct baseMargin *LowerStrobeMargin;
  struct baseMargin *UpperStrobeMargin;
  INT16             AverageByteMargin;


  MaxChDdr = CommonParameters->MaxChDdr;

  for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {

    if (!CommonParameters->ChannelEnabledLocal[ChannelIndex]) {
      continue;
    }

    for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {

      // loop through 5 bytes per sub channel
      for (StrobeLinear = 0; StrobeLinear < (MAX_STROBE / 4); StrobeLinear++) {

        StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

        for (DimmIndex = 0; DimmIndex < MAX_DIMM; DimmIndex++) {
          for (RankIndex = 0; RankIndex < MAX_RANK_DIMM; RankIndex++) {
            if (CommonParameters->RankMask & (1 << (DimmIndex * MAX_RANK_DIMM + RankIndex))) {
              continue;
            }
            if (!IsRankPresent (CommonParameters->Socket, ChannelIndex, DimmIndex, RankIndex)) {
              continue;
            }
            LowerStrobeMargin = GetResultPtrAtStrobeLevel (
                                  CommonParameters,
                                  0,
                                  ChannelIndex,
                                  StrobeLogical,
                                  &(Results1D[DimmIndex * MAX_RANK_DIMM + RankIndex])
                                );

            UpperStrobeMargin = GetResultPtrAtStrobeLevel (
                                  CommonParameters,
                                  0,
                                  ChannelIndex,
                                  (StrobeLogical + (MAX_STROBE / 4)),
                                  &(Results1D[DimmIndex * MAX_RANK_DIMM + RankIndex])
                                );
            AverageByteMargin = ((LowerStrobeMargin->p - LowerStrobeMargin->n) + (UpperStrobeMargin->p - UpperStrobeMargin->n)) / 2;
            //if currently zero, just pick the avg margin between upper and lower strobes
            if ((*MarginsPerByte)[ChannelIndex][SubChannelIndex][StrobeLinear] == 0) {
              (*MarginsPerByte)[ChannelIndex][SubChannelIndex][StrobeLinear]  = AverageByteMargin;
            } else {
              //for non-zero results, pick the smaller value to get common margins for all dimms
              if ((*MarginsPerByte)[ChannelIndex][SubChannelIndex][StrobeLinear]  > AverageByteMargin) {
                (*MarginsPerByte)[ChannelIndex][SubChannelIndex][StrobeLinear]  = AverageByteMargin;
              }
            }
          } //RankIndex
        } //DimmIndex
      } // StrobeLinear loop ...
    } // SubChannelIndex loop ...
  } // ChannelIndex loop
} //GetEyewidthPerStrobeFromCurrentResults1D

/**
  Sweep Tx and discover margins per subchannel per strobe

  @param[in, out] TxDqDqsResults       - Pointer to a Tco training results data structure

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
SweepTxPerStrobe (
  IN OUT  INT16             (*TxDqDqsResults)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 4]
  )
{
  UINT8         Socket;
  UINT8         MaxChDdr;
  UINT32        ChannelMask = 0;
  COMMON_1D_PARAMETERS *CommonParametersPtr;
  RESULTS_1D    Results[MAX_RANK_CH];


  CommonParametersPtr = RcAllocatePool (sizeof (*CommonParametersPtr));
  if (CommonParametersPtr == NULL) {
    return FAILURE;
  }

  ZeroMem ((UINT8 *) (CommonParametersPtr), sizeof (*CommonParametersPtr));

  Socket = GetCurrentSocketId ();
  MaxChDdr = GetMaxChDdr ();

  //
  // Per training HAS, use the same pattern as the Pre Dfe 2D centering test
  //
  SetCurrentTestType (Socket, WritePreDfe2DCenteringTest);

  CreateContextParameters (
    CommonParametersPtr,
    DdrLevel,
    Socket,
    ChannelMask,
    0,
    ALL_SUBCH,
    NULL,
    DdrLevel, TxDqDelay, 0,
    1, 1,
    0, 0, DEFAULT_MARGIN_OFFSET,
    DefaultStartOffset | LimitSearch,
    DefaultResults | ResultsRelative,
    DefaultFsm
    );

  AllocatePoolForResultsAllRanks1D (CommonParametersPtr, Results);

  ProcessSweepAllRanks1D (
    CommonParametersPtr,
    NULL,
    SetupSweepAdvance,
    ExecuteSweepAdvance,
    NULL,
    NULL,
    ResultProcessingAdvance,
    Results
    );

  GetEyewidthPerStrobeFromCurrentResults1D (CommonParametersPtr, Results, TxDqDqsResults);

  ReleaseResourcesAllRanks1D (CommonParametersPtr, Results);

  SetCurrentTestType (Socket, DefaultTest);

  RcFreePool (CommonParametersPtr);

  return MRC_STATUS_SUCCESS;
}


/**
  Copy Tco DqDqs setting values from the input structure to an output structure of just setting values

  @param[in]  BestTcoResults   - Pointer to a Tco training results data structure
  @param[out] Settings         - Pointer the current Setting Values in the structure

  @retval None
**/
VOID
GetTcoDqDqsSettingValues (
  IN      SETTINGVALUE_EYEWIDTH_PAIR  (*BestTcoResults)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 4],
     OUT  INT16                       (*Settings)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 4]
)
{
  PSYSHOST            Host;
  UINT8               Ch;
  UINT8               SubCh;
  UINT8               MaxChDdr;
  UINT8               Socket;
  UINT8               Strobe;

  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;
  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      for (Strobe = 0; Strobe < (MAX_STROBE_DDR5 / 4); Strobe++) {
        (*Settings)[Ch][SubCh][Strobe] = (*BestTcoResults)[Ch][SubCh][Strobe].SettingValue;
      } //Strobe
    } //SubCh
  } //Ch
}


/**
  Print Margins from input structure per subchannel per strobe

  @param[in, out] TxDqDqsResults       - Pointer to a Tco training results data structure

  @retval n/a
**/
VOID
DisplayPerStrobeTcoDqResults (
  IN OUT  INT16             (*TxDqDqsResults)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 4]
  )
{
#ifdef DEBUG_CODE_BLOCK
  PSYSHOST             Host;
  UINT8                Socket;
  UINT8                Ch;
  UINT8                SubCh;
  UINT8                Strobe;
  UINT8                MaxChDdr;
  UINT8                SubChMSVx8;


  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;

  SubChMSVx8 = GetSubChMaxStrobeValid (Host) / 2;

  MaxChDdr = GetMaxChDdr ();

  RcDebugPrint (SDBG_MAX, "Eyewidth results per byte\nBYTE:      ");
  for (Strobe = 0; Strobe < SubChMSVx8; Strobe++) {
    //
    // Skip if this is an ECC Strobe when ECC is disabled
    //
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }

    RcDebugPrint (SDBG_MEM_TRAIN,
      "  %2d ", Strobe);
  } // Strobe loop
  RcDebugPrint (SDBG_MEM_TRAIN,
              "\n");
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
                          "");
      for (Strobe = 0; Strobe < SubChMSVx8; Strobe++) {
      //
      // Skip if this is an ECC Strobe when ECC is disabled
      //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }

        RcDebugPrint (SDBG_MEM_TRAIN,"%5d", (*TxDqDqsResults)[Ch][SubCh][Strobe]);

      } //Strobe
      RcDebugPrint (SDBG_MEM_TRAIN,
                  "\n");
    } //SubCh
  } //Ch
#endif //DEBUG_CODE_BLOCK
} //DisplayPerStrobeTcoDqResults

/**
  Tco Dq/Dqs Fine Training Algorithm

  @param[in, out] BestTcoResults   - Pointer to a Tco DqDqs training results data structure

  @retval SUCCESS
**/
MRC_STATUS
TcoDqDqsFineTraining (
  IN OUT SETTINGVALUE_EYEWIDTH_PAIR  (*BestTcoResults)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 4]
)
{
  PSYSHOST             Host;
  UINT8                Socket;
  INT16                InitialOffset = -4;
  INT16                OffsetAdjustment;
  INT16                (*SettingValues)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 4];
  UINT8                FineStepOffsetSize = 1;
  UINT8                Step;
  POWER_TRAIN_PARAM    Param;
  INT16                (*PerParamTcoResults)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 4];
  MRC_STATUS           Status;


  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;
  // Reuse power training parameter structure

  Param.ParamGsm = LinearCntlTcoDqDqs;
  Status = GetPowerParameterSettings (Host, Socket, &Param);
  if (Status != MRC_STATUS_SUCCESS) {
    return Status;
  }

  SettingValues = RcAllocatePool (sizeof (*SettingValues));
  if (SettingValues == NULL) {
    return FAILURE;
  }
  ZeroMem ((UINT8 *) (SettingValues), sizeof (*SettingValues));

  PerParamTcoResults = RcAllocatePool (sizeof (*PerParamTcoResults));
  if (PerParamTcoResults == NULL) {
    return FAILURE;
  }

  GetTcoDqDqsSettingValues (BestTcoResults, SettingValues);

  for (Step = 0; Step < NUM_TCO_FINE_SETTINGS; Step += FineStepOffsetSize) {
    RcDebugPrint (SDBG_MAX,
      "Tco DqDqs Fine Sweep Offset %d\n", ((Step * FineStepOffsetSize) + InitialOffset));
    if (Step == 0) {
      //The algorithm starts from -4 from the existing position and sweeps forward to +3
      OffsetAdjustment = InitialOffset;
    } else {
      OffsetAdjustment = FineStepOffsetSize;
    }

    OffsetAllTcoDqDqsSettingValues (Param.ParamGsm, OffsetAdjustment, SettingValues);

    ApplyOffsetToParamPerStrobe (&Param, OffsetAdjustment);

    ZeroMem ((UINT8 *) (PerParamTcoResults), sizeof (*PerParamTcoResults));

    //The incoming offset of 0 was tested in coarse training, so only run test if
    //this condition is not true
    if (((Step * FineStepOffsetSize) + InitialOffset) != 0) {

      SweepTxPerStrobe (PerParamTcoResults);

      DisplayPerStrobeTcoDqResults (PerParamTcoResults);

      UpdateBestTcoPerStrobeResults (&Param, SettingValues, PerParamTcoResults, BestTcoResults);

    }

  }

  SetTcoPerStrobeToBestResults (&Param, BestTcoResults);

  RcFreePool (PerParamTcoResults);
  RcFreePool (SettingValues);

  return MRC_STATUS_SUCCESS;
}

/**
  Tco Dq/Dqs Coarse Training Algorithm

  @param[in, out] BestTcoResults   - Pointer to a Tco training results data structure

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
TcoDqDqsCoarseTraining (
  IN OUT SETTINGVALUE_EYEWIDTH_PAIR  (*BestTcoResults)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 4]
)
{
  PSYSHOST             Host;
  UINT8                Socket;
  POWER_TRAIN_PARAM    Param;
  UINT8                Settings;
  MRC_STATUS           Status;
  INT16                (*PerParamTcoResults)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 4];
  INT16                (*SettingValues)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 4];


  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;
  // Reuse power training parameter structure

  Param.ParamGsm = LinearCntlTcoDqDqs;
  Status = GetPowerParameterSettings (Host, Socket, &Param);
  if (Status != MRC_STATUS_SUCCESS) {
    return Status;
  }

  SettingValues = RcAllocatePool (sizeof (*SettingValues));
  if (SettingValues == NULL) {
    return FAILURE;
  }
  ZeroMem ((UINT8 *) (SettingValues), sizeof (*SettingValues));

  PerParamTcoResults = RcAllocatePool (sizeof (*PerParamTcoResults));
  if (PerParamTcoResults == NULL) {
    return FAILURE;
  }

  for (Settings = 0; Settings < Param.NumSettings; Settings++) {
    RcDebugPrint (SDBG_MAX,
      "Tco DqDqs Coarse Sweep Linear Setting %d\n", Param.SettingList[Settings]);
    //
    // Set parameters for each Settings
    //
    SetParamPerChAllStrobes (&Param, Settings);

    SetAllTcoDqDqsSettingValues (Param.SettingList[Settings], SettingValues);
    //
    // Initialize the per parameter results to 0
    //
    ZeroMem ((UINT8 *) (PerParamTcoResults), sizeof (*PerParamTcoResults));

    SweepTxPerStrobe (PerParamTcoResults);

    DisplayPerStrobeTcoDqResults (PerParamTcoResults);

    UpdateBestTcoPerStrobeResults (&Param, SettingValues, PerParamTcoResults, BestTcoResults);

  } //Settings

  SetTcoPerStrobeToBestResults (&Param, BestTcoResults);

  RcFreePool (PerParamTcoResults);
  RcFreePool (SettingValues);

  return MRC_STATUS_SUCCESS;
} //TcoDqDqsCoarseTraining

/**
  Optimizes TCO by margining TX DQ/DQS eye width at various settings for TCO.

  @param[in]      Host  - Pointer to sysHost structure

  @retval SUCCESS
**/
UINT32
TrainTcoDqDqs (
  IN     PSYSHOST Host
  )
{
  UINT8                       Socket;
  SETTINGVALUE_EYEWIDTH_PAIR  (*BestTcoResults)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 4];

  Socket = Host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  // or if memory flow is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0)
      || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)
      || (!IsMemFlowEnabled (TcoDqDqsTraining))) {
    return SUCCESS;
  }

  BestTcoResults = RcAllocatePool (sizeof (*BestTcoResults));
  if (BestTcoResults == NULL) {
    ASSERT (FALSE);
    return FAILURE;
  }

  ZeroMem ((UINT8 *) (BestTcoResults), sizeof (*BestTcoResults));

  TcoDqDqsCoarseTraining (BestTcoResults);

  TcoDqDqsFineTraining (BestTcoResults);

  DisplayResultsDdr5 (Host, Socket, TcoDqDqs);

  RcFreePool (BestTcoResults);
  return SUCCESS;

} // TrainTcoDqDqs

/**

  Do pushing in/out for TxDq/TxDqs

  @param[in] Socket     - Current socket
  @param[in] Direction  - DENORMALIZE (-1) or RENORMALIZE (1)

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
EFIAPI
WrLvlPushOutDdr5 (
  IN UINT8 Socket,
  IN INT8 Direction
  )
{
  PSYSHOST               Host;
  UINT8                  Ch;
  UINT8                  ChannelLocal;
  UINT8                  SubChannel;
  UINT8                  Dimm = 0;
  UINT8                  Dimm2 = 0;
  UINT8                  Rank;
  UINT8                  Strobe;
  UINT8                  Bit;
  UINT8                  MaxChDdr;
  UINT8                  SubChMSVx4;
  BOOLEAN                Adjust[MAX_DIMM];
  INT16                  PiDelay[MAX_DIMM];
  UINT16                 MinDelay[MAX_DIMM];
  UINT16                 MaxDelay[MAX_DIMM];
  INT16                  CurCWL[MAX_DIMM][SUB_CH];
  UINT16                 MinLimit;
  UINT16                 MaxLimit;
  UINT16                 UsDelay;
  CHIP_WL_CLEANUP_STRUCT WLChipCleanUpStruct;

  Host = GetSysHostPointer ();

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    if (Direction == DENORMALIZE) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "WriteLeveling Pushout\n BEFORE WrLvl pushout values\n");
    } else {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "WriteLeveling Pullin\n BEFORE WrLvl pullin values\n");
    }
    DisplayResultsDdr5 (Host, Socket, TxDqsDelay);
    DisplayResultsDdr5 (Host, Socket, TxDqBitDelay);
  }
#endif //DEBUG_CODE_BLOCK

  MaxChDdr = GetMaxChDdr ();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);

  ZeroMem ((UINT16 *) MaxDelay, sizeof (MaxDelay));
  ZeroMem ((UINT16 *) MinDelay, sizeof (MinDelay));
  ZeroMem ((INT16 *) PiDelay, sizeof (PiDelay));
  ZeroMem ((BOOLEAN *) Adjust, sizeof (Adjust));

  GetDataGroupLimits (Host, DdrLevel, TxDqBitDelay, &MinLimit, &MaxLimit, &UsDelay);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      ChannelLocal = Ch;
      GetChannelMapping (Ch, SubChannel, &ChannelLocal);

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
          continue;
        }

        //
        // Get current CWL
        //
        GetCwlAdjDdr5 (Host, Socket, ChannelLocal, SubChannel, Dimm, &CurCWL[Dimm][SubChannel]);

      } // Dimm
    } // SubChannel

    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
          continue;
        }

        MinDelay[Dimm] = MaxLimit;
        MaxDelay[Dimm] = MinLimit;
        Adjust[Dimm] = TRUE;

      } // Dimm

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
          continue;
        }

        for (Dimm2 = 0; Dimm2 < MAX_DIMM; Dimm2++) {
          //
          // DDRT dimm has seperate CWL_ADJ settings. So if 2 dimms share the same type, saying share the same CWL_ADJ registers,
          // the Max/Min Delay should be calculated across dimms within the common channel.
          // If one dimm is DDRT while the other is not, skip it since they have sperate CWL-ADJ register settings.
          //
          // If Dimm == Dimm2 here, this loop simply updates Min/Max value for this Dimm.
          //
          if ((IsDimmPresent (Socket, Ch, Dimm2) == FALSE) ||
              (IsDcpmmPresentDimm (Socket, Ch, Dimm) != IsDcpmmPresentDimm (Socket, Ch, Dimm2))) {
            continue;
          }

          for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
            if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
              continue;
            }

            for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
              //
              // Skip if this is an ECC nibble when ECC is disabled
              //
              if (IsStrobeNotValidDdr5 (Host, Strobe)) {
                continue;
              }

              GetSetDataGroup (Host, Socket, Ch, SubChannel, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, (INT16 *)&PiDelay[Dimm]);
              UpdateMinMaxInt (PiDelay[Dimm], (INT16 *)&MinDelay[Dimm2], (INT16 *)&MaxDelay[Dimm2]);

              for (Bit = 0; Bit < BITS_PER_NIBBLE; Bit++) {

                GetSetDataGroup (Host, Socket, Ch, SubChannel, Dimm, Rank, Strobe, Bit, DdrLevel, TxDqBitDelay, GSM_READ_ONLY, (INT16 *)&PiDelay[Dimm]);
                UpdateMinMaxInt (PiDelay[Dimm], (INT16 *)&MinDelay[Dimm2], (INT16 *)&MaxDelay[Dimm2]);

              } // Bit
            } // Strobe
          } // Rank
        } // Dimm2
      } // Dimm

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
          continue;
        }

        if (Direction == DENORMALIZE) {
          //
          // Direction == DENORMALIZE
          //
          if ((CurCWL[Dimm][SubChannel] > (MIN_CWL_ADJ_DDR5 + 1)) && (MaxDelay[Dimm] <= (MaxLimit - (2 * DCLKPITICKS) - (EXTRAPIMARGIN / 2)))) {
            CurCWL[Dimm][SubChannel] = CurCWL[Dimm][SubChannel] - 2;
            PiDelay[Dimm] = 2 * DCLKPITICKS;
          } else if ((CurCWL[Dimm][SubChannel] > MIN_CWL_ADJ_DDR5) && (MaxDelay[Dimm] <= (MaxLimit - DCLKPITICKS - (EXTRAPIMARGIN / 2)))) {
            CurCWL[Dimm][SubChannel] = CurCWL[Dimm][SubChannel] - 1;
            PiDelay[Dimm] = DCLKPITICKS;
          } else {
            Adjust[Dimm] = FALSE;
          }
        } else {
          //
          // Direction == RENORMALIZE
          //
          if ((CurCWL[Dimm][SubChannel] < (MAX_CWL_ADJ_DDR5 - 1)) && (MinDelay[Dimm] >= ((2 * DCLKPITICKS) + (EXTRAPIMARGIN / 2)))) {
            CurCWL[Dimm][SubChannel] += 2;
            PiDelay[Dimm] = -2 * DCLKPITICKS;
          } else if ((CurCWL[Dimm][SubChannel] < MAX_CWL_ADJ_DDR5) && (MinDelay[Dimm] >= (DCLKPITICKS + (EXTRAPIMARGIN / 2)))) {
            CurCWL[Dimm][SubChannel] += 1;
            PiDelay[Dimm] = -DCLKPITICKS;
          } else {
            Adjust[Dimm] = FALSE;
          }
        } // if DENORMALIZE

        if (Adjust[Dimm]) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, NO_RANK, NO_STROBE, NO_BIT, "Push to CWL_ADJ=%d and TXDq/Dqs by %d\n", CurCWL[Dimm][SubChannel], PiDelay[Dimm]);

          //
          // Set adjusted CWL
          //
          WriteChipWrADDDelaysDdr5 (Host, Socket, Ch, SubChannel, CWLADJ_ALL, 0, CurCWL[Dimm][SubChannel], &WLChipCleanUpStruct);

          //
          // Set TxDq/TxDqs
          //
          for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
            if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
              continue;
            }

            for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
              //
              // Skip if this is an ECC nibble when ECC is disabled
              //
              if (IsStrobeNotValidDdr5 (Host, Strobe)) {
                continue;
              }

              GetSetDataGroup (Host, Socket, Ch, SubChannel, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, (INT16 *)&PiDelay[Dimm]);
              GetSetDataGroup (Host, Socket, Ch, SubChannel, Dimm, Rank, Strobe, ALL_BITS, DdrLevel, TxDqDelay, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, (INT16 *)&PiDelay[Dimm]);

            } // Strobe
          } // Rank
        } // if Adjust
      } // Dimm
    } // SubChannel
  } // Ch

  IO_Reset (Host, Socket);

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    if (Direction == DENORMALIZE) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "WriteLeveling Pushout\n AFTER WrLvl pushout values\n");
    } else {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "WriteLeveling Pullin\n AFTER WrLvl pullin values\n");
    }
    DisplayResultsDdr5 (Host, Socket, TxDqsDelay);
    DisplayResultsDdr5 (Host, Socket, TxDqBitDelay);
  }
#endif //DEBUG_CODE_BLOCK

  return MRC_STATUS_SUCCESS;
}

/**

  Normalize CMD training.
  Normalize CLK/CA/CS/TxDq/TxDqs to reduce turnaround latency.

  @param[in] Host   - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
EFIAPI
NormalizeCmd (
  IN PSYSHOST  Host
  )
{
  UINT8               Socket;
  UINT8               Ch;
  UINT8               SubChannel;
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               Strobe;
  UINT8               Bit;
  UINT8               MaxChDdr;
  UINT8               SubChMSVx4;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  INT16               PiDelay = 0;
  UINT16              MinDelay;
  UINT16              MaxDelay;
  UINT8               Clk;
  UINT8               ClkEnabled[MAX_CLK];
  UINT8               CtlIndex[MAX_CLK];

  Socket = Host->var.mem.currentSocket;
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Normalizing CLK/CS/CA/TxDQ/TxDQS\n");

  if (!IsMemFlowEnabled (CmdNormalization)) {
    return SUCCESS;
  }

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);

  //
  // Push out TxDqsDelay/TxDqBitDelay by denormalization
  //
  WrLvlPushOutDdr5 (Socket, DENORMALIZE);

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    RcDebugPrint (SDBG_MAX, "\n BEFORE Normalization settings...\n");
    DisplayCCCResults (Host, Socket);
    DisplayResultsDdr5 (Host, Socket, TxDqsDelay);
    DisplayResultsDdr5 (Host, Socket, TxDqBitDelay);
  }
#endif //DEBUG_CODE_BLOCK

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    //
    // Set available Clk flag
    //
    ZeroMem ((UINT8 *) ClkEnabled, sizeof (ClkEnabled));
    ZeroMem ((UINT8 *) CtlIndex, sizeof (CtlIndex));

    MinDelay = 255;
    MaxDelay = 0;

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

      RankList = GetRankNvList (Host, Socket, Ch, Dimm);
      for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        Clk = (*RankList)[Rank].ckIndex;
        ClkEnabled[Clk] = 1;
        CtlIndex[Clk] = (*RankList)[Rank].ctlIndex;

      } // Rank
    } // Dimm

    //
    // Read CS delay
    //
    for (Clk = 0; Clk < MAX_CLK; Clk++) {
      if (ClkEnabled[Clk] == 0) {
        continue;
      }
      GetSetCtlGroupDelay (Host, Socket, Ch, CtlIndex[Clk] + CtlGrp0, GSM_READ_ONLY, &PiDelay, &MinDelay, &MaxDelay);
    }

    //
    // Read CMD delay
    //
    if ((Host->nvram.mem.dimmTypePresent == RDIMM) || (Host->nvram.mem.dimmTypePresent == LRDIMM)) {
      GetSetCmdGroupDelay (Host, Socket, Ch, RcdCmdAll, GSM_READ_ONLY, &PiDelay, &MinDelay, &MaxDelay);
    } else {
      GetSetCmdGroupDelay (Host, Socket, Ch, CmdAll, GSM_READ_ONLY, &PiDelay, &MinDelay, &MaxDelay);
    }

    //
    // Read DQS/DQ delay
    //
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

      for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
            //
            // Skip if this is an ECC nibble when ECC is disabled
            //
            if (IsStrobeNotValidDdr5 (Host, Strobe)) {
              continue;
            }

            GetSetDataGroup (Host, Socket, Ch, SubChannel, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, (INT16 *)&PiDelay);
            UpdateMinMaxInt (PiDelay, (INT16 *)&MinDelay, (INT16 *)&MaxDelay);

            for (Bit = 0; Bit < BITS_PER_NIBBLE; Bit++) {
              GetSetDataGroup (Host, Socket, Ch, SubChannel, Dimm, Rank, Strobe, Bit, DdrLevel, TxDqBitDelay, GSM_READ_ONLY, (INT16 *)&PiDelay);
              UpdateMinMaxInt (PiDelay - CMD_NORMALIZE_TXDQ_GUARDBAND, (INT16 *)&MinDelay, (INT16 *)&MaxDelay);

            } // Bit
          } // Strobe
        } // SubChannel
      } // Rank
    } // Dimm

    //
    // Normalize to zero by subtracting the minimum.
    // Being used as offset in following GetSetxxxDelay functions.
    //
    PiDelay = -MinDelay;
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "Normalization values: Min PI = %-4d, Max PI = %-4d, normalization = %-4d ticks\n",
                            MinDelay, MaxDelay, PiDelay);

    (*ChannelNvList)[Ch].normalizationFactor = PiDelay;

    //
    // Set new CLK/CS delay
    //
    for (Clk = 0; Clk < MAX_CLK; Clk++) {
      if (ClkEnabled[Clk] == 0) {
        continue;
      }
      GetSetClkDelay (Host, Socket, Ch, Clk, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &PiDelay);
      GetSetCtlGroupDelay (Host, Socket, Ch, CtlIndex[Clk] + CtlGrp0, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &PiDelay, &MinDelay, &MaxDelay);
    } // clk loop

    //
    // Set new CMD delay
    //
    if ((Host->nvram.mem.dimmTypePresent == RDIMM) || (Host->nvram.mem.dimmTypePresent == LRDIMM)) {
      GetSetCmdGroupDelay (Host, Socket, Ch, RcdCmdAll,  GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &PiDelay, &MinDelay, &MaxDelay);
    } else {
      GetSetCmdGroupDelay (Host, Socket, Ch, CmdAll,  GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &PiDelay, &MinDelay, &MaxDelay);
    }

    //
    // Set new DQS/DQ delay
    //
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

      for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
            //
            // Skip if this is an ECC nibble when ECC is disabled
            //
            if (IsStrobeNotValidDdr5 (Host, Strobe)) {
              continue;
            }

            GetSetDataGroup (Host, Socket, Ch, SubChannel, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, (INT16 *)&PiDelay);
            GetSetDataGroup (Host, Socket, Ch, SubChannel, Dimm, Rank, Strobe, ALL_BITS, DdrLevel, TxDqDelay, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, (INT16 *)&PiDelay);

          } // Strobe
        } // SubChannel
      } // Rank
    } // Dimm
  } // Ch

  SetNormalizationFactorEn (Socket, 1);

  IO_Reset (Host, Socket);

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    RcDebugPrint (SDBG_MAX, "\n AFTER Normalization settings...\n");
    DisplayCCCResults (Host, Socket);
    DisplayResultsDdr5 (Host, Socket, TxDqsDelay);
    DisplayResultsDdr5 (Host, Socket, TxDqBitDelay);
  }
#endif //DEBUG_CODE_BLOCK

  RcDebugPrint (SDBG_MAX, "\n Rerun receive enable training after CMD normalization...\n");
  ReceiveEnableDdr5(Host);

  return SUCCESS;
}

/**

  DeNormalize CMD.
  Function should be invoked after CmdNormalize training step finishes. It has 2 directions.

  By DENORMALIZE direction, all signals will be pushed out back to the original value, same
  as the status before CmdNormalize training step.

  By RENORMALIZE direction, all signals will be pushed in again after DENORMALIZE operation.

  @param[in] Socket     - Current Socket
  @param[in] Direction  - DENORMALIZE (-1) or RENORMALIZE (1)

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
EFIAPI
DeNormalizeCmd (
  IN UINT8     Socket,
  IN INT8      Direction
  )
{
  PSYSHOST            Host;
  UINT8               Ch;
  UINT8               SubChannel;
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               Strobe;
  UINT8               MaxChDdr;
  UINT8               SubChMSVx4;
  INT16               PiDelay = 0;
  UINT16              MinDelay;
  UINT16              MaxDelay;
  UINT8               Clk;
  UINT8               CkEnabled[MAX_CLK];
  UINT8               CtlIndex[MAX_CLK];
  SYS_SETUP           *Setup;
  struct channelNvram (*ChannelNvList )[MAX_CH];
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];

  Host = GetSysHostPointer ();
  Setup = GetSysSetupPointer ();
  MaxChDdr = GetMaxChDdr ();
  SubChMSVx4 = GetMaxStrobeValid (Host);

  if (!IsMemFlowEnabled (CmdNormalization)) {
    return SUCCESS;
  }

  //  Added conditional to identify the type of normalization being done
  RcDebugPrint (SDBG_MAX, "\n");
  if ((Direction == DENORMALIZE) && (IsCmdNormalized (Socket))) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "DeNormalize Cmd\n");
  } else if ((Direction == RENORMALIZE) && (!IsCmdNormalized (Socket))) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "ReNormalize Cmd\n");
  } else {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Error: De/Re-normalization cannot be invoked again.\n");
    return MRC_STATUS_INVALID_PARAMETER;
  }

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    PiDelay = Direction * (*ChannelNvList)[Ch].normalizationFactor;

    MinDelay = 255;
    MaxDelay = 0;

    //
    //  Check the value and see if it is the same value as the normalization factor
    //
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Cmd normalization Factor %d\tDirection %d\n", (*ChannelNvList)[Ch].normalizationFactor, Direction);

    ZeroMem ((UINT8 *)CkEnabled, sizeof (CkEnabled));
    ZeroMem ((UINT8 *)CtlIndex, sizeof (CtlIndex));

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

      RankList = GetRankNvList (Host, Socket, Ch, Dimm);

      for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        Clk = (*RankList)[Rank].ckIndex;
        CtlIndex[Clk] = (*RankList)[Rank].ctlIndex;
        CkEnabled[Clk] = 1;
      } // Rank loop
    } // Dimm loop

    //
    // Set CLK/CS delay
    //
    for (Clk = 0; Clk < MAX_CLK; Clk++) {
      if (CkEnabled[Clk] == 0) {
        continue;
      }

      GetSetClkDelay (Host, Socket, Ch, Clk, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &PiDelay);
      GetSetCtlGroupDelay (Host, Socket, Ch, CtlIndex[Clk] + CtlGrp0, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &PiDelay, &MinDelay, &MaxDelay);
    } // Clk loop

    //
    // Set CMD delay
    //
    if ((Host->nvram.mem.dimmTypePresent == RDIMM) || (Host->nvram.mem.dimmTypePresent == LRDIMM)) {
      GetSetCmdGroupDelay (Host, Socket, Ch, RcdCmdAll, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &PiDelay, &MinDelay, &MaxDelay);
    } else {
      GetSetCmdGroupDelay (Host, Socket, Ch, CmdAll, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &PiDelay, &MinDelay, &MaxDelay);
    }

    //
    // Set DQ/DQS delay
    //
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Ch, Dimm ) == FALSE) {
        continue;
      }

      for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
            //
            // Skip if this is an ECC nibble when ECC is disabled
            //
            if (IsStrobeNotValidDdr5 (Host, Strobe)) {
              continue;
            }

            GetSetDataGroup (Host, Socket, Ch, SubChannel, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, (INT16 *)&PiDelay);
            GetSetDataGroup (Host, Socket, Ch, SubChannel, Dimm, Rank, Strobe, ALL_BITS, DdrLevel, TxDqDelay, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, (INT16 *)&PiDelay);
          } // Strobe loop
        } // SubChannel loop
      } // Rank loop
    } // Dimm loop
  } // Ch loop

  //
  // Push out TxDqsDelay/TxDqBitDelay by de/re-normalization
  //
  if (Direction == DENORMALIZE) {
    WrLvlPushOutDdr5 (Socket, RENORMALIZE);
    SetNormalizationFactorEn (Socket, 0);
  } else {
    WrLvlPushOutDdr5 (Socket, DENORMALIZE);
    SetNormalizationFactorEn (Socket, 1);
  }

  IO_Reset (Host, Socket);

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    RcDebugPrint (SDBG_MAX, "\n AFTER de/re-normalization settings...\n");
    DisplayCCCResults (Host, Socket);
    DisplayResultsDdr5 (Host, Socket, TxDqsDelay);
    DisplayResultsDdr5 (Host, Socket, TxDqBitDelay);
  }
#endif //DEBUG_CODE_BLOCK

  //
  // Re-run Receive Enable training
  //
  RcDebugPrint (SDBG_MAX, "\n Rerun receive enable training after CMD de/re-normalization...\n");
  ReceiveEnableDdr5 (Host);

  return MRC_STATUS_SUCCESS;
} // DeNormalizeCmd


/**
  Calculates (or trains) the final settings for ODTLon_WR_Offset and ODTLoff_WR_Offset

  @param[in]      Host  - Pointer to sysHost structure

  @retval SUCCESS
**/
UINT32
WriteOdtLatencyTraining (
  IN     PSYSHOST Host
  )
{
  UINT8                       Socket;
  UINT8                       Ch;
  UINT8                       MaxChDdr;
  UINT8                       SubCh;
  UINT8                       Dimm;
  UINT8                       Rank;


  Socket = Host->var.mem.currentSocket;

  MaxChDdr = GetMaxChDdr ();
  //
  // Return if this socket is disabled
  // or if memory flow is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0)
      || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)
      || (!IsMemFlowEnabled (TxOdtLatencyTraining))) {
    return SUCCESS;
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
          continue;
        }

        for (Rank = 0; Rank < GetNumberOfRanksOnDimm (Socket, Ch, Dimm); Rank++) {
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          CalculateAndProgramDramOdtlValues (Host, Socket, Ch, SubCh, Dimm, Rank);
        } // Rank
      } // Dimm
    } //SubCh
  } //Ch

  return SUCCESS;

} // WriteOdtLatencyTraining
