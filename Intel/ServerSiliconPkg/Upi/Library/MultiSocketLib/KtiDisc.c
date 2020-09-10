/** @file
  This files includes functions which are used for KTI topology discovery etc.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2020 Intel Corporation. <BR>

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

#include <Library/UncoreLib.h>
#include <Library/MultiSocketLib.h>
#include <Library/EightSocketLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <KtiMisc.h>
#include <SiliconSetting.h>
#include <Library/MemoryCoreLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/KtiApi.h>
#include <Guid/KtiIpInterface.h>
#include <Library/PcuIpLib.h>
#include <Library/ChaLib.h>
#include "MultiSocketInternal.h"

STATIC
KTI_STATUS
ConstructRingTree (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_DISC_DATA              *DiscEngData,
  TOPOLOGY_TREE_NODE         *RingTree,
  UINT8                      RootSocId
  );

STATIC
KTI_STATUS
CheckForRings (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_DISC_DATA              *DiscEngData,
  TOPOLOGY_TREE_NODE         *RingTree,
  UINT8                      RootSocId
  );

STATIC
KTI_STATUS
ParseRings (
  KTI_DISC_DATA              *DiscEngData
  );

STATIC
KTI_STATUS
AssignVnCommon (
  KTI_DISC_DATA           *DiscEngData
  );

STATIC
KTI_STATUS
ComputeTopologyTree (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_DISC_DATA            *DiscEngData
  );

STATIC
KTI_STATUS
ExploreThisNode (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_DISC_DATA              *DiscEngData,
  UINT8                      RootSocId,
  TOPOLOGY_TREE_NODE         *Parent,
  UINT8                      ParentIdx,
  UINT8                      Hop
  );

STATIC
KTI_STATUS
CheckAndAddNodeToTopologyTree (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_DISC_DATA              *DiscEngData,
  UINT8                      RootSocId,
  TOPOLOGY_TREE_NODE         *Parent,
  TOPOLOGY_TREE_NODE         *LeafNode,
  UINT8                      Hop
  );

STATIC
KTI_STATUS
TrackPathLoad (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_DISC_DATA              *DiscEngData,
  TOPOLOGY_TREE_NODE         *Tree,
  TOPOLOGY_TREE_NODE         *LeafNode,
  UINT8                      Adjust
  );

STATIC
UINT8
CalculatePathLoad (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  KTI_DISC_DATA             *DiscEngData,
  TOPOLOGY_TREE_NODE        *Tree,
  TOPOLOGY_TREE_NODE        *LeafNode
  );

STATIC
KTI_STATUS
DetectDeadlock (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_DISC_DATA              *DiscEngData
  );

STATIC
KTI_STATUS
CopyTopologyTree (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_DISC_DATA            *DiscEngData
  );

STATIC
KTI_STATUS
CalculateRT (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
  );

STATIC
KTI_STATUS
SetupNonBCRoute (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  TOPOLOGY_TREE_NODE       *TopologyTree,
  TOPOLOGY_TREE_NODE       *Root,
  BOOLEAN                  FoundInAlternateTree,
  TOPOLOGY_TREE_NODE       *Interm,
  TOPOLOGY_TREE_NODE       *Dest
  );

STATIC
KTI_STATUS
SetupBCRoute (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  TOPOLOGY_TREE_NODE       *TopologyTree,
  TOPOLOGY_TREE_NODE       *Root,
  TOPOLOGY_TREE_NODE       *Interm,
  TOPOLOGY_TREE_NODE       *Dest,
  BOOLEAN                  PassThruFanout
  );

STATIC
KTI_STATUS
SetPassThruFanoutRoute (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  TOPOLOGY_TREE_NODE       *Parent,
  UINT8                    RootBcRti,
  UINT8                    InPort,
  UINT8                    OutPort
  );

STATIC
KTI_STATUS
SetRootNodeFanoutRoute (
  OUT KTI_SOCKET_DATA          *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN  UINT8                    RootBcRti,
  IN  UINT8                    OutPort,
  IN  BOOLEAN                  IsSecondLink
  );

STATIC
VOID
OptimizeChaFanoutSnoop (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  KTI_SOCKET_DATA           *SocketData
  );

STATIC
KTI_STATUS
Rearrange8sAlmostFullyConnectTopologyTree (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_DISC_DATA            *DiscEngData
  );

STATIC
KTI_STATUS
SelectUpiPortForRouteFrRootNodeToFpga (
  IN     KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN     KTI_SOCKET_DATA            *SocketData,
  IN     TOPOLOGY_TREE_NODE         *Interm,
  IN     TOPOLOGY_TREE_NODE         *Dest,
  IN OUT UINT8                      *Port1,
  IN OUT UINT8                      *Port2,
  IN OUT UINT8                      *Port3,
  IN OUT BOOLEAN                    *UseDualOrTripleLink
  );

STATIC
KTI_STATUS
SelectUpiPortForPassThruRouteToFpga (
  IN     KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN     KTI_SOCKET_DATA            *SocketData,
  IN     TOPOLOGY_TREE_NODE         *Grandparent,
  IN     TOPOLOGY_TREE_NODE         *Parent,
  IN     TOPOLOGY_TREE_NODE         *Child,
  IN OUT UINT8                      *ParentPort
  );

/**
  Translate Si port to gen port number

  @param SiPort - SiPort number to be translated

  @retval port number

**/
UINT8
TranslateToGenPort (
  UINT8                   SiPort
  )
{
  // Currently for all supported CPUs, Si port numbering and generic port numbering is same
  return SiPort;
}

/**
  Copies the socket data to disc engine structure and applies the KTI port mapping to make
  it Si agnostic.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC internal variables.
  @param DiscEngData       - Pointer to socket specific data

  @retval 0     - Success
  @retval Other - Failure

**/
KTI_STATUS
PrepareDiscEngData (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_DISC_DATA            *DiscEngData
  )
{
  UINT8               Idx1, Idx2;
  UINT8               TempPort;
  UINT8               PeerPort, PeerSocId;

  //
  // Copy the Si specific CPU socket data and convert that into Si agnostic data using the port mapping.
  //
  ZeroMem ((VOID *) DiscEngData, sizeof (KTI_DISC_DATA));
  CopyMem ((UINT8 *) DiscEngData->Cpu, (UINT8 *) SocketData->Cpu, sizeof (DiscEngData->Cpu));

  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if ((DiscEngData->Cpu[Idx1].Valid == TRUE) && (DiscEngData->Cpu[Idx1].SocType == SOCKET_TYPE_CPU)) {
      DiscEngData->TotCpus++;
      DiscEngData->CpuList = DiscEngData->CpuList | (1 << Idx1);
      for (Idx2 = 0; Idx2 < KtiInternalGlobal->KtiPortCnt; Idx2++) {
        if ((DiscEngData->Cpu[Idx1].LinkData[Idx2].Valid == TRUE)) {
          TempPort = (UINT8) DiscEngData->Cpu[Idx1].LinkData[Idx2].PeerPort;
          DiscEngData->Cpu[Idx1].LinkData[Idx2].PeerPort = TranslateToGenPort (TempPort);
        }
      }
    } else if ((DiscEngData->Cpu[Idx1].SocType == SOCKET_TYPE_FPGA) && !(SocketFpgasInitialized (KtiInternalGlobal)) ) {
      DiscEngData->Cpu[Idx1].Valid = FALSE;
      for (Idx2 = 0; Idx2 < KtiInternalGlobal->KtiPortCnt; Idx2++) {
        if (DiscEngData->Cpu[Idx1].LinkData[Idx2].Valid == TRUE) {
          PeerPort = (UINT8)DiscEngData->Cpu[Idx1].LinkData[Idx2].PeerPort;
          PeerSocId = (UINT8)DiscEngData->Cpu[Idx1].LinkData[Idx2].PeerSocId;
          if ((PeerSocId >= MAX_SOCKET) || (PeerPort >= (KtiInternalGlobal->KtiPortCnt))) {
            KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n PeerPort:%u >= KtiPortCnt:%u or PeerSocId:%u >= MAX_SOCKET:%u",
              PeerPort, (KtiInternalGlobal->KtiPortCnt), PeerSocId, MAX_SOCKET));
            KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
            return KTI_FAILURE;
          }

          DiscEngData->Cpu[PeerSocId].LinkData[PeerPort].Valid = FALSE;
        }
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  This routine utilizes the ring pairs found in LocateRings() routine and use the links
  that connects two nodes in each ring pair as the spoke of the pinwheel.

  Currently this routine is only used in 8S4L twisted fully connect cubes topology
  to identify which port should be assigned with opposite VN of what was assigned
  at a given socket.

  Notes: Depending on how we assign the nodes in pinwheel, the diagram below could have:
  (1) S0 - S1, S2 - S3, S6 - S7 and S4 - S5 as spokes of the pinwheel, or
  (2) S0 - S2, S1 - S3, S4 - S6 and S5 - S7 as spokes of the pinwheel.

  For 8S4L twisted fully connect cubes topology, it would use format (2) as spokes
  of pinwheel.
      +-----------------------+
      |                       |       Rings pairs:
      S0 ---- S1     S6 ---- S7       ============
      |       | \   / |       |       S0 - S2, S2 - S0
      |       |  \ /  |       |       S1 - S3, S3 - S1
      |       |   \   |       |       S4 - S6, S6 - S4
      |       |  / \  |       |       S5 - S7, S7 - S5
      |       | /   \ |       |
      S2 ---- S3     S4 ---- S5
      |                       |
      +-----------------------+

  @param[in,out] SocketData           Pointer to socket specific data
  @param[in]     KtiInternalGlobal    KTIRC internal variables
  @param[in,out] DiscEngData          Pointer to socket specific data

  @retval KTI_SUCCESS  Ring pairs are parsed. LEP info in SocketData structure are updated.
  @retval KTI_IGNORE   Function is not applicable to the current topology.
  @retval KTI_FAILURE  No ring found.

**/
KTI_STATUS
LocateSpokeInPinwheel (
  IN OUT  KTI_SOCKET_DATA          *SocketData,
  IN      KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN OUT  KTI_DISC_DATA            *DiscEngData
  )
{
  UINT8   Idx;
  UINT8   Port;
  UINT8   PeerSocId;

  if (KTIVAR->SysConfig != SYS_CONFIG_8S) {
    return KTI_IGNORE;
  }

  if (KtiInternalGlobal->EightSocketTopology != Topo8STwistedFullyConnectCubes) {
    return KTI_IGNORE;
  }

  if (DiscEngData->TotRings == 0) {
    KTI_ASSERT ((DiscEngData->TotRings > 0), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
    return KTI_FAILURE;
  }

  for (Idx = 0; Idx < MAX_SOCKET; Idx++) {
    PeerSocId = DiscEngData->RingNodePairs[Idx];
    if ((DiscEngData->Cpu[Idx].Valid == FALSE) || (PeerSocId == 0xFF)) {
      continue;
    }

    for (Port = 0; Port < KtiInternalGlobal->KtiPortCnt; Port++) {
      if (PeerSocId == DiscEngData->Cpu[Idx].LinkData[Port].PeerSocId) {
        DiscEngData->Cpu[Idx].LinkData[Port].SpokeOfPinwheel = TRUE;
        break;
      }
    }
  }
  CopyMem ((UINT8 *) SocketData->Cpu, (UINT8 *) DiscEngData->Cpu, sizeof (DiscEngData->Cpu));

  return KTI_SUCCESS;
}

/**
  This Routine will dump assigned VN for each valid socket.

  @param DiscEngData       - Pointer to socket specific data

  @retval                  - VOID
**/
STATIC
VOID
DumpVnAssignment (
  KTI_DISC_DATA             *DiscEngData
  )
{
  UINT8    Idx1;

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\n  VN Assignment:"));
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (DiscEngData->Cpu[Idx1].Valid == TRUE) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n    %u - VN%u", Idx1, DiscEngData->Cpu[Idx1].Vn));
    }
  }

  return;
}

/**
  Assign VN to the nodes in 8S 2Clumps topology.
  The clump with legacy socket in it will be assigned with VN0; the other clump will be assigned with VN1.

  For example:
             +----------------+
     +-------|--------+       |
     |       |        |       |
    S0 ---- S1       S6 ---- S7
     | \   / |        | \   / |
     |  \ /  |        |  \ /  |
     |   \   |        |   \   |
     |  / \  |        |  / \  |
     | /   \ |        | /   \ |
    S2 ---- S3       S4 ---- S5
     |       |        |       |
     +-------|--------+       |
             +----------------+

  VN Assignment:
    S0  -  VN0
    S1  -  VN0
    S2  -  VN0
    S3  -  VN0
    S4  -  VN1
    S5  -  VN1
    S6  -  VN1
    S7  -  VN1

  @param KtiInternalGlobal - KTIRC internal variables
  @param DiscEngData       - Pointer to socket specific data

  @retval 0     - Success
  @retval Other - Failure
**/
STATIC
KTI_STATUS
AssignVnToTwoClumps (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  KTI_DISC_DATA             *DiscEngData
  )
{
  UINT8        ClumpIdx;
  UINT8        Index, TmpSocId;

  //
  // There are total of two clumps in 8S 2Clumps topology.
  //
  ClumpIdx = KtiInternalGlobal->Topo8S2ClumpsInfo.ClumpWithLegacySocket;
  if (ClumpIdx >= MAX_CLUMPS_NUMBER_IN_8S_TOPOLOGY) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "  %a () - Fatel Error: Clump index [%X] overflow", __FUNCTION__, ClumpIdx));
    KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
  }

  //
  // The legacy socket is either in Clump#0 or in Clump#1.
  // Get the index of the clump that does not contain the legacy socket.
  //
  ClumpIdx = (ClumpIdx == 0) ? 1 : 0;

  //
  // Assign VN1 to all sockets in Clump[ClumpIdx]
  // Note: Other sockets not in Clump[ClumpIdx] will be assigned VN0 by default
  //
  for (Index = 0; Index < SOCKETS_NUMBER_IN_EACH_CLUMP; Index++) {
    TmpSocId = KtiInternalGlobal->Topo8S2ClumpsInfo.Clumps[ClumpIdx][Index];
    if (TmpSocId >= MAX_SOCKET) {
      KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "  %a () - Fatel Error: SocketId: %X is invalid", __FUNCTION__, TmpSocId));
      KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_CPU_SOCKET_ID);
    }

    DiscEngData->Cpu[TmpSocId].Vn = VN1;
  }

  return KTI_SUCCESS;
}

/**
  Assign VN if the knob OutVn1En is set to TRUE.

  @param[in] KtiInternalGlobal - KTIRC internal variables
  @param[in] DiscEngData       - Pointer to socket specific data

  @retval KTI_SUCCESS     - Success
  @retval Others          - Failure

**/
STATIC
KTI_STATUS
AssignVn (
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN KTI_DISC_DATA             *DiscEngData
  )
{
  KTI_STATUS      Status;

  if (KTIVAR->OutVn1En == FALSE) {
    return KTI_SUCCESS;
  }

  if ((KtiInternalGlobal->EightSocketTopology == Topo8STwoClumps) && (KTIVAR->SysConfig == SYS_CONFIG_8S)) {
    Status = AssignVnToTwoClumps (KtiInternalGlobal, DiscEngData);
  } else {
    Status = AssignVnCommon (DiscEngData);
  }

  DumpVnAssignment (DiscEngData);
  return Status;
}

/**
  Perform the KTI Topology Discovery of the system. It discovers the system
  topology from each socket's perspective and calculates the route. This
  function must be invoked only by SBSP after collecting LEP of each socket populated.
  This function doesn't program any H/W registers. Route Table of each socket is
  returned in SocketData. The caller is responsible for porgramming the Route Table into
  the appropriate registers upon successful exit from this function.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval 0     - Success
  @retval Other - Failure

**/
KTI_STATUS
KtiTopologyDiscovery (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
  KTI_DISC_DATA           DiscEngData;

  // Prepare the discovery engine data structure
  PrepareDiscEngData (SocketData, KtiInternalGlobal, &DiscEngData);

  // Check and identify the rings present in the topology
  LocateRings (KtiInternalGlobal, &DiscEngData);

  // Assign VN to each node that is part of a ring
  AssignVn (KtiInternalGlobal, &DiscEngData);

  //
  // Locate pinwheel's spokes in 8S4L twisted fully connect topology
  //
  LocateSpokeInPinwheel (SocketData, KtiInternalGlobal, &DiscEngData);

  // Discover the topology and construct the topology tree.
  ComputeTopologyTree (KtiInternalGlobal, &DiscEngData);

  // Rearrange topology tree for 8-socket Almost Fully Connect topology if necessary
  Rearrange8sAlmostFullyConnectTopologyTree (KtiInternalGlobal, &DiscEngData);

  // Check to make sure the route is deadlock free
  DetectDeadlock (KtiInternalGlobal, &DiscEngData);

  // Copy the topology tree info into Host/SocketData
  CopyTopologyTree (SocketData, KtiInternalGlobal, &DiscEngData);

  // Sanity check discoverd topology
  if (SanityCheckDiscoveredTopology (SocketData, KtiInternalGlobal) != KTI_SUCCESS) {
    return KTI_FAILURE;
  }

  // Calculate the route setting
  CalculateRT (SocketData, KtiInternalGlobal);

  return KTI_SUCCESS;
}

/**
  Identifies all the rings present in a topology. A ring is defined as 4 sockets
  forming a circle. Each ring is assigned a unique number and the sockets
  are marked with the number of rings that they belong to and ring number that they
  belong to. This routine also identifies the pair of nodes that belong to more than
  one ring.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param DiscEngData       - Pointer to discovery engine data

  @retval 0     - Success
  @retval Other - Failure

**/
KTI_STATUS
LocateRings (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_DISC_DATA              *DiscEngData
  )
{
  TOPOLOGY_TREE_NODE  RingTree[MAX_RING_TREE_NODES];
  UINT8               Index;

  KtiCheckPoint (0xFF, 0xFF, 0xFF, STS_TOPOLOGY_DISCOVERY, MINOR_STS_DETECT_RING);

  if ((KTIVAR->SysConfig == SYS_CONFIG_8S) && ((KtiInternalGlobal->EightSocketTopology == Topo8SAlmostFullyConnect) ||
      (KtiInternalGlobal->EightSocketTopology == Topo8STwoClumps))) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Skip Locating the Rings\n"));
    return KTI_SUCCESS;
  }

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Locating the Rings Present in the Topology\n"));

  //
  // Locate all the rings present in the system
  //
  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if (DiscEngData->Cpu[Index].Valid == TRUE) {
      ZeroMem ((VOID *) (RingTree), sizeof (RingTree));
      ConstructRingTree (KtiInternalGlobal, DiscEngData, RingTree, Index);
      CheckForRings (KtiInternalGlobal, DiscEngData, RingTree, Index);
    }
  }

  //
  // Dump all the rings present in the system
  //
  if (DiscEngData->TotRings == 0) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  No Rings Found\n"));
  } else {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  %u Ring(s) Found", DiscEngData->TotRings));
    for (Index = 0; Index < DiscEngData->TotRings; Index++) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n    Ring %u:   CPU%u -> CPU%u -> CPU%u -> CPU%u -> CPU%u",
                  Index, DiscEngData->Rings[Index][0], DiscEngData->Rings[Index][1],
                  DiscEngData->Rings[Index][2], DiscEngData->Rings[Index][3], DiscEngData->Rings[Index][4]));
    }
  }

  //
  // Parse the rings and identify the pair of nodes that belong to more than 1 ring
  //
  if (DiscEngData->TotRings > 0) {
    ParseRings (DiscEngData);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Ring Pairs:\n"));
    for (Index = 0; Index < MAX_SOCKET; Index++) {
      if (DiscEngData->Cpu[Index].Valid == TRUE && DiscEngData->RingNodePairs[Index] != 0xFF) {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "    %u  -  %u\n", Index, DiscEngData->RingNodePairs[Index]));
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  Construct a simple tree for each socket with nodes spanning upto 4 hops. Then parse
  the tree to look for any rings.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param DiscEngData       - Pointer to socket specific data
  @param RingTree          - Pointer to full system KTI fabric tree
  @param RootSocId         - Root Socket Id

  @retval 0     - Success
  @retval Other - Failure

**/
STATIC
KTI_STATUS
ConstructRingTree (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_DISC_DATA              *DiscEngData,
  TOPOLOGY_TREE_NODE         *RingTree,
  UINT8                      RootSocId
  )
{
  TOPOLOGY_TREE_NODE  Child, Parent, GrandParent, RootSocket;
  KTI_LINK_DATA       ParentLep;
  KTI_STATUS          Status;
  UINT8               Index;
  UINT8               KtiLink;
  UINT8               NumOfHops;
  UINT8               SbspSocId;
  BOOLEAN             Is8STwistedFullyConnect;

  SbspSocId = KtiInternalGlobal->SbspSoc;

  GrandParent.RawNode        = 0;
  //
  //  Add this socket as Root of the topology tree
  //
  RootSocket.RawNode          = 0;
  RootSocket.Node.Valid       = TRUE;
  RootSocket.Node.SocId       = RootSocId;
  RootSocket.Node.SocType     = SOCKET_TYPE_CPU;
  RootSocket.Node.ParentPort  = 0xF;
  RootSocket.Node.Hop         = 0;
  RootSocket.Node.ParentIndex = 0x7F;
  if ((Status = AddNodeToRingTree (RingTree, &RootSocket)) != KTI_SUCCESS) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n  Adding CPU%u to Ring Tree of CPU%u failed - Data Structure Overflow.",
      RootSocket.Node.SocId, RootSocket.Node.SocId));
    KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
  }

  Is8STwistedFullyConnect = FALSE;
  if ((KtiInternalGlobal->EightSocketTopology == Topo8STwistedFullyConnectCubes) && (KTIVAR->SysConfig == SYS_CONFIG_8S)) {
    Is8STwistedFullyConnect = TRUE;
  }

  //
  // Default hop level is 4 (hop 0 being root) since the ring formation involves only 4 nodes.
  //
  NumOfHops = 4;

  //
  // Do a Breadth First Search (BSF) and construct the tree. To start with, the RingTree must
  // have the root socket already. The RingTree will grow as we discover new sockets in BSF manner.
  //
  for (Index = 0; ((RingTree[Index].Node.Valid != FALSE) && (RingTree[Index].Node.SocType != SOCKET_TYPE_FPGA)); Index++) {
    //
    // Stop constructing the tree when we reach hop level "NumOfHops" (hop 0 being root)
    //
    Parent = RingTree[Index];
    if (Parent.Node.Hop == NumOfHops) {
      break;
    }

    if (Parent.Node.SocId >= MAX_SOCKET) {
      KTI_ASSERT ((Parent.Node.SocId < MAX_SOCKET), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_CPU_SOCKET_ID);
      return KTI_FAILURE;
    }

    if (Index != 0) {
      if (Parent.Node.ParentIndex >= MAX_RING_TREE_NODES) {
        KTI_ASSERT ((Parent.Node.ParentIndex < MAX_RING_TREE_NODES), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
        return KTI_FAILURE;
      }
      GrandParent.RawNode = RingTree[Parent.Node.ParentIndex].RawNode;
    }

    Child.RawNode     = 0;
    Child.Node.Valid  = TRUE;
    Child.Node.Hop    = Parent.Node.Hop + 1;
    Child.Node.ParentIndex = Index;
    if (Parent.Node.SocType == SOCKET_TYPE_CPU) {
      for (KtiLink = 0; KtiLink < KtiInternalGlobal->KtiPortCnt; KtiLink++) {
        ParentLep = DiscEngData->Cpu[Parent.Node.SocId].LinkData[KtiLink];

        //
        // Skip this link if it is marked as "Not Valid" or "DualLink" or "TwoSkt3Link".
        //
        if (ParentLep.Valid != TRUE || ParentLep.DualLink == TRUE || ParentLep.TwoSkt3Link == TRUE) {
          continue;
        }

        if (Index != 0 && ParentLep.PeerSocId == GrandParent.Node.SocId) {
          continue; // Skip the link if it is pointing towards root
        }

        //
        // Skip this link if it doesn't support route through traffic
        //
        if (ParentLep.DisallowRouteThru == TRUE) {
          continue;
        }

        //
        // Get the Child Socket Id, Type and port that connects it to the parent form LEP
        //
        Child.Node.SocId      = ParentLep.PeerSocId;
        Child.Node.SocType    = ParentLep.PeerSocType;
        Child.Node.ParentPort = ParentLep.PeerPort;

        if (Is8STwistedFullyConnect && (Child.Node.ParentPort == CROSSLINK_PORT_ID)) {
          //
          // This link is the cross link within the fully connected 4S cube in 8S twisted fully connect cubes.
          // Don't detect the ring formed using this UPI link for easy VN assignment calculation
          //
          continue;
        }

        // Add the node into the Ring Tree
        if ((Status = AddNodeToRingTree (RingTree, &Child)) != KTI_SUCCESS) {
          KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n  Adding CPU%u to Ring Tree of CPU%u failed - Data Structure Overflow.",
                     Child.Node.SocId, RootSocket.Node.SocId));
          KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
        }
      } // for(KtiLink...)
    } else {
      //
      // This is a fatal error
      //
      KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Unknown Socket Type found in CPU%u TopologyTree. Topology not supported.\n", RootSocId));
      KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE);
    }
  } // for(Index...)

  return KTI_SUCCESS;
}

