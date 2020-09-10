/** @file
  This code provides common functions for CPU and PCH Trace Hub.

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

#include <Base.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <IioPlatformData.h>
#include <Library/BaseLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Private/Library/DxeTraceHubLib.h>
#include <Protocol/MpService.h>
#include <Library/PciSegmentLib.h>
#include <Library/IoLib.h>
#include <Library/MtrrLib.h>
#include <Library/MemoryAllocationLib.h>
#include <ConfigBlock/TraceHubConfig.h>
#include <RcRegs.h>
#include <IioRegs.h>
#include <Library/IioDataHubLib.h>
#include <Private/Library/PchPsfPrivateLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/IioAccessLib.h>
#include <Register/PchRegsTraceHub.h>
#include <IndustryStandard/Pci22.h>
#include <Library/CommonTraceHubLib.h>
#include <Library/IioNpkInit.h>
#include <Cpu/Msr/CommonMsr.h>

TRACE_HUB_DEVICE_INFO TraceHubDeviceTable[] = {
  {IIO_TRACE_HUB_PCI_DEVICE_NUMBER, IIO_TRACE_HUB_PCI_FUNCTION_NUMBER},
  {PCI_DEVICE_NUMBER_PCH_TRACE_HUB, PCI_FUNCTION_NUMBER_PCH_TRACE_HUB}
};

/**
  Perform MMIO write for uBIOS

  @param[in]  Address              An address to be written
  @param[in]  Value                A pointer of buffer contains the data to be written

  @retval  RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
EFIAPI
ConfigureMscForTraceHubToLog (
  IN UINTN                    Address,
  IN UINT32                   Value
  )
{
  if (UbiosGenerationEnabled ()) {
    DEBUG ((DEBUG_ERROR, "\n  mov  DWORD PTR ds:[0%llxh], 0%08xh\n", Address, Value));
  }

  return RETURN_SUCCESS;
}

/**
  Function to set all MTRRs on the current thread to the MTRR set passed in.

  @param MtrrSettings -  Pointer to MTRR Settings to use
**/
VOID
SetAllMtrrs (
  VOID    *MtrrSettings
  )
{
  MtrrSetAllMtrrs ((MTRR_SETTINGS*)MtrrSettings);
}

/**
  @param[in]  *SocketPresent                    - Pointer to array indicating if socket is present
  @param[in]  CpuTraceHubConfig                 - All CPU Trace Hubs configurations in two dimensional Array
  @param[in]  *PchTraceHubConfig                - Pointer to PCH Trace Hub configuration

  @return  Total size of all Trace Hub buffers
**/
UINT64
EFIAPI
TraceHubCalculateTotalBufferSizeDxe (
  IN UINT8                    *SocketPresent,
  IN TRACE_HUB_CONFIG         CpuTraceHubConfig[][NUMBER_TRACE_HUB_PER_SOCKET],
  IN TRACE_HUB_INIT_CONFIGS   *PchTraceHubConfig
  )
{
  UINT64           TotalTraceBufferSize;
  UINT8            IioIndex;
  UINT8            TraceHubIndex;

  TotalTraceBufferSize = 0;
  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {
    for (TraceHubIndex = 0; TraceHubIndex < NUMBER_TRACE_HUB_PER_SOCKET; TraceHubIndex++) {
      if ((CpuTraceHubConfig[IioIndex][TraceHubIndex].EnableMode != TraceHubModeDisabled) ) {
        if (SocketPresent[IioIndex] == 0) {
          continue;
        }
        TotalTraceBufferSize += CpuTraceHubConfig[IioIndex][TraceHubIndex].MemReg0Size
          + CpuTraceHubConfig[IioIndex][TraceHubIndex].MemReg1Size;
      }
    }
  }
  if ((PchTraceHubConfig->EnableMode != TraceHubModeDisabled) ) {
    TotalTraceBufferSize += PchTraceHubConfig->MemReg0Size + PchTraceHubConfig->MemReg1Size;
  }
  //
  // make the total size to be powoer of 2, to ensure use least MTRR when set cache
  //
  if (TotalTraceBufferSize > GetPowerOfTwo64 (TotalTraceBufferSize)) {
    TotalTraceBufferSize = 2 * GetPowerOfTwo64 (TotalTraceBufferSize);
  }
  return TotalTraceBufferSize;
}

