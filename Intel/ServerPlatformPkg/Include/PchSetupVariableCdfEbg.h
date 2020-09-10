/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#ifndef _SETUP_VARIABLE_CDF_H_
#define _SETUP_VARIABLE_CDF_H_

#define PCH_SETUP_NAME L"PchSetup"

#include <PchLimits.h>

#define SATA_MODE_AHCI                    0
#define SATA_MODE_RAID                    1
#define SATA_TEST_MODE_ENABLE             1

#pragma pack(1)
/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:  - Initial version.
**/

/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:  - Initial version.
**/

#define PCH_SETUP_REVISION  1

typedef struct {
  UINT8   Revision;

  UINT8   BiosGuard;

  //
  // PCH_DEVICE_ENABLES
  //

  UINT16  RootPortDid;              // not a SETUP item, used by BIOS to pass Root Port DID to SETUP items

  UINT8   DisableDsxAcPresentPulldown;
  UINT8   PchLan;
  UINT8   DciUsb3TypecUfpDbg;
  UINT8   PchEnableDbcObs;
  UINT8   GbeRegionInvalid;
  UINT8   LomLanSupported;
  UINT8   PchWakeOnLan;
  UINT8   SlpLanLowDc;
  UINT8   PchWakeOnWlan;
  UINT8   PchSmbus;
  UINT8   PchEnergyReport;
  UINT8   SlpS0Override;
  UINT8   SlpS0DisQForDebug;
  UINT8   EnableTcoTimer;
  UINT8   PchCrid;
  UINT8   PchCridSupport;
  UINT8   PchRtcMemoryLock;
  UINT8   PchBiosLock;
  UINT8   UnlockGpioPads;

  UINT8   DeepSxMode;
  UINT8   PchWakeOnWlanDeepSx;
  UINT8   LanWakeFromDeepSx;

  UINT32  EnableTimedGpio0;
  UINT32  EnableTimedGpio1;

  UINT32  PmcDbgMsgEn;

  UINT8   Hpet;
  UINT8   PchAllUnLock;

  UINT8   StateAfterG3;
  UINT8   IchPort80Route;
  UINT8   EnhancePort8xhDecoding;
  UINT8   PchSirqMode;
  UINT8   PciePllSsc;
  UINT8   WdtEnable;

  //
  // Usb Config
  //
  UINT8   PchUsbManualMode;

  //
  // Usb Config
  //
  UINT8   PchUsbPortDisable;
  UINT8   PchUsbHsPort[SETUP_MAX_USB2_PORTS];
  UINT8   PchUsbSsPort[SETUP_MAX_USB3_PORTS];
  UINT8   EnableComplianceMode;

  UINT8   PchUsbOverCurrentEnable;
  UINT8   PchXhciOcLock;
  UINT8   PchUsb2SusWellPgEnable;

  UINT8   PchUsbPdoProgramming;

  //
  // IEH Configuration
  //
  UINT8  IehMode;

  //
  // xDCI Config
  //
  UINT8 PchXdciSupport;

  //
  // XHCI Wake On USB
  //
  UINT8   XhciWakeOnUsbEnabled;

  //
  // Sata CONFIG
  //
  UINT8   PchSata[PCH_MAX_SATA_CONTROLLERS];
  //
  // Sata Interface Mode
  // 0 - IDE  1 - RAID  2 - AHCI
  //
  UINT8   SataInterfaceMode[PCH_MAX_SATA_CONTROLLERS];
  UINT8   SataRstInterrupt[PCH_MAX_SATA_CONTROLLERS];
// APTIOV_SERVER_OVERRIDE_START
  UINT8   SataLegacyBootContSel;
// APTIOV_SERVER_OVERRIDE_END
  UINT8   SataSgpioSupport[PCH_MAX_SATA_CONTROLLERS];
  //
  // Sata DevSlp port
  //
  UINT8  SataDevSlpPort[PCH_MAX_SATA_CONTROLLERS];

  //
  // Sata controller 1 setup options
  //
  UINT8   SataPortController1[PCH_MAX_SATA_PORTS];
  UINT8   SataHotPlugController1[PCH_MAX_SATA_PORTS];
  UINT8   SataMechanicalSwController1[PCH_MAX_SATA_PORTS];
  UINT8   SataSpinUpController1[PCH_MAX_SATA_PORTS];
  UINT8   SataExternalController1[PCH_MAX_SATA_PORTS];
  UINT8   SataTypeController1[PCH_MAX_SATA_PORTS];

  UINT8   PxDevSlpController1[PCH_MAX_SATA_PORTS];
  UINT8   EnableDitoConfigController1[PCH_MAX_SATA_PORTS];
  UINT16  DitoValController1[PCH_MAX_SATA_PORTS];
  UINT8   DmValController1[PCH_MAX_SATA_PORTS];
  //
  // Sata controller 2 setup options
  //
  UINT8   SataPortController2[PCH_MAX_SATA_PORTS];
  UINT8   SataHotPlugController2[PCH_MAX_SATA_PORTS];
  UINT8   SataMechanicalSwController2[PCH_MAX_SATA_PORTS];
  UINT8   SataSpinUpController2[PCH_MAX_SATA_PORTS];
  UINT8   SataExternalController2[PCH_MAX_SATA_PORTS];
  UINT8   SataTypeController2[PCH_MAX_SATA_PORTS];

  UINT8   PxDevSlpController2[PCH_MAX_SATA_PORTS];
  UINT8   EnableDitoConfigController2[PCH_MAX_SATA_PORTS];
  UINT16  DitoValController2[PCH_MAX_SATA_PORTS];
  UINT8   DmValController2[PCH_MAX_SATA_PORTS];
  //
  // Sata controller 3 setup options
  //
  UINT8   SataPortController0[PCH_MAX_SATA_PORTS];
  UINT8   SataHotPlugController0[PCH_MAX_SATA_PORTS];
  UINT8   SataMechanicalSwController0[PCH_MAX_SATA_PORTS];
  UINT8   SataSpinUpController0[PCH_MAX_SATA_PORTS];
  UINT8   SataExternalController0[PCH_MAX_SATA_PORTS];
  UINT8   SataTypeController0[PCH_MAX_SATA_PORTS];

  UINT8   PxDevSlpController0[PCH_MAX_SATA_PORTS];
  UINT8   EnableDitoConfigController0[PCH_MAX_SATA_PORTS];
  UINT16  DitoValController0[PCH_MAX_SATA_PORTS];
  UINT8   DmValController0[PCH_MAX_SATA_PORTS];

  UINT8   SataLegacyOrom[PCH_MAX_SATA_CONTROLLERS];
  //
  // Sata raid
  //
  UINT8   SataRaidR0[PCH_MAX_SATA_CONTROLLERS];
  UINT8   SataRaidR1[PCH_MAX_SATA_CONTROLLERS];
  UINT8   SataRaidR10[PCH_MAX_SATA_CONTROLLERS];
  UINT8   SataRaidR5[PCH_MAX_SATA_CONTROLLERS];
  UINT8   SataRaidIrrt[PCH_MAX_SATA_CONTROLLERS];
  UINT8   SataRaidOub[PCH_MAX_SATA_CONTROLLERS];
  UINT8   SataHddlk[PCH_MAX_SATA_CONTROLLERS];
  UINT8   SataLedl[PCH_MAX_SATA_CONTROLLERS];
  UINT8   SataRaidIooe[PCH_MAX_SATA_CONTROLLERS];
  UINT8   SataRaidSrt[PCH_MAX_SATA_CONTROLLERS];
  UINT8   SataRaidLoadEfiDriver[PCH_MAX_SATA_CONTROLLERS];
  UINT8   SataRaidOromDelay[PCH_MAX_SATA_CONTROLLERS];
  UINT8   RaidDeviceId[PCH_MAX_SATA_CONTROLLERS];
  UINT8   SataSalp[PCH_MAX_SATA_CONTROLLERS];
  UINT8   SataSpeed[PCH_MAX_SATA_CONTROLLERS];
  UINT8   SataTestMode[PCH_MAX_SATA_CONTROLLERS];

  UINT8   SataRstOptaneMemory;
  UINT8   SataRstCpuAttachedStorage;

  UINT8   RstPcieRemapEnabled[PCH_MAX_SATA_PORTS];

  // AcpiDebug Setup Options

  //
  // PCI_EXPRESS_CONFIG, MAX 20 root ports
  //
  UINT8   PcieRootPort8xhDecode;
  UINT8   Pcie8xhDecodePortIndex;
  UINT8   PcieRootPortPeerMemoryWriteEnable;
  UINT8   PcieComplianceTestMode;
  UINT8   PcieBifurcation[PCH_MAX_PCIE_CONTROLLERS];
  UINT8   PcieRstPcieStorageRemap[PCH_MAX_RST_PCIE_STORAGE_CR]; ///< @deprecated
  UINT8   PcieRootPortEn[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortAspm[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortSEE[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortCompletionTimeout[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortPMCE[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortHPE[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortAER[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortSpeed[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortTHS[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortL1SubStates[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortACS[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortPTM[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortMVC[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortDPC[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortEDPC[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortEqPh3Method[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortUptp[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortDptp[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieLaneCm[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieLaneCp[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortSI[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortMrls[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortMPS[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortRO[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortNS[PCH_MAX_PCIE_ROOT_PORTS];
  UINT16  PcieDetectTimeoutMs[PCH_MAX_PCIE_ROOT_PORTS];

  UINT8   PchPcieSnoopLatencyOverrideMode[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieSnoopLatencyOverrideMultiplier[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieNonSnoopLatencyOverrideMode[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieNonSnoopLatencyOverrideMultiplier[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieSnoopLatencyOverrideValue[PCH_MAX_PCIE_ROOT_PORTS];
  UINT16  PchPcieNonSnoopLatencyOverrideValue[PCH_MAX_PCIE_ROOT_PORTS];

  UINT8   PcieSwEqOverride;

  UINT8   PcieSwEqCoeffCm[5];
  UINT8   PcieSwEqCoeffCp[5];

  // PCIe equalization
  UINT8   PchPcieEqOverrideDefaultSettings[PCH_MAX_PCIE_ROOT_PORTS];

  UINT8   PchPcieEqPh1DownstreamPortTransmitterPreset[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh1UpstreamPortTransmitterPreset[PCH_MAX_PCIE_ROOT_PORTS];

  UINT8   Ph2LocalTransmitterOverrideEnable[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   LocalTransmitterOverridePrecursorCoeff[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   LocalTransmitterOverrideCursorCoeff[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   LocalTransmitterOverridePostCursorCoeff[PCH_MAX_PCIE_ROOT_PORTS];

  UINT8   PchPcieEqPh3FomHintTableFormatEntry1[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTablePresetEntry1[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTableD1Entry1[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTableD2Entry1[PCH_MAX_PCIE_ROOT_PORTS];

  UINT8   PchPcieEqPh3FomHintTableFormatEntry2[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTablePresetEntry2[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTableD1Entry2[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTableD2Entry2[PCH_MAX_PCIE_ROOT_PORTS];

  UINT8   PchPcieEqPh3FomHintTableFormatEntry3[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTablePresetEntry3[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTableD1Entry3[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTableD2Entry3[PCH_MAX_PCIE_ROOT_PORTS];

  UINT8   PchPcieEqPh3FomHintTableFormatEntry4[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTablePresetEntry4[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTableD1Entry4[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTableD2Entry4[PCH_MAX_PCIE_ROOT_PORTS];

  UINT8   PchPcieEqPh3FomHintTableFormatEntry5[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTablePresetEntry5[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTableD1Entry5[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTableD2Entry5[PCH_MAX_PCIE_ROOT_PORTS];

  UINT8   PchPcieEqPh3FomHintTableFormatEntry6[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTablePresetEntry6[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTableD1Entry6[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTableD2Entry6[PCH_MAX_PCIE_ROOT_PORTS];

  UINT8   PchPcieEqPh3FomHintTableFormatEntry7[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTablePresetEntry7[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTableD1Entry7[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTableD2Entry7[PCH_MAX_PCIE_ROOT_PORTS];

  UINT8   PchPcieEqPh3FomHintTableFormatEntry8[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTablePresetEntry8[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTableD1Entry8[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTableD2Entry8[PCH_MAX_PCIE_ROOT_PORTS];

  UINT8   PchPcieEqPh3FomHintTableFormatEntry9[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTablePresetEntry9[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTableD1Entry9[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTableD2Entry9[PCH_MAX_PCIE_ROOT_PORTS];

  UINT8   PchPcieEqPh3FomHintTableFormatEntry10[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTablePresetEntry10[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTableD1Entry10[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTableD2Entry10[PCH_MAX_PCIE_ROOT_PORTS];

  UINT8   PchPcieEqPh3FomHintTableFormatEntry11[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTablePresetEntry11[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTableD1Entry11[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTableD2Entry11[PCH_MAX_PCIE_ROOT_PORTS];

  UINT8   PchPcieEqPh3FomHintTableFormatEntry12[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTablePresetEntry12[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTableD1Entry12[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieEqPh3FomHintTableD2Entry12[PCH_MAX_PCIE_ROOT_PORTS];


  //
  // PCIe IMR
  //
  UINT8   PcieImrEnable;
  UINT16  PcieImrSize;
  UINT8   ImrRpSelection;
  //
  // PCIe LTR Configuration
  //
  UINT8   PchPcieLtrEnable[PCH_MAX_PCIE_ROOT_PORTS];
  //
  // ADR Configuration
  //
  UINT8   PchAdrEn;
  UINT8   PchAdrGpioSel;
  UINT8   PchAdrTimerEn;
  UINT8   PchAdrReset;
  UINT8   PchAdrTimerVal;
  UINT8   PchAdrMultiplierVal;
  UINT8   PchAdrSrcSel[PCH_MAX_ADR_SOURCE_NUMBER];

  UINT8   PchAdrTimer1Val;
  UINT8   PchAdrMultiplier1Val;
  UINT8   PchAdrTimer2Val;
  UINT8   PchAdrMultiplier2Val;
  UINT8   AdrHostPartitionReset;
  UINT8   AdrSrcOverride;
  //
  // Interrupt Configuration
  //
  UINT8   PchIoApic24119Entries;
  UINT8   Enable8254ClockGating;

  //
  // PCH Thermal Throttling
  //
  UINT8   PchCrossThrottling;

  //
  // TraceHub Setup Options
  //
  UINT8   PchTraceHubMode;
  UINT32  PchTraceHubMemReg0Size;
  UINT32  PchTraceHubMemReg1Size;
  UINT8   AetEnableMode;

  //
  // Thermal Throttling Level options
  //
  UINT8   PchTtLevelSuggestSet;
  UINT16  PchThrmT0Level;
  UINT16  PchThrmT1Level;
  UINT16  PchThrmT2Level;
  UINT8   PchThrmTtEnable;
  UINT8   PchThrmTtState13Enable;
  UINT8   PchThrmTtLock;

  //
  // DMI Setup Options
  //
  //
  // DMI Thermal Throttling
  //
  UINT8   PchDmiTsSuggestSet;
  UINT8   PchTs0Width;
  UINT8   PchTs1Width;
  UINT8   PchTs2Width;
  UINT8   PchTs3Width;
  UINT8   PchDmiTsawEn;

  //
  // SATA Thermal Throttling
  //
  UINT8   PchSataTsSuggestSet[PCH_MAX_SATA_CONTROLLERS];
  UINT8   PchP0T1M[PCH_MAX_SATA_CONTROLLERS];
  UINT8   PchP0T2M[PCH_MAX_SATA_CONTROLLERS];
  UINT8   PchP0T3M[PCH_MAX_SATA_CONTROLLERS];
  UINT8   PchP0TDisp[PCH_MAX_SATA_CONTROLLERS];
  UINT8   PchP0Tinact[PCH_MAX_SATA_CONTROLLERS];
  UINT8   PchP0TDispFinit[PCH_MAX_SATA_CONTROLLERS];
  UINT8   PchP1T1M[PCH_MAX_SATA_CONTROLLERS];
  UINT8   PchP1T2M[PCH_MAX_SATA_CONTROLLERS];
  UINT8   PchP1T3M[PCH_MAX_SATA_CONTROLLERS];
  UINT8   PchP1TDisp[PCH_MAX_SATA_CONTROLLERS];
  UINT8   PchP1Tinact[PCH_MAX_SATA_CONTROLLERS];
  UINT8   PchP1TDispFinit[PCH_MAX_SATA_CONTROLLERS];

  UINT8   MemoryThermalManagement;
  UINT8   ExttsViaTsOnBoard;
  UINT8   ExttsViaTsOnDimm;

  //
  // eSPI Setup Options
  //
  UINT8   PchEspiLgmrEnable;
  UINT8   PchEspiHostC10ReportEnable; // EBG_TODO review addition

  //
  // HD-Audio Configuration
  //
  UINT8  PchHdAudio;
  UINT8  PchHdAudioDsp;
  UINT8  PchHdAudioDspUaaCompliance;
  UINT8  PchHdAudioCodecSxWakeCapability;
  UINT8  PchHdAudioPme;
  UINT8  PchHdaHdAudioLinkFreq;
  UINT8  PchHdaAudioVcType;
  UINT8  TestHdaPowerClockGating;
  UINT8  TestHdaConfigLockdown;
  UINT8  TestHdaLowFreqLinkClockSource;

  //
  // Pch Serial Io configuration
  //
  UINT8   PchSerialIoUart[PCH_MAX_SERIALIO_UART_CONTROLLERS];
  UINT8   PchUartDmaEnable[PCH_MAX_SERIALIO_UART_CONTROLLERS];
  UINT8   PchUartHwFlowCtrl[PCH_MAX_SERIALIO_UART_CONTROLLERS];
  UINT8   PchUartPowerGating[PCH_MAX_SERIALIO_UART_CONTROLLERS];
  UINT8   PchGpio;
  UINT8   PchGpioIrqRoute;
  UINT16  PchGpioDebounce;
  UINT8   PchGpioTestDevices;
  UINT8   PchAdditionalSerialIoDevices;

  UINT8   OsDebugPort;

  // APTIOV_SERVER_OVERRIDE_RC_START : EIP250562: PCH / IIO Resource reservation for Hot-Plug
  #ifdef HOTPLUG_FLAG
    UINT8   PcieExtraBusRsvd[PCH_MAX_PCIE_ROOT_PORTS];
    UINT16  PcieMemRsvd[PCH_MAX_PCIE_ROOT_PORTS];
    UINT8   PcieMemRsvdalig[PCH_MAX_PCIE_ROOT_PORTS];
    UINT16  PciePFMemRsvd[PCH_MAX_PCIE_ROOT_PORTS];
    UINT8   PciePFMemRsvdalig[PCH_MAX_PCIE_ROOT_PORTS];
    UINT16  Pcie64PFMemRsvd[PCH_MAX_PCIE_ROOT_PORTS];
    UINT8  Pcie64PFMemRsvdalig[PCH_MAX_PCIE_ROOT_PORTS];
    UINT8   PcieIoRsvd[PCH_MAX_PCIE_ROOT_PORTS];
  #endif
  // APTIOV_SERVER_OVERRIDE_RC_END : EIP250562: PCH / IIO Resource reservation for Hot-Plug

  //
  // SMBUS
  //
  UINT8   SmbusSpdWriteDisable;
  UINT8   SmbusDefeaturing;
  //
  // P2SB
  //
  UINT8   PchSidebandLock;
  //
  // Flash Protection configuration
  //
  UINT8   FprrEnable;
  //
  // HSIO message
  //
  UINT8   ChipsetInitMessage;
  UINT8   BypassPhySyncReset;
  UINT8   WmphyMargining;

  //
  // PSF
  //
  UINT8   PsfTccEnable;

  //
  // Debug Panel
  //
  UINT8   DciEn;
  UINT8   DciDbcMode;
  UINT8   FirmwareConfiguration;
  //
  // SCS Configuration
  //
  UINT8   PchScsEmmcEnabled;
  UINT8   PchScsEmmcHs400Enabled;
  //
  // DWR
  //
  UINT8   Dwr_Enable;
  UINT8   Dwr_Stall;
  UINT8   Dwr_BmcRootPort;

  //
  // HSUART
  //
  UINT8   PchHsuartMode[3];

  //
  // Rlink
  //
  UINT8   PchRlinkClockGating;
  UINT8   PchDmiAspm;

  UINT8   LowPowerS0Idle;

  UINT8   GlobalResetMasksOverride;
  UINT32  GlobalResetEventMask;
  UINT32  GlobalResetTriggerMask;

  //
  // FIA
  //
  UINT8  FiaMuxSupported;
  UINT8  FiaMuxOverride;
  UINT8  FiaMuxCfgInvalidate;
  UINT8  FiaLaneConfig[24];
  UINT8  HideFiaLane[24];
  UINT8  FiaLaneLinkWidth[24];

} PCH_SETUP;


#pragma pack()

#ifndef PLATFORM_SETUP_VARIABLE_NAME
#define PLATFORM_SETUP_VARIABLE_NAME             L"Setup"
#endif

// APTIOV_SERVER_OVERRIDE_RC_START : To reslove redefintion issue
//#define SETUP_GUID \
// { 0xEC87D643, 0xEBA4, 0x4BB5, { 0xA1, 0xE5, 0x3F, 0x3E, 0x36, 0xB2, 0x0D, 0xA9 } }
// APTIOV_SERVER_OVERRIDE_RC_END : To reslove redefintion issue


#define DEBUG_CONFIG_GUID \
 { 0xDE0A5E74, 0x4E3E, 0x3D96, { 0xA4, 0x40, 0x2C, 0x96, 0xEC, 0xBD, 0x3C, 0x97 } }

extern EFI_GUID  gSetupVariableGuid;
extern EFI_GUID  gPchSetupVariableGuid;
extern EFI_GUID  gDebugConfigVariableGuid;
#endif
