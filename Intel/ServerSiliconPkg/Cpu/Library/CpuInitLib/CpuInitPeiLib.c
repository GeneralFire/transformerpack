/** @file
  Cpu Initialization Pei Phase Module.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2020 Intel Corporation. <BR>

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

#include "CpuInit.h"
#include <Library/PcuMailBoxLib.h>
#include <Library/VcuMailBoxLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/TimerLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/PmcSetStrapMessageLib.h>
#include <Library/CpuIpLib.h>
#include <Register/Cpuid.h>


/**

  Executed by SBSP only. Check WFR for all socket CPUs

  BIOS SETUP option:
     0 = Disabled;  1 = Enabled;  2 = AUTO
**/
VOID
WFRSocketWA (
  )
{
  UINT8                               socket;
  UINT8                               Stack;
  UINT32                              Status = 0;
  MSR_UNCORE_RATIO_LIMIT_REGISTER     UncoreRatioLimitMsr;
  ICX_MSR_UNCORE_PERF_STATUS_REGISTER UncorePerfStatusMsr;
  UINT8                               MaxClmRatio[MAX_SOCKET];
  UINT8                               MeshMaxRatio[MAX_SOCKET];
  EFI_STATUS                          EfiStatus = EFI_SUCCESS;
  UINT32                              CoreRatio = 0;
  UINT32                              MeshRatio = 0;

  if (GetCpuPreMemPolicy()->WFRWAEnable == 0) {
    goto WFRWAExit;
  }

  RcDebugPrint (SDBG_DEFAULT, "\n:: Start WFRSocketWA()...\n");

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (SocketPresent (socket)) {
      for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
        if (CheckStackPersonality (socket, Stack, TYPE_MCP)) {
          Status = 1;
        }
      }  //Stack for loop
    }
  }  //socket for loop

  if (!Status && (GetCpuPreMemPolicy()->WFRWAEnable == 2)) {
    goto WFRWAExit;
  }

  Status = 0;

  // Init entire array to zero
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    MaxClmRatio[socket] = 0;
    MeshMaxRatio[socket] = 0;
  }

  // Check WFR socket, and prepare MaxClmRatio and MeshMaxRatio
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (!SocketPresent (socket)) {
      continue;
    }

    if (SocketPresent (socket)) {
      // populate elements of valid socket with actual values
      RcDebugPrint (SDBG_DEFAULT, "\nDo WFR WA in current S%2d: \n", socket);

      UncoreRatioLimitMsr.Uint64 = ReadMsrPipeHL64 (socket, MSR_UNCORE_RATIO_LIMIT);
      MaxClmRatio[socket] = (UINT8) UncoreRatioLimitMsr.Bits.MaxClrRatio;


      //
      // Set Mesh Ratio to max CLM ratio to read Mesh P1 ratio
      //

      CoreRatio = UncoreRatioLimitMsr.Bits.MaxClrRatio;
      MeshRatio = UncoreRatioLimitMsr.Bits.MaxClrRatio;
      EfiStatus = PcuSetCoreMeshRatio (socket, (UINT8)CoreRatio, (UINT8)MeshRatio);
      if (EFI_ERROR (EfiStatus)) {
        RcDebugPrint (SDBG_DEFAULT, "\nBIOS failed to request pcode to set UNCORE_RATIO_LIMIT in current S%2d: \n", socket);
        Status = 1;
      } else {
        UncorePerfStatusMsr.Uint64 = ReadMsrPipeHL64 (socket, ICX_MSR_UNCORE_PERF_STATUS);
        MeshMaxRatio[socket] = (UINT8) UncorePerfStatusMsr.Bits.CurrentClrRatio;
      }
    }
  }  // for loop

  if (Status) {
    goto WFRWAExit;
  }

  // set UNCORE_RATIO_LIMIT min = max = clm_p1
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (SocketPresent (socket)) {
      UncoreRatioLimitMsr.Uint64 = ReadMsrPipeHL64 (socket, MSR_UNCORE_RATIO_LIMIT);
      UncoreRatioLimitMsr.Bits.MaxClrRatio = MaxClmRatio[socket];
      UncoreRatioLimitMsr.Bits.MinClrRatio = MaxClmRatio[socket];
      WriteMsrPipeHL64 (socket, MSR_UNCORE_RATIO_LIMIT, UncoreRatioLimitMsr.Uint64);
    }
  } // for loop

  // Program P1 for core and mesh using B2P
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (SocketPresent (socket)) {
      CoreRatio = CORE_RATIO_P1;
      MeshRatio = MeshMaxRatio[socket];
      EfiStatus = PcuSetCoreMeshRatio (socket, (UINT8)CoreRatio, (UINT8)MeshRatio);
      if (EFI_ERROR (EfiStatus)) {
        RcDebugPrint (SDBG_DEFAULT, "\nBIOS failed to request pcode to set Core and Mesh ratio in current S%2d: \n", socket);
        Status = 1;
      }
    }
  } // for loop

  if (Status) {
    goto WFRWAExit;
  }

  DeCoupleCLMFrequency();

  RcDebugPrint (SDBG_DEFAULT, "\nBIOS completed WFR WA.\n");

