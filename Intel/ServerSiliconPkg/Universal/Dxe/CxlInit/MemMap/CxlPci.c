/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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
**/

#include "CxlDxe.h"

/**
  Reads PCI controller registers in PCI configuration space

  @param  PciIo                 A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Offset                The offset within the PCI configuration space for the PCI controller.
  @param  Size                  The size in bytes to read.
  @param  Buffer                The destination buffer to store the results.

  @retval EFI_SUCCESS           The data was read from to the PCI controller.
  @return others                Some error occurred when reading PCI data.

**/
EFI_STATUS
ReadPciCfgData (
  IN  EFI_PCI_IO_PROTOCOL         *PciIo,
  IN  UINT32                      Offset,
  IN  UINTN                       Size,
  OUT VOID                        *Buffer
  )
{
  EFI_STATUS                      Status;
  UINT64                          Address;
  UINTN                           Segment;
  UINTN                           Bus;
  UINTN                           Device;
  UINTN                           Function;

  Status = PciIo->GetLocation (PciIo, &Segment, &Bus, &Device, &Function);
  if (!EFI_ERROR (Status)) {
    Address = PCI_SEGMENT_LIB_ADDRESS (Segment, Bus, Device, Function, Offset);
    PciSegmentReadBuffer (Address, Size, Buffer);
  }

  return Status;
}


/**
  Get PCI device serial number for the PCI IO protocol instance

  @param[in]  PciIo               A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param[out] SerialNumber        Output serial number on the PCI device.

  @retval EFI_SUCCESS             Serial number is successfully retrieve on the PCI device.
  @retval EFI_NOT_FOUND           No PCIE serial number is found on the PCI device.
  @retval EFI_DEVICE_ERROR        The PCI device does not follow PCIE spec

**/
EFI_STATUS
GetPciSerialNumber (
  IN  EFI_PCI_IO_PROTOCOL         *PciIo,
  OUT UINT64                      *SerialNumber
  )
{
  EFI_STATUS                                       Status;
  UINT32                                           ExCapOffset;
  PCI_EXPRESS_EXTENDED_CAPABILITIES_HEADER         CapHeader;
  PCI_EXPRESS_EXTENDED_CAPABILITIES_SERIAL_NUMBER  CapSerialNumber;

  ExCapOffset = EFI_PCIE_CAPABILITY_BASE_OFFSET;
  do {
    Status = ReadPciCfgData (PciIo, ExCapOffset, sizeof (CapHeader), &CapHeader);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    if (CapHeader.CapabilityId == PCI_EXPRESS_EXTENDED_CAPABILITY_SERIAL_NUMBER_ID) {
      Status = ReadPciCfgData (PciIo, ExCapOffset, sizeof (CapSerialNumber), &CapSerialNumber);
      if (EFI_ERROR (Status)) {
        return Status;
      }
      *SerialNumber = CapSerialNumber.SerialNumber;
      return EFI_SUCCESS;
    }
    ExCapOffset = CapHeader.NextCapabilityOffset;
    if (ExCapOffset == 0) {
      return EFI_NOT_FOUND;
    }
    //
    // Ensure the PCIE header is fully compatible with PCIE spec:
    // "For Extended Capabilities implemented in Configuration Space,
    // this offset is relative to the beginning of PCI compatible Configuration Space
    // and thus must always be either 000h (for terminating list of Capabilities)
    // or greater than 0FFh.
    // The bottom 2 bits of this offset are Reserved and must be implemented as 00b
    // although software must mask them to allow for future uses of these bits."
    //
  } while ((ExCapOffset > EFI_PCIE_CAPABILITY_BASE_OFFSET) && ((ExCapOffset & (BIT0 | BIT1)) == 0));

  return EFI_DEVICE_ERROR;
}


