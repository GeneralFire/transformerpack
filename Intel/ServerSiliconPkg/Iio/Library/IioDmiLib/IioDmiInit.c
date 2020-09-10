/** @file
  This file initialize the DMI device

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include <IndustryStandard/PciExpress21.h>

#include <Library/DebugLib.h>
#include <Library/CpuAndRevisionLib.h>

#include <IioPlatformData.h>
#include <IioSetupDefinitions.h>

#include <Library/OemIioInit.h>
#include <Library/IioAccessLib.h>
#include <Library/IioDataHubLib.h>
#include <Library/IioDmiLib.h>
#include <Library/IioSysInfoLib.h>
#include <Library/IioMs2iosfInit.h>

#include <Private/Library/PeiPchDmiLib.h>

#include <LibraryPrivate/IioDebug.h>
#include <LibraryPrivate/IioDmiConf.h>

#include <Private/Library/PeiPchDmiLib.h>


/**
 Initialize DMI IIOInitPhase1 before re-train.

 @param[in] IioGlobalData  Pointer global IIO data structure
 @param[in] IioIndex       Socket where DMI is located
 @param[in] PortIndex      Port where DMI is located
 **/
VOID
DmiPreInit (
  IN  IIO_GLOBALS             *IioGlobalData,
  IN  UINT8                    IioIndex,
  IN  UINT8                    PortIndex
  )
{
  //
  // Disable interrupts on legacy Host bridge (Legacy DMI).
  //
  IioDmiProgramCmdRegister (IioIndex, PortIndex);

  //
  // Write back read value to lock RW-O INTPIN for legacy socket DMI
  //
  LockIntPinRegister (IioIndex, PortIndex);

  //
  // Configure Max Payload Size capability for DMI hierarchy (PCH rootports).
  //
  if (IioGlobalData->SetupData.IioPcieConfig.PcieMaxPayload[PORT_0_INDEX] != PCIE_MAX_PAYLOAD_AUTO) {

    IioDmiSetMpss (IioIndex, PortIndex, IioGlobalData->SetupData.IioPcieConfig.PcieMaxPayload[PORT_0_INDEX]);
  }
  //
  // SA BSU 0.8.11
  // DevCap2.ari default is enabled for DMI port and it should be disabled
  // This is a permanent workaround
  //
  IioDmiCleanUpDevCap2AriBit (IioIndex, PortIndex);

  //
  // Program MiscCtrlStat registers
  //
  IioDmiMiscCtrlStsInit (IioGlobalData, IioIndex, PortIndex);
}


/**
  DMI initialization after bus numbers are assigned
  but other DMI resources are not assigned.

  @param[in] IioGlobalData  Pointer to the Universal Data Storage (UDS) for this driver
  @param[in] IioIndex       Socket where DMI is located
  @param[in] PortIndex      Port where DMI is located
**/
VOID
DmiDeviceInit (
  IN  IIO_GLOBALS             *IioGlobalData,
  IN  UINT8                    IioIndex,
  IN  UINT8                    PortIndex
  )
{
#ifndef IA32
  ASSERT(FALSE);
#else
  UINT8  CpuMpss;
  UINT8  PchMpss;
  UINT8  Mps;

  //
  // Program the Device Control features.
  // Read MPSS from PCH DMI and compare with CPU side. Use the smaller value.
  //
  CpuMpss = IioDmiIpGetMaxPayloadSizeSupported ();
  PchMpss = PchDmiGetMaxPayloadSizeSupported ();
  Mps = MIN (CpuMpss, PchMpss);
  IioDmiProgramDevCtrlReg (IioIndex, PortIndex,
                           IioGlobalData->SetupData.IioPcieConfig.PcieUnsupportedRequests[PORT_0_INDEX], Mps);
  IioMs2iosfUpdateDmiMps (IioGlobalData, IioIndex, Mps);
  //
  // Set D0 or D3Hot state based on setup option
  //
  if (!HasRlink () && IioGlobalData->SetupData.PcieDState[PORT_0_INDEX] == 1) {
    //
    // Program D3_Hot
    // Not supported by RLINK
    //
    IioDmiProgramPmCsr (IioIndex, PortIndex, 3);    // bit1:0 = 11: D3_hot
  } else {
    //
    // Program D0
    //
    IioDmiProgramPmCsr (IioIndex, PortIndex, 0);    // bit1:0 = 00: D0
  }

  //
  // Program the DEVCTRL2 Completion Timeout features
  //
  if (IioGlobalData->SetupData.CompletionTimeoutGlobal) {
    IioDmiProgramDevCtr2lReg (IioIndex, PortIndex, 0, IioGlobalData->SetupData.CompletionTimeoutGlobalValue);
  } else {
    if (IioGlobalData->SetupData.CompletionTimeout[PORT_0_INDEX]) {
      IioDmiProgramDevCtr2lReg (IioIndex, PortIndex, 0, IioGlobalData->SetupData.CompletionTimeoutValue[PORT_0_INDEX]);
    } else {
      IioDmiProgramDevCtr2lReg (IioIndex, PortIndex, 1, 0);
    }
  }
#endif
}

