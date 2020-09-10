/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2020 Intel Corporation. <BR>

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

#ifndef _PCIE_COMMON_INIT_LIB_H_
#define _PCIE_COMMON_INIT_LIB_H_

#include <Library/PciePlatformHookLib.h>
#include <Library/UsraAccessApi.h>
#include <IndustryStandard/PciExpress31.h>

#define PCI_EXPRESS_EXTENDED_CAPABILITY_PTM_ID            0x001F
#define PCI_EXPRESS_EXTENDED_CAPABILITY_PTM_VER1          0x1

typedef union {
  struct {
    UINT8                                                 RequesterCapable: 1;
    UINT8                                                 ResponderCapable: 1;
    UINT8                                                 RootCapable     : 1;
    UINT8                                                 EnhancedPtm     : 1;
    UINT8                                                 Reserved        : 4;
    UINT8                                                 LocalClockGranularity;
    UINT16                                                Reserved2;
  } Bits;
  UINT32                                                  Uint32;
} PCI_EXPRESS_EXTENDED_CAPABILITIES_PTM_CAPABILITY;

typedef union {
  struct {
    UINT8                                                 Enable     : 1;
    UINT8                                                 RootSelect : 1;
    UINT8                                                 Reserved   : 6;
    UINT8                                                 EffectiveGranularity;
    UINT16                                                Reserved2;
  } Bits;
  UINT32                                                  Uint32;
} PCI_EXPRESS_EXTENDED_CAPABILITIES_PTM_CONTROL;

typedef struct {
  PCI_EXPRESS_EXTENDED_CAPABILITIES_HEADER                Header;
  PCI_EXPRESS_EXTENDED_CAPABILITIES_PTM_CAPABILITY        Capability;
  PCI_EXPRESS_EXTENDED_CAPABILITIES_PTM_CONTROL           Control;
} PCI_EXPRESS_EXTENDED_CAPABILITIES_PTM;

typedef enum _PCIE_LIB_FEATURE_TYPE {
  PCIE_LIB_FEATURE_INVALID       = 0,
  PCIE_LIB_FEATURE_SKIP          = 1,
  PCIE_LIB_FEATURE_ASPM          = 2,
  PCIE_LIB_FEATURE_CTO           = 3,
  PCIE_LIB_FEATURE_MAXPAYLOAD    = 4,
  PCIE_LIB_FEATURE_COMMONCLOCK   = 5,
  PCIE_LIB_FEATURE_EXTTAGFIELD   = 6,
  PCIE_LIB_FEATURE_ATOMICOP      = 7,
  PCIE_LIB_FEATURE_ARIFORWARD    = 8,
  PCIE_LIB_FEATURE_LTR           = 9,
  PCIE_LIB_FEATURE_MRRS          = 10,
  PCIE_LIB_FEATURE_RELAXORDER    = 11,
  PCIE_LIB_FEATURE_NOSNOOP       = 12,
  PCIE_LIB_FEATURE_EXTSYNC       = 13,
  PCIE_LIB_FEATURE_PTM           = 14,
  PCIE_LIB_FEATURE_10BIT_TAG     = 15,
  // Add more feature here
  PCIE_LIB_FEATURE_MAX
} PCIE_LIB_FEATURE_TYPE;

typedef enum _PCIE_LIB_PORTTYPE {
  PCIE_LIB_DMI  = 0,
  PCIE_LIB_PCIE = 1,
  PCIE_LIB_PORT_TYPE_MAX
} PCIE_LIB_PORTTYPE;

typedef enum {
  PcieLibAspmDisabled = 0,
  PcieLibAspmL0s      = 1,
  PcieLibAspmL1       = 2,
  PcieLibAspmL0sL1    = 3,
  PcieLibAspmAutoConfig,
  PcieLibAspmMax
} PCIE_LIB_ASPM_POLICY;

typedef enum {
  PcieLibCompletionTO_Default  = 0,
  PcieLibCompletionTO_50_100us = 1,
  PcieLibCompletionTO_1_10ms   = 2,
  PcieLibCompletionTO_16_55ms  = 5,
  PcieLibCompletionTO_65_210ms = 6,
  PcieLibCompletionTO_260_900ms= 9,
  PcieLibCompletionTO_1_3P5s   = 10,
  PcieLibCompletionTO_4_13s    = 13,
  PcieLibCompletionTO_17_64s   = 14,
  PcieLibCompletionTO_Disabled
} PCIE_LIB_COMPLETION_TIMEOUT_POLICY;

