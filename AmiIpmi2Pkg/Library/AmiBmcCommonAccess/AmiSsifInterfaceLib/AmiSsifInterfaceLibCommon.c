//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file AmiSsifInterfaceLibCommon.c
    SSIF Transport Implementation common functions and variables.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <AmiIpmiInitHooks.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmiBmcCommonInterfaceLib.h>
#include <Library/AmiSsifInterfaceLib.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

extern AMI_SSIF_ALERT_PIN_CHECK AMI_SSIF_ALERT_PIN_CHECK_HOOK_LIST \
                                    EndOfSsifAlertPinCheckHookList;
AMI_SSIF_ALERT_PIN_CHECK    *gSsifAlertPinCheckHookList[] = { \
                                AMI_SSIF_ALERT_PIN_CHECK_HOOK_LIST \
                                NULL \
                                };


//---------------------------------------------------------------------------

/** @internal
    Check the SMBUS alert pin status function

    @param  VOID    Nothing.

    @retval TRUE    Alert pin status is set.
    @retval FALSE   Alert pin status is not set.

**/
BOOLEAN
CheckAlertPinHook (
  VOID )
{
    BOOLEAN                     CheckAlertSignal = FALSE;
    UINTN                       Index;

    for (Index = 0; gSsifAlertPinCheckHookList[Index];Index++) {
        CheckAlertSignal = gSsifAlertPinCheckHookList[Index]();
    }

    return CheckAlertSignal;
}

/** @internal
    Sends the command/data to Ssif interface.

    @param[in]  Interface   Pointer to System interface.
    @param[in]  Context     NULL here.
    @param[in]  Data        Pointer to command data that will be sent to BMC
                            along with Command.
    @param[in]  DataSize    Size of the command data.

    @return EFI_STATUS  Status returned from Smbus send command.

**/
EFI_STATUS
SendDataToSsifBmcPort (
  IN AMI_SYSTEM_INTERFACE   *Interface,
  IN VOID                   *Context,
  IN UINT8                  *Data,
  IN UINT8                  DataSize )
{
    EFI_STATUS                 Status;
    EFI_SMBUS_DEVICE_ADDRESS   BmcAddress;
    UINTN                      IpmiWriteCommand;
    UINT8                      IpmiData[AMI_IPMI_SMBUS_BLOCK_LENGTH] = {0};
    UINTN                      DataLength = DataSize;
    UINT8                      DataIndex = 0;
    BOOLEAN                    PECSupport;
    UINT8                      RetryCount = 0;
    UINT8                      OriginalDataSize = DataSize;

    PECSupport = Interface->Ssif.PecSupport;
    BmcAddress.SmbusDeviceAddress = IPMI_SSIF_SLAVE_ADDRESS;

    do {
        if (OriginalDataSize == DataSize) {
            if (DataSize <= AMI_IPMI_SMBUS_BLOCK_LENGTH) {
                // Working single writes start.
                DataLength = DataSize;
                IpmiWriteCommand = AMI_IPMI_SMBUS_SINGLE_WRITE_CMD;
                CopyMem (
                    IpmiData,
                    &Data[DataIndex*AMI_IPMI_SMBUS_BLOCK_LENGTH],
                    DataLength );
            } else {
                // Working multi-part writes start.
                IpmiWriteCommand = AMI_IPMI_SMBUS_MULTI_WRITE_START_CMD;
                DataLength = AMI_IPMI_SMBUS_BLOCK_LENGTH;
                CopyMem (
                    IpmiData,
                    &Data[DataIndex*AMI_IPMI_SMBUS_BLOCK_LENGTH],
                    DataLength );
            }
        } else {
            if (DataSize > AMI_IPMI_SMBUS_BLOCK_LENGTH) {
                // Working multi-part writes middle.
                IpmiWriteCommand = AMI_IPMI_SMBUS_MULTI_WRITE_MIDDLE_CMD;
                DataLength = AMI_IPMI_SMBUS_BLOCK_LENGTH;
                CopyMem (
                    IpmiData,
                    &Data[DataIndex*AMI_IPMI_SMBUS_BLOCK_LENGTH],
                    DataLength );
            } else {
                // Working multi-part writes end.
                IpmiWriteCommand = AMI_IPMI_SMBUS_MULTI_WRITE_END_CMD;
                DataLength = DataSize;
                CopyMem (
                    IpmiData,
                    &Data[DataIndex*AMI_IPMI_SMBUS_BLOCK_LENGTH],
                    DataLength );
            }
        }

        Status = AmiIpmiSmbusSendCommand (
                    Interface,
                    BmcAddress,
                    IpmiWriteCommand,
                    EfiSmbusWriteBlock,
                    PECSupport,
                    &DataLength,
                    IpmiData );

        if (!EFI_ERROR(Status)) {
            if (DataSize >=  AMI_IPMI_SMBUS_BLOCK_LENGTH) {
                RetryCount = 0;
                DataSize -= AMI_IPMI_SMBUS_BLOCK_LENGTH;
                DataIndex++;
            } else {
                DataSize = 0;
            }
        } else {
            if (RetryCount == Interface->Ssif.SsifRetryCounter) {
                break;
            } else {
                RetryCount++;
                // Failed retries delay about 60ms to 250ms.
                MicroSecondDelay (IPMI_SSIF_TIME_BETWEEN_REQUEST_RETRIES_DELAY);
                /* If the Multi-part write fails, then try to write the
                   data from the beginning.*/
                if (IpmiWriteCommand != AMI_IPMI_SMBUS_SINGLE_WRITE_CMD) {
                    DataSize = OriginalDataSize;
                    DataIndex = 0;
                }
            }
        }
    } while (DataSize);

    return Status;
}

