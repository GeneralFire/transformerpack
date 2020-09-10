/** @file
  The PCH Init PEIM implements the PCH PEI Init PPI.

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
//
// @todo: EBG: THIS FILE IS NOT FINISHED
// This whole file should be reviewed for EBG once requierments will be formed
//
#include "PchInitPei.h"
#include "PchUsb.h"
#include <Library/MeChipsetLib.h>
#include <Library/PostCodeLib.h>
#include <Library/OcWdtLib.h>
#include <Library/PchResetLib.h>
#include <Library/SpiLib.h>
#include <Library/PchDmiLib.h>
#include <Library/PeiP2sbPrivateLib.h>
#include <Library/PeiRtcLib.h>
#include <Library/PeiWdtLib.h>
#include <Library/PeiSmbusLib.h>
#include <Library/PeiDciInitLib.h>
#include <Library/PeiItssLib.h>
#include <Library/PeiHsioLib.h>
#include <Library/HdaSocLib.h>
#include <Register/PchPcieRpRegs.h>
#include <Register/MeRegs.h>
#include <Library/PeiEnterprisePcieRpInitLib.h>
#include <Library/SpiAccessPrivateLib.h>
#include <Library/EspiAccessPrivateLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PmcSocLib.h>
#include <Library/SataSocLib.h>

#include <PchLimits.h>

#if 0
// @todo: EBG PMC
VOID PchInstallDwrPpi (VOID);
#endif
EFI_STATUS
EFIAPI
PchTraceHubPowerGateNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

static EFI_PEI_NOTIFY_DESCRIPTOR  mPeiPchTraceHubNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gMeBeforeDidSentPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) PchTraceHubPowerGateNotifyCallback
};

/**
  Notify function to disable and power gate Pch trace hub

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
  @retval     EFI_UNSUPPORTED   The device is not supported
**/
EFI_STATUS
EFIAPI
PchTraceHubPowerGateNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  return PchTraceHubDisable ();
}


/**
  Check some PCH policies are valid for debugging unexpected problem if these
  values are not initialized or assigned incorrect resource.

  @param[in] SiPreMemPolicy  The SI PreMem Policy PPI instance

**/
VOID
PchValidatePolicy (
  IN  SI_PREMEM_POLICY_PPI   *SiPreMemPolicy
  )
{
  EFI_STATUS                  Status;
  PCH_GENERAL_PREMEM_CONFIG   *PchGeneralPreMemConfig;
  PCH_TRACE_HUB_PREMEM_CONFIG *PchTraceHubPreMemConfig;
  UINT32                      TotalTraceBufferSize;
  UINT32                      TraceHubBufferSizeTab[8] = {0, SIZE_1MB, SIZE_8MB, SIZE_64MB, SIZE_128MB, SIZE_256MB, SIZE_512MB, SIZE_1GB};

  TotalTraceBufferSize      = 0;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPchTraceHubPreMemConfigGuid, (VOID *) &PchTraceHubPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  TotalTraceBufferSize += TraceHubBufferSizeTab[PchTraceHubPreMemConfig->TraceHub.MemReg0Size] +
                            TraceHubBufferSizeTab[PchTraceHubPreMemConfig->TraceHub.MemReg1Size];
  if (TotalTraceBufferSize > SIZE_512MB) {
    DEBUG ((DEBUG_ERROR, "Total Trace Hub Memory size over limited 512MB, reset trace hub memory region size to policy default\n"));
    PchTraceHubPreMemConfig->TraceHub.MemReg0Size = TraceBuffer64M;
    PchTraceHubPreMemConfig->TraceHub.MemReg1Size = TraceBuffer64M;
    DEBUG ((DEBUG_ERROR, "Enforce PCH TH mem reg 0/1 policy to 64MB/64MB\n"));
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPchGeneralPreMemConfigGuid, (VOID *) &PchGeneralPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  ASSERT (SiPreMemPolicy->TableHeader.Header.Revision == SI_PREMEM_POLICY_REVISION);
}

/**
  Build Memory Mapped IO Resource which is used to build E820 Table in LegacyBios,
  the resource range should be preserved in ACPI as well.

**/
VOID
PchPreservedMmioResource (
  VOID
  )
{
  //
  // This function builds a HOB that describes a chunk of system memory.
  //
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_MAPPED_IO,
    (EFI_RESOURCE_ATTRIBUTE_PRESENT | EFI_RESOURCE_ATTRIBUTE_INITIALIZED | EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE),
    PCH_PRESERVED_BASE_ADDRESS,
    PCH_PRESERVED_MMIO_SIZE
    );
  //
  // This function builds a HOB for the memory allocation.
  //
  BuildMemoryAllocationHob (
    PCH_PRESERVED_BASE_ADDRESS,
    PCH_PRESERVED_MMIO_SIZE,
    EfiMemoryMappedIO
    );
  return;
}

