/** @file
  File inclues functions to configure UMA based clustering related settings.

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
#include <Library/MemMcIpLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/MemRcLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/KtiApi.h>
#include <Library/IoAccessLib.h>
#include <ScratchpadList.h>
#include <Library/ReferenceCodePolicyLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/KtiSimLib.h>
#include <Library/IoLib.h>
#include <RcRegs.h>
#include <Library/ChaLib.h>
#include <Guid/UboxIpInterface.h>
#include <Library/Ms2IdiLib.h>
#include <Library/OobIpLib.h>
#include <Library/PcuIpLib.h>
#include <Library/M2IalIpLib.h>
#include <Guid/KtiIpInterface.h>
#include <Guid/IioIpInterface.h>
#include <Guid/MsmIpInterface.h>
#include <Library/CsrAccessLib.h>
#include <Library/PmcSetStrapMessageLib.h>
#include <Library/MemTypeLib.h>
#include <Library/CxlLib.h>
#include <Guid/SocketCommonRcVariable.h>
#include <Library/SncPrefetchLib.h>
#include "SncPrefetchLibInternal.h"


/**
  Program Uma Cluster Config

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param  Socket           - Socket

  @retval None

**/
VOID
ProgramUmaClusterConfig (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  UINT8                    Socket
  )
{
  UINT8   Stack, LinkIndex;
  UINT8   UmaClusterEnable;
  UINT8   DefeatureXor;
  UINT32  UmaClusterCfg;

  LinkIndex = 0;
  //
  // Program UMA_CLUSTER_CFG for MS2IDI/IIO/KTI in cold reset
  //

  UmaClusterEnable  = 0;
  DefeatureXor      = 0;
  UmaClusterCfg     = 0;
  if ((KTIVAR->OutSncEn == KTI_DISABLE) && (KTIVAR->OutUmaClustering != KTI_DISABLE)) {
    UmaClusterEnable = 1;
  }

  //
  // Defeature_Xor doesn't support in ICX. Should clear it.
  //
  DefeatureXor = 0;
  UmaClusterCfg = Ms2IdiSetUmaClusterCfg (Socket, UmaClusterEnable, DefeatureXor);

  //
  // Program UMA_CLUSTER_CONFIG_IIO_VTD_REG
  //
  for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
    if (StackPresent (Socket, Stack)) {
      IioSetUmaClusterCfg (Socket, Stack, UmaClusterEnable, DefeatureXor);
    }
  }

  for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
    if (KTIVAR->CpuInfo[Socket].LepInfo[LinkIndex].Valid == TRUE) {
      ProgramKtiAgentUmaCluster (Socket, LinkIndex, UmaClusterEnable, DefeatureXor);
    }
  }
}

/**
  Routine to Check UMA based clustering downgrade

  @param[in]  KtiInternalGlobal   Pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS
  @retval KTI_IGNORE

**/
KTI_STATUS
CheckUmaBasedClusteringDowngrade (
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
  UINT8                       UmaBasedClustingDowngrade;

  //
  // SP7 bit [22:21] indicates previous UMA based clustering degrade happened
  //
  if (EFI_SUCCESS == UBoxGetUmaBasedClusteringDowngradeInScratchpad (\
                       KtiInternalGlobal->SbspSoc, 0, &UmaBasedClustingDowngrade)) {
    if (UmaBasedClustingDowngrade == HEMI2DISABLE) {
      // UMA Hemi->Disabled mode
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n   UMA Hemi->Disabled detected!\n"));
      KTIVAR->OutUmaClustering = UMABASEDCLUSTERING_DISABLED;
      KTIVAR->OutSncEn = 0;
      KTIVAR->UmaClusteringDowngrade = TRUE;
      KTIVAR->OutSncPrefetchEn = (KTIVAR->OutKtiPrefetch | KTIVAR->OutXptPrefetch | KTIVAR->OutXptRemotePrefetch);
    }
  }

  return KTI_SUCCESS;
}