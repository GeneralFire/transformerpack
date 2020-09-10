/** @file
  The SATA controller SoC specific implementation.

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

#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiRstPrivateLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PeiSataLib.h>
#include <Library/PchFiaLib.h>
#include <Library/PmcSocLib.h>
#include <Library/PsfLib.h>
#include <Library/PeiHsioLib.h>
#include <Library/HsioSocLib.h>
#include <Library/GpioPrivateLib.h>
#include <Library/PchFiaLib.h>
#include <Library/SataSocLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PchIoExpanderBdfLib.h>
#include <Library/PchPolicyLib.h>
#include <Library/PeiItssLib.h>
#include <Library/PchIoExpander.h>
#include <Library/SataLib.h>
#include <SataSocIntegration.h>

/**
  Checks if SATA controller has lanes connected

  @param[in]  SataController    Pointer to SATA Controller structure
  @param[in]  Port              Port number (0 based) to be examined

  @retval TRUE if SATA controller has lanes assigned, otherwise FALSE
**/
BOOLEAN
EbgIsSataPortConnected (
  IN  SATA_CONTROLLER   *SataController,
  IN  UINT8             Port
  )
{
  BOOLEAN PhyConnected;

  if (SataController == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: NULL pointer detected!\n", __FUNCTION__));
    return FALSE;
  }

  PhyConnected = PchFiaIsSataPortConnected (SataController->SataCtrlIndex, Port);

  DEBUG ((DEBUG_INFO, "%a: Port: %d PHY %a connected\n", __FUNCTION__, Port, (PhyConnected) ? "" : "NOT"));

  return PhyConnected;
}

/**
  Checks if SATA controller has lanes connected

  Currently on I/O Expander it's always return true

  @param[in]  SataController    Pointer to SATA Controller structure
  @param[in]  Port              Port number (0 based) to be examined

  @retval TRUE if SATA controller has lanes assigned, otherwise FALSE
**/
BOOLEAN
EbgIoExpanderIsSataPortConnected (
  IN  SATA_CONTROLLER   *SataController,
  IN  UINT8             Port
  )
{
  return TRUE;
}