/** @internal
    Receives the Data from BMC port

    @param[in]  Interface   Pointer to System interface.
    @param[in]  Context     NULL here.
    @param[out] Data        Pointer to response data that is received from
                            BMC.
    @param[out] DataSize    Size of the response data.

    @retval EFI_SUCCESS    Data received from BMC successfully.
    @retval Others         Status of the Receiving data to BMC port.

    @return EFI_STATUS  Status returned from Smbus send command.

**/
EFI_STATUS
ReceiveBmcDataFromSsifPort (
  IN  AMI_SYSTEM_INTERFACE  *Interface,
  IN  VOID                  *Context,
  OUT UINT8                 *Data,
  OUT UINT8                 *DataSize )
{
    EFI_STATUS                  Status;
    EFI_SMBUS_DEVICE_ADDRESS    BmcAddress;
    UINTN                       IpmiReadCommand;
    UINT8                       IpmiData[AMI_IPMI_SMBUS_BLOCK_LENGTH] = {0};
    UINTN                       DataLength = *DataSize;
    BOOLEAN                     PECSupport;
    UINT8                       RetryCount = 0;
    UINT8                       OriginalDataSize = *DataSize;

    PECSupport = Interface->Ssif.PecSupport;
    BmcAddress.SmbusDeviceAddress = IPMI_SSIF_SLAVE_ADDRESS;
    IpmiReadCommand = AMI_IPMI_SMBUS_SINGLE_READ_CMD;

    while (RetryCount <= Interface->Ssif.SsifRetryCounter) {
        Status = AmiIpmiSmbusSendCommand (
                    Interface,
                    BmcAddress,
                    IpmiReadCommand,
                    EfiSmbusReadBlock,
                    PECSupport,
                    &DataLength,
                    (VOID *)IpmiData );

        if (EFI_ERROR(Status)) {
            RetryCount++;
            // Failed retries delay about 60ms to 250ms.
            MicroSecondDelay (IPMI_SSIF_TIME_BETWEEN_REQUEST_RETRIES_DELAY);
            /* If the Multi-part Read command fails, then try to read the
               data from the beginning.*/
            if (IpmiReadCommand != AMI_IPMI_SMBUS_SINGLE_READ_CMD) {
                IpmiReadCommand = AMI_IPMI_SMBUS_SINGLE_READ_CMD;
            }
            DataLength = OriginalDataSize;
            continue;
        }

        if (IpmiReadCommand == AMI_IPMI_SMBUS_SINGLE_READ_CMD) {
            if ((IpmiData[0] == AMI_IPMI_MULTI_READ_ZEROTH_STRT_BIT) && \
                (IpmiData[1] == AMI_IPMI_MULTI_READ_FIRST_STRT_BIT)) {
                // Working multi-part reads start.
                CopyMem (
                    Data,
                    &IpmiData[2],
                    DataLength-2 );
                *DataSize = (UINT8)DataLength-2;
                IpmiReadCommand = AMI_IPMI_SMBUS_MULTI_READ_MIDDLE_CMD;
            } else {
                // Working single reads start.
                CopyMem (
                    Data,
                    IpmiData,
                    DataLength );
                *DataSize = (UINT8)DataLength;
                break;
            }
        } else {
            if(IpmiData[0] == 0xFF) {
                // Working multi-part reads end.
                CopyMem (
                    &Data[*DataSize],
                    &IpmiData[1],
                    DataLength-1 );
                *DataSize += (UINT8)DataLength-1;
                break;
            } else {
                // Working multi-part reads middle.
                CopyMem (
                    &Data[*DataSize],
                    &IpmiData[1],
                    DataLength-1 );
                *DataSize += (UINT8)DataLength-1;
                IpmiReadCommand = AMI_IPMI_SMBUS_MULTI_READ_MIDDLE_CMD;
            }
        }
    }

    return Status;
}

