/** @file
  Install Iio Port Bifurcation Init Data.

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

#include "PeiCommonBoardInitLib.h"
#include <PiPei.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Guid/SetupVariable.h>
#include <Guid/PlatformInfo.h>
#include <Ppi/Smbus2.h>
#include <GpioInitData.h>
#include <Upi/KtiHost.h>
#include "IioBifurcationSlotTable.h"
#include <Library/GpioLib.h>
#include <Library/UbaIioConfigLib.h>
#include <Library/UbaPlatLib.h>
#include <Library/HobLib.h>
#include <Library/UbaIioPortBifurcationInitLib.h>
#include <Library/IioSysInfoLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Pins/GpioPinsVer3.h>
#include <Library/GpioGsxLib.h>

PCIE_PORT_HIDE_TABLE   HideTableListGen5[] = {
//Port 1 2 3 4 5 6 7 8
      {0,1,0,1,0,1,0,1}, //{IIO_BIFURCATE_x4x4x4x4,
      {0,1,1,1,0,1,0,1}, //{IIO_BIFURCATE_x4x4xxx8,
      {0,1,0,1,0,1,1,1}, //{IIO_BIFURCATE_xxx8x4x4,
      {0,1,1,1,0,1,1,1}, //{IIO_BIFURCATE_xxx8xxx8,
      {0,1,1,1,1,1,1,1}, //{IIO_BIFURCATE_xxxxxx16,
      {0,1,1,1,0,1,0,0}, //{IIO_BIFURCATE_x2x2x4x8,
      {0,1,1,1,0,0,0,1}, //{IIO_BIFURCATE_x4x2x2x8,
      {0,1,0,0,0,1,1,1}, //{IIO_BIFURCATE_x8x2x2x4,
      {0,0,0,1,0,1,1,1}, //{IIO_BIFURCATE_x8x4x2x2,
      {0,1,0,1,0,1,0,0}, //{IIO_BIFURCATE_x2x2x4x4x4,
      {0,1,0,1,0,0,0,1}, //{IIO_BIFURCATE_x4x2x2x4x4,
      {0,1,0,0,0,1,0,1}, //{IIO_BIFURCATE_x4x4x2x2x4,
      {0,0,0,1,0,1,0,1}, //{IIO_BIFURCATE_x4x4x4x2x2,
      {0,1,1,1,0,0,0,0}, //{IIO_BIFURCATE_x2x2x2x2x8,
      {0,0,0,0,0,1,1,1}, //{IIO_BIFURCATE_x8x2x2x2x2,
      {0,1,0,1,0,0,0,0}, //{IIO_BIFURCATE_x2x2x2x2x4x4,
      {0,1,0,0,0,1,0,0}, //{IIO_BIFURCATE_x2x2x4x2x2x4,
      {0,0,0,1,0,1,0,0}, //{IIO_BIFURCATE_x2x2x4x4x2x2,
      {0,1,0,0,0,0,0,1}, //{IIO_BIFURCATE_x4x2x2x2x2x4,
      {0,0,0,1,0,0,0,1}, //{IIO_BIFURCATE_x4x2x2x4x2x2,
      {0,0,0,0,0,1,0,1}, //{IIO_BIFURCATE_x4x4x2x2x2x2,
      {0,1,0,0,0,0,0,0}, //{IIO_BIFURCATE_x2x2x2x2x2x2x4,
      {0,0,0,1,0,0,0,0}, //{IIO_BIFURCATE_x2x2x2x2x4x2x2,
      {0,0,0,0,0,1,0,0}, //{IIO_BIFURCATE_x2x2x4x2x2x2x2,
      {0,0,0,0,0,0,0,1}, //{IIO_BIFURCATE_x4x2x2x2x2x2x2,
      {0,0,0,0,0,0,0,0} //{IIO_BIFURCATE_x2x2x2x2x2x2x2x2,

};
PCIE_PORT_HIDE_TABLE   HideTableListGen5Ntb[] = {
//Port 1 2 3 4 5 6 7 8
      {0,1,0,1,0,1,1,0}, //{IIO_BIFURCATE_x4x4x4x4,
      {0,1,1,1,0,1,1,0}, //{IIO_BIFURCATE_x4x4xxx8,
      {0,1,0,1,1,1,1,0}, //{IIO_BIFURCATE_xxx8x4x4,
      {0,1,1,1,1,1,1,0}, //{IIO_BIFURCATE_xxx8xxx8,
      {1,1,1,1,1,1,1,0}, //{IIO_BIFURCATE_xxxxxx16,
      {0,1,1,1,0,1,0,0}, //{IIO_BIFURCATE_x2x2x4x8,
      {0,1,1,1,0,0,1,0}, //{IIO_BIFURCATE_x4x2x2x8,
      {0,1,0,0,1,1,1,0}, //{IIO_BIFURCATE_x8x2x2x4,
      {0,0,0,1,1,1,1,0}, //{IIO_BIFURCATE_x8x4x2x2,
      {0,1,0,1,0,1,0,0}, //{IIO_BIFURCATE_x2x2x4x4x4,
      {0,1,0,1,0,0,1,0}, //{IIO_BIFURCATE_x4x2x2x4x4,
      {0,1,0,0,0,1,1,0}, //{IIO_BIFURCATE_x4x4x2x2x4,
      {0,0,0,1,0,1,1,0}, //{IIO_BIFURCATE_x4x4x4x2x2,
      {0,1,1,1,0,0,0,0}, //{IIO_BIFURCATE_x2x2x2x2x8,
      {0,0,0,0,1,1,1,0}, //{IIO_BIFURCATE_x8x2x2x2x2,
      {0,1,0,1,0,0,0,0}, //{IIO_BIFURCATE_x2x2x2x2x4x4,
      {0,1,0,0,0,1,0,0}, //{IIO_BIFURCATE_x2x2x4x2x2x4,
      {0,0,0,1,0,1,0,0}, //{IIO_BIFURCATE_x2x2x4x4x2x2,
      {0,1,0,0,0,0,1,0}, //{IIO_BIFURCATE_x4x2x2x2x2x4,
      {0,0,0,1,0,0,1,0}, //{IIO_BIFURCATE_x4x2x2x4x2x2,
      {0,0,0,0,0,1,1,0}, //{IIO_BIFURCATE_x4x4x2x2x2x2,
      {0,1,0,0,0,0,0,0}, //{IIO_BIFURCATE_x2x2x2x2x2x2x4,
      {0,0,0,1,0,0,0,0}, //{IIO_BIFURCATE_x2x2x2x2x4x2x2,
      {0,0,0,0,0,1,0,0}, //{IIO_BIFURCATE_x2x2x4x2x2x2x2,
      {0,0,0,0,0,0,1,0}, //{IIO_BIFURCATE_x4x2x2x2x2x2x2,
      {0,0,0,0,0,0,0,0} //{IIO_BIFURCATE_x2x2x2x2x2x2x2x2,

};
PCIE_PORT_HIDE_TABLE   HideTableListGen4[] = {
//Port 1 2 3 4 5 6 7 8
      {1,0,1,0,1,0,1,0}, //{IIO_BIFURCATE_x4x4x4x4,
      {1,1,1,0,1,0,1,0}, //{IIO_BIFURCATE_x4x4xxx8,
      {1,0,1,0,1,1,1,0}, //{IIO_BIFURCATE_xxx8x4x4,
      {1,1,1,0,1,1,1,0}, //{IIO_BIFURCATE_xxx8xxx8,
      {1,1,1,1,1,1,1,0} //{IIO_BIFURCATE_xxxxxx16,
};

VOID
SetBifurcations (
    IN OUT IIO_GLOBALS *IioGlobalData,
    IN     IIO_BIFURCATION_DATA_ENTRY_EX *BifurcationTable,
    IN     UINT8                  BifurcationEntries,
    IN     BOOLEAN                *AutoBifEnable
)
{
  UINT8 Socket;
  UINT8 Iou;
  UINT8 Index;
  UINT8 MaxIouNumPerSocket;

  MaxIouNumPerSocket = GetMaxIouNumPerSocket ();
  for (Index = 0; Index < BifurcationEntries; Index++) {
    Socket = BifurcationTable[Index].Socket;
    Iou = BifurcationTable[Index].IouNumber;
    if (Iou < MaxIouNumPerSocket){
      if (IioGlobalData->SetupData.ConfigIOU[Socket][Iou] == IIO_BIFURCATE_AUTO) {
        IioGlobalData->SetupData.ConfigIOU[Socket][Iou] = BifurcationTable[Index].Bifurcation;
      }
    } else {
      DEBUG((EFI_D_ERROR, "Invalid bifurcation table: Bad Iou (%d)", Iou));
    }
  } // for Index
}

VOID
EnableHotPlug (
    IN OUT IIO_GLOBALS *IioGlobalData,
    IN UINT16 Port,
    IN UINT8 VppPort,
    IN UINT8 VppAddress,
    IN UINT8 PortOwnership
  )
{
  IioGlobalData->SetupData.SLOTHPCAP[Port]= ENABLE;
  IioGlobalData->SetupData.SLOTAIP[Port]  = ENABLE;  // Attention Indicator Present
  IioGlobalData->SetupData.SLOTPIP[Port]  = ENABLE;  // Power Indicator Present
  IioGlobalData->SetupData.SLOTMRLSP[Port]= ENABLE; // MRL Sensor Present
  IioGlobalData->SetupData.SLOTABP[Port]  = ENABLE; // Attention Button Present
  IioGlobalData->SetupData.SLOTPCP[Port]  = ENABLE; // Power Controlled Present

 if (PortOwnership ==  PCIEAIC_OCL_OWNERSHIP) {
    IioGlobalData->SetupData.SLOTAIP[Port]  = DISABLE;  // Attention Indicator Present
    IioGlobalData->SetupData.SLOTPIP[Port]  = DISABLE;  // Power Indicator Present
    // APTIOV_SERVER_OVERRIDE_RC_START : Enable PCIe Hot-plug for NVME controller coming from Oculink ports
    IioGlobalData->SetupData.SLOTABP[Port]  = DISABLE;
    IioGlobalData->SetupData.SLOTPCP[Port]  = DISABLE;
    IioGlobalData->SetupData.SLOTMRLSP[Port]= DISABLE;
    // APTIOV_SERVER_OVERRIDE_RC_END : Enable PCIe Hot-plug for NVME controller coming from Oculink ports
  }
  if (PortOwnership ==  VMD_OWNERSHIP) {
    IioGlobalData->SetupData.SLOTABP[Port]  = DISABLE;
    IioGlobalData->SetupData.SLOTPCP[Port]  = DISABLE;
    IioGlobalData->SetupData.SLOTMRLSP[Port]= DISABLE;
  }
  //
  // Set SLTCAP settings based on VMD/PCIe SSD Ownership
  //
  if ((PortOwnership == PCIEAIC_OCL_OWNERSHIP) ||
      (PortOwnership == VMD_OWNERSHIP)) {
    IioGlobalData->SetupData.SLOTHPSUP[Port]= ENABLE;   // HotPlug Surprise
  }

  if (VppAddress != VPP_ADDR_MAX) {
    IioGlobalData->SetupData.VppEnabled[Port]= TRUE;
    IioGlobalData->SetupData.VppPort[Port] = VppPort;
    IioGlobalData->SetupData.VppAddress[Port] = VppAddress;
  } else {
    DEBUG ((EFI_D_ERROR, "PCIE HOT Plug. Missing VPP values on slot table\n"));
  }
}

VOID
ConfigSlots (
    IN OUT IIO_GLOBALS        *IioGlobalData,
    IN IIO_SLOT_CONFIG_DATA_ENTRY_EX  *Slot,
    IN UINT8                  SlotEntries
  )
{
  UINT8  Index;
  UINT16 Port;

  for (Index = 0; Index < SlotEntries; Index ++) {
    Port=Slot[Index].PortIndex;
    if ((Slot[Index].Hidden == HIDE) && (Slot[Index].SlotNumber != NO_SLT_IMP)) {
      IioGlobalData->SetupData.HidePEXPMenu[Port] = HIDE;
      IioGlobalData->SetupData.PEXPHIDE[Port] = HIDE;
    }
    /// Check if slot is assigned.
    if (Slot[Index].SlotNumber != NO_SLT_IMP){
      IioGlobalData->SetupData.SLOTIMP[Port] = SLT_IMP;
      IioGlobalData->SetupData.SLOTPSP[Port] = Slot[Index].SlotNumber;
      IioGlobalData->SetupData.SLOTEIP[Port] = Slot[Index].InterLockPresent;
      if (Slot[Index].SlotPowerLimitScale != PWR_SCL_MAX) {
        IioGlobalData->SetupData.SLOTSPLS[Port] = Slot[Index].SlotPowerLimitScale;
        IioGlobalData->SetupData.SLOTSPLV[Port] = Slot[Index].SlotPowerLimitValue;
      }
      if ((Slot[Index].MuxSMBusAddress != SMB_ADDR_MAX) && (Slot[Index].MuxSMBusChannel != SMB_DATA_MAX)) {
        IioGlobalData->SetupData.MuxAddress[Port] = Slot[Index].MuxSMBusAddress;
        IioGlobalData->SetupData.ChannelID[Port] = Slot[Index].MuxSMBusChannel;
      }
      if (Slot[Index].HotPlugCapable != DISABLE) {
        EnableHotPlug (IioGlobalData, Port, Slot[Index].VppPort, Slot[Index].VppAddress, REGULAR_PCIE_OWNERSHIP);
      }
      if (Slot[Index].ExtnCardHotPlugCapable != DISABLE) {
        DEBUG ((EFI_D_INFO, "Slot[%d].ExtnCardHPVppPort = 0x%x\n", Index, Slot[Index].ExtnCardHPVppPort));
        DEBUG ((EFI_D_INFO, "Slot[%d].ExtnCardHPVppAddress = 0x%x\n", Index, Slot[Index].ExtnCardHPVppAddress));
        EnableHotPlug (IioGlobalData, Port, Slot[Index].ExtnCardHPVppPort, Slot[Index].ExtnCardHPVppAddress, REGULAR_PCIE_OWNERSHIP);
      }
    }
  }
}

/**
    Verify if VMD is enabled and override Slot configuration
    based on the VMD settings

    @param IioGlobalData           Pointer to Iio Globals.
    @param Slot                  - Slot configuration settings
    @param SlotEntries           - Number of slot entries

    @retval None

**/
/**
    Verify if VMD is enabled and override Slot configuration
    based on the VMD settings

    @param IioGlobalData           Pointer to Iio Globals.
    @param Slot                  - Slot configuration settings
    @param SlotEntries           - Number of slot entries

    @retval None

**/

