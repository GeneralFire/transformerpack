/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include <PiPei.h>
#include "SerialPortLibUsb.h"
#include <Library/SerialPortLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>

STATIC USB2_SETUP_PACKET mSetAddress = {
  USB2_REQUEST_TYPE_HOST_TO_DEVICE | USB2_REQUEST_TYPE_STANDARD | USB2_REQUEST_TYPE_DEVICE,
  USB2_REQUEST_SET_ADDRESS,
  {USB2_DEBUG_PORT_DEFAULT_ADDRESS, 0x00},
  0x00,
  0x00
  };

STATIC USB2_SETUP_PACKET mGetDeviceDescriptor = {
  USB2_REQUEST_TYPE_DEVICE_TO_HOST | USB2_REQUEST_TYPE_STANDARD | USB2_REQUEST_TYPE_DEVICE,
  USB2_REQUEST_GET_DESCRIPTOR,
  {0x00, USB2_DESCRIPTOR_TYPE_DEVICE},
  0x0000,
  0x0008
  };

STATIC USB2_SETUP_PACKET mGetDebugDescriptor = {
  USB2_REQUEST_TYPE_DEVICE_TO_HOST | USB2_REQUEST_TYPE_STANDARD | USB2_REQUEST_TYPE_DEVICE,
  USB2_REQUEST_GET_DESCRIPTOR,
  {0x00, USB2_DESCRIPTOR_TYPE_DEBUG},
  0x0000,
  sizeof(USB2_DEBUG_DESCRIPTOR_TYPE)
  };

STATIC USB2_SETUP_PACKET mSetDebugFeature = {
  USB2_REQUEST_TYPE_HOST_TO_DEVICE | USB2_REQUEST_TYPE_STANDARD | USB2_REQUEST_TYPE_DEVICE, // 0x00,
  USB2_REQUEST_SET_FEATURE,                                                                 // 0x03,
  {USB2_FEATURE_DEBUG_MODE, 0x00},                                                          // 0x06, 0x00,
  0x0000,
  0x0000
  };

