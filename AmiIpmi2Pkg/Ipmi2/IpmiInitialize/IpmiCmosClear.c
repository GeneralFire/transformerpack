//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

/** @file IpmiCmosClear.c
    Check CMOS Clear flag is set and save NVRAM variable.

*/

//---------------------------------------------------------------------------

#include <AmiIpmiNetFnChassisDefinitions.h>
#include <Library/AmiIpmiBootLib.h>
#include <Protocol/Reset.h>
#include "DxeIpmiInitialize.h"

//---------------------------------------------------------------------------

/** @internal
    Clears CMOS flag bit in Boot flags.

    @param[in] Param5   Boot Flags.

    @return EFI_STATUS  Return status of the function calls used.

**/
EFI_STATUS
IpmiClearCmosFlag (
  IN AMI_BOOT_OPTIONS_BOOT_FLAGS    Param5 )
{
    EFI_STATUS                              Status;
    AMI_SET_BOOT_OPTIONS_BOOT_FLAG_REQUEST  RequestData;
    UINT8                                   RequestDataSize;
    UINT8                                   ResponseDataSize;
    UINT8                                   CompletionCode;

    // Wait until Set In Progress field is cleared.
    Status = AmiIpmiGetSysBootOptionsWaitSetInProgressClear (&gIpmiInstance->IpmiTransport);
    
    if (EFI_ERROR (Status)) {
        return Status;
    }

    Param5.CmosClear                        = AMI_IPMI_BIT_CLEAR; // Clear CMOS Flag bit.
    RequestData.Param5                      = Param5; // Retain other flag value.
    RequestData.Request.ParameterSelector   = AmiBootOptionParamBootFlags; // Parameter Selector.
    ResponseDataSize                        = sizeof (CompletionCode);
    RequestDataSize                         = sizeof (RequestData);
    Status = gIpmiInstance->IpmiTransport.SendIpmiCommand (
                                            &gIpmiInstance->IpmiTransport,
                                            IPMI_NETFN_CHASSIS,
                                            AMI_BMC_LUN,
                                            IPMI_CHASSIS_SET_SYSTEM_BOOT_OPTIONS,
                                            (UINT8*) &RequestData,
                                            RequestDataSize,
                                            (UINT8*) &CompletionCode,
                                            &ResponseDataSize );
    return Status;
}

/**
    Notification function to reset the system for CMOS clear.

    @param[in] Event    Event which caused this handler.
    @param[in] Context  Context passed during Event Handler registration.

    @return VOID    Nothing.

**/
VOID
EFIAPI
ResetArchProtocolNotification (
  IN EFI_EVENT  Event,
  IN VOID       *Context )
{
    EFI_STATUS      Status;
    VOID            *ResetArchProtocol = NULL;

    DEBUG ((DEBUG_INFO, "%a Entered...\n", __FUNCTION__));

    Status = gBS->LocateProtocol (
                    &gEfiResetArchProtocolGuid,
                    NULL,
                    (VOID **)&ResetArchProtocol );
    
    DEBUG ((DEBUG_INFO, "gBS->LocateProtocol gEfiResetArchProtocolGuid protocol  status %r\n", Status));
    
    if (!EFI_ERROR (Status)) {
        // Reset System
        gRT->ResetSystem (
                IPMI_CMOS_CLEAR_RESET_TYPE,
                EFI_SUCCESS,
                0,
                NULL );
    }
    DEBUG ((DEBUG_INFO, "%a Exiting...\n", __FUNCTION__));
}

/** @internal
    Checks for CMOS Clear flag bit is set and create a Nvram variable if it
    is set.

    @param VOID     Nothing.

    @return EFI_STATUS  Return status of the functions used.

**/
EFI_STATUS 
IpmiCmosClear (
  VOID )
{
    EFI_STATUS                                  Status;
    AMI_GET_BOOT_OPTIONS_BOOT_FLAG_RESPONSE     BootFlags;
    VOID                                        *NotifyRegistration;
    UINTN                                       Size;
    UINT8                                       Value;

    Size = sizeof (UINT8);
    Status = gRT->GetVariable (
                    L"IpmiCmosClear",
                    &gEfiIpmiCmosClearVariableGuid,
                    NULL,
                    &Size,
                    &Value );
    
    if (!EFI_ERROR (Status)) {
        Status = gRT->SetVariable (
                        L"IpmiCmosClear",
                        &gEfiIpmiCmosClearVariableGuid,
                        EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                        0,
                        &Value );
        if (EFI_ERROR (Status)) {
            DEBUG ((DEBUG_ERROR, "%a IpmiCmosClear SetVariable Status:%r\n", __FUNCTION__,Status));
            return Status;
        }
    }
    
    Status = AmiIpmiGetBootFlags (
                &gIpmiInstance->IpmiTransport,
                &BootFlags );

    if (EFI_ERROR (Status)) {
        return Status;
    }
    
    if (BootFlags.Param5.BootFlagValid) {
        if (BootFlags.Param5.CmosClear) {
            Size = sizeof (UINT8);
            Value = 1;
            Status = gRT->SetVariable (
                            L"IpmiCmosClear",
                            &gEfiIpmiCmosClearVariableGuid,
                            EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                            Size,
                            &Value );
              
            if (!EFI_ERROR (Status)) {
                // Create Notification event for Reset arch protocol GUID
                EfiCreateProtocolNotifyEvent (
                    &gEfiResetArchProtocolGuid,
                    TPL_NOTIFY,
                    ResetArchProtocolNotification,
                    NULL,
                    &NotifyRegistration );

                // Clear the BMC BootFlags
                Status = IpmiClearCmosFlag (BootFlags.Param5);
            }
        }
    }

    return Status;
}

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
