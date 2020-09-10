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
#include <Library/SiliconWorkaroundLib.h>

#define DQS2DQ_TRACKING_RETURN_VALUE_INITIALIZATION  0x7F
#define CMD_TYPE_OVERRIDE_ENABLE  1
#define CMD_TYPE_OVERRIDE_DISABLE 0
#define CMD_TYPE_OVERRIDE_DEFAULT 0xFF
#define CMD_TYPE_OVERRIDE_MR46    0x11
#define CMD_TYPE_OVERRIDE_MR47    0x22
#define DQS_INTERVAL_TIMER_RUN_TIME_2048_ENCODING 0x40
#define DQS_INTERVAL_TIMER_RUN_TIME_2048  2048
#define DB_TRK_CALC_INIT          256   // 256 tCK data buffer calculation period

//
// Update minimum txdq_dly setting to 192 if command normalization is not enabled
// Update minimum txdq_dly setting to 64 if command normalization is enabled
//
#define TX_RETRAIN_MIN_TXDQ_DLY_CMD_NORMAL_DIS  192
#define TX_RETRAIN_MIN_TXDQ_DLY_CMD_NORMAL_EN   64

/**

  Configures the data buffer to DRAM tDQS2DQ Tracking Initialization Mode or DRAM
  tDQS2DQ Tracking Mode based on input paramater.

  @param[in] Socket        - Socket number
  @param[in] Channel       - Channel number
  @param[in] Dimm          - DIMM number in Channel
  @param[in] Rank          - Rank number on DIMM
  @param[in] TrackingMode  - 0 for Tracking Initialization
                           - 1 for Tracking Mode

  @retval N/A

**/
VOID
EFIAPI
ConfigureDbDqs2Dq (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    TrackingMode
  )
{
#ifdef LRDIMM_SUPPORT
  PSYSHOST                      Host;
  DDR5_DATA_BUFFER_RWB0_STRUCT  DataBufferRwB0;

  if (IsLrdimmPresent (Socket, Channel, Dimm) == TRUE) {
    Host = GetSysHostPointer ();

    DataBufferRwB0.Data = 0;
    DataBufferRwB0.Bits.tracking_mode = TrackingMode;
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RWB0_REG, DataBufferRwB0.Data);
  }
#endif // LRDIMM_SUPPORT
}

/**

  Configures DRAM tDQS2DQ Tracking Return Value.

  @param[in] Host          - Pointer to sysHost
  @param[in] Socket        - Socket number
  @param[in] Channel       - Channel number
  @param[in] Dimm          - DIMM number in Channel
  @param[in] Rank          - Rank number on DIMM
  @param[in] Value         - Value to be set

  @retval N/A

**/
VOID
ConfigureDbDqs2DqTrackingReturnValue (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Value
  )
{
#ifdef LRDIMM_SUPPORT
  DDR5_DATA_BUFFER_RWB1_STRUCT  Ddr5DataBuffterRWB1;

  if (IsLrdimmPresent (Socket, Channel, Dimm) == TRUE) {
    Ddr5DataBuffterRWB1.Data = 0;
    Ddr5DataBuffterRWB1.Bits.dram_tdqs2dq_tracking_return_value_1_2 = Value;
    SendMrw (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RWB1_REG, CW_RCD_DB, Ddr5DataBuffterRWB1.Data);
  }
#endif // LRDIMM_SUPPORT
}

