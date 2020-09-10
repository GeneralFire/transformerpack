/** @file
  The PCH Initialization Dispatcher After Memory.

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

#include "PchInitPei.h"

#include <Library/PmcSocLib.h>
#include <Library/ConfigBlockLib.h>
#include <ConfigBlock.h>
#include <PchDmiConfig.h>
#include <Library/PeiItssLib.h>
#include <Library/PchAdrLib.h>
#include <Register/PmcRegs.h>
#include <Register/PmcRegsFivr.h>
#include <Register/PmcRegsEbg.h>
#include <Library/DciPrivateLib.h>
#include <Library/PchFiaLib.h>
#include <Library/PeiPmcPrivateLib2.h>
#include <Library/PmcLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PchIoExpanderBdfLib.h>
#include <Library/PchIoExpander.h>

#include <Library/PeiServicesLib.h>

/**
  Restore Bus Master Enable (BME) bit in PMC device (clear the bit) after PCIe enumaration.

  Read from PMC Command register always returns 0 on I/O enable bit (bit 0).
  Use read/modify/write procedure to PMC Command register causes erasing I/O space enable bit that disables ACPI I/O space.
  PCIe enumerators can use such method and can override I/O enable bit.
  To prevent I/O space disabling by enumerators Bus Master Enable (BME) bit should be set.
  In this case enumerator skips the device during enumeration.

  @param[in] PmcPciCfgBase              PMC PCI config base address

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               Cannot access PMC device.
**/
EFI_STATUS
PmcEnableIOSpaceAndBusMaster (
  UINT64           PmcPciCfgBase
  );

/**
  Checks if ADR is enabled

  @retval BOOLEAN   TRUE if ADR is FORCE_ENABLE, FALSE otherwise.
**/
STATIC
BOOLEAN
IsAdrEnabled (
  VOID
  )
{
  SI_POLICY_PPI         *SiPolicy;
  ADR_CONFIG            *AdrConfig;
  EFI_STATUS            Status;

  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **)&SiPolicy
             );
  if (EFI_ERROR (Status)) {
    //
    // SI_POLICY_PPI must be installed at this point
    //
    ASSERT (FALSE);
    return FALSE;
  }

  Status = GetConfigBlock ((VOID *) SiPolicy, &gAdrConfigGuid, (VOID *) &AdrConfig);
  ASSERT_EFI_ERROR (Status);

  return (AdrConfig->AdrEn == FORCE_ENABLE);
}

/**
  Perform SoC specific SLP_S0 configuration.

  @param[in] PmcHandle  Pointer to PMC handle
**/
STATIC
VOID
EbgPchPmcConfigureSlpS0 (
  IN PMC_HANDLE  *PmcHandle
  )
{
  ///
  /// PWRMBASE + 0x1B1C = 0x801E
  ///
  MmioOr32 (PmcHandle->PwrmBase + R_PMC_PWRM_1B1C, 0x801E);
  ///
  /// PWRMBASE + 0x1B20 = 0x00138801
  ///
  MmioWrite32 (PmcHandle->PwrmBase + R_PMC_PWRM_CPPM_MISC_CFG, 0x00138801);
}

