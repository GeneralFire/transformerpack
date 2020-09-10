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

#include <RcRegs.h>
#include <Library/DebugLib.h>
#include <Library/UsraCsrLib.h>
#include <Guid/IioIpInterface.h>
#include <Library/BaseLib.h>
#include <Upi/KtiSi.h>
#include <Library/IioAccessLib.h>
#include <KtiSetupDefinitions.h>
#include <Library/RcrbAccessLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Memory/MemDefaults.h>

#define IIO_PRQ_CREDIT_TABLE_SIZE     8

#define IIO_TOTAL_MMIOL_TGT_LIST      4

UINT32 MmiolTargetOffsetIio[IIO_TOTAL_MMIOL_TGT_LIST] = {MMIOL_TGT_LIST0_IIO_VTD_REG,
                                                         MMIOL_TGT_LIST1_IIO_VTD_REG,
                                                         MMIOL_TGT_LIST2_IIO_VTD_REG,
                                                         MMIOL_TGT_LIST3_IIO_VTD_REG};

UINT32 MmiolRuleOffsetIio[TOTAL_MMIO_VTD_PER_STACK_SPR] = {MMIOL_RULE0_IIO_VTD_REG,
                                                           MMIOL_RULE1_IIO_VTD_REG,
                                                           MMIOL_RULE2_IIO_VTD_REG,
                                                           MMIOL_RULE3_IIO_VTD_REG,
                                                           MMIOL_RULE4_IIO_VTD_REG,
                                                           MMIOL_RULE5_IIO_VTD_REG,
                                                           MMIOL_RULE6_IIO_VTD_REG,
                                                           MMIOL_RULE7_IIO_VTD_REG,
                                                           MMIOL_RULE8_IIO_VTD_REG,
                                                           MMIOL_RULE9_IIO_VTD_REG,
                                                           MMIOL_RULE10_IIO_VTD_REG,
                                                           MMIOL_RULE11_IIO_VTD_REG,
                                                           MMIOL_RULE12_IIO_VTD_REG,
                                                           MMIOL_RULE13_IIO_VTD_REG,
                                                           MMIOL_RULE14_IIO_VTD_REG,
                                                           MMIOL_RULE15_IIO_VTD_REG,
                                                           MMIOL_RULE16_IIO_VTD_REG,
                                                           MMIOL_RULE17_IIO_VTD_REG,
                                                           MMIOL_RULE18_IIO_VTD_REG,
                                                           MMIOL_RULE19_IIO_VTD_REG};

