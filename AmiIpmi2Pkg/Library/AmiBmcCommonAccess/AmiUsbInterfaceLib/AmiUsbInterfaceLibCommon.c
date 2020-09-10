//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file AmiUsbInterfaceLibCommon.c
    AMI USB Interface Implementation common functions.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/AmiUsbInterfaceLib.h>
#include <Library/AmiBmcCommonInterfaceLib.h>

//---------------------------------------------------------------------------

/** @internal
    Sends command data to BMC USB interface.

    @param[in]      Interface       Pointer to System interface.
    @param[in]      IpmiRequest     Pointer to input command data
    @param[in, out] DataSize        On input, Size of command data.
                                    On Output, Size of response data.

    @retval EFI_SUCCESS             Data sent successfully.
    @retval EFI_INVALID_PARAMETER   Data is too big or too small to send.
    @retval EFI_UNSUPPORTED         Unsupported command.
    @retval EFI_DEVICE_ERROR        Unknown error.
    @retval Others                  Error status while sending the data.

**/
EFI_STATUS
SendDataToUsbBmc (
  IN     AMI_SYSTEM_INTERFACE   *Interface,
  IN     UINT8                  *IpmiRequest,
  IN OUT UINT32                 *DataSize )
{
    EFI_STATUS              Status;
    UINT32                  Timeout;
    UINT32                  DataTransferSize;
    AMI_IPMI_CONFIG_CMD     CmdHeader = { \
                                            AMI_IPMI_BEGIN_SIG, \
                                            AMI_IPMI_USB_CMD_RESERVED, \
                                            0, 0, 0, 0, 0 \
                                        };

    // Size of input command data.
    CmdHeader.DataInLen = *DataSize;

    // Send the Command header.
    DataTransferSize = sizeof(CmdHeader);
    Status = AmiIpmiIssueBmcUsbTransaction (
                Interface,
                AMI_IPMI_COMMON_WRITE_10_OPCODE,
                AMI_IPMI_CMD_SECTOR,
                Interface->Usb->BulkOutEndpointAddr,
                (UINT8*)&CmdHeader,
                DataTransferSize,
                NULL );
    DEBUG((DEBUG_INFO, "Send the Command header: AmiIpmiIssueBmcUsbTransaction: Status: %r \n", Status));
    if (EFI_ERROR(Status)) {
        return Status;
    }

    // Send the IPMI packets.
    DataTransferSize = *DataSize;
    Status = AmiIpmiIssueBmcUsbTransaction (
                Interface,
                AMI_IPMI_COMMON_WRITE_10_OPCODE,
                AMI_IPMI_DATA_SECTOR,
                Interface->Usb->BulkOutEndpointAddr,
                IpmiRequest,
                DataTransferSize,
                NULL );
    DEBUG((DEBUG_INFO, "Send the IPMI packets : AmiIpmiIssueBmcUsbTransaction: Status: %r \n", Status));
    if (EFI_ERROR(Status)) {
        return Status;
    }

    Timeout = AMI_IPMI_TWO_THOUSAND_MS;
    // Wait for command completion, Read Status now.
    do {
        DataTransferSize = sizeof (CmdHeader);
        Status = AmiIpmiIssueBmcUsbTransaction (
                    Interface,
                    AMI_IPMI_COMMON_READ_10_OPCODE,
                    AMI_IPMI_CMD_SECTOR,
                    Interface->Usb->BulkInEndpointAddr,
                    (UINT8*)&CmdHeader,
                    DataTransferSize,
                    NULL );
        DEBUG((DEBUG_INFO, "Wait for command completion : AmiIpmiIssueBmcUsbTransaction: Status: %r \n", Status));
        if (EFI_ERROR(Status)) {
            return Status;
        }
        // Stall for 1ms.
        if (CmdHeader.Status & AMI_IPMI_IN_PROCESS) {
            MicroSecondDelay (AMI_IPMI_ONE_THOUSAND_MICRO_SECOND);
        } else {
            break;
        }
    } while(--Timeout);

    // Check for the status of command completion.
    switch (CmdHeader.Status) {
        case AMI_IPMI_ERR_SUCCESS:
            Status = EFI_SUCCESS;
            // If command is successful, update the ResponseDataSize.
            *DataSize = CmdHeader.DataOutLen;
            DEBUG((DEBUG_INFO,"ResponseDataSize: %lx \n", *DataSize));
            break;
        case AMI_IPMI_ERR_BIG_DATA:
            DEBUG((DEBUG_ERROR,"Too much data send to IPMI over USB\n"));
            Status = EFI_INVALID_PARAMETER;
            break;
        case AMI_IPMI_ERR_NO_DATA:
            DEBUG((DEBUG_ERROR,"Too little data send to IPMI over USB\n"));
            Status = EFI_INVALID_PARAMETER;
            break;
        case AMI_IPMI_ERR_UNSUPPORTED:
            DEBUG((DEBUG_ERROR,"Unsupported command\n"));
            Status = EFI_UNSUPPORTED;
            break;
        default:
            DEBUG((DEBUG_ERROR,"Unknown status\n"));
            Status = EFI_DEVICE_ERROR;
    }

    return Status;
}

