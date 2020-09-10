/** @file

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
#include <KtiMisc.h>
#include <Library/MemoryCoreLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/KtiApi.h>

STATIC
KTI_STATUS
Map8SNodes (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_DISC_DATA            *DiscEngData
  );

STATIC
KTI_STATUS
Map8SRoutes (
  KTI_DISC_DATA            *DiscEngData,
  UINT8                    (*OemRoute)[4][2]
  );

STATIC
KTI_STATUS
Map8SRoutes_VN1Disable (
  KTI_DISC_DATA            *DiscEngData,
  UINT8                    (*OemRoute)[4][3]
  );

STATIC
KTI_STATUS
Map6SNodes (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_DISC_DATA            *DiscEngData
  );

STATIC
KTI_STATUS
Map6SRoutes (
  KTI_DISC_DATA            *DiscEngData,
  UINT8         (*OemRoute)[3][3]
  );


/**
  Check this Child Node is in this route

  @param Child       - Pointer to child topology tree node
  @param DiscEngData - Pointer to socket specific data

  @retval TRUE  - yes
  @retval FALSE - no

**/
BOOLEAN
CheckChildNodeInThisRoute (
  UINT8               RootSocket,
  TOPOLOGY_TREE_NODE  *Parent,
  TOPOLOGY_TREE_NODE  *Child,
  UINT8               (*OemRoute)[4][2]
  )
{
  BOOLEAN       AddNode;
  UINT8         PathNum;

  //
  // If it is 1 Hop away node (i.e immediate neighbor), just add the node
  //
  AddNode = FALSE;
  if (Child->Node.Hop == 1) {
    AddNode = TRUE;
  } else {
    //
    // If it is 2 Hop away node, make sure the template routing indicates this route to be used
    //
    for (PathNum = 0; PathNum < 4; PathNum++) {
      if (OemRoute[RootSocket][PathNum][0] == Parent->Node.SocId && OemRoute[RootSocket][PathNum][1] == Child->Node.SocId) {
        AddNode = TRUE;
        break;
      }
    }
  }

  return AddNode;
}

/**
  Check this Child Node is in this route

  @param Child       - Pointer to child topology tree node
  @param DiscEngData - Pointer to socket specific data
  @retval TRUE  - yes
  @retval FALSE - no

**/
BOOLEAN
CheckChildNodeInThisRoute_VN1Disable (
  KTI_DISC_DATA       *DiscEngData,
  UINT8               RootSocket,
  TOPOLOGY_TREE_NODE  *Parent,
  TOPOLOGY_TREE_NODE  *Child,
  UINT8               (*OemRoute_VN1Disable)[4][3]
  )
{
  BOOLEAN       AddNode;
  UINT8         PathNum;
  TOPOLOGY_TREE_NODE  GrandParent;

  //
  // If it is 1 Hop away node (i.e immediate neighbor), just add the node
  //
  AddNode = FALSE;
  if (Child->Node.Hop == 1) {
    AddNode = TRUE;
  } else if (Child->Node.Hop == 2) {
    //
    // If it is 2 Hop away node, make sure the template routing indicates this route to be used
    //
    for (PathNum = 0; PathNum < 4; PathNum++) {
      if (OemRoute_VN1Disable[RootSocket][PathNum][0] == Parent->Node.SocId &&
          OemRoute_VN1Disable[RootSocket][PathNum][1] == Child->Node.SocId ) {
          AddNode = TRUE;
          break;
      }
    }
  } else {
    //
    // If it is 3 Hop away node, make sure the template routing indicates this route to be used
    //
    GrandParent = DiscEngData->CpuTree[RootSocket][Parent->Node.ParentIndex];
    for (PathNum = 0; PathNum < 4; PathNum++) {
      if (OemRoute_VN1Disable[RootSocket][PathNum][0] == GrandParent.Node.SocId &&
          OemRoute_VN1Disable[RootSocket][PathNum][1] == Parent->Node.SocId &&
          OemRoute_VN1Disable[RootSocket][PathNum][2] == Child->Node.SocId) {
        AddNode = TRUE;
        break;
      }
    }
  }

  return AddNode;
}


