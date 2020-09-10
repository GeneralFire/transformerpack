//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file AmiBmcCommonInterfaceLib.c
    AmiBmcCommonInterfaceLib generic functions for all interfaces.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <Uefi/UefiBaseType.h>
#include <AmiIpmiInitHooks.h>
#include <AmiCommandSpecificRetryCountTable.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmiIpmiLib.h>
#include <Library/AmiBmcCommonInterfaceLib.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

extern AMI_IPMI_INIT_INTERFACE_DATA_HOOK INIT_INTERFACE_DATA_HOOK_LIST \
                                            EndOfInitInterfaceDataHookList;
AMI_IPMI_INIT_INTERFACE_DATA_HOOK   *gInitInterfaceDataHook [] = { \
                                        INIT_INTERFACE_DATA_HOOK_LIST \
                                        NULL \
                                        };

extern AMI_IPMI_SEND_COMMAND_WRAPPER IPMI_SEND_COMMAND_LIST EndOfIpmiSendCommandList;

AMI_IPMI_SEND_COMMAND_WRAPPER *gIpmiSendCommand [] = {IPMI_SEND_COMMAND_LIST NULL};

//---------------------------------------------------------------------------

/** @internal
    Read 8 bit data from BMC port based on access type.

    @param[in] AccessType   Specifies MMIO or IO access.
    @param[in] Address      Specifies Address to read.

    @return UINT8   Data read.

**/
UINT8
AmiIpmiBmcRead8 (
  IN UINT8  AccessType,
  IN UINTN  Address )
{
    if (AccessType == AmiIpmiIoAccess) {
        return IoRead8 (Address);
    } else {
        return MmioRead8 (Address);
    }
}

/** @internal
    Write 8 bit data to BMC port based on access type.

    @param[in] AccessType   Specifies MMIO or IO access.
    @param[in] Address      Specifies Address to write.
    @param[in] Data         Specifies data to be written.

    @return Data written.

**/
UINT8
AmiIpmiBmcWrite8 (
  IN UINT8  AccessType,
  IN UINTN  Address,
  IN UINT8  Data )
{
    if (AccessType == AmiIpmiIoAccess) {
        return IoWrite8 (
                    Address,
                    Data );
    } else {
        return MmioWrite8 (
                    Address,
                    Data );
    }
}

/** @internal
    Acquire the lock to use the IPMI transport.

    @param[out] Lock    Pointer to Lock.

    @return VOID    Returns nothing.

**/
VOID
AmiIpmiTransportAcquireLock (
  OUT BOOLEAN   *Lock )
{
    *Lock = TRUE;
}

/** @internal
    Release the lock of IPMI transport.

    @param[out] Lock    Pointer to Lock.

    @return VOID    Returns nothing.

**/
VOID
AmiIpmiTransportReleaseLock (
  OUT BOOLEAN   *Lock )
{
    *Lock = FALSE;
}

/** @internal
    Returns the Lock state of IPMI transport.

    @param[in]  Lock      Pointer to Lock.

    @retval TRUE    IPMI transport is in lock state.
    @retval FALSE   IPMI transport is in release state.

*/
BOOLEAN
AmiIpmiIsIpmiTransportlocked (
  IN BOOLEAN    *Lock )
{
    return *Lock;
}

/** @internal
    Updates the SoftErrorCount of specific interface based on the BMC Error input.

    @param[in]      BmcError        BMC Error.
    @param[in, out] Interface       Interface pointer to update soft error count.
    @param[in]      InterfaceType   Interface type to communicate.

    @retval EFI_SUCCESS             Updated SoftErrorCount of specific interface.
    @retval EFI_INVALID_PARAMETER   Invalid Interface pointer or Interface type.

**/
EFI_STATUS
AmiIpmiUpdateSoftErrorCount (
  IN     UINT8                      BmcError,
  IN OUT AMI_SYSTEM_INTERFACE       *Interface,
  IN     AMI_SYSTEM_INTERFACE_TYPE  InterfaceType )
{
    UINT8   Errors[] = {0xC0, 0xC3, 0xC4, 0xC9, 0xCE, 0xCF, 0xFF, 0x00};
    UINT8   Index = 0;

    if ((!Interface) || (InterfaceType <= AmiSysInterfaceUnknown) || \
        (InterfaceType >= AmiSysInterfaceMax)) {
        return EFI_INVALID_PARAMETER;
    }
    while (Errors[Index] != 0) {
        
        if (Errors[Index] == BmcError) {
            
            switch (InterfaceType) {
                case AmiSysInterfaceKcs:
                    Interface->Kcs->KcsSoftErrorCount++;
                    break;

                case AmiSysInterfaceBt:
                    Interface->Bt.BtSoftErrorCount++;
                    break;

                case AmiSysInterfaceSsif:
                    Interface->Ssif.SsifSoftErrorCount++;
                    break;

                case AmiSysInterfaceIpmb:
                    Interface->Ipmb.IpmbSoftErrorCount++;
                    break;

                case AmiSysInterfaceOem1:
                    Interface->Oem1.Oem1SoftErrorCount++;
                    break;

                case AmiSysInterfaceOem2:
                    Interface->Oem2.Oem2SoftErrorCount++;
                    break;
                default :
                    break;
            }
        }
        Index++;
    }
    return EFI_SUCCESS;
}

