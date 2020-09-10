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

#ifndef __CHA_LIB_H__
#define __CHA_LIB_H__

#include <Uefi.h>
#include <Base.h>
#include <Upi/KtiHost.h>

#define CHA_DEFAULT_CFG   0xFFFFFFFF

#define ChaGetSncCfg(_SocId_, _CtrId_)      UsraCsrRead((_SocId_), (_CtrId_), CHA_SNC_CONFIG_CHA_PMA_REG)

typedef enum {
  ChaLbWMcheckRacN0RegIndex       = 0,
  ChaLbWMcheckRac0x3FFN0RegIndex  = 1,
  ChaLbWMcheckWac0x3FFN0RegIndex  = 2,
  ChaLbWMcheckWac0x3FFN1RegIndex  = 3,
  ChaLbWMcheckWacN0RegIndex       = 4,
  ChaLbWMcheckWacN1RegIndex       = 5,
  ChaLbWMcheckCp0x3FFN0RegIndex   = 6,
  ChaLbWMcheckCp0x3FFN1RegIndex   = 7,
  ChaLbWMcheckCpN0RegIndex        = 8,
  ChaLbWMcheckCpN1RegIndex        = 9
} CHA_SAI_REG_ENUM;

typedef enum {
  CHA_SNOOP_THRESHOLD_DISABLED = 0,
  CHA_SNOOP_THRESHOLD_LOW,
  CHA_SNOOP_THRESHOLD_MED,
  CHA_SNOOP_THRESHOLD_HIGH,
  CHA_SNOOP_THRESHOLD_MAX
} CHA_SNOOP_THRESHOLD;

