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
#include <Library/SysHostPointerLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemMcIpLib.h>
#include "Include/MemDdrioPrivate.h"
#include <Library/MemRcLib.h>
#include <Library/MemRoutingLib.h>
#define DQ_SLEW_RATE_COMP_INIT      7
#define DQ_SLEW_RATE_COMP_MATCHED   0
#define TX_FALL_SLEW_RATE_UPPER_LIMIT  31
#define TX_FALL_SLEW_RATE_UPPER_BASE   25

/**
  Initialize TxRiseFallSlewRate register for TrainTxRiseFallSlewRate training

  @param[in]  Socket    - Socket number

  @retval  MRC_STATUS_SUCCESS
**/
MRC_STATUS
TxSlewRateInit (
  IN  UINT8             Socket
  )
{
  PSYSHOST            Host;
  UINT8               Ch;
  INT16               TxRiseFallSlewRateValue;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    //
    // TxRiseFallSlewRate is fixed during training
    //
    TxRiseFallSlewRateValue = DQ_SLEW_RATE_COMP_INIT;
    GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, 0, 0, ALL_STROBES, 0, DdrLevel, TxRiseFallSlewRate, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &TxRiseFallSlewRateValue);
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Finalize TxFallSlewRate and TxRiseFallSlewRate registers after TrainTxRiseFallSlewRate training

  @param[in]  Socket    - Socket number

  @retval  MRC_STATUS_SUCCESS
**/
MRC_STATUS
TxSlewRateFinalize (
  IN  UINT8             Socket
  )
{
  PSYSHOST            Host;
  UINT8               Strobe;
  UINT8               Ch;
  INT16               TxRiseFallSlewRateValue;
  INT16               TxFallSlewRateValue;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT8               MSVx8;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  MSVx8 = GetMaxStrobeValid (Host) / 2;

  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // If the best setting for TxFallSlewRate was some number X in the range 25->31, then set
  // DqSlewRateCompOffset to 32 X and set TxFallSlewRate to X. If the best setting for
  // TxFallSlewRate was some number X in the range 0->8 then set DqSlewRateCompOffset to 0
  // and TxFallSlewRate to X.
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    for (Strobe = 0; Strobe < MSVx8; Strobe++) {
      if (IsStrobeNotValid (Host, Strobe)) {
        continue;
      }
      //
      // TxFallSlewRate has been trained - read the trained value
      //
      GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, 0, 0, Strobe, 0, DdrLevel, TxFallSlewRate, GSM_READ_ONLY, &TxFallSlewRateValue);
      if ((TxFallSlewRateValue >= TX_FALL_SLEW_RATE_UPPER_BASE) && (TxFallSlewRateValue <= TX_FALL_SLEW_RATE_UPPER_LIMIT)) {
        TxRiseFallSlewRateValue = (TX_FALL_SLEW_RATE_UPPER_LIMIT + 1) - TxFallSlewRateValue;
      } else {
        TxRiseFallSlewRateValue = DQ_SLEW_RATE_COMP_MATCHED;
      }
      GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, 0, 0, Strobe, 0, DdrLevel, TxRiseFallSlewRate, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &TxRiseFallSlewRateValue);
    } // Strobe
  } // Ch

  return MRC_STATUS_SUCCESS;
}

/**
  Power Training hook router for MemDdrioIpLib
  General purpose API to handle special cases during power training

  @param[in]  Socket          - Socket number
  @param[in]  PowerTrainHook  - The hook identifier

  @retval  MRC_STATUS_SUCCESS if no failure
  @retval  MRC_STATUS_SIGNAL_NOT_SUPPORTED if hook is not supported
  @retval  !MRC_STATUS_SUCCESS otherwise
**/
MRC_STATUS
EFIAPI
MemPowerTrainingDdrioHook (
  IN  UINT8             Socket,
  IN  POWER_TRAIN_HOOK  PowerTrainHook
  )
{
  MRC_STATUS                      Status;
  UINT8                           Index;
  POWER_TRAINING_HOOK_FUNCTIONS   FunctionList[] = {{TxSlewRatePreTest,   TxSlewRateInit},
                                                    {TxSlewRatePostTest,  TxSlewRateFinalize}
                                                   };

  Status = MRC_STATUS_SIGNAL_NOT_SUPPORTED;
  for (Index = 0; Index < sizeof (FunctionList) / sizeof (FunctionList[0]); Index++) {
    if (FunctionList[Index].PowerTrainHook == PowerTrainHook) {
      Status = FunctionList[Index].HookFunction (Socket);
      break;
    }
  }

  return Status;
}