/** @internal
    SSIF interface Ipmi send command Implementation

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
    @retval EFI_BUFFER_TOO_SMALL    Buffer is too small to update
                                    response data.
    @retval EFI_SUCCESS             Command sent successfully.
    @return Others                  Error status returned from BMC while
                                    executing the command.

**/
EFI_STATUS
AmiIpmiSsifSendCommand (
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
    UINT8           CmdDataBuffer[AMI_IPMI_MAX_SSIF_CMD_DATA_SIZE];

    if (InterfaceType != AmiSysInterfaceSsif) {
        return EFI_UNSUPPORTED;
    }

    if (Interface->Ssif.InterfaceState != AmiIpmiInterfaceInitialized) {
        return EFI_NOT_READY;
    }

    if (!ResponseDataSize || (!ResponseData && *ResponseDataSize)) {
        return EFI_INVALID_PARAMETER;
    }

    if (AmiIpmiIsIpmiTransportlocked (&Interface->Ssif.SsifTransportLocked)) {
        return EFI_ACCESS_DENIED;
    } else {
        AmiIpmiTransportAcquireLock (&Interface->Ssif.SsifTransportLocked);
    }

    /* Check the SSIF interface multi-part reads/writes supported.
       Block length include Command data, NetFn, and Command parameter.*/
    if (((Interface->Ssif.RwSupport == AmiSsifSinglePartRw) && \
         ((CommandDataSize + 2) > AMI_IPMI_SMBUS_BLOCK_LENGTH)) || \
        ((Interface->Ssif.RwSupport == AmiSsifMultiPartRw) && \
         ((CommandDataSize + 2) > (2*AMI_IPMI_SMBUS_BLOCK_LENGTH))) || \
        ((Interface->Ssif.RwSupport == AmiSsifMultiPartRwWithMiddle) && \
         ((CommandDataSize + 2) > AMI_IPMI_MAX_SSIF_CMD_DATA_SIZE))) {
        AmiIpmiTransportReleaseLock (&Interface->Ssif.SsifTransportLocked);
        return EFI_INVALID_PARAMETER;
    }

    if (Interface->Ssif.SsifSoftErrorCount >= MAX_BMC_CMD_FAIL_COUNT) {
        AmiIpmiTransportReleaseLock (&Interface->Ssif.SsifTransportLocked);
        return EFI_NOT_READY;
    }

    CmdDataBuffer[0] = (UINT8) ((NetFunction << 2) | (Lun & 0x03));
    CmdDataBuffer[1] = Command;

    if (CommandDataSize > 0) {
        if (CommandData == NULL) {
            AmiIpmiTransportReleaseLock (&Interface->Ssif.SsifTransportLocked);
            return EFI_INVALID_PARAMETER;
        }
        if (CommandDataSize <= (AMI_IPMI_MAX_SSIF_CMD_DATA_SIZE - 2) ) {
            CopyMem (
                &CmdDataBuffer[2],
                CommandData,
                CommandDataSize );
        } else {
            AmiIpmiTransportReleaseLock (&Interface->Ssif.SsifTransportLocked);
            return EFI_BAD_BUFFER_SIZE;
        }
    }

    Status = SendDataToSsifBmcPort (
                        Interface,
                        Context,
                        CmdDataBuffer,
                        (UINT8)(CommandDataSize + 2));
        
    if (Status != EFI_SUCCESS) {
        Interface->Ssif.SsifSoftErrorCount++;
        AmiIpmiTransportReleaseLock (&Interface->Ssif.SsifTransportLocked);
        return Status;
    }

    // Hook to check smbus alert pin.
    if (Interface->Ssif.SmbAlertSupport) {
        if (!CheckAlertPinHook()) {
            AmiIpmiTransportReleaseLock (&Interface->Ssif.SsifTransportLocked);
            return EFI_DEVICE_ERROR;
        }
    } else {
        MicroSecondDelay(IPMI_SSIF_WAIT_RESPONSE_DATA_DELAY);
    }

    DataSize = AMI_IPMI_SMBUS_BLOCK_LENGTH;

    Status = ReceiveBmcDataFromSsifPort (
                        Interface,
                        Context,
                        CmdDataBuffer,
                        &DataSize);
        
    if (Status != EFI_SUCCESS) {
        Interface->Ssif.SsifSoftErrorCount++;
        AmiIpmiTransportReleaseLock (&Interface->Ssif.SsifTransportLocked);
        return Status;
    }

    if (CompletionCode != NULL) {
        *CompletionCode = CmdDataBuffer[2];
    }

    if (AMI_IPMI_ERROR_COMPLETION_CODE(CmdDataBuffer[2])) {
        AmiIpmiUpdateSoftErrorCount (
            CmdDataBuffer[2],
            Interface,
            InterfaceType );
        // Write completion code into return buffer if ipmi command returns an error.
        if (*ResponseDataSize) {
            if (ResponseData) {
                *ResponseData = CmdDataBuffer[2];
            }
            *ResponseDataSize = 1;
        }
        AmiIpmiTransportReleaseLock (&Interface->Ssif.SsifTransportLocked);
        return EFI_DEVICE_ERROR;
    }

    if (DataSize < 3) {
        AmiIpmiTransportReleaseLock (&Interface->Ssif.SsifTransportLocked);
        return EFI_DEVICE_ERROR;
    }

    if ((DataSize - 2) > *((UINT8 *)ResponseDataSize)) {
        *ResponseDataSize = (UINT8) (DataSize - 3);
        AmiIpmiTransportReleaseLock (&Interface->Ssif.SsifTransportLocked);
        return EFI_BUFFER_TOO_SMALL;
    }

    // Copying the response data into ResponseData buffer.
    CopyMem (
        ResponseData,
        &CmdDataBuffer[2],
        (DataSize - 2) );
    *ResponseDataSize = (UINT8) (DataSize - 2);

    AmiIpmiTransportReleaseLock (&Interface->Ssif.SsifTransportLocked);
    return EFI_SUCCESS;
}

