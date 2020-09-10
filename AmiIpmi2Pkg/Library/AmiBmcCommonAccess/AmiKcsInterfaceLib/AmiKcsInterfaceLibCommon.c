//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file AmiKcsInterfaceLibCommon.c
    KCS Transport Implementation library common functions, variables and macros.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <AmiIpmiInitHooks.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmiBmcCommonInterfaceLib.h>
#include <Library/AmiKcsInterfaceLib.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define KCS_RETRY_COUNTER    Interface->Kcs->KcsRetryCounter

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

extern AMI_OEM_INIT_KCS_INTERFACE_DATA_HOOK OEM_INIT_KCS_INTERFACE_DATA_HOOK_LIST \
                                                EndOfOemInitKcsInterfaceDataHookList;
AMI_OEM_INIT_KCS_INTERFACE_DATA_HOOK    *gOemInitKcsInterfaceDataHookList[] = { \
                                            OEM_INIT_KCS_INTERFACE_DATA_HOOK_LIST \
                                            NULL \
                                            };
//---------------------------------------------------------------------------

/** @internal
    OEM hook to initialize HOST BMC KCS interface data.

    @param[in] MmioBaseAddress      Pointer to the MMIO base address.
    @param[in] BaseAddressRange     Pointer to the base address range.

    @return EFI_STATUS  Return status of the Hook.

**/
EFI_STATUS
OemInitKcsInterfaceDataHook (
  IN UINTN  *MmioBaseAddress,
  IN UINTN  *BaseAddressRange )
{
    EFI_STATUS  Status;
    UINTN       Index;
    
    for (Index = 0; gOemInitKcsInterfaceDataHookList[Index]; Index++) {
        Status = gOemInitKcsInterfaceDataHookList[Index] (
                    MmioBaseAddress,
                    BaseAddressRange );
    }
    return Status;
}