/**
  Configures which PM_SYNC messages are allowed to be
  sent over PM_SYNC interface.

  @param[in] PmcHandle  Pointer to PMC handle
**/
STATIC
VOID
EbgPchPmSyncInitMessages (
  IN PMC_HANDLE  *PmcHandle
  )
{
  UINT32   Data32;

  MmioWrite32 (PmcHandle->PwrmBase + R_PMC_PWRM_PM_SYNC_STATE_HYS, 0x00200840);

  //
  // Configure which PM_SYNC messages will be sent.
  MmioWrite32 (PmcHandle->PwrmBase + R_PMC_PWRM_PM_SYNC_MODE, 0x7000F4E6);

  Data32 = 0x7000F4C0;

  if (IsAdrEnabled ()) {
    Data32 &= (UINT32) ~(BIT13);
    DEBUG ((DEBUG_INFO, "PmcConfigurePmSyncC0 () ADR disabled\n"));
  }

  MmioWrite32 (PmcHandle->PwrmBase + R_PMC_PWRM_PM_SYNC_MODE_C0, Data32);

  //
  // Enable PM_SYNC events according to recommendation.
  // Enabled events:
  // SB IP driven wake
  // DMI put snoop to SA
  // DMI put non-snoop to SA
  //
  MmioWrite32 (PmcHandle->PwrmBase + R_PMC_PWRM_EN_MISC_EVENT, 0x00000A20);

  //
  // Configure which snoopable backbone requests will set the SLOW_RING
  // indication sent up over the PM_SYNC.
  // Enabled for:
  // All PCIe ports
  // All SATA ports
  // GbE
  // LPSS
  // CNVi
  // HDA
  // PCIe NAND
  // Legacy cluster
  // P2SB
  // ME
  // Thermal sensor
  // ISH
  // USB OTG
  // USB3
  //
  MmioWrite32 (PmcHandle->PwrmBase + R_PMC_PWRM_EN_SN_SLOW_RING, 0x0E2D9F01);
  MmioWrite32 (PmcHandle->PwrmBase + R_PMC_PWRM_EN_SN_SLOW_RING2, 1);

  //
  // Configure which snoopable backbone requests should set the SA indication
  // sent up over PM_SYNC. This configuration matches SLOW_RING configuration.
  //
  MmioWrite32 (PmcHandle->PwrmBase + R_PMC_PWRM_EN_SN_SA,  0x0E2D9F01);
  MmioWrite32 (PmcHandle->PwrmBase + R_PMC_PWRM_EN_SN_SA2, 1);

  //
  // Configure which snoopable backbone requests should set SLOW_RING_CF
  // indication sent up over PM_SYNC.
  // Enabled for:
  // Legacy cluster
  // P2SB
  //
  MmioWrite32 (PmcHandle->PwrmBase + R_PMC_PWRM_EN_SN_SLOW_RING_CF, 0x00018000);

  //
  // Configure which non-snoopable backbone requests should set SA
  // indication sent up over PM_SYNC
  // Enabled for:
  // LPSS
  // HDA
  // SCS
  // ME
  // ISH
  //
  MmioWrite32 (PmcHandle->PwrmBase + R_PMC_PWRM_EN_NS_SA, 0x04043400);

  //
  // Configure which pegged active signals should set the SLOW_RING
  // indication sent up over PM_SYNC
  // Enabled for:
  // All SATA ports
  // ISH
  // HDA
  // PCIe NAND
  // BM_BUSY#
  // USB3
  // XHCI LTRINF
  // Audio DSP
  // SCS
  // GbE TSN - only if not strap disabled.
  //
  MmioWrite32 (PmcHandle->PwrmBase + R_PMC_PWRM_EN_PA_SLOW_RING, 0x00181300);

  Data32 = 0x02006001;
  MmioWrite32 (PmcHandle->PwrmBase + R_PMC_PWRM_EN_PA_SLOW_RING2, Data32);

  //
  // Configure which pegged active signals should set the SA indication
  // sent up over PM_SYNC. This configuration matches SLOW_RING indication
  // configuration.
  //
  MmioWrite32 (PmcHandle->PwrmBase + R_PMC_PWRM_EN_PA_SA, 0x00181300);
  Data32 = 0x02006001;
  MmioWrite32 (PmcHandle->PwrmBase + R_PMC_PWRM_EN_PA_SA2, Data32);

  //
  // Configure which backbone clock wake signals should set the SLOW_RING indication
  // sent up over the PM_SYNC
  // Enabled for:
  // All PCIe ports
  // All SATA ports
  // GbE
  // CNVi
  // HDA
  // USB OTG
  // VLW
  // Legacy cluster
  // P2SB
  // ISH
  // ME
  // PCIe NAND
  // SMBUS
  // CHAP
  // Thermal sensor
  // SPI
  // PMC
  // Display
  // Southport A, B, C and D
  // Primary channel
  // MEIO
  // GPIO
  // Audio DSP
  // LPSS
  // SCS
  // THC 0 and 1
  //
  MmioWrite32 (PmcHandle->PwrmBase + R_PMC_PWRM_EN_CW_SLOW_RING, 0x2F8FBB01);
  MmioWrite32 (PmcHandle->PwrmBase + R_PMC_PWRM_EN_CW_SLOW_RING2, 0x1A80C7E0);

  //
  // Configure which backbone clock wake signals should set the SA indication
  // sent up over the PM_SYNC. This configuration matches the SLOW_RING indication
  // configuration.
  //
  MmioWrite32 (PmcHandle->PwrmBase + R_PMC_PWRM_EN_CW_SA, 0x2F8FBB01);
  MmioWrite32 (PmcHandle->PwrmBase + R_PMC_PWRM_EN_CW_SA2, 0x1A80C7E0);

  //
  // Configure which clock wake signals should set the SLOW_RING_CF indication
  // sent up over the PM_SYNC
  // Enabled for:
  // Legacy cluster
  // P2SB
  //
  MmioWrite32 (PmcHandle->PwrmBase + R_PMC_PWRM_EN_CW_SLOW_RING_CF, 0x00018000);
}

