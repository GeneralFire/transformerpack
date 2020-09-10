/** @file
  ACPI Watchdog Description Table as defined in Intel
  ICH Family Watchdog Timer (WDT) Application Note (AP-725)

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#ifndef _WATCH_DOG_DESCRIPTION_TABLE_H_
#define _WATCH_DOG_DESCRIPTION_TABLE_H_

//
// Include files
//
#include <IndustryStandard/Acpi.h>

//
// Ensure proper structure formats
//
#pragma pack(1)
//
// WDDT structure ACPI 6.2 compliant
//
typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER             Header;
  UINT16                                  SpecVersion;
  UINT16                                  TableVersion;
  UINT16                                  Vid;
  EFI_ACPI_6_2_GENERIC_ADDRESS_STRUCTURE  BaseAddress;
  UINT16                                  TimerMaxCount;
  UINT16                                  TimerMinCount;
  UINT16                                  TimerCountPeriod;
  UINT16                                  Status;
  UINT16                                  Capability;
} EFI_ACPI_WATCH_DOG_DESCRIPTION_TABLE;

//
// "WDDT" Watchdog Description Table signatures
//
#define EFI_ACPI_WDDT_SIGNATURE 0x54444457

#pragma pack()

//
// WDDT Revision
//
#define EFI_ACPI_WATCHDOG_DESCRIPTION_1_0_TABLE_REVISION  0x01

//
// WDDT Spec Version
//
#define EFI_ACPI_WDDT_SPEC_1_0_VERSION                0x01

//
// WDDT Description Table Version
//
#define EFI_ACPI_WDDT_TABLE_1_0_VERSION               0x01

//
// WDT Status
//
#define EFI_ACPI_WDDT_STATUS_AVAILABLE             0x0001
#define EFI_ACPI_WDDT_STATUS_ACTIVE                0x0002
#define EFI_ACPI_WDDT_STATUS_OWNED_BY_BIOS         0x0000
#define EFI_ACPI_WDDT_STATUS_OWNED_BY_OS           0x0004
#define EFI_ACPI_WDDT_STATUS_USER_RESET_EVENT      0x0800
#define EFI_ACPI_WDDT_STATUS_WDT_EVENT             0x1000
#define EFI_ACPI_WDDT_STATUS_POWER_FAIL_EVENT      0x2000
#define EFI_ACPI_WDDT_STATUS_UNKNOWN_RESET_EVENT   0x4000

//
// WDT Capability
//
#define EFI_ACPI_WDDT_CAPABILITY_AUTO_RESET            0x0001
#define EFI_ACPI_WDDT_CAPABILITY_ALERT_SUPPORT         0x0002
#define EFI_ACPI_WDDT_CAPABILITY_PLATFORM_SHUTDOWN     0x0004
#define EFI_ACPI_WDDT_CAPABILITY_IMMEDIATE_SHUTDOWN    0x0008
#define EFI_ACPI_WDDT_CAPABILITY_BIOS_HANDOFF_SUPPORT  0x0010

#endif
