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

#include <Library/CpuCsrAccessLib.h>
#include "Include/MemMcRegs.h"
#include <Library/MemMcIpRasLib.h>
#include <SiliconSetting.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/PcuMailBoxRasLib.h>
#include <Library/CpuAndRevisionLib.h>

/**
  Enable Interrupt for DDRT Alerts

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.

  @retval None
**/
VOID
EFIAPI
EnableInterruptForDdrtAlerts (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt
  )
{
  DDRT_MCA_CTL_MC_2LM_STRUCT  DdrtMcaCtlStruct;

  DdrtMcaCtlStruct.Data = ReadCpuCsr (Skt, ChOnSkt, DDRT_MCA_CTL_MC_2LM_REG);
  DdrtMcaCtlStruct.Bits.dis_pending_intr_error = 0;
  WriteCpuCsr (Skt, ChOnSkt, DDRT_MCA_CTL_MC_2LM_REG, DdrtMcaCtlStruct.Data);
}

/**
  Set threshold for number of times UC error is retried by FMC

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  CntThreshold    threshold value to be programmed.

  @retval None
**/
VOID
EFIAPI
SetDdrtRetryUcErrorCounterThreshold (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT32  CntThreshold
  )
{
  DDRT_RETRY_UC_ERROR_COUNTER_MCDDC_DP_STRUCT  DdrtRetryUcErrorCounter;

  DdrtRetryUcErrorCounter.Data = ReadCpuCsr (Skt, ChOnSkt, DDRT_RETRY_UC_ERROR_COUNTER_MCDDC_DP_REG);
  DdrtRetryUcErrorCounter.Bits.cnt_threshold = CntThreshold;
  WriteCpuCsr (Skt, ChOnSkt, DDRT_RETRY_UC_ERROR_COUNTER_MCDDC_DP_REG, DdrtRetryUcErrorCounter.Data);
}

/**
  Set threshold for number of retries performed on a Corrected error before attempting to correct with ECC
  This is applicable in ECC mode A operation

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket
  @param[in]  CntThreshold    threshold value to be programmed.

  @retval None
**/
VOID
EFIAPI
SetDdrtEccModeAThreshold (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT32  CntThreshold
  )
{
  DDRT_ECC_MODE_A_INFO_MC_2LM_STRUCT  DdrtEccModeAInfo;
  DdrtEccModeAInfo.Data = ReadCpuCsr (Skt, ChOnSkt, DDRT_ECC_MODE_A_INFO_MC_2LM_REG);
  DdrtEccModeAInfo.Bits.cnt_threshold = CntThreshold;
  WriteCpuCsr (Skt, ChOnSkt, DDRT_ECC_MODE_A_INFO_MC_2LM_REG, DdrtEccModeAInfo.Data);
}


/**
  Set threshold for number of retries performed on a Corrected error before attempting to correct with ECC
  This is applicable in ECC mode A operation

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket
  @param[in]  ModeInfo        Mode A vs Mode B values to be programmed

  @retval None
**/
VOID
EFIAPI
SetDdrtEccMode (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT32  ModeInfo
  )
{
  DDRT_ECC_MODE_MC_2LM_STRUCT  DdrtEccMode;
  DdrtEccMode.Data = ReadCpuCsr (Skt, ChOnSkt, DDRT_ECC_MODE_MC_2LM_REG);
  DdrtEccMode.Bits.em = ModeInfo;
  WriteCpuCsr (Skt, ChOnSkt, DDRT_ECC_MODE_MC_2LM_REG, DdrtEccMode.Data);
}

/**
  Get DDRT ECC Mode information for this channel

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket

  @return  DdrtEccMode
**/

UINT32
EFIAPI
GetDdrtEccMode (
  IN   UINT8   Skt,
  IN   UINT8   ChOnSkt
  )
{
  DDRT_ECC_MODE_MC_2LM_STRUCT  DdrtEccMode;

  DdrtEccMode.Data = ReadCpuCsr (Skt, ChOnSkt, DDRT_ECC_MODE_MC_2LM_REG);
  return DdrtEccMode.Bits.em;
}

/**
  Set Ddrtdata path delay to failed nibble and enable force correction

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket
  @param[in]  FailedNibble    Failed nibble/device to be programmed
  @param[in]  ForceCorrection value to be programmed into ddrt_force_correction

  @retval None
**/
VOID
EFIAPI
SetDdrtDataPathDelay (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT8   FailedNibble,       OPTIONAL
  IN  UINT8   ForceCorrection     OPTIONAL
  )
{
  DDRT_DATAPATH_DELAY_MCDDC_DP_STRUCT  DdrtDataPathDelay;

  DdrtDataPathDelay.Data = ReadCpuCsr (Skt, ChOnSkt, DDRT_DATAPATH_DELAY_MCDDC_DP_REG);
  if (FailedNibble != 0xFF) {
    DdrtDataPathDelay.Bits.failed_nibble_number = FailedNibble;
  }
  if (ForceCorrection != 0xFF) {
    DdrtDataPathDelay.Bits.ddrt_force_correction = ForceCorrection;
  }
  WriteCpuCsr (Skt, ChOnSkt, DDRT_DATAPATH_DELAY_MCDDC_DP_REG, DdrtDataPathDelay.Data);
}

