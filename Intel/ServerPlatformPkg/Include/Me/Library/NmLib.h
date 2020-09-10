/** @file
  The library provides functions relevant to SPS NM.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains a 'Sample Driver' and is licensed as such under the terms
  of your license agreement with Intel or your vendor. This file may be modified
  by the user, subject to the additional terms of the license agreement.

@par Specification Reference:
**/

#ifndef _NM_LIB_H_
#define _NM_LIB_H_

#include <Base.h>

typedef enum {
  PowerOptimizedMode,
  PerformanceOptimizedMode,
} BIOS_BOOTING_MODE;

/**
  Return information if the SPS NM SPS NM requires Power Optimized Mode or Performance Optimized Mode.

  @param[out] BiosBootingMode     In case the function returns EFI_SUCCESS, then this variable
                                  stores information if the SPS NM requires Power Optimized Mode
                                  or Performance Optimized Mode

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_INVALID_PARAMETER   Invalid pointer to BiosBootingMode
  @retval EFI_UNSUPPORTED         The ME doesn't run SPS firmware or SPS NM is disabled
  @retval EFI_TIMEOUT             Timeout while waiting for the ME
  @retval EFI_ABORTED             In case of the boot after dirty warm reset,
                                  the execution of the function is aborted
  @retval EFI_NOT_FOUND           Cannot locate MeRcVariable config
**/
EFI_STATUS
GetBootingModeRequestedBySpsNm (
  OUT BIOS_BOOTING_MODE  *BiosBootingMode
  );

#endif // _NM_LIB_H_
