/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2018 Intel Corporation. <BR>

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

#include "Variable.h"

#define HOB_DATA_SIZE_ALIGNMENT 8
#define GET_OCCUPIED_SIZE(ActualSize, Alignment) \
  ((ActualSize)+(((Alignment)-((ActualSize)& ((Alignment)-1))) & ((Alignment)-1)))

//
// Module globals
//
EFI_PEI_READ_ONLY_VARIABLE2_PPI mVariablePpi = {
  FspGetVariable,
  FspGetNextVariableName
};

EFI_PEI_PPI_DESCRIPTOR     mPpiListVariable = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiReadOnlyVariable2PpiGuid,
  &mVariablePpi
};


/**
  Provide the functionality of the variable services.

  @param  FileHandle   Handle of the file being invoked.
                       Type EFI_PEI_FILE_HANDLE is defined in FfsFindNextFile().
  @param  PeiServices  General purpose services available to every PEIM.

  @retval EFI_SUCCESS  If the interface could be successfully installed
  @retval Others       Returned from PeiServicesInstallPpi()
**/
EFI_STATUS
EFIAPI
PeimInitializeVariableServices (
  IN       EFI_PEI_FILE_HANDLE       FileHandle,
  IN CONST EFI_PEI_SERVICES          **PeiServices
  )
{
  EFI_STATUS          Status;

  //
  // Create HOBs.
  //
  Status = PlatformHobBuildLib (PeiServices);
  ASSERT_EFI_ERROR (Status);

  return PeiServicesInstallPpi (&mPpiListVariable);
}

/**
  This service retrieves a variable's value using its name and GUID.

  Read the specified variable from the UEFI variable store. If the Data
  buffer is too small to hold the contents of the variable, the error
  EFI_BUFFER_TOO_SMALL is returned and DataSize is set to the required buffer
  size to obtain the data.

  @param  This                  A pointer to this instance of the EFI_PEI_READ_ONLY_VARIABLE2_PPI.
  @param  VariableName          A pointer to a null-terminated string that is the variable's name.
  @param  VariableGuid          A pointer to an EFI_GUID that is the variable's GUID. The combination of
                                VariableGuid and VariableName must be unique.
  @param  Attributes            If non-NULL, on return, points to the variable's attributes.
  @param  DataSize              On entry, points to the size in bytes of the Data buffer.
                                On return, points to the size of the data returned in Data.
  @param  Data                  Points to the buffer which will hold the returned variable value.

  @retval EFI_SUCCESS           The variable was read successfully.
  @retval EFI_NOT_FOUND         The variable could not be found.
  @retval EFI_BUFFER_TOO_SMALL  The DataSize is too small for the resulting data.
                                DataSize is updated with the size required for
                                the specified variable.
  @retval EFI_INVALID_PARAMETER VariableName, VariableGuid, DataSize or Data is NULL.
  @retval EFI_DEVICE_ERROR      The variable could not be retrieved because of a device error.

**/
EFI_STATUS
EFIAPI
FspGetVariable (
  IN CONST  EFI_PEI_READ_ONLY_VARIABLE2_PPI *This,
  IN CONST  CHAR16                          *VariableName,
  IN CONST  EFI_GUID                        *VariableGuid,
  OUT       UINT32                          *Attributes,
  IN OUT    UINTN                           *DataSize,
  OUT       VOID                            *Data
  )
{
  UINTN                   VarDataSize;
  VOID                    *VarDataPtr;
  EFI_HOB_GUID_TYPE  *FspInitNvBuffer;
  EFI_PEI_HOB_POINTERS        HobList;

  if (VariableName == NULL || VariableGuid == NULL || DataSize == NULL) {

    return EFI_INVALID_PARAMETER;
  }

  if (CompareGuid (VariableGuid, &gEfiMemoryConfigDataGuid)) {
    //
    // Find MRC data in NVRAM
    //
    FspInitNvBuffer = (EFI_HOB_GUID_TYPE *)((FSPM_UPD_COMMON *)GetFspApiParameter ())->FspmArchUpd.NvsBufferPtr;
    if (FspInitNvBuffer == NULL)
      return EFI_INVALID_PARAMETER;

    //
    // Verify Hob GUID
    //
    if (GET_HOB_TYPE(FspInitNvBuffer) != EFI_HOB_TYPE_GUID_EXTENSION)
      return EFI_UNSUPPORTED;

    if (!CompareGuid (VariableGuid, &FspInitNvBuffer->Name))
      return EFI_UNSUPPORTED;

    //
    // Get data size and pointer
    //
    VarDataSize = GET_GUID_HOB_DATA_SIZE (FspInitNvBuffer);
    VarDataPtr = GET_GUID_HOB_DATA (FspInitNvBuffer);

  } else {
    //
    // Retrieve Variable from HOB.
    //
    HobList.Guid = GetFirstGuidHob(VariableGuid);
    if (HobList.Guid == NULL) {
      return EFI_UNSUPPORTED;
    }
    //
    // Get data size and pointer
    //
    VarDataPtr = GET_GUID_HOB_DATA(HobList.Guid);
    VarDataSize = GET_GUID_HOB_DATA_SIZE(HobList.Guid);
  }

  //
  // Since HOB data size (VarDataSize) always be 8 byte alignment, it would cause an assert
  // if *DataSize is not 8 aligment structure. The solution is to force the HOB data size (VarDataSize)
  // to *DataSize.
  //
  if ((VarDataSize > *DataSize) && (VarDataSize == GET_OCCUPIED_SIZE(*DataSize, HOB_DATA_SIZE_ALIGNMENT))) {
    VarDataSize = *DataSize;
  }

  //
  // Get data size
  //
  if (*DataSize >= VarDataSize) {
    if (Data == NULL) {
      return EFI_INVALID_PARAMETER;
    }

    CopyMem (Data, VarDataPtr, VarDataSize);

    if (Attributes != NULL) {
      *Attributes = 0;  // Force to zero
    }

    *DataSize = VarDataSize;
    return EFI_SUCCESS;
  } else {
    *DataSize = VarDataSize;
    return EFI_BUFFER_TOO_SMALL;
  }
}

