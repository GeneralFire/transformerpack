/** @file
  Defines SMEE IP related functions

  This file supports SMEE IP access through API functions.

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
#include <Library/PipeSyncLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/BaseMemoryLib.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/CsrAccessLib.h>
#include <CmiPerformance.h>
#include <Library/MemSmeeIpTargetLib.h>
#include "Include/MemSmeeRegs.h"
#include <Library/SysHostPointerLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/SimulationServicesLib.h>
#include "MemSmeeCmiCredits.h"
#include <Register/Cpuid.h>
#include <Library/MemRcLib.h>
#include <Library/SecurityPolicyLib.h>

#define TME_CAPABILITY_BIT 13

typedef union {
  MSR_TME_ACTIVATE_REGISTER Reg;
  UINT64_STRUCT Data;
} TME_ACTIVATE_TYPE;

/**
  Disable Clock Gating

  @param[in] Socket       - Socket number, 0 based
  @param[in] Ch           - Channel number

**/
VOID
EFIAPI
DisableClkGating (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
  TME_SMEE_TME_STRUCT        TmeCfgRegValue;

  TmeCfgRegValue.Data = MemReadPciCfgEp (Socket, Ch, TME_SMEE_TME_REG);
  TmeCfgRegValue.Bits.disable_clk_gating = 1;
  MemWritePciCfgEp (Socket, Ch, TME_SMEE_TME_REG, TmeCfgRegValue.Data);
}
/**

Reports whether TME capability is present in the system or not

@retval TRUE  - TME capability is present in the system
        FALSE - TME capability is NOT present in the system

**/
BOOLEAN
IsTmeCapabilitySupported (
  IN UINT8 Socket
  )
{
  PSYSHOST            Host;
  BOOLEAN             IsTmeCapableSocket = FALSE;
  CPUID_REGISTER_INFO CpuIdType;

  ZeroMem (&CpuIdType, sizeof (CPUID_REGISTER_INFO));
  Host = GetSysHostPointer ();
  //
  //  TME capability in CPUID.(EAX = 7,ECX = 0):ECX[13]
  //

  CpuIdType.Ecx = Host->var.common.procCom[Socket].ExtCpuid7.Ecx;

  if ((CpuIdType.Ecx & (1 << TME_CAPABILITY_BIT)) != 0) {
    IsTmeCapableSocket = TRUE;
  }
  RcDebugPrint (SDBG_MINMAX, "S%d TME CPUID = %d\n", Socket, IsTmeCapableSocket);

  return IsTmeCapableSocket;
}

/**
  Check if TME enabled in given Socket

  @param[in] Socket       - Socket number, 0 based

  @retval TRUE -  TME is enabled
  @retval FALSE - TME is disabled
**/
BOOLEAN
EFIAPI
IsTmeEnabled (
  IN UINT8 Socket
  )
{
  TME_ACTIVATE_TYPE TmeActReg;
  BOOLEAN TmeActivated = FALSE;

  if (IsSiliconWorkaroundEnabled ("S1409604444")) {
    return FALSE;
  }

  //Read TME Activate register
  if (IsTmeCapabilitySupported (Socket)) {
    TmeActReg.Data = ReadMsrPipeHL (Socket, MSR_TME_ACTIVATE);
    if (TmeActReg.Reg.Bits.TmeEnable !=0) {
      TmeActivated =  TRUE;
    }
    RcDebugPrint (SDBG_MINMAX, "S%d TmeActivated = %d\n", Socket, TmeActivated);
  }
  return TmeActivated;
}

/**
  Checks for Sgx enabled in Cpu Policy

  @retval TRUE if SGX feature enabled in Cpu Policy
  @retval FALSE if SGX feature disabled in Cpu Policy
**/
BOOLEAN
IsSgxEnabled (
  )
{
  SECURITY_POLICY               *SecurityPolicy;
  SecurityPolicy = GetSecurityPolicy ();
  ASSERT (SecurityPolicy != NULL);
  return ((SecurityPolicy != NULL) && (SecurityPolicy->EnableSgx == SECURITY_POLICY_ENABLE));
}

