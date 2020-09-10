/** @file
  ACPI Static resource definition table implementation, defined at
  http://microsoft.com/hwdev/design/srat.htm.

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

#ifndef _SRAT_H_
#define _SRAT_H_

//
// Statements that include other files
//
#include <IndustryStandard/Acpi.h>

//
#define EFI_ACPI_OEM_SRAT_REVISION  0x00000002  //

//
// TBD :Backward Compatibility per ACPI 3.0. Required by Hyper-V. OS's ok so far as of 5/27/09
//
#define EFI_ACPI_SRAT_RESERVED_FOR_BACKWARD_COMPATIBILITY  0x00000001
//
// Define the number of each table type.
// This is where the table layout is modified.
//
#define PROCESSOR_LOCAL_APIC_SAPIC_AFFINITY_STRUCTURE_COUNT   MAX_CPU_NUM
#define MEMORY_AFFINITY_STRUCTURE_COUNT                       MC_MAX_NODE*MAX_CRS_ENTRIES_PER_NODE
#define X2APIC_AFFINITY_STRUCTURE_COUNT                       MAX_CPU_NUM
//
// Statis Resource Affinity Table header definition.  The table
// must be defined in a platform specific manner.
//
//
// Ensure proper structure formats
//
#pragma pack(1)

typedef struct {
  EFI_ACPI_6_2_SYSTEM_RESOURCE_AFFINITY_TABLE_HEADER          *SratHeader;

  EFI_ACPI_6_2_PROCESSOR_LOCAL_APIC_SAPIC_AFFINITY_STRUCTURE  *Apic;
  EFI_ACPI_6_2_MEMORY_AFFINITY_STRUCTURE                      *Memory;
  EFI_ACPI_6_2_PROCESSOR_LOCAL_X2APIC_AFFINITY_STRUCTURE      *x2Apic;

} STATIC_RESOURCE_AFFINITY_TABLE;

#pragma pack()

#endif
