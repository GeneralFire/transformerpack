/** @file
  Snc and prefetch configuration entry point.

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

#include <Library/OemProcMemInitLib.h>
#include <Uefi.h>
#include <Library/PeiServicesLib.h>
#include <KtiMisc.h>
#include <KtiSetupDefinitions.h>
#include <Upi/KtiSi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/KtiApi.h>
#include <Library/IoAccessLib.h>
#include <ScratchpadList.h>
#include <Library/ReferenceCodePolicyLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/KtiSimLib.h>
#include <Guid/UboxIpInterface.h>
#include <Guid/SocketCommonRcVariable.h>
#include <Library/SncPrefetchLib.h>
#include "SncPrefetchLibInternal.h"


/**
  Disable SNC Flag

  This routine to disable SNC flag for warm reset

  @param  KtiInternalGlobal - KTIRC Internal Global data

  @retval None

**/
VOID
DisabledSncFlag (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8                   SbspId;

  //
  // Set the flag in scratchpad to indicate the populated memory is not suitable for SNC mode operation.
  // Trigger a warm reset to reconfigure the system to operate in non-SNC mode.
  //
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nFail to boot in SNC mode!\n"));
  KTIVAR->OutSncEn = 0;
  KTIVAR->OutUmaClustering = 0;

  //
  // Re-initialize OutSncPrefetchEn variable
  //
  KTIVAR->OutSncPrefetchEn = (KTIVAR->OutKtiPrefetch | KTIVAR->OutXptPrefetch | KTIVAR->OutXptRemotePrefetch);

  //
  // Set SNC Mode Enable failure/recovery flag
  //
  SbspId = GetSysSbspSocketIdNv ();
  UBoxSetSncModeFailRecoveryFlagInScratchpad (SbspId, 0, 1);
}

/**
  Disable Prefetch Flag

  This routine to disable Prefetch flag for warm reset

  @param  KtiInternalGlobal - KTIRC Internal Global data

  @retval None

**/
VOID
DisabledPrefetchFlag (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8                   SbspId;

  //
  // Update internal variables to indicate the populated memory is not suitable for Prefetch mode operation.
  // Trigger a warm reset to reconfigure the system to operate in non-Prefetch mode.
  //
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nFail to boot in Prefetch mode!\n"));
  KTIVAR->OutKtiPrefetch = 0;
  KTIVAR->OutXptPrefetch = 0;
  KTIVAR->OutXptRemotePrefetch = 0;

  //
  // Re-initialize OutSncPrefetchEn variable
  //
  KTIVAR->OutSncPrefetchEn = (KTIVAR->OutSncEn | KTIVAR->OutUmaClustering);

  //
  // Set Prefetch Mode Enable failure/recovery flag
  //
  SbspId = GetSysSbspSocketIdNv ();
  UBoxSetPrefetchModeFailRecoveryFlagInScratchpad (SbspId, 0, 1);
}

/**
  Programs SNC Mode

  This routine programs the SNC mode.

  @param  KtiInternalGlobal- KTIRC Internal Global data
  @param  MemInfo          - Memory Information structure.

  @retval UNCORE_SUCCESS   - SNC mode is programmed successfully.
  @retval UNCORE_FAILURE   - Memory profile is not suitable for SNC mode operation.

**/
KTI_STATUS
EnableSncMode (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo
  )
{
  KTI_STATUS     Status;

  Status = KTI_SUCCESS;

  Status = CalculateClusterBase (KtiInternalGlobal, MemInfo);

  if (Status == KTI_FAILURE) {
    //
    // Set the flag in scratchpad to indicate the populated memory is not suitable for SNC or UMA Clustering mode operation.
    // Trigger a warm reset to reconfigure the system to operate in non-SNC and non-UMA-Cluster mode.
    //
    if (KTIVAR->OutSncEn != KTI_DISABLE) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nRequest disable SNC and issue WARM RESET!"));
      DisabledSncFlag (KtiInternalGlobal);
      if (IsSkxFamilyCpu (KtiInternalGlobal->CpuType)) {
        //
        // In Purley SKX/CLX BIOS, recovery flow always disables SNC and prefetch at the same time.
        // Starting with ICX/SPR BIOS, we may allow SNC and prefetch to be disabled individually in the recovery flow.
        //
        DisabledPrefetchFlag (KtiInternalGlobal);
      }
      SetNewResetRequired (POST_RESET_WARM);
    }
    if (KTIVAR->OutUmaClustering == UMABASEDCLUSTERING_HEMISPHERE) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nUMA based clustering downgrade from Hemi to Disabled mode!\n"));
      UBoxSetUmaBasedClusteringDowngradeInScratchpad (GetSysSbspSocketIdNv (), 0, HEMI2DISABLE);
      KTIVAR->OutUmaClustering = UMABASEDCLUSTERING_DISABLED;
      SetNewResetRequired (POST_RESET_WARM);
    }
    if (KTIVAR->OutUmaClustering == UMABASEDCLUSTERING_QUADRANT) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nUMA based clustering downgrade from Quad to Disabled mode!\n"));
      UBoxSetUmaBasedClusteringDowngradeInScratchpad (GetSysSbspSocketIdNv (), 0, QUAD2DISABLE);
      KTIVAR->OutUmaClustering = UMABASEDCLUSTERING_DISABLED;
      SetNewResetRequired (POST_RESET_WARM);
    }
  } else if (Status == KTI_SNC_CHANGED) {
    //
    // Trigger a warm reset to reconfigure the system to operate in non-SNC mode.
    //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nRequest to change number of snc cluster and issue WARM RESET!"));
    SetNewResetRequired (POST_RESET_WARM);
  }

  //
  // Write to SNC related registers now
  //
  ConfigureSncMode (KtiInternalGlobal, MemInfo, Status);

  return Status;
}