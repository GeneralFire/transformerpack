//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file UsbInterfaceDxeLib.c
    USB Transport Dxe phase Implementation.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <StatusCodes.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/AmiUsbInterfaceLib.h>
#include <Protocol/UsbIo.h>
#include <Protocol/IpmiTransportProtocol.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define AMI_IPMI_IS_BULK_IN_ENDPOINT(EndPoint)      ((EndPoint & BIT7) != 0x00)
#define AMI_IPMI_IS_BULK_OUT_ENDPOINT(EndPoint)     ((EndPoint & BIT7) == 0x00)

// Status code definition to report BMC Selftest fail.
#ifndef AMI_DXE_BS_EC_USB_BMC_SELFTEST_FAILED
#define AMI_DXE_BS_EC_USB_BMC_SELFTEST_FAILED   (AMI_STATUS_CODE_CLASS | 0x00000007)
#endif

#ifndef AMI_IPMI_DXE_USB_BMC_SELFTEST_FAILED
#define AMI_IPMI_DXE_USB_BMC_SELFTEST_FAILED    (EFI_SOFTWARE_DXE_BS_DRIVER | \
                                                AMI_DXE_BS_EC_USB_BMC_SELFTEST_FAILED)
#endif

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

EFI_USB_IO_PROTOCOL     *gUsbIo = NULL;

/** @internal
    
    Checks for Vendor ID and Product ID of BMC USB Interface for given handle

    @param[in] EFI_HANDLE
    
    @return EFI_STATUS          Status returned from get device descriptor
    @retval EFI_UNSUPPORTED     Failed to get device description

**/
EFI_STATUS
IsBmcUsbUsingHandle(
  IN  EFI_HANDLE *ControllerHandle  
  ) 
{
    
    EFI_STATUS                  Status;
    EFI_USB_DEVICE_DESCRIPTOR   DevDesc;
    
    Status = gBS->HandleProtocol (
                            ControllerHandle,
                            &gEfiUsbIoProtocolGuid,
                            (VOID **)&gUsbIo);
        
    if (EFI_ERROR(Status)) {
        return EFI_UNSUPPORTED;
    }
    
   /* GetDeviceDescriptor API is not available in UsbIoPpi.
       So Get Device descriptor details using UsbIo->ControlTransfer API.
       Fill the Device request structure.*/
    // Issue ControlTransfer transaction
    Status = gUsbIo->UsbGetDeviceDescriptor ( 
                                        gUsbIo,
                                        &DevDesc);

    if (EFI_ERROR(Status)) {
        return EFI_UNSUPPORTED;
    }

    /* Check for Vendor ID and Product ID. */
    if ((DevDesc.IdVendor != BMC_USB_INTERFACE_VENDOR_ID) && 
        (DevDesc.IdProduct != BMC_USB_INTERFACE_PRODUCT_ID)) {
        return EFI_UNSUPPORTED;
    }
    return Status;
}