/** @internal
    Initialize interface specific data.

    @param[in, out]  Interface  System interface pointer.

    @return EFI_STATUS  Return status of the specific interface hook.

**/
EFI_STATUS
AmiIpmiInitInterfaceData (
  IN OUT AMI_IPMI_BMC_INSTANCE_DATA   *IpmiInstance )
{
    EFI_STATUS  Status = EFI_SUCCESS;
    UINTN       Index;

    if (IpmiInstance->Signature != AMI_SM_IPMI_BMC_SIGNATURE) {
        IpmiInstance->Signature                        = AMI_SM_IPMI_BMC_SIGNATURE;
        IpmiInstance->SlaveAddress                     = IPMI_BMC_SLAVE_ADDRESS;
        IpmiInstance->BmcStatus                        = AmiBmcStatusOk;
        IpmiInstance->IpmiTransport.SendIpmiCommand    = EfiIpmiSendCommand;
        IpmiInstance->IpmiTransport.SendIpmiCommandEx  = EfiIpmiSendCommandEx;
        IpmiInstance->IpmiTransport.GetBmcStatus       = EfiIpmiGetBmcStatus;
    }

    for (Index = 0; gInitInterfaceDataHook[Index]; Index++) {
        Status = gInitInterfaceDataHook[Index] (&IpmiInstance->Interface);
        DEBUG ((DEBUG_INFO, "InitInterfaceDataHook[%d] Status = %r\n", Index, Status));
        if (Status == EFI_OUT_OF_RESOURCES) {
            break;
        }
    }

    return Status;
}

/** @internal
    Updates Command specific retry counter based on the availability.
    If not available, updates default retry counter value.

    @param[in] NetFunction  Net function of the command.
    @param[in] Command      Command to be sent to BMC.

    @return UINT32  Retry count value. Returns 0 if none of the delay
                    is provided.

    @note   This function is only for KCS and BT interface.

**/
UINT32
AmiIpmiUpdateCommandSpecificDelay (
  IN UINT8  NetFunction,
  IN UINT8  Command )
{
    UINTN   TableSize;
    UINTN   Index;

    DEBUG((DEBUG_INFO, "%a Entry... \n", __FUNCTION__));

    // Command specific retry count table.
    TableSize = (sizeof(gCommandSpecificRetryCountTable)/
                   sizeof(AMI_COMMAND_SPECIFIC_RETRY_COUNT));

    for (Index = 0; Index < TableSize-1; Index++) {
        if (NetFunction == gCommandSpecificRetryCountTable[Index].NetFunction &&\
            Command == gCommandSpecificRetryCountTable[Index].Command ){
            DEBUG((DEBUG_INFO,"Retry counter value changed.\n"));

            if (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceKcs) {
                return gCommandSpecificRetryCountTable[Index].Data32.RetryCounter;
            } else if (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceBt) {
                return gCommandSpecificRetryCountTable[Index].Data32.BtDelay;
            }
        }
    }
    DEBUG((DEBUG_INFO,"Retry counter value is not changed as there is no match.\n"));
    if(DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceKcs) {
        return IPMI_KCS_COMMAND_PORT_READ_RETRY_COUNTER;
    } else if(DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceBt) {
        return IPMI_BT_COMMAND_PORT_READ_RETRY_COUNTER;
    }
    return 0;
}