#ifdef NETCHIP_WORKAROUNDS
STATIC USB2_SETUP_PACKET mSetConfiguration1[8]   = {0x00, 0x09, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
#endif

/**
  Read data from serial device and save the datas in buffer.
  @param[in] *This              Usb2DebugPortInstance pointer
  @param[out] *Buffer           Buffer pointer to receive data.
  @param[out] *Length           Packet length
  @param[in] Timeout            Timeout value in microseconds
  @param[in] DataToggle         Data toggle

  @retval RETURN_SUCCESS        Data successfully received.
  @retval RETURN_DEVICE_ERROR   Device error encountered.
  @retval RETURN_TIMEOUT        Timeout occurred when waiting for data input.
**/
RETURN_STATUS
Usb2DebugPortIn (
  IN  USB2_DEBUG_PORT_INSTANCE        *This,
  OUT UINT8                           *Buffer,
  OUT UINTN                           *Length,
  IN  UINTN                           TimeOut,
  IN  UINT8                           DataToggle
  )
{
  UINTN             BufferIndex;
  UINTN             RetryCount = 3;

  *Length = 0;

  //
  // Setup PIDs
  //
  This->DebugRegister->TokenPid = USB2_PID_TOKEN_IN;
  if (DataToggle) {
    This->DebugRegister->SendPid = USB2_PID_DATA1;
  } else {
    This->DebugRegister->SendPid = USB2_PID_DATA0;
  }

  while (TimeOut && RetryCount) {
    ClrR32Bit(&This->DebugRegister->ControlStatus, USB2_DEBUG_PORT_STATUS_WRITE);
    SetR32Bit(&This->DebugRegister->ControlStatus, USB2_DEBUG_PORT_STATUS_GO);

    //
    // Wait for complete
    //
    while (!IsBitSet(&This->DebugRegister->ControlStatus, USB2_DEBUG_PORT_STATUS_DONE)) {
    }

    if (IsBitSet(&This->DebugRegister->ControlStatus, USB2_DEBUG_PORT_STATUS_ERROR)) {
      --RetryCount;
    }
    else if (This->DebugRegister->ReceivedPid == This->DebugRegister->SendPid) {
        // APTIOV_SERVER_OVERRIDE_RC_START : Added check for condition when Buffer is NULL
        if (Buffer == NULL) {
          return (UINT32) EFI_INVALID_PARAMETER;
        }
        // APTIOV_SERVER_OVERRIDE_RC_END : Added check for condition when Buffer is NULL
       *Length = This->DebugRegister->ControlStatus & USB2_DEBUG_PORT_STATUS_LENGTH;
       for (BufferIndex = 0; BufferIndex < *Length &&
              BufferIndex < USB2_DEBUG_PORT_DEVICE_BUFFER_MAX; ++BufferIndex) {
          Buffer[BufferIndex] = This->DebugRegister->DataBuffer[BufferIndex];
       }
       return RETURN_SUCCESS;
    }
    else MicroSecondDelay(1);
    --TimeOut;
  }

  if (RetryCount == 0) {
    return RETURN_DEVICE_ERROR;
  }

  return RETURN_TIMEOUT;
}

/**
  Write data from input buffer to serial device.

  @param[in] *This              Usb2DebugPortInstance pointer
  @param[in] *Buffer            Buffer pointer to receive data.
  @param[in] *Length            Packet length
  @param[in] Timeout            Timeout value in microseconds
  @param[in] Token              Token id
  @param[in] DataToggle         Data toggle

  @retval RETURN_SUCCESS        Data successfully received.
  @retval RETURN_DEVICE_ERROR   Device error encountered.
  @retval RETURN_TIMEOUT        Timeout occurred when waiting for data input.
**/
RETURN_STATUS
Usb2DebugPortOut (
  IN  USB2_DEBUG_PORT_INSTANCE            *This,
  IN      UINT8                           *Buffer,
  IN      UINTN                           Length,
  IN      UINTN                           TimeOut,
  IN      UINT8                           Token,
  IN      UINT8                           DataToggle
  )
{
  UINTN             BufferIndex;
  UINTN             RetryCount = 3;

  This->DebugRegister->TokenPid = Token;
  if (DataToggle) {
    This->DebugRegister->SendPid = USB2_PID_DATA1;
  } else {
    This->DebugRegister->SendPid = USB2_PID_DATA0;
  }

  for (BufferIndex = 0; BufferIndex < Length; ++BufferIndex) {
    This->DebugRegister->DataBuffer[BufferIndex] = Buffer[BufferIndex];
  }

  //
  // Fill the data length
  //
  ClrR32Bit(&This->DebugRegister->ControlStatus, USB2_DEBUG_PORT_STATUS_LENGTH);
  SetR32Bit((UINT32*)&This->DebugRegister->ControlStatus, (UINT32)Length);
  while (TimeOut && RetryCount) {
    SetR32Bit(&This->DebugRegister->ControlStatus, USB2_DEBUG_PORT_STATUS_WRITE);
    SetR32Bit(&This->DebugRegister->ControlStatus, USB2_DEBUG_PORT_STATUS_GO);

    //
    // Wait for complete
    //
    while (!IsBitSet(&This->DebugRegister->ControlStatus, USB2_DEBUG_PORT_STATUS_DONE)) {
    }

    if (IsBitSet(&This->DebugRegister->ControlStatus, USB2_DEBUG_PORT_STATUS_ERROR)) {
      --RetryCount;
    } else {

    //
        // Debug
    //
    if (This->DebugRegister->ReceivedPid == USB2_PID_HANDSHAKE_ACK) {
      return RETURN_SUCCESS;
    } else if (This->DebugRegister->ReceivedPid == USB2_PID_HANDSHAKE_NYET) {
      return RETURN_SUCCESS;
    } else {
      MicroSecondDelay(1);
    }
    }
    --TimeOut;
  }

  if (RetryCount == 0) {
  return RETURN_DEVICE_ERROR;
  }

  return RETURN_TIMEOUT;
}


/**
  Usb2DebugPortControlTransfer internal function invoked by
  USB initialization phase.

  @param[in] *UsbDbg            Usb2DebugPortInstance pointer
  @param[in] *SetupPacket       USB2_SETUP_PACKET pointer.
  @param[out] *Data             Data pointer
  @param[out] DataToggle        Data length

  @retval RETURN_SUCCESS        Control transfer successful.
  @retval RETURN_UNSUPPORTED    Unsupported type of control transfer.
  @retval !RETURN_SUCCESS       Error
**/
RETURN_STATUS
Usb2DebugPortControlTransfer (
      IN  USB2_DEBUG_PORT_INSTANCE        *UsbDbg,
      IN  USB2_SETUP_PACKET               *SetupPacket,
      OUT UINT8                           *Data,
      OUT UINTN                           *DataLength
  )
{
  RETURN_STATUS          Status;

  if ((SetupPacket->Length > 0) && (!IsBitSet((UINT32*)&SetupPacket->RequestType, USB2_REQUEST_TYPE_DEVICE_TO_HOST))) {
    return RETURN_UNSUPPORTED;
  }

  Status = Usb2DebugPortOut(UsbDbg, (UINT8 *)SetupPacket, sizeof(USB2_SETUP_PACKET), 1000000, USB2_PID_TOKEN_SETUP, 0);
  if (RETURN_ERROR(Status)) {
    return Status;
  }

  //
  // Data phase
  //
  if (SetupPacket->Length > 0) {
    if (IsBitSet((UINT32*)&SetupPacket->RequestType, USB2_REQUEST_TYPE_DEVICE_TO_HOST)) {
      Status = Usb2DebugPortIn(UsbDbg, Data, DataLength, 1000000, 1);
      if (RETURN_ERROR(Status)) {
        return Status;
      }
    }
  }

  //
  // Status handshake
  //
  if (IsBitSet((UINT32*)&SetupPacket->RequestType, USB2_REQUEST_TYPE_DEVICE_TO_HOST)) {
    Status = Usb2DebugPortOut(UsbDbg, NULL, 0, 1000000, USB2_PID_TOKEN_OUT, 1);
  } else {
    Status = Usb2DebugPortIn(UsbDbg, NULL, &Status, 1000000, 1);
  }

  return Status;
}

/**
  Method to send data over USB2 serial port.

  @param[in] *Instance    Usb2DebugPort instance pointer
  @param[in] *Data      Data pointer
  @param[in, out] *Length  Data length

  @retval RETURN_SUCCESS        The serial device was initialized.
  @retval RETURN_DEVICE_ERROR   The serial device could not be initialized.
**/
RETURN_STATUS
Usb2DebugPortSend (
  IN USB2_DEBUG_PORT_INSTANCE             *Instance,
  IN      UINT8                           *Data,
  IN  OUT UINTN                           *Length
  )
{
  RETURN_STATUS                            Status;
  UINTN                                 BytesToSend;

  if (Instance->Ready == TRUE) {
     Instance->DebugRegister->UsbAddress = USB2_DEBUG_PORT_DEFAULT_ADDRESS;
     Instance->DebugRegister->UsbEndPoint = Instance->WriteEndpoint;

     BytesToSend = 0;
     while (*Length > 0) {
       BytesToSend = ((*Length) > 8)? 8 : *Length;
       Status = Usb2DebugPortOut(Instance, Data, BytesToSend, 1000000, (UINT8)USB2_PID_TOKEN_OUT, Instance->WriteEndpointDataToggle);
       if (RETURN_ERROR(Status)) {
         return Status;
       }
       Instance->WriteEndpointDataToggle ^= 0x01;

       *Length -= BytesToSend;
       Data += BytesToSend;
     }

     //
     // Send zero-length packet to end of this send session, if the length of data
     // is integer number of wMaxPacketSize (8 bytes for debug port)
     //
     Status = Usb2DebugPortOut(Instance, Data, 0, 1000000, (UINT8)USB2_PID_TOKEN_OUT, Instance->WriteEndpointDataToggle);
     if (!RETURN_ERROR(Status)) {
       Instance->WriteEndpointDataToggle ^= 0x01;
     }
  }

  return RETURN_SUCCESS;
}


/**
  Discover the USB2 serial device.

  @param[in]    Usb2DebugPortInstance object pointer.

  @retval RETURN_SUCCESS        The serial device was initialized.
  @retval RETURN_DEVICE_ERROR   The serial device could not be initialized.

**/
RETURN_STATUS
EFIAPI
DiscoverUsb2DebugPort(
  USB2_DEBUG_PORT_INSTANCE  *Instance
  )
{
    UINT8                           Bus;
    UINT8                           Device;
    UINT8                           Function;
    UINT16                          Command;
    UINT8                           CapabilityPtr;
    UINT16                          BarOffset;
    UINT8                           BarIndex;
    UINT32                          DebugPortBase;
    UINT32                          UsbBase;
    UINT32                          UsbSpaceSize;

    UsbSpaceSize = 0;
    DebugPortBase = 0;


    //
    // Try default location to find debug port Bus0:Device29:Function7
    //
    // Find USB2 whose ClassCode = 0C 03 20
    //    Base        0C (Serial Bus Controller)
    //    Sub-Class   03 (USB)
    //    Interface:  20 (EHCI)
    //                          10 (OHCI) 00 (UHCI) 80 (non-specific) FE (USB device)
    //
    Bus = PcdGet8(PcdUsbSerialEhciBus);
    Device = PcdGet8(PcdUsbSerialEhciDev);
    Function = PcdGet8(PcdUsbSerialEhciFunc);
    CapabilityPtr = PCI_EHCI_DEFAULT_DEBUG_CAPID_OFFSET;
    if ((PciRead8(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_CLASSCODE_OFFSET+2)) != PCI_CLASS_SERIAL)
        || (PciRead8(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_CLASSCODE_OFFSET+1)) != PCI_CLASS_SERIAL_USB)
        || (PciRead8(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_CLASSCODE_OFFSET)) != PCI_CLASS_SERIAL_USB_EHCI)
        || (PciRead8(PCI_LIB_ADDRESS(Bus, Device, Function, CapabilityPtr)) != PCI_CAPABILITY_ID_DEBUG_PORT)) {

      return RETURN_NOT_FOUND;
    }

    //
    // Find the base address of debug port register in Debug Port capability register
    //
  BarOffset = PciRead16(PCI_LIB_ADDRESS(Bus, Device, Function, CapabilityPtr + 2)) & 0x1FFF;
  BarIndex = (UINT8)(PciRead16(PCI_LIB_ADDRESS(Bus, Device, Function, CapabilityPtr + 2)) >> 13)-1;
    //
    // USBBASE is at 10-13h, i.e. the firt BAR
    //
    UsbBase = PciRead32(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET));

    //
    // Set USBBASE address if necessary
    //
    if (UsbBase == 0) {

       //
       // Detect size
       //
       UsbBase = 0xFFFFFFFF;
       PciWrite32(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET), UsbBase);
       UsbSpaceSize = PciRead32(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET));
       UsbSpaceSize = (~(UsbSpaceSize & 0xFFFFFFF0)) + 1;

       //
       // Write real base address
       //
       UsbBase = PCI_EHCI_DEFAULT_USBBASE_ADDRESS;
       PciWrite32(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET), UsbBase);
       UsbBase = PciRead32(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET));

       if (UsbBase == 0) {
          return RETURN_DEVICE_ERROR;
       }
    }
    //
    // Find the base address of debug port register in Debug Port capability register
    //
    DebugPortBase = PciRead32(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET + BarIndex * 4));
    if (DebugPortBase == 0) {
       DebugPortBase = UsbBase + UsbSpaceSize;
       PciWrite32(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET + BarIndex * 4), DebugPortBase);
       DebugPortBase = PciRead32(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET + BarIndex * 4));
       if (DebugPortBase == 0) {
          return RETURN_DEVICE_ERROR;
       }
    }

    //
    //  Set MSE bit - enable the address space
    //
    Command = PciRead16(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_COMMAND_OFFSET));
    if ((Command & 0x2) == 0) {
       Command |= 0x02;
       PciWrite16(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_COMMAND_OFFSET), Command);
       Command = PciRead16(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_COMMAND_OFFSET));
    }

    {
      //
      // find EHCI register: PORTSC, HCSPARAMS
      //
      Instance->EhciCapRegister = (USB2_EHCI_CAPABILITY_REGISTER *)(UINTN)UsbBase;
      Instance->EhciOpRegister = (USB2_EHCI_OPERATIONAL_REGISTER *)(UINTN)(UsbBase + Instance->EhciCapRegister->CapLength);
      //
      // find debug port number: indicated by a 4-bit field (20-23) in the HCSPARAMS register
      //
      Instance->PortNumber = (UINT8)((Instance->EhciCapRegister->HcsParams & 0x00F00000) >> 20);
      Instance->DebugRegister = (USB2_DEBUG_PORT_REGISTER *)(UINTN)(DebugPortBase + BarOffset);
      Instance->PortSc = &Instance->EhciOpRegister->PortSc[Instance->PortNumber - 1];

      Instance->PciBusNumber = Bus;
      Instance->PciDeviceNumber = Device;
      Instance->PciDeviceFunction = Function;

      Instance->BarIndex = BarIndex;
      Instance->BarOffset = BarOffset;
      Instance->PortBase = DebugPortBase;
    }

    return RETURN_SUCCESS;
}