/**
  Computes the KTI CPU topology found in the system

  @param KtiInternalGlobal - KTIRC internal variables.
  @param DiscEngData       - Pointer to socket specific data

   @retval 0     - Success
   @retval Other - Failure

**/
KTI_STATUS
ComputeTopologyTree8S (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_DISC_DATA              *DiscEngData
  )
{
  UINT8         OemRoute_VN1Disable[MAX_SOCKET][4][3];
  UINT8         OemRoute[MAX_SOCKET][4][2];
  UINT8         Idx1, Idx2, KtiLink, MaxHop;
  TOPOLOGY_TREE_NODE  Parent, Child;
  KTI_LINK_DATA       ParentLep;
  BOOLEAN       AddNode;

  SetMem ((VOID *) OemRoute, sizeof (OemRoute), 0xFF);
  SetMem ((VOID *) OemRoute_VN1Disable, sizeof (OemRoute_VN1Disable), 0xFF);
  Map8SNodes (KtiInternalGlobal, DiscEngData);

  if (KTIVAR->OutVn1En == TRUE) {
    Map8SRoutes (DiscEngData, OemRoute);
    MaxHop = 2;
  } else {
    Map8SRoutes_VN1Disable (DiscEngData, OemRoute_VN1Disable);
    MaxHop = 3;
  }


  //
  // Construct the topology tree that uses the template routing
  //
  Child.RawNode    = 0;
  Child.Node.Valid = TRUE;
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (DiscEngData->Cpu[Idx1].Valid == TRUE) {
      for (Idx2 = 0; DiscEngData->CpuTree[Idx1][Idx2].Node.Valid == TRUE && DiscEngData->CpuTree[Idx1][Idx2].Node.Hop < MaxHop; Idx2++) {

        Parent = DiscEngData->CpuTree[Idx1][Idx2];
        Child.Node.Hop = Parent.Node.Hop + 1; // One level down from parent
        Child.Node.ParentIndex = Idx2; // Remember who is the immediate parent of this child

        for (KtiLink = 0; KtiLink < KtiInternalGlobal->KtiPortCnt; KtiLink++) { // Explore all the links of this parent

          ParentLep = DiscEngData->Cpu[Parent.Node.SocId].LinkData[KtiLink];
          if (ParentLep.Valid != TRUE || ParentLep.DualLink == TRUE || ParentLep.TwoSkt3Link == TRUE) {
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
          if (ShorterPathExists (DiscEngData->CpuTree[Idx1], &Child) == TRUE) {
            continue;
          }

          if (KTIVAR->OutVn1En == TRUE) {
             AddNode = CheckChildNodeInThisRoute (Idx1, &Parent, &Child, OemRoute);
          } else {
             AddNode = CheckChildNodeInThisRoute_VN1Disable (DiscEngData, Idx1, &Parent, &Child, OemRoute_VN1Disable);
          }

          if (AddNode == FALSE) {
            continue;
          }

          if (AddNodeToTopologyTree (DiscEngData->CpuTree[Idx1], &Child) != KTI_SUCCESS) {
            KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n  Adding CPU%u to Topology Discovery Tree of CPU%u failed.",
                                 Child.Node.SocId, Idx1));
            KTI_ASSERT (FALSE, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_5D);
          }
        } // for(KtiLink...)
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  This routine establishes a mapping between intel's ref platform and this platform. Port number is not relevant.

  Ref Platform:

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


  OEM Platform:

     +-------------------------+
     |                         |
    S1 ----- S2       S3 ----- S6
     |       | \     /|        |
     |       |   \ /  |        |
     |       |    \   |        |
     |       |  /   \ |        |
     |       |/      \|        |
    S5 ----- S7       S0 ----- S4
     |                         |
     +-------------------------+



  Due to isomorphic nature of the 8SG topology, the OEM Platform can be redrawn as:

     +-------------------------+
     |                         |
    S0 ----- S4       S1 ----- S2
     |       | \     /|        |
     |       |   \ /  |        |
     |       |    \   |        |
     |       |  /   \ |        |
     |       |/      \|        |
    S3 ----- S6       S5 ----- S7
     |                         |
     +-------------------------+


  So the socket mapping would be:

    Intel   OEM
      S0     S0
      S1     S4
      S2     S3
      S3     S6
      S4     S5
      S5     S7
      S6     S1
      S7     S2

   @param KtiInternalGlobal - KTIRC internal variables.
   @param DiscEngData       - Pointer to socket specific data

   @retval 0     - Success
   @retval Other - Failure

**/
STATIC
KTI_STATUS
Map8SNodes (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_DISC_DATA            *DiscEngData
  )
{
  UINT8               Idx1, KtiLink;
  UINT8               RingNum1, RingNum2, RingNum3;
  UINT8               RefSoc;
  KTI_LINK_DATA       ParentLep;
  UINT8               TempSoc1 =0 , TempSoc2=0;


  //
  // Begin with S0 of this platform, identify its pair and map it to S0, S2 of ref platform
  //
  RefSoc = 0;
  DiscEngData->SocMap[0] = RefSoc;
  DiscEngData->SocMap[2] = DiscEngData->RingNodePairs[RefSoc];

  // Identify S0's two rings present in this platform
  RingNum1 = DiscEngData->RingInfo[RefSoc][1];
  RingNum2 = DiscEngData->RingInfo[RefSoc][2];

  // Identify the other 2 nodes of S0's first ring
  for (Idx1 = 0; Idx1 < CPUS_PER_RING; Idx1++) {
    if (DiscEngData->Rings[RingNum1][Idx1] == RefSoc || DiscEngData->Rings[RingNum1][Idx1] == DiscEngData->RingNodePairs[RefSoc]) {
      continue;
    }
    TempSoc1 = DiscEngData->Rings[RingNum1][Idx1];
    break;
  }
  TempSoc2 = DiscEngData->RingNodePairs[TempSoc1];

  //
  // Make sure TempSoc2 is the diagonally opposite in S0's first ring; otherwise swap out TempSoc1 & TempSoc2
  //
  for (KtiLink = 0; KtiLink < KtiInternalGlobal->KtiPortCnt; KtiLink++) {
    ParentLep = DiscEngData->Cpu[RefSoc].LinkData[KtiLink];
    if (ParentLep.Valid == TRUE) {
      if (ParentLep.PeerSocId == TempSoc2) {
        TempSoc2 = TempSoc1;
        TempSoc1 = (UINT8)ParentLep.PeerSocId;
        break;
      }
    }
  }

  // TempSoc1 and TempSoc2 can be now mapped to S1 and S3 in intel ref platform
  DiscEngData->SocMap[1] = TempSoc1;
  DiscEngData->SocMap[3] = TempSoc2;


  // Identify the other 2 nodes of S0's second ring
  for (Idx1 = 0; Idx1 < CPUS_PER_RING; Idx1++) {
    if (DiscEngData->Rings[RingNum2][Idx1] == RefSoc || DiscEngData->Rings[RingNum2][Idx1] == DiscEngData->RingNodePairs[RefSoc]) {
      continue;
    }
    TempSoc1 = DiscEngData->Rings[RingNum2][Idx1];
    break;
  }
  TempSoc2 = DiscEngData->RingNodePairs[TempSoc1];

  //
  // Make sure TempSoc2 is the diagonally opposite in S0's second ring; otherwise swap out TempSoc1 & TempSoc2
  //
  for (KtiLink = 0; KtiLink < KtiInternalGlobal->KtiPortCnt; KtiLink++) {
    ParentLep = DiscEngData->Cpu[RefSoc].LinkData[KtiLink];
    if (ParentLep.Valid == TRUE) {
      if (ParentLep.PeerSocId == TempSoc2) {
        TempSoc2 = TempSoc1;
        TempSoc1 = (UINT8)ParentLep.PeerSocId;
        break;
      }
    }
  }

  // TempSoc1 and TempSoc2 can be now mapped to S7 and S5 in intel ref platform
  DiscEngData->SocMap[7] = TempSoc1;
  DiscEngData->SocMap[5] = TempSoc2;


  RefSoc = DiscEngData->SocMap[1];

  // Identify RefSoc's ring that doesn't belong to S0
  if (DiscEngData->RingInfo[RefSoc][1] == RingNum1 || DiscEngData->RingInfo[RefSoc][1] == RingNum2) {
    RingNum3 = DiscEngData->RingInfo[RefSoc][2];
  } else {
    RingNum3 = DiscEngData->RingInfo[RefSoc][1];
  }

  // Identify the other 2 nodes ofRefSoc's ring
  for (Idx1 = 0; Idx1 < CPUS_PER_RING; Idx1++) {
    if (DiscEngData->Rings[RingNum3][Idx1] == RefSoc || DiscEngData->Rings[RingNum3][Idx1] == DiscEngData->RingNodePairs[RefSoc]) {
      continue;
    }
    TempSoc1 = DiscEngData->Rings[RingNum3][Idx1];
    break;
  }
  TempSoc2 = DiscEngData->RingNodePairs[TempSoc1];

  //
  // Make sure TempSoc2 is the diagonally opposite in RefSoc's ring; otherwise swap out TempSoc1 & TempSoc2
  //
  for (KtiLink = 0; KtiLink < KtiInternalGlobal->KtiPortCnt; KtiLink++) {
    ParentLep = DiscEngData->Cpu[RefSoc].LinkData[KtiLink];
    if (ParentLep.Valid == TRUE) {
      if (ParentLep.PeerSocId == TempSoc2) {
        TempSoc2 = TempSoc1;
        TempSoc1 = (UINT8)ParentLep.PeerSocId;
        break;
      }
    }
  }

  // TempSoc1 and TempSoc2 can be now mapped to S4 and S6 in intel ref platform
  DiscEngData->SocMap[4] = TempSoc1;
  DiscEngData->SocMap[6] = TempSoc2;

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n    Socket Mapping"));
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n      %u  -  %u ", Idx1, DiscEngData->SocMap[Idx1]));
  }

  return KTI_SUCCESS;
}

/**
  Uses the following static routing as template and produces the routing for the given platform
  using the socket mapping.

  S0 -> S2 -> S3;   S0 -> S7 -> S6;   S0 -> S1 -> S4;   S0 -> S2 -> S5
  S1 -> S3 -> S2;   S1 -> S3 -> S6;   S1 -> S4 -> S5;   S1 -> S0 -> S7
  S2 -> S0 -> S1;   S2 -> S3 -> S6;   S2 -> S5 -> S4;   S2 -> S0 -> S7
  S3 -> S1 -> S0;   S3 -> S1 -> S4;   S3 -> S2 -> S5;   S3 -> S6 -> S7
  S4 -> S1 -> S0;   S4 -> S5 -> S2;   S4 -> S6 -> S3;   S4 -> S6 -> S7
  S5 -> S7 -> S0;   S5 -> S2 -> S3;   S5 -> S4 -> S1;   S5 -> S7 -> S6
  S6 -> S7 -> S0;   S6 -> S3 -> S2;   S6 -> S4 -> S1;   S6 -> S4 -> S5
  S7 -> S5 -> S2;   S7 -> S6 -> S3;   S7 -> S0 -> S1;   S7 -> S5 -> S4

  @param DiscEngData - Pointer to socket specific data
  @param OemRoute    -

  @retval 0     - Success
  @retval Other - Failure

**/
STATIC
KTI_STATUS
Map8SRoutes (
  KTI_DISC_DATA            *DiscEngData,
  UINT8                    (*OemRoute)[4][2]
  )
{
  UINT8         RefRoute[8][4][2] = {
                {{2, 3}, {7, 6}, {1, 4}, {2, 5}}, // S0
                {{3, 2}, {3, 6}, {4, 5}, {0, 7}}, // S1
                {{0, 1}, {3, 6}, {5, 4}, {0, 7}}, // S2
                {{1, 0}, {1, 4}, {2, 5}, {6, 7}}, // S3
                {{1, 0}, {5, 2}, {6, 3}, {6, 7}}, // S4
                {{7, 0}, {2, 3}, {4, 1}, {7, 6}}, // S5
                {{7, 0}, {3, 2}, {4, 1}, {4, 5}}, // S6
                {{5, 2}, {6, 3}, {0, 1}, {5, 4}}  // S7
             };
  UINT8         Idx1, Idx2, Idx3, OemSoc;

  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    OemSoc = DiscEngData->SocMap[Idx1];
    for (Idx2 = 0; Idx2 < 4; Idx2++) {
      for (Idx3 = 0; Idx3 < 2; Idx3++) {
        OemRoute[OemSoc][Idx2][Idx3] = DiscEngData->SocMap[RefRoute[Idx1][Idx2][Idx3]];
      }
    }
  }

  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n"));
    for (Idx2 = 0; Idx2 < 4; Idx2++) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "  S%u -> S%u -> S%u", Idx1, OemRoute[Idx1][Idx2][0], OemRoute[Idx1][Idx2][1]));
      KTI_ASSERT ((OemRoute[Idx1][Idx2][0] != 0xFF && OemRoute[Idx1][Idx2][1] != 0xFF), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_5F);
    }
  }

  return KTI_SUCCESS;
}

