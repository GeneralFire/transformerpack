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
  UBOX: Configure Routing table per Port

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param ChaPortFieldWidth    - Width of field used to CHA port
  @param LocalPort            - Local port used for routing
  @param DestSocId            - Dest CPU Socket Node number (Socket ID)

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureRouteTablePerPort (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    PortFieldWidth,
  IN UINT8    LocalPort,
  IN UINT8    DestSocId
  )
{
  UINT32              Data32;
  UINT32              FieldMask;

  FieldMask = FLD_MASK (PortFieldWidth, (PortFieldWidth * DestSocId));

  Data32 = UsraCsrRead (SocId, BoxInst, QPI_RT_UBOX_M2U_REG);
  Data32 = Data32 & ~FieldMask;
  Data32 = Data32 | (LocalPort << (PortFieldWidth * DestSocId));
  UsraCsrWrite ((UINT8)SocId, BoxInst, QPI_RT_UBOX_M2U_REG, Data32);

  return EFI_SUCCESS;
}

/**
  UBOX: Configure Routing table

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param M2PcieKtiRt          - M2Pcie Kti Routing table

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureRouteTable (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   M2PcieKtiRt
  )
{
  QPI_RT_UBOX_M2U_STRUCT   QpiRtUboxM2u;

  //program m2ubox route table
  QpiRtUboxM2u.Data = UsraCsrRead (SocId, BoxInst, QPI_RT_UBOX_M2U_REG);
  QpiRtUboxM2u.Bits.qpiroutingtable = M2PcieKtiRt;
  UsraCsrWrite ((UINT8)SocId, BoxInst, QPI_RT_UBOX_M2U_REG, QpiRtUboxM2u.Data);

  return EFI_SUCCESS;
}