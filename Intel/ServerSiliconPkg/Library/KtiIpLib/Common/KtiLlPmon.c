/** @file
  KTI *_KTI_LLPMON_REG related registers access.

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
#include <Base.h>
#include "Include/KtiRegs.h"
#include <Library/UsraCsrLib.h>
#include <Guid/KtiIpInterface.h>
#include <Library/EmulationConfigurationLib.h>
#include <KtiSetupDefinitions.h>
#include <Library/KtiApi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoAccessLib.h>
#include <Library/DebugLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/UpimdfIpAccessLib.h>

STATIC CHAR8 KTI_IP_LIB_REGISTER_WRITE_MSG[] = "\n  Socket %u Port %u : %a write 0x%x\n";

/**
  GetPrefetchEnabled
  Called by UpdateKtiPrefetchStatusAsRegisterValue()

  @param SocketId              - Socket Id to program
  @param Port                  - Port to program

  @retval enableprefetch
**/
UINT8
EFIAPI
GetKtiPrefetch (
  IN UINT8                    SocketId,
  IN UINT8                    Port
  )
{
  KTIAGCTRL_KTI_LLPMON_STRUCT KtiAgCtrl;
  KtiAgCtrl.Data = UpimdfIpReadAccess32 (SocketId, Port, 0, KTIAGCTRL_KTI_LLPMON_REG);
  if (KtiAgCtrl.Bits.enableprefetch == 1) {
    return 1;
  } else {
    return 0;
  }
}

/**
  Program KTI Link Layer mesh credit control bitfields

  @param SocketId                   - Socket ID
  @param Port                       - Link number
  @param KtiLinkVnaOverride         - Option to override KTI LL VNA Credit allocation
  @param OutVn1En                   - boolean: is VN1 enabled on this system
  @param OutKtiFpgaEnable           - boolean: is FPGA enabled on this system
  @param KtiLLVnaCredit             - LL credit allocation, if KTI_LL_VNA_SETUP_MAX requested and Vn1=0
  @param KtiLLVnaCreditWithVn1En    - LL credit allocation, if KTI_LL_VNA_SETUP_MAX requested and Vn1=1

  @retval None

**/
VOID
EFIAPI
ProgramLinkCreditControl (
  UINT8                    SocketId,
  UINT8                    Port,
  UINT8                    KtiLinkVnaOverride,
  UINT8                    OutVn1En,
  UINT8                    OutKtiFpgaEnable,
  UINT8                    KtiLLVnaCredit,
  UINT8                    KtiLLVnaCreditWithVn1En
  )
{
  KTILCCL_KTI_LLPMON_STRUCT KtiLccl;
  //
  // Setup LinkCreditControl
  //
  KtiLccl.Data = UpimdfIpReadAccess32 (SocketId, Port, 0, KTILCCL_KTI_LLPMON_REG);
  switch (KtiLinkVnaOverride) {
    case KTI_LL_VNA_SETUP_MAX:
      //
      // Program normal values
      //
      if (OutVn1En == TRUE) {
        KtiLccl.Bits.vna_credits = KtiLLVnaCreditWithVn1En;
      } else {
        KtiLccl.Bits.vna_credits = KtiLLVnaCredit;
      }
      break;
    case KTI_LL_VNA_SETUP_MIN:
      //
      // Program architectural minimum
      //
      KtiLccl.Bits.vna_credits = KTI_LL_VNA_MIN;
      break;
    default:
      //
      // Check if user-selected value is valid, if so program it
      //
      if ((KtiLinkVnaOverride > KtiLLVnaCreditWithVn1En) &&
        (OutVn1En == TRUE)) {
        KtiLccl.Bits.vna_credits = KtiLLVnaCreditWithVn1En;
      } else if ((KtiLinkVnaOverride > KtiLLVnaCredit) &&
        (OutVn1En == FALSE)) {
        KtiLccl.Bits.vna_credits = KtiLLVnaCredit;
      } else {
        KtiLccl.Bits.vna_credits = KtiLinkVnaOverride;
      }
  }
  if (OutVn1En == TRUE) {
    KtiLccl.Bits.vn1_enable = 1;
  }

  DEBUG ((DEBUG_INFO, KTI_IP_LIB_REGISTER_WRITE_MSG, SocketId, Port, "KTILCCL", KtiLccl.Data));
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILCCL_KTI_LLPMON_REG, KtiLccl.Data);
}

