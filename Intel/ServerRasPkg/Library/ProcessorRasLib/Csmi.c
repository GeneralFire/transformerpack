/** @file
  Implementation of MCE function for processor RAS lib.

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

#include "ProcessorRasLibInternal.h"
#include <Library/MemMcIpRasLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/SmmServicesTableLib.h>
#include <UncoreCommonIncludes.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/SiliconWorkaroundLib.h>

typedef union {
  struct {
    UINT32  Vector:8;          ///< The vector number of the interrupt being sent.
    UINT32  DeliveryMode:3;
    UINT32  Reserved0:1;       ///< Reserved.
    UINT32  DeliveryStatus:1;  ///< 0: Idle, 1: send pending.
    UINT32  Reserved1:3;       ///< Reserved.
    UINT32  Mask:1;            ///< 0: Not masked, 1: Masked.
    UINT32  Reserved2:14;      ///< Reserved.
  } Bits;
  UINT32    Uint32;
} LOCAL_APIC_LVT_CMCI;

/**
  This function enables the SMI generation for Corrected errors in eMCA mode

  @param[in] Socket   The socket number

  @retval None.

**/
VOID
EnableEmcaSmiForCorrectedErrors (
  IN  UINT8 Socket,
  IN  UINT8 Mc
  )
{
  EXRAS_CONFIG_SMI_EN Enables;

  ZeroMem ((VOID*)(UINTN)&Enables, sizeof (EXRAS_CONFIG_SMI_EN));
  Enables.CfgMcaCmciOnFailover      = TRUE;
  Enables.CfgMcaCmciOnMirrorcorr    = TRUE;
  Enables.CfgMcaSmiOnMirrorcorr     = FALSE;
  Enables.CfgMcaCmciOnCorrCountthr  = FALSE;
  Enables.CfgMcaSmiOnCorrCountthr   = FALSE;
  Enables.CfgMcaCmciOnCorr          = TRUE;
  Enables.CfgMcaSmiOnCorr           = TRUE;
  Enables.CfgMcaCmciOnRecov         = TRUE;
  Enables.CfgMcaSmiOnRecov          = FALSE;

  SetM2mExRasConfigSmiEn (Socket, Mc, &Enables);
  RAS_DEBUG ((LEVEL_REG, "[CpuRas]EnableEmcaSmiForCorrectedErrors: CSR (S 0x%x I 0x%x ) = 0x%x\n",
              Socket, Mc, GetM2mExRasConfig (Socket, Mc)));

  return;
}


/**

  Enables SMI for corrected error in EMCA mode

**/
VOID
EFIAPI
EnableEmcaSmi (
  VOID
  )
{
  UINT8             Socket;
  UINT8             Mc;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (IsSocketPresent (Socket) == FALSE) {
      continue;
    }
    for (Mc = 0; Mc < MAX_IMC; Mc++) {
      if (IsNodePresent (SKTMC_TO_NODE (Socket, Mc))) {
        RAS_DEBUG ((LEVEL_FUNC_FLOW, "[CpuRas]EnableEmcaSmi for socket 0x%x, Mc 0x%x\n", Socket, Mc));
        EnableEmcaSmiForCorrectedErrors (Socket, Mc);
      }
    }
  }
}

/**
  Enable CMCI->SMI morphing for all the banks

  @param[in]  MsmiBankBitFieldEn        The bit mask of banks to enable

**/
VOID
EFIAPI
ApEnableCsmiGen2 (
  IN MC_ENABLE_INFO  *McEnableInfo
  )
{
  UINT32 McBankNum;
  UINT32 McBankSupp;
  UINT32 Index;
  UINT32 MsrAddr;
  UINT64 Value;
  UINT16 McBankScope;

  McBankNum = GetNumMcBanksPerLp ();

  McBankSupp = GetEmcaBankSupp () & McEnableInfo->McBankBitFieldEn;

  for (Index = 0; Index < McBankNum; Index++) {
    if (((McBankSupp >> Index) & 0x1) != 0) {
      McBankScope = GetMcBankScope (Index);
      if (McBankScope == McEnableInfo->McScope) {
        MsrAddr = GetAddrMcBankCtl2 (Index);
        if (MsrAddr == MSR_IA32_INVALID) {
          RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "[CpuRas]MsrAddr == MSR_IA32_INVALID\n"));
          RAS_ASSERT (FALSE);
          return;
        }
        Value = AsmMsrOr64(MsrAddr, BIT32);    //CMCI to SMI CTL
        RAS_DEBUG ((MP_PRINT | LEVEL_REG, "[CpuRas]Enable EMCA2 Csmi for bank 0x%x MSR(0x%x) = 0x%lx\n", Index, MsrAddr, Value));
      }
    }
  }
}

