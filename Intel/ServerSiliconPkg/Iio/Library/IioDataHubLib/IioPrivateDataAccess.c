/** @file
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

#include <Uefi.h>
#include <IioRegs.h>
#include <Library/IioDataHubLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <LibraryPrivate/IioDebug.h>

IIO_CONFIG*
GetIioConfig (VOID)
{
  IIO_GLOBALS* IioGlobalData;

  GetIioGlobalData (&IioGlobalData);
  return &IioGlobalData->SetupData;
}

IIO_V_DATA*
GetIioVData (VOID)
{
  IIO_GLOBALS* IioGlobalData;
  GetIioGlobalData (&IioGlobalData);

  return &IioGlobalData->IioVar.IioVData;
}

IIO_OUT_DATA*
GetIioOutData (VOID)
{
  IIO_GLOBALS* IioGlobalData;
  GetIioGlobalData (&IioGlobalData);

  return &IioGlobalData->IioVar.IioOutData;
}

UINT8
GetSocketPortBusNumber (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex
  )
{
  IIO_GLOBALS* IioGlobalData;
  GetIioGlobalData (&IioGlobalData);

  return IioGlobalData->IioVar.IioVData.SocketPortBusNumber[IioIndex][PortIndex];
}

/**
  Function to obtain bus number when the stack is the Uncore.
  @param[in] IioIndex               - The socket number
  @param[in] UncoreStackIndex       - The index of the uncore stack
  @param[in] UncoreBusNumber        - Pointer to the value that represents the bus number (output)

  @return EFI_INVALID_PARAMETER     When invalid case options
  @return EFI_SUCCESS               When a valid assignment is done
**/
EFI_STATUS
GetSocketUncoreBusNumber (
  IN    UINT8      IioIndex,
  IN    UINT8      UncoreStackIndex,
  OUT   UINT8      *UncoreBusNumber
  )
{
  IIO_GLOBALS* IioGlobalData;

  GetIioGlobalData (&IioGlobalData);
  if ((IioIndex < ARRAY_SIZE (IioGlobalData->IioVar.IioVData.SocketUncoreBusNumber)) &&
      (UncoreStackIndex < ARRAY_SIZE (IioGlobalData->IioVar.IioVData.SocketUncoreBusNumber[0]))) {
    *UncoreBusNumber = IioGlobalData->IioVar.IioVData.SocketUncoreBusNumber[IioIndex][UncoreStackIndex];
    return EFI_SUCCESS;
  } else {
    ASSERT (IioIndex < ARRAY_SIZE (IioGlobalData->IioVar.IioVData.SocketUncoreBusNumber));
    ASSERT (UncoreStackIndex < ARRAY_SIZE (IioGlobalData->IioVar.IioVData.SocketUncoreBusNumber[0]));
    return EFI_INVALID_PARAMETER;
  }
}

UINT8
GetPciePortDevice (
  IN UINT8    PortIndex
  )
{
  IIO_GLOBALS *IioGlobalData;
  GetIioGlobalData (&IioGlobalData);

  return IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].Device;
}

UINT8
GetPciePortFunction (
  IN UINT8    PortIndex
  )
{
  IIO_GLOBALS *IioGlobalData;
  GetIioGlobalData (&IioGlobalData);

  return IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].Function;
}

/**
  Retrieve super cluster port of PCIe port

  @param[in] PortIndex - Index to the IIO port

  @return Super cluster port of IIO port is returned.
**/
UINT8
GetPciePortSuperClusterPort (
  IN UINT8    PortIndex
  )
{
  IIO_GLOBALS *IioGlobalData;

  GetIioGlobalData (&IioGlobalData);
  if (PortIndex >= ARRAY_SIZE (IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo)) {

    ASSERT (FALSE);
    return 0;
  }
  return IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].SuperClusterPort;
}


INTEL_IIO_PORT_INFO*
GetPciePortInfo (
  IN UINT8    PortIndex
  )
{
  IIO_GLOBALS *IioGlobalData;
  GetIioGlobalData (&IioGlobalData);

  return &IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex];
}

