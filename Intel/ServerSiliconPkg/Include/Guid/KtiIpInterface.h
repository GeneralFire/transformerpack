/** @file
  KTI generation specific silicon access interface

  This library abstracts silicon register differences between different versions
  of KTI IP blocks.
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

#ifndef _KTI_IP_INTERFACE_H_
#define _KTI_IP_INTERFACE_H_

#include <Uefi.h>
#include <Base.h>
#include <Upi/KtiHost.h>

/**
  Check to see if IAR write-enable bit is clear
  The write_en bit must be clear before reading the KTI IAR register

  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxInst   - Box Instance, 0 based

  @retval register value read
**/
BOOLEAN
EFIAPI
IsIarWriteEnableClear (
  IN UINT8                 SocId,
  IN UINT8                 BoxInst
  );

//
// Function pointer definiton for IsIarWriteEnableClear ()
//
typedef
BOOLEAN
(EFIAPI *IS_IAR_WRITE_ENABLE_CLEAR) (
  IN UINT8                 SocId,
  IN UINT8                 BoxInst
  );


/**
  Get IAR Data

  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxInst   - Box Instance, 0 based
  @param UnitId    - Unit ID (lane 0-19=0-19, fwdc=20, reserved=21-62, PQD=62, Broadcast Data Lane=63)
             Broadcast Data Lane UnitId should NOT be used for reads.
  @param RegOffset - Register offset; values come from the auto generated header file

  @retval register value read
**/
UINT8
EFIAPI
GetIarData (
  IN UINT8                 SocId,
  IN UINT8                 BoxInst,
  IN UINT8                 UnitId,
  IN UINT32                RegOffset
  );

//
// Function pointer definiton for GetIarData ()
//
typedef
UINT8
(EFIAPI *GET_IAR_DATA) (
  IN UINT8                 SocId,
  IN UINT8                 BoxInst,
  IN UINT8                 UnitId,
  IN UINT32                RegOffset
  );

/**
  Write to IAR register.

  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxInst   - Box Instance, 0 based
  @param UnitId    - Unit ID (lane 0-19=0-19, fwdc=20, reserved=21-62, PQD=62, Broadcast Data Lane=63)
  @param RegOffset - Register offset; values come from the auto generated header file
  @param Data      - Data to write

  @retval None
**/
VOID
EFIAPI
WriteIarData (
  IN UINT8                 SocId,
  IN UINT8                 BoxInst,
  IN UINT8                 UnitId,
  IN UINT32                RegOffset,
  IN UINT8                 Data
  );

//
// Function pointer definiton for WriteIarData ()
//
typedef
VOID
(EFIAPI *WRITE_IAR_DATA) (
  IN UINT8                 SocId,
  IN UINT8                 BoxInst,
  IN UINT8                 UnitId,
  IN UINT32                RegOffset,
  IN UINT8                 Data
  );


/**
Write safe value into the Rx PI Duty cycle corrector CSR

@param Socket    - CPU Socket Node number (Socket ID)
@param Port      - KTI Port number

@retval None
**/
VOID
EFIAPI
OverrideRxDccExtVal (
  IN UINT8 Socket,
  IN UINT8 Port
  );

//
// Function pointer definiton for OverrideRxDccExtVal ()
//
typedef
VOID
(EFIAPI *OVERRIDE_RX_DCC_EXT_VAL) (
  IN UINT8 Socket,
  IN UINT8 Port
  );


/**
Apply the RxDCC Clock override for this Socket/Port

@param Socket    - CPU Socket Node number (Socket ID)
@param Port      - KTI Port number

@retval None
**/
VOID
EFIAPI
ApplyRxDccClockOverride (
  IN UINT8 Socket,
  IN UINT8 Port
  );

//
// Function pointer definiton for ApplyRxDccClockOverride ()
//
typedef
VOID
(EFIAPI *APPLY_RX_DCC_CLOCK_OVERRIDE) (
  IN UINT8 Socket,
  IN UINT8 Port
  );


/**
  Function definition from ClearOrSetKtiDfxLckForFullSpeedTransition ()

  Loop for all Sockets and all KTI ports: set or clear these bits:
  DfxLckCtlCfg.Bits.reutenglck = Value;
  DfxLckCtlCfg.Bits.privdbg_phy_l0_sync_lck = Value;

  @param Socket    - CPU Socket Node number (Socket ID)
  @param Port      - KTI Port number
  @param Value     - 0 or 1 to clear or set the lck bits (respectively)

  @retval None
**/
VOID
EFIAPI
SetKtiDfxLck (
  IN UINT8                 Socket,
  IN UINT8                 Port,
  IN UINT16                Value
  );

//
// Function pointer definiton for SetKtiDfxLck ()
//
typedef
VOID
(EFIAPI *SET_KTI_DFX_LCK) (
  IN UINT8                 Socket,
  IN UINT8                 Port,
  IN UINT16                Value
  );

/**
  Check to see if LBC lbc_start bit is clear
  The lbc_start bit must be clear before reading/writing the KTI LBC register

  @param Socket    - CPU Socket Node number (Socket ID)
  @param Port      - KTI Port number on socket
  @param LbcType   - LBC_AFE or LBC_COM - Note: unused in 10nm

  @retval TRUE  - lbc_start bit is clear
  @retval FALSE - lbc_start bit is set
**/
BOOLEAN
EFIAPI
IsLbcStartClear (
  IN UINT8                 Socket,
  IN UINT8                 Port,
  IN UINT8                 LbcType
);

//
// Function pointer definiton for IsLbcStartClear ()
//
typedef
BOOLEAN
(EFIAPI *IS_LBC_START_CLEAR) (
  IN UINT8                 Socket,
  IN UINT8                 Port,
  IN UINT8                 LbcType
  );


/**
  Write to LBC register.

  Note, depending on the value of the parameter LbcType, this function
  will write into LBC_PER_IOU_CONTROL (AFE type) or into KTIMISC_LBC_CONTROL
  (COM type). This function assumes that the bitfield structure of both
  registers is the same and just varies the write destination based on the
  LbcType.

  @param Socket    - CPU Socket Node number (Socket ID)
  @param Port      - KTI Port Index, 0 based
  @param LbcType   - LBC_AFE or LBC_COM - Note: unused in 10nm
  @param LaneMask  - 20-bit wide lane mask
  @param LoadSel   - 7-bit wide load select (address within LBC)
  @param Data      - 14-bit data value to write

**/
VOID
EFIAPI
WriteLbcData (
  IN UINT8                 Socket,
  IN UINT8                 Port,
  IN UINT8                 LbcType,
  IN UINT32                LaneMask,
  IN UINT32                LoadSel,
  IN UINT32                Data
  );

//
// Function pointer definiton for WriteLbcData ()
//
typedef
VOID
(EFIAPI *WRITE_LBC_DATA) (
  IN UINT8                 Socket,
  IN UINT8                 Port,
  IN UINT8                 LbcType,
  IN UINT32                LaneMask,
  IN UINT32                LoadSel,
  IN UINT32                Data
  );


//
// KTI_LLPMON_REG
//

/**
  Returns the total Vendor ID of this KTI LLPMON.
  Called from GetKtiAgentCount() and several other places

  @param SocId             - Soc ID With KTI LLPMON
  @param Port              - Port ID of the KTI Agent

  @retval 8086 - Intel KTI LLPMON
  @retval FFFF - No KTI LLPMON Present

**/
UINT16
EFIAPI
GetKtiLlpmonVendorId (
  IN UINT8                      SocId,
  IN UINT8                      Port
  );

//
// Function pointer definiton for GetKtiLlpmonVendorId ()
//
typedef
UINT16
(EFIAPI *GET_KTI_LLPMON_VENDOR_ID) (
  IN UINT8                      SocId,
  IN UINT8                      Port
  );


/**
  Routine to collect and clear KTI Agent errors

  @param SocketId               - Socket to program
  @param Port                   - Port to program
  @param LinkValid             - Is there a valid link on this KTI Port - True/False
  @param OutKtiFpgaPresent      - Is FPGA Present for this socket - True/False
  @param OutKtiFpgaEnable       - Is FPGA enabled by KTIRC - True/False
  @param SocketFpgasInitialized - Is FPGA Initialized - True/False

  @retval None

**/
VOID
EFIAPI
CollectAndClearKtiPortErrors (
  IN UINT8                    SocketId,
  IN UINT8                    Port,
  IN BOOLEAN                  LinkValid,
  IN BOOLEAN                  OutKtiFpgaPresent,
  IN BOOLEAN                  OutKtiFpgaEnable,
  IN BOOLEAN                  SocketFpgasInitialized
  );

//
// Function pointer definiton for CollectAndClearKtiPortErrors ()
//
typedef
VOID
(EFIAPI *COLLECT_AND_CLEAR_KTI_PORT_ERRORS) (
  IN UINT8                    SocketId,
  IN UINT8                    Port,
  IN BOOLEAN                  LinkValid,
  IN BOOLEAN                  OutKtiFpgaPresent,
  IN BOOLEAN                  OutKtiFpgaEnable,
  IN BOOLEAN                  SocketFpgasInitialized
  );


/**
  Routine to program D2C register

  @param SocId             - Socket to program
  @param Port              - Port to program
  @param D2cEnable         - enable or disable D2C for this KTI Port

  @retval None

**/
VOID
EFIAPI
ProgramD2CRegister (
  IN UINT8                    SocId,
  IN UINT8                    Port,
  IN BOOLEAN                  D2cEnable
  );

//
// Function pointer definiton for ProgramD2CRegister ()
//
typedef
VOID
(EFIAPI *PROGRAM_D2C_REGISTER) (
  IN UINT8                    SocId,
  IN UINT8                    Port,
  IN BOOLEAN                  D2cEnable
  );


/**
  Routine to program program KTI Agent prefetch enable CSR
  Called by SetupUpiMiniSad()

  @param SocId                 - Socket Id to program
  @param Port                  - Port to program
  @param RdCurForXptPrefetchEn - Setup option RdCur for XPT Prefetch
  @param KtiOrRemoteXptPrefetchEnable:
           0: KtiXptPrefetch Enabled
           1: RemoteXptPrefetch Enabled
           2: KtiXptPrefetch Disabled
           3: RemoteXptPrefetch Disabled

  @retval None
**/
VOID
EFIAPI
ProgramKtiAgentPrefetch (
  IN UINT8                    SocId,
  IN UINT8                    Port,
  IN UINT8                    RdCurForXptPrefetchEn,
  IN UINT8                    KtiOrRemoteXptPrefetchEnable
  );

//
// Function pointer definiton for ProgramKtiAgentPrefetch ()
//
typedef
VOID
(EFIAPI *PROGRAM_KTI_AGENT_PREFETCH) (
  IN UINT8                    SocId,
  IN UINT8                    Port,
  IN UINT8                    RdCurForXptPrefetchEn,
  IN UINT8                    KtiOrRemoteXptPrefetchEnable
  );


/**
  GetPrefetchEnabled
  Called by UpdateKtiPrefetchStatusAsRegisterValue()

  @param SocId                 - Socket Id to program
  @param Port                  - Port to program

  @retval enableprefetch
**/
UINT8
EFIAPI
GetKtiPrefetch (
  IN UINT8                    SocId,
  IN UINT8                    Port
  );