/**
  Identify the rings present in the system and store if it is not found already.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param DiscEngData       - Pointer to socket specific data
  @param RingTree          - Pointer to full system KTI fabric tree
  @param RootSocId         - Root Socket Id

  @retval 0     - Success
  @retval Other - Failure

**/
STATIC
KTI_STATUS
CheckForRings (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_DISC_DATA              *DiscEngData,
  TOPOLOGY_TREE_NODE         *RingTree,
  UINT8                      RootSocId
  )
{
  TOPOLOGY_TREE_NODE  RingNode;
  UINT8               RingSocs[CPUS_PER_RING + 1];  // +1 needed since Element-0 is repeated in last Element to show a full circle.
  UINT8               Idx1, Idx2, Idx3;
  BOOLEAN             ValidRing;
  BOOLEAN             Is8STwistedFullyConnect;

  Is8STwistedFullyConnect = FALSE;
  if ((KtiInternalGlobal->EightSocketTopology == Topo8STwistedFullyConnectCubes) && (KTIVAR->SysConfig == SYS_CONFIG_8S)) {
    Is8STwistedFullyConnect = TRUE;
  }

  // Identify the index of the last node in the ring tree
  KTI_ASSERT ((RingTree[0].Node.SocId == RootSocId), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
  for (Idx1 = MAX_RING_TREE_NODES - 1; Idx1 != 0; Idx1--) {
    if (RingTree[Idx1].Node.Valid == TRUE) {
      break;
    }
  }

  //
  // The ring tree is a tree that spans 4 hops. By traversing from each leaf node towards the root node and checking
  // if the leaf node is same as root node (i.e the loop completes) and nodes don't repeat we can find if it is a ring or not.
  //
  for (Idx2 = Idx1; Idx2  != 0; Idx2--) {

    RingNode = RingTree[Idx2];

    //
    // We need to explore only the leaf nodes. If we explored all leaf nodes (i.e all nodes at hop level 4) break out.
    //
    if (RingNode.Node.Hop != 4) {
      break;
    }

    // If the leaf node is not same as root node, it can form a ring, so continue to next leaf
    if (RingNode.Node.SocId  !=  RootSocId) {
      continue;
    }

    //
    // We found a leaf node which is same as root node. Traverse back to the root to find out the nodes of the (possible) ring
    //
    Idx3 = (UINT8) RingNode.Node.Hop;

    if ((Idx3 > CPUS_PER_RING) || (RingNode.Node.ParentIndex >= MAX_RING_TREE_NODES)) {
      KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
      return KTI_FAILURE;
    }

    while (Idx3 != 0) {
      RingSocs[Idx3] = (UINT8) RingNode.Node.SocId;
      RingNode.Node = RingTree[RingNode.Node.ParentIndex].Node;
      Idx3--;
      KTI_ASSERT ((Idx3 == RingNode.Node.Hop), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
    }
    RingSocs[Idx3] = RootSocId;

    //
    // Make sure it is a valid ring. For a ring to be valid, the nodes 0 to 3 should not repeat and nodes 0 & 2, nodes 1 & 3
    // should not have any direct link between them.
    //

    // Make sure the 4 nodes of the ring are unique
    if ((RingSocs[0] == RingSocs[1]) || (RingSocs[0] == RingSocs[2]) || (RingSocs[0] == RingSocs[3]) ||
        (RingSocs[1] == RingSocs[2]) || (RingSocs[1] == RingSocs[3]) || (RingSocs[2] == RingSocs[3])) {
      continue;
    }

    // Make sure the diagonally opposite nodes dosen't have connection between them
    ValidRing = TRUE;
    for (Idx3 = 0; Idx3  < KtiInternalGlobal->KtiPortCnt; Idx3++) {
      if ((DiscEngData->Cpu[RingSocs[0]].LinkData[Idx3].PeerSocId == RingSocs[2]) ||
          (DiscEngData->Cpu[RingSocs[1]].LinkData[Idx3].PeerSocId == RingSocs[3])) {
        if (Is8STwistedFullyConnect && (Idx3 == CROSSLINK_PORT_ID)) {
          //
          // Forcing fully connect cubes in 8S twisted fully connect cube topology to be a ring
          // so that it becomes single pinwheel and reuses single pinwheel VN assignment flow
          //
          continue;
        }
        ValidRing = FALSE;
        break;
      }
    }
    if (ValidRing == FALSE) {
      continue;
    }

    //
    // Now that we found a valid ring, make sure this ring is not detected already as part of other root sockets
    //
    ValidRing = TRUE;
    for (Idx3 = 0; Idx3 < DiscEngData->TotRings; Idx3++) {
      if (CompareRing (RingSocs, DiscEngData->Rings[Idx3]) == TRUE) {
        ValidRing = FALSE;
        break;
      }
    }
    if (ValidRing == TRUE) {
      CopyMem ((UINT8 *) DiscEngData->Rings[DiscEngData->TotRings], (UINT8 *) RingSocs, sizeof (DiscEngData->Rings[DiscEngData->TotRings]));
      DiscEngData->TotRings++;
    }
  } // for(Idx...)

  return KTI_SUCCESS;
}

/**
  Parse the rings and identify the number of rings and the ring number that each socket belongs to.

  @param DiscEngData - Pointer to socket specific data

  @retval 0     - Success
  @retval Other - Failure

**/
STATIC
KTI_STATUS
ParseRings (
  KTI_DISC_DATA              *DiscEngData
  )
{
  UINT8               TmpSocId;
  UINT8               Idx1, Idx2, TmpIdx;

  //
  // Identify the number of rings each socket is part of and the ring number
  //
  for (Idx1 = 0; Idx1 < DiscEngData->TotRings; Idx1++) {
    for (Idx2 = 0; Idx2 < CPUS_PER_RING; Idx2++) {
      TmpSocId = DiscEngData->Rings[Idx1][Idx2];
      if (TmpSocId >= MAX_SOCKET) {
        KTI_ASSERT ((TmpSocId < MAX_SOCKET), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_CPU_SOCKET_ID);
        return KTI_FAILURE;
      }

      TmpIdx = DiscEngData->RingInfo[TmpSocId][0]; // Element 0 contains the number of rings this socket is already part of
      if ((TmpIdx + 1) >= MAX_RINGS) {
        KTI_ASSERT (((TmpIdx + 1) < MAX_RINGS), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_CPU_SOCKET_ID);
        return KTI_FAILURE;
      }

      DiscEngData->RingInfo[TmpSocId][TmpIdx+1] = Idx1; // Element 1 to x contains the number of the ring which this socket is part
      DiscEngData->RingInfo[TmpSocId][0]++; // Increase the number of ring counts of which this socket is part of
    }
  }

  //
  // Identify the pair of sockets that is part of any 2 rings in the topology
  //
  SetMem ((UINT8 *) DiscEngData->RingNodePairs, sizeof (DiscEngData->RingNodePairs), 0xFF);
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (DiscEngData->RingInfo[Idx1][0] == 2) { // If this socket is part of 2 different rings
      for (Idx2 = 0; Idx2 < MAX_SOCKET; Idx2++) { // Identify the other socket that is part of the same 2 rings
        if (Idx2 != Idx1 && DiscEngData->RingInfo[Idx2][0] == 2) {
          if ((DiscEngData->RingInfo[Idx1][1] == DiscEngData->RingInfo[Idx2][1] &&
               DiscEngData->RingInfo[Idx1][2] == DiscEngData->RingInfo[Idx2][2]) ||
              (DiscEngData->RingInfo[Idx1][1] == DiscEngData->RingInfo[Idx2][2] &&
               DiscEngData->RingInfo[Idx1][2] == DiscEngData->RingInfo[Idx2][1])) {
            DiscEngData->RingNodePairs[Idx1] = Idx2;
            break;
          }
        }
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  Assign VN to the nodes. It is assigned to only the nodes that are part of a ring. Two adjacent nodes of the ring
  are assigned VN0 while the other two nodes are assigned VN1. If a pair of nodes is part of more than one ring
  such as in 8SG topology, the VN will be assigned such that each pair of nodes in all rings are different.

  For example:
    +-------------------------+
     |                         |
    S0 ----- S1       S6 ----- S7
     |       | \     /|        |
     |       |   \ /  |        |
     |       |    \   |        |
     |       |  /   \ |        |
     |       |/      \|        |
    S2 ----- S3       S4 ----- S5
     |                         |
     +-------------------------+

  VN Assignment:
    S0  -  VN0
    S1  -  VN1
    S2  -  VN0
    S3  -  VN1
    S4  -  VN0
    S5  -  VN1
    S6  -  VN0
    S7  -  VN1

  @param DiscEngData - Pointer to socket specific data

  @retval 0     - Success
  @retval Other - Failure

**/
STATIC
KTI_STATUS
AssignVnCommon (
  KTI_DISC_DATA           *DiscEngData
  )
{
  UINT8               TmpSoc0, TmpSoc1, TmpSoc2;
  UINT8               Idx1, Idx2, Idx3;
  UINT8               SocVnAssigned, VnAssigned, TmpVn;

  //
  // For each ring, identify the pair of nodes that is also part of another ring and assign same VN number to those pair of nodes
  //
  SocVnAssigned = 0; // Sockets for which VN is assignment
  VnAssigned = 0; // VN (i.e VN0/VN1) assigned to each socket
  for (Idx1 = 0; Idx1 < DiscEngData->TotRings; Idx1++) {
    for (Idx2 = 0; Idx2 < CPUS_PER_RING; Idx2++) {
      TmpSoc0 = DiscEngData->Rings[Idx1][Idx2];
      if (TmpSoc0 >= MAX_SOCKET) {
        KTI_ASSERT ((TmpSoc0 < MAX_SOCKET), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_CPU_SOCKET_ID);
        return KTI_FAILURE;
      }
      TmpSoc1 = DiscEngData->RingNodePairs[TmpSoc0];

      //
      // If the node is not assigned VN already and it has a pair with which it shares another ring, assign VN to the pair of sockets.
      // Make sure the VN assigned to the pair is different from the VN, if any, already assigned to other 2 nodes of the ring.
      // Since VN is assigned to pair of nodes, just checkig the VN assigned for just of the other 2 nodes is sufficient.
      //
      if (((SocVnAssigned & (1 << TmpSoc0)) == 0) && (TmpSoc1 != 0xFF)) {
        for (Idx3 = 0; Idx3 < CPUS_PER_RING; Idx3++) {
          TmpSoc2 = DiscEngData->Rings[Idx1][Idx3];
          if (TmpSoc2 != TmpSoc0 && TmpSoc2 != TmpSoc1) {
            if (SocVnAssigned & (1 << TmpSoc2)) {
              TmpVn = (((VnAssigned >> TmpSoc2) & 0x1) == VN0) ? VN1 : VN0;
            } else {
              TmpVn = VN0;
            }
            VnAssigned = VnAssigned | (TmpVn << TmpSoc0) | (TmpVn << TmpSoc1);
            SocVnAssigned = SocVnAssigned | (1 << TmpSoc0) | (1 << TmpSoc1);
            break;
          }
        }
      }
    }
  }

  //
  // For the nodes that are part of only one ring, pair it with immediate neighbor and assign the VN to both of them.
  //
  for (Idx1 = 0; Idx1 < DiscEngData->TotRings; Idx1++) {
    //
    // Check if any of the nodes of this ring is assigned a VN already
    //
    TmpVn = 0xFF;
    for (Idx2 = 0; Idx2 < CPUS_PER_RING; Idx2++) {
      TmpSoc0 = DiscEngData->Rings[Idx1][Idx2];
      if (SocVnAssigned & (1 << TmpSoc0)) {
        TmpVn = (((VnAssigned >> TmpSoc0) & 0x1) == VN0) ? VN1 : VN0;
        break;
      }
    }

    if (TmpVn == 0xFF) {
      //
      // If none of the nodes of this ring is assigned an VN, then this ring has nodes that are part of only this ring.
      // Assign VN0 to first two nodes of the ring and VN1 to the remaining two nodes of the ring.
      //
      for (Idx2 = 0; Idx2 < CPUS_PER_RING; Idx2++) {
        TmpSoc0 = DiscEngData->Rings[Idx1][Idx2];
        VnAssigned = VnAssigned | ((Idx2 /2) << TmpSoc0);
        SocVnAssigned = SocVnAssigned | (1 << TmpSoc0);
      }
    } else {
      //
      // This means either two nodes or all 4 nodes are already assigned VN. In case 1, assign VN to the
      // remaining 2 nodes. For case 2, nothing to do.
      //
      for (Idx2 = 0; Idx2 < CPUS_PER_RING; Idx2++) {
        TmpSoc0 = DiscEngData->Rings[Idx1][Idx2];
        if ((SocVnAssigned & (1 << TmpSoc0)) == 0) {
          VnAssigned = VnAssigned | (TmpVn << TmpSoc0);
          SocVnAssigned = SocVnAssigned | (1 << TmpSoc0);
        }
      }
    }
  }

  //
  // Verify that all nodes of a ring is assigned a VN; otherwise it is fatal condition.
  //
  for (Idx1 = 0; Idx1 < DiscEngData->TotRings; Idx1++) {
    for (Idx2 = 0; Idx2 < CPUS_PER_RING; Idx2++) {
      TmpSoc0 = DiscEngData->Rings[Idx1][Idx2];
      if ((SocVnAssigned & (1 << TmpSoc0)) == 0) {
        KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n  CPU%u VN Allocation Failed - Topology Not Supported!!! \n  System Halted", TmpSoc0));
        KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
      }
    }
  }

  // Copy the assignedVN into Disc Eng structure
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (DiscEngData->Cpu[Idx1].Valid == TRUE) {
      DiscEngData->Cpu[Idx1].Vn = (VnAssigned >> Idx1) & 0x1;
    }
  }

  //
  // If there are no rings, all sockets would have been assigned VN0 which will cause deadlock in some
  // degraded topology scenarios.  Assign VN1 for half of the populated sockets.
  // (Non-degraded topologies will not have this problem since they will have rings and VN will be assigned correctly).
  //
  VnAssigned = VN1;
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
      if ((DiscEngData->Cpu[Idx1].Valid == TRUE) && ((SocVnAssigned & (1 << Idx1)) == 0)) {
        DiscEngData->Cpu[Idx1].Vn = VnAssigned;
        (VnAssigned == VN0) ? (VnAssigned = VN1) : (VnAssigned = VN0); // Toggle between VN0& VN1
      }
  }

  return KTI_SUCCESS;
}

/**
  Computes the KTI CPU topology found in the system

  @param KtiInternalGlobal - KTIRC internal variables.
  @param DiscEngData       - Pointer to socket specific data

  @retval 0     - Success
  @retval Other - Failure

**/
STATIC
KTI_STATUS
ComputeTopologyTree (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_DISC_DATA            *DiscEngData
  )
{
  TOPOLOGY_TREE_NODE  Parent;
  UINT8               Idx1, Idx2, Hop, RootSocId, Ring4S;
  KTI_STATUS          KtiStatus;
  BOOLEAN             Is8STwistedFullyConnect;

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\n  Constructing Topology Tree"));
  KtiCheckPoint (0xFF,  SOCKET_TYPE_CPU, 0, STS_TOPOLOGY_DISCOVERY, MINOR_STS_CONSTRUCT_TOPOLOGY_TREE);

  Is8STwistedFullyConnect = FALSE;
  if ((KtiInternalGlobal->EightSocketTopology == Topo8STwistedFullyConnectCubes) && (KTIVAR->SysConfig == SYS_CONFIG_8S)) {
    Is8STwistedFullyConnect = TRUE;
  }

  //
  // Add each populated socket as Root Node in its topology tree
  //
  Parent.RawNode          = 0;
  Parent.Node.Valid       = TRUE;
  Parent.Node.SocType     = SOCKET_TYPE_CPU;
  Parent.Node.ParentPort  = 0xF;
  Parent.Node.Hop         = 0;
  Parent.Node.ParentIndex = 0x7F;
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if ((DiscEngData->Cpu[Idx1].Valid == TRUE) && (DiscEngData->Cpu[Idx1].SocType == SOCKET_TYPE_CPU)) {
      Parent.Node.SocId = Idx1;
      if (AddNodeToTopologyTree (DiscEngData->CpuTree[Idx1], &Parent) != KTI_SUCCESS) {
        KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n  Adding CPU%u to Topology Discovery Tree of CPU%u failed - Data Structure Overflow.",
                          Parent.Node.SocId, Parent.Node.SocId));
        KTI_ASSERT (FALSE, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_55);
      }
    }
  }

  if ((KTIVAR->SysConfig == SYS_CONFIG_8S) && (KtiInternalGlobal->EightSocketTopology == Topo8STwoClumps)) {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\nCompute topology tree for 8S 2Clumps topology\n"));

    KtiStatus = ComputeTopologyTree8S2Clumps (KtiInternalGlobal, DiscEngData);
    if (KtiStatus != KTI_SUCCESS) {
      KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\nError: %a (): 8S 2Clumps compute topology tree failed.\n", __FUNCTION__));
      KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
    }

    return KtiStatus;
  }

  //
  // Do the topology discovery using Breadth First Search (BSF). To start with, the Topology Tree must
  // have the root socket already added. The TopologyTree will grow as we discover new sockets in BSF manner.
  // 4S ring topology is a special case; to avoid deadlock, we need to construct the tree for sockets in the
  // order they appear in the ring. For non-degraded 8S Single pinwheel and 6S configs, we will use the intel
  // suggested base line routing.
  //
  if ((DiscEngData->TotRings == 4) && !Is8STwistedFullyConnect) { // 8S
    ComputeTopologyTree8S (KtiInternalGlobal, DiscEngData);
  } else if (DiscEngData->TotRings == 2 && DiscEngData->TotCpus == 6) { // 6S
    ComputeTopologyTree6S (KtiInternalGlobal, DiscEngData);
  } else {
    // Degraded 6S (6SG1) topology uses generic topology flow - Just as if 'DEGRADE_TOPOLOGY_AS_IS' was set.

    //
    // Identify if this is a 4S with ring topology
    //
    if (DiscEngData->TotRings == 1 && DiscEngData->CpuList == 0xF) {
      Ring4S = TRUE;
    } else {
      Ring4S = FALSE;
    }
    for (Hop = 0; Hop < MAX_SOCKET; Hop++) {
      for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
        if (Ring4S == TRUE) {
          if (Idx1 < 4) {
            RootSocId = DiscEngData->Rings[0][Idx1];
          } else {
            break;
          }
        } else {
          RootSocId = Idx1;
        }

        if (RootSocId >= MAX_SOCKET) {
          KTI_ASSERT ((RootSocId < MAX_SOCKET), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_CPU_SOCKET_ID);
          return KTI_FAILURE;
        }

        if ((DiscEngData->Cpu[RootSocId].Valid == TRUE) && (DiscEngData->Cpu[RootSocId].SocType == SOCKET_TYPE_CPU)) {
          for (Idx2 = 0; DiscEngData->CpuTree[RootSocId][Idx2].Node.Valid != FALSE; Idx2++) {
            Parent = DiscEngData->CpuTree[RootSocId][Idx2];
            if (Parent.Node.Hop < Hop) {
              continue;
            } else if (Parent.Node.Hop > Hop) {
              break;
            }
            ExploreThisNode (KtiInternalGlobal, DiscEngData, RootSocId, &Parent, Idx2, Hop);
          }
        }
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  Computes the KTI CPU topology found in the system

  @param KtiInternalGlobal - KTIRC internal variables.
  @param DiscEngData       - Pointer to socket specific data
  @param RootSocId         - Root Socket ID
  @param Parent            - KTI fabric tree structure
  @param ParentIdx         - parent ID
  @param Hop               - Hops in path

  @retval 0     - Success
  @retval Other - Failure

**/
STATIC
KTI_STATUS
ExploreThisNode (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  KTI_DISC_DATA             *DiscEngData,
  UINT8                     RootSocId,
  TOPOLOGY_TREE_NODE        *Parent,
  UINT8                     ParentIdx,
  UINT8                     Hop
  )
{
  TOPOLOGY_TREE_NODE  Child;
  UINT8               KtiLink;
  KTI_LINK_DATA       ParentLep;
  UINT8               ParentNodeId;

  ParentNodeId = (UINT8) Parent->Node.SocId;
  Child.RawNode    = 0;
  Child.Node.Valid = TRUE;
  Child.Node.Hop = Parent->Node.Hop + 1; // One level down from parent
  Child.Node.ParentIndex = ParentIdx; // Remember who is the immediate parent of this child

  if (RootSocId != ParentNodeId) {
    ParentLep = DiscEngData->Cpu[ParentNodeId].LinkData[Parent->Node.ParentPort];
    if (ParentLep.DisallowRouteThru == TRUE) {
      //
      // Incoming port on Parent Node (i.e. Intermediate Node) doesn't support
      // route-through path. Therefore, skip exploring further on this node.
      //
      return KTI_SUCCESS;
    }
  }

  for (KtiLink = 0; KtiLink < KtiInternalGlobal->KtiPortCnt; KtiLink++) { // Explore all the links of this parent
    ParentLep = DiscEngData->Cpu[ParentNodeId].LinkData[KtiLink];
    //
    // Skip this link if it is marked as "Not Valid" or "DualLink" or "TwoSkt3Link".
    // A Link is marked as "Not Valid" (before entering this function) when we don't
    // want to explore it. Also, for SV validation purpose, we might want to mark a link as "Not Valid"
    // to simulate different topology. When there is a dual-link exists between any two CPU sockets,
    // one of the link is marked as "DualLink" at both the ends.
    // Exploring only one link of a dual-link is sufficient to identify the neighbor.
    //
    if ((ParentLep.Valid != TRUE) || (ParentLep.TwoSkt3Link == TRUE) || (ParentLep.DualLink == TRUE)) {
      continue;
    }

    //
    // Skip this link (outgoing) on the intermediate Node if the link doesn't support route-through traffic
    //
    if ((Child.Node.Hop > 1) && (ParentLep.DisallowRouteThru == TRUE)) {
      continue;
    }

    //
    // Get the Child Socket Id, Type and port that connects it to the parent form LEP
    //
    Child.Node.SocId      = ParentLep.PeerSocId;
    Child.Node.SocType    = ParentLep.PeerSocType;
    Child.Node.ParentPort = ParentLep.PeerPort;

    //
    // Make sure that this socket is not already found in the tree between hop level 0 and parent's hop level.
    // The rational is that, if we find child socket already at that level, then we have a shorter path to this child.
    //
    if (ShorterPathExists (DiscEngData->CpuTree[RootSocId], &Child) == TRUE) {
      continue;
    }

    //
    // Finally, if we have multiple paths that are of equal hop length, then choose
    // the one that is used least number of times; if they are used equal number of times, then
    // choose the one that goes thru the minimum numberd socket.
    //
    if (CheckAndAddNodeToTopologyTree (KtiInternalGlobal, DiscEngData, RootSocId, Parent, &Child, Hop) != KTI_SUCCESS) {
      KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n  Adding CPU%u to Topology Discovery Tree of CPU%u failed.",
                            Child.Node.SocId, RootSocId));
      KTI_ASSERT (FALSE, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_5B);
    }
  } // for(KtiLink...)

  return KTI_SUCCESS;
}

/**
  Add additional route found during topology discovery to Alternate routes data structure.

  @param[in] KtiInternalGlobal   KTIRC internal variables.
  @param[in,out] DiscEngData     Pointer to topology info structure consumed by discovery engine.
  @param[in] RootSocId           Root Socket ID
  @param[in] LeafNode            The new tree node found in alternate path.
  @param[in] TreeIndex           Index of the tree node in topology tree.

  @retval KTI_SUCCESS  The new node was added to alternate routes data structure.
  @retval KTI_FAILURE  Unable to add the new node as Alternate Routes table has been fully consumed.

**/
KTI_STATUS
AddNodeToAlternateTopologyTree (
  IN     KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN OUT KTI_DISC_DATA            *DiscEngData,
  IN     UINT8                    RootSocId,
  IN     TOPOLOGY_TREE_NODE       *LeafNode,
  IN     UINT8                    TreeIndex
  )
{
  if (DiscEngData->AlternateCpuTree[RootSocId][TreeIndex].Node.Valid == TRUE) {
    //
    // The alternate path table has already been filled.
    //
    return KTI_FAILURE;
  }

  DiscEngData->AlternateCpuTree[RootSocId][TreeIndex] = *LeafNode;

  return KTI_SUCCESS;
}

/**
  Adds current KTI node to KTI system fabric

  @param KtiInternalGlobal - KTIRC internal variables.
  @param DiscEngData       - Pointer to socket specific data
  @param RootSocId         - Root Socket ID
  @param Parent            - KTI fabric tree structure
  @param LeafNode          - KTI fabric tree structure
  @param Hop               - Hops in path

  @retval 0 - Successful
  @retval 1 - Failure

**/
STATIC
KTI_STATUS
CheckAndAddNodeToTopologyTree (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_DISC_DATA            *DiscEngData,
  UINT8                    RootSocId,
  TOPOLOGY_TREE_NODE       *Parent,
  TOPOLOGY_TREE_NODE       *LeafNode,
  UINT8                    Hop
  )
{
  UINT8                 Idx1;
  UINT8                 Path1Usage, Path2Usage;
  TOPOLOGY_TREE_NODE    *Tree;
  BOOLEAN               AddToAlternateTree;
  TOPOLOGY_TREE_NODE    TreeParent;
  BOOLEAN               PassThruTrafficUseDiffVn;
  UINT8                 ParentSocId;
  UINT8                 Port;
  UINT8                 RootSocVn;

  KTI_ASSERT ((DiscEngData != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_56);
  KTI_ASSERT ((Parent != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_57);
  KTI_ASSERT ((LeafNode != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_58);

  if ((DiscEngData == NULL) || (LeafNode == NULL) || (Parent == NULL)) {
    return KTI_FAILURE;
  }

  Tree = DiscEngData->CpuTree[RootSocId];
  AddToAlternateTree = FALSE;
  PassThruTrafficUseDiffVn = FALSE;
  ParentSocId = (UINT8) Parent->Node.SocId;
  RootSocVn = DiscEngData->Cpu[RootSocId].Vn;

  if ((KtiInternalGlobal->EightSocketTopology == Topo8STwistedFullyConnectCubes) && (KTIVAR->SysConfig == SYS_CONFIG_8S)) {
    //
    // Source traffic uses different VN than pass through traffic at a given socket
    // for better QoS for the route-through traffic.
    //
    PassThruTrafficUseDiffVn = TRUE;

    if (Hop != 0) {
      Port = GetChildLocalPortFromLEP (KtiInternalGlobal, DiscEngData->Cpu, Tree, Parent);
      if (IsUpiPortWithOppositeRootVn (KtiInternalGlobal, DiscEngData->Cpu, RootSocId, Port)) {
        RootSocVn = (RootSocVn == VN0) ? VN1 : VN0;
      }
    }
  }

  for (Idx1 = 0; Idx1 < MAX_TREE_NODES; Idx1++) {
    //
    // If the LeafNode is not already found in the tree, add it to the tree
    //
    if (Tree[Idx1].Node.Valid == FALSE) {
      Tree[Idx1] = *LeafNode;
      if (Hop != 0) {
        TrackPathLoad (KtiInternalGlobal, DiscEngData, Tree, LeafNode, 0);
      }
      return KTI_SUCCESS;
    }

    //
    // If Socket ID, Type & Hop level of the node to be added matches a node in the tree
    // (i.e the LeafNode is already in the tree with different path to the root), then choose
    // the path that is used minimum number of times.
    //
    if (Tree[Idx1].Node.SocId   == LeafNode->Node.SocId   &&
        Tree[Idx1].Node.SocType == LeafNode->Node.SocType &&
        Tree[Idx1].Node.Hop     == LeafNode->Node.Hop) {

      //
      // Find the immediate parent of the leaf nodes and check how many times the immediate
      // parent used the link that connects it to the leaf node
      //
      Path1Usage = CalculatePathLoad (KtiInternalGlobal, DiscEngData, Tree, &Tree[Idx1]);

      Path2Usage = CalculatePathLoad (KtiInternalGlobal, DiscEngData, Tree, Parent) + (UINT8)LeafNode->Node.Hop;

      if ((KTIVAR->SysConfig == SYS_CONFIG_8S) && (KtiInternalGlobal->EightSocketTopology == Topo8SAlmostFullyConnect)) {
        AddToAlternateTree = TRUE;
      } else if ((KTIVAR->SysConfig == SYS_CONFIG_4S) && (KtiInternalGlobal->FpgaList != 0)) {
        AddToAlternateTree = TRUE;
      }

      //
      // Select the path that is used minimum number of times.
      //
      if (Path2Usage < Path1Usage) {
        if (PassThruTrafficUseDiffVn && (RootSocVn == DiscEngData->Cpu[ParentSocId].Vn)) {
          //
          // If the new path (route-through traffic) is using the same VN as the traffic originated
          // from RootSocId; don't replace the existing node with this new leaf node
          //
          return KTI_SUCCESS;
        }

        //
        // Store the additional path in Alternate Routes table.
        //
        if (AddToAlternateTree) {
          AddNodeToAlternateTopologyTree (KtiInternalGlobal, DiscEngData, RootSocId, &Tree[Idx1], Idx1);
        }

        //
        // Replace the existing node with the leaf node
        //
        TrackPathLoad (KtiInternalGlobal, DiscEngData, Tree, &Tree[Idx1], 1);
        Tree[Idx1] = *LeafNode;
        TrackPathLoad (KtiInternalGlobal, DiscEngData, Tree, LeafNode, 0);
        return KTI_SUCCESS;
      } else {
        //
        // Existing path is optimum. Leave the tree intact unless optimization is required where
        // route through traffic and source traffic must use different VN.
        //


        if (PassThruTrafficUseDiffVn) {
          GetParentSocket (Tree, &Tree[Idx1], &TreeParent);
          if (TreeParent.Node.Valid == FALSE) {
            return KTI_SUCCESS;
          }

          RootSocVn = DiscEngData->Cpu[RootSocId].Vn;
          Port = GetChildLocalPortFromLEP (KtiInternalGlobal, DiscEngData->Cpu, Tree, &TreeParent);
          if (IsUpiPortWithOppositeRootVn (KtiInternalGlobal, DiscEngData->Cpu, RootSocId, Port)) {
            RootSocVn = (RootSocVn == VN0) ? VN1 : VN0;
          }

          ParentSocId = (UINT8) TreeParent.Node.SocId;
          if (DiscEngData->Cpu[ParentSocId].Vn == RootSocVn) {
            //
            // Replace the existing node with the leaf node only if the existing node is using
            // the same VN as source traffic.
            //
            TrackPathLoad (KtiInternalGlobal, DiscEngData, Tree, &Tree[Idx1], 1);
            Tree[Idx1] = *LeafNode;
            TrackPathLoad (KtiInternalGlobal, DiscEngData, Tree, LeafNode, 0);
            return KTI_SUCCESS;
          }
        }

        //
        // Store the additional path in Alternate routes table if needed.
        //
        if (AddToAlternateTree) {
          AddNodeToAlternateTopologyTree (KtiInternalGlobal, DiscEngData, RootSocId, LeafNode, Idx1);
        }

        return KTI_SUCCESS;
      }
    }
  }

  return (KTI_STATUS) KTI_FAILURE;
}

/**
  Checks and resolves potential dead locks due to overlapping routes

  @param KtiInternalGlobal - KTIRC internal variables.
  @param DiscEngData       - Pointer to socket specific data
  @param Tree              - KTI fabric tree structure
  @param LeafNode          - KTI fabric tree structure
  @param Adjust            - 1 = Remove, 0 = Add to linkload

  @retval 0 - Successful
  @retval 1 - Failure

**/
STATIC
KTI_STATUS
TrackPathLoad (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_DISC_DATA            *DiscEngData,
  TOPOLOGY_TREE_NODE       *Tree,
  TOPOLOGY_TREE_NODE       *LeafNode,
  UINT8                    Adjust
  )
{
  UINT8               Port0; // Parent Port that connects it to the child
  TOPOLOGY_TREE_NODE  Child, Parent;

  Child = *LeafNode;
  while (Child.Node.Hop != 0) {
    Parent = Tree[Child.Node.ParentIndex];
    Port0 = GetChildLocalPortFromLEP (KtiInternalGlobal, DiscEngData->Cpu, &Parent, &Child);
    if ((Port0 == 0xFF) ||
        (Port0 >= (KtiInternalGlobal->KtiPortCnt)) ||
        (Port0 >= MAX_FW_KTI_PORTS)) {
      KTI_ASSERT ((Port0 < (KtiInternalGlobal->KtiPortCnt)), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_59);
      break;
    }
    if (Adjust == 0) {
      DiscEngData->LinkLoad[Parent.Node.SocId][Port0]++;
    } else {
      KTI_ASSERT ((DiscEngData->LinkLoad[Parent.Node.SocId][Port0] != 0), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_5C);
      DiscEngData->LinkLoad[Parent.Node.SocId][Port0]--;
    }
    Child = Parent;
  }

  return KTI_SUCCESS;
}

/**
  Checks and resolves potential dead locks due to overlapping routes

  @param KtiInternalGlobal - KTIRC internal variables.
  @param DiscEngData       - Pointer to socket specific data
  @param Tree              - KTI fabric tree structure
  @param LeafNode          - KTI fabric tree structure

**/
STATIC
UINT8
CalculatePathLoad (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_DISC_DATA              *DiscEngData,
  TOPOLOGY_TREE_NODE         *Tree,
  TOPOLOGY_TREE_NODE         *LeafNode
  )
{
  UINT8               Port0; // Parent Port that connects it to the child
  UINT8               Usage;
  TOPOLOGY_TREE_NODE  Child, Parent;

  Usage = 0;
  Child = *LeafNode;
  while (Child.Node.Hop != 0) {
    Parent = Tree[Child.Node.ParentIndex];
    Port0 = GetChildLocalPortFromLEP (KtiInternalGlobal, DiscEngData->Cpu, &Parent, &Child);
    if ((Port0 == 0xFF) ||
        (Port0 >= (KtiInternalGlobal->KtiPortCnt)) ||
        (Port0 >= MAX_FW_KTI_PORTS)) {
      KTI_ASSERT ((Port0 < (KtiInternalGlobal->KtiPortCnt)), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_5A);
      break;
    }
    Usage = Usage + DiscEngData->LinkLoad[Parent.Node.SocId][Port0];
    Child = Parent;
  }

  return Usage;
}

/**
  Treat the topology tree as a Directed Graph and check if the Graph is Directed Acylic Graph (DAG)
  or not using "Topological Sort" algorithm. If the graph is acyclic it indicates the routes are deadlock free.
  The Directed Graph consists of finite set of vertices V and Edges E. In KTI terminology, source/sink of
  KTI packets represent Vertices and the link connecting them represent Edges. So V & E for all
  supported topology is finite set as defined below:

      V = {S0...S7}{P0...P2}{VN0/VN1}{TX/RX} = 8 * 3 * 2 * 2 = 96
      E = {S0...S7}{P0...P2}{VN0/VN1} = 8 * 3 * 2 = 48

  The topology tree is converted into an array consisting the list of adjacent nodes of each node. Since
  each node has 3 links and 2 VNs, a node can have maximum 6 outgoing edges; so it can have maximum
  of 6 nodes in its adjacency list.

  Example: Consider the 4S ring topology

       0   0
    S0 ----- S1
    1|       |1
     |       |
     |       |
     |       |
    1|       |1
    S3 ----- S2
       0   0

  The route/topology tree for each socket would be:
    "S0 P0 VN0 TX" -> "S1 P0 VN0 RX";
    "S0 P1 VN0 TX" -> "S3 P1 VN0 RX";
    "S0 P0 VN0 TX" -> "S1 P0 VN0 RX" -> "S1 P1 VN0 TX" -> "S2 P1 VN0 RX";

    "S1 P0 VN0 TX" -> "S0 P0 VN0 RX";
    "S1 P1 VN0 TX" -> "S2 P1 VN0 RX";
    "S1 P0 VN0 TX" -> "S0 P0 VN0 RX" -> "S0 P1 VN0 TX" -> "S3 P1 VN0 RX";

    "S2 P0 VN0 TX" -> "S3 P0 VN0 RX";
    "S2 P1 VN0 TX" -> "S1 P1 VN0 RX";
    "S2 P0 VN0 TX" -> "S3 P0 VN0 RX" -> "S3 P1 VN0 TX" -> "S0 P1 VN0 RX";

    "S3 P0 VN0 TX" -> "S2 P0 VN0 RX";
    "S3 P1 VN0 TX" -> "S0 P1 VN0 RX";
    "S3 P0 VN0 TX" -> "S2 P0 VN0 RX" -> "S2 P1 VN0 TX" -> "S1 P1 VN0 RX";

  The above topology tree can be represented as an array of adjacency list as shown below:
    Node           Adjacency List
    S0 P0 VN0 TX   S1 P0 VN0 RX
    S0 P0 VN0 RX   S0 P1 VN0 TX
    S0 P1 VN0 TX   S3 P1 VN0 RX
    S1 P0 VN0 TX   S0 P0 VN0 RX
    S1 P0 VN0 RX   S1 P1 VN0 TX
    S1 P1 VN0 TX   S2 P1 VN0 RX
    S2 P0 VN0 TX   S3 P0 VN0 RX
    S2 P0 VN0 RX   S2 P1 VN0 TX
    S2 P1 VN0 TX   S1 P1 VN0 RX
    S3 P0 VN0 TX   S2 P0 VN0 RX
    S3 P0 VN0 RX   S3 P1 VN0 TX
    S3 P1 VN0 TX   S0 P1 VN0 RX

  While this particular topology has only one adjacent node for each node, complex topologies such as 8SG will
  have more than one node in the adjacency list.

  By applying the topological sort on the above table, it can be detected if it is DAG or not.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param DiscEngData       - Pointer to socket specific data

  @retval 0     - Success
  @retval Other - Failure

**/
STATIC
KTI_STATUS
DetectDeadlock (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_DISC_DATA              *DiscEngData
  )
{
  UINT8               Idx1, Idx2, Idx3;
  UINT8               GrandParentIdx, ParentIdx, ChildIdx;
  UINT8               Port1, Port2;
  UINT8               RootVn, Temp1;
  UINT8               Vertices[MAX_SOCKET * MAX_FW_KTI_PORTS * 2 * 2];
  UINT8               AdjList[MAX_SOCKET * MAX_FW_KTI_PORTS * 2 * 2][MAX_FW_KTI_PORTS * 2];
  UINT8               Indegree[MAX_SOCKET * MAX_FW_KTI_PORTS * 2 * 2];
  BOOLEAN             DeadlockFound;
  TOPOLOGY_TREE_NODE  *TopologyTree;
  TOPOLOGY_TREE_NODE  *Parent, *Child;
  UINT8               MaxVertices;

  ZeroMem ((VOID *) Vertices, sizeof (Vertices));
  SetMem ((VOID *) AdjList, sizeof (AdjList), 0xFF);
  ZeroMem ((VOID *) Indegree, sizeof (Indegree));

  MaxVertices = sizeof (Vertices) / sizeof (Vertices[0]);

  //
  // Construct the adjacency list. Note that the pass thru route is split into two separate independent nodes;
  // one containing the RX and the other containing the TX at the intermediate node. The list is contructed
  // by parsing the topology tree of each socket. In other words, the graph is nothing but collection of the
  // topology tree of each node.
  //
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (DiscEngData->Cpu[Idx1].Valid == TRUE) {
      //
      // VN assignment is at the soure (root node) and they don't change along the path. Identify the
      // root node's VN assignment.
      //
      TopologyTree = DiscEngData->CpuTree[Idx1];
      RootVn = DiscEngData->Cpu[TopologyTree[0].Node.SocId].Vn;

      //
      // Traverse the topology tree of this socket and mark the Vertices found in its tree.
      //
      for (Idx2 = 1; TopologyTree[Idx2].Node.Valid != FALSE; Idx2++) {
        //
        // Extract the Parent and Child nodes
        //
        Child = &TopologyTree[Idx2];
        if (Child == NULL) {
          KTI_ASSERT ((Child != NULL), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
          return KTI_FAILURE;
        }

        Port2 = (UINT8) Child->Node.ParentPort; // Child's Port that connects it to the parent
        if ((Port2 >= (KtiInternalGlobal->KtiPortCnt)) || (Child->Node.SocId >= MAX_SOCKET)) {
          KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
          return KTI_FAILURE;
        }

        Port1 = (UINT8) DiscEngData->Cpu[Child->Node.SocId].LinkData[Port2].PeerPort; // Parent's Port that connects it to this child

        Parent = &TopologyTree[Child->Node.ParentIndex];
        if (Parent == NULL) {
          KTI_ASSERT ((Parent != NULL), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
          return KTI_FAILURE;
        }

        //
        // Handling the UPI port that is assigned with opposite VN of what was assigned at the Root Node
        //
        if (IsUpiPortWithOppositeRootVn (KtiInternalGlobal, DiscEngData->Cpu, (UINT8) Parent->Node.SocId, Port1)) {
          RootVn = (RootVn == VN0)? VN1 : VN0;
        }

        //
        // Mark the corresponding vertices of the parent & child as valid.
        // Note that the parent is always at "TX" node and the child is always at "RX" node.
        //
        ParentIdx = (UINT8)(Parent->Node.SocId * (KtiInternalGlobal->KtiPortCnt) * 2 * 2) + (Port1 * 2 * 2) + (RootVn * 2) + TX;
        ChildIdx = (UINT8)(Child->Node.SocId * (KtiInternalGlobal->KtiPortCnt) * 2 * 2) + (Port2 * 2 * 2) + (RootVn * 2) + RX;
        if ((ParentIdx >= MaxVertices) || (ChildIdx >= MaxVertices)) {
          KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
          return KTI_FAILURE;
        }
        Vertices[ParentIdx] = TRUE;
        Vertices[ChildIdx] = TRUE;

        //
        // Record the child as adjacent node in parent's adjacency list if it is not already found.
        //
        for (Idx3 = 0; Idx3 < KtiInternalGlobal->KtiPortCnt * 2; Idx3++) {
          if (AdjList[ParentIdx][Idx3] == ChildIdx) {
            break;
          } else if (AdjList[ParentIdx][Idx3] == 0xFF) {
            AdjList[ParentIdx][Idx3] = ChildIdx;
            break;
          }
        }

        //
        // If the parent is not the root node, make sure that the pass thru route is split into separate nodes corresponding
        // to RX & TX and recorded in the adjacency list.
        //
        if (Parent->Node.Hop != 0) {
          Port1 = (UINT8)Parent->Node.ParentPort; // Port connecting the parent to grand parent
          GrandParentIdx = (UINT8)(Parent->Node.SocId * (KtiInternalGlobal->KtiPortCnt) * 2 * 2) + (Port1 * 2 * 2) + (RootVn * 2) + RX;
          for (Idx3 = 0; Idx3 < KtiInternalGlobal->KtiPortCnt * 2; Idx3++) {
            if (AdjList[GrandParentIdx][Idx3] == ParentIdx) {
              break;
            } else if (AdjList[GrandParentIdx][Idx3] == 0xFF) {
              AdjList[GrandParentIdx][Idx3] = ParentIdx;
              break;
            }
          }
        }

      }
    }
  }


  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\n Adjacency Table"));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n ----------------"));
  for (Idx1 = 0; Idx1 < MAX_SOCKET * (KtiInternalGlobal->KtiPortCnt) * 2 * 2; Idx1++) {
    if (Vertices[Idx1] == TRUE) {
      Temp1 = Idx1 % ((KtiInternalGlobal->KtiPortCnt) * 2 * 2);
      Port1 = Temp1 / (2 * 2);
      RootVn = (Temp1 % (2 * 2)) / 2;
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nS%u P%u VN%u %a (%02u) :", Idx1 / ((KtiInternalGlobal->KtiPortCnt) * 2 * 2), Port1, RootVn, (Idx1 & 0x1) ? "RX" : "TX", Idx1));
      for (Idx2 = 0; Idx2 < KtiInternalGlobal->KtiPortCnt * 2; Idx2++) {
        if (AdjList[Idx1][Idx2] != 0xFF) {
          Temp1 = AdjList[Idx1][Idx2] % ((KtiInternalGlobal->KtiPortCnt) * 2 * 2);
          Port2 = Temp1 / (2 * 2);
          RootVn = (Temp1 % (2 * 2)) / 2;
          KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "   S%u P%u VN%u %a (%02u)", AdjList[Idx1][Idx2] / ((KtiInternalGlobal->KtiPortCnt) * 2 * 2), Port2, RootVn,
              (AdjList[Idx1][Idx2] & 0x1) ? "RX" : "TX", AdjList[Idx1][Idx2]));
        }
      }
    }
  }


  //
  // Do topological sort of the graph. A topological sort is an ordering of the vertices in a directed graph
  // such that if there is a path from node i to j, then node j appears after node i in the ordering.
  // If there is a cycle in the graph then such ordering is not possible and it indicates a deadlock in the route.
  // Note that we are not interested in doing the actual sorting. We just knock out the vertices with indegree
  // 0 and repeat the loop for all nodes. At the end if we find any node with non-zero indegree, it implies
  // there is a cycle in the graph. Also note that the list of nodes in the array is nothing but the nodes that
  // constitue the cycle in the graph.
  //

  //
  // Construct a table containing the indgree of each node.
  //
  for (Idx1 = 0; Idx1 < MAX_SOCKET * (KtiInternalGlobal->KtiPortCnt) * 2 * 2; Idx1++) {
    if (Vertices[Idx1] == TRUE) {
      for (Idx3 = 0; Idx3 < KtiInternalGlobal->KtiPortCnt * 2; Idx3++) {
        if (AdjList[Idx1][Idx3] == 0xFF) {
          break;
        } else {
          Indegree[AdjList[Idx1][Idx3]]++;
        }
      }
    }
  }


  //
  // Remove all nodes with indegree zero and all its outgoing edges by decreasing the indegree of the nodes
  // that are connected by the edges being removed.
  //
  for (Idx1 = 0; Idx1 < MAX_SOCKET * (KtiInternalGlobal->KtiPortCnt) * 2 * 2; Idx1++) {
    for (Idx2 = 0; Idx2 < MAX_SOCKET * (KtiInternalGlobal->KtiPortCnt) * 2 * 2; Idx2++) {
      if (Vertices[Idx2] == TRUE && Indegree[Idx2] == 0) {
        Vertices[Idx2] = FALSE;
        for (Idx3 = 0; Idx3 < KtiInternalGlobal->KtiPortCnt * 2; Idx3++) {
          if (AdjList[Idx2][Idx3] == 0xFF) {
            break;
          } else {
            Indegree[AdjList[Idx2][Idx3]]--;
          }
        }
      }
    }
  }

  //
  // Check for cycle and print the nodes that form the cycle. If we find any valid node in the vertices it indicates a cycle.
  //
  DeadlockFound = FALSE;
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\n Checking for Deadlock..."));
  for (Idx1 = 0; Idx1 < MAX_SOCKET * (KtiInternalGlobal->KtiPortCnt) * 2 * 2; Idx1++) {
    if (Vertices[Idx1] == TRUE) {
      DeadlockFound = TRUE;
      for (Idx2 = 0; Idx2 < KtiInternalGlobal->KtiPortCnt * 2; Idx2++) {
        if (AdjList[Idx1][Idx2] == 0xFF) {
          break;
        } else {
          Temp1 = Idx1 % ((KtiInternalGlobal->KtiPortCnt) * 2 * 2);
          Port1 = Temp1 / (2 * 2);
          RootVn = (Temp1 % (2 * 2)) / 2;
          KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nS%u P%u VN%u %a ->  ", Idx1 / ((KtiInternalGlobal->KtiPortCnt) * 2 * 2), Port1, RootVn, (Idx1 & 0x1) ? "RX" : "TX"));
          Temp1 = AdjList[Idx1][Idx2] % ((KtiInternalGlobal->KtiPortCnt) * 2 * 2);
          Port2 = Temp1 / (2 * 2);
          RootVn = (Temp1 % (2 * 2)) / 2;
          KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "S%u P%u VN%u %a", AdjList[Idx1][Idx2] / ((KtiInternalGlobal->KtiPortCnt) * 2 * 2), Port2, RootVn, (AdjList[Idx1][Idx2] & 0x1) ? "RX" : "TX"));
        }
      }
    }
  }

  if (DeadlockFound == TRUE) {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\n Dead Lock Found!!! \n Topology Not Supported. System Halted\n"));
    KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
  }

  return KTI_SUCCESS;
}

