/** @file
  Configure CHA IO related registers for 10nm.

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
#include <Library/UsraCsrLib.h>
#include <Library/ChaLib.h>
#include <Library/ChaLibSkx10nm.h>
#include <Library/SystemInfoLib.h>
#include <Library/KtiApi.h>
#include <Library/MemoryServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryCoreLib.h>

/**
  CHA: Configure MMIOL Rule

  @param SbspSocId            - SBSP Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param MmiolRuleRegIndex    - MMIOL Rule Register, 0 based
  @param EnableRule           - Enable MMIOL Rule
  @param MmiolBase            - MMIOL Base
  @param MmiolLimit           - MMIOL Limit

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureMmiolRule (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT8    MmiolRuleRegIndex,
  UINT32   EnableRule,
  UINT32   MmiolBase,
  UINT32   MmiolLimit
  )
{
  UINT32               MmiolRuleOffset[14][2] = { { MMIO_RULE_CFG_0_N0_CHABC_SAD1_REG, MMIO_RULE_CFG_0_N1_CHABC_SAD1_REG },
                                                  { MMIO_RULE_CFG_1_N0_CHABC_SAD1_REG, MMIO_RULE_CFG_1_N1_CHABC_SAD1_REG },
                                                  { MMIO_RULE_CFG_2_N0_CHABC_SAD1_REG, MMIO_RULE_CFG_2_N1_CHABC_SAD1_REG },
                                                  { MMIO_RULE_CFG_3_N0_CHABC_SAD1_REG, MMIO_RULE_CFG_3_N1_CHABC_SAD1_REG },
                                                  { MMIO_RULE_CFG_4_N0_CHABC_SAD1_REG, MMIO_RULE_CFG_4_N1_CHABC_SAD1_REG },
                                                  { MMIO_RULE_CFG_5_N0_CHABC_SAD1_REG, MMIO_RULE_CFG_5_N1_CHABC_SAD1_REG },
                                                  { MMIO_RULE_CFG_6_N0_CHABC_SAD1_REG, MMIO_RULE_CFG_6_N1_CHABC_SAD1_REG },
                                                  { MMIO_RULE_CFG_7_N0_CHABC_SAD1_REG, MMIO_RULE_CFG_7_N1_CHABC_SAD1_REG },
                                                  { MMIO_RULE_CFG_8_N0_CHABC_SAD1_REG, MMIO_RULE_CFG_8_N1_CHABC_SAD1_REG },
                                                  { MMIO_RULE_CFG_9_N0_CHABC_SAD1_REG, MMIO_RULE_CFG_9_N1_CHABC_SAD1_REG },
                                                  { MMIO_RULE_CFG_10_N0_CHABC_SAD1_REG, MMIO_RULE_CFG_10_N1_CHABC_SAD1_REG },
                                                  { MMIO_RULE_CFG_11_N0_CHABC_SAD1_REG, MMIO_RULE_CFG_11_N1_CHABC_SAD1_REG },
                                                  { MMIO_RULE_CFG_12_N0_CHABC_SAD1_REG, MMIO_RULE_CFG_12_N1_CHABC_SAD1_REG },
                                                  { MMIO_RULE_CFG_13_N0_CHABC_SAD1_REG, MMIO_RULE_CFG_13_N1_CHABC_SAD1_REG } };
  MMIO_RULE_CFG_0_N0_CHABC_SAD1_STRUCT      RuleLo;
  MMIO_RULE_CFG_0_N1_CHABC_SAD1_STRUCT      RuleHi;

  RuleLo.Data = EnableRule; // Enable the rule
  RuleLo.Bits.baseaddress = MmiolBase >> 22;
  RuleHi.Data = 0;
  RuleHi.Bits.limitaddress = MmiolLimit >> 22;
  UsraCsrWrite (SocId, BoxInst, MmiolRuleOffset[MmiolRuleRegIndex][1], RuleHi.Data);
  UsraCsrWrite (SocId, BoxInst, MmiolRuleOffset[MmiolRuleRegIndex][0], RuleLo.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Configure MMIOH Rule

  @param SbspSocId            - SBSP Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param MmiohRuleRegIndex    - MMIOH Rule Register, 0 based
  @param Target               - Target
  @param MmiohBase            - MMIOH Base
  @param MmiohLimit           - MMIOH Limit

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureMmiohRule (
  UINT8         SocId,
  UINT8         BoxInst,
  UINT8         MmiohRuleRegIndex,
  UINT32        Target,
  UINT64_STRUCT MmiohBase,
  UINT64_STRUCT MmiohLimit
  )
{
  UINT32               MmiohRuleOffset[13][2] = { { MMIOH_RULE_CFG_0_N0_CHABC_SAD1_REG, MMIOH_RULE_CFG_0_N1_CHABC_SAD1_REG },
                                                  { MMIOH_RULE_CFG_1_N0_CHABC_SAD1_REG, MMIOH_RULE_CFG_1_N1_CHABC_SAD1_REG },
                                                  { MMIOH_RULE_CFG_2_N0_CHABC_SAD1_REG, MMIOH_RULE_CFG_2_N1_CHABC_SAD1_REG },
                                                  { MMIOH_RULE_CFG_3_N0_CHABC_SAD1_REG, MMIOH_RULE_CFG_3_N1_CHABC_SAD1_REG },
                                                  { MMIOH_RULE_CFG_4_N0_CHABC_SAD1_REG, MMIOH_RULE_CFG_4_N1_CHABC_SAD1_REG },
                                                  { MMIOH_RULE_CFG_5_N0_CHABC_SAD1_REG, MMIOH_RULE_CFG_5_N1_CHABC_SAD1_REG },
                                                  { MMIOH_RULE_CFG_6_N0_CHABC_SAD1_REG, MMIOH_RULE_CFG_6_N1_CHABC_SAD1_REG },
                                                  { MMIOH_RULE_CFG_7_N0_CHABC_SAD1_REG, MMIOH_RULE_CFG_7_N1_CHABC_SAD1_REG },
                                                  { MMIOH_RULE_CFG_8_N0_CHABC_SAD1_REG, MMIOH_RULE_CFG_8_N1_CHABC_SAD1_REG },
                                                  { MMIOH_RULE_CFG_9_N0_CHABC_SAD1_REG, MMIOH_RULE_CFG_9_N1_CHABC_SAD1_REG },
                                                  { MMIOH_RULE_CFG_10_N0_CHABC_SAD1_REG, MMIOH_RULE_CFG_10_N1_CHABC_SAD1_REG },
                                                  { MMIOH_RULE_CFG_11_N0_CHABC_SAD1_REG, MMIOH_RULE_CFG_11_N1_CHABC_SAD1_REG },
                                                  { MMIOH_RULE_CFG_12_N0_CHABC_SAD1_REG, MMIOH_RULE_CFG_12_N1_CHABC_SAD1_REG } };

  MMIOH_RULE_CFG_0_N0_CHABC_SAD1_STRUCT         RuleLo;
  MMIOH_RULE_CFG_0_N1_CHABC_SAD1_STRUCT         RuleHi;

  RuleLo.Data = 1; // Enable the rule
  RuleLo.Bits.baseaddress = (MmiohBase.lo >> 30) | ((MmiohBase.hi & 0xFFFFF) << 2);
  RuleHi.Data = 0;
  RuleHi.Bits.limitaddress = (MmiohLimit.lo >> 30) | ((MmiohLimit.hi & 0xFFFFF) << 2);
  RuleHi.Bits.target = Target;
  UsraCsrWrite (SocId, BoxInst, MmiohRuleOffset[MmiohRuleRegIndex][1], RuleHi.Data);
  UsraCsrWrite (SocId, BoxInst, MmiohRuleOffset[MmiohRuleRegIndex][0], RuleLo.Data);

  return EFI_SUCCESS;
}

/**
  CHA:  Get MMIOH Interleave Target List

  @param SocId                               - CPU Socket Node number (Socket ID)
  @param BoxInst                             - Box Instance, 0 based
  @param MmiohTargetInterleaveListRegIndex   - MMIOH Interleave List Register Index, 0 based
  @param MmiohTargetInterleaveList           - MMIOH Interleave List

  @retval                                    - EFI_UNSUPPORTED: the function not implemented
                                               EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
ChaGetMmiohTargetInterleaveList (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  IN  UINT8    MmiohTargetInterleaveListRegIndex,
  OUT UINT32*  MmiohTargetInterleaveList
  )
{
  return EFI_UNSUPPORTED;
}

/**
  CHA: Configure MMIOH Target Interleave List

  @param SocId                             - CPU Socket Node number (Socket ID)
  @param BoxInst                           - Box Instance, 0 based
  @param MmiohTargetInterleaveListRegIndex - MMIOH Target Interleave List Register Index, 0 based
  @param MmiohTargetInterleaveList         - MMIOH Target Interleave List

  @retval                                  - EFI_UNSUPPORTED: the function not implemented
                                             EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
ChaConfigureMmiohTargetInterleaveList (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT8    MmiohTargetInterleaveListRegIndex,
  UINT32   MmiohTargetInterleaveList
  )
{
  return EFI_UNSUPPORTED;
}
/**
  CHA: Configure MMIOH Rule Entry

  @param SocId                  - CPU Socket Node number (Socket ID)
  @param MaxSocket              - Max Socket to support
  @param CpuSktHotPlugEn        - CPU Socket Hot Plug enabled
  @param MmiohTargetSize        - Mmioh Target Size
  @param UseNonInterleavedMmioh - Use non-Interleaved Mmioh

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureCpuMmiohEntry (
  UINT8           SocId,
  UINT8           MaxSocket,
  KTI_SOCKET_DATA *SocketData,
  UINT8           CpuSktHotPlugEn,
  UINT16          MmiohTargetSize,
  BOOLEAN         UseNonInterleavedMmioh
  )
{
  UINT8     Stack, Socket, TargetIndex = 0;
  UINT64_STRUCT MmiohBaseTmp, MmiohLimitTmp;
  KTI_CPU_RESOURCE  SocketResouceTmp;

  for (Socket = 0; Socket < MaxSocket; Socket++) {
    GetSocketResouce (Socket, &SocketResouceTmp);
    //
    // Socket present?
    //
    if ((SocketData->Cpu[Socket].Valid == TRUE) || (CpuSktHotPlugEn == TRUE)) {
      if (Socket == SocId) {
        for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
          //
          // Program a local rule for each enabled stack
          //
          if (IfStackPresent (Socket, Stack)) {
            MmiohBaseTmp = SocketResouceTmp.StackRes[Stack].MmiohBase;
            if ((MmiohBaseTmp.hi != 0xFFFFFFFF) && (MmiohBaseTmp.lo != 0xFFFFFFFF)) {
              MmiohLimitTmp = SocketResouceTmp.StackRes[Stack].MmiohLimit;
              if ((MmiohBaseTmp.hi < MmiohLimitTmp.hi) || ((MmiohBaseTmp.hi == MmiohLimitTmp.hi) && (MmiohBaseTmp.lo < MmiohLimitTmp.lo))) {
                ChaConfigureMmiohRule (SocId, 0, TargetIndex, Stack | SOCKET_LOCAL_4BITS_NID_MSB, MmiohBaseTmp, MmiohLimitTmp);
                TargetIndex++;
              }
            }
          } //if (StackPresent..
        } //for (Stack = 0; Stack < MAX_IIO_STACK...
      } else {
        //
        // Program off socket rule
        //
        // MMIOH Rule
        //
        MmiohBaseTmp = SocketResouceTmp.MmiohBase;
        //
        // Don't process is socket starved
        //
        if ((MmiohBaseTmp.hi != 0xFFFFFFFF) && (MmiohBaseTmp.lo != 0xFFFFFFFF)) {
          MmiohLimitTmp = SocketResouceTmp.MmiohLimit;
          if ((MmiohBaseTmp.hi < MmiohLimitTmp.hi) || ((MmiohBaseTmp.hi == MmiohLimitTmp.hi) && (MmiohBaseTmp.lo < MmiohLimitTmp.lo))) {
            ChaConfigureMmiohRule (SocId, 0, TargetIndex, Socket, MmiohBaseTmp, MmiohLimitTmp);
            TargetIndex++;
          }
        } //if ((MmiohBaseTmp.hi != 0xFFFFFFFF)...
      } //if (Socket == SocId) {...
    } //if ((SocketData->Cpu[Socket].Valid == TRUE)...
  } //for (Socket = 0; Socket < TmpTotCpu;

  return EFI_SUCCESS;
}

/**
  CHA: Configure NEM RULE Base

  @param SbspSocId            - SBSP Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param MmioNemRuleBase      - MMIO NEM Rule Base

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureNemRuleBase (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   MmioNemRuleBase
  )
{
  MMIO_RULE_CFG_0_N0_CHABC_SAD1_STRUCT      RuleLo;

  RuleLo.Data = UsraCsrRead(SocId, BoxInst, MMIO_RULE_CFG_13_N0_CHABC_SAD1_REG);

  //
  // To cover any gap, lower Rule13 to start at end of mmcfg for NEM/Ego coverage and point at legacy cstack
  //
  RuleLo.Bits.baseaddress = MmioNemRuleBase >> 22;

  UsraCsrWrite(SocId, BoxInst, MMIO_RULE_CFG_13_N0_CHABC_SAD1_REG, RuleLo.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Configure UBOX MMIO

  @param SbspSocId            - SBSP Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param UboxMmioBase         - UBOX MMIO Base
  @param UboxMmioLimit        - UBOX MMIO Limit

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureUboxMmio (
  UINT8         SocId,
  UINT8         BoxInst,
  UINT32        UboxMmioBase,
  UINT32        UboxMmioLimit
  )
{
  MMIO_UBOX_RULE_N0_CHABC_SAD1_STRUCT      RuleLo;
  MMIO_UBOX_RULE_N1_CHABC_SAD1_STRUCT      RuleHi;

  //
  // Program CHA mmio dedicated rule for ubox mmio bars
  //
  RuleLo.Data = 1; // Enable the rule
  RuleHi.Data = 0;
  RuleLo.Bits.baseaddress = UboxMmioBase >> 22;
  RuleHi.Bits.limitaddress = UboxMmioLimit >> 22;

  UsraCsrWrite (SocId, BoxInst, MMIO_UBOX_RULE_N1_CHABC_SAD1_REG, RuleHi.Data);
  UsraCsrWrite (SocId, BoxInst, MMIO_UBOX_RULE_N0_CHABC_SAD1_REG, RuleLo.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Configure SAD Target

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param FlashTarget      - Flash Target     : 0xFFFFFFFF: no need to configure
  @param LegacyPchTarget  - Legacy PCH Target: 0xFFFFFFFF: no need to configure
  @param VgaTarget        - VGA target       : 0xFFFFFFFF: no need to configure
  @param SourceId         - Source ID        : 0xFFFFFFFF: no need to configure
  @param SourceIdEnable   - SourceId enabled : 0xFFFFFFFF: no need to configure

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaSadTargetCfg (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   FlashTarget,
  UINT32   LegacyPchTarget,
  UINT32   VgaTarget,
  UINT32   SourceId,
  UINT32   SourceIdEnable
  )
{
  SAD_TARGET_CHABC_SAD1_STRUCT         SadTarget;

  SadTarget.Data = UsraCsrRead (SocId, BoxInst, SAD_TARGET_CHABC_SAD1_REG);

  if (FlashTarget != CHA_DEFAULT_CFG) {
    SadTarget.Bits.flash_target = FlashTarget;
  }
  if (LegacyPchTarget != CHA_DEFAULT_CFG) {
    SadTarget.Bits.legacy_pch_target = LegacyPchTarget;
  }
  if (VgaTarget != CHA_DEFAULT_CFG) {
    SadTarget.Bits.vga_target = VgaTarget;
  }
  if (SourceId != CHA_DEFAULT_CFG) {
    SadTarget.Bits.sourceid = SourceId;
  }
  if (SourceIdEnable != CHA_DEFAULT_CFG) {
    SadTarget.Bits.sourceiden = SourceIdEnable;
  }

  UsraCsrWrite (SocId, BoxInst, SAD_TARGET_CHABC_SAD1_REG, SadTarget.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Configure the Local socket ID

  @param SocId           - CPU Socket Node number (Socket ID)
  @param BoxInst         - Box Instance, 0 based
  @param LocalSocketId   - Local Socket Id to be configured

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureLocalSocketId (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT8    LocalSocketId
  )
{
  //
  // SAD Control - program the local socket id (node id)
  //
  UsraCsrWrite (SocId, BoxInst, SAD_CONTROL_CHABC_SAD1_REG, LocalSocketId);

  return EFI_SUCCESS;
}

/**
  CHA: Get LT Target and Enabled status

  @param SocId           - CPU Socket Node number (Socket ID)
  @param BoxInst         - Box Instance, 0 based
  @param LtTarget        - LT Target         : 0xFFFFFFFF: no need to configure
  @param LtTargetEnabled - LT Target Enabled : 0xFFFFFFFF: no need to configure

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaGetLtTargetAndStatus (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   *LtTarget,
  UINT32   *LtTargetEnabled
  )
{
  LT_CONTROL_CHABC_SAD1_STRUCT         LtControl;

  LtControl.Data = UsraCsrRead (SocId, BoxInst, LT_CONTROL_CHABC_SAD1_REG);

  if (LtTargetEnabled != NULL) {
    *LtTargetEnabled = LtControl.Bits.lt_target_enable;
  }

  if (LtTarget != NULL) {
    *LtTarget = LtControl.Bits.lt_target;
  }

  return EFI_SUCCESS;
}

/**
  CHA: Set LT Target and status

  @param SocId           - CPU Socket Node number (Socket ID)
  @param BoxInst         - Box Instance, 0 based
  @param LtTarget        - LT Target
  @param LtTargetEnabled - LT Target Enabled

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaSetLtTargetAndStatus (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   LtTarget,
  UINT32   LtTargetEnabled
  )
{
  LT_CONTROL_CHABC_SAD1_STRUCT         LtControl;

  LtControl.Data = UsraCsrRead (SocId, BoxInst, LT_CONTROL_CHABC_SAD1_REG);

  LtControl.Bits.lt_target_enable = LtTargetEnabled;
  LtControl.Bits.lt_target = LtTarget;

  UsraCsrWrite (SocId, BoxInst, LT_CONTROL_CHABC_SAD1_REG, LtControl.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Get Mmcfg Target List

  @param SocId           - CPU Socket Node number (Socket ID)
  @param BoxInst         - Box Instance, 0 based
  @param MmcfgTargetList - Mmcfg Target List

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaGetMmcfgTargetList (
  IN     UINT8   SocId,
  IN     UINT8   BoxInst,
  IN OUT UINT32* MmcfgTargetList
  )
{
  *MmcfgTargetList = UsraCsrRead (SocId, BoxInst, MMCFG_TARGET_LIST_CHABC_SAD1_REG);

  return EFI_SUCCESS;
}

/**
  CHA: Get Mmcfg Target List

  @param SocId             - CPU Socket Node number (Socket ID)
  @param BoxInst           - Box Instance, 0 based
  @param MmCfgTargetList   - Mmcfg Target List to be set

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaSetMmcfgTargetList (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   MmCfgTargetList
  )
{
  UsraCsrWrite (SocId, BoxInst, MMCFG_TARGET_LIST_CHABC_SAD1_REG, MmCfgTargetList);

  return EFI_SUCCESS;
}

/**
  CHA: Configure Mmcfg Rules

  @param SocId           - CPU Socket Node number (Socket ID)
  @param BoxInst         - Box Instance, 0 based
  @param EnableRule      - Enable Mmcfg Rule
  @param MmcfgLength     - Mmcfg Length
  @param ValidTarget     - Valid Target
  @param BaseAddrLow     - Base Address Low
  @param BaseAddrHi      - Base Address Hi
  @param LocalRuleCfg    - pointer to Local Rule CFG array
  @param LocalRuleAddr   - pointer to Local Rule Address array

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureMmcfgRules (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT8    EnableRule,
  UINT32   MmcfgLength,
  UINT32   ValidTarget,
  UINT32   BaseAddrLow,
  UINT32   BaseAddrHi,
  UINT32   *LocalRuleCfg,
  UINT32   *LocalRuleAddr
  )
{
  UINT32    MmcfgLocalRuleAddressOffset[] = { MMCFG_LOCAL_RULE_ADDRESS_CFG_0_CHABC_SAD1_REG,
                                              MMCFG_LOCAL_RULE_ADDRESS_CFG_1_CHABC_SAD1_REG};
  UINT8     Index;

  for (Index = 0; Index < sizeof (MmcfgLocalRuleAddressOffset) / sizeof (UINT32); Index ++) {
    UsraCsrWrite (SocId, BoxInst, MmcfgLocalRuleAddressOffset[Index], *((volatile UINT32 *)(LocalRuleAddr + Index)));
  }
  UsraCsrWrite(SocId, BoxInst, MMCFG_LOCAL_RULE_CHABC_SAD1_REG, *((volatile UINT32 *)LocalRuleCfg));

  return EFI_SUCCESS;
}

/**
  CHA: Get Mmcfg Target List and Rules

  @param SocId           - CPU Socket Node number (Socket ID)
  @param BoxInst         - Box Instance, 0 based
  @param MmCfgTargetList - Mmcfg Target List
  @param MmcfgRule       - Mmcfg Rule
  @param LocalRuleAddr   - Local Rule Address

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaGetMmcfgTargetListAndRules (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   *MmCfgTargetList,
  UINT32   *MmcfgRule,
  UINT32   LocalRuleAddr[]
  )
{
  if (MmCfgTargetList != NULL) {
    *MmCfgTargetList = UsraCsrRead (SocId, BoxInst, MMCFG_TARGET_LIST_CHABC_SAD1_REG);
  }
  if (MmcfgRule != NULL) {
    *MmcfgRule = UsraCsrRead(SocId, BoxInst, MMCFG_RULE_N0_CHABC_SAD1_REG);
  }
  if (LocalRuleAddr != NULL) {
    LocalRuleAddr[0] = UsraCsrRead (SocId, BoxInst, MMCFG_LOCAL_RULE_ADDRESS_CFG_0_CHABC_SAD1_REG);
    LocalRuleAddr[1] = UsraCsrRead (SocId, BoxInst, MMCFG_LOCAL_RULE_ADDRESS_CFG_1_CHABC_SAD1_REG);
  }

  return EFI_SUCCESS;
}

/**
  CHA: Set Mmcfg Target List and Rules

  @param SocId           - CPU Socket Node number (Socket ID)
  @param BoxInst         - Box Instance, 0 based
  @param MmCfgTargetList - Mmcfg Target List
  @param MmcfgRule       - Mmcfg Rule
  @param LocalRuleAddr   - Array to Local Rule Address

  @retval                - EFI_SUCCESS: success
                         - EFI_UNSUPPORTED: unsupported register on this HW IP
                         - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaSetMmcfgTargetListAndRules (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   MmCfgTargetList,
  IN UINT32   MmcfgRule,
  IN UINT32   LocalRuleAddr[]
  )
{
  MMCFG_RULE_N0_CHABC_SAD1_STRUCT  MmcfgRuleN0;
  MmcfgRuleN0.Data = MmcfgRule;

  //
  // If Rule_Enable is 0, the MMCFG_RULE should be programmed first
  //
  if ( MmcfgRuleN0.Bits.rule_enable == 0) {
    UsraCsrWrite (SocId, BoxInst, MMCFG_RULE_N0_CHABC_SAD1_REG, MmcfgRule);
  }

  UsraCsrWrite (SocId, BoxInst, MMCFG_TARGET_LIST_CHABC_SAD1_REG, MmCfgTargetList);
  UsraCsrWrite (SocId, BoxInst, MMCFG_LOCAL_RULE_ADDRESS_CFG_0_CHABC_SAD1_REG, LocalRuleAddr[0]);
  UsraCsrWrite (SocId, BoxInst, MMCFG_LOCAL_RULE_ADDRESS_CFG_1_CHABC_SAD1_REG, LocalRuleAddr[1]);

  //
  // If Rule_Enable is 1, the MMCFG_TARGET_LIST/MMCFG_LOCAL_RULE should be programmed first
  //
  if ( MmcfgRuleN0.Bits.rule_enable == 1) {
    UsraCsrWrite (SocId, BoxInst, MMCFG_RULE_N0_CHABC_SAD1_REG, MmcfgRule);
  }

  return EFI_SUCCESS;
}

/**
  CHA: Set Temp Bus No. and Mmcfg

  @param SocId           - CPU Socket Node number (Socket ID)
  @param BoxInst         - Box Instance, 0 based
  @param MmCfgTargetList - Used to save original Mmcfg Target List
  @param MmcfgRule       - Used to save original Mmcfg Rule
  @param LocalRuleAddr   - Used to save original Local Rule Address

  @retval                - EFI_SUCCESS: success
                         - EFI_UNSUPPORTED: unsupported register on this HW IP
                         - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaSetTempBusNumAndMmcfg (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT UINT32   *MmCfgTargetList,
  OUT UINT32   *MmcfgRule,
  OUT UINT32   LocalRuleAddr[]
  )
{
  UINT32  MMCFG_RULE_New, MMCFG_TARGET_LIST_New;
  UINT32  MMCFG_RULE_Addr[2];
  UINT32  Data32;

  // save original CPUBUSNO_UBOX and MMCFG_LOCAL_RULE and MMCFG_TARGET_LIST on NBSP
  ChaGetMmcfgTargetListAndRules (SocId, 0, MmCfgTargetList, MmcfgRule, LocalRuleAddr);
  // assign temp CPUBUSNO_UBOX and MMCFG_LOCAL_RULE/MMCFG_TARGET_LIST on NBSP, for IIO stack access
  Data32 = 0x76543210 | (1<<(SocId*4+3));
  MMCFG_TARGET_LIST_New = Data32;
  Data32 = GetStackBus (SocId, 0);
  Data32 += (GetStackBus (SocId, 0) << 8);
  Data32 += (GetStackBus (SocId, 1) << 16);
  Data32 += (GetStackBus (SocId, 2) << 24);
  MMCFG_RULE_Addr [0] = Data32;
  Data32 = GetStackBus (SocId, 3);
  Data32 += (GetStackBus (SocId, 4) << 8);
  Data32 += (GetStackBus (SocId, 5) << 16);
  MMCFG_RULE_Addr [1] = Data32;
  Data32 = *MmcfgRule;
  Data32 |= BIT0;
  MMCFG_RULE_New = Data32;
  ChaSetMmcfgTargetListAndRules (SocId, 0, MMCFG_TARGET_LIST_New, MMCFG_RULE_New, MMCFG_RULE_Addr);

  return EFI_SUCCESS;
}

/**
  CHA: Set Mmcfg Rule

  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxInst   - Box Instance, 0 based
  @param MmcfgRule - Mmcfg Rule

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaSetMmcfgRule (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   MmcfgRule
  )
{
  UsraCsrWrite (SocId, BoxInst, MMCFG_RULE_N0_CHABC_SAD1_REG, MmcfgRule);

  return EFI_SUCCESS;
}

/**
  CHA: Get Mmcfg Valid Targets

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param MmcfgRuleValidTarget - Mmcfg valid targets

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaGetMmcfgRuleValidTarget (
  IN  UINT8   SocId,
  IN  UINT8   BoxInst,
  OUT UINT32* MmcfgRuleValidTarget
  )
{
  MMCFG_RULE_N0_CHABC_SAD1_STRUCT MmcfgRule0;

  MmcfgRule0.Data = UsraCsrRead (SocId, BoxInst, MMCFG_RULE_N0_CHABC_SAD1_REG);

  *MmcfgRuleValidTarget = MmcfgRule0.Bits.valid_targets;

  return EFI_SUCCESS;
}

/**
  CHA: Set Mmcfg Rule Valid Target

  @param SocId       - CPU Socket Node number (Socket ID)
  @param BoxInst     - Box Instance, 0 based
  @param ValidTarget - Mmcfg Rule Valid Target

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaSetMmcfgRuleValidTarget (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   ValidTarget
  )
{
  MMCFG_RULE_N0_CHABC_SAD1_STRUCT MmcfgRule0;

  MmcfgRule0.Data = UsraCsrRead (SocId, BoxInst, MMCFG_RULE_N0_CHABC_SAD1_REG);
  MmcfgRule0.Bits.valid_targets = ValidTarget;

  UsraCsrWrite (SocId, BoxInst, MMCFG_RULE_N0_CHABC_SAD1_REG, MmcfgRule0.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Configure IO Port Target List

  @param SocId                    - CPU Socket Node number (Socket ID)
  @param IoTgtLst                 - IO Target List

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureIoPortTargetList (
  UINT8    SocId,
  UINT32   IoTgtLst[MAX_SAD_TARGETS]
  )
{
  UINT8     RegIndex = 0;
  UINT8     TargetIndex = 0;
  UINT32    Data32 = 0;
  UINT32    IoPortTargetOffset[] = { IOPORT_TARGET_LIST_CFG_0_CHABC_SAD_REG,
                                     IOPORT_TARGET_LIST_CFG_1_CHABC_SAD_REG,
                                     IOPORT_TARGET_LIST_CFG_2_CHABC_SAD_REG,
                                     IOPORT_TARGET_LIST_CFG_3_CHABC_SAD_REG};

  for (RegIndex = 0; RegIndex < MAX_SAD_TARGETS/8; RegIndex++) {
    Data32 = 0;
    for (TargetIndex = 0; TargetIndex < 8; TargetIndex++) {
      if (IoTgtLst[RegIndex*8 + TargetIndex] & SOCKET_LOCAL_FLAG) {
        Data32 |= (((IoTgtLst[RegIndex*8 + TargetIndex] & 0xF) | BIT3) << (4*TargetIndex));
      } else if (IoTgtLst[RegIndex*8 + TargetIndex] & SOCKET_REMOTE_FLAG)  {
        Data32 |= ((IoTgtLst[RegIndex*8 + TargetIndex] & 0xF) << (4*TargetIndex));
      } else {
        Data32 |= (BIT3 << (4*TargetIndex));
      }
    }
    UsraCsrWrite (SocId, 0, IoPortTargetOffset[RegIndex], Data32);
  }

  return EFI_SUCCESS;
}

/**
  CHA: Configure IO APIC Target List

  @param SocId                    - CPU Socket Node number (Socket ID)
  @param IoApicTgtLst             - IO APIC Target List

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureIoApicTargetList (
  UINT8    SocId,
  UINT32   IoApicTgtLst [MAX_SAD_TARGETS]
  )
{
  UINT8     RegIndex = 0;
  UINT8     TargetIndex = 0;
  UINT32    Data32 = 0;
  UINT32    IoApicTargetOffset[] = { IOAPIC_TARGET_LIST_CFG_0_CHABC_SAD1_REG,
                                     IOAPIC_TARGET_LIST_CFG_1_CHABC_SAD1_REG,
                                     IOAPIC_TARGET_LIST_CFG_2_CHABC_SAD1_REG,
                                     IOAPIC_TARGET_LIST_CFG_3_CHABC_SAD1_REG};

  for (RegIndex = 0; RegIndex < MAX_SAD_TARGETS/8; RegIndex++) {
    Data32 = 0;
    for (TargetIndex = 0; TargetIndex < 8; TargetIndex++) {
      if (IoApicTgtLst[RegIndex*8 + TargetIndex] & SOCKET_LOCAL_FLAG) {
        Data32 |= (((IoApicTgtLst[RegIndex*8 + TargetIndex] & 0xF) | BIT3) << (4*TargetIndex));
      } else if (IoApicTgtLst[RegIndex*8 + TargetIndex] & SOCKET_REMOTE_FLAG)  {
        Data32 |= ((IoApicTgtLst[RegIndex*8 + TargetIndex] & 0xF) << (4*TargetIndex));
      } else {
        Data32 |= (BIT3 << (4*TargetIndex));
      }
    }
    UsraCsrWrite (SocId, 0, IoApicTargetOffset[RegIndex], Data32);
  }

  return EFI_SUCCESS;
}

/**
  CHA: Configure MMIO Target List

  @param SocId                    - CPU Socket Node number (Socket ID)
  @param BoxInst                  - Box Instance, 0 based
  @param MmioTargetListRegIndex   - MMIO Target List Register Index, 0 based
  @param MmioTgtLst               - MMIO Target List

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureMmioTargetList (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT8    MmioTargetListRegIndex,
  UINT32   MmioTgtLst
  )
{
  UINT32              MmiolTargetOffset[2] = { MMIO_TARGET_LIST_CFG_0_CHABC_SAD1_REG,
                                               MMIO_TARGET_LIST_CFG_1_CHABC_SAD1_REG};

  UsraCsrWrite (SocId, BoxInst, MmiolTargetOffset[MmioTargetListRegIndex], MmioTgtLst);

  return EFI_SUCCESS;
}

/**
  CHA: Reprogram bus/mmcfg on PBSP sockets

  @param SocId        - CPU Socket Node number (Socket ID)
  @param BoxInst      - Box Instance, 0 based
  @param MmcfgLength  - Mmcfg length
  @param Targets      - Mmcfg valid target
  @param Base_address - Mmcfg rule base address

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.
**/
EFI_STATUS
EFIAPI
ReprogramPbspBusmmcfg (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT8    MmcfgLength,
  UINT8    Targets,
  UINT32   Base_address
  )
{
  MMCFG_RULE_N0_CHABC_SAD1_STRUCT  mmcfgrule0;
  UINT32                           Data32;

  mmcfgrule0.Data = UsraCsrRead (SocId, 0, MMCFG_RULE_N0_CHABC_SAD1_REG);
  mmcfgrule0.Bits.rule_enable = 0;
  mmcfgrule0.Bits.length = MmcfgLength;
  mmcfgrule0.Bits.base_address = Base_address;
  mmcfgrule0.Bits.valid_targets = Targets;
  UsraCsrWrite (SocId, 0, MMCFG_RULE_N0_CHABC_SAD1_REG, mmcfgrule0.Data);
  Data32 = 0;
  UsraCsrWrite (SocId, 0, MMCFG_RULE_N1_CHABC_SAD1_REG, Data32);

  return EFI_SUCCESS;
}