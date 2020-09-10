/** @file
  ME Measurements Library

 @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2019 - 2020 Intel Corporation.

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

**/


#include <Uefi.h>
#include <PchAccess.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MeMeasurementsLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MmPciBaseLib.h>
#include <Guid/EventGroup.h>
#include <Protocol/Tcg2Protocol.h>
#include <MeMeasurements.h>
#include <MeChipset.h>
#include <Register/MeRegs.h>

/**
  Perform measurement for HER register.

  @param[in] HerValue             The value of HECI Extend Register.
  @param[in] Index                HerValue Size.

  @retval EFI_SUCCESS             Measurement performed
  @retval EFI_INVALID_PARAMETER   Null pointer passed as parameter
**/
EFI_STATUS
MeasureHer (
  IN  UINT32                      *HerValue,
  IN  UINT8                       Index
  )
{
  EFI_STATUS            Status;
  EFI_TCG2_PROTOCOL     *Tcg2Protocol;
  EFI_TCG2_EVENT        *Tcg2Event;
  UINT32                EventSize;
  EFI_TCG_EV_POST_CODE  EventPostCode;

  if (HerValue == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (
                  &gEfiTcg2ProtocolGuid,
                  NULL,
                  &Tcg2Protocol
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // The below data will be stored in Tcg eventlog
  //
  EventPostCode.PostCodeAddress = *HerValue;
  EventPostCode.PostCodeLength  = sizeof (UINT32);
  EventSize = sizeof (EFI_TCG_EV_POST_CODE);

  Tcg2Event = (EFI_TCG2_EVENT*) AllocateZeroPool (EventSize + sizeof (EFI_TCG2_EVENT) - sizeof (Tcg2Event->Event));

  if (Tcg2Event == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Fill the Tcg2Event Header
  //
  Tcg2Event->Size = sizeof (EFI_TCG_EV_POST_CODE) + sizeof (EFI_TCG2_EVENT) - sizeof (Tcg2Event->Event);
  Tcg2Event->Header.HeaderSize = sizeof (EFI_TCG2_EVENT_HEADER);
  Tcg2Event->Header.HeaderVersion = EFI_TCG2_EVENT_HEADER_VERSION;
  Tcg2Event->Header.PCRIndex = 0; //PCRi_CRTM_AND_POST_BIOS
  Tcg2Event->Header.EventType = EV_S_CRTM_CONTENTS;

  CopyMem (&Tcg2Event->Event[0], &EventPostCode, sizeof (EFI_TCG_EV_POST_CODE));

  Status = Tcg2Protocol->HashLogExtendEvent (
                           Tcg2Protocol,
                           0,
                           (EFI_PHYSICAL_ADDRESS) (&(EventPostCode)),
                           EventSize,
                           Tcg2Event
                           );

  FreePool (Tcg2Event);

  return Status;
}

/**
  Me Measurement.

  @retval EFI_NOT_READY           Not ready for measurement.
  @retval EFI_SUCCESS             Measurement done
**/
EFI_STATUS
MeMeasurement (
  VOID
  )
{
  EFI_STATUS    Status;
  DATA32_UNION  Data32[MAX_HER_REGISTERS];
  UINT8         HerMax;
  UINT8         HerIndex;
  UINT8         Index;

  Index = 0;
  Status = EFI_SUCCESS;

  //
  // Measure HER
  //
  HerMax                = R_ME_HER5;

  Data32[Index].Data32  = MmioRead32 (MmPciBase (ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER) + R_ME_HERS);
  if ((Data32[Index].Data32 & B_ME_EXTEND_REG_VALID) == B_ME_EXTEND_REG_VALID) {
    if ((Data32[Index].Data8[0] & B_ME_EXTEND_REG_ALGORITHM) == V_ME_SHA_256) {
      HerMax = R_ME_HER8;
    }

    for (HerIndex = R_ME_HER1, Index = 0; HerIndex <= HerMax; HerIndex += 4, Index++) {
      Data32[Index].Data32 = MmioRead32 (MmPciBase (ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER) + HerIndex);
    }

    Status = MeasureHer (&Data32->Data32, Index);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "ME Measurement feature failed, Status is %r \n", Status));
    }
  } else {
    Status = EFI_NOT_READY;
  }

  return Status;
}

/**
  Signal a event for last checking.
  ME Measurements EndOfDxeEvent notification handler.

  @param[in] Event                The event that triggered this notification function
  @param[in] ParentImageHandle    Pointer to the notification functions context

  @retval EFI_SUCCESS             Event executed and closed.
**/
VOID
EFIAPI
MeMeasurementEndOfDxeNotification (
  IN  EFI_EVENT                   Event,
  IN  VOID                        *Context
  )
{
  MeMeasurement ();
  gBS->CloseEvent (Event);

  return;
}

/**
  Register End of Dxe notification for ME Measurements

  @retval  Status   Notification installation status
**/
EFI_STATUS
RegisterMeMeasurementsEndOfDxeNotification (
  VOID
  )
{
  EFI_EVENT             EndOfDxeEvent;
  EFI_STATUS            Status;

  if (PcdGetBool(PcdHeciEndOfDxeNotify) == TRUE) {
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    MeMeasurementEndOfDxeNotification,
                    NULL,
                    &gEfiEndOfDxeEventGroupGuid,
                    &EndOfDxeEvent
                    );
    ASSERT_EFI_ERROR (Status);
  } else {
    Status = EFI_SUCCESS;
  }

  return Status;
}
