/**@file
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

#include <Uefi.h>
#include <Stl2BiosLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

extern EFI_RUNTIME_SERVICES               *gRT;

/**

  Set Stl2BiosFusaSaf variable with values from arguments.

  @param[in] SafSupport           A pointer with value to set in Stl2Bios variable
  @param[in] EnableSaf            A pointer with value to set in Stl2Bios variable
  @param[in] DfxPathImageLoadAddr A pointer to NVME memory with path to image to set in Stl2Bios variable

  @retval EFI_SUCCESS            The Stl2Bios variable set correct.
  @retval Others returned from gRT->SetVariable

**/
EFI_STATUS
SetStl2BiosFusaSafVariable (
  UINT8         *SafSupport,
  UINT8         *EnableSaf,
  UINT64        *DfxPathImageLoadAddr
  )
{
  EFI_STATUS                          Status                       = EFI_SUCCESS;
  STL_2_BIOS_FUSA_SAF_DATA_VARIABLE   Stl2BiosFusaSafVariableData;
  UINTN                               Stl2BiosFusaSafVariableSize  = sizeof (STL_2_BIOS_FUSA_SAF_DATA_VARIABLE);

  if ((SafSupport == NULL)  ||
      (SafSupport == FALSE) ||
      (EnableSaf  == NULL)  ||
      (DfxPathImageLoadAddr == NULL)) {
    // if conditions are not met then set default value
    Stl2BiosFusaSafVariableData.Bits.Enable      = 0x0;
    Stl2BiosFusaSafVariableData.Bits.ImgLoad     = 0x0;
    Stl2BiosFusaSafVariableData.SafImagePathAddr = 0x0;
  } else {
    // initialize Stl2Bios variable struct
    Stl2BiosFusaSafVariableData.Bits.Enable      = ((*EnableSaf) == 0x1 ? 0x1 : 0x0);
    Stl2BiosFusaSafVariableData.SafImagePathAddr = *DfxPathImageLoadAddr;
    Stl2BiosFusaSafVariableData.Bits.ImgLoad     = 0x1;
  }

  Status = gRT->SetVariable (
    STL_2_BIOS_FUSA_SAF_DATA_VARIABLE_NAME,
    &gStl2BiosFusaSafUefiVariableGuid,
    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
    Stl2BiosFusaSafVariableSize,
    &Stl2BiosFusaSafVariableData
    );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "%a: Cannot allocate %s  variable!\n", __FUNCTION__, STL_2_BIOS_FUSA_SAF_DATA_VARIABLE_NAME));
  } else {
    DEBUG ((EFI_D_INFO, "%a: Successfully allocated %s variable\n", __FUNCTION__, STL_2_BIOS_FUSA_SAF_DATA_VARIABLE_NAME));
  }

  return Status;
}

/**

  Get Stl2BiosFusaSaf variable from memory.

  @param[out] The pointer which will be point to Stl2BiosFusaSaf.

  @retval EFI_SUCCESS            The function completed successfully.
  @retval EFI_NOT_FOUND          The variable was not found.
  @retval EFI_BUFFER_TOO_SMALL   The DataSize is too small for the result.
  @retval EFI_INVALID_PARAMETER  VariableName is NULL.
  @retval EFI_INVALID_PARAMETER  VendorGuid is NULL.
  @retval EFI_INVALID_PARAMETER  DataSize is NULL.
  @retval EFI_INVALID_PARAMETER  The DataSize is not too small and Data is NULL.
  @retval EFI_DEVICE_ERROR       The variable could not be retrieved due to a hardware error.
  @retval EFI_SECURITY_VIOLATION The variable could not be retrieved due to an authentication failure.

**/
EFI_STATUS
GetStl2BiosFusaSafVariable (
  OUT STL_2_BIOS_FUSA_SAF_DATA_VARIABLE *Stl2BiosFusaSafVariableData
  )
{
  EFI_STATUS Status                      = EFI_SUCCESS;
  UINTN      Stl2BiosFusaSafVariableSize = sizeof (STL_2_BIOS_FUSA_SAF_DATA_VARIABLE);

  Status = gRT->GetVariable (
    STL_2_BIOS_FUSA_SAF_DATA_VARIABLE_NAME,
    &gStl2BiosFusaSafUefiVariableGuid,
    NULL,
    &Stl2BiosFusaSafVariableSize,
    Stl2BiosFusaSafVariableData
    );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Error get variable %s - %r\n", __FUNCTION__, STL_2_BIOS_FUSA_SAF_DATA_VARIABLE_NAME, Status));
  }

  return Status;
}