/**
  Average Rx DQ delays for both nibbles in each byte, and adjust Rx DQS delays to account for changes in Rx DQ.

  @param[in]  Host    Pointer to the host structure
  @param[in]  Socket  The target socket

  @retval   Whether or not nibble averaging has been performed.
**/
BOOLEAN
RxDqDqsNibbleAveraging (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  UINT8                 Ch;
  UINT8                 Dimm;
  UINT8                 Rank;
  UINT8                 Strobe;
  INT16                 CompositeRxDqDelay;
  UINT32                SumRxDqDqsDelay;
  UINT32                AverageRxDqDqsDelay;
  INT16                 RxDqDqsPi;
  INT16                 RxDqPi;
  INT16                 RxDqsPi;
  INT16                 Delta;
  struct dimmNvram      (*DimmNvList)[MAX_DIMM];
  struct channelNvram   (*ChannelNvList)[MAX_CH];
  UINT8                 MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8                 MaxChDdr;

  // The Rx DQ delay circuitry, which is the primary focus of this function, is re-appropriated when Rx DFE is enabled,
  // so nibble averaging is not applicable.
  if (GetRxDfeEnableChip (Host, Socket)) {
    return FALSE;
  }

  ChannelNvList = GetChannelNvList (Host, Socket);
  MaxChDdr      = GetMaxChDdr ();
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList)[Dimm].numRanks)) {
          continue;
        }

        // Don't waste cycles averaging on non-zero LRDIMM ranks on non-BRS systems, since the values will be
        // overridden in RxDqDqsLrdimmNormalization.
        if (IsLrdimmPresent (Socket, Ch, Dimm) && (Rank > 0) && (IsBrsPresent (Host, Socket) == FALSE)) {
          continue;
        }

        for (Strobe = 0; Strobe < MSVx8; Strobe++) {

          SumRxDqDqsDelay = 0;
          CompositeRxDqDelay = 0;
          AverageRxDqDqsDelay = 0;

          // Get delay from the first nibble
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RxDqDqsDelay, GSM_READ_ONLY, &RxDqDqsPi);
          SumRxDqDqsDelay  = SumRxDqDqsDelay  + RxDqDqsPi;
          
          // Get delay from the second nibble
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe + MSVx8, 0, DdrLevel, RxDqDqsDelay, GSM_READ_ONLY, &RxDqDqsPi);
          SumRxDqDqsDelay  = SumRxDqDqsDelay  + RxDqDqsPi;
          AverageRxDqDqsDelay = SumRxDqDqsDelay / 2;

          if  (AverageRxDqDqsDelay > 71) {
            CompositeRxDqDelay = 0;
          } else if (AverageRxDqDqsDelay > 63) {
            CompositeRxDqDelay = 1;
          } else if (AverageRxDqDqsDelay > 55) {
            CompositeRxDqDelay = 2;
          } else if (AverageRxDqDqsDelay > 47) {
            CompositeRxDqDelay = 3;
          } else if (AverageRxDqDqsDelay > 39) {
            CompositeRxDqDelay = 4;
          } else {
            CompositeRxDqDelay = 5;
          }

          // Update the DQ delay for the first nibble, and adjust the DQS delay to account for a shift in the DQ delay
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RxDqDelay, GSM_READ_ONLY, &RxDqPi);
          Delta = CompositeRxDqDelay - RxDqPi;
          RxDqsPi = 8 * Delta;
          // write the final DQ and DQS settings
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RxDqDelay, GSM_UPDATE_CACHE, &CompositeRxDqDelay);
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RxDqsDelay,  GSM_UPDATE_CACHE | GSM_WRITE_OFFSET, &RxDqsPi);

          // Update the DQ delay for the second nibble, and adjust the DQS delay to account for a shift in the DQ delay
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe + MSVx8, 0, DdrLevel, RxDqDelay, GSM_READ_ONLY, &RxDqPi);
          Delta = CompositeRxDqDelay - RxDqPi;
          RxDqsPi = 8 * Delta;
          // write the final DQ and DQS settings
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe + MSVx8, 0, DdrLevel, RxDqDelay, GSM_UPDATE_CACHE, &CompositeRxDqDelay);
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe + MSVx8, 0, DdrLevel, RxDqsDelay,  GSM_UPDATE_CACHE | GSM_WRITE_OFFSET, &RxDqsPi);
        } // Strobe
      } // Rank
    } // Dimm
  } // Ch

  return TRUE;
}