/**
  Configure CPPM forced alignment after CPPM configuration
  has been done in PMC.

  @param[in] PmcHandle  Pointer to PMC handle
**/
STATIC
VOID
EbgPchPmcConfigureCppmForceAlignment (
  IN PMC_HANDLE  *PmcHandle
  )
{
  UINT32 Data32Or;
  UINT32 Data32And;

  //
  // Recommendation on programming is delivered by IP
  // team.
  //
  Data32Or = 0xD80A8000;
  Data32And = ~0u;

  if (PchStepping () == PCH_Z0) {
    Data32And &= ~(UINT32)BIT28;
  }

  MmioAndThenOr32 (
    PmcHandle->PwrmBase + R_PMC_PWRM_1BD0,
    Data32And,
    Data32Or
    );

  //
  // Lock PCH Forced alignment config
  //
  MmioOr32 (PmcHandle->PwrmBase + R_PMC_PWRM_1BD0, BIT30);
}

/**
  Called after CPU C10 gate is configured on PMC side to
  finish up feature configuration on SoC side.

  @param[in] PmcHandle  Pointer to PMC handle
**/
STATIC
VOID
EbgPchPmcConfigurePowerButtonDebounce (
  IN PMC_HANDLE  *PmcHandle
  )
{
  GpioSetPwrBtnDebounceTimer (PmcHandle->PmConfig->PowerButtonDebounce);
}

/**
  Called after CPU C10 gate is configured on PMC side to
  finish up feature configuration on SoC side.

  @param[in] PmcHandle  Pointer to PMC handle
**/
STATIC
VOID
EbgPchPmcConfigureCpuC10Gate (
  IN PMC_HANDLE  *PmcHandle
  )
{
  GpioEnableCpuC10GatePin ();
}

/**
  Called after PMC is done configuring VR alert to finish up
  init on SoC side.

  @param[in] PmcHandle  Pointer to PMC handle
**/
STATIC
VOID
EbgPchPmcConfigureVrAlert (
  IN PMC_HANDLE  *PmcHandle
  )
{
  GpioEnableVrAlert ();
}