/**
  Initialize the Serial Device hardware.

  @param[in]  *Instance  Pointer to USB2 debug port object instance

  @retval RETURN_SUCCESS    The serial device was initialized successfully.
  @retval !RETURN_SUCCESS   Error.

**/
RETURN_STATUS
EFIAPI
InitializeUsb2DebugPort (USB2_DEBUG_PORT_INSTANCE  *Instance)
{
   USB2_DEBUG_DESCRIPTOR_TYPE      DebugDescriptor;
   UINTN                           Length;
   RETURN_STATUS                      Status;

   //
   // Reset Host controller if necessary
   //
   if (!IsBitSet(&Instance->EhciOpRegister->UsbStatus, USB2_EHCI_USBSTS_HC_HALTED)) {
     ClrR32Bit(&Instance->EhciOpRegister->UsbCommand, USB2_EHCI_USBCMD_RUN);
     while (!IsBitSet(&Instance->EhciOpRegister->UsbStatus, USB2_EHCI_USBSTS_HC_HALTED)) {
        MicroSecondDelay (10);
     }
   }
   SetR32Bit(&Instance->EhciOpRegister->UsbCommand, USB2_EHCI_USBCMD_RESET);

   //
   // Ensure that the Host controller is reset (RESET bit must be cleared after reset)
   //
   while (IsBitSet(&Instance->EhciOpRegister->UsbCommand, USB2_EHCI_USBCMD_RESET)) {
      MicroSecondDelay (10);
   }

   //
   // Start the Host controller if it's not running
   //
   if (IsBitSet(&Instance->EhciOpRegister->UsbStatus, USB2_EHCI_USBSTS_HC_HALTED)) {
     SetR32Bit(&Instance->EhciOpRegister->UsbCommand, USB2_EHCI_USBCMD_RUN);

     //
     // Ensure that the Host controller is started (HALTED bit must be cleared)
     //
     while (IsBitSet(&Instance->EhciOpRegister->UsbStatus, USB2_EHCI_USBSTS_HC_HALTED)) {
        MicroSecondDelay (10);
     }
   }

   //
   // Take ownership of the debug port
   //
   SetR32Bit(&Instance->DebugRegister->ControlStatus, USB2_DEBUG_PORT_STATUS_OWNER);

   //
   // Power-up the debug port
   //
   if (IsBitSet(&Instance->EhciCapRegister->HcsParams, 0x00000010)) {

     //
     // We can control the port power only if PPC in HCSPARAMS is set
     //
     if (IsBitSet(Instance->PortSc, USB2_EHCI_PORTSC_PORT_POWER)) {
       ClrR32Bit(Instance->PortSc, USB2_EHCI_PORTSC_PORT_POWER);
       MicroSecondDelay(20000);
     }
     SetR32Bit(Instance->PortSc, USB2_EHCI_PORTSC_PORT_POWER);
   }

   //
   // Wait for 250ms
   //
   MicroSecondDelay(250000);

   //
   // Start to reset
   //
   SetR32Bit(Instance->PortSc, USB2_EHCI_PORTSC_PORT_RESET);

   //
   // Wait for 50ms
   //
   MicroSecondDelay(50000);

   //
   // Wait for the reset bit to go low...
   //
   ClrR32Bit(Instance->PortSc, USB2_EHCI_PORTSC_PORT_RESET);

   //
   // When software writes a zero to this bit there may be a delay before the bit
   // status changes to a zero. The bit status will not read as a zero until after
   // the reset has completed.
   //
   while (!IsBitClear(Instance->PortSc, USB2_EHCI_PORTSC_PORT_RESET)) {
      MicroSecondDelay(10);
   }

   //
   // Check to see if the port was enabled...
   //
   if (!IsBitSet(Instance->PortSc, USB2_EHCI_PORTSC_PORT_ENABLED)) {
     //
     // ENABLED bit must be set to 1 after reset, if the device is attached and is high speed device.
     //
     return RETURN_DEVICE_ERROR;
   }

   //
   // Set that the port is enabled and owned in the DebugPortControl Register
   // Also set the s/w InUse flag and clear the done flag
   //
   SetR32Bit(&Instance->DebugRegister->ControlStatus, USB2_DEBUG_PORT_STATUS_ENABLED
                                                 | USB2_DEBUG_PORT_STATUS_OWNER
                                                 | USB2_DEBUG_PORT_STATUS_INUSE
                                                 | USB2_DEBUG_PORT_STATUS_DONE);

   //
   // Reset the PORT Enabled/Disabled bit so that the system Host controller
   // driver doesn't see an enabled port when it is first loaded
   //
   ClrR32Bit(Instance->PortSc, USB2_EHCI_PORTSC_PORT_ENABLED);

   //
   // Turn off the Host controller by setting the RUN/STOP bit to a zero
   //
   ClrR32Bit(&Instance->EhciOpRegister->UsbCommand, USB2_EHCI_USBCMD_RUN);

   //
   // Try to get the debug descriptor at address 127
   //
   Instance->DebugRegister->UsbAddress = USB2_DEBUG_PORT_DEFAULT_ADDRESS;
   Instance->DebugRegister->UsbEndPoint = 0;
   Status = Usb2DebugPortControlTransfer(Instance, &mGetDebugDescriptor, (UINT8 *)&DebugDescriptor, &Length);
   if (RETURN_ERROR(Status)) {
     //
     // Try address 0
     //
     Instance->DebugRegister->UsbAddress = 0;
     Instance->DebugRegister->UsbEndPoint = 0;
     Status = Usb2DebugPortControlTransfer(Instance, &mGetDebugDescriptor, (UINT8 *)&DebugDescriptor, &Length);
     if (RETURN_ERROR(Status)) {
       return Status;
     }

     //
     // Set address to 127
     //
     Status = Usb2DebugPortControlTransfer(Instance, &mSetAddress, NULL, NULL);
     if (RETURN_ERROR(Status)) {
       return Status;
     }
     MicroSecondDelay(100000);
     Instance->DebugRegister->UsbAddress = USB2_DEBUG_PORT_DEFAULT_ADDRESS;
     Instance->DebugRegister->UsbEndPoint = 0;
   }

   Instance->ReadEndpoint = DebugDescriptor.DebugInEndpoint;
   Instance->WriteEndpoint = DebugDescriptor.DebugOutEndpoint;

   //
   // SetFeature - DEBUG_MODE
   //
   Status = Usb2DebugPortControlTransfer(Instance, &mSetDebugFeature, NULL, NULL);
   if (RETURN_ERROR(Status)) {
     return Status;
   }

   //
   // Init the data toggle and other fields
   //
   Instance->WriteEndpointDataToggle = 0;
   Instance->ReadEndpointDataToggle = 0;

   if (!Instance->Ready) {

      UINT8  *Str = "USB Debug Port Message\r\n";
      UINT8  *Tmp = Str;
      UINTN   Len = 1;

      Instance->Ready = TRUE;

      //
      // Debug
      //
      while (*Tmp++)
      Len++;

      Usb2DebugPortSend (Instance, Str,  &Len);
   }

   return RETURN_SUCCESS;
}

