/** @file

  This file includes some common functions to support 8S/4S topology degrade.

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
#include <Library/BaseMemoryLib.h>
#include <Library/KtiApi.h>
#include <Library/UncoreLib.h>
#include <Library/MultiSocketLib.h>

CHAR8 ERROR_MSG_TOO_MANY_PEERS[] = "\n%a (): ERROR! Too many peer sockets returned!\n";
CHAR8 SPR_UCC_SPREAD_TOPOLOGY[] = "  SPR-UCC: 2S-%a Spread\n";
CHAR8 SPR_UCC_SHADOW_TOPOLOGY[] = "  SPR-UCC: 2S-%a Shadow\n";

/**
  This routine will collect the topology information for all sockets.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocInfo           - The pointer to the array of SOCKET_TOPOLOGY_INFO type

  @retval                  - VOID.

**/
VOID
EFIAPI
CollectTopologyInfo (
  IN     KTI_SOCKET_DATA             *SocketData,
  IN     KTI_HOST_INTERNAL_GLOBAL    *KtiInternalGlobal,
  IN OUT SOCKET_TOPOLOGY_INFO        *SocInfo
  )
{
  UINT8   Idx1, Idx2;

  ZeroMem ((VOID *) SocInfo, sizeof (SOCKET_TOPOLOGY_INFO) * MAX_SOCKET);

  //
  // Identify the peer socket list & count for each populated socket.
  //
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if ((SocketData->Cpu[Idx1].Valid == TRUE) && (SocketData->Cpu[Idx1].SocType == SOCKET_TYPE_CPU)) {
      SocInfo[Idx1].IsSocValid = TRUE;
      for (Idx2 = 0; Idx2 < KtiInternalGlobal->KtiPortCnt; Idx2++) {
        if ((SocketData->Cpu[Idx1].LinkData[Idx2].Valid == TRUE) && (SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocType == SOCKET_TYPE_CPU)) {
          SocInfo[Idx1].ActiveLinkCnt++;
          if ((SocInfo[Idx1].PeerSocList & (1 << SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocId)) == 0) {
            SocInfo[Idx1].PeerSocList |= (UINT8)(1 << SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocId);
            SocInfo[Idx1].PeerSocCnt++;
          }
          if (SocketData->Cpu[Idx1].LinkData[Idx2].DualLink == FALSE) {
            continue;
          }
          if ((SocInfo[Idx1].DualLinkPeerSocList & (1 << SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocId)) == 0) {
            SocInfo[Idx1].DualLinkPeerSocList |= (UINT8)(1 << SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocId);
            SocInfo[Idx1].DualLinkCnt++;
          }
        }
      }
    }
  }

  return;
}

/**
  This routine will remove a specified socket from the system.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SpecifiedSoc      - The specified socket to be removed

  @retval KTI_SUCCESS      - Successful

**/
KTI_STATUS
EFIAPI
RemoveOneSpecifiedSocket (
  IN  KTI_SOCKET_DATA           *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  UINT8                     SpecifiedSoc
  )
{
  UINT8            Idx1, Idx2;
  KTI_LINK_DATA    *LepPtr;

  //
  // Disable all links connected to Socket[SpecifiedSoc]
  //
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (Idx1 == SpecifiedSoc) {
      continue;
    }
    if (SocketData->Cpu[Idx1].Valid == FALSE) {
      continue;
    }
    for (Idx2 = 0; Idx2 < KtiInternalGlobal->KtiPortCnt; Idx2++) {
      LepPtr = &SocketData->Cpu[Idx1].LinkData[Idx2];
      if ((LepPtr->Valid == TRUE) && (LepPtr->PeerSocId == SpecifiedSoc)) {
        ZeroMem ((VOID *) LepPtr, sizeof (KTI_LINK_DATA));
      }
    }
  }

  //
  // Disable Socket[SpecifiedSoc]
  //
  ZeroMem ((VOID *) SocketData->Cpu[SpecifiedSoc].LinkData, sizeof (SocketData->Cpu[SpecifiedSoc].LinkData));
  SocketData->Cpu[SpecifiedSoc].Valid = FALSE;

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "Socket[%X] is removed\n", SpecifiedSoc));

  return KTI_SUCCESS;
}

/**
  This routine will find and remove all sockets that are not in the RetainedSocList from the system.
  Note:
  The legacy socket will always be retained no matter it is in the RetainedSocList.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param RetainedSocList   - A pointer to the list of socket IDs that need be retained
  @param RetainedSocCnt    - The number of socket IDs in the list

  @retval KTI_SUCCESS      - Successful

**/
KTI_STATUS
EFIAPI
RemoveUnusedSockets (
  IN  KTI_SOCKET_DATA           *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  UINT8                     *RetainedSocList,
  IN  UINT8                     RetainedSocCnt
  )
{
  UINT8    Idx1, Idx2;

  //
  // Check each socket ID in RetainedSocList, which should be less than MAX_SOCKET
  //
  KTI_ASSERT (RetainedSocList != NULL, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_CPU_SOCKET_ID);
  KTI_ASSERT (RetainedSocCnt <= MAX_SOCKET, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_CPU_SOCKET_ID);
  for (Idx1 = 0; Idx1 < RetainedSocCnt; Idx1++) {
    if (RetainedSocList[Idx1] >= MAX_SOCKET) {
      KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n%a() - RetainedSocList[%X] = %X is invalid!\n", __FUNCTION__, Idx1, RetainedSocList[Idx1]));
      KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_CPU_SOCKET_ID);
    }
  }

  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (SocketData->Cpu[Idx1].Valid == TRUE) {
      if (Idx1 == KtiInternalGlobal->SbspSoc) {  //Always retain the legacy socket
        continue;  //Skip the legacy socket
      }

      for (Idx2 = 0; Idx2 < RetainedSocCnt; Idx2++) {
        if (Idx1 == RetainedSocList[Idx2]) {  //The Socket[Idx1] is in the RetainedSocList
          break;
        }
      }
      if (Idx2 < RetainedSocCnt) {
        continue;  //Skip the Socket[Idx1]
      }

      RemoveOneSpecifiedSocket (SocketData, KtiInternalGlobal, Idx1);
    }
  }

  return KTI_SUCCESS;
}

/**
  This routine will disconnet two sockets by disabling the UPI links that connecting them.

  @param[in] SocketData        - Pointer to socket specific data
  @param[in] KtiInternalGlobal - KTIRC Internal Global data
  @param[in] SocId1            - The first socket to be examined
  @param[in] SocId2            - The second socket to be examined

  @retval KTI_SUCCESS          - Successful
  @retvel KTI_FAILURE          - Failed

**/
KTI_STATUS
EFIAPI
DisconnectTwoSockets (
  IN  KTI_SOCKET_DATA           *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  UINT8                     SocId1,
  IN  UINT8                     SocId2
  )
{
  UINT8                         Idx1;
  UINT8                         TempPeerPort;

  if ((SocId1 >= MAX_SOCKET) || (SocId2 >= MAX_SOCKET)) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n%a() - SocId1(%x) or SocId2(%x) is invalid!\n", __FUNCTION__, SocId1, SocId2));
    KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_CPU_SOCKET_ID);

    return KTI_FAILURE;
  }

  for (Idx1 = 0; Idx1 < KtiInternalGlobal->KtiPortCnt; Idx1++) {
    if (SocketData->Cpu[SocId1].LinkData[Idx1].Valid == FALSE) {
      continue;
    }

    if (SocketData->Cpu[SocId1].LinkData[Idx1].PeerSocId == SocId2) {
      //
      // SocId1 and SocId2 are peers, disable links between them
      //
      TempPeerPort = (UINT8) SocketData->Cpu[SocId1].LinkData[Idx1].PeerPort;
      ZeroMem ((VOID *) &SocketData->Cpu[SocId1].LinkData[Idx1], sizeof (KTI_LINK_DATA));
      ZeroMem ((VOID *) &SocketData->Cpu[SocId2].LinkData[TempPeerPort], sizeof (KTI_LINK_DATA));
    }
  }

  return KTI_SUCCESS;
}