/**
  Copies the Si agnostic toplogy tree created by the discovery engine and apply the port mapping to make Si specific.
  Also copies the VN assigned, and the LEP Info and Topology Tree Info into Host structure

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC internal variables.
  @param DiscEngData       - Pointer to socket specific data

  @retval 0     - Success
  @retval Other - Failure

**/
STATIC
KTI_STATUS
CopyTopologyTree (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_DISC_DATA              *DiscEngData
  )
{
  UINT8               Idx1, Idx2;
  UINT8               TempPort;

  //
  // Copy the Si agnostic toplogy tree created by the discovery engine and apply the port mapping to make Si specific.
  //
  CopyMem ((UINT8 *) SocketData->CpuTree, (UINT8 *) DiscEngData->CpuTree, sizeof (SocketData->CpuTree));
  CopyMem ((UINT8 *) SocketData->AlternateCpuTree, (UINT8 *) DiscEngData->AlternateCpuTree, sizeof (SocketData->AlternateCpuTree));

  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if ((DiscEngData->Cpu[Idx1].Valid == TRUE) && (DiscEngData->Cpu[Idx1].SocType == SOCKET_TYPE_CPU)) {
      for (Idx2 = 1; DiscEngData->CpuTree[Idx1][Idx2].Node.Valid == TRUE; Idx2++) { // Skip the root node since it doesn't have parent
        TempPort = (UINT8) DiscEngData->CpuTree[Idx1][Idx2].Node.ParentPort;
        SocketData->CpuTree[Idx1][Idx2].Node.ParentPort = TranslateToSiPort (TempPort);

        if (KtiInternalGlobal->CpuType == CPU_CPX &&
            DiscEngData->AlternateCpuTree[Idx1][Idx2].Node.Valid == TRUE) {
          TempPort = (UINT8) DiscEngData->AlternateCpuTree[Idx1][Idx2].Node.ParentPort;
          SocketData->AlternateCpuTree[Idx1][Idx2].Node.ParentPort = TranslateToSiPort (TempPort);
        }
      }
    }
  }

  // Copy the VN assigned
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (SocketData->Cpu[Idx1].Valid == TRUE) {
      SocketData->Cpu[Idx1].Vn = DiscEngData->Cpu[Idx1].Vn;
    }
  }

  // Copy the LEP Info and Topology Tree Info into Host structure
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (SocketData->Cpu[Idx1].Valid == TRUE) {
      KTIVAR->CpuInfo[Idx1].Valid  = SocketData->Cpu[Idx1].Valid;
      KTIVAR->CpuInfo[Idx1].SocId  = SocketData->Cpu[Idx1].SocId;
      KTIVAR->CpuInfo[Idx1].SocType  = SocketData->Cpu[Idx1].SocType;
      CopyMem ((UINT8 *) KTIVAR->CpuInfo[Idx1].LepInfo, (UINT8 *) SocketData->Cpu[Idx1].LinkData, sizeof (KTIVAR->CpuInfo[Idx1].LepInfo));
      CopyMem ((UINT8 *) KTIVAR->CpuInfo[Idx1].TopologyInfo, (UINT8 *) SocketData->CpuTree[Idx1], sizeof (KTIVAR->CpuInfo[Idx1].TopologyInfo));
    }
  }

  TopologyInfoDump (KtiInternalGlobal, SocketData->Cpu, SocketData->CpuTree, SocketData->AlternateCpuTree, TRUE);

  return KTI_SUCCESS;
}

