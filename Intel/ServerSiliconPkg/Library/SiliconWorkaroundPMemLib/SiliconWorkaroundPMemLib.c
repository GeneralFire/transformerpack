/** @file
  Silicon workaround library.
  Each SOC is required to provide an implementation of this
  library to handle silicon workarounds that are applicable to the
  said SOC.
  Different tables are created to enable specific workarounds that
  are depending on:
  Silicon environment (Silicon, Simics, CTE, emulation and etc.)
  Silicon stepping
  The array (a.k.a. table) must be ended with NULL to indicate the
  end of the list.
  For workaround that contains Sighting number, it is added to the
  table using the sighting number preceded with "S".

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <Library/SiliconWorkaroundPMemLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/SiliconWorkaroundHelperLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/EmulationConfigurationLib.h>

CONST CHAR8 *CwvA0WorkaroundTable[] = {
  "S14011274250",
  "S1508018006",
  "S14012067920",
  NULL
};

CONST CHAR8 *AllCwvSteppingsWorkaroundTable[] = {
  "S22011122815",
  NULL
};
/**
  Return whether the workaround is enabled.

  This function returns a boolean that determines whether the workaround is enabled
  given a workaround name.

  @param[in]      WorkaroundName         An ASCII string that represents the workaround name.
                                         This workaround name should correspond to an entry
                                         in the silicon workarounds table(s).

  @retval TRUE    The workaround is enabled.
  @retval FALSE   The workaround is not found in the table(s) and therefore disabled.

**/
BOOLEAN
EFIAPI
IsPmemSiliconWorkaroundEnabled (
  PSYSHOST          Host,
  UINT8             Socket,
  UINT8             Channel,
  UINT8             Dimm,
  IN CONST CHAR8    *WorkaroundName
  )
{
  BOOLEAN    WorkaroundEnabled;

  WorkaroundEnabled = FALSE;

  if (IsDcpmmPresentDimm (Socket, Channel, Dimm) == 0) {
    return FALSE;
  }

  if (UbiosGenerationOrHsleEnabled ()) {
    return FALSE;
  }

  if ((GetFmcType(Socket, Channel) == FMC_CWV_TYPE) && (GetFmcStepping (Host, Socket, Channel, Dimm) == FmcStepA0)) {
    WorkaroundEnabled = FindWorkaround (WorkaroundName, CwvA0WorkaroundTable);
    if (WorkaroundEnabled) {
      return WorkaroundEnabled;
    }
  }

  WorkaroundEnabled = FindWorkaround (WorkaroundName, AllCwvSteppingsWorkaroundTable);

  return WorkaroundEnabled;
}