/**
    Execute the Get System Interface Capability command and update the RwSupport
    and PecSupport of Ipmi Instance.

    @param IpmiInstance     Ipmi Instance Data structure pointer.

    @return EFI_STATUS       Return Status

**/
VOID
GetSystemInterfaceCapability (
  IN  AMI_IPMI_BMC_INSTANCE_DATA  *IpmiInstance 
  )
{
    EFI_STATUS                                      Status;
    AMI_IPMI_GET_SYSTEM_INTERFACE_CAPABILITY_REQ    GetSystemInterfaceCapabilityCmd;
    AMI_IPMI_GET_SYSTEM_INTERFACE_CAPABILITY_RES    GetSsifInterfaceCapability;
    UINT8                                           DataSize = sizeof (GetSsifInterfaceCapability);

    GetSystemInterfaceCapabilityCmd.SystemInterfaceType = 0x0; // SSIF
    GetSystemInterfaceCapabilityCmd.Reserved = 0x0;

    Status = IpmiInstance->IpmiTransport.SendIpmiCommand (
                                            &IpmiInstance->IpmiTransport,
                                            IPMI_NETFN_APP,
                                            AMI_BMC_LUN,
                                            IPMI_APP_GET_SYSTEM_INTERFACE_CAPABILITIES,
                                            (UINT8*) &GetSystemInterfaceCapabilityCmd,
                                            sizeof (GetSystemInterfaceCapabilityCmd),
                                            (UINT8*) &GetSsifInterfaceCapability,
                                            &DataSize);

    DEBUG ((DEBUG_INFO, "%a Status %r\n", __FUNCTION__, Status));

    if (!EFI_ERROR (Status)) {
        IpmiInstance->Interface.Ssif.RwSupport = GetSsifInterfaceCapability.TransactionSupport;
        IpmiInstance->Interface.Ssif.PecSupport = GetSsifInterfaceCapability.PecSupport;
    }
}

