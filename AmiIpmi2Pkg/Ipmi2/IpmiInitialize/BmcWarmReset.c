//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

/** @file BmcWarmReset.c
    Sends Warm Reset command to BMC.

*/

//---------------------------------------------------------------------------

#include <ServerMgmtSetup.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AMIPostMgr.h>
#include <Protocol/IpmiTransportProtocol.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define STRING_BUFFER_LENGTH    100

//---------------------------------------------------------------------------

/** @internal
    This function sends warm reset command to BMC

    @param[in] This     Ipmi Transport Protocol pointer.

    @return EFI_STATUS  Return status of send ipmi command.

**/
EFI_STATUS
SendWarmResetCommandToBMC (
  IN EFI_IPMI_TRANSPORT     *This )
{
    EFI_STATUS      Status;
    UINT8           CompletionCode;
    UINT8           ResponseDataSize = sizeof (CompletionCode);

    Status = This->SendIpmiCommand (
                        This,
                        IPMI_NETFN_APP,
                        AMI_BMC_LUN,
                        IPMI_APP_WARM_RESET,
                        NULL,
                        0,
                        &CompletionCode,
                        &ResponseDataSize );

    return Status;
}

/**
    Sends warm reset command to BMC as per user choice.

    @param[in] HiiHandle    Handle to HII database.
    @param[in] Class        Formset Class.
    @param[in] SubClass     Formset Subclass.
    @param[in] Key          Formset Key.

    @retval EFI_UNSUPPORTED     Invalid key.
    @retval EFI_SUCCESS         User gives no in setup prompt or warm reset
                                command sent successfully.
    @retval Others              Error status returned while locating
                                protocol or error occurred while sending
                                command.

**/
EFI_STATUS
BmcWarmResetCallbackFunction (
 IN EFI_HII_HANDLE     HiiHandle,
 IN UINT16             Class,
 IN UINT16             SubClass,
 IN UINT16             Key )
{
    EFI_STATUS                  Status = EFI_SUCCESS;
    EFI_IPMI_TRANSPORT          *IpmiTransport;
    AMI_POST_MANAGER_PROTOCOL   *AmiPostMgr = NULL;
    UINT8                       MsgBoxSel = 0;
    CALLBACK_PARAMETERS         *CallbackParameters;
    EFI_STRING                  CharPtrString;
    EFI_STRING                  OutputString;

    DEBUG ((DEBUG_INFO, "%a Key Value : %x\n", __FUNCTION__, Key));

    // Continue only for BMC_WARM_RESET_KEY.
    if (Key != BMC_WARM_RESET_KEY) {
        return EFI_UNSUPPORTED;
    }

    // Get the call back parameters and verify the action.
    CallbackParameters = GetCallbackParameters ();
    if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) {
        DEBUG ((DEBUG_INFO,"\n CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n",Status));
        return EFI_SUCCESS;
    }

    // Locate AmiPostMgrProtocol to display the message boxes in setup.
    Status = gBS->LocateProtocol (
                    &gAmiPostManagerProtocolGuid,
                    NULL,
                    (VOID **)&AmiPostMgr );
    DEBUG ((DEBUG_INFO, "gAmiPostManagerProtocolGuid locateProtocol Status: %r\n", Status));

    if (EFI_ERROR(Status)) {
        return  Status;
    }

    // Locate IpmiTransport protocol.
    Status = gBS->LocateProtocol (
                    &gEfiDxeIpmiTransportProtocolGuid,
                    NULL,
                    (VOID **)&IpmiTransport );
    DEBUG ((DEBUG_INFO, " gEfiDxeIpmiTransportProtocolGuid Status: %r \n", Status));

    // If locate protocol fails, display error message and return.
    if (EFI_ERROR(Status)) {
        OutputString = HiiGetString (
                            HiiHandle,
                            STRING_TOKEN(STR_PROTOCOL_NOT_FOUND),
                            NULL );
        
        CharPtrString = HiiGetString (
                            HiiHandle,
                            STRING_TOKEN(STR_BMC_WARM_RESET_ERROR_INFO),
                            NULL );
        
        if((OutputString != NULL) && (CharPtrString != NULL)) {
            AmiPostMgr->DisplayMsgBox (
                            CharPtrString,
                            OutputString,
                            MSGBOX_TYPE_OK,
                            &MsgBoxSel );
            gBS->FreePool (OutputString);
            gBS->FreePool (CharPtrString);
            return Status;
        }
    }

    // Get confirmation from the user before doing warm reset.
    
    OutputString = HiiGetString (
                        HiiHandle,
                        STRING_TOKEN(STR_WARM_RESET_BMC),
                        NULL );
    
    CharPtrString = HiiGetString (
                        HiiHandle,
                        STRING_TOKEN(STR_BMC_WARM_RESET_INFO),
                        NULL );
    
    if((OutputString != NULL) && (CharPtrString != NULL)) {
        Status = AmiPostMgr->DisplayMsgBox (
                                 CharPtrString,
                                 OutputString,
                                 MSGBOX_TYPE_YESNO,
                                 &MsgBoxSel );
        DEBUG ((DEBUG_INFO, "DisplayMsgBox return Status: %r\n", Status));
        DEBUG ((DEBUG_INFO, "MsgBoxSel: %x\n", MsgBoxSel));
        gBS->FreePool (OutputString);
        gBS->FreePool (CharPtrString);
    }

    // If user said no, then return EFI_SUCCESS to quit.
    if((EFI_ERROR(Status)) || MsgBoxSel == MSGBOX_NO) {
        return EFI_SUCCESS;
    }

    // Send warm reset command to BMC.
    Status = SendWarmResetCommandToBMC (IpmiTransport);
    DEBUG ((DEBUG_INFO, " SendWarmResetCommandToBMC Status: %r \n", Status));
    if(EFI_ERROR(Status)) {
        OutputString = HiiGetString (
                            HiiHandle,
                            STRING_TOKEN(STR_BMC_WARM_RESET_FAIL),
                            NULL );

        CharPtrString = HiiGetString (
                            HiiHandle,
                            STRING_TOKEN(STR_BMC_WARM_RESET_ERROR_INFO),
                            NULL );

        if((OutputString != NULL) && (CharPtrString != NULL)) {
            AmiPostMgr->DisplayMsgBox (
                            CharPtrString,
                            OutputString,
                            MSGBOX_TYPE_OK,
                            &MsgBoxSel );
            gBS->FreePool (OutputString);
            gBS->FreePool (CharPtrString);
            return  Status;
        }
    }

    // If command success, display the message and return.
    
    OutputString = HiiGetString (
                        HiiHandle,
                        STRING_TOKEN(STR_BMC_WARM_RESET_DONE),
                        NULL);
    
    CharPtrString = HiiGetString (
                        HiiHandle,
                        STRING_TOKEN(STR_BMC_WARM_RESET_INFO),
                        NULL);

    if((OutputString != NULL) && (CharPtrString != NULL)) {
        AmiPostMgr->DisplayMsgBox (
                        CharPtrString,
                        OutputString,
                        MSGBOX_TYPE_OK,
                        &MsgBoxSel );
         gBS->FreePool (OutputString);
         gBS->FreePool (CharPtrString);
    }
    return Status;
}

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