/*
  Check TME registers, if we need to do CMI Initialization

  @param[in] Socket       - Socket number, 0 based
  @param[in] Ch           - Channel number, 0 based

  @retval TRUE -  TME CMI Initialization Required
  @retval FALSE - TME CMI Initialization not Required
*/
BOOLEAN
MeeCmiInitRequired (
  UINT8 Socket,
  UINT8 Ch
  )
{
  TME_SMEE_TME_STRUCT         TmeCfgRegValue;

  TmeCfgRegValue.Data = MemReadPciCfgEp (Socket, Ch, TME_SMEE_TME_REG);
  if (IsTmeEnabled (Socket) || TmeCfgRegValue.Bits.use_rd_cpl_fab_buffer != 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Program config done bit in TME CMI registers

  @param[in] Socket       - Socket number
  @param[in] McId         - Memory Controller number

  @retval N/A
**/
VOID
EFIAPI
ProgramTmeCmiConfigDoneAllCh (
  IN UINT8 Socket,
  IN UINT8 McId
  )
{
  UINT8 Ch = 0;
  UINT8 ChId = 0;
  UINT8 MaxChPerMc;
  TME_CMI_N0_SMEE_TME_STRUCT  TmeCmiCfgRegValue;
  struct channelNvram (*ChannelNvList) [MAX_CH];
  PSYSHOST Host = GetSysHostPointer ();

  if (IsSiliconWorkaroundEnabled ("S1409604444")) {
    return;
  }

  MaxChPerMc = GetNumChannelPerMc ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  //Set Initialized bit and wait for done
  for (ChId = 0; ChId < MaxChPerMc; ChId++) {
    Ch = (McId * MaxChPerMc) + ChId;
    if ((*ChannelNvList) [Ch].enabled == 0) {
      continue;
    }
    if (!MeeCmiInitRequired (Socket, Ch)) {
      return;
    }
    TmeCmiCfgRegValue.Data = MemReadPciCfgEp (Socket, Ch, TME_CMI_N0_SMEE_TME_REG);
    TmeCmiCfgRegValue.Bits.credit_config_done = 1;
    MemWritePciCfgEp (Socket, Ch, TME_CMI_N0_SMEE_TME_REG, TmeCmiCfgRegValue.Data);
  }
}

/**
  Wait for Initialized bit set for TME CMI registers. The CMI initialized
  bit will be polled till CATCHALL_TIMEOUT = 100 milliseconds. If bit dint get
  set then its a fatal condition and fatal error will be reported by this function.

  @param[in] Socket       - Socket number
  @param[in] McId         - Memory Controller number

  @retval N/A
**/
VOID
EFIAPI
WaitForTmeCmiConfigCompleteAllCh (
  IN UINT8 Socket,
  IN UINT8 McId
  )
{
  UINT8 Ch = 0;
  UINT8 ChId = 0;
  UINT8 MaxChPerMc;
  struct channelNvram (*ChannelNvList) [MAX_CH];
  PSYSHOST Host = GetSysHostPointer ();

  MaxChPerMc = GetNumChannelPerMc ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  if (IsSiliconWorkaroundEnabled ("S1409604444")) {
    return;
  }

  //Set Initialized bit and wait for done
  for (ChId = 0; ChId < MaxChPerMc; ChId++) {
    Ch = (McId * MaxChPerMc) + ChId;
    if ((*ChannelNvList) [Ch].enabled == 0) {
      continue;
    }
    if (!MeeCmiInitRequired (Socket, Ch)) {
      return;
    }
    WaitForTmeCmiCfgCompleteTarget (Socket, Ch);
  }
}

/**
  Enable PRMRR Security

  @param[in] Socket       - Socket number
  @param[in] ChId         - Channel Number

  @retval N/A
**/
VOID
EFIAPI
EnablePrmrrSecurity (
  IN UINT8 Socket,
  IN UINT8 ChId
  )
{
  PRMRR_SMEE_TME_STRUCT            PrmrrSmeeTme;

  if (IsSiliconWorkaroundEnabled ("S1409604444")) {
    return;
  }

  PrmrrSmeeTme.Data = UsraCsrRead (Socket, ChId, PRMRR_SMEE_TME_REG);
  PrmrrSmeeTme.Bits.secured = 1;
  UsraCsrWrite (Socket, ChId, PRMRR_SMEE_TME_REG, PrmrrSmeeTme.Data);
}

/**
  Program TME CMI registers

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number
  @param[in] ProgramableValue  - Programmable CMI credit value

  @retval N/A
**/
VOID
EFIAPI
ProgramTmeCmiRegisters (
  IN UINT8   Socket,
  IN UINT8   Ch,
  IN UINT8   ProgramableValue
  )
{
  UINT32                     RegData = 0;
  UINT8                      IndexVal = 0;
  TME_CMI_N0_SMEE_TME_STRUCT TmeCmiCfgRegValue;
  TME_SMEE_TME_STRUCT        TmeCfgRegValue;
  TME_RDB_N0_SMEE_TME_STRUCT TmeRdbCfgRegValue;
  TME_RDB_N1_SMEE_TME_STRUCT TmeRdbCfgRegValue1;
  CMI_DEFAULT_REG_VALUE      *McTmeCmiCfgValue = NULL;
  UINT8                      McTmeCmiCfgLen = 0;
  CMI_DEFAULT_REG_VALUE      *McTmeRdbCfgValue = NULL;
  UINT8                      McTmeRdbCfgLen = 0;
  CMI_DEFAULT_REG_VALUE      *McTmeRdbCfgValue1 = NULL;
  UINT8                      McTmeRdbCfgValue1Len = 0;
  CMI_DEFAULT_REG_VALUE      *McTmeCfgValue = NULL;
  UINT8                      McTmeCfgLen = 0;
  SYS_SETUP                  *Setup;

  PrintMcTmeCmiDataVersion ();
  GetMcTmeStructures (Socket, Ch, &McTmeCmiCfgValue, &McTmeCmiCfgLen,
                      &McTmeRdbCfgValue, &McTmeRdbCfgLen,
                      &McTmeRdbCfgValue1, &McTmeRdbCfgValue1Len,
                      &McTmeCfgValue, &McTmeCfgLen);

  //tme_cmi_cfg starts
  if (McTmeCmiCfgValue != NULL) {
    TmeCmiCfgRegValue.Data = MemReadPciCfgEp (Socket, Ch, TME_CMI_N0_SMEE_TME_REG);
    for (IndexVal = 0; IndexVal < McTmeCmiCfgLen; IndexVal++) {
      RegData = (TmeCmiCfgRegValue.Data >> McTmeCmiCfgValue[IndexVal].BitStart) & McTmeCmiCfgValue[IndexVal].AndVal;
      TmeCmiCfgRegValue.Data &= ~(McTmeCmiCfgValue[IndexVal].AndVal << McTmeCmiCfgValue[IndexVal].BitStart);

      GetMcTmeStructureField (ProgramableValue, McTmeCmiCfgValue[IndexVal], &RegData);

      TmeCmiCfgRegValue.Data |= (RegData << McTmeCmiCfgValue[IndexVal].BitStart);
    }

    if (IsSiliconWorkaroundEnabled ("S1409718810")) {
      Setup = GetSysSetupPointer ();
      if (Setup->mem.dfxMemSetup.DfxTmeCmiCplFabCreditWa == TME_CMI_CPL_FAB_CREDIT_WORKAROUND_ENABLE) {
        if (IsTmeEnabled (Socket) || IsSgxEnabled ()) {
          TmeCmiCfgRegValue.Bits.rd_cpl_fab_input_credits = 7;
        }
      }
    }
    MemWritePciCfgEp (Socket, Ch, TME_CMI_N0_SMEE_TME_REG, TmeCmiCfgRegValue.Data);
  }
  //tme_cmi_cfg ends
  //tme_rdb_n0 starts
  if (McTmeRdbCfgValue != NULL) {
    TmeRdbCfgRegValue.Data = MemReadPciCfgEp (Socket, Ch, TME_RDB_N0_SMEE_TME_REG);
    for (IndexVal = 0; IndexVal < McTmeRdbCfgLen; IndexVal++) {
      RegData = (TmeRdbCfgRegValue.Data >> McTmeRdbCfgValue[IndexVal].BitStart) & McTmeRdbCfgValue[IndexVal].AndVal;
      TmeRdbCfgRegValue.Data &= ~(McTmeRdbCfgValue[IndexVal].AndVal << McTmeRdbCfgValue[IndexVal].BitStart);

      GetMcTmeStructureField (ProgramableValue, McTmeRdbCfgValue[IndexVal], &RegData);

      TmeRdbCfgRegValue.Data |= (RegData << McTmeRdbCfgValue[IndexVal].BitStart);
    }
    MemWritePciCfgEp (Socket, Ch, TME_RDB_N0_SMEE_TME_REG, TmeRdbCfgRegValue.Data);
  }
  //tme_rdb_n0 ends
  //tme_rdb_n1 starts
  if (McTmeRdbCfgValue1 != NULL) {
    TmeRdbCfgRegValue1.Data = MemReadPciCfgEp (Socket, Ch, TME_RDB_N1_SMEE_TME_REG);
    for (IndexVal = 0; IndexVal < McTmeRdbCfgValue1Len; IndexVal++) {
      RegData = (TmeRdbCfgRegValue1.Data >> McTmeRdbCfgValue1[IndexVal].BitStart) & McTmeRdbCfgValue1[IndexVal].AndVal;
      TmeRdbCfgRegValue1.Data &= ~(McTmeRdbCfgValue1[IndexVal].AndVal << McTmeRdbCfgValue1[IndexVal].BitStart);

      GetMcTmeStructureField (ProgramableValue, McTmeRdbCfgValue1[IndexVal], &RegData);

      TmeRdbCfgRegValue1.Data |= (RegData << McTmeRdbCfgValue1[IndexVal].BitStart);
    }
    MemWritePciCfgEp (Socket, Ch, TME_RDB_N1_SMEE_TME_REG, TmeRdbCfgRegValue1.Data);
  }
  //tme_rdb_n1 ends

  ProgramMcTmeCmiQdepthRegisters (Socket, Ch, ProgramableValue);

  //tme_cfg starts
  if (McTmeCfgValue != NULL) {
    TmeCfgRegValue.Data = MemReadPciCfgEp (Socket, Ch, TME_SMEE_TME_REG);
    for (IndexVal = 0; IndexVal < McTmeCfgLen; IndexVal++) {
      RegData = (TmeCfgRegValue.Data >> McTmeCfgValue[IndexVal].BitStart) & McTmeCfgValue[IndexVal].AndVal;
      TmeCfgRegValue.Data &= ~(McTmeCfgValue[IndexVal].AndVal << McTmeCfgValue[IndexVal].BitStart);

      GetMcTmeStructureField (ProgramableValue, McTmeCfgValue[IndexVal], &RegData);

      TmeCfgRegValue.Data |= (RegData << McTmeCfgValue[IndexVal].BitStart);
    }
    MemWritePciCfgEp (Socket, Ch, TME_SMEE_TME_REG, TmeCfgRegValue.Data);
  }

  if (IsSiliconWorkaroundEnabled ("S1707233823")) {
    DisableClkGating (Socket, Ch);
  }
  //tme_cfg ends
}
