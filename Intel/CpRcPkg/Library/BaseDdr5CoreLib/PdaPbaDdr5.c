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
#include <Memory/CpgcDefinitions.h>
#include "Ddr5Core.h"
#include <Memory/Ddr5Cmds.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CteNetLib.h>
#include "MemHost.h"
#include <Library/MemDdrioIpLib.h>
#include <Library/IoAccessLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/Ddr5CoreLib.h>
#include <Memory/Ddr5RcwRegs.h>
#include <Memory/Ddr5DbRegs.h>
#include <Memory/JedecDefinitions.h>

/**

Force DDR5 Mode during PBA

@param[in] Socket           - Socket to initialize
@param[in] Ch               - Channel Number

@retval None

**/
VOID
ForceDdr5Mode(
  IN UINT8     Socket,
  IN UINT8     Ch
);

/**

  Configures the data buffer for PDA enumeration.

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Socket number
  @param[in] ChannelMask  - Bitmask of channels to configure

  @retval none

**/
VOID
ConfigureBufferForPda (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChannelMask
)
{
  UINT8                                        Channel;
  UINT8                                        Dimm;
  UINT8                                        Rank = 0;
  UINT8                                        MaxChDdr;
  DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_STRUCT  ParityAndAlert;
  DDR5_DATA_BUFFER_RW82_STRUCT                 DataBufferRw82;

  MaxChDdr = GetMaxChDdr ();

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    // Go to next channel if this one is disabled
    if ((IsChannelEnabled (Socket, Channel) == FALSE) || (((1 << Channel) & ChannelMask) == 0)) {
      continue;
    }
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsLrdimmPresent (Socket, Channel, Dimm) == FALSE) {
        continue;
      }

      if (IsDcpmmPresentDimm(Socket, Channel, Dimm) == TRUE) {
        continue;
      }

      //
      // Block commands to DRAM, forward commands to DB
      //
      ParityAndAlert.Data = 0;
#ifdef USE_LATEST_RCD_SPEC
      ParityAndAlert.Bits.data_buffer_interface_forward_all_cmds = 1;
#else // #ifdef USE_LATEST_RCD_SPEC
      ParityAndAlert.Bits.data_buffer_interface_block_all_cmds = 1;
#endif // #ifdef USE_LATEST_RCD_SPEC
      RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAndAlert.Data);

      //
      // Configure DB in DQ pass-through mode for writes
      //
      DataBufferRw82.Data = 0;
      DataBufferRw82.Bits.dq_pass_through_mode = 1;
      // Set to write direction
      DataBufferRw82.Bits.dq_pass_through_mode_direction_select = 0;
      DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW82_REG, DataBufferRw82.Data);

      //
      // Block commands to DB, forward commands to DRAM
      //
      ParityAndAlert.Data = 0;
#ifdef USE_LATEST_RCD_SPEC
      ParityAndAlert.Bits.dram_interface_forward_all_cmds = 1;
#else // #ifdef USE_LATEST_RCD_SPEC
      ParityAndAlert.Bits.dram_interface_block_all_cmds = 1;
#endif // #ifdef USE_LATEST_RCD_SPEC
      RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAndAlert.Data);

    } // Dimm loop
  } // Channel loop
}

/**

  Restores the data buffer configuration after PDA enumeration is complete.

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Socket number
  @param[in] ChannelMask  - Bitmask of channels number to restore

  @retval none

**/
VOID
RestoreBufferAfterPda (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChannelMask
  )
{
  UINT8                                        Channel;
  UINT8                                        Dimm;
  UINT8                                        Rank = 0;
  UINT8                                        MaxChDdr;
  DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_STRUCT  ParityAndAlert;
  DDR5_DATA_BUFFER_RW82_STRUCT                 DataBufferRw82;

  MaxChDdr = GetMaxChDdr ();

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    // Go to next channel if this one is disabled
    if ((IsChannelEnabled (Socket, Channel) == FALSE) || (((1 << Channel) & ChannelMask) == 0)) {
      continue;
    }
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsLrdimmPresent (Socket, Channel, Dimm) == FALSE) {
        continue;
      }

      if (IsDcpmmPresentDimm(Socket, Channel, Dimm) == TRUE) {
        continue;
      }

      //
      // Block commands to DRAM, forward commands to DB
      //
      ParityAndAlert.Data = 0;