/*
  Locate peer sockets for the target socket.
  Given the target socket has N peers:
  1) If (N > *BufferSize), this routine will just return the first discovered *BufferSize peers;
  2) If (N <= *BufferSize), all peers will be put in the first N bytes of PeerSocBuf, the remaining bytes are 0xFF.

  @param[in]     SocInfo               - A pointer to the array of SOCKET_TOPOLOGY_INFO type
  @param[in]     SocketId              - The target socket for which to locate peers
  @param[in]     DualLink              - TRUE:  Locate peers(Must be connected by dual link)
                                         FALSE: Locate peers(Regardless it is connected by dual or single link)
  @param[in out] BufferSize            - Input:  It is the size of PeerSocBuf
                                         Output: It is the real number of peers returned in the PeerSocBuf
  @param[out]    PeerSocBuf            - A pointer to the buffer to hold the peer sockets

  @retval        TRUE                  - Return successfully
  @retval        FALSE                 - Failed
*/
BOOLEAN
EFIAPI
LocatePeerSockets (
  IN      SOCKET_TOPOLOGY_INFO         *SocInfo,
  IN      UINT8                        SocketId,
  IN      BOOLEAN                      DualLink,
  IN OUT  UINT8                        *BufferSize,
  OUT     UINT8                        *PeerSocBuf
  )
{
  UINT8                                Idx1;
  UINT8                                PeerCnt;
  UINT8                                PeerListBitmap;

  if ((BufferSize == NULL) || (PeerSocBuf == NULL)) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n%a() - Error: Input pointer is NULL\n", __FUNCTION__));
    return FALSE;
  }

  if (DualLink) {
    PeerListBitmap = SocInfo[SocketId].DualLinkPeerSocList;
  } else {
    PeerListBitmap = SocInfo[SocketId].PeerSocList;
  }

  //
  // Set 0xFF to the PeerSocBuf
  //
  SetMem ((VOID *) PeerSocBuf, (UINTN) *BufferSize, 0xFF);

  PeerCnt = 0;
  for (Idx1 = 0; (Idx1 < MAX_SOCKET) && (PeerCnt < *BufferSize); Idx1++) {
    if (Idx1 == SocketId) {
      continue;
    }

    if (!SocInfo[Idx1].IsSocValid) {
      continue;
    }

    if ((PeerListBitmap & (1 << Idx1)) != 0) {
      PeerSocBuf[PeerCnt] = Idx1;
      PeerCnt++;
    }
  }

  *BufferSize = PeerCnt;
  return TRUE;
}

/*
  This routine will disable some tagged UPI ports for the target socket, also disable the
  corresponding peer ports.

  @param[in]     SocketData            - Socket specific data structure
  @param[in]     KtiInternalGlobal     - KTIRC internal variables.
  @param[in]     TargetSocket          - Target socket ID
  @param[in]     DisPortBitmap         - The bitmap of UPI Ports that need to disable

  @retval        VOID
*/
VOID
EFIAPI
DisableTaggedUpiPorts (
  IN  KTI_SOCKET_DATA                  *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL         *KtiInternalGlobal,
  IN  UINT8                            TargetSocket,
  IN  UINT8                            DisPortBitmap
  )
{
  UINT8                                Idx1;
  UINT8                                PeerSoc;
  UINT8                                PeerPort;
  KTI_LINK_DATA                        *LepPtr;

  if (DisPortBitmap == 0) {
    //
    // The target socket has no port tagged to be disabled
    //
    return;
  }

  for (Idx1 = 0; Idx1 < KtiInternalGlobal->KtiPortCnt; Idx1++) {
    if ((DisPortBitmap & (1 << Idx1)) == 0) {
      continue;
    }

    PeerSoc = (UINT8) SocketData->Cpu[TargetSocket].LinkData[Idx1].PeerSocId;
    PeerPort = (UINT8) SocketData->Cpu[TargetSocket].LinkData[Idx1].PeerPort;

    LepPtr = &SocketData->Cpu[TargetSocket].LinkData[Idx1];
    ZeroMem ((VOID *) LepPtr, sizeof (KTI_LINK_DATA));

    LepPtr = &SocketData->Cpu[PeerSoc].LinkData[PeerPort];
    ZeroMem ((VOID *) LepPtr, sizeof (KTI_LINK_DATA));
  }
}

/*
  This routine will disable some tagged UPI ports in a 4S ring.
  Note:
  Both RingSocBuf and DisPortBitmapBuf are 4 bytes buffers, and they are tightly coupled.
  The elements in these two buffers are one-one mapped, for example:
  RingSocBuf[0] indicates the first socket in the 4S ring, while DisPortBitmapBuf[0] is the
  UPI Port bitmap of RingSocBuf[0] to be disabled.

  @param[in]     SocketData            - Socket specific data structure
  @param[in]     KtiInternalGlobal     - KTIRC internal variables.
  @param[in]     RingSocBuf            - A buffer contains 4 sockets that make the 4S ring
  @param[in]     DisPortBitmapBuf      - A buffer contains UPI Ports that need to disable

  @retval        VOID
*/
VOID
EFIAPI
DisableTaggedUpiPortsIn4SRing (
  IN  KTI_SOCKET_DATA                  *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL         *KtiInternalGlobal,
  IN  UINT8                            *RingSocBuf,
  IN  UINT8                            *DisPortBitmapBuf
  )
{
  UINT8                                Idx1;

  for (Idx1 = 0; Idx1 < 4; Idx1++) {
    if (DisPortBitmapBuf[Idx1] == 0) {
      //
      // The socket "RingSocBuf[Idx1]" has no port tagged to be disabled
      //
      continue;
    }

    DisableTaggedUpiPorts (SocketData, KtiInternalGlobal, RingSocBuf[Idx1], DisPortBitmapBuf[Idx1]);
  }
}

/*
  This routine examines two input sockets to see if they have a valid link to connect with each other.
  Valid link means the UPI Port of that link can support route thru. If BothLinkSide = FALSE, a link
  can be a valid link as long as the FirstSocket's UPI Port can support route thru.

  @param[in]     SocketData            - Socket specific data structure
  @param[in]     KtiInternalGlobal     - KTIRC internal variables.
  @param[in]     FirstSocket           - The first socket to be examined
  @param[in]     SecondSocket          - The second socket to be examined
  @param[in]     BothLinkSide          - TRUE: Need to check both sides of the link
                                         FALSE: Just need to check the First socket's side of the link
  @param[out]    DisPortBitmap         - Return the FirstSocket's ports that need to be disabled, could be NULL

  @retval        TRUE                  - Found a valid link between the two input sockets
  @retval        FALSE                 - No valid link found
*/
BOOLEAN
EFIAPI
HasValidUpiLinkToConnectTwoSockets (
  IN      KTI_SOCKET_DATA              *SocketData,
  IN      KTI_HOST_INTERNAL_GLOBAL     *KtiInternalGlobal,
  IN      UINT8                        FirstSocket,
  IN      UINT8                        SecondSocket,
  IN      BOOLEAN                      BothLinkSide,
  OUT     UINT8                        *DisPortBitmap
  )
{
  UINT8                                Idx1;
  UINT8                                PeerPort;
  BOOLEAN                              ValidLinkFound;

  ValidLinkFound = FALSE;

  for (Idx1 = 0; Idx1 < KtiInternalGlobal->KtiPortCnt; Idx1++) {
    if (SocketData->Cpu[FirstSocket].LinkData[Idx1].Valid == FALSE) {
      continue;
    }

    if (SocketData->Cpu[FirstSocket].LinkData[Idx1].PeerSocType != SOCKET_TYPE_CPU) {
      continue;
    }

    if (SocketData->Cpu[FirstSocket].LinkData[Idx1].PeerSocId != SecondSocket) {
      continue;
    }

    PeerPort = (UINT8) SocketData->Cpu[FirstSocket].LinkData[Idx1].PeerPort;

    if (!ValidLinkFound) {
      if (BothLinkSide) {
        if ((SocketData->Cpu[FirstSocket].LinkData[Idx1].DisallowRouteThru == FALSE) &&
            (SocketData->Cpu[SecondSocket].LinkData[PeerPort].DisallowRouteThru == FALSE)) {
          ValidLinkFound = TRUE;
          continue;
        }
      } else {
        if (SocketData->Cpu[FirstSocket].LinkData[Idx1].DisallowRouteThru == FALSE) {
          ValidLinkFound = TRUE;
          continue;
        }
      }

      //
      // Not a valid link, so tag it
      //
      if (DisPortBitmap != NULL) {
        *DisPortBitmap |= (1 << Idx1);
      }

      continue;
    }

    //
    // Already found a valid link, so do not need any other links, so tag it
    //
    if (DisPortBitmap != NULL) {
      *DisPortBitmap |= (1 << Idx1);
    }
  }

  return ValidLinkFound;
}

