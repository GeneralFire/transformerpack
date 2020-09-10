/** @file
  IIO generation specific silicon access interface

  This library abstracts silicon register differences between different versions
  of IIO IP blocks.
  This library is designed to be directly linked into the Uncore consumer of these
  services
  OR
  To be used to expose a dynamic interface (PPI, DXE Protocol, or SMM Protocol) as
  appropriate for the phase being executed.

  Furthermore, this subsystem is designed to support multiple different versions
  dynamically linked at runtime.

  The expected valid uses for this library are thus:
  (1) One Uncore PEIM supporting a homogenous system and the single instance is statically linked
  (2) One Uncore PEIM supporting a heterogeneous system where a router instance is statically linked
    and multiple PEIM produce silicon specific instances of the library and the router selects the
    correct instance per socket.

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
#ifndef __IIO_IP_INTERFACE_H__
#define __IIO_IP_INTERFACE_H__

#include <Uefi.h>
#include <Base.h>
#include <Library/UsraCsrLib.h>
#include <RcRegs.h>

#define IioGetUncoreSnc(_SocId_, _CtrId_)   UsraCsrRead((_SocId_), (_CtrId_), UNCORE_SNC_IIO_VTD_REG)
#define IioGetSncCfg(_SocId_, _CtrId_)      UsraCsrRead((_SocId_), (_CtrId_), SNC_CONFIG_IIO_VTD_REG)
#define IioGetSnc2Base(_SocId_, _CtrId_)    UsraCsrRead((_SocId_), (_CtrId_), SNC_BASE_2_IIO_VTD_REG)
#define IioGetVid(_SocId_, _CtrId_)         UsraCsrRead((_SocId_), (_CtrId_), VID_IIO_M2IAL_REG);

/**
  IIO: Set SNC two LM masks

  @param SocId        - Socket ID
  @param CtrId        - Controller ID
  @param TwoLmMask1Lo - Two LM field mask1 low part
  @param TwoLmMask1Hi - Two LM field mask1 high part
  @param TwoLmMask2   - Two LM field mask2

  @retval             - EFI_UNSUPPORTED: the function not implemented
                        EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
IioSetSncBaseTwoLmMask (
  UINT8 SocId,
  UINT8 CtrId,
  UINT32 TwoLmMask1Lo,
  UINT32 TwoLmMask1Hi,
  UINT32 TwoLmMask2
  );
//
// Function pointer definiton for IioSetSncBaseTwoLmMask()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_SNC_BASE_TWO_LM_MASK) (
  UINT8 SocId,
  UINT8 CtrId,
  UINT32 TwoLmMask1Lo,
  UINT32 TwoLmMask1Hi,
  UINT32 TwoLmMask2
  );

/**
  IIO:  Check if device exists

  @param SocId        - Socket ID
  @param CtrId        - Controller ID
  @param DeviceExists - TRUE if device exist and FALSE otherwise

  @retval             - EFI_UNSUPPORTED: the function not implemented
                        EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioDeviceExists (
  UINT8    SocId,
  UINT8    CtrId,
  BOOLEAN* DeviceExists
  );
//
// Function pointer definiton for IioDeviceExists()
//
typedef
EFI_STATUS
(EFIAPI *IIO_DEVICE_EXISTS) (
  UINT8    SocId,
  UINT8    CtrId,
  BOOLEAN* DeviceExists
  );

/**
  IIO:  Set Root bus base and enable

  @param SocId      - Socket ID
  @param CtrId      - Controller ID
  @param BusBase    - Root Bus Base
  @param BusNumC    - Root Bus Number

  @retval           - EFI_UNSUPPORTED: the function not implemented
                      EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetRootBusBaseEn (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 BusBase,
  UINT32 BusNumC
  );
//
// Function pointer definiton for IioSetRootBusBaseEn()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_ROOT_BUS_BASE_EN) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 BusBase,
  UINT32 BusNumC
  );

/**
  IIO:  Set Snc

  @param SocId      - Socket ID
  @param CtrId      - Controller ID
  @param SncConfig  - SNC configuration value

  @retval           - EFI_UNSUPPORTED: the function not implemented
                      EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetSncCfg (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 SncConfig
  );
//
// Function pointer definiton for IioSetSncCfg()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_SNC_CFG) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 SncConfig
  );

/**
  IIO:  Set Node id

  @param SocId      - Socket ID
  @param CtrId      - Controller ID

  @retval           - EFI_UNSUPPORTED: the function not implemented
                      EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetNodeId (
  UINT8 SocId,
  UINT8 CtrId
  );
//
// Function pointer definiton for IioSetNodeId()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_NODEID) (
  UINT8 SocId,
  UINT8 CtrId
  );

/**
  IIO:  Set MMIOH granularity

  @param SocId            - Socket ID
  @param CtrId            - Controller ID
  @param MmiohGranularity - Min mmioh granularity to each stack

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetMmiohGranularity (
  UINT8 SocId,
  UINT8 CtrId,
  UINT8 MmiohGranularity
  );
//
// Function pointer definiton for IioSetMmiohGranularity()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_MMIOH_GRANULARITY) (
  UINT8 SocId,
  UINT8 CtrId,
  UINT8 MmiohGranularity
  );

/**
  IIO:  Set MMIOL granularity

  @param SocId            - Socket ID
  @param CtrId            - Controller ID
  @param MmiohGranularity - Min mmiol granualarity to each stack

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetMmiolGranularity (
  UINT8 SocId,
  UINT8 CtrId,
  UINT8 MmiolGranularity
  );
//
// Function pointer definiton for IioSetMmiolGranularity()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_MMIOL_GRANULARITY) (
  UINT8 SocId,
  UINT8 CtrId,
  UINT8 MmiolGranularity
  );

/**
  IIO:  Set MMCFG Base

  @param SocId     - Socket ID
  @param CtrId     - Controller ID
  @param MmcfgBase - The MMCFG Base

  @retval          - EFI_UNSUPPORTED: the function not implemented
                     EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetMmcfgBase (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 MmcfgBase
  );
//
// Function pointer definiton for IioSetMmcfgBase()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_MMCFG_BASE) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 MmcfgBase
  );

/**
  IIO:  Set MMCFG Limit

  @param SocId      - Socket ID
  @param CtrId      - Controller ID
  @param MmcfgLimit - The MMCFG Limit

  @retval           - EFI_UNSUPPORTED: the function not implemented
                      EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetMmcfgLimit (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 MmcfgLimit
  );
//
// Function pointer definiton for IioSetMmcfgLimit()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_MMCFG_LIMIT) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 MmcfgLimit
  );

/**
  IIO:  Set MMCFG Limit

  @param SocId            - Socket ID
  @param CtrId            - Controller ID
  @param GlobBusIdRule    - The Global bus id rule
  @param MmcfgTarget      - MMCFG target
  @param MmcfgTargetLocal - MMCFG local target
  @param MmcfgLocalRule   - Array of MMCFG Local rule

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioCfgBusId (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 GlobBusIdRule,
  UINT32 MmcfgTarget,
  UINT32 MmcfgTargetLocal[],
  UINT32 MmcfgLocalRule[]
  );
//
// Function pointer definiton for IioCfgBusId()
//
typedef
EFI_STATUS
(EFIAPI *IIO_CFG_BUSID) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 GlobBusIdRule,
  UINT32 MmcfgTarget,
  UINT32 MmcfgTargetLocal[],
  UINT32 MmcfgLocalRule[]
  );

/**
  IIO:  Updates the given MMCFG rule and returns the udpated value

  @param SocId            - Socket ID
  @param ValidSoc         - Indicates if socket is valid (valid = 1 otherwise 0)
  @param SocType          - The socket type
  @param TargetOffset     - The target offset
  @param MmcfgRule        - The MMCFG rule
  @param UpdatedBusIdRule - The Updated Bus Id rule

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioGetUpdatedBusIdRule (
  UINT8   SocId,
  UINT8   ValidSoc,
  UINT8   SocType,
  UINT8   TargetOffset,
  UINT32  MmcfgRule,
  UINT32* UpdatedBusIdRule
  );
//
// Function pointer definiton for IioGetUpdatedBusIdRule()
//
typedef
EFI_STATUS
(EFIAPI *IIO_GET_UPDATED_BUSID_RULE) (
  UINT8   SocId,
  UINT8   ValidSoc,
  UINT8   SocType,
  UINT8   TargetOffset,
  UINT32  MmcfgRule,
  UINT32* UpdatedBusIdRule
  );

/**
  IIO:  Set Bus no configuration

  @param SocId     - Socket ID
  @param CtrId     - Controller ID
  @param BusBase   - SNC uncore value
  @param BusLimit  - SNC configuration value

  @retval          - EFI_UNSUPPORTED: the function not implemented
                     EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetBusNoCfg (
  UINT8 SocId,
  UINT8 CtrId,
  UINT8 BusBase,
  UINT8 BusLimit
  );
//
// Function pointer definiton for IioSetBusNoCfg()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_BUS_NO_CFG) (
  UINT8 SocId,
  UINT8 CtrId,
  UINT8 BusBase,
  UINT8 BusLimit
  );

/**
  IIO:  Set the ITC IRP Overflow enable bit

  @param SocId        - Socket ID
  @param CtrId        - Controller ID
  @param OverFlowEn   - Overflow enable value (0|1)

  @retval             - EFI_UNSUPPORTED: the function not implemented
                        EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetItcIrpOflEn (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 OverFlowEn
  );
//
// Function pointer definiton for IioSetItcIrpOflEn()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_ITC_IRP_OFL_EN) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 OverFlowEn
  );

/**
  IIO:  Set the Isoc enable bit

  @param SocId    - Socket ID
  @param CtrId    - Controller ID
  @param IsocEn   - Isoc enable value (0|1)

  @retval         - EFI_UNSUPPORTED: the function not implemented
                    EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetIsocEn (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 IsocEn
  );
//
// Function pointer definiton for IioSetIsocEn()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_ISOC_EN) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 IsocEn
  );

/**
  IIO:  Set DDRT ranges for IO ports.

  The caller verifies DdrtQosMode knob is Mode 2 before calling this routine to
  program DDRT ranges. DDRT ranges are 0 if the knob DdrtQosMode is Mode 0 or 1
  where ddrt_defeature bit is set to 1.

  @param SocId      - Socket ID
  @param CtrId      - Controller ID
  @param DdrtBase   - DDRT uncore value
  @param DdrtLimit  - DDRT configuration value
  @param MaskLo     - Mask for the low part
  @param MaskHi     - Mask for the high part

  @retval           - EFI_UNSUPPORTED: the function not implemented
                      EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetDdrtBase (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 DdrtBase,
  UINT32 DdrtLimit,
  UINT32 MaskLo,
  UINT32 MaskHi
  );
//
// Function pointer definiton for IioSetDdrtBase()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_DDRT_BASE) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 DdrtBase,
  UINT32 DdrtLimit,
  UINT32 MaskLo,
  UINT32 MaskHi
  );

/**
  IIO:  Set DDRT credit enable values

  @param SocId       - Socket ID
  @param CtrId       - Controller ID
  @param DdrtQosMode - DDRT QoS Mode

  @retval           - EFI_UNSUPPORTED: the function not implemented
                      EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetDdrtCrdEn (
  UINT8 SocId,
  UINT8 CtrId,
  UINT8 DdrtQosMode
  );
//
// Function pointer definiton for IioSetDdrtCrdEn()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_DDRT_CRD_EN) (
  UINT8 SocId,
  UINT8 CtrId,
  UINT8 DdrtQosMode
  );

/**
  IIO:  Set the Isoc/MeSeg configuration

  @param SocId    - Socket ID
  @param CtrId    - Controller ID

  @retval         - EFI_UNSUPPORTED: the function not implemented
                    EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetIsocMeSegCfg (
  UINT8  SocId,
  UINT8  CtrId
  );
//
// Function pointer definiton for IioSetIsocMeSegCfg()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_ISOC_MESEG_CFG) (
  UINT8  SocId,
  UINT8  CtrId
  );

/**
  IIO:  Create a new rule

  @param SocId         - Socket ID
  @param RuleEn        - Rule enable value
  @param Serial        - Serialize rule value
  @param MmiolBase     - MMIOL Base
  @param MmiolLimit    - MMIOL Limit
  @param RemoteStackId - Remote Stack ID
  @param NewMmiolRule  - New MMIOL Limit

  @retval             - EFI_UNSUPPORTED: the function not implemented
                        EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioNewMmiolRule (
  UINT8   SocId,
  UINT8   RuleEn,
  UINT8   Serial,
  UINT32  MmiolBase,
  UINT32  MmiolLimit,
  UINT8   RemoteStackId,
  UINT32* NewMmiolRule
  );
//
// Function pointer definiton for IioNewMmiolRule()
//
typedef
EFI_STATUS
(EFIAPI *IIO_NEW_MMIOL_RULE) (
  UINT8   SocId,
  UINT8   RuleEn,
  UINT8   Serial,
  UINT32  MmiolBase,
  UINT32  MmiolLimit,
  UINT8   RemoteStackId,
  UINT32* NewMmiolRule
  );

/**
  IIO:  Create a new rule for NEM/Ego coverage and point locally

  @param SocId           - Socket ID
  @param RuleEn          - Rule enable value
  @param AboveMmiol      - Indicates if MMCFG is above MMIOL
  @param CurrentRule     - The current rule value
  @param MmiolBase       - MMIOL Base
  @param MmiolLimit      - MMIOL Limit
  @param NemRuleBase     - Nem rule base
  @param NewMmiolNemRule - Nem MMIOL new rule

  @retval                - EFI_UNSUPPORTED: the function not implemented
                           EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioNewMmiolNemRule (
  UINT8   SocId,
  UINT8   RuleEn,
  BOOLEAN AboveMmiol,
  UINT32  CurrentRule,
  UINT32  MmiolBase,
  UINT32  MmiolLimit,
  UINT32  NemRuleBase,
  UINT32* NewMmiolNemRule
  );
//
// Function pointer definiton for IioNewMmiolNemRule()
//
typedef
EFI_STATUS
(EFIAPI *IIO_NEW_MMIOL_NEM_RULE) (
  UINT8   SocId,
  UINT8   RuleEn,
  BOOLEAN AboveMmiol,
  UINT32  CurrentRule,
  UINT32  MmiolBase,
  UINT32  MmiolLimit,
  UINT32  NemRuleBase,
  UINT32* NewMmiolNemRule
  );

/**
  IIO:  Set the MMIOL rule using an index

  @param SocId    - Socket ID
  @param CtrId    - Controller ID
  @param RuleIdx  - The rule index
  @param RuleData - The rule data

  @retval         - EFI_UNSUPPORTED: the function not implemented
                    EFI_INVALID_PARAMETER   A required parameter was NULL or is out of range.
                    EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetMmiolRuleIdx (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 RuleIdx,
  UINT32 RuleData
  );
//
// Function pointer definiton for IioSetMmiolRuleIdx()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_MMIOL_RULE_IDX) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 RuleIdx,
  UINT32 RuleData
  );

/**
  IIO:  Set the MMIOL Traget list

  @param SocId      - Socket ID
  @param CtrId      - Controller ID
  @param DecoderNum - Mmiol Decoder Num
  @param ListData   - The target list data

  @retval         - EFI_UNSUPPORTED: the function not implemented
                    EFI_INVALID_PARAMETER   A required parameter was NULL or is out of range.
                    EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetMmiolListIdx (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 DecoderNum,
  UINT32 ListData[]
  );
//
// Function pointer definiton for IioSetMmiolListIdx()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_MMIOL_LIST_IDX) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 DecoderNum,
  UINT32 ListData[]
  );

/**
  IIO:  Set the MMIOH rule using an index

  @param SocId          - Socket ID
  @param CtrId          - Controller ID
  @param Target         - The target
  @param TgtIdx         - The target index
  @param SerializerRule - The serializer rule
  @param RemoteStackId  - The Remote stack ID
  @param BaseHi         - The MMIOH base high part
  @param BaseLo         - The MMIOH base low part
  @param LimitHi        - The MMIOH limit high part
  @param LimitLo        - The MMIOH limit low part

  @retval               - EFI_UNSUPPORTED: the function not implemented
                          EFI_INVALID_PARAMETER   A required parameter was NULL or is out of range.
                          EFI_SUCCESS: the function is excuted successfully
**/
EFI_STATUS
EFIAPI
IioSetMmiohRuleIdx (
  UINT8  SocId,
  UINT8  CtrId,
  UINT8  Target,
  UINT8  TgtIdx,
  UINT8  SerializerRule,
  UINT8  RemoteStackId,
  UINT32 BaseHi,
  UINT32 BaseLo,
  UINT32 LimitHi,
  UINT32 LimitLo
  );
