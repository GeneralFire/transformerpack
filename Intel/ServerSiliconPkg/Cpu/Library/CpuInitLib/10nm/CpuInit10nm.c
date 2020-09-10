/** @file
  Cpu Initialization Common Module.

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
#include <Library/CsrAccessLib.h>
#include <Library/BaseLib.h>
#include <Library/CpuIpLib.h>

/**

  Set B2P SET_MIN_ICCP_LICENSE

  @param socket:         CPU socket ID
  @param MailboxData:    Value to B2P

  @retval VOID

**/
VOID
SetB2PAvxIccpLicense (
  IN      UINT8     Socket,
  IN      UINT32    MailboxData
  )
{
  UINT32 PcodeMailboxStatus = 1;
  B2P_READ_WRITE_PM_CONFIG_INTERFACE_STRUCT    WritePmCfgCmdB2p;

  WritePmCfgCmdB2p.Data = 0;

  WritePmCfgCmdB2p.Bits.command = (UINT32) MAILBOX_BIOS_CMD_WRITE_PM_CONFIG;
  WritePmCfgCmdB2p.Bits.SubCommand = (UINT32) MAILBOX_BIOS_CMD_WRITE_PM_CONFIG_INTERFACE_SUB_COMMAND_MIN_ICCP_LEVEL;

  PcodeMailboxStatus = ReadModifyWritePcuMailbox (Socket, MAILBOX_BIOS_CMD_ZERO, WritePmCfgCmdB2p.Data, &MailboxData, 0, 0);

  RcDebugPrint (SDBG_DEFAULT, "\nSend B2P MAILBOX_BIOS_CMD_WRITE_PM_CONFIG (0x%x) = 0x%x on S%d, PcodeMailboxStatus:0x%x\n",
             WritePmCfgCmdB2p.Data, MailboxData, Socket, PcodeMailboxStatus);

}

/**

  Set a given TDP level on all sockets

  @param ConfLevelInfoPtr:    Pointer to CPU_CONFIG_LEVEL_INFO Structure

  @retval VOID

**/
VOID
ActiveIssLevel_IcxA (
  IN OUT CPU_CONFIG_LEVEL_INFO *ConfLevelInfoPtr
  )
{
  UINT8          Socket;
  UINT32         PcodeMailboxStatus;
  UINT32         MailboxData;
  UINT32         IssLevel;
  UINT32         AvxLevel;
  CONFIG_TDP_CONTROL_PCU_FUN3_STRUCT      ConfigTdpCtlCsr;

  IssLevel = ConfLevelInfoPtr->IssSystemStatus;
  AvxLevel = ConfLevelInfoPtr->AvxSystemStatus;

  if ((IssLevel == 0) && (AvxLevel >0)) {
    IssLevel = AvxLevel;
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if ((!RasInProgress() && (SocketPresent(Socket))) ||
      (RasInProgress() && GetRasSocId() == Socket)) {

     if ((ConfLevelInfoPtr->DynamicIssEnableFlag == 0) && (ConfLevelInfoPtr->IssMaxLevel[Socket] > 0)) {
       RcDebugPrint (SDBG_DEFAULT, "  ::  S%d: Active SST-PP Level: %d [ICX-LCC]\n", Socket, IssLevel);

       ConfigTdpCtlCsr.Data = UsraCsrRead (Socket, 0, CONFIG_TDP_CONTROL_PCU_FUN3_REG);
       ConfigTdpCtlCsr.Bits.tdp_level = IssLevel;
       UsraCsrWrite (Socket, 0, CONFIG_TDP_CONTROL_PCU_FUN3_REG, ConfigTdpCtlCsr.Data);

       if (ConfLevelInfoPtr->CurrentTdpLevel[Socket] != (UINT8) (IssLevel & 0xFF)) {
         ConfLevelInfoPtr->CurrentTdpLevel[Socket] = (UINT8) (IssLevel & 0xFF);
         // B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_SET_LEVEL
         PcodeMailboxStatus = SendConfigTdpSubCommand (Socket,
                                                       MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_SET_LEVEL,
                                                       IssLevel,
                                                       NULL);
       }
     }

     // Set MAILBOX_BIOS_CMD_SET_MIN_ICCP_LICENSE
     // AVX license mapping for CPX/ICX :  SSE = 2, AVX2 = 4, AVX512 = 5
     switch (AvxLevel) {
        case 1:
          MailboxData = 4;
          break;
        case 2:
          MailboxData = 5;
          break;
        default:
          MailboxData = 2;
          break;
      }

      if (AvxLevel > 0) {
        SetB2PAvxIccpLicense (Socket, MailboxData);
      }
    }
  }
}

/**

  Set a given TDP level on all sockets

  @param ConfLevelInfoPtr:    Pointer to CPU_CONFIG_LEVEL_INFO Structure

  @retval VOID

**/
VOID
SetIssConfigTdpLevel (
  IN OUT CPU_CONFIG_LEVEL_INFO *ConfLevelInfoPtr
  )
{

  if (IsCpuAndRevision (CPU_ICXSP, REV_RX)) {
    ActiveIssLevel_IcxA (ConfLevelInfoPtr);
  } else {
    ActiveIssLevel (ConfLevelInfoPtr);
  }

}

/**

  Enable or disable SMT per platform request

  @param Socket           socket index
  @param CpuPolicy        pointer of platform policy
  @param CpuNvramData     pointer of NVRAM data

  @retval VOID

**/
VOID
ConfigSocketSmt (
  IN    UINT8                Socket,
  IN    CPU_PRE_MEM_POLICY   *CpuPolicy,
  OUT   CPU_NVRAM            *CpuNvramData
  )
{
  UINT8                                 SmtCapable = 0;
  UINT8                                 SmtEnabled = 0;
  CSR_DESIRED_CORES_PCU_FUN1_STRUCT     DesiredCoresPcuCsr;
  CSR_DESIRED_CORES_PCU_FUN1_STRUCT     DesiredCoresPcuCsrNew;
  UINT8                                 ResetNeeded;

  DesiredCoresPcuCsr.Data     = UsraCsrRead (Socket, 0, CSR_DESIRED_CORES_PCU_FUN1_REG);
  DesiredCoresPcuCsrNew.Data  = DesiredCoresPcuCsr.Data;
  SmtEnabled  = DesiredCoresPcuCsr.Bits.smt_disable ? 0 : 1;
  SmtCapable  = GetSmtCapability (Socket);
  RcDebugPrint (SDBG_DEFAULT, "\n  :: SmtEnabled = %d, SmtCapable = %d, smtEnable knob = %d\n",
                SmtEnabled, SmtCapable, CpuPolicy->smtEnable);
  ResetNeeded = GetSysResetRequired();
  //
  // If 1. someone ask cold reboot, or 2. not SMT capable, or 3. request value equal to current
  // just return to avoid waste of time
  //
  if ((!SmtCapable) || (ResetNeeded & POST_RESET_POWERGOOD)) {
    return;
  }

  if (SmtEnabled == CpuPolicy->smtEnable) {
    // Still need update this for S3 comparing
    CpuNvramData->DesiredCoresCfg2Csr[Socket] = DesiredCoresPcuCsrNew.Data;
    return;
  }

  //
  // Ask cold reboot and return, avoid waste of time
  //
  if (DesiredCoresPcuCsr.Bits.lock) {
    SetSysResetRequired (ResetNeeded | POST_RESET_POWERGOOD);
    return;
  }
  DesiredCoresPcuCsrNew.Bits.smt_disable = CpuPolicy->smtEnable ? 0 : 1;
  SetSysResetRequired (ResetNeeded | POST_RESET_WARM);
  RcDebugPrint (SDBG_DEFAULT, "  [s%d] DesiredCoresPcuCsr = %08X\n", Socket, DesiredCoresPcuCsr.Data);

  //
  // Write new data to CSR DESIRED_CORES_PCU_FUN1_REG
  //
  UsraCsrWrite (Socket, 0, CSR_DESIRED_CORES_PCU_FUN1_REG, DesiredCoresPcuCsrNew.Data);
  RcDebugPrint (SDBG_DEFAULT, "  [s%d] Writing DesiredCoresPcu CSR = %08X\n\n", Socket, DesiredCoresPcuCsrNew.Data);

  //
  // Update PMC set_strap message data register with the new SmtDisable setting
  //
  if (PcdGetBool (PcdPmcStrapSetVdmSupported)) {
    UpdatePmcSetStrapMsgSmtDisable ((UINT8) DesiredCoresPcuCsrNew.Bits.smt_disable, NULL);
  }

  CpuNvramData->DesiredCoresCfg2Csr[Socket] = DesiredCoresPcuCsrNew.Data;
}

