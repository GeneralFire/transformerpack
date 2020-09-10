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

#include "Ddr5Core.h"
#include <Library/MemoryCoreLib.h>
#include <Memory/Ddr5MrRegs.h>
#include <Memory/CpgcDefinitions.h>
#include <Library/RcMemAllocLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Memory/Ddr5RcwRegs.h>
#include <Library/SimulationServicesLib.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Memory/JedecDefinitions.h>

#define   RD_DQS_STEP_SIZE_DDR5  2

typedef struct {
  UINT8 BitErrorResult[READ_DQDQS_DDR5_LENGTH][MAX_BITS / 8]; ///< each UINT8 holds 8 bits of results.
} BIT_ERROR_RESULTS_RD_DDR5;

#define MAX_RX_VREF_SAMPLES 3

#define MIN_RCOMP_RXDQS   24
#define MAX_RCOMP_RXDQS  151
#define RXDQS_GUARDBAND    8

#define EVEN_ODD         2
#define EVEN_UI          0
#define ODD_UI           1


#define MAX_DRAM_DQS_OFFSET 4


typedef struct {
  UINT16 PiCenter;
} ReadDqDqsTrainResultPi;

typedef struct {
  UINT8 RxVref;
} ReadDqDqsTrainResultRxVref;

typedef struct {
  UINT8 DramDqsOffset;
  UINT8 RxStrobeInvert;
} ReadDqDqsTrainResultStrobe;


extern VOID
SetupRecEnFineTestDdr5 (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT32   ChBitmask,
  IN UINT8    Dimm,
  IN UINT8    Rank
  );

/**

Take DDRT2 Databuffers Out of Training Mode

@param[in] Host - Pointer to sysHost, the system Host (root) structure
@param[in] Socket  - Socket Number
@param[in] Channel - Channel Number

@retval N/A

**/
VOID
ClearDatabufferTrainingMode(
  IN PSYSHOST Host,
  IN UINT8    Socket
);

/**

  Store the DQ De Swizzle Result

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Socket number
  @param[in] Ch           - Ch number
  @param[in] Dimm         - Dimm number
  @param[in] Rank         - Rank number
  @param[in] SubCh        - SubCh number
  @param[in] Bit          - Bit number

  @retval NA

**/
VOID
StoreDqDeSwizzleResult (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    SubCh,
  IN UINT8    Bit
  );

/**

  Program Rx Strobe Invert.

  @param[in] Host                    - Pointer to sysHost
  @param[in] Dimm                    - Dimm number
  @param[in] Rank                    - Rank number
  @param[in] AllStrobePassDqsOffset  - AllStrobePassDqsOffset value
  @param[in] ResultPass              - ResultPass value
  @param[in] RxStrobeInvertLoop      - RxStrobeInvertLoop loop number

  @retval Status

**/
VOID
ProgramRxStrobeInvert (
  IN PSYSHOST Host,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    AllStrobePassDqsOffset[MAX_CH][SUB_CH],
  IN UINT8 (*ResultPass) [MAX_CH][SUB_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_STROBE_DDR5/2],
  IN UINT8    RxStrobeInvertLoop
  );

//
// Local function prototypes
//
STATIC
UINT32
FindPerBitResults (
  IN  PSYSHOST                  Host,
  IN  UINT8                     Socket,
  IN  UINT8                     Dimm,
  IN  UINT8                     Rank,
  IN  UINT8                     RxStrobeInvertLoop,
  IN  UINT8                     AllStrobePassDqsOffset[MAX_CH][SUB_CH],
  OUT BIT_ERROR_RESULTS_RD_DDR5 (*ErrorResult)[MAX_CH][SUB_CH][EVEN_ODD]
  );

STATIC
UINT32
FindPerBitResultsBrs (
  IN  PSYSHOST      Host,
  IN  UINT8         Socket,
  IN  UINT8         Dimm,
  IN  UINT8         Rank,
  OUT BErrorResult  (*ErrorResult)[MAX_CH][SUB_CH]
  );

  /**

  Save io latency, receive enable delay.

  @param[in] Host           - Pointer to sysHost
  @param[in] Dimm           - Dimm number
  @param[in] Rank           - Rank number
  @param[in] ReadDqsOffset  - ReadDqsOffset value

  @retval Status

  **/
UINT32
SaveIoLatAndReceiveEnableDelay (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8 IoLatencySubCh[MAX_CH][SUB_CH],
  IN UINT16 ReceiveEnableDelay[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

  /**

  Restore io latency, receive enable delay.

  @param[in] Host                   - Pointer to sysHost
  @param[in] Socket                 - Socket number
  @param[in] Dimm                   - Dimm number
  @param[in] Rank                   - Rank number
  @param[in] IoLatencySubCh         - Saved IO Latency values
  @param[in] ReceiveEnableDelay     - Saved Receive Enable values
  @param[in] AllStrobePassDqsOffset - Indication of whether all strobes have passed

  @retval N/A

  **/
VOID
RestoreIoLatAndReceiveEnableDelay (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    IoLatencySubCh[MAX_CH][SUB_CH],
  IN UINT16   ReceiveEnableDelay[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN UINT8    AllStrobePassDqsOffset[MAX_CH][SUB_CH]
  );

VOID
RdDqDqsInitDdr5 (
  IN PSYSHOST Host,
  IN UINT8 Socket
  );

/**

  Program Read DQS Offset Timing.

  @param[in] Host                   - Pointer to sysHost
  @param[in] Dimm                   - Dimm number
  @param[in] Rank                   - Rank number
  @param[in] ReadDqsOffset          - ReadDqsOffset value
  @param[in] AllStrobePassDqsOffset - Strobe Pass info
  @param[in] ResultPass             - Result info
  @param[in] IoLatencySubChOriginal - IO Latency info

  @retval Status

**/
UINT32
ProgramReadDqsOffsetTiming (
  IN PSYSHOST Host,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    ReadDqsOffset,
  IN UINT8    AllStrobePassDqsOffset[MAX_CH][SUB_CH],
  IN UINT8 (*ResultPass) [MAX_CH][SUB_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_STROBE_DDR5/2],
  IN UINT8 IoLatencySubChOriginal[MAX_CH][SUB_CH]
  );

  /**

  Program Read DQS Offset Timing.

  @param[in] Host           - Pointer to sysHost
  @param[in] Socket         - Socket number
  @param[in] Ch             - Ch number
  @param[in] SubCh          - SubCh number
  @param[in] Dimm           - Dimm number
  @param[in] Rank           - Rank number
  @param[in] ReadDqsOffset  - ReadDqsOffset value

  @retval NA

  **/
VOID
ProgramReadDqsOffsetDirectly (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubCh,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    ReadDqsOffset
  );

  /**

   Check the RxStrobeP and RxStrobeN value check

  @param[in] Host               - Pointer to sysHost
  @param[in] Socket             - Socket number
  @param[in] Ch                 - Ch number
  @param[in] SubCh              - SubCh number
  @param[in] Dimm               - Dimm number
  @param[in] Rank               - Rank number
  @param[in] Evenodd            - Even or Odd Strobe
  @param[in] BitNibbleMin       - Minimal Nibble Delay Value
  @param[in] RxStrobeInvert     - RxStrobe Invert Value
  @param[in] DramReadDqsOffset  - Dram Read Dqs Offset value
  @param[in] EncodedNibble      - Encoded Nibble Value

  @retval NA

  **/
VOID
RxStrobeResultCheck (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubCh,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Evenodd,
  IN INT16*   BitNibbleMin,
  IN UINT8    RxStrobeInvert[MAX_CH][SUB_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_STROBE_DDR5/2],
  IN UINT8*   DramReadDqsOffset,
  IN UINT8    EncodedNibble
  );

/**
  Display DRAM Read DQS Offset.

  @param[in]  Socket    Socket number.
**/
VOID
DisplayDramReadDqsOffset (
  IN  UINT8   Socket
  );

#define   REC_EN_LOOP_COUNT 6

/*
  Function to specify loopcount and number of cachelines

  @param[out] LoopCount   Pointer to specify the loopcount
  @param[out] NumCl       Pointer to specify the number of cachelines

  @retval     EFI_SUCCESS Completes execution

*/
STATIC
EFI_STATUS
EFIAPI
SetupRecEnFineParamTarget1 (
  OUT UINT8 *LoopCount,
  OUT UINT8 *NumCl
  )
{

  *LoopCount = REC_EN_LOOP_COUNT + 1;
  *NumCl = 32;

  return EFI_SUCCESS;

} // SetupRecEnFineParamTarget1

/*

  Function to specify  Cpgc Addresses, Wdb pattern, loopcount and number of cachelines

  @param[in]  Host        Pointer to SysHost structure
  @param[out] CpgcAddress Pointer to TCpgcAddress structure
  @param[out] WdbPattern  Pointer to TWdbPattern structure
  @param[out] LoopCount   Pointer to specify the loopcount
  @param[out] NumCl       Pointer to specify the number of cachelines

*/
STATIC
VOID
SetupRecEnFineParam1 (
  PSYSHOST     Host,
  TCPGCAddress *CpgcAddress,
  TWDBPattern  *WdbPattern,
  UINT8        *LoopCount,
  UINT8        *NumCl
  )
{

  SetupRecEnFineParamTarget1 (LoopCount, NumCl);

} // SetupRecEnFineParam


/**

  This function gets the test engine ready to run tests for Receive Enable Fine training

  @param[in] Host              - Pointer to sysHost
  @param[in] Socket              - Socket number
  @param[in] ChBitmask - Bitmask of channels to test

  @retval N/A

**/
VOID
SetupReadDqDqsTestDdr5 (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT32   ChBitmask,
  IN UINT8    Rank
  )
{
  UINT8               LoopCount;
  UINT8               NumCl;

  //                                Rank, Bank, Row, Col
  TCPGCAddress        CpgcAddress = { { 0, 0, 0, 31 },     // Start
                                      { 0, 4, 0, 31 },  // Stop
                                      { 0, 0, 0, 0 },     // Order
                                      { 0, 0, 0, 0 },     // IncRate
                                      { 0, 0, 0, 0 } };   // IncValue
  //                            IncRate, Start, Stop, DQPat
  TWDBPattern         WdbPattern = { 0, 0, 1, BasicVA }; // Should not matter what this is. We do not compare data.

  if (Host->DdrioUltSupport) {
    LoopCount = 2;
    NumCl     = 32;
    CteSetupPattern ("rcven", (LoopCount) | (NumCl << 4), ChBitmask, 1, Rank);
    CteSetOptions ("cmd_stretch", ChBitmask, 0x3, 0, 3);
    CteSetOptions ("cmd_stretch", ChBitmask, 0x3, 4, 3);
  } else {
    SetupRecEnFineParam1 (Host, &CpgcAddress, &WdbPattern, &LoopCount, &NumCl);

    SetupIOTest (Host, Socket, ChBitmask, PatRd, 32, 1, &CpgcAddress, 0, &WdbPattern, 0, 0, 8);
  }
  return;

} // SetupRecEnFineTestDdr5

/**

  @param[in] Host              - Pointer to sysHost
  @param[in] Socket            - Socket number
  @param[in] Dimm              - DIMM number
  @param[in] Rank              - Rank number
  @param[in] ChBitmask         - Channel Mask within the MemSS
  @param[in] BwSerr            - Include the error rwsult for this channel

  @retval Status

**/
STATIC
UINT32
RunReadTrainingPatternTest (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Dimm,
  UINT8     Rank,
  UINT32    ChBitmask,
  UINT32    BwSerr[MAX_CH][SUB_CH][3]
  )
{
  UINT8   Ch;
  UINT32  Status;
  MRC_RT  MrcRankList;
  MRC_MST MemSsType;
  UINT8   MaxChDdr;
  MRC_TT  CurrentTestType;

  CurrentTestType = GetCurrentTestType (Socket);
  MaxChDdr = GetMaxChDdr ();
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  MrcRankList.Dimm = Dimm;
  MrcRankList.Rank = Rank;

  Status = SUCCESS;

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if ((ChBitmask & (1 << Ch)) == 0) {
      continue;
    }

    SelectCPGCRanks (Host, Socket, Ch, 1 << GetLogicalRank (Host, Socket, Ch, Dimm, Rank), 0, 0);
  } // Ch loop
  if (Host->DdrioUltSupport) {
    //
    // LoopCnt = 1
    // NunCL = 32; Just for ULT to save the test time. By HAS, it needs to be 64.
    //
    CteSetupPattern ("rddqdqs", (1) | (32 << 4), ChBitmask, 3, GetPhyRank (Dimm, Rank));
  } else {
    SetupTest (Host, Socket, MemSsType, ChBitmask, 1, &MrcRankList, DdrLevel, CurrentTestType, gsmCsnDelim, 0, FIRST_ITERATION_LOOP);
  }

  IO_Reset (Host, Socket);

  //
  // Run the test
  //
  if (Host->DdrioUltSupport) {
    CteStartPattern (ChBitmask, 0x3, GetPhyRank (Dimm, Rank));
    CteWaitPatternDone ();
  } else {
    TestControl (Host, Socket, MemSsType, ChBitmask, NO_SUBCH, CurrentTestType, 1, START_TEST, NOT_STOP_TEST);
  }
  //
  // Collect test results
  //
  Status = CollectTestResultsDdr5 (Host, Socket, ChBitmask, BwSerr);

  IO_Reset (Host, Socket);

  return Status;
}

/**

  Populates DqSwzDdr5 structure with LRDIMM swizzle information if an LRDIMM is present.

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number
  @param[in] Channel - Channel number
  @param[in] Dimm    - DIMM number
  @param[in] Rank    - Rank number on DIMM

  @retval N/A

**/
VOID
PopulateLrdimmSwizzleInfo (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
#ifdef LRDIMM_SUPPORT
  MRC_STATUS                 Status;
  UINT8                      SubChannel;
  UINT8                      DataBuffer;
  UINT8                      Dword;
  UINT8                      DqNumber;
  UINT32                     SwizzleValue;
  UINT32                     DqMap;
  UINT8                      MapMask = 0x7;  // Each map value is 3 bits
  DDR5_LRDIMM_TRAINING_DATA  *TrainingData;
  struct ddrRank             (*RankList) [MAX_RANK_DIMM];
  UINT32                     (*DbToDramDqMap)[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][SUB_CH][SWIZZLE_RESULTS_LEN];
  UINT32                     (*HostToDbDqMap)[MAX_CH][MAX_DIMM][SUB_CH][SWIZZLE_RESULTS_LEN];

  Status = GetDdr5LrdimmTrainingData (&TrainingData);
  if (Status != MRC_STATUS_SUCCESS) {
    return;
  }
  DbToDramDqMap = &(TrainingData->DbToDramDqMap);
  HostToDbDqMap = &(TrainingData->HostToDbDqMap);

  RankList = GetRankNvList (Host, Socket, Channel, Dimm);

  // Loop through to find the Host-DRAM mapping by first retrieving Host-DB mapping.
  // Then apply Host-DB mapping to DB-DRAM mapping to find final Host-DRAM map.
  for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
    for (DataBuffer = 0; DataBuffer < DB_PER_SUBCH; DataBuffer++) {
      for (DqNumber = 0; DqNumber < DB_DQ_LANE; DqNumber++) {
        // First get Host to DB mapping
        Dword = (DqNumber + (DataBuffer * DB_DQ_LANE)) / 10;
        SwizzleValue = ((*HostToDbDqMap)[Channel][Dimm][SubChannel][Dword] >> (((DqNumber + (DataBuffer * DB_DQ_LANE)) % 10) * 3));
        SwizzleValue &= MapMask;  // This represents the DQ lane on the DB

        // Use DQ lane on DB to find the mapping from DB to DRAM
        Dword = (UINT8) ((SwizzleValue + (DataBuffer * DB_DQ_LANE)) / 10);
        DqMap = ((*DbToDramDqMap)[Channel][Dimm][Rank][SubChannel][Dword] >> (((SwizzleValue + (DataBuffer * DB_DQ_LANE)) % 10) * 3));
        DqMap &= MapMask;

        // Store mapping (Host-DRAM) in DqSwzDdr5
        (*RankList) [Rank].DqSwzDdr5[SubChannel][ (DataBuffer * DB_DQ_LANE + DqNumber) / 2] |= (DqMap << ((DqNumber % 2) * 4));
      } // Dq loop
    } // DataBuffer loop
  } // SubChannel loop
#endif // LRDIMM_SUPPORT
}

/**

  Detects DQ swizzling on board routing for DDR5 DIMM, NVMDIMM DIMMs.

  @param Host  - Pointer to sysHost

  @retval Status

**/
UINT32
DqSwizzleDiscoveryDdr5 (
  PSYSHOST Host
  )
{
  struct rankDevice  (*RankStruct) [MAX_RANK_DIMM];
  UINT8               Socket;
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               Rank;
  UINT32              ChBitmask;
  UINT32              BwSerr[MAX_CH][SUB_CH][3];
  UINT8               Lane;
  UINT8               Strobe;
  UINT8               Bit;
  UINT8               Mask;
  UINT8               MaxStrobe = GetMaxStrobeValid (Host) / 2;
  UINT8               Result;
  UINT8               i;
  UINT8               FoundSel;
  UINT8               SubCh = 0;
  UINT8               SwzSel = 0;
  UINT8               MaxLanes = 8;
  UINT8               LanesPerBwSerr = 4;
  UINT8               SimFeedback = 0;
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];
  struct ddrRank      (*RankList) [MAX_RANK_DIMM];
  UINT8  SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8  SubChMSVx8 = GetSubChMaxStrobeValid (Host) / 2;
  UINT8  MBV = 40;
  UINT8  MaxChDdr;
  DDR5_MODE_REGISTER_25_STRUCT  Mr25;
  DDR5_MODE_REGISTER_26_STRUCT  Mr26;
  DDR5_MODE_REGISTER_27_STRUCT  Mr27;
  DDR5_MODE_REGISTER_28_STRUCT  Mr28;

  MaxChDdr = GetMaxChDdr ();
  Socket = Host->var.mem.currentSocket;
  //
  // Return if Socket is disable or No DIMMs found or DqSwizzleDiscovery is disabled
  //
  if ((!IsMemFlowEnabled (DqSwizzleDiscoveryTest)) || (Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return SUCCESS;
  }

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_DQ_SWIZZLE_DISCOVERY, (UINT16) ((Socket << 8))));

  DoCompDdrioPerSetupOption (Socket, TRAINING_COMP_EVERY_TRAINING);

  SetAepTrainingMode (Host, Socket, CHECKPOINT_MINOR_RX_BACKSIDE_PHASE_TRAINING);

  if (IsBrsPresent (Host, Socket)) {
    MBV = GetMaxBitsValid(Host);//Maximum bits per sub channel.
  } else {
    MBV = GetMaxBitsValid(Host) / 2;//Maximum bits per sub channel.
  }

  SetCurrentTestType (Socket, DqSwzDiscTestDdr5);

  //Clear WDB
  SetPatternBuffers (Host, Socket, MEMINIT, 0);

  //
  // Initialize structures to write during the loop.
  //
  Mr25.Data = 0;
  Mr25.Bits.read_training_pattern_format = 0; // 0B: Serial
  Mr26.Data = 0;
  Mr27.Data = 0;

  //
  // For each DIMM
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    //
    // For each Rank.
    //
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      //
      // Initialize to no ranks present
      //
      ChBitmask = 0;
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);

        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if ((*DimmNvList) [Dimm].dimmPresent == 0) {
          continue;
        }
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }

        // LRDIMM has already run swizzle discovery
        if (IsLrdimmPresent (Socket, Ch, Dimm) == TRUE) {
          PopulateLrdimmSwizzleInfo (Host, Socket, Ch, Dimm, Rank);
          continue;
        }

        // Inidicate this Rank is present on at least one channel
        ChBitmask |= (1 << Ch);

        // Initialize data structure
        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          //
          // Set the Read Training Pattern Format as serial.
          // Probably, no need to set as it's 0 by default.
          //
          DramModeRegWrite (Socket, Ch, 1 << SubCh, Dimm, Rank, DDR5_MODE_REGISTER_25_REG, Mr25.Data);
          //
          // Set the Read Pattern Data0/Data1 as 0
          //
          DramModeRegWrite (Socket, Ch, 1 << SubCh, Dimm, Rank, DDR5_MODE_REGISTER_26_REG, Mr26.Data);
          DramModeRegWrite (Socket, Ch, 1 << SubCh, Dimm, Rank, DDR5_MODE_REGISTER_27_REG, Mr27.Data);
          //
          // Clear the dqSwzDdr5.
          //
          ZeroMem ((UINT8 *) ((*RankList) [Rank].DqSwzDdr5), sizeof ((*RankList) [Rank].DqSwzDdr5));
        }
        //
        // Set mpr_train_ddr_on in host side to filter ACT/PRE.
        //
        SetMprModeFilterActPre (Host, Socket, Ch, 1);
        ConvertRdToMrr (Host, Socket, Ch, 1);
      }  // Ch loop
      //
      // Go to the next Rank if this Rank is not present on any channels
      //
      if (!ChBitmask) {
        continue;
      }
      //
      // Loop through bits 0-7 selecting one DQ to drive high. Run the test for all the channels in parallel.
      //
      for (Lane = 0; Lane < 8; Lane++) {

        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          if ((ChBitmask & (1 << Ch)) == 0) {
            continue;
          }
          for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
            //
            // (for x8 device bits[7:0] map to DQ[0:7]) (DDR4)
            // (for x4 device bits[7:4] map to DQ[0:3]) (DDR4)
            //
            // Invert the DQ. The Read Pattern Invert -
            // Lower DQ Bits register will apply to x4, x8, and x16 devices.(DDR5)
            //
            Mr28.Data = 0;
            Mr28.Bits.dq_invert_lower_dq_bits = BIT0 << Lane;
            DramModeRegWrite (Socket, Ch, 1 << SubCh, Dimm, Rank, DDR5_MODE_REGISTER_28_REG, Mr28.Data);
          }
        } // Ch loop

        //
        // Run test
        //
        RunReadTrainingPatternTest (Host, Socket, Dimm, Rank, ChBitmask, BwSerr);
        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          if ((ChBitmask & (1 << Ch)) == 0) {
            continue;
          }
          RankList = GetRankNvList (Host, Socket, Ch, Dimm);
          DimmNvList = GetDimmNvList (Host, Socket, Ch);
          for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
            if ((*DimmNvList) [Dimm].x4Present && ((*DimmNvList) [Dimm].DcpmmPresent == 0)) {
              if (Lane >= 4) {
                continue;
              } else {
                MaxLanes = 4;
                MaxStrobe = SubChMSVx4;
                LanesPerBwSerr = 8;
                Mask = 0xF;
              }
            } else {
              MaxLanes = 8;
              MaxStrobe = SubChMSVx8;
              LanesPerBwSerr = 4;
              Mask = 0xFF;
            }

            // Simics workaround starts
            if (GetEmulation () & SIMICS_FLAG) {
              SimFeedback = 1;
            }
            if (SimFeedback) {

              //
              //HSX test results with SRx4
              //Dq Swizzle Discovery -- Started
              //Checkpoint: Socket 0, 0xB7, 0x1E, 0x0000
              //N0.C0.R0:  0 ...##... ...##... ...##... ...##... ...##... ...##... ...##... ...##... ...##...
              //N0.C0.R0:  1 #......# #......# #......# #......# #......# #......# #......# #......# #......#
              //N0.C0.R0:  2 ..#..#.. ..#..#.. ..#..#.. ..#..#.. ..#..#.. ..#..#.. ..#..#.. ..#..#.. ..#..#..
              //N0.C0.R0:  3 .#....#. .#....#. .#....#. .#....#. .#....#. .#....#. .#....#. .#....#. .#....#.
              //DQ lanes:     000000000011111111112222222222333333333344444444445555555555666666666677
              //              012345678901234567890123456789012345678901234567890123456789012345678901
              //N0.C0.D0.R0:  302103123021031230210312302103123021031230210312302103123021031230210312
              //Dq Swizzle Discovery - 69ms
              //
              if (MaxLanes <= 4) {
                UINT32 Pattern = 0;
                switch (Lane) {
                  case 0:
                    Pattern = 0x18181818;
                    break;
                  case 1:
                    Pattern = 0x81818181;
                    break;
                  case 2:
                    Pattern = 0x24242424;
                    break;
                  case 3:
                    Pattern = 0x42424242;
                    break;
                  default:
                    //Automated add of default case. Please review.
                    break;
                }
                BwSerr[Ch][SubCh][0] = ~Pattern;
                BwSerr[Ch][SubCh][1] = ~Pattern;
                BwSerr[Ch][SubCh][2] = (UINT8) ~Pattern;
              } else {
                BwSerr[Ch][SubCh][0] = ~(0x01010101 << Lane);
                BwSerr[Ch][SubCh][1] = ~(0x01010101 << Lane);
                BwSerr[Ch][SubCh][2] = ~(0x01 << Lane);
              }

              //Simics workaround ends
            }
