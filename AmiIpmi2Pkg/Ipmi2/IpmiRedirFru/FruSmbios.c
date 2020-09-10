//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file FruSmbios.c
    Function implementations to publish Fru strings in Smbios tables.

*/

#include "IpmiRedirFru.h"

/** @internal
    Converts the low nibble of a byte to hex unicode character.

    @param[in] Nibble   The Nibble that needs to be converted as Hex Unicode
                        character.

    @return CHAR16  The Hex Unicode character of input nibble.

**/
CHAR16
NibbleToHexChar (
  IN UINT8  Nibble )
{
    Nibble &= 0x0F;
    if (Nibble <= 0x9) {
        return (CHAR16)(Nibble + L'0');
    }

    return (CHAR16)(Nibble - 0xA + L'A');
}

/** @internal
    Converts binary input string to Unicode hexadecimal string.

    @param[out]     Str                     The output unicode hex string.
    @param[in, out] HexStringBufferLength   On input, length of the unicode buffer.
                                            On output, unicode hex string length.
    @param[in]      Buf                     The Binary input string.
    @param[in]      Len                     The Binary input string length.

    @retval EFI_BUFFER_TOO_SMALL  Str string buffer length is not sufficient.
    @retval EFI_SUCCESS           The Binary input string is converted successfully.

**/
EFI_STATUS
BufToHexString (
  OUT    CHAR16     *Str,
  IN OUT UINTN      *HexStringBufferLength,
  IN     UINT8      *Buf,
  IN     UINTN      Len )
{
    UINTN   Idx;
    UINT8   Byte;
    UINTN   StrLen;

    /* Make sure string is either passed or allocate enough.
      It takes 2 Unicode characters (4 bytes) to represent 1 byte of the binary buffer.
      Plus the Unicode termination character.*/
    StrLen = Len * 2;
    if (StrLen > ((*HexStringBufferLength) - 1)) {
        *HexStringBufferLength = StrLen + 1;
        return EFI_BUFFER_TOO_SMALL;
    }

    *HexStringBufferLength = StrLen + 1;
    // Ends the string.
    Str[StrLen] = L'\0';

    for (Idx = 0; Idx < Len; Idx++) {
        Byte = Buf[Idx];
        Str[StrLen - 1 - Idx * 2] = NibbleToHexChar (Byte);
        Str[StrLen - 2 - Idx * 2] = NibbleToHexChar ((UINT8)(Byte >> 4));
    }

    return EFI_SUCCESS;
}

/** @internal
    Converts binary buffer to Unicode character.

    @param[out] UnicodeStr  The Unicode string to be written to.
                            The buffer must be large enough.
    @param[in]  BinPtr      The binary string to be converted.
    @param[in]  Length      Length of the binary string to be converted.

    @return CHAR16* The address to the Unicode string - same as UnicodeStr.

**/
CHAR16*
IpmiBinary2Unicode (
  OUT CHAR16    *UnicodeStr,
  IN  UINT8     *BinPtr,
  IN  UINTN     Length )
{
    EFI_STATUS    Status;
    UINTN         BufferLen;

    /* 1 byte of the binary buffer takes 2 unicode characters (4 bytes) for hex representation.
       BufferLen includes space for hex representation + "h" suffix + null terminator.*/
    BufferLen = (Length * 2 + 2) * sizeof (UINT16);

    Status = BufToHexString (
                UnicodeStr,
                &BufferLen,
                BinPtr,
                Length );

    if (EFI_ERROR(Status)) {
        UnicodeStr[0] = L'\0';
    } else {
        UnicodeStr[Length * 2] = L'h';
        UnicodeStr[Length * 2 + 1] = L'\0';
    }

    return UnicodeStr;
}

/** @internal
    Converts 6bit ASCII characters to Unicode.

    @param[out] UnicodeStr  The Unicode string to be written to. The buffer must
                            be large enough.
    @param[in]  AsciiStr    The 6bit ASCII string to be converted.
    @param[in]  Length      Length of the ASCII string to be converted.

    @retval CHAR16* The address to the Unicode string.

**/
CHAR16*
Ipmi6bAscii2Unicode (
  OUT CHAR16    *UnicodeStr,
  IN  UINT8     *AsciiStr,
  IN  UINTN     Length )
{
    UINTN   StrIndex1;
    UINTN   StrIndex2;
    UINT32  Str;

    /* Unpack 3 6bit value (24 bits) to 4 8bit value at a time (3 bytes to 4 bytes)
       Also 6bit to 8bit value has to be offset by 0x20.
       i.e., 000000b -> 0x20, 000001b -> 0x21, etc.*/

    // Convert AsciiStr up to length that is multiple of 3.
    for (StrIndex1 = 0; StrIndex1 < Length / 3; StrIndex1++) {
        Str = *((UINT32 *) (AsciiStr + StrIndex1 * 3));
        for (StrIndex2 = 0; StrIndex2 < 4; StrIndex2++) {
            UnicodeStr[StrIndex1 * 4 + StrIndex2] = (CHAR8) (Str & 0x3F) + 0x20;
            Str >>= 6;
        }
    }

    // Convert rest of AsciiStr entries.
    Str = *((UINT32 *) (AsciiStr + StrIndex1 * 3));
    for (StrIndex2 = 0; StrIndex2 < (Length - ((Length / 3) * 3)); StrIndex2++) {
        UnicodeStr[StrIndex1 * 4 + StrIndex2] = (CHAR8) (Str & 0x3F) + 0x20;
        Str >>= 6;
    }

    UnicodeStr[StrIndex1 * 4 + StrIndex2] = '\0';

    return UnicodeStr;
}
/**
    Converts ASCII characters to Unicode.

    @param[OUT] UnicodeStr   The Unicode string to be written to. The buffer must
                             be large enough.
    @param[IN]  AsciiStr     The ASCII string to be converted.
    @param[IN]  Length       Length of the ASCII string to be converted.

    @retval CHAR16* The address to the Unicode string - same as UnicodeStr.

**/

