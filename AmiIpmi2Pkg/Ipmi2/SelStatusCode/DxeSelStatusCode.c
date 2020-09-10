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

/** @file DxeSelStatusCode.c
    DXE Driver that provides SEL status code reporting routines.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <IndustryStandard/IpmiNetFnSensorEvent.h>
#include <AmiServerMgmtSetupVariable.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/AmiSelLib.h>
#include <Protocol/ReportStatusCodeHandler.h>
#include <Protocol/IpmiTransportProtocol.h>
#include <Protocol/SelProtocol.h>
#include <Library/AmiIpmiLib.h>
//---------------------------------------------------------------------------

EFI_IPMI_TRANSPORT                  *gIpmiTransport;
EFI_SM_SEL_STATUS_CODE_PROTOCOL     gSelProtocol;
UINT8                               gEraseSel;
BOOLEAN                             gDelsupport = FALSE;
BOOLEAN                             gLogProgressCode;
BOOLEAN                             gLogErrorCode;

//---------------------------------------------------------------------------

/**
    Notification function for Virtual address change event.
    Convert all addresses inside gEfiSelStatusCodeProtocol and
    gIpmiTransport pointer.

    @param[in] Event    Event which caused this handler.
    @param[in] Context  Context passed during Event Handler registration.

    @return VOID    Nothing.

**/
VOID
EFIAPI
DxeSelStatusCodeVirtualAddressChangeEvent (
  IN EFI_EVENT  Event,
  IN VOID       *Context )
{
    // Change all addresses inside gEfiSelStatusCodeProtocol stored locally.
    gRT->ConvertPointer (
            0,
            (VOID **) &(gSelProtocol.EfiSelReportStatusCode));

    // Change the address pointed by gIpmiTransport as it is used at runtime.
    gRT->ConvertPointer (
            0,
            (VOID **) &gIpmiTransport );
}

/**
    Convert status code value and extended data to BMC SEL record format and
    send the command to log SEL in BMC.

    @param[in] This         Pointer to the Sel status code protocol.
    @param[in] Value        EFI status code Value.
    @param[in] CodeType     EFI status code type.
    @param[in] Instance     The enumeration of a hardware or software entity
                            within the system. A system may contain multiple
                            entities that match a class/subclass pairing.
                            The instance differentiates between them. An
                            instance of 0 indicates that instance
                            information is unavailable, not meaningful, or
                            not relevant. Valid instance numbers start with 1.
    @param[in] CallerId     This optional parameter may be used to identify
                            the caller. This parameter allows the status code
                            driver to apply different rules to different
                            callers.
    @param[in] Data         This optional parameter may be used to pass
                            additional data.

    @retval EFI_UNSUPPORTED Unable to report status code due to disabled
                            setup options.
    @retval Others          Other status of function calls used.

**/
EFI_STATUS
EFIAPI
EfiSelReportStatusCode (
  IN EFI_SM_SEL_STATUS_CODE_PROTOCOL     *This,
  IN EFI_STATUS_CODE_TYPE                CodeType,
  IN EFI_STATUS_CODE_VALUE               Value,
  IN UINT32                              Instance,
  IN EFI_GUID                            *CallerId, OPTIONAL
  IN EFI_STATUS_CODE_DATA                *Data OPTIONAL )
{
    EFI_STATUS                                  Status;
    IPMI_SEL_EVENT_RECORD_DATA                  SelRecordData;
    IPMI_PLATFORM_EVENT_MESSAGE_DATA_REQUEST    EventMessage;

    DEBUG ((DEBUG_INFO, "%a Entered... Value: %x \n", __FUNCTION__, Value));

    /* Check if logging only progress code, only error code or both is opted
       by user.*/
    switch (CodeType & EFI_STATUS_CODE_TYPE_MASK) {
        case EFI_PROGRESS_CODE:
            if (!gLogProgressCode) {
                return EFI_UNSUPPORTED;
            }
            break;
        case EFI_ERROR_CODE:
            if (!gLogErrorCode) {
                return EFI_UNSUPPORTED;
            }
            break;
        default:
            break;
    }

    /* Check if status code equivalent to SEL entry is found in table.
           If found, log the event else return status.*/
    if (USE_PLATFORM_EVENT_MESSAGE) {
        Status = AmiIpmiFillAndLogEventRecord (
                    gIpmiTransport,
                    Value,
                    CodeType & EFI_STATUS_CODE_TYPE_MASK,
                    &EventMessage );
    } else {
        Status = AmiIpmiFillAndLogEventRecord (
                    gIpmiTransport,
                    Value,
                    CodeType & EFI_STATUS_CODE_TYPE_MASK,
                    &SelRecordData );
    }
    DEBUG ((DEBUG_INFO, "AmiIpmiFillAndLogEventRecord Status: %r \n", Status));
    return Status;
}