/**

  ChaGetSetSaiRegisters.

  @param SocId         - CPU Socket Node number (Socket ID)
  @param BoxInst       - Box Instance, 0 based
  @param SaiRegIndex   - SaiRegIndex
  @param Get           - TRUE: read register into *Value, FALSE: write register using data in *Value
  @param *Value        - *Value: pointer of UINT32, can be input & output

  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaGetSetSaiRegisters (
  UINT8                      SocId,
  UINT8                      BoxInst,
  IN CHA_SAI_REG_ENUM        SaiRegIndex,
  IN BOOLEAN                 Get,
  IN OUT UINT32              *Value
  );
//
// Function pointer definition for ChaGetSetSaiRegisters()
//
typedef
EFI_STATUS
(EFIAPI *CHA_GET_SET_SAI_REGS) (
  IN UINT8                      SocId,
  IN UINT8                      BoxInst,
  IN CHA_SAI_REG_ENUM           SaiRegIndex,
  IN BOOLEAN                    Get,
  IN OUT UINT32                 *Value
  );

/**
  CHA: Configure KTI TOR Timeout

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param EntryCtrIncCtl   - Indication of which bit of upper base counter
                            increments entry counter.
  @param Level0Offset     - Encoded Intel UPI Level 0 Timeout offset.
  @param Level1Offset     - Encoded Intel UPI Level 1 Timeout offset.
  @param Level2Offset     - Encoded Intel UPI Level 2 Timeout offset.
  @param Level3Offset     - Encoded Intel UPI Level 3 Timeout offset.
  @param Level4Offset     - Encoded Intel UPI Level 4 Timeout offset.
  @param Level6Offset     - Encoded Intel UPI Level 6 Timeout offset.

  @retval EFI_STATUS      - EFI_SUCCESS: success
                          - EFI_UNSUPPORTED: unsupported register on this HW IP
                          - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureKtiTorTimeout (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    EntryCtrIncCtl,
  IN UINT8    Level0Offset,
  IN UINT8    Level1Offset,
  IN UINT8    Level2Offset,
  IN UINT8    Level3Offset,
  IN UINT8    Level4Offset,
  IN UINT8    Level6Offset
  );

//
// Function pointer definition for ChaConfigureKtiTorTimeout()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_KTI_TOR_TIMEOUT) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    EntryCtrIncCtl,
  IN UINT8    Level0Offset,
  IN UINT8    Level1Offset,
  IN UINT8    Level2Offset,
  IN UINT8    Level3Offset,
  IN UINT8    Level4Offset,
  IN UINT8    Level6Offset
  );

/**
  CHA: Fixup PAM Attributes

  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxInst   - Box Instance, 0 based

  @retval          - EFI_SUCCESS: success
                   - EFI_UNSUPPORTED: unsupported register on this HW IP
                   - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaFixupPamAttributes (
  IN UINT8    SocId,
  IN UINT8    BoxInst
  );

//
// Function pointer definition for ChaFixupPamAttributes()
//
typedef
EFI_STATUS
(EFIAPI *CHA_FIXUP_PAM_ATTRIBUTES) (
  IN UINT8    SocId,
  IN UINT8    BoxInst
  );

/**
  CHA: Program All PAM region access to DRAM

  @param SocId     - CPU Socket Node number (Socket ID)

  @retval          - EFI_SUCCESS: success
                   - EFI_UNSUPPORTED: unsupported register on this HW IP
                   - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ProgramAllPamAccessToDram (
  IN UINT8 SocId
  );

//
// Function pointer definition for ProgramAllPamAccessToDram()
//
typedef
EFI_STATUS
(EFIAPI *PROGRAM_ALL_PAM_ACCESS_TO_DRAM) (
  IN UINT8 SocId
  );

/**
  CHA: Enable Modulo3 based KTI Link

  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxInst   - Box Instance, 0 based

  @retval          - EFI_SUCCESS: success
                   - EFI_UNSUPPORTED: unsupported register on this HW IP
                   - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaSadTargetEnableModulo3BasedKtiLink (
  IN UINT8    SocId,
  IN UINT8    BoxInst
  );

//
// Function pointer definition for ChaSadTargetEnableModulo3BasedKtiLink()
//
typedef
EFI_STATUS
(EFIAPI *CHA_SAD_TARGET_EN_MODULO3_BASED_KTI_LINK) (
  IN UINT8    SocId,
  IN UINT8    BoxInst
  );

/**
  CHA: Configure SAD Target

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param FlashTarget      - Flash Target     : 0xFFFFFFFF: no need to configure
  @param LegacyPchTarget  - Legacy PCH Target: 0xFFFFFFFF: no need to configure
  @param VgaTarget        - VGA target       : 0xFFFFFFFF: no need to configure
  @param SourceId         - Source ID        : 0xFFFFFFFF: no need to configure
  @param SourceIdEnable   - SourceId enabled : 0xFFFFFFFF: no need to configure

  @retval                 - EFI_SUCCESS: success
                          - EFI_UNSUPPORTED: unsupported register on this HW IP
                          - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaSadTargetCfg (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   FlashTarget,
  IN UINT32   LegacyPchTarget,
  IN UINT32   VgaTarget,
  IN UINT32   SourceId,
  IN UINT32   SourceIdEnable
  );

//
// Function pointer definition for ChaSadTargetCfg()
//
typedef
EFI_STATUS
(EFIAPI *CHA_SAD_TARGET_CFG) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   FlashTarget,
  IN UINT32   LegacyPchTarget,
  IN UINT32   VgaTarget,
  IN UINT32   SourceId,
  IN UINT32   SourceIdEnable
  );

/**
  CHA: Configure the Local socket ID

  @param SocId           - CPU Socket Node number (Socket ID)
  @param BoxInst         - Box Instance, 0 based
  @param LocalSocketId   - Local Socket Id to be configured

  @retval                - EFI_SUCCESS: success
                         - EFI_UNSUPPORTED: unsupported register on this HW IP
                         - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureLocalSocketId (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    LocalSocketId
  );

//
// Function pointer definition for ChaConfigureLocalSocketId()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_LOCAL_SOCKET_ID) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    LocalSocketId
  );

/**
  CHA: Get LT Target and Enabled status

  @param SocId           - CPU Socket Node number (Socket ID)
  @param BoxInst         - Box Instance, 0 based
  @param LtTarget        - LT Target         : 0xFFFFFFFF: no need to configure
  @param LtTargetEnabled - LT Target Enabled : 0xFFFFFFFF: no need to configure

  @retval                - EFI_SUCCESS: success
                         - EFI_UNSUPPORTED: unsupported register on this HW IP
                         - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaGetLtTargetAndStatus (
  IN     UINT8    SocId,
  IN     UINT8    BoxInst,
  IN OUT UINT32   *LtTarget,
  IN OUT UINT32   *LtTargetEnabled
  );

//
// Function pointer definition for ChaGetLtTargetAndStatus()
//
typedef
EFI_STATUS
(EFIAPI *CHA_GET_LT_TARGET_STATUS) (
  IN     UINT8    SocId,
  IN     UINT8    BoxInst,
  IN OUT UINT32   *LtTarget,
  IN OUT UINT32   *LtTargetEnabled
  );

/**
  CHA: Set LT Target and status

  @param SocId           - CPU Socket Node number (Socket ID)
  @param BoxInst         - Box Instance, 0 based
  @param LtTarget        - LT Target
  @param LtTargetEnabled - LT Target Enabled

  @retval                - EFI_SUCCESS: success
                         - EFI_UNSUPPORTED: unsupported register on this HW IP
                         - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaSetLtTargetAndStatus (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   LtTarget,
  IN UINT32   LtTargetEnabled
  );

//
// Function pointer definition for ChaSetLtTargetAndStatus()
//
typedef
EFI_STATUS
(EFIAPI *CHA_SET_LT_TARGET_STATUS) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   LtTarget,
  IN UINT32   LtTargetEnabled
  );

/**
  CHA: Get Mmcfg Target List

  @param  SocId           - CPU Socket Node number (Socket ID)
  @param  BoxInst         - Box Instance, 0 based
  @param  MmcfgTargetList - Mmcfg Target List

  @retval                 - EFI_SUCCESS: success
                          - EFI_UNSUPPORTED: unsupported register on this HW IP
                          - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaGetMmcfgTargetList (
  IN     UINT8   SocId,
  IN     UINT8   BoxInst,
  IN OUT UINT32  *MmcfgTargetList
  );

//
// Function pointer definition for ChaGetMmcfgTargetList()
//
typedef
EFI_STATUS
(EFIAPI *CHA_GET_MMCFG_TARGET_LIST) (
  IN     UINT8   SocId,
  IN     UINT8   BoxInst,
  IN OUT UINT32  *MmcfgTargetList
  );

/**
  CHA: Set Mmcfg Target List

  @param SocId             - CPU Socket Node number (Socket ID)
  @param BoxInst           - Box Instance, 0 based
  @param MmCfgTargetList   - Mmcfg Target List to be set

  @retval                  - EFI_SUCCESS: success
                           - EFI_UNSUPPORTED: unsupported register on this HW IP
                           - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaSetMmcfgTargetList (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   MmCfgTargetList
  );

//
// Function pointer definition for ChaSetMmcfgTargetList()
//
typedef
EFI_STATUS
(EFIAPI *CHA_SET_MMCFG_TARGET_LIST) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   MmCfgTargetList
  );

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

  @retval                - EFI_SUCCESS: success
                         - EFI_UNSUPPORTED: unsupported register on this HW IP
                         - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureMmcfgRules (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    EnableRule,
  IN UINT32   MmcfgLength,
  IN UINT32   ValidTarget,
  IN UINT32   BaseAddrLow,
  IN UINT32   BaseAddrHi,
  IN UINT32   *LocalRuleCfg,
  IN UINT32   *LocalRuleAddr
  );

//
// Function pointer definition for ChaConfigureMmcfgRules()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_MMCFG_RULES) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    EnableRule,
  IN UINT32   MmcfgLength,
  IN UINT32   ValidTarget,
  IN UINT32   BaseAddrLow,
  IN UINT32   BaseAddrHi,
  IN UINT32   *LocalRuleCfg,
  IN UINT32   *LocalRuleAddr
  );

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
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   *MmCfgTargetList,
  IN UINT32   *MmcfgRule,
  IN UINT32   LocalRuleAddr[]
  );

//
// Function pointer definition for ChaGetMmcfgTargetListAndRules()
//
typedef
EFI_STATUS
(EFIAPI *CHA_GET_MMCFG_TARGET_LIST_RULES) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   *MmCfgTargetList,
  IN UINT32   *MmcfgRule,
  IN UINT32   LocalRuleAddr[]
  );

/**
  CHA: Set Mmcfg Rule

  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxInst   - Box Instance, 0 based
  @param MmcfgRule - Mmcfg Rule

  @retval          - EFI_SUCCESS: success
                   - EFI_UNSUPPORTED: unsupported register on this HW IP
                  - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaSetMmcfgRule (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   MmcfgRule
  );

//
// Function pointer definition for ChaSetMmcfgRule()
//
typedef
EFI_STATUS
(EFIAPI *CHA_SET_MMCFG_RULES) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   MmcfgRule
  );


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
  );

//
// Function pointer definition for ChaSetMmcfgTargetListAndRules()
//
typedef
EFI_STATUS
(EFIAPI *CHA_SET_MMCFG_TARGET_LIST_RULES) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   MmCfgTargetList,
  IN UINT32   MmcfgRule,
  IN UINT32   LocalRuleAddr[]
  );

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
  );

//
// Function pointer definition for ChaSetTempBusNumAndMmcfg()
//
typedef
EFI_STATUS
(EFIAPI *CHA_SET_TEMP_BUSNUM_MMCFG) (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT UINT32   *MmCfgTargetList,
  OUT UINT32   *MmcfgRule,
  OUT UINT32   LocalRuleAddr0[]
  );

/**
  CHA: Get Mmcfg Valid Targets

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param MmcfgRuleValidTarget - Mmcfg valid targets

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaGetMmcfgRuleValidTarget (
  IN  UINT8   SocId,
  IN  UINT8   BoxInst,
  OUT UINT32  *MmcfgRuleValidTarget
  );

//
// Function pointer definition for ChaGetMmcfgRuleValidTarget()
//
typedef
EFI_STATUS
(EFIAPI *CHA_GET_MMCFG_RULE_VALID_TARGET) (
  IN  UINT8   SocId,
  IN  UINT8   BoxInst,
  OUT UINT32  *MmcfgRuleValidTarget
  );


/**
  CHA: Set Mmcfg Rule Valid Target

  @param SocId       - CPU Socket Node number (Socket ID)
  @param BoxInst     - Box Instance, 0 based
  @param ValidTarget - Mmcfg Rule Valid Target

  @retval            - EFI_SUCCESS: success
                     - EFI_UNSUPPORTED: unsupported register on this HW IP
                     - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaSetMmcfgRuleValidTarget (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   ValidTarget
  );

//
// Function pointer definition for ChaSetMmcfgRuleValidTarget()
//
typedef
EFI_STATUS
(EFIAPI *CHA_SET_MMCFG_RULE_VALID_TARGET) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   ValidTarget
  );


/**
  CHA: Configure IO Port Target List

  @param SocId                    - CPU Socket Node number (Socket ID)
  @param IoTgtLst                 - IO Target List

  @retval                         - EFI_SUCCESS: success
                                  - EFI_UNSUPPORTED: unsupported register on this HW IP
                                  - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureIoPortTargetList (
  IN UINT8    SocId,
  IN UINT32   IoTgtLst[MAX_SAD_TARGETS]
  );

//
// Function pointer definition for ChaConfigureIoPortTargetList()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_IOPORT_TARGET_LIST) (
  IN UINT8    SocId,
  IN UINT32   IoTgtLst[MAX_SAD_TARGETS]
  );


/**
  CHA: Configure IO APIC Target List

  @param SocId                    - CPU Socket Node number (Socket ID)
  @param IoApicTgtLst             - IO APIC Target List

  @retval                         - EFI_SUCCESS: success
                                  - EFI_UNSUPPORTED: unsupported register on this HW IP
                                  - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureIoApicTargetList (
  IN UINT8    SocId,
  IN UINT32   IoApicTgtLst [MAX_SAD_TARGETS]
  );

//
// Function pointer definition for ChaConfigureIoApicTargetList()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_IOAPIC_TARGET_LIST) (
  IN UINT8    SocId,
  IN UINT32   IoApicTgtLst [MAX_SAD_TARGETS]
  );


/**
  CHA:  Get MMIOH Interleave Target List

  @param SocId                               - CPU Socket Node number (Socket ID)
  @param BoxInst                             - Box Instance, 0 based
  @param MmiohTargetInterleaveListRegIndex   - MMIOH Interleave List Register Index, 0 based
  @param MmiohTargetInterleaveList           - MMIOH Interleave List

  @retval                                    - EFI_SUCCESS: success
                                             - EFI_UNSUPPORTED: unsupported register on this HW IP
                                             - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaGetMmiohTargetInterleaveList (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  IN  UINT8    MmiohTargetInterleaveListRegIndex,
  OUT UINT32   *MmiohTargetInterleaveList
  );

//
// Function pointer definition for ChaGetMmiohTargetInterleaveList()
//
typedef
EFI_STATUS
(EFIAPI *CHA_GET_MMIOH_TARGET_INTERLEAVE_LIST) (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  IN  UINT8    MmiohTargetInterleaveListRegIndex,
  OUT UINT32   *MmiohTargetInterleaveList
  );


/**
  CHA: Configure MMIOH Target Interleave List

  @param SocId                             - CPU Socket Node number (Socket ID)
  @param BoxInst                           - Box Instance, 0 based
  @param MmiohTargetInterleaveListRegIndex - MMIOH Target Interleave List Register Index, 0 based
  @param MmiohTargetInterleaveList         - MMIOH Target Interleave List

  @retval                                  - EFI_SUCCESS: success
                                           - EFI_UNSUPPORTED: unsupported register on this HW IP
                                           - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureMmiohTargetInterleaveList (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    MmiohTargetInterleaveListRegIndex,
  IN UINT32   MmiohTargetInterleaveList
  );

//
// Function pointer definition for ChaConfigureMmiohTargetInterleaveList()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_MMIOH_TARGET_INTERLEAVE_LIST) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    MmiohTargetInterleaveListRegIndex,
  IN UINT32   MmiohTargetInterleaveList
  );

/**
  CHA: Configure NEM RULE Base

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param MmioNemRuleBase      - MMIO NEM Rule Base

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureNemRuleBase (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   MmioNemRuleBase
  );

//
// Function pointer definition for ChaConfigureNemRuleBase()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_NEM_RULE_BASE) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   MmioNemRuleBase
  );


/**
  CHA: Replicate ME Seg registers from SBSP to to be online PBSP

  @param SbspSocId           - SBSP Node number (Socket ID)
  @param BoxInst             - Box Instance, 0 based
  @param TobeOnlinePbspSocId - to be online PBSP Node number

  @retval                    - EFI_SUCCESS: success
                             - EFI_UNSUPPORTED: unsupported register on this HW IP
                             - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaRasReplicateMeSeg (
  IN UINT8    SbspSocId,
  IN UINT8    BoxInst,
  IN UINT8    TobeOnlinePbspSocId
  );

//
// Function pointer definition for ChaRasReplicateMeSeg()
//
typedef
EFI_STATUS
(EFIAPI *CHA_RAS_REPLICATE_ME_SEG) (
  IN UINT8    SbspSocId,
  IN UINT8    BoxInst,
  IN UINT8    TobeOnlinePbspSocId
  );


/**
  CHA: Replicate DRAM Settings from SBSP to to be online PBSP

  @param SbspSocId           - SBSP Node number (Socket ID)
  @param BoxInst             - Box Instance, 0 based
  @param TobeOnlinePbspSocId - to be online PBSP Node number

  @retval                    - EFI_SUCCESS: success
                             - EFI_UNSUPPORTED: unsupported register on this HW IP
                             - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaRasReplicateDramSetting (
  IN UINT8    SbspSocId,
  IN UINT8    BoxInst,
  IN UINT8    TobeOnlinePbspSocId
  );

//
// Function pointer definition for ChaRasReplicateDramSetting()
//
typedef
EFI_STATUS
(EFIAPI *CHA_RAS_REPLICATE_DRAM_SETTING) (
  IN UINT8    SbspSocId,
  IN UINT8    BoxInst,
  IN UINT8    TobeOnlinePbspSocId
  );

/**
  CHA: Get the Remote Stack List

  @param SocId                  - CPU Socket Node number (Socket ID)
  @param MaxSocket              - Max Socket to support
  @param *SocketData            - Socket Data
  @param CpuSktHotPlugEn        - CPU Socket Hot Plug enabled
  @param MmiohTargetSize        - Mmioh Target Size
  @param UseNonInterleavedMmioh - Use non-Interleaved Mmioh
  @param *MmiohRemStackTgtLst   - Array of UINT32 Remote Stack Target Lists, size = [MAX_SAD_TARGETS / 8]

  @retval                       - EFI_UNSUPPORTED: the function not implemented
                                  EFI_SUCCESS: the function is excuted successfully
**/
EFI_STATUS
EFIAPI
ChaGetRemStackTgtLst (
  IN  UINT8           SocId,
  IN  UINT8           MaxSocket,
  IN  KTI_SOCKET_DATA *SocketData,
  IN  UINT8           CpuSktHotPlugEn,
  IN  UINT16          MmiohTargetSize,
  IN  BOOLEAN         UseNonInterleavedMmioh,
  OUT UINT32          *MmiohRemStackTgtLst
);
//
// Function pointer definition for ChaConfigureCpuMmiohEntry()
//
typedef
EFI_STATUS
(EFIAPI *CHA_GET_REM_STACK_TGT_LIST) (
  IN  UINT8           SocId,
  IN  UINT8           MaxSocket,
  IN  KTI_SOCKET_DATA *SocketData,
  IN  UINT8           CpuSktHotPlugEn,
  IN  UINT16          MmiohTargetSize,
  IN  BOOLEAN         UseNonInterleavedMmioh,
  OUT UINT32          *MmiohRemStackTgtLst
);

