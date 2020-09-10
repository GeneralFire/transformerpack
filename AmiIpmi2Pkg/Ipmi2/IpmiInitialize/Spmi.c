//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file Spmi.c
    Installs SPMI ACPI Table.

*/

//---------------------------------------------------------------------------

#include "Spmi.h"

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

UINT16               gIpmiSpecRevision;

//---------------------------------------------------------------------------
/** @internal
    Initiates the SPMI update hooks of OEM.

    @param[in] BootServices     Pointer the set of Boot services.
    @param[in] SpmiData         Pointer to the Spmi table data.

    @return EFI_STATUS  Return status of the hooks.

**/
EFI_STATUS
IpmiOemSpmiUpdateHook (
  IN CONST EFI_BOOT_SERVICES    *BootServices,
  IN       VOID                 *SpmiData )
{
    EFI_STATUS  Status = EFI_SUCCESS;
    UINTN       Index;

    for (Index = 0; gIpmiOemSpmiUpdateHookList[Index]; Index++) {
        Status = gIpmiOemSpmiUpdateHookList[Index] (
                    BootServices,
                    SpmiData );
    }
    return Status;
}

/** @internal
    This function will update the IPMI ASL object for IPMI specification
    Version.

    @param[in] Context  Context passed during Event Handler registration.

    @return VOID    Nothing.

**/
VOID
UpdateIpmiAslObject (
  IN EFI_EVENT    Event,
  IN VOID         *Context )
{
    EFI_STATUS                Status;
    EFI_ACPI_SDT_HEADER       *DsdtAddr = 0;

    DEBUG ((DEBUG_INFO, "IPMI:Getting DSDT\n"));
    Status = AmiGetDsdt (
                &DsdtAddr,
                EFI_ACPI_TABLE_VERSION_ALL );

    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR,"IPMI:Failed to get DSDT\n"));
        return;
    }

    DEBUG ((DEBUG_INFO, "IPMI:Updating SRVV ASL Object\n"));
    DEBUG ((DEBUG_INFO, "gIpmiSpecRevision = %x\n",gIpmiSpecRevision));
    Status = AmiUpdateAmlObjectValue (
                            DsdtAddr,
                            (CHAR8 *)"SRVV",
                            (UINT64)gIpmiSpecRevision );

    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "IPMI:Failed to update SRVV ASL Object\n"));
    }
}

