/** @file
  Header file for the SmbiosMisc Driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2018 Intel Corporation. <BR>

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

#ifndef _SMBIOS_MISC_H_
#define _SMBIOS_MISC_H_

#include <PiDxe.h>
#include <IndustryStandard/SmBios.h>
#include <Protocol/Smbios.h>

//
// Handles to request for specific tables, rather than letting the SMBIOS
// protocol choose, so they can be used to fill in fields in other tables
// without tracking what handles were assigned.
//
#define COOLING_DEVICE_HANDLE           0x1000
#define TEMPERATURE_PROBE_HANDLE        0x1001
#define MANAGEMENT_DEVICE_HANDLE        0x1002
#define THRESHOLD_HANDLE                0x1003

//
// Handles that are not applicable are filled with 0xFFFF in table fields.
//
#define INPUT_VOLTAGE_PROBE_HANDLE      0xFFFF  // no voltabe probe
#define INPUT_CURRENT_PROBE_HANDLE      0xFFFF  // no current probe

//
// Many SMBIOS structure fields use 0x8000 to indicate the value is unknown.
//
#define UNKNOWN_VALUE                   0x8000

//
// IPMI field values.
//
#define IPMI_SPECIFICATION_REVISION     0x20  // revision 2.0
#define IPMI_I2C_SLAVE_ADDRESS          0x20
#define IPMI_NV_STORAGE_DEVICE_ADDRESS  0xFF  // no NV storage
#define IPMI_BASE_ADDRESS               0xCA3

//
// Type 14 itemtype defination
//
#define FIRMWARE_VERSION_INFO_GROUP_NAME    "Firmware Version Info"

//
// SMBIOS Type 13 (Language Information Structure) defines a flag
// attribute that determines the format of the Current Language
// string. These are its possible values.
//
typedef enum {
  LanguageFormatLong = 0,
  LanguageFormatAbbreviated = 1,
} EFI_SMBIOS_TYPE_13_FLAG;

typedef enum {
  CoolingDeviceTypeOther = 1,
  CoolingDeviceTypeUnknown = 2,
  CoolingDeviceTypeFan = 3,
  CoolingDeviceTypeCentrifugalBlower = 4,
  CoolingDeviceTypeChipFan = 5,
  CoolingDeviceTypeCabinetFan = 6,
  CoolingDeviceTypePowerSupplyFan = 7,
  CoolingDeviceTypeHeatPipe = 8,
  CoolingDeviceTypeIntegratedRefrigeration = 9,
  CoolingDeviceTypeActiveCooling = 16,
  CoolingDeviceTypePassiveCooling = 17
} COOLING_DEVICE_TYPE;

typedef enum {
  CoolingDeviceStatusOther = 1,
  CoolingDeviceStatusUnknown = 2,
  CoolingDeviceStatusOk = 3,
  CoolingDeviceStatusNonCritical = 4,
  CoolingDeviceStatusCritical = 5,
  CoolingDeviceStatusNonRecoverable = 6
} COOLING_DEVICE_STATUS;

typedef enum {
  TemperatureProbeSiteOther = 1,
  TemperatureProbeSiteUnknown = 2,
  TemperatureProbeSiteProcessor = 3,
  TemperatureProbeSiteDisk = 4,
  TemperatureProbeSitePeripheralBay = 5,
  TemperatureProbeSiteSystemManagementModule = 6,
  TemperatureProbeSiteMotherboard = 7,
  TemperatureProbeSiteMemoryModule = 8,
  TemperatureProbeSiteProcessorModule = 9,
  TemperatureProbeSitePowerUnit = 10,
  TemperatureProbeSiteAddInCard = 11
} TEMPERATURE_PROBE_SITE;

typedef enum {
  TemperatureProbeStatusOther = 1,
  TemperatureProbeStatusUnknown = 2,
  TemperatureProbeStatusOk = 3,
  TemperatureProbeStatusNonCritical = 4,
  TemperatureProbeStatusCritical = 5,
  TemperatureProbeStatusNonRecoverable = 6
} TEMPERATURE_PROBE_STATUS;

typedef enum {
  PowerSupplyTypeOther = 1,
  PowerSupplyTypeUnknown = 2,
  PowerSupplyTypeLinear = 3,
  PowerSupplyTypeSwitching = 4,
  PowerSupplyTypeBattery = 5,
  PowerSupplyTypeUps = 6,
  PowerSupplyTypeConverter = 7,
  PowerSupplyTypeRegulator = 8
} POWER_SUPPLY_TYPE;

typedef enum {
  PowerSupplyStatusOther = 1,
  PowerSupplyStatusUnknown = 2,
  PowerSupplyStatusOk = 3,
  PowerSupplyStatusNonCritical = 4,
  PowerSupplyStatusCritical = 5
} POWER_SUPPLY_STATUS;

typedef enum {
  PowerSupplyInputVoltageRangeSwitchingOther = 1,
  PowerSupplyInputVoltageRangeSwitchingUnknown = 2,
  PowerSupplyInputVoltageRangeSwitchingManual = 3,
  PowerSupplyInputVoltageRangeSwitchingAutoSwitch = 4,
  PowerSupplyInputVoltageRangeSwitchingWideRange = 5,
  PowerSupplyInputVoltageRangeSwitchingNotApplicable = 6
} POWER_SUPPLY_INPUT_VOLTAGE_RANGE_SWITCHING;

/**
  Add an SMBIOS record.

  @param  Smbios                The EFI_SMBIOS_PROTOCOL instance.
  @param  SmbiosHandle          A unique handle will be assigned to the SMBIOS record.
  @param  Record                The data for the fixed portion of the SMBIOS record. The format of the record is
                                determined by EFI_SMBIOS_TABLE_HEADER.Type. The size of the formatted area is defined
                                by EFI_SMBIOS_TABLE_HEADER.Length and either followed by a double-null (0x0000) or
                                a set of null terminated strings and a null.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_OUT_OF_RESOURCES  Record was not added due to lack of system resources.
**/
EFI_STATUS
AddSmbiosRecord (
  IN EFI_SMBIOS_PROTOCOL        *Smbios,
  OUT EFI_SMBIOS_HANDLE         *SmbiosHandle,
  IN EFI_SMBIOS_TABLE_HEADER    *Record
  );

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
  );

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
  );