/** @internal
    KCS error exit function implementation based on the Flow chart given in
    IPMI specification.

    @param[in]  Interface   Pointer to System interface.
    @param[in]  Context     NULL here.

    @retval EFI_DEVICE_ERROR    Device error or retry count exceeded.
    @retval EFI_ABORTED         Bmc is in idle.

**/
EFI_STATUS
KcsErrorExit (
  IN AMI_SYSTEM_INTERFACE   *Interface,
  IN VOID                   *Context )
{
    UINT8                   KcsData;
    AMI_IPMI_KCS_STATUS     KcsStatus;
    UINT8                   RetryCount;
    UINT32                  Counter;
    UINTN                   KcsDataPort;
    UINTN                   KcsCommandPort;
    AMI_IPMI_ACCESS_TYPE    AccessType;
    
    AccessType = Interface->Kcs->AccessType;

    // Update KcsDataPort and KcsCommandPort based on Interface AccessType.
    if (AccessType == AmiIpmiIoAccess) {
        KcsDataPort = (UINTN) Interface->Kcs->DataPort;
        KcsCommandPort = (UINTN) Interface->Kcs->CommandPort;
    } else {
        KcsDataPort = Interface->Kcs->MmioBaseAddress;
        KcsCommandPort = KcsDataPort + Interface->Kcs->BaseAddressRange;
    }

    /* Flow chart: Aborting KCS Transactions in-progress and/or Retrieving
       KCS Error Status. Error exit Flow chart: Check for RETRY LIMIT.
       Default delay is 100 microseconds for each retry.*/
    RetryCount = 0;
    while (RetryCount < AMI_IPMI_KCS_ABORT_RETRY_COUNT) {

        // Error exit Flow chart: wait for IBF=0.
        Counter = KCS_RETRY_COUNTER;
        while (TRUE) {
            KcsStatus.RawData = AmiIpmiBmcRead8 (
                                    AccessType,
                                    KcsCommandPort );
            if (!KcsStatus.Status.Ibf) {
                break;
            }
            if (Counter-- == 0) {
                return EFI_TIMEOUT;
            }
            MicroSecondDelay (IPMI_KCS_DELAY_PER_RETRY);
        }

        // Error exit Flow chart: GET_STATUS/ABORT to CMD; phase = error1.
        KcsData = AMI_IPMI_KCS_ABORT;
        AmiIpmiBmcWrite8 (
            AccessType,
            KcsCommandPort,
            KcsData );

        // Error exit Flow chart: wait for IBF=0.
        Counter = KCS_RETRY_COUNTER;
        while (TRUE) {
            KcsStatus.RawData = AmiIpmiBmcRead8 (
                                    AccessType,
                                    KcsCommandPort );
            if (!KcsStatus.Status.Ibf) {
                break;
            }
            if (Counter-- == 0) {
                return EFI_TIMEOUT;
            }
            MicroSecondDelay (IPMI_KCS_DELAY_PER_RETRY);
        }

        // Error exit Flow chart: clear OBF.
        KcsStatus.RawData = AmiIpmiBmcRead8 (
                                AccessType,
                                KcsDataPort );

        // Error exit Flow chart: 00h to DATA_IN; phase = error2.
        KcsData = 0x0;
        AmiIpmiBmcWrite8 (
            AccessType,
            KcsDataPort,
            KcsData );

        // Error exit Flow chart: wait for IBF=0.
        Counter = KCS_RETRY_COUNTER;
        while (TRUE) {
            KcsStatus.RawData = AmiIpmiBmcRead8 (
                                    AccessType,
                                    KcsCommandPort );
            if (!KcsStatus.Status.Ibf) {
                break;
            }
            if (Counter-- == 0) {
                return EFI_TIMEOUT;
            }
            MicroSecondDelay (IPMI_KCS_DELAY_PER_RETRY);
        }

        // Check for KCS State.
        if (KcsStatus.Status.State == AmiIpmiKcsReadState) {

            // Error exit Flow chart: KCS is in READ_STEATE so wait for OBF=1.
            Counter = KCS_RETRY_COUNTER;
            while (TRUE) {
                KcsStatus.RawData = AmiIpmiBmcRead8 (
                                        AccessType,
                                        KcsCommandPort );
                if (KcsStatus.Status.Obf) {
                    break;
                }
                if (Counter-- == 0) {
                    return EFI_TIMEOUT;
                }
                MicroSecondDelay (IPMI_KCS_DELAY_PER_RETRY);
            }

            // Error exit Flow chart: Read error status code byte from DATA_OUT.
            AmiIpmiBmcRead8 (
                AccessType,
                KcsDataPort );

            // Error exit Flow chart: Write READ dummy byte to DATA_IN; phase = error3.
            KcsData = AMI_IPMI_KCS_READ;
            AmiIpmiBmcWrite8 (
                AccessType,
                KcsDataPort,
                KcsData );

            // Error exit Flow chart: wait for IBF=0.
            Counter = KCS_RETRY_COUNTER;
            while (TRUE) {
                KcsStatus.RawData = AmiIpmiBmcRead8 (
                                        AccessType,
                                        KcsCommandPort );
                if (!KcsStatus.Status.Ibf) {
                    break;
                }
                if (Counter-- == 0) {
                    return EFI_TIMEOUT;
                }
                MicroSecondDelay (IPMI_KCS_DELAY_PER_RETRY);
            }

            // Check for KCS State.
            if (KcsStatus.Status.State == AmiIpmiKcsIdleState) {

                // Error exit Flow chart: KCS is in IDLE_STEATE so wait for OBF=1.
                Counter = KCS_RETRY_COUNTER;
                while (TRUE) {
                    KcsStatus.RawData = AmiIpmiBmcRead8 (
                                            AccessType,
                                            KcsCommandPort );
                    if (KcsStatus.Status.Obf) {
                        break;
                    }
                    if (Counter-- == 0) {
                        return EFI_TIMEOUT;
                    }
                    MicroSecondDelay (IPMI_KCS_DELAY_PER_RETRY);  // Default delay is 100 microseconds.
                }

                // Error exit Flow chart: clear OBF.
                KcsStatus.RawData = AmiIpmiBmcRead8 (
                                        AccessType,
                                        KcsDataPort );

                // Error exit Flow chart: phase = idle return EFI_ABORTED.
                return EFI_ABORTED;
            } else {
                /* Error exit Flow chart: KCS is not in IDLE_STATE so increment retry count
                   and continue.*/
                RetryCount++;
                continue;
            }

        } else {
            /* Error exit Flow chart: KCS is not in READ_STATE so increment retry count
               and continue.*/
            RetryCount++;
            continue;
        }
    } // While loop for Retry count.

    /* Control comes here only when retry limit is exceeded so
       Error exit Flow chart: return Communication Failure.*/
    return EFI_DEVICE_ERROR;
}

