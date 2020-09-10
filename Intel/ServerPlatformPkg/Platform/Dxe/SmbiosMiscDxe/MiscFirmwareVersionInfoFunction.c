/** @file
  Group Associations: SMBIOS Type 14

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2020 Intel Corporation. <BR>

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
  Add the Firmware Version Information structure (Type 14) to SMBIOS.

  @param[in]  Smbios              A pointer to the SMBIOS protocol.

  @retval EFI_SUCCESS             The structure was added to SMBIOS.
  @retval EFI_INVALID_PARAMETER   An input pointer was null on entry.
  @retval EFI_OUT_OF_RESOURCES    The required memory could not be allocated.
**/
EFI_STATUS
FirmwareVersionInfo (
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  )
{
  EFI_STATUS                Status;
  SMBIOS_TABLE_TYPE14       *SmbiosRecord;
  CHAR8                     *OptionalStrStart;
  CHAR8                     *FwVerInfStr;
  UINTN                     FwVerInfStrLen;
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
  FwVerInfStr = (CHAR8 *) FIRMWARE_VERSION_INFO_GROUP_NAME;
  if (FwVerInfStr == NULL) {
    return EFI_UNSUPPORTED;
  }
  FwVerInfStrLen = AsciiStrnLenS (FwVerInfStr, SMBIOS_STRING_MAX_LENGTH);
  if (FwVerInfStrLen >= SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  SmbiosRecord = NULL;

  TotalSize = sizeof (SMBIOS_TABLE_TYPE14) + FwVerInfStrLen + 1 + 1;

  SmbiosRecord = AllocateZeroPool (TotalSize);
  if (SmbiosRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_GROUP_ASSOCIATIONS;
  SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE14);
  SmbiosRecord->Hdr.Handle = 0;
  SmbiosRecord->GroupName  = 1;
  SmbiosRecord->Group->ItemType  = PcdGet8 (PcdSmbiosTableType);

  //
  // Copy the strings to the end.
  //
  OptionalStrStart = (CHAR8 *) (SmbiosRecord + 1);

  AsciiStrnCpyS (OptionalStrStart, SMBIOS_STRING_MAX_LENGTH, FwVerInfStr, FwVerInfStrLen);

  Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);
  FreePool (SmbiosRecord);
  return Status;
}
