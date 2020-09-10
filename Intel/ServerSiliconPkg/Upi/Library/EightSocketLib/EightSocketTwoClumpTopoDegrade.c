/** @file

  Topology degrade implementation for 8 sockets that may have two clumps(4S Fully Connected).

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <KtiMisc.h>
#include <KtiSetupDefinitions.h>
#include <Library/UncoreLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/KtiApi.h>
#include <Library/MultiSocketLib.h>
#include "EightSocketTopoDegradeSupport.h"
#include <Library/CpuAndRevisionLib.h>

/**
  This routine rearranges the nodes in each 4-socket cube (a.k.a. clump) into
  the order of a ring format, and then verify UPI Port 3 is the cross links
  within each fully connected cubes.
  Note: This routine must avoid changing the order of nodes in SourceClumps[][].

  @param[in] SocketData         Socket specific data structure
  @param[in] KtiInternalGlobal  KTIRC internal variables.
  @param[in] SourceClumps       Pointer to the buffer that contains all sockets
                                in two fully connected cubes.

  @retval TRUE:  The system meets 8-socket twisted cubes fully connect cubes topology.
          FALSE: This system cannot be setup as 8-socket twisted cubes fully connect cubes.
**/
BOOLEAN
Is8STwistedFullyConnectCubes (
  IN  KTI_SOCKET_DATA           *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  UINT8                     (*SourceClumps)[SOCKETS_NUMBER_IN_EACH_CLUMP]
  )
{
  UINT8     Idx1;
  UINT8     Idx2;
  UINT8     ClumpNode0;
  UINT8     ClumpNode1;
  UINT8     ClumpNode2;
  UINT8     ClumpNode3;
  UINT8     Temp;
  UINT8     Clumps[MAX_CLUMPS_NUMBER_IN_8S_TOPOLOGY][SOCKETS_NUMBER_IN_EACH_CLUMP];

  if (KtiInternalGlobal->KtiPortCnt < 4) {
    return FALSE;
  }

  CopyMem ((VOID *) Clumps, (VOID *) SourceClumps, sizeof (Clumps));

  for (Idx1 = 0; Idx1 < MAX_CLUMPS_NUMBER_IN_8S_TOPOLOGY; Idx1++) {
    ClumpNode0 = Clumps[Idx1][0];

    //
    // Rearrange the nodes in this clump so that the order of the nodes
    // are arranged in a ring format
    //
    for (Idx2 = 1; Idx2 < SOCKETS_NUMBER_IN_EACH_CLUMP; Idx2++) {
      //
      // Node at index 2 of the ring is always connected to port 3 of Node at Index 0
      //
      if (Clumps[Idx1][Idx2] == SocketData->Cpu[ClumpNode0].LinkData[CROSSLINK_PORT_ID].PeerSocId) {
        if (Idx2 != 2) {
          Temp = Clumps[Idx1][2];
          Clumps[Idx1][2] = Clumps[Idx1][Idx2];
          Clumps[Idx1][Idx2] = Temp;
        }
        break;
      }
    }

    ClumpNode1 = Clumps[Idx1][1];
    ClumpNode2 = Clumps[Idx1][2];
    ClumpNode3 = Clumps[Idx1][3];

    if ((SocketData->Cpu[ClumpNode1].LinkData[CROSSLINK_PORT_ID].PeerSocId != ClumpNode3) ||
        (SocketData->Cpu[ClumpNode2].LinkData[CROSSLINK_PORT_ID].PeerSocId != ClumpNode0) ||
        (SocketData->Cpu[ClumpNode3].LinkData[CROSSLINK_PORT_ID].PeerSocId != ClumpNode1) ||
        (SocketData->Cpu[ClumpNode0].LinkData[CROSSLINK_PORT_ID].PeerSocId != ClumpNode2)) {
      return FALSE;
    }
  }

  return TRUE;
}

