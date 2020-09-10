/** @file
  Null instance of library MeEnahncedDebugSupportLib.

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

#include <Library/ChipsetInitLib.h>

/**
  This function retrieves Chipset Init Table header from MeEDebugHob.

  @param[out] CsmeChipsetInitVerInfo  Version Info of Chipset Init Table held by CSME

  @retval EFI_SUCCESS                 Function completed successfully
  @retval EFI_INVALID_PARAMETER       Data Buffer is NULL
  @retval EFI_NOT_FOUND               MeEDebugHob not found
  @retval EFI_UNSUPPORTED             Not supported
**/
EFI_STATUS
GetEDebugModeChipsetInitHeader (
  OUT PCH_HSIO_VER_INFO *CsmeChipsetInitVerInfo
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Check if Firmware runs in EDebug Mode

  @retval TRUE            Firmware runs in EDebug Mode.
  @retval FALSE           Firmware is not running in EDebug Mode.
**/
BOOLEAN
IsEDebugMode (
  VOID
  )
{
  return FALSE;
}

/**
  Check if Firmware requests the platform to continue booting in EDebug Mode

  @retval TRUE            EDebug mode requested.
  @retval FALSE           EDebug mode not requested.
**/
BOOLEAN
IsInvokeEDebugModeRequested (
  VOID
  )
{
  return FALSE;
}

/**
  This function will check if the platform should continue the boot and transfer
  from EDebugMode to Normal Mode. The following outcome is possible:
    - transfer to Normal Mode and continue the boot
    - remain in Debug Mode and disable HECI1 interface

  @param[in] TablesMatched   Informs if CSME holds the same Chipset Init Tables as BIOS
**/
VOID
ConfigureEDebugMode (
  IN BOOLEAN TablesMatched
  )
{
  return;
}

/**
  Init and install Enhanced Debug Hob.
**/
VOID
InstallEDebugHob (
  VOID
  )
{
  return;
}