/*
  SPR has a limitation that UPI Port#3 does not support route thru, so the Port#3 is not allow to be used
  in 4S Ring. Apparently SPR cannot support 4S2L ring. This routine checks if Port#3 must be used to make
  a 4S1L ring. If any two sockets connected by more than one link, return the bitmap of links that are
  expected to be disabled.
  Note:
  Caller must make sure the 4 sockets in RingSocBuf are already in 4S Ring order, that is:
  RingSocBuf[0] - RingSocBuf[1] - RingSocBuf[2] - RingSocBuf[3] - RingSocBuf[0]

  @param[in]     SocketData            - Socket specific data structure
  @param[in]     KtiInternalGlobal     - KTIRC internal variables.
  @param[in]     RingSocBuf            - A buffer contains 4 sockets that can make a 4S ring
  @param[out]    DisPortBitmapBuf      - A buffer to return the UPI ports to be disabled, could be NULL

  @retval        TRUE                  - Restricted Port(#3) must be used to make 4S1L ring
  @retval        FALSE                 - Restricted Port(#3) is not necessary for the 4S1L ring
*/
BOOLEAN
EFIAPI
IsRestrictedPortUsedIn4SRing (
  IN  KTI_SOCKET_DATA                  *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL         *KtiInternalGlobal,
  IN  UINT8                            *RingSocBuf,
  OUT UINT8                            *DisPortBitmapBuf
  )
{
  BOOLEAN                              ValidLinkFound;
  UINT8                                Idx1;
  UINT8                                RootSoc;
  UINT8                                PeerSoc;
  UINT8                                *DisPortBitmapPtr;

  if (!IsSprFamilyCpu (KtiInternalGlobal->CpuType)) {
    //
    // Just return FALSE if not SPR Family CPU since only SPR has route thru limitation on Port#3.
    //
    return FALSE;
  }

  if (RingSocBuf == NULL) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n%a() - Error: Input pointer is NULL\n", __FUNCTION__));
    return TRUE;
  }

  for (Idx1 = 0; Idx1 < CPUS_PER_RING; Idx1++) {
    if (RingSocBuf[Idx1] >= MAX_SOCKET) {
      KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n%a() - Invalid SocketId: RingSocBuf[%x]=%x\n", __FUNCTION__, Idx1, RingSocBuf[Idx1]));
      return TRUE;
    }
  }

  if (DisPortBitmapBuf != NULL) {
    SetMem ((VOID *) DisPortBitmapBuf, CPUS_PER_RING, 0);
  }

  for (Idx1 = 0; Idx1 < CPUS_PER_RING; Idx1++) {
    RootSoc = RingSocBuf[Idx1];
    PeerSoc = RingSocBuf[(Idx1 + 1) % CPUS_PER_RING];

    if (DisPortBitmapBuf == NULL) {
      DisPortBitmapPtr = NULL;
    } else {
      DisPortBitmapPtr = &DisPortBitmapBuf[Idx1];
    }

    //
    // Examine each two connected sockets
    //
    ValidLinkFound = HasValidUpiLinkToConnectTwoSockets (SocketData, KtiInternalGlobal, RootSoc, PeerSoc, TRUE, DisPortBitmapPtr);
    if (!ValidLinkFound) {
      //
      // Not able to make a 4S1L ring
      //
      return TRUE;
    }
  }  // for (Idx1 ...)

  return FALSE;
}

/*
  SPR has a limitation that UPI Port#3 does not support route thru, so the Port#3 is not allow to be used
  on middle socket of a 3S chain. This routine checks if the UPI Port#3 is used by the middle socket.
  If the middle socket has more than one UPI ports connecting to its peer socket, return the bitmap of ports
  that are expected to be disabled as SPR only support 3S1L Chain.
  Note:
  Caller must make sure the 3 sockets in ChainSocBuf are already in 3S Chain order, that is:
  ChainSocBuf[0] - ChainSocBuf[1] - ChainSocBuf[2]

  @param[in]     SocketData            - Socket specific data structure
  @param[in]     KtiInternalGlobal     - KTIRC internal variables.
  @param[in]     ChainSocBuf           - A buffer contains three sockets that can make a 3S chain
  @param[out]    DisPortBitmap         - The bitmap of Middle socket's UPI ports that need to disable

  @retval        TRUE                  - Port#3 is used by middle socket (not allowed)
  @retval        FALSE                 - Port#3 is not used by middle socket
*/
BOOLEAN
EFIAPI
IsRestrictedPortOnMiddleSocketOf3SChain (
  IN  KTI_SOCKET_DATA                  *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL         *KtiInternalGlobal,
  IN  UINT8                            *ChainSocBuf,
  OUT UINT8                            *DisPortBitmap
  )
{
  BOOLEAN                              ValidLinkFound;
  UINT8                                Idx1;

  if ((ChainSocBuf == NULL) || (DisPortBitmap == NULL)) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n%a() - Error: Input pointer is NULL\n", __FUNCTION__));
    return TRUE;
  }

  for (Idx1 = 0; Idx1 < 3; Idx1++) {
    if (ChainSocBuf[Idx1] >= MAX_SOCKET) {
      KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n%a() - Invalid SocketId: ChainSocBuf[%x]=%x\n", __FUNCTION__, Idx1, ChainSocBuf[Idx1]));
      return TRUE;
    }
  }

  //
  // Examine the first socket pairs: (ChainSocBuf[1], ChainSocBuf[0])
  //
  ValidLinkFound = HasValidUpiLinkToConnectTwoSockets (SocketData, KtiInternalGlobal, ChainSocBuf[1], ChainSocBuf[0], FALSE, DisPortBitmap);
  if (!ValidLinkFound) {
    return TRUE;
  }

  //
  // Examine the second socket pairs: (ChainSocBuf[1], ChainSocBuf[2])
  //
  ValidLinkFound = HasValidUpiLinkToConnectTwoSockets (SocketData, KtiInternalGlobal, ChainSocBuf[1], ChainSocBuf[2], FALSE, DisPortBitmap);
  if (!ValidLinkFound) {
    return TRUE;
  }

  return FALSE;
}

/*
  Routine to get the number of common peer sockets between two input sockets.
  Note: Caller must make sure that Socket[SocId1] and Socket[SocId2] are peers.

  @param SocId1       - The ID of Socket#1
  @param SocId2       - The ID of Socket#2
  @param SocInfo      - A pointer to the array of SOCKET_TOPOLOGY_INFO type
  @param BufSize      - The size of the buffer(in Byte)
  @param SocBuffer    - A pointer to the buffer to hold the common peer socket IDs

  @retval             - The number of common peer sockets. In addition, if SocBuffer is not NULL,
                        at most BufSize(if they have) of common peer socket IDs will be returned
                        in the buffer pointed by SocBuffer.
*/
UINT8
EFIAPI
GetCommonPeerSocketCnt (
  IN  UINT8                 SocId1,
  IN  UINT8                 SocId2,
  IN  SOCKET_TOPOLOGY_INFO  *SocInfo,
  IN  UINT8                 BufSize,
  OUT UINT8                 *SocBuffer
  )
{
  UINT8    Idx;
  UINT8    CommonPeerSocketList;
  UINT8    CommonPeerSocCnt;

  CommonPeerSocketList = SocInfo[SocId1].PeerSocList & SocInfo[SocId2].PeerSocList;

  if (CommonPeerSocketList == 0) {  // Skip the loop if no common peer socket
    return 0;
  }

  CommonPeerSocCnt = 0;
  for (Idx = 0; Idx < MAX_SOCKET; Idx++) {
    if ((CommonPeerSocketList & (1 << Idx)) == 0) {
      continue;
    }

    if ((SocBuffer != NULL) && (CommonPeerSocCnt < BufSize)) {
      SocBuffer[CommonPeerSocCnt] = Idx;
    }

    CommonPeerSocCnt++;
  }

  return CommonPeerSocCnt;
}

/*
  Routine to discover clump(4S fully connected) that contains the input socket.

  @param SocketId          - The ID of the input Socket to be examined if it is part of a clump
  @param SocInfo           - A pointer to the array of SOCKET_TOPOLOGY_INFO type
  @param ExcludedSocList   - A pointer to a list of sockets that should be excluded (If NULL, no socket will be excluded)
  @param ExcludedSocCnt    - Number of sockets that should be excluded (If 0, no socket will be excluded)
  @param ClumpSocList      - A pointer to the buffer to store the returned Clump socket list
                             (If ClumpSocList is NULL, then will not return Clump socket list)

  @retval                  - TRUE:  A clump including SocketId is discovered
                           - FALSE: No clump that includes SocketId is discovered
*/
BOOLEAN
EFIAPI
DiscoverClump (
  IN  UINT8                     SocketId,
  IN  SOCKET_TOPOLOGY_INFO      *SocInfo,
  IN  UINT8                     *ExcludedSocList,
  IN  UINT8                     ExcludedSocCnt,
  OUT UINT8                     *ClumpSocList
  )
{
  UINT8    Idx1, Idx2;
  UINT8    SocketList[SOCKETS_NUMBER_IN_EACH_CLUMP];
  UINT8    PeerSocList;
  UINT8    CommonPeerSocCnt;
  BOOLEAN  ExcludeThisSoc;

  PeerSocList = SocInfo[SocketId].PeerSocList;

  SocketList[0] = SocketId;

  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (SocInfo[Idx1].IsSocValid == FALSE) {
      continue;
    }
    if (Idx1 == SocketId) {
      continue;
    }

    if ((ExcludedSocList != NULL) && (ExcludedSocCnt != 0)) {
      ExcludeThisSoc = FALSE;

      for (Idx2 = 0; Idx2 < ExcludedSocCnt; Idx2++) {
        if (Idx1 == ExcludedSocList[Idx2]) {
          ExcludeThisSoc = TRUE;
          break;
        }
      }
      if (ExcludeThisSoc) {
        continue;
      }
    }

    if ((PeerSocList & (1 << Idx1)) == 0) {
      continue;
    }

    SocketList[1] = Idx1;

    CommonPeerSocCnt = GetCommonPeerSocketCnt (SocketId, Idx1, SocInfo, 2, &SocketList[2]);
    if (CommonPeerSocCnt == 2) {
      KTI_ASSERT ((SocketList[2] < MAX_SOCKET) && (SocketList[3] < MAX_SOCKET) , ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_CPU_SOCKET_ID);

      if ((SocInfo[SocketList[2]].PeerSocList & (1 << SocketList[3])) != 0) {  //Socket[SocketList[2]] and Socket[SocketList[3]] are peers
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  Sockets: S[%x], S[%x], S[%x], S[%x] make a clump\n",
                             SocketList[0], SocketList[1], SocketList[2], SocketList[3]));

        if (ClumpSocList != NULL) {
          CopyMem ((VOID *) ClumpSocList, (VOID *) SocketList, sizeof (SocketList));
        }

        return TRUE;
      }
    }
  }

  //
  // Not discover a clump includes the SocketId
  //
  return FALSE;
}