#ifdef USE_LATEST_RCD_SPEC
      ParityAndAlert.Bits.data_buffer_interface_forward_all_cmds = 1;
#else // #ifdef USE_LATEST_RCD_SPEC
      ParityAndAlert.Bits.data_buffer_interface_block_all_cmds = 1;
#endif // #ifdef USE_LATEST_RCD_SPEC
      RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAndAlert.Data);

      //
      // Configure DB in DQ pass-through mode for reads
      //
      DataBufferRw82.Data = 0;
      DataBufferRw82.Bits.dq_pass_through_mode = 1;
      DataBufferRw82.Bits.dq_pass_through_mode_direction_select = 1;
      DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW82_REG, DataBufferRw82.Data);

      //
      // Block commands to DB, forward commands to DRAM
      //
      ParityAndAlert.Data = 0;
#ifdef USE_LATEST_RCD_SPEC
      ParityAndAlert.Bits.dram_interface_forward_all_cmds = 1;
#else // #ifdef USE_LATEST_RCD_SPEC
      ParityAndAlert.Bits.dram_interface_block_all_cmds = 1;
#endif // #ifdef USE_LATEST_RCD_SPEC
      RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAndAlert.Data);

    } // Dimm loop
  } // Channel loop
}
/**

  Set PDA Enumerate ID.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket to initialize
  @param[in] ChannelMask      - Bitmask of channels to init (1 in a bit position = init that channel number)

  @retval status

**/
UINT32
SetPdaEnumerateIdDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChannelMask
  )
{
  UINT8                           Ch;
  UINT8                           Dimm;
  UINT8                           Rank;
  UINT8                           RankPresent;
  struct dimmNvram                (*DimmNvList) [MAX_DIMM];
  struct channelNvram             (*ChannelNvList) [MAX_CH];
  UINT8                           SubChannel = 0;
  UINT8                           MaxStrobe = 0;
  UINT8                           SubChannelMask = 0;
  UINT8                           Strobe = 0;
  UINT8                           SubChMSVx4;
  UINT8                           SubChMSVx8;
  struct ddrRank                  (*RankList) [MAX_RANK_DIMM];
  UINT8                           MaxChDdr;
  UINT8                           LogRank;
  UINT32                          CpgcSeqRankL2PMapping[MAX_CH][SUB_CH][2];
  WRITE_TIMINGS                   WriteTimings;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  SubChMSVx8 = GetSubChMaxStrobeValid (Host) / 2;

  //
  // Return if this Socket is disabled
  //
  if (IsSocketDimmPopulated (Host, Socket) != TRUE) {
    return SUCCESS;
  }

  RcDebugPrint (SDBG_MAX,
    "Set Pda Enumerate Id Start \n");

  ChannelNvList = GetChannelNvList (Host, Socket);
  ZeroMem ((UINT8 *) CpgcSeqRankL2PMapping, sizeof (CpgcSeqRankL2PMapping));
  //
  // Save the CPGC Rank L2P mapping settings
  //
  SaveCpgcRankL2pMapping (Host, Socket, CpgcSeqRankL2PMapping);

  // Configure buffer for PDA if necessary
  ConfigureBufferForPda (Host, Socket, ChannelMask);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      //
      // Initialize to no ranks present
      //
      RankPresent = 0;

      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        // Go to next channel if this one is disabled
        if (((*ChannelNvList) [Ch].enabled == 0) || (((1 << Ch) & ChannelMask) == 0)) {
          continue;
        }
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        DimmNvList = GetDimmNvList (Host, Socket, Ch);

        if ((*DimmNvList) [Dimm].dimmPresent == 0) {
          continue;
        }
        // Continue if this Rank is disabled
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }
        //
        // Inidicate this Rank is present on at least one channel
        //
        RankPresent |= (*RankList) [Rank].enabled;
      } // Ch loop

      //
      // Go to the next Rank if this Rank is not present on any channels
      //
      if (!RankPresent) {
        continue;
      }

      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (((*ChannelNvList) [Ch].enabled == 0)  || (((1 << Ch) & ChannelMask) == 0)) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        // Continue to the next Rank if this one is disabled
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }

        //
        // Bypass DDRT2 for PDA enumeration
        //
        if (IsDcpmmPresentDimm (Socket, Ch, Dimm)) {
          continue;
        }

        LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
        RcDebugPrint (SDBG_MAX,  "Set Pda Enumerate Id Start for dimm %d \n", Dimm);
        SelectCPGCRanks (Host, Socket, Ch, 1 << LogRank, 0, 0);

        ProgramPdaPbaTimingValues (Host, Socket, Ch, &WriteTimings);

        if (!(*DimmNvList) [Dimm].x4Present) {
          MaxStrobe = SubChMSVx8;
        } else {
          MaxStrobe = SubChMSVx4;
        }

        EnterRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);

        //
        // Drive DQS LOW.Prior to when the MPC command for PDA Enumerate Programming Mode entry is sent by the host, the host must drive DQS_t and
        // DQS_c differentially low.
        //
        SetDriveDqsLowDdr5 (Host, Socket, Ch, SUB_CH_A, 1, TRUE);
        SetDriveDqsLowDdr5 (Host, Socket, Ch, SUB_CH_B, 1, TRUE);
        //
        // Set all the DQ as high.
        //
        for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
          //
          // Skip if this is an ECC Strobe when ECC is disabled
          //
          if (IsStrobeNotValidDdr5 (Host, Strobe)) {
            continue;
          }
          //
          // Drive DQ high.
          //
          if ((*DimmNvList) [Dimm].x4Present) {
            OverrideDqValueDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Strobe, TRUE, 0xF);
          } else {
            //
            // Drive all 8 DQs high.
            //
            OverrideDqValueDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Strobe, TRUE, 0xF);
            OverrideDqValueDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Strobe + SubChMSVx8, TRUE, 0xF);
          }
        }//Strobe loop
        //
        // Give some time to IP to sample it and take effective.
        //
        TrainingDelay (Host, 60, 1);
        //
        // Send MPC command to enter PDA_ENUMERATE_PROGRAM_MODE.
        //
        SubChannelMask = (1 << SubChannel);
        WriteMPC (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank, ENTER_PDA_ENUMERATE_PROGRAM_MODE, TIMING_4N, TRUE);
        for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
          //
          // Skip if this is an ECC Strobe when ECC is disabled
          //
          if (IsStrobeNotValidDdr5 (Host, Strobe)) {
            continue;
          }
          //
          // Drive according DQ low.
          //
          if ((*DimmNvList) [Dimm].x4Present) {
            SetDataTxOnDdrioDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Strobe, 1);
            OverrideDqValueDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Strobe, TRUE, 0);
          } else {
            SetDataTxOnDdrioDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Strobe, 1);
            SetDataTxOnDdrioDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Strobe + SubChMSVx8, 1);
            OverrideDqValueDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Strobe, TRUE, 0);
            OverrideDqValueDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Strobe + SubChMSVx8, TRUE, 0);
          }
          //
          // Give some time to IP to sample it and take effective.
          //
          TrainingDelay (Host, 60, 1);
          FixedDelayMicroSecond (1);
          //
          // Send PDA Enumerate ID.
          //
          WriteMPC (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank, PDA_ENUMERATE_ID | Strobe, TIMING_4N, TRUE);
          FixedDelayMicroSecond (1);
          FixedDelayMicroSecond (1);
          //
          // Restore it to high.
          //
          if ((*DimmNvList) [Dimm].x4Present) {
            SetDataTxOnDdrioDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Strobe, 0);
            OverrideDqValueDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Strobe, TRUE, 0xF);
          } else {
            SetDataTxOnDdrioDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Strobe, 0);
            SetDataTxOnDdrioDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Strobe + SubChMSVx8, 0);
            OverrideDqValueDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Strobe, TRUE, 0xF);
            OverrideDqValueDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Strobe + SubChMSVx8, TRUE, 0xF);
          }
          //
          // Give some time to IP to sample it and take effective.
          //
          TrainingDelay (Host, 60, 1);
        }//Strobe loop
        //
        // Send MPC command to exit PDA_ENUMERATE_PROGRAM_MODE.
        //
        SubChannelMask = (1 << SubChannel);
        WriteMPC (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank, EXIT_PDA_ENUMERATE_PROGRAM_MODE, TIMING_4N, TRUE);
        //
        // Disable the DQ override.
        //
        for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
          //
          // Skip if this is an ECC Strobe when ECC is disabled
          //
          if (IsStrobeNotValidDdr5 (Host, Strobe)) {
            continue;
          }
          if ((*DimmNvList) [Dimm].x4Present) {
            OverrideDqValueDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Strobe, FALSE, 0);
          } else {
            OverrideDqValueDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Strobe, FALSE, 0);
            OverrideDqValueDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Strobe + SubChMSVx8, FALSE, 0);
          }
        }//Strobe loop

        //
        // Give some time to IP to sample it and take effective.
        //
        TrainingDelay (Host, 60, 1);

        //
        //Restore DriveDqsLow for all the strobes.
        //
        SetDriveDqsLowDdr5 (Host, Socket, Ch, SUB_CH_A, 0, FALSE);
        SetDriveDqsLowDdr5 (Host, Socket, Ch, SUB_CH_B, 0, FALSE);

        ExitRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);

        //
        // Restore the CWL
        //
        ProgramJedecTimingsDdrio (Host, Socket, Ch, &WriteTimings);
      } //Ch loop
    }//Rank loop
  }//Dimm loop
  //
  // Restore the CPGC Rank L2P Mapping Settings.
  //
  RestoreCpgcRankL2pMapping (Host, Socket, CpgcSeqRankL2PMapping);

  RestoreBufferAfterPda (Host, Socket, ChannelMask);

  return SUCCESS;
}