//
// Function pointer definiton for GetKtiPrefetch ()
//
typedef
UINT8
(EFIAPI *GET_KTI_PREFETCH) (
  IN UINT8                    SocId,
  IN UINT8                    Port
  );


/**
  Program KTI Link Layer mesh credit control bitfields

  @param SocketId                   - Socket ID
  @param Port                       - KTI Port number
  @param KtiLinkVnaOverride         - Optional user-selected value to override KTI LL VNA Credit allocation
  @param OutVn1En                   - indicates whether VN1 is enabled (e.g. 8s topology)
  @param OutKtiFpgaEnable           - indicates whether there is an FPGA enabled in the system topology
  @param KtiLLVnaCredit             - LL credit allocation, if KTI_LL_VNA_SETUP_MAX requested and Vn1=0
  @param KtiLLVnaCreditWithVn1En    - LL credit allocation, if KTI_LL_VNA_SETUP_MAX requested and Vn1=1


  @retval None
**/
VOID
EFIAPI
ProgramLinkCreditControl (
  IN UINT8                    SocketId,
  IN UINT8                    Port,
  IN UINT8                    KtiLinkVnaOverride,
  IN UINT8                    OutVn1En,
  IN UINT8                    OutKtiFpgaEnable,
  IN UINT8                    KtiLLVnaCredit,
  IN UINT8                    KtiLLVnaCreditWithVn1En
  );

//
// Function pointer definiton for ProgramLinkCreditControl ()
//
typedef
VOID
(EFIAPI *PROGRAM_LINK_CREDIT_CONTROL) (
  IN UINT8                    SocketId,
  IN UINT8                    Port,
  IN UINT8                    KtiLinkVnaOverride,
  IN UINT8                    OutVn1En,
  IN UINT8                    OutKtiFpgaEnable
  );


/**
  Program the KTI Link Layer CRC enable bitfield

  @param Index              - Socket ID
  @param LinkIndex          - Link number
  @param OutKtiCrcMode      - KTI RC determined CRC mode
  @param DfxCrcMode         - Optional DFX Override CRC mode, typically KTI_AUTO

  @retval None

**/
VOID
EFIAPI
SetKtiLinkCrcMode (
  IN UINT8                    Index,
  IN UINT8                    LinkIndex,
  IN UINT8                    OutKtiCrcMode,
  IN UINT32                   DfxCrcMode
  );

//
// Function pointer definiton for SetKtiLinkCrcMode ()
//
typedef
VOID
(EFIAPI *SET_KTI_LINK_CRC_MODE) (
  IN UINT8                    Index,
  IN UINT8                    LinkIndex,
  IN UINT8                    OutKtiCrcMode,
  IN UINT32                   DfxCrcMode
  );


/**
  Get the KTI Link Layer CRC setting
  called from PrimeHostStructurePhyLinkLayer():

  @param IN  Index              - Socket ID
  @param IN  LinkIndex          - Link number

  @retval OutKtiCrcMode         - KTI RC determined CRC mode (CRC_MODE_16BIT, or CRC_MODE_ROLLING_32BIT)

**/
UINT32
EFIAPI
GetCrcMode (
  IN UINT8                Index,
  IN UINT8                LinkIndex
  );

//
// Function pointer definiton for GetCrcMode ()
//
typedef
UINT32
(EFIAPI *GET_CRC_MODE) (
  IN UINT8                Index,
  IN UINT8                LinkIndex
  );

/**
  Trigger the KTI Link Layer reset by set the link_layer_reset

  @param IN  Index              - Socket ID
  @param IN  LinkIndex          - Link number

  @retval None

**/
VOID
EFIAPI
TriggerKtiLinkLayerReset (
  IN UINT8                    Index,
  IN UINT8                    LinkIndex
  );

//
// Function pointer definiton for TriggerKtiLinkLayerReset ()
//
typedef
VOID
(EFIAPI *TRIGGER_KTI_LINK_LAYER_RESET) (
  IN UINT8                    Index,
  IN UINT8                    LinkIndex
  );


/**
  Check the KTI Link Layer reset status by reading the link_layer_reset

  @param IN  Index              - Socket ID
  @param IN  LinkIndex          - Link number

  @retval   1: LLR is not done
            0: LLR is done

**/
BOOLEAN
EFIAPI
CheckKtiLinkLayerResetResult (
  IN UINT8                Index,
  IN UINT8                LinkIndex
  );

//
// Function pointer definiton for CheckKtiLinkLayerResetResult ()
//
typedef
BOOLEAN
(EFIAPI *CHECK_KTI_LINK_LAYER_RESET_RESULT) (
  IN UINT8                Index,
  IN UINT8                LinkIndex
  );


/**
  Get the KTI Link Layer VN1 setting
  called from PrimeHostStructurePhyLinkLayer():

  @param Index              - Socket ID
  @param LinkIndex          - Link number

  @retval OutVn1En          - VN1 Supported by this Socket/ Link

**/
UINT32
EFIAPI
GetVn1Supported (
  IN  UINT8                Index,
  IN  UINT8                LinkIndex
  );

//
// Function pointer definiton for GetVn1Supported ()
//
typedef
UINT32
(EFIAPI *GET_VN1_SUPPORTED) (
  IN  UINT8                Index,
  IN  UINT8                LinkIndex
  );


/**
  Program Bus TAD CSRs for each KTI Port

  @param SocId                  - Socket to program
  @param Port                   - Port to program
  @param *BusLocalTgtLst        - target for each enabled stack
  @param *BusLocalLimits        - Array of limits for each enabled stack

  @retval None
**/
VOID
EFIAPI
ProgramKtiBusTad (
  IN UINT8                 SocId,
  IN UINT8                 Port,
  IN UINT32                *BusLocalTgtLst,
  IN UINT32                *BusLocalLimits
  );

//
// Function pointer definiton for ProgramKtiBusTad ()
//
typedef
VOID
(EFIAPI *PROGRAM_KTI_BUS_TAD) (
  IN UINT8                 SocId,
  IN UINT8                 Port,
  IN UINT32                *BusLocalTgtLst,
  IN UINT32                *BusLocalLimits
  );


/**
  Program IO TAD CSRs for each KTI Port

  @param SocId                  - Socket to program
  @param Port                   - Port to program
  @param IoLocalTgtLst          - target for each enabled stack
  @param *IoLocalLimits         - limit for each enabled stack

  @retval None
**/
VOID
EFIAPI
ProgramKtiIoTad (
  IN UINT8                 SocId,
  IN UINT8                 Port,
  IN UINT32                IoLocalTgtLst,
  IN UINT32                *IoLocalLimits
  );

//
// Function pointer definiton for ProgramKtiIoTad ()
//
typedef
VOID
(EFIAPI *PROGRAM_KTI_IO_TAD) (
  IN UINT8                 SocId,
  IN UINT8                 Port,
  IN UINT32                IoLocalTgtLst,
  IN UINT32                *IoLocalLimits
  );


/**
  Program IO APIC TAD CSRs for each KTI Port

  @param SocId                  - Socket to program
  @param Port                   - Port to program
  @param IoApicLocalTgtLst      - target for each enabled stack
  @param *IoApicLocalLimits    - limit for each enabled stack

  @retval None
**/
VOID
EFIAPI
ProgramKtiIoApicTad (
  IN UINT8                 SocId,
  IN UINT8                 Port,
  IN UINT32                IoApicLocalTgtLst,
  IN UINT32                *IoApicLocalLimits
  );

//
// Function pointer definiton for ProgramKtiIoApicTad ()
//
typedef
VOID
(EFIAPI *PROGRAM_KTI_IO_APIC_TAD) (
  IN UINT8                 SocId,
  IN UINT8                 Port,
  IN UINT32                IoApicLocalTgtLst,
  IN UINT32                *IoApicLocalLimits
  );


/**
  Program MMIOL TAD CSRs for each KTI Port

  @param SocId                  - Socket to program
  @param Port                   - Port to program
  @param CpuResourceMap         - Pointer to the CPU Resource Map for this Socket

  @retval None
**/
VOID
EFIAPI
ProgramKtiMmiolTad (
  IN UINT8                 SocketId,
  IN UINT8                 Port,
  IN KTI_CPU_RESOURCE      *CpuResourceMap
  );

//
// Function pointer definiton for ProgramKtiMmiolTad ()
//
typedef
VOID
(EFIAPI *PROGRAM_KTI_MMIOL_TAD) (
  IN UINT8                 SocketId,
  IN UINT8                 Port,
  IN KTI_CPU_RESOURCE      *CpuResourceMap
  );


/**
  Program MMIOH TAD CSRs for each KTI Port

  @param SocId                  - Socket to program
  @param Port                   - Port to program
  @param CpuResourceMap         - Pointer to the CPU Resource Map for this Socket

  @retval None
**/
VOID
EFIAPI
ProgramKtiMmiohTad (
  IN UINT8                 SocketId,
  IN UINT8                 Port,
  IN KTI_CPU_RESOURCE      *CpuResourceMap
  );

//
// Function pointer definiton for ProgramKtiMmiohTad ()
//
typedef
VOID
(EFIAPI *PROGRAM_KTI_MMIOH_TAD) (
  IN UINT8                 SocketId,
  IN UINT8                 Port,
  IN KTI_CPU_RESOURCE      *CpuResourceMap
  );


/**
  Program UMA Cluster register in Kti Agents
  Called from ProgramUmaClusterConfig(), loop for each socket and each port

  @param  Socket               -  Socket Id
  @param  Port                 -  Port Id
  @param  UmaClusterEnable     -  Enable bit for UMA based clustering mode
  @param  DefeatureXor         -  Defeature Xor

  @retval None

**/
VOID
EFIAPI
ProgramKtiAgentUmaCluster (
  IN UINT8                    Socket,
  IN UINT8                    Port,
  IN UINT8                    UmaClusterEnable,
  IN UINT8                    DefeatureXor
  );

//
// Function pointer definiton for ProgramKtiAgentUmaCluster (
//
typedef
VOID
(EFIAPI *PROGRAM_KTI_AGENT_UMA_CLUSTER) (
  IN UINT8                    Socket,
  IN UINT8                    Port,
  IN UINT8                    UmaClusterEnable,
  IN UINT8                    DefeatureXor
  );


/**
  Program Number of CHA per Cluster in UPI Uncore_Snc_Config
  This routine is to program UMA Cluster register in Kti Agents
  Called from ProgramNumOfChaPerCluster() in a loop for all Sockets and Ports

  @param  SocketId            -  Socket Id
  @param  Port                -  Port Id
  @param  SncConfigData       -  Value program to KTI UNCORE_SNC_CONFIG register

  @retval None

**/
VOID
EFIAPI
KtiUncoreChaPerCluster (
  IN UINT8                    Socket,
  IN UINT8                    Port,
  IN UINT32                   SncConfigData
  );

//
// Function pointer definiton for KtiUncoreChaPerCluster ()
//
typedef
VOID
(EFIAPI *KTI_UNCORE_CHA_PER_CLUSTER) (
  IN UINT8                    Socket,
  IN UINT8                    Port,
  IN UINT32                   SncConfigData
  );


/**
  Program data into KTI SNC_CONFIG CSR
  This data is already programmed in MS2IDI SNC_CONFIG CSR

  @param  SocketId            -  Socket Id
  @param  Port                -  Port Id
  @param  SncConfigMs2IdiData -  Value program to KTI SNC_CONFIG register

  @retval None

**/
VOID
EFIAPI
ProgramKtiSncConfig (
  IN UINT8                    SocketId,
  IN UINT8                    Port,
  IN UINT32                   SncConfigMs2IdiData
  );

