/** @file
  This file implements an API for enabling Block and drain operations in
  IIO Trace hub PCI devices.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <Base.h>
#include <PiDxe.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/IioDataHubLib.h>
#include <Library/IioAccessLib.h>
#include <IioPlatformData.h>
#include <Library/IioInitLib.h>
#include <Library/IioInitDxeLib.h>
#include <Library/IioNpkInit.h>
#include <Pi/PiBootMode.h>
#include <Library/UefiBootServicesTableLib.h>
#include <RcRegs.h>
#include <Library/UsraAccessApi.h>
#include <Library/UsraCsrLib.h>
#include <IndustryStandard/Pci22.h>
#include <ConfigBlock/TraceHubConfig.h>
#include <Library/SystemInfoLib.h>
#include <LibraryPrivate/IioDebug.h>
#include <IioRegs.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/KtiApi.h>
#include <Library/IioMs2iosfInit.h>
#include <PchReservedResources.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/MemMapDataLib.h>
#include <Private/Library/DxeTraceHubLib.h>

#include "IioNpkInitInternal.h"

#define CNVRT_64MB_TO_BYTE             26
#define MEM_4GB_WITH_64MB_GRAN         0x40

extern EFI_HANDLE       mImageHandle;

/**
  Ensures that the Trace Hub device BAR memory space is activated for use.

  @param  Socket                Designates socket in which to activate Trace Hub on

  @retval VOID
**/
VOID
EnableTraceHubMemSpace (
  UINT8  Socket,
  UINT8  TraceHubBusNo
  )
{
  UINT16                        TraceHubCmdRegOld;
  UINT16                        TraceHubCmdReg;

  TraceHubCmdRegOld = IioPciExpressRead16 (Socket,
                                          TraceHubBusNo,
                                          IIO_TRACE_HUB_PCI_DEVICE_NUMBER,
                                          IIO_TRACE_HUB_PCI_FUNCTION_NUMBER,
                                          IIO_TRACE_HUB_PCI_CMD_REG);

  //
  // Enable config space on Trace Hub device
  //
  TraceHubCmdReg = TraceHubCmdRegOld | EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER;
  IioPciExpressWrite16 (Socket,
                        TraceHubBusNo,
                        IIO_TRACE_HUB_PCI_DEVICE_NUMBER,
                        IIO_TRACE_HUB_PCI_FUNCTION_NUMBER,
                        IIO_TRACE_HUB_PCI_CMD_REG,
                        TraceHubCmdReg);

  return;
}

/**
  This routine enables block and drain operations for IIO trace hub PCI devices.

  @param[in] IioIndex      - Index with the changed IIO (Socket)
  @param[in] Stack         - Trace Hub stack number

  @return Standard EFI_STATUS is returned.

**/
EFI_STATUS
IioTraceHubInitPowerConfig (
  IN UINT8             IioIndex,
  IN UINT8             Stack
  )
{
  EFI_STATUS                  Status = EFI_SUCCESS;
  UINT8                       TraceHubBusNo;
  UINT16                      TraceHubCmdRegOld;
  UINT16                      TraceHubCmdReg;
  UINT32                      MtbBarL;
  UINT32                      MtbBarH;
  UINT64                      MtbBar;
  UINT32                      PmtcsReg;
  IIO_GLOBALS                *IioGlobalData;

  IIO_D_THDBG ("[%d.%d] %a Start\n", IioIndex, Stack, __FUNCTION__);

  Status = GetIioGlobalData (&IioGlobalData);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  TraceHubBusNo = IioTraceHubGetPciBusNo (CpuTraceHub, IioIndex, Stack);
  //
  // Get MTB_LBAR and MTB_UBAR values & combine it to get 64 bit MTB_BAR address.
  //
  MtbBarL = IioPciExpressRead32 (IioIndex, TraceHubBusNo, IIO_TRACE_HUB_PCI_DEVICE_NUMBER,
                                  IIO_TRACE_HUB_PCI_FUNCTION_NUMBER, IIO_TRACE_HUB_CSR_MTB_LBAR_REG);

  MtbBarH = IioPciExpressRead32 (IioIndex, TraceHubBusNo, IIO_TRACE_HUB_PCI_DEVICE_NUMBER,
                                  IIO_TRACE_HUB_PCI_FUNCTION_NUMBER, IIO_TRACE_HUB_CSR_MTB_UBAR_REG);

  MtbBar = (((UINT64)MtbBarH)<<32) | (MtbBarL & (~0x7));

  //
  // Setting memory space enable bit in PCI command register, so we can perform MMIO access to
  // PMTCS register which is relative to the MTB BAR.
  //
  TraceHubCmdRegOld = IioPciExpressRead16 (IioIndex, TraceHubBusNo, IIO_TRACE_HUB_PCI_DEVICE_NUMBER,
                                            IIO_TRACE_HUB_PCI_FUNCTION_NUMBER, IIO_TRACE_HUB_PCI_CMD_REG);

  TraceHubCmdReg = TraceHubCmdRegOld | MEMORY_SPACE_ENABLE_BIT;
  IioPciExpressWrite16 (IioIndex, TraceHubBusNo, IIO_TRACE_HUB_PCI_DEVICE_NUMBER, IIO_TRACE_HUB_PCI_FUNCTION_NUMBER,
                          IIO_TRACE_HUB_PCI_CMD_REG, TraceHubCmdReg);

  //
  // Read PMTCS Register by adding its offset(0xFFD00) to MTB Base Address Value
  // and then set Block and Drain Enable bit(BIT0) in the PMTCS register.
  //
  PmtcsReg = MmioRead32 ((UINTN) MtbBar + PMTCS_REG_OFFSET);
  PmtcsReg |= BLOCK_DRAIN_ENABLE_BIT;
  IioMmioWrite32 ((UINTN) MtbBar + PMTCS_REG_OFFSET, PmtcsReg);

  //
  // Restore the Trace Hub PCI command register value.
  //
  IioPciExpressWrite16 (IioIndex, TraceHubBusNo, IIO_TRACE_HUB_PCI_DEVICE_NUMBER, IIO_TRACE_HUB_PCI_FUNCTION_NUMBER,
                          IIO_TRACE_HUB_PCI_CMD_REG, TraceHubCmdRegOld);

  return Status;
}

