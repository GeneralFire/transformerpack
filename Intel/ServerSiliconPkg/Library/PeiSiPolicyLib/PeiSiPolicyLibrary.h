/** @file
  Header file for the PeiSiPolicyLib library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2018  Intel Corporation. <BR>

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

#ifndef _PEI_SI_POLICY_LIBRARY_H_
#define _PEI_SI_POLICY_LIBRARY_H_

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/SiPolicyLib.h>
#include <Library/PchPolicyLib.h>
#include <Library/PeiAmtPolicyLib.h>
#include <Library/PeiSpsPreMemPolicyLib.h>
#include <Library/PeiMePolicyLib.h>
#include <PchAccess.h>
#include <Library/SiPolicyLib.h>

#define TEMP_MEM_BASE_ADDRESS 0xFE600000
#define TEMP_IO_BASE_ADDRESS  0xD000

//
// IO/MMIO resource limits
//
#define TEMP_MEM_SIZE         V_XDCI_CFG_MEM_LENGTH
#define TEMP_IO_SIZE          0x10

#endif // _PEI_SI_POLICY_LIBRARY_H_