/*
  Routine to degrade to a single Clump(a.k.a Fully Connect Cube).

  @param[in] SocketData         - Socket specific data structure
  @param[in] KtiInternalGlobal  - KTIRC internal variables
  @param[in] Clump              - A list of all 4 sockets included in the clump

  @retval    VOID
*/
VOID
EFIAPI
DegradeToSingleClump (
  IN  KTI_SOCKET_DATA           *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  UINT8                     *Clump
  )
{
  UINT8                         Idx1;

  KTIVAR->SysConfig = SYS_CONFIG_4S;

  //
  // Indicates that the legacy socket is in one Fully Connect Cube, so remove the other clump
  //
  RemoveUnusedSockets (SocketData,
                       KtiInternalGlobal,
                       Clump,
                       SOCKETS_NUMBER_IN_EACH_CLUMP);

  for (Idx1 = 0; Idx1 < SOCKETS_NUMBER_IN_EACH_CLUMP; Idx1++) {
    if (Clump[Idx1] >= 4) {  // If any socket ID >= 4, it should be 8S config.
      KTIVAR->SysConfig = SYS_CONFIG_8S;
      break;
    }
  }

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  Degraded to single clump: S[%x] - S[%x] - S[%x] - S[%x]\n",
                       Clump[0], Clump[1], Clump[2], Clump[3]));

  return;
}

/*
  Try to degrade to 4S Fully Connect topology.

  @param[in]     SocketData            - Socket specific data structure
  @param[in]     KtiInternalGlobal     - KTIRC internal variables.
  @param[in]     SocInfo               - A pointer to the array of SOCKET_TOPOLOGY_INFO type

  @retval        TRUE                  - Degrade to 4S Fully Connect topology successfully
  @retval        FALSE                 - Not able to degrade to 4S Fully Connect topology
*/
BOOLEAN
EFIAPI
TryToDegradeTo4SFullyConnect (
  IN  KTI_SOCKET_DATA           *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  SOCKET_TOPOLOGY_INFO      *SocInfo
  )
{
  UINT8                         Clump[4];
  BOOLEAN                       ClumpDiscovered;

  if (SocInfo[KtiInternalGlobal->SbspSoc].PeerSocCnt < 3) {
    //
    // Legacy socket has less than 3 peers, so it is not able to be a 4SFullyConnectCube
    //
    return FALSE;
  }

  ClumpDiscovered = DiscoverClump (KtiInternalGlobal->SbspSoc,
                                   SocInfo,
                                   NULL,
                                   0,
                                   Clump);

  if (!ClumpDiscovered) {
    return FALSE;
  }

  DegradeToSingleClump (SocketData, KtiInternalGlobal, Clump);

  if (IsSprFamilyCpu (KtiInternalGlobal->CpuType)) {
    RemoveDualLink (KtiInternalGlobal, SocketData);
  }

  return TRUE;
}

/*
  This routine will find 4 candidate sockets that can form a valid 4S-Ring.
  Note:
  1) It can support to find sockets to make 4S2L-Ring(DualLink = TRUE) or 4S1L ring(DualLink = FALSE).
  2) If SPR program, this routine can support to dicover a ring without using Port#3
     since this port cannot support route thru.

  @param[in]     SocketData            - Socket specific data structure
  @param[in]     KtiInternalGlobal     - KTIRC internal variables.
  @param[in]     SocInfo               - A pointer to the array of SOCKET_TOPOLOGY_INFO type
  @param[in]     DualLink              - TRUE:  Try to discover 4S2L Ring
                                         FALSE: Try to discover 4S1L Ring
  @param[out]    RingSocBuf            - 4 Sockets in ring order returned if 4S-Ring discovered
  @param[out]    DisPortBitmapBuf      - Specified ports of each socket in RingSocBuf will be disabled to make a valid 4S-Ring
                                         Note: Used by SPR only since its port#3 does not support route thru; otherwise can be NULL.

  @retval        TRUE                  - Discovered 4S-Ring successfully
  @retval        FALSE                 - No 4S-Ring discovered
*/
BOOLEAN
EFIAPI
Discover4SRing (
  IN  KTI_SOCKET_DATA           *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  SOCKET_TOPOLOGY_INFO      *SocInfo,
  IN  BOOLEAN                   DualLink,
  OUT UINT8                     *RingSocBuf,
  OUT UINT8                     *DisPortBitmapBuf
  )
{
  UINT8                         Idx1;
  UINT8                         Idx2;
  UINT8                         Idx3;
  UINT8                         CommonPeerList;
  UINT8                         BuffSize;
  UINT8                         PeerSocBuf[6];  //Support a socket has maximum of 6 UPI links

  if (RingSocBuf == NULL) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n%a() - Error: Input pointer is NULL\n", __FUNCTION__));
    return FALSE;
  }

  SetMem ((VOID *) RingSocBuf, CPUS_PER_RING, 0xFF);

  //
  // If Legacy Socket has less than two peer sockets, impossible to make a 4S-Ring
  //
  if (DualLink) {
    if (SocInfo[KtiInternalGlobal->SbspSoc].DualLinkCnt < 2) {  //Not support 4S2L Ring
      return FALSE;
    }
  } else {
    if (SocInfo[KtiInternalGlobal->SbspSoc].PeerSocCnt < 2) {  //Not support 4S1L Ring
      return FALSE;
    }
  }

  BuffSize = sizeof (PeerSocBuf);
  LocatePeerSockets (SocInfo, KtiInternalGlobal->SbspSoc, DualLink, &BuffSize, PeerSocBuf);
  if (BuffSize > sizeof (PeerSocBuf)) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, ERROR_MSG_TOO_MANY_PEERS, __FUNCTION__));
    KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
    return FALSE;
  }

  RingSocBuf[0] = KtiInternalGlobal->SbspSoc;

  for (Idx1 = 0; (Idx1 < BuffSize) && (PeerSocBuf[Idx1] < MAX_SOCKET); Idx1++) {
    RingSocBuf[1] = PeerSocBuf[Idx1];

    for (Idx2 = Idx1 + 1; (Idx2 < BuffSize) && (PeerSocBuf[Idx2] < MAX_SOCKET); Idx2++) {
      RingSocBuf[3] = PeerSocBuf[Idx2];

      if (DualLink) {
        CommonPeerList = SocInfo[RingSocBuf[1]].DualLinkPeerSocList & SocInfo[RingSocBuf[3]].DualLinkPeerSocList;
      } else {
        CommonPeerList = SocInfo[RingSocBuf[1]].PeerSocList & SocInfo[RingSocBuf[3]].PeerSocList;
      }

      for (Idx3 = 0; Idx3 < MAX_SOCKET; Idx3++) {
        if (Idx3 == RingSocBuf[0]) {
          continue;
        }

        if ((CommonPeerList & (1 << Idx3)) == 0) {
          continue;
        }

        //
        // 4S ring is discovered and it is already in 4S Ring order:
        // RingSocBuf[0] - RingSocBuf[1] - RingSocBuf[2] - RingSocBuf[3] - RingSocBuf[0]
        //
        RingSocBuf[2] = Idx3;

        //
        // Further check the restricted condition to see if this 4S Ring is valid(allowed)
        //
        if (IsRestrictedPortUsedIn4SRing (SocketData, KtiInternalGlobal, RingSocBuf, DisPortBitmapBuf)) {
          continue;
        }

        //
        // Already found 4 candidate sockets that can form a 4S-Ring
        //
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Discovered 4S-Ring: S[%x] - S[%x] - S[%x] - S[%x]\n",
                            RingSocBuf[0], RingSocBuf[1], RingSocBuf[2], RingSocBuf[3]));

        return TRUE;
      } // for (Idx3 ...)
    } // for (Idx2 ...)
  } // for (Idx1 ...)

  return FALSE;
}

