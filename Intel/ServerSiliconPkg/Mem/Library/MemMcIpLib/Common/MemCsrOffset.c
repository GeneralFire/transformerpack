/** @file
  Memory Initialization Module.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2019 Intel Corporation. <BR>

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
#include <Library/MemoryCoreLib.h>
#include <SysHostChip.h>
#include <UncoreCommonIncludes.h>
#include "Include/MemMcRegs.h"
#include "Include/MemMcIpLibInternal.h"
#include <KtiMisc.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemAccessLib.h>

#ifdef DDR5_SUPPORT
#include <Memory/Ddr5Cmds.h>
#endif

/**

  This function returns the CH corrected offset for each used CPGC register, and corrects for HA0/1 if needed

  @param socket              - Socket number
  @param ch                  - Channel Number
  @param regOffset           - Base register offset

  @retval New register Offset based on CH number

**/
UINT32
GetCpgcRegOffset (
  UINT8     socket,
  UINT8     ch,
  UINT32    Offset
  )
{
  UINT32 regOffset;
  UINT8  NumChPerMc;
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();
  NumChPerMc = GetNumChannelPerMc ();

  //SKX change
  ch = ch % NumChPerMc;

  //
  // Deal with pseudo offsets
  //
  regOffset = MemPciCfgOffset (socket, ch, Offset);

  return regOffset;

} // GetCpgcRegOffset10nm

/**

  Set Directory Enable mode in the MC.

  This routine returns without programming the dir_en register field
  because this dir_en bit in MC is not used by any functional logic
  in MC.

  @param Socket         - Socket number
  @param Mcid           - Memory controller
  @param DirEn          - Directory Enable

  @retval n/a

**/
VOID
SetDirectoryEnableMc (
  IN UINT8     Socket,
  IN UINT8     Mcid,
  IN UINT32    DirEn
  )
{
  //
  // Returns without programming the dir_en register field because this dir_en bit
  // in MC is not used by any functional logic in MC.
  //
  return;
}

/**

  Set dis_isoch_rd and defeature_3 bits in MCDECS_CHICKEN_BITS

  @param Socket         - Socket number
  @param Ch             - Channel
  @param DisIsocRd      - TRUE if IOSC enable; FALSE otherwise
  @param Def3           - TRUE if Directory Mode feature is enabled; FALSE otherwise

  @retval n/a

**/
VOID
SetDisIsocRdDef3Mc (
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     DisIsocRd,
  IN UINT8     Def3
  )
{
  //an EMPTY function to keep API consistency
  return;
}

/**

  Enable/Disable Error injection bits on each channel

  @param[in] MemTechType  - Memory Technology Type
  @param Socket           - Socket number
  @param McId             - Memory controller id
  @param ErrInj           - Value to enable/disable error injection

  @retval N/A

**/
VOID
SetErrInjValPerMc (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          Socket,
  IN UINT8          McId,
  IN UINT8          ErrInj
  )
{
  UINT8                                    Ch;
  UINT8                                    NumChPerMc;
  RSP_FUNC_RANK_BANK_MATCH_MC_MAIN_STRUCT  RspFuncRankBank;

  if (MemTechType == MemTechHbm) {
    NumChPerMc = GetNumChannelPerMcHbm ();
  } else {
    NumChPerMc = GetNumChannelPerMc ();
  }

  for (Ch = 0; Ch < NumChPerMc; Ch++) {
    RspFuncRankBank.Data = ChRegisterRead (MemTechType, Socket, (McId * NumChPerMc + Ch), RSP_FUNC_RANK_BANK_MATCH_MC_MAIN_REG);
    RspFuncRankBank.Bits.dis_tgr_inj  = ErrInj & 0x1;
    RspFuncRankBank.Bits.dis_tgr_inj2 = ErrInj & 0x1;
    ChRegisterWrite (MemTechType, Socket, (McId * NumChPerMc + Ch), RSP_FUNC_RANK_BANK_MATCH_MC_MAIN_REG, RspFuncRankBank.Data);
  }
}
