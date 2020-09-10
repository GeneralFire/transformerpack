/** @file
  PEI Common RC policy update library implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <PiPei.h>

#include <Library/BaseMemoryLib.h>
#include <Library/SetupLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>

#include <Ppi/UpiPolicyPpi.h>
#include <Ppi/MemoryPolicyPpi.h>

#define DFX_DISABLE  0

/**
  Update the Common RC policy from Setup.

  @param[in]  SetupData         A pointer to the setup data.

  @retval None

**/

/**
**/
VOID
GetPlatformSetupValueforCommonRc (
  // APTIOV_SERVER_OVERRIDE_RC_START: To solve structure redefinition error in AMI.
  IN INTEL_SETUP_DATA  *SetupData
  // APTIOV_SERVER_OVERRIDE_RC_END: To solve structure redefinition error in AMI.
  )
{
  UPI_POLICY_PPI              *UpiPolicyPpi;
  UINT8                        i;
  SYS_SETUP                   *setup;
  MEMORY_POLICY_PPI           *MemSetupPpi;
  EFI_STATUS                  Status;
  UINT8                       DfxEnable;

  Status = PeiServicesLocatePpi (
             &gMemoryPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &MemSetupPpi
             );
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return;
  }

  setup = MemSetupPpi->SysSetup;

  UpiPolicyPpi = NULL;

  DfxEnable = DFX_DISABLE;
  if (FixedPcdGetBool (PcdBiosDfxKnobEnabled)) {
    DfxEnable = SetupData->SocketConfig.IioConfig.DFXEnable;
  }

  //
  // Init input parameters here according to setup options
  //
#ifdef   DEBUG_CODE_BLOCK

  {
    EFI_BOOT_MODE               BootMode;

    Status = PeiServicesGetBootMode (&BootMode);
    ASSERT_EFI_ERROR (Status);

    //
    // 5370428: Diable Serial Debug Message Level option doesn't work for the first boot after LBG part being replaced (Opal City)
    // with default setting, let the debug msg level to default; do not force
    /*if (BootMode == BOOT_WITH_DEFAULT_SETTINGS ){
      SerialDebugMsgLevelSetting = SDBG_MAX;
      SetupData->SystemConfig.serialDebugMsgLvlTrainResults = 0;
      SetupData->SystemConfig.serialDebugTrace = 0;
    } */

  }

#endif //DEBUG_CODE_BLOCK

  if (DfxEnable) {
    PcdSet8S (PcdEvMode, 1);
    SetupData->SocketConfig.SocketProcessorCoreConfiguration.ProcessorSmxEnable = 0;
  }

  setup->Pci64BitResourceAllocation = SetupData->SocketConfig.IioConfig.Pci64BitResourceAllocation;

  Status = PeiServicesLocatePpi (
             &gUpiSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &UpiPolicyPpi
             );
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return;
  }


  //
  // SKX_TODO: This variable needs to be extended to 32bit and updated w/ MMIOH base
  // And MRC code needs to consume it when enabling TOHM
  //
  if (UpiPolicyPpi->Upi.mmiohBase == MMIOH_BASE_ICX) {
    setup->common.maxAddrMem = MAX_MEM_ADDR_ICX;     // Supports 52-bit addressing
  } else if (UpiPolicyPpi->Upi.mmiohBase == MMIOH_BASE_SNR) {
    setup->common.maxAddrMem = MAX_MEM_ADDR_SNR;     // Supports 42-bit addressing
  } else {
    setup->common.maxAddrMem = MAX_MEM_ADDR;         // Supports 46-bit addressing
  }

  if (FixedPcdGetBool (PcdBiosDfxKnobEnabled)) {
    for (i = 0; i < MAX_B2P_MAILBOX_GROUPS; i++) {
      setup->common.DfxB2PMailboxCmdEnMask[i] = SetupData->SocketConfig.CommonRcConfig.DfxB2PMailboxCmdEnMask[i];
    }
  }
  setup->common.DfxPpvEnabled = DFX_PPV_AUTO;

} // GetPlatformSetupValueforCommonRc

/**
  Update PEI PSMI policy.

  @param[in, out] SetupData       Pointer to SETUP_DATA structure.

  @return Update status.

**/

EFI_STATUS
EFIAPI
UpdatePeiCommonRcPolicy (
  // APTIOV_SERVER_OVERRIDE_RC_START: To solve structure redefinition error in AMI.
  IN OUT    INTEL_SETUP_DATA          *SetupData
  // APTIOV_SERVER_OVERRIDE_RC_END: To solve structure redefinition error in AMI.
  )
{
  GetPlatformSetupValueforCommonRc (SetupData);

  return EFI_SUCCESS;

} // UpdatePeiCommonRcPolicy

