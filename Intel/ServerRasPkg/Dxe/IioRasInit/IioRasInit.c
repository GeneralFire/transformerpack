/** @file
  Implement IIO initialization protocol to provide IIO initialization functions.
  This interfaces will be called by PciHostBridge driver in the PCI bus enumeration.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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
#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Protocol/PciCallback.h>
#include <IioPlatformData.h>
#include <Library/IioRasInitLib.h>
#include <Library/IioDataHubLib.h>

BOOLEAN    mIioRasInitCallBackPciScanningDone = FALSE;
/**

  Call back routine used to do all IIO RAS configuration before PCI Scan

  @param IioGlobalData  - Pointer to IIO_GLOBALS

  @retval None
**/
VOID
EFIAPI
IioRasInitCallBackPciScanning (
  IN  EFI_HANDLE                                   RootBridgeHandle,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS  Address,
  IN  EFI_PCI_ENUMERATION_PHASE                    PciPhase,
  IN  EFI_PCI_CALLBACK_CONTEXT                     *Context
  )
{

  EFI_STATUS                    Status = EFI_SUCCESS;


  if (mIioRasInitCallBackPciScanningDone) {
      return;
  }

  Status = IioRasInitBeforePciScan ( );

  mIioRasInitCallBackPciScanningDone = TRUE;
}

/**

  Install the IIO RAS Initialization

  @param[in] ImageHandle  - ImageHandle of the loaded driver
  @param[in] SystemTable  - Pointer to the BIOS System Table

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
EFIAPI
IioRasInitEntry (
  IN EFI_HANDLE                 ImageHandle,
  IN EFI_SYSTEM_TABLE           *SystemTable
  )
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  EFI_PCI_CALLBACK_PROTOCOL     *PciCallBackProtocol;

  DEBUG ((DEBUG_INFO, "IioRasInit DXE Driver\n"));

  Status = gBS->LocateProtocol (
                   &gEfiPciCallbackProtocolGuid,
                   NULL,
                   &PciCallBackProtocol
                   );
  ASSERT_EFI_ERROR (Status);

  Status = PciCallBackProtocol->RegisterPciCallback(
                    PciCallBackProtocol,
                    IioRasInitCallBackPciScanning,
                    EfiPciEnumerationDeviceScanning
                    );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

