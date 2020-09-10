/** @file
  This file includes common functions for UPI prefetch

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
#include <Library/UncoreLib.h>
#include <Library/MultiSocketLib.h>
#include <KtiMisc.h>
#include <UncoreCommonIncludes.h>
#include <Library/KtiApi.h>
#include <Library/MemRcLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Guid/KtiIpInterface.h>
#include <Library/CpuAndRevisionLib.h>

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
  UINT8                       Socket;

  KTIVAR->OutKtiPrefetch = KTI_DISABLE;
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    // check if socket is valid
    if (SocketPresent (Socket)) {
      KTIVAR->OutKtiPrefetch = GetKtiPrefetch (Socket, 0);
      if (KTIVAR->OutKtiPrefetch == KTI_ENABLE) {
        break;
      }
    }
  }
}

/**
  Dump SNC related register in Kti Agents and M3KTI

  This routine is to dump SNC related register in Kti Agents and M3KTI

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
  UINT8  Ctr, M3KtiCtr, ProgrammedM3Kti;

  ProgrammedM3Kti = M3KtiCtr = 0xff;
  for (Ctr = 0; Ctr < KtiInternalGlobal->KtiPortCnt; Ctr++) {
    if (KTIVAR->CpuInfo[Soc].LepInfo[Ctr].Valid == TRUE){
      DumpKtiLlPmonSncXptRegister (Soc, Ctr);
      if (!IsSkxFamilyCpu(KtiInternalGlobal->CpuType)) {
        DumpM3KtiSncXptRegister (Soc, Ctr);
      } else {
        //
        // SKX + CPX, KTI port 0 & 1 share 1 M3KTI0, KTI port 2 owns M3KTI1
        //
        M3KtiCtr = PORT_TO_M3KTI (Ctr);
        if (ProgrammedM3Kti != M3KtiCtr) {
          DumpM3KtiSncXptRegister (Soc, M3KtiCtr);
          ProgrammedM3Kti = M3KtiCtr;
        }
      }
    }
  }
}