//
// All the following routines are Si specific and needs to be ported based on Si capability
//
/**
  Calculate routing for socket

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval 0 - Success
  @retval Other - Failure

**/
STATIC
KTI_STATUS
CalculateRT (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  TOPOLOGY_TREE_NODE  TopologyTree[MAX_TREE_NODES];
  TOPOLOGY_TREE_NODE  *CpuTree;
  TOPOLOGY_TREE_NODE  Child, Parent;
  UINT8               Idx1, Idx2;
  UINT8               KtiLink;
  KTI_LINK_DATA       ParentLep;
  BOOLEAN             FoundInAlternateTree;
  UINT8               SupportedSocketType;

  SupportedSocketType = (1 << SOCKET_TYPE_CPU) | (1 << SOCKET_TYPE_FPGA);

  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if ((SocketData->Cpu[Idx1].Valid == TRUE) && (SocketData->Cpu[Idx1].SocType == SOCKET_TYPE_CPU)) {

      CpuTree = SocketData->CpuTree[Idx1];
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n Calculating Route for CPU%u ", CpuTree[0].Node.SocId));
      KtiCheckPoint ((UINT8) (1 << CpuTree[0].Node.SocId), (UINT8) CpuTree[0].Node.SocType, 0, STS_TOPOLOGY_DISCOVERY, MINOR_STS_CALCULATE_ROUTE);

      for (Idx2 = 0; ((CpuTree[Idx2].Node.Valid != FALSE) && ((SupportedSocketType & (1 << CpuTree[Idx2].Node.SocType)) != 0)); Idx2++) {

        FoundInAlternateTree    = FALSE;
        Parent            = CpuTree[Idx2];

        Child.RawNode = 0;
        Child.Node.Valid  = TRUE;
        Child.Node.Hop    = Parent.Node.Hop + 1; // One level down from parent
        Child.Node.ParentIndex = Idx2;

        if ((SupportedSocketType & (1 << Parent.Node.SocType)) != 0) {
          for (KtiLink = 0; KtiLink < KtiInternalGlobal->KtiPortCnt; KtiLink++) {

            ParentLep = SocketData->Cpu[Parent.Node.SocId].LinkData[KtiLink];

            //
            // Skip the Invalid/Duplicate Links/3 links/4 links
            //
            if ((ParentLep.Valid != TRUE) ||
                (ParentLep.TwoSkt3Link == TRUE) ||
                (ParentLep.TwoSkt4Link == TRUE) ||
                (ParentLep.DualLink == TRUE)) {
              continue;
            }

            //
            // Get the Child's socket Id, Type and parent port from LEP
            //
            Child.Node.SocId      = ParentLep.PeerSocId;
            Child.Node.SocType    = ParentLep.PeerSocType;
            Child.Node.ParentPort = ParentLep.PeerPort;

            //
            // Make a copy of topology tree CpuTree for all tree nodes on this socket
            //
            CopyMem ((UINT8 *) TopologyTree, (UINT8 *)SocketData->CpuTree[Idx1], sizeof (SocketData->CpuTree[Idx1]));

            //
            // Check that this Child is found in the topology tree
            //
            if (NodeFoundInTopologyTree (KtiInternalGlobal, TopologyTree, SocketData->AlternateCpuTree[Idx1], &Parent, &Child, &FoundInAlternateTree)) {
              //
              // Setup the route including snoop fanout route.
              //
              SetupNonBCRoute (SocketData, KtiInternalGlobal, TopologyTree, &TopologyTree[0], FoundInAlternateTree, &Parent, &Child);

              if (KtiInternalGlobal->SnoopFanoutEn == TRUE && !FoundInAlternateTree) {
                if (KtiInternalGlobal->Is4SRing && (TopologyTree[0].Node.SocId != Child.Node.SocId && Parent.Node.Hop == 1)) {
                  //
                  // Check if 4S ring and the socket is not Root socket and hop is 1 only (i.e interm socket)
                  //
                  SetupBCRoute (SocketData, KtiInternalGlobal, TopologyTree, &TopologyTree[0], &Parent, &Child, TRUE);
                } else {
                  SetupBCRoute (SocketData, KtiInternalGlobal, TopologyTree, &TopologyTree[0], &Parent, &Child, FALSE);
                }
              }
            }
          } // for(KtiLink...)
        } else {
          //
          // This is a fatal error
          //
          KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Unknown Socket Type found in CPU%u TopologyTree. Topology not supported.\n", TopologyTree[0].Node.SocId));
          KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE);
        }
      }
    }
  }

  // Optimize the Cha from sending snoop to the requester in snoop fanout mode.
  if (KtiInternalGlobal->SnoopFanoutEn == TRUE) {
    OptimizeChaFanoutSnoop (KtiInternalGlobal, SocketData);
  }

  RouteTableDump (SocketData, KtiInternalGlobal);

  return KTI_SUCCESS;
}

/**
  Setup route for node

  @param SocketData           - Pointer to socket specific data
  @param KtiInternalGlobal    - KTIRC internal variables.
  @param TopologyTree         - Pointer to full system KTI fabric tree
  @param Root                 - Pointer to root KTI node
  @param FoundInAlternateTree - TRUE if the node is found in Alternate routes table; FALSE otherwise.
  @param Interm               - Pointer to intermediate KTI node
  @param Dest                 - Pointer to destination KTI node

  @retval 0 - Successful
  @retval 1 - Failure

**/
STATIC
KTI_STATUS
SetupNonBCRoute (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  TOPOLOGY_TREE_NODE         *TopologyTree,
  TOPOLOGY_TREE_NODE         *Root,
  BOOLEAN                    FoundInAlternateTree,
  TOPOLOGY_TREE_NODE         *Interm,
  TOPOLOGY_TREE_NODE         *Dest
  )
{
  TOPOLOGY_TREE_NODE   Parent;
  TOPOLOGY_TREE_NODE   Child;
  TOPOLOGY_TREE_NODE   Grandparent;
  UINT8                cLocalPort[4];
  UINT8                pLocalPort1;
  UINT8                RootVn;
  KTI_CPU_SOCKET_DATA  *Cpu;
  KTI_STATUS           Status;
  UINT8                ParentNodeId;
  BOOLEAN              DualOrTripleLink;

  Status = KTI_SUCCESS;
  Cpu = SocketData->Cpu;
  RootVn = Cpu[Root->Node.SocId].Vn;

  //
  // Set the Forward Path from "Root" to "Dest". Backward path from "Dest" to "Root" will be
  // set when this routineis called with "Dest" as "Root" and "Root" as "Dest".
  //
  //
  // 1. Recursively set the Forward Path in:
  //     - Socket "Interm"
  //     - Sockets that exist between "Interm" & "Root"
  //     - Socket "Root"
  // (No need to set the Forward path in Socket "Dest" itself since the Local Routing rules in "Dest" will take care of it)
  //
  Parent  = *Interm;
  Child   = *Dest;

  while (1) {
    if (Parent.Node.SocId == Root->Node.SocId && Parent.Node.SocType == Root->Node.SocType) {
      if (Parent.Node.SocType == SOCKET_TYPE_CPU) {
        //
        // In Root CPU socket, setup the route in Cha & M2PCIe.
        //
        if (MoreThanOneLinkExists (KtiInternalGlobal, SocketData->Cpu, &Parent, &Child)) {
          GetMultiLinkFromLEP (KtiInternalGlobal, SocketData->Cpu, &Parent, &Child, cLocalPort, 4);
          if (cLocalPort[0] == KTI_INVALID_PORT || cLocalPort[1] == KTI_INVALID_PORT) {
            KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Invalid Dual-Link between CPU%u and CPU%u. ", Parent.Node.SocId, Child.Node.SocId));
            KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
          }

          if (cLocalPort[0] >= (KtiInternalGlobal->KtiPortCnt)) {
            KTI_ASSERT ((cLocalPort[0] < (KtiInternalGlobal->KtiPortCnt)), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_KTI_PORT_ID);
            return KTI_FAILURE;
          }

          DualOrTripleLink = TRUE;

          SelectUpiPortForRouteFrRootNodeToFpga (
            KtiInternalGlobal,
            SocketData,
            &Child,
            Dest,
            &cLocalPort[0],
            &cLocalPort[1],
            &cLocalPort[2],
            &DualOrTripleLink
            );

          SetRootNodeRoute (
            SocketData,
            KtiInternalGlobal,
            (UINT8) Parent.Node.SocId,
            (UINT8) Child.Node.SocId,
            (UINT8) Dest->Node.SocId,
            cLocalPort,
            sizeof(cLocalPort) / sizeof(cLocalPort[0]),
            DualOrTripleLink,
            FoundInAlternateTree,
            RootVn
            );

        } else {
          cLocalPort[0] = GetChildLocalPortFromLEP (KtiInternalGlobal, SocketData->Cpu, &Parent, &Child);
          if (cLocalPort[0] == KTI_INVALID_PORT) {
            KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Invalid Link between CPU%u and CPU%u. ", Parent.Node.SocId, Child.Node.SocId));
            KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
          }
          cLocalPort[3] = cLocalPort[2] = cLocalPort[1] = cLocalPort[0];
          SetRootNodeRoute (
            SocketData,
            KtiInternalGlobal,
            (UINT8) Parent.Node.SocId,
            (UINT8) Child.Node.SocId,
            (UINT8) Dest->Node.SocId,
            cLocalPort,
            sizeof(cLocalPort) / sizeof(cLocalPort[0]),
            FALSE,
            FoundInAlternateTree,
            RootVn
            );
        }
      } else {
        KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Unknown Socket Type found in CPU%u TopologyTree. Topology not supported.\n", Root->Node.SocId));
        KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE);
      }
      break;
    } else if (Parent.Node.SocType == SOCKET_TYPE_CPU) {
      //
      // Setup the path in intermediate CPU socket found in between root & dest
      //
      GetParentSocket (TopologyTree, &Parent, &Grandparent);

      ParentNodeId = (UINT8) Parent.Node.SocId;
      pLocalPort1 = (UINT8) Parent.Node.ParentPort;
      cLocalPort[0] = GetChildLocalPortFromLEP (KtiInternalGlobal, SocketData->Cpu, &Parent, &Child);

      SelectUpiPortForPassThruRouteToFpga (KtiInternalGlobal, SocketData, &Grandparent, &Parent, &Child, &pLocalPort1);

      SelectOutportForOptimizedPassThruRoute (KtiInternalGlobal, SocketData, ParentNodeId, pLocalPort1, &cLocalPort[0]);

      if ((cLocalPort[0] >= (KtiInternalGlobal->KtiPortCnt)) || (pLocalPort1 >= (KtiInternalGlobal->KtiPortCnt))) {
        KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Invalid Port connection between CPU%u and CPU%u.", ParentNodeId, Child.Node.SocId));
        KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_KTI_PORT_ID);
        return KTI_FAILURE;
      }

      SetPassThruRoute (SocketData, KtiInternalGlobal, ParentNodeId, (UINT8) Dest->Node.SocId, pLocalPort1, cLocalPort[0], RootVn);

      if (Dest->Node.SocType == SOCKET_TYPE_FPGA) {
        //
        // Also setup the pass through route for the opposite direction. This step is done here because
        // Topology Discovery code doesn't include FPGA node as Hop 0 Root Node.
        //
        SetPassThruRoute (SocketData, KtiInternalGlobal, ParentNodeId, (UINT8) Grandparent.Node.SocId, cLocalPort[0], pLocalPort1, RootVn);
      }

      if (KTIVAR->SysConfig >= SYS_CONFIG_4S) {
        if (MoreThanOneLinkExists (KtiInternalGlobal, SocketData->Cpu, &Grandparent, &Parent)) {
          //
          // Setup the route through for the dual link on the intermediate node.
          // Route through traffic are allowed on:
          //          ----------------                           ----------------
          // ---UPI-->| Intermediate |---UPI-->         ---UPI-->| Intermediate |--UPI-->
          // ---UPI-->|     Node     |            and   ---UPI-->|     Node     |--UPI-->
          //          ----------------                           ----------------
          //  (dual UPI links to 1 UPI)             (dual UPI links to dual UPI links)
          //
          pLocalPort1 = (UINT8) SocketData->Cpu[ParentNodeId].LinkData[pLocalPort1].DualLinkIndex;

          if (MoreThanOneLinkExists (KtiInternalGlobal, SocketData->Cpu, &Parent, &Child)) {
            cLocalPort[0] = (UINT8) SocketData->Cpu[ParentNodeId].LinkData[cLocalPort[0]].DualLinkIndex;
          }
          SetPassThruRoute (SocketData, KtiInternalGlobal, ParentNodeId, (UINT8) Dest->Node.SocId, pLocalPort1, cLocalPort[0], RootVn);
        }
      }

      if ((KTIVAR->SysConfig == SYS_CONFIG_8S) && (KtiInternalGlobal->EightSocketTopology == Topo8STwoClumps)) {
        //
        // Note: If 8S 2Clumps topology, there might be DualLink only between Grandparent and Parent.
        //       In this case, only inport has DualLink but the outport does not have. So just need to update the pLocalPort1.
        //
        if (MoreThanOneLinkExists (KtiInternalGlobal, SocketData->Cpu, &Grandparent, &Parent)) {
          //
          // Setup the route through for the dual link on the intermediate node
          //
          pLocalPort1 = (UINT8) SocketData->Cpu[ParentNodeId].LinkData[pLocalPort1].DualLinkIndex;
          SetPassThruRoute (SocketData, KtiInternalGlobal, ParentNodeId, (UINT8) Dest->Node.SocId, pLocalPort1, cLocalPort[0], RootVn);
        }
      }
    } else {
      KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Unknown Intermediate Socket Type found in CPU %u Topology Tree", Root->Node.SocId));
      KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE);
    }

    //
    // Go up one step towards the root & repeat the loop
    //
    Child = Parent;
    GetParentSocket (TopologyTree, &Child, &Parent);
  }

  return Status;
}

