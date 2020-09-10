/** @file
  This file describes the contents of the ACPI Watchdog Description
  Table (WDDT).  Some additional ACPI values are defined in Acpi1_0.h and
  Acpi2_0.h.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2019 Intel Corporation. <BR>

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

#ifndef _WDDT_H
#define _WDDT_H

//
// Statements that include other files
//
#include <IndustryStandard/Acpi.h>

//
// WDDT Definitions, ACPI 6.2
//
#define EFI_ACPI_WDDT_DESCRIPTION_TABLE_REVISION      1
#define EFI_ACPI_OEM_WDDT_REVISION                    0x00000001
#define EFI_ACPI_WDDT_SPEC_VERSION                    0x0100
#define EFI_ACPI_WDDT_TABLE_VERSION                   0x0100
#define EFI_ACPI_WDDT_TIMER_MAX_COUNT                 0x003f
#define EFI_ACPI_WDDT_TIMER_MIN_VALUE                 0x0004
#define EFI_ACPI_WDDT_TIMER_PERIOD_COUNT              600
//
// WDDT_STATUS bits
//
#define EFI_ACPI_WDDT_STATUS_AVAILABLE            0x0001
#define EFI_ACPI_WDDT_STATUS_ACTIVE               0x0002
#define EFI_ACPI_WDDT_STATUS_OWNED_BY_BIOS        0x0000
#define EFI_ACPI_WDDT_STATUS_OWNED_BY_OS          0x0004
#define EFI_ACPI_WDDT_STATUS_USER_RESET_EVENT     0x0800
#define EFI_ACPI_WDDT_STATUS_WDT_EVENT            0x1000
#define EFI_ACPI_WDDT_STATUS_POWER_FAIL_EVENT     0x2000
#define EFI_ACPI_WDDT_STATUS_UNKNOWN_RESET_EVENT  0x4000

//
// WDDT Capability bits
//
#define EFI_ACPI_WDDT_CAPABILITY_AUTO_RESET           0x0001
#define EFI_ACPI_WDDT_CAPABILITY_ALERT_SUPPORT        0x0002
#define EFI_ACPI_WDDT_CAPABILITY_PLATFORM_SHUTDOWN    0x0004
#define EFI_ACPI_WDDT_CAPABILITY_IMMEDIATE_SHUTDOWN   0x0008
#define EFI_ACPI_WDDT_CAPABILITY_BIOS_HANDOFF_SUPPORT 0x0010

#pragma pack()

#endif
