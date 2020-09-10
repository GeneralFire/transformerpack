/** @file
  PEIM Private Library to initialize for VPU device

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
#include <Ppi/SiPolicy.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PeiVpuInitLib.h>
#include <Register/VpuRegs.h>
#include <Register/SaRegsHostBridge.h>
#include <Library/VpuInfoLib.h>
#include <IndustryStandard/Pci22.h>
#include <Library/CpuRegbarAccessLib.h>
#include <CpuSbInfo.h>


/**
  VpuInit: Initialize the VPU device

  @param[in] VPU_PEI_CONFIG             *VpuPeiConfig
**/
VOID
VpuInit (
  IN  VPU_PEI_CONFIG    *VpuPeiConfig
  )
{
  BOOLEAN              VpuEnabled;
  UINT64               VpuBaseAddress;

  DEBUG ((DEBUG_INFO, "VpuInit Begins\n"));
  VpuBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, VPU_BUS_NUM, VPU_DEV_NUM, VPU_FUN_NUM, 0);

  //
  // Check if VPU is supported and enabled.
  //
  if (IsVpuSupported ()) {
    DEBUG ((DEBUG_INFO, "Silicon supports VPU.\n"));
    VpuEnabled = TRUE;
    //
    // VPU is not fused off, check if it is enabled via policy
    //
    if (VpuPeiConfig->VpuEnable == 0) {
      DEBUG ((DEBUG_INFO, "Policy decides to disable VPU\n"));
      VpuEnabled = FALSE;
    }
  } else {
    DEBUG ((DEBUG_INFO, "VPU is not supported\n"));
    VpuEnabled = FALSE;
    return;
  }

  //
  //  Program VPU bit at DEVEN register
  //
  if (VpuEnabled) {
    //
    //  Set D11EN bit at DEVEN register
    //
    DEBUG ((DEBUG_INFO, "Enable VPU via DEVEN regiser\n"));
    PciSegmentOr32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, R_SA_DEVEN), (UINT32) B_SA_DEVEN_D11EN_MASK);
  } else {
    //
    //  Clear D11EN bit at DEVEN register
    //
    DEBUG ((DEBUG_INFO, "Disable VPU via DEVEN regiser\n"));
    PciSegmentAnd32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, R_SA_DEVEN), (UINT32) ~B_SA_DEVEN_D11EN_MASK);
    return;
  }

  //
  //  Buttress initalization
  //

  // PMCTL register offset 0x1D0h contains the low power clock gating controls of the Buttress and must be programmed to 0x001F.
  CpuRegbarWrite32 (CPU_SB_PID_VPU_BUTTREESS, R_VPU_PCR_PMCTL, 0x001F);

  DEBUG ((DEBUG_INFO, "VpuInit Ends\n"));
}