WFRWAExit:
  return;
}

/**

  Executed by SBSP only. Collect package BSP data

  @retval VOID

**/
VOID
CollectPbspData (
  )
{
  UINT8   socket;
  UINT32  csrReg;

  //
  // Read APIC ID  and stepping from scratch pad register of each PBSP
  //
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (SocketPresent(socket)) {
      csrReg = UsraCsrRead (socket, 0, GetSrPbspCheckinCsr());

      //
      // CSR bit[23:8] = APIC ID at reset
      //
      GetCpuVarData()->PackageBspApicID[socket] = (csrReg >> 8) & 0xffff;

      //
      // Clear the APIC ID and Stepping data in scratch pad CSR
      //
      csrReg &= 0x870000ff;
      UsraCsrWrite (socket, 0, GetSrPbspCheckinCsr(), csrReg);
    }
  }
}

/**

  Check if VT/TXT is supported, and if a power-good reset is needed
  to unlock the MSR to allow changing the current setting.

  @retval SetSysResetRequired() updated (if reset is needed)

**/
VOID
CheckVtIfNeedReset (
  )
{
  CPUID_VERSION_INFO_ECX             RegEcx;
  MSR_IA32_FEATURE_CONTROL_REGISTER  MsrIa32FeatureControl;
  BOOLEAN                            CurrentVmxState;

  //
  // Note all logical threads' VT/TXT feature MSRs are configured identically on cold boot path.
  // Here we check if the current config needs to be changed (i.e.,mismatch with BIOS Setup),
  // and if a power-good reset is needed to "unlock" the MSR for that change.
  //

  //
  // Check if VT is supported via CPUID Function 1, which returns
  //    ECX[6] Intel TXT support feature flag
  //    ECX[5] Intel VT  support feature flag
  //

  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL,  &RegEcx.Uint32, NULL);

  if (RegEcx.Bits.VMX == 0) {
    return;
  }

  MsrIa32FeatureControl.Uint64 = AsmReadMsr64 (MSR_IA32_FEATURE_CONTROL);

  //
  // MSR_IA32_FEATURE_CONTROL Definition:
  //    Bit 2   Enable VMX outside SMX operation (R/WL): This bit enables Intelr
  //            Virtualization Technology in an environment that may not include
  //            Intelr Trusted Execution Technology support.
  //
  //    Bit 1   Enable VMX inside SMX operation (R/WL): This bit enables Intelr
  //            Virtualization Technology in an environment that includes Intelr
  //            Trusted Execution Technology support.
  //
  //    Bit 0   Lock (R/WO) (1 = locked) When set, locks this MSR from being witten,
  //            writes to this bit will result in GP(0) until an S5 reset occurs.
  //            BIOS must ensure this lock bit is set before boot to OS.
  //


  //
  // If unlocked, no reset needed.
  //
  if (MsrIa32FeatureControl.Bits.Lock == 0) {
    return;
  }

  CurrentVmxState = FALSE;
  if (MsrIa32FeatureControl.Bits.EnableVmxOutsideSmx) {
    CurrentVmxState = TRUE;
  }

  //
  // Need a reset only if this MSR is locked and VMX state has to be changed.
  //

  if (CurrentVmxState != (BOOLEAN)GetCpuPreMemPolicy()->vtEnable) {
    //
    // We need a power good reset to unlock MSR_IA32_FEATURE_CONTROL.
    //
    SetSysResetRequired(GetSysResetRequired() | POST_RESET_POWERGOOD);
  }
}

