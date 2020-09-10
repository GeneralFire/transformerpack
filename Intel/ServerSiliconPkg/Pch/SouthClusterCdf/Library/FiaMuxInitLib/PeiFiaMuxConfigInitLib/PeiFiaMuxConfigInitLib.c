/** @file
  This file is a library for FIA MUX specific Initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiFiaMuxConfigInitLib.h>
#include <Library/MeFiaMuxLib.h>
#include <PchResetPlatformSpecific.h>
#include <Pch/IncludePrivate/Library/SiScheduleResetLib.h>
#include <Library/SiPolicyLib.h>
#include <ConfigBlock/FiaMuxConfig.h>
#include <Library/HobLib.h>
#include <Library/FiaOverrideStatusHob.h>


/**
  Compare FIA MUX configurations from CSME and from softstraps.

  @retval TRUE   Configurations are identical
  @retval FALSE  Configurations are different
**/
BOOLEAN
CompareFiaMuxConfiguration (
  IN ME_FIA_CONFIG *FiaMuxFromCsme,
  IN ME_FIA_CONFIG *FiaMuxFromSoftStraps
)
{
  if (FiaMuxFromCsme == NULL || FiaMuxFromSoftStraps == NULL) {
    return FALSE;
  }

  if (FiaMuxFromSoftStraps->FiaMuxRecordsCount != FiaMuxFromCsme->FiaMuxRecordsCount) {
    return FALSE;
  }

  if (FiaMuxFromSoftStraps->SataRecordsCount != FiaMuxFromCsme->SataRecordsCount) {
    return FALSE;
  }

  if (FiaMuxFromSoftStraps->PcieRootPortRecordsCount != FiaMuxFromCsme->PcieRootPortRecordsCount) {
    return FALSE;
  }

  if (CompareMem (
        FiaMuxFromCsme->FiaMux,
        FiaMuxFromSoftStraps->FiaMux,
        FiaMuxFromSoftStraps->FiaMuxRecordsCount * sizeof (FIA_MUX_RECORD)) != 0) {
    return FALSE;
  }

  if (CompareMem (
        FiaMuxFromCsme->Sata,
        FiaMuxFromSoftStraps->Sata,
        FiaMuxFromSoftStraps->SataRecordsCount * sizeof (SATA_RECORD)) != 0) {
    return FALSE;
  }

  if (CompareMem (
        FiaMuxFromCsme->PcieRootPort,
        FiaMuxFromSoftStraps->PcieRootPort,
        FiaMuxFromSoftStraps->PcieRootPortRecordsCount * sizeof (PCIE_ROOT_PORT_RECORD)) != 0) {
    return FALSE;
  }

  return TRUE;
}

/**
  Creates FIA_OVERRIDE_STATUS_HOB.

  @param[in] FiaMuxConfigGetStatus     Status returned by MeFiaMuxConfigGet (MeFiaMuxLib).
  @param[in] FiaMuxConfigSetStatus     Status returned by MeFiaMuxConfigSet (MeFiaMuxLib).
  @param[in] FiaMuxConfigSetRequired   Provides information about the need to set the desired FIA lane assignment.
                                         FALSE = the requested config matches the actual one.
                                         TRUE  = the requested config does not match the actual one.

  @retval EFI_SUCCESS            The function completed successfully.
  @retval EFI_OUT_OF_RESOURCES   HOB creation failed.
**/
EFI_STATUS
FiaOverrideStatusCreateHob (
  UINT32    FiaMuxConfigGetStatus,
  UINT32    FiaMuxConfigSetStatus,
  BOOLEAN   FiaMuxConfigSetRequired
  )
{
  FIA_OVERRIDE_STATUS_HOB   FiaOverrideStatusHob;
  VOID                      *HobPtr;
  EFI_STATUS                Status;

  Status = EFI_NOT_READY;

  DEBUG ((DEBUG_INFO, "%a() - Start\n", __FUNCTION__));

  //
  // Initialize the HOB data
  //
  FiaOverrideStatusHob.FiaMuxConfigGetStatus     = FiaMuxConfigGetStatus;
  FiaOverrideStatusHob.FiaMuxConfigSetStatus     = FiaMuxConfigSetStatus;
  FiaOverrideStatusHob.FiaMuxConfigSetRequired   = FiaMuxConfigSetRequired;

  HobPtr = BuildGuidDataHob(&gFiaOverrideStatusHobGuid, &FiaOverrideStatusHob, sizeof(FiaOverrideStatusHob));

  if (HobPtr != NULL) {
    DEBUG((DEBUG_INFO, "FIA MUX Override Status HOB created\n"));
    DEBUG((DEBUG_INFO, "FIA MUX Override Status HOB parameters:\n"));
    DEBUG((DEBUG_INFO, "\tFiaMuxConfigGetStatus = %r,\n", FiaOverrideStatusHob.FiaMuxConfigGetStatus));
    DEBUG((DEBUG_INFO, "\tFiaMuxConfigSetStatus = %r,\n", FiaOverrideStatusHob.FiaMuxConfigSetStatus));
    DEBUG((DEBUG_INFO, "\tFiaMuxConfigSetRequired = %d\n", FiaOverrideStatusHob.FiaMuxConfigSetRequired));
    Status = EFI_SUCCESS;
  } else {
    DEBUG((DEBUG_ERROR, "ERROR: FIA MUX Override Status HOB not created\n"));
    Status = EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO, "%a() - End\n", __FUNCTION__));
  return Status;
}


