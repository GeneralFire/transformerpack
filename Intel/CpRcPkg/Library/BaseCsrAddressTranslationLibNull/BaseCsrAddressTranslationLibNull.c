/** @file
  Csr To Pcie Conversion library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2018 Intel Corporation. <BR>

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

#include <Library/CsrAddressTranslationLib.h>

//////////////////////////////////////////////////////////////////////////
//
// USRA Csr to PCIE  Address Library
// This Lib provide the way use platform Library instance
//
//////////////////////////////////////////////////////////////////////////

/*
  This function identifies the different access type and then goes to different address translation routines.

  @param[in] Address              A pointer of the address of the USRA Address Structure
  @param[out] AlignedAddress      A pointer of aligned address converted from USRA address

  @retval 0                       The function completed successfully.
*/
UINTN
EFIAPI
CsrGetPhysicalAddress (
  IN USRA_ADDRESS             *Address,
  OUT UINTN                   *AlignedAddress
  )
{
  *AlignedAddress = 0;
  return 0;
}