/**

  Config failover cores
  If some core met BIST failure or MLC error, set the core to disable
  Then search any sparing core available, if true, swap the sparing core out

  @param Socket               socket index
  @param CpuPolicy            pointer of platform policy
  @param AvailCoresMask       pointer of AvailCoresMask
  @param ResolvedCoresMask    pointer of ResolvedCoresMask
  @param BistResultMask       pointer of BistResultMask, if found core mask from OscCoreDisMask,
                                    also update to BistResultMask
  @param DesiredCoresCurrent  pointer of DesiredCoresCurrent, if some core move out of disable, update this struct
  @param NonSparingCoresMask  pointer of non sparing cores mask structure

  @retval Reset Required      - TRUE, DesiredCore or/and BIST result were updated, need warm reset
                              - FALSE, no update, no warm reset required

**/
BOOLEAN
ConfigFailoverCores (
  IN      UINT8                Socket,
  IN      CPU_PRE_MEM_POLICY   *CpuPolicy,
  IN      UINT64_DATA          *AvailCoresMask,
  IN      UINT64_DATA          *ResolvedCoresMask,
  IN OUT  UINT64_DATA          *BistResultMask,
  IN OUT  UINT64_DATA          *DesiredCoresCurrent,
  OUT     UINT64_DATA          *NonSparingCoresMask
  )
{
  UINT32                                  MaxEnabledCores;
  UINT64_DATA                             BistResultCopy;
  UINT64_DATA                             DesiredCoresCopy;
  CSR_DESIRED_CORES_PCU_FUN1_STRUCT       DesiredCoresPcuCsr;
  UINT32                                  SpareCoreCount;
  UINT32                                  FusedCoreCount;
  UINT32                                  BistFailedCount;
  UINT64_DATA                             SpareCoresCurrent;
  BOOLEAN                                 ResetRequired = FALSE;
  UINT8                                   BitPosition;

  UINTN                                   Count;
  UINTN                                   Bit;
  UINT32                                  BitMask;

  NonSparingCoresMask->Data32.Low = (UINT32) -1;
  NonSparingCoresMask->Data32.High = (UINT32) -1;
  DesiredCoresPcuCsr.Data = UsraCsrRead (Socket, 0, CSR_DESIRED_CORES_PCU_FUN1_REG);
  MaxEnabledCores = DesiredCoresPcuCsr.Bits.max_cores;
  FusedCoreCount = CountMaskBits64 (AvailCoresMask->Data);
  if ((MaxEnabledCores > 0) && (FusedCoreCount > MaxEnabledCores)) {
    NonSparingCoresMask->Data32.Low = 0;
    NonSparingCoresMask->Data32.High = 0;

    for (Count = 0, Bit = 0; ((Count < MaxEnabledCores) && (Bit < 64)); Bit++) {
      BitMask = (Bit < 32) ? (1 << Bit): (1 << (Bit - 32));
      if (Bit < 32) {
        if (AvailCoresMask->Data32.Low & BitMask) {
          NonSparingCoresMask->Data32.Low |= BitMask;
          Count++;
        }
      } else {
        if (AvailCoresMask->Data32.High & BitMask) {
          NonSparingCoresMask->Data32.High |= BitMask;
          Count++;
        }
      }
    }
  }

  RcDebugPrint (SDBG_DEFAULT, " S%d MaxEnabledCores    = %d\n", Socket, MaxEnabledCores);
  RcDebugPrint (SDBG_DEFAULT, " S%d FusedCoreCount     = %d\n", Socket, FusedCoreCount);
  RcDebugPrint (SDBG_DEFAULT, " S%d NonSparingCoresMask= %016lX\n", Socket, NonSparingCoresMask->Data);

  if (!(CheckCurrentReset (POST_RESET_POWERGOOD) && CpuPolicy->CheckCpuBist && CpuPolicy->CoreFailover)) {
    return FALSE;
  }

  //
  // fused cores must > max number of enabled cores
  // MaxEnabledCores = 0 means no sparing support
  if (!(MaxEnabledCores && (FusedCoreCount > MaxEnabledCores))) {
    return FALSE;
  }

  DesiredCoresCopy.Data = DesiredCoresCurrent->Data;
  BistResultCopy.Data   = BistResultMask->Data;

  SpareCoresCurrent.Data = (DesiredCoresCopy.Data | BistResultCopy.Data) ^ BistResultCopy.Data;
  SpareCoreCount  = CountMaskBits64 (SpareCoresCurrent.Data);
  BistFailedCount = CountMaskBits64 (BistResultCopy.Data);

  //
  // Enable spare cores to replace BIST-failed cores, if possible
  //
  if (SpareCoreCount > 0) {

    //
    // If SpareCoreCount is still > 0 after replace BIST-failed cores with sparing core,
    // replace the faulty Core which has persistent MLC error with sparing core.
    //
    if (CpuPolicy->OscCoreDisMask[Socket] != 0) {
      CpuPolicy->OscCoreDisMask[Socket] &= ResolvedCoresMask->Data;
      BistResultCopy.Data |= CpuPolicy->OscCoreDisMask[Socket];

      SpareCoresCurrent.Data = (DesiredCoresCopy.Data | BistResultCopy.Data) ^ BistResultCopy.Data;
      SpareCoreCount = CountMaskBits64 (SpareCoresCurrent.Data);
      BistFailedCount = CountMaskBits64 (BistResultCopy.Data);

      RcDebugPrint (SDBG_DEFAULT, " S%d Found OscCoreDisMask = \n", Socket, CpuPolicy->OscCoreDisMask[Socket]);
    }

    RcDebugPrint (SDBG_DEFAULT, " S%d SpareCoreCount       = %d\n", Socket, SpareCoreCount);
    RcDebugPrint (SDBG_DEFAULT, " S%d BistFailedCount      = %d\n", Socket, BistFailedCount);
    RcDebugPrint (SDBG_DEFAULT, " S%d SpareCoresCurrent    = %016lX\n", Socket, SpareCoresCurrent.Data);

    while (SpareCoreCount && BistFailedCount) {
      //
      // Get bit position of first spare core and clear bit
      //
      BitPosition = (UINT8)LowBitSet64 (SpareCoresCurrent.Data);
      ClearBit64 (&SpareCoresCurrent.Data, BitPosition);
      ClearBit64 (&DesiredCoresCopy.Data, BitPosition);
      SpareCoreCount--;
      BistFailedCount--;
      ResetRequired = TRUE;
    }
  }

  //
  // Save changed config
  //
  if (ResetRequired) {
    BistResultMask->Data = BistResultCopy.Data;
    DesiredCoresCurrent->Data = DesiredCoresCopy.Data;

    return TRUE;
  }

  return FALSE;
}

