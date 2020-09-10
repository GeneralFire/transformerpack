/** @file
  Header file for the PolicyInitPei PEIM.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2020 Intel Corporation. <BR>

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

#ifndef _PEI_POLICY_INIT_H_
#define _PEI_POLICY_INIT_H_

#ifdef CDF_SC_FLAG
#include <PiPei.h>
#include <Library/SiPolicyLib.h>
#include <Library/PeiSiPolicyUpdateLib.h>
#include <Library/PeiPchPolicyDebugLib.h>
#include "PeiSiPolicyInit.h"
#else
#include <Library/IoLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/PchPcieDeviceTable.h>
#include <Library/PchPolicyLib.h>
#include <Guid/SetupVariable.h>
#include <PchSetupVariable.h>
#include <PeiPchPolicyUpdateLib.h>
#endif
#include <FirmwareConfigurations.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiPolicyInitLib.h>
#include <Library/PeiUpiPolicyLib.h>
#include <Library/PeiUpiPolicyUpdateLib.h>
#include <Library/PeiMemoryPolicyUpdateLib.h>
#include <Library/PeiCpuPolicyUpdateLib.h>
#include <Library/RasRcPolicyPpiLib.h>
#include <Library/RasRcPolicyPpiUpdateLib.h>
#endif  // _PEI_POLICY_INIT_H_