VOID
OverrideConfigSlots (
    IN OUT IIO_GLOBALS        *IioGlobalData,
    IN IIO_SLOT_CONFIG_DATA_ENTRY_EX  *Slot,
    IN UINT8                  SlotEntries
  )
{
  UINT8  Index;
  UINT16 Port;
  UINT8  MaxPortNumberPerSocket;
  UINT16 IioIndex;
  UINT8  Stack;
  UINT16 PortIndex;

  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[0];
  for (Index =0; Index < SlotEntries; Index ++) {
    Port = Slot[Index].PortIndex;

    IioGlobalData->IioVar.IioOutData.RetimerConnectCount[Port] = Slot[Index].RetimerConnectCount;

    //
    // Check if Slot is capable of PcieSSD Solution and override the SLOT Config values
    //
    if (Slot[Index].PcieSSDCapable) {
      IioIndex = Port/MaxPortNumberPerSocket;
      PortIndex = (Port - (MaxPortNumberPerSocket * IioIndex));
      Stack = IioGlobalData->IioVar.IioVData.StackPerPort[IioIndex][PortIndex];
      DEBUG ((DEBUG_INFO, "Stack = %x, Port = %x, PortIndex = %x\n", Stack, Port, PortIndex));

      //
      // check if VMD will own Pcie Root Port
      //
      if (IioGlobalData->SetupData.VMDEnabled[IioIndex][Stack]) {
        if (IioGlobalData->SetupData.VMDPortEnable[IioIndex][PortIndex]) {
          IioGlobalData->IioVar.IioOutData.PciePortOwnership[Port] = VMD_OWNERSHIP;
        }
      } else {

        DEBUG ((DEBUG_INFO, "IioGlobalData->SetupData.PcieAICEnabled[%x] = %x\n", Stack, IioGlobalData->SetupData.PcieAICEnabled[(IioIndex * MAX_STACKS_PER_SOCKET) + Stack]));
        //
        // Check if Pcie AIC Card will be present on Pcie Root Port
        //
        if (IioGlobalData->SetupData.PcieAICEnabled[(IioIndex * MAX_STACKS_PER_SOCKET) + Stack]) {
          //
          // Force to have this port enabled by default for hot-plug.
          //
          IioGlobalData->SetupData.IioPcieConfig.PciePortEnable[(IioIndex * MaxPortNumberPerSocket) + Port] = ENABLE;
          IioGlobalData->IioVar.IioOutData.PciePortOwnership[Port] = PCIEAIC_OCL_OWNERSHIP;
          DEBUG ((EFI_D_INFO, "Port = %x, PciePortEnable = %x\n", Port, IioGlobalData->SetupData.IioPcieConfig.PciePortEnable[(IioIndex * MaxPortNumberPerSocket) + Port]));
        }
      } // No _VMD Ownership

      DEBUG ((DEBUG_INFO, "PciePortOwnerShip[%x] = %x\n",Port, IioGlobalData->IioVar.IioOutData.PciePortOwnership[Port]));

      // if PcieSSDSupport required do slot override settings accordingly
      if ((IioGlobalData->IioVar.IioOutData.PciePortOwnership[Port] != REGULAR_PCIE_OWNERSHIP) &&
          (IioGlobalData->SetupData.PEXPHIDE [Port] != HIDE)) {
        IioGlobalData->SetupData.SLOTIMP[Port] = SLT_IMP;
        IioGlobalData->SetupData.SLOTPSP[Port] = 0x50  + Port; // Just program a value for PCIEACI_OCL/VMD
        IioGlobalData->SetupData.SLOTEIP[Port] = DISABLE;

        if (Slot[Index].SlotPowerLimitScale != PWR_SCL_MAX) {
          IioGlobalData->SetupData.SLOTSPLS[Port] = Slot[Index].SlotPowerLimitScale;
          IioGlobalData->SetupData.SLOTSPLV[Port] = Slot[Index].SlotPowerLimitValue;
        }
        DEBUG ((DEBUG_INFO,"Slot[Index].PcieSSDVppPort = %x\n", Slot[Index].PcieSSDVppPort));
        // Enable hot-plug if slot/port supports it
        if (Slot[Index].PcieSSDVppPort != VPP_PORT_MAX) {
        DEBUG ((DEBUG_INFO, "IioGlobalData->SetupData.VMDHotPlugEnable[%d][%x] = %x\n", IioIndex, Stack, IioGlobalData->SetupData.VMDHotPlugEnable[IioIndex][Stack]));
        DEBUG ((DEBUG_INFO, "IioGlobalData->SetupData.PcieAICHotPlugEnable[%x] = %x\n",Stack,IioGlobalData->SetupData.PcieAICHotPlugEnable[(IioIndex * MAX_STACKS_PER_SOCKET) + Stack]));
        // Check if hot-plug is enabled for VMD or PCIeAIC case.
          if (((IioGlobalData->IioVar.IioOutData.PciePortOwnership[Port] == VMD_OWNERSHIP) &&
               (IioGlobalData->SetupData.VMDHotPlugEnable[IioIndex][Stack])) ||
              ((IioGlobalData->IioVar.IioOutData.PciePortOwnership[Port] == PCIEAIC_OCL_OWNERSHIP) &&
               (IioGlobalData->SetupData.PcieAICHotPlugEnable[(IioIndex * MAX_STACKS_PER_SOCKET) + Stack]))) {
            EnableHotPlug (IioGlobalData, Port, Slot[Index].PcieSSDVppPort, Slot[Index].PcieSSDVppAddress, IioGlobalData->IioVar.IioOutData.PciePortOwnership[Port]);
            DEBUG ((DEBUG_INFO,"Enable HotPlug Done\n"));
          }
        }
        //
        // Unhide the port in order to get configured and it will be hide later for VMDLateSetup if VMD own the Pcie Root Port
        //
        IioGlobalData->SetupData.PEXPHIDE[Port] = NOT_HIDE;
      }// PcieSSDSupport
    }// PcieSSDCapable
  }// Per Slot
}

