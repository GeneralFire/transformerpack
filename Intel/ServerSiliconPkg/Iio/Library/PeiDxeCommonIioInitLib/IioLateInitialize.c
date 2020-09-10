/** @file
  Implement IIO initialization protocol to provide IIO initialization functions.
  This interfaces will be called by PciHostBridge driver in the PCI bus enumeration.

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

#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>

#include <IioSetupDefinitions.h>
#include <Library/IioDataHubLib.h>
#include <Library/IioAccessLib.h>
#include <Library/IioInitLib.h>
#include <RcRegs.h>
#include <Chip/Include/CpuPciAccess.h>
#include "IioWorkAround.h"
#include <Library/IioVtdInit.h>
#include <Library/IioIoApicInit.h>
#include <Library/IioDmiLib.h>
#include "IioEarlyInitialize.h"
#include <Library/IioSecurePlatformLib.h>
#include "IioLateInitialize.h"
#include <Library/BaseMemoryLib.h>
#include <Library/IioVmdInit.h>
#include <Library/IioMs2iosfInit.h>
#include <Library/CpuAndRevisionLib.h>
#include <PcieRegs.h>
#include <Library/IioSysInfoLib.h>
#include <Library/IioPcieInit.h>
#include <Library/IioNtbInit.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Guid/IioIpInterface.h>
#include <Library/IioDfxInit.h>
#include <Library/IioMiscFeatureLib.h>
#include <Library/IioCxlInit.h>

#include <LibraryPrivate/IioDebug.h>
#include <Library/CheckpointLib.h>


/**

  Call back routine used to do all IIO chipset initialization

  @param   IioGlobalData - Pointer to IIO_GLOBALS
  @param   IioIndex      - Socket Index

  @retval  None

**/
VOID
IioInit (
  IN  IIO_GLOBALS  *IioGlobalData,
  IN  UINT8        IioIndex
  )
{
  UINT8                                Phase;
  UINT8                                PortIndex;
  UINT8                                MaxPortNumberPerSocket;
  UINT8                                Stack;

  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];

  for (Phase = 1; Phase <= IIOInitPhase3; Phase = Phase << 1) {

    //
    // Warning: If anyone should decide that changing order of silicon functional module calls,
    // make sure you examine all code to verify that the functions you are changing are
    // not order dependent!
    //

    if (Phase == IIOInitPhase1) {
       //
       // 1404610230:IIO Break Points needs to be improved.
       //
       OutputCheckpointSocket (IioIndex, STS_IIO_PCIE_PORT_INIT, Phase, IioIndex);
    }

    //
    // Initialize stack
    //
    for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
      if (!IioIsStackPresent (IioIndex, Stack)) {
        continue;
      }
      IioStackInit (Phase, IioGlobalData, IioIndex, Stack);
    }

    //
    // Then do the DMI and PCIE ports
    //
    for ( PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {

      if (!IioVData_ConfigurePciePort (IioGlobalData, IioIndex, PortIndex)) {  //check whether the PCIe port can be configured
        continue;
      }
      if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort)) {
          IioDmiInit (IioGlobalData, IioIndex, Phase);
      } else {

        PciePortInit (Phase, IioGlobalData, IioIndex, PortIndex);
      }
    } // End of for ( PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++)

  } // Phase for ..

  //
  // Initialize CXL.io ports.
  //
  CxlIoPortInit (IioGlobalData, IioIndex);

  //
  // Initialize FBLP.
  //
  FblpPostTrainInit (IioGlobalData, IioIndex);

  //
  // Initialize IOAPIC if SKX is present
  //
  if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    OutputCheckpointSocket (IioIndex, STS_IIO_IOAPIC_INIT, Phase, IioIndex);
    IioIoApicInit (IioGlobalData, IioIndex);
  } // CPU_SKX
  //
  //1404610230:IIO Break Points needs to be improved.
  //
  OutputCheckpointSocket (IioIndex, STS_IIO_VTD_INIT, Phase, IioIndex);

  //
  // Initialize Vtd Registers
  //
  IioVtdInit (IioGlobalData, IioIndex);


  //
  //1404610230:IIO Break Points needs to be improved.
  //
  OutputCheckpointSocket (IioIndex, STS_IIO_DFX_INIT, Phase, IioIndex);
  IioDfxInit (Phase, IioIndex);

  if (IioGlobalData->SetupData.DfxJitterLogger) {
      ShowRxStats (IioGlobalData);
  }
}