/** @internal
    If following conditions are met, returns TRUE else FALSE.
    1. Locates each USB IO protocol instance.
    2. Checks for Vendor ID and Product ID of BMC USB Interface.

    @param  VOID    Nothing.

    @retval TRUE    BMC USB Interface is found.
    @retval FALSE   BMC USB Interface is not found.

**/
BOOLEAN
IsBmcUsbInterface (
  IN OUT EFI_HANDLE *Handle 
  )
{
    EFI_STATUS                  Status;
    EFI_USB_DEVICE_DESCRIPTOR   DevDesc;
    UINTN                       NumOfHandles;
    EFI_HANDLE                  *HandleBuffer;
    UINT8                       Index;

    DEBUG((DEBUG_INFO, "%a Entry...\n", __FUNCTION__));

    // Get list of available USB I/O devices.
    Status = gBS->LocateHandleBuffer (
                                ByProtocol,
                                &gEfiUsbIoProtocolGuid,
                                NULL,
                                &NumOfHandles,
                                &HandleBuffer );
    DEBUG((DEBUG_INFO, "LocateHandleBuffer.... Status: %r \n", Status));
    if (EFI_ERROR(Status)) {
        return FALSE;
    }

    for (Index = 0; Index < NumOfHandles; ++Index) {
        Status = gBS->HandleProtocol (
                                  HandleBuffer[Index],
                                  &gEfiUsbIoProtocolGuid,
                                  (VOID **)&gUsbIo );
        
        DEBUG((DEBUG_INFO, "HandleProtocol gEfiUsbIoProtocolGuid .... Status: %r \n", Status));
        if (EFI_ERROR(Status)) {
            return FALSE;
        }
        /* GetDeviceDescriptor API is not available in UsbIoPpi.
           So Get Device descriptor details using gUsbIo->ControlTransfer API.
           Fill the Device request structure.*/

        // Issue ControlTransfer transaction.
        Status = gUsbIo->UsbGetDeviceDescriptor (
                            gUsbIo,
                            &DevDesc );
        DEBUG((DEBUG_INFO, "UsbGetDeviceDescriptor .... Status: %r \n", Status));

        // Return FALSE for error case.
        if (EFI_ERROR(Status)) {
            return FALSE;
        }

        DEBUG((DEBUG_INFO, "DevDesc.IdVendor: %X \n", DevDesc.IdVendor));
        DEBUG((DEBUG_INFO, "DevDesc.IdProduct: %X \n", DevDesc.IdProduct));

        /* Check for Vendor ID and Product ID. Return TRUE if Vendor ID and
           Product ID matches with BMC USB Interface. Else, return FALSE.*/
        if ((DevDesc.IdVendor == BMC_USB_INTERFACE_VENDOR_ID) && \
            (DevDesc.IdProduct == BMC_USB_INTERFACE_PRODUCT_ID)) {
            DEBUG((DEBUG_INFO, "BMC USB interface is found.\n"));
            *Handle = HandleBuffer[Index];
            return TRUE;
        }
    }
    *Handle = NULL;
    return FALSE;
}

