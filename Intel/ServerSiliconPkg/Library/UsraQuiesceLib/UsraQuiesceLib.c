/** @file
  Implementation of UsraQuiesceLib class library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2017 Intel Corporation. <BR>

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
#include <Protocol/QuiesceProtocol.h>
#include <Library/UsraQuiesceLib.h>
#include <Library/SiliconAccessLib.h>

#pragma pack(1)

UINT8 mMemQuiesceEnabled = FALSE;
UINT64 *mQuiescePtr;
QUIESCE_DATA_TABLE_ENTRY *mCurrQuiescePtr = NULL, *mStartQuiescePtr = NULL;

/**
  This function will read data from Quiesce buffer if CSR found, otherwise, read from HW.

  @param Address   - Pointer to USRA Address

  @retval data read for READ_ACCESS

**/
UINT32
QuiesceRegisterRead (
  IN USRA_ADDRESS             *Address
  )
{
  QUIESCE_DATA_TABLE_ENTRY *Ptr;
  UINT32 Data32;

  UINT64 RegAddress;
  UINT8  Width;

  RegAddress = InternalGetRegisterAddress(Address);
  Width  = (UINT8)(Address->Attribute.AccessWidth);

  if (mCurrQuiescePtr) {
    Ptr = mCurrQuiescePtr - 1;
    while (Ptr >= mStartQuiescePtr) {
      if (Ptr->CsrAddress == RegAddress) {
        return  Ptr->DataMask.Data;
      }
      Ptr--;
    }
  }

  //
  // If data not in quiesce buffer, read from hardware
  //
  switch (Width) {
  case 8:
    Data32 = *(volatile UINT8 *)(UINTN)RegAddress;
    break;
  case 16:
    Data32 = *(volatile UINT16 *)(UINTN)RegAddress;
    break;
  case 32:
    Data32 = *(volatile UINT32 *)(UINTN)RegAddress;
    break;
  default:
    Data32 = *(volatile UINT32 *)(UINTN)RegAddress;
    break;
  }
  return  Data32;
}

/**
  This function will write data to Quiesce buffer.

  @param RegAddr   - Pointer to USRA Address
  @param Buffer    - Pointer to data to be written

  @retval None

**/
VOID
QuiesceRegisterWrite (
  IN USRA_ADDRESS             *Address,
  IN VOID                    *Buffer
  )
{
  UINT64 RegAddress;
  UINT8  Width;
  UINT32 Data;

  RegAddress = InternalGetRegisterAddress(Address);
  Width  = (UINT8)(Address->Attribute.AccessWidth);
  Data = *((UINT32*)Buffer);

  if (!mCurrQuiescePtr) {
    return;
  }
  mCurrQuiescePtr->Operation       = EnumQuiesceWrite;
  mCurrQuiescePtr->AccessWidth     = Width;
  mCurrQuiescePtr->CsrAddress      = RegAddress;
  mCurrQuiescePtr->AndMask         = 0;
  mCurrQuiescePtr->DataMask.Data   = Data;
  mCurrQuiescePtr++;
  mQuiescePtr = (UINT64 *)mCurrQuiescePtr;

  return;
}

/**
  This function will return Quiesce status.

  @param VOID

  @retval Quiesce status.

**/
BOOLEAN
QuiesceStatusCheck (
  VOID
  )
{
  if (mMemQuiesceEnabled !=0) {
    return TRUE;
  }
  else
    return FALSE;
}