CHAR16*
IpmiAscii2Unicode (
  OUT CHAR16         *UnicodeStr,
  IN  UINT8          *AsciiStr,
  IN  UINTN           Length )
{
    UINTN   i;

    for (i = 0; i < Length; i++) {
        UnicodeStr[i] = AsciiStr[i];
    }

    UnicodeStr[i] = '\0';

    return UnicodeStr;
}
/** @internal
    Reads the FRU data from InputDataBuffer and convert it to unicode string.

    @param[in]  InputDataBuffer         Input FRU data buffer.
    @param[in]  Type                    FRU data encoded type.
    @param[in]  Length                  FRU data length.
    @param[out] OutputUnicodeString     Output unicode string buffer.

    @retval EFI_SUCCESS           FRU data converted to unicode successfully.
    @retval EFI_UNSUPPORTED       FRU data type not supported.
    @retval EFI_OUT_OF_RESOURCES  Buffer allocation failed for unicode string.

**/
EFI_STATUS
GetUnicodeFromEncodedFru (
  IN  UINT8     *InputDataBuffer,
  IN  UINT8     Type,
  IN  UINT8     Length,
  OUT CHAR16    **OutputUnicodeString )
{
    EFI_STATUS  Status = EFI_SUCCESS;

    switch (Type) {
        case 0:
            if (FRU_SMBIOS_BINARY_TYPE_CODE_SUPPORT) {
                /* Binary coding. Allocate space for hex representation + "h" suffix
                   + null terminator.*/
                *OutputUnicodeString = AllocateRuntimeZeroPool ((Length * 2 + 2) * sizeof(UINT16));
                if (*OutputUnicodeString != NULL) {
                    IpmiBinary2Unicode (
                        *OutputUnicodeString,
                        (InputDataBuffer + 1),
                        Length );
                    break;
                }
            }
            Status = EFI_OUT_OF_RESOURCES;

        case 2:
            // 6 bit ASCII Packed coding.
            *OutputUnicodeString = AllocateRuntimeZeroPool ((((Length * 8) / 3) + 2));
            if (*OutputUnicodeString != NULL) {
                Ipmi6bAscii2Unicode (
                    *OutputUnicodeString,
                    (InputDataBuffer + 1),
                    Length );
                break;
            }
            Status = EFI_OUT_OF_RESOURCES;

        case 3:
            // ASCII coding.
            *OutputUnicodeString = AllocateRuntimeZeroPool (((Length * 2) + 2));
            if (*OutputUnicodeString != NULL) {
                IpmiAscii2Unicode (
                    *OutputUnicodeString,
                    (InputDataBuffer + 1),
                    Length );
                break;
            }
            Status = EFI_OUT_OF_RESOURCES;

        default:
            Status = EFI_UNSUPPORTED;
    }
    return Status;
}

/** @internal
    Reads the FRU data from InputDataBuffer and save it in OutputUnicodeString.

    @param[in]  InputDataBuffer         Input FRU data buffer.
    @param[out] OutputUnicodeString     Output unicode string buffer.

    @retval EFI_SUCCESS     FRU data converted to unicode successfully.
    @retval EFI_NOT_FOUND   FRU data length is not valid in InputDataBuffer.
    @retval Others          Return status of function used.

**/

EFI_STATUS
GetFruData (
  IN  UINT8     *InputDataBuffer,
  OUT CHAR16    **OutputUnicodeString 
  )
{
    UINT8       Type = (((*InputDataBuffer) & FRU_DATA_TYPE_MASK) >> 0x06);
    UINT8       Length = ((*InputDataBuffer) & FRU_DATA_LENGTH_MASK);
    EFI_STATUS  Status;

    if (Length > 0) {
        Status = GetUnicodeFromEncodedFru (
                    InputDataBuffer,
                    Type,
                    Length,
                    OutputUnicodeString );
    } else {
        return EFI_NOT_FOUND;
    }
    return Status;
}

