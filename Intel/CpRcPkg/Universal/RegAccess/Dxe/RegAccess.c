/** @file
  USRA DXE driver manage all USRA entry initialized in PEI phase and DXE phase, and
  produce the implementation of native USRA protocol defined

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

#include <Protocol/SiliconRegAccess.h>
#include <Library/SiliconAccessLib.h>

//////////////////////////////////////////////////////////////////////////
//
// Common Hardware Access Library
//
//////////////////////////////////////////////////////////////////////////

/**
  This API performs 8-bit, 16-bit, 32-bit or 64-bit silicon register read operations.
  It transfers data from a register into a naturally aligned data buffer.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be read out
  @param[in] Buffer               A pointer of buffer for the value read from the register

  @retval RETURN_SUCCESS          The function completed successfully.
  @retval Others                  Return Error
**/
RETURN_STATUS
EFIAPI
DxeRegRead (
  IN USRA_ADDRESS             *Address,
  IN VOID                     *Buffer
  )
{
  return InternalRegisterRead(Address, Buffer);
};

/**
  This API performs 8-bit, 16-bit, 32-bit or 64-bit silicon register write operations.
  It transfers data from a naturally aligned data buffer into a silicon register.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be written
  @param[in] Buffer               A pointer of buffer for the value write to the register

  @retval RETURN_SUCCESS          The function completed successfully.
  @retval Others                  Return Error
**/
RETURN_STATUS
EFIAPI
DxeRegWrite (
  IN USRA_ADDRESS             *Address,
  IN VOID                     *Buffer
  )
{
  return InternalRegisterWrite(Address, Buffer);
};

/**
  This API performs 8-bit, 16-bit, 32-bit or 64-bit silicon register AND then OR operations. It read data from a
  register, And it with the AndBuffer, then Or it with the OrBuffer, and write the result back to the register

  @param[in] Address              A pointer of the address of the silicon register to be modified
  @param[in] AndBuffer            A pointer of buffer for the value used for AND operation
                                  A NULL pointer means no AND operation. RegisterModify() equivalents to RegisterOr()
  @param[in] OrBuffer             A pointer of buffer for the value used for OR operation
                                  A NULL pointer means no OR operation. RegisterModify() equivalents to RegisterAnd()

  @retval RETURN_SUCCESS          The function completed successfully.
  @retval Others                  Return Error
**/
RETURN_STATUS
EFIAPI
DxeRegModify (
  IN USRA_ADDRESS             *Address,
  IN VOID                     *AndBuffer,
  IN VOID                     *OrBuffer
  )
{
  return InternalRegisterModify(Address, AndBuffer, OrBuffer);
};

/**
  This API gets the flat address from the given USRA Address.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be read out

  @retval                         The flat address
**/
UINTN
EFIAPI
DxeGetRegAddr (
  IN USRA_ADDRESS               *Address
  )
{
  return InternalGetRegisterAddress(Address);
};
