/** @file
  ACPI Platform Driver Hooks

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2020 Intel Corporation. <BR>

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

//
// Statements that include other files
//
#include <Library/PlatformSpecificAcpiTableLib.h>


extern EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE *mSpcrTable;


/**

  Test whether DevicePath is a valid Terminal

  @param DevicePath  -   DevicePath to be checked
  @param TerminalType-   If is terminal, give its type
  @param Com         -   If is Com Port, give its type

  @retval TRUE        -   If DevicePath point to a Terminal
  @retval FALSE

**/
BOOLEAN
IsTerminalDevicePath (
  IN  EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  OUT UINT8                    *TerminalType,
  OUT UINTN                    *Com
  )
{
  UINT8                 *Ptr;
  VENDOR_DEVICE_PATH    *Vendor;
  ACPI_HID_DEVICE_PATH  *Acpi;

  Ptr = (UINT8 *) DevicePath;
  Ptr    = Ptr + GetDevicePathSize (DevicePath) -
           sizeof (VENDOR_DEVICE_PATH) - sizeof (EFI_DEVICE_PATH_PROTOCOL);
  Vendor = (VENDOR_DEVICE_PATH *) Ptr;

  if (Vendor->Header.Type == MESSAGING_DEVICE_PATH &&
      Vendor->Header.SubType == MSG_VENDOR_DP &&
      DevicePathNodeLength (&Vendor->Header) == sizeof(VENDOR_DEVICE_PATH)) {
    //
    // There are four kinds of Terminal types
    // check to see whether this devicepath
    // is one of that type
    //
    if (CompareGuid (&Vendor->Guid, &gEfiVT100Guid)) {
      *TerminalType = 0;
    } else if (CompareGuid (&Vendor->Guid, &gEfiVT100PlusGuid)) {
      *TerminalType = 1;
    } else if (CompareGuid (&Vendor->Guid, &gEfiVTUTF8Guid)) {
      *TerminalType = 2;
    } else if (CompareGuid (&Vendor->Guid, &gEfiPcAnsiGuid)) {
      *TerminalType = 3;
    } else {
      return FALSE;
    }

    Ptr   = Ptr - sizeof (UART_DEVICE_PATH) - sizeof (ACPI_HID_DEVICE_PATH);
    Acpi  = (ACPI_HID_DEVICE_PATH *) Ptr;
    if (Acpi->Header.Type == ACPI_DEVICE_PATH &&
        Acpi->Header.SubType == ACPI_DP &&
        DevicePathNodeLength (&Acpi->Header) == sizeof (ACPI_HID_DEVICE_PATH) &&
        Acpi->HID == EISA_PNP_ID (0x0501)) {
      *Com = Acpi->UID;
      return TRUE;
    }
  }
  return FALSE;
}