/**
  CHA: Configure MMIOH Rule Entry

  @param SocId                  - CPU Socket Node number (Socket ID)
  @param MaxSocket              - Max Socket to support
  @param CpuSktHotPlugEn        - CPU Socket Hot Plug enabled
  @param MmiohTargetSize        - Mmioh Target Size
  @param UseNonInterleavedMmioh - Use non-Interleaved Mmioh

  @retval                       - EFI_SUCCESS: success
                                - EFI_UNSUPPORTED: unsupported register on this HW IP
                                - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureCpuMmiohEntry (
  IN UINT8           SocId,
  IN UINT8           MaxSocket,
  IN KTI_SOCKET_DATA *SocketData,
  IN UINT8           CpuSktHotPlugEn,
  IN UINT16          MmiohTargetSize,
  IN BOOLEAN         UseNonInterleavedMmioh
  );

//
// Function pointer definition for ChaConfigureCpuMmiohEntry()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_CPU_MMIOH_ENTRY) (
  IN UINT8           SocId,
  IN UINT8           MaxSocket,
  IN KTI_SOCKET_DATA *SocketData,
  IN UINT8           CpuSktHotPlugEn,
  IN UINT16          MmiohTargetSize,
  IN BOOLEAN         UseNonInterleavedMmioh
  );


/**
  CHA: Configure System snoop filter for FPGA

  @param CpuList              - List of CPUs populated in the system
  @param SocketData           - Socket specific data structure

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureSnoopFilterForFpga (
  IN UINT32                     CpuList,
  IN KTI_SOCKET_DATA            *SocketData
  );

//
// Function pointer definition for ChaConfigureSnoopFilterForFpga()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_SNOOP_FILTER_FOR_FPGA) (
  IN UINT32                     CpuList,
  IN KTI_SOCKET_DATA            *SocketData
  );


/**
  CHA: Populate the specific destination socket field in the requested SocID's
  KTI Routing Table.

  @param[in] SocId                CPU Socket Node number (Socket ID)
  @param[in] ChaCount             Total number of CHAs for socket "SocId"
  @param[in] FieldMask            Clear the entry (or entries) of the desination socket
  @param[in] PortSetting          Values to be written into entry (or entries) of destination socket

  @retval  EFI_SUCCESS            CHA KTI_ROUTING_TABLE register was updated successfully.
           EFI_UNSUPPORTED        Unsupported register on this HW IP
           EFI_INVALID_PARAMETER  Invalid parameter

**/
EFI_STATUS
EFIAPI
ChaProgramRouteTableForIndividualDestNodeId (
  IN UINT8    SocId,
  IN UINT8    ChaCount,
  IN UINT64   FieldMask,
  IN UINT64   PortSetting
  );

//
// Function pointer definition for ChaProgramRouteTableForIndividualDestNodeId ()
//
typedef
EFI_STATUS
(EFIAPI *CHA_PROG_KTIRT_FOR_INDIVIDUAL_DEST_NODE_ID) (
  IN UINT8    SocId,
  IN UINT8    ChaCount,
  IN UINT64   FieldMask,
  IN UINT64   PortSetting
  );


/**
  CHA: Program KTI routing table register(s)

  @param[in] SocId                      CPU Socket Node number (Socket ID)
  @param[in] ChaCount                   Total number of CHAs for socket "SocId"
  @param[in] SysConfig                  Topology type
  @param[in] DualLinksInterleavingMode  Dual Links Interleaving Mode
  @param[in] SbspSktId                  SBSP CPU Node ID
  @param[in] SocketData                 Socket specific data structure
  @param[in] UpiRoutingMode             UPI routing mode, see UPI_ROUTING_MODE enum.

  @retval  EFI_SUCCESS            CHA KTI_ROUTING_TABLE register(s) was updated successfully.
           EFI_UNSUPPORTED        Unsupported register on this HW IP
           EFI_INVALID_PARAMETER  Invalid parameter

**/
EFI_STATUS
EFIAPI
ChaProgramFullKtiRouteTable (
  IN UINT8              SocId,
  IN UINT8              ChaCount,
  IN UINT8              SysConfig,
  IN UINT8              DualLinksInterleavingMode,
  IN UINT8              SbspSktId,
  IN KTI_SOCKET_DATA    *SocketData,
  IN UINT8              UpiRoutingMode
  );

//
// Function pointer definition for ChaProgramFullKtiRouteTable()
//
typedef
EFI_STATUS
(EFIAPI *CHA_PROG_FULL_KTI_ROUTE_TABLE) (
  IN UINT8              SocId,
  IN UINT8              ChaCount,
  IN UINT8              SysConfig,
  IN UINT8              DualLinksInterleavingMode,
  IN UINT8              SbspSktId,
  IN KTI_SOCKET_DATA    *SocketData,
  IN UINT8              UpiRoutingMode
  );


