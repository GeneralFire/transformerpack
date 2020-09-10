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

#include <Library/MemoryCoreLib.h>
#include <SysHostChip.h>
#include <Library/PipeSyncLib.h>
#include <Library/MemCpgcIpLib.h>
#include "Chip10nm/Include/MemHostChip10nm.h"
#include <KtiSetupDefinitions.h>
#include <Library/MemFmcIpLib.h>
#include <Cpu/CpuCoreRegs.h>
#include <Chip/Include/FnvAccessCommon.h>
#include <ScratchpadList.h>
#include <Library/MemDdrioIpLib.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/MemBootHealthLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/CheckpointLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemRcLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/SgxPreMemInit.h>
#include <Library/MemCallTableLib.h>
#include <Library/ProcMemInitTargetLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/SsaBuiltInRmtLib.h>
// APTIOV_SERVER_OVERRIDE_RC_START :  Wrapper for OEM to override TurnAroundTimings
#include <AmiMrcHooksWrapperLib/AmiMrcHooksWrapperLib.h>
// APTIOV_SERVER_OVERRIDE_RC_END :  Wrapper for OEM to override TurnAroundTimings
#include <Library/MemTurnaroundTimingLib.h>
#include <Library/PerformanceTrackerLib.h>
#ifdef CWV_A0_REMOVE_POST_PO
#include <Library/PreSiliconEnvDetectLib.h>
#endif

#ifdef DDR5_SUPPORT
#include <Library/Ddr5CoreLib.h>
#endif

BOOLEAN
EFIAPI
DoesCpuSupportNearMemory (
  VOID
  )
{
  //
  // TODO: Add CPU type support in near future
  //
  return FALSE;
}

// -------------------------------------
// Code section
// -------------------------------------

/**
  Unlock access to the Memory Controller and related sub-systems
  if required by the processor.

  @param[in]      Host          - Pointer to sysHost

  @retVal SUCCESS
**/

UINT32
UnlockMemoryAfterRamp (
  IN PSYSHOST  Host
  )
{
  UINT8                   Socket;
  UINT64_STRUCT           UnlockMsrCmd;

  Socket = Host->var.mem.currentSocket;
  if (Host->nvram.mem.socket[Socket].maxDimmPop != 0) {
    //
    // Add command to unlock memory after memory subsystem is brought up
    //
    if (IsSiliconWorkaroundEnabled ("S1405762598")) {
      UnlockMsrCmd.lo = 0;
      UnlockMsrCmd.hi = 0;
      WriteMsrPipe (Host, Socket, MSR_LT_UNLOCK_MEMORY, UnlockMsrCmd);
    } else {
      SetCpgcLock (Socket, CpgcFullyUnlocked);
    }
  } // socket loop
  return SUCCESS;
}

/**
  FMC CSR unlock sequence for channels in a bitmask

  @param[in] Socket      - Processor socket ID
  @param[in] ChBitMask   - Channel bit mask (socket channel mask)

  @retval 0 - SUCCESS
  @retval 1 - FAILURE on any channel

**/
UINT32
FmcCsrUnlock(
  IN  UINT8    Socket,
  IN  UINT8    ChBitMask
  )
{
  UINT8           Ch;
  UINT32          Status = SUCCESS;

  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if (!IsChannelEnabled(Socket, Ch) || ((ChBitMask & (1 << Ch)) == 0)) {
      continue;
    }

    if (EFI_ERROR(FmcCsrUnlockCh(Socket, Ch))) {
      Status = FAILURE;
    }

  }
  return Status;
}

