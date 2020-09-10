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
#include <Base.h>
#include <Uefi.h>

#include <RcRegs.h>

#include <IioPlatformData.h>
#include <IioSetupDefinitions.h>

#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/OemIioInit.h>
#include <Library/PcieCommonInitLib.h>
#include <Library/PcdLib.h>
#include <Library/CpuAndRevisionLib.h>

#include <ConfigBlock/TraceHubConfig.h>

#include <Library/IioAccessLib.h>
#include <Library/IioSysInfoLib.h>
#include <Library/IioPcieInit.h>
#include <Library/IioNtbInit.h>

#include <Library/PchPcrLib.h>

#include <Chip/Include/CpuPciAccess.h>
#include <Cpu/CpuCoreRegs.h>
#include <PcieRegs.h>
#include <Guid/IioIpInterface.h>
#include <Library/PcuIpLib.h>
#include <Library/IioDataHubLib.h>
#include <Library/KtiApi.h>
#include <LibraryPrivate/IioDebug.h>


UINT8 ITC_ALL[]     = {Ch0, Ch1, Ch2, Ch3, Ch4, Ch5, Ch6, Ch7, LastChannel};
UINT8 ITC_CH0[]     = {Ch0, LastChannel};
UINT8 ITC_DEFAULT[] = {Ch0, Ch2, Ch3, Ch4, Ch5, LastChannel};
UINT8 ITC_DMI[]     = {Ch0, Ch1, Ch2, Ch3, LastChannel};
UINT8 ITC_2xPCIe[]  = {Ch0, Ch1, Ch2, Ch3, Ch4, Ch5, Ch6, Ch7, LastChannel};
UINT8 ITC_PORT0[]   = {Ch0, Ch1, Ch2, Ch3, LastChannel};
UINT8 ITC_PORT1[]   = {Ch4, Ch5, Ch6, Ch7, LastChannel};
UINT8 ITC_TIP_FEC[] = {Ch0, Ch1, LastChannel};

typedef struct _CPU_STACK_CHS_TABLE {
  UINT8 *Stack[MAX_IIO_STACK];
} CPU_STACK_CHS_TABLE;
//
// TODO: This table needs to be updated with SPR and any new CPU.
//
CPU_STACK_CHS_TABLE mCpuStackChannel[] = {
  {ITC_CH0,     ITC_CH0,    ITC_CH0,    ITC_CH0,    ITC_CH0,     ITC_CH0   },  // CPU_SKX
  {ITC_CH0,     ITC_CH0,    ITC_CH0,    ITC_CH0,    ITC_CH0,     ITC_CH0   },  // CPU_CLX
  {ITC_CH0,     ITC_CH0,    ITC_CH0,    ITC_CH0,    ITC_CH0,     ITC_CH0   },  // CPU_CPX
  {ITC_DEFAULT, ITC_PORT1,  ITC_PORT1,  ITC_PORT1,  ITC_PORT1,   NULL      },  // CPU_SNR
  {ITC_DMI,     ITC_PORT0,  ITC_PORT0,  ITC_PORT0,  ITC_PORT0,   ITC_PORT0 },  // CPU_ICXSP
  {ITC_DEFAULT, ITC_PORT1,  ITC_PORT1,  ITC_PORT0,  ITC_TIP_FEC, ITC_PORT0 },  // CPU_ICXD
  {NULL,        NULL,       NULL,       NULL,       NULL,        NULL      },  // CPU_SPRSP
  {NULL,        NULL,       NULL,       NULL,       NULL,        NULL      },  // CPU_TNR
};

/**
  This function provides IioGlobalData structure pointer retrieves it from HOB if necessary

  @param IioGlobalData - Pointer to IioGlobalData

  @retval EFI_SUCCESS       If IioGlobalData pointer retrieved successfully.
  @retval EFI_NOT_FOUND     If IioSiPolicyHob is not found.

**/
EFI_STATUS
GetIioGlobalData (
  IN OUT IIO_GLOBALS        **IioGlobalData
  )
{
  return GetIioGlobalDataEx (IioGlobalData, FALSE);
}

/**
  This function provides IioGlobalData structure pointer retrieves it from HOB if necessary

  @param IioGlobalData - Pointer to IioGlobalData
  @param FetchFromHob  - allow to override saved pointer find the HOB again

  @retval EFI_SUCCESS       If IioGlobalData pointer retrieved successfully.
  @retval EFI_NOT_FOUND     If IioSiPolicyHob is not found.

**/
EFI_STATUS
GetIioGlobalDataEx (
  IN OUT IIO_GLOBALS        **IioGlobalData,
  IN     BOOLEAN            FetchFromHob
  )
{
  //
  // STATIC not works in NEM - this is just RO var. then
  //
  STATIC IIO_GLOBALS* sIioGlobalData = NULL;
  EFI_HOB_GUID_TYPE *GuidHob;

  if (sIioGlobalData == NULL || FetchFromHob) {

    GuidHob = GetFirstGuidHob (&gIioSiPolicyHobGuid);
    if (GuidHob == NULL) {
      DEBUG ((EFI_D_ERROR, "[Iio] ERROR: IioPrivateDataAccess: Failed to get gIioSiPolicyHobGuid Hob!\n"));
      ASSERT (FALSE);
      return EFI_NOT_FOUND;
    }

    *IioGlobalData = (IIO_GLOBALS *) GET_GUID_HOB_DATA (GuidHob);
    sIioGlobalData = *IioGlobalData; // this can't work in NEM - sIioGlobalData is RO
    return EFI_SUCCESS;
  } else {
    *IioGlobalData = sIioGlobalData;
    return EFI_SUCCESS;
  }
}

/**
  This function verifies if Iio Port is present and update
  PciePortConfig

  @param[in]  IioGlobalData - Pointer to IioGlobalData
  @param[in]  IioIndex      - IIO instance index (usually socket)
**/
VOID
IioPreLinkDataInitPerSocket (
  IIO_GLOBALS  *IioGlobalData,
  UINT8        IioIndex
  )
{
  UINT8   PortIndex;
  UINT8   BusBase;
  UINT8   Device = 0;
  UINT8   Function = 0;
  UINT8   StackIndex;
  UINT8   MaxPortNumberPerSocket;
  UINT16  VidPort;
  CHAR8  *StsStrPtr = "?";

  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];
  for (PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {
    //
    // Dmi on SPR has no PCI device instance. Its PciePortConfig should be hard code to enabled.
    //
    if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPortActive)) {
      if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
        if (((BIT0 << IioIndex) & GetPchPresentBitMap()) != 0) {
          IioGlobalData->IioVar.IioOutData.PciePortConfig[IioIndex * MaxPortNumberPerSocket] = 0x01;
          StsStrPtr = "enabled";
          IIO_D_PCILOG ("[%d p%d] DMI device is %a\n", IioIndex, PortIndex, StsStrPtr);
        }
        continue;
      }
    }
    //
    // Disables port config if stack is not present
    //
    StackIndex = IioGlobalData->IioVar.IioVData.StackPerPort[IioIndex][PortIndex];
    if (!IioIsStackPresent (IioIndex, StackIndex)) {
      IIO_D_PCILOG ("Socket= %x, Stack = %x, PciePortConfig[%x] disabled. Stack not present\n", IioIndex, StackIndex, PortIndex);
      IioGlobalData->IioVar.IioOutData.PciePortConfig[(IioIndex*MaxPortNumberPerSocket) + PortIndex] = 0x00;
      SetPEXPHideForNotConfiguredPorts (IioIndex, PortIndex, IioGlobalData);
      continue;
    }
    BusBase = IioGlobalData->IioVar.IioVData.SocketPortBusNumber[IioIndex][PortIndex];
    Device = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].Device;
    Function = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].Function;

    StackIndex = IioGlobalData->IioVar.IioVData.StackPerPort[IioIndex][PortIndex];

    IioGlobalData->IioVar.IioOutData.PciePortConfig[(IioIndex*MaxPortNumberPerSocket) + PortIndex] = 1;

    //
    // If the Port is not present (e.g. Vendor ID is not Intel, then the port doesn't exist and should not be configured).
    // This condition could exist on some SKUs where certain IOUs are not present or if some external entity has disabled a port.
    // For Gen4 IP, NTB is on a different BDF than previous generations so this must be checked to properly configure NTB ports.
    //
    VidPort = IioPciExpressRead16 (IioIndex, BusBase, Device, Function, PCI_VENDOR_ID_OFFSET);
    if ((VidPort == 0xFFFF) && !IsNtbSbActive (IioIndex, PortIndex)) {
      //
      // if applies, set PciePortConfig for Rlink
      //
      if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPortActive) && HasRlink ()) {

        IioGlobalData->IioVar.IioOutData.PciePortConfig[(IioIndex*MaxPortNumberPerSocket) + PortIndex] = 0x01;
        StsStrPtr = "active DMI/Rlink";

      } else {

        IioGlobalData->IioVar.IioOutData.PciePortConfig[(IioIndex*MaxPortNumberPerSocket) + PortIndex] = 0x00;
        StsStrPtr = "disabled (not present)";
        SetPEXPHideForNotConfiguredPorts (IioIndex, PortIndex, IioGlobalData);
      }
    } else {
      IioGlobalData->IioVar.IioOutData.PciePortPresent[(IioIndex*MaxPortNumberPerSocket) + PortIndex] = 0x01;
      StsStrPtr = "enabled";
    }
    IIO_D_PCILOG ("[%d.%d p%d] %02X:%02X:%02X.%X: PCI device %04X:%04X is %a\n", IioIndex, StackIndex, PortIndex,
                  IioGlobalData->IioVar.IioVData.SegmentSocket[IioIndex], BusBase, Device, Function,
                  IioPciExpressRead16 (IioIndex, BusBase, Device, Function, PCI_VENDOR_ID_OFFSET),
                  IioPciExpressRead16 (IioIndex, BusBase, Device, Function, PCI_DEVICE_ID_OFFSET),
                  StsStrPtr);
  }
}


