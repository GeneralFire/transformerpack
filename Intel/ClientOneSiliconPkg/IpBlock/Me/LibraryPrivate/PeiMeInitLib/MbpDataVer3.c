/** @file
  ME BIOS Payload Data implementation.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

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
#include <PiPei.h>
#include <Library/ChipsetInitLib.h>
#include <MeBiosPayloadHob.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiMeLib.h>
#include <Library/DebugLib.h>
#include <Library/MeTypeLib.h>
#include <Library/MeUefiFwHealthStatusLib.h>
#include <Library/MeGetConfigBlock.h>
#include <Library/PreSiliconEnvDetectLib.h>

EFI_STATUS
SyncMbpAfterDid (
  IN EFI_PEI_SERVICES                   **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR          *NotifyDesc,
  IN VOID                               *Ppi
  );

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_NOTIFY_DESCRIPTOR mSyncMbpNotifyList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gDramInitDoneSentPpiGuid,
    SyncMbpAfterDid
  }
};

/**
  In case of WS FW syncing missing mbp items can be done DramInitDone message,
  so the BIOS installs notification on this signal.

  @param[in, out] MbpHob              Pointer to MBP Hob.
**/

VOID
SyncMissingMbpItems (
  IN OUT ME_BIOS_PAYLOAD_HOB *MbpHob
  )
{
  EFI_STATUS Status;

  if (!MeTypeIsClient ()) {
    return;
  }

  Status = PeiServicesNotifyPpi (mSyncMbpNotifyList);
  ASSERT_EFI_ERROR (Status);
}