//
// Function pointer definiton for IioSetMmiohRuleIdx()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_MMIOH_RULE_IDX) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT8  Target,
  UINT8  TgtIdx,
  UINT8  SerializerRule,
  UINT8  RemoteStackId,
  UINT32 BaseHi,
  UINT32 BaseLo,
  UINT32 LimitHi,
  UINT32 LimitLo
  );

/**
  IIO:  Set Write combining disable bit

  @param SocId     - Socket ID
  @param CtrId     - Controller ID
  @param WrtComb   - Wrt combining bit value
  @param DcaEn     - Enable DCA

  @retval          - EFI_UNSUPPORTED: the function not implemented
                     EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetWrtCombDca (
  UINT8   SocId,
  UINT8   CtrId,
  UINT8   WrtComb,
  BOOLEAN DcaEn
  );
//
// Function pointer definiton for IioSetWrtCombDca()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_WRT_COMB_DCA) (
  UINT8   SocId,
  UINT8   CtrId,
  UINT8   WrtComb,
  BOOLEAN DcaEn
  );

/**
  IIO:  Set value of register MiscCtrl

  @param SocId             - Socket ID
  @param CtrId             - Controller ID
  @param OutLegacyVgaSoc   - OutLegacyVgaSoc current value
  @param MaxAddrLine       - MaxAddress in size in bits
  @param OrderingFlag      - Indicates if ordering is enabled

  @retval                  - EFI_UNSUPPORTED: the function not implemented
                             EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetMiscCtrl (
  UINT8   SocId,
  UINT8   CtrId,
  UINT8   OutLegacyVgaSoc,
  UINT8   SplitLock,
  UINT8   MaxAddrLine,
  BOOLEAN OrderingFlag
  );
//
// Function pointer definiton for IioSetMiscCtrl()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_MISC_CTRL) (
  UINT8   SocId,
  UINT8   CtrId,
  UINT8   OutLegacyVgaSoc,
  UINT8   MaxAddrLine,
  BOOLEAN OrderingFlag
  );

/**
  IIO:  Set the SAD targets

  @param SocId             - Socket ID
  @param CtrId             - Controller ID
  @param SbspSoc           - The SBSP Soc ID
  @param PchStack          - The PCH stack
  @param LegacyPchTgt      - The legacy PCH target
  @param VgaTgt            - The VGA target
  @param OutLegacyVgaSoc   - The Legacy VGA Soc ID
  @param OutLegacyVgaStack - The Legacy VGA Soc ID

  @retval                - EFI_UNSUPPORTED: the function not implemented
                           EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetSadTargets (
  UINT8 SocId,
  UINT8 CtrId,
  UINT8 SbspSoc,
  UINT8 PchStack,
  UINT8 LegacyPchTgt,
  UINT8 VgaTgt,
  UINT8 OutLegacyVgaSoc,
  UINT8 OutLegacyVgaStack
  );
//
// Function pointer definiton for IioSetSadTargets()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_SAD_TARGETS) (
  UINT8 SocId,
  UINT8 CtrId,
  UINT8 SbspSoc,
  UINT8 PchStack,
  UINT8 LegacyPchTgt,
  UINT8 VgaTgt,
  UINT8 OutLegacyVgaSoc,
  UINT8 OutLegacyVgaStack
  );

/**
  IIO:  Set Snc uncore register

  @param SocId            - Socket ID
  @param CtrId            - Controller ID
  @param NumChaCluster    - Number of channels per cluster
  @param BaseChaCluster1  - Base of channels for cluster1
  @param BaseChaCluster2  - Base of channels for cluster2
  @param BaseChaCluster3  - Base of channels for cluster3

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetSncUncore (
  UINT8  SocId,
  UINT8  CtrId,
  UINT8  NumChaCluster,
  UINT8  BaseChaCluster1,
  UINT8  BaseChaCluster2,
  UINT8  BaseChaCluster3
  );
//
// Function pointer definiton for IioSetSncUncore()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_SNC_UNCORE) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT8  NumChaCluster,
  UINT8  BaseChaCluster1,
  UINT8  BaseChaCluster2,
  UINT8  BaseChaCluster3
  );


/**
  IIO:  Get Snc uncore configuration

  @param SocId            - Socket ID
  @param CtrId            - Controller ID
  @param NumChaPerCluster - Number of CHAs per SNC Cluster
  @param BaseChaCluster1  - Base of CHA Cluster 1
  @param BaseChaCluster2  - Base of CHA Cluster 2
  @param BaseChaCluster3  - Base of CHA Cluster 3

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
IioGetSncUncoreCfg (
  UINT8  SocId,
  UINT8  CtrId,
  UINT8  *NumChaPerCluster,
  UINT8  *BaseChaCluster1,
  UINT8  *BaseChaCluster2,
  UINT8  *BaseChaCluster3
  );
//
// Function pointer definiton for IioGetSncUncoreCfg()
//
typedef
EFI_STATUS
(EFIAPI *IIO_GET_SNC_UNCORE_CFG) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT8  *NumChaPerCluster,
  UINT8  *BaseChaCluster1,
  UINT8  *BaseChaCluster2,
  UINT8  *BaseChaCluster3
  );

/**
  IIO:  Check if Starting address is zero

  @param SocId              - Socket ID
  @param CtrId              - Controller ID
  @param IsStartingAddrZero - TRUE if starting addr == 0

  @retval                   - EFI_UNSUPPORTED: the function not implemented
                              EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioIsStartingAddrZero (
  UINT8    SocId,
  UINT8    CtrId,
  BOOLEAN* IsStartingAddrZero
  );
//
// Function pointer definiton for IioIsStartingAddrZero()
//
typedef
EFI_STATUS
(EFIAPI *IIO_IS_STARTING_ADDR_ZERO) (
  UINT8    SocId,
  UINT8    CtrId,
  BOOLEAN* IsStartingAddrZero
  );

/**
  IIO:  Get the class code

  @param SocId      - Socket ID
  @param CtrId      - Controller ID
  @param ClassCode  - Class ID

  @retval           - EFI_UNSUPPORTED: the function not implemented
                      EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioGetClassCode (
  UINT8   SocId,
  UINT8   CtrId,
  UINT32* ClassCode
  );
//
// Function pointer definiton for IioGetClassCode()
//
typedef
EFI_STATUS
(EFIAPI *IIO_GET_CLASS_CODE) (
  UINT8   SocId,
  UINT8   CtrId,
  UINT32* ClassCode
  );

/**
  IIO:  Get the class code

  @param SocId        - Socket ID
  @param CtrId        - Controller ID
  @param SncUpperBase - Snc Upper Base

  @retval             - EFI_UNSUPPORTED: the function not implemented
                        EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioGetSncUpperBase (
  UINT8   SocId,
  UINT8   CtrId,
  UINT32* SncUpperBase
  );
//
// Function pointer definiton for IioGetSncUpperBase()
//
typedef
EFI_STATUS
(EFIAPI *IIO_GET_SNC_UPPER_BASE) (
  UINT8   SocId,
  UINT8   CtrId,
  UINT32* SncUpperBase
  );

/**
  IIO:  Get the 2LM Address mask

  @param SocId          - Socket ID
  @param CtrId          - Controller ID
  @param Snc2LmAddrMask - The 2LM mask

  @retval               - EFI_UNSUPPORTED: the function not implemented
                          EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioGetSnc2LmAddrMask (
  UINT8   SocId,
  UINT8   CtrId,
  UINT32* Snc2LmAddrMask
  );
//
// Function pointer definiton for IioGetSnc2LmAddrMask()
//
typedef
EFI_STATUS
(EFIAPI *IIO_GET_SNC_2LM_ADDR_MASK) (
  UINT8   SocId,
  UINT8   CtrId,
  UINT32* Snc2LmAddrMask
  );

/**
  IIO:  Set UMA cluster configuration

  @param SocId            - Socket ID
  @param CtrId            - Controller ID
  @param UmaClusterEnable - Enable bit for UMA based clustering mode
  @param DefeatureXor     - Defeature Xor

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetUmaClusterCfg (
  UINT8  SocId,
  UINT8  CtrId,
  UINT8  UmaClusterEnable,
  UINT8  DefeatureXor
  );
//
// Function pointer definiton for IioSetUmaClusterCfg()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_UMA_CLUSTER_CFG) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT8  UmaClusterEnable,
  UINT8  DefeatureXor
  );

/**
  IIO:  Get UMA cluster configuration

  @param SocId          - Socket ID
  @param CtrId          - Controller ID
  @param UmaClusterEn   - UMA clustering enable
  @param XorDefeature   - Enable for XOR defeature mode

  @retval               - EFI_UNSUPPORTED: the function not implemented
                          EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioGetUmaClusterCfg (
  UINT8  SocId,
  UINT8  CtrId,
  UINT8  *UmaClusterEn,
  UINT8  *XorDefeature
  );
//
// Function pointer definiton for IioGetUmaClusterCfg()
//
typedef
EFI_STATUS
(EFIAPI *IIO_GET_UMA_CLUSTER_CFG) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT8  *UmaClusterEn,
  UINT8  *XorDefeature
  );

/**
  IIO:  Set SNC upper base configuration

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param SncUpperBase  - The UMA cluster configuration

  @retval              - EFI_UNSUPPORTED: the function not implemented
                         EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetSncUpperBase (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 SncUpperBase
  );
//
// Function pointer definiton for IioSetSncUpperBase()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_SNC_UPPER_BASE) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 SncUpperBase
  );

/**
  IIO:  Get the MCTP segment ID size

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param MctpSegIdSize - The MCTP segment ID size

  @retval              - EFI_UNSUPPORTED: the function not implemented
                         EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioGetMctpSegIdSize (
  UINT8  SocId,
  UINT8  CtrId,
  UINT8* MctpSegIdSize
  );
//
// Function pointer definiton for IioGetMctpSegIdSize()
//
typedef
EFI_STATUS
(EFIAPI *IIO_GET_MCTP_SEG_ID_SIZE) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT8* MctpSegIdSize
  );

/**
  IIO:  Set the DMI configuration type 0

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param Type0         - The DMI configuration type 0 value

  @retval              - EFI_UNSUPPORTED: the function not implemented
                         EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetDmiCfgType0 (
  UINT8  SocId,
  UINT8  CtrId,
  UINT8  Type0
  );
//
// Function pointer definiton for IioSetDmiCfgType0()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_DMI_CFG_TYPE0) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT8  Type0
  );

/**
  IIO:  Set the MMIOH target list using an index

  @param SocId    - Socket ID
  @param CtrId    - Controller ID
  @param TgtLst   - The target list
  @param TgtIdx   - The target index
  @retval         - EFI_UNSUPPORTED: the function not implemented
                    EFI_INVALID_PARAMETER   A required parameter was NULL or is out of range.
                    EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetMmiohListIdx (
  UINT8   SocId,
  UINT8   CtrId,
  UINT32  TgtLst,
  UINT8   TgtIdx
  );
//
// Function pointer definiton for IioSetMmiohListIdx()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_MMIOH_LIST_IDX) (
  UINT8   SocId,
  UINT8   CtrId,
  UINT32  TgtLst,
  UINT8   TgtIdx
  );

/**
  IIO:  Create a new rule

  @param SocId                    - Socket ID
  @param RuleEn                   - Rule enable value
  @param SerialRmt                - Serialize remote rule value
  @param MmiohBase                - MMIOH Base
  @param MmiohSize                - MMIOL Size
  @param NewMmiohInterleavedRule0 - New MMIOL Interleaved Rule0

  @retval                         - EFI_UNSUPPORTED: the function not implemented
                                    EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioNewMmiohInterleavedRule0 (
  UINT8   SocId,
  UINT8   RuleEn,
  UINT8   SerialRmt,
  UINT32  MmiohBase,
  UINT16  MmiohSize,
  UINT32* NewMmiohInterleavedRule0
  );
//
// Function pointer definiton for IioNewMmiohInterleavedRule0()
//
typedef
EFI_STATUS
(EFIAPI *IIO_NEW_MMIOH_INTER_LEAVED_RULE0) (
  UINT8   SocId,
  UINT8   RuleEn,
  UINT8   SerialRmt,
  UINT32  MmiohBase,
  UINT16  MmiohSize,
  UINT32* NewMmiohInterleavedRule0
  );

/**
  IIO:  Create a new rule

  @param SocId                    - Socket ID
  @param SerialLcl                - Serialize local rule value
  @param MmiohLimit               - MMIOH Limit
  @param NewMmiohInterleavedRule1 - New MMIOL Interleaved Rule1

  @retval                         - EFI_UNSUPPORTED: the function not implemented
                                    EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioNewMmiohInterleavedRule1 (
  UINT8   SocId,
  UINT8   SerialLcl,
  UINT32  MmiohLimit,
  UINT32* NewMmiohInterleavedRule1
  );
//
// Function pointer definiton for IioNewMmiohInterleavedRule1()
//
typedef
EFI_STATUS
(EFIAPI *IIO_NEW_MMIOH_INTER_LEAVED_RULE1) (
  UINT8   SocId,
  UINT8   SerialLcl,
  UINT32  MmiohLimit,
  UINT32* NewMmiohInterleavedRule1
  );

/**
  IIO:  Create a new rule

  @param SocId                       - Socket ID
  @param RuleEn                      - Rule enable value
  @param Serial                      - Serialize rule value
  @param MmiohBase                   - MMIOH Base
  @param NewMmiohNonInterleavedRule0 - New MMIOL Non-Interleaved Rule0

  @retval                            - EFI_UNSUPPORTED: the function not implemented
                                       EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioNewMmiohNonInterleavedRule0 (
  UINT8   SocId,
  UINT8   RuleEn,
  UINT8   Serial,
  UINT32  MmiohBase,
  UINT32* NewMmiohNonInterleavedRule0
  );
//
// Function pointer definiton for IioNewMmiohNonInterleavedRule0()
//
typedef
EFI_STATUS
(EFIAPI *IIO_NEW_MMIOH_NON_INTER_LEAVED_RULE0) (
  UINT8   SocId,
  UINT8   RuleEn,
  UINT8   Serial,
  UINT32  MmiohBase,
  UINT32* NewMmiohNonInterleavedRule0
  );

/**
  IIO:  Create a new rule

  @param SocId                       - Socket ID
  @param MmiohLimit                  - MMIOH Limit
  @param NewMmiohNonInterleavedRule1 - New MMIOL Non-Interleaved Rule1

  @retval                            - EFI_UNSUPPORTED: the function not implemented
                                       EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioNewMmiohNonInterleavedRule1 (
  UINT8   SocId,
  UINT32  MmiohLimit,
  UINT32* NewMmiohNonInterleavedRule1
  );
//
// Function pointer definiton for IioNewMmiohNonInterleavedRule1()
//
typedef
EFI_STATUS
(EFIAPI *IIO_NEW_MMIOH_NON_INTER_LEAVED_RULE1) (
  UINT8   SocId,
  UINT32  MmiohLimit,
  UINT32* NewMmiohNonInterleavedRule1
  );

/**
  IIO:  Update NonInterleaveRule0 Serialize rule

  @param SocId                                - Socket ID
  @param NonInterleaveRule0                   - Non-Interleaved Rule0 value
  @param InterleaveRule1                      - Interleaved Rule1 value
  @param UpdateSerialMmiohNonInterleavedRule1 - Update Serial Mmioh Non-Interleaved Rule1

  @retval                                     - EFI_UNSUPPORTED: the function not implemented
                                                EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioUpdateSerialMmiohNonInterleavedRule1 (
  UINT8   SocId,
  UINT32  NonInterleaveRule0,
  UINT32  InterleaveRule1,
  UINT32* UpdateSerialMmiohNonInterleavedRule1
  );
//
// Function pointer definiton for IioUpdateSerialMmiohNonInterleavedRule1()
//
typedef
EFI_STATUS
(EFIAPI *IIO_UPDATE_SERIAL_MMIOH_NON_INTER_LEAVED_RULE1) (
  UINT8   SocId,
  UINT32  NonInterleaveRule0,
  UINT32  InterleaveRule1,
  UINT32* UpdateSerialMmiohNonInterleavedRule1
  );

/**
  IIO:  Update NonInterleaveRule0 Serialize rule

  @param SocId                 - Socket ID
  @param CtrId                 - Controller ID
  @param InterleaveRule0       - Interleaved Rule0 value
  @param InterleaveRule1       - Interleaved Rule1 value
  @param NonInterleaveRule0    - Non-Interleaved Rule0 value
  @param NonInterleaveRule1    - Non-Interleaved Rule1 value
  @param NonInterleaveTgt      - Non-Interleaved Rule1 value

  @retval                      - EFI_UNSUPPORTED: the function not implemented
                                 EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetMmioRules (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 InterleaveRule0,
  UINT32 InterleaveRule1,
  UINT32 NonInterleaveRule0,
  UINT32 NonInterleaveRule1,
  UINT8  NonInterleaveTgt
  );
//
// Function pointer definiton for IioSetMmioRules()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_MMIO_RULES) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 InterleaveRule0,
  UINT32 InterleaveRule1,
  UINT32 NonInterleaveRule0,
  UINT32 NonInterleaveRule1,
  UINT8  NonInterleaveTgt
  );

/**
  IIO:  Set the Isoch Request Priority enable bit

  @param SocId   - Socket ID
  @param CtrId   - Controller ID
  @param IrpEn   - Isoch Request Priority enable value (0|1)

  @retval        - EFI_UNSUPPORTED: the function not implemented
                   EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetIsochRqtPrEn (
  UINT8 SocId,
  UINT8 CtrId,
  UINT8 IrpEn
  );
//
// Function pointer definiton for IioSetIsochRqtPrEn()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_ISOCH_RQT_PR_EN) (
  UINT8 SocId,
  UINT8 CtrId,
  UINT8 IrpEn
  );

/**
  IIO:  Set the SNC Base address using and index

  @param SocId      - Socket ID
  @param CtrId      - Controller ID
  @param ClusterIdx - The cluster index
  @param ClMemBase  - The cluster memory base address

  @retval           - EFI_UNSUPPORTED: the function not implemented
                      EFI_INVALID_PARAMETER   A required parameter was NULL or is out of range.
                      EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetSncBaseAddr (
  UINT8  SocId,
  UINT8  CtrId,
  UINT8  ClusterIdx,
  UINT32 ClMemBase
  );
//
// Function pointer definiton for IioSetSncBaseAddr ()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_SNC_BASE_ADDR) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT8  ClusterIdx,
  UINT32 ClMemBase
  );

/**
  IIO:  Get the starting address using and index

  @param SocId      - Socket ID
  @param CtrId      - Controller ID
  @param ClusterIdx - The cluster index
  @param SncIdx     - The Snc Idx

  @retval           - EFI_UNSUPPORTED: the function not implemented
                      EFI_INVALID_PARAMETER: A required parameter was NULL or is out of range.
                      EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioGetSncIdx (
  UINT8   SocId,
  UINT8   CtrId,
  UINT8   ClusterIdx,
  UINT32* SncIdx
  );
//
// Function pointer definiton for IioGetSncIdx()
//
typedef
EFI_STATUS
(EFIAPI *IIO_GET_SNC_IDX) (
  UINT8   SocId,
  UINT8   CtrId,
  UINT8   ClusterIdx,
  UINT32* SncIdx
  );

/**
  IIO:  Set the MctpBroadcastSettings

  @param SocId                - Socket ID
  @param CtrId                - Controller ID
  @param StackPresentBitMap   - The stack bit map
  @param SocketsPresentBitmap - The socket bit map

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetMctpBroadcastSettings (
  UINT8  SocId,
  UINT8  CtrId,
  UINT16 StackPresentBitMap,
  UINT8  SocketsPresentBitmap
  );
//
// Function pointer definiton for IioSetMctpBroadcastSettings()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_MCTP_BROAD_CAST_SETTINGS) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT16 StackPresentBitMap,
  UINT8  SocketsPresentBitmap
  );

/**
  IIO:  Set the MCTP segment ID size

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param SegmentSocket - The Segment socket ID
  @param MmcfgRule     - Th MMCFG rule

  @retval              - EFI_UNSUPPORTED: the function not implemented
                         EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetMctpSegIdSize (
  UINT8  SocId,
  UINT8  CtrId,
  UINT8  SegmentSocket,
  UINT32 MmcfgRule
  );
//
// Function pointer definiton for IioSetMctpSegIdSize()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_MCTP_SEG_ID_SIZE) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT8  SegmentSocket,
  UINT32 MmcfgRule
  );

/**
  Executed by SBSP only. Clear DMICTRL.auto_pm_complete bit.

  @param[in] SocId          Socket ID

  @retval                   EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully
**/
EFI_STATUS
EFIAPI
IioClearDmictrlAutoCompletePm (
  IN UINT8               SocId
  );