/**
  Uses the following static routing as template and produces the routing for the given platform
  using the socket mapping.

  S0 -> S7;  S0 -> S2 -> S5; S0 -> S1 -> S4 -> S6; S0 -> S2 -> S3
  S1 -> S0 -> S7; S1 -> S3 -> S2 -> S5; S1 -> S4 -> S6
  S2 -> S0 -> S7; S2 -> S5 -> S4; S2 -> S3 -> S6;  S2 -> S0 -> S1
  S3 -> S6 -> S7; S3 -> S2 -> S5; S3 -> S1 -> S4;  S3 -> S1 -> S0
  S4 -> S5 -> S7; S4 -> S6; S4 -> S1 -> S0; S4 -> S5 -> S2 -> S3
  S5 -> S7 -> S6; S5 -> S7 -> S0; S5 -> S4 -> S1; S5 -> S2 -> S3
  S6 -> S4 -> S5; S6 -> S3 -> S2; S6 -> S7 -> S0; S6 -> S4 -> S1
  S7 -> S5 -> S4; S7 -> S5 -> S2; S7 -> S0 -> S1; S7 -> S6 -> S3

  @param DiscEngData - Pointer to socket specific data
  @param OemRoute    -

  @retval 0     - Success
  @retval Other - Failure

**/
STATIC
KTI_STATUS
Map8SRoutes_VN1Disable (
  KTI_DISC_DATA            *DiscEngData,
  UINT8                    (*OemRoute)[4][3]
  )
{
  UINT8     RefRoute[8][4][3] = {
                {{2, 3, 0xFF}, {2, 5, 0xFF}, {1, 4,    6}, {   7, 0xFF, 0xFF}}, // S0
                {{0, 7, 0xFF}, {4, 6, 0xFF}, {3, 2,    5}, {0xFF, 0xFF, 0xFF}}, // S1
                {{0, 1, 0xFF}, {3, 6, 0xFF}, {5, 4, 0xFF}, {   0,    7, 0xFF}}, // S2
                {{1, 0, 0xFF}, {1, 4, 0xFF}, {2, 5, 0xFF}, {   6,    7, 0xFF}}, // S3
                {{1, 0, 0xFF}, {5, 7, 0xFF}, {5, 2,    3}, {   6, 0xFF, 0xFF}}, // S4
                {{7, 0, 0xFF}, {2, 3, 0xFF}, {4, 1, 0xFF}, {   7,    6, 0xFF}}, // S5
                {{7, 0, 0xFF}, {3, 2, 0xFF}, {4, 1, 0xFF}, {   4,    5, 0xFF}}, // S6
                {{5, 2, 0xFF}, {6, 3, 0xFF}, {0, 1, 0xFF}, {   5,    4, 0xFF}}  // S7
             };

  UINT8         Idx1, Idx2, Idx3, OemSoc;

  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    OemSoc = DiscEngData->SocMap[Idx1];
    for (Idx2 = 0; Idx2 < 4; Idx2++) {
      for (Idx3 = 0; Idx3 < 3; Idx3++) {
        if (RefRoute[Idx1][Idx2][Idx3] != 0xFF) {
          OemRoute[OemSoc][Idx2][Idx3] = DiscEngData->SocMap[RefRoute[Idx1][Idx2][Idx3]];
        }
      }
    }
  }

  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (DiscEngData->Cpu[Idx1].Valid == TRUE) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\n"));
      for (Idx2 = 0; Idx2 < 4; Idx2++) {
          if (OemRoute[Idx1][Idx2][0] != 0xFF) {
            KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "  S%u->S%u", Idx1, OemRoute[Idx1][Idx2][0]));
            if (OemRoute[Idx1][Idx2][1] != 0xFF) {
              KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "->S%u", OemRoute[Idx1][Idx2][1]));
            }
            if (OemRoute[Idx1][Idx2][2] != 0xFF) {
              KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "->S%u", OemRoute[Idx1][Idx2][2]));
            }
            KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "; "));
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

  @retval 0     - Success
  @retval Other - Failure

