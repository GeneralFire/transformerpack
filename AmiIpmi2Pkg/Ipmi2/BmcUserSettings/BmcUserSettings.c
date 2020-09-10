//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file BmcUserSettings.c
    Provides functionality to Add/Delete/Change BMC user
    settings.

*/

//---------------------------------------------------------------------------

#include "BmcUserSettings.h"

//---------------------------------------------------------------------------

/** @internal
    This function collects number of users allowed BMC.

    @param VOID     Nothing.

    @return VOID    Nothing.

**/
VOID
GetMaxUserAllowable (
 VOID )
{
    EFI_STATUS                        Status;
    AMI_GET_USER_ACCESS_REQUEST       GetUserAccessRequest;
    AMI_GET_USER_ACCESS_RESPONSE      GetUserAccessResponse;
    UINT8                             ResponseDataSize;

    DEBUG ((DEBUG_INFO, "IPMI: Inside %a\n", __FUNCTION__));
    if (gPcdLanChannelBufferPtr[0] != 0) {
        GetUserAccessRequest.ChannelNo = gPcdLanChannelBufferPtr[0];
        GetUserAccessRequest.UserId = 1;// Specifying any User id to get Max UserId. Ex: 1.
        GetUserAccessRequest.Reserve1 = 0x00;
        GetUserAccessRequest.Reserve2 = 0x00;
        ResponseDataSize = sizeof (GetUserAccessResponse);
        Status = gIpmiTransport->SendIpmiCommand (
                                    gIpmiTransport,
                                    IPMI_NETFN_APP,
                                    AMI_BMC_LUN,
                                    IPMI_APP_GET_USER_ACCESS,
                                    (UINT8 *)&GetUserAccessRequest,
                                    sizeof (GetUserAccessRequest),
                                    (UINT8 *)&GetUserAccessResponse,
                                    &ResponseDataSize );

        DEBUG ((DEBUG_INFO, "IPMI: Status of Get User Access Command: %r \n", Status));

        if (!EFI_ERROR(Status)) {
            DEBUG ((DEBUG_INFO, "IPMI: Maximum User Id allowable: %04x \n", GetUserAccessResponse.MaxUserId));
            gAllowableUser = GetUserAccessResponse.MaxUserId;
            return;
        }
    }
}

/** @internal
    This function collects all user names from BMC.

    @param VOID     Nothing.

    @return VOID    Nothing.

**/
VOID
CollectAllBmcUserDetails (
  VOID )
{
    UINT8                       Index;
    EFI_STATUS                  Status;
    AMI_GET_USER_NAME_REQUEST   GetUserAccessRequest;
    AMI_GET_USER_NAME_RESPONSE  GetUserNameResponse;
    UINT8                       GetUserNameResponseSize;
    CHAR8                       *InvalidUserName = "Invalid";
    CHAR8                       *Reserved = "Reserved";

    DEBUG ((DEBUG_INFO, "IPMI: Inside %a\n", __FUNCTION__));

    // Send Get User Name Command to BMC for all possible User IDs.
    GetUserNameResponseSize = sizeof (GetUserNameResponse);

    for(Index = 0; Index <= gAllowableUser; Index++) {
        gBmcUserDetailsList[Index].UserId = Index;
        if ((Index == 0) || (Index == 1)) {// User Id 0 & 1 are reserved as per IPMI specification.
            CopyMem (
                gBmcUserDetailsList[Index].UserName,
                Reserved,
                AsciiStrnLenS (Reserved, MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER) );
            continue;
        }

        CopyMem (
            gBmcUserDetailsList[Index].UserName,
            InvalidUserName,
            AsciiStrnLenS (InvalidUserName, MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER) );

        GetUserAccessRequest.UserId     = Index;
        GetUserAccessRequest.Reserved   = 0x00;
        GetUserNameResponseSize         = sizeof (GetUserNameResponse);
        ZeroMem (
            GetUserNameResponse.UserName,
            MAX_BMC_USER_NAME_LENGTH );

        Status = gIpmiTransport->SendIpmiCommand (
                                       gIpmiTransport,
                                       IPMI_NETFN_APP,
                                       AMI_BMC_LUN,
                                       IPMI_APP_GET_USER_NAME,
                                       (UINT8 *)&GetUserAccessRequest,
                                       sizeof (GetUserAccessRequest),
                                       (UINT8 *)&GetUserNameResponse,
                                       &GetUserNameResponseSize );
        
        DEBUG ((DEBUG_INFO, "IPMI: Status of Get User Name Command: %r \n", Status));
        DEBUG ((DEBUG_INFO, "IPMI: CompletionCode: %x \n", GetUserNameResponse.CompletionCode));

        if (!EFI_ERROR (Status)) {
            // Copy the user name in User Details Global List.
            if (GetUserNameResponse.UserName[0] != 0x00) {
                CopyMem (
                    gBmcUserDetailsList[Index].UserName,
                    GetUserNameResponse.UserName,
                    sizeof(GetUserNameResponse) );
            }
        }
        DEBUG ((DEBUG_INFO, "IPMI: User Id: %x \n", gBmcUserDetailsList[Index].UserId));
        DEBUG ((DEBUG_INFO, "IPMI: User Name: %s \n", gBmcUserDetailsList[Index].UserName));
    }
}

/** @internal
    Checks the given user name already available in global user list.

    @param[in, out] UserDetails     Contains user name.

    @retval TRUE    User already available.
    @retval FALSE   User not available.

**/
BOOLEAN
CheckUserNameAvailability (
  IN OUT BMC_USER_DETAILS   *UserDetails )
{
    UINT8   Index;

    DEBUG ((DEBUG_INFO, "IPMI: Inside %a\n", __FUNCTION__));
    DEBUG ((DEBUG_INFO, "IPMI: UserName: %s", UserDetails->UserName));

    for (Index = 0; Index <= gAllowableUser; Index++) {
        if (AsciiStrnCmp (
                gBmcUserDetailsList[Index].UserName,
                UserDetails->UserName,
                MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER ) == 0x00) {
            UserDetails->UserId = gBmcUserDetailsList[Index].UserId;
            DEBUG ((DEBUG_INFO, "IPMI: Available UserId: %x", UserDetails->UserId));
            DEBUG ((DEBUG_INFO, "IPMI: Returning TRUE from %a\n", __FUNCTION__));
            return TRUE;
        }
    }

    DEBUG ((DEBUG_INFO, "IPMI: Returning FALSE from %a\n", __FUNCTION__));
    return FALSE;
}

/** @internal
    Checks add new user is possible or not.

    @param VOID     Nothing.

    @retval TRUE    Add new user is possible.
    @retval FALSE   Add new user is not possible.

**/
BOOLEAN
CheckAddNewUserPossibility (
  VOID )
{
    UINT8   Index;
    CHAR8   *InvalidUserName = "Invalid";

    DEBUG ((DEBUG_INFO, "IPMI: Inside %a\n", __FUNCTION__));
    for (Index = 0; Index <= gAllowableUser; Index++) {
        if (AsciiStrnCmp (
                gBmcUserDetailsList[Index].UserName,
                InvalidUserName,
                MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER ) == 0x00) {
            DEBUG ((DEBUG_INFO, "IPMI: Returning TRUE from %a\n", __FUNCTION__));
            return TRUE;
        }
    }
    DEBUG ((DEBUG_INFO, "IPMI: Returning FALSE from %a\n", __FUNCTION__));
    return FALSE;
}

/** @internal
    Send Set User Name command to BMC.

    @param[in] UserId       User Id.
    @param[in] UserName     User Name.

    @return EFI_STATUS  Return status of the functions used.

**/
EFI_STATUS
SendSetUserNameCommandToBmc(
  IN UINT8      UserId,
  IN CHAR8      *UserName )
{
    EFI_STATUS                  Status;
    AMI_SET_USER_NAME_REQUEST   SetUserNameRequest;
    UINT8                       ResponseDataSize;
    UINT8                       CompletionCode;

    DEBUG ((DEBUG_INFO, "IPMI: Inside %a\n", __FUNCTION__));
    DEBUG ((DEBUG_INFO, "IPMI: User Id: %x\n", UserId));
    DEBUG ((DEBUG_INFO, "IPMI: User Name: %a\n", UserName));

    ZeroMem (
       &SetUserNameRequest, 
       sizeof (AMI_SET_USER_NAME_REQUEST) );

    SetUserNameRequest.UserId = UserId;

    DEBUG ((DEBUG_INFO, "IPMI: AsciiStrnLenS(UserName, MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER): %x\n", AsciiStrnLenS(UserName, MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER)));
    CopyMem (
        &SetUserNameRequest.UserName,
        UserName,
        AsciiStrnLenS (UserName, MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER) );

    ResponseDataSize = sizeof (CompletionCode);
    
    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_APP,
                                AMI_BMC_LUN,
                                IPMI_APP_SET_USER_NAME,
                                (UINT8 *)&SetUserNameRequest,
                                sizeof (SetUserNameRequest),
                                &CompletionCode,
                                &ResponseDataSize );
    
    DEBUG ((DEBUG_INFO, "Status of Set User Name Command: %r  CompletionCode: %x\n", Status, CompletionCode));

    return Status; 
}

/** @internal
    Delete user details from global list.

    @param[in] UserName     User Name.

    @retval EFI_SUCCESS     User details successfully deleted from list.
    @retval EFI_NOT_FOUND   User is not available in list.

**/
EFI_STATUS
DeleteUserEntryFromList (
  IN CHAR8      *UserName )
{
    UINT8   Index;
    CHAR8   *InvalidUserName = "Invalid";

    DEBUG ((DEBUG_INFO, "IPMI: Inside %a\n", __FUNCTION__));
    for (Index = 0; Index <= gAllowableUser; Index++) {

        if (AsciiStrnCmp (
                gBmcUserDetailsList[Index].UserName,
                UserName,
                MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER) == 0x00) {
            AsciiStrCpyS (
                gBmcUserDetailsList[Index].UserName,
                MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER,
                InvalidUserName );
            DEBUG ((DEBUG_INFO, "IPMI: gBmcUserDetailsList[%x].UserName: %s\n", Index, gBmcUserDetailsList[Index].UserName));
            return EFI_SUCCESS;
        }

    }
    DEBUG ((DEBUG_INFO, "IPMI: Returning from %a\n", __FUNCTION__));
    return EFI_NOT_FOUND;
}

/** @internal
    Add user details to global list.

    @param[in] UserDetails  User details.

    @return VOID    Nothing.

**/
VOID
AddUserEntryToList (
  IN BMC_USER_DETAILS      *UserDetails )
{
    DEBUG ((DEBUG_INFO, "IPMI: Inside %a\n", __FUNCTION__));

    gBmcUserDetailsList[UserDetails->UserId].UserId = UserDetails->UserId;
    CopyMem (
        gBmcUserDetailsList[UserDetails->UserId].UserName,
        UserDetails->UserName,
        sizeof(UserDetails->UserName) );

    DEBUG ((DEBUG_INFO, "IPMI: gBmcUserDetailsList[UserDetails->UserId].UserId:%x\n", gBmcUserDetailsList[UserDetails->UserId].UserId));
    DEBUG ((DEBUG_INFO, "IPMI: gBmcUserDetailsList[UserDetails->UserId].UserName:%s\n", gBmcUserDetailsList[UserDetails->UserId].UserName));
    DEBUG ((DEBUG_INFO, "IPMI: Returning from %a\n", __FUNCTION__));
}

/** @internal
    Parse the global list and get the available used id for new user.

    @param[out] UserId  Pointer to User Id.

    @retval EFI_SUCCESS     User id is available and updated.
    @retval EFI_NOT_FOUND   No more user id is available.

**/
EFI_STATUS
GetAvailableUserId (
  OUT UINT8     *UserId )
{
    UINT8   Index;
    CHAR8   *InvalidUserName = "Invalid";

    DEBUG ((DEBUG_INFO, "IPMI: Inside %a\n", __FUNCTION__));

    for (Index = 0; Index <= gAllowableUser; Index++){
        if (AsciiStrnCmp (
                gBmcUserDetailsList[Index].UserName,
                InvalidUserName,
                MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER) == 0x00) {
            *UserId = gBmcUserDetailsList[Index].UserId;
            DEBUG ((DEBUG_INFO,"IPMI: User Id: %x", *UserId));
            DEBUG ((DEBUG_INFO,"IPMI: Returning EFI_SUCCESS from %a\n", __FUNCTION__));
            return EFI_SUCCESS;
        }
    }
    DEBUG ((DEBUG_ERROR, "IPMI: Returning EFI_NOT_FOUND from %a\n", __FUNCTION__));
    return  EFI_NOT_FOUND;
}