#ifdef DEBUG_CODE_BLOCK
            if (GetDebugLevel () & SDBG_MAX) {
              DisplayBwSerrDdr5 (Host, Socket, Ch, SubCh, Dimm, Rank, BwSerr[Ch][SubCh], Lane);
            }
#endif //DEBUG_CODE_BLOCK
            //
            // record passing Lane - 3 bits per target Lane
            // bits[1:0] are DQ swizzle within the Nibble
            // Bit[2] is Nibble swap, if applicable
            //
            for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {

               //
               // X8 dimm   SubChMSVx8 = 5
               // Strobe      CPGC lanes           BwSerr index
               // 0           [7:0]                0
               // 1           [15:8]               0
               // 2           [23:16]              0
               // 3           [31:24]              0
               // 4           [39:32] ECC          2

               // X4 dimm   SubChMSVx4 = 10
               // Strobe      CPGC lanes           BwSerr index
               // 0           [3:0]                0
               // 5           [7:4]                0
               // 1           [11:8]               0
               // 6           [15:12]              0
               // 2           [19:16]              0
               // 7           [23:20]              0
               // 3           [27:24]              0
               // 8           [31:28]              0
               // 4           [35:32] ECC          2
               // 9           [39:36] ECC          2

              if (MaxStrobe == SubChMSVx8) {
                if (Strobe == SubChMSVx8 - 1) {
                  // Strobe 4 is ECC
                  Result = BwSerr[Ch][SubCh][2] & Mask;
                } else {
                  // Strobe 0 - 3
                  Result = (BwSerr[Ch][SubCh][0] >> (Strobe * 8)) & Mask;
                }
              } else {
                if ((Strobe == SubChMSVx4 / 2 - 1) || (Strobe == SubChMSVx4 - 1)) {
                  // Strobe 4 and 9 are ECC
                  Result = (BwSerr[Ch][SubCh][2] >> ((Strobe / (SubChMSVx4 / 2)) * 4)) & Mask;
                } else {
                  if (Strobe < SubChMSVx4 / 2 - 1) {
                    // Strobe 0 - 3
                    Result = (BwSerr[Ch][SubCh][0] >> (Strobe * 8)) & Mask;
                  } else {
                    // Strobe 5 - 8
                    Result = ((BwSerr[Ch][SubCh][0] >> ((Strobe - SubChMSVx4 / 2) * 8)) >> 4) & Mask;
                  }
                } // x4 non ECC strobe
              } // x4 

              FoundSel = 0;
              for (i = 0; i < MaxLanes; i++) {
                if ((Result & (1 << i)) == 0) {
                  if (!FoundSel) {
                    SwzSel = i;
                    FoundSel = 1;
                  } else {
                    //
                    // Flag error if more than one DQ Lane = 1 per Strobe
                    //
                    Host->nvram.mem.socket[Socket].FaultyPartsFlag[Ch] |= ~0;
                    RankList = GetRankNvList (Host, Socket, Ch, Dimm);
                    (*RankList) [Rank].faultyParts[0] |= FPT_TRAINING_FAILED;
                    RcDebugPrint (SDBG_MAX,
                      "More than one DQ Lane = 1: 0x%x!\n", Result);
                    EwlOutputType10 (WARN_DQ_SWIZZLE_DISC, WARN_DQ_SWIZZLE_DISC_UNCORR, Socket, Ch, NO_DIMM, NO_RANK, EwlSeverityWarning, (UINT8) BwSerr[Ch][SubCh][Strobe / LanesPerBwSerr], FoundSel, MaxLanes, Strobe);
                    DisableChannelSw (Host, Socket, Ch);
                  }
                }
              } // i loop

              if (!FoundSel) {
                //
                // Flag error if no DQ Lane = 1 per byte
                //
                Host->nvram.mem.socket[Socket].FaultyPartsFlag[Ch] |= ~0;
                RankList = GetRankNvList (Host, Socket, Ch, Dimm);
                (*RankList) [Rank].faultyParts[0] |= FPT_TRAINING_FAILED;
                RcDebugPrint (SDBG_MAX,
                  "No DQ lanes = 1!\n");
                EwlOutputType10 (WARN_DQ_SWIZZLE_DISC, WARN_DQ_SWIZZLE_DISC_UNCORR, Socket, Ch, NO_DIMM, NO_RANK, EwlSeverityWarning, (UINT8) BwSerr[Ch][SubCh][Strobe / LanesPerBwSerr], FoundSel, MaxLanes, Strobe);
                DisableChannelSw (Host, Socket, Ch);
              }
              //
              // Save Result for later use
              //
              (*RankList) [Rank].DqSwzDdr5[SubCh][ (Strobe * MaxLanes + Lane) / 2] |= (SwzSel << ((Lane % 2) * 4));
            } // Strobe loop
          } //subchannel loop
        } // Ch loop
      } // Lane loop

      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if ((ChBitmask & (1 << Ch)) == 0) {
          continue;
        }
        RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);
        DimmNvList = GetDimmNvList (Host, Socket, Ch);

        if ((*DimmNvList) [Dimm].DcpmmPresent == 1) {
          SetRankDAMprFnv (Host, Socket, Ch, Dimm, 0);
          SetRankDMprFnv (Host, Socket, Ch, Dimm, 0);
        }
#ifdef LRDIMM_SUPPORT
        (*RankStruct) [Rank].CurrentDramMode = 0;