//
// Function pointer definiton for ProgramKtiSncConfig ()
//
typedef
VOID
(EFIAPI *PROGRAM_KTI_SNC_CONFIG) (
  IN UINT8                    SocketId,
  IN UINT8                    Port,
  IN UINT32                   SncConfigMs2IdiData
  );


/**
  Program SNC_Base_1 and SNC_BASE_2, for 2LM case
  Called from Program2LMHashRegister()

  @param  Socket               -  Socket Id
  @param  Port                 -  Port Id
  @param  TwoLM_Address_Mask   -  Pointer to KTI_AGENT_2LM_ADDRESS_MASK

  @retval None
**/
VOID
EFIAPI
ProgramKtiAgent2LMAddrMask (
  IN UINT8                    Socket,
  IN UINT8                    Port,
  IN UINT32                   *TwoLM_Address_Mask
  );

//
// Function pointer definiton for ProgramKtiAgent2LMAddrMask ()
//
typedef
VOID
(EFIAPI *PROGRAM_KTI_AGENT_2LM_ADDR_MASK) (
  IN UINT8                    Socket,
  IN UINT8                    Port,
  IN UINT32                   *TwoLM_Address_Mask
  );


/**
  Program all SNC_BASE for all clusters, called on warm-reset-path only
  Called from ConfigureSncMode()

  @param Soc               - Socket ID
  @param Port              - Port ID
  @param Cluster           - Cluster Index
  @param ClusterSncBase    - Cluster SNC Base

**/
VOID
EFIAPI
KtiAgentSetSncBaseAddr (
  IN UINT8  Soc,
  IN UINT8  Port,
  IN UINT8  Cluster,
  IN UINT32 ClusterSncBase
  );

//
// Function pointer definiton for KtiAgentSetSncBaseAddr()
//
typedef
VOID
(EFIAPI *KTI_AGENT_SET_SNC_BASE_ADDR) (
  IN UINT8  Soc,
  IN UINT8  Port,
  IN UINT8  Cluster,
  IN UINT32 ClusterSncBase
  );


/**
  Routine to program SNC Upper Base in KTI Agent
  called by ProgramSncUpperBaseFor52BitExtend() on cold and warm reset flows

  @param Soc               - Socket Id
  @param Port              - Port Id
  @param SncUpperBase      - SNC Upper Base value

**/
VOID
EFIAPI
KtiAgentSetSncUpperBase (
  IN UINT8                    Soc,
  IN UINT8                    Port,
  IN UINT32                   SncUpperBase
  );

//
// Function pointer definiton for KtiAgentSetSncUpperBase ()
//
typedef
VOID
(EFIAPI *KTI_AGENT_SET_SNC_UPPER_BASE) (
  IN UINT8                    Soc,
  IN UINT8                    Port,
  IN UINT32                   SncUpperBase
  );


/**
  Routine to initialize LEP data structures
  called by ParseCpuLep()

  @param Soc               - Socket Id
  @param Port              - Port Id
  @param *PeerSocId        - Peer Socket ID
  @param *PeerPort         - Peer Port ID
  @param *PeerSocType      - Peer Socket Type

  @retval None

**/
VOID
EFIAPI
ReadLep (
  IN  UINT8                    Soc,
  IN  UINT8                    Port,
  OUT UINT8                    *PeerSocId,
  OUT UINT8                    *PeerPort,
  OUT UINT8                    *PeerSocType
  );

//
// Function pointer definiton for ReadLep ()
//
typedef
VOID
(EFIAPI *READ_LEP) (
  IN  UINT8                    Soc,
  IN  UINT8                    Port,
  OUT UINT8                    *PeerSocId,
  OUT UINT8                    *PeerPort,
  OUT UINT8                    *PeerSocType
  );


/**
  Routine to get link ll_status value
  called by ParseCpuLep()

  @param Soc               - Socket Id
  @param Port              - Port Id

  @retval ll_status value
**/
UINT32
EFIAPI
GetLlStatus (
  IN  UINT8                    Soc,
  IN  UINT8                    Port
  );

//
// Function pointer definiton for GetLlStatus ()
//
typedef
UINT32
(EFIAPI *GET_LL_STATUS) (
  IN  UINT8                    Soc,
  IN  UINT8                    Port
  );


/**
  Routine to get link init_state value
  called by ParseCpuLep()

  @param Soc               - Socket Id
  @param Port              - Port Id

  @retval init_state value
**/
UINT32
EFIAPI
GetInitState (
  IN  UINT8                    Soc,
  IN  UINT8                    Port
  );

//
// Function pointer definiton for GetInitState ()
//
typedef
UINT32
(EFIAPI *GET_INIT_STATE) (
  IN  UINT8                    Soc,
  IN  UINT8                    Port
  );


/**
  Routine to get link KTILS CSR value
  called by SlowModeWaEntry ()

  @param SocketId          - Socket Id
  @param Port              - Port Id

  @retval KTILS.Data value
**/
UINT32
EFIAPI
GetKtiLs (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port
);

//
// Function pointer definiton for GetKtiLs ()
//
typedef
UINT32
(EFIAPI *GET_KTILS) (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port
  );

/**
  Routine to get link current_crc_mode value
  called by PrimeHostStructurePhyLinkLayer()

  @param Soc               - Socket Id
  @param Port              - Port Id

  @retval current_crc_mode value
**/
UINT32
EFIAPI
GetCurrentCrcMode (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port
  );

//
// Function pointer definiton for GetCurrentCrcMode ()
//
typedef
UINT32
(EFIAPI *GET_CURRENT_CRC_MODE) (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port
  );


/**
  Program the min path in KTIQRT CSR
  establishes the minimum route tree from SBSP to all APs
  called from SetupSbspPathToAllSockets()

  @param SbspSocketId,
  @param ParentSocketId,
  @param DestinationSocketId,
  @param ParentPort,
  @param ChildPort,
  @param PortFieldWidth

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
  );

//
// Function pointer definiton for ProgramMinimumPath ()
//
typedef
VOID
(EFIAPI *PROGRAM_MINIMUM_PATH) (
  IN UINT8 SbspSocketId,
  IN UINT8 ParentSocketId,
  IN UINT8 DestinationSocketId,
  IN UINT8 ParentPort,
  IN UINT8 ChildPort,
  IN UINT8 PortFieldWidth
  );

/**
  Program KTIQRT CSR route
  Called from ProgramM3KtiAndKtiAgentRouteTable()

  @param SocketId,
  @param Port,
  @param KtiAgentRt
  @param ProgramPeMode

  @retval None
**/
VOID
EFIAPI
ProgramKtiAgentRouteTable (
  IN UINT8  SocketId,
  IN UINT8  Port,
  IN UINT32 KtiAgentRt,
  IN BOOLEAN  ProgramPeMode
  );

//
// Function pointer definiton for ProgramKtiAgentRouteTable ()
//
typedef
VOID
(EFIAPI *PROGRAM_KTI_AGENT_ROUTE_TABLE) (
  IN UINT8  SocketId,
  IN UINT8  Port,
  IN UINT32 KtiAgentRt,
  IN BOOLEAN  ProgramPeMode
  );


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
  IN UINT8                         SocketId,
  IN UINT8                         Port,
  IN UINT8                         VgaEnable,
  IN UINT8                         VgaTarget
  );

//
// Function pointer definiton for ProgramKtiVgaSadTargets ()
//
typedef
VOID
(EFIAPI *PROGRAM_KTI_VGA_SAD_TARGETS) (
  IN UINT8                         SocketId,
  IN UINT8                         Port,
  IN UINT8                         VgaEnable,
  IN UINT8                         VgaTarget
  );

/**
  Set the enable_targeting_iioX of KTILK_TAD_IIO_STACKID_ENABLE_KTI_LLPMON_REG

  @param SocId       - CPU Socket Node number (Socket ID)
  @param Port        - UPI PORT
  @param StackBitmap - Stack Bitmap
**/
VOID
EFIAPI
ProgramKtiIioStackIdEnable (
  IN UINT8                 SocId,
  IN UINT8                 Port,
  IN UINT32                StackBitmap
  );

//
// Function pointer definiton for ProgramKtiIioStackIdEnable ()
//
typedef
VOID
(EFIAPI *PROGRAM_KTI_IIO_STACKID_ENABLE) (
  IN UINT8                 SocId,
  IN UINT8                 Port,
  IN UINT32                StackBitmap
  );


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
  );

//
// Function pointer definiton for GetKtiRate ()
//
typedef
UINT8
(EFIAPI *GET_KTI_RATE) (
  IN UINT8                    SocketId,
  IN UINT8                    Port
  );

/**
  Routine to set the mask / enable the COR PHY RESET error in cor_mask & cor_cnt_mask of
  KTIERRDIS0_KTI_LLPMON_REG

  @param SocketId               - Socket to program
  @param Port                   - Port to program
  @param MaskFlag               - It is used to indicate if it is going to mask the COR PHY RESET error in
                                  cor_mask & cor_cnt_mask; or enable the COR PHY RESET error in cor_mask &
                                  cor_cnt_mask

  @retval

**/
VOID
EFIAPI
ConfiguCorPhyResetErrOnKtiErrDis0 (
  IN UINT8                    SocketId,
  IN UINT8                    Port,
  IN BOOLEAN                  MaskFlag
  );

//
// Function pointer definiton for ConfiguCorPhyResetErrOnKtiErrDis0 ()
//
typedef
UINT8
(EFIAPI *CONFIG_COR_PHY_RESET_ERR_ON_KTIERRDIS0) (
  IN UINT8                    SocketId,
  IN UINT8                    Port,
  IN BOOLEAN                  MaskFlag
  );

/**
  Dump SNC related register in Kti Agents

  This routine is to dump SNC related register in Kti Agents
  @param  SocketId             - Socket Id
  @param  Port                 - Port ID

  @retval None
**/
VOID
EFIAPI
DumpKtiLlPmonSncXptRegister (
  IN UINT8 SocketId,
  IN UINT8 Port
  );

//
// Function pointer definiton for DumpKtiLlPmonSncXptRegister ()
//
typedef
VOID
(EFIAPI *DUMP_KTI_LLPMON_SNC_XPT_REGISTER) (
  IN UINT8 SocketId,
  IN UINT8 Port
  );


/**
  Program the mesh credits for the KTI Port

  @param  SocketId             - Socket Id
  @param  Port                 - Port ID
  @param  OutVn1En             - indicates whether VN1 is enabled (e.g. 8s topology)

  @retval None
**/
VOID
EFIAPI
ProgramKtiPortCreditSetting (
  IN UINT8 SocketId,
  IN UINT8 Port,
  IN UINT8 OutVn1En
  );

//
// Function pointer definiton for ProgramKtiPortCreditSetting ()
//
typedef
VOID
(EFIAPI *PROGRAM_KTI_PORT_CREDIT_SETTING) (
  IN UINT8 SocketId,
  IN UINT8 Port,
  IN UINT8 OutVn1En
  );