/**
  Helper routine which physically varies the port status

  @param Param1: protocol based PCI address for the root port device

  @retval boolean indicating TRUE for port active or FALSE for not active

**/
BOOLEAN
IsPciePortDLActive (
  IN  UINT8          IioIndex,
  IN  UINT8          PortIndex
  )
{
  PCI_REG_PCIE_LINK_STATUS         LnkSts;
  UINT8                            PCIeCapOffset;
  PCI_ROOT_BRIDGE_PCI_ADDRESS      PciAddress;

  GetPciePortBusDeviceFunction (IioIndex, PortIndex, &PciAddress);
  IIO_D_PCILOG ("IsPciePortDLActive:%d Port:%d Bus:%x Dev:%d Func:%d Slot Power...\n",
    IioIndex, PortIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function);

  PCIeCapOffset = IioPCIeGetCapOffset (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
    EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (PCIeCapOffset == 0) {
    IIO_D_PCIERR ("IsPciePortDLActive Can't find PCIeCapOffset!\n");
    ASSERT (PCIeCapOffset != 0);
    return FALSE;
  } else {
    LnkSts.Uint16 = IioPciExpressRead16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
      PCIeCapOffset + R_PCIE_LSTS_OFFSET);
    if (LnkSts.Bits.DataLinkLayerLinkActive != 0) {
      return  TRUE;
    } else {
      return  FALSE;
    }
  }
}

/**
  Check if PCIE Gen4 Supported or not.

  @retval TRUE  - CPU Type supports PCIE Gen4.
  @retval FALSE - CPU Type do not support PCIE Gen4.
**/
BOOLEAN
IsPCIeGen4 (
  VOID
  )
{
  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    return TRUE;
  } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    return TRUE;
  } else if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    return TRUE;
  }
  return FALSE;
}

/**
  Returns generation of the PCIE port.

  @param[in]  IioGlobalData - Pointer to IIO global data
  @param[in]  IioIndex      - Socket Index
  @param[in]  PortIndex     - Port Index

  @retval The ports supported Pcie controller generation.
**/
IIO_PORT_GEN
IioPortControllerGen (
  IN  IIO_GLOBALS *IioGlobalData,
  IN  UINT8        IioIndex,
  IN  UINT8        PortIndex
  )
{

  if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    if (PortIndex <= PORT_5D_INDEX) {
      return IIO_GEN3;
    }
  }
  if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort)) {
      return IIO_GEN3;
    } else if (PortIndex <= PORT_5D_INDEX_5) {
      return IIO_GEN4;
    }
  }
  if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
    if (PortIndex <= PORT_1D_INDEX_S1) {
      return IIO_GEN3;
    }
  }
  if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort)) {
      return IIO_GEN3;
    } else if (PortIndex <= PORT_2D_INDEX_1) {
      return IIO_GEN4;
    }
  }
  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort)) {
      //
      // DMI port is gen4 in SPR.
      //
      return IIO_GEN4;
    } else if (PortIndex <= PORT_5H_INDEX_SPR) {
      if ((PortIndex & 0x1) == 0x0) {
        //
        // Gen4 ports have even indexes
        //
        return IIO_GEN4;

      } else {
        //
        // Gen5 ports have odd indexes
        //
        return IIO_GEN5;
      }
    }
  }
  return IIO_GEN_MAX;
}

/**
  Get the stack which the PCIe port belongs to

  @param[in] IioIndex       Socket Index
  @param[in] PortIndex      Port Index

  @retval  Stack number
**/
UINT8
IioGetStackForPort (
  IN UINT8     IioIndex,
  IN UINT8     PortIndex
  )
{
  IIO_GLOBALS            *IioGlobalData;
  EFI_STATUS              Status;

  Status = GetIioGlobalData (&IioGlobalData);

  return IioGlobalData->IioVar.IioVData.StackPerPort[IioIndex][PortIndex];
}

/**
  Returns true if given socket is present in current configuration
  For now this is only wrapper on IioGlobal var.

  @param[in]  IioIndex  Index of the socket
  @retval  TRUE   if given socket is present
  @retval  FALSE  otherwise
 */
BOOLEAN
IioIsSocketPresent (
  IN  UINT8  IioIndex
  )
{
  IIO_GLOBALS            *IioGlobalData;

  if (EFI_ERROR (GetIioGlobalData (&IioGlobalData))) {
    return FALSE;
  }
  return IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex] != 0;
}

/*
  Return if the stack indexed is present or not

  @param[in] IioIndex      - Socket Index
  @param[in] StackIndex    - Stack Index

  @retval    TRUE       Stack present
  @retval    FALSE      Stack not present
**/
BOOLEAN
IioIsStackPresent (
  IN UINT8     IioIndex,
  IN UINT8     StackIndex
  )
{
  return IfStackPresent (IioIndex, StackIndex);
}
/**
    Check if stack of port present or not

    @param IioIndex      - Socket Index
    @param PortIndex     - Port Index

    @retval TRUE  - Stack is present
    @retval FALSE - Stack is not present

**/
BOOLEAN
IioIsPortPresent (
  IN UINT8          IioIndex,
  IN UINT8          PortIndex
  )
{
  UINT8    Stack;

  Stack = IioGetStackForPort (IioIndex, PortIndex);

  return IioIsStackPresent (IioIndex, Stack);
}

/**

    Check if current port is TPH (TLP Processing hints) capable

    @param IioGlobalData - Pointer to IIO globals
    @param IioIndex      - Current socket.
    @param PortIndex     - Current port

    @retval TRUE  - Port is TPH capable.
    @retval FALSE - Port is not TPH capable.

**/
BOOLEAN
IsPciePortTPHCapable (
  IN  UINT8              IioIndex,
  IN  UINT8              PortIndex
  )
{
  PCI_REG_PCIE_DEVICE_CAPABILITY2  DevCap2;
  UINT8                            PCIeCapOffset;
  PCI_ROOT_BRIDGE_PCI_ADDRESS      PciAddress;
  UINT16                           VendorID;
  UINT16                           DeviceID;

  GetPciePortBusDeviceFunction(IioIndex, PortIndex, &PciAddress);

  VendorID = (UINT16)IioPciExpressRead16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function, PCI_VENDOR_ID_OFFSET);
  DeviceID = (UINT16)IioPciExpressRead16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function, PCI_DEVICE_ID_OFFSET);


  IIO_D_PCILOG ("IsPciePortTPHCapable:%d Port:%d Bus:%x Dev:%d Func:%d \n", IioIndex,
    PortIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function);

  if ((VendorID == 0xFFFF) || (DeviceID == 0xFFFF)) {
    //
    // Rlink has VID and DID 0xFFFF, avoid printing ERROR message
    //
    if (!IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort)) {
      IIO_D_PCIERR ("Skipping for invalid PCIe ports... IIO[%x] PCIE PortIndex:%x...\n", IioIndex, PortIndex);
    }
    return FALSE;
  }

  PCIeCapOffset = IioPCIeGetCapOffset (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
    EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (PCIeCapOffset == 0) {
    IIO_D_PCIERR ("IsPciePortTPHCapable Can't find PCIeCapOffset!\n");
    ASSERT (PCIeCapOffset != 0);
    return FALSE;
  } else {
    DevCap2.Uint32 = IioPciExpressRead32 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
      PCIeCapOffset + R_PCIE_DCAP2_OFFSET);
    if (DevCap2.Bits.TphCompleter != 0) {      //check the TPH completer capability
      return TRUE;
    } else {
      return FALSE;
    }
  }
}


/**
  Determines if the PCIe slots have devices connected or not.

  @param IioGlobalData  - Pointer to IIO_GLOBALS
  @param IioIndex            - Index to IIO

  @retval TRUE  - if one or more PCIe slots are populated
  @retval FALSE - if all the PCIe slots for that IIO are open / empty

**/
BOOLEAN
IsIioPortPopulated (
  IN  IIO_GLOBALS         *IioGlobalData,
  IN  UINT8               IioIndex,
  IN  UINT8               PortIndex
  )
{
  BOOLEAN                 IioPortsActive = FALSE;

  if (!IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort)) {
    //
    // Check whether the PCIe port can be configured
    //
    if (IioVData_ConfigurePciePort (IioGlobalData, IioIndex, PortIndex)) {
      //
      // Is data link layer active?
      //
      if (IsPciePortDLActive (IioIndex, PortIndex)) {
        IioPortsActive = TRUE;
      }
    }
  } else if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPortActive)) {

    IioPortsActive = TRUE;
  }
  if (!IioPortsActive) {
    IIO_D_PCIWARN ("[%d p%d] No PCIe device connected\n", IioIndex, PortIndex);
  }
  return IioPortsActive;
}


/**
    This functions indicates if the IIO Port can be configured or not.

    @param IioGlobalData - Pointer to IIO_GLOBALS
    @param IioIndex      - Index to Iio
    @param PortIndex     - Index to Iio Root Port

    @retval TRUE         - if Iio Port can be configured
    @retval FALSE        - if Iio Port CAN not be configured.

**/
BOOLEAN
IioVData_ConfigurePciePort (
  IN  IIO_GLOBALS                     *IioGlobalData,
  IN  UINT8                           IioIndex,
  IN  UINT8                           PortIndex
  )
{
  UINT8   MaxPortNumberPerSocket;

  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];

  //
  // For CPX, mark DMI port as not configurable if it will not be used as DMI
  // (not DMI == PCIe)
  //
  if (IsThisCpx () && (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPortAsPci) == TRUE)) {
      IIO_D_DMILOG ("[%d][%d] Port not used as DMI, will not be enabled as PCIe\n", IioIndex, PortIndex);
    return FALSE;
  }

  return (IioGlobalData->IioVar.IioOutData.PciePortConfig[(IioIndex * MaxPortNumberPerSocket) + PortIndex] == 0)  \
          ? FALSE  : TRUE;
}