typedef struct _PCIE_LIB_CTO_INF {
  PCIE_LIB_FEATURE_TYPE Type;
  PCIE_LIB_COMPLETION_TIMEOUT_POLICY Policy;
} PCIE_LIB_CTO_INF;

typedef struct _PCIE_LIB_RELAXORDER_INF {
  PCIE_LIB_FEATURE_TYPE Type;
  UINT8                 Flag;
} PCIE_LIB_RELAXORDER_INF;

typedef struct _PCIE_LIB_EXTSYNC_INF {
  PCIE_LIB_FEATURE_TYPE Type;
  UINT8                 Flag;
} PCIE_LIB_EXTSYNC_INF;

typedef struct _PCIE_LIB_NOSNOOP_INF {
  PCIE_LIB_FEATURE_TYPE Type;
  UINT8                 Flag;
} PCIE_LIB_NOSNOOP_INF;

#define PCIE_LIB_MAXPAYLOAD_AUTO        0
#define PCIE_LIB_MAXPAYLOAD_OVERRIDE    1

typedef struct _PCIE_LIB_MAXPAYLOAD_INF {
  PCIE_LIB_FEATURE_TYPE Type;
  UINT8                 Flag;
  UINT16                MaxPayload;
} PCIE_LIB_MAXPAYLOAD_INF;

typedef struct _PCIE_LIB_ASPM_INF {
  PCIE_LIB_FEATURE_TYPE Type;
  PCIE_LIB_ASPM_POLICY  Policy;
  UINT8                 L0sLatency: 3,
                        L1Latency : 3,
                        Reserved0 : 2;
  UINT8                 L0sAcceptLatency: 3,
                        L1AcceptLatency : 3,
                        Reserved1       : 2;
} PCIE_LIB_ASPM_INF;

typedef struct _PCIE_LIB_COMMONCLOCK_INF {
  PCIE_LIB_FEATURE_TYPE Type;
  UINT8                 Flag;
} PCIE_LIB_COMMONCLOCK_INF;

#define PCIE_LIB_EXTTAGFIELD_5_BIT      0
#define PCIE_LIB_EXTTAGFIELD_8_BIT      1

typedef struct _PCIE_LIB_EXTTAGFIELD_INF {
  PCIE_LIB_FEATURE_TYPE Type;
  UINT8                 Flag;
} PCIE_LIB_EXTTAGFIELD_INF;

typedef struct _PCIE_LIB_10BIT_TAG_INF {
  PCIE_LIB_FEATURE_TYPE Type;
  UINT8                 Flag;
} PCIE_LIB_10BIT_TAG_INF;

#define PCIE_LIB_ATOMICOP_AUTO        2
#define PCIE_LIB_ATOMICOP_ENABLE      1
#define PCIE_LIB_ATOMICOP_DISABLE     0

typedef struct _PCIE_LIB_ATOMICOP_INF {
  PCIE_LIB_FEATURE_TYPE Type;
  UINT8                 Flag;
} PCIE_LIB_ATOMICOP_INF;


#define PCIE_LIB_LTR_AUTO        2
#define PCIE_LIB_LTR_ENABLE      1
#define PCIE_LIB_LTR_DISABLE     0

typedef struct _PCIE_LIB_LTR_INF {
  PCIE_LIB_FEATURE_TYPE Type;
  UINT8                 Flag;
} PCIE_LIB_LTR_INF;

#define PCIE_LIB_MRRS_AUTO        0
#define PCIE_LIB_MRRS_OVERRIDE    1

typedef struct _PCIE_LIB_MRRS_INF {
  PCIE_LIB_FEATURE_TYPE Type;
  UINT8                 Flag;
  UINT16                MaxReadReqSize;
} PCIE_LIB_MRRS_INF;

