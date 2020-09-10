/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#define IIO_TOTAL_CLUSTERS_REGS  5

UINT32 IioSncBaseRegisterOffset[IIO_TOTAL_CLUSTERS_REGS] = {SNC_BASE_1_IIO_VTD_REG,
                                                            SNC_BASE_2_IIO_VTD_REG,
                                                            SNC_BASE_3_IIO_VTD_REG,
                                                            SNC_BASE_4_IIO_VTD_REG,
                                                            SNC_BASE_5_IIO_VTD_REG};

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
  )
{
  SAD_CONTROL_IIO_VTD_STRUCT SadControlIio;
  SadControlIio.Data = UsraCsrRead (SocId, CtrId, SAD_CONTROL_IIO_VTD_REG);
  SadControlIio.Bits.local_nodeid = SocId;
  UsraCsrWrite (SocId, CtrId, SAD_CONTROL_IIO_VTD_REG, SadControlIio.Data);

  return EFI_SUCCESS;
}

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
  )
{
  SAD_CONTROL_IIO_VTD_STRUCT SadControlIio;
  //
  // Mmioh granularity.
  // lsb of address check for serialize rule 2 is mmiol_granularity + 24.
  // So when allocating high MMIOH with a minimum granularity of 16MB per IIO stack, program to 0, 32MB, program to 1, etc.
  //
  SadControlIio.Data = UsraCsrRead (SocId, CtrId, SAD_CONTROL_IIO_VTD_REG);
  SadControlIio.Bits.mmioh_granularity = MmiohGranularity - 24;
  UsraCsrWrite (SocId, CtrId, SAD_CONTROL_IIO_VTD_REG, SadControlIio.Data);

  return EFI_SUCCESS;
}

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
  )
{
  SAD_CONTROL_IIO_VTD_STRUCT SadControlIio;
  //
  // Mmiol granularity.
  // lsb of address check for serialize rule 2 is mmiol_granularity + 22.
  // So when allocating low MMIO with a minimum granularity of 4MB per IIO stack program to 0, for 8MB program to 1, etc.
  //
  SadControlIio.Data = UsraCsrRead (SocId, CtrId, SAD_CONTROL_IIO_VTD_REG);
  SadControlIio.Bits.mmiol_granularity = MmiolGranularity - 22;
  UsraCsrWrite (SocId, CtrId, SAD_CONTROL_IIO_VTD_REG, SadControlIio.Data);

  return EFI_SUCCESS;
}

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
  )
{
  MMCFG_BASE_N0_IIO_VTD_STRUCT MmcfgBase0;
  MMCFG_BASE_N1_IIO_VTD_STRUCT MmcfgBase1;

  MmcfgBase0.Data = UsraCsrRead (SocId, CtrId, MMCFG_BASE_N0_IIO_VTD_REG);
  MmcfgBase0.Bits.mmcfg_base_addr = MmcfgBase >> 26;
  UsraCsrWrite (SocId, CtrId, MMCFG_BASE_N0_IIO_VTD_REG, MmcfgBase0.Data);

  MmcfgBase1.Data = UsraCsrRead (SocId, CtrId, MMCFG_BASE_N1_IIO_VTD_REG);
  MmcfgBase1.Bits.mmcfg_base_addr = 0;
  UsraCsrWrite (SocId, CtrId, MMCFG_BASE_N1_IIO_VTD_REG, MmcfgBase1.Data);

  return EFI_SUCCESS;
}

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
  )
{
  MMCFG_LIMIT_N1_IIO_VTD_STRUCT  MmcfgLimit1;
  MMCFG_LIMIT_N0_IIO_VTD_STRUCT  MmcfgLimit0;

  MmcfgLimit0.Data = UsraCsrRead (SocId, CtrId, MMCFG_LIMIT_N0_IIO_VTD_REG);
  MmcfgLimit0.Bits.mmcfg_limit_addr = MmcfgLimit >> 26;
  UsraCsrWrite (SocId, CtrId, MMCFG_LIMIT_N0_IIO_VTD_REG, MmcfgLimit0.Data);

  MmcfgLimit1.Data = UsraCsrRead (SocId, CtrId, MMCFG_LIMIT_N1_IIO_VTD_REG);
  MmcfgLimit1.Bits.mmcfg_limit_addr = 0;
  UsraCsrWrite (SocId, CtrId, MMCFG_LIMIT_N1_IIO_VTD_REG, MmcfgLimit1.Data);

  return EFI_SUCCESS;
}

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
  )
{
  BUSID_GLOB_RULE_IIO_VTD_STRUCT GlobBusIdRule;
  // s5372108: [FPGA] ME in Operational mode causes Inband PECI messages to FPGA
  // FPGA does not support inband PECI messages and will not respond to them. SPS FW is placing a fix to
  // restrict inband PECI cycles to listed valid targets in iio_busid_glob_rule_b0d05f0 register.
  //
  // Clear the related bits which indicate the bus range of FPGA NID in valid_targets field
  //
  GlobBusIdRule.Data = MmcfgRule;
  if ((ValidSoc == TRUE) && (SocType == 1)) {
    GlobBusIdRule.Bits.valid_targets &= (~(1 << TargetOffset));
  }
  *UpdatedBusIdRule = GlobBusIdRule.Data;
  return EFI_SUCCESS;
}

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
  )
{
  ITCCTRL45_N0_IIO_VTD_STRUCT  ItcCtrl45n0;
  ITCCTRL45_N1_IIO_VTD_STRUCT  ItcCtrl45n1;

  //
  // Isoc/MeSeg enabling in IIO, OK to program on every boot regardless of Isoc/MeSeg enabled
  // To be programmed only on Cstacks before warm reset
  //
  ItcCtrl45n0.Data = UsraCsrRead (SocId, CtrId, ITCCTRL45_N0_IIO_VTD_REG);
  ItcCtrl45n1.Data = UsraCsrRead (SocId, CtrId, ITCCTRL45_N1_IIO_VTD_REG);
  ItcCtrl45n0.Bits.dfx_max_cache = 0x2e;
  ItcCtrl45n1.Bits.max_isoch = 0x2;
  //
  // 4928588: Cloned From SKX Si Bug Eco: Isoch trafiic does not get priority over Non-isoch traffic for credit returns
  // Note: permanent workaround for SKX, but this workaround won't be needed for ICX
  // This bitfield only makes a difference if there is Isoc traffic, otherwise benign:
  //
  ItcCtrl45n0.Bits.isoch_absolute_priority = 0x1;
  UsraCsrWrite (SocId, CtrId, ITCCTRL45_N0_IIO_VTD_REG, ItcCtrl45n0.Data);
  UsraCsrWrite (SocId, CtrId, ITCCTRL45_N1_IIO_VTD_REG, ItcCtrl45n1.Data);

  return EFI_SUCCESS;
}