/**
  CHA: Programm SNC Configuration

  @param SocId                 - CPU Socket Node number (Socket ID)
  @param BoxInst               - Box Instance, 0 based
  @param ChaCount              - Total CHA Number
  @param Operation             - Operation type: Normal or Recovery operation
  @param UmaEn                 - UMA based clustering is enabled
  @param NumOfCluster          - Number Of Cluster
  @param BaseChaCluster1       - Number Of CHA per cluster 1
  @param BaseChaCluster2       - Number Of CHA per cluster 2
  @param BaseChaCluster3       - Number Of CHA per cluster 3
  @param SncConfiguration      - SNC configuration register value

  @retval                      - EFI_SUCCESS: success
                               - EFI_UNSUPPORTED: unsupported register on this HW IP
                               - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaProgramSncConfig (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    ChaCount,
  IN UINT8    Operation,
  IN BOOLEAN  UmaEn,
  IN UINT8    NumOfCluster,
  IN UINT8    BaseChaCluster1,
  IN UINT8    BaseChaCluster2,
  IN UINT8    BaseChaCluster3,
  OUT UINT32  *SncConfiguration
  );

//
// Function pointer definition for ChaProgramSncConfig()
//
typedef
EFI_STATUS
(EFIAPI *CHA_PROGRAMM_SNC_CONFIG) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    ChaCount,
  IN UINT8    Operation,
  IN BOOLEAN  UmaEn,
  IN UINT8    NumOfCluster,
  IN UINT8    BaseChaCluster1,
  IN UINT8    BaseChaCluster2,
  IN UINT8    BaseChaCluster3,
  OUT UINT32  *SncConfiguration
  );


/**
  CHA: Programm credits to Cha -> M2UPI

  @param SocId                 - CPU Socket Node number (Socket ID)
  @param BoxInst               - Box Instance, 0 based
  @param AdVnaCreditPerCha     - AD VNA credit per CHA
  @param BlVnaCreditPerCha     - BL VNA credit per CHA

  @retval                      - EFI_SUCCESS: success
                               - EFI_UNSUPPORTED: unsupported register on this HW IP
                               - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureChaToM2UpiCredit (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   AdVnaCreditPerCha,
  IN UINT32   BlVnaCreditPerCha
  );

//
// Function pointer definition for ChaConfigureChaToM2UpiCredit()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_CHA_TO_M2UPI_CREDIT) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   AdVnaCreditPerCha,
  IN UINT32   BlVnaCreditPerCha
  );


/**
  CHA: Programm credits to Cha -> M2PCIe

  @param SocId                   - CPU Socket Node number (Socket ID)
  @param BoxInst                 - Box Instance, 0 based
  @param BlCreditsToM2pciePerCha - BL Credits to M2PCIe per CHA

  @retval                        - EFI_SUCCESS: success
                                 - EFI_UNSUPPORTED: unsupported register on this HW IP
                                 - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureChaToM2PCIeCredit (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   BlCreditsToM2pciePerCha
  );

//
// Function pointer definition for ChaConfigureChaToM2PCIeCredit()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_CHA_TO_M2PCIE_CREDIT) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   BlCreditsToM2pciePerCha
  );
/**
  CHA: Programm credits to Cha -> M2IAL AD VNA

  @param SocId                   - CPU Socket Node number (Socket ID)
  @param BoxInst                 - Box Instance, 0 based
  @param AdCreditsToM2pciePerCha - Ad Credits to M2IAL per CHA
  @param HbmSku                  - Hbm Sku Flag

  @retval                        - EFI_SUCCESS: success
                                 - EFI_UNSUPPORTED: unsupported register on this HW IP
                                 - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureChaToM2IalAdVnaCredit (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   AdCreditsToM2pciePerCha,
  IN BOOLEAN  HbmSku
  );

//
// Function pointer definition for ChaConfigureChaToM2IalCredit()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_CHA_TO_M2IAL_AD_VNA_CREDIT) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   AdCreditsToM2pciePerCha,
  IN BOOLEAN  HbmSku
  );
/**
  CHA: Programm credits to Cha -> Ubox

  @param SocId                 - CPU Socket Node number (Socket ID)
  @param BoxInst               - Box Instance, 0 based
  @param BlVnaCreditPerCha     - BL VNA credit per CHA

  @retval                      - EFI_SUCCESS: success
                               - EFI_UNSUPPORTED: unsupported register on this HW IP
                               - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureChaToUboxCredit (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   BlVnaCreditPerCha
  );

//
// Function pointer definition for ChaConfigureChaToUboxCredit()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_CHA_TO_UBOX_CREDIT) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   BlVnaCreditPerCha
  );

/**
  CHA: Enable Isoc in CHA

  @param SocId                 - CPU Socket Node number (Socket ID)
  @param BoxInst               - Box Instance, 0 based

  @retval                      - EFI_SUCCESS: success
                               - EFI_UNSUPPORTED: unsupported register on this HW IP
                               - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaEnableIsocInCha (
  IN UINT8    SocId,
  IN UINT8    BoxInst
  );

//
// Function pointer definition for ChaEnableIsocInCha()
//
typedef
EFI_STATUS
(EFIAPI *CHA_ENABLE_ISOC_IN_CHA) (
  IN UINT8    SocId,
  IN UINT8    BoxInst
  );

/**
  CHA: Configure Demand Priority Local

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param EnableDemandPriority - Enable demand priority

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureDemandPriorityLocal (
  UINT8    SocId,
  UINT8    BoxInst,
  BOOLEAN  EnableDemandPriority
  );

//
// Function pointer definition for ChaConfigureDemandPriorityLocal()
//
typedef
EFI_STATUS
(EFIAPI *CHA_ENABLE_DEMAND_PRIORITY_LOCAL) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  BOOLEAN  EnableDemandPriority
  );

/**
  CHA: Configure Demand Priority Remote

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param EnableDemandPriority - Enable demand priority

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureDemandPriorityRemote (
  UINT8    SocId,
  UINT8    BoxInst,
  BOOLEAN  EnableDemandPriority
  );

//
// Function pointer definition for ChaConfigureDemandPriorityRemote()
//
typedef
EFI_STATUS
(EFIAPI *CHA_ENABLE_DEMAND_PRIORITY_REMOTE) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  BOOLEAN  EnableDemandPriority
  );

/**
  CHA: Configure KTI Fanout

  @param[in] SocId                CPU Socket Node number (Socket ID)
  @param[in] BoxInst              Box Instance, 0 based
  @param[in] ChaKtiFanOut         CHA KTI Fanout value to be written into CHA_RTR_SNP_CTL0 register
  @param[in] ChaKtiFanOut1        CHA KTI Fanout value to be written into CHA_RTR_SNP_CTL1 register.
                                  This field is only applicable to SOC that supports 6 UPI links.

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureSnoopFanout (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   ChaKtiFanOut,
  IN UINT32   ChaKtiFanOut1
  );

//
// Function pointer definition for ChaConfigureSnoopFanout()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_SNOOP_FANOUT) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   ChaKtiFanOut,
  IN UINT32   ChaKtiFanOut1
  );


/**
  CHA: Configure HA - Disable Egress Isoc Credit Reservation

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param DisEgressIsocRes     - Disable Egress Isoc credit reservation


  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaDisableEgressIsocRes (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    DisEgressIsocRes
  );

//
// Function pointer definition for ChaDisableEgressIsocRes()
//
typedef
EFI_STATUS
(EFIAPI *CHA_DISABLE_EGRESS_ISOC_RES) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    DisEgressIsocRes
  );


/**
  CHA: Configure HA Coherency TOR

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param HitMEEnable          - Enable HitME cache overloading. HitME$
                                should be enabled only when directory is
                                enabled.
  @param HitMERfoDirs         - Enable HitME DIR=S RFO optimization. This
                                optimization allows snoops to be broadcast in
                                parallel with memory lookup for RFO HitS in the
                                local CA.
  @param HitMEShar            - HitME cache allows caching of SHARed lines
  @param Is2SConfig           - 2S config flag

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureHaCohTor (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   HitMEEnable,
  IN UINT32   HitMERfoDirs,
  IN UINT32   HitMEShar,
  IN UINT8    Is2SConfig
  );

//
// Function pointer definition for ChaConfigureHaCohTor()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_HA_COH_TOR) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   HitMEEnable,
  IN UINT32   HitMERfoDirs,
  IN UINT32   HitMEShar,
  IN UINT8    Is2SConfig
  );

/**
  CHA: Configure HA Coherency1

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param HaMiniBypassDis      - HA Mini Bypass mode. When a Read request gets
                                read out of the TORReqQ and there is not
                                anything ahead of it in the HApipe, the read
                                will be issued right away saving 2 cycles.
  @param StaleAtoSOptEn       - A to S directory optimization. When RdData*
                                finds DIR=A and all snoop responses received are
                                RspI, then directory is moved to S and data is
                                returned in S-state. This optimization will not
                                be effective in xNC configuration where BuriedM
                                is possible.
  @param GateOsbIodcAlloc     - When OSB indicates that there arent enough snoop
                                credits dont allocate IODC entry
  @param Is2SConfig           - 2S config flag

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureHaCoh1 (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   HaMiniBypassDis,
  IN UINT32   StaleAtoSOptEn,
  IN UINT32   GateOsbIodcAlloc,
  IN UINT8    Is2SConfig
  );

//
// Function pointer definition for ChaConfigureHaCoh1()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_HA_COH1) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   HaMiniBypassDis,
  IN UINT32   StaleAtoSOptEn,
  IN UINT32   GateOsbIodcAlloc,
  IN UINT8    Is2SConfig
  );

/**
  CHA: Configure StaleAtoSOptEn

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param StaleAtoSOptEn       - A to S directory optimization. When RdData*
                                finds DIR=A and all snoop responses received are
                                RspI, then directory is moved to S and data is
                                returned in S-state. This optimization will not
                                be effective in xNC configuration where BuriedM
                                is possible.

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureStaleAtoSOptEn (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   StaleAtoSOptEn
  );

//
// Function pointer definition for ChaConfigureStaleAtoSOptEn()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_STALE_ATOS_OPT_EN) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   StaleAtoSOptEn
  );


/**
  CHA: Configure OSB Settings

  @param SbspSocId            - SBSP Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param IodcEnable           - Enabled IODC. IODC can be enabled only in
                                DIRECTORY enabled configurations.
  @param DisDirectory         - Disable Directory. Directory should be
                                disabled for single socket configurations.
                                Directory should be enabled for all multi-socket
                                configurations.
  @param SysOsbEn             - System OSB enable or not
  @param SysConfig            - System Configure
  @param Is4SRing             - Is 4S Ring or not
  @param Is4SFC               - Is 4S full connection

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureOsbSetting (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   IodcEnable,
  IN UINT32   DisDirectory,
  IN UINT8    SysOsbEn,
  IN UINT32   SysConfig,
  IN UINT32   Is4SRing,
  IN UINT32   Is4SFC
  );

//
// Function pointer definition for ChaConfigureOsbSetting()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_OSB_SETTING) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   IodcEnable,
  IN UINT32   DisDirectory,
  IN UINT8    SysOsbEn,
  IN UINT32   SysConfig,
  IN UINT32   Is4SRing,
  IN UINT32   Is4SFC
  );


/**
  CHA: Configure HA Coherency to Enable KTI, ActiveNode and DataM

  @param[in] SbspSocId            - SBSP Node number (Socket ID)
  @param[in] BoxInst              - Box Instance, 0 based
  @param[in] ActiveNodeMask       - Active Nodes In System. One bit per Node ID of
                                    caching agents in the system. It is used to
                                    control which node snoops are spawned to and
                                    responses are expected from. Default value is
                                    NodeID_0 and NodeID_1 active.
  @param[in] KtiEnabledMask       - Bitmask indicating which UPI link(s) is enabled.
                                    This is used by OSB logic to determine if a spcific
                                    UPI link is enabled. Bit-0 indicates UPI Link 0,
                                    Bit-1 indicates UPI link 1, ..., Bit-5 indicates UPI
                                    link 5.
  @param[in] RdInvOwnAcceptSm     - Allows Data_M in the local CA without WB to
                                    memory for local RdInvOwn receiving Rsp*IWb.
                                    Expect to be set to 1 in multi-socket systems.
  @param[in] RdDataMigAcceptSm    - Allows Data_M in the local CA without WB to
                                    memory for local RdDataMig receiving Rsp*IWb.
                                    Expect to be set to 1 in multi-socket systems.

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureHaCohKtiEnableActiveNodeAndDataM (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   ActiveNodeMask,
  IN UINT32   KtiEnabledMask,
  IN UINT32   RdInvOwnAcceptSm,
  IN UINT32   RdDataMigAcceptSm
  );

//
// Function pointer definition for ChaConfigureHaCohKtiEnableActiveNodeAndDataM()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_HA_COH_KTI_ENABLE_ACTIVE_NODE_DATAM) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   ActiveNodeMask,
  IN UINT32   KtiEnabledMask,
  IN UINT32   RdInvOwnAcceptSm,
  IN UINT32   RdDataMigAcceptSm
  );

/**
  CHA: Check whether Overall Two LM Enable

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param TwoLMEnabled         - TRUE: 2LM enabled; FALSE: 2LM not enabled

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaCheck2LMEnabled (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* TwoLMEnabled
  );

//
// Function pointer definition for ChaCheck2LMEnabled()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CHECK_2LM_ENABLED) (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* TwoLMEnabled
  );


/**
  CHA: Configure OSB Power Heuristics

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigurePowerHeuristics (
  IN UINT8    SocId,
  IN UINT8    BoxInst
  );

//
// Function pointer definition for ChaConfigurePowerHeuristics()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_PWR_HEURISTICS) (
  IN UINT8    SocId,
  IN UINT8    BoxInst
  );


/**
  CHA: Reprogram bus/mmcfg on PBSP sockets

  @param SocId        - CPU Socket Node number (Socket ID)
  @param BoxInst      - Box Instance, 0 based
  @param MmcfgLength  - Mmcfg length
  @param Targets      - Mmcfg valid target
  @param Base_address - Mmcfg rule base address

  @retval             - EFI_SUCCESS: success
                      - EFI_UNSUPPORTED: unsupported register on this HW IP
                      - EFI_INVALID_PARAMETER: invalid parameter
**/
EFI_STATUS
EFIAPI
ReprogramPbspBusmmcfg (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    MmcfgLength,
  IN UINT8    Targets,
  IN UINT32   Base_address
  );
//
// Function pointer definition for ReprogramPbspBusmmcfg()
//
typedef
EFI_STATUS
(EFIAPI *REPROGRAM_PBSP_BUS_MMCFG) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    MmcfgLength,
  IN UINT8    Targets,
  IN UINT32   Base_address
  );

/**
  Enable mmcfg on PBSP sockets

  @param SocId        - CPU Socket Node number (Socket ID)

  @retval EFI_SUCCESS            The function is executed successfully without any error.
**/
EFI_STATUS
EFIAPI
EnablePbspBusmmcfgInCHA (
  UINT8    SocId
  );

//
// 10nm only end
//