**/
KTI_STATUS
ComputeTopologyTree6S (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_DISC_DATA            *DiscEngData
  )
{
  UINT8         OemRoute[MAX_SOCKET][3][3];
  UINT8         Idx1, Idx2, Idx3, KtiLink;
  TOPOLOGY_TREE_NODE  GrandParent, Parent, Child;
  KTI_LINK_DATA       ParentLep;
  BOOLEAN       AddNode;

  SetMem ((VOID *) OemRoute, sizeof (OemRoute), 0xFF);
  Map6SNodes (KtiInternalGlobal, DiscEngData);
  Map6SRoutes (DiscEngData, OemRoute);

  //
  // Construct the topology tree that uses the template routing
  //
  Child.RawNode    = 0;
  Child.Node.Valid = TRUE;
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (DiscEngData->Cpu[Idx1].Valid == TRUE) {
      for (Idx2 = 0;  DiscEngData->CpuTree[Idx1][Idx2].Node.Valid == TRUE && DiscEngData->CpuTree[Idx1][Idx2].Node.Hop < MAX_NUM_HOPS; Idx2++) {

        Parent = DiscEngData->CpuTree[Idx1][Idx2];
        Child.Node.Hop = Parent.Node.Hop + 1; // One level down from parent
        Child.Node.ParentIndex = Idx2; // Remember who is the immediate parent of this child

        for (KtiLink = 0; KtiLink < KtiInternalGlobal->KtiPortCnt; KtiLink++) { // Explore all the links of this parent

          ParentLep = DiscEngData->Cpu[Parent.Node.SocId].LinkData[KtiLink];
          if (ParentLep.Valid != TRUE || ParentLep.DualLink == TRUE || ParentLep.TwoSkt3Link == TRUE) {
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
          if (ShorterPathExists (DiscEngData->CpuTree[Idx1], &Child) == TRUE) {
            continue;
          }

          //
          // If it is 1 Hop away node (i.e immediate neighbor), just add the node
          //
          AddNode = FALSE;
          if (Child.Node.Hop == 1) {
            AddNode = TRUE;
          } else if (Child.Node.Hop == 2) {
            //
            // If it is 2 Hop away node, make sure the template routing indicates this route to be used
            //
            for (Idx3 = 0; Idx3 < 3; Idx3++) {
              if (OemRoute[Idx1][Idx3][0] == Parent.Node.SocId &&
                  OemRoute[Idx1][Idx3][1] == Child.Node.SocId &&
                  OemRoute[Idx1][Idx3][2] == 0xFF) {
                AddNode = TRUE;
                break;
              }
            }
          } else {
            //
            // If it is 3 Hop away node, make sure the template routing indicates this route to be used
            //
            GrandParent = DiscEngData->CpuTree[Idx1][Parent.Node.ParentIndex];
            for (Idx3 = 0; Idx3 < 3; Idx3++) {
              if (OemRoute[Idx1][Idx3][0] == GrandParent.Node.SocId &&
                  OemRoute[Idx1][Idx3][1] == Parent.Node.SocId &&
                  OemRoute[Idx1][Idx3][2] == Child.Node.SocId) {
                AddNode = TRUE;
                break;
              }
            }
          }

          if (AddNode == FALSE) {
            continue;
          }

          if (AddNodeToTopologyTree (DiscEngData->CpuTree[Idx1], &Child) != KTI_SUCCESS) {
            KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n  Adding CPU%u to Topology Discovery Tree of CPU%u failed.",
                                 Child.Node.SocId, Idx1));
            KTI_ASSERT (FALSE, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_5E);
          }
        } // for(KtiLink...)
      }
    }
  }

  return KTI_SUCCESS;

}


