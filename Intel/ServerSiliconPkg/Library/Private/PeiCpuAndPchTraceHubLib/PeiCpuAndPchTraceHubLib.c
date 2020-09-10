/** @file
  This code provides common functions for CPU and PCH Trace Hub.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/
#include <Base.h>
#include <IndustryStandard/Pci22.h>
#include <PeiCpuAndPchTraceHubLibInternal.h>
#include <Library/PciSegmentLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Private/Library/PchPsfPrivateLib.h>
#include <Library/PeiServicesLib.h>
#include <IioPlatformData.h>
#include <RcRegs.h>
#include <Library/IioDataHubLib.h>
#include <Library/CommonTraceHubLib.h>

TRACE_HUB_DEVICE_INFO TraceHubDeviceTable[] = {
  {IIO_TRACE_HUB_PCI_DEVICE_NUMBER, IIO_TRACE_HUB_PCI_FUNCTION_NUMBER},
  {PCI_DEVICE_NUMBER_PCH_TRACE_HUB, PCI_FUNCTION_NUMBER_PCH_TRACE_HUB}
};

/*
  Secure steps to ensure trusted Trace Hub buffers BAR programming.

  @returns  Standard EFI_STATUS is returned.
*/
EFI_STATUS
TraceHubBuffersSecureSteps (
  VOID
  )
{
  EFI_STATUS                   Status;
  IIO_GLOBALS                  *IioGlobalData;
  UINT8                        IioIndex;
  UINT8                        TraceHubInSocket;
  UINT8                        BusNumber;
  UINT8                        Stack;
  UINT32                       MtbBarLow;
  TRACE_HUB_CONFIG             *PchTraceHubConfig;
  BOOLEAN                     PchTraceHubHidden;

  DEBUG ((DEBUG_INFO, "[IIO](TH) %a\n", __FUNCTION__));

  PchTraceHubHidden = FALSE;

  PeiPchTraceHubGetConfig (&PchTraceHubConfig);

  Status = GetIioGlobalData (&IioGlobalData);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  if (!IioIsDevicePresent (0, PCH_NPK_BUS, TraceHubDeviceTable[PchTraceHub].DevNum, TraceHubDeviceTable[PchTraceHub].FuncNum, NULL)) {
    PsfUnhideTraceHubDevice ();
    if (IioIsDevicePresent (0, PCH_NPK_BUS, TraceHubDeviceTable[PchTraceHub].DevNum, TraceHubDeviceTable[PchTraceHub].FuncNum, NULL)) {
      DEBUG ((DEBUG_ERROR, "[IIO](TH) Unhiding before configuration.\n"));
    } else {
      DEBUG ((DEBUG_ERROR, "[IIO](TH) PCH Trace Hub not accessible\n"));
      PchTraceHubHidden = TRUE;
    }
  }

  if (!PchTraceHubHidden) {
    MtbBarLow = GetTraceHubMtbLBar (PchTraceHub, PCH_NPK_BUS);
    if (MtbBarLow == 0 || GetTraceHubMtbUBar (PchTraceHub, PCH_NPK_BUS) != 0) {
      DEBUG ((DEBUG_ERROR, "[IIO](TH) [0.0 Bus: %x] ERROR: PCH Trace Hub MTB_LBAR = 0\n", PCH_NPK_BUS));
      ASSERT (GetTraceHubMtbUBar (PchTraceHub, PCH_NPK_BUS) == 0);
      return EFI_NOT_READY;
    }

    PciSegmentOr8 (GetTraceHubPciBase (PchTraceHub, PCH_NPK_BUS) + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER);

    DEBUG ((DEBUG_INFO, "[IIO](TH) PCH lock buffers BAR access\n"));
    TraceHubLockBuffersBar (*PchTraceHubConfig, MtbBarLow);
  }

  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {
    for (Stack = FIRST_TRACE_HUB_STACK_NUMBER; Stack < FIRST_TRACE_HUB_STACK_NUMBER + NUMBER_TRACE_HUB_PER_SOCKET; Stack++) {
      if (!IioIsStackPresent (IioIndex, Stack)) {
        continue;
      }
      BusNumber = IioTraceHubGetPciBusNo (CpuTraceHub, IioIndex, Stack);

      if (!IioIsDevicePresent (IioIndex, BusNumber, TraceHubDeviceTable[CpuTraceHub].DevNum, TraceHubDeviceTable[CpuTraceHub].FuncNum, NULL)) {
        DEBUG ((DEBUG_ERROR, "[IIO](TH) [%d.%d] Config Space of CPU Trace Hub device is hidden.\n", IioIndex, Stack));
        continue;
      }

      MtbBarLow = GetTraceHubMtbLBar (CpuTraceHub, BusNumber);
      if (MtbBarLow == 0 || GetTraceHubMtbUBar (CpuTraceHub, BusNumber) != 0) {
        DEBUG ((DEBUG_ERROR, "[IIO](TH) [%d.%d Bus: %x] ERROR MTB_BAR = 0. Returning EFI_NOT_READY\n", IioIndex, Stack, BusNumber));
        ASSERT (GetTraceHubMtbUBar (CpuTraceHub, BusNumber) == 0);
        return EFI_NOT_READY;
      }
      TraceHubInSocket = Stack - FIRST_TRACE_HUB_STACK_NUMBER;

      PciSegmentOr8 (GetTraceHubPciBase (CpuTraceHub, BusNumber) + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER);

      DEBUG ((DEBUG_INFO, "[IIO](TH) [%d.%d Bus: %x] CPU lock buffers BAR access\n", IioIndex, Stack, BusNumber));
      TraceHubLockBuffersBar (IioGlobalData->SetupData.CpuTraceHubConfig[IioIndex][TraceHubInSocket], MtbBarLow);

    }
  }
  return EFI_SUCCESS;
}