/** @file NfitTableUpdate.c
  Implementation of NfitTableUpdateProtocol

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

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
**/

#include <PiDxe.h>
#include <Protocol/NfitTableUpdateProtocol.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include "NfitTableUpdate.h"
#include "JedecNvdimm.h"
#include "Nfit.h"

EFI_NFIT_TABLE_UPDATE_PROTOCOL mNfitTableUpdateProtocol;

/**
  @brief Build NVDIMM Firmware Interface Tables (NFIT) in ACPI tables

  @param[in,out] NfitTablePointer  pointer to NFIT in ACPI tables

  @retval EFI_SUCCESS          operation completed successfully
  @retval EFI_NOT_FOUND        no NVDIMM found

**/
EFI_STATUS
EFIAPI
UpdateAcpiTable (
  IN OUT UINT64 *NfitTablePointer
  )
{
  UINTN                JedecNvdimmTableSize;
  EFI_STATUS           Status;

  JedecNvdimmTableSize = sizeof(NVDIMM_FW_INTERFACE_TABLE);
  Status = EFI_SUCCESS;
  //
  // Build the actual NFIT table with needed sub-tables
  //
  Status = BuildNfitTable(NfitTablePointer, JedecNvdimmTableSize, mPmemRegions);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Nfit: BuildNfitTable failed, status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return Status;
}



EFI_STATUS
InstallNfitTableUpdateProtocol (
  IN EFI_HANDLE        ImageHandle
)
{
  EFI_STATUS Status;

  mNfitTableUpdateProtocol.UpdateAcpiTable = UpdateAcpiTable;

  Status = gBS->InstallProtocolInterface (
           &ImageHandle,
           &gEfiNfitTableUpdateProtocolGuid,
           EFI_NATIVE_INTERFACE,
           &mNfitTableUpdateProtocol
           );

  return EFI_SUCCESS;
}
