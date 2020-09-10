//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file IpmiSmbiosTypes.c
    Installs SMBIOS type 38 and 42 structures.

*/

//---------------------------------------------------------------------------

#include "DxeIpmiInitialize.h"

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define SSIF_INTERFACE_TYPE     0x4

/* Macro represent size of SmBios structure end value. Every SmBios type
   ends with 0x0000.*/
#define SIZE_OF_SMBIOS_STRUCTURE_END_MACRO          sizeof(UINT16)

// Base Address Map Bit.
#define BASE_ADDRESS_MAP_BIT_MASK                   (~BIT0)
#define MMIO_MAPPED_BASE_ADDRESS                    0x00
#define IO_MAPPED_BASE_ADDRESS                      0x01

#define TYPE42_MC_HOST_INTERFACE_DATA               0xFF
#define TYPE42_MC_HOST_PROTOCOL_DATA                0xFFFFFFFF

// NV Storage Device Address.
#ifndef NV_STORAGE_DEVICE_ADDRESS
#define NV_STORAGE_DEVICE_ADDRESS                   0xFF
#endif

// Byte difference.
#define DIFFERENCEBYTE                              0x1
#define DIFFERENCE4BYTE                             0x4
#define DIFFERENCE16BYTE                            0x10

/* Base Address Modifier (This field is unused and set to 00h for SSIF).
       bit  7:6 - Register spacing.
            00b =  interface registers are on successive byte boundaries.
            01b = interface registers are on 32-bit boundaries.
            10b =  interface registers are on 16-byte boundaries.
            11b = reserved.*/
#define REGISTER_SPACING_BYTE_BOUNDARY              (0x00 << 6)
#define REGISTER_SPACING_4BYTE_BOUNDARY             (0x01 << 6)
#define REGISTER_SPACING_16BYTE_BOUNDARY            (0x02 << 6)
#define REGISTER_SPACING_BYTE_BOUNDARY_RESERVED     (0x03 << 6)

/*    bit 5 - reserved. Return as 0b.
      bit 4 -  LS-bit for addresses
          0b = Address bit 0 = 0b
          1b = Address bit 0 = 1b*/
#define BASE_ADDRESS_MODIFIER_LS_BIT_FOR_ADDRESS(BaseAddress) ((BaseAddress & BIT0) << 4)

/* Interrupt Info Identifies the type and polarity of the interrupt
   associated with the IPMI system interface, if any.
      bit 3 - 1b =  interrupt info specified
          0b = interrupt info not specified
      bit 2 - reserved. Return as 0b.
      bit 1 - Interrupt Polarity.
          1b = active high, 0b = active low.
      bit 0 - Interrupt Trigger Mode.
          1b = level, 0b = edge.*/
#define INTERRUPT_INFO_NOT_SPECIFIED                0x00

// Interrupt number for IPMI System Interface. 00h = unspecified/unsupported.
#define INTERRUPT_NUMBER_UNSPECIFIED                0x00

// Finding Register spacing value.
#if (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceKcs)
#define REGISTERDIFFERENCE (IPMI_KCS_COMMAND_PORT - IPMI_KCS_DATA_PORT)
#elif (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceBt)
#define REGISTERDIFFERENCE (IPMI_BT_BUFFER_PORT - IPMI_BT_CTRL_PORT)
#endif

#if REGISTERDIFFERENCE == DIFFERENCEBYTE
#define  REGISTER_SPACING  REGISTER_SPACING_BYTE_BOUNDARY
#elif REGISTERDIFFERENCE == DIFFERENCE4BYTE
#define REGISTER_SPACING  REGISTER_SPACING_4BYTE_BOUNDARY
#elif REGISTERDIFFERENCE == DIFFERENCE16BYTE
#define  REGISTER_SPACING REGISTER_SPACING_16BYTE_BOUNDARY
#endif

// Base address Modifier and Interrupt info defines.
#if (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceKcs)
#define BASE_ADDRESS_MODIFIER_INTERRUPT_INFO   \
              ( REGISTER_SPACING |\
                BASE_ADDRESS_MODIFIER_LS_BIT_FOR_ADDRESS(IPMI_KCS_BASE_ADDRESS) |\
                INTERRUPT_INFO_NOT_SPECIFIED \
              )