/**
  Receive data over the USB2 serial device.

  This method calls Usb2DebugPortIn method internal to this library.

  @param[out] *Data      Pointer to data
  @param[in out] *Length  Data length

  @retval RETURN_SUCCESS        The serial device was initialized.
  @retval RETURN_DEVICE_ERROR   The serial device could not be initialized.
**/
RETURN_STATUS
UsbDbgIn (
      OUT UINT8                           *Data,
  IN  OUT UINTN                           *Length
  )
{
  RETURN_STATUS                          Status = RETURN_SUCCESS;
  UINTN                                  BytesReceived;
  UINTN                                  AllBytesReceived;
  UINTN                                  Index;
  USB2_DEBUG_PORT_INSTANCE              *Instance;

  Instance = (USB2_DEBUG_PORT_INSTANCE *)(UINTN)(*(UINT32 volatile *)(UINTN)(PCI_EHCI_DEFAULT_USBBASE_ADDRESS + USB_DBG_DATA_OFFSET));

  if (Instance->Ready == TRUE) {
     Instance->DebugRegister->UsbAddress = USB2_DEBUG_PORT_DEFAULT_ADDRESS;
     Instance->DebugRegister->UsbEndPoint = Instance->ReadEndpoint;

     if (*Length <= 0 || Data == NULL) {
       *Length = 0;  // if incorrect buffer or length specified, read data into internal buffer
     }
     AllBytesReceived = 0;
     BytesReceived = 0;
     do {
        if (Instance->TempDataLength == 0 || *Length == 0) {
           if (Instance->TempDataLength == 0) {
              Instance->TempDataIndex = 0;
           }
           if  (*Length >= USB2_DEBUG_PORT_DEVICE_BUFFER_MAX) {
        //
              // If there's enough space in buffer, try storing directly the data in user's buffer
        //
              Status = Usb2DebugPortIn(Instance, Data, &BytesReceived, 1, Instance->ReadEndpointDataToggle);
              if (RETURN_ERROR(Status)) {
                 break;
              }
           } else if ((USB2_DEBUG_PORT_DRIVER_BUFFER_MAX - Instance->TempDataIndex - Instance->TempDataLength) >= USB2_DEBUG_PORT_DEVICE_BUFFER_MAX) {
        //
              // Otherwise, we have to store received data in a temporary buffer first to avoid overflow
        //
              Status = Usb2DebugPortIn(Instance, Instance->TempData + Instance->TempDataIndex, &BytesReceived, 1, Instance->ReadEndpointDataToggle);
              if (RETURN_ERROR(Status)) {
                 break;
              }
              Instance->TempDataLength += (UINT32)BytesReceived;
           } else {
              Status = RETURN_OUT_OF_RESOURCES;
              break;
           }
           Instance->ReadEndpointDataToggle ^= 0x01;
           if (BytesReceived == 0) {
        //
              // Got a ZLP (Zero-Length Packet), stop receiving and send received data up
        //
              break;
           }
        }

    //
        // Move the data from internal temporary buffer to user's buffer, if any
       //
        if (Instance->TempDataLength > 0) {
           BytesReceived = (*Length > (UINTN)Instance->TempDataLength)? Instance->TempDataLength : *Length;
           for (Index = 0; Index < BytesReceived; ++Index) {
              Data[Index] = Instance->TempData[Instance->TempDataIndex++];
              if (Instance->TempDataIndex >= USB2_DEBUG_PORT_DRIVER_BUFFER_MAX) {
                 Instance->TempDataIndex = 0;
              }
           }
           Instance->TempDataLength -= (UINT32)BytesReceived;
        }

        *Length -= BytesReceived;
        Data += BytesReceived;
        AllBytesReceived += BytesReceived;
     } while (*Length > 0);

     *Length = AllBytesReceived;
     if (*Length > 0) {
        return RETURN_SUCCESS;
     }
  }

  *(UINT32 volatile *)(UINTN)(PCI_EHCI_DEFAULT_USBBASE_ADDRESS + USB_DBG_DATA_OFFSET) = (UINT32)(UINTN)Instance;

  return Status;
}


