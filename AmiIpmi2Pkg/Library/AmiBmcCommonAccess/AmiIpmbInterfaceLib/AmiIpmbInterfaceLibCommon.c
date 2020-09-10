//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file AmiIpmbInterfaceLibCommon.c
    IPMB Transport implementation common library functions.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmiBmcCommonInterfaceLib.h>
#include <Library/AmiIpmbInterfaceLib.h>

//---------------------------------------------------------------------------

/** @internal
    Verify the data integrity using checksum of BMC response data.

    @param[in] ResponseData    Response data from BMC.
    @param[in] ResponseSize    Data size of response data.

    @retval EFI_SUCCESS             Data integrity is valid.
    @retval EFI_INVALID_PARAMETER   Invalid parameter.

**/
EFI_STATUS
CheckDataValidity (
  IN UINT8  *ResponseData,
  IN UINT8  ResponseSize )
{
    UINT8   Index;
    UINT8   CheckSum = 0;
    UINT8   DataSum = 0;

    // Calculate header checksum.
    for (Index = 0; Index < 2; Index++) {
        DataSum += ResponseData[Index];
    }

    // Verify header checksum.
    CheckSum = (UINT8) (0x100 - DataSum);
    if (CheckSum != ResponseData[2]) {
        return EFI_INVALID_PARAMETER;
    }

    DataSum = 0;

    // Calculate information checksum.
    for (Index = 3; Index < (ResponseSize - 1); Index++) {
        DataSum += ResponseData[Index];
    }

    // Verify information checksum.
    CheckSum = (UINT8) (0x100 - DataSum);
    if (CheckSum != ResponseData[ResponseSize - 1]) {
        return EFI_INVALID_PARAMETER;
    }
    return EFI_SUCCESS;
}

/** @internal
    Sends the command/data to Ipmb interface.

    @param[in]  Interface   Pointer to System interface.
    @param[in]  Context     NULL here.
    @param[in]  Data        Pointer to command data that will be sent to BMC
                            along with Command.
    @param[in]  DataSize    Size of the command data.

    @return EFI_STATUS  Status returned from I2C send command.

**/
EFI_STATUS
SendDataToIpmbBmcPort (
  IN AMI_SYSTEM_INTERFACE   *Interface,
  IN VOID                   *Context,
  IN UINT8                  *Data,
  IN UINT8                  DataSize )
{
    EFI_STATUS              Status;
    EFI_I2C_REQUEST_PACKET  Packet;
    
    // Pack command data.
    Packet.Operation[0].Buffer          = Data;
    Packet.Operation[0].LengthInBytes   = DataSize;
    Packet.Operation[0].Flags           = AMI_IPMI_WRITE_FLAG;

    // Call the StartRequest function.
    Status = AmiIpmiI2cSendCommand (
                Interface,
                IPMI_BMC_SLAVE_ADDRESS,
                &Packet );
    
    return Status;
}

/** @internal
    Receives the data from Ipmb interface.

    @param[in]  Interface   Pointer to System interface.
    @param[in]  Context     NULL here.
    @param[out] Data        Pointer to response data that is received from BMC.
    @param[out] DataSize    Size of the response data.

    @return EFI_STATUS  Status returned from I2C send command.

**/
EFI_STATUS
ReceiveBmcDataFromIpmbPort (
  IN  AMI_SYSTEM_INTERFACE  *Interface,
  IN  VOID                  *Context,
  OUT UINT8                 *Data,
  OUT UINT8                 *DataSize )
{
    EFI_STATUS                  Status;
    EFI_I2C_REQUEST_PACKET      Packet;

    // Pack command data.
    Packet.Operation[0].Buffer  = Data;
    Packet.Operation[0].Flags   = AMI_IPMI_READ_FLAG;

    // Call the StartRequest function.
    Status = AmiIpmiI2cSendCommand (
                Interface,
                IPMI_BMC_SLAVE_ADDRESS,
                &Packet );
    
    if(!EFI_ERROR(Status)) {
        Data        = Packet.Operation[0].Buffer;
        *DataSize   = (UINT8)Packet.Operation[0].LengthInBytes;
    }
    return Status;
}