//
// SKX only start
//
/**
  CHA: Configure ISOC Mode

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param IsocEnable           - Enable ISOC mode. This will be used for TOR
                                pipeline to reserve TOR entries for ISOC

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureIsoc (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  OUT BOOLEAN  IsocEnable
  );

//
// Function pointer definition for ChaConfigureIsoc()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_ISOC) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  OUT BOOLEAN  IsocEnable
  );


/**
  CHA: Use PA6 xor PA12 value to decide the Intel UPI link to use in 2S-2K case

  @param SocId                 - CPU Socket Node number (Socket ID)
  @param BoxInst               - Box Instance, 0 based
  @param EnableXorBasedKtiLink - Use PA6 xor PA12 value to decide the Intel UPI
                                 link to use in 2S-2K case

  @retval                      - EFI_SUCCESS: success
                               - EFI_UNSUPPORTED: unsupported register on this HW IP
                               - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaCboCohEnableXorBasedKtilink (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   EnableXorBasedKtiLink
  );

/**
  CHA: Set CBO SwapKtiLinkIntlv bit to support SKX/CLX twisted board topology.
  This function is only applicable to SKX/CLX and not CPX.

  @param SocId                 - CPU Socket Node number (Socket ID)
  @param BoxInst               - Box Instance, 0 based
  @param SwapKtiLinkIntlv      - This field is only applicable to SKX/CLX.
                                 Set to 1 to support twisted board topologies where link0 on one socket is
                                 connected to link1 on the other socket in 2S-2Link configurations and with
                                 EnableXorBasedKTILink set.

  @retval                      - EFI_UNSUPPORTED: the function not implemented
                                 EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
ChaCboCohSwapKtiLinkIntlv (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   SwapKtiLinkIntlv
  );

//
// Function pointer definition for ChaCboCohEnableXorBasedKtilink()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CBO_COH_ENABLE_XOR_BASED_KTI_LINK) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   EnableXorBasedKtiLink
  );


/**
  CHA: Configure Cbo Coherency

  @param SocId                 - CPU Socket Node number (Socket ID)
  @param BoxInst               - Box Instance, 0 based
  @param FpgaList              - FPGA List
  @param SbspSoc               - SBSP socket Node number

  @retval                      - EFI_SUCCESS: success
                               - EFI_UNSUPPORTED: unsupported register on this HW IP
                               - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureCboCoh (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   FpgaList,
  IN UINT8    SbspSoc
  );

//
// Function pointer definition for ChaConfigureCboCoh()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_CBO_COH) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   FpgaList,
  IN UINT8    SbspSoc
  );

/**
  CHA: configure Cbo Coherency 1 to cover IPQ disable and EnablePushMtoIIodcTwoLMOpt
  in this routine.

  @param SocId                       - CPU Socket Node number (Socket ID)
  @param BoxInst                     - Box Instance, 0 based
  @param IpqDisable                  - IPQ disable
  @param EnablePushMtoIIodcTwoLMOpt  - Enable PushMtoI IODC 2LM Optimization.

  @retval                            - EFI_SUCCESS: success
                                     - EFI_UNSUPPORTED: unsupported register on this HW IP
                                     - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureIpqAndEnablePushMtoIIodcTwoLMOpt (
  UINT8     SocId,
  UINT8     BoxInst,
  BOOLEAN   IpqDisable,
  BOOLEAN   EnablePushMtoIIodcTwoLMOpt
  );

//
// Function pointer definition for ChaConfigureIpqAndEnablePushMtoIIodcTwoLMOpt()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CONFIGURE_IPQ_AND_ENABLEPUSHMTOIIODCTWOLMOPT) (
  UINT8     SocId,
  UINT8     BoxInst,
  BOOLEAN   IpqDisable,
  BOOLEAN   EnablePushMtoIIodcTwoLMOpt
  );
/**
  CHA: Configure DBP

  @param SocId                 - CPU Socket Node number (Socket ID)
  @param BoxInst               - Box Instance, 0 based

  @retval                      - EFI_SUCCESS: success
                               - EFI_UNSUPPORTED: unsupported register on this HW IP
                               - EFI_INVALID_PARAMETER: invalid parametery

**/
EFI_STATUS
EFIAPI
ChaConfigureDbp (
  IN UINT8    SocId,
  IN UINT8    BoxInst
  );

//
// Function pointer definition for ChaConfigureDbp()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_DBP) (
  IN UINT8    SocId,
  IN UINT8    BoxInst
  );