/**
  Program DDRTCnstrnt.ddr_wrwr_s with value passed

  @param[in]  Socket                   - Socket ID
  @param[in]  ChOnSkt                  - Channel on socket
  @param[in]   DdrtWrWrS                - value to prgram in t_ddrt_wrwr_s

  @retval None
**/
VOID
EFIAPI
SetDdrtCnsTrnt (
  IN   UINT8    Socket,
  IN   UINT8    ChOnSkt,
  IN   UINT32   DdrtWrWrS
  )
{
  T_DDRT_WR_CNSTRNT_MC_2LM_STRUCT            DdrtWrCnstrnt;
  UINT32                                     Command;
  UINT8                                      Node;

  DdrtWrCnstrnt.Data      = ReadCpuCsr (Socket, ChOnSkt, T_DDRT_WR_CNSTRNT_MC_2LM_REG);
  DdrtWrCnstrnt.Bits.t_ddrt_wrwr_s = DdrtWrWrS;

  if (IsSiliconWorkaroundEnabled ("S14010263993")) {
    Node = SKT_CH_TO_NODE(Socket,ChOnSkt);
    if (!IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
      Command = MAILBOX_BIOS_CR_WRITE_CMD (NODE_TO_MC(Node), SKTCH_TO_NODECH (ChOnSkt), MAILBOX_BIOS_CMD_CR_PROXY_T_DDRT_WR_CNSTRNT_INDEX);
      ReadWritePcuMailbox (Socket, MAILBOX_BIOS_CMD_ZERO, Command, &DdrtWrCnstrnt.Data, 0, 0);
    }
  } else {
    WriteCpuCsr (Socket, ChOnSkt, T_DDRT_WR_CNSTRNT_MC_2LM_REG, DdrtWrCnstrnt.Data);
  }
}

/**

  @brief Programs Error Signaling for McDdrt Errors.
  Note that DDRT_FNV_INTR is used to select error signaling, not DDRT_FNV0/1_EVENT.

  @param[in] Socket      - Socket index at system level
  @param[in] Ch          - Channel index at socket level
  @param[in] Dimm        - DIMM slot index within DDR channel
  @param[in] Priority    - 0:Low Priotity, 1:High Priority.
                           There are two buckets of errors. Each bucket can be
                           programmed to a particular signal scheme.
  @param[in] Signal      - BIT0 - SMI, BIT1 - ERROR0#, BIT2 - MSMI#
                           (all mutually exclusive)

  @retval EFI_SUCCESS           - signal programmed successfully
  @retval EFI_INVALID_PARAMETER - given DIMM is not NGN DIMM

**/
EFI_STATUS
EFIAPI
McDdrtErrorLogProgramSignal (
  UINT8 Socket,
  UINT8 Ch,
  UINT8 Dimm,
  UINT8 Priority,
  UINT8 Signal
  )
{
  DDRT_FNV_INTR_CTL_MCDDC_DP_STRUCT   FnvIntrCtrl;

  if ((Priority != DDRT_SIGNAL_LOW_PRIORITY_EVENT) && (Priority != DDRT_SIGNAL_HIGH_PRIORITY_EVENT)) {
    return EFI_INVALID_PARAMETER;
  }

  if (Signal == 0) {
    return EFI_SUCCESS;
  }

  FnvIntrCtrl.Data = ReadCpuCsr (Socket, Ch, DDRT_FNV_INTR_CTL_MCDDC_DP_REG);

  if (Priority == DDRT_SIGNAL_LOW_PRIORITY_EVENT) {
    if (Dimm == 0) {
      FnvIntrCtrl.Bits.slot0_lo_smi_en  = (Signal & DDRT_SIGNAL_SMI_FLAG) ? 1 : 0;
      FnvIntrCtrl.Bits.slot0_lo_err0_en = (Signal & DDRT_SIGNAL_ERROR0_FLAG)   ? 1 : 0;
    } else {
      FnvIntrCtrl.Bits.slot1_lo_smi_en  = (Signal & DDRT_SIGNAL_SMI_FLAG) ? 1 : 0;
      FnvIntrCtrl.Bits.slot1_lo_err0_en = (Signal & DDRT_SIGNAL_ERROR0_FLAG)   ? 1 : 0;
    }
  } else {
    if (Dimm == 0) {
      FnvIntrCtrl.Bits.slot0_hi_smi_en  = (Signal & DDRT_SIGNAL_SMI_FLAG) ? 1 : 0;
      FnvIntrCtrl.Bits.slot0_hi_err0_en = (Signal & DDRT_SIGNAL_ERROR0_FLAG)   ? 1 : 0;
    } else {
      FnvIntrCtrl.Bits.slot1_hi_smi_en  = (Signal & DDRT_SIGNAL_SMI_FLAG) ? 1 : 0;
      FnvIntrCtrl.Bits.slot1_hi_err0_en = (Signal & DDRT_SIGNAL_ERROR0_FLAG)   ? 1 : 0;
    }
  }

  WriteCpuCsr (Socket, Ch, DDRT_FNV_INTR_CTL_MCDDC_DP_REG, FnvIntrCtrl.Data);

  return EFI_SUCCESS;
}