/**
    Calls Sel Report status code function to log Sel in Bmc.

    @param[in] CodeType     EFI status code type.
    @param[in] Value        EFI status code Value.
    @param[in] Instance     The enumeration of a hardware or software entity
                            within the system. A system may contain multiple
                            entities that match a class/subclass pairing.
                            The instance differentiates between them. An
                            instance of 0 indicates that instance
                            information is unavailable, not meaningful, or
                            not relevant. Valid instance numbers start with 1.
    @param[in] CallerId     This optional parameter may be used to identify
                            the caller. This parameter allows the status code
                            driver to apply different rules to different
                            callers.
    @param[in] Data         This optional parameter may be used to pass
                            additional data.

    @retval EFI_SUCCESS         Status code reported to BMC successfully.
    @retval EFI_DEVICE_ERROR    Status code entry is not found in table.

**/
EFI_STATUS
EFIAPI
DxeSelStatusCodeReportWorker (
  IN EFI_STATUS_CODE_TYPE     CodeType,
  IN EFI_STATUS_CODE_VALUE    Value,
  IN UINT32                   Instance,
  IN EFI_GUID                 *CallerId,
  IN EFI_STATUS_CODE_DATA     *Data )
{
    EFI_STATUS  Status = EFI_SUCCESS;

    if (((CodeType & EFI_STATUS_CODE_TYPE_MASK) == EFI_PROGRESS_CODE) ||
         ((CodeType & EFI_STATUS_CODE_TYPE_MASK) == EFI_ERROR_CODE)) {

        /* Check if status code to equivalent SEL entry found in table.
           If not found, return.*/
        if (AmiIpmiConvertEfiStatusCodeToSelEntry (
                Value,
                CodeType & EFI_STATUS_CODE_TYPE_MASK ) == NULL ) {
            return EFI_DEVICE_ERROR;
        }
        Status = gSelProtocol.EfiSelReportStatusCode (
                                &gSelProtocol,
                                CodeType,
                                Value,
                                0,
                                NULL,
                                NULL );
        if (EFI_ERROR (Status)) {
            Status = EFI_DEVICE_ERROR;
        }
    }

    return Status;
}

