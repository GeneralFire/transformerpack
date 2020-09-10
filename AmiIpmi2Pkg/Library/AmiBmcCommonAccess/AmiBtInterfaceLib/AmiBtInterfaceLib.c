//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file AmiBtInterfaceLib.c
    BT Transport implementation library functions.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <Uefi/UefiBaseType.h>
#include <AmiIpmiInitHooks.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmiBmcCommonInterfaceLib.h>
#include <Library/AmiBtInterfaceLib.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define BT_DELAY_COUNTER    Interface->Bt.BtRetryCount

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

extern AMI_OEM_INIT_BT_INTERFACE_DATA_HOOK OEM_INIT_BT_INTERFACE_DATA_HOOK_LIST \
                                                EndOfOemInitBtInterfaceDataHookList;
AMI_OEM_INIT_BT_INTERFACE_DATA_HOOK *gOemInitBtInterfaceDataHookList[] = { \
                                        OEM_INIT_BT_INTERFACE_DATA_HOOK_LIST \
                                        NULL \
                                        };

//---------------------------------------------------------------------------

/** @internal
    OEM hook to initializes HOST BMC BT interface data.

    @param[in] MmioBaseAddress      Pointer to the MMIO base address.
    @param[in] BaseAddressRange     Pointer to the base address range.

    @return EFI_STATUS  Return status of the Hook.

**/
EFI_STATUS
OemInitBtInterfaceDataHook (
  IN UINTN  *MmioBaseAddress,
  IN UINTN  *BaseAddressRange )
{
    EFI_STATUS  Status = EFI_SUCCESS;
    UINTN       Index;
   
    for (Index = 0; gOemInitBtInterfaceDataHookList[Index]; Index++) {
        Status = gOemInitBtInterfaceDataHookList[Index] (
                    MmioBaseAddress,
                    BaseAddressRange );
    }
    return Status;
}

/** @internal
    Get the BT interface port addresses based on access type.

    @param[in]  Interface           Pointer to System interface.
    @param[out] BtCtrlPort          Pointer to Bt control port.
    @param[out] BtComBufferPort     Pointer to Bt communication buffer port.
    
    @return VOID    Nothing.

**/
VOID
GetBtPortAddresses (
  IN  AMI_SYSTEM_INTERFACE  *Interface,
  OUT UINTN                 *BtCtrlPort,
  OUT UINTN                 *BtComBufferPort )
{
    // Update Bt Ports based on Interface AccessType.
    if (Interface->Bt.AccessType == AmiIpmiIoAccess) {
        *BtCtrlPort      = (UINTN) Interface->Bt.CtrlPort;
        *BtComBufferPort = (UINTN) Interface->Bt.ComBuffer;
    } else {
        *BtCtrlPort      = Interface->Bt.MmioBaseAddress;
        *BtComBufferPort = *BtCtrlPort + Interface->Bt.BaseAddressRange;
    }
}