/**
    Auto determine which PCIe Root port to be hidden if its
    lanes are assigned to its preceding root port...use the
    Setup option variable of ConfigIOU to determine which ports
    are to be hidden on each IOU for corresponding IIO

    @param IOUx                  - IOUx Index
    @param IioIndex              - Index to Iio
    @param IioGlobalData           Pointer to Iio Globals.

    @retval None

**/
VOID
CalculatePEXPHideFromIouBif (
    IN OUT IIO_GLOBALS *IioGlobalData,
    IN UINT8 IioIndex,
    IN UINT8 Iou
)
{

  UINT8 *PXPHide;
  UINT8 *HidePEXPMenu;
  UINT8 CurrentIOUConfigValue;
  UINT16 PXPOffset;
  UINT8 PortIndex;
  UINT8 MaxPortNumPerIou;
  UINT8 HideTableListSize;
  PCIE_PORT_HIDE_TABLE *HideTableListPtr;

  if(Iou >= GetMaxIouNumPerSocket ()) {
    return;
  }
  PXPHide = IioGlobalData->SetupData.PEXPHIDE;
  HidePEXPMenu = IioGlobalData->SetupData.HidePEXPMenu;
  MaxPortNumPerIou = GetMaxPortNumPerIou ();
  CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][Iou];
  PXPOffset = IioIndex * NUMBER_PORTS_PER_SOCKET + Iou * MaxPortNumPerIou + PORT_1A_INDEX_SPR;

  // Hide all ports for IIO_BIFURCATE_xxxxxxxx
  if (CurrentIOUConfigValue == IIO_BIFURCATE_xxxxxxxx) {
    for (PortIndex = 0; PortIndex < MaxPortNumPerIou; PortIndex++) {
      PXPHide[PXPOffset + PortIndex] = HIDE;
      HidePEXPMenu[PXPOffset + PortIndex] = HIDE;
    } // for PortIndex
    IioGlobalData->SetupData.ConfigIOU[IioIndex][Iou] = IIO_BIFURCATE_x4x4x4x4;
    return;
  }

  if (IIO_MODE_GEN4_ONLY == IioGlobalData->SetupData.PcieSubSystemMode[IioIndex][Iou]) {
    HideTableListPtr = HideTableListGen4;
    HideTableListSize = sizeof (HideTableListGen4) / sizeof (PCIE_PORT_HIDE_TABLE);
  } else {
    HideTableListPtr = HideTableListGen5;
    HideTableListSize = sizeof (HideTableListGen5) / sizeof (PCIE_PORT_HIDE_TABLE);
    PortIndex  = GetPortIndexbyStack (Iou + 1, IioPortH);
    if ((IioGlobalData->IioVar.IioOutData.PciePortOwnership[(IioIndex * NUMBER_PORTS_PER_SOCKET) + PortIndex] == NTB_PORT_DEF_NTB_NTB ) ||
      (IioGlobalData->IioVar.IioOutData.PciePortOwnership[(IioIndex * NUMBER_PORTS_PER_SOCKET) + PortIndex] == NTB_PORT_DEF_NTB_RP )) {
      HideTableListPtr = HideTableListGen5Ntb;
      HideTableListSize = sizeof (HideTableListGen5Ntb) / sizeof (PCIE_PORT_HIDE_TABLE);
    }
  }

  if (CurrentIOUConfigValue >= HideTableListSize) {
    CurrentIOUConfigValue = IIO_BIFURCATE_x4x4x4x4;
    IioGlobalData->SetupData.ConfigIOU[IioIndex][Iou] = CurrentIOUConfigValue;
    DEBUG ((DEBUG_WARN,"ConfigIOU is override with default value on IioIndex %d IOU %d\n", IioIndex, Iou));
  }

  for (PortIndex = 0; PortIndex < MaxPortNumPerIou; PortIndex++) {
    PXPHide[PXPOffset + PortIndex] = HideTableListPtr[CurrentIOUConfigValue].PortHide[PortIndex];
    HidePEXPMenu[PXPOffset + PortIndex] = HideTableListPtr[CurrentIOUConfigValue].PortHide[PortIndex];
  } // for PortIndex
}