/*
  Routine to check if the input configuration is possible to be a 8S-2Clump topology.
  A topology cannot be 8S-2Clump due to below reasons:
  (1) Total Socket count < 8
  (2) Any socket has >5 actived UPI links ==> Could be AFC or DPW
  (3) Any socket has >1 DualLinks ==> Must be DPW
  (4) Any two sockets have >2 common peers ==> Must be AFC

  @param[in] SocInfo        - The pointer to the array of SOCKET_TOPOLOGY_INFO type

  @retval    TRUE           - It is possible to be 8S-2Clump topology, but need more check.
  @retval    FALSE          - It is impossible to be 8S-2Clump topology.
*/
BOOLEAN
EFIAPI
IsEightSocketTwoClumpPossible (
  IN  SOCKET_TOPOLOGY_INFO  *SocInfo
  )
{
  UINT8                 Idx1, Idx2;
  UINT8                 CommonPeerSocketCnt;

  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (SocInfo[Idx1].IsSocValid == FALSE) {
      return FALSE;  // Less than 8S cannot make two clumps
    }

    //
    // 8S-AFC/DPW: A socket may have more than 5 actived links
    // 8S-2Clump:  A socket may have 5 actived links at most
    //
    if (SocInfo[Idx1].ActiveLinkCnt > 5) {
      return FALSE;
    }

    //
    // 8S-AFC:    A socket has 0 Dual Link
    // 8S-2Clump: A socket has 1 Dual Link at most
    // 8S-DPW:    A socket has 3 Dual Links at most
    //
    if (SocInfo[Idx1].DualLinkCnt > 1) {  //Must be DPW.
      return FALSE;
    }

    for (Idx2 = Idx1 + 1; Idx2 < MAX_SOCKET; Idx2++) {
      if (SocInfo[Idx2].IsSocValid == FALSE) {
        return FALSE;  // Less than 8S cannot make two clumps
      }

      if ((SocInfo[Idx1].PeerSocList & (1 << Idx2)) == 0) {
        //
        //Socket[Idx1] and Socket[Idx2] are not peers
        //
        continue;
      }

      //
      // Only do the check for any two peer sockets
      //
      CommonPeerSocketCnt = GetCommonPeerSocketCnt (Idx1, Idx2, SocInfo, 0, NULL);

      //
      // 8S-DPW:    Every two peer sockets do not have any common peer socket.
      // 8S-AFC:    Every two peer sockets may have at most 4 common peer sockets.
      // 8S-2Clump: Every two peer sockets may have at most 2 common peer sockets.
      //
      if (CommonPeerSocketCnt > 2) {  //Must be AFC.
        return FALSE;
      }
    }
  }

  return TRUE;
}

