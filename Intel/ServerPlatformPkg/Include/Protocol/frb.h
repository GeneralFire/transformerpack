/** @file
  This code abstracts FRB Protocol.

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

#ifndef _EFI_FRB_H_
#define _EFI_FRB_H_

#include "Guid/FrbGuid.h"

typedef struct _EFI_SM_FRB_PROTOCOL EFI_SM_FRB_PROTOCOL;

typedef struct _EFI_FRB_STAT {
  BOOLEAN                       FrbEnabled;
  BOOLEAN                       FrbFailed;
  UINTN                         FrbTimeout;
  FRB_SYSTEM_BOOT_POLICY        FrbCurrentBootPolicy;
  FRB_SYSTEM_TIMEOUT_INTERRUPT  FrbTimeoutInterrupt;
} EFI_FRB_STAT;

//
//  FRB Protocol APIs
//
typedef
EFI_STATUS
(EFIAPI *EFI_GET_FRB_STAT) (
  IN EFI_SM_FRB_PROTOCOL                * This,
  IN EFI_FRB_TYPE                       FrbType,
  OUT EFI_FRB_STAT                      * FrbStatData
  );

typedef
EFI_STATUS
(EFIAPI *EFI_SET_FRB_TIMER) (
  IN EFI_SM_FRB_PROTOCOL                * This,
  IN EFI_FRB_TYPE                       FrbType,
  IN UINTN                              *Timeout
  );

typedef
EFI_STATUS
(EFIAPI *EFI_SET_FRB_PRE_TIMEOUT_POLICY) (
  IN EFI_SM_FRB_PROTOCOL                * This,
  IN EFI_FRB_TYPE                       FrbType,
  IN UINTN                              *PreTimeoutInterval,
  IN FRB_SYSTEM_TIMEOUT_INTERRUPT       FrbTimeoutInterrupt
  );

typedef
EFI_STATUS
(EFIAPI *EFI_SET_FRB_BOOT_POLICY) (
  IN EFI_SM_FRB_PROTOCOL                * This,
  IN EFI_FRB_TYPE                       FrbType,
  IN FRB_SYSTEM_BOOT_POLICY             FrbBootPolicy
  );

typedef
EFI_STATUS
(EFIAPI *EFI_ENABLE_FRB) (
  IN EFI_SM_FRB_PROTOCOL                * This,
  IN EFI_FRB_TYPE                       FrbType,
  IN BOOLEAN                            UseResidualCount,
  IN BOOLEAN                            StartFrbCountDown,
  IN BOOLEAN                            ClearFrbGlobalStatus
  );

typedef
EFI_STATUS
(EFIAPI *EFI_DISABLE_FRB) (
  IN EFI_SM_FRB_PROTOCOL                * This,
  IN EFI_FRB_TYPE                       FrbType
  );

//
// FRB PROTOCOL
//
typedef struct _EFI_SM_FRB_PROTOCOL {
  EFI_GET_FRB_STAT                GetFrbStatData;
  EFI_SET_FRB_TIMER               SetFrbTimerData;
  EFI_SET_FRB_PRE_TIMEOUT_POLICY  SetFrbPreTimeoutPolicy;
  EFI_SET_FRB_BOOT_POLICY         SetFrbBootPolicy;
  EFI_ENABLE_FRB                  EnableFrb;
  EFI_DISABLE_FRB                 DisableFrb;
} EFI_SM_FRB_PROTOCOL;

extern EFI_GUID gEfiFrbCodeProtocolGuid;

#endif