VOID
DumpPort(
    IIO_GLOBALS *IioGlobalData,
    UINT16       Port,
    UINT8        NumberOfPorts
)
{
  UINT16 Index;
  DEBUG ((EFI_D_INFO, "IDX, Port Hide, Slot Impl, Slot Number, HotPlug, PcieSSD, VppPort, VppAddress, Interlock\n"));
  for (Index = Port; Index < (Port + NumberOfPorts); Index++ ) {
  DEBUG ((EFI_D_INFO, "%3d|   %2d    |    %2d    |   %3d      |   %3d  |  %3d  |  0x%02x  |  0x%02x     |  %2d      \n", \
                       Index, \
                       IioGlobalData->SetupData.PEXPHIDE[Index],  \
                       IioGlobalData->SetupData.SLOTIMP[Index],   \
                       IioGlobalData->SetupData.SLOTPSP[Index],   \
                       IioGlobalData->SetupData.SLOTHPCAP[Index], \
                       IioGlobalData->IioVar.IioOutData.PciePortOwnership[Index],   \
                       IioGlobalData->SetupData.VppPort[Index],   \
                       IioGlobalData->SetupData.VppAddress[Index],\
                       IioGlobalData->SetupData.SLOTEIP[Index]));
  }
}

/**
  Dump iio configuration. Dump the current IIO configuration to the serial log.

  @param IioIndex       - Index to Iio
  @param IioGlobalData  - Pointer to Iio Globals.

  @retval None
**/
VOID
DumpIioConfiguration(
  IN UINT8 IioIndex,
  IN IIO_GLOBALS *IioGlobalData
)
{
  UINT8 Iou;
  UINT16 PortIndex;
  UINT8 MaxPortNumberPerSocket;
  UINT8 Bifurcation;
  UINT8 IouPorts;

  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];
  PortIndex = IioIndex * MaxPortNumberPerSocket;
  /// First dump the socket number;
  DEBUG((EFI_D_INFO, "Socket number: %d \n", IioIndex));

  /// Dump DMI configuration:
  if (PortIndex == 0) {
    if (IioIndex == 0){
      DEBUG((EFI_D_INFO, "PORT 0: DMI Port\n"));
    } else {
      DEBUG((EFI_D_INFO, "PORT 0: DMI Port working as PCIE\n"));
    }
    DumpPort(IioGlobalData, PortIndex, 1);
  }
  IouPorts = 8;
  /// Dump IOU bifurcations:
  for (Iou = Iio_Iou0; Iou< Iio_IouMax; Iou ++) {
    /// Reset port index.
    PortIndex = IioIndex * MaxPortNumberPerSocket;
    // Get the bifurcation
    switch (Iou) {
      case Iio_Iou0:
        Bifurcation = IioGlobalData->SetupData.ConfigIOU[IioIndex][0];
        PortIndex += PORT_1A_INDEX_SPR;
        DEBUG((EFI_D_INFO, "IUO0: Root Port 1, Bifurcation: %d\n", Bifurcation));
        break;
      case Iio_Iou1:
        Bifurcation = IioGlobalData->SetupData.ConfigIOU[IioIndex][1];
        PortIndex += PORT_2A_INDEX_SPR;
        DEBUG((EFI_D_INFO, "IUO1: Root Port 2, Bifurcation: %d\n", Bifurcation));
        break;
      case Iio_Iou2:
        Bifurcation = IioGlobalData->SetupData.ConfigIOU[IioIndex][2];
        PortIndex += PORT_3A_INDEX_SPR;
        DEBUG((EFI_D_INFO, "IUO2: Root Port 3, Bifurcation: %d\n", Bifurcation));
        break;
      case Iio_Iou3:
        Bifurcation = IioGlobalData->SetupData.ConfigIOU[IioIndex][3];
        PortIndex += PORT_4A_INDEX_SPR;
        DEBUG((EFI_D_INFO, "IOU3, Bifurcation: %d\n", Bifurcation));
        break;
      case Iio_Iou4:
        Bifurcation = IioGlobalData->SetupData.ConfigIOU[IioIndex][4];
        PortIndex += PORT_5A_INDEX_SPR;
        DEBUG((EFI_D_INFO, "IOU4, Bifurcation: %d\n", Bifurcation));
        break;
      default:
        DEBUG((EFI_D_INFO, "Iou no detected = %d",Iou));
        break;
    }
    DumpPort(IioGlobalData, PortIndex, IouPorts);
  }
}

