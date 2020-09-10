//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file PeiAmiUsbInterfaceLib.c
    USB Transport Pei phase Implementation library functions.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <AmiModulePkg/Usb/Pei/UsbBotPeim.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/AmiUsbInterfaceLib.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

/* The Value field specifies the descriptor type in the high byte and
   the descriptor index in the low byte.
   Value = Type << 8 | Index. */
#define DESCRIPTOR_TYPE_INDEX_VALUE(Type)  ((Type << 8) | 0x00)

// Status code definition to report BMC Selftest fail.
#ifndef AMI_PEIM_EC_USB_BMC_SELFTEST_FAILED
#define AMI_PEIM_EC_USB_BMC_SELFTEST_FAILED    (AMI_STATUS_CODE_CLASS | 0x0000000C)
#endif

#ifndef PEI_USB_BMC_SELFTEST_FAILED
#define PEI_USB_BMC_SELFTEST_FAILED     (EFI_SOFTWARE_PEI_MODULE | AMI_PEIM_EC_USB_BMC_SELFTEST_FAILED)
#endif

//---------------------------------------------------------------------------

/** @internal
    If following conditions are met, returns TRUE else FALSE.
    1. Checks for PEI_BOT_DEVICE_SIGNATURE.
    2. Checks for Vendor ID and Product ID of BMC USB Interface.

    @param[in] PeiServices      Indirect reference to the PEI Services Table.
    @param[in] PeiBotDevice     Pointer to PEI BOT device structure.

    @retval TRUE    BMC USB Interface is found.
    @retval FALSE   BMC USB Interface is not found.

**/
BOOLEAN
IsBmcUsbInterface (
  IN EFI_PEI_SERVICES   **PeiServices,
  IN PEI_BOT_DEVICE     *PeiBotDevice )
{
    EFI_STATUS                  Status;
    PEI_USB_IO_PPI              *UsbIo = NULL;
    EFI_USB_DEVICE_REQUEST      DevReq;
    EFI_USB_DEVICE_DESCRIPTOR   DevDesc;
    UINTN                       DescriptorLength;
    UINT32                      UsbStatus;

    /* Verify if this Block IO PPI is installed by USB PEI driver.
       Use signature for this purpose. Return FALSE, if
       Signature does not match with PEI_BOT_DEVICE_SIGNATURE.*/
    if (PeiBotDevice->Signature != PEI_BOT_DEVICE_SIGNATURE) {
        return FALSE;
    }

    // Get the pointer to UsbIo from PEI_BOT_DEVICE structure.
    UsbIo = PeiBotDevice->UsbIoPpi;

    /* GetDeviceDescriptor API is not available in UsbIoPpi.
       So Get Device descriptor details using UsbIo->ControlTransfer API.
       Fill the Device request structure.*/
    DevReq.RequestType  = USB_DEV_GET_DESCRIPTOR_REQ_TYPE;
    DevReq.Request      = USB_DEV_GET_DESCRIPTOR;
    DevReq.Value        = DESCRIPTOR_TYPE_INDEX_VALUE (USB_DT_DEVICE);
    DevReq.Index        = 0x00;
    DevReq.Length       = sizeof (DevDesc);
    DescriptorLength    = sizeof (DevDesc);

    // Issue ControlTranfer transaction.
    Status = UsbIo->UsbControlTransfer (
                        PeiServices,
                        UsbIo,
                        &DevReq,
                        EfiUsbDataIn,
                        AMI_IPMI_THREE_THOUSAND_MS,
                        &DevDesc,
                        DescriptorLength,
                        &UsbStatus );
    // Return FALSE, For Error case.
    if (EFI_ERROR(Status)) {
        return FALSE;
    }

    /* Check for Vendor ID and Product ID
       Return TRUE, If Vendor ID and Product ID matches
       with BMC Usb Interface. Else, return FALSE.*/
    if ((DevDesc.IdVendor == BMC_USB_INTERFACE_VENDOR_ID) &&\
         (DevDesc.IdProduct == BMC_USB_INTERFACE_PRODUCT_ID)) {
        DEBUG ((DEBUG_INFO, "BMC USB interface is found.\n"));
        return TRUE;
    } else {
        return FALSE;
    }
}

