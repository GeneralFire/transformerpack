/** @file
  This is internal header file used by MultiSocketLib. Interfaces in this file are only used in this library.

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

#ifndef _KTI_MULTI_SOCKET_INTERNAL_H_
#define _KTI_MULTI_SOCKET_INTERNAL_H_

#define NORTH_CAP_MS2IOSF       0
#define SOUTH_CAP_MS2IOSF       1

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
  );

KTI_STATUS
ConfigSocketLinkEparams (
  UINT8          Socket,
  UINT8          Link,
  VOID           *LinkSpeedParameter
  );

KTI_STATUS
GatherCommonKtiCapabilities (
  KTI_HOST_INTERNAL_GLOBAL       *KtiInternalGlobal,
  UINT32                         *CrcMode,
  UINT32                         *Vn1Supported,
  UINT32                         *L0pSupport,
  UINT32                         *L1Support,
  UINT32                         *FailoverSupport
  );

KTI_STATUS
SelectSupportedKtiLinkSpeed (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

UINT8
GetMaxSupportedLinkSpeed (
  VOID
  );

KTI_STATUS
CheckLinkSpeedSupported (
  UINT8               RequestedLinkSpeed
  );

KTI_STATUS
ClearOrSetKtiDfxLckForFullSpeedTransition (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  UINT8                     Value
  );

KTI_STATUS
NormalizeLinkOptions (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

KTI_STATUS
ProgramPhyLayer (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  KTI_SOCKET_DATA           *SocketData,
  UINT8                     Group
  );

/**
  The RxDCC value should be overwritten to a 'safe' value to ensure reliable startup of the RxDCC clock when transitioning from SLOW to FAST.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

**/
VOID
SlowModeRxDccOverride (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Routine to setup the config access path from SBSP to the given socket.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param Interm            - Immediate parent of the socket in SBSP minimum tree for which the config
                             access path needs to be set from SBSP
  @param Dest              - socket for which the config access path needs to be set from SBSP
  @param DestPort          - Port of the given socket's immediate parent that connects them.

  @retval                  - KTI_SUCCESS

**/
KTI_STATUS
SetupSbspConfigAccessPath (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  TOPOLOGY_TREE_NODE        *Interm,
  TOPOLOGY_TREE_NODE        *Dest,
  UINT8                     DestPort
  );

/**
  Routine to setup the boot path from Remote CPUs (i.e >1 hop away) to Legacy CPU.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param Interm            - Immediate parent of the "Dest" in SBSP minimum tree
  @param Dest              - Remote CPU socket for which the boot path needs to be set
  @param DestPort          - Port of Remote CPU socket's immediate parent that connects them.

  @retval 0 - Successful
  @retval Other - failure

**/
KTI_STATUS
SetupRemoteCpuBootPath (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  TOPOLOGY_TREE_NODE        *Interm,
  TOPOLOGY_TREE_NODE        *Dest,
  UINT8                     DestPort
  );

/**
  Checks the link in slow mode wa request the warm reset or not

  @param SocketData        - add argument and description to function comment
  @param KtiInternalGlobal - KTIRC internal variables.

**/
VOID
UpiSlowModeWaSkx (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

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
  );

/**
  This routine assigns UPI port to each MS2IOSF source based on affinity.

  It affinitizes M2IOSF sources to UPI port on the same cap, if that option
  exists. That is M2IOSFs on the northcap of the die divide remote P2P traffic
  among UPIs enabled in the northcap and M2IOSFs in the southcap of the dies
  send traffic to UPI on the southcap if that UPI is enabled.

  @param[in,out] SocketData        Pointer to socket specific data
  @param[in] KtiInternalGlobal     KTIRC internal variables.
  @param[in] RootSocId             Root Node node ID
  @param[in] DestSocId             Destination Node node ID
  @param[in] OutPorts              Array that stores KTI ports of Root Node
  @param[in] TotalActiveLinks      Number of enabled UPI links between Root Node and Destination Node
  @param[in] PortFieldWidth        Port field width; this is SoC specific.

  @retval KTI_SUCCESS           M2PcieKtiRt tables are updated with valid routing based on affinity.
  @retval KTI_IGNORE            Affinity assignment is not performed.

**/
KTI_STATUS
AffinitizeM2IosfSourceToUpiPort (
  IN OUT KTI_SOCKET_DATA          *SocketData,
  IN     KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN     UINT8                     RootSocId,
  IN     UINT8                     DestSocId,
  IN     UINT8                     *OutPorts,
  IN     UINT8                     TotalActiveLinks,
  IN     UINT8                     PortFieldWidth
  );

/**
  This routine checks if the requested UPI link requires opposite VN of what was assigned
  to its Root Node.

  This routine is currently used to handle the special VN assignment requirement for
  8-socket 4-Link (Twisted cubes fully connect cubes) topology in SPR. To guarantee
  better QoS for the Route-Through traffic, the source traffic uses different VN as
  the route-thru traffic at a given socket. If not, the source traffic might starve
  the route-thru traffic as the source count (i.e CHA/Core) is increasing gen-to-gen.
  Using the following socket mapping as an example:
      +-----------------------+
      |                       |       VN Assignment:
      S0 ---- S1     S6 ---- S7       ===============
      | \   / | \   / | \   / |       VN0: S0, S2, S4, S6
      |  \ /  |  \ /  |  \ /  |       VN1: S1, S3, S5, S7
      |   \   |   \   |   \   |
      |  / \  |  / \  |  / \  |
      | /   \ | /   \ | /   \ |
      S2 ---- S3     S4 ---- S5
      |                       |
      +-----------------------+
  To meet the VN assignment requirement, the following VN assignment should be used:
    VN0: S0, S2, S4, S6
    VN1: S1, S3, S5, S7
  In addition, UPI Port 2 traffic in the above example should use VN that is opposite
  of the above VN assignment to guarantee that the Source traffic doesn't use same VN
  as the pass-thru traffic.

  @param[in] SocketData            Pointer to socket specific data
  @param[in] KtiInternalGlobal     KTIRC internal variables
  @param[in] SocId                 Socket ID to check for
  @param[in] PortId                UPI port number

  @retval TRUE   This UPI Port should use VN that is opposite of the VN assigned to this Socket
  @retval FALSE  This UPI Port uses the same VN as other ports of this Socket

**/
BOOLEAN
IsUpiPortWithOppositeRootVn (
  IN  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN  KTI_CPU_SOCKET_DATA      *SocketData,
  IN  UINT8                    SocId,
  IN  UINT8                    PortId
  );

/**
  This routine is used in topology (such as 8S double pinwheel) that involves
  multiple ports that can be used to configure the route through traffic.

  Based on topology specific criteria, this routine will update the correct
  UPI port to be used as the outgoing port on route through socket.

  @param[in] KtiInternalGlobal     KTIRC internal variables.
  @param[in] SocketData            Pointer to socket specific data
  @param[in] RouteThruNodeId       Route Through node ID (or intermediate node)
  @param[in] RtPortIn              Incoming UPI port on Route Through Node
  @param[in,out] RtPortOut         Pointer to store outgoing UPI Port on Route Through Node

  @retval KTI_SUCCESS   RtPortOut contains the final outgoing UPI Port to be used for route-through.
  @retval KTI_IGNORE    Customization of outgoing route-through UPI port is NOT required.
  @retval KTI_FAILURE   Route through incoming and/or outgoing ports passed into this function are invalid.

**/
KTI_STATUS
SelectOutportForOptimizedPassThruRoute (
  IN     KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN     KTI_SOCKET_DATA            *SocketData,
  IN     UINT8                      RouteThruNodeId,
  IN     UINT8                      RtPortIn,
  IN OUT UINT8                      *RtPortOut
  );

#endif // _KTI_MULTI_SOCKET_INTERNAL_H_