/*
  Try to handle the SPR-UCC 2S topology.

  @param[in]     SocketData            - Socket specific data structure
  @param[in]     KtiInternalGlobal     - KTIRC internal variables.
  @param[in]     SocInfo               - A pointer to the array of SOCKET_TOPOLOGY_INFO type

  @retval        TRUE                  - Successfully degraded to a supported 2S(4 Nodes) topology
  @retval        FALSE                 - 2S(4 Nodes) topolody not supported
*/
BOOLEAN
EFIAPI
TryToHandleClusterOnPackage2S (
  IN  KTI_SOCKET_DATA           *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  SOCKET_TOPOLOGY_INFO      *SocInfo
  )
{
  UINT8                         RingSocBuf[CPUS_PER_RING];
  UINT8                         PrimaryClusterNodeId;
  UINT8                         SecondaryClusterNodeId;

  if (!IsCpuAndRevision (CPU_SPRSP, REV_UX)) {
    return FALSE;
  }

  if (!Discover4SRing (SocketData, KtiInternalGlobal, SocInfo, FALSE, RingSocBuf, NULL)) {
    //
    // No 4S-Ring discovered
    //
    return FALSE;
  }

  PrimaryClusterNodeId = KtiInternalGlobal->SbspSoc;
  SecondaryClusterNodeId = (UINT8) SocketData->Cpu[PrimaryClusterNodeId].LinkData[UPIMDF_PORT_NUMBER_SPRUCC].PeerSocId;

  KTIVAR->SysConfig = SYS_CONFIG_4S;
  KtiInternalGlobal->Is4SRing = TRUE;
  KtiInternalGlobal->IsRouteThrough = TRUE;

  if ((SocInfo[PrimaryClusterNodeId].DualLinkCnt == 1) &&
      (SocInfo[SecondaryClusterNodeId].DualLinkCnt == 1)) {

    //
    // Case 1: Two Dual-Link
    //
    if (((SocInfo[PrimaryClusterNodeId].DualLinkPeerSocList & (1 << EVEN_NUMBER_PEER_NODE_ID_SPRUCC)) != 0) &&
        ((SocInfo[SecondaryClusterNodeId].DualLinkPeerSocList & (1 << ODD_NUMBER_PEER_NODE_ID_SPRUCC)) != 0)) {

      KtiInternalGlobal->IsDoubleLinks = TRUE;
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, SPR_UCC_SPREAD_TOPOLOGY, "4L"));  //2S-4L Spread

    } else {

      RemoveDualLink (KtiInternalGlobal, SocketData);
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, SPR_UCC_SHADOW_TOPOLOGY, "2L(Degraded)"));  //Degraded 2S-2L Shadow

    }

  } else if ((SocInfo[PrimaryClusterNodeId].DualLinkCnt == 1) &&
             (SocInfo[SecondaryClusterNodeId].DualLinkCnt == 0)) {

    //
    // Case 2: One Dual-Link on Primary Cluster
    //
    if ((SocInfo[PrimaryClusterNodeId].DualLinkPeerSocList & (1 << EVEN_NUMBER_PEER_NODE_ID_SPRUCC)) != 0) {

      KtiInternalGlobal->IsDoubleLinks = TRUE;
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, SPR_UCC_SPREAD_TOPOLOGY, "3L"));  //2S-3L Spread

    } else {

      RemoveDualLink (KtiInternalGlobal, SocketData);
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, SPR_UCC_SHADOW_TOPOLOGY, "2L(Degraded)"));  //Degraded 2S-2L Shadow

    }

  } else if ((SocInfo[PrimaryClusterNodeId].DualLinkCnt == 0) &&
             (SocInfo[SecondaryClusterNodeId].DualLinkCnt == 1)) {

    //
    // Case 3: One Dual-Link on Secondary Cluster
    //
    RemoveDualLink (KtiInternalGlobal, SocketData);

    if ((SocInfo[SecondaryClusterNodeId].DualLinkPeerSocList & (1 << EVEN_NUMBER_PEER_NODE_ID_SPRUCC)) != 0) {

      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, SPR_UCC_SHADOW_TOPOLOGY, "2L(Degraded)"));  //Degraded 2S-2L Shadow

    } else {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, SPR_UCC_SPREAD_TOPOLOGY, "2L(Degraded)"));  //Degraded 2S-2L Spread
    }

  } else {

    //
    // Case 4: No Dual-Link
    //
    if ((SocInfo[PrimaryClusterNodeId].PeerSocCnt == 3) &&
        (SocInfo[SecondaryClusterNodeId].PeerSocCnt == 3)) {

      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, SPR_UCC_SHADOW_TOPOLOGY, "4L"));  //2S-4L Shadow
      KtiInternalGlobal->Is4SRing = FALSE;
      KtiInternalGlobal->IsRouteThrough = FALSE;

    } else if ((SocInfo[PrimaryClusterNodeId].PeerSocCnt == 3) &&
               (SocInfo[SecondaryClusterNodeId].PeerSocCnt == 2)) {

      if ((SocInfo[SecondaryClusterNodeId].PeerSocList & (1 << EVEN_NUMBER_PEER_NODE_ID_SPRUCC)) != 0) {

        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, SPR_UCC_SHADOW_TOPOLOGY, "3L"));  //2S-3L Shadow

      } else {

        DisconnectTwoSockets (SocketData, KtiInternalGlobal, PrimaryClusterNodeId, ODD_NUMBER_PEER_NODE_ID_SPRUCC);
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, SPR_UCC_SPREAD_TOPOLOGY, "2L(Degraded)"));  //Degraded 2S-2L Spread

      }

    } else if ((SocInfo[PrimaryClusterNodeId].PeerSocCnt == 2) &&
               (SocInfo[SecondaryClusterNodeId].PeerSocCnt == 3)) {

      if ((SocInfo[PrimaryClusterNodeId].PeerSocList & (1 << EVEN_NUMBER_PEER_NODE_ID_SPRUCC)) != 0) {

        DisconnectTwoSockets (SocketData, KtiInternalGlobal, SecondaryClusterNodeId, EVEN_NUMBER_PEER_NODE_ID_SPRUCC);
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, SPR_UCC_SPREAD_TOPOLOGY, "2L(Degraded)"));  //Degraded 2S-2L Spread

      } else {

        DisconnectTwoSockets (SocketData, KtiInternalGlobal, SecondaryClusterNodeId, ODD_NUMBER_PEER_NODE_ID_SPRUCC);
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, SPR_UCC_SHADOW_TOPOLOGY, "2L(Degraded)"));  //Degraded 2S-2L Shadow

      }

    } else if ((SocInfo[PrimaryClusterNodeId].PeerSocCnt == 2) &&
               (SocInfo[SecondaryClusterNodeId].PeerSocCnt == 2)) {

      if ((SocInfo[PrimaryClusterNodeId].PeerSocList & (1 << EVEN_NUMBER_PEER_NODE_ID_SPRUCC)) != 0) {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, SPR_UCC_SPREAD_TOPOLOGY, "2L"));  //2S-2L Spread
      } else {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, SPR_UCC_SHADOW_TOPOLOGY, "2L"));  //2S-2L Shadow
      }

    } else {
      //
      // Should never come here!
      //
      KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
    }

  }

  return TRUE;
}