/**

  Call back routine used to do all IIO post PCie port configuration, initialization

  @param IioGlobalData  - Pointer to IIO_GLOBALS
  @param IioIndex       - Socket Index

  @retval None
**/
VOID
IioPostInit (
  IN  IIO_GLOBALS  *IioGlobalData,
  IN  UINT8         IioIndex
  )
{
  IIO_DEVFUNHIDE_TABLE      IioDevFuncHideNonRes[MaxIIO];

  //
  // init the IIO misc. control features (Dev 5, Fun 0)
  //
  IioWorkAround (IioGlobalData, IioIndex, IioPortEnumProgramMISCCTRL);

  IioMiscFeatureInit (IioGlobalData, IioIndex);

  ProgProblematicPortConfig (IioGlobalData, IioIndex);

  //
  //1404749419:PCIE NTB: NTB setup needs to be moved to before bifurcation
  //
  if ((IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) || IsPCIeGen4 ()) {
    //
    //1404610230:IIO Break Points needs to be improved.
    //
    //let's switch to Ntb mode here
    OutputCheckpointSocket (IioIndex, STS_IIO_NTB_INIT, STS_IIO_INIT, IioIndex);
    NtbInit (IioGlobalData, IioIndex);
  }
  //
  // Any Workaround for PCIe PortPostInit phase
  //
  IioWorkAround (IioGlobalData, IioIndex, IioPostInitEnd);

  //
  // Any post PCIe init DMI/RLink settings are done below
  //
  IioDmiPostInit (IioIndex);

  //
  // Any post PCIe init settings are done below
  //
  PciePortPostInit (IioGlobalData, IioIndex);

  //
  // Enable PECI
  //
  //
  //1404610230:IIO Break Points needs to be improved.
  //
  OutputCheckpointSocket (IioIndex, STS_IIO_SECURITY_INIT, STS_IIO_INIT, IioIndex);
  TxtTrustPeci (IioGlobalData, IioIndex);

  //
  // Clean IioDevFuncHideNonRes before use it
  //
  ZeroMem (&(IioDevFuncHideNonRes), sizeof (IIO_DEVFUNHIDE_TABLE) * MaxIIO);
  // Now Hide IIO Devices before Pcie Resource Allocation phase

  IioDevicesHide (IioGlobalData, IioIndex, IioBeforeResources, IioDevFuncHideNonRes);

}