/**
  This function returns which port on the Root Node should be used for sending outgoing snoop traffic.

  @param[in] SocketData        - pointer to the socket data structure
  @param[in] KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param[in] TopologyTree      - Pointer to full system KTI fabric tree
  @param[in] Root              - Pointer to root node
  @param[in] Dest              - Pointer to destination node
  @param[out]OutPort           - Root Node's UPI Port to send ongoing snoop traffic

  @retval KTI_SUCCESS      - Valid OutPort was returned.
  @retval KTI_FAILURE      - Invalid OutPort was returned.

**/
KTI_STATUS
GetOutPortForRootNodeFanout (
  IN  KTI_SOCKET_DATA          *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN  TOPOLOGY_TREE_NODE       *TopologyTree,
  IN  TOPOLOGY_TREE_NODE       *Root,
  IN  TOPOLOGY_TREE_NODE       *Dest,
  OUT UINT8                    *OutPort
  )
{
  UINT8                 LowerCpuNodeId;
  UINT8                 LowerNodeIdChaFanout;
  UINT8                 i;
  UINT8                 j;
  UINT8                 cLocalPort[4];
  UINT8                 MaxLocalPort;
  KTI_STATUS            Status;
  BOOLEAN               SupportExtSnpFanoutTable;
  UINT8                 BitFieldWidth;
  UINT32                FieldMask;
  UINT8                 FirstNodeIdInExtFanoutTable;

  Status        = KTI_SUCCESS;
  MaxLocalPort  = sizeof(cLocalPort) / sizeof(cLocalPort[0]);

  SetMem (cLocalPort, MaxLocalPort, KTI_INVALID_PORT);

  *OutPort = KTI_INVALID_PORT;

  if (!MoreThanOneLinkExists (KtiInternalGlobal, SocketData->Cpu, Root, Dest)) {
    *OutPort = GetChildLocalPortFromLEP (KtiInternalGlobal, SocketData->Cpu, Root, Dest);
    return KTI_SUCCESS;
  }

  //
  // There are multiple links between root and destination nodes.
  //

  if (Root->Node.SocId < Dest->Node.SocId) {
    //
    // For dual-link or Triple links configs, we select the first link.
    //
    GetMultiLinkFromLEP (KtiInternalGlobal, SocketData->Cpu, Root, Dest, cLocalPort,  sizeof(cLocalPort) / sizeof(cLocalPort[0]));
    *OutPort = cLocalPort[0];
    return KTI_SUCCESS;
  }

  //
  // If there are multiple links betwee Root and Destination nodes, we will reuse the same link that was
  // already setup in CPU with lower node ID, since CPU with lower node ID was processed first.
  // This info can be obtained by reading from lower node ID's CHA fanout table which is already configured
  // prior to setting up snoop fanout pass through route.
  //

  BitFieldWidth = ChaSnpFanoutTablePerEntrySize (&SupportExtSnpFanoutTable, &FirstNodeIdInExtFanoutTable);
  FieldMask = FLD_MASK (BitFieldWidth, 0);

  LowerCpuNodeId = (UINT8) Dest->Node.SocId;

  if (SupportExtSnpFanoutTable && (LowerCpuNodeId >= FirstNodeIdInExtFanoutTable)) {
    LowerNodeIdChaFanout = (UINT8) ((SocketData->Route[LowerCpuNodeId].ChaKtiFanOut1[ALL_CHA_DATA] >> (BitFieldWidth * (LowerCpuNodeId - FirstNodeIdInExtFanoutTable))) & FieldMask);
  } else {
    LowerNodeIdChaFanout = (UINT8) ((SocketData->Route[LowerCpuNodeId].ChaKtiFanOut[ALL_CHA_DATA] >> (BitFieldWidth * LowerCpuNodeId)) & FieldMask);
  }

  GetMultiLinkFromLEP (KtiInternalGlobal, SocketData->Cpu, Dest, Root, cLocalPort, MaxLocalPort);

  for (i = 0; i < BitFieldWidth; i++) {
    if ((LowerNodeIdChaFanout & (1 << i)) == 0) {
      continue;
    }
    for (j = 0; (j < MaxLocalPort) && (cLocalPort[j] != KTI_INVALID_PORT); j++) {
      if (i == cLocalPort[j]) {
        *OutPort = (UINT8) SocketData->Cpu[LowerCpuNodeId].LinkData[i].PeerPort;
        return KTI_SUCCESS;
      }
    }
  }

  if (*OutPort == KTI_INVALID_PORT) {
    Status = KTI_FAILURE;
  }

  return Status;
}


/**
  This function returns the Intermediate Node's UPI ports for incoming and outgoing traffic
  for snoop fanout pass through route.

  @param[in] SocketData        - pointer to the socket data structure
  @param[in] KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param[in] TopologyTree      - Pointer to full system KTI fabric tree
  @param[in] Interm            - Pointer to intermediate node
  @param[in] Dest              - Pointer to destination node
  @param[out]InPort            - Intermediate Node's UPI Port that receives the incoming traffic
  @param[out]OutPort           - Intermediate Node's UPI Port that sends the outgoing traffic

  @retval KTI_SUCCESS      - Valid InPort and OutPort were returned.
  @retval KTI_FAILURE      - Invalid InPort and/or OutPort were returned.

**/
KTI_STATUS
GetInPortOutPortForPassThruFanout (
  IN  KTI_SOCKET_DATA          *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN  TOPOLOGY_TREE_NODE       *TopologyTree,
  IN  TOPOLOGY_TREE_NODE       *Interm,
  IN  TOPOLOGY_TREE_NODE       *Dest,
  OUT UINT8                    *InPort,
  OUT UINT8                    *OutPort
  )
{
  TOPOLOGY_TREE_NODE    Grandparent;
  UINT8                 GrandparentNodeId;
  UINT8                 GrandparentChaFanoutSetting;
  UINT8                 i;
  UINT8                 j;
  UINT8                 cLocalPort[4];
  UINT8                 MaxLocalPort;
  KTI_STATUS            Status;
  BOOLEAN               SupportExtSnpFanoutTable;
  UINT8                 BitFieldWidth;
  UINT32                FieldMask;
  UINT8                 FirstNodeIdInExtFanoutTable;

  Status        = KTI_SUCCESS;
  MaxLocalPort  = sizeof(cLocalPort) / sizeof(cLocalPort[0]);

  SetMem (cLocalPort, MaxLocalPort, KTI_INVALID_PORT);

  *InPort = KTI_INVALID_PORT;
  *OutPort = GetChildLocalPortFromLEP (KtiInternalGlobal, SocketData->Cpu, Interm, Dest);

  GetParentSocket (TopologyTree, Interm, &Grandparent);

  if (!MoreThanOneLinkExists (KtiInternalGlobal, SocketData->Cpu, &Grandparent, Interm)) {
    *InPort = (UINT8) Interm->Node.ParentPort;
    return KTI_SUCCESS;
  }

  //
  // There are multiple links between grandparent and intermediate nodes.
  // Find out which port on Grandparent Node that was setup for sending snoop traffic to intermediate node.
  // This info can be obtained by reading from Grandparent's CHA fanout table which is already configured
  // prior to setting up snoop fanout pass through route.
  //

  if (Dest->Node.Hop >= MAX_NUM_HOPS) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "  %a(): Unsupported SnoopFanout for multi-link %u-hop away node!", __FUNCTION__, Dest->Node.Hop));
    KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
  }

  BitFieldWidth = ChaSnpFanoutTablePerEntrySize (&SupportExtSnpFanoutTable, &FirstNodeIdInExtFanoutTable);
  FieldMask = FLD_MASK (BitFieldWidth, 0);

  GrandparentNodeId = (UINT8) Grandparent.Node.SocId;

  if (GrandparentNodeId >= MAX_SOCKET) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Invalid GrandparentNodeId=%x\n", GrandparentNodeId));
    KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
    return KTI_FAILURE;
  }

  if (SupportExtSnpFanoutTable && (GrandparentNodeId >= FirstNodeIdInExtFanoutTable)) {
    GrandparentChaFanoutSetting = (UINT8) ((SocketData->Route[GrandparentNodeId].ChaKtiFanOut1[ALL_CHA_DATA] >> (BitFieldWidth * (GrandparentNodeId - FirstNodeIdInExtFanoutTable))) & FieldMask);
  } else {
    GrandparentChaFanoutSetting = (UINT8) ((SocketData->Route[GrandparentNodeId].ChaKtiFanOut[ALL_CHA_DATA] >> (BitFieldWidth * GrandparentNodeId)) & FieldMask);
  }

  GetMultiLinkFromLEP (KtiInternalGlobal, SocketData->Cpu, &Grandparent, Interm, cLocalPort, MaxLocalPort);

  for (i = 0; i < BitFieldWidth; i++) {
    if ((GrandparentChaFanoutSetting & (1 << i)) == 0) {
      continue;
    }
    for (j = 0; (j < MaxLocalPort) && (cLocalPort[j] != KTI_INVALID_PORT); j++) {
      if (i == cLocalPort[j]) {
        *InPort = (UINT8) SocketData->Cpu[GrandparentNodeId].LinkData[i].PeerPort;
        break;
      }
    }
  }

  if (*InPort >= KtiInternalGlobal->KtiPortCnt) {
    return KTI_FAILURE;
  }

  SelectOutportForOptimizedPassThruRoute (KtiInternalGlobal, SocketData, (UINT8) Interm->Node.SocId, *InPort, OutPort);

  return Status;
}

/**
  Setup broadcast routing.

  @param SocketData        - pointer to the socket data structure
  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param TopologyTree      - Pointer to full system KTI fabric tree
  @param Root              - Pointer to root KTI node
  @param Interm            - Pointer to intermediate KTI node
  @param Dest              - Pointer to destination KTI node
  @param PassThruFanout    - Pass-through fanout on/off

  @retval KTI_SUCCESS      - on successful completion
  @retval KTI_FAILURE      - Failed to setup BC route
**/
STATIC
KTI_STATUS
SetupBCRoute (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  TOPOLOGY_TREE_NODE       *TopologyTree,
  TOPOLOGY_TREE_NODE       *Root,
  TOPOLOGY_TREE_NODE       *Interm,
  TOPOLOGY_TREE_NODE       *Dest,
  BOOLEAN                  PassThruFanout
  )
{
  UINT8 cLocalPort;
  UINT8 pLocalPort1;
  UINT8 pLocalPort2;
  UINT8 RootNodeId;

  cLocalPort = pLocalPort1 = pLocalPort2 = 0xFF;

  //
  //  Set the broadcast path to "Dest" in "Interm" socket
  //
  if (Interm->Node.SocId == Root->Node.SocId && Interm->Node.SocType == Root->Node.SocType) {
    GetOutPortForRootNodeFanout (SocketData, KtiInternalGlobal, TopologyTree, Root, Dest, &cLocalPort);

    if (cLocalPort >= KtiInternalGlobal->KtiPortCnt) {
      KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Invalid Port connection between CPU%u and CPU%u. ", Interm->Node.SocId, Dest->Node.SocId));
      KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
    }

    RootNodeId = (UINT8) Root->Node.SocId;
    SetRootNodeFanoutRoute (SocketData, KtiInternalGlobal, RootNodeId, cLocalPort, FALSE);

    if (KtiInternalGlobal->SnoopFanoutChaInterleaveEn == TRUE) {
      cLocalPort = (UINT8) SocketData->Cpu[RootNodeId].LinkData[cLocalPort].DualLinkIndex;
      if (cLocalPort != KTI_INVALID_PORT) {
        SetRootNodeFanoutRoute (SocketData, KtiInternalGlobal, RootNodeId, cLocalPort, TRUE);
      }
    }
  } else if (Interm->Node.SocType == SOCKET_TYPE_CPU) {
    if ((!KtiInternalGlobal->Is4SRing) || (KtiInternalGlobal->Is4SRing && PassThruFanout)) {  // check if (not 4S ring) or (4S ring with passthru)
      //
      // Setup the path in intermediate CPU socket found in between root & dest
      //
      GetInPortOutPortForPassThruFanout (SocketData, KtiInternalGlobal, TopologyTree, Interm, Dest, &pLocalPort1, &cLocalPort);

      if ((cLocalPort >= (KtiInternalGlobal->KtiPortCnt)) || (pLocalPort1 >= (KtiInternalGlobal->KtiPortCnt))) {
        KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Invalid Port connection between CPU %u and CPU %u.", Interm->Node.SocId, Dest->Node.SocId));
        KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
      }

      SetPassThruFanoutRoute (SocketData, KtiInternalGlobal, Interm, (UINT8) Root->Node.SocId, pLocalPort1, cLocalPort);

      if ((KtiInternalGlobal->SnoopFanoutChaInterleaveEn == TRUE) && (KTIVAR->SysConfig >= SYS_CONFIG_4S)) {
        if (MoreThanOneLinkExists (KtiInternalGlobal, SocketData->Cpu, Root, Interm) &&
            MoreThanOneLinkExists (KtiInternalGlobal, SocketData->Cpu, Interm, Dest)) {
          //
          // Setup the route through for the dual link on the intermediate node
          //
          if ((pLocalPort1 >= MAX_FW_KTI_PORTS) || (cLocalPort >= MAX_FW_KTI_PORTS)) {
            KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Invalid local port!\n"));

            return KTI_FAILURE;
          }
          pLocalPort1 = (UINT8) SocketData->Cpu[Interm->Node.SocId].LinkData[pLocalPort1].DualLinkIndex;
          cLocalPort = (UINT8) SocketData->Cpu[Interm->Node.SocId].LinkData[cLocalPort].DualLinkIndex;
          SetPassThruFanoutRoute (SocketData, KtiInternalGlobal, Interm, (UINT8) Root->Node.SocId, pLocalPort1, cLocalPort);
        }
      }
    }
  } else {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Unknown Intermediate Socket Type found in CPU %u Topology Tree", Root->Node.SocId));
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Topology not supported\n"));
    KTI_ASSERT (TRUE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
  }

  return KTI_SUCCESS;
}

/**
  Setup snoop fanout routing for a pass-through node.

  @param SocketData - pointer to the socket data structure
  @param Parent     - pointer to KTI parent node
  @param RootBcRti  -
  @param InPort     - port receiving snoop
  @param OutPort    - port to send out snoop

  @retval KTI_SUCCESS - on successful completion

**/
STATIC
KTI_STATUS
SetPassThruFanoutRoute (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  TOPOLOGY_TREE_NODE       *Parent,
  UINT8                    RootBcRti,
  UINT8                    InPort,
  UINT8                    OutPort
  )
{
  UINT8   M3KtiNum;
  UINT8   M3KtiPortNum;
  UINT8   ParentNodeId;
  BOOLEAN IsCpx;
  UINT8   BitFieldWidth;

  ParentNodeId = (UINT8) Parent->Node.SocId;

  IsCpx = FALSE;
  BitFieldWidth = 3;

  if (KtiInternalGlobal->CpuType == CPU_CPX) {
    IsCpx = TRUE;
    BitFieldWidth = 8;
  }

  if (KTIVAR->SharedKtiPort0And1) {
    //
    // Need to differentiate fanout routes between 2 ports for each M3KTI
    //
    M3KtiNum = PORT_TO_M3KTI (InPort); // determine M3KTI agent based on InPort
    M3KtiPortNum = InPort & 0x1;
  } else {
    //
    // Only 1 Fanout route per M3KTI
    //
    M3KtiNum = InPort;
    M3KtiPortNum = 0;
  }

  if (IsCpx && (RootBcRti >= 4)) {
    SocketData->Route[ParentNodeId].M3KtiSnpFanOut1[M3KtiNum][M3KtiPortNum] |= ((1 << OutPort) << (BitFieldWidth * (RootBcRti - 4)));
  } else {
    SocketData->Route[ParentNodeId].M3KtiSnpFanOut[M3KtiNum][M3KtiPortNum] |= ((1 << OutPort) << (BitFieldWidth * RootBcRti));
  }

  return KTI_SUCCESS;
}

/**
  Setup snoop fanout routing for root node

  @param[out] SocketData         Pointer to the socket data structure
  @param[in]  KtiInternalGlobal  Pointer to the KTI RC internal global structure
  @param[in]  RootBcRti          Root Node node ID
  @param[in]  OutPort            Port from which snoop to be sent
  @param[in]  IsSecondLink       TRUE if OutPort is the second link of the dual or triple links pair.
                                 FALSE otherwise.

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
SetRootNodeFanoutRoute (
  OUT KTI_SOCKET_DATA          *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN  UINT8                    RootBcRti,
  IN  UINT8                    OutPort,
  IN  BOOLEAN                  IsSecondLink
  )
{
  UINT8   Idx1;
  UINT32  EncodingData0;
  UINT32  EncodingData1;
  BOOLEAN SupportExtSnpFanoutTable;
  UINT8   BitFieldWidth;
  UINT8   FirstNodeIdInExtFanoutTable;

  BitFieldWidth = ChaSnpFanoutTablePerEntrySize (&SupportExtSnpFanoutTable, &FirstNodeIdInExtFanoutTable);
  EncodingData1 = EncodingData0 = 0;

  //
  // Request coming from all populated sockets including the local socket should trigger a snoop to OutPort
  //
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if ((KtiInternalGlobal->CpuList & (1 << Idx1))) {
      if (SupportExtSnpFanoutTable && (Idx1 >= FirstNodeIdInExtFanoutTable)) {
        EncodingData1 |= ((1 << OutPort) << (BitFieldWidth * (Idx1 - FirstNodeIdInExtFanoutTable))) ; // program send2ktiport for this RHNID in CHA
      } else {
        EncodingData0 |= ((1 << OutPort) << (BitFieldWidth * Idx1)) ; // program send2ktiport for this RHNID in CHA
      }
    }
  }

  if (IsSecondLink && (KtiInternalGlobal->SnoopFanoutChaInterleaveEn == TRUE)) {
    //
    // When SnoopFanoutChaInterleaveEn is enabled, the second link of the duallink pair is assigned to
    // Odd CHAs.
    //
    SocketData->Route[RootBcRti].ChaKtiFanOut[ODD_CHA_DATA] |= EncodingData0;
    SocketData->Route[RootBcRti].ChaKtiFanOut1[ODD_CHA_DATA] |= EncodingData1;
  } else {
    //
    // Index 0 indicates either fanout settings for all CHAs (when SnoopFanoutChaInterleaveEn = FALSE)
    // or fanout settings for Even CHA IDs (when SnoopFanoutChaInterleaveEn = TRUE; first link of the duallink pair
    // is assigned to Even CHAs).
    //
    SocketData->Route[RootBcRti].ChaKtiFanOut[ALL_CHA_DATA] |= EncodingData0;
    SocketData->Route[RootBcRti].ChaKtiFanOut1[ALL_CHA_DATA] |= EncodingData1;
  }

  return KTI_SUCCESS;
}

/**
  When the system is in Snoop Fanout mode, the Cha is required to send one snoop per link. But in some topologies
  it is possible the original requester is the only socket present on any of its link. In such cases, the Cha can be optimized
  to no send the snoop over the link to reduce the uncore ring traffic.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param SocketData - socket specific data structure

  @retval None

**/
STATIC
VOID
OptimizeChaFanoutSnoop (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  KTI_SOCKET_DATA           *SocketData
  )
{
  UINT8               Index1, Index2, PeerSocId, PeerPort, M3KtiNum;
  UINT8               M3KtiPortNum;
  UINT32              Data32;
  BOOLEAN             SupportExtSnpFanoutTable;
  UINT8               BitFieldWidth;
  UINT8               DestNodeId;
  UINT8               FirstNodeIdInExtFanoutTable;

  BitFieldWidth = ChaSnpFanoutTablePerEntrySize (&SupportExtSnpFanoutTable, &FirstNodeIdInExtFanoutTable);

  //
  // For each valid sockets, find out if the pass-through traffic is needed on the peer port of peer socket,
  // If there is no pass-through fanout traffic, then Cha can be optimized to no send the snoop over the link to reduce the uncore ring traffic for RHNID = peer socket.
  //
  for (Index1 = 0; Index1 < MAX_SOCKET; Index1++) {
    if ((SocketData->Cpu[Index1].Valid == TRUE) && (SocketData->Cpu[Index1].SocType == SOCKET_TYPE_CPU)) {

      //
      // find out if the pass-through traffic is needed on the peer port of peer socket,
      //
      for (Index2 = 0; Index2 < KtiInternalGlobal->KtiPortCnt; Index2++) {
        if (SocketData->Cpu[Index1].LinkData[Index2].Valid == TRUE ) {

          //
          //Read M3KTI Fanout value from Peer Port of Peer Socket
          //If is 0, there is no pass-through fanout needed on Peer Port of Peer Socket
          //
          PeerSocId = (UINT8) SocketData->Cpu[Index1].LinkData[Index2].PeerSocId;
          PeerPort =  (UINT8) SocketData->Cpu[Index1].LinkData[Index2].PeerPort;

          if ((PeerSocId >= MAX_SOCKET) || (PeerPort >= KtiInternalGlobal->KtiPortCnt)) {
            KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n PeerPort:%u >= KtiPortCnt:%u or PeerSocId:%u >= MAX_SOCKET:%u",
              PeerPort, KtiInternalGlobal->KtiPortCnt, PeerSocId, MAX_SOCKET));
            KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
            return;
          }

          DestNodeId = (UINT8) SocketData->Cpu[PeerSocId].LinkData[PeerPort].PeerSocId;

          if (KTIVAR->SharedKtiPort0And1) {
            //
            // Need to differentiate fanout routes between 2 ports for each M3KTI
            //
            M3KtiNum = PORT_TO_M3KTI (PeerPort);
            M3KtiPortNum = PeerPort & 0x1;
          } else {
            //
            // Only 1 Fanout route per M3KTI
            //
            M3KtiNum = PeerPort;
            M3KtiPortNum = 0;
          }

          if ((M3KtiNum >= MAX_M3KTI) || (M3KtiPortNum >= MAX_PORT_IN_M3KTI)) {
            KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n M3KtiNum%u >= MAX_M3KTI:%u or M3KtiPortNum%u >= MAX_PORT_IN_M3KTI:%u",
              M3KtiNum, MAX_M3KTI, M3KtiPortNum, MAX_PORT_IN_M3KTI));
            KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
            return;
          }

          //
          // CPX has 2 sets of M3KtiSnpFanOut* table for storing route through Snoop fanout setting.
          // M3KtiSnpFanOut array stores RHNID 0 - 3; M3KtiSnpFanOut1 array stores RHNID 4 - 7.
          // Other SOCs use only M3KtiSnpFanOut table for RHNID 0 - 7.
          //
          if ((KtiInternalGlobal->CpuType == CPU_CPX) && (DestNodeId >= 4)) {
            Data32 = SocketData->Route[PeerSocId].M3KtiSnpFanOut1[M3KtiNum][M3KtiPortNum];
          } else {
            Data32 = SocketData->Route[PeerSocId].M3KtiSnpFanOut[M3KtiNum][M3KtiPortNum];
          }

          if (Data32 == 0) {
            //
            // Optimize the CHA to not send snoop to the link that peer socket id is equal to RHNID
            //

            //
            // CPX uses 2 sets of ChaKtiFanOut* table for storing CHA Snoop Fanout setting.
            // ChaKtiFanOut stores RHNID 0 - 4 setting; ChaKtiFanOut1 stores RHNID 5 - 7 setting.
            // Other SOCs use only ChaKtiFanOut for RHNID 0 - 7.
            //
            if (SupportExtSnpFanoutTable && (PeerSocId >= FirstNodeIdInExtFanoutTable)) {
              Data32 = SocketData->Route[Index1].ChaKtiFanOut1[ALL_CHA_DATA];
              Data32 = Data32 & (~((1 << Index2) << (BitFieldWidth * (PeerSocId - FirstNodeIdInExtFanoutTable))));
              SocketData->Route[Index1].ChaKtiFanOut1[ALL_CHA_DATA] = Data32;
            } else {
              Data32 = SocketData->Route[Index1].ChaKtiFanOut[ALL_CHA_DATA];
              Data32 = Data32 & (~((1 << Index2) << (BitFieldWidth * PeerSocId)));
              SocketData->Route[Index1].ChaKtiFanOut[ALL_CHA_DATA] = Data32;
            }
            KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n CPU%1u CHA Snoop fanout optimized for Link%1u target CPU%1u",
                                 Index1, Index2, PeerSocId));
          }
        }
      }
    }
  }

  return;
}

/**
  Configure CHA Routing table for SBSP or other node that is connected to SBSP.

  @param[in] SocId                 CPU Socket Node number (Socket ID)
  @param[in] ChaCount              Total number of CHAs for the socket "SocId"
  @param[in] ChaPortFieldWidth     Width of field used for UPI port
  @param[in] LocalPort             Local port used for routing to Destination Socket
  @param[in] DestSocId             Destination CPU Socket Node number (Socket ID)
  @param[in] UpiRoutingMode        UPI routing mode. See UPI_ROUTING_MODE enum.

  @retval KTI_SUCCESS  CHA Route Table was programmed.

**/
KTI_STATUS
ConfigureChaKtiRTForSbspOrRouteToSbsp (
  IN UINT8    SocId,
  IN UINT8    ChaCount,
  IN UINT8    ChaPortFieldWidth,
  IN UINT8    LocalPort,
  IN UINT8    DestSocId,
  IN UINT8    UpiRoutingMode
  )
{
  UINT64              FieldMask;
  UINT64              PortSetting;
  UINT8               FieldWidth;
  UINT8               DestSocIdLocation;

  FieldWidth = ChaPortFieldWidth;
  PortSetting = LocalPort;
  DestSocIdLocation = DestSocId;

  if (UpiRoutingMode == UpiRoutingModeB2) {
    //
    // Default routing mode for CPX/SPR
    // 16-entry table: each Node has 2 entries
    //
    FieldWidth *= 2;
    PortSetting |= (LocalPort << ChaPortFieldWidth);
  }

  FieldMask = LShiftU64 (~(LShiftU64 (MAX_UINT64, FieldWidth)), (FieldWidth * DestSocIdLocation));
  PortSetting = LShiftU64 (PortSetting, FieldWidth * DestSocIdLocation);
  ChaProgramRouteTableForIndividualDestNodeId (SocId, ChaCount, FieldMask, PortSetting);

  return KTI_SUCCESS;
}