/** @internal
    Gets bulk-in and bulk-out end points for USB.

    @param[in, out] Interface   Pointer to System interface.

    @retval EFI_SUCCESS     Endpoint addresses are found.
    @retval EFI_NOT_FOUND   Endpoint addresses are not found.
    @retval Others          Error status occurred while getting endpoints.

**/
EFI_STATUS
GetBulkEndPoints (
  IN OUT AMI_SYSTEM_INTERFACE   *Interface )
{
    EFI_STATUS                      Status;
    UINT8                           EndPoint = 0;
    EFI_USB_INTERFACE_DESCRIPTOR    InterfaceDescriptor;
    EFI_USB_ENDPOINT_DESCRIPTOR     EndpointDescriptor;
    BOOLEAN                         BulkInEndpointFound = FALSE;
    BOOLEAN                         BulkOutEndpointFound = FALSE;

    DEBUG((DEBUG_INFO, "%a Entry... \n", __FUNCTION__));

    Status = gUsbIo->UsbGetInterfaceDescriptor (
                                        gUsbIo,
                                        &InterfaceDescriptor );
    DEBUG((DEBUG_INFO, "UsbGetInterfaceDescriptor: Status: %r \n", Status));
    if (EFI_ERROR(Status)) {
        return Status;
    }
    DEBUG((DEBUG_INFO, "InterfaceDescriptor.NumEndpoints: %X \n", InterfaceDescriptor.NumEndpoints));

    // Get Bulk-in/Bulk-out end point address.
    do {
        Status = gUsbIo->UsbGetEndpointDescriptor (
                                            gUsbIo,
                                            EndPoint,
                                            &EndpointDescriptor );
        DEBUG((DEBUG_INFO, "UsbGetInterfaceDescriptor: Status: %r EndPoint: %x \n", Status, EndPoint));
        if (EFI_ERROR(Status)) {
            return Status;
        }
        DEBUG((DEBUG_INFO, "EndpointDescriptor.Attributes: %x \n", EndpointDescriptor.Attributes));
        DEBUG((DEBUG_INFO, "EndpointDescriptor.EndpointAddress: %x \n", EndpointDescriptor.EndpointAddress));
        // Check for Bulk EndPoint Attributes.
        if (EndpointDescriptor.Attributes != USB_ENDPOINT_BULK) {
            ++EndPoint;
            continue;
        }

        if ((BulkInEndpointFound != TRUE) && \
            AMI_IPMI_IS_BULK_IN_ENDPOINT(EndpointDescriptor.EndpointAddress)) {
            Interface->Usb->BulkInEndpointAddr = EndpointDescriptor.EndpointAddress;
            BulkInEndpointFound = TRUE;
            DEBUG((DEBUG_INFO, "BulkInEndpointAddr: %x \n", Interface->Usb->BulkInEndpointAddr));
        } else if ((BulkOutEndpointFound != TRUE) && \
                AMI_IPMI_IS_BULK_OUT_ENDPOINT(EndpointDescriptor.EndpointAddress)) {
            Interface->Usb->BulkOutEndpointAddr = EndpointDescriptor.EndpointAddress;
            BulkOutEndpointFound = TRUE;
            DEBUG((DEBUG_INFO, "BulkOutEndpointAddr: %x \n", Interface->Usb->BulkOutEndpointAddr));
        }

        DEBUG((DEBUG_INFO, "BulkInEndpointFound: %x \n", BulkInEndpointFound));
        DEBUG((DEBUG_INFO, "BulkOutEndpointFound: %x \n", BulkOutEndpointFound));

        if ((BulkInEndpointFound == TRUE) && (BulkOutEndpointFound == TRUE)) {
            return EFI_SUCCESS;
        }
    } while (++EndPoint < InterfaceDescriptor.NumEndpoints);

    return EFI_NOT_FOUND;
}


/** @internal
    Sends Clear Feature command for specified Bulk-In/Bulk-out endpoint.

    @param[in] Interface        Pointer to System interface.
    @param[in] EndpointAddr     Endpoint Address.

    @return EFI_STATUS      Status returned from usb transaction.

**/
EFI_STATUS
BotClearEndpoint (
  IN AMI_SYSTEM_INTERFACE   *Interface,
  IN UINT16                 EndpointAddr )
{
    EFI_USB_DEVICE_REQUEST  DevReq;
    EFI_STATUS              Status;
    UINT32                  UsbStatus;

    ZeroMem (
        &DevReq,
        sizeof(DevReq) );

    // Fill the Device Request structure to Clear specific device feature.
    DevReq.RequestType  = USB_TARGET_ENDPOINT;
    DevReq.Request      = USB_DEV_CLEAR_FEATURE;
    DevReq.Value        = USB_FEATURE_ENDPOINT_HALT;
    DevReq.Length       = 0x00;

    // Clear Feature Halt for provided Bulk-In/Bulk-out Endpoint.
    DevReq.Index = (UINT8)EndpointAddr;

    // Issue ControlTranfer transaction.
    Status = gUsbIo->UsbControlTransfer (
                        gUsbIo,
                        &DevReq,
                        EfiUsbNoData,
                        AMI_IPMI_THREE_THOUSAND_MS,
                        NULL,
                        0,
                        &UsbStatus );

    if (EFI_ERROR(Status)) {
       DEBUG((DEBUG_ERROR, "%a Error : UsbStatus = %x\n", __FUNCTION__, UsbStatus));
    }
    return Status;
}

