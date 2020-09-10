
//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file BmcSelfTestLog.c
    Functions to log BMC Self test status in setup.

*/

//---------------------------------------------------------------------------

#include "BmcSelfTestLog.h"

//---------------------------------------------------------------------------

/** @internal
    Convert EFI_STATUS CODE value to match the string index in the array of
    SetupStrings.

    @param[in] Value    Value of Status code need to be converted.

    @return UINT8   String Array Index equivalent for status code.

**/
UINT8
ConvertStatusCode (
  IN EFI_STATUS_CODE_VALUE  Value )
{
    switch (Value) {

        case EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_HARD_FAIL:
            return 2;

        case EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_SOFT_FAIL:
            return 3;

        case EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_COMM_ERROR:
            return 4;

        case EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_SEL_NOT_ACCESSIBLE:
            return 5;

        case EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_SDR_REPOSITORY_NOT_ACCESSABLE:
            return 6;

        case EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_BMC_FRU_NOT_ACCESSIBLE:
            return 7;

        case EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_IPMB_NOT_RESPONDING:
            return 8;

        case EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_SDR_EMPTY:
            return 9;

        case EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_BMC_FRU_CORRUPTED:
            return 10;

        case EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_BB_CORRUPTED:
            return 11;

        case EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_FIRMWARE_CORRUPTED:
            return 12;

        case EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_FORCE_UPDATE_MODE:
            return 13;

        default :
            return 0;
    }
}

/** @internal
    This function updates Time to BMC Self Test Log

    @param[in] Index    Index of Log.

    @return VOID    Nothing.

**/
VOID
UpdateTime (
  IN UINT8 Index )
{
    EFI_TIME    Time;

    // Getting the time and date using the runtime service GetTime.
    gRT->GetTime (
            &Time,
            NULL );

    /* Filling the gBstLog NvRam variable with date and time
       of each particular log.*/
    gBstLog.Date[Index].Year    = Time.Year;
    gBstLog.Date[Index].Month   = Time.Month;
    gBstLog.Date[Index].Day     = Time.Day;
    gBstLog.Time[Index].Hour    = Time.Hour;
    gBstLog.Time[Index].Minute  = Time.Minute;
    gBstLog.Time[Index].Second  = Time.Second;
}

