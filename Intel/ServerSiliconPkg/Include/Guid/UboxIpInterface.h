/** @file
  UBOX generation specific silicon access interface

  This library abstracts silicon register differences between different versions
  of UBOX IP blocks.
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

#ifndef _UBOX_IP_INTERFACE_H_
#define _UBOX_IP_INTERFACE_H_

#include <Uefi.h>
#include <Base.h>

typedef enum {
  UboxLbWMcheckRacN0RegIndex       = 0,
  UboxLbWMcheckWacN0RegIndex       = 1,
  UboxLbWMcheckWacN1RegIndex       = 2,
  UboxLbWMcheckCpN0RegIndex        = 3,
  UboxLbWMcheckCpN1RegIndex        = 4
} UBOX_SAI_REG_ENUM;

#define QUAD2HEMI    1
#define QUAD2DISABLE 2
#define HEMI2DISABLE 3

//
// UBoxMisc Start
//
/**
  UBOX: Hide the requested devices on requested socket
  Note: The API is only implemented for SKX now

  @param[in] SocId          - CPU Socket Node number (Socket ID)
  @param[in] BoxInst        - Box Instance, 0 based
  @param[in] BusNum         - Bus Number for devices to devhide, 0 based.
  @param[in] DevHideBitmask - Bitmask of devices to hide

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxHideUboxDevice (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    BusNum,
  IN UINT32   DevHideBitmask
  );

//
// Function pointer definiton for UBoxHideUboxDevice()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_HIDE_UBOX_DEVICE) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    BusNum,
  IN UINT32   DevHideBitmask
  );

/**
  UBOX: configure MMCFG rule, such as MmcfgLengthUbox

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param MmCfgSize        - Mmcfg size

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureMmcfgRule (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   MmCfgSize
  );

//
// Function pointer definiton for UBoxConfigureMmcfgRule()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_CONFIGURE_MMCFG_RULE) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   MmCfgSize
  );

/**
  UBOX: Enable/Disable current CPUBUSNO Valid bit

  @param SocId              - CPU Socket Node number (Socket ID)
  @param BoxInst            - Box Instance, 0 based
  @param CpuBusNoProgrammed - 1: programmed; 0: not programmed

  @retval                   - EFI_UNSUPPORTED: the function not implemented
                            - EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxSetCpuBusNoValid (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    CpuBusNoProgrammed
  );

//
// Function pointer definiton for UBoxSetCpuBusNoValid()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_SET_CPU_BUS_NO_VALID) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    CpuBusNoProgrammed
  );

/**
  UBOX: Configure SBREG bar Page Table Entry register

  @param SocId              - CPU Socket Node number (Socket ID)
  @param BoxInst            - Box Instance, 0 based
  @param PortId             - Port ID for SBREG bar Page Table Entry register

  @retval                   - EFI_UNSUPPORTED: the function not implemented
                              EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureSbRegPteReg(
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   PortId
  );

//
// Function pointer definiton for UBoxConfigureSbRegPteReg()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_CONFIGURE_SB_REG_PTE_REG) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   PortId
  );

/**
  UBOX: Configure CPU BUS NO.

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureCpuBusNo (
  IN UINT8    SocId,
  IN UINT8    BoxInst
  );

//
// Function pointer definiton for UBoxConfigureCpuBusNo()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_CONFIGURE_CPU_BUS_NO) (
  IN UINT8    SocId,
  IN UINT8    BoxInst
  );

/**
  UBOX: Get SBREG bar Page Table Entry Registers' flat address

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param RegAddress       - Register value

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxGetSbRegPte0RegisterFlatAddr (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT UINTN*   RegAddress
  );

//
// Function pointer definiton for UBoxGetSbRegPte0RegisterFlatAddr()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_GET_SB_REG_PTE0_REGISTER_FLAT_ADDR) (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT UINTN*   RegAddress
  );

//
// UBoxMisc End
//

//
// UBoxScratchpad Start
//

/**
  UBOX: Check warm boot flag set or not

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param bWarmBootFlagSet - Check WarmBoot Flag Set in Scratchpad or not

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxCheckWarmBootFlagSetInScratchpad (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* bWarmBootFlagSet
  );

//
// Function pointer definiton for UBoxCheckWarmBootFlagSetInScratchpad()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_CHECK_WARM_BOOT_FLAG_SET_IN_SCRATCHPAD) (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* bWarmBootFlagSet
  );

/**
  UBOX: Check flag in scratch pad1 register to see whether Fpga Initialized

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param bFpgaInitialied  - Whether FPGA initialized or not

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxCheckFpgasInitialized (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* bFpgaInitialied
  );

//
// Function pointer definiton for UBoxCheckFpgasInitialized()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_CHECK_FPGAS_INITIALIZED) (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* bFpgaInitialied
  );

/**
  UBOX: Check warm boot flag set or not

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param Flag             - 0 or 1

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxSetResetAllowedFlagInScratchpad (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    Flag
  );

//
// Function pointer definiton for UBoxSetResetAllowedFlagInScratchpad()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_SET_RESET_ALLOWED_FLAG_IN_SCRATCHPAD) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    Flag
  );

/**
  UBOX: Update CPU List in Scratchpad register

  @param SocId            - CPU Socket Node number (Socket ID)
  @param AllCpus          - TRUE: update for all CPUs, FALSE: only update for socket "SocId"
  @param *SocketData      - Socket Data

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxUpdateCpuListInScratchpad (
  IN UINT8           SocId,
  IN BOOLEAN         AllCpus,
  IN KTI_SOCKET_DATA *SocketData
  );

//
// Function pointer definiton for UBoxUpdateCpuListInScratchpad()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_UPDATE_CPU_LIST_IN_SCRATCHPAD) (
  IN UINT8           SocId,
  IN BOOLEAN         AllCpus,
  IN KTI_SOCKET_DATA *SocketData
  );

/**
  UBOX: Store the following in Sticky scratchpad 7 bit [30] - Set SNC Mode Enable failure/recovery flag

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param Flag             - 0 or 1

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxSetSncModeFailRecoveryFlagInScratchpad (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    Flag
  );

//
// Function pointer definiton for UBoxSetSncModeFailRecoveryFlagInScratchpad()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_SET_SNC_MODE_FAIL_RECOVERY_FLAG_IN_SCRATCHPAD) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    Flag
  );

/**
  UBOX: Store the following in Sticky scratchpad 7 bit [20] - Set Prefetch Mode Enable failure/recovery flag

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param Flag             - 0 or 1

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

  **/
