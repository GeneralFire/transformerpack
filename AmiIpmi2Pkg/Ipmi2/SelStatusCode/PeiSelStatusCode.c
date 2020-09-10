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

/** @file PeiSelStatusCode.c
    PEIM to provide SEL status code reporting routines.
    This requires the platform portion of status code module to define 
    some values. The conversion table from status codes to SEL is defined in
    AmiStatusCodeConversion header file.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <IndustryStandard/IpmiNetFnSensorEvent.h>
#include <AmiServerMgmtSetupVariable.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/AmiSelLib.h>
#include <Ppi/MemoryDiscovered.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/ReportStatusCodeHandler.h>
#include <Library/BaseMemoryLib.h>
#include <Ppi/SelPpi.h>

//---------------------------------------------------------------------------

/** @internal
    Pei Sel Ppi local instance.
*/
typedef struct {
    /// Log Sel status codes setup option.
    UINT8                             LogSelStatusCodes;
    /// Report Status code.
    EFI_PEI_SEL_REPORT_STATUS_CODE    EfiSelReportStatusCode;
    /// Pointer to Ipmi Transport Ppi.
    PEI_IPMI_TRANSPORT_PPI            *IpmiTransportPpi;
} PEI_SEL_STATUS_CODE_INSTANCE;

//---------------------------------------------------------------------------

/**
    After memory is discovered, Re-install the Pei Sel Ppi with updated
    IpmiTransportPpi.

    @param[in] PeiServices          Describes the list of possible PEI
                                    Services.
    @param[in] NotifyDescriptor     Pointer to notify descriptor.
    @param[in] Ppi                  Pointer to Ppi.

    @return EFI_STATUS  Status returned from the callback.

**/
EFI_STATUS
EFIAPI
UpdatedPeiSelPpi (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi )
{
    EFI_STATUS                      Status;
    PEI_IPMI_TRANSPORT_PPI          *IpmiTransportPpi;
    EFI_PEI_PPI_DESCRIPTOR          *PeiSelPpiDesc;
    PEI_SEL_STATUS_CODE_INSTANCE    *PeiSelPpi;

    DEBUG ((DEBUG_INFO, "%a Entry\n", __FUNCTION__));

    // Locate Pei Sel Ppi.
    Status = (*PeiServices)->LocatePpi (
                                (CONST EFI_PEI_SERVICES **)PeiServices,
                                &gEfiPeiSelPpiGuid,
                                0,
                                &PeiSelPpiDesc,
                                (VOID **)&PeiSelPpi );
    
    DEBUG ((DEBUG_INFO, "LocatePpi for gEfiPeiSelPpiGuid Status: %r \n", Status));
    
    if (EFI_ERROR (Status)) {
        return Status;
    }

    // Locate Ipmi Transport Ppi to update new pointer in PeiSelPpi.
    Status = (*PeiServices)->LocatePpi (
                                (CONST EFI_PEI_SERVICES **)PeiServices,
                                &gEfiPeiIpmiTransportPpiGuid,
                                0,
                                NULL,
                                (VOID **)&IpmiTransportPpi );
    
    DEBUG ((DEBUG_INFO, "LocatePpi for gEfiPeiIpmiTransportPpiGuid Status: %r \n", Status));
    
    if (EFI_ERROR (Status)) {
        return Status;
    }

    PeiSelPpiDesc->Ppi              = PeiSelPpi;
    PeiSelPpi->IpmiTransportPpi     = IpmiTransportPpi;

    // Re-install the PEI SEL PPI with updated IpmiTransportPpi
    Status = (*PeiServices)->ReInstallPpi (
                                (CONST EFI_PEI_SERVICES **)PeiServices,
                                PeiSelPpiDesc,
                                PeiSelPpiDesc );
    
    DEBUG ((DEBUG_INFO, "ReInstallPpi for PeiSelPpi Status: %r \n", Status));
    DEBUG ((DEBUG_INFO, "%a Exit\n", __FUNCTION__));

    return Status;
}

