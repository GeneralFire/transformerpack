/** @file
  Other SMBIOS Types:
  OEM Strings: SMBIOS Type 11
  System Configuration Options: SMBIOS Type 12
  Cooling Device: SMBIOS Type 27
  Temperature Probe: SMBIOS Type 28
  Management Device: SMBIOS Type 34
  Management Device Component: SMBIOS Type 35
  Management Device Threshold Data: SMBIOS Type 36
  IPMI Device Information: SMBIOS Type 38
  System Power Supply: SMBIOS Type 39

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2017 Intel Corporation. <BR>

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
  Add the OEM Strings structure (Type 11) to SMBIOS.

  @param[in]  Smbios              A pointer to the SMBIOS protocol.

  @retval EFI_SUCCESS             The structure was added to SMBIOS.
  @retval EFI_INVALID_PARAMETER   An input pointer was null on entry.
  @retval EFI_UNSUPPORTED         A required PCD was missing or malformed.
  @retval EFI_OUT_OF_RESOURCES    The required memory could not be allocated.
**/
EFI_STATUS
AddOemStrings (
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  )
{
  EFI_STATUS                Status;
  SMBIOS_TABLE_TYPE11       *SmbiosRecord;
  CHAR8                     *OptionalStrStart;
  CHAR8                     *OemStringStr;
  UINTN                     OemStringStrLen;
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
  OemStringStr = (CHAR8 *) PcdGetPtr (PcdSmbiosTablesOemString1);
  if (OemStringStr == NULL) {
    return EFI_UNSUPPORTED;
  }
  OemStringStrLen = AsciiStrnLenS (OemStringStr, SMBIOS_STRING_MAX_LENGTH);
  if (OemStringStrLen >= SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  //
  // Allocate space for the table plus optional strings. Each string ends with
  // a null, with another null after the last string.
  //
  TotalSize = sizeof (SMBIOS_TABLE_TYPE11) + OemStringStrLen + 1 + 1;
  SmbiosRecord = AllocateZeroPool (TotalSize);
  ASSERT (SmbiosRecord != NULL);
  if (SmbiosRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_OEM_STRINGS;
  SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE11);
  SmbiosRecord->Hdr.Handle = 0;         // filled by SMBIOS protocol
  SmbiosRecord->StringCount = 1;

  //
  // Copy the strings to the end.
  //
  OptionalStrStart = (CHAR8 *) (SmbiosRecord + 1);

  AsciiStrnCpyS (OptionalStrStart, SMBIOS_STRING_MAX_LENGTH, OemStringStr, OemStringStrLen);

  Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);
  FreePool (SmbiosRecord);
  return Status;
}

/**
  Add the System Configuration Options structure (Type 12) to SMBIOS.

  @param[in]  Smbios              A pointer to the SMBIOS protocol.

  @retval EFI_SUCCESS             The structure was added to SMBIOS.
  @retval EFI_INVALID_PARAMETER   An input pointer was null on entry.
  @retval EFI_UNSUPPORTED         A required PCD was missing or malformed.
  @retval EFI_OUT_OF_RESOURCES    The required memory could not be allocated.
**/
EFI_STATUS
AddSystemConfigurationOptions (
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  )
{
  EFI_STATUS                Status;
  SMBIOS_TABLE_TYPE12       *SmbiosRecord;
  CHAR8                     *OptionalStrStart;
  CHAR8                     *SysConOptionStr;
  UINTN                     SysConOptionStrLen;
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
  SysConOptionStr = (CHAR8 *) PcdGetPtr (PcdSmbiosTablesSysConfigOption1);
  if (SysConOptionStr == NULL) {
    return EFI_UNSUPPORTED;
  }
  SysConOptionStrLen = AsciiStrnLenS (SysConOptionStr, SMBIOS_STRING_MAX_LENGTH);
  if (SysConOptionStrLen >= SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  //
  // Allocate space for the table plus optional strings. Each string ends with
  // a null, with another null after the last string.
  //
  TotalSize = sizeof (SMBIOS_TABLE_TYPE12) + SysConOptionStrLen + 1 + 1;
  SmbiosRecord = AllocateZeroPool (TotalSize);
  ASSERT (SmbiosRecord != NULL);
  if (SmbiosRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_SYSTEM_CONFIGURATION_OPTIONS;
  SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE12);
  SmbiosRecord->Hdr.Handle = 0;         // filled by SMBIOS protocol
  SmbiosRecord->StringCount = 1;

  //
  // Copy the strings to the end.
  //
  OptionalStrStart = (CHAR8 *) (SmbiosRecord + 1);

  AsciiStrnCpyS (OptionalStrStart, SMBIOS_STRING_MAX_LENGTH, SysConOptionStr, SysConOptionStrLen);

  Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);
  FreePool (SmbiosRecord);
  return Status;
}