//
// KTI_REUT
//
/**
  Tell the link to report L1 state to the PCU
  Called from DisableLinkForceToLowPower(), KtiPhyLinkAfterWarmReset()

  @param SocketId,
  @param Port

  @retval None
**/
VOID
EFIAPI
SetUpiLinkToL1 (
  IN UINT8  SocketId,
  IN UINT8  Port
  );

//
// Function pointer definiton for SetUpiLinkToL1 ()
//
typedef
VOID
(EFIAPI *SET_UPI_LINK_TO_L1) (
  IN UINT8  SocketId,
  IN UINT8  Port
  );


/**
  Set c_init_begin to the value
  Called from DisableLinkForceToLowPower()

  @param SocketId,
  @param Port,
  @param Value

  @retval None
**/
VOID
EFIAPI
SetCInitBegin (
  IN UINT8  SocketId,
  IN UINT8  Port,
  IN UINT8  Value
  );

//
// Function pointer definiton for SetCInitBegin ()
//
typedef
VOID
(EFIAPI *SET_C_INIT_BEGIN) (
  IN UINT8  SocketId,
  IN UINT8  Port,
  IN UINT8  Value
  );

/**
  Set cp_reset
  Called from DisableLinkForceToLowPower() and IssuePhyResetOnCpuLink()

  @param SocketId,
  @param Port

  @retval None
**/
VOID
EFIAPI
SetCpReset (
  IN UINT8  SocketId,
  IN UINT8  Port
  );

//
// Function pointer definiton for SetCpReset ()
//
typedef
VOID
(EFIAPI *SET_CP_RESET) (
  IN UINT8  SocketId,
  IN UINT8  Port
  );


/**
  Get L0p/L1/FailoverEn settings
  Called from PrimeHostStructurePhyLinkLayer()

  @param SocketId,
  @param Port,
  @param *ccl0p,
  @param *ccl1,
  @param *ccfailover,

  @retval None
**/
VOID
EFIAPI
GetKtiReutCapabilities (
  IN UINT8                     SocketId,
  IN UINT8                     Port,
  OUT UINT32                   *ccl0p,
  OUT UINT32                   *ccl1,
  OUT UINT32                   *ccfailover
  );

//
// Function pointer definiton for GetKtiReutCapabilities ()
//
typedef
VOID
(EFIAPI *GET_KTI_REUT_CAPABILITIES) (
  IN UINT8                     SocketId,
  IN UINT8                     Port,
  OUT UINT32                   *ccl0p,
  OUT UINT32                   *ccl1,
  OUT UINT32                   *ccfailover
  );


/**
  Get current L0p/L1/FailoverEn settings
  Called from PrimeHostStructurePhyLinkLayer()

  @param SocketId,
  @param Port,
  @param *OutKtiLinkL0pEn,
  @param *OutKtiLinkL1En,
  @param *OutKtiFailoverEn

  @retval None
**/
VOID
EFIAPI
GetKtiReutCurrentSettings (
  IN UINT8                     SocketId,
  IN UINT8                     Port,
  OUT UINT8                    *OutKtiLinkL0pEn,
  OUT UINT8                    *OutKtiLinkL1En,
  OUT UINT8                    *OutKtiFailoverEn
  );

//
// Function pointer definiton for GetKtiReutCurrentSettings ()
//
typedef
VOID
(EFIAPI *GET_KTI_REUT_CURRENT_SETTINGS) (
  IN UINT8                     SocketId,
  IN UINT8                     Port,
  OUT UINT8                    *OutKtiLinkL0pEn,
  OUT UINT8                    *OutKtiLinkL1En,
  OUT UINT8                    *OutKtiFailoverEn
  );


/**
  Program late action register per CPU per link

  @param Cpu   - Socket ID
  @param Link  - Port number
  @param Value - Value to program

  @retval None

**/
VOID
EFIAPI
ProgramLateActionOnCpuLink (
  IN UINT8                    Cpu,
  IN UINT8                    Link,
  IN UINT32                   Value
  );

//
// Function pointer definiton for ProgramLateActionOnCpuLink ()
//
typedef
VOID
(EFIAPI *PROGRAM_LATE_ACTION_ON_CPU_LINK) (
  IN UINT8                    Cpu,
  IN UINT8                    Link,
  IN UINT32                   Value
  );

/**
  Program values for KTIREUT_PH_CTR1 bitfields according to KTIVAR and KTISETUP parameters
  called by ProgramPhyLayerControlRegisters()

  @param Cpu             - Socket ID
  @param Link            - Port number
  @param ProgramCNumInit - controls whether c_numinit bit gets programmed
  @param ProgramCOpSpeed - controls whether c_op_speed bit gets programmed
  @param c_l0p_en        - CSR bitfield (of same name) value to be programmed
  @param c_l1_en         - CSR bitfield (of same name) value to be programmed
  @param c_failover_en   - CSR bitfield (of same name) value to be programmed
  @param c_numinit       - CSR bitfield (of same name) value to be programmed

  @retval None

**/
VOID
EFIAPI
ProgramKtiReutPhCtr1 (
  IN UINT8                    Cpu,
  IN UINT8                    Link,
  IN BOOLEAN                  ProgramCNumInit,
  IN BOOLEAN                  ProgramCOpSpeed,
  IN UINT32                   c_l0p_en,
  IN UINT32                   c_l1_en,
  IN UINT32                   c_failover_en,
  IN UINT32                   c_numinit
  );

/**
  Program values for KTIREUT_PH_CTR2 register.

  @param SocketId        - Socket ID
  @param KtiPort         - Kti Port number
  @param RedriverStatus  - Kit Port redriver status, TRUE: redriver present; FALSE: no redriver or not care

  @retval None
**/
VOID
EFIAPI
ProgramKtiReutPhCtr2 (
  IN UINT8                    SocketId,
  IN UINT8                    KtiPort,
  IN BOOLEAN                  RedriverStatus
  );

//
// Function pointer definiton for ProgramKtiReutPhCtr1 ()
//
typedef
VOID
(EFIAPI *PROGRAM_KTI_REUT_PH_CTRL) (
  IN UINT8                    Cpu,
  IN UINT8                    Link,
  IN BOOLEAN                  ProgramCNumInit,
  IN BOOLEAN                  ProgramCOpSpeed,
  IN UINT32                   c_l0p_en,
  IN UINT32                   c_l1_en,
  IN UINT32                   c_failover_en,
  IN UINT32                   c_numinit
  );

/**
  Program values for KTIREUT_PH_CTR1 and KTIREUT_PH_CIS.
  called by KtiTransitionFullSpeedOnWarmReset()

  @param SocketId   - Socket ID
  @param Port  - Port number

  @retval NONE

**/
VOID
EFIAPI
ProgramKtiReutPhWA (
  UINT8                    SocketId,
  UINT8                    Port
  );

//
// Function pointer definiton for ProgramKtiReutPhWA ()
//
typedef
VOID
(EFIAPI *PROGRAM_KTI_REUT_PH_WA) (
  IN UINT8                    Cpu,
  IN UINT8                    Port
  );

/**
  get s_op_speed value

  @param SocId      - Socket ID
  @param Port       - Port number

  @retval 0 = Slow speed, 1 = Fast speed
**/
UINT8
EFIAPI
GetKtiSpeed (
  IN UINT8                    SocId,
  IN UINT8                    Port
  );

//
// Function pointer definiton for GetKtiSpeed ()
//
typedef
UINT8
(EFIAPI *GET_KTI_SPEED) (
  IN UINT8                    SocId,
  IN UINT8                    Port
  );


/**
  get s_link_up value

  @param Cpu        - Socket ID
  @param Link       - Port number

  @retval s_link_up  - s_link_up from KTIREUT_PH_CIS
**/
BOOLEAN
EFIAPI
IsLinkTrained (
  IN UINT8                    Cpu,
  IN UINT8                    Link
  );

//
// Function pointer definiton for IsLinkTrained ()
//
typedef
BOOLEAN
(EFIAPI *IS_LINK_TRAINED) (
  IN UINT8                    Cpu,
  IN UINT8                    Link
  );

/**
  Check if the KTIREUT_PH_CSS.s_tx_state bitfield value matches the PhyStatus after phy reset when
  c_init_begin is cleared

  @param Cpu         - Socket ID
  @param Link        - Port number
  @param PhyStatus   - KTI Link Phy status

  @retval TRUE  - s_tx_state matches the PhyStatus
  @retval FALSE - s_tx_state doesn't match the PhyStatus

**/
BOOLEAN
EFIAPI
CheckLinkInPhyStatus (
  IN  UINT8                    Cpu,
  IN  UINT8                    Link,
  IN  UINT8                    PhyStatus
  );

//
// Function pointer definiton for CheckLinkInPhyStatus ()
//
typedef
BOOLEAN
(EFIAPI *CHECK_LINK_IN_PHYSTATUS) (
  IN UINT8                    Cpu,
  IN UINT8                    Link,
  IN UINT8                    PhyStatus
  );

/**
  Set the Freq_Selection of PIPE_CLK_RATE_CTRL_KTI_REUT_REG for UPI speed transition.

  @param Cpu             - Socket ID
  @param Link            - Port number
  @param LinkSpeed       - Target Link Speed
  @param SpeedMode       - The target speed mode, 1: Fast mode; 0: slow mode

  @retval EFI_SUCCESS - on successful completion
          Others      - There is an error happened.

**/
EFI_STATUS
EFIAPI
ProgramLogPhyFreqSelecton (
  IN  UINT8                    Cpu,
  IN  UINT8                    Link,
  IN  UINT8                    LinkSpeed,
  IN  UINT8                    SpeedMode
  );

//
// Function pointer definiton for ProgramLogPhyFreqSelecton ()
//
typedef
EFI_STATUS
(EFIAPI *PROGRAM_LOGPHY_FREQ_SELECTION) (
  IN UINT8                    Cpu,
  IN UINT8                    Link,
  IN UINT8                    LinkSpeed,
  IN  UINT8                   SpeedMode
  );

/**
  Check if the cri_pclkchangeok_flag of PIPE_CLK_RATE_CTRL_KTI_REUT_REG is set.

  @param Cpu        - Socket ID
  @param Link       - Port number

  @retval FALSE - HSPHY clock change failed
          TRUE -  HSPHY clock change is done

**/
BOOLEAN
EFIAPI
CheckPhyClkChangeDone (
  IN  UINT8                    Cpu,
  IN  UINT8                    Link
  );

//
// Function pointer definiton for CheckPhyClkChangeDone ()
//
typedef
EFI_STATUS
(EFIAPI *CHECK_PHY_CLK_CHANGE_DONE) (
  IN UINT8                    Cpu,
  IN UINT8                    Link
  );

/**
  Clear the cri_pclkchangeok_flag of PIPE_CLK_RATE_CTRL_KTI_REUT_REG.

  @param SocketId        - Socket ID
  @param Port            - Port number

  @retval none

**/
VOID
EFIAPI
ClearPhyClkChangeOkFlag (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port
  );

//
// Function pointer definiton for ClearPhyClkChangeOkFlag ()
//
typedef
EFI_STATUS
(EFIAPI *CLEAR_PHY_CLK_CHANGE_OK_FLAG) (
  IN UINT8                    Cpu,
  IN UINT8                    Link
  );