/** @internal
    Sends the command to BT interface BMC port.

    @param[in] Interface    Pointer to System interface.
    @param[in] Context      NULL here.
    @param[in] Data         Pointer to command data that will be sent to BMC
                            along with Command.
    @param[in] DataSize     Size of the command data.

    @retval EFI_NOT_READY   Interface is not ready to send data.
    @retval EFI_SUCCESS     Command sent to BMC successfully.

**/
EFI_STATUS
SendDataToBtBmcPort (
  IN AMI_SYSTEM_INTERFACE   *Interface,
  IN VOID                   *Context,
  IN UINT8                  *Data,
  IN UINT8                  DataSize )
{
    UINT8                   BtCntlData;
    UINT8                   Index;
    UINT32                  Retry;
    UINTN                   BtCtrlPort;
    UINTN                   BtComBufferPort;
    AMI_IPMI_ACCESS_TYPE    AccessType;
    UINT8                   TempDataSize;
    BOOLEAN                 MultipleDataSend = FALSE;

    AccessType = Interface->Bt.AccessType;

    // Get Bt Ports addresses.
    GetBtPortAddresses (
        Interface,
        &BtCtrlPort,
        &BtComBufferPort );
    do {
        /* Wait for B_BUSY bit to clear (BMC ready to accept a request).
               Default delay for each retry is 15 micro seconds.*/
        for (Retry = 0; Retry < BT_DELAY_COUNTER; Retry++) {
            BtCntlData = AmiIpmiBmcRead8 (
                            AccessType,
                            BtCtrlPort );
            if (!(BtCntlData & AMI_IPMI_B_BUSY_BIT)) {
                break;
            }
            MicroSecondDelay (IPMI_BT_DELAY_PER_RETRY);
        }
        if (Retry == BT_DELAY_COUNTER) {
            return EFI_TIMEOUT;
        }

        // Wait for H2B_ATN bit to clear (Acknowledgment of previous commands).
        for (Retry = 0; Retry < BT_DELAY_COUNTER; Retry++) {
            BtCntlData = AmiIpmiBmcRead8 (
                            AccessType,
                            BtCtrlPort );
            if (!(BtCntlData & AMI_IPMI_H2B_ATN_BIT)) {
                break;
            }
            MicroSecondDelay (IPMI_BT_DELAY_PER_RETRY);
        }
        if (Retry == BT_DELAY_COUNTER) {
            return EFI_TIMEOUT;
        }

        // Set CLR_WR_PTR.
        BtCntlData = AMI_IPMI_CLR_WR_PTR_BIT;
        AmiIpmiBmcWrite8 (
                AccessType,
                BtCtrlPort,
                BtCntlData );

        if (DataSize > Interface->Bt.HosttoBmcBufferSize ){
            TempDataSize = Interface->Bt.HosttoBmcBufferSize;
            MultipleDataSend = TRUE;
        } else {
            TempDataSize = DataSize;
            MultipleDataSend = FALSE;
        }
        // Send each message byte out (write data to HOST2BMC buffer).
        for (Index = 0; Index < TempDataSize; Index++) {  // Send the message.
            AmiIpmiBmcWrite8 (
                    AccessType,
                    BtComBufferPort,
                    *(Data + Index) );
        }

        // Set H2B_ATN bit to inform BMC that data is available.
        BtCntlData = AMI_IPMI_H2B_ATN_BIT;
        AmiIpmiBmcWrite8 (
                AccessType,
                BtCtrlPort,
                BtCntlData );
        //Command data size greater than available Input buffer size.
        if (MultipleDataSend){
            Data = Data + TempDataSize;
            DataSize -= TempDataSize;
            for (Retry = 0; Retry < BT_DELAY_COUNTER; Retry++) {
                BtCntlData = AmiIpmiBmcRead8 (
                                AccessType,
                                BtCtrlPort );
                if ((BtCntlData & AMI_IPMI_B_BUSY_BIT)) {
                    break;
                }
                MicroSecondDelay (15);  // 15 microsecond delay.
            }
            if (Retry == BT_DELAY_COUNTER) {
                return EFI_TIMEOUT;
            }
        }
    } while (MultipleDataSend);


    return EFI_SUCCESS;
}