//
// Function pointer definiton for IioClearDmictrlAutoCompletePm()
//
typedef
EFI_STATUS
(EFIAPI *IIO_CLEAR_DMICTRL_AUTO_COMPLETE_PM) (
  IN UINT8               SocId
  );

/**
  IIO: Configure MMIOH Stack Interleave List

  @param SocId                             - CPU Socket Node number (Socket ID)
  @param BoxInst                           - Box Instance, 0 based
  @param MmiohIntlvStackListRegIndex       - MMIOH Interleave Stack List Register Index, 0 based
  @param MmiohIntlvStackList               - MMIOH Interleave Stack List

  @retval                                  - EFI_UNSUPPORTED: the function not implemented
                                             EFI_SUCCESS: the function is excuted successfully
**/
EFI_STATUS
EFIAPI
IioConfigureMmiohIntlvStackList (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    MmiohIntlvStackListRegIndex,
  IN UINT32   MmiohIntlvStackList
  );
//
// Function pointer definiton for IioConfigureMmiohIntlvStackList()
//
typedef
EFI_STATUS
(EFIAPI *IIO_CONFIGURE_MMIOH_INTLV_STACK_LIST) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    MmiohIntlvStackListRegIndex,
  IN UINT32   MmiohIntlvStackList
  );

/**
  IIO:  Set the didicated NCS,NCB and DRS credits

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param *Ncs          - Pointer to the dedicated NCS credits data buffer
  @param *Ncb          - Pointer to the dedicated NCB credits data buffer
  @param *Drs          - Pointer to the dedicated DRS credits data buffer

  @retval              - EFI_UNSUPPORTED: the function not implemented
                         EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetDedicatedNcsNcbDrs (
  UINT8  SocId,
  UINT8  CtrId,
  UINT8  *Ncs,
  UINT8  *Ncb,
  UINT8  *Drs
  );

//
// Function pointer definiton for IioSetDedicatedNcsNcbDrs()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_DEDICATED_NCS_NCB_DRS) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT8  *Ncs,
  UINT8  *Ncb,
  UINT8  *Drs
  );

/**
  IIO:  Set Irp egress credit control register's bl_shared_cdt_threshold.

  @param SocId                 - Socket ID
  @param CtrId                 - Controller ID
  @param BlSharedCdtThreshold  - The value of BlSharedCdtThreshold

  @retval                      - EFI_UNSUPPORTED: the function not implemented
                                 EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetBlSharedCdtThreshold (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 BlSharedCdtThreshold
  );

//
// Function pointer definiton for IioSetBlSharedCdtThreshold ()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_BL_SHARED_CDT_THRESHOLD) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 BlSharedCdtThreshold
  );

/**
  IIO: Set Irp egress credit control register's ad_shared_cdt_threshold.

  @param SocId                 - Socket ID
  @param CtrId                 - Controller ID
  @param AdSharedCdtThreshold  - The value of AdSharedCdtThreshold

  @retval                      - EFI_UNSUPPORTED: the function not implemented
                                 EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioSetAdSharedCdtThreshold (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 AdSharedCdtThreshold
  );

//
// Function pointer definiton for IioSetAdSharedCdtThreshold ()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_AD_SHARED_CDT_THRESHOLD) (
  UINT8  SocId,
  UINT8  CtrId,
  UINT32 AdSharedCdtThreshold
  );

/**
  IIO: Clear the Spare2 (Bit25) in IRP_MISC_DFX0 for p2p traffic performance.
  SPR A0, A1 and B0 WA.

  @param SocId                 - Socket ID
  @param CtrId                 - Controller ID

  @retval                      - EFI_UNSUPPORTED: the function not implemented
                                 EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioClearIrpMiscDfx0Spare2 (
  UINT8  SocId,
  UINT8  CtrId
  );

//
// Function pointer definiton for IioClearIrpMiscDfx0Spare2 ()
//
typedef
EFI_STATUS
(EFIAPI *IIO_CLEAR_IRP_MISC_DFX0_SPARE2) (
  UINT8  SocId,
  UINT8  CtrId
  );

/**
  IIO: Clear irp_ob_ncs_faf_en in IIOMISCCTRL_N1_IIO_VTD_REG for legacy DMI stack
  w/a for all SPR sku and stepping

  @param SocId                 - Socket ID
  @param CtrId                 - Controller ID

  @retval                      - EFI_UNSUPPORTED: the function not implemented
                                 EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioClearMiscCtrlN1Ncs (
  UINT8  SocId,
  UINT8  CtrId
  );

//
// Function pointer definiton for IioClearMiscCtrlN1Ncs ()
//
typedef
EFI_STATUS
(EFIAPI *IIO_CLEAR_MISC_CTRL_N1_NCS) (
  UINT8  SocId,
  UINT8  CtrId
  );

/**
  Sets IIO TOLM and TOHM base registers

  @param[in] SocId     Socket ID
  @param[in] CtrId     Controller ID
  @param[in] Tolm      TOLM in 64MB granularity
  @param[in] Tohm      TOHM in 64MB granularity

  @retval EFI_SUCCESS  IIO TOLM and TOHM is successfully set.

**/
EFI_STATUS
EFIAPI
IioSetTolmTohm (
  IN UINT8           SocId,
  IN UINT8           CtrlId,
  IN UINT32          Tolm,
  IN UINT32          Tohm
  );

