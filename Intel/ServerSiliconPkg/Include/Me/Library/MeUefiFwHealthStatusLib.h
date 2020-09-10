/** @file
  Definitions for Me Uefi Health Status Library.
  Implements Health Status Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation.

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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

#ifndef _ME_UEFI_FW_HEALTH_STATUS_LIB_H_
#define _ME_UEFI_FW_HEALTH_STATUS_LIB_H_


typedef enum {
  // Drivers' status
  Heci1PpiInstalledStatus,
  Heci2PpiInstalledStatus,
  Heci3PpiInstalledStatus,
  Heci4PpiInstalledStatus,
  Heci1DxeInstalledStatus,
  Heci2DxeInstalledStatus,
  Heci3DxeInstalledStatus,
  Heci4DxeInstalledStatus,
  Heci1SmmInstalledStatus,
  Heci2SmmInstalledStatus,
  Heci3SmmInstalledStatus,
  Heci4SmmInstalledStatus,
  // Sendings Status
  DidSendingStatus,
  CoreBiosDoneSendingStatus,
  EndOfPostSendingStatus,
  // Receiving Status
  MeUmaSizeInfoReceivingStatus,
  BiosPayloadForWsReceivingStatus,
  // State of ME devices
  Heci1State,
  Heci2State,
  Heci3State,
  Heci4State,
  IdeRState,
  KTState,
  AcpiFilesLoadingStatus,
  Heci1HideInMe,
  Heci2HideInMe,
  Heci3HideInMe,
  Heci4HideInMe,

  HealthStatusFieldNumMax,
} ME_UEFI_FW_HEALTH_STATUS_FIELD_ID;

typedef enum {
  StatusNotTriggeredSupported = 0,   // not triggerred/supported
  StatusError                 = 1,   // error status
  StatusOk                    = 2,   // OK status
  StatusOther                 = 3,   // status for other purpose
} ME_UEFI_FW_HEALTH_STATUS;

/**
  Function sets value of determined field in MeUefiFwHealthStatus HOB.

  @param[in] FieldId           MeUefiFwHealthStatus field id
  @param[in] HealthStatus      required status for determined field

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_OUT_OF_RESOURCES  HOB creation failed
  @retval EFI_COMPROMISED_DATA  HOB data is not valid
  @return EFI_UNSUPPORTED       HOB can't be created at DXE phase
**/
EFI_STATUS
SetMeUefiFwHealthStatus (
  IN ME_UEFI_FW_HEALTH_STATUS_FIELD_ID   FieldId,
  IN ME_UEFI_FW_HEALTH_STATUS            HealthStatus
  );

/**
  Function gets value of determined field from MeUefiFwHealthStatus HOB.

  @param[in] FieldId            MeUefiFwHealthStatus field id
  @param[out] HealthStatus      status of determined field

  @retval EFI_SUCCESS           The function completed successfully
  @retval EFI_INVALID_PARAMETER Pointer to HealthStatus is NULL
  @retval EFI_OUT_OF_RESOURCES  MeUefiFwHealthStatus HOB doesn't exist
  @retval EFI_COMPROMISED_DATA  HOB data is not valid
  @retval EFI_UNSUPPORTED       MeUefiFwHealthStatus is not supported
**/
EFI_STATUS
GetMeUefiFwHealthStatus (
  IN  ME_UEFI_FW_HEALTH_STATUS_FIELD_ID  FieldId,
  OUT ME_UEFI_FW_HEALTH_STATUS           *HealthStatus
  );

/**
  Function sets value (only StatusOk or StatusError) of the determined field in MeUefiFwHealthStatus HOB,
  depending on the received value of Efi Status.

  @param[in] FieldId           MeUefiFwHealthStatus field id
  @param[in] Status            EFI status for determined field

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_OUT_OF_RESOURCES  HOB creation failed
**/
EFI_STATUS
SetMeUefiFwHealthEfiStatus (
  ME_UEFI_FW_HEALTH_STATUS_FIELD_ID    FieldId,
  EFI_STATUS                           EfiStatus
  );
#endif // _ME_UEFI_FW_HEALTH_STATUS_LIB_H_
