/** @file
  This is the main DXE file for LT.  It represents an abstract outline of the
  steps required during DXE for enabling LT.  Each individual step is further
  abstracted behind a function call interface.  This is intended to minimize
  the need to modify this file when porting LT to future platforms.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 - 2020 Intel Corporation. <BR>

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

#include "TxtDxeLib.h"
#include "TxtDxeProtocol.h"
#include <Library/LtDxeLib.h>
#include <Library/PreSiliconEnvDetectLib.h> //for IsSimicsEnvironment()

LT_DXE_LIB_CONTEXT          mLtDxeCtx ;
BOOLEAN                    ScheckDone = FALSE;
BOOLEAN                     LtDxeEnabled = TRUE;
BOOLEAN                     DisableBIOSACMFlag = FALSE;

/**

  This function gets registered as a callback to run the SCHECK function
  from the LT BIOS ACM as a result of Boot Events.

  @param Event     - A pointer to the Event that triggered the callback.
  @param Context   - A pointer to private data registered with the callback function.

  @retval EFI_SUCCESS   - Always.

**/
EFI_STATUS
EFIAPI
ScheckCallback (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EFI_STATUS          Status;
  UINT32              Data32;
  UINT8               BiosAcmErrorResetBit;
  UINT8               AcmTypeValue;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
#ifdef SKX_HOST
  DEBUG ((EFI_D_INFO, "DXE DoScheck ...\n"));
  DoScheck (&mLtDxeCtx) ;
  DEBUG ((EFI_D_INFO, "DoScheck Done!\n"));
#endif // SKX_HOST
//
// PURLEY_OVERRIDE_BEGIN
//
//  WA_UnLock();
//
// PURLEY_OVERRIDE_END
//
// APTIOV_SERVER_OVERRIDE_RC_START
    PrintLtsxErrors();   // TXT Error and status Register message printing.
// APTIOV_SERVER_OVERRIDE_RC_END

  // If LT_SPAD_DONE_LOW 0xFED300A0 BIT30 = 0, do legacy boot
  Data32 = *(volatile UINT32 *)(UINTN)(TXT_PUBLIC_BASE + LT_SPAD_DONE_LOW);
  DEBUG ((EFI_D_INFO, "\tLT_SPAD_DONE_LOW[0x%08x] = 0x%08x\n", TXT_PUBLIC_BASE + LT_SPAD_DONE_LOW, Data32));

  if ((BIT30 & (*(volatile UINT32 *)(UINTN)(TXT_PUBLIC_BASE + LT_SPAD_DONE_LOW))) == 0) {
    // BIOS ACM DoScheck error
    DEBUG ((EFI_D_ERROR, "\tBIOS ACM DoScheck Failed. Reboot in Non-Ltsx mode\n"));

    // When BIOS ACM error occurs, If BiosAcmErrorReset setup option is enabled,
    // Disable LT-SX and reset system
    Status = GetBiosAcmErrorReset (&BiosAcmErrorResetBit);

    if ( (Status != EFI_SUCCESS) || BiosAcmErrorResetBit) {
      DisableLtsxAndReset ();
    } else {
      // When BIOS ACM error occurs, If BiosAcmErrorReset setup option is disabled,
      // Ignore BIOS ACM errors and continue booting with no further BIOSACM being called
      //
      Status = GetAcmType (&AcmTypeValue);

      if (Status != EFI_SUCCESS) {
        DEBUG ((EFI_D_ERROR, "\tERROR: No LtDxe Lib file, no platform functions available\n"));
      } else {
        DEBUG ((EFI_D_ERROR, "\tERROR: IGNORING ACM ERROR - ACM Type %x detected - BIOS setup option is set to ignore ACM error\n",
          AcmTypeValue));
      }
      gBS->CloseEvent (Event);
      return EFI_SUCCESS;
    }
  }

  TxtLockConfigForAll (&mLtDxeCtx) ;
  //
  // If LT_SPAD_DONE_LOW 0xFED300A0 BIT30 = 0, do legacy boot
  //
  Data32 = *(volatile UINT32 *)(UINTN)(TXT_PUBLIC_BASE + LT_SPAD_DONE_LOW);
  DEBUG ((EFI_D_INFO, "\tLT_SPAD_DONE_LOW[0x%08x] = 0x%08x\n", TXT_PUBLIC_BASE + LT_SPAD_DONE_LOW, Data32));

  if ((BIT30 & (*(volatile UINT32 *)(UINTN)(TXT_PUBLIC_BASE + LT_SPAD_DONE_LOW))) == 0) {
    //
    // BIOS ACM DoScheck error
    //
    DEBUG ((EFI_D_ERROR, "\tBIOS ACM LockConfig Failed. Reboot in Non-Ltsx mode\n"));
    //
    // When BIOS ACM error occurs, If BiosAcmErrorReset setup option is enabled,
    // Disable LT-SX and reset system
    //
    Status = GetBiosAcmErrorReset (&BiosAcmErrorResetBit);

    if ( (Status != EFI_SUCCESS) || BiosAcmErrorResetBit) {
      DisableLtsxAndReset ();
    } else {
      //
      // When BIOS ACM error occurs, If BiosAcmErrorReset setup option is disabled,
      // Ignore BIOS ACM errors and continue booting with no further BIOSACM being called
      //
      Status = GetAcmType (&AcmTypeValue);

      if (Status != EFI_SUCCESS) {
        DEBUG ((EFI_D_ERROR, "\tERROR: No LtDxe Lib file, no platform funtions available\n"));
      } else {
        DEBUG ((EFI_D_ERROR, "\tERROR: IGNORING ACM ERROR - ACM Type %x detected - BIOS setup option is set to ignore ACM error\n",
          AcmTypeValue));
      }

      gBS->CloseEvent (Event);
      return EFI_SUCCESS;
    }
  }
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return EFI_SUCCESS ;
}