/**
  Force Phy Layer readapt before full-speed transition
  Set sp_rx_calib AND Set sp_tx_calib
  Called from ProgramPhyLayerControlRegisters()

  @param Cpu        - Socket ID
  @param Link       - Port number

  @retval None
**/
VOID
EFIAPI
ForcePhyLayerReadapt (
  IN UINT8                    Cpu,
  IN UINT8                    Link
  );

//
// Function pointer definiton for ForcePhyLayerReadapt ()
//
typedef
VOID
(EFIAPI *FORCE_PHY_LAYER_READAPT) (
  IN UINT8                    Cpu,
  IN UINT8                    Link
  );


/**
  Set Linear time value SKX-workaround s4929026, s5370781
  Called from ProgramPhyLayerControlRegisters()

  @param Cpu        - Socket ID
  @param Link       - Port number
  @param LinkSpeed  - Current link speed mode; 0 - Slow Speed; 1- Full Speed.

  @retval None
**/
VOID
EFIAPI
SetKtiL0pExitWorkaround (
  IN UINT8                    Cpu,
  IN UINT8                    Link,
  IN UINT8                    LinkSpeed
  );

//
// Function pointer definiton for SetKtiL0pExitWorkaround ()
//
typedef
VOID
(EFIAPI *SET_KTI_L0P_EXIT_WORKAROUND) (
  IN UINT8                    Cpu,
  IN UINT8                    Link,
  IN UINT8                    LinkSpeed
  );

/**
  Increasing the L0p lock time to workaround the issue where UPI L0p transitions
  appear to be hitting a "phase wander" condition where the inactive lanes are
  not tracking like the active ones are and upon exit a relock is necessary
  creating a correctable CRC.

  @param[in] SocketId      Socket ID
  @param[in] Port          UPI Port number
**/
VOID
EFIAPI
ExtendKtiL0pExitTime (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port
  );
//
// Function pointer definition for ExtendKtiL0pExitTime ()
//
typedef
VOID
(EFIAPI *EXTEND_KTI_L0P_EXIT_TIME) (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port
  );

/**
  Set Physical Determinism Control
  Called from ProgramPhyLayerControlRegisters()

  @param Cpu        - Socket ID
  @param Link       - Port number

  @retval None
**/
VOID
EFIAPI
SetPhysicalDeterminismControl (
  IN UINT8                    Cpu,
  IN UINT8                    Link
  );

//
// Function pointer definiton for SetPhysicalDeterminismControl ()
//
typedef
VOID
(EFIAPI *SET_PHYSICAL_DETERMINISM_CONTROL) (
  IN UINT8                    Cpu,
  IN UINT8                    Link
  );

/**
  Set c_init value

  @param SocketId   - Socket ID
  @param Port       - Port number
  @param Data       - Value write to c_init

  @retval

**/
VOID
EFIAPI
SetKtiCInit (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port,
  IN  UINT8                    Data
  );

//
// Function pointer definiton for SetKtiCInit ()
//
typedef
VOID
(EFIAPI *SET_KTI_C_INIT) (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port,
  IN  UINT8                    Data
  );

#if defined (IA32)
/**
Polls for cp_reset to complete

@param SocId             - CPU Socket Number, 0 based
@param Port              - CPU Port Number, 0 based

@retval EFI_SUCCESS      - cp_reset was cleared
@retval EFI_DEVICE_ERROR - timeout waiting for cp_reset bit to clear
**/
EFI_STATUS
EFIAPI
KtiPollForLinkResetCpl (
  IN UINT8                      SocId,
  IN UINT8                      Port
);

//
// Function pointer definiton for KtiPollForLinkResetCpl ()
//
typedef
EFI_STATUS
(EFIAPI *KTI_POLL_FOR_LINK_RESET_CPL) (
  IN UINT8                      SocId,
  IN UINT8                      Port
  );

/**
Clear UPI link errors for SlowMode WA, called per-link

@param Socket          - Socket ID
@param Link            - Link number

**/
VOID
EFIAPI
ClearSlowModeWaLinkErrors (
  IN UINT8                      Socket,
  IN UINT8                      Link
);

//
// Function pointer definiton for ClearSlowModeWaLinkErrors ()
//
typedef
VOID
(EFIAPI *CLEAR_SLOW_MODE_WA_LINK_ERRORS) (
  IN UINT8                      Socket,
  IN UINT8                      Link
  );


/**

Checks the link in slow mode and attempts to retrain if link failed

@param SocId             - CPU Socket Number, 0 based
@param Port              - CPU Port Number, 0 based

@retval TRUE             - Repaired
@retval FALSE            - Failed to repaired

**/
BOOLEAN
EFIAPI
ApplySlowModePhyRepair (
  UINT8                      SocId,
  UINT8                      Port
);

//
// Function pointer definiton for ApplySlowModePhyRepair ()
//
typedef
BOOLEAN
(EFIAPI *APPLY_SLOW_MODE_PHY_REPAIR) (
  UINT8                      SocId,
  UINT8                      Port
  );


/**
Check if this link trained to L0

@param SocId             - CPU Socket Number, 0 based
@param Port              - CPU Port Number, 0 based

@retval EFI_SUCCESS      - UPI Link trained
@retval EFI_DEVICE_ERROR - UPI Link failed to train in slow mode

**/
EFI_STATUS
EFIAPI
IsLinkTrainedInSlowMode (
  UINT8                      SocId,
  UINT8                      Port
);

//
// Function pointer definiton for IsLinkTrainedInSlowMode ()
//
typedef
EFI_STATUS
(EFIAPI *IS_LINK_TRAINED_IN_SLOW_MODE) (
  UINT8                      SocId,
  UINT8                      Port
  );


/**
Check if other side of link recovered

@param SocId             - CPU Socket Number, 0 based
@param Port              - CPU Port Number, 0 based

@retval TRUE             - other side of link recovered, force use of KTILS
@retval FALSE            - link is fine, return

**/
BOOLEAN
EFIAPI
IsOtherSideOfLinkRecovered (
  UINT8                      SocId,
  UINT8                      Port
);

//
// Function pointer definiton for IsOtherSideOfLinkRecovered ()
//
typedef
BOOLEAN
(EFIAPI *IS_OTHER_SIDE_OF_LINK_RECOVERED) (
  UINT8                      SocId,
  UINT8                      Port
  );


/**
Check for any indications of a link layer problem

@param SocId             - CPU Socket Number, 0 based
@param Port              - CPU Port Number, 0 based

@retval TRUE             - Link layer problem detected
@retval FALSE            - Link layer problem not detected
**/
BOOLEAN
EFIAPI
DetectLinkLayerProblem (
  UINT8                      SocId,
  UINT8                      Port
);

//
// Function pointer definiton for DetectLinkLayerProblem ()
//
typedef
BOOLEAN
(EFIAPI *DETECT_LINK_LAYER_PROBLEM) (
  UINT8                      SocId,
  UINT8                      Port
  );
#endif //defined (IA32)


//
// M3KTI_MAIN
//
/**
  Returns the total number of M3KTI agents

  @param SocId             - Soc ID with M3KTI agents
  @param MaxM3kti          - Max M3KTI agents

  @retval number of M3KTI
**/
UINT8
EFIAPI
GetM3KtiCount (
  IN UINT8                      SocId,
  IN UINT8                      MaxM3kti
  );

//
// Function pointer definiton for GetM3KtiCount ()
//
typedef
UINT8
(EFIAPI *GET_M3KTI_COUNT) (
  IN UINT8                      SocId,
  IN UINT8                      MaxM3kti
  );


/**
  Routine to program M3EFQPrivC2 register

  @param SocId             - Socket ID
  @param M3Kti             - M3KTI number

  @retval None
**/
VOID
EFIAPI
ProgramM3EFQPrivC2 (
  IN UINT8                      SocId,
  IN UINT8                      M3Kti
  );

//
// Function pointer definiton for ProgramM3EFQPrivC2 ()
//
typedef
VOID
(EFIAPI *PROGRAM_M3EFQPRIVC2) (
  IN UINT8                      SocId,
  IN UINT8                      M3Kti
  );


/**
  Mask the remotevnacreditoverflow
  SKX WA s5372538

  @param SocId                       - Socket ID
  @param M3Kti                       - M3KTI number

  @retval None
**/
VOID
EFIAPI
MaskRemoteVnaCreditOverflow (
  IN UINT8                      SocId,
  IN UINT8                      M3Kti
  );

//
// Function pointer definiton for MaskRemoteVnaCreditOverflow ()
//
typedef
VOID
(EFIAPI *MASK_REMOTE_VNA_CREDIT_OVERFLOW) (
  IN UINT8                      SocId,
  IN UINT8                      M3Kti
  );


/**
  Routine to collect and clear M3KTI errors

  @param SocId             - Socket to program
  @param M3Kti             - M3KTI number
  @param M3KtiErrLog       - pointer to M3KTI error log structure

  @retval None

**/
VOID
EFIAPI
ClearM3KtiErrors (
  IN UINT8                      SocId,
  IN UINT8                      M3Kti,
  OUT KTI_CPU_M3KTI_ERRLOG   *M3KtiErrLog
  );

//
// Function pointer definiton for ClearM3KtiErrors ()
//
typedef
VOID
(EFIAPI *CLEAR_M3KTI_ERRORS) (
  IN UINT8                      SocId,
  IN UINT8                      M3Kti,
  OUT KTI_CPU_M3KTI_ERRLOG   *M3KtiErrLog
  );


/**
  Program  M3INGDBGREMCRE_0/1_KTI0/1_M3KTI_MAIN_REG, the value need to match with LCCL setting on peer port of peer socket

  @param SocId             - Socket ID
  @param M3Kti             - M3Kti number
  @param VnaCredit0        - Vna Credit data 0
  @param VnaCredit1        - Vna Credit data 1, OPTIONAL
  @param VnaCredit2        - Vna Credit data 2, OPTIONAL

  @retval None

**/
VOID
EFIAPI
ProgramLinkLayerOnM3Kti (
  IN UINT8                    SocId,
  IN UINT8                    M3Kti,
  IN UINT32                   *VnaCredit0,
  IN UINT32                   *VnaCredit1  OPTIONAL,
  IN UINT32                   *VnaCredit2  OPTIONAL
  );

//
// Function pointer definiton for ProgramLinkLayerOnM3Kti ()
//
typedef
VOID
(EFIAPI *PROGRAM_LINK_LAYER_ON_M3KTI) (
  IN UINT8                    SocId,
  IN UINT8                    M3Kti,
  IN UINT32                   *VnaCredit0,
  IN UINT32                   *VnaCredit1  OPTIONAL,
  IN UINT32                   *VnaCredit2  OPTIONAL
  );


/**
  Calculate the M3KTI pass-thru route encoding for given input & output port

  @param[in] SocId        - Socket to program
  @param[in] InPort       - input KTI port for route through
  @param[in] OutPort      - Output KTI port for route through
  @param[in] DestSocId    - endpoint processor socket ID
  @param[in,out]Data32    - current m3kti routing table value
  @param[in] ShareUpiP0P1 - UPI0 and UPI1 shared one Instance

**/
VOID
EFIAPI
ConvertM3KTIRoutingTable (
  IN UINT8                   SocId,
  IN UINT8                   InPort,
  IN UINT8                   OutPort,
  IN UINT8                   DestSocId,
  IN UINT32                  *Data32,
  IN BOOLEAN                 ShareUpiP0P1
  );

