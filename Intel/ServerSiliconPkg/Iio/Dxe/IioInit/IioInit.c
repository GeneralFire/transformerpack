/** @file
  Implement IIO initialization protocol to provide IIO initialization functions.
  This interfaces will be called by PciHostBridge driver in the PCI bus enumeration.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2019 Intel Corporation. <BR>

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
#include <IioPlatformData.h>
#include <Library/OemIioInit.h>
#include <Library/IioInitLib.h>
#include <Library/IioInitDxeLib.h>
#include <Protocol/IioSystem.h>
#include <Protocol/IioUds.h>
#include <Protocol/CpuCsrAccess.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/IioNtbInit.h>
#include <Library/IioDataHubLib.h>

#include <LibraryPrivate/IioDebug.h>

EFI_IIO_SYSTEM_PROTOCOL       mIioSystemProtocol;
EFI_HANDLE                    mImageHandle;


/**
  This function is Iio Workarounds interface

  @param[in] IioGlobalData IIO global data structure
**/

VOID
IioLateWorkarounds (
  IN IIO_GLOBALS              *IioGlobalData
  )
{
  NtbLateWorkarounds (IioGlobalData);
}

/**

  Install the IIO Initialization

  @param[in] ImageHandle  - ImageHandle of the loaded driver
  @param[in] SystemTable  - Pointer to the BIOS System Table

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
EFIAPI
IioInitEntry (
  IN EFI_HANDLE                 ImageHandle,
  IN EFI_SYSTEM_TABLE           *SystemTable
  )
{
  IIO_GLOBALS                   *IioGlobalData;
  EFI_HANDLE                    IioSystemProtocolHandle = NULL;
  EFI_STATUS                    Status = EFI_SUCCESS;

  mImageHandle = ImageHandle;

  Status = GetIioGlobalData (&IioGlobalData);
  ASSERT_EFI_ERROR (Status);

  if (UbiosGenerationEnabled ()) {
    DEBUG ((EFI_D_ERROR, "IIO CSR Trace start\n"));
  }

  Status = IioPostLateInitialize (IioGlobalData);
  ASSERT_EFI_ERROR (Status);

  Status = IioSecurePlatform (IioGlobalData);
  ASSERT_EFI_ERROR (Status);

  //
  // Install Dummy protocols for VMD and HFI support
  //
  OemCheckForOtherSupport (IioGlobalData);
  OemUpdateHidePCIePortMenu (IioGlobalData);
  OemUpdateX2ApicSetupItems (IioGlobalData);

  IioLateWorkarounds (IioGlobalData);

  //
  // Build the IIO_SYSTEM_PROTOCOL driver instance for protocol publishing
  //
  ZeroMem (&mIioSystemProtocol, sizeof (EFI_IIO_SYSTEM_PROTOCOL));
  mIioSystemProtocol.IioGlobalData = IioGlobalData;
  mIioSystemProtocol.IioGetCpuUplinkPort = IioGetCpuUplinkPort;

  //
  // Install IIO System Protocol
  //
  Status = gBS->InstallProtocolInterface (
                    &IioSystemProtocolHandle,
                    &gEfiIioSystemProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mIioSystemProtocol
                    );
  ASSERT_EFI_ERROR (Status);

  if (UbiosGenerationEnabled ()) {
    DEBUG ((EFI_D_ERROR, "IIO CSR Trace end\n"));
  }

  return Status;
}

