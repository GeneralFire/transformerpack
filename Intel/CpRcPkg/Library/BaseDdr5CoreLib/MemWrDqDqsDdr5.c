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
#include <Memory/CpgcDefinitions.h>
#include <Library/RcMemAllocLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Memory/Ddr5RcwRegs.h>
#include <Library/SimulationServicesLib.h>

#define WR_DQ_DQS_PATTERN 0x1C711C71

#define WRITE_DQ_DQS_MAX_SAMPLE WR_DQ_PI_RANGE_DDR5

typedef struct {
  UINT8 BitErrorResult[WR_DQ_PI_RANGE_DDR5][MAX_BITS / 8];    ///< each UINT8 holds 8 bits of results.
} BIT_ERROR_RESULTS_WR_DDR5;

//
// Local function prototypes
//
/**

  This function does the pattern init for DDR5 write DQDQS training.

  @param[IN] Host                - Pointer to sysHost
  @param[IN] Socket              - Socket number

  @retval N/A

**/

VOID
WrDqDqsInitDdr5 (
  IN PSYSHOST           Host,
  IN UINT8              Socket
  );

/**

  This function collects all the data from DqDqs algorithm

  @param[IN] Host                - Pointer to sysHost
  @param[IN] Socket              - Socket number
  @param[IN] Dimm                - DIMM number
  @param[IN] Rank                - Rank number
  ErrorResult[IN OUT]            - Error result return data

  @retval 0 - successful
  @retval Other - failure

**/
STATIC
UINT32
FindPerBitResults (
  IN  PSYSHOST                  Host,
  IN  UINT8                     Socket,
  IN  UINT8                     Dimm,
  IN  UINT8                     Rank,
  OUT BIT_ERROR_RESULTS_WR_DDR5 (*ErrorResult)[MAX_CH][SUB_CH]
  );


//
// Internal definitions
//
// Define the max number of sample in this algorithm
//