/*
  Try to degrade to 4S Ring topology.
  Note:
  1) This is a generic implementaion that can support both 4S2L and 4S1L ring degradation.
     If want to try 4S2L ring, just need to have the input parameter DualLink = TRUE;
     If want to try 4S1L ring, just need to have the input parameter DualLink = FALSE.
  2) If SPR program, this routine can support to handle the limitation that Link#3 does not
     support route thru.

  @param[in]     SocketData            - Socket specific data structure
  @param[in]     KtiInternalGlobal     - KTIRC internal variables.
  @param[in]     SocInfo               - A pointer to the array of SOCKET_TOPOLOGY_INFO type
  @param[in]     DualLink              - TRUE:  Try if possible to degrade to 4S2L Ring
                                         FALSE: Try if possible to degrade to 4S1L Ring

  @retval        TRUE                  - Degrade to 4S Ring successfully
  @retval        FALSE                 - Not able to degrade to 4S Ring
*/
BOOLEAN
EFIAPI
TryToDegradeTo4SRing (
  IN  KTI_SOCKET_DATA           *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  SOCKET_TOPOLOGY_INFO      *SocInfo,
  IN  BOOLEAN                   DualLink
  )
{
  UINT8                         Idx1;
  UINT8                         RingSocBuf[CPUS_PER_RING];
  UINT8                         DisPortBitmapBuf[CPUS_PER_RING];

  if (!Discover4SRing (SocketData, KtiInternalGlobal, SocInfo, DualLink, RingSocBuf, DisPortBitmapBuf)) {
    //
    // No 4S-Ring discovered
    //
    return FALSE;
  }

  //
  // Remove the sockets(if any) that are not part of the 4S-Ring
  //
  RemoveUnusedSockets (SocketData, KtiInternalGlobal, RingSocBuf, sizeof (RingSocBuf));

  //
  // Remove the links between (RingSocBuf[0], RingSocBuf[2], (RingSocBuf[1], RingSocBuf[3]) pairs
  //
  DisconnectTwoSockets (SocketData, KtiInternalGlobal, RingSocBuf[0], RingSocBuf[2]);
  DisconnectTwoSockets (SocketData, KtiInternalGlobal, RingSocBuf[1], RingSocBuf[3]);

  if (DualLink) {
    KtiInternalGlobal->IsDoubleLinks = TRUE;
  } else {
    if (IsSprFamilyCpu (KtiInternalGlobal->CpuType)) {
      //
      // SPR Family CPU but not UCC chop
      //
      DisableTaggedUpiPortsIn4SRing (SocketData, KtiInternalGlobal, RingSocBuf, DisPortBitmapBuf);
    } else {
      RemoveDualLink (KtiInternalGlobal, SocketData);  // Remove dual links if any
    }
  }

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  Degraded to %a Ring\n", (DualLink ? "4S2L" : "4S1L")));

  KTIVAR->SysConfig = SYS_CONFIG_4S;
  for (Idx1 = 0; Idx1 < sizeof (RingSocBuf); Idx1++) {
    if (RingSocBuf[Idx1] >= 4) {
      KTIVAR->SysConfig = SYS_CONFIG_8S;
      break;
    }
  }

  KtiInternalGlobal->Is4SRing = TRUE;
  KtiInternalGlobal->IsRouteThrough = TRUE;

  return TRUE;
}

/*
  Try to degrade to 3S Ring topology.
  Note:
  This is a generic implementaion that can support both 3S2L and 3S1L ring degradation.
  If want to try 3S2L ring, just need to have the input parameter DualLink = TRUE;
  If want to try 3S1L ring, just need to have the input parameter DualLink = FALSE.

  @param[in]     SocketData            - Socket specific data structure
  @param[in]     KtiInternalGlobal     - KTIRC internal variables.
  @param[in]     SocInfo               - A pointer to the array of SOCKET_TOPOLOGY_INFO type
  @param[in]     DualLink              - TRUE:  Try if possible to degrade to 3S2L Ring
                                         FALSE: Try if possible to degrade to 3S1L Ring

  @retval        TRUE                  - Degrade to 3S Ring successfully
  @retval        FALSE                 - Not able to degrade to 3S Ring
*/
BOOLEAN
EFIAPI
TryToDegradeTo3SRing (
  IN  KTI_SOCKET_DATA           *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  SOCKET_TOPOLOGY_INFO      *SocInfo,
  IN  BOOLEAN                   DualLink
  )
{
  UINT8                         Idx1;
  UINT8                         Idx2;
  UINT8                         BuffSize;
  UINT8                         PeerListBitmap;
  UINT8                         PeerSocBuf[6];  //Support a socket has maximum of 6 UPI links
  UINT8                         RingSocBuf[3];

  if (DualLink) {
    if (SocInfo[KtiInternalGlobal->SbspSoc].DualLinkCnt < 2) {  //Not support 3S2L Ring
      return FALSE;
    }
  } else {
    if (SocInfo[KtiInternalGlobal->SbspSoc].PeerSocCnt < 2) {  //Not support 3S1L Ring
      return FALSE;
    }
  }

  BuffSize = sizeof (PeerSocBuf);
  LocatePeerSockets (SocInfo, KtiInternalGlobal->SbspSoc, DualLink, &BuffSize, PeerSocBuf);
  if (BuffSize > sizeof (PeerSocBuf)) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, ERROR_MSG_TOO_MANY_PEERS, __FUNCTION__));
    KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
    return FALSE;
  }

  RingSocBuf[0] = KtiInternalGlobal->SbspSoc;

  for (Idx1 = 0; (Idx1 < BuffSize) && (PeerSocBuf[Idx1] < MAX_SOCKET); Idx1++) {
    RingSocBuf[1] = PeerSocBuf[Idx1];

    if (DualLink) {
      PeerListBitmap = SocInfo[RingSocBuf[1]].DualLinkPeerSocList;
    } else {
      PeerListBitmap = SocInfo[RingSocBuf[1]].PeerSocList;
    }

    for (Idx2 = Idx1 + 1; (Idx2 < BuffSize) && (PeerSocBuf[Idx2] < MAX_SOCKET); Idx2++) {
      RingSocBuf[2] = PeerSocBuf[Idx2];

      if ((PeerListBitmap & (1 << RingSocBuf[2])) != 0) {
        RemoveUnusedSockets (SocketData, KtiInternalGlobal, RingSocBuf, sizeof (RingSocBuf));

        if (DualLink) {
          KtiInternalGlobal->IsDoubleLinks = TRUE;
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  Degraded to 3S2L Ring:"));
        } else {
          //
          // Remove dual links if any
          //
          RemoveDualLink (KtiInternalGlobal, SocketData);
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  Degraded to 3S1L Ring:"));
        }

        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, " S[%x] - S[%x] - S[%x]\n",
                             RingSocBuf[0], RingSocBuf[1], RingSocBuf[2]));

        KTIVAR->SysConfig = SYS_CONFIG_4S;
        if ((RingSocBuf[0] >= 4) || (RingSocBuf[1] >= 4) || (RingSocBuf[1] >= 4)) {
          KTIVAR->SysConfig = SYS_CONFIG_8S;
        }

        return TRUE;
      }
    }  // for (Idx2)
  }  //for (Idx1 ...)

  return FALSE;
}