/** @internal
    Performs BOT Recovery Reset in case of certain Error conditions.

    @param[in] Interface    Pointer to System interface.

    @return EFI_STATUS      Status returned from usb transaction.

**/
EFI_STATUS
BotRecoveryReset (
  IN AMI_SYSTEM_INTERFACE   *Interface )
{
    EFI_STATUS              Status;
    EFI_USB_DEVICE_REQUEST  DevReq;
    UINT32                  UsbStatus;

    ZeroMem (
        &DevReq,
        sizeof(DevReq) );

    // Fill the Device Request structure to issue the Bulk-Only Mass Storage Reset.
    DevReq.RequestType  = USB_REQ_TYPE_CLASS | USB_TARGET_INTERFACE;
    DevReq.Request      = 0xFF;
    DevReq.Value        = 0x00;
    DevReq.Index        = 0x00;
    DevReq.Length       = 0x00;

    // Issue ControlTranfer transaction.
    Status = gUsbIo->UsbControlTransfer (
                        gUsbIo,
                        &DevReq,
                        EfiUsbNoData,
                        AMI_IPMI_THREE_THOUSAND_MS,
                        NULL,
                        0,
                        &UsbStatus );

    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_ERROR, "%a Error : UsbStatus = %x\n", __FUNCTION__, UsbStatus));
        return Status;
    }

    // Clear Feature Halt for Bulk-In and Bulk-Out Endpoints.
    BotClearEndpoint (
                Interface,
                Interface->Usb->BulkInEndpointAddr );

    Status = BotClearEndpoint (
                Interface,
                Interface->Usb->BulkOutEndpointAddr );
    return Status;
}

/** @internal
    Gets the BOT status sequence.

    @param[in] Interface        Pointer to System interface.

    @retval EFI_DEVICE_ERROR    Error while sending data or CSW error.
    @retval EFI_SUCCESS         BOT status retrieved successfully.

**/
EFI_STATUS
BotStatusPhase (
  IN AMI_SYSTEM_INTERFACE   *Interface )
{
    EFI_STATUS      Status;
    AMI_IPMI_CSW    Csw;
    UINTN           DataSize = sizeof(Csw);
    UINT32          UsbStatus = 0;

    ZeroMem (
        &Csw,
        DataSize );

    // Get the status field from bulk transfer.
    Status = gUsbIo->UsbBulkTransfer (
                        gUsbIo,
                        Interface->Usb->BulkInEndpointAddr,
                        &Csw,
                        &DataSize,
                        Interface->Usb->BulkTransferTimeOut,
                        &UsbStatus );

    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_ERROR, "Dxe BotStatusPhase Error : Status = %r UsbStatus = %x\n",Status,UsbStatus));
        if (UsbStatus == EFI_USB_ERR_STALL) {

          // Clear stall in Bulk-In Endpoint.
          BotClearEndpoint (
              Interface,
              Interface->Usb->BulkInEndpointAddr );

          DataSize = sizeof(Csw);
          // Send bulk transfer command again.
          ZeroMem (
              &Csw,
              DataSize );
          
          UsbStatus = 0;
          Status = gUsbIo->UsbBulkTransfer (
                            gUsbIo,
                            Interface->Usb->BulkInEndpointAddr,
                            &Csw,
                            &DataSize,
                            Interface->Usb->BulkTransferTimeOut,
                            &UsbStatus );
        }
        if (EFI_ERROR(Status)) {
            DEBUG((DEBUG_ERROR, "BotStatusPhase Error : Performing RecoveryReset\n"));
            BotRecoveryReset (Interface);
            return EFI_DEVICE_ERROR;
        }
    }

    // Check for valid CSW.
    if ((DataSize != sizeof (Csw)) || \
        (Csw.CswSignature != AMI_IPMI_BOT_CSW_SIGNATURE) || \
        (Csw.CswTag != Interface->Usb->BOTCommandTag)) {
        BotRecoveryReset (Interface);
        return EFI_DEVICE_ERROR;
    }
    // Check for meaningful CSW.
    if (Csw.CswStatus) {
        if (Csw.CswStatus == AMI_IPMI_CSW_PHASE_ERROR) {
            BotRecoveryReset (Interface);
        }
        return EFI_DEVICE_ERROR;
    }

    return EFI_SUCCESS;
}