/**

  Executed by SBSP only.
  Restore FLEX_RATIO, DESIRED_CORES CSRS for all CPUs on S3 resume path.

  @retval FLEX_RATIO, DESIRED_CORES CSRs restored from nvram.cpu structure
  @retval SetSysResetRequired() updated (if reset is needed)

**/
VOID
RestoreProcessorCsrsOnS3Resume (
  )
{
  UINT8       socket;
  UINT8       resetNeeded = 0;
  UINT32      flexRatioN0PcuFun3;
  CPU_NVRAM   *NvRamData;

  //
  //  Intel PPV requirement:  Do not touch power-on default ratio of all CPU sockets if selected in BIOS Setup.
  //  This allows each CPU socket to run at different default ratio/freq as-is.
  //  Desired_CORES CSR is also left alone, since this PPV use case does not involve S3 resume
  //  CAUTION:  This usage is out-of-spec, so use at your own risk.
  //
  if (GetCpuPreMemPolicy()->AllowMixedPowerOnCpuRatio) {
    return;
  }

  NvRamData = GetCpuNvramData();
  if (NvRamData == NULL) {
    RcDebugPrint (SDBG_DEFAULT, "NvramData Returned NULL pointer\n");
    RC_FATAL_ERROR ((NvRamData != NULL), ERR_NO_MEMORY, RC_FATAL_ERROR_MINOR_CODE_0);
    return;
  }

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (SocketPresent(socket)) {
      //
      // restore FLEX_RATIO CSR
      //
      flexRatioN0PcuFun3 = UsraCsrRead (socket, 0, FLEX_RATIO_N0_PCU_FUN3_REG);
      if (flexRatioN0PcuFun3 != NvRamData->flexRatioCsr) {
        UsraCsrWrite (socket, 0, FLEX_RATIO_N0_PCU_FUN3_REG, NvRamData->flexRatioCsr);
        resetNeeded |= POST_RESET_WARM;
        //
        // Update PMC set_strap message data register witht the new flex ratio setting
        //
        if (PcdGetBool (PcdPmcStrapSetVdmSupported)) {
          UpdatePmcSetStrapMsgFlexRatio ((UINT8) (NvRamData->flexRatioCsr >> 8), NULL);
        }
      }
      //
      // restore DESIRED_CORES CSR
      //
      resetNeeded |= RestoreDesiredCores(socket);
    }
  }
  if (resetNeeded) {
    SetSysResetRequired(GetSysResetRequired() | resetNeeded);
  }
}

/**
Initialize CPU related uncore CSRs for all sockets

  @retval VOID

**/
VOID
CpuUncoreInit (
  )
{
  UINT8                              socket;
  UINT8                              SbspSocketIdNv;
  MSR_UNCORE_RATIO_LIMIT_REGISTER    MsrUncoreRatioLimit;

  SbspSocketIdNv = GetSysSbspSocketIdNv ();
  RcDebugPrint (SDBG_DEFAULT, "CpuUncoreInit()\n");
  //
  // Loop thru all CPU sockets
  //
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (!SocketPresent (socket)) {
      continue;
    }

    MsrUncoreRatioLimit.Uint64 = ReadMsrPipeHL64 (socket, MSR_UNCORE_RATIO_LIMIT);
    MsrUncoreRatioLimit.Uint32 = CpuUncoreRatioInit (socket, MsrUncoreRatioLimit.Uint32);
    WriteMsrPipeHL64 (socket, MSR_UNCORE_RATIO_LIMIT, MsrUncoreRatioLimit.Uint64);

    RcDebugPrint (SDBG_DEFAULT, " Write Socket %2d MSR_UNCORE_RATIO_LIMIT.MinClrRatio [14:8] = 0x%x, MSR_UNCORE_RATIO_LIMIT.MaxClrRatio [7:0] = 0x%x\n",
      socket, MsrUncoreRatioLimit.Bits.MinClrRatio, MsrUncoreRatioLimit.Bits.MaxClrRatio);

    //
    // Program  other uncore CSRs here if needed...
    //
  }
}

