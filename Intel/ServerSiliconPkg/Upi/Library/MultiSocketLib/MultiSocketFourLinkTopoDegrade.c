/** @file

  Topology degrade implementation for sockets that have a maximum of 3 or 4 UPI links.

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

#include <KtiSetupDefinitions.h>
#include <Library/UncoreLib.h>
#include <KtiMisc.h>
#include <Library/KtiApi.h>
#include <Library/MultiSocketLib.h>
#include <Library/CpuAndRevisionLib.h>

/**
  Identifies what is the "best" topology that can be derived out of system with degraded topology.

  If each socket has connection to at least 2 different other sockets, 4S Ring topology is possible.
  If a fully connected 3S with legacy socket is possible then 3S Ring topology is possible.
  If legacy socket is connected to 2 different other sockets then 3S Chain topology is possible.
  If legacy socket is connected to 1 other socket then 2S Single link topology is possible.
  Otherwise it is a 1S topology.

  This function should be called only when the system has degraded 4S topology.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param DiscEngData       - Pointer to discovery engine data

  @retval KTI_SUCCESS

**/
KTI_STATUS
HandleDegraded4S (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_DISC_DATA            *DiscEngData
  )
{
  UINT8   Idx1, Idx2, Idx3;
  UINT8   Soc1, Soc2;
  UINT8   Temp1, Temp2;
  UINT8   PeerSocList[MAX_SOCKET];
  UINT8   PeerSocCnt[MAX_SOCKET];
  UINT8   Ring3SList;
  UINT8   PeerSocId, PeerPort = 0;
  UINT8   RingNumber, RingNode, RingNodeList = 0;
  BOOLEAN Is4SRing, Is3SRing, Is3SChain, Is2S, Is1S;
  BOOLEAN IsDegradedFrom4S;

  //
  // if DiscEngData is NULL, this function is called from the 4S degradation path
  //
  if (DiscEngData == NULL) {
    //
    // Degrade 4S config to 4S-Ring or less
    //
    IsDegradedFrom4S = TRUE;
  } else {
    //
    // Degrade > 4S config to 4S-Ring or less
    //
    IsDegradedFrom4S = FALSE;
  }

  if (KTISETUP->DfxParm.DfxSystemDegradeMode == DEGRADE_TOPOLOGY_AS_IS) {
    KTIVAR->SysConfig = SYS_CONFIG_4S;
    return KTI_SUCCESS;
  } else if (KTISETUP->DfxParm.DfxSystemDegradeMode == DEGRADE_TOPOLOGY_1S) {
    DegradeTo1S (SocketData, KtiInternalGlobal);
    KTIVAR->SysConfig = SYS_CONFIG_1S;
    return KTI_SUCCESS;
  }

  Is4SRing = Is3SRing = Is3SChain = Is2S = Is1S = FALSE;
  Ring3SList = 0;

  //
  // Identify the peer socket list & count for each populated socket.
  //
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    PeerSocList[Idx1] = 0;
    PeerSocCnt[Idx1] = 0;
    if ((SocketData->Cpu[Idx1].Valid == TRUE) && (SocketData->Cpu[Idx1].SocType == SOCKET_TYPE_CPU)) {
      for (Idx2 = 0; Idx2 < KtiInternalGlobal->KtiPortCnt; Idx2++) {
        if ((SocketData->Cpu[Idx1].LinkData[Idx2].Valid == TRUE) && (SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocType == SOCKET_TYPE_CPU)) {
          if ((PeerSocList[Idx1] & (1 << SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocId)) == 0) {
            PeerSocList[Idx1] = PeerSocList[Idx1] |(UINT8)(1 << SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocId);
            PeerSocCnt[Idx1]++;
          }
        }
      }
    }
  }

  //
  // If legacy socket is not connected to any other sockets then it is 1S;  Else if it is conneted to one another socket, it is 2S single link.
  //
  if (PeerSocCnt[KtiInternalGlobal->SbspSoc] == 0) {
    Is1S = TRUE;
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Reducing it to 1S"));
  } else if (PeerSocCnt[KtiInternalGlobal->SbspSoc] == 1) {
    Is2S = TRUE;
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Reducing it to 2S Single Link "));
  } else { // 4S or larger degradation
    //
    // Determine if a 4S-Ring is possible
    //
    if (IsDegradedFrom4S) {
      //
      // If each socket has 2 or more peers (i.e links) then it can be reduced to a 4S ring
      //
      Is4SRing = TRUE;
      for (Idx1 = 0; Idx1 < 4; Idx1++) {
        if (PeerSocCnt[Idx1] < 2) {
          Is4SRing = FALSE;
          break;
        }
      } // for each Socket
    } else { // Larger than 4S degradation to 4S-Ring or less
      //
      // If there is a ring with legacy socket then it can be reduced to 4S ring
      //
      Is4SRing = ((DiscEngData->RingInfo[KtiInternalGlobal->SbspSoc][0] == 1) ?  TRUE : FALSE);
    } // Larger than a 4S degradation

    //
    // Determine the degradation landing-zone
    //
    if (Is4SRing) {
      KtiInternalGlobal->Is4SRing = TRUE;
      KtiInternalGlobal->IsRouteThrough = TRUE;
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Reducing it to 4S Ring "));
    } else {
      //
      // It must be either 3S Ring or Chain. If there is a triangle involving the legacy socket then it is 3S Ring.
      // Otherwise it is 3S Chain.
      //

      //
      // Detect if there is any "triangle" pattern involving the legacy socket in the topology. To detect it,
      // first we need to check if legacy socket is connected to at least 2 immediate neighbor sockets.
      // And at least 2 of those immediate neighbors have a connection between them.
      //
      for (Idx2 = 0; Idx2 < MAX_SOCKET; Idx2++) {
        if (PeerSocList[KtiInternalGlobal->SbspSoc] & (1 << Idx2)) {
          for (Idx3 = 0; Idx3 < MAX_SOCKET; Idx3++) {
            if ((PeerSocList[Idx2] & (1 << Idx3)) && (PeerSocList[KtiInternalGlobal->SbspSoc] & (1 << Idx3))) {
              //
              // If Idx2's neighbor socket Idx3 is also a neighbor of KtiInternalGlobal->SbspSoc
              // then KtiInternalGlobal->SbspSoc, Idx2 and Idx3 form a triangle.
              //
              Is3SRing = TRUE;
              Ring3SList = (1 << KtiInternalGlobal->SbspSoc)  | (1 << Idx2) | (1 << Idx3);
              break;
            }
          }
          if (Is3SRing == TRUE) {
            break;
          }
        }
      }

      if (Is3SRing == TRUE) {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Reducing it to 3S Ring (socket list: 0x%02X)", Ring3SList));
      } else {
        Is3SChain = TRUE;
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Reducing it to 3S Chain "));
      }
    } // Degradation landing-zone
  } // 4S or larger degradation

  //
  // Ensure only one Reduced Topology is selected.
  //
  Temp1 = (Is4SRing << 4) | (Is3SRing << 3) | (Is3SChain << 2) | (Is2S << 1) | Is1S;
  Temp2 = 0;
  for (Idx1 = 0; Idx1 < 5; Idx1++) {
    if (Temp1 & (1 << Idx1)) {
      Temp2++;
    }
  }
  if (Temp2 != 1) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n  Reducing to supported topology failed"));
    KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
  }

  //
  // Carry out the topology reduction by marking the appropriate socket(s) and/or link(s) as invalid
  //
  if (Is1S == TRUE) {
    //
    // Mark all sockets except legacy socket as invalid; also mark all the links as invalid
    //
    KTIVAR->SysConfig = SYS_CONFIG_1S;
    for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
      if (Idx1 != KtiInternalGlobal->SbspSoc) {
        SocketData->Cpu[Idx1].Valid = FALSE;
      }
      for (Idx2 = 0; Idx2 < KtiInternalGlobal->KtiPortCnt; Idx2++) {
        SocketData->Cpu[Idx1].LinkData[Idx2].Valid = FALSE;
      }
    }
  } else if (Is2S == TRUE) {
    //
    // Retain only the two sockets and the link between them
    //

    // Identify the peer socket and port
    PeerSocId = 0xFF;
    for (Idx2 = 0; Idx2 < KtiInternalGlobal->KtiPortCnt; Idx2++) {
      if ((SocketData->Cpu[KtiInternalGlobal->SbspSoc].LinkData[Idx2].Valid == TRUE) &&
          (SocketData->Cpu[KtiInternalGlobal->SbspSoc].LinkData[Idx2].PeerSocType == SOCKET_TYPE_CPU)) {
        PeerSocId = (UINT8) SocketData->Cpu[KtiInternalGlobal->SbspSoc].LinkData[Idx2].PeerSocId;
        PeerPort   = (UINT8) SocketData->Cpu[KtiInternalGlobal->SbspSoc].LinkData[Idx2].PeerPort;
        break;
      }
    }

    KTI_ASSERT ((PeerSocId != 0xFF), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);

    // Mark all other sockets and links as invalid
    for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
      if ((Idx1 != KtiInternalGlobal->SbspSoc) && (Idx1 != PeerSocId)) {
        SocketData->Cpu[Idx1].Valid = FALSE;
      }
      for (Idx3 = 0; Idx3 < KtiInternalGlobal->KtiPortCnt; Idx3++) {
        if ((Idx1 == KtiInternalGlobal->SbspSoc) && (Idx3 == Idx2)) {
          continue;
        } else if ((Idx1 == PeerSocId) && (Idx3 == PeerPort)) {
          continue;
        }
        SocketData->Cpu[Idx1].LinkData[Idx3].Valid = FALSE;
      }
    } // for each socket

    //
    // If 2S1L, and adjacent SID is 1, then treat as 2S1L, else treat as 4S or 8S
    //
    KTIVAR->SysConfig = ((PeerSocId <= 1) ? SYS_CONFIG_2S1L : (PeerSocId <= 3) ? SYS_CONFIG_4S : SYS_CONFIG_8S );


  } else if (Is3SChain == TRUE) {
    //
    // If the third link of the legacy socket is valid, mark it as invalid. For the remaining 2 peers
    // of the legacy socket make sure all their links except the one that connects them to
    // the legacy socket are marked as invalid.
    //
    KtiInternalGlobal->IsRouteThrough = TRUE;

    if (PeerSocCnt[KtiInternalGlobal->SbspSoc] == 3) {
      // Take out the socket that is connected to the 3rd link of the legacy socket
      PeerSocId = (UINT8) SocketData->Cpu[KtiInternalGlobal->SbspSoc].LinkData[2].PeerSocId;
      if (PeerSocId >= MAX_SOCKET) {
        KTI_ASSERT ((PeerSocId < MAX_SOCKET), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_CPU_SOCKET_ID);
        return KTI_FAILURE;
      }
      SocketData->Cpu[KtiInternalGlobal->SbspSoc].LinkData[2].Valid = FALSE;
      SocketData->Cpu[PeerSocId].Valid = FALSE;
      PeerSocList[KtiInternalGlobal->SbspSoc] = PeerSocList[KtiInternalGlobal->SbspSoc] & (UINT8) (~(1 << PeerSocId));
      PeerSocCnt[KtiInternalGlobal->SbspSoc] = 2;
      for (Idx2 = 0; Idx2 < KtiInternalGlobal->KtiPortCnt; Idx2++) {
        SocketData->Cpu[PeerSocId].LinkData[Idx2].Valid = FALSE;
      }
    }

    KTIVAR->SysConfig = ((PeerSocList[KtiInternalGlobal->SbspSoc] <= 0x0F) ? SYS_CONFIG_4S : SYS_CONFIG_8S);

    //
    // Mark all the links that are not part of the chain as invalid
    // (Legacy socket's link are taken care above)
    //
    for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
      if ((Idx1 != KtiInternalGlobal->SbspSoc) && (SocketData->Cpu[Idx1].Valid == TRUE) &&
          (SocketData->Cpu[Idx1].SocType == SOCKET_TYPE_CPU)) {
        if (!(PeerSocList[KtiInternalGlobal->SbspSoc] & (1 << Idx1))) { // Remove the socket is not part of  the chain
          SocketData->Cpu[Idx1].Valid = FALSE;
        }
        for (Idx2 = 0; Idx2 < KtiInternalGlobal->KtiPortCnt; Idx2++) {
          if ((SocketData->Cpu[Idx1].LinkData[Idx2].Valid == TRUE) &&
              (SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocType == SOCKET_TYPE_CPU) &&
              ((UINT8) SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocId != KtiInternalGlobal->SbspSoc)) {
            SocketData->Cpu[Idx1].LinkData[Idx2].Valid = FALSE;
          }
        }
      }
    }
  } else if (Is3SRing == TRUE) {
    //
    // Retain only the links that are part of the 3S Ring.
    //
    for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
      if (Ring3SList & (1 << Idx1)) { // If this socket is part of the 3S ring
        for (Idx2 = 0; Idx2 < KtiInternalGlobal->KtiPortCnt; Idx2++) {
          if ((SocketData->Cpu[Idx1].LinkData[Idx2].Valid == TRUE) &&
              (SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocType == SOCKET_TYPE_CPU)) {
            //
            // Check if this link's peer socket is part of the 3S ring and mark it as invalid
            //
            PeerSocId = (UINT8) SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocId;

            if (!(Ring3SList & (1 << PeerSocId))) {
              PeerPort  = (UINT8) SocketData->Cpu[Idx1].LinkData[Idx2].PeerPort;
              if (PeerPort >= (KtiInternalGlobal->KtiPortCnt)) {
                KTI_ASSERT ((PeerPort < (KtiInternalGlobal->KtiPortCnt)), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_KTI_PORT_ID);
                return KTI_FAILURE;
              }

              SocketData->Cpu[Idx1].LinkData[Idx2].Valid = FALSE;
              SocketData->Cpu[PeerSocId].LinkData[PeerPort].Valid = FALSE;
            }
          }
        }
      } else {
        //
        // Mark the socket that is not part of the 3S Ring and all its links as invalid
        //
        SocketData->Cpu[Idx1].Valid = FALSE;
        for (Idx2 = 0; Idx2 < KtiInternalGlobal->KtiPortCnt; Idx2++) {
          SocketData->Cpu[Idx1].LinkData[Idx2].Valid = FALSE;
        }
      }
    } // for each socket in 3S Ring

    KTIVAR->SysConfig = ((Ring3SList <= 0x0F) ? SYS_CONFIG_4S : SYS_CONFIG_8S);

  } else {
    //
    // 4S or greater degradation
    //
    if (IsDegradedFrom4S) {  // 4S degradation
      //
      // If there is any socket that has more than 2 links, take one of the link out.
      // Note that since each socket is assured of at least two links at this point,
      // if a socket has 3 links there must be other socket that has 3 links as well.
      // By taking link between those 2 sockets 4S Ring can be formed.
      //
      KTIVAR->SysConfig = SYS_CONFIG_4S;
      Soc1 = Soc2 = 0xFF;
      for (Idx1 = 0; Idx1 < 4; Idx1++) {
        if (PeerSocCnt[Idx1] == 3) {
          Soc1 = Idx1;
          for (Idx2 = Idx1 + 1; Idx2 < 4; Idx2++) {
            if (PeerSocCnt[Idx2] == 3) {
              Soc2 = Idx2;
              break;
            }
          }
          break;
        }
      }

      // Mark the link between the sockets as invalid
      if (Soc1 != 0xFF) {
        KTI_ASSERT ((Soc2 != 0xFF), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_73);
        for (Idx2 = 0; Idx2 < KtiInternalGlobal->KtiPortCnt; Idx2++) {
          if ((SocketData->Cpu[Soc1].LinkData[Idx2].Valid == TRUE) &&
              (SocketData->Cpu[Soc1].LinkData[Idx2].PeerSocType == SOCKET_TYPE_CPU) &&
              (SocketData->Cpu[Soc1].LinkData[Idx2].PeerSocId == (UINT32) Soc2)) {
            PeerPort = (UINT8) SocketData->Cpu[Soc1].LinkData[Idx2].PeerPort;
            if (PeerPort >= (KtiInternalGlobal->KtiPortCnt)) {
              KTI_ASSERT ((PeerPort < (KtiInternalGlobal->KtiPortCnt)), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_KTI_PORT_ID);
              return KTI_FAILURE;
            }

            SocketData->Cpu[Soc1].LinkData[Idx2].Valid = FALSE;
            SocketData->Cpu[Soc2].LinkData[PeerPort].Valid = FALSE;
            break;
          }
        }
      }

      // Make sure it is a 4S ring
      for (Idx1 = 0; Idx1 < 4; Idx1++) {
        Soc1 = Soc2 = 0xFF;
        for (Idx2 = 0; Idx2 < KtiInternalGlobal->KtiPortCnt; Idx2++) {
          if ((SocketData->Cpu[Idx1].LinkData[Idx2].Valid == TRUE) &&
              (SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocType == SOCKET_TYPE_CPU)) {
            if (Soc1 == 0xFF) {
              Soc1 = (UINT8) SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocId;
            } else if (Soc2 == 0xFF) {
              Soc2 = (UINT8) SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocId;
            } else {
              KTI_ASSERT (FALSE, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_74);
            }
          }
        }
        if ((Soc1 == 0xFF) || (Soc2 == 0xFF)) {
          KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n  Reducing the Topology to 4S Ring failed!!!"));
          KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
        }
      } // for each socket
    } else {
      //
      // Greater than 4S degradation
      //
      RingNumber = DiscEngData->RingInfo[KtiInternalGlobal->SbspSoc][1];
      for (Idx1 = 0; Idx1 < CPUS_PER_RING; Idx1++) {
        RingNode = DiscEngData->Rings[RingNumber][Idx1];
        RingNodeList |= 1 << RingNode;
      }

      for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
        if (!(RingNodeList & (1 << Idx1))) {
          //
          // Remove sockets not part of the ring
          //
          SocketData->Cpu[Idx1].Valid = FALSE;
          for (Idx2 = 0; Idx2 < (KtiInternalGlobal->KtiPortCnt); Idx2++) {
            SocketData->Cpu[Idx1].LinkData[Idx2].Valid = FALSE;
          }
        } else {
          //
          // Socket is part of the ring
          //
          for (Idx2 = 0; Idx2 < (KtiInternalGlobal->KtiPortCnt); Idx2++) {
            PeerSocId = (UINT8) SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocId;
            if (!(RingNodeList & (1 << PeerSocId))) {
              //
              // Peer socket not part of the ring
              // Remove the link
              //
              SocketData->Cpu[Idx1].LinkData[Idx2].Valid = FALSE;
            }
          }
        }
      } // for each socket

      KTIVAR->SysConfig = ((RingNodeList <= 0x0F) ?  SYS_CONFIG_4S : SYS_CONFIG_8S);

    } // 4S vs > 4S degradation
  } // 4S or greater degradation

  return KTI_SUCCESS;
} // HandleDegraded4S