/**
    Ipmi send command API for default Host to BMC interface.

    @param[in]      This                Pointer to Ipmi Protocol/Ppi.
    @param[in]      NetFunction         Net function of the command.
    @param[in]      Lun                 Logical Unit Number of the command.
    @param[in]      Command             Command to be sent to BMC.
    @param[in]      CommandData         Command data to be sent along with
                                        Command.
    @param[in]      CommandDataSize     Command Data size.
    @param[out]     ResponseData        Pointer to the response data buffer.
    @param[in, out] ResponseDataSize    Pointer to the response data size.

    @retval EFI_INVALID_PARAMETER   Invalid parameter.
    @retval Others                  Status returned from BMC while executing
                                    the command.

**/
EFI_STATUS
EFIAPI
EfiIpmiSendCommand (
  IN     AMI_IPMI_TRANSPORT_INSTANCE    *This,
  IN     UINT8                          NetFunction,
  IN     UINT8                          Lun,
  IN     UINT8                          Command,
  IN     UINT8                          *CommandData,
  IN     UINT8                          CommandDataSize,
  OUT    UINT8                          *ResponseData,
  IN OUT UINT8                          *ResponseDataSize )
{
    EFI_STATUS                     Status = EFI_UNSUPPORTED;
    AMI_IPMI_BMC_INSTANCE_DATA     *IpmiInstance;
    UINT8                          CompletionCode;
    UINT8                          Index;
    UINT32                         CommandResDataSize;

    if (!ResponseDataSize) {
        return EFI_INVALID_PARAMETER;
    }
    IpmiInstance = AMI_IPMI_INSTANCE_FROM_EFI_SM_IPMI_BMC_THIS (This);

    DEBUG ((DEBUG_INFO, "%a - InterfaceType = %d.\n", __FUNCTION__, DEFAULT_SYSTEM_INTERFACE));

    CommandResDataSize = *ResponseDataSize;

    for (Index = 0; gIpmiSendCommand[Index]; Index++) {
        Status = gIpmiSendCommand[Index] (
                    &IpmiInstance->Interface,
                    NetFunction,
                    Lun,
                    Command,
                    CommandData,
                    CommandDataSize,
                    ResponseData,
                    &CommandResDataSize,
                    &CompletionCode,
                    DEFAULT_SYSTEM_INTERFACE,
                    NULL );

        DEBUG ((DEBUG_INFO, "%a - gIpmiSendCommand[%d] Status = %r.\n", __FUNCTION__, Index, Status));

        // Break the loop if status is not EFI_UNSUPPORTED.
        if (Status != EFI_UNSUPPORTED) {
            break;
        }
    }

    *ResponseDataSize = (UINT8) CommandResDataSize;
    return Status;
}

/**
    Ipmi send command API for specific Host to BMC interface.
    It updates completion code.

    @param[in]      This                Pointer to Ipmi Protocol/Ppi.
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

    @retval EFI_UNSUPPORTED     Interface type is invalid or CommandDataSize
                                or ResponseDataSize is invalid for given
                                InterfaceType.
    @retval Others              Status returned from BMC while executing
                                the command.

**/
EFI_STATUS
EFIAPI
EfiIpmiSendCommandEx (
  IN     AMI_IPMI_TRANSPORT_INSTANCE    *This,
  IN     UINT8                          NetFunction,
  IN     UINT8                          Lun,
  IN     UINT8                          Command,
  IN     UINT8                          *CommandData,
  IN     UINT32                         CommandDataSize,
  OUT    UINT8                          *ResponseData,
  IN OUT UINT32                         *ResponseDataSize,
  OUT    UINT8                          *CompletionCode,
  IN     AMI_SYSTEM_INTERFACE_TYPE      InterfaceType )
{
    EFI_STATUS                     Status = EFI_UNSUPPORTED;
    AMI_IPMI_BMC_INSTANCE_DATA     *IpmiInstance;
    UINT8                          Index;

    if (InterfaceType == AmiSysInterfaceUnknown) {
        InterfaceType = DEFAULT_SYSTEM_INTERFACE; 
    }

    if (InterfaceType && (InterfaceType >= AmiSysInterfaceMax)) {
        DEBUG ((DEBUG_INFO, "%a - InterfaceType = %d Not supported\n", __FUNCTION__, InterfaceType));
        return Status;
    }

    if (InterfaceType != AmiSysInterfaceUsb && \
        ((CommandDataSize > 0xFF) || (*ResponseDataSize > 0xFF))) {
        return Status;
    }

    DEBUG ((DEBUG_INFO, "%a - InterfaceType = %d.\n", __FUNCTION__, InterfaceType));

    IpmiInstance = AMI_IPMI_INSTANCE_FROM_EFI_SM_IPMI_BMC_THIS (This);

    for (Index = 0; gIpmiSendCommand[Index]; Index++) {
        Status = gIpmiSendCommand[Index] (
                    &IpmiInstance->Interface, 
                    NetFunction,
                    Lun,
                    Command,
                    CommandData,
                    CommandDataSize,
                    ResponseData,
                    ResponseDataSize,
                    CompletionCode,
                    InterfaceType,
                    NULL );

        DEBUG ((DEBUG_INFO, "%a - gIpmiSendCommand[%d] Status = %r.\n", __FUNCTION__, Index, Status));

        //break the loop if status is not EFI_UNSUPPORTED
        if (Status != EFI_UNSUPPORTED) {
            break;
        }
    }

    return Status;
}