EFI_STATUS
GetPlatformIioTable (
  IN IIO_GLOBALS                                  *IioGlobalData,
  PLATFORM_IIO_CONFIG_UPDATE_TABLE_3             *IioConfigTablePtr
)
{
  EFI_STATUS                        Status;
  UBA_CONFIG_DATABASE_PPI           *UbaConfigPpi = NULL;
  UINTN                             TableSize;

  Status = PeiServicesLocatePpi (
              &gUbaConfigDatabasePpiGuid,
              0,
              NULL,
              &UbaConfigPpi
              );
  if (EFI_ERROR(Status)) {
    return PLATFORM_IIO_CONFIG_UPDATE_VERSION_UNSUPPORTED;
  }

  TableSize = sizeof(PLATFORM_IIO_CONFIG_UPDATE_TABLE_3);
  Status = UbaConfigPpi->GetData (
                                 UbaConfigPpi,
                                 &gPlatformIioConfigDataGuid,
                                 IioConfigTablePtr,
                                 &TableSize
                                 );
  if (EFI_ERROR(Status)) {
    return Status;
  }
  ASSERT (IioConfigTablePtr->Signature == PLATFORM_IIO_CONFIG_UPDATE_SIGNATURE);
  return EFI_SUCCESS;
}

VOID
SystemHideIioPortsCommon(
    IIO_GLOBALS *IioGlobalData
)
{
  UINT8   MaxIouNumPerSocket;
  UINT8   Iou;
  UINT8   IioIndex;

  MaxIouNumPerSocket = GetMaxIouNumPerSocket ();
  for (IioIndex = Iio_Socket0; IioIndex < MaxIIO; IioIndex++) {
    if (IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) {
      for (Iou = 0; Iou < MaxIouNumPerSocket; Iou++) {
        CalculatePEXPHideFromIouBif (IioGlobalData, IioIndex, Iou);
      }
      DumpIioConfiguration (IioIndex, IioGlobalData);
    }
  }
}

