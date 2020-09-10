/** @file
  This is internal header file used by UncoreLib. Interfaces in this file are only used in this library.

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

#ifndef _UNCORE_LIB_INTERNAL_H_
#define _UNCORE_LIB_INTERNAL_H_

// (4GB - 256MB - 1)  MMIOL Limit change for ADP PCH
#if FixedPcdGetBool (PcdAdpPchSupport) == 1
#undef   MMIOL_LIMIT
#define  MMIOL_LIMIT   0xDFFFFFFF
#endif

/**
  Routine to program MSM BUS NUM and MCTP Eid information

  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval 0                - Successful
  @retval Other            - failure

**/
KTI_STATUS
UpdateCpuBusAndMctpEidOnMsm (
  KTI_HOST_INTERNAL_GLOBAL    *KtiInternalGlobal
  );

/**
  A routine for programming the DDRT QoS Cfg

  @param  KtiInternalGlobal -   KTIRC Internal Global data

  @retval KTI_SUCCESS       -   DDRT QoS Cfg successfully
          KTI_IGNORE        -   Ignore to program DDRT QoS
**/
KTI_STATUS
SetDDRTQosCfg (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Set SET_DMI_CFG_TYPE0 to one when stack zero of PBSP is configured as HOST bridge (DMI).

  DMI register can only be accessed after M2IOSF bank decoder is programmed by BIOS.

  @param[in] KtiInternalGlobal - Pointer to the KTI RC internal global structure

  @retval None

**/
VOID
SetDmiCfgType0 (
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

/**
  Program SNC registers on Sierra Peak

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocketId          - Socket

  @retval KTI_SUCCESS
**/
KTI_STATUS
ProgramSncRegistersOnSpk (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocketId
  );

/**
  Check whether SPK supported or not.
  Currently Simcs not support SPK.

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocketId          - Socket

  @retval TRUE             - SPK supported
  @retval FALSE            - SPK not supported
**/
BOOLEAN
IsSpkSupported (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocketId
  );

/**
  Routine to program the final route, VN selection and fanout setting for the given topology.
  Refer to KtiDisc.c file for more info on how the route settings are calculated.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval 0                - Successful
  @retval Other            - failure

**/
KTI_STATUS
ProgramSystemRoute (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Collect and clear error info.

  @param SocketData        - pointer to the socket data structure
  @param KtiInternalGlobal - pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
CollectAndClearErrors (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Program Snc configuration in CHA before memory is ready

  @param SocketData          - Pointer to socket specific data
  @param KtiInternalGlobal   - KTIRC Internal Global data

  @retval KTI_SUCCESS

**/
KTI_STATUS
ProgramSncConfigureInChaBeforeMemoryReady (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Routine to trigger MDFIS advanced training

  @param[in]  KtiInternalGlobal   Pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS
  @retval KTI_IGNORE

**/
KTI_STATUS
ConfigureMDFIAvanacedTraining (
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

/**
  Switch core and mesh ratios.

  @param[in]    CoreRatio       - The desired Core ratio
  @param[in]    MeshRatio       - The desired Mesh ratio

  @retval       KTI_SUCCESS
  @retval       KTI_IGNORE
**/
KTI_STATUS
SwitchCoreMeshRatio (
  IN  UINT8     CoreRatio,
  IN  UINT8     MeshRatio
  );

/**
  Get bitmask to determine which BGF pointer separation need to be programmed

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocketId          - Socket ID

  @retval Bitmask to indicate which BGF pointer separation need to be programmed
          It can be the combination of the following values:
             - PTRSEP_OVERRIDE_BGF_M3KTI_KTI
             - PTRSEP_OVERRIDE_BGF_M2PCIE
             - PTRSEP_OVERRIDE_BGF_M2M
**/
UINT8
BgfPtrOverrideSupported (
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  UINT8                     SocketId
  );

VOID
ProgramAEPQoSSpare (
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

VOID
Program_IRQ_Threshold (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

VOID
SetAtoS (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Dump current bus assignments

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param SocketData - Pointer to socket specific data
  @param PrintMissingSockets - TRUE/FALSE

  @retval None

**/
VOID
DumpCurrentBusInfo (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_SOCKET_DATA            *SocketData,
  BOOLEAN                    PrintMissingSockets
  );

/**
  Prime the KTIRC stackres data structure based on input option and system topology.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
PrimeHostStructureStack (
  IN KTI_SOCKET_DATA     *SocketData,
  OUT KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Setup system coherency.

  @param SocketData        - pointer to the socket data structure
  @param KtiInternalGlobal - pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
SetupSystemCoherency (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  KTI misc reg programming.

  @param SocketData        - pointer to the socket data structure
  @param KtiInternalGlobal - pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
SetupKtiMisc (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  SBSP programs bus numbers and mmcfg rules for self.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param Targets           - Enabled socket targets
  @param MmcfgTgtlist      - Common mmcfg target list
  @param SocId             - Socket to program

**/
KTI_STATUS
KtiFinalSbspBusCfg (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Targets,
  UINT32                     MmcfgTgtlist,
  UINT8                      SocId
  );

/**
  Routine to program IIO BUS NUM for the given socket

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
UpdateCpuBusOnIio (
  KTI_HOST_INTERNAL_GLOBAL    *KtiInternalGlobal,
  UINT8                       SocId
  );

/**
  Routine to program the final IOPORT SAD for the given socket

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramCpuIoEntries (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocId
  );

/**
  Routine to program the final IoApic SAD for given socket

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramCpuIoApicEntries (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocId
  );

/**
  Routine to program the final Mmmiol rules for given socket for SKX

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramCpuMmiolEntries (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocId
  );

/**
  Routine to program the final IO SAD setting for the given topology on 10nm project.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramCpuMmiohEntries (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocId
  );

/**
  Routine to program the final IO SAD setting for the given topology.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramCpuMmiohEntriesIio (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocId
  );

/**
  Routine to program the final IO SAD setting for the given topology.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramCpuMiscSadEntries (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocId
  );

/**
  Routine to program the IIO registers related to system address space.

  @param SocketData        - CPU socket info
  @param KtiInternalGlobal - KTIRC internal variables.
  @param SocId             - processor socket ID

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramSystemAddressMap (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocId
  );

/**
  Routine to program the TCSKTSAD register on Cstack of Legacy Socket.
  This configures MCTP Broadcast traffic routing.

  @param SocketData        - CPU socket info
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramMctpBroadcastSettings (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
  );
/**
  Routine to program the final Mmmiol rules for given socket

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0 - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramCpuMmiolEntriesIio (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocId
  );

/**
  Routine to program MCTP Segment ID Size field.
  This is required to enable MCTP traffic routing.
  Needs to be programmed on each present stack.

  @param SocId     - processor socket ID
  @param IioStack  - IIO stack ID
  @param MmcfgRule - contents of BUSID_GLOB_RULE

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramMctpSegmentIdSize (
  UINT8          SocId,
  UINT8          IioStack,
  UINT32         MmcfgRule
  );

/**
  Dump current bar assignments

  @param SocketData          - Pointer to socket specific data
  @param PrintMissingSockets - TRUE/FALSE

  @retval None

**/
VOID
DumpCurrentBarInfo (
  KTI_SOCKET_DATA         *SocketData,
  BOOLEAN                 PrintMissingSockets
  );

/**
  Setup OSB settings.

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param socket            - Socket ID to configure
  @param cha               - CHA index
  @param HaCoh             - HA/CHA coherency data
  @param Is4SFC            - If it is 4 socket full connection

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ConfigureOsbSetting (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  UINT8                    socket,
  UINT8                    cha,
  VOID                     *HaCoh,
  BOOLEAN                  Is4SFC
  );

/**
  Get RAS Type from CAP Register

  @param  SocId             - Soc ID of the CPU who's enabled Ras info is requested, 0...3

  @retval RasType

**/
UINT32
GetRasTypeByCapRegister (
  UINT8                      SocId
  );

/**
  Setup MMIOL for the cold-reset path.

  This routine configures MMIOL during the cold-reset path. It configures UBOX MMIO Base
  which is required for CSRs access in UPI and MRC.

  @param[in] SocketData           Pointer to socket specific data
  @param[in] KtiInternalGlobal    KTIRC Internal Global data

  @retval KTI_SUCCESS             Successful

**/
KTI_STATUS
EarlyIoSad (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Routine to program the final IO SAD setting for the given topology.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
SetupSystemIoSadEntries (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Program PMon discovery table registers on OOB-MSM

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocketId          - Socket

**/
KTI_STATUS
ProgramPmonDiscoveryTableOnOob (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocketId
  );

/**
  Program PMon control access

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocketId          - Socket

**/
KTI_STATUS
ProgramPmonControlAccess (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocketId
  );

/**

  Init Upi Affinity Status

  @param[in] KtiInternalGlobal    Pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS   Successful completion.
  @retval KTI_IGNORE

**/
KTI_STATUS
InitUpiAffinityStatus (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Program system coherency config register

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS - on successful completion

**/

EFI_STATUS
ProgramSystemCoherencyConfig(
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  This routine returns legacy stack IO Size (in number of SAD Targets).
  Note: Total SAD targets is 32.

  @param[in] KtiInternalGlobal    Pointer to the KTI RC internal global structure

  @retval  Number of SAD Targets for legacy stack IO Size.

**/
UINT8
GetLegacyStackIoSize (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Implement the si_wa, just after MMCFG are programmed for all the sockets.

  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval
**/
VOID
ProgramMiscSiWa (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );
#endif // _UNCORE_LIB_INTERNAL_H_