#endif
      } // Ch loop
    } // Rank loop
  } // Dimm loop

  RcDebugPrint (SDBG_MEM_TRAIN,
    "\n\n");
  RcDebugPrint (SDBG_MEM_TRAIN,
    "START_DATA_RX_DQ_SWIZZLE \n");
  RcDebugPrint (SDBG_MEM_TRAIN, "BIT:              ");
  for (Bit = 0; Bit < MBV; Bit++) {
    RcDebugPrint (SDBG_MEM_TRAIN, "%4d", Bit);
  }
  RcDebugPrint (SDBG_MEM_TRAIN, "\n");

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
        if ((*DimmNvList) [Dimm].dimmPresent == 0) {
          continue;
        }
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT,
            "");
          for (Bit = 0; Bit < MBV; Bit++) {
            RcDebugPrint (SDBG_MEM_TRAIN,
              " %3d", ((*RankList) [Rank].DqSwzDdr5[SubCh][Bit / 2] >> ((Bit % 2) * 4)) & 7);
            //
            // Store the DQ deswizzle result.
            //
            StoreDqDeSwizzleResult (Host, Socket, Ch, Dimm, Rank, SubCh, Bit);
          } // Bit loop
          RcDebugPrint (SDBG_MEM_TRAIN,
            "\n");
        } // SubCh loop
      } // Rank loop
    }// Dimm Loop
  } // Ch loop
  RcDebugPrint (SDBG_MEM_TRAIN,
    "STOP_DATA_RX_DQ_SWIZZLE \n");
  RcDebugPrint (SDBG_MAX,"\n");

  Host->nvram.mem.socket[Socket].TrainingStepDone.DqSwzDone = 1;
  SetAepTrainingMode (Host,Socket, DISABLE_TRAINING_STEP);

  FifoTrainReset (Host, Socket);

  return SUCCESS;
} // DqSwizzleDiscoveryDdr5

  /**

  DDR5 read DQ DQS training DRAM LFSR setup.

  @param Host  - Pointer to sysHost

  @retval Status

  **/
UINT32
CoarseReadDqDqsDramPatternInit (
  PSYSHOST Host,
  UINT8    Dimm,
  UINT8    Rank
  )
{
  UINT8                         Socket;
  UINT8                         Ch;
  UINT8                         MaxChDdr;
  DDR5_MODE_REGISTER_25_STRUCT  Mr25;
  DDR5_MODE_REGISTER_26_STRUCT  Mr26;
  DDR5_MODE_REGISTER_27_STRUCT  Mr27;

  MaxChDdr = GetMaxChDdr ();
  Socket = Host->var.mem.currentSocket;
  RcDebugPrint (SDBG_MEM_TRAIN,
    "CoarseReadDqDqsDramPatternInit start\n");

  //
  // Initialize structures to write during the loop.
  //
  Mr25.Data = 0;
  Mr25.Bits.read_training_pattern_format = 0; // 0B: Serial
  Mr26.Data = 0;
  Mr26.Bits.read_pattern_lfsr_seed_ui_0 = ((RD_DQ_DQS_PATTERN_DDR5 & BIT0) == 0) ? 0 : 1;
  Mr26.Bits.read_pattern_lfsr_seed_ui_1 = ((RD_DQ_DQS_PATTERN_DDR5 & BIT1) == 0) ? 0 : 1;
  Mr26.Bits.read_pattern_lfsr_seed_ui_2 = ((RD_DQ_DQS_PATTERN_DDR5 & BIT2) == 0) ? 0 : 1;
  Mr26.Bits.read_pattern_lfsr_seed_ui_3 = ((RD_DQ_DQS_PATTERN_DDR5 & BIT3) == 0) ? 0 : 1;
  Mr26.Bits.read_pattern_lfsr_seed_ui_4 = ((RD_DQ_DQS_PATTERN_DDR5 & BIT4) == 0) ? 0 : 1;
  Mr26.Bits.read_pattern_lfsr_seed_ui_5 = ((RD_DQ_DQS_PATTERN_DDR5 & BIT5) == 0) ? 0 : 1;
  Mr26.Bits.read_pattern_lfsr_seed_ui_6 = ((RD_DQ_DQS_PATTERN_DDR5 & BIT6) == 0) ? 0 : 1;
  Mr26.Bits.read_pattern_lfsr_seed_ui_7 = ((RD_DQ_DQS_PATTERN_DDR5 & BIT7) == 0) ? 0 : 1;
  Mr27.Data = 0;
  Mr27.Bits.read_pattern_lfsr_seed_ui_8 = ((RD_DQ_DQS_PATTERN_DDR5 & BIT8) == 0) ? 0 : 1;
  Mr27.Bits.read_pattern_lfsr_seed_ui_9 = ((RD_DQ_DQS_PATTERN_DDR5 & BIT9) == 0) ? 0 : 1;
  Mr27.Bits.read_pattern_lfsr_seed_ui_10 = ((RD_DQ_DQS_PATTERN_DDR5 & BIT10) == 0) ? 0 : 1;
  Mr27.Bits.read_pattern_lfsr_seed_ui_11 = ((RD_DQ_DQS_PATTERN_DDR5 & BIT11) == 0) ? 0 : 1;
  Mr27.Bits.read_pattern_lfsr_seed_ui_12 = ((RD_DQ_DQS_PATTERN_DDR5 & BIT12) == 0) ? 0 : 1;
  Mr27.Bits.read_pattern_lfsr_seed_ui_13 = ((RD_DQ_DQS_PATTERN_DDR5 & BIT13) == 0) ? 0 : 1;
  Mr27.Bits.read_pattern_lfsr_seed_ui_14 = ((RD_DQ_DQS_PATTERN_DDR5 & BIT14) == 0) ? 0 : 1;
  Mr27.Bits.read_pattern_lfsr_seed_ui_15 = ((RD_DQ_DQS_PATTERN_DDR5 & BIT15) == 0) ? 0 : 1;

  //
  // Initialize to no ranks present
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
      continue;
    }
    //
    // Set the Read Training Pattern Format as serial.
    // Probably, no need to set as it's 0 by default.
    //
    DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_25_REG, Mr25.Data);
    DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_26_REG, Mr26.Data);
    DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_27_REG, Mr27.Data);
    if (IsDcpmmPresentDimm (Socket, Ch, Dimm) == TRUE) {
      DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW83_REG, LRDIMM_HRD_TRAINING);
    }
  }  // Ch loop
  return SUCCESS;
}

/**

Perform Coarse ReadDqDqs training for BRS DDR5.

@param Host - Pointer to sysHost, the system Host (root) structure

@retval SUCCESS

**/
UINT32
CoarseReadDqDqsPerBitBrs (
  IN PSYSHOST Host
  )
{
  struct rankDevice  (*RankStruct) [MAX_RANK_DIMM];
  UINT8               Socket;
  UINT8               Ch;
  UINT8               SubCh;
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               Nibble;
  UINT8               EncodedNibble;
  UINT8               Bit;
  UINT8               BitIdx;
  UINT8               Msl;
  INT16               BitSkew[BITS_PER_NIBBLE];
  INT16               BitNibbleMin;
  INT16               BitSkewMax;
  UINT16              BitCenter[8];
  UINT8               RankPresent;
  UINT8               Channel = 0;
  PerBitPi            ErrorStart[MAX_CH];
  PerBitPi            ErrorEnd[MAX_CH];
  PerBitPi            PiPosition[MAX_CH];
  UINT16              WorstLeft;
  UINT16              WorstRight;
  UINT32              Status;
  UINT8               FaultyPartsStatus;
  struct bitMarginCh  ResultsBit;
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];
  struct ddrRank      (*RankList) [MAX_RANK_DIMM];
  BErrorResult        (*ErrorResult) [MAX_CH][SUB_CH];
  UINT8  MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MBV = GetMaxBitsValid (Host);

  if (!IsMemFlowEnabled (ReadDqDqsTraining)) {
    return SUCCESS;
  }

  Status = SUCCESS;
  Socket = Host->var.mem.currentSocket;
  SetCurrentTestType (Socket, RdDqTest);
  if (!IsBrsPresent (Host, Socket)) {
    return SUCCESS;
  }

  //
  // Return if this Socket is disabled
  //

  if (IsSocketDimmPopulated (Host, Socket) != TRUE) {
    return SUCCESS;
  }

  ErrorResult = RcAllocatePool (sizeof (*ErrorResult));
  if (ErrorResult == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }
  RdDqDqsInitDdr5 (Host, Socket);

  // Initialize LE and RE Margin results structure


  ZeroMem ((UINT8 *)&ResultsBit, sizeof (ResultsBit));
  ZeroMem ((UINT8 *) (*ErrorResult), sizeof ((*ErrorResult)));
  //
  // Train each DIMM
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    //
    // Train each Rank
    //
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      //
      // Initialize to no ranks present
      //
      RankPresent = 0;

      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }

        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }

        if (FeaturePcdGet (PcdCteBuild) == TRUE) {
          //
          // Init DRAM interface only for CTE.
          //
          InitDramInterface (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank);
        }
        //
        // Inidicate this Rank is present on at least one channel
        //
        RankPresent |= (*RankList) [Rank].enabled;

        //
        // Tell the iMC not to issue activates and precharges
        //
        SetMprTrainModeDdr5 (Host, Socket, Ch, 1);
        ConvertRdToMrr (Host, Socket, Ch, 1);
      } // Ch loop

      //
      // Go to the next Rank if this Rank is not present on any channels
      //
      if (!RankPresent) {
        continue;
      }
      //
      // DRAM Pattern init.
      //
      CoarseReadDqDqsDramPatternInit (Host, Dimm, Rank);
      //
      // Data collecting
      //
      Status = FindPerBitResultsBrs (Host, Socket, Dimm, Rank, ErrorResult);

      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }
        SetMprTrainModeDdr5 (Host, Socket, Ch, 0);
        ConvertRdToMrr (Host, Socket, Ch, 0);

        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }
        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          Channel = Ch;
          GetChannelMapping (Ch, SubCh, &Channel);

          SetRankMPR (Host, Socket, Channel, Dimm, Rank, 0);
#ifdef LRDIMM_SUPPORT
          (*RankStruct) [Rank].CurrentDramMode = 0;
#endif

          WorstLeft = 0xFF;
          WorstRight = 0xFF;

          for (Bit = 0; Bit < MBV; Bit++) {

            Nibble = Bit / 4;
            if (Nibble % 2) {
              EncodedNibble = ((Nibble + 17) / 2) - 4;
            } else {
              EncodedNibble = Nibble / 2;
            }

            //
            // Skip if this is an ECC Strobe when ECC is disabled
            //
            if (IsBitNotValidSubChDdr5 (Host, Bit)) {
              continue;
            }

            Status = FindErrorGraphMin (Host, Socket, Ch, Dimm, Rank, EncodedNibble, &(*ErrorResult) [Ch][SubCh].BitErrorResult[Bit][0],
                       MEM_CHIP_POLICY_VALUE (Host, MaxPhaseInReadAdjustmentDq), &ErrorStart[Ch].PiPosition[Bit],
                       &ErrorEnd[Ch].PiPosition[Bit], &PiPosition[Ch].PiPosition[Bit]);

            //LE and RE Margins
            ResultsBit.bits[Ch][Bit].n = ErrorEnd[Ch].PiPosition[Bit];
            ResultsBit.bits[Ch][Bit].p = ErrorStart[Ch].PiPosition[Bit];

            if (Status == FAILURE) {

              //
              // Mark this part as faulty
              //
              Host->nvram.mem.socket[Socket].FaultyPartsFlag[Ch] |= (1 << EncodedNibble);

              (*RankList) [Rank].faultyParts[EncodedNibble] |= FPT_RD_DQ_DQS_FAILED;

              RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, EncodedNibble, Bit,
                "Failed Read DqDqsPerBit\n");

              FaultyPartsStatus = EvaluateFaultyParts (Host, Socket, Ch);

              switch (FaultyPartsStatus) {
                case FPT_NO_ERROR:
                  break;
                case FPT_CORRECTABLE_ERROR:
                  // if correctable  log the warning -> OutputWarning
                  // careful for cases when warning gets promoted to error !!!  - log the Strobe
                  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, EncodedNibble, NO_BIT,
                    "RdDqDqs Correctable\n");
                  EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_FPT_MINOR_RD_DQ_DQS, Socket, Ch, Dimm, Rank, EwlSeverityWarning, 0xFF, Bit, RxDqsBitDelay, DdrLevel, (UINT8) (ResultsBit.bits[Ch][Bit].p - ResultsBit.bits[Ch][Bit].n));
                  Status = SUCCESS;
                  break;
                default:
                  // does anything need to be done here?
                  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "RdDqDqs training failure!!!\n");
                  //
                  // Add this error to the warning log for both correctable and uncorrectable errors.
                  //
                  DisableChannelSw (Host, Socket, Ch);
                  EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_RD_DQ_DQS, Socket, Ch, Dimm, Rank, EwlSeverityWarning, 0xFF, Bit, RxDqsBitDelay, DdrLevel, (UINT8) (ResultsBit.bits[Ch][Bit].p - ResultsBit.bits[Ch][Bit].n));
                  Status = FAILURE;
                  break;
              }
            } // if FAILURE


            if ((PiPosition[Ch].PiPosition[Bit] - ErrorEnd[Ch].PiPosition[Bit]) < WorstLeft) {
              WorstLeft = PiPosition[Ch].PiPosition[Bit] - ErrorEnd[Ch].PiPosition[Bit];
            }
            if ((ErrorStart[Ch].PiPosition[Bit] - PiPosition[Ch].PiPosition[Bit]) < WorstRight) {
              WorstRight = ErrorStart[Ch].PiPosition[Bit] - PiPosition[Ch].PiPosition[Bit];
            }
          } // Bit loop

#ifdef DEBUG_CODE_BLOCK
          //Print LE and RE Per Bit Deskew Margins for current Rank
          if (GetDebugLevel () & SDBG_MAX) {
            DisplayREandLEMarginsDdr5 (Host, Socket, Ch, SubCh, Dimm, Rank, &ResultsBit, RxDqsDelay);
          }
#endif // DEBUG_CODE_BLOCK
          DetectLowMargin (Host, Ch, Dimm, Rank, &ResultsBit, RxDqsDelay, NON_PER_BIT);

          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
            "Dimm = %d, Rank = %d\n", Dimm, Rank);
          RcDebugPrint (SDBG_MAX,
            "         Per Bit Margin Center       Per Bit Skew\n");
          RcDebugPrint (SDBG_MAX,
            "N# = Nibble, BCx = Bit Center, BSx = Bit Skew , MSL = Most Skewed Lane\n\n");
          RcDebugPrint (SDBG_MAX,
            "N# BC0 BC1 BC2 BC3   NPi   BS0 BS1 BS2 BS3  MSL\n");

          // Loop for each Nibble/Strobe.
          for (Nibble = 0; Nibble < MSVx4; Nibble++) {

            //
            // Skip if this is an ECC Nibble when ECC is disabled
            //
            if (IsNibbleNotValid (Host, Nibble)) {
              continue;
            }
            EncodedNibble = GetStrobeFromNibble (Host, Nibble);

            BitNibbleMin = 127;   // Set to largest possible
            RcDebugPrint (SDBG_MAX,
              "%2d ", Nibble);
            //
            // Loop for all the bits in this Nibble
            //
            for (Bit = 0; Bit < BITS_PER_NIBBLE; Bit++) {
              //
              // Get the absolute Bit index (0-72)
              //
              BitIdx = (Nibble * 4) + Bit;
              //
              // Find the center for this Bit
              //
              BitCenter[Bit] = PiPosition[Ch].PiPosition[BitIdx];

              RcDebugPrint (SDBG_MAX,
                "%3d ", BitCenter[Bit]);
              //
              // Check if the center of this Bit is smaller than the smallest found so far
              //
              if (BitCenter[Bit] < BitNibbleMin) {
                BitNibbleMin = BitCenter[Bit];
              }
            } // for (Bit)

            RcDebugPrint (SDBG_MAX,
              "  %3d   ", BitNibbleMin);
            GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, EncodedNibble, 0, DdrLevel, RxDqDqsDelay, GSM_UPDATE_CACHE, &BitNibbleMin);
            // Loop for all the bits in this Nibble
            BitSkewMax = 0;
            Msl = 0;
            for (Bit = 0; Bit < BITS_PER_NIBBLE; Bit++) {
              BitSkew[Bit] = BitCenter[Bit] - BitNibbleMin;
              if (BitSkew[Bit] > BitSkewMax) {
                BitSkewMax = BitSkew[Bit];
                Msl = Bit;
              }
              RcDebugPrint (SDBG_MAX,
                "%3d ", BitSkew[Bit]);

            } // for (Bit)

            RcDebugPrint (SDBG_MAX, " %3d\n", Msl);
            //
            // Update the register
            // Pass a pointer to all bits in the Nibble - the Get/Set routine will need them all
            //
            GetSetDataGroup (Host, Socket, Channel, SubCh, Dimm, Rank, EncodedNibble, ALL_BITS, DdrLevel, RxDqsBitDelay, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET,
              &BitSkew[0]);
          } // for Nibble
        }//subch loop
      } // Ch loop
    } // Rank loop
  } // Dimm loop

  FifoTrainReset (Host, Socket);