/**
  Return the board ID.

  @return   The board ID.
**/
UINT8
EFIAPI
SystemBoardIdValue (
  VOID
  )
{
  EFI_HOB_GUID_TYPE       *GuidHob;
  EFI_PLATFORM_INFO       *mPlatformInfo;

  GuidHob = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    return TypePlatformUnknown;
  }
  mPlatformInfo = GET_GUID_HOB_DATA(GuidHob);
  return mPlatformInfo->BoardId;
}


EFI_STATUS
SmbusWriteByteThroughMux (
  IN IIO_GLOBALS          *IioGlobalData,
  IN  MUX_SMB_ADDR        *SmbAddrTablePtr
  )
{
  CONST EFI_PEI_SERVICES    **PeiServices;
  EFI_STATUS                Status;
  EFI_SMBUS_DEVICE_ADDRESS  SmbusDeviceAddress;
  EFI_SMBUS_DEVICE_COMMAND  SmbusCommand;
  EFI_SMBUS_OPERATION       SmbusOperation;
  EFI_PEI_SMBUS2_PPI        *Smbus = NULL;
  UINT8                     SmbusByteData;
  UINT8                     RetryCount;
  UINTN                     SmbusByteLength;

  if (SmbAddrTablePtr->RegOffset == SMB_ADDR_MAX) {
    return EFI_UNSUPPORTED;
  }
  PeiServices = GetPeiServicesTablePointer ();
  Status = (**PeiServices).LocatePpi (
    PeiServices,
    &gEfiPeiSmbus2PpiGuid,
    0,
    NULL,
    &Smbus
    );
  if (Status != EFI_SUCCESS || Smbus == NULL) {
    DEBUG ((DEBUG_ERROR, "!!!!Failed to get SMBus protocol %x\n", Status));
    return EFI_UNSUPPORTED;
  }

  for (RetryCount = 0; RetryCount < NUM_OF_RETRIES; RetryCount++) {
    //
    // Configure through the smbus MUX PCA9545 to start communicate to retimer
    //
    if (SmbAddrTablePtr->MuxSmbAddress != SMB_ADDR_MAX) {
      SmbusDeviceAddress.SmbusDeviceAddress = SmbAddrTablePtr->MuxSmbAddress >> 1;
      SmbusCommand = 0; //byte 0 in MUX is for channel enabling
      if (SmbAddrTablePtr->MuxSmbChannel < 4) {
        SmbusByteData = 1 << SmbAddrTablePtr->MuxSmbChannel; // bit x for channel x
      }
      SmbusByteLength = 1;
      Status = Smbus->Execute (
        Smbus,
        SmbusDeviceAddress,
        SmbusCommand,
        EfiSmbusSendByte,
        FALSE,
        &SmbusByteLength,
        &SmbusByteData
        );
      if (EFI_ERROR(Status)) {
        continue;
      }
    }
    //
    // Write to retimer0 if exist on that stack
    //
    if (SmbAddrTablePtr->SmbAddress0 != 0xff) {
      SmbusDeviceAddress.SmbusDeviceAddress = SmbAddrTablePtr->SmbAddress0 >> 1;
      SmbusCommand = SmbAddrTablePtr->RegOffset;
      SmbusByteData = SmbAddrTablePtr->RegValue;
      SmbusByteLength = 1;
      SmbusOperation = EfiSmbusWriteByte;
      Status = Smbus->Execute (
        Smbus,
        SmbusDeviceAddress,
        SmbusCommand,
        SmbusOperation,
        FALSE,
        &SmbusByteLength,
        &SmbusByteData
        );
      if (EFI_ERROR(Status)) {
        continue;
      }
    }
    //
    // Write to retimer1 if exist on that stack
    //
    if (SmbAddrTablePtr->SmbAddress1 != 0xff) {
      SmbusDeviceAddress.SmbusDeviceAddress = SmbAddrTablePtr->SmbAddress1 >> 1;
      SmbusCommand = SmbAddrTablePtr->RegOffset;
      SmbusByteData = SmbAddrTablePtr->RegValue;
      SmbusByteLength = 1;
      SmbusOperation = EfiSmbusWriteByte;
      Status = Smbus->Execute (
        Smbus,
        SmbusDeviceAddress,
        SmbusCommand,
        SmbusOperation,
        FALSE,
        &SmbusByteLength,
        &SmbusByteData
        );
    }
    if (!EFI_ERROR(Status)) {
      return EFI_SUCCESS;
    }
  } //RetryCount

  DEBUG ((DEBUG_ERROR, "Status = %r\n", Status));
  return EFI_UNSUPPORTED;
}

//
// Init retimers with input in hierarchies Mux/Retimer/Offset
// Second retimer could have the same value as that for the first one.
//
VOID
IioRetimerInit (
  IN IIO_GLOBALS          *IioGlobalData,
  MUX_SMB_ADDR            *Retimer_Init_Table
  )
{
  UINT8 Index;
  EFI_STATUS                Status;

  for (Index = 0; Retimer_Init_Table[Index].MuxSmbAddress != 0xff; Index++) {
    Status = SmbusWriteByteThroughMux (IioGlobalData, &Retimer_Init_Table[Index]);
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "!!!!Failed to write smbus register in Retimer_Init_Table[%d]\n", Index));
    }
  }
}

