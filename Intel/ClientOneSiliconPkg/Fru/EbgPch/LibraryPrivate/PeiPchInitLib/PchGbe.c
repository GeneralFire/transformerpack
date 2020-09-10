/** @file
  The GBE controller SoC specific implementation.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

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

#include <Ppi/SiPolicy.h>

#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PchInfoLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PeiGbeInitLib.h>
#include <Library/PmcSocLib.h>
#include <Library/PchFiaLib.h>
#include <Library/PeiItssLib.h>
#include <Library/PeiPchPcieClocksLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PchIoExpanderBdfLib.h>
#include <Library/PchPolicyLib.h>
#include <Library/PchIoExpander.h>
#include <Register/PchRegs.h>
#include <PmConfig.h>
#include <GbeConfig.h>
#include <GbeHandle.h>
#include <PchPcieRpConfig.h>
#include <Library/GbeSocLib.h>

/**
  This function checks if GbE controller is supported (not disabled by fuse)

  @param[in]  GbeController  Pointer to GBE controller structure

  @retval GbE support state
**/
STATIC
BOOLEAN
GbeIsSupported (
  IN  GBE_CONTROLLER          *GbeController
  )
{
  return PmcIsGbeSupported ();
}

/**
  This function checks if GbE is function disabled
  by static power gating

  @param[in]  GbeController  Pointer to GBE controller structure

  @retval GbE device state
**/
STATIC
BOOLEAN
GbeIsFunctionDisabled (
  IN  GBE_CONTROLLER          *GbeController
  )
{
  return PmcIsGbeFunctionDisabled ();
}

/**
  This function enables GbE controller by disabling static power gating.
  Static power gating disabling takes place after Global Reset, G3 or DeepSx transition.

  @param[in]  GbeController  Pointer to GBE controller structure
**/
STATIC
VOID
GbeEnable (
  IN  GBE_CONTROLLER          *GbeController
  )
{
  PmcEnableGbe ();
}

/**
  This function disables GbE device by static power gating and enables ModPHY SPD gating (PCH-LP only).
  For static power gating to take place Global Reset, G3 or DeepSx transition must happen.

  @param[in]  GbeController  Pointer to GBE controller structure
**/
STATIC
VOID
GbeDisable (
  IN  GBE_CONTROLLER          *GbeController
  )
{
  PmcStaticDisableGbe ();
}

/**
  Disables one PCIe clock.

  @param[in]  GbeController   Pointer to GBE controller structure
**/
STATIC
VOID
GbeDisableClock (
  IN  GBE_CONTROLLER       *GbeController
  )
{
  DisableClock (PchClockUsageLan);
}

/**
  Load default values to GBE structures
  - Initialize data structures to zero
  - Initialize function pointers to NULL
  - Initialize pointes for GBE_HANDLE substructures

  @param[in out]  GbeHandle      Pointer to GBE handle structure
  @param[in out]  GbePrivate     Pointer to GBE private configuration structure
  @param[in out]  GbeController  Pointer to GBE controller identification structure
  @param[in out]  GbeCallback    Pointer to GBE callback structure
**/
STATIC
VOID
GbeHandleLoadDefaults (
  IN OUT  GBE_HANDLE          *GbeHandle,
  IN OUT  GBE_PRIVATE_CONFIG  *GbePrivate,
  IN OUT  GBE_CONTROLLER      *GbeController,
  IN OUT  GBE_CALLBACK        *GbeCallback
  )
{
  ZeroMem (GbePrivate, sizeof (GbePrivate));
  GbeHandle->PrivateConfig = GbePrivate;

  ZeroMem (GbeCallback, sizeof (GbeCallback));
  GbeHandle->Callback = GbeCallback;

  ZeroMem (GbeController, sizeof (GbeController));
  GbeHandle->Controller = GbeController;

  GbeHandle->GbeConfig = NULL;
}