/**
  Check if MBP items that might be missing on the first boot are available.
  If not, additional sync messages are executed in order to retrieve them.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The notify callback PPI.

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
SyncMbpAfterDid (
  IN EFI_PEI_SERVICES                   **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR          *NotifyDesc,
  IN VOID                               *Ppi
  )
{
  EFI_STATUS           Status;
  ME_BIOS_PAYLOAD_HOB  *MbpHob;
  VOID                 *TestPpi;

  Status = PeiServicesLocatePpi (
             &gDramInitDoneSentPpiGuid,
             0,
             NULL,
             &TestPpi
             );
  if (EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }

  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);

  if (MbpHob != NULL) {
    if (MbpHob->MeBiosPayload.FwCapsSku.Available == FALSE) {
      Status = PeiHeciGetFwCapsSkuMsg (&MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities);
      if (!EFI_ERROR (Status)) {
        MbpHob->MeBiosPayload.FwCapsSku.Available = TRUE;
      }
    }
    if (MbpHob->MeBiosPayload.FwFeaturesState.Available == FALSE) {
      Status = PeiHeciGetFwFeatureStateMsg (&MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures);
      if (!EFI_ERROR (Status)) {
        MbpHob->MeBiosPayload.FwFeaturesState.Available = TRUE;
      }
    }
    if (MbpHob->MeBiosPayload.FwPlatType.Available == FALSE) {
      Status = PeiHeciGetPlatformTypeMsg (&MbpHob->MeBiosPayload.FwPlatType.RuleData);
      if (!EFI_ERROR (Status)) {
        MbpHob->MeBiosPayload.FwPlatType.Available = TRUE;
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  Prepare MbpData Hob for SPS.

  @param[in] MbpHob             Pointer to Mbp Hob
  @param[in] MbpSensitivePpi    Pointer to Mbp Sensitive Ppi

  @retval    EFI_SUCCESS        The function completed successfully.
  @retval    EFI_UNSUPPORTED    MBP Hob creation skipped
  @retval    Other              Failed to create MBP Hob
**/
EFI_STATUS
PrepareSpsMeBiosPayload (
  IN ME_BIOS_PAYLOAD                     *MbpData,
  IN ME_BIOS_PAYLOAD_SENSITIVE           *MbpSensitiveData
  )
{
  EFI_STATUS              Status;
  SI_PREMEM_POLICY_PPI    *SiPreMemPolicyPpi;
  PCH_HSIO_PREMEM_CONFIG  *HsioPreMemConfig;
  PCH_HSIO_VER_INFO       *CsmeChipsetInitVerInfoPtr;
  PCH_HSIO_VER_INFO       *ChipsetInitTblPtr;
  UINT32                  ChipsetInitTblLen;
  VOID                    *TestPpi;

  Status = EFI_SUCCESS;

  if (!MeTypeIsClient ()) {
    DEBUG ((DEBUG_INFO, "[SPS] Create MBP HOB for non WS firmware\n"));
    if (MbpData == NULL) {
      ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
      return EFI_INVALID_PARAMETER;
    }
    if (MbpSensitiveData != NULL) {
      ZeroMem (MbpSensitiveData, sizeof (ME_BIOS_PAYLOAD_SENSITIVE));
    }
    ZeroMem (MbpData, sizeof (ME_BIOS_PAYLOAD));

    DEBUG ((DEBUG_INFO, "[SPS] Read ChipsetInit Binary from CSME\n"));

    Status = PeiServicesLocatePpi (
               &gSiPreMemPolicyPpiGuid,
               0,
               NULL,
               (VOID**) &SiPreMemPolicyPpi
               );
    ASSERT_EFI_ERROR (Status);
    if (!EFI_ERROR (Status)) {
      Status = GetConfigBlock ((VOID*) SiPreMemPolicyPpi, &gHsioPreMemConfigGuid, (VOID*) &HsioPreMemConfig);
      if (EFI_ERROR (Status)) {
        if (!IsSimicsEnvironment ()) {
          HsioPreMemConfig->ChipsetInitMessage = 1;
          Status = EFI_SUCCESS;
        }
      }
    }

    CsmeChipsetInitVerInfoPtr = (PCH_HSIO_VER_INFO*) MbpData->MphyData.ChipsetInitVer;

    if (!EFI_ERROR (Status) && (HsioPreMemConfig->ChipsetInitMessage == 1)) {
      ChipsetInitTblLen = sizeof (PCH_HSIO_VER_INFO);
      ChipsetInitTblPtr = (PCH_HSIO_VER_INFO*) AllocateZeroPool (ChipsetInitTblLen * 2);
      ASSERT (ChipsetInitTblPtr != NULL);
      if (ChipsetInitTblPtr != NULL) {
        Status = PeiServicesLocatePpi (
                   &gDramInitDoneSentPpiGuid,
                   0,
                   NULL,
                   &TestPpi
                   );

        if (EFI_ERROR (Status)) {
          // DID has not been sent yet, so use BUP_MPHY_READ_FROM_MPHY command
          Status = PeiHeciReadFromMphy ((UINT8*) ChipsetInitTblPtr, ChipsetInitTblLen);
        } else {
          // DID has been sent, so use ICC_READ_FROM_MPHY_CMD
          Status = PeiHeciReadChipsetInitMsg ((UINT8*) ChipsetInitTblPtr, &ChipsetInitTblLen);
        }

        if (!EFI_ERROR (Status)) {
          CsmeChipsetInitVerInfoPtr->BaseCrc = ChipsetInitTblPtr->BaseCrc;
          CsmeChipsetInitVerInfoPtr->OemCrc = ChipsetInitTblPtr->OemCrc;
          CsmeChipsetInitVerInfoPtr->BaseCrcValid = ChipsetInitTblPtr->BaseCrcValid;
          CsmeChipsetInitVerInfoPtr->OemCrcValid = ChipsetInitTblPtr->OemCrcValid;
          CsmeChipsetInitVerInfoPtr->SusCrc = ChipsetInitTblPtr->SusCrc;
          CsmeChipsetInitVerInfoPtr->Version = ChipsetInitTblPtr->Version;
          CsmeChipsetInitVerInfoPtr->MetalLayer = ChipsetInitTblPtr->MetalLayer;
          CsmeChipsetInitVerInfoPtr->BaseLayer = ChipsetInitTblPtr->BaseLayer;
          CsmeChipsetInitVerInfoPtr->OemVersion = ChipsetInitTblPtr->OemVersion;
          CsmeChipsetInitVerInfoPtr->SusCrcValid = ChipsetInitTblPtr->SusCrcValid;
          MbpData->MphyData.Available = TRUE;
        } else {
          DEBUG ((DEBUG_WARN, "[SPS] WARNING: Can't read HSIO,"\
                "skip HSIO handshake as don't have version information\n"));
          MbpData->MphyData.Available = FALSE;
        }
        FreePool (ChipsetInitTblPtr);
      }
    }
  }

  return Status;
}

/**
  Function sets value of determined field in MeUefiFwHealthStatus HOB.

  @param[in] Status                Mbp Uefi Status, EFI_SUCCESS, or any error

  @retval    EFI_SUCCESS           The function completed successfully.
  @retval    EFI_OUT_OF_RESOURCES  HOB creation failed
**/
EFI_STATUS
ReportMeUefiFwMbpHealthStatus (
  IN EFI_STATUS   Status
  )
{
  ME_UEFI_FW_HEALTH_STATUS FwHealthStatus;

  if (EFI_ERROR (Status)) {
    FwHealthStatus = StatusError;
  } else {
    FwHealthStatus = StatusOk;
  }

  return SetMeUefiFwHealthStatus (BiosPayloadForWsReceivingStatus, FwHealthStatus);

}
