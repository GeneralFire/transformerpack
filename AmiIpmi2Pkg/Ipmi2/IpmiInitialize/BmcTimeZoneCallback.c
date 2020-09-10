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

/** @file BmcTimeZoneCallback.c
    1. Using setup callback function, validates Time Zone entered in Setup.
    2. Using Hook function, loads optimal defaults when F3 is pressed.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <ServerMgmtSetup.h>
#include <AmiIpmiNetFnStorageDefinitions.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AMIPostMgr.h>
#include <Protocol/IpmiTransportProtocol.h>
#include <Library/BaseMemoryLib.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define STRING_BUFFER_LENGTH    100

typedef UINT16  STRING_REF;

#define STRING_TOKEN(Token)    Token

//---------------------------------------------------------------------------

/** @internal
    Sets default Time Zone value to 0x07FF.

    @param[in, out] TimeZone    Pointer to TimeZone setup variable.

    @return VOID    Nothing.

**/
VOID
SetDefaultTimeZone (
  IN OUT  CHAR16  TimeZone[] )
{
    TimeZone[0] ='0';
    TimeZone[1] ='x';
    TimeZone[2] ='0';
    TimeZone[3] ='7';
    TimeZone[4] ='F';
    TimeZone[5] ='F';
}

/** @internal
    Validates time zone entered in Setup and returns Status.
    1. Checks for special condition 0x07FF. Return EFI_SUCCESS if time zone
    entered in setup is 0x07FF.
    2. Checks below conditions also
    a. Time should be in (+/-)hh:mm format
    b. TimeZone[0] should be +/- character
    c. TimeZone[1] should be between 0 and 2
    d. TimeZone[2] should be between 0 and 4
    e. TimeZone[3] should be : character
    f. TimeZone[4] should be between 0 and 5
    g. TimeZone[5] should be between 0 and 9

    @param[in] TimeZone     Pointer to TimeZone setup variable.

    @retval EFI_SUCCESS             Valid time zone.
    @retval EFI_INVALID_PARAMETER   Invalid time zone.

**/

EFI_STATUS
ValidateTimeZone (
  IN  CHAR16  TimeZone[] )
{
    INT16   UtcOffset = 0;

    DEBUG ((DEBUG_INFO, "%a Entry... TimeZone: %S \n", __FUNCTION__, TimeZone));

    /* Check for special condition 0x07FF. Return success if condition met
       When 0x07FF is entered in setup, BIOS time is considered as local time.*/
    if ((TimeZone[0] == '0') && \
            ((TimeZone[1] == 'x') || (TimeZone[1] == 'X')) && \
            (TimeZone[2] == '0') && \
            (TimeZone[3] == '7') && \
            ((TimeZone[4] == 'f') || (TimeZone[4] == 'F')) && \
            ((TimeZone[5] == 'f') || (TimeZone[5] == 'F')) ) {
        return EFI_SUCCESS;
    }

    // Check for special characters.
    if ((TimeZone[0] != '+') && (TimeZone[0] != '-')) {
        goto ErrorExit;
    } else if (TimeZone[3] != ':') {
        goto ErrorExit;
    }

    /* Validate TimeZone[5] range. Range: 0 - 9.
       Converting char to number - 1's position.*/
    if ((TimeZone[5] >= '0') && (TimeZone[5] <= '9')) {
        UtcOffset += (TimeZone[5] - 0x30);
        DEBUG ((DEBUG_INFO, " After Converting char to number - 1's position UtcOffset: %d \n", UtcOffset));
    } else {
        goto ErrorExit;
    }

    /* Validate TimeZone[4] range. Range: 0 - 5.
       Converting char to number - 10's position.*/
    if ((TimeZone[4] >= '0') && (TimeZone[4] <= '5')) {
        UtcOffset += ( (TimeZone[4] - 0x30) * 10);
        DEBUG ((DEBUG_INFO," After Converting char to number - 10's position UtcOffset: %d \n", UtcOffset));
    } else {
        goto ErrorExit;
    }

    /* Validate TimeZone[2] range. Range: 0 - 9.
       Converting Hour to minute - 1's position.*/
    if ((TimeZone[2] >= '0') && (TimeZone[2] <= '9')) {
        UtcOffset += ( (TimeZone[2] - 0x30) * 60);
        DEBUG ((DEBUG_INFO," After Converting Hour to minute - 1's position UtcOffset: %d \n", UtcOffset));
    } else {
        goto ErrorExit;
    }

    /* Validate TimeZone[1] range. Range: 0 - 2.
       Converting Hour to minute - 10's position.*/
    if ((TimeZone[1] >= '0') && (TimeZone[1] <= '2')) {
        UtcOffset += ( (TimeZone[1] - 0x30) * 60 * 10);
        DEBUG ((DEBUG_INFO," After Converting Hour to minute - 10's position UtcOffset: %d \n", UtcOffset));
    } else {
        goto ErrorExit;
    }

    // Check for UtcOffset should be in the range 0 to 1440.
    if ((UtcOffset <= MAX_UTC_OFFSET) && \
        (UtcOffset >= MIN_UTC_OFFSET)) {
        return EFI_SUCCESS;
    }

ErrorExit:
    return EFI_INVALID_PARAMETER;
}