/**
  Initialize GBE structures - Common for PCH Legacy and I/O Expander
  This should be done according GBE IP integration in SoC

  @param[in]      SiPolicy       Pointer to Silicon Policy
  @param[in out]  GbeHandle      Pointer to GBE handle structure
  @param[in out]  GbePrivate     Pointer to GBE private configuration structure
  @param[in out]  GbeController  Pointer to GBE controller identification structure
  @param[in out]  GbeCallback    Pointer to GBE callback structure
**/
STATIC
VOID
GbeHandleInitCommon (
  IN      SI_POLICY_PPI       *SiPolicy,
  IN OUT  GBE_HANDLE          *GbeHandle,
  IN OUT  GBE_PRIVATE_CONFIG  *GbePrivate,
  IN OUT  GBE_CONTROLLER      *GbeController,
  IN OUT  GBE_CALLBACK        *GbeCallback
  )
{
  EFI_STATUS            Status;
  PCH_PM_CONFIG         *PmConfig;

  //
  // Initialize: data with defaults, function pointers to NULL, substructures pointers
  //
  GbeHandleLoadDefaults (GbeHandle, GbePrivate, GbeController, GbeCallback);

  //
  // Initialize GBE Private Configuration
  //
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPmConfigGuid, (VOID *) &PmConfig);
  if (!EFI_ERROR (Status)) {
    GbePrivate->WolEnableOverride = PmConfig->WakeConfig.WolEnableOverride != 0;
  }
  GbePrivate->InterruptPin = ItssGetDevIntPin (SiPolicy, GbeDevNumber (), GbeFuncNumber ());

  if (!PchFiaGetGbeLaneNum (&GbePrivate->LaneNum)) {
    GbePrivate->LaneNum = 0xFF; // no lane assigned
  }

  if (IsClkReqAssigned (PchClockUsageLan)) {
    GbePrivate->HasClkReq = TRUE;
  }


  //
  // Initialize GBE callback pointers
  //
  GbeCallback->IsSupported = GbeIsSupported;
  GbeCallback->IsFunctionDisabled = GbeIsFunctionDisabled;
  GbeCallback->Enable = GbeEnable;
  GbeCallback->Disable = GbeDisable;
  GbeCallback->ModPhyPowerGating = NULL;
  GbeCallback->DisableClk = GbeDisableClock;

  //
  // Set pointer to GBE Config Block
  //
  if (SiPolicy) {
    Status = GetConfigBlock ((VOID *) SiPolicy, &gGbeConfigGuid, (VOID *) &GbeHandle->GbeConfig);
    ASSERT (GbeHandle->GbeConfig != NULL);
  }
}

/**
  Initialize GBE structures - PCH Legacy
  This should be done according GBE IP integration in SoC

  @param[in]      SiPolicy       Pointer to Silicon Policy
  @param[in out]  GbeHandle      Pointer to GBE handle structure
  @param[in out]  GbePrivate     Pointer to GBE private configuration structure
  @param[in out]  GbeController  Pointer to GBE controller identification structure
  @param[in out]  GbeCallback    Pointer to GBE callback structure
**/
STATIC
VOID
PchGbeHandleInit (
  IN      SI_POLICY_PPI       *SiPolicy,
  IN OUT  GBE_HANDLE          *GbeHandle,
  IN OUT  GBE_PRIVATE_CONFIG  *GbePrivate,
  IN OUT  GBE_CONTROLLER      *GbeController,
  IN OUT  GBE_CALLBACK        *GbeCallback
  )
{
  EFI_STATUS            Status;

  GbeHandleInitCommon (SiPolicy, GbeHandle, GbePrivate, GbeController, GbeCallback);

  //
  // Initialize GBE device data
  //
  Status = GbeGetController (GbeController);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR: Cannot get GBE controller!\n", __FUNCTION__));
    ASSERT (FALSE);
  }

  GbeHandle->TempBar           = PcdGet32 (PcdSiliconInitTempMemBaseAddr);
}

