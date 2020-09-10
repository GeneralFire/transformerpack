/** @file
  Boot service DXE IFWI ID library implementation.
  These functions in this file can be called during DXE and cannot be called during runtime
  or in SMM which should use a RT or SMM library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 Intel Corporation. <BR>

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

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/HobLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/IfwiIdLib.h>

#include <Guid/IfwiId.h>

/**
  This function returns IFWI ID by searching HOB or FV.

  @param[out] IfwiIdImage   The IFWI ID got from HOB or FV.

  @retval EFI_SUCCESS               IFWI ID has been got successfully.
  @retval EFI_NOT_FOUND             IFWI ID image is not found, and no parameter will be modified.
  @retval EFI_INVALID_PARAMETER     The parameter is NULL.

**/
EFI_STATUS
EFIAPI
GetIfwiId (
  OUT IFWI_ID_IMAGE     *IfwiIdImage
  )
{
  EFI_STATUS    Status;
  VOID          *Address;
  UINTN         Size;

  Address = NULL;
  Size = 0;

  if (IfwiIdImage == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Address = GetFirstGuidHob (&gIfwiIdGuid);
  if (Address != NULL) {
    Size = sizeof (IFWI_ID_IMAGE);
    CopyMem ((VOID *) IfwiIdImage, GET_GUID_HOB_DATA (Address), Size);

    DEBUG ((EFI_D_INFO, "DXE get IFWI ID from HOB successfully\n"));
    DEBUG ((EFI_D_INFO, "IFWI ID: %s\n", (CHAR16 *) (&(IfwiIdImage->IfwiIdString))));
    return EFI_SUCCESS;
  }

  Status = GetSectionFromAnyFv (
             &gIfwiIdGuid,
             EFI_SECTION_RAW,
             0,
             &Address,
             &Size
             );
  if ((Status == EFI_SUCCESS) && (Address != NULL)) {
    //
    // IFWI ID image is present in FV.
    //
    Size = sizeof (IFWI_ID_IMAGE);
    CopyMem ((VOID *) IfwiIdImage, Address, Size);
    //
    // GetSectionFromAnyFv () allocated buffer for Address, now free it.
    //
    FreePool (Address);

    DEBUG ((EFI_D_INFO, "DXE get IFWI ID from FV successfully\n"));
    DEBUG ((EFI_D_INFO, "IFWI ID: %s\n", (CHAR16 *) (&(IfwiIdImage->IfwiIdString))));
    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}
