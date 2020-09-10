/** @file
  Implementation of CHA related interface

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#include <RcRegs.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/BaseLib.h>
#include <Library/ChaIpLib/Include/ChaRegs.h>

/**
  set LLC error injection.

  @param Socket  -- socket id
  @param Cha  --  Cha index

  @return none
**/
VOID
EFIAPI
SetLlcErrorinjection (
  IN  UINT8    Socket,
  IN  UINT8    Cha,
  IN  UINT64   PhyAddr
  )
{
  SADDBGMM0_CHA_MISC_STRUCT      SadDebugMm0;
  SADDBGMM1_CHA_MISC_STRUCT      SadDebugMm1;
  SADDBGMM2_CHA_MISC_STRUCT      SadDebugMm2;
  UINT64                         Temp;

  //
  // SADDBGMM0[31:0]   - mask field for address[37:6]
  //
  SadDebugMm0.Data = ReadCpuCsr (Socket, Cha, SADDBGMM0_CHA_MISC_REG);
  SadDebugMm0.Bits.mask_37_6  = 00;
  WriteCpuCsr (Socket, Cha, SADDBGMM0_CHA_MISC_REG, SadDebugMm0.Data);

  //
  // SADDBGMM1[7:0]   - mask field for address[45:38]
  // SADDBGMM1[31:8]  - mask field for address[29:6]
  //
  SadDebugMm1.Data = ReadCpuCsr (Socket, Cha, SADDBGMM1_CHA_MISC_REG);
  SadDebugMm1.Bits.mask_51_38 = 00;
  Temp = RShiftU64(PhyAddr, 6);
  Temp &= 0x3ffff;
  SadDebugMm1.Bits.match_23_6 = (UINT32)Temp;
  WriteCpuCsr (Socket, Cha, SADDBGMM1_CHA_MISC_REG, SadDebugMm1.Data);

  //
  // SADDBDMM2[27:07] - match field for address[51:24]
  //
  SadDebugMm2.Data = ReadCpuCsr (Socket, Cha, SADDBGMM2_CHA_MISC_REG);
  SadDebugMm2.Bits.match_51_24        = (UINT32)RShiftU64(PhyAddr, 24);
  SadDebugMm2.Bits.errinjaddrvalid    = 1;
  SadDebugMm2.Bits.errinjopcodevalid  = 1;
  WriteCpuCsr (Socket, Cha, SADDBGMM2_CHA_MISC_REG, SadDebugMm2.Data);
}