/**

  Routing Mode A (SKX, CLX, ICX-R0): 8-entry table; one link between nodes; Indexing using NodeID[2:0].

  This routine determines CHA KTI Route Table programming values based on the UPI routing mode A,
  and populates the route table setting into ChaKtiRt in SocketData structure.

  @param[in,out] SocketData        Pointer to socket specific data
  @param[in] RootSocId             Root Node node ID (a.k.a. socket ID)
  @param[in] DestSocId             Destination node node ID (a.k.a. socket ID)
  @param[in] OutPorts              Array that stores KTI ports of Root Node
  @param[in] PortFieldWidth        Port field width; this is SoC specific.
  @param[in] MultiLink             TRUE - Dual or triple or four links exists;
                                   FALSE - No Dual link nor triple link nor quad link

  @retval KTI_SUCCESS  ChaKtiRt is populated with valid route table settings.

**/
KTI_STATUS
SetupUpiRoutingModeA (
  IN OUT KTI_SOCKET_DATA          *SocketData,
  IN     UINT8                     RootSocId,
  IN     UINT8                     DestSocId,
  IN     UINT8                     *OutPorts,
  IN     UINT8                     PortFieldWidth,
  IN     BOOLEAN                   MultiLink
  )
{
  UINT32       FieldMask;

  //
  // Routing Mode A (SKX, CLX, ICX-R0): 8-entry table; one link between nodes; Indexing using NodeID[2:0]
  //

  KTI_ASSERT ((!MultiLink), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);

  FieldMask = FLD_MASK (PortFieldWidth, (PortFieldWidth * DestSocId));
  SocketData->Route[RootSocId].ChaKtiRt &= ~FieldMask;
  SocketData->Route[RootSocId].ChaKtiRt |= (OutPorts[0] << (PortFieldWidth * DestSocId));

  return KTI_SUCCESS;
}

/**

  Routing Mode B0 (SKX, CLX, ICX-R0): 8-entry table - Up to 2 links between nodes. Indexing using p6^p12.

  This routine determines CHA KTI Route Table programming values based on the UPI routing mode B0,
  and populates the route table setting into ChaKtiRt in SocketData structure.

  @param[in,out] SocketData        Pointer to socket specific data
  @param[in] KtiInternalGlobal     Pointer to KTIRC internal variables structure
  @param[in] RootSocId             Root Node node ID (a.k.a. socket ID)
  @param[in] DestSocId             Destination node node ID (a.k.a. socket ID)
  @param[in] OutPorts              Array that stores KTI ports of Root Node
  @param[in] PortFieldWidth        Port field width; this is SoC specific.
  @param[in] PortFieldMask         Port field mask; this is SoC specific.
  @param[in] MultiLink             TRUE - Dual or triple or four links exists;
                                   FALSE - No Dual link nor triple link nor quad link

  @retval KTI_SUCCESS  ChaKtiRt is populated with valid route table settings.

**/
KTI_STATUS
SetupUpiRoutingModeB0 (
  IN OUT KTI_SOCKET_DATA          *SocketData,
  IN     KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN     UINT8                     RootSocId,
  IN     UINT8                     DestSocId,
  IN     UINT8                     *OutPorts,
  IN     UINT8                     PortFieldWidth,
  IN     UINT8                     PortFieldMask,
  IN     BOOLEAN                   MultiLink
  )
{
  UINT8        PeerPort1;
  UINT8        PeerPort2;

  //
  // Dual links found between nodes.
  // Routing Mode B0 (SKX, CLX, ICX-R0): 8-entry table - Up to 2 links between nodes. Indexing using p6^p12.
  //

  if (SocketData->Cpu[DestSocId].SocType == SOCKET_TYPE_FPGA) {
    //
    // ICX and SKX don't need to program routing to FPGA
    //
    return KTI_IGNORE;
  }

  if (IsSkxFamilyCpu (KtiInternalGlobal->CpuType)) {
    //
    // For SKX/CLX: In case of 2SEP, dual link, the indexing into CHA and M2Mem routing tables is done by even/odd address
    // This indexing mode is set off by CBO_COH_CONFIG.enablexorbasedktilink = 1 for CHAs and kti2slinkintlvena = 1 for m2MEm
    // [3:2] = 0x1 - route even-addressed entries to port 1
    // [1:0] = 0x0 - route odd-addressed entries to port 0
    //
    SocketData->Route[RootSocId].ChaKtiRt = 0x4;
  } else {
    //
    // Based on SBSP's routing table, find out what these ports are connected on another socket.
    //
    if (RootSocId != 0) {
      PeerPort1 = (UINT8) (SocketData->Route[0].ChaKtiRt & PortFieldMask);
      PeerPort2 = (UINT8) ((SocketData->Route[0].ChaKtiRt >> PortFieldWidth) & PortFieldMask);
      if ((PeerPort1 < (KtiInternalGlobal->KtiPortCnt)) && (PeerPort2 < (KtiInternalGlobal->KtiPortCnt))) {
        OutPorts[0] = (UINT8) SocketData->Cpu[0].LinkData[PeerPort1].PeerPort;
        OutPorts[1] = (UINT8) SocketData->Cpu[0].LinkData[PeerPort2].PeerPort;
      }
    }
    SocketData->Route[RootSocId].ChaKtiRt = (UINT32) (OutPorts[0] | (OutPorts[1] << PortFieldWidth));
  }

  return KTI_SUCCESS;
}

/**

  16-entry table: Indexing using NodeId[2:0],p6^p12; up to 2 links between nodes. For CPX, ICX-L0, SPR.

  This routine handles UPI Routing Mode B2 single link topology only. It populates the route table
  setting into ChaKtiRt in SocketData structure. If AlternateRoute function parameter is TRUE,
  it stores route table setting into ChaKtiRt1 in SocketData structure.

  @param[in,out] SocketData        Pointer to socket specific data
  @param[in] RootSocId             Root Node node ID (a.k.a. socket ID)
  @param[in] DestSocId             Destination node node ID (a.k.a. socket ID)
  @param[in] OutPorts              Array that stores KTI ports of Root Node
  @param[in] PortFieldWidth        Port field width; this is SoC specific.
  @param[in] AlternateRoute        TRUE indicates there is second route between Root Node (SocId)
                                   and Destination Node (DestSocId); therefore update the DestSocId's
                                   second entry with this route. FALSE otherwise.

  @retval KTI_SUCCESS  ChaKtiRt or ChaKtiRt1 is populated with valid route table settings.

**/
KTI_STATUS
SetupUpiRoutingModeB2SingleLink (
  IN OUT KTI_SOCKET_DATA          *SocketData,
  IN     UINT8                     RootSocId,
  IN     UINT8                     DestSocId,
  IN     UINT8                     *OutPorts,
  IN     UINT8                     PortFieldWidth,
  IN     BOOLEAN                   AlternateRoute
  )
{
  UINT32       FieldMask;
  UINT32       Data32;
  UINT32       BitOffset;
  UINT8        NodeEntriesWidth;

  //
  // 16-entry table: Indexing using NodeId[2:0],p6^p12; up to 2 links between nodes. For CPX, ICX-L0.
  //
  NodeEntriesWidth = PortFieldWidth * 2; // Each node has 2 entries (3-bit wide per entry).

  if (DestSocId < 4) {
    BitOffset = NodeEntriesWidth * DestSocId;
  } else {
    BitOffset = NodeEntriesWidth * (DestSocId - 4);
  }

  if (!AlternateRoute) {
    Data32 = (UINT32) OutPorts[0];

    //
    // Check if the second entry of the Dest node was already updated with a valid UPI link
    //
    if ((SocketData->Route[RootSocId].ChaKtiRtSecondEntryUpdated & (1 << DestSocId)) == 0) {
      //
      // Write the port number to both entries for this destination node
      //
      FieldMask = FLD_MASK (NodeEntriesWidth, BitOffset);
      Data32 = (UINT32) (OutPorts[0] | (OutPorts[0] << PortFieldWidth));
    } else {
      //
      // Second entry for this destination node was already updated. Only write the new port number
      // into the first entry.
      //
      FieldMask = FLD_MASK (PortFieldWidth, BitOffset);
      Data32 = (UINT32) OutPorts[0];
    }
  } else {
    //
    // A second route is found between these nodes. Update the second entry with this route.
    //
    BitOffset += PortFieldWidth;
    FieldMask = FLD_MASK (PortFieldWidth, BitOffset);
    Data32 = OutPorts[0];
    SocketData->Route[RootSocId].ChaKtiRtSecondEntryUpdated |= (1 << DestSocId);
  }

  if (DestSocId < 4) {
    SocketData->Route[RootSocId].ChaKtiRt &= ~FieldMask;
    SocketData->Route[RootSocId].ChaKtiRt |= (Data32 << BitOffset);
  } else {
    SocketData->Route[RootSocId].ChaKtiRt1 &= ~FieldMask;
    SocketData->Route[RootSocId].ChaKtiRt1 |= (Data32 << BitOffset);
  }

  return KTI_SUCCESS;
}

/**

  16-entry table: Indexing using NodeId[2:0],p6^p12; up to 2 links between nodes. For CPX, ICX-L0, SPR.

  This routine handles UPI Routing Mode B2 two links topology. For Destination NodeID 0 - 3,
  the route table setting is stored in ChaKtiRt in SocketData structure. For Destination
  NodeID 4- 7, the route table setting is stored in ChaKtiRt1 in SocketData structure.

  @param[in,out] SocketData        Pointer to socket specific data
  @param[in] KtiInternalGlobal     Pointer to KTIRC internal variables structure
  @param[in] RootSocId             Root Node node ID (a.k.a. socket ID)
  @param[in] IntermediateSocId     Intermediate node ID (the node is either Destination Node
                                   itself or connected directly to RootNode)
  @param[in] DestSocId             Destination node node ID (a.k.a. socket ID)
  @param[in] OutPorts              Array that stores KTI ports of Root Node
  @param[in] PortFieldWidth        Port field width; this is SoC specific.
  @param[in] PortFieldMask         Port field mask; this is SoC specific.

  @retval KTI_SUCCESS  ChaKtiRt or ChaKtiRt1 is populated with valid route table settings.

**/
KTI_STATUS
SetupUpiRoutingModeB2MultiLink (
  IN OUT KTI_SOCKET_DATA          *SocketData,
  IN     KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN     UINT8                     RootSocId,
  IN     UINT8                     IntermediateSocId,
  IN     UINT8                     DestSocId,
  IN     UINT8                     *OutPorts,
  IN     UINT8                     PortFieldWidth,
  IN     UINT8                     PortFieldMask
  )
{
  UINT32       Data32;
  UINT8        NodeEntriesWidth;
  UINT8        PeerPort1;
  UINT8        PeerPort2;

  //
  // Dual links found between nodes.
  // 16-entry table: Indexing using NodeId[2:0],p6^p12. Up to 2 links between nodes. For CPX, ICX-L0.
  //

  NodeEntriesWidth = PortFieldWidth * 2; // Each node has 2 entries (3-bit wide per entry).

  if (DestSocId == IntermediateSocId) {
    //
    // Root and Destination Nodes are One hop away
    // Make sure forward and backward path are using the same link
    //
    if (RootSocId > DestSocId) {
      //
      // We are in backward path since the setup of Root Node routes always start with the Node with the lower Node ID.
      // Using DestSocId's ChaKtiRt tables to find out what ports were routed from DestSocId to RootSocId.
      // Then use the same link (PeerPort) to route from RootSocId back to DestSocId.
      //
      if (RootSocId < 4) {
        PeerPort1 = (UINT8) ((SocketData->Route[DestSocId].ChaKtiRt >> (RootSocId * NodeEntriesWidth)) & PortFieldMask);
        PeerPort2 = (UINT8) ((SocketData->Route[DestSocId].ChaKtiRt >> (RootSocId * NodeEntriesWidth + PortFieldWidth)) & PortFieldMask);
      } else {
        PeerPort1 = (UINT8) ((SocketData->Route[DestSocId].ChaKtiRt1 >> ((RootSocId - 4) * NodeEntriesWidth)) & PortFieldMask);
        PeerPort2 = (UINT8) ((SocketData->Route[DestSocId].ChaKtiRt1 >> ((RootSocId - 4) * NodeEntriesWidth + PortFieldWidth)) & PortFieldMask);
      }
      if ((PeerPort1 < (KtiInternalGlobal->KtiPortCnt)) && (PeerPort2 < (KtiInternalGlobal->KtiPortCnt))) {
        OutPorts[0] = (UINT8) SocketData->Cpu[DestSocId].LinkData[PeerPort1].PeerPort;
        OutPorts[1] = (UINT8) SocketData->Cpu[DestSocId].LinkData[PeerPort2].PeerPort;
      }
    }
  } else {
    //
    // Root and Destination Nodes are at least 2 hops away (i.e. RootNode <--> IntermediateNode <--> ... <--> DestinationNode)
    // Make sure Destination Node is connected to Root Node via the same link as RootNode to IntermediateNode
    //
    if (IntermediateSocId < 4) {
      OutPorts[0] = (UINT8) ((SocketData->Route[RootSocId].ChaKtiRt >> (IntermediateSocId * NodeEntriesWidth)) & PortFieldMask);
      OutPorts[1] = (UINT8) ((SocketData->Route[RootSocId].ChaKtiRt >> (IntermediateSocId * NodeEntriesWidth + PortFieldWidth)) & PortFieldMask);
    } else {
      OutPorts[0] = (UINT8) ((SocketData->Route[RootSocId].ChaKtiRt1 >> ((IntermediateSocId - 4) * NodeEntriesWidth)) & PortFieldMask);
      OutPorts[1] = (UINT8) ((SocketData->Route[RootSocId].ChaKtiRt1 >> ((IntermediateSocId - 4) * NodeEntriesWidth + PortFieldWidth)) & PortFieldMask);
    }
  }

  Data32 = (UINT32) (OutPorts[0] | (OutPorts[1] << PortFieldWidth));

  if (DestSocId < 4) {
    SocketData->Route[RootSocId].ChaKtiRt |= (Data32 << (NodeEntriesWidth * DestSocId));
  } else {
    SocketData->Route[RootSocId].ChaKtiRt1 |= (Data32 << (NodeEntriesWidth * (DestSocId - 4)));
  }

  return KTI_SUCCESS;
}

/**

  Routing Mode C0 (SKX, CLX, ICX-R0): 8-entry table; indexing using 0,Mod3[1:0].

  This routine handles UPI Routing Mode C0 two sockets three links topology.
  The route table setting is stored in ChaKtiRt in SocketData structure.

  @param[in,out] SocketData        Pointer to socket specific data
  @param[in] RootSocId             Root Node node ID (a.k.a. socket ID)
  @param[in] DestSocId             Destination node node ID (a.k.a. socket ID)
  @param[in] OutPorts              Array that stores KTI ports of Root Node
  @param[in] PortFieldWidth        Port field width; this is SoC specific.
  @param[in] MultiLink             TRUE - Dual or triple or four links exists;
                                   FALSE - No Dual link nor triple link nor quad link

  @retval KTI_SUCCESS  ChaKtiRt or ChaKtiRt1 is populated with valid route table settings.

**/
KTI_STATUS
SetupUpiRoutingModeC0 (
  IN OUT KTI_SOCKET_DATA          *SocketData,
  IN     UINT8                     RootSocId,
  IN     UINT8                     DestSocId,
  IN     UINT8                     *OutPorts,
  IN     UINT8                     PortFieldWidth,
  IN     BOOLEAN                   MultiLink
  )
{
  //
  // KTIVAR->SysConfig == SYS_CONFIG_2S3L
  // Routing Mode C0 (SKX, CLX, ICX-R0): 8-entry table; indexing using 0,Mod3[1:0]
  // In the 2S-3link mode, the modulo three will be used as the *index* into the CHA's KTI route table.
  // So you can specify which link is connected to which on each socket by programming one socket to map 0->link0, 1->link1, and 2->link2
  // then program the other socket to map 0,1,2 to whichever links link0/link1/link2 are connected to.
  //

  KTI_ASSERT ((MultiLink), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);

  if (RootSocId != 0) {
    OutPorts[0] = (UINT8) SocketData->Cpu[0].LinkData[0].PeerPort;       // get the socket 0 link 0 peer port
    OutPorts[1] = (UINT8) SocketData->Cpu[0].LinkData[1].PeerPort;       // get the socket 0 link 1 peer port
    OutPorts[2] = (UINT8) SocketData->Cpu[0].LinkData[2].PeerPort;       // get the socket 0 link 2 peer port
  }
  SocketData->Route[RootSocId].ChaKtiRt = (OutPorts[0] | (OutPorts[1] << PortFieldWidth) | (OutPorts[2] << (PortFieldWidth * 2)));

  return KTI_SUCCESS;
}

/**

  16-entry table: Indexing using NodeId[1:0],mod3[1:0]. For CPX, SPR.
  That means, only Node ID 0 - 3 are used to index into the table.

  This routine handles UPI Routing Mode C1 single link route. For Destination NodeID 0 - 1,
  the route table setting is stored in ChaKtiRt in SocketData structure. For Destination
  NodeID 2 - 3, the route table setting is stored in ChaKtiRt1 in SocketData structure.

  @param[in,out] SocketData        Pointer to socket specific data
  @param[in] RootSocId             Root Node node ID (a.k.a. socket ID)
  @param[in] DestSocId             Destination node node ID (a.k.a. socket ID)
  @param[in] OutPorts              Array that stores KTI ports of Root Node
  @param[in] PortFieldWidth        Port field width; this is SoC specific.

  @retval KTI_SUCCESS  ChaKtiRt or ChaKtiRt1 is populated with valid route table settings.

**/
KTI_STATUS
SetupUpiRoutingModeC1SingleLink (
  IN OUT KTI_SOCKET_DATA          *SocketData,
  IN     UINT8                     RootSocId,
  IN     UINT8                     DestSocId,
  IN     UINT8                     *OutPorts,
  IN     UINT8                     PortFieldWidth
  )
{
  UINT32       FieldMask;
  UINT32       Data32;
  UINT32       BitOffset;
  UINT8        NodeEntriesWidth;

  //
  // 16-entry table: Indexing using NodeId[1:0],mod3[1:0]. That means, only Node ID 0 - 3 are used to index into the table.
  //

  if (SocketData->Cpu[DestSocId].SocType == SOCKET_TYPE_FPGA) {
    //
    // The index into the routing table uses only lower 2 bits of Node ID, but FPGA's node ID can be assigned
    // as 6 or 7, therefore we need to map out Bit-2.
    //
    DestSocId &= (BIT1 | BIT0);
  }

  if (DestSocId > 3) {
    KTI_ASSERT ((DestSocId <= 3), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_CPU_SOCKET_ID);
    return KTI_FAILURE;
  }

  NodeEntriesWidth = PortFieldWidth * 4; // Each node has 4 entries (3-bit wide per entry).

  Data32 = (UINT32) (OutPorts[0] | (OutPorts[0] << PortFieldWidth) | (OutPorts[0] << (PortFieldWidth * 2)) | (OutPorts[0] << (PortFieldWidth * 3)));

  if (DestSocId < 2) {
    BitOffset = NodeEntriesWidth * DestSocId;
    FieldMask = FLD_MASK (NodeEntriesWidth, BitOffset);
    SocketData->Route[RootSocId].ChaKtiRt &= ~FieldMask;
    SocketData->Route[RootSocId].ChaKtiRt |= (Data32 << BitOffset);
  } else {
    BitOffset = NodeEntriesWidth * (DestSocId - 2);
    FieldMask = FLD_MASK (NodeEntriesWidth, BitOffset);
    SocketData->Route[RootSocId].ChaKtiRt1 &= ~FieldMask;
    SocketData->Route[RootSocId].ChaKtiRt1 |= (Data32 << BitOffset);
  }

  return KTI_SUCCESS;
}

/**

  16-entry table: Indexing using NodeId[1:0],mod3[1:0]. For CPX, SPR.
  That means, only Node ID 0 - 3 are used to index into the table.

  This routine handles UPI Routing Mode C1 multi-link route. For Destination NodeID 0 - 1,
  the route table setting is stored in ChaKtiRt in SocketData structure. For Destination
  NodeID 2 - 3, the route table setting is stored in ChaKtiRt1 in SocketData structure.

  @param[in,out] SocketData        Pointer to socket specific data
  @param[in] KtiInternalGlobal     Pointer to KTIRC internal variables structure
  @param[in] RootSocId             Root Node node ID (a.k.a. socket ID)
  @param[in] DestSocId             Destination node node ID (a.k.a. socket ID)
  @param[in] OutPorts              Array to save KTI ports of Root Node
  @param[in] PortFieldWidth        Port field width; this is SoC specific.
  @param[in] PortFieldMask         Port field mask; this is SoC specific.

  @retval KTI_SUCCESS  ChaKtiRt or ChaKtiRt1 is populated with valid route table settings.

**/
KTI_STATUS
SetupUpiRoutingModeC1MultiLink (
  IN OUT KTI_SOCKET_DATA          *SocketData,
  IN     KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN     UINT8                     RootSocId,
  IN     UINT8                     DestSocId,
  IN     UINT8                     *OutPorts,
  IN     UINT8                     PortFieldWidth,
  IN     UINT8                     PortFieldMask
  )
{
  UINT32       Data32;
  UINT8        NodeEntriesWidth;
  UINT8        PeerPort1;
  UINT8        PeerPort2;
  UINT8        PeerPort3;

  //
  // Routing Mode C1 (CPX): 16-entry table; indexing using NodeId[1:0],Mod3[1:0]
  // KTIVAR->SysConfig == SYS_CONFIG_2S3L. This mode is used for 2S3L. Therefore the last entry in each Node
  // is a don't care field.
  //

  if (DestSocId > 3) {
    KTI_ASSERT ((DestSocId <= 3), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_CPU_SOCKET_ID);
    return KTI_FAILURE;
  }

  NodeEntriesWidth = PortFieldWidth * 4; // Each node has 4 entries (3-bit wide per entry).

  if ((RootSocId > DestSocId) && (SocketData->Cpu[DestSocId].SocType == SOCKET_TYPE_CPU)) {
    //
    // We are in backward path since the setup of Root Node routes always start with the Node with the lower Node ID.
    // Checking for CPU type ensures we don't read FPGA's Routing table since we do not program FPGA routing table.
    // Using DestSocId's ChaKtiRt tables to find out what ports were routed from DestSocId to RootSocId.
    // Then use the same link (PeerPort) to route from RootSocId back to DestSocId.
    //
    if (RootSocId < 2) {
      PeerPort1 = (UINT8) ((SocketData->Route[DestSocId].ChaKtiRt >> (RootSocId * NodeEntriesWidth)) & PortFieldMask);
      PeerPort2 = (UINT8) ((SocketData->Route[DestSocId].ChaKtiRt >> (RootSocId * NodeEntriesWidth + PortFieldWidth)) & PortFieldMask);
      PeerPort3 = (UINT8) ((SocketData->Route[DestSocId].ChaKtiRt >> (RootSocId * NodeEntriesWidth + 2 * PortFieldWidth)) & PortFieldMask);
    } else {
      PeerPort1 = (UINT8) ((SocketData->Route[DestSocId].ChaKtiRt1 >> ((RootSocId - 2) * NodeEntriesWidth)) & PortFieldMask);
      PeerPort2 = (UINT8) ((SocketData->Route[DestSocId].ChaKtiRt1 >> ((RootSocId - 2) * NodeEntriesWidth + PortFieldWidth)) & PortFieldMask);
      PeerPort3 = (UINT8) ((SocketData->Route[DestSocId].ChaKtiRt1 >> ((RootSocId - 2) * NodeEntriesWidth + 2 * PortFieldWidth)) & PortFieldMask);
    }
    if ((PeerPort1 < (KtiInternalGlobal->KtiPortCnt)) && (PeerPort2 < (KtiInternalGlobal->KtiPortCnt)) && (PeerPort3 < (KtiInternalGlobal->KtiPortCnt))) {
      OutPorts[0] = (UINT8) SocketData->Cpu[DestSocId].LinkData[PeerPort1].PeerPort;
      OutPorts[1] = (UINT8) SocketData->Cpu[DestSocId].LinkData[PeerPort2].PeerPort;
      OutPorts[2] = (UINT8) SocketData->Cpu[DestSocId].LinkData[PeerPort3].PeerPort;
    }
  }

  Data32 = (UINT32) (OutPorts[0] | (OutPorts[1] << PortFieldWidth) | (OutPorts[2] << (PortFieldWidth * 2)) | (OutPorts[0] << (PortFieldWidth * 3)));

  if (DestSocId < 2) {
    SocketData->Route[RootSocId].ChaKtiRt |= (Data32 << (NodeEntriesWidth * DestSocId));
  } else {
    SocketData->Route[RootSocId].ChaKtiRt1 |= (Data32 << (NodeEntriesWidth * (DestSocId - 2)));
  }

  return KTI_SUCCESS;
}