EFI_STATUS
EFIAPI
UBoxSetPrefetchModeFailRecoveryFlagInScratchpad (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    Flag
  );

//
// Function pointer definiton for UBoxSetPrefetchModeFailRecoveryFlagInScratchpad()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_SET_PREFETCH_MODE_FAIL_RECOVERY_FLAG_IN_SCRATCHPAD) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    Flag
  );

/**
  UBOX: Store the following in Sticky scratchpad 7 bit [22:21]
        Set UMA based clustering downgrade

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param Downgrade        - 0/1/2/3
                            0:default; 1: Quad-> Hemi 2: Quad->Disable 3: Hemi->Disable
  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxSetUmaBasedClusteringDowngradeInScratchpad (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    Downgrade
  );

//
// Function pointer definiton for UBoxSetUmaBasedClusteringDowngradeInScratchpad()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_SET_UMA_BASED_CLUSTERING_DOWNGRADE_FLAG_IN_SCRATCHPAD) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    Downgrade
  );

/**
  UBOX: Get UMA based clustering downgrade from
        Sticky scratchpad 7 bit [22:21]

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param Downgrade        - 0/1/2/3
                            0:default; 1: Quad-> Hemi 2: Quad->Disable 3: Hemi->Disable
  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxGetUmaBasedClusteringDowngradeInScratchpad (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT UINT8    *Downgrade
  );

//
// Function pointer definiton for UBoxGetUmaBasedClusteringDowngradeInScratchpad()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_GET_UMA_BASED_CLUSTERING_DOWNGRADE_FLAG_IN_SCRATCHPAD) (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT UINT8    *Downgrade
  );

/**
  UBOX: Update SBSP SocId in Scratchpad register

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param SbspSocId        - SBSP SocId

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxUpdateSbspSocIdInScratchpad (
  IN UINT8           SocId,
  IN UINT8           BoxInst,
  IN UINT8           SbspSocId
  );

//
// Function pointer definiton for UBoxUpdateSbspSocIdInScratchpad()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_UPDATE_SBSP_SOC_ID_IN_SCRATCHPAD) (
  IN UINT8           SocId,
  IN UINT8           BoxInst,
  IN UINT8           SbspSocId
  );

/**
  UBOX: Get Kti Topology in BIOS sticky scratchpad

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param KtiTop           - Return the Kti topology in UBox reg

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxGetKtiTopologyInScratchpad (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT UINT64*  KtiTop
  );

//
// Function pointer definiton for UBoxGetKtiTopologyInScratchpad()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_GET_KTI_TOPOLOGY_IN_SCRATCHPAD) (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT UINT64*  KtiTop
  );

/**
  UBOX: Set Kti Topology in BIOS sticky scratchpad

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param KtiTopology      - Kti Topology

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxSetKtiTopologyInScratchpad (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT64   KtiTopology
  );

//
// Function pointer definiton for UBoxSetKtiTopologyInScratchpad()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_SET_KTI_TOPOLOGY_IN_SCRATCHPAD) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT64   KtiTopology
  );

/**
  UBOX: Get Kti Topology status in BIOS sticky lockbypass register

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param TopoType         - Topology Type: 0b=DPW; 1b=AFC
  @param TopoTypeValid    - Topology Type valid or not: 0b=Invalid; 1b=Valid

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxGetKtiTopologyStatusInLockbypassReg (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT UINT8    *TopoType,
  OUT UINT8    *TopoTypeValid
  );

//
// Function pointer definiton for UBoxGetKtiTopologyStatusInLockbypassReg()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_GET_KTI_TOPOLOGY_STATUS_IN_LOCKBYPASS_REG) (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT UINT8    *TopoType,
  OUT UINT8    *TopoTypeValid
  );

/**
  UBOX: Set Kti Topology status in BIOS sticky lockbypass register

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param TopoType         - Topology Type: 0b=DPW; 1b=AFC
  @param TopoTypeValid    - Topology Type valid or not: 0b=Invalid; 1b=Valid

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxSetKtiTopologyStatusInLockbypassReg (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  IN  UINT8    TopoType,
  IN  UINT8    TopoTypeValid
  );

//
// Function pointer definiton for UBoxSetKtiTopologyStatusInLockbypassReg()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_SET_KTI_TOPOLOGY_STATUS_IN_LOCKBYPASS_REG) (
  IN  UINT8    SocId,
  IN  UINT8    TopoType,
  IN  UINT8    TopoTypeValid
  );

/**
  UBOX: Check SncModeFailRecovery flag set or not

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param SncModeFailReccovery - SNC mode fail recovery set or not

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxCheckSncModeFailRecoveryFlagSetInScratchpad (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* SncModeFailReccovery
  );

//
// Function pointer definiton for UBoxCheckSncModeFailRecoveryFlagSetInScratchpad()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_CHECK_SNC_MODE_FAIL_RECOVERY_FLAG_SET_IN_SCRATCHPAD) (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* SncModeFailReccovery
  );

/**
  UBOX: Check PrefetchModeFailRecovery flag set or not

  @param SocId                    - CPU Socket Node number (Socket ID)
  @param BoxInst                  - Box Instance, 0 based
  @param PrefetchModeFailRecovery - SNC mode fail recovery set or not

  @retval                         - EFI_UNSUPPORTED: the function not implemented
                                    EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxCheckPrefetchModeFailRecoveryFlagSetInScratchpad (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* PrefetchModeFailReccovery
  );

//
// Function pointer definiton for UBoxCheckPrefetchModeFailRecoveryFlagSetInScratchpad()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_CHECK_PREFETCH_MODE_FAIL_RECOVERY_FLAG_SET_IN_SCRATCHPAD) (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* PrefetchModeFailReccovery
  );

/**
  UBOX: Set RemoteSocketReleased flag in Scratchpad

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param Flag             - 0 or 1

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxSetRemoteSocketReleasedFlagInScratchpad (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    Flag
  );

//
// Function pointer definiton for UBoxSetRemoteSocketReleasedFlagInScratchpad()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_SET_REMOTE_SOCKET_RELEASED_FLAG_IN_SCRATCHPAD) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    Flag
  );

/**
  UBOX: Check CompletedColdResetFlow flag set or not

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param bCmpColdReset    - Complete cold reset or not

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxCheckCompletedColdResetFlowFlagSetInScratchpad (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* bCmpColdReset
  );

//
// Function pointer definiton for UBoxCheckCompletedColdResetFlowFlagSetInScratchpad()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_CHECK_COMPLETED_COLD_RESET_FLOW_FLAG_SET_IN_SCRATCHPAD) (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* bCmpColdReset
  );

/**
  UBOX: Check AepDimmPresent flag set or not

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @bAepDimmPresent        - AEP DIMM present or not

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxCheckAepDimmPresentFlagSetInScratchpad (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* bAepDimmPresent
  );

//
// Function pointer definiton for UBoxCheckAepDimmPresentFlagSetInScratchpad()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_CHECK_AEP_DIMM_PRESENT_FLAG_SET_IN_SCRATCHPAD) (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* bAepDimmPresent
  );

/**
  UBOX: Set Cold reset flag in Scratchpad

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param Flag             - 0 or 1

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxSetColdResetFlowFlagInScratchpad (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    Flag
  );

//
// Function pointer definiton for UBoxSetColdResetFlowFlagInScratchpad()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_SET_COLD_RESET_FLOW_FLAG_IN_SCRATCHPAD) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    Flag
  );

/**
  UBOX: Get Bios Non-Sticky ScratchPad Registers' value

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param RegIndex         - Non-Sticky ScratchPad Register index
  @param RegValue         - Register value

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxGetBiosNonStickyScratchPadRegisterValue (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  IN  UINT8    RegIndex,
  OUT UINT32*  RegValue
  );

//
// Function pointer definiton for UBoxGetBiosNonStickyScratchPadRegisterValue()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_GET_BIOS_NON_STICKY_SCRATCH_PAD_REGISTER_VALUE) (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  IN  UINT8    RegIndex,
  OUT UINT32*  RegValue
  );

/**
  UBOX: Set Bios Non-Sticky ScratchPad Registers' value

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param RegIndex         - Non-Sticky ScratchPad Register index
  @param RegValue         - Register value

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxSetBiosNonStickyScratchPadRegisterValue (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    RegIndex,
  IN UINT32   RegValue
  );

//
// Function pointer definiton for UBoxSetBiosNonStickyScratchPadRegisterValue()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_SET_BIOS_NON_STICKY_SCRATCH_PAD_REGISTER_VALUE) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    RegIndex,
  IN UINT32   RegValu
  );

/**
  UBOX: Get Bios Non-Sticky ScratchPad Registers' flat address

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param RegIndex         - Non-Sticky ScratchPad Register index
  @param RegVal           - Register value

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxGetBiosNonStickyScratchPadRegisterFlatAddr (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  IN  UINT8    RegIndex,
  OUT UINTN    *RegVal
  );

//
// Function pointer definiton for UBoxGetBiosNonStickyScratchPadRegisterFlatAddr()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_GET_BIOS_NON_STICKY_SCRATCH_PAD_REGISTER_FLAT_ADDR) (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  IN  UINT8    RegIndex,
  OUT UINTN    *RegVal
  );

/**

  Computes address of BIOSNONSTICKYSCRATCHPAD10_UBOX_MISC_REG

  @param[in] SocId      - CPU Socket Node number
  @param[in] BoxInst    - Box Instance, 0 based
  @param[in, out] Size  - Ptr to register size in bytes (may be updated if pseudo-offset)
  @param[out] Address   - Return the address

  @retval               - EFI_UNSUPPORTED: the function not implemented
                          EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxGetBiosNonStickyScratchPad10CsrAddress (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT UINT8    *Size,
  OUT UINT64   *Address
  );

//
// Function pointer definiton for UBoxGetBiosNonStickyScratchPad10CsrAddress()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_GET_BIOS_NON_STICKY_SCRATCH_PAD_10_CSR_ADDRESS) (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT UINT8    *Size,
  OUT UINT64   *Address
  );

/**
  UBOX: Get CPU Stepping in BIOS sticky scratchpad

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param LlcBist          - LLC BITS Result

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxGetLlcBistResultsInScratchpad (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT UINT32*  LlcBist
  );

//
// Function pointer definiton for UBoxGetLlcBistResultsInScratchpad()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_GET_LLC_BIST_RESULTS_IN_SCRATCHPAD) (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT UINT32*  LlcBist
  );

/**
  UBOX: Issue BOOT_GO command

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxIssueBootGoCommand (
  IN UINT8    SocId,
  IN UINT8    BoxInst
  );

//
// Function pointer definiton for UBoxIssueBootGoCommand()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_ISSUE_BOOT_GO_COMMAND) (
  IN UINT8    SocId,
  IN UINT8    BoxInst
  );

/**
  UBOX: Get CPU Stepping in BIOS sticky scratchpad

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param CpuStepping      - CPU Stepping in BIOS sticky scratchpad

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxGetCpuSteppingInScratchpad (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT UINT8*   CpuStepping
  );

//
// Function pointer definiton for UBoxGetCpuSteppingInScratchpad()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_GET_CPU_STEPPING_IN_SCRATCHPAD) (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT UINT8*   CpuStepping
  );

/**
  UBOX: Read CheckInBit to determine if AP has checked-in

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param bCheckInBitsSet  - CheckIn Bits set or not

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxCheckInBitsIsSetInScratchpad (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* bCheckInBitsSet
  );

//
// Function pointer definiton for UBoxCheckInBitsIsSetInScratchpad()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_CHECK_IN_BITS_IS_SET_IN_SCRATCHPAD) (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* bCheckInBitsSet
  );

/**
  UBOX: Read InHltState to determine if AP enter halt state

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param bCheckInBitsSet  - CheckIn Bits set or not

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxInHltStateIsSetInScratchpad (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* bSetInHltState
  );

//
// Function pointer definiton for UBoxCheckInBitsIsSetInScratchpad()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_IN_HLT_STATE_IS_SET_IN_SCRATCHPAD) (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* bSetInHltState
  );

/**
  UBOX: Clear the flag in scratchpad register which indicate
        PBSP in hlt state which is used for MDFS training

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxClearPbspInHltStateFlagInScratchpad (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst
  );

//
// Function pointer definiton for UBoxClearPbspInHltStateFlagInScratchpad()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_CLEAR_NBSP_IN_HLT_STATE_FLAG_IN_SCRATCHPAD) (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst
  );

/**
  UBOX: Check whether socket needs BootGo

  @param SocId            - CPU Socket Node number (Socket ID)
  @param RequireBootGo    - Check whether socket SocId needs BootGo

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxCheckSocketRequireBootGo (
  IN  UINT8    SocId,
  OUT BOOLEAN* RequireBootGo
  );
//
// Function pointer definiton for UBoxCheckSocketRequireBootGo()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_CHECK_SOCKET_REQUIRE_BOOT_GO) (
  IN  UINT8    SocId,
  OUT BOOLEAN* RequireBootGo
  );

/**
  UBOX: Store Link Fix Status

  @param SocId            - CPU Socket Node number (Socket ID)
  @param LinkFixStatus    - Link Fix Status

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxStoreLinkFixStatus (
  IN UINT8    SocId,
  IN UINT32   LinkFixStatus
  );

//
// Function pointer definiton for UBoxStoreLinkFixStatus()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_STORE_LINK_FIX_STATUS) (
  IN UINT8    SocId,
  IN UINT32   LinkFixStatus
  );

/**
  UBOX: Set link failure status

  @param SocId            - CPU Socket Node number (Socket ID)
  @param bLinkFailure     - TRUE: Link Failure
                            FALSE: Link success

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxSetLinkFailureStatus (
  IN UINT8    SocId,
  IN BOOLEAN  bLinkFailure
  );

//
// Function pointer definiton for UBoxSetLinkFailureStatus()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_SET_LINK_FAILURE_STATUS) (
  IN UINT8    SocId,
  IN BOOLEAN  bLinkFailure
  );

/**
  UBOX: Record slow mode WA reset times

  @param SocId            - CPU Socket Node number (Socket ID)

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxRecordSlowModeWARstTimes (
  IN UINT8    SocId
  );

//
// Function pointer definiton for UBoxRecordSlowModeWARstTimes()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_RECORD_SLOW_MODE_WA_RST_TIMES) (
  IN UINT8    SocId
  );

/**
  UBOX: Set/Clear boot flow enters cold reset flag in Sticky BIOS Scratchpad register

  @param SocId            - CPU Socket Node number (Socket ID)
  @param Operation        - 1: set the flag
                            0: clear the flag

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxSetBiosEnterColdRstFlowFlag (
  IN UINT8    SocId,
  IN UINT8    Operation
  );

//
// Function pointer definiton for UBoxSetBiosEnterColdRstFlowFlag()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_SET_BIOS_ENTER_COLD_RST_FLOW_FLAG) (
  IN UINT8    SocId,
  IN UINT8    Operation
  );

/**
  UBOX: Send the Dispatch PIPE and Data PIPE via BIOSNONSTICKYSCRATCHPAD13 & BIOSNONSTICKYSCRATCHPAD15
  BIOSNONSTICKYSCRATCHPAD13 --- Dispatch PIPE
  BIOSNONSTICKYSCRATCHPAD15 --- Data PIPE

  @param SocId              - CPU Socket Node number (Socket ID)
  @param BoxInst            - Box Instance, 0 based
  @param DispatchPipeData   - Dispatch info for NBSP
  @param DataPipeData       - Data info for NBSP

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

  **/
