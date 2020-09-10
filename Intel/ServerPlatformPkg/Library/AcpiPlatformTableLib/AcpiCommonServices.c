/** @file
  In ACPI Platform driver, this file contains non-ACPI table related
  common services that aid to fill ACPI tables

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

#include "AcpiCommonServices.h"

extern EFI_IIO_UDS_PROTOCOL        *mIioUds;


/**
  Converts ascii value to decimal number. Checks if the input ascii value
  is a valid decimal number. If ascii value is not a decimal number, value
  0xFF returned

  @param[in]  UINT8       AsciiValue

  @retval     UINT8       0-9
                          0xFF - Non-numeric Ascii value

**/
UINT8
ConvertAsciiToDecimal (
  IN  UINT8     AsciiValue)
{
  if(InternalAsciiIsDecimalDigitCharacter(AsciiValue)) {
    return (UINT8)(AsciiValue - '0x30');
  }
  return 0xFF;
}


/**
  ACPI device name composed with socket and stack number. CXL devices named as
  CXxy. x - Socket Number and y - Stack Number.
  This function gets socket and stack number of the ACPI device.

  @param[in]  CHAR8       PathName. Example "_SB_.CX01.FIX1"
  @param[out] IIO_STACK   Socket and Stack number

  @retval     EFI_STATUS  EFI_SUCCESS -   Valid ACPI devices found. Socket and Stack
                                          number updated to IIO_STACK data structure
                          EFI_NOT_FOUND - Valid device name is not found
 **/
EFI_STATUS
EFIAPI
GetCxlStackAttributes (
  IN  CHAR8     *PathName,
  OUT IIO_STACK *IioStack
  )
{
  if (IsCxlAcpiDevicePath(PathName) == TRUE) {
    IioStack->SocketId = PathName[7];
    IioStack->StackId = PathName[8];
  }
  if (InternalAsciiIsDecimalDigitCharacter(IioStack->SocketId) &&
      InternalAsciiIsDecimalDigitCharacter(IioStack->StackId)) {
        IioStack->SocketId = ConvertAsciiToDecimal(IioStack->SocketId);
        IioStack->StackId = ConvertAsciiToDecimal(IioStack->StackId);
        return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}


/**
  Checks if the given ACPI device is CXL device or not

  @param[in]  CHAR8       PathName. Example "_SB_.CX01.FIX1"

  @retval     BOOLEAN     TRUE    CXL ACPI device
                          FALSE   Non-CXL ACPI device

**/
BOOLEAN
EFIAPI
IsCxlAcpiDevicePath (
  IN  CHAR8     *PathName
  )
{
  if ((PathName[5] == 'C') && (PathName[6] == 'X')) {
    return TRUE;
  }

  return FALSE;
}


/**
  Check if socket and stack numbers are valid

  @param[in]  IIO_STACK   IioStack  Socket and Stack number

  @retval     BOOLEAN     TRUE      Valid Socket and Stack number
                          FALSE     Invalid socket or stack number

**/
BOOLEAN
EFIAPI
IsStackAttributeValid (
  IN  IIO_STACK *IioStack
  )
{

  if ((IioStack->StackId < GetMaxStackNumPerSocket()) &&
      (IioStack->SocketId < MAX_SOCKET)) {
        return TRUE;
    }
  return FALSE;
}


/**
  Check if socket and stack numbers are valid

  @param[in]  IIO_STACK   IioStack  Socket and Stack number

  @retval     BOOLEAN     TRUE      Valid Socket and Stack number
                          FALSE     Invalid socket or stack number

**/
BOOLEAN
EFIAPI
IsStackInCxlMode (
  IN  UINT8 Socket,
  IN  UINT8 Stack
)
{
  if (mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].CxlPresentBitmap & (BIT0 << Stack)) {
    return TRUE;
  }
  return FALSE;
}