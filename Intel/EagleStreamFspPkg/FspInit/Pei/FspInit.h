/** @file

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

#ifndef _PLATFORM_LATE_INIT_PEI_H_
#define _PLATFORM_LATE_INIT_PEI_H_

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/PciCf8Lib.h>
#include <Library/PciLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/HobLib.h>
#include <Library/FspCommonLib.h>
#include <Library/FspPlatformLib.h>

#include <Ppi/PchPolicy.h>
#include <PchAccess.h>
#include <Library/MmPciBaseLib.h>

#include <FspUpd.h>
#include <FspsUpd.h>
#include <FspmUpd.h>

/**
This function assign temporary PCI resource for SOC devices.

@retval EFI_SUCCESS if it completed successfully.
**/
EFI_STATUS
AssignTemporaryPciResource(
/*IN  SI_POLICY_PPI  *SiPolicy,*/
IN  PCH_POLICY_PPI *PchPolicy
//IN  PCIE_IP_GLOBAL_POLICY_PPI   *PcieIpGlobalPolicy
);

#endif
