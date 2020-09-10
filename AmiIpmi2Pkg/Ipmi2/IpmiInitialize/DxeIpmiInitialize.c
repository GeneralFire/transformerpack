//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file DxeIpmiInitialize.c
    Dxe Ipmi Transport driver.

*/

//---------------------------------------------------------------------------

#include "DxeIpmiInitialize.h"
#include <Library/AmiSelLib.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

AMI_IPMI_BMC_INSTANCE_DATA       *gIpmiInstance = NULL;
SERVER_MGMT_CONFIGURATION_DATA   gServerMgmtConfiguration;
UINT32                           gAttributes;
EFI_STATUS                       gSetupStatus = EFI_SUCCESS;
EFI_HANDLE                       gNewHandle = NULL;

EFI_STATUS_CODE_VALUE gErrorBitToStatusCodeMap [] = {
    EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_FIRMWARE_CORRUPTED,
    EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_BB_CORRUPTED,
    EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_BMC_FRU_CORRUPTED,
    EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_SDR_EMPTY,
    EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_IPMB_NOT_RESPONDING,
    EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_BMC_FRU_NOT_ACCESSIBLE,
    EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_SDR_REPOSITORY_NOT_ACCESSABLE,
    EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_SEL_NOT_ACCESSIBLE
    };
// Time Stamp Event - To Synchronize BIOS and BMC Time using Set SelTime command.
IPMI_PLATFORM_EVENT_MESSAGE_DATA_REQUEST    gEventMessage = {
    // Generator Id: BIOS.
    (UINT8) AMI_IPMI_GENERATOR_ID (AMI_IPMI_BIOS_SOFTWARE_ID),
    IPMI_EVM_REVISION,
    // Sensor Type.
    0x12,
    // Sensor Number.
    AMI_IPMI_DEFAULT_SENSOR_NUMBER,
    // Event DIR.
    IPMI_SENSOR_TYPE_EVENT_CODE_DISCRETE,
    // Sensor specific Offset for Time stamp Clock Synch Event.
    05,
    // ED2
    00,
    // ED3
    0xFF
    };

IPMI_SEL_EVENT_RECORD_DATA  gTimeStampEvtRecord = {
    // Record Id.
    0000,
    // Record Type.
    IPMI_SEL_SYSTEM_RECORD,
    // Time stamp.
    00000000,
    // Generator Id: BIOS.
    (UINT16) AMI_IPMI_GENERATOR_ID (AMI_IPMI_BIOS_SOFTWARE_ID),
    IPMI_EVM_REVISION,
    // Sensor Type.
    0x12,
    // Sensor Number.
    AMI_IPMI_DEFAULT_SENSOR_NUMBER,
    // Event DIR.
    IPMI_SENSOR_TYPE_EVENT_CODE_DISCRETE,
    // Sensor specific Offset for Time stamp Clock Synch Event.
    05,
    // ED2
    00,
    // ED3
    0xFF
    };

//---------------------------------------------------------------------------

/** @internal
    Calculates and returns the total number of days passed till the day in a
    year.

    @param[in] Time     Contains detailed information about date and time.

    @return UINTN   Returns number of days passed.

**/
UINTN
CalculateNumOfDayPassedThisYear (
  IN EFI_TIME   Time )
{
    UINTN   Index;
    UINTN   NumOfDays;
    INTN    DaysOfMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    NumOfDays = 0;
    for (Index = 1; Index < Time.Month; Index++) {
        NumOfDays += DaysOfMonth[(Index - 1)];
    }

    NumOfDays += Time.Day;
    return NumOfDays;
}

/** @internal
    Verifies the given year is leap year or not.

    @param[in] Year     Year in YYYY format.

    @retval TRUE    If Year is a leap year.
    @retval FALSE   If Year is a Non-leap year.

**/
BOOLEAN
IsLeapYear (
  IN UINT16     Year )
{
    if (Year % 4 == 0) {
        if (Year % 100 == 0) {
            if (Year % 400 == 0) {
                return TRUE;
            }
        } else {
            return TRUE;
        }
    }
    return FALSE;
}

/** @internal
    This function returns number of leap years from 1970.

    @param[in] CurYear  Current year.

    @return UINTN   Returns number of leap years.

**/
UINTN
CountNumOfLeapYears (
  IN UINT16     CurYear )
{
    UINT16      NumOfYear;
    UINT16      BaseYear;
    UINT16      Index;
    UINTN       Count = 0;

    BaseYear = 1970;
    NumOfYear = (UINT16)(CurYear - 1970);
    for (Index = 0; Index <= NumOfYear; Index++) {
        if (IsLeapYear ((UINT16)(BaseYear + Index))) {
            Count += 1;
        }
    }
    return Count;
}