UINT32 IioMmiohRuleOffset[TOTAL_MMIO_VTD_PER_STACK_SPR][2] = {
                                     { MMIOH_RULE0_N0_IIO_VTD_REG, MMIOH_RULE0_N1_IIO_VTD_REG },
                                     { MMIOH_RULE1_N0_IIO_VTD_REG, MMIOH_RULE1_N1_IIO_VTD_REG },
                                     { MMIOH_RULE2_N0_IIO_VTD_REG, MMIOH_RULE2_N1_IIO_VTD_REG },
                                     { MMIOH_RULE3_N0_IIO_VTD_REG, MMIOH_RULE3_N1_IIO_VTD_REG },
                                     { MMIOH_RULE4_N0_IIO_VTD_REG, MMIOH_RULE4_N1_IIO_VTD_REG },
                                     { MMIOH_RULE5_N0_IIO_VTD_REG, MMIOH_RULE5_N1_IIO_VTD_REG },
                                     { MMIOH_RULE6_N0_IIO_VTD_REG, MMIOH_RULE6_N1_IIO_VTD_REG },
                                     { MMIOH_RULE7_N0_IIO_VTD_REG, MMIOH_RULE7_N1_IIO_VTD_REG },
                                     { MMIOH_RULE8_N0_IIO_VTD_REG, MMIOH_RULE8_N1_IIO_VTD_REG },
                                     { MMIOH_RULE9_N0_IIO_VTD_REG, MMIOH_RULE9_N1_IIO_VTD_REG },
                                     { MMIOH_RULE10_N0_IIO_VTD_REG,MMIOH_RULE10_N1_IIO_VTD_REG },
                                     { MMIOH_RULE11_N0_IIO_VTD_REG,MMIOH_RULE11_N1_IIO_VTD_REG },
                                     { MMIOH_RULE12_N0_IIO_VTD_REG,MMIOH_RULE12_N1_IIO_VTD_REG },
                                     { MMIOH_RULE13_N0_IIO_VTD_REG,MMIOH_RULE13_N1_IIO_VTD_REG },
                                     { MMIOH_RULE14_N0_IIO_VTD_REG,MMIOH_RULE14_N1_IIO_VTD_REG },
                                     { MMIOH_RULE15_N0_IIO_VTD_REG,MMIOH_RULE15_N1_IIO_VTD_REG },
                                     { MMIOH_RULE16_N0_IIO_VTD_REG,MMIOH_RULE16_N1_IIO_VTD_REG },
                                     { MMIOH_RULE17_N0_IIO_VTD_REG,MMIOH_RULE17_N1_IIO_VTD_REG },
                                     { MMIOH_RULE18_N0_IIO_VTD_REG,MMIOH_RULE18_N1_IIO_VTD_REG },
                                     { MMIOH_RULE19_N0_IIO_VTD_REG,MMIOH_RULE19_N1_IIO_VTD_REG }};

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
  )
{
  SNC_BASE_1_IIO_VTD_STRUCT SncBase1_Iio;
  SNC_BASE_2_IIO_VTD_STRUCT SncBase2_Iio;

  SncBase1_Iio.Data = UsraCsrRead (SocId, CtrId, SNC_BASE_1_IIO_VTD_REG);
  SncBase1_Iio.Bits.address_mask_45_40 = TwoLmMask1Lo;
  SncBase1_Iio.Bits.address_mask_hi_51_46 = TwoLmMask1Hi;
  UsraCsrWrite (SocId, CtrId, SNC_BASE_1_IIO_VTD_REG, SncBase1_Iio.Data);

  SncBase2_Iio.Data = UsraCsrRead (SocId, CtrId, SNC_BASE_2_IIO_VTD_REG);
  SncBase2_Iio.Bits.address_mask_39_28 = TwoLmMask2;
  UsraCsrWrite (SocId, CtrId, SNC_BASE_2_IIO_VTD_REG, SncBase2_Iio.Data);

  return EFI_SUCCESS;
}

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
  )
{
  //
  //Empty function to keep API consistency
  //
  *DeviceExists = FALSE;
  return EFI_SUCCESS;
}

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
  )
{
  ROOTBUS_IIO_VTD_STRUCT RootBus;

  RootBus.Data = UsraCsrRead (SocId, CtrId, ROOTBUS_IIO_VTD_REG);
  RootBus.Bits.rootbuss = BusBase;
  RootBus.Bits.rootbuss_valid = 1;
  RootBus.Bits.rootbusc = BusNumC;
  UsraCsrWrite (SocId, CtrId, ROOTBUS_IIO_VTD_REG, RootBus.Data);

  return EFI_SUCCESS;
}

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
  )
{
  UsraCsrWrite (SocId, CtrId, SNC_CONFIG_IIO_VTD_REG, SncConfig);

  return EFI_SUCCESS;
}

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
  )
{
  //
  //Empty function to keep API consistency
  //
  return EFI_SUCCESS;
}

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
  )
{
  ITCERRDIS_IIO_DFX_VTD_STRUCT ItcErrDis;
  ItcErrDis.Data = UsraCsrRead (SocId, CtrId, ITCERRDIS_IIO_DFX_VTD_REG);
  ItcErrDis.Bits.itc_irp_cred_of = OverFlowEn & 0x1;
  UsraCsrWrite (SocId, CtrId, ITCERRDIS_IIO_DFX_VTD_REG, ItcErrDis.Data);

  return EFI_SUCCESS;
}

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
  )
{
  LOWBWMEM_BASE_N0_IIO_VTD_STRUCT   LowBwMemBaseN0;
  LOWBWMEM_BASE_N1_IIO_VTD_STRUCT   LowBwMemBaseN1;
  LOWBWMEM_LIMIT_N0_IIO_VTD_STRUCT  LowBwMemLimitN0;
  LOWBWMEM_LIMIT_N1_IIO_VTD_STRUCT  LowBwMemLimitN1;

  LowBwMemBaseN0.Data  = 0;
  LowBwMemBaseN1.Data  = 0;
  LowBwMemLimitN0.Data = 0;
  LowBwMemLimitN1.Data = 0;

  LowBwMemBaseN0.Bits.addr_range_field = DdrtBase & MaskLo;
  LowBwMemBaseN1.Bits.addr_range_field = DdrtBase >> MaskHi;
  UsraCsrWrite (SocId, CtrId, LOWBWMEM_BASE_N0_IIO_VTD_REG, LowBwMemBaseN0.Data);
  UsraCsrWrite (SocId, CtrId, LOWBWMEM_BASE_N1_IIO_VTD_REG, LowBwMemBaseN1.Data);

  LowBwMemLimitN0.Bits.addr_range_field = DdrtLimit & MaskLo;
  LowBwMemLimitN1.Bits.addr_range_field = DdrtLimit >> MaskHi;
  UsraCsrWrite (SocId, CtrId, LOWBWMEM_LIMIT_N0_IIO_VTD_REG, LowBwMemLimitN0.Data);
  UsraCsrWrite (SocId, CtrId, LOWBWMEM_LIMIT_N1_IIO_VTD_REG, LowBwMemLimitN1.Data);

  return EFI_SUCCESS;
}

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
  )
{
  LOWBWMEM_CTRL_IIO_VTD_STRUCT LowBwMemCtrl;

  LowBwMemCtrl.Data = UsraCsrRead (SocId, CtrId, LOWBWMEM_CTRL_IIO_VTD_REG);
  LowBwMemCtrl.Bits.rdtrackerlowbwcredits = 0x20;
  LowBwMemCtrl.Bits.wrtrackerlowbwcredits = 0x10;
  if (DdrtQosMode == DDRT_QOS_MODE_2) {
    LowBwMemCtrl.Bits.rdtrackerlowbwcrediten = 0x01;
    LowBwMemCtrl.Bits.wrtrackerlowbwcrediten = 0x01;
  } else {
    LowBwMemCtrl.Bits.rdtrackerlowbwcrediten = 0;
    LowBwMemCtrl.Bits.wrtrackerlowbwcrediten = 0;
  }
  UsraCsrWrite (SocId, CtrId, LOWBWMEM_CTRL_IIO_VTD_REG, LowBwMemCtrl.Data);

  return EFI_SUCCESS;
}

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
  )
{
  if (RuleIdx >= TOTAL_MMIO_VTD_PER_STACK_SPR) {
    DEBUG ((DEBUG_ERROR, "ERROR!! out of bounds access detected\n"));
    ASSERT (RuleIdx < TOTAL_MMIO_VTD_PER_STACK_SPR);
    return EFI_INVALID_PARAMETER;
  }
  UsraCsrWrite (SocId, CtrId, MmiolRuleOffsetIio[RuleIdx], RuleData);

  return EFI_SUCCESS;
}

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
  )
{
  UINT8     RegIndex = 0;
  UINT8     TargetIndex = 0;
  UINT32    Data32 = 0;
  UINT32    FieldMask = 0;
  UINT32    RegNum = 0;

  RegNum = ((DecoderNum % 6) == 0) ? (DecoderNum / 6) : (DecoderNum / 6 + 1);

  for (RegIndex = 0; RegIndex < RegNum; RegIndex++) {
    Data32 = UsraCsrRead (SocId, CtrId, MmiolTargetOffsetIio[RegIndex]);

    for (TargetIndex = 0; (TargetIndex < 6) && (TargetIndex < DecoderNum - 6 * RegIndex); TargetIndex++) {
      FieldMask = ~FLD_MASK(5, (TargetIndex * 5));
      if (ListData[RegIndex*6 + TargetIndex] & SOCKET_LOCAL_FLAG) {
        Data32 = (Data32 & FieldMask) | ((ListData[RegIndex*6 + TargetIndex] & 0x1F | BIT4) <<  (5 * TargetIndex));
      } else if (ListData[RegIndex*6 + TargetIndex] & SOCKET_REMOTE_FLAG)  {
        Data32 = (Data32 & FieldMask) | ((ListData[RegIndex*6 + TargetIndex] & 0x1F) << (5 * TargetIndex));
      } else {
        Data32 = (Data32 & FieldMask) | (BIT4 <<  (5 * TargetIndex));
      }
    }
    UsraCsrWrite (SocId, CtrId, MmiolTargetOffsetIio[RegIndex], Data32);
  }

  return EFI_SUCCESS;
}