/**
  Program the KTI Link Layer CRC enable bitfield

  @param SocketId              - Socket ID
  @param Port          - Link number
  @param OutKtiCrcMode      - KTI RC determined CRC mode
  @param DfxCrcMode         - Optional DFX Override CRC mode, typically KTI_AUTO

  @retval None

**/
VOID
EFIAPI
SetKtiLinkCrcMode (
  UINT8                    SocketId,
  UINT8                    Port,
  UINT8                    OutKtiCrcMode,
  UINT32                   DfxCrcMode
  )
{
  KTILCL_KTI_LLPMON_STRUCT     KtiLcl;
  //
  // Setup CRC mode based on the output variable and check the per-link DFX override.
  //
  KtiLcl.Data = UpimdfIpReadAccess32 (SocketId, Port, 0, KTILCL_KTI_LLPMON_REG);
  KtiLcl.Bits.next_crc_mode = OutKtiCrcMode;
  if (DfxCrcMode < CRC_MODE_AUTO) {
    KtiLcl.Bits.next_crc_mode = DfxCrcMode;
  }
  DEBUG ((DEBUG_INFO, KTI_IP_LIB_REGISTER_WRITE_MSG, SocketId, Port, "KTILCL", KtiLcl.Data));
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILCL_KTI_LLPMON_REG, KtiLcl.Data);
}

/**
  Routine to get link current_crc_mode value
  called by PrimeHostStructurePhyLinkLayer()

  @param SocketId          - Socket Id
  @param Port              - Port Id

  @retval current_crc_mode value
**/
UINT32
EFIAPI
GetCurrentCrcMode (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port
  )
{
  KTILS_KTI_LLPMON_STRUCT             KtiLs;
  KtiLs.Data = UpimdfIpReadAccess32 (SocketId, Port, 0, KTILS_KTI_LLPMON_REG);
  return KtiLs.Bits.current_crc_mode;
}

/**
  Trigger the KTI Link Layer reset by set the link_layer_reset

  @param SocketId              - Socket ID
  @param Port                  - Link number

  @retval None

**/
VOID
EFIAPI
TriggerKtiLinkLayerReset (
  UINT8                    SocketId,
  UINT8                    Port
  )
{
  KTILCL_KTI_LLPMON_STRUCT     KtiLcl;

  KtiLcl.Data = UpimdfIpReadAccess32 (SocketId, Port, 0, KTILCL_KTI_LLPMON_REG);
  KtiLcl.Bits.link_layer_reset = 1;
  DEBUG ((DEBUG_INFO, KTI_IP_LIB_REGISTER_WRITE_MSG, SocketId, Port, "KTILCL", KtiLcl.Data));
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILCL_KTI_LLPMON_REG, KtiLcl.Data);
}

/**
  Check the KTI Link Layer reset status by reading the link_layer_reset

  @param IN  SocketId      - Socket ID
  @param IN  Port          - Link number

  @retval   FALSE: LLR is not done
            TRUE: LLR is done

**/
BOOLEAN
EFIAPI
CheckKtiLinkLayerResetResult (
  UINT8                SocketId,
  UINT8                Port
  )
{
  KTILCL_KTI_LLPMON_STRUCT     KtiLcl;
  UINT8                        Count;
  UINT8                        *RegAddr;
  BOOLEAN                      PowerGood;

  //
  // Polling the bit0 to be 0
  //
  if (UbiosGenerationEnabled()) {
    RegAddr = (UINT8 *) UsraGetCsrRegisterAddress (SocketId, Port, KTILCL_KTI_LLPMON_REG);
    PowerGood = CheckCurrentReset (POST_RESET_POWERGOOD);
    EmulationPolling (RegAddr, BIT0, 0, PowerGood);
  }

  for (Count = 10; Count > 0; Count--) {
    KtiLcl.Data = UpimdfIpReadAccess32 (SocketId, Port, 0, KTILCL_KTI_LLPMON_REG);
    if (KtiLcl.Bits.link_layer_reset == 0) {
      return TRUE;
    }
    FixedDelayMicroSecond (100);
  }
  return FALSE;
}

/**
  Get the KTI Link Layer VN1 setting
  called from PrimeHostStructurePhyLinkLayer():

  @param SocketId      - Socket ID
  @param Port          - Link number

  @retval OutVn1En     - VN1 Supported by this Socket/ Link
**/
UINT32
EFIAPI
GetVn1Supported (
  IN  UINT8                SocketId,
  IN  UINT8                Port
  )
{
  KTILCP_KTI_LLPMON_STRUCT       KtiLcp;

  //Skip LCP VN1 checking for SPR A0/B0
  if (IsSiliconWorkaroundEnabled("S14011213678")) {
    KtiLcp.Bits.vn1_supported = 1;
  } else {
    KtiLcp.Data = UpimdfIpReadAccess32 (SocketId, Port, 0, KTILCP_KTI_LLPMON_REG);
  }

  return KtiLcp.Bits.vn1_supported;
}