/**

  Detect any core BIST failure and report the core bitmap.

  @param Socket               socket index
  @param CpuPolicy            pointer of platform policy
  @param AvailCoresMask       pointer of AvailCoresMask
  @param ResolvedCoresMask    pointer of ResolvedCoresMask
  @param BistResultMask       pointer of BistResultMask

  @retval VOID

**/
VOID
DetectCpuBistFailure (
  IN    UINT8               Socket,
  IN    CPU_PRE_MEM_POLICY  *CpuPolicy,
  IN    UINT64_DATA         *AvailCoresMask,
  IN    UINT64_DATA         *ResolvedCoresMask,
  OUT   UINT64_DATA         *BistResultMask
)
{
  UINT64          BistFailEmulationMask;
  UINT32          BistValue = 0;
  UINT8           SnrCPUModuleIndex;
  UINT64_DATA     BistResult;
  UINT8           SbspSktId;

  if ((CpuPolicy == NULL) || (AvailCoresMask == NULL) || (ResolvedCoresMask == NULL)) {
    return;
  }

  ZeroMem (&BistResult, sizeof(BistResult));
  //
  // Read BIST_RESULT CSR to get BIST failure info
  //   0 - BIST failure, 1 - BIST pass
  //
  if (CpuPolicy->CheckCpuBist && !IsSiliconWorkaroundEnabled ("S14012066447")) {
    if ((IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) && !(GetEmulation() & SIMICS_FLAG)) {
      //
      // BIST WA for SNR/TNR PCODE BIST Results
      // SNR BIST results are for the MLC, which is per-module, not per-core
      //
      BistResult.Data32.High = UsraCsrRead (Socket, 0, BIST_RESULTS1_UBOX_DFX_REG);
      BistResult.Data32.Low = UsraCsrRead (Socket, 0, BIST_RESULTS0_UBOX_DFX_REG);

      for (SnrCPUModuleIndex = 0; SnrCPUModuleIndex < SNR_MAX_TMT_MODULES; SnrCPUModuleIndex++) {
        if (((BistResult.Data32.Low >> SnrCPUModuleIndex) & 1) == 1) {
          BistValue |= (0xF << (SnrCPUModuleIndex * 4) );
        }
      }

      //
      // Removing extra BIST result from non-available or disable cores
      //
      BistResult.Data32.High = BistResult.Data32.High & AvailCoresMask->Data32.High & ResolvedCoresMask->Data32.High;
      BistResult.Data32.Low = BistValue & AvailCoresMask->Data32.Low & ResolvedCoresMask->Data32.Low;
    }  else {
      BistResult.Data32.High = UsraCsrRead (Socket, 0, BIST_RESULTS1_UBOX_DFX_REG) & AvailCoresMask->Data32.High;
      BistResult.Data32.Low = UsraCsrRead (Socket, 0, BIST_RESULTS0_UBOX_DFX_REG) & AvailCoresMask->Data32.Low;
    }

    //
    // Convert BIST fail bits to 1 and mask out core that are already disabled by writing DesiredCores
    //
    BistResult.Data ^= ResolvedCoresMask->Data;
  }

  //
  // Only for SBSP,
  //  get DFX setup option to emulate failed cores
  //
  SbspSktId = GetSysSbspSocketIdNv ();
  if ((Socket == SbspSktId) && (CpuPolicy->DfxBistFailureEmulation > 0)) {
    BistFailEmulationMask = CpuPolicy->DfxBistFailureEmulation;
    RcDebugPrint (SDBG_DEFAULT, "  :: bistFailEmulationMask = %016lX\n", BistFailEmulationMask);
    BistResult.Data = (BistFailEmulationMask & AvailCoresMask->Data) | BistResult.Data;
  }

  if (BistResultMask != NULL) {
    BistResultMask->Data = BistResult.Data;
  }
}

/**

  Process setup core disable request and ISS core disable request.
  If ISS is enabled, will skip user setting from setup knob.

  @param Socket               socket index
  @param CpuPolicy            pointer of platform policy
  @param CpuVarDataPtr        pointer of CPU var data
  @param AvailCoresMask       pointer of AvailCoresMask
  @param CoreDisableMask      pointer of core disable bit mask

  @retval VOID

**/
VOID
ConfigIssCoreMask (
  IN    UINT8               Socket,
  IN    CPU_PRE_MEM_POLICY  *CpuPolicy,
  IN    CPU_VAR_DATA        *CpuVarDataPtr,
  IN    UINT64_DATA         *AvailCoresMask,
  OUT   UINT64_DATA         *CoreDisableMask
)
{
  UINT32              ConfigTdpLevel;
  UINT64_DATA         ConfigTdpLevelEnabledCoreMask;
  UINT64_DATA         DisableMask;

  ConfigTdpLevel = CpuVarDataPtr->IssConfigTdpCurrentLevel;
  DisableMask.Data = CpuPolicy->CoreDisableMask[Socket] & AvailCoresMask->Data;

  RcDebugPrint (SDBG_DEFAULT, "  :: S%d setup input disable = %016lX \n", Socket, DisableMask.Data);

  //
  // If config TDP level is other than 0, and ISS enabled,
  //  get the core mask associated with that level and apply it here
  //
  if ((ConfigTdpLevel > 2) && (CpuVarDataPtr->IssCapableSystem == 1)) {
    ZeroMem (&DisableMask, sizeof(DisableMask));
    RcDebugPrint (SDBG_DEFAULT, "  :: SST-PP is enabled, skip user input!\n");

    if (CpuPolicy->DynamicIss) {
      RcDebugPrint (SDBG_DEFAULT, "  :: Dynamic SST-PP is enabled, skip POST time core disable!\n");
    } else {
      //
      // Get the core mask associated with this level
      //
      ConfigTdpLevelEnabledCoreMask.Data = CpuVarDataPtr->IssConfigTdpEnabledCoreMask[Socket][ConfigTdpLevel] & AvailCoresMask->Data;

      //
      // Make sure to include the core disable mask from ISS B2P mailbox
      //
      DisableMask.Data = ((~ConfigTdpLevelEnabledCoreMask.Data) & AvailCoresMask->Data);

      RcDebugPrint (SDBG_DEFAULT, "  :: S%d SST-PP disable mask = %016lX \n", Socket, DisableMask.Data);
    }
  }

  if (CoreDisableMask != NULL) {
    CopyMem (CoreDisableMask, &DisableMask, sizeof(DisableMask));
  }
}

