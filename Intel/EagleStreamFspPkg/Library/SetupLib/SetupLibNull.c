/** @file
  Library functions for SetupLib.
  This library instance provides methods to access Setup option.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2016 Intel Corporation. <BR>

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

#include <Uefi/UefiBaseType.h>
#include <SetupTable.h>

/**
  This function provides a means by which to retrieve a value for a given option.

  Returns the data, data type and data size specified by OptionNumber and Guid.

  @param[in]  Guid          Pointer to a 128-bit unique value that designates
                            which namespace to retrieve a value from.
  @param[in]  OptionNumber  The option number to retrieve a current value for.
  @param[in, out] Data      A pointer to the buffer to be retrieved.
  @param[in, out] DataSize  The size, in bytes, of Buffer.

  @retval EFI_SUCCESS           Data is successfully reterieved.
  @retval EFI_INVALID_PARAMETER Guid is NULL or DataSize is NULL or OptionNumber is invalid.
  @retval EFI_BUFFER_TOO_SMALL  Input data buffer is not enough.
  @retval EFI_NOT_FOUND         The given option is not found.

**/
EFI_STATUS
EFIAPI
GetOptionData (
  IN EFI_GUID            *Guid,
  IN UINTN               OptionNumber,
  IN OUT VOID            *Data,
  IN UINTN               DataSize
  )
{
  return EFI_NOT_FOUND;
}

/**
  This function provides a means by which to set a value for a given option number.

  Sets a buffer for the token specified by OptionNumber to the value specified by
  Data and DataSize.
  If DataSize is greater than the maximum size support by OptionNumber,
  then set DataSize to the maximum size supported by OptionNumber.

  @param[in]  Guid              Pointer to a 128-bit unique value that
                                designates which namespace to set a value from.
  @param[in]  OptionNumber      The option number to set a current value for.
  @param[in]  Data              A pointer to the buffer to set.
  @param[in, out] DataSize      The size, in bytes, of Buffer.

  @retval EFI_SUCCESS           Data is successfully updated.
  @retval EFI_INVALID_PARAMETER OptionNumber is invalid, Guid is NULL, or Data is NULL, or DataSize is NULL.
  @retval EFI_NOT_FOUND         The given option is not found.
  @retval EFI_UNSUPPORTED       Set action is not supported.
**/
EFI_STATUS
EFIAPI
SetOptionData (
  IN EFI_GUID *Guid,
  IN UINTN    OptionNumber,
  IN VOID     *Data,
  IN UINTN    DataSize
  )
{
  return EFI_NOT_FOUND;
}

/**
  Get all data in the setup

  @retval EFI_SUCCESS           Data is committed successfully.
  @retval EFI_INVALID_PARAMETER Guid is NULL.
  @retval EFI_NOT_FOUND         Guid is not found.
  @retval EFI_DEVICE_ERROR      Data can't be committed.
**/
EFI_STATUS
EFIAPI
GetEntireConfig (
  // APTIOV_SERVER_OVERRIDE_RC_START      
  OUT INTEL_SETUP_DATA *SetupData
  // APTIOV_SERVER_OVERRIDE_RC_START
  )
{
  return EFI_NOT_FOUND;
}

/**
  Get the SetupData pointer.
  First time call this API, will allocate the memory for SetupData pointer, and build a HOB to cache the setup data.
  The other time call this API, get the SetupData pointer from the HOB.

  @param[out] SetupData        A pointer to the buffer for Setup variable

  @retval EFI_SUCCESS              Data is committed successfully.
  @retval EFI_INVALID_PARAMETER    Input data is NULL.
  @retval EFI_NOT_FOUND            Setup variable is not found.
**/
**/
EFI_STATUS
EFIAPI
GetEntireConfigPtr (
// APTIOV_SERVER_OVERRIDE_RC_START
  OUT INTEL_SETUP_DATA **SetupData
// APTIOV_SERVER_OVERRIDE_RC_START
  )
{
  return EFI_NOT_FOUND;
}

/**
  Set all data in the setup

  @retval EFI_SUCCESS           Data is committed successfully.
  @retval EFI_INVALID_PARAMETER Guid is NULL.
  @retval EFI_NOT_FOUND         Guid is not found.
  @retval EFI_DEVICE_ERROR      Data can't be committed.
**/
EFI_STATUS
EFIAPI
SetEntireConfig (
  // APTIOV_SERVER_OVERRIDE_RC_START
  IN INTEL_SETUP_DATA *SetupData
  // APTIOV_SERVER_OVERRIDE_RC_END
  )
{
  return EFI_NOT_FOUND;
}

/**
  This function provides a means by which to retrieve a value for a given option.

  Returns the data, data type and data size specified by OptionNumber and Guid.

  @param[in]  Guid          Pointer to a 128-bit unique value that designates
                            which namespace to retrieve a value from.
  @param[in]  Variable      Pointer to data location where variable is stored.

  @retval EFI_SUCCESS           Data is successfully reterieved.
  @retval EFI_INVALID_PARAMETER Guid or Variable is null.
  @retval EFI_NOT_FOUND         The given option is not found.

**/
EFI_STATUS
EFIAPI
GetSpecificConfigGuid (
  IN EFI_GUID            *Guid,
  OUT VOID               *Variable
  )
{
  return EFI_NOT_FOUND;
}

/**
  This function provides a means by which to set a value for a given option number.

  Sets a buffer for the token specified by OptionNumber to the value specified by
  Data and DataSize.
  If DataSize is greater than the maximum size support by OptionNumber,
  then set DataSize to the maximum size supported by OptionNumber.

  @param[in]  Guid              Pointer to a 128-bit unique value that
                                designates which namespace to set a value from.
  @param[in]  Variable      Pointer to data location where variable is stored.

  @retval EFI_SUCCESS           Data is successfully updated.
  @retval EFI_INVALID_PARAMETER OptionNumber is invalid, Guid is NULL, or Data is NULL, or DataSize is NULL.
  @retval EFI_NOT_FOUND         The given option is not found.
  @retval EFI_UNSUPPORTED       Set action is not supported.
**/
EFI_STATUS
EFIAPI
SetSpecificConfigGuid (
  IN EFI_GUID *Guid,
  IN VOID *Variable
  )
{
  return EFI_NOT_FOUND;
}