#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    DisplayDramReadDqsOffset (Socket);
    DisplayResultsDdr5 (Host, Socket, RxDqDqsDelay);
    DisplayResultsDdr5 (Host, Socket, RxDqDelay);
    DisplayResultsDdr5 (Host, Socket, RxDqsPDelay);
    DisplayResultsDdr5 (Host, Socket, RxDqsNDelay);
    DisplayResultsDdr5 (Host, Socket, RxDqsBitDelay);
    DisplayResultsDdr5 (Host, Socket, RxDqsNBitDelay);
    DisplayResultsDdr5 (Host, Socket, RxDqsPBitDelay);
    RcDebugPrint (SDBG_MEM_TRAIN,"\n");
  }
#endif // DEBUG_CODE_BLOCK
  ReadDqDqsCleanup (Host, Socket);//BRS_TODO. Check with EV whether this is needed.

  RcFreePool (ErrorResult);
  return Status;

} // CoarseReadDqDqsPerBitBrs


/**

  Offsets the RxVref across all populated channels, sub-channels, and strobe

  @param[in]  Host      Pointer to syshost
  @param[in]  Socket    Current socket under test (0-based)
  @param[in]  Dimm      Current dimm under test (0-based)
  @param[in]  Rank      Current rank under test (0-based)
  @param[in]  Offset    Offset to be applied to all

**/
VOID
OffsetRxVref (
  IN    PSYSHOST Host,
  IN    UINT8   Socket,
  IN    UINT8   Dimm,
  IN    UINT8   Rank,
  IN    INT16   Offset
  )
{

  UINT8   Ch;
  UINT8   MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, ALL_STROBES, ALL_BITS, DdrLevel, RxVref, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_WRITE_OFFSET, &Offset);

  }

}

/**
  Check whether or not Coarse Rd Dq/Dqs training is completed

  @param[in] Socket         - Socket number

  @retval TRUE              - Coarse Rd Dq/Dqs training is completed
          FALSE             - Coarse Rd Dq/Dqs training is not completed

**/
BOOLEAN
IsCoarseRdDqDqsTrainingDone (
  IN  UINT8       Socket
  )
{
  PSYSHOST  Host;

  Host = GetSysHostPointer ();

  if (Host->var.mem.socket[Socket].TrainingStatus.Bits.CoarseRdDqDqsTrainingDone == 1) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Set Coarse Rd Dq/Dqs training done flag

  @param[in] Socket         - Socket number
  @param[in] TrainingDone   - Coarse Rd Dq/Dqs training done or not

  @retval n/a

**/
VOID
SetCoarseRdDqDqsTrainingDone (
  IN  UINT8       Socket,
  IN  BOOLEAN     TrainingDone
  )
{
  PSYSHOST  Host;

  Host = GetSysHostPointer ();

  if (TrainingDone) {
    Host->var.mem.socket[Socket].TrainingStatus.Bits.CoarseRdDqDqsTrainingDone = 1;
  } else {
    Host->var.mem.socket[Socket].TrainingStatus.Bits.CoarseRdDqDqsTrainingDone = 0;
  }
}

/**

Perform Coarse ReadDqDqs training for DDR5.

@param[in] Host - Pointer to sysHost, the system Host (root) structure

@retval SUCCESS

**/
UINT32
CoarseReadDqDqsPerBit (
  IN PSYSHOST Host
  )
{
  BOOLEAN                     EvenOddPass = TRUE;
  BOOLEAN                     AllBitsPass = TRUE;
  UINT8                       Socket;
  UINT8                       Ch;
  UINT8                       SubCh;
  UINT8                       Dimm;
  UINT8                       Rank;
  UINT8                       Nibble;
  UINT8                       EncodedNibble;
  UINT8                       Bit;
  UINT8                       BitIdx;
  UINT8                       Msl;
  UINT8                       RankPresent;
  UINT8                       Channel = 0;
  UINT8                       Evenodd;
  UINT8                       RxVrefLoop = 0;
  UINT8                       AllStrobePassDqsOffset[MAX_CH][SUB_CH];
  UINT8                       AllStrobePassDqsOffsetFlag = 1;
  UINT8                       AllChPassDqsOffsetFlag = 1;
  UINT8                       AllChPassDqsOffset = 0;
  UINT8                       MaxChDdr;
  UINT8                       RxStrobeInvertLoop = 0;
  UINT8                       MSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8                       MBV = MAX_BITS_DDR5 / 2;
  UINT8                       RxStrobeInvertValue = 0;
  UINT8                       IoLatencySubCh[MAX_CH][SUB_CH];
  UINT8                       DramReadDqsOffset = 0;
  UINT8                       (*ResultPass)[MAX_CH][SUB_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2];
  UINT8                       ResultPassBit[SUB_CH][MAX_BITS_DDR5 / 2];
  INT16                       RxVrefOffset[MAX_RX_VREF_SAMPLES] = {0, -5, +5};
  INT16                       BitSkew[BITS_PER_NIBBLE];
  INT16                       BitNibbleMin;
  INT16                       BitSkewMax;
  UINT16                      BitCenter[8];
  UINT16                      WorstLeft;
  UINT16                      WorstRight;
  UINT16                      ReceiveEnableDelay[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT32                      Status;
  PerBitPi                    ErrorStart[MAX_CH];
  PerBitPi                    ErrorEnd[MAX_CH];
  PerBitPi                    PiPosition[MAX_CH][SUB_CH][EVEN_ODD];
  MRC_GT                      GroupStrobe;
  MRC_GT                      GroupBit;
  struct bitMarginCh          ResultsBit[SUB_CH][EVEN_ODD];
  struct dimmNvram            (*DimmNvList)[MAX_DIMM];
  struct ddrRank              (*RankList)[MAX_RANK_DIMM];
  struct rankDevice           (*RankStruct)[MAX_RANK_DIMM];
  BIT_ERROR_RESULTS_RD_DDR5   (*ErrorResult)[MAX_CH][SUB_CH][EVEN_ODD];
  ReadDqDqsTrainResultPi      (*TrainingResultPi)[MAX_CH][SUB_CH][MAX_BITS][EVEN_ODD];
  ReadDqDqsTrainResultRxVref  TrainingResultRxVref[MAX_CH][SUB_CH][MAX_BITS];
  ReadDqDqsTrainResultStrobe  TrainingResultStrobe[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  ACT_TO_ACT_SAVE_STRUCT      ActToActSave[MAX_CH];
  UINT8                       (*ErrorSamples)[READ_DQDQS_DDR5_LENGTH];
  UINTN                       Index;
  BOOLEAN                     CheckCurrentSweep;
  BOOLEAN                     CheckForPassingCondition[MAX_CH][SUB_CH][EVEN_ODD][MAX_BITS];

  if (!IsMemFlowEnabled (ReadDqDqsTraining)) {
    return SUCCESS;
  }

  MaxChDdr = GetMaxChDdr ();
  Status = SUCCESS;
  Socket = Host->var.mem.currentSocket;
  SetCurrentTestType (Socket, RdDqTest);

  //
  // Return if this Socket is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return SUCCESS;
  }

  ErrorResult = RcAllocatePool (sizeof (*ErrorResult));
  if (ErrorResult == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }

  DoCompDdrioPerSetupOption (Socket, TRAINING_COMP_EVERY_TRAINING);
  TrainingResultPi = RcAllocatePool (sizeof (*TrainingResultPi));
  if (TrainingResultPi == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }

  ResultPass = RcAllocatePool (sizeof (*ResultPass));
  if (ResultPass == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }

  ErrorSamples = RcAllocatePool (sizeof (*ErrorSamples));
  if (ErrorSamples == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }

  SetAepTrainingMode (Host, Socket, CHECKPOINT_MINOR_RX_BACKSIDE_PHASE_TRAINING);

  RdDqDqsInitDdr5 (Host, Socket);

  //
  // Initialize ResultPass, ResultsBit, ErrorResult structure.
  //
  ZeroMem ((UINT8 *) (ResultPass), sizeof (*ResultPass));
  ZeroMem ((UINT8 *) &ResultsBit, sizeof (ResultsBit));
  ZeroMem ((UINT8 *) (*ErrorResult), sizeof ((*ErrorResult)));
  ZeroMem ((UINT8 *) (TrainingResultPi), sizeof ((*TrainingResultPi)));
  ZeroMem ((UINT8 *) &ResultPassBit, sizeof (ResultPassBit));
  ZeroMem ((UINT8 *) &IoLatencySubCh, sizeof (IoLatencySubCh));
  ZeroMem ((UINT8 *) &ReceiveEnableDelay, sizeof (ReceiveEnableDelay));
  SetMem (&(*ResultPass), sizeof ((*ResultPass)), 1);


  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    ActToActSave[Ch].TRrdSOrg = 0;
    ActToActSave[Ch].TRrdLOrg = 0;
    ActToActSave[Ch].TRcdRdOrg = 0;
    ActToActSave[Ch].TRrsrOrg = 0;
    ActToActSave[Ch].TRrSgOrg = 0;
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    SetActToActPeriod (Host, Socket, Ch, ActToActSave);
  }

  //
  // Train each Dimm
  //
  SetTrainingModeDdr5 (Socket, TmRxDqDqs);
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    //
    // Train each Rank
    //
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      AllChPassDqsOffset = 0;
      ZeroMem ((UINT8 *)&AllStrobePassDqsOffset, sizeof (AllStrobePassDqsOffset));
      //
      // RxVrefLoop = 0 : default Rx VREF
      // RxVrefLoop = 1 : default Rx VREF - 5 ticks
      // RxVrefLoop = 2 : default Rx VREF + 5 ticks
      //
      SaveIoLatAndReceiveEnableDelay (Host, Socket, Dimm, Rank, IoLatencySubCh, ReceiveEnableDelay);

      for (RxVrefLoop = 0; RxVrefLoop < (sizeof (RxVrefOffset) / sizeof (RxVrefOffset[0])); RxVrefLoop++) {
        RxStrobeInvertLoop = 0;
        ZeroMem ((UINT8 *) &CheckForPassingCondition, sizeof (CheckForPassingCondition));

        if (AllChPassDqsOffset == 1) {
          break;
        }

        //
        // Modify Rx VREF for current sample
        //
        RcDebugPrint (SDBG_MAX, "Setting new RX VREF\n");
        OffsetRxVref (Host, Socket, Dimm, Rank, RxVrefOffset[RxVrefLoop]);

        if (RxVrefLoop != 0) {
          RestoreIoLatAndReceiveEnableDelay (Host, Socket, Dimm, Rank, IoLatencySubCh, ReceiveEnableDelay, AllStrobePassDqsOffset);
        }
        RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
          "RX VREF Loop %2d\n", RxVrefLoop);
        for (DramReadDqsOffset = 0; DramReadDqsOffset < MAX_DRAM_DQS_OFFSET; DramReadDqsOffset++) {
          //
          //Rx Strobe Invert Loop.
          //
          for (RxStrobeInvertValue = 0; RxStrobeInvertValue < 2; RxStrobeInvertValue++) {
            //
            // Initialize to no ranks present
            //
            RankPresent = 0;

            for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
              RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);
              if (IsChannelEnabled (Socket, Ch) == FALSE) {
                continue;
              }

              RankList = GetRankNvList (Host, Socket, Ch, Dimm);
              DimmNvList = GetDimmNvList (Host, Socket, Ch);
              if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
                continue;
              }

              if (FeaturePcdGet (PcdCteBuild) == TRUE) {
                //
                // Init DRAM interface only for CTE.
                //
                InitDramInterface (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank);
              }
              //
              // Inidicate this Rank is present on at least one channel
              //
              RankPresent |= (*RankList) [Rank].enabled;

              //
              // Tell the iMC not to issue activates and precharges
              //
              SetMprModeFilterActPre (Host, Socket, Ch, 1);
              ConvertRdToMrr (Host, Socket, Ch, 1);
            } // Ch loop

            //
            // Go to the next Rank if this Rank is not present on any channels
            //
            if (!RankPresent) {
              continue;
            }

            //
            // Program read dqs offset timing.
            //
            if (!((RxVrefLoop == 0) && (DramReadDqsOffset == 0))) {
              ProgramReadDqsOffsetTiming (Host, Dimm, Rank, DramReadDqsOffset, AllStrobePassDqsOffset, ResultPass, IoLatencySubCh);
            }

            //
            // Program RxStrobeInvert
            //
            if (!((RxStrobeInvertLoop == 0) && (((RxVrefLoop == 0) && (DramReadDqsOffset == 0))))) {
              ProgramRxStrobeInvert  (Host, Dimm, Rank, AllStrobePassDqsOffset, ResultPass, RxStrobeInvertLoop);
            }

            //
            // DRAM Pattern init.
            //
            CoarseReadDqDqsDramPatternInit (Host, Dimm, Rank);

            //
            // Print DramReadDqsOffset and RxStrobeInvertLoop info
            //
            RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
              "DramReadDqsOffset = %d, RxStrobeInvertLoop = %d\n", DramReadDqsOffset, RxStrobeInvertLoop);

            //
            // Data collecting
            //
            Status = FindPerBitResults (Host, Socket, Dimm, Rank, RxStrobeInvertLoop, AllStrobePassDqsOffset, ErrorResult);

            for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
              if (IsChannelEnabled (Socket, Ch) == FALSE) {
                continue;
              }
              SetMprModeFilterActPre (Host, Socket, Channel, 0);
              ConvertRdToMrr (Host, Socket, Channel, 0);

              RankList = GetRankNvList (Host, Socket, Ch, Dimm);
              RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);
              DimmNvList = GetDimmNvList (Host, Socket, Ch);
              if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
                continue;
              }
              for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
                if (AllStrobePassDqsOffset[Ch][SubCh] == 1) {
                  continue;
                }
                Channel = Ch;
                GetChannelMapping (Ch, SubCh, &Channel);

                SetRankMPR (Host, Socket, Channel, Dimm, Rank, 0);
#ifdef LRDIMM_SUPPORT
                (*RankStruct) [Rank].CurrentDramMode = 0;