/**
  Configure Trace Hub Msc operational region regarding to buffer base and size

  @param[in] TraceHubDevice         Specify CPU or PCH trace hub device
  @param[in] Busno                  Bus number
  @param[in] Msc0Base               Base Address of MSC0BAR
  @param[in] Msc0Size               Size of MSC0Size
  @param[in] Msc1Base               Base Address of MSC1BAR
  @param[in] Msc1Size               Size of MSC1Size
**/
VOID
ConfigureMscForTraceHubDxe (
  IN  TRACE_HUB_DEVICE              TraceHubDevice,
  IN  UINT8                         BusNo,
  IN  UINT64                        Msc0Base,
  IN  UINT32                        Msc0Size,
  IN  UINT64                        Msc1Base,
  IN  UINT32                        Msc1Size
  )
{
  UINT32                            TraceHubMtbLBar;
  UINT32                            TraceHubMtbUBar;
  UINT64                            TraceHubMtbBar;
  UINT64                            TraceHubPciBase;
  UINT32                            Msc0BaseLow;
  UINT32                            Msc0BaseUp;
  UINT32                            Msc1BaseLow;
  UINT32                            Msc1BaseUp;
  UINT32                            MsuSpareReg;

  TraceHubPciBase = GetTraceHubPciBase (TraceHubDevice, BusNo);
  PciSegmentOr8 (TraceHubPciBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER);
  ConfigureMscForTraceHubToLog (TraceHubPciBase + PCI_COMMAND_OFFSET, PciSegmentRead8 (TraceHubPciBase + PCI_COMMAND_OFFSET));

  TraceHubMtbLBar = GetTraceHubMtbLBar (TraceHubDevice, BusNo);
  TraceHubMtbUBar = GetTraceHubMtbUBar (TraceHubDevice, BusNo);
  TraceHubMtbBar = LShiftU64 (TraceHubMtbUBar, 32) | TraceHubMtbLBar;

  ///
  ///  Program MSC0BAR, MSC0SIZE, MSC1BAR, MAS1SIZE
  ///
  ///  Write the base address and buffer size for Memory Region 0 to the the MSC0BAR and MSC0SIZE registers.
  ///  Write the base address and buffer size for Memory Region 1 to the the MSC1BAR and MSC1SIZE registers.
  ///  Note that BAR and Size are shifted left 12 bits by the hardware to arrive at physical starting address and size.
  ///  BIOS, therefore, should shift-right by 12 bits before writing a value to these registers.
  ///  This also forces the starting address and size to be on 4kB boundaries.
  ///

  Msc0Base = (Msc0Base) >> 12;
  Msc0BaseLow = (UINT32) Msc0Base;
  Msc0BaseUp = (Msc0Base) >> 32;
  Msc1Base = (Msc1Base) >> 12;
  Msc1BaseLow = (UINT32) Msc1Base;
  Msc1BaseUp = (Msc1Base) >> 32;
  if (Msc0Size != 0) {
    IioMmioWrite32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MTB_MSCBAR(0), Msc0BaseLow);
    IioMmioWrite32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MTB_MSCUBAR(0), Msc0BaseUp);
    IioMmioWrite32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MTB_MSCSIZE(0), (Msc0Size) >> 12);
    DEBUG ((DEBUG_ERROR, "[IIO](TH) [%llx] = 0x%08x\n", TraceHubMtbBar + R_TRACE_HUB_MEM_MTB_MSCBAR(0), MmioRead32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MTB_MSCBAR(0))));
    DEBUG ((DEBUG_ERROR, "[IIO](TH) [%llx] = 0x%08x\n", TraceHubMtbBar + R_TRACE_HUB_MEM_MTB_MSCSIZE(0), MmioRead32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MTB_MSCSIZE(0))));
  }
  if (Msc1Size != 0) {
    IioMmioWrite32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MTB_MSCBAR(1), Msc1BaseLow);
    IioMmioWrite32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MTB_MSCUBAR(1), Msc1BaseUp);
    IioMmioWrite32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MTB_MSCSIZE(1), (Msc1Size) >> 12);
    DEBUG ((DEBUG_ERROR, "[IIO](TH) [%llx] = 0x%08x\n", TraceHubMtbBar + R_TRACE_HUB_MEM_MTB_MSCBAR(1), MmioRead32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MTB_MSCBAR(1))));
    DEBUG ((DEBUG_ERROR, "[IIO](TH) [%llx] = 0x%08x\n", TraceHubMtbBar + R_TRACE_HUB_MEM_MTB_MSCSIZE(1), MmioRead32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MTB_MSCSIZE(1))));
  }

  //
  // Program MSUSPARE register
  //
  MsuSpareReg = MmioRead32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MSUSPARE);
  MsuSpareReg &= ~(B_TRACE_HUB_MEM_MSUAPARE_ALLOW_MEM_ACCESS);
  MsuSpareReg |= B_TRACE_HUB_MEM_MSUAPARE_MSC0_LOCK;
  IioMmioWrite32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MSUSPARE, MsuSpareReg);

}