EFI_STATUS
EFIAPI
UBoxPipeDispatchData (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32    DispatchPipeData,
  IN UINT32    DataPipeData
  );

//
// Function pointer definiton for UBoxPipeDispatchData()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_PIPE_DISPATCH_DATA) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    DispatchPipeData,
  IN UINT8    DataPipeData
  );

//
// UBoxScratchpad End
//

//
// UBoxM2U Start
//
/**
  UBOX: Configure Routing table per Port

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param ChaPortFieldWidth    - Width of field used to CHA port
  @param LocalPort            - Local port used for routing
  @param DestSocId            - Dest CPU Socket Node number (Socket ID)

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureRouteTablePerPort (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    PortFieldWidth,
  IN UINT8    LocalPort,
  IN UINT8    DestSocId
  );

//
// Function pointer definiton for UBoxConfigureRouteTablePerPort()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_CONFIGURE_ROUTE_TABLE_PER_PORT) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    PortFieldWidth,
  IN UINT8    LocalPort,
  IN UINT8    DestSocId
  );

/**
  UBOX: Configure Routing table

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param M2PcieKtiRt          - M2Pcie Kti Routing table

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureRouteTable (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   M2PcieKtiRt
  );

//
// Function pointer definiton for UBoxConfigureRouteTable()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_CONFIGURE_ROUTE_TABLE) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   M2PcieKtiRt
  );
/**
  UBOX: Collect and clear error info.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param M2PcieErrlog         - M2Pcie Error log

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxCollectAndClearErrors (
  IN  UINT8                  SocId,
  IN  UINT8                  BoxInst,
  OUT KTI_CPU_M2PCIE_ERRLOG* M2PcieErrlog
  );

//
// Function pointer definiton for UBoxCollectAndClearErrors()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_COLLECT_AND_CLEAR_ERRORS) (
  IN  UINT8                  SocId,
  IN  UINT8                  BoxInst,
  OUT KTI_CPU_M2PCIE_ERRLOG* M2PcieErrlog
  );
/**
  Select the way R2PCIe routes NDR and DRS.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param SelNdrDrs            - The way R2PCIe routes NDR and DRS
                                packets destined to remote sockets
                                1: using the QPI_RT
                                0: using the Intel UPI Link_ID field

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxSelectR2PCIeRouteNdrAndDrs (
  IN UINT8                  SocId,
  IN UINT8                  BoxInst,
  IN UINT8                  SelNdrDrs
  );

//
// Function pointer definiton for UBoxSelectR2PCIeRouteNdrAndDrs()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_SELECT_R2_PCIE_ROUTE_NDR_AND_DRS) (
  IN UINT8                  SocId,
  IN UINT8                  BoxInst,
  IN UINT8                  SelNdrDrs
  );
//
// UBoxM2U Enad
//

//
// UBoxDfx Start
//
/**
  UBOX: Configure UBox MMIO Base and BAR.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param UboxMmioBase         - Ubox MMIO base for socket "SocId"

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureUboxMmio (
  IN UINT8                  SocId,
  IN UINT32                 UboxMmioBase
  );

//
// Function pointer definiton for UBoxConfigureUboxMmio()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_CONFIGURE_UBOX_MMIO) (
  IN UINT8                  SocId,
  IN UINT32                 UBox_MmioBase
  );
//
// UBoxDfx End
//

//
// UboxCfg Start
//
/**
  UBOX: Programm CPU Node ID.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param LocalSocId           - Node Id of the local Socket, 0xFFFFFFFF: no need to prgramm
  @param LockNodeId           - NodeId of the lock master, 0xFFFFFFFF: no need to prgramm
  @param LegacyNodeId         - NodeID of the legacy socket, 0xFFFFFFFF: no need to prgramm

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureCpuNodeId (
  IN UINT8                  SocId,
  IN UINT8                  BoxInst,
  IN UINT32                 LocalSocId,
  IN UINT32                 LockNodeId,
  IN UINT32                 LegacyNodeId
  );

//
// Function pointer definiton for UBoxConfigureCpuNodeId()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_CONFIGURE_CPU_NODE_ID) (
  IN UINT8                  SocId,
  IN UINT8                  BoxInst,
  IN UINT32                 LocalSocId,
  IN UINT32                 LockNodeId,
  IN UINT32                 LegacyNodeId
  );

/**
  UBOX: Programm CPU Node ID to indicate which CPUs are enabled.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param CpuList              - Bit mask to indicate which node_id is enabled

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureCpuEnabled (
  IN UINT8                  SocId,
  IN UINT8                  BoxInst,
  IN UINT32                 CpuList
  );

//
// Function pointer definiton for UBoxConfigureCpuEnabled()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_CONFIGURE_CPU_ENABLED) (
  IN UINT8                  SocId,
  IN UINT8                  BoxInst,
  IN UINT32                 CpuList
  );

/**
  UBOX: Programm CPU broadcast list.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param IntList              - 0xFFFFFFFF: no need to prgramm
                                Broadcast list for interrupts and PMReq in an
                                hot-add capable system
  @param StopStartList        - 0xFFFFFFFF: no need to prgramm
                                Broadcast list for StopReq/StartReq in a hot-add
                                capable system
  @param MiscList             - 0xFFFFFFFF: no need to prgramm
                                Broadcast list for WbInvdAck/InvdAck/EOI in a
                                hot-add capable system
  @param IntPrioList          - 0xFFFFFFFF: no need to prgramm
                                Broadcast list for IntPrioUpd in a hot-add
                                capable system
  @parm  BcastListRtn         - BCASTList

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureCpuBroadcastList (
  IN  UINT8                  SocId,
  IN  UINT8                  BoxInst,
  IN  UINT32                 IntList,
  IN  UINT32                 StopStartList,
  IN  UINT32                 MiscList,
  IN  UINT32                 IntPrioList,
  OUT UINT32*                BcastListRtn
  );

//
// Function pointer definiton for UBoxConfigureCpuBroadcastList()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_CONFIGURE_CPU_BROADCAST_LIST) (
  IN  UINT8                  SocId,
  IN  UINT8                  BoxInst,
  IN  UINT32                 IntList,
  IN  UINT32                 StopStartList,
  IN  UINT32                 MiscList,
  IN  UINT32                 IntPrioList,
  OUT UINT32*                BcastList
  );

/**
  UBOX: Configure IPI target list.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param Type                 - Programm type: 0: clear specified target
                                               1: set specified target
                                               2: set all target list
  @param PendingSocId         - Pending Soc ID
  @param BcastListRtn         - return value of register BCASTLIST_UBOX_CFG_REG

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureCpuIpiTargetList (
  IN  UINT8                  SocId,
  IN  UINT8                  BoxInst,
  IN  UINT8                  Type,
  IN  UINT8                  PendingSocId,
  OUT UINT32*                BcastListRtn
  );

//
// Function pointer definiton for UBoxConfigureCpuIpiTargetList()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_CONFIGURE_CPU_IPI_TARGET_LIST) (
  IN  UINT8                  SocId,
  IN  UINT8                  BoxInst,
  IN  UINT8                  Type,
  IN  UINT8                  PendingSocId,
  OUT UINT32*                BrdCstList
  );

/**
  UBOX: Programm Lock Control Register.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param SplitLockEnable      - 1: Enable; 0: Disable

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureLockControlRegister (
  IN UINT8                  SocId,
  IN UINT8                  BoxInst,
  IN UINT8                  SplitLockEnable
  );

//
// Function pointer definiton for UBoxConfigureLockControlRegister()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_CONFIGURE_LOCK_CONTROL_REGISTER) (
  IN UINT8                  SocId,
  IN UINT8                  BoxInst,
  IN UINT8                  SplitLockEnable
  );

/**
  UBOX: Ubox per-error enable mask. If masked, a given Ubox error will not be signaled via SMI and will not be logged. .

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureUboxErrorCtrl (
  IN UINT8                  SocId,
  IN UINT8                  BoxInst
  );

//
// Function pointer definiton for UBoxConfigureUboxErrorCtrl()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_CONFIGURE_UBOX_ERROR_CTRL) (
  IN UINT8                  SocId,
  IN UINT8                  BoxInst
  );
/**
  UBOX: Programm group ID and NodeId mapping.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param GidNidMapping        - Programm group ID and NodeId mapping

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureGidNidMapping (
  IN UINT8                  SocId,
  IN UINT8                  BoxInst,
  IN UINT32                 GidNidMapping
  );

//
// Function pointer definiton for UBoxConfigureGidNidMapping()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_CONFIGURE_GID_NID_MAPPING) (
  IN UINT8                  SocId,
  IN UINT8                  BoxInst,
  IN UINT32                 GidNidMapping
  );

/**
  UBOX: Programm CPU Node Local ID.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param LocalSocId           - Node Id of the local Socket
  @param RegMask              - Specify programm bits

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureCpuLocalId (
  IN UINT8                  SocId,
  IN UINT8                  BoxInst,
  IN UINT8                  LocalSocId,
  IN UINT32                 RegMask
  );

//
// Function pointer definiton for UBoxConfigureCpuLocalId()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_CONFIGURE_CPU_LOCAL_ID) (
  IN UINT8                  SocId,
  IN UINT8                  BoxInst,
  IN UINT8                  LocalSocId,
  IN UINT32                 RegMask
  );

/**

  UBoxGetSetSaiRegisters.

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
UBoxGetSetSaiRegisters (
  UINT8                      SocId,
  UINT8                      BoxInst,
  IN UBOX_SAI_REG_ENUM       SaiRegIndex,
  IN BOOLEAN                 Get,
  IN OUT UINT32              *Value
  );

//
// Function pointer definiton for UBoxGetSetSaiRegisters()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_GET_SET_SAI_REGISTERS) (
  UINT8                      SocId,
  UINT8                      BoxInst,
  IN UBOX_SAI_REG_ENUM       SaiRegIndex,
  IN BOOLEAN                 Get,
  IN OUT UINT32              *Value
  );

/**
  UBOX:  Set the initial credits for Ubox

  @param SocId       - Socket ID
  @param BoxInst     - Box Instance, 0 based
  @param BlVn0Credit - BlVn0 Credit value

  @retval          - NONE

**/
EFI_STATUS
EFIAPI
UboxSetUbox2M2UInitCrd (
  IN UINT8           SocId,
  IN UINT8           BoxInst,
  IN UINT8           BlVn0Credit
  );
