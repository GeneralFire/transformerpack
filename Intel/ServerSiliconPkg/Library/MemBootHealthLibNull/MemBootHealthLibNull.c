/** @file
  MemBootHealthLibNull.c

  Library implementation of API to support Mem Boot Health function.
  This is a NULL implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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
#include <Base.h>

/**
  This function Executes Mem Boot Health test and runs it.
  Should be called during fast boot where MRC training is not done, to validate
  the current margins.

  @retval EFI_SUCCESS           -  Boot Health test successfully executed
  @retval EFI_INVALID_PARAMETER -  Invalid parameter passed.
  @retval EFI_UNSUPPORTED       -  Feature not supported
**/
EFI_STATUS
EFIAPI
MemBootHealthTest (
  VOID
  )
{
  return EFI_UNSUPPORTED;
}

/**
Set MemBootHealth Margin Test failure flag into CMOS.
  @param[in]  HasError    failure flag. TRUE: has detected failure. FALSE: no failure
  @retval N/A
**/
VOID
EFIAPI
SetMemBootHealthCheckErrorFlag (
  IN BOOLEAN HasError
)
{
  //NULL Function
}
/**
Get/Read the MemBootHealth Margin Test failure flag from CMOS.

@retval margin test failure flag. TURE: failed, FALSE: passed
**/
BOOLEAN
EFIAPI
GetMemBootHealthCheckErrorFlag (
  VOID
)
{
  return FALSE;
}