/**
  This routine establishes a mapping between intel's ref platform and this platform. Port number is not relevant.

  Ref Platform:

    S0 ----- S1 ----- S4
     |       |        |
     |       |        |
     |       |        |
     |       |        |
     |       |        |
    S2 ----- S3 ----- S5


  OEM Platform:

    S5 ----- S1 ----- S3
     |       |        |
     |       |        |
     |       |        |
     |       |        |
     |       |        |
    S4 ----- S2 ----- S0



  The socket mapping would be:

    Intel   OEM
      S0     S5
      S1     S1
      S2     S4
      S3     S2
      S4     S3
      S5     S0

  @param KtiInternalGlobal - KTIRC internal variables.
  @param DiscEngData - Pointer to socket specific data

  @retval 0     - Success
  @retval Other - Failure

**/
STATIC
KTI_STATUS
Map6SNodes (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_DISC_DATA            *DiscEngData
  )
{
  UINT8               Idx1, KtiLink;
  UINT8               RingNum1, RingNum2;
  UINT8               RefSoc = 0;
  UINT8               TempSoc1, TempSoc2;
  KTI_LINK_DATA       ParentLep;

  //
  // Identify the two nodes that are part of both the rings and map them to S1 & S3 of the ref topology
  //
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (DiscEngData->RingNodePairs[Idx1] != 0xFF) {
      RefSoc = Idx1;
      DiscEngData->SocMap[1] = RefSoc;
      DiscEngData->SocMap[3] = DiscEngData->RingNodePairs[RefSoc];
      break;
    }
  }
  KTI_ASSERT ((Idx1 < MAX_SOCKET), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);

  // Identify RefSoc's two rings present in this platform
  RingNum1 = DiscEngData->RingInfo[RefSoc][1];
  RingNum2 = DiscEngData->RingInfo[RefSoc][2];

  // Identify the other 2 nodes of RefSoc's first ring
  TempSoc1 = 0xFF;
  TempSoc2 = 0xFF;
  for (Idx1 = 0; Idx1 < CPUS_PER_RING; Idx1++) {
    if (DiscEngData->Rings[RingNum1][Idx1] == RefSoc || DiscEngData->Rings[RingNum1][Idx1] == DiscEngData->RingNodePairs[RefSoc]) {
      continue;
    }
    if (TempSoc1 == 0xFF) {
      TempSoc1 = DiscEngData->Rings[RingNum1][Idx1];
    } else {
      TempSoc2 = DiscEngData->Rings[RingNum1][Idx1];
    }
  }
  KTI_ASSERT ((TempSoc1 != 0xFF), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_60);
  KTI_ASSERT ((TempSoc2 != 0xFF), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_61);

  //
  // Make sure TempSoc2 is the diagonally opposite in RefSoc's first ring; otherwise swap out TempSoc1 & TempSoc2
  //
  for (KtiLink = 0; KtiLink < KtiInternalGlobal->KtiPortCnt; KtiLink++) {
    ParentLep = DiscEngData->Cpu[RefSoc].LinkData[KtiLink];
    if (ParentLep.Valid == TRUE) {
      if (ParentLep.PeerSocId == TempSoc2) {
        TempSoc2 = TempSoc1;
        TempSoc1 = (UINT8)ParentLep.PeerSocId;
        break;
      }
    }
  }

  // TempSoc1 and TempSoc2 can be now mapped to S0 and S2 of intel ref platform
  DiscEngData->SocMap[0] = TempSoc1;
  DiscEngData->SocMap[2] = TempSoc2;


  // Identify the other 2 nodes of RefSoc's second ring
  TempSoc1 = 0xFF;
  TempSoc2 = 0xFF;
  for (Idx1 = 0; Idx1 < CPUS_PER_RING; Idx1++) {
    if (DiscEngData->Rings[RingNum2][Idx1] == RefSoc || DiscEngData->Rings[RingNum2][Idx1] == DiscEngData->RingNodePairs[RefSoc]) {
      continue;
    }
    if (TempSoc1 == 0xFF) {
      TempSoc1 = DiscEngData->Rings[RingNum2][Idx1];
    } else {
      TempSoc2 = DiscEngData->Rings[RingNum2][Idx1];
    }
  }
  KTI_ASSERT ((TempSoc1 != 0xFF), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_62);
  KTI_ASSERT ((TempSoc2 != 0xFF), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_63);

  //
  // Make sure TempSoc2 is the diagonally opposite in S0's second ring; otherwise swap out TempSoc1 & TempSoc2
  //
  for (KtiLink = 0; KtiLink < KtiInternalGlobal->KtiPortCnt; KtiLink++) {
    ParentLep = DiscEngData->Cpu[RefSoc].LinkData[KtiLink];
    if (ParentLep.Valid == TRUE) {
      if (ParentLep.PeerSocId == TempSoc2) {
        TempSoc2 = TempSoc1;
        TempSoc1 = (UINT8)ParentLep.PeerSocId;
        break;
      }
    }
  }

  // TempSoc1 and TempSoc2 can be now mapped to S4 and S5 of intel ref platform
  DiscEngData->SocMap[4] = TempSoc1;
  DiscEngData->SocMap[5] = TempSoc2;

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n    Socket Mapping"));
  for (Idx1 = 0; Idx1 < 6; Idx1++) {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n      %u  -  %u ", Idx1, DiscEngData->SocMap[Idx1]));
  }

  return KTI_SUCCESS;
}