/**

  Executed by SBSP only. Program DesiredCores, SMTDisable for all CPUs

  @retval CORE_OFF_MASK CSRs programmed
  @retval SetSysResetRequired() updated (if reset is needed)

**/
VOID
EFIAPI
SetActiveCoresAndSmtEnableDisable (
  )
{
  UINT8   Socket;
  UINT8   ResetNeeded = 0;

  UINT64_DATA  AvailCoresMask;
  UINT64_DATA  ResolvedCoresMask;
  UINT64_DATA  BistResultMask;
  UINT64_DATA  CoreDisableReqMask;
  UINT64_DATA  DesiredCoresCurrent;
  UINT64_DATA  NumberOfCoresDisabledMask;

  CSR_DESIRED_CORES_MASK0_PCU_FUN1_STRUCT  csrDesiredCoresM0Current;
  CSR_DESIRED_CORES_MASK1_PCU_FUN1_STRUCT  csrDesiredCoresM1Current;
  CSR_DESIRED_CORES_PCU_FUN1_STRUCT        DesiredCoresPcuCsr;

  CPU_PRE_MEM_POLICY   *CpuPolicy;
  CPU_NVRAM            *CpuNvramData;
  CPU_VAR_DATA         *CpuVarDataPtr;

  BOOLEAN       SpareCoreConfigured;
  UINT64_DATA   NonSparingCoresMask;
  UINT64_DATA   DesiredCoresNew;
  UINT64        Mask = 0;
  UINT8         BitPosition = 0;

  //
  // -----------------------------------------------------------------------------------------------------
  // Configure number of active(enabled) cores and SMT(HT) enable/disable based on input parameters
  //   - Assume that this is executed on a cold boot path (after a power good reset)
  //   - Read CSR RESOLVED_CORES_MASK[7:0] and RESOLVED_CORES_MASK2[7:0] (for IVT CPU) bitmap to determine
  //      how many/which cores are available in a CPU pacakge.
  //     (example, a bitmap of 00111111b indicates that 6 cores are available)
  //   - Use input parameter CoreDisableMask[]  for each socket independently
  //   - Use input parameter smtEnable and RESOLVED_CORES_MASK[24] SMT_CAPABILITY for setting smtDisable bit.
  //      Note that smt enable/disable symetry is maintained among all CPU sockets.
  // -----------------------------------------------------------------------------------------------------
  //
  RcDebugPrint (SDBG_DEFAULT, "\n:: SetActiveCoresAndSmtEnableDisable_10nm()\n");

  CpuPolicy     = GetCpuPreMemPolicy();
  CpuNvramData  = GetCpuNvramData();
  CpuVarDataPtr = GetCpuVarData();

  if (CpuNvramData == NULL) {
    RcDebugPrint (SDBG_DEFAULT, "  :: CpuNvramData Returned NULL pointer, cannot continue\n");
    RC_FATAL_ERROR ((CpuNvramData != NULL), ERR_NO_MEMORY, RC_FATAL_ERROR_MINOR_CODE_0);
    return;
  }

  if (CpuVarDataPtr == NULL) {
    RcDebugPrint (SDBG_DEFAULT, "  :: CpuVarDataPtr Returned NULL pointer, cannot continue\n");
    RC_FATAL_ERROR ((CpuVarDataPtr != NULL), ERR_NO_MEMORY, RC_FATAL_ERROR_MINOR_CODE_0);
    return;
  }

  //
  // Allow this CPU BIST error exception to avoid being promoted to fatal
  //
  SetPromoteWarningException (ERROR_CPU_BIST, ERROR_CPU_BIST_MINOR_SOME_BISTRESULTMASK);

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if ((!RasInProgress() && SocketPresent(Socket)) ||
      (RasInProgress() && GetRasSocId() == Socket)) {

      ZeroMem (&CoreDisableReqMask, sizeof(CoreDisableReqMask));
      ZeroMem (&BistResultMask, sizeof(BistResultMask));
      ZeroMem (&NumberOfCoresDisabledMask, sizeof(NumberOfCoresDisabledMask));
      ZeroMem (&DesiredCoresNew, sizeof(DesiredCoresNew));
      ResetNeeded = 0;

      AvailCoresMask.Data= PcuGetAvailableCores (Socket);
      ResolvedCoresMask.Data = PcuGetResolvedCores (Socket);

      //
      // Count resolved cores (register bits that pcode has pre-loaded) and save
      // On cold boot this register = fused cores, including spare cores
      //
      ResolvedCoresMask.Data &= AvailCoresMask.Data;

      //
      // Get and save desired cores, max cores that can be enabled
      // On cold boot desired cores = spare cores
      //
      csrDesiredCoresM0Current.Data = UsraCsrRead (Socket, 0, CSR_DESIRED_CORES_MASK0_PCU_FUN1_REG);
      csrDesiredCoresM1Current.Data = UsraCsrRead (Socket, 0, CSR_DESIRED_CORES_MASK1_PCU_FUN1_REG);
      DesiredCoresCurrent.Data32.High = csrDesiredCoresM1Current.Bits.core_off_mask;
      DesiredCoresCurrent.Data32.Low = csrDesiredCoresM0Current.Bits.core_off_mask;
      DesiredCoresPcuCsr.Data = UsraCsrRead (Socket, 0, CSR_DESIRED_CORES_PCU_FUN1_REG);

      //
      // Save Core info
      //
      CpuVarDataPtr->FusedCores[Socket] = AvailCoresMask.Data;
      CpuVarDataPtr->ActiveCores[Socket] = ResolvedCoresMask.Data;

      //
      // Produce BistResultMask if any BIST failure detected
      //
      DetectCpuBistFailure (Socket, CpuPolicy, &AvailCoresMask, &ResolvedCoresMask, &BistResultMask);

      //
      // Handle ISS and setup knob core disable request
      //
      ConfigIssCoreMask (Socket, CpuPolicy, CpuVarDataPtr, &AvailCoresMask, &CoreDisableReqMask);

      ConvertNumberOfCores2DisableToMask (Socket, CpuPolicy, &AvailCoresMask, &CoreDisableReqMask, &NumberOfCoresDisabledMask);

      RcDebugPrint (SDBG_DEFAULT, "  :: S%d AvailCoresMask      = %016lx\n", Socket, AvailCoresMask.Data);
      RcDebugPrint (SDBG_DEFAULT, "  :: S%d ResolvedCoresMask   = %016lx\n", Socket, ResolvedCoresMask.Data);
      RcDebugPrint (SDBG_DEFAULT, "  :: S%d DesiredCoresCurrent = %016lx\n", Socket, DesiredCoresCurrent.Data);
      RcDebugPrint (SDBG_DEFAULT, "  :: S%d BistResultMask      = %016lx\n", Socket, BistResultMask.Data);
      RcDebugPrint (SDBG_DEFAULT, "  :: S%d CoreDisableReqMask  = %016lx\n", Socket, CoreDisableReqMask.Data);
      RcDebugPrint (SDBG_DEFAULT, "  :: S%d NumberOfCoresDisabledMask  = %016lx\n", Socket, NumberOfCoresDisabledMask.Data);

      RcDebugPrint (SDBG_DEFAULT, "  :: CheckCpuBist          = %d\n", CpuPolicy->CheckCpuBist);
      RcDebugPrint (SDBG_DEFAULT, "  :: CoreFailover          = %d\n", CpuPolicy->CoreFailover);

      ConfigSocketSmt (Socket, CpuPolicy, CpuNvramData);

      SpareCoreConfigured = ConfigFailoverCores (
                                  Socket,
                                  CpuPolicy,
                                  &AvailCoresMask,
                                  &ResolvedCoresMask,
                                  &BistResultMask,
                                  &DesiredCoresCurrent,
                                  &NonSparingCoresMask);
      if (SpareCoreConfigured) {
        ResetNeeded |= POST_RESET_WARM;
      }

      //
      // If BIST failure is found, and sparing is not possible, create error log entry
      //  else try switch sparing core
      //
      if (BistResultMask.Data && (BistResultMask.Data != ResolvedCoresMask.Data)) {
        if (SpareCoreConfigured) {
          RcDebugPrint (
              SDBG_DEFAULT,
              "  :: S%d BIST error found and logged, spare core enabled, BistResultMask = 0x%016lX\n",
              Socket,
              BistResultMask.Data);
          LogCpuWarning (WARN_CPU_BIST, WARN_CPU_BIST_MINOR_ALL, Socket);
        } else {
          RcDebugPrint (
              SDBG_DEFAULT,
              "  :: S%d BIST error found and logged, sparing disabled, BistResultMask = 0x%016lX\n",
              Socket,
              BistResultMask.Data);
          LogCpuError (ERROR_CPU_BIST, ERROR_CPU_BIST_MINOR_SOME_BISTRESULTMASK, Socket,
              BistResultMask.Data);
        }
      } else if (BistResultMask.Data == ResolvedCoresMask.Data) {
        //
        // Check if all cores of the Socket failed BIST
        //
        RcDebugPrint (SDBG_DEFAULT, "  :: S%d BIST errors cause all cores disabled!\n", Socket);
        FatalError (ERROR_CPU_BIST, ERROR_CPU_BIST_MINOR_ALL);
        CpuDeadLoop ();
      }

      //
      // Build our mask of cores to disable
      //
      // 1st, merge setup and BIST disable request together to new value
      //
      DesiredCoresNew.Data |= CoreDisableReqMask.Data | BistResultMask.Data | NumberOfCoresDisabledMask.Data;

      //
      // If disable all the cores, force reserve one core as active.
      //
      Mask = ResolvedCoresMask.Data & (~DesiredCoresNew.Data);
      if (Mask == 0) {
        BitPosition = (UINT8)LowBitSet64 (DesiredCoresNew.Data);
        ClearBit64 (&DesiredCoresNew.Data, BitPosition);
        RcDebugPrint (SDBG_DEFAULT, " \n S%d Warning: Try to disable all cores, force reserve one core as active!\n", Socket);
      }

      //
      // Then remove sparing cores bits in DesiredCores, and compare with current
      //  disable request to check anything changed
      //
      if ((DesiredCoresCurrent.Data & NonSparingCoresMask.Data) != DesiredCoresNew.Data) {
        //
        // If DesiredCores CSR is locked, this has all been a waste of time - hard reset and try again
        //
        if (DesiredCoresPcuCsr.Bits.lock) {
          SetSysResetRequired(GetSysResetRequired() | POST_RESET_POWERGOOD);
          break;
        }

        //
        // Don't forget to carry the sparing cores bits in current DesiredCores
        //
        DesiredCoresNew.Data |= DesiredCoresCurrent.Data & (~NonSparingCoresMask.Data);
        ResetNeeded |= POST_RESET_WARM;
      }

      if (ResetNeeded) {
        SetSysResetRequired (GetSysResetRequired() | ResetNeeded);

      } else {
        //
        // No reset needed, so let's make sure we write desired
        //  cores back the same as we read them
        //
        DesiredCoresNew.Data = DesiredCoresCurrent.Data;
      }

      RcDebugPrint (SDBG_DEFAULT, "\n  :: S%d  DesiredCoresNew = %016lx\n", Socket, DesiredCoresNew.Data);

      //
      // CSR is not locked (will defer locking this CSR before hand off to OS)
      //  and new data != current data
      //
      if ((DesiredCoresPcuCsr.Bits.lock == 0) && (DesiredCoresCurrent.Data != DesiredCoresNew.Data)) {
        //
        // Write new data to CSR CSR_DESIRED_CORES
        //
        UsraCsrWrite (Socket, 0, CSR_DESIRED_CORES_MASK0_PCU_FUN1_REG, DesiredCoresNew.Data32.Low);
        UsraCsrWrite (Socket, 0, CSR_DESIRED_CORES_MASK1_PCU_FUN1_REG, DesiredCoresNew.Data32.High);
        RcDebugPrint (SDBG_DEFAULT, "  :: S%d Writing DesiredCores CSR = %016lx\n\n", Socket, DesiredCoresNew.Data);

        //
        // Update PMC set_strap message data register with the new core_off_mask settings
        //
        if (PcdGetBool (PcdPmcStrapSetVdmSupported)) {
          UpdatePmcSetStrapMsgCoreOffMask (DesiredCoresNew.Data32.Low, DesiredCoresNew.Data32.High, NULL);
        }
      }

      //
      // Save current DESIRED_CORES CSR for S3 resume
      //
      CpuNvramData->DesiredCoresCsr[Socket] = DesiredCoresNew.Data;
    }  //end if Socket present

  } //end for Socket loop
}

