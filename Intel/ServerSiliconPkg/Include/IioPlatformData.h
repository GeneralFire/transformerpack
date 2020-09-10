/** @file
  This file provides required platform data structure for IOH.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#ifndef _IIO_PLATFORM_DATA_H_
#define _IIO_PLATFORM_DATA_H_

#include <ReferenceCodeDataTypes.h>
#include <SysRegs.h>
#include <Upi/KtiSi.h>
#include <IioRegs.h>
#include <IioConfig.h>

#define IIO_HANDLE          VOID *
#define IIO_STATUS          UINT32

typedef struct {
  UINT8   Register;
  UINT8   Function;
  UINT8   Device;
  UINT8   Bus;
  UINT32  ExtendedRegister;
} PCI_ROOT_BRIDGE_PCI_ADDRESS;

typedef enum {
  DmiTypeVc0,
  DmiTypeVc1,
  DmiTypeVcm,
  MaxDmiVcType
} DMI_VC_TYPE;

typedef enum {
  DmiTypeTc0,
  DmiTypeTc1,
  DmiTypeTc2,
  DmiTypeTc3,
  DmiTypeTc4,
  DmiTypeTc5,
  DmiTypeTc6,
  DmiTypeTc7,
  MaxDmiTcType
}DMI_TC_TYPE;

typedef enum {
  IIOInitPhase1 = 1,
  IIOInitPhase2 = 2,
  IIOInitPhase3 = 4,
} IIO_INIT_PHASE;

typedef enum {
  IioBeforeBifurcation,            // Point before IOU Bi-fucuation and link training, no generic inbound access at this point
  IioAfterBifurcation,             // Point immediately after IOU bifurcation and link training but before any PCIe root port initialization
  IioPortEnumeration,              // Point before Port initialization, no generic inbound access at this point
  IioPortEnumProgramMISCCTRL,      // Inside IioPortInit.PcieSlotInit
  IioEndOfPei,
  IioEnumEnd,
  IioVtDPreEn,
  IioVtDInit,
  IioVtDEn,                        // At this point it has been decided to enable VtD through setup IioVtdInit.VtdInitialization
  IioPostInitEnd,                  // this is the last stage of IIO PCIe port init
  IioBeforeResources,              // At this point IIO Ports configuration has been completed
  IioAfterResources,               // At this point PCIe Resources allocation has been completed
  IioReadyToBoot
} IIO_INIT_ENUMERATION;


extern CONST CHAR8* IioPortLabel[];

#define IIO_PORT_LABEL(x)   ( ((x) < NUMBER_PORTS_PER_SOCKET) ? (IioPortLabel[(x)]) : IioPortLabel[NUMBER_PORTS_PER_SOCKET] )

#pragma pack(1)

typedef union {
  struct {
   UINT32  Value;
   UINT32  ValueHigh;
  } Address32bit;
  UINT64 Address64bit;
} IIO_PTR_ADDRESS;

typedef union {
  UINT64   QWord;
  struct {
    UINT32 DWordL;
    UINT32 DWordH;
  } Bits;
} IIO_QWORD;

typedef struct {
  UINT32 Device;
  UINT32 Function;
  UINT32 RegOffset;
  UINT32 AndMask;
  UINT32 OrMask;
} PCI_OP_STRUCT;

typedef struct {
  UINT32 Instance;
  UINT32 RegOffset;
  UINT32 AndMask;
  UINT32 OrMask;
} CSR_ACCESS_OP_STRUCT;

//
// 4986112:ESCALATE from hexaii_hsx_mock:C0 BIOS issues for isoc/ME
//
typedef struct {
  UINT8  Isoc;
  UINT32 meRequestedSize;
  UINT32 ieRequestedSize;
//
// 4926908: SKX not support Vcp
//
  UINT8  Vc1_pri_en;
//
// 4926908: SKX not support Vcp
//
  UINT8 Isoc_Enable;
} ISOC_VC_TABLE_STRUCT;

/*
 * Following are the data structure defined to support multiple CBDMA types on a system
 */

typedef struct {
  UINT32  NoSnoopSupported : 1;
  UINT32  RelaxOrderSupported : 1;
} CB_CONFIG_CAPABILITY;

