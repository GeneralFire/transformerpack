/** @file

  File implements Receive Enable Normalize\Denormalize operation

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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
#include <Library/MemDdrioIpLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/SysHostPointerLib.h>
#include <Include/BaseMemoryCoreInternal.h>

#define RECEN_DENORMALIZE_HIGH_RANGE 300
#define RECEN_DENORMALIZE_LOW_RANGE 64
#define RECEN_DENORMALIZE_IOLATENCY 2
#define RECEN_DENORMALIZE_PIVALUE (RECEN_DENORMALIZE_IOLATENCY * QCLKPITICKS)

/*
  Denormalize receive enable by 128 Pi
  Increase delay by 128 pi for receive enable signal for
  When Min Pi value in a given rank is less than 64 and Max Pi value is
  less than 300.

  @param[in] Socket - Socket number

  @retval EFI_SUCCESS    -  Successfully denormalized
  @retval !EFI_SUCCESS   -  Error during denormalization
*/
EFI_STATUS
EFIAPI
DenormalizeRecvEnable (
  IN UINT8 Socket
  )
{
  UINT8                             Ch = 0;
  struct channelNvram               (*ChannelNvList)[MAX_CH];
  struct dimmNvram                  (*DimmNvList)[MAX_DIMM];
  struct ddrRank                    (*RankList)[MAX_RANK_DIMM];
  PSYSHOST                          Host = NULL;
  UINT8                             Dimm = 0;
  UINT8                             Rank = 0;
  UINT8                             NumRanks = 0;
  UINT8                             Strobe = 0;
  UINT8                             LogRank = 0;
  INT16                             PiDelay = 0;
  UINT16                            MinValueInStrobe = 0;
  UINT16                            MaxValueInStrobe = 0;
  UINT16                            UsDelay = 0;
  UINT8                             IoLatency = 0;
  UINT8                             MSVx4     = 0;
  EFI_STATUS                        Status = EFI_SUCCESS;
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  ASSERT (Host != NULL);
  if (Host == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  MSVx4 = GetMaxStrobeValid (Host);
  ChannelNvList = GetChannelNvList (Host, Socket);
  ASSERT (ChannelNvList != NULL);

  RcDebugPrint (SDBG_MAX, "DenormalizeRecvEnable: Before Denormalization RecvEnable Values Start");
  DisplayResults (Host, Socket, RecEnDelay);
  RcDebugPrint (SDBG_MAX, "DenormalizeRecvEnable: Before Denormalization RecvEnable Values End\n");

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    NumRanks = Host->var.mem.socket[Socket].maxRankDimm;
    for (Rank = 0; Rank < NumRanks; Rank++) {

      for (Ch = 0; Ch < MaxChDdr; Ch++) {
        if ((*ChannelNvList)[Ch].enabled == 0) {
          continue;
        }

        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        ASSERT (RankList != NULL);

        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        ASSERT (DimmNvList != NULL);

        // Continue to the next Rank if this one is disabled
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList)[Dimm].numRanks)) {
          continue;
        }
        //
        // Get the logical rank #
        //
        LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
        // Initialize MaxValueInStrobe to 0 and MinValueInStrobe to maximum value, these variables
        // are used in the below loop to find Max\Min among strobes within a given Rank.
        GetDataGroupLimits (Host, DdrLevel, RecEnDelay, &MaxValueInStrobe, &MinValueInStrobe, &UsDelay);
        for (Strobe = 0; Strobe < MSVx4; Strobe++) {
          //
          // Skip if this is an ECC Strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, Strobe)) {
            continue;
          }

          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RecEnDelay, GSM_READ_ONLY,
            &PiDelay);
          if (MinValueInStrobe > PiDelay) {
            MinValueInStrobe = PiDelay;
          }
          if (MaxValueInStrobe < PiDelay) {
            MaxValueInStrobe = PiDelay;
          }
        }

        if ((MinValueInStrobe < RECEN_DENORMALIZE_LOW_RANGE) && (MaxValueInStrobe < RECEN_DENORMALIZE_HIGH_RANGE)){
          RcDebugPrint (SDBG_MAX, "Applying Denormalize for Logical Rank = %d\n", LogRank);
          GetIoLatency (Socket, Ch, LogRank, GSM_READ_ONLY, &IoLatency);
          IoLatency += RECEN_DENORMALIZE_IOLATENCY;
          PiDelay = RECEN_DENORMALIZE_PIVALUE;
          SetIOLatency (Socket, Ch, LogRank, IoLatency);
          for (Strobe = 0; Strobe < MSVx4; Strobe++) {
            //
            // Skip if this is an ECC Strobe when ECC is disabled
            //
            if (IsStrobeNotValid (Host, Strobe)) {
              continue;
            }
            GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RecEnDelay,  GSM_UPDATE_CACHE | GSM_WRITE_OFFSET,
            &PiDelay);
          }
        }
      }
    }
  }
  RcDebugPrint (SDBG_MAX, "\nDenormalizeRecvEnable: After Denormalization RecvEnable Values Start\n");
  DisplayResults (Host, Socket, RecEnDelay);
  RcDebugPrint (SDBG_MAX, "DenormalizeRecvEnable: After Denormalization RecvEnable Values End\n");
  return Status;
}

