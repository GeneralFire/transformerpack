/** @file

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

#include "Include/ChaRegs.h"
#include "Include/ChaMemDecodePrivate.h"
#include <MemDecodeCommonIncludes.h>
#include <Library/UsraCsrLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/RcDebugLib.h>
#include <Library/KtiApi.h>

//
// Convert a UNICAST CHA MMIO address to FZM domain handle that BIOS fills in the FZM ACPI table
// OS will use the "DomainHandle << 8 | region ID" to write MSR_FZMRANGE_INDEX:
// Bit [7:0]  - Region_ID. Region ID can be 0 - 63.  CONFIG is 64.
// Bits[63:8] - Domain Handle.
//
#define ADDR_TO_DOMAIN_HANDLE(Addr) (((Addr) & ~0xFFF) >> 8)

UINT32 mDramRuleCfg2ChaRegs[MAX_SAD_RULES] = {
  DRAM_RULE_CFG2_0_CHABC_SAD_REG,  DRAM_RULE_CFG2_1_CHABC_SAD_REG,  DRAM_RULE_CFG2_2_CHABC_SAD_REG,
  DRAM_RULE_CFG2_3_CHABC_SAD_REG,  DRAM_RULE_CFG2_4_CHABC_SAD_REG,  DRAM_RULE_CFG2_5_CHABC_SAD_REG,
  DRAM_RULE_CFG2_6_CHABC_SAD_REG,  DRAM_RULE_CFG2_7_CHABC_SAD_REG,  DRAM_RULE_CFG2_8_CHABC_SAD_REG,
  DRAM_RULE_CFG2_9_CHABC_SAD_REG,  DRAM_RULE_CFG2_10_CHABC_SAD_REG, DRAM_RULE_CFG2_11_CHABC_SAD_REG,
  DRAM_RULE_CFG2_12_CHABC_SAD_REG, DRAM_RULE_CFG2_13_CHABC_SAD_REG, DRAM_RULE_CFG2_14_CHABC_SAD_REG,
  DRAM_RULE_CFG2_15_CHABC_SAD_REG
};


UINT32 mRemoteDramRuleCfgN0[MAX_REMOTE_SAD_RULES] = {
  REMOTE_DRAM_RULE_CFG_0_N0_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_1_N0_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_2_N0_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_3_N0_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_4_N0_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_5_N0_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_6_N0_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_7_N0_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_8_N0_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_9_N0_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_10_N0_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_11_N0_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_12_N0_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_13_N0_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_14_N0_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_15_N0_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_16_N0_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_17_N0_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_18_N0_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_19_N0_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_20_N0_CHABC_SAD_REG
};

UINT32 mRemoteDramRuleCfgN1[MAX_REMOTE_SAD_RULES] = {
  REMOTE_DRAM_RULE_CFG_0_N1_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_1_N1_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_2_N1_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_3_N1_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_4_N1_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_5_N1_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_6_N1_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_7_N1_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_8_N1_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_9_N1_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_10_N1_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_11_N1_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_12_N1_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_13_N1_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_14_N1_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_15_N1_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_16_N1_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_17_N1_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_18_N1_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_19_N1_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_20_N1_CHABC_SAD_REG
};


UINT32 mDramRtModeOffset[][MaxRtHalves][MAX_MODE_TABLES_IMC] = {
  {
    {DRAM_H0_RT0_MODE0_CHABC_SAD_REG, DRAM_H0_RT0_MODE1_CHABC_SAD_REG},
    {DRAM_H1_RT0_MODE0_CHABC_SAD_REG, DRAM_H1_RT0_MODE1_CHABC_SAD_REG}
  },
  {
    {DRAM_H0_RT1_MODE0_CHABC_SAD_REG, DRAM_H0_RT1_MODE1_CHABC_SAD_REG},
    {DRAM_H1_RT1_MODE0_CHABC_SAD_REG, DRAM_H1_RT1_MODE1_CHABC_SAD_REG}
  },
  {
    {DRAM_H0_RT2_MODE0_CHABC_SAD_REG, DRAM_H0_RT2_MODE1_CHABC_SAD_REG},
    {DRAM_H1_RT2_MODE0_CHABC_SAD_REG, DRAM_H1_RT2_MODE1_CHABC_SAD_REG}
  }
};


/**

  For SPR, programs the durable_mem_en bit of the CHA DRAM_RULE_CFG2 registers for each DRAM rule.
  For ICX, it is an empty function.

  @param[in] SocketId                The socket number ID and 0 should always be the SBSP.
  @param[in] SadIndex                The index of SAD entry.
  @param[in] SadData                 Logic information for a SAD entry.

  @retval None

**/
VOID
ChaSetSadDataEx (
  IN UINT8     ScktId,
  IN UINT8     SadId,
  IN SAD_DATA  *SadData
  )
{
  DRAM_RULE_CFG2_0_CHABC_SAD_STRUCT DramRuleCfg2;

  DramRuleCfg2.Data = UsraCsrRead (ScktId, 0, mDramRuleCfg2ChaRegs[SadId]);
  DramRuleCfg2.Bits.durable_mem_en      = SadData->DurableMem;
  DramRuleCfg2.Bits.nxm                 = SadData->Nxm;
  DramRuleCfg2.Bits.cxl_accelerator_mem = SadData->CxlAccMem;
  DramRuleCfg2.Bits.cxl_mem_expander    = SadData->CxlExpMem;

  RcDebugPrint (SDBG_MAX, "    DRAM_RULE_CFG2: cxl_accelerator_mem    Durable_mem_en    cxl_mem_expander    NXM \n");
  RcDebugPrint (SDBG_MAX, "                    %d                      %d                 %d                   %d \n",
    DramRuleCfg2.Bits.cxl_accelerator_mem,
    DramRuleCfg2.Bits.durable_mem_en,
    DramRuleCfg2.Bits.cxl_mem_expander,
    DramRuleCfg2.Bits.nxm
    );

  UsraCsrWrite (ScktId, 0, mDramRuleCfg2ChaRegs[SadId], DramRuleCfg2.Data);
}


