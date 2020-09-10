/** @file
  This file describes the contents of the ACPI High Precision Event Timer Description Table
  HPET).  Some additional ACPI values are defined in Acpi1_0.h, Acpi2_0.h, and Acpi3_0.h
  All changes to the HPET contents should be done in this file.

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

#ifndef _HPET_H_
#define _HPET_H_

//
// Statements that include other files
//
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/HighPrecisionEventTimerTable.h>

//
// HPET Definitions
//
#define EFI_ACPI_OEM_HPET_REVISION    0x00000001

#define EFI_ACPI_EVENT_TIMER_BLOCK_ID 0x8086A701

//
// Event Timer Block Base Address Information
//
#define EFI_ACPI_EVENT_TIMER_BLOCK_ADDRESS_SPACE_ID EFI_ACPI_6_2_SYSTEM_MEMORY
// APTIOV_SERVER_OVERRIDE_RC_START : Register Bit Width is 64b
#define EFI_ACPI_EVENT_TIMER_BLOCK_BIT_WIDTH        0x40
// APTIOV_SERVER_OVERRIDE_RC_END : Register Bit Width is 64b
#define EFI_ACPI_EVENT_TIMER_BLOCK_BIT_OFFSET       0x00
#define EFI_ACPI_EVENT_TIMER_ACCESS_SIZE            0x00
#define EFI_ACPI_EVENT_TIMER_BLOCK_ADDRESS          0x00000000FED00000

#define EFI_ACPI_HPET_NUMBER                        0x00

// APTIOV_SERVER_OVERRIDE_RC_START : HPET timer accuracy, the main counter is clocked by the 14.31818 MHz clock.
#define EFI_ACPI_MIN_CLOCK_TICK                     14318
// APTIOV_SERVER_OVERRIDE_RC_END : HPET timer accuracy, the main counter is clocked by the 14.31818 MHz clock.

#define EFI_ACPI_HPET_ATTRIBUTES                    0x00

#endif