/** @internal
    Receives the Data from BT interface BMC port.

    @param[in]  Interface   Pointer to System interface.
    @param[in]  Context     NULL here.
    @param[out] Data        Pointer to response data that is received from BMC.
    @param[out] DataSize    Size of the response data.

    @retval EFI_NOT_READY           Interface is not ready to receive data.
    @retval EFI_SUCCESS             Data received from BMC successfully.
    @retval EFI_INVALID_PARAMETER   Invalid parameter.

**/
EFI_STATUS
ReceiveBmcDataFromBtPort (
  IN  AMI_SYSTEM_INTERFACE  *Interface,
  IN  VOID                  *Context,
  OUT UINT8                 *Data,
  OUT UINT8                 *DataSize )
{
    UINT8                   BtCntlData;
    UINT8                   Length = 0;
    UINT8                   TempDataSize;
    UINT8                   Index;
    UINT32                  Retry;
    UINTN                   BtCtrlPort;
    UINTN                   BtComBufferPort;
    AMI_IPMI_ACCESS_TYPE    AccessType;
    BOOLEAN                 MultipleDataReceive = FALSE;

    AccessType = Interface->Bt.AccessType;

    // Get Bt Ports addresses.
    GetBtPortAddresses (
        Interface,
        &BtCtrlPort,
        &BtComBufferPort );
    do {
        /* Wait for B2H_ATN bit to be set,signaling data is available for host.
           Default delay for each retry is 15 micro seconds.*/
        for (Retry = 0; Retry < BT_DELAY_COUNTER; Retry++) {
            BtCntlData = AmiIpmiBmcRead8 (
                            AccessType,
                            BtCtrlPort );
            if (BtCntlData & AMI_IPMI_B2H_ATN_BIT) {
                break;
            }
            MicroSecondDelay (IPMI_BT_DELAY_PER_RETRY);
        }
        if (Retry == BT_DELAY_COUNTER) {
            return EFI_TIMEOUT;
        }

        // Set H_BUSY bit, indicating host is in process of reading data from interface.
        BtCntlData = AmiIpmiBmcRead8 (
                        AccessType,
                        BtCtrlPort );
        if (!(BtCntlData & AMI_IPMI_H_BUSY )) {
            BtCntlData = AMI_IPMI_H_BUSY;        // most bits are rw1c, so clear them.
            AmiIpmiBmcWrite8 (
                AccessType,
                BtCtrlPort,
                BtCntlData );
        }

        // Clear B2H_ATN bit,to acknowledge receipt of message response.
        BtCntlData = AMI_IPMI_B2H_ATN_BIT;     // Most bits are rw1c, so clear them.
        AmiIpmiBmcWrite8 (
            AccessType,
            BtCtrlPort,
            BtCntlData );

        // Set CLR_RD_PTR bit.
        BtCntlData = AMI_IPMI_CLR_RD_PTR_BIT;  // Most bits are rw1c, so clear them.
        AmiIpmiBmcWrite8 (
            AccessType,
            BtCtrlPort,
            BtCntlData );

        if (!Length){
            // Read the data bytes from BMC.
            Length = AmiIpmiBmcRead8 (
                        AccessType,
                        BtComBufferPort );
            if (Length == 0x00) {
                return EFI_INVALID_PARAMETER;
            }
            AmiIpmiBmcWrite8 (
                        AccessType,
                        BtCtrlPort,
                        BtCntlData );
            *DataSize = Length;
            //Increment Length to include length field
            Length++;
        }
        if (Length > Interface->Bt.BmctoHostBufferSize) {
            TempDataSize = Interface->Bt.BmctoHostBufferSize;
            MultipleDataReceive = TRUE;
        } else {
            TempDataSize = Length;
            MultipleDataReceive = FALSE;
        }

        for (Index = 0; Index < TempDataSize; Index++) {
            *(Data + Index) = AmiIpmiBmcRead8 (
                                AccessType,
                                BtComBufferPort );
        }

        // Clear H_BUSY bit indicating host is done reading data from BMC.
        BtCntlData = AmiIpmiBmcRead8 (
                        AccessType,
                        BtCtrlPort );
        if (BtCntlData & AMI_IPMI_H_BUSY) {
            BtCntlData = AMI_IPMI_H_BUSY;        // Most bits are rw1c, so clear them.
            AmiIpmiBmcWrite8 (
                AccessType,
                BtCtrlPort,
                BtCntlData );
        }

        if (MultipleDataReceive) {
            Data = Data + TempDataSize;
            Length -= TempDataSize;
        }
    } while (MultipleDataReceive);

    return EFI_SUCCESS;
}