/**

    This function return the current port Link Width.

    @param IioGlobalData - Pointer to IIO_GLOBALS
    @param IioIndex      - Index to Iio
    @param PortIndex     - Index to Iio Root Port

    @retval IioGlobalData->IioVar.IioOutData.CurrentPXPMap[(Iio*MaxPortNumberPerSocket)+PortIndex] - Current bifurcation value.

**/
UINT8
GetAssignedPortIndexWidth (
  IN  IIO_GLOBALS             *IioGlobalData,
  IN  UINT8                  IioIndex,
  IN  UINT8                  PortIndex
  )
{
  UINT8   MaxPortNumberPerSocket;

  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];
  return IioGlobalData->IioVar.IioOutData.CurrentPXPMap[(IioIndex*MaxPortNumberPerSocket)+PortIndex];
}


/**
    This function returns the bifurcation value of the Port specified by PortIndex and IIoIndex.

    @param IioGlobalData - Pointer to IIO_GLOBALS
    @param IioIndex      - Index to Iio
    @param PortIndex     - Index to Iio Root Port

    @return Bifurcation is returned, one of IIO_BIFURCATE_<*> definitions.
**/
UINT8
CheckBifurcationMapPort (
  IN IIO_GLOBALS    *IioGlobalData,
  IN  UINT8          IioIndex,
  IN  UINT8          PortIndex
  )
{
  UINT8    Iou;
  UINT8    MaxPortNumPerIou;
  UINT8    MaxIouNumPerSocket;
  UINT8    Bifurcation;

  Bifurcation = 0;

  if (IioIndex >= ARRAY_SIZE(IioGlobalData->SetupData.ConfigIOU)) {

    ASSERT (FALSE);
    return Bifurcation;
  }
  if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort)) {

    if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
      return IIO_BIFURCATE_xxx8xxx8;
    } else {
      return Bifurcation;
    }
  }
  MaxPortNumPerIou = GetMaxPortNumPerIou ();
  MaxIouNumPerSocket = GetMaxIouNumPerSocket ();
  if (MaxPortNumPerIou > 0) {

    Iou = (PortIndex - 1) / MaxPortNumPerIou;
    ASSERT (Iou < MaxIouNumPerSocket);
    ASSERT (Iou < ARRAY_SIZE(IioGlobalData->SetupData.ConfigIOU[0]));
    Bifurcation = IioGlobalData->SetupData.ConfigIOU[IioIndex][Iou];
  }
  return Bifurcation;
}

/**

    This function checks if IIO port is enabled or not.

    @param IioGlobalData - Pointer to IIO_GLOBALS
    @param IioIndex      - Index to Iio
    @param PortIndex     - Index to Iio Root Port

    @retval PCI_PORT_ENABLE         - to indicate port is to be disabled
    @retval PCI_PORT_DISABLE        - to indicate port is to be enabled
    @retval PCI_PORT_AUTO           - to indicate port is to be auto


**/
UINT8
CheckVarPortEnable (
  IN  IIO_GLOBALS           *IioGlobalData,
  IN  UINT8                 IioIndex,
  IN  UINT8                 PortIndex
  )
{
  UINT8 PciePortEnable;
  UINT8 MaxPortNumberPerSocket;

  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];
  PciePortEnable = \
      IioGlobalData->SetupData.IioPcieConfig.PciePortEnable[((IioIndex) * MaxPortNumberPerSocket) + PortIndex];

  if (PciePortEnable == IIO_OPTION_ENABLE) {
    return PCIE_PORT_ENABLE;
  }

  if (PciePortEnable == IIO_OPTION_DISABLE) {
    return PCIE_PORT_DISABLE;
  }

  return PCIE_PORT_AUTO;
}


/**
    This function checks if PCIE port is enabled or not.

    @param IioGlobalData - Pointer to IIO_GLOBALS
    @param IioIndex      - Index to Iio
    @param PortIndex     - Index to Iio Root Port

    @retval TRUE         - to indicate port is to be disabled
    @retval FALSE        - to indicate port is to be enabled
**/
BOOLEAN
CheckPciePortEnable (
  IN  IIO_GLOBALS           *IioGlobalData,
  IN  UINT8                 IioIndex,
  IN  UINT8                 PortIndex
  )
{
  UINT8   PciePortEnable;
  UINT8   VarPortEnable;

  PciePortEnable = TRUE;

  if (!CheckNtbPortConfSetting (IioIndex, PortIndex) &&
      !IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort)) {

    VarPortEnable = CheckVarPortEnable (IioGlobalData, IioIndex, PortIndex);

    switch (VarPortEnable) {
      case PCIE_PORT_ENABLE:
        break;
      case PCIE_PORT_AUTO:
        if (!IsIioPortPopulated (IioGlobalData, IioIndex, PortIndex)) {
          if (!IioIsHpEnabledOnPort (IioIndex, PortIndex)) {
            PciePortEnable = FALSE;
          }
        }
        break;
      case PCIE_PORT_DISABLE:
      default:
        PciePortEnable = FALSE;
        break;
    }
  }
  return PciePortEnable;
}

/**
  Returns ASPM value for given IIO port based on global ASPM override and per-port configuration
  If the ASPM shouldn't be changed previous value is returned.

    @param IioGlobalData - Pointer to IIO_GLOBALS
    @param IioIndex      - Index to Iio
    @param PortIndex     - Index to Iio Root Port

    @retval PCIE_LIB_ASPM_POLICY - ASPM value to set in for port
**/
UINT8
GetAspmSettingForPort (
  IN  IIO_GLOBALS      *IioGlobalData,
  IN  UINT8            IioIndex,
  IN  UINT8            PortIndex
  )
{
  UINT8         StackIndex;
  IIO_CONFIG    *IioSetup;

  IioSetup = GetIioConfig();

  StackIndex = IioGlobalData->IioVar.IioVData.StackPerPort[IioIndex][PortIndex];

  if (!IsDmiStack (StackIndex)) {
    switch (IioSetup->PcieGlobalAspm) {
    case PCIE_GLOBAL_ASPM_DISABLE:
      //
      // ASPM is globally disabled for IIO
      //
      return PcieLibAspmDisabled;
    case PCIE_GLOBAL_ASPM_PER_PORT:
      //
      // ASPM is configured "per-port" - no global override
      //
      break;
    case PCIE_GLOBAL_ASPM_L1_ONLY:
      //
      // ASPM is override as L1-only for all IIO ports
      //
      return PcieLibAspmL1;
    default:
      ASSERT (FALSE);
      return PCIE_ASPM_AUTO;
    }
  }

  //
  // Return value set for given port
  //
  switch (IioSetup->IioPcieConfig.PcieAspm[IioIndex][PortIndex]) {
  case PCIE_ASPM_AUTO:
    return PCIE_ASPM_AUTO;
  case PCIE_ASPM_L1_ONLY:
    return PcieLibAspmL1;
  case PCIE_ASPM_DISABLE:
    return PcieLibAspmDisabled;
  default:
    ASSERT (FALSE);
    return PCIE_ASPM_AUTO;
  }
}

/**

  Helper routine which returns the PCI header class code value from the derived BDF
  out of logical index of socket & port

  @param IioGlobalData - IIO Global data structure
  @param IioIndex      - Socket Index
  @param PortIndex     - Port Index

  @retval Class code value from PCI header

**/
UINT32
GetPortClassCode (
  IN IIO_GLOBALS   *IioGlobalData,
  IN UINT8          IioIndex,
  IN UINT8          PortIndex
  )
{
  UINT32  ClassCode = 0;
  UINT8   Bus;
  UINT8   Device;
  UINT8   Function;

  Bus       = IioGlobalData->IioVar.IioVData.SocketPortBusNumber[IioIndex][PortIndex];
  Device    = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].Device;
  Function  = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].Function;
  ClassCode = (UINT32)(IioPciExpressRead32 (IioIndex, Bus, Device, Function, PCI_REVISION_ID_OFFSET) >> 8);

  return ClassCode;
}

/**

  Helper routine which returns the PCI header device ID from the derived BDF
  out of logical index of socket & port

  @param[in] IioGlobalData - IIO Global data structure
  @param[in] IioIndex      - Socket Index
  @param[in] PortIndex     - Port Index

  @retval Device ID from PCI header

**/
UINT32
GetPortDeviceId (
  IN IIO_GLOBALS   *IioGlobalData,
  IN UINT8          IioIndex,
  IN UINT8          PortIndex
  )
{
  UINT32  DeviceId;
  UINT8   Bus;
  UINT8   Device;
  UINT8   Function;

  Bus = IioGlobalData->IioVar.IioVData.SocketPortBusNumber[IioIndex][PortIndex];
  Device = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].Device;
  Function = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].Function;

  DeviceId = IioPciExpressRead32 (IioIndex, Bus, Device, Function, PCI_VENDOR_ID_OFFSET);

  return DeviceId;
}

/**

    Get Size of BAR for PCIE port

    @param IioIndex      - IIO Index
    @param PortIndex     - PCIE port Index

    @retval BarSize

**/
UINT64
GetPortBarSize (
  IN  IIO_GLOBALS   *IioGlobalData,
  IN  UINT8         IioIndex,
  IN  UINT8         PortIndex
  )
{
  UINT64  BarSize;
  UINT32  PcieBarN0;
  UINT32  PcieBarN1;
  UINT8   Bus;
  UINT8   Device;
  UINT8   Function;

  Bus = IioGlobalData->IioVar.IioVData.SocketPortBusNumber[IioIndex][PortIndex];
  Device = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].Device;
  Function = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].Function;

  IioPciExpressWrite32 (IioIndex, Bus, Device, Function, R_PCIE_PORT_BAR_N0, 0xFFFFFFFF);
  IioPciExpressWrite32 (IioIndex, Bus, Device, Function, R_PCIE_PORT_BAR_N1, 0xFFFFFFFF);

  PcieBarN0 = IioPciExpressRead32 (IioIndex, Bus, Device, Function, R_PCIE_PORT_BAR_N0);
  PcieBarN1 = IioPciExpressRead32 (IioIndex, Bus, Device, Function, R_PCIE_PORT_BAR_N1);

  BarSize = LShiftU64(PcieBarN1, 32) | (PcieBarN0 & 0xFFFFFFF0);
  BarSize = (~BarSize) + 1;

  return BarSize;
}

