/** @file
  Header file for PchPsfPrivateLib.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

#ifndef _PSF_PRIVATE_LIB_H_
#define _PSF_PRIVATE_LIB_H_

#include <Library/PchPcrLib.h>
#include <Register/PchPcrRegs.h>

typedef struct {
  UINT32       Id;
  PCH_SBI_PID  SbPid;
} PSF_SEGMENT;

#define PSF_PORT_NULL ((PSF_PORT){0,0})
#define PSF_IS_PORT_NULL(PsfPort) ((PsfPort.PsfPid == 0) && (PsfPort.RegBase == 0))

//
// Structure for storing data on both PSF SideBand Port ID and
// PSF port register offset for specific device
//
typedef struct {
  PCH_SBI_PID  PsfPid;
  UINT16       RegBase;
} PSF_PORT;

/**
  Disable device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfDisableDevice (
  IN PSF_PORT  PsfPort
  );

/**
  Enable device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfEnableDevice (
  IN PSF_PORT  PsfPort
  );

/**
  Hide PciCfgSpace of device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfHideDevice (
  IN PSF_PORT  PsfPort
  );

/**
  Unhide PciCfgSpace of device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfUnhideDevice (
  IN PSF_PORT  PsfPort
  );

/**
  Disable device BARs at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     PSF PORT data structure
  @param[in] BarDisMask  BIT0-BAR0, BIT1-BAR1,...
                         Mask corresponds to 32bit wide BARs
**/
VOID
PsfDisableDeviceBar (
  IN PSF_PORT  PsfPort,
  IN UINT32    BarDisMask
  );

/**
  Enable device BARs at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     PSF PORT data structure
  @param[in] BarEnMask   BIT0-BAR0, BIT1-BAR1,...
                         Mask corresponds to 32bit wide BARs
**/
VOID
PsfEnableDeviceBar (
  IN PSF_PORT  PsfPort,
  IN UINT32    BarEnMask
  );

/**
  This procedure will set BARx value for TraceHub ACPI device at PSF level

  @param[in] BarNum          BAR Number (0:BAR0, 1:BAR1)
  @param[in] BarValue        32bit BAR value
**/
VOID
PsfSetTraceHubAcpiDeviceBarValue (
  IN UINT8   BarNum,
  IN UINT32  BarValue
  );

/**
  This procedure will enable MSE for TraceHub ACPI device at PSF level
**/
VOID
PsfEnableTraceHubAcpiDeviceMemorySpace (
  VOID
  );

/**
  Enable HECI device at PSF level

  @param[in] HeciDevice       HECIx Device (HECI1-4)
**/
VOID
PsfEnableHeciDevice (
  IN UINT8      HeciDevice
  );

/**
  Disable HECI device at PSF level

  @param[in] HeciDevice       HECIx Device (HECI1-4)
**/
VOID
PsfDisableHeciDevice (
  IN UINT8      HeciDevice
  );

/**
  Enable SOL device at PSF level
**/
VOID
PsfEnableSolDevice (
  VOID
  );

/**
  Disable SOL device at PSF level
**/
VOID
PsfDisableSolDevice (
  VOID
  );

/**
  Set PMC ABASE value in PSF

  @param[in] Address     Address for ACPI base.
**/
VOID
PsfSetPmcAbase (
  IN  UINT16       Address
  );

/**
  Get PMC ABASE value from PSF

  @retval Address     Address for ACPI base.
**/
UINT16
PsfGetPmcAbase (
  VOID
  );

/**
  Get PMC PWRMBASE value from PSF

  @retval Address     Address for PWRM base.
**/
UINT32
PsfGetPmcPwrmBase (
  VOID
  );

/**
  Disable SATA device at PSF level

  @param[in]  SataCtrlIndex     SATA controller index
**/
VOID
PsfDisableSataDevice (
  IN UINT32     SataCtrlIndex
  );

/**
  Disable TraceHub ACPI devices at PSF level
**/
VOID
PsfDisableTraceHubAcpiDevice (
  VOID
  );

/**
  Hide TraceHub ACPI devices PciCfgSpace at PSF level
**/
VOID
PsfHideTraceHubAcpiDevice (
  VOID
  );

/**
  This procedure will hide TraceHub PciCfgSpace at PSF level
**/
VOID
PsfHideTraceHubDevice (
  VOID
  );

