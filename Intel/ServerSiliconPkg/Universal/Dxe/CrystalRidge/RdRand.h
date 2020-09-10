/** @file
  Header for the RDRAND APIs used by RNG DXE driver.
  Support API definitions for RDRAND instruction access, which will leverage
  Intel Secure Key technology to provide high-quality random numbers for use
  in applications, or entropy for seeding other random number generators.
  Refer to http://software.intel.com/en-us/articles/intel-digital-random-number
  generator-drng-software-implementation-guide/ for more information about Intel
  Secure Key technology.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2018 Intel Corporation. <BR>

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

#ifndef __RD_RAND_H__
#define __RD_RAND_H__

#include <Library/BaseLib.h>

//
// The maximun number of retries to obtain one available random number.
//
#define RETRY_LIMIT    10

/**
  Generates a 64-bit random number through RDRAND instruction.

  @param[out]  Rand          Buffer pointer to store the random result.

  @retval TRUE               RDRAND call was successful.
  @retval FALSE              Failed attempts to call RDRAND.

**/
BOOLEAN
EFIAPI
RdRand64Step  (
  OUT UINT64   *Rand
  );


/**
  Calls RDRAND to obtain a 64-bit random number.

  @param[out]  Rand          Buffer pointer to store the random result.
  @param[in]   NeedRetry     Determine whether or not to loop retry.

  @retval EFI_SUCCESS        RDRAND call was successful.
  @retval EFI_NOT_READY      Failed attempts to call RDRAND.

**/
EFI_STATUS
EFIAPI
RdRand64 (
  OUT UINT64       *Rand,
  IN BOOLEAN       NeedRetry
  );

#endif  // __RD_RAND_H__