/** @internal
    Displays error message pop up in setup.

    @param[in] HiiHandle    Handle to HII database.
    @param[in] Message      Error message token value.

    @return VOID    Nothing.

**/
VOID
DisplayErrorMessagePopUp (
  IN EFI_HII_HANDLE     HiiHandle,
  IN UINT16             Message )
{
    UINT8           MsgBoxSel;
    EFI_STRING      CharPtrString;
    EFI_STRING      MessagePtrString;
    
    CharPtrString = HiiGetString (
                        HiiHandle,
                        STRING_TOKEN(STR_BMC_USER_SETTINGS_INFO),
                        NULL);

    MessagePtrString = HiiGetString (
                            HiiHandle,
                            Message,
                            NULL);
    
    if ((CharPtrString != NULL) && (MessagePtrString != NULL)) {
        gAmiPostMgr->DisplayMsgBox (
                        CharPtrString,
                        MessagePtrString,
                        MSGBOX_TYPE_OK,
                        &MsgBoxSel );
    }
}

/** @internal
    Send Set user password command as per input data.

    @param[in] UserId           User Id.
    @param[in] PasswordSize     Password size(16 or 20 byte).
    @param[in] Operation        Tells what operation to perform.
    @param[in] UserPassword     Password.

    @retval EFI_INVALID_PARAMETER   Error completion code.
    @retval Others                  Status returned of function calls used.

**/
EFI_STATUS
PerformGivenUserPasswordAction (
  IN UINT8      UserId,
  IN UINT8      PasswordSize,
  IN UINT8      Operation,
  IN CHAR8      *UserPassword )
{
    AMI_SET_USER_PASSWORD_REQUEST   SetUserPassword;
    EFI_STATUS                      Status;
    UINT8                           ResponseDataSize;
    UINT8                           CompletionCode;

    DEBUG ((DEBUG_INFO, "IPMI: Inside %a\n", __FUNCTION__));

    ZeroMem (
        &SetUserPassword,
        sizeof (AMI_SET_USER_PASSWORD_REQUEST) );
    
    CopyMem (
        SetUserPassword.PasswordData,
        UserPassword,
        AsciiStrnLenS(UserPassword, MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER) );

    DEBUG ((DEBUG_INFO, "IPMI: UserId: %x\n", UserId));
    DEBUG ((DEBUG_INFO, "IPMI: UserId: %x  IPMI: IsPasswordSize20: %x\n",UserId, PasswordSize));
    DEBUG ((DEBUG_INFO, "IPMI: Operation: %x\n", Operation));

    SetUserPassword.UserId          = UserId;
    SetUserPassword.PasswordSize    = PasswordSize;
    SetUserPassword.Operation       = Operation;
    ResponseDataSize                = sizeof (CompletionCode);

    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_APP,
                                AMI_BMC_LUN,
                                IPMI_APP_SET_USER_PASSWORD,
                                (UINT8 *)&SetUserPassword,
                                sizeof (SetUserPassword),
                                &CompletionCode,
                                &ResponseDataSize );
    
    DEBUG ((DEBUG_INFO, "IPMI: Set User Password Command Status: %r CompletionCode: %x \n", Status, CompletionCode));

    if((!EFI_ERROR(Status)) && (CompletionCode == PASSWORD_DOES_NOT_MATCH_CC)) {
        return EFI_INVALID_PARAMETER;
    }
    return Status;
}

/** @internal
    Deletes user name from BMC.

    @param[in] UserId   User Id.

    @return EFI_STATUS  Return status of function calls used.

**/
EFI_STATUS
DeleteUserNameFromBmc (
  IN UINT8  UserId )
{
    EFI_STATUS  Status;
    CHAR8       DeleteUserName[MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER];

    DEBUG ((DEBUG_INFO, "IPMI: Inside %a\n", __FUNCTION__));
    DEBUG ((DEBUG_INFO, "IPMI: User Id: %x\n", UserId));

    SetMem (
        DeleteUserName,
        MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER,
        IPMI_BMC_DELETE_USER_DATA );
    
    Status = SendSetUserNameCommandToBmc (
                UserId,
                DeleteUserName );

    DEBUG ((DEBUG_INFO, "IPMI: Set User Name Command Status: %r\n", Status));
    DEBUG ((DEBUG_INFO, "IPMI: Returning from %a\n", __FUNCTION__));
    return Status;
}

/** @internal
    Displays text box in setup to get user password.

    @param[in] HiiHandle        Pointer to the HII Handle.
    @param[in] TitleToken       Display title token.
    @param[in] UserPassword     Pointer to a User password string.

    @return VOID    Nothing.

**/
VOID
DisplayTextBox (
  IN VOID       *HiiHandle,
  IN UINT16     TitleToken,
  IN CHAR16     **UserPassword )
{
    EFI_STATUS              Status;
    TEXT_INPUT_TEMPLATE     BmcUserPasswordPrompt;

    DEBUG ((DEBUG_INFO, "IPMI: Inside %a\n", __FUNCTION__));

    // Fill the Text input template.
    BmcUserPasswordPrompt.ItemToken     = STR_BMC_USER_PASSWORD_ADD;
    BmcUserPasswordPrompt.MaxSize       = BMC_USER_PASSWORD_MAX_SIZE;
    BmcUserPasswordPrompt.FilterKey     = TSE_FILTER_KEY_NULL;
    BmcUserPasswordPrompt.Hidden        = TRUE;
    BmcUserPasswordPrompt.Value         = *UserPassword;

    /* Call DisplayTextBox() function to display the text box in setup and
       get the Secret string.*/
    Status = gAmiPostMgr->DisplayTextBox (
                            HiiHandle,
                            TitleToken,
                            &BmcUserPasswordPrompt,
                            1,
                            NULL );

    DEBUG ((DEBUG_INFO, "IPMI: Status of DisplayTextBox: %r\n", Status));
    DEBUG ((DEBUG_INFO, "IPMI: Returning from %a\n", __FUNCTION__));
}

/**
    This function Set User Access with ChannelNo,UserId&UserPrivilegeLimit.

    @param [in] ChannelNo - A channel number to giver user access.
    @param [in] UserId - Id of the user to provide user access.
    @param [in] UserPrivilegeLimit - Privilege to be set for the user.

    @return EFI_STATUS  Return status of function calls used.
**/
EFI_STATUS
SetUserAccessCommand(
   IN  UINT8 ChannelNo,
   IN  UINT8 UserId,
   IN  UINT8 UserPrivilegeLimit )
{
    EFI_STATUS                          Status = EFI_SUCCESS;
    AMI_SET_USER_ACCESS_REQUEST         SetUserAccess;
    UINT8                               ResponseDataSize;
    UINT8                               CompletionCode;

    ZeroMem (
        &SetUserAccess,
        sizeof (AMI_SET_USER_ACCESS_REQUEST) );

    SetUserAccess.ChannelNo = ChannelNo;
    SetUserAccess.EnableIpmiMessaging = 1;
    SetUserAccess.EnableUserLinkAuthetication = 1;
    SetUserAccess.UserRestrictedToCallback = 0;
    SetUserAccess.EnableChangeBitsInByte1 = 1;
    SetUserAccess.UserID = UserId;
    DEBUG ((DEBUG_INFO, "IPMI: SetUserAccess.UserID = %x\n",SetUserAccess.UserID));
    SetUserAccess.UserPrivilegeLimit = UserPrivilegeLimit;
    DEBUG ((DEBUG_INFO, "IPMI: SetUserAccess.UserPrivilegeLimit = %x\n",SetUserAccess.UserPrivilegeLimit));
    SetUserAccess.UserSimultaneousSessionLimit = 0;

    ResponseDataSize = sizeof(CompletionCode);
    Status = gIpmiTransport->SendIpmiCommand (
                                 gIpmiTransport,
                                 IPMI_NETFN_APP,
                                 AMI_BMC_LUN,
                                 IPMI_APP_SET_USER_ACCESS,
                                 (UINT8 *)&SetUserAccess,
                                 sizeof(SetUserAccess),
                                 &CompletionCode,
                                 &ResponseDataSize );
    DEBUG ((DEBUG_INFO, "IPMI:CommandCompletionCode: %x   Status of IPMI_APP_SET_USER_ACCESS Command: %r \n", CompletionCode, Status));

    return Status;
}