//
// Function pointer definiton for IioSetTolmTohm()
//
typedef
EFI_STATUS
(EFIAPI *IIO_SET_TOLM_TOHM) (
  IN UINT8           SocId,
  IN UINT8           CtrlId,
  IN UINT32          Tolm,
  IN UINT32          Tohm
  );

typedef struct {
  //
  // Function pointers
  //
  IIO_SET_SNC_BASE_TWO_LM_MASK                    IioSetSncBaseTwoLmMask;
  IIO_DEVICE_EXISTS                               IioDeviceExists;
  IIO_SET_ROOT_BUS_BASE_EN                        IioSetRootBusBaseEn;
  IIO_SET_SNC_CFG                                 IioSetSncCfg;
  IIO_SET_NODEID                                  IioSetNodeId;
  IIO_SET_MMIOH_GRANULARITY                       IioSetMmiohGranularity;
  IIO_SET_MMIOL_GRANULARITY                       IioSetMmiolGranularity;
  IIO_SET_MMCFG_BASE                              IioSetMmcfgBase;
  IIO_SET_MMCFG_LIMIT                             IioSetMmcfgLimit;
  IIO_CFG_BUSID                                   IioCfgBusId;
  IIO_GET_UPDATED_BUSID_RULE                      IioGetUpdatedBusIdRule;
  IIO_SET_BUS_NO_CFG                              IioSetBusNoCfg;
  IIO_SET_ITC_IRP_OFL_EN                          IioSetItcIrpOflEn;
  IIO_SET_ISOC_EN                                 IioSetIsocEn;
  IIO_SET_DDRT_BASE                               IioSetDdrtBase;
  IIO_SET_DDRT_CRD_EN                             IioSetDdrtCrdEn;
  IIO_SET_ISOC_MESEG_CFG                          IioSetIsocMeSegCfg;
  IIO_NEW_MMIOL_RULE                              IioNewMmiolRule;
  IIO_NEW_MMIOL_NEM_RULE                          IioNewMmiolNemRule;
  IIO_SET_MMIOL_RULE_IDX                          IioSetMmiolRuleIdx;
  IIO_SET_MMIOL_LIST_IDX                          IioSetMmiolListIdx;
  IIO_SET_MMIOH_RULE_IDX                          IioSetMmiohRuleIdx;
  IIO_SET_WRT_COMB_DCA                            IioSetWrtCombDca;
  IIO_SET_MISC_CTRL                               IioSetMiscCtrl;
  IIO_SET_SAD_TARGETS                             IioSetSadTargets;
  IIO_SET_SNC_UNCORE                              IioSetSncUncore;
  IIO_GET_SNC_UNCORE_CFG                          IioGetSncUncoreCfg;
  IIO_IS_STARTING_ADDR_ZERO                       IioIsStartingAddrZero;
  IIO_GET_CLASS_CODE                              IioGetClassCode;
  IIO_GET_SNC_UPPER_BASE                          IioGetSncUpperBase;
  IIO_GET_SNC_2LM_ADDR_MASK                       IioGetSnc2LmAddrMask;
  IIO_SET_UMA_CLUSTER_CFG                         IioSetUmaClusterCfg;
  IIO_GET_UMA_CLUSTER_CFG                         IioGetUmaClusterCfg;
  IIO_SET_SNC_UPPER_BASE                          IioSetSncUpperBase;
  IIO_GET_MCTP_SEG_ID_SIZE                        IioGetMctpSegIdSize;
  IIO_SET_DMI_CFG_TYPE0                           IioSetDmiCfgType0;
  IIO_SET_MMIOH_LIST_IDX                          IioSetMmiohListIdx;
  IIO_NEW_MMIOH_INTER_LEAVED_RULE0                IioNewMmiohInterleavedRule0;
  IIO_NEW_MMIOH_INTER_LEAVED_RULE1                IioNewMmiohInterleavedRule1;
  IIO_NEW_MMIOH_NON_INTER_LEAVED_RULE0            IioNewMmiohNonInterleavedRule0;
  IIO_NEW_MMIOH_NON_INTER_LEAVED_RULE1            IioNewMmiohNonInterleavedRule1;
  IIO_UPDATE_SERIAL_MMIOH_NON_INTER_LEAVED_RULE1  IioUpdateSerialMmiohNonInterleavedRule1;
  IIO_SET_MMIO_RULES                              IioSetMmioRules;
  IIO_SET_ISOCH_RQT_PR_EN                         IioSetIsochRqtPrEn;
  IIO_SET_SNC_BASE_ADDR                           IioSetSncBaseAddr;
  IIO_GET_SNC_IDX                                 IioGetSncIdx;
  IIO_SET_MCTP_BROAD_CAST_SETTINGS                IioSetMctpBroadcastSettings;
  IIO_SET_MCTP_SEG_ID_SIZE                        IioSetMctpSegIdSize;
  IIO_CLEAR_DMICTRL_AUTO_COMPLETE_PM              IioClearDmictrlAutoCompletePm;
  IIO_CONFIGURE_MMIOH_INTLV_STACK_LIST            IioConfigureMmiohIntlvStackList;
  IIO_SET_DEDICATED_NCS_NCB_DRS                   IioSetDedicatedNcsNcbDrs;
  IIO_SET_BL_SHARED_CDT_THRESHOLD                 IioSetBlSharedCdtThreshold;
  IIO_SET_AD_SHARED_CDT_THRESHOLD                 IioSetAdSharedCdtThreshold;
  IIO_CLEAR_IRP_MISC_DFX0_SPARE2                  IioClearIrpMiscDfx0Spare2;
  IIO_CLEAR_MISC_CTRL_N1_NCS                      IioClearMiscCtrlN1Ncs;
  IIO_SET_TOLM_TOHM                               IioSetTolmTohm;
} IIO_IP_INTERFACE;

