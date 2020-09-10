/** @file
  Implementation of M2PCIE silicon interface

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <Library/CpuCsrAccessLib.h>
#include <RcRegs.h>
#include <Library/MmPciExtLib.h>
#include <Library/IoLib.h>
#include <PCIeErrorTypes.h>

/**
  Programming M2PCIE Error Configuration

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @retval None.

**/
VOID
EFIAPI
M2PcieErrorConfig (
  IN  UINT8  Socket,
  IN  UINT8  Bus,
  IN  UINT8  Device,
  IN  UINT8  Func
  )
{
  //
  // place holder in case some configuration needed
  //
}

/**
  This function is to check the M2PCIE Error, if there is error on M2PCIE on the
  ErrSeverity return TRUE, else return FALSE

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param ErrSeverity The error's severity
  @retval None.

**/
BOOLEAN
EFIAPI
IsIioM2pcieError (
  IN  UINT8  Socket,
  IN  UINT8  Bus,
  IN  UINT8  Device,
  IN  UINT8  Function,
  IN  UINT8  ErrSeverity
  )
{
  R2PINGERRLOG0_M2UPCIE_MAIN_STRUCT        R2PingErrLog0;
  R2EGRERRLOG_M2UPCIE_MAIN_STRUCT          R2EgrErrLog;

  if (ErrSeverity == NONFATAL_ERROR_TYPE || ErrSeverity == FATAL_ERROR_TYPE) {
    R2PingErrLog0.Data = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (R2PINGERRLOG0_M2UPCIE_MAIN_REG), NULL));
    R2EgrErrLog.Data = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (R2EGRERRLOG_M2UPCIE_MAIN_REG), NULL));
    if (R2PingErrLog0.Data != 0 || R2EgrErrLog.Data != 0) {
      return TRUE;
    }
  }

  if (ErrSeverity == COR_ERROR_TYPE) {
  //
  // M2PCIE error are hardcoded to fatal error
  //
  }
  return FALSE;
}

/**
  Clear M2PCIE Error Status register.

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @retval None.

**/
VOID
EFIAPI
ClearM2pcieErrorStatus (
  IN  UINT8  Socket,
  IN  UINT8  Bus,
  IN  UINT8  Device,
  IN  UINT8  Function
  )
{
  R2PINGERRLOG0_M2UPCIE_MAIN_STRUCT        R2PingErrLog0;
  R2EGRERRLOG_M2UPCIE_MAIN_STRUCT          R2EgrErrLog;

  R2PingErrLog0.Data = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (R2PINGERRLOG0_M2UPCIE_MAIN_REG), NULL));
  if (R2PingErrLog0.Data != 0) {
    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (R2PINGERRLOG0_M2UPCIE_MAIN_REG), NULL), R2PingErrLog0.Data);
  }
  R2EgrErrLog.Data = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (R2EGRERRLOG_M2UPCIE_MAIN_REG), NULL));
  if (R2EgrErrLog.Data != 0) {
    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (R2EGRERRLOG_M2UPCIE_MAIN_REG), NULL), R2EgrErrLog.Data);
  }

}