/**
    Notification function for updating the SPMI data and creates the SPMI ACPI Table.

    @param[in] Event    Event which caused this handler.
    @param[in] Context  Context passed during Event Handler registration.

    @return VOID    Nothing.

**/
VOID
EFIAPI
PublishSpmiAcpiTables (
  IN EFI_EVENT  Event,
  IN VOID       *Context )
{
    EFI_STATUS                                              Status;
    EFI_ACPI_SERVICE_PROCESSOR_MANAGEMENT_INTERFACE_TABLE   *SpmiTable = NULL;
    EFI_BMC_SELF_TEST_LOG_PROTOCOL                          *BmcSelfTestProtocol = NULL;
    UINT8                                                   SpmiOemId[7] = ACPI_OEM_ID_MAK; 
    UINT8                                                   SpmiOemTableId[8] = ACPI_OEM_TBL_ID_MAK;
    EFI_EVENT                                               EndOfDxeEvent;

    Status = gBS->LocateProtocol (
                    &gEfiAcpiTableProtocolGuid,
                    NULL,
                    (VOID **)&gAcpiTable );

    DEBUG((DEBUG_INFO, "Locate ACPI Table protocol Status: %r \n", Status));

    if (EFI_ERROR (Status)) {
        return;
    }

    Status = gBS->AllocatePool (
                    EfiRuntimeServicesData,
                    sizeof(EFI_ACPI_SERVICE_PROCESSOR_MANAGEMENT_INTERFACE_TABLE),
                    (VOID **)&SpmiTable );

    ASSERT(SpmiTable);

    if (!SpmiTable) {
        return;
    }

    // Fill in Spmi table.
    CopyMem (
        SpmiTable,
        &gSpmi,
        sizeof(EFI_ACPI_SERVICE_PROCESSOR_MANAGEMENT_INTERFACE_TABLE) );

    CopyMem (
        SpmiTable->AcpiHeader.OemId,
        SpmiOemId,
        sizeof(SpmiOemId) );

    CopyMem (
        (UINT8 *)&SpmiTable->AcpiHeader.OemTableId,
        SpmiOemTableId,
        sizeof(SpmiOemTableId) );

    SpmiTable->AcpiHeader.CreatorId = SPMI_CREATOR_ID;

    switch (DEFAULT_SYSTEM_INTERFACE) {
        case AmiSysInterfaceKcs:
            SpmiTable->SpmiData.InterfaceType       = AmiSysInterfaceKcs;
            SpmiTable->SpmiData.BaseAddress.Address = IPMI_KCS_BASE_ADDRESS;
            break;

        case AmiSysInterfaceBt:
            SpmiTable->SpmiData.InterfaceType       = AmiSysInterfaceBt;
            SpmiTable->SpmiData.BaseAddress.Address = IPMI_BT_BASE_ADDRESS;
            break;

        case AmiSysInterfaceSsif:
            SpmiTable->SpmiData.InterfaceType                   = AmiSysInterfaceSsif;
            SpmiTable->SpmiData.BaseAddress.Address             = IPMI_SSIF_SLAVE_ADDRESS;
            SpmiTable->SpmiData.BaseAddress.AddressSpaceId      = EFI_ACPI_2_0_SMBUS;
            SpmiTable->SpmiData.BaseAddress.RegisterBitWidth    = 0x00;
            break;

        default:
            // Other interface types not listed in SPMI table interface type.
            return;
    }

    // Locate the BMC self test protocol
    Status = gBS->LocateProtocol (
                    &gEfiBmcSelfTestLogProtocolGuid,
                    NULL,
                    (VOID **)&BmcSelfTestProtocol );

    DEBUG ((DEBUG_INFO, "gEfiBmcSelfTestLogProtocolGuid: Status:  %r\n", Status));

    // Update IPMI specification version
    if (BmcSelfTestProtocol != NULL) {
        gIpmiSpecRevision = (UINT16)(BmcSelfTestProtocol->GetDeviceIdResponse.SpecificationVersion & AMI_IPMI_MASK_UPPER_NIBBLE);
        gIpmiSpecRevision |= (UINT16)((BmcSelfTestProtocol->GetDeviceIdResponse.SpecificationVersion & AMI_IPMI_MASK_LOWER_NIBBLE) << 8 );
        SpmiTable->SpmiData.SpecificationRevision = gIpmiSpecRevision;


        Status = gBS->CreateEventEx (
                        EVT_NOTIFY_SIGNAL,
                        TPL_CALLBACK,
                        UpdateIpmiAslObject,
                        NULL,
                        &gEfiEndOfDxeEventGroupGuid,
                        &EndOfDxeEvent
                        );

        if (EFI_ERROR (Status)) {
            DEBUG((DEBUG_ERROR,"SPMI: Create an Event for Update IPMI Asl Object - returned %r\n", Status));
            ASSERT_EFI_ERROR(Status);
        }
    }

    Status = IpmiOemSpmiUpdateHook (
                gBS,
                (VOID *)&SpmiTable->SpmiData );

    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "IpmiOemSpmiUpdateHook is failed and Status is %r and initializing unspecified values.\n", Status));
        SpmiTable->SpmiData.BaseAddress.Address = 0x00;
        SpmiTable->SpmiData.BaseAddress.AddressSpaceId = EFI_ACPI_2_0_SYSTEM_MEMORY; // System Memory.
    }

    // Add Spmi table.
    Status = gAcpiTable->InstallAcpiTable (
                            gAcpiTable,
                            SpmiTable,
                            sizeof (EFI_ACPI_SERVICE_PROCESSOR_MANAGEMENT_INTERFACE_TABLE),
                            &gSpmiTblHandle );

    DEBUG((DEBUG_INFO, "AcpiTable->InstallAcpiTable(SPMI) = %r\n", Status));
    ASSERT_EFI_ERROR(Status);

    // Free memory used for table and close the event.
    gBS->FreePool (SpmiTable);
    gBS->CloseEvent (Event);
}

/** @internal
    Registers notification call back AcpiTable guid to publish SPMI table
    and update IPMI Specification Version ASL Object update if
    AcpiTable guid is already installed.

    @param VOID     Nothing.

    @return EFI_STATUS     Return status of function used.

**/
EFI_STATUS 
SpmiSupport (
  VOID )
{
    EFI_STATUS  Status;
    EFI_EVENT   AcpiEvent;

    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    PublishSpmiAcpiTables,
                    NULL,
                    &gEfiAcpiTableGuid,
                    &AcpiEvent );

    DEBUG ((DEBUG_INFO, "IPMI: Adding PublishSpmiAcpiTables to gEfiAcpiTableGuid event group, Status: %r\n", Status));

    ASSERT_EFI_ERROR(Status); 

    // If AcpiTable protocol has been installed, then we can use it right away.
    PublishSpmiAcpiTables (
        AcpiEvent,
        NULL );

    return Status;
}