//
// Function pointer definiton for UboxSetUbox2M2UInitCrd()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_SET_UBOX2M2U_INIT_CRD) (
  IN UINT8           SocId,
  IN UINT8           BoxInst,
  IN UINT8           BlVn0Credit
  );
/**
  UBOX: Platform QoS Memory BW Enforcement Delay & Time Window setting

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfullye

**/
EFI_STATUS
EFIAPI
UBoxSetMemoryBWEnforcement (
  IN UINT8                  SocId,
  IN UINT8                  BoxInst
  );

//
// Function pointer definiton for SetMemoryBWEnforcement()
//
typedef
EFI_STATUS
(EFIAPI *UBOX_SET_MEMORY_BW_ENFORCEMENT) (
  UINT8                      SocId,
  UINT8                      BoxInst
  );

/**
  Get the "completion delay reset count" value.

  @param[in]  Socket  The socket to read the count from.
  @param[out] Count   The "completion delay reset count" value.

  @retval   EFI_SUCCESS       The "completion delay reset count" value was successfully retrieved.
            EFI_UNSUPPORTED   Getting the "completion delay reset count" is not supported.
**/
EFI_STATUS
EFIAPI
UBoxGetCompletionDelayResetCount (
  IN  UINT8   Socket,
  OUT UINT32  *Count
  );
