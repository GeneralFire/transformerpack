/** @file
  Implements ME UEFI FW Health Status Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation.

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
#include <Base.h>
#include <PiPei.h>
#include <Uefi.h>

#include <Guid/GlobalVariable.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/MeUefiFwHealthStatusLib.h>

#pragma pack (1)
typedef union _ME_UEFI_FW_HEALTH_STATUS_SET_ {

  struct {
    UINT32 Set1;
    UINT32 Set2;
  } Data;

  struct {
    UINT32 Heci1PpiStatus                : 2,
           Heci2PpiStatus                : 2,
           Heci3PpiStatus                : 2,
           Heci4PpiStatus                : 2,
           Heci1DxeStatus                : 2,
           Heci2DxeStatus                : 2,
           Heci3DxeStatus                : 2,
           Heci4DxeStatus                : 2,
           Heci1SmmStatus                : 2,
           Heci2SmmStatus                : 2,
           Heci3SmmStatus                : 2,
           Heci4SmmStatus                : 2,
           Reserved1                     : 8;
    UINT32 DidSendingStatus              : 2,
           CoreBiosDoneSendingStatus     : 2,
           EndOfPostSendingStatus        : 2,
           MeUmaSizeInfoRcvStatus        : 2,
           BiosPayloadForWsRcvStatus     : 2,
           HandlerEnableDisableOptStatus : 2,
           Reserved2                     : 20;
    UINT32 Heci1State                    : 2,
           Heci2State                    : 2,
           Heci3State                    : 2,
           Heci4State                    : 2,
           IdeRState                     : 2,
           KTState                       : 2,
           Heci1HideInMe                 : 2,
           Heci2HideInMe                 : 2,
           Heci3HideInMe                 : 2,
           Heci4HideInMe                 : 2,
           Reserved3                     : 12;
    UINT32 AcpiFilesLoadingStatus        : 2,
           Reserved4                     : 30;
  } Bits;

} ME_UEFI_FW_HEALTH_STATUS_SET;

typedef struct _ME_UEFI_FW_HEALTH_STATUS_HOB_
{
  ME_UEFI_FW_HEALTH_STATUS_SET   HealthStatus;
} ME_UEFI_FW_HEALTH_STATUS_HOB;
#pragma pack ()

/**
  Function creates HOB with info about MeUefiFwHealthStatus.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_OUT_OF_RESOURCES  HOB creation failed
  @retval EFI_UNSUPPORTED       MeUefiFwHealthStatus is not supported
                                HOB has been not created
**/
STATIC EFI_STATUS
CreateHealthStatusHob (
  VOID
  )
{
#ifdef ME_TESTMENU_FLAG
  ME_UEFI_FW_HEALTH_STATUS_HOB   HealthStatusHob;
  EFI_HOB_GUID_TYPE              *pGuidHob;

  ZeroMem (&HealthStatusHob, sizeof (HealthStatusHob));

  BuildGuidDataHob (&gMeUefiFwHealthStatusHobGuid, &HealthStatusHob, sizeof (HealthStatusHob));

  pGuidHob = GetFirstGuidHob (&gMeUefiFwHealthStatusHobGuid);
  if (pGuidHob != NULL) {
    DEBUG ((DEBUG_INFO, "INFO: HealthStatus HOB was created successfully\n")); // temporary
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_ERROR, "ERROR: HealthStatus HOB has not been registered\n"));

  return EFI_OUT_OF_RESOURCES;
#else // ME_TESTMENU_FLAG
  return EFI_UNSUPPORTED;