/**
  Override default bifurcation slots.
  The bifurcation IDs are available to PCH via GSX GPIOs (serial GPIO expander).
  Riser ID bits are connected via PCH parallel GPIOs.

  @param IioGlobalData           Pointer to Iio Globals.
**/
VOID
OverrideDefaultBifSlots (
  IN OUT IIO_GLOBALS       *IioGlobalData
  )
{
  UINT32                      Data[2];
  UINT32                      GpioVal;
  UINT32                      RiserID0;
  UINT32                      RiserID1;
  UINT8                       BifIOU[2][5];
  UINT8                       Skt1DMIBif;
  EFI_STATUS                  Status;
  UINT16                      GpioCom;
  UINT16                      NIn;
  UINT16                      NOut;
  UINT8                       Socket;
  UINT8                       IouNumber;

  Status = GpioGetInputValue (GPIO_VER3_GPP_J13, &GpioVal);  // get FM_PCIE_FV_BIF_EN value
  if (EFI_ERROR(Status) || (GpioVal == 0)) {
    DEBUG ((DEBUG_ERROR, "Failed to get GPIO_J13 value or its value is 0, Status = %r\n", Status));
    return;
  }

  // If FM_PCIE_FV_BIF_EN == Logic 1, set PCIe bifurcation on both CPUs for all PCIe ports.
  // Detail can refer Archer_City_RPAS_Rev_0.9.pdf, section 6.11 CPU PCIE AUTO BIFURCATION SUPPORT.

  // For Riser cards, BIOS get the bifurcation IDs via PCH parallel GPIOs.
  Status = GpioGetInputValue (GPIO_VER3_GPP_M12, &RiserID0);  // CPU0 RISER1 (left)
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get GPIO_M12 value, Status = %r\n", Status));
  }
  Status = GpioGetInputValue (GPIO_VER3_GPP_M15, &RiserID1);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get GPIO_M15 value, Status = %r\n", Status));
  }
  if ((RiserID1 == 0) && (RiserID0 == 0)) {
    IioGlobalData->SetupData.ConfigIOU[0][0] = IIO_BIFURCATE_xxxxxx16;
    IioGlobalData->SetupData.ConfigIOU[0][1] = IIO_BIFURCATE_xxxxxx16;
  } else if ((RiserID1 == 0) && (RiserID0 == 1)) {
    IioGlobalData->SetupData.ConfigIOU[0][0] = IIO_BIFURCATE_xxxxxx16;
    IioGlobalData->SetupData.ConfigIOU[0][1] = IIO_BIFURCATE_xxx8xxx8;
  } else {
    DEBUG ((DEBUG_INFO, "The other situations will keep settings in function SetBifurcations()\n"));
  }

  Status = GpioGetInputValue (GPIO_VER3_GPP_M16, &RiserID0);  // CPU1 RISER2 (right)
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get GPIO_M16 value, Status = %r\n", Status));
  }
  Status = GpioGetInputValue (GPIO_VER3_GPP_M17, &RiserID1);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get GPIO_M17 value, Status = %r\n", Status));
  }
  if ((RiserID1 == 0) && (RiserID0 == 0)) {
    IioGlobalData->SetupData.ConfigIOU[1][1] = IIO_BIFURCATE_xxxxxx16;
    IioGlobalData->SetupData.ConfigIOU[1][2] = IIO_BIFURCATE_xxxxxx16;
  } else if ((RiserID1 == 0) && (RiserID0 == 1)) {
    IioGlobalData->SetupData.ConfigIOU[1][1] = IIO_BIFURCATE_xxx8xxx8;
    IioGlobalData->SetupData.ConfigIOU[1][2] = IIO_BIFURCATE_xxxxxx16;
  } else {
    DEBUG ((DEBUG_INFO, "The other situations will keep settings in function SetBifurcations()\n"));
  }

  // For Broadway card type and MCIO card adapter configuration, BIOS get the bifurcation IDs via PCH GSX GPIOs (serial GPIO expander).
  GpioCom = 0;
  NIn = 0x4;
  NOut = 0x1;
  for (Socket = 0; Socket < 2; Socket++) {
    for (IouNumber = 0; IouNumber < 5; IouNumber++ ) {
      BifIOU[Socket][IouNumber] = 0xFF;
    }
  }
  Status = InitGSX (GpioCom, NIn, NOut);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "InitGSX fail, Status = %r.\n", Status));
    return;
  }
  Status = GsxRead (GpioCom, Data);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "GsxRead fail, Status = %r.\n", Status));
    return;
  }
  DEBUG ((DEBUG_INFO, "GsxRead Data[0] = %x.\n", Data[0]));
  BifIOU[0][2] = (UINT8) (Data[0] & 0x7);          // Slot B (Broadway)
  BifIOU[0][3] = (UINT8) ((Data[0] >> 3) & 0x1F);  // MCIO to PCIe CEM Adapter
  BifIOU[0][4] = (UINT8) ((Data[0] >> 8) & 0x1F);  // MCIO to PCIe CEM Adapter
  BifIOU[1][0] = (UINT8) ((Data[0] >> 13) & 0x7);  // Slot E (Broadway)
  BifIOU[1][3] = (UINT8) ((Data[0] >> 16) & 0x1F); // MCIO to PCIe CEM Adapter
  BifIOU[1][4] = (UINT8) ((Data[0] >> 21) & 0x1F); // MCIO to PCIe CEM Adapter
  Skt1DMIBif = (UINT8) ((Data[0] >> 26) & 0x1);    // TODO: current we have no setup variable to set bifurcation for skt1 DMI, when it is implemnted,
                                                   // will according this value to auto set DMI(as PCIe) bifurcation

  for (Socket = 0; Socket < 2; Socket++) {
    for (IouNumber = 0; IouNumber < 5; IouNumber++ ) {
      if (((Socket == 0) && ((IouNumber == 0) || (IouNumber == 1))) || ((Socket == 1) && ((IouNumber == 1) || (IouNumber == 2)))) {  // Riser card related, setting as above, skip them
        continue;
      }
      switch(BifIOU[Socket][IouNumber]) {
        case 0:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_xxxxxx16;
          break;
        case 1:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_xxx8xxx8;
          break;
        case 2:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_xxx8x4x4;
          break;
        case 3:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_x4x4xxx8;
          break;
        case 4:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_x4x4x4x4;
          break;
        case 5:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_x4x2x2x8;
          break;
        case 6:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_x2x2x4x8;
          break;
        case 7:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_x8x4x2x2;
          break;
        case 8:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_x8x2x2x4;
          break;
        case 9:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_x2x2x2x2x8;
          break;
        case 10:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_x8x2x2x2x2;
          break;
        case 11:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_x2x2x4x4x4;
          break;
        case 12:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_x4x2x2x4x4;
          break;
        case 13:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_x4x4x2x2x4;
          break;
        case 14:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_x4x4x4x2x2;
          break;
        case 15:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_x2x2x2x2x4x4;
          break;
        case 16:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_x2x2x4x2x2x4;
          break;
        case 17:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_x4x2x2x2x2x4;
          break;
        case 18:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_x2x2x4x4x2x2;
          break;
        case 19:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_x4x2x2x4x2x2;
          break;
        case 20:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_x4x4x2x2x2x2;
          break;
        case 21:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_x2x2x2x2x2x2x4;
          break;
        case 22:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_x2x2x2x2x4x2x2;
          break;
        case 23:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_x4x2x2x2x2x2x2;
          break;
        case 24:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_x2x2x2x2x2x2x2x2;
          break;
        default:
          IioGlobalData->SetupData.ConfigIOU[Socket][IouNumber] = IIO_BIFURCATE_xxxxxxxx;
          break;
      }
    }
  }
}