#endif
                WorstLeft = 0xFF;
                WorstRight = 0xFF;
                AllBitsPass = TRUE;
                for (Bit = 0; Bit < MBV; Bit++) {
                  EvenOddPass = TRUE;
                  CheckCurrentSweep = TRUE;

                  for (Evenodd = 0; Evenodd < 2; Evenodd++) {

                    Nibble = Bit / 4;
                    EncodedNibble = GetStrobeFromNibble (Host, Nibble);
                    //
                    // Skip if this is an ECC Strobe when ECC is disabled
                    //
                    if (IsBitNotValidSubChDdr5 (Host, Bit)) {
                      continue;
                    }

                    //
                    // Copy the bit-packed error data into an array for FindErrorGraphMin
                    //
                    for (Index = 0; Index < READ_DQDQS_DDR5_LENGTH; Index++) {
                      if (((*ErrorResult)[Ch][SubCh][Evenodd].BitErrorResult[Index][Bit / MAX_BITS_IN_BYTE] & (BIT0 << (Bit % MAX_BITS_IN_BYTE))) == 0) {
                        (*ErrorSamples)[Index] = 0;
                      } else {
                        (*ErrorSamples)[Index] = 1;
                      }
                    }

                    Status = FindErrorGraphMin (Host, Socket, Ch, Dimm, Rank, 0, &(*ErrorSamples)[0],
                               READ_DQDQS_DDR5_LENGTH, &ErrorStart[Ch].PiPosition[Bit],
                               &ErrorEnd[Ch].PiPosition[Bit], &PiPosition[Ch][SubCh][Evenodd].PiPosition[Bit]);

                    // If there was an error finding edges, do not check current sweep for possible passing conditions
                    if (Status == FAILURE) {
                      CheckCurrentSweep = FALSE;
                    }

                    //LE and RE Margins
                    ResultsBit[SubCh][Evenodd].bits[Ch][Bit].n = ErrorEnd[Ch].PiPosition[Bit] + RXDQS_SWEEP_START;
                    ResultsBit[SubCh][Evenodd].bits[Ch][Bit].p = ErrorStart[Ch].PiPosition[Bit] + RXDQS_SWEEP_START;
                    PiPosition[Ch][SubCh][Evenodd].PiPosition[Bit] = PiPosition[Ch][SubCh][Evenodd].PiPosition[Bit] + RXDQS_SWEEP_START;

                    //
                    // Go to next loop if no margin or can't find the eye
                    //
                    if ((ResultsBit[SubCh][Evenodd].bits[Ch][Bit].n == RXDQS_SWEEP_START) ||
                        (ResultsBit[SubCh][Evenodd].bits[Ch][Bit].p == (RXDQS_SWEEP_END - 1)) ||
                        (Status == FAILURE)) {
                      EvenOddPass = FALSE;
                      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, EncodedNibble, Bit,
                        "Training failed with LE %3d RE %3d RxStrobe Invert %2d DRAM DQS Offset %2d RX VREF Loop %2d\n",
                        ResultsBit[SubCh][Evenodd].bits[Ch][Bit].n,
                        ResultsBit[SubCh][Evenodd].bits[Ch][Bit].p,
                        RxStrobeInvertValue, DramReadDqsOffset,
                        RxVrefLoop);
                    }

                    // A passing condition is when the previous sweep found a left edge greater or equal to 0 and a right edge less than the
                    // max of 175 and the current sweep finds a left edge greater than 0 and a right edge less then or equal to the max of 175.
                    if ((EvenOddPass == FALSE) && (CheckForPassingCondition[Ch][SubCh][Evenodd][Bit] == TRUE) && (CheckCurrentSweep == TRUE)) {
                      if ((ResultsBit[SubCh][Evenodd].bits[Ch][Bit].n > RXDQS_SWEEP_START) && (ResultsBit[SubCh][Evenodd].bits[Ch][Bit].p <= (RXDQS_SWEEP_END - 1))) {
                        EvenOddPass = TRUE;
                        RcDebugPrint (SDBG_MAX, "Found Passing condition for bit: %d - Previous sweep LE >= 0, RE < 175, Current sweep LE: %d, RE: %d\n",
                          Bit, ResultsBit[SubCh][Evenodd].bits[Ch][Bit].n, ResultsBit[SubCh][Evenodd].bits[Ch][Bit].p);
                      }
                    }

                    // The passing condition for this bit failed on the current sweep, so reset the CheckForPassingCondtion variable back to false.
                    CheckForPassingCondition[Ch][SubCh][Evenodd][Bit] = FALSE;

                    // If there is a failure b/c the left edge is at the min (0), check previous sweep to check for a passing condition.
                    if ((EvenOddPass == FALSE) && ((ResultsBit[SubCh][Evenodd].bits[Ch][Bit].n >= RXDQS_SWEEP_START) &&
                         (ResultsBit[SubCh][Evenodd].bits[Ch][Bit].p < (RXDQS_SWEEP_END - 1))) && (CheckCurrentSweep == TRUE)) {
                      CheckForPassingCondition[Ch][SubCh][Evenodd][Bit] = TRUE;
                    }

                    //
                    // Go to next loop if center is not within the range: When implementing the extended range, the bit centers need to be check such
                    // that they are not below 24 + GB or above 151 - GB. This is because 24 ticks of range need to be allocated for periodic retraining.
                    // Initially, GB should be set to 8 ticks
                    //
                    if ((((ResultsBit[SubCh][Evenodd].bits[Ch][Bit].n + ResultsBit[SubCh][Evenodd].bits[Ch][Bit].p) / 2) < MIN_RCOMP_RXDQS + RXDQS_GUARDBAND) ||
                        (((ResultsBit[SubCh][Evenodd].bits[Ch][Bit].n + ResultsBit[SubCh][Evenodd].bits[Ch][Bit].p) / 2) > MAX_RCOMP_RXDQS - RXDQS_GUARDBAND)) {
                      EvenOddPass = FALSE;
                      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, EncodedNibble, Bit,
                        "Training failure: hit GuardBand with LE %3d RE %3d RxStrobe Invert %2d DRAM DQS Offset %2d RX VREF Loop %2d\n",
                        ResultsBit[SubCh][Evenodd].bits[Ch][Bit].n,
                        ResultsBit[SubCh][Evenodd].bits[Ch][Bit].p,
                        RxStrobeInvertValue, DramReadDqsOffset, RxVrefLoop);
                    }

                    if ((PiPosition[Ch][SubCh][Evenodd].PiPosition[Bit] - ErrorEnd[Ch].PiPosition[Bit]) < WorstLeft) {
                      WorstLeft = PiPosition[Ch][SubCh][Evenodd].PiPosition[Bit] - ErrorEnd[Ch].PiPosition[Bit];
                    }
                    if ((ErrorStart[Ch].PiPosition[Bit] - PiPosition[Ch][SubCh][Evenodd].PiPosition[Bit]) < WorstRight) {
                      WorstRight = ErrorStart[Ch].PiPosition[Bit] - PiPosition[Ch][SubCh][Evenodd].PiPosition[Bit];
                    }
                  } // EvenOdd loop

                  if (EvenOddPass == FALSE) {
                    (*ResultPass) [Ch][SubCh][Dimm][Rank][EncodedNibble] = 0;
                    AllBitsPass = FALSE;
                    ResultPassBit[SubCh][Bit] = 0;
                  } else {
                    ResultPassBit[SubCh][Bit] = 1;
                  }
                }//Bit loop

                //
                // Loop for each Nibble/Strobe.
                //
                for (Nibble = 0; Nibble < MSVx4; Nibble++) {
                  //
                  // Skip if this is an ECC Nibble when ECC is disabled
                  //
                  if (IsNibbleNotValid (Host, Nibble)) {
                    continue;
                  }
                  EncodedNibble = GetStrobeFromNibble (Host, Nibble);
                  if ((ResultPassBit[SubCh][ (Nibble * BITS_PER_NIBBLE) + 0] == 1) &&
                      (ResultPassBit[SubCh][ (Nibble * BITS_PER_NIBBLE) + 1] == 1) &&
                      (ResultPassBit[SubCh][ (Nibble * BITS_PER_NIBBLE) + 2] == 1) &&
                      (ResultPassBit[SubCh][ (Nibble * BITS_PER_NIBBLE) + 3] == 1)) {

                    (*ResultPass) [Ch][SubCh][Dimm][Rank][EncodedNibble] = 1;
                    (*TrainingResultPi) [Ch][SubCh][Bit][EVEN_UI].PiCenter = PiPosition[Ch][SubCh][EVEN_UI].PiPosition[Bit];
                    (*TrainingResultPi) [Ch][SubCh][Bit][ODD_UI].PiCenter = PiPosition[Ch][SubCh][ODD_UI].PiPosition[Bit];
                    TrainingResultRxVref[Ch][SubCh][Bit].RxVref = RxVrefLoop;

                    TrainingResultStrobe [Ch][SubCh][EncodedNibble].RxStrobeInvert = RxStrobeInvertValue;
                    TrainingResultStrobe [Ch][SubCh][EncodedNibble].DramDqsOffset = DramReadDqsOffset;
                  } else {
                    (*ResultPass) [Ch][SubCh][Dimm][Rank][EncodedNibble] = 0;
                  }
                } //Nibble loop
              } // subch loop
            } // ch loop

            AllChPassDqsOffsetFlag = 1;
            for (Ch = 0; Ch < MaxChDdr; Ch++) {
              if (IsChannelEnabled (Socket, Ch) == FALSE) {
                continue;
              }
              RankList = GetRankNvList (Host, Socket, Ch, Dimm);
              RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);
              DimmNvList = GetDimmNvList (Host, Socket, Ch);
              if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
                continue;
              }
              for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
                if (AllStrobePassDqsOffset[Ch][SubCh] == 1) {
                  continue;
                }
                Channel = Ch;
                GetChannelMapping (Ch, SubCh, &Channel);
                AllStrobePassDqsOffsetFlag = 1;
                for (Bit = 0; Bit < MBV; Bit++) {

                  Nibble = Bit / 4;
                  EncodedNibble = GetStrobeFromNibble (Host, Nibble);
                  //
                  // Skip if this is an ECC Strobe when ECC is disabled
                  //
                  if (IsBitNotValid (Host, Bit)) {
                    continue;
                  }
                  if ((*ResultPass) [Ch][SubCh][Dimm][Rank][EncodedNibble] == 0) {
                    AllStrobePassDqsOffsetFlag = 0;
                    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, EncodedNibble, Bit,
                      "AllStrobePassDqsOffset is 0 and RxStrobeInvert is %d\n", RxStrobeInvertValue);
                    break;
                  }
                } // Bit loop
                //} //Evenodd loop
                if (AllStrobePassDqsOffsetFlag == 1) {
                  AllStrobePassDqsOffset[Ch][SubCh] = 1;
                } else {
                  AllChPassDqsOffsetFlag = 0;
                }
              }//subch loop
            } // Ch loop
            if (AllChPassDqsOffsetFlag == 1) {
              AllChPassDqsOffset = 1;
            }

            if (AllChPassDqsOffset == 1) {
              RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                "AllChPassDqsOffset is 1 and RxStrobeInvert is %d\n", RxStrobeInvertValue);
              break;
            }
            RxStrobeInvertLoop = RxStrobeInvertLoop + 1;
          } //RxStrobeInvert

          if (AllChPassDqsOffset != 1) {
            RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
              "AllChPassDqsOffset is 0 and DramReadDqsOffset is %d\n", DramReadDqsOffset);
            continue;
          }

          for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
            if (IsChannelEnabled (Socket, Ch) == FALSE) {
              continue;
            }
            SetMprModeFilterActPre (Host, Socket, Channel, 0);
            ConvertRdToMrr (Host, Socket, Channel, 0);

            RankList = GetRankNvList (Host, Socket, Ch, Dimm);
            RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);
            DimmNvList = GetDimmNvList (Host, Socket, Ch);
            if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
              continue;
            }
            for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
              Channel = Ch;
              GetChannelMapping (Ch, SubCh, &Channel);

              for (Evenodd = 0; Evenodd < 2; Evenodd++) {

                if (Evenodd == 0) {
                  RcDebugPrint (SDBG_MAX, "\nRxDqsP: \n");
                  GroupStrobe = RxDqsPDelay;
                  GroupBit = RxDqsPBitDelay;
                } else {
                  RcDebugPrint (SDBG_MAX, "\nRxDqsN: \n");
                  GroupStrobe = RxDqsNDelay;
                  GroupBit = RxDqsNBitDelay;
                }
#ifdef DEBUG_CODE_BLOCK
                //
                //Print LE and RE Per Bit Deskew Margins for current Rank
                //
                if (GetDebugLevel () & SDBG_MAX) {
                  DisplayREandLEMarginsDdr5 (Host, Socket, Ch, SubCh, Dimm, Rank, &ResultsBit[SubCh][Evenodd], RxDqsDelay);
                }
#endif // DEBUG_CODE_BLOCK
                DetectLowMargin (Host, Ch, Dimm, Rank, &ResultsBit[SubCh][Evenodd], RxDqsDelay, NON_PER_BIT);

                RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
                  "Dimm = %d, Rank = %d\n", Dimm, Rank);
                RcDebugPrint (SDBG_MAX,
                  "         Per Bit Margin Center       Per Bit Skew\n");
                RcDebugPrint (SDBG_MAX,
                  "N# = Nibble, BCx = Bit Center, BSx = Bit Skew , Msl = Most Skewed Lane\n\n");
                RcDebugPrint (SDBG_MAX,
                  "N# BC0 BC1 BC2 BC3   NPi   BS0 BS1 BS2 BS3  Msl\n");
                //
                // Loop for each Nibble/Strobe.
                //
                for (Nibble = 0; Nibble < MSVx4; Nibble++) {

                  //
                  // Skip if this is an ECC Nibble when ECC is disabled
                  //
                  if (IsNibbleNotValid (Host, Nibble)) {
                    continue;
                  }
                  EncodedNibble = GetStrobeFromNibble (Host, Nibble);
                  //
                  // Set to largest possible
                  //
                  BitNibbleMin = 127;
                  RcDebugPrint (SDBG_MAX,
                    "%2d ", Nibble);
                  //
                  // Loop for all the bits in this Nibble
                  //
                  for (Bit = 0; Bit < BITS_PER_NIBBLE; Bit++) {
                    //
                    // Get the absolute Bit index (0-72)
                    //
                    BitIdx = (Nibble * 4) + Bit;
                    //
                    // Find the center for this Bit
                    //
                    BitCenter[Bit] = PiPosition[Ch][SubCh][Evenodd].PiPosition[BitIdx];

                    RcDebugPrint (SDBG_MAX,
                      "%3d ", BitCenter[Bit]);
                    //
                    // Check if the center of this Bit is smaller than the smallest found so far
                    //
                    if (BitCenter[Bit] < BitNibbleMin) {
                      BitNibbleMin = BitCenter[Bit];
                    }
                  } // for (Bit)

                  RcDebugPrint (SDBG_MAX,
                    "  %3d   ", BitNibbleMin);
                  GetSetDataGroup (Host, Socket, Channel, SubCh, Dimm, Rank, EncodedNibble, 0, DdrLevel, GroupStrobe, GSM_UPDATE_CACHE, &BitNibbleMin);
                  //
                  // Loop for all the bits in this Nibble
                  //
                  BitSkewMax = 0;
                  Msl = 0;
                  for (Bit = 0; Bit < BITS_PER_NIBBLE; Bit++) {
                    BitSkew[Bit] = BitCenter[Bit] - BitNibbleMin;
                    if (BitSkew[Bit] > BitSkewMax) {
                      BitSkewMax = BitSkew[Bit];
                      Msl = Bit;
                    }
                    RcDebugPrint (SDBG_MAX,
                      "%3d ", BitSkew[Bit]);

                  } // for (Bit)
                  GetSetDataGroup (Host, Socket, Channel, SubCh, Dimm, Rank, EncodedNibble, ALL_BITS, DdrLevel, GroupBit, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET,
                    BitSkew);
                  RcDebugPrint (SDBG_MAX, " %3d\n", Msl);
                } // for Nibble
              } //Evenodd loop
            }//subch loop
          } // Ch loop

          if (AllChPassDqsOffset == 1) {
            RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
              "AllChPassDqsOffset is 0 and DramReadDqsOffset is %d\n", DramReadDqsOffset);
            break;
          }
        } //DramReadDqsOffset loop

        //
        // Restore original Rx VREF
        //
        RcDebugPrint (SDBG_MAX, "B. Restoring RX VREF\n");
        OffsetRxVref (Host, Socket, Dimm, Rank, -RxVrefOffset[RxVrefLoop]);

        if (AllChPassDqsOffset == 1) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
            "AllChPassDqsOffset is 0 and DramReadDqsOffset is %d\n", DramReadDqsOffset);
          break;
        }
      }//RxVrefLoop loop
    } // Rank loop
  } // Dimm loop

  //
  // Setting CoarseRdDqDqsTrainingDone Indicates MRR in-band (CPGC) transaction is available
  // Otherwise, MRR uses out-band (SMB) transaction
  //
  SetCoarseRdDqDqsTrainingDone (Socket, TRUE);
  ClearDatabufferTrainingMode(Host, Socket);

  SetTrainingModeDdr5 (Socket, TmRxDqDqsCoarseExit);
  SetAepTrainingMode (Host,Socket, DISABLE_TRAINING_STEP);
  FifoTrainReset (Host, Socket);
