/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2008 - 2013 Intel Corporation. <BR>

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

#ifndef _PLATFORM_ACPI_UPDATE_LIB_H
#define _PLATFORM_ACPI_UPDATE_LIB_H

#include <Base.h>
#include <Uefi.h>

#define PLATFORM_ACPI_FIX_UPDATE_SIGNATURE  SIGNATURE_32 ('A', 'C', 'P', 'F')
#define PLATFORM_ACPI_FIX_UPDATE_VERSION    01


// {81129EF8-391D-4f63-AE99-58517EC077E3}
#define   PLATFORM_ACPI_FIX_TABLE_GUID \
{ 0x81129ef8, 0x391d, 0x4f63, { 0xae, 0x99, 0x58, 0x51, 0x7e, 0xc0, 0x77, 0xe3 } }

typedef struct {
  UINT32                  Signature;
  UINT32                  Version;

  VOID                    *TablePtr;

} ACPI_FIX_UPDATE_TABLE;

EFI_STATUS
PlatformGetAcpiFixTableDataPointer (
  IN  VOID                          **TablePtr
);

STATIC  EFI_GUID gPlatformAcpiFixTableGuid = PLATFORM_ACPI_FIX_TABLE_GUID;

#endif //_PLATFORM_ACPI_UPDATE_LIB_H
