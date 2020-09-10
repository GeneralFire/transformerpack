/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#ifndef _SPS_SMM_HMRFPO_EXT_PROTOCOL_H_
#define _SPS_SMM_HMRFPO_EXT_PROTOCOL_H_

#include <Sps.h>

extern EFI_GUID gSpsSmmHmrfpoExtProtocolGuid;

typedef enum {
  HmrfpoExtLockType0,    // Full ME Region lock without region validation
  HmrfpoExtLockType1,    // Global lock of Fail-safe FPT directory partition via WPR0
  HmrfpoExtLockType2,    // Global lock of Fail-safe FPT directory partitions (FPT, MFS, RCV)
  HmrfpoExtLockTypeMax   // LockType Limit
} HMRFPO_EXT_LOCK_TYPE;

typedef enum {
  HmrfpoExtStatusDisabled               = 0x00,
  HmrfpoExtStatusLocked                 = 0x01,
  HmrfpoExtStatusEnabled                = 0x02,
  HmrfpoExtStatusUpdateInProgressLock   = 0x03
} HMRFPO_EXT_STATUS;

typedef enum {
  HmrfpoExtEnableRegionIntelMe   = 0x00
} HMRFPO_EXT_REGION_SELECT;

typedef enum {
  HmrfpoExtLegacyReset           = 0x00,
  HmrfpoExtFactoryDefaultReset   = 0x02
} HMRFPO_EXT_RESET_TYPE;


typedef EFI_STATUS (EFIAPI *HMRFPO_EXT_LOCK_FUNC) (
  VOID
  );

typedef EFI_STATUS (EFIAPI *HMRFPO_EXT_ENABLE_FUNC) (
  IN  HMRFPO_EXT_REGION_SELECT  RegionSelect,
  OUT UINT32                    *FactoryBase,
  OUT UINT32                    *FactoryLimit
  );

typedef EFI_STATUS (EFIAPI *HMRFPO_EXT_MERESET_FUNC) (
  IN HMRFPO_EXT_RESET_TYPE  ResetType
  );

typedef EFI_STATUS (EFIAPI *HMRFPO_EXT_LOCK_IMMEDIATELY_FUNC) (
  IN HMRFPO_EXT_LOCK_TYPE Type
  );

typedef EFI_STATUS (EFIAPI *HMRFPO_EXT_GET_STATUS_FUNC) (
  OUT HMRFPO_EXT_STATUS *ExtStatus
  );

typedef struct {

  MKHI_FW_VERSION   ActiveFwVersion;     // Active operational firmware
  MKHI_FW_VERSION   RecoveryFwVersion;   // Recovery firmware
  MKHI_FW_VERSION   BackupFwVersion;     // Backup operational firmware (optional)

  HMRFPO_EXT_LOCK_FUNC              Lock;
  HMRFPO_EXT_ENABLE_FUNC            Enable;
  HMRFPO_EXT_MERESET_FUNC           MeReset;
  HMRFPO_EXT_LOCK_IMMEDIATELY_FUNC  LockImmediately;
  HMRFPO_EXT_GET_STATUS_FUNC        GetStatus;

} SPS_SMM_HMRFPO_EXT_PROTOCOL;

#endif // _SPS_SMM_HMRFPO_EXT_PROTOCOL_H_