/**
  This function performs basic initialization for PCH in PEI phase after Policy produced.
  If any of the base address arguments is zero, this function will disable the corresponding
  decoding, otherwise this function will enable the decoding.
  This function locks down the AcpiBase.

  @param[in] SiPreMemPolicyPpi         The Silicon PreMem Policy PPI instance
**/
VOID
EFIAPI
PchOnPolicyInstalled (
  IN  SI_PREMEM_POLICY_PPI    *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                  Status;
  PCH_GENERAL_PREMEM_CONFIG   *PchGeneralPreMemConfig;
  PCH_TRACE_HUB_PREMEM_CONFIG *PchTraceHubPreMemConfig;
  BOOLEAN                     DisableTraceHub;

  PostCode (0xB48);
  DEBUG ((DEBUG_INFO, "PchOnPolicyInstalled() - Start\n"));

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPchGeneralPreMemConfigGuid, (VOID *) &PchGeneralPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPchTraceHubPreMemConfigGuid, (VOID *) &PchTraceHubPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Set PWRM Base in PMC device
  ///
  Status = PmcSetPwrmBase (PmcPciCfgBase (), PCH_PWRM_BASE_ADDRESS);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Set ACPI Base in PMC device
  ///
  Status = PmcSetAcpiBase (PcdGet16 (PcdAcpiBaseAddress));
  ASSERT_EFI_ERROR (Status);

  //
  // Configure HD-Audio
  //
  PostCode (0xB0E);
  HdaPreMemInit (SiPreMemPolicyPpi);

  //
  // Check to disable and lock WDT according to Policy.
  //
  WdtDisableAndLock (SiPreMemPolicyPpi);

  PostCode (0xB49);

  //
  // After this call all PCIe links from active PCH
  // root ports will be in link active if they
  // have device connected.
  //
  PcieRpInitPreMem (SiPreMemPolicyPpi);

  //
  // Configure BIOS HSIO if ChipsetInit HSIO Settings in CSME are right
  // The HSIO PPI MUST be ready before this function.
  // The HSIO programming should be done before any IP configuration.
  //

  //
  // HSIO Miscellaneous programming
  //
  PchHsioBiosProgPreMem (SiPreMemPolicyPpi);

  PostCode (0xB4A);
  DciConfiguration (SiPreMemPolicyPpi);

  PostCode (0xB4D);
  DisableTraceHub = TraceHubInitialize ((UINT8) PchTraceHubPreMemConfig->TraceHub.EnableMode);
  if (DisableTraceHub) {
    //
    // Register notify to power gate pch trace hub
    // It needs to be power gated between ME GetImrSize and SendDID
    //
    if (EFI_ERROR (PeiServicesNotifyPpi (&mPeiPchTraceHubNotifyList))) {
      ASSERT (FALSE);
    }
  }

  PostCode (0xB15);
  Status = SmbusInit (SiPreMemPolicyPpi);

  if (PchGeneralPreMemConfig->Port80Route == PchReservedPageToLpc) {
    PchDmiSetReservedPageRegToLpc ();
  } else {
    PchDmiSetReservedPageRegToPcieRootPort ();
  }

  PostCode (0xB52);
  P2sbHpetInit ();

  LpcOnPolicyConfigure (SiPreMemPolicyPpi);

  PostCode (0xB55);

  DEBUG ((DEBUG_INFO, "PchOnPolicyInstalled() - End\n"));
}

/**
  This code is intended to handle all cases where IP could be fuse disabled, static power gated
  or disabled due to soft strap lane assignment

  @retval EFI_SUCCESS
**/
VOID
PchEarlyDisabledDeviceHandling (
  VOID
  )
{
  UINT32                    UartNumber;

  DEBUG ((DEBUG_INFO, "PchEarlyDisabledDeviceHandling() - Start\n"));

  ///
  /// BWG 5.3.1 Fuse Disabled, Static Disabled, non-Static function disable Device Handling
  ///

  //
  // PCIe Root Ports fused
  //
  PchPcieRpDisableUnusedPorts ();

  //
  // SATA fused
  //
  PchSataDisableUnusedCtrls ();

  //
  // LPSS fused
  //
  if (!PmcIsSerialIoSupported ()) {
    for (UartNumber = 0; UartNumber < GetPchMaxSerialIoUartControllersNum (); UartNumber++) {
      DEBUG ((DEBUG_INFO, "SerialIo UART%d Static PG disabled - performing PSF disable\n", UartNumber));
      PsfDisableDevice (PsfSerialIoUartPort (UartNumber));
    }
  }

  //
  // Gbe fused
  //
  if (PchIsGbeSupported () && !PmcIsGbeSupported ()) {
    DEBUG ((DEBUG_INFO, "GBE Supported and Fuse disabled - performing PSF disable\n"));
    PsfDisableGbeDevice ();
  }
}

