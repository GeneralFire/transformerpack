/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef _BZM_DXE_H_
#define _BZM_DXE_H_

#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <IndustryStandard/Acpi.h>

///
/// "BZMI" Boot-time fast Zeroed Memory Interface ACPI Table Signature
///
#define BOOTTIME_ZEROED_MEMORY_INTERFACE_TABLE_SIGNATURE   SIGNATURE_32('B', 'Z', 'M', 'I')

///
/// BZMI Revision
///
#define BOOTTIME_ZEROED_MEMORY_INTERFACE_TABLE_REVISION    0x1

///
/// BZMI ACPI Table Structure Definition.
///
#pragma pack(1)
typedef struct {
  //
  // The common ACPI description table header.
  //
  EFI_ACPI_DESCRIPTION_HEADER Header;

  //
  // Number of BZM Domains.
  //
  UINT16                      DomainNumber;

  //
  // Reserved.
  //
  UINT16                      Reserved;

  //
  // BZM Domain 0 - MAX_SOCKET * MAX_DRAM_CLUSTERS Identifier.
  //   Bits [55:0]            BZM Domain-n Domain Handle.
  //   Bits [61:56]           Reserved.
  //
  UINT64                      DomainIdentifier[MAX_SOCKET * MAX_DRAM_CLUSTERS];
} BOOTTIME_ZEROED_MEMORY_INTERFACE;
#pragma pack()


/**
  This function programs MSR to enable the Boot-time fast Zero Memory
  support on all processors.

  @retval EFI_SUCCESS   This function programmed the MSR successfully.
  @retval Others        This function failed to program the MSR.

**/
EFI_STATUS
EnableBzmSupport (
  VOID
  );

/**
  This function installs the BZMI ACPI table.

  @retval EFI_SUCCESS     This function is executed successfully.
  @retval Others          This fuction failed to be executed.

**/
EFI_STATUS
InstallBzmiAcpiTable (
  VOID
  );

#endif  // _BZM_DXE_H_
