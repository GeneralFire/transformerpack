/** @file
  Interface header file for the Mttr Calc library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

#ifndef _MTRR_CALC_LIB_H_
#define _MTRR_CALC_LIB_H_

#include <Uefi.h>

//
// MAX_AVAILABLE_MTRRS needs to be configurable in the future
//

#define MAX_AVAILABLE_MTRRS     8

/**
  Return the number of MTRR's required.

  @param[in]  MemoryLength        The top of cached memory.
  @param[out] MtrrsConsumed       Ptr to the MTRR's consumed.

  @retval EFI_SUCCESS             Success.
  @retval !EFI_SUCCESS            Failure.
**/

EFI_STATUS
EFIAPI
CountNumberOfMtrrsConsumed (
  IN UINT64 MemoryLength,
  OUT UINT8 *MtrrsConsumed
  );

#endif  // #ifndef _MTRR_CALC_LIB_H_