/**

    This function update Subsystem Vendor Id in all Uncore devices

    @param IioGlobalData - Pointer to IIO_GLOBALS

    @retval None

**/
VOID
UpdateSsids (
  IN  IIO_GLOBALS  *IioGlobalData
  )
{
  UINT8            IioIndex;
  UINT8            Stack;
  UINT16           PerMonSvid;
  UINT16           VtSdid;
  UINT16           VtSvid;
  UINT8            Bus;
  UINT8            Device;
  UINT8            Function;
  UINT8            MaxPortNumberPerSocket;
  UINT8            PortIndex;
  UINT8            NtbPortPerPortIndex;
  EFI_STATUS       Status;

  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++){
    if (!IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]){
      continue;
    }

    for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
      if (!(IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex] & (1 << Stack))){
        continue;
      }

      Bus = IioGlobalData->IioVar.IioVData.SocketStackBus[IioIndex][Stack];

      if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
        //
        // 4928464: BIOS needs to update SVID for dev5 Func0/6 to 0x8086
        //
        PerMonSvid = IioPciExpressRead16 (IioIndex, Bus, PMON_DEV_NUM, PMON_FUNC_NUM, PCI_SVID_OFFSET);
        PerMonSvid = 0x8086; // subsystem_vendor_identification_number
        IioPciExpressWrite16 (IioIndex, Bus, PMON_DEV_NUM, PMON_FUNC_NUM, PCI_SVID_OFFSET, PerMonSvid);
      }
      //
      //5371441:SVID/SID registers of IIO & PCH devices are not programmed depending on PcdLockCsrSsidSvidRegister PCD
      //
      if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
        Device = CORE05_DEV_NUM;
        Function = CORE05_FUNC_NUM;
      } else {
        Device = MS2IOSF_DEV_NUM;
        Function = MS2IOSF_DFX_FUNC_NUM;
      }
      if (FeaturePcdGet (PcdLockCsrSsidSvidRegister)) {
        VtSvid = IioPciExpressRead16 (IioIndex, Bus, Device, Function, PCI_SVID_OFFSET);
        VtSvid = 0x8086; // subsystem_vendor_identification_number
        IioPciExpressWrite16 (IioIndex, Bus, Device, Function, PCI_SVID_OFFSET, VtSvid);

        VtSdid = IioPciExpressRead16 (IioIndex, Bus, Device, Function, PCI_SID_OFFSET);
        VtSdid = 0x0; // subsystem_device_identification_number
        IioPciExpressWrite16 (IioIndex, Bus, Device, Function, PCI_SID_OFFSET, VtSvid);
      }
      //
      // End s5371441
      //
    }
    //
    // For 10nm A0 stepping, program the subsystem Device/Vendor ID in the NTB configuration space
    //
    if (IsSiliconWorkaroundEnabled ("S1406760259")) {
      MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];
      for (PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {

        Status = GetNtbPortPerPortIndex (PortIndex, &NtbPortPerPortIndex);
        if (Status != EFI_SUCCESS) {
          continue;
        }

        //
        // If statements to resolve KW issue
        //
        if (IioIndex >= MaxIIO || PortIndex >= NELEMENTS (IioGlobalData->IioVar.IioVData.SocketPortBusNumber[IioIndex])) {
          ASSERT (FALSE);
          continue;
        }
        if (PortIndex >= NELEMENTS (IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo)) {
          ASSERT (FALSE);
          continue;
        }

        if (GetPciePortOwnership (IioIndex, PortIndex) == NTB_PORT_DEF_NTB_NTB)  {

          Bus = IioGlobalData->IioVar.IioVData.SocketPortBusNumber[IioIndex][PortIndex];
          Device = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].NtbDevice;
          Function = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].NtbFunction;

          VtSvid = IioPciExpressRead16 (IioIndex, Bus, Device, Function, PCI_SVID_OFFSET);
          VtSvid = 0x8086; // subsystem_vendor_identification_number
          IioPciExpressWrite16 (IioIndex, Bus, Device, Function, PCI_SVID_OFFSET, VtSvid);

          VtSdid = IioPciExpressRead16 (IioIndex, Bus, Device, Function, PCI_SID_OFFSET);
          VtSdid = 0x0; // subsystem_device_identification_number
          IioPciExpressWrite16 (IioIndex, Bus, Device, Function, PCI_SID_OFFSET, VtSvid);
        }
      }
    }
  }

}