/** @internal
    This routine returns BMC status, channel type etc.

    @param[in]  This        Pointer to Ipmi Protocol/Ppi.
    @param[out] BmcStatus   Pointer to Bmc status.
    @param[out] ComAddress  Pointer to Bmc communication address.

    @retval EFI_SUCCESS             Returned the Bmc status successfully.
    @retval EFI_INVALID_PARAMETER   Invalid parameter.

**/
EFI_STATUS
AmiIpmiGetBmcStatusLocal (
  IN  AMI_IPMI_TRANSPORT_INSTANCE   *This,
  OUT AMI_BMC_STATUS                *BmcStatus,
  OUT AMI_SM_COM_ADDRESS            *ComAddress,
  IN  VOID                          *Context,
  IN  AMI_SYSTEM_INTERFACE_TYPE     InterfaceType )
{
    EFI_STATUS                      Status;
    IPMI_GET_DEVICE_ID_RESPONSE     GetDeviceIdResponse;
    AMI_IPMI_BMC_INSTANCE_DATA      *IpmiInstance;

    if (!BmcStatus || !ComAddress) {
        return EFI_INVALID_PARAMETER;
    }
    IpmiInstance = AMI_IPMI_INSTANCE_FROM_EFI_SM_IPMI_BMC_THIS (This);

    // Get the device ID information for the BMC.
    Status = AmiIpmiGetDeviceId (
                This,
                &GetDeviceIdResponse );
    // If there is no error then proceed to check the data returned by the BMC.
    if (!EFI_ERROR(Status)) {
        /* If the controller is not in Update Mode then set the BMC Status to indicate
           the BMC is ok.*/
        if (!GetDeviceIdResponse.FirmwareRev1.Bits.UpdateMode)
        {
            IpmiInstance->BmcStatus = AmiBmcStatusOk;
        }
    } else {
        IpmiInstance->BmcStatus = AmiBmcStatusSoftFail;
        switch (InterfaceType) {
            case AmiSysInterfaceKcs: 
                if (IpmiInstance->Interface.Kcs->KcsSoftErrorCount >= AMI_IPMI_MAX_SOFT_COUNT) {
                    IpmiInstance->BmcStatus = AmiBmcStatusHardFail;
                }
                break;
            case AmiSysInterfaceBt:
                if (IpmiInstance->Interface.Bt.BtSoftErrorCount >= AMI_IPMI_MAX_SOFT_COUNT) {
                    IpmiInstance->BmcStatus = AmiBmcStatusHardFail;
                }
                break;
            case AmiSysInterfaceSsif:
                if (IpmiInstance->Interface.Ssif.SsifSoftErrorCount >= AMI_IPMI_MAX_SOFT_COUNT) {
                    IpmiInstance->BmcStatus = AmiBmcStatusHardFail;
                }
                break;
            case AmiSysInterfaceUsb:
                IpmiInstance->BmcStatus = AmiBmcStatusHardFail;
                break;
            case AmiSysInterfaceOem1:
                if (IpmiInstance->Interface.Oem1.Oem1SoftErrorCount >= AMI_IPMI_MAX_SOFT_COUNT) {
                    IpmiInstance->BmcStatus = AmiBmcStatusHardFail;
                }
                break;
            case AmiSysInterfaceOem2:
                if (IpmiInstance->Interface.Oem2.Oem2SoftErrorCount >= AMI_IPMI_MAX_SOFT_COUNT) {
                    IpmiInstance->BmcStatus = AmiBmcStatusHardFail;
                }
                break;
            default:
                IpmiInstance->BmcStatus = AmiBmcStatusSoftFail;
        }
    }
    *BmcStatus                              = IpmiInstance->BmcStatus;
    ComAddress->ChannelType                 = AmiChannelMediumTypeBmc;
    ComAddress->BmcAddress.SlaveAddress     = IpmiInstance->SlaveAddress;
    ComAddress->BmcAddress.ChannelAddress   = 0x0;

    return EFI_SUCCESS;
}