/** @internal
    Checks for the Status BMC KCS interface.

    @param[in]  Interface   Pointer to System interface.
    @param[in]  KcsState    Kcs read/write state.
    @param[out] Idle        Pointer to Bmc idle state.
    @param[in]  Context     NULL here.

    @retval EFI_DEVICE_ERROR    Command retry count exceeded.
    @retval EFI_SUCCESS         Checked the KCS status successfully.
    @retval Others              Return status of functions used.

**/
EFI_STATUS
KcsCheckStatus (
  IN  AMI_SYSTEM_INTERFACE  *Interface,
  IN  AMI_IPMI_KCS_STATE    KcsState,
  OUT BOOLEAN               *Idle,
  IN  VOID                  *Context )
{
    AMI_IPMI_KCS_STATUS     KcsStatus;
    UINT32                  Counter;
    UINTN                   KcsDataPort;
    UINTN                   KcsCommandPort;
    AMI_IPMI_ACCESS_TYPE    AccessType;
    
    AccessType = Interface->Kcs->AccessType;

    // Update KcsDataPort and KcsCommandPort based on Interface AccessType.
    if (AccessType == AmiIpmiIoAccess) {
        KcsDataPort = (UINTN) Interface->Kcs->DataPort;
        KcsCommandPort = (UINTN) Interface->Kcs->CommandPort;
    } else {
        KcsDataPort = Interface->Kcs->MmioBaseAddress;
        KcsCommandPort = KcsDataPort + Interface->Kcs->BaseAddressRange;
    }

    *Idle = FALSE;

    /* Write/Read Transfer Flow Chart: wait for IBF=0.
       Default delay is 100 microseconds for each retry.*/
    Counter = KCS_RETRY_COUNTER;
    while (TRUE) {
        KcsStatus.RawData = AmiIpmiBmcRead8 (
                                AccessType,
                                KcsCommandPort );
        if (!KcsStatus.Status.Ibf) {
            break;
        }
        if (Counter-- == 0) {
            return EFI_TIMEOUT;
        }
        MicroSecondDelay (IPMI_KCS_DELAY_PER_RETRY);
    }

    // Write Transfer Flow Chart:
    if (KcsState == AmiIpmiKcsWriteState) {

        // Write Transfer Flow Chart: Check whether KCS is in WRITE_STATE or not.
        if (KcsStatus.Status.State == AmiIpmiKcsWriteState) {

            /* KCS is in WRITE_STATE so 
               Write Transfer Flow Chart: Clear OBF; phase=wr_data
               and return EFI_SUCCESS*/
            AmiIpmiBmcRead8 (
                AccessType,
                KcsDataPort );
            return EFI_SUCCESS;
        } else {
            // KCS is not in WRITE_STATE so Write Transfer Flow Chart: Error Exit.
            return KcsErrorExit (
                        Interface,
                        Context );
        }
    }

    // Read Transfer Flow Chart:
    if (KcsState == AmiIpmiKcsReadState) {

        /* Read Transfer Flow Chart: If KCS is neither in READ_STATE nor in
           IDLE_STATE then call error exit path.*/
        if ((KcsStatus.Status.State != AmiIpmiKcsReadState) && \
                (KcsStatus.Status.State != AmiIpmiKcsIdleState)) {
            return KcsErrorExit (
                        Interface,
                        Context );
        } else {
            // Read Transfer Flow Chart: wait for OBF=1.
            Counter = KCS_RETRY_COUNTER;
            while (TRUE) {
                KcsStatus.RawData = AmiIpmiBmcRead8 (
                                        AccessType,
                                        KcsCommandPort );
                if (KcsStatus.Status.Obf) {
                    break;
                }
                if (Counter-- == 0) {
                    return EFI_TIMEOUT;
                }
                MicroSecondDelay (IPMI_KCS_DELAY_PER_RETRY);
            }

            /*Read Transfer Flow Chart: Read dummy data byte from DATA_OUT
               If KCS is in IDLE_STATE after waiting for OBF==1 then Read dummy data
               byte from DATA_OUT.*/
            if (KcsStatus.Status.State == AmiIpmiKcsIdleState) {
                AmiIpmiBmcRead8 (
                    AccessType,
                    KcsDataPort );
                // Read Transfer Flow Chart: phase = idle.
                *Idle = TRUE;
            }
        }
    }

    return EFI_SUCCESS;
}

