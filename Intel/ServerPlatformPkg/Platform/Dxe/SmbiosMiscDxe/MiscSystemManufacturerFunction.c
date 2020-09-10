/** @file
  System Information: SMBIOS Type 1.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2008 - 2017 Intel Corporation. <BR>

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
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>

/**
  Add the System Information structure (Type 1) to SMBIOS.

  @param[in]  Smbios              A pointer to the SMBIOS protocol.

  @retval EFI_SUCCESS             The structure was added to SMBIOS.
  @retval EFI_INVALID_PARAMETER   An input pointer was null on entry.
  @retval EFI_UNSUPPORTED         A required PCD was missing or malformed.
  @retval EFI_OUT_OF_RESOURCES    The required memory could not be allocated.
**/
EFI_STATUS
AddSystemInformation (
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  )
{
  EFI_STATUS                Status;
  SMBIOS_TABLE_TYPE1        *SmbiosRecord;
  CHAR8                     *OptionalStrStart;
  CHAR8                     *ManufacturerStr;
  CHAR8                     *ProductNameStr;
  CHAR8                     *VersionStr;
  CHAR8                     *SerialNumberStr;
  CHAR8                     *SkuNumberStr;
  CHAR8                     *FamilyStr;
  UINTN                     ManufacturerStrLen;
  UINTN                     ProductNameStrLen;
  UINTN                     VersionStrLen;
  UINTN                     SerialNumberStrLen;
  UINTN                     SkuNumberStrLen;
  UINTN                     FamilyStrLen;
  UINTN                     TotalSize;
  EFI_SMBIOS_HANDLE         SmbiosHandle;
  EFI_HOB_GUID_TYPE         *GuidHob;

  //
  // First check for invalid parameters.
  //
  if (Smbios == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Get the strings and their lengths.
  //
  ManufacturerStr = (CHAR8 *) PcdGetPtr (PcdSmbiosTablesSystemManufacturer);
  if (ManufacturerStr == NULL) {
    return EFI_UNSUPPORTED;
  }
  ManufacturerStrLen = AsciiStrnLenS (ManufacturerStr, SMBIOS_STRING_MAX_LENGTH);
  if (ManufacturerStrLen >= SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  ProductNameStr = (CHAR8 *) PcdGetPtr (PcdSmbiosTablesSystemProductName);
  if (ProductNameStr == NULL) {
    return EFI_UNSUPPORTED;
  }
  ProductNameStrLen = AsciiStrnLenS (ProductNameStr, SMBIOS_STRING_MAX_LENGTH);
  if (ProductNameStrLen >= SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  VersionStr = (CHAR8 *) PcdGetPtr (PcdSmbiosTablesSystemVersion);
  if (VersionStr == NULL) {
    return EFI_UNSUPPORTED;
  }
  VersionStrLen = AsciiStrnLenS (VersionStr, SMBIOS_STRING_MAX_LENGTH);
  if (VersionStrLen >= SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  SerialNumberStr = (CHAR8 *) PcdGetPtr (PcdSmbiosTablesSystemSerialNumber);
  if (SerialNumberStr == NULL) {
    return EFI_UNSUPPORTED;
  }
  SerialNumberStrLen = AsciiStrnLenS (SerialNumberStr, SMBIOS_STRING_MAX_LENGTH);
  if (SerialNumberStrLen >= SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  SkuNumberStr = (CHAR8 *) PcdGetPtr (PcdSmbiosTablesSystemSkuNumber);
  if (SkuNumberStr == NULL) {
    return EFI_UNSUPPORTED;
  }
  SkuNumberStrLen = AsciiStrnLenS (SkuNumberStr, SMBIOS_STRING_MAX_LENGTH);
  if (SkuNumberStrLen >= SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  FamilyStr = (CHAR8 *) PcdGetPtr (PcdSmbiosTablesSystemFamily);
  if (FamilyStr == NULL) {
    return EFI_UNSUPPORTED;
  }
  FamilyStrLen = AsciiStrnLenS (FamilyStr, SMBIOS_STRING_MAX_LENGTH);
  if (FamilyStrLen >= SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  //
  // Allocate space for the table plus optional strings. Each string ends with
  // a null, with another null after the last string.
  //
  TotalSize = sizeof (SMBIOS_TABLE_TYPE1) +
              ManufacturerStrLen + 1 +
              ProductNameStrLen + 1 +
              VersionStrLen + 1 +
              SerialNumberStrLen + 1 +
              SkuNumberStrLen + 1 +
              FamilyStrLen + 1 + 1;
  SmbiosRecord = AllocateZeroPool (TotalSize);
  ASSERT (SmbiosRecord != NULL);
  if (SmbiosRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_SYSTEM_INFORMATION;
  SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE1);
  SmbiosRecord->Hdr.Handle = 0;         // filled by SMBIOS protocol
  SmbiosRecord->Manufacturer = 1;       // 1st optional string
  SmbiosRecord->ProductName = 2;        // 2nd optional string
  SmbiosRecord->Version = 3;            // 3rd optional string
  SmbiosRecord->SerialNumber = 4;       // 4th optional string

  GuidHob = GetFirstGuidHob (&gEfiMfgUUIDHobGuid);
  if (GuidHob == NULL) {
    //
    // UUID was not located in the system, per SMBIOS spec set the values to all
    // zero to indicate the UUID is not present in the system.
    //
    DEBUG ((EFI_D_ERROR, "ERROR: UUID was not located\n"));
    ZeroMem (&SmbiosRecord->Uuid, sizeof (SmbiosRecord->Uuid));
  } else {
    CopyMem (
      &SmbiosRecord->Uuid,
      GET_GUID_HOB_DATA (GuidHob),
      sizeof (SmbiosRecord->Uuid)
      );
  }

  SmbiosRecord->WakeUpType = SystemWakeupTypePowerSwitch;
  SmbiosRecord->SKUNumber = 5;          // 5th optional string
  SmbiosRecord->Family = 6;             // 6th optional string

  //
  // Copy the strings to the end.
  //
  OptionalStrStart = (CHAR8 *) (SmbiosRecord + 1);

  AsciiStrnCpyS (OptionalStrStart, SMBIOS_STRING_MAX_LENGTH, ManufacturerStr, ManufacturerStrLen);
  OptionalStrStart += ManufacturerStrLen + 1;

  AsciiStrnCpyS (OptionalStrStart, SMBIOS_STRING_MAX_LENGTH, ProductNameStr, ProductNameStrLen);
  OptionalStrStart += ProductNameStrLen + 1;

  AsciiStrnCpyS (OptionalStrStart, SMBIOS_STRING_MAX_LENGTH, VersionStr, VersionStrLen);
  OptionalStrStart += VersionStrLen + 1;

  AsciiStrnCpyS (OptionalStrStart, SMBIOS_STRING_MAX_LENGTH, SerialNumberStr, SerialNumberStrLen);
  OptionalStrStart += SerialNumberStrLen + 1;

  AsciiStrnCpyS (OptionalStrStart, SMBIOS_STRING_MAX_LENGTH, SkuNumberStr, SkuNumberStrLen);
  OptionalStrStart += SkuNumberStrLen + 1;

  AsciiStrnCpyS (OptionalStrStart, SMBIOS_STRING_MAX_LENGTH, FamilyStr, FamilyStrLen);

  Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);
  FreePool (SmbiosRecord);
  return Status;
}