/** @internal
    Reads the FRU information and publishes the FRU stings by updating the
    dynamic PCDs. If the FRU information is not present, publishes dummy records.

    @param[in] This                         Pointer to the Redir FRU protocol.
    @param[in] GenerateDummySmbiosRecords   If TRUE, generates dummy FRU records.
                                            If FALSE, generates FRU records.

    @return VOID    Nothing.

**/
VOID
GenerateFruSmbiosData (
  IN EFI_SM_FRU_REDIR_PROTOCOL                      *This
  )
{
    EFI_STATUS                                      Status;
    AMI_IPMI_FRU_GLOBAL                             *FruPrivate;
    AMI_IPMI_FRU_COMMON_HEADER                      FruCommonHeader;
    UINTN                                           Index;
    UINTN                                           Offset = 0;
    UINTN                                           Length;
    UINT8                                           *TempPtr;
    UINT8                                           PcdSize;
    CHAR16                                          *StrPtr;
    CHAR16                                          *AssetTagString = NULL;
    CHAR16                                          *ManufacturerString = NULL;
    CHAR16                                          *ProductName = NULL;
    CHAR16                                          *ProductNameWithModelNo = NULL;
    UINTN                                           SizeofBuffer;
    UINT8                                           FruRecIndex = 0;
    AMI_IPMI_FRU_MULTI_RECORD_HEADER                MultiRecordHeader;
    BOOLEAN                                         IsSystemUuidSubRecordType = FALSE;
    UINT8                                           SubRecordType = 0;
    AMI_IPMI_SYSTEM_UUID_SUB_RECORD_WITH_CHECKSUM   SystemUuidSubRecordwithChecksum;
    UINTN                                           GetFruRedirDataSize;
    EFI_GUID                                        *Uuid = NULL;
    EFI_GUID                                        DefaultUuid;

    DEBUG ((DEBUG_INFO, "%a Entry... \n", __FUNCTION__));

    /* Search through the FRU records in mIpmiFruGlobal->FruDeviceInfo
       to identify FRU_DEVICE_ID record.*/
    if (FRU_DEVICE_ID != 0xFF) {
        FruPrivate = AMI_IPMI_INSTANCE_FROM_EFI_SM_IPMI_FRU_THIS (This);
        for (Index = 0; Index < FruPrivate->NumSlots; Index++) {
            if ((FruPrivate->FruDeviceInfo[Index].Valid) && (FruPrivate->FruDeviceInfo[Index].FruDevice.Bits.FruDeviceId == FRU_DEVICE_ID)) {
                FruRecIndex = (UINT8) Index;
                break;
            }
        }
    }

    GetFruRedirDataSize = sizeof (AMI_IPMI_FRU_COMMON_HEADER);
    Status = This->GetFruRedirData (
                            This,
                            FruRecIndex,
                            0,
                            &GetFruRedirDataSize,
                            (UINT8*) &FruCommonHeader );

    if (EFI_ERROR (Status)) {
        return;
    }
    if (CalculateCheckSum8 (
            (UINT8 *)&FruCommonHeader,
            sizeof (FruCommonHeader) ) != 0) {
        DEBUG((DEBUG_ERROR, "FRU header invalid.\n"));
        return;
    }
    // Prepare Manufacturer data (SMBIOS Type 1).

    Offset = FruCommonHeader.ProductInfoStartingOffset * 8;

    if (Offset > 0) {
        Length = 0;
        // Size of the length of FRU area to read.
        GetFruRedirDataSize = sizeof (UINT8);
        Status = This->GetFruRedirData (
                                This,
                                FruRecIndex,
                                (Offset + 1),
                                &GetFruRedirDataSize,
                                (UINT8*) &Length );

        if (!EFI_ERROR(Status) && (Length > 0)) {

            Length = Length * 8;
            TempPtr = (UINT8*)AllocateRuntimeZeroPool (Length);

            if (TempPtr != NULL) {
                Status = This->GetFruRedirData  (
                                        This,
                                        FruRecIndex,
                                        Offset,
                                        &Length,
                                        TempPtr );

                if (!EFI_ERROR (Status)) {
                    /*  Validating Product Info Area Checksum before reading
                        the Product Info Area Data.*/
                    if (CalculateCheckSum8 (
                            TempPtr,
                            Length ) == 0) {
                        // Product Manufacturer.
                        Offset = PRODUCT_MFG_OFFSET;
                        Length = FRU_DATA_LENGTH_MASK & TempPtr[Offset];
                        Status = GetFruData (
                                     &TempPtr[Offset],
                                     &ManufacturerString );

                        if (!EFI_ERROR (Status)) {
                            SizeofBuffer = StrnSizeS (ManufacturerString, MAX_STRING_SIZE);
                            PcdSize = (UINT8)PcdGetSize(AmiPcdSystemManufacturer);
                            if (SizeofBuffer > PcdSize) {
                                DEBUG ((DEBUG_INFO, "AmiPcdSystemManufacturer:SizeofBuffer > PcdSize so truncating the string\n"));
                                SizeofBuffer = PcdSize - 2;//To avoid overwriting last byte of PCD data(Null terminator)
                            }
                            DEBUG ((DEBUG_INFO, "SystemManufacturer: ManufacturerString: %S\n", ManufacturerString));
                            PcdSetPtrS (
                                AmiPcdSystemManufacturer,
                                &SizeofBuffer,
                                (VOID*)ManufacturerString );
                            DEBUG ((DEBUG_INFO, "SystemManufacturer: %S\n", (CHAR16 *) PcdGetPtr (AmiPcdSystemManufacturer)));
                        }

                        /* Product Name. For SMBIOS, it is actually concatenation of Product
                           Name and Model Number or Part Number.*/
                        Offset = Offset + Length + 1;
                        Length = FRU_DATA_LENGTH_MASK & TempPtr[Offset];

                        Status = GetFruData (
                                    &TempPtr[Offset], 
                                    &ProductName);

                        Offset = Offset + Length + 1;
                        Length = FRU_DATA_LENGTH_MASK & TempPtr[Offset];

                        /* Product Model Number, concatenate it to the existing
                           (if exists) Product Name.*/
                        if (!EFI_ERROR (Status)) {
                            Status = GetFruData (
                                        &TempPtr[Offset],
                                        &StrPtr );

                            if (!EFI_ERROR (Status)) {
                                ProductNameWithModelNo = (CHAR16*)AllocateRuntimeZeroPool (StrnSizeS (ProductName, MAX_STRING_SIZE) + StrnSizeS (StrPtr, MAX_STRING_SIZE) + 2); // 2 bytes for space character L" "
                                if (ProductNameWithModelNo != NULL) {
                                    StrCpyS (
                                       ProductNameWithModelNo,
                                       MAX_STRING_SIZE,
                                       ProductName );

                                    StrCat (
                                       ProductNameWithModelNo,
                                       L" " );

                                    StrCat (
                                       ProductNameWithModelNo,
                                       StrPtr );

                                    FreePool (ProductName);
                                    FreePool (StrPtr);
                                    SizeofBuffer = StrnSizeS (ProductNameWithModelNo, MAX_STRING_SIZE);
                                    PcdSize = (UINT8)PcdGetSize(AmiPcdSystemProductName);
                                    if (SizeofBuffer > PcdSize) {
                                        DEBUG ((DEBUG_INFO, " AmiPcdSystemProductName:SizeofBuffer > PcdSize so truncating the string \n"));
                                        SizeofBuffer = PcdSize - 2;//To avoid overwriting last byte of PCD data(Null terminator)
                                    }
                                    DEBUG ((DEBUG_INFO, "SystemProductName: StrPtr: %S\n", ProductNameWithModelNo));

                                    PcdSetPtrS (
                                        AmiPcdSystemProductName,
                                        &SizeofBuffer,
                                        (VOID*)ProductNameWithModelNo );

                                    DEBUG ((DEBUG_INFO, "SystemProductName: %S\n", (CHAR16 *) PcdGetPtr (AmiPcdSystemProductName)));
                                    FreePool (ProductNameWithModelNo);
                                } else { // Free the allocated buffer for StrPtr
                                    FreePool (StrPtr);
                                  } 
                            } else { // Update the product name as Model No not available
                                SizeofBuffer = StrnSizeS (ProductName, MAX_STRING_SIZE);
                                PcdSize = (UINT8)PcdGetSize(AmiPcdSystemProductName);
                                if (SizeofBuffer > PcdSize) {
                                    DEBUG ((DEBUG_INFO, "AmiPcdSystemProductName:SizeofBuffer > PcdSize so truncating the string \n"));
                                    SizeofBuffer = PcdSize - 2;//To avoid overwriting last byte of PCD data(Null terminator)
                                }
                                DEBUG ((DEBUG_INFO, " AmiPcdSystemProductName: ProductName: %S\n", ProductName));
                                PcdSetPtrS (
                                    AmiPcdSystemProductName,
                                    &SizeofBuffer,
                                    (VOID*)ProductName );

                                DEBUG ((DEBUG_INFO, "SystemProductName: %S\n", (CHAR16 *) PcdGetPtr (AmiPcdSystemProductName)));
                                FreePool (ProductName);
                              }
                        } 

                        // Product Version.
                        Offset = Offset + Length + 1;
                        Length = FRU_DATA_LENGTH_MASK & TempPtr[Offset];
                        Status = GetFruData (
                                     &TempPtr[Offset],
                                     &StrPtr );
                        if (!EFI_ERROR (Status)) {
                            SizeofBuffer = StrnSizeS (StrPtr, MAX_STRING_SIZE);
                            PcdSize = (UINT8)PcdGetSize(AmiPcdSystemVersion);
                            if (SizeofBuffer > PcdSize) {
                                DEBUG ((DEBUG_INFO, "AmiPcdSystemVersion:SizeofBuffer > PcdSize so truncating the string \n"));
                                SizeofBuffer = PcdSize - 2;//To avoid overwriting last byte of PCD data(Null terminator)
                            }
                            DEBUG ((DEBUG_INFO, "AmiPcdSystemVersion: StrPtr: %S \n", StrPtr));
                            PcdSetPtrS (
                                AmiPcdSystemVersion,
                                &SizeofBuffer,
                                (VOID*)StrPtr );
                            DEBUG ((DEBUG_INFO, "AmiPcdSystemVersion: %S\n", (CHAR16 *) PcdGetPtr (AmiPcdSystemVersion)));
                            FreePool (StrPtr);
                        }

                        // Product Serial Number.
                        Offset = Offset + Length + 1;
                        Length = FRU_DATA_LENGTH_MASK & TempPtr[Offset];
                        Status = GetFruData (
                                     &TempPtr[Offset],
                                     &StrPtr );
                        
                        if (!EFI_ERROR (Status)) {
                            SizeofBuffer = StrnSizeS (StrPtr, MAX_STRING_SIZE);
                            PcdSize = (UINT8)PcdGetSize(AmiPcdSystemSerialNumber);
                            if (SizeofBuffer > PcdSize) {
                                DEBUG ((DEBUG_INFO, "AmiPcdSystemSerialNumber:SizeofBuffer > PcdSize so truncating the string \n"));
                                SizeofBuffer = PcdSize - 2;//To avoid overwriting last byte of PCD data(Null terminator)
                            }
                            DEBUG ((DEBUG_INFO, "AmiPcdSystemSerialNumber: StrPtr: %S\n", StrPtr));
                            PcdSetPtrS (
                                AmiPcdSystemSerialNumber,
                                &SizeofBuffer,
                                (VOID*)StrPtr );
                            DEBUG ((DEBUG_INFO, "AmiPcdSystemSerialNumber: %S\n", (CHAR16 *) PcdGetPtr (AmiPcdSystemSerialNumber)));  
                            FreePool (StrPtr);
                        }

                        // Product Asset Tag.
                        Offset = Offset + Length + 1;
                        Length = FRU_DATA_LENGTH_MASK & TempPtr[Offset];
                        Status = GetFruData (
                                     &TempPtr[Offset],
                                     &AssetTagString );
                        if (EFI_ERROR (Status)) {
                            DEBUG ((DEBUG_ERROR, "GetFRUData :AssetTagString: Status %r\n",Status));
                        }
                    }
                }
                FreePool (TempPtr);
            }
        }
    }

    // Prepare Base Board data (SMBIOS Type 2)
    Offset = FruCommonHeader.BoardAreaStartingOffset * 8;
    if (Offset > 0)  {
        Length = 0;
        // Size of the length of FRU area to read.
        GetFruRedirDataSize = sizeof (UINT8);
        Status = This->GetFruRedirData (
                                This,
                                FruRecIndex,
                                (Offset + 1),
                                &GetFruRedirDataSize,
                                (UINT8*) &Length );

        if (!EFI_ERROR(Status) && (Length > 0)) {
            Length = Length * 8;
            TempPtr = (UINT8*) AllocateRuntimeZeroPool (Length);

            if (TempPtr != NULL) {
                Status = This->GetFruRedirData  (
                                        This,
                                        FruRecIndex,
                                        Offset,
                                        &Length,
                                        TempPtr );

                if (!EFI_ERROR (Status)) {
                    /* Validating Board Info Area Checksum before reading the Board Info
                       Area Data.*/
                    if (CalculateCheckSum8 (
                            TempPtr,
                            Length ) == 0) {
                        // Fill the AssertTag.
                        if (AssetTagString != NULL) {
                            SizeofBuffer = StrnSizeS (AssetTagString, MAX_STRING_SIZE);
                            PcdSize = (UINT8)PcdGetSize(AmiPcdBaseBoardAssetTag);
                            if (SizeofBuffer > PcdSize) {
                                DEBUG ((DEBUG_INFO, " AmiPcdBaseBoardAssetTag:SizeofBuffer > PcdSize so truncating the string \n"));
                                SizeofBuffer = PcdSize - 2;//To avoid overwriting last byte of PCD data(Null terminator)
                            }
                            DEBUG ((DEBUG_INFO, "AssetTagString: StrPtr: %S\n", StrPtr));
                            PcdSetPtrS (
                                AmiPcdBaseBoardAssetTag,
                                &SizeofBuffer,
                                (VOID*)AssetTagString );
                                DEBUG ((DEBUG_INFO, "AmiPcdBaseBoardAssetTag: %S\n", (CHAR16 *) PcdGetPtr (AmiPcdBaseBoardAssetTag)));
                        }

                        // Board Manufacturer.
                        Offset = BOARD_MFG_OFFSET;
                        Length = FRU_DATA_LENGTH_MASK & TempPtr[Offset];
                        Status = GetFruData (
                                     &TempPtr[Offset],
                                     &StrPtr );

                        if (!EFI_ERROR (Status)) {
                            SizeofBuffer = StrnSizeS (StrPtr, MAX_STRING_SIZE);
                            PcdSize = (UINT8)PcdGetSize(AmiPcdBaseBoardManufacturer);
                            if (SizeofBuffer > PcdSize) {
                                DEBUG ((DEBUG_INFO, "AmiPcdBaseBoardManufacturer:SizeofBuffer > PcdSize so truncating the string \n"));
                                SizeofBuffer = PcdSize - 2;//To avoid overwriting last byte of PCD data(Null terminator)
                            }
                            DEBUG ((DEBUG_INFO, "AmiPcdBaseBoardManufacturer: StrPtr: %S\n", StrPtr));
                            PcdSetPtrS (
                                AmiPcdBaseBoardManufacturer,
                                &SizeofBuffer,
                                (VOID*)StrPtr );
                            DEBUG ((DEBUG_INFO, "AmiPcdBaseBoardManufacturer: %S\n", (CHAR16 *) PcdGetPtr (AmiPcdBaseBoardManufacturer)));
                            FreePool (StrPtr);
                        }

                        // Board Product Name.
                        Offset = Offset + Length + 1;
                        Length = FRU_DATA_LENGTH_MASK & TempPtr[Offset];
                        Status = GetFruData (
                                    &TempPtr[Offset],
                                    &StrPtr );

                        if (!EFI_ERROR (Status)) {
                            SizeofBuffer = StrnSizeS (StrPtr, MAX_STRING_SIZE);
                            PcdSize = (UINT8)PcdGetSize(AmiPcdBaseBoardProductName);
                            if (SizeofBuffer > PcdSize) {
                                DEBUG ((DEBUG_INFO, "AmiPcdBaseBoardProductName:SizeofBuffer > PcdSize so truncating the string \n"));
                                SizeofBuffer = PcdSize - 2;//To avoid overwriting last byte of PCD data(Null terminator)
                            }
                            DEBUG ((DEBUG_INFO, "AmiPcdBaseBoardProductName: StrPtr: %S\n", StrPtr));
                            PcdSetPtrS (
                                AmiPcdBaseBoardProductName,
                                &SizeofBuffer,
                                (VOID*)StrPtr );
                           DEBUG ((DEBUG_INFO, "AmiPcdBaseBoardProductName: %S\n", (CHAR16 *) PcdGetPtr (AmiPcdBaseBoardProductName)));
                           FreePool (StrPtr);
                        }

                        // Board Serial Number.
                        Offset = Offset + Length + 1;
                        Length = FRU_DATA_LENGTH_MASK & TempPtr[Offset];
                        Status = GetFruData (
                                    &TempPtr[Offset],
                                    &StrPtr );

                        if (!EFI_ERROR (Status)) {
                            SizeofBuffer = StrnSizeS(StrPtr, MAX_STRING_SIZE);
                            PcdSize = (UINT8)PcdGetSize(AmiPcdBaseBoardSerialNumber);
                            if (SizeofBuffer > PcdSize) {
                                DEBUG ((DEBUG_INFO, "AmiPcdBaseBoardSerialNumber:SizeofBuffer > PcdSize so truncating the string\n"));
                                SizeofBuffer = PcdSize - 2;//To avoid overwriting last byte of PCD data(Null terminator)
                            }
                            DEBUG ((DEBUG_INFO, "AmiPcdBaseBoardSerialNumber: StrPtr: %S\n", StrPtr));
                            PcdSetPtrS (
                                AmiPcdBaseBoardSerialNumber,
                                &SizeofBuffer,
                                (VOID*)StrPtr );
                            DEBUG ((DEBUG_INFO, "AmiPcdBaseBoardSerialNumber: %S\n", (CHAR16 *) PcdGetPtr (AmiPcdBaseBoardSerialNumber)));
                            FreePool (StrPtr);
                        }

                        // Board Version Number.
                        Offset = Offset + Length + 1;
                        Length = FRU_DATA_LENGTH_MASK & TempPtr[Offset];
                        Status = GetFruData (
                                     &TempPtr[Offset],
                                     &StrPtr );

                        if (!EFI_ERROR (Status)) {
                            SizeofBuffer = StrnSizeS (StrPtr, MAX_STRING_SIZE);
                            PcdSize = (UINT8)PcdGetSize(AmiPcdBaseBoardPartNumber);
                            if (SizeofBuffer > PcdSize) {
                                DEBUG ((DEBUG_INFO, "AmiPcdBaseBoardPartNumber:SizeofBuffer > PcdSize so truncating the string \n"));
                                SizeofBuffer = PcdSize - 2;//To avoid overwriting last byte of PCD data(Null terminator)
                            }
                            DEBUG ((DEBUG_INFO, "AmiPcdBaseBoardPartNumber: StrPtr: %S\n", StrPtr));
                            PcdSetPtrS (
                                    AmiPcdBaseBoardPartNumber,
                                &SizeofBuffer,
                                (VOID*)StrPtr );
                            DEBUG ((DEBUG_INFO, "AmiPcdBaseBoardPartNumber: %S\n", (CHAR16 *) PcdGetPtr (AmiPcdBaseBoardPartNumber)));
                            FreePool (StrPtr);
                        }
                    }
                }
                FreePool (TempPtr);
            }
        }
    }

    // Prepare Chassis data (SMBIOS Type 3)
    Offset = FruCommonHeader.ChassisInfoStartingOffset * 8;
    if (Offset > 0) {
        Length = 0;
        // Size of the length of FRU area to read.
        GetFruRedirDataSize = sizeof (UINT8);
        This->GetFruRedirData (
                        This,
                        FruRecIndex,
                        (Offset + 1),
                        &GetFruRedirDataSize,
                        (UINT8*) &Length );

        if (!EFI_ERROR(Status) && (Length > 0)) {
            Length = Length * 8;
            TempPtr = (UINT8*) AllocateRuntimeZeroPool (Length);

            if (TempPtr != NULL) {
                Status = This->GetFruRedirData (
                                        This,
                                        FruRecIndex,
                                        Offset,
                                        &Length,
                                        TempPtr );

                if (!EFI_ERROR (Status)) {
                    /* Validating Chasis Info Area Checksum before reading the Chassis Info
                       Area Data.*/
                    if (CalculateCheckSum8 (
                            TempPtr,
                            Length ) == 0) {
                        // Update Chassis type and ChassisLockPresent bit.
                        Status = PcdSet8S (
                                     AmiPcdChassisType,
                                     TempPtr[CHASSIS_TYPE_OFFSET] | BIT7);
                        ASSERT_EFI_ERROR(Status);
                        DEBUG ((DEBUG_INFO, "AmiPcdChassisType: %x\n", PcdGet8 (AmiPcdChassisType) ));

                        // Chassis Version.
                        Offset = CHASSIS_PART_NUMBER;
                        Length = FRU_DATA_LENGTH_MASK & TempPtr[Offset];
                        Status = GetFruData (
                                     &TempPtr[Offset],
                                     &StrPtr );

                        if (!EFI_ERROR (Status)) {
                            SizeofBuffer = StrnSizeS(StrPtr, MAX_STRING_SIZE);
                            PcdSize = (UINT8)PcdGetSize(AmiPcdChassisPartNumber);
                            if (SizeofBuffer > PcdSize) {
                                DEBUG ((DEBUG_INFO, "AmiPcdChassisVersion:SizeofBuffer > PcdSize so truncating the string \n"));
                                SizeofBuffer = PcdSize - 2;//To avoid overwriting last byte of PCD data(Null terminator)
                            }
                            DEBUG ((DEBUG_INFO, "AmiPcdChassisPartNumber: StrPtr: %S\n", StrPtr));
                            PcdSetPtrS (
                                AmiPcdChassisPartNumber,
                                &SizeofBuffer,
                                (VOID*)StrPtr );
                            DEBUG ((DEBUG_INFO, "AmiPcdChassisPartNumber: %S\n", (CHAR16 *) PcdGetPtr (AmiPcdChassisPartNumber)));
                            FreePool (StrPtr);
                        }

                        // Chassis Serial Number.
                        Offset = Offset + Length + 1;
                        Length = FRU_DATA_LENGTH_MASK & TempPtr[Offset];
                        Status = GetFruData (
                                     &TempPtr[Offset],
                                     &StrPtr );

                        if (!EFI_ERROR (Status)) {
                            SizeofBuffer = StrnSizeS (StrPtr, MAX_STRING_SIZE);
                            PcdSize = (UINT8)PcdGetSize(AmiPcdChassisSerialNumber);
                            if (SizeofBuffer > PcdSize) {
                                DEBUG ((DEBUG_INFO, "AmiPcdChassisSerialNumber:SizeofBuffer > PcdSize so truncating the string \n"));
                                SizeofBuffer = PcdSize - 2;//To avoid overwriting last byte of PCD data(Null terminator)
                            }
                            DEBUG ((DEBUG_INFO, "AmiPcdChassisSerialNumber: StrPtr: %S\n", StrPtr));
                            PcdSetPtrS (
                                AmiPcdChassisSerialNumber,
                                &SizeofBuffer,
                                (VOID*)StrPtr );
                            DEBUG ((DEBUG_INFO, "AmiPcdChassisSerialNumber: %S\n", (CHAR16 *) PcdGetPtr (AmiPcdChassisSerialNumber)));
                            FreePool (StrPtr);
                        }

                        /* Data already received at product entry and fill
                           the Manufacturer and asset tag.*/
                        if (ManufacturerString != NULL) {
                            SizeofBuffer = StrnSizeS (ManufacturerString, MAX_STRING_SIZE);
                            PcdSize = (UINT8)PcdGetSize(AmiPcdChassisManufacturer);
                            if (SizeofBuffer > PcdSize) {
                                DEBUG ((DEBUG_INFO, "AmiPcdChassisManufacturer:SizeofBuffer > PcdSize so truncating the string \n"));
                                SizeofBuffer = PcdSize - 2;//To avoid overwriting last byte of PCD data(Null terminator)
                            }
                            DEBUG ((DEBUG_INFO, "AmiPcdChassisManufacturer: ManufacturerString: %S\n", ManufacturerString));
                            PcdSetPtrS (
                                AmiPcdChassisManufacturer,
                                &SizeofBuffer,
                                (VOID*)ManufacturerString );
                            DEBUG ((DEBUG_INFO, "AmiPcdChassisManufacturer: %S\n", (CHAR16 *) PcdGetPtr (AmiPcdChassisManufacturer)));
                        }

                        if (AssetTagString != NULL) {
                            SizeofBuffer = StrnSizeS (AssetTagString, MAX_STRING_SIZE);
                            PcdSize = (UINT8)PcdGetSize(AmiPcdChassisAssetTag);
                            if (SizeofBuffer > PcdSize) {
                                DEBUG ((DEBUG_INFO, "AmiPcdChassisAssetTag:SizeofBuffer > PcdSize so truncating the string \n"));
                                SizeofBuffer = PcdSize - 2;//To avoid overwriting last byte of PCD data(Null terminator)
                            }
                            DEBUG ((DEBUG_INFO, "AmiPcdChassisAssetTag: AssetTagString: %S\n", AssetTagString));
                            PcdSetPtrS (
                                AmiPcdChassisAssetTag,
                                &SizeofBuffer,
                                (VOID*)AssetTagString );
                            DEBUG ((DEBUG_INFO, "AmiPcdChassisAssetTag: %S\n", (CHAR16 *) PcdGetPtr (AmiPcdChassisAssetTag)));
                        }
                    }
                }
                FreePool (TempPtr);
            }
        }
    }

    // Free the memory allocated.
    if (ManufacturerString != NULL) {
        FreePool (ManufacturerString);
    }
    if (AssetTagString != NULL) {
        FreePool (AssetTagString);
    }

    // Prepare System UUID data (SMBIOS Type 1).
    Offset = FruCommonHeader.MultiRecInfoStartingOffset * 8;
    if (Offset > 0) {
        DEBUG ((DEBUG_INFO, "Multirecord Info Area present\n"));

        do {
            GetFruRedirDataSize = sizeof(AMI_IPMI_FRU_MULTI_RECORD_HEADER);

            // Get the Multi Record Header
            Status = This->GetFruRedirData (
                                    This,
                                    FruRecIndex,
                                    Offset,
                                    &GetFruRedirDataSize,
                                    (UINT8*) &MultiRecordHeader );
            if (!EFI_ERROR(Status)) {

                // Calculate Record Header Checksum.
                if (CalculateCheckSum8 (
                        (UINT8 *)&MultiRecordHeader,
                        sizeof (AMI_IPMI_FRU_MULTI_RECORD_HEADER) ) != 0) {
                    DEBUG((DEBUG_ERROR, "FRU MultiRecord Record invalid.\n"));
                    break;
                }

                // Check Record ID.
                if ((MultiRecordHeader.RecordTypeId != MANAGEMENT_ACCESS_RECORD) && (!(MultiRecordHeader.EndofList)) ) {
                    // If not Management Access Record, move to next record.
                    Offset += sizeof(AMI_IPMI_FRU_MULTI_RECORD_HEADER) + MultiRecordHeader.RecordLength;
                    continue;
                } else { 
                    // Management Access Record found.
                    if (MultiRecordHeader.RecordTypeId == MANAGEMENT_ACCESS_RECORD) {
                        /* If Management Access Record is found, then check System UUID
                           sub record type.*/
                        DEBUG ((DEBUG_INFO, "Management Access record is found\n"));

                        // Size of the length of FRU area to read.
                        GetFruRedirDataSize = sizeof (UINT8);
                        Status = This->GetFruRedirData (
                                                This,
                                                FruRecIndex,
                                                (Offset + sizeof(AMI_IPMI_FRU_MULTI_RECORD_HEADER)),
                                                &GetFruRedirDataSize,
                                                (UINT8*) &SubRecordType );

                        if (!(EFI_ERROR(Status))) {

                            if (SubRecordType == SYSTEM_UUID_SUB_RECORD_TYPE) {
                                // Calculate record checksum.
                                SystemUuidSubRecordwithChecksum.RecordChecksum = MultiRecordHeader.RecordChecksum; // Copying record CheckSum
                                GetFruRedirDataSize = sizeof(SystemUuidSubRecordwithChecksum)- RECORD_CHECKSUM_SIZE;
                                Status = This->GetFruRedirData (
                                                            This,
                                                            FruRecIndex,
                                                            (Offset + sizeof(AMI_IPMI_FRU_MULTI_RECORD_HEADER)),
                                                            &GetFruRedirDataSize,
                                                            (UINT8*) &(SystemUuidSubRecordwithChecksum.SubRecordId) );
                                if (!EFI_ERROR(Status)) {
                                    if ((CalculateCheckSum8 (
                                        (UINT8 *)&SystemUuidSubRecordwithChecksum,
                                        sizeof(SystemUuidSubRecordwithChecksum) )) == 0) {
                                         /* System UUID sub record type is found, if checksum
                                            is zero.*/
                                         DEBUG ((DEBUG_INFO, "System UUID Sub Record Type found and Checksum OK!!!\n"));
                                         IsSystemUuidSubRecordType = TRUE;
                                    }
                                }
                                break;
                            } else if (!(MultiRecordHeader.EndofList)) {
                                /* If System UUID sub record type is not found,
                                   move to next record.*/
                                Offset += sizeof(AMI_IPMI_FRU_MULTI_RECORD_HEADER) + MultiRecordHeader.RecordLength;
                                continue;
                              } else {
                                  // End of list is found, so no more records.
                                  break;
                                }
                        } else if (!(MultiRecordHeader.EndofList)) {
                            // Still records are available, so move to next record.
                            Offset += sizeof(AMI_IPMI_FRU_MULTI_RECORD_HEADER) + MultiRecordHeader.RecordLength;
                            continue;
                          } else { // No more records.
                              break;
                            }
                    } else { // End of entire MultiRecord.
                        break;
                      }
                }
            } // if (!EFI_ERROR(Status))
        } while(!EFI_ERROR(Status));

        if (IsSystemUuidSubRecordType) {

            // Copy the UUID in Pcd.
            SizeofBuffer = sizeof (SystemUuidSubRecordwithChecksum.Uuid);
            PcdSize = (UINT8)PcdGetSize(AmiPcdSystemUuid);
            if (SizeofBuffer > PcdSize) {
                DEBUG ((DEBUG_INFO, "AmiPcdSystemUuid:SizeofBuffer > PcdSize so truncating the string \n"));
                SizeofBuffer = PcdSize - 2;//To avoid overwriting last byte of PCD data(Null terminator)
            }
            DEBUG ((DEBUG_INFO, "UUID Size: %x\n", SizeofBuffer));
            //System UUID will be updated if BMC supports Get System GUID command
            Uuid = (EFI_GUID *)PcdGetPtr (AmiPcdSystemUuid);
            ZeroMem (&DefaultUuid, sizeof(EFI_GUID) );
            if (CompareGuid ((CONST EFI_GUID*)Uuid, (CONST EFI_GUID*)&DefaultUuid)== TRUE) {
                PcdSetPtrS (AmiPcdSystemUuid, &SizeofBuffer, (VOID*)&(SystemUuidSubRecordwithChecksum.Uuid));
            }
        }
    }
    DEBUG ((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));
}