#elif (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceBt)
#define BASE_ADDRESS_MODIFIER_INTERRUPT_INFO   \
              ( REGISTER_SPACING |\
                BASE_ADDRESS_MODIFIER_LS_BIT_FOR_ADDRESS(IPMI_BT_BASE_ADDRESS) |\
                INTERRUPT_INFO_NOT_SPECIFIED \
              )
#else
#define BASE_ADDRESS_MODIFIER_INTERRUPT_INFO   0x00
#endif


/**
    Management Controller Host Interface Type Identifiers.
*/
typedef enum {
    /// Reserved.
    AmiMcHostInterfaceTypeReserved0     = 0x00,
    /// Reserved.
    AmiMcHostInterfaceTypeReserved1     = 0x01,
    /// The Keyboard Controller Style.
    AmiMcHostInterfaceTypeKCS           = 0x02,
    /// 8250 UART Register Compatible.
    AmiMcHostInterfaceType8250UART      = 0x03,
    /// 16450 UART Register Compatible.
    AmiMcHostInterfaceType16450UART     = 0x04,
    /// 16550/16550A UART Register Compatible.
    AmiMcHostInterfaceType16550UART     = 0x05,
    /// 16650/16650A UART Register Compatible.
    AmiMcHostInterfaceType16650UART     = 0x06,
    /// 16750/16750A UART Register Compatible.
    AmiMcHostInterfaceType16750UART     = 0x07,
    /// 16850/16850A UART Register Compatible.
    AmiMcHostInterfaceType16850UART     = 0x08,
    /// OEM.
    AmiMcHostInterfaceTypeOEM           = 0xF0,
    /// Reserved.
    AmiMcHostInterfaceTypeReserved      = 0xF1
} AMI_MC_HOST_INTERFACE_TYPE;

/**
    Management Controller Host Interface Protocol Identifiers.
*/
typedef enum {
    /// Reserved.
    AmiMcHostInterfaceProtocolReserved0     = 0x00,
    /// Reserved.
    AmiMcHostInterfaceProtocolReserved1     = 0x01,
    /// IPMI : Intelligent Platform Management Interface.
    AmiMcHostInterfaceProtocolIPMI          = 0x02,
    /// MCTP : Management Component Transport Protocol.
    AmiMcHostInterfaceProtocolMCTP          = 0x03,
    /// OEM.
    AmiMcHostInterfaceProtocolOEM           = 0xF0,
    /// Reserved.
    AmiMcHostInterfaceProtocolReserved      = 0xF1
} AMI_MC_HOST_INTERFACE_PROTOCOL;

#pragma pack(1)
/**
    Smbios type 42 protocol structure.
*/
typedef struct {
    /// Protocol type.
    UINT8               ProtocolType;
    UINT8               ProtocolTypeSpecificDataLength;   ///< Protocol Type Specific Data Length.
    UINT32              ProtocolTypeSpecificData;   ///< Protocol Type specific data.
} AMI_SMBIOS_TYPE42_MC_HOST_PROTOCOL;

/**
    Smbios type 42 instance structure.
*/
typedef struct {
    /// Smbios type 42.
    SMBIOS_TABLE_TYPE42             SmbiosType42;
    /// Number of protocol Records.
    UINT8                           NumberOfProtocolRecords;
    /// Smbios type 42 protocol Record list.
    AMI_SMBIOS_TYPE42_MC_HOST_PROTOCOL  ProtocolRecords[1];
} AMI_SMBIOS_TABLE_TYPE42_INSTANCE;
#pragma pack()

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

extern IPMI_OEM_SMBIOS_TYPES_UPDATE_HOOK IPMI_OEM_SMBIOS_TYPES_UPDATE_HOOK_LIST EndOfIpmiOemSmbiosTypesHookList;
IPMI_OEM_SMBIOS_TYPES_UPDATE_HOOK *gIpmiOemSmbiosTypesUpdateHookList[] = { IPMI_OEM_SMBIOS_TYPES_UPDATE_HOOK_LIST NULL };

