/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <Library/MemoryServicesLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/HbmCoreLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/TimerLib.h>

/**
  This function detects the HBM frequency via B2P mailbox.

  @param[in] SocketId           Socket index.

  @retval MRC_STATUS_SUCCESS    This function is executed successfully.
  @retval MRC_STATUS_FAILURE    This function is failed to be executed.
**/
MRC_STATUS
EFIAPI
HbmDetectFrequency (
  IN UINT8         SocketId
  )
{
  UINT8            HbmIoId;
  UINT8            MaxHbmIo;
  UINT8            RefClk;
  UINT8            QClkRatio;
  UINT8            MaxFreq;
  UINT8            MemSsBitMap;
  UINT16           HbmFreqInMhz;
  BOOLEAN          SendRampBit;
  EFI_STATUS       Status;

  MaxHbmIo = GetMaxIoHbm ();
  MemSsBitMap = HbmDetected (SocketId);
  if (MemSsBitMap == 0) {
    HbmDebugPrint (HBM_SDBG_MIN, "Skip HBM frequency detection for socket %d as no HBM SS enabled\n", SocketId);
    return MRC_STATUS_SUCCESS;
  }

  RefClk = 1;             // HBM always use refclk 100Mhz.
  QClkRatio = 32;         // This input is ignored and HBM ratio is extracted from fuses.
  SendRampBit = FALSE;    // DDR code will set the ramp bit with/without DDR DIMM populated.

  Status = PcuSetMCFreq (SocketId, RefClk, QClkRatio, PCU_MEM_HBM, MemSsBitMap, SendRampBit);
  if (EFI_ERROR (Status)) {
    HbmDebugPrint (HBM_SDBG_MINMAX, "Failed to issue SET_MC_FREQ B2P CMD on socket %d, status = %r\n", SocketId, Status);
    return MRC_STATUS_FAILURE;
  }

  //
  // Wait for HBM MCs to be brought up.
  //
  NanoSecondDelay (30);

  QClkRatio = 0;
  for (HbmIoId = 0; HbmIoId < MaxHbmIo; HbmIoId++) {
    if (IsHbmMemSsEnabled(SocketId, HbmIoId)) {
      Status = PcuReadMcFreq (SocketId, PCU_MEM_HBM, (BIT0 << HbmIoId), &RefClk, &QClkRatio, &MaxFreq);
      if (!EFI_ERROR (Status)) {
        break;     // Assume HBM frequency is the same for all HBM memory subsystems.
      }
    }
  }

  if (QClkRatio == 0) {
    HbmDebugPrint (HBM_SDBG_MINMAX, "Failed to detect HBM frequency on socket %d\n", SocketId);
    RC_FATAL_ERROR (FALSE, ERR_READ_MC_FREQ, ERR_NOT_ABLE_READ_MC_FREQ);
  }

  //
  // HBM always use refclk 100Mhz.
  //
  HbmFreqInMhz = QClkRatio * 100;
  HbmDebugPrint (HBM_SDBG_MIN, "HBM frequency = %dMHz\n", HbmFreqInMhz);

  //
  // Save the HBM frequency into HBM Host.
  // HBM can only support 3.2GHz and 2.8GHz.
  //
  if (HbmFreqInMhz == 2800) {
    HbmSetFrequency (SocketId, HBM2E_2p8);
  } else {
    HbmSetFrequency (SocketId, HBM2E_3p2);
  }

  return MRC_STATUS_SUCCESS;
}