typedef struct {
  UINT8                   CB_VER;
  UINT8                   BusNo;
  UINT8                   DevNo;
  UINT8                   FunNo;
  UINT8                   MaxNoChannels;
  CB_CONFIG_CAPABILITY    CBConfigCap;
} CBDMA_CONTROLLER;

typedef struct {
  CBDMA_CONTROLLER     CbDmaDevice;
} DMA_HOST;

// <<<< end of CBDMA data structures >>>>

typedef union {
struct {
    UINT32 Dev0 : 1;
    UINT32 Dev1 : 1;
    UINT32 Dev2 : 1;
    UINT32 Dev3 : 1;
    UINT32 Dev4 : 1;
    UINT32 Dev5 : 1;
    UINT32 Dev6 : 1;
    UINT32 Dev7 : 1;
    UINT32 Dev8 : 1;
    UINT32 Dev9 : 1;
    UINT32 Dev10 : 1;
    UINT32 Dev11 : 1;
    UINT32 Dev12 : 1;
    UINT32 Dev13 : 1;
    UINT32 Dev14 : 1;
    UINT32 Dev15 : 1;
    UINT32 Dev16 : 1;
    UINT32 Dev17 : 1;
    UINT32 Dev18 : 1;
    UINT32 Dev19 : 1;
    UINT32 Dev20 : 1;
    UINT32 Dev21 : 1;
    UINT32 Dev22 : 1;
    UINT32 Dev23 : 1;
    UINT32 Dev24 : 1;
    UINT32 Dev25 : 1;
    UINT32 Dev26 : 1;
    UINT32 Dev27 : 1;
    UINT32 Dev28 : 1;
    UINT32 Dev29 : 1;
    UINT32 Dev30 : 1;
    UINT32 Dev31 : 1;
  } Bits;
  UINT32 Data;
} DEVHIDE_FIELD;

typedef struct {
  UINT32 DevToHide[NUM_DEVHIDE_REGS_PER_STACK];
} IIO_DEVFUNHIDE;

typedef struct {
  UINT32 DevHideRegister;
  UINT32 DevHideBit;
} IIO_PORT_HIDE_REGISTER_BIT;


typedef struct {
  IIO_DEVFUNHIDE IioStackDevHide[12]; // Max IIO stack for SPR / ICX / SKX
  IIO_DEVFUNHIDE UncoreStackDevHide[MAX_UNCORE_STACK];
} IIO_DEVFUNHIDE_TABLE;

typedef struct {
  UINT8   LinkWidth;
  UINT8   LinkSpeed;
} IIO_DMI_DATA;

typedef struct {
    UINT8       SystemRasType;
    BOOLEAN     IsocEnable;
    UINT8       EVMode;
    UINT32      meRequestedSize;
    UINT32      ieRequestedSize;
    UINT8       DmiVc[MaxDmiVcType];
    UINT8       DmiVcId[MaxDmiVcType];
    UINT8       DmiTc[MaxDmiTcType];
    UINT8       PlatformType;
    UINT8       IOxAPICCallbackBootEvent;
    UINT8       RasOperation;
    UINT8       SocketUnderOnline;
    UINT8       CompletedReadyToBootEventServices;
    UINT8       SocketPresent[MaxIIO];
    UINT8       SocketBaseBusNumber[MaxIIO];
    UINT8       SocketLimitBusNumber[MaxIIO];
    UINT32      StackPresentBitmap[MaxIIO];
    UINT64_STRUCT    SegMmcfgBase[MaxIIO];
    UINT8       SegmentSocket[MaxIIO];
    UINT8       SocketStackPersonality[MaxIIO][MAX_IIO_STACK];
    UINT8       SocketStackBus[MaxIIO][MAX_IIO_STACK];
    UINT8       SocketStackBaseBusNumber[MaxIIO][MAX_IIO_STACK];
    UINT8       SocketStackLimitBusNumber[MaxIIO][MAX_IIO_STACK];
    UINT32      SocketStackMmiolBase[MaxIIO][MAX_IIO_STACK];
    UINT32      SocketStackMmiolLimit[MaxIIO][MAX_IIO_STACK];
    UINT8       SocketPortBusNumber[MaxIIO][NUMBER_PORTS_PER_SOCKET];
    UINT8       StackPerPort[MaxIIO][NUMBER_PORTS_PER_SOCKET];
    UINT8       SocketUncoreBusNumber[MaxIIO][MAX_UNCORE_STACK]; // 10nm only
    UINT32      PchIoApicBase;
    UINT32      PciResourceMem32Base[MaxIIO];
    UINT32      PciResourceMem32Limit[MaxIIO];
    UINT8       Pci64BitResourceAllocation;
    UINT32      StackPciResourceMem32Limit[MaxIIO][MAX_IIO_STACK];
    UINT32      VtdBarAddress[MaxIIO][MAX_IIO_STACK];
    UINT32      IoApicBase[MaxIIO][MAX_IIO_STACK];
    UINT32      RcBaseAddress;
    UINT64      PciExpressBase;
    UINT32      PmBase[MaxIIO];
    UINT32      PchSegRegBaseAddress[MaxIIO];
    UINT8       PcieRiser1Type;
    UINT8       PcieRiser2Type;
    UINT8       DmiVc1;
    UINT8       DmiVcm;
    UINT8       Emulation;
    UINT8       SkuPersonality[MAX_SOCKET];
    UINT8       StackPersonality[MaxIIO][MAX_IIO_STACK];
    UINT8       StackId[MaxIIO][MAX_IIO_STACK];
    UINT8       VMDStackEnable[MaxIIO][MAX_IIO_STACK];
    UINT8       IODC;
    UINT8       MultiPch;
    UINT8       FpgaActive[MaxIIO];
    UINT32      TraceHubMemBase;
    UINT8       DmiSocketMap;
    IIO_DMI_DATA DmiSocketData[MAX_SOCKET];
} IIO_V_DATA;

