/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2008 - 2019 Intel Corporation. <BR>

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

#ifndef _UPI_RAS_MAIN_H
#define _UPI_RAS_MAIN_H

#include <Library/UncoreLib.h>
#include <Library/MultiSocketLib.h>

RAS_MPLINK_STATUS
ValidateParameters (
  IN UINT8 EventType,
  IN INT32 SocketType,
  IN INT32 SocketId
  );

RAS_MPLINK_STATUS
PowerUpOnlinedSocket (
  VOID
  );

RAS_MPLINK_STATUS
ReleaseResetOnlinedSocket (
  VOID
  );

RAS_MPLINK_STATUS
PowerDownOnlinedSocket (
  IN INT32            SocketId,
  IN KTI_SOCKET_DATA  *RasSocketData
  );

RAS_MPLINK_STATUS
PrepareHostDataStruct (
  VOID
  );

RAS_MPLINK_STATUS
DataDumpRasMpLink (
  VOID
  );

RAS_MPLINK_STATUS
OpenSbspConfigSpaceForHotAdd (
  VOID
  );

RAS_MPLINK_STATUS
AdjustSbspConfigSpaceForHotRemoval (
  VOID
  );

RAS_MPLINK_STATUS
CollectLepForOnlineEvent (
  VOID
  );

RAS_MPLINK_STATUS
CollectLepForOfflineEvent (
  VOID
  );

RAS_MPLINK_STATUS
CopyTopologyInfoToHostParm (
  VOID
  );

RAS_MPLINK_STATUS
SetConfigAndBootPath (
  VOID
  );

RAS_MPLINK_STATUS
UpdateCpuList (
  IN BOOLEAN AllCpus
  );

RAS_MPLINK_STATUS
VerifyCpuCheckIn (
  VOID
  );

RAS_MPLINK_STATUS
HaltPbsp (
  VOID
  );

RAS_MPLINK_STATUS
IssueWarmResetOfOnlinedCpu (
  IN UINT8 IssueOnlineCmd
  );

RAS_MPLINK_STATUS
SyncUpPbspForRas (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  UINT8                    RstType
  );

RAS_MPLINK_STATUS
SyncUpDramSad (
  VOID
  );

RAS_MPLINK_STATUS
AdjustIntDomainForHotAdd (
  VOID
  );

RAS_MPLINK_STATUS
PrepareForSlowSpeedTraining (
  IN KTI_SOCKET_DATA *SocketData
  );

KTI_STATUS
KtiFullSpeedTransitionForHotAdd (
  VOID
  );

KTI_STATUS
MemSpeedModeTransitionForHotAdd (
  VOID
  );

RAS_MPLINK_STATUS
KtiPhyLayerFullSpeedTransitionForCpuHotAdd (
  VOID
  );

KTI_STATUS
ProgramLateActionForHotAdd (
  IN UINT8 Value
  );

RAS_MPLINK_STATUS
ProgramUnlockDfx (
 );

RAS_MPLINK_STATUS
ProgramPhyLayerForHotAdd (
  VOID
  );

RAS_MPLINK_STATUS
ProgramLinkLayerForHotAdd (
  VOID
  );

KTI_STATUS
VerifyLinkStatusAfterWarmReset (
  VOID
  );

RAS_MPLINK_STATUS
KtiRasUpdateSysCpuCsrAccessVar (
  VOID
  );

RAS_MPLINK_STATUS
SetupSbspConfigAccessPath (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  TOPOLOGY_TREE_NODE       *Interm,
  TOPOLOGY_TREE_NODE       *Dest,
  UINT8                    DestPort
  );

RAS_MPLINK_STATUS
SetupRemoteCpuBootPath (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  TOPOLOGY_TREE_NODE       *Interm,
  TOPOLOGY_TREE_NODE       *Dest,
  UINT8                    DestPort
  );

KTI_STATUS
AddNodeToTopologyTree (
  TOPOLOGY_TREE_NODE *Tree,
  TOPOLOGY_TREE_NODE *Node
  );

KTI_STATUS
UpdateCpuInfoInKtiInternal (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  UINT8                    SocktId
  );

BOOLEAN
CheckThisSocketInfoWithSbsp (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  UINT8                    SbspSktId,
  UINT8                    SocketId
  );

RAS_MPLINK_STATUS
KtiFinalCpuBusCfg (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
  );

RAS_MPLINK_STATUS
ProgramSystemRoute (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
  );

RAS_MPLINK_STATUS
SetupSystemIoSadEntries (
  IN OUT KTI_SOCKET_DATA   *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
  );

RAS_MPLINK_STATUS
SetupKtiMisc (
  OUT KTI_SOCKET_DATA      *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
  );

RAS_MPLINK_STATUS
CollectAndClearErrors (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
  );

RAS_MPLINK_STATUS
SetupSystemCoherency (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
  );

RAS_MPLINK_STATUS
ProgramLateActionOnCpuLink (
   UINT8     Cpu,
   UINT8     Link,
   IN UINT32 Value
);

RAS_MPLINK_STATUS
ProgramPhyLayerControlRegisters (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  KTI_SOCKET_DATA           *SocketData,
  UINT8                     Socket,
  UINT8                     LinkIndex,
  UINT8                     Group
  );

RAS_MPLINK_STATUS
ProgramEparams (
  UINT8          Index,
  UINT8          LinkIndex,
  UINT8          Group
  );

KTI_STATUS
KtiEvAutoRecipe (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  UINT8          Socket,
  UINT8          LinkIndex
  );

RAS_MPLINK_STATUS
ProgramLinkLayerControlRegisters (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  UINT8                    Index,
  UINT8                    LinkIndex,
  UINT8                    Group
  );

STATIC
RAS_MPLINK_STATUS
ProgramLinkLayer (
  UINT8          Group
  );

RAS_MPLINK_STATUS
NormalizeLinkOptions (
  KTI_SOCKET_DATA     *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

KTI_STATUS
ClearOrSetKtiDfxLckForFullSpeedTransition (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  UINT8                     Value
  );

KTI_STATUS
ProgramLinkLayerOnM3Kti (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  UINT8                    Index,
  UINT8                    M3Kti,
  UINT32                   VnaCredit
  );

RAS_MPLINK_STATUS
ClearCinitbeginOnInvalidLinks (
  IN KTI_SOCKET_DATA *SocketData
  );

KTI_STATUS
ProgramMiscBeforeWarmReset (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
  );

RAS_MPLINK_STATUS
PrimeHostStructure (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
  );

RAS_MPLINK_STATUS
ProgramMsrForWa (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
  );

KTI_STATUS
ReceivingKtiPortsFullSpeedTransition (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  UINT8                    Socket,
  UINT8                    LinkIndex
  );

KTI_STATUS
ReceivingKtiPortsSetCinitbegin (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
  );

KTI_STATUS
ReceivingKtiPortsReenableLinkLayerClockGating (
  );

#endif // _UPI_RAS_MAIN_H