/**
  Normalize Rx DQ and DQS delays across all ranks of an LRDIMM to a common value.

  @param[in]  Host    Pointer to the host structure
  @param[in]  Socket  The target socket

  @retval   Whether or not LRDIMM rank normalization has been performed.
**/
BOOLEAN
RxDqDqsLrdimmNormalization (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
#ifdef LRDIMM_SUPPORT

  UINT8                 Channel;
  UINT8                 ChannelIncrement;
  UINT8                 Dimm;
  UINT8                 Rank;
  UINT8                 Strobe;
  INT16                 RxDqPi = 0;
  INT16                 RxDqNPi = 0;
  INT16                 RxDqPPi = 0;
  DIMM_NVRAM_STRUCT     (*DimmNvList)[MAX_DIMM];
  UINT8                 MaxChannel;
  UINT8                 MaxRank;
  UINT8                 MaxStrobe;
  BOOLEAN               Rank0Initialized;

  // The Rx DQ delay circuitry, which is the primary focus of this function, is re-appropriated when Rx DFE is enabled,
  // so LRDIMM rank normalization is not applicable.
  if (GetRxDfeEnableChip (Host, Socket)) {
    return FALSE;
  }

  if (IsBrsPresent (Host, Socket)) {
    // This is only to be applied to 10nm Wave 1 memory IPs, which excludes Breton Sound.
    return FALSE;
  }

  MaxStrobe = GetMaxStrobeValid (Host);

  ChannelIncrement = GetChInc (Host);
  MaxChannel = GetMaxChDdr ();
  for (Channel = 0; Channel < MaxChannel; Channel += ChannelIncrement) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Channel);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsLrdimmPresent (Socket, Channel, Dimm) == FALSE) {
        continue;
      }

      for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {

        MaxRank = Host->var.mem.socket[Socket].maxRankDimm;
        Rank0Initialized = FALSE;

        for (Rank = 0; Rank < MaxRank; Rank++) {
          if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT) ||
              (Rank >= (*DimmNvList)[Dimm].numRanks))
          {
            continue;
          }

          if (Rank == 0) {
            GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RxDqDelay, GSM_READ_ONLY, &RxDqPi);
            GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RxDqsNDelay, GSM_READ_ONLY, &RxDqNPi);
            GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RxDqsPDelay,  GSM_READ_ONLY, &RxDqPPi);
            Rank0Initialized = TRUE;
          } else {
            ASSERT (Rank0Initialized);
            GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RxDqDelay, GSM_UPDATE_CACHE, &RxDqPi);
            GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RxDqsNDelay, GSM_UPDATE_CACHE, &RxDqNPi);
            GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RxDqsPDelay,  GSM_UPDATE_CACHE, &RxDqPPi);
          }

        } // Rank
      } // Strobe
    } // Dimm
  } // Channel

  return TRUE;

#else

  return FALSE;

#endif // LRDIMM_SUPPORT ... else
}

/**

  Clean up any chipset bugs related to ReadDqDqsPerBit training

  @param[in] Host   - Pointer to sysHost, the system Host (root) structure struct
  @param[in] Socket - Socket Id

  @retval  MRC_STATUS_SUCCESS if no failure
  @retval  !MRC_STATUS_SUCCESS otherwise

**/
MRC_STATUS
EFIAPI
ReadDqDqsCleanup (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket
)
{
  BOOLEAN StrobesAveraged;
  BOOLEAN RanksNormalized;

  RcDebugPrint (SDBG_MAX,
                     "Starting_ReadDqDqsCleanup\n");

  StrobesAveraged = RxDqDqsNibbleAveraging (Host, Socket);
  RanksNormalized = RxDqDqsLrdimmNormalization (Host, Socket);

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    if (StrobesAveraged || RanksNormalized) {
      DisplayResults (Host, Socket, RxDqDqsDelay);
      DisplayResults (Host, Socket, RxDqDelay);
      DisplayResults (Host, Socket, RxDqsPDelay);
      DisplayResults (Host, Socket, RxDqsNDelay);
      DisplayResults (Host, Socket, RxDqsBitDelay);
    } else {
      RcDebugPrint (SDBG_MAX,
                              "\nNO CLEANUP REQUIRED\n");
    }
  }
#endif //DEBUG_CODE_BLOCK
  RcDebugPrint (SDBG_MAX,
                     "\nFinished_ReadDqDqsCleanup\n");
  return MRC_STATUS_SUCCESS;
} //ReadDqDqsCleanup

/**

  Return Override Read Preamble timing setting

  @param[in] Socket         - Socket number
  @param[in] PreambleType   - READ_PREAMBLE

  @retval PREAMBLE_1TCLK, PREAMBLE_2TCLK, PREAMBLE_3TCLK, PREAMBLE_4TCLK

**/
UINT8
EFIAPI
GetReadPreambleOverride (
  IN      UINT8   Socket,
  IN      UINT8   ReadPreamble
  )
{
  UINT8             ReadPreambleNew;

  //
  // Return caller’s ReadPreamble by default
  //
  ReadPreambleNew = ReadPreamble;

  return ReadPreambleNew;
}