/**
  This procedure will unhide TraceHub PciCfgSpace at PSF level
**/
VOID
PsfUnhideTraceHubDevice (
  VOID
  );

/**
  This procedure will disable TraceHub device at PSF level
**/
VOID
PsfDisableTraceHubDevice (
  VOID
  );

/**
  Configures rootspace 3 bus number for PCIe IMR use

  @param[in] Rs3Bus        bus number
**/
VOID
PsfSetRs3Bus (
  UINT8 Rs3Bus
  );

/**
  Disable PCIe Root Port at PSF level

  @param[in] RpIndex        PCIe Root Port Index (0 based)
**/
VOID
PsfDisablePcieRootPort (
  IN UINT32  RpIndex
  );

/**
  Specifies the root port configuration of the
  PCIe controller. The number on the left of x
  signifies the number of root ports in the controller
  while value on the right is link width. N stands for
  the number of PCIe lanes per root port instance.
**/
typedef enum {
  PsfPcieCtrl4xn,
  PsfPcieCtrl1x2n_2xn,
  PsfPcieCtrl2xn_1x2n,
  PsfPcieCtrl2x2n,
  PsfPcieCtrl1x4n,
  PsfPcieCtrlUndefined
} PSF_PCIE_CTRL_CONFIG;

/**
  Program PSF grant counts for PCI express depending on controllers configuration

  @param[in] PsfPcieCtrlConfigTable   Table with PCIe controllers configuration
  @param[in] NumberOfPcieControllers  Number of PCIe controllers. This is also the size of PsfPcieCtrlConfig table
**/
VOID
PsfConfigurePcieGrantCounts (
  IN PSF_PCIE_CTRL_CONFIG  *PsfPcieCtrlConfigTable,
  IN UINT32                NumberOfPcieControllers
  );

/**
  Program PSF EOI Multicast configuration for ITSS
**/
VOID
PsfConfigurEoiForItss (
  VOID
  );

/**
  This function enables EOI message forwarding in PSF for PCIe ports
  for cases where IOAPIC is present behind this root port.

  @param[in] RpIndex        Root port index (0 based)

  @retval Status
**/
EFI_STATUS
PsfConfigurEoiForPciePort (
  IN  UINT32   RpIndex
  );

//
// Structure for PSF Port Destination ID
//
typedef union {
  UINT32 RegVal;
  struct {
    UINT32  ChannelId   : 8;  // Channel ID
    UINT32  PortId      : 7;  // Port ID
    UINT32  PortGroupId : 1;  // Port Group ID
    UINT32  PsfId       : 8;  // PSF ID
    UINT32  Rsvd        : 7;  // Reserved
    UINT32  ChanMap     : 1;  // Channel map
  } Fields;
} PSF_PORT_DEST_ID;

/**
  PCIe PSF port destination ID (psf_id:port_group_id:port_id:channel_id)

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval Destination ID
**/
PSF_PORT_DEST_ID
PsfPcieDestinationId (
  IN UINT32 RpIndex
  );

/**
  PSF early initialization.
**/
VOID
PsfEarlyInit (
  VOID
  );

/**
  Assign new function number for PCIe Port Number.

  @param[in] RpIndex        PCIe Root Port Index (0 based)
  @param[in] NewFunction    New Function number
**/
VOID
PsfSetPcieFunction (
  IN UINT32  RpIndex,
  IN UINT32  NewFunction
  );

/**
  This function enables PCIe Relaxed Order in PSF
**/
VOID
PsfEnablePcieRelaxedOrder (
  VOID
  );

/**
  Enable VTd support in PSF.
**/
VOID
PchPsfEnableVtd (
  VOID
  );

/**
  Disable PSF address-based peer-to-peer decoding.
**/
VOID
PchPsfDisableP2pDecoding (
  VOID
  );

/**
  Perform registers programming required for
  Management Component Transport Protocol Broadcast Cycle.

  Agent Destination Addresses are being programmed only when adequate
  PCIe root port controllers are function enabled.

  Function sets CSME PMT as a message broadcaster and programs the targets
  of the message in registers only if adequate PCIe root port controllers
  are function enabled. Conditionally, if the CPU PEG exist and is function
  enabled, DMI is also a target.
**/
VOID
PsfConfigureMctpCycle (
  VOID
  );

/**
  This procedure will hide PMC device at PSF level
**/
VOID
PsfHidePmcDevice (
  VOID
  );

#endif