/*
  Try to degrade to 3S Chain topology.
  Note:
  1) This is a generic implementaion that can support both 3S2L and 3S1L chain degradation.
     If want to try 3S2L chain, just need to have the input parameter DualLink = TRUE;
     If want to try 3S1L chain, just need to have the input parameter DualLink = FALSE.
  2) This routine will first try to discover 3S chain with legacy socket in the middle,
     if failed, then it will try to discover 3S chain with legacy socket not in the middle.
  3) If SPR program, this routine can support to handle the limitation that Link#3 does not
     support route thru.

  @param[in]     SocketData            - Socket specific data structure
  @param[in]     KtiInternalGlobal     - KTIRC internal variables.
  @param[in]     SocInfo               - A pointer to the array of SOCKET_TOPOLOGY_INFO type
  @param[in]     DualLink              - TRUE:  Try if possible to degrade to 3S2L Chain
                                         FALSE: Try if possible to degrade to 3S1L Chain

  @retval        TRUE                  - Degrade to 3S Chain successfully
  @retval        FALSE                 - Not able to degrade to 3S Chain
*/
BOOLEAN
EFIAPI
TryToDegradeTo3SChain (
  IN  KTI_SOCKET_DATA           *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  SOCKET_TOPOLOGY_INFO      *SocInfo,
  IN  BOOLEAN                   DualLink
  )
{
  UINT8                         Idx1;
  UINT8                         Idx2;
  UINT8                         BuffSize;
  UINT8                         BuffSizeTmp;
  UINT8                         PeerSocBuf[6];  //Support a socket has maximum of 6 UPI links
  UINT8                         PeerSocBufTmp[6];  //Support a socket has maximum of 6 UPI links
  UINT8                         ChainSocBuf[3];
  UINT8                         DisPortBitmap;
  BOOLEAN                       TopoDiscovered;
  BOOLEAN                       IsSpr;

  TopoDiscovered = FALSE;

  IsSpr = FALSE;
  if (IsSprFamilyCpu (KtiInternalGlobal->CpuType)) {
    IsSpr = TRUE;
  }

  //
  // Locate peers for legacy socket and store them in the PeerSocBuf[]
  //
  BuffSize = sizeof (PeerSocBuf);
  LocatePeerSockets (SocInfo, KtiInternalGlobal->SbspSoc, DualLink, &BuffSize, PeerSocBuf);
  if (BuffSize > sizeof (PeerSocBuf)) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, ERROR_MSG_TOO_MANY_PEERS, __FUNCTION__));
    KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
    return FALSE;
  }

  //
  //Examine the case that Legacy Socket in the middle of 3S chain
  //
  if (BuffSize >= 2) {
    //
    // To make ChainSocBuf[0] - ChainSocBuf[1] - ChainSocBuf[2] in the 3S Chain order, store Legacy socket in ChainSocBuf[1]
    //
    ChainSocBuf[1] = KtiInternalGlobal->SbspSoc;

    for (Idx1 = 0; Idx1 < BuffSize && PeerSocBuf[Idx1] < MAX_SOCKET; Idx1++) {
      ChainSocBuf[0] = PeerSocBuf[Idx1];

      for (Idx2 = Idx1 + 1; Idx2 < BuffSize && PeerSocBuf[Idx2] < MAX_SOCKET; Idx2++) {
        ChainSocBuf[2] = PeerSocBuf[Idx2];

        if (IsSpr) {
          DisPortBitmap = 0;

          if (IsRestrictedPortOnMiddleSocketOf3SChain (SocketData, KtiInternalGlobal, ChainSocBuf, &DisPortBitmap)) {
            //
            // For SPR, if Link#3 is used by middle socket(ie. legacy socket in this case), this is not allowed
            //
            continue;
          }
        }

        //
        // 3S Chain discovered
        //
        TopoDiscovered = TRUE;
        break;
      }  //for (Idx2 ...)

      if (TopoDiscovered) {
        break;
      }
    }  //for (Idx1 ...)
  }  //if (BuffSize >= 2)

  //
  //Examine the case that Legacy Socket not in the middle of 3S chain
  //
  if ((!TopoDiscovered) && (BuffSize >= 1)) {
    //
    // ChainSocBuf[0] (ie. Legacy socket) is the first socket of the 3S chain
    //
    ChainSocBuf[0] = KtiInternalGlobal->SbspSoc;

    for (Idx1 = 0; Idx1 < BuffSize && PeerSocBuf[Idx1] < MAX_SOCKET; Idx1++) {
      ChainSocBuf[1] = PeerSocBuf[Idx1];  // ChainSocBuf[1] is the middle socket of the 3S chain

      //
      // Below is to locate peers for ChainSocBuf[1] (ie. middle socket of the 3s chain)
      // Therefore, the PeerSocBufTmp[] contians all peers of ChainSocBuf[1] now
      //
      BuffSizeTmp = sizeof (PeerSocBufTmp);
      LocatePeerSockets (SocInfo, ChainSocBuf[1], DualLink, &BuffSizeTmp, PeerSocBufTmp);
      if (BuffSizeTmp > sizeof (PeerSocBufTmp)) {
        KtiDebugPrintFatal ((KTI_DEBUG_ERROR, ERROR_MSG_TOO_MANY_PEERS, __FUNCTION__));
        KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
        return FALSE;
      }

      for (Idx2 = 0; Idx2 < BuffSizeTmp && PeerSocBufTmp[Idx2] < MAX_SOCKET; Idx2++) {
        if (PeerSocBufTmp[Idx2] == ChainSocBuf[0]) {
          continue;
        }

        ChainSocBuf[2] = PeerSocBufTmp[Idx2];  // ChainSocBuf[2] is the last socket of the 3S chain

        if (IsSpr) {
          DisPortBitmap = 0;

          if (IsRestrictedPortOnMiddleSocketOf3SChain (SocketData, KtiInternalGlobal, ChainSocBuf, &DisPortBitmap)) {
            //
            // For SPR, if Link#3 is used by middle socket(ie. legacy socket in this case), this is not allowed
            //
            continue;
          }
        }

        //
        // 3S Chain discovered
        //
        TopoDiscovered = TRUE;
        break;
      }  //for (Idx2 ...)

      if (TopoDiscovered) {
        break;
      }
    }  //for (Idx1 ...)
  }  //if ((!TopoDiscovered) && (BuffSize >= 1))

  if (TopoDiscovered) {
    RemoveUnusedSockets (SocketData, KtiInternalGlobal, ChainSocBuf, sizeof (ChainSocBuf));
    DisconnectTwoSockets (SocketData, KtiInternalGlobal, ChainSocBuf[0], ChainSocBuf[2]);

    if (DualLink) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  Degraded to 3S2L Chain:"));
    } else {
      if (IsSpr) {
        DisableTaggedUpiPorts (SocketData, KtiInternalGlobal, ChainSocBuf[1], DisPortBitmap);
      } else {
        //
        // Remove dual links if any
        //
        RemoveDualLink (KtiInternalGlobal, SocketData);
      }

      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  Degraded to 3S1L Chain:"));
    }

    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, " S[%x] - S[%x] - S[%x]\n",
                         ChainSocBuf[0], ChainSocBuf[1], ChainSocBuf[2]));

    KTIVAR->SysConfig = SYS_CONFIG_4S;
    if ((ChainSocBuf[0] >= 4) || (ChainSocBuf[1] >= 4) || (ChainSocBuf[2] >= 4)) {
      KTIVAR->SysConfig = SYS_CONFIG_8S;
    }

    KtiInternalGlobal->IsRouteThrough = TRUE;

    return TRUE;
  }

  return FALSE;
}

/**
  This routine checks if the number of links connecting 2 given sockets match the requested
  topology including 2S4L, 2S3L, 2S2L, 2S1L.
  If match, for 2S2L, it marks the second link as DualLink on both connecting sockets.
            for 2S3L, it marks the second link and third link as TwoSkt3Link on both connecting sockets.
            for 2S4L, it marks the 2nd/3rd/4th link as TwoSkt4Link on both connecting sockets.
            then, degrade to the requested topology.
  Else just return FALSE indicating that the requested topology is not supported.

  @param[in]  KtiInternalGlobal      KTIRC internal variables.
  @param[in]  SocketData             Socket specific data structure
  @param[in]  RootSocId              Root socket ID.
  @param[in]  PeerSocId              Peer socket ID.
  @param[in]  NumOfLinksToCheck      Number of active UPI links to check, value should be 1, 2, 3 or 4

  @retval     TRUE                   Degraded to 2SnL(n=1,2,3,4) as expected.
  @retval     FALSE                  Not support the expected 2S configuration.
**/
BOOLEAN
EFIAPI
HandleTwoSocketMultipleLinks (
  IN  KTI_SOCKET_DATA                *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL       *KtiInternalGlobal,
  IN  UINT8                          RootSocId,
  IN  UINT8                          PeerSocId,
  IN  UINT8                          NumOfLinksToCheck
  )
{
  UINT8                              Idx1;
  UINT8                              LocalPort;
  UINT8                              PeerPort;
  UINT8                              TempPort;
  UINT8                              TempPort2;
  UINT8                              ActiveLinkCnt;
  UINT8                              ActiveLinkBuf[MAX_FW_KTI_PORTS];  // Support a socket with maximum number of links
  UINT8                              TwoSocBuf[2];

  ActiveLinkCnt = 0;

  SetMem ((VOID *) ActiveLinkBuf, sizeof (ActiveLinkBuf), 0xFF);

  for (Idx1 = 0; (Idx1 < KtiInternalGlobal->KtiPortCnt) && (ActiveLinkCnt < MAX_FW_KTI_PORTS); Idx1++) {
    if (SocketData->Cpu[RootSocId].LinkData[Idx1].Valid == FALSE) {
      continue;
    }
    if (SocketData->Cpu[RootSocId].LinkData[Idx1].PeerSocType != SOCKET_TYPE_CPU) {
      continue;
    }
    if (SocketData->Cpu[RootSocId].LinkData[Idx1].PeerSocId != PeerSocId) {
      continue;
    }

    ActiveLinkBuf[ActiveLinkCnt] = Idx1;
    ActiveLinkCnt++;
  }

  if (ActiveLinkCnt < NumOfLinksToCheck) {
    KtiDebugPrintInfo0 ((
      KTI_DEBUG_INFO0,
      "\nSetup 2S%dL Failed! Found %d active links between S[%d] and S[%d]!\n",
      NumOfLinksToCheck,
      ActiveLinkCnt,
      RootSocId,
      PeerSocId
      ));
    return FALSE;
  }

  for (Idx1 = 0; (Idx1 < ActiveLinkCnt) && (ActiveLinkBuf[Idx1] < KtiInternalGlobal->KtiPortCnt); Idx1++) {
    LocalPort = ActiveLinkBuf[Idx1];
    PeerPort = (UINT8) SocketData->Cpu[RootSocId].LinkData[LocalPort].PeerPort;

    if (Idx1 >= NumOfLinksToCheck) {
      //
      // We've already found sufficient number of active links required.
      // Remove the extra active links
      //
      SocketData->Cpu[RootSocId].LinkData[LocalPort].Valid = 0;
      SocketData->Cpu[PeerSocId].LinkData[PeerPort].Valid = 0;

      ActiveLinkBuf[Idx1] = 0xFF;
      continue;
    }

    if ((NumOfLinksToCheck == 2) && (Idx1 > 0)) {
      SocketData->Cpu[RootSocId].LinkData[LocalPort].DualLink = TRUE;
      SocketData->Cpu[PeerSocId].LinkData[PeerPort].DualLink = TRUE;

      //
      // Update DualLinkIndex for Root socket
      //
      TempPort = ActiveLinkBuf[0];
      SocketData->Cpu[RootSocId].LinkData[LocalPort].DualLinkIndex = TempPort;
      SocketData->Cpu[RootSocId].LinkData[TempPort].DualLinkIndex = LocalPort;

      //
      // Update DualLinkIndex for Peer socket
      //
      TempPort2 = (UINT8) SocketData->Cpu[RootSocId].LinkData[TempPort].PeerPort;
      SocketData->Cpu[PeerSocId].LinkData[PeerPort].DualLinkIndex = TempPort2;
      SocketData->Cpu[PeerSocId].LinkData[TempPort2].DualLinkIndex = PeerPort;

      if (SocketData->Cpu[RootSocId].LinkData[LocalPort].PeerPort != LocalPort) {
        KTIVAR->TwistedKtiLinks = TRUE;
      }
    } else if ((NumOfLinksToCheck == 3) && (Idx1 > 0)) {
      SocketData->Cpu[RootSocId].LinkData[LocalPort].TwoSkt3Link = TRUE;
      SocketData->Cpu[PeerSocId].LinkData[PeerPort].TwoSkt3Link = TRUE;
    } else if ((NumOfLinksToCheck == 4) && (Idx1 > 0)) {
        SocketData->Cpu[RootSocId].LinkData[LocalPort].TwoSkt4Link = TRUE;
        SocketData->Cpu[PeerSocId].LinkData[PeerPort].TwoSkt4Link = TRUE;
    }
  }

  switch (NumOfLinksToCheck) {
    case 1:
      KTIVAR->SysConfig = SYS_CONFIG_2S1L;
      break;

    case 2:
      KTIVAR->SysConfig = SYS_CONFIG_2S2L;
      if (Is16EntryUpiRoutingTableSupported ()) {
        KtiInternalGlobal->UpiRoutingMode = UpiRoutingModeB2;
      } else {
        KtiInternalGlobal->UpiRoutingMode = UpiRoutingModeB0;
      }
      break;

    case 3:
      KTIVAR->SysConfig = SYS_CONFIG_2S3L;
      if (Is16EntryUpiRoutingTableSupported ()) {
        KtiInternalGlobal->UpiRoutingMode = UpiRoutingModeC1;
      } else {
        KtiInternalGlobal->UpiRoutingMode = UpiRoutingModeC0;
      }
      break;

    case 4:
      KTIVAR->SysConfig = SYS_CONFIG_2S4L;
      //
      // Check whether it's 2S4L twisted mode
      // S0U0 <-> S1U1 S0U1 <-> S1U0 S0U2 <-> S1U2 S0U3 <-> S1U3
      //
      if (SocketData->Cpu[RootSocId].LinkData[0].PeerPort != 0) {
        KTIVAR->TwistedKtiLinks = TRUE;
      }
      KtiInternalGlobal->UpiRoutingMode = UpiRoutingModeD0;
      break;

    default:
      break;
  }

  TwoSocBuf[0] = RootSocId;
  TwoSocBuf[1] = PeerSocId;
  RemoveUnusedSockets (SocketData, KtiInternalGlobal, TwoSocBuf, sizeof (TwoSocBuf));
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  Degraded to 2S%xL: S[%x] - S[%x]\n", NumOfLinksToCheck, RootSocId, PeerSocId));

  return TRUE;
}

