/** @file
  Source code file for Silicon Init Post Memory module.

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

#include "SiInit.h"
#include <Library/PerformanceLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PeiP2sbPrivateLib.h>
#include <Register/PchRegs.h>
#include <Register/PchRegsLpc.h>
#include <Register/P2sbRegs.h>
#include <Register/PmcRegs.h>
#include <Register/SpiRegs.h>
#include <PchPcieRpInfo.h>
#include <Register/IehRegs.h>
#include <Register/UsbRegs.h>
#include <Register/MeRegs.h>
#include <Library/MeInitLib.h>
#include <Library/MeChipsetLib.h>
#include <Library/ActiveManagementLib.h>
#include <Library/PeiSiSsidLib.h>

// Signal that DRAM_INIT_DONE message can be sent
GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_PPI_DESCRIPTOR mMeReadyForSendingDidPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gMeReadyForSendingDidPpiGuid,
  NULL
};

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_NOTIFY_DESCRIPTOR  mSiInitNotifyList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiEndOfPeiSignalPpiGuid,
    SiInitOnEndOfPei
  }
};

static EFI_PEI_NOTIFY_DESCRIPTOR  mSiInitPostMemNotifyList[] = {
  {
#ifndef FSP_FLAG
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_DISPATCH | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
#else
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
#endif
    &gSiPolicyPpiGuid,
    SiInitPostMemOnPolicy
  }
};

/**
  Silicon Init End of PEI callback function. This is the last change before entering DXE and OS when S3 resume.

  @param[in] PeiServices   - Pointer to PEI Services Table.
  @param[in] NotifyDesc    - Pointer to the descriptor for the Notification event that
                             caused this function to execute.
  @param[in] Ppi           - Pointer to the PPI data associated with this function.

  @retval EFI_STATUS       - Always return EFI_SUCCESS
**/
EFI_STATUS
SiInitOnEndOfPei (
  IN EFI_PEI_SERVICES                   **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR          *NotifyDesc,
  IN VOID                               *Ppi
  )
{
  EFI_STATUS             Status;
  SI_POLICY_PPI          *SiPolicy;
  SI_CONFIG              *SiConfig;

  //
  // Get Policy settings through the SiPolicy PPI
  //
  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicy
             );
  if (Status != EFI_SUCCESS) {
    ASSERT (FALSE);
    return EFI_SUCCESS;
  }

  Status = GetConfigBlock ((VOID *) SiPolicy, &gSiConfigGuid, (VOID *) &SiConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "SiInitOnEndOfPei - Start\n"));

  //
  // Initializes PCH after End of Pei
  //

  PchOnEndOfPei ();

  //
  // Configure P2SB at the end of EndOfPei
  // This must be done before POSTBOOT_SAI programming.
  //
  P2sbLock (SiPolicy);

  DEBUG ((DEBUG_INFO, "SiInitOnEndOfPei - End\n"));
  return EFI_SUCCESS;
}

/**
  Notify other silicon components that DRAM_INIT_DONE message can be sent.
**/
VOID
NotifyMeReadyForSendingDid (
  VOID
  )
{
  EFI_STATUS Status;

  DEBUG ((DEBUG_INFO, "Sending gMeReadyForSendingDidPpiGuid Notification ...\n"));
  Status = PeiServicesInstallPpi (&mMeReadyForSendingDidPpi);
  DEBUG ((DEBUG_INFO, "Sent gMeReadyForSendingDidPpiGuid Notification (%r)\n", Status));
  ASSERT_EFI_ERROR (Status);
}

/**
  Slicon Initializes after PostMem phase Policy PPI produced,
  All required polices must be installed before the callback

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  SiPolicy PPI.

  @retval EFI_SUCCESS             Succeeds.
  @retval EFI_UNSUPPORTED         The function failed to locate SiPolicy
**/
EFI_STATUS
EFIAPI
SiInitPostMemOnPolicy (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN  VOID                         *Ppi
  )
{
  SI_POLICY_PPI            *SiPolicy;
  EFI_STATUS                Status;
  DEBUG ((DEBUG_INFO, "SiInit () - Start\n"));

  SiPolicy = (SI_POLICY_PPI*) Ppi;

  ///
  /// SubsystemID programming on Internal Devices.
  ///
  DEBUG ((DEBUG_INFO, "SubsystemID programming on Internal Devices\n"));
  SiProgramSsid (SiPolicy, NULL);

  //
  // Initializes ME after memory services initialized
  // Required before PCH to get HSIO information
  //
  MePostMemInit (SiPolicy);

  NotifyMeReadyForSendingDid ();

  //
  // Initializes PCH after memory services initialized
  //
  PchInit (SiPolicy);

  //
  // Initializes I/O Expander after memory services initialized
  //
  IoExpanderInit (SiPolicy);

  AmtPostMemInit ();

  DEBUG ((DEBUG_INFO, "SiInit middle () - End\n"));
  //
  // Install EndOfPei callback function.
  //
  Status = PeiServicesNotifyPpi (mSiInitNotifyList);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "SiInit () - End\n"));


  return EFI_SUCCESS;
}

/**
  Silicon Initializes after memory services initialized

  @param[in] FileHandle           The file handle of the file, Not used.
  @param[in] PeiServices          General purpose services available to every PEIM.

  @retval EFI_SUCCESS             The function completes successfully
**/
EFI_STATUS
EFIAPI
SiInit (
  IN  EFI_PEI_FILE_HANDLE               FileHandle,
  IN CONST EFI_PEI_SERVICES             **PeiServices
  )
{
  EFI_STATUS                Status;

  //
  // Install PostMem phase OnPolicyInstalled callback function.
  //
  Status = PeiServicesNotifyPpi (mSiInitPostMemNotifyList);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