/** @internal
    IPMB interface send command implementation.

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
    @retval EFI_BAD_BUFFER_SIZE     Invalid buffer size.
    @retval EFI_DEVICE_ERROR        Error completion code or data size
                                    retrieved is small.
    @retval EFI_BUFFER_TOO_SMALL    Buffer is too small to update response
                                    data.
    @retval EFI_SUCCESS             Command sent successfully.
    @return Others                  Error status returned from BMC while
                                    executing the command.

**/
EFI_STATUS
AmiIpmiIpmbSendCommand (
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
    EFI_STATUS      Status;
    UINT8           DataSize;
    UINT8           DataSum = 0;
    UINT8           CheckSum = 0;
    UINT8           Index;
    UINT8           CmdDataBuffer[AMI_IPMI_MAX_IPMB_CMD_DATA_SIZE];
    
    if (InterfaceType != AmiSysInterfaceIpmb) {
        return EFI_UNSUPPORTED;
    }

    if (Interface->Ipmb.InterfaceState != AmiIpmiInterfaceInitialized) {
        return EFI_NOT_READY;
    }

    if (!ResponseDataSize || (!ResponseData && *ResponseDataSize)) {
        return EFI_INVALID_PARAMETER;
    }

    if (AmiIpmiIsIpmiTransportlocked (&Interface->Ipmb.IpmbTransportLocked)) {
        return EFI_ACCESS_DENIED;
    } else {
        AmiIpmiTransportAcquireLock (&Interface->Ipmb.IpmbTransportLocked);
    }

    if (Interface->Ipmb.IpmbSoftErrorCount >= MAX_BMC_CMD_FAIL_COUNT) {
        AmiIpmiTransportReleaseLock (&Interface->Ipmb.IpmbTransportLocked);
        return EFI_NOT_READY;
    }

    /* Request Packet format.
       | Slave Address | Netfun/Lun | CheckSum (Check Sum of previous data)
       | Slave Address | Seq No | Command | Data 1..N
       | CheckSum (Check Sum of previous data).*/
    CmdDataBuffer[0] = IPMI_BMC_SLAVE_ADDRESS;
    CmdDataBuffer[1] = (UINT8) ((NetFunction << 2) | (Lun & 0x03));

    DataSum += CmdDataBuffer[1] + IPMI_BMC_SLAVE_ADDRESS;
    CheckSum = (UINT8) (0x100 - DataSum);
    DataSum = 0;

    CmdDataBuffer[2] = CheckSum;

    CmdDataBuffer[3] = IPMI_BMC_SLAVE_ADDRESS;
    DataSum += IPMI_BMC_SLAVE_ADDRESS;

    CmdDataBuffer[4] = AMI_IPMI_SEQ_NO;
    DataSum += AMI_IPMI_SEQ_NO;

    CmdDataBuffer[5] = Command;
    DataSum += Command;

    if (CommandDataSize > 0) {
        if (CommandData == NULL) {
            AmiIpmiTransportReleaseLock (&Interface->Ipmb.IpmbTransportLocked);
            return EFI_INVALID_PARAMETER;
        }
        //Last data will be Checksum so limiting the Max command data to < AMI_IPMI_MAX_IPMB_CMD_DATA_SIZE - 6
        if (CommandDataSize < (AMI_IPMI_MAX_IPMB_CMD_DATA_SIZE - 6)) {
            CopyMem (
                &CmdDataBuffer[6],
                CommandData,
                CommandDataSize );
        } else {
           AmiIpmiTransportReleaseLock (&Interface->Ipmb.IpmbTransportLocked);
           return EFI_BAD_BUFFER_SIZE;
        }

        for(Index = 0; Index < CommandDataSize; Index++) {
            DataSum += CmdDataBuffer[6 + Index];
        }
    }
    CheckSum = (UINT8) (0x100 - DataSum);  // Find the checksum for the packing data.
    CmdDataBuffer[6] = CheckSum;  // Update the checksum.

    if ((Status = SendDataToIpmbBmcPort (
                    Interface,
                    Context,
                    CmdDataBuffer,
                    (UINT8) (CommandDataSize + 7) ) ) != EFI_SUCCESS) {
        Interface->Ipmb.IpmbSoftErrorCount++;
        AmiIpmiTransportReleaseLock (&Interface->Ipmb.IpmbTransportLocked);
        return Status;
    }

    DataSize = AMI_IPMI_MAX_IPMB_CMD_DATA_SIZE;
    if ((Status = ReceiveBmcDataFromIpmbPort (
                    Interface,
                    Context,
                    CmdDataBuffer,
                    &DataSize )) != EFI_SUCCESS) {
        Interface->Ipmb.IpmbSoftErrorCount++;
        AmiIpmiTransportReleaseLock (&Interface->Ipmb.IpmbTransportLocked);
        return Status;
    }

    /* Response Packet format.
       | Slave Address | Netfun/Lun | CheckSum (Check Sum of previous data)
       | Slave Address | Seq No | Command | Completion code| Data 1..N
       | CheckSum (Check Sum of previous data).*/

    // Calculate and verify checksum.
    Status = CheckDataValidity (
                CmdDataBuffer,
                DataSize );
    if (EFI_ERROR(Status)) { 
        AmiIpmiTransportReleaseLock (&Interface->Ipmb.IpmbTransportLocked);
        return Status;
    }

    if (CompletionCode != NULL) {
        *CompletionCode = CmdDataBuffer[6];
    }

    if (AMI_IPMI_ERROR_COMPLETION_CODE(CmdDataBuffer[6])) {
        AmiIpmiUpdateSoftErrorCount (
            CmdDataBuffer[6],
            Interface,
            InterfaceType );
        // Write completion code into return buffer if an IPMI command returns an error
        if (*ResponseDataSize) {
            if (ResponseData) {
                *ResponseData = CmdDataBuffer[6];
            }
            *ResponseDataSize = 1;
        }
        AmiIpmiTransportReleaseLock (&Interface->Ipmb.IpmbTransportLocked);
        return EFI_DEVICE_ERROR;
    }

    if (DataSize < 8) {
        AmiIpmiTransportReleaseLock (&Interface->Ipmb.IpmbTransportLocked);
        return EFI_DEVICE_ERROR;
    }
    if ((DataSize - 7) > *((UINT8 *)ResponseDataSize)) {
        *ResponseDataSize = (UINT8) (DataSize - 7);
        AmiIpmiTransportReleaseLock (&Interface->Ipmb.IpmbTransportLocked);
        return EFI_BUFFER_TOO_SMALL;
    }

    // Copying the response data into ResponseData buffer.
    CopyMem (
        ResponseData,
        &CmdDataBuffer[6],
        (DataSize - 7) );
    *ResponseDataSize = (UINT8) (DataSize - 7);

    AmiIpmiTransportReleaseLock (&Interface->Ipmb.IpmbTransportLocked);
    return EFI_SUCCESS;
}

/** @internal
    Initialize IPMB interface specific data.

    @param[in, out]  Interface  System interface pointer.

    @retval EFI_SUCCESS     Interface is successfully initialized.
    @retval Others          Error status while initializing interface.

**/
EFI_STATUS
AmiInitIpmbInterfaceData (
  IN OUT AMI_SYSTEM_INTERFACE   *Interface )
{
    EFI_STATUS  Status = EFI_SUCCESS;
    
    if (Interface->Ipmb.InterfaceState == AmiIpmiInterfaceInitialized) {
        return Status;
    }
    
    Status = AmiIpmiGetI2cApiPtr (Interface);
    
    if (EFI_ERROR(Status)) {
        Interface->Ipmb.InterfaceState = AmiIpmiInterfaceInitError;
        return Status;
    }
    
    AmiIpmiTransportReleaseLock (&Interface->Ipmb.IpmbTransportLocked);
    
    Interface->Ipmb.InterfaceState = AmiIpmiInterfaceInitialized;
    
    DEBUG ((DEBUG_INFO, "%a Initialized Status = %r\n", __FUNCTION__, Status));
    
    return Status;
}