/**
  Write to ACS register

  @param SocId         - Socket ID
  @param CtrId         - Controller ID
  @param AcsCtrl       - Value to be written to ACS Register

  @retval                   EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully
**/

EFI_STATUS
EFIAPI
IioWriteACS (
IN  UINT8                       SocId,
IN  UINT8                       CtrId,
IN  UINT16                      AcsCtrl
);

/**
  Set MC Base Address and MC Index Position

  @param SocId                - Socket ID
  @param CtrId                - Controller ID
  @param McastIndxPos         - MultiCast Index
  @param McastBaseAddress     - MultiCast Base Adress

  @retval                   EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully
**/

EFI_STATUS
EFIAPI
IioSetMcBaseAdressAndIndexPosition (
IN  UINT8                       SocId,
IN  UINT8                       CtrId,
IN  UINT8                       McastIndxPos,
IN  EFI_PHYSICAL_ADDRESS        McastBaseAddress
);

/**
  Clear MCAST block all

  @param SocId                - Socket ID
  @param CtrId                - Controller ID

  @retval                   EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully
**/

EFI_STATUS
EFIAPI
IioClearMcBlockAll (
IN  UINT8                       SocId,
IN  UINT8                       CtrId
);


/**
  Set MC Number of groups MC_NUM_GROUP and MC_ENABLE

  @param SocId                - Socket ID
  @param CtrId                - Controller ID
  @param McastNumGrp          - MultiCast number of groups

  @retval                   EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully
**/