EFI_STATUS
ReadFiaMuxConfFromSoftStraps (
  OUT ME_FIA_CONFIG *FiaConfigSoftStraps
);

/**
  Initialize FIA MUX configuration.

  Assumption:
  The FIA MUX Policy and FIA MUX HOB structures are created and
  initialized prior to entry into this function. They contain the
  current FIA MUX configuration read from ME.

  Arguments:
  None

  Returns:
  @retval EFI_SUCCESS             The function completed successfully.
  @retval EFI_INVALID_PARAMETER   Invalid parameter
  @retval EFI_NOT_READY           Returned if this API is called before ME signals init
                                  complete, or after END_OF_POST was sent.
  @retval EFI_DEVICE_ERROR        Returned if HECI transport is not available.
  @retval EFI_PROTOCOL_ERROR      Unexpected protocol error returned by ME.
  @retval EFI_UNSUPPORTED         Function not supported on current firmware
  @retval EFI_OUT_OF_RESOURCES    SKU is invalid for current configuration
  @retval EFI_ABORTED             Configuration invalidated
**/
EFI_STATUS
PeiFiaMuxConfigInit (
  VOID
)
{
  EFI_STATUS                  Status;
  SI_PREMEM_POLICY_PPI        *SiPreMemPolicy;
  PCH_FIA_MUX_PREMEM_CONFIG   *FiaMuxPreMemConfig;
  PCH_RESET_DATA              ResetData;
  UINT32                      LanesAllowed;
  BOOLEAN                     OverridesActive;
  UINT32                      FiaMuxConfigStatusGet;
  UINT32                      FiaMuxConfigStatusSet;
  BOOLEAN                     InvalidateCsmeFiaMux;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  if (!MeFiaMuxAllowed ()) {
    DEBUG ((DEBUG_INFO, "%a FiaMux is not allowed\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicy
             );
  if (Status != EFI_SUCCESS) {
    ASSERT (FALSE);
    return Status;
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gFiaMuxPreMemConfigGuid, (VOID *) &FiaMuxPreMemConfig);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

//
// Initialize FIA MUX Config Status HOB parameters with default values.
//
  FiaMuxConfigStatusGet = EFI_NOT_READY;
  FiaMuxConfigStatusSet = EFI_NOT_READY;

  OverridesActive = FALSE;

  //
  // Check if requested FIA MUX configuration same as the current one.
  //
  ME_FIA_CONFIG FiaConfigFromCsme;
  Status = MeFiaMuxConfigGet (
             &(FiaMuxPreMemConfig->FiaMuxConfig),
             &FiaConfigFromCsme,
             &LanesAllowed,
             &OverridesActive
             );
  FiaMuxConfigStatusGet = Status;

  InvalidateCsmeFiaMux = FALSE;

  if (FiaMuxPreMemConfig->FiaMuxOverrideRequired == 1 && FiaMuxPreMemConfig->FiaMuxConfig.FiaMuxCfgInvalidate == 1) {
    // Compare FIA mux configuration from the CSME with that one from soft straps
    ME_FIA_CONFIG FiaConfigSoftStraps;
    if (!EFI_ERROR (ReadFiaMuxConfFromSoftStraps (&FiaConfigSoftStraps))) {
      if (!CompareFiaMuxConfiguration (&FiaConfigFromCsme, &FiaConfigSoftStraps)) {
        InvalidateCsmeFiaMux = TRUE;
        DEBUG ((DEBUG_INFO, "CSME FIA mux configuration must be invalidated.\n"));
      } else {
        DEBUG ((DEBUG_INFO, "No need to invalidate CSME FIA mux configuration.\n"));
      }
    }
  }

  if (FiaMuxPreMemConfig->FiaMuxOverrideRequired) {
    if (!EFI_ERROR (Status) && ((OverridesActive && FiaMuxPreMemConfig->FiaMuxConfig.FiaMuxCfgInvalidate == 0) || InvalidateCsmeFiaMux)) {
      Status = MeFiaMuxConfigSet (&(FiaMuxPreMemConfig->FiaMuxConfig));
      FiaMuxConfigStatusSet = Status;

      if (!EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "Trigger global reset\n"));
        CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
        StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
        SiScheduleResetSetType (EfiResetPlatformSpecific, &ResetData);
      } else {
        //
        // Request ME FIA MUX configuration fail
        //
        DEBUG ((DEBUG_ERROR, "%a: Request ME FIA MUX configuration fail with status = %r\n", __FUNCTION__, Status));
      }
    }
  }

  FiaOverrideStatusCreateHob (FiaMuxConfigStatusGet, FiaMuxConfigStatusSet, OverridesActive);

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
  return Status;
}
