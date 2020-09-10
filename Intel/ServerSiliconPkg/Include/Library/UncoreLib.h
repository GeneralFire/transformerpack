/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#ifndef _UNCORE_LIB_H_
#define _UNCORE_LIB_H_

#include <ReferenceCodeDataTypes.h>
#include "Chip/Include/CpuPciAccess.h"
#include <Upi/KtiHost.h>

VOID
KtiInit (
  VOID
  );

KTI_STATUS
GetChaCountAndList (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  UINT8                    SocId
  );

KTI_STATUS
EFIAPI
SendMailBoxCmdToPcode (
  UINT8     Cpu,
  UINT32    Command,
  UINT32    Data
  );

VOID
UncoreEnableMeshMode (
  VOID
  );

KTI_STATUS
KtiMain (
  VOID
  );

KTI_STATUS
AllocateIioResources (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
  );

VOID
KtiFixedDelay (
  UINT32          MicroSecs
  );

KTI_STATUS
ParseCpuLep (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocId
  );

UINT32
KtiReadPciCfg (
  UINT8     SocId,
  UINT8     BoxInst,
  UINT32    RegOffset
  );

VOID
KtiWritePciCfg (
  UINT8     SocId,
  UINT8     BoxInst,
  UINT32    RegOffset,
  UINT32    data
  );

VOID
KtiCheckPoint (
  UINT8                      SocId,
  UINT8                      SocType,
  UINT8                      KtiLink,
  UINT8                      MajorCode,
  UINT8                      MinorCode
  );

VOID
KtiAssert (
  UINT8                      MajorCode,
  UINT8                      MinorCode
  );