/**
  Initialize GBE structures - I/O Expander
  This should be done according GBE IP integration in SoC

  @param[in]      SiPolicy       Pointer to Silicon Policy
  @param[in out]  GbeHandle      Pointer to GBE handle structure
  @param[in out]  GbePrivate     Pointer to GBE private configuration structure
  @param[in out]  GbeController  Pointer to GBE controller identification structure
  @param[in out]  GbeCallback    Pointer to GBE callback structure
**/
STATIC
VOID
IoExpanderGbeHandleInit (
  IN      SI_POLICY_PPI       *SiPolicy,
  IN      UINT32              IoeId,
  IN OUT  GBE_HANDLE          *GbeHandle,
  IN OUT  GBE_PRIVATE_CONFIG  *GbePrivate,
  IN OUT  GBE_CONTROLLER      *GbeController,
  IN OUT  GBE_CALLBACK        *GbeCallback
  )
{
  EFI_STATUS            Status;

  GbeHandleInitCommon (SiPolicy, GbeHandle, GbePrivate, GbeController, GbeCallback);

  //
  // Override for I/O Expander callbacks until it will be valid
  //
  GbeCallback->IsSupported = NULL;
  GbeCallback->IsFunctionDisabled = NULL;
  GbeCallback->Enable = NULL;
  GbeCallback->Disable = NULL;
  GbeCallback->ModPhyPowerGating = NULL;
  GbeCallback->DisableClk = NULL;

  GbePrivate->HasClkReq = FALSE;

  //
  // Initialize GBE controller data
  //
  Status = IoExpanderGbeGetController (IoeId, GbeController);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR: Cannot get GBE controller!\n", __FUNCTION__));
    ASSERT (FALSE);
  }

  GbeHandle->TempBar           = PchGetTempBarByIoExpanderId (IoeId);
}

/**
  Get GBE controller instance for I/O Expander

  @param[in]   IoeId               I/O Expander Id (0 ... n - I/O Expander)
  @param[out]  GbeController       Pointer to GBE controller identification structure

  @retval     EFI_SUCCESS           - Completed successfully
              EFI_INVALID_PARAMETER - GbeDevice NULL
**/
EFI_STATUS
IoExpanderGbeGetController (
  IN  UINT32             IoeId,
  OUT GBE_CONTROLLER     *GbeController
  )
{
  EFI_STATUS            Status;

  if (GbeController == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (IoeId >= IO_EXPANDER_MAX) {
    return EFI_INVALID_PARAMETER;
  }

  Status = PchGetSegmentByIoExpanderId (IoeId, &GbeController->Segment);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = PchGetBusByIoExpanderId (IoeId, &GbeController->Bus);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  GbeController->Device            = GbeDevNumber ();
  GbeController->Function          = GbeFuncNumber ();
  GbeController->PciCfgBaseAddr    = GbePciCfgBaseByIoExpanderId (IoeId);

  return EFI_SUCCESS;
}

/**
  Initialize LAN device on PCH

  @param[in]      SiPolicy       Pointer to Silicon Policy
**/
VOID
EbgPchGbeInit (
  IN  SI_POLICY_PPI  *SiPolicy
  )
{
  GBE_HANDLE           GbeHandle;
  GBE_PRIVATE_CONFIG   GbePrivate;
  GBE_CONTROLLER       GbeController;
  GBE_CALLBACK         GbeCallback;

  PchGbeHandleInit (SiPolicy, &GbeHandle, &GbePrivate, &GbeController, &GbeCallback);

  //
  // Enable CLKREQ# if supported by board regardless of GbE being enabled
  // to allow clock shut-down.
  //
  if (GbePrivate.HasClkReq) {
    EnableClkReq (PchClockUsageLan);
  }

  GbeInit (&GbeHandle);
}

/**
  Initialize LAN device on I/O Expander

  @param[in]      SiPolicy       Pointer to Silicon Policy
  @param[in]      IoeId          I/O Expander Id (0 ... n - I/O Expander)
**/
VOID
EbgIoExpanderGbeInit (
  IN  SI_POLICY_PPI  *SiPolicy,
  IN  UINT32         IoeId
  )
{
  GBE_HANDLE           GbeHandle;
  GBE_PRIVATE_CONFIG   GbePrivate;
  GBE_CONTROLLER       GbeController;
  GBE_CALLBACK         GbeCallback;

  IoExpanderGbeHandleInit (SiPolicy, IoeId, &GbeHandle, &GbePrivate, &GbeController, &GbeCallback);
  //
  // Enable CLKREQ# if supported by board regardless of GbE being enabled
  // to allow clock shut-down.
  //
  if (GbePrivate.HasClkReq) {
    EnableClkReq (PchClockUsageLan);
  }

  GbeInit (&GbeHandle);
}
