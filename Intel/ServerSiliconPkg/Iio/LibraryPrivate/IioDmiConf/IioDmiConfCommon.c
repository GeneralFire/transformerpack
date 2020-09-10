/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include <Uefi.h>
#include <RcRegs.h>

#include <IndustryStandard/PciExpress21.h>

#include <Library/DebugLib.h>
#include <Library/CpuAndRevisionLib.h>

#include <PchReservedResources.h>
#include <Register/PchRegsPcr.h>
#include <Register/PchRegsPmc.h>
#include <Register/PchRegsDmi.h>
#include <Register/PchRegsPcie.h>
#include <Library/PchPcrLib.h>

#include <Library/IioAccessLib.h>
#include <Library/IioDataHubLib.h>

#include <LibraryPrivate/IioDebug.h>
#include <LibraryPrivate/IioDmiConf.h>


/**
  This routine will update DMI link speed on PCH side.

  On input it takes a vector (bitmap) of supported IIO link speeds,
  matches to the PCH supported link speed vector, and the higest
  speed supported by both IIO and PCH is selected to be set in PCH
  and returned so that the caller can set it also in IIO stack.

  @param[in] IioInded  IIO instance (usually socket)
  @return On exit the highest common link speed is returned.
**/
UINT8
IioDmiSetPchLinkSpeed (
  IN  UINT8  IioIndex
  )
{
  PCH_DMI_PCR_LCAP2 LCap2;
  PCH_DMI_PCR_LCTL2 LCtl2;
  UINT8      IioSpeedVector;
  UINT8      SpeedVector;
  UINT8      Speed;

  IioSpeedVector = IioDmiGetLinkSpeedVector (IioIndex, PORT_0_INDEX);

  LCap2.Dword = IioMmioRead32 (PCH_PCR_ADDRESS (PID_DMI, R_PCH_DMI_PCR_LCAP2));
  SpeedVector = IioSpeedVector & LCap2.Bits.SLSV;
  for (Speed = 0; SpeedVector != 0; Speed++) {
    SpeedVector >>= 1;
  }
  IIO_D_DMILOG ("%a: DMI link speed vector IIO 0x%X, PCH 0x%X -> target speed %d\n", __FUNCTION__,
                IioSpeedVector, LCap2.Bits.SLSV, Speed);
  LCtl2.Dword = IioMmioRead32 (PCH_PCR_ADDRESS (PID_DMI, R_PCH_DMI_PCR_LCTL2));
  if (LCtl2.Bits.TLS != Speed) {

    LCtl2.Bits.TLS = Speed;
    IioMmioWrite32 (PCH_PCR_ADDRESS (PID_DMI, R_PCH_DMI_PCR_LCTL2), LCtl2.Dword);
  }
  return Speed;
} // IioDmiSetPchLinkSpeed()

