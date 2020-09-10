/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2017 Intel Corporation. <BR>

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

#include "SerialPortLibUsb.h"

/**
  Verifies if the bit positions specified by a mask are set in a register.

  @param[in, out] *Register    32-bit register
  @param[in] BitMask      32-bit mask

  @return  BOOLEAN  - TRUE if all bits specified by the mask are enabled.
          - FALSE even if one of the bits specified by the mask
            is not enabled.
**/
BOOLEAN
IsBitSet(
  IN OUT  UINT32  *Register,
  IN    UINT32  BitMask
  )
{
  BOOLEAN    Result = FALSE;
  if (((*(volatile UINT32 *)(Register)) & (BitMask)) != 0) {
    Result = TRUE;
  }
  return Result;
}

/**
  Verifies if the bit positions specified by a mask are cleared in a register.

  @param[in, out] *Register    32-bit register
  @param[in] BitMask      32-bit mask

  @return  BOOLEAN  - TRUE if all bits specified by the mask are disabled.
          - FALSE even if one of the bits specified by the mask
            is enabled.
**/
BOOLEAN
IsBitClear(
  IN OUT  UINT32  *Register,
  IN    UINT32  BitMask
  )
{
  BOOLEAN    Result = FALSE;
  if(((*(volatile UINT32 *)(Register)) & (BitMask)) == 0) {
    Result = TRUE;
  }
  return Result;
}

/**
  Sets bits as per the enabled bit positions in the mask

  @param[in, out] *Register    32-bit register
  @param[in] BitMask      32-bit mask

**/
VOID
SetR32Bit(
  IN OUT  UINT32  *Register,
  IN    UINT32  BitMask
)
{
    UINT32    RegisterValue = *(volatile UINT32 *)(Register);
    RegisterValue |= (UINT32)(BitMask);
    *(volatile UINT32 *)(Register) = RegisterValue;
}

/**
  Clears bits as per the enabled bit positions in the mask

  @param[in, out] *Register    32-bit register
  @param[in] BitMask      32-bit mask

**/
VOID
ClrR32Bit(
  IN OUT  UINT32  *Register,
  IN    UINT32  BitMask
)
{
    UINT32    RegisterValue = *(volatile UINT32 *)(Register);
    RegisterValue &= (UINT32)(~(BitMask));
    *(volatile UINT32 *)(Register) = RegisterValue;
}

/**
  Clears bits as per the enabled bit positions in the mask
  and sets new value to the register.

  @param[in, out] *Register    32-bit register
  @param[in] BitMask      32-bit mask
  @param[in] Value          32-bit Value

**/
VOID
ClrAndSetR32Bit(
  IN OUT  UINT32  *Register,
  IN    UINT32  BitMask,
  IN    UINT32  Value
)
{
    UINT32    RegisterValue = *(volatile UINT32 *)(Register);
    RegisterValue &= (UINT32)(~(BitMask));
    RegisterValue |= (UINT32)(Value);
    *(volatile UINT32 *)(Register) = RegisterValue;
}

/**
  Sets bits as per the enabled bit positions in the mask

  @param[in, out] *Register    16-bit register
  @param[in] BitMask      26-bit mask

**/
VOID
Set16Bit(
  IN OUT  UINT16  *Register,
  IN    UINT16  BitMask
)
{
    UINT16    RegisterValue = *(volatile UINT16 *)(Register);
    RegisterValue |= (UINT16)(BitMask);
    *(volatile UINT16 *)(Register) = RegisterValue;
}

/**
  Clears bits as per the enabled bit positions in the mask

  @param[in, out] *Register    16-bit register
  @param[in] BitMask      16-bit mask

**/
VOID
ClrR16Bit(
  IN OUT  UINT16  *Register,
  IN    UINT16  BitMask
)
{
    UINT16    RegisterValue = *(volatile UINT16 *)(Register);
    RegisterValue &= (UINT16)(~(BitMask));
    *(volatile UINT16 *)(Register) = RegisterValue;
}

/**
  Clears bits as per the enabled bit positions in the mask

  @param[in, out] *Register    8-bit register
  @param[in] BitMask      8-bit mask

**/
VOID
SetR8Bit(
  IN OUT  UINT8  *Register,
  IN    UINT8  BitMask
)
{
    UINT8    RegisterValue = *(volatile UINT8 *)(Register);
    RegisterValue |= (UINT8)(BitMask);
    *(volatile UINT8 *)(Register) = RegisterValue;
}

/**
  Clears bits as per the enabled bit positions in the mask

  @param[in, out] *Register    8-bit register
  @param[in] BitMask      8-bit mask

**/
VOID
ClrR8Bit(
  IN OUT  UINT8  *Register,
  IN    UINT8  BitMask
)
{
    UINT8    RegisterValue = *(volatile UINT8 *)(Register);
    RegisterValue &= (UINT8)(~(BitMask));
    *(volatile UINT8 *)(Register) = RegisterValue;
}