/**
    Locates Sel Ppi and calls Sel report status code function to log event
    in Sel.

    @param[in] PeiServices  Describes the list of possible PEI Services.
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
PeiSelStatusCodeReportWorker (
  IN CONST EFI_PEI_SERVICES        **PeiServices,
  IN       EFI_STATUS_CODE_TYPE    CodeType,
  IN       EFI_STATUS_CODE_VALUE   Value,
  IN       UINT32                  Instance,
  IN CONST EFI_GUID                *CallerId,
  IN CONST EFI_STATUS_CODE_DATA    *Data )
{
    PEI_SEL_STATUS_CODE_PPI     *PeiSelPpi;
    EFI_STATUS                  Status = EFI_SUCCESS;

    if (((CodeType & EFI_STATUS_CODE_TYPE_MASK) == EFI_PROGRESS_CODE) ||
            ((CodeType & EFI_STATUS_CODE_TYPE_MASK)== EFI_ERROR_CODE)) {

        /* Check if status code to equivalent SEL entry is found in table.
           If not found, then return.*/
        if (AmiIpmiConvertEfiStatusCodeToSelEntry (
                Value,
                CodeType & EFI_STATUS_CODE_TYPE_MASK ) == NULL) {
            return EFI_DEVICE_ERROR;
        }
        DEBUG ((DEBUG_INFO, "%a Entry Value: %x \n", __FUNCTION__, Value));

        // Locate PeiSel Ppi.
        Status = (*PeiServices)->LocatePpi (
                                    PeiServices,
                                    &gEfiPeiSelPpiGuid,
                                    0,
                                    NULL,
                                    (VOID **)&PeiSelPpi );
        
        DEBUG ((DEBUG_INFO, " LocatePpi for gEfiPeiSelPpiGuid Status: %r \n", Status));
        
        if (!EFI_ERROR (Status)) {
            Status = PeiSelPpi->EfiSelReportStatusCode (
                                    PeiServices,
                                    PeiSelPpi,
                                    CodeType,
                                    Value,
                                    Instance,
                                    CallerId,
                                    Data );
            
            DEBUG ((DEBUG_INFO, "EfiSelReportStatusCode Status: %r \n", Status));
        }
        if (EFI_ERROR (Status)) {
            Status = EFI_DEVICE_ERROR;
        }
    }
    return Status;
}

/**
    Convert status code value and extended data to BMC SEL record format and
    send the command to log SEL in BMC.

    @param[in] PeiServices  Pointer to the PEI Core data Structure.
    @param[in] PeiSelPpi    Pointer to the PEI SEL PPI.
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

    @retval EFI_UNSUPPORTED PeiServices is null or unable to report status
                            code due to disabled setup options.
    @retval Others          Other status of function calls used.

**/
EFI_STATUS
EFIAPI
EfiSelReportStatusCode (
  IN CONST  EFI_PEI_SERVICES         **PeiServices,
  IN        PEI_SEL_STATUS_CODE_PPI  *PeiSelPpi,
  IN        EFI_STATUS_CODE_TYPE     CodeType,
  IN        EFI_STATUS_CODE_VALUE    Value,
  IN        UINT32                   Instance,
  IN CONST  EFI_GUID                 *CallerId, OPTIONAL
  IN CONST  EFI_STATUS_CODE_DATA     *Data  OPTIONAL )
{
    EFI_STATUS                                  Status;
    IPMI_SEL_EVENT_RECORD_DATA                  SelRecordData;
    BOOLEAN                                     LogProgressCode;
    BOOLEAN                                     LogErrorCode;
    IPMI_PLATFORM_EVENT_MESSAGE_DATA_REQUEST    EventMessage;

    DEBUG ((DEBUG_INFO, "%a Entry... Value: %x \n", __FUNCTION__, Value));

    // Check if we are called during PEI.
    if (PeiServices == NULL) {
        return EFI_UNSUPPORTED;
    }
    LogProgressCode = (PeiSelPpi->LogSelStatusCodes == AMI_IPMI_LOG_BOTH_PROGRESS_AND_ERROR_CODES) || \
                       (PeiSelPpi->LogSelStatusCodes == AMI_IPMI_LOG_PROGRESS_CODE_ONLY);
    LogErrorCode = (PeiSelPpi->LogSelStatusCodes == AMI_IPMI_LOG_BOTH_PROGRESS_AND_ERROR_CODES) || \
                   (PeiSelPpi->LogSelStatusCodes == AMI_IPMI_LOG_ERROR_CODE_ONLY);

    // Check if logging only progress code, only error code or both is opted by user.
    switch (CodeType & EFI_STATUS_CODE_TYPE_MASK) {
        case EFI_PROGRESS_CODE:
            if (!LogProgressCode) {
                return EFI_UNSUPPORTED;
            }
            break;
        case EFI_ERROR_CODE:
            if (!LogErrorCode) {
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
                    ((PEI_SEL_STATUS_CODE_INSTANCE*)PeiSelPpi)->IpmiTransportPpi,
                    Value,
                    CodeType & EFI_STATUS_CODE_TYPE_MASK,
                    &EventMessage );
    } else {
        Status = AmiIpmiFillAndLogEventRecord (
                    ((PEI_SEL_STATUS_CODE_INSTANCE*)PeiSelPpi)->IpmiTransportPpi,
                    Value,
                    CodeType & EFI_STATUS_CODE_TYPE_MASK,
                    &SelRecordData );
    }
    DEBUG ((DEBUG_INFO, "AmiIpmiFillAndLogEventRecord Status: %r \n", Status));
    return Status;
}