EFI_STATUS
EFIAPI
IioSetMcNumberOfGroupsAndMcEnable (
IN  UINT8                       SocId,
IN  UINT8                       CtrId,
IN  UINT8                       McastNumGrp
);

/**
  MMCAST Receive should be set for NTB

  @param SocId                - Socket ID
  @param CtrId                - Controller ID
  @param McastNumGrp          - MultiCast number of groups

  @retval                   EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully
**/

EFI_STATUS
EFIAPI
IioSetMmCastForNtb (
IN  UINT8                       SocId,
IN  UINT8                       CtrId,
IN  UINT8                       McastNumGrp
);

/**
  Set RCRBBar and initialize the MemBar for DMI Port --- Don't support it, except SPR

  @param[in]  SocketId             - Socket Id
  @param[in]  DmiRcrbBarAddr       - DMI RCRB Bar Address
  @param[in]  MemResBase           - The start address of available MMIO resource
  @param[out] *DmiMemBarBase       - DMI MEMBAR Base in its RCRB region
  @param[out] *DmiMemBarSize       - DMI MEMBAR Size in its RCRB region

  @retval                   EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully
**/

EFI_STATUS
EFIAPI
DmiRcrbInitialize (
IN  UINT8                       SocketId,
IN  UINT32                      DmiRcrbBarAddr,
IN  UINT32                      MemResBase,
OUT UINT32                      *DmiMemBarBase,
OUT UINT32                      *DmiMemBarSize
);

