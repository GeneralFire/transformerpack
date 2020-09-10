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

#include <Base.h>
#include <Uefi.h>

#include <PcieRegs.h>
#include <IndustryStandard/PciExpress21.h>

#include <Library/DebugLib.h>
#include <Library/CpuAndRevisionLib.h>

#include <IioPlatformData.h>
#include <Library/IioAccessLib.h>
#include <Library/IioDataHubLib.h>
#include <Library/IioApiLib.h>
#include <Library/IioVmdInit.h>
#include <LibraryPrivate/IioDebug.h>
#include <LibraryPrivate/IioPcieLib.h>


/**
  Get Vtd base address

  @param[in] SocId    - Socket Index
  @param[in] StackId    - Stack Index
  @retval    Vtd base address
**/
UINT32
EFIAPI
GetVtdBar (
  IN UINT8     SocId,
  IN UINT8     StackId
  )
{
  IIO_GLOBALS *IioGlobalData;
  GetIioGlobalData (&IioGlobalData);

  return (UINT32)IioGlobalData->IioVar.IioVData.VtdBarAddress[SocId][StackId];
}

/**
  Get socket ending bus number

  @param[out] BusNumber - BusNumber
  @param[in]  SocId    - Socket Index
  @param[in]  StackId    - Stack Index
  @retval EFI_SUCCESS provided bus number
  @retval EFI_INVALID_PARAMETER  if NULL BusNumber pointer passed
**/
EFI_STATUS
EFIAPI
GetSocketStackLimitBus (
  OUT UINT8    *BusNumber,
  IN UINT8     SocId,
  IN UINT8     StackId
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  IIO_GLOBALS *IioGlobalData;

  ASSERT (BusNumber != NULL);

  GetIioGlobalData (&IioGlobalData);

  if (BusNumber == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (SocId >= ARRAY_SIZE (IioGlobalData->IioVar.IioVData.SocketStackLimitBusNumber));
  ASSERT (StackId >= ARRAY_SIZE (IioGlobalData->IioVar.IioVData.SocketStackLimitBusNumber[0]));

  *BusNumber = IioGlobalData->IioVar.IioVData.SocketStackLimitBusNumber[SocId][StackId];

  return Status;
}

/**
  Get max port number every socket

  @param[in] SocId    - Socket Index
  @retval    Max port number per socket
**/
UINT8
EFIAPI
GetMaxPortPerSocket (
  IN UINT8     SocId
  )
{
  IIO_GLOBALS *IioGlobalData;
  GetIioGlobalData (&IioGlobalData);

  return IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[SocId];
}

/**
  Get the stack which the PCIe port belongs to

  @param[in] SocId     - Socket Index
  @param[in] PortIndex - Port Index
  @retval    Stack id
**/
UINT8
EFIAPI
GetStackPerPort (
  IN UINT8     SocId,
  IN UINT8     PortIndex
  )
{
  return IioGetStackForPort (SocId, PortIndex);
}

/**
  Get the bus number which the PCIe port belongs to

  @param[in] SocId     - Socket Index
  @param[in] PortIndex - Port Index
  @retval    bus number
**/
UINT8
EFIAPI
GetSocketPortBusNum (
  IN UINT8     SocId,
  IN UINT8     PortIndex
  )
{
  IIO_GLOBALS *IioGlobalData;
  GetIioGlobalData (&IioGlobalData);

  return IioGlobalData->IioVar.IioVData.SocketPortBusNumber[SocId][PortIndex];
}

/**
  Get current PCIe port map

  @param[in] SocId     - Socket Index
  @param[in] PortIndex - Port Index
  @retval    Current PCIe port map
**/
UINT8
EFIAPI
GetCurrentPXPMap (
  IN UINT8     SocId,
  IN UINT8     PortIndex
  )
{
  UINT32  MaxPortNumberPerSocket;
  IIO_GLOBALS *IioGlobalData;

  GetIioGlobalData (&IioGlobalData);
  MaxPortNumberPerSocket = GetMaxPortPerSocket (SocId);

  return IioGlobalData->IioVar.IioOutData.CurrentPXPMap[(SocId*(MaxPortNumberPerSocket))+PortIndex];
}

/**
  Get PCIe port ownership

  @param[in] SocId     - Socket Index
  @param[in] PortIndex - Port Index
  @retval    PCIe port ownership
**/
UINT8
EFIAPI
GetPCIePortOwnerShip (
  IN UINT8     SocId,
  IN UINT8     PortIndex
  )
{
  UINT32  MaxPortNumberPerSocket;
  IIO_GLOBALS *IioGlobalData;

  GetIioGlobalData (&IioGlobalData);
  MaxPortNumberPerSocket = GetMaxPortPerSocket (SocId);

  return IioGlobalData->IioVar.IioOutData.PciePortOwnership[(SocId * MaxPortNumberPerSocket) + PortIndex];
}

/**
  Check if VMD is enabled for given IIO port.

  @param[in] IioIndex - IIO instance (usually socket)
  @param[in] IioPort  - IIO port index in IIO instance

  @return TRUE is returned if VMD is enabled for IIO port, otherwise FALSE.
**/
BOOLEAN
EFIAPI
IioVmdPortIsEnabled (
  IN  UINT8    IioIndex,
  IN  UINT8    IioPort
  )
{
  IIO_GLOBALS *IioGlobalData;
  GetIioGlobalData (&IioGlobalData);

  return IsVMDEnabledForPort (IioIndex, IioPort);
}


/**
  Return VMD device location for IIO stack.

  @param[in] IioIndex   - IIO instance (usually socket).
  @param[in] IioStack   - IIO stack index in IIO instance.
  @param[in] PciDevPtr  - Pointer to a buffer for PCI device number of VMD device.
  @param[in] PciFuncPtr - Pointer to a buffer for PCI function number of VMD device.

  @retval EFI_INVALID_PARAMETER - Invalid IIO instance or stack.
  @retval EFI_NOT_STARTED       - VMD is not enabled in the given IIO stack.
  @retval EFI_SUCCESS           - VMD is enabled, device and function number updated.
**/
EFI_STATUS
EFIAPI
IioVmdGetPciLocation (
  IN  UINT8  IioIndex,
  IN  UINT8  IioStack,
  OUT UINT8  *PciDevPtr,
  OUT UINT8  *PciFuncPtr
  )
{
  EFI_STATUS   Status;
  IIO_GLOBALS *IioGlobalData;

  Status = GetIioGlobalData (&IioGlobalData);

  if (IioIndex >= MAX_SOCKET || IioStack >= MAX_VMD_STACKS_PER_SOCKET ||
      EFI_ERROR (Status)) {

    IIO_D_ERR ("%a: Invalid stack [%d.%d] or IIO HOB not available (%r)\n",
               __FUNCTION__, IioIndex, IioStack, Status);
    return EFI_INVALID_PARAMETER;
  }
  if (IioGlobalData->SetupData.VMDEnabled[IioIndex][IioStack] == 0) {
    return EFI_NOT_STARTED;
  }
  if (PciDevPtr != 0) {

    if (IsCpuAndRevision (CPU_SKX, REV_ALL) ||
        IsCpuAndRevision (CPU_CLX, REV_ALL) ||
        IsCpuAndRevision (CPU_CPX, REV_ALL)) {
      *PciDevPtr = IIO_VMD_DEV_14NM;
    } else {
      *PciDevPtr = IIO_VMD_DEV_10NM;
    }
  }
  if (PciFuncPtr != 0) {

    *PciFuncPtr = IIO_VMD_FUNC;
  }
  return EFI_SUCCESS;
}

/**
  Returns TRUE if given PCH rootport can be assigned to VMD

  @param[in] IioIndex   - IIO instance (usually socket).
  @param[in] PortId     - Index of the PCH rootport.

  @return TRUE if given PCH rootport can be assigned to VMD
 */
BOOLEAN
EFIAPI
IioVmdIsPchPortAllowed (
  IN UINT8  IioIndex,
  IN UINT8  PortId
  )
{
  return IioIsPchPortAllowed (IioIndex, PortId);
}

/**
  Functions sets link params for given PCIE port.

  @param[in]   IioIndex          Port Index
  @param[in]   PortIndex         Port Index
  @param[out]  LinkWidth         Buffer to return current link width
  @param[out]  MaxLinkWidth      Buffer to return max link width
  @param[out]  LinkSpeed         Buffer to return current link speed
 */
VOID
IioGetPciePortLinkStatus (
  IN  UINT8     IioIndex,
  IN  UINT8     PortIndex,
  OUT UINT8     *LinkWidth,
  OUT UINT8     *MaxLinkWidth,
  OUT UINT8     *LinkSpeed
  )
{
  IIO_GLOBALS                   *IioGlobalData;
  EFI_STATUS                    Status;
  PCI_REG_PCIE_LINK_STATUS      LnkSts;

  Status = GetIioGlobalData (&IioGlobalData);

  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // DMI config data is not available in DXE phase - get link status values from IioGlobalData
  //
  if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort) &&
      (IioGlobalData->IioVar.IioVData.DmiSocketMap & (1 << IioIndex))) {
    *LinkWidth = IioGlobalData->IioVar.IioVData.DmiSocketData[IioIndex].LinkWidth;
    *LinkSpeed = IioGlobalData->IioVar.IioVData.DmiSocketData[IioIndex].LinkSpeed;
    *MaxLinkWidth = IioGetLanesNumOfPort (IioGlobalData, IioIndex, PortIndex);
    return;
  }

  //
  // MaxLinkWidth is depending on current bifurcation
  //
  *MaxLinkWidth = IioGetLanesNumOfPort (IioGlobalData, IioIndex, PortIndex);

  //
  // Check if link is trained
  //
  Status = GetPcieCapReg16 (IioIndex, PortIndex, R_PCIE_LSTS_OFFSET, &LnkSts.Uint16);
  if (EFI_ERROR (Status)) {
    IIO_D_PCIERR ("[%d.%d] Can't read link status register.\n", IioIndex, PortIndex, Status);
    return;
  }

  //
  // Can't trust Negotiated Link Width to be zero if not linked, so need to check port-linked bit
  //
  if (LnkSts.Bits.DataLinkLayerLinkActive == 1) {
    *LinkWidth = (UINT8) LnkSts.Bits.NegotiatedLinkWidth;
  } else {
    *LinkWidth = 0;
  }

  *LinkSpeed = (UINT8) LnkSts.Bits.CurrentLinkSpeed;
}

/**
  Return stack bus per socket

  @param[in] SocId      - Socket Index
  @param[in] StackId    - Stack Index
  @retval    Stack bus per socket
**/
UINT8
EFIAPI
GetSocketStackBus (
  IN UINT8     SocId,
  IN UINT8     StackId
  )
{
  IIO_GLOBALS *IioGlobalData;
  GetIioGlobalData (&IioGlobalData);

  return IioGlobalData->IioVar.IioVData.SocketStackBus[SocId][StackId];
}

/**
  This function handles WAs needed in Intel IPs for given Segment, Bus, Device, Function
  in early PCIe enumeration.

  @param[in] Segment                 - Segment
  @param[in] Bus                     - Bus
  @param[in] Device                  - Device of the virtual root port
  @param[in] Function                - Function of the virtual root port
  @param[in] DidVid                  - Device-Vendor ID in the given Bus, Device, Function.
**/
VOID
IioPciHookBeforeEnumeration (
  IN  UINT8          Segment,
  IN  UINT8          Bus,
  IN  UINT8          Device,
  IN  UINT8          Function,
  IN  UINT32         DidVid
  )
{
  IioPciIpHookBeforeEnumeration (Segment, Bus, Device, Function, DidVid);
}