/**
  Program IO TAD CSRs for each KTI Port

  @param SocketId               - Socket to program
  @param Port                   - Port to program
  @param IoLocalTgtLst          - target for each enabled stack
  @param IoLocalLimits[2]       - limit for each enabled stack

  @retval None
**/
VOID
EFIAPI
ProgramKtiIoTad (
  IN UINT8                 SocketId,
  IN UINT8                 Port,
  IN UINT32                IoLocalTgtLst,
  IN UINT32                *IoLocalLimits
  )
{
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_TAD_IO0_KTI_LLPMON_REG, IoLocalTgtLst);
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_TAD_IO1_KTI_LLPMON_REG, IoLocalLimits[0]);
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_TAD_IO2_KTI_LLPMON_REG, IoLocalLimits[1]);
}

/**
  Program IO APIC TAD CSRs for each KTI Port

  @param SocketId               - Socket to program
  @param Port                   - Port to program
  @param IoApicLocalTgtLst      - target for each enabled stack
  @param IoApicLocalLimits[2]   - limit for each enabled stack

  @retval None
**/
VOID
EFIAPI
ProgramKtiIoApicTad (
  IN UINT8                 SocketId,
  IN UINT8                 Port,
  IN UINT32                IoApicLocalTgtLst,
  IN UINT32                *IoApicLocalLimits
  )
{
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_TAD_IOAPIC0_KTI_LLPMON_REG, IoApicLocalTgtLst);
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_TAD_IOAPIC1_KTI_LLPMON_REG, IoApicLocalLimits[0]);
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_TAD_IOAPIC2_KTI_LLPMON_REG, IoApicLocalLimits[1]);
}

/**
  Program data into KTI SNC_CONFIG CSR

  @param  SocketId            -  Socket Id
  @param  Port                -  Port Id
  @param  SncConfigData       -  Value program to KTI SNC_CONFIG register

  @retval None

**/
VOID
EFIAPI
ProgramKtiSncConfig (
  IN UINT8                    SocketId,
  IN UINT8                    Port,
  IN UINT32                   SncConfigData
  )
{
  //
  // Program KTILK_SNC_CONFIG_KTI_LLPMON_REG
  //
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_SNC_CONFIG_KTI_LLPMON_REG, SncConfigData);
}

/**
  Routine to initialize LEP data structures
  called by ParseCpuLep()

  @param SocketId          - Socket Id
  @param Port              - Port Id
  @param *PeerSocId        - Peer Socket ID
  @param *PeerPort         - Peer Port ID
  @param *PeerSocType      - Peer Socket Type

  @retval None

**/
VOID
EFIAPI
ReadLep (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port,
  OUT UINT8                    *PeerSocId,
  OUT UINT8                    *PeerPort,
  OUT UINT8                    *PeerSocType
  )
{
  KTILP0_KTI_LLPMON_STRUCT            Lep0;
  //
  // Identify the Peer Port number
  //
  Lep0.Data   = UpimdfIpReadAccess32 (SocketId, Port, 0, KTILP0_KTI_LLPMON_REG);
  *PeerSocId   = (UINT8) Lep0.Bits.base_nodeid;
  *PeerPort    = (UINT8) Lep0.Bits.sending_port;
  *PeerSocType = (UINT8) Lep0.Bits.sku_type;
}

/**
  Routine to get link ll_status value
  called by ParseCpuLep()

  @param SocketId          - Socket Id
  @param Port              - Port Id

  @retval ll_status bitfield value from KTIPCSTS_KTI_LLPMON_REG
**/
UINT32
EFIAPI
GetLlStatus (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port
  )
{
  KTIPCSTS_KTI_LLPMON_STRUCT          PcSts;
  PcSts.Data = UpimdfIpReadAccess32 (SocketId, Port, 0, KTIPCSTS_KTI_LLPMON_REG);
#ifdef RAS_KTI_DEBUG
  DEBUG ((DEBUG_INFO, "\n   PcSts = 0x%08X", PcSts.Data));
#endif // RAS_KTI_DEBUG
  return PcSts.Bits.ll_status;
}

/**
  Routine to get link init_state value
  called by ParseCpuLep()

  @param SocketId          - Socket Id
  @param Port              - Port Id

  @retval init_state bitfield value from KTILS_KTI_LLPMON_REG
**/
UINT32
EFIAPI
GetInitState (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port
  )
{
  KTILS_KTI_LLPMON_STRUCT             KtiLs;
  KtiLs.Data = UpimdfIpReadAccess32 (SocketId, Port, 0, KTILS_KTI_LLPMON_REG);
#ifdef RAS_KTI_DEBUG
  DEBUG ((DEBUG_INFO, "\n   KtiLs = 0x%08X", KtiLs.Data));
#endif // RAS_KTI_DEBUG
  return KtiLs.Bits.init_state;
}