/*
  Try to degrade to 2S topology in this order: 2S4L, 2S3L, 2S2L and 2S1L.

  @param[in]     SocketData            - Socket specific data structure
  @param[in]     KtiInternalGlobal     - KTIRC internal variables.
  @param[in]     SocInfo               - A pointer to the array of SOCKET_TOPOLOGY_INFO type

  @retval        TRUE                  - Degrade to 2S Ring successfully
  @retval        FALSE                 - Not able to degrade to 2S Ring
*/
BOOLEAN
EFIAPI
TryToDegradeTo2S (
  IN  KTI_SOCKET_DATA           *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  SOCKET_TOPOLOGY_INFO      *SocInfo
  )
{
  BOOLEAN                       DegradeDone;
  UINT8                         Idx1;
  UINT8                         NumOfLinksToCheck;
  UINT8                         BuffSize;
  UINT8                         PeerSocBuf[MAX_FW_KTI_PORTS];  // Support a socket with maximum number of links

  if (SocInfo[KtiInternalGlobal->SbspSoc].PeerSocCnt < 1) {
    return FALSE;
  }

  //
  // Locate peers for legacy socket and store them in the PeerSocBuf[]
  //
  BuffSize = sizeof (PeerSocBuf);
  LocatePeerSockets (SocInfo, KtiInternalGlobal->SbspSoc, FALSE, &BuffSize, PeerSocBuf);
  if (BuffSize > sizeof (PeerSocBuf)) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, ERROR_MSG_TOO_MANY_PEERS, __FUNCTION__));
    KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
    return FALSE;
  }

  DegradeDone = FALSE;
  for (NumOfLinksToCheck = MAX_2S_LINKS; NumOfLinksToCheck > 0; NumOfLinksToCheck--) {
    //
    // Handle 2S system in this order[2S6L, 2S5L, 2S4L, 2S3L, 2S2L, 2S1L] in order to keep as many actived link as possible
    // to gain best performance.
    //
    for (Idx1 = 0; (Idx1 < BuffSize) && (PeerSocBuf[Idx1] < MAX_SOCKET); Idx1++) {
      DegradeDone = HandleTwoSocketMultipleLinks (SocketData, KtiInternalGlobal, KtiInternalGlobal->SbspSoc, PeerSocBuf[Idx1], NumOfLinksToCheck);
      if (DegradeDone) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

/*
  Process the serialized topology degradation according to the degrade rules.

  @param[in] SocketData               - Socket specific data structure
  @param[in] KtiInternalGlobal        - KTIRC internal variables.
  @param[in] SocInfo                  - A pointer to the array of SOCKET_TOPOLOGY_INFO type

  @retval    TRUE                     - Topology degradation is done successfully.
*/
BOOLEAN
EFIAPI
ProcessSerializedTopologyDegradation (
  IN  KTI_SOCKET_DATA           *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  SOCKET_TOPOLOGY_INFO      *SocInfo
  )
{
  BOOLEAN                       DegradeResult;

  KTIVAR->SysConfig = SYS_CONFIG_4S;

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

  if (IsCpuAndRevision (CPU_SPRSP, REV_UX)) {
    DegradeResult = TryToHandleClusterOnPackage2S (SocketData, KtiInternalGlobal, SocInfo);
    if (!DegradeResult) {
      //
      // Degrade to SPR-UCC 1S
      //
      RemoveOneSpecifiedSocket (SocketData, KtiInternalGlobal, EVEN_NUMBER_PEER_NODE_ID_SPRUCC);
      RemoveOneSpecifiedSocket (SocketData, KtiInternalGlobal, ODD_NUMBER_PEER_NODE_ID_SPRUCC);
      KTIVAR->SysConfig = SYS_CONFIG_2S1L;

      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  Degraded to SPR-UCC: 1S\n"));
    }

    return TRUE;
  }

  DegradeResult = TryToDegradeTo4SFullyConnect (SocketData, KtiInternalGlobal, SocInfo);
  if (DegradeResult) {
    return TRUE;
  }

  DegradeResult = TryToDegradeTo4SRing (SocketData, KtiInternalGlobal, SocInfo, FALSE);  //4S1L Ring
  if (DegradeResult) {
    return TRUE;
  }

  DegradeResult = TryToDegradeTo3SRing (SocketData, KtiInternalGlobal, SocInfo, TRUE);  //3S2L Ring
  if (DegradeResult) {
    return TRUE;
  }

  DegradeResult = TryToDegradeTo3SRing (SocketData, KtiInternalGlobal, SocInfo, FALSE);  //3S1L Ring
  if (DegradeResult) {
    return TRUE;
  }

  DegradeResult = TryToDegradeTo3SChain (SocketData, KtiInternalGlobal, SocInfo, FALSE);  //3S1L Chain
  if (DegradeResult) {
    return TRUE;
  }

  DegradeResult = TryToDegradeTo2S (SocketData, KtiInternalGlobal, SocInfo);
  if (DegradeResult) {
    return TRUE;
  }

  DegradeTo1S (SocketData, KtiInternalGlobal);
  KTIVAR->SysConfig = SYS_CONFIG_1S;
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  Degraded to 1S.\n"));
  return TRUE;
}