/**

  Perform Write DQDQS training for DDR5.

  @param[in] Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
WriteDqDqsPerBitDdr5 (
  IN PSYSHOST Host
  )
{
  UINT8                             Socket;
  UINT8                             Ch;
  UINT32                            ChBitmask;
  UINT8                             SubCh;
  UINT8                             ChannelLocal;
  UINT8                             Dimm;
  UINT8                             Rank;
  UINT8                             LogRank = 0;
  UINT8                             Nibble;
  UINT8                             EncodedNibble;
  UINT8                             Bit;
  UINT8                             BitIdx;
  UINT8                             Msl;
  INT16                             BitSkew[BITS_PER_NIBBLE];
  INT16                             BitNibbleMin;
  INT16                             BitSkewMax;
  UINT16                            BitCenter[8];
  UINT8                             RankPresent;
  PerBitPi                          ErrorStart[MAX_CH];
  PerBitPi                          ErrorEnd[MAX_CH];
  PerBitPi                          PiPosition[MAX_CH];
  UINT32                            Status;
  UINT16                            TxDqs;
  UINT8                             FaultyPartsStatus;
  UINT8                             ProblemEyeSize = 0;
  UINT8                             ProblemStrobe = 0;
  UINT8                             Channel = 0;
  struct bitMarginCh                ResultsBit;
  struct channelNvram               (*ChannelNvList)[MAX_CH];
  struct dimmNvram                  (*DimmNvList)[MAX_DIMM];
  struct ddrRank                    (*RankList)[MAX_RANK_DIMM];
  BIT_ERROR_RESULTS_WR_DDR5         (*ErrorResult)[MAX_CH][SUB_CH];
  MRC_RT                            MrcRankList;
  MRC_MST                           MemSsType;
  UINT8                             (*ErrorSamples)[WR_DQ_PI_RANGE_DDR5];
  UINTN                             Index;

  TCPGCAddress                      CPGCAddress =        {
    {0, 0, 0, 0},      // Start
    {0, 0xF, 0, 0x1F}, // Stop
    {0, 0, 0, 0},      // Order
    {0, 0, 0, 0},      // IncRate
    {0, 4, 0, 4}       // IncValue
  };

  //
  // Note: Column increment of 4 assumes BL16 and use_bl4_col_addr = 1
  //

  TWDBPattern WDBPattern  = {0,  0,  1,  BasicVA};
  UINT8 SubChMSVx4 = GetSubChMaxStrobeValid(Host);
  UINT8 SubChMSVx8 = GetSubChMaxStrobeValid(Host) / 2;
  UINT8 MBV = MAX_BITS_DDR5 / 2;
  UINT8 MaxChDdr;

  if (!IsMemFlowEnabled (WriteDqDqsTraining)) {
    return SUCCESS;
  }

  MaxChDdr = GetMaxChDdr ();
  Socket = Host->var.mem.currentSocket;
  MemSsType = GetSysCpuCsrAccessVar()->MemSsType[Socket];
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

  ErrorSamples = RcAllocatePool (sizeof (*ErrorSamples));
  if (ErrorSamples == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }

  Status = SUCCESS;
  SetCurrentTestType (Socket, WrDqTest);

  DoCompDdrioPerSetupOption (Socket, TRAINING_COMP_EVERY_TRAINING);

  WrDqDqsInitDdr5 (Host, Socket);

  ChannelNvList = GetChannelNvList (Host, Socket);
  //
  // Initialize LE and RE Margin results structure
  //
  ZeroMem ((UINT8 *)&ResultsBit, sizeof (ResultsBit));
  ZeroMem ((UINT8 *) (*ErrorResult), sizeof ((*ErrorResult)));
  if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    //
    // Clear the Write leveling DDRIO settings.
    //
    ClearDdrioWlModeDdr5 (Host, Socket);
    RestoreCwlForSpid (Host, Socket);
  }
  SetTrainingModeDdr5 (Socket, TmTxDqDqs);

  //
  // Train each DIMM
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    //
    // Train each rank
    //
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      //
      // Initialize to no ranks present
      //
      RankPresent = 0;

      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if ((*ChannelNvList)[Ch].enabled == 0) {
          continue;
        }

        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }

        //
        // Init DRAM interface only for CTE.
        //
        if (FeaturePcdGet (PcdCteBuild) == TRUE) {
          InitDramInterface (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank);
          //
          // Disable the external WL mode.
          //
          DisableExternalWriteLeveling (Host, Socket, Ch, Dimm, Rank);
        }
        //
        // Inidicate this rank is present on at least one channel
        //
        RankPresent |= (*RankList)[Rank].enabled;
        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          GetChannelMapping (Ch, SubCh, &ChannelLocal);
          //
          // Get the logical rank #
          //
          LogRank = GetLogicalRank (Host, Socket, ChannelLocal, Dimm, Rank);

          SelectCPGCRanks (Host, Socket, ChannelLocal, 1 << LogRank, 0, 0);
          ConvertRdToMrr (Host, Socket, ChannelLocal, 0);
        } //Subch
      } // ch loop

      ChBitmask = GetChBitmask (Host, Socket, Dimm, Rank);
      if (Host->DdrioUltSupport) {
        //
        // LoopCnt = 1
        // NunCL = 32; Just for ULT to save the test time. By HAS, it needs to be 64.
        //
        CteSetupPattern ("wrdqdqs", (1) | (32 << 4), ChBitmask, 3, GetPhyRank (Dimm, Rank));
      } else {
        MrcRankList.Dimm = Dimm;
        MrcRankList.Rank = Rank;
        ChBitmask = GetChBitmask (Host, Socket, Dimm, Rank);
        SetupTest (Host, Socket, MemSsType, ChBitmask, 1, &MrcRankList, DdrLevel, WrDqTest, gsmCsnDelim, 0, FIRST_ITERATION_LOOP);
      }
      //
      // Go to the next rank if this rank is not present on any channels
      //
      if (!RankPresent) {
        continue;
      }
      if (IsBrsPresent (Host, Socket)) {
        ChBitmask = GetChBitmask (Host, Socket, Dimm, Rank);
        //
        // Setup the WrDqDqs test
        //
        SetupIOTest (Host, Socket, ChBitmask, PatWrRd, 32, 1, &CPGCAddress, NSOE, &WDBPattern, 0, 0, CPGC_SUBSEQ_WAIT_DDRT); //FNV restricted to 32
        SetCpgcPatControl (Host, Socket, ChBitmask, BTBUFFER, BTBUFFER, BTBUFFER, DDR_CPGC_PATBUF_MUX0, DDR_CPGC_PATBUF_MUX1, DDR_CPGC_PATBUF_MUX2);
      }

      //
      // Data collecting
      //
      Status = FindPerBitResults (Host, Socket, Dimm, Rank, ErrorResult);

      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if ((*ChannelNvList) [Ch].enabled == 0) {
          continue;
        }

        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }
        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          Channel = Ch;
          if (IsBrsPresent (Host, Socket)) {
            GetChannelMapping (Ch, SubCh, &Channel);
          }

          Status = SUCCESS;

          for (Bit = 0; Bit < MBV; Bit++) {
            Nibble = Bit / 4;
            EncodedNibble = GetStrobeFromNibble (Host, Nibble);

            //
            // Skip if this is an ECC strobe when ECC is disabled
            //
          if (IsBitNotValidSubChDdr5 (Host, Bit)) {
              continue;
            }
            if ((*RankList) [Rank].faultyParts[EncodedNibble] != 0) {
              RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, EncodedNibble, NO_BIT,
                "skipped DDR5 Write DqDqs FindErrorGraphMin\n");
              continue;
            }

            //
            // Copy the bit-packed error data into an array for FindErrorGraphMin
            //
            for (Index = 0; Index < WRITE_DQ_DQS_MAX_SAMPLE; Index++) {
              if (((*ErrorResult)[Ch][SubCh].BitErrorResult[Index][Bit / MAX_BITS_IN_BYTE] & (BIT0 << (Bit % MAX_BITS_IN_BYTE))) == 0) {
                (*ErrorSamples)[Index] = 0;
              } else {
                (*ErrorSamples)[Index] = 1;
              }
            }

            Status = FindErrorGraphMin (Host, Socket, Ch, Dimm, Rank, EncodedNibble, &(*ErrorSamples)[0],
                       WRITE_DQ_DQS_MAX_SAMPLE, &ErrorStart[Ch].PiPosition[Bit],
                       &ErrorEnd[Ch].PiPosition[Bit], &PiPosition[Ch].PiPosition[Bit]);


            ErrorStart[Ch].PiPosition[Bit] = ErrorStart[Ch].PiPosition[Bit] + WR_DQ_PI_START_DDR5;
            ErrorEnd[Ch].PiPosition[Bit] = ErrorEnd[Ch].PiPosition[Bit] + WR_DQ_PI_START_DDR5;
            PiPosition[Ch].PiPosition[Bit] = PiPosition[Ch].PiPosition[Bit] + WR_DQ_PI_START_DDR5;

            //
            // Add in Tx Dqs offset
            //
            if ((*DimmNvList) [Dimm].x4Present) {
              GetSetDataGroup (Host, Socket, Channel, SubCh, Dimm, Rank, EncodedNibble, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, (INT16 *)&TxDqs);
            } else {
              GetSetDataGroup (Host, Socket, Channel, SubCh, Dimm, Rank, EncodedNibble % SubChMSVx8, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, (INT16 *)&TxDqs);
            }

            ErrorStart[Ch].PiPosition[Bit] = (UINT16) (TxDqs - TX_DQ_START_OFFSET_DDR5 - TX_PER_BIT_SETTLE_TIME) + ErrorStart[Ch].PiPosition[Bit];
            ErrorEnd[Ch].PiPosition[Bit] = (UINT16) (TxDqs - TX_DQ_START_OFFSET_DDR5 - TX_PER_BIT_SETTLE_TIME) + ErrorEnd[Ch].PiPosition[Bit];
            PiPosition[Ch].PiPosition[Bit] = (UINT16) (TxDqs - TX_DQ_START_OFFSET_DDR5 - TX_PER_BIT_SETTLE_TIME) + PiPosition[Ch].PiPosition[Bit];

            //LE and RE Margins
            ResultsBit.bits[Ch][Bit].n = ErrorEnd[Ch].PiPosition[Bit];
            ResultsBit.bits[Ch][Bit].p = ErrorStart[Ch].PiPosition[Bit];

            if (Status == FAILURE) {
              //
              // Mark this part as faulty
              //
              Host->nvram.mem.socket[Socket].FaultyPartsFlag[Ch] |= (1 << EncodedNibble);

              (*RankList) [Rank].faultyParts[EncodedNibble] |= FPT_WR_DQ_DQS_FAILED;

              RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, EncodedNibble, Bit,
                "Failed Write DqDqsPerBit\n");

              FaultyPartsStatus = EvaluateFaultyParts (Host, Socket, Ch);

              switch (FaultyPartsStatus) {
              case FPT_NO_ERROR:
                Status = SUCCESS;
                break;
              case FPT_CORRECTABLE_ERROR:
                //
                // if correctable log the warning -> OutputWarning
                // careful for cases when warning gets promoted to error !!!  - log the strobe
                //
                RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, EncodedNibble, NO_BIT,
                  "DDR5 WrDqDqs Correctable\n");
                EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_FPT_MINOR_WR_DQ_DQS, Socket, Ch, Dimm, Rank, EwlSeverityWarning, EncodedNibble, Bit, TxDqDelay, DdrLevel, (UINT8) (ErrorStart[Ch].PiPosition[Bit] - ErrorEnd[Ch].PiPosition[Bit]));
                Status = SUCCESS;
                break;
              default:
                // no need to warning/disable channel - it is done out of the loop
                ProblemEyeSize = (UINT8) (ErrorStart[Ch].PiPosition[Bit] - ErrorEnd[Ch].PiPosition[Bit]);
                ProblemStrobe = EncodedNibble;
                Status = FAILURE;
                break;
              }

              if (Status == FAILURE) {
                break;  // failure confirmed, break the bit loop.
            }
            }
          } // bit loop

          //
          // This function can be used to implement any required workarounds
          //
          TxDqWA (Socket, Ch, SubCh, Dimm, Rank, &PiPosition[Ch], &ResultsBit, TRUE);

#ifdef DEBUG_CODE_BLOCK
          //Print LE and RE Per Bit Deskew Margins for current rank
          if (GetDebugLevel () & SDBG_MAX) {
            DisplayREandLEMarginsDdr5 (Host, Socket, Ch, SubCh, Dimm, Rank, &ResultsBit, TxDqDelay);
          }
#endif // DEBUG_CODE_BLOCK
          DetectLowMargin (Host, Ch, Dimm, Rank, &ResultsBit, TxDqDelay, NON_PER_BIT);

          // BRS TODO: need to check status
          if (Status) {
            RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT,
              "DDR5 WrDq/Dqs Training Failure! (status = %d)\n", Status);
            EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_WR_DQ_DQS, Socket, Channel, Dimm, Rank, EwlSeverityWarning, ProblemStrobe, NO_BIT, TxDqsDelay, DdrLevel, ProblemEyeSize);

            //
            // This is a fatal error condition so map out the entire channel.
            //
            DisableChannelSw (Host, Socket, Channel);
          } // if status

          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
            "dimm = %d, rank = %d\n", Dimm, Rank);
          RcDebugPrint (SDBG_MAX,
            "         Per Bit Margin Center       Per Bit Skew\n");
          RcDebugPrint (SDBG_MAX,
            "N# = Nibble, BCx = Bit Center, BSx = Bit Skew , MSL = Most Skewed Lane\n\n");
          RcDebugPrint (SDBG_MAX,
            "N# BC0 BC1 BC2 BC3   NPi   BS0 BS1 BS2 BS3  MSL\n");

          // Loop for each nibble/strobe.
          for (Nibble = 0; Nibble < SubChMSVx4; Nibble++) {
            //
            // Skip if this is an ECC nibble when ECC is disabled
            // Note: unlike usual in training code, per-bit skew register are mapped linerly, so ECC bits are on the end
            //
            if (IsNibbleNotValidDdr5 (Host, Nibble)) {
              continue;
            }
            EncodedNibble = GetStrobeFromNibble (Host, Nibble);
            BitNibbleMin = 1024;   // Set to largest possible
            RcDebugPrint (SDBG_MAX,
              "%2d ", Nibble);

            // Loop for all the bits in this nibble
            for (Bit = 0; Bit < BITS_PER_NIBBLE; Bit++) {
              // Get the absolute bit index (0-79)
              BitIdx = (Nibble * 4) + Bit;

              // Find the center for this bit
              BitCenter[Bit] = PiPosition[Ch].PiPosition[BitIdx];

              RcDebugPrint (SDBG_MAX,
                "%3d ", BitCenter[Bit]);

              // Check if the center of this bit is smaller than the smallest found so far
              if (BitCenter[Bit] < BitNibbleMin) {
                BitNibbleMin = BitCenter[Bit];
              }
            } // bit loop

            RcDebugPrint (SDBG_MAX,
              "  %3d   ", BitNibbleMin);
            GetSetDataGroup (Host, Socket, Channel, SubCh, Dimm, Rank, EncodedNibble, 0, DdrLevel, TxDqDelay, GSM_UPDATE_CACHE, &BitNibbleMin);
            // Loop for all the bits in this nibble
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

            } // bit loop
            RcDebugPrint (SDBG_MAX, " %3d\n", Msl);
            //
            // Call SetGDCR to update the appropriate register field
            // Pass a pointer to all bits in the nibble - the Get/Set routine will need them all
            //
            GetSetDataGroup (Host, Socket, Channel, SubCh, Dimm, Rank, EncodedNibble, ALL_BITS, DdrLevel, TxDqBitDelay, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET,
                BitSkew);
          } // nibble loop
        } //subch loop
      } //ch loop
    } // rank loop
  } // dimm loop

  SetTrainingModeDdr5 (Socket, TmTxDqDqsExit);

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    if (IsBrsPresent (Host, Socket)) {
      DisplayResultsDdr5 (Host, Socket, TxDqDelay);
    }
    DisplayResultsDdr5 (Host, Socket, TxDqBitDelay);
  }
#endif // DEBUG_CODE_BLOCK
  IO_Reset (Host, Socket);
  FifoTrainReset (Host, Socket);

  RcFreePool (ErrorResult);
  RcFreePool (ErrorSamples);

  Host->nvram.mem.socket[Socket].TrainingStepDone.TxVref = 1;

  return Status;

} //WriteDqDqsPerBitDdr5

/**
  Enable DB DFE circuit.

  @param[in]  Host            Pointer to sysHost
  @param[in]  Socket          Socket number

  @retval MRC_STATUS_SUCCESS  Successfully enabled.
  @retval other               An error occurred.
**/
MRC_STATUS
EnableDbDfeCircuit (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
#ifdef LRDIMM_SUPPORT
  UINT8                             Dimm;
  UINT8                             Rank;
  UINT8                             Channel;
  DIMM_NVRAM_STRUCT                 (*DimmNvList)[MAX_DIMM];
  DDR5_DATA_BUFFER_RWA0_STRUCT      DbRwA0;

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      for (Channel = 0; Channel < MAX_CH; Channel += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Channel) == FALSE) {
          continue;
        }
        if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Channel);
        if (Rank >= (*DimmNvList)[Dimm].numRanks) {
          continue;
        }
        if (!IsLrdimmPresent (Socket, Channel, Dimm)) {
          continue;
        }

        DbRwA0.Data = 0;  // there is no DB RW cache, assume zero

