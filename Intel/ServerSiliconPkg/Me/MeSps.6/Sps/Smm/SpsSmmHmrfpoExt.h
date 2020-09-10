/** @file
  Abstract:
  SPS SMM driver - HMRFPO extended protocol.

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

#ifndef _SPS_SMM_HMRFPO_EXT_H_
#define _SPS_SMM_HMRFPO_EXT_H_

#include <Protocol/SpsSmmHmrfpoExtProtocol.h>

#define HMRFPO_EXT_LOCK_FAIL     0x0
#define HMRFPO_EXT_LOCK_SUCCESS  0x1


#pragma pack (1)
typedef struct {
  MKHI_MESSAGE_HEADER Mkhi;
  UINT64              Nonce;
} HECI_MSG_HMRFPO_EXT_ENABLE_REQ;

typedef struct {
  MKHI_MESSAGE_HEADER Mkhi;
  UINT32              FactoryBase;
  UINT32              FactoryLimit;
  UINT8               Status;
  UINT8               Reserved[3];
} HECI_MSG_HMRFPO_EXT_ENABLE_RSP;

typedef struct {
  MKHI_MESSAGE_HEADER Mkhi;
  UINT64              Nonce;
} HECI_MSG_HMRFPO_EXT_MERESET_REQ;

typedef struct {
  MKHI_MESSAGE_HEADER Mkhi;
  UINT8               Status;
  UINT8               Reserved[3];
} HECI_MSG_HMRFPO_EXT_MERESET_RSP;

typedef struct {
  MKHI_MESSAGE_HEADER   Mkhi;
} HECI_MSG_HMRFPO_EXT_LOCK_REQ;

typedef struct {
  MKHI_MESSAGE_HEADER   Mkhi;
  UINT64                Nonce;
  UINT32                FactoryBase;
  UINT32                FactoryLimit;
  UINT8                 Status;
  UINT8                 Reserved[3];
} HECI_MSG_HMRFPO_EXT_LOCK_RSP;

typedef struct {
  MKHI_MESSAGE_HEADER   Mkhi;
  UINT8                 Type;
  UINT8                 Reserved[3];
} HECI_MSG_HMRFPO_EXT_LOCK_IMMEDIATELY_REQ;

typedef struct {
  MKHI_MESSAGE_HEADER   Mkhi;
  UINT8                 Status;
  UINT8                 Reserved[3];
} HECI_MSG_HMRFPO_EXT_LOCK_IMMEDIATELY_RSP;

typedef struct {
  MKHI_MESSAGE_HEADER   Mkhi;
} HECI_MSG_HMRFPO_EXT_GETSTATE_REQ;

typedef struct {
  MKHI_MESSAGE_HEADER   Mkhi;
  UINT8                 State;
  UINT8                 Reserved[3];
} HECI_MSG_HMRFPO_EXT_GETSTATE_RSP;

typedef struct {
  HECI_MSG_HMRFPO_EXT_LOCK_REQ Req;
  HECI_MSG_HMRFPO_EXT_LOCK_RSP Rsp;
} HECI_MSG_HMRFPO_EXT_LOCK;

typedef struct {
  HECI_MSG_HMRFPO_EXT_ENABLE_REQ Req;
  HECI_MSG_HMRFPO_EXT_ENABLE_RSP Rsp;
} HECI_MSG_HMRFPO_EXT_ENABLE;

typedef struct {
  HECI_MSG_HMRFPO_EXT_MERESET_REQ Req;
  HECI_MSG_HMRFPO_EXT_MERESET_RSP Rsp;
} HECI_MSG_HMRFPO_EXT_MERESET;

typedef struct {
  HECI_MSG_HMRFPO_EXT_LOCK_IMMEDIATELY_REQ Req;
  HECI_MSG_HMRFPO_EXT_LOCK_IMMEDIATELY_RSP Rsp;
} HECI_MSG_HMRFPO_EXT_LOCK_IMMEDIATELY;

typedef struct {
  HECI_MSG_HMRFPO_EXT_GETSTATE_REQ Req;
  HECI_MSG_HMRFPO_EXT_GETSTATE_RSP Rsp;
} HECI_MSG_HMRFPO_EXT_GETSTATE;
#pragma pack ()


/**
  Initialize HMRFPO Extended protocol.

  @param[in] pSpsSmmHmrfpo    Pointer to base HMRFPO protocol.

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_INVALID_PARAMETER  Pointer to Smm Hmrfpo protocol is Null.
**/
EFI_STATUS
InitializeHmrfpoExt (
  IN SPS_SMM_HMRFPO_PROTOCOL      *pSpsSmmHmrfpo
  );

/**
  Procedure to send HMRFPO_LOCK HECI Request

  @param[in,out] Nonce    buffer to store nonce value from HECI message response

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_DEVICE_ERROR       Cannot get MeFirmwareStatus
                                 or send or receive message or status of response are failed.
  @retval EFI_INVALID_PARAMETER  Pointer to Heci message is Null (from CallMsg_HmrfpoExtLock).
**/
EFI_STATUS
HeciReq_HmrfpoExtLock (
  IN OUT UINT64    *Nonce
  );

