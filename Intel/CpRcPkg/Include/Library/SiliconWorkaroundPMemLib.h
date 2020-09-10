/** @file
  Silicon workaround library. Each platform is required to provide an implementation
  of this library class to handle silicon workarounds depending on:
  Silicon environment (Silicon, Simics, CTE, emulation and etc.)
  Silicon stepping

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

#ifndef __SILICON_WORKAROUND_PMEM_LIB__
#define __SILICON_WORKAROUND_PMEM_LIB__

#include <Uefi/UefiBaseType.h>
#include "SysHost.h"

/**
  Return whether the workaround is enabled.

  This function returns a boolean that determines whether the workaround is enabled
  given a workaround name.
  
  @param[in] Host                        Pointer to sysHost
  @param[in] Socket                      Socket Id
  @param[in] Channel                     Socket channel number
  @param[in] Dimm                        DIMM slot number
  @param[in] WorkaroundName              An ASCII string that represents the workaround name.
                                         This workaround name should correspond to an entry
                                         in the silicon workarounds table(s).

  @retval TRUE    The workaround is enabled.
  @retval FALSE   The workaround is not found in the table(s) or not applicable to DDR5 and therefore disabled.

**/
BOOLEAN
EFIAPI
IsPmemSiliconWorkaroundEnabled (
  IN PSYSHOST          Host,
  IN UINT8             Socket,
  IN UINT8             Channel,
  IN UINT8             Dimm,
  IN CONST CHAR8       *WorkaroundName
  );

#endif // __SILICON_WORKAROUND_PMEM_LIB__