#ifdef USE_LATEST_DB_SPEC
        DbRwA0.Bits.dfe_feature_enable = 1;
#else // #ifdef USE_LATEST_DB_SPEC
        DbRwA0.Bits.dfe_feature_enable_control = 1;
#endif // #ifdef USE_LATEST_DB_SPEC

        DbRwA0.Bits.tap_1_enable_bit_for_all_dq = 1;
        DbRwA0.Bits.tap_2_enable_bit_for_all_dq = 1;
        DbRwA0.Bits.tap_3_enable_bit_for_all_dq = 1;
        DbRwA0.Bits.tap_4_enable_bit_for_all_dq = 1;
        DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RWA0_REG, DbRwA0.Data);

      } // Channel loop
    } // Rank loop
  } // Dimm loop

  return MRC_STATUS_SUCCESS;
#else // #ifdef LRDIMM_SUPPORT
  return MRC_FUNCTION_NOT_IMPLEMENTED;
#endif // #ifdef LRDIMM_SUPPORT
}

/**

  This function does the pattern init for DDR5 write DQDQS training.

  @param[IN] Host                - Pointer to sysHost
  @param[IN] Socket              - Socket number

  @retval N/A

**/

VOID
WrDqDqsInitDdr5 (
  IN PSYSHOST           Host,
  IN UINT8              Socket
  )
{
  MRC_STATUS  Status;
  //
  // If Tx DFE is going to be trained, enable the circuit now so any timing
  // effects are taken into account during the Basic Host Tx DQ DQS sweep.
  //
  if (IsMemFlowEnabled (DbDfeDdr5Training)) {

#ifdef LRDIMM_SUPPORT
    Status = EnableDbDfeCircuit (Host, Socket);
    if (Status != MRC_STATUS_SUCCESS) {
      RcDebugPrintWithDevice (
        SDBG_ERROR, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Warning: failed to enable DB DFE circuit before Basic Host Tx DQ DQS sweep!\n"
        );
    }
#endif // #ifdef LRDIMM_SUPPORT
  }
  if (IsMemFlowEnabled (TxDfeDdr5Training)) {
    //
    // Init the Tx DFE Tap value before write dqdqs
    //
    TxDfeTapInitDdr5 (Host, Socket);
  }
  if (IsBrsPresent (Host, Socket)) {
    //
    // Fill 1 cacheline of the WDB with a simple pattern
    //
    WriteTemporalPattern (Host, Socket, WR_DQ_DQS_PATTERN, 4, FALSE);
  } else {
    //
    // Fill 2 cacheline of the WDB with a simple pattern
    //
    WriteWDBDqLanePattern (Host, Socket, WR_DQ_DQS_PATTERN, 2, FALSE);
  }
} // WrDqDqsInitDdr5



