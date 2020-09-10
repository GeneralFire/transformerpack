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

/** @file FruSetupScreen.c
    Initializes FRU information to display in server management setup screen.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <Setup.h>
#include <AmiServerMgmtSetupVariable.h>
#include <ServerMgmtSetupStrDefs.h>
#include <Library/PcdLib.h>
#include <Library/BaseLib.h>
#include <Library/PrintLib.h>
#include <Library/HiiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

typedef UINT16              STRING_REF;
#define STRING_BUFFER_LENGTH 100
#define MAX_STRING_SIZE      0xFF

//---------------------------------------------------------------------------

/**
    This function initializes the FRU information related setup option values

    @param[in] HiiHandle    Handle to HII database.
    @param[in] Class        Indicates the setup class.

    @return VOID    Nothing.

**/
VOID
InitFruStrings (
  IN EFI_HII_HANDLE     HiiHandle,
  IN UINT16             Class )
{

    CHAR16          *DefaultString = L"To be filled by O.E.M.                                            ";
    CHAR16          *SdrDefaultString = L"Unknown";
    EFI_GUID        DefaultSystemUuid = {0x00};
    CHAR16          *UpdateStr;
    EFI_GUID        *Uuid;
    EFI_STRING     CharPtrString;
    DEBUG ((DEBUG_INFO, "%a: Class ID:  %x\n", __FUNCTION__, Class));

    // Continue only for Server Mgmt setup page.
    if (Class != SERVER_MGMT_CLASS_ID) {
        return;
    }

    // System Manufacturer.
    UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdSystemManufacturer);
    if ((StrnCmp (
            UpdateStr,
            DefaultString,
            MAX_STRING_SIZE ) == 0x00) || \
         (StrnCmp (
             UpdateStr,
             L"",
             MAX_STRING_SIZE ) == 0x00)) {
        UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    }
    DEBUG ((DEBUG_INFO, "SystemManufacturer: UpdateStr: %S\n", UpdateStr));
    HiiSetString (
        HiiHandle,
        STR_SYS_MANUFACTURER_VAL,
        UpdateStr,
        NULL );

    // System Product Name.
    UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdSystemProductName);
    if ((StrnCmp (
            UpdateStr,
            DefaultString,
            MAX_STRING_SIZE ) == 0x00) || \
        (StrnCmp (
            UpdateStr,
            L"",
            MAX_STRING_SIZE ) == 0x00)) {
        UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    }
    DEBUG ((DEBUG_INFO, "SystemProductName: UpdateStr: %S\n", UpdateStr));
    HiiSetString (
        HiiHandle,
        STR_SYS_PROD_NAME_VAL,
        UpdateStr,
        NULL );

    // System Version.
    UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdSystemVersion);
    DEBUG ((DEBUG_INFO, "SystemVersion: UpdateStr: %S\n", UpdateStr));
    if ((StrnCmp (
            UpdateStr,
            DefaultString,
            MAX_STRING_SIZE ) == 0x00) || \
        (StrnCmp (
            UpdateStr,
            L"",
            MAX_STRING_SIZE ) == 0x00)) {
        UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    }
    HiiSetString (
        HiiHandle,
        STR_SYS_VERSION_VAL,
        UpdateStr,
        NULL );

    // System Serial Number.
    UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdSystemSerialNumber);
    DEBUG ((DEBUG_INFO, "SystemSerialNumber: UpdateStr: %S\n", UpdateStr));
    if ((StrnCmp (
            UpdateStr,
            DefaultString,
            MAX_STRING_SIZE ) == 0x00) || \
        (StrnCmp (
            UpdateStr,
            L"",
            MAX_STRING_SIZE ) == 0x00)) {
        UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    }
    HiiSetString (
        HiiHandle,
        STR_SYS_SERIAL_NUM_VAL,
        UpdateStr,
        NULL );
    // Baseboard Manufacturer.
    UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdBaseBoardManufacturer);
    DEBUG ((DEBUG_INFO, "BaseBoardManufacturer: UpdateStr: %S\n", UpdateStr));
    if ((StrnCmp (
            UpdateStr,
            DefaultString,
            MAX_STRING_SIZE ) == 0x00) || \
         (StrnCmp (
             UpdateStr,
             L"",
             MAX_STRING_SIZE ) == 0x00)) {
        UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    }
    HiiSetString (
        HiiHandle,
        STR_BRD_MANUFACTURER_VAL,
        UpdateStr,
        NULL );
    // Baseboard Product Name.
    UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdBaseBoardProductName);
    DEBUG ((DEBUG_INFO, "BaseBoardProductName: UpdateStr: %S\n", UpdateStr));
    if ( (StrnCmp (
            UpdateStr,
            DefaultString,
            MAX_STRING_SIZE ) == 0x00) || \
         (StrnCmp (
             UpdateStr,
             L"",
             MAX_STRING_SIZE ) == 0x00)) {
        UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    }
    HiiSetString (
        HiiHandle,
        STR_BRD_PROD_NAME_VAL,
        UpdateStr,
        NULL );

    // Baseboard Version.
    UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdBaseBoardVersion);
    DEBUG ((DEBUG_INFO, "BaseBoardVersion: UpdateStr: %S\n", UpdateStr));
    if ((StrnCmp (
            UpdateStr,
            DefaultString,
            MAX_STRING_SIZE ) == 0x00) || \
        (StrnCmp (
             UpdateStr,
             L"",
             MAX_STRING_SIZE ) == 0x00)) {
        UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    }
    HiiSetString (
        HiiHandle,
        STR_BRD_VERSION_VAL,
        UpdateStr,
        NULL );

    // Baseboard Serial Number.
    UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdBaseBoardSerialNumber);
    DEBUG ((DEBUG_INFO, "BaseBoardSerialNumber: UpdateStr: %S\n", UpdateStr));
    if ((StrnCmp (
            UpdateStr,
            DefaultString,
            MAX_STRING_SIZE ) == 0x00) || \
        (StrnCmp (
            UpdateStr,
            L"",
            MAX_STRING_SIZE ) == 0x00)) {
        UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    }
    HiiSetString (
        HiiHandle,
        STR_BRD_SERIAL_NUM_VAL,
        UpdateStr,
        NULL );
    // Chassis Manufacturer.
    UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdChassisManufacturer);
    if ((StrnCmp (UpdateStr, DefaultString, MAX_STRING_SIZE) == 0x00) || (StrnCmp (UpdateStr, L"",MAX_STRING_SIZE) == 0x00)) {
        UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    }
    DEBUG ((DEBUG_INFO, "ChassisManufacturer: UpdateStr: %S\n", UpdateStr));
    HiiSetString (
        HiiHandle,
        STR_CHA_MANUFACTURER_VAL,
        UpdateStr,
        NULL );

    // Chassis Version.
    UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdChassisVersion);
    DEBUG ((DEBUG_INFO, "ChassisVersion: UpdateStr: %S\n", UpdateStr));
    if ((StrnCmp (
            UpdateStr,
            DefaultString,
            MAX_STRING_SIZE ) == 0x00) || \
        (StrnCmp (
            UpdateStr,
            L"",
            MAX_STRING_SIZE ) == 0x00)) {
        UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    }
    HiiSetString (
        HiiHandle,
        STR_CHA_VERSION_VAL,
        UpdateStr,
        NULL);

    // Chassis Serial Number.
    UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdChassisSerialNumber);
    DEBUG ((DEBUG_INFO, "ChassisSerialNumber: UpdateStr: %S\n", UpdateStr));
    if ((StrnCmp (
            UpdateStr,
            DefaultString,
            MAX_STRING_SIZE ) == 0x00) || \
        (StrnCmp (
            UpdateStr,
            L"",
            MAX_STRING_SIZE ) == 0x00)) {
        UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    }
    HiiSetString (
        HiiHandle,
        STR_CHA_SERIAL_NUM_VAL,
        UpdateStr,
        NULL );
    // Sdr version number.
    UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdSdrVersionNo);
    DEBUG ((DEBUG_INFO, "SdrVersionNo: UpdateStr: %S\n", UpdateStr));
    if ((StrnCmp (
            UpdateStr,
            SdrDefaultString,
            MAX_STRING_SIZE ) == 0x00) || \
        (StrnCmp (
            UpdateStr,
            L"",
            MAX_STRING_SIZE ) == 0x00)) {
        UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
    }
    HiiSetString (
        HiiHandle,
        STR_SDR_VERSION_VAL,
        UpdateStr,
        NULL );

    // System Unique ID.
    Uuid = (EFI_GUID *) PcdGetPtr (AmiPcdSystemUuid);
    if (CompareGuid (
            (CONST EFI_GUID*)Uuid,
            (CONST EFI_GUID*)&DefaultSystemUuid ) == FALSE) {
        CharPtrString = AllocateZeroPool (STRING_BUFFER_LENGTH);
        if (CharPtrString != NULL){
            DEBUG ((DEBUG_INFO, "UUID:%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x\n", Uuid->Data1, Uuid->Data2, Uuid->Data3, Uuid->Data4[0], Uuid->Data4[1], Uuid->Data4[2], Uuid->Data4[3], Uuid->Data4[4], Uuid->Data4[5], Uuid->Data4[6], Uuid->Data4[7]));
            UnicodeSPrint(
                CharPtrString,
                STRING_BUFFER_LENGTH,
                L"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                Uuid->Data1,
                Uuid->Data2, 
                Uuid->Data3,
                Uuid->Data4[0],
                Uuid->Data4[1],
                Uuid->Data4[2],
                Uuid->Data4[3],
                Uuid->Data4[4],
                Uuid->Data4[5],
                Uuid->Data4[6],
                Uuid->Data4[7] );

            HiiSetString (
                HiiHandle,
                STR_SYSTEM_UUID_VAL,
                CharPtrString,
                NULL );
            gBS->FreePool (CharPtrString);
        }
    } else {
        UpdateStr = (CHAR16 *) PcdGetPtr (AmiPcdStrDefault);
        HiiSetString (
            HiiHandle,
            STR_SYSTEM_UUID_VAL,
            UpdateStr,
            NULL );
    } 

    DEBUG ((DEBUG_INFO, "%a Exiting...\n", __FUNCTION__));
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