/**

    Get 64-bit BAR address for the given device

    @param IioGlobalData - IIO Global data structure
    @param IioIndex      - IIO Index
    @param PortIndex     - PCIE port Index

    @retval BarAddr

**/
UINT64
GetPortBarAddr (
  IN  UINT8         IioIndex,
  IN  UINT8         Bus,
  IN  UINT8         Device,
  IN  UINT8         Function,
  IN  UINT16        Offset
  )
{
  UINT64  BarAddr;
  UINT32  PcieBarN0;
  UINT32  PcieBarN1;

  PcieBarN0 = IioPciExpressRead32 (IioIndex, Bus, Device, Function, Offset);
  PcieBarN1 = IioPciExpressRead32 (IioIndex, Bus, Device, Function, Offset + 0x4);

  BarAddr = LShiftU64(PcieBarN1, 32) | (PcieBarN0 & 0xFFFFFFF0);

  return BarAddr;
}

/**
  This function checks whether given IIO port is DMI port of given identity.

  @param[in] IioIndex     - Index of IIO instance
  @param[in] PortIndex    - IIO port index
  @param[in] DmiIdentity  - current role of DMI port in the system

  @return TRUE if port is acting as specified in the DmiIdentity parameter, otherwise FALSE.
**/
BOOLEAN
IioDmiIsPortIdentity (
  IN  UINT8              IioIndex,
  IN  UINT8              PortIndex,
  IN  IIO_DMI_IDENTITY   DmiIdentity
  )
{
  IIO_GLOBALS            *IioGlobalData;
  EFI_STATUS             Status;
  BOOLEAN                IsActive;

  if (PortIndex != DMI_PORT_INDEX) {
    //
    // Only port 0 can act as DMI.
    //
    return FALSE;
  }

  switch (DmiIdentity) {
    case IioDmiPort:
      return TRUE;
    case IioDmiPortActive:
    case IioDmiPortInactive:
      IsActive = FALSE;
      if (IioIndex == SOCKET_0_INDEX) {
        //
        // Port O on socket 0 is always a DMI/RLINK port in all current programs
        //
        IsActive = TRUE;
      } else {
        //
        // Code below is for further support multi-PCH case on PortIndex 0 of non-legacy sockets
        //
        Status = GetIioGlobalData (&IioGlobalData);
        if (!EFI_ERROR (Status)) {

          if (IioGlobalData->IioVar.IioVData.MultiPch) {
            if ((IsCpuAndRevision (CPU_SPRSP, REV_ALL)) &&
               (((BIT0 << IioIndex) & GetPchPresentBitMap()) != 0)) {
              IsActive = TRUE;
            } else {
              //
              // if Class code matches the PCI Host Bridge's base class (06), subclass (0),
              // Reg. Prog interface(0), then it is connected to the PCH on the down link
              // which eventually determines the DMI port
              //
              if (GetPortClassCode (IioGlobalData, IioIndex, PortIndex) == (PCI_CLASS_BRIDGE << 16 | PCI_CLASS_BRIDGE_HOST << 8)) {
                IsActive = TRUE;  //return TRUE to indicate that the PCI address belongs to DMI port
              }
            }
          }
        }
      }
      return (DmiIdentity == IioDmiPortActive) ? IsActive : !IsActive;
    case IioDmiPortAsPci:
      //
      // For SKX/CLX/CPX: Device ID DMI == 0x2020, PCIe == 0x2030
      //
      Status = GetIioGlobalData (&IioGlobalData);
      if (!EFI_ERROR (Status)) {
        if (GetPortDeviceId (IioGlobalData, IioIndex, PortIndex) == VID_DID_SKX_PCIE_PORT) {
          return TRUE;
        }
      }
      break;
    default:
      IIO_D_DMIERR ("[%d][%d] Asked for DMI identity (%d) out of range\n", IioIndex, PortIndex, DmiIdentity);
      break;
    }
    return FALSE;
}

/**
  This function checks if the given CPU is connected to R-Link

  @returns TRUE if the CPU is connected to R-Link, otherwise FALSE.
**/
BOOLEAN
HasRlink (
  VOID
  )
{
  if (IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL)
      || IsCpuAndRevision (CPU_TNR, REV_ALL)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**

    Check if Port will be configured as NTB

    @param IioGlobalData - Global IIO Data Structure
    @param Iio           - Iio Index
    @param Bus           - Bus Number

    @retval TRUE/FALSE   - Based on setup option value

**/
BOOLEAN
CheckNtbPortConfSetting (
  IN  UINT8              IioIndex,
  IN  UINT8              PortIndex
  )
{
  IIO_OUT_DATA    *IioOutData;

  IioOutData = GetIioOutData ();

  if ((IioOutData->PciePortOwnership[(IioIndex * NUMBER_PORTS_PER_SOCKET) + PortIndex] == NTB_PORT_DEF_NTB_NTB ) ||
      (IioOutData->PciePortOwnership[(IioIndex * NUMBER_PORTS_PER_SOCKET) + PortIndex] == NTB_PORT_DEF_NTB_RP )) {
    return TRUE;
  }

  return FALSE;
}

/**
    Used to calculate mask for disabling PCIE lines in PCIE gen3 controller
    for port during link training when setup knob Override Max Link Width is used.

    @param[in] IioGlobalData - Pointer to Global IIO Data Structure
    @param[in] Iio           - Iio Index
    @param[in] PortIndex     - IIO port index
    @param[in out] LaneMask  - Pointer to Lane mask value to be calculated
**/


VOID
IioPortCalculateLaneDisableMask (
  IIO_GLOBALS                           *IioGlobalData,
  UINT8                                 IioIndex,
  UINT8                                 PortIndex,
  UINT16                                *LaneMask
)
{
  UINT8        DefaultMaxLinkWidth;
  UINT8        OverrideMaxLinkWidthOptVal;
  UINT8        OverrideMaxLinkWidth;
  UINT8        *ArrayMaxLinkWidth;
  UINT8        *ArrayReverseLink;
  UINT8        MaxPortNumberPerSocket;
  IIO_CONFIG   *SetupData;
  IIO_OUT_DATA *IioOutData;

  SetupData = &IioGlobalData->SetupData;
  IioOutData = &IioGlobalData->IioVar.IioOutData;

  ArrayMaxLinkWidth = &(IioOutData->MaxPXPMap[0]);
  ArrayReverseLink = &(IioOutData->LaneReversedPXPMap[0]);

  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);

  DefaultMaxLinkWidth = IioOutData->CurrentPXPMap[(IioIndex * MaxPortNumberPerSocket) + PortIndex];
  OverrideMaxLinkWidthOptVal = SetupData->PciePortLinkMaxWidth[((IioIndex) * MaxPortNumberPerSocket) + PortIndex];
  OverrideMaxLinkWidth = ArrayMaxLinkWidth[(IioIndex * MaxPortNumberPerSocket) + PortIndex];

  switch (DefaultMaxLinkWidth) {
    case PORT_LINK_WIDTH_x16:
      // if port is bifurcated to x16, force all 16 lanes to the mask specified by the override link width
      switch (OverrideMaxLinkWidth) {
        case PORT_LINK_WIDTH_x8:
          // Force x16 to x8
          if (ArrayReverseLink[(IioIndex * MaxPortNumberPerSocket) + PortIndex]) {
            *LaneMask = 0x00FF;
          } else {
            *LaneMask = 0xFF00;
          }
          break;

        case PORT_LINK_WIDTH_x4:
          // Force x16 to x4
          if (ArrayReverseLink[(IioIndex * MaxPortNumberPerSocket) + PortIndex]) {
            *LaneMask = 0x0FFF;
          } else {
            *LaneMask = 0xFFF0;
          }
          break;

        case PORT_LINK_WIDTH_x2:
          // Force x16 to x2
          if (ArrayReverseLink[(IioIndex * MaxPortNumberPerSocket) + PortIndex]) {
            *LaneMask = 0x3FFF;
          } else {
            *LaneMask = 0xFFFC;
          }
          break;

        case PORT_LINK_WIDTH_x1:
          // Force x16 to x1
          if(ArrayReverseLink[(IioIndex * MaxPortNumberPerSocket) + PortIndex]) {
            *LaneMask = 0x7FFF;
          } else {
            *LaneMask = 0xFFFE;
          }
          break;

        default:
          *LaneMask = 0x0000;
          break;
      }//End switch(OverrideMaxLinkWidth) x16
      break;

    case PORT_LINK_WIDTH_x8:
      // if port is bifurcated to x8, set lane mask according to bifurcation and which port it is.
      switch(OverrideMaxLinkWidth) {
        case PORT_LINK_WIDTH_x4:
          // Force x8 to x4
          if (CheckPortBifx8Low (IioIndex, PortIndex)) {
            if(ArrayReverseLink[(IioIndex * MaxPortNumberPerSocket) + PortIndex]) {
              *LaneMask |= 0x000F;
            } else {
              *LaneMask |= 0x00F0;
            }
          } else if (CheckPortBifx8High (IioIndex, PortIndex)) {
            if(ArrayReverseLink[(IioIndex * MaxPortNumberPerSocket) + PortIndex]) {
              *LaneMask |= 0x0F00;
            } else {
              *LaneMask |= 0xF000;
            }
          } else {
            *LaneMask |= 0x0000;
          }
          break;

        case PORT_LINK_WIDTH_x2:
          // Force x8 to x2
          if (CheckPortBifx8Low (IioIndex, PortIndex)) {
            if (ArrayReverseLink[(IioIndex * MaxPortNumberPerSocket) + PortIndex]) {
              *LaneMask |= 0x003F;
            } else {
              *LaneMask |= 0x00FC;
            }
          } else if (CheckPortBifx8High (IioIndex, PortIndex)) {
            if (ArrayReverseLink[(IioIndex * MaxPortNumberPerSocket) + PortIndex]) {
              *LaneMask |= 0x3F00;
            } else {
              *LaneMask |= 0xFC00;
            }
          } else {
            *LaneMask |= 0x0000;
            break;
          }// else
          break;

        case PORT_LINK_WIDTH_x1:
          // Force x8 to x1
          if (CheckPortBifx8Low (IioIndex, PortIndex)) {
            if(ArrayReverseLink[(IioIndex * MaxPortNumberPerSocket) + PortIndex]) {
              *LaneMask |= 0x007F;
            } else {
              *LaneMask |= 0x00FE;
            }
          } else if (CheckPortBifx8High (IioIndex, PortIndex)) {
            if(ArrayReverseLink[(IioIndex * MaxPortNumberPerSocket) + PortIndex]) {
              *LaneMask |= 0x7F00;
            } else {
              *LaneMask |= 0xFE00;
            }
          } else {
            *LaneMask |= 0x0000;
            break;
          }// End switch(PortIndex)
          break;

        default:
          //Automated add of default case. Please review.
          break;
      } // switch(OverrideMaxLinkWidth) for x8
      break;

    case PORT_LINK_WIDTH_x4:
      switch (OverrideMaxLinkWidth) {
        case PORT_LINK_WIDTH_x2:
          // Force x4 to x2
          if (CheckPortBifx8Low (IioIndex, PortIndex) || IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPortActive)) {
            if (ArrayReverseLink[(IioIndex * MaxPortNumberPerSocket) + PortIndex]) {
              *LaneMask |= 0x0003;
            } else {
              *LaneMask |= 0x000C;
            }
          } else if (CheckPortBifx4Low (IioIndex, PortIndex)) {
            if (ArrayReverseLink[(IioIndex * MaxPortNumberPerSocket) + PortIndex]) {
              *LaneMask |= 0x0030;
            } else {
              *LaneMask |= 0x00C0;
            }
          } else if (CheckPortBifx8High (IioIndex, PortIndex)) {
            if (ArrayReverseLink[(IioIndex * MaxPortNumberPerSocket) + PortIndex]) {
              *LaneMask |= 0x0300;
            } else {
              *LaneMask |= 0x0C00;
            }
          } else if (CheckPortBifx4High (IioIndex, PortIndex)) {
            if (ArrayReverseLink[(IioIndex * MaxPortNumberPerSocket) + PortIndex]) {
              *LaneMask |= 0x3000;
            } else {
              *LaneMask |= 0xC000;
            }
          } else {
            *LaneMask |= 0x0000;
          } //else
          break;

        case PORT_LINK_WIDTH_x1:
          // Force x4 to x1
          if (CheckPortBifx8Low (IioIndex, PortIndex) || IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPortActive)) {
            if (ArrayReverseLink[(IioIndex * MaxPortNumberPerSocket) + PortIndex]) {
              *LaneMask |= 0x0007;
            } else {
              *LaneMask |= 0x000E;
            }
          } else if (CheckPortBifx4Low (IioIndex, PortIndex)) {
            if (ArrayReverseLink[(IioIndex * MaxPortNumberPerSocket) + PortIndex]) {
              *LaneMask |= 0x0070;
            } else {
              *LaneMask |= 0x00E0;
            }
          } else if (CheckPortBifx8High (IioIndex, PortIndex)) {
            if (ArrayReverseLink[(IioIndex * MaxPortNumberPerSocket) + PortIndex]) {
              *LaneMask |= 0x0700;
            } else {
              *LaneMask |= 0x0E00;
            }
          } else if (CheckPortBifx4High (IioIndex, PortIndex)) {
            if (ArrayReverseLink[(IioIndex * MaxPortNumberPerSocket) + PortIndex]) {
              *LaneMask |= 0x7000;
            } else {
              *LaneMask |= 0xE000;
            }
          } else {
            *LaneMask |= 0x0000;
          }//Else
          break;

        default:
          //Automated add of default case. Please review.
          break;
      }// End  switch(OverrideMaxLinkWidth) x4
      break;
  } // End switch(DefaultMaxLinkWidth)
}