/**

  Perform Periodic Tx Retraining

  @param[in] Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
EFIAPI
PeriodicTxRetrain (
  IN PSYSHOST Host
  )
{
  UINT8                         MaxChDdr;
  UINT8                         Socket;
  UINT8                         Ch;
  UINT8                         Dimm;
  UINT8                         Rank;
  UINT32                        Status;
  UINT16                        TxDqDly = 0;
  UINT16                        MinTxDqDly;
  BOOLEAN                       CapErrorFlagMr46;
  BOOLEAN                       CapErrorFlagMr47;
  UINT8                         ParityStatus;
  struct dimmNvram              (*DimmNvList)[MAX_DIMM];
  DDR5_MODE_REGISTER_45_STRUCT  Ddr5ModeRegister45;
  UINT8                         Strobe;
  UINT8                         Bit;
  UINT8                         MSVx4;
  UINT8                         (*Data)[SUB_CH][MAX_STROBE_DDR5 / 2];

  Ddr5ModeRegister45.Data = 0;
  MaxChDdr = GetMaxChDdr ();
  MSVx4 = GetMaxStrobeValid (Host);

  if (!IsMemFlowEnabled (PeriodicTxReTraining)) {
    return SUCCESS;
  }

  //
  // Workaround for the new memflow bit support in simics.
  // Once the memflow support fix in simics is checked in, remove the below check.
  //
  if (IsSiliconWorkaroundEnabled ("S1507757586")) {
    return SUCCESS;
  }

  Status = SUCCESS;
  Socket = Host->var.mem.currentSocket;

  //
  // Return if this Socket is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return SUCCESS;
  }

  SetCurrentTestType (Socket, PeriodicTxReTrainTest);

  DoCompDdrioPerSetupOption (Socket, TRAINING_COMP_EVERY_TRAINING);

  SetAepTrainingMode (Host, Socket, CHECKPOINT_MINOR_RX_BACKSIDE_PHASE_TRAINING);

  //
  // DDRIO Retrain Init.
  //
  ConfigureDdrioTxRetrain (Socket, 1, 1);

  //
  // Enable CAP.
  //
  EnableCAParityRuntime (Host, Socket);

  Data = RcAllocatePool (sizeof (*Data));
  if (Data == NULL) {
    return FAILURE;
  }
  ZeroMem ((UINT8 *) (Data), sizeof ((*Data)));

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }

        if (IsDcpmmPresentDimm (Socket, Ch, Dimm) == TRUE) {
          continue;
        }

        DimmNvList = GetDimmNvList (Host, Socket, Ch);

        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList)[Dimm].numRanks)) {
          continue;
        }

        //
        // Check minimum txdq_dly setting
        //
        for (Strobe = 0; Strobe < MSVx4; Strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
            continue;
          }

          for (Bit = 0; Bit < BITS_PER_NIBBLE; Bit++) {

            GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, Bit, DdrLevel, TxDqBitDelay, GSM_READ_ONLY, (INT16 *)&TxDqDly);

            if (IsMemFlowEnabled (CmdNormalization)) {
              MinTxDqDly = TX_RETRAIN_MIN_TXDQ_DLY_CMD_NORMAL_EN;
            } else {
              MinTxDqDly = TX_RETRAIN_MIN_TXDQ_DLY_CMD_NORMAL_DIS;
            }
            if (TxDqDly < MinTxDqDly) {
              RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, Bit,
                                      "WARNING: Tx ReTraining txdq_dly %d small than %d\n", TxDqDly, MinTxDqDly);
            }
          }
        }

        if (FeaturePcdGet (PcdCteBuild) == TRUE) {
          //
          // Init DRAM interface only for CTE.
          //
          InitDramInterface (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank);
        }

        // Configure data buffer tDQS2DQ tracking control word
        ConfigureDbDqs2Dq (Socket, Ch, Dimm, Rank, DQS2DQ_INITIALIZATION);

        ConfigureDbDqs2DqTrackingReturnValue (Host, Socket, Ch, Dimm, Rank, DQS2DQ_TRACKING_RETURN_VALUE_INITIALIZATION);

        //
        // Configure the DRAM DQS Oscillator Interval as 2048 clocks.
        //
        Ddr5ModeRegister45.Bits.dqs_interval_timer_run_time = DQS_INTERVAL_TIMER_RUN_TIME_2048_ENCODING;
        DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_45_REG, Ddr5ModeRegister45.Data);

        //
        // Enable Tx Retrain
        //
        ConfigureDdrioTxRetrainEnable (Socket, Ch, Dimm, Rank, TX_RETRAIN_ENABLE);
        //
        // Tell the iMC not to issue activates and precharges
        //
        SetMprModeFilterActPre (Host, Socket, Ch, !(*DimmNvList)[Dimm].DcpmmPresent);
        //
        // Start the Re Training
        //
        WriteMPC (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank, Start_DQS_Osc, TIMING_1N, TRUE);
        //
        // Wait for 2048 clocks
        //
        TrainingDelay (Host, DQS_INTERVAL_TIMER_RUN_TIME_2048 * 2, 10);

        //
        // Enable command type override and set the read command type encoding 1 for DDRIO snooping MR46
        //
        McCmdTypeOverrideEnableAndEncoding (Socket, Ch, CMD_TYPE_OVERRIDE_ENABLE, CMD_TYPE_OVERRIDE_MR46);

        //
        // Issue MRR to MR46, DDRIO will snoop LSB and record
        //
        ReadDramMr (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_46_REG, Data);

        //
        // Check for CAP
        //
        ParityStatus = ChipGetParityErrorStatus (Host, Socket, Ch, Dimm);
        if (ParityStatus != 0) {
          CapErrorFlagMr46 = TRUE;
          RcDebugPrintWithDevice (
            SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
            "ERROR: CAP error for MR46\n"
            );
        } else {
          CapErrorFlagMr46 = FALSE;
        }

        //
        // Enable command type override and set the read command type encoding 2 for DDRIO snooping MR47
        //
        McCmdTypeOverrideEnableAndEncoding (Socket, Ch, CMD_TYPE_OVERRIDE_ENABLE, CMD_TYPE_OVERRIDE_MR47);
        //
        // Issue MRR to MR47, DDRIO will snoop MSB and record
        //
        ReadDramMr (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_47_REG, Data);

        //
        // Check for CAP
        //
        ParityStatus = ChipGetParityErrorStatus (Host, Socket, Ch, Dimm);
        if (ParityStatus != 0) {
          CapErrorFlagMr47 = TRUE;
          RcDebugPrintWithDevice (
            SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
            "ERROR: CAP error for MR47\n"
            );
        } else {
          CapErrorFlagMr47 = FALSE;
        }

        if (CapErrorFlagMr46 || CapErrorFlagMr47) {
          //
          // If a CA Parity Error was observed, return a fatal error.
          //
          RcDebugPrintWithDevice (
            SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
            "Tx ReTraining Initialziation fails\n"
            );
          RcFreePool (Data);
          RC_FATAL_ERROR (FALSE, ERR_DDR_INIT, ERR_TX_RETRAIN_CAP);
          return FAILURE;
        } else {
          if (IsLrdimmPresent (Socket, Ch, Dimm) == TRUE) {
            // Wait for data buffer to perform calculations
            TrainingDelay (Host, DB_TRK_CALC_INIT * 2, 2);
          }
          //
          // Tx ReTrain PHY Update
          //
          TxReTrainDdrioPhyUpdate (Socket, Ch, Dimm, Rank);
          //
          // Disable the MC SPID Command Override.
          //
          McCmdTypeOverrideEnableAndEncoding (Socket, Ch, CMD_TYPE_OVERRIDE_DISABLE, CMD_TYPE_OVERRIDE_DEFAULT);
        }

      } // Ch loop
    } // Rank loop
  } // Dimm loop

  //
  // Clean up
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    SetMprModeFilterActPre (Host, Socket, Ch, 0);
  }
  //
  // Clear the TX Retrain Initialization Bit
  //
  ConfigureDdrioTxRetrain (Socket, 1, 0);

  //
  // Disable TX Retrain Enable
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }

        if (IsDcpmmPresentDimm (Socket, Ch, Dimm) == TRUE) {
          continue;
        }

        DimmNvList = GetDimmNvList (Host, Socket, Ch);

        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList)[Dimm].numRanks)) {
          continue;
        }

        ConfigureDdrioTxRetrainEnable (Socket, Ch, Dimm, Rank, TX_RETRAIN_DISABLE);

      } // Ch loop
    } // Rank loop
  } // Dimm loop

  FifoTrainReset (Host, Socket);
  RcFreePool (Data);

  return Status;
} // PeriodicTxRetrain