/**
  CHA: Configure HOPs Number

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param NumOfHops            - Number of HOPs

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureHopsNum (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   NumOfHops
  );

//
// Function pointer definition for ChaConfigureHopsNum()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_HOPS_NUM) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   NumOfHops
  );


/**
  CHA: Configure Cbsads Unicast Cfg Spare.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param CfgValue             - Configurations

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureCbsadsUnicastCfgSpare (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   CfgValue
  );

//
// Function pointer definition for ChaConfigureCbsadsUnicastCfgSpare()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_CBSADS_UNICAST_CFG_SPARE) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   CfgValue
  );


/**
  CHA: Enable credit change without triggering warm-reset.

  @param SocId                 - CPU Socket Node number (Socket ID)
  @param BoxInst               - Box Instance, 0 based

  @retval                      - EFI_SUCCESS: success
                               - EFI_UNSUPPORTED: unsupported register on this HW IP
                               - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaEnableCreditChange (
  IN UINT8    SocId,
  IN UINT8    BoxInst
  );

//
// Function pointer definition for ChaEnableCreditChange()
//
typedef
EFI_STATUS
(EFIAPI *CHA_ENABLE_CREDIT_CHANGE) (
  IN UINT8    SocId,
  IN UINT8    BoxInst
  );


//
// 10nm only start
//
/**
  CHA: Configure TOR Thresholds: Loc2rem_thresh_norm and Loc2rem_thresh_empty

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param Loc2rem_thresh_norm  - number of local to remote requests allowed in
                                pipeline when remote requests are also present
                                in pipeline
  @param Loc2rem_thresh_empty - number of local to remote requests allowed in
                                pipeline when pipeline is empty of remote
                                requests
  @param Tor_threshold        - remote requests threshold

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureTorThresholds (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   Loc2rem_thresh_norm,
  IN UINT32   Loc2rem_thresh_empty,
  IN UINT32   Tor_threshold
  );
//
// Function pointer definition for ChaConfigureTorThresholds()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_TOR_THRESHOLDS) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   Loc2rem_thresh_norm,
  IN UINT32   Loc2rem_thresh_empty,
  IN UINT32   Tor_threshold
  );
/**
  CHA: Configure DDRT QoS TOR Thresholds.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param DdrtQosMode          - DDRT QoS Mode

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureDDRTQoSTorThresholds (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    DdrtQosMode
  );

//
// Function pointer definition for ChaConfigureDDRTQoSTorThresholds()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_DDRT_QOS_TOR_THRESHOLDS) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    DdrtQosMode
  );


/**
  CHA: Enable KTI Broadcast Route Table

  @param SocId                - CPU Socket Node number (Socket ID)
  @param Enable               - TURE: enable the broadcast, FALSE: disable the broadcast

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaKtiBroadcastRouteTableEnable (
  IN  UINT8    SocId,
  IN  BOOLEAN  Enable
  );

//
// Function pointer definition for ChaKtiBroadcastRouteTableEnable()
//
typedef
EFI_STATUS
(EFIAPI *CHA_KTI_BST_RT_ENABLE) (
  IN  UINT8    SocId,
  IN  BOOLEAN  Enable
  );


/**
  CHA: Configure HA OSB

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param EnableOsbLocRd       - Threshold based enablement of local read OSB
  @param EnableOsbLocRdCur    - Threshold based&nbsp;enablement of local RdCur OSB
  @param EnableOsbRmtRd       - Force enablement of remote read OSB
  @param Is2SConfig           - 2S config flag

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureHaOsbSad (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   EnableOsbLocRd,
  IN UINT32   EnableOsbLocRdCur,
  IN UINT32   EnableOsbRmtRd,
  IN UINT8    Is2SConfig
  );

//
// Function pointer definition for ChaConfigureHaOsbSad()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_HA_OSB_SAD) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   EnableOsbLocRd,
  IN UINT32   EnableOsbLocRdCur,
  IN UINT32   EnableOsbRmtRd,
  IN UINT8    Is2SConfig
  );


/**
  CHA: Configure UBOX MMIO

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param UboxMmioBase         - UBOX MMIO Base
  @param UboxMmioLimit        - UBOX MMIO Limit

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureUboxMmio (
  IN UINT8         SocId,
  IN UINT8         BoxInst,
  IN UINT32        UboxMmioBase,
  IN UINT32        UboxMmioLimit
  );

//
// Function pointer definition for ChaConfigureUboxMmio()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_UBOX_MMIO) (
  IN UINT8         SocId,
  IN UINT8         BoxInst,
  IN UINT32        UboxMmioBase,
  IN UINT32        UboxMmioLimit
  );


/**
  CHA: Programm SNC register in Non-SNC mode

  @param SocId                - CPU Socket Node number (Socket ID)
  @param ChaCount             - Total CHA Number

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaProgrammSncInNonSncMode (
  IN UINT8    SocId,
  IN UINT8    ChaCount
  );

//
// Function pointer definition for ChaProgrammSncInNonSncMode()
//
typedef
EFI_STATUS
(EFIAPI *CHA_PROGRAMM_SNC_IN_NON_SNC_MODE) (
  IN UINT8    SocId,
  IN UINT8    ChaCount
  );


/**
  CHA: Configure CHA to M2M credit without cluster

  @param SocId                    - CPU Socket Node number (Socket ID)
  @param NumOfCha                 - CHA count for socket "SocId"
  @param MaxIMC                   - Max iMC number
  @param AdCreditsToM2mPerCha     - AD credit to M2M per CHA
  @param BlCreditsToM2mPerCha     - BL credit to M2M per CHA
  @param AdCreditsLeftOver        - AD credit leftover
  @param BlCreditsLeftOver        - BL credit leftover
  @param AdCreditMask             - AD Credit Mask
  @param BlCreditMask             - BL Credit Mask

  @retval                         - EFI_SUCCESS: success
                                  - EFI_UNSUPPORTED: unsupported register on this HW IP
                                  - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureChaToM2MCreditWithoutCluster (
  IN UINT8    SocId,
  IN UINT8    NumOfCha,
  IN UINT8    MaxIMC,
  IN UINT32   AdCreditsToM2mPerCha,
  IN UINT32   BlCreditsToM2mPerCha,
  IN UINT8    AdCreditsLeftOver,
  IN UINT8    BlCreditsLeftOver,
  IN UINT32   AdCreditMask,
  IN UINT32   BlCreditMask
  );

//
// Function pointer definition for ChaConfigureChaToM2MCreditWithoutCluster()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_CHA2M2M_CREDIT_WITHOUT_CLUSTER) (
  IN UINT8    SocId,
  IN UINT8    NumOfCha,
  IN UINT8    MaxIMC,
  IN UINT32   AdCreditsToM2mPerCha,
  IN UINT32   BlCreditsToM2mPerCha,
  IN UINT8    AdCreditsLeftOver,
  IN UINT8    BlCreditsLeftOver,
  IN UINT32   AdCreditMask,
  IN UINT32   BlCreditMask
  );


/**
  CHA: Configure CHA to M2M credit with cluster

  @param SocId                    - CPU Socket Node number (Socket ID)
  @param NumOfCha                 - CHA count for socket "SocId"
  @param NumOfCluster             - Cluster Number
  @param NumOfChaPerCluster       - CHA count per cluster
  @param MaxIMC                   - Max iMC number
  @param AdCreditsToM2mPerCha     - AD credit to M2M per CHA
  @param BlCreditsToM2mPerCha     - BL credit to M2M per CHA
  @param NonClusterChaCredit      - Non Cluster CHA Credit
  @param AdCreditsLeftOver        - AD credit leftover
  @param BlCreditsLeftOver        - BL credit leftover
  @param AdCreditMask             - AD Credit Mask
  @param BlCreditMask             - BL Credit Mask

  @retval                         - EFI_SUCCESS: success
                                  - EFI_UNSUPPORTED: unsupported register on this HW IP
                                  - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureChaToM2MCreditWithCluster (
  IN UINT8    SocId,
  IN UINT8    NumOfCha,
  IN UINT8    NumOfCluster,
  IN UINT8    NumOfChaPerCluster,
  IN UINT8    MaxIMC,
  IN UINT32   AdCreditsToM2mPerCha,
  IN UINT32   BlCreditsToM2mPerCha,
  IN UINT32   NonClusterChaCredit,
  IN UINT8    AdCreditsLeftOver,
  IN UINT8    BlCreditsLeftOver,
  IN UINT32   AdCreditMask,
  IN UINT32   BlCreditMask
  );

//
// Function pointer definition for ChaConfigureChaToM2MCreditWithCluster()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_CHA2M2M_WITH_CLUSTER) (
  IN UINT8    SocId,
  IN UINT8    NumOfCha,
  IN UINT8    NumOfCluster,
  IN UINT8    NumOfChaPerCluster,
  IN UINT8    MaxIMC,
  IN UINT32   AdCreditsToM2mPerCha,
  IN UINT32   BlCreditsToM2mPerCha,
  IN UINT32   NonClusterChaCredit,
  IN UINT8    AdCreditsLeftOver,
  IN UINT8    BlCreditsLeftOver,
  IN UINT32   AdCreditMask,
  IN UINT32   BlCreditMask
  );

/**
  CHA: Configure CHA to HBM M2M credit

  @param SocId                    - CPU Socket Node number (Socket ID)
  @param NumOfCha                 - Total Cha number of the Socket
  @param HbmAdClusterCHAsClose    - HBM AD credit to close M2M per CHA
  @param HbmAdClusterCHAsDistant  - HBM AD credit to distant M2M per CHA
  @param HbmBlClusterCHAsClose    - HBM BL credit to close M2M per CHA
  @param HbmBlClusterCHAsDistant  - HBM BL credit to close M2M per CHA

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureChaToHbmM2mCredit (
  IN UINT8    SocId,
  IN UINT8    NumOfCha,
  IN UINT8    HbmAdClusterCHAsClose,
  IN UINT8    HbmAdClusterCHAsDistant,
  IN UINT8    HbmBlClusterCHAsClose,
  IN UINT8    HbmBlClusterCHAsDistant
  );

//
// Function pointer definition for ChaConfigureChaToHbmM2mCredit()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_CHA2HBM_M2M_CREDIT) (
  IN UINT8    SocId,
  IN UINT8    NumOfCha,
  IN UINT8    HbmAdClusterCHAsClose,
  IN UINT8    HbmAdClusterCHAsDistant,
  IN UINT8    HbmBlClusterCHAsClose,
  IN UINT8    HbmBlClusterCHAsDistant
  );

/**
  CHA: Read H0 CH_ROUTE_TABLE

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param NumOfRt              - Number of CH_ROUTE_TABLE
  @param H0ChRouteTable       - H0ChRouteTable

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaH0ChRouteTableRead (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    NumOfRt,
  OUT UINT32* H0ChRouteTable
  );

//
// Function pointer definition for ChaH0ChRouteTableRead()
//
typedef
EFI_STATUS
(EFIAPI *CHA_H0_CH_RT_READ) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    NumOfRt,
  OUT UINT32* H0ChRouteTable
  );


//
// 10nm only end
//

//
// SKX only start
//
/**
  CHA: Configure SNC Attributes

  @param SocId                 - CPU Socket Node number (Socket ID)
  @param BoxInst               - Box Instance, 0 based
  @param FullSncEnable         - full snc enable
  @param SncIndEnable          - SNC IND enable
  @param NumSncClusters        - Num of SNC Clusters
  @param SncMidPointId         - Last ring ID that is in the lower SNC cluster.

  @retval                      - EFI_SUCCESS: success
                               - EFI_UNSUPPORTED: unsupported register on this HW IP
                               - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureSnc (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   FullSncEnable,
  IN UINT32   SncIndEnable,
  IN UINT32   NumSncClusters,
  IN UINT32   SncMidPointId
  );

//
// Function pointer definition for ChaConfigureSnc()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_SNC) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   FullSncEnable,
  IN UINT32   SncIndEnable,
  IN UINT32   NumSncClusters,
  IN UINT32   SncMidPointId
  );

/**
  CHA: Get SNC Attributes

  @param SocId                 - CPU Socket Node number (Socket ID)
  @param BoxInst               - Box Instance, 0 based
  @param SncConfiguration      - SNC Attributes

  @retval                      - EFI_SUCCESS: success
                               - EFI_UNSUPPORTED: unsupported register on this HW IP
                               - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaGetSncConfig (
  IN  UINT8   SocId,
  IN  UINT8   BoxInst,
  OUT UINT32  *SncConfiguration
  );

//
// Function pointer definition for ChaGetSncConfig()
//
typedef
EFI_STATUS
(EFIAPI *CHA_GET_SNC_CONFIG) (
  IN  UINT8   SocId,
  IN  UINT8   BoxInst,
  OUT UINT32  *SncConfiguration
  );


/**
  CHA: Check Xpt Prefetch Disabled status

  @param SocId                    - CPU Socket Node number (Socket ID)
  @param BoxInst                  - Box Instance, 0 based
  @param XptPrefetchDisableStatus - 1: Xpt Prefetch disabled
                                  - 0: Xpt Prefetch not disabled

  @retval                         - EFI_SUCCESS: success
                                  - EFI_UNSUPPORTED: unsupported register on this HW IP
                                  - EFI_INVALID_PARAMETER: invalid parameter
**/
EFI_STATUS
EFIAPI
ChaCheckXptPrefetchDisableStatus (
  IN  UINT8   SocId,
  IN  UINT8   BoxInst,
  OUT UINT8   *XptPrefetchDisableStatus
  );

//
// Function pointer definition for ChaCheckXptPrefetchDisableStatus()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CHECK_XPT_PREFETCH_DISABLE_STATUS) (
  IN  UINT8   SocId,
  IN  UINT8   BoxInst,
  OUT UINT8   *XptPrefetchDisableStatus
  );