/**
    Set or clear Memory Space Enable bit

    @param IioIndex          - Socket Index
    @param Bus               - PCIE port Bus
    @param Device            - PCIE port Device
    @param Function          - PCIE port Function
    @param Offset            - Offset to program
    @param Enable            - Set or clear Memory Space Enable bit
**/
VOID
SetMemorySpaceEnable (
  IN  UINT8         IioIndex,
  IN  UINT8         Bus,
  IN  UINT8         Device,
  IN  UINT8         Function,
  IN  UINT16        Offset,
  IN  BOOLEAN       Enable
  )
{
  UINT16  PciCmd;

  // Enable or Disable the address access on the port
  PciCmd = IioPciExpressRead16 (IioIndex, Bus, Device, Function, Offset);
  if (Enable) {
    PciCmd |= EFI_PCI_COMMAND_MEMORY_SPACE;  //Memory Space Enable
  } else {
    PciCmd &= ~EFI_PCI_COMMAND_MEMORY_SPACE;  //Memory Space Enable
  }
  IioPciExpressWrite16 (IioIndex, Bus, Device, Function, Offset, PciCmd);
}


/**
  Return TRUE if VMD 2.0 is supported
**/
BOOLEAN
IsVmd20Supported (
  VOID
  )
{
  if (IsCpuAndRevision (CPU_SKX, REV_ALL) ||
      IsCpuAndRevision (CPU_CLX, REV_ALL) ||
      IsCpuAndRevision (CPU_CPX, REV_ALL) ) {
    //
    // 14nm programs doesn't support VMD 2.0
    //
    return FALSE;
  }

  return TRUE;
}

/**
    Check if at least one Port on the Stack will be configured as VMD

    @param IioIndex      - Iio Index
    @param StackIndex    - Stack Index

    @retval VMDPortsEnable   - TRUE/FALSE to indicate that at least one port in the stack
                               will be configured as VMD.
**/
BOOLEAN
IsVMDEnabledForStack (
  IN  UINT8              IioIndex,
  IN  UINT8              StackIndex
  )
{
  IIO_CONFIG    *IioSetupData;

  IioSetupData = GetIioConfig ();

  if (!IsVmd20Supported () && IsDmiStack (StackIndex)) {
    return FALSE;
  }

  if (StackIndex >= MAX_VMD_STACKS_PER_SOCKET) {
    return FALSE;
  }


  if (IioSetupData->VMDEnabled[IioIndex][StackIndex]) {
    return TRUE;
  }

  return FALSE;
}

/**
    Check if given Port on the IIO complex will be configured as VMD

    @param IioIndex      - Iio Index
    @param PortIndex     - Port Index

    @return TRUE if given port will be configured as VMD, FALSE otherwise.
**/
BOOLEAN
IsVMDEnabledForPort (
  IN  UINT8              IioIndex,
  IN  UINT8              PortIndex
  )
{
  IIO_V_DATA        *IioVData;
  IIO_CONFIG        *IioSetupData;

  if (PortIndex >= NUMBER_PORTS_PER_SOCKET) {
    ASSERT(FALSE);
    return FALSE;
  }

  IioVData = GetIioVData ();
  if (!IsVMDEnabledForStack (IioIndex,
    IioVData->StackPerPort[IioIndex][PortIndex])) {
    return FALSE;
  }

  IioSetupData = GetIioConfig ();

  return (IioSetupData->VMDPortEnable[IioIndex][PortIndex] != 0);
}