/*
  Normalize receive enable signal.
  Decrease IO latency by 2 and decrease Pi delay by 128.

  @param[in] Socket - Socket number

  @retval EFI_SUCCESS    -  Successfully Normalized
  @retval !EFI_SUCCESS   -  Error during Normalization
*/
EFI_STATUS
EFIAPI
RenormalizeRecvEnable (
  IN UINT8 Socket
  )
{
  UINT8                             Ch = 0;
  struct channelNvram               (*ChannelNvList)[MAX_CH];
  struct dimmNvram                  (*DimmNvList)[MAX_DIMM];
  struct ddrRank                    (*RankList)[MAX_RANK_DIMM];
  PSYSHOST                          Host = NULL;
  UINT8                             Dimm = 0;
  UINT8                             Rank = 0;
  UINT8                             NumRanks = 0;
  UINT8                             Strobe = 0;
  UINT8                             LogRank = 0;
  INT16                             PiDelay = 0;
  UINT8                             LogicDelayIsOne = 0;
  UINT8                             IoLatency = 0;
  UINT8                             MSVx4     = 0;
  EFI_STATUS                        Status = EFI_SUCCESS;
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  ASSERT (Host != NULL);
  if (Host == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  MSVx4 = GetMaxStrobeValid (Host);
  ChannelNvList = GetChannelNvList (Host, Socket);
  ASSERT (ChannelNvList != NULL);

  RcDebugPrint (SDBG_MAX, "RenormalizeRecvEnable: Before Renormalization RecvEnable Values Start");
  DisplayResults (Host, Socket, RecEnDelay);
  RcDebugPrint (SDBG_MAX, "RenormalizeRecvEnable: Before Renormalization RecvEnable Values End\n");

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    NumRanks = Host->var.mem.socket[Socket].maxRankDimm;
    for (Rank = 0; Rank < NumRanks; Rank++) {

      for (Ch = 0; Ch < MaxChDdr; Ch++) {
        if ((*ChannelNvList)[Ch].enabled == 0) {
          continue;
        }

        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        ASSERT (RankList != NULL);

        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        ASSERT (DimmNvList != NULL);

        // Continue to the next Rank if this one is disabled
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList)[Dimm].numRanks)) {
          continue;
        }
        //
        // Get the logical rank #
        //
        LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
        LogicDelayIsOne = 0;
        for (Strobe = 0; Strobe < MSVx4; Strobe++) {
          //
          // Skip if this is an ECC Strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, Strobe)) {
            LogicDelayIsOne++;
            continue;
          }
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RecEnDelay, GSM_READ_ONLY,
            &PiDelay);
          if (PiDelay >= 128) {
            LogicDelayIsOne++;
          }
        }

        if (LogicDelayIsOne == MSVx4) {

          GetIoLatency (Socket, Ch, LogRank, GSM_READ_ONLY, &IoLatency);
          IoLatency -= RECEN_DENORMALIZE_IOLATENCY;
          PiDelay = -(INT16)RECEN_DENORMALIZE_PIVALUE;
          SetIOLatency (Socket, Ch, LogRank, IoLatency);

          for (Strobe = 0; Strobe < MSVx4; Strobe++) {
            //
            // Skip if this is an ECC Strobe when ECC is disabled
            //
            if (IsStrobeNotValid (Host, Strobe)) {
              continue;
            }
            GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RecEnDelay,  GSM_UPDATE_CACHE | GSM_WRITE_OFFSET,
            &PiDelay);
          }
        }
      }
    }
  }
  RcDebugPrint (SDBG_MAX, "\nRenormalizeRecvEnable: After Renormalization RecvEnable Values Start");
  DisplayResults (Host, Socket, RecEnDelay);
  RcDebugPrint (SDBG_MAX, "RenormalizeRecvEnable: After Renormalization RecvEnable Values End\n");
  return Status;
}