/**
  IIO:  Set the MMIOH rule using an index

  @param SocId          - Socket ID
  @param CtrId          - Controller ID
  @param Target         - The target
  @param TgtIdx         - The target index
  @param SerializerRule - The serializer rule
  @param RemoteStackId  - The remote stack ID
  @param BaseHi         - The MMIOH base high part
  @param BaseLo         - The MMIOH base low part
  @param LimitHi        - The MMIOH limit high part
  @param LimitLo        - The MMIOH limit low part

  @retval               - EFI_UNSUPPORTED: the function not implemented
                          EFI_INVALID_PARAMETER: A required parameter was NULL or is out of range.
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
  )
{
  MMIOH_RULE0_N0_IIO_VTD_STRUCT IioMmiohRuleLo;
  MMIOH_RULE0_N1_IIO_VTD_STRUCT IioMmiohRuleHi;

  if (TgtIdx >= TOTAL_MMIO_VTD_PER_STACK_SPR) {
    DEBUG ((DEBUG_ERROR, "ERROR!! out of bounds access detected\n"));
    ASSERT (TgtIdx < TOTAL_MMIO_VTD_PER_STACK_SPR);
    return EFI_INVALID_PARAMETER;
  }

  IioMmiohRuleLo.Data = 0;
  IioMmiohRuleHi.Data = 0;
  //
  // For valid Base and Limit, then programm the fileds of registers
  //
  if ((LShiftU64 (BaseHi, 32) + BaseLo) < (LShiftU64 (LimitHi, 32) + LimitLo)) {
    IioMmiohRuleLo.Bits.ruleenable = 1;
    IioMmiohRuleLo.Bits.serializerule = SerializerRule;
    IioMmiohRuleLo.Bits.target = Target;
    IioMmiohRuleLo.Bits.baseaddress  = (BaseLo  >> 30) | ((BaseHi & 0xFFFFF) << 2);
    IioMmiohRuleLo.Bits.limitaddress = (LimitLo >> 30);
    IioMmiohRuleHi.Bits.limitaddress = (LimitHi & 0xFFFFF);
    IioMmiohRuleHi.Bits.remotestackid = RemoteStackId;
  }
  UsraCsrWrite (SocId, CtrId, IioMmiohRuleOffset[TgtIdx][1], IioMmiohRuleHi.Data);
  UsraCsrWrite (SocId, CtrId, IioMmiohRuleOffset[TgtIdx][0], IioMmiohRuleLo.Data);

  return EFI_SUCCESS;
}

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
  )
{
  CIPCTRL_IIO_VTD_STRUCT Cipctrl;

  Cipctrl.Data = UsraCsrRead (SocId, CtrId, CIPCTRL_IIO_VTD_REG);
  Cipctrl.Bits.diswrcomb = WrtComb;
  UsraCsrWrite (SocId, CtrId, CIPCTRL_IIO_VTD_REG, Cipctrl.Data);

  return EFI_SUCCESS;
}

/**
  IIO:  Set value of register MiscCtrl

  @param SocId             - Socket ID
  @param CtrId             - Controller ID
  @param OutLegacyVgaSoc   - OutLegacyVgaSoc current value
  @param SplitLock         - SplitLock setup value
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
  )
{
  TOCM_IIO_VTD_STRUCT           IioTocm;
  IIOMISCCTRL_N0_IIO_VTD_STRUCT IioMiscCtrl;

  IioMiscCtrl.Data = UsraCsrRead (SocId, CtrId, IIOMISCCTRL_N0_IIO_VTD_REG);

  if (OutLegacyVgaSoc == SocId) {
    IioMiscCtrl.Bits.rvgaen = 0;
  } else {
    IioMiscCtrl.Bits.rvgaen = 1;
  }

  IioTocm.Data = (UINT8) UsraCsrRead (SocId, CtrId, TOCM_IIO_VTD_REG);
  IioTocm.Bits.tocm = MaxAddrLine - 32;
  IioTocm.Bits.tocmvalid = 1;
  UsraCsrWrite (SocId, CtrId, TOCM_IIO_VTD_REG, (UINT32) IioTocm.Data);
  //
  // Todo: Need to recheck for SPR
  //
  //Only DMI stack needs this setting
/*
  if (OrderingFlag == TRUE) {
    IioMiscCtrlDmi.Data = UsraCsrRead (SocId, CtrId, IIOMISCCTRL_MODE_DMI_0_5_0_N0_IIO_PCIEDMI_REG);
    IioMiscCtrlDmi.Bits.dmi_vc1_write_ordering = 0x1;
    IioMiscCtrlDmi.Bits.dmi_vc1_vt_d_fetch_ordering = 0x0;
    UsraCsrWrite (SocId, CtrId, IIOMISCCTRL_MODE_DMI_0_5_0_N0_IIO_PCIEDMI_REG, IioMiscCtrlDmi.Data);
  }
*/
  if (SplitLock) {
    IioMiscCtrl.Bits.splitlockopt = 1;
  }

  UsraCsrWrite (SocId, CtrId, IIOMISCCTRL_N0_IIO_VTD_REG, IioMiscCtrl.Data);

  return EFI_SUCCESS;
}

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
  )
{
  UNCORE_SNC_IIO_VTD_SPRA0_STRUCT                               UncoreSncCfgA0;
  UNCORE_SNC_IIO_VTD_SPRB0_SPRHBM_SPRC0_SPRMCC_SPRUCC_STRUCT    UncoreSncCfgB0C0Hbm;

  if (IsCpuAndRevision (CPU_SPRSP, REV_A0)) {
    UncoreSncCfgA0.Data = UsraCsrRead (SocId, CtrId, UNCORE_SNC_IIO_VTD_REG);
    UncoreSncCfgA0.Bits.num_chas_per_cluster = NumChaCluster;
    UncoreSncCfgA0.Bits.base_cha_cluster_1   = BaseChaCluster1;
    UncoreSncCfgA0.Bits.base_cha_cluster_2   = BaseChaCluster2;
    UncoreSncCfgA0.Bits.base_cha_cluster_3   = BaseChaCluster3;
    UsraCsrWrite (SocId, CtrId, UNCORE_SNC_IIO_VTD_REG, UncoreSncCfgA0.Data);
  } else if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) {
    UncoreSncCfgB0C0Hbm.Data = UsraCsrRead (SocId, CtrId, UNCORE_SNC_IIO_VTD_REG);
    UncoreSncCfgB0C0Hbm.Bits.num_chas_per_cluster = NumChaCluster;
    UncoreSncCfgB0C0Hbm.Bits.base_cha_cluster_1   = BaseChaCluster1;
    UncoreSncCfgB0C0Hbm.Bits.base_cha_cluster_2   = BaseChaCluster2;
    UncoreSncCfgB0C0Hbm.Bits.base_cha_cluster_3   = BaseChaCluster3;
    UsraCsrWrite (SocId, CtrId, UNCORE_SNC_IIO_VTD_REG, UncoreSncCfgB0C0Hbm.Data);
  }

  return EFI_SUCCESS;
}

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
  )
{
  UNCORE_SNC_IIO_VTD_SPRA0_STRUCT                               UncoreSncCfgA0;
  UNCORE_SNC_IIO_VTD_SPRB0_SPRHBM_SPRC0_SPRMCC_SPRUCC_STRUCT    UncoreSncCfgB0C0Hbm;

  if (IsCpuAndRevision (CPU_SPRSP, REV_A0)) {
    UncoreSncCfgA0.Data = UsraCsrRead (SocId, CtrId, UNCORE_SNC_IIO_VTD_REG);
    *NumChaPerCluster = (UINT8) UncoreSncCfgA0.Bits.num_chas_per_cluster;
    *BaseChaCluster1  = (UINT8) UncoreSncCfgA0.Bits.base_cha_cluster_1;
    *BaseChaCluster2  = (UINT8) UncoreSncCfgA0.Bits.base_cha_cluster_2;
    *BaseChaCluster3  = (UINT8) UncoreSncCfgA0.Bits.base_cha_cluster_3;
  } else if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) {
    UncoreSncCfgB0C0Hbm.Data = UsraCsrRead (SocId, CtrId, UNCORE_SNC_IIO_VTD_REG);
    *NumChaPerCluster = (UINT8) UncoreSncCfgB0C0Hbm.Bits.num_chas_per_cluster;
    *BaseChaCluster1  = (UINT8) UncoreSncCfgB0C0Hbm.Bits.base_cha_cluster_1;
    *BaseChaCluster2  = (UINT8) UncoreSncCfgB0C0Hbm.Bits.base_cha_cluster_2;
    *BaseChaCluster3  = (UINT8) UncoreSncCfgB0C0Hbm.Bits.base_cha_cluster_3;
  }

  return EFI_SUCCESS;
}

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
  )
{
  SNC_BASE_1_IIO_VTD_STRUCT  SncBase;
  SncBase.Data = UsraCsrRead (SocId, CtrId, SNC_BASE_1_IIO_VTD_REG);
  *IsStartingAddrZero = (SncBase.Bits.starting_addr_0 == 0);

  return EFI_SUCCESS;
}

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
  )
{
  *ClassCode = UsraCsrRead (SocId, CtrId, RID_CCR_IIO_M2IAL_REG) >> 8;

  return EFI_SUCCESS;
}

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
  )
{
  *SncUpperBase = UsraCsrRead (SocId, CtrId, SNC_UPPER_BASE_IIO_VTD_REG);

  return EFI_SUCCESS;
}

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
  )
{
  SNC_BASE_1_IIO_VTD_STRUCT SncBase1_Iio;
  SNC_BASE_2_IIO_VTD_STRUCT SncBase2_Iio;

  SncBase1_Iio.Data = UsraCsrRead (SocId, CtrId, SNC_BASE_1_IIO_VTD_REG);
  SncBase2_Iio.Data = UsraCsrRead (SocId, CtrId, SNC_BASE_2_IIO_VTD_REG);

  *Snc2LmAddrMask = SncBase2_Iio.Bits.address_mask_39_28 + \
                    (SncBase1_Iio.Bits.address_mask_45_40 << 12) + \
                    (SncBase1_Iio.Bits.address_mask_hi_51_46 << 18);

  return EFI_SUCCESS;
}

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
  )
{
  UMA_CLUSTER_CONFIG_IIO_VTD_STRUCT  UmaClusterCfg;

  UmaClusterCfg.Data = UsraCsrRead (SocId, CtrId, UMA_CLUSTER_CONFIG_IIO_VTD_REG);
  UmaClusterCfg.Bits.uma_clustering_enable = ((UmaClusterEnable == 0) ? 0 : 1);
  UmaClusterCfg.Bits.uma_defeature_xor = DefeatureXor;
  UsraCsrWrite (SocId, CtrId, UMA_CLUSTER_CONFIG_IIO_VTD_REG, UmaClusterCfg.Data);

  return EFI_SUCCESS;
}

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
  )
{
  UMA_CLUSTER_CONFIG_IIO_VTD_STRUCT  UmaClusterCfg;
  UmaClusterCfg.Data = UsraCsrRead (SocId, CtrId, UMA_CLUSTER_CONFIG_IIO_VTD_REG);
  *UmaClusterEn = (UINT8)UmaClusterCfg.Bits.uma_clustering_enable;
  *XorDefeature = (UINT8)UmaClusterCfg.Bits.uma_defeature_xor;

  return EFI_SUCCESS;
}

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
  )
{
  UsraCsrWrite (SocId, CtrId, SNC_UPPER_BASE_IIO_VTD_REG, SncUpperBase);

  return EFI_SUCCESS;
}

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
  )
{
  ITCCTRL23_N1_IIO_VTD_STRUCT  ItcCtrl23;

  ItcCtrl23.Data = UsraCsrRead (SocId, CtrId, ITCCTRL23_N1_IIO_VTD_REG);
  *MctpSegIdSize  = (UINT8)ItcCtrl23.Bits.mctp_segid_size;

  return EFI_SUCCESS;
}

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
  )
{
  SWDBGCTL0_IIO_DFX_VTD_STRUCT SwDbgCtrl0;
  SwDbgCtrl0.Data = UsraCsrRead (SocId, CtrId, SWDBGCTL0_IIO_DFX_VTD_REG);
  SwDbgCtrl0.Bits.set_dmi_cfg_type0 = Type0 & 0x1;
  UsraCsrWrite (SocId, CtrId, SWDBGCTL0_IIO_DFX_VTD_REG, SwDbgCtrl0.Data);

  return EFI_SUCCESS;
}

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
  )
{
  //
  //Empty function to keep API consistency
  //
  return EFI_SUCCESS;
}

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
  )
{
  //
  //Empty function to keep API consistency
  //
  *NewMmiohInterleavedRule0 = 0;
  return EFI_SUCCESS;
}

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
  )
{
  //
  //Empty function to keep API consistency
  //
  *NewMmiohInterleavedRule1 = 0;
  return EFI_SUCCESS;
}

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
  )
{
  //
  //Empty function to keep API consistency
  //
  *NewMmiohNonInterleavedRule0 = 0;
  return EFI_SUCCESS;
}

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
  )
{
  //
  //Empty function to keep API consistency
  //
  *NewMmiohNonInterleavedRule1 = 0;
  return EFI_SUCCESS;
}

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
  )
{
  //
  //Empty function to keep API consistency
  //
  *UpdateSerialMmiohNonInterleavedRule1 = 0;
  return EFI_SUCCESS;
}

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
  )
{
  //
  //Empty function to keep API consistency
  //
  return EFI_SUCCESS;
}

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
  )
{
  //
  //Empty function to keep API consistency
  //
  return EFI_SUCCESS;
}

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
  )
{
  UsraCsrWrite (SocId, CtrId, BUSID_GLOB_RULE_IIO_VTD_REG, GlobBusIdRule);
  UsraCsrWrite (SocId, CtrId, BUSID_GLOB_RULE_TGT_IIO_VTD_REG, MmcfgTarget);
  UsraCsrWrite (SocId, CtrId, BUSID_LOCAL_RULE_ADDRESS_0_IIO_VTD_REG, MmcfgLocalRule[0]);
  UsraCsrWrite (SocId, CtrId, BUSID_LOCAL_RULE_ADDRESS_1_IIO_VTD_REG, MmcfgLocalRule[1]);
  UsraCsrWrite (SocId, CtrId, BUSID_LOCAL_RULE_ADDRESS_2_IIO_VTD_REG, MmcfgLocalRule[2]);
  UsraCsrWrite (SocId, CtrId, BUSID_LOCAL_RULE_ADDRESS_3_IIO_VTD_REG, MmcfgLocalRule[3]);
  UsraCsrWrite (SocId, CtrId, BUSID_LOCAL_RULE_TGT_IIO_VTD_REG, MmcfgTargetLocal[0]);
  UsraCsrWrite (SocId, CtrId, BUSID_LOCAL_RULE_TGT1_IIO_VTD_REG, MmcfgTargetLocal[1]);

  return EFI_SUCCESS;
}

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
  )
{
  return EFI_SUCCESS;
}

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
)
{
  UsraCsrWrite (SocId, CtrId, ACSCAP_IIO_PCIE_G5_REG, (UINT32)AcsCtrl);

  return EFI_SUCCESS;
}

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
)
{
//TODO: Need to be updated for SPR 0p5 register drop!!

  return EFI_SUCCESS;
}

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
)
{
  //TODO: Need to be updated for SPR 0p5 register drop!!

  return EFI_SUCCESS;
}

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
)
{
  //TODO: Need to be updated for SPR 0p5 register drop!!

  return EFI_SUCCESS;
}

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
)
{
  //TODO: Need to be updated for SPR 0p5 register drop!!

  return EFI_SUCCESS;
}