/** @internal
    Sends Clear Feature command for specified Bulk-In/Bulk-out endpoint.

    @param[in] Interface        Pointer to System interface.
    @param[in] EndpointAddr     Bulk Endpoint Address.

    @return EFI_STATUS  Status returned from usb transaction.

**/
EFI_STATUS
BotClearEndpoint (
  IN AMI_SYSTEM_INTERFACE   *Interface,
  IN UINT16                 EndpointAddr )
{
    EFI_USB_DEVICE_REQUEST  DevReq;
    EFI_STATUS              Status;
    UINT32                  UsbStatus;
    PEI_USB_IO_PPI          *UsbIo = (PEI_USB_IO_PPI *)(UINTN *)Interface->Usb->UsbInterfaceApiPtr;
    EFI_PEI_SERVICES        **PeiServices = (EFI_PEI_SERVICES **)Interface->Usb->Context;

    ZeroMem (
        &DevReq,
        sizeof(EFI_USB_DEVICE_REQUEST) );

    // Fill the Device Request structure to Clear specific device feature.
    DevReq.RequestType  = USB_REQ_TYPE_ENDPOINT;
    DevReq.Request      = USB_DEV_CLEAR_FEATURE;
    DevReq.Value        = EfiUsbEndpointHalt;
    DevReq.Length       = 0x00;

    // Clear Feature Halt for provided Bulk-In/Bulk-out Endpoint.
    DevReq.Index = (UINT8)EndpointAddr;

    // Issue ControlTranfer transaction.
    Status = UsbIo->UsbControlTransfer (
                        PeiServices,
                        UsbIo,
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

    @return EFI_STATUS  Status returned from usb transaction.

**/
EFI_STATUS
BotRecoveryReset (
  IN AMI_SYSTEM_INTERFACE   *Interface )
{
    EFI_STATUS              Status;
    EFI_USB_DEVICE_REQUEST  DevReq;
    EFI_PEI_SERVICES        **PeiServices = (EFI_PEI_SERVICES **)Interface->Usb->Context;
    PEI_USB_IO_PPI          *UsbIo = (PEI_USB_IO_PPI *)(UINTN *)Interface->Usb->UsbInterfaceApiPtr;
    UINT32                  UsbStatus;

    ZeroMem (
        &DevReq,
        sizeof(EFI_USB_DEVICE_REQUEST) );

    // Fill the Device Request structure to issue the Bulk-Only Mass Storage Reset.
    DevReq.RequestType  = USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE;
    DevReq.Request      = 0xFF;
    DevReq.Value        = 0x00;
    DevReq.Index        = 0x00;
    DevReq.Length       = 0x00;

    // Issue ControlTranfer transaction.
    Status = UsbIo->UsbControlTransfer (
                        PeiServices,
                        UsbIo,
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

    // Clear Feature Halt for Bulk-In End point.
    BotClearEndpoint (
        Interface,
        Interface->Usb->BulkInEndpointAddr );

    // Clear Feature Halt for Bulk-Out Endpoint.
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
    EFI_STATUS          Status;
    AMI_IPMI_CSW        Csw;
    UINTN               DataSize = sizeof(Csw);
    EFI_PEI_SERVICES    **PeiServices = (EFI_PEI_SERVICES **)Interface->Usb->Context;
    PEI_USB_IO_PPI      *UsbIo = (PEI_USB_IO_PPI *)(UINTN *)Interface->Usb->UsbInterfaceApiPtr;
    UINT32              UsbStatus = 0;

    ZeroMem (
        &Csw,
        DataSize );

    // Get the status field from bulk transfer.
    Status = UsbIo->UsbBulkTransfer (
                        PeiServices,
                        UsbIo,
                        Interface->Usb->BulkInEndpointAddr,
                        &Csw,
                        &DataSize,
                        Interface->Usb->BulkTransferTimeOut,
                        &UsbStatus );
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_ERROR, "Dxe BotStatusPhase Error : Status = %r UsbStatus = %x\n",Status,UsbStatus));
        if (UsbStatus == EFI_USB_ERR_STALL) {

          // Clear stall in Bulk-In Endpoint.
          BotClearEndpoint(
              Interface,
              Interface->Usb->BulkInEndpointAddr );

          // Send bulk transfer command again.
          ZeroMem (
              &Csw,
              sizeof(Csw) );
          
          DataSize = sizeof(AMI_IPMI_CSW);
          UsbStatus = 0;
          Status = UsbIo->UsbBulkTransfer (
                            PeiServices,
                            UsbIo,
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
    if ((DataSize != sizeof (AMI_IPMI_CSW)) || \
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
    EFI_PEI_SERVICES        **PeiServices = (EFI_PEI_SERVICES **)Interface->Usb->Context;
    PEI_USB_IO_PPI          *UsbIo = (PEI_USB_IO_PPI *)(UINTN *)Interface->Usb->UsbInterfaceApiPtr;
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
        Cbw.CbwFlags        = EfiUsbDataIn << AMI_IPMI_CBW_FLAG_SHIFT;
        CmdBuffer.Opcode    = AMI_IPMI_SCSI_AMICMD_CURI_WRITE;
    } else {
        Cbw.CbwFlags        = EfiUsbDataOut << AMI_IPMI_CBW_FLAG_SHIFT;
        CmdBuffer.Opcode    = AMI_IPMI_SCSI_AMICMD_CURI_READ;  
    }

    CmdBuffer.Lba               = SwapBytes32 (Sector);
    CmdBuffer.TransferLength    = AMI_IPMI_TRANSFER_LENGTH;
    CmdBuffer.Lun               = AMI_BMC_LUN;

    // Copy the buffer to command block's command buffer.
    CopyMem (
        Cbw.CbwCb,
        &CmdBuffer,
        sizeof(CmdBuffer) );

    // Send the command to host using bulk transfer.
    Status = UsbIo->UsbBulkTransfer (
                        PeiServices,
                        UsbIo,
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
  OUT UINT8                 *ResponseData,
  OUT UINT8                 *CompletionCode )
{
    EFI_STATUS          Status;
    PEI_USB_IO_PPI      *UsbIo = (PEI_USB_IO_PPI *)(UINTN *)Interface->Usb->UsbInterfaceApiPtr;
    UINT32              UsbStatus;
    UINT8               *TempData = (UINT8 *)(UINTN *)Interface->Usb->UsbCommBufferPtr;
    UINT32              DataBytesRead = 0;
    UINT32              MaxTempDataSize;
    EFI_PEI_SERVICES    **PeiServices = (EFI_PEI_SERVICES **)Interface->Usb->Context;

    // Read all the data from BMC in a loop.
    while ((DataBytesRead < (ResponseDataSize - 1 )) || (ResponseDataSize == 1)) {
        // Issue Bulk Transfer to read the data from BMC.
        MaxTempDataSize = AMI_IPMI_MAX_USB_CMD_DATA_SIZE;
        Status = UsbIo->UsbBulkTransfer (
                            PeiServices,
                            UsbIo,
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
    EFI_STATUS          Status;
    PEI_USB_IO_PPI      *UsbIo = (PEI_USB_IO_PPI *)(UINTN *)Interface->Usb->UsbInterfaceApiPtr;
    EFI_PEI_SERVICES    **PeiServices = (EFI_PEI_SERVICES **)Interface->Usb->Context;
    UINT32              UsbStatus;
    UINT32              RemainingDataBytes = CommandRequestDataSize;
    UINT32              IpmiRequestSize;
    UINT32              IpmiRequestOffset;
    UINT32              RequestDataOffset = 0;

    // Ipmi Command and NetFnLun is already filled in UsbCommBufferPtr by EfiSendIpmiCommand().
    UINT8               *IpmiRequest = (UINT8 *)(UINTN *)Interface->Usb->UsbCommBufferPtr;

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
            (IpmiRequestSize - IpmiRequestOffset) );
        RemainingDataBytes -= IpmiRequestSize;

        /* RequestDataOffset is calculated based on IpmiRequestSize as a
           check to avoid sending command data size more than specified BMC
           data size in MAX_USB_CMD_DATA_SIZE.*/
        RequestDataOffset += (IpmiRequestSize - IpmiRequestOffset);

        DEBUG((DEBUG_INFO, "RemainingDataBytes: %x \n", RemainingDataBytes));

        // Issue Bulk Transfer to write the data into BMC.
        Status = UsbIo->UsbBulkTransfer (
                            PeiServices,
                            UsbIo,
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
    EFI_STATUS          Status;
    EFI_PEI_SERVICES    **PeiServices = (EFI_PEI_SERVICES **)Interface->Usb->Context;
    PEI_USB_IO_PPI      *UsbIo = (PEI_USB_IO_PPI *)(UINTN *)Interface->Usb->UsbInterfaceApiPtr;
    UINT32              UsbStatus;

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
    if ((Opcode == AMI_IPMI_COMMON_READ_10_OPCODE) && \
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
        Status = UsbIo->UsbBulkTransfer (
                            PeiServices,
                            UsbIo,
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
    if (EFI_ERROR(Status)) {
        return Status;
    }

    return Status;
}

/** @internal
    Allocate memory for Usb interface, checks for Bmc usb interface
    and initializes the variables.

    @param[in, out] Interface   Pointer to System interface.

    @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.
    @retval EFI_NOT_FOUND           Block IO Ppi/Bmc usb interface is not
                                    found.
    @retval EFI_SUCCESS             Interface is found and initialized
                                    successfully.

**/
EFI_STATUS
AmiIpmiGetUsbApiPtr (
  IN OUT AMI_SYSTEM_INTERFACE   *Interface )
{
    EFI_STATUS                      Status;
    UINTN                           BlockIoPpiInstance = 0;
    EFI_PEI_RECOVERY_BLOCK_IO_PPI   *BlockIoPpi = NULL;
    PEI_BOT_DEVICE                  *PeiBotDevice = NULL;
    CONST EFI_PEI_SERVICES          **PeiServices = GetPeiServicesTablePointer ();

    // Allocate memory for USB interface.
    if (Interface->Usb == NULL) {
        Interface->Usb = (AMI_USB_SYSTEM_INTERFACE*)AllocateZeroPool (sizeof (AMI_USB_SYSTEM_INTERFACE));
        if (Interface->Usb == NULL) {
            DEBUG ((DEBUG_ERROR, "%a - Failed to allocate memory for Interface->Usb! \n", __FUNCTION__));
            return EFI_OUT_OF_RESOURCES;
        }
    }
    Interface->Usb->UsbInterfaceApiGuid = gEfiPeiVirtualBlockIoPpiGuid;
    Interface->Usb->Context = (VOID *)PeiServices;
    
    // Locate all the instances of Block Io Ppi.
    do {
        Status = (*PeiServices)->LocatePpi (
                                    PeiServices,
                                    &gEfiPeiVirtualBlockIoPpiGuid,
                                    BlockIoPpiInstance,
                                    NULL,
                                    (VOID **)&BlockIoPpi );
        
        DEBUG ((DEBUG_INFO, "gEfiPeiVirtualBlockIoPpiGuid Instance[%x] = Status %r\n", BlockIoPpiInstance, Status));
        
        // For error case, print the error status and return
        if (EFI_ERROR (Status)) {
            DEBUG ((DEBUG_ERROR, "BMC USB interface is not found.\n"));
            return EFI_NOT_FOUND;
        }

        // Get the pointer to PEI_BOT_DEVICE structure from BlockIoPpi.
        PeiBotDevice = PEI_BOT_DEVICE_FROM_THIS (BlockIoPpi);

        // Check for Bmc Usb Interface.
        if (IsBmcUsbInterface (
                (EFI_PEI_SERVICES **)PeiServices,
                PeiBotDevice )) {
            // Allocate memory for USB Interface Communication Buffer.
            Interface->Usb->UsbCommBufferPtr = (UINTN)(UINTN *)AllocateZeroPool (AMI_IPMI_MAX_USB_CMD_DATA_SIZE);
            
            if (!Interface->Usb->UsbCommBufferPtr) {
                DEBUG ((DEBUG_ERROR, "%a - Failed to allocate memory for Interface->Usb->UsbCommBufferPtr! \n", __FUNCTION__));
                return EFI_OUT_OF_RESOURCES;
            }
            
            Interface->Usb->UsbInterfaceApiPtr = (UINTN)PeiBotDevice->UsbIoPpi;
            Interface->Usb->BulkInEndpointAddr = PeiBotDevice->BulkInEndpoint->EndpointAddress;
            Interface->Usb->BulkOutEndpointAddr = PeiBotDevice->BulkOutEndpoint->EndpointAddress;
            Interface->Usb->BulkTransferTimeOut = PcdGet16 (AmiPcdBulkDataTransferTimeout);
            return EFI_SUCCESS;
        }

        // Increment the BlockIo PPI instance.
        BlockIoPpiInstance++;
    } while(TRUE);

    return EFI_NOT_FOUND;
}