/**
  Set MTRR attributes for Trace Hub buffer.

  @param[in]  TraceHubMemBase           - Base address of Trace Hub memory buffers
  @param[in]  TraceHubMemSize           - Total size of Trace Hub memory buffers

  @return  Standard EFI_STATUS is returned
**/
EFI_STATUS
SetMtrrsForTraceHubMemory (
  IN UINT64                      TraceHubMemBase,
  IN UINT64                      TraceHubMemSize
  )
{
  MTRR_SETTINGS               MtrrSettings;
  EFI_MP_SERVICES_PROTOCOL    *MpService = NULL;
  EFI_STATUS                  Status;

  Status = gBS->LocateProtocol (
            &gEfiMpServiceProtocolGuid,
            NULL,
            &MpService
            );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[IIO](TH) ERROR: Failed to locate MpServices, Status = %r\n", Status));
    return Status;
  }

  //
  // Only set MTRR on BSP if we can sync APs
  //
  if (MpService != NULL) {
    Status = MtrrSetMemoryAttribute (TraceHubMemBase , TraceHubMemSize, CacheUncacheable);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[IIO](TH) ERROR: Set MTRR attribute fail, Status = %r\n", Status));
    } else {
      MtrrGetAllMtrrs (&MtrrSettings);
      //
      // Sync all AP MTRRs with BSP
      //
      Status = MpService->StartupAllAPs (
                MpService,
                (EFI_AP_PROCEDURE) SetAllMtrrs,
                FALSE,
                NULL,
                0,
                (VOID*)&MtrrSettings,
                NULL
                );
    }
  }
  return Status;
}

/**
  Write MSR_NPK_STH_ACPIBAR_BASE (0x80) on AP's.

  @param[in] MsrValPtr      Pointer to the value to be written
**/
VOID
ApWriteSthAcpiBarMsr (
  VOID                      *MsrValPtr
  )
{
  AsmWriteMsr64 (MSR_NPK_STH_ACPIBAR_BASE, (UINT64)*(UINT64*)MsrValPtr);
}

/**
  Programming MSR_NPK_STH_ACPIBAR_BASE (0x80) with coresponding Trace Hub FW_BAR value.
  In case AetEnableMode is CPU Trace Hub then program MSR in APs in the same Die which Trace Hub belongs to.
  In case AetEnableMode is PCH Trace Hub then program MSR in all processors with PCH Trace Hub FW_BAR.

  @param[in] AetEnableMode        AET Enable Mode knob value directing AET to CPU or PCH Trace Hub
  @param[in] Base                 Value to be written to MSR
  @param[in] IioIndex             In case of AetEnableMode set to AetCpuTraceHub this is Index with the changed IIO (Socket).
                                  In case of AetEnableMode set to AetPchTraceHub this is ignored and not used.
  @param[in] Stack                In case of AetEnableMode set to AetCpuTraceHub this is Trace Hub stack number.
                                  In case of AetEnableMode set to AetPchTraceHub this is ignored and not used.

  @return  Standard EFI_STATUS is returned
**/
EFI_STATUS
IioTraceHubWriteSthAcpibarMsr (
  IN  UINT8                       AetEnableMode,
  IN  UINT64                      Base,
  IN  UINT8                       IioIndex,
  IN  UINT8                       Stack
  )
{
  EFI_STATUS                      Status;
  EFI_MP_SERVICES_PROTOCOL        *MpService;
  EFI_PROCESSOR_INFORMATION       ProcessorInfoBuffer;
  UINTN                           NumberOfProcessors;
  UINTN                           NumberOfEnabledProcessors;
  UINTN                           BspIndex;
  UINT8                           CoreIndex;

  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, &MpService);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

    Status = MpService->WhoAmI (MpService, &BspIndex);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = MpService->GetNumberOfProcessors (MpService, &NumberOfProcessors, &NumberOfEnabledProcessors);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Set lock bit (BIT0) befor writing to MSR
  //
  Base |= BIT0;
  DEBUG ((
    DEBUG_INFO,
    "[IIO](TH) Programming MSR_NPK_STH_ACPIBAR_BASE (0x80) with %s Trace Hub FW_BAR: 0x%llx\n",
    (AetEnableMode == AetCpuTraceHub) ? L"CPU" : L"PCH",
    Base));

  for (CoreIndex = 0; CoreIndex < NumberOfProcessors; CoreIndex++) {
    Status = MpService->GetProcessorInfo (
                          MpService,
                          CoreIndex | CPU_V2_EXTENDED_TOPOLOGY,
                          &ProcessorInfoBuffer);

    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;
    }

    //
    // When AET directed to CPU Trace Hub the respective CPU thread should be configured
    // to trace to the CPU or North Trace Hub of the same CPU die.
    //
    if (AetEnableMode == AetCpuTraceHub) {
      if (ProcessorInfoBuffer.Location.Package != IioIndex ||
            (UINT8) (Stack - FIRST_TRACE_HUB_STACK_NUMBER) != ProcessorInfoBuffer.ExtendedInformation.Location2.Die) {

        continue;
      }
    }

    if (CoreIndex == BspIndex) {
      //
      // Write MSR on BSP
      //
      AsmWriteMsr64 (MSR_NPK_STH_ACPIBAR_BASE, Base);
    } else {
      //
      // Write MSR on APs
      //
      Status = MpService->StartupThisAP (
                            MpService,
                            (EFI_AP_PROCEDURE) ApWriteSthAcpiBarMsr,
                            CoreIndex,
                            NULL,
                            0,
                            &Base,
                            NULL);

      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
        return Status;
      }
    }
  }

  return Status;
}