/**
  Add the Cooling Device structure (Type 27) to SMBIOS.

  @param[in]  Smbios              A pointer to the SMBIOS protocol.

  @retval EFI_SUCCESS             The structure was added to SMBIOS.
  @retval EFI_INVALID_PARAMETER   An input pointer was null on entry.
  @retval EFI_UNSUPPORTED         A required PCD was missing or malformed.
  @retval EFI_OUT_OF_RESOURCES    The required memory could not be allocated.
**/
EFI_STATUS
AddCoolingDevice (
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  )
{
  EFI_STATUS                Status;
  SMBIOS_TABLE_TYPE27       *SmbiosRecord;
  CHAR8                     *OptionalStrStart;
  CHAR8                     *DescriptionStr;
  UINTN                     DescriptionStrLen;
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
  DescriptionStr = (CHAR8 *) PcdGetPtr (PcdSmbiosTablesCoolingDeviceDescription);
  if (DescriptionStr == NULL) {
    return EFI_UNSUPPORTED;
  }
  DescriptionStrLen = AsciiStrnLenS (DescriptionStr, SMBIOS_STRING_MAX_LENGTH);
  if (DescriptionStrLen >= SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  //
  // Allocate space for the table plus optional strings. Each string ends with
  // a null, with another null after the last string.
  //
  TotalSize = sizeof (SMBIOS_TABLE_TYPE27) + DescriptionStrLen + 1 + 1;
  SmbiosRecord = AllocateZeroPool (TotalSize);
  ASSERT (SmbiosRecord != NULL);
  if (SmbiosRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_COOLING_DEVICE;
  SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE27);
  SmbiosRecord->Hdr.Handle = 0;         // filled by SMBIOS protocol
  SmbiosRecord->TemperatureProbeHandle = TEMPERATURE_PROBE_HANDLE;
  SmbiosRecord->DeviceTypeAndStatus.CoolingDevice = CoolingDeviceTypeFan;
  SmbiosRecord->DeviceTypeAndStatus.CoolingDeviceStatus = CoolingDeviceStatusOk;
  SmbiosRecord->CoolingUnitGroup = 1;
  SmbiosRecord->OEMDefined = 0;
  SmbiosRecord->NominalSpeed = UNKNOWN_VALUE;
  SmbiosRecord->Description = 1;        // 1st optional string

  //
  // Copy the strings to the end.
  //
  OptionalStrStart = (CHAR8 *) (SmbiosRecord + 1);

  AsciiStrnCpyS (OptionalStrStart, SMBIOS_STRING_MAX_LENGTH, DescriptionStr, DescriptionStrLen);

  SmbiosHandle = COOLING_DEVICE_HANDLE;
  Status = Smbios->Add (
    Smbios,
    NULL,
    &SmbiosHandle,
    (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord
    );
  FreePool (SmbiosRecord);
  return Status;
}

/**
  Add the Temperature Probe structure (Type 28) to SMBIOS.

  @param[in]  Smbios              A pointer to the SMBIOS protocol.

  @retval EFI_SUCCESS             The structure was added to SMBIOS.
  @retval EFI_INVALID_PARAMETER   An input pointer was null on entry.
  @retval EFI_UNSUPPORTED         A required PCD was missing or malformed.
  @retval EFI_OUT_OF_RESOURCES    The required memory could not be allocated.
**/
EFI_STATUS
AddTemperatureProbe (
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  )
{
  EFI_STATUS                Status;
  SMBIOS_TABLE_TYPE28       *SmbiosRecord;
  CHAR8                     *OptionalStrStart;
  CHAR8                     *DescriptionStr;
  UINTN                     DescriptionStrLen;
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
  DescriptionStr = (CHAR8 *) PcdGetPtr (PcdSmbiosTablesTemperatureProbeDescription);
  if (DescriptionStr == NULL) {
    return EFI_UNSUPPORTED;
  }
  DescriptionStrLen = AsciiStrnLenS (DescriptionStr, SMBIOS_STRING_MAX_LENGTH);
  if (DescriptionStrLen >= SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  //
  // Allocate space for the table plus optional strings. Each string ends with
  // a null, with another null after the last string.
  //
  TotalSize = sizeof (SMBIOS_TABLE_TYPE28) + DescriptionStrLen + 1 + 1;
  SmbiosRecord = AllocateZeroPool (TotalSize);
  ASSERT (SmbiosRecord != NULL);
  if (SmbiosRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_TEMPERATURE_PROBE;
  SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE28);
  SmbiosRecord->Hdr.Handle = 0;         // filled by SMBIOS protocol
  SmbiosRecord->Description = 1;        // 1st optional string
  SmbiosRecord->LocationAndStatus.TemperatureProbeSite = TemperatureProbeSiteUnknown;
  SmbiosRecord->LocationAndStatus.TemperatureProbeStatus = TemperatureProbeStatusUnknown;
  SmbiosRecord->MaximumValue  = UNKNOWN_VALUE;
  SmbiosRecord->MinimumValue  = UNKNOWN_VALUE;
  SmbiosRecord->Resolution = UNKNOWN_VALUE;
  SmbiosRecord->Tolerance = UNKNOWN_VALUE;
  SmbiosRecord->Accuracy = UNKNOWN_VALUE;
  SmbiosRecord->OEMDefined = 0;
  SmbiosRecord->NominalValue = UNKNOWN_VALUE;

  //
  // Copy the strings to the end.
  //
  OptionalStrStart = (CHAR8 *) (SmbiosRecord + 1);

  AsciiStrnCpyS (OptionalStrStart, SMBIOS_STRING_MAX_LENGTH, DescriptionStr, DescriptionStrLen);

  SmbiosHandle = TEMPERATURE_PROBE_HANDLE;
  Status = Smbios->Add (
    Smbios,
    NULL,
    &SmbiosHandle,
    (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord
    );
  FreePool (SmbiosRecord);
  return Status;
}

/**
  Add the Management Device structure (Type 34) to SMBIOS.

  @param[in]  Smbios              A pointer to the SMBIOS protocol.

  @retval EFI_SUCCESS             The structure was added to SMBIOS.
  @retval EFI_INVALID_PARAMETER   An input pointer was null on entry.
  @retval EFI_UNSUPPORTED         A required PCD was missing or malformed.
  @retval EFI_OUT_OF_RESOURCES    The required memory could not be allocated.
**/
EFI_STATUS
AddManagementDevice (
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  )
{
  EFI_STATUS                Status;
  SMBIOS_TABLE_TYPE34       *SmbiosRecord;
  CHAR8                     *OptionalStrStart;
  CHAR8                     *DescriptionStr;
  UINTN                     DescriptionStrLen;
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
  DescriptionStr = (CHAR8 *) PcdGetPtr (PcdSmbiosTablesManagementDeviceDescription);
  if (DescriptionStr == NULL) {
    return EFI_UNSUPPORTED;
  }
  DescriptionStrLen = AsciiStrnLenS (DescriptionStr, SMBIOS_STRING_MAX_LENGTH);
  if (DescriptionStrLen >= SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  //
  // Allocate space for the table plus optional strings. Each string ends with
  // a null, with another null after the last string.
  //
  TotalSize = sizeof (SMBIOS_TABLE_TYPE34) + DescriptionStrLen + 1 + 1;
  SmbiosRecord = AllocateZeroPool (TotalSize);
  ASSERT (SmbiosRecord != NULL);
  if (SmbiosRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_MANAGEMENT_DEVICE;
  SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE34);
  SmbiosRecord->Hdr.Handle = 0;         // filled by SMBIOS protocol
  SmbiosRecord->Description = 1;        // 1st optional string
  SmbiosRecord->Type = ManagementDeviceTypeUnknown;
  SmbiosRecord->Address  = 0;
  SmbiosRecord->AddressType = ManagementDeviceAddressTypeUnknown;

  //
  // Copy the strings to the end.
  //
  OptionalStrStart = (CHAR8 *) (SmbiosRecord + 1);

  AsciiStrnCpyS (OptionalStrStart, SMBIOS_STRING_MAX_LENGTH, DescriptionStr, DescriptionStrLen);

  SmbiosHandle = MANAGEMENT_DEVICE_HANDLE;
  Status = Smbios->Add (
    Smbios,
    NULL,
    &SmbiosHandle,
    (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord
    );
  FreePool (SmbiosRecord);
  return Status;
}

/**
  Add the Management Device Component structure (Type 35) to SMBIOS.

  @param[in]  Smbios              A pointer to the SMBIOS protocol.

  @retval EFI_SUCCESS             The structure was added to SMBIOS.
  @retval EFI_INVALID_PARAMETER   An input pointer was null on entry.
  @retval EFI_UNSUPPORTED         A required PCD was missing or malformed.
  @retval EFI_OUT_OF_RESOURCES    The required memory could not be allocated.
**/
EFI_STATUS
AddManagementDeviceComponent (
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  )
{
  EFI_STATUS                Status;
  SMBIOS_TABLE_TYPE35       *SmbiosRecord;
  CHAR8                     *OptionalStrStart;
  CHAR8                     *DescriptionStr;
  UINTN                     DescriptionStrLen;
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
  DescriptionStr = (CHAR8 *) PcdGetPtr (PcdSmbiosTablesManagementDeviceComponentDescription);
  if (DescriptionStr == NULL) {
    return EFI_UNSUPPORTED;
  }
  DescriptionStrLen = AsciiStrnLenS (DescriptionStr, SMBIOS_STRING_MAX_LENGTH);
  if (DescriptionStrLen >= SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  //
  // Allocate space for the table plus optional strings. Each string ends with
  // a null, with another null after the last string.
  //
  TotalSize = sizeof (SMBIOS_TABLE_TYPE35) + DescriptionStrLen + 1 + 1;
  SmbiosRecord = AllocateZeroPool (TotalSize);
  ASSERT (SmbiosRecord != NULL);
  if (SmbiosRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_MANAGEMENT_DEVICE_COMPONENT;
  SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE35);
  SmbiosRecord->Hdr.Handle = 0;         // filled by SMBIOS protocol
  SmbiosRecord->Description = 1;        // 1st optional string
  SmbiosRecord->ManagementDeviceHandle = MANAGEMENT_DEVICE_HANDLE;
  SmbiosRecord->ComponentHandle = COOLING_DEVICE_HANDLE;
  SmbiosRecord->ThresholdHandle = THRESHOLD_HANDLE;

  //
  // Copy the strings to the end.
  //
  OptionalStrStart = (CHAR8 *) (SmbiosRecord + 1);

  AsciiStrnCpyS (OptionalStrStart, SMBIOS_STRING_MAX_LENGTH, DescriptionStr, DescriptionStrLen);

  Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);
  FreePool (SmbiosRecord);
  return Status;
}

/**
  Add the Management Device Threshold Data structure (Type 36) to SMBIOS.

  @param[in]  Smbios              A pointer to the SMBIOS protocol.

  @retval EFI_SUCCESS             The structure was added to SMBIOS.
  @retval EFI_INVALID_PARAMETER   An input pointer was null on entry.
  @retval EFI_UNSUPPORTED         A required PCD was missing or malformed.
  @retval EFI_OUT_OF_RESOURCES    The required memory could not be allocated.
**/
EFI_STATUS
AddManagementDeviceThresholdData (
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  )
{
  EFI_STATUS                Status;
  SMBIOS_TABLE_TYPE36       *SmbiosRecord;
  UINTN                     TotalSize;
  EFI_SMBIOS_HANDLE         SmbiosHandle;

  //
  // First check for invalid parameters.
  //
  if (Smbios == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Allocate space for the table plus optional strings. Each string ends with
  // a null, with another null after the last string.
  //
  TotalSize = sizeof (SMBIOS_TABLE_TYPE36) + 1 + 1;
  SmbiosRecord = AllocateZeroPool (TotalSize);
  ASSERT (SmbiosRecord != NULL);
  if (SmbiosRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_MANAGEMENT_DEVICE_THRESHOLD_DATA;
  SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE36);
  SmbiosRecord->Hdr.Handle = 0;         // filled by SMBIOS protocol
  SmbiosRecord->LowerThresholdNonCritical = UNKNOWN_VALUE;
  SmbiosRecord->UpperThresholdNonCritical = UNKNOWN_VALUE;
  SmbiosRecord->LowerThresholdCritical = UNKNOWN_VALUE;
  SmbiosRecord->UpperThresholdCritical = UNKNOWN_VALUE;
  SmbiosRecord->LowerThresholdNonRecoverable = UNKNOWN_VALUE;
  SmbiosRecord->UpperThresholdNonRecoverable = UNKNOWN_VALUE;

  SmbiosHandle = THRESHOLD_HANDLE;
  Status = Smbios->Add (
    Smbios,
    NULL,
    &SmbiosHandle,
    (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord
    );
  FreePool (SmbiosRecord);
  return Status;
}

/**
  Add the IPMI Device Information structure (Type 38) to SMBIOS.

  @param[in]  Smbios              A pointer to the SMBIOS protocol.

  @retval EFI_SUCCESS             The structure was added to SMBIOS.
  @retval EFI_INVALID_PARAMETER   An input pointer was null on entry.
  @retval EFI_UNSUPPORTED         A required PCD was missing or malformed.
  @retval EFI_OUT_OF_RESOURCES    The required memory could not be allocated.
**/
EFI_STATUS
AddIpmiDeviceInformation (
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  )
{
  EFI_STATUS                Status;
  SMBIOS_TABLE_TYPE38       *SmbiosRecord;
  UINTN                     TotalSize;
  EFI_SMBIOS_HANDLE         SmbiosHandle;

  //
  // First check for invalid parameters.
  //
  if (Smbios == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Allocate space for the table plus optional strings. Each string ends with
  // a null, with another null after the last string.
  //
  TotalSize = sizeof (SMBIOS_TABLE_TYPE38) + 1 + 1;
  SmbiosRecord = AllocateZeroPool (TotalSize);
  ASSERT (SmbiosRecord != NULL);
  if (SmbiosRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_IPMI_DEVICE_INFORMATION;
  SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE38);
  SmbiosRecord->Hdr.Handle = 0;         // filled by SMBIOS protocol
  SmbiosRecord->InterfaceType = IPMIDeviceInfoInterfaceTypeKCS;
  SmbiosRecord->IPMISpecificationRevision  = IPMI_SPECIFICATION_REVISION;
  SmbiosRecord->I2CSlaveAddress = IPMI_I2C_SLAVE_ADDRESS;
  SmbiosRecord->NVStorageDeviceAddress = IPMI_NV_STORAGE_DEVICE_ADDRESS;
  SmbiosRecord->BaseAddress = IPMI_BASE_ADDRESS;
  SmbiosRecord->BaseAddressModifier_InterruptInfo = 0;
  SmbiosRecord->InterruptNumber = 0;

  Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);
  FreePool (SmbiosRecord);
  return Status;
}

/**
  Add the System Power Supply structure (Type 39) to SMBIOS.

  @param[in]  Smbios              A pointer to the SMBIOS protocol.

  @retval EFI_SUCCESS             The structure was added to SMBIOS.
  @retval EFI_INVALID_PARAMETER   An input pointer was null on entry.
  @retval EFI_UNSUPPORTED         A required PCD was missing or malformed.
  @retval EFI_OUT_OF_RESOURCES    The required memory could not be allocated.
**/
EFI_STATUS
AddSystemPowerSupply (
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  )
{
  EFI_STATUS                Status;
  SMBIOS_TABLE_TYPE39       *SmbiosRecord;
  CHAR8                     *OptionalStrStart;
  CHAR8                     *LocationStr;
  CHAR8                     *DeviceNameStr;
  CHAR8                     *ManufacturerStr;
  CHAR8                     *SerialNumberStr;
  CHAR8                     *AssetTagStr;
  CHAR8                     *ModelPartStr;
  CHAR8                     *RevisionLevelStr;
  UINTN                     LocationStrLen;
  UINTN                     DeviceNameStrLen;
  UINTN                     ManufacturerStrLen;
  UINTN                     SerialNumberStrLen;
  UINTN                     AssetTagStrLen;
  UINTN                     ModelPartStrLen;
  UINTN                     RevisionLevelStrLen;
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
  LocationStr = (CHAR8 *) PcdGetPtr (PcdSmbiosTablesSystemPowerSupplyLocation);
  if (LocationStr == NULL) {
    return EFI_UNSUPPORTED;
  }
  LocationStrLen = AsciiStrnLenS (LocationStr, SMBIOS_STRING_MAX_LENGTH);
  if (LocationStrLen >= SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  DeviceNameStr = (CHAR8 *) PcdGetPtr (PcdSmbiosTablesSystemPowerSupplyDeviceName);
  if (DeviceNameStr == NULL) {
    return EFI_UNSUPPORTED;
  }
  DeviceNameStrLen = AsciiStrnLenS (DeviceNameStr, SMBIOS_STRING_MAX_LENGTH);
  if (DeviceNameStrLen >= SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  ManufacturerStr = (CHAR8 *) PcdGetPtr (PcdSmbiosTablesSystemPowerSupplyManufacturer);
  if (ManufacturerStr == NULL) {
    return EFI_UNSUPPORTED;
  }
  ManufacturerStrLen = AsciiStrnLenS (ManufacturerStr, SMBIOS_STRING_MAX_LENGTH);
  if (ManufacturerStrLen >= SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  SerialNumberStr = (CHAR8 *) PcdGetPtr (PcdSmbiosTablesSystemPowerSupplySerialNumber);
  if (SerialNumberStr == NULL) {
    return EFI_UNSUPPORTED;
  }
  SerialNumberStrLen = AsciiStrnLenS (SerialNumberStr, SMBIOS_STRING_MAX_LENGTH);
  if (SerialNumberStrLen >= SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  AssetTagStr = (CHAR8 *) PcdGetPtr (PcdSmbiosTablesSystemPowerSupplyAssetTagNumber);
  if (AssetTagStr == NULL) {
    return EFI_UNSUPPORTED;
  }
  AssetTagStrLen = AsciiStrnLenS (AssetTagStr, SMBIOS_STRING_MAX_LENGTH);
  if (AssetTagStrLen >= SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  ModelPartStr = (CHAR8 *) PcdGetPtr (PcdSmbiosTablesSystemPowerSupplyModelPartNumber);
  if (ModelPartStr == NULL) {
    return EFI_UNSUPPORTED;
  }
  ModelPartStrLen = AsciiStrnLenS (ModelPartStr, SMBIOS_STRING_MAX_LENGTH);
  if (ModelPartStrLen >= SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  RevisionLevelStr = (CHAR8 *) PcdGetPtr (PcdSmbiosTablesSystemPowerSupplyRevisionLevel);
  if (RevisionLevelStr == NULL) {
    return EFI_UNSUPPORTED;
  }
  RevisionLevelStrLen = AsciiStrnLenS (RevisionLevelStr, SMBIOS_STRING_MAX_LENGTH);
  if (RevisionLevelStrLen >= SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  //
  // Allocate space for the table plus optional strings. Each string ends with
  // a null, with another null after the last string.
  //
  TotalSize = sizeof (SMBIOS_TABLE_TYPE39) +
              LocationStrLen + 1 +
              DeviceNameStrLen + 1 +
              ManufacturerStrLen + 1 +
              SerialNumberStrLen + 1 +
              AssetTagStrLen + 1 +
              ModelPartStrLen + 1 +
              RevisionLevelStrLen + 1 + 1;
  SmbiosRecord = AllocateZeroPool (TotalSize);
  ASSERT (SmbiosRecord != NULL);
  if (SmbiosRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_SYSTEM_POWER_SUPPLY;
  SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE39);
  SmbiosRecord->Hdr.Handle = 0;         // filled by SMBIOS protocol
  SmbiosRecord->PowerUnitGroup = 1;
  SmbiosRecord->Location = 1;           // 1st optional string
  SmbiosRecord->DeviceName = 2;         // 2nd optional string
  SmbiosRecord->Manufacturer = 3;       // 3rd optional string
  SmbiosRecord->SerialNumber = 4;       // 4th optional string
  SmbiosRecord->AssetTagNumber = 5;     // 5th optional string
  SmbiosRecord->ModelPartNumber = 6;    // 6th optional string
  SmbiosRecord->RevisionLevel = 7;      // 7th optional string
  SmbiosRecord->MaxPowerCapacity  = UNKNOWN_VALUE;
  SmbiosRecord->PowerSupplyCharacteristics.PowerSupplyHotReplaceable = 0;
  SmbiosRecord->PowerSupplyCharacteristics.PowerSupplyPresent = 1;
  SmbiosRecord->PowerSupplyCharacteristics.PowerSupplyUnplugged = 0;
  SmbiosRecord->PowerSupplyCharacteristics.InputVoltageRangeSwitch = PowerSupplyInputVoltageRangeSwitchingUnknown;
  SmbiosRecord->PowerSupplyCharacteristics.PowerSupplyStatus = PowerSupplyStatusUnknown;
  SmbiosRecord->PowerSupplyCharacteristics.PowerSupplyType = PowerSupplyTypeUnknown;
  SmbiosRecord->InputVoltageProbeHandle = INPUT_VOLTAGE_PROBE_HANDLE;
  SmbiosRecord->CoolingDeviceHandle = COOLING_DEVICE_HANDLE;
  SmbiosRecord->InputCurrentProbeHandle = INPUT_CURRENT_PROBE_HANDLE;

  //
  // Copy the strings to the end.
  //
  OptionalStrStart = (CHAR8 *) (SmbiosRecord + 1);

  AsciiStrnCpyS (OptionalStrStart, SMBIOS_STRING_MAX_LENGTH, LocationStr, LocationStrLen);
  OptionalStrStart += LocationStrLen + 1;

  AsciiStrnCpyS (OptionalStrStart, SMBIOS_STRING_MAX_LENGTH, DeviceNameStr, DeviceNameStrLen);
  OptionalStrStart += DeviceNameStrLen + 1;

  AsciiStrnCpyS (OptionalStrStart, SMBIOS_STRING_MAX_LENGTH, ManufacturerStr, ManufacturerStrLen);
  OptionalStrStart += ManufacturerStrLen + 1;

  AsciiStrnCpyS (OptionalStrStart, SMBIOS_STRING_MAX_LENGTH, SerialNumberStr, SerialNumberStrLen);
  OptionalStrStart += SerialNumberStrLen + 1;

  AsciiStrnCpyS (OptionalStrStart, SMBIOS_STRING_MAX_LENGTH, AssetTagStr, AssetTagStrLen);
  OptionalStrStart += AssetTagStrLen + 1;

  AsciiStrnCpyS (OptionalStrStart, SMBIOS_STRING_MAX_LENGTH, ModelPartStr, ModelPartStrLen);
  OptionalStrStart += ModelPartStrLen + 1;

  AsciiStrnCpyS (OptionalStrStart, SMBIOS_STRING_MAX_LENGTH, RevisionLevelStr, RevisionLevelStrLen);

  Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);
  FreePool (SmbiosRecord);
  return Status;
}