/** @internal
    Receives response data from BMC USB interface.

    @param[in]      Interface           Pointer to System interface.
    @param[in]      ResponseData        Pointer to the response data.
    @param[in, out] ResponseDataSize    Pointer to size of response data.
    @param[out]     CompletionCode      Pointer to completion code.

    @retval EFI_SUCCESS         Data received successfully.
    @retval EFI_DEVICE_ERROR    Unknown error.
    @retval Others              Error status while receiving the data.

**/
EFI_STATUS
ReceiveDataFromUsbBmc (
  IN     AMI_SYSTEM_INTERFACE   *Interface,
  IN     UINT8                  *ResponseData,
  IN OUT UINT32                 *ResponseDataSize,
  OUT    UINT8                  *CompletionCode )
{
    EFI_STATUS              Status;
    UINT32                  DataTransferSize = *ResponseDataSize;
    AMI_IPMI_CONFIG_CMD     CmdHeader = { \
                                            AMI_IPMI_BEGIN_SIG, \
                                            AMI_IPMI_USB_CMD_RESERVED, \
                                            0, 0, 0, 0, 0 \
                                        };

    DEBUG((DEBUG_INFO, "%a: DataTransferSize: %x \n", __FUNCTION__, DataTransferSize));

    // Get the IPMI response data.
    Status = AmiIpmiIssueBmcUsbTransaction (
                Interface,
                AMI_IPMI_COMMON_READ_10_OPCODE,
                AMI_IPMI_DATA_SECTOR,
                Interface->Usb->BulkInEndpointAddr,
                ResponseData,
                DataTransferSize,
                CompletionCode );
    DEBUG((DEBUG_INFO, "Get the IPMI response: AmiIpmiIssueBmcUsbTransaction: Status: %r \n", Status));
    if (EFI_ERROR(Status)) {
        return Status;
    }

    /* Send configuration command to see if read of data itself had any problems.
       This read verifies that the previous read above was okay.*/
    DataTransferSize = sizeof(CmdHeader);
    Status = AmiIpmiIssueBmcUsbTransaction (
                Interface,
                AMI_IPMI_COMMON_READ_10_OPCODE,
                AMI_IPMI_CMD_SECTOR,
                Interface->Usb->BulkInEndpointAddr,
                (UINT8*)&CmdHeader,
                DataTransferSize,
                NULL );
    DEBUG((DEBUG_INFO, "Send configuration command: AmiIpmiIssueBmcUsbTransaction: Status: %r \n", Status));
    if (EFI_ERROR(Status)) {
        return Status;
    }

    /* Check the status of transaction to read response data.
       For success case, update response data size and return EFI_SUCCESS
       For error case, return EFI_DEVICE_ERROR.*/
    if (CmdHeader.Status == AMI_IPMI_ERR_SUCCESS) {
        *ResponseDataSize = CmdHeader.DataOutLen;
        Status = EFI_SUCCESS;
    } else {
        Status = EFI_DEVICE_ERROR;
    }

    return Status;
}