/** @internal
    Sends the command to KCS interface BMC port.

    @param[in]  Interface   Pointer to System interface.
    @param[in]  Context     NULL here.
    @param[in]  Data        Pointer to command data that will be sent to BMC
                            along with Command.
    @param[in]  DataSize    Size of the command data.

    @retval EFI_DEVICE_ERROR    Command retry count exceeded.
    @retval EFI_SUCCESS         Command sent to BMC successfully.
    @retval Others              Status of the Sending data to BMC port.

**/
EFI_STATUS
SendDataToBmcPort (
  IN AMI_SYSTEM_INTERFACE   *Interface,
  IN VOID                   *Context,
  IN UINT8                  *Data,
  IN UINT8                  DataSize )
{
    AMI_IPMI_KCS_STATUS     KcsStatus;
    UINT8                   KcsData;
    EFI_STATUS              Status;
    UINT8                   Index;
    BOOLEAN                 Idle;
    UINT32                  Counter;
    UINTN                   KcsDataPort;
    UINTN                   KcsCommandPort;
    AMI_IPMI_ACCESS_TYPE    AccessType;
    
    AccessType = Interface->Kcs->AccessType;
    
    // Update KcsDataPort and KcsCommandPort based on Interface AccessType.
    if (AccessType == AmiIpmiIoAccess) {
        KcsDataPort = (UINTN) Interface->Kcs->DataPort;
        KcsCommandPort = (UINTN) Interface->Kcs->CommandPort;
    } else {
        KcsDataPort = Interface->Kcs->MmioBaseAddress;
        KcsCommandPort = KcsDataPort + Interface->Kcs->BaseAddressRange;
    }

    /* Write Transfer Flow Chart: Wait for IBF=0.
       Default delay is 100 microseconds for each retry.*/
    Counter = KCS_RETRY_COUNTER;
    while (TRUE) {
        KcsStatus.RawData = AmiIpmiBmcRead8 (
                                AccessType,
                                KcsCommandPort );
        if (!(KcsStatus.Status.Ibf)) {
            break;
        }
        if (Counter-- == 0) {
            return EFI_TIMEOUT;
        }
        MicroSecondDelay (IPMI_KCS_DELAY_PER_RETRY);
    }

    // Write Transfer Flow Chart: Clear OBF.
    AmiIpmiBmcRead8 (
        AccessType,
        KcsDataPort );

    // Write Transfer Flow Chart: WR_START to CMD phase=wr_start.
    KcsData = AMI_IPMI_KCS_WRITE_START;
    AmiIpmiBmcWrite8 (
        AccessType,
        KcsCommandPort,
        KcsData );

    // Check for KCS status.
    if ((Status = KcsCheckStatus (
                    Interface,
                    AmiIpmiKcsWriteState,
                    &Idle,
                    Context )) != EFI_SUCCESS) {
        return Status;
    }

    // Send the data to BMC.
    for (Index = 0; Index < DataSize; Index++) {

        if (Index < (DataSize - 1)) {

            // Write Transfer Flow Chart: data byte to DATA.
            AmiIpmiBmcWrite8 (
                AccessType,
                KcsDataPort,
                Data[Index] );

            // Check for KCS status.
            if ((Status = KcsCheckStatus (
                            Interface,
                            AmiIpmiKcsWriteState,
                            &Idle,
                            Context )) != EFI_SUCCESS) {
                return Status;
            }
        } else {
            // Write Transfer Flow Chart: This is Last write byte so WR_END to CMD; phase=wr_end_cmd.
            KcsData = AMI_IPMI_KCS_WRITE_END;
            AmiIpmiBmcWrite8 (
                AccessType,
                KcsCommandPort,
                KcsData );

            // Check for KCS status.
            if ((Status = KcsCheckStatus (
                            Interface,
                            AmiIpmiKcsWriteState,
                            &Idle,
                            Context )) != EFI_SUCCESS) {
                return Status;
            }

            // Write Transfer Flow Chart: data byte to DATA.
            AmiIpmiBmcWrite8 (
                AccessType,
                KcsDataPort,
                Data[Index] );
        }
    }
    return EFI_SUCCESS;
}