/** @internal
    1. Compares BIOS and BMC time stamps. If they are different, BMC time
    stamp is updated to BIOS time using Set SEL Time command.
    2. Logs Time stamp Clock Synch pair of Events.

    @param VOID     Nothing.

    @return EFI_STATUS  Return status of the send ipmi command.

**/
EFI_STATUS
SetTimeStamp (
  VOID )
{
    EFI_STATUS                                  Status;
    AMI_GET_SEL_TIME_RESPONSE                   SelTimeResponse;
    UINT32                                      NumOfSeconds;
    UINT16                                      NumOfYears;
    UINTN                                       NumOfLeapYears;
    UINTN                                       NumOfDays;
    EFI_TIME                                    Time = {2012, 03, 06, 00, 00, 00, 00, 00, 00, 00, 00};
    UINT8                                       ResponseDataSize;
    UINT32                                      CurrentBmcTime;
    AMI_ADD_SEL_ENTRY_RESPONSE                  AddSelEntryResponse;
    AMI_GET_SEL_TIME_UTC_OFFSET_RESPONSE        UtcOffsetResponse;
    UINT8                                       CompletionCode;
#if DxeSelStatusCode_SUPPORT
    SERVER_MGMT_CONFIGURATION_DATA              ServerMgmtConfiguration;
    UINTN                                       Size;
    UINT32                                      Attributes;
#endif

    DEBUG ((DEBUG_INFO, "%a - Entered...\n", __FUNCTION__));

    // Get current UTC offset from BMC.
    ResponseDataSize = sizeof (UtcOffsetResponse);
    Status = gIpmiInstance->IpmiTransport.SendIpmiCommand (
                                            &gIpmiInstance->IpmiTransport,
                                            IPMI_NETFN_STORAGE,
                                            AMI_BMC_LUN,
                                            AMI_STORAGE_GET_SEL_TIME_UTC_OFFSET,
                                            NULL,
                                            0,
                                            (UINT8*)&UtcOffsetResponse,
                                            &ResponseDataSize );

    DEBUG ((DEBUG_INFO, "%a - AMI_STORAGE_GET_SEL_TIME_UTC_OFFSET status = %r  CompletionCode = %x Value = %x\n", 
            __FUNCTION__, Status, UtcOffsetResponse.CompletionCode, UtcOffsetResponse.UtcOffset));

    if (EFI_ERROR (Status) || (UtcOffsetResponse.CompletionCode)) {
        UtcOffsetResponse.UtcOffset = EFI_UNSPECIFIED_TIMEZONE;
    }

    /* Commenting code as Time Zone issue faced in OS
    if (UtcOffsetResponse.UtcOffset != EFI_UNSPECIFIED_TIMEZONE) {

        // Read the BIOS time using RuntimeServices->GetTime().
        Status = gRT->GetTime (&Time, NULL);
        DEBUG ((DEBUG_INFO, "gRT->GetTime status %r\n", Status));
        if (EFI_ERROR (Status)) {
            return Status;
        }
        /* BMC Time Zone is calculated as Local time - UTC. BIOS Time Zone
           is calculated as Local time + UTC. To overcome the above
           difference in calculation -1 is used for BIOS time zone.*/
        //if (UtcOffsetResponse.UtcOffset != (Time.TimeZone * -1)) {

            /* Read the BIOS Time Zone using RuntimeServices->GetTime().
               Reading once again to get the latest time.*/
            /*Status = gRT->GetTime (&Time, NULL);
            DEBUG ((DEBUG_INFO, "gRT->GetTime status %r\n", Status));
            if (EFI_ERROR (Status)) {
                return Status;
            }
            Time.TimeZone = UtcOffsetResponse.UtcOffset * -1;
            // Set the BIOS time Zone using RuntimeServices->SetTime().
            Status = gRT->SetTime (&Time);
            DEBUG ((DEBUG_INFO, "gRT->SetTime status %r\n", Status));
            if (EFI_ERROR (Status)) {
                return Status;
            }
        }
    }*/

    // Get BMC time using Get SEL Time command.
    ResponseDataSize = sizeof (SelTimeResponse);
    Status = gIpmiInstance->IpmiTransport.SendIpmiCommand (
                                            &gIpmiInstance->IpmiTransport,
                                            IPMI_NETFN_STORAGE,
                                            AMI_BMC_LUN,
                                            IPMI_STORAGE_GET_SEL_TIME,
                                            NULL,
                                            0,
                                            (UINT8*)&SelTimeResponse,
                                            &ResponseDataSize );

    DEBUG ((DEBUG_INFO, "%a - IPMI_STORAGE_GET_SEL_TIME status %r\n", __FUNCTION__, Status));

    if (EFI_ERROR(Status)) {
        return Status;
    }

    CopyMem (
        &CurrentBmcTime,
        &SelTimeResponse.Timestamp,
        sizeof (SelTimeResponse.Timestamp) );

    // Read the BIOS time using RuntimeServices->GetTime().
    Status = gRT->GetTime (
                    &Time,
                    NULL );

    DEBUG ((DEBUG_INFO, "%a - gRT->GetTime status %r\n", __FUNCTION__, Status));
    
    if (EFI_ERROR (Status)) {
        return Status;
    }

    NumOfYears = (UINT16)(Time.Year - 1970);
  
    NumOfLeapYears = CountNumOfLeapYears (Time.Year);
    
    /* If this is an actual leap year, we need to reduce the count by 1
       if it's not past February.  This will allow the proper number of
       days to be calculated later.*/
    if (IsLeapYear (Time.Year) && (Time.Month < 3)) {
        ASSERT (NumOfLeapYears > 0);
        NumOfLeapYears--;
    }

    NumOfDays = CalculateNumOfDayPassedThisYear (Time);

    // Add 365 days for all years. Add additional days for Leap Years.
    NumOfDays += (NumOfLeapYears + (365 * (NumOfYears)) - 1);

    NumOfSeconds = (UINT32)(3600 * 24 * NumOfDays + (Time.Hour * 3600) + \
                    (60 * Time.Minute) + Time.Second);

    /* Because the precision of the BMC time is 1 second, there is chance
       that the difference between BMC time and BIOS time to vary by
       1 second, also considering the lag between BMC time read and BIOS
       time read another 1 second variation is allowed.*/
    DEBUG ((DEBUG_INFO, "%a - CurrentBmcTime %x NumOfSeconds: %x \n", __FUNCTION__, CurrentBmcTime, NumOfSeconds));

    if ((CurrentBmcTime < (NumOfSeconds - 2)) || \
        (CurrentBmcTime > (NumOfSeconds + 2))) {
        // Log Time stamp Clock Synch First Event of pair.
#if DxeSelStatusCode_SUPPORT
        if (AmiIpmiIsSelFull (
                &gIpmiInstance->IpmiTransport,
                NULL ) == TRUE) {
            Size = sizeof (SERVER_MGMT_CONFIGURATION_DATA);
            Status = gRT->GetVariable (
                            L"ServerSetup",
                            &gEfiServerMgmtSetupVariableGuid,
                            &Attributes,
                            &Size,
                            &ServerMgmtConfiguration );
            DEBUG ((DEBUG_INFO, " gEfiServerMgmtSetupVariableGuid Status: %r \n", Status));
            if (ServerMgmtConfiguration.SelFull == AMI_IPMI_ERASE_WHEN_SEL_FULL) {
                AmiIpmiEraseSelElogData (
                    &gIpmiInstance->IpmiTransport,
                    AMI_IPMI_ERASE_WHEN_SEL_FULL );
            } else if (ServerMgmtConfiguration.SelFull == AMI_IPMI_DEL_WHEN_SEL_FULL){
                AmiIpmiEraseSelElogData (
                    &gIpmiInstance->IpmiTransport,
                    AMI_IPMI_DEL_WHEN_SEL_FULL );
            }
        }
#endif
        if (USE_PLATFORM_EVENT_MESSAGE) {
            ResponseDataSize = 1;
            Status = gIpmiInstance->IpmiTransport.SendIpmiCommand (
                                                    &gIpmiInstance->IpmiTransport,
                                                    IPMI_NETFN_SENSOR_EVENT,
                                                    AMI_BMC_LUN,
                                                    IPMI_SENSOR_PLATFORM_EVENT_MESSAGE,
                                                    (UINT8 *)&gEventMessage,
                                                    sizeof (gEventMessage),
                                                    &CompletionCode,
                                                    &ResponseDataSize );
            
            DEBUG ((DEBUG_INFO, "%a - Status of Platform Event Message command= %r\n", __FUNCTION__, Status));
        } else {
            ResponseDataSize = sizeof (AddSelEntryResponse);
            Status = gIpmiInstance->IpmiTransport.SendIpmiCommand (
                                                    &gIpmiInstance->IpmiTransport,
                                                    IPMI_NETFN_STORAGE,
                                                    AMI_BMC_LUN,
                                                    IPMI_STORAGE_ADD_SEL_ENTRY,
                                                    (UINT8 *)&gTimeStampEvtRecord,
                                                    sizeof (gTimeStampEvtRecord),
                                                    (UINT8 *)&AddSelEntryResponse,
                                                    &ResponseDataSize );
            DEBUG ((DEBUG_INFO, "%a - Time stamp - First Event pair IPMI_STORAGE_ADD_SEL_ENTRY status: %r CompletionCode: %x\n", __FUNCTION__, Status, AddSelEntryResponse.CompletionCode));
        }
        if (EFI_ERROR (Status) || (AddSelEntryResponse.CompletionCode)) {
            return Status;
        }

        // Send SEL Time Command using Set SEL time Command.
        ResponseDataSize = 1;
        Status = gIpmiInstance->IpmiTransport.SendIpmiCommand (
                                                &gIpmiInstance->IpmiTransport,
                                                IPMI_NETFN_STORAGE,
                                                AMI_BMC_LUN,
                                                IPMI_STORAGE_SET_SEL_TIME,
                                                (UINT8 *)&NumOfSeconds,
                                                sizeof (NumOfSeconds),
                                                &CompletionCode,
                                                &ResponseDataSize );

        DEBUG ((DEBUG_INFO, "%a - IPMI_STORAGE_SET_SEL_TIME status %r\n", __FUNCTION__, Status));

        if (EFI_ERROR (Status)) {
            return Status;
        }

        // Log Time stamp Clock Synch Second Event of pair.
#if DxeSelStatusCode_SUPPORT
        if (AmiIpmiIsSelFull (
                &gIpmiInstance->IpmiTransport,
                NULL ) == TRUE) {
            Size = sizeof (SERVER_MGMT_CONFIGURATION_DATA);
            Status = gRT->GetVariable (
                            L"ServerSetup",
                            &gEfiServerMgmtSetupVariableGuid,
                            &Attributes,
                            &Size,
                            &ServerMgmtConfiguration );
            DEBUG ((DEBUG_INFO, " gEfiServerMgmtSetupVariableGuid Status: %r \n", Status));
            if (ServerMgmtConfiguration.SelFull == AMI_IPMI_ERASE_WHEN_SEL_FULL) {
                AmiIpmiEraseSelElogData (
                    &gIpmiInstance->IpmiTransport,
                    AMI_IPMI_ERASE_WHEN_SEL_FULL );
            } else if (ServerMgmtConfiguration.SelFull == AMI_IPMI_DEL_WHEN_SEL_FULL){
                AmiIpmiEraseSelElogData (
                    &gIpmiInstance->IpmiTransport,
                    AMI_IPMI_DEL_WHEN_SEL_FULL );
            }
        }
#endif
        if (USE_PLATFORM_EVENT_MESSAGE) {
            gEventMessage.OEMEvData2 |= 0x80; // Set Event second of pair bit
            ResponseDataSize = 1;
            Status = gIpmiInstance->IpmiTransport.SendIpmiCommand (
                                                    &gIpmiInstance->IpmiTransport,
                                                    IPMI_NETFN_SENSOR_EVENT,
                                                    AMI_BMC_LUN,
                                                    IPMI_SENSOR_PLATFORM_EVENT_MESSAGE,
                                                    (UINT8 *)&gEventMessage,
                                                    sizeof (gEventMessage),
                                                    &CompletionCode,
                                                    &ResponseDataSize );

            DEBUG ((DEBUG_INFO, "%a - Status of Platform Event Message command= %r\n", __FUNCTION__, Status));
        } else {
            gTimeStampEvtRecord.OEMEvData2 |= 0x80; // Set Event second of pair bit
            ResponseDataSize = sizeof (AddSelEntryResponse);
            Status = gIpmiInstance->IpmiTransport.SendIpmiCommand (
                                                    &gIpmiInstance->IpmiTransport,
                                                    IPMI_NETFN_STORAGE,
                                                    AMI_BMC_LUN,
                                                    IPMI_STORAGE_ADD_SEL_ENTRY,
                                                    (UINT8 *)&gTimeStampEvtRecord,
                                                    sizeof (gTimeStampEvtRecord),
                                                    (UINT8 *)&AddSelEntryResponse,
                                                    &ResponseDataSize );
            DEBUG ((DEBUG_INFO, "%a - Time stamp - second Event pair IPMI_STORAGE_ADD_SEL_ENTRY status: %r CompletionCode: %x\n", __FUNCTION__, Status, AddSelEntryResponse.CompletionCode));
        }
        if (EFI_ERROR (Status) || (AddSelEntryResponse.CompletionCode)) {
            return Status;
        }

        if (UtcOffsetResponse.UtcOffset != EFI_UNSPECIFIED_TIMEZONE) {
            // Set current UTC offset  from BMC.
            ResponseDataSize = 1;
            Status = gIpmiInstance->IpmiTransport.SendIpmiCommand (
                                                    &gIpmiInstance->IpmiTransport,
                                                    IPMI_NETFN_STORAGE,
                                                    AMI_BMC_LUN,
                                                    AMI_STORAGE_SET_SEL_TIME_UTC_OFFSET,
                                                    (UINT8 *)&UtcOffsetResponse.UtcOffset,
                                                    sizeof (UtcOffsetResponse.UtcOffset),
                                                    &CompletionCode,
                                                    &ResponseDataSize );
            DEBUG ((DEBUG_INFO, "%a - AMI_STORAGE_SET_SEL_TIME_UTC_OFFSET status = %r  Value = %04x\n", __FUNCTION__, Status,UtcOffsetResponse.UtcOffset));
        }
    }

    DEBUG ((DEBUG_INFO, "%a - Exiting...\n", __FUNCTION__));
    return Status;
}