/**

    Get B2P UNCORE_P0_P1_INFO

    @param Socket:    Socket # being configured
    @param MsrValue:  MSR Low 32bits value

    @retval:        Return data from B2P mailbox command

**/
UINT32
CpuUncoreRatioInit (
  UINT8         Socket,
  UINT32        MsrValue
  )
{
  UINT8                                             ConfigTdpLevel;
  MSR_UNCORE_RATIO_LIMIT_REGISTER                   UncoreRatioLimit;
  B2P_CONFIG_TDP_GET_UNCORE_P0_P1_INFO_DATA_STRUCT  GetUncoreP0P1Return;
  B2P_CONFIG_TDP_GET_RATIO_INFO_RETURN_DATA_STRUCT  GetRatioInfoReturn;
  B2P_GET_LEVELS_INFO_OUTPUT_STRUCT                 GetLevelsInfoReturn;

  UncoreRatioLimit.Uint32 = MsrValue;

  GetLevelsInfoReturn.Data = GetB2pConfigTdpLevelInfo (Socket, 0);
  ConfigTdpLevel = (UINT8) GetLevelsInfoReturn.Bits.CurrentTdpLevel;

  if (IsCpuAndRevision (CPU_ICXSP, REV_RX)) {
    RcDebugPrint (SDBG_DEFAULT, "\n  :: Skip for ICX R Stepping!\n");
  } else if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
    RcDebugPrint (SDBG_DEFAULT, "\n  :: Skip for SNR Stepping!\n");
  } else if (IsCpuAndRevision (CPU_TNR, REV_ALL)) {
    RcDebugPrint (SDBG_DEFAULT, "\n  :: Skip for TNR Stepping!\n");
  } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)){
    // get B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_UNCORE_P0_P1_INFO
    GetUncoreP0P1Return.Data = GetB2pConfigTdpGetUncoreP0P1 (Socket, ConfigTdpLevel);
    if (GetUncoreP0P1Return.Data > 0) {
      //
      // MinClrRatio should be set to Uncore Pn, but GET_UNCORE_P0_P1_INFO
      // could not return Uncore Pn. So keep MinClrRatio to be original MSR
      // value (normally it should have been Uncore Pn) or be assigned to
      // default value later if original MSR value is 0.
      //
      UncoreRatioLimit.Bits.MaxClrRatio = (UINT8) GetUncoreP0P1Return.Bits.UncoreP0;
    }
  } else {
    // Read Uncore Ratio
    // get B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_RATIO_INFO
    GetRatioInfoReturn.Data = GetB2pConfigTdpGetRatioInfo (Socket, ConfigTdpLevel, 1);
    if (GetRatioInfoReturn.Data > 0) {
      UncoreRatioLimit.Bits.MaxClrRatio = (UINT8) GetRatioInfoReturn.Bits.P0;
      UncoreRatioLimit.Bits.MinClrRatio = (UINT8) GetRatioInfoReturn.Bits.Pn;
    }
  }

  RcDebugPrint (SDBG_DEFAULT, "\n:  Get UncoreRatioLimit  MaxClrRatio: %x,  MinClrRatio: %x,    UncoreRatioLimit.Uint32 = 0x%x\n",
    UncoreRatioLimit.Bits.MaxClrRatio, UncoreRatioLimit.Bits.MinClrRatio, UncoreRatioLimit.Uint32);

  //If B2P Command is not available
  if (UncoreRatioLimit.Bits.MaxClrRatio == 0) {
    UncoreRatioLimit.Bits.MaxClrRatio = 0x3f;
  }

  if (UncoreRatioLimit.Bits.MinClrRatio == 0) {
    UncoreRatioLimit.Bits.MinClrRatio = 8;
  }

  return UncoreRatioLimit.Uint32;
}

