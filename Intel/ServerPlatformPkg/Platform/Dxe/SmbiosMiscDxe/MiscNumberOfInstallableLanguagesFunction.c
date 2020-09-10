/** @file
  BIOS Language Information: SMBIOS Type 13.

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

#include "SmbiosMisc.h"
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>

/**
  Add the BIOS Language Information structure (Type 13) to SMBIOS.

  @param[in]  Smbios              A pointer to the SMBIOS protocol.

  @retval EFI_SUCCESS             The structure was added to SMBIOS.
  @retval EFI_INVALID_PARAMETER   An input pointer was null on entry.
  @retval EFI_OUT_OF_RESOURCES    The required memory could not be allocated.
**/
EFI_STATUS
AddInstallableLanguages (
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  )
{
  EFI_STATUS                Status;
  SMBIOS_TABLE_TYPE13       *SmbiosRecord;
  CHAR8                     *OptionalStrStart;
  CHAR8                     *SystemLanguageStr;
  CHAR8                     SystemLanguageStrMod[5];
  UINTN                     SystemLanguageStrLen;
  UINTN                     TotalSize;
  EFI_SMBIOS_HANDLE         SmbiosHandle;

  //
  // First check for invalid parameters.
  //
  if (Smbios == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Get the strings and their lengths.
  //
  SystemLanguageStr = (CHAR8 *) PcdGetPtr (PcdUefiVariableDefaultPlatformLang);
  if (SystemLanguageStr == NULL) {
    return EFI_UNSUPPORTED;
  }
  SystemLanguageStrLen = AsciiStrnLenS (SystemLanguageStr, SMBIOS_STRING_MAX_LENGTH);
  if (SystemLanguageStrLen >= SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  //
  // The PCD format is like "en-US"; we need "enUS". Make sure the format is
  // correct and then remove the dash.
  //
  if (SystemLanguageStrLen != 5) {
    return EFI_UNSUPPORTED;
  }
  if (SystemLanguageStr[2] != '-') {
    return EFI_UNSUPPORTED;
  }
  SystemLanguageStrMod[0] = SystemLanguageStr[0];
  SystemLanguageStrMod[1] = SystemLanguageStr[1];
  SystemLanguageStrMod[2] = SystemLanguageStr[3];
  SystemLanguageStrMod[3] = SystemLanguageStr[4];
  SystemLanguageStrMod[4] = SystemLanguageStr[5];
  SystemLanguageStrLen--;

  //
  // Allocate space for the table plus optional strings. Each string ends with
  // a null, with another null after the last string.
  //
  TotalSize = sizeof (SMBIOS_TABLE_TYPE13) + SystemLanguageStrLen + 1 + 1;
  SmbiosRecord = AllocateZeroPool (TotalSize);
  ASSERT (SmbiosRecord != NULL);
  if (SmbiosRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_BIOS_LANGUAGE_INFORMATION;
  SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE13);
  SmbiosRecord->Hdr.Handle = 0;         // filled by SMBIOS protocol

  SmbiosRecord->InstallableLanguages = 1;  // 1st optional string
  SmbiosRecord->Flags = LanguageFormatAbbreviated;
  SmbiosRecord->CurrentLanguages = 1;  // The currently selected language (1-based).

  //
  // Copy the strings to the end.
  //
  OptionalStrStart = (CHAR8 *) (SmbiosRecord + 1);

  AsciiStrnCpyS (OptionalStrStart, SMBIOS_STRING_MAX_LENGTH, SystemLanguageStrMod, SystemLanguageStrLen);

  Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);
  FreePool (SmbiosRecord);
  return Status;
}