#endif // ME_TESTMENU_FLAG
} // CreateHealthStatusHob ()

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
  ME_UEFI_FW_HEALTH_STATUS_FIELD_ID    FieldId,
  ME_UEFI_FW_HEALTH_STATUS             HealthStatus
  )
{
#ifdef ME_TESTMENU_FLAG
  EFI_HOB_GUID_TYPE              *pGuidHob;
  EFI_STATUS                     Status;
  ME_UEFI_FW_HEALTH_STATUS_HOB   *HealthStatusHob;

  pGuidHob = GetFirstGuidHob (&gMeUefiFwHealthStatusHobGuid);

  if (pGuidHob == NULL) {
    if (sizeof(UINTN) != sizeof (UINT32)) {
      return EFI_UNSUPPORTED;
    }
    Status = CreateHealthStatusHob ();
    if (EFI_ERROR (Status)) {
      return Status;
    }

    pGuidHob = GetFirstGuidHob (&gMeUefiFwHealthStatusHobGuid);

    if (pGuidHob == NULL) {
      return EFI_COMPROMISED_DATA;
    }
  }

  HealthStatusHob = GET_GUID_HOB_DATA (pGuidHob);

  if (HealthStatusHob == NULL) {
    return EFI_COMPROMISED_DATA;
  }

  switch (FieldId) {
    case Heci1PpiInstalledStatus:
      HealthStatusHob->HealthStatus.Bits.Heci1PpiStatus = HealthStatus;
      break;
    case Heci2PpiInstalledStatus:
      HealthStatusHob->HealthStatus.Bits.Heci2PpiStatus = HealthStatus;
      break;
    case Heci3PpiInstalledStatus:
      HealthStatusHob->HealthStatus.Bits.Heci3PpiStatus = HealthStatus;
      break;
    case Heci4PpiInstalledStatus:
      HealthStatusHob->HealthStatus.Bits.Heci4PpiStatus = HealthStatus;
      break;
    case Heci1DxeInstalledStatus:
      HealthStatusHob->HealthStatus.Bits.Heci1DxeStatus = HealthStatus;
      break;
    case Heci2DxeInstalledStatus:
      HealthStatusHob->HealthStatus.Bits.Heci2DxeStatus = HealthStatus;
      break;
    case Heci3DxeInstalledStatus:
      HealthStatusHob->HealthStatus.Bits.Heci3DxeStatus = HealthStatus;
      break;
    case Heci4DxeInstalledStatus:
      HealthStatusHob->HealthStatus.Bits.Heci4DxeStatus = HealthStatus;
      break;
    case Heci1SmmInstalledStatus:
      HealthStatusHob->HealthStatus.Bits.Heci1SmmStatus = HealthStatus;
      break;
    case Heci2SmmInstalledStatus:
      HealthStatusHob->HealthStatus.Bits.Heci2SmmStatus = HealthStatus;
      break;
    case Heci3SmmInstalledStatus:
      HealthStatusHob->HealthStatus.Bits.Heci3SmmStatus = HealthStatus;
      break;
    case Heci4SmmInstalledStatus:
      HealthStatusHob->HealthStatus.Bits.Heci4SmmStatus = HealthStatus;
      break;
    case DidSendingStatus:
      HealthStatusHob->HealthStatus.Bits.DidSendingStatus = HealthStatus;
      break;
    case CoreBiosDoneSendingStatus:
      HealthStatusHob->HealthStatus.Bits.CoreBiosDoneSendingStatus = HealthStatus;
      break;
    case EndOfPostSendingStatus:
      HealthStatusHob->HealthStatus.Bits.EndOfPostSendingStatus = HealthStatus;
      break;
    case MeUmaSizeInfoReceivingStatus:
      HealthStatusHob->HealthStatus.Bits.MeUmaSizeInfoRcvStatus = HealthStatus;
      break;
    case BiosPayloadForWsReceivingStatus:
      HealthStatusHob->HealthStatus.Bits.BiosPayloadForWsRcvStatus = HealthStatus;
      break;
    case Heci1State:
      HealthStatusHob->HealthStatus.Bits.Heci1State = HealthStatus;
      break;
    case Heci2State:
      HealthStatusHob->HealthStatus.Bits.Heci2State = HealthStatus;
      break;
    case Heci3State:
      HealthStatusHob->HealthStatus.Bits.Heci3State = HealthStatus;
      break;
    case Heci4State:
      HealthStatusHob->HealthStatus.Bits.Heci4State = HealthStatus;
      break;
    case IdeRState:
      HealthStatusHob->HealthStatus.Bits.IdeRState = HealthStatus;
      break;
    case KTState:
      HealthStatusHob->HealthStatus.Bits.KTState = HealthStatus;
      break;
    case AcpiFilesLoadingStatus:
      HealthStatusHob->HealthStatus.Bits.AcpiFilesLoadingStatus = HealthStatus;
      break;
    case Heci1HideInMe:
      HealthStatusHob->HealthStatus.Bits.Heci1HideInMe = HealthStatus;
      break;
    case Heci2HideInMe:
      HealthStatusHob->HealthStatus.Bits.Heci2HideInMe = HealthStatus;
      break;
    case Heci3HideInMe:
      HealthStatusHob->HealthStatus.Bits.Heci3HideInMe = HealthStatus;
      break;
    case Heci4HideInMe:
      HealthStatusHob->HealthStatus.Bits.Heci4HideInMe = HealthStatus;
      break;
    default:
      DEBUG ((
        DEBUG_ERROR,
        "[%a] Invalid MeUefiFwHealthStatus field id = %d\n",
        __FUNCTION__,
        FieldId
        ));
      return EFI_INVALID_PARAMETER;
  }
#endif // ME_TESTMENU_FLAG

  return EFI_SUCCESS;
} // SetMeUefiFwHealthStatus

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
  ME_UEFI_FW_HEALTH_STATUS_FIELD_ID    FieldId,
  ME_UEFI_FW_HEALTH_STATUS             *HealthStatus
  )
{
#ifdef ME_TESTMENU_FLAG
  EFI_HOB_GUID_TYPE              *pGuidHob;
  ME_UEFI_FW_HEALTH_STATUS_HOB   *HealthStatusHob;

  if (HealthStatus == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  pGuidHob = GetFirstGuidHob (&gMeUefiFwHealthStatusHobGuid);

  if (pGuidHob == NULL) {
    return EFI_COMPROMISED_DATA;
  }

  HealthStatusHob = GET_GUID_HOB_DATA (pGuidHob);

  if (HealthStatusHob == NULL) {
    return EFI_COMPROMISED_DATA;
  }

  switch (FieldId) {
    case Heci1PpiInstalledStatus:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.Heci1PpiStatus;
      break;
    case Heci2PpiInstalledStatus:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.Heci2PpiStatus;
      break;
    case Heci3PpiInstalledStatus:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.Heci3PpiStatus;
      break;
    case Heci4PpiInstalledStatus:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.Heci4PpiStatus;
      break;
    case Heci1DxeInstalledStatus:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.Heci1DxeStatus;
      break;
    case Heci2DxeInstalledStatus:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.Heci2DxeStatus;
      break;
    case Heci3DxeInstalledStatus:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.Heci3DxeStatus;
      break;
    case Heci4DxeInstalledStatus:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.Heci4DxeStatus;
      break;
    case Heci1SmmInstalledStatus:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.Heci1SmmStatus;
      break;
    case Heci2SmmInstalledStatus:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.Heci2SmmStatus;
      break;
    case Heci3SmmInstalledStatus:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.Heci3SmmStatus;
      break;
    case Heci4SmmInstalledStatus:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.Heci4SmmStatus;
      break;
    case DidSendingStatus:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.DidSendingStatus;
      break;
    case CoreBiosDoneSendingStatus:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.CoreBiosDoneSendingStatus;
       break;
    case EndOfPostSendingStatus:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.EndOfPostSendingStatus;
      break;
    case MeUmaSizeInfoReceivingStatus:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.MeUmaSizeInfoRcvStatus;
      break;
    case BiosPayloadForWsReceivingStatus:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.BiosPayloadForWsRcvStatus;
      break;
    case Heci1State:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.Heci1State;
      break;
    case Heci2State:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.Heci2State;
      break;
    case Heci3State:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.Heci3State;
      break;
    case Heci4State:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.Heci4State;
      break;
    case IdeRState:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.IdeRState;
      break;
    case KTState:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.KTState;
      break;
    case AcpiFilesLoadingStatus:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.AcpiFilesLoadingStatus;
      break;
    case Heci1HideInMe:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.Heci1HideInMe;
      break;
    case Heci2HideInMe:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.Heci2HideInMe;
      break;
    case Heci3HideInMe:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.Heci3HideInMe;
      break;
    case Heci4HideInMe:
      *HealthStatus = HealthStatusHob->HealthStatus.Bits.Heci4HideInMe;
      break;
    default:
      *HealthStatus = StatusNotTriggeredSupported;
      DEBUG ((
        DEBUG_ERROR,
        "[%a] Invalid MeUefiFwHealthStatus field id = %d\n",
        __FUNCTION__,
        FieldId
        ));
      return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
#else // ME_TESTMENU_FLAG
  return EFI_UNSUPPORTED;
#endif // ME_TESTMENU_FLAG
} // GetMeUefiFwHealthStatus

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
  )
{
  EFI_STATUS ErrorStatus = EFI_SUCCESS;

#ifdef ME_TESTMENU_FLAG
  if (!EFI_ERROR (EfiStatus)) {
    ErrorStatus = SetMeUefiFwHealthStatus (FieldId, StatusOk);
  } else {
    ErrorStatus = SetMeUefiFwHealthStatus (FieldId, StatusError);
  }
#endif // ME_TESTMENU_FLAG

  return ErrorStatus;
} // SetMeUefiFwHealthEfiStatus