//
// Function pointer definiton for ConvertM3KTIRoutingTable ()
//
typedef
VOID
(EFIAPI *CONVERT_M3KTI_ROUTING_TABLE) (
  IN UINT8                   SocId,
  IN UINT8                   InPort,
  IN UINT8                   OutPort,
  IN UINT8                   DestSocId,
  IN UINT32                  *Data32,
  IN BOOLEAN                 ShareUpiP0P1
  );

/**
  Program M3KTI routing table.

  @param InPort      - source port
  @param InterSocket - intermediate socket ID
  @param DestSocId   - destination socket ID
  @param OutPort     - endpoint port
  @param ShareUpiP0P1- UPI0 and UPI1 shared one Instance

  @retval None

**/
VOID
EFIAPI
ProgramM3KtiRoute (
  IN UINT8                     InPort,
  IN UINT8                     InterSocket,
  IN UINT8                     DestSocId,
  IN UINT8                     OutPort,
  IN BOOLEAN                   ShareUpiP0P1
  );

//
// Function pointer definiton for ProgramM3KtiRoute ()
//
typedef
VOID
(EFIAPI *PROGRAM_M3KTI_ROUTE) (
  IN UINT8                     InPort,
  IN UINT8                     InterSocket,
  IN UINT8                     DestSocId,
  IN UINT8                     OutPort,
  IN BOOLEAN                   ShareUpiP0P1
  );


/**
  Configure M3KTI mesh stop.  This routine programs the route table, virtual
  network, direct to core enabling, pe_mode and other miscellaneous setting on
  M3KTI.

  @param[in] SocId                 - Socket Id
  @param[in] M3Kti                 - M3KTI mesh stop ID
  @param[in] M3KtiRt               - M3KTI Route table setting
  @param[in] M3KtiRt1              - M3KTI Route table setting for Port 1 on this mesh stop; this field
                                     is only applicable to CPX
  @param[in] IgrVnSel              - Virtual network selection setting
  @param[in] D2cEn                 - Direc-to-core enabling. True/False
  @param[in] ProgramPeMode         - PE bit setting. True/False
  @param[in] DisableMeshCreditPush - Set to TRUE to disable mesh credit push on M3KTI tile; FALSE otherwise.

  @retval None
**/
VOID
EFIAPI
ProgramM3KtiMeshStop (
  IN UINT8                    SocId,
  IN UINT8                    M3Kti,
  IN UINT32                   M3KtiRt,
  IN UINT32                   M3KtiRt1,
  IN UINT32                   IgrVnSel,
  IN BOOLEAN                  D2cEn,
  IN BOOLEAN                  ProgramPeMode,
  IN BOOLEAN                  DisableMeshCreditPush
  );

//
// Function pointer definition for ProgramM3KtiMeshStop ()
//
typedef
VOID
(EFIAPI *PROGRAM_M3KTI_MESH_STOP) (
  IN UINT8                    SocId,
  IN UINT8                    M3Kti,
  IN UINT32                   M3KtiRt,
  IN UINT32                   M3KtiRt1,
  IN UINT32                   IgrVnSel,
  IN BOOLEAN                  D2cEn,
  IN BOOLEAN                  ProgramPeMode,
  IN BOOLEAN                  DisableMeshCreditPush
  );


/**
  Program M3KTI RPQ VNA Credits

  @param SocId             - Socket Id
  @param M3Kti             - M3Kti number
  @param RpqVna            - rpq_vna credits to program in M3KEGRCTRL registers

  @retval None
**/
VOID
EFIAPI
ProgramM3KtiRpqVnaCredits (
  IN UINT8                    SocId,
  IN UINT8                    M3Kti,
  IN UINT32                   RpqVna
  );

//
// Function pointer definiton for ProgramM3KtiRpqVnaCredits ()
//
typedef
VOID
(EFIAPI *PROGRAM_M3KTI_RPQ_VNA_CREDITS) (
  IN UINT8                    SocId,
  IN UINT8                    M3Kti,
  IN UINT32                   RpqVna
  );

/**
  Program M3KTI to M2IAL NCB and NCS Credits

  @param SocId             - Socket Id
  @param M3Kti             - M3Kti number
  @param NcbCredit         - Ncb Credit value
  @param NcsCredit         - Ncs Credit value

  @retval None
**/
VOID
EFIAPI
M3KtiConfigureM3KtiToM2IalCredit (
  IN UINT8                     SocId,
  IN UINT8                     M3Kti,
  IN UINT8                     NcbCredit,
  IN UINT8                     NcsCredit
  );

//
// Function pointer definiton for M3KtiConfigureM3KtiToM2IalCredit ()
//
typedef
VOID
(EFIAPI *M3KTI_CONFIGURE_M3KTI_TO_M2IAL_CREDIT) (
  IN UINT8                     SocId,
  IN UINT8                     M3Kti,
  IN UINT8                     NcbCredit,
  IN UINT8                     NcsCredit
  );


/**
  Program M3KTI IIO NCB and NCS Credits

  @param SocId             - Socket Id
  @param M3Kti             - M3Kti number

  @retval None
**/
VOID
EFIAPI
ProgramM3KtiNcbNcsCredits (
  IN UINT8                    SocId,
  IN UINT8                    M3Kti
  );

//
// Function pointer definiton for ProgramM3KtiNcbNcsCredits ()
//
typedef
VOID
(EFIAPI *PROGRAM_M3KTI_NCB_NCS_CREDITS) (
  IN UINT8                    SocId,
  IN UINT8                    M3Kti
  );


/**
  Program M3KTI VNA/VN0 Credits

  @param SocId               - Socket Id
  @param M3Kti               - M3Kti number
  @param M2upiRouteThruAdVna - AD Vna credits to program in M3KRTE registers
  @param M2upiRouteThruAdVn0 - AD Vn0 credits to program in M3KRTE registers
  @param M2upiRouteThruBlVna - BL Vna credits to program in M3KRTE registers
  @param M2upiRouteThruBlVn0 - BL Vn0 credits to program in M3KRTE registers

  @retval None
**/
VOID
EFIAPI
ProgramM3KtiRouteThruCredits (
  IN UINT8                    SocId,
  IN UINT8                    M3Kti,
  IN UINT32                   M2upiRouteThruAdVna,
  IN UINT32                   M2upiRouteThruAdVn0,
  IN UINT32                   M2upiRouteThruBlVna,
  IN UINT32                   M2upiRouteThruBlVn0
  );

//
// Function pointer definiton for ProgramM3KtiRouteThruCredits ()
//
typedef
VOID
(EFIAPI *PROGRAM_M3KTI_ROUTE_THRU_CREDITS) (
  IN UINT8                    SocId,
  IN UINT8                    M3Kti,
  IN UINT32                   M2upiRouteThruAdVna,
  IN UINT32                   M2upiRouteThruAdVn0,
  IN UINT32                   M2upiRouteThruBlVna,
  IN UINT32                   M2upiRouteThruBlVn0
  );


/**
  Routine to setup M3KTI misc
  Called at end of SetupKtiMisc()

  SKX WA HSD 4929504: KTI tile performance critical CSRs for BIOS programming

  @param SocId             - Socket to program
  @param M3Kti             - M3Kti number

  @retval None
**/
VOID
EFIAPI
SetupM3KtiMisc (
  IN UINT8                    Soc,
  IN UINT8                    M3Kti
  );

//
// Function pointer definiton for SetupM3KtiMisc ()
//
typedef
VOID
(EFIAPI *SETUP_M3KTI_MISC) (
  IN UINT8                    Soc,
  IN UINT8                    M3Kti
  );


/**
  Routine to program M3KTI MiniSAD registers

  @param SocId             - Socket to program
  @param M3Kti             - M3Kti number
  @param UpiMiniSADMode    - MiniSAD Mode for M3KPRECTRL
  @param UPIMiniSad        - MiniSAD Table for M3KPRETL

  @retval None
**/
VOID
EFIAPI
ProgramM3ktiMiniSad (
  IN UINT8                       Soc,
  IN UINT8                       M3Kti,
  IN UINT32                      UpiMiniSadMode,
  IN UINT32                      UPIMiniSad
  );

//
// Function pointer definiton for ProgramM3ktiMiniSad ()
//
typedef
VOID
(EFIAPI *PROGRAM_M3KTI_MINI_SAD) (
  IN UINT8                       Soc,
  IN UINT8                       M3Kti,
  IN UINT32                      UpiMiniSadMode,
  IN UINT32                      UPIMiniSad
  );


/**
  Routine to program M3KTI Snoop Fanout

  @param[in] SocId             Socket to program
  @param[in] M3Kti             M3Kti number
  @param[in] M3KtiSnpFanOut    Pointer to an array of Snoop Fanout route setting (Element 0 is
                               for Port 0, element 1 is for Port 1. Port 1 is only applicable to
                               SOC that has shared KTI agent.
  @param[in] M3KtiSnpFanOut1   Pointer to another array of Snoop Fanout route setting for SOC.
                               Currently this parameter is only used by CPX for destination node
                               IDs 4 to 7.
  @param[in] ProgramPeMode     PE bit setting. True/False

  @retval None
**/
VOID
EFIAPI
ProgramM3KtiSnoopFanout (
  IN UINT8                    SocId,
  IN UINT8                    M3Kti,
  IN UINT32                   *M3KtiSnpFanOut,
  IN UINT32                   *M3KtiSnpFanOut1,
  IN BOOLEAN                  ProgramPeMode
  );

//
// Function pointer definition for ProgramM3KtiSnoopFanout ()
//
typedef
VOID
(EFIAPI *PROGRAM_M3KTI_SNOOP_FANOUT) (
  IN UINT8                    SocId,
  IN UINT8                    M3Kti,
  IN UINT32                   *M3KtiSnpFanOut,
  IN UINT32                   *M3KtiSnpFanOut1,
  IN BOOLEAN                  ProgramPeMode
  );


/**
  Get the number of M3KTI -> M2PCIE BL credits to program in each bitfield, for each KTI Port

  @param SocId           - Socket to program
  @param M2Pcie          - M2Pcie index
  @param *KtiBlArray     - pointer to a table of credit values for current system config
  @param *OutM3KPciPort0 - Pointer to variable to store M3KTI -> M2PCIE BL Credit CSR value for KTI Port 0
  @param *OutM3KPciPort1 - Pointer to variable to store M3KTI -> M2PCIE BL Credit CSR value for KTI Port 1
  @param *OutM3KPciPort2 - Pointer to variable to store M3KTI -> M2PCIE BL Credit CSR value for KTI Port 2

  @retval None
**/
VOID
EFIAPI
GetM3KtiToM2PcieBlCredits (
  IN UINT8      SocId,
  IN UINT8      M2Pcie,
  IN UINT8      *KtiBlArray,//[MAX_KTI_M2PCIE_ENTRY];
  IN OUT UINT32 *OutM3KPciPort0,
  IN OUT UINT32 *OutM3KPciPort1,
  IN OUT UINT32 *OutM3KPciPort2
  );

//
// Function pointer definiton for GetM3KtiToM2PcieBlCredits ()
//
typedef
VOID
(EFIAPI *GET_M3KTI_TO_M2PCIE_BL_CREDITS) (
  IN UINT8  SocId,
  IN UINT8  M2Pcie,
  IN UINT8  *KtiBlArray,
  IN UINT32 *OutM3KPciPort0,
  IN UINT32 *OutM3KPciPort1,
  IN UINT32 *OutM3KPciPort2
  );