/** @internal
    USB interface Ipmi send command Implementation.

    @param[in]      Interface           Pointer to System interface.
    @param[in]      NetFunction         Net function of the command.
    @param[in]      Lun                 Logical Unit Number of the command.
    @param[in]      Command             Command to be sent to BMC.
    @param[in]      CommandData         Command data to be sent along with
                                        Command.
    @param[in]      CommandDataSize     Command Data size.
    @param[out]     ResponseData        Pointer to the response data buffer.
    @param[in, out] ResponseDataSize    Pointer to the response data size.
    @param[out]     CompletionCode      Pointer to completion code.
    @param[in]      InterfaceType       Interface type.
    @param[in]      Context             NULL here.

    @retval EFI_UNSUPPORTED         Interface type is not supported.
    @retval EFI_NOT_READY           Interface is not initialized.
    @retval EFI_ACCESS_DENIED       Interface is locked.
    @retval EFI_INVALID_PARAMETER   Invalid parameter.
    @retval EFI_DEVICE_ERROR        Error completion code or data size
                                    retrieved is small.
    @retval EFI_BUFFER_TOO_SMALL    Buffer is too small to update response
                                    data.
    @retval EFI_SUCCESS             Command sent successfully.
    @return Others                  Error status returned from BMC while
                                    executing the command.

**/
EFI_STATUS
AmiIpmiUsbSendCommand (
  IN     AMI_SYSTEM_INTERFACE       *Interface,
  IN     UINT8                      NetFunction,
  IN     UINT8                      Lun,
  IN     UINT8                      Command,
  IN     UINT8                      *CommandData,
  IN     UINT32                     CommandDataSize,
  OUT    UINT8                      *ResponseData,
  IN OUT UINT32                     *ResponseDataSize,
  OUT    UINT8                      *CompletionCode,
  IN     AMI_SYSTEM_INTERFACE_TYPE  InterfaceType,
  IN     VOID                       *Context )
{
    EFI_STATUS                  Status;
    AMI_IPMI_USB_COMMAND_DATA   *IpmiRequest;
    UINT32                      DataSize;

    if (InterfaceType != AmiSysInterfaceUsb){
        return EFI_UNSUPPORTED;
    }

    if (Interface->Usb == NULL || Interface->Usb->InterfaceState != AmiIpmiInterfaceInitialized)
        return EFI_NOT_READY;

    if (!Interface->Usb->UsbInterfaceApiPtr)
        return EFI_UNSUPPORTED;

    if (!ResponseDataSize || (!ResponseData && *ResponseDataSize) || (CommandDataSize && !CommandData)) {
        return EFI_INVALID_PARAMETER;
    }
    
    if (AmiIpmiIsIpmiTransportlocked (&Interface->Usb->UsbTransportLocked)) {
        return EFI_ACCESS_DENIED;
    } else {
        AmiIpmiTransportAcquireLock (&Interface->Usb->UsbTransportLocked);
    }

    // Fill the Ipmi Command and NetFnLun in UsbCommBufferPtr.
    IpmiRequest = (AMI_IPMI_USB_COMMAND_DATA*)(UINTN *)Interface->Usb->UsbCommBufferPtr;
    IpmiRequest->NetFnLun = AMI_IPMI_COMBINE_NETFUN_LUN (NetFunction, Lun);
    IpmiRequest->Command = Command;

    /* Send Data to UsbBmc
       Data size is calculated as command data size + command header size.*/
    DataSize = AMI_IPMI_USB_COMMAND_DATA_HEADER_SIZE + CommandDataSize;
    if ((Status = SendDataToUsbBmc (
                    Interface,
                    (UINT8*)CommandData,
                    &DataSize )) != EFI_SUCCESS) {
        AmiIpmiTransportReleaseLock (&Interface->Usb->UsbTransportLocked);
        return Status;
    }

    /* SendDataToUsbBmc() function updates the ResponseDataSize value in DataSize.
       Check if the ResponseData input variable is capable of holding complete data.
       Validate ResponseDataSize, if it is lesser than required response data size,
       update required data size in it and return EFI_BUFFER_TOO_SMALL.*/
    if (DataSize > *ResponseDataSize) {
        *ResponseDataSize = DataSize;
        AmiIpmiTransportReleaseLock (&Interface->Usb->UsbTransportLocked);
        return EFI_BUFFER_TOO_SMALL;
    }

    if ((Status = ReceiveDataFromUsbBmc (
                    Interface,
                    (UINT8*)ResponseData,
                    &DataSize,
                    CompletionCode )) != EFI_SUCCESS) {
        AmiIpmiTransportReleaseLock (&Interface->Usb->UsbTransportLocked);
        return Status;
    }

    // Check for command completion code.
    if (AMI_IPMI_ERROR_COMPLETION_CODE(ResponseData[0])) {
        /* Write completion code into return buffer and return device error
           if an IPMI command returns an error.*/
        if (*ResponseDataSize != 1) {
            *ResponseDataSize = 1;
        }
        AmiIpmiTransportReleaseLock (&Interface->Usb->UsbTransportLocked);
        return EFI_DEVICE_ERROR;
    }
    // Update the ResponseDataSize with output data.
    *ResponseDataSize = DataSize;

    AmiIpmiTransportReleaseLock (&Interface->Usb->UsbTransportLocked);
    return Status;
}

/** @internal
    Initialize USB interface specific data.

    @param[in, out]  Interface  System interface pointer.

    @retval EFI_SUCCESS             Interface is successfully initialized.
    @retval EFI_OUT_OF_RESOURCES    Enough resources are not available.
    @retval EFI_UNSUPPORTED         Interface is not supported.
    @retval Others                  Error status while initializing interface.

**/
EFI_STATUS
AmiInitUsbInterfaceData (
  IN OUT AMI_SYSTEM_INTERFACE   *Interface )
{
    EFI_STATUS  Status = EFI_SUCCESS;
    
    if ((Interface->Usb != NULL) && (Interface->Usb->InterfaceState == AmiIpmiInterfaceInitialized)) {
        return Status;
    }
    
    Status = AmiIpmiGetUsbApiPtr (Interface);
    
    if (EFI_ERROR(Status)) {
        if ((Status == EFI_OUT_OF_RESOURCES) || (Status == EFI_UNSUPPORTED) || (Interface->Usb == NULL)) {
            return Status;
        }
        Interface->Usb->InterfaceState = AmiIpmiInterfaceInitError;
    } else {
        Interface->Usb->InterfaceState = AmiIpmiInterfaceInitialized;
    }
    
    AmiIpmiTransportReleaseLock (&Interface->Usb->UsbTransportLocked);
    
    return Status;
}


