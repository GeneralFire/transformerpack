/** @file
  This file initialize the Vtd registers

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
#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>

#include <RcRegs.h>

#include <Library/SiliconWorkaroundLib.h>

#include <Library/IioDataHubLib.h>
#include <Library/IioAccessLib.h>
#include <Library/IioVtdInit.h>
#include <LibraryPrivate/IioDebug.h>
#include <Private/Library/PchPsfPrivateLib.h>

/**
  Init if necessary and return VTd BAR

  @param IioGlobalData - pointer to IioGlobalData structure
  @param IioIndex      - IIO index to be processed
  @param Stack         - Index of the stack in socket
 */
UINTN
ProgramVtdBar (
    IN IIO_GLOBALS        *IioGlobalData,
    IN UINT8              IioIndex,
    IN UINT8              Stack
  )
{
  UINT32     VtdBar;

  //
  // For now only old-style init BAR based on value reserved in IioGlobalData
  //
  VtdBar = IioGlobalData->IioVar.IioVData.VtdBarAddress[IioIndex][Stack];
  IioWriteCpuCsr32 (IioIndex, Stack, VTBAR_N0_IIO_VTD_REG, VtdBar | B_IIO_VT_BAR_EN);

  return VtdBar;
}

/**
  IIO VTD initialization on given socket

  @param IioGlobalData - pointer to IioGlobalData structure
  @param IioIndex      - IIO index to be processed
**/
VOID
IioVtdInit (
  IN IIO_GLOBALS        *IioGlobalData,
  IN UINT8              IioIndex
  )
{
  UINTN      VtdBar;
  UINT8      Stack;


  if (!IioGlobalData->SetupData.VTdSupport) {
    //
    // VTd is not enabled - skip the rest of initialization
    //
    IIO_D_VTDDBG ("VT-d Initialization disabled.\n");
    return;
  }

  PchPsfEnableVtd ();

  for (Stack=0; Stack < MAX_IIO_STACK; Stack++) {

    // Check for a valid stack
    if (!(IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex] & (1 << Stack))){
      continue;
    }

    VtdBar = ProgramVtdBar (IioGlobalData, IioIndex, Stack);
    IIO_D_VTDLOG ("[%d.%d] VT-d initialization, BAR=%p\n",
                  IioIndex, Stack, VtdBar);
  }
}