/**
  Program the M3KTI -> M2PCIE BL credits determined by GetM3KtiToM2PcieBlCredits()

  @param SocId           - Socket index
  @param Port            - Port index
  @param *OutM3KPciPort0 - Pointer to M3KTI -> M2PCIE BL Credit CSR value for KTI Port 0
  @param *OutM3KPciPort1 - Pointer to M3KTI -> M2PCIE BL Credit CSR value for KTI Port 1
  @param *OutM3KPciPort2 - Pointer to M3KTI -> M2PCIE BL Credit CSR value for KTI Port 2

  @retval None
**/
VOID
EFIAPI
ProgramM3KtiToM2PcieBlCredits (
  IN UINT8 SocId,
  IN UINT8 Port,
  IN UINT32 *OutM3KPciPort0,
  IN UINT32 *OutM3KPciPort1,
  IN UINT32 *OutM3KPciPort2
  );

//
// Function pointer definiton for ProgramM3KtiToM2PcieBlCredits ()
//
typedef
VOID
(EFIAPI *PROGRAM_M3KTI_TO_M2PCIE_BL_CREDITS) (
  IN UINT8 SocId,
  IN UINT8 Port,
  IN UINT32 *OutM3KPciPort0,
  IN UINT32 *OutM3KPciPort1,
  IN UINT32 *OutM3KPciPort2
  );


/**
  Dump SNC related register in M3KTI

  This routine is to dump SNC related register M3KTI

  @param  Soc                  -  Socket ID
  @param  M3Kti                -  M3KTI ID

  @retval None

**/
VOID
EFIAPI
DumpM3KtiSncXptRegister (
  IN UINT8                    Soc,
  IN UINT8                    M3Kti
  );

//
// Function pointer definiton for DumpM3KtiSncXptRegister ()
//
typedef
VOID
(EFIAPI *DUMP_M3KTI_SNC_XPT_REGISTER) (
  IN UINT8                    Soc,
  IN UINT8                    M3Kti
  );

/**
  ICXSP R0 WA: Mask address parity error for P2P transactions

  @param SocId     - CPU Socket Node number (Socket ID)
  @param Port      - UPI PORT
**/
VOID
EFIAPI
MaskP2PTxnsErr (
  UINT8                 SocId,
  UINT8                 Port
  );

/**
  ICXSP R0 WA: enable M3KINGPRIVC1.AllowRateMatchingStallWhenUclkSlower

  @param SocId             - Socket ID
  @param M3Kti             - M3Kti number

**/
VOID
EFIAPI
ProgramExtraStallOnM3Kti (
  UINT8                    SocId,
  UINT8                    M3Kti
  );

//
// Function pointer definiton for ProgramExtraStallOnM3Kti ()
//
typedef
VOID
(EFIAPI *M3KTI_EXTRA_STALL_WA) (
  UINT8                    SocId,
  UINT8                    M3Kti
  );

/**
  ICXSP R0 WA: set M3KINGPRIVC1.DisableAdBypassBusy1

  @param SocId             - Socket ID
  @param M3Kti             - M3Kti number

**/
VOID
EFIAPI
ProgramDisableAdByPassOnM3Kti (
  UINT8                    SocId,
  UINT8                    M3Kti
  );

//
// Function pointer definiton for ProgramExtraStallOnM3Kti ()
//
typedef
VOID
(EFIAPI *M3KTI_DISABLE_ADBYPASS_WA) (
  UINT8                    SocId,
  UINT8                    M3Kti
  );

/**
  Get sp_use_back_channel value

  @param SocketId   - Socket ID
  @param Link       - Port number

  @retval: the value of sp_use_back_channel

**/
UINT8
EFIAPI
GetKtiSpUseBackChannel (
  UINT8                    SocketId,
  UINT8                    Link
  );

//
// Function pointer definition for GetKtiSpUseBackChannel ()
//
typedef
UINT8
(EFIAPI *GET_KTI_SP_USE_BACK_CHANNEL) (
  UINT8                        SocketId,
  UINT8                        Link
  );

/**
  Set sp_use_back_channel value

  @param SocketId   - Socket ID
  @param Link       - Port number
  @param Data       - Value write to SpUseBackChannel

  @retval

**/
VOID
EFIAPI
SetKtiSpUseBackChannel (
  UINT8                    SocketId,
  UINT8                    Link,
  UINT8                    Data
  );

//
// Function pointer definition for SetKtiSpUseBackChannel ()
//
typedef
VOID
(EFIAPI *SET_KTI_SP_USE_BACK_CHANNEL) (
  UINT8                        SocketId,
  UINT8                        Link,
  UINT8                        Data
  );

/**
  Check if the KTIREUT_PH_CIS.sp_use_back_channel bitfield value be set to 1 by HW

  @param SocketId   - Socket ID
  @param Link       - Port number

  @retval TRUE  - sp_use_back_channel is set to 1 by HW
  @retval FALSE - Timeout

**/
BOOLEAN
EFIAPI
PollKtiSpUseBackChannel (
  UINT8                    SocketId,
  UINT8                    Link
  );

//
// Function pointer definition for PollKtiSpUseBackChannel ()
//
typedef
BOOLEAN
(EFIAPI *POLL_KTI_SP_USE_BACK_CHANNEL) (
  UINT8                        SocketId,
  UINT8                        Link
  );

/**
  Program the PH_TCC with the safe coeff

  @param SocketId   - Socket ID
  @param Link       - Port number
  @param Data       - Safe coeff for all lanes

  @retval

**/
VOID
EFIAPI
SetKtiPhTcc (
  UINT8                    SocketId,
  UINT8                    Link,
  UINT32                   Data
  );

//
// Function pointer definiton for SetKtiPhTcc ()
//
typedef
VOID
(EFIAPI *SET_KTI_PH_TCC) (
  UINT8                        SocketId,
  UINT8                        Link,
  UINT8                        Data
  );

/**
  Program the PH_TCA with the safe coeff for adaptaion

  @param SocketId   - Socket ID
  @param Link       - Port number
  @param Data       - Safe coeff for all lanes

  @retval

**/
VOID
EFIAPI
SetKtiPhTca (
  IN  UINT8                    SocketId,
  IN  UINT8                    Link,
  IN  UINT32                   Data
  );

//
// Function pointer definiton for SetKtiPhTca ()
//
typedef
VOID
(EFIAPI *SET_KTI_PH_TCA) (
  UINT8                        SocketId,
  UINT8                        Link,
  UINT8                        Data
  );

/**
  Set the c_tx_adapt_enable Bit

  @param SocketId   - Socket ID
  @param Link       - Port number
  @param Data       - Data write to the c_tx_adapt_enable

  @retval

**/
VOID
EFIAPI
SetKtiTxAdaptEn (
  IN  UINT8                    SocketId,
  IN  UINT8                    Link,
  IN  UINT8                    Data
  );

//
// Function pointer definiton for SetKtiTxAdaptEn ()
//
typedef
VOID
(EFIAPI *SET_KTI_TX_ADAPT_EN) (
  UINT8                        SocketId,
  UINT8                        Link,
  UINT8                        Data
  );

/**
  Set the Start_Test Bit and exploopcnt (optional) for the txeq adaptation flow

  @param SocketId   - Socket ID
  @param Link       - Port number
  @param ExpLoopCnt - Exponential Loop Count Exponential Test Loop Count.

  @retval

**/
VOID
EFIAPI
SetKtiPhTtpgc (
  IN  UINT8                    SocketId,
  IN  UINT8                    Link,
  IN  UINT8                    *ExpLoopCnt OPTIONAL
  );

//
// Function pointer definiton for SetKtiPhTtpgc ()
//
typedef
VOID
(EFIAPI *SET_KTI_PH_TTPGC) (
  UINT8                        SocketId,
  UINT8                        Link,
  UINT8                        *ExpLoopCnt OPTIONAL
  );

/**
  Set init_check_mask for the txeq adaptation flow

  @param SocketId      - Socket ID
  @param Link          - Port number
  @param InitCheckMask - Initial Check Mask

  @retval

**/
VOID
EFIAPI
SetKtiPhTtpcc (
  IN  UINT8                    SocketId,
  IN  UINT8                    Link,
  IN  UINT8                    InitCheckMask
  );

//
// Function pointer definiton for SetKtiPhTtpcc ()
//
typedef
VOID
(EFIAPI *SET_KTI_PH_TTPCC) (
  UINT8                        SocketId,
  UINT8                        Link,
  IN  UINT8                    InitCheckMask
  );

/**
  Set t_tdst for the slow mode DET_TX secondary timer

  @param SocketId      - Socket ID
  @param Port          - Port number
  @param Ttdst         - Time for Squelch exit detect/debounce

  @retval

**/
VOID
EFIAPI
SetKtiPhTsecSl (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port,
  IN  UINT8                    Ttdst
  );

//
// Function pointer definiton for SetKtiPhTsecSl ()
//
typedef
VOID
(EFIAPI *SET_KTI_PH_TSEC_SL) (
  UINT8                        SocketId,
  UINT8                        Link,
  UINT8                        Ttdst
  );

/**
  Get the sleffm of PH_FOM_LANE

  @param SocketId   - Socket ID
  @param Link       - Port number
  @param Lane       - Lane number

  @retval: TRUE  -- The FOM value of the Socket x Port y Lane z returned is valid
           FALSE -- The FOM value of the Socket x Port y Lane z returned is invalid

**/
BOOLEAN
EFIAPI
GetKtiPhFomLane (
  IN  UINT8                    SocketId,
  IN  UINT8                    Link,
  IN  UINT8                    Lane,
  OUT UINT8                    *FomOfLane
  );

//
// Function pointer definiton for GetKtiPhFomLane ()
//
typedef
VOID
(EFIAPI *GET_KTI_PH_FOM_LANE) (
  UINT8                        SocketId,
  UINT8                        Link,
  UINT8                        Lane,
  UINT8                        *FomOfLane
  );

/**
  Set the ctxcoefficientperlane of PH_TCL_LANE

  @param SocketId   - Socket ID
  @param Link       - Port number
  @param Lane       - Lane number
  @param Data       - value write to ctxcoefficientperlane

  @retval

**/
VOID
EFIAPI
SetKtiPhTclLane (
  IN  UINT8                    SocketId,
  IN  UINT8                    Link,
  IN  UINT8                    Lane,
  IN  UINT32                   Data
  );

//
// Function pointer definiton for SetKtiPhTclLane ()
//
typedef
VOID
(EFIAPI *SET_KTI_PH_TCL_LANE) (
  UINT8                        SocketId,
  UINT8                        Link,
  UINT8                        Lane,
  UINT32                       Data
  );

/**
Get KTI Link Frequency

@param SocketId              - Socket Id to program
@param Link                  - Port to program

@retval KTI Pipe rate for the KTI port
**/
UINT8
EFIAPI
GetKtiPipePhRate (
  IN UINT8                    SocketId,
  IN UINT8                    Link
  );

//
// Function pointer definiton for GetKtiPipePhRate ()
//
typedef
VOID
(EFIAPI *GET_KTI_PIPE_PH_RATE) (
  UINT8                        SocketId,
  UINT8                        Link
  );