/**
    Ipmi get bmc status API which returns BMC status, channel type etc.

    @param[in]  This        Pointer to Ipmi Protocol/Ppi.
    @param[out] BmcStatus   Pointer to Bmc status.
    @param[out] ComAddress  Pointer to Bmc communication address.

    @return EFI_STATUS  Status returned while getting the Bmc status.

**/
EFI_STATUS
EFIAPI
EfiIpmiGetBmcStatus (
  IN  AMI_IPMI_TRANSPORT_INSTANCE   *This,
  OUT AMI_BMC_STATUS                *BmcStatus,
  OUT AMI_SM_COM_ADDRESS            *ComAddress )
{
    return AmiIpmiGetBmcStatusLocal (
                This,
                BmcStatus,
                ComAddress,
                NULL,
                DEFAULT_SYSTEM_INTERFACE );
}

/**
    Execute the Self Test command with specific interface 

    @param IpmiInstance       IpmiInstance Data structure pointer.
     @param InterfaceType     Interface Type

    @return EFI_STATUS       Return Status
**/
EFI_STATUS
SelfTestBySpecificInterface (
  IN OUT AMI_IPMI_BMC_INSTANCE_DATA      *IpmiInstance,
  IN     AMI_SYSTEM_INTERFACE_TYPE       InterfaceType,
  OUT    AMI_BMC_STATUS                  *BmcStatus
  )
{
    EFI_STATUS                        Status;
    IPMI_SELF_TEST_RESULT_RESPONSE    BstStatus;
    UINT32                            ResponseDataSize;
    UINT8                             CompletionCode;

    if (!IpmiInstance || !BmcStatus) {
        return EFI_INVALID_PARAMETER;
    }

    ResponseDataSize = sizeof(IPMI_SELF_TEST_RESULT_RESPONSE);

    Status = IpmiInstance->IpmiTransport.SendIpmiCommandEx (
                                                &IpmiInstance->IpmiTransport,
                                                IPMI_NETFN_APP,
                                                AMI_BMC_LUN,
                                                IPMI_APP_GET_SELFTEST_RESULTS,
                                                NULL,
                                                0,
                                                (UINT8 *)&BstStatus,
                                                &ResponseDataSize,
                                                &CompletionCode,
                                                InterfaceType);

    DEBUG ((DEBUG_INFO, "IPMI_APP_GET_SELFTEST_RESULTS Status: %r BstStatus.Result :%x \n", Status ,BstStatus.Result));

    if (EFI_ERROR(Status)) {
        *BmcStatus = AmiBmcStatusHardFail;
        return Status;
    }

    if (CompletionCode == AMI_IPMI_COMPLETION_CODE_SUCCESS) {
        /* Check the self test results.  Cases 55h - 58h are Ipmi defined
           test results. Additional Cases are device specific test results.*/
        switch (BstStatus.Result) {
            case IPMI_APP_SELFTEST_NO_ERROR: // 0x55
            case IPMI_APP_SELFTEST_NOT_IMPLEMENTED: // 0x56
            case AMI_IPMI_APP_SELFTEST_RESERVED: // 0xFF
                *BmcStatus = AmiBmcStatusOk;
                break;

            case IPMI_APP_SELFTEST_ERROR: // 0x57
                *BmcStatus = AmiBmcStatusSoftFail;
                break;

            default: // 0x58 and Other Device Specific Hardware Error.
                *BmcStatus = AmiBmcStatusHardFail;
                break;
        }
    }

    return Status;
    
}