/**

  This routine gets things ready for DDR training

  @param Host  - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
PreTrainingInit (
  PSYSHOST Host
  )
{
  UINT8                                 ch;
  UINT8                                 socket = Host->var.mem.currentSocket;
  struct channelNvram                   (*channelNvList)[MAX_CH];
  struct dimmNvram                      (*dimmNvList)[MAX_DIMM];
  BootMode                              SysBootMode;
  UINT8                                 MaxChDdr;
#ifdef CWV_A0_REMOVE_POST_PO
  SYS_SETUP                             *Setup;
  BOOLEAN                               SimicsEnv;
  Setup  = GetSysSetupPointer ();
  SimicsEnv = IsSimicsEnvironment ();
#endif
  MaxChDdr = GetMaxChDdr ();
  SysBootMode = GetSysBootMode ();

  //
  // Return if this socket is disabled
  //

  if (IsSocketDimmPopulated (Host, socket) != TRUE) {
    return SUCCESS;
  }

  if (SysBootMode == S3Resume || Host->var.mem.subBootMode == WarmBootFast) {
    return SUCCESS;
  }

  channelNvList = GetChannelNvList(Host, socket);

  SetUbiosOutputMode (ASM_OUTPUT_ENABLE);

  PretrainingDdrioInit (Host, socket);

  InitStartingOffset (Host, socket);

  ClearWdbCacheValidAllChannels (socket);

  CacheAndClearRidEntries (socket);
  //
  // Configure Tx_Ui
  //
  ConfigureTxUi (Host, socket);
#ifdef DDR5_SUPPORT
  Ddr5PreTrainingInit (Host);
  if (IsBrsPresent (Host, socket)) {

    for (ch = 0; ch < MaxChDdr; ch++) {
      CmdCtlOutputEnable (Host, socket, ch, CmdCtlOutputEnableAll, NULL);
    }

    if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    //
    //BRS CTE_WA_START
    //
    (*channelNvList)[1].enabled = 0;//BRS CTE_WA
    //
    //BRS CTE_WA_END
    //
    }
  }
#if FixedPcdGetBool(PcdMcrSupport)
  if (FeaturePcdGet (PcdCteBuild) == FALSE) {
    //
    // This seems to be required only for BSSA. Skipping this for CTE
    //
    ResetAllDdrChannelsPreTraining (Host, socket, CH_BITMASK);
  }
#else
  ResetAllDdrChannelsPreTraining (Host, socket, CH_BITMASK);
#endif // FixedPcdGetBool(PcdMcrSupport)
  FmcCsrUnlock(socket, CH_BITMASK);
  JedecInitPreTraining (Host, socket);
#ifdef CWV_A0_REMOVE_POST_PO
  if ((Setup->mem.dfxMemSetup.dfxOptions & CR_SKIP_MB2_COMMAND) && \
      (Host->nvram.mem.socket[socket].DcpmmPresent == 1)        && \
      (SimicsEnv == FALSE)) {
  SetBreakpointCsr (socket, (UINT32) ((CHECKPOINT_MAJOR_DDRIO_INIT << 8) + CHECKPOINT_MINOR_BEFORE_DDRT_IO_INIT));
  RcDebugPrint (SDBG_MINMAX,
    "MB2 DDRT_FREQ Init: \n                    Breaking for Skip MB2 command\n");
  }
#endif
  OutputCheckpoint (CHECKPOINT_MAJOR_DDRIO_INIT, CHECKPOINT_MINOR_BEFORE_DDRT_IO_INIT, 0);
#ifdef CWV_A0_REMOVE_POST_PO
  if ((Setup->mem.dfxMemSetup.dfxOptions & CR_SKIP_MB2_COMMAND) && \
      (Host->nvram.mem.socket[socket].DcpmmPresent == 1)        && \
      (SimicsEnv == FALSE)) {
    RcDebugPrint (SDBG_MINMAX,"MB2 DDRT_FREQ Init: MB2 command Skipped\n");
  } else {
    RcDebugPrint (SDBG_MINMAX,"MB2 DDRT_FREQ Init: Start run MB2 command...\n");
#endif
    FnvDdrtIoInit (Host, socket, DDRT_FREQ);
#ifdef CWV_A0_REMOVE_POST_PO
  }
#endif
  Host->var.mem.socket[socket].ioInitdone = 1;

#if defined(CWV_A0_REFACTOR_POST_PO)
  //
  // DDRT 2.0: Move host senseamp training after DDRT IO Init
  //
  SenseAmpOffset (Host);
#endif

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList) [ch].enabled == 0) {
      continue;
    }
    ProgramCasLDdr5 (Host, socket, ch);
    if (UbiosGenerationOrHsleEnabled() == TRUE) {
      ProgramReqRxPathUngate(Host, socket, ch);
    }
  }
  CacheTxVrefCwvTarget(Host, socket, SysBootMode);

  OutputCheckpoint (CHECKPOINT_MAJOR_DDRIO_INIT, CHECKPOINT_MINOR_DDRT_IO_INIT, 0);
  if (Host->var.mem.subBootMode == ColdBoot){
    SetAepTrainingMode (Host,socket, ENABLE_TRAINING_MODE);
  }

  InitDdrioInterfaceLateFmc (Host, socket);

#endif //DDR5_SUPPORT

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    CpgcInorderModeEnable (socket, ch, TRUE);
    ConfigureUniseqModeBeforeTraining (socket, ch);
    if (FeaturePcdGet (PcdCosimBuild) == TRUE) {
      ProgramtCLtCWLAdd (Host, socket, ch);
    } else {
      if (UbiosGenerationOrHsleEnabled () == FALSE) {
        ProgramtCLtCWLAdd (Host, socket, ch);
      }
    }

    ProgramDdrtGnt2Erid(Host, socket, ch);
    DisableScrambling(Host, socket, ch);
    UpdateFnvDdrioCache10nm(Host, socket, ch);
    InvalidateDdrtPrefetchCache(Host, socket, ch);
  } // ch loop

  if (SysBootMode == NormalBoot && (Host->var.mem.subBootMode == ColdBoot || Host->var.mem.subBootMode == ColdBootFast)) {
    //
    // 5387194: CLONE from skylake_server: Prefetch cache misses during CPGC tests
    // Disable C2 of NVMDIMM
    //
    ConfigNvmDimmC2 (Host, socket, TRUE);
  }

  if (Host->nvram.mem.socket[socket].DcpmmPresent == 1) {
    //
    // Set gnt_32_cont_en = 1 once for all of the DDRT training
    //
    Grant32ContEn (Host, socket, 1);
  }

  if (SysBootMode == NormalBoot && Host->var.mem.subBootMode == ColdBootFast) {
    ToggleBcomFlopEn(Host, socket, 0);
    //
    // Turn on Core BCOM for channel with LRDIMM/NVMDimm to mitigate large cycle delays between the 2 dimms
    //
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      //
      // If BWV present, leave in IO BCOM mode
      //
      if (GetFmcType (socket, ch) == FMC_BWV_TYPE) {
        continue;
      }

      dimmNvList = GetDimmNvList (Host, socket, ch);
      if ((Host->nvram.mem.socket[socket].ddrFreq >= DDR_2666) && ((*dimmNvList)[0].lrDimmPresent == 1) && ((*dimmNvList)[1].DcpmmPresent == 1)) {
        ToggleBcomFlopEnPerCh (Host, socket, ch, 1);
      }
    }
  }

  return SUCCESS;
} // PreTrainingInit

/**

  Performs post-training hardware initialization

  @param[in] Host   Pointer to sysHost

  @retval SUCCESS   All operations completed successfully

**/
UINT32
PostTrainingInit (
  IN  PSYSHOST  Host
  )
{
  UINT8                 Ch;
  UINT8                 McId;
  UINT32                ChDisableMask;
  struct channelNvram   (*ChannelNvList)[MAX_CH];
  CPU_CSR_ACCESS_VAR    *CpuCsrAccessVar;
  UINT8                 Socket = Host->var.mem.currentSocket;
  SYS_SETUP             *Setup;
  EFI_STATUS            Status;
  UINT8                 MaxChDdr;
  UINT8                 NumChPerMc;
  UINT8                 MaxImc;

  MaxChDdr   = GetMaxChDdr ();
  NumChPerMc = GetNumChannelPerMc ();
  Setup      = GetSysSetupPointer ();
  MaxImc     = GetMaxImc ();

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();


  if (!IsMemFlowEnabled (PostTrainingInitialization)) {
    return SUCCESS;
  }


  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // Return if this socket is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0)) {
    return SUCCESS;
  }

  IO_Reset (Host, Socket);
  ConfigureRxPathUngate (Socket, RX_PATH_UNGATE_ENABLE);
  //
  // Disable ALL Channels and return if no DIMMs
  //
  if (Host->nvram.mem.socket[Socket].maxDimmPop == 0) {

    for (McId = 0; McId < MaxImc; McId++) {
      if (McId >= CpuCsrAccessVar->DdrMaxImc) {
        continue;
      }

      //
      // Disable all channels since there are no populated DIMMs
      //
      ChDisableMask = ((1 << NumChPerMc) - 1);
      for (Ch = 0; Ch < MaxChDdr; Ch++) {
        if (McId != GetMCID (Host, Socket, Ch)) {
          continue;
        }
      } // Ch loop

      Status = DisableChannelsDdrio (Socket, McId, ChDisableMask);
      if (EFI_ERROR (Status)) {
        RcDebugPrint (SDBG_ERROR,
          "Error disabling channels in Socket %d MC %d. Channel Disable Mask: 0x%x\n Return Status: %r\n",
          Socket, McId, ChDisableMask, Status);
      }

      Status = DisableChannelsMc (Socket, McId, ChDisableMask);
      if (EFI_ERROR (Status)) {
        RcDebugPrint (SDBG_ERROR,
          "Error disabling channels in Socket %d MC %d. Channel Disable Mask: 0x%x\n Return Status: %r\n",
          Socket, McId, ChDisableMask, Status);
      }
    } // McId loop
    return SUCCESS;
  }

  //
  // Update dimmmtr.rank_disable setting on mapout results after training
  //
  CheckRankPopLocal (Host, Socket);

  if (Setup->mem.optionsExt & TURNAROUND_OPT_EN) {
    //
    // Calculate and program optimal TA times based on formulas and training results
    //
    OptimizeTurnaroundTimings (Socket);

    //
    // Override TA times for select environments
    //
    OverrideTurnaroundTimes (Socket);
// APTIOV_SERVER_OVERRIDE_RC_START : Wrapper for OEM to override TurnAroundTimings
    AmiTurnAroundTimingsWrapper (Host, Socket);
// APTIOV_SERVER_OVERRIDE_RC_END : Wrapper for OEM to override TurnAroundTimings
    //
    // Provide a summary of final TA values and indicate if any values deviate from
    // those defined by performance architecture
    //
    ComparePerformanceTurnaroundTimes (Socket);
  }

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    if (Host->var.mem.channelEn <= 2) {
      if ((*ChannelNvList)[Host->var.mem.channelEn].enabled) {
        for (McId = 0; McId < MaxImc; McId++) {
          if (McId >= CpuCsrAccessVar->DdrMaxImc) {
            continue;
          }
          ChDisableMask = ((1 << NumChPerMc) - 1);
          for (Ch = 0; Ch < MaxChDdr; Ch++) {
            if (Ch == Host->var.mem.channelEn) {
              //
              // Remove the channel from the disabled list
              //
              ChDisableMask &= ~(BIT0 << Ch);
              continue;
            }

            if ((*ChannelNvList)[Ch].enabled) {
              DisableChannelSw (Host, Socket, Ch);
            }
          } //Ch

          Status = DisableChannelsDdrio (Socket, McId, ChDisableMask);
          if (EFI_ERROR (Status)) {
            RcDebugPrint (SDBG_ERROR,
            "Error disabling channels in Socket %d MC %d. Channel Disable Mask: 0x%x\n Return Status: %r\n",
            Socket, McId, ChDisableMask, Status);
          }

          Status = DisableChannelsMc (Socket, McId, ChDisableMask);
          if (EFI_ERROR (Status)) {
            RcDebugPrint (SDBG_ERROR,
              "Error disabling channels in Socket %d MC %d. Channel Disable Mask: 0x%x\n Return Status: %r\n",
              Socket, McId, ChDisableMask, Status);
          }
        } //McId
      }
    }
  } else {

    for (McId = 0; McId < MaxImc; McId++) {
      if (!IsMcValid (Host, McId)) {
        continue;
      }
      ChDisableMask = ((1 << NumChPerMc) - 1);
      for (Ch = 0; Ch < NumChPerMc; Ch++) {
        if ((*ChannelNvList)[McId * NumChPerMc + Ch].enabled) {
          //
          // Remove the channel from the disabled list
          //
          ChDisableMask &= ~(BIT0 << Ch);
          continue;
        }

        RcDebugPrint (SDBG_MAX, "Socket %d Channel %d Unused or disabled in software. Disabling in hardware.\n",
          Socket, (McId * NumChPerMc + Ch));

      } // Ch loop

      Status = DisableChannelsDdrio (Socket, McId, ChDisableMask);
      if (EFI_ERROR (Status)) {
        RcDebugPrint (SDBG_ERROR,
          "Error disabling channels in Socket %d MC %d. Channel Disable Mask: 0x%x\n Return Status: %r\n",
          Socket, McId, ChDisableMask, Status);
      }

      Status = DisableChannelsMc (Socket, McId, ChDisableMask);
      if (EFI_ERROR (Status)) {
        RcDebugPrint (SDBG_ERROR,
          "Error disabling channels in Socket %d MC %d. Channel Disable Mask: 0x%x\n Return Status: %r\n",
          Socket, McId, ChDisableMask, Status);
      }
    } // McId loop
  }

  //
  // Call Ddrio hook
  //
  PostTrainingInitDdrio (Host);

  //
  // Call Mc hook
  //
  PostTrainingInitMc (MemTechDdr, Socket);


  return SUCCESS;
} // PostTrainingInit