/**

    Get B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_P1_INFO

    @param Socket:        Socket # being configured
    @param ConfigIndex:   ConfigTdp Level

    @retval          FuseP1 value

**/
UINT32
GetB2PFuseP1Ratio (
  UINT8         Socket,
  UINT8         ConfigIndex
  )
{
  UINT32                                    PcodeMailboxStatus = 0;
  B2P_CONFIG_TDP_GET_P1_INFO_DATA_STRUCT    GetP1InfoReturn;

  GetP1InfoReturn.Data = 0;

  if (!(IsCpuAndRevision (CPU_ICXSP, REV_RX) || IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL))) {
    PcodeMailboxStatus = SendConfigTdpSubCommand (
                           Socket,
                           MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_P1_INFO,
                           ConfigIndex,
                           &GetP1InfoReturn.Data
                           );
  }

  if (PcodeMailboxStatus == 0) {
    RcDebugPrint (SDBG_DEFAULT, "\nGet P1 = 0x%x from MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_P1_INFO (0x%x)\n",
                GetP1InfoReturn.Data, MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_P1_INFO);
  } else {
    RcDebugPrint (SDBG_DEFAULT, "\nFail to get MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_P1_INFO (0x%x) = 0x%x, MBoxStatus = 0x%x\n",
                MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_P1_INFO, GetP1InfoReturn.Data, PcodeMailboxStatus);
  }

  return GetP1InfoReturn.Data;
}