/**
  Routine to get link KTILS CSR value
  called by SlowModeWaEntry ()

  @param Soc               - Socket Id
  @param Port              - Port Id

  @retval KTILS.Data value
**/

UINT32
EFIAPI
GetKtiLs (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port
  )
{
  KTILS_KTI_LLPMON_STRUCT             KtiLs;
  KtiLs.Data = UpimdfIpReadAccess32 (SocketId, Port, 0, KTILS_KTI_LLPMON_REG);
  DEBUG ((DEBUG_INFO, "\n   KtiLs.Data = 0x%08X", KtiLs.Data));
  return KtiLs.Data;
}

/**
  Program the min path in KTIQRT CSR
  establishes the minimum route tree from SBSP to all APs
  called from SetupSbspPathToAllSockets()

  @param SbspSocketId,        - Socket Id of SBSP
  @param ParentSocketId,      - Socket Id of parent socket in this route
  @param DestinationSocketId, - Socket Id of child socket in this route
  @param ParentPort,          - Port Id of parent socket which is connected to child socket
  @param ChildPort            - Port Id of child socket which is connected to parent socket
  @param PortFieldWidth       - Width of port field

  @retval None
**/
VOID
EFIAPI
ProgramMinimumPath (
  IN UINT8 SbspSocketId,
  IN UINT8 ParentSocketId,
  IN UINT8 DestinationSocketId,
  IN UINT8 ParentPort,
  IN UINT8 ChildPort,
  IN UINT8 PortFieldWidth
  )
{
  UINT32 Data32;
  UINT32 FieldMask;

  //
  // Forward Path in KTI Agent
  //
  FieldMask = FLD_MASK (PortFieldWidth, (PortFieldWidth * DestinationSocketId));
  Data32 = UpimdfIpReadAccess32 (ParentSocketId, ParentPort, 0, KTIQRT_KTI_LLPMON_REG);
  Data32 &= ~FieldMask;
  Data32 |= (ChildPort << (DestinationSocketId * PortFieldWidth));
  UpimdfIpWriteAccess32 (ParentSocketId, ParentPort, UPIMDF_INSTANCE_BROADCAST, KTIQRT_KTI_LLPMON_REG, Data32);

  //
  // Backward Path in KTI Agent
  //
  FieldMask = FLD_MASK (PortFieldWidth, (PortFieldWidth * SbspSocketId));
  Data32 = UpimdfIpReadAccess32 (ParentSocketId, ChildPort, 0, KTIQRT_KTI_LLPMON_REG);
  Data32 &= ~FieldMask;
  Data32 |= (ParentPort << (SbspSocketId * PortFieldWidth));
  UpimdfIpWriteAccess32 (ParentSocketId, ChildPort, UPIMDF_INSTANCE_BROADCAST, KTIQRT_KTI_LLPMON_REG, Data32);
}

/**
  Routine to program VGA Sad rules in KTI agents

  @param SocketId   - Socket Id
  @param Port       - Port Id
  @param VgaEnable  - Enables VGA range to be match (applies to both MMIO space and Legacy_IO space ranges)
  @param VgaTarget  - 3-bit value to indicate IIO 0-5. Values 6,7 are RSVD encodings

  @retval None
**/
VOID
EFIAPI
ProgramKtiVgaSadTargets (
  UINT8                         SocketId,
  UINT8                         Port,
  UINT8                         VgaEnable,
  UINT8                         VgaTarget
  )
{
  KTILK_TAD_LEG_KTI_LLPMON_STRUCT  SadTargetKti;
  SadTargetKti.Data = UpimdfIpReadAccess32 (SocketId, Port, 0, KTILK_TAD_LEG_KTI_LLPMON_REG);
  SadTargetKti.Bits.vga_enable = VgaEnable;
  SadTargetKti.Bits.vga_target = VgaTarget;
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_TAD_LEG_KTI_LLPMON_REG, SadTargetKti.Data);
}

/**
Get KTI Link Frequency

@param SocketId              - Socket Id to program
@param Port                  - Port to program

@retval KTI Frequency value from KtiRate bitfield
**/
UINT8
EFIAPI
GetKtiRate (
  IN UINT8                    SocketId,
  IN UINT8                    Port
  )
{
  KTIMISCSTAT_KTI_LLDFX_STRUCT KtiMiscSts;
  KtiMiscSts.Data = UpimdfIpReadAccess32 (SocketId, Port, 0, KTIMISCSTAT_KTI_LLDFX_REG);
  return (UINT8) KtiMiscSts.Bits.kti_rate;
}