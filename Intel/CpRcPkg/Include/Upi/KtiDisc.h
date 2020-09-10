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

#ifndef _KTI_DISCOVERY_H_
#define _KTI_DISCOVERY_H_

#include <Upi/KtiSi.h>

#define MAX_TREE_NODES       (MAX_SOCKET + 2) // 2 additional nodes since a node will appear more than once in the tree when it is being constructed
//
// A CPU with 3 links supported will have 1 + 1*3 + 3*2 + 6*2 + 12*2 = 46 nodes maximum in ring tree
//
// For topology that has 4 CPU sockets (each has 4 UPI links - exclude dual link) and 2 companion dies
// (each companion die has 2 UPI link - exclude dual link), the MAX_RING_TREE_NODES is calculated as:
// - 0 hop |--- 1 hop -------|------------------ 2 hops -----------------|--------------3 hops -----------------------|----------- 4 hops --------------------------
//    1 + (1 link * 4 Nodes) + [(1 link * 1 node) + (3 links * 3 nodes)] + [(1 link * 3 Nodes) + (3 links * 7 nodes)] + [(1 link * 6 nodes) + (3 links * 18 nodes)] = 99 nodes
//
#define MAX_RING_TREE_NODES  46
#define MAX_RINGS            6   // Maximum number of rings possible in systems with upto 8 sockets (HyperCube)
#define CPUS_PER_RING        4   // # of CPUs in a CPU ring
#define VN0                  0
#define VN1                  1
#define TX                   0
#define RX                   1
#define CROSSLINK_PORT_ID    3
#define MAX_NUM_HOPS         3   // Max number of hops used in 6S topology tree discovery and performance response encoding field.

//
// These definitions are used to access ChaKtiFanOut array in KTI_CPU_RT structure type.
// For SOCs that do not support snoop fanout CHA interleaving, only element 0 in ChaKtiFanOut
// array will be used to store fanout setting for all CHAs.
//
#define ALL_CHA_DATA         0
#define ODD_CHA_DATA         1

#define  EVEN_NUMBER_PEER_NODE_ID_SPRUCC 0x02  //SPR-UCC 2S Topology: Even numbered Node ID of the 2nd socket cluster#0
#define  ODD_NUMBER_PEER_NODE_ID_SPRUCC  0x03  //SPR-UCC 2S Topology: Odd numbered Node ID of the 2nd socket cluster#1

#pragma pack(1)

typedef struct {
  BOOLEAN        IsSocValid;
  UINT8          ActiveLinkCnt;
  UINT8          PeerSocList;
  UINT8          PeerSocCnt;
  UINT8          DualLinkPeerSocList;
  UINT8          DualLinkCnt;
} SOCKET_TOPOLOGY_INFO;

//
// Generic Data structure to describe Link Exchange Parameter (LEP) info
//
typedef struct {
  UINT32  Valid : 1;              ///< TRUE, if the link is valid (i.e trained successfully for low speed, no validation override that disables it)
  UINT32  PeerSocId : 3;          ///< Socket ID
  UINT32  PeerSocType : 2;        ///< Socket Type
  UINT32  PeerPort : 4;           ///< Port of the peer socket
  UINT32  DualLink : 1;           ///< TRUE, if there is a second link to the same neighbor
  UINT32  TwoSkt3Link : 1;        ///< TRUE, if there is a second and third link to the same neighbor
  UINT32  TwoSkt4Link : 1;        ///< TRUE, if there are 4 links between 2 sockets
  UINT32  DualLinkIndex : 3;      ///< Index of the second link that is connected to the same immediate neighbor
  UINT32  DisallowRouteThru : 1;  ///< TRUE if the link is not allowed to configure as route through traffic
  UINT32  SpokeOfPinwheel : 1;    ///< TRUE if the link is chosen as spoke of pinwheel
  UINT32  Rsvd1 : 14;
} KTI_LINK_DATA;

//
// Generic Data structure to describe socket info
//
typedef struct {
  UINT8         Valid   : 1;
  UINT8         SocId   : 3;  // Socket ID
  UINT8         Vn      : 1;  // VN used by traffic originating at this socket
  UINT8         SocType : 2;  // Socket Type
  UINT8         Rsvd1   : 1;
  KTI_LINK_DATA LinkData[MAX_FW_KTI_PORTS];     // KTI Link data
} KTI_CPU_SOCKET_DATA;

