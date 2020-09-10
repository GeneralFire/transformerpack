/** @file
  This protocol produces BmcAcpiSwChildPolicy Protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2018 Intel Corporation. <BR>

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

#ifndef _BMC_ACPI_SW_CHILD_POLICY_H_
#define _BMC_ACPI_SW_CHILD_POLICY_H_

typedef struct _EFI_BMC_ACPI_SW_CHILD_POLICY_PROTOCOL EFI_BMC_ACPI_SW_CHILD_POLICY_PROTOCOL;

#define EFI_BMC_ACPI_SW_CHILD_POLICY_PROTOCOL_GUID \
  { 0x89843c0b, 0x5701, 0x4ff6, 0xa4, 0x73, 0x65, 0x75, 0x99, 0x04, 0xf7, 0x35 }

typedef
EFI_STATUS
(EFIAPI *EFI_SET_ACPI_POWER_STATE_IN_BMC) (
  IN EFI_BMC_ACPI_SW_CHILD_POLICY_PROTOCOL   *This,
  IN UINT8                                   PowerState,
  IN UINT8                                   DeviceState
  );

struct _EFI_BMC_ACPI_SW_CHILD_POLICY_PROTOCOL {
  EFI_SET_ACPI_POWER_STATE_IN_BMC            SetACPIPowerStateInBMC;
};

extern EFI_GUID   gEfiBmcAcpiSwChildPolicyProtocolGuid;

#endif