/**

  Lock MSR for this thread.

  @param None

  @retval EFI_SUCCESS           - MSR Locked

**/
EFI_STATUS
EFIAPI
LockLTPatch (
  IN  EFI_MP_SERVICES_PROTOCOL  *MpService

  )
{
  UINT64              MsrLtCtrlSts;

  //
  // Set LT_CONTROL_LOCK
  //
  MsrLtCtrlSts = 0x1;
  AsmWriteMsr64 (MSR_LTCTRLSTS, MsrLtCtrlSts);

  return EFI_SUCCESS;
}

/**

  Lock LT MSR.

  @param None

  @retval EFI_SUCCESS           - Multiple processors re-load microcode patch

**/
EFI_STATUS
LockLtMsr (
  VOID
  )
{
  EFI_STATUS              Status;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  LockLTPatch (mLtDxeCtx.MpService);
  if (mLtDxeCtx.MpService == NULL) {
    ASSERT (mLtDxeCtx.MpService);
    return EFI_INVALID_PARAMETER;
  }
  Status = mLtDxeCtx.MpService->StartupAllAPs (
                        mLtDxeCtx.MpService,
                        LockLTPatch,
                        FALSE,//OVERRIDE_HSD_5371612
                        NULL,
                        0,
                        (VOID *)mLtDxeCtx.MpService,
                        NULL
                        );
  DEBUG ((EFI_D_INFO, "\tMSR_LTCTRLSTS[0x%08x] = 0x%016lx\n", MSR_LTCTRLSTS, AsmReadMsr64 (MSR_LTCTRLSTS)));
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return EFI_SUCCESS;
}

/**

Callback function to register Lock event;

@param Event
@param Context

@retval VOID

**/
VOID
EFIAPI
DummyNotificationFunction (
      EFI_EVENT                 Event,
      VOID                      *Context
  )
{

}
/**

  Callback function to lock down chipset after all chipset config is done

  @param EFI_HANDLE                                   RootBridgeHandle
  @param EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS  PciAddress
  @param EFI_PCI_ENUMERATION_PHASE                    Phase
  @param EFI_PCI_CALLBACK_CONTEXT                     *Context

  @retval EFI_SUCCESS           - Multiple processors re-load microcode patch

**/
VOID
EFIAPI
LTLockEventNotify (
  IN  EFI_HANDLE                                   RootBridgeHandle,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS  PciAddress,
  IN  EFI_PCI_ENUMERATION_PHASE                    Phase,
  IN  EFI_PCI_CALLBACK_CONTEXT                     *Context
  )
{
  EFI_STATUS    Status;
  EFI_EVENT     ChipsetLockEvent;

  ChipsetLockEvent = NULL;
  if (!ScheckDone) {
    DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
    //
    // Only lock when disabled if LockChipset is set and EVMode is disabled
    // LtDxe Lib functions in platform must be installed to read LockChipset and EVMode bits
    //
    if (PcdGetBool (PcdLtConfigLockEnable)) {
      LockLtMsr ();
    }

    if (PcdGetBool (PcdProcessorLtsxEnable)) {
        ScheckCallback (0, NULL);
    }
    ScheckDone = TRUE;
    DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));
  }
  //
  // Signal LtLock Event
  //
  Status = gBS->CreateEventEx (
      EVT_NOTIFY_SIGNAL,
      TPL_CALLBACK,
      DummyNotificationFunction,
      NULL,
      &gChipsetLockDoneGuid,
      &ChipsetLockEvent
  );
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status)) {
      gBS->SignalEvent (ChipsetLockEvent);
  }
}