/**
  Send data over USB2 serial device.

  This method calls Usb2DebugPortSend method internal to this library.

  @param[out] *Data    Pointer to data
  @param[in]  *Length  Data length

  @retval RETURN_SUCCESS        The serial device was initialized.
  @retval RETURN_DEVICE_ERROR   The serial device could not be initialized.
**/
VOID
UsbDbgOut (
  OUT  UINT8                           *Data,
  IN   UINTN                           *Length
  )
{
  USB2_DEBUG_PORT_INSTANCE        *Instance;

  Instance = (USB2_DEBUG_PORT_INSTANCE *)(UINTN)(*(UINT32 volatile *)(UINTN)(PCI_EHCI_DEFAULT_USBBASE_ADDRESS + USB_DBG_DATA_OFFSET));
  Usb2DebugPortSend (Instance, Data,  Length);
  *(UINT32 volatile *)(UINTN)(PCI_EHCI_DEFAULT_USBBASE_ADDRESS + USB_DBG_DATA_OFFSET) = (UINT32)(UINTN)Instance;

}

/**
  Initialize USB2 serial port.

  This method invokes various internal functions to facilitate
  detection and initialization of USB2 serial port.

  @retval RETURN_SUCCESS        The serial device was initialized.
**/
RETURN_STATUS
EFIAPI
SerialPortInitialize (
  VOID
  )
{
#if defined (SERIAL_IO_LIB_USB_IA32)
  USB2_DEBUG_PORT_INSTANCE  UsbDbg;
  GUID                      UsbDbgGuid =  { 0xf880aae0, 0xe4ac, 0x4c64, 0xa3, 0x26, 0x82, 0x70, 0x9c, 0xc2, 0x41, 0xea };
  VOID            *DataPtr;
    EFI_PEI_HOB_POINTERS      Hob;
    RETURN_STATUS             Status;

  //
  // Initialize USB debug
  //
  SetMem (&UsbDbg, sizeof(UsbDbg), 0);
  Status = DiscoverUsb2DebugPort(&UsbDbg);
    if (RETURN_ERROR(Status)) {
      return Status;
    }

    InitializeUsb2DebugPort (&UsbDbg);

  //
  // Save Instance into HOB
  //
  BuildGuidDataHob (
    &UsbDbgGuid,
    (VOID*) &UsbDbg,
    sizeof (UsbDbg)
    );

  Hob.Raw = GetFirstGuidHob (&UsbDbgGuid);
    if (Hob.Raw != NULL) {
        DataPtr = (VOID *)GET_GUID_HOB_DATA(Hob.Guid);
        *(UINT32 volatile *)(PCI_EHCI_DEFAULT_USBBASE_ADDRESS + USB_DBG_DATA_OFFSET) = (UINT32)(UINTN)DataPtr;
    }
    else return RETURN_UNSUPPORTED;

#endif
  return RETURN_SUCCESS;
}