/**
  CHA: Dump SNC XPT KTI related register

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaDumpSncXptKtiRegister (
  IN UINT8    SocId,
  IN UINT8    BoxInst
  );

//
// Function pointer definition for ChaDumpSncXptKtiRegister()
//
typedef
EFI_STATUS
(EFIAPI *CHA_DUMP_SNC_XPT_KTI_REG) (
  IN UINT8    SocId,
  IN UINT8    BoxInst
  );


/**
  CHA: Configure XPT Mini SAD table

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param XptMiniSadTable_CHABC- XPT Mini SAD table
  @param Minisadmodeselect    - Mini SAD Mode Select

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureXptMiniSAD (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   XptMiniSadTable_CHABC,
  IN UINT32   Minisadmodeselect
  );

//
// Function pointer definition for ChaConfigureXptMiniSAD()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_XPT_MINI_SAD) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   XptMiniSadTable_CHABC,
  IN UINT32   Minisadmodeselect
  );


/**
  CHA: Enable/Disable Xpt Prefetch

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param EnablePrefetch       - 1: Enable; 0: Disable

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureXptPrefetch (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN BOOLEAN  EnablePrefetch
  );

//
// Function pointer definition for ChaConfigureXptPrefetch()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_XPT_PREFETCH) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN BOOLEAN  EnablePrefetch
  );


/**
  CHA: Set SNC Base Address

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param Cluster              - Cluster Number
  @param ClusterMemBase       - Cluster memory base address

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaSetSncBaseAddress (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    Cluster,
  IN UINT32   ClusterMemBase
  );

//
// Function pointer definition for ChaSetSncBaseAddress()
//
typedef
EFI_STATUS
(EFIAPI *CHA_SET_SNC_BASE_ADDRESS) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    Cluster,
  IN UINT32   ClusterMemBase
  );


/**
  CHA: Get SNC Base Starting Address

  @param SocId                  - CPU Socket Node number (Socket ID)
  @param BoxInst                - Box Instance, 0 based
  @param SocketOwnMemoryBelow4G - TRUE  : The socket owns 0-4G in cold reset path
                                  FALSE : the socket doesn't own 0-4G in cold reset path

  @retval                       - EFI_SUCCESS: success
                                - EFI_UNSUPPORTED: unsupported register on this HW IP
                                - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaCheckSocketOwnMemoryBelow4G (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* SocketOwnMemoryBelow4G
  );

//
// Function pointer definition for ChaCheckSocketOwnMemoryBelow4G()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CHECK_SOCKET_OWN_MEM_BELOW_4G) (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* SocketOwnMemoryBelow4G
  );


/**
  CHA: Check whether 2LM Address Mask is configured

  @param SocId                      - CPU Socket Node number (Socket ID)
  @param BoxInst                    - Box Instance, 0 based
  @param TwoLMAddressMaskConfigured - TRUE  : 2LM Address Mask is configured
                                      FALSE : 2LM Address Mask is not configured

  @retval                           - EFI_SUCCESS: success
                                    - EFI_UNSUPPORTED: unsupported register on this HW IP
                                    - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaCheck2LMAddressMaskConfigured (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* TwoLMAddressMaskConfigured
  );

//
// Function pointer definition for ChaCheck2LMAddressMaskConfigured()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CHECK_2LM_ADD_MASK_CONFIGED) (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* TwoLMAddressMaskConfigured
  );


/**
  CHA: Configure 2LM Address Mask

  @param SocId                 - CPU Socket Node number (Socket ID)
  @param BoxInst               - Box Instance, 0 based
  @param TwoLM_Address_Mask    - Point to 2LM Address Mass struct

  @retval                      - EFI_SUCCESS: success
                               - EFI_UNSUPPORTED: unsupported register on this HW IP
                               - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigure2LMAddressMask (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINTN    *TwoLM_Address_Mask
  );

//
// Function pointer definition for ChaConfigure2LMAddressMask()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_2LM_ADD_MASK) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINTN    *TwoLM_Address_Mask
  );


/**
  CHA: SNC Full enabled status and SNC INC Enabled status

  @param SocId                 - CPU Socket Node number (Socket ID)
  @param BoxInst               - Box Instance, 0 based
  @param SncEnableStatus       - 1: Enabled; 0: Not enabled
  @param SncIndEnabled         - 1: Enabled; 0: Not enabled
  @param SncConfiguration      - Snc configuration register value

  @retval                      - EFI_SUCCESS: success
                               - EFI_UNSUPPORTED: unsupported register on this HW IP
                               - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaGetSncConfiguration (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  IN  UINT8    *SncEnableStatus,
  IN  UINT8    *SncIndEnabled,
  OUT UINT32   *SncConfiguration
  );

//
// Function pointer definition for ChaGetSncConfiguration()
//
typedef
EFI_STATUS
(EFIAPI *CHA_GET_SNC_CONFIGURATION) (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  IN  UINT8    *SncEnableStatus,
  IN  UINT8    *SncIndEnabled,
  OUT UINT32   *SncConfiguration
  );


/**
  CHA: Configure CHA to M2M credits

  @param SocId                    - CPU Socket Node number (Socket ID)
  @param BoxInst                  - Box Instance, 0 based
  @param AdCredit                 - AD Credit to be programmed
  @param BlCredit                 - BL Credit to be programmed
  @param AdCreditMask             - AD Credit Mask
  @param BlCreditMask             - BL Credit Mask
  @param DisableResCreditForIsoch - TRUE: to disable reserving credits for isoch

  @retval                         - EFI_SUCCESS: success
                                  - EFI_UNSUPPORTED: unsupported register on this HW IP
                                  - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureChaToM2MCredit (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   AdCredit,
  IN UINT32   BlCredit,
  IN UINT32   AdCreditMask,
  IN UINT32   BlCreditMask,
  IN BOOLEAN  DisableResCreditForIsoch
  );

//
// Function pointer definition for ChaConfigureChaToM2MCredit()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_CHA2M2M_CREDIT) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   AdCredit,
  IN UINT32   BlCredit,
  IN UINT32   AdCreditMask,
  IN UINT32   BlCreditMask,
  IN BOOLEAN  DisableResCreditForIsoch
  );


/**
  CHA: This is only a W/A - 5331750: TOR TO with LLC victim caused 2-socket system hang when running max local read BW test

  @param SocId                    - CPU Socket Node number (Socket ID)
  @param BoxInst                  - Box Instance, 0 based

  @retval                         - EFI_SUCCESS: success
                                  - EFI_UNSUPPORTED: unsupported register on this HW IP
                                  - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaWAToConfigureHaBlCredit (
  IN UINT8    SocId,
  IN UINT8    BoxInst
  );

//
// Function pointer definition for ChaWAToConfigureHaBlCredit()
//
typedef
EFI_STATUS
(EFIAPI *CHA_WA_CFG_HABL_CREDIT) (
  IN UINT8    SocId,
  IN UINT8    BoxInst
  );

//
// SKX only end
//

//
// CHA API for memory decoder
//

typedef struct _SAD_DATA {
  BOOLEAN   Enabled;
  UINT8     InterleaveMode;
  UINT8     Attribute;
  BOOLEAN   Cacheable;
  UINT32    Limit;           // in 64MB unit
  UINT64    InterleaveList;  // 7nm wave 4 uses full 64 bits
  UINT8     EncMod3AsMod2;   // 14nm only
  UINT8     EncMod3;         // 14nm only
  UINT8     EncMod3Mode;     // 14nm only
  BOOLEAN   DurableMem;      // 10nm wave 3 and 7nm wave 4 only
  BOOLEAN   Nxm;             // 10nm wave 3 and 7nm wave 4 only
  BOOLEAN   CxlAccMem;       // 10nm wave 3 and 7nm wave 4 only
  BOOLEAN   CxlExpMem;       // 10nm wave 3 and 7nm wave 4 only
  BOOLEAN   SingleWay;       // 7nm wave 4 only
  UINT8     TargetChannel;   // 7nm wave 4 only
  UINT8     TargetMc;        // 7nm wave 4 only
} SAD_DATA;

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
  );

//
// Function pointer definition for ChaSetSadData()
//
typedef
EFI_STATUS
(EFIAPI *CHA_SET_SAD_DATA) (
  IN UINT8    SocketId,
  IN UINT8    SadIndex,
  IN SAD_DATA *SadData
  );

typedef struct _REMOTE_SAD_DATA {
  BOOLEAN   Enabled;
  UINT32    Base;
  UINT32    Limit;
  UINT8     SocketId;
  UINT8     Attribute;
  BOOLEAN   CxlMemEnabled;  // If CXL memory is enabled, 10nm wave 3 only
} REMOTE_SAD_DATA;


/**
  Programs CHA remote DRAM rule.

  @param SocketId                The socket number ID and 0 should always be the SBSP.
  @param SadIndex                The index of SAD entry.
  @param RemoteSadData           Logic information for a SAD entry.

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaSetRemoteSadRule (
  IN UINT8           SocketId,
  IN UINT8           SadIndex,
  IN REMOTE_SAD_DATA *RemoteSadData
  );

//
// Function pointer definition for ChaSetRemoteSadRule()
//
typedef
EFI_STATUS
(EFIAPI *CHA_SET_REMOTE_SAD_RULE) (
  IN UINT8           SocketId,
  IN UINT8           SadIndex,
  IN REMOTE_SAD_DATA *RemoteSadData
  );



typedef enum {
  ModeTypeRt0,
  ModeTypeRt1,
  ModeTypeRt2,
  ModeTypeRt3,
  MaxModeType
} MODE_TYPE;

//
// 7nm wave 4 uses the following mapping:
// 7nm Rt0 => ModeTypeRt0, H0
// 7nm Rt1 => ModeTypeRt0, H1
// 7nm Rt2 => ModeTypeRt1, H0
// 7nm Rt3 => ModeTypeRt1, H1
// 7nm Rt4 => ModeTypeRt2, H0
// 7nm Rt5 => ModeTypeRt2, H1
// 7nm Rt6 => ModeTypeRt3, H0
// 7nm Rt7 => ModeTypeRt3, H1
//
typedef struct _MODE_TYPE_INFO {
  MODE_TYPE           RtType;
  UINT8               RtHalf;  // 0: H0; 1: H1
} MODE_TYPE_INFO;


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
  );

//
// Function pointer definition for ChaSetRtMode()
//
typedef
EFI_STATUS
(EFIAPI *CHA_SET_RT_MODE) (
  IN UINT8            SocketId,
  IN MODE_TYPE_INFO   *TypeInfo,
  IN UINT8            EncRtMode[]
  );

typedef enum {
  RouteTypeMcTarget,
  RouteTypeMcChannel,
  RoutetypeRouteThroughRange,
  MaxRouteType
} ROUTE_TYPE_INFO;

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
  );

//
// Function pointer definition for ChaSetRtType()
//
typedef
EFI_STATUS
(EFIAPI *CHA_SET_RT_TYPE) (
  IN UINT8            SocketId,
  IN ROUTE_TYPE_INFO  TypeInfo,
  IN UINT8            Data[]
  );

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
  );

//
// Function pointer definition for ChaSetBlockDecoderBase()
//
typedef
EFI_STATUS
(EFIAPI *CHA_SET_BLOCK_DECODER_BASE) (
  IN UINT8      SocketId,
  IN UINT32     Base,
  IN UINT32     Limit
  );

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
  );

//
// Function pointer definition for ChaSetBlockDecoderEnableCfg()
//
typedef
EFI_STATUS
(EFIAPI *CHA_SET_BLOCK_DECODER_ENABLE_CFG) (
  IN UINT8      SocketId,
  IN UINT8      Index,
  IN UINT8      McChEnMask[MAX_IMC][MAX_MC_CH]
  );


typedef struct _GLOBAL_INTERLEAVE_DATA {
  UINT8   Rt0InterleaveShift; // 10nm RT0 interleave mode (1: XOR or 0: de-feature);
                              // Number of bits to shift before feeding to 7nm RT0 & RT1
  UINT8   Rt1InterleaveShift; // Number of bits to shift before feeding to 10nm RT1 or 7nm RT2 & RT3.
  UINT8   Rt2InterleaveShift; // Number of bits to shift before feeding to 10nm RT2 or 7nm RT4 & RT5.
  UINT8   Rt3InterleaveShift; // Number of bits to shift for 7nm RT6 & RT7 (7nm only)
} GLOBAL_INTERLEAVE_DATA;


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
  );

//
// Function pointer definition for ChaSetGlobalInterleave()
//
typedef
EFI_STATUS
(EFIAPI *CHA_SET_GLOBAL_INTERLEAVE) (
  IN UINT8                    SocketId,
  IN GLOBAL_INTERLEAVE_DATA   *GlobalInterleaveData
  );

typedef enum {
  RouteTable0,
  RouteTable1,
  RouteTable2,
  RouteTable3,
  MaxRouteTableType
} ROUTE_TABLE_TYPE;

typedef enum {
  RouteTableChannel,
  RouteTableTarget,
  MaxRouteTableDestination
} ROUTE_TABLE_DESTINATION;


//
// 7nm wave 4 uses the following mapping:
// 7nm Rt0 => RouteTable0,   H0
// 7nm Rt1 => RouteTable0,   H1
// 7nm Rt2 => RouteTable1,   H0
// 7nm Rt3 => RouteTable1,   H1
// 7nm Rt4 => RouteTable2,   H0
// 7nm Rt5 => RouteTable2,   H1
// 7nm Rt6 => RouteTable3,   H0
// 7nm Rt7 => RouteTable3,   H1
//
// Note: 10nm RT2LM => RouteTable3
//
typedef struct _ROUTE_TABLE_TYPE_INFO {
  ROUTE_TABLE_TYPE        RtType;
  UINT8                   RtHalf;     // 0: H0; 1: H1
} ROUTE_TABLE_TYPE_INFO;


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
  );

//
// Function pointer definition for ChaSetRouteTable()
//
typedef
EFI_STATUS
(EFIAPI *CHA_SET_ROUTE_TABLE) (
  IN UINT8                 SocketId,
  IN UINT16                ChaId,
  IN ROUTE_TABLE_TYPE_INFO *TypeInfo OPTIONAL,
  IN UINT64                Data
  );

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
  );

//
// Function pointer definition for ChaSet2lmRegisters()
//
typedef
EFI_STATUS
(EFIAPI *CHA_SET_2LM_REGS) (
  IN UINT8    SocketId,
  IN UINT32   TwoLmMask
  );

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
  );

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
  );

//
// Function pointer definition for ChaDisableEarlyRead()
//
typedef
EFI_STATUS
(EFIAPI *CHA_DISABLE_EARLY_READ) (
  IN UINT8    SocketId,
  IN UINT8    DisSpecRdValue,
  IN BOOLEAN  IodcEnableValue
  );

/**
  Programs CHA to set the HA credit change field.

  @param SocId            The socket number ID
  @param HaCreditChange   The HA credit change value

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                          - EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
ChaSetHaCreditChange (
  IN UINT8    SocId,
  IN UINT8    HaCreditChange
  );

//
// Function pointer definiton for ChaSetHaCreditChange()
//
typedef
EFI_STATUS
(EFIAPI *CHA_SET_HA_CREDIT_CHANGE) (
  IN UINT8    SocId,
  IN UINT8    HaCreditChange
  );

/**
  Programs CHA cluster to unlock for access.

  @param[in] SocketId                The socket ID and 0 should always be the SBSP.
  @param[in] ClusterId               The index of cluster to unlock.

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaUnlockDramCluster (
  IN UINT8     SocketId,
  IN UINT8     ClusterId
  );

//
// Function pointer definition for ChaUnlockDramCluster ()
//
typedef
EFI_STATUS
(EFIAPI *CHA_UNLOCK_DRAM_CLUSTER) (
  IN UINT8     SocketId,
  IN UINT8     ClusterId
  );

/**
  Programs CHA cluster to lock from access.

  @param[in] SocketId                The socket ID and 0 should always be the SBSP.

  @retval EFI_SUCCESS                The function executed successfully without any error.
  @retval EFI_UNSUPPORTED            The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaLockDramClusters (
  IN UINT8     SocketId
  );

//
// Function pointer definition for ChaLockDramClusters ()
//
typedef
EFI_STATUS
(EFIAPI *CHA_LOCK_DRAM_CLUSTERS) (
  IN UINT8     SocketId
  );


/**
  Get Domain handle for a cluster

  @param[in]  SocketId                The socket ID and 0 should always be the SBSP.
  @param[in]  ClusterId               The index of cluster to get domain handle.
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
  );

//
// Function pointer definition for ChaGetClusterDomainHandle ()
//
typedef
EFI_STATUS
(EFIAPI *CHA_GET_CLUSTER_DOMAIN_HANDLE) (
  IN  UINT8     SocketId,
  IN  UINT8     ClusterId,
  OUT UINT64    *DomainHandle
  );

/**
  Programs CHA register settings for "EliminateDirectoryInFarMemory == ELIMINATE_DIRECTORY_IN_FM_ENABLE"

  @param SocId                   CPU Socket Node number (Socket ID)

  @retval EFI_SUCCESS            The function is executed successfully without any error.

**/
EFI_STATUS
EFIAPI
ChaEliminateDirectoryInFarMemory (
  IN UINT8  SocId
  );

