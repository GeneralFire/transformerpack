/** @file
  Driver which creates events for sending END_OF_POST and CORE_BIOS_DONE messages
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

#include <Base.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MeMeasurementsLib.h>

#include <Protocol/HeciControlProtocol.h>
#include <Protocol/Wdt.h>
#if AMT_SUPPORT
#include <Protocol/AmtReadyToBoot.h>
#endif // AMT_SUPPORT

#include <PchResetPlatformSpecific.h>
#include <MkhiMsgs.h>
#include <MeState.h>
#include <Register/HeciRegs.h>
#include <Library/DxeMeLib.h>
#include <Library/MeTypeLib.h>
#include <Protocol/Smbios.h>
#include <Library/MeUtilsLib.h>
#include <Cpu/Msr/CommonMsr.h>

#include "ClientCoreInternal.h"
#include <Library/ServerDxeMeLib.h>


/**
  Function to set D0I3 bits for HECI devices when gBS->ExitBootServices() called or LegacyBoot event.

  @param[in]  Event                   Pointer to this event
  @param[in]  Context                 Event handler private data

**/
VOID
EFIAPI
HeciD0I3EntryPriorToBoot (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  DEBUG ((DEBUG_INFO, "HeciD0I3EntryPriorToBoot - setting D0I3 bits for HECI devices on boot time\n"));

  SetD0I3Bit (HECI1);
  SetD0I3Bit (HECI2);
  SetD0I3Bit (HECI3);
  SetD0I3Bit (HECI4);
}

/**
  Function to register gBS->ExitBootServices() and LegacyBoot callbacks for setting D0I3 bits in HECI devices.
**/
VOID
RegisterD0I3EventsForHeciDevices (
  VOID
  )
{
  EFI_EVENT                   Event;
  EFI_STATUS                  Status;
  ///
  /// Don't set D0I3 if disabled by policy
  ///
  if (MeHeciD0I3Enabled () == FALSE) {
    DEBUG ((DEBUG_INFO, "RegisterD0I3EventsForHeciDevices - SKIP setting D0I3 bits for HECI devices on boot time\n"));
    return;
  }

  ///
  /// Create ExitBootServices/LegacyBootEvent for setting the HECIx D0I3 bit
  ///
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  HeciD0I3EntryPriorToBoot,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &Event
                  );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  HeciD0I3EntryPriorToBoot,
                  NULL,
                  &gEfiEventLegacyBootGuid,
                  &Event
                  );
  ASSERT_EFI_ERROR (Status);
}

/**
  Dummy return for Me signal event use

  @param[in] Event                The event that triggered this notification function
  @param[in] Context              Pointer to the notification functions context
**/
VOID
EFIAPI
MeEmptyEvent (
  IN  EFI_EVENT                   Event,
  IN  VOID                        *Context
  )
{
  gBS->CloseEvent (Event);
}

/**
  Signal a event to save Me relevant registers and this event must be run before ExitPmAuth.

  @param[in] Event                The event that triggered this notification function
  @param[in] ParentImageHandle    Pointer to the notification functions context

**/
VOID
EFIAPI
MeScriptSaveEvent (
  IN  EFI_EVENT                   Event,
  IN  VOID                        *ParentImageHandle
  )
{
  DEBUG ((DEBUG_INFO, "%a () - start\n", __FUNCTION__));

  //
  // Sending notification that BIOS is starting to run 3rd party code.
  //
  if (MeCoreBiosDoneEnabled ()) {
    ServerHeciCoreBiosDoneMsg ();
  }

  DEBUG ((DEBUG_INFO, "%a () - end\n", __FUNCTION__));
}

/**
  Signal Me Ready To Boot Event
  Perform tasks below in the following order:
  1. Send EOP message.
  2. Lock config for manufacturing mode.

  @param[in] Event            The event that triggered this notification function
  @param[in] Context          Pointer to the notification functions context
**/
VOID
EFIAPI
MeReadyToBootEvent (
  IN  EFI_EVENT                   Event,
  IN  VOID                        *ParentImageHandle
  )
{
  EFI_STATUS                  Status;
  UINT32                      MeMode;
  HECI_FWS_REGISTER           MeFirmwareStatus;
  HECI_CONTROL                *Heci;
  EFI_EVENT                   MePlatformReadyToBootEvent;
#if AMT_SUPPORT
  UINTN                       Index;
  UINTN                       Count;
  EFI_HANDLE                  *Handles;
  AMT_READY_TO_BOOT_PROTOCOL  *AmtReadyToBoot;
#endif // AMT_SUPPORT

  DEBUG ((DEBUG_INFO, "%a () - start\n", __FUNCTION__));

#if AMT_SUPPORT
  ///
  /// We will trigger all events in order
  ///
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gAmtReadyToBootProtocolGuid,
                  NULL,
                  &Count,
                  &Handles
                  );
  if (!EFI_ERROR (Status)) {
    for (Index = 0; Index < Count; Index++) {
      Status = gBS->HandleProtocol (Handles[Index], &gAmtReadyToBootProtocolGuid, (VOID**)&AmtReadyToBoot);
      ASSERT_EFI_ERROR (Status);
      AmtReadyToBoot->Signal ();
    }
  }
