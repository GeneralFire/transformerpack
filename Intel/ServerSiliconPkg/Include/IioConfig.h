/** @file

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

#ifndef _IIO_CONFIG_H
#define _IIO_CONFIG_H
#include <ConfigBlock/TraceHubConfig.h>

#pragma pack(1) //to align members on byte boundary
/**
The Silicon Policy allows the platform code to publish a set of configuration
information that the RC drivers will use to configure the silicon hardware.
**/
typedef struct {
  UINT8   DfxGen3OverrideMode[MAX_TOTAL_PORTS];
  UINT8   DfxGen3TestCard[MAX_TOTAL_PORTS];
  UINT8   DfxGen3ManualPh2_Precursor[MAX_TOTAL_PORTS];
  UINT8   DfxGen3ManualPh2_Cursor[MAX_TOTAL_PORTS];
  UINT8   DfxGen3ManualPh2_Postcursor[MAX_TOTAL_PORTS];
  UINT8   DfxGen3ManualPh3_Precursor[MAX_TOTAL_PORTS];
  UINT8   DfxGen3ManualPh3_Cursor[MAX_TOTAL_PORTS];
  UINT8   DfxGen3ManualPh3_Postcursor[MAX_TOTAL_PORTS];
  UINT8   DfxGen4OverrideMode[MAX_TOTAL_PORTS];
  UINT8   DfxGen4TestCard[MAX_TOTAL_PORTS];
  UINT8   DfxGen4ManualPh2_Precursor[MAX_TOTAL_PORTS];
  UINT8   DfxGen4ManualPh2_Cursor[MAX_TOTAL_PORTS];
  UINT8   DfxGen4ManualPh2_Postcursor[MAX_TOTAL_PORTS];
  UINT8   DfxGen4ManualPh3_Precursor[MAX_TOTAL_PORTS];
  UINT8   DfxGen4ManualPh3_Cursor[MAX_TOTAL_PORTS];
  UINT8   DfxGen4ManualPh3_Postcursor[MAX_TOTAL_PORTS];
  UINT8   DfxDnTxPreset[MAX_TOTAL_PORTS];
  UINT8   DfxRxPreset[MAX_TOTAL_PORTS];
  UINT8   DfxUpTxPreset[MAX_TOTAL_PORTS];
  UINT8   DfxDnTxPresetGen4[MAX_TOTAL_PORTS];
  UINT8   DfxUpTxPresetGen4[MAX_TOTAL_PORTS];

  UINT8   PciePortClkGateEnable[MAX_TOTAL_PORTS];
  UINT8   ExtendedSync[MAX_TOTAL_PORTS];
  UINT8   PciePortEnable[MAX_TOTAL_PORTS];
  UINT8   PcieMaxPayload[MAX_TOTAL_PORTS];
  UINT8   PcieAspm[MAX_SOCKET][NUMBER_PORTS_PER_SOCKET];   // On Setup
  UINT8   PciePortLinkSpeed[MAX_TOTAL_PORTS];
  UINT8   PciePtm;
  UINT8   PcieHotPlugEnable;
  UINT8   PCIe_LTR;
  UINT8   PcieUnsupportedRequests[MAX_TOTAL_PORTS];
  UINT8   RpCorrectableErrorEsc[MAX_SOCKET];           //on Setup
  UINT8   RpUncorrectableNonFatalErrorEsc[MAX_SOCKET]; //on Setup
  UINT8   RpUncorrectableFatalErrorEsc[MAX_SOCKET];    //on Setup
  UINT8   ComplianceMode[MAX_TOTAL_PORTS];
} IIO_PCIE_CONFIG_DATA;

