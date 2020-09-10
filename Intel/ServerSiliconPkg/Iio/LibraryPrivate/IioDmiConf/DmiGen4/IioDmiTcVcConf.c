/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#include <Library/DebugLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/SystemInfoLib.h>

#include <Library/IioAccessLib.h>
#include <Library/IioDataHubLib.h>

#include <LibraryPrivate/IioDebug.h>

/**
  Program Vc channels and Tc/Vc mapping

  @param[in] IioGlobalData - Pointer to IIO_GLOBALS
 */
VOID
IioDmiProgramTcVcRegs (
  IN  IIO_GLOBALS *IioGlobalData
  )
{
  UINT32  Data32And;
  UINT32  Data32Or;
  UINT8   TcIndex;
  UINT8   VcIndex;
  UINT32  Register;
  UINT32  Value32;
  UINT8   VcId;
  UINT8   VcMap[MaxDmiVcType] = {0};

  //
  // Create the VcMap based on TC/VC mappings
  //
  for (TcIndex = 0; TcIndex < MaxDmiTcType; TcIndex++) {
    VcIndex = IioGlobalData->IioVar.IioVData.DmiTc[TcIndex];
    if (VcIndex < MaxDmiVcType) {
      VcMap[VcIndex] |= 1 << TcIndex;
    }
  }

  //
  // Write to the VC register the correct configuration
  //
  for (VcIndex = 0; VcIndex < MaxDmiVcType; VcIndex++) {

    if (IioGlobalData->IioVar.IioVData.DmiVc[VcIndex] == 0) {
      //
      // This VC is not enabled
      //
      continue;
    }
    //
    // Map TCs to VC, Set the VC ID, Enable VC
    //
    VcId = IioGlobalData->IioVar.IioVData.DmiVcId[VcIndex];
    Data32And = (UINT32) (~(B_IIO_RCRB_DMIVCxRCTL_VCx_ID_MASK | B_IIO_RCRB_DMIVCxRCTL_VCx_TCVCxM_MASK));
    Data32Or = VcId << 24;  //Bit[26:24] are used for VcId
    Data32Or |= VcMap[VcIndex]; //Mapping is programmed in Bits[7:0], Bit[0] is RO
    Data32Or |= B_IIO_RCRB_DMIVCxRCTL_VCx_EN;

    //
    // Selecting Control VC register
    //
    switch (VcIndex) {
    case DmiTypeVc0:
      Register = VC0RCTL_IIO_PCIEDMI_REG;
      break;
    case DmiTypeVc1:
      Register = VC1RCTL_IIO_PCIEDMI_REG;
      break;
    case DmiTypeVcm:
      Register = VCMRCTL_IIO_PCIEDMI_REG;
      break;
    default:
      ASSERT (FALSE);
      return;
    }

    Value32 = IioReadCpuCsr32 (0, 0, Register);
    Value32 &= Data32And;
    Value32 |= Data32Or;
    IioWriteCpuCsr32 (0, 0, Register, Value32);
  }

  //
  // BIOS must program the extended VC Count:
  // set the DMI Port Register DMIBAR Offset 004h[2:0]=001b
  //
  Data32And = (UINT32) (~0x07);
  if (IioGlobalData->IioVar.IioVData.DmiVc[DmiTypeVc1] == 1) {
    Data32Or = 1;
  } else {
    Data32Or = 0;
  }

  Value32 = IioReadCpuCsr32 (0, 0, PVCCR1_IIO_PCIEDMI_REG);
  Value32 &= Data32And;
  Value32 |= Data32Or;
  IioWriteCpuCsr32 (0, 0, PVCCR1_IIO_PCIEDMI_REG, Value32);
}

/**
  Poll Vc channels and Tc/Vc mapping

  @param[in] IioGlobalData - Pointer to IIO_GLOBALS
 */
VOID
IioDmiPollTcVcRegs (
  IN  IIO_GLOBALS *IioGlobalData
  )
{
  UINT8   VcIndex;
  UINT32  Register;
  UINT8   VcId;

  for (VcIndex = 0; VcIndex < MaxDmiVcType; VcIndex ++) {
    if (IioGlobalData->IioVar.IioVData.DmiVc[VcIndex] != 1) {
      //
      // This VC is not enabled
      //
      continue;
    }

    VcId = IioGlobalData->IioVar.IioVData.DmiVcId[VcIndex];
    IIO_D_DMIDBG ("VcId = %d\n", VcId);
    switch (VcIndex) {
      case DmiTypeVc0:
      Register = VC0RSTS_IIO_PCIEDMI_REG;
      break;
    case DmiTypeVc1:
      Register = VC1RSTS_IIO_PCIEDMI_REG;
      break;
    case DmiTypeVcm:
      Register = VCMRSTS_IIO_PCIEDMI_REG;
      break;
    default:
      ASSERT (FALSE);
      return;
    }

    if (IioGlobalData->IioVar.IioVData.Emulation & SIMICS_FLAG) {
      //
      // Skip polling negotiation in SIMICS
      //
      return;
    }

    //
    // Wait for negotiation to complete
    //
    while ((IioReadCpuCsr32 (0, 0, Register) & B_IIO_RCRB_DMIVCxRSTS_VCx_NP) != 0);
  } // foreach VC
}
