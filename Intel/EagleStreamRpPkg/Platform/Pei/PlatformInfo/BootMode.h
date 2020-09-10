/** @file
  BootMode header file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2019 Intel Corporation. <BR>

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

#ifndef _EFI_BOOT_MODE_H_
#define _EFI_BOOT_MODE_H_

#include <Base.h>
#include <PiPei.h>
#include <Uefi.h>
#include <Platform.h>
#include <BackCompatible.h>
#include <Guid/SetupVariable.h>
#include <Guid/AdminPasswordGuid.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Library/SetupLib.h>
#include <Library/PchInfoLib.h>
#include <Library/GpioLib.h>
#include <Register/PchRegsPmc.h>
#ifdef CDF_SC_FLAG
#include <Register/PchRegsPmc.h>
#include <Register/PchRegs.h>
#endif
#include <Library/MmPciBaseLib.h>
#include <Ppi/BootInRecoveryMode.h>
#include <IndustryStandard/SmBios.h>

/**
  Parse the status registers for figuring out the wake-up event.

  @param[out]  WakeUpType       Updates the wakeuptype based on the status registers
**/
VOID
GetWakeupEvent (
  OUT  UINT8   *WakeUpType
  );

/**
  Admin password is read and saved to HOB
  to prevent its cleared on defaults load.

  @param PeiServices          - pointer to the PEI Service Table
  @param SystemConfiguration  - pointer to Platform Setup Configuration

  @retval None.

**/
VOID
SaveAdminPassToHob (
  IN CONST EFI_PEI_SERVICES **PeiServices,
  IN SYSTEM_CONFIGURATION   *SystemConfiguration
  );

/**
  Get sleep type after wakeup

  @param PeiServices       Pointer to the PEI Service Table.
  SleepType                Sleep type to be returned.

  @retval TRUE              A wake event occured without power failure.
  @retval FALSE             Power failure occured or not a wakeup.

**/
BOOLEAN
GetSleepTypeAfterWakeup (
  IN  CONST EFI_PEI_SERVICES    **PeiServices,
  OUT UINT16                    *SleepType
  );

/**

    Check to see if CMOS is bad or cleared

    @param None

    @retval TRUE  - CMOS is bad
    @retval FALSE - CMOS is good

**/
BOOLEAN
IsCMOSBad(
     VOID
  );

/**

    Determines the Boot Mode of the platform and sets the Boot Mode variable

    @param[in]  PeiServices       General purpose services available to every PEIM.
    @param[in]  NotifyDescriptor  Notify that this module published.
    @param[in]  Ppi               PPI that was installed.

    @retval Status - Success if BootMode was set

**/
EFI_STATUS
UpdateBootMode (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );
#endif
