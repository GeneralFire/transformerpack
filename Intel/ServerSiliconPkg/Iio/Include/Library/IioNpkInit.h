/** @file
  This file contains macros defintion and routine declaration for
  IIO Trace Hub Initialization

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

#ifndef _IIO_NPK_INIT_H_
#define _IIO_NPK_INIT_H_

#include <Uefi.h>
#include <ConfigBlock/TraceHubConfig.h>
#include <IioPlatformData.h>


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
  );

/**
  This routine initializes FW_BAR for IIO trace hub PCI devices.

  @param[in] IioGlobalData   Pointer to IIO Universal Data Structure (UDS)
  @param[in] IioIndex        Index with the changed IIO (Socket)
  @param[in] Stack           Trace Hub stack number

  @return programmed FW_BAR value

**/
UINT64
IioTraceHubInitializeFwBar (
  IN IIO_GLOBALS      *IioGlobalData,
  IN UINT8             IioIndex,
  IN UINT8             Stack
  );

/**
  Allocates memory buffer of specified size.

  @param[in]  TraceHubMemSize           - Total size of Trace Hub memory buffers

  @returns  Address of allocated buffer
**/
UINT64
IioTracHubAllocateBuffer (
  IN UINT64                 TraceHubMemSize
  );

/**
  Hide CPU Trace Hub Config Space. Used when Trace Hub is in ACPI mode.

  @param[in] IioIndex                   IIO instance (usually socket)
  @param[in] TraceHubIndex              Trace Hub Index in socket
**/
VOID
HideCpuTraceHub (
  IN UINT8                              IioIndex,
  IN UINT8                              TraceHubIndex
  );

/**
  Disable CPU Trace Hub PSF function.
  Used when disabling Fabric decode for source decode or abort access when in Fabric decode.

  @param[in] IioIndex                   IIO instance (usually socket)
  @param[in] TraceHubIndex              Trace Hub Index in socket
**/
VOID
IioCpuTraceHubPsfFunctionDisable (
  IN UINT8                              IioIndex,
  IN UINT8                              TraceHubIndex
  );

#endif // _IIO_NPK_INIT_H_
