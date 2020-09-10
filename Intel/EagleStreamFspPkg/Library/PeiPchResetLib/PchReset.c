/** @file
  PCH RESET PEIM DRIVER.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2017 Intel Corporation. <BR>

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

#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Ppi/Reset2.h>
#include <Ppi/PchReset.h>
#include <FspEas.h>
#include <Library/FspCommonLib.h>
#include <Library/FspSwitchStackLib.h>
#include <Library/BaseMemoryLib.h>


/**
  Resets the entire platform.

  @param[in] ResetType            UEFI defined reset type.
  @param[in] ResetStatus          The status code for the reset.
  @param[in] DataSize             The size of ResetData in bytes.
  @param[in] ResetData            Optional element used to introduce a platform specific reset.
                                  The exact type of the reset is defined by the EFI_GUID that follows
                                  the Null-terminated Unicode string.

**/
VOID
EFIAPI
ResetSystem (
  IN EFI_RESET_TYPE     ResetType,
  IN EFI_STATUS         ResetStatus,
  IN UINTN              DataSize,
  IN VOID               *ResetData OPTIONAL
  )
{
  UINT32 FspStatusResetRequired = 0x40000000;
  EFI_GUID            *GuidPtr;

  DEBUG ((DEBUG_INFO, "ResetSystem() Start\n"));
  switch (ResetType) {
  case EfiResetWarm:
    FspStatusResetRequired = FSP_STATUS_RESET_REQUIRED_WARM;
    break;

  case EfiResetCold:
    FspStatusResetRequired = FSP_STATUS_RESET_REQUIRED_COLD;
    break;

  case EfiResetShutdown:
    FspStatusResetRequired = FSP_STATUS_RESET_REQUIRED_3;
    break;

  case EfiResetPlatformSpecific:
    if (ResetData == NULL ) {
      DEBUG ((DEBUG_ERROR, "[PeiResetSystemLib] ResetData is not available.\n"));
      return;
    }
    GuidPtr = (EFI_GUID *) ((UINT8 *) ResetData + DataSize - sizeof (EFI_GUID));
    if (CompareGuid (GuidPtr, &gPchGlobalResetGuid)) {
      FspStatusResetRequired = FSP_STATUS_RESET_REQUIRED_4;
    }
    else {
      return;
    }
    break;

  default:
    return;
  }

  //
  // Return control back to boot loader with Reset Requirement set
  //
  SetFspApiReturnStatus (FspStatusResetRequired);
  Pei2LoaderSwitchStack ();

  DEBUG ((DEBUG_INFO, "ResetSystem() End\n"));
  //
  // Given we should have reset getting here would be bad
  //
  ASSERT (FALSE);
}

/**
  Initialize PCH Reset APIs

  @retval EFI_SUCCESS             APIs are installed successfully
  @retval EFI_OUT_OF_RESOURCES    Can't allocate pool
**/
EFI_STATUS
EFIAPI
PchInitializeReset (
  VOID
  )
{
  EFI_STATUS              Status;
  EFI_PEI_RESET2_PPI      *EfiPeiReset2Ppi;
  EFI_PEI_PPI_DESCRIPTOR  *EfiPeiReset2Descriptor;

  DEBUG ((DEBUG_INFO, "PchInitializeReset() Start\n"));

  EfiPeiReset2Descriptor = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  EfiPeiReset2Ppi = (EFI_PEI_RESET2_PPI *) AllocateZeroPool (sizeof (EFI_PEI_RESET2_PPI));
  if ((EfiPeiReset2Descriptor == NULL) ||
      (EfiPeiReset2Ppi == NULL)) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  ///
  /// Initialize the EFI Reset2 ppi instance
  ///
  EfiPeiReset2Ppi->ResetSystem  = ResetSystem;
  EfiPeiReset2Descriptor->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  EfiPeiReset2Descriptor->Guid  = &gEfiPeiReset2PpiGuid;
  EfiPeiReset2Descriptor->Ppi   = EfiPeiReset2Ppi;

  Status = PeiServicesInstallPpi (EfiPeiReset2Descriptor);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "PchInitializeReset() End\n"));

  return Status;
}