/**
    Notification function for Real Time Clock Arch protocol.
    Set time stamp to BMC, Validates time zone setup values and sets default
    time zone values in setup variable and also sets UTC offset in BMC.

    @param[in] Event    Event which caused this handler.
    @param[in] Context  Context passed during Event Handler registration.

    @return VOID    Nothing.

**/
VOID
EFIAPI
RealTimeClockArchProtocolNotification (
  IN  EFI_EVENT     Event,
  IN  VOID          *Context )
{
    EFI_STATUS      Status;
    VOID            *RealTimeClockArchProtocol = NULL;

    DEBUG ((DEBUG_INFO, "%a Entered...\n", __FUNCTION__));

    Status = gBS->LocateProtocol (
                    &gEfiRealTimeClockArchProtocolGuid,
                    NULL,
                    (VOID **)&RealTimeClockArchProtocol );

    DEBUG ((DEBUG_INFO, "gBS->LocateProtocol gEfiRealTimeClockArchProtocolGuid protocol  status %r\n", Status));

    if (!EFI_ERROR (Status)) {
        SetTimeStamp ();
    }
    DEBUG ((DEBUG_INFO, "%a Exiting...\n", __FUNCTION__));
}


/** @internal
    1. Validates Time Zone setup variable.
    2. Converts time zone string to UTC offset format.

    @param[in] TimeZone     Time Zone string entered in setup.

    @retval INT16   UTC offset obtained from Time Zone string.

**/
INT16
GetUtcOffsetFromTimeZone (
  IN CHAR16     TimeZone[] )
{
    INT16   UtcOffset = 0;

    DEBUG ((DEBUG_INFO, "%a Entered... TimeZone: %S \n", __FUNCTION__, TimeZone));

    /* If setup variable is not found then return Unspecified Time zone
       and also if 0x07FF is entered in setup, return Unspecified Time zone.*/
    if (EFI_ERROR (gSetupStatus)) {
        goto ErrorExit;
    } else if ((TimeZone[0] == '0') && \
                ((TimeZone[1] == 'x') || (TimeZone[1] == 'X')) && \
                (TimeZone[2] == '0') && \
                (TimeZone[3] == '7') && \
                ((TimeZone[4] == 'f') || (TimeZone[4] == 'F')) && \
                ((TimeZone[5] == 'f') || (TimeZone[5] == 'F'))) {
        goto ErrorExit;
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
        DEBUG ((DEBUG_INFO," \n After Converting char to number - 1's position UtcOffset: %d \n", UtcOffset));
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
    if (!((UtcOffset <= MAX_UTC_OFFSET) && \
            (UtcOffset >= MIN_UTC_OFFSET))) {
        goto ErrorExit;
    }

    // Check for sign and form UtcOffset accordingly.
    if (TimeZone[0] == '-') {
        UtcOffset *= -1;
    }
    return UtcOffset;

ErrorExit:
    // Return unspecified time zone.
    return EFI_UNSPECIFIED_TIMEZONE;
}

/** @internal
    Get SEL Time UTC Offset from BMC. Compare both Bmc UTC Offset
    and UTC Offset entered in Setup, if they are different, Bmc UTC Offset
    is updated to UTC Offset entered in Setup using Set SEL Time UTC Offset
    command.

    @param VOID     Nothing.

    @retval EFI_SUCCESS     Time zone is set in Bmc.
    @retval Others          Error status returned while executing command.

**/
EFI_STATUS
SetTimeZone (
  VOID )
{
    EFI_STATUS                              Status;
    UINT8                                   ResponseDataSize;
    INT16                                   SetupUtcOffset = 0x00;
    AMI_GET_SEL_TIME_UTC_OFFSET_RESPONSE    UtcOffsetResponse;
    UINT8                                   CompletionCode;

    DEBUG ((DEBUG_INFO, "%a Entered...\n", __FUNCTION__));

    // Convert Setup TimeZone string to UTC offset in minutes.
    SetupUtcOffset = GetUtcOffsetFromTimeZone (gServerMgmtConfiguration.TimeZone);
    DEBUG ((DEBUG_INFO,"SetupUtcOffset: %x \n", SetupUtcOffset));

    if (!EFI_ERROR(gSetupStatus)) {
        // Set default Time zone values - 0x07FF.
        gServerMgmtConfiguration.TimeZone[0] ='0';
        gServerMgmtConfiguration.TimeZone[1] ='x';
        gServerMgmtConfiguration.TimeZone[2] ='0';
        gServerMgmtConfiguration.TimeZone[3] ='7';
        gServerMgmtConfiguration.TimeZone[4] ='F';
        gServerMgmtConfiguration.TimeZone[5] ='F';

        gRT->SetVariable (
                L"ServerSetup",
                &gEfiServerMgmtSetupVariableGuid,
                gAttributes,
                sizeof(SERVER_MGMT_CONFIGURATION_DATA),
                &gServerMgmtConfiguration );
    }

    // Get SEL Time UTC Offset Command.
    ResponseDataSize = sizeof (UtcOffsetResponse);
    Status = gIpmiInstance->IpmiTransport.SendIpmiCommand (
                                            &gIpmiInstance->IpmiTransport,
                                            IPMI_NETFN_STORAGE,
                                            AMI_BMC_LUN,
                                            AMI_STORAGE_GET_SEL_TIME_UTC_OFFSET,
                                            NULL,
                                            0,
                                            (UINT8 *)&(UtcOffsetResponse),
                                            &ResponseDataSize );

    DEBUG ((DEBUG_INFO, "AMI_STORAGE_GET_SEL_TIME_UTC_OFFSET status: %r CompletionCode: %x\n", Status, UtcOffsetResponse.CompletionCode));
    
    if (EFI_ERROR(Status) || (UtcOffsetResponse.CompletionCode)) {
        return Status;
    }

    DEBUG ((DEBUG_INFO,"BmcUtcOffset: %x \n", UtcOffsetResponse.UtcOffset));

    /** If both Setup and BMC UTC offset are same then Return
        else, Set Setup UTC offset to BMC.*/
    if ((SetupUtcOffset == UtcOffsetResponse.UtcOffset) || \
         (SetupUtcOffset == EFI_UNSPECIFIED_TIMEZONE)) {
        DEBUG ((DEBUG_INFO,"Set SEL Time UTC Offset Command is not send to BMC as SetupUtcOffset = BmcUtcOffset/0x7FF.\n"));
        return EFI_SUCCESS;
    }

    /** Set SEL Time UTC Offset Command
        Whenever the Set SEL Time command is executed, UTC Offset must be
        re-written after using the Set SEL Time command.*/
    ResponseDataSize = 1;
    Status = gIpmiInstance->IpmiTransport.SendIpmiCommand (
                                            &gIpmiInstance->IpmiTransport,
                                            IPMI_NETFN_STORAGE,
                                            AMI_BMC_LUN,
                                            AMI_STORAGE_SET_SEL_TIME_UTC_OFFSET,
                                            (UINT8 *)&(SetupUtcOffset),
                                            sizeof (SetupUtcOffset),
                                            &CompletionCode,
                                            &ResponseDataSize );

    DEBUG ((DEBUG_INFO, "AMI_STORAGE_SET_SEL_TIME_UTC_OFFSET status %r\n", Status));
    
    if (EFI_ERROR(Status)) {
        return Status;
    }

    DEBUG ((DEBUG_INFO, "%a Exiting...\n", __FUNCTION__));

    return EFI_SUCCESS;
}

/**
    Notification function for Virtual address change event. Convert all
    addresses inside IpmiTransportProtocol.

    @param[in] Event    Event which caused this handler.
    @param[in] Context  Context passed during Event Handler registration.

    @return VOID    Nothing.

**/
VOID
EFIAPI
DxeIpmiIntializeVirtualAddressChangeEvent (
  IN EFI_EVENT  Event,
  IN VOID       *Context )
{
    // Change all addresses inside IpmiTransportProtocol stored locally.
    gRT->ConvertPointer (
            0,
            (VOID **) &(gIpmiInstance->IpmiTransport.SendIpmiCommand) );

    gRT->ConvertPointer (
            0,
            (VOID **) &(gIpmiInstance->IpmiTransport.GetBmcStatus) );

    gRT->ConvertPointer (
            0,
            (VOID **) &(gIpmiInstance->IpmiTransport.SendIpmiCommandEx) );

    // Change the address pointed by gIpmiInstance.
    gRT->ConvertPointer (
            0,
            (VOID **) &gIpmiInstance );
}

/** @internal
    Install Dxe Ipmi transport protocol.

    @param  VOID    Nothing.

    @return EFI_STATUS  Status returned from functions used.

**/
EFI_STATUS
IpmiInstallProtocol (
  VOID )
{
    EFI_STATUS                      Status = EFI_SUCCESS;
    EFI_STATUS                      StatusOfGetDeviceId = EFI_DEVICE_ERROR;
    EFI_BMC_SELF_TEST_LOG_PROTOCOL  *BmcSelfTestProtocol = NULL;
    IPMI_GET_DEVICE_ID_RESPONSE     GetDeviceIdResponse;
    IPMI_SELF_TEST_RESULT_RESPONSE  BstStatus;
    EFI_STATUS_CODE_VALUE           StatusCodeValue[AMI_IPMI_BST_LOG_SIZE];
    EFI_EVENT                       EfiVirtualAddressChangeNotifyEvent = NULL;
    UINT8                           Index;
    UINT8                           ErrorCount = 0;
    UINT8                           BitSelect = 0;
    UINT8                           ErrorBitVal = 0;

    DEBUG ((DEBUG_INFO, "%a - Entry\n", __FUNCTION__));

    // IPMI Protocol Revision.
    gIpmiInstance->IpmiTransport.Revision           = EFI_IPMI_PROTOCOL_REVISION;

    Status = AmiIpmiGetSelfTestResults (
                                &gIpmiInstance->IpmiTransport,
                                &BstStatus );

    // If error status, then the BMC is not responding, so send an error.
    if (EFI_ERROR (Status)) {
        if (ErrorCount < AMI_IPMI_BST_LOG_SIZE) {
            StatusCodeValue[ErrorCount++] = EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | \
                                            EFI_CU_FP_EC_COMM_ERROR;
        }
        gIpmiInstance->BmcStatus = AmiBmcStatusHardFail;
    }

    if (gIpmiInstance->BmcStatus == AmiBmcStatusSoftFail) {

        // Check the Error bit set and report the error if BmcStatus is soft fail.
        ErrorBitVal = BstStatus.Param;

        // Loop until all bits are checked. BitSelect initial value is 0.
        while ((BitSelect < 8) && ErrorBitVal) {
            if (ErrorBitVal & BIT0) { // Check if selected bit is set.
                if (ErrorCount < AMI_IPMI_BST_LOG_SIZE) {
                    // Store the status code mapped for this error bit.
                    StatusCodeValue[ErrorCount++] = gErrorBitToStatusCodeMap[BitSelect];
                } else {
                    break;
                }
            }
            // Select the next bit to be checked.
            BitSelect = BitSelect + 1;
            // Remove tested bit from the value.
            ErrorBitVal = ErrorBitVal >> 1;
        }
        if (ErrorCount < AMI_IPMI_BST_LOG_SIZE) {
            StatusCodeValue[ErrorCount++] = EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | \
                                            EFI_CU_FP_EC_SOFT_FAIL;
        }
    } else if (gIpmiInstance->BmcStatus == AmiBmcStatusHardFail) {
        if (ErrorCount < AMI_IPMI_BST_LOG_SIZE) {
            StatusCodeValue[ErrorCount++] = EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | \
                                            EFI_CU_FP_EC_HARD_FAIL;
        }
    }

    /* The Device ID information is only retrieved when the self test
       has passed or generated a SOFTFAIL. The BMC Status may be
       overwritten to show the BMC is in Force Update mode. But the BMC
       Status will always be preserved if a hard fail has occurred since
       this code won't execute.*/
    if ((gIpmiInstance->BmcStatus == AmiBmcStatusOk) || \
        (gIpmiInstance->BmcStatus == AmiBmcStatusSoftFail)) {
        if (IPMI_BMC_TIME_SYNC_TO_BIOS_TIME_SUPPORT) {
            VOID  *NotifyRegistration;

            // Create Notification event for Real time clock arch protocol guid.
            EfiCreateProtocolNotifyEvent (
                &gEfiRealTimeClockArchProtocolGuid,
                TPL_CALLBACK,
                RealTimeClockArchProtocolNotification,
                NULL,
                &NotifyRegistration );
        }

        StatusOfGetDeviceId = AmiIpmiGetDeviceId (
                                            &gIpmiInstance->IpmiTransport,
                                            &GetDeviceIdResponse );
        
        if(!EFI_ERROR (StatusOfGetDeviceId)) {

            /* If the controller is in Update Mode and the maximum
               number of errors has not been exceeded, then save the
               error code to the StatusCode array and increment the
               counter. Set the BMC Status to indicate the BMC is in
               force update mode.*/
            if (gIpmiInstance->BmcStatus == AmiBmcStatusUpdateInProgress) {
                if (ErrorCount < AMI_IPMI_BST_LOG_SIZE) {
                    StatusCodeValue[ErrorCount++] = EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR\
                                                      | EFI_CU_FP_EC_FORCE_UPDATE_MODE;
                }
            }
        }
    }

    DEBUG ((DEBUG_INFO, "BMC status %x\n", gIpmiInstance->BmcStatus));

    for (Index = 0; Index < ErrorCount; Index++) {
        ReportStatusCode (
            EFI_ERROR_CODE | EFI_ERROR_MINOR,
            StatusCodeValue[Index] );
    }

    /* Install BmcSelfTestProtocolLog protocol.
       It is used in BMC Self Test Log Setup page to display BMC self test results.
       Allocate memory for BMC Self-test log Instance.*/
    BmcSelfTestProtocol = AllocateZeroPool (sizeof (EFI_BMC_SELF_TEST_LOG_PROTOCOL));
    
    if (BmcSelfTestProtocol == NULL) {
        DEBUG ((DEBUG_ERROR, "Failed to allocate memory for BmcSelfTestProtocol! \n"));
        ASSERT (BmcSelfTestProtocol != NULL);
    } else {

        /* GetDeviceId command succeed, Copy the BMC details into self test
           protocol to display it in setup.*/
        if (!EFI_ERROR (StatusOfGetDeviceId)) {
            BmcSelfTestProtocol->IsGetDeviceIdCommandPassed = TRUE;
            CopyMem (
                &BmcSelfTestProtocol->GetDeviceIdResponse,
                &GetDeviceIdResponse,
                sizeof (GetDeviceIdResponse) );
        } else {
            BmcSelfTestProtocol->IsGetDeviceIdCommandPassed = FALSE;
        }

        BmcSelfTestProtocol->BmcStatus = gIpmiInstance->BmcStatus;

        // Copy the Status code structure into BMC Self test protocol.
        CopyMem (
            &BmcSelfTestProtocol->BmcSelfTestError[0],
            &StatusCodeValue[0],
            ErrorCount * sizeof (EFI_STATUS_CODE_VALUE) );
        BmcSelfTestProtocol->BmcSelfTestErrorCount = ErrorCount;

        Status = gBS->InstallProtocolInterface (
                        &gNewHandle,
                        &gEfiBmcSelfTestLogProtocolGuid,
                        EFI_NATIVE_INTERFACE,
                        BmcSelfTestProtocol );

        DEBUG ((DEBUG_INFO, "BMC self test protocol Install status %r\n", Status));

        ASSERT_EFI_ERROR (Status);

        if (EFI_ERROR (Status)) {
            // Error in installing the protocol. So free the allocated memory.
            FreePool (BmcSelfTestProtocol);
        }
    }

    // Now install the Protocol if the BMC is not in a Hard Fail State.
    if (gIpmiInstance->BmcStatus != AmiBmcStatusHardFail) {

        // Add SPMI support.
        SpmiSupport ();

        // Add Ipmi SMBIOS types support.
        IpmiSmbiosTypesSupport ();

        if (BMC_TIMEZONE_SUPPORT) {
            // Set Time zone to BMC.
            SetTimeZone ();
        }


        if (PeiIpmiCmosClear_SUPPORT) {
            // CMOS Clear support.
            IpmiCmosClear ();
        }

        // Install the protocol.
        Status = gBS->InstallProtocolInterface (
                                        &gNewHandle,
                                        &gEfiDxeIpmiTransportProtocolGuid,
                                        EFI_NATIVE_INTERFACE,
                                        &gIpmiInstance->IpmiTransport );

        DEBUG ((DEBUG_INFO, "gEfiDxeIpmiTransportProtocolGuid protocol  status %r\n", Status));
       
        if (EFI_ERROR (Status)) {
            // Error in installing the protocol. So free the allocated memory.
            FreePool (gIpmiInstance);
            return Status;
        }

        Status = IpmiInstallInfoProtocol(gNewHandle);
        DEBUG ((DEBUG_INFO, "%a - Ipmi Install info Protocol Status = %r\n", __FUNCTION__, Status));

        //Create event for the Virtual Address Change Event.
        Status = gBS->CreateEventEx (
                        EVT_NOTIFY_SIGNAL,
                        TPL_NOTIFY,
                        DxeIpmiIntializeVirtualAddressChangeEvent,
                        NULL,
                        (CONST EFI_GUID *)&gEfiEventVirtualAddressChangeGuid,
                        &EfiVirtualAddressChangeNotifyEvent );

        DEBUG ((DEBUG_INFO, "Create event for the Virtual Address Change Event  status %r\n", Status));

    }

    DEBUG ((DEBUG_INFO, "%a - Exit\n", __FUNCTION__));
    return Status;
}

/**
    Notify call back function.

    @param[in] Event    Event which caused this handler.
    @param[in] Context  Context passed during Event Handler registration.

    @return VOID    Nothing.

**/
VOID
EFIAPI
DxeNotifyCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context )
{
    EFI_STATUS  Status = EFI_SUCCESS;

    DEBUG ((DEBUG_INFO, "%a - Entry\n", __FUNCTION__));

    Status = AmiIpmiInitInterfaceData (gIpmiInstance);

    // Return if there is no memory found to allocate.
    if (Status == EFI_OUT_OF_RESOURCES) {
        return;
    }

    switch (DEFAULT_SYSTEM_INTERFACE) {

        case AmiSysInterfaceKcs:
            if ((gIpmiInstance->Interface.Kcs != NULL) && \
                (gIpmiInstance->Interface.Kcs->InterfaceState != AmiIpmiInterfaceInitialized)) {
                DEBUG ((DEBUG_INFO, "AmiSysInterfaceKcs AmiIpmiInterfaceNotReady in %a \n", __FUNCTION__));
                return;
            }
            break;

        case AmiSysInterfaceBt:
            if (gIpmiInstance->Interface.Bt.InterfaceState != AmiIpmiInterfaceInitialized) {
                DEBUG ((DEBUG_INFO, "AmiSysInterfaceBt AmiIpmiInterfaceNotReady in %a \n", __FUNCTION__));
                return;
            }
            break;

        case AmiSysInterfaceSsif:
            if (gIpmiInstance->Interface.Ssif.InterfaceState != AmiIpmiInterfaceInitialized) {
                DEBUG ((DEBUG_INFO, "AmiSysInterfaceSsif AmiIpmiInterfaceNotReady in %a \n", __FUNCTION__));
                return;
            }
            break;

        case AmiSysInterfaceIpmb:
            if (gIpmiInstance->Interface.Ipmb.InterfaceState != AmiIpmiInterfaceInitialized) {
                DEBUG ((DEBUG_INFO, "AmiSysInterfaceIpmb AmiIpmiInterfaceNotReady in %a \n", __FUNCTION__));
                return;
            }
            break;

        case AmiSysInterfaceUsb:
            if ((gIpmiInstance->Interface.Usb != NULL) && \
                (gIpmiInstance->Interface.Usb->InterfaceState != AmiIpmiInterfaceInitialized)) {
                DEBUG ((DEBUG_INFO, "AmiSysInterfaceUsb AmiIpmiInterfaceNotReady in %a \n", __FUNCTION__));
                return;
            }
            break;

        case AmiSysInterfaceOem1:
            if (gIpmiInstance->Interface.Oem1.InterfaceState != AmiIpmiInterfaceInitialized) {
                DEBUG ((DEBUG_INFO, "AmiSysInterfaceOem1 AmiIpmiInterfaceNotReady in %a \n", __FUNCTION__));
                return;
            }
            break;

        case AmiSysInterfaceOem2:
            if (gIpmiInstance->Interface.Oem2.InterfaceState != AmiIpmiInterfaceInitialized) {
                DEBUG ((DEBUG_INFO, "AmiSysInterfaceOem2 AmiIpmiInterfaceNotReady in %a \n", __FUNCTION__));
                return;
            }
            break;
            
        default:
            break;
    }

    if (gIpmiInstance->IpmiTransport.Revision != EFI_IPMI_PROTOCOL_REVISION) {
        Status = IpmiInstallProtocol ();
        DEBUG ((DEBUG_INFO, "%a - IpmiInstallProtocol Status = %r\n", __FUNCTION__, Status));
    }

    DEBUG ((DEBUG_INFO, "%a Exit\n", __FUNCTION__));
}