/**
  IIO:  Create a new rule

  @param SocId         - Socket ID
  @param RuleEn        - Rule enable value
  @param Serial        - Serialize rule value
  @param MmiolBase     - MMIOL Base
  @param MmiolLimit    - MMIOL Limit
  @param RemoteStackId - Remote Stack ID
  @param NewMmiolRule  - New MMIOL Limit

  @retval              - EFI_UNSUPPORTED: the function not implemented
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
  )
{
  MMIOL_RULE0_IIO_VTD_STRUCT Rule;

  Rule.Data = 0;
  Rule.Bits.ruleenable    = RuleEn & 0x1; // Enable the rule
  Rule.Bits.baseaddress   = MmiolBase >> 22;
  Rule.Bits.limitaddress  = MmiolLimit >> 22;
  Rule.Bits.serializerule = Serial & 0x3;
  Rule.Bits.remotestackid = RemoteStackId;
  *NewMmiolRule = Rule.Data;

  return EFI_SUCCESS;
}

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
  )
{
  MMIOL_RULE0_IIO_VTD_STRUCT Rule;

  Rule.Data = CurrentRule;
  Rule.Bits.ruleenable   = 1; // Enable the rule
  Rule.Bits.baseaddress  = MmiolBase >> 22;
  Rule.Bits.limitaddress = MmiolLimit >> 22;

  if (AboveMmiol == TRUE) {
    //
    // Handle nonval case where OEM attempts to put mmiol below mmcfg.
    // To cover any gap, lower Rule11 to start at end of mmcfg for NEM/Ego coverage and point locally
    //
    Rule.Bits.baseaddress = NemRuleBase >> 22;
  }
  *NewMmiolNemRule = Rule.Data;
  return EFI_SUCCESS;
}

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
  )
{
  SAD_TARGET_IIO_VTD_STRUCT SadTargetIio;

  SadTargetIio.Data = UsraCsrRead (SocId, CtrId, SAD_TARGET_IIO_VTD_REG);

  if (SocId == SbspSoc) {
    SadTargetIio.Bits.legacy_pch_target = LegacyPchTgt;
  } else {
    SadTargetIio.Bits.legacy_pch_target = SbspSoc;
  }

  SadTargetIio.Bits.pch_stack = PchStack;

  if (SocId == OutLegacyVgaSoc) {
    SadTargetIio.Bits.vga_target = VgaTgt;
  } else {
    SadTargetIio.Bits.vga_target = OutLegacyVgaSoc;
  }

  SadTargetIio.Bits.vga_stack = OutLegacyVgaStack;

  UsraCsrWrite (SocId, CtrId, SAD_TARGET_IIO_VTD_REG, SadTargetIio.Data);

  return EFI_SUCCESS;
}

/**
  IIO:  Set SNC Base address using and index

  @param SocId      - Socket ID
  @param CtrId      - Controller ID
  @param ClusterIdx - The cluster index
  @param ClMemBase  - The cluster memory base address

  @return           - EFI_UNSUPPORTED: the function not implemented
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
  )
{
  SNC_BASE_1_IIO_VTD_STRUCT  SncBase;

  if (ClusterIdx >= IIO_TOTAL_CLUSTERS_REGS) {
    DEBUG ((DEBUG_ERROR, "ERROR!! out of bounds access detected\n"));
    ASSERT (ClusterIdx < IIO_TOTAL_CLUSTERS_REGS);
    return EFI_INVALID_PARAMETER;
  }

  SncBase.Data = UsraCsrRead (SocId, CtrId, IioSncBaseRegisterOffset[ClusterIdx]);
  SncBase.Bits.starting_addr_0 = ClMemBase & 0x0000FFFF;
  UsraCsrWrite (SocId, CtrId, IioSncBaseRegisterOffset[ClusterIdx], SncBase.Data);

  return EFI_SUCCESS;
}

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
  )
{
  if (ClusterIdx >= IIO_TOTAL_CLUSTERS_REGS) {
    DEBUG ((DEBUG_ERROR, "ERROR!! out of bounds access detected\n"));
    ASSERT (ClusterIdx < IIO_TOTAL_CLUSTERS_REGS);
    return EFI_INVALID_PARAMETER;
  }

  *SncIdx = UsraCsrRead (SocId, CtrId, IioSncBaseRegisterOffset[ClusterIdx]);

  return EFI_SUCCESS;
}

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
  )
{
  TCSKTSAD_IIO_VTD_STRUCT TcSktSad;

  //
  // This register gets locked by IIO_DFX_LCK_CTL.DBGBUSLCK by IioLateInitialize code later
  //
  TcSktSad.Data = UsraCsrRead (SocId, CtrId, TCSKTSAD_IIO_VTD_REG);

  //
  // mctpb_loc_skt is a vector of the enabled IIO stack IDs in the socket
  // excluding the stack that is being programmed. This indicates where
  // within the local socket MCTP broadcast messages should be directed.
  //
  TcSktSad.Bits.mctpb_loc_skt = StackPresentBitMap;

  //
  // mctpb_rem_skt is a vector of the remote node-ids in the system
  // excluding the nodeid of the socket being programmed. This indicates
  // which remote sockets MCTP broadcast messages should be programmed.
  //
  TcSktSad.Bits.mctpb_rem_skt = SocketsPresentBitmap;
  UsraCsrWrite (SocId, CtrId, TCSKTSAD_IIO_VTD_REG, TcSktSad.Data);

  return EFI_SUCCESS;
}

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
  )
{
  BUSID_GLOB_RULE_IIO_VTD_STRUCT BusIdGlobRule;
  ITCCTRL23_N1_IIO_VTD_STRUCT    ItcCtrl23;

  BusIdGlobRule.Data = MmcfgRule;

  ItcCtrl23.Data = UsraCsrRead (SocId, CtrId, ITCCTRL23_N1_IIO_VTD_REG);

  switch (BusIdGlobRule.Bits.length) {
    case 2:
      //
      // 010: MaxBusNumber = 512 (ie 512MB MMCFG range)
      // 2 segments of 256 buses, thus 1-bit-wide SegmentID
      //
      ItcCtrl23.Bits.mctp_segid_size = 1;
      break;
    case 1:
      //
      // 001: MaxBusNumber = 1024 (ie 1G MMCFG range)
      // 4 segments of 256 buses, thus 2-bit-wide SegmentID
      //
      ItcCtrl23.Bits.mctp_segid_size = 2;
      break;
    case 0:
      //
      // 000: MaxBusNumber = 2048 (ie 2G MMCFG range)
      // 8 segments of 256 buses, thus 3-bit-wide SegmentID
      //
      ItcCtrl23.Bits.mctp_segid_size = 3;
      break;
    default:
      //
      // 256 or less buses in the system, 1 segment
      // No SegmentID field required
      //
      ItcCtrl23.Bits.mctp_segid_size = 0;
      break;
  }

  //
  // Program the local socket segment ID
  //
  ItcCtrl23.Bits.cfg_my_seg = SegmentSocket;

  UsraCsrWrite (SocId, CtrId, ITCCTRL23_N1_IIO_VTD_REG, ItcCtrl23.Data);

  return EFI_SUCCESS;
}