/**
  Configure PCH Trace Hub device.

  @param[in]  PchTraceHubConfig         PCH Trace Hub configuration
  @param[in]  TraceHubMemSize           Total size of Trace Hub memory buffers
  @param[in]  TraceHubMemBase           Base address for PCH Trace Hub memory buffers

  @return  Standard EFI_STATUS is returned.
**/
EFI_STATUS
IioTraceHubPchConf (
  IN  TRACE_HUB_INIT_CONFIGS        PchTraceHubConfig,
  IN  UINT64                        TraceHubMemSize,
  IN  UINT64                        TraceHubMemBase
  )
{
  UINT32                            MtbBarLow;
  UINT32                            MtbBarHi;
  UINT64                            MtbBar;
  UINT64                            Base;
  UINT32                            FwBarLow;
  UINT32                            FwBarHi;

  //
  // Config Space needs to be accessible to check if debugger is in use (SCRPD0[BIT24])
  //
  if (!IioIsDevicePresent (0, PCH_NPK_BUS, TraceHubDeviceTable[PchTraceHub].DevNum, TraceHubDeviceTable[PchTraceHub].FuncNum, NULL)) {
    PsfUnhideTraceHubDevice ();
    if (IioIsDevicePresent (0, PCH_NPK_BUS, TraceHubDeviceTable[PchTraceHub].DevNum, TraceHubDeviceTable[PchTraceHub].FuncNum, NULL)) {
      DEBUG ((DEBUG_ERROR, "[IIO](TH) PCH Trace Hub exposed to check if debugger in use.\n"));
    } else {
      DEBUG ((DEBUG_ERROR, "[IIO](TH) PCH Trace Hub not accessible. Disabled.\n"));
      return EFI_NOT_READY;
    }
  }

  MtbBarLow = GetTraceHubMtbLBar (PchTraceHub, PCH_NPK_BUS);
  MtbBarHi  = GetTraceHubMtbUBar (PchTraceHub, PCH_NPK_BUS);
  MtbBar    = LShiftU64 (MtbBarHi, 32) | MtbBarLow;

  if (MtbBar == 0) {
    DEBUG ((DEBUG_ERROR, "[IIO](TH) [0.0 Bus: %x] ERROR: PCH Trace Hub MTB_BAR = 0\n", PCH_NPK_BUS));
    return EFI_NOT_READY;
  }

  PciSegmentOr8 (GetTraceHubPciBase (PchTraceHub, PCH_NPK_BUS) + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER);

  //
  // Set Host Debugger mode always when debugger in use
  // or Platform Debug Consent enabled and Trace Hub disabled
  //
  if (IsDebuggerInUse (PchTraceHub, MtbBar) ||
      (PchTraceHubConfig.PlatformDebugConsent && PchTraceHubConfig.EnableMode == TraceHubModeDisabled)) {

    PchTraceHubConfig.EnableMode = TraceHubModeHostDebugger;
  }

  //
  // Configure MSC if needed for this device
  //
  if ((PchTraceHubConfig.MemReg0Size != 0 || PchTraceHubConfig.MemReg1Size != 0) &&
      TraceHubMemSize > 0 &&
      TraceHubMemBase > 0 &&
      PchTraceHubConfig.EnableMode != TraceHubModeDisabled) {

      DEBUG ((DEBUG_INFO, "[IIO](TH) Configuring MSC for PCH Trace Hub\n"));
      ConfigureMscForTraceHubDxe (
        PchTraceHub,
        PCH_NPK_BUS,
        TraceHubMemBase,
        PchTraceHubConfig.MemReg0Size,
        TraceHubMemBase + PchTraceHubConfig.MemReg0Size,
        PchTraceHubConfig.MemReg1Size);
  }

  if (PchTraceHubConfig.AetEnableMode == AetPchTraceHub) {
    FwBarLow = GetTraceHubFwLBar (PchTraceHub, PCH_NPK_BUS);
    FwBarHi  = GetTraceHubFwUBar (PchTraceHub, PCH_NPK_BUS);
    Base     = LShiftU64 (FwBarHi, 32) | FwBarLow;
    IioTraceHubWriteSthAcpibarMsr (
      PchTraceHubConfig.AetEnableMode,
      Base,
      MaxIIO,
      MAX_IIO_STACK);
  }

  if (PchTraceHubConfig.EnableMode == TraceHubModeHostDebugger) {
    DEBUG ((DEBUG_INFO, "[IIO](TH) PCH Trace Hub in ACPI mode, hiding Config Space.\n"));
    PsfHideTraceHubDevice ();
  }

  return EFI_SUCCESS;
}