/** @internal
    Registers protocol notify call back.

    @param[in] ProtocolGuid     Pointer to Protocol Guid to register
                                call back.

    @retval EFI_INVALID_PARAMETER   If the ProtocolGuid is 0 or NULL.
    @retval Others                  Status of call back registration.

**/
EFI_STATUS
DxeRegisterProtocolCallback (
  IN EFI_GUID   *ProtocolGuid )
{
    EFI_STATUS  Status;
    EFI_EVENT   NotifyEvent;
    VOID        *Registration;
    EFI_GUID    NullGuid;

    ZeroMem (
       &NullGuid,
       sizeof (EFI_GUID) );

    if ((ProtocolGuid == NULL) || \
        ((ProtocolGuid != NULL) && \
        CompareGuid (
            ProtocolGuid,
            &NullGuid ))) {
        return EFI_INVALID_PARAMETER;
    }

    Status = gBS->CreateEvent (
                    EVT_NOTIFY_SIGNAL,
                    TPL_NOTIFY,
                    DxeNotifyCallback,
                    NULL,
                    &NotifyEvent );
    
    if (!EFI_ERROR (Status)) {
        Status = gBS->RegisterProtocolNotify (
                        ProtocolGuid,
                        NotifyEvent,
                        &Registration );
    }
    return Status;
}