/*
  Routine to check the UPI links between two clumps.
  Note: The caller must ensure there are two clumps in the topology.

  @param[in] SocInfo           - A pointer to the array of SOCKET_TOPOLOGY_INFO type
  @param[in] Clumps            - A pointer to the buffer that contains all sockets in two clumps
  @param[out]ClumpsPeerList    - Return a list of Socket IDs(0xFF if no peer socket) in Clump[1] that links to each socket in Clump[0]

  @retval    Topology8S2ClumpsDoubleLinked: 8S-2Clumps topology and 2 clumps are linked by DoubleLink, no need to degrade.
  @retval    Topology8S2ClumpsSingleLinked: 8S-2Clumps topology and 2 clumps are linked by SignleLink, need to remove all Dual Links.
  @retval    Topology8S2ClumpsUnsupported: Need to degrade to one clump with legacy socket in it.
*/
TOPO_8S2CLUMPS_LINK_STS
EFIAPI
CheckLinksBetweenTwoClumps (
  IN  SOCKET_TOPOLOGY_INFO      *SocInfo,
  IN  UINT8                     (*Clumps)[SOCKETS_NUMBER_IN_EACH_CLUMP],
  OUT UINT8                     *ClumpsPeerList
  )
{
  UINT8                       Idx0, Idx1;
  UINT8                       C0SocId, C1SocId;
  UINT8                       PeerSocList;
  UINT8                       PeerSocCnt;
  UINT8                       C0PeerSocList[SOCKETS_NUMBER_IN_EACH_CLUMP];
  BOOLEAN                     C0DualLinkedToC1[SOCKETS_NUMBER_IN_EACH_CLUMP];
  TOPO_8S2CLUMPS_LINK_STS     LinkStatus;

  SetMem ((VOID *) C0PeerSocList, sizeof (C0PeerSocList), 0xFF);

  for (Idx0 = 0; Idx0 < SOCKETS_NUMBER_IN_EACH_CLUMP; Idx0++) {  //Idx0 is an Index of Clumps[0]
    C0SocId = Clumps[0][Idx0];  // Retrieve each socket from Clumps[0]
    KTI_ASSERT (SocInfo[C0SocId].DualLinkCnt <= 1, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);

    if (SocInfo[C0SocId].DualLinkCnt == 0) {
      PeerSocList = SocInfo[C0SocId].PeerSocList;
      C0DualLinkedToC1[Idx0] = FALSE;
    } else {
      PeerSocList = SocInfo[C0SocId].DualLinkPeerSocList;
      C0DualLinkedToC1[Idx0] = TRUE;
    }

    PeerSocCnt = 0;
    for (Idx1 = 0; Idx1 < SOCKETS_NUMBER_IN_EACH_CLUMP; Idx1++) {  //Idx1 is an Index of Clumps[1]
      C1SocId = Clumps[1][Idx1];  // Retrieve each socket from Clumps[1]

      if ((PeerSocList & (1 << C1SocId)) != 0) {
        PeerSocCnt++;
        if (PeerSocCnt > 1) {
          KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "  Fatal Error: Socket[%x] in Clumps[0] links to both Socket[%x] and Socket[%x] in Clumps[1]\n",
                               C0SocId, C0PeerSocList[Idx0], C1SocId));
          KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
        }
        C0PeerSocList[Idx0] = C1SocId;
      }
    }
  }

  LinkStatus = Topology8S2ClumpsDoubleLinked;
  for (Idx0 = 0; Idx0 < SOCKETS_NUMBER_IN_EACH_CLUMP; Idx0++) {
    if (C0PeerSocList[Idx0] == 0xFF) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  Socket[%x] in Clumps[0] does not link to any socket in Clumps[1]\n", Clumps[0][Idx0]));
      LinkStatus = Topology8S2ClumpsUnsupported;
      break;
    }
    if (!C0DualLinkedToC1[Idx0]) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  Socket[%x] in Clumps[0] links to Socket[%x] in Clumps[1] by single link\n", Clumps[0][Idx0], C0PeerSocList[Idx0]));
      LinkStatus = Topology8S2ClumpsSingleLinked;
    }
  }

  if ((LinkStatus == Topology8S2ClumpsDoubleLinked) || (LinkStatus == Topology8S2ClumpsSingleLinked)) {
    for (Idx0 = 0; Idx0 < SOCKETS_NUMBER_IN_EACH_CLUMP - 1; Idx0++) {
      for (Idx1 = Idx0 + 1; Idx1 < SOCKETS_NUMBER_IN_EACH_CLUMP; Idx1++) {
        if (C0PeerSocList[Idx0] == C0PeerSocList[Idx1]) {
          KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "  Fatal Error: Socket[%x] and Socket[%x] in Clumps[0] link to same Socket[%x] in Clumps[1]\n",
                               Clumps[0][Idx0], Clumps[0][Idx1], C0PeerSocList[Idx0]));
          KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
        }
      }
    }
  }

  if (ClumpsPeerList != NULL) {
    CopyMem ((VOID *) ClumpsPeerList, (VOID *) C0PeerSocList, sizeof (C0PeerSocList));
  }

  return LinkStatus;
}

