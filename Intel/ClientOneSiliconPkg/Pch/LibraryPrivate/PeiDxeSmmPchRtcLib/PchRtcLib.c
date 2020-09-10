/** @file
  PCH RTC library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

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
#include <Uefi/UefiBaseType.h>
#include <Library/PchPcrLib.h>
#include <Library/PchRtcLib.h>
#include <Register/PchRegsLpc.h>
#include <Register/RtcRegs.h>
#include <Register/ItssRegs.h>
#include <Library/IoLib.h>
#include <Register/PchPcrRegs.h>


/**
  @brief Read Backed Up Control (BUC) register.

  @return The register value is returned.
**/
UINT32
EFIAPI
PchRtcReadBuc (
  VOID
  )
{
  UINT32 Data = 0;
  Data = MmioRead32 (PCH_PCR_ADDRESS (PID_RTC_HOST, R_RTC_PCR_BUC));
  return Data;
}


/**
  @brief Write Backed Up Control (BUC) register.

  @param[in] Data Value to write to BUC register.

  @return Void.
**/
VOID
EFIAPI
PchRtcWriteBuc (
  IN UINT32 Data
  )
{
  MmioWrite32 (PCH_PCR_ADDRESS (PID_RTC_HOST, R_RTC_PCR_BUC), Data);
}

UINT8
ReadNmiEn (
  VOID
  )
/*++

  Reads NMI_EN register

--*/
{
  UINT8           Nmi;
  UINT32          Data32;


  Data32 = MmioRead32 (PCH_PCR_ADDRESS (PID_ITSS, R_ITSS_PCR_GIC));
  Data32 |= (UINT32) (B_ITSS_PCR_GIC_AME);;
  MmioWrite32 (PCH_PCR_ADDRESS (PID_ITSS, R_ITSS_PCR_GIC), Data32);

  Nmi     = (IoRead8 (R_PCH_IO_NMI_EN));

  Data32 = MmioRead32 (PCH_PCR_ADDRESS (PID_ITSS, R_ITSS_PCR_GIC));
  Data32 &= (UINT32) ~(B_ITSS_PCR_GIC_AME);
  MmioWrite32 (PCH_PCR_ADDRESS (PID_ITSS, R_ITSS_PCR_GIC), Data32);

  return Nmi;
}