/**
  Return the next variable name and GUID.

  This function is called multiple times to retrieve the VariableName
  and VariableGuid of all variables currently available in the system.
  On each call, the previous results are passed into the interface,
  and, on return, the interface returns the data for the next
  interface. When the entire variable list has been returned,
  EFI_NOT_FOUND is returned.

  @param  This              A pointer to this instance of the EFI_PEI_READ_ONLY_VARIABLE2_PPI.

  @param  VariableNameSize  On entry, points to the size of the buffer pointed to by VariableName.
                            On return, the size of the variable name buffer.
  @param  VariableName      On entry, a pointer to a null-terminated string that is the variable's name.
                            On return, points to the next variable's null-terminated name string.
  @param  VariableGuid      On entry, a pointer to an EFI_GUID that is the variable's GUID.
                            On return, a pointer to the next variable's GUID.

  @retval EFI_SUCCESS           The variable was read successfully.
  @retval EFI_NOT_FOUND         The variable could not be found.
  @retval EFI_BUFFER_TOO_SMALL  The VariableNameSize is too small for the resulting
                                data. VariableNameSize is updated with the size
                                required for the specified variable.
  @retval EFI_INVALID_PARAMETER VariableName, VariableGuid or
                                VariableNameSize is NULL.
  @retval EFI_DEVICE_ERROR      The variable could not be retrieved because of a device error.

**/
EFI_STATUS
EFIAPI
FspGetNextVariableName (
  IN CONST  EFI_PEI_READ_ONLY_VARIABLE2_PPI *This,
  IN OUT UINTN                              *VariableNameSize,
  IN OUT CHAR16                             *VariableName,
  IN OUT EFI_GUID                           *VariableGuid
  )
{
  return EFI_UNSUPPORTED;
}