/**
  Disables channels in HW that are supported in IP (MC, DDRIO, etc) but are unsupported
  by the platform

  @param[in]  Socket        Socket number to disable unused channels for

  @retval     EFI_SUCCESS   Unsupported channels successfully disabled
**/
EFI_STATUS
EFIAPI
DisableUnsupportedChannelsHw (
  IN  UINT8   Socket
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  EFI_STATUS  DisableDdrioChStatus;
  EFI_STATUS  DisableMcChStatus;

  //
  // We don't want to disable any channels in HVM Mode
  //
  if (IsHvmModeEnable () || IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    return EFI_SUCCESS;
  }

  DisableMcChStatus = DisableUnsupportedChannelsMc (Socket);
  DisableDdrioChStatus = DisableUnsupportedChannelsDdrio (Socket);

  if (EFI_ERROR (DisableDdrioChStatus) || EFI_ERROR (DisableMcChStatus)) {
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
} // DisableUnsupportedHwChannels

/*++

  Initialize the SSA API

  @param Host  - Pointer to sysHost

  @retval SUCCESS

--*/
UINT32
ExecuteSsaRmt (
    PSYSHOST Host
  )
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  RunBuiltInBssaRmt ();

  RcDebugPrint (SDBG_MINMAX,
         "ExecuteSsaRmt: Trigger Target Loader with default checkpoint used in EFI Shell Loader tool\n");
  OutputCheckpoint (CHECKPOINT_MAJOR_SSA_API_INIT, CHECKPOINT_MINOR_SSA_API_INIT_EV_LOADER, 0);

  return SUCCESS;
} //ExecuteSsaRmt