UINT8
GetPciePortOwnership (
  IN UINT8     IioIndex,
  IN UINT8     PortIndex
  )
{
  UINT32       MaxPortNumberPerSocket;
  UINT32       Index;
  IIO_GLOBALS *IioGlobalData;

  GetIioGlobalData (&IioGlobalData);
  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);

    //
    // If statement to resolve KW issue
    //
  Index = (IioIndex * MaxPortNumberPerSocket) + PortIndex;
  if (Index >= NELEMENTS (IioGlobalData->IioVar.IioOutData.PciePortOwnership)) {
    ASSERT (FALSE);
    return REGULAR_PCIE_OWNERSHIP;
  }

  return IioGlobalData->IioVar.IioOutData.PciePortOwnership[(IioIndex * MaxPortNumberPerSocket) + PortIndex];
}

/**
  Get stack index for given IIO port index on given IIO instance.

  @param[in]      IioIndex       - IIO instance (usually socket).
  @param[in]      PortIndex      - IIO port index
  @param[out]     StackIndex     - pointer to index of IIO stack

  @retval                        - EFI_INVALID_PARAMETER: input parameters out of range
                                   EFI_SUCCESS: the function is excuted successfully
**/
EFI_STATUS
GetPcieStackPerPort (
  IN     UINT8                     IioIndex,
  IN     UINT8                     PortIndex,
     OUT UINT8                     *StackIndex
  )
{
  IIO_GLOBALS                      *IioGlobalData;

  GetIioGlobalData (&IioGlobalData);
  if (IioIndex >= NELEMENTS (IioGlobalData->IioVar.IioVData.StackPerPort) ||
      PortIndex >= NELEMENTS (IioGlobalData->IioVar.IioVData.StackPerPort[IioIndex])) {
    IIO_D_PCIERR ("[%d p%d] Stack not found!\n", IioIndex, PortIndex);
    ASSERT (FALSE);
    *StackIndex = 1;
    return EFI_INVALID_PARAMETER;
  }
  *StackIndex = IioGlobalData->IioVar.IioVData.StackPerPort[IioIndex][PortIndex];
  return EFI_SUCCESS;
}

/**
  Retrieve the number of PCI ports in IIO instance.

  @param[in] IioIndex - IIO instance (usually socket).

  @return Number of PCI ports is returned, or 0 if IioIndex is invalid.
**/
UINT8
GetMaxPciePortNumberPerSocket (
  IN UINT8    IioIndex
  )
{
  IIO_GLOBALS *IioGlobalData;

  GetIioGlobalData (&IioGlobalData);

  if (IioIndex >= NELEMENTS (IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket)) {
    ASSERT(FALSE);
    return 0;
  }
  return IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];
}

VOID
GetPciePortBusDeviceFunction (
  IN UINT8                           IioIndex,
  IN UINT8                           PortIndex,
  IN OUT PCI_ROOT_BRIDGE_PCI_ADDRESS *PciAddress
  )
{
  IIO_GLOBALS*                       IioGlobalData;

  //
  // If statements to resolve KW issue
  //
  if (IioIndex >= MaxIIO || PortIndex >= NELEMENTS (IioGlobalData->IioVar.IioVData.SocketPortBusNumber[IioIndex])) {
    ASSERT (FALSE);
    return;
  }
  if (PortIndex >= NELEMENTS (IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo)) {
    ASSERT (FALSE);
    return;
  }

  GetIioGlobalData (&IioGlobalData);
  PciAddress->Bus = IioGlobalData->IioVar.IioVData.SocketPortBusNumber[IioIndex][PortIndex];
  if (CheckNtbPortConfSetting (IioIndex, PortIndex)) {
    PciAddress->Device = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].NtbDevice;
    PciAddress->Function = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].NtbFunction;
  } else {
    PciAddress->Device = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].Device;
    PciAddress->Function = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].Function;
  }
  PciAddress->Register = 0;
  PciAddress->ExtendedRegister = 0;
}