/**
  Add the Base Board (or Module) Information structure (Type 2) to SMBIOS.

  @param[in]  Smbios              A pointer to the SMBIOS protocol.

  @retval EFI_SUCCESS             The structure was added to SMBIOS.
  @retval EFI_INVALID_PARAMETER   An input pointer was null on entry.
  @retval EFI_UNSUPPORTED         A required PCD was missing or malformed.
  @retval EFI_OUT_OF_RESOURCES    The required memory could not be allocated.
**/
EFI_STATUS
AddBaseBoardInformation (
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  );

/**
  Add the System Enclosure or Chassis structure (Type 3) to SMBIOS.

  @param[in]  Smbios              A pointer to the SMBIOS protocol.

  @retval EFI_SUCCESS             The structure was added to SMBIOS.
  @retval EFI_INVALID_PARAMETER   An input pointer was null on entry.
  @retval EFI_UNSUPPORTED         A required PCD was missing or malformed.
  @retval EFI_OUT_OF_RESOURCES    The required memory could not be allocated.
**/
EFI_STATUS
AddChassisInformation (
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  );

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
  );

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
  );

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
  );

/**
  Add the BIOS Language Information structure (Type 14) to SMBIOS.

  @param[in]  Smbios              A pointer to the SMBIOS protocol.

  @retval EFI_SUCCESS             The structure was added to SMBIOS.
  @retval EFI_INVALID_PARAMETER   An input pointer was null on entry.
  @retval EFI_OUT_OF_RESOURCES    The required memory could not be allocated.
**/
EFI_STATUS
FirmwareVersionInfo (
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  );

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
  );

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
  );

/**
  Add the System Boot Information structure (Type 32) to SMBIOS.

  @param[in] Smbios               A pointer to the SMBIOS protocol.

  @retval EFI_SUCCESS             The structure was added to SMBIOS.
  @retval EFI_INVALID_PARAMETER   An input pointer was null on entry.
  @retval EFI_OUT_OF_RESOURCES    The required memory could not be allocated.
**/
EFI_STATUS
AddBootInfoStatus (
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  );

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
  );

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
  );

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
  );

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
  );

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
  );

#endif  // _SMBIOS_MISC_H_
