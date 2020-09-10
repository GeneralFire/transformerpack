/** @file
  This code provides base Trace Hub functions.

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
#include <Base.h>
#include <Library/PciSegmentLib.h>
#include <Library/DebugLib.h>
#include <Library/CommonTraceHubLib.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>

extern TRACE_HUB_DEVICE_INFO TraceHubDeviceTable[];

/**
  Get Trace Hub PCI address

  @param[in]  TraceHubDevice            Specify CPU or PCH trace hub device
  @param[in]  BusNo                     Bus number

  @retval UINT64                        Trace Hub Pci address
**/
UINT64
GetTraceHubPciBase (
  IN  TRACE_HUB_DEVICE      TraceHubDevice,
  IN  UINT8                 BusNo
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           0,
           BusNo,
           TraceHubDeviceTable[TraceHubDevice].DevNum,
           TraceHubDeviceTable[TraceHubDevice].FuncNum,
           0
           );
}

/**
  Get Trace Hub MTB Lower Bar

  @param[in]  TraceHubDevice            Specify CPU or PCH trace hub device
  @param[in]  BusNo                     Bus number

  @retval UINT32                        Trace Hub MTB Lower BAR
**/
UINT32
GetTraceHubMtbLBar (
  IN  TRACE_HUB_DEVICE      TraceHubDevice,
  IN  UINT8                 BusNo
  )
{
  UINT32 MtbBar;

  MtbBar = PciSegmentRead32 (GetTraceHubPciBase (TraceHubDevice, BusNo) + R_TRACE_HUB_MEM_CSR_MTB_LBAR) & 0xFFF00000;
  ASSERT (MtbBar != 0xFFF00000);
  return MtbBar;
}

/**
  Get Trace Hub MTB Upper Bar

  @param[in]  TraceHubDevice            Specify CPU or PCH trace hub device
  @param[in]  BusNo                     Bus number

  @retval UINT32                        Trace Hub MTB Upper BAR
**/
UINT32
GetTraceHubMtbUBar (
  IN  TRACE_HUB_DEVICE      TraceHubDevice,
  IN  UINT8                 BusNo
  )
{
  UINT32 MtbBar;

  MtbBar = PciSegmentRead32 (GetTraceHubPciBase (TraceHubDevice, BusNo) + R_TRACE_HUB_MEM_CSR_MTB_UBAR);
  ASSERT (MtbBar != 0xFFFFFFFF);
  return MtbBar;
}

/**
  Get Trace Hub FW Lower Bar

  @param[in]  TraceHubDevice            Specify CPU or PCH trace hub device
  @param[in]  BusNo                     Bus number

  @retval UINT32                        Trace Hub FW Lower BAR
**/
UINT32
GetTraceHubFwLBar (
  IN  TRACE_HUB_DEVICE      TraceHubDevice,
  IN  UINT8                 BusNo
  )
{
  UINT32 FwLbar;

  FwLbar = PciSegmentRead32 (GetTraceHubPciBase (TraceHubDevice, BusNo) + R_TRACE_HUB_FW_LBAR) & (~0x7);
  return FwLbar;
}

/**
  Get Trace Hub FW Upper Bar

  @param[in]  TraceHubDevice            Specify CPU or PCH trace hub device
  @param[in]  BusNo                     Bus number

  @retval UINT32                        Trace Hub FW Upper BAR
**/
UINT32
GetTraceHubFwUBar (
  IN  TRACE_HUB_DEVICE      TraceHubDevice,
  IN  UINT8                 BusNo
  )
{
  UINT32 FwUbar;

  FwUbar = PciSegmentRead32 (GetTraceHubPciBase (TraceHubDevice, BusNo) + R_TRACE_HUB_FW_UBAR);
  ASSERT (FwUbar != 0xFFFFFFFF);
  return FwUbar;
}

/**
  Check if debugger is in use

  @param[in]  TraceHubDevice            Specify CPU or PCH trace hub device
  @param[in]  MtbBar                    Trace Hub MTB_BAR value

  @retval  TRUE                         debugger is in use
  @retval  FALSE                        debugger is NOT in use
**/
BOOLEAN
IsDebuggerInUse (
  IN TRACE_HUB_DEVICE          TraceHubDevice,
  IN UINTN                     MtbBar
  )
{
  UINT32                       Scrpd0;

  Scrpd0 = MmioRead32 (MtbBar + R_TRACE_HUB_MEM_MTB_SCRPD_0);
  DEBUG ((DEBUG_INFO, "[IIO](TH) %s ", ((TraceHubDevice == CpuTraceHub) ? L"CPU" : L"PCH")));
  DEBUG ((DEBUG_INFO, "Trace Hub MTB_BAR: 0x%llx SCRPD0 = 0x%x,", MtbBar, Scrpd0));

  //
  // Return FALSE when SCRPD0 is invalid
  //
  if (Scrpd0 == 0xffffffff) {
    DEBUG ((DEBUG_INFO, " invalid SCRPD0.\n"));
    return FALSE;
  }
  if (Scrpd0 & B_TRACE_HUB_MEM_MTB_SCRPD_0_DEBUGGER_IN_USE) {
    DEBUG ((DEBUG_INFO, " debugger in use.\n"));
    return TRUE;
  } else {
    DEBUG ((DEBUG_INFO, " debugger NOT in use.\n"));
    return FALSE;
  }
}

/*
  This function locks untrusted access to Trace Hub buffers BAR registers or if Trace Hub disabled
  or buffers are not requested locks Trace Hub buffers.

  @param[in]  TraceHubConfig            Trace Hub Configuration
  @param[in]  MtbBar                    Trace Hub MTB_BAR value

*/
VOID
TraceHubLockBuffersBar (
  IN  TRACE_HUB_CONFIG         TraceHubConfig,
  IN  UINT32                   MtbBar
  )
{
  UINT32                       MsuSpareReg;
  UINT32                       MmioSwpLow;
  UINT32                       MmioSwpHi;

  if (TraceHubConfig.EnableMode != TraceHubModeDisabled
  && (TraceHubConfig.MemReg0Size != 0 || TraceHubConfig.MemReg1Size != 0))
  {
    DEBUG ((DEBUG_INFO, "[IIO](TH) Programming SAI write policy reg\n"));
    MmioSwpLow = MmioRead32 (MtbBar + R_TRACE_HUB_MEM_MMIO_SWP_LOW);
    MmioSwpHi = MmioRead32 (MtbBar + R_TRACE_HUB_MEM_MMIO_SWP_UP);
    MmioSwpLow &= ~BIT25;
    MmioSwpHi &= ~BIT26;
    MmioWrite32 (MtbBar + R_TRACE_HUB_MEM_MMIO_SWP_LOW, MmioSwpLow);
    MmioWrite32 (MtbBar + R_TRACE_HUB_MEM_MMIO_SWP_UP, MmioSwpHi);
  } else {
    DEBUG ((DEBUG_INFO, "[IIO](TH) Set lock bit. Trace Hub disabled or buffers requested 0\n"));
    MsuSpareReg = MmioRead32 (MtbBar + R_TRACE_HUB_MEM_MSUSPARE);
    MsuSpareReg |= B_TRACE_HUB_MEM_MSUAPARE_MSC0_LOCK;
    MmioWrite32 (MtbBar + R_TRACE_HUB_MEM_MSUSPARE, MsuSpareReg);
  }
}