//---------------------------------------------------------------------------

/** @internal
    Initiates the Smbios Types structure update hooks of OEM.

    @param[in, out] SmbiosTypeRecord    Pointer to the Smbios types structure.

    @return EFI_STATUS  Return status of the hooks.

**/
EFI_STATUS IpmiOemSmbiosTypesUpdateHook (
  IN OUT VOID   **SmbiosTypeRecord )
{
    EFI_STATUS  Status;
    UINTN       Index;

    for (Index = 0; gIpmiOemSmbiosTypesUpdateHookList[Index]; Index++) {
        Status = gIpmiOemSmbiosTypesUpdateHookList [Index] (SmbiosTypeRecord);
    }
    return Status;
}

/**
    Notification function for SMBIOS protocol.
    Installs SMBIOS type 38 and type 42 structure.

    @param[in] Event    Event which caused this handler.
    @param[in] Context  Context passed during Event Handler registration.

    @return VOID    Nothing.

**/
VOID
EFIAPI
InstallSmbiosTypesStructure (
  IN  EFI_EVENT     Event,
  IN  VOID          *Context )
{
    EFI_STATUS                          Status;
    EFI_SMBIOS_HANDLE                   SmbiosHandle;
    EFI_SMBIOS_PROTOCOL                 *SmbiosProtocol;
    EFI_BMC_SELF_TEST_LOG_PROTOCOL      *BmcSelfTestProtocol = NULL;
    SMBIOS_TABLE_TYPE38                 *Type38Record;
    AMI_SMBIOS_TABLE_TYPE42_INSTANCE    *Type42Record;
    EFI_SMBIOS_TYPE                     Type;
    EFI_SMBIOS_TABLE_HEADER             *Record;

    DEBUG ((DEBUG_INFO, "%a Entered...  \n", __FUNCTION__));

    Status = gBS->LocateProtocol (
                    &gEfiSmbiosProtocolGuid,
                    NULL,
                    (VOID **) &SmbiosProtocol );
    
    DEBUG ((DEBUG_INFO, "gBS->LocateProtocol gEfiSmbiosProtocolGuid protocol  status %r\n", Status));
    
    if (EFI_ERROR (Status)) {
        return;
    }

    SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    Type = EFI_SMBIOS_TYPE_IPMI_DEVICE_INFORMATION;
    Status = SmbiosProtocol->GetNext (
                                SmbiosProtocol,
                                &SmbiosHandle,
                                &Type,
                                &Record,
                                NULL );
    DEBUG ((DEBUG_INFO, "SmbiosProtocol->GetNext Status:%r \n", Status));
    if (EFI_ERROR(Status)) {
    
        // Allocate memory for SmBiosType38 structure
        Type38Record = (SMBIOS_TABLE_TYPE38 *)AllocateZeroPool (sizeof (SMBIOS_TABLE_TYPE38) + SIZE_OF_SMBIOS_STRUCTURE_END_MACRO);
        if (Type38Record == NULL) {
            DEBUG ((DEBUG_ERROR, "Allocating Space for SmBios Type38Record fails...\n"));
            return;
        }
    
        // Fill Type 38 Structure
        Type38Record->Hdr.Type      = EFI_SMBIOS_TYPE_IPMI_DEVICE_INFORMATION;
        Type38Record->Hdr.Length    = sizeof (SMBIOS_TABLE_TYPE38);
        Type38Record->Hdr.Handle    = 0xFFFF;    // To be updated by SMBIOS driver.
        // Creating the Smbios Type 38 Structure.
        //
        SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
        Status = SmbiosProtocol->Add (
                                    SmbiosProtocol,
                                    NULL,
                                    &SmbiosHandle,
                                    (EFI_SMBIOS_TABLE_HEADER*) Type38Record );
        DEBUG ((DEBUG_INFO, "SmbiosProtocol->Add Status:%r \n", Status));
        if (EFI_ERROR (Status)) {
            return;
        }

        //Free the memory allocated for Type 38 structure
        FreePool(Type38Record);
        SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
        Status = SmbiosProtocol->GetNext (
                                    SmbiosProtocol,
                                    &SmbiosHandle,
                                    &Type,
                                    &Record,
                                    NULL );
        if (EFI_ERROR (Status)) {
            DEBUG ((DEBUG_INFO, "SmbiosProtocol->GetNext EFI_SMBIOS_TYPE_IPMI_DEVICE_INFORMATION Status:%r \n", Status));
            return;
        }
    }
    Type38Record = (SMBIOS_TABLE_TYPE38 *)Record;
    
    // Locate the BMC self test protocol and update IPMI Specification Version.
    Status = gBS->LocateProtocol (
                    &gEfiBmcSelfTestLogProtocolGuid,
                    NULL,
                    (VOID **)&BmcSelfTestProtocol );
    
    DEBUG ((DEBUG_INFO, "gEfiBmcSelfTestLogProtocolGuid: Status:  %r\n", Status));

    if ((!EFI_ERROR (Status)) && BmcSelfTestProtocol->IsGetDeviceIdCommandPassed) {
        /**
            In IPMI device Id command SpecificationVersion Bits 7:4 hold the least significant digit, while
            Bits 3:0 hold the most significant digit.
            Example:  A value of 02h indicates version 2.0.
            SMBIOS record IPMISpecificationRevision Bits 7:4 hold the most significant digit of the version, while
            Bits 3:0 hold the least significant bits.
            Example: A value of 20h indicates version 2.0.
        */
        Type38Record->IPMISpecificationRevision = (BmcSelfTestProtocol->GetDeviceIdResponse.SpecificationVersion << 0x04 | \
                                                   BmcSelfTestProtocol->GetDeviceIdResponse.SpecificationVersion >> 0x04);
    }

    switch (DEFAULT_SYSTEM_INTERFACE) {
        case AmiSysInterfaceKcs:
            Type38Record->InterfaceType = IPMIDeviceInfoInterfaceTypeKCS;
            Type38Record->BaseAddress = ((IPMI_KCS_BASE_ADDRESS & BASE_ADDRESS_MAP_BIT_MASK)| IO_MAPPED_BASE_ADDRESS);
            break;
            
        case AmiSysInterfaceBt:
            Type38Record->InterfaceType = IPMIDeviceInfoInterfaceTypeBT;
            Type38Record->BaseAddress = ((IPMI_BT_BASE_ADDRESS & BASE_ADDRESS_MAP_BIT_MASK)| IO_MAPPED_BASE_ADDRESS);
            break;
            
        case AmiSysInterfaceSsif:
            Type38Record->InterfaceType = IPMIDeviceInfoInterfaceTypeSSIF;
            Type38Record->BaseAddress = IPMI_SSIF_SLAVE_ADDRESS << 1;
            break;
            
        default :
            Type38Record->InterfaceType = IPMIDeviceInfoInterfaceTypeUnknown ;//Unknown interface
            break;
    }

    Type38Record->I2CSlaveAddress = gIpmiInstance->SlaveAddress;
    Type38Record->NVStorageDeviceAddress = NV_STORAGE_DEVICE_ADDRESS;
    Type38Record->BaseAddressModifier_InterruptInfo = (UINT8)BASE_ADDRESS_MODIFIER_INTERRUPT_INFO;
    Type38Record->InterruptNumber = INTERRUPT_NUMBER_UNSPECIFIED;
    
    // Update SMBIOS Type 38 record with OEM Hooks if any.
    Status = IpmiOemSmbiosTypesUpdateHook ((VOID **) &Type38Record);
    
    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "IpmiOemSmbiosTypesUpdateHook TYPE 38 status is %r and initializing unspecified values.\n", Status));
        Type38Record->BaseAddress = 0x00;
        Type38Record->BaseAddressModifier_InterruptInfo = 0x00;
    }


    // Allocate memory for SmBiosType42 structure.
    Type42Record = (AMI_SMBIOS_TABLE_TYPE42_INSTANCE *)AllocateZeroPool (sizeof (AMI_SMBIOS_TABLE_TYPE42_INSTANCE) + SIZE_OF_SMBIOS_STRUCTURE_END_MACRO);
    if (Type42Record == NULL) {
        DEBUG ((DEBUG_ERROR, "Allocating Space for SmBios Type42Record fails...\n"));
        return;
    }
    
    // Fill Type 42 Structure.
    Type42Record->SmbiosType42.Hdr.Type = EFI_SMBIOS_TYPE_MANAGEMENT_CONTROLLER_HOST_INTERFACE;
    Type42Record->SmbiosType42.Hdr.Length = sizeof (AMI_SMBIOS_TABLE_TYPE42_INSTANCE);  //Length of the structure, a minimum of 0Bh.
    Type42Record->SmbiosType42.Hdr.Handle = 0xFFFF;    //To be updated by SMBIOS driver.
    
    switch (DEFAULT_SYSTEM_INTERFACE) {
        case AmiSysInterfaceKcs:
            Type42Record->SmbiosType42.InterfaceType = AmiMcHostInterfaceTypeKCS;  //TO BE DONE <Need to check with DMTF for other Interface types>
            break;
            
        default :
            Type42Record->SmbiosType42.InterfaceType = AmiMcHostInterfaceTypeOEM;
            break;
    }
    Type42Record->SmbiosType42.InterfaceTypeSpecificDataLength = sizeof (Type42Record->SmbiosType42.InterfaceTypeSpecificData); 
    Type42Record->SmbiosType42.InterfaceTypeSpecificData[0] = TYPE42_MC_HOST_INTERFACE_DATA;  // This field has a minimum of four bytes.
    Type42Record->NumberOfProtocolRecords = 0x01;
    Type42Record->ProtocolRecords[0].ProtocolType = AmiMcHostInterfaceProtocolIPMI;
    Type42Record->ProtocolRecords[0].ProtocolTypeSpecificDataLength = sizeof (Type42Record->ProtocolRecords[0].ProtocolTypeSpecificData);
    Type42Record->ProtocolRecords[0].ProtocolTypeSpecificData = TYPE42_MC_HOST_PROTOCOL_DATA;
    
    
    // Update Smbios Type 42 record with Oem Hooks if any.
    Status = IpmiOemSmbiosTypesUpdateHook ((VOID **) &Type42Record);
    
    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "IpmiOemSmbiosTypesUpdateHook TYPE 42 status is %r and initializing unspecified values.\n", Status));
    }
    
    // Now we have got the full SMBIOS 42 record, call SMBIOS protocol to add this record.
    SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    Status = SmbiosProtocol->Add (
                                SmbiosProtocol,
                                NULL,
                                &SmbiosHandle,
                                (EFI_SMBIOS_TABLE_HEADER*) Type42Record );
        
    DEBUG ((DEBUG_INFO, "SmbiosProtocol->Add Type42Record....  Status: %r \n", Status));
    FreePool (Type42Record);

    //Funtion to update System Information to BMC
    Status = IpmiUpdateSystemInfo (SmbiosProtocol);
    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "IpmiUpdateSystemInfo Status %r\n", Status));
    }
    
    Status = gBS->CloseEvent (Event);
    
    DEBUG ((DEBUG_INFO, "%a Exiting...  \n", __FUNCTION__));
}

/** @internal
    Registers notification call back on SMBIOS protocol to install Smbios
    type 38 and 42 structure.

    @param VOID     Nothing.

    @return VOID    Nothing.

**/
VOID
IpmiSmbiosTypesSupport (
  VOID )
{
    VOID    *SmbiosProtocolRegistration;

    DEBUG ((DEBUG_INFO, "%a Entered...   \n", __FUNCTION__));

    // Create Notification event for SmbiosProtocol GUID
    EfiCreateProtocolNotifyEvent (
        &gEfiSmbiosProtocolGuid,
        TPL_CALLBACK,
        InstallSmbiosTypesStructure,
        NULL,
        &SmbiosProtocolRegistration );

    DEBUG ((DEBUG_INFO, "%a Exiting...\n", __FUNCTION__));
}


