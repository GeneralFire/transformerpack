/** @file
  KTI *_M3KTI_MAIN_REG related registers access.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <Uefi.h>
#include <Base.h>
#include <Guid/KtiIpInterface.h>
#include "Include/KtiRegs.h"
#include <Library/UsraCsrLib.h>
#include <Library/DebugLib.h>
#include <Library/UpimdfIpAccessLib.h>

/**
  Returns the total number of M3KTI agents

  @param SocId             - Soc ID with M3KTI agents
  @param MaxM3kti          - Max M3KTI agents

  @retval number of M3KTI

**/
UINT8
EFIAPI
GetM3KtiCount (
  UINT8                      SocId,
  UINT8                      MaxM3kti
  )
{
  UINT8   Ctr;
  UINT8   Count = 0;
  UINT16  Data16;

  for (Ctr = 0; Ctr < MaxM3kti; Ctr++) {
    Data16 = (UINT16) UpimdfIpReadAccess32 (SocId, Ctr, 0, VID_M3KTI_MAIN_REG);
    if (Data16 != 0xFFFF) {
      Count++;
    }
  }

  return Count;
}

/**
  Routine to program M3KTI MiniSAD registers

  @param SocId             - Socket to program
  @param M3Kti             - M3Kti number
  @param UpiMiniSADMode    - MiniSAD Mode for M3KPRECTRL
  @param UPIMiniSad        - MiniSAD Table for M3KPRETL

  @retval None
**/
VOID
EFIAPI
ProgramM3ktiMiniSad (
  UINT8                       SocId,
  UINT8                       M3Kti,
  UINT32                      UpiMiniSadMode,
  UINT32                      UPIMiniSad
  )
{
  M3KPRECTRL_M3KTI_MAIN_STRUCT         M3KPreCtrl;
  //
  // mini sad mode select
  //
  M3KPreCtrl.Data = UpimdfIpReadAccess32 (SocId, M3Kti, 0, M3KPRECTRL_M3KTI_MAIN_REG);
  M3KPreCtrl.Bits.mini_sad_mode = UpiMiniSadMode;
  //
  // 4929504: KTI tile performance critical CSRs for BIOS programming
  //
  M3KPreCtrl.Bits.remspecrdthreshold = 2;
  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3KPRECTRL_M3KTI_MAIN_REG, M3KPreCtrl.Data);
  //
  //set mini sad table
  //
  UpimdfIpWriteAccess32 (SocId, M3Kti, UPIMDF_INSTANCE_BROADCAST, M3KPRETL_M3KTI_MAIN_REG, UPIMiniSad);
}

/**
  Dump SNC related register in Kti Agents and M3KTI

  This routine is to dump SNC related register in Kti Agents and M3KTI

  @param  KtiInternalGlobal    -  KTIRC Internal Global data
  @param  Soc                  -  Socket Id

  @retval None

**/
VOID
EFIAPI
DumpM3KtiSncXptRegister (
  IN UINT8                    Soc,
  IN UINT8                    M3Kti
  )
{
  DEBUG ((DEBUG_INFO, "\nM3KTI %u:", M3Kti));
  DEBUG ((DEBUG_INFO, "\nM3KPRECTRL_%d         : 0x%08X", M3Kti, UpimdfIpReadAccess32 (Soc, M3Kti, 0, M3KPRECTRL_M3KTI_MAIN_REG)));
  //
  //mini sad table
  //
  DEBUG ((DEBUG_INFO, "\nM3KPRETL_%d           : 0x%08X", M3Kti, UpimdfIpReadAccess32 (Soc, M3Kti, 0, M3KPRETL_M3KTI_MAIN_REG)));
}