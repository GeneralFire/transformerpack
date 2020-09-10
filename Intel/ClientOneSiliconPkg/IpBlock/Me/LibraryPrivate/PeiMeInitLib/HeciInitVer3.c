/** @file
  Framework PEIM to HECI.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

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

#include <Ppi/HeciPpi.h>
#include <Ppi/HeciControlPpi.h>
#include <Ppi/SiPolicy.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiMeLib.h>
#include <Library/PostCodeLib.h>
#include <Library/PmcLib.h>
#include <Library/HeciInitLib.h>

//
// Function Implementations
//

/**
  Initialize MMIO BAR address for HECI devices
  For SPS HeciControl is used (it has self initialization)

  @retval EFI_SUCCESS             HECI devices initialize successfully
  @retval others                  Error occur
**/
EFI_STATUS
PeiHeciDevicesInit (
  VOID
  )
{
  // HECI initialization is made using HeciControl
  return EFI_SUCCESS;
}

/**
  Internal function performing PM register initialization for Me
**/
VOID
MePmInit (
  VOID
  )
{
  ///
  /// Before system memory initialization, BIOS should check the wake status to determine if Intel Management Engine
  /// has reset the system while the host was in a sleep state. If platform was not in a sleep state
  /// BIOS should ensure a non-sleep exit path is taken. One way to accomplish this is by forcing
  /// an S5 exit path by the BIOS.
  ///
  if (PmcGetSleepTypeAfterWake () == PmcNotASleepState) {
    PmcSetSleepState (PmcS5SleepState);
    DEBUG ((DEBUG_INFO, "MePmInit () - Force an S5 exit path.\n"));
  }
}