/**
  Show PCH related information
**/
VOID
PchShowInformation (
  VOID
  )
{
  DEBUG_CODE_BEGIN ();
  CHAR8           Buffer[PCH_STEPPING_STR_LENGTH_MAX];

  DEBUG ((DEBUG_INFO, "PCH Series   : %a\n", PchGetSeriesStr ()));
  PchGetSteppingStr (Buffer, sizeof (Buffer));
  DEBUG ((DEBUG_INFO, "PCH Stepping : %a\n", Buffer));
  DEBUG ((DEBUG_INFO, "PCH SKU      : %a\n", PchGetSkuStr ()));
  DEBUG_CODE_END ();
}

/**
  PCH init pre-memory entry point
**/
VOID
EFIAPI
PchInitPrePolicy (
  VOID
  )
{
  //
  // Dump PCH information
  //
  DEBUG_CODE_BEGIN ();
  PchShowInformation ();
  PchFiaPrintLosRegisters ();
  DEBUG_CODE_END ();

  PostCode (0xB00);
  DEBUG ((DEBUG_INFO, "PchInitPrePolicy() - Start\n"));

  //
  // Check if Pch is supported
  //
  if (!IsPchSupported ()) {
    DEBUG ((DEBUG_ERROR, "PCH SKU is not supported due to no proper PCH LPC found!\n"));
    ASSERT (FALSE);
  }

  //
  // Check if SBREG has been set.
  //
  if (!P2sbIsSbregReady ()) {
    DEBUG ((DEBUG_INFO, "SBREG should be programmed before here\n"));
    ASSERT (FALSE);
  }

  //
  // Check if PCH PWRM Base has been set
  //
  DEBUG ((DEBUG_INFO, "PCH PWRM Base needs to be programmed before here\n"));
  ASSERT (PmcGetPwrmBase () != 0);

  //
  // Build Memory Mapped IO Resource which is used to build E820 Table in LegacyBios.
  // the resource range should be preserved in ACPI as well.
  //
  PchPreservedMmioResource ();


#ifndef FSP_FLAG
  ///
  /// If there was unexpected reset but no WDT expiration and no resume from S3/S4,
  /// clear unexpected reset status and enforce expiration. This is to inform Firmware
  /// which has no access to unexpected reset status bit, that something went wrong.
  ///
  OcWdtResetCheck ();
#endif
  //
  // Initialize WDT and install WDT PPI
  //
  WdtInstallResetCallback ();
  OcWdtInit ();

  //
  // Installs PCH SPI PPI
  //
  SpiServiceInit ();

  PostCode (0xB02);

  //
  // Perform PSF early initialization.
  //
  PsfEarlyInit ();
#if 0
  // @todo: EBG PMC
  //
  // Install DWR handlers
  //
  PchInstallDwrPpi();
#endif
  //
  // The 8259 PIC is still functional and not masked by default,
  // even if APIC is enabled.
  // So need to set all 8259 interrupts to disabled.
  //
  ItssMask8259Interrupts ();

  //
  // Before any interrupt is enabled, set master message enable.
  //
  ItssSetMasterMessageEnable ();

  //
  // PCH-H DMI early init (before any upstream bus master transactions are enabled)
  //
  if (IsPchH ()) {
    PchDmi15EarlyInit ();
  }

  //
  // According to bios spec,
  // Setup "uCode Patch Region Enable" in DMI
  //
  PchDmiEnableUCodePatchRegion ();

  //
  // Do P2SB early configuration.
  //
  P2sbEarlyConfig ();

  //
  // Configura RTC when power failure case.
  //
  if (!PmcIsRtcBatteryGood ()) {
    RtcPowerFailureConfiguration ();
  }

  //
  // Clear CF9GR if it's set in previous boot.
  //
  PmcDisableCf9GlobalReset ();

  //
  // Enhance port 8xh LPC decoding
  //
  LpcOnEarlyPeiConfigure ();

  //
  // Handle all static disabled controllers.
  //
  PchEarlyDisabledDeviceHandling ();

  PostCode (0xB7F);
  DEBUG ((DEBUG_INFO, "PchInitPrePolicy() - End\n"));
}