/**
    Setup and initialize the BMC for the DXE phase. In order to 
    verify the BMC is functioning as expected, the BMC Self test is
    performed. The results are then checked and any errors are reported
    to the error manager. Errors are collected throughout this routine
    and reported just prior to installing the driver. If there are more
    errors than AMI_IPMI_BST_LOG_SIZE, then they will be ignored.

    @param[in] ImageHandle  Handle of this driver image.
    @param[in] SystemTable  Table containing standard EFI services.

    @retval EFI_SUCCESS             Successful driver initialization.
    @retval EFI_UNSUPPORTED         BmcSupport setup option is disabled.
    @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.
    @retval Others                  Error status while initializing the
                                    driver.

**/
EFI_STATUS
EFIAPI
InitializeIpmiPhysicalLayer (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable )
{
    EFI_STATUS                  Status = EFI_SUCCESS;
    UINT8                       Index;
    UINTN                       Size;
    AMI_IPMI_INTERFACE_STATE    InterfaceState = AmiIpmiInterfaceNotReady;

    InitAmiLib (
        ImageHandle,
        SystemTable );

    Size = sizeof (SERVER_MGMT_CONFIGURATION_DATA);
    gSetupStatus = gRT->GetVariable (
                            L"ServerSetup",
                            &gEfiServerMgmtSetupVariableGuid,
                            &gAttributes,
                            &Size,
                            &gServerMgmtConfiguration );

    DEBUG ((DEBUG_INFO, "GetVariable Status of ServerSetup :%r, ServerMgmtConfiguration.BmcSupport: %x \n", gSetupStatus, gServerMgmtConfiguration.BmcSupport));

    if (EFI_ERROR(gSetupStatus)) {
        gServerMgmtConfiguration.BmcSupport = DEFAULT_BMC_SUPPORT;
    }
    
    if (!gServerMgmtConfiguration.BmcSupport) {
        return EFI_UNSUPPORTED;
    }

    // Allocate memory for Ipmi Instance.
    gIpmiInstance = (AMI_IPMI_BMC_INSTANCE_DATA *)AllocateRuntimeZeroPool (sizeof (AMI_IPMI_BMC_INSTANCE_DATA));
    if (gIpmiInstance == NULL) {
        DEBUG ((DEBUG_ERROR, "Failed to allocate memory for gIpmiInstance! \n"));
        return EFI_OUT_OF_RESOURCES;
    }

    Status = AmiIpmiInitInterfaceData (gIpmiInstance);

    if (Status == EFI_OUT_OF_RESOURCES) {
        FreePool (gIpmiInstance);
        return Status;
    }

    // Check default interface data initialized successfully else register notify protocol.
    for (Index = AmiSysInterfaceKcs; Index < AmiSysInterfaceMax; Index++) {
        switch (Index) {
            case AmiSysInterfaceKcs:
                if ((gIpmiInstance->Interface.Kcs != NULL) && \
                    (gIpmiInstance->Interface.Kcs->InterfaceState == AmiIpmiInterfaceInitialized) &&\
                    (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceKcs)) {
                    InterfaceState = AmiIpmiInterfaceInitialized;
                    DEBUG ((DEBUG_INFO, "Default AmiSysInterfaceKcs AmiIpmiInterfaceInitialized\n"));
                }
                break;

            case AmiSysInterfaceBt:
                if ((gIpmiInstance->Interface.Bt.InterfaceState == AmiIpmiInterfaceInitialized) &&\
                    (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceBt)) {
                    InterfaceState = AmiIpmiInterfaceInitialized;
                    DEBUG ((DEBUG_INFO, "Default AmiSysInterfaceBt AmiIpmiInterfaceInitialized\n"));
                }
                break;

            case AmiSysInterfaceSsif:
                if ((gIpmiInstance->Interface.Ssif.InterfaceState == AmiIpmiInterfaceInitialized) &&\
                    (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceSsif)) {
                    InterfaceState = AmiIpmiInterfaceInitialized;
                    DEBUG ((DEBUG_INFO, "Default AmiSysInterfaceSsif AmiIpmiInterfaceInitialized\n"));
                } else if (gIpmiInstance->Interface.Ssif.InterfaceState == AmiIpmiInterfaceInitError) {
                    // Register protocol notify for SMBUS Protocol.
                    Status = DxeRegisterProtocolCallback (
                                &gIpmiInstance->Interface.Ssif.SsifInterfaceApiGuid );
                    DEBUG ((DEBUG_INFO, "SMBUS Protocol is not ready. RegisterProtocolCallback Status: %r\n", Status));
                }
                break;

            case AmiSysInterfaceIpmb:
                if ((gIpmiInstance->Interface.Ipmb.InterfaceState == AmiIpmiInterfaceInitialized) &&\
                    (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceIpmb)) {
                    InterfaceState = AmiIpmiInterfaceInitialized;
                    DEBUG ((DEBUG_INFO, "Default AmiSysInterfaceIpmb AmiIpmiInterfaceInitialized\n"));
                } else if (gIpmiInstance->Interface.Ipmb.InterfaceState == AmiIpmiInterfaceInitError) {
                    // Register Protocol notify for I2C Protocol.
                    Status = DxeRegisterProtocolCallback (
                                &gIpmiInstance->Interface.Ipmb.IpmbInterfaceApiGuid );
                    DEBUG ((DEBUG_INFO, "I2C Protocol is not ready. RegisterProtocolCallback Status: %r\n", Status));
                }
                break;

            case AmiSysInterfaceUsb:
                if ((gIpmiInstance->Interface.Usb != NULL) && \
                    (gIpmiInstance->Interface.Usb->InterfaceState == AmiIpmiInterfaceInitialized) &&\
                    (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceUsb)) {
                    InterfaceState = AmiIpmiInterfaceInitialized;
                    DEBUG ((DEBUG_INFO, "Default AmiSysInterfaceUsb AmiIpmiInterfaceInitialized\n"));
                }else if ((gIpmiInstance->Interface.Usb != NULL) && \
                          (gIpmiInstance->Interface.Usb->InterfaceState == AmiIpmiInterfaceInitError) && \
                          ((DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceUsb) || (!BMC_USB_DRIVER_BINDING_SUPPORT))) {
                    // Register Protocol notify for USB Protocol.
                    Status = DxeRegisterProtocolCallback (
                                &gIpmiInstance->Interface.Usb->UsbInterfaceApiGuid );
                    DEBUG ((DEBUG_INFO, "USB Protocol is not ready. RegisterProtocolCallback Status: %r\n", Status));
                }
                break;

            case AmiSysInterfaceOem1:
                if ((gIpmiInstance->Interface.Oem1.InterfaceState == AmiIpmiInterfaceInitialized) &&\
                    (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceOem1)) {
                    InterfaceState = AmiIpmiInterfaceInitialized;
                    DEBUG ((DEBUG_INFO, "Default AmiSysInterfaceOem1 AmiIpmiInterfaceInitialized\n"));
                } else if (gIpmiInstance->Interface.Oem1.InterfaceState == AmiIpmiInterfaceInitError) {
                    // Register Protocol notify for OEM Protocol.
                    Status = DxeRegisterProtocolCallback (
                                &gIpmiInstance->Interface.Oem1.Oem1InterfaceApiGuid );
                    DEBUG ((DEBUG_INFO, "OEM1 Protocol is not ready. RegisterProtocolCallback Status: %r\n", Status));
                }
                break;

            case AmiSysInterfaceOem2:
                if ((gIpmiInstance->Interface.Oem2.InterfaceState == AmiIpmiInterfaceInitialized) &&\
                    (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceOem2)) {
                    InterfaceState = AmiIpmiInterfaceInitialized;
                    DEBUG ((DEBUG_INFO, "Default AmiSysInterfaceOem2 AmiIpmiInterfaceInitialized\n"));
                } else if (gIpmiInstance->Interface.Oem2.InterfaceState == AmiIpmiInterfaceInitError) {
                    // Register Protocol notify for OEM Protocol.
                    Status = DxeRegisterProtocolCallback (
                                &gIpmiInstance->Interface.Oem2.Oem2InterfaceApiGuid );
                    DEBUG ((DEBUG_INFO, "OEM2 Protocol is not ready. RegisterProtocolCallback Status: %r\n", Status));
                }
                break;

            default:
                break;
        }
    }

    // Default Interface data should be initialized to install Ipmi Protocol.
    if (InterfaceState != AmiIpmiInterfaceInitialized) {
        DEBUG ((DEBUG_INFO, "Interface not ready yet. \n"));  
        return EFI_SUCCESS;
    }

    // Install Ipmi Protocol interface.
    Status = IpmiInstallProtocol ();

    DEBUG ((DEBUG_INFO, "%a - IpmiInstallProtocol Status = %r\n", __FUNCTION__, Status)); 

    return Status;
}