/**
  Trace Hub initialization.

  @return  Standard EFI_STATUS is returned.
**/
EFI_STATUS
IioTraceHubInitialize (
  VOID
  )
{
  EFI_STATUS                    Status;
  UINT8                         Stack;
  UINT8                         IioIndex;
  IIO_GLOBALS                   *IioGlobalData;
  TRACE_HUB_INIT_CONFIGS        PchTraceHubConfig;
  UINT64                        TraceHubMemSize;
  UINT64                        TraceHubMemBase;
  UINT8                         BusNumber;
  UINT8                         TraceHubIndex;
  UINT32                        MtbBarLow;
  UINT32                        MtbBarHi;
  UINT64                        MtbBar;
  UINT64                        Base;

  DEBUG ((DEBUG_INFO, "[IIO](TH) %a\n", __FUNCTION__));

  TraceHubMemBase = 0;

  Status = GetIioGlobalData (&IioGlobalData);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = PchTraceHubGetConfig (&PchTraceHubConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[IIO](TH) ERROR: Failed to get PCH Trace Hub config. Status: %r\n", Status));
    return Status;
  }

  //
  // Calculate total buffer size for PCH and CPU Trace Hubs
  //
  TraceHubMemSize = TraceHubCalculateTotalBufferSizeDxe (
                      IioGlobalData->IioVar.IioVData.SocketPresent,
                      &IioGlobalData->SetupData.CpuTraceHubConfig[0],
                      &PchTraceHubConfig);

  if (TraceHubMemSize > 0) {
    TraceHubMemBase = IioTracHubAllocateBuffer (TraceHubMemSize);

    if (TraceHubMemBase == 0) {
      DEBUG ((DEBUG_ERROR, "[IIO](TH) ERROR: Failed to allocate memory for Trace Hub buffers. MSC not configured.\n"));
    } else {
      Status = SetMtrrsForTraceHubMemory (TraceHubMemBase, TraceHubMemSize);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "[IIO](TH) ERROR: Setting MTRR attributs failed. Status: %r\n", Status));
      }
    }
  } else {
    DEBUG ((DEBUG_ERROR, "[IIO](TH) Trace Hub requested memory Size is 0. Not allocating memory.\n"));
  }

  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {
    if (!IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) {
      continue;
    }
    for (Stack = FIRST_TRACE_HUB_STACK_NUMBER; Stack < FIRST_TRACE_HUB_STACK_NUMBER + NUMBER_TRACE_HUB_PER_SOCKET; Stack++) {
      if (!IioIsStackPresent (IioIndex, Stack)) {
        continue;
      }
      BusNumber = IioTraceHubGetPciBusNo (CpuTraceHub, IioIndex, Stack);

      if (!IioIsDevicePresent (IioIndex, BusNumber, TraceHubDeviceTable[CpuTraceHub].DevNum, TraceHubDeviceTable[CpuTraceHub].FuncNum, NULL)) {
        DEBUG ((DEBUG_ERROR, "[IIO](TH) [%d.%d Bus: %x] Config Space of CPU Trace Hub is not accessible\n", IioIndex, Stack, BusNumber));
        continue;
      }

      Status = IioTraceHubInitPowerConfig (IioIndex, Stack);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "[IIO](TH)[%d.%d] ERROR Initializing Iio Trace Hub power configuration failed. Status: %r\n", IioIndex, Stack, Status));
      }

      if (PchTraceHubConfig.AetEnableMode == AetCpuTraceHub) {
        Base = IioTraceHubInitializeFwBar (IioGlobalData, IioIndex, Stack);
        if (Base != 0) {
          IioTraceHubWriteSthAcpibarMsr (PchTraceHubConfig.AetEnableMode, Base, IioIndex, Stack);
        }
      }

      TraceHubIndex = Stack - FIRST_TRACE_HUB_STACK_NUMBER;

      if (TraceHubMemSize > 0 && TraceHubMemBase > 0) {
        MtbBarLow = GetTraceHubMtbLBar (CpuTraceHub, BusNumber);
        MtbBarHi  = GetTraceHubMtbUBar (CpuTraceHub, BusNumber);
        MtbBar    = LShiftU64 (MtbBarHi, 32) | MtbBarLow;
        if (MtbBar == 0) {
          DEBUG ((DEBUG_ERROR, "[IIO](TH) [%d.%d Bus: %x] ERROR MTB_BAR = 0\n", IioIndex, Stack, BusNumber));
          return EFI_NOT_READY;
        }

        PciSegmentOr8 (GetTraceHubPciBase (CpuTraceHub, BusNumber) + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER);
        if (IsDebuggerInUse (CpuTraceHub, MtbBar)
            || (PchTraceHubConfig.PlatformDebugConsent && IioGlobalData->SetupData.CpuTraceHubConfig[IioIndex][TraceHubIndex].EnableMode == TraceHubModeDisabled)) {
          IioGlobalData->SetupData.CpuTraceHubConfig[IioIndex][TraceHubIndex].EnableMode = TraceHubModeHostDebugger;
        }

        if (IioGlobalData->SetupData.CpuTraceHubConfig[IioIndex][TraceHubIndex].EnableMode != TraceHubModeDisabled) {
          DEBUG ((DEBUG_INFO, "[IIO](TH) [%d.%d Bus: %x] Configuring MSC for CPU Trace Hub\n", IioIndex, Stack, BusNumber));
          ConfigureMscForTraceHubDxe (
            CpuTraceHub,
            BusNumber,
            TraceHubMemBase,
            IioGlobalData->SetupData.CpuTraceHubConfig[IioIndex][TraceHubIndex].MemReg0Size,
            TraceHubMemBase + IioGlobalData->SetupData.CpuTraceHubConfig[IioIndex][TraceHubIndex].MemReg0Size,
            IioGlobalData->SetupData.CpuTraceHubConfig[IioIndex][TraceHubIndex].MemReg1Size);

          //
          // Move base address for next Trace Hub MSC buffer
          //
          TraceHubMemBase += IioGlobalData->SetupData.CpuTraceHubConfig[IioIndex][TraceHubIndex].MemReg0Size
            + IioGlobalData->SetupData.CpuTraceHubConfig[IioIndex][TraceHubIndex].MemReg1Size;
        } else {
          PciSegmentAnd8 (GetTraceHubPciBase (CpuTraceHub, BusNumber) + PCI_COMMAND_OFFSET, (UINT8)~(EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER));
        }
      }

      if (IioGlobalData->SetupData.CpuTraceHubConfig[IioIndex][TraceHubIndex].EnableMode == TraceHubModeHostDebugger) {
        HideCpuTraceHub (IioIndex, TraceHubIndex);
      }

    }
  }

  Status = IioTraceHubPchConf (PchTraceHubConfig, TraceHubMemSize, TraceHubMemBase);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[IIO](TH) ERROR Failed to configure PCH Trace Hub. Status= %r\n", Status));
  }

  return Status;
}
