/** @file
  Header file for Host Bridge Info Lib implementation.

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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

#ifndef _HOST_BRIDGE_INFO_LIB_H_
#define _HOST_BRIDGE_INFO_LIB_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci22.h>

/**
  Checks if SAM is enabled or not.

  @retval FALSE  SAM is disabled
  @retval TRUE   SAM is enabled
**/
BOOLEAN
EFIAPI
IsSamEnabled (
  VOID
  );

/**
  GetSamFwInfo - This function performs SAM ready state and get its FW version info.

  This function does the following steps:
  (1) Examines SAM_READY (Register SAM_STATUS_1, bit 30)
  (2) If 0, retry in SAM_READY_TIMEOUT ms (1ms timeout)
  (3) Return & print  FW version in success, else return 0x0.

  @retval UINT32       - SAM FW Version if SAM is ready, else 0x0.
**/
UINT32
EFIAPI
GetSamFwInfo (
  VOID
  );

#endif