/** @internal
    Sends the BOT command sequence.

    @param[in] Interface        Pointer to System interface.
    @param[in] Opcode           Opcode of the command.
    @param[in] Sector           DATA /Command sector.
    @param[in] DataTransferSize Size of data to be transferred from/to host
                                in the data phase.

    @retval EFI_DEVICE_ERROR    BOT command status failed.
    @retval EFI_SUCCESS         BOT command status success.

**/
EFI_STATUS
BotCommandPhase (
  IN AMI_SYSTEM_INTERFACE   *Interface,
  IN UINT8                  Opcode,
  IN UINT8                  Sector,
  IN UINT32                 DataTransferSize )
{
    EFI_STATUS              Status;
    AMI_IPMI_COMN_RWV_CMD   CmdBuffer;
    AMI_IPMI_BOT_CMD_BLK    Cbw;
    UINTN                   DataSize = sizeof(Cbw);
    UINT32                  UsbStatus;

    ZeroMem (
        &Cbw,
        DataSize );
    ZeroMem (
        &CmdBuffer,
        sizeof(CmdBuffer) );

    // Fill the command block.
    Cbw.CbwSignature    = AMI_IPMI_BOT_CBW_SIGNATURE;
    Cbw.CbwTag          = ++Interface->Usb->BOTCommandTag;
    Cbw.CbwDataLength   = DataTransferSize;
    Cbw.CbwLun          = AMI_BMC_LUN;
    Cbw.CbwLength       = sizeof(CmdBuffer);

    // Fill OEM defined command for read/write and fill the command buffer.
    if (Opcode == AMI_IPMI_COMMON_WRITE_10_OPCODE) {
        Cbw.CbwFlags = EfiUsbDataIn << AMI_IPMI_CBW_FLAG_SHIFT;
        CmdBuffer.Opcode = AMI_IPMI_SCSI_AMICMD_CURI_WRITE;
    } else {
        Cbw.CbwFlags = EfiUsbDataOut << AMI_IPMI_CBW_FLAG_SHIFT;
        CmdBuffer.Opcode = AMI_IPMI_SCSI_AMICMD_CURI_READ;  
    }

    CmdBuffer.Lba = SwapBytes32 (Sector);
    CmdBuffer.TransferLength = AMI_IPMI_TRANSFER_LENGTH;
    CmdBuffer.Lun = AMI_BMC_LUN;

    // Copy the buffer to command block's command buffer.
    CopyMem (
        Cbw.CbwCb,
        &CmdBuffer,
        sizeof(CmdBuffer) );

    // Send the command to host using bulk transfer.
    Status = gUsbIo->UsbBulkTransfer (
                        gUsbIo,
                        Interface->Usb->BulkOutEndpointAddr,
                        &Cbw,
                        &DataSize,
                        Interface->Usb->BulkTransferTimeOut,
                        &UsbStatus );
    if (EFI_ERROR(Status)) {
        // Command phase fail, we need to recovery reset this device.
        BotRecoveryReset (Interface);
        return EFI_DEVICE_ERROR;
    }

    return Status;
}