/**

  @brief Detects the NGN DIMM error and returns 24-bit log.

  @param[in] Socket         - Socket index at system level
  @param[in] Ch             - Channel index at socket level
  @param[in] Dimm           - DIMM slot index within DDR channel
  @param[in] Priority       - 0 for low priority event, 1 for high
  @param[in] ClearOnRead    - when set, clears the error when a NGN DIMM
                              is found and reported
  @param[out] *EventData    - DDRT_EVENT_DATA outcome outcome

  @retval EFI_SUCCESS             - A NGN DIMM with error was found, pLog updated
  @retval EFI_INVALID_PARAMETER   - Given DIMM is not NGN DIMM or Log is NULL
  @retval EFI_NOT_FOUND           - No error found on NGN DIMM, Log = 0

**/
EFI_STATUS
EFIAPI
McDdrtErrorLogDetectError (
  UINT8   Socket,
  UINT8   Ch,           // at socket level
  UINT8   Dimm,         // with in channel
  UINT8   Priority,
  BOOLEAN ClearOnRead,
  DDRT_EVENT_DATA  *EventData
  )
{
  UINT8                           MaxChDdr;
  UINT32                          RegOffset;
  DDRT_FNV0_EVENT0_MC_2LM_STRUCT  FnvEvent;
  UINT32                          Command;
  UINT8                           Node;

  MaxChDdr = GetMaxChDdr();

  if ((EventData == NULL) ||
      ((Priority != DDRT_SIGNAL_LOW_PRIORITY_EVENT) && (Priority != DDRT_SIGNAL_HIGH_PRIORITY_EVENT)) ||
      (Ch >= MaxChDdr)) {
    return EFI_INVALID_PARAMETER;
  }

  EventData->Data = 0;

  if (Dimm == 0) {
    RegOffset = (Priority == DDRT_SIGNAL_LOW_PRIORITY_EVENT) ? DDRT_FNV0_EVENT0_MC_2LM_REG : DDRT_FNV0_EVENT1_MC_2LM_REG;
  } else {
    RegOffset = (Priority == DDRT_SIGNAL_LOW_PRIORITY_EVENT) ? DDRT_FNV1_EVENT0_MC_2LM_REG : DDRT_FNV1_EVENT1_MC_2LM_REG;
  }

  FnvEvent.Data = ReadCpuCsr (Socket, Ch, RegOffset);

  if (!FnvEvent.Bits.valid) {
    return EFI_NOT_FOUND;
  }

  EventData->Bits.Log = FnvEvent.Bits.log;
  EventData->Bits.Overflow = FnvEvent.Bits.overflow;
  EventData->Bits.ErrorId = FnvEvent.Bits.id;

  if (ClearOnRead) {
    FnvEvent.Bits.log = 0;
    FnvEvent.Bits.id  = 0;

    if (IsSiliconWorkaroundEnabled ("S14010263993")) {
      if (Dimm == 0) {
        RegOffset = (Priority == DDRT_SIGNAL_LOW_PRIORITY_EVENT) ? MAILBOX_BIOS_CMD_CR_PROXY_DDRT_FNV0_EVENT0_INDEX : MAILBOX_BIOS_CMD_CR_PROXY_DDRT_FNV0_EVENT1_INDEX;
      } else {
        RegOffset = (Priority == DDRT_SIGNAL_LOW_PRIORITY_EVENT) ? MAILBOX_BIOS_CMD_CR_PROXY_DDRT_FNV1_EVENT0_INDEX : MAILBOX_BIOS_CMD_CR_PROXY_DDRT_FNV1_EVENT1_INDEX;
      }
      Node = SKT_CH_TO_NODE(Socket,Ch);
      Command = MAILBOX_BIOS_CR_WRITE_CMD (NODE_TO_MC(Node), SKTCH_TO_NODECH (Ch), RegOffset);
      ReadWritePcuMailbox (Socket, MAILBOX_BIOS_CMD_ZERO, Command, &FnvEvent.Data, 0, 0);
    } else {
      WriteCpuCsr (Socket, Ch, RegOffset, FnvEvent.Data);
    }
  }

  return EFI_SUCCESS;
}