/**

  16-entry table: Indexing using NID[1:0], PA[13:12]^PA[7:6]. For SPR.
  That means, only Node ID 0 - 3 are used to index into the table.

  This routine handles UPI Routing Mode D0 single link route. For Destination NodeID 0 - 1,
  the route table setting is stored in ChaKtiRt in SocketData structure. For Destination
  NodeID 2 - 3, the route table setting is stored in ChaKtiRt1 in SocketData structure.

  @param[in,out] SocketData        Pointer to socket specific data
  @param[in] RootSocId             Root Node node ID (a.k.a. socket ID)
  @param[in] DestSocId             Destination node node ID (a.k.a. socket ID)
  @param[in] OutPorts              Array that stores KTI ports of Root Node
  @param[in] PortFieldWidth        Port field width; this is SoC specific.

  @retval KTI_SUCCESS  ChaKtiRt or ChaKtiRt1 is populated with valid route table settings.

**/
KTI_STATUS
SetupUpiRoutingModeD0SingleLink (
  IN OUT KTI_SOCKET_DATA          *SocketData,
  IN     UINT8                     RootSocId,
  IN     UINT8                     DestSocId,
  IN     UINT8                     *OutPorts,
  IN     UINT8                     PortFieldWidth
  )
{
  UINT32       FieldMask;
  UINT32       Data32;
  UINT32       BitOffset;
  UINT8        NodeEntriesWidth;

  //
  // 16-entry table: Indexing using NID[1:0], PA[13:12]^PA[7:6]. That means, only Node ID 0 - 3 are used to index into the table.
  //

  if (DestSocId > 3) {
    KTI_ASSERT ((DestSocId <= 3), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_CPU_SOCKET_ID);
    return KTI_FAILURE;
  }

  NodeEntriesWidth = PortFieldWidth * 4; // Each node has 4 entries (3-bit wide per entry).

  Data32 = (UINT32) (OutPorts[0] | (OutPorts[0] << PortFieldWidth) | (OutPorts[0] << (PortFieldWidth * 2)) | (OutPorts[0] << (PortFieldWidth * 3)));

  if (DestSocId < 2) {
    BitOffset = NodeEntriesWidth * DestSocId;
    FieldMask = FLD_MASK (NodeEntriesWidth, BitOffset);
    SocketData->Route[RootSocId].ChaKtiRt &= ~FieldMask;
    SocketData->Route[RootSocId].ChaKtiRt |= (Data32 << BitOffset);
  } else {
    BitOffset = NodeEntriesWidth * (DestSocId - 2);
    FieldMask = FLD_MASK (NodeEntriesWidth, BitOffset);
    SocketData->Route[RootSocId].ChaKtiRt1 &= ~FieldMask;
    SocketData->Route[RootSocId].ChaKtiRt1 |= (Data32 << BitOffset);
  }

  return KTI_SUCCESS;
}

/**

  16-entry table: Indexing using NID[1:0], PA[13:12]^PA[7:6]. Up to 4 links between nodes. For SPR.
  That also means, only Node ID 0 - 3 are used to index into the table.

  This routine handles UPI Routing Mode D0 multi-link route. For Destination NodeID 0 - 1,
  the route table setting is stored in ChaKtiRt in SocketData structure. For Destination
  NodeID 2 - 3, the route table setting is stored in ChaKtiRt1 in SocketData structure.

  @param[in,out] SocketData        Pointer to socket specific data
  @param[in] KtiInternalGlobal     Pointer to KTIRC internal variables structure
  @param[in] RootSocId             Root Node node ID (a.k.a. socket ID)
  @param[in] IntermediateSocId     Intermediate node ID (the node is either Destination Node
                                   itself or connected directly to RootNode)
  @param[in] DestSocId             Destination node node ID (a.k.a. socket ID)
  @param[in] OutPorts              Array that stores KTI ports of Root Node
  @param[in] PortFieldWidth        Port field width; this is SoC specific.
  @param[in] PortFieldMask         Port field mask; this is SoC specific.

  @retval KTI_SUCCESS  ChaKtiRt or ChaKtiRt1 is populated with valid route table settings.

**/
KTI_STATUS
SetupUpiRoutingModeD0MultiLink (
  IN OUT KTI_SOCKET_DATA          *SocketData,
  IN     KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN     UINT8                     RootSocId,
  IN     UINT8                     IntermediateSocId,
  IN     UINT8                     DestSocId,
  IN     UINT8                     *OutPorts,
  IN     UINT8                     PortFieldWidth,
  IN     UINT8                     PortFieldMask
  )
{
  UINT32       Data32;
  UINT8        NodeEntriesWidth;
  UINT8        PeerPort1;
  UINT8        PeerPort2;
  UINT8        PeerPort3;
  UINT8        PeerPort4;

  //
  // Four links found between nodes.
  // 16-entry table: Indexing using NID[1:0], PA[13:12]^PA[7:6].
  //

  NodeEntriesWidth = PortFieldWidth * 4; // Each node has 4 entries (3-bit wide per entry).

  if (DestSocId == IntermediateSocId) {
    //
    // Root and Destination Nodes are One hop away
    // Make sure forward and backward path are using the same link
    //
    if (RootSocId > DestSocId) {
      //
      // We are in backward path since the setup of Root Node routes always start with the Node with the lower Node ID.
      // Using DestSocId's ChaKtiRt tables to find out what ports were routed from DestSocId to RootSocId.
      // Then use the same link (PeerPort) to route from RootSocId back to DestSocId.
      //
      if (RootSocId < 2) {
        PeerPort1 = (UINT8) ((SocketData->Route[DestSocId].ChaKtiRt >> (RootSocId * NodeEntriesWidth)) & PortFieldMask);
        PeerPort2 = (UINT8) ((SocketData->Route[DestSocId].ChaKtiRt >> (RootSocId * NodeEntriesWidth + PortFieldWidth)) & PortFieldMask);
        PeerPort3 = (UINT8) ((SocketData->Route[DestSocId].ChaKtiRt >> (RootSocId * NodeEntriesWidth + PortFieldWidth * 2)) & PortFieldMask);
        PeerPort4 = (UINT8) ((SocketData->Route[DestSocId].ChaKtiRt >> (RootSocId * NodeEntriesWidth + PortFieldWidth * 3)) & PortFieldMask);
      } else {
        PeerPort1 = (UINT8) ((SocketData->Route[DestSocId].ChaKtiRt1 >> ((RootSocId - 2) * NodeEntriesWidth)) & PortFieldMask);
        PeerPort2 = (UINT8) ((SocketData->Route[DestSocId].ChaKtiRt1 >> ((RootSocId - 2) * NodeEntriesWidth + PortFieldWidth)) & PortFieldMask);
        PeerPort3 = (UINT8) ((SocketData->Route[DestSocId].ChaKtiRt1 >> ((RootSocId - 2) * NodeEntriesWidth + PortFieldWidth * 2)) & PortFieldMask);
        PeerPort4 = (UINT8) ((SocketData->Route[DestSocId].ChaKtiRt1 >> ((RootSocId - 2) * NodeEntriesWidth + PortFieldWidth * 3)) & PortFieldMask);
      }
      if ((PeerPort1 < (KtiInternalGlobal->KtiPortCnt)) && (PeerPort2 < (KtiInternalGlobal->KtiPortCnt)) &&
          (PeerPort3 < (KtiInternalGlobal->KtiPortCnt)) && (PeerPort4 < (KtiInternalGlobal->KtiPortCnt))) {
        OutPorts[0] = (UINT8) SocketData->Cpu[DestSocId].LinkData[PeerPort1].PeerPort;
        OutPorts[1] = (UINT8) SocketData->Cpu[DestSocId].LinkData[PeerPort2].PeerPort;
        OutPorts[2] = (UINT8) SocketData->Cpu[DestSocId].LinkData[PeerPort3].PeerPort;
        OutPorts[3] = (UINT8) SocketData->Cpu[DestSocId].LinkData[PeerPort4].PeerPort;
      }
    }
  }

  Data32 = (UINT32) (OutPorts[0] | (OutPorts[1] << PortFieldWidth) | (OutPorts[2] << (PortFieldWidth * 2)) | (OutPorts[3] << (PortFieldWidth * 3)));

  if (DestSocId < 2) {
    SocketData->Route[RootSocId].ChaKtiRt |= (Data32 << (NodeEntriesWidth * DestSocId));
  } else {
    SocketData->Route[RootSocId].ChaKtiRt1 |= (Data32 << (NodeEntriesWidth * (DestSocId - 2)));
  }

  return KTI_SUCCESS;
}

/**
  Determine the CHA KTI Route Table programming values based on the UPI routing mode.
  This routine populates the route table setting into ChaKtiRt and ChaKtiRt1 in
  SocketData structure.

  @param[in,out] SocketData        Pointer to socket specific data
  @param[in] KtiInternalGlobal     Pointer to KTIRC internal variables structure
  @param[in] RootSocId             Root Node node ID (a.k.a. socket ID)
  @param[in] IntermediateSocId     Intermediate node ID (the node is either Destination Node
                                   itself or connected directly to RootNode)
  @param[in] DestSocId             Destination node node ID (a.k.a. socket ID)
  @param[in] OutPorts              Array to save KTI ports of Root Node
  @param[in] MaxPorts              Max ports number in array OutPorts
  @param[in] MultiLink             TRUE - Dual or triple or four links exists;
                                   FALSE - No Dual link nor triple link nor quad link
  @param[in] AlternateRoute        TRUE indicates there is second route between Root Node (SocId)
                                   and Destination Node (DestSocId); therefore update the DestSocId's
                                   second entry with this route.
                                   FALSE otherwise.

  @retval KTI_SUCCESS  ChaKtiRt (and ChaKtiRt1) are populated with valid route table settings.

**/
KTI_STATUS
SetRootNodeRouteInChaKtiRouteTable (
  IN OUT KTI_SOCKET_DATA          *SocketData,
  IN     KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN     UINT8                     RootSocId,
  IN     UINT8                     IntermediateSocId,
  IN     UINT8                     DestSocId,
  IN     UINT8                     *OutPorts,
  IN     UINT8                     MaxPorts,
  IN     BOOLEAN                   MultiLink,
  IN     BOOLEAN                   AlternateRoute
  )
{
  UINT8        PortFieldWidth;
  UINT8        PortFieldMask;

  if (KtiInternalGlobal->CpuType == CPU_CPX || IsSprFamilyCpu (KtiInternalGlobal->CpuType)) {
    PortFieldWidth = 3;
    PortFieldMask = 7;
  } else {
    PortFieldWidth = 2;
    PortFieldMask = 3;
  }

  switch (KtiInternalGlobal->UpiRoutingMode) {
  case UpiRoutingModeA:
    SetupUpiRoutingModeA (SocketData, RootSocId, DestSocId, OutPorts, PortFieldWidth, MultiLink);
    break;

  case UpiRoutingModeB0:
    SetupUpiRoutingModeB0 (SocketData, KtiInternalGlobal, RootSocId, DestSocId, OutPorts, PortFieldWidth, PortFieldMask, MultiLink);
    break;

  case UpiRoutingModeB2:
    if (MultiLink) {
      SetupUpiRoutingModeB2MultiLink (SocketData, KtiInternalGlobal, RootSocId, IntermediateSocId, DestSocId, OutPorts, PortFieldWidth, PortFieldMask);
    } else {
      SetupUpiRoutingModeB2SingleLink (SocketData, RootSocId, DestSocId, OutPorts, PortFieldWidth, AlternateRoute);
    }
    break;

  case UpiRoutingModeC0:
    SetupUpiRoutingModeC0 (SocketData, RootSocId, DestSocId, OutPorts, PortFieldWidth, MultiLink);
    break;

  case UpiRoutingModeC1:
    if (MultiLink) {
      SetupUpiRoutingModeC1MultiLink (SocketData, KtiInternalGlobal, RootSocId, DestSocId, OutPorts, PortFieldWidth, PortFieldMask);
    } else {
      SetupUpiRoutingModeC1SingleLink (SocketData, RootSocId, DestSocId, OutPorts, PortFieldWidth);
    }
    break;

  case UpiRoutingModeD0:
    if (MultiLink) {
      SetupUpiRoutingModeD0MultiLink (SocketData, KtiInternalGlobal, RootSocId, IntermediateSocId, DestSocId, OutPorts, PortFieldWidth, PortFieldMask);
    } else {
      SetupUpiRoutingModeD0SingleLink (SocketData, RootSocId, DestSocId, OutPorts, PortFieldWidth);
    }
    break;

  default:
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n CHA UPI Routing Mode %d not supported\n", KtiInternalGlobal->UpiRoutingMode));
    KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
    break;
  }

  return KTI_SUCCESS;
}

/**
  Dump CHA KTI Route table programming values to output device

  @param[in] SocketData          Socket specific data structure
  @param[in] KtiInternalGlobal   KTIRC internal variables.

**/
VOID
ChaKtiRouteTableDump (
  IN KTI_SOCKET_DATA           *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
#if (KTI_DEBUG_LEVEL & KTI_DEBUG_INFO1)
  UINT8               Index, Index1;
  UINT32              Data32;
  UINT32              BitOffset;
  UINT8               Index2;
  BOOLEAN             PrintNodeSecondEntry;
  BOOLEAN             PrintNodeThirdEntry;
  BOOLEAN             PrintNodeForthEntry;
  UINT8               PortFieldWidth;
  UINT8               PortFieldMask;

  if (KtiInternalGlobal->CpuType == CPU_CPX || IsSprFamilyCpu (KtiInternalGlobal->CpuType)) {
    PortFieldWidth = 0x3;
    PortFieldMask  = 0x7;
  } else {
    PortFieldWidth = 2; // Two bit field to encode link 0, 1 & 2
    PortFieldMask  = 0x3;
  }

  //
  // Decides how many entries to display for source to each destination node
  //
  PrintNodeSecondEntry = FALSE;
  PrintNodeThirdEntry = FALSE;
  PrintNodeForthEntry = FALSE;

  switch (KtiInternalGlobal->UpiRoutingMode) {
  case UpiRoutingModeD0:
    PrintNodeForthEntry = TRUE;
  case UpiRoutingModeC1:
    PrintNodeThirdEntry = TRUE;
  case UpiRoutingModeB2:
    PrintNodeSecondEntry = TRUE;
    break;

  default:
    break;
  }

  //
  // Dump the Route Table
  //
  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if ((SocketData->Cpu[Index].Valid == FALSE) || (SocketData->Cpu[Index].SocType != SOCKET_TYPE_CPU)) {
      continue;
    }

    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\nCPU%u Routing Table (KTI_ROUTING_TABLE*_CHA_PMA_REG)", Index));
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n----------------------------------------------------"));
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDestSocket  Port"));

    for (Index1 = 0; Index1 < MAX_SOCKET; Index1++) {
      if (SocketData->Cpu[Index1].Valid == TRUE && SocketData->Cpu[Index1].SocType == SOCKET_TYPE_CPU && Index1 != Index) { // Exclude Self
        if (KtiInternalGlobal->UpiRoutingMode == UpiRoutingModeB2) {
          if (Index1 < 4) {
            BitOffset = PortFieldWidth * Index1 * 2;
            Data32 = SocketData->Route[Index].ChaKtiRt;
          } else {
            BitOffset = PortFieldWidth * (Index1 - 4) * 2;
            Data32 = SocketData->Route[Index].ChaKtiRt1;
          }
        } else if ((KtiInternalGlobal->UpiRoutingMode == UpiRoutingModeC1) ||
                   (KtiInternalGlobal->UpiRoutingMode == UpiRoutingModeD0)) {
          if (Index1 < 2) {
            BitOffset = PortFieldWidth * Index1 * 4;
            Data32 = SocketData->Route[Index].ChaKtiRt;
          } else {
            BitOffset = PortFieldWidth * (Index1 - 2) * 4;
            Data32 = SocketData->Route[Index].ChaKtiRt1;
          }
        } else {
          BitOffset = PortFieldWidth * Index1;
          Data32 = SocketData->Route[Index].ChaKtiRt;
        }
        KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n   CPU%u%7u", Index1, ((Data32 >> BitOffset) & PortFieldMask)));

        //
        // Display the second entry of the same destination node
        //
        if (PrintNodeSecondEntry) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n       %7u", ((Data32 >> (BitOffset + PortFieldWidth)) & PortFieldMask)));
        }

        //
        // Display the third entry of the same destination node
        //
        if (PrintNodeThirdEntry) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n       %7u", ((Data32 >> (BitOffset + PortFieldWidth * 2)) & PortFieldMask)));
        }

        //
        // Display the forth entry of the same destination node
        //
        if (PrintNodeForthEntry) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n       %7u", ((Data32 >> (BitOffset + PortFieldWidth * 3)) & PortFieldMask)));
        }
      }

      if (SocketData->Cpu[Index1].SocType == SOCKET_TYPE_FPGA) {
        if (KtiInternalGlobal->UpiRoutingMode == UpiRoutingModeB2) {
          if (Index1 < 4) {
            BitOffset = PortFieldWidth * Index1 * 2;
            Data32 = SocketData->Route[Index].ChaKtiRt;
          } else {
            BitOffset = PortFieldWidth * (Index1 - 4) * 2;
            Data32 = SocketData->Route[Index].ChaKtiRt1;
          }
          KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n   PGA%u%7u", Index1, ((Data32 >> BitOffset) & PortFieldMask)));
          //
          // Display the second entry of the same destination node
          //
          KtiDebugPrintInfo0 ((
            KTI_DEBUG_INFO0,
            "\n       %7u",
            ((Data32 >> (BitOffset + PortFieldWidth)) & PortFieldMask)
            ));
        } else if (KtiInternalGlobal->UpiRoutingMode == UpiRoutingModeC1) {
          Index2 = Index1 & (BIT0 | BIT1);
          if (Index2 < 2) {
            BitOffset = PortFieldWidth * Index2 * 4;
            Data32 = SocketData->Route[Index].ChaKtiRt;
          } else {
            BitOffset = PortFieldWidth * (Index2 - 2) * 4;
            Data32 = SocketData->Route[Index].ChaKtiRt1;
          }
          KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n   PGA%u%7u", Index1, ((Data32 >> BitOffset) & PortFieldMask)));
        }
      }
    }
  }

#endif // (KTI_DEBUG_LEVEL & KTI_DEBUG_INFO1)
}

/**
  Dump KTI Agent Route table programming values to output device

  @param[in] SocketData          Socket specific data structure
  @param[in] KtiInternalGlobal   KTIRC internal variables.

**/
VOID
KtiAgentRouteTableDump (
  IN KTI_SOCKET_DATA           *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
#if (KTI_DEBUG_LEVEL & KTI_DEBUG_INFO1)
  UINT8               Index;
  UINT8               Index1;
  UINT32              Data32;
  UINT8               Port;
  UINT8               OutPort;
  UINT8               PortFieldWidth;
  UINT8               PortFieldMask;

  if (KtiInternalGlobal->CpuType == CPU_CPX) {
    PortFieldWidth = 0x3;
    PortFieldMask  = 0x7;
  } else {
    PortFieldWidth = 2; // Two bit field to encode link 0, 1, 2 & 3
    PortFieldMask  = 0x3;
  }

  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if ((SocketData->Cpu[Index].Valid == FALSE) || (SocketData->Cpu[Index].SocType != SOCKET_TYPE_CPU)) {
      continue;
    }

    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\nCPU%u KTIQRT_KTI_LLPMON_REG programming (KtiAgentRt):", Index));
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n----------------------------------------------------"));
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nPort  CPU0  CPU1  CPU2  CPU3  CPU4  CPU5  CPU6  CPU7"));

    for (Port = 0; Port < KtiInternalGlobal->KtiPortCnt; Port++) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n  %u   ", Port));
      for (Index1 = 0; Index1 < MAX_SOCKET; Index1++) {
        if (SocketData->Cpu[Index1].Valid == TRUE) {
          Data32 = PortFieldWidth * Index1;
          OutPort = (UINT8)((SocketData->Route[Index].KtiAgentRt[Port] >> Data32) & PortFieldMask);
          KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, " %-5u", OutPort));
        } else {
          KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, " -    "));
        }
      }
    }
  }
#endif // (KTI_DEBUG_LEVEL & KTI_DEBUG_INFO1)
}