/**

  This function collects all the data from DqDqs algorithm

  @param Host                - Pointer to sysHost
  @param Socket              - Socket number
  @param dimm                - DIMM number
  @param rank                - Rank number
  ErrorResult                - Error result return data

  @retval 0 - successful
  @retval Other - failure

**/
STATIC
UINT32
FindPerBitResults (
  IN  PSYSHOST                  Host,
  IN  UINT8                     Socket,
  IN  UINT8                     Dimm,
  IN  UINT8                     Rank,
  OUT BIT_ERROR_RESULTS_WR_DDR5 (*ErrorResult)[MAX_CH][SUB_CH]
  )
{
  UINT8               Ch;
  UINT8               SubCh;
  UINT8               ChannelLocal;
  UINT32              ChBitmask;
  UINT8               Bit;
  INT16               PiDelay;
  UINT8               PiIndex;
  UINT8               Strobe;
  UINT8               MaxStrobe;
  UINT32              ErrResult;
  UINT32              Status;
  UINT16              TxDqs;
  UINT16              TxDq;
  UINT8               ChStatus;
  UINT32              BwSerr[MAX_CH][SUB_CH][3];
  UINT32              BwSerrEvenOdd[MAX_CH][SUB_CH][2][3];
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT8               SubChMSVx8 = GetSubChMaxStrobeValid(Host) / 2;
  UINT8               MBV = GetMaxBitsValid (Host);
  MRC_MST             MemSsType;
  UINT8               MaxChDdr;
  MaxChDdr = GetMaxChDdr ();
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  ZeroMem ((UINT8 *) (&BwSerrEvenOdd), sizeof (BwSerrEvenOdd));


  Status = SUCCESS;
  ChannelNvList = GetChannelNvList (Host, Socket);

  ChBitmask = GetChBitmask (Host, Socket, Dimm, Rank);
  ErrResult = 0;
  ChStatus = 0;

  ZeroMem ((UINT8 *) (*ErrorResult), sizeof (*ErrorResult));

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "Ddr5 TxDqDqs Pi Scanning...\n");
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "     PI |              Sub-channel A                   |               Sub-channel B                  |\n");
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "DQ:     | 0      7 8     15 16    23 24    31 32    39 | 40    47 48    55 56    63 64    69 70    79 |\n");

  for (PiDelay = WR_DQ_PI_START_DDR5; PiDelay < WR_DQ_PI_START_DDR5 + WR_DQ_PI_RANGE_DDR5; PiDelay += WR_DQ_STEP_SIZE) {

    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if ((*ChannelNvList) [Ch].enabled == 0) {
        continue;
      }

      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
        continue;
      }

      //
      // Set the new Pi.
      //
      MaxStrobe = GetMaxStrobeWithinSubCh (Host, Socket, Ch, Dimm, WrLvlDelay, DdrLevel);
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

        GetChannelMapping (Ch, SubCh, &ChannelLocal);

        for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
          if (IsStrobeNotValidDdr5 (Host, Strobe)) {
            continue;
          }
          GetSetDataGroup (Host, Socket, ChannelLocal, SubCh, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, (INT16 *)&TxDqs);
          TxDq = TxDqs - TX_DQ_START_OFFSET_DDR5 - TX_PER_BIT_SETTLE_TIME + PiDelay;
          GetSetDataGroup (Host, Socket, ChannelLocal, SubCh, Dimm, Rank, Strobe, 0, DdrLevel, TxDqDelay, GSM_UPDATE_CACHE, (INT16 *)&TxDq);
          if (!(*DimmNvList) [Dimm].x4Present) {
            GetSetDataGroup (Host, Socket, ChannelLocal, SubCh, Dimm, Rank, Strobe + SubChMSVx8, 0, DdrLevel, TxDqDelay, GSM_UPDATE_CACHE, (INT16 *)&TxDq);
          }
        } // strobe loop
      } //subch loop
    } // ch loop

    //
    // Clear Errors and Run Test
    //
    if (Host->DdrioUltSupport) {
      CteStartPattern (ChBitmask, 0x3, GetPhyRank (Dimm, Rank));
      CteWaitPatternDone ();
    } else {
      TestControl (Host, Socket, MemSsType, ChBitmask, NO_SUBCH, GetCurrentTestType (Socket), 1, START_TEST, NOT_STOP_TEST);
    }

    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if ((*ChannelNvList) [Ch].enabled == 0) {
        continue;
      }
      ChStatus = 1 << Ch;

      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList)[Dimm].numRanks)) {
        continue;
      }

      //
      // invalidate prefetch cache between cpgc tests for write dq dqs
      //
      InvalidateDdrtPrefetchCache (Host, Socket, Ch);

      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        BwSerr[Ch][SubCh][0] = 0;
        BwSerr[Ch][SubCh][1] = 0;
        BwSerr[Ch][SubCh][2] = 0;
      }
      //
      // Collect test results
      //
      Status = CollectTestResultsDdr5 (Host, Socket, ChStatus, BwSerr);
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

        if (Host->DdrioUltSupport) {
          CteGetMcErrorCounter (1 << Ch, 1 << SubCh, &BwSerrEvenOdd[Ch][SubCh][0][0], &BwSerrEvenOdd[Ch][SubCh][0][2], &BwSerrEvenOdd[Ch][SubCh][1][0], &BwSerrEvenOdd[Ch][SubCh][1][2]);
          BwSerr[Ch][SubCh][0] = BwSerrEvenOdd[Ch][SubCh][0][0] | BwSerrEvenOdd[Ch][SubCh][1][0];
          BwSerr[Ch][SubCh][2] = BwSerrEvenOdd[Ch][SubCh][0][2] | BwSerrEvenOdd[Ch][SubCh][1][2];
        }