typedef struct {
  //
  // Function pointers
  //
  IS_IAR_WRITE_ENABLE_CLEAR                                   IsIarWriteEnableClear;
  GET_IAR_DATA                                                GetIarData;
  WRITE_IAR_DATA                                              WriteIarData;
  OVERRIDE_RX_DCC_EXT_VAL                                     OverrideRxDccExtVal;
  APPLY_RX_DCC_CLOCK_OVERRIDE                                 ApplyRxDccClockOverride;
  SET_KTI_DFX_LCK                                             SetKtiDfxLck;
  IS_LBC_START_CLEAR                                          IsLbcStartClear;
  WRITE_LBC_DATA                                              WriteLbcData;
  GET_KTI_LLPMON_VENDOR_ID                                    GetKtiLlpmonVendorId;
  COLLECT_AND_CLEAR_KTI_PORT_ERRORS                           CollectAndClearKtiPortErrors;
  PROGRAM_D2C_REGISTER                                        ProgramD2CRegister;
  PROGRAM_KTI_AGENT_PREFETCH                                  ProgramKtiAgentPrefetch;
  GET_KTI_PREFETCH                                            GetKtiPrefetch;
  PROGRAM_LINK_CREDIT_CONTROL                                 ProgramLinkCreditControl;
  SET_KTI_LINK_CRC_MODE                                       SetKtiLinkCrcMode;
  GET_CRC_MODE                                                GetCrcMode;
  TRIGGER_KTI_LINK_LAYER_RESET                                TriggerKtiLinkLayerReset;
  CHECK_KTI_LINK_LAYER_RESET_RESULT                           CheckKtiLinkLayerResetResult;
  GET_VN1_SUPPORTED                                           GetVn1Supported;
  PROGRAM_KTI_BUS_TAD                                         ProgramKtiBusTad;
  PROGRAM_KTI_IO_TAD                                          ProgramKtiIoTad;
  PROGRAM_KTI_IO_APIC_TAD                                     ProgramKtiIoApicTad;
  PROGRAM_KTI_MMIOL_TAD                                       ProgramKtiMmiolTad;
  PROGRAM_KTI_MMIOH_TAD                                       ProgramKtiMmiohTad;
  PROGRAM_KTI_AGENT_UMA_CLUSTER                               ProgramKtiAgentUmaCluster;
  KTI_UNCORE_CHA_PER_CLUSTER                                  KtiUncoreChaPerCluster;
  PROGRAM_KTI_SNC_CONFIG                                      ProgramKtiSncConfig;
  PROGRAM_KTI_AGENT_2LM_ADDR_MASK                             ProgramKtiAgent2LMAddrMask;
  KTI_AGENT_SET_SNC_BASE_ADDR                                 KtiAgentSetSncBaseAddr;
  KTI_AGENT_SET_SNC_UPPER_BASE                                KtiAgentSetSncUpperBase;
  READ_LEP                                                    ReadLep;
  GET_LL_STATUS                                               GetLlStatus;
  GET_INIT_STATE                                              GetInitState;
  GET_KTILS                                                   GetKtiLs;
  GET_CURRENT_CRC_MODE                                        GetCurrentCrcMode;
  PROGRAM_MINIMUM_PATH                                        ProgramMinimumPath;
  PROGRAM_KTI_AGENT_ROUTE_TABLE                               ProgramKtiAgentRouteTable;
  PROGRAM_KTI_VGA_SAD_TARGETS                                 ProgramKtiVgaSadTargets;
  PROGRAM_KTI_IIO_STACKID_ENABLE                              ProgramKtiIioStackIdEnable;
  GET_KTI_RATE                                                GetKtiRate;
  CONFIG_COR_PHY_RESET_ERR_ON_KTIERRDIS0                      ConfiguCorPhyResetErrOnKtiErrDis0;
  DUMP_KTI_LLPMON_SNC_XPT_REGISTER                            DumpKtiLlPmonSncXptRegister;
  PROGRAM_KTI_PORT_CREDIT_SETTING                             ProgramKtiPortCreditSetting;
  SET_UPI_LINK_TO_L1                                          SetUpiLinkToL1;
  SET_C_INIT_BEGIN                                            SetCInitBegin;
  SET_CP_RESET                                                SetCpReset;
  GET_KTI_REUT_CAPABILITIES                                   GetKtiReutCapabilities;
  GET_KTI_REUT_CURRENT_SETTINGS                               GetKtiReutCurrentSettings;
  PROGRAM_LATE_ACTION_ON_CPU_LINK                             ProgramLateActionOnCpuLink;
  PROGRAM_KTI_REUT_PH_CTRL                                    ProgramKtiReutPhCtr1;
  PROGRAM_KTI_REUT_PH_WA                                      ProgramKtiReutPhWA;
  GET_KTI_SPEED                                               GetKtiSpeed;
  IS_LINK_TRAINED                                             IsLinkTrained;
  CHECK_LINK_IN_PHYSTATUS                                     CheckLinkInPhyStatus;
  PROGRAM_LOGPHY_FREQ_SELECTION                               ProgramLogPhyFreqSelecton;
  CHECK_PHY_CLK_CHANGE_DONE                                   CheckPhyClkChangeDone;
  CLEAR_PHY_CLK_CHANGE_OK_FLAG                                ClearPhyClkChangeOkFlag;
  FORCE_PHY_LAYER_READAPT                                     ForcePhyLayerReadapt;
  SET_KTI_L0P_EXIT_WORKAROUND                                 SetKtiL0pExitWorkaround;
  SET_PHYSICAL_DETERMINISM_CONTROL                            SetPhysicalDeterminismControl;
  SET_KTI_C_INIT                                              SetKtiCInit;
#if defined (IA32)
  KTI_POLL_FOR_LINK_RESET_CPL                                 KtiPollForLinkResetCpl;
  CLEAR_SLOW_MODE_WA_LINK_ERRORS                              ClearSlowModeWaLinkErrors;
  APPLY_SLOW_MODE_PHY_REPAIR                                  ApplySlowModePhyRepair;
  IS_LINK_TRAINED_IN_SLOW_MODE                                IsLinkTrainedInSlowMode;
  IS_OTHER_SIDE_OF_LINK_RECOVERED                             IsOtherSideOfLinkRecovered;
  DETECT_LINK_LAYER_PROBLEM                                   DetectLinkLayerProblem;
#endif //defined (IA32)
  GET_M3KTI_COUNT                                             GetM3KtiCount;
  PROGRAM_M3EFQPRIVC2                                         ProgramM3EFQPrivC2;
  MASK_REMOTE_VNA_CREDIT_OVERFLOW                             MaskRemoteVnaCreditOverflow;
  CLEAR_M3KTI_ERRORS                                          ClearM3KtiErrors;
  PROGRAM_LINK_LAYER_ON_M3KTI                                 ProgramLinkLayerOnM3Kti;
  CONVERT_M3KTI_ROUTING_TABLE                                 ConvertM3KTIRoutingTable;
  PROGRAM_M3KTI_ROUTE                                         ProgramM3KtiRoute;
  PROGRAM_M3KTI_MESH_STOP                                     ProgramM3KtiMeshStop;
  PROGRAM_M3KTI_RPQ_VNA_CREDITS                               ProgramM3KtiRpqVnaCredits;
  PROGRAM_M3KTI_NCB_NCS_CREDITS                               ProgramM3KtiNcbNcsCredits;
  PROGRAM_M3KTI_ROUTE_THRU_CREDITS                            ProgramM3KtiRouteThruCredits;
  SETUP_M3KTI_MISC                                            SetupM3KtiMisc;
  PROGRAM_M3KTI_MINI_SAD                                      ProgramM3ktiMiniSad;
  PROGRAM_M3KTI_SNOOP_FANOUT                                  ProgramM3KtiSnoopFanout;
  GET_M3KTI_TO_M2PCIE_BL_CREDITS                              GetM3KtiToM2PcieBlCredits;
  PROGRAM_M3KTI_TO_M2PCIE_BL_CREDITS                          ProgramM3KtiToM2PcieBlCredits;
  DUMP_M3KTI_SNC_XPT_REGISTER                                 DumpM3KtiSncXptRegister;
  M3KTI_EXTRA_STALL_WA                                        ProgramExtraStallOnM3Kti;
  M3KTI_DISABLE_ADBYPASS_WA                                   ProgramDisableAdByPassOnM3Kti;
  GET_KTI_SP_USE_BACK_CHANNEL                                 GetKtiSpUseBackChannel;
  SET_KTI_SP_USE_BACK_CHANNEL                                 SetKtiSpUseBackChannel;
  POLL_KTI_SP_USE_BACK_CHANNEL                                PollKtiSpUseBackChannel;
  SET_KTI_PH_TCC                                              SetKtiPhTcc;
  SET_KTI_PH_TCA                                              SetKtiPhTca;
  SET_KTI_TX_ADAPT_EN                                         SetKtiTxAdaptEn;
  SET_KTI_PH_TTPGC                                            SetKtiPhTtpgc;
  SET_KTI_PH_TTPCC                                            SetKtiPhTtpcc;
  SET_KTI_PH_TSEC_SL                                          SetKtiPhTsecSl;
  GET_KTI_PH_FOM_LANE                                         GetKtiPhFomLane;
  SET_KTI_PH_TCL_LANE                                         SetKtiPhTclLane;
  GET_KTI_PIPE_PH_RATE                                        GetKtiPipePhRate;
  EXTEND_KTI_L0P_EXIT_TIME                                    ExtendKtiL0pExitTime;
} KTI_IP_INTERFACE;

//
// This API supports multiple instances with the GUID identifying the instance
//
#define KTI_IP_INTERFACE_CPU_GUID \
  { 0xCC75DACB, 0xA115, 0x4FAD, {0xA7, 0xC6, 0x63, 0x0D, 0xC9, 0xA3, 0x62, 0xB7 }} // {CC75DACB-A115-4FAD-A7C6-630DC9A362B7}
extern EFI_GUID gKtiIpInterfaceCpuGuid;

#define KTI_IP_INTERFACE_PCIE_GEN4_GUID \
  { 0xe899d31e, 0xaff8, 0x49cb, {0x8c, 0x1d, 0x58, 0x94, 0x3b, 0x3a, 0x59, 0x72 }}// {E899D31E-AFF8-49CB-8C1D-58943B3A5972}
extern EFI_GUID gKtiIpInterfacePcieGen4Guid;

/**
 * Unlock and Lock definitions used to control PHY CSR registers lock\unlock mechanism
**/
#define     UNLOCK    FALSE
#define     LOCK      TRUE

/**
 * Locks or Unlocks PHY CSR block of registers.
 * dfxuniphy registers are locked for security reasons. Registers must be unlocked
 * before attempting to write CSR
 * This is single function to handle both Lock and Unlock request. The 3rd parameter
 * is boolean with the following logic:
 * TURE = Locks the PHY registers [Writes 1 to dfxuniphylck]
 * FALSE = Unlocks the PHY registers [Writes 0 to dfxuniphylck]
 *
 * @param[in] Socket - Socket Number to be programmed
 * @param[in] KTI Link - KTI Link to be programmed
 * @param[in] LockManager - TRUE = Lock; FALSE = Unlock
 **/

VOID
PhyCsrLockManager (
  IN    UINT8                     SocketIndex,
  IN    UINT8                     LinkIndex,
  IN    BOOLEAN                   LockManager
  );

#endif // _KTI_IP_INTERFACE_H_