/** @internal
    Read Response data from BMC.

    @param[in]  Interface           Pointer to System interface.
    @param[in]  EndpointAddr        Bulk End-point Address.
    @param[in]  ResponseDataSize    Size of data to be read from BMC.
    @param[out] ResponseData        Pointer to Response data.
    @param[out] CompletionCode      Pointer to the completion code.

    @retval EFI_SUCCESS     Response data is read successfully.
    @retval Others          Error status of the USB transaction.

**/
EFI_STATUS
ReadResponseData (
  IN  AMI_SYSTEM_INTERFACE  *Interface,
  IN  UINT8                 EndpointAddr,
  IN  UINT32                ResponseDataSize,
  IN  UINT8                 *ResponseData,
  OUT UINT8                 *CompletionCode )
{
    EFI_STATUS  Status;
    UINT32      UsbStatus;
    UINT8       *TempData = (UINT8 *)(UINTN *)Interface->Usb->UsbCommBufferPtr;
    UINT32      DataBytesRead = 0;
    UINT32      MaxTempDataSize;

    // Read all the data from BMC in a loop.
    while ((DataBytesRead < (ResponseDataSize - 1)) || (ResponseDataSize == 1)) {
        // Issue Bulk Transfer to read the data from BMC.
        MaxTempDataSize = AMI_IPMI_MAX_USB_CMD_DATA_SIZE;
        Status = gUsbIo->UsbBulkTransfer (
                            gUsbIo,
                            EndpointAddr,
                            TempData,
                            (UINTN*)&MaxTempDataSize,
                            Interface->Usb->BulkTransferTimeOut,
                            &UsbStatus );
        // For error case, return the Status.
        if (EFI_ERROR(Status)) {
            return Status;
        }

        // Control comes here only when UsbBulkTransfer is success.
        if (DataBytesRead == 0) {
            // When DataBytesRead is zero, check for error completion code.
            if (CompletionCode != NULL) {
                *CompletionCode = TempData[0];
            }

            if (!AMI_IPMI_ERROR_COMPLETION_CODE(TempData[0])) {
                /* If there is no response data other than successful completion
                   code, then update the completion code and return EFI_SUCCESS.*/
                if (ResponseDataSize == 1) {
                    ResponseData[0] = TempData[0];
                    return EFI_SUCCESS;
                }
            } else {
                /* For error complete code, update completion code and return EFI_SUCCESS.
                   Error completion code is handled in EfiSendIpmiCommand() function.*/
                ResponseData[0] = TempData[0];
                return EFI_SUCCESS;
            }
        }
        // Update the complete data in ResponseData pointer and update DataBytesRead value.
        CopyMem (
            &ResponseData[DataBytesRead],
            &TempData[0],
            MaxTempDataSize );
        DataBytesRead += MaxTempDataSize;
    }

    return EFI_SUCCESS;
}

/** @internal
    Write command data to BMC.

    @param[in] Interface                Pointer to System interface.
    @param[in] EndpointAddr             Bulk End-point Address.
    @param[in] CommandRequestDataSize   Size of Command, NetFnLun and command data.
    @param[in] RequestData              Pointer to Request data.

    @retval EFI_SUCCESS     Command data written to BMC successfully.
    @retval Others          Error status returned from usb transaction.

**/
EFI_STATUS
WriteCommandData (
  IN AMI_SYSTEM_INTERFACE   *Interface,
  IN UINT8                  EndpointAddr,
  IN UINT32                 CommandRequestDataSize,
  IN UINT8                  *RequestData )
{
    EFI_STATUS  Status;
    UINT32      UsbStatus;
    UINT32      RemainingDataBytes = CommandRequestDataSize;
    UINT32      IpmiRequestSize;
    UINT32      IpmiRequestOffset;
    UINT32      RequestDataOffset = 0;

    // Ipmi Command and NetFnLun is already filled in TempData by EfiSendIpmiCommand().
    UINT8       *IpmiRequest = (UINT8 *)(UINTN *)Interface->Usb->UsbCommBufferPtr;

    // Write all the data into BMC in a loop.
    while (RemainingDataBytes != 0) {
        // Remaining data bytes will be Request data + command header size.
        if (RemainingDataBytes <= AMI_IPMI_MAX_USB_CMD_DATA_SIZE) {
            IpmiRequestSize = RemainingDataBytes;
        } else {
            IpmiRequestSize = AMI_IPMI_MAX_USB_CMD_DATA_SIZE;
        }

        // Fill command data in IpmiRequest.
        if (RemainingDataBytes == CommandRequestDataSize) {
            IpmiRequestOffset = AMI_IPMI_USB_COMMAND_DATA_HEADER_SIZE;
        } else {
            IpmiRequestOffset = 0;
        }

        DEBUG((DEBUG_INFO, "IpmiRequestSize: %x \n", IpmiRequestSize));
        DEBUG((DEBUG_INFO, "IpmiRequestOffset: %x \n", IpmiRequestOffset));
        DEBUG((DEBUG_INFO, "RequestDataOffset: %x \n", RequestDataOffset));

        // Copy the data into IpmiRequest and update RemainingDataBytes.
        CopyMem (
            &IpmiRequest[IpmiRequestOffset],
            &RequestData[RequestDataOffset],
            (IpmiRequestSize -IpmiRequestOffset) );
        RemainingDataBytes -= IpmiRequestSize;

        /* RequestDataOffset is calculated based on IpmiRequestSize as a
           check to avoid sending command data size more than specified BMC
           data size in MAX_USB_CMD_DATA_SIZE.*/
        RequestDataOffset += (IpmiRequestSize - IpmiRequestOffset);

        DEBUG((DEBUG_INFO, "RemainingDataBytes: %x \n", RemainingDataBytes));

        // Issue Bulk Transfer to write the data into BMC.
        Status = gUsbIo->UsbBulkTransfer (
                            gUsbIo,
                            EndpointAddr,
                            (UINT8*)&IpmiRequest[0],
                            (UINTN*)&IpmiRequestSize,
                            Interface->Usb->BulkTransferTimeOut,
                            &UsbStatus );
        // For error case, return the Status.
        if (EFI_ERROR(Status)) {
            return Status;
        }
    }

    return EFI_SUCCESS;
}

