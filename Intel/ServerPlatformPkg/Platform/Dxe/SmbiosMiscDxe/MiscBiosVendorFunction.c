/** @file
  BIOS Information: SMBIOS Type 0.

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
#include <Library/BaseMemoryLib.h>
#include <Library/BiosIdLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>

/**
  Add the BIOS Information structure (Type 0) to SMBIOS.

  @param[in]  Smbios              A pointer to the SMBIOS protocol.

  @retval EFI_SUCCESS             The structure was added to SMBIOS.
  @retval EFI_INVALID_PARAMETER   An input pointer was null on entry.
  @retval EFI_UNSUPPORTED         A required PCD was missing or malformed.
  @retval EFI_OUT_OF_RESOURCES    The required memory could not be allocated.
**/
EFI_STATUS
AddBiosInformation (
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  )
{
  EFI_STATUS                Status;
  SMBIOS_TABLE_TYPE0        *SmbiosRecord;
  CHAR8                     *OptionalStrStart;
  CHAR8                     *VendorStr;
  CHAR8                     *BiosVersionStr;
  CHAR8                     *BiosReleaseDateStr;
  UINTN                     VendorStrLen;
  UINTN                     BiosVersionStrLen;
  UINTN                     BiosReleaseDateStrLen;
  CHAR16                    UnicodeBiosVersion[SMBIOS_STRING_MAX_LENGTH];
  CHAR16                    UnicodeBiosReleaseDate[SMBIOS_STRING_MAX_LENGTH];
  CHAR8                     AsciiBiosVersion[SMBIOS_STRING_MAX_LENGTH];
  CHAR8                     AsciiBiosReleaseDate[SMBIOS_STRING_MAX_LENGTH];
  UINTN                     TotalSize;
  UINT32                    BiosRomSize;
  EFI_SMBIOS_HANDLE         SmbiosHandle;
  MISC_BIOS_CHARACTERISTICS_EXTENSION CharExt;

  //
  // First check for invalid parameters.
  //
  if (Smbios == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Get the strings and their lengths.
  //
  VendorStr = (CHAR8 *) PcdGetPtr (PcdSmbiosTablesBiosVendor);
  if (VendorStr == NULL) {
    return EFI_UNSUPPORTED;
  }
  VendorStrLen = AsciiStrnLenS (VendorStr, SMBIOS_STRING_MAX_LENGTH);
  if (VendorStrLen >= SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  //
  // Use null string pointers to indicate failure to get them from the library.
  //
  BiosVersionStr = NULL;
  BiosReleaseDateStr = NULL;
  BiosVersionStrLen = 0;
  BiosReleaseDateStrLen = 0;

  //
  // Get the BIOS version and release date.
  //
  Status = GetBiosVersionDateTime (
    UnicodeBiosVersion,
    UnicodeBiosReleaseDate,
    NULL
    );
  if (!EFI_ERROR (Status)) {
    //
    // Got the strings. Convert them to ASCII.
    //
    BiosVersionStrLen = StrnLenS (
      UnicodeBiosVersion,
      SMBIOS_STRING_MAX_LENGTH
      );
    if (BiosVersionStrLen > 0 &&
        BiosVersionStrLen < SMBIOS_STRING_MAX_LENGTH) {
      UnicodeStrToAsciiStrS (
        UnicodeBiosVersion,
        AsciiBiosVersion,
        BiosVersionStrLen + 1
        );
      BiosVersionStr = AsciiBiosVersion;
    }

    BiosReleaseDateStrLen = StrnLenS (
      UnicodeBiosReleaseDate,
      SMBIOS_STRING_MAX_LENGTH
      );
    if (BiosReleaseDateStrLen > 0 &&
        BiosReleaseDateStrLen < SMBIOS_STRING_MAX_LENGTH) {
      UnicodeStrToAsciiStrS (
        UnicodeBiosReleaseDate,
        AsciiBiosReleaseDate,
        BiosReleaseDateStrLen + 1
        );
      BiosReleaseDateStr = AsciiBiosReleaseDate;
    }
  }

  //
  // If the version string pointer is still null, get it from the PCD.
  //
  if (BiosVersionStr == NULL) {
    BiosVersionStr = (CHAR8 *) PcdGetPtr (PcdSmbiosTablesBiosVersion);
    if (BiosVersionStr == NULL) {
      return EFI_UNSUPPORTED;
    }
    BiosVersionStrLen = AsciiStrnLenS (BiosVersionStr, SMBIOS_STRING_MAX_LENGTH);
    if (BiosVersionStrLen >= SMBIOS_STRING_MAX_LENGTH) {
      return EFI_UNSUPPORTED;
    }
  }

  //
  // If the release date string pointer is still null, get it from the PCD.
  //
  if (BiosReleaseDateStr == NULL) {
    BiosReleaseDateStr = (CHAR8 *) PcdGetPtr (PcdSmbiosTablesBiosReleaseDate);
    if (BiosReleaseDateStr == NULL) {
      return EFI_UNSUPPORTED;
    }
    BiosReleaseDateStrLen = AsciiStrnLenS (BiosReleaseDateStr, SMBIOS_STRING_MAX_LENGTH);
    if (BiosReleaseDateStrLen >= SMBIOS_STRING_MAX_LENGTH) {
      return EFI_UNSUPPORTED;
    }
  }

  //
  // Allocate space for the table plus optional strings. Each string ends with
  // a null, with another null after the last string.
  //
  TotalSize = sizeof (SMBIOS_TABLE_TYPE0) +
              VendorStrLen + 1 +
              BiosVersionStrLen + 1 +
              BiosReleaseDateStrLen + 1 + 1;
  SmbiosRecord = AllocateZeroPool (TotalSize);
  ASSERT (SmbiosRecord != NULL);
  if (SmbiosRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  BiosRomSize = PcdGet32 (PcdBiosSize);

  SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_BIOS_INFORMATION;
  SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE0);
  SmbiosRecord->Hdr.Handle = 0;         // filled by SMBIOS protocol
  SmbiosRecord->Vendor = 1;             // 1st optional string
  SmbiosRecord->BiosVersion = 2;        // 2nd optional string
  SmbiosRecord->BiosSegment = 0xF000;
  SmbiosRecord->BiosReleaseDate = 3;    // 3rd optional string

  //
  // If ROM size > 16MB then BiosSize field must be 0xFF
  //
  if ((BiosRomSize >> 20) < 16) {
    SmbiosRecord->BiosSize = (UINT8) ((BiosRomSize / SIZE_64KB) - 1);
  } else {
    SmbiosRecord->BiosSize = 0xFF;
  }

  SmbiosRecord->BiosCharacteristics.Reserved = 0;
  SmbiosRecord->BiosCharacteristics.Unknown = 0;
  SmbiosRecord->BiosCharacteristics.BiosCharacteristicsNotSupported = 0;
  SmbiosRecord->BiosCharacteristics.IsaIsSupported = 0;
  SmbiosRecord->BiosCharacteristics.McaIsSupported = 0;
  SmbiosRecord->BiosCharacteristics.EisaIsSupported = 0;
  SmbiosRecord->BiosCharacteristics.PciIsSupported = 1;
  SmbiosRecord->BiosCharacteristics.PcmciaIsSupported = 0;
  SmbiosRecord->BiosCharacteristics.PlugAndPlayIsSupported = 1;
  SmbiosRecord->BiosCharacteristics.ApmIsSupported = 0;
  SmbiosRecord->BiosCharacteristics.BiosIsUpgradable = 1;
  SmbiosRecord->BiosCharacteristics.BiosShadowingAllowed = 1;
  SmbiosRecord->BiosCharacteristics.VlVesaIsSupported = 0;
  SmbiosRecord->BiosCharacteristics.EscdSupportIsAvailable = 0;
  SmbiosRecord->BiosCharacteristics.BootFromCdIsSupported = 1;
  SmbiosRecord->BiosCharacteristics.SelectableBootIsSupported = 1;
  SmbiosRecord->BiosCharacteristics.RomBiosIsSocketed = 0;
  SmbiosRecord->BiosCharacteristics.BootFromPcmciaIsSupported = 0;
  SmbiosRecord->BiosCharacteristics.EDDSpecificationIsSupported = 1;
  SmbiosRecord->BiosCharacteristics.JapaneseNecFloppyIsSupported = 0;
  SmbiosRecord->BiosCharacteristics.JapaneseToshibaFloppyIsSupported = 0;
  SmbiosRecord->BiosCharacteristics.Floppy525_360IsSupported = 0;
  SmbiosRecord->BiosCharacteristics.Floppy525_12IsSupported = 1;
  SmbiosRecord->BiosCharacteristics.Floppy35_720IsSupported = 1;
  SmbiosRecord->BiosCharacteristics.Floppy35_288IsSupported = 1;
  SmbiosRecord->BiosCharacteristics.PrintScreenIsSupported = 1;
  SmbiosRecord->BiosCharacteristics.Keyboard8042IsSupported = 1;
  SmbiosRecord->BiosCharacteristics.SerialIsSupported = 1;
  SmbiosRecord->BiosCharacteristics.PrinterIsSupported = 1;
  SmbiosRecord->BiosCharacteristics.CgaMonoIsSupported = 1;
  SmbiosRecord->BiosCharacteristics.NecPc98 = 0;
  SmbiosRecord->BiosCharacteristics.ReservedForVendor = 0;

  //
  // BIOS Characteristics Extension Bytes.
  //
  CharExt.BiosReserved.AcpiIsSupported = 1;
  CharExt.BiosReserved.UsbLegacyIsSupported = 1;
  CharExt.BiosReserved.AgpIsSupported = 0;
  CharExt.BiosReserved.I2OBootIsSupported = 0;
  CharExt.BiosReserved.Ls120BootIsSupported = 1;
  CharExt.BiosReserved.AtapiZipDriveBootIsSupported = 1;
  CharExt.BiosReserved.Boot1394IsSupported = 0;
  CharExt.BiosReserved.SmartBatteryIsSupported = 0;
  if (PcdGetBool (PcdUefiOptimizedBoot)) {
    CharExt.SystemReserved.BiosBootSpecIsSupported = 0;
  } else {
    CharExt.SystemReserved.BiosBootSpecIsSupported = 1;
  }
  CharExt.SystemReserved.FunctionKeyNetworkBootIsSupported = 1;
  CharExt.SystemReserved.TargetContentDistributionEnabled = 1;
  CharExt.SystemReserved.UefiSpecificationSupported = 1;
  CharExt.SystemReserved.VirtualMachineSupported = 0;
  CharExt.SystemReserved.ExtensionByte2Reserved = 0;
  CopyMem (
    SmbiosRecord->BIOSCharacteristicsExtensionBytes,
    &CharExt,
    sizeof (SmbiosRecord->BIOSCharacteristicsExtensionBytes)
    );

  SmbiosRecord->SystemBiosMajorRelease = 0xFF;
  SmbiosRecord->SystemBiosMinorRelease = 0xFF;
  SmbiosRecord->EmbeddedControllerFirmwareMajorRelease = 0xFF;
  SmbiosRecord->EmbeddedControllerFirmwareMinorRelease = 0xFF;

  //
  // Since PcdGet32 only returns 32 bit values, we always represent the ROM
  // size as a multiple of 1MB. Note that SMBIOS spec assumes that the ROM size
  // is always a multiple of 1MB. Also note that this code rounds down the size
  // of the ROM when the size is not a multiple of 1MB.
  //
  SmbiosRecord->ExtendedBiosSize.Size                = (UINT16)(BiosRomSize >> 20) & 0x3FFF;
  SmbiosRecord->ExtendedBiosSize.Unit                = 0x0;

  //
  // Copy the strings to the end.
  //
  OptionalStrStart = (CHAR8 *) (SmbiosRecord + 1);

  AsciiStrnCpyS (OptionalStrStart, SMBIOS_STRING_MAX_LENGTH, VendorStr, VendorStrLen);
  OptionalStrStart += VendorStrLen + 1;

  AsciiStrnCpyS (OptionalStrStart, SMBIOS_STRING_MAX_LENGTH, BiosVersionStr, BiosVersionStrLen);
  OptionalStrStart += BiosVersionStrLen + 1;

  AsciiStrnCpyS (OptionalStrStart, SMBIOS_STRING_MAX_LENGTH, BiosReleaseDateStr, BiosReleaseDateStrLen);

  Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);
  FreePool (SmbiosRecord);
  return Status;
}