/*
  Routine to handle degrade for 8S2Clump topology. (Clump means 4S Fully Connect)
  Note:
  For CPX program, if there are no two clumps discovered, this routine will do nothing
  and just return FALSE. After that, the AFC/DPW rules will be applied to degrade the
  topology.

  @param[in] SocketData               - Socket specific data structure
  @param[in] KtiInternalGlobal        - KTIRC internal variables.
  @param[in] SocInfo                  - A pointer to the array of SOCKET_TOPOLOGY_INFO type
  @param[in] HasTwoClumps             - TRUE: Has two Clumps; FALSE: Does not have two Clumps

  @retval    TRUE                 - Topology degradation is done.
  @retval    FALSE                - Topology degradation is not done.
*/
BOOLEAN
EFIAPI
HandleDegraded8S2Clumps (
  IN  KTI_SOCKET_DATA           *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  SOCKET_TOPOLOGY_INFO      *SocInfo,
  IN  BOOLEAN                   HasTwoClumps
  )
{
  TOPO_8S2CLUMPS_LINK_STS       LinkStatus;
  UINT8                         ClumpIdx;

  KTIVAR->SysConfig = SYS_CONFIG_8S;

  //
  // Check the DFX setup knob to see if it requests exceptional handling for the system degrade mode.
  //
  if (KTISETUP->DfxParm.DfxSystemDegradeMode == DEGRADE_TOPOLOGY_AS_IS) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  DfxSystemDegradeMode == AS_IS - No need to degrade\n"));

    return TRUE;
  } else if (KTISETUP->DfxParm.DfxSystemDegradeMode == DEGRADE_TOPOLOGY_1S) {
    DegradeTo1S (SocketData, KtiInternalGlobal);

    KTIVAR->SysConfig = SYS_CONFIG_1S;
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  DfxSystemDegradeMode == 1S - Degraded to 1S\n"));

    return TRUE;
  }

  if (!HasTwoClumps) {
    return FALSE;
  }

  LinkStatus = KtiInternalGlobal->Topo8S2ClumpsInfo.LinkStatus;

  if (LinkStatus == Topology8S2ClumpsDoubleLinked) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  Two clumps linked by DualLink - No need to degrade.\n"));

    KtiInternalGlobal->EightSocketTopology = Topo8STwoClumps;
  } else if (LinkStatus == Topology8S2ClumpsSingleLinked) {  // One or more Dual Link is broken
    //
    // Remove all DualLink
    //
    RemoveDualLink (KtiInternalGlobal, SocketData);

    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  Degraded to two clumps which are linked by SingleLink.\n"));

    KtiInternalGlobal->EightSocketTopology = Topo8STwoClumps;
  } else if (LinkStatus == Topology8S2ClumpsUnsupported) {
    //
    // Two clumps are not well connected, have to remove one clump
    //
    ClumpIdx = KtiInternalGlobal->Topo8S2ClumpsInfo.ClumpWithLegacySocket;

    if (ClumpIdx < MAX_CLUMPS_NUMBER_IN_8S_TOPOLOGY) {
      //
      // Legacy socket is part of one clump
      //
      DegradeToSingleClump (SocketData, KtiInternalGlobal, KtiInternalGlobal->Topo8S2ClumpsInfo.Clumps[ClumpIdx]);
    } else {
      KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "  Fatal Error: Invalide Clump Index!!\n"));
      KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
      return FALSE;
    }
  }

  return TRUE;
}