/**
    Validates the UTC offset given in setup and display error messages if
    given input is invalid data.

    @param[in] HiiHandle    Handle to HII database.
    @param[in] Class        Formset Class.
    @param[in] SubClass     Formset Subclass.
    @param[in] Key          Formset Key.

    @retval EFI_UNSUPPORTED     Invalid key or to load defaults.
    @retval EFI_SUCCESS         Invalid action or validated time zone
                                successfully.

**/
EFI_STATUS
BmcTimeZoneCallback (
  IN EFI_HII_HANDLE     HiiHandle,
  IN UINT16             Class,
  IN UINT16             SubClass,
  IN UINT16             Key )
{
    SERVER_MGMT_CONFIGURATION_DATA     ServerMgmtConfiguration ;
    BOOLEAN                            BrowserDataStatus;
    EFI_STATUS                         Status = EFI_SUCCESS;
    UINTN                              SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA) ;
    AMI_POST_MANAGER_PROTOCOL          *AmiPostMgr = NULL;
    UINT8                              MsgBoxSel;
    EFI_STRING                         OutputString;
    EFI_STRING                         CharPtrString;
    UINTN                              BufferLength = STRING_BUFFER_LENGTH;
    CALLBACK_PARAMETERS                *CallbackParameters;
    
    DEBUG ((DEBUG_INFO, "\nEntered BmcTimeZoneCallback Key: %x BMC_TIME_ZONE_KEY: %x \n", Key, BMC_TIME_ZONE_KEY));

    // Check and return if the key does not match.
    if (Key != BMC_TIME_ZONE_KEY) {
        DEBUG ((DEBUG_ERROR,"\nCallback function is called with Wrong Key Value. Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    // Get the call back parameters and verify the action
    CallbackParameters = GetCallbackParameters();
    if ( CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD ) {
        DEBUG ((DEBUG_ERROR, "IPMI: CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD... so load the defaults \n"));
        return  EFI_UNSUPPORTED;
    } else if ( CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED ) {
        DEBUG ((DEBUG_INFO, "\n CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n",Status));
        return  EFI_SUCCESS;
    }
 
    // Get Browser DATA
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration );

    if (!BrowserDataStatus) {
        DEBUG ((DEBUG_ERROR,"\nStatus of HiiGetBrowserData() = %d\n",BrowserDataStatus));
        return EFI_UNSUPPORTED;
    }

    // Validate UTC TimeZone offset and display error message.
    Status = ValidateTimeZone (ServerMgmtConfiguration.TimeZone);

    if (EFI_ERROR (Status)) {
        // Set Default time zone for error case.
        SetDefaultTimeZone (ServerMgmtConfiguration.TimeZone);
        
        // Locate AmiPostMgr protocol to print the Error message.
        if (!EFI_ERROR(gBS->LocateProtocol (
                                &gAmiPostManagerProtocolGuid,
                                NULL,
                                (VOID **)&AmiPostMgr ))) {
            
            OutputString = HiiGetString (
                                HiiHandle,
                                STRING_TOKEN(STR_INVALID_TIME_ZONE),
                                NULL );

            CharPtrString = HiiGetString (
                                HiiHandle,
                                STRING_TOKEN(STR_TIME_ZONE_ERROR_INFO),
                                NULL );

            if ((OutputString != NULL) && (CharPtrString != NULL)) {
                AmiPostMgr->DisplayMsgBox (
                                CharPtrString,
                                OutputString,
                                MSGBOX_TYPE_OK,
                                &MsgBoxSel );
                gBS->FreePool(OutputString);
                gBS->FreePool(CharPtrString);
            }
        }

        // Set Browser data.
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
    }

    DEBUG ((DEBUG_INFO, "%a Exiting...\n", __FUNCTION__));

    return EFI_SUCCESS;
}

/** @internal
    Initializes the current BMC Time Zone to be displayed in setup.

    @param[in] HiiHandle    Handle to HII database.
    @param[in] Class        Formset Class.

    @return VOID    Nothing.

**/
VOID
InitBmcTimeZoneDisplay (
  IN EFI_HII_HANDLE     HiiHandle,
  IN UINT16             Class )
{
    EFI_IPMI_TRANSPORT                      *IpmiTransport;
    EFI_STATUS                              Status;
    UINT8                                   ResponseDataSize;
    AMI_GET_SEL_TIME_UTC_OFFSET_RESPONSE    UtcOffsetResponse;
    CHAR16                                  CurrTimeZone[7];

    // Locate IpmiTransport protocol.
    Status = gBS->LocateProtocol (
                    &gEfiDxeIpmiTransportProtocolGuid,
                    NULL,
                    (VOID**)&IpmiTransport );
    
    DEBUG ((DEBUG_INFO, "%a Entry... gEfiDxeIpmiTransportProtocolGuid Status: %r \n", __FUNCTION__, Status));

    if (!EFI_ERROR (Status)) {

        // Get SEL Time UTC Offset Command.
        ResponseDataSize = sizeof (UtcOffsetResponse);
        Status = IpmiTransport->SendIpmiCommand (
                                    IpmiTransport,
                                    IPMI_NETFN_STORAGE,
                                    AMI_BMC_LUN,
                                    AMI_STORAGE_GET_SEL_TIME_UTC_OFFSET,
                                    NULL,
                                    0,
                                    (UINT8*)&(UtcOffsetResponse),
                                    &ResponseDataSize );
        
        DEBUG ((DEBUG_INFO, " In InitBmcTimeZoneDisplay : AMI_STORAGE_GET_SEL_TIME_UTC_OFFSET Status: %r\n", Status));
        
        if (!EFI_ERROR (Status) && (!UtcOffsetResponse.CompletionCode) && \
            (UtcOffsetResponse.UtcOffset != EFI_UNSPECIFIED_TIMEZONE)) {

            CurrTimeZone[0] = '+';
            if (UtcOffsetResponse.UtcOffset < 0) {
                CurrTimeZone[0] = '-';
                UtcOffsetResponse.UtcOffset *= -1;
            }

            // Convert the UTC offset to unicode to display in setup.
            CurrTimeZone[1] = ((UtcOffsetResponse.UtcOffset / 60 ) / 10) + 0x30;
            CurrTimeZone[2] = ((UtcOffsetResponse.UtcOffset / 60 ) % 10) + 0x30;
            CurrTimeZone[3] = ':';
            CurrTimeZone[4] = ((UtcOffsetResponse.UtcOffset % 60 ) / 10) + 0x30;
            CurrTimeZone[5] = ((UtcOffsetResponse.UtcOffset % 60 ) % 10) + 0x30;
            CurrTimeZone[6] = 0x00; //Null termination
            HiiSetString (
                HiiHandle,
                STRING_TOKEN(STR_BMC_CURR_TIME_ZONE_VALUE),
                CurrTimeZone,
                NULL );
        }
    }
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
