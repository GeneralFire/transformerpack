/** @file
  Implementation of UsraQuiesceLibNull class library.

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

#include <Library/UsraQuiesceLib.h>

#pragma pack(1)

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
  return 0;
}

/**
  This function will write data to Quiesce buffer.

  @param RegAddr   - Pointer to USRA Address
  @param Buffer    - Pointer to data to be written

**/
VOID
QuiesceRegisterWrite (
  IN USRA_ADDRESS             *Address,
  IN VOID                    *Buffer
  )
{
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
  return FALSE;
}