/**
  Write data from buffer to serial device.

  Writes NumberOfBytes data bytes from Buffer to the serial device.
  The number of bytes actually written to the serial device is returned.
  If the return value is less than NumberOfBytes, then the write operation failed.
  If Buffer is NULL, then ASSERT().
  If NumberOfBytes is zero, then return 0.

  @param  Buffer           Pointer to the data buffer to be written.
  @param  NumberOfBytes    Number of bytes to written to the serial device.

  @retval 0                NumberOfBytes is 0.
  @retval >0               The number of bytes written to the serial device.
                           If this value is less than NumberOfBytes, then the read operation failed.

**/
UINTN
EFIAPI
SerialPortWrite (
  IN UINT8     *Buffer,
  IN UINTN     NumberOfBytes
)
{
  UsbDbgOut (Buffer, (UINTN*) &NumberOfBytes);
  return 0;
}


/**
  Read data from serial device and save the datas in buffer.

  Reads NumberOfBytes data bytes from a serial device into the buffer
  specified by Buffer. The number of bytes actually read is returned.
  If the return value is less than NumberOfBytes, then the rest operation failed.
  If Buffer is NULL, then ASSERT().
  If NumberOfBytes is zero, then return 0.

  @param  Buffer           Pointer to the data buffer to store the data read from the serial device.
  @param  NumberOfBytes    Number of bytes which will be read.

  @retval 0                Read data failed, No data is to be read.
  @retval >0               Aactual number of bytes read from serial device.

**/
UINTN
EFIAPI
SerialPortRead (
  OUT UINT8     *Buffer,
  IN  UINTN     NumberOfBytes
)
{
  UsbDbgIn (Buffer, (UINTN*) &NumberOfBytes);
  return 0;
}

/**
  Polls a serial device to see if there is any data waiting to be read.

  Polls a serial device to see if there is any data waiting to be read.
  If there is data waiting to be read from the serial device, then TRUE is returned.
  If there is no data waiting to be read from the serial device, then FALSE is returned.

  @retval TRUE             Data is waiting to be read from the serial device.
  @retval FALSE            There is no data waiting to be read from the serial device.

**/
BOOLEAN
EFIAPI
SerialPortPoll (
  VOID
  )
{
  return FALSE;
}