STATIC
KTI_STATUS
Map6SRoutes (
  KTI_DISC_DATA            *DiscEngData,
  UINT8                    (*OemRoute)[3][3]
  )
/**

  Uses the following static routing as tamplate and produces the routing for the given platform
  using the socket mapping.

  S0->S2->S3;  S0->S1->S4;  S0->S2->S3->S5
  S1->S3->S2;  S1->S3->S5
  S2->S0->S1;  S2->S3->S5;  S2->S0->S1->S4
  S3->S1->S0;  S3->S1->S4
  S4->S5->S3;  S4->S1->S0;  S4->S5->S3->S2
  S5->S4->S1;  S5->S3->S2;  S5->S4->S1->S0

  @param DiscEngData - Pointer to socket specific data
  @param OemRoute    -

  @retval 0     - Success
  @retval Other - Failure

**/
{
  UINT8         RefRoute[6][3][3] = {
                {{2, 3, 0xFF}, {1, 4, 0xFF}, {2, 3, 5}}, // S0
                {{3, 2, 0xFF}, {3, 5, 0xFF}, {0xFF, 0xFF, 0xFF}}, // S1
                {{0, 1, 0xFF}, {3, 5, 0xFF}, {0, 1, 4}}, // S2
                {{1, 0, 0xFF}, {1, 4, 0xFF}, {0xFF, 0xFF, 0xFF}}, // S3
                {{5, 3, 0xFF}, {1, 0, 0xFF}, {5, 3, 2}}, // S4
                {{4, 1, 0xFF}, {3, 2, 0xFF}, {4, 1, 0}}  // S5
             };
  UINT8         Idx1, Idx2, Idx3, OemSoc;

  for (Idx1 = 0; Idx1 < 6; Idx1++) {
    OemSoc = DiscEngData->SocMap[Idx1];
    for (Idx2 = 0; Idx2 < 3; Idx2++) {
      for (Idx3 = 0; Idx3 < 3; Idx3++) {
        if (RefRoute[Idx1][Idx2][Idx3] != 0xFF) {
          OemRoute[OemSoc][Idx2][Idx3] = DiscEngData->SocMap[RefRoute[Idx1][Idx2][Idx3]];
        }
      }
    }
  }

  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (DiscEngData->Cpu[Idx1].Valid == TRUE) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n"));
      for (Idx2 = 0; Idx2 < 3; Idx2++) {
        if (OemRoute[Idx1][Idx2][0] != 0xFF) {
          KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "  S%u -> S%u -> S%u", Idx1, OemRoute[Idx1][Idx2][0], OemRoute[Idx1][Idx2][1]));
          if (OemRoute[Idx1][Idx2][2] != 0xFF) {
            KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, " -> S%u", OemRoute[Idx1][Idx2][2]));
          }
        }
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  To get ThisSoc's peer socket which must be in different clump from ThisSoc - so called cross clump peer.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param ThisSoc           - The SocketId whose cross clump peer is required.

  @retval                  - The cross clump peer socket ID if successful
  @retval                  - 0xFF if failed
