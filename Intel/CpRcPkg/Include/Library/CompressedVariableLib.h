/** @file
  Interface header file for the Compressed Variable library class.

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

#ifndef _COMPRESSED_VARIABLE_LIB_H_
#define _COMPRESSED_VARIABLE_LIB_H_

#include <Uefi.h>

//
// Maximum length of variable names
//

#define MAX_VARIABLE_NAME_LENGTH  128

/**
  Compress and save data to Nvram.

  @param[in] VariableName        Name of the variable to load.
  @param[in] VariableGuid        GUID of the variable to load.
  @param[in] VariableBuffer      Ptr to the buffer to load the variable to.
  @param[in] VariableBufferSize  Size of the buffer in bytes.

  @retval EFI_SUCCESS           The variable was saved successfully.
  @retval !EFI_SUCCESS          Failure.

**/

EFI_STATUS
EFIAPI
CompressAndSaveToVariable (
  IN CHAR16 *VariableName,
  IN EFI_GUID VariableGuid,
  IN VOID *VariableBuffer,
  IN UINT32 VariableBufferSize
  );

/**
  Retrieve data from a HOB(s), then compress and save the data.

  @param[in]  HobGuid        GUID of the HOB to save.
  @param[in]  VariableName   Name of the variable to save as.
  @param[in]  VariableGuid   GUID of the variable to save as.

  @retval EFI_SUCCESS           The variable was saved successfully.
  @retval !EFI_SUCCESS          Failure.

**/

EFI_STATUS
EFIAPI
SaveVariableFromHob (
  IN EFI_GUID HobGuid,
  IN CHAR16 *VariableName,
  IN EFI_GUID VariableGuid
  );

/**
  Load and decompress a variable from Nvram.

  @param[in]  VariableName        Name of the variable to load.
  @param[in]  VariableGuid        GUID of the variable to load.
  @param[out] VariableBuffer      Ptr to the buffer to load the variable to.
  @param[in]  VariableBufferSize  Size of the buffer in bytes.

  @retval EFI_SUCCESS           The variable was loaded and decompressed successfully.
  @retval EFI_NOT_FOUND         The variable was not found.
  @retval EFI_BUFFER_TOO_SMALL  The buffer was not large enough to hold the decompressed variable.

**/

EFI_STATUS
EFIAPI
LoadCompressedVariable (
  IN CHAR16 *VariableName,
  IN EFI_GUID VariableGuid,
  OUT VOID *VariableBuffer,
  IN UINT32 VariableBufferSize
  );

/**
  Load a variable from Nvram.

  @param[in]  VariableName        Name of the variable to load.
  @param[in]  VariableGuid        GUID of the variable to load.
  @param[out] VariableBuffer      Ptr to the buffer to load the variable to.
  @param[in]  VariableBufferSize  Size of the buffer in bytes.

  @retval EFI_SUCCESS           The variable was loaded and successfully.
  @retval EFI_NOT_FOUND         The variable was not found.
  @retval EFI_BUFFER_TOO_SMALL  The buffer was not large enough to hold the variable.

**/

EFI_STATUS
EFIAPI
LoadUncompressedVariable (
  IN CHAR16 *VariableName,
  IN EFI_GUID VariableGuid,
  IN OUT VOID *VariableBuffer,
  IN UINT32 VariableBufferSize
  );

#endif // #ifndef _COMPRESSED_VARIABLE_LIB_H_