/**
  Set RCRBBar and initialize the MemBar for DMI Port

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
)
{
  UINT64       MemBarBase, MemBarSize;
  EFI_STATUS   Status;

  //
  // Set the SocketId's stack0's RCRB BAR
  //
  SetRcrbBar (SocketId, 0, TYPE_DMI_RCRB, DmiRcrbBarAddr);

  //
  // Set the Sbsp's stack0's MemBar in RCRB region
  //
  Status = ProgramMemBar0Register (DmiRcrbBarAddr,
                                   (EXPPTMBAR_N0_IIO_PCIEDMI_REG & 0xFFF),
                                   MemResBase,
                                   &MemBarBase,
                                   &MemBarSize
                                  );
  if (!EFI_ERROR (Status)) {
    if ((RShiftU64 (MemBarBase, 32) == 0) && (RShiftU64 (MemBarSize, 32) == 0)) {
      *DmiMemBarBase = (UINT32)MemBarBase;
      *DmiMemBarSize = (UINT32)MemBarSize;
      return Status;
    } else {
      return EFI_UNSUPPORTED;
    }
  }

  return Status;
}

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
  )
{
  CSIPISOCRES_IIO_DFX_VTD_STRUCT CSipiSocRes;
  CSipiSocRes.Data = UsraCsrRead (SocId, CtrId, CSIPISOCRES_IIO_DFX_VTD_REG);
  CSipiSocRes.Bits.isoc_enabled = IsocEn & 0x1;
  UsraCsrWrite (SocId, CtrId, CSIPISOCRES_IIO_DFX_VTD_REG, CSipiSocRes.Data);

  return EFI_SUCCESS;
}

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
  )
{
  R2P2PNCBDEDCRED0_IIO_DFX_VTD_STRUCT NcbCred0;
  R2P2PNCBDEDCRED1_IIO_DFX_VTD_STRUCT NcbCred1;
  R2P2PNCBDEDCRED2_IIO_DFX_VTD_STRUCT NcbCred2;
  R2P2PNCBDEDCRED3_IIO_DFX_VTD_STRUCT NcbCred3;

  R2P2PNCSDEDCRED0_IIO_DFX_VTD_STRUCT NcsCred0;
  R2P2PNCSDEDCRED1_IIO_DFX_VTD_STRUCT NcsCred1;
  R2P2PNCSDEDCRED2_IIO_DFX_VTD_STRUCT NcsCred2;
  R2P2PNCSDEDCRED3_IIO_DFX_VTD_STRUCT NcsCred3;

  R2P2PDRSDEDCRED0_IIO_DFX_VTD_STRUCT DrsCred0;
  R2P2PDRSDEDCRED1_IIO_DFX_VTD_STRUCT DrsCred1;
  R2P2PDRSDEDCRED2_IIO_DFX_VTD_STRUCT DrsCred2;
  R2P2PDRSDEDCRED3_IIO_DFX_VTD_STRUCT DrsCred3;

  NcbCred0.Data = NcbCred1.Data = NcbCred2.Data = NcbCred3.Data = 0;
  NcsCred0.Data = NcsCred1.Data = NcsCred2.Data = NcsCred3.Data = 0;
  DrsCred0.Data = DrsCred1.Data = DrsCred2.Data = DrsCred3.Data = 0;

  NcsCred0.Bits.meshagent0_ncsdedcr  = Ncs[0];  //Stack0
  NcsCred0.Bits.meshagent1_ncsdedcr  = Ncs[1];  //Stack1
  NcsCred2.Bits.meshagent8_ncsdedcr  = Ncs[8];  //Stack8
  NcsCred3.Bits.meshagent12_ncsdedcr = Ncs[12]; //UPI0

  UsraCsrWrite (SocId, CtrId, R2P2PNCSDEDCRED0_IIO_DFX_VTD_REG, NcsCred0.Data);
  UsraCsrWrite (SocId, CtrId, R2P2PNCSDEDCRED1_IIO_DFX_VTD_REG, NcsCred1.Data);
  UsraCsrWrite (SocId, CtrId, R2P2PNCSDEDCRED2_IIO_DFX_VTD_REG, NcsCred2.Data);
  UsraCsrWrite (SocId, CtrId, R2P2PNCSDEDCRED3_IIO_DFX_VTD_REG, NcsCred3.Data);

  NcbCred0.Bits.meshagent0_ncbdedcr  = Ncb[0];  //Stack0
  NcbCred0.Bits.meshagent1_ncbdedcr  = Ncb[1];  //Stack1
  NcbCred2.Bits.meshagent8_ncbdedcr  = Ncb[8];  //Stack8
  NcbCred3.Bits.meshagent12_ncbdedcr = Ncb[12]; //UPI0

  UsraCsrWrite (SocId, CtrId, R2P2PNCBDEDCRED0_IIO_DFX_VTD_REG, NcbCred0.Data);
  UsraCsrWrite (SocId, CtrId, R2P2PNCBDEDCRED1_IIO_DFX_VTD_REG, NcbCred1.Data);
  UsraCsrWrite (SocId, CtrId, R2P2PNCBDEDCRED2_IIO_DFX_VTD_REG, NcbCred2.Data);
  UsraCsrWrite (SocId, CtrId, R2P2PNCBDEDCRED3_IIO_DFX_VTD_REG, NcbCred3.Data);

  DrsCred3.Bits.meshagent12_drsdedcr  = Drs[12]; //UPI0

  UsraCsrWrite (SocId, CtrId, R2P2PDRSDEDCRED0_IIO_DFX_VTD_REG, DrsCred0.Data);
  UsraCsrWrite (SocId, CtrId, R2P2PDRSDEDCRED1_IIO_DFX_VTD_REG, DrsCred1.Data);
  UsraCsrWrite (SocId, CtrId, R2P2PDRSDEDCRED2_IIO_DFX_VTD_REG, DrsCred2.Data);
  UsraCsrWrite (SocId, CtrId, R2P2PDRSDEDCRED3_IIO_DFX_VTD_REG, DrsCred3.Data);

  return EFI_SUCCESS;
}


/**
  IIO: Set Irp egress credit control register's bl_shared_cdt_threshold.

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
  )
{
  IRPEGCREDITS2_IIO_DFX_VTD_STRUCT   IrpEgCredits2;

  IrpEgCredits2.Data = UsraCsrRead (SocId, CtrId, IRPEGCREDITS2_IIO_DFX_VTD_REG);
  IrpEgCredits2.Bits.bl_shared_cdt_threshold = BlSharedCdtThreshold;
  UsraCsrWrite (SocId, CtrId, IRPEGCREDITS2_IIO_DFX_VTD_REG, IrpEgCredits2.Data);
  return EFI_SUCCESS;
}

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
  )
{
  IRPEGCREDITS_IIO_DFX_VTD_STRUCT   IrpEgCredits;

  IrpEgCredits.Data = UsraCsrRead (SocId, CtrId, IRPEGCREDITS_IIO_DFX_VTD_REG);
  IrpEgCredits.Bits.ad_shared_cdt_threshold = AdSharedCdtThreshold;
  UsraCsrWrite (SocId, CtrId, IRPEGCREDITS_IIO_DFX_VTD_REG, IrpEgCredits.Data);
  return EFI_SUCCESS;
}

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
  )
{
  IRP_MISC_DFX0_IIO_DFX_VTD_STRUCT   IrpMiscDfx0;

  IrpMiscDfx0.Data = UsraCsrRead (SocId, CtrId, IRP_MISC_DFX0_IIO_DFX_VTD_REG);
  //
  // Spare2 (BIT25) is in SV_HOOK.
  //
  IrpMiscDfx0.Data &= (~BIT25);
  UsraCsrWrite (SocId, CtrId, IRP_MISC_DFX0_IIO_DFX_VTD_REG, IrpMiscDfx0.Data);
  return EFI_SUCCESS;
}

/**
  IIO: Clear irp_ob_ncs_faf_en in IIOMISCCTRL_N1_IIO_VTD_REG
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
  )
{
  IIOMISCCTRL_N1_IIO_VTD_STRUCT IioMiscCtrlN1;

  IioMiscCtrlN1.Data = UsraCsrRead (SocId, CtrId, IIOMISCCTRL_N1_IIO_VTD_REG);
  IioMiscCtrlN1.Bits.irp_ob_ncs_faf_en = 0;
  UsraCsrWrite (SocId, CtrId, IIOMISCCTRL_N1_IIO_VTD_REG, IioMiscCtrlN1.Data);

  return EFI_SUCCESS;
}

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
  )
{
  DFX_PG_RAC_N0_IIO_VTD_STRUCT   DfxPgRacN0;
  DFX_PG_WAC_N0_IIO_VTD_STRUCT   DfxPgWacN0;

  DfxPgRacN0.Data = UsraCsrRead (SocId, CtrId, DFX_PG_RAC_N0_IIO_VTD_REG);
  DfxPgWacN0.Data = UsraCsrRead (SocId, CtrId, DFX_PG_WAC_N0_IIO_VTD_REG);

  DfxPgRacN0.Bits.dfx_sai_pol_4 = AccessEnableOrDisable;
  DfxPgWacN0.Bits.dfx_sai_pol_4 = AccessEnableOrDisable;

  UsraCsrWrite (SocId, CtrId, DFX_PG_RAC_N0_IIO_VTD_REG, DfxPgRacN0.Data);
  UsraCsrWrite (SocId, CtrId, DFX_PG_WAC_N0_IIO_VTD_REG, DfxPgWacN0.Data);

  return EFI_SUCCESS;
}