/**
  Configure CSMI threshold

  @param[in]  CsmiThreshold    CSMI threshold setting
  @param[in]  CloakingEn       Indicates whether cloaking is enabled or not

**/
VOID
EFIAPI
ConfigureCsmiThreshold (
  IN UINT16    CsmiThreshold,
  IN BOOLEAN   CloakingEn
  )
{
  BOOLEAN   Is14nm;

  Is14nm = IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL);

  if (Is14nm || IsCpuAndRevision (CPU_ICXSP , REV_ALL)) {
    SetKtiCsmiThreshold (CsmiThreshold, CloakingEn);
  }

  if (IsSiliconWorkaroundEnabled ("S1507064340")) {
    return;
  }
  SetUboxCsmiThreshold (CsmiThreshold, CloakingEn);
}

/**
  Enables Csmi Gen2 for all APs

  @param[in] CsmiBankBitFieldEn    Bitmaks for the banks to enable Csmi Gen2
  @param[in] CsmiThreshold         CSMI threshold setting
  @param[in] CloakingEn            Indicates whether cloaking is enabled or not

**/
VOID
EFIAPI
EnableCsmiGen2 (
  IN UINT32    CsmiBankBitFieldEn,
  IN UINT16    CsmiThreshold,
  IN BOOLEAN   CloakingEn
  )
{
  MC_ENABLE_INFO  McEnableInfo;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "[CpuRas] Enable EMCA2 CSMI bank bit mask field 0x%x, CSMI threshold 0x%x\n", CsmiBankBitFieldEn, CsmiThreshold));
  ConfigureCsmiThreshold (CsmiThreshold, CloakingEn);

  //
  // Enable CSMI according to MC bank scope
  //
  McEnableInfo.McBankBitFieldEn = CsmiBankBitFieldEn;
  McEnableInfo.McScope = MC_SCOPE_TD;
  MultiProcessorExecute (AllAps, ALL_PACKAGE, ALL_CORE, FALSE, ApEnableCsmiGen2, &McEnableInfo);

  McEnableInfo.McScope = MC_SCOPE_CORE;
  MultiProcessorExecute (OneApPerCore, ALL_PACKAGE, ALL_CORE, FALSE, ApEnableCsmiGen2, &McEnableInfo);

  McEnableInfo.McScope = MC_SCOPE_PKG;
  MultiProcessorExecute (OneApPerPackage, ALL_PACKAGE, ALL_CORE, FALSE, ApEnableCsmiGen2, &McEnableInfo);
  EnableEmcaSmi ();
}

/**

    Enable emca gen 1 CSMI morphing

    @param node - NodeId System Wide

    @retval None

**/
VOID
EnableMcCsmiGen1 (
  IN UINT8        Socket,
  IN UINT8        Mc
  )
{
  SetM2mExRasConfigMcaSmiOnCorr (Socket, Mc, TRUE);

  RAS_DEBUG ((LEVEL_REG, "[CpuRas]EnableMcCsmiGen1: CSR (S 0x%x I 0x%x) = 0x%x\n",
              Socket, Mc, GetM2mExRasConfig (Socket, Mc)));

  EnableMcChanCsmiGen1 (Socket, Mc);
}

/**

  Enables Csmi Gen1.

**/
VOID
EFIAPI
EnableCsmiGen1 (
  VOID
  )
{
  UINT8             Socket;
  UINT8             Mc;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (IsSocketPresent (Socket) == FALSE) {
      continue;
    }
    for (Mc = 0; Mc < MAX_IMC; Mc++) {
      if (IsNodePresent (SKTMC_TO_NODE (Socket, Mc))) {
        RAS_DEBUG ((LEVEL_FUNC_FLOW, "[CpuRas]EnableCsmiGen1 for socket 0x%x, Mc 0x%x\n", Socket, Mc));
        EnableMcCsmiGen1 (Socket, Mc);
      }
    }
  }
}

/**
  Read Local APIC Reg (copy)

  This function reads a Local Apic Register


  @param[in] MmioOffset  Offset to read

  @retval UINT32         Register contents

**/