/**
  Programs route table mode registers for all SAD rules.

  @param[in] SocketId                The socket number ID and 0 should always be the SBSP.
  @param[in] TypeInfo                The bit structure to specify the exact route table mode registers.
  @param[in] EncRtMode               Input array of route table modes and its size is maximum SAD rule number.

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaSetRtMode (
  IN UINT8            SocketId,
  IN MODE_TYPE_INFO   *TypeInfo,
  IN UINT8            EncRtMode[]
  )
{
  UINT32              Mode[MAX_MODE_TABLES_IMC];
  UINTN               SadIndex;
  UINTN               RegIndex;

  if (TypeInfo == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (TypeInfo->RtHalf >= MaxRtHalves) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&Mode, sizeof (Mode));
  switch (TypeInfo->RtType) {
  case ModeTypeRt0:
  case ModeTypeRt1:
  case ModeTypeRt2:
    if (TypeInfo->RtHalf >= ARRAY_SIZE (mDramRtModeOffset[TypeInfo->RtType])) {
      return EFI_INVALID_PARAMETER;
    }
    for (SadIndex = 0; SadIndex < MAX_SAD_RULES; SadIndex++) {
      if ((SadIndex / 10) < MAX_MODE_TABLES_IMC) {
        Mode[SadIndex / 10] |= EncRtMode[SadIndex] << ((SadIndex % 10) * 3);
      }
    }
    for (RegIndex = 0; RegIndex < MAX_MODE_TABLES_IMC; RegIndex++) {
      UsraCsrWrite (
        SocketId,
        0,
        mDramRtModeOffset[TypeInfo->RtType][TypeInfo->RtHalf][RegIndex],
        Mode[RegIndex]
      );
      RcDebugPrint (SDBG_MAX, "    H%d DramRt%dMode%dCfg: %08x\n",
        TypeInfo->RtHalf, TypeInfo->RtType, RegIndex, Mode[RegIndex]
        );
    }
    break;
  default:
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

/**
  Programs CHA remote DRAM rule.

  @param SocketId                The socket number ID and 0 should always be the SBSP.
  @param SadIndex                The index of SAD entry.
  @param RemoteSadData           Logic information for a SAD entry.

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.

  **/
EFI_STATUS
EFIAPI
ChaSetRemoteSadRule (
  IN UINT8           SocketId,
  IN UINT8           SadIndex,
  IN REMOTE_SAD_DATA *RemoteSadData
  )
{
  REMOTE_DRAM_RULE_CFG_0_N0_CHABC_SAD_STRUCT RemoteDramRuleN0;
  REMOTE_DRAM_RULE_CFG_0_N1_CHABC_SAD_STRUCT RemoteDramRuleN1;

  if ((RemoteSadData == NULL) || (SadIndex >= MAX_REMOTE_SAD_RULES)) {
    return EFI_INVALID_PARAMETER;
  }

  RemoteDramRuleN0.Data = 0;
  RemoteDramRuleN1.Data = 0;

  RemoteDramRuleN0.Bits.rule_enable         = RemoteSadData->Enabled;
  RemoteDramRuleN0.Bits.cxl_accelerator_mem = RemoteSadData->CxlMemEnabled;
  RemoteDramRuleN0.Bits.attr                = RemoteSadData->Attribute;
  RemoteDramRuleN0.Bits.base                = RemoteSadData->Base;
  RemoteDramRuleN1.Bits.pkg                 = RemoteSadData->SocketId;
  RemoteDramRuleN1.Bits.limit               = RemoteSadData->Limit;


  //
  // Update Remote DRAM CHA CSR
  //
  UsraCsrWrite (
    SocketId,
    0,
    mRemoteDramRuleCfgN0[SadIndex],
    RemoteDramRuleN0.Data
  );

  UsraCsrWrite (
    SocketId,
    0,
    mRemoteDramRuleCfgN1[SadIndex],
    RemoteDramRuleN1.Data
  );

  return EFI_SUCCESS;
}