/**
  Get FW_BAR alignment

  @param[in] IioIndex             Index with the changed IIO (Socket)
  @param[in] TraceHubBusNo        Bus number of the device

  @return  Alignment of FW_BAR
**/
UINT8
IioTraceHubGetFwBarAlignment (
  IN  UINT8             IioIndex,
  IN  UINT8             TraceHubBusNo
  )
{
  UINT32                          FwLbar;
  UINT32                          FwUbar;
  UINT64                          FwBar;
  UINT8                           Alignment;

  //
  // Dynamically check alignment of FW_BAR. Write F's and read the alignment
  //
  IioPciExpressWrite32 (
    IioIndex,
    TraceHubBusNo,
    IIO_TRACE_HUB_PCI_DEVICE_NUMBER,
    IIO_TRACE_HUB_PCI_FUNCTION_NUMBER,
    IIO_TRACE_HUB_FW_LBAR_REG,
    0xFFFFFFFF);
  FwLbar = IioPciExpressRead32 (
            IioIndex,
            TraceHubBusNo,
            IIO_TRACE_HUB_PCI_DEVICE_NUMBER,
            IIO_TRACE_HUB_PCI_FUNCTION_NUMBER,
            IIO_TRACE_HUB_FW_LBAR_REG);

  FwLbar &= (~0x7);

  IioPciExpressWrite32 (
    IioIndex,
    TraceHubBusNo,
    IIO_TRACE_HUB_PCI_DEVICE_NUMBER,
    IIO_TRACE_HUB_PCI_FUNCTION_NUMBER,
    IIO_TRACE_HUB_FW_UBAR_REG,
    0xFFFFFFFF);
  FwUbar = IioPciExpressRead32 (
            IioIndex,
            TraceHubBusNo,
            IIO_TRACE_HUB_PCI_DEVICE_NUMBER,
            IIO_TRACE_HUB_PCI_FUNCTION_NUMBER,
            IIO_TRACE_HUB_FW_UBAR_REG);

  FwBar = LShiftU64 (FwUbar, 32) | FwLbar;
  Alignment = 0;
  while (!(FwBar & ((UINT64) 1 << Alignment)) && (Alignment < 64)) {
    Alignment++;
  }

  return Alignment;
}