//
// Generic Data structure to describe topology tree
//
typedef union {
  struct {
    UINT32  Valid : 1;
    UINT32  SocId : 3;              ///< Socket ID
    UINT32  SocType : 2;            ///< Socket Type
    UINT32  Rsvd1 : 2;
    UINT32  ParentPort : 4;         ///< Port that connects to the parent
    UINT32  Hop : 4;                ///< Depth of the node from root socket
    UINT32  Rsvd2 : 3;
    UINT32  ParentIndex : 7;        ///< Index of immediate parent
    UINT32  Rsvd3 : 6;
  } Node;
  UINT32  RawNode;
} TOPOLOGY_TREE_NODE;


typedef struct {
  UINT32  IgrVnSel;         // Ingress VN SEL setting. Same setting is used for both ports of this M3KTI
} M3KTI_VN_SEL;

//
// Data structure to capture all the route setting
//
typedef struct {
  UINT32         ChaKtiRt;
  UINT32         ChaKtiRt1;
  UINT8          ChaKtiRtSecondEntryUpdated;
  UINT32         M2PcieKtiRt[MAX_IIO_STACK];
  UINT32         M3KtiRt[MAX_M3KTI];
  UINT32         M3KtiRt1[MAX_M3KTI];
  M3KTI_VN_SEL   M3KtiVnSel[MAX_M3KTI];
  BOOLEAN        M3KtiDisMeshCreditPush[MAX_M3KTI];  // Set to TRUE to disable mesh credit push on M3KTI to prevent sending
                                                     // credit returns through the sideband on the said M3KTI tile. Default is FALSE.
  UINT32         ChaKtiFanOut[2];    // By default (SnoopFanoutChaInterleaveEn = FALSE), Index 0 stores snoop fanout setting for all odd and even CHAs.
                                     // If SnoopFanoutChaInterleaveEn = TRUE, then Index 0 stores Even CHA snoop fanout setting, Index 1 stores Odd CHA setting.
  UINT32         ChaKtiFanOut1[2];   // This is used by SOC that supports 6 UPI links to store fanout setting for Request NodeID 5 - 7.
  UINT32         M3KtiSnpFanOut[MAX_M3KTI][MAX_PORT_IN_M3KTI];
  UINT32         M3KtiSnpFanOut1[MAX_M3KTI][MAX_PORT_IN_M3KTI];
  UINT32         KtiAgentRt[MAX_FW_KTI_PORTS];
} KTI_CPU_RT;

//
// Topology Tree info generated and consumed by Discovery engine.
// The Si specific data (currently only port number) is abstracted out using generic port number
// 0, 1, ... MAX_FW_KTI_PORTS. If a Si family has port numbering that is contiguous, then this abstraction
// has no effect.
//
typedef struct {
  UINT8               TotCpus;  // Total number of CPUs populated
  UINT32              CpuList;  // List of CPUs populated
  UINT8               TotRings;  // Total number of rings present in the topology
  UINT8               RingNodePairs[MAX_SOCKET];  // Socket pair that is common to any two rings
  UINT8               SocMap[MAX_SOCKET]; // Mapping of intel reference system socket id to this system's socket id
  KTI_CPU_SOCKET_DATA Cpu[MAX_SOCKET]; // Si agnostic socket data (Port numbers are generalized)
  UINT8               Rings[MAX_RINGS][CPUS_PER_RING + 1];  //All the rings present in the topology.
                                                            //Note: '+1' needed since Element-0 is repeated in last Element to show a full circle.
  UINT8               RingInfo[MAX_SOCKET][MAX_RINGS];  // Number of rings (Element 0) and the ring numbers (Element 1 onwards) this socket is a part of
  UINT8               LinkLoad[MAX_SOCKET][MAX_FW_KTI_PORTS]; // Number of times this link is used; link number is Si agnostic
  TOPOLOGY_TREE_NODE  CpuTree[MAX_SOCKET][MAX_TREE_NODES];  // Si agnostic topology tree (Port numbers are generalized)
  TOPOLOGY_TREE_NODE  AlternateCpuTree[MAX_SOCKET][MAX_TREE_NODES];  // Alternate path for topology tree (Port numbers are generalized)
} KTI_DISC_DATA;


//
// Topology Tree and Final Routing setting for each populated socket
//
typedef struct {
  KTI_CPU_SOCKET_DATA Cpu[MAX_SOCKET];
  TOPOLOGY_TREE_NODE  SbspMinPathTree[MAX_TREE_NODES];
  TOPOLOGY_TREE_NODE  CpuTree[MAX_SOCKET][MAX_TREE_NODES];
  TOPOLOGY_TREE_NODE  AlternateCpuTree[MAX_SOCKET][MAX_TREE_NODES];  // Alternate path for topology tree (Port numbers are generalized)
  KTI_CPU_RT          Route[MAX_SOCKET];
} KTI_SOCKET_DATA;

#pragma pack()

#endif // _KTI_DISCOVERY_H_