#ifdef DEBUG_CODE_BLOCK
        if (GetDebugLevel () & SDBG_MAX) {
          DisplayBwSerrDdr5Ch (Host, Socket, Ch, SubCh, Dimm, Rank, BwSerr[Ch][SubCh], PiDelay);
        }
#endif // DEBUG_CODE_BLOCK
        for (Bit = 0; Bit < MBV; Bit++) {
          if (IsBitNotValid (Host, Bit)) {
            continue;
          }
          if (Bit < 32) {
            ErrResult = BwSerr[Ch][SubCh][0];
          } else {
            ErrResult = BwSerr[Ch][SubCh][2];
          }


          for (PiIndex = 0; PiIndex < WR_DQ_STEP_SIZE; PiIndex++) {
            //
            // Any failure is a failure for the bit
            //
            if (ErrResult & (BIT0 << (Bit % 32))) {
              (*ErrorResult)[Ch][SubCh].BitErrorResult[(PiDelay % WR_DQ_PI_RANGE_DDR5) + PiIndex][Bit / MAX_BITS_IN_BYTE] |= (BIT0 << (Bit % MAX_BITS_IN_BYTE));
            } else {
              (*ErrorResult)[Ch][SubCh].BitErrorResult[(PiDelay % WR_DQ_PI_RANGE_DDR5) + PiIndex][Bit / MAX_BITS_IN_BYTE] &= ~(BIT0 << (Bit % MAX_BITS_IN_BYTE));
            }
          } // piIndex loop
        } // bit loop
      } //subCh loop
    } // ch loop
  } // PiDelay loop

  return Status;
} // FindPerBitResults