/**

  This is the entry point to the LT DXE Driver.  This routine checks to see if
  the platform should be configured for LT and if so, configures the platform
  by reserving and initializing LT Configuration Space and LT Device Memory and
  registering a callback to run SCHECK from the LT BIOS ACM prior to boot.

  If the platform should not be configured for LT, this routine checks the
  establishment bit in the TPM and resets it if it is asserted.

  @param ImageHandle       - A handle for this module
  @param SystemTable       - A pointer to the EFI System Table

  @retval EFI_SUCCESS   - Always.

**/
EFI_STATUS
EFIAPI
DriverEntry (
  IN      EFI_HANDLE          ImageHandle,
  IN      EFI_SYSTEM_TABLE    *SystemTable
  )
{
  EFI_STATUS                  Status;
  UINT32                      Data32;
  EFI_PCI_CALLBACK_PROTOCOL   *PciCallBackProtocol;
  UINT8                       BiosAcmErrorResetBit;
  UINT8                       AcmTypeValue;
  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));

// OVERRIDE_HSD_4757591_BEGIN
  EFI_PHYSICAL_ADDRESS        TXTPublicBase  = TXT_PUBLIC_PRIVATE_BASE; //Added using sdl token
// OVERRIDE_HSD_4757591_END
// APTIOV_SERVER_OVERRIDE_RC_START  
//Moved later to get ACM type to get BiosAcmAddress from PlatformTxtPolicyData
#if 0
  //
  // Find ACM Binary type to Suppress "BIOS ACM Error Reset" setup option, if PW ACM is present.
  //
  Status = FindAcmBinaryType ();

  if (Status != EFI_SUCCESS) {
    DEBUG ((EFI_D_ERROR, "\tACM Binary was not found\n"));
    return EFI_UNSUPPORTED;
  }
#endif
// APTIOV_SERVER_OVERRIDE_RC_END
  //
  // Register for protocol notifications, because ReadyToBoot is too late
  // Need to lock LT config MSR whether it is enabled or not
  //
  Status = gBS->LocateProtocol (
                 &gEfiPciCallbackProtocolGuid,
                 NULL,
                 &PciCallBackProtocol
                 );
  ASSERT_EFI_ERROR (Status);

  //
  // Register for Scheck/LockConfig Callback
  //
  Status = PciCallBackProtocol->RegisterPciCallback (
                         PciCallBackProtocol,
                         LTLockEventNotify,
                         EfiPciEnumerationResourceAssigned
                         );
  ASSERT_EFI_ERROR (Status);

  //
  // Initialize the LT DXE Lib functions
  //
  Status = InitializeLtDxeLib (ImageHandle, SystemTable, &mLtDxeCtx);
  if (EFI_ERROR (Status)) {
    LtDxeEnabled = FALSE;
    DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));
    return EFI_SUCCESS;
  }
// APTIOV_SERVER_OVERRIDE_RC_START
  Status = FindAcmBinaryType(&mLtDxeCtx);
  if (Status != EFI_SUCCESS) {
    DEBUG ((EFI_D_ERROR, "\tACM Binary was not found\n"));
    return EFI_UNSUPPORTED;
  }
