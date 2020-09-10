/** @file
  Implementation of patrol scrub interface

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 Intel Corporation. <BR>

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

/**
  Get LLC slice bit map

  @param Socket -- socket index

  @return LLC slice bit map.
**/
UINT64
EFIAPI
GetLlcSliceBitMap (
  IN   UINT8   Socket
  )
{
  CAPID6_PCU_FUN3_STRUCT        Capid6;
  CAPID7_PCU_FUN3_STRUCT        Capid7;
  UINT64                        SliceBitmap;

  Capid6.Data = ReadCpuCsr (Socket, 0, CAPID6_PCU_FUN3_REG);
  Capid7.Data = ReadCpuCsr (Socket, 0, CAPID7_PCU_FUN3_REG);

  SliceBitmap = Capid6.Bits.llc_slice_en_low + LShiftU64(Capid7.Bits.llc_slice_en_high, 32);

  return SliceBitmap;
}


/**
  Get the bit map of enabled LLC IA Core from the Capability Register.

  @param[in] SocketId     Socket index.

  @retval Bit map of enabled LLC IA Core.
**/
UINT64
EFIAPI
GetLlcIaCoreEnBitMap (
  IN UINT8   SocketId
  )
{
  CAPID8_PCU_FUN3_STRUCT        Capid8Reg;
  CAPID9_PCU_FUN3_STRUCT        Capid9Reg;
  UINT64                        LlcIaCoreEnBitMap;

  Capid8Reg.Data = ReadCpuCsr (SocketId, 0, CAPID8_PCU_FUN3_REG);
  Capid9Reg.Data = ReadCpuCsr (SocketId, 0, CAPID9_PCU_FUN3_REG);

  LlcIaCoreEnBitMap = Capid8Reg.Bits.llc_ia_core_en_low + LShiftU64 (Capid9Reg.Bits.llc_ia_core_en_high, 32);

  return LlcIaCoreEnBitMap;
}
