/** @file
  Miscellaneous PCIE features implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <Chip/Include/SysHostChip.h>
#include <Library/IioAccessLib.h>
#include <Library/IioDataHubLib.h>
#include <PcieRegs.h>
#include <Library/IioDataHubLib.h>

#include <LibraryPrivate/IioDebug.h>
#include <LibraryPrivate/IioPcieLib.h>


/**

  Determines if the PCIe slots have devices connected or not
  and disabled the link

  @param IioGlobalData  - Pointer to UDS
  @param IioIndex     - index to CPU/IIO

  @retval NONE
**/
VOID
IioDisableLinkPorts (
  IN  IIO_GLOBALS            *IioGlobalData,
  IN  UINT8                  IioIndex
  )
{
  SECBUS_IIO_PCIE_G5_STRUCT       Scbn;
  PRIBUS_IIO_PCIE_G5_STRUCT       Pbn;
  SUBBUS_IIO_PCIE_G5_STRUCT       Sbbn;
  UINT8                           PortIndex;
  UINT8                           MaxPortNumberPerSocket;
  UINT16                          Present;
  PCI_REG_PCIE_LINK_CONTROL       LnkCon;
  UINT8                           PCIeCapOffset;
  IIO_CONFIG                      *SetupData;
  PCI_ROOT_BRIDGE_PCI_ADDRESS     PciAddress;

  SetupData = GetIioConfig ();
  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);
  for (PortIndex = 1; PortIndex < MaxPortNumberPerSocket; PortIndex++) {
    //
    // 5333019: BIOS Disables PCIe Link for Slot 2 When Hot Plug Enabled and Slot Empty At G3 Exit
    //
    // Check whether the PCIe port can be configured
    if ((IioVData_ConfigurePciePort (IioGlobalData, IioIndex, (UINT8)PortIndex) == FALSE) ||
       (CheckVarPortEnable (IioGlobalData, IioIndex, PortIndex) == TRUE) ||
       IioIsHpEnabledOnPort (IioIndex, PortIndex) ||
       (SetupData->PEXPHIDE[(IioIndex * MaxPortNumberPerSocket) + PortIndex] == TRUE) ) {
      continue;
    }

    GetPciePortBusDeviceFunction (IioIndex, PortIndex, &PciAddress);
    IIO_D_PCIDBG ("IioDisableLinkPorts:%d Port:%d Bus:%x Dev:%d Func:%d \n",
      IioIndex, PortIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function);

    PCIeCapOffset = IioPCIeGetCapOffset (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
      EFI_PCI_CAPABILITY_ID_PCIEXP);
    if (!PCIeCapOffset) {
      IIO_D_PCIERR ("IioDisableLinkPorts Can't find PCIeCapOffset!\n");
      ASSERT (PCIeCapOffset);
    } else {
      Scbn.Data = IioReadCpuCsr8 (IioIndex, PortIndex, SECBUS_IIO_PCIE_G5_REG);
      Pbn.Data = IioReadCpuCsr8 (IioIndex, PortIndex, PRIBUS_IIO_PCIE_G5_REG);
      Sbbn.Data = IioReadCpuCsr8 (IioIndex, PortIndex, SUBBUS_IIO_PCIE_G5_REG);
      //
      // Check if there are EndPoint connected in the downstream buses
      //
      if (Scbn.Data > Pbn.Data ) {
        for (PciAddress.Bus = Scbn.Data; PciAddress.Bus <= Sbbn.Data; PciAddress.Bus++) {
           //
           // Check if there is someone in the Dev 0 Fun 0 if not then there is nobody
           //
           Present = IioPciExpressRead16 ( IioIndex, PciAddress.Bus, 0, 0, 0);
           if (Present == 0xFFFF) {
             LnkCon.Uint16 = IioPciExpressRead16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
               PCIeCapOffset + R_PCIE_LCTL_OFFSET);
             LnkCon.Bits.LinkDisable = 1;
             IioPciExpressWrite16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
               PCIeCapOffset + R_PCIE_LCTL_OFFSET, LnkCon.Uint16);
             IIO_D_PCIERR ("No PCIe devices connected to IIO%d, PortIndex = %X Disabling link\n",IioIndex, PortIndex);
          } else {
             break;
          }
        }
      }
    }
  }
}