KTI_STATUS
ProgramMeshCredits (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

KTI_STATUS
ProgramMeshCredits1S (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

KTI_STATUS
ProgramMeshCreditsCommon (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

KTI_STATUS
ProgramM2PcieCreditSetting (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  UINT8                     Soc
  );

VOID
ProgramM2iosfP2pCredits (
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
  );

KTI_STATUS
ProgramChaCreditSetting (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  UINT8                     Soc
  );

VOID
MCAErrWa (
  BOOLEAN          Mask
  );

/**
  Count the number of active links

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID

  @retval number of active links

**/
UINT8
CountActiveKtiLink (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  UINT8                     Soc
  );

KTI_STATUS
GetSocketWays (
  UINT8                      SocId,
  UINT8                      *SocketWays
  );

BOOLEAN
WasSocketFpgaInitialized (
  UINT8                      SocId,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Returns the total Iios enabled for a given CPU socket.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param SocId             - Soc ID of the CPU who's enabled IIOs info is requested, 0,1,2,3,etc
  @param IIOCount          - Total IIOs in given socket

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
GetIioCount (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocId,
  OUT UINT8                     *IioCount
  );

KTI_STATUS
GetM2PcieCount (
  UINT8                      SocId,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Get system RAS type and save this info into KTI internal global buffer and KTI output buffer.

  @param SocId             - Soc ID of the CPU who's enabled M2Pcie info is requested, 0...3
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval 0 - Successful
  @retval 1 - Failure

**/
KTI_STATUS
GetRasType (
  UINT8                      SocId,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Update struct with CPU Segment Type

  @param KtiInternalGlobal - KTIRC internal variables.
  @param SocId             - Soc ID of the CPU who's enabled Ras info is requested, 0...3

  @retval 0 - Successful

**/
KTI_STATUS
EFIAPI
GetCpuSegmentType (
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN UINT8                     SocId
  );

KTI_STATUS
GetM2MemCount (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocId,
  UINT8                      *M2MemCount
  );

/**
  Get max Mesh2Mem supported

  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval Max M2M supported
**/
UINT8
GetMaxM2mSupported (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

UINT64
GetTopologyBitmap (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
);

KTI_STATUS
CalculateChaToM2mCredit (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc,
  UINT8                      NumOfCha,
  UINT32                     *Data32_AD,
  UINT32                     *Data32_BL
  );

KTI_STATUS
ReCalAndProgramChaToM2mCreditWhenSncEn (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo,
  UINT8                      SocId
  );

KTI_STATUS
GetLegacyPchTargetSktId (
  UINT8                      SocId,
  UINT8                      *LegacyPchSktId
  );

KTI_STATUS
ParseFpgaLep (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocId
  );

KTI_STATUS
UpdateStructureForHotPlug (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  UINT8                     TmpTotCpu
  );

KTI_STATUS
CalculateBusResourceRatio (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  UINT8                     *BusSize,
  UINT8                     TmpTotCpu
  );

KTI_STATUS
CalculateBusResources (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  UINT8                     *BusSize,
  UINT8                     TmpTotCpu
  );

/**
  Programs hash mask to 4G as default if 2LM Mode

  This routine programs the SNC mode.

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param Socket            - Socket
  @param Enable            - 2LM enabled or disabled

  @retval None

**/
VOID
Program2LMHashRegister (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Socket,
  BOOLEAN                    Enable
  );

KTI_STATUS
ProgramM2mCreditSetting (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc
  );

VOID
ProgramM2mTransgressCredits (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo
  );

KTI_STATUS
ProgramChaToHbmM2mCredit(
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo,
  UINT8                      SocId
  );

VOID
ProgramM2mCreditBetweenHbmAndDdr (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocId,
  UINT8                      AdCreditsHbmM2mToDdrM2m,
  UINT8                      BlCreditsHbmM2mToDdrM2m,
  UINT8                      BlCreditsDdrM2mToHbmM2m
  );

VOID
ProgramM2mPrefetchCredit (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo
  );

KTI_STATUS
ProgramNonSncChaToM2mCredit (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo
  );

KTI_STATUS
ProgramChaToM2mCreditWithoutCluster (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo,
  UINT8                      SocId
  );

KTI_STATUS
ProgramChaToM2mCreditWithCluster (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo,
  UINT8                      SocId
  );

KTI_STATUS
CalculateIoResources (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  UINT8                     TmpTotCpu
  );

KTI_STATUS
CalculateIoApicResources (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

KTI_STATUS
CalculateMmiolResources (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  UINT8                     TmpTotCpu
  );

KTI_STATUS
CalculateMmiohResources (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  UINT8                     TmpTotCpu
  );

KTI_STATUS
CalculateMmiolGranularity (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

KTI_STATUS
CalculateMmiohGranularity (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

VOID
PrintResourceMapHeader (
  VOID
  );

VOID
PrintCpuResourceMapDetails (
  UINT8            Socket,
  KTI_SOCKET_DATA  *SocketData,
  KTI_CPU_RESOURCE *CpuRes
  );

KTI_STATUS
CheckOemSegmentResources (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_CPU_RESOURCE         CpuRes[MAX_SOCKET]
  );

KTI_STATUS
CheckOemSocketResources (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_CPU_RESOURCE         CpuRes[MAX_SOCKET]
  );

KTI_STATUS
WalkOemSocketResources (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_CPU_RESOURCE         CpuRes[MAX_SOCKET]
  );

KTI_STATUS
CheckOemStackResources (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_CPU_RESOURCE         CpuRes[MAX_SOCKET]
  );

KTI_STATUS
WalkOemStackResources (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_CPU_RESOURCE         CpuRes[MAX_SOCKET]
  );

KTI_STATUS
CheckForOemResourceUpdate (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

VOID
DataDumpKtiResourceAllocation (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

KTI_STATUS
SanityCheckDiscoveredTopology (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

KTI_STATUS
SanityCheckLep (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_CPU_SOCKET_DATA        *CpuData
  );

KTI_STATUS
DegradeTo1S (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

KTI_STATUS
PreProcessFeatureVsTopology (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

VOID
TopologyInfoDump (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN KTI_CPU_SOCKET_DATA        *Cpu,
  IN TOPOLOGY_TREE_NODE         (*CpuTree)[MAX_TREE_NODES],
  IN TOPOLOGY_TREE_NODE         (*AlternateCpuTree)[MAX_TREE_NODES],
  IN BOOLEAN                    SiPort
  );

VOID
LepDump (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_CPU_SOCKET_DATA        *CpuData
  );

UINT8
TranslateToSiPort (
  UINT8                   GenPort
  );

BOOLEAN
CheckCpuConnectedToLegCpu (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocId
  );

BOOLEAN
MoreThanOneLinkExists (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_CPU_SOCKET_DATA        *CpuData,
  TOPOLOGY_TREE_NODE         *Parent,
  TOPOLOGY_TREE_NODE         *Child
  );

KTI_STATUS
RemoveDualLink (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_SOCKET_DATA            *SocketData
  );

KTI_STATUS
GetMultiLinkFromLEP (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_CPU_SOCKET_DATA        *CpuData,
  TOPOLOGY_TREE_NODE         *Node1,
  TOPOLOGY_TREE_NODE         *Node2,
  UINT8                      *Ports,
  UINT8                      MaxLinks
  );

UINT8
GetChildLocalPortFromLEP (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_CPU_SOCKET_DATA        *CpuData,
  TOPOLOGY_TREE_NODE         *Parent,
  TOPOLOGY_TREE_NODE         *Child
  );

UINT8
GetChildPeerPortFromLEP (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_CPU_SOCKET_DATA        *CpuData,
  TOPOLOGY_TREE_NODE         *Parent,
  TOPOLOGY_TREE_NODE         *Child
  );

KTI_STATUS
GetParentSocket (
  TOPOLOGY_TREE_NODE     *TopologyTree,
  TOPOLOGY_TREE_NODE     *Child,
  TOPOLOGY_TREE_NODE     *Parent
  );

UINT8
GetParentIndex (
  KTI_CPU_SOCKET_DATA      *CpuData,
  TOPOLOGY_TREE_NODE       *Tree,
  TOPOLOGY_TREE_NODE       *Child
  );

UINT8
GetChildHopLength (
  TOPOLOGY_TREE_NODE   *Tree,
  UINT8                SocId,
  UINT8                SocType
  );

BOOLEAN
NodeFoundInTopologyTree (
  IN     KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN OUT TOPOLOGY_TREE_NODE         *Tree,
  IN     TOPOLOGY_TREE_NODE         *AlternateTree,
  IN OUT TOPOLOGY_TREE_NODE         *Parent,
  IN     TOPOLOGY_TREE_NODE         *SearchNode,
  OUT    BOOLEAN                    *FoundInAlternateTree
  );

BOOLEAN
NodeFoundInMinPathTree (
  TOPOLOGY_TREE_NODE      *Tree,
  TOPOLOGY_TREE_NODE      *SearchNode
  );

KTI_STATUS
AddNodeToTopologyTree (
  TOPOLOGY_TREE_NODE       *Tree,
  TOPOLOGY_TREE_NODE       *Node
  );

KTI_STATUS
AddNodeToRingTree (
  TOPOLOGY_TREE_NODE      *Tree,
  TOPOLOGY_TREE_NODE      *Node
  );

UINT8
CountImmediateChildren (
  TOPOLOGY_TREE_NODE       *Tree,
  TOPOLOGY_TREE_NODE       *Parent
  );

BOOLEAN
ShorterPathExists (
  TOPOLOGY_TREE_NODE      *Tree,
  TOPOLOGY_TREE_NODE      *SearchNode
  );

BOOLEAN
CompareRing (
  UINT8                *Ring1,
  UINT8                *Ring2
  );

VOID
KtiLogWarning (
  UINT8                      SocId,
  UINT8                      SocType,
  UINT8                      KtiLink,
  UINT8                      WarnCode
  );

VOID
KtiLogTopologyChange (
  UINT64                  TopologyBefore,
  UINT64                  TopologyAfter,
  TOPOLOGY_DEGRADE_REASON Reason
  );

#define KTILIB_IAR_IOVC  20
#define KTILIB_IAR_PDQ   62
#define KTILIB_IAR_BCAST 63

UINT8
KtiReadIar (
  UINT8                 SocId,
  UINT8                 BoxInst,
  UINT8                 UnitId,
  UINT32                RegOffset
  );

VOID
KtiWriteIar (
  UINT8                 SocId,
  UINT8                 BoxInst,
  UINT8                 UnitId,
  UINT32                RegOffset,
  UINT8                 data
  );

VOID
KtiWriteLbc (
  UINT8                 SocId,
  UINT8                 Port,
  UINT8                 LbcType,
  UINT32                LaneMask,
  UINT32                LoadSel,
  UINT32                Data
  );

KTI_STATUS
KtiEvAutoRecipe (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Socket,
  UINT8                      LinkIndex
  );

KTI_STATUS
HideMcpDevices (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_SOCKET_DATA            *SocketData
  );

KTI_STATUS
KtiDetermineIfRequireNonInterleavedDecoder (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_CPU_RESOURCE         CpuRes[MAX_SOCKET]
  );

BOOLEAN
IsXGreaterThanYUINT64 (
  UINT64_STRUCT X,
  UINT64_STRUCT Y
  );

BOOLEAN
IsXEqualYUINT64 (
  UINT64_STRUCT X,
  UINT64_STRUCT Y
  );

VOID
GetNeighborSocs (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc,
  UINT8                      *NeighborSoc1,
  UINT8                      *NeighborSoc2,
  UINT8                      *RemoteSoc
  );

VOID
KtiOutPort32 (
  UINT16   ioAddress,
  UINT32   data
  );

UINT32
KtiInPort32 (
  UINT16   ioAddress
  );

UINT8
EfiDisableInterrupts_1 (
  VOID
  );

UINT8
EfiHalt_1 (
  VOID
  );

UINT32
GetPhysicalBusNumber (
  UINT8    Soc,
  UINT32   BusAddr
  );

UINT8
KtiGetSiliconRevision (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8     Socket,
  UINT8     CpuStep,
  UINT8     Cf8Access
  );

KTI_STATUS
GetIioAndM2PcieCount (
  UINT8                      SocId,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

VOID
DetectMaxAddress (
  IN     KTI_SOCKET_DATA            *SocketData,
  IN OUT KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

KTI_STATUS
ProgramMiscBeforeWarmReset (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
  );

KTI_STATUS
HandleDegraded4S (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_DISC_DATA            *DiscEngData
  );

KTI_STATUS
CpxHandleDegraded4S (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_DISC_DATA            *DiscEngData
  );

/**
  Get CpuType, CpuSubType and CpuStepping, then save them in KTIRC Internal Global data.

  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval None

**/
VOID
KtiGetCpuTypeInfo (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Update Kti Internal Global data.

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param SocktId           - Socket ID

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
UpdateCpuInfoInKtiInternal (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocktId
  );

/**
  Assign the real value for MMCFG size and base, MMIOL base and size and validate these value.

  @param KtiInternalGlobal - KTIRC internal variables.

  @retval KTI_SUCCESS

**/
KTI_STATUS
UpdateMmcfgMmiolBaseSize (
  IN OUT KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

/**

  @param[in] SocketData             Pointer to socket specific data
  @param[in,out] KtiInternalGlobal  Pointer to KTIRC Internal Global data structure

  @retval KTI_IGNORE  Feature not supported
  @retval KTI_SUCCESS

**/
KTI_STATUS
ProcessStrapsConfig (
  IN     KTI_SOCKET_DATA            *SocketData,
  IN OUT KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );


/**
  Check CSRs content that are associated with PMC set_strap settings.

  This routine is part of warm-reset elimination flow (i.e. skipping warm-reset
  on the cold-reset path). The VDM set_straps are the configuration straps
  transferred during Reset Phase 5 from PCH to CPU. Pcode will populate
  specific registers when processing set_strap message data registers. BIOS
  will read these registers and compare their values with BIOS setup options.
  If the registers match the setup option, the routine sets
  KtiInternalGlobal->SkipWarmResetRequest to TRUE. This routine covers only
  SNC, TwoLM and UMA set_strap settings.

  @param[in] SocketData             Pointer to socket specific data
  @param[in,out] KtiInternalGlobal  Pointer to KTIRC Internal Global data structure

  @retval KTI_SUCCESS

**/
KTI_STATUS
CheckSetStrapRegsConfiguration (
  IN     KTI_SOCKET_DATA            *SocketData,
  IN OUT KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  This routine determines the latest consumed physical address by system

  @param SocketData        - CPU related info
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval KTI_SUCCESS

**/
KTI_STATUS
CalculateLatestConsumedPA (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
Update Max Kti Agent for a given CPU socket.

@param KtiInternalGlobal - KTIRC internal variables.
@param SocId             - Soc ID

@retval 0 - Successful
@retval 1 - Failure

**/
KTI_STATUS
UpdateMaxKtiAgentAndM3KtiCount (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  UINT8                     SocId
 );

/**
  Routine to initialize SocketData

  @param[in]  KtiInternalGlobal   Pointer to the KTI RC internal global structure
  @param[out] SocketData          Pointer to socket specific data

  @retval KTI_SUCCESS

**/
KTI_STATUS
InitSocketData (
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  OUT KTI_SOCKET_DATA           *SocketData
  );

/**
  Update M2Pcie credits

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
UpdateM2PcieCreditSetting (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Routine to get topology for credit programming.

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID

  @retval Topology index  - Topology defined as defined for each CPU type

**/
UINT8
GetTopologyIndexForCredit (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc
  );

/**
  Get the prq setting for the selected stack
  @param SocId               - Socket ID
  @param M2UPcieStackCpx     - M2PCIE stack number
  @param KtiInternalGlobal   - pointer to the KTI RC internal global structure

  @retval Credit for the selected stack

**/
UINT8
GetPrqCredit (
  UINT8                       SocId,
  UINT8                       M2UPcieStackCpx,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Routine to get the index used to access credits arrays based on the stack number being used

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Stack             - Stack number whose index is to be computed
  @param StackIndex        - Stack Index returned

  @retval SUCCESS if stack number passed is valid, FAILURE otherwise

**/
KTI_STATUS
GetIndexForStack (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Stack,
  UINT8                      *StackIndex
  );

/**
Program all the credits for destination UBOX.

@param KtiInternalGlobal - pointer to the KTI RC internal global structure
@param Soc               - Socket ID

@retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
ProgramCreditForDestUbox (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      Soc
  );

/**
  Program CHA credits.

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID

  @retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
ProgramCreditForDestM2UPcie (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc
  );

/**
  Trigger late action credit reload without issuing a warm-reset.

  @param[in] KtiInternalGlobal    Pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS   Successful completion.
  @retval KTI_IGNORE    Triggering late action credit update is not needed.

**/
KTI_STATUS
LateActionCreditUpdateWithoutWarmReset (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Update IsocEn knob setting based on AEP DIMM presence detection.
  This code needs to be called before BIOS updates ChaToM2m credit programming.

  @param[in] KtiInternalGlobal    KTIRC Internal Global data

**/
VOID
UpdateIsocEnSetting (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
Populate CpuCsrAccessVar structure and display current bus assignments

@param KtiInternalGlobal    - pointer to the KTI RC internal global structure
@param CpuCsrAccessVar      - pointer to CpuCsrAccessVar structure to be populated
@param SocketData           - Pointer to socket specific data
@param PrintMissingSockets  - TRUE/FALSE

@retval None

**/
VOID
UpdateAndDisplayBusInformation (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  CPU_CSR_ACCESS_VAR         *CsrAccessVarPtr,
  KTI_SOCKET_DATA            *SocketData,
  BOOLEAN                    PrintMissingSockets
);

/**
  This routine assigns MMIOL resources to all supported Ubox BAR regions.

  For hot plug systems, resources will be preallocated.

  @param[in] SocketData                CPU related info
  @param[in, out] KtiInternalGlobal    KTIRC internal variables.

  @retval KTI_SUCCESS    Ubox BAR regions are assigned with MMIOL resources.
  @retval KTI_IGNORE     Unsupported feature.

**/
KTI_STATUS
CalculateUboxMmioResources (
  IN     KTI_SOCKET_DATA            *SocketData,
  IN OUT KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**

  Or current resetRequired with the new required type of reset
  @param[in] resetRequired     - type of reset required

**/
VOID
EFIAPI
SetNewResetRequired (
  UINT8 NewResetRequired
  );
#endif // _UNCORE_LIB_H_