/**
  Procedure to send HMRFPO_ENABLE HECI Request

  Please note: After sending HmrfpoEnable message ME needs to reset itself
  - this means that this is highly probable that this function fails at first few tries.
  This is recommended to call this function in a loop a few times until success is returned.
  Please refer to ME-Bios spec. for more details.

  @param[in]    Nonce         Nonce value for current session
  @param[out]   FactoryBase   Pointer to the base address of the Intel ME factory data area.
                              Passing NULL pointer is legal.
  @param[out]   FactoryLimit  Pointer to the length of the Intel ME factory data area.
                              Passing NULL pointer is legal.

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_DEVICE_ERROR       Cannot get MeFirmwareStatus
                                 or send or receive message or status of response are failed.
  @retval EFI_INVALID_PARAMETER  Pointer to Heci message is Null (from CallMsg_HmrfpoExtEnable).
**/
EFI_STATUS
HeciReq_HmrfpoExtEnable (
  IN  UINT64        Nonce,
  OUT UINT32        *FactoryBase,
  OUT UINT32        *FactoryLimit
  );

/**
  Procedure to send HMRFPO_RESET HECI Request

  Please note: HmrfpoReset message make ME to reset itself
  this means that this is highly probable that this function return fail (timeout occurs).

  @param[in]  Nonce      Nonce value for current session

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_DEVICE_ERROR       Cannot get MeFirmwareStatus
                                 or send or receive message or status of response are failed.
  @retval EFI_INVALID_PARAMETER  Pointer to Heci message is Null (from CallMsg_HmrfpoExtMeReset).
**/
EFI_STATUS
HeciReq_HmrfpoExtMeReset (
  IN UINT64        Nonce
  );

/**
  Procedure to send HMRFPO_LOCK_IMMEDIATELY HECI Request

  @param[in] Type               Type of lock.

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_DEVICE_ERROR       Cannot get MeFirmwareStatus
                                 or send or receive message or status of response are failed.
  @retval EFI_INVALID_PARAMETER  Type parameter not supported
                                 or pointer to Heci message is Null (from CallMsg_HmrfpoExtLockImmediately).
**/
EFI_STATUS
HeciReq_HmrfpoExtLockImmediately (
  IN HMRFPO_EXT_LOCK_TYPE  Type
  );

/**
  Procedure to send HMRFPO_GETSTATUS HECI Request. Return status of flash protection override.

  @param[out]   State            Status of flash protection override.

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_DEVICE_ERROR       Cannot get MeFirmwareStatus
                                 or send or receive message or status of response are failed.
  @retval EFI_INVALID_PARAMETER  State or pointer to Heci message is Null (from CallMsg_HmrfpoExtGetStatus).
**/
EFI_STATUS
HeciReq_HmrfpoExtGetStatus (
  OUT HMRFPO_EXT_STATUS  *State
  );


/**
  Procedure to send HMRFPO_LOCK HECI Request

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_DEVICE_ERROR       Send or receive message or status of response are failed
                                 (from HeciReq_HmrfpoExtLock).
  @retval EFI_INVALID_PARAMETER  Pointer to Heci message is Null (from HeciReq_HmrfpoExtLock).
**/
EFI_STATUS
HmrfpoExt_Lock (
  VOID
  );


/**
  Procedure to send HMRFPO_LOCK_IMMEDIATELY HECI Request

  @param[in] Type               Type of lock.

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_DEVICE_ERROR       Send or receive message or status of response are failed
                                 (from HeciReq_HmrfpoExtLockImmediately).
  @retval EFI_INVALID_PARAMETER  Type parameter not supported
                                 or pointer to Heci message is Null (from HeciReq_HmrfpoExtLockImmediately).
**/
EFI_STATUS
HmrfpoExt_LockImmediately (
  IN HMRFPO_EXT_LOCK_TYPE Type
  );


/**
  Procedure to send HMRFPO_ENABLE HECI Request

  @param[in]    RegionSelect  Indicate region for enable.
                              Only HmrfpoExtEnableRegionIntelMe is supported.
  @param[out]   FactoryBase   Pointer to the base address of the Intel ME factory data area.
                              Passing NULL pointer is legal.
  @param[out]   FactoryLimit  Pointer to the length of the Intel ME factory data area.
                              Passing NULL pointer is legal.

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_UNSUPPORTED        Variable gNonceValue is 0 or unsupported RegionSelect.
  @retval EFI_DEVICE_ERROR       Send or receive message or status of response are failed
                                 (from HeciReq_HmrfpoExtEnable).
  @retval EFI_INVALID_PARAMETER  Pointer to Heci message is Null (from HeciReq_HmrfpoExtEnable).
**/
EFI_STATUS
HmrfpoExt_Enable (
  IN  HMRFPO_EXT_REGION_SELECT  RegionSelect,
  OUT UINT32                    *FactoryBase,
  OUT UINT32                    *FactoryLimit
  );

/**
  Procedure to send HMRFPO_MERESET HECI Request

  @param[in]   ResetType      Reset type.

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_UNSUPPORTED        Variable gNonceValue is 0 or unsupported reset type.
  @retval EFI_DEVICE_ERROR       Send or receive message or status of response are failed
                                 (from HeciReq_HmrfpoExtMeReset).
  @retval EFI_INVALID_PARAMETER  Pointer to Heci message is Null (from HeciReq_HmrfpoExtMeReset).
**/
EFI_STATUS
HmrfpoExt_MeReset (
  IN HMRFPO_EXT_RESET_TYPE  ResetType
  );

/**
  Procedure to send HMRFPO_GETSTATUS HECI Request. Return status of flash protection override.

  @param[out]  ExtStatus         Status of flash protection override.

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_DEVICE_ERROR       ExtStatus is unsupported
                                 or send or receive message are failed (from HeciReq_HmrfpoExtGetStatus).
  @retval EFI_INVALID_PARAMETER  ExtStatus or pointer to Heci message is Null (from HeciReq_HmrfpoExtGetStatus).
**/
EFI_STATUS
HmrfpoExt_GetStatus (
  OUT HMRFPO_EXT_STATUS *ExtStatus
  );

#endif /* _SPS_SMM_HMRFPO_EXT_H_ */