/**
  This function will hide rootports based on PEXPHIDE Settings

  @param[in]     IioGlobalData  Pointer to IIO_GLOBALS
  @param[in]     IioIndex       Socket/IIO complex index
  @param[in out] IioDevFuncHide Pointer to table with devices to hide
**/
VOID
IioRootportsHide (
  IN      IIO_GLOBALS                   *IioGlobalData,
  IN      UINT8                         IioIndex,
  IN OUT  IIO_DEVFUNHIDE_TABLE          *IioDevFuncHide
  )
{
  UINT8                                 PortIndex;
  UINT8                                 BusNumber;
  UINT8                                 PortStack;
  UINT8                                 Stack = 0;
  UINT8                                 FuncX = 0;
  UINT8                                 DevNum = 0;
  UINT8                                 MaxPortNumberPerSocket;
  IIO_CONFIG                            *SetupData;
  IIO_V_DATA                            *IioVData;
  EFI_STATUS                            Status;

  IIO_D_PCIDBG ("IioInit PCIe device hide..\n");

  SetupData = GetIioConfig ();
  IioVData = GetIioVData ();

  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);

  //
  // If statements to resolve KW issue
  //
  if (IioIndex >= MaxIIO || MaxPortNumberPerSocket > NUMBER_PORTS_PER_SOCKET) {
    ASSERT (FALSE);
    return;
  }

  for (PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {
    BusNumber = GetSocketPortBusNumber (IioIndex, PortIndex);

    Status = GetPcieStackPerPort (IioIndex, PortIndex, &Stack);
    if (Status != EFI_SUCCESS) {
      continue;
    }

    //
    // Skip ports if stack is disabled
    //
    if (!(IioVData->StackPresentBitmap[IioIndex] & (1 << Stack))) {
      SetupData->PEXPHIDE[(IioIndex * MaxPortNumberPerSocket) + PortIndex] = 1;
      continue;
    }

    //
    // Check if IioRootPort is populated otherwise hide the port.
    //
    if (!CheckPciePortEnable (IioGlobalData, IioIndex, PortIndex)) {
      SetupData->PEXPHIDE[(IioIndex * MaxPortNumberPerSocket) + PortIndex] = 1;
    }

    //
    // Per PCI specification, if any function is present on a device, then Device/Function 0 must be present as well
    //
    if (!IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPortActive)) {   //Skip DMI port
      if (!(PortIndex - (MAX_IOU_PORT_DEVICES * Stack) - 1)) {
        for (PortStack = IioPortB; PortStack < MAX_IOU_PORT_DEVICES; PortStack++) {
          if (!(SetupData->PEXPHIDE[(IioIndex * MaxPortNumberPerSocket) + PortIndex + PortStack])) {
            SetupData->PEXPHIDE[(IioIndex * MaxPortNumberPerSocket) + PortIndex] = 0;
          } // End of if (!(SetupData->PEXPHIDE[(IioIndex * MaxPortNumberPerSocket) + PortIndex + PortStack]))
        } // End of for (PortStack = IioPortB; PortStack < MAX_IOU_PORT_DEVICES; PortStack++)
      } // End of if (!(PortIndex - (MAX_IOU_PORT_DEVICES * Stack) - 1))
    } // End of if (!IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPortActive))

    if (IioVData->StackPersonality[IioIndex][Stack] == TYPE_DISABLED) {
      SetupData->PEXPHIDE[(IioIndex * MaxPortNumberPerSocket) + PortIndex] = 1;
    }

    DevNum = GetPciePortDevice (PortIndex);
    FuncX = GetPciePortFunction (PortIndex);

    //
    // Include to IioDevFuncHide the PCIE root ports which will be hidden
    //
    if (SetupData->PEXPHIDE[(IioIndex * MaxPortNumberPerSocket) + PortIndex]) {
      IioDevFuncHide[IioIndex].IioStackDevHide[Stack].DevToHide[FuncX] |= (1 << DevNum);
      IIO_D_PCIDBG ("Bus=x%02X, Device=%d, Function=%d will be hidden from M2ISOF\n", BusNumber, DevNum, FuncX);
    } // end of if PEXPHIDE == 1
  } // foreach PortIndex
}


/**
    This function will initialize PCIe Error Injection

    @param IioGlobalData - Pointer to IIO_GLOBALS
    @param IioIndex      - Index to CPU/IIO
    @param IioIndex      - Index to PCIe port

    @retval None
**/
VOID
PcieErrInjConInit(
  IN  UINT8          IioIndex,
  IN  UINT8          PortIndex
  )
{
  //TODO:  Implement when 0p8 EIP register drop is avaliable
}

/**
 Gets and print rx stats per lane

    @param IioGlobalData        - Pointer to IioGlobalData
**/
VOID
ShowRxStats (
  IN  IIO_GLOBALS     *IioGlobalData
  )
{
  ShowRxStatsPrivate (IioGlobalData);
}