/**

    Check if it is a port connected to FPGA (Only for SKX-FPGA).

    @param IioGlobalData - Pointer to IIO global data/methods
    @param PortIndex     - Port number being initialized.

    @retval TRUE - It is a port connected to FPGA
    @retval FALSE - It is NOT a port connected to FPGA

**/
BOOLEAN
CheckFPGAPort (
  IN IIO_GLOBALS   *IioGlobalData,
  IN UINT8         IioIndex,
  IN UINT8         PortIndex
  )
{
  if (IioGlobalData->IioVar.IioVData.SkuPersonality[IioIndex] == TYPE_FPGA) {
    if ((PortIndex >= PORT_4A_INDEX) && (PortIndex <= PORT_4D_INDEX)) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Checks if PCI device at given address exists.

  @param[in]  IioIndex         - Socket ID
  @param[in]  Bus              - PCI bus number
  @param[in]  Device           - PCI device number
  @param[in]  Function         - PCI function number
  @param[out] PciHdrPtr        - Optional pointer to a buffer for PCI device header

  @retval TRUE if exists
  @retval FALSE otherwise
**/
BOOLEAN
IioIsDevicePresent (
  IN  UINT8                      IioIndex,
  IN  UINT8                      Bus,
  IN  UINT8                      Device,
  IN  UINT8                      Function,
  OUT PCI_TYPE_GENERIC           *PciHdrPtr
  )
{
  UINT8      Index;

  union {
    struct {
      UINT16 VendorId;
      UINT16 DeviceId;
    } Hdr;
    UINT32   DWord;
  } PciHdr;

  PciHdr.DWord = IioPciExpressRead32 (IioIndex, Bus, Device, Function, PCI_VENDOR_ID_OFFSET);

  if (PciHdr.Hdr.VendorId == 0xFFFF || PciHdr.Hdr.DeviceId == 0xFFFF) {
    return FALSE;
  } else {
    if (PciHdr.Hdr.VendorId == 0x8086 && PciHdr.Hdr.DeviceId == V_VMD_DUMMY_DID) {
      return FALSE;
    }
  }

  if (PciHdrPtr != NULL) {
    ((UINT32*)PciHdrPtr)[0] = PciHdr.DWord;
    for (Index = 1; Index < sizeof(*PciHdrPtr) / sizeof(UINT32); Index++) {
      ((UINT32*)PciHdrPtr)[Index] = IioPciExpressRead32 (IioIndex, Bus, Device, Function, Index * sizeof(UINT32));
    }
  }

  return PciHdr.Hdr.VendorId != 0xFFFF;
}

/**
  Returns Pcie Negotiated Link Width from link status register.

  @param[in]  IioIndex                 - Socket ID
  @param[in]  PortIndex                - Controller ID
  @param[out] NegotiatedLinkWidthPtr   - Pointer to UINT16 Value that returns Negotiated Link Width

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
EFIAPI
IioGetPcieNegotiatedLinkWidth (
  IN  UINT8                       IioIndex,
  IN  UINT8                       PortIndex,
  OUT UINT16                      *NegotiatedLinkWidthPtr
  )
{
  EFI_STATUS                      Status;
  PCI_REG_PCIE_LINK_STATUS        LnkSts;
  IIO_GLOBALS                     *IioGlobalData;

  Status = GetIioGlobalData (&IioGlobalData);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (PortIndex >= GetMaxPciePortNumberPerSocket (IioIndex) ||
      IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort) ||
      NegotiatedLinkWidthPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetPcieCapReg16 (IioIndex, PortIndex, R_PCIE_LSTS_OFFSET, &LnkSts.Uint16);
  if (EFI_ERROR (Status)) {
    IIO_D_PCIERR ("[%d p%d] %a: Cannot read LinkStatus register (%r)\n",
                  IioIndex, PortIndex, __FUNCTION__, Status);
    return Status;
  }
  *NegotiatedLinkWidthPtr = LnkSts.Bits.NegotiatedLinkWidth;

  return EFI_SUCCESS;
}

/**
  Returns Pcie Max Link Width from link capability register.

  @param[in]  IioIndex          - Socket ID
  @param[in]  PortIndex         - Controller ID
  @param[out] MaxLinkWidthPtr   - Pointer to UINT32 Value that returns Max Link Width

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
EFIAPI
IioGetPcieMaxLinkWidth (
  IN  UINT8                       IioIndex,
  IN  UINT8                       PortIndex,
  OUT UINT32                      *MaxLinkWidthPtr
  )
{
  EFI_STATUS                      Status;
  PCI_REG_PCIE_LINK_CAPABILITY    LnkCap;
  IIO_GLOBALS                     *IioGlobalData;

  Status = GetIioGlobalData (&IioGlobalData);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (PortIndex >= GetMaxPciePortNumberPerSocket (IioIndex) ||
      IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPortActive) ||
      MaxLinkWidthPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetPcieCapReg32 (IioIndex, PortIndex, R_PCIE_LCAP_OFFSET, &LnkCap.Uint32);
  if (EFI_ERROR (Status)) {
    IIO_D_PCIERR ("[%d p%d] %a: Cannot read LinkCapability register (%r)\n",
                  IioIndex, PortIndex, __FUNCTION__, Status);
    return Status;
  }
  *MaxLinkWidthPtr = LnkCap.Bits.MaxLinkWidth;

  return EFI_SUCCESS;
}

/**
  Returns Pcie Current Link Speed from link status register.

  @param[in]  IioIndex               - Socket ID
  @param[in]  PortIndex              - Controller ID
  @param[out] CurrentLinkSpeedPtr    - Pointer to UINT16 Value that returns Current Link Speed

  @return Standard EFI_STATUS is returned.
**/

EFI_STATUS
EFIAPI
IioGetPcieCurrentLinkSpeed (
  IN  UINT8                       IioIndex,
  IN  UINT8                       PortIndex,
  OUT UINT16                      *CurrentLinkSpeedPtr
  )
{
  EFI_STATUS                      Status;
  PCI_REG_PCIE_LINK_STATUS        LnkSts;
  IIO_GLOBALS                     *IioGlobalData;

  Status = GetIioGlobalData (&IioGlobalData);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (PortIndex >= GetMaxPciePortNumberPerSocket (IioIndex) ||
      IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort) ||
      CurrentLinkSpeedPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetPcieCapReg16 (IioIndex, PortIndex, R_PCIE_LSTS_OFFSET, &LnkSts.Uint16);
  if (EFI_ERROR (Status)) {
    IIO_D_PCIERR ("[%d p%d] %a: Cannot read LinkStatus register (%r)\n",
                  IioIndex, PortIndex, __FUNCTION__, Status);
    return Status;
  }
  *CurrentLinkSpeedPtr = LnkSts.Bits.CurrentLinkSpeed;

  return EFI_SUCCESS;
}

/**
  Returns Pcie Target Link Speed from link control2 register.

  @param[in]  IioIndex           - Socket ID
  @param[in]  PortIndex          - Controller ID
  @param[out] TargetLinkSpeedPtr - Pointer to UINT16 Value that returns Target Link Speed

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
EFIAPI
IioGetPcieTargetLinkSpeed (
  IN  UINT8                       IioIndex,
  IN  UINT8                       PortIndex,
  OUT UINT16                      *TargetLinkSpeedPtr
  )
{
  EFI_STATUS                      Status;
  PCI_REG_PCIE_LINK_CONTROL2      LnkCtl2;
  IIO_GLOBALS                     *IioGlobalData;

  Status = GetIioGlobalData (&IioGlobalData);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (PortIndex >= GetMaxPciePortNumberPerSocket (IioIndex) ||
      IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort) ||
      TargetLinkSpeedPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetPcieCapReg16 (IioIndex, PortIndex, R_PCIE_LCTL2_OFFSET, &LnkCtl2.Uint16);
  if (EFI_ERROR (Status)) {
    IIO_D_PCIERR ("[%d p%d] %a: Cannot read LinkControl2 register (%r)\n",
                  IioIndex, PortIndex, __FUNCTION__, Status);
    return Status;
  }
  *TargetLinkSpeedPtr = LnkCtl2.Bits.TargetLinkSpeed;

  return EFI_SUCCESS;
}

/**
  Dump Iio Port Info

  @param[in] IioGlobalData    - Pointer to IioGlobalData

  @retval None
**/

VOID
DumpIioPciePortPcieLinkStatus (
  IN IIO_GLOBALS              *IioGlobalData
  )
{
  UINT8                       IioIndex;
  UINT8                       PortIndex;
  UINT8                       SegmentNum;
  UINT8                       Bus;
  UINT8                       Device;
  UINT8                       Function;
  UINT8                       MaxPortNumberPerSocket;
  UINT16                      NegotiatedLinkWidthStatus;
  UINT16                      CurrentLinkSpeedStatus;
  EFI_STATUS                  Status;

  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {

    if (!IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) {
      continue;
    }
    MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];
    for (PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {
      Bus      = IioGlobalData->IioVar.IioVData.SocketPortBusNumber[IioIndex][PortIndex];
      Device   = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].Device;
      Function = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].Function;
      //
      // check whether the PCIe port can be configured
      //
      if (!IioVData_ConfigurePciePort (IioGlobalData, IioIndex, PortIndex)) {
        continue;
      }
      //
      // skip DMI port
      //
      if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort)) {
        continue;
      }
      //
      // skip ports assigned to VMD
      //
      if (IsVMDEnabledForPort (IioIndex, PortIndex)) {
        continue;
      }

      SegmentNum = IioGlobalData->IioVar.IioVData.SegmentSocket[IioIndex];
      //
      // check if device is hidden or not
      //
      if (!IioIsDevicePresent (IioIndex, Bus, Device, Function, NULL)) {
        IIO_D_PCIWARN ("%02X:%02X:%02X.%X:\t\tDevice is not present!\n",
                      SegmentNum, Bus, Device, Function);
        continue;
      }

      Status = IioGetPcieNegotiatedLinkWidth (IioIndex, PortIndex, &NegotiatedLinkWidthStatus);
      if (EFI_ERROR (Status)) {
        IIO_D_PCIWARN ("%02X:%02X:%02X.%X:\t\tDevice Link Width not found! (%r)\n",
                      SegmentNum, Bus, Device, Function, Status);
        continue;
      }

      Status = IioGetPcieCurrentLinkSpeed (IioIndex, PortIndex, &CurrentLinkSpeedStatus);
      if (EFI_ERROR (Status)) {
        IIO_D_PCIWARN ("%02X:%02X:%02X.%X:\t\tDevice Link Speed not found! (%r)\n",
                      SegmentNum, Bus, Device, Function, Status);
        continue;
      }

      if (NegotiatedLinkWidthStatus == 0) {
        IIO_D_PCIWARN ("%02X:%02X:%02X.%X:\t\tLink Down!\n",
                      SegmentNum, Bus, Device, Function);
      } else {
        if ((NegotiatedLinkWidthStatus == 1) ||
            (NegotiatedLinkWidthStatus == 2) ||
            (NegotiatedLinkWidthStatus == 4) ||
            (NegotiatedLinkWidthStatus == 8) ||
            (NegotiatedLinkWidthStatus == 16)) {
          IIO_D_PCILOG ("%02X:%02X:%02X.%X:\t\tLink up as x%02d Gen%d!\n",
                        SegmentNum, Bus, Device, Function,
                        NegotiatedLinkWidthStatus, CurrentLinkSpeedStatus);
        }
      }
    }
  }
}