**/
STATIC
UINT8
GetCrossClumpPeerSoc (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  UINT8                    ThisSoc
  )
{
  UINT8    Idx1;
  UINT8    TmpSoc;
  UINT8    MasterClump;

  //
  // Master clump means the clump with legacy socket
  //
  MasterClump = KtiInternalGlobal->Topo8S2ClumpsInfo.ClumpWithLegacySocket;
  if (MasterClump >= MAX_CLUMPS_NUMBER_IN_8S_TOPOLOGY) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "  %a () - Fatel Error: Clump index [%X] overflow", __FUNCTION__, MasterClump));
    KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
    return 0xFF;
  }

  for (Idx1 = 0; Idx1 < SOCKETS_NUMBER_IN_EACH_CLUMP; Idx1++) {
    TmpSoc = KtiInternalGlobal->Topo8S2ClumpsInfo.Clumps[MasterClump][Idx1];
    if (TmpSoc >= MAX_SOCKET) {
      KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_CPU_SOCKET_ID);
      return 0xFF;
    }
    if (TmpSoc == ThisSoc) {
      return KtiInternalGlobal->Topo8S2ClumpsInfo.ClumpsPeerList[Idx1];
    }
  }

  //
  // If comes here, it means ThisSoc is not in MasterClump.
  //
  for (Idx1 = 0; Idx1 < SOCKETS_NUMBER_IN_EACH_CLUMP; Idx1++) {
    TmpSoc = KtiInternalGlobal->Topo8S2ClumpsInfo.ClumpsPeerList[Idx1];
    if (TmpSoc >= MAX_SOCKET) {
      KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_CPU_SOCKET_ID);
      return 0xFF;
    }
    if (TmpSoc == ThisSoc) {
      return KtiInternalGlobal->Topo8S2ClumpsInfo.Clumps[MasterClump][Idx1];
    }
  }

  return 0xFF;
}

