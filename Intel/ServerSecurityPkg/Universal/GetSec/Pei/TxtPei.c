/** @file
  This is the main PEIM file for LT.  It represents an abstract outline of the
  steps required during PEI for enabling LT.  Each individual step is further
  abstracted behind a function call interface.  This is intended to minimize
  the need to modify this file when porting LT to future platforms.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 - 2019 Intel Corporation. <BR>

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

#include "TxtPeiLib.h"
#include <Library/LtPeiLib.h>
#include <Library/PmcLib.h>


EFI_STATUS
EFIAPI
ScleanRun (
  IN CONST EFI_PEI_SERVICES         **PeiServices,
  IN PEI_TXT_SCLEAN_PPI             *This
  );

PEI_TXT_SCLEAN_PPI  mScleanPpi = {ScleanRun};

STATIC
EFI_PEI_PPI_DESCRIPTOR mScleanPpiList = {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gPeiTxtScleanPpiGuid,
  &mScleanPpi
};

STATIC
EFI_PEI_PPI_DESCRIPTOR mTxtMemoryUnlockPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiTxtMemoryUnlockedPpiGuid,
  NULL
};

/**

  Ensure memory secrets are cleared before lock

  @param PeiServices   - Pointer to PEI Services Table.
  @param This          - PPI pointer

  @retval EFI_SUCCESS - Always

**/
EFI_STATUS
EFIAPI
ScleanRun (
  IN CONST EFI_PEI_SERVICES         **PeiServices,
  IN PEI_TXT_SCLEAN_PPI             *This
  )
{
  EFI_STATUS                        Status;
  LT_PEI_LIB_CONTEXT                LtPeiCtx;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  Status = InitializeLtPeiLib (PeiServices, &LtPeiCtx);
  ASSERT_EFI_ERROR (Status) ;

  DoClearSecrets (&LtPeiCtx) ;
  //
  // Should never get here
  //
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));
  while (1);

  return EFI_SUCCESS ;
}


#ifndef EFI_PLATFORM_TXT_POLICY_NO_SCHECK_IN_RESUME

EFI_STATUS
EFIAPI
BuildScheckCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

EFI_STATUS
EFIAPI
ScheckCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDesc,
  IN VOID                       *Ppi
  );

STATIC
EFI_PEI_NOTIFY_DESCRIPTOR        mBootModeNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMasterBootModePpiGuid,
  BuildScheckCallback
};

STATIC
EFI_PEI_NOTIFY_DESCRIPTOR        mScheckCallbackNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiPostScriptTablePpiGuid,
  ScheckCallback
};

EFI_STATUS
EFIAPI
LtSxMemoryInitializedPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

STATIC
EFI_PEI_NOTIFY_DESCRIPTOR        mMemoryInitializedCallbackNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  LtSxMemoryInitializedPpiNotifyCallback
};


/**

  Executes the SCHECK function from the LT BIOS ACM on S3 Resume.


  @param PeiServices   - Pointer to PEI Services Table.
  @param NotifyDesc    - Pointer to the descriptor for the Notification
                         event that caused this function to execute.
  @param Ppi           - Pointer the the Ppi data associated with this function.


  @retval EFI_SUCCESS in all cases.

**/
EFI_STATUS
EFIAPI
ScheckCallback (
  IN      EFI_PEI_SERVICES          **PeiServices,
  IN      EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDesc,
  IN      VOID                      *Ppi
  )
{
  EFI_STATUS                      Status;
  LT_PEI_LIB_CONTEXT              LtPeiCtx;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  //
  // Initialize LtPeiCtx
  //
  Status = InitializeLtPeiLib (PeiServices, &LtPeiCtx);

  //
  // If conditions are met that we used to set LT Lock Config on initial boot, then lock again on S3.
  // Even if InitializeLtPeiLib returns error, we need to lock LtMsr if needed, check Status after locking
  //

  //
  // If the helper lib is installed at the platform level
  //  then these GetBit procs will return valid values, else just return
  //
  if (Status != EFI_SUCCESS) {
    DEBUG ((EFI_D_INFO, "\tNo helper lib installed at platform level, cannot do LT lock\n"));

  } else {
    if (PcdGetBool (PcdLtConfigLockEnable))
      LockLtMsrOnAll (&LtPeiCtx);

    if ((!PcdGetBool (PcdProcessorLtsxEnable)) || !IsLtEnabled (&LtPeiCtx) || EFI_ERROR (Status)) {
      return EFI_SUCCESS;
    }
    DEBUG ((EFI_D_INFO, "\tIsLtEnabled is enabled\n"));

    PeiTxtLockConfigForAll (&LtPeiCtx) ;
  }
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return EFI_SUCCESS ;
}

