/** @file
  This implements filling out the HECI Message responsible of passing
  CPU Info data.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2010 - 2020 Intel Corporation. <BR>

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
#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/MpService.h>
#include <Library/DebugLib.h>
#include <Register/Cpuid.h>
#include <Guid/SpsInfoHobGuid.h>
#include <Protocol/HeciControlProtocol.h>
#include <Protocol/CpuInfo.h>
#include <Sps.h>
#include <Library/MeTypeLib.h>
#include <Library/MeUtilsLib.h>
#include <Register/ArchitecturalMsr.h>
#include "SpsDxe.h"
#include <CpuRegs.h>
#include <Register/ArchMsr.h>
#include <CpuPowerMgmt.h>
#include <Library/SpsDxeLib.h>
#include <Cpu/CpuCoreRegs.h>

/**
  Gather and send to ME Host configuration data needed by Node Manager

  @param[in] pSpsContext    Sps Dxe context

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
SpsNmSendHostCfgInfo (
  SPS_DXE_CONTEXT       *pSpsContext
  )
{
  EFI_STATUS                    Status;
  UINT32                        HeciMsgLen;
  UINT32                        RspLen;
  UINT32                        Reg;
  UINT32                        Index;
  HECI_CONTROL                  *Heci;
  MKHI_MSG_NM_HOST_CFG_BUFFER   *HeciMsg;

  DEBUG ((DEBUG_INFO, "[SPS] %a ()\n", __FUNCTION__));

  if (MeIsInRecovery ()) {
    DEBUG ((DEBUG_WARN, "[SPS] WARNING: SPS in recovery. Do not send Host Configuration\n"));
    return EFI_UNSUPPORTED;
  }

  Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, &Heci);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot locate HECI protocol (%r)\n", Status));
    return Status;
  }

  HeciMsg = (MKHI_MSG_NM_HOST_CFG_BUFFER*) AllocateZeroPool (sizeof (MKHI_MSG_NM_HOST_CFG_BUFFER));
  if (HeciMsg == NULL) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Out of resources  (NMHostCfgReq)\n"));
    ASSERT (HeciMsg != NULL);
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem (HeciMsg, sizeof (HeciMsg));
  HeciMsg->NMHostCfgReq.Mkhi.Fields.GroupId = MKHI_GRP_NM;
  HeciMsg->NMHostCfgReq.Mkhi.Fields.Command = NM_CMD_HOSTCFG;
  HeciMsg->NMHostCfgReq.Capabilities.Bits.TurboEn = SpsGetTurboBit ();

#if ME_TESTMENU_FLAG
  if (pSpsContext->pSpsPolicy->NmPowerMsmtOverride) {
    HeciMsg->NMHostCfgReq.Capabilities.Bits.PowerMsmt = (UINT16) pSpsContext->pSpsPolicy->Common.NmPowerMsmtSupport;
  } else {
#endif // ME_TESTMENU_FLAG
    HeciMsg->NMHostCfgReq.Capabilities.Bits.PowerMsmt = (UINT16) pSpsContext->pSpsHob->NmPowerMsmtSupport;
#if ME_TESTMENU_FLAG
  }

  if (pSpsContext->pSpsPolicy->NmHwChangeOverride) {
    HeciMsg->NMHostCfgReq.Capabilities.Bits.HwChange = (UINT16) pSpsContext->pSpsPolicy->Common.NmHwChangeStatus;
  } else {
#endif // ME_TESTMENU_FLAG
    HeciMsg->NMHostCfgReq.Capabilities.Bits.HwChange = (UINT16) pSpsContext->pSpsHob->NmHwChangeStatus;
#if ME_TESTMENU_FLAG
  }
#endif // ME_TESTMENU_FLAG

  HeciMsg->NMHostCfgReq.Capabilities.Bits.MsgVer = MKHI_MSG_NM_HOST_CFG_VER;
  //
  // ME wants PLATFORM_INFO MSRs sent raw.
  //
  HeciMsg->NMHostCfgReq.PlatformInfo  = AsmReadMsr64 (SPR_MSR_PLATFORM_INFO);
  HeciMsg->NMHostCfgReq.Altitude = (INT16) pSpsContext->pSpsPolicy->SpsAltitude;

  Status = SpsGetCoresAndTreads (
             &(HeciMsg->NMHostCfgReq.ProcCoresEnabled),
             &(HeciMsg->NMHostCfgReq.ProcThreadsEnabled)
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = SpsFillStatesTable (
             &(HeciMsg->NMHostCfgReq.PStatesNumber),
             HeciMsg->NMHostCfgReq.NPSSPStatesRatio,
             &(HeciMsg->NMHostCfgReq.GPSSPstatesNumber),
             HeciMsg->NMHostCfgReq.GPSSPStatesRatio,
             (UINT8) HeciMsg->NMHostCfgReq.Capabilities.Bits.TurboEn
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  HeciMsg->NMHostCfgReq.TStatesNumber = SpsGetTStatesNumber (0);

  //
  // Hardware Power Management may be configured in one of 4 modes:
  // (0) Disabled    - legacy ACPI P-states/T-states interface used
  // (1) Native      - new _CPC and legacy objects are exposed to OS, OS decides what will be used
  // (2) Out of Band - HWPM enforced, no PM interface exposed to OS in ACPI
  // (3) Native w/o Legacy - new _CPC object exposed in ACPI without legacy objects
  //
  if (SpsIsLegacyOob (&(HeciMsg->NMHostCfgReq.ProcNumber))) {
    HeciMsg->NMHostCfgReq.PStatesNumber = 0;
    HeciMsg->NMHostCfgReq.TStatesNumber = 0;
  }
  //
  // Number of cores implemented can be found in CPU ID function 11 - core topology.
  // Core topology sub-function 1 gives the number of possible logical threads, sub-function 0
  // the number of threads per core. To calculate cores divide threads by threads per core.
  //
  AsmCpuidEx (CPUID_EXTENDED_TOPOLOGY, 1, NULL, &Reg, NULL, NULL);
  HeciMsg->NMHostCfgReq.ProcCoresNumber = (UINT16) Reg;
  AsmCpuidEx (CPUID_EXTENDED_TOPOLOGY, 0, NULL, &Reg, NULL, NULL);
  HeciMsg->NMHostCfgReq.ProcCoresNumber /= (UINT16) Reg;
  HeciMsg->NMHostCfgReq.ProcCoresNumber *= HeciMsg->NMHostCfgReq.ProcNumber;

  DEBUG ((DEBUG_INFO, "[SPS] Sending Host Configuration Info to ME\n"));
  DEBUG ((DEBUG_INFO, "[SPS]   Capabilities   : 0x%04x\n", HeciMsg->NMHostCfgReq.Capabilities.Uint16));
  DEBUG ((DEBUG_INFO, "[SPS]        TurboEn   : %d\n", HeciMsg->NMHostCfgReq.Capabilities.Bits.TurboEn));
  DEBUG ((DEBUG_INFO, "[SPS]        SmiOptim  : %d\n", HeciMsg->NMHostCfgReq.Capabilities.Bits.SmiOptim));
  DEBUG ((DEBUG_INFO, "[SPS]        PowerMsmt : %d\n", HeciMsg->NMHostCfgReq.Capabilities.Bits.PowerMsmt));
  DEBUG ((DEBUG_INFO, "[SPS]        HwChange  : %d\n", HeciMsg->NMHostCfgReq.Capabilities.Bits.HwChange));
  DEBUG ((DEBUG_INFO, "[SPS]        MsgVer    : %d\n", HeciMsg->NMHostCfgReq.Capabilities.Bits.MsgVer));
  DEBUG ((DEBUG_INFO, "[SPS]   P/T-states:    %d/%d\n",
                     HeciMsg->NMHostCfgReq.PStatesNumber, HeciMsg->NMHostCfgReq.TStatesNumber));
  DEBUG ((DEBUG_INFO, "[SPS]   Min/max power: %d/%d\n",
                     HeciMsg->NMHostCfgReq.MinPower, HeciMsg->NMHostCfgReq.MaxPower));
  DEBUG ((DEBUG_INFO, "[SPS]   Processor packages: %d\n", HeciMsg->NMHostCfgReq.ProcNumber));
  DEBUG ((DEBUG_INFO, "[SPS]   Processor cores:    %d\n", HeciMsg->NMHostCfgReq.ProcCoresNumber));
  DEBUG ((DEBUG_INFO, "[SPS]   Processor cores enabled:   %d\n", HeciMsg->NMHostCfgReq.ProcCoresEnabled));
  DEBUG ((DEBUG_INFO, "[SPS]   processor threads enabled: %d\n", HeciMsg->NMHostCfgReq.ProcThreadsEnabled));
  DEBUG ((DEBUG_INFO, "[SPS]   Platform info:     0x%08X%08X\n",
                     (UINT32) (HeciMsg->NMHostCfgReq.PlatformInfo >> 32), (UINT32) HeciMsg->NMHostCfgReq.PlatformInfo));
  DEBUG ((DEBUG_INFO, "[SPS]   Altitude: %d\n", HeciMsg->NMHostCfgReq.Altitude));
  DEBUG ((DEBUG_INFO, "[SPS]   GPSSPStatesRatio: %d\n", HeciMsg->NMHostCfgReq.GPSSPstatesNumber));

  if ((HeciMsg->NMHostCfgReq.PStatesNumber > 0)) {
    DEBUG ((DEBUG_INFO, "[SPS]   NPSSPStatesRatio[%d]: ", HeciMsg->NMHostCfgReq.PStatesNumber));
    for (Index = 0; Index < HeciMsg->NMHostCfgReq.PStatesNumber; Index++) {
      DEBUG ((DEBUG_INFO, " %02X", HeciMsg->NMHostCfgReq.NPSSPStatesRatio[Index]));
    }
    DEBUG ((DEBUG_INFO, "\n"));
  }

  if ((HeciMsg->NMHostCfgReq.GPSSPstatesNumber > 0)) {
    DEBUG ((DEBUG_INFO, "[SPS]   GPSSPStatesRatio[%d]: ", HeciMsg->NMHostCfgReq.GPSSPstatesNumber));
    for (Index = 0; Index < HeciMsg->NMHostCfgReq.GPSSPstatesNumber; Index++) {
      DEBUG ((DEBUG_INFO, " %02X", HeciMsg->NMHostCfgReq.GPSSPStatesRatio[Index]));
    }
    DEBUG ((DEBUG_INFO, "\n"));
  }

  HeciMsgLen = sizeof (MKHI_MSG_NM_HOST_CFG_REQ);
  RspLen = sizeof (MKHI_MSG_NM_HOST_CFG_RSP);
  Status = Heci->HeciSendAndReceive (
                   Heci,
                   HECI_DEFAULT_PCH_ID,
                   NULL,
                   HECI_DEFAULT_RETRY_NUMBER,
                   SPS_CLIENTID_BIOS,
                   SPS_CLIENTID_ME_MKHI,
                   (UINT32*) &HeciMsg->NMHostCfgReq,
                   HeciMsgLen,
                   (UINT32*) &HeciMsg->NMHostCfgRsp,
                   &RspLen,
                   NULL,
                   NULL
                   );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot send Host configuration info to NM (%r)\n", Status));
  } else if (HeciMsg->NMHostCfgRsp.Mkhi.Fields.IsResponse != 1 ||
            HeciMsg->NMHostCfgRsp.Mkhi.Fields.GroupId != MKHI_GRP_NM ||
            HeciMsg->NMHostCfgRsp.Mkhi.Fields.Command != NM_CMD_HOSTCFG) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Wrong response format for Host configuration info to NM (MKHI: 0x%x)\n",
           HeciMsg->NMHostCfgRsp.Mkhi.Data));
  } else {
    switch (HeciMsg->NMHostCfgRsp.Status) {
      case STATUS_NM_HOST_CFG_RSP_OK:
        DEBUG ((DEBUG_INFO, "[SPS] Configuration info to NM was sent.\n"));
        break;
      case STATUS_NM_HOST_CFG_RSP_WRONG_MSG_FORMAT:
        DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Host configuration info response has wrong format\n"));
        break;
      case STATUS_NM_HOST_CFG_RSP_ALTITUDE_MISSING:
        DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Altitude was missing in Host configuration info\n"));
        break;
      case STATUS_NM_HOST_CFG_RSP_PSTATESRATIO_MISSING:
        DEBUG ((DEBUG_ERROR, "[SPS] ERROR: PStatesRatio was missing in Host configuration info\n"));
        break;
      default:
        DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Unknown Host configuration info status (%d)\n", HeciMsg->NMHostCfgRsp.Status));
        break;
    }
  }

  FreePool (HeciMsg);

  return Status;
} // SpsNmSendHostCfgInfo ()
