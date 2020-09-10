/** @file
  Code File for CPU Power Management

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include "CpuPpmIncludes.h"

/**

  This routine is called to program Software LTR Override. It will be call multiple time passing any the SocketNumber to be programmed.

  @param SocketNumber      Socket index that is executing.
  @parm  PpmPolicy         Pointer to PPM_POLICY_CONFIGURATION instance

  @retval                  None

**/
VOID
EFIAPI
InitializeSwLtrOverride (
  UINT8                    SocketNumber,
  PPM_POLICY_CONFIGURATION *PpmPolicy
  )
{
  UINT64                         RegAddr = 0;
  PCIE_ILTR_OVRD_PCU_FUN1_STRUCT PcieIltrOvrdCsr;


  //
  // Read SW_LTR_OVRD
  //
  PcieIltrOvrdCsr.Data = UsraCsrRead (SocketNumber, 0, PCIE_ILTR_OVRD_PCU_FUN1_REG);

  if (PpmPolicy->AdvPwrMgtCtl.SwLtrOvrdCtl == 1) {
    PcieIltrOvrdCsr.Bits.sxl_v      = PpmPolicy->PcieIltrOvrd.SnpLatVld;
    PcieIltrOvrdCsr.Bits.force_sxl  = PpmPolicy->PcieIltrOvrd.SnpLatOvrd;
    PcieIltrOvrdCsr.Bits.sxlm       = PpmPolicy->PcieIltrOvrd.SnpLatMult;
    PcieIltrOvrdCsr.Bits.sxl        = PpmPolicy->PcieIltrOvrd.SnpLatVal;
    PcieIltrOvrdCsr.Bits.nl_v       = PpmPolicy->PcieIltrOvrd.NonSnpLatVld;
    PcieIltrOvrdCsr.Bits.force_nl   = PpmPolicy->PcieIltrOvrd.NonSnpLatOvrd;
    PcieIltrOvrdCsr.Bits.multiplier = PpmPolicy->PcieIltrOvrd.NonSnpLatMult;
    PcieIltrOvrdCsr.Bits.nstl       = PpmPolicy->PcieIltrOvrd.NonSnpLatVal;

    //
    // Write SW_LTR_OVRD
    //
    UsraCsrWrite (SocketNumber, 0, PCIE_ILTR_OVRD_PCU_FUN1_REG, PcieIltrOvrdCsr.Data);
    RegAddr = UsraGetCsrRegisterAddress (SocketNumber, 0, PCIE_ILTR_OVRD_PCU_FUN1_REG);
    S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &PcieIltrOvrdCsr.Data);
  }

  return;
}
