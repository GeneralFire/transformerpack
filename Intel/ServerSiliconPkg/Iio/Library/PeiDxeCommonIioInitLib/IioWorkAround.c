/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#include <IioPlatformData.h>
#include <Library/OemIioInit.h>
#include <RcRegs.h>
#include <Chip/Include/SysHostChip.h>
#include <Library/IoLib.h>
#include "IioWorkAround.h"
#include <Library/SiliconWorkaroundLib.h>
#include <Chip/Include/CpuPciAccess.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/IioSysInfoLib.h>
#include <PcieRegs.h>
#include <Library/IioPcieInit.h>
#include <IioSetupDefinitions.h>
#include <LibraryPrivate/IioMiscWorkArounds.h>

/**

  Routine contains early workarounds before rootport bifurcation

  @param IioGlobalData - Pointer to IioGlobalData structure.
  @param IioIndex      - IIO index

  @retval None

**/
VOID
IioCommonWABeforeBif (
  IN  IIO_GLOBALS             *IioGlobalData,
  IN  UINT8                   IioIndex
  )
{
  //
  // Call CPU-specific private lib
  //
  IioWABeforeBif (IioGlobalData, IioIndex);

  //
  // Following are the Workarounds/ECOs which gets applied for each PCIe port
  //
  IioPcieWorkaroundsPreLinkPerSocket (IioGlobalData, IioIndex);
}

/**

 * Routine name: IioWorkAround()
 * Description: Routine contains switching control to early workaround routines

    @param IioGlobalData          - Pointer to IioGlobalData structure.
    @param IioIndex               - IIO index
    @param IIO_INIT_ENUMERATION   - Calling Phase used as switch control

    @retval None

**/
VOID
IioWorkAround (
  IN  IIO_GLOBALS                                     *IioGlobalData,
  IN  UINT8                                           IioIndex,
  IN  IIO_INIT_ENUMERATION                            Phase
  )
{
  switch(Phase) {
  case IioBeforeBifurcation:
    IioCommonWABeforeBif(IioGlobalData, IioIndex);
    break;
  case IioPortEnumProgramMISCCTRL:
    IioWAPortEnumCtrl(IioGlobalData, IioIndex);
    break;
  case IioPostInitEnd:
    IioPcieWorkaroundsPostLinkPerSocket (IioGlobalData, IioIndex);
    break;
  default:
    break;
  }

  // 600015278: Routine "OemIioEarlyWorkAround" is missing in Purley BIOS
  // OEM hook to override the DFX WA settings
  //
  OemIioEarlyWorkAround(IioGlobalData, IioIndex, Phase);

  return;
}