/**

  Where SNC is enabled, each SNC cluster shall have its own independent CHA DRAM decoder CSRs.
  BIOS Unlocks ClusterId and locks other clusters. ClusterId is only valid in SNC mode.
  For All2All and UBC mode, all clusters will be unlocked.

  @param[in] SocketId                The socket ID and 0 should always be the SBSP.
  @param[in] ClusterId               The index of cluster to unlock.

  @retval EFI_SUCCESS               The function executed successfully without any error.

**/
EFI_STATUS
EFIAPI
ChaUnlockDramCluster (
  IN UINT8     ScktId,
  IN UINT8     ClusterId
  )
{
  SNC_LOCK_CHABC_SAD_STRUCT  SncLock;

  SncLock.Data = UsraCsrRead (ScktId, 0, SNC_LOCK_CHABC_SAD_REG);

  if (IfFeatureEnabled (TypeSnc)) {
    SncLock.Bits.snc_lock = 0xF & ~(BIT0 << ClusterId); // Unlock only ClusterId
  } else {
    SncLock.Bits.snc_lock = 0;                          // Unlock all clusters
  }

  RcDebugPrint (SDBG_MAX, "\n  CHA unlock DRAM cluster %d snc_lock bits: 0x%x\n",
    ClusterId,
    SncLock.Bits.snc_lock
    );

  UsraCsrWrite (ScktId, 0, SNC_LOCK_CHABC_SAD_REG, SncLock.Data);

  return EFI_SUCCESS;
}

/**

  Where SNC is enabled, each SNC cluster shall have its own independent CHA DRAM decoder CSRs.
  BIOS locks all clusters here.

  @param[in] SocketId                The socket ID and 0 should always be the SBSP.

  @retval EFI_SUCCESS                The function executed successfully without any error.

**/
EFI_STATUS
EFIAPI
ChaLockDramClusters (
  IN UINT8     ScktId
  )
{

  SNC_LOCK_CHABC_SAD_STRUCT  SncLock;

  SncLock.Data = UsraCsrRead (ScktId, 0, SNC_LOCK_CHABC_SAD_REG);
  SncLock.Bits.snc_lock = 0xF; // Lock all clusters

  RcDebugPrint (SDBG_MAX, "\n  CHA lock all DRAM clusters snc_lock bits: 0x%x\n",
    SncLock.Bits.snc_lock
    );

  UsraCsrWrite (ScktId, 0, SNC_LOCK_CHABC_SAD_REG, SncLock.Data);

  return EFI_SUCCESS;
}


/**
  Get Domain handle for a cluster

  @param[in] SocketId                 The socket ID and 0 should always be the SBSP.
  @param[in] ClusterId                The index of cluster to get domain handle.
  @param[out] DomainHandle            Pointer to the output domain handle.

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaGetClusterDomainHandle (
  IN  UINT8   SocketId,
  IN  UINT8   ClusterId,
  OUT UINT64  *DomainHandle
  )
{
  UINT8       TotalClusters;
  UINT8       BoxInst;
  UINTN       Address;

  if ((SocketId >= MAX_SOCKET) || (DomainHandle == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (IfFeatureEnabled (TypeSnc)) {
    TotalClusters = GetNumOfClusterPerSystem ();
  } else {
    TotalClusters = 1;
  }

  if (ClusterId >= TotalClusters) {
    return EFI_INVALID_PARAMETER;
  }

  BoxInst = GetTotChaCount (SocketId) * ClusterId / TotalClusters;
  //
  // We use USRA API to get the MMIO address FZM_BITS_CHA_MISC_REG for the base address
  // of UNICAST group.
  //
  Address = UsraGetCsrRegisterAddress (SocketId, BoxInst, FZM_BITS_CHA_MISC_REG);
  *DomainHandle = ADDR_TO_DOMAIN_HANDLE (Address);

  RcDebugPrint (SDBG_MAX, "Socket %d Cluster %d: DomainHandle 0x%lx\n", SocketId, ClusterId, *DomainHandle);

  return EFI_SUCCESS;
}


/**
Programs CHA to disable 2LM local WCiLF optimization and disable 2LM remote
WCiLF/PCIe write optimization when Directory Mode is disabled.

@param SocketId                The socket number ID and 0 should always be the SBSP.
@param DirDisable              Directory Mode enabled or disabled.

@retval EFI_SUCCESS            The function is executed successfully without any error.
@retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.

**/
EFI_STATUS
EFIAPI
ChaDisable2lmOptimization (
  IN UINT8    SocketId,
  IN BOOLEAN  DirDisable
  )
{
  return EFI_SUCCESS;
}