typedef struct _PCIE_LIB_PTM_INF {
  PCIE_LIB_FEATURE_TYPE Type;
  UINT8                 Flag;
  PCI_EXPRESS_EXTENDED_CAPABILITIES_PTM_CONTROL  UpStreamPtmCtrl;
} PCIE_LIB_PTM_INF;

typedef union _PCIE_LIB_ROOTPORT_FEATURE_NODE {
  struct {
    PCIE_LIB_FEATURE_TYPE     Type;
  } Common;
  PCIE_LIB_ASPM_INF              Aspm;
  PCIE_LIB_CTO_INF               Cto;
  PCIE_LIB_MAXPAYLOAD_INF        MaxPayload;
  PCIE_LIB_COMMONCLOCK_INF       CommonClock;
  PCIE_LIB_EXTTAGFIELD_INF       ExtendedTagField;
  PCIE_LIB_ATOMICOP_INF          AtomicOp;
  PCIE_LIB_LTR_INF               Ltr;
  PCIE_LIB_MRRS_INF              Mrrs;
  PCIE_LIB_RELAXORDER_INF        RelaxedOrder;
  PCIE_LIB_EXTSYNC_INF           ExtendedSync;
  PCIE_LIB_NOSNOOP_INF           NoSnoop;
  PCIE_LIB_PTM_INF               Ptm;
  PCIE_LIB_10BIT_TAG_INF         TenBitTag;
} PCIE_LIB_ROOTPORT_FEATURE_NODE;

typedef struct _PCIE_LIB_ROOT_PORT_INF {
  UINT8                            Valid;
  PCIE_LIB_PORTTYPE                Type;
  UINT8                            Name[12];     // Port Name
  UINT8                            Segment;      // PCI Segment of root port device
  UINT8                            Bus;          // PCI Bus of root port device
  UINT8                            Dev;          // PCI Device of root port device
  UINT8                            Fun;          // PCI Function of root port device
  UINT8                            Attribute;    // Attribute, see PCIE_LIB_ATTR_*
  UINT8                            NestedLevel;  // Number of levels in PCI hierarchy connected to this port
  PCIE_LIB_ROOTPORT_FEATURE_NODE  *FeatureList;  // Point to feature list table, ends with FEATURE_MAX entry
} PCIE_LIB_ROOT_PORT_INF;

#define PCIE_LIB_ATTR_ENDPOINT     0x01          // This bit indicates endpoint device present
#define PCIE_LIB_ATTR_HOTPLUG      0x02          // This bit indicates rootport is hotplug capable

typedef struct _PCIE_LIB_HOST_BRIDGE_INF {
  UINT8                            Revision;
  UINT8                            BusUsedNum;         // Number of used buses
  UINT8                            BusBase;            // Min Bus
  UINT8                            BusLimit;           // Max Bus
  UINT8                            Name[10];           // Name
  UINT8                            Attribute;          // Attribute
  PCIE_LIB_ROOT_PORT_INF          *RootPortList;
  UINT8                            Reserved[8];
} PCIE_LIB_HOST_BRIDGE_INF;

#define PCIE_LIB_HOSTBRIDGE_REVISION_1    1
#define PCIE_LIB_HOSTBRIDGE_REVISION_END  0
#define PCIE_LIB_HOSTBRIDGE_REVISION_SKIP 0xFF

#define PCIE_LIB_ATTR_BUS_SHORTAGE        0x80          // This bit indicates lack of bus numbers in this stack

typedef struct _PCIE_LIB_TOPOLOGY_INF {
  UINT8                            Revision;
  PCIE_LIB_HOST_BRIDGE_INF        *HostBridgeList;
  UINT8                            Reserved[8];
} PCIE_LIB_TOPOLOGY_INF;

#define PCIE_LIB_TOPOLOGY_REVISION_1      1

/**
  Parse top level of PCI topology.

  Platform developer needs to fill out the information of PCIE_LIB_TOPOLOGY_INF
  and its sub-content in HostBridgeList and call PcieInfoTreePaser
  to initialize whole PCI tree.

  @param[in]    TopologyInf    PCI topology.

  @return Standard EFI status is returned.
**/
EFI_STATUS
PcieInfoTreePaser (
  PCIE_LIB_TOPOLOGY_INF         *TopologyInf
  );

#endif // _PCIE_COMMON_INIT_LIB_H_