/**
  Check if the system has degraded 4S topology. If yes, then force certain links and/or
  sockets out to make it a supported topology.

  The following topologies are supported (in order of preference):

      4S Ring Topology
      3S Ring Topology
      3S Chain Topology
      2S Single Link Topology
      1S Topology

   This routine should be called only when sockets with ID > 3 are invalid for the system.
   (So, in case of 8S system, this should be called only after sockets 4 to 7 has been
   marked as invalid already)

   Note that, in case of certain socket(s) forced out because it is not part of the topology,
   we mark the socket as invalid in SocketData structure. This will ensure that the rest of
   code that runs after this routine will be agnostic of the sockets being removed and it
   will not assign resources for the sockets that will not be part of the system after reset.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval 0 - Successful

**/
KTI_STATUS
CheckForDegraded4S (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8                      Idx1, Idx2;
  BOOLEAN                    Degraded;
  UINT8                      ActiveLink;
  UINT8                      DualLink;
  BOOLEAN                    IsSpr;
  SOCKET_TOPOLOGY_INFO       SocInfo[MAX_SOCKET];

  //
  // Check if it is 4S topology if not, it is a degraded 4S topology.
  //
  Degraded = FALSE;
  IsSpr = FALSE;

  if (IsSprFamilyCpu (KtiInternalGlobal->CpuType)) {
    IsSpr = TRUE;
  }

  for (Idx1 = 0; Idx1 < 4; Idx1++) {
    ActiveLink = 0;
    DualLink = 0;
    if ((SocketData->Cpu[Idx1].Valid == TRUE) && (SocketData->Cpu[Idx1].SocType == SOCKET_TYPE_CPU)) {
      for (Idx2 = 0; Idx2 < KtiInternalGlobal->KtiPortCnt; Idx2++) {
        if (SocketData->Cpu[Idx1].LinkData[Idx2].Valid == TRUE) {
          ActiveLink++;
          if (SocketData->Cpu[Idx1].LinkData[Idx2].DualLink == TRUE) {
            DualLink++;
          }
        } else {
          Degraded = TRUE;
        }
      }
      if (Degraded) {
        break;
      }
    }
  }

  if (!Degraded && !IsSpr) {
    KTIVAR->SysConfig = SYS_CONFIG_4S;
    if ((GetKtiPortCnt () == 2) || (DualLink == 2)) {
      KtiInternalGlobal->Is4SRing = TRUE;
      KtiInternalGlobal->IsRouteThrough = TRUE;
    }
    return KTI_SUCCESS;
  }

  //
  // It is a degraded topology; reduce it to one of the supported topology.
  //
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n\n  Degraded 4S topology is detected.\n\n"));

  if (IsSpr) {
    CollectTopologyInfo (SocketData, KtiInternalGlobal, SocInfo);
    ProcessSerializedTopologyDegradation (SocketData, KtiInternalGlobal, SocInfo);
  } else {
    HandleDegraded4S (SocketData, KtiInternalGlobal, NULL);
  }

  return KTI_SUCCESS;
}
