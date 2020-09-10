/** @file
  DebugPrintErrorLevel access library instance.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2020 Intel Corporation. <BR>

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

#include "DebugPrintErrorLevelLibCmosCommon.h"

UINT32 mDebugPrintErrorLevel  = 0xFFFFFFFF;

/**
  Returns if the TRUE when the input address is within the CODE NEM region

  @param   address     Address to check

  @retval  TRUE        The address is within the code NEM range
  @retval  FALSE       The address is NOT within the code NEM range

**/
BOOLEAN
EFIAPI
isCodeNEMRegionAddress(
  UINTN address
)
{
  UINTN start = 0;
  UINTN end = 0;
  start = FixedPcdGet32 (PcdFlashSecCacheRegionBase);
  end = start + (FixedPcdGet32 (PcdFlashSecCacheRegionSize) - 1);
  return (address >= start && address <= end);
}

/**
  Returns the debug print error level mask for the current module.

  @return  Debug print error level mask for the current module.

**/
UINT32
EFIAPI
GetErrorLevel (
  VOID
  )
{
  UINT32 ErrorLevel = 0;
  UINT32 *ErrorLevelCache = NULL;

  if (PcdGet8(PcdDebugPropertyMask) == 0) {
    return ErrorLevel;
  }

  if (mDebugPrintErrorLevel == 0xFFFFFFFF) {
    ErrorLevelCache = GetDataCache (&gErrorLevelDataCacheGuid);
    if (ErrorLevelCache == NULL) { // Only use CMOS if faster cache fails.
      ErrorLevel = CmosRead32(PcdGet8(PcdCmosDebugPrintLevelReg));
      SetDataCache (&gErrorLevelDataCacheGuid, &ErrorLevel, sizeof (ErrorLevel));
    } else {
      ErrorLevel = *ErrorLevelCache;
    }
    if(!isCodeNEMRegionAddress((UINTN)(&mDebugPrintErrorLevel))) {
      mDebugPrintErrorLevel = ErrorLevel;
    }
    return ErrorLevel; // Must use local var - global fails in pre-mem.
  }

  return mDebugPrintErrorLevel;
}

/**
  Sets the global debug print error level mask fpr the entire platform.

  @param   ErrorLevel     Global debug print error level

  @retval  TRUE           The debug print error level mask was sucessfully set.
  @retval  FALSE          The debug print error level mask could not be set.

**/
BOOLEAN
EFIAPI
SetErrorLevel (
  IN UINT32 ErrorLevel
  )
{
  CmosWrite32 (PcdGet8(PcdCmosDebugPrintLevelReg), ErrorLevel);
  SetDataCache (&gErrorLevelDataCacheGuid, &ErrorLevel, sizeof (ErrorLevel));
  if(!isCodeNEMRegionAddress((UINTN)(&mDebugPrintErrorLevel))) {
    mDebugPrintErrorLevel = ErrorLevel;
  }

  return EFI_SUCCESS;
}