/** @internal
    BT interface send command implementation.

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
AmiIpmiBtSendCommand (
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
    UINT8       DataSize;
    EFI_STATUS  Status;
    UINT8       Seq = 0;
    UINT8       CmdDataBuffer[AMI_IPMI_MAX_BT_CMD_DATA_SIZE];

    if (InterfaceType != AmiSysInterfaceBt) {
        return EFI_UNSUPPORTED;
    }

    if (Interface->Bt.InterfaceState != AmiIpmiInterfaceInitialized) {
        return EFI_NOT_READY;
    }

    if (!ResponseDataSize || (!ResponseData && *ResponseDataSize)) {
        return EFI_INVALID_PARAMETER;
    }

    if (AmiIpmiIsIpmiTransportlocked (&Interface->Bt.BtTransportLocked)) {
        return EFI_ACCESS_DENIED;
    } else {
        AmiIpmiTransportAcquireLock (&Interface->Bt.BtTransportLocked);
    }

    CmdDataBuffer[0] = (UINT8)CommandDataSize + 0x03;  // NetFn+Cmd+Seq+Data
    CmdDataBuffer[1] = (UINT8) ((NetFunction << 2) | (Lun & 0xfc));
    CmdDataBuffer[2] = Seq;
    CmdDataBuffer[3] = Command;

    if (CommandDataSize > 0) {
        if (CommandData == NULL) {
            AmiIpmiTransportReleaseLock (&Interface->Bt.BtTransportLocked);
            return EFI_INVALID_PARAMETER;
        }
        if (CommandDataSize <= (AMI_IPMI_MAX_BT_CMD_DATA_SIZE - 4) ) {
            CopyMem (
                &CmdDataBuffer[4],
                CommandData,
                CommandDataSize );
        } else {
            AmiIpmiTransportReleaseLock (&Interface->Bt.BtTransportLocked);
            return EFI_BAD_BUFFER_SIZE;
        }
    }

    if (COMMAND_SPECIFIC_COUNTER_UPDATE_SUPPORT) {
        // Update Command Specific Delay Counter.
        Interface->Bt.BtRetryCount = AmiIpmiUpdateCommandSpecificDelay (
                                    NetFunction,
                                    Command );
        DEBUG((DEBUG_INFO, "Counter value is %x\n", Interface->Bt.BtRetryCount));
    }

    Status = SendDataToBtBmcPort (
                        Interface,
                        Context,
                        CmdDataBuffer,
                        (UINT8)(CommandDataSize + 4));
        
    if (Status != EFI_SUCCESS) {
        Interface->Bt.BtSoftErrorCount++;
        AmiIpmiTransportReleaseLock (&Interface->Bt.BtTransportLocked);
        return Status;
    }
    
    DataSize = AMI_IPMI_MAX_BT_CMD_DATA_SIZE;
    
    Status = ReceiveBmcDataFromBtPort (
                        Interface,
                        Context,
                        CmdDataBuffer,
                        &DataSize);
    
    if (Status != EFI_SUCCESS) {
        Interface->Bt.BtSoftErrorCount++;
        AmiIpmiTransportReleaseLock (&Interface->Bt.BtTransportLocked);
        return Status;
    }

    if (CompletionCode != NULL) {
        *CompletionCode = CmdDataBuffer[4];
    }

    if (AMI_IPMI_ERROR_COMPLETION_CODE(CmdDataBuffer[4])) {
        AmiIpmiUpdateSoftErrorCount (
            CmdDataBuffer[4],
            Interface,
            InterfaceType );
        // Write completion code into return buffer if ipmi command returns an error.
        if (*ResponseDataSize) {
            if (ResponseData) {
                *ResponseData = CmdDataBuffer[4];
            }
            *ResponseDataSize = 1;
        }
        AmiIpmiTransportReleaseLock (&Interface->Bt.BtTransportLocked);
        return EFI_DEVICE_ERROR;
    }

    if (DataSize < 4) {
        AmiIpmiTransportReleaseLock (&Interface->Bt.BtTransportLocked);
        return EFI_DEVICE_ERROR;
    }

    if ((DataSize - 3) > *((UINT8 *)ResponseDataSize)) {
        *ResponseDataSize = (UINT8) (DataSize - 3);
        AmiIpmiTransportReleaseLock (&Interface->Bt.BtTransportLocked);
        return EFI_BUFFER_TOO_SMALL;
    }

    // Copying the response data into ResponseData buffer.
    CopyMem (
        ResponseData,
        &CmdDataBuffer[4],
        (DataSize - 3) );
    *ResponseDataSize = (UINT8) (DataSize - 3);

    AmiIpmiTransportReleaseLock (&Interface->Bt.BtTransportLocked);
    return EFI_SUCCESS;
}

/**
    Execute the Get BT Interface Capability command and update the input 
    and output buffer value of Ipmi Instance.

    @param IpmiInstance Data structure pointer.

    @retval  VOID
**/

