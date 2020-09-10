/** @file
  Platform variable initialization PEIM.

  This PEIM determines whether to load variable defaults. Ordinarily, the
  decision is based on the boot mode, but an OEM hook is provided to override
  that. The appropriate HOBs and PCDs are created to signal DXE code to update
  the variable default values.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2019 Intel Corporation. <BR>

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

#ifndef _PLATFORM_VARIABLE_INIT_PEI_H_
#define _PLATFORM_VARIABLE_INIT_PEI_H_

#include <PiPei.h>

#include <Library/MultiPlatSupportLib.h>

#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/FirmwareVolumeInfo.h>
#include <Ppi/CpuIo.h>

#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/PlatformVariableHookLib.h>
#include <Library/PlatformSetupVariableSyncLib.h>

//
// We only have one ID for all the platforms.
//
#define BOARD_ID_DEFAULT  0

#endif  // #ifndef _PLATFORM_VARIABLE_INIT_PEI_H_
