/** @file
  This file describes the contents of the ACPI Maximum System Characteristics Table (MSCT).
  Some additional ACPI values are defined in Acpi1_0.h, Acpi2_0.h, and Acpi3_0.h.
  All changes to the MSCT contents should be done in this file.

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

#ifndef _MSCT_H_
#define _MSCT_H_

//
// Statements that include other files
//
#include <IndustryStandard/Acpi.h>
#include <Platform.h>
#include <UncoreCommonIncludes.h>

//
// MSCT Definitions, see specification for details.
//
#ifndef EFI_ACPI_6_2_MAXIMUM_SYSTEM_CHARACTERISTICS_TABLE_SIGNATURE
#define EFI_ACPI_6_2_MAXIMUM_SYSTEM_CHARACTERISTICS_TABLE_SIGNATURE       0x5443534D
#endif
#define EFI_ACPI_MAXIMUM_SYSTEM_CHARACTERISTICS_TABLE_REVISION            0x01
#define EFI_ACPI_OEM_MSCT_REVISION                                        0x00000001
#define EFI_ACPI_MAXIMUM_PROXIMITY_DOMAIN_INFORMATION_STRUCTURE_REVISION  0x01


//
// MSCT Table definition
//
#pragma pack(1)

typedef struct {
  UINT8   Revision;
  UINT8   Length;
  UINT32  ProxDomRangeLow;
  UINT32  ProxDomRangeHigh;
  UINT32  MaxProcessorCapacity;
  UINT64  MaxMemoryCapacity;
} EFI_ACPI_MAXIMUM_PROXIMITY_DOMAIN_INFORMATION_STRUCTURE;

typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER                              Header;
  UINT32                                                   OffsetProxDomInfo;
  UINT32                                                   MaxNumProxDom;
  UINT32                                                   MaxNumClockDom;
  UINT64                                                   MaxPhysicalAddress;
  EFI_ACPI_MAXIMUM_PROXIMITY_DOMAIN_INFORMATION_STRUCTURE  ProxDomInfoStructure[MAX_SOCKET];
} EFI_ACPI_MAXIMUM_SYSTEM_CHARACTERISTICS_TABLE;

#pragma pack()

#endif  //_MSCT_H_
