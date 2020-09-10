/** @file NfitTableUpdate.c
  Implementation of NfitTableUpdateProtocol

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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
#include "CrystalRidge.h"
#include "Nfit.h"

EFI_NFIT_TABLE_UPDATE_PROTOCOL mNfitTableUpdateProtocol;


/**
  @brief Build NVDIMM Firmware Interface Tables (NFIT) in ACPI tables

  @param[in,out] NfitTablePtr  pointer to NFIT in ACPI tables

  @retval EFI_SUCCESS          operation completed successfully
  @retval EFI_NOT_FOUND        no NVDIMM found

**/
EFI_STATUS
EFIAPI
UpdateAcpiTable (
  IN OUT UINT64 *NfitTablePtr
  )
{
  EFI_STATUS                Status = EFI_SUCCESS;
  EFI_HANDLE                Handle = NULL;
  UINTN                     NfitTableSize = sizeof (NVDIMM_FW_INTERFACE_TABLE);

  if (!IsSystemWithDcpmm ()) {
    //
    // NFIT is needed only if NVDIMMs are present.
    //
    return EFI_NOT_FOUND;
  }
  //
  // Call OEM-hook to create NFIT table in buffer. If success, then skip code below
  //
  Status = OemCreateNfitTable (NfitTablePtr, (UINT32)NfitTableSize);
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, CR_INFO_STR ("PMem") "OemCreateNfitTable succeeded - BuildNfitTable skipped\n"));
    return EFI_SUCCESS;
  }
  //
  // Build the actual NFIT table with needed sub-tables
  //
  Status = NfitBuildTable ((NVDIMM_FW_INTERFACE_TABLE*)NfitTablePtr, NfitTableSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, CR_ERROR_STR ("PMem") "NfitBuildTable failed (%r)\n", Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }
  //
  // Call OEM-hook to update NFIT table in buffer
  //
  OemUpdateNfitTable (NfitTablePtr);
  //
  // Update checksum because of possible changes in OemUpdateNfitTable
  //
  Status = NfitUpdateTableChecksum ((NVDIMM_FW_INTERFACE_TABLE*)NfitTablePtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, CR_ERROR_STR ("PMem") "NfitUpdateTableChecksum failed, status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }
  //
  // Having created Nfit table, Install NfitBindingProtocol so that the
  // UEFI driver can bind to it instead of some virtual Pci Device.
  //
  if (mSystemMemoryMap->MemSetup.dfxMemSetup.dfxLoadDimmMgmtDriver == LOAD_NGN_DIMM_MGMT_DRIVER_EN) {

    DEBUG ((DEBUG_INFO, CR_INFO_STR ("PMem") "Load PMem DIMM MGMT driver - gNfitBindingProtocolGuid will be published\n"));
    Status = gBS->InstallProtocolInterface (&Handle, &gNfitBindingProtocolGuid, EFI_NATIVE_INTERFACE, NULL);
    ASSERT_EFI_ERROR (Status);
    return Status;
  }
  return EFI_SUCCESS;
}

EFI_STATUS
InstallNfitTableUpdateProtocol (
  IN EFI_HANDLE        ImageHandle
)
{
  EFI_STATUS Status;

  DEBUG ((DEBUG_INFO, CR_INFO_STR ("PMem") "Installing NfitTableUpdateProtocol\n"));

  mNfitTableUpdateProtocol.UpdateAcpiTable = UpdateAcpiTable;

  Status = gBS->InstallProtocolInterface (
           &ImageHandle,
           &gEfiNfitTableUpdateProtocolGuid,
           EFI_NATIVE_INTERFACE,
           &mNfitTableUpdateProtocol
           );

  return Status;
}
