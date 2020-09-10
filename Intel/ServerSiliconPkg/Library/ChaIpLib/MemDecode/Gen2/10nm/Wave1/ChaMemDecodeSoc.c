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

UINT32 mRemoteDramRuleCfgN0[REMOTE_SAD_RULES_10NM_WAVE1] = {
  REMOTE_DRAM_RULE_CFG_0_N0_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_1_N0_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_2_N0_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_3_N0_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_4_N0_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_5_N0_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_6_N0_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_7_N0_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_8_N0_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_9_N0_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_10_N0_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_11_N0_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_12_N0_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_13_N0_CHABC_SAD_REG
};

UINT32 mRemoteDramRuleCfgN1[REMOTE_SAD_RULES_10NM_WAVE1] = {
  REMOTE_DRAM_RULE_CFG_0_N1_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_1_N1_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_2_N1_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_3_N1_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_4_N1_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_5_N1_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_6_N1_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_7_N1_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_8_N1_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_9_N1_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_10_N1_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_11_N1_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_12_N1_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_13_N1_CHABC_SAD_REG
};

UINT32 mDramRt0Rt1ModeOffset[][MaxRtHalves][MAX_MODE_TABLES_IMC] = {
  {
    {DRAM_H0_RT0_MODE0_CHABC_SAD_REG, DRAM_H0_RT0_MODE1_CHABC_SAD_REG},
    {DRAM_H1_RT0_MODE0_CHABC_SAD_REG, DRAM_H1_RT0_MODE1_CHABC_SAD_REG}
  },
  {
    {DRAM_H0_RT1_MODE0_CHABC_SAD_REG, DRAM_H0_RT1_MODE1_CHABC_SAD_REG},
    {DRAM_H1_RT1_MODE0_CHABC_SAD_REG, DRAM_H1_RT1_MODE1_CHABC_SAD_REG}
  }
};

UINT32 mDramRt2ModeOffset[MaxRtHalves] = {
  DRAM_H0_RT2_MODE0_CHABC_SAD_REG,
  DRAM_H1_RT2_MODE0_CHABC_SAD_REG
};


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
    if (TypeInfo->RtHalf >= ARRAY_SIZE (mDramRt0Rt1ModeOffset[TypeInfo->RtType])) {
      return EFI_INVALID_PARAMETER;
    }
    for (SadIndex = 0; SadIndex < SAD_RULES_10NM; SadIndex++) {
      if ((SadIndex / 10) < MAX_MODE_TABLES_IMC) {
        Mode[SadIndex / 10] |= EncRtMode[SadIndex] << ((SadIndex % 10) * 3);
      }
    }
    for (RegIndex = 0; RegIndex < MAX_MODE_TABLES_IMC; RegIndex++) {
      UsraCsrWrite (
        SocketId,
        0,
        mDramRt0Rt1ModeOffset[TypeInfo->RtType][TypeInfo->RtHalf][RegIndex],
        Mode[RegIndex]
      );
      RcDebugPrint (SDBG_MAX, "    H%d DramRt%dMode%dCfg: %08x\n",
        TypeInfo->RtHalf, TypeInfo->RtType, RegIndex, Mode[RegIndex]
        );
    }
    break;
  case ModeTypeRt2:
    if (TypeInfo->RtHalf >= ARRAY_SIZE (mDramRt2ModeOffset)) {
      return EFI_INVALID_PARAMETER;
    }
    for (SadIndex = 0; SadIndex < SAD_RULES_10NM; SadIndex++) {
      Mode[0] |= EncRtMode[SadIndex] << (SadIndex * 2);
    }
    UsraCsrWrite (
      SocketId,
      0,
      mDramRt2ModeOffset[TypeInfo->RtHalf],
      Mode[0]
    );

    RcDebugPrint (SDBG_MAX, "    H%d DramRt2ModeCfg: %08x\n", TypeInfo->RtHalf, Mode[0]);
    break;
  default:
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}


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
  return;
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

  if ((RemoteSadData == NULL) || (SadIndex >= REMOTE_SAD_RULES_10NM_WAVE1)) {
    return EFI_INVALID_PARAMETER;
  }

  RemoteDramRuleN0.Data = 0;
  RemoteDramRuleN1.Data = 0;

  RemoteDramRuleN0.Bits.rule_enable = RemoteSadData->Enabled;
  RemoteDramRuleN0.Bits.attr = RemoteSadData->Attribute;
  RemoteDramRuleN0.Bits.base = RemoteSadData->Base;
  RemoteDramRuleN1.Bits.pkg = RemoteSadData->SocketId;
  RemoteDramRuleN1.Bits.limit = RemoteSadData->Limit;


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

  For 10nm SOCs beyond ICX, Where SNC is enabled each SNC cluster shall have its own independent CHA DRAM decoder CSRs.
  BIOS Unlocks ClusterId and locks other clusters.

  @param[in] SocketId                The socket ID and 0 should always be the SBSP.
  @param[in] ClusterId               The index of cluster to unlock.

  @retval EFI_UNSUPPORTED            The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaUnlockDramCluster (
  IN UINT8     ScktId,
  IN UINT8     ClusterId
  )
{
  return EFI_UNSUPPORTED;
}

/**

  For 10nm SOCs beyond ICX, Where SNC is enabled each SNC cluster shall have its own independent CHA DRAM decoder CSRs.
  BIOS locks all clusters here.

  @param[in] SocketId                The socket ID and 0 should always be the SBSP.

  @retval EFI_SUCCESS                The function executed successfully without any error.
  @retval EFI_UNSUPPORTED            The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaLockDramClusters (
  IN UINT8     ScktId
  )
{
  return EFI_UNSUPPORTED;
}


/**
  Get Domain handle for a cluster

  @param[in] SocketId                 The socket ID and 0 should always be the SBSP.
  @param[in] ClusterId                The index of cluster to get domain handle.
  @param[out] DomainHandle            Pointer to the output domain handle.

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
  return EFI_UNSUPPORTED;
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
  HA_COH_CHABC_SAD1_STRUCT           HaCoh;

  HaCoh.Data = UsraCsrRead (SocketId, 0, HA_COH_CHABC_SAD1_REG);
  if (DirDisable) {
    HaCoh.Bits.disable_loc_meminvitox = 1;
    HaCoh.Bits.disable_rmt_meminvitox = 1;
  } else {
    HaCoh.Bits.disable_loc_meminvitox = 0;
    HaCoh.Bits.disable_rmt_meminvitox = 0;
  }

  UsraCsrWrite (SocketId, 0, HA_COH_CHABC_SAD1_REG, HaCoh.Data);

  return EFI_SUCCESS;
}

/**
  This function programs the FZM_CAPABILITY register to enumerate memory regions that are fast-zero capable.

  @param[in] SocketId             Socket Index.
  @param[in] CapabilityVector     Vector indicates which memory regions are fast-zero capable.
                                  One bit per general-purpose DRAM rule.

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
  //
  // Stub function for API consistency
  //
  return EFI_UNSUPPORTED;
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
  //
  // Stub function for API consistency
  //
  return EFI_UNSUPPORTED;
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
  //
  // Stub function for API consistency
  //
  return;
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
  //
  // Stub function for API consistency
  //
  return;
}
