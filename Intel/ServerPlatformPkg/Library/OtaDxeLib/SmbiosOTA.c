/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2018 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary    and
  confidential information of Intel Corporation and its suppliers and licensors,
  and is protected by worldwide copyright and trade secret laws and treaty
  provisions. No part of the Material may be used, copied, reproduced, modified,
  published, uploaded, posted, transmitted, distributed, or disclosed in any way
  without Intel's prior express written permission.

  No license under any patent, copyright, trade secret or other intellectual
  property right is granted to or conferred upon you by disclosure or delivery
  of the Materials, either expressly, by implication, inducement, estoppel or
  otherwise. Any license under such intellectual property rights must be
  express and approved by Intel in writing.

  Unless otherwise agreed by Intel in writing, you may not remove or alter
  this notice or any other notice embedded in Materials by Intel or
  Intel's suppliers or licensors in any way.
**/

#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/HiiLib.h>
#include <Library/HobLib.h>

#include <IndustryStandard/SmBios.h>
#include <Protocol/Smbios.h>

#include <DxeOneTouchPlatform.h>
#include <DxeOobOneTouch.h>
#include <SmbiosOTA.h>

//--------------------------------------------------------------------------
//
CHAR16  GroupStructureString[] = L"Group: One Touch Activation (OTA)";  // String for Group Association Structure
CHAR16  OemStructureString[]   = L"Member: OTA General Information";    // String for OEM Structure
//
//--------------------------------------------------------------------------
//************************** LOCAL FUNCTIONS *******************************
//--------------------------------------------------------------------------
/**
  Unicode2AsciizString(): Converts given UnicodeZ string to ASCIIZ string in
  the given area and returns the length of ASCIIZ string (inclduing NULL).

  @param   Unicode      INPUT: CHAR16*, ptr to UnicodeZ string
  @param   Asciiz       INPUT/OUTPUT: UINT8*, ptr to ASCIIZ string
  @param   AsciizLength INPUT/OUTPUT: UINTN*, ptr to area where ASCIIZ string length (including NULL) is returned

  @retval  EFI_STATUS   EFI_SUCCESS  Success
                        Other        Error

**/
//--------------------------------------------------------------------------
EFI_STATUS
Unicode2AsciizString (
  CHAR16      *Unicode,
  UINT8       *Asciiz,
  UINTN       *AsciizLength
  )
{
  // Local data
  EFI_STATUS s;
  UINTN      l;
  CHAR16     *puz;
  UINT16     u;
  UINT8      *paz;

  DEBUG ((EFI_D_ERROR, "[OOB-SMBIOS] Unicode2AsciizString()  { -> Convert Unicode string to ASCIIZ String\n"));

  // Scan Unicode string and form ASCIIZ string
  paz = Asciiz; puz = Unicode;                             // Ptr to ASCIIZ string, Ptr to Unicode string
  l = 0; s = EFI_SUCCESS;                                  // Length of string in characters including NULL, Assume success

  for (;;) {                                               // Scan Unicode string
    u = *puz;                                              //   Get next Unicode Char

    if ((l > MAX_ASCIIZ_STRING_LENGTH_B) ||                //   If max string length exceeds OR
        ((u & 0xFF00) != 0)) {                             //      invalid Character
      s = EFI_DEVICE_ERROR; break;                         //     Error, Stop
    }

    *paz = (UINT8) (u & 0x00FF);                           //   Put in ASCIIZ string
    paz++; puz++; l++;                                     //   Update ptr to ASCIIZ, Unicode, #of characters in string

    if (u == 0x0000) { break; }                            //   If end reached, Stop
  }

  *AsciizLength = l;                                       // ASCIIZ string length (including NULL)

  DEBUG ((EFI_D_ERROR, "[OOB-SMBIOS] Unicode2AsciizString()  } -> "));
  if (s == EFI_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success, ASCIIZ String length (including NULL) = 0x%04x\n", l)); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return s;                                                // Status
}

//--------------------------------------------------------------------------
/**
  AddStringAndAdd2Smbios(): Converts the given Unicode string to ASCIIZ string,
  adds ASCIIZ string to the given SMBIOS structure, and adds the formed
  structure to other SMBIOS Structures.

  @param   Smbios          INPUT: EFI_SMBIOS_PROTOCOL*, Ptr to SMBIOS Protocol
  @param   Handle          INPUT/OUTPUT: EFI_SMBIOS_HANDLE*, Ptr to area where unique handle assigned to SMBIOS structure is returned
  @param   Data            INPUT: EFI_SMBIOS_TABLE_HEADER*, Ptr to given SMBIOS Structure
  @param   FormatedLength  INPUT: UINTN, Length of formatted area of given SMBIOS Structure
  @param   Unicode         INPUT: CHAR16*, Ptr to Unicode string to be converted to ASCIIZ string

  @retval  EFI_STATUS      EFI_SUCCESS  Success
                           Other        Error

**/
//--------------------------------------------------------------------------
EFI_STATUS
AddStringAndAdd2Smbios (
  IN     EFI_SMBIOS_PROTOCOL     *Smbios,
  IN OUT EFI_SMBIOS_HANDLE       *Handle,
  IN     EFI_SMBIOS_TABLE_HEADER *Data,
  IN     UINTN                   FormattedLength,
  IN     CHAR16                  *Unicode
  )
{
  // Local data
  EFI_STATUS  s;
  UINTN       StringLength;

  DEBUG ((EFI_D_ERROR, "[OOB-SMBIOS] AddStringAndAdd2Smbios()  { -> Add String and Add Structure to SMBIOS\n"));

  // String: Data = Ptr to SMBIOS Structure, Formattedlength = formatted area length
  s = Unicode2AsciizString (                                         // Convert to ASCIIZ String
             Unicode,                                                //   Ptr to Unicode String
             (UINT8*) ((UINTN) Data + FormattedLength),              //   Ptr to ASCIIZ String
             (UINTN*) &StringLength);                                //   Ptr to ASCIIZ String Length (including NULL)

  if (s == EFI_SUCCESS) {                                            //   If success
    *((UINT8*) ((UINTN) Data + FormattedLength + StringLength)) = 0; //   NULL Terminator of structure

    // Add formed structure to other SMBIOS structures: Data = Ptr to formed structure
    *Handle = SMBIOS_HANDLE_PI_RESERVED;                             //   Assign unique handle automatically
    DEBUG ((EFI_D_ERROR, "    Add Formed Structure to other SMBIOS structures, Handle before adding 0x%04X\n", *Handle));

    s = Smbios->Add (Smbios, NULL, Handle, Data);                    //   Add SMBIOS Structure

    DEBUG ((EFI_D_ERROR, "    Structure addition to SMBIOS: EFI Status 0x%08X, Handle after adding 0x%04X -> ", s, *Handle));
    Data->Handle = *Handle;                                          //     Update Handle in local data for display purpose

    if (s == EFI_SUCCESS) {                                          //   If success
      DEBUG ((EFI_D_ERROR, "Success\n"));
    } else {
      DEBUG ((EFI_D_ERROR, "Error\n"));
    }

    // Display OEM Structure data: Data = Ptr to SMBIOS Structure, Formattedlength = formatted area length, StringLength = string length
    StringLength = FormattedLength + StringLength + 1;               //   Structure length in bytes
    DEBUG ((EFI_D_ERROR, "        ******** Formed SMBIOS Structure Data, Length 0x%02X\n", StringLength));
    DisplayBinaryData ((UINT8*) Data, (UINT32) StringLength);        //   Ptr to data, Data size in bytes
  }

  DEBUG ((EFI_D_ERROR, "[OOB-SMBIOS] AddStringAndAdd2Smbios()  } -> "));
  if (s == EFI_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success, Handle of added Structure = 0x%04x\n", *Handle)); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return s;                                                          // Status
}

//--------------------------------------------------------------------------
/**
  AddOemStructureOTA(): This function forms the data of OEM Structure for OTA
  and adds OEM Structure to SMBIOS structures.

  @param   Ott        INPUT: OOB_TXT_TPM*, Ptr to OOB_TXT_TPM
  @param   Opd        INPUT: OT_PLATFORM_DATA*, Ptr to OT_PLATFORM_DATA
  @param   Smbios     INPUT: EFI_SMBIOS_PROTOCOL*, Ptr to SMBIOS Protocol
  @param   Handle     INPUT/OUTPUT: EFI_SMBIOS_HANDLE*, Ptr to area where unique handle assigned to OEM structure is returned

  @retval  EFI_STATUS EFI_SUCCESS  Success
                      Other        Error
**/
EFI_STATUS
AddOemStructureOTA (
  IN     OOB_TXT_TPM          *Ott,
  IN     OT_PLATFORM_DATA     *Opd,
  IN     EFI_SMBIOS_PROTOCOL  *Smbios,
  IN OUT EFI_SMBIOS_HANDLE    *Handle
  )
{
  // Local data
  EFI_STATUS         s;
  OEM_STRUCTURE_OTA  *Data;
  UINTN              MaxSize, Length;

  DEBUG ((EFI_D_ERROR, "[OOB-SMBIOS] AddOemStructureOTA()  { -> Form and Add OEM Structure Type-%d (0x%02X)\n", OEM_STRUCTURE_TYPE_OTA, OEM_STRUCTURE_TYPE_OTA));

  s = EFI_SUCCESS;                                                   // Assume success

  // Allocate memory for holding Structure data
  DEBUG ((EFI_D_ERROR, "    Allocate memory for OEM Structure: "));
  Length = sizeof (OEM_STRUCTURE_OTA);                               // Formatted area Length of OEM Structure
  MaxSize = Length + (MAX_ASCIIZ_STRING_LENGTH_B + 2);               // Max Length of OEM Structure (one string with double NULL at end)
  Data = NULL;                                                       // Ptr to data
  Data = (OEM_STRUCTURE_OTA*)(UINTN) AllocateZeroPool (MaxSize);     // Allocate memory and init with 0
  if (Data != NULL) {                                                // If memory ptr valid
    DEBUG ((EFI_D_ERROR, "Success\n"));
  } else {                                                           // If memory ptr not valid
    s = EFI_DEVICE_ERROR;                                            //   Error
    DEBUG ((EFI_D_ERROR, "Error, Memory not available\n"));
  }

  if (Length > (sizeof (UINT8) * 255)) {                             // If Structure formatted length too big
    s = EFI_DEVICE_ERROR;                                            //   Error
    DEBUG ((EFI_D_ERROR, "    Length of formatted area of Structure > 255 -> Error\n"));
  }

  // Form OEM Structure: Data = Ptr to SMBIOS Structure, Length = formatted area Length
  if (s == EFI_SUCCESS) {                                            // If success
    DEBUG ((EFI_D_ERROR, "    Form OEM Structure\n"));
    Data->Type = OEM_STRUCTURE_TYPE_OTA;                             // Structure Type
    Data->Length = (UINT8) Length;                                   // Length of formatted area of structure
    Data->Handle = 0;                                                // Handle
    Data->MemberIdentifier = OEM_OTA_GENERAL_INFORMATION_BIT;        // Member identifier
    Data->MemberName = 0x01;                                         // String# of Member name
    Data->SpecificationVersion = Ott->Header.Version;                // OTA Specification Version#
    Data->ImplementationMajorVersion = Ott->Header.MajorVersionImp;  // Major Version# of OTA implementation
    Data->ImplementationMinorVersion = Ott->Header.MinorVersionImp;  // Minor Version# of OTA implementation
    Data->FeatureSupported = Ott->Header.FeatureSupported;           // Bit-mapped information of supported features
    Data->FeatureEnabled = Ott->Header.FeatureEnabled;               // Bit-mapped information of enabled features
    Data->TpmUsage = Ott->Header.TpmUsage;                           // Bit-mapped information of TPM usage

    FillOobSupportedTaskInfo (                                       // Bit-mapped information of Use-Case (Task) Supported
      (UINT8*)(UINTN) &Data->UseCaseSupported, sizeof (Data->UseCaseSupported));

    Data->MaxMkTmeKeys = Opd->MaxMkTmeKeys;                          // Max #of MK-TME Keys available for use
    Data->MaxMkTmeKeyIdBits = Opd->MaxMkTmeKeyIdBits;                // Max #of Bits allowed for use as Key Identifiers for MK-TME
    Data->MkTmeKeyIdBits = Opd->MkTmeKeyIdBits;                      // #of Bits allocated for use as Key Identifiers for MK-TME

    Data->PfrState = Opd->PfrState;                                  // Bit-mapped information of PFR State
    Data->PfrRecoveryCount = Opd->PfrRecoveryCount;                  // #of Recovery since last AC power cycle
    Data->PfrLastRecoveryReason = Opd->PfrLastRecoveryReason;        // Last Recovery Reason
    Data->PfrPanicEventCount = Opd->PfrPanicEventCount;              // #of Panic events since last AC power cycle
    Data->PfrLastPanicReason = Opd->PfrLastPanicReason;              // Last Panic Reason

    // Add String, Add structure to SMBIOS: Data = Ptr to SMBIOS Structure, Length = formatted area Length
    s = AddStringAndAdd2Smbios (Smbios, Handle,                      // Add String, Add to SMBIOS structures
          (EFI_SMBIOS_TABLE_HEADER*) Data, Length,
          (CHAR16*) &OemStructureString[0]);
  }

  // Deallocate memory
  if (Data != NULL) { FreePool ((VOID*)(UINTN) (Data)); }            // If memory allocated, Deallocate memory

  DEBUG ((EFI_D_ERROR, "[OOB-SMBIOS] AddOemStructureOTA()  } -> "));
  if (s == EFI_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return s;                                                          // Status
}

//--------------------------------------------------------------------------
/**
  AddGroupAssociationStructureOTA(): This function forms the data of Group
  Association Structure for KNL and adds it to SMBIOS structures.

  @param   Smbios     INPUT: EFI_SMBIOS_PROTOCOL*, Ptr to SMBIOS protocol
  @param   Handle     INPUT: EFI_SMBIOS_HANDLE*, Ptr to array of handles assigned to OEM Structures of Group Members

  @retval  EFI_STATUS EFI_SUCCESS  Success
                      Other        Error

**/
EFI_STATUS
AddGroupAssociationStructureOTA (
  IN     EFI_SMBIOS_PROTOCOL  *Smbios,
  IN OUT EFI_SMBIOS_HANDLE    *Handle
  )
{
  // Local data
  EFI_STATUS                       s;
  GROUP_ASSOCIATION_STRUCTURE_OTA  *Data;
  EFI_SMBIOS_HANDLE                *SmbiosHandle;
  UINTN                            MaxSize, Length;

  DEBUG ((EFI_D_ERROR, "[OOB-SMBIOS] AddGroupAssociationStructureOTA()  { -> Form and Add Group Association Structure Type-%d (0x%02X)\n", GROUP_ASSOCIATION_STRUCTURE_TYPE, GROUP_ASSOCIATION_STRUCTURE_TYPE));
  s = EFI_SUCCESS;                                                   // Assume success

  // Allocate memory for holding Structure data
  DEBUG ((EFI_D_ERROR, "    Allocate memory for Group Association Structure: "));
  Length = sizeof (GROUP_ASSOCIATION_STRUCTURE_OTA);                 // Formatted area Length of Group Assciation Structure
  MaxSize = Length + (MAX_ASCIIZ_STRING_LENGTH_B + 2);               // Max Length of OEM Structure 1 (one string with double NULL at end)
  Data = NULL;                                                       // Ptr to data
  Data = (GROUP_ASSOCIATION_STRUCTURE_OTA*)(UINTN)                   // Allocate memory and init with 0
                      AllocateZeroPool (MaxSize);
  if (Data != NULL) {                                                // If memory ptr valid
    DEBUG ((EFI_D_ERROR, "Success\n"));
  } else {                                                           // If memory ptr not valid
    s = EFI_DEVICE_ERROR;                                            //   Error
    DEBUG ((EFI_D_ERROR, "Error, Memory not available\n"));
  }

  if (Length > (sizeof (UINT8) * 255)) {                             // If Structure formatted length too big
    s = EFI_DEVICE_ERROR;                                            //   Error
    DEBUG ((EFI_D_ERROR, "    Length of formatted area of Structure > 255 -> Error\n"));
  }

  // Form Group Association Structure: Data = Ptr to SMBIOS Structure, Length = formatted area Length
  if (s == EFI_SUCCESS) {                                            // If success
    DEBUG ((EFI_D_ERROR, "    Form Group Association Structure\n"));
    SmbiosHandle = Handle;                                           //   Ptr to EFI_SMIBIOS_HANDLE of Group Members
    Data->Type = GROUP_ASSOCIATION_STRUCTURE_TYPE;                   //   Structure Type
    Data->Length = (UINT8) Length;                                   //   Length of formatted area of structure
    Data->Handle = 0;                                                //   Handle
    Data->GroupName = 0x01;                                          //   String# of Group name
    Data->Item1Type = OEM_STRUCTURE_TYPE_OTA;                        //   Structure Type of OEM Structure for Member-1
    Data->Item1Handle = *SmbiosHandle;                               //   Handle of OEM Structure for Member-1
    SmbiosHandle++;                                                  //   Update ptr to EFI_SMBIOS_HANDLE
    // Add String, Add to SMBIOS: Data = Ptr to SMBIOS Structure, Length = formatted area Length
    s = AddStringAndAdd2Smbios (Smbios, Handle,                      //   Add String, Add to SMBIOS structures
                 (EFI_SMBIOS_TABLE_HEADER*) Data, Length,
                 (CHAR16*) &GroupStructureString[0]);
  }

  // Deallocate memory
  if (Data != NULL) { FreePool ((VOID*)(UINTN) (Data)); }            // If memory allocated, Deallocate memory

  DEBUG ((EFI_D_ERROR, "[OOB-SMBIOS] AddGroupAssociationStructureOTA()  } -> "));
  if (s == EFI_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return s;                                                          // Status
}

//--------------------------------------------------------------------------
//************************** PUBLIC FUNCTIONS ******************************
//--------------------------------------------------------------------------
/*
  This function generates the SMBIOS Structures for OTA.

  @param   Ott        INPUT: OOB_TXT_TPM*, Ptr to OOB_TXT_TPM
  @param   Opd        INPUT: OT_PLATFORM_DATA*, Ptr to OT_PLATFORM_DATA

  @retval  EFI_STATUS EFI_SUCCESS  Success
                      Other        Error
*/
//--------------------------------------------------------------------------
EFI_STATUS
GenerateSmbiosStructuresOTA (
  IN  OOB_TXT_TPM      *Ott,
  IN  OT_PLATFORM_DATA *Opd
  )
{
  // Local data
  EFI_STATUS            s;
  EFI_SMBIOS_PROTOCOL   *Smbios;                                     // Ptr to SMBIOS Protocol
  EFI_SMBIOS_HANDLE     OemHandle[MAX_OEM_STRUCTURE];                // Ptr to array of Handles assigned to OEM Structures by BIOS

  DEBUG ((EFI_D_ERROR, "[OOB-SMBIOS] GenerateSmbiosStructuresOTA()  { -> Generate OTA SMBIOS Structures\n"));

  // Get ptr to SMBIOS Protocol
  s = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid,                  // Ptr to SMBIOS Protocol
                 NULL, (VOID**) &Smbios);
  if (s != EFI_SUCCESS) {                                            // If not success
    DEBUG ((EFI_D_ERROR, "    ERROR: SMBIOS Protocol not found, EFI Status 0x%08X\n", s));
  }

  // Generate OEM Structure(s)
  if (s == EFI_SUCCESS) {                                            // If success
    s = AddOemStructureOTA (Ott, Opd, Smbios, &OemHandle[0]);        //   Add OTA OEM Structure
  }

  // Generate Group Association Structure
  if (s == EFI_SUCCESS) {                                            // If success
    s = AddGroupAssociationStructureOTA (Smbios, &OemHandle[0]);     //   Add Group Association Structure
  }

  DEBUG ((EFI_D_ERROR, "[OOB-SMBIOS] GenerateSmbiosStructuresOTA()  } -> "));
  if (s == EFI_SUCCESS) { DEBUG ((EFI_D_ERROR, "Success\n")); }
  else { DEBUG ((EFI_D_ERROR, "Error\n")); }

  return s;                                                          // Status
}

//--------------------------------------------------------------------------
