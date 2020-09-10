/** @file
  Header file for CPU and PCH Trace Hub Lib.

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
#ifndef _BASE_TRACEHUB_LIB_H_
#define _BASE_TRACEHUB_LIB_H_

#include <IioRegs.h>
#include <Register/PchRegsTraceHub.h>
#include <ConfigBlock/TraceHubConfig.h>

#define PCH_NPK_BUS                                   0
#define R_TRACE_HUB_MEM_CSR_MTB_LBAR                  0x10
#define R_TRACE_HUB_MEM_CSR_MTB_UBAR                  0x14
#define R_TRACE_HUB_FW_LBAR                           0x70
#define R_TRACE_HUB_FW_UBAR                           0x74
#define R_TRACE_HUB_MEM_MTB_SCRPD_0                   0xE0
#define B_TRACE_HUB_MEM_MTB_SCRPD_0_DEBUGGER_IN_USE   BIT24
#define R_TRACE_HUB_MEM_MSUSPARE                      0xA00FC
#define B_TRACE_HUB_MEM_MSUAPARE_ALLOW_MEM_ACCESS     BIT1
#define B_TRACE_HUB_MEM_MSUAPARE_MSC0_LOCK            BIT2
#define R_TRACE_HUB_MEM_MMIO_SWP_LOW                  0xFFC38
#define R_TRACE_HUB_MEM_MMIO_SWP_UP                   0xFFC3C
#define R_TRACE_HUB_MEM_MTB_MSC0BASE                  0xA0100
#define R_TRACE_HUB_MEM_MTB_MSC0UBASE                 0xA0134
#define R_TRACE_HUB_MEM_MTB_MSCCTL(n)                 (R_TRACE_HUB_MEM_MTB_MSC0BASE + 0x100 * n)
#define B_TRACE_HUB_MEM_MTB_MSCCTL_MSCEN              BIT0
#define R_TRACE_HUB_MEM_MTB_MSCBAR(n)                 (R_TRACE_HUB_MEM_MTB_MSC0BASE + 0x100 * n + 0x08)
#define R_TRACE_HUB_MEM_MTB_MSCUBAR(n)                (R_TRACE_HUB_MEM_MTB_MSC0UBASE + 0x100 * n)
#define R_TRACE_HUB_MEM_MTB_MSCSIZE(n)                (R_TRACE_HUB_MEM_MTB_MSC0BASE + 0x100 * n + 0x0c)

typedef struct {
  UINT8 DevNum;
  UINT8 FuncNum;
} TRACE_HUB_DEVICE_INFO;

typedef enum {
  CpuTraceHub,
  PchTraceHub
} TRACE_HUB_DEVICE;

typedef enum {
  AetDisabled,
  AetPchTraceHub,
  AetCpuTraceHub,
  AetModeMax
} AET_ENABLE_MODE;

/**
  Get Trace Hub PCI address

  @param[in]  TraceHubDevice            Specify CPU or PCH trace hub device
  @param[in]  BusNo                     Bus number

  @retval UINT64                        Trace Hub Pci address
**/
UINT64
GetTraceHubPciBase (
  IN  TRACE_HUB_DEVICE                  TraceHubDevice,
  IN  UINT8                             BusNo
  );

/**
  Get Trace Hub MTB Lower Bar

  @param[in]  TraceHubDevice            Specify CPU or PCH trace hub device
  @param[in]  BusNo                     Bus number

  @retval UINT32                        Trace Hub MTB Lower BAR
**/
UINT32
GetTraceHubMtbLBar (
  IN  TRACE_HUB_DEVICE                  TraceHubDevice,
  IN  UINT8                             BusNo
  );

/**
  Get Trace Hub MTB Upper Bar

  @param[in]  TraceHubDevice            Specify CPU or PCH trace hub device
  @param[in]  BusNo                     Bus number

  @retval UINT32                        Trace Hub MTB Upper BAR
**/
UINT32
GetTraceHubMtbUBar (
  IN  TRACE_HUB_DEVICE                  TraceHubDevice,
  IN  UINT8                             BusNo
  );

/**
  Get Trace Hub FW Lower Bar

  @param[in]  TraceHubDevice            Specify CPU or PCH trace hub device
  @param[in]  BusNo                     Bus number

  @retval UINT32                        Trace Hub FW Lower BAR
**/
UINT32
GetTraceHubFwLBar (
  IN  TRACE_HUB_DEVICE                  TraceHubDevice,
  IN  UINT8                             BusNo
  );

/**
  Get Trace Hub FW Upper Bar

  @param[in]  TraceHubDevice            Specify CPU or PCH trace hub device
  @param[in]  BusNo                     Bus number

  @retval UINT32                        Trace Hub FW Upper BAR
**/
UINT32
GetTraceHubFwUBar (
  IN  TRACE_HUB_DEVICE                  TraceHubDevice,
  IN  UINT8                             BusNo
  );

/**
  Check if debugger is in use

  @param[in]  TraceHubDevice            Specify CPU or PCH trace hub device
  @param[in]  MtbBar                    Trace Hub MTB_BAR value

  @retval  TRUE                         debugger is in use
  @retval  FALSE                        debugger is NOT in use
**/
BOOLEAN
IsDebuggerInUse (
  IN TRACE_HUB_DEVICE                   TraceHubDevice,
  IN UINTN                              MtbBar
  );

/*
  This function locks untrusted access to Trace Hub buffers BAR registers or if Trace Hub disabled
  or buffers are not requested locks Trace Hub buffers.

  @param[in]  TraceHubConfig            Trace Hub Configuration
  @param[in]  MtbBar                    Trace Hub MTB_BAR value

*/
VOID
TraceHubLockBuffersBar (
  IN  TRACE_HUB_CONFIG                  TraceHubConfig,
  IN  UINT32                            MtbBar
  );

#endif