/**
    This function updates the BMC Self Test log in setup.

    @param[in] HiiHandle    Handle to HII database.
    @param[in] Class        Indicates the setup class.

    @return VOID    Nothing.

**/
VOID
BmcSelfTestLogStrings (
  IN EFI_HII_HANDLE     HiiHandle,
  IN UINT16             Class )
{
    EFI_STATUS                      Status = EFI_SUCCESS;
    EFI_BMC_SELF_TEST_LOG_PROTOCOL  *BmcSelfTestProtocol = NULL;
    UINT8                           Index;
    UINT8                           LogIndex;
    UINTN                           Size;
    UINT16                          StringRef;
    UINT16                          SetupStrings[14] = {
                                        STRING_TOKEN(STR_BMCSELFTESTLOG_PROTOCOL_NOT_FOUND),
                                        STRING_TOKEN(STR_BMCSELFTESTLOG_EMPTY),
                                        STRING_TOKEN(STR_BMC_HARD_FAIL),
                                        STRING_TOKEN(STR_BMC_SOFT_FAIL),
                                        STRING_TOKEN(STR_BMC_COMMUNICATION_ERROR),
                                        STRING_TOKEN(STR_SEL_DEVICE_INACCESSIBLE),
                                        STRING_TOKEN(STR_SDR_REPOSITORY_INACCESSIBLE),
                                        STRING_TOKEN(STR_BMC_FRU_DEVICE_INACCESSIBLE),
                                        STRING_TOKEN(STR_IPMB_SIGNAL_LINES_UNRESPONSIVE),
                                        STRING_TOKEN(STR_SDR_REPOSITORY_EMPTY),
                                        STRING_TOKEN(STR_BMC_FRU_AREA_CORRUPTED),
                                        STRING_TOKEN(STR_BOOTBLOCK_FIRMWARE_CORRUPTED),
                                        STRING_TOKEN(STR_CONTROLLER_OPERATIONAL_FIRMWARE_CORRUPTED),
                                        STRING_TOKEN(STR_FORCE_UPDATE_MODE)
                                    };
    EFI_STRING                      CharPtrString;
    EFI_STRING                      CharPtrString1;
    SERVER_MGMT_CONFIGURATION_DATA  ServerMgmtConfiguration;

    DEBUG ((DEBUG_INFO, "%a: Class ID:  %x\n", __FUNCTION__, Class));
    //
    // Continue only for Server Mgmt setup page
    //
    if (Class != SERVER_MGMT_CLASS_ID) {
        return;
    }

    // Get ServerMgmt Setup variable.
    Size = sizeof (SERVER_MGMT_CONFIGURATION_DATA);
    Status = gRT->GetVariable (
                    L"ServerSetup",
                    &gEfiServerMgmtSetupVariableGuid,
                    NULL,
                    &Size,
                    &ServerMgmtConfiguration );
    
    DEBUG ((DEBUG_INFO, "gRT->GetVariable ServerSetup status %r \n", Status));

    if (EFI_ERROR (Status)) {
        // Handle error case with default values.
        ServerMgmtConfiguration.BstErase = ERASE_ON_EVERY_RESET;
        ServerMgmtConfiguration.BstLogFull = CLEAR_LOG;
    }

    // Get BstLogVar NVRAM variable.
    Size = sizeof (BMC_SELF_TEST_LOG_DATA);
    Status = gRT->GetVariable (
                    L"BstLogVar",
                    &gEfiServerMgmtSetupVariableGuid,
                    NULL,
                    &Size,
                    &gBstLog );
    
    DEBUG ((DEBUG_INFO, "gRT->GetVariable BstLogVar status %r \n", Status));

    if (EFI_ERROR (Status)) {
        // Handle error case with default values.
        ServerMgmtConfiguration.BstErase = ERASE_ON_EVERY_RESET;
    }

    // Clear gBstLog NvRam variable with condition check.
    if ((ServerMgmtConfiguration.BstErase == ERASE_ON_EVERY_RESET) || \
         ((gBstLog.LogSize == AMI_IPMI_BST_LOG_SIZE) && \
         (ServerMgmtConfiguration.BstLogFull == CLEAR_LOG))) {
        ZeroMem (
            &gBstLog,
            sizeof (BMC_SELF_TEST_LOG_DATA) );
    }

    DEBUG ((DEBUG_INFO, "gBstLog.logSize:  %x \n", gBstLog.LogSize));
    LogIndex = gBstLog.LogSize;

    // Locate the BMC self test protocol.
    Status = gBS->LocateProtocol (
                    &gEfiBmcSelfTestLogProtocolGuid,
                    NULL,
                    (VOID **)&BmcSelfTestProtocol );
    
    DEBUG ((DEBUG_INFO, "LocateProtocol gEfiBmcSelfTestLogProtocolGuid: Status:  %r\n", Status));

    /* Displaying the string PASSED or FAILED depending upon the Protocol
       locate status and Error count.*/
    if ((!EFI_ERROR (Status)) && (BmcSelfTestProtocol->BmcStatus == AmiBmcStatusOk)) {
        CharPtrString = HiiGetString (
                            HiiHandle,
                            STRING_TOKEN(STR_PROTOCOL_LOCATE_STATUS_PASSED),
                            NULL );
    } else {
        CharPtrString = HiiGetString (
                            HiiHandle,
                            STRING_TOKEN(STR_PROTOCOL_LOCATE_STATUS_FAILED),
                            NULL );
    }

    if(CharPtrString != NULL) {
       HiiSetString (
           HiiHandle,
           STRING_TOKEN(STR_IPMI_BMC_SELF_TEST_STATUS_VAL),
           CharPtrString,
           NULL );
       gBS->FreePool (CharPtrString);
    }

    /* Displaying BMC details in setup. If GetDeviceId command fails, "Unknown"
       will be printed for each parameter.*/
    if ((!EFI_ERROR (Status)) && BmcSelfTestProtocol->IsGetDeviceIdCommandPassed) {
         CharPtrString = AllocateZeroPool (STRING_BUFFER_LENGTH);
         if (CharPtrString != NULL){
             // BMC Device ID.
             UnicodeSPrint(
                 CharPtrString,
                 STRING_BUFFER_LENGTH,
                 L"%d",
                 BmcSelfTestProtocol->GetDeviceIdResponse.DeviceId );

             HiiSetString (
                 HiiHandle,
                 STRING_TOKEN(STR_IPMI_BMC_DEVICE_ID_VAL),
                 CharPtrString,
                 NULL );
             // BMC Device Revision.
             UnicodeSPrint(
                CharPtrString,
                STRING_BUFFER_LENGTH,
                L"%x",
                BmcSelfTestProtocol->GetDeviceIdResponse.DeviceRevision );

            HiiSetString (
                HiiHandle,
                STRING_TOKEN(STR_IPMI_BMC_DEVICE_REVISION_VAL),
                CharPtrString,
                NULL );

            // BMC Firmware Revision.
            UnicodeSPrint(
                CharPtrString,
                STRING_BUFFER_LENGTH,
                L"%d.%02x",
                BmcSelfTestProtocol->GetDeviceIdResponse.FirmwareRev1.Bits.MajorFirmwareRev,
                BmcSelfTestProtocol->GetDeviceIdResponse.MinorFirmwareRev );

            HiiSetString(
                HiiHandle,
                STRING_TOKEN(STR_IPMI_BMC_FIRMWARE_REVISION_VAL),
                CharPtrString,
                NULL);
            // IPMI Specification Version.
            UnicodeSPrint(
                CharPtrString,
                STRING_BUFFER_LENGTH,
                L"%x.%x",
                ((BmcSelfTestProtocol->GetDeviceIdResponse.SpecificationVersion)& 0x0F),
                ((BmcSelfTestProtocol->GetDeviceIdResponse.SpecificationVersion) >> 0x04) );

            HiiSetString (
                HiiHandle,
                STRING_TOKEN(STR_IPMI_VERSION_VAL),
                CharPtrString,
                NULL );
            gBS->FreePool (CharPtrString);
         }
    }

    if (EFI_ERROR (Status)) {
        // Bmc self test protocol is not found, so log error.
        if (LogIndex < AMI_IPMI_BST_LOG_SIZE) {
            gBstLog.Log[LogIndex] = 0; // Index of String Indicating protocol error.
            if (TSE_MULTILINE_CONTROLS) {
                UpdateTime (LogIndex);
            }
            DEBUG ((DEBUG_INFO, "gBstLog.Log[%x]:  %x \n", LogIndex, gBstLog.Log[LogIndex]));
            LogIndex = LogIndex + 1;
        }
    } else {
        // Fill the gBstLog NVRAM variable with BMC Self Test Status codes.
        if (BmcSelfTestProtocol->BmcSelfTestErrorCount != 0 && LogIndex < AMI_IPMI_BST_LOG_SIZE) {
            for(Index=0; Index < BmcSelfTestProtocol->BmcSelfTestErrorCount; Index++, LogIndex++) {
                if (LogIndex == AMI_IPMI_BST_LOG_SIZE) {
                    break;
                }
                gBstLog.Log[LogIndex] = ConvertStatusCode (BmcSelfTestProtocol->BmcSelfTestError[Index]);
                DEBUG ((DEBUG_INFO, "gBstLog.Log[%x]:  %x \n", LogIndex, gBstLog.Log[LogIndex]));

                if (TSE_MULTILINE_CONTROLS){
                    UpdateTime (LogIndex);
                }
            }
        }
    }
    gBstLog.LogSize = LogIndex;

    DEBUG ((DEBUG_INFO, "gBstLog.LogSize:  %x \n", gBstLog.LogSize));
    // Set BstLogVar Nvram variable.
    Status = gRT->SetVariable (
                    L"BstLogVar",
                    &gEfiServerMgmtSetupVariableGuid,
                    VAR_FLAG_ATTRIBUTES,
                    sizeof(BMC_SELF_TEST_LOG_DATA),
                    &gBstLog );
    
    DEBUG ((DEBUG_INFO, "gRT->SetVariable BstLogVar status %r \n", Status));

    // Add/log BmcSelfTest status codes in setup and also update log size in setup.
    CharPtrString = HiiGetString (
                        HiiHandle,
                        STRING_TOKEN(STR_LOG_AREA_USAGE),
                        NULL);
    
    if (CharPtrString != NULL) {
        CharPtrString1 = AllocateZeroPool (STRING_BUFFER_LENGTH);
        if (CharPtrString1 != NULL){
            UnicodeSPrint(
                CharPtrString1,
                STRING_BUFFER_LENGTH,
                CharPtrString,
                gBstLog.LogSize );

            HiiSetString (
                HiiHandle,
                STRING_TOKEN(STR_LOG_CAPACITY),
                CharPtrString1,
                NULL );
            gBS->FreePool (CharPtrString1);
        }
        gBS->FreePool (CharPtrString);
    }

    // Check whether log is empty or not and update the strings.
    StringRef = STRING_TOKEN(STR_IPMI_SELF_TEST_LOG_1);
    if (!gBstLog.LogSize) {
        CharPtrString = HiiGetString (
                            HiiHandle,
                            SetupStrings[1],
                            NULL );
        if (CharPtrString != NULL) {
            HiiSetString  (
                HiiHandle,
                StringRef,
                CharPtrString,
                NULL );
            gBS->FreePool (CharPtrString);
        }
    } else {
        // Displaying the Date, Time and Status code Heading.
        if (TSE_MULTILINE_CONTROLS) {
            CharPtrString = HiiGetString (
                                HiiHandle,
                                STRING_TOKEN(STR_DATE_TIME_STATUSCODE),
                                NULL );

            if (CharPtrString != NULL) {
                HiiSetString (
                    HiiHandle,
                    STRING_TOKEN(STR_DATE_TIME_STATUSCODE_HEADING),
                    CharPtrString,
                    NULL );
                 gBS->FreePool (CharPtrString);
            }
        }
        for (Index = 0; Index < gBstLog.LogSize ; Index++, StringRef++) {
            // Displaying the Date and Time if the token TSE_MULTILINE_CONTROLS is set.

            CharPtrString = HiiGetString (
                                HiiHandle,
                                SetupStrings[gBstLog.Log[Index]],
                                NULL );
            if (TSE_MULTILINE_CONTROLS) {
                if (CharPtrString != NULL) {
                    CharPtrString1 = AllocateZeroPool (STRING_BUFFER_LENGTH);
                    if (CharPtrString1 != NULL){
                        UnicodeSPrint(
                            CharPtrString1,
                            STRING_BUFFER_LENGTH,
                            L"%02d/%02d/%02d  %02d:%02d:%02d  %s\n",
                            gBstLog.Date[Index].Month,
                            gBstLog.Date[Index].Day,
                            gBstLog.Date[Index].Year,
                            gBstLog.Time[Index].Hour,
                            gBstLog.Time[Index].Minute,
                            gBstLog.Time[Index].Second,
                            CharPtrString );

                        HiiSetString (
                            HiiHandle,
                            StringRef,
                            CharPtrString1,
                            NULL );
                        gBS->FreePool (CharPtrString1);
                    }
                    gBS->FreePool (CharPtrString);
                }
            } else {
                if (CharPtrString != NULL) {
                    HiiSetString (
                        HiiHandle,
                        StringRef,
                        CharPtrString,
                        NULL );
                    gBS->FreePool (CharPtrString);
                }
            }
        } // for loop.
    }// if (!gBstLog.LogSize)
}


