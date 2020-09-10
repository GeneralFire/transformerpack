/** @file
  Definitions for Me Enhanced Debug Lib

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

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
#ifndef _ME_ENHANCED_DEBUG_LIB_H
#define _ME_ENHANCED_DEBUG_LIB_H

/**
  This function retrieves Chipset Init Table header from MeEDebugHob.

  @param[out] CsmeChipsetInitVerInfo  Version Info of Chipset Init Table held by CSME

  @retval EFI_SUCCESS                 Function completed successfully
  @retval EFI_INVALID_PARAMETER       Data Buffer is NULL
  @retval EFI_NOT_FOUND               MeEDebugHob not found
**/
EFI_STATUS
GetEDebugModeChipsetInitHeader (
  OUT PCH_HSIO_VER_INFO *CsmeChipsetInitVerInfo
  );

/**
  Check if Firmware runs in EDebug Mode

  @retval TRUE            Firmware runs in EDebug Mode.
  @retval FALSE           Firmware is not running in EDebug Mode.
**/
BOOLEAN
IsEDebugMode (
  VOID
  );

/**
  This function will check if the platform should continue the boot in EDebug
  Mode. The following outcome is possible:
    - enter EDebug mode and continue the boot
    - enter Edebug mode and perform global reset
    - remain in Debug mode

  @param[in] TablesMatched   Informs if CSME holds the same Chipset Init Tables as BIOS
**/
VOID
ConfigureEDebugMode (
  IN BOOLEAN TablesMatched
  );

/**
  Init and install Enhanced Debug Hob.
**/
VOID
InstallEDebugHob (
  VOID
  );

#endif //_ME_ENHANCED_DEBUG_LIB_H