/**

  Set PBA Enumerate ID.

  @param[in] Host        - Pointer to sysHost
  @retval status

**/
UINT32
PbaEnumerateIdDdr5 (
  IN PSYSHOST  Host
  )
{
  UINT8                Ch;
  UINT8                Dimm;
  UINT8                Rank;
  UINT8                Socket;
  struct dimmNvram     (*DimmNvList) [MAX_DIMM];
  struct channelNvram  (*ChannelNvList) [MAX_CH];
  UINT8                MaxStrobePairs = 0;
  UINT8                Strobe = 0;
  DDR5_DATA_BUFFER_RW81_STRUCT  DataBufferRw81;
  DDR5_DATA_BUFFER_RW92_STRUCT  DataBufferRw92;
  DDR5_DATA_BUFFER_RW93_STRUCT  DataBufferRw93;
  UINT8                SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8                SubChMSVx8 = GetSubChMaxStrobeValid (Host) / 2;
  UINT8                MaxChDdr;
  UINT8                LogicalRank;
  WRITE_TIMINGS        WriteTimings;

  //
  // Return if this step is disabled
  //
  if (!IsMemFlowEnabled (PbaEnumerateId)) {
    return SUCCESS;
  }

  MaxChDdr = GetMaxChDdr ();
  Socket = Host->var.mem.currentSocket;
  //
  // Return if this Socket is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return SUCCESS;
  }
  //
  // Return if LRDIMM is not present.
  //
  if (Host->nvram.mem.socket[Socket].lrDimmPresent == 0) {
    return SUCCESS;
  }
  RcDebugPrint (SDBG_MAX,
    "Set Pba Enumerate Id Start \n");

  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // add the code to enter capt and exit at the end.
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    // DIMM Ranks do not have their own data buffer - data buffers are shared for ranks, so no need to loop over
    // multiple ranks.  Just set rank to 0 here.
    Rank = 0;

    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if ((*ChannelNvList) [Ch].enabled == 0) {
        continue;
      }
      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      // Continue to the next Rank if this one is disabled
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
        continue;
      }

      // Setup the CPGC to test the proper logical rank
      LogicalRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
      SelectCPGCRanks (Host, Socket, Ch, 1 << LogicalRank, 0, 0);
      ForceDdr5Mode (Socket, Ch);
      ProgramPdaPbaTimingValues (Host, Socket, Ch, &WriteTimings);

      if (!(*DimmNvList) [Dimm].x4Present) {
        MaxStrobePairs = SubChMSVx8;
      } else {
        MaxStrobePairs = SubChMSVx4 / 2;
      }
      //
      // Send MRW to select all the devices.
      //
      DataBufferRw93.Data = 0;
      DataBufferRw93.Bits.pba_select_id = PBA_SELECT_ALL;
      DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, DataBufferRw93.Data);

      //
      // Drive DQS LOW.Prior to when the MRW command for PBA Enumerate Programming Mode entry is sent by the host, the host must drive DQS_t and
      // DQS_c differentially low.
      //
      SetDriveDqsLowDdr5 (Host, Socket, Ch, SUB_CH_A, 1, TRUE);
      SetDriveDqsLowDdr5 (Host, Socket, Ch, SUB_CH_B, 1, TRUE);
      //
      // Set all the DQ as high.
      //
      for (Strobe = 0; Strobe < MaxStrobePairs; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }
        //
        // Drive DQ high.
        //
        SetDataTxOnDdrioDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Strobe, 1);
        SetDataTxOnDdrioDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Strobe + MaxStrobePairs, 1);
        OverrideDqValueDdr5 (Host, Socket, Ch,  TWO_SUBCH_MASK, Strobe, TRUE, 0xF);
        OverrideDqValueDdr5 (Host, Socket, Ch,  TWO_SUBCH_MASK, Strobe + MaxStrobePairs, TRUE, 0xF);
      }//Strobe loop

      //
      // Enable PBA Enumerate Programming Mode.
      //
      DataBufferRw81.Data = 0;
      DataBufferRw81.Bits.pba_enumerate_programming_mode = 1;
      DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW81_REG, DataBufferRw81.Data);

      for (Strobe = 0; Strobe < MaxStrobePairs; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }
        //
        // Drive according DQ low.
        //
        OverrideDqValueDdr5 (Host, Socket, Ch,  TWO_SUBCH_MASK, Strobe, TRUE, 0);
        OverrideDqValueDdr5 (Host, Socket, Ch,  TWO_SUBCH_MASK, Strobe + MaxStrobePairs, TRUE, 0);

        //
        // Send PBA Enumerate ID.
        //
        // Give every data buffer a unique ID
        //
        DataBufferRw92.Data = 0;
        DataBufferRw92.Bits.pba_set_id = Strobe;
        DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW92_REG, DataBufferRw92.Data);

        //
        // Restore it to high.
        //
        OverrideDqValueDdr5 (Host, Socket, Ch,  TWO_SUBCH_MASK, Strobe, TRUE, 0xF);
        OverrideDqValueDdr5 (Host, Socket, Ch,  TWO_SUBCH_MASK, Strobe + MaxStrobePairs, TRUE, 0xF);
      }//Strobe loop

      //
      // Disable PBA Enumerate Programming Mode.
      //
      DataBufferRw81.Bits.pba_enumerate_programming_mode = 0;
      DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW81_REG, DataBufferRw81.Data);
      //
      // Disable the DQ override.
      //
      for (Strobe = 0; Strobe < MaxStrobePairs; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }
        SetDataTxOnDdrioDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Strobe, 0);
        SetDataTxOnDdrioDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Strobe + MaxStrobePairs, 0);
        OverrideDqValueDdr5 (Host, Socket, Ch,  TWO_SUBCH_MASK, Strobe, FALSE, 0);
        OverrideDqValueDdr5 (Host, Socket, Ch,  TWO_SUBCH_MASK, Strobe + MaxStrobePairs, FALSE, 0);
      }//Strobe loop
      //
      //Restore DriveDqsLow for all the strobes.
      //
      SetDriveDqsLowDdr5 (Host, Socket, Ch, SUB_CH_A, 0, FALSE);
      SetDriveDqsLowDdr5 (Host, Socket, Ch, SUB_CH_B, 0, FALSE);
      //
      // Restore the CWL
      //
      ProgramJedecTimingsDdrio (Host, Socket, Ch, &WriteTimings);
    } //Ch loop
  }//Dimm loop

  return EFI_SUCCESS;
}

/**

Force DDR5 Mode during PBA

@param[in] Socket           - Socket to initialize
@param[in] Ch               - Channel Number

@retval None

**/
VOID
ForceDdr5Mode(
  IN UINT8     Socket,
  IN UINT8     Ch
)
{
  SetCpgcTrainTarget(Socket, Ch, FALSE);
  SetDdrtForceMode(Socket, Ch, FALSE);
}