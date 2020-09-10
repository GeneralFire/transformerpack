/** @file DxeCrystalRidgeFlushNearMemoryLib.c
  NM Flush DXE library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

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
**/

//
// Include files
//
#include <Library/CrystalRidgeFlushNearMemoryLib.h>
#include <Library/CmosAccessLib.h>
#include <PiDxe.h>
#include <Platform.h>

/**

  Routine description: This routine determines if NM Flush is needed upon S0>S5 transition or Warm reset

  @param[out] AdwbFlushTaskCount  Count the number of NM flush task needed for AppDirect-WB.

  @return EFI status is returned

**/
EFI_STATUS
DetermineNearMemoryFlushCapability(
    OUT UINTN *AdwbFlushTaskCount
    )
{
  //
  // Not supported in non-SMM
  //
  return EFI_UNSUPPORTED;
}

/**

  Routine description: This routine sets bit0 in CMOS Diagnostic Status register

  @param [in] Value - NM flush status bit value

  @return EFI status is returned

**/
EFI_STATUS
SignalNmFlushStatusToCmosReg (
  IN UINT8 Value
  )
{
  //
  // Not supported in non-SMM
  //
  return EFI_UNSUPPORTED;
}

/**

  Routine description: This routine retrieves the bit0 value from CMOS Diagnostic Status register

  @param None

  @return NM flush status bit value

**/
UINT8
GetNmFlushStatusFromCmosReg (
  VOID
  )
{
  UINT8 Data;

  //
  // supported in SMM and non-SMM for Crystal Ridge Protocol usages
  //

  Data = CmosRead8 (CMOS_DIAG_STAT_REG);
  if (Data & CMOS_DIAG_STAT_REG_NMFLUSH) {
    return (UINT8)SIG_NM_ERROR;
  }
  return (UINT8)SIG_NM_SUCCESS;
}


/**

  Routine description: Flush all near memory cache lines containing AppDirect data

  @return EFI status is returned

**/
EFI_STATUS
FlushNearMemoryCache(
    VOID
    )
{
   //
   // Not supported in non-SMM
   //
   return EFI_UNSUPPORTED;
}