// APTIOV_SERVER_OVERRIDE_RC_END : Moved later to get ACM type to get BiosAcmAddress from PlatformTxtPolicyData
  //
  // Determine TPM presence. If not present - nothing to do. Nothing but presence is precondition of this module since all
  // commands executed by BIOS ACM don't depend on TPM state. TPM_NV_read will be successfully executed even if TPM is disabled
  // and/or deactivated because all indices defined for BIOS ACM usage don't require authorization. TPM_ResetEstablishmentBit
  // doesn't depend on TPM state at all and can be executed with disabled/deactivated TPM always.
  // Case when TPM is completely not functional is not considered.
  //
  if (!IsTpmPresent (&mLtDxeCtx)) {
    DEBUG ((EFI_D_ERROR, "\tTpm is not present\n"));
    LtDxeEnabled = FALSE;
    return EFI_SUCCESS;
  }

  //
  // Detect LT capable Processor
  //
  if (!IsLtProcessor (&mLtDxeCtx)) {
    DEBUG ((EFI_D_ERROR, "\tNot LT processor\n"));
    LtDxeEnabled = FALSE;
    return EFI_SUCCESS ;
  }

  //
  // If LT is enabled, configure platform appropriately
  //
  if (IsLtEnabled (&mLtDxeCtx)) {
    //
    // If LT_SPAD_DONE_LOW 0xFED300A0 BIT30 = 0, do legacy boot
    //
    Data32 = *(volatile UINT32 *)(UINTN)(TXT_PUBLIC_BASE + LT_SPAD_DONE_LOW);
    DEBUG ((EFI_D_INFO, "\tLT_SPAD_DONE_LOW[0x%08x] = 0x%08x\n", TXT_PUBLIC_BASE + LT_SPAD_DONE_LOW, Data32));

    if ((BIT30 & (*(volatile UINT32 *)(UINTN)(TXT_PUBLIC_BASE + LT_SPAD_DONE_LOW))) == 0) {
      //
      // BIOS ACM error
      //
      DEBUG ((EFI_D_ERROR, "\tBIOS ACM Failed. Reboot in non-ltsx mode\n"));
      Status = GetBiosAcmErrorReset (&BiosAcmErrorResetBit);
      if ((Status == EFI_SUCCESS) && BiosAcmErrorResetBit) {
        DisableLtsxAndReset ();
      } else {
        Status = GetAcmType (&AcmTypeValue);
        if (Status == EFI_SUCCESS) {
          DEBUG ((EFI_D_ERROR, "\tERROR: IGNORING ACM ERROR - ACM Type %x detected - BIOS setup option is set to ignore ACM error\n",
            AcmTypeValue));
        } else {
          DEBUG ((EFI_D_ERROR, "\tERROR: No LtDxe Lib functions installed - BIOS setup option is set to ignore ACM error\n"));
        }
        DisableBIOSACMFlag = TRUE;
      }
    }

    //
    // Initialize LT Device Memory
    //
    Status = SetupLtDeviceMemory (&mLtDxeCtx);
    //
    // OVERRIDE_HSD_4757591_BEGIN
    //
    //
    // Add TPM Decode and TXT Public and Private memory region as Runtime memory so that it can be reported in E820 table
    //
    Status = gDS->AddMemorySpace (
                    EfiGcdMemoryTypeMemoryMappedIo,
                    TXTPublicBase,
                    TXT_PUBLIC_PRIVATE_SIZE,
                    EFI_MEMORY_RUNTIME | EFI_MEMORY_UC
                    );
    ASSERT_EFI_ERROR (Status);

    Status = gDS->AllocateMemorySpace (
                    EfiGcdAllocateAddress,
                    EfiGcdMemoryTypeMemoryMappedIo,
                    0,
                    TXT_PUBLIC_PRIVATE_SIZE,
                    &TXTPublicBase,
                    ImageHandle,
                    NULL
                    );
    ASSERT_EFI_ERROR (Status);

    Status = gDS->SetMemorySpaceAttributes (
                    TXTPublicBase,
                    TXT_PUBLIC_PRIVATE_SIZE,
                    EFI_MEMORY_RUNTIME | EFI_MEMORY_UC
                    );
    ASSERT_EFI_ERROR (Status);
    //
    // OVERRIDE_HSD_4757591_END
    //
  } else {
    Status = EFI_UNSUPPORTED;
  }
  //
  //
  // Install TXT DXE Protocol so that functions in TXT DXE Driver can be used by other modules
  if (Status == EFI_SUCCESS) {
    Status = InstallTxtDxeProtocol (ImageHandle, SystemTable);
    DEBUG ((EFI_D_INFO, "\tInstall TXT_DXE_PROTOCOL..."));
    if (Status == EFI_SUCCESS) {
      DEBUG ((EFI_D_INFO, "Success\n"));
    } else {
      DEBUG ((EFI_D_ERROR, "Error\n"));
    }
  }

  if (Status != EFI_SUCCESS) {
    DEBUG ((EFI_D_ERROR, "\tLT not enabled\n"));
    //
    // LT is not enabled, so make sure TPM Establishment bit is de-asserted
    //
    if (IsLtEstablished (&mLtDxeCtx)) {
      Status = ResetTpmEstBit (&mLtDxeCtx) ;
      ASSERT_EFI_ERROR (Status) ;
    }
  }
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return EFI_SUCCESS ;
}