/**

    Verifiy if the PCIe Root Port is a superclusterport.

    @param IioGlobalData - Pointer to IIO_GLOBALS
    @param PortIndex     - Index to Iio Root Port

    @retval TRUE/FALSE

**/
BOOLEAN
IsSuperClusterPort (
  UINT8        PortIndex
  )
{
  BOOLEAN SuperCluster = FALSE;

  if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    if ((PortIndex == PCIE_PORT_0_SUPER_CLUSTER_PORT_0) || (PortIndex == PCIE_PORT_1A_SUPER_CLUSTER_PORT_1) ||
        (PortIndex == PCIE_PORT_2A_SUPER_CLUSTER_PORT_2) || (PortIndex == PCIE_PORT_3A_SUPER_CLUSTER_PORT_3) ||
        (PortIndex == PCIE_PORT_4A_SUPER_CLUSTER_PORT_4) || (PortIndex == PCIE_PORT_5A_SUPER_CLUSTER_PORT_5)) {
      SuperCluster = TRUE;
    }
  } else if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    if ((PortIndex == PCIE_PORT_0_SUPER_CLUSTER_PORT_SPR) || (PortIndex == PCIE_PORT_1A_SUPER_CLUSTER_PORT_SPR) ||
        (PortIndex == PCIE_PORT_2A_SUPER_CLUSTER_PORT_SPR) || (PortIndex == PCIE_PORT_3A_SUPER_CLUSTER_PORT_SPR) ||
        (PortIndex == PCIE_PORT_4A_SUPER_CLUSTER_PORT_SPR) || (PortIndex == PCIE_PORT_5A_SUPER_CLUSTER_PORT_SPR)) {
      SuperCluster = TRUE;
    }
  } else {
    if ((PortIndex == PCIE_PORT_0_SUPER_CLUSTER_PORT) || (PortIndex == PCIE_PORT_1A_SUPER_CLUSTER_PORT) ||
        (PortIndex == PCIE_PORT_2A_SUPER_CLUSTER_PORT) || (PortIndex == PCIE_PORT_3A_SUPER_CLUSTER_PORT) ||
        ((IsThisCpx () || IioIsFourthPCIeGen3ControllerPresent (0)) && PortIndex == PCIE_PORT_4A_SUPER_CLUSTER_PORT)) {
      SuperCluster = TRUE;
    }
  }

  return SuperCluster;
}
/**
  Returns boolean if Sku is "CLX-64L" for a given CPU socket.

  @param IioIndex          - Socket ID of the CPU who's sku info requested

  @retval TRUE  - CLX-64L sku
  @retval FALSE - Not CLX-64L Sku
**/
BOOLEAN
IsSocket64LPresent (
  IN UINT8         IioIndex
  )
{
  if ((SEGMENT_TYPE_WRKST == PcuGetSegType (IioIndex)) && (EXSEGMENT_TYPE_64L == PcuGetExsegType (IioIndex))) {
    return TRUE;
  }

  return FALSE;
}

/**
  Returns TRUE if currently running on CPX cpu

  @retval TRUE  - this is CPX
  @retval FALSE - this is not CPX
 */
BOOLEAN
IsThisCpx ( VOID )
{
  return IsCpuAndRevision (CPU_CPX, REV_ALL);
}

/**

  Returns boolean if the fourth PCIe Gen3 controller is present
  for a given CPU socket.

  @param IioIndex          - Soc ID of the CPU who's sku info requested

  @retval TRUE  - 4th controller present
  @retval FALSE - otherwise

**/
BOOLEAN
IioIsFourthPCIeGen3ControllerPresent (
  IN UINT8         IioIndex
  )
{
  if (IsSocket64LPresent (IioIndex)) {
    return TRUE;
  }

  return FALSE;
}

/**
    Verify if the PcieRootPort can be bifurcated x8 and belong to A-B

    @param IioIndex      -  Soc ID of the CPU who's sku info requested
    @param PortIndex     - Index to Iio Root Port

    @retval TRUE/FALSE

**/
BOOLEAN
CheckPortBifx8Low(
  UINT8 IioIndex,
  UINT8 PortIndex
  )
{
  BOOLEAN Portx8 = FALSE;

 if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    if ((PortIndex == PORT_1A_INDEX_1) || (PortIndex == PORT_2A_INDEX_2) ||
        (PortIndex == PORT_3A_INDEX_3) || (PortIndex == PORT_4A_INDEX_4) ||
        (PortIndex == PORT_5A_INDEX_5)) {
      Portx8 = TRUE;
    }
  } else if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    if (PortIndex == PORT_1A_INDEX || PortIndex == PORT_2A_INDEX || PortIndex == PORT_3A_INDEX ||
       (PortIndex == PORT_4A_INDEX && IioIsFourthPCIeGen3ControllerPresent (IioIndex)) ||
       (PortIndex == PORT_4A_INDEX && IsThisCpx ())) {
      Portx8 = TRUE;
    }
  } else {
    if ((PortIndex == PORT_1A_INDEX) || (PortIndex == PORT_2A_INDEX) ||
        (PortIndex == PORT_3A_INDEX)) {
      Portx8 = TRUE;
    }
  }
  return Portx8;
}

/**

    Verify if the PcieRootPort can be bifurcated x8 and belong to C-D

    @param IioIndex      -  Soc ID of the CPU who's sku info requested
    @param PortIndex     - Index to Iio Root Port

    @retval TRUE/FALSE

**/
BOOLEAN
CheckPortBifx8High (
  UINT8 IioIndex,
  UINT8 PortIndex
  )
{
  BOOLEAN Portx8 = FALSE;

  if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    if ((PortIndex == PORT_1C_INDEX_1) || (PortIndex == PORT_2C_INDEX_2) ||
        (PortIndex == PORT_3C_INDEX_3) || (PortIndex == PORT_4C_INDEX_4) ||
        (PortIndex == PORT_5C_INDEX_5)) {
      Portx8 = TRUE;
    }
  } else if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    if (PortIndex == PORT_1C_INDEX || PortIndex == PORT_2C_INDEX || PortIndex == PORT_3C_INDEX ||
       (PortIndex == PORT_4C_INDEX && IioIsFourthPCIeGen3ControllerPresent (IioIndex)) ||
       (PortIndex == PORT_4C_INDEX && IsThisCpx ())) {
      Portx8 = TRUE;
    }
  } else {
    if ((PortIndex == PORT_1C_INDEX) || (PortIndex == PORT_2C_INDEX) ||
        (PortIndex == PORT_3C_INDEX)){
      Portx8 = TRUE;
    }
  }
  return Portx8;
}

/**

    Verify if the PcieRootPort can be bifurcated x4 and is B

    @param IioIndex      -  Soc ID of the CPU who's sku info requested
    @param PortIndex     - Index to Iio Root Port

    @retval TRUE/FALSE

**/
BOOLEAN
CheckPortBifx4Low(
  UINT8 IioIndex,
  UINT8 PortIndex
  )
{
  BOOLEAN Portx4 = FALSE;

  if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    if ((PortIndex == PORT_1B_INDEX_1) || (PortIndex == PORT_2B_INDEX_2) ||
        (PortIndex == PORT_3B_INDEX_3) || (PortIndex == PORT_4B_INDEX_4) ||
        (PortIndex == PORT_5B_INDEX_5)) {
      Portx4 = TRUE;
    }
  } else if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    if (PortIndex == PORT_1B_INDEX || PortIndex == PORT_2B_INDEX || PortIndex == PORT_3B_INDEX ||
       (PortIndex == PORT_4B_INDEX && IioIsFourthPCIeGen3ControllerPresent (IioIndex)) ||
       (PortIndex == PORT_4B_INDEX && IsThisCpx ())) {
      Portx4 = TRUE;
    }
  } else {
    if ((PortIndex == PORT_1B_INDEX) || (PortIndex == PORT_2B_INDEX) ||
        (PortIndex == PORT_3B_INDEX)) {
      Portx4 = TRUE;
    }
  }

  return Portx4;
}

/**

    Verify if the PcieRootPort can be bifurcated x4 and is D

    @param IioIndex      -  Soc ID of the CPU who's sku info requested
    @param PortIndex     - Index to Iio Root Port

    @retval TRUE/FALSE

**/
BOOLEAN
CheckPortBifx4High (
  UINT8 IioIndex,
  UINT8 PortIndex
  )
{
  BOOLEAN Portx4 = FALSE;

  if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    if ((PortIndex == PORT_1D_INDEX_1) || (PortIndex == PORT_2D_INDEX_2) ||
        (PortIndex == PORT_3D_INDEX_3) || (PortIndex == PORT_4D_INDEX_4) ||
        (PortIndex == PORT_5D_INDEX_5)) {
      Portx4 = TRUE;
    }
  } else if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    if (PortIndex == PORT_1D_INDEX || PortIndex == PORT_2D_INDEX || PortIndex == PORT_3D_INDEX ||
       (PortIndex == PORT_4D_INDEX && IioIsFourthPCIeGen3ControllerPresent (IioIndex)) ||
       (PortIndex == PORT_4D_INDEX && IsThisCpx ())) {
      Portx4 = TRUE;
    }
  } else {
    if ((PortIndex == PORT_1D_INDEX) || (PortIndex == PORT_2D_INDEX) ||
        (PortIndex == PORT_3D_INDEX)) {
      Portx4 = TRUE;
    }
  }

  return Portx4;
}


/**
  Check whether PCI root port initialization applies to a given stack.

  @param[in] IioGlobalPtr - Pointer to IIO global data structure.
  @param[in] IioIndex     - IIO instnance index, typically socket.
  @param[in] IioStack     - IIO stack index within IIO instance.

  @return For PCIe or DMI stack true is returned, otherwise false.
**/
BOOLEAN
IsPcieRootPortStack (
  IN IIO_GLOBALS        *IioGlobalData,
  IN UINT8               IioIndex,
  IN UINT8               StackIndex
  )
{
  BOOLEAN PcieRootPortStack = FALSE;

  PcieRootPortStack = (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_UBOX_IIO);

  return PcieRootPortStack;
}

/**

  Helper routine which returns the stack number where is found the CBDMA

  @param IioGlobalData - IIO Global data structure

  @retval Stack number of CBDMA

**/
UINT8
GetCbDmaStack (
  VOID
  )
{
  UINT8 Stack;

  if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    Stack = IIO_CSTACK;
  } else {
    Stack = IIO_STACK0;
  }

  return Stack;
}

/**
  Determine if the specified stack is the DMI stack

  @param Stack - the stack that is to be checked

  @return TRUE/FALSE depending on whether the specified PCIe stack is DMI or not
**/
BOOLEAN
IsDmiStack (
  IN  UINT8 Stack
  )
{
  if ((Stack == IIO_CSTACK) || (Stack == IIO_STACK0)) {
    return TRUE;
  }

  return FALSE;
}

