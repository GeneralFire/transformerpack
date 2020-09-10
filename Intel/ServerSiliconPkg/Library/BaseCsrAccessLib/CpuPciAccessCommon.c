/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include <Chip/Include/CpuPciAccess.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <UncoreCommonIncludes.h>
#include <Library/SysHostPointerLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemoryServicesLib.h>

#include <Library/UpiHostDataLib.h>
#include <Library/KtiApi.h>

/**

  Update CpuCsrAccessVar structure.

  @param CpuCsrAccessVar - pointer to CpuCsrAccessVar structure to be populated

  @retval None

**/
VOID
UpdateCpuCsrAccessVar_PEI (
  OUT CPU_CSR_ACCESS_VAR *CsrAccessVarPtr
  )
{

#if defined(IA32)
  UINT8 Socket, Stack;
  UINT8 MemoryController;
  UINT8 Channel;
  KTI_HOST_OUT *KtiHostOutPtr;
  PSYSHOST     Host;
  SYS_SETUP    *Setup;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;

  Host = GetSysHostPointer();

  if ((Host == NULL)              ||
      (CsrAccessVarPtr == NULL))  {
    ASSERT (FALSE);
    return;
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

    CsrAccessVarPtr->stackPresentBitmap[Socket] = KtiHostOutPtr->stackPresentBitmap[Socket];

    CopyMem (
      CsrAccessVarPtr->StackBus[Socket],
      KtiHostOutPtr->StackBus[Socket],
      sizeof (CsrAccessVarPtr->StackBus[0])
      );

    CsrAccessVarPtr->SocketFirstBus[Socket] = KtiHostOutPtr->SocketFirstBus[Socket];
    CsrAccessVarPtr->SocketLastBus[Socket]  = KtiHostOutPtr->SocketLastBus[Socket];
    CsrAccessVarPtr->segmentSocket[Socket]  = KtiHostOutPtr->segmentSocket[Socket];
    CsrAccessVarPtr->mmCfgBaseH[Socket] = KtiHostOutPtr->mmCfgBaseH[Socket];
    CsrAccessVarPtr->mmCfgBaseL[Socket] = KtiHostOutPtr->mmCfgBaseL[Socket];

    for (MemoryController = 0; MemoryController < MAX_IMC; MemoryController++) {
      CsrAccessVarPtr->imcEnabled[Socket][MemoryController] = Host->var.mem.socket[Socket].imcEnabled[MemoryController];
    }

    for (Channel = 0; Channel < MAX_CH; Channel++) {
      CsrAccessVarPtr->mcId[Socket][Channel] = Host->var.mem.socket[Socket].channelList[Channel].mcId;
    }

    CopyMem (
      CsrAccessVarPtr->MmioBar[Socket],
      KtiHostOutPtr->MmioBar[Socket],
      sizeof (CsrAccessVarPtr->MmioBar[0])
      );
    CsrAccessVarPtr->Socket10nmUboxBus0[Socket] = KtiHostOutPtr->Socket10nmUboxBus0[Socket];

    for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
      CsrAccessVarPtr->StackMmiol[Socket][Stack] = KtiHostOutPtr->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiolBase;
    }
  }

  CsrAccessVarPtr->socketPresentBitMap  = GetSocketPresentBitMap ();
  CsrAccessVarPtr->FpgaPresentBitMap    = KtiHostOutPtr->FpgaPresentBitMap;
  CsrAccessVarPtr->SlaveDieBitMap       = KtiHostOutPtr->SlaveDieBitMap;
  CsrAccessVarPtr->mmCfgBase            = KtiHostOutPtr->mmCfgBase;
  CsrAccessVarPtr->KtiPortCnt           = KtiHostOutPtr->KtiPortCnt;
  CsrAccessVarPtr->HbmSku               = KtiHostOutPtr->HbmSku;

  Setup = GetSysSetupPointer ();
  if (Setup != NULL) {
    CsrAccessVarPtr->PostedCsrAccessAllowed = Setup->mem.dfxMemSetup.DfxPostedCsrAccessAllowed;
    CsrAccessVarPtr->SocketConfig = Setup->common.SocketConfig;
  } else {
    CsrAccessVarPtr->PostedCsrAccessAllowed = FALSE;
  }

  CsrAccessVarPtr->DataPopulated = TRUE; // We have provided CSR access var data

#endif //defined (IA32) || defined (SIM_BUILD) || defined (HEADLESS_MRC)
  return;

}