/** @internal
    Receives the Data from KCS interface BMC port.

    @param[in]  Interface   Pointer to System interface.
    @param[in]  Context     NULL here.
    @param[out] Data        Pointer to response data that is received from BMC.
    @param[out] DataSize    Size of the response data.

    @retval EFI_SUCCESS    Data received from BMC successfully.
    @retval Others         Status of the Receiving data to BMC port.

**/
EFI_STATUS
ReceiveBmcDataFromPort (
  IN  AMI_SYSTEM_INTERFACE  *Interface,
  IN  VOID                  *Context,
  OUT UINT8                 *Data,
  OUT UINT8                 *DataSize )
{
    UINT8                   KcsData;
    EFI_STATUS              Status;
    BOOLEAN                 Idle;
    UINT8                   Count;
    UINTN                   KcsDataPort;
    AMI_IPMI_ACCESS_TYPE    AccessType;
    
    AccessType = Interface->Kcs->AccessType;
    
    // Update KcsDataPort based on Interface AccessType.
    if (AccessType == AmiIpmiIoAccess) {
        KcsDataPort = (UINTN) Interface->Kcs->DataPort;
    } else {
        KcsDataPort = Interface->Kcs->MmioBaseAddress;
    }

    Count = 0;

    while (TRUE) {

        // Check for KCS status.
        if ((Status = KcsCheckStatus (
                        Interface,
                        AmiIpmiKcsReadState,
                        &Idle,
                        Context )) != EFI_SUCCESS) {
            return Status;
        } 

        /* If KCS in IDLE_STATE that means all the data byte have been read
           so update Data size and return EFI_SUCCESS.*/
        if (Idle) {
            *DataSize = Count;
            return EFI_SUCCESS;
        }

        // Read Transfer Flow Chart: Read data byte from DATA_OUT.
        Data[Count] = AmiIpmiBmcRead8 (
                        AccessType,
                        KcsDataPort );
        Count++;

        // Read Transfer Flow Chart: write READ byte to DATA_IN.
        KcsData = AMI_IPMI_KCS_READ;
        AmiIpmiBmcWrite8 (
            AccessType,
            KcsDataPort,
            KcsData );
    }
}

