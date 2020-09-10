/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2019 Intel Corporation. <BR>

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

#ifndef _SPS_SMM_HMRFPO_PROTOCOL_H_
#define _SPS_SMM_HMRFPO_PROTOCOL_H_

extern EFI_GUID gSpsSmmHmrfpoProtocolGuid;

typedef struct _SPS_SMM_HMRFPO_PROTOCOL_ _SPS_SMM_HMRFPO_PROTOCOL;

typedef EFI_STATUS (EFIAPI *HMRFPO_LOCK_FUNC)(VOID);

#define HMRFPO_EN_REGION_INTEL_ME     0x0

typedef EFI_STATUS (EFIAPI *HMRFPO_ENABLE_FUNC)(UINT32);

typedef EFI_STATUS (EFIAPI *HMRFPO_MERESET_FUNC)(VOID);
// APTIOV_SERVER_OVERRIDE_RC_START : Provide Nonce in SpsSmm.
typedef UINT64 (EFIAPI *HMRFPO_GET_NONCE)(void);
// APTIOV_SERVER_OVERRIDE_RC_END : Provide Nonce in SpsSmm.

typedef struct
{
  UINT16 Minor;
  UINT16 Major;
  UINT16 Build;
  UINT16 Patch;
} ME_FW_VERSION;

typedef struct _SPS_SMM_HMRFPO_PROTOCOL_ {

  ME_FW_VERSION   ActiveFwVersion;     // Active operational firmware
  ME_FW_VERSION   RecoveryFwVersion;   // Recovery firmware
  ME_FW_VERSION   BackupFwVersion;     // Backup operational firmware (optional)

  HMRFPO_LOCK_FUNC     Lock;
  HMRFPO_ENABLE_FUNC   Enable;
  HMRFPO_MERESET_FUNC  MeReset;
  // APTIOV_SERVER_OVERRIDE_RC_START : Provide Nonce in SpsSmm.
   HMRFPO_GET_NONCE     GetNonce;
  // APTIOV_SERVER_OVERRIDE_RC_END : Provide Nonce in SpsSmm.

} SPS_SMM_HMRFPO_PROTOCOL;

#endif // _SPS_SMM_HMRFPO_PROTOCOL_H_

