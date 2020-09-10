/** @file
  This file includes functions to handle Uncore configurations after MRC.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2019 Intel Corporation. <BR>

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


#include <Library/MemoryCoreLib.h>
#include <RcRegs.h>
#include <KtiSetupDefinitions.h>
#include <Upi/KtiSi.h>
#include <KtiMisc.h>
#include <UncoreCommonIncludes.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemRcLib.h>
#include <Pi/PiHob.h>
#include <Library/HobLib.h>
#include <Library/KtiApi.h>
#include <Library/UncoreLib.h>
#include <Library/MultiSocketLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/KtiApi.h>
#include <Library/ReferenceCodePolicyLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <ScratchpadList.h>
#include <Guid/UboxIpInterface.h>
#include <Guid/IioIpInterface.h>
#include <Guid/SocketCommonRcVariable.h>
#include "UncoreLibInternal.h"


/**
  Update IsocEn knob setting based on AEP DIMM presence detection.
  This code needs to be called before BIOS updates ChaToM2m credit programming.

  @param[in] KtiInternalGlobal    KTIRC Internal Global data

**/
VOID
UpdateIsocEnSetting (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  BOOLEAN    AepDimmPresent;

  UBoxCheckAepDimmPresentFlagSetInScratchpad (KtiInternalGlobal->SbspSoc, 0, &AepDimmPresent);

  if (AepDimmPresent) {
    KTIVAR->OutIsocEn = FALSE;
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nPMem DIMM Presence detected. Forcing KTIVAR->OutIsocEn to %d\n", KTIVAR->OutIsocEn));
  }
}