typedef struct {
    UINT8       Device;
    UINT8       Function;
} IIO_PORT_INFO;

typedef struct {
    UINT8                     Valid;
    UINT8                     IioUplinkPortIndex;   //defines platform specific uplink port index (if any else FF)
    IIO_PORT_INFO             UplinkPortInfo;
} IIO_UPLINK_PORT_INFO;

typedef struct _INTEL_IIO_PORT_INFO {
    UINT8   Device;
    UINT8   Function;
    UINT8   DfxDevice;
    UINT8   DfxFunction;
    UINT8   DfxClusterDevice;
    UINT8   DfxClusterFunction;
    UINT8   DfxReutLinkSel;
    UINT8   SuperClusterPort;
    UINT8   NtbDevice;
    UINT8   NtbFunction;
} INTEL_IIO_PORT_INFO;

typedef struct _INTEL_DMI_PCIE_INFO {
    INTEL_IIO_PORT_INFO         PortInfo[NUMBER_PORTS_PER_SOCKET];
} INTEL_DMI_PCIE_INFO;

typedef struct _INTEL_IIO_PRELINK_DATA {
    INTEL_DMI_PCIE_INFO         PcieInfo;
    IIO_UPLINK_PORT_INFO        UplinkInfo[MaxIIO];
} INTEL_IIO_PRELINK_DATA;

typedef struct {
  UINT8       PciePortPresent[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  UINT8       PciePortConfig[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  UINT8       PciePortOwnership[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  UINT8       CurrentPXPMap[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  UINT8       MaxPXPMap[MaxIIO * NUMBER_PORTS_PER_SOCKET];          // Max link width
  UINT8       LaneReversedPXPMap[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  UINT8       PciePortMaxWidth[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  UINT8       PciePortNegWidth[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  UINT8       PciePortNegSpeed[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  UINT8       RetimerConnectCount[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  IIO_PTR_ADDRESS   PtrAddress;
  IIO_PTR_ADDRESS   PtrPcieTopology;
  UINT64      McastRsvdMemory;
  DMA_HOST    DMAhost[MaxIIO];
  UINT8       resetRequired;
  UINT8       MaxPciePortNumberPerSocket[MaxIIO];
  UINT8       MemResetRequired;
} IIO_OUT_DATA;

typedef struct {
  IIO_V_DATA   IioVData;
  INTEL_IIO_PRELINK_DATA PreLinkData;
  IIO_OUT_DATA IioOutData;
} IIO_VAR;

typedef struct {
  IIO_CONFIG   SetupData;
  IIO_VAR      IioVar;
} IIO_GLOBALS;

#pragma pack()

#endif //_IIO_PLATFORM_DATA_H_