/**
  Initialize SSE support.
**/
VOID
InitXMM (
  VOID
  )
{
  UINT32  RegEdx;

  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, NULL, &RegEdx);

  //
  // Check whether SSE2 is supported
  //
  if ((RegEdx & BIT26) != 0) {
    AsmWriteCr0 (AsmReadCr0 () | BIT1);
    AsmWriteCr4 (AsmReadCr4 () | BIT9 | BIT10);
  }
}

/**

  Executed by SBSP only. Configure CPU features that require a reset to take effect

  @retval VOID
  @retval Related CPU CSRs are programmed as needed.
  @retval SetSysResetRequired() is updated as needed

**/
VOID
EFIAPI
CpuInit (
  )
{
  EFI_STATUS Status;
  RcDebugPrint (SDBG_DEFAULT, "\n\nCpuInit Start\n\n");

  InitXMM ();

  //
  // Initialize CPU related uncore CSRs
  //
  CpuUncoreInit ();

  //
  // read package BSP data
  // (can only be done after CPU topology has been discovered by KTI RC )
  //
  CollectPbspData ();

  //
  // Set early C1e before CPL2 as ACP request
  //
  SetEarlyC1e ();

  //
  // WFR buffering issue WA
  // HSD 5330798: WFR WA should be disabled on A0
  //
  if (IsSiliconWorkaroundEnabled ("S5330798")) {
    WFRSocketWA ();
  }

  // Init the Cpu Nvram Data;
  Status = InitCpuNvram();
  if (Status != EFI_SUCCESS) {
    RcDebugPrint (SDBG_DEFAULT, "InitCpuNvram call has failed\n");
    RC_FATAL_ERROR ((Status == EFI_SUCCESS), ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_1);
    return;
  }

  if (GetBootModeHob() == BOOT_ON_S3_RESUME) {    //S3 resume path
    RestoreProcessorCsrsOnS3Resume();
  } else {                            // normal boot path

    CheckVtIfNeedReset();
    if (GetSysResetRequired() & POST_RESET_POWERGOOD) {
      return;
    }

    ProgramProcessorFlexRatio();
    SetActiveCoresAndSmtEnableDisable();
    CpuPhysicalAddressLimit();
  }

  Config_IOT_LLC ();

  //
  // CPU MISC configuration
  //
  CpuInitMISC ();
  EnableTscReset ();
}

/**

  Executed by SBSP only. Configure CPU registers that are required to be configured for PCODE usage prior to setting BIOS_INIT_Done.

  @retval VOID
  @retval Related CPU CSRs are programmed as needed.
  @retval SetSysResetRequired() is updated as needed

**/
VOID
CpuInitMISC (
  )
{
  UINT8   socket;
  UINT8   sbspSktId = 0;
  EFI_STATUS Status = EFI_SUCCESS;
  sbspSktId = GetSysSbspSocketIdNv ();

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (SocketPresent(socket)) {

      RcDebugPrint (SDBG_DEFAULT, "CPUMISC Current S%2d: \n", socket);
      //
      // Program DMICTRL...We only want to do this for SBSP... clear bits 0:1 of DMICTRL on Legacy socket only
      //
      if (socket == sbspSktId) {
        SetAbortInboundRequests (socket, 0);
      } // if socket 0

      if (!(GetEmulation() & SIMICS_FLAG)) {
        if (GetCpuPreMemPolicy()->PmaxDisable) {
          Status = PcuSetPmaxDisable (socket);
        }
      }
    } // end check if socket present
  } // end for{} loop
}


/**
  Init the CPU Nvram Data;
  @Result: EFI_SUCCESS the Nvram data init success;
                EFI_OUT_OF_RESOURCES Can't create CPU Nvram Data hob.
**/
EFI_STATUS
InitCpuNvram(
  )
{
  VOID*               CpuNvramGuidHob;

  if (GetFirstGuidHob (&gEfiCpuNvramDataGuid) != NULL) {
    return EFI_SUCCESS;
  }

  RcDebugPrint (SDBG_DEFAULT, "[WARNING]: GetFirstGuidHob is null,build GuidHob... InitCpuNvram CpuInitPeiLib.c\n");
  CpuNvramGuidHob = BuildGuidHob (&gEfiCpuNvramDataGuid, sizeof(CPU_NVRAM));
  if (CpuNvramGuidHob == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem(CpuNvramGuidHob, sizeof(CPU_NVRAM));
  return EFI_SUCCESS;
}