VOID
GetBtInterfaceCapability (
  IN OUT   AMI_IPMI_BMC_INSTANCE_DATA      *IpmiInstance 
)
{
    EFI_STATUS                           Status;
    AMI_IPMI_BT_INTERFACE_CAPABILITY_RES Responsedata;
    UINT8                                ResponseSize = sizeof(AMI_IPMI_BT_INTERFACE_CAPABILITY_RES);

    Status = IpmiInstance->IpmiTransport.SendIpmiCommand (
                                            &IpmiInstance->IpmiTransport,
                                            IPMI_NETFN_APP,
                                            AMI_BMC_LUN,
                                            IPMI_APP_GET_BT_INTERFACE_CAPABILITY,
                                            NULL,
                                            0,
                                            (UINT8*) &Responsedata,
                                            &ResponseSize );

    if(EFI_ERROR(Status) || Responsedata.CompletionCode){
       DEBUG ((DEBUG_ERROR, " IPMI_APP_GET_BT_INTERFACE_CAPABILITY Status: %r Completion code: %x\n", Status, Responsedata.CompletionCode));
       return;
    }
    IpmiInstance->Interface.Bt.HosttoBmcBufferSize = Responsedata.InputBuffSize;
    IpmiInstance->Interface.Bt.BmctoHostBufferSize = Responsedata.OutputBuffSize;

    DEBUG ((DEBUG_ERROR, " InputBuffSize:%x OutBuffSize%x  BtRetry %x Status %r \n",IpmiInstance->Interface.Bt.HosttoBmcBufferSize,IpmiInstance->Interface.Bt.BmctoHostBufferSize,Responsedata.RecommandedRetires, Status));

    return;
}

/** @internal
    Initialize BT interface specific data.

    @param[in, out]  Interface  System interface pointer.

    @retval EFI_SUCCESS     Interface is successfully initialized.
    @retval Others          Return status while initializing interface.

**/
EFI_STATUS
AmiInitBtInterfaceData (
  IN OUT AMI_SYSTEM_INTERFACE   *Interface )
{
    EFI_STATUS  Status = EFI_SUCCESS;
    AMI_IPMI_BMC_INSTANCE_DATA  *IpmiInstance;

    if (Interface->Bt.InterfaceState == AmiIpmiInterfaceInitialized) {
        return Status;
    }

    Interface->Bt.CtrlPort              = IPMI_BT_CTRL_PORT;     // BT Control Port
    Interface->Bt.ComBuffer             = IPMI_BT_BUFFER_PORT; // BT Buffer Port
    Interface->Bt.IntMaskPort           = IPMI_BT_INTERRUPT_MASK_PORT; // BT IntMask Port
    Interface->Bt.BtRetryCount          = IPMI_BT_COMMAND_PORT_READ_RETRY_COUNTER; // BT retry count 
    Interface->Bt.HosttoBmcBufferSize   = IPMI_BT_BUFFER_SIZE;// Host to Bmc Buffer Size.
    Interface->Bt.BmctoHostBufferSize   = IPMI_BT_BUFFER_SIZE;// Bmc to Host Buffer Size.

    if (IPMI_DEFAULT_ACCESS_TYPE) {
        Interface->Bt.AccessType        = AmiIpmiIoAccess;
        Interface->Bt.MmioBaseAddress   = 0;
        Interface->Bt.BaseAddressRange  = 0;
    } else {
        Interface->Bt.AccessType        = AmiIpmiMmioAccess;
        Status = OemInitBtInterfaceDataHook (
                    &Interface->Bt.MmioBaseAddress,
                    &Interface->Bt.BaseAddressRange );
        DEBUG ((DEBUG_INFO, "OemInitBtInterfaceDataHook Status is %r.\n", Status));
    }

    AmiIpmiTransportReleaseLock (&Interface->Bt.BtTransportLocked);

    Interface->Bt.InterfaceState = AmiIpmiInterfaceInitialized;

    IpmiInstance = AMI_IPMI_INSTANCE_FROM_INTERFACE_THIS (Interface);
    if (IpmiInstance->Signature != AMI_SM_IPMI_BMC_SIGNATURE) {
        return EFI_NOT_FOUND;
    }
    GetBtInterfaceCapability (IpmiInstance);
    DEBUG ((DEBUG_INFO, "%a Initialized Status = %r\n", __FUNCTION__, Status));

    return Status;
}
