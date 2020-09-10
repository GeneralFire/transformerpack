/** @file
  This file describes the contents of the MIGT ACPI table.

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

#ifndef _MIGT_H_
#define _MIGT_H_

//
// Statements that include other files
//
#include <IndustryStandard/Acpi.h>

//
// MIGT ACPI structure
//
typedef struct {

  EFI_ACPI_DESCRIPTION_HEADER             Header;
  // MIGT Specific Entries
  EFI_ACPI_6_2_GENERIC_ADDRESS_STRUCTURE  ControlRegister;
  UINT32                                  ControlRegisterValue;
  EFI_ACPI_6_2_GENERIC_ADDRESS_STRUCTURE  ActionRegion;

} EFI_MIGT_ACPI_DESCRIPTION_TABLE;


//
// MIGT ACPI Definitions
//
#define MIGT_SMI_SERVICE_ID                       0xFD        // Door Bell

#define EFI_MIGT_ACPI_TABLE_SIGNATURE             SIGNATURE_32('M','I','G','T')
#define EFI_MIGT_ACPI_DESCRIPTION_TABLE_REVISION  0x01
#define EFI_MIGT_ACPI_OEM_REVISION                0x00000000

//
// MIGT Control Register Generic Address Information
//
#define EFI_MIGT_CR_ACPI_ADDRESS_SPACE_ID         EFI_ACPI_6_2_SYSTEM_IO
#define EFI_MIGT_CR_ACPI_REGISTER_BIT_WIDTH       0x8
#define EFI_MIGT_CR_ACPI_REGISTER_BIT_OFFSET      0x0
#define EFI_MIGT_CR_ACPI_SMI_ADDRESS              0xB2

//
// MIGT Action Region Generic Address Information
//
#define EFI_MIGT_AR_ACPI_ADDRESS_SPACE_ID         EFI_ACPI_6_2_SYSTEM_MEMORY
#define EFI_MIGT_AR_ACPI_REGISTER_BIT_WIDTH       64
#define EFI_MIGT_AR_ACPI_REGISTER_BIT_OFFSET      0x0
#define EFI_MIGT_AR_ACPI_MEMORY_ADDRESS           0x0

#endif

