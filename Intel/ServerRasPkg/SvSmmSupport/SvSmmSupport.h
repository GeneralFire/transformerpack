/** @file
  SV SMM Driver implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2008 - 2018 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

#ifndef _SV_SMM_SUPPORT_H
#define _SV_SMM_SUPPORT_H


typedef struct _SVOS_ACPI_DATA {
  UINTN               AcpiGasAddress;
  UINT8               SmiDoorBell;
} SVOS_ACPI_DATA;

typedef struct _SV_SMM_SERVICE_DATA {
  UINTN               SvSmmExtenderAddress;
} SV_SMM_SERVICE_DATA;

typedef struct _SV_COMMUNICATION_DATA {
  SV_SMM_SERVICE_DATA SmmData;
  SVOS_ACPI_DATA      AcpiData;
} SV_COMMUNICATION_DATA;


VOID
EFIAPI
GetSvosGasAddress (
  OUT UINTN         *Address
  );

VOID
EFIAPI
SetSvosGasAddress (
  IN UINTN          Address
  );

VOID
EFIAPI
GetSvosSmiDoorBell (
  OUT UINT8         *SmiDoorBell
  );

VOID
EFIAPI
SetSvosSmiDoorBell (
  IN UINT8          SmiDoorBell
  );

#endif