/**
  Get the PMC SOC configuration.

  @param[out] PmcSocConfig  Configuration of the PMC
**/
VOID
EbgPchPmcGetSocConfig (
  IN SI_POLICY_PPI    *SiPolicy,
  OUT PMC_SOC_CONFIG  *PmcSocConfig
  )
{
  UINT32          PortNum;
  UINT32          MaxUsb3PortNum;

  if (PmcSocConfig == NULL) {
    return;
  }

  ZeroMem (PmcSocConfig, sizeof (PMC_SOC_CONFIG));

  PmcSocConfig->CppmCgInterfaceVersion = 1;
  PmcSocConfig->LpmSupported = FALSE;
  PmcSocConfig->TimedGpioSupported = TRUE;
  PmcSocConfig->CpuIovrRampTime = 0;
  PmcSocConfig->ModPhySusPgSupported = FALSE;
  PmcSocConfig->SciIrq = ItssGetPmcSciDevIntConfig (SiPolicy);
  PmcSocConfig->EspiBoot = IsEspiEnabled ();
  PmcSocConfig->UsbDbcConnected = IsPchUsbDbcConnected ();
  PmcSocConfig->DisableIosfSbClockGating = TRUE;
  PmcSocConfig->SkipModPhyGatingPolicy = TRUE;

  MaxUsb3PortNum = GetPchXhciMaxUsb3PortNum ();
  for (PortNum = 0; PortNum < MaxUsb3PortNum; PortNum++) {
    if (PchFiaIsUsb3PortConnected (PortNum)) {
      PmcSocConfig->Usb3LanesConnectedBitmask |= (1 << PortNum);
    }
  }

}

