/** @file
  This protocol produces OsWdtPolicy Protocol.

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

#ifndef _OS_WDT_POLICY_H_
#define _OS_WDT_POLICY_H_

typedef struct _EFI_OS_WDT_POLICY_PROTOCOL  EFI_OS_WDT_POLICY_PROTOCOL;

typedef struct {
  UINT8       OsWdtAction;
  UINTN       OsWdtTimeOut;
  BOOLEAN     OsWdtFlag;
} EFI_OS_WDT_POLICY_RECORD;

typedef
EFI_STATUS
(EFIAPI *EFI_OS_WDT_SET_POLICY) (
  IN EFI_OS_WDT_POLICY_PROTOCOL   *This,
  IN EFI_OS_WDT_POLICY_RECORD     *PolicyRecord
  );

typedef
EFI_STATUS
(EFIAPI *EFI_OS_WDT_GET_POLICY) (
  IN EFI_OS_WDT_POLICY_PROTOCOL     *This,
  IN OUT EFI_OS_WDT_POLICY_RECORD   *PolicyRecord
  );


struct _EFI_OS_WDT_POLICY_PROTOCOL {
  EFI_OS_WDT_SET_POLICY         OsWdtSetPolicy;
  EFI_OS_WDT_GET_POLICY         OsWdtGetPolicy;
};

extern EFI_GUID   gEfiOsWdtPolicyProtocolGuid;

#endif
