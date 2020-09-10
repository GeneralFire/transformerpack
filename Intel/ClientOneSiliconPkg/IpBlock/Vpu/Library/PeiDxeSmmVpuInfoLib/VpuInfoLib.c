/** @file
  This file provides VPU Public Library implementation

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

#include <Uefi.h>
#include <IndustryStandard/Pci22.h>
#include <Library/VpuInfoLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <Register/SaRegsHostBridge.h>
#include <Register/VpuRegs.h>

/**
  GetVpuBusNumber: Get VPU Bus Number

  @retval PCI bus number for VPU
**/
UINT8
GetVpuBusNumber (
  VOID
  )
{
  return (UINT8)VPU_BUS_NUM;
}

/**
  GetVpuDevNumber: Get VPU Dev Number

  @retval PCI dev number for VPU
**/
UINT8
GetVpuDevNumber (
  VOID
  )
{
  return (UINT8)VPU_DEV_NUM;
}

/**
  GetVpuFunNumber: Get VPU Fun Number

  @retval PCI fun number for VPU
**/
UINT8
GetVpuFuncNumber (
  VOID
  )
{
  return (UINT8)VPU_FUN_NUM;
}

/**
  IsVpuSupported: Check if VPU is supported or not by Silicon.

  @retval FALSE = VPU is fused off. TRUE = VPU is supported by Sillicon.
**/
BOOLEAN
IsVpuSupported (
  VOID
  )
{
  UINT32               CapIdCValue;
  ///
  /// Check if Silicon is VPU capable
  ///
  CapIdCValue = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, R_SA_MC_CAPID0_C));
  if (CapIdCValue & B_SA_MC_CAPID0_C_VPU_DIS) {
    DEBUG ((DEBUG_INFO, "VPU is Fused off\n"));
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "VPU is supported by Silicon\n"));
  return TRUE;
}