/**
  Will send IPC command to PMC to write Private Register through proxy access

  @param[in]  PrivateRegisterIndex  Private Register Index
  @param[in]  WriteValue            Write value

  @retval     EFI_SUCCESS           Value changed successfully
**/
STATIC
EFI_STATUS
EbgPchPmcWritePrivateRegister32 (
  IN  UINT8        PrivateRegisterIndex,
  IN  UINT32       WriteValue
  )
{
  EFI_STATUS                Status;
  PMC_IPC_COMMAND_BUFFER    Wbuf;

  if (PrivateRegisterIndex >= V_PMC_PRIVATE_REGISTER_ID_MAX) {
    DEBUG ((
      DEBUG_ERROR,
      "Unsupported PMC Private Register Index provided: %d, max is %d\n",
      PrivateRegisterIndex, V_PMC_PRIVATE_REGISTER_ID_MAX
      ));
    return EFI_UNSUPPORTED;
  }

  Status = EFI_SUCCESS;
  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));

  Wbuf.Buf0 = PrivateRegisterIndex;
  Wbuf.Buf1 = WriteValue;
  Status = PmcSendCommand (
    V_PMC_PWRM_IPC_CMD_COMMAND_PMC_PRV_REG_ACCESS,
    0x1,  // Read
    0x2,  // Write buffer size
    &Wbuf,
    NULL
    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error while writing PMC Private Register id %d - %r\n", PrivateRegisterIndex, Status));
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  This function shall program Global Reset Event/Trigger masks according to values
  provided in PMC Config Block. Programming shall be done through proxy access to
  those Private Configuration Registers

  @param[in] PmConfig  PM configuration
**/
STATIC
VOID
EbgPchPmcProgramGlobalResetMasks (
  IN PCH_PM_CONFIG  *PmConfig
  )
{
  EFI_STATUS                Status;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  if (!PmConfig->GlobalResetMasksOverride) {
    DEBUG ((DEBUG_INFO, "Global Resets Masks override is disabled.\n"));
    return;
  }

  Status = EbgPchPmcWritePrivateRegister32 (
             V_PMC_PRIVATE_REGISTER_ID_GBLRST_TRIG_MASK0,
             PmConfig->GlobalResetTriggerMask.Value
             );
  if (EFI_ERROR (Status)) {
    return;
  }

  Status = EbgPchPmcWritePrivateRegister32 (
             V_PMC_PRIVATE_REGISTER_ID_GBLRST_EVENT_MASK0,
             PmConfig->GlobalResetEventMask.Value
             );
  if (EFI_ERROR (Status)) {
    return;
  }
}

/**
  Build a PMC handle for EBG

  @param[in]  SiPolicy      SiPolicyPpi instance
  @param[in]  PmcSocConfig  Pointer to PMC SoC configuration
  @param[in]  PmcCallbacks  Pointer to PMC SoC callbacks
  @param[in]  PmConfig      Pointer to PMC platform configuration
  @param[out] PmcHandle     On output pointer to initialized PMC handle
**/
STATIC
VOID
EbgPchPmcGetHandleCommon (
  IN  SI_POLICY_PPI   *SiPolicy,
  IN  PMC_SOC_CONFIG  *PmcSocConfig,
  IN  PMC_CALLBACK    *PmcCallback,
  IN  PCH_PM_CONFIG   **PmConfig,
  OUT PMC_HANDLE      *PmcHandle
  )
{
  EFI_STATUS  Status;

  EbgPchPmcGetSocConfig (SiPolicy, PmcSocConfig);

  Status = GetConfigBlock ((VOID *) SiPolicy, &gPmConfigGuid, (VOID *) PmConfig);
  ASSERT_EFI_ERROR (Status);

  PmcCallback->PmSyncInitMessages = EbgPchPmSyncInitMessages;
  PmcCallback->PmcConfigureSlpS0 = EbgPchPmcConfigureSlpS0;
  PmcCallback->PmcConfigureModPhySusPg = NULL; // Not supported
  PmcCallback->PmcConfigureCppmForceAlignment = EbgPchPmcConfigureCppmForceAlignment;
  PmcCallback->PmcConfigurePowerButtonDebounce = EbgPchPmcConfigurePowerButtonDebounce;
  PmcCallback->PmcConfigureCpuC10Gate = EbgPchPmcConfigureCpuC10Gate;
  PmcCallback->PmcConfigureVrAlert = EbgPchPmcConfigureVrAlert;

  PmcHandle->PmcSocConfig = PmcSocConfig;
  PmcHandle->PmConfig = *PmConfig;
  PmcHandle->PmcCallback = PmcCallback;
}

/**
  Build a PMC handle for EBG

  @param[in]  SiPolicy      SiPolicyPpi instance
  @param[in]  PmcSocConfig  Pointer to PMC SoC configuration
  @param[in]  PmcCallbacks  Pointer to PMC SoC callbacks
  @param[in]  PmConfig      Pointer to PMC platform configuration
  @param[out] PmcHandle     On output pointer to initialized PMC handle
**/
VOID
EbgPchPmcGetHandle (
  IN  SI_POLICY_PPI   *SiPolicy,
  IN  PMC_SOC_CONFIG  *PmcSocConfig,
  IN  PMC_CALLBACK    *PmcCallback,
  IN  PCH_PM_CONFIG   **PmConfig,
  OUT PMC_HANDLE      *PmcHandle
  )
{
  EbgPchPmcGetHandleCommon (SiPolicy, PmcSocConfig, PmcCallback, PmConfig, PmcHandle);

  PmcHandle->PwrmBase = PmcGetPwrmBase ();
}

/**
  Build a PMC handle for EBG

  @param[in]  SiPolicy      SiPolicyPpi instance
  @param[in]  IoeId         I/O Expander Id (0 ... n - I/O Expander)
  @param[in]  PmcSocConfig  Pointer to PMC SoC configuration
  @param[in]  PmcCallbacks  Pointer to PMC SoC callbacks
  @param[in]  PmConfig      Pointer to PMC platform configuration for PCH
  @param[in]  IoePmConfig   Pointer to PMC platform configuration for I/O Expander
  @param[out] PmcHandle     On output pointer to initialized PMC handle
**/
VOID
EbgPchIoExpanderPmcGetHandle (
  IN  SI_POLICY_PPI   *SiPolicy,
  IN  UINT32          IoeId,
  IN  PMC_SOC_CONFIG  *PmcSocConfig,
  IN  PMC_CALLBACK    *PmcCallback,
  IN  PCH_PM_CONFIG   **PmConfig,
  IN  PCH_PM_CONFIG   *IoePmConfig,
  OUT PMC_HANDLE      *PmcHandle
  )
{
  EbgPchPmcGetHandleCommon (SiPolicy, PmcSocConfig, PmcCallback, PmConfig, PmcHandle);

  // W/A until separated policy for I/O Expander
  // Create copy of PCH_PM_CONFIG for I/O Expander init flow
  CopyMem (IoePmConfig, *PmConfig, sizeof (PCH_PM_CONFIG));
  PmcHandle->PmConfig = IoePmConfig;

  PmcCallback->PmcConfigurePowerButtonDebounce = NULL; // Not supported on I/O Expander
  PmcCallback->PmcConfigureCpuC10Gate = NULL; // Enabled once GPIO available
  PmcCallback->PmcConfigureVrAlert = NULL; // Enabled once GPIO available

  PmcHandle->PwrmBase = (UINT32) PchGetPmcBarByIoExpanderId (IoeId);

  PmcHandle->PmcSocConfig->TimedGpioSupported = FALSE; // Revise on I/O Expander
  PmcHandle->PmcSocConfig->OsIdleSupported = FALSE; // Not supported on I/O Expander

  // W/A until separated policy for I/O Expander
  // Override PCH_PM_CONFIG for I/O Expander init flow
  PmcHandle->PmConfig->EnableTcoTimer = FALSE; // Not supported on I/O Expander
  PmcHandle->PmConfig->VrAlert = FALSE; // Enable once GPIO will be available
  PmcHandle->PmConfig->PowerButtonDebounce = 0; // Not supported on I/O Expander
  PmcHandle->PmConfig->PwrBtnOverridePeriod = 0; // Not supported on I/O Expander
  PmcHandle->PmConfig->DisableNativePowerButton = TRUE; // Not supported on I/O Expander
  PmcHandle->PmConfig->CpuC10GatePinEnable = FALSE; // Not supported on I/O Expander
}

/**
  Program MMIO for EBG PCH PMC controller.

  @param[in] SiPolicy  The Silicon Policy PPI instance
  @param[in] IoeId     I/O Expander Id (0 ... n - I/O Expander)
**/
VOID
EbgPchPmcProgramMmio (
  IN SI_POLICY_PPI  *SiPolicy,
  IN UINT32         IoeId
  )
{
  EFI_STATUS  Status;

  ///
  /// Set PWRM Base in PMC device
  ///
  Status = PmcSetPwrmBase (PmcPciCfgBaseByIoExpanderId (IoeId),  PchGetPmcBarByIoExpanderId (IoeId));
  ASSERT_EFI_ERROR (Status);
}

/**
  Initialize EBG PCH PMC controller.

  @param[in] SiPolicy  The Silicon Policy PPI instance
**/
VOID
EbgPchPmcInit (
  IN SI_POLICY_PPI  *SiPolicy
  )
{
  PMC_ADR_SOC_CONFIG   PmcAdrSocConfig;
  PMC_SOC_CONFIG       PmcSocConfig;
  PMC_HANDLE           PmcHandle;
  PMC_CALLBACK         PmcCallback;
  PCH_PM_CONFIG        *PmConfig;

  EbgPchPmcGetHandle (SiPolicy, &PmcSocConfig, &PmcCallback, &PmConfig, &PmcHandle);
  PmcInit (&PmcHandle);

  if (PmcSocConfig.TimedGpioSupported) {
    if (PmConfig->EnableTimedGpio0) {
      GpioEnableTimeSync (0);
    }
    if (PmConfig->EnableTimedGpio1) {
      GpioEnableTimeSync (1);
    }
  }

  //
  // W/A for IIO enumerator resetting PMC BAR address.
  // After we set PWRM and ACPI base in early PEI but before we
  // hide PMC config space access IIO PCI enumerator will
  // disable PMC bus master and IO space enable bit which
  // will disable ACPI IO BAR access causing boot issues.
  // As a temporary w/a we reenable the access here.
  //
  PmcEnableIOSpaceAndBusMaster (PmcPciCfgBase ());

  //
  // Will program Global Reset Trigger and Events masks
  // based on value provided in PMC config block
  //
  EbgPchPmcProgramGlobalResetMasks (PmConfig);

  PmcAdrSocConfig.AdrPhaseType = AdrDualPhase;
  PmcAdrSocConfig.AdrGenCfgMoved = TRUE;
  PchAdrConfigure (&PmcAdrSocConfig);

  PsfHidePmcDevice ();
}

/**
  Initialize EBG PCH PMC controller.

  @param[in] SiPolicy  The Silicon Policy PPI instance
  @param[in] IoeId     I/O Expander Id (0 ... n - I/O Expander)
**/
VOID
EbgPchIoExpanderPmcInit (
  IN  SI_POLICY_PPI  *SiPolicy,
  IN  UINT32         IoeId
  )
{
  PMC_SOC_CONFIG       PmcSocConfig;
  PMC_HANDLE           PmcHandle;
  PMC_CALLBACK         PmcCallback;
  PCH_PM_CONFIG        *PmConfig;
  PCH_PM_CONFIG        IoePmConfig;

  EbgPchIoExpanderPmcGetHandle (SiPolicy, IoeId, &PmcSocConfig, &PmcCallback, &PmConfig, &IoePmConfig, &PmcHandle);
  PmcInit (&PmcHandle);

  //PsfHidePmcDevice (); // Enable later once Psf access will be available
}

/**
  Init PMC at end of PEI.

  @param[in] SiPolicy  Pointer to SiPolicy
**/
VOID
EbgPchPmcInitEndOfPei (
  IN SI_POLICY_PPI  *SiPolicy
  )
{
  PMC_SOC_CONFIG       PmcSocConfig;
  PMC_HANDLE           PmcHandle;
  PMC_CALLBACK         PmcCallback;
  PCH_PM_CONFIG        *PmConfig;

  EbgPchPmcGetHandle (SiPolicy, &PmcSocConfig, &PmcCallback, &PmConfig, &PmcHandle);

  //
  // Configure Pm timer
  // This must be done after uCode PM timer emulation.
  //
  PmcConfigurePmTimer (&PmcHandle);
  PmcLockSettings (&PmcHandle);

  //
  // Lock V1p05 PHY FET Ramp Time settings
  //
  MmioOr32 (PmcHandle.PwrmBase + R_PMC_PWRM_EXT_FET_RAMP_CONFIG, B_PMC_PWRM_EXT_FET_RAMP_CONFIG_V105_PHY_FRT_LOCK | B_PMC_PWRM_EXT_FET_RAMP_CONFIG_V105_PHY_IS_FRT_LOCK);
}

/**
  Init PMC at end of PEI.

  @param[in] SiPolicy  Pointer to SiPolicy
**/
VOID
EbgPchIoExpanderPmcInitEndOfPei (
  IN  SI_POLICY_PPI  *SiPolicy
  )
{
  PMC_SOC_CONFIG       PmcSocConfig;
  PMC_HANDLE           PmcHandle;
  PMC_CALLBACK         PmcCallback;
  PCH_PM_CONFIG        *PmConfig;
  PCH_PM_CONFIG        IoePmConfig;
  UINT8                IoExpanderIndex;

  for (IoExpanderIndex = 0; IoExpanderIndex < PchGetNumberDetectedIoExpanders (); IoExpanderIndex++) {
    EbgPchIoExpanderPmcGetHandle (SiPolicy, IoExpanderIndex, &PmcSocConfig, &PmcCallback, &PmConfig, &IoePmConfig, &PmcHandle);

    PmcLockSettings (&PmcHandle);

    //
    // Lock V1p05 PHY FET Ramp Time settings
    //
    MmioOr32 (PmcHandle.PwrmBase + R_PMC_PWRM_EXT_FET_RAMP_CONFIG, B_PMC_PWRM_EXT_FET_RAMP_CONFIG_V105_PHY_FRT_LOCK | B_PMC_PWRM_EXT_FET_RAMP_CONFIG_V105_PHY_IS_FRT_LOCK);
  }
}