/**
  DMI initialization all miscellaneous CSRs

  @param[in]  IioGlobalData  Pointer to the Universal Data Storage (UDS) for this driver
  @param[in]  IioIndex       Socket/IIO complex index
  @param[in]  PortIndex      Index of the PCIE port
**/

VOID
DmiMiscInit (
  IN  IIO_GLOBALS             *IioGlobalData,
  IN  UINT8                    IioIndex,
  IN  UINT8                    PortIndex
  )
{
  if (!HasRlink ()) {
    //
    // Enable MSI is necessary
    //
    IioDmiEnableMsi (IioIndex, PortIndex, IioGlobalData->SetupData.MsiEn[PORT_0_INDEX]);

    //
    // 4986124: HSX Clone: [C0 PO] Isoc: Vcm appears to be the only traffic getting priority at the HA
    // open bits definition to setup Vc1 correctly and get priority on the ring
    //
    // 4927708: IIO: vc1m_nosnoopopdis changed to vc1m_ns_disable
    // PCIEIPERFCTRLSTS.vc1m_nosnoopopdis has been removed for SKX and replaced by TSWCTL5.vc1m_ns_disable
    IioDmiClearVc1mNoSnoopopis (IioIndex, PortIndex);

    IioDmiProgramPerfCtrlSts (IioGlobalData, IioIndex, PortIndex);
  }

  //
  // Program Root Port Error Escalation enable/disable
  //
  IioDmiProgramRootPortCmdReg (
    IioIndex, PortIndex,
    IioGlobalData->SetupData.IioPcieConfig.RpCorrectableErrorEsc[IioIndex],
    IioGlobalData->SetupData.IioPcieConfig.RpUncorrectableNonFatalErrorEsc[IioIndex],
    IioGlobalData->SetupData.IioPcieConfig.RpUncorrectableFatalErrorEsc[IioIndex]);
}

/**
  Initialization of all DMI resources (bus and other resources) are assigned.

  @param[in]  IioGlobalData  Pointer to the Universal Data Storage (UDS) for this driver
  @param[in]  IioIndex       Socket/IIO complex index
  @param[in]  PortIndex      Index of the PCIE port
**/
VOID
DmiResourceAssignedInit (
  IN  IIO_GLOBALS             *IioGlobalData,
  IN  UINT8                    IioIndex,
  IN  UINT8                    PortIndex
  )
{
  //
  // Program the DMIRCBAR
  //
  if (IioGlobalData->IioVar.IioVData.RcBaseAddress) {
    IioDmiEnableDmiRcBar (IioIndex, PortIndex, IioGlobalData->IioVar.IioVData.RcBaseAddress);
  }

  //
  // Handle programming of RWO and RWOG registers for IIO Port functions
  //
  IioDmiLockRegisters (IioIndex, PortIndex);

  //
  // DMI masks should not be set, so clear them out
  //
  IioDmiClearErorMasks (IioIndex, PortIndex);
}

/**
  Main DMI initialization

  @param[in]  IioGlobalData   Pointer to the IIO global data
  @param[in]  IioIndex        Socket/IIO complex index
  @param[in]  Phase           Defines the phase of PCI initialization that the PCI driver is executing
 */
VOID
IioDmiInit (
  IN IIO_GLOBALS              *IioGlobalData,
  IN UINT8                     IioIndex,
  IN IIO_INIT_PHASE            Phase
  )
{
  IIO_D_DMIDBG ("DMI Init Phase%d \n", Phase);

  switch (Phase) {
    case IIOInitPhase1:
      DmiPreInit (IioGlobalData, IioIndex, PORT_0_INDEX);
      if (!HasRlink ()) {
        OemDmiPreInit (IioGlobalData, IioIndex, PORT_0_INDEX);
      }
      break;

    case IIOInitPhase2:
      DmiDeviceInit (IioGlobalData, IioIndex, PORT_0_INDEX);
      if (!HasRlink ()) {
        OemDmiDeviceInit (IioGlobalData, IioIndex, PORT_0_INDEX);
      }
      break;

    case IIOInitPhase3:
      DmiMiscInit (IioGlobalData, IioIndex, PORT_0_INDEX);
      DmiResourceAssignedInit (IioGlobalData, IioIndex, PORT_0_INDEX);
      if (HasRlink ()) {
        if (IioGlobalData->SetupData.SERRE [PORT_0_INDEX]) {
          IioDmiSerreInit (IioIndex, PORT_0_INDEX);
        }
      }

      if (!HasRlink ()) {
        OemDmiResourceAssignedInit (IioGlobalData, IioIndex, PORT_0_INDEX);
      }
      break;

    default:
      ASSERT (FALSE);
      break;
  }
  IIO_D_DMIDBG ("DMI Init Phase%d done.\n", Phase);
}