/**

  Program the IIO_GLOBALS data structure with OEM IIO init values for SLOTs and Bifurcation.

  @param mSB - pointer to this protocol
  @param IioUds - Pointer to the IIO UDS datastructure.

  @retval EFI_SUCCESS

**/
VOID
IioPortBifurcationInitCallback (
    IN IIO_GLOBALS *IioGlobalData
)
{
  UINT8                                IioIndex;
  UINT8                                IouIndex;
  IIO_BIFURCATION_DATA_ENTRY_EX        *BifurcationTableEx = NULL;
  UINT8                                BifurcationEntries;
  EFI_STATUS                           Status;
  PLATFORM_IIO_CONFIG_UPDATE_TABLE_3   IioConfigTable; // PLATFORM_IIO_CONFIG_UPDATE_TABLE_3 is back compatable to PLATFORM_IIO_CONFIG_UPDATE_TABLE_EX
  IIO_SLOT_CONFIG_DATA_ENTRY_EX        *SlotTableEx = NULL;
  UINT8                                SlotEntries;
  UINT8                                BoardId;
  BOOLEAN                              AutoBifEnable[MaxIIO * MAX_IOU_PER_SOCKET];

  BoardId = SystemBoardIdValue ();
  SetMem ((VOID *)AutoBifEnable, MaxIIO * MAX_IOU_PER_SOCKET, FALSE);

  for (IioIndex = Iio_Socket0; IioIndex < MaxIIO; IioIndex++) {
    for (IouIndex = Iio_Iou0; IouIndex < MAX_IOU_PER_SOCKET; IouIndex++) {
      if (IioGlobalData->SetupData.ConfigIOU[IioIndex][IouIndex] == IIO_BIFURCATE_AUTO) {
        AutoBifEnable[(IioIndex * MAX_IOU_PER_SOCKET) + IouIndex] = TRUE;
      } // if
    } // for IouIndex
  } // for IioIndex

  // This function outline:
  // 1. Based on platform apply the default bifurcation and slot configuration.
  // 2. Apply dynamic overrides based on GPIO and other configurations.
  // 3. Hide unused ports due bifurcation.
  Status = GetPlatformIioTable (IioGlobalData, &IioConfigTable);
  if (EFI_ERROR(Status)) {
    return;
  }
  if ((IioConfigTable.Version == PLATFORM_IIO_CONFIG_UPDATE_VERSION_2) ||
     (IioConfigTable.Version == PLATFORM_IIO_CONFIG_UPDATE_VERSION_3)) { // for 10nm
    if (IioConfigTable.CallUpdate != NULL) {
      IioConfigTable.CallUpdate (IioGlobalData);
    }
    if (IioConfigTable.Version == PLATFORM_IIO_CONFIG_UPDATE_VERSION_3) {
      if (IioConfigTable.RetimerInitTablePtr != NULL) {
        IioRetimerInit (IioGlobalData, IioConfigTable.RetimerInitTablePtr);
      }
    }
    BifurcationTableEx = IioConfigTable.IioBifurcationTablePtr;
    BifurcationEntries = (UINT8) (IioConfigTable.IioBifurcationTableSize / sizeof (IIO_BIFURCATION_DATA_ENTRY_EX));
    SlotTableEx = IioConfigTable.IioSlotTablePtr;
    SlotEntries = (UINT8) (IioConfigTable.IioSlotTableSize / sizeof (IIO_SLOT_CONFIG_DATA_ENTRY_EX));

    SetBifurcations (IioGlobalData, BifurcationTableEx, BifurcationEntries, AutoBifEnable);
    if (BoardId <= TypePlatformMax) {
      OverrideDefaultBifSlots (IioGlobalData);
    }
    SystemHideIioPortsCommon (IioGlobalData);   // hide ports according to overrided IOU config.
    ConfigSlots (IioGlobalData, SlotTableEx, SlotEntries);
    OverrideConfigSlots (IioGlobalData, SlotTableEx, SlotEntries);
  }
}

IIO_PORT_BIFURCATION_INIT_TABLE    IioPortBifurcationInitTable =
{
  IIO_PORT_BIFURCATION_INIT_SIGNATURE,
  IIO_PORT_BIFURCATION_INIT_VERSION,
  IioPortBifurcationInitCallback
};

EFI_STATUS
InstallIioPortBifurcationInitData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
)
{
  EFI_STATUS                            Status = EFI_SUCCESS;

   Status = UbaConfigPpi->AddData (
                                  UbaConfigPpi,
                                  &gIioPortBifurcationInitDataGuid,
                                  &IioPortBifurcationInitTable,
                                  sizeof(IioPortBifurcationInitTable)
                                  );

  return Status;
}