UINT32
EFIAPI
ReadLocalApicRegrep (
  IN      UINTN  MmioOffset
  )
{
  UINT32    MsrIndex;
  UINT32    Value;
  UINTN     Addr;

  ASSERT ((MmioOffset & 0xf) == 0);

  if (GetApicMode () == LOCAL_APIC_MODE_XAPIC) {
    Addr = PcdGet32 (PcdCpuLocalApicBaseAddress) + MmioOffset;
    Value = MmioRead32 (Addr);
    RAS_DEBUG ((MP_PRINT | LEVEL_REG, "[CpuRas]Read XAPIC mode local APIC register MMIO(0x%lx) = 0x%x\n", (UINT64) Addr, Value));
  } else {
    //
    // DFR is not supported in x2APIC mode.
    //
    ASSERT (MmioOffset != XAPIC_ICR_DFR_OFFSET);
    //
    // Note that in x2APIC mode, ICR is a 64-bit MSR that needs special treatment. It
    // is not supported in this function for simplicity.
    //
    ASSERT (MmioOffset != XAPIC_ICR_HIGH_OFFSET);

    MsrIndex = (UINT32) (MmioOffset >> 4) + X2APIC_MSR_BASE_ADDRESS;
    Value = AsmReadMsr32 (MsrIndex);
    RAS_DEBUG ((MP_PRINT | LEVEL_REG, "[CpuRas]Read X2APIC mode MSR(0x%x) = 0x%x\n", MsrIndex, Value));
  }

  return Value;
}

VOID
EFIAPI
ApSignalCmci (
  IN UINT32             *BankIndex
  )
{
  UINT32                      ApicId;
  LOCAL_APIC_LVT_CMCI         CmciLvt;
  UINT32                      CmciVec;
  UINT32                      MsrIdx;
  IA32_MCI_CTL2_MSR_STRUCT    Ctl2;

  ApicId = GetApicId ();

  // Find if CMCI enabled
  MsrIdx = GetAddrMcBankCtl2 (*BankIndex);
  if (MsrIdx == MSR_IA32_INVALID) {
    RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "[CpuRas]MsrIdx == MSR_IA32_INVALID\n"));
    RAS_ASSERT (FALSE);
    return;
  }
  Ctl2.Data = AsmReadMsr64 (MsrIdx);
  RAS_DEBUG ((MP_PRINT | LEVEL_REG, "[CpuRas]Read Machine Check Control2 MSR(0x%x) = 0x%x\n", MsrIdx, Ctl2.Data));
  if (Ctl2.Bits.CMCI_EN) {
    CmciLvt.Uint32 = ReadLocalApicRegrep ((MSR_IA32_X2APIC_LVT_CMCI - X2APIC_MSR_BASE_ADDRESS) << 4);
    if (!CmciLvt.Bits.Mask) {
      CmciVec = CmciLvt.Bits.Vector;
      RAS_DEBUG ((MP_PRINT | LEVEL_REG, "[CpuRas]SendFixedIpi ApicId = 0x%x, CmciVec = 0x%x\n", ApicId, CmciVec));
      SendFixedIpi (ApicId,(UINT8) CmciVec);
      RAS_DEBUG ((MP_PRINT | LEVEL_BASIC_FLOW, "[CpuRas]Successfully notify OS via CMCI\n"));
    } else {
      RAS_DEBUG ((MP_PRINT | LEVEL_BASIC_FLOW, "[CpuRas]CMCI is masked, signal CMCI failed\n"));
    }
  } else {
    RAS_DEBUG ((MP_PRINT | LEVEL_BASIC_FLOW, "[CpuRas]CMCI is disabled, signal CMCI failed\n"));
  }
  return;
}

/**
  Signal a CMCI to the OS

  This function will signal an CMCI to the OS

  @param[in] BankIndex  Bank Index of the CPU.
  @param[in] Package    The package number of the CPU.
  @param[in] Core       The Core number of the CPU.

**/
VOID
EFIAPI
SignalCmciToOs (
  IN  UINT32  BankIndex,
  IN  UINT32  Package,
  IN  UINT32  Core,
  IN  UINTN   *CpuIndex OPTIONAL
  )
{
  UINT16              CmciScope;

  CmciScope = GetMcBankScope (BankIndex);

  if ((CmciScope == MC_SCOPE_TD) && (CpuIndex != NULL)) {
    MultiProcessorStartupThisThread (*CpuIndex, ApSignalCmci, &BankIndex);
  } else if (CmciScope == MC_SCOPE_CORE){
    MultiProcessorExecute (AllAps, Package, Core, FALSE, ApSignalCmci, &BankIndex);
  } else if (CmciScope == MC_SCOPE_PKG) {
    MultiProcessorExecute (AllAps, Package, ALL_CORE, FALSE, ApSignalCmci, &BankIndex);
  } else {
    // shall never happen
    ASSERT (FALSE);
    return;
  }

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "[CpuRas]SignalCmciToOs: Bank = 0x%x Scope = 0x%x Package = 0x%x Core = 0x%x\n",
              BankIndex, CmciScope, Package, Core));
}



