/** @file
  Configure Ubox registers.

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
#include <Upi/KtiHost.h>
#include <Library/UpiHostDataLib.h>
#include <Library/CpuAndRevisionLib.h>

/**
  UBOX: Configure UBox MMIO Base and BAR.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param UboxMmioBase         - Ubox MMIO base for socket "SocId"

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureUboxMmio (
  IN UINT8                  SocId,
  IN UINT32                 UboxMmioBase
  )
{
  UINT32              UboxMmioBarOffset[TYPE_MAX_MMIO_BAR] = {  SCF_BAR_UBOX_DFX_REG,
                                                                PCU_BAR_UBOX_DFX_REG,
                                                                MEM0_BAR_UBOX_DFX_REG,
                                                                MEM1_BAR_UBOX_DFX_REG,
                                                                MEM2_BAR_UBOX_DFX_REG,
                                                                MEM3_BAR_UBOX_DFX_REG,
                                                                MEM4_BAR_UBOX_DFX_REG,
                                                                MEM5_BAR_UBOX_DFX_REG,
                                                                MEM6_BAR_UBOX_DFX_REG,
                                                                MEM7_BAR_UBOX_DFX_REG,
                                                                SBREG_BAR_UBOX_DFX_REG,
                                                              };
  SCF_BAR_UBOX_DFX_STRUCT         UboxMmioBarStruct;
  UINT8                           Ctr;
  KTI_HOST_OUT                    *KtiHostOutPtr;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;

  //
  //Program ubox mmio base
  //
  UsraCsrWrite ((UINT8)SocId, 0, MMIO_BASE_UBOX_DFX_REG, UboxMmioBase >> 23);

  //
  //Program each UBOX  MMIO bar
  //
  for (Ctr = 0; Ctr < TYPE_MAX_MMIO_BAR; Ctr++) {
    if (KtiHostOutPtr->MmioBar[SocId][Ctr] == 0) {
      //
      // BAR isn't assigned. Skip this BAR.
      //
      continue;
    }

    UboxMmioBarStruct.Data = 0;
    UboxMmioBarStruct.Bits.valid = 1;
    UboxMmioBarStruct.Bits.base_addr = (KtiHostOutPtr->MmioBar[SocId][Ctr] & 0x007FF000) >> 12;

    if (TYPE_SCF_BAR == Ctr) {
      UboxMmioBarStruct.Bits.limit = ((KtiHostOutPtr->MmioBar[SocId][Ctr] + UBOX_MMIO_SIZE_PER_BAR * 4 - 1) & 0x007FF000) >> 12;
    } else {
      UboxMmioBarStruct.Bits.limit = ((KtiHostOutPtr->MmioBar[SocId][Ctr] + UBOX_MMIO_SIZE_PER_BAR - 1) & 0x007FF000) >> 12;
    }
    //UboxMmioBarStruct.Bits.bar_no = Ctr;
    UsraCsrWrite ((UINT8)SocId, 0, UboxMmioBarOffset[Ctr], UboxMmioBarStruct.Data);
  }

  return EFI_SUCCESS;
}