/**
  Computes the topology tree for 8S 2Clumps topology.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param DiscEngData       - Pointer to socket specific data

  @retval KTI_SUCCESS      - Success
  @retval KTI_FAILURE      - Failure
**/
KTI_STATUS
ComputeTopologyTree8S2Clumps (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_DISC_DATA            *DiscEngData
  )
{
  UINT8               Idx1, Idx2, KtiLink;
  UINT8               HopLevel;
  UINT8               RootSocId, CrossClumpPeer;
  KTI_LINK_DATA       ParentLep;
  TOPOLOGY_TREE_NODE  Parent, Child;
  TOPOLOGY_TREE_NODE  *Tree;

  Child.RawNode    = 0;
  Child.Node.Valid = TRUE;

  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if ((DiscEngData->Cpu[Idx1].Valid == FALSE) || (DiscEngData->Cpu[Idx1].SocType != SOCKET_TYPE_CPU)) {
      //
      // This should not happen at all
      //
      KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
      return KTI_FAILURE;
    }

    RootSocId = Idx1;
    Tree = DiscEngData->CpuTree[RootSocId];

    CrossClumpPeer = GetCrossClumpPeerSoc (KtiInternalGlobal, RootSocId);
    if (CrossClumpPeer == 0xFF) {
      KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_CPU_SOCKET_ID);
      return KTI_FAILURE;
    }

    HopLevel = 0;

    for (Idx2 = 0; Idx2 < MAX_TREE_NODES; Idx2++) {
      if (Tree[Idx2].Node.Valid == FALSE) {
        break;
      }

      if (HopLevel == 2) {
        //
        // 8S 2Clumps topology just support HopLevel 2 at most.
        //
        break;
      }

      if (HopLevel == 1) {
        //
        // If HopLevel == 1, it indicates that the HopLevel=1 sockets are already added to the CpuTree[RootSocId]
        //
        if (Tree[Idx2].Node.SocId != CrossClumpPeer) {
          //
          // Just need to explore the CrossClumpPeer to find the HopLevel=2 sockets
          //
          continue;
        }
      }

      Parent = Tree[Idx2];
      Child.Node.Hop = Parent.Node.Hop + 1; // One level down from parent
      Child.Node.ParentIndex = Idx2; // Remember who is the immediate parent of this child

      HopLevel = (UINT8) (Parent.Node.Hop + 1);  // Indicates the current HopLevel to explore

      for (KtiLink = 0; KtiLink < KtiInternalGlobal->KtiPortCnt; KtiLink++) { // Explore all the links of this parent
        ParentLep = DiscEngData->Cpu[Parent.Node.SocId].LinkData[KtiLink];
        if (ParentLep.Valid != TRUE || ParentLep.DualLink == TRUE || ParentLep.TwoSkt3Link == TRUE) {
          continue;
        }

        //
        // Get the Child Socket Id, Type and port that connects it to the parent from LEP
        //
        Child.Node.SocId      = ParentLep.PeerSocId;
        Child.Node.SocType    = ParentLep.PeerSocType;
        Child.Node.ParentPort = ParentLep.PeerPort;

        //
        // Make sure that this socket is not already found in the tree between hop level 0 and parent's hop level.
        // The rational is that, if we find child socket already at that level, then we have a shorter path to this child.
        //
        if (ShorterPathExists (Tree, &Child) == TRUE) {
          continue;
        }

        if (AddNodeToTopologyTree (Tree, &Child) != KTI_SUCCESS) {
          KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n  Adding CPU%u to Topology Discovery Tree of CPU%u failed.",
                               Child.Node.SocId, Idx1));
          KTI_ASSERT (FALSE, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_5D);
        }
      }  // for (KtiLink...)
    }  // for (Idx2...)
  }  // for (RootSocId...)

  return KTI_SUCCESS;
}
