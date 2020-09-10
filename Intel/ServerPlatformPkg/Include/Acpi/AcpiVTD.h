/** @file
  This file describes the contents of the VTD ACPI Support

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2020 Intel Corporation. <BR>

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

#ifndef _ACPI_VTD_H
#define _ACPI_VTD_H

//
// Statements that include other files
//
#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/DevicePathLib.h>
#include <Library/HobLib.h>
#include <Acpi/DMARemappingReportingTable.h>
#include <IndustryStandard/Acpi.h>
#include <Protocol/DmaRemap.h>
#include <Guid/HobList.h>
#include <Guid/PlatformInfo.h>
#include <Guid/SocketVariable.h>

#define SEGMENT0                        0x00
#define SEGMENT1                        0x01

//
// equates used in DMAR Table.
//
#define EFI_ACPI_DMAR_OEMID                 "INTEL "    // OEM ID - 6 Bytes
#define EFI_ACPI_DMAR_OEM_TABLEID           "INTEL ID"  // OEM Table ID - 8 Bytes
#define EFI_ACPI_DMAR_OEM_REVISION          0x01
#define EFI_ACPI_DMAR_OEM_CREATOR_ID        0x01
#define EFI_ACPI_DMAR_OEM_CREATOR_REVISION  0x01

//EDK2_TODO Compiler is reporting error with below statement.
//Need to resovle later.
//#pragma pack(1)
// APTIOV_SERVER_OVERRIDE_RC_START: Increasing Table Size for 8S Configuration
#if (MAX_SOCKET > 4)
#define TABLE_SIZE                      5 * 1024
#else
#define TABLE_SIZE                      4 * 1024
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: Increasing Table Size for 8S Configuration

typedef struct {
  UINT32                                Signature;
  EFI_HANDLE                            Handle;
  EFI_DMA_REMAP_PROTOCOL                DmaRemapProt;
  EFI_ACPI_DMAR_DESCRIPTION_TABLE       *Dmar;
  EFI_ACPI_DMAR_DESCRIPTION_TABLE       *DmarOrder;
} EFI_VTD_SUPPORT_INSTANCE;

//EDK2_TODO Compiler is reporting error with below statement.
//Need to resovle later.

#endif
