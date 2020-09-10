/** @file
  Header file for SV SMM access

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 Intel Corporation. <BR>

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

#ifndef _SVOS_SMM_PROTOCOL_H_
#define _SVOS_SMM_PROTOCOL_H_

//
// Includes
//

#define SVOS_SMM_GUID \
  { \
    0xf139bd77, 0x7ff7, 0x49d6, 0x90, 0x86, 0xd5, 0xa, 0xb2, 0x6f, 0x6d, 0xd7 \
  }

//
// Protocol Data Structures
//
typedef
EFI_STATUS
(EFIAPI *EFI_SV_EXTENDER) (
  VOID
);

typedef
VOID
(EFIAPI *EFI_SVOS_ACPI_SERVICE_GET_SVOS_GAS_ADDRESS) (
  OUT UINTN                               *Address
  );

typedef
VOID
(EFIAPI *EFI_SVOS_ACPI_SERVICE_SET_SVOS_GAS_ADDRESS) (
  IN UINTN                               Address
  );

typedef
VOID
(EFIAPI *EFI_SVOS_ACPI_SERVICE_GET_SVOS_SMI_DOORBELL) (
  OUT UINT8                               *SmiDoorBell
  );

typedef
VOID
(EFIAPI *EFI_SVOS_ACPI_SERVICE_SET_SVOS_SMI_DOORBELL) (
  IN UINT8                               SmiDoorBell
  );

typedef struct _EFI_SV_SMM_SERVICE_PROTOCOL {

  UINTN                         SvSmmExtenderAddress;
  EFI_SV_EXTENDER               SVExtenderHandler;

} EFI_SV_SMM_SERVICE_PROTOCOL;


typedef struct _EFI_SVOS_ACPI_SERVICE_PROTOCOL {


  EFI_SVOS_ACPI_SERVICE_GET_SVOS_GAS_ADDRESS  GetSvosGasAddress;
  EFI_SVOS_ACPI_SERVICE_SET_SVOS_GAS_ADDRESS  SetSvosGasAddress;
  EFI_SVOS_ACPI_SERVICE_GET_SVOS_SMI_DOORBELL GetSvosSmiDoorBell;
  EFI_SVOS_ACPI_SERVICE_SET_SVOS_SMI_DOORBELL SetSvosSmiDoorBell;

} EFI_SVOS_ACPI_SERVICE_PROTOCOL;

typedef struct _EFI_SV_SERVICE_PROTOCOL {

  EFI_SV_SMM_SERVICE_PROTOCOL     SmmService;

  EFI_SVOS_ACPI_SERVICE_PROTOCOL  AcpiService;

} EFI_SV_SERVICE_PROTOCOL;

extern EFI_GUID gEfiSvSmmProtocolGuid;

#endif
