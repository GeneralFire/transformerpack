/** @file
  .

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

//
// Include files
//
#include <Uefi.h>
#include "Include/UboxRegs.h"
#include <Library/UsraCsrLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/DebugLib.h>

/**
  UBOX: Collect and clear error info.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param M2PcieErrlog         - M2Pcie Error log, output the information into the pointer

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxCollectAndClearErrors (
  IN  UINT8                  SocId,
  IN  UINT8                  BoxInst,
  OUT KTI_CPU_M2PCIE_ERRLOG* M2PcieErrlog
  )
{
  UINT32                            Data32;
  R2PINGERRMSK0_UBOX_M2U_STRUCT     r2pIngErrReg_m2u;

  //
  //M2UBOX
  //
  Data32 = UsraCsrRead (SocId, BoxInst, R2EGRERRLOG_UBOX_M2U_REG);
  M2PcieErrlog->R2Egrerrlog = Data32;
  UsraCsrWrite ((UINT8)SocId, BoxInst, R2EGRERRLOG_UBOX_M2U_REG, Data32);

  Data32 = UsraCsrRead (SocId, BoxInst, R2PINGERRLOG0_UBOX_M2U_REG);// RW1CS
  M2PcieErrlog->R2Ingerrlog0 = Data32;
  UsraCsrWrite ((UINT8)SocId, BoxInst, R2PINGERRLOG0_UBOX_M2U_REG, Data32);

  r2pIngErrReg_m2u.Data = UsraCsrRead (SocId, BoxInst, R2PINGERRMSK0_UBOX_M2U_REG);
  r2pIngErrReg_m2u.Bits.parerring0msk = 1;
  UsraCsrWrite ((UINT8)SocId, BoxInst, R2PINGERRMSK0_UBOX_M2U_REG, r2pIngErrReg_m2u.Data);

  return EFI_SUCCESS;
}

/**
  Select the way R2PCIe routes NDR and DRS.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param SelNdrDrs            - The way R2PCIe routes NDR and DRS
                                packets destined to remote sockets
                                1: using the QPI_RT
                                0: using the Intel UPI Link_ID field

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxSelectR2PCIeRouteNdrAndDrs (
  IN UINT8                  SocId,
  IN UINT8                  BoxInst,
  IN UINT8                  SelNdrDrs
  )
{
  R2PGNCTRL_UBOX_M2U_STRUCT                       UboxR2pgnCtrl;

  //program UBOX_M2U portion
  UboxR2pgnCtrl.Data = UsraCsrRead (SocId, BoxInst, R2PGNCTRL_UBOX_M2U_REG);

  if (SelNdrDrs == 1) {
    UboxR2pgnCtrl.Bits.qpi_rt_sel_ndr_drs = 1; // Set qpi_rt_sel_ndr_drs, Bits[1:1]
  } else {
    UboxR2pgnCtrl.Bits.qpi_rt_sel_ndr_drs = 0;
  }
  UsraCsrWrite (SocId, BoxInst, R2PGNCTRL_UBOX_M2U_REG, UboxR2pgnCtrl.Data);

  return EFI_SUCCESS;
}
