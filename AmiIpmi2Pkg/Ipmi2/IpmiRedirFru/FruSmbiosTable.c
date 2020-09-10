//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file FruSmbiosTable.c
    Update SMBIOS Type 1, 2, 3 structures with FRU information.

*/
#include "IpmiRedirFru.h"

EFI_SMBIOS_PROTOCOL         *gSmbiosProtocol;

/**
    Add new requested SMBIOS table and return the Table pointer on successful table cration.

    @param[in] SmbiosType         Type of SMBIOS table to add.
    @param[in] SmbiosTableSize    Size of SMBIOS table to add.
    @param[in][out] SmbiosHeader  Pointer to the Added SMBIOS table.

    @return EFI_STATUS

**/
EFI_STATUS
EFIAPI
FruSmbiosTableAdd (
  IN     EFI_SMBIOS_TYPE            SmbiosType,
  IN     UINT8                      SmbiosTableSize,
  IN OUT EFI_SMBIOS_TABLE_HEADER    *SmbiosHeader 
  )
{
    EFI_STATUS                      Status;
    EFI_SMBIOS_HANDLE               SmbiosHandle;
    
    // Type 1 structure is not available, needs to be created.
    SmbiosHeader = AllocateZeroPool (SmbiosTableSize + SIZE_OF_SMBIOS_STRUCTURE_END_MACRO);
    
    if (SmbiosHeader == NULL) {
        DEBUG ((DEBUG_ERROR, "%a Memory allocation failed!! \n", __FUNCTION__));
        return EFI_OUT_OF_RESOURCES;
    }
    
    SmbiosHeader->Type = SmbiosType;
    SmbiosHeader->Length = SmbiosTableSize;
    SmbiosHeader->Handle = 0xFFFF;//To be updated by SMBIOS driver.

    // Creating the Smbios Type 1 Structure.
    SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    Status = gSmbiosProtocol->Add (
                                gSmbiosProtocol,
                                NULL,
                                &SmbiosHandle,
                                SmbiosHeader );
    
    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a SMBIOS Type %x SmbiosProtocol->Add : Status:%r \n", __FUNCTION__, SmbiosType, Status));
        FreePool (SmbiosHeader);
        return Status;
    }
    
    //Free the memory allocated for structure
    FreePool (SmbiosHeader);
    
    Status = gSmbiosProtocol->GetNext (
                                gSmbiosProtocol,
                                &SmbiosHandle,
                                &SmbiosType,
                                &SmbiosHeader,
                                NULL );
    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a SMBIOS Type %x SmbiosProtocol->GetNext Status:%r \n", __FUNCTION__, SmbiosType, Status));
    }
    
    return Status;
}
/**
    Notification function for SMBIOS protocol.
    Update SMBIOS type 1,2,3 structure.

    @param[in] Event    Event which caused this handler.
    @param[in] Context  Context passed during Event Handler registration.

    @return VOID    Nothing.

**/
VOID
EFIAPI
FruSmbiosTableUpdate (
  IN EFI_EVENT  Event,
  IN VOID       *Context 
  )
{
    EFI_STATUS                  Status;
    EFI_SMBIOS_HANDLE           SmbiosHandle;
    EFI_SMBIOS_TYPE             SmbiosType;
    EFI_SMBIOS_TABLE_HEADER     *SmbiosHeader;
    SMBIOS_TABLE_TYPE1          *Type1Structure;
    SMBIOS_TABLE_TYPE2          *Type2Structure;
    SMBIOS_TABLE_TYPE3          *Type3Structure;
    UINTN                       StringNumber;
    CHAR16                      *UnicodeString;
    CHAR16                      *DefaultString = L"To be filled by O.E.M.                                            ";
    CHAR16                      *VersionString = L"UsePartNumber";
    CHAR8                       AsciiString[MAX_STRING_SIZE];
    EFI_GUID                    *Uuid = NULL;
    EFI_GUID                    DefaultUuid;

    DEBUG ((DEBUG_INFO, "%a Entry...\n", __FUNCTION__));

    Status = gBS->LocateProtocol (
                        &gEfiSmbiosProtocolGuid,
                        NULL,
                        (VOID **) &gSmbiosProtocol);

    if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "%a Locate SmbiosProtocol Status:%r \n", __FUNCTION__, Status));
        return;
    }

    Status = gBS->CloseEvent (Event);
    DEBUG ((DEBUG_INFO, "CloseEvent Status:%r \n", Status));

    //SMBIOS Type 1 structure process start.
    SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    SmbiosType = EFI_SMBIOS_TYPE_SYSTEM_INFORMATION;

    Status = gSmbiosProtocol->GetNext (
                                gSmbiosProtocol,
                                &SmbiosHandle,
                                &SmbiosType,
                                &SmbiosHeader,
                                NULL );

    DEBUG ((DEBUG_INFO, "%a Type 1 SmbiosProtocol->GetNext Status:%r \n", __FUNCTION__, Status));

    if (EFI_ERROR(Status)) {

        Status = FruSmbiosTableAdd (
                            SmbiosType,
                            sizeof (SMBIOS_TABLE_TYPE1),
                            SmbiosHeader );

        if (EFI_ERROR(Status)) {
            DEBUG ((DEBUG_ERROR, "%a Failed to add new SMBIOS table!! \n", __FUNCTION__));
            return;
        }
    }

    Type1Structure = (SMBIOS_TABLE_TYPE1 *)SmbiosHeader;

    //Initialize with null string index
    Type1Structure->Manufacturer = 0x00;
    Type1Structure->ProductName = 0x00;
    Type1Structure->Version = 0x00;
    Type1Structure->SerialNumber = 0x00;

    // Copy the Uuid
    Uuid = (EFI_GUID *)PcdGetPtr (AmiPcdSystemUuid);

    // Assigning zero to defaultUuid
    ZeroMem (&DefaultUuid, sizeof(EFI_GUID) );

    if (CompareGuid ((CONST EFI_GUID*)Uuid, (CONST EFI_GUID*)&DefaultUuid ) == FALSE) {
        CopyMem (&(Type1Structure->Uuid), Uuid, sizeof(EFI_GUID));
    }

    // Type 1 Manufacturer String.
    SmbiosHandle = Type1Structure->Hdr.Handle;
    StringNumber = 0x01;
    UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdSystemManufacturer);

    if ( StrnCmp (UnicodeString, DefaultString, MAX_STRING_SIZE) == 0x00 ) {
        UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    }

    if (StrnCmp (UnicodeString, L"", MAX_STRING_SIZE) != 0x00){

        UnicodeStrToAsciiStr (UnicodeString, AsciiString);

        Status = gSmbiosProtocol->UpdateString (
                                        gSmbiosProtocol,
                                        &SmbiosHandle,
                                        &StringNumber,
                                        AsciiString );
        if (!EFI_ERROR(Status)) {
            DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Type 1 Manufacturer String Number [%d] Status:%r \n", Status, StringNumber));
            Type1Structure->Manufacturer = (SMBIOS_TABLE_STRING)StringNumber;
            StringNumber ++;
        }
    }

    // Type 1 Product Name String.
    UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdSystemProductName);
    if ( StrnCmp (UnicodeString, DefaultString, MAX_STRING_SIZE) == 0x00 ) {
        UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    }

    if (StrnCmp (UnicodeString, L"", MAX_STRING_SIZE) != 0x00){

        UnicodeStrToAsciiStr (UnicodeString, AsciiString);

        Status = gSmbiosProtocol->UpdateString (
                                        gSmbiosProtocol,
                                        &SmbiosHandle,
                                        &StringNumber,
                                        AsciiString );
        if (!EFI_ERROR(Status)) {
            DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Type 1 Product Name String Number [%d] Status:%r \n", Status, StringNumber));
            Type1Structure->ProductName = (SMBIOS_TABLE_STRING)StringNumber;
            StringNumber ++;
        }
    }

    // Type 1 Version String.
    UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdSystemVersion);
    if ( StrnCmp (UnicodeString, DefaultString, MAX_STRING_SIZE) == 0x00 ) {
        UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    } 

    if (StrnCmp (UnicodeString, L"", MAX_STRING_SIZE) != 0x00){
        UnicodeStrToAsciiStr (UnicodeString, AsciiString);

        Status = gSmbiosProtocol->UpdateString (
                                        gSmbiosProtocol,
                                        &SmbiosHandle,
                                        &StringNumber,
                                        AsciiString );
        if (!EFI_ERROR(Status)) {
            DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Type 1 Version String Number [%d] Status:%r \n", Status, StringNumber));
            Type1Structure->Version = (SMBIOS_TABLE_STRING)StringNumber;
            StringNumber ++;
        }
    }

    // Type 1 Serial Number String.
    UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdSystemSerialNumber);
    if ( StrnCmp (UnicodeString, DefaultString, MAX_STRING_SIZE) == 0x00 ) {
        UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    }

    if (StrnCmp (UnicodeString, L"", MAX_STRING_SIZE) != 0x00){
        UnicodeStrToAsciiStr (UnicodeString, AsciiString);

        Status = gSmbiosProtocol->UpdateString (
                                        gSmbiosProtocol,
                                        &SmbiosHandle,
                                        &StringNumber,
                                        AsciiString );
        if (!EFI_ERROR(Status)) {
            DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Type 1 Serial Number String Number [%d] Status:%r \n", Status, StringNumber));
            Type1Structure->SerialNumber = (SMBIOS_TABLE_STRING)StringNumber;
        }
    }

    // SMBIOS Type 2 structure process start.
    SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    SmbiosType = EFI_SMBIOS_TYPE_BASEBOARD_INFORMATION;

    Status = gSmbiosProtocol->GetNext (
                                gSmbiosProtocol,
                                &SmbiosHandle,
                                &SmbiosType,
                                &SmbiosHeader,
                                NULL );

    DEBUG ((DEBUG_INFO, "%a Type 2 SmbiosProtocol->GetNext Status:%r \n", __FUNCTION__, Status));

    if (EFI_ERROR(Status)) {

        Status = FruSmbiosTableAdd (
                            SmbiosType,
                            sizeof (SMBIOS_TABLE_TYPE2),
                            SmbiosHeader );

        if (EFI_ERROR(Status)) {
            DEBUG ((DEBUG_ERROR, "%a Failed to add new SMBIOS table!! \n", __FUNCTION__));
            return;
        }
    }

    Type2Structure = (SMBIOS_TABLE_TYPE2 *)SmbiosHeader;

    //Initialize with null string index
    Type2Structure->Manufacturer = 0x00;
    Type2Structure->ProductName = 0x00;
    Type2Structure->Version = 0x00;
    Type2Structure->SerialNumber = 0x00;
    Type2Structure->AssetTag = 0x00;

    // Type 2 Manufacturer String
    SmbiosHandle = Type2Structure->Hdr.Handle;
    StringNumber = 0x01;

    UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdBaseBoardManufacturer);
    if ( StrnCmp (UnicodeString, DefaultString, MAX_STRING_SIZE) == 0x00 ) {
        UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    }

    if (StrnCmp (UnicodeString, L"", MAX_STRING_SIZE) != 0x00) {
        UnicodeStrToAsciiStr (UnicodeString, AsciiString);

        Status = gSmbiosProtocol->UpdateString (
                                        gSmbiosProtocol,
                                        &SmbiosHandle,
                                        &StringNumber,
                                        AsciiString );
        if (!EFI_ERROR(Status)) {
            DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Type 2 Manufacturer String Number [%d] Status:%r \n", StringNumber, Status));
            Type2Structure->Manufacturer = (SMBIOS_TABLE_STRING)StringNumber;
            StringNumber ++;
         }
    }

    // Type 2 Product Name String
    UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdBaseBoardProductName);
    if (StrnCmp (UnicodeString, DefaultString, MAX_STRING_SIZE) == 0x00) {
        UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    }

    if (StrnCmp (UnicodeString, L"", MAX_STRING_SIZE) != 0x00) {
        UnicodeStrToAsciiStr (UnicodeString, AsciiString);

        Status = gSmbiosProtocol->UpdateString (
                                        gSmbiosProtocol,
                                        &SmbiosHandle,
                                        &StringNumber,
                                        AsciiString );
        if (!EFI_ERROR(Status)) {
            DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Type 2 Product Name String Number [%d] Status:%r \n", StringNumber, Status));
            Type2Structure->ProductName = (SMBIOS_TABLE_STRING)StringNumber;
            StringNumber ++;
         }
    }

    // Type 2 Version String.
    UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdBaseBoardVersion);
    if (StrnCmp (UnicodeString, DefaultString, MAX_STRING_SIZE) == 0x00) {
        UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    } else if (StrnCmp (UnicodeString, VersionString, MAX_STRING_SIZE) == 0x00) {
        UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdBaseBoardPartNumber);
        if (StrnCmp (UnicodeString, DefaultString, MAX_STRING_SIZE) == 0x00)
            UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    }

    if (StrnCmp (UnicodeString, L"", MAX_STRING_SIZE) != 0x00) {
        UnicodeStrToAsciiStr (UnicodeString, AsciiString);

        Status = gSmbiosProtocol->UpdateString (
                                        gSmbiosProtocol,
                                        &SmbiosHandle,
                                        &StringNumber,
                                        AsciiString );
        if (!EFI_ERROR(Status)) {
            DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Type 2 Version String Number [%d] Status:%r \n", StringNumber, Status));
            Type2Structure->Version = (SMBIOS_TABLE_STRING)StringNumber;
            StringNumber ++;
         }
    }

    // Type 2 Serial Number String.
    UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdBaseBoardSerialNumber);
    if (StrnCmp (UnicodeString, DefaultString, MAX_STRING_SIZE) == 0x00) {
        UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    }

    if (StrnCmp (UnicodeString, L"", MAX_STRING_SIZE) != 0x00) {
        UnicodeStrToAsciiStr (UnicodeString, AsciiString);

        Status = gSmbiosProtocol->UpdateString (
                                        gSmbiosProtocol,
                                        &SmbiosHandle,
                                        &StringNumber,
                                        AsciiString );
        if (!EFI_ERROR(Status)) {
            DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Type 2 Serial Number String Number [%d] Status:%r \n", StringNumber, Status));
            Type2Structure->SerialNumber = (SMBIOS_TABLE_STRING)StringNumber;
            StringNumber ++;
         }
    }

    // Type 2 AssetTag String.
    UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdBaseBoardAssetTag);
    if ( StrnCmp (UnicodeString, DefaultString, MAX_STRING_SIZE) == 0x00 ) {
        UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    }

    if (StrnCmp (UnicodeString, L"", MAX_STRING_SIZE) != 0x00) {
        UnicodeStrToAsciiStr (UnicodeString, AsciiString);

        Status = gSmbiosProtocol->UpdateString (
                                        gSmbiosProtocol,
                                        &SmbiosHandle,
                                        &StringNumber,
                                        AsciiString );
        if (!EFI_ERROR(Status)) {
            DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Type 2 AssetTag String Number [%d] Status:%r \n", StringNumber, Status));
            Type2Structure->AssetTag = (SMBIOS_TABLE_STRING)StringNumber;
         }
    }

    // SMBIOS Type 3 structure process start.
    SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    SmbiosType = EFI_SMBIOS_TYPE_SYSTEM_ENCLOSURE;

    Status = gSmbiosProtocol->GetNext (
                                gSmbiosProtocol,
                                &SmbiosHandle,
                                &SmbiosType,
                                &SmbiosHeader,
                                NULL );

    DEBUG ((DEBUG_INFO, "%a Type 3 SmbiosProtocol->GetNext Status:%r \n", __FUNCTION__, Status));

    if (EFI_ERROR(Status)) {

        Status = FruSmbiosTableAdd (
                            SmbiosType,
                            sizeof (SMBIOS_TABLE_TYPE3),
                            SmbiosHeader );

        if (EFI_ERROR(Status)) {
            DEBUG ((DEBUG_ERROR, "%a Failed to add new SMBIOS table!! \n", __FUNCTION__));
            return;
        }
    }

    Type3Structure = (SMBIOS_TABLE_TYPE3 *)SmbiosHeader;

    //Initialize with null string index
    Type3Structure->Manufacturer = 0x00;
    Type3Structure->Version = 0x00;
    Type3Structure->SerialNumber = 0x00;
    Type3Structure->AssetTag = 0x00;
    Type3Structure->Type = PcdGet8 (AmiPcdChassisType);

    // Type 3 Manufacturer String
    SmbiosHandle = Type3Structure->Hdr.Handle;
    StringNumber = 0x01;

    UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdChassisManufacturer);
    if (StrnCmp (UnicodeString, DefaultString, MAX_STRING_SIZE) == 0x00) {
        UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    }

    if (StrnCmp (UnicodeString, L"", MAX_STRING_SIZE) != 0x00) {
        UnicodeStrToAsciiStr (UnicodeString, AsciiString);

        Status = gSmbiosProtocol->UpdateString (
                                        gSmbiosProtocol,
                                        &SmbiosHandle,
                                        &StringNumber,
                                        AsciiString );
        if (!EFI_ERROR(Status)) {
            DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Type 3 Manufacturer String Number [%d] Status:%r \n", StringNumber, Status));
            Type3Structure->Manufacturer = (SMBIOS_TABLE_STRING)StringNumber;
            StringNumber ++;
         }
    }

    // Type 3 Version String.
    UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdChassisVersion);
    if (StrnCmp (UnicodeString, DefaultString, MAX_STRING_SIZE) == 0x00 ) {
        UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    } else if (StrnCmp (UnicodeString, VersionString, MAX_STRING_SIZE) == 0x00 ){
        UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdChassisPartNumber);
        if (StrnCmp (UnicodeString, DefaultString, MAX_STRING_SIZE) == 0x00 )
            UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    }

    if (StrnCmp (UnicodeString, L"", MAX_STRING_SIZE) != 0x00) {
        UnicodeStrToAsciiStr (UnicodeString, AsciiString);

        Status = gSmbiosProtocol->UpdateString (
                                        gSmbiosProtocol,
                                        &SmbiosHandle,
                                        &StringNumber,
                                        AsciiString );
        if (!EFI_ERROR(Status)) {
            DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Type 3 Version String Number [%d] Status:%r \n", StringNumber, Status));
            Type3Structure->Version = (SMBIOS_TABLE_STRING)StringNumber;
            StringNumber ++;
         }
    }

    // Type 3 SerialNumber String.
    UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdChassisSerialNumber);
    if (StrnCmp (UnicodeString, DefaultString, MAX_STRING_SIZE) == 0x00) {
        UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    }

    if (StrnCmp (UnicodeString, L"", MAX_STRING_SIZE) != 0x00) {
        UnicodeStrToAsciiStr (UnicodeString, AsciiString);

        Status = gSmbiosProtocol->UpdateString (
                                        gSmbiosProtocol,
                                        &SmbiosHandle,
                                        &StringNumber,
                                        AsciiString );
        if (!EFI_ERROR(Status)) {
            DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Type 3 SerialNumber String Number [%d] Status:%r \n", StringNumber, Status));
            Type3Structure->SerialNumber = (SMBIOS_TABLE_STRING)StringNumber;
            StringNumber ++;
         }
    }

    // Type 3 AssetTag String.
    UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdChassisAssetTag);
    if ( StrnCmp (UnicodeString, DefaultString, MAX_STRING_SIZE) == 0x00 ) {
        UnicodeString = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    }

    if (StrnCmp (UnicodeString, L"", MAX_STRING_SIZE) != 0x00) {
        UnicodeStrToAsciiStr (UnicodeString, AsciiString);

        Status = gSmbiosProtocol->UpdateString (
                                        gSmbiosProtocol,
                                        &SmbiosHandle,
                                        &StringNumber,
                                        AsciiString );
        if (!EFI_ERROR(Status)) {
            DEBUG ((DEBUG_INFO, "SmbiosProtocol->UpdateString Type 3 AssetTag String Number [%d] Status:%r \n", StringNumber, Status));
            Type3Structure->AssetTag = (SMBIOS_TABLE_STRING)StringNumber;
         }
    }

    DEBUG ((DEBUG_INFO, "Exiting from %a\n", __FUNCTION__ ));
}

/** @internal
    Registers notification callback on SMBIOS protocol to update FRU Smbios
    structure.

    @param VOID     Nothing.

    @return VOID    Nothing.

**/
VOID
UpdateFruSmbiosTables (
  VOID 
  )
{
    VOID    *SmbiosProtocolRegistration;

    // Create Notification event for Smbios Protocol.
    EfiCreateProtocolNotifyEvent (
                    &gEfiSmbiosProtocolGuid,
                    TPL_CALLBACK,
                    FruSmbiosTableUpdate,
                    NULL,
                    &SmbiosProtocolRegistration );
}