/** @internal
    Issues USB transaction to BMC USB interface.

    @param[in]      Interface       Pointer to System interface.
    @param[in]      Opcode          Opcode of the command.
    @param[in]      Sector          Data/Command sector.
    @param[in]      EndpointAddr    USB End point address.
    @param[in, out] Data            Pointer to input data.
    @param[in, out] DataSize        Size of input data.
    @param[out]     CompletionCode  Pointer to the completion code.

    @return EFI_STATUS  Return status of the usb transaction.

**/
EFI_STATUS
AmiIpmiIssueBmcUsbTransaction (
  IN     AMI_SYSTEM_INTERFACE   *Interface,
  IN     UINT8                  Opcode,
  IN     UINT8                  Sector,
  IN     UINT8                  EndpointAddr,
  IN OUT UINT8                  *Data,
  IN OUT UINT32                 DataSize,
  OUT    UINT8                  *CompletionCode )
{
    EFI_STATUS  Status;
    UINT32      UsbStatus;

    DEBUG((DEBUG_INFO, "DataSize: %x \n", DataSize));

    // Command Phase.
    Status = BotCommandPhase (
                Interface,
                Opcode,
                Sector,
                DataSize );
    DEBUG((DEBUG_INFO, "BotCommandPhase: Status: %r \n", Status));
    if (EFI_ERROR(Status)) {
        return Status;
    }

    // Data Phase.
    if ((Opcode == AMI_IPMI_COMMON_READ_10_OPCODE ) && \
        (Sector == AMI_IPMI_DATA_SECTOR)) {
        // Read the Response data.
        Status = ReadResponseData (
                    Interface,
                    EndpointAddr,
                    DataSize,
                    Data,
                    CompletionCode );
        DEBUG((DEBUG_INFO, "ReadResponseData: Status: %r \n", Status));
        if (EFI_ERROR(Status)) {
            return Status;
        }
    } else if ((Opcode == AMI_IPMI_COMMON_WRITE_10_OPCODE) && \
               (Sector == AMI_IPMI_DATA_SECTOR)) {
        // Write the Command data.
        Status = WriteCommandData (
                    Interface,
                    EndpointAddr,
                    DataSize,
                    Data );
        DEBUG((DEBUG_INFO, "WriteCommandData: Status: %r \n", Status));
        if (EFI_ERROR(Status)) {
            return Status;
        }
    } else {
        Status = gUsbIo->UsbBulkTransfer (
                            gUsbIo,
                            EndpointAddr,
                            Data,
                            (UINTN*)&DataSize,
                            Interface->Usb->BulkTransferTimeOut,
                            &UsbStatus );
        DEBUG((DEBUG_INFO, "UsbIo->UsbBulkTransfer: Status: %r \n", Status));
        if (EFI_ERROR(Status)) {
            return Status;
        }
    }

    // Status Phase.
    Status = BotStatusPhase (Interface);
    DEBUG((DEBUG_INFO, "BotStatusPhase: Status: %r \n", Status));
    return Status;
}