/**
  IioTraceHubGetPciBusNo: Returns the NPK PCI bus number

  @param[in]  TraceHubDevice - Specify CPU or PCH trace hub device
  @param[in]  IioIndex       - Index to CPU/IIO
  @param[in]  Stack          - Stack number

  @retval  UINT8             - Returns the NPK PCI bus no
**/
UINT8
IioTraceHubGetPciBusNo (
  IN TRACE_HUB_DEVICE   TraceHubDevice,
  IN UINT8              IioIndex,
  IN UINT8              Stack
  )
{
  IIO_V_DATA            *IioVData;

  if (TraceHubDevice == PchTraceHub) {
    return PCH_NPK_BUS;
  }

  IioVData = GetIioVData ();

  return IioVData->SocketStackBus[IioIndex][Stack];
}

/**
  Get MS2IOSF channels per stack.

  @param    StackIndex  Stack Index

  @retval   Channels    Channels Connected
**/
UINT8*
GetMs2iosfChannels (
  IN  UINT8    StackIndex
  )
{
  UINT8  *Channels = NULL;

  if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    return Channels = mCpuStackChannel[CPU_ICXD].Stack[StackIndex];
  }
  if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
    return Channels = mCpuStackChannel[CPU_SNR].Stack[StackIndex];
  }
  if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    return Channels = mCpuStackChannel[CPU_ICXSP].Stack[StackIndex];
  }

  return Channels;
}

/**
  Get MS2IOSF channel index per port.

  @param PortIndex     - Port index.

  @retval  Channel index
**/
UINT8
GetMs2iosfChannelIndex (
  IN  UINT8    PortIndex
  )
{
  UINT8  ChannelIndex = 0;

  if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    return 0;
  }
  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    if (PortIndex == 0) {
      ChannelIndex = 7;
    } else {
      ChannelIndex = (PortIndex - 1) % 8;
    }
  } else if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    //
    // DMI/RLINK connected to the Port 1 of MS2IOSF on ICX_D
    //
    if (PortIndex == 0) {
      ChannelIndex = 4;
    } else if (PortIndex <= 4 ) {
      //
      // ICX-D has single PCIe Gen4(flipped) on Stack 1. PCIe Ports 1A-1D connected to Port 1 of MS2IOSF S1 on ICX-D
      //
      ChannelIndex = 4 + ((PortIndex - 1) % 4);
    } else {
      //
      // ICX-D HCC has single PCIe Gen4. PCIe Ports 2A-2D connected to Port 0 of MS2IOSF S5 on ICX-D
      //
      ChannelIndex = (PortIndex - 1) % 4;
    }
  } else if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
    //
    // DMI/RLINK connected to the Port 1 of MS2IOSF on SNR
    //
    if (PortIndex == 0) {
      ChannelIndex = 4;
    } else {
      ChannelIndex = 4 + PortIndex%4 - 1;
    }
  } else {
    //
    // For ICX
    // DMI connected to the Port 0 of MS2IOSF on ICX
    //
    if (PortIndex == 0) {
      ChannelIndex = 0;
    } else {
      ChannelIndex = (PortIndex - 1) % 4;
    }
  }

  return ChannelIndex;
}

/**
   Gets the mode of the given stack.

   @param[in] IioGlobalData - Pointer to IIO global data
   @param[in] IioIndex      - IIO instance index (usually socket)
   @param[in] Stack         - IIO stack index

   @retval PI5_MODE_DMI           Stack is in DMI mode (only stack 0 for SPR)
   @retval PI5_MODE_IAL           Stack is in IAL mode
   @retval PI5_MODE_GEN4_ONLY     Stack is in Gen4 only mode
   @retval PI5_MODE_GEN5_ONLY     Stack is in Gen5 only mode
   @retval PI5_MODE_GEN5_AND_GEN4 Stack is in Gen5/Gen4 mixed mode
   @retval PI5_MODE_NTB_GEN4      Stack is in Gen4 only NTB mode
   @retval PI5_MODE_NTB           Stack is in Gen5/Gen4 mixed NTB mode
**/
PI5_MODES
GetStackMode (
  IN IIO_GLOBALS *IioGlobalData,
  IN UINT8        IioIndex,
  IN UINT8        Stack
  )
{
  PI5_MODES    ResultMode;
  UINT8        Pi5Mode;
  UINT8        IouIndex;
  UINT8        PortIndex;

  if (!IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    ASSERT (FALSE);
    return PI5_MODE_MAX;
  }

  if (Stack >= GetMaxStackNumPerSocket () ) {
    ASSERT (FALSE);
    return PI5_MODE_MAX;
  }

  if (IIO_STACK0 == Stack) {
    return PI5_MODE_DMI;
  }

  ResultMode = PI5_MODE_GEN5_AND_GEN4;
  IouIndex   = Stack - 1;

  if (IioIndex < ARRAY_SIZE (IioGlobalData->SetupData.PcieSubSystemMode) &&
      IouIndex < ARRAY_SIZE (IioGlobalData->SetupData.PcieSubSystemMode[0])) {
    Pi5Mode = IioGlobalData->SetupData.PcieSubSystemMode[IioIndex][IouIndex];
  } else {
    ASSERT (FALSE);
    return PI5_MODE_MAX;
  }

  switch (Pi5Mode) {
  case IIO_MODE_GEN4_ONLY:
    ResultMode = PI5_MODE_GEN4_ONLY;
    break;
  case IIO_MODE_CXL:
  case IIO_MODE_FORCE_CXL:
    ResultMode = PI5_MODE_IAL;
    break;
  case IIO_MODE_GEN5:
    PortIndex  = GetPortIndexbyStack (Stack, IioPortH);
    if (CheckNtbPortConfSetting (IioIndex, PortIndex)) {
      ResultMode = PI5_MODE_NTB;
    } else {
      ResultMode = PI5_MODE_GEN5_AND_GEN4;
    }
    break;
  default:
    IIO_D_ERR ("[%a] PcieSubSystemMode unsupported %d\n", __FUNCTION__, Pi5Mode);
    ASSERT (FALSE);
    break;
  }

  return ResultMode;
}

/**
    This function hides PCIE Port menu in setup menu for ports that can not be configured.

    @param[in] IioIndex      - Socket number of the port being initialized.
    @param[in] PortIndex     - Port number being initialized.
    @param[in] IioGlobalData - Pointer to IIO globals.
**/
VOID
SetPEXPHideForNotConfiguredPorts (
  IN     UINT8                           IioIndex,
  IN     UINT8                           PortIndex,
  IN OUT IIO_GLOBALS                     *IioGlobalData
  )
{
  UINT8                                  MaxPortNumberPerSocket;
  UINT16                                 TotalPortIndex;

  if (!IioVData_ConfigurePciePort (IioGlobalData, IioIndex, PortIndex)) {
    MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];
    TotalPortIndex = IioIndex * MaxPortNumberPerSocket + PortIndex;

    IioGlobalData->SetupData.PEXPHIDE[TotalPortIndex] = 0x1;
    IioGlobalData->SetupData.HidePEXPMenu[TotalPortIndex] = 0x1;
    IIO_D_PCILOG ("%a Hiding PXPMenu for [%d.%d]\n", __FUNCTION__, IioIndex, PortIndex);
  }
}


/**
  Checks whether it is HCC SKU or not.

  @param[in] IioIndex - IIO instance index (usually socket)

  @retval TRUE    if it is HCC SKU
  @retval FALSE   otherwise
**/
BOOLEAN
IioIsHccSku (
  UINT8 IioIndex
  )
{
  return (GetChopType (IioIndex) == TypeHcc);
}

/**
  Checks whether it is LCC SKU or not.

  @param[in] IioIndex - IIO instance index (usually socket)

  @retval TRUE    if it is LCC SKU
  @retval FALSE   otherwise
**/
BOOLEAN
IioIsLccSku (
  UINT8 IioIndex
  )
{
  return (GetChopType (IioIndex) == TypeLcc);
}

/**
  Checks whether the given channel index is Vc1 or Vcm.

  @param[in] IioIndex     - Socket Index
  @param[in] StackIndex   - MS2IOSF stack index
  @param[in] ChannelIndex - MS2IOSF stack channel index
  @param[in] CheckVc1     - Check if this MS2IOSF channel is assigned to Vc1
  @param[in] CheckVcm     - Check if this MS2IOSF channel is assigned to Vcm

  @retval TRUE    if given channel is Vc1 or Vcm based on input check
  @retval FALSE   otherwise
**/
BOOLEAN
IsVc1OrVcm (
  IN UINT8    IioIndex,
  IN UINT8    StackIndex,
  IN UINT8    ChannelIndex,
  IN BOOLEAN  CheckVc1,
  IN BOOLEAN  CheckVcm
  )
{
  if (!(IioIndex == SOCKET_0_INDEX && StackIndex == IIO_STACK0)) {
    //
    // Only DMI/Rlink currently supported
    //
    return FALSE;
  }

  if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    if (CheckVc1 && ChannelIndex == Ch5) {
      return TRUE;
    } else if (CheckVcm && ChannelIndex == Ch2) {
      return TRUE;
    }
  }

  if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
    if (CheckVc1 && ChannelIndex == Ch5) {
      return TRUE;
    } else if (CheckVcm && ChannelIndex == Ch2) {
      return TRUE;
    }
  }

  if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    if (CheckVc1 && ChannelIndex == Ch1) {
      return TRUE;
    } else if (CheckVcm && ChannelIndex == Ch2) {
      return TRUE;
    }
  }

  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    if (CheckVc1 && ChannelIndex == Ch6) {
      return TRUE;
    } else if (CheckVcm && ChannelIndex == Ch5) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Get HCX sub-system type
  @param[in] IioIndex

  @retval    The HCX sub-system type.
**/
IIO_HCX_TYPE
EFIAPI
GetHcxType (
  IN  UINT8       IioIndex
  )
{
  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
// SPR_TODO    return (GetChopType (IioIndex) == TypeMcc) ? IIO_HCB : IIO_HCA;
    return IIO_HCA;
  } else {
    return IIO_HCX_NONE;
  }
}
