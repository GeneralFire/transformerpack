/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2017 Intel Corporation. <BR>

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
#include <Upi/KtiHost.h>
#include "OemIioInitCommon.h"

/**
 Invoque the SystemIioPortBifurcationInit to update IioGloblaData
 with the curent IIO bifurcation acording to the board detected.

 @param  IioGlobalData  Pointer to the IIO Global for this driver.

 @retval None
**/
VOID
OemGetIioPortsBifurcation (
  IN OUT  IIO_GLOBALS  *IioGlobalData
  )
{
  return;
}

/**

  This function returns the Uplink port details from the IIO
               side, as per the platform board schematics, if no
               uplink port then return 0xFF for both IIO and
               port index.

  @param[in]   IioIndex - Index to Iio

  @param[out]  PortIndex - Index to Iio Port
  @param[out]  Bus       - Bus where uplink is located
  @param[out]  Device    - Device where uplink is located
  @param[out]  Function  - Function where uplink is located

 **/
VOID
OemIioUplinkPortDetails(
  IN   IIO_GLOBALS  *IioGlobalData,
  IN   UINT8   IioIndex,
  OUT  UINT8   *PortIndex,
  OUT  UINT8   *Bus,
  OUT  UINT8   *Dev,
  OUT  UINT8   *Func
  )
{
  UINT8       MaxPortNumberPerSocket;
  *PortIndex  = 0xFF;
  *Bus        = 0xFF;
  *Dev        = 0xFF;
  *Func       = 0xFF;

  if (IioIndex == 0) {
    *PortIndex = PcdGet8(PcdOemSkuUplinkPortIndex);
    if (*PortIndex != 0xFF) {
      MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];
      if (*PortIndex < ARRAY_SIZE (IioGlobalData->IioVar.IioVData.SocketPortBusNumber[IioIndex]) && (*PortIndex < MaxPortNumberPerSocket)) {
        *Bus        = IioGlobalData->IioVar.IioVData.SocketPortBusNumber[IioIndex][*PortIndex];
        *Dev        = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[*PortIndex].Device;
        *Func       = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[*PortIndex].Function;
      }
    }
  }
  return;
}

/**
  OEM Hook to check for particular support on IIO initialized devices and take action,
  such as install the proper protocol to indicate it is detected

 @param  IioGlobalData  Pointer to the IIO Global for this driver.

 @retval None
**/
VOID
OemCheckForOtherSupport(
   IN IIO_GLOBALS * IioGlobalData
  )
{
  EFI_STATUS   Status;
  EFI_HANDLE   VmdHandle = NULL;
  EFI_HANDLE   HfiPcieGen3Handle = NULL;
  BOOLEAN      VmdGlobalEnabled = FALSE;
  BOOLEAN      HfiPresent = FALSE;
  UINT8        IioIndex = 0;
  UINT8        StackIndex = 0;

  //
  // Check if HFI SKU is present and VMD is enabled globally
  //
  for (IioIndex = 0; IioIndex < MAX_SOCKET; IioIndex++) {
    if (IioGlobalData->IioVar.IioVData.SkuPersonality[IioIndex] == TYPE_MCP) {
      HfiPresent = TRUE;
    }

    for (StackIndex = 0; StackIndex < MAX_VMD_STACKS_PER_SOCKET; StackIndex++) {
      if (IioGlobalData->SetupData.VMDEnabled[IioIndex][StackIndex]) {
        VmdGlobalEnabled = TRUE;
      }
    }
  }

  //
  //  Add VMD driver OpRom in BIOS
  //  Install VMD Dummy protocol to indicate VMD enabled
  //
  if (VmdGlobalEnabled) {
    Status = gBS->InstallProtocolInterface (
               &VmdHandle,
               &gEfiVMDDriverProtocolGuid,
               EFI_NATIVE_INTERFACE,
               NULL
               );

    if (EFI_ERROR(Status)){
      DEBUG ((DEBUG_ERROR, "VMD EFI Driver GUID NOT installed = %x!!!\n", Status));
    } else {
      DEBUG ((DEBUG_INFO, "VMD EFI Driver GUID installed successfully = %x!\n", Status));
    }
  }

  //
  // Install HFI protocol to indicate it is detected.
  //
  if (HfiPresent) {
    Status = gBS->InstallProtocolInterface (
               &HfiPcieGen3Handle,
               &gEfiHfiPcieGen3ProtocolGuid,
               EFI_NATIVE_INTERFACE,
               NULL
               );

    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_ERROR, "HfiPcieGen3H EFI Driver GUID NOT installed = %x!!!\n", Status));
    } else {
      DEBUG ((DEBUG_INFO, "HfiPcieGen3H EFI Driver GUID installed successfully = %x!\n", Status));
    }
  }

}


