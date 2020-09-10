/** @file AcpiNotify.c
  ACPI Notification handling

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

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
**/

#include <Library/PmcLib.h>
#include "CrystalRidge.h"
#include "Ars/ArsPatrolScrubber.h"
#include "Dsm.h"
#include "Ars/ArsFlows.h"


/**
  Clears notification status from all NVDIMM leaf devices
**/
VOID
EFIAPI
AcpiClearLeafNotifications (
  VOID
  )
{
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "AcpiClearLeafNotifications\n"));
  if (mNvdimmAcpiSmmInterface->NotificationConsumedSequenceNumber ==
      mNvdimmAcpiSmmInterface->NotificationProducedSequenceNumber) {
    mNvdimmAcpiSmmInterface->NotificationBitmap0 = 0;
    mNvdimmAcpiSmmInterface->NotificationBitmap1 = 0;
  } else {
    CRDEBUG ((DEBUG_INFO,
            CR_INFO_STR ("CRP") "AcpiClearLeafNotifications: CSN(0x%x)!=PSN(0x%x), keeping notification bitmap\n",
            mNvdimmAcpiSmmInterface->NotificationConsumedSequenceNumber,
            mNvdimmAcpiSmmInterface->NotificationProducedSequenceNumber));
  }
}


/**
  Adds notification of specified NVDIMM leaf device

  @param[in]   Socket         Socket index
  @param[in]   Ch             Channel index
  @param[in]   Dimm           Dimm index

**/
VOID
EFIAPI
AcpiAddLeafNotification (
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 Dimm
  )
{
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "S%d.C%d.D%d: AcpiAddLeafNotification\n", Socket, Ch, Dimm));
  if (Socket < 4) {
    mNvdimmAcpiSmmInterface->NotificationBitmap0 |= DIMM_TO_BITMAP (Socket, Ch, Dimm);
  } else {
    mNvdimmAcpiSmmInterface->NotificationBitmap1 |= DIMM_TO_BITMAP (Socket, Ch, Dimm);
  }
  mNvdimmAcpiSmmInterface->NotificationProducedSequenceNumber++;
}


/**
  Triggers ACPI notification of NVDIMM leaf devices
**/
VOID
EFIAPI
AcpiLeafNotify (
  VOID
  )
{
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "AcpiLeafNotify: %llx:%llx\n", mNvdimmAcpiSmmInterface->NotificationBitmap0, mNvdimmAcpiSmmInterface->NotificationBitmap1));
  if (mNvdimmAcpiSmmInterface->NotificationBitmap0 == 0 && mNvdimmAcpiSmmInterface->NotificationBitmap1 == 0) {
    return;
  }
  PmcSetSwGpeSts ();
}


/**
  Clears NVDIMM root device notification status
**/
VOID
EFIAPI
AcpiClearRootNotification (
  VOID
  )
{
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "AcpiClearRootNotification\n"));
  if (mNvdimmAcpiSmmInterface->RootNotificationConsumedSequenceNumber ==
      mNvdimmAcpiSmmInterface->RootNotificationProducedSequenceNumber) {
    mNvdimmAcpiSmmInterface->RootNotificationStatus = 0;
  } else {
    CRDEBUG ((DEBUG_INFO,
            CR_INFO_STR ("CRP") "AcpiClearRootNotification: CSN(0x%x)!=PSN(0x%x), keeping notification status\n",
            mNvdimmAcpiSmmInterface->RootNotificationConsumedSequenceNumber,
            mNvdimmAcpiSmmInterface->RootNotificationProducedSequenceNumber));
  }
}

/**
  Adds to error list for specified DIMM and DPA

  @param[in]   Socket         Socket index
  @param[in]   Ch             Channel index
  @param[in]   Dimm           Dimm index
  @param[in]   Dpa            DPA
  @param[in]   RangeExponent  DPA range exponent (base 2)

**/
VOID
EFIAPI
AcpiAddToErrorList (
  IN UINT8  Socket,
  IN UINT8  Ch,
  IN UINT8  Dimm,
  IN UINT64 Dpa,
  IN UINT8  RangeExponent
  )
{
  NVDIMM     *NvmDimm;
  EFI_STATUS Status;
  UINT64     Spa;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "S%d.C%d.D%d: AcpiAddToErrorList (..., 0x%llx, 0x%x)\n",
          Socket, Ch, Dimm, Dpa, RangeExponent));
  if (RangeExponent > UNCORRECTABLE_UNIT_SIZE_SHIFT) {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: AcpiAddToErrorList: range exponent %d not supported, ignored.\n",
            Socket, Ch, Dimm, RangeExponent));
    return;
  }
  AlignAddressForArs (&Dpa, FALSE);
  NvmDimm = GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED);
  if (NvmDimm == NULL) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: AcpiAddToErrorList: DIMM not found!\n",
            Socket, Ch, Dimm));
    return;
  }
  Status = GetPmemSpaFromDpa (NvmDimm, Dpa, &Spa);
  if (EFI_ERROR (Status)) {

    if (Status != EFI_NOT_FOUND) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: AcpiAddToErrorList: "
                           "PMEM SPA for DPA 0x%llx not found: %r\n",
              Socket, Ch, Dimm, Dpa, Status));
    }
    return;
  }
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "S%d.C%d.D%d: AcpiAddToErrorList: PMEM SPA for DPA 0x%llx: 0x%llx\n",
          Socket, Ch, Dimm, Dpa, Spa));
  Status = ArsPatrolScrubberAdd (ConvertSocketChannelDimmToNfitHandle (Socket, Ch, Dimm), Spa);
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: ArsPatrolScrubberAdd (..., 0x%llx) failed: %r\n",
            Socket, Ch, Dimm, Spa, Status));
    return;
  }
}

/**
  Adds NVDIMM root device notification and adds to error list for specified DIMM and DPA

  @param[in]   Socket         Socket index
  @param[in]   Ch             Channel index
  @param[in]   Dimm           Dimm index
  @param[in]   Dpa            DPA
  @param[in]   RangeExponent  DPA range exponent (base 2)

**/
VOID
EFIAPI
AcpiAddRootNotification (
  IN UINT8  Socket,
  IN UINT8  Ch,
  IN UINT8  Dimm,
  IN UINT64 Dpa,
  IN UINT8  RangeExponent
  )
{
  AcpiAddToErrorList(Socket, Ch, Dimm, Dpa, RangeExponent);
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "S%d.C%d.D%d: AcpiAddRootNotification\n",
          Socket, Ch, Dimm));
  mNvdimmAcpiSmmInterface->RootNotificationStatus = 1;
  mNvdimmAcpiSmmInterface->RootNotificationProducedSequenceNumber++;
}

/**
  Triggers ACPI notification of NVDIMM root device

  @param[in]   Force  TRUE:  notify even if no notification is pending
                      FALSE: notify only if a notification is pending
**/
VOID
EFIAPI
AcpiRootNotifyEx (
  IN BOOLEAN Force
  )
{
  if (Force) {
    mNvdimmAcpiSmmInterface->RootNotificationStatus = 1;
    mNvdimmAcpiSmmInterface->RootNotificationProducedSequenceNumber++;
  }
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "AcpiRootNotifyInt: %d\n", mNvdimmAcpiSmmInterface->RootNotificationStatus));
  if (mNvdimmAcpiSmmInterface->RootNotificationStatus == 0) {
    return;
  }
  PmcSetSwGpeSts ();
}

/**
  Triggers ACPI notification of NVDIMM root device if such notification is pending
**/
VOID
EFIAPI
AcpiRootNotify (
  VOID
  )
{
  AcpiRootNotifyEx (FALSE);
}