#endif // AMT_SUPPORT

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  MeEmptyEvent,
                  NULL,
                  &gMePlatformReadyToBootGuid,
                  &MePlatformReadyToBootEvent
                  );
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status)) {
    gBS->SignalEvent (MePlatformReadyToBootEvent);
  }

  Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, (VOID **) &Heci);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - cannot locate gHeciControlProtocolGuid\n", __FUNCTION__));
    goto MeReadyToBootEventExit;
  }

  MeBiosGetMeMode (&MeMode);
  Heci->HeciFirmwareStatus (Heci, HECI_DEFAULT_PCH_ID, 1, &MeFirmwareStatus.ul);

  ///
  /// Send EOP if ME is at normal mode and ME is ready
  ///
  if ((MeMode == ME_MODE_NORMAL) &&
      ((MeFirmwareStatus.r.CurrentState == ME_STATE_NORMAL) &&
       (MeFirmwareStatus.r.ErrorCode == ME_ERROR_CODE_NO_ERROR))) {
    UINT32 RequestedActions = HeciEopStatusSuccess;

    if (MeEndOfPostEnabled ()) {
      DEBUG ((DEBUG_INFO, "[ME] Sending END_OF_POST to ME\n"));
      Status = MeEndOfPostEvent (&RequestedActions);
      MeSetEndOfPostDone ();

      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "[ME] MeEndOfPostEvent failed with Status = %r\n", Status));
        MeReportError (MSG_EOP_ERROR);
      } else if (RequestedActions == HeciEopPerformGlobalReset) {
        WDT_PROTOCOL        *WdtProtocol;
        PCH_RESET_DATA      ResetData;
        Status = gBS->LocateProtocol (&gWdtProtocolGuid, NULL, (VOID **) &WdtProtocol);
        if (!EFI_ERROR (Status)) {
          WdtProtocol->AllowKnownReset ();
        }
        DEBUG ((DEBUG_INFO, "Global Reset requested by FW EOP ACK %r\n", Status));
        CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
        StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
        gRT->ResetSystem (EfiResetPlatformSpecific, EFI_SUCCESS, sizeof (PCH_RESET_DATA), &ResetData);
      }
    }
  } else {
    DEBUG ((
      DEBUG_ERROR,
      "[ME] EndOfPost not sent due wrong MeMode(0x%x) or MeFirmwareStatus(0x%x)\n",
      MeMode,
      MeFirmwareStatus.ul));
    MeReportError (MSG_EOP_ERROR);
  }

  ///
  /// PMIR Configuration & FDSW Lockdown
  ///
  LockConfig ();

MeReadyToBootEventExit:
  gBS->CloseEvent (Event);
  DEBUG ((DEBUG_INFO, "%a () - end, status: %r\n", __FUNCTION__, Status));
}

/**
  ClientCore driver entry point used to register events "End of Dxe" and "Ready to boot"

  @param[in] ImageHandle          Standard entry point parameter.
  @param[in] SystemTable          Standard entry point parameter.

  @retval EFI_UNSUPPORTED         Unsupported type of ME
  @retval EFI_SUCCESS             Success
**/
EFI_STATUS
EFIAPI
ClientCoreEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS            Status;
  EFI_EVENT             Event;
  BOOLEAN               DisMeDevReq;
  MSR_CPU_TYPE_REGISTER CpuTypeMsr;

  DisMeDevReq  = FALSE;

  DEBUG ((DEBUG_INFO, "%a () - start\n", __FUNCTION__));

  if (!MeTypeIsClient ()) {
    Status = EFI_UNSUPPORTED;
    goto ClientCoreEntryPointExit;
  }

#if AMT_SUPPORT
  InitializeHeciFlow ();
#endif // AMT_SUPPORT

  RegisterD0I3EventsForHeciDevices ();

  //
  // Register End of DXE event
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  MeScriptSaveEvent,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &Event
                  );

  DEBUG ((DEBUG_INFO, "Register EndOfDxe event, status: %r\n", Status));
  ASSERT_EFI_ERROR (Status);

  ///
  /// Create a Ready to Boot event.
  ///
  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             MeReadyToBootEvent,
             (VOID *) &ImageHandle,
             &Event
             );

  DEBUG ((DEBUG_INFO, "Register ReadyToBoot event, status: %r\n", Status));
  ASSERT_EFI_ERROR (Status);

  ///
  /// Create Ready to Boot events. The first event is created with higher TPL level in order to
  /// ensure that SMBIOS tables are updated before they are measured by TPM (see Extended Measurement
  /// boot for more details).
  ///
  DisMeDevReq  = FALSE;
  Status = EfiCreateEventReadyToBootEx (
             TPL_NOTIFY,
             AddFwStsSmbiosEntry,
             (VOID*)&DisMeDevReq,
             &Event
             );

  DEBUG ((DEBUG_INFO, "Register AddFwStsSmbios event, status: %r\n", Status));
  ASSERT_EFI_ERROR (Status);

  //
  // Install reset handler.
  //
  RegisterMeReset ();

  Status = RegisterMeMeasurementsEndOfDxeNotification ();
  ASSERT_EFI_ERROR (Status);

  //
  // For Corporate SKU send CPU brand class to ME
  //
  CpuTypeMsr.Uint64 = AsmReadMsr64 (MSR_CPU_TYPE);

  Status = HeciSendCpuBrandClassFuse ((UINT8) CpuTypeMsr.Bits.FuseCpuType);
  DEBUG ((DEBUG_INFO, "HeciSendCpuBrandClassFuse (CpuBrand = %d), status: %r\n", (UINT8) CpuTypeMsr.Bits.FuseCpuType, Status));

ClientCoreEntryPointExit:
  DEBUG ((DEBUG_INFO, "%a () - end, status: %r\n", __FUNCTION__, Status));
  return EFI_SUCCESS;
}