/*++

Routine Description:

  This function updates SPCR table.

  Note that user may change CR settings via setup or other methods.
  The SPCR table must match.

Arguments:

  None

Returns:

  TRUE - SPCR Table updated and is valid.

--*/
BOOLEAN
UpdateSpcrTable (
  VOID
  )
{
  EFI_STATUS Status;
  UINT8      BaudRate;
  UINT8      TerminalType;
  UINT32     Control;
  UINTN      Com;
  UINTN      Size;
  EFI_SERIAL_IO_PROTOCOL   *SerialIo;
  EFI_DEVICE_PATH_PROTOCOL *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL *TempDevicePath;
  EFI_DEVICE_PATH_PROTOCOL *DevicePathInstance;
  EFI_HANDLE               Handle;
  UINTN                    Index;
  EFI_HANDLE               *Handles;
  UINTN                    HandleCount;
  EFI_SIO_PROTOCOL         *Sio;
  ACPI_RESOURCE_HEADER_PTR ResourceList;
  UINT16                   ComBaseAddress;
  UINT16                   ComIrq;

  if (mSpcrTable == NULL) {
    return FALSE;
  }

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSioProtocolGuid,
                  NULL,
                  &HandleCount,
                  &Handles
                  );
  if (EFI_ERROR (Status)) {
    HandleCount = 0;
    Handles     = NULL;
  }

  ComBaseAddress = 0;
  ComIrq = 0;
  for (Index = 0; Index < HandleCount; Index++) {
    DevicePath = DevicePathFromHandle (Handles[Index]);

    if (DevicePath == NULL){
      continue;
    }

    while (IsDevicePathEnd (DevicePath)) {
      if (DevicePathType (DevicePath) == ACPI_DEVICE_PATH && DevicePathSubType (DevicePath) == ACPI_DP) {
        if (((ACPI_HID_DEVICE_PATH *) DevicePath)->HID == EISA_PNP_ID (0x501)) {
          break;
        }
      }
      DevicePath = NextDevicePathNode (DevicePath);
    }

    if (!IsDevicePathEnd (DevicePath)) {
      Status = gBS->HandleProtocol (Handles[Index], &gEfiSioProtocolGuid, (VOID **) &Sio);
      ASSERT_EFI_ERROR (Status);

      Status = Sio->GetResources (Sio, &ResourceList);
      ASSERT_EFI_ERROR (Status);

      while (ResourceList.SmallHeader->Byte != ACPI_END_TAG_DESCRIPTOR) {

        if (ResourceList.SmallHeader->Byte == ACPI_FIXED_LOCATION_IO_PORT_DESCRIPTOR) {
          ComBaseAddress = ((EFI_ACPI_FIXED_LOCATION_IO_PORT_DESCRIPTOR *) ResourceList.SmallHeader)->BaseAddress;
        } else if (ResourceList.SmallHeader->Byte == ACPI_IRQ_NOFLAG_DESCRIPTOR) {
          ComIrq = ((EFI_ACPI_IRQ_NOFLAG_DESCRIPTOR *) ResourceList.SmallHeader)->Mask;
        }


        if (ResourceList.SmallHeader->Bits.Type == 0) {
          ResourceList.SmallHeader = (ACPI_SMALL_RESOURCE_HEADER *) ((UINT8 *) ResourceList.SmallHeader
                                  + ResourceList.SmallHeader->Bits.Length
                                  + sizeof (*ResourceList.SmallHeader));
        } else {
          ResourceList.LargeHeader = (ACPI_LARGE_RESOURCE_HEADER *) ((UINT8 *) ResourceList.LargeHeader
                                  + ResourceList.LargeHeader->Length
                                  + sizeof (*ResourceList.LargeHeader));
        }
      }
    }

    if (ComBaseAddress != 0 && ComIrq != 0) {
      break;
    }
  }


  if (ComBaseAddress == 0 || ComIrq == 0) {
    return FALSE;
  }
  mSpcrTable->BaseAddress.Address = ComBaseAddress;
  mSpcrTable->Irq = (UINT8) LowBitSet32 (ComIrq);
  mSpcrTable->GlobalSystemInterrupt = mSpcrTable->Irq;
  CopyMem (mSpcrTable->Header.OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (mSpcrTable->Header.OemId));
  mSpcrTable->Header.OemTableId = PcdGet64 (PcdAcpiDefaultOemTableId);
  mSpcrTable->Header.CreatorId = PcdGet32 (PcdAcpiDefaultCreatorId);
  mSpcrTable->Header.CreatorRevision = PcdGet32 (PcdAcpiDefaultCreatorRevision);

  //
  // Locate the COM1 terminal
  //

  Status = GetVariable2 (
              L"ConIn",
              &gEfiGlobalVariableGuid,
              (VOID**) &DevicePath,
              NULL);
  if (DevicePath == NULL) {
    return FALSE;
  }

  TerminalType = 0;
  TempDevicePath = DevicePath;
  do {
    DevicePathInstance = GetNextDevicePathInstance (&TempDevicePath, &Size);
    if (DevicePathInstance == NULL) {
      break;
    }
    if (IsTerminalDevicePath (DevicePathInstance, &TerminalType, &Com) && Com == 0) {
      break;
    }
    gBS->FreePool (DevicePathInstance);
    DevicePathInstance = NULL;
  } while (TempDevicePath != NULL);

  if (DevicePathInstance == NULL) {
    gBS->FreePool (DevicePath);
    return FALSE;
  }

  TempDevicePath = DevicePathInstance;
  Status = gBS->LocateDevicePath (
                  &gEfiSerialIoProtocolGuid,
                  &TempDevicePath,
                  &Handle
                  );
  gBS->FreePool (DevicePathInstance);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiSerialIoProtocolGuid,
                  (VOID **) &SerialIo
                  );
  ASSERT_EFI_ERROR (Status);

  switch (SerialIo->Mode->BaudRate) {
  case 9600:
    BaudRate = 3;
    break;
  case 19200:
    BaudRate = 4;
    break;
  case 57600:
    BaudRate = 6;
    break;
  case 115200:
    BaudRate = 7;
    break;
  default:
    DEBUG(( EFI_D_ERROR, "\nSPCR: Unsupported baud rate!\n" ));
    goto UpdateSpcrExit;
    break;
  }
  mSpcrTable->BaudRate = BaudRate;
  if (SerialIo->Mode->Parity != NoParity) {
    DEBUG(( EFI_D_ERROR, "\nSPCR: Only support no parity!\n" ));
    goto UpdateSpcrExit;
  }
  if (SerialIo->Mode->DataBits != 8) {
    DEBUG(( EFI_D_ERROR, "\nSPCR: Only support 8 data bits!\n" ));
    goto UpdateSpcrExit;
  }
  if (SerialIo->Mode->StopBits != OneStopBit) {
    DEBUG(( EFI_D_ERROR, "\nSPCR: Only support 1 stop bit!\n" ));
    goto UpdateSpcrExit;
  }
  Status = SerialIo->GetControl (
                        SerialIo,
                        &Control
                        );
  ASSERT_EFI_ERROR (Status);

  if (Control & EFI_SERIAL_HARDWARE_FLOW_CONTROL_ENABLE) {
    mSpcrTable->FlowControl |= 2;
  }
  mSpcrTable->TerminalType = TerminalType;
  return TRUE;

UpdateSpcrExit:
  return FALSE;
}


EFI_STATUS
PatchSpcrAcpiTable (
  IN OUT  EFI_ACPI_COMMON_HEADER  *Table
  )
{
  EFI_STATUS    Status = EFI_SUCCESS;

  if(!UpdateSpcrTable()) {
    Status = EFI_NOT_READY;
  }
  return Status;
}
