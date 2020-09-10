/** @file
  Abstract:
  Structures and macros for SPS SMM driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2020 Intel Corporation. <BR>

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

#ifndef _SPSSMM_H_
#define _SPSSMM_H_

#include <Protocol/SpsSmmHmrfpoProtocol.h>
#include <DualBiosMsgs.h>

#define HECI_ME_REINIT_TIMEOUT   2000000   ///< 2 secs timeout for ME reinit

/*****************************************************************************
* RAS Configuration
*****************************************************************************/
#define HECI_RSP_STATUS_SUCCESS       0x0

#define HECI_MSG_ENABLE_REQ_MKHI_HDR  0x00000105
#define HECI_MSG_ENABLE_RSP_MKHI_HDR  0x00008105

#define HECI_MSG_MERESET_REQ_MKHI_HDR  0x00000005
#define HECI_MSG_MERESET_RSP_MKHI_HDR  0x00008005

#define HECI_MSG_LOCK_REQ_MKHI_HDR  0x00000205
#define HECI_MSG_LOCK_RSP_MKHI_HDR  0x00008205

EFI_STATUS
HeciReq_HmrfpoLock (
  IN OUT UINT64    *Nonce,
  IN     UINT32     RegionToLock
  );

EFI_STATUS
HeciReq_HmrfpoEnable (
  IN UINT64   Nonce,
  IN UINT32   RegionToEnable
  );

EFI_STATUS
HeciReq_HmrfpoMeReset (
  IN UINT64        Nonce
  );

EFI_STATUS
HeciReq_RegionSelect (
  IN HECI_CONTROL   *pSmmHeci,
  IN UINT64         Nonce
  );

EFI_STATUS
HeciReq_WatchdogControl (
  IN HECI_CONTROL   *pSmmHeci,
  IN UINT8          Action
  );


EFI_STATUS
BiosUpdate_RegionSelect (VOID);

EFI_STATUS
BiosUpdate_UpdateAllowed (VOID);

EFI_STATUS
BiosUpdate_GetActiveRegionNum (
  OUT UINT8               *Region
  );

EFI_STATUS
GetSpsConfiguration (
  OUT SPS_INFO_HOB        *pHob
  );

VOID
RetrieveMeVersions (
  IN OUT SPS_SMM_HMRFPO_PROTOCOL *pSpsSmmHmrfpo
  );

EFI_STATUS
Hmrfpo_Lock (VOID);

EFI_STATUS
Hmrfpo_MeReset (VOID);

EFI_STATUS
Hmrfpo_Enable (
  IN UINT32  RegionToEnable
  );


/**
  This code is called on ReadyToBoot event

  @param[in] Protocol   Points to the protocol's unique identifier
  @param[in] Interface  Points to the interface instance
  @param[in] Handle     The handle on which the interface was installed

  @retval EFI_SUCCESS   always success successfully
**/
EFI_STATUS
EFIAPI
SpsSmmReadyToBootCallback (
  IN CONST EFI_GUID     *Protocol,
  IN VOID               *Interface,
  IN EFI_HANDLE         Handle
  );

EFI_STATUS
EFIAPI
SpsSmmExitBootServicesCallback (
  IN CONST EFI_GUID     *Protocol,
  IN VOID               *Interface,
  IN EFI_HANDLE         Handle
  );

EFI_STATUS EFIAPI
MeStorageSmmEntryWrite (
  IN     UINT8   Index,
     OUT UINT8  *pStatus,
  IN     VOID   *pBuffer,
  IN     UINT32  BufLen
  );

EFI_STATUS EFIAPI
MeStorageSmmEntryRead (
  IN     UINT8   Index,
     OUT UINT8  *pStatus,
     OUT VOID   *pBuffer,
  IN OUT UINT32 *pBufLen
  );

#endif /* _SPSSMM_H_ */