#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    DisplayDramReadDqsOffset (Socket);
    DisplayResultsDdr5 (Host, Socket, RecEnDelay);
    DisplayResultsDdr5 (Host, Socket, RxCoarseDelay);
    DisplayResultsDdr5 (Host, Socket, RxDqsPDelay);
    DisplayResultsDdr5 (Host, Socket, RxDqsNDelay);
    DisplayResultsDdr5 (Host, Socket, RxDqsBitDelay);
    DisplayResultsDdr5 (Host, Socket, RxDqsNBitDelay);
    DisplayResultsDdr5 (Host, Socket, RxDqsPBitDelay);
    DisplayResultsDdr5 (Host, Socket, RxVref);
    DisplayResultsDdr5 (Host, Socket, RxStrobeInvert);
    RcDebugPrint (SDBG_MEM_TRAIN,"\n");
  }
#endif // DEBUG_CODE_BLOCK

  RcFreePool (ErrorResult);
  RcFreePool (TrainingResultPi);
  RcFreePool (ResultPass);
  RcFreePool (ErrorSamples);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    RestoreActToActPeriod (Socket, Ch, ActToActSave);
  }


  return Status;

} // CoarseReadDqDqsPerBit

/**

  Take DDRT2 Out of Training Mode
  @param[in] Host - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Number
  @param[in] Channel - Channel Number

  @retval N/A

**/
VOID
ClearDatabufferTrainingMode (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  UINT8                         MaxChDdr;
  UINT8                         Ch;
  UINT8                         Dimm;

  MaxChDdr = GetMaxChDdr ();
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

      if (IsDcpmmPresentDimm (Socket, Ch, Dimm) == TRUE) {
        DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, 0, DDR5_DATA_BUFFER_RW83_REG, LRDIMM_EXIT_TRAINING);
      }
    }
  }
}

/**
  Run the per Bit read dqdqs test for BRS.
  @param Host              - Pointer to sysHost
  @param Socket            - Socket number
  @param Dimm              - DIMM number
  @param Rank              - Rank number
  @param ErrorResult       - include the error rwsult for this channel

  @retval Status

**/
STATIC
UINT32
FindPerBitResultsBrs (
  IN  PSYSHOST      Host,
  IN  UINT8         Socket,
  IN  UINT8         Dimm,
  IN  UINT8         Rank,
  OUT BErrorResult  (*ErrorResult) [MAX_CH][SUB_CH]
  )
{
  UINT8               Ch;
  UINT32              ChBitmask;
  UINT8               Bit;
  UINT16              PiDelay;
  UINT8               SubCh;
  UINT8               ChannelLocal = 0;
  UINT8               PiIndex;
  UINT32              ErrResult;
  UINT8               ChStatus;
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];
  UINT8               DumArr[7] = {1,1,1,1,1,1,1};
  UINT32              BwSerr[MAX_CH][SUB_CH][3];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  UINT8 MBV = GetMaxBitsValid (Host);

  ChBitmask = GetChBitmask (Host, Socket, Dimm, Rank);
  ErrResult = 0;
  ChStatus = 0;
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
      continue;
    }

    //
    // Clear Dimm error results
    //
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      ChannelLocal = Ch;
      GetChannelMapping (Ch, SubCh, &ChannelLocal);
      SelectCPGCRanks (Host, Socket, ChannelLocal, 1 << GetLogicalRank (Host, Socket, ChannelLocal, Dimm, Rank), 0, 0);

      for (Bit = 0; Bit < MBV; Bit++) {
        for (PiDelay = 0; PiDelay < MEM_CHIP_POLICY_VALUE (Host, MaxPhaseInReadAdjustmentDq); PiDelay++) {
          (*ErrorResult) [Ch][SubCh].BitErrorResult[Bit][PiDelay] = 0;
        } //piDelay loop
      } // Bit loop
    } //SubCh loop
  } // Ch loop
  //
  // Setup the CPGC sequence for the MR31 command.
  //
  SetupReadDqDqsTestDdr5 (Host, Socket, ChBitmask, Rank);
  if (Host->DdrioUltSupport) {
    //
    // LoopCnt = 2
    // NunCL = 32;
    //
    CteSetupPattern ("rddqdqs", (1) | (32 << 4), ChBitmask, 3, GetPhyRank (Dimm, Rank));
  }

  SetCpgcPatControl (Host, Socket, ChBitmask, BTBUFFER, BTBUFFER, BTBUFFER, DDR_CPGC_PATBUF_MUX0, DDR_CPGC_PATBUF_MUX1, DDR_CPGC_PATBUF_MUX2);//BRS_todo. ChBitmask = Ddr5 channel bit mask

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "RxDqDqs DDR5 Pi Scanning...\n");
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "     PI |              Sub-channel A                   |               Sub-channel B                  |\n");
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "DQ:     | 0      7 8     15 16    23 24    31 32    39 | 40    47 48    55 56    63 64    69 70    79 |\n");

  for (PiDelay = 0; PiDelay < MEM_CHIP_POLICY_VALUE (Host, MaxPhaseInReadAdjustmentDq); PiDelay += RD_DQS_STEP_SIZE_DDR5) {

    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }

      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
        continue;
      }

      //
      // Change the RX Dq Dqs PI Setting
      //
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        ChannelLocal = Ch;
        GetChannelMapping (Ch, SubCh, &ChannelLocal);
        GetSetDataGroup (Host, Socket, ChannelLocal, NO_SUBCH, Dimm, Rank, ALL_STROBES, 0, DdrLevel, RxDqDqsDelay, GSM_UPDATE_CACHE, &PiDelay);
      }

    } // Ch loop

    IO_Reset (Host, Socket);

    //
    // Run the test
    //
    if (Host->DdrioUltSupport) {
      CteStartPattern (ChBitmask, 0x3, GetPhyRank (Dimm, Rank));
      CteWaitPatternDone ();
    } else {
      RunIOTest (Host, Socket, ChBitmask, BasicVA, DumArr, 1, 0);
    }
    //
    // Check the results
    //
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }

      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
        continue;
      }
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        ChStatus = 1 << Ch;
        BwSerr[Ch][SubCh][0] = 0;
        BwSerr[Ch][SubCh][1] = 0;
        BwSerr[Ch][SubCh][2] = 0;
        //
        // Collect test results
        //
        CollectTestResultsDdr5 (Host, Socket, ChStatus, BwSerr);
#ifdef DEBUG_CODE_BLOCK
        if (GetDebugLevel () & SDBG_MAX) {
          DisplayBwSerrDdr5 (Host, Socket, Ch, SubCh, Dimm, Rank, BwSerr[Ch][SubCh], PiDelay);
        }
#endif // DEBUG_CODE_BLOCK
        for (Bit = 0; Bit < MBV; Bit++) {
          if (IsBitNotValid (Host, Bit)) {
            continue;
          }
          if (Bit < 32) {
            ErrResult = BwSerr[Ch][SubCh][0];
          } else if (Bit < 64) {
            ErrResult = BwSerr[Ch][SubCh][1];
          } else  {
            ErrResult = BwSerr[Ch][SubCh][2];
          }


          for (PiIndex = 0; PiIndex < RD_DQS_STEP_SIZE_DDR5; PiIndex++) {
            //
            // Any failure is a failure for the Strobe
            // Assumption for SHL operation: if shift Bit above 32, operation will become << (Bit%32) by CPU instruction set
            //
            if (ErrResult & (1<<Bit)) {
              (*ErrorResult) [Ch][SubCh].BitErrorResult[Bit][ (PiDelay % (MAX_PHASE_IN_FINE_ADJUSTMENT * 2)) + PiIndex] = 1;
            } else {
              (*ErrorResult) [Ch][SubCh].BitErrorResult[Bit][ (PiDelay % (MAX_PHASE_IN_FINE_ADJUSTMENT * 2)) + PiIndex] = 0;
            }
          } // piIndex loop
        } // Bit loop
      } //subCh loop
    } // Ch loop

    IO_Reset (Host, Socket);
  } // piDelay loop

  return SUCCESS;
} // FindPerBitResultsBrs



/**
  Run the per Bit read dqdqs test.
  @param[in] Host              - Pointer to sysHost
  @param[in] Socket            - Socket number
  @param[in] Dimm              - DIMM number
  @param[in] Rank              - Rank number
  @param[in] ErrorResult       - include the error rwsult for this channel

  @retval Status

**/
STATIC
UINT32
FindPerBitResults (
  IN  PSYSHOST                  Host,
  IN  UINT8                     Socket,
  IN  UINT8                     Dimm,
  IN  UINT8                     Rank,
  IN  UINT8                     RxStrobeInvertLoop,
  IN  UINT8                     AllStrobePassDqsOffset[MAX_CH][SUB_CH],
  OUT BIT_ERROR_RESULTS_RD_DDR5 (*ErrorResult)[MAX_CH][SUB_CH][EVEN_ODD]
  )
{
  UINT8               Ch;
  UINT32              ChBitmask;
  UINT8               Bit;
  UINT16              PiDelay;
  UINT8               SubCh;
  UINT8               ChannelLocal = 0;
  UINT8               PiIndex;
  UINT32              ErrResult;
  UINT8               ChStatus;
  UINT8               Evenodd;
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT32              BwSerr[MAX_CH][SUB_CH][2][3];
  MRC_RT              MrcRankList;
  MRC_MST             MemSsType;
  UINT8               MaxChDdr;
  MRC_TT              CurrentTestType;

  CurrentTestType = GetCurrentTestType (Socket);
  MaxChDdr = GetMaxChDdr ();
  UINT8 MBV = MAX_BITS_DDR5 / 2;


  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  MrcRankList.Dimm = Dimm;
  MrcRankList.Rank = Rank;

  ChBitmask = GetChBitmask (Host, Socket, Dimm, Rank);
  ErrResult = 0;
  ChStatus = 0;
  ZeroMem ((UINT8 *) (&BwSerr), sizeof (BwSerr));
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
      continue;
    }

    //
    // Clear Dimm error results
    //
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      ChannelLocal = Ch;
      GetChannelMapping (Ch, SubCh, &ChannelLocal);
      SelectCPGCRanks (Host, Socket, ChannelLocal, 1 << GetLogicalRank (Host, Socket, Ch, Dimm, Rank), 0, 0);
    } //SubCh loop
  } // Ch loop

  ZeroMem ((UINT8 *) (*ErrorResult), sizeof (*ErrorResult));

  //
  // Setup the CPGC sequence for the MR31 command.
  //
  if (Host->DdrioUltSupport) {
    //
    // LoopCnt = 1;
    // NunCL = 32; Just for ULT to save the test time. By HAS, it needs to be 64.
    //
    CteSetupPattern ("rddqdqs", (1) | (32 << 4), ChBitmask, 3, GetPhyRank (Dimm, Rank));
  } else {
    SetupTest (Host, Socket, MemSsType, ChBitmask, 1, &MrcRankList, DdrLevel, CurrentTestType, gsmCsnDelim, 0, FIRST_ITERATION_LOOP);
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "RxDqDqs DDR5 Pi Scanning...\n");
  for (PiDelay = RXDQS_SWEEP_START; PiDelay < RXDQS_SWEEP_END; PiDelay += RD_DQS_STEP_SIZE_DDR5) {
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }

      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
        continue;
      }

      //
      // Change the RX Dq Dqs PI Setting for all strobes in the channel
      // GetSetRxDelay sets the offset for all 20 strobes in the channel, but results are tracked per subchannel
      //
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        if (AllStrobePassDqsOffset[Ch][SubCh] == 1) {
          continue;
        }
        GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, ALL_STROBES, 0, DdrLevel, RxDqsDelay, GSM_UPDATE_CACHE, &PiDelay);
        break;
      } // SubCh loop

    } // Ch loop
    IO_Reset (Host, Socket);

    //
    // Run the test
    //
    if (Host->DdrioUltSupport) {
      CteStartPattern (ChBitmask, 0x3, GetPhyRank (Dimm, Rank));
      CteWaitPatternDone ();
    } else {
      TestControl (Host, Socket, MemSsType, ChBitmask, NO_SUBCH, CurrentTestType, 1, START_TEST, NOT_STOP_TEST);
    }
    //
    // Check the results
    //
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }

      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
        continue;
      }
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        if (AllStrobePassDqsOffset[Ch][SubCh] == 1) {
          continue;
        }
        ChStatus = 1 << Ch;

        BwSerr[Ch][SubCh][0][0] = 0;
        BwSerr[Ch][SubCh][0][1] = 0;
        BwSerr[Ch][SubCh][0][2] = 0;
        BwSerr[Ch][SubCh][1][0] = 0;
        BwSerr[Ch][SubCh][1][1] = 0;
        BwSerr[Ch][SubCh][1][2] = 0;

        //
        // Collect test results
        //
        CollectTestResultsEvenOddDdr5 (Host, Socket, ChStatus, BwSerr);
        if (Host->DdrioUltSupport) {
          CteGetMcErrorCounter (1 << Ch, 1 << SubCh, &BwSerr[Ch][SubCh][0][0], &BwSerr[Ch][SubCh][0][2], &BwSerr[Ch][SubCh][1][0], &BwSerr[Ch][SubCh][1][2]);
        }
#ifdef DEBUG_CODE_BLOCK
        if (GetDebugLevel () & SDBG_MAX) {
          DisplayBwSerrEvenOddDdr5 (Host, Socket, Ch, SubCh, Dimm, Rank, BwSerr[Ch][SubCh], PiDelay, AllStrobePassDqsOffset);
        }
#endif // DEBUG_CODE_BLOCK
        for (Evenodd = 0; Evenodd < 2; Evenodd++) {
          for (Bit = 0; Bit < MBV; Bit++) {
            if (IsBitNotValid (Host, Bit)) {
              continue;
            }
            if (Bit < 32) {
              ErrResult = BwSerr[Ch][SubCh][Evenodd][0];
            } else  {
              ErrResult = BwSerr[Ch][SubCh][Evenodd][2];
            }


            for (PiIndex = 0; PiIndex < RD_DQS_STEP_SIZE_DDR5; PiIndex++) {
              //
              // Any failure is a failure for the Strobe
              //
              if (ErrResult & (BIT0 << (Bit % 32))) {
                (*ErrorResult)[Ch][SubCh][Evenodd].BitErrorResult[PiDelay + PiIndex][Bit / MAX_BITS_IN_BYTE] |= (BIT0 << (Bit % MAX_BITS_IN_BYTE));
              } else {
                (*ErrorResult)[Ch][SubCh][Evenodd].BitErrorResult[PiDelay + PiIndex][Bit / MAX_BITS_IN_BYTE] &= ~(BIT0 << (Bit % MAX_BITS_IN_BYTE));
              }
            } // piIndex loop
          } // Bit loop
        } //Evenodd loop
      } //subCh loop
    } // Ch loop

    IO_Reset (Host, Socket);
  } // piDelay loop
  return SUCCESS;
} // FindPerBitResults