/*++

  Wrapper to call Memory margin test

  @param Host  - Pointer to sysHost

  @retval SUCCESS

--*/
UINT32
MemBootHealthTestWrapper (
  PSYSHOST Host
  )
{
  MemBootHealthTest ();
  return EFI_SUCCESS;
}

/**
  This function implements a MRC reset loop using scratchpad registers

  @param Host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
DDRResetLoop (
  PSYSHOST       Host
  )
{
  BIOS_SCRATCHPAD5_STRUCT        Spad5;
  BIOS_SCRATCHPAD0_STRUCT        Spad0;
  UINT16                         tempCounter;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  //
  // If DDR_RESET_LOOP is set then we need to detect if DDRT is installed and if so we need to
  // prevent a reset at this time to avoid a failure due to an unexpected clock stop error.
  //
  if ((Setup->mem.options & DDR_RESET_LOOP) && (Host->nvram.mem.DcpmmPresent == 0)) {
    //
    // Check bit 2 to see if we need to keep cycling.
    //
    Spad5.Data = GetStickyScratchpad (0, BIOS_STICKY_SCRATCHPAD_5);
    Spad0.Data = GetStickyScratchpad (0, BIOS_STICKY_SCRATCHPAD_0);
    //
    // If the user has requested MRC cycling and this is the first
    // time through the loop, set BIT2 of SPAD0 here to keep cycling
    // in the future.  Someone can then clear this bit with ITP or
    // external tools to stop cycling and proceed to system boot.
    //
    if (Spad5.Bits.DdrLoopCount == 0) {
      Spad0.Bits.DdrLoopTest = 1;
      SetStickyScratchpad (0, BIOS_STICKY_SCRATCHPAD_0, Spad0.Data);
    }
    if (Spad0.Bits.DdrLoopTest) {
      //
      // Increment the loop count.
      //

      tempCounter = (UINT16) Spad5.Data;
      tempCounter++;
      Spad5.Bits.DdrLoopCount = tempCounter;
      SetStickyScratchpad (0, BIOS_STICKY_SCRATCHPAD_5, Spad5.Data);
      RcDebugPrint (SDBG_MINMAX,
                     "DDR LC: %d\n", tempCounter);

      SetSysResetRequired(GetSysResetRequired() | POST_RESET_WARM);
      return  FAILURE;
    } else {
      //
      // Clear out the counter since we're done cycling.
      //

      Spad5.Data = GetStickyScratchpad (0, BIOS_STICKY_SCRATCHPAD_5);
      Spad5.Bits.DdrLoopCount = 0;
      SetStickyScratchpad (0, BIOS_STICKY_SCRATCHPAD_5, Spad5.Data);

    }
  } // if DDR_RESET_LOOP

  return SUCCESS;
} // DDRResetLoop

#define LT_ESTS   0xFED30008
#define LT_E2STS  0xFED308F0
/**

  Checks for secrets in memory

  @param Host  - Pointer to sysHost

  @retval N/A

**/
VOID
CheckSecrets10nm (
  PSYSHOST Host
  )
{
  CAPID0_PCU_FUN3_STRUCT  capId0;

  capId0.Data = Host->var.common.procCom[0].capid0;
  if (capId0.Bits.lt_sx_en) {
    if (((*(UINT32 *) (UINTN) LT_ESTS) & BIT6)) {
      SetWipeMemory ();
      Host->var.mem.skipMemoryInit = 0;
    } else if (((*(UINT32 *) (UINTN) LT_E2STS) & BIT1)) {
      SetWipeMemory ();
      Host->var.mem.skipMemoryInit = 0;
    }
  }
} // CheckSecrets10nm


/**
  Common DDR training entry

  @param Host     - Pointer to sysHost structure

  @retval SUCCESS:  Memory initialization completed successfully.
  @retval FAILURE:  A reset is required
**/
UINT32
DdrTraining (
  IN PSYSHOST Host
  )
{
  UINT32              Status;
  CallTableEntryType  *TrainingCallTable;
  UINT8               TrainingTableSize;

  Status = FAILURE;
  ZeroMem ((VOID *) &Host->var.mem.TrainingVar, sizeof (struct TrainingVariable));
  GetTrainingCallTablePtrs(&TrainingCallTable, &TrainingTableSize);
  Status = MemMain (Host, TrainingCallTable, TrainingTableSize, Host->var.mem.currentSocket);
  SetMemPhaseCP (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MAJOR_NO_CODE);

  return Status;
} // DdrTraining10nm

/**

  Restore timing parameters for fast boot or resume paths

  @param[in]  Host    - Pointer to sysHost
  @param[in]  Socket  - Processor to initialize
  @param[in]  Ch      - Channel number

  @retval N/A

**/
VOID
SetRestoreTimings (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT8     Ch
  )
{
  SetRestoreTimingsDdrio (Host, Socket, Ch);
}
