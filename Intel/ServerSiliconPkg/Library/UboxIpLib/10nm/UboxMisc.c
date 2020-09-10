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
#include "SysHost.h"
#include <Library/CsrAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/RcDebugLib.h>
#include <Upi/KtiHost.h>
#include <Library/UpiHostDataLib.h>

/**
  UBOX: Configure SBREG bar Page Table Entry register

  @param SocId              - CPU Socket Node number (Socket ID)
  @param BoxInst            - Box Instance, 0 based
  @param PortId             - Port ID for SBREG bar Page Table Entry register

  @retval                   - EFI_UNSUPPORTED: the function not implemented
                              EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxConfigureSbRegPteReg(
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   PortId
  )
{
  SBREG_PTE0_UBOX_MISC_STRUCT SbRegPte0;

  SbRegPte0.Data = UsraCsrRead (SocId, BoxInst, SBREG_PTE0_UBOX_MISC_REG);
  SbRegPte0.Bits.dest_portid = (UINT8)(PortId & 0xFF);
  SbRegPte0.Bits.valid = 1;
  UsraCsrWrite (SocId, BoxInst, SBREG_PTE0_UBOX_MISC_REG, SbRegPte0.Data);

  return EFI_SUCCESS;
}