/**
  This function programs the FZM_CAPABILITY register to enumerate memory regions that are fast-zero capable.

  @param[in] SocketId             Socket Index.
  @param[in] CapabilityVector     Vector indicates which memory regions are fast-zero capable.
                                  One bit per general- purpose DRAM rule.

  @retval EFI_SUCCESS             This function is executed successfully.
  @retval EFI_UNSUPPORTED         This function is not supported by the library instance.
**/
EFI_STATUS
EFIAPI
ChaSetFzmCapability (
  IN UINT8    SocketId,
  IN UINT16   CapabilityVector
  )
{
  FZM_CAPABILITY_CHABC_SAD_STRUCT  FzmCapabilityReg;

  FzmCapabilityReg.Data = 0;
  FzmCapabilityReg.Bits.capability_vector = CapabilityVector;
  UsraCsrWrite (SocketId, 0, FZM_CAPABILITY_CHABC_SAD_REG, FzmCapabilityReg.Data);

  return EFI_SUCCESS;
}

/**
  This function enables or disables biased 2 way near memory cache in CHA.

  @param[in] ScktId               Socket index.
  @param[in] ChaId                CHA index on socket.
  @param[in] IsTwoWayNmCacheEn    Flag indicates if biased 2 way NM cache needs to be enabled.

  @retval EFI_SUCCESS             This function is executed successfully.
  @retval EFI_UNSUPPORTED         This function is not supported by the library instance.
**/
EFI_STATUS
EFIAPI
ChaSetTwoWayNmCache (
  IN UINT8    ScktId,
  IN UINT8    ChaId,
  IN BOOLEAN  IsTwoWayNmCacheEn
  )
{
  CBO_COH_CONFIG_CHA_PMA_STRUCT   CboCohCfgReg;

  CboCohCfgReg.Data = UsraCsrRead (ScktId, ChaId, CBO_COH_CONFIG_CHA_PMA_REG);
  CboCohCfgReg.Bits.disablenm2waybias = !IsTwoWayNmCacheEn;
  UsraCsrWrite (ScktId, ChaId, CBO_COH_CONFIG_CHA_PMA_REG, CboCohCfgReg.Data);

  return EFI_SUCCESS;
}

/**
  This function sets the disable_loc_meminvitox bit field of HA_COH_CHABC_SAD1_STRUCT register.

  @param[in] SocketId            Socket index.
  @param[in] LocMemInvItoXDis    Flag indicates if local MemInvItoX needs to be disabled.

**/
VOID
EFIAPI
ChaSetLocMemInvItoX (
  IN UINT8     SocketId,
  IN BOOLEAN   LocMemInvItoXDis
  )
{
  HA_COH_CHABC_SAD1_STRUCT      HaCohReg;

  if (SocketId >= MAX_SOCKET) {
    return;
  }

  HaCohReg.Data = UsraCsrRead (SocketId, 0, HA_COH_CHABC_SAD1_REG);
  HaCohReg.Bits.disable_loc_meminvitox = LocMemInvItoXDis;
  UsraCsrWrite (SocketId, 0, HA_COH_CHABC_SAD1_REG, HaCohReg.Data);
}

/**

  This function sets the DisCleanDirUpdate field of the MKTME_CONFIG_CHA_PMA register.

  @param[in] SocketId                      Socket index.
  @param[in] ChaId                         CHA index within socket.
  @param[in] DisCleanDirUpdate             Data to be written to the field.

**/
VOID
EFIAPI
ChaSetDisCleanDirUpdate (
  IN UINT8    SocketId,
  IN UINT8    ChaId,
  IN UINT8    DisCleanDirUpdate
  )
{
  MKTME_CONFIG_CHA_PMA_STRUCT   MktmeConfigReg;

  MktmeConfigReg.Data = UsraCsrRead (SocketId, ChaId, MKTME_CONFIG_CHA_PMA_REG);
  MktmeConfigReg.Bits.discleandirupdate = DisCleanDirUpdate;
  UsraCsrWrite (SocketId, ChaId, MKTME_CONFIG_CHA_PMA_REG, MktmeConfigReg.Data);
}