// Note: UBoxGetCompletionDelayResetCount does not need to be made available through UBOX_IP_INTERFACE.

/**
  Set the "completion delay reset count" value.

  @param[in]  Socket  The socket to write the count to.
  @param[in]  Count   The "completion delay reset count" value.

  @retval   EFI_ACCESS_DENIED   The "completion delay reset count" value could not be set (such as due to the value
                                being greater than what is supported).
            EFI_SUCCESS         The "completion delay reset count" value was successfully set.
            EFI_UNSUPPORTED     Setting the "completion delay reset count" is not supported.
**/
EFI_STATUS
EFIAPI
UBoxSetCompletionDelayResetCount (
  IN  UINT8   Socket,
  IN  UINT32  Count
  );
// Note: UBoxSetCompletionDelayResetCount does not need to be made available through UBOX_IP_INTERFACE.

typedef struct {
  //
  // Function pointers
  //
  UBOX_HIDE_UBOX_DEVICE                                           HideUboxDevice;
  UBOX_CONFIGURE_MMCFG_RULE                                       ConfigureMmcfgRule;
  UBOX_SET_CPU_BUS_NO_VALID                                       SetCpuBusNoValid;
  UBOX_CONFIGURE_SB_REG_PTE_REG                                   ConfigureSbRegPteReg;
  UBOX_CONFIGURE_CPU_BUS_NO                                       ConfigureCpuBusNo;
  UBOX_GET_SB_REG_PTE0_REGISTER_FLAT_ADDR                         GetSbRegPte0RegisterFlatAddr;
  UBOX_CHECK_WARM_BOOT_FLAG_SET_IN_SCRATCHPAD                     CheckWarmBootFlagSetInScratchpad;
  UBOX_CHECK_FPGAS_INITIALIZED                                    CheckFpgasInitialized;
  UBOX_SET_RESET_ALLOWED_FLAG_IN_SCRATCHPAD                       SetResetAllowedFlagInScratchpad;
  UBOX_UPDATE_CPU_LIST_IN_SCRATCHPAD                              UpdateCpuListInScratchpad;
  UBOX_SET_SNC_MODE_FAIL_RECOVERY_FLAG_IN_SCRATCHPAD              SetSncModeFailRecoveryFlagInScratchpad;
  UBOX_UPDATE_SBSP_SOC_ID_IN_SCRATCHPAD                           UpdateSbspSocIdInScratchpad;
  UBOX_GET_KTI_TOPOLOGY_IN_SCRATCHPAD                             GetKtiTopologyInScratchpad;
  UBOX_SET_KTI_TOPOLOGY_IN_SCRATCHPAD                             SetKtiTopologyInScratchpad;
  UBOX_CHECK_SNC_MODE_FAIL_RECOVERY_FLAG_SET_IN_SCRATCHPAD        CheckSncModeFailRecoveryFlagSetInScratchpad;
  UBOX_SET_REMOTE_SOCKET_RELEASED_FLAG_IN_SCRATCHPAD              SetRemoteSocketReleasedFlagInScratchpad;
  UBOX_CHECK_COMPLETED_COLD_RESET_FLOW_FLAG_SET_IN_SCRATCHPAD     CheckCompletedColdResetFlowFlagSetInScratchpad;
  UBOX_CHECK_AEP_DIMM_PRESENT_FLAG_SET_IN_SCRATCHPAD              CheckAepDimmPresentFlagSetInScratchpad;
  UBOX_SET_COLD_RESET_FLOW_FLAG_IN_SCRATCHPAD                     SetColdResetFlowFlagInScratchpad;
  UBOX_GET_BIOS_NON_STICKY_SCRATCH_PAD_REGISTER_VALUE             GetBiosNonStickyScratchPadRegisterValue;
  UBOX_SET_BIOS_NON_STICKY_SCRATCH_PAD_REGISTER_VALUE             SetBiosNonStickyScratchPadRegisterValue;
  UBOX_GET_BIOS_NON_STICKY_SCRATCH_PAD_REGISTER_FLAT_ADDR         GetBiosNonStickyScratchPadRegisterFlatAddr;
  UBOX_GET_BIOS_NON_STICKY_SCRATCH_PAD_10_CSR_ADDRESS             GetBiosNonStickyScratchPad10CsrAddress;
  UBOX_GET_LLC_BIST_RESULTS_IN_SCRATCHPAD                         GetLlcBistResultsInScratchpad;
  UBOX_ISSUE_BOOT_GO_COMMAND                                      IssueBootGoCommand;
  UBOX_GET_CPU_STEPPING_IN_SCRATCHPAD                             GetCpuSteppingInScratchpad;
  UBOX_CHECK_IN_BITS_IS_SET_IN_SCRATCHPAD                         CheckInBitsIsSetInScratchpad;
  UBOX_CHECK_SOCKET_REQUIRE_BOOT_GO                               CheckSocketRequireBootGo;
  UBOX_STORE_LINK_FIX_STATUS                                      StoreLinkFixStatus;
  UBOX_SET_LINK_FAILURE_STATUS                                    SetLinkFailureStatus;
  UBOX_RECORD_SLOW_MODE_WA_RST_TIMES                              RecordSlowModeWARstTimes;
  UBOX_SET_BIOS_ENTER_COLD_RST_FLOW_FLAG                          SetBiosEnterColdRstFlowFlag;
  UBOX_CONFIGURE_ROUTE_TABLE_PER_PORT                             ConfigureRouteTablePerPort;
  UBOX_CONFIGURE_ROUTE_TABLE                                      ConfigureRouteTable;
  UBOX_COLLECT_AND_CLEAR_ERRORS                                   CollectAndClearErrors;
  UBOX_SELECT_R2_PCIE_ROUTE_NDR_AND_DRS                           SelectR2PCIeRouteNdrAndDrs;
  UBOX_CONFIGURE_UBOX_MMIO                                        ConfigureUboxMmio;
  UBOX_CONFIGURE_CPU_NODE_ID                                      ConfigureCpuNodeId;
  UBOX_CONFIGURE_CPU_ENABLED                                      ConfigureCpuEnabled;
  UBOX_CONFIGURE_CPU_BROADCAST_LIST                               ConfigureCpuBroadcastList;
  UBOX_CONFIGURE_CPU_IPI_TARGET_LIST                              ConfigureCpuIpiTargetList;
  UBOX_CONFIGURE_LOCK_CONTROL_REGISTER                            ConfigureLockControlRegister;
  UBOX_CONFIGURE_UBOX_ERROR_CTRL                                  ConfigureUboxErrorCtrl;
  UBOX_CONFIGURE_GID_NID_MAPPING                                  ConfigureGidNidMapping;
  UBOX_CONFIGURE_CPU_LOCAL_ID                                     ConfigureCpuLocalId;
  UBOX_GET_SET_SAI_REGISTERS                                      GetSetSaiRegisters;
  UBOX_SET_MEMORY_BW_ENFORCEMENT                                  SetMemoryBWEnforcement;
  UBOX_GET_KTI_TOPOLOGY_STATUS_IN_LOCKBYPASS_REG                  UBoxGetKtiTopologyStatusInLockbypassReg;
  UBOX_SET_KTI_TOPOLOGY_STATUS_IN_LOCKBYPASS_REG                  UBoxSetKtiTopologyStatusInLockbypassReg;
  UBOX_SET_PREFETCH_MODE_FAIL_RECOVERY_FLAG_IN_SCRATCHPAD         UBoxSetPrefetchModeFailRecoveryFlagInScratchpad;
  UBOX_CHECK_PREFETCH_MODE_FAIL_RECOVERY_FLAG_SET_IN_SCRATCHPAD   UBoxCheckPrefetchModeFailRecoveryFlagSetInScratchpad;
} UBOX_IP_INTERFACE;

//
// This API supports multiple instances with the GUID identifying the instance
//
#define UBOX_IP_INTERFACE_CLX_CPU_GUID \
  { 0xfedb4f83, 0x3be3, 0x44b2, {0x8d, 0x29, 0x72, 0xad, 0xd0, 0xb6, 0x3c, 0x18 }} // {FEDB4F83-3BE3-44B2-8D29-72ADD0B63C18}
extern EFI_GUID gUboxIpInterfaceCpuGuid;

#define UBOX_IP_INTERFACE_CLX_PCIE_GEN4_GUID \
  { 0x32c49b21, 0xd303, 0x4cfe, {0xb7, 0xf5, 0xa1, 0x5f, 0x8f, 0x65, 0xff, 0x6e }} // {32C49B21-D303-4CFE-B7F5-A15F8F65FF6E}
extern EFI_GUID gUboxIpInterfacePcieGen4Guid;

#endif // _UBOX_IP_INTERFACE_H_