/**
    Execute the Get Global Enable command to get receive message queue interrupt.

    @return VOID

**/
VOID
GetGlobalEnables (
  IN OUT AMI_IPMI_BMC_INSTANCE_DATA  *IpmiInstance 
)
{
    EFI_STATUS                          Status;
    AMI_IPMI_BMC_GLOBAL_ENABLES_RES     BmcGlobalEnables;
    UINT32                              ResponseDataSize = sizeof (BmcGlobalEnables);

    //
    // Get Global Enable Information.
    //
    Status = IpmiInstance->IpmiTransport.SendIpmiCommand(
                                            &IpmiInstance->IpmiTransport,
                                            IPMI_NETFN_APP,
                                            AMI_BMC_LUN,
                                            IPMI_APP_GET_BMC_GLOBAL_ENABLES,
                                            NULL,
                                            0,
                                            (UINT8 *) (&BmcGlobalEnables),
                                            (UINT8 *) &ResponseDataSize
                                            );

    DEBUG ((DEBUG_INFO, "Get BMC Global Enable status %r\n", Status));
    if (!EFI_ERROR(Status)) {
        //
        // Set Smb alert pin based on ReceiveMsgQueueInterrupt bit
        //
        IpmiInstance->Interface.Ssif.SmbAlertSupport = BmcGlobalEnables.ReceiveMsgQueueInterrupt;
    }
}
/** @internal
    Initialize SSIF interface specific data.

    @param[in, out]  Interface  System interface pointer.

    @retval EFI_SUCCESS     Interface is successfully initialized.
    @retval Others          Error status while initializing interface.

**/
EFI_STATUS
AmiInitSsifInterfaceData (
  IN OUT AMI_SYSTEM_INTERFACE   *Interface )
{
    EFI_STATUS  Status = EFI_SUCCESS;
    
    if (Interface->Ssif.InterfaceState == AmiIpmiInterfaceInitialized) {
        return Status;
    }

    Interface->Ssif.SsifRetryCounter    = IPMI_SSIF_COMMAND_REQUEST_RETRY_COUNTER;
    Interface->Ssif.PecSupport          = FALSE;
    Interface->Ssif.RwSupport           = 0x0; // SSIF multi-part reads/writes support.
    Interface->Ssif.SmbAlertSupport     = FALSE; // SMB alert pin support.

    Status = AmiIpmiGetSmbusApiPtr (Interface);

    if (EFI_ERROR(Status)) {
        Interface->Ssif.InterfaceState = AmiIpmiInterfaceInitError;
        return Status;
    }

    AmiIpmiTransportReleaseLock (&Interface->Ssif.SsifTransportLocked);
    
    Interface->Ssif.InterfaceState = AmiIpmiInterfaceInitialized;
    
    DEBUG ((DEBUG_INFO, "%a Initialized Status = %r\n", __FUNCTION__, Status));
    
    return Status;
}