/** @internal
    KCS interface send command implementation.

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
    @retval EFI_NOT_READY           Interface is not initialized or exceeded
                                    command retry count.
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
AmiIpmiKcsSendCommand (
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
    UINT8       CmdDataBuffer[AMI_IPMI_MAX_KCS_CMD_DATA_SIZE];

    if (InterfaceType != AmiSysInterfaceKcs) {
        return EFI_UNSUPPORTED;
    }

    if (Interface->Kcs->InterfaceState != AmiIpmiInterfaceInitialized) {
        return EFI_NOT_READY;
    }

    if (!ResponseDataSize || (!ResponseData && *ResponseDataSize)) {
        return EFI_INVALID_PARAMETER;
    }

    if (AmiIpmiIsIpmiTransportlocked (&Interface->Kcs->KcsTransportLocked)) {
        return EFI_ACCESS_DENIED;
    } else {
        AmiIpmiTransportAcquireLock (&Interface->Kcs->KcsTransportLocked);
    }

    if (Interface->Kcs->KcsSoftErrorCount >= MAX_BMC_CMD_FAIL_COUNT) {
        AmiIpmiTransportReleaseLock (&Interface->Kcs->KcsTransportLocked);
        return EFI_NOT_READY;
    }

    CmdDataBuffer[0] = (UINT8) ((NetFunction << 2) | (Lun & 0x03));
    CmdDataBuffer[1] = Command;
    if (CommandDataSize > 0) {
        if (CommandData == NULL) {
            AmiIpmiTransportReleaseLock (&Interface->Kcs->KcsTransportLocked);
            return EFI_INVALID_PARAMETER;
        }
        if (CommandDataSize <= (AMI_IPMI_MAX_KCS_CMD_DATA_SIZE - 2)) {
            CopyMem (
                &CmdDataBuffer[2],
                CommandData,
                CommandDataSize );
        } else {
            AmiIpmiTransportReleaseLock (&Interface->Kcs->KcsTransportLocked);
            return EFI_BAD_BUFFER_SIZE;
        }
    }

    if (COMMAND_SPECIFIC_COUNTER_UPDATE_SUPPORT) {
        // Update Command Specific Retry Counter.
        Interface->Kcs->KcsRetryCounter = (UINT16)AmiIpmiUpdateCommandSpecificDelay (
                                                                                NetFunction,
                                                                                Command );
        DEBUG((DEBUG_INFO,"Counter value is %x\n",Interface->Kcs->KcsRetryCounter));
    }

    if ((Status = SendDataToBmcPort (
                    Interface,
                    Context,
                    CmdDataBuffer,
                    (UINT8)(CommandDataSize + 2) )) != EFI_SUCCESS) {
        Interface->Kcs->KcsSoftErrorCount++;
        AmiIpmiTransportReleaseLock (&Interface->Kcs->KcsTransportLocked);
        return Status;
    }
    
    DataSize = AMI_IPMI_MAX_KCS_CMD_DATA_SIZE;
    if ((Status = ReceiveBmcDataFromPort (
                    Interface,
                    Context,
                    CmdDataBuffer,
                    &DataSize )) != EFI_SUCCESS) {
        Interface->Kcs->KcsSoftErrorCount++;
        AmiIpmiTransportReleaseLock (&Interface->Kcs->KcsTransportLocked);
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
        AmiIpmiTransportReleaseLock (&Interface->Kcs->KcsTransportLocked);
        return EFI_DEVICE_ERROR;
    }

    if (DataSize < 3) {
        AmiIpmiTransportReleaseLock (&Interface->Kcs->KcsTransportLocked);
        return EFI_DEVICE_ERROR;
    }

    if ((DataSize - 2) > *((UINT8 *)ResponseDataSize)) {
        *ResponseDataSize = (UINT8) (DataSize - 2);
        AmiIpmiTransportReleaseLock (&Interface->Kcs->KcsTransportLocked);
        return EFI_BUFFER_TOO_SMALL;
    }

    // Copying the response data into ResponseData buffer.
    CopyMem (
        ResponseData,
        &CmdDataBuffer[2],
        (DataSize - 2) );
    *ResponseDataSize = (UINT8) (DataSize - 2);

    AmiIpmiTransportReleaseLock (&Interface->Kcs->KcsTransportLocked);
    return EFI_SUCCESS;
}

/** @internal
    Initialize KCS interface specific data.

    @param[in, out] Interface   System interface pointer.

    @retval EFI_SUCCESS             Interface is successfully initialized.
    @retval EFI_OUT_OF_RESOURCES    Enough resources are not exists.
    @retval Others                  Return status while initializing interface.

**/
EFI_STATUS
AmiInitKcsInterfaceData (
  IN OUT AMI_SYSTEM_INTERFACE   *Interface )
{
    EFI_STATUS  Status = EFI_SUCCESS;
    
    if ((Interface->Kcs != NULL) && \
        (Interface->Kcs->InterfaceState == AmiIpmiInterfaceInitialized)) {
        return Status;
    }

    Status = AmiIpmiGetKcsApiPtr (Interface);
    if (EFI_ERROR(Status)) {
        if (Status != EFI_OUT_OF_RESOURCES) {
            Interface->Kcs->InterfaceState = AmiIpmiInterfaceInitError;
        }
        return Status;
    }
    
    Interface->Kcs->KcsRetryCounter = IPMI_KCS_COMMAND_PORT_READ_RETRY_COUNTER;
    
    if (IPMI_DEFAULT_ACCESS_TYPE) {
        Interface->Kcs->AccessType = AmiIpmiIoAccess;
        Interface->Kcs->MmioBaseAddress = 0;
        Interface->Kcs->BaseAddressRange = 0;
    } else {
        Interface->Kcs->AccessType = AmiIpmiMmioAccess;
        Status = OemInitKcsInterfaceDataHook (
                    &Interface->Kcs->MmioBaseAddress,
                    &Interface->Kcs->BaseAddressRange );
        DEBUG ((DEBUG_INFO, "OemInitKcsInterfaceDataHook Status is %r.\n", Status));
    }

    AmiIpmiTransportReleaseLock (&Interface->Kcs->KcsTransportLocked);

    Interface->Kcs->InterfaceState = AmiIpmiInterfaceInitialized;
    
    DEBUG ((DEBUG_INFO, "%a Initialized Status = %r\n", __FUNCTION__, Status));
    
    return Status;
}

