//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file
    Contains the NULL functions to Install Acpi table, Uninstall Acpi table,
    Reinstall Acpi table and to Get Acpi table without locating Acpi sdt
    protocol and Acpi table protocol in driver.
*/
#include <PiDxe.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <IndustryStandard/Acpi20.h>
#include <AmiPlatformAcpiLib.h>

/**
    Locates the Acpi sdt protocol and Acpi Table protocol and returns
    EFI_SUCCESS. Otherwise causes assert based on the Status returned by
    LocateProtocol.

    @param VOID

    @return EFI_STATUS
    @retval Status            When Acpi sdt protocol and Acpi Table protocol
                              are successfully located. Otherwise causes
                              assert based on Status.

**/
EFI_STATUS 
InitAcpiLib (
    VOID
)
{
    return EFI_SUCCESS;
}

/**
     Returns specific ACPI table for the Signature and Version passed as
     inputs. Otherwise returns EFI_NOT_FOUND.
     
     Notes:   This function will check each ACPI table with Version and
              Signature and returns valid ACPI table. For DSDT, it will check
              for version. If the Version is EFI_ACPI_TABLE_VERSION_X or
              EFI_ACPI_TABLE_VERSION_NONE, then it will return
              XSDT->FADT->DSDT (64 bit). If the Version is
              EFI_ACPI_TABLE_VERSION_1_0B, then it will return
              RSDT->FADT->DSDT (32 bit).

    @param   Version            - ACPI table version.
    @param   Signature          - Signature of the ACPI table.
    @param   **Table            - Contains a header of ACPI table found.
    @param   *TableKey          - Contains handle of the ACPI table found.

    @return EFI_STATUS
    @retval Status            EFI_SUCCESS             When requested table with Version and Signature is found.
                              EFI_NOT_READY           When ACPI SDT Protocol is not located.
                              EFI_INVALID_PARAMETER   When Signature or Version is 0.
                              EFI_NOT_FOUND           When requested table with Version and Signature is not found.
**/
EFI_STATUS 
AcpiLibGetAcpiTable (
    IN   EFI_ACPI_TABLE_VERSION     Version,
    IN   UINT32                     Signature,
    OUT  EFI_ACPI_SDT_HEADER        **Table,
    OUT  UINTN                      *TableKey
)
{
    return EFI_SUCCESS;
}

/**
    Uses InstallAcpiTable of ACPI table protocol to install the ACPI table
    using AcpiTableBuffer and AcpiTableBufferSize inputs and returns
    TableKey as unique identifier and returns status returned by
    InstallAcpiTable.

    @param    *AcpiTableBuffer      - Pointer of ACPI table.
    @param    AcpiTableBufferSize   - Size if the AcpiTableBuffer.
    @param    *TableKey             - Contains a unique identifier of ACPI table installed.

    @return EFI_STATUS
    @retval Status            EFI_SUCCESS         When requested ACPI table is installed.
                              EFI_NOT_READY       When ACPI Table Protocol is not located.
**/
EFI_STATUS 
AcpiLibInstallAcpiTable (
    IN   VOID                 *AcpiTableBuffer,
    IN   UINTN                AcpiTableBufferSize,
    OUT  UINTN                *TableKey
)
{
    return EFI_SUCCESS;

}

/**
    Uses UninstallAcpiTable of ACPI table protocol to Uninstall the ACPI
    table using TableKey and returns status returned by UninstallAcpiTable.

    @param    TableKey             - Contains a unique identifier of ACPI table to be Uninstalled.

    @return EFI_STATUS
    @retval Status            EFI_SUCCESS         When requested ACPI table is uninstalled.
                              EFI_NOT_READY       When ACPI Table Protocol is not located.
**/
EFI_STATUS 
AcpiLibUninstallAcpiTable (
    IN  UINTN                     TableKey
)
{
 
    return EFI_SUCCESS;

}

/**
    Uninstalls the existing ACPI table and installs the new ACPI table using
    AcpiLibInstallAcpiTable and AcpiLibUninstallAcpiTable. Uses
    AcpiLibUninstallAcpiTable is to uninstall the ACPI table using TableKey.
    Based on status returned, AcpiLibInstallAcpiTable will be called to
    install the ACPI table using AcpiTableBuffer, length of the table and
    returns TableKey and status.

    @param      *AcpiTableBuffer      - Data of ACPI table.
    @param      *TableKey             - Contains a unique identifier of ACPI table installed.

    @return EFI_STATUS
    @retval Status            EFI_SUCCESS         When requested ACPI table is uninstalled and installed.
                              EFI_NOT_READY       When inputs are NULL or 0.
**/
EFI_STATUS 
AcpiLibReinstallAcpiTable (
    IN      VOID                 *AcpiTableBuffer,
    IN OUT  UINTN                *TableKey
)
{

    return EFI_SUCCESS;

}