/**
  This routine initializes FW_BAR for IIO trace hub PCI devices.

  @param[in] IioGlobalData   Pointer to IIO Universal Data Structure (UDS)
  @param[in] IioIndex        Index with the changed IIO (Socket)
  @param[in] Stack           Trace Hub stack number

  @return programmed FW_BAR value

**/
UINT64
IioTraceHubInitializeFwBar (
  IN  IIO_GLOBALS                 *IioGlobalData,
  IN  UINT8                       IioIndex,
  IN  UINT8                       Stack
  )
{
  EFI_STATUS                      Status;
  UINT8                           TraceHubBusNo;
  UINT32                          FwLbar;
  UINT32                          FwUbar;
  KTI_STACK_RESOURCE              StackResource;
  UINT8                           Alignment;
  UINT32                          FwBarSize;
  UINT64                          Base;


  GetStackResouce (IioIndex, Stack, &StackResource);

  Base = (((UINT64)StackResource.MmiohLimit.hi) << 32) | StackResource.MmiohLimit.lo;
  TraceHubBusNo = IioTraceHubGetPciBusNo (CpuTraceHub, IioIndex, Stack);

  Alignment = IioTraceHubGetFwBarAlignment (IioIndex, TraceHubBusNo);
  FwBarSize = 1 << Alignment;
  Status = gDS->AllocateMemorySpace (
                  EfiGcdAllocateMaxAddressSearchTopDown,
                  EfiGcdMemoryTypeMemoryMappedIo,
                  Alignment,
                  FwBarSize,
                  &Base,
                  mImageHandle,
                  NULL
                );
  if (EFI_ERROR (Status)) {
    IIO_D_THERR ("[%d.%d] Allocating pages for FW_BAR failed (%r)\n", IioIndex, Stack, Status);
    return 0;
  }

  EnableTraceHubMemSpace (IioIndex, TraceHubBusNo);

  FwLbar = (UINT32) Base;

  IioPciExpressWrite32 (
    IioIndex,
    TraceHubBusNo,
    IIO_TRACE_HUB_PCI_DEVICE_NUMBER,
    IIO_TRACE_HUB_PCI_FUNCTION_NUMBER,
    IIO_TRACE_HUB_FW_LBAR_REG,
    FwLbar);

  FwUbar = (UINT32) (Base >> 32);

  IioPciExpressWrite32 (
    IioIndex,
    TraceHubBusNo,
    IIO_TRACE_HUB_PCI_DEVICE_NUMBER,
    IIO_TRACE_HUB_PCI_FUNCTION_NUMBER,
    IIO_TRACE_HUB_FW_UBAR_REG,
    FwUbar);

  //
  // Need to program FW_BAR bank to allow decoding
  //
  IioMs2iosfUpdateFwBarShadowReg (IioGlobalData, IioIndex, Stack, Base);
  IioTraceHubPsfShadowPciFunc (IioIndex, Stack - FIRST_TRACE_HUB_STACK_NUMBER, FwLbar, FwUbar);

  return Base;
}

/**
  Allocates memory buffer of specified size.

  @param[in]  TraceHubMemSize           - Total size of Trace Hub memory buffers

  @returns  Address of allocated buffer
**/
UINT64
IioTracHubAllocateBuffer (
  IN UINT64                 TraceHubMemSize
  )
{
  UINT64                          Address;
  EFI_STATUS                      Status;
  SYSTEM_MEMORY_MAP_HOB           *SystemMemoryMap;
  UINT32                          Index;
  EFI_PHYSICAL_ADDRESS            CurrentElementBegin;
  UINT64                          CurrentElementSize;

  Address = 0;
  Status = EFI_SUCCESS;

  SystemMemoryMap = GetSystemMemoryMapData ();

  if (SystemMemoryMap == NULL) {
    ASSERT_EFI_ERROR (EFI_NOT_FOUND);
    return 0;
  }

  //
  // Iterate over memory elements
  //
  for (Index = 0; Index < SystemMemoryMap->numberEntries; Index++) {
    if (SystemMemoryMap->Element[Index].BaseAddress < MEM_4GB_WITH_64MB_GRAN) {
      //
      // Skip the element contains memory < 4G
      //
      continue;
    }

    CurrentElementSize = LShiftU64 (SystemMemoryMap->Element[Index].ElementSize, CNVRT_64MB_TO_BYTE);
    if ((SystemMemoryMap->Element[Index].Type == MemType1lmDdr || SystemMemoryMap->Element[Index].Type == MemType2lmDdrCacheMemoryMode) &&
        CurrentElementSize > TraceHubMemSize) {

      //
      // This is memory element with required size and type
      //
      CurrentElementBegin = LShiftU64 (SystemMemoryMap->Element[Index].BaseAddress, CNVRT_64MB_TO_BYTE);

      Address = CurrentElementBegin + CurrentElementSize;
      do {
        Address -= TraceHubMemSize;
        Address &= ~( (EFI_PHYSICAL_ADDRESS) EFI_PAGE_MASK);

        if (CurrentElementBegin > Address) {
          //
          // End of this memory chunk reached
          //
          Status = EFI_OUT_OF_RESOURCES;
          break;
        }
        Status = gBS->AllocatePages (
                        AllocateAddress,
                        EfiReservedMemoryType,
                        EFI_SIZE_TO_PAGES((UINTN) TraceHubMemSize),
                        &Address
                        );

        if (!EFI_ERROR (Status)) {
          IIO_D_THLOG ("%a (%r). Address: 0x%llx\n", __FUNCTION__, Status, Address);
          return Address;
        }
      } while (EFI_ERROR (Status));
    } // if valid memory element
  } // for (iteration of memory elements)

  IIO_D_THDBG ("%a (%r)\n", __FUNCTION__, Status);
  return 0;
}