/**

  Initialize the Rd DqDqs operation

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number

  @retval N/A

**/
VOID
RdDqDqsInitDdr5 (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  MRC_STATUS  Status;

  //
  // If Rx DFE is going to be trained, enable the circuit now so any timing
  // effects are taken into account during the Basic Host Rx DQ DQS sweep.
  //
  if (IsMemFlowEnabled (EarlyReadVrefCenteringTraining)) {
    Status = EnableDfe (Host, Socket, RxDfe);
    if (Status != MRC_STATUS_SUCCESS) {
      RcDebugPrintWithDevice (
        SDBG_ERROR, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Warning: failed to enable DDRIO DFE circuit before Basic Host Rx DQ DQS sweep!\n"
        );
    }
  }

  if (IsBrsPresent (Host, Socket)) {
    WriteTemporalPattern (Host, Socket, RD_DQ_DQS_PATTERN_DDR5, 4, FALSE);
  } else {
    WriteWDBDqLanePattern (Host, Socket, RD_DQ_DQS_PATTERN_DDR5, 1, FALSE);
  }
} // RdDqDqsInitDdr5

/**

  Program the Host Interface Read DQS Offset Timing on the data buffer.

  @param[in] Host           - Pointer to sysHost
  @param[in] Socket         - Socket number
  @param[in] Channel        - Channel number
  @param[in] SubChMask      - Mask of subchannel numbers to be programmed
  @param[in] Dimm           - DIMM number in channel
  @param[in] Rank           - Rank number on DIMM
  @param[in] ReadDqsOffset  - Read DQS Offset value to be programmed

  @retval N/A

**/
VOID
ProgramDbReadDqsOffset (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     SubChMask,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     ReadDqsOffset
  )
{
#ifdef LRDIMM_SUPPORT
  DDR5_DATA_BUFFER_RW8F_STRUCT  DataBufferRw8f;

  DataBufferRw8f.Data = 0;

#if USE_LATEST_DB_SPEC
  DataBufferRw8f.Bits.host_interface_dqs0_pre_launch = ReadDqsOffset;
  DataBufferRw8f.Bits.host_interface_dqs1_pre_launch = ReadDqsOffset;
#else
  DataBufferRw8f.Bits.host_interface_dqs_pre_launch = ReadDqsOffset;
#endif // USE_LATEST_DB_SPEC

  DbControlWordWrite (Host, Socket, Channel, SubChMask, Dimm, Rank, DDR5_DATA_BUFFER_RW8F_REG, DataBufferRw8f.Data);

#endif // LRDIMM_SUPPORT
}

/**

  Program Read DQS Offset Timing.

  @param[in] Host                   - Pointer to sysHost
  @param[in] Dimm                   - Dimm number
  @param[in] Rank                   - Rank number
  @param[in] ReadDqsOffset          - ReadDqsOffset value
  @param[in] AllStrobePassDqsOffset - Strobe Pass info
  @param[in] ResultPass             - Result info
  @param[in] IoLatencySubChOriginal - IO Latency info

  @retval Status

**/
UINT32
ProgramReadDqsOffsetTiming (
  IN PSYSHOST Host,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    ReadDqsOffset,
  IN UINT8    AllStrobePassDqsOffset[MAX_CH][SUB_CH],
  IN UINT8    (*ResultPass) [MAX_CH][SUB_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_STROBE_DDR5/2],
  IN UINT8    IoLatencySubChOriginal[MAX_CH][SUB_CH]
  )
{
  UINT8                             Socket;
  UINT8                             Ch;
  UINT8                             SubCh;
  UINT8                             LogRank;
  UINT8                             IoLatency;
  UINT8                             MaxChDdr;
  DDR5_MODE_REGISTER_40_STRUCT      Mr40;

  Socket = Host->var.mem.currentSocket;
  MaxChDdr = GetMaxChDdr ();
  RcDebugPrint (SDBG_MEM_TRAIN,
    "ProgramReadDqsOffsetTiming start\n");

  //
  // Initialize structure to write during the loop.
  //
  Mr40.Data = 0;
  Mr40.Bits.read_dqs_offset_timing = ReadDqsOffset;

  //
  // Initialize to no ranks present
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      if (AllStrobePassDqsOffset[Ch][SubCh] == 1) {
        continue;
      }

      //
      // Get the logical Rank #
      //
      LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
      //
      // Program IO latency before moving DQS
      // Each time, call this function, the DRAM DQS offset/Iolatency are increased by 1
      //
      IoLatency = IoLatencySubChOriginal[Ch][SubCh];
      IoLatency = IoLatency + ReadDqsOffset;
      SetIoLatencyDdr5 (Socket, Ch, SubCh, LogRank, IoLatency);
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT,
        "IoLatency %d  \n", IoLatency);

      //
      // Program the read dqs offset timing.
      //
      if (IsLrdimmPresent (Socket, Ch, Dimm) == 0) {
        DramModeRegWrite (Socket, Ch, (1 << SubCh), Dimm, Rank, DDR5_MODE_REGISTER_40_REG, Mr40.Data);
      } else {
        ProgramDbReadDqsOffset (Host, Socket, Ch, (1 << SubCh), Dimm, Rank, ReadDqsOffset);
      }
    }
  }  // Ch loop
  return SUCCESS;
}


/**

  Program Rx Strobe Invert.

  @param[in] Host                    - Pointer to sysHost
  @param[in] Dimm                    - Dimm number
  @param[in] Rank                    - Rank number
  @param[in] AllStrobePassDqsOffset  - AllStrobePassDqsOffset value
  @param[in] ResultPass              - ResultPass value
  @param[in] RxStrobeInvertLoop      - RxStrobeInvertLoop loop number

  @retval Status

**/
VOID
ProgramRxStrobeInvert (
  IN PSYSHOST Host,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    AllStrobePassDqsOffset[MAX_CH][SUB_CH],
  IN UINT8    (*ResultPass) [MAX_CH][SUB_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_STROBE_DDR5/2],
  IN UINT8    RxStrobeInvertLoop
  )
{
  UINT8                             Socket;
  UINT8                             Ch;
  UINT8                             SubCh;
  UINT8                             MaxChDdr;
  UINT8                             Strobe = 0;
  UINT8                             SubChMSVx4;
  INT16                             ReceiveEnableOffset = 0;
  INT16                             RxStrobeInvertValue = 0;
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);


  Socket = Host->var.mem.currentSocket;
  MaxChDdr = GetMaxChDdr ();
  RcDebugPrint (SDBG_MEM_TRAIN,
    "ProgramRxStrobeInvert start\n");

  //
  // Initialize to no ranks present
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      if (AllStrobePassDqsOffset[Ch][SubCh] == 1) {
        continue;
      }

      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }

        if ((RxStrobeInvertLoop % 2) != 0) {
          //
          // Rx Strobe invert change from 0 to 1
          //
          RxStrobeInvertValue = 1;
          //
          // Program the Rx Strobe Invert Value
          //
          GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, Strobe, 0, DdrLevel, RxStrobeInvert, GSM_UPDATE_CACHE , &RxStrobeInvertValue);
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, Strobe, NO_BIT,
            "Program the Rx Strobe Invert Value %d \n", RxStrobeInvertValue);

          if (GetPreambleState (Socket, READ_PREAMBLE) != PREAMBLE_4TCLK) {
            ReceiveEnableOffset = -RECEIVE_ENABLE_OFFSET_32;
            GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, Strobe, 0, DdrLevel, RecEnDelay, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET,
              (INT16 *)&ReceiveEnableOffset);
          }
        } else {
          //
          // Rx Strobe invert change from 1 to 0
          //
          RxStrobeInvertValue = 0;
          //
          // Program the Rx Strobe Invert Value
          //
          GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, Strobe, 0, DdrLevel, RxStrobeInvert, GSM_UPDATE_CACHE , &RxStrobeInvertValue);

          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, Strobe, NO_BIT,
            "Program the Rx Strobe Invert Value %d \n", RxStrobeInvertValue);

          // Do not adjust Receive Enable offset for initial RxStrobeInvertLoop since Receive Enable values are restored in this case
          if ((RxStrobeInvertLoop != 0) && (GetPreambleState (Socket, READ_PREAMBLE) != PREAMBLE_4TCLK)) {
            ReceiveEnableOffset = RECEIVE_ENABLE_OFFSET_32;
            GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, Strobe, 0, DdrLevel, RecEnDelay, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET,
              (INT16 *)&ReceiveEnableOffset);
          }
        }
      } // Strobe loop
    }  // SubCh loop
  } // Ch loop
  return;
} //ProgramRxStrobeInvert


  /**

  Program Read DQS Offset Timing.

  @param[in] Host           - Pointer to sysHost
  @param[in] Socket         - Socket number
  @param[in] Ch             - Ch number
  @param[in] SubCh          - SubCh number
  @param[in] Dimm           - Dimm number
  @param[in] Rank           - Rank number
  @param[in] ReadDqsOffset  - ReadDqsOffset value

  @retval NA

  **/
VOID
ProgramReadDqsOffsetDirectly (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubCh,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    ReadDqsOffset
  )
{
  UINT8                             LogRank;
  UINT8                             IoLatency;
  UINT8                             MaxChDdr;
  DDR5_MODE_REGISTER_40_STRUCT      Mr40;

  MaxChDdr = GetMaxChDdr ();
  RcDebugPrint (SDBG_MEM_TRAIN,
    "ProgramReadDqsOffsetDirectly start\n");

  //
  // Skip if input offset is zero.
  //
  if (ReadDqsOffset == 0) {
    return;
  }

  //
  // Initialize structure to write during the loop.
  //
  Mr40.Data = 0;
  Mr40.Bits.read_dqs_offset_timing = ReadDqsOffset;

  //
  // Initialize to no ranks present
  //
  //
  // Get the logical Rank #
  //
  LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
  //
  // Program IO latency before moving DQS
  //
  GetIoLatencyDdr5 (Socket, Ch, SubCh, LogRank, GSM_READ_ONLY, &IoLatency);
  //
  // Each time, call this function, the DRAM DQS offset/Iolatency are increased by 1
  //
  IoLatency++;
  SetIoLatencyDdr5 (Socket, Ch, SubCh, LogRank, IoLatency);
  //
  // Program the read dqs offset timing.
  //
  if (IsLrdimmPresent (Socket, Ch, Dimm) == 0) {
    DramModeRegWrite (Socket, Ch, 1 << SubCh, Dimm, Rank, DDR5_MODE_REGISTER_40_REG, Mr40.Data);
  } else {
    ProgramDbReadDqsOffset (Host, Socket, Ch, (1 << SubCh), Dimm, Rank, ReadDqsOffset);
  }
  return;
}

  /**

   Check the RxStrobeP and RxStrobeN value check

  @param[in] Host               - Pointer to sysHost
  @param[in] Socket             - Socket number
  @param[in] Ch                 - Ch number
  @param[in] SubCh              - SubCh number
  @param[in] Dimm               - Dimm number
  @param[in] Rank               - Rank number
  @param[in] Evenodd            - Even or Odd Strobe
  @param[in] BitNibbleMin       - Minimal Nibble Delay Value
  @param[in] RxStrobeInvert     - RxStrobe Invert Value
  @param[in] DramReadDqsOffset  - Dram Read Dqs Offset value
  @param[in] EncodedNibble      - Encoded Nibble Value

  @retval NA

  **/
VOID
RxStrobeResultCheck (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubCh,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Evenodd,
  IN INT16*   BitNibbleMin,
  IN UINT8    RxStrobeInvertArray[MAX_CH][SUB_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_STROBE_DDR5/2],
  IN UINT8*   DramReadDqsOffset,
  IN UINT8    EncodedNibble
  )
{
  INT16               RxStrobeValueOffset = 0;
  INT16               RxStrobeInvertValue = 0;
  if ((*BitNibbleMin) > MAX_RCOMP_RXDQS) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, EncodedNibble, NO_BIT,
      "BitNibbleMin %d is greater than 151 in %d type strobe\n", (*BitNibbleMin), Evenodd);
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return;
  }
  if (Evenodd == 0) {
    if ((*BitNibbleMin) < MIN_RCOMP_RXDQS) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, EncodedNibble, NO_BIT,
        "BitNibbleMin %d is less than 24 in %d type strobe\n", (*BitNibbleMin), Evenodd);

      //
      // Invert the RX strobe.
      //
      RxStrobeInvertArray[Ch][SubCh][Dimm][Rank][EncodedNibble] = 1;
      RxStrobeInvertValue = 1;
      //
      // Program the Rx Strobe Invert Value
      //
      GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedNibble, 0, DdrLevel, RxStrobeInvert, GSM_UPDATE_CACHE , &RxStrobeInvertValue);


      if (GetPreambleState (Socket, READ_PREAMBLE) >= PREAMBLE_2TCLK) {
        (*BitNibbleMin) = (*BitNibbleMin) + QCLKPITICKS;
      } else {
        (*BitNibbleMin) = (*BitNibbleMin) + QCLKPITICKS * 2;
        //
        // Increase the DRAM DQS offset by 1
        //
        (*DramReadDqsOffset) = (*DramReadDqsOffset) + 1;
        ProgramReadDqsOffsetDirectly (Host, Socket, Ch, SubCh, Dimm, Rank, (*DramReadDqsOffset));
      }
    }
  } else {
    if ((*BitNibbleMin) < MIN_RCOMP_RXDQS) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, EncodedNibble, NO_BIT,
        "BitNibbleMin %d is less than 24 in %d type strobe\n", (*BitNibbleMin), Evenodd);
      if (GetPreambleState (Socket, READ_PREAMBLE) >= PREAMBLE_2TCLK) {
        if (RxStrobeInvertArray[Ch][SubCh][Dimm][Rank][EncodedNibble] == 1) {
          (*BitNibbleMin) = (*BitNibbleMin) + QCLKPITICKS;
        } else {
          RxStrobeInvertArray[Ch][SubCh][Dimm][Rank][EncodedNibble] = 1;
          RxStrobeInvertValue = 1;
          //
          // Program the Rx Strobe Invert Value
          //
          GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedNibble, 0, DdrLevel, RxStrobeInvert, GSM_UPDATE_CACHE , &RxStrobeInvertValue);

          (*BitNibbleMin) = (*BitNibbleMin) + QCLKPITICKS;
          //
          // Update the value for RxStrobeP
          //
          RxStrobeValueOffset = QCLKPITICKS;
          GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedNibble, 0, DdrLevel, RxDqsPDelay, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET, &RxStrobeValueOffset);
        }
      } else {
        if (RxStrobeInvertArray[Ch][SubCh][Dimm][Rank][EncodedNibble] == 1) {
          (*BitNibbleMin) = (*BitNibbleMin) + QCLKPITICKS * 2;
        } else {
          RxStrobeInvertArray[Ch][SubCh][Dimm][Rank][EncodedNibble] = 1;
          RxStrobeInvertValue = 1;
          //
          // Program the Rx Strobe Invert Value
          //
          GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedNibble, 0, DdrLevel, RxStrobeInvert, GSM_UPDATE_CACHE , &RxStrobeInvertValue);

          (*BitNibbleMin) = (*BitNibbleMin) + QCLKPITICKS * 2;
          //
          // Update the value for RxStrobeP
          //
          RxStrobeValueOffset = QCLKPITICKS * 2;
          GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedNibble, 0, DdrLevel, RxDqsPDelay, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET, &RxStrobeValueOffset);

        }
        //
        // Increase the DRAM DQS offset by 1
        //
        DramReadDqsOffset = DramReadDqsOffset + 1;
        ProgramReadDqsOffsetDirectly (Host, Socket, Ch, SubCh, Dimm, Rank, (*DramReadDqsOffset));

      }
    }
  }

  return;
}

