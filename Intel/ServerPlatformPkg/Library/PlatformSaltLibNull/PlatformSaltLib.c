/** @file
  Platform Salt Null Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2017 Intel Corporation. <BR>

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

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/PlatformSaltLib.h>


/**
  This function takes in an allocated salt buffer, and then fills it with the platform
  specific Salt value, and will return Status accordinly. In the event of an error, the
  value of SaltLen is not valid.

  @param[out]      Salt      Pointer to the Salt buffer from the Caller
  @param[in, out]  SaltLen   Size of Salt (in bytes) requested by Caller, and the value
                             returned is the size that was actually stored in the buffer.
                             (in both directions it must be <= size of the Salt buffer).

  @return  EFI_SUCCESS       The function completes successfully
  @return  Others            An error occured, or Salt is not supported

**/
EFI_STATUS
EFIAPI
GetPlatformSalt (
  IN     VOID   *Salt,
  IN OUT UINTN  *SaltLen
  )
{
  *SaltLen = 0;
  return EFI_UNSUPPORTED;
}
