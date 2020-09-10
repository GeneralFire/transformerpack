/** @file
  Pei/Dxe/Smm Pch TraceHub Lib.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation.

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

#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PostCodeLib.h>
#include <Private/Library/PchSbiAccessLib.h>
#include <Private/Library/PchPsfPrivateLib.h>
#include <Private/Library/PmcPrivateLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <ConfigBlock/TraceHubConfig.h>
#include <IndustryStandard/Pci30.h>
#include <PchReservedResources.h>
#include <Register/RegsUsb.h>
#include <Register/PchRegs.h>
#include <Register/PchRegsPcr.h>
#include <Register/PchRegsTraceHub.h>
#include <Library/PeiServicesLib.h>
#include <PchResetPlatformSpecific.h>
#include <Ppi/SiPolicy.h>

/**
  Set TraceHub base address.
  This cycle decoding is allowed to set when DMIC.SRL is 0.
  Programming steps:
  1. Program TraceHub PCI Offset 0x70~0x77 to the 64-bit base address.
  2. Program PCR[PSF2] + TraceHub RS0 offset 0x000 and 0x004 to TraceHub 64-bit base address.
  3. Manually write 1 to MSEN, PCR[PSF2] + TraceHub RS0 offset 0x01C[1] to activate the shadow.

  @param[in] AddressHi                   High 32-bits for TraceHub base address.
  @param[in] AddressLo                   Low 32-bits for TraceHub base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
TraceHubBaseSet (
  UINT32                                AddressHi,
  UINT32                                AddressLo
  )
{
  UINT64                                TraceHubBase;

  //
  // check address valid
  //
  if ((AddressLo & 0x0003FFFF) != 0) {
    DEBUG ((DEBUG_ERROR, "TraceHubBaseSet Error. Invalid Address: %x%x", AddressHi, AddressLo));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  TraceHubBase = PCI_SEGMENT_LIB_ADDRESS (
                   DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                   DEFAULT_PCI_BUS_NUMBER_PCH,
                   PCI_DEVICE_NUMBER_PCH_TRACE_HUB,
                   PCI_FUNCTION_NUMBER_PCH_TRACE_HUB,
                   0
                   );
  if (PciSegmentRead16 (TraceHubBase) == 0xFFFF) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }
  //
  // Program TraceHub PCI Offset 0x70~0x77 to base address.
  //
  PciSegmentWrite32 (TraceHubBase + R_TRACE_HUB_CFG_FW_LBAR, AddressLo);
  PciSegmentWrite32 (TraceHubBase + R_TRACE_HUB_CFG_FW_UBAR, AddressHi);
  //
  // Program TraceHub ACPI BASE in PSF to TraceHub base address.
  //
  PsfSetTraceHubAcpiDeviceBarValue (0, AddressLo);
  PsfSetTraceHubAcpiDeviceBarValue (1, AddressHi);
  //
  // Enable TraceHub ACPI MSE to activate the shadow.
  //
  PsfEnableTraceHubAcpiDeviceMemorySpace ();
  //
  // Hide Trace Hub ACPI Device
  //
  PsfHideTraceHubAcpiDevice ();

  return EFI_SUCCESS;
}

/**
  Check if Trace Hub device is applicable to IPC1 PG command

  @param [in] DeviceId         Trace Hub Device ID

  @retval TRUE                 Supports PG TH
  @retval FALSE                Not supprots PG TH
**/
BOOLEAN
IsTraceHubPowerGateSupported (
  UINT16                       DeviceId
  )
{
  if ((DeviceId == V_CNL_PCH_LP_TRACE_HUB_DEVICE_ID) || (DeviceId == V_CNL_PCH_H_TRACE_HUB_DEVICE_ID)) {
    return FALSE;
  }
  return TRUE;
}

/**
  Used to program xHCI DCI MMIO registers via SBI.

  @param[in] Offset          DbC offset
  @param[in] Data            Data

**/
STATIC
VOID
SbiDbcSet (
  UINT16                     Offset,
  UINT32                     Data
  )
{
  UINT8                      Response;

  PchSbiExecutionEx (
    PID_XHCI,
    Offset,
    MemoryWrite,
    FALSE,
    0xf,
    0x5,
    (PCI_DEVICE_NUMBER_PCH_XHCI << 3) + PCI_FUNCTION_NUMBER_PCH_XHCI,
    &Data,
    &Response
    );
  DEBUG ((DEBUG_INFO, "[0x%x] = 0x%08x\n", Offset, Data));

  if (Response != SBI_SUCCESSFUL) {
    DEBUG ((DEBUG_INFO, "Write [0x%x] Failed!" , Offset));
  }
}