/**

    This routine enable Problematic Port feature

    @param IioGlobalData - Pointer to IIO_GLOBALS

    @retval None

**/
VOID
ProgProblematicPortConfig (
  IN  IIO_GLOBALS                *IioGlobalData,
  IN  UINT8                       IioIndex
  )
{
  UINT8                                   PortIndex;
  UINT16                                  AcsCtrl;
  IIOMISCCTRL_N0_IIO_VTD_STRUCT           IioMiscCtrl;
  UINT8                                   Stack;
  UINT8                                   MaxPortNumberPerSocket;
  EFI_STATUS                              Status;
  PCI_ROOT_BRIDGE_PCI_ADDRESS             PciAddress;

  //
  // If Problematic Port functionality is disabled, do nothing
  //
  if (!IioGlobalData->SetupData.ProblematicPort) {
    return;
  }

  for (Stack =0; Stack < MAX_IIO_STACK; Stack++) {
    if (!(IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex] & (1 << Stack))) {
      continue;
    }

    if (IioGlobalData->SetupData.ProblematicPort == 0x01){
      //
      // If NP_PROBLEMATIC flow, enable Lock Thawing Mode
      //
      IioMiscCtrl.Data = IioReadCpuCsr32 (IioIndex, Stack, IIOMISCCTRL_N0_IIO_VTD_REG);
      IioMiscCtrl.Bits.lock_thaw_mode = 1;// A and B step are the same bit offset
      IioWriteCpuCsr32 (IioIndex, Stack, IIOMISCCTRL_N0_IIO_VTD_REG, IioMiscCtrl.Data);
    }
  }

  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];
  for (PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {
    if (!IioVData_ConfigurePciePort (IioGlobalData, IioIndex, PortIndex)) {
      continue;
    }
    if (!(IsCpuAndRevision (CPU_SPRSP, REV_ALL) && IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPortActive))) { //Skip DMI on SPR
      GetPciePortBusDeviceFunction (IioIndex, PortIndex, &PciAddress);
      if (IioPciExpressRead32 (IioIndex,
                                 PciAddress.Bus,
                                 PciAddress.Device,
                                 PciAddress.Function,
                                 PCI_VENDOR_ID_OFFSET) != 0x8086) {
        continue;
      }
    }

    //
    // 4927900: IIO: NTB setting change for acsctrl and pmcsr.power_state
    //
    // 1. if NTB Conf, skip acsctrl setting before NTB mode set via PPD. Done in NtbInit().
    //
    if (!CheckNtbPortConfSetting (IioIndex, PortIndex)) {
      //
      // Force ACS functionality to disabled
      //
      AcsCtrl = 0x00;
      Status = IioWriteACS ( IioIndex, PortIndex, AcsCtrl);
      ASSERT_EFI_ERROR (Status);
    }

    Peer2peerMemoryConfig (IioGlobalData, IioIndex, PortIndex);
  }

  return;
}


/**

    This function is used as interface to initialize IIO Si features
    after Pcie Link has been trained

    @param IIO Global Data        -  Pointer to IIO_GLOBALS

    @retval Status
    @retval IIO_SUCCESS           - Installed successfully

**/
VOID
IioLatePostLinkTrainingPhase (
  IN  IIO_GLOBALS         *IioGlobalData,
  IN  UINT8               IioIndex
  )
{
  //
  //1404610230:IIO Break Points needs to be improved.
  //
  OutputCheckpointSocket (IioIndex, STS_IIO_LATE_INIT_ENTRY, 0, 0xFF);

  IioInit (IioGlobalData, IioIndex);
  IioPostInit (IioGlobalData, IioIndex);
  //
  //1404610230:IIO Break Points needs to be improved.
  //
  OutputCheckpointSocket (IioIndex, STS_IIO_LATE_INIT_EXIT, 0, 0xFF);
}

/**

    This function is used as interface to initialize IIO Si features
    for DXE/SMM drivers.

    @param IIO Global Data        -  Pointer to IIO_GLOBALS

    @retval Status
    @retval IIO_SUCCESS           - Installed successfully

**/
IIO_STATUS
IioLateInitializeEntry (
  IN IIO_GLOBALS                *IioGlobalData
  )
{
  IIO_STATUS    Status = IIO_SUCCESS;
  UINT8         IioIndex;


  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {
    if (!IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) {
      continue;
    }
    IIO_D_LOG ("IioLateInitialization for Socket = %x Start..\n", IioIndex);

    IioEarlyPostLinkTrainingPhase (IioGlobalData, IioIndex);

    IioLatePostLinkTrainingPhase (IioGlobalData, IioIndex);
  }

  //
  // 5372030: [SKX H0 PO] New BIOS knob to enable WA
  //
  PciePhyTestMode (IioGlobalData);

  //
  //dump link status
  //
  DumpIioPciePortPcieLinkStatus (IioGlobalData);

  //
  // Register Vmd init callback on End-Of-Pei
  //
  IioVmdInit ();

  return Status;
}
