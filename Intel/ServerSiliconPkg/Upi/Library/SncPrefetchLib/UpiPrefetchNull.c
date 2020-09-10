/** @file
  Files includes functions which is compatible in single-socket

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2020 Intel Corporation. <BR>

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
#include <UncoreCommonIncludes.h>
#include <Library/UncoreLib.h>
#include <Library/MultiSocketLib.h>
#include <KtiMisc.h>
#include <KtiSetupDefinitions.h>
#include <RcRegs.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/KtiApi.h>
#include <Guid/UboxIpInterface.h>
#include <Library/PcuIpLib.h>
#include "SncPrefetchLibInternal.h"


/**
  Routine to update KTIVAR KtiPrefetch value as the register in KTI Agent

  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval None

**/
VOID
UpdateKtiPrefetchStatusAsRegisterValue (
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
  )
{
  return;
}

/**
  Dump SNC related register in Kti Agents and M3KTI

  This routine is to dump SNC related register in Kti Agents

  @param  KtiInternalGlobal    -  KTIRC Internal Global data
  @param  Soc                  -  Socket Id

  @retval None

**/
VOID
DumpKtiAgentM3KtiSncXptRegister (
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN UINT8                    Soc
  )
{
  return;
}

/**
  This routine is to disable xpt remote prefetch feature

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param Socket            - Socket Id

  @retval None

**/
VOID
DisableRemoteXpt (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      Socket
  )
{
  return;
}

/**
  This routine is to enable xpt remote prefetch feature

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param  Socket           - Socket Id
  @param  MemInfo          - Memory Information structure.

  @retval None

**/
VOID
ProgramRemoteXpt (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                  Socket,
  IN MEM_INFO               *MemInfo
  )
{
  return;
}

/**
  Program Kti Prefetch setting

  This routine is to enable kti prefetch feature

  @param  KtiInternalGlobal - KTIRC Internal Global data
  @param  Socket            - Socket Id
  @param  Minisadmodeselect - Minisad mode
  @param  MiniSadTable      - Minisad table value

  @retval None

**/
VOID
SetupUpiMiniSad (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Socket,
  UINT32                     Minisadmodeselect,
  UINT32                     MiniSadTable
  )
{
  return;
}

/**
  Enable Preftch in KTI Agent
  For KTI prefetch, need to configure: ktiagctrl.enableprefetch
  For remote XPT prefetch, need to configure: ktiagctrl.enableprefetch

  This routine is to enable prefetch in KTI agent.

  @param  KtiInternalGlobal            -  KTIRC Internal Global data
  @param  Socket                       -  Socket Id
  @param  KtiOrRemoteXptPrefetchEnable:
           0: KtiXptPrefetch Enabled
           1: RemoteXptPrefetch Enabled
           2: KtiXptPrefetch Disabled
           3: RemoteXptPrefetch Disabled

  @retval None

**/
VOID
EnablePrefetchInKtiAgent (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Socket,
  UINT8                      KtiOrRemoteXptPrefetchEnable
  )
{
  return ;
}

/**
  Program SNC config register in Kti Agents

  This routine is to program SNC config register in Kti Agents

  @param  KtiInternalGlobal    -  KTIRC Internal Global data
  @param  Soc                  -  Socket Id
  @param  SncConfigData        -  SNC Config Data
  @param  SncConfigMs2IdiData  -  SNC Config Data in Mesh2IDI

  @retval None

**/
VOID
ProgramSncConfigInKtiAgent (
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN UINT8                    Soc,
  IN UINT32                   SncConfigData,
  IN UINT32                   SncConfigMs2IdiData
  )
{
  return;
}