/**
  Dump M3KTI Route table programming values to output device

  @param[in] SocketData          Socket specific data structure
  @param[in] KtiInternalGlobal   KTIRC internal variables.

**/
VOID
M3KtiRouteTableDump (
  IN KTI_SOCKET_DATA           *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
#if (KTI_DEBUG_LEVEL & KTI_DEBUG_INFO1)
  UINT8               Index;
  UINT8               Index1;
  UINT8               InPort;
  UINT8               M3KtiNum;
  UINT32              Data32;
  UINT32              M3KtiRt;
  UINT8               OutPort;
  BOOLEAN             IsCpx;
  UINT8               PortFieldWidth;
  UINT8               PortFieldMask;


  if (KtiInternalGlobal->CpuType == CPU_CPX) {
    IsCpx = TRUE;
    PortFieldWidth = 0x3;
    PortFieldMask  = 0x7;
  } else {
    IsCpx = FALSE;
    PortFieldWidth = 2; // Two bit field to encode link 0, 1, 2 & 3
    PortFieldMask  = 0x3;
  }

  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if ((SocketData->Cpu[Index].Valid == FALSE) || (SocketData->Cpu[Index].SocType != SOCKET_TYPE_CPU)) {
      continue;
    }

    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\nCPU%u M3KTI Route Table (M3KKRT*_M3KTI_MAIN_REG)", Index));
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n----------------------------------------------------------------"));
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nInPort  M3KtiNum  CPU0  CPU1  CPU2  CPU3  CPU4  CPU5  CPU6  CPU7"));

    for (InPort = 0; InPort < KtiInternalGlobal->KtiPortCnt; InPort++) {
      if (KTIVAR->SharedKtiPort0And1) {
        M3KtiNum = PORT_TO_M3KTI (InPort);
        if ((InPort & 1) != 0) {
          //
          // Odd Port ID on this M3KTI
          //
          if (IsCpx) {
            M3KtiRt = SocketData->Route[Index].M3KtiRt1[M3KtiNum];
          } else {
            M3KtiRt = SocketData->Route[Index].M3KtiRt[M3KtiNum] >> 16;
          }
        } else {
          M3KtiRt = SocketData->Route[Index].M3KtiRt[M3KtiNum];
        }
      } else {
        M3KtiNum = InPort;
        M3KtiRt = SocketData->Route[Index].M3KtiRt[M3KtiNum];
      }

      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n%-10u%-8u", InPort, M3KtiNum));

      for (Index1 = 0; Index1 < MAX_SOCKET; Index1++) {
        if (SocketData->Cpu[Index1].Valid == TRUE) {
          Data32 = PortFieldWidth * Index1;
          OutPort = (UINT8)((M3KtiRt >> Data32) & PortFieldMask);
          KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, " %-5u", OutPort));
        } else {
          KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, " -    "));
        }
      }
    }
  }

#endif // (KTI_DEBUG_LEVEL & KTI_DEBUG_INFO1)
}

/**
  Dump M2PCIe route table to output device

  @param SocketData        - socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval VOID

**/
VOID
M2PcieRouteTableDump (
  IN KTI_SOCKET_DATA           *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
#if (KTI_DEBUG_LEVEL & KTI_DEBUG_INFO1)
  UINT8               Index;
  UINT8               Stack;
  UINT8               Index1;
  UINT32              Data32;
  UINT8               OutPort;
  UINT8               PortFieldWidth;
  UINT8               PortFieldMask;
  BOOLEAN             M2PciePresent;

  if (KtiInternalGlobal->CpuType == CPU_CPX) {
    PortFieldWidth = 0x3;
    PortFieldMask  = 0x7;
  } else {
    PortFieldWidth = 2; // Two bit field to encode link 0, 1 & 2
    PortFieldMask  = 0x3;
  }

  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if ((SocketData->Cpu[Index].Valid == FALSE) || (SocketData->Cpu[Index].SocType != SOCKET_TYPE_CPU)) {
      continue;
    }
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\nCPU%u M2Pcie UPI Route Table (UPI_RT_M2UPCIE_MAIN_REG)", Index));
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n-------------------------------------------------------------"));
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nM2PcieIndex  CPU0  CPU1  CPU2  CPU3  CPU4  CPU5  CPU6  CPU7"));
    for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n%-13u", Stack));
      M2PciePresent = ((KtiInternalGlobal->M2PcieActiveBitmap[Index] & (1 << Stack)) == 0) ? FALSE : TRUE;
      Data32 = SocketData->Route[Index].M2PcieKtiRt[Stack];

      for (Index1 = 0; Index1 < MAX_SOCKET; Index1++) {
        if (M2PciePresent && (SocketData->Cpu[Index1].Valid == TRUE)) {
          OutPort = (UINT8)((Data32 >> (PortFieldWidth * Index1)) & PortFieldMask);
          KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, " %-5u", OutPort));
        } else {
          KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, " -    "));
        }
      }
    }
  }

#endif // (KTI_DEBUG_LEVEL & KTI_DEBUG_INFO1)
}

/**
  Rearrange topology tree for 8-socket Almost Fully Connect topology if necessary
  so that the request and the response routes travel through the same intermediate
  node. The adjustment is only required if there are two different routes that
  connect two nodes. This routine only makes adjustment to 2-hop away node, since
  8-socket Almost Fully Connect topology only performs maximum of 2 hops.

  @param[in] KtiInternalGlobal       KTIRC internal variables.
  @param[in,out] DiscEngData         Pointer to socket specific data

  @retval KTI_SUCCESS     Either no rearrangement needed or the rearrangment is done
                          successfully.

**/
STATIC
KTI_STATUS
Rearrange8sAlmostFullyConnectTopologyTree (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_DISC_DATA            *DiscEngData
  )
{
  UINT8               SbspSocId;
  UINT8               RootNodeId;
  UINT8               DestNodeId;
  UINT8               RootTreeIdx;
  UINT8               DestTreeIdx;
  UINT8               Idx;
  TOPOLOGY_TREE_NODE  RootIntermediateNode;
  TOPOLOGY_TREE_NODE  DestIntermediateNode;
  TOPOLOGY_TREE_NODE  RootNodeAltCpuTree;
  BOOLEAN             FoundInDestCpuTree;

  SbspSocId = KtiInternalGlobal->SbspSoc;

  if (KTIVAR->SysConfig != SYS_CONFIG_8S) {
    return KTI_SUCCESS;
  }

  if (KtiInternalGlobal->EightSocketTopology != Topo8SAlmostFullyConnect) {
    return KTI_SUCCESS;
  }

  //
  // Start from the Node with higher Node ID and travel down; exclude SBSP socket
  //
  for (RootNodeId = (MAX_SOCKET - 1); RootNodeId > 0; RootNodeId--) {
    if ((DiscEngData->Cpu[RootNodeId].Valid == FALSE) || (DiscEngData->Cpu[RootNodeId].SocType != SOCKET_TYPE_CPU)) {
      continue;
    }


    for (Idx = 0; Idx < MAX_TREE_NODES; Idx++) {
      RootTreeIdx = Idx;
      if (DiscEngData->CpuTree[RootNodeId][RootTreeIdx].Node.Valid == FALSE) {
        //
        // If there are no more valid nodes, proceed to the next RootNode
        //
        break;
      }

      if (DiscEngData->CpuTree[RootNodeId][RootTreeIdx].Node.Hop != 2) {
        continue;
      }

      //
      // Skip if there is only one route between the Root Node and Destination Node.
      //
      if (DiscEngData->AlternateCpuTree[RootNodeId][RootTreeIdx].Node.Valid == FALSE) {
        continue;
      }

      //
      // Since we begin on RootNode with higher Node ID, skip if the Destination Node ID is higher
      // than the Root Node ID; this means that we were done with one direction and are now going
      // on an opposite direction
      //
      DestNodeId = (UINT8) DiscEngData->CpuTree[RootNodeId][RootTreeIdx].Node.SocId;

      if (RootNodeId <= DestNodeId) {
        continue;
      }

      if (DestNodeId >= MAX_SOCKET) {
        KTI_ASSERT ((DestNodeId < MAX_SOCKET), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_CPU_SOCKET_ID);
        return KTI_FAILURE;
      }

      GetParentSocket (DiscEngData->CpuTree[RootNodeId], &DiscEngData->CpuTree[RootNodeId][RootTreeIdx], &RootIntermediateNode);

      //
      // Find the equivalent route in Destination Node's topology tree
      //

      FoundInDestCpuTree = FALSE;

      for (DestTreeIdx = 0; DestTreeIdx < MAX_TREE_NODES; DestTreeIdx++) {
        if (DiscEngData->CpuTree[DestNodeId][DestTreeIdx].Node.Valid == FALSE) {
          //
          // If there are no more valid nodes, it means we are done searching in DestNode's toplogy tree
          //
          break;
        }

        if (DiscEngData->CpuTree[DestNodeId][DestTreeIdx].Node.Hop != 2) {
          continue;
        }

        if (DiscEngData->CpuTree[DestNodeId][DestTreeIdx].Node.SocId == RootNodeId) {
          //
          // Found the route that is connecting DestNode <===> RootNode
          //
          FoundInDestCpuTree = TRUE;
          break;
        }
      }

      if (!FoundInDestCpuTree) {
        //
        // Didn't find any similar route in DestNode's topology tree.
        // Nothing else to do with this RootNode. Proceed to the next RootNode.
        //
        continue;
      }

      GetParentSocket (DiscEngData->CpuTree[DestNodeId], &DiscEngData->CpuTree[DestNodeId][DestTreeIdx], &DestIntermediateNode);

      //
      // Verify if DestNode===>RootNode and RootNode=====>DestNode routes are travelling through
      // the same intermediate Node.
      // If they are different, on the RootNode's topology tree, swap the first and second routes entries
      //
      if (DestIntermediateNode.Node.SocId != RootIntermediateNode.Node.SocId) {
        RootNodeAltCpuTree.RawNode = DiscEngData->CpuTree[RootNodeId][RootTreeIdx].RawNode;
        DiscEngData->CpuTree[RootNodeId][RootTreeIdx].RawNode = DiscEngData->AlternateCpuTree[RootNodeId][RootTreeIdx].RawNode;
        DiscEngData->AlternateCpuTree[RootNodeId][RootTreeIdx].RawNode = RootNodeAltCpuTree.RawNode;
      }
    }

  }
  return KTI_SUCCESS;
}

/**
  This routine selects which UPI link between the Root Node and Intermediate
  Node to route the traffic to FPGA node (destination node). This routine applies
  to the case where there are multiple links between Root Node and Intermediate
  Node, but only single link between Intermediate Node and Destination Node.

  The caller already knows there are at least dual or triple links between Root and
  Intermediate Nodes before calling this routine.

  @param[in] KtiInternalGlobal        KTIRC internal variables.
  @param[in] SocketData               Pointer to socket specific data
  @param[in] Interm                   Intermediate CPU node
  @param[in] Dest                     Destination node
  @param[in,out] Port1                First UPI link between Root and Intermediate Nodes
  @param[in,out] Port2                Second UPI link between Root and Intermediate Nodes
  @param[in,out] Port3                Third UPI link between Root and Intermediate Nodes
  @param[in,out] UseDualOrTripleLink  Set to TRUE so that route between Root and Intermediate
                                      targeted Destination Node is setup as single link only.

  @retval KTI_SUCCESS     Either no reselection of UPI port needed or UPI port is selected
                          successfully based on SysConfig criteria.

**/
STATIC
KTI_STATUS
SelectUpiPortForRouteFrRootNodeToFpga (
  IN     KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN     KTI_SOCKET_DATA            *SocketData,
  IN     TOPOLOGY_TREE_NODE         *Interm,
  IN     TOPOLOGY_TREE_NODE         *Dest,
  IN OUT UINT8                      *Port1,
  IN OUT UINT8                      *Port2,
  IN OUT UINT8                      *Port3,
  IN OUT BOOLEAN                    *UseDualOrTripleLink
  )
{
  if (Dest->Node.SocType != SOCKET_TYPE_FPGA) {
    return KTI_SUCCESS;
  }

  if ((Dest->Node.Hop < 2) || MoreThanOneLinkExists (KtiInternalGlobal, SocketData->Cpu, Interm, Dest)) {
    //
    // Nothing to do, as we only need to reselect the route for destination that is 2 hops away;
    // and there are multiple links between RootNode and IntermediateNode, but only single link
    // between IntermediateNode and DestinationNode
    //
    return KTI_SUCCESS;
  }

  if (KTIVAR->SysConfig == SYS_CONFIG_4S) {
    //
    // 4S2FPGA
    // Use second UPI port out of duallink to improve performance
    //
    *Port3 = *Port1 = *Port2;
  } else {
    //
    // For 2S1FPGA or 2S2FPGA:
    //
    if ((Interm->Node.SocId & BIT0) == 0) {
      //
      // CPU Node on the left use the first port
      //
      *Port3 = *Port2 = *Port1;
    } else {
      *Port3 = *Port1 = *Port2;
    }
  }
  *UseDualOrTripleLink = FALSE;

  return KTI_SUCCESS;
}

/**
  This routine selects which UPI port on the Intermediate Node to pass through
  the traffic to FPGA node (destination node). This routine applies
  to the case where there are multiple links between Root Node and Intermediate
  Node, but only single link between Intermediate Node and Destination Node.

  @param[in] KtiInternalGlobal        KTIRC internal variables.
  @param[in] SocketData               Pointer to socket specific data
  @param[in] Grandparent              Root node
  @param[in] Parent                   Intermediate CPU node
  @param[in] Child                    Destination node
  @param[in,out] ParentPort           UPI port on the intermediate node to route the
                                      traffic to Destination node

  @retval KTI_SUCCESS     Either no reselection of UPI port needed or ParentPort is updated
                          successfully based on SysConfig criteria.

**/
STATIC
KTI_STATUS
SelectUpiPortForPassThruRouteToFpga (
  IN     KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN     KTI_SOCKET_DATA            *SocketData,
  IN     TOPOLOGY_TREE_NODE         *Grandparent,
  IN     TOPOLOGY_TREE_NODE         *Parent,
  IN     TOPOLOGY_TREE_NODE         *Child,
  IN OUT UINT8                      *ParentPort
  )
{
  UINT8                       GrandparentPort[4];
  UINT8                       GrandparentNodeId;

  if (Child->Node.SocType != SOCKET_TYPE_FPGA) {
    return KTI_SUCCESS;
  }

  if (MoreThanOneLinkExists (KtiInternalGlobal, SocketData->Cpu, Grandparent, Parent)) {
    GetMultiLinkFromLEP (
      KtiInternalGlobal,
      SocketData->Cpu,
      Grandparent,
      Parent,
      GrandparentPort,
      sizeof(GrandparentPort) / sizeof(GrandparentPort[0])
      );
  } else {
    return KTI_SUCCESS;
  }

  GrandparentNodeId = (UINT8) Grandparent->Node.SocId;

  if (KTIVAR->SysConfig == SYS_CONFIG_4S) {
    //
    // 4S2FPGA
    // Use second UPI link between Grandparent and parent to route the traffic to FPGA for performance
    //
    *ParentPort = (UINT8) SocketData->Cpu[GrandparentNodeId].LinkData[GrandparentPort[1]].PeerPort;
  } else {
    //
    // For 2S1FPGA or 2S2FPGA
    //
    if ((Parent->Node.SocId & BIT0) != 0) {
      //
      // Parent Node on the right use the second UPI link between Grandparent and parent
      // to route the traffic to FPGA, to balance the traffic
      //
      *ParentPort = (UINT8) SocketData->Cpu[GrandparentNodeId].LinkData[GrandparentPort[1]].PeerPort;
    }
  }

  return KTI_SUCCESS;
}

/**

  This routine disables P2P traffic balancing for route with multiple links
  to improve remote P2P traffic performance. It uses the first active link
  found between Root and Destination Nodes for P2P traffic.
  This routine updates SocketData->Route[SocId].M2PcieKtiRt[StackId].

  @param[in,out] SocketData        Pointer to socket specific data
  @param[in] KtiInternalGlobal     KTIRC internal variables.
  @param[in] RootSocId             Root Node node ID
  @param[in] DestSocId             Destination Node node ID
  @param[in] OutPorts              Array that stores KTI ports of Root Node
  @param[in] PortFieldWidth        Port field width; this is SoC specific.
  @param[in] MultiLink             TRUE - Dual or triple or four links exists;
                                   FALSE - No Dual link nor triple link nor quad link
  @param[in] AlternateRoute        TRUE indicates there is second route between Root Node (SocId)
                                   and Destination Node (DestSocId). FALSE otherwise.

  @retval KTI_SUCCESS    M2PcieKtiRt tables are updated with valid routing.
  @retval KTI_IGNORE     The route passed into this function is neither multi-link nor multiple routes.

**/
KTI_STATUS
DisableP2pTrafficBalancingForMultiLink (
  IN OUT KTI_SOCKET_DATA          *SocketData,
  IN     KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN     UINT8                     RootSocId,
  IN     UINT8                     DestSocId,
  IN     UINT8                     *OutPorts,
  IN     UINT8                     PortFieldWidth,
  IN     BOOLEAN                   MultiLink,
  IN     BOOLEAN                   AlternateRoute
  )
{
  UINT8   Stack;
  UINT8   PeerPort;
  UINT32  FieldMask;

  if (!MultiLink && !AlternateRoute) {
    return KTI_IGNORE;
  }

  PeerPort = KTI_INVALID_PORT;

  if (RootSocId > DestSocId) {
    //
    // We are in backward path since the setup of Root Node routes always start
    // with the Node with lower Node ID. Using DestSocId's M2PcieKtiRt table to
    // find out what port was already setup to route from DestSocId (lower Node
    // ID) to RootSocId (higher Node ID). Then use the same link to route from
    // RootSocId back to DestSocId.
    //
    FieldMask = FLD_MASK (PortFieldWidth, (PortFieldWidth * RootSocId));
    for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
      if ((KtiInternalGlobal->M2PcieActiveBitmap[DestSocId] & (1 << Stack)) != 0) {
        PeerPort = (UINT8) ((SocketData->Route[DestSocId].M2PcieKtiRt[Stack] & FieldMask) >> (RootSocId * PortFieldWidth));
        break;
      }
    }

    if (PeerPort < KtiInternalGlobal->KtiPortCnt) {
      OutPorts[0] = (UINT8) SocketData->Cpu[DestSocId].LinkData[PeerPort].PeerPort;
    }
  }

  FieldMask = FLD_MASK (PortFieldWidth, (PortFieldWidth * DestSocId));
  for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
    if ((KtiInternalGlobal->M2PcieActiveBitmap[RootSocId] & (1 << Stack)) == 0) {
      continue;
    }

    SocketData->Route[RootSocId].M2PcieKtiRt[Stack] &= ~FieldMask;
    SocketData->Route[RootSocId].M2PcieKtiRt[Stack] |= (OutPorts[0] << (PortFieldWidth * DestSocId));
  }

  return KTI_SUCCESS;
}

/**

  This routine splits M2IOSF traffics across multiple UPI links, for P2P traffic improvement.
  This routine updates SocketData->Route[SocId].M2PcieKtiRt[StackId].

  @param[in,out] SocketData        Pointer to socket specific data
  @param[in] KtiInternalGlobal     KTIRC internal variables.
  @param[in] RootSocId             Root Node node ID
  @param[in] DestSocId             Destination Node node ID
  @param[in] OutPorts              Array that stores KTI ports of Root Node
  @param[in] OutPortsSize          Number of elements in OutPorts
  @param[in] PortFieldWidth        Port field width; this is SoC specific.
  @param[in] MultiLink             TRUE - Dual or triple or four links exists;
                                   FALSE - No Dual link nor triple link nor quad link
  @param[in] AlternateRoute        TRUE indicates there is second route between Root Node (SocId)
                                   and Destination Node (DestSocId). FALSE otherwise.

  @retval KTI_SUCCESS           M2PcieKtiRt tables are updated with valid routing.
  @retval KTI_FAILURE           OutPorts does not provide valid UPI links.
                                M2PcieKtiRt tables are not updated.

**/
KTI_STATUS
SetupM2PcieUpiRt (
  IN OUT KTI_SOCKET_DATA          *SocketData,
  IN     KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN     UINT8                     RootSocId,
  IN     UINT8                     DestSocId,
  IN     UINT8                     *OutPorts,
  IN     UINT8                     OutPortsSize,
  IN     UINT8                     PortFieldWidth,
  IN     BOOLEAN                   MultiLink,
  IN     BOOLEAN                   AlternateRoute
  )
{
  UINT32      FieldMask;
  UINT8       i;
  UINT8       Stack;
  UINT8       TotalActiveLinks;
  KTI_STATUS  Status;

  FieldMask = FLD_MASK (PortFieldWidth, (PortFieldWidth * DestSocId));

  if ((KtiInternalGlobal->CpuType == CPU_CPX) && (KTIVAR->SysConfig == SYS_CONFIG_4S) && !KtiInternalGlobal->Is4SRing &&
      (GetDieCount (KtiInternalGlobal->SbspSoc) != TWO_DIE_IN_SOCKET) && KtiInternalGlobal->FpgaList == 0) {
    //
    // Turn off P2P traffic balancing to increase performance for remote P2P in 4S fully connect (non-mcp)
    //
    Status = DisableP2pTrafficBalancingForMultiLink (
               SocketData,
               KtiInternalGlobal,
               RootSocId,
               DestSocId,
               OutPorts,
               PortFieldWidth,
               MultiLink,
               AlternateRoute
               );
    if (Status == KTI_SUCCESS) {
      return Status;
    }
  }

  if (!MultiLink) {
    //
    // Only single link between Root and Destination Nodes
    //
    i = 0;
    if (AlternateRoute) {
      //
      // If there are two routes between Root Node and Destination Node (such as in 8S almost fully
      // connect topology), assign second half of stacks to use this route.
      //
      i = KtiInternalGlobal->TotActiveM2Pcie[RootSocId] / 2;
    }

    for (Stack = i; Stack < MAX_IIO_STACK; Stack++) {
      if ((KtiInternalGlobal->M2PcieActiveBitmap[RootSocId] & (1 << Stack)) == 0) {
        continue;
      }

      SocketData->Route[RootSocId].M2PcieKtiRt[Stack] &= ~FieldMask;
      SocketData->Route[RootSocId].M2PcieKtiRt[Stack] |= (OutPorts[0] << (PortFieldWidth * DestSocId));
    }
    return KTI_SUCCESS;
  }

  //
  // Multi links between Root Node and its immediate neighbor (Destination Node)
  //

  for (i = 0, TotalActiveLinks = 0; i < OutPortsSize; i++) {
    if (OutPorts[i] != KTI_INVALID_PORT) {
      TotalActiveLinks++;
    }
  }

  if (TotalActiveLinks == 0) {
    KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
    return KTI_FAILURE;
  }

  //
  // Affinitize MS2IOSF source to UPI port if possible
  //
  if (KTI_SUCCESS == AffinitizeM2IosfSourceToUpiPort (
                       SocketData,
                       KtiInternalGlobal,
                       RootSocId,
                       DestSocId,
                       OutPorts,
                       TotalActiveLinks,
                       PortFieldWidth
                       )) {
    return KTI_SUCCESS;
  }

  //
  // Split MS2IOSF source to UPI port based on available UPI links without considering the
  // affinity between MS2IOSF source to UPI port
  //
  for (Stack = 0, i = 0; Stack < MAX_IIO_STACK; Stack++) {
    if ((KtiInternalGlobal->M2PcieActiveBitmap[RootSocId] & (1 << Stack)) == 0) {
      continue;
    }

    SocketData->Route[RootSocId].M2PcieKtiRt[Stack] &= ~FieldMask;
    SocketData->Route[RootSocId].M2PcieKtiRt[Stack] |= (OutPorts[i] << (PortFieldWidth * DestSocId));

    if (++i >= TotalActiveLinks) {
      //
      // restart from OutPorts[0]
      //
      i = 0;
    }
  }

  return KTI_SUCCESS;
}