/**
  Configures GPIO pins for SATA DevSlp

  @param[in]  SataController    Pointer to SATA Controller structure
  @param[in]  Port              Port number (0 based) for Dev Slp enable
  @param[in]  ResetType         GPIO reset type (see GPIO_RESET_CONFIG)

  @retval EFI_STATUS      Status returned by worker function
**/
EFI_STATUS
EbgSataDevSlpEnable (
  IN  SATA_CONTROLLER   *SataController,
  IN  UINT8             Port,
  IN  UINT32            ResetType
  )
{
  EFI_STATUS Status;

  if (SataController == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: NULL pointer detected!\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  Status = GpioEnableSataDevSlpPin (SataController->SataCtrlIndex, Port, ResetType);

  DEBUG ((DEBUG_INFO, "%a: Status: %r\n", __FUNCTION__, Status));

  return Status;
}

/**
  Configures GPIO pins for SATA Hot Plug

  @param[in]  SataController    Pointer to SATA Controller structure
  @param[in]  Port              Port number (0 based) for Hot Plug enable

  @retval EFI_STATUS      Status returned by worker function
**/
EFI_STATUS
EbgSataHotPlugEnable (
  IN  SATA_CONTROLLER    *SataController,
  IN  UINT8              Port
  )
{
  EFI_STATUS Status;

  if (SataController == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: NULL pointer detected!\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  Status = GpioEnableSataGpPin (SataController->SataCtrlIndex, Port);

  DEBUG ((DEBUG_INFO, "%a: Status: %r\n", __FUNCTION__, Status));

  return Status;
}

/**
  Configures GPIO pins for SATA Serial GPIO

  @param[in]  SataController     Pointer to SATA Controller structure

  @retval EFI_STATUS      Status returned by worker function
**/
EFI_STATUS
EbgSataSgpioEnable (
  IN  SATA_CONTROLLER    *SataController
  )
{
  EFI_STATUS Status;

  if (SataController == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: NULL pointer detected!\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  Status = GpioEnableSataSgpio (SataController->SataCtrlIndex);

  DEBUG ((DEBUG_INFO, "%a: Status: %r\n", __FUNCTION__, Status));

  return Status;
}

/**
  Configures GPIO pins for SATA LED

  @param[in]  SataController     Pointer to SATA Controller structure

  @retval EFI_STATUS      Status returned by worker function
**/
EFI_STATUS
EbgSataLedEnable (
  IN  SATA_CONTROLLER    *SataController
  )
{
  EFI_STATUS Status;

  if (SataController == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: NULL pointer detected!\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  Status =  GpioEnableSataLed (SataController->SataCtrlIndex);

  DEBUG ((DEBUG_INFO, "%a: Status: %r\n", __FUNCTION__, Status));

  return Status;
}

/**
  Disables SATA controller in power controller and fabric

  @param[in]  SataController     Pointer to SATA Controller structure

**/
VOID
EbgSataCtrlDisable (
  IN  SATA_CONTROLLER    *SataController
  )
{

  if (SataController == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: NULL pointer detected!\n", __FUNCTION__));
    return;
  }

  PsfDisableSataDevice (SataController->SataCtrlIndex);
  PmcDisableSata (SataController->SataCtrlIndex);
}

/**
  Enables SATA test mode in PHY

  @param[in]  SataController     Pointer to SATA Controller structure

**/
VOID
EbgSataTestModeEnable (
  IN  SATA_CONTROLLER    *SataController
  )
{
  HsioSataTestModeEnable (SataController->SataCtrlIndex);
}

/**
  Load default values to SATA structures
  - Initialize data structures to zero
  - Initialize function pointers to NULL
  - Initialize pointes for SATA_HANDLE substructures

  @param[in out]  Sata            Pointer to SATA handle structure
  @param[in out]  SataPrivate     Pointer to SATA private configuration structure
  @param[in out]  SataController  Pointer to SATA controller identification structure
  @param[in out]  SataCallback    Pointer to SATA callback structure
**/
VOID
SataHandleLoadDefaults (
  IN OUT  SATA_HANDLE         *Sata,
  IN OUT  SATA_PRIVATE_CONFIG *SataPrivate,
  IN OUT  SATA_CONTROLLER     *SataController,
  IN OUT  SATA_CALLBACK       *SataCallback
  )
{
  ZeroMem (SataPrivate, sizeof (SataPrivate));
  Sata->PrivateConfig = SataPrivate;

  ZeroMem (SataCallback, sizeof (SataCallback));
  Sata->Callback = SataCallback;

  ZeroMem (SataController, sizeof (SataController));
  Sata->Controller = SataController;

  Sata->SataConfig = NULL;
}

/**
  Initialize SATA structures - Common for PCH Legacy and I/O Expander
  This should be done according SATA IP integration in SoC

  @param[in]      SiPolicy        Pointer to Silicon Policy
  @param[in]      SataCtrlIndex   SATA controller index in SoC
  @param[in out]  Sata            Pointer to SATA handle structure
  @param[in out]  SataPrivate     Pointer to SATA private configuration structure
  @param[in out]  SataController  Pointer to SATA controller identification structure
  @param[in out]  SataCallback    Pointer to SATA callback structure
**/
STATIC
VOID
SataHandleInitCommon (
  IN      SI_POLICY_PPI       *SiPolicy,
  IN      UINT32              SataCtrlIndex,
  IN OUT  SATA_HANDLE         *Sata,
  IN OUT  SATA_PRIVATE_CONFIG *SataPrivate,
  IN OUT  SATA_CONTROLLER     *SataController,
  IN OUT  SATA_CALLBACK       *SataCallback
  )
{
  EFI_STATUS            Status;
  PCH_PM_CONFIG         *PmConfig;

  //
  // Initialize: data with defaults, function pointers to NULL, substructures pointers
  //
  SataHandleLoadDefaults (Sata, SataPrivate, SataController, SataCallback);

  //
  // Initialize SATA Private Configuration
  //
  SataPrivate->RemappingSupported           = FALSE;
  SataPrivate->StorageRemappingEnabled      = FALSE;
  SataPrivate->SataSramParityCheckDisable   = FALSE;
  SataPrivate->SirC4Programming             = FALSE;
  SataPrivate->SataPowerGatingSupported     = FALSE;
  SataPrivate->SataSkipPortClockDisable     = TRUE;
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPmConfigGuid, (VOID *) &PmConfig);
  if (EFI_ERROR (Status)) {
    SataPrivate->SataPsOnSupported          = FALSE;
  } else {
    SataPrivate->SataPsOnSupported          = (BOOLEAN) PmConfig->PsOnEnable;
  }
  SataPrivate->SataSupportedRstMode         = RstPremium;
  SataPrivate->SataOscClkFreq               = SataSosc100Mhz;
  SataPrivate->InterruptPin                 = ItssGetDevIntPin (SiPolicy, SataDevNumber (SataCtrlIndex), SataFuncNumber (SataCtrlIndex));

  //
  // Initialize SATA callback pointers
  //
  SataCallback->SataIsPhyConnected    = EbgIsSataPortConnected;
  SataCallback->SataTestModeEnable    = EbgSataTestModeEnable;
  SataCallback->SataDevSleepEnable    = EbgSataDevSlpEnable;
  SataCallback->SataSgpioEnable       = EbgSataSgpioEnable;
  SataCallback->SataLedEnable         = EbgSataLedEnable;
  SataCallback->SataHotPlugEnable     = EbgSataHotPlugEnable;
  SataCallback->SataControllerDisable = EbgSataCtrlDisable;

  //
  // Set pointer to SATA Config Block
  //
  if (SiPolicy) {
    Sata->SataConfig = GetPchSataConfig (SiPolicy, SataCtrlIndex);
    ASSERT (Sata->SataConfig != NULL);
  }
}

/**
  Initialize SATA structures - PCH Legacy
  This should be done according SATA IP integration in SoC

  @param[in]      SiPolicy        Pointer to Silicon Policy
  @param[in]      SataCtrlIndex   SATA controller index in SoC
  @param[in out]  Sata            Pointer to SATA handle structure
  @param[in out]  SataPrivate     Pointer to SATA private configuration structure
  @param[in out]  SataController  Pointer to SATA controller identification structure
  @param[in out]  SataCallback    Pointer to SATA callback structure
**/
VOID
PchSataHandleInit (
  IN      SI_POLICY_PPI       *SiPolicy,
  IN      UINT32              SataCtrlIndex,
  IN OUT  SATA_HANDLE         *Sata,
  IN OUT  SATA_PRIVATE_CONFIG *SataPrivate,
  IN OUT  SATA_CONTROLLER     *SataController,
  IN OUT  SATA_CALLBACK       *SataCallback
  )
{
  EFI_STATUS            Status;

  SataHandleInitCommon (SiPolicy, SataCtrlIndex, Sata, SataPrivate, SataController, SataCallback);

  //
  // Initialize SATA controller data
  //
  Status = SataGetController (SataCtrlIndex, SataController);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR: Cannot get SATA Controller!\n", __FUNCTION__));
    ASSERT (FALSE);
  }

  Sata->TempBar           = PcdGet32 (PcdSiliconInitTempMemBaseAddr);
}

/**
  Get SATA controller instance for I/O Expander

  @param[in]  IoeId               I/O Expander Id (0 ... n - I/O Expander)
  @param[in]  SataCtrlIndex       SATA controller index
  @param[out] SataController      Pointer to SATA controller structure

  @retval     EFI_SUCCESS           - Completted successfully
              EFI_INVALID_PARAMETER - SataController NULL or SataCtrlIndex out of the range
**/
EFI_STATUS
IoExpanderSataGetController (
  IN  UINT32              IoeId,
  IN  UINT32              SataCtrlIndex,
  OUT SATA_CONTROLLER     *SataController
  )
{
  EFI_STATUS            Status;

  if (SataController == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (SataCtrlIndex >= MaxSataControllerNum ()) {
    return EFI_INVALID_PARAMETER;
  }
  if (IoeId >= IO_EXPANDER_MAX) {
    return EFI_INVALID_PARAMETER;
  }

  Status = PchGetSegmentByIoExpanderId (IoeId, &SataController->Segment);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = PchGetBusByIoExpanderId (IoeId, &SataController->Bus);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  SataController->SataCtrlIndex     = SataCtrlIndex;
  SataController->Device            = SataDevNumber (SataCtrlIndex);
  SataController->Function          = SataFuncNumber (SataCtrlIndex);
  SataController->PciCfgBaseAddr    = SataPciCfgBaseByIoExpanderId (IoeId, SataCtrlIndex);
  SataController->TotalCtrlPortNum  = MaxSataPortNum (SataCtrlIndex);

  return EFI_SUCCESS;
}

/**
  Initialize SATA structures - I/O Expander
  This should be done according SATA IP integration in SoC

  @param[in]      SiPolicy        Pointer to Silicon Policy
  @param[in]      SataCtrlIndex   SATA controller index in SoC
  @param[in out]  Sata            Pointer to SATA handle structure
  @param[in out]  SataPrivate     Pointer to SATA private configuration structure
  @param[in out]  SataController  Pointer to SATA controller identification structure
  @param[in out]  SataCallback    Pointer to SATA callback structure
**/
VOID
IoExpanderSataHandleInit (
  IN      SI_POLICY_PPI       *SiPolicy,
  IN      UINT32              IoeId,
  IN      UINT32              SataCtrlIndex,
  IN OUT  SATA_HANDLE         *Sata,
  IN OUT  SATA_PRIVATE_CONFIG *SataPrivate,
  IN OUT  SATA_CONTROLLER     *SataController,
  IN OUT  SATA_CALLBACK       *SataCallback
  )
{
  EFI_STATUS            Status;

  SataHandleInitCommon (SiPolicy, SataCtrlIndex, Sata, SataPrivate, SataController, SataCallback);

  //
  // Override for I/O Expander callbacks until it will be valid
  //
  SataCallback->SataIsPhyConnected    = EbgIoExpanderIsSataPortConnected;
  SataCallback->SataTestModeEnable    = NULL;
  SataCallback->SataDevSleepEnable    = NULL;
  SataCallback->SataSgpioEnable       = NULL;
  SataCallback->SataLedEnable         = NULL;
  SataCallback->SataHotPlugEnable     = NULL;
  SataCallback->SataControllerDisable = NULL;

  //
  // Initialize SATA controller data
  //
  Status = IoExpanderSataGetController (IoeId, SataCtrlIndex, SataController);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR: Cannot get SATA Controller!\n", __FUNCTION__));
    ASSERT (FALSE);
  }

  Sata->TempBar           = PchGetTempBarByIoExpanderId (IoeId);
}

VOID
SataHandlePrint (
  IN  SATA_HANDLE     *Sata
  )
{
  SATA_PRIVATE_CONFIG   *Priv;
  SATA_CONTROLLER       *SataController;

  SataController  = Sata->Controller;
  Priv            = Sata->PrivateConfig;

  DEBUG ((DEBUG_INFO, "\n---------------- SATA controller: -----------------\n"));
  DEBUG ((DEBUG_INFO, "\tAddrress = 0x%08x\n",              SataController->PciCfgBaseAddr));
  DEBUG ((DEBUG_INFO, "\tS:B:D:F  = %02d:%02d:%02d:%02d\n", SataController->Segment, SataController->Bus, SataController->Device, SataController->Function));
  DEBUG ((DEBUG_INFO, "\tIdx      = %d\n",                  SataController->SataCtrlIndex));
  DEBUG ((DEBUG_INFO, "\tPorts    = %d\n",                  SataController->TotalCtrlPortNum));
  DEBUG ((DEBUG_INFO, "RemappingSupported           = %d\n", Priv->RemappingSupported));
  DEBUG ((DEBUG_INFO, "StorageRemappingEnabled      = %d\n", Priv->StorageRemappingEnabled));
  DEBUG ((DEBUG_INFO, "SataSramParityCheckDisable   = %d\n", Priv->SataSramParityCheckDisable));
  DEBUG ((DEBUG_INFO, "SirC4Programming             = %d\n", Priv->SirC4Programming));
  DEBUG ((DEBUG_INFO, "SataPowerGatingSupported     = %d\n", Priv->SataPowerGatingSupported));
  DEBUG ((DEBUG_INFO, "SataSkipPortClockDisable     = %d\n", Priv->SataSkipPortClockDisable));
  DEBUG ((DEBUG_INFO, "SataPsOnSupported            = %d\n", Priv->SataPsOnSupported));
  DEBUG ((DEBUG_INFO, "SataSupportedRstMode         = %d\n", Priv->SataSupportedRstMode));
  DEBUG ((DEBUG_INFO, "SataOscClkFreq               = %d\n", Priv->SataOscClkFreq));
  DEBUG ((DEBUG_INFO, "InterruptPin                 = %d\n", Priv->InterruptPin));
}

/**
  Disable unused SATA Controllers

**/
VOID
PchSataDisableUnusedCtrls (
  VOID
  )
{
  UINT32                SataCtrlIndex;

  for (SataCtrlIndex = 0; SataCtrlIndex < MAX_SATA_CONTROLLER; SataCtrlIndex++) {
    if (!PmcIsSataSupported (SataCtrlIndex)) {
      DEBUG ((DEBUG_INFO, "SATA %d Fuse disabled - performing PSF disable\n", SataCtrlIndex + 1));
      PsfDisableSataDevice (SataCtrlIndex);
    }
  }
}

VOID
PchSataInit (
  IN  SI_POLICY_PPI  *SiPolicy
  )
{
  SATA_HANDLE           Sata;
  SATA_PRIVATE_CONFIG   SataPrivate;
  SATA_CONTROLLER       SataController;
  SATA_CALLBACK         SataCallback;
  UINT32                SataCtrlIndex;

  for (SataCtrlIndex = 0; SataCtrlIndex < MAX_SATA_CONTROLLER; SataCtrlIndex++) {
    PchSataHandleInit (SiPolicy, SataCtrlIndex, &Sata, &SataPrivate, &SataController, &SataCallback);
    SataHandlePrint (&Sata);
    ConfigureSata (&Sata);
  }
}

VOID
IoExpanderSataInit (
  IN  SI_POLICY_PPI  *SiPolicy,
  IN  UINT32         IoeId
  )
{
  SATA_HANDLE           Sata;
  SATA_PRIVATE_CONFIG   SataPrivate;
  SATA_CONTROLLER       SataController;
  SATA_CALLBACK         SataCallback;
  UINT32                SataCtrlIndex;

  for (SataCtrlIndex = 0; SataCtrlIndex < MAX_SATA_CONTROLLER; SataCtrlIndex++) {
    IoExpanderSataHandleInit (SiPolicy, IoeId, SataCtrlIndex, &Sata, &SataPrivate, &SataController, &SataCallback);
    SataHandlePrint (&Sata);
    DEBUG ((DEBUG_INFO, "I/O Expander Id              = %d\n", IoeId));
    ConfigureSata (&Sata);
  }
}

VOID
PchSataAfterRstInit (
  IN  SI_POLICY_PPI  *SiPolicy
  )
{
  SATA_HANDLE           Sata;
  SATA_PRIVATE_CONFIG   SataPrivate;
  SATA_CONTROLLER       SataController;
  SATA_CALLBACK         SataCallback;
  UINT32                SataCtrlIndex;

  for (SataCtrlIndex = 0; SataCtrlIndex < MAX_SATA_CONTROLLER; SataCtrlIndex++) {
    PchSataHandleInit (SiPolicy, SataCtrlIndex, &Sata, &SataPrivate, &SataController, &SataCallback);
    ConfigureSataAfterRst (&Sata);
  }
}
