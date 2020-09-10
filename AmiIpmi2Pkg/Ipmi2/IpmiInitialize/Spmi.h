//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file Spmi.h
    Definitions related to SPMI table.

*/

#ifndef _SPMI_H_
#define _SPMI_H_

//---------------------------------------------------------------------------

#include <IndustryStandard/Acpi.h>
#include <Library/AmiAmlLib.h>
#include <Protocol/AcpiTable.h>
#include <AcpiOemElinks.h>
#include "DxeIpmiInitialize.h"

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

// "SPMI" Service processor Management Interface Table
#define EFI_ACPI_2_0_SERVICE_PROCESSOR_MANAGEMENT_INTERFACE_TABLE_SIGNATURE  0x494d5053

// SPMI REV 5
#define EFI_ACPI_SERVICE_PROCESSOR_MANAGEMENT_INTERFACE_TABLE_REVISION 0x05

#define EFI_ACPI_OEM_SPMI_REVISION 0x00000000

// SPMI Interface Types
# define KEYBOARD_CONTROLLER_STYLE                   0x01
# define SERVER_MANAGEMENT_INTERFACE_CHIP            0x02
# define BLOCK_TRANSFER                              0x03
# define SMBUS_SYSTEM_INTERFACE                      0x04

// Register bit width
#define REG_BIT_WIDTH_BYTE          8
#define REG_BIT_WIDTH_WORD          16
#define REG_BIT_WIDTH_DWORD         32
#define REG_BIT_WIDTH_QWORD         64

// Register bit offset
#define REG_BIT_OFFSET0             0

#define SPMI_ACCESS_BYTE            0x1
#define SPMI_ACCESS_WORD            0x2
#define SPMI_ACCESS_DWORD           0x3
#define SPMI_ACCESS_QWORD           0x4

// UID
#define UID_BYTE1                   0
#define UID_BYTE2                   0
#define UID_BYTE3                   0
#define UID_BYTE4                   0
#define SPMI_GPE                    0
#define SPMI_INTERRUPT_TYPE         0
#define GLOBAL_SYSTEM_INTERRUPT     0
#define PCI_DEVICE_FLAG             0
#define SPMI_CREATOR_ID             0x2E494D41; //"AMI."

#pragma pack (1)
/**
    Service Processor Management Interface Structure.
*/
typedef struct {
    /// Interface type.
    UINT8                                   InterfaceType;
    /// Reserved.
    UINT8                                   Reserved;
    /// Ipmi specification version.
    UINT16                                  SpecificationRevision;
    /// Interrupt type.
    UINT8                                   IntType;
    /// General Purpose Event.
    UINT8                                   Gpe;
    /// Reserved.
    UINT8                                   Reserved1;
    /// Pci device flag.
    UINT8                                   PciDeviceFlag;
    /// Global System Interrupt.
    UINT32                                  GlobalSystemInt;
    /// Base address of the GAS structure.
    EFI_ACPI_2_0_GENERIC_ADDRESS_STRUCTURE  BaseAddress;
    /// PCI Segment Group Number/UID byte 1.
    UINT8                                   PciSegOrUID1;
    /// PCI Bus Number/UID byte 2.
    UINT8                                   PciBusOrUID2;
    /// PCI Device Number/UID byte 3.
    UINT8                                   PciDevOrUID3;
    /// PCI Function Number/UID byte 4.
    UINT8                                   PciFunOrUID4;
    /// Reserved.
    UINT8                                   Reserved2;
} EFI_ACPI_SERVICE_PROCESSOR_MANAGEMENT_INTERFACE_TABLE_STRUCTURE;

/**
    SPMI table structure with ACPI table header.
*/
typedef struct {
    /// ACPI table header.
    EFI_ACPI_DESCRIPTION_HEADER                                         AcpiHeader;
    /// SPMI structure.
    EFI_ACPI_SERVICE_PROCESSOR_MANAGEMENT_INTERFACE_TABLE_STRUCTURE     SpmiData;
} EFI_ACPI_SERVICE_PROCESSOR_MANAGEMENT_INTERFACE_TABLE;

#pragma pack ()

/**
    The SPMI update hook of OEM.

    @param[in] BootServices     Pointer the set of Boot services.
    @param[in] SpmiData         Pointer to the Spmi table data.

    @return EFI_STATUS  Return status of the hooks.

**/
typedef EFI_STATUS (IPMI_OEM_SPMI_UPDATE_HOOK) (
  IN CONST EFI_BOOT_SERVICES    *BootServices,
  IN       VOID                 *SpmiData
);

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

EFI_ACPI_TABLE_PROTOCOL     *gAcpiTable = NULL;
UINTN                       gSpmiTblHandle = 0;

// Service Processor Management Interface Table definition
EFI_ACPI_SERVICE_PROCESSOR_MANAGEMENT_INTERFACE_TABLE gSpmi = {
    {
        EFI_ACPI_2_0_SERVICE_PROCESSOR_MANAGEMENT_INTERFACE_TABLE_SIGNATURE,
        sizeof (EFI_ACPI_SERVICE_PROCESSOR_MANAGEMENT_INTERFACE_TABLE),
        EFI_ACPI_SERVICE_PROCESSOR_MANAGEMENT_INTERFACE_TABLE_REVISION,
        0x00,                           // Checksum will be updated at runtime
        ' ', ' ', ' ', ' ', ' ', ' ',   // OEM ID. It is expected that these values will be programmed at runtime
        0,                              // OEM Table ID
        EFI_ACPI_OEM_SPMI_REVISION,
        0,                              // Creator ID
        0,                              // Creator Revision
    },
    // Beginning of SPMI specific fields
    {
        0,                               // Interface Type
        0x01,                            // Reserved should be 01h
        0,                               // IPMI Specification Version
        SPMI_INTERRUPT_TYPE,
        SPMI_GPE,
        0,                               // Reserved
        PCI_DEVICE_FLAG,
        GLOBAL_SYSTEM_INTERRUPT,
        { // GAS Structure
            EFI_ACPI_2_0_SYSTEM_IO,
            REG_BIT_WIDTH_BYTE,
            REG_BIT_OFFSET0,
            SPMI_ACCESS_BYTE,
            0,                           // Address
        },
        UID_BYTE1,
        UID_BYTE2,
        UID_BYTE3,
        UID_BYTE4,
        0                                // Reserved
    }
};

extern IPMI_OEM_SPMI_UPDATE_HOOK IPMI_OEM_SPMI_UPDATE_HOOK_LIST EndOfIpmiOemSpmiUpdateHookList;
IPMI_OEM_SPMI_UPDATE_HOOK* gIpmiOemSpmiUpdateHookList[] = {IPMI_OEM_SPMI_UPDATE_HOOK_LIST NULL};

//---------------------------------------------------------------------------

#endif  // #ifndef _SPMI_H_