/**
    This function collects all BMC user details from BMC.

    @param[in] HiiHandle    Handle to HII database.
    @param[in] Class        Formset Class.
    @param[in] SubClass     Formset Subclass.
    @param[in] Key          Formset Key.

    @retval EFI_UNSUPPORTED     Invalid key or error in locating protocol.
    @retval EFI_SUCCESS         Successful completion of the function.

**/
EFI_STATUS
BmcUserSettingsCallbackFunction (
  IN EFI_HII_HANDLE     HiiHandle,
  IN UINT16             Class,
  IN UINT16             SubClass,
  IN UINT16             Key )
{
    EFI_STATUS              Status = EFI_SUCCESS;
    CALLBACK_PARAMETERS     *CallbackParameters;
    STATIC BOOLEAN          IsFirstTimeCallBack = TRUE;
    UINT8                   ChannelNumberBuffer[BMC_LAN_COUNT];
    UINT8                   ChannelCount = BMC_LAN_COUNT;

    DEBUG ((DEBUG_INFO, "%a Entry... Key: %x\n", __FUNCTION__, Key));

    // Continue only for BMC_USER SETTING_KEY.
    if (Key != BMC_USER_SETTINGS_KEY) {
        DEBUG ((DEBUG_ERROR, "IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    // Get the call back parameters and verify the action.
    CallbackParameters = GetCallbackParameters ();
    if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGING) {
        DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGING... so return EFI_SUCCESS\n"));
        return EFI_SUCCESS;
    }

    DEBUG ((DEBUG_INFO, "IPMI: IsFirstTimeCallBack: %x\n", IsFirstTimeCallBack));
    if (COLLECT_BMC_USER_DETAILS_FOR_EVERY_BMC_USER_SETTING_CALLBACK == 0) {
        if (IsFirstTimeCallBack == FALSE) {
            return EFI_SUCCESS;
        }
    }

    // Locate AmiPostMgrProtocol to display the message boxes in setup.
    Status = gBS->LocateProtocol (
                    &gAmiPostManagerProtocolGuid,
                    NULL,
                    (VOID **)&gAmiPostMgr );
    
    DEBUG ((DEBUG_INFO, "IPMI: gAmiPostManagerProtocolGuid locateProtocol Status: %r\n", Status));

    if (EFI_ERROR (Status)) {
        return EFI_UNSUPPORTED;
    }

    // Locate IpmiTransport protocol.
    Status = gBS->LocateProtocol (
                    &gEfiDxeIpmiTransportProtocolGuid,
                    NULL,
                    (VOID **)&gIpmiTransport );

    DEBUG ((DEBUG_INFO, " IPMI: gEfiDxeIpmiTransportProtocolGuid Status: %r \n", Status));

    // If locate protocol fails, display error message and return.
    if (EFI_ERROR (Status)) {
        DisplayErrorMessagePopUp (
            HiiHandle,
            STRING_TOKEN(STR_IPMI_PROTOCOL_ERROR) );
        return EFI_UNSUPPORTED;
    }
    
    // Collect BMC LAN Channels
    gPcdLanChannelBufferPtr = PcdGetPtr(AmiPcdLanChannelPtr);
    gPcdSize = (UINT8)PcdGetSize(AmiPcdLanChannelPtr);
    if (gPcdLanChannelBufferPtr[0] == 0) {
        DEBUG ((DEBUG_INFO, "IPMI: PcdLanChannelPtr is NULL\n"));
        Status = AmiIpmiGetLanChannelNumber (
                       gIpmiTransport, 
                       ChannelNumberBuffer,
                       &ChannelCount );

        DEBUG ((DEBUG_INFO, "IPMI: GetLanChannelNumber Status:%r \n",Status));
        if ((EFI_ERROR (Status)) || (gPcdLanChannelBufferPtr[0] == 0)) {
             DisplayErrorMessagePopUp ( 
                      HiiHandle, 
                      STRING_TOKEN(STR_VALID_BMC_LAN_CHANNEL_NOT_FOUND) );\

             return  EFI_UNSUPPORTED;
        }
    }
    // Collect maximum user allowable.
    GetMaxUserAllowable ();

    if (IsFirstTimeCallBack) {
        CollectAllBmcUserDetails ();
    } else {
        if (COLLECT_BMC_USER_DETAILS_FOR_EVERY_BMC_USER_SETTING_CALLBACK) {
            CollectAllBmcUserDetails ();
        }
    }

    IsFirstTimeCallBack = FALSE;

    DEBUG ((DEBUG_INFO, "IPMI: Returning from %a\n", __FUNCTION__));
    return EFI_SUCCESS;
}

/**
    This function validates whether add new user is possible or not.

    @param[in] HiiHandle    Handle to HII database.
    @param[in] Class        Formset Class.
    @param[in] SubClass     Formset Subclass.
    @param[in] Key          Formset Key.

    @retval EFI_UNSUPPORTED     Invalid key or error occurs in function
                                calls used.
    @retval EFI_SUCCESS         Successful completion of the function.

**/
EFI_STATUS
BmcUserSettingsAddUserCallbackFunction (
  IN EFI_HII_HANDLE     HiiHandle,
  IN UINT16             Class,
  IN UINT16             SubClass,
  IN UINT16             Key )
{
    BOOLEAN                            BrowserDataStatus;
    CALLBACK_PARAMETERS                *CallbackParameters;
    BOOLEAN                            IsAddNewUserPossible;
    SERVER_MGMT_CONFIGURATION_DATA     ServerMgmtConfiguration ;
    UINTN                              SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);

    DEBUG ((DEBUG_INFO, "%a Entry... Key: %x\n", __FUNCTION__, Key));

    // Continue only for BMC_ADD USER KEY.
    if (Key != ADD_BMC_USER_KEY) {
        DEBUG ((DEBUG_ERROR, "IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    // Get the call back parameters and verify the action.
    CallbackParameters = GetCallbackParameters ();
    DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action: %x\n", CallbackParameters->Action));
    if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGING) {
        DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGING... so return EFI_SUCCESS\n"));
        return EFI_SUCCESS;
    }

    IsAddNewUserPossible = CheckAddNewUserPossibility ();

    if (IsAddNewUserPossible == FALSE) {
        DEBUG ((DEBUG_ERROR, "IPMI: Add New User Not Possible!!!\n"));
        DisplayErrorMessagePopUp (
            HiiHandle,
            STRING_TOKEN(STR_NEW_USER_ADD_ERROR) );
        *(CallbackParameters->ActionRequest) = EFI_BROWSER_ACTION_REQUEST_FORM_DISCARD_EXIT;
        return EFI_SUCCESS;
    }

    BrowserDataStatus = HiiGetBrowserData (
                            &gEfiServerMgmtSetupVariableGuid,
                            L"ServerSetup",
                            SelectionBufferSize,
                            (UINT8*)&ServerMgmtConfiguration );
   if (!BrowserDataStatus) {
       DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
       return EFI_UNSUPPORTED;
    }

    ZeroMem (
        ServerMgmtConfiguration.AddBmcUserName,
        MAX_BMC_USER_NAME_LENGTH );

    ServerMgmtConfiguration.AddBmcUserPrivilegeLimit  = 0;
    ServerMgmtConfiguration.AddBmcUserValidBmcUserCredentials = 0;
    ServerMgmtConfiguration.AddBmcUserValidBmcUser = 0;
    ServerMgmtConfiguration.AddBmcUserValidBmcLanChannel = 0;
    ServerMgmtConfiguration.AddBmcChannelNo = 0;
    ServerMgmtConfiguration.AddBmcUserAccess = 0;

    // Set Browser data.
    BrowserDataStatus = HiiSetBrowserData (
                            &gEfiServerMgmtSetupVariableGuid, 
                            L"ServerSetup",
                            SelectionBufferSize,
                            (UINT8*)&ServerMgmtConfiguration,
                            NULL );
    DEBUG ((DEBUG_INFO, "IPMI: Status of HiiSetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }

    DEBUG ((DEBUG_INFO, "IPMI: Returning from %a\n", __FUNCTION__));
    return EFI_SUCCESS;
}

/**
    This function send set user name command to BMC.

    @param[in] HiiHandle    Handle to HII database.
    @param[in] Class        Formset Class.
    @param[in] SubClass     Formset Subclass.
    @param[in] Key          Formset Key.

    @retval EFI_UNSUPPORTED     Invalid key.
    @retval EFI_SUCCESS         Successful completion of the function.
    @retval Others              Error status of function calls used.

**/
EFI_STATUS
BmcAddUserNameCallbackFunction (
  IN EFI_HII_HANDLE     HiiHandle,
  IN UINT16             Class,
  IN UINT16             SubClass,
  IN UINT16             Key )
{
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration ;
    BOOLEAN                             BrowserDataStatus;
    EFI_STATUS                          Status = EFI_SUCCESS;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    BOOLEAN                             UserNameAlreadyAvailable;
    CALLBACK_PARAMETERS                 *CallbackParameters;
    CHAR16                              NameBuffer[MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER];

    DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    // Check for the key and Return if Key value does not match.
    if (Key != ADD_BMC_USER_NAME_KEY) {
        DEBUG ((DEBUG_ERROR,"IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    // Get the call back parameters and verify the action.
    CallbackParameters = GetCallbackParameters ();
    if (CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
        DEBUG ((DEBUG_ERROR,"IPMI: CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD... so return EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    } else if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) {
        DEBUG ((DEBUG_INFO,"IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return EFI_SUCCESS;
    }
    BrowserDataStatus = HiiGetBrowserData (
                            &gEfiServerMgmtSetupVariableGuid,
                            L"ServerSetup",
                            SelectionBufferSize,
                            (UINT8*)&ServerMgmtConfiguration );
    if (!BrowserDataStatus) {
        DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
        return EFI_UNSUPPORTED;
    }

    // Convert User name format from UNICODE to ASCII.
    ZeroMem (
        gBmcAddUserDetails.UserName,
        MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER );
    
    ZeroMem (
        NameBuffer,
        MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER);

    CopyMem (
        NameBuffer,
        ServerMgmtConfiguration.AddBmcUserName,
        sizeof(ServerMgmtConfiguration.AddBmcUserName) );

    UnicodeStrToAsciiStrS (
        NameBuffer,
        gBmcAddUserDetails.UserName,
        MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER );
    DEBUG ((DEBUG_INFO, "IPMI: AddUserName: %a\n", gBmcAddUserDetails.UserName));

    // Check the user name is already available in User Details list.
    UserNameAlreadyAvailable = CheckUserNameAvailability (&gBmcAddUserDetails);
    DEBUG ((DEBUG_INFO, "IPMI: UserNameAlreadyAvailable:%x\n", UserNameAlreadyAvailable));

    if (UserNameAlreadyAvailable) {
        // Display User Name already available pop up message.
        DisplayErrorMessagePopUp (
            HiiHandle,
            STRING_TOKEN(STR_USER_NAME_EXISTS) );
        
        ZeroMem (
            ServerMgmtConfiguration.AddBmcUserName,
            MAX_BMC_USER_NAME_LENGTH );
        
        ZeroMem (
            &gBmcAddUserDetails,
            sizeof (gBmcAddUserDetails.UserName) );

        ServerMgmtConfiguration.AddBmcChannelNo            = 0; 
        ServerMgmtConfiguration.AddBmcUserPrivilegeLimit   = 0; 
    } else {
        Status = GetAvailableUserId (&(gBmcAddUserDetails.UserId));
        DEBUG ((DEBUG_INFO, "IPMI: Status of GetAvailableUserId Function = %r\n", Status));
        
        if (EFI_ERROR (Status)) {
            DisplayErrorMessagePopUp (
                HiiHandle,
                STRING_TOKEN(STR_USER_ADD_NOT_POSSIBLE) );
        } else {
            // Send Set User Name Command.
            Status = SendSetUserNameCommandToBmc (
                        gBmcAddUserDetails.UserId,
                        gBmcAddUserDetails.UserName );
            DEBUG ((DEBUG_INFO, "IPMI: Set User Name Command Status: %r\n", Status));
            
            if (EFI_ERROR (Status)){
                DisplayErrorMessagePopUp (
                    HiiHandle,
                    STRING_TOKEN(STR_SET_USER_NAME_CMD_FAIL) );
            } else {
                ServerMgmtConfiguration.AddBmcUserValidBmcUser = 1;
            }
        }
        if (EFI_ERROR (Status)) {
            ZeroMem (
                ServerMgmtConfiguration.AddBmcUserName,
                MAX_BMC_USER_NAME_LENGTH );

            ZeroMem (
                &gBmcAddUserDetails,
                sizeof (BMC_USER_DETAILS) );

            ServerMgmtConfiguration.AddBmcChannelNo = 0;
            ServerMgmtConfiguration.AddBmcUserPrivilegeLimit = 0;
        }
    }

    ServerMgmtConfiguration.AddBmcUserPrivilegeLimit = 0;
    ServerMgmtConfiguration.AddBmcUserValidBmcUserCredentials = 0;
    ServerMgmtConfiguration.AddBmcUserValidBmcLanChannel = 0;
    ServerMgmtConfiguration.AddBmcChannelNo = 0;
    ServerMgmtConfiguration.AddBmcUserAccess = 0;

    // Set Browser data.
    BrowserDataStatus = HiiSetBrowserData (
                            &gEfiServerMgmtSetupVariableGuid,
                            L"ServerSetup",
                            SelectionBufferSize,
                            (UINT8*)&ServerMgmtConfiguration,
                            NULL );
    DEBUG ((DEBUG_INFO, "IPMI: Status of HiiSetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }
    return EFI_SUCCESS;
}

/**
    This function adds user password to BMC.

    @param[in] HiiHandle    Handle to HII database.
    @param[in] Class        Formset Class.
    @param[in] SubClass     Formset Subclass.
    @param[in] Key          Formset Key.

    @retval EFI_UNSUPPORTED     Invalid key.
    @retval EFI_SUCCESS         Successful completion of the function.
    @retval Others              Error status of functions calls used.

**/
EFI_STATUS
BmcAddUserPasswordCallbackFunction (
  IN EFI_HII_HANDLE     HiiHandle,
  IN UINT16             Class,
  IN UINT16             SubClass,
  IN UINT16             Key )
{
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration ;
    EFI_STATUS                          Status = EFI_SUCCESS;
    BOOLEAN                             BrowserDataStatus;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    CALLBACK_PARAMETERS                 *CallbackParameters;
    CHAR16                              PasswordBuffer[MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER];

    DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    // Check for the key and Return if Key value does not match.
    if (Key != ADD_BMC_USER_PASSWORD_KEY) {
        DEBUG ((DEBUG_ERROR,"IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    // Get the call back parameters and verify the action.
    CallbackParameters = GetCallbackParameters ();
    if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) {
        DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return EFI_SUCCESS;
    }

    // Get Browser data.
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration );
    if (!BrowserDataStatus) {
        DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
        return EFI_UNSUPPORTED;
    }

    // Return EFI_SUCCESS if the password is NULL.
    if (ServerMgmtConfiguration.AddBmcUserPassword[0] == '\0') {
        DEBUG ((DEBUG_INFO, "IPMI: Add Bmc User Password is NULL\n"));
        return EFI_SUCCESS;
    }

    // Convert User Password format from UNICODE to ASCII.
    ZeroMem (
        gBmcAddUserDetails.UserPassword,
        MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER );
    
    ZeroMem (
        PasswordBuffer,
        MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER );
    CopyMem (
        PasswordBuffer,
        ServerMgmtConfiguration.AddBmcUserPassword,
        sizeof(ServerMgmtConfiguration.AddBmcUserPassword) );

    UnicodeStrToAsciiStrS (
        PasswordBuffer,
        gBmcAddUserDetails.UserPassword,
        MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER );

    Status = PerformGivenUserPasswordAction (
                gBmcAddUserDetails.UserId,
                AmiSetPasswordSizeTwentyByte,
                AmiSetUserPasswordOperationSetPassword,
                (CHAR8 *)gBmcAddUserDetails.UserPassword );
    
    if(EFI_ERROR (Status)) {
        // Display User Name already available pop up message.
        DisplayErrorMessagePopUp (
            HiiHandle,
            STRING_TOKEN(STR_SET_USER_PASSWORD_FAIL) );
        
        ZeroMem (
            ServerMgmtConfiguration.AddBmcUserPassword,
            MAX_BMC_USER_PASSWORD_LENGTH );
        
        ZeroMem (
            &gBmcAddUserDetails.UserPassword,
            sizeof (gBmcAddUserDetails.UserPassword) );
    } else {
        AddUserEntryToList (&gBmcAddUserDetails);
        ZeroMem (
            ServerMgmtConfiguration.AddBmcUserPassword,
            MAX_BMC_USER_PASSWORD_LENGTH ); //Clearing the password from browser data.
 
        ServerMgmtConfiguration.AddBmcUserValidBmcUserCredentials = 1;
        // Setting user with default channel and Access
        Status = PerformGivenUserPasswordAction (
                    gBmcAddUserDetails.UserId,
                    AmiSetPasswordSizeTwentyByte,
                    AmiSetUserPasswordOperationDisableUser,
                    gBmcAddUserDetails.UserPassword);

        DEBUG ((DEBUG_INFO, "IPMI: PerformGivenUserPasswordAction disable Access Status= %r\n",Status ));
        if (gPcdLanChannelBufferPtr[0] != 0) {
            Status = SetUserAccessCommand(
                        (UINT8)gPcdLanChannelBufferPtr[0],
                        (UINT8)gBmcAddUserDetails.UserId,
                        USER_PRIVILEGE_NO_ACCESS);

            DEBUG ((DEBUG_INFO, "IPMI: SetUserAccessCommand Status= %r\n",Status ));
            if (EFI_ERROR (Status)) {
                DisplayErrorMessagePopUp ( 
                        HiiHandle,
                        STRING_TOKEN(STR_SET_USER_ACCESS_CMD_FAIL) );

                return EFI_UNSUPPORTED;
            }
        }
        DisplayErrorMessagePopUp ( 
                HiiHandle,
                STRING_TOKEN(STR_ADD_USERNAME_OPERATION_PASSED) );
    }

    ServerMgmtConfiguration.AddBmcChannelNo = 0;

    // Set Browser data.
    BrowserDataStatus = HiiSetBrowserData (
                            &gEfiServerMgmtSetupVariableGuid,
                            L"ServerSetup",
                            SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration,
                            NULL );
    if (!BrowserDataStatus) {
        DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiSetBrowserData() = %d\n", BrowserDataStatus));
        return EFI_UNSUPPORTED;
        }
    DEBUG ((DEBUG_INFO, "IPMI: Returning from %a\n", __FUNCTION__));
    return EFI_SUCCESS;
}
/**
    This function enables/disables a user in BMC as per given input.

    @param [in] HiiHandle - A handle that was previously registered in the
                               HII Database.
    @param [in] Class - Class of the Form Callback Protocol passed in
    @param [in] SubClass - Sub Class of the Form Callback Protocol passed in
    @param [in] Key - Key of the Form Callback Protocol passed in

    @retval EFI_UNSUPPORTED     Invalid key.
    @retval EFI_SUCCESS         Successful completion of the function.
    @retval Others              Error status of functions calls used.
**/
EFI_STATUS
BmcAddUserAccessCallbackFunction(
  IN  EFI_HII_HANDLE    HiiHandle,
  IN  UINT16            Class,
  IN  UINT16            SubClass,
  IN  UINT16            Key )
{
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration ;
    BOOLEAN                             BrowserDataStatus;
    EFI_STATUS                          Status = EFI_SUCCESS;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    CALLBACK_PARAMETERS                 *CallbackParameters;

    DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    //
    // Check for the key and Return if Key value does not match
    //
    if ( Key != ADD_BMC_USER_ACCESS_KEY ) {
        DEBUG ((DEBUG_ERROR, "IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    //
    // Get the call back parameters and verify the action
    //
    CallbackParameters = GetCallbackParameters();
    if (CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
        DEBUG ((DEBUG_ERROR, "IPMI: CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD... so return EFI_UNSUPPORTED\n"));
        return  EFI_UNSUPPORTED;
    } else if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) {
        DEBUG ((DEBUG_INFO, "\n CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return  EFI_SUCCESS;
    }
    //
    // Get Browser DATA
    //
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration );
    DEBUG ((DEBUG_INFO, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }

    DEBUG ((DEBUG_INFO, "\n ServerMgmtConfiguration.ChangeUserSettingsBmcUser: %x\n", ServerMgmtConfiguration.AddBmcUserAccess));
    DEBUG ((DEBUG_INFO, "IPMI:gBmcChangeUserDetails.UserId %x\n", gBmcAddUserDetails.UserId));
    Status = PerformGivenUserPasswordAction(
                gBmcAddUserDetails.UserId,
                AmiSetPasswordSizeTwentyByte,
                ServerMgmtConfiguration.AddBmcUserAccess,
                gBmcAddUserDetails.UserPassword);

    DEBUG ((DEBUG_INFO, "IPMI: User Enable/Disable Status: %r\n", Status));
    if (EFI_ERROR (Status)) {
        if (ServerMgmtConfiguration.AddBmcUserAccess) {
            DisplayErrorMessagePopUp (
                    HiiHandle,
                    STRING_TOKEN(STR_USER_ENABLE_OPERATION_FAIL) );
        } else {
            DisplayErrorMessagePopUp (
                    HiiHandle,
                    STRING_TOKEN(STR_USER_DISABLE_OPERATION_FAIL) );
        }
        ServerMgmtConfiguration.AddBmcUserAccess = 0;
        //
        // Set Browser DATA
        //
        BrowserDataStatus = HiiSetBrowserData (
                                 &gEfiServerMgmtSetupVariableGuid,
                                 L"ServerSetup",
                                 SelectionBufferSize,
                                 (UINT8*)&ServerMgmtConfiguration,
                                  NULL );

        if (!BrowserDataStatus) {
            DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiSetBrowserData() = %d\n",BrowserDataStatus));
            return EFI_UNSUPPORTED;
        }
    } else {
        if (ServerMgmtConfiguration.AddBmcUserAccess) {
            DisplayErrorMessagePopUp (
                    HiiHandle,
                    STRING_TOKEN(STR_USER_ENABLE_OPERATION_PASS) );
        } else {
           DisplayErrorMessagePopUp (
                   HiiHandle,
                   STRING_TOKEN(STR_USER_DISABLE_OPERATION_PASS) );
        }
    }
    DEBUG ((DEBUG_INFO, "%a Exit... \n", __FUNCTION__ ));
    return EFI_SUCCESS;
}
/**
    This function sends get channel access command to BMC for given input.

    @param[in] HiiHandle    Handle to HII database.
    @param[in] Class        Formset Class.
    @param[in] SubClass     Formset Subclass.
    @param[in] Key          Formset Key.

    @retval EFI_UNSUPPORTED     Invalid key.
    @retval EFI_SUCCESS         Successful completion of the function.
    @retval Others              Error status of function calls used.

**/
EFI_STATUS
BmcAddUserChannelCallbackFunction (
  IN EFI_HII_HANDLE     HiiHandle,
  IN UINT16             Class,
  IN UINT16             SubClass,
  IN UINT16             Key )
{
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration;
    EFI_STATUS                          Status = EFI_SUCCESS;
    BOOLEAN                             BrowserDataStatus;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    CALLBACK_PARAMETERS                 *CallbackParameters;
    IPMI_GET_CHANNEL_ACCESS_REQUEST     GetChannelAccessCommand;
    IPMI_GET_CHANNEL_ACCESS_RESPONSE    GetChannelAccessResponse;
    UINT8                               GetChannelAccessResponseDataSize;
    UINT8                               Index;

    DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    // Check for the key and Return if Key value does not match.
    if (Key != ADD_BMC_USER_CHANNEL_NO_KEY) {
        DEBUG ((DEBUG_ERROR,"IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    // Get the call back parameters and verify the action.
    CallbackParameters = GetCallbackParameters ();
    if (CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
        DEBUG ((DEBUG_ERROR,"IPMI: CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD... so return EFI_UNSUPPORTED\n"));
        return  EFI_UNSUPPORTED;
    } else if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) {
        DEBUG ((DEBUG_INFO,"IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return EFI_SUCCESS;
    }

    // Get Browser data.
    BrowserDataStatus = HiiGetBrowserData (
                             &gEfiServerMgmtSetupVariableGuid,
                             L"ServerSetup",
                             SelectionBufferSize,
                             (UINT8*)&ServerMgmtConfiguration );

       if (!BrowserDataStatus) {
           DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
           return EFI_UNSUPPORTED;
       }
    DEBUG ((DEBUG_INFO, "IPMI: User entered channel no = %x\n", ServerMgmtConfiguration.AddBmcChannelNo));
    if (gPcdLanChannelBufferPtr[0] == 0) {
        return EFI_UNSUPPORTED; 
    }
    for (Index=0;Index<gPcdSize;Index++) {
            if ((gPcdLanChannelBufferPtr[Index] == ServerMgmtConfiguration.AddBmcChannelNo)) {
                 GetChannelAccessCommand.ChannelNumber.Bits.ChannelNo   = ServerMgmtConfiguration.AddBmcChannelNo;
                 GetChannelAccessCommand.AccessType.Bits.MemoryType     = AmiChannelAccessTypeNonVolatile;  // Non Volatile channel Access
                 GetChannelAccessCommand.ChannelNumber.Bits.Reserved    = 0x00;
                 GetChannelAccessCommand.AccessType.Bits.Reserved       = 0x00;
                 GetChannelAccessResponseDataSize                       = sizeof (GetChannelAccessResponse);

                 Status = gIpmiTransport->SendIpmiCommand (
                                             gIpmiTransport,
                                             IPMI_NETFN_APP,
                                             AMI_BMC_LUN,
                                             IPMI_APP_GET_CHANNEL_ACCESS,
                                             (UINT8 *)&GetChannelAccessCommand,
                                             sizeof (GetChannelAccessCommand),
                                             (UINT8 *)&GetChannelAccessResponse,
                                             &GetChannelAccessResponseDataSize );

                 DEBUG ((DEBUG_INFO, "IPMI: CompletionCode: %x \n", GetChannelAccessResponse.CompletionCode));
                 DEBUG ((DEBUG_INFO, "IPMI: Status of IPMI_APP_GET_CHANNEL_ACCESS Command: %r \n", Status));
                 DEBUG ((DEBUG_INFO, "IPMI: GetChannelAccessResponse.ChannelPriviledgeLimit: %x \n", GetChannelAccessResponse.PrivilegeLimit.Bits.ChannelPriviledgeLimit));
                 ServerMgmtConfiguration.AddBmcUserMaximumPossibleUserPrivilegeLevel = GetChannelAccessResponse.PrivilegeLimit.Bits.ChannelPriviledgeLimit;

                 break;
            }
    }
    if ((EFI_ERROR (Status)) || \
        ((GetChannelAccessResponse.CompletionCode != COMPLETION_CODE_SUCCESS ) || (Index == gPcdSize))) {
        DisplayErrorMessagePopUp (
            HiiHandle,
            STRING_TOKEN(STR_GET_CHANNEL_ACCESS_CMD_FAIL) );

        ServerMgmtConfiguration.AddBmcUserPrivilegeLimit  = 0;
        ServerMgmtConfiguration.AddBmcUserMaximumPossibleUserPrivilegeLevel =0;
        ServerMgmtConfiguration.AddBmcChannelNo = 0;
    } else {
        ServerMgmtConfiguration.AddBmcUserValidBmcLanChannel = 1;
        Status = SetUserAccessCommand((UINT8)ServerMgmtConfiguration.AddBmcChannelNo,(UINT8)gBmcAddUserDetails.UserId,USER_PRIVILEGE_NO_ACCESS);
        DEBUG ((DEBUG_INFO, "IPMI: SetUserAccessCommand Status= %r\n",Status ));
        if (EFI_ERROR (Status)) {
            DisplayErrorMessagePopUp (
                    HiiHandle,
                    STR_SET_USER_ACCESS_CMD_FAIL );

            return EFI_UNSUPPORTED;
        }
    }

    DEBUG ((DEBUG_INFO, "IPMI: AddBmcUserMaximumPossibleUserPrivilegeLevel(Before Set Browser data) = %x\n", ServerMgmtConfiguration.AddBmcUserMaximumPossibleUserPrivilegeLevel));

    // Set Browser data.
    BrowserDataStatus = HiiSetBrowserData (
                             &gEfiServerMgmtSetupVariableGuid,
                             L"ServerSetup",
                             SelectionBufferSize,
                             (UINT8*)&ServerMgmtConfiguration,
                             NULL );
    if (!BrowserDataStatus) {
        DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiSetBrowserData() = %d\n",BrowserDataStatus));
        return EFI_UNSUPPORTED;
    }
    DEBUG ((DEBUG_INFO, "%a Exit... \n", __FUNCTION__ ));
    return EFI_SUCCESS;
}

/**
    This function sends set user access command to BMC for given input.

    @param[in] HiiHandle    Handle to HII database.
    @param[in] Class        Formset Class.
    @param[in] SubClass     Formset Subclass.
    @param[in] Key          Formset Key.

    @retval EFI_UNSUPPORTED     Invalid key.
    @retval EFI_SUCCESS         Successful completion of the function.
    @retval Others              Error status of function calls used.

**/
EFI_STATUS
BmcAddUserChannelPrivilegeLimitCallbackFunction (
  IN EFI_HII_HANDLE     HiiHandle,
  IN UINT16             Class,
  IN UINT16             SubClass,
  IN UINT16             Key )
{
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration;
    EFI_STATUS                          Status = EFI_SUCCESS;
    BOOLEAN                             BrowserDataStatus;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    CALLBACK_PARAMETERS                 *CallbackParameters;

    DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    // Check for the key and Return if Key value does not match.
    if (Key != ADD_BMC_USER_PRIVILEGE_LIMIT_KEY) {
        DEBUG ((DEBUG_ERROR,"IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    // Get the call back parameters and verify the action.
    CallbackParameters = GetCallbackParameters ();
    if (CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
        DEBUG ((DEBUG_ERROR, "IPMI: CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD... so return EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }
    if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) {
        DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return EFI_SUCCESS;
    }


    // Get browser data.
    BrowserDataStatus = HiiGetBrowserData (
                            &gEfiServerMgmtSetupVariableGuid,
                            L"ServerSetup",
                            SelectionBufferSize,
                            (UINT8*)&ServerMgmtConfiguration );
    if (!BrowserDataStatus) {
        DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
        return EFI_UNSUPPORTED;
    }

    DEBUG((DEBUG_INFO, "IPMI: AddBmcUserPrivilegeLimit = %x\n", ServerMgmtConfiguration.AddBmcUserPrivilegeLimit));

    Status = SetUserAccessCommand((UINT8)ServerMgmtConfiguration.AddBmcChannelNo,(UINT8)gBmcAddUserDetails.UserId,(UINT8)ServerMgmtConfiguration.AddBmcUserPrivilegeLimit);

    if(EFI_ERROR (Status)) {
        DisplayErrorMessagePopUp (
            HiiHandle,
            STRING_TOKEN(STR_SET_USER_ACCESS_CMD_FAIL) );
        ServerMgmtConfiguration.AddBmcUserPrivilegeLimit = 0;

        // Set browser data.
        BrowserDataStatus = HiiSetBrowserData (
                                &gEfiServerMgmtSetupVariableGuid,
                                L"ServerSetup",
                                SelectionBufferSize,
                                (UINT8*)&ServerMgmtConfiguration,
                                NULL );
        if (!BrowserDataStatus) {
            DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiSetBrowserData() = %d\n",BrowserDataStatus));
             return EFI_UNSUPPORTED;
        }
        DEBUG ((DEBUG_INFO, "%a Exit... \n", __FUNCTION__ ));
        return EFI_SUCCESS;
    }

    DisplayErrorMessagePopUp (
            HiiHandle,
            (STR_SET_USER_ACCESS_CMD_PASS) );

    DEBUG ((DEBUG_INFO, "%a Exit... \n", __FUNCTION__ ));
    return EFI_SUCCESS;
}

/**
    This function Clears previous NVRAM value before performing delete user
    action.

    @param[in] HiiHandle    Handle to HII database.
    @param[in] Class        Formset Class.
    @param[in] SubClass     Formset Subclass.
    @param[in] Key          Formset Key.

    @retval EFI_UNSUPPORTED     Invalid key.
    @retval EFI_SUCCESS         Successful completion of the function.
    @retval Others              Error status of function calls used.

**/
EFI_STATUS
BmcDeleteUserCallbackFunction (
  IN EFI_HII_HANDLE     HiiHandle,
  IN UINT16             Class,
  IN UINT16             SubClass,
  IN UINT16             Key )
{
    BOOLEAN                            BrowserDataStatus;
    CALLBACK_PARAMETERS                *CallbackParameters;
    SERVER_MGMT_CONFIGURATION_DATA     ServerMgmtConfiguration ;
    UINTN                              SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);

    DEBUG ((DEBUG_INFO, "%a Entry... Key Value: %x\n", __FUNCTION__, Key));

    // Continue only for BMC_ADD USER KEY.
    if (Key != DELETE_BMC_USER_KEY) {
        DEBUG ((DEBUG_ERROR,"IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    // Get the call back parameters and verify the action.
    CallbackParameters = GetCallbackParameters ();
    DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action: %x\n", CallbackParameters->Action));
    if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGING) {
        DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGING... so return EFI_SUCCESS\n"));
        return EFI_SUCCESS;
    }

    // Get browser data.
    BrowserDataStatus = HiiGetBrowserData (
                            &gEfiServerMgmtSetupVariableGuid,
                            L"ServerSetup",
                            SelectionBufferSize,
                            (UINT8*)&ServerMgmtConfiguration );
    if (!BrowserDataStatus) {
        DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
        return EFI_UNSUPPORTED;
    }
    ZeroMem (
        ServerMgmtConfiguration.DeleteBmcUserName,
        MAX_BMC_USER_NAME_LENGTH );

    // Set browser data.
    BrowserDataStatus = HiiSetBrowserData (
                            &gEfiServerMgmtSetupVariableGuid,
                            L"ServerSetup",
                            SelectionBufferSize,
                            (UINT8*)&ServerMgmtConfiguration,
                            NULL );
    if (!BrowserDataStatus) {
        DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiSetBrowserData() = %d\n",BrowserDataStatus));
        return EFI_UNSUPPORTED;
    }

    DEBUG ((DEBUG_INFO, "IPMI: Returning from %a\n", __FUNCTION__));
    return  EFI_SUCCESS;
}

/**
    This function gets the user name for deleting a user from BMC.

    @param[in] HiiHandle    Handle to HII database.
    @param[in] Class        Formset Class.
    @param[in] SubClass     Formset Subclass.
    @param[in] Key          Formset Key.

    @retval EFI_UNSUPPORTED     Invalid key.
    @retval EFI_SUCCESS         Successful completion of the function.
    @retval Others              Error status of function calls used.

**/
EFI_STATUS
BmcDeleteUserNameCallbackFunction (
  IN EFI_HII_HANDLE     HiiHandle,
  IN UINT16             Class,
  IN UINT16             SubClass,
  IN UINT16             Key )
{
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration;
    BOOLEAN                             BrowserDataStatus;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    BOOLEAN                             UserNameAlreadyAvailable;
    CALLBACK_PARAMETERS                 *CallbackParameters;
    CHAR16                              NameBuffer[MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER];
    DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    // Check for the key and Return if Key value does not match.
    if (Key != DELETE_BMC_USER_NAME_KEY) {
        DEBUG ((DEBUG_ERROR, "IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    // Get the call back parameters and verify the action.
    CallbackParameters = GetCallbackParameters ();
    if (CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
        DEBUG ((DEBUG_ERROR, "IPMI: CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD... so return EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    } else if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) {
        DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return EFI_SUCCESS;
    }

    // Get browser data.
    BrowserDataStatus = HiiGetBrowserData (
                            &gEfiServerMgmtSetupVariableGuid,
                            L"ServerSetup",
                            SelectionBufferSize,
                            (UINT8*)&ServerMgmtConfiguration );

     if (!BrowserDataStatus) {
         DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
         return EFI_UNSUPPORTED;
     }

    // Convert User name format from UNICODE to ASCII.
    ZeroMem (
        gBmcDeleteUserDetails.UserName,
        MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER );

    ZeroMem (
        NameBuffer,
        MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER );

    CopyMem (
        NameBuffer,
        ServerMgmtConfiguration.DeleteBmcUserName,
        sizeof(ServerMgmtConfiguration.DeleteBmcUserName) );

    UnicodeStrToAsciiStrS (
        NameBuffer,
        gBmcDeleteUserDetails.UserName,
        MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER );
    DEBUG ((DEBUG_INFO, "IPMI: gBmcDeleteUserDetails.UserName = %a\n", gBmcDeleteUserDetails.UserName));

    // Check the user name is already available in User Details list.
    UserNameAlreadyAvailable = CheckUserNameAvailability (&gBmcDeleteUserDetails);
    DEBUG ((DEBUG_INFO, "IPMI: UserNameAlreadyAvailable:%x\n", UserNameAlreadyAvailable));

    if (!UserNameAlreadyAvailable) {

        // Display User Name not available pop up message.
        DisplayErrorMessagePopUp (
            HiiHandle,
            STRING_TOKEN(STR_USER_NAME_NOT_AVAILABLE) );
        
        ZeroMem (
            ServerMgmtConfiguration.DeleteBmcUserName,
            MAX_BMC_USER_NAME_LENGTH );
        
        ZeroMem (
            &gBmcDeleteUserDetails,
            sizeof (gBmcDeleteUserDetails) );

    } else {
        ServerMgmtConfiguration.DeleteValidBmcUser = 1;
    }
    // Set browser data.
    BrowserDataStatus = HiiSetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration,
                           NULL );
    if (!BrowserDataStatus) {
        DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiSetBrowserData() = %d\n", BrowserDataStatus));
        return EFI_UNSUPPORTED;
    }
    DEBUG ((DEBUG_INFO, "%a Exit... \n", __FUNCTION__));
    return EFI_SUCCESS;
}

/**
    This function gets the user password for deleting a user from BMC.

    @param[in] HiiHandle    Handle to HII database.
    @param[in] Class        Formset Class.
    @param[in] SubClass     Formset Subclass.
    @param[in] Key          Formset Key.

    @retval EFI_UNSUPPORTED     Invalid key.
    @retval EFI_SUCCESS         Successful completion of the function.
    @retval Others              Error status of function calls used.

**/
EFI_STATUS
BmcDeleteUserPasswordCallbackFunction (
  IN EFI_HII_HANDLE     HiiHandle,
  IN UINT16             Class,
  IN UINT16             SubClass,
  IN UINT16             Key )
{
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration;
    EFI_STATUS                          Status = EFI_SUCCESS;
    BOOLEAN                             BrowserDataStatus;
    UINTN                               SelectionBufferSize =sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    CALLBACK_PARAMETERS                 *CallbackParameters;
    CHAR16                              *UserPassword = NULL;
    CHAR16                              Password[MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER];

    DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    // Check for the key and Return if Key value does not match
    if (Key != DELETE_BMC_USER_PASSWORD_KEY) {
        DEBUG ((DEBUG_ERROR,"IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    // Get the call back parameters and verify the action.
    CallbackParameters = GetCallbackParameters ();
    if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) {
        DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return EFI_SUCCESS;
    }

    // Get browser data.
    BrowserDataStatus = HiiGetBrowserData (
                              &gEfiServerMgmtSetupVariableGuid,
                              L"ServerSetup",
                              SelectionBufferSize,
                              (UINT8*)&ServerMgmtConfiguration );
    if (!BrowserDataStatus) {
        DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
        return EFI_UNSUPPORTED;
     }
    // Call DisplayTextBox function to get user password.
    UserPassword = Password;
    ZeroMem (
        UserPassword,
        MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER );
    
    DisplayTextBox (
        HiiHandle,
        STR_BMC_USER_PASSWORD_DELETE,
        (CHAR16 **)&UserPassword );
    
    UnicodeStrToAsciiStrS (
        (CHAR16 *)UserPassword,
        gBmcDeleteUserDetails.UserPassword,
        MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER );

    if (((AsciiStrnLenS (gBmcDeleteUserDetails.UserPassword, MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER) < BMC_USER_PASSWORD_MIN_SIZE) || \
            (AsciiStrnLenS (gBmcDeleteUserDetails.UserPassword, MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER) > BMC_USER_PASSWORD_MAX_SIZE))) {
        // Display User Name not available pop up message.
        DisplayErrorMessagePopUp (
            HiiHandle,
            STRING_TOKEN(STR_USER_PASSWORD_INVALID_SIZE) );
        
        ZeroMem (
            gBmcDeleteUserDetails.UserPassword,
            MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER );
        return EFI_INVALID_PARAMETER;
    }

    Status = PerformGivenUserPasswordAction (
                gBmcDeleteUserDetails.UserId,
                AmiSetPasswordSizeTwentyByte,
                AmiSetUserPasswordOperationTestPassword,
                gBmcDeleteUserDetails.UserPassword );
    
    if (EFI_ERROR (Status)) {
        DisplayErrorMessagePopUp (
            HiiHandle,
            STRING_TOKEN(STR_USER_PASSWORD_MISMATCH) );
        return EFI_UNSUPPORTED;
    }

    Status = DeleteUserNameFromBmc (gBmcDeleteUserDetails.UserId);
    if (EFI_ERROR(Status)) {
        DisplayErrorMessagePopUp (
            HiiHandle,
            STRING_TOKEN(STR_USER_NAME_DELETE_FAIL) );
        return EFI_UNSUPPORTED;
    }

    Status = DeleteUserEntryFromList (gBmcDeleteUserDetails.UserName);
    DEBUG ((DEBUG_INFO, "\nStatus of DeleteUserEntryFromList = %r\n", Status));
    if (EFI_ERROR (Status)) {
        DisplayErrorMessagePopUp (
            HiiHandle,
            STRING_TOKEN(STR_USER_NAME_DELETE_FAIL) );
        return EFI_UNSUPPORTED;
    }

    ServerMgmtConfiguration.DeleteValidBmcUser = 0;

    DisplayErrorMessagePopUp (
        HiiHandle,
        STRING_TOKEN(STR_USER_DELETED) );

    ZeroMem (
        &gBmcDeleteUserDetails,
        sizeof (gBmcDeleteUserDetails) );
    ZeroMem (
        ServerMgmtConfiguration.DeleteBmcUserName,
        MAX_BMC_USER_NAME_LENGTH );

    // Set browser data.
    BrowserDataStatus = HiiSetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration,
                           NULL );

    if (!BrowserDataStatus) {
        DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiSetBrowserData() = %d\n", BrowserDataStatus));
        return EFI_UNSUPPORTED;
    }
    DEBUG((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));
    return EFI_SUCCESS;
}

/**
    This function load default values to controls before changing user
    settings.

    @param[in] HiiHandle    Handle to HII database.
    @param[in] Class        Formset Class.
    @param[in] SubClass     Formset Subclass.
    @param[in] Key          Formset Key.

    @retval EFI_UNSUPPORTED     Invalid key.
    @retval EFI_SUCCESS         Successful completion of the function.
    @retval Others              Error status of function calls used.

**/
EFI_STATUS
ChangeUserSettingsCallbackFunction (
  IN EFI_HII_HANDLE     HiiHandle,
  IN UINT16             Class,
  IN UINT16             SubClass,
  IN UINT16             Key )
{
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration;
    BOOLEAN                             BrowserDataStatus;
    UINTN                               SelectionBufferSize  = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    CALLBACK_PARAMETERS                 *CallbackParameters;

    DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    // Check for the key and Return if Key value does not match.
    if (Key != CHANGE_BMC_USER_SETTINGS_KEY) {
        DEBUG ((DEBUG_ERROR, "IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    // Get the call back parameters and verify the action.
    CallbackParameters = GetCallbackParameters ();
    if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGING) {
        DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGING... so return EFI_SUCCESS\n"));
        return EFI_SUCCESS;
    }

    // Get browser data.
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration);
    DEBUG ((DEBUG_INFO, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
         return EFI_UNSUPPORTED;
    }
    ZeroMem (
        ServerMgmtConfiguration.ChangeUserSettingsBmcCurrentUserName,
        MAX_BMC_USER_NAME_LENGTH );
    
    ServerMgmtConfiguration.ChangeUserSettingsValidBmcUser = 0;
    ServerMgmtConfiguration.ChangeUserSettingsBmcUserAccess = 0;
    ServerMgmtConfiguration.ChangeUserSettingsValidBmcUserCredentials = 0;
    ServerMgmtConfiguration.ChangeUserSettingsValidPassword = 0;
    ServerMgmtConfiguration.ChangeUserSettingsBmcChannelNo = 0;
    ServerMgmtConfiguration.ChangeUserSettingsBmcUserMaximumPossibleUserPrivilegeLevel = 0;
    ServerMgmtConfiguration.ChangeUserSettingsBmcUserPrivilegeLimit = 0;
    ServerMgmtConfiguration.ChangeUserSettingsValidBmcLanChannel = 0;

    // Set browser data.
    BrowserDataStatus = HiiSetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration,
                           NULL );
    if (!BrowserDataStatus) {
        DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiSetBrowserData() = %d\n", BrowserDataStatus));
        return EFI_UNSUPPORTED;
    }
    DEBUG((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));
    return EFI_SUCCESS;
}

/**
    This function gets the current user name for changing a user details
    from BMC.

    @param[in] HiiHandle    Handle to HII database.
    @param[in] Class        Formset Class.
    @param[in] SubClass     Formset Subclass.
    @param[in] Key          Formset Key.

    @retval EFI_UNSUPPORTED     Invalid key.
    @retval EFI_SUCCESS         Successful completion of the function.
    @retval Others              Error status of function calls used.

**/
EFI_STATUS
ChangeUserSettingsCurrentBmcUserNameCallbackFunction (
  IN EFI_HII_HANDLE     HiiHandle,
  IN UINT16             Class,
  IN UINT16             SubClass,
  IN UINT16             Key )
{
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration ;
    BOOLEAN                             BrowserDataStatus;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    BOOLEAN                             UserNameAlreadyAvailable;
    CALLBACK_PARAMETERS                 *CallbackParameters;
    CHAR16                              NameBuffer[MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER];
    DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    // Check for the key and Return if Key value does not match.
    if (Key != CHANGE_USER_SETTINGS_BMC_CURRENT_USER_NAME_KEY) {
        DEBUG ((DEBUG_ERROR, "IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    // Get the call back parameters and verify the action.
    CallbackParameters = GetCallbackParameters ();
    if (CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
        DEBUG ((DEBUG_ERROR, "IPMI: CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD... so return EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    } else if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) {
        DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return EFI_SUCCESS;
    }

    // Get browser data.
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration );

    if (!BrowserDataStatus) {
        DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
        return EFI_UNSUPPORTED;
    }
    // Convert User name format from UNICODE to ASCII.
    ZeroMem (
        gBmcChangeUserDetails.UserName,
        MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER );
    ZeroMem (
        NameBuffer,
        MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER );
    CopyMem(
        NameBuffer,
        ServerMgmtConfiguration.ChangeUserSettingsBmcCurrentUserName,
        sizeof(ServerMgmtConfiguration.ChangeUserSettingsBmcCurrentUserName));
    
    UnicodeStrToAsciiStrS (
        NameBuffer,
        gBmcChangeUserDetails.UserName,
        MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER );

    // Check the user name is already available in User Details list.
    UserNameAlreadyAvailable = CheckUserNameAvailability (&gBmcChangeUserDetails);
    DEBUG ((DEBUG_INFO, "IPMI: UserNameAlreadyAvailable:%x\n", UserNameAlreadyAvailable));

    if( !UserNameAlreadyAvailable ) {
        // Display User Name not available pop up message.
        DisplayErrorMessagePopUp (
            HiiHandle,
            STRING_TOKEN(STR_USER_NAME_NOT_FOUND) );
        
        ZeroMem (
            ServerMgmtConfiguration.ChangeUserSettingsBmcCurrentUserName,
            MAX_BMC_USER_NAME_LENGTH );
        
        ZeroMem (
            gBmcChangeUserDetails.UserName,
            sizeof (gBmcChangeUserDetails.UserName) );
        ServerMgmtConfiguration.ChangeUserSettingsValidBmcUser             = 0;
        ServerMgmtConfiguration.ChangeUserSettingsValidBmcUserCredentials  = 0;
     
    } else {
        ServerMgmtConfiguration.ChangeUserSettingsValidBmcUser             = 1;
        ServerMgmtConfiguration.ChangeUserSettingsValidBmcUserCredentials  = 0;
        ServerMgmtConfiguration.ChangeUserSettingsValidPassword = 0;
    }

    ServerMgmtConfiguration.ChangeUserSettingsBmcChannelNo = 0;
    ServerMgmtConfiguration.ChangeUserSettingsBmcUserMaximumPossibleUserPrivilegeLevel = 0;
    ServerMgmtConfiguration.ChangeUserSettingsBmcUserPrivilegeLimit = 0;
    ServerMgmtConfiguration.ChangeUserSettingsBmcUserAccess = 0;
    ServerMgmtConfiguration.ChangeUserSettingsValidBmcLanChannel = 0;

    // Set browser data.
    BrowserDataStatus = HiiSetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration,
                           NULL );
    if (!BrowserDataStatus) {
        DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiSetBrowserData() = %d\n", BrowserDataStatus));
        return EFI_UNSUPPORTED;
    }
    DEBUG((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));
    return EFI_SUCCESS;
}

/**
    This function gets the current user password for changing a user details from BMC.

    @param[in] HiiHandle    Handle to HII database.
    @param[in] Class        Formset Class.
    @param[in] SubClass     Formset Subclass.
    @param[in] Key          Formset Key.

    @retval EFI_UNSUPPORTED     Invalid key.
    @retval EFI_SUCCESS         Successful completion of the function.
    @retval Others              Error status of function calls used.

**/
EFI_STATUS
ChangeUserSettingsCurrentBmcUserPasswordCallbackFunction (
  IN EFI_HII_HANDLE     HiiHandle,
  IN UINT16             Class,
  IN UINT16             SubClass,
  IN UINT16             Key )
{
    AMI_GET_USER_ACCESS_REQUEST         GetUserAccesscommond;
    AMI_GET_USER_ACCESS_RESPONSE        GetUserAccessResponse;
    UINT8                               GetUserAccessResponseDataSize;
    IPMI_GET_CHANNEL_ACCESS_REQUEST     GetChannelAccessCommand;
    IPMI_GET_CHANNEL_ACCESS_RESPONSE    GetChannelAccessResponse;
    UINT8                               GetChannelAccessResponseDataSize;
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration;
    EFI_STATUS                          Status = EFI_SUCCESS;
    BOOLEAN                             BrowserDataStatus;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    CALLBACK_PARAMETERS                 *CallbackParameters;
    CHAR16                              *UserPassword;
    CHAR16                              Password[MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER];
    UINT8                               Index;

    DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    // Check for the key and Return if Key value does not match.
    if (Key != CHANGE_USER_SETTINGS_BMC_USER_CURRENT_PASSWORD_KEY) {
        DEBUG ((DEBUG_ERROR, "IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    // Get the call back parameters and verify the action
    CallbackParameters = GetCallbackParameters ();
    if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) {
        DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return EFI_SUCCESS;
    }

    // Get browser data.
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration );
    if (!BrowserDataStatus) {
        DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiGetBrowserData() = %d\n",BrowserDataStatus));
        return EFI_UNSUPPORTED;
    }
    // Call DisplayTextBox function to get user password.
    UserPassword = Password;
    ZeroMem (
        UserPassword,
        MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER );
    
    DisplayTextBox (
        HiiHandle,
        STR_BMC_USER_PASSWORD_CHANGE,
        (CHAR16 **)&UserPassword );
    
    UnicodeStrToAsciiStrS (
        (CHAR16 *)UserPassword,
        gBmcChangeUserDetails.UserPassword,
        MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER );

    if (((AsciiStrnLenS (gBmcChangeUserDetails.UserPassword, MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER) < BMC_USER_PASSWORD_MIN_SIZE) || \
            (AsciiStrnLenS (gBmcChangeUserDetails.UserPassword, MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER) > BMC_USER_PASSWORD_MAX_SIZE))) {
        // Display User Name not available pop up message.
        DisplayErrorMessagePopUp (
            HiiHandle,
            STRING_TOKEN(STR_INVALID_USER_PASSWORD) );
        
        ZeroMem (
            gBmcChangeUserDetails.UserPassword,
            sizeof (gBmcChangeUserDetails.UserPassword) );
        return EFI_INVALID_PARAMETER;
    }

    Status = PerformGivenUserPasswordAction (
                gBmcChangeUserDetails.UserId,
                AmiSetPasswordSizeTwentyByte,
                AmiSetUserPasswordOperationTestPassword,
                gBmcChangeUserDetails.UserPassword );
    
    if (EFI_ERROR (Status)){
        DisplayErrorMessagePopUp (
            HiiHandle,
            STRING_TOKEN(STR_USER_PASSWORD_MISMATCH) );
        
        ZeroMem (
            gBmcChangeUserDetails.UserPassword,
            sizeof (gBmcChangeUserDetails.UserPassword) );
        return EFI_UNSUPPORTED;
    } else {
        ServerMgmtConfiguration.ChangeUserSettingsValidPassword = 1;
        ServerMgmtConfiguration.ChangeUserSettingsValidBmcUserCredentials = 1;
        for(Index=0;Index<gPcdSize;Index++){
            GetChannelAccessCommand.ChannelNumber.Bits.ChannelNo = gPcdLanChannelBufferPtr[Index];
            GetChannelAccessCommand.AccessType.Bits.MemoryType = AmiChannelAccessTypeNonVolatile;// Non Volatile channel Access
            GetChannelAccessCommand.ChannelNumber.Bits.Reserved = 0x00;
            GetChannelAccessCommand.AccessType.Bits.Reserved = 0x00;
            GetChannelAccessResponseDataSize = sizeof (GetChannelAccessResponse);

            Status = gIpmiTransport->SendIpmiCommand (
                                          gIpmiTransport,
                                          IPMI_NETFN_APP,
                                          AMI_BMC_LUN,
                                          IPMI_APP_GET_CHANNEL_ACCESS,
                                          (UINT8 *)&GetChannelAccessCommand,
                                          sizeof (GetChannelAccessCommand),
                                          (UINT8 *)&GetChannelAccessResponse,
                                           &GetChannelAccessResponseDataSize );

            DEBUG ((DEBUG_INFO, "IPMI: CommandCompletionCode: %x \n",GetChannelAccessResponse.CompletionCode));
           DEBUG ((DEBUG_INFO, "IPMI: Status of IPMI_APP_GET_CHANNEL_ACCESS Command: %r \n", Status));

           if ((!EFI_ERROR (Status)) && (GetChannelAccessResponse.CompletionCode == COMPLETION_CODE_SUCCESS)) {
               DEBUG ((DEBUG_INFO, "IPMI: GetChannelAccessResponse.ChannelPriviledgeLimit: %x \n", GetChannelAccessResponse.PrivilegeLimit.Bits.ChannelPriviledgeLimit));
               ServerMgmtConfiguration.ChangeUserSettingsBmcUserMaximumPossibleUserPrivilegeLevel = GetChannelAccessResponse.PrivilegeLimit.Bits.ChannelPriviledgeLimit;
               GetUserAccessResponseDataSize= sizeof(GetUserAccessResponse);
               GetUserAccesscommond.UserId = gBmcChangeUserDetails.UserId;
               GetUserAccesscommond.ChannelNo = gPcdLanChannelBufferPtr[Index];
               GetUserAccesscommond.Reserve1 = 0x00;
               GetUserAccesscommond.Reserve2 = 0x00;

               Status = gIpmiTransport->SendIpmiCommand(
                                            gIpmiTransport,
                                            IPMI_NETFN_APP,
                                            AMI_BMC_LUN,
                                            IPMI_APP_GET_USER_ACCESS,
                                            (UINT8 *)&GetUserAccesscommond,
                                            sizeof (GetUserAccesscommond),
                                            (UINT8*)&GetUserAccessResponse,
                                            &GetUserAccessResponseDataSize );
               DEBUG ((DEBUG_INFO, "IPMI: IPMI_APP_GET_USER_ACCESS = %r\n", Status));
               if ((!EFI_ERROR (Status)) && (GetUserAccessResponse.CompletionCode == COMPLETION_CODE_SUCCESS)) {
                       ServerMgmtConfiguration.ChangeUserSettingsBmcUserAccess = GetUserAccessResponse.UserIdEnable;
                       if (GetUserAccessResponse.UserPrivilegeLimit != USER_PRIVILEGE_NO_ACCESS) {
                           ServerMgmtConfiguration.ChangeUserSettingsBmcChannelNo = gPcdLanChannelBufferPtr[Index];
                           ServerMgmtConfiguration.ChangeUserSettingsBmcUserPrivilegeLimit = GetUserAccessResponse.UserPrivilegeLimit;
                           ServerMgmtConfiguration.ChangeUserSettingsValidBmcLanChannel = 1;
                           break;
                       }
               } 
           } 
        }
    }

    // Set browser data.
    BrowserDataStatus = HiiSetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration,
                           NULL );
    if (!BrowserDataStatus) {
        DEBUG ((DEBUG_ERROR,"IPMI: Status of HiiSetBrowserData() = %d\n", BrowserDataStatus));
        return EFI_UNSUPPORTED;
    }
    DEBUG ((DEBUG_INFO, "IPMI: Returning from %a\n", __FUNCTION__));
    return EFI_SUCCESS;
}

/**
    This function gets new user password for changing a user details in BMC.

    @param[in] HiiHandle    Handle to HII database.
    @param[in] Class        Formset Class.
    @param[in] SubClass     Formset Subclass.
    @param[in] Key          Formset Key.

    @retval EFI_UNSUPPORTED     Invalid key.
    @retval EFI_SUCCESS         Successful completion of the function.
    @retval Others              Error status of function calls used.

**/
EFI_STATUS
ChangeUserSettingsBmcUserPasswordCallbackFunction (
  IN EFI_HII_HANDLE     HiiHandle,
  IN UINT16             Class,
  IN UINT16             SubClass,
  IN UINT16             Key )
{
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration;
    BOOLEAN                             BrowserDataStatus;
    EFI_STATUS                          Status = EFI_SUCCESS;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    CALLBACK_PARAMETERS                 *CallbackParameters;
    CHAR16                              PasswordBuffer[MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER];

    DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    // Check for the key and Return if Key value does not match.
    if (Key != CHANGE_USER_SETTINGS_BMC_USER_PASSWORD_KEY) {
        DEBUG ((DEBUG_ERROR, "IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    // Get the call back parameters and verify the action.
    CallbackParameters = GetCallbackParameters ();
    if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) {
        DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return EFI_SUCCESS;
    }

    // Get browser data.
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration );

    if (!BrowserDataStatus) {
        DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
        return EFI_UNSUPPORTED;
    }
   // Return EFI_SUCCESS if the password is NULL.
   if (ServerMgmtConfiguration.ChangeUserSettingsBmcUserPassword[0] == '\0') {
       DEBUG ((DEBUG_INFO, "IPMI: Change Bmc User Password is NULL\n"));
       return EFI_SUCCESS;
   }
    // Convert User Password format from UNICODE to ASCII.
    ZeroMem (
        gBmcChangeUserDetails.UserPassword,
        MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER );

    ZeroMem(
        PasswordBuffer,
        MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER);

    CopyMem(
        PasswordBuffer,
        ServerMgmtConfiguration.ChangeUserSettingsBmcUserPassword,
        sizeof(ServerMgmtConfiguration.ChangeUserSettingsBmcUserPassword));

    UnicodeStrToAsciiStrS(
        PasswordBuffer,
        gBmcChangeUserDetails.UserPassword,
        MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER );

    Status = PerformGivenUserPasswordAction (
                gBmcChangeUserDetails.UserId,
                AmiSetPasswordSizeTwentyByte,
                AmiSetUserPasswordOperationSetPassword,
                (CHAR8 *)gBmcChangeUserDetails.UserPassword );
    
    if (EFI_ERROR (Status)) {
        // Display User Name already available pop up message.
        DisplayErrorMessagePopUp (
            HiiHandle,
            STR_SET_USER_PASSWORD_FAIL );
        
        ZeroMem (
            ServerMgmtConfiguration.ChangeUserSettingsBmcUserPassword,
            MAX_BMC_USER_PASSWORD_LENGTH );
        
        ZeroMem (
            &gBmcChangeUserDetails.UserPassword,
            sizeof(gBmcChangeUserDetails.UserPassword) );
    } else {
        ZeroMem (
            ServerMgmtConfiguration.ChangeUserSettingsBmcUserPassword,
            MAX_BMC_USER_PASSWORD_LENGTH ); //Clearing the password from browser data.
        DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_CHANGED_USERPASSWORD_OPERATION_PASSED) );
    }

    // Set browser data.
    BrowserDataStatus = HiiSetBrowserData (
                            &gEfiServerMgmtSetupVariableGuid,
                            L"ServerSetup",
                            SelectionBufferSize,
                            (UINT8*)&ServerMgmtConfiguration,
                            NULL );
    if (!BrowserDataStatus) {
        DEBUG ((DEBUG_ERROR, "\nStatus of HiiSetBrowserData() = %d\n",BrowserDataStatus));
        return EFI_UNSUPPORTED;
    }
    DEBUG ((DEBUG_INFO, "IPMI: Returning from %a\n", __FUNCTION__));
    return EFI_SUCCESS;
}

/**
    This function enables/disables a user in BMC as per given input.

    @param[in] HiiHandle    Handle to HII database.
    @param[in] Class        Formset Class.
    @param[in] SubClass     Formset Subclass.
    @param[in] Key          Formset Key.

    @retval EFI_UNSUPPORTED     Invalid key.
    @retval EFI_SUCCESS         Successful completion of the function.
    @retval Others              Error status of function calls used.

**/
EFI_STATUS
ChangeUserSettingsEnableOrDisableBmcUserCallbackFunction (
  IN EFI_HII_HANDLE     HiiHandle,
  IN UINT16             Class,
  IN UINT16             SubClass,
  IN UINT16             Key )
{
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration;
    EFI_STATUS                          Status = EFI_SUCCESS;
    BOOLEAN                             BrowserDataStatus;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    CALLBACK_PARAMETERS                 *CallbackParameters;

    DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    // Check for the key and Return if Key value does not match.
    if (Key != CHANGE_USER_SETTINGS_BMC_USER_KEY) {
        DEBUG ((DEBUG_ERROR, "IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    // Get the call back parameters and verify the action.
    CallbackParameters = GetCallbackParameters();
    if (CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
        DEBUG ((DEBUG_ERROR, "IPMI: CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD... so return EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    } else if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) {
        DEBUG ((DEBUG_INFO, "\n CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return EFI_SUCCESS;
    }

    // Get browser data.
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration);
    if (!BrowserDataStatus) {
        DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
        return EFI_UNSUPPORTED;
    }

    DEBUG ((DEBUG_INFO, "\n ServerMgmtConfiguration.ChangeUserSettingsBmcUser: %x\n", ServerMgmtConfiguration.ChangeUserSettingsBmcUserAccess));
    DEBUG ((DEBUG_INFO, "IPMI:gBmcChangeUserDetails.UserId %x\n", gBmcChangeUserDetails.UserId));
    Status = PerformGivenUserPasswordAction(
                gBmcChangeUserDetails.UserId,
                AmiSetPasswordSizeTwentyByte,
                ServerMgmtConfiguration.ChangeUserSettingsBmcUserAccess,
                gBmcChangeUserDetails.UserPassword );

    DEBUG ((DEBUG_INFO, "IPMI: User Enable/Disable Status: %r\n", Status));

    if (EFI_ERROR (Status)) {
        if (ServerMgmtConfiguration.ChangeUserSettingsBmcUserAccess) {
            DisplayErrorMessagePopUp (
                HiiHandle,
                STRING_TOKEN(STR_USER_ENABLE_OPERATION_FAIL) );
        } else {
            DisplayErrorMessagePopUp (
                HiiHandle,
                STRING_TOKEN(STR_USER_DISABLE_OPERATION_FAIL) );
        }
        ServerMgmtConfiguration.ChangeUserSettingsBmcUserAccess = 0;

        // Set browser data.
        BrowserDataStatus = HiiSetBrowserData (
                               &gEfiServerMgmtSetupVariableGuid,
                               L"ServerSetup",
                               SelectionBufferSize,
                               (UINT8*)&ServerMgmtConfiguration,
                               NULL );        
        if (!BrowserDataStatus) {
            DEBUG ((DEBUG_ERROR, "\nStatus of HiiSetBrowserData() = %d\n", BrowserDataStatus));
            return EFI_UNSUPPORTED;
        };
    } else {
        if (ServerMgmtConfiguration.ChangeUserSettingsBmcUserAccess) {
            DisplayErrorMessagePopUp (
                HiiHandle,
                STRING_TOKEN(STR_USER_ENABLE_OPERATION_PASS) );
        } else {
            DisplayErrorMessagePopUp (
                HiiHandle,
                STRING_TOKEN(STR_USER_DISABLE_OPERATION_PASS) );
        }
    }
    DEBUG((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));
    return EFI_SUCCESS;
}

/**
    This function sends get channel access command for the given channel and
    determines the maximum possible privilege level for the channel.

    @param[in] HiiHandle    Handle to HII database.
    @param[in] Class        Formset Class.
    @param[in] SubClass     Formset Subclass.
    @param[in] Key          Formset Key.

    @retval EFI_UNSUPPORTED     Invalid key.
    @retval EFI_SUCCESS         Successful completion of the function.
    @retval Others              Error status of function calls used.

**/
EFI_STATUS
ChangeUserSettingsBmcUserChannelCallbackFunction (
  IN EFI_HII_HANDLE     HiiHandle,
  IN UINT16             Class,
  IN UINT16             SubClass,
  IN UINT16             Key )
{
    AMI_GET_USER_ACCESS_REQUEST         GetUserAccesscommond;
    AMI_GET_USER_ACCESS_RESPONSE        GetUserAccessResponse;
    UINT8                               GetUserAccessResponseDataSize;
    IPMI_GET_CHANNEL_ACCESS_REQUEST     GetChannelAccessCommand;
    IPMI_GET_CHANNEL_ACCESS_RESPONSE    GetChannelAccessResponse;
    UINT8                               GetChannelAccessResponseDataSize;
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration;
    EFI_STATUS                          Status = EFI_SUCCESS;
    BOOLEAN                             BrowserDataStatus;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    CALLBACK_PARAMETERS                 *CallbackParameters;
    UINT8                               Index;

    DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    // Check for the key and Return if Key value does not match.
    if (Key != CHANGE_USER_SETTINGS_BMC_USER_CHANNEL_NO_KEY) {
        DEBUG ((DEBUG_ERROR, "IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    // Get the call back parameters and verify the action.
    CallbackParameters = GetCallbackParameters ();
    if (CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
        DEBUG ((DEBUG_ERROR, "IPMI: CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD... so return EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    } else if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) {
        DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return EFI_SUCCESS;
    }

    // Get Browser Data
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration );

    if (!BrowserDataStatus) {
        DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
        return EFI_UNSUPPORTED;
    }

    DEBUG ((DEBUG_INFO, "IPMI: User entered channel no = %x\n", ServerMgmtConfiguration.ChangeUserSettingsBmcChannelNo));
    for (Index=0;Index<gPcdSize;Index++) {
          if ((gPcdLanChannelBufferPtr[Index] == ServerMgmtConfiguration.ChangeUserSettingsBmcChannelNo)) {
              GetChannelAccessCommand.ChannelNumber.Bits.ChannelNo   = ServerMgmtConfiguration.ChangeUserSettingsBmcChannelNo;
              GetChannelAccessCommand.AccessType.Bits.MemoryType     = AmiChannelAccessTypeNonVolatile;// Non Volatile channel Access
              GetChannelAccessCommand.ChannelNumber.Bits.Reserved    = 0x00;
              GetChannelAccessCommand.AccessType.Bits.Reserved       = 0x00;
              GetChannelAccessResponseDataSize                       = sizeof (GetChannelAccessResponse);

              Status = gIpmiTransport->SendIpmiCommand (
                                          gIpmiTransport,
                                          IPMI_NETFN_APP,
                                          AMI_BMC_LUN,
                                          IPMI_APP_GET_CHANNEL_ACCESS,
                                          (UINT8 *)&GetChannelAccessCommand,
                                          sizeof (GetChannelAccessCommand),
                                          (UINT8 *)&GetChannelAccessResponse,
                                          &GetChannelAccessResponseDataSize );

              DEBUG ((DEBUG_INFO, "IPMI: CompletionCode: %x \n", GetChannelAccessResponse.CompletionCode));
              DEBUG ((DEBUG_INFO, "IPMI: Status of IPMI_APP_GET_CHANNEL_ACCESS Command: %r \n", Status));
              
              
              if ((!EFI_ERROR (Status)) && (GetChannelAccessResponse.CompletionCode == COMPLETION_CODE_SUCCESS)) {
              DEBUG ((DEBUG_INFO, "IPMI: GetChannelAccessResponse.ChannelPriviledgeLimit: %x \n", GetChannelAccessResponse.PrivilegeLimit.Bits.ChannelPriviledgeLimit));
              ServerMgmtConfiguration.ChangeUserSettingsBmcUserMaximumPossibleUserPrivilegeLevel = GetChannelAccessResponse.PrivilegeLimit.Bits.ChannelPriviledgeLimit;
              break;
              }
          }
    }

    if ((EFI_ERROR (Status)) || (GetChannelAccessResponse.CompletionCode != COMPLETION_CODE_SUCCESS) || (Index == gPcdSize)) {
        DisplayErrorMessagePopUp (
            HiiHandle,
            STRING_TOKEN(STR_GET_CHANNEL_ACCESS_CMD_FAIL) );
        ServerMgmtConfiguration.ChangeUserSettingsBmcUserPrivilegeLimit  = 0;
        ServerMgmtConfiguration.ChangeUserSettingsBmcUserMaximumPossibleUserPrivilegeLevel =0;
        ServerMgmtConfiguration.ChangeUserSettingsValidBmcLanChannel = 0;
        ServerMgmtConfiguration.ChangeUserSettingsBmcChannelNo = 0;
    } else {
        GetUserAccessResponseDataSize= sizeof(GetUserAccessResponse);
        GetUserAccesscommond.UserId = gBmcChangeUserDetails.UserId;
        GetUserAccesscommond.ChannelNo = ServerMgmtConfiguration.ChangeUserSettingsBmcChannelNo;
        GetUserAccesscommond.Reserve1 = 0x00;
        GetUserAccesscommond.Reserve2 = 0x00;

        Status = gIpmiTransport->SendIpmiCommand(
                                    gIpmiTransport,
                                    IPMI_NETFN_APP,
                                    AMI_BMC_LUN,
                                    IPMI_APP_GET_USER_ACCESS,
                                    (UINT8 *)&GetUserAccesscommond,
                                    sizeof (GetUserAccesscommond),
                                    (UINT8*)&GetUserAccessResponse,
                                    &GetUserAccessResponseDataSize );
        DEBUG ((DEBUG_INFO, "IPMI: IPMI_APP_GET_USER_ACCESS = %r\n", Status));
        if ((!EFI_ERROR (Status)) && (GetUserAccessResponse.CompletionCode == COMPLETION_CODE_SUCCESS)) {
            ServerMgmtConfiguration.ChangeUserSettingsBmcUserAccess = GetUserAccessResponse.UserIdEnable;
            ServerMgmtConfiguration.ChangeUserSettingsBmcUserPrivilegeLimit = GetUserAccessResponse.UserPrivilegeLimit;
            ServerMgmtConfiguration.ChangeUserSettingsValidBmcLanChannel = 1;
            if (ServerMgmtConfiguration.ChangeUserSettingsBmcUserPrivilegeLimit > ServerMgmtConfiguration.ChangeUserSettingsBmcUserMaximumPossibleUserPrivilegeLevel) {
                DEBUG ((DEBUG_INFO, "IPMI:BmcUserPrivilegeLimit is higher than Channel maximum privilegeLimit\n"));
                ServerMgmtConfiguration.ChangeUserSettingsBmcUserMaximumPossibleUserPrivilegeLevel = ServerMgmtConfiguration.ChangeUserSettingsBmcUserPrivilegeLimit;
            }
            DEBUG ((DEBUG_INFO, "IPMI: ChangeUserSettingsBmcUserPrivilegeLimit = %x\n", ServerMgmtConfiguration.ChangeUserSettingsBmcUserPrivilegeLimit));
         } else {
            return EFI_UNSUPPORTED;
         }
    }
    DEBUG ((DEBUG_INFO, "IPMI: AddBmcUserMaximumPossibleUserPrivilegeLevel(Before Set Browser data) = %x\n", ServerMgmtConfiguration.ChangeUserSettingsBmcUserMaximumPossibleUserPrivilegeLevel));

    // Set browser data.
    BrowserDataStatus = HiiSetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration,
                           NULL );
    DEBUG ((DEBUG_INFO, "IPMI: ChangeUserMaximumPossibleUserPrivilegeLevel(After Set Browser data) = %x\n", ServerMgmtConfiguration.ChangeUserSettingsBmcUserMaximumPossibleUserPrivilegeLevel));
    if (!BrowserDataStatus) {
        DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiSetBrowserData() = %d\n", BrowserDataStatus));
        return EFI_UNSUPPORTED;
    }
    DEBUG((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));
    return EFI_SUCCESS;
}

/**
    This function sets the privilege level for a user as per the given input.

    @param[in] HiiHandle    Handle to HII database.
    @param[in] Class        Formset Class.
    @param[in] SubClass     Formset Subclass.
    @param[in] Key          Formset Key.

    @retval EFI_UNSUPPORTED     Invalid key.
    @retval EFI_SUCCESS         Successful completion of the function.
    @retval Others              Error status of function calls used.

**/
EFI_STATUS
ChangeUserSettingsBmcUserChannelPrivilegeLimitCallbackFunction (
  IN EFI_HII_HANDLE     HiiHandle,
  IN UINT16             Class,
  IN UINT16             SubClass,
  IN UINT16             Key )
{
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration;
    BOOLEAN                             BrowserDataStatus;
    EFI_STATUS                          Status = EFI_SUCCESS;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    CALLBACK_PARAMETERS                 *CallbackParameters;

    DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    // Check for the key and Return if Key value does not match.
    if (Key != CHANGE_USER_SETTINGS_BMC_USER_PRIVILEGE_LIMIT_KEY) {
        DEBUG ((DEBUG_ERROR, "IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    // Get the call back parameters and verify the action.
    CallbackParameters = GetCallbackParameters ();
    if (CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
        DEBUG ((DEBUG_ERROR, "IPMI: CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD... so return EFI_UNSUPPORTED\n"));
        return  EFI_UNSUPPORTED;
    }
    if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) {
        DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return  EFI_SUCCESS;
    }
    // Get browser data.
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration );

    if (!BrowserDataStatus) {
        DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
        return EFI_UNSUPPORTED;
    }
    
    DEBUG ((DEBUG_INFO, "IPMI: ChangeUserSettingsBmcUserPrivilegeLimit = %x\n", ServerMgmtConfiguration.ChangeUserSettingsBmcUserPrivilegeLimit));
    Status = SetUserAccessCommand(
                (UINT8)ServerMgmtConfiguration.ChangeUserSettingsBmcChannelNo,
                (UINT8)gBmcChangeUserDetails.UserId,
                (UINT8)ServerMgmtConfiguration.ChangeUserSettingsBmcUserPrivilegeLimit );

    if(EFI_ERROR(Status)) {
        DisplayErrorMessagePopUp (
            HiiHandle,
            STRING_TOKEN(STR_SET_USER_ACCESS_CMD_FAIL) );
        ServerMgmtConfiguration.ChangeUserSettingsBmcUserPrivilegeLimit = 0;

        // Set browser data.
        BrowserDataStatus = HiiSetBrowserData (
                               &gEfiServerMgmtSetupVariableGuid,
                               L"ServerSetup",
                               SelectionBufferSize,
                               (UINT8*)&ServerMgmtConfiguration,
                               NULL );
       
        if (!BrowserDataStatus) {
            DEBUG ((DEBUG_ERROR, "IPMI: Status of HiiSetBrowserData() = %d\n", BrowserDataStatus));
             return EFI_UNSUPPORTED;
        }
        DEBUG((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));
        return EFI_SUCCESS;
    }

    DisplayErrorMessagePopUp (
        HiiHandle,
        STRING_TOKEN(STR_SET_USER_ACCESS_CMD_PASS) );

    DEBUG((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));
    return EFI_SUCCESS;
}