/** @internal
    Allocate memory for Usb interface, checks for Bmc usb interface
    and initializes the variables.

    @param[in, out] Interface   Pointer to System interface.

    @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.
    @retval EFI_NOT_FOUND           Bmc usb interface is not found.
    @retval EFI_SUCCESS             Interface is found and initialized
                                    successfully.

**/
EFI_STATUS
AmiIpmiGetUsbApiPtr (
  IN OUT AMI_SYSTEM_INTERFACE   *Interface )
{
    EFI_STATUS          Status = EFI_SUCCESS;
    VOID                *DxeIpmiTransportRegistration = NULL;

    DEBUG((DEBUG_INFO, "%a Entry...\n", __FUNCTION__));

    if (Interface->Usb == NULL) {
        Interface->Usb = (AMI_USB_SYSTEM_INTERFACE*)AllocateRuntimeZeroPool (sizeof (AMI_USB_SYSTEM_INTERFACE));
        if (Interface->Usb == NULL) {
            DEBUG ((DEBUG_ERROR, "%a - Failed to allocate memory for Interface->Usb! \n", __FUNCTION__));
            return EFI_OUT_OF_RESOURCES;
        }
    }
    Interface->Usb->UsbInterfaceApiGuid = gEfiUsbIoProtocolGuid;
    // To Enable and Disable BMC USB Interface at exit boot via IPMI command"
    if (AMI_VIRTUAL_BMC_USB_INTERFACE) {
        if (DEFAULT_SYSTEM_INTERFACE != AmiSysInterfaceUsb) {
            EfiCreateProtocolNotifyEvent (
                                &gEfiDxeIpmiTransportProtocolGuid,
                                TPL_CALLBACK,
                                BmcUsbInterfaceHook,
                                NULL,
                                &DxeIpmiTransportRegistration );
        }
    }

    // Check for BMC USB Interface.
    if (IsBmcUsbInterface(&Interface->Usb->UsbIoHandle)) {
        Interface->Usb->BOTCommandTag = 0; //Initial value is zero.
        Interface->Usb->UsbInterfaceApiPtr = (UINTN) gUsbIo;
        // Get Bulk In and Out end point addresses and fill in private data.
        Status = GetBulkEndPoints (Interface);
        DEBUG((DEBUG_INFO, "GetBulkEndPoints: Status: %r \n", Status));
        if (EFI_ERROR(Status)) {
            DEBUG((DEBUG_ERROR, "Failed to get Bulk In and Out end point addresses! \n"));
            return Status;
        }
        Interface->Usb->UsbCommBufferPtr = (UINTN)(UINTN *)AllocateRuntimeZeroPool (AMI_IPMI_MAX_USB_CMD_DATA_SIZE);
        if (!Interface->Usb->UsbCommBufferPtr) {
            DEBUG ((DEBUG_ERROR, "%a - Failed to allocate memory for Interface->Usb->UsbCommBufferPtr! \n", __FUNCTION__));
            return EFI_OUT_OF_RESOURCES;
        }
        // Assign Timeout value to USB structure member.
        Interface->Usb->BulkTransferTimeOut = PcdGet16 (AmiPcdBulkDataTransferTimeout);
        return Status;
    }

    return EFI_NOT_FOUND;
}