/**
  Set CXL node handle by matching CXL vendor ID / Serial number.

  @param[in] CpuCsrAccessVar      Pointer to CpuCsrAccessVar.
  @param[in] SerialNumber         PCI device serial number.
  @param[in] VendorID             PCI device vendor id.
  @param[in] Segment              PCI device segment number.
  @param[in] Bus                  PCI device bus number.

**/
VOID
SetCxlNodeHandle (
  IN CPU_CSR_ACCESS_VAR           *CpuCsrAccessVar,
  IN UINT64                       SerialNumber,
  IN UINT16                       VendorId,
  IN UINTN                        Segment,
  IN UINTN                        Bus
  )
{
  UINT8                           SocketId;
  CXL_NODE_DATA                   *CxlNode;
  LIST_ENTRY                      *Link;
  LIST_ENTRY                      *Header;

  Header = &gCxlData.List;
  for (Link = GetFirstNode (Header); !IsNull (Header, Link); Link = GetNextNode (Header, Link)) {
    CxlNode = CXL_NODE_FROM_LIST (Link);
    if ((CxlNode->Info.VendorId != VendorId) || (CxlNode->Info.SerialNumber != SerialNumber)) {
      continue;
    }
    SocketId = CxlNode->SocketId;
    if (CpuCsrAccessVar->segmentSocket[SocketId] != Segment) {
      continue;
    }
    if ((Bus >= CpuCsrAccessVar->SocketFirstBus[SocketId]) && (Bus <= CpuCsrAccessVar->SocketLastBus[SocketId])) {
      CxlNode->Handles[CxlNode->HandleCount].Segment = (UINT8) Segment;
      CxlNode->Handles[CxlNode->HandleCount].Bus = (UINT8) Bus;
      CxlNode->HandleCount++;
      DEBUG ((EFI_D_INFO, "[CXL] CXL device VID: %x SN: %lx is located PCI segment %d bus 0x%x\n",
        SerialNumber, VendorId, Segment, Bus));
    }
  }
}


/**
  Calculates CXL PCI handles.

  This function is invoked after PCI enumeration to get the exact PCI handle for all PCI devices.

**/
VOID
CalculateCxlPciHandles (
  VOID
  )
{
  EFI_STATUS              Status;
  UINTN                   HandleCount;
  EFI_HANDLE              *Handles;
  EFI_PCI_IO_PROTOCOL     *PciIo;
  UINT16                  VendorId;
  UINT64                  SerialNumber;
  UINTN                   Segment;
  UINTN                   Bus;
  UINTN                   Device;
  UINTN                   Function;
  UINTN                   Index;
  CPU_CSR_ACCESS_VAR      *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();
  ASSERT (CpuCsrAccessVar != NULL);

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &Handles
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "[CXL] Failed to find any PCI IO protocol %r!\n", Status));
    return;
  }

  DEBUG ((DEBUG_INFO, "[CXL] PCI IO protocol handle count %d\n", HandleCount));
  for(Index = 0; Index < HandleCount; Index ++) {
    Status = gBS->HandleProtocol (Handles[Index], &gEfiPciIoProtocolGuid, &PciIo);
    if (EFI_ERROR(Status)) {
      continue;
    }
    Status = PciIo->GetLocation (PciIo, &Segment, &Bus, &Device, &Function);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[CXL] Failed to get PCI device location %r\n", Status));
      continue;
    }
    if ((Device != 0) || (Function != 0)) {
      continue; // CXL device must have 0 device and function number.
    }

    DEBUG ((DEBUG_INFO, "[CXL] Get info for PCI Segment %d Bus %d Dev %d Fun %d\n", Segment, Bus, Device, Function));
    Status = ReadPciCfgData (PciIo, PCI_VENDOR_ID_OFFSET, sizeof (VendorId), &VendorId);
    if (EFI_ERROR (Status)) {
      continue;
    }
    Status = GetPciSerialNumber (PciIo, &SerialNumber);
    if (EFI_ERROR (Status)) {
      continue;
    }
    DEBUG ((DEBUG_INFO, "[CXL] Found PCI vendor ID %x and serial number 0x%lx\n", VendorId, SerialNumber));
    SetCxlNodeHandle (CpuCsrAccessVar, SerialNumber, VendorId, Segment, Bus);
  }

  FreePool(Handles);
}
