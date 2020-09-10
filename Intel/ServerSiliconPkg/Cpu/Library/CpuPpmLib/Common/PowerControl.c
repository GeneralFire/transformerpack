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

  This routine is called to program PROCHOT_RESPONSE_RATIO_CFG.
  It will be call multiple time passing any the SocketNumber to be programmed.

  @param SocketNumber      Socket index that is executing.
  @param ProchotRatio      The data of Prochot Ratio

  @retval                  None

**/
VOID
EFIAPI
InitializeProchotResponseRatio (
  UINT8               SocketNumber,
  UINT32              ProchotRatio
  )
{
  UINT32                                 MaxRatio;
  UINT32                                 MinRatio;
  UINT64                                 RegAddr = 0;
  PROCHOT_RESPONSE_RATIO_PCU_FUN2_STRUCT ProchotResponseRatioCsr;


  MaxRatio = (UsraCsrRead (SocketNumber, 0, PLATFORM_INFO_N0_PCU_FUN0_REG) >> 8) & 0x000000ff;  //Bit[15:8]
  MinRatio = (UsraCsrRead (SocketNumber, 0, PLATFORM_INFO_N1_PCU_FUN0_REG) >> 8) & 0x000000ff;  //Bit[15:8]

  // Read CSR
  ProchotResponseRatioCsr.Data = UsraCsrRead (SocketNumber, 0, PROCHOT_RESPONSE_RATIO_PCU_FUN2_REG);

  //
  // If setup value non-zero, set prochot ratio using setup value
  //
  if (ProchotRatio) {
    if (ProchotRatio < MinRatio) {
      ProchotResponseRatioCsr.Bits.prochot_ratio = MinRatio;
    } else if (ProchotRatio > MaxRatio) {
      ProchotResponseRatioCsr.Bits.prochot_ratio = MaxRatio;
    } else {
      ProchotResponseRatioCsr.Bits.prochot_ratio = ProchotRatio;
    }
  //
  // Otherwise, adjust current value within acceptable range if needed (Pn is reset value)
  //
  } else {
    if (ProchotResponseRatioCsr.Bits.prochot_ratio < MinRatio) {
      ProchotResponseRatioCsr.Bits.prochot_ratio = MinRatio;
    } else if (ProchotResponseRatioCsr.Bits.prochot_ratio > MaxRatio) {
      ProchotResponseRatioCsr.Bits.prochot_ratio = MaxRatio;
    }
  }

  UsraCsrWrite (SocketNumber, 0, PROCHOT_RESPONSE_RATIO_PCU_FUN2_REG, ProchotResponseRatioCsr.Data);
  RegAddr = UsraGetCsrRegisterAddress (SocketNumber, 0, PROCHOT_RESPONSE_RATIO_PCU_FUN2_REG);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &ProchotResponseRatioCsr.Data);
}