/**
  IIO: Set Spare IITC register value for CsrType1SerPerfDisable

  This function sets the register value for CsrType1SerPerfDisable

  @param SocId                   - Socket ID
  @param CtrId                   - Controller ID
  @param CsrType1SerPerfDisable  - Bit value to CsrType1SerPerfDisable

  @retval                        - EFI_UNSUPPORTED: the function not implemented
                                   EFI_SUCCESS: the function is excuted successfully
**/
EFI_STATUS
EFIAPI
IioSetType1SerPerfDisable (
  UINT8    SocId,
  UINT8    CtrId,
  BOOLEAN  CsrType1SerPerfDisable
  );
/**
  IIO:  Set Spare IITC register value for CsrMcp4sType1Enable

  This function sets the register value for CsrMcp4sType1Enable

  @param SocId                - Socket ID
  @param CtrId                - Controller ID
  @param CsrMcp4sType1Enable  - Bit value to CsrMcp4sType1Enable

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS: the function is excuted successfully
**/
EFI_STATUS
EFIAPI
IioSetMcp4sType1Enable (
  UINT8    SocId,
  UINT8    CtrId,
  BOOLEAN  CsrMcp4sType1Enable
  );
/**
  IIO: This routine programs DFX register access control in BIOS.

  @param SocId                 - Socket ID
  @param CtrId                 - Controller ID
  @param AccessEnableOrDisable - Enable or disable register access

  @retval                      - EFI_UNSUPPORTED: the function not implemented
                                 EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
IioProgramDfxRegisterAccessControl (
  UINT8  SocId,
  UINT8  CtrId,
  BOOLEAN AccessEnableOrDisable
  );

/**
  Protects a specified address range via the GENPROT mechanism.
  Long term : The routine should be implemented in IioIpLib with general
              search for available GENPROT registers.
  Short term: Implement API temporarily with a hardcoded GENPROT allocation
              until a full implementation in IioIpLib is available

  @param[in] SocId - Socket Index
  @param[in] Base  - Base of requested address range (in 64MB units)
  @param[in] Limit - Limit of requested address range (in 64MB units)

  @retval EFI_SUCCESS           Requested address range is protected
  @retval EFI_OUT_OF_RESOURCES  Requested address range is NOT protected
  @retval EFI_INVALID_PARAMETER Parameter is invalid
  @retval EFI_UNSUPPORTED       API is not supported

**/
EFI_STATUS
EFIAPI
SetGenprotRange (
  IN UINT8  SocId,
  IN UINT32 Base,
  IN UINT32 Limit
  );

//
// This API supports multiple instances with the GUID identifying the instance
//
#define IIO_IP_INTERFACE_CPU_GUID \
  { 0x96e629a1, 0x4f87, 0x434c, { 0xac, 0xa3, 0xef, 0x27, 0xca, 0x1a, 0x5f, 0x5a }} // {96E629A1-4F87-434C-ACA3-EF27CA1A5F5A}
extern EFI_GUID gIioIpInterfaceCpuGuid;

#define IIO_IP_INTERFACE_PCIE_GEN4_GUID \
  { 0x0c343cae, 0x84f7, 0x4062, { 0xa5, 0x17, 0x1d, 0x5f, 0xdb, 0xc8, 0x5c, 0xa5 }} // {0C343CAE-84F7-4062-A517-1D5FDBC85CA5}
extern EFI_GUID gIioIpInterfacePcieGen4Guid;
#endif  // __IIO_IP_INTERFACE_H__