/**
  This function update the HidePEXPMenu based on PciePort present values and
  do extra setup based on the IIO Features enabled.

  @param IioGlobalData     - Pointer to IioGlobalData

  @retval None
**/
VOID
OemUpdateHidePCIePortMenu (
  IN  IIO_GLOBALS  *IioGlobalData
  )
{
  EFI_STATUS Status;
  UINT8      HidePEXPMenuOption[MAX_TOTAL_PORTS];

  CopyMem (
    (VOID *)&HidePEXPMenuOption,
    (VOID *)&(IioGlobalData->SetupData.HidePEXPMenu),
    sizeof(HidePEXPMenuOption)
    );
  Status = SetOptionData (&gEfiSocketIioVariableGuid, OFFSET_OF(SOCKET_IIO_CONFIGURATION, HidePEXPMenu), &HidePEXPMenuOption, sizeof(HidePEXPMenuOption));

}

/**
  This function update the VtdSupport setup item based on x2Apic status from CPU side.
  If CPU threads > 256, x2Apic should be enabled thus Vtd need to be enabled, either.

  @param IioGlobalData     - Pointer to IioGlobalData

  @retval None
**/
VOID
OemUpdateX2ApicSetupItems (
  IN  IIO_GLOBALS  *IioGlobalData
  )
{
  UINT8         VTdSupportNvm;
  UINT8         ProcessorX2apicNvm;

  EFI_STATUS    Status;

  Status = GetOptionData (&gEfiSocketIioVariableGuid, OFFSET_OF (SOCKET_IIO_CONFIGURATION, VTdSupport), &VTdSupportNvm, sizeof(UINT8));
  if (EFI_ERROR (Status)) {
    //Not able to read Setup, initialize to default
    VTdSupportNvm = 0;
    DEBUG ((EFI_D_ERROR, "Fail to get VTdSupport NV Variable!(0x%x)\n", Status));
  }
  if (VTdSupportNvm != IioGlobalData->SetupData.VTdSupport) {
    Status = SetOptionData (&gEfiSocketIioVariableGuid, OFFSET_OF(SOCKET_IIO_CONFIGURATION, VTdSupport), &(IioGlobalData->SetupData.VTdSupport), sizeof(UINT8));
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Fail to update VTdSupport NV Variable!(0x%x)\n", Status));
    }
  }

  Status = GetOptionData (&gEfiSocketProcessorCoreVarGuid, OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, ProcessorX2apic), &ProcessorX2apicNvm, sizeof(UINT8));
  if (EFI_ERROR (Status)) {
    //Not able to read Setup, initialize to default
    ProcessorX2apicNvm = 0;
    DEBUG ((EFI_D_ERROR, "Fail to get ProcessorX2apic NV Variable!(0x%x)\n", Status));
  }
  if ( ProcessorX2apicNvm != IioGlobalData->SetupData.ProcessorX2apic) {
    Status = SetOptionData (&gEfiSocketProcessorCoreVarGuid, OFFSET_OF(SOCKET_PROCESSORCORE_CONFIGURATION, ProcessorX2apic), &(IioGlobalData->SetupData.ProcessorX2apic), sizeof(UINT8));
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Fail to update ProcessorX2apic NV Variable!(0x%x)\n", Status));
      return;
    }
  }
}