/**

  Build ScheckCallback

  @param  PeiServices       - Pointer to the PEI Services Table.
  @param  NotifyDescriptor  - Pointer to the instance of the EFI_PEI_NOTIFY_DESCRIPTOR
  @param  Ppi               - Pointer to the instance of the interface

  @retval EFI_SUCCESS       - ScheckCallback is generated successfully

**/
EFI_STATUS
EFIAPI
BuildScheckCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_BOOT_MODE                   BootMode;
  EFI_STATUS                      Status;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  //
  // Get Boot Mode to see if we're coming back from S3
  //
  Status = (*PeiServices)->GetBootMode (PeiServices, &BootMode);
  ASSERT_EFI_ERROR (Status);

  //
  // If we're resuming from S3 and LT is enabled, we need to run SCHECK
  //
  if (BootMode == BOOT_ON_S3_RESUME) {
    //
    // Register SCHECK to be called when BootScript is finished
    // restoring the platform configuration
    //
    Status = (*PeiServices)->NotifyPpi (PeiServices, &mScheckCallbackNotifyList);
    ASSERT_EFI_ERROR (Status);
  }
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return EFI_SUCCESS;
}

#endif

/**

  Executes the ClearSecretsFlag after memory is discovered.
  System should reset in this routine.

  @param PeiServices   - Pointer to PEI Services Table.
  @param NotifyDesc    - Pointer to the descriptor for the Notification
                         event that caused this function to execute.
  @param Ppi           - Pointer the the Ppi data associated with this function.


  @retval EFI_SUCCESS in all cases.

**/
EFI_STATUS
EFIAPI
LtSxMemoryInitializedPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS              Status;
  LT_PEI_LIB_CONTEXT      LtPeiCtx;
  UINT8                   BiosTrustedBit;
  UINT8                   SecretsBit; 

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  Status = InitializeLtPeiLib (PeiServices, &LtPeiCtx);
  ASSERT_EFI_ERROR (Status) ;

  //
  // TXT is not enabled in PEI phase, so we enable it here for LT-SX detection.
  //
  if (PcdGetBool (PcdProcessorLtsxEnable)) {
    DEBUG ((EFI_D_INFO, "\tLT_SPAD_DONE[0x%08x] = 0x%016lx\n", TXT_PUBLIC_BASE + LT_SPAD_DONE, *(volatile UINT64 *)(UINTN)(TXT_PUBLIC_BASE + LT_SPAD_DONE)));
    DEBUG ((EFI_D_INFO, "\tLT_E2STS[0x%08x] = 0x%016lx\n", TXT_PUBLIC_BASE + LT_E2STS, *(volatile UINT64 *)(UINTN)(TXT_PUBLIC_BASE + LT_E2STS)));
    BiosTrustedBit = (((*(volatile UINT64 *)(UINTN)(TXT_PUBLIC_BASE + LT_SPAD_DONE)) & BIT59) == 0) ? (UINT8) 0 : (UINT8) 1;
    SecretsBit = (((*(volatile UINT32 *)(UINTN)(TXT_PUBLIC_BASE + LT_E2STS)) & BIT1) == 0) ? (UINT8) 0 : (UINT8) 1;
    //
    // Only need to do ClearSecrets if both, BIOS is trusted and the Poison bit is set
    //
    if (BiosTrustedBit && SecretsBit) {
      //
      // Force LTSX code regardless of CPU fusing as long as system/BIOS has LTSX enabled (through FIT table)
      // 
      DoClearSecrets (&LtPeiCtx);
      //
      // Reset platform, should not get here
      //
      Status = EFI_UNSUPPORTED;
      ASSERT_EFI_ERROR (Status);
    }
  }
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return Status ;
}