/**
    This is the standard EFI driver Entry point function. This function
    initializes the private data required for creating SEL Driver.

    @param[in] ImageHandle  Handle of this driver image.
    @param[in] SystemTable  Table containing standard EFI services.

    @retval EFI_SUCCESS     Protocol successfully initialized and installed.
    @retval EFI_UNSUPPORTED Sel logging is not supported or disabled in
                            setup option.
    @retval Others          Error status of functions calls used.

**/
EFI_STATUS
EFIAPI
InitializeDxeSel (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable )
{
    EFI_HANDLE                      NewHandle = NULL;
    EFI_STATUS                      Status = EFI_SUCCESS;
    SERVER_MGMT_CONFIGURATION_DATA  ServerMgmtConfiguration;
    UINTN                           Size;
    UINT32                          Attributes;
    EFI_RSC_HANDLER_PROTOCOL        *RscHandlerProtocol = NULL;
    EFI_EVENT                       EfiVirtualAddressChangeNotifyEvent = NULL;

    DEBUG ((DEBUG_INFO, "%a Entered...\n", __FUNCTION__));

    /* Attempt to locate the IPMI protocol. If it is not found, return
       status and SEL Reporting cannot be supported, so do not install protocol.*/
    Status = gBS->LocateProtocol (
                    &gEfiDxeIpmiTransportProtocolGuid,
                    NULL,
                    (VOID **)&gIpmiTransport );
    
    DEBUG ((DEBUG_INFO, " gEfiDxeIpmiTransportProtocolGuid Status: %r \n", Status));
    
    if (EFI_ERROR (Status)) {
        return Status;
    }

    Size = sizeof (SERVER_MGMT_CONFIGURATION_DATA);
    Status = gRT->GetVariable (
                    L"ServerSetup",
                    &gEfiServerMgmtSetupVariableGuid,
                    &Attributes,
                    &Size,
                    &ServerMgmtConfiguration );
    
    DEBUG ((DEBUG_INFO, " gEfiServerMgmtSetupVariableGuid Status: %r \n", Status));
    
    if (EFI_ERROR(Status)) {
        // Load default values.
        ServerMgmtConfiguration.SelLogging          = AMI_IPMI_SEL_LOGGING_ENABLED;
        ServerMgmtConfiguration.SelFull             = AMI_IPMI_DO_NOTHING_WHEN_SEL_FULL;
        ServerMgmtConfiguration.SelErase            = AMI_IPMI_DO_NOT_ERASE;
        ServerMgmtConfiguration.LogSelStatusCodes   = AMI_IPMI_LOG_ERROR_CODE_ONLY;
    }

    DEBUG ((DEBUG_INFO, " ServerMgmtConfiguration.SelLogging: %X \n", ServerMgmtConfiguration.SelLogging));

    // If Sel components setup option is disabled, then return.
    if (ServerMgmtConfiguration.SelLogging == AMI_IPMI_SEL_LOGGING_DISABLED) {
        return EFI_UNSUPPORTED;
    }

    /* Check if Erase sel on every boot or next boot and update NVRAM
       Variable accordingly.*/
    if ((ServerMgmtConfiguration.SelErase == AMI_IPMI_ERASE_NEXT_BOOT) || \
        (ServerMgmtConfiguration.SelErase == AMI_IPMI_ERASE_EVERY_BOOT)) {
        AmiIpmiEraseSelElogData (
            gIpmiTransport,
            AMI_IPMI_ERASE_WHEN_SEL_FULL );
        if (ServerMgmtConfiguration.SelErase == AMI_IPMI_ERASE_NEXT_BOOT) {
            ServerMgmtConfiguration.SelErase = AMI_IPMI_DO_NOT_ERASE;

            DEBUG ((DEBUG_INFO, " ServerMgmtConfiguration.SelErase: %X \n", ServerMgmtConfiguration.SelErase));
            Status = gRT->SetVariable (
                            L"ServerSetup",
                            &gEfiServerMgmtSetupVariableGuid,
                            Attributes,   // Attributes
                            Size,
                            &ServerMgmtConfiguration );
            ASSERT_EFI_ERROR (Status);
        }
    }

    // When SEL is full, if user opts to erase when SEL is full, erase it.
    gEraseSel = ServerMgmtConfiguration.SelFull;
    if ((AmiIpmiIsSelFull (
             gIpmiTransport,
             &gDelsupport ) == TRUE) && \
        (gEraseSel == AMI_IPMI_ERASE_WHEN_SEL_FULL)) {
        AmiIpmiEraseSelElogData (
            gIpmiTransport,
            AMI_IPMI_ERASE_WHEN_SEL_FULL );
    }
    DEBUG ((DEBUG_INFO, "SelDelSupport: %X \n", gDelsupport));

    /* Check if Logging of status codes is enabled in setup.
       If not enabled, don't install the protocol.*/
    if (ServerMgmtConfiguration.LogSelStatusCodes == AMI_IPMI_LOG_SEL_STATUSCODE_DISABLED) {
        return EFI_UNSUPPORTED;
    }


    /* Check if logging only progress code, only error code
       or both is opted by user.*/
    gLogProgressCode = (ServerMgmtConfiguration.LogSelStatusCodes == AMI_IPMI_LOG_BOTH_PROGRESS_AND_ERROR_CODES) || \
                       (ServerMgmtConfiguration.LogSelStatusCodes == AMI_IPMI_LOG_PROGRESS_CODE_ONLY);
    gLogErrorCode = (ServerMgmtConfiguration.LogSelStatusCodes == AMI_IPMI_LOG_BOTH_PROGRESS_AND_ERROR_CODES) || \
                    (ServerMgmtConfiguration.LogSelStatusCodes == AMI_IPMI_LOG_ERROR_CODE_ONLY);

    gSelProtocol.EfiSelReportStatusCode = EfiSelReportStatusCode;

    Status = gBS->InstallProtocolInterface (
                    &NewHandle,
                    &gEfiSelStatusCodeProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &gSelProtocol );
    
    DEBUG ((DEBUG_INFO, " gEfiSelStatusCodeProtocolGuid Status: %r \n", Status));
    
    if (EFI_ERROR (Status)) {
        return Status;
    }

    // Create event for the Virtual Address Change Event.
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_NOTIFY,
                    DxeSelStatusCodeVirtualAddressChangeEvent,
                    NULL,
                    (CONST EFI_GUID *)&gEfiEventVirtualAddressChangeGuid,
                    &EfiVirtualAddressChangeNotifyEvent );
    
    DEBUG ((DEBUG_INFO, "Create event for the Virtual Address Change Event status %r\n", Status));
    
    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Create event is failing, So returning without registering DxeSelStatusCodeReportWorker handler\n"));
        return Status;
    }

    Status = gBS->LocateProtocol (
                    &gEfiRscHandlerProtocolGuid,
                    NULL,
                    (VOID **) &RscHandlerProtocol );
    
    DEBUG ((DEBUG_INFO, " gEfiRscHandlerProtocolGuid Status: %r \n", Status));
    ASSERT_EFI_ERROR (Status);
    
    if (!EFI_ERROR (Status)) {
        Status = RscHandlerProtocol->Register (
                                        DxeSelStatusCodeReportWorker,
                                        TPL_HIGH_LEVEL );
        DEBUG ((DEBUG_INFO, " RscHandlerProtocol->Register Status: %r \n", Status));
    }

    DEBUG ((DEBUG_INFO, "%a Exiting...\n", __FUNCTION__));

    return EFI_SUCCESS;
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