//
// Function pointer definition for ChaEliminateDirectoryInFarMemory()
//
typedef
EFI_STATUS
(EFIAPI *CHA_ELIM_DIR_IN_FM) (
  IN UINT8  SocId
  );

/**
  Programs CHA register settings for "DisableDirForAppDirect == DIRECTORY_FOR_APP_DIRECT_ENABLE"

  @param SocId                   CPU Socket Node number (Socket ID)

  @retval EFI_SUCCESS            The function is executed successfully without any error.

**/
EFI_STATUS
EFIAPI
ChaDisableDirForAppDirect (
  IN UINT8  SocId
  );

//
// Function pointer definition for ChaDisableDirForAppDirect()
//
typedef
EFI_STATUS
(EFIAPI *CHA_DIS_DIR_FOR_AD) (
  IN UINT8  SocId
  );

/**
  CHA: This routine determines the route table settings for Odd CHAs and Even CHAs
  in 2-socket-duallink topology when CHA interleave mode is enabled in the setup
  knob.

  @param[in] RootSocId                  Root Node Socket ID (0 or 1 since this routine is specific to 2S2L)
  @param[in] SbspSktId                  SBSP Socket Node ID
  @param[in] SocketData                 Socket specific data structure
  @param[in] UpiRoutingMode             UPI routing mode, see UPI_ROUTING_MODE enum
  @param[out] OddChaData                Route table setting for Odd CHAs
  @param[out] EvenChaData               Route table setting for Even CHAs
  @param[out] FieldMask                 Clear Destination Node entry(s) in KTI_ROUTING_TABLE registers

  @retval  EFI_SUCCESS            Route table settings for odd and even CHAs, and field mask were
                                  populated and returned.

**/
EFI_STATUS
EFIAPI
ChaInterleaveKtiRouteTableSettingFor2S2L (
  IN  UINT8              RootSocId,
  IN  UINT8              SbspSktId,
  IN  KTI_SOCKET_DATA    *SocketData,
  IN  UINT8              UpiRoutingMode,
  OUT UINT32             *OddChaData,
  OUT UINT32             *EvenChaData,
  OUT UINT32             *FieldMask
  );

/**

  CHA: Configure Clock Modulation: disable_clockmod in CLOCK_MODULATION_RATE_CHABC_SAD_REG

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param ClockModulationEn    - TRUE: enable, FALSE: disable


  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureClockModulation (
  UINT8    SocId,
  UINT8    BoxInst,
  BOOLEAN  ClockModulationEn
  );

//
// Function pointer definition for ChaConfigureTorThresholds()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CONFIGURE_CLOCK_MODULATION) (
  IN UINT8   SocId,
  IN UINT8   BoxInxt,
  IN BOOLEAN ClockModulationEn
  );

/**

  Programs CHA to set snoop throttle threshold.

  @param SocId              The socket number ID
  @param BoxInst            Box Instance, 0 based
  @param SnoopThrottleLevel The Snoop throttle threshold

  @retval - EFI_UNSUPPORTED:       The function not implemented
          - EFI_INVALID_PARAMETER: The function is not executed successfully
                                   due to some illegal input.
          - EFI_SUCCESS:           The function is excuted successfully

**/
EFI_STATUS
EFIAPI
ChaSetSnoopThrottleLevel (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    SnoopThrottleLevel
  );

//
// Function pointer definiton for ChaSetSnoopThrottleLevel()
//
typedef
EFI_STATUS
(EFIAPI *CHA_SET_SNOOP_THROTTLE) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    SnoopThrottleLevel
  );

/**
  CHA: Override default snoop_credits setting
  This function may be called for CLX+FPGA, and for ICX-SP-LCC FPGA platform configurations.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
ChaOverrideSnoopCredits  (
  UINT8    SocId,
  UINT8    BoxInst
  );

//
// Function pointer definiton for ChaOverrideSnoopCredits()
//
typedef
EFI_STATUS
(EFIAPI *CHA_OVERRIDE_SNOOP_CREDITS) (
  UINT8    SocId,
  UINT8    BoxInst
  );

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
  );

//
// Function pointer definiton for ChaSetFzmCapability()
//
typedef
EFI_STATUS
(EFIAPI *CHA_SET_FZM_CAPABILITY) (
  IN UINT8    SocketId,
  IN UINT16   CapabilityVector
  );

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
  );

//
// Function pointer definiton for ChaSetTwoWayNmCache()
//
typedef
EFI_STATUS
(EFIAPI *CHA_SET_TWO_WAY_NM_CACHE) (
  IN UINT8    ScktId,
  IN UINT8    ChaId,
  IN BOOLEAN  IsTwoWayNmCacheEn
  );

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
  );

//
// Function pointer definiton for ChaSetLocMemInvItoX()
//
typedef
VOID
(EFIAPI *CHA_SET_LOCMEMINVITOX) (
  IN UINT8       SocketId,
  IN BOOLEAN     LocMemInvItoXDis
  );

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
  );

//
// Function pointer definiton for ChaSetDisCleanDirUpdate()
//
typedef
VOID
(EFIAPI *CHA_SET_DIS_CLEAN_DIR_UPDATE) (
  IN UINT8    SocketId,
  IN UINT8    ChaId,
  IN UINT8    DisCleanDirUpdate
  );

/**
  Silicon workaround for systems with CXL endpoint device installed.

  @param[in] SocId     - CPU Socket Node number (Socket ID)
  @param[in] NumOfCha  - The total number of Chas of this socket

  @retval              - N/A
**/
VOID
EFIAPI
WorkaroundWithCxlEpDeviceInstalled (
  IN  UINT8        SocId,
  IN  UINT8        NumOfCha
  );

/**
  Return size of per entry in CHA snoop fanout router table, that is, the
  length of send2ktiport_* bitfield. The function also provide information such as:
  - if send2ktiport_* field is extended to CHA_RTR_SNP_CTL1 register.
  - the first requester node ID field (send2ktiport_i) in CHA_RTR_SNP_CTL1 register.

  @param[out] SupportExtSnpFanoutTable  TRUE if send2ktiport_* field is extended to
                                        CHA_RTR_SNP_CTL1 register. FALSE otherwise.
  @param[out] FirstNodeIdInExtTable     The first requester Node ID in CHA_RTR_SNP_CTL1 register.

  @retval  Size of per entry in CHA snoop fanout router table, i.e. length of send2ktiport_* bitfield.

**/
UINT8
EFIAPI
ChaSnpFanoutTablePerEntrySize (
  OUT BOOLEAN  *SupportExtSnpFanoutTable, OPTIONAL
  OUT UINT8    *FirstNodeIdInExtTable     OPTIONAL
  );

//
// Function pointer definition for ChaSnpFanoutTablePerEntrySize()
//
typedef
UINT8
(EFIAPI *CHA_SNP_FANOUT_TABLE_PER_ENTRY_SIZE) (
  OUT BOOLEAN  *SupportExtSnpFanoutTable, OPTIONAL
  OUT UINT8    *FirstNodeIdInExtTable     OPTIONAL
);
#endif // __CHA_LIB_H__