EFI_STATUS
EFIAPI
TxtPeimEntry (
  IN          EFI_PEI_FILE_HANDLE   FileHandle,
  IN    CONST EFI_PEI_SERVICES      **PeiServices
  )
/**
  This is the entry point to the LT PEIM.  The LT PEIM checks for an LT
  capable platform and determines whether SCLEAN should be run.  If so,
  it launches the BIOS ACM to run SCLEAN (which will reset the platform).
  If not, the PEIM checks to see if the platform is resuming from S3.

  If the platform is resuming from S3, this code will register a callback
  so that SCHECK will be run when BootScript is done restoring the platform's
  configuration.

  @param FfsHeader     - A pointer the the FFS File containing this PEIM.
  @param PeiServices   - A Pointer to the PEI Services Table.

  @retval EFI_UNSUPPORTED   - If the platform is not LT capable.
  @retval EFI_SUCCESS       - In all other cases not listed above.
**/
{
  EFI_STATUS                      Status;
  LT_PEI_LIB_CONTEXT              LtPeiCtx;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  //
  // Initialize the LT PEI Lib functions
  //
  Status = InitializeLtPeiLib (PeiServices, &LtPeiCtx);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (IsLtEnabled (&LtPeiCtx) != FALSE) {
    //
    // Need to install S3 Callback for any situation on resume
    // The callback will determine what is needed to be done after Boot Script
    //
#ifndef EFI_PLATFORM_TXT_POLICY_NO_SCHECK_IN_RESUME
    Status = (*PeiServices)->NotifyPpi (PeiServices, &mBootModeNotifyList);
    ASSERT_EFI_ERROR (Status);
    DEBUG ((EFI_D_INFO, "\tBuildScheckCallback was successfully installed\n"));
#endif

    //
    // Install TXT SClean PPI
    //
    Status = (**PeiServices).InstallPpi (PeiServices, &mScleanPpiList);
    DEBUG ((EFI_D_INFO, "\tScleanRun was successfully installed\n"));
    ASSERT_EFI_ERROR (Status);

    //
    // If TXT Enable then install PPI to tell memory code that it can run.
    //
    Status = (**PeiServices).InstallPpi (PeiServices, &mTxtMemoryUnlockPpiList);
    DEBUG ((EFI_D_INFO, "\tgPeiTxtMemoryUnlockedPpiGuid was successfully installed\n"));
    ASSERT_EFI_ERROR (Status);
  }

  //
  // Determine TPM presence. If not present - nothing to do. Nothing but presence is precondition of this module since all
  // commands executed by BIOS ACM don't depend on TPM state. TPM_NV_read will be successfully executed even if TPM is disabled
  // and/or deactivated because all indices defined for BIOS ACM usage don't require authorization. TPM_ResetEstablishmentBit
  // doesn't depend on TPM state at all and can be executed with disabled/deactivated TPM always.
  // Case when TPM is completely not functional is not considered.
  //
  if (!IsTpmPresent (&LtPeiCtx)) {
    DEBUG ((EFI_D_ERROR, "\tTPM is not present\n"));
    return EFI_UNSUPPORTED;
  }

  //
  // Detect LT capable Processor
  //
  if (!IsLtProcessor (&LtPeiCtx)) {
    DEBUG ((EFI_D_ERROR, "\tCPU does not support TXT\n"));
    return EFI_SUCCESS;
  }

  //
  // Check if we need to run SCLEAN
  //
  if (PmcIsDwrBootMode ()) {
    DEBUG ((EFI_D_ERROR, "\tDWR detected, skipping TPM detection in TxtPei module\n"));
    return EFI_UNSUPPORTED;
  }

  //
  // Setup callback for Clearing Secrets Bits
  //
  Status = (*PeiServices)->NotifyPpi (PeiServices, &mMemoryInitializedCallbackNotifyList);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((EFI_D_INFO, "\tLtSxMemoryInitializedPpiNotifyCallback was successfully installed\n"));
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return EFI_SUCCESS ;
}