/*
  Routine to handle degrade for 8S Twisted Fully Connect Cubes topology.
  Note:
  1) For SPR program, the topology degradation is a bit special because SPR carries a limitation
     that it does not support route thru traffic on Link#3. The existing degrade algorithm does not
     support handling this limitation, so we need to make sure all degradation work be properly
     handled in this routine and this routine must return TRUE.
  2) If it is GNR program, it has no limitation. This routine can support as well.

  @param[in] SocketData               - Socket specific data structure
  @param[in] KtiInternalGlobal        - KTIRC internal variables.
  @param[in] SocInfo                  - A pointer to the array of SOCKET_TOPOLOGY_INFO type
  @param[in] HasTwoClumps             - TRUE: Has two Clumps; FALSE: Does not have two Clumps

  @retval    TRUE                     - Topology degradation is done.
*/
BOOLEAN
EFIAPI
HandleDegraded8STwistedFullyConnectCubes (
  IN  KTI_SOCKET_DATA           *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  SOCKET_TOPOLOGY_INFO      *SocInfo,
  IN  BOOLEAN                   HasTwoClumps
  )
{
  UINT8                         ClumpIdxWithLegacySocket;

  KTIVAR->SysConfig = SYS_CONFIG_8S;

  //
  // Check the DFX setup knob to see if it requests exceptional handling for the system degrade mode.
  //
  if (KTISETUP->DfxParm.DfxSystemDegradeMode == DEGRADE_TOPOLOGY_AS_IS) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  DfxSystemDegradeMode == AS_IS - No need to degrade\n"));

    return TRUE;
  } else if (KTISETUP->DfxParm.DfxSystemDegradeMode == DEGRADE_TOPOLOGY_1S) {
    DegradeTo1S (SocketData, KtiInternalGlobal);

    KTIVAR->SysConfig = SYS_CONFIG_1S;
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  DfxSystemDegradeMode == 1S - Degraded to 1S\n"));

    return TRUE;
  }

  if (HasTwoClumps) {
    if (KtiInternalGlobal->Topo8S2ClumpsInfo.LinkStatus == Topology8S2ClumpsSingleLinked) {
      if (Is8STwistedFullyConnectCubes (SocketData, KtiInternalGlobal, KtiInternalGlobal->Topo8S2ClumpsInfo.Clumps)) {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  8S Twisted Fully Connect Cubes - No need to degrade.\n"));

        KtiInternalGlobal->EightSocketTopology = Topo8STwistedFullyConnectCubes;
        KtiInternalGlobal->IsRouteThrough = TRUE;
        return TRUE;
      }
    }
  }

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\n  Degraded 8S topology is detected.\n\n"));

  //
  // If legacy socket is part of a clump, then just degrade to a single clump with legacy socket in it
  //
  ClumpIdxWithLegacySocket = KtiInternalGlobal->Topo8S2ClumpsInfo.ClumpWithLegacySocket;
  if (ClumpIdxWithLegacySocket < MAX_CLUMPS_NUMBER_IN_8S_TOPOLOGY) {
    //
    // Legacy socket is part of a clump
    //
    DegradeToSingleClump (SocketData, KtiInternalGlobal, KtiInternalGlobal->Topo8S2ClumpsInfo.Clumps[ClumpIdxWithLegacySocket]);
    return TRUE;
  }

  //
  // Need to further degrade the topology ...
  //
  ProcessSerializedTopologyDegradation (SocketData, KtiInternalGlobal, SocInfo);
  return TRUE;
}