/**

  Get TDP information for TDP levels supported

  @param Socket:              Socket to get info for
  @param ConfLevelInfoPtr:    Pointer to CPU_CONFIG_LEVEL_INFO Structure

  @retval VOID

**/
VOID
GetConfigTdpLevelInfo (
  IN  UINT8                    Socket,
  IN OUT CPU_CONFIG_LEVEL_INFO *ConfLevelInfoPtr
  )
{
  UINT8                                     ConfigTdpLevel;
  UINT8                                     MaxLevel;
  B2P_GET_LEVELS_INFO_OUTPUT_STRUCT         GetLevelsInfoReturn;
  B2P_GET_TDP_INFO_OUTPUT_STRUCT            GetTdpInfoReturn;
  B2P_CONFIG_TDP_GET_P1_INFO_DATA_STRUCT    GetP1InfoReturn;
  B2P_GET_ICCP_TJMAX_OUTPUT_STRUCT          GetIccpTjmaxReturn;
  B2P_SEND_SUBCOMMAND_DATA_GET_CORE_MASK    GetCoreMask;
  UINT64_DATA                               FusedCoresMask;
  UINT64_DATA                               CoreMaskReturn;
  PLATFORM_INFO_N1_PCU_FUN0_STRUCT          PlatformInfoN1Csr;
  B2P_SEND_SUBCOMMAND_DATA_GET_TURBO_LIMIT  GetTurboRatioLimit;
  B2P_CONFIG_TDP_GET_CONFIG_TDP_CONTROL_DATA_STRUCT  B2pGetConfigTdpCtrl;
  B2P_SEND_SUBCOMMAND_DATA_PBF_GET_CORE_MASK_INFO    B2pPbfGetCoreMask;
  B2P_CONFIG_TDP_PBF_GET_P1HI_P1LO_INFO_DATA_STRUCT  B2pPbfGetP1HiP1Lo;
  B2P_CONFIG_TDP_GET_RATIO_INFO_RETURN_DATA_STRUCT   GetRatioInfoReturn;

  RcDebugPrint (SDBG_DEFAULT, "  :: GetConfigTdpLevelInfo ()\n");

  MaxLevel = ConfLevelInfoPtr->ConfigTdpMaxLevel[Socket];

  PlatformInfoN1Csr.Data = ConfLevelInfoPtr->PlatformInfoN1Csr[Socket];

  if (MaxLevel > 0) {
    ConfLevelInfoPtr->AvxSystemStatus |= (1 << Socket);
  }

  if ((MaxLevel > 2) || PlatformInfoN1Csr.Bits.config_tdp_ext_en) {
    GetLevelsInfoReturn.Data = GetB2pConfigTdpLevelInfo(Socket, 0);
    if (GetLevelsInfoReturn.Bits.Enabled) {
      ConfLevelInfoPtr->IssSystemStatus |= (1 << Socket);

      if (GetLevelsInfoReturn.Bits.MaxTdpLevel > 2) {
        MaxLevel = GetLevelsInfoReturn.Bits.MaxTdpLevel & 0xFF;
        ConfLevelInfoPtr->IssMaxLevel[Socket] = MaxLevel;
        ConfLevelInfoPtr->CurrentTdpLevel[Socket] = GetLevelsInfoReturn.Bits.CurrentTdpLevel & 0xFF;
      }
    } else {
      MaxLevel = 2;
    }
  }
  RcDebugPrint (SDBG_DEFAULT, "Socket:%d - MaxLevel:%d\n", Socket, MaxLevel);

  // Sanity check: Pcode return against data structure
  if (MaxLevel + 1 > CONFIG_TDP_TOTAL_LEVEL) {
    RcDebugPrint (SDBG_ERROR, "Fatal Error: Inconsistant TDP levels. BIOS stopped\n");
    CpuDeadLoop ();
  }

  // Get the # of cores returned by fused cores CSR
  FusedCoresMask.Data = PcuGetAvailableCores (Socket);

  for (ConfigTdpLevel = 0; ConfigTdpLevel <= MaxLevel; ConfigTdpLevel++) {

    //Init CoreCount field from FusedCoresMask, it may be chnaged below in case CoreMaskReturn.Data > 0
    ConfLevelInfoPtr->CoreCount[Socket][ConfigTdpLevel] = CountMaskBits64 (FusedCoresMask.Data);
    RcDebugPrint (SDBG_ERROR, "Init CoreCount[%d][%d] = %d\n", Socket, ConfigTdpLevel, ConfLevelInfoPtr->CoreCount[Socket][ConfigTdpLevel]);

    if ((ConfigTdpLevel == 1) || (ConfigTdpLevel == 2)) {
      GetTdpInfoReturn.Data = GetConfigTdpInfoFromCsr (Socket, ConfigTdpLevel);
    } else {
      // B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_TDP_INFO
      GetTdpInfoReturn.Data = GetB2pConfigTdpInfo(Socket, ConfigTdpLevel);
    }

    if (GetTdpInfoReturn.Data > 0) {
      ConfLevelInfoPtr->TdpRatio[Socket][ConfigTdpLevel] = (UINT8) GetTdpInfoReturn.Bits.RatioThisLevel;
      ConfLevelInfoPtr->PkgTdp[Socket][ConfigTdpLevel] = (UINT16) GetTdpInfoReturn.Bits.PkgTdpThisLevel;
    }

    if ((ConfigTdpLevel == 1) || (ConfigTdpLevel == 2)) {
      continue;
    }

    // get B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_P1_INFO
    GetP1InfoReturn.Data = GetB2PFuseP1Ratio (Socket, ConfigTdpLevel);

    if (GetP1InfoReturn.Data > 0) {
      ConfLevelInfoPtr->SseP1Ratio[Socket][ConfigTdpLevel] = (UINT8) GetP1InfoReturn.Bits.SseP1;
      ConfLevelInfoPtr->Avx2P1Ratio[Socket][ConfigTdpLevel] = (UINT8) GetP1InfoReturn.Bits.Avx2P1;
      ConfLevelInfoPtr->Avx3P1Ratio[Socket][ConfigTdpLevel] = (UINT8) GetP1InfoReturn.Bits.Avx3P1;
    }

    if (IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) {
      RcDebugPrint (SDBG_DEFAULT, "\n  :: Skip ConfigTdpGetRatioInfo for SNR/TNR Stepping!\n");
    } else {
      //Read Core Ratio
      GetRatioInfoReturn.Data = GetB2pConfigTdpGetRatioInfo (Socket, ConfigTdpLevel, 0);

      if (GetRatioInfoReturn.Data > 0) {
        ConfLevelInfoPtr->CoreP0Ratio[Socket][ConfigTdpLevel] = (UINT8) GetRatioInfoReturn.Bits.P0;
        ConfLevelInfoPtr->CoreP1Ratio[Socket][ConfigTdpLevel] = (UINT8) GetRatioInfoReturn.Bits.P1;
        ConfLevelInfoPtr->CorePnRatio[Socket][ConfigTdpLevel] = (UINT8) GetRatioInfoReturn.Bits.Pn;
        ConfLevelInfoPtr->CorePmRatio[Socket][ConfigTdpLevel] = (UINT8) GetRatioInfoReturn.Bits.Pm;
      }

      //Read Uncore Ratio
      GetRatioInfoReturn.Data = GetB2pConfigTdpGetRatioInfo (Socket, ConfigTdpLevel, 1);

      if (GetRatioInfoReturn.Data > 0) {
        ConfLevelInfoPtr->UncoreP0Ratio[Socket][ConfigTdpLevel] = (UINT8) GetRatioInfoReturn.Bits.P0;
        ConfLevelInfoPtr->UncoreP1Ratio[Socket][ConfigTdpLevel] = (UINT8) GetRatioInfoReturn.Bits.P1;
        ConfLevelInfoPtr->UncorePnRatio[Socket][ConfigTdpLevel] = (UINT8) GetRatioInfoReturn.Bits.Pn;
        ConfLevelInfoPtr->UncorePmRatio[Socket][ConfigTdpLevel] = (UINT8) GetRatioInfoReturn.Bits.Pm;
      }
    }

    // B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_PWR_INFO
    ConfLevelInfoPtr->PowerMinMax[Socket][ConfigTdpLevel] = GetB2pConfigTdpPowerInfo(Socket, ConfigTdpLevel);

    // B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_ICCP_TJMAX_INFO
    GetIccpTjmaxReturn.Data = GetB2pConfigTdpTjMax(Socket, ConfigTdpLevel);
    if (GetIccpTjmaxReturn.Data > 0) {
      ConfLevelInfoPtr->Tjmax[Socket][ConfigTdpLevel] = (UINT8) GetIccpTjmaxReturn.Bits.MaxJunctionTempThisLevel;
    }

    // B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_CORE_MASK
    //  MaskIndex Bit[15:8] Input: 0 returns bits 31:0, 1 returns bits 63:32
    GetCoreMask.Bits.ConfigIndex = ConfigTdpLevel;

    GetCoreMask.Bits.MaskIndex = 0;
    ConfLevelInfoPtr->IssEnabledCoreMask[Socket][ConfigTdpLevel] = (UINT32)GetB2pConfigTdpCoreMask(Socket, GetCoreMask.Data);

    GetCoreMask.Bits.MaskIndex = 1;
    ConfLevelInfoPtr->IssEnabledCoreMask[Socket][ConfigTdpLevel] = (UINT64)LShiftU64 (GetB2pConfigTdpCoreMask(Socket, GetCoreMask.Data), 32) | (UINT64)ConfLevelInfoPtr->IssEnabledCoreMask[Socket][ConfigTdpLevel];
    CoreMaskReturn.Data = ConfLevelInfoPtr->IssEnabledCoreMask[Socket][ConfigTdpLevel] & FusedCoresMask.Data;

    if (CoreMaskReturn.Data > 0) {
      ConfLevelInfoPtr->CoreCount[Socket][ConfigTdpLevel] = CountMaskBits64 (CoreMaskReturn.Data);
      RcDebugPrint (SDBG_ERROR, "Updated CoreCount[%d][%d] = %d\n", Socket, ConfigTdpLevel, ConfLevelInfoPtr->CoreCount[Socket][ConfigTdpLevel]);
    }

    // B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_TURBO_LIMIT
    GetTurboRatioLimit.Bits.ConfigIndex = ConfigTdpLevel;
    GetTurboRatioLimit.Bits.AvxIndex = 0;
    GetTurboRatioLimit.Bits.WordIndex = 0;
    ConfLevelInfoPtr->SseTurboRatio[Socket][ConfigTdpLevel].Data32.High = GetB2pConfigTdpTurboLimit(Socket, GetTurboRatioLimit.Data);
    GetTurboRatioLimit.Bits.WordIndex = 1;
    ConfLevelInfoPtr->SseTurboRatio[Socket][ConfigTdpLevel].Data32.Low = GetB2pConfigTdpTurboLimit(Socket, GetTurboRatioLimit.Data);

    GetTurboRatioLimit.Bits.AvxIndex = 1;
    GetTurboRatioLimit.Bits.WordIndex = 0;
    ConfLevelInfoPtr->Avx2TurboRatio[Socket][ConfigTdpLevel].Data32.High = GetB2pConfigTdpTurboLimit(Socket, GetTurboRatioLimit.Data);
    GetTurboRatioLimit.Bits.WordIndex = 1;
    ConfLevelInfoPtr->Avx2TurboRatio[Socket][ConfigTdpLevel].Data32.Low = GetB2pConfigTdpTurboLimit(Socket, GetTurboRatioLimit.Data);

    GetTurboRatioLimit.Bits.AvxIndex = 2;
    GetTurboRatioLimit.Bits.WordIndex = 0;
    ConfLevelInfoPtr->Avx3TurboRatio[Socket][ConfigTdpLevel].Data32.High = GetB2pConfigTdpTurboLimit(Socket, GetTurboRatioLimit.Data);
    GetTurboRatioLimit.Bits.WordIndex = 1;
    ConfLevelInfoPtr->Avx3TurboRatio[Socket][ConfigTdpLevel].Data32.Low = GetB2pConfigTdpTurboLimit(Socket, GetTurboRatioLimit.Data);

    //
    // Get PBF Capable
    //
    B2pGetConfigTdpCtrl.Data = GetB2pConfigTdpControl(Socket, ConfigTdpLevel);
    ConfLevelInfoPtr->PbfCapable[Socket][ConfigTdpLevel] = (UINT8) B2pGetConfigTdpCtrl.Bits.PbfSupport;

    if (B2pGetConfigTdpCtrl.Bits.PbfSupport) {
      // B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_PBF_GET_CORE_MASK_INFO
      B2pPbfGetCoreMask.Bits.ConfigIndex = ConfigTdpLevel;
      B2pPbfGetCoreMask.Bits.MaskIndex = 0;
      ConfLevelInfoPtr->PbfP1HiCoreMap[Socket][ConfigTdpLevel] = (UINT32)GetB2pConfigTdpPbfGetCoreMask(Socket, B2pPbfGetCoreMask.Data) & FusedCoresMask.Data32.Low;

      B2pPbfGetCoreMask.Bits.MaskIndex = 1;
      ConfLevelInfoPtr->PbfP1HiCoreMap[Socket][ConfigTdpLevel] = (UINT64) LShiftU64 (GetB2pConfigTdpPbfGetCoreMask(Socket, B2pPbfGetCoreMask.Data) & FusedCoresMask.Data32.High, 32) | (UINT64) ConfLevelInfoPtr->PbfP1HiCoreMap[Socket][ConfigTdpLevel];


      // B2P MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_PBF_GET_P1HI_P1LO_INFO
      B2pPbfGetP1HiP1Lo.Data = GetB2pConfigTdpPbfGetP1HiP1Lo(Socket, ConfigTdpLevel);
      ConfLevelInfoPtr->PbfP1LowRatio[Socket][ConfigTdpLevel] = (UINT8) B2pPbfGetP1HiP1Lo.Bits.P1Lo;
      ConfLevelInfoPtr->PbfP1HighRatio[Socket][ConfigTdpLevel] = (UINT8) B2pPbfGetP1HiP1Lo.Bits.P1Hi;
    }
  }

  return;
}

