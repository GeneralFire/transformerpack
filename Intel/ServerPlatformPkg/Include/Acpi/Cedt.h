/** @file
  This file describes the contents of the CEDT ACPI table.

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

#ifndef _CEDT_H_
#define _CEDT_H_

//
// Statements that include other files
//
#include <IndustryStandard/Acpi.h>
#include <IioRegs.h>

//
// Macro for ACPI CEDT header
//
#define EFI_ACPI_CEDT_TABLE_SIGNATURE               SIGNATURE_32('C','E','D','T')
#define EFI_ACPI_CEDT_DESCRIPTION_TABLE_REVISION    0x01
#define EFI_ACPI_CEDT_OEM_REVISION                  0x00000000
#define EFI_ACPI_CEDT_CREATOR_ID                    SIGNATURE_32 ('I', 'N', 'T', 'L')
#define EFI_ACPI_CEDT_CREATOR_REVISION              0x00000001

//
// Macro for ACPI CHBS structure
//
#define EFI_ACPI_CXL_HOST_BRIDGE_STRUCTURE_TYPE     0x00
#define EFI_ACPI_CXL_HOST_BRIDGE_STRUCTURE_LENGTH   0x18
#define EFI_ACPI_CXL_SPECIFICATION_REVISION_1P1     0x00
#define EFI_ACPI_CXL_SPECIFICATION_REVISION_2P0     0x01
#define EFI_ACPI_CXL_1P1_RCRB_SIZE                  0x2000
#define EFI_ACPI_CXL_2P0_RCRB_SIZE                  0x10000

//
// ACPI CEDT structure
//
#pragma pack(1)

typedef struct {
  UINT8   Type;
  UINT8   Rsvd;
  UINT16  Length;
  UINT32  Uid;
  UINT32  Version;
  UINT32  RcrbSize;
  UINT64  RcrbBase;
} EFI_ACPI_CXL_HOST_BRIDGE_STRUCTURE;

typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER             Header;
  EFI_ACPI_CXL_HOST_BRIDGE_STRUCTURE      CxlHostBridgeStructure[MAX_SOCKET * MAX_IOU_PER_SOCKET];
} EFI_ACPI_CEDT_DESCRIPTION_TABLE;

#pragma pack()

#endif

