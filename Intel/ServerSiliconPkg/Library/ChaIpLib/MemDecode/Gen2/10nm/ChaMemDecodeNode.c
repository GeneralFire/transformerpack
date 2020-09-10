/** @file
  Configure CHA memory  decoder related registers for 10nm.

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

//
// Include files
//
#include "Include/ChaRegs.h"
#include <Library/BaseMemoryLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/ChaLib.h>
#include <Library/KtiApi.h>
#include <Library/RcDebugLib.h>
#include <Library/MemoryCoreLib.h>
#include <MemDecodeCommonIncludes.h>
#include "Include/ChaMemDecodePrivate.h"

UINT32 mDramRuleCfgCha[SAD_RULES_10NM] = {
  DRAM_RULE_CFG_0_CHABC_SAD_REG,  DRAM_RULE_CFG_1_CHABC_SAD_REG,  DRAM_RULE_CFG_2_CHABC_SAD_REG,
  DRAM_RULE_CFG_3_CHABC_SAD_REG,  DRAM_RULE_CFG_4_CHABC_SAD_REG,  DRAM_RULE_CFG_5_CHABC_SAD_REG,
  DRAM_RULE_CFG_6_CHABC_SAD_REG,  DRAM_RULE_CFG_7_CHABC_SAD_REG,  DRAM_RULE_CFG_8_CHABC_SAD_REG,
  DRAM_RULE_CFG_9_CHABC_SAD_REG,  DRAM_RULE_CFG_10_CHABC_SAD_REG, DRAM_RULE_CFG_11_CHABC_SAD_REG,
  DRAM_RULE_CFG_12_CHABC_SAD_REG, DRAM_RULE_CFG_13_CHABC_SAD_REG, DRAM_RULE_CFG_14_CHABC_SAD_REG,
  DRAM_RULE_CFG_15_CHABC_SAD_REG
};

UINT32 mInterleaveListCfgCha[SAD_RULES_10NM] = {
  INTERLEAVE_LIST_CFG_0_CHABC_SAD_REG,  INTERLEAVE_LIST_CFG_1_CHABC_SAD_REG,  INTERLEAVE_LIST_CFG_2_CHABC_SAD_REG,
  INTERLEAVE_LIST_CFG_3_CHABC_SAD_REG,  INTERLEAVE_LIST_CFG_4_CHABC_SAD_REG,  INTERLEAVE_LIST_CFG_5_CHABC_SAD_REG,
  INTERLEAVE_LIST_CFG_6_CHABC_SAD_REG,  INTERLEAVE_LIST_CFG_7_CHABC_SAD_REG,  INTERLEAVE_LIST_CFG_8_CHABC_SAD_REG,
  INTERLEAVE_LIST_CFG_9_CHABC_SAD_REG,  INTERLEAVE_LIST_CFG_10_CHABC_SAD_REG, INTERLEAVE_LIST_CFG_11_CHABC_SAD_REG,
  INTERLEAVE_LIST_CFG_12_CHABC_SAD_REG, INTERLEAVE_LIST_CFG_13_CHABC_SAD_REG, INTERLEAVE_LIST_CFG_14_CHABC_SAD_REG,
  INTERLEAVE_LIST_CFG_15_CHABC_SAD_REG
};

UINT32 mRtTypeOffset[] = {
  DRAM_MC_TARGET_CHABC_SAD_REG,
  DRAM_MC_CHANNEL_CHABC_SAD_REG,
  DRAM_ROUTETHROUGH_RANGE_CHABC_SAD_REG
};

UINT32 mBlockDecoderEnCfgChaRegs[] = {
  BLOCK_DECODER_EN_CFG_0_CHABC_SAD1_REG,
  BLOCK_DECODER_EN_CFG_1_CHABC_SAD1_REG,
  BLOCK_DECODER_EN_CFG_2_CHABC_SAD1_REG,
  BLOCK_DECODER_EN_CFG_3_CHABC_SAD1_REG,
  BLOCK_DECODER_EN_CFG_4_CHABC_SAD1_REG,
  BLOCK_DECODER_EN_CFG_5_CHABC_SAD1_REG,
  BLOCK_DECODER_EN_CFG_6_CHABC_SAD1_REG,
  BLOCK_DECODER_EN_CFG_7_CHABC_SAD1_REG
};

UINT32 mRouteTableRegs[][MaxRouteTableDestination][MaxRtHalves] = {
  {
    {H0_CH_ROUTE_TABLE_0_CHA_MISC_REG, H1_CH_ROUTE_TABLE_0_CHA_MISC_REG},
    {H0_TGT_ROUTE_TABLE_0_CHA_MISC_REG, H1_TGT_ROUTE_TABLE_0_CHA_MISC_REG}
  },
  {
    {H0_CH_ROUTE_TABLE_1_CHA_MISC_REG, H1_CH_ROUTE_TABLE_1_CHA_MISC_REG},
    {H0_TGT_ROUTE_TABLE_1_CHA_MISC_REG, H1_TGT_ROUTE_TABLE_1_CHA_MISC_REG}
  },
  {
    {H0_CH_ROUTE_TABLE_2_CHA_MISC_REG, H1_CH_ROUTE_TABLE_2_CHA_MISC_REG},
    {H0_TGT_ROUTE_TABLE_2_CHA_MISC_REG, H1_TGT_ROUTE_TABLE_2_CHA_MISC_REG}
  },
  {
    {0, 0}, // Place holder as no 2LM channel route table
    {H0_TGT_ROUTE_TABLE_2LM_CHA_MISC_REG, H1_TGT_ROUTE_TABLE_2LM_CHA_MISC_REG}
  }
};


/**
  Programs CHA DRAM rule and interleave list for a logical sad entry.

  @param SocketId                The socket number ID and 0 should always be the SBSP.
  @param SadIndex                The index of SAD entry.
  @param SadData                 Logic information for a SAD entry.

  @retval EFI_SUCCESS            The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.

**/
EFI_STATUS
EFIAPI
ChaSetSadData (
  IN UINT8    SocketId,
  IN UINT8    SadIndex,
  IN SAD_DATA *SadData
  )
{
  DRAM_RULE_CFG_0_CHABC_SAD_STRUCT ChaDramRule;

  if (SadData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (SadIndex >= SAD_RULES_10NM) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Clear data field first
  //
  ChaDramRule.Data = 0;

  //
  // Update fields based on SAD table entry
  //
  ChaDramRule.Bits.rule_enable = SadData->Enabled;
  ChaDramRule.Bits.attr = SadData->Attribute;
  ChaDramRule.Bits.limit = SadData->Limit;

  ChaDramRule.Bits.interleave_mode = SadData->InterleaveMode;
  ChaDramRule.Bits.nm_cacheable = SadData->Cacheable;

  //
  // Update DRAM CHA CSR
  //
  UsraCsrWrite (
    SocketId,
    0,
    mDramRuleCfgCha[SadIndex],
    ChaDramRule.Data
  );

  //
  // Update Interleave CHA CSR
  //
  UsraCsrWrite (
    SocketId,
    0,
    mInterleaveListCfgCha[SadIndex],
    (UINT32) SadData->InterleaveList
  );

  ChaSetSadDataEx (SocketId, SadIndex, SadData);

  return EFI_SUCCESS;
}


/**
  Programs mc target, channel and route through info for all SAD rules.

  @param[in] SocketId                The socket number ID and 0 should always be the SBSP.
  @param[in] TypeInfo                The enumerate structure to specify the exact route type register.
  @param[in] Data                    Input array of SAD route type data and its size is maximum SAD rule number.

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaSetRtType (
  IN UINT8            SocketId,
  IN ROUTE_TYPE_INFO  TypeInfo,
  IN UINT8            Data[]
  )
{
  UINT8               Shift;
  UINT32              RegData;
  UINTN               SadIndex;

  if (TypeInfo >= ARRAY_SIZE (mRtTypeOffset)) {
    return EFI_INVALID_PARAMETER;
  }

  Shift = (TypeInfo == RoutetypeRouteThroughRange) ? 1 : 2;
  RegData = 0;
  for (SadIndex = 0; SadIndex < SAD_RULES_10NM; SadIndex++) {
    RegData |= Data[SadIndex] << Shift * SadIndex;
  }

  RcDebugPrint (SDBG_MAX, "Program %a: %08x\n",
    (TypeInfo == RoutetypeRouteThroughRange) ? "Routethrough Range" :
    (TypeInfo == RouteTypeMcTarget) ? "target config" : "channel config",
    RegData
    );

  UsraCsrWrite (
    SocketId,
    0,
    mRtTypeOffset[TypeInfo],
    RegData
  );

  return EFI_SUCCESS;
}


/**
  Programs the base address of block decoder.

  @param SocketId                The socket number ID and 0 should always be the SBSP.
  @param Base                    The base address of block decoder in 64MB unit.
  @param Limit                   The limit address of block decoder in 64MB unit.

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaSetBlockDecoderBase (
  IN UINT8      SocketId,
  IN UINT32     Base,
  IN UINT32     Limit
  )
{
  BLOCK_DECODER_ADDR_N0_CHABC_SAD1_STRUCT  ChaBlockDecoderAddrN0;
  BLOCK_DECODER_ADDR_N1_CHABC_SAD1_STRUCT  ChaBlockDecoderAddrN1;

  ChaBlockDecoderAddrN0.Data = 0;
  ChaBlockDecoderAddrN1.Data = 0;

  ChaBlockDecoderAddrN0.Bits.block_base = Base;
  ChaBlockDecoderAddrN1.Bits.block_limit = Limit;
  ChaBlockDecoderAddrN1.Bits.block_rule_en = 1;

  RcDebugPrint (SDBG_MAX, "Program Block Decoder Control Mailbox Base: 0x%x Limit: 0x%x\n",
    Base,
    Limit
    );

  //
  // Write CHA CSRs for block decoders
  //
  UsraCsrWrite (
    SocketId,
    0,
    BLOCK_DECODER_ADDR_N0_CHABC_SAD1_REG,
    ChaBlockDecoderAddrN0.Data
  );

  UsraCsrWrite (
    SocketId,
    0,
    BLOCK_DECODER_ADDR_N1_CHABC_SAD1_REG,
    ChaBlockDecoderAddrN1.Data
  );

  return EFI_SUCCESS;
}


/**
  Programs the CHA block decode Enable configure registers.

  @param SocketId                The socket number ID and 0 should always be the SBSP.
  @param Index                   Programs the CHA block decode Enable configure registers.
  @param McChEnMask              Array of memory controller and channel that should enable block decoder.

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaSetBlockDecoderEnableCfg (
  IN UINT8      SocketId,
  IN UINT8      Index,
  IN UINT8      McChEnMask[MAX_IMC][MAX_MC_CH]
  )
{
  UINT16        McChEnBitMask;
  UINT8         Mc;
  UINT8         Ch;

  if (Index >= ARRAY_SIZE (mBlockDecoderEnCfgChaRegs)) {
    return EFI_INVALID_PARAMETER;
  }

  McChEnBitMask = 0;
  for (Mc = 0; Mc < MAX_IMC; Mc++) {
    for (Ch = 0; Ch < MAX_MC_CH; Ch++) {
      if (McChEnMask[Mc][Ch] != 0) {
        McChEnBitMask |= 1 << (Ch * 4 + Mc);
      }
    }
  }

  RcDebugPrint (SDBG_MAX, "Program Block Decoder Control Enable Cfg: 0x%x\n",
    McChEnBitMask
    );

  UsraCsrWrite (
    SocketId,
    0,
    mBlockDecoderEnCfgChaRegs[Index],
    McChEnBitMask
  );

  return EFI_SUCCESS;
}


/**
  Programs CHA global interleave register.

  @param SocketId                The socket number ID and 0 should always be the SBSP.
  @param GlobalInterleaveData    Pointer to logical structure to contain global interleave info.

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaSetGlobalInterleave (
  IN UINT8                    SocketId,
  IN GLOBAL_INTERLEAVE_DATA   *GlobalInterleaveData
  )
{
  DRAM_GLOBAL_INTERLEAVE_CHABC_SAD_STRUCT  GlobalDramIntlv;

  if (GlobalInterleaveData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  GlobalDramIntlv.Data = UsraCsrRead (SocketId, 0, DRAM_GLOBAL_INTERLEAVE_CHABC_SAD_REG);

  GlobalDramIntlv.Bits.rt0_interleave_mode = GlobalInterleaveData->Rt0InterleaveShift;
  GlobalDramIntlv.Bits.rt1_interleave_shift = GlobalInterleaveData->Rt1InterleaveShift;
  GlobalDramIntlv.Bits.rt2_interleave_shift = GlobalInterleaveData->Rt2InterleaveShift;

  UsraCsrWrite (SocketId, 0, DRAM_GLOBAL_INTERLEAVE_CHABC_SAD_REG, GlobalDramIntlv.Data);

  return EFI_SUCCESS;
}


/**
  Programs CHA route table based on the type specified.

  @param SocketId                The socket number ID and 0 should always be the SBSP.
  @param ChaId                   The instance of CHA to program.
  @param TypeInfo                For 14nm this field is ignored as 14nm has fixed route table.
                                 For 10nm, it is bit structure to specify the exact CHA route table.
  @param Data                    The 64-bit data to program to the route table register.

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.

**/
EFI_STATUS
EFIAPI
ChaSetRouteTable (
  IN UINT8                 SocketId,
  IN UINT16                ChaId,
  IN ROUTE_TABLE_TYPE_INFO *TypeInfo OPTIONAL,
  IN UINT64                Data
  )
{
  if (TypeInfo == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (TypeInfo->RtType >= MaxRouteTableType) {
    return EFI_INVALID_PARAMETER;
  }

  if (TypeInfo->RtHalf >= MaxRtHalves) {
    return EFI_INVALID_PARAMETER;
  }

  if (TypeInfo->RtType != RouteTable3) { // 10nm RT2LM
    UsraCsrWrite (
      SocketId,
      (UINT8) ChaId,
      mRouteTableRegs[TypeInfo->RtType][RouteTableChannel][TypeInfo->RtHalf],
      (UINT32) RShiftU64 (Data, 32)
      );
  }

  UsraCsrWrite (
    SocketId,
    (UINT8) ChaId,
    mRouteTableRegs[TypeInfo->RtType][RouteTableTarget][TypeInfo->RtHalf],
    (UINT32) Data
    );

  return EFI_SUCCESS;
}

/**
  Programs miscellaneous CHA registers in 2LM mode.

  @param SocketId                The socket number ID and 0 should always be the SBSP.
  @param TwoLmMask               The address mask in 2LM.

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.

**/
EFI_STATUS
EFIAPI
ChaSet2lmRegisters (
  IN UINT8    SocketId,
  IN UINT32   TwoLmMask
  )
{
  UINT8                        ChaId;
  UINT8                        TotalCha;
  TWO_LM_CONFIG_CHA_PMA_STRUCT CboTwoLmConfig;

  if (FeaturePcdGet (PcdCteBuild)) {
    TotalCha = 28;
  } else {
    TotalCha = GetTotChaCount (SocketId);
  }

  CboTwoLmConfig.Data = 0;
  CboTwoLmConfig.Bits.enable = 1;
  CboTwoLmConfig.Bits.mask = TwoLmMask & 0x3FFFF;
  CboTwoLmConfig.Bits.mask_hi = (TwoLmMask >> 18) & 0x3F;

  for (ChaId = 0; ChaId < TotalCha ; ChaId++) {
    UsraCsrWrite (SocketId, ChaId, TWO_LM_CONFIG_CHA_PMA_REG, CboTwoLmConfig.Data);
  }

  return EFI_SUCCESS;
}


/**
  Programs CHA to disable early read.

  @param SocketId                The socket number ID and 0 should always be the SBSP.
  @param DisSpecRdValue          Disable early read value.
  @param IodcEnableValue         IODC Enable value.

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.

**/
EFI_STATUS
EFIAPI
ChaDisableEarlyRead (
  IN UINT8    SocketId,
  IN UINT8    DisSpecRdValue,
  IN BOOLEAN  IodcEnableValue
  )
{
  HA_COH_CHABC_SAD1_STRUCT           HaCoh;

  //
  // Update the HA_COH_CHABC_SAD1_REG
  //
  HaCoh.Data = UsraCsrRead (SocketId, 0, HA_COH_CHABC_SAD1_REG);

  HaCoh.Bits.dis_spec_rd = DisSpecRdValue;

  UsraCsrWrite (SocketId, 0, HA_COH_CHABC_SAD1_REG, HaCoh.Data);

  return EFI_SUCCESS;
}