/**
  Display DRAM Read DQS Offset.

  @param[in]  Socket    Socket number.
**/
VOID
DisplayDramReadDqsOffset (
  IN  UINT8   Socket
  )
{
#ifdef DEBUG_CODE_BLOCK
  PSYSHOST                      Host;
  UINT8                         MaxChDdr;
  UINT8                         Channel;
  UINT8                         Dimm;
  UINT8                         SubChannel;
  UINT8                         Rank;
  UINT8                         Strobe;
  UINT8                         MaxStrobe;
  UINT8                         SubChMSVx4;
  UINT8                         SubChMSVx8;
  UINT8                         ReadDqsOffset[MAX_CH][MAX_DIMM][SUB_CH][MAX_RANK_DIMM][MAX_STROBE_DDR5 / 2];
  UINT32                        DbgLevel;
  DDR5_MODE_REGISTER_40_STRUCT  Mr40;
  DIMM_NVRAM_STRUCT             (*DimmNvList)[MAX_DIMM];

  //
  // Every print in this function should use the same level.
  //
  DbgLevel = SDBG_MEM_TRAIN;

  //
  // If this function can't print, return immediately.
  //
  if ((GetDebugLevel () & DbgLevel) == 0) {
    return;
  }

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  SubChMSVx8 = SubChMSVx4 / 2;

  //
  // Gather data up-front so any prints by these functions do not interrupt
  // the summary below.
  //
  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
        continue;
      }

      DimmNvList = GetDimmNvList (Host, Socket, Channel);

      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

        for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
          if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT) ||
              Rank >= (*DimmNvList)[Dimm].numRanks) {
            continue;
          }

          if (IsX4Dimm (Socket, Channel, Dimm) &&
              !IsDcpmmPresentDimm (Socket, Channel, Dimm)) {
            MaxStrobe = SubChMSVx4;
          } else {
            MaxStrobe = SubChMSVx8;
          }

          for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
            if (IsStrobeNotValidDdr5 (Host, Strobe)) {
              continue;
            }
            ReadDramModeRegCachePerRank (Socket, Channel, SubChannel, Dimm, Rank, DDR5_MODE_REGISTER_40_REG, &Mr40.Data);
            ReadDqsOffset[Channel][Dimm][SubChannel][Rank][Strobe] = Mr40.Bits.read_dqs_offset_timing;
          } // Strobe data loop

        } // Rank data loop
      } // SubChannel data loop
    } // Dimm data loop
  } // Channel data loop

  AcquirePrintControl ();
  RcDebugPrint (DbgLevel, "START_DATA_DRAM_READ_DQS_OFFSET\n");
  RcDebugPrint (DbgLevel, "DRAM:             ");

  //
  // Print a header row for strobe columns.
  //
  MaxStrobe = SubChMSVx4; // x4 is the larger value
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (DbgLevel, " %3d", Strobe);
  }
  RcDebugPrint (DbgLevel, "\n");

  //
  // Print the data.
  //
  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
        continue;
      }

      DimmNvList = GetDimmNvList (Host, Socket, Channel);

      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

        for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
          if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT) ||
              Rank >= (*DimmNvList)[Dimm].numRanks) {
            continue;
          }

          if (IsX4Dimm (Socket, Channel, Dimm) &&
              !IsDcpmmPresentDimm (Socket, Channel, Dimm)) {
            MaxStrobe = SubChMSVx4;
          } else {
            MaxStrobe = SubChMSVx8;
          }

          RcDebugPrintWithDevice (DbgLevel, Socket, Channel, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT, "");
          for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
            if (IsStrobeNotValidDdr5 (Host, Strobe)) {
              continue;
            }
            RcDebugPrint (DbgLevel, " %3d", ReadDqsOffset[Channel][Dimm][SubChannel][Rank][Strobe]);
          } // Strobe display loop
          RcDebugPrint (DbgLevel, "\n");

        } // Rank display loop
      } // SubChannel display loop
    } // Dimm display loop
  } // Channel display loop

  RcDebugPrint (DbgLevel, "STOP_DATA_DRAM_READ_DQS_OFFSET\n");
  ReleasePrintControl ();
#endif // #ifdef DEBUG_CODE_BLOCK
} // DisplayDramReadDqsOffset

  /**

  Save io latency, receive enable delay.

  @param[in] Host           - Pointer to sysHost
  @param[in] Dimm           - Dimm number
  @param[in] Rank           - Rank number
  @param[in] ReadDqsOffset  - ReadDqsOffset value

  @retval Status

  **/
UINT32
SaveIoLatAndReceiveEnableDelay (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8 IoLatencySubCh[MAX_CH][SUB_CH],
  IN UINT16 ReceiveEnableDelay[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{

  UINT8                             Ch;
  UINT8                             SubCh;
  UINT8                             LogRank;
  UINT8                             MaxChDdr;
  UINT8                              Strobe = 0;
  UINT8               SubChMSVx4;
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);

  MaxChDdr = GetMaxChDdr ();
  RcDebugPrint (SDBG_MEM_TRAIN,
    "SaveIoLatAndReceiveEnableDelay start\n");

  //
  // Initialize to no ranks present
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

      //
      // Get the logical Rank #
      //
      LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
      //
      // Program IO latency before moving DQS
      //
      GetIoLatencyDdr5 (Socket, Ch, SubCh, LogRank, GSM_READ_ONLY | GSM_READ_CSR, &IoLatencySubCh[Ch][SubCh]);


      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }

        GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, Strobe, 0, DdrLevel, RecEnDelay, GSM_READ_ONLY | GSM_READ_CSR,
          (INT16 *)&ReceiveEnableDelay[Ch][SubCh][Strobe]);
      }//Strobe loop
    }
  }  // Ch loop
  return SUCCESS;
}

  /**

  Restore io latency, receive enable delay.

  @param[in] Host                   - Pointer to sysHost
  @param[in] Socket                 - Socket number
  @param[in] Dimm                   - Dimm number
  @param[in] Rank                   - Rank number
  @param[in] IoLatencySubCh         - Saved IO Latency values
  @param[in] ReceiveEnableDelay     - Saved Receive Enable values
  @param[in] AllStrobePassDqsOffset - Indication of whether all strobes have passed

  @retval N/A

  **/
VOID
RestoreIoLatAndReceiveEnableDelay (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    IoLatencySubCh[MAX_CH][SUB_CH],
  IN UINT16   ReceiveEnableDelay[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN UINT8    AllStrobePassDqsOffset[MAX_CH][SUB_CH]
  )
{

  UINT8                             Ch;
  UINT8                             SubCh;
  UINT8                             LogRank;
  UINT8                             MaxChDdr;
  UINT8                              Strobe = 0;
  UINT8               SubChMSVx4;
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);

  MaxChDdr = GetMaxChDdr ();
  RcDebugPrint (SDBG_MEM_TRAIN,
    "RestoreIoLatAndReceiveEnableDelay start\n");

  //
  // Initialize to no ranks present
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      if (AllStrobePassDqsOffset[Ch][SubCh] == 1) {
        continue;
      }

      //
      // Get the logical Rank #
      //
      LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
      //
      // Program IO latency before moving DQS
      //
      SetIoLatencyDdr5 (Socket, Ch, SubCh, LogRank, IoLatencySubCh[Ch][SubCh]);


      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }

        GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, Strobe, 0, DdrLevel, RecEnDelay , GSM_UPDATE_CACHE,
          (INT16 *)&ReceiveEnableDelay[Ch][SubCh][Strobe]);
      }//Strobe loop
    }
  }  // Ch loop
}

/**

  Store the DQ De Swizzle Result

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Socket number
  @param[in] Ch           - Ch number
  @param[in] Dimm         - Dimm number
  @param[in] Rank         - Rank number
  @param[in] SubCh        - SubCh number
  @param[in] Bit          - Bit number

  @retval NA

**/
VOID
StoreDqDeSwizzleResult (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    SubCh,
  IN UINT8    Bit
  )
{
  UINT8             BitTemp = 0;
  UINT8             MaxBitsSubch = 40;
  struct ddrRank    (*RankList) [MAX_RANK_DIMM];

  RankList = GetRankNvList (Host, Socket, Ch, Dimm);

  if (Bit >= MaxBitsSubch) {
    RcDebugPrint (SDBG_MAX,
      "The input bit:%d is greater than expected\n", Bit);
    return;
  }

  if (IsLrdimmPresent (Socket, Ch, Dimm) == TRUE) {
    //
    // Translate LRDIMM swizzle info into DeDqSwzDdr5 variable, this is different from RDIMM,
    // For RDimm: DqSwzDdr5 store DRAM DQ to HOST DQ
    // For LRDIMM: DqSwzDdr5 contains HOST DQ to DRAM DQ
    //   - DeDqSwzDdr5 will be stored HOST to DRAM DQ connectivity by bit level, DeDqSwzDdr5[0] = 3 means HOST DQ0 and it connected to DQ3 of DRAM device
    //     and it will follow x4 strobe pair layout 0-3 to strobe 0 and 20-23 to strobe 1
    //
    BitTemp = (((*RankList) [Rank].DqSwzDdr5[SubCh][Bit / 2] >> ((Bit % 2) * 4)) & 3);
    if (Bit < BITS_PER_NIBBLE) {
      (*RankList) [Rank].DeDqSwzDdr5[SubCh][Bit] = BitTemp;
    } else if (Bit < (BITS_PER_NIBBLE * 2)) {
      (*RankList) [Rank].DeDqSwzDdr5[SubCh][Bit + (BITS_PER_NIBBLE * 1) ] = BitTemp;
    } else if (Bit < (BITS_PER_NIBBLE * 3)) {
      (*RankList) [Rank].DeDqSwzDdr5[SubCh][Bit + (BITS_PER_NIBBLE * 2) ] = BitTemp;
    } else if (Bit < (BITS_PER_NIBBLE * 4)) {
      (*RankList) [Rank].DeDqSwzDdr5[SubCh][Bit + (BITS_PER_NIBBLE * 3) ] = BitTemp;
    } else if (Bit < (BITS_PER_NIBBLE * 5)) {
      (*RankList) [Rank].DeDqSwzDdr5[SubCh][Bit + (BITS_PER_NIBBLE * 4) ] = BitTemp;
    } else if (Bit < (BITS_PER_NIBBLE * 6)) {
      (*RankList) [Rank].DeDqSwzDdr5[SubCh][Bit - (BITS_PER_NIBBLE * 4) ] = BitTemp;
    } else if (Bit < (BITS_PER_NIBBLE * 7)) {
      (*RankList) [Rank].DeDqSwzDdr5[SubCh][Bit - (BITS_PER_NIBBLE * 3) ] = BitTemp;
    } else if (Bit < (BITS_PER_NIBBLE * 8)) {
      (*RankList) [Rank].DeDqSwzDdr5[SubCh][Bit - (BITS_PER_NIBBLE * 2) ] = BitTemp;
    } else if (Bit < (BITS_PER_NIBBLE * 9)) {
      (*RankList) [Rank].DeDqSwzDdr5[SubCh][Bit - (BITS_PER_NIBBLE * 1) ] = BitTemp;
    } else if (Bit < (BITS_PER_NIBBLE * 10)) {
      (*RankList) [Rank].DeDqSwzDdr5[SubCh][Bit] = BitTemp;
    }
  } else {
    if (IsX4Dimm (Socket, Ch, Dimm)) {
      BitTemp = (((*RankList) [Rank].DqSwzDdr5[SubCh][Bit / 2] >> ((Bit % 2) * 4)) & 3);
      if (Bit < BITS_PER_NIBBLE) {
        (*RankList) [Rank].DeDqSwzDdr5[SubCh][BitTemp] = Bit;
      } else if (Bit < (BITS_PER_NIBBLE * 2)) {
        (*RankList) [Rank].DeDqSwzDdr5[SubCh][BitTemp + (BITS_PER_NIBBLE * 2) ] = Bit - BITS_PER_NIBBLE;
      } else if (Bit < (BITS_PER_NIBBLE * 3)) {
        (*RankList) [Rank].DeDqSwzDdr5[SubCh][BitTemp + (BITS_PER_NIBBLE * 4) ] = Bit - (BITS_PER_NIBBLE * 2);
      } else if (Bit < (BITS_PER_NIBBLE * 4)) {
        (*RankList) [Rank].DeDqSwzDdr5[SubCh][BitTemp + (BITS_PER_NIBBLE * 6) ] = Bit - (BITS_PER_NIBBLE * 3);
      } else if (Bit < (BITS_PER_NIBBLE * 5)) {
        (*RankList) [Rank].DeDqSwzDdr5[SubCh][BitTemp + (BITS_PER_NIBBLE * 8) ] = Bit - (BITS_PER_NIBBLE * 4);
      } else if (Bit < (BITS_PER_NIBBLE * 6)) {
        (*RankList) [Rank].DeDqSwzDdr5[SubCh][BitTemp + BITS_PER_NIBBLE] = Bit - (BITS_PER_NIBBLE * 5);
      } else if (Bit < (BITS_PER_NIBBLE * 7)) {
        (*RankList) [Rank].DeDqSwzDdr5[SubCh][BitTemp + (BITS_PER_NIBBLE * 3) ] = Bit - (BITS_PER_NIBBLE * 6);
      } else if (Bit < (BITS_PER_NIBBLE * 8)) {
        (*RankList) [Rank].DeDqSwzDdr5[SubCh][BitTemp + (BITS_PER_NIBBLE * 5) ] = Bit - (BITS_PER_NIBBLE * 7);
      } else if (Bit < (BITS_PER_NIBBLE * 9)) {
        (*RankList) [Rank].DeDqSwzDdr5[SubCh][BitTemp + (BITS_PER_NIBBLE * 7) ] = Bit - (BITS_PER_NIBBLE * 8);
      } else if (Bit < (BITS_PER_NIBBLE * 10)) {
        (*RankList) [Rank].DeDqSwzDdr5[SubCh][BitTemp + (BITS_PER_NIBBLE * 9) ] = Bit - (BITS_PER_NIBBLE * 9);
      }
    } else {
      BitTemp = (((*RankList) [Rank].DqSwzDdr5[SubCh][Bit / 2] >> ((Bit % 2) * 4)) & 7);
      if (Bit < MAX_BITS_IN_BYTE) {
        (*RankList) [Rank].DeDqSwzDdr5[SubCh][BitTemp] = Bit;
      } else if (Bit < (MAX_BITS_IN_BYTE * 2)) {
        (*RankList) [Rank].DeDqSwzDdr5[SubCh][BitTemp + MAX_BITS_IN_BYTE] = Bit - MAX_BITS_IN_BYTE;
      } else if (Bit < (MAX_BITS_IN_BYTE * 3)) {
        (*RankList) [Rank].DeDqSwzDdr5[SubCh][BitTemp + (MAX_BITS_IN_BYTE * 2) ] = Bit - (MAX_BITS_IN_BYTE * 2);
      } else if (Bit < (MAX_BITS_IN_BYTE * 4)) {
        (*RankList) [Rank].DeDqSwzDdr5[SubCh][BitTemp + (MAX_BITS_IN_BYTE * 3) ] = Bit - (MAX_BITS_IN_BYTE * 3);
      } else if (Bit < (MAX_BITS_IN_BYTE * 5)) {
        (*RankList) [Rank].DeDqSwzDdr5[SubCh][BitTemp + (MAX_BITS_IN_BYTE * 4) ] = Bit - (MAX_BITS_IN_BYTE * 4);
      }
    }
  } // IsLrdimmPresent
  return;
}