/*
  Routine to check 8S2Clump topology. (Clump means 4S Fully Connect)

  @param[in] SocketData        - Socket specific data structure
  @param[in] KtiInternalGlobal - KTIRC internal variables.

  @retval                      - TRUE:  8S2Clump Topology degradation is done.
                               - FALSE: Need to further check and handle for this topology.
*/
BOOLEAN
EFIAPI
CheckEightSocketTwoClumpTopology (
  IN  KTI_SOCKET_DATA           *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
  BOOLEAN                     Is8S2ClumpPossible;
  BOOLEAN                     ClumpDiscovered;
  BOOLEAN                     DegradeDone;
  UINT8                       Idx1;
  UINT8                       ClumpsCount;
  UINT8                       ClumpWithLegacySocket;
  UINT8                       Clumps[MAX_CLUMPS_NUMBER_IN_8S_TOPOLOGY][SOCKETS_NUMBER_IN_EACH_CLUMP];
  SOCKET_TOPOLOGY_INFO        SocInfo[MAX_SOCKET];

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n%a () Enter\n", __FUNCTION__));

  DegradeDone = FALSE;

  CollectTopologyInfo (SocketData, KtiInternalGlobal, SocInfo);

  SetMem ((VOID *) KtiInternalGlobal->Topo8S2ClumpsInfo.Clumps, sizeof (Clumps), 0xFF);
  SetMem ((VOID *) KtiInternalGlobal->Topo8S2ClumpsInfo.ClumpsPeerList, SOCKETS_NUMBER_IN_EACH_CLUMP, 0xFF);
  KtiInternalGlobal->Topo8S2ClumpsInfo.ClumpWithLegacySocket = 0xFF;
  KtiInternalGlobal->Topo8S2ClumpsInfo.LinkStatus = Topology8S2ClumpsUndetermined;

  //
  // Roughly check the possibility of 8S 2Clumps topology
  //
  Is8S2ClumpPossible = IsEightSocketTwoClumpPossible (SocInfo);
  if (Is8S2ClumpPossible) {
    //
    // Passed the rough checking, then try to discover clumps
    //
    ClumpsCount = 0;
    ClumpWithLegacySocket = 0xFF;

    SetMem ((VOID *) &Clumps[0][0], sizeof (Clumps), 0xFF);

    for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
      if (SocInfo[Idx1].IsSocValid == FALSE) {
        continue;
      }

      if (ClumpsCount < MAX_CLUMPS_NUMBER_IN_8S_TOPOLOGY) {
        if (ClumpsCount == 1) {
          //
          // There are two clumps at most in the topology, and two clumps do not have any common socket.
          // Skip the sockets already discovered in Clumps[0] before discovering the second clump.
          //
          if ((Idx1 == Clumps[0][0]) || (Idx1 == Clumps[0][1]) ||
              (Idx1 == Clumps[0][2]) || (Idx1 == Clumps[0][3])) {
            continue;
          }
        }

        ClumpDiscovered = DiscoverClump (Idx1, SocInfo, Clumps[0], SOCKETS_NUMBER_IN_EACH_CLUMP, Clumps[ClumpsCount]);
        if (ClumpDiscovered) {
          if ((KtiInternalGlobal->SbspSoc == Clumps[ClumpsCount][0]) ||
              (KtiInternalGlobal->SbspSoc == Clumps[ClumpsCount][1]) ||
              (KtiInternalGlobal->SbspSoc == Clumps[ClumpsCount][2]) ||
              (KtiInternalGlobal->SbspSoc == Clumps[ClumpsCount][3])) {
            ClumpWithLegacySocket = ClumpsCount;
          }
          ClumpsCount++;
        }
      }
    }

    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  Discovered total of %x clumps\n", ClumpsCount));
    if (ClumpWithLegacySocket < MAX_CLUMPS_NUMBER_IN_8S_TOPOLOGY) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  Legacy socket in clump#%x\n", ClumpWithLegacySocket));
    }

    CopyMem ((VOID *) KtiInternalGlobal->Topo8S2ClumpsInfo.Clumps, (VOID *) Clumps, sizeof (Clumps));
    KtiInternalGlobal->Topo8S2ClumpsInfo.ClumpWithLegacySocket = ClumpWithLegacySocket;

    if ((ClumpsCount < MAX_CLUMPS_NUMBER_IN_8S_TOPOLOGY) || (ClumpWithLegacySocket >= MAX_CLUMPS_NUMBER_IN_8S_TOPOLOGY)) {
      //
      // Not discover two valid clumps, so 8S 2Clumps topology is impossible.
      //
      Is8S2ClumpPossible = FALSE;
    }
  }

  if (Is8S2ClumpPossible) {
    //
    // The 8S 2Clumps topology is still possible, that is two clumps are already discovered.
    // Further check the links between two clumps.
    //
    KtiInternalGlobal->Topo8S2ClumpsInfo.LinkStatus = CheckLinksBetweenTwoClumps (SocInfo,
                                                                                  Clumps,
                                                                                  KtiInternalGlobal->Topo8S2ClumpsInfo.ClumpsPeerList
                                                                                 );
  }

  if (KtiInternalGlobal->CpuType == CPU_CPX) {
    DegradeDone = HandleDegraded8S2Clumps (SocketData, KtiInternalGlobal, SocInfo, Is8S2ClumpPossible);
  }

  if (IsSprFamilyCpu (KtiInternalGlobal->CpuType) || IsGnrSrfFamilyCpu (KtiInternalGlobal->CpuType)) {
    DegradeDone = HandleDegraded8STwistedFullyConnectCubes (SocketData, KtiInternalGlobal, SocInfo, Is8S2ClumpPossible);
  }

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a () Exit\n\n", __FUNCTION__));
  return DegradeDone;
}