/**
  Get SST-CP system level Status.

  @return SST-CP system level Status.

**/
UINT32
GetSstCpSystemStatus (
  VOID
  )
{
  UINT8       Socket;
  UINT8       SocketEnableBitMap = 0;
  UINT8       SstCpCapableSystem = 0;
  UINT8       SstCpEnableSystem = 0;

  B2P_READ_PM_CONFIG_OUTPUT_STRUCT  GetPmFeatureReturn;

  if (!(IsCpuAndRevision(CPU_SNR, REV_ALL) || IsCpuAndRevision(CPU_TNR, REV_ALL))) {
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if (SocketPresent(Socket)) {
        SocketEnableBitMap |= (1 << Socket);

        GetPmFeatureReturn.Data = ReadPmFeatureB2p (Socket);

        if ((GetPmFeatureReturn.Data > 0) && (GetPmFeatureReturn.Bits.SstCpCapability == 1)) {
          SstCpCapableSystem |= (1 << Socket);
        }

        if ((GetPmFeatureReturn.Data > 0) && (GetPmFeatureReturn.Bits.SstCpState == 1)) {
          SstCpEnableSystem |= (1 << Socket);
        }
      }
    }
  }
  RcDebugPrint (SDBG_DEFAULT, "\n  :: SocketEnableBitMap: 0x%x, SstCpCapableSystem: 0x%x,  SstCpEnableSystem: 0x%x\n",
                              SocketEnableBitMap, SstCpCapableSystem, SstCpEnableSystem);

  GetPmFeatureReturn.Data = 0;

  if (SstCpCapableSystem == SocketEnableBitMap) {
    GetPmFeatureReturn.Bits.SstCpCapability = 1;
  }

  if (SstCpEnableSystem == SocketEnableBitMap) {
    GetPmFeatureReturn.Bits.SstCpState = 1;
  }

  RcDebugPrint (SDBG_DEFAULT, "\n  ::  SST-CP system status - Capable: %d,    Enable Status: %d\n",
                              GetPmFeatureReturn.Bits.SstCpCapability, GetPmFeatureReturn.Bits.SstCpState);

  return GetPmFeatureReturn.Data;
}