typedef struct {

/**
==================================================================================================
==================================      VTd Setup Options       ==================================
==================================================================================================
**/

    UINT8                   VTdSupport;
    UINT8                   InterruptRemap;
    UINT8                   CoherencySupport;
    UINT8                   ATS;
    UINT8                   PostedInterrupt;

/**
==================================================================================================
==================================      PCIE Setup Options       ==================================
==================================================================================================
**/
    UINT8   VtdAcsWa;

    // Platform data needs to update these PCI Configuration settings
    UINT8    SLOTEIP[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];          // Electromechanical Interlock Present - Slot Capabilities (D0-10 / F0 / R0xA4 / B17)
    UINT8    SLOTHPCAP[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];        // Slot Hot Plug capable - Slot Capabilities (D0-10 / F0 / R0xA4 / B6)
    UINT8    SLOTHPSUP[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];        // Hot Plug surprise supported - Slot Capabilities (D0-10 / F0 / R0xA4 / B5)
    UINT8    SLOTPIP[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];          // Power Indicator Present - Slot Capabilities (D0-10 / F0 / R0xA4 / B4)
    UINT8    SLOTAIP[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];          // Attention Inductor Present - Slot Capabilities (D0-10 / F0 / R0xA4 / B3)
    UINT8    SLOTMRLSP[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];        // MRL Sensor Present - Slot Capabilities (D0-10 / F0 / R0xA4 / B2)
    UINT8    SLOTPCP[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];          // Power Controller Present - Slot Capabilities (D0-10 / F0 / R0xA4 /B1)
    UINT8    SLOTABP[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];          // Attention Button Present - Slot Capabilities (D0-10 / F0 / R0xA4 / B0)
    UINT8    PcieSSDCapable[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];   // Indicate if Port will PcieSSD capable.

    UINT8    PcieHotPlugOnPort[MAX_SOCKET][NUMBER_PORTS_PER_SOCKET];  // manual override of hotplug for port

    // General PCIE Configuration
    UINT8   PcieSubSystemMode[MAX_SOCKET][MAX_IOU_PER_SOCKET];  //on Setup

    UINT8   CompletionTimeoutGlobal;            // HSD 4165823
    UINT8   CompletionTimeoutGlobalValue;
    UINT8   CompletionTimeout[MAX_SOCKET];  // On Setup
    UINT8   CompletionTimeoutValue[MAX_SOCKET]; // On Setup
    UINT8   CoherentReadPart;
    UINT8   CoherentReadFull;
    UINT8   PcieGlobalAspm;                    // HSD 4165822
    UINT8   StopAndScream;                     // HSD 4166867
    UINT8   SnoopResponseHoldOff;              // HSD 4986531
    //
    // PCIE capability
    //
    UINT8   PcieExtendedTagField;              // HSD 4756805
    UINT8   Pcie10bitTag;
    UINT8   PCIe_AtomicOpReq;                  // HSD 4756805
    UINT8   PcieMaxReadRequestSize;            // HSD 4926642


    // mixc PCIE configuration
    UINT8    PcieLinkDis[MAX_TOTAL_PORTS];          // On Setup
    UINT8    PcieCommonClock[MAX_TOTAL_PORTS];  // On Setup
    UINT8    PcieDState[MAX_TOTAL_PORTS];           // On Setup
    UINT8    PcieL0sLatency[MAX_TOTAL_PORTS];       //On Setup
    UINT8    PcieL1Latency[MAX_TOTAL_PORTS];        //On Setup
    UINT8    MsiEn[MAX_TOTAL_PORTS];                // On Setup
    UINT8    IODC[MAX_TOTAL_PORTS];                 // On Setup
    //
    // VPP Control
    //
    BOOLEAN  VppEnabled[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];        // 00 -- Disable, 01 -- Enable  //no setup option defined- aj
    UINT8    VppPort[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];          // 00 -- Port 0, 01 -- Port 1   //no setup option defined- aj
    UINT8    VppAddress[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];       // 01-07 for SMBUS address of Vpp   //no setup option defined- aj

    //
    // Mux and channel for segment
    //
    UINT8    MuxAddress[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];        // SMBUS address of MUX              //no setup option defined
    UINT8    ChannelID[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];      // 00 -- channel 0, 01 -- channel 1  //no setup option defined

    //
    // PCIE setup options for Link Control2
    //
    UINT8    PciePortLinkMaxWidth[MAX_TOTAL_PORTS]; // On Setup
    UINT8    DeEmphasis[MAX_TOTAL_PORTS];       // On Setup

    //
    // PCIE RAS (Errors)
    //

    UINT8   Serr;
    UINT8   Perr;
    UINT8   IioErrorEn;
    UINT8   LerEn;
    UINT8   WheaPcieErrInjEn;

    //
    // PciePll
    //
    UINT8    PciePllSsc;                        //On Setup

    //
    // PCIE  Link Training Ctrl
    //
    UINT16   DelayBeforePCIeLinkTraining;       //On Setup

/**
==================================================================================================
==================================    Crystal Beach 3 Setup Options    ===========================
==================================================================================================
**/
      UINT8                 Cb3DmaEn[TOTAL_CB3_DEVICES];        // on setup
      UINT8                 Cb3NoSnoopEn[TOTAL_CB3_DEVICES];    // on setup
      UINT8                 DisableTPH;
      UINT8                 PrioritizeTPH;
      UINT8                 CbRelaxedOrdering;
      UINT8                 CbDmaMultiCastEnable;               // MultiCastEnable test enable

      UINT8                 DsaEn[NUM_DSA*MAX_SOCKET];                           // on setup
      UINT8                 IaxEn[NUM_IAX*MAX_SOCKET];                           // on setup
      UINT8                 CpmEn[NUM_CPM*MAX_SOCKET];                           // on setup
      UINT8                 HqmEn[NUM_HQM*MAX_SOCKET];                           // on setup

/**
==================================================================================================
==================================    MISC IOH Setup Options            ==========================
==================================================================================================
**/

    // The following are for hiding each individual device and function
    UINT8   PEXPHIDE[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];  // Hide any of the DMI or PCIE devices - SKT 0,1,2,3; Device 0-10 PRD
    UINT8   PCUF6Hide;                            // Hide Device PCU Device 30, Function 6
    UINT8   EN1K;                                 // Enable/Disable 1K granularity of IO for P2P bridges 0:20:0:98 bit 2
    UINT8   DualCvIoFlow;                         // Dual CV IO Flow
    UINT8   PcieBiosTrainEnable;                  // Used as a work around for A0 PCIe
    UINT8   MultiCastEnable;                      // MultiCastEnable test enable
    UINT8   McastBaseAddrRegion;                  // McastBaseAddrRegion
    UINT8   McastIndexPosition;                   // McastIndexPosition
    UINT8   McastNumGroup;                        // McastNumGroup
    UINT8   MctpEn[MAX_TOTAL_PORTS];              // Enable/Disable MCTP for each Root Port

    UINT8   LegacyVgaSoc;
    UINT8   LegacyVgaStack;

    UINT8   HidePEXPMenu[MAX_TOTAL_PORTS];          // to suppress /display the PCIe port menu

    BOOLEAN PoisonMmioReadEn[MAX_SOCKET][MAX_IIO_STACK]; // on setup

/**
==================================================================================================
==================================    NTB Related Setup Options ==========================
==================================================================================================
**/
  UINT8   NtbPpd[MAX_SOCKET*NUMBER_NTB_PORTS_PER_SOCKET];                   //on setup option
  UINT8   NtbBarSizeOverride[MAX_SOCKET*NUMBER_NTB_PORTS_PER_SOCKET];       //on setup option
  UINT8   NtbSplitBar[MAX_SOCKET*NUMBER_NTB_PORTS_PER_SOCKET];             //on setup option
  UINT8   NtbBarSizeImBar1[MAX_SOCKET*NUMBER_NTB_PORTS_PER_SOCKET];         //on setup option
  UINT8   NtbBarSizeImBar2[MAX_SOCKET*NUMBER_NTB_PORTS_PER_SOCKET];         //on setup option
  UINT8   NtbBarSizeImBar2_0[MAX_SOCKET*NUMBER_NTB_PORTS_PER_SOCKET];          //on setup option
  UINT8   NtbBarSizeImBar2_1[MAX_SOCKET*NUMBER_NTB_PORTS_PER_SOCKET];          //on setup option
  UINT8   NtbBarSizeEmBarSZ1[MAX_SOCKET*NUMBER_NTB_PORTS_PER_SOCKET];         //on setup option
  UINT8   NtbBarSizeEmBarSZ2[MAX_SOCKET*NUMBER_NTB_PORTS_PER_SOCKET];         //on setup option
  UINT8   NtbBarSizeEmBarSZ2_0[MAX_SOCKET*NUMBER_NTB_PORTS_PER_SOCKET];          //on setup option
  UINT8   NtbBarSizeEmBarSZ2_1[MAX_SOCKET*NUMBER_NTB_PORTS_PER_SOCKET];          //on setup option
  UINT8   NtbXlinkCtlOverride[MAX_SOCKET*NUMBER_NTB_PORTS_PER_SOCKET];      //on setup option

  UINT8   NtbLinkBiosTrainEn;                                                   // on setup option
/**
==================================================================================================
==================================    VMD Related Setup Options ==========================
==================================================================================================
**/
  UINT8   VMDEnabled[MAX_SOCKET][MAX_VMD_STACKS_PER_SOCKET];
  UINT8   VMDPortEnable[MAX_SOCKET][NUMBER_PORTS_PER_SOCKET];
  UINT8   VMDHotPlugEnable[MAX_SOCKET][MAX_VMD_STACKS_PER_SOCKET];
  UINT8   VMDCfgBarSz[MAX_SOCKET][MAX_VMD_STACKS_PER_SOCKET];
  UINT8   VMDCfgBarAttr[MAX_SOCKET][MAX_VMD_STACKS_PER_SOCKET];
  UINT8   VMDMemBarSz1[MAX_SOCKET][MAX_VMD_STACKS_PER_SOCKET];
  UINT8   VMDMemBar1Attr[MAX_SOCKET][MAX_VMD_STACKS_PER_SOCKET];
  UINT8   VMDMemBarSz2[MAX_SOCKET][MAX_VMD_STACKS_PER_SOCKET];
  UINT8   VMDMemBar2Attr[MAX_SOCKET][MAX_VMD_STACKS_PER_SOCKET];
  UINT8   VMDPchPortEnable[MAX_SOCKET][MAX_VMD_ROOTPORTS_PER_PCH];
  UINT8   VMDDirectAssign[MAX_SOCKET][MAX_VMD_STACKS_PER_SOCKET];

  /**
  ==================================================================================================
  ==================================    PcieSSD Related Setup Options ==========================
  ==================================================================================================
  **/
  UINT8   PcieAICEnabled[MAX_SOCKET*MAX_STACKS_PER_SOCKET];                         // Indicate if PCIE AIC Device will be connected behind an specific IOUx
  UINT8   PcieAICPortEnable[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];
  UINT8   PcieAICHotPlugEnable[MAX_SOCKET*MAX_STACKS_PER_SOCKET];


/**
==================================================================================================
==================================    Gen3 Related Setup Options ==========================
==================================================================================================
**/

  //PCIE Global Option
  UINT8   NoSnoopRdCfg;                             //on Setup
  UINT8   NoSnoopWrCfg;                             //on Setup
  UINT8   MaxReadCompCombSize;                      //on Setup
  UINT8   ProblematicPort;                          //on Setup
  UINT8   DmiAllocatingFlow;                        //on Setup
  UINT8   PcieAllocatingFlow;                       //on Setup
  UINT8   PcieAcpiHotPlugEnable;                    //on Setup
  UINT8   HaltOnDmiDegraded;                        //on Setup
  UINT8   RxClockWA;
  UINT8   GlobalPme2AckTOCtrl;                      //on Setup

  UINT8   PcieSlotOprom1;                           //On Setup
  UINT8   PcieSlotOprom2;                           //On Setup
  UINT8   PcieSlotOprom3;                           //On Setup
  UINT8   PcieSlotOprom4;                           //On Setup
  UINT8   PcieSlotOprom5;                           //On Setup
  UINT8   PcieSlotOprom6;                           //On Setup
  UINT8   PcieSlotOprom7;                           //On Setup
  UINT8   PcieSlotOprom8;                           //On Setup
  UINT8   PcieSlotItemCtrl;                         //On Setup
  UINT8   PcieRelaxedOrdering;                      //On Setup
  UINT8   PciePhyTestMode;                          //On setup
  UINT8   PcieEnqCmdSupport;                        //On setup
/**
==================================================================================================
==================================    IOAPIC Related Setup Options ==========================
==================================================================================================
**/

  UINT8   DevPresIoApicIio[TOTAL_IIO_STACKS];
/**
==================================================================================================
==================================    Security Related Setup Options ==========================
==================================================================================================
**/
  UINT8   LockChipset;
  UINT8   PeciInTrustControlBit;
  UINT8   ProcessorX2apic;
  UINT8   ProcessorMsrLockControl;
  UINT8   Xppdef;
  UINT8   Pci64BitResourceAllocation;

/**
==================================================================================================
==================================    Iio Dfx Related Setup Options ==========================
==================================================================================================
**/
  UINT8   DFXEnable;                     // On Setup
  UINT8   DfxDisableBiosDone;            // When DFXEnable is on this knob controls writing MSR 151h.
  UINT8   DfxLtssmLogger;                // On Setup
  UINT8   DfxLtssmLoggerStop;            // On Setup
  UINT8   DfxLtssmLoggerSpeed;           // On Setup
  UINT8   DfxLtssmLoggerMask;            // On Setup
  UINT8   DfxJitterLogger;               // On Setup
  UINT8   DfxIioRcFlow;                  // On Setup
  UINT8   DfxIioLinkTraining;            // On Setup
  UINT8   DfxOtcPipeHazardThresh;        // On Setup
  UINT8   DfxBadTransactionTypeWa;       // On Setup
  UINT8   DfxNpkAllocatingFlow;          // On Setup

  UINT32  DfxDevHideOnUncoreStacks[MAX_SOCKET][NUM_DEVHIDE_UNCORE_STACKS][NUM_DEVHIDE_REGS_PER_STACK];
  UINT32  DfxDevHideOnIioStacks[MAX_SOCKET][NUM_DEVHIDE_IIO_STACKS][NUM_DEVHIDE_REGS_PER_STACK];

  UINT8   DfxGen3NTBTestCard[MAX_TOTAL_PORTS];  // On Setup

  UINT8   DfxLinkRetrainEn[MAX_TOTAL_PORTS];      // Enable Link Re-Train if connected at degraded speed or width
  UINT8   InboundConfiguration[MAX_TOTAL_PORTS];  // On Setup
  BOOLEAN SkipPortPersonalityLock;                // On Setup

  BOOLEAN CxlHeaderBypass;                        // On Setup

/**
==================================================================================================
======================    IIO Global Performance Tuner Related Setup Options =====================
==================================================================================================
**/
  UINT8  PerformanceTuningMode;

/**
=================================================================================================
======================    PCI-E Data Link Feature Exchange Enable  ===============================
==================================================================================================
**/
  UINT8   PcieDataLinkFeatureExchangeEnable[MAX_TOTAL_PORTS];    //On Setup

/**
==================================================================================================
======================    IIO Trace Hub struct for setup options =================================
==================================================================================================
**/
  TRACE_HUB_CONFIG    CpuTraceHubConfig[MAX_SOCKET][NUMBER_TRACE_HUB_PER_SOCKET];

  UINT8   SLOTIMP[MAX_TOTAL_PORTS];
  UINT8   SLOTSPLS[MAX_TOTAL_PORTS];
  UINT8   SLOTSPLV[MAX_TOTAL_PORTS];
  UINT16  SLOTPSP[MAX_TOTAL_PORTS];
  UINT8   ConfigIOU[MAX_SOCKET][MAX_IOU_PER_SOCKET];         // 00-x4x4x4x4, 01-x4x4x8NA, 02-x8NAx4x4, 03-x8NAx8NA, 04-x16 (P1p2p3p4)

  UINT8   EOI[MAX_TOTAL_PORTS];
  UINT8   MSIFATEN[MAX_TOTAL_PORTS];
  UINT8   MSINFATEN[MAX_TOTAL_PORTS];
  UINT8   MSICOREN[MAX_TOTAL_PORTS];
  UINT8   ACPIPMEn[MAX_TOTAL_PORTS];
  UINT8   DISL0STx[MAX_TOTAL_PORTS];
  UINT8   P2PRdDis[MAX_TOTAL_PORTS];
  UINT8   DisPMETOAck[MAX_TOTAL_PORTS];
  UINT8   ACPIHP[MAX_TOTAL_PORTS];
  UINT8   ACPIPM[MAX_TOTAL_PORTS];
  UINT8   SRIS[MAX_TOTAL_PORTS];
  UINT8   TXEQ[MAX_TOTAL_PORTS];
  UINT8   SERRE[MAX_TOTAL_PORTS];

  //
  // Sierra Peak (SPK)
  //
  UINT8   SierraPeakMemBufferSize[MAX_SOCKET];        // on setup
  IIO_PCIE_CONFIG_DATA  IioPcieConfig;
} IIO_CONFIG;
#pragma pack()

#endif // _IIO_CONFIG_H