/**
    This function is the entry point to initialize and install the Sel
    status code Ppi.

    @param[in] FileHandle    Pointer to image file handle.
    @param[in] PeiServices   Describes the list of possible PEI Services.

    @retval EFI_SUCCESS             Successful driver initialization.
    @retval EFI_UNSUPPORTED         If Sel logging setup option is disabled.
    @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.
    @retval Others                  Error status while initializing the
                                    driver.

**/
EFI_STATUS
InitializePeiSel (
  IN        EFI_PEI_FILE_HANDLE     FileHandle,
  IN CONST  EFI_PEI_SERVICES        **PeiServices )
{
    EFI_STATUS                          Status;
    EFI_PEI_READ_ONLY_VARIABLE2_PPI     *PeiVariable;
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration;
    UINTN                               Size;
    PEI_IPMI_TRANSPORT_PPI              *IpmiTransportPpi;
    EFI_PEI_RSC_HANDLER_PPI             *RscHandlerPpi;
    EFI_PEI_PPI_DESCRIPTOR              *PeiSelPpiDesc;
    PEI_SEL_STATUS_CODE_INSTANCE        *PeiSelPpi;
    static EFI_PEI_NOTIFY_DESCRIPTOR    NotifyList[] = {
                                                        (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | \
                                                        EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
                                                        &gEfiPeiMemoryDiscoveredPpiGuid,
                                                        UpdatedPeiSelPpi
                                                       };

    // Locate Ipmi Transport Ppi to send commands to BMC.
    Status = (*PeiServices)->LocatePpi (
                                PeiServices,
                                &gEfiPeiIpmiTransportPpiGuid,
                                0,
                                NULL,
                                (VOID **)&IpmiTransportPpi );
    
    DEBUG ((DEBUG_INFO, " gEfiPeiIpmiTransportPpiGuid Status: %r \n", Status));
    
    if (EFI_ERROR (Status)) {
        return Status;
    }

    // Locate the Setup configuration value.
    Status = (*PeiServices)->LocatePpi (
                                PeiServices,
                                &gEfiPeiReadOnlyVariable2PpiGuid,
                                0,
                                NULL,
                                (VOID **)&PeiVariable );
    
    DEBUG ((DEBUG_INFO, " gEfiPeiReadOnlyVariable2PpiGuid status %r\n", Status));
    ASSERT_EFI_ERROR (Status);

    if (!EFI_ERROR (Status)) {
        Size = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
        Status = PeiVariable->GetVariable (
                                PeiVariable,
                                L"ServerSetup",
                                &gEfiServerMgmtSetupVariableGuid,
                                NULL,
                                &Size,
                                &ServerMgmtConfiguration );
        DEBUG ((DEBUG_INFO, " PeiVariable->GetVariable status %r \n", Status));
        DEBUG ((DEBUG_INFO, " SelLogging: %x LogSelStatusCodes: %x \n", ServerMgmtConfiguration.SelLogging, ServerMgmtConfiguration.LogSelStatusCodes));
    }
    if (EFI_ERROR(Status)) {
        // Load default values.
        ServerMgmtConfiguration.SelLogging           = AMI_IPMI_SEL_LOGGING_ENABLED;
        ServerMgmtConfiguration.SelFull              = AMI_IPMI_DO_NOTHING_WHEN_SEL_FULL;
        ServerMgmtConfiguration.SelErase             = AMI_IPMI_DO_NOT_ERASE;
        ServerMgmtConfiguration.LogSelStatusCodes    = AMI_IPMI_LOG_ERROR_CODE_ONLY;
    }

    // Set the value of NVRAM in to PCD to access while checking action in SEL is full case.
    Status = PcdSet8S (
                 AmiPcdSelFullBehaviour,
                 ServerMgmtConfiguration.SelFull );

    ASSERT_EFI_ERROR(Status);

    DEBUG ((DEBUG_INFO, "SelFullBehaviour PCD value is: %d \n", PcdGet8(AmiPcdSelFullBehaviour)));

    // Check if SEL components enabled in setup, if not enabled then return.
    if ((!ServerMgmtConfiguration.SelLogging) || \
        (!ServerMgmtConfiguration.LogSelStatusCodes)) {
        return EFI_UNSUPPORTED;
    }

    /* Allocate descriptor and Ppi structures and Install Pei Sel Ppi.*/
    PeiSelPpi = (PEI_SEL_STATUS_CODE_INSTANCE *)AllocateZeroPool (sizeof (PEI_SEL_STATUS_CODE_INSTANCE));
    if (PeiSelPpi == NULL) {
        DEBUG ((DEBUG_ERROR, "Failed to allocate memory for PeiSelPpi! \n"));
        return EFI_OUT_OF_RESOURCES;
    }
    
    PeiSelPpi->LogSelStatusCodes        = ServerMgmtConfiguration.LogSelStatusCodes;
    PeiSelPpi->EfiSelReportStatusCode   = EfiSelReportStatusCode;
    // Update IpmiTransportPpi Pointer for later use.
    PeiSelPpi->IpmiTransportPpi         = IpmiTransportPpi;

    PeiSelPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *)AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
    if (PeiSelPpiDesc == NULL) {
        DEBUG ((DEBUG_ERROR, "Failed to allocate memory for PeiSelPpiDesc! \n"));
        // Frees the allocated memory for PeiSelPpi and return.
        FreePool (PeiSelPpi);
        return EFI_OUT_OF_RESOURCES;
    }

    PeiSelPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
    PeiSelPpiDesc->Guid  = &gEfiPeiSelPpiGuid;
    PeiSelPpiDesc->Ppi   = PeiSelPpi;

    Status = (*PeiServices)->InstallPpi (
                                PeiServices,
                                PeiSelPpiDesc );
    
    DEBUG ((DEBUG_INFO, " (*PeiServices)->InstallPpi Status: %r \n", Status));
    
    if (EFI_ERROR (Status)) {
        // Error in installing the Ppi. So free the allocated memory.
        FreePool (PeiSelPpi);
        FreePool (PeiSelPpiDesc);
        return Status;
    }

    /* Install call-back to update IpmiTransportPpi pointer in PeiSelPpi
       after MRC in PEI phase.*/
    Status = (*PeiServices)->NotifyPpi (
                                PeiServices,
                                &NotifyList[0] );
    
    DEBUG ((DEBUG_INFO, " (*PeiServices)->NotifyPpi Status: %r \n", Status));
    ASSERT_EFI_ERROR (Status);

    Status = (*PeiServices)->LocatePpi (
                                PeiServices,
                                &gEfiPeiRscHandlerPpiGuid,
                                0,
                                NULL,
                                (VOID **) &RscHandlerPpi );
    
    DEBUG ((DEBUG_INFO, " gEfiPeiRscHandlerPpiGuid Status: %r \n", Status));
    
    if (!EFI_ERROR (Status)) {
        Status = RscHandlerPpi->Register (PeiSelStatusCodeReportWorker);
        DEBUG ((DEBUG_INFO, " RscHandlerPpi->Register Status: %r \n", Status));
    }

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
