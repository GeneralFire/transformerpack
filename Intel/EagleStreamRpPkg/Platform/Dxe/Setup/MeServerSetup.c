/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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
#if ME_TESTMENU_FLAG

#include "SetupPlatform.h"
#include "Library/MeUefiFwHealthStatusLib.h"

#define ME_INFO_MAX_STR_SIZE 0x200

/**
  This function updates ME UEFI FW Health Status related strings in BIOS setup database.

  Some of the strings contain dummy values to be update with run-time data.
  These strings are updated by this function before any setup screen is
  printed. For some reason the strings from various screens,
  although identified with uniqueue ids, are stored in separate databases.
  Class identifes the database. To avoid multiple requests to ME
  and multiple traces parts of this function are executed for particular
  forms.

  @param[out] HiiAdvancedHandle - BIOS setup datebase handle
  @param[out] HiiMainHandle - BIOS setup datebase handle

  @return Void.
**/
VOID
MeUefiFwHealthStatusInitSetupStrings (
  OUT EFI_HII_HANDLE HiiAdvancedHandle,
  OUT EFI_HII_HANDLE HiiMainHandle
  )
{
  CHAR16                StrBuf[ME_INFO_MAX_STR_SIZE];
  ME_UEFI_FW_HEALTH_STATUS         MeUefiFwHealthStatus;
  UINT8                 FieldIdx;
  EFI_STRING_ID         UefiFwHealthStatusStringId[HealthStatusFieldNumMax] = {
      STRING_TOKEN (STR_HECI1_PPI_STATUS_VALUE),
      STRING_TOKEN (STR_HECI2_PPI_STATUS_VALUE),
      STRING_TOKEN (STR_HECI3_PPI_STATUS_VALUE),
      STRING_TOKEN (STR_HECI4_PPI_STATUS_VALUE),
      STRING_TOKEN (STR_HECI1_DXE_STATUS_VALUE),
      STRING_TOKEN (STR_HECI2_DXE_STATUS_VALUE),
      STRING_TOKEN (STR_HECI3_DXE_STATUS_VALUE),
      STRING_TOKEN (STR_HECI4_DXE_STATUS_VALUE),
      STRING_TOKEN (STR_HECI1_SMM_STATUS_VALUE),
      STRING_TOKEN (STR_HECI2_SMM_STATUS_VALUE),
      STRING_TOKEN (STR_HECI3_SMM_STATUS_VALUE),
      STRING_TOKEN (STR_HECI4_SMM_STATUS_VALUE),
      STRING_TOKEN (STR_DID_SENDING_STATUS_VALUE),
      STRING_TOKEN (STR_CORE_BIOS_DONE_SENDING_STATUS_VALUE),
      STRING_TOKEN (STR_END_OF_POST_SENDING_STATUS_VALUE),
      STRING_TOKEN (STR_ME_UMA_RECEIVING_STATUS_VALUE),
      STRING_TOKEN (STR_BIOS_PAYLOAD_RECEIVING_STATUS_VALUE),
      STRING_TOKEN (STR_ME_HECI1_STATE_VALUE),
      STRING_TOKEN (STR_ME_HECI2_STATE_VALUE),
      STRING_TOKEN (STR_ME_HECI3_STATE_VALUE),
      STRING_TOKEN (STR_ME_HECI4_STATE_VALUE),
      STRING_TOKEN (STR_ME_IDER_STATE_VALUE),
      STRING_TOKEN (STR_ME_KT_STATE_VALUE),
      STRING_TOKEN (STR_ME_ACPI_STATUS_VALUE),
      STRING_TOKEN (STR_ME_HECI1_HIDE_STATE_VALUE),
      STRING_TOKEN (STR_ME_HECI2_HIDE_STATE_VALUE),
      STRING_TOKEN (STR_ME_HECI3_HIDE_STATE_VALUE),
      STRING_TOKEN (STR_ME_HECI4_HIDE_STATE_VALUE),
  };

  EFI_STATUS Status;

  DEBUG ((EFI_D_INFO, "[ME] MeUefiFwHealthStatusInitSetupStrings called...\n"));

// APTIOV_SERVER_OVERRIDE_RC_END : HiiMainHandle wasn't updated in AptioV
  if ((HiiAdvancedHandle != NULL)/* && (HiiMainHandle != NULL)*/) {
// APTIOV_SERVER_OVERRIDE_RC_END : HiiMainHandle wasn't updated in AptioV

    for (FieldIdx = Heci1PpiInstalledStatus; FieldIdx <= Heci3SmmInstalledStatus; FieldIdx++) {

      Status = GetMeUefiFwHealthStatus (FieldIdx, &MeUefiFwHealthStatus);

      if (EFI_ERROR (Status)) {
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Unknown");
        continue;
      }

      switch (MeUefiFwHealthStatus) {
      case StatusNotTriggeredSupported:
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Not Triggered");
        break;
      case StatusError:
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Not Installed");
        break;
      case StatusOk:
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Installed");
        break;
      default:
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Unknown");
        break;
      }

      HiiSetString (HiiAdvancedHandle, UefiFwHealthStatusStringId[FieldIdx], StrBuf, NULL);
    }

    for (FieldIdx = DidSendingStatus; FieldIdx <= EndOfPostSendingStatus; FieldIdx++) {

      Status = GetMeUefiFwHealthStatus (FieldIdx, &MeUefiFwHealthStatus);

      if (EFI_ERROR (Status)) {
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Unknown");
        continue;
      }

      switch (MeUefiFwHealthStatus) {
      case StatusNotTriggeredSupported:
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Not sent - not triggered");
        break;
      case StatusError:
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Not sent - error");
        break;
      case StatusOk:
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Sent");
        break;
      case StatusOther:
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Invalid Response");
        break;
      default:
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Unknown");
        break;
      }

      HiiSetString (HiiAdvancedHandle, UefiFwHealthStatusStringId[FieldIdx], StrBuf, NULL);
    }

    for (FieldIdx = MeUmaSizeInfoReceivingStatus; FieldIdx <= BiosPayloadForWsReceivingStatus; FieldIdx++) {

      Status = GetMeUefiFwHealthStatus (FieldIdx, &MeUefiFwHealthStatus);

      if (EFI_ERROR (Status)) {
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Unknown");
        continue;
      }

      switch (MeUefiFwHealthStatus) {
      case StatusNotTriggeredSupported:
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Not received - not triggered");
        break;
      case StatusError:
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Not Received - error");
        break;
      case StatusOk:
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Received");
        break;
      default:
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Unknown");
        break;
      }

      HiiSetString (HiiAdvancedHandle, UefiFwHealthStatusStringId[FieldIdx], StrBuf, NULL);
    }

    for (FieldIdx = Heci1State; FieldIdx <= KTState; FieldIdx++) {

      Status = GetMeUefiFwHealthStatus (FieldIdx, &MeUefiFwHealthStatus);

      if (EFI_ERROR (Status)) {
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Unknown");
        continue;
      }

      switch (MeUefiFwHealthStatus) {
      case StatusOther:
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Disabled");
        break;
      case StatusOk:
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Enabled");
        break;
      default:
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Unknown");
        break;
      }

      HiiSetString (HiiAdvancedHandle, UefiFwHealthStatusStringId[FieldIdx], StrBuf, NULL);
    }

    Status = GetMeUefiFwHealthStatus (AcpiFilesLoadingStatus, &MeUefiFwHealthStatus);

    if (!EFI_ERROR (Status)) {
      switch (MeUefiFwHealthStatus) {
      case StatusNotTriggeredSupported:
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Not Triggered");
        break;
      case StatusError:
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Failure");
        break;
      case StatusOk:
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Success");
        break;
      default:
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Unknown");
        break;
      }
    } else {
      UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Unknown");
    }

    HiiSetString (HiiAdvancedHandle, UefiFwHealthStatusStringId[AcpiFilesLoadingStatus], StrBuf, NULL);

    for (FieldIdx = Heci1HideInMe; FieldIdx <= Heci4HideInMe; FieldIdx++) {

      Status = GetMeUefiFwHealthStatus (FieldIdx, &MeUefiFwHealthStatus);

      if (EFI_ERROR (Status)) {
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"Unknown");
        continue;
      }

      switch (MeUefiFwHealthStatus) {
      case StatusOk:
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"True");
        break;
      default:
        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"False");
        break;
      }

      HiiSetString (HiiAdvancedHandle, UefiFwHealthStatusStringId[FieldIdx], StrBuf, NULL);
    }
  }
}
#endif // ME_TESTMENU_FLAG