/**
  For xHCI.DbC.Trace does not survive across Sx, that FW agent is responsible to resume tracing back or DbC3 would be suspended.
**/
STATIC
VOID
ConfigureXhciDbcTrace (
  VOID
  )
{
  UINT64                TraceHubBaseAddress;
  UINT32                MtbBar;
  UINT32                MtbBarH;

  TraceHubBaseAddress = PCI_SEGMENT_LIB_ADDRESS (
                          DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                          DEFAULT_PCI_BUS_NUMBER_PCH,
                          PCI_DEVICE_NUMBER_PCH_TRACE_HUB,
                          PCI_FUNCTION_NUMBER_PCH_TRACE_HUB,
                          0
                          );
  MtbBar = PciSegmentRead32 (TraceHubBaseAddress + R_TRACE_HUB_CFG_CSR_MTB_LBAR) & B_TRACE_HUB_CFG_CSR_MTB_RBAL;
  //
  // Ensure MSE is set
  //
  if (!(PciSegmentRead8 (TraceHubBaseAddress + PCI_COMMAND_OFFSET) & EFI_PCI_COMMAND_MEMORY_SPACE)) {
    PciSegmentOr8 (TraceHubBaseAddress + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
  }
  //
  // Set DbC Trace in Payload and Status Base
  //
  SbiDbcSet (R_XHCI_PCR_DCI_DBC_TRACE_IN_PAYLOAD_BP_LOW, MtbBar + R_TRACE_HUB_MEM_MTB_80000);
  SbiDbcSet (R_XHCI_PCR_DCI_DBC_TRACE_IN_STATUS_BP_LOW, MtbBar + R_TRACE_HUB_MEM_MTB_DBCSTSCMD);

  MtbBarH = PciSegmentRead32 (TraceHubBaseAddress + R_TRACE_HUB_CFG_CSR_MTB_UBAR);
  SbiDbcSet (R_XHCI_PCR_DCI_DBC_TRACE_IN_PAYLOAD_BP_HIGH, MtbBarH);
  SbiDbcSet (R_XHCI_PCR_DCI_DBC_TRACE_IN_STATUS_BP_HIGH, MtbBarH);
  //
  // Set DbC Trace in Payload and Status Qualifier
  //
  SbiDbcSet (R_XHCI_PCR_DCI_DBC_TRACE_IN_PAYLOAD_QUALIFIERS, V_XHCI_PCR_DCI_DBC_TRACE_QUALIFIERS);
  SbiDbcSet (R_XHCI_PCR_DCI_DBC_TRACE_IN_STATUS_QUALIFIERS, V_XHCI_PCR_DCI_DBC_TRACE_QUALIFIERS);
}

/**
  This function performs basic initialization for TraceHub, and return if trace hub needs to be power gated.

  This routine will consume address range 0xFE0C0000 - 0xFE3FFFFF for BARs usage.
  Although controller allows access to a 64bit address resource, PEI phase is a 32bit env,
  addresses greater than 4G is not allowed by CPU address space.
  So, the addresses must be limited to below 4G and UBARs should be set to 0.
  If this routine is called by platform code, it is expected EnableMode is passed in as PchTraceHubModeDisabled,
  relying on the Intel TH debugger to enable it through the "Scratchpad0 bit [24]".
  By this practice, it gives the validation team the capability to use single debug BIOS
  to validate the early trace functionality and code path that enable/disable Intel TH using BIOS policy.

  @param[in] EnableMode              Trace Hub Enable Mode from policy

  @retval TRUE                       Need to power gate trace hub
  @retval FALSE                      No need to power gate trace hub
**/
BOOLEAN
TraceHubInitialize (
  IN UINT8                           EnableMode
  )
{
  UINT64                  TraceHubBaseAddress;
  UINT32                  Data32;
  UINT8                   Response;
  UINT32                  MtbBarL;
  UINT32                  SwBarL;
  UINT32                  FwBarL;
  UINT32                  Scrpd0;
  PCH_RESET_DATA          ResetData;

  DEBUG ((DEBUG_INFO, "TraceHubInitialize() - Start\n"));
  MtbBarL = PCH_TRACE_HUB_MTB_BASE_ADDRESS;
  SwBarL = PCH_TRACE_HUB_SW_BASE_ADDRESS;
  FwBarL = PCH_TRACE_HUB_FW_BASE_ADDRESS;
  //
  // Check if Trace Hub Device is present
  //
  TraceHubBaseAddress = PCI_SEGMENT_LIB_ADDRESS (
                          DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                          DEFAULT_PCI_BUS_NUMBER_PCH,
                          PCI_DEVICE_NUMBER_PCH_TRACE_HUB,
                          PCI_FUNCTION_NUMBER_PCH_TRACE_HUB,
                          0
                          );
  if (PciSegmentRead16 (TraceHubBaseAddress) == 0xFFFF) {
    if (EnableMode == TraceHubModeDisabled) {
      DEBUG ((DEBUG_INFO, "TraceHubInitialize() - End. TraceHub device is not present due to TH mode is disabled\n"));
      return FALSE;
    }
    // We are here assuming TH is in PG state, but user opt-in TH enabled, so we un-gate trace hub
    // Noted: Trace hub PG state preserves until G3 / global reset.
    // No need to check tool ownership via SCRPD0[24], for it can only be set when TH is active (un-gated).
    //
    DEBUG ((DEBUG_INFO, "TraceHubInitialize - un-gate trace hub due to user opt-in PCH trace hub mode from disabled to enabled.\n"));
    CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
    StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
    PeiServicesResetSystem2 (EfiResetPlatformSpecific, EFI_SUCCESS, sizeof (PCH_RESET_DATA), &ResetData);
  }
  ///
  /// Clear LPMEN bit in R_TRACE_HUB_CFG_CSR_MTB_LPP_CTL register
  ///
  PchSbiExecutionEx (
    PID_NPK,
    R_TRACE_HUB_MEM_CSR_MTB_LPP_CTL,
    MemoryRead,
    FALSE,
    0x0F,
    0,
    (PCI_DEVICE_NUMBER_PCH_TRACE_HUB << 3) + PCI_FUNCTION_NUMBER_PCH_TRACE_HUB,
    &Data32,
    &Response
    );
  DEBUG ((DEBUG_INFO, "LPP_CTL = 0x%08x\n", Data32));

  Data32 &= ~B_TRACE_HUB_MEM_CSR_MTB_LPP_CTL_LPMEN;
  PchSbiExecutionEx (
    PID_NPK,
    R_TRACE_HUB_MEM_CSR_MTB_LPP_CTL,
    MemoryWrite,
    FALSE,
    0x0F,
    0,
    (PCI_DEVICE_NUMBER_PCH_TRACE_HUB << 3) + PCI_FUNCTION_NUMBER_PCH_TRACE_HUB,
    &Data32,
    &Response
    );

  ///
  /// Step 2. Check MSE. Perform initialization only when it has not been set yet.
  /// If yes, then we intitialize the same bar address with no harm.
  ///
  if ((PciSegmentRead32 (TraceHubBaseAddress + PCI_COMMAND_OFFSET) & EFI_PCI_COMMAND_MEMORY_SPACE) == EFI_PCI_COMMAND_MEMORY_SPACE) {
    DEBUG ((DEBUG_INFO, "TraceHubInitialize() - End. Early init done already \n"));
    MtbBarL = PciSegmentRead32 (TraceHubBaseAddress + R_TRACE_HUB_CFG_CSR_MTB_LBAR) & B_TRACE_HUB_CFG_CSR_MTB_RBAL;
    SwBarL = PciSegmentRead32 (TraceHubBaseAddress + R_TRACE_HUB_CFG_SW_LBAR) & B_TRACE_HUB_CFG_SW_RBAL;
    FwBarL = PciSegmentRead32 (TraceHubBaseAddress + R_TRACE_HUB_CFG_FW_LBAR) & B_TRACE_HUB_CFG_FW_RBAL;
    DEBUG ((DEBUG_INFO, "TraceHubInitialize() - MTB_LBAR = 0x%08x\n", MtbBarL));
    DEBUG ((DEBUG_INFO, "TraceHubInitialize() - SW_LBAR  = 0x%08x\n", SwBarL));
    DEBUG ((DEBUG_INFO, "TraceHubInitialize() - FW_LBAR  = 0x%08x\n", FwBarL));
  }

  ///
  /// Step 3. Program the MTB Base Address Register fixed BAR and enable MSE
  ///
  // Set MTB_LBAR (PCI offset 0x10)
  DEBUG ((DEBUG_INFO, "TraceHubInitialize() - Setting MTB_BAR\n"));
  PciSegmentWrite32 (TraceHubBaseAddress + R_TRACE_HUB_CFG_CSR_MTB_LBAR, MtbBarL);
  //
  // Clear MTB_UBAR (PCI offset 0x14)
  //
  PciSegmentWrite32 (TraceHubBaseAddress + R_TRACE_HUB_CFG_CSR_MTB_UBAR, 0);
  //
  // Enable MSE to access MMIO
  //
  PciSegmentOr8 (TraceHubBaseAddress + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
  ///
  /// Step 4. Check if STT is disconnected and if user requested disabling of Trace Hub
  ///
  Scrpd0 = MmioRead32 ((UINT32) (PCH_TRACE_HUB_MTB_BASE_ADDRESS + R_TRACE_HUB_MEM_CSR_MTB_SCRATCHPAD0));
  if ((((Scrpd0 & B_TRACE_HUB_MEM_CSR_MTB_SCRATCHPAD0_DEBUGGER_IN_USE) == 0) && (EnableMode == TraceHubModeDisabled)) ||
      (Scrpd0 == 0xffffffff)) {
    if (Scrpd0 == 0xffffffff) {
      DEBUG ((DEBUG_INFO, "Pch Trace Hub MMIO inaccessible, disable it\n"));
    }
    ///
    /// Step 4.1 Clear MSE
    ///
    PciSegmentWrite8 (TraceHubBaseAddress + PCI_COMMAND_OFFSET, 0);
    ///
    /// Step 4.2 Clear MTB_BAR
    ///
    //
    // Clear MTB_LBAR (PCI offset 0x10)
    //
    DEBUG ((DEBUG_INFO, "TraceHubInitialize() - Clearing MTB_BAR\n"));
    PciSegmentWrite32 (TraceHubBaseAddress + R_TRACE_HUB_CFG_CSR_MTB_LBAR, 0);
    DEBUG ((DEBUG_INFO, "TraceHubInitialize() - End. STT disconnected and Trace Hub requested to be disable\n"));
    return TRUE;
  }
  //
  // Setup xHCI DbC for Trace Hub tracing
  //
  DEBUG ((DEBUG_INFO, "TraceHubInitialize() - Setup DbC3 Tracing\n"));
  ConfigureXhciDbcTrace ();
  ///
  /// Step. 5 Clear MSE
  ///
  PciSegmentWrite8 (TraceHubBaseAddress + PCI_COMMAND_OFFSET, 0);

  ///
  /// Step 6. Program the SW Base Address Register with fixed BAR
  ///
  //
  // SW_LBAR (PCI offset 0x18)
  //
  DEBUG ((DEBUG_INFO, "TraceHubInitialize() - Setting SW_BAR\n"));
  PciSegmentWrite32 (TraceHubBaseAddress + R_TRACE_HUB_CFG_SW_LBAR, SwBarL);
  //
  // SW_UBAR (PCI offset 0x1C)
  //
  PciSegmentWrite32 (TraceHubBaseAddress + R_TRACE_HUB_CFG_SW_UBAR, 0);

  ///
  /// Step 7. Program the FW BAR and Shadow PCI Device
  /// (PCI offset 0x70+0x74) - aka FTMR
  ///
  /// At this point, a shadow PCI device (0/20/4) within the backbone PSF needs to be programmed
  /// with the value of FW BAR, have its memory space enabled, and then hide the shadow device
  ///
  DEBUG ((DEBUG_INFO, "TraceHubInitialize() - Setting FW_BAR\n"));
  TraceHubBaseSet (0, (UINT32) FwBarL);

  ///
  /// Step 8. Enable MSE and BME
  ///
  DEBUG ((DEBUG_INFO, "TraceHubInitialize() - Enabling MSE and BME\n"));
  PciSegmentOr8 (TraceHubBaseAddress + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER);

  DEBUG ((DEBUG_INFO, "TraceHubInitialize () Assigned BARs:\n"));
  DEBUG ((DEBUG_INFO, "TraceHubInitialize () FW_LBAR  = 0x%08x\n", PciSegmentRead32 (TraceHubBaseAddress + R_TRACE_HUB_CFG_FW_LBAR)));
  DEBUG ((DEBUG_INFO, "TraceHubInitialize () MTB_LBAR = 0x%08x\n", PciSegmentRead32 (TraceHubBaseAddress + R_TRACE_HUB_CFG_CSR_MTB_LBAR)));
  DEBUG ((DEBUG_INFO, "TraceHubInitialize () SW_LBAR  = 0x%08x\n", PciSegmentRead32 (TraceHubBaseAddress + R_TRACE_HUB_CFG_SW_LBAR)));
  ///
  /// Lock power gate control register
  ///
  DEBUG ((DEBUG_INFO, "TraceHubInitialize() Locking HSWPGCR1\n"));
  PmcLockHostSwPgCtrl ();

  DEBUG ((DEBUG_INFO, "TraceHubInitialize () - End\n"));
  return FALSE;
}

/**
  Disable and power gate Pch trace hub

  @retval     EFI_SUCCESS       The function completed successfully.
  @retval     EFI_UNSUPPORTED   The device is not supported
**/
EFI_STATUS
PchTraceHubDisable (
  VOID
  )
{
  UINT64                  TraceHubBaseAddress;
  UINT16                  DeviceId;

  ///
  /// Check if Trace Hub Device is present
  ///
  TraceHubBaseAddress = PCI_SEGMENT_LIB_ADDRESS (
                          DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                          DEFAULT_PCI_BUS_NUMBER_PCH,
                          PCI_DEVICE_NUMBER_PCH_TRACE_HUB,
                          PCI_FUNCTION_NUMBER_PCH_TRACE_HUB,
                          0
                          );
  if (PciSegmentRead16 (TraceHubBaseAddress) == 0xFFFF) {
    DEBUG ((DEBUG_INFO, "PchTraceHubDisable() TraceHub device is not present\n"));
    return EFI_UNSUPPORTED;
  }
  DeviceId = PciSegmentRead16 (TraceHubBaseAddress + PCI_DEVICE_ID_OFFSET);
  DEBUG ((DEBUG_INFO, "DeviceId = 0x%x\n", DeviceId));
  ///
  /// Disable trace hub phantom device via PSF
  ///
  PsfDisableTraceHubAcpiDevice ();
  ///
  /// Disable trace hub via PSF
  ///
  DEBUG ((DEBUG_INFO, "PchTraceHubDisable() disable Trace Hub device via PSF\n"));
  PsfDisableTraceHubDevice ();
  ///
  /// Disable Trace Hub
  ///
  PostCode (0xB50);
  if (IsTraceHubPowerGateSupported (DeviceId)) {
    DEBUG ((DEBUG_INFO, "PchTraceHubDisable() Power gate Trace Hub device via PMC\n"));
    PmcDisableTraceHub ();
  }
  ///
  /// Lock power gate control register
  ///
  DEBUG ((DEBUG_INFO, "PchTraceHubDisable() Locking HSWPGCR1\n"));
  PmcLockHostSwPgCtrl ();

  return EFI_SUCCESS;
}

/**
  Function to get PCH Trace Hub Configuration.

  @param[in,out]  DciEn                 - A pointer to the value determining if DCI debug from host enabled.
  @param[in,out]  PchTraceHubConfig     - A pointer to the PCH Trace Hub configuration.

  @retval  EFI_SUCCESS                  - The function completed successfully.
**/
EFI_STATUS
GetPchTraceHubConfig (
  IN OUT  UINT32                            *DciEn,
  IN OUT  TRACE_HUB_CONFIG                  *PchTraceHubConfig
  )
{
  EFI_STATUS                        Status;
  PCH_TRACE_HUB_PREMEM_CONFIG       *PchTraceHubConfigBlock;
  PCH_DCI_PREMEM_CONFIG             *DciPreMemConfig;
  SI_PREMEM_POLICY_PPI              *SiPreMemPolicy;

  Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, &SiPreMemPolicy);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to locate SiPreMemPolicyPpi. Status= %r\n", Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }
  Status = GetConfigBlock (SiPreMemPolicy,
                           &gDciPreMemConfigGuid,
                           &DciPreMemConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get DciPreMemConfig. Status= %r\n", Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }
  Status = GetConfigBlock (SiPreMemPolicy,
                           &gPchTraceHubPreMemConfigGuid,
                           &PchTraceHubConfigBlock);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get PchTraceHubConfig. Status= %r\n", Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  *DciEn = DciPreMemConfig->DciEn;
  *PchTraceHubConfig = PchTraceHubConfigBlock->TraceHub;

  return EFI_SUCCESS;
}
