/** @file

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

#include <Library/MemoryCoreLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemCpgcIpLib.h>
#include "Include/MemCpgcRegs.h"
#include <UncoreCommonIncludes.h>
#include <Library/SysHostPointerLib.h>
#include "Include/CpgcChip.h"
#include "Include/MemCpgcRegCache.h"
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemRoutingLib.h>
#include <Library/MemRcLib.h>

/**

  Set the CPGC Pattern

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket
  @param[in] ChbitMask - Ch Bit mask for which test should be setup for.
  @param[in] Mux0      - MUX 0
  @param[in] Mux1      - MUX 1
  @param[in] Mux2      - MUX 2
  @param[in] Patbuf0   - Pattern Buffer 0
  @param[in] Patbuf1   - Pattern Buffer 1
  @param[in] Patbuf2   - Pattern Buffer 2

  @retval N/A

**/
VOID
SetCpgcPatControl (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChbitMask,
  IN UINT8     Mux0,
  IN UINT8     Mux1,
  IN UINT8     Mux2,
  IN UINT32    Patbuf0,
  IN UINT32    Patbuf1,
  IN UINT32    Patbuf2
  )
{
  UINT8    Ch, SubCh;
  UINT8    MaxChDdr;
  UINT32   MuxCtl[CPGC_MAX_NUMBER_DPAT_UNISEQ];
  UINT32   LfsrSize[CPGC_MAX_NUMBER_DPAT_UNISEQ] = {CPGC_DONT_CARE, CPGC_DONT_CARE, CPGC_DONT_CARE};
  UINT32   Patbuf[CPGC_MAX_NUMBER_DPAT_UNISEQ];

  MaxChDdr = GetMaxChDdr ();

  Patbuf[0] = Patbuf0;
  Patbuf[1] = Patbuf1;
  Patbuf[2] = Patbuf2;
  MuxCtl[0] = Mux0;
  MuxCtl[1] = Mux1;
  MuxCtl[2] = Mux2;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((ChbitMask & (1 << Ch)) == 0) {
      continue;
    }

    for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {
      SetCpgcDpatUniseqCfg (Host, Socket, Ch, SubCh, MuxCtl, LfsrSize);
      SetCpgcDpatUniseqPat (Host, Socket, Ch, SubCh, Patbuf);
    }
  }
}

/*

  Enable/Disable the MPR mode for DDR5 DRAMs

  @param[in]  Host      Pointer to SysHost structure
  @param[in]  Socket    Current socket under test (0-based)
  @param[in]  Ch        Current channel under test (0-based)
  @param[in]  Enable    1: Enable 0: Disable

*/
VOID
SetMprTrainModeDdr5(
  IN        PSYSHOST    Host,
  IN        UINT8       Socket,
  IN        UINT8       Ch,
  IN        UINT8       Enable
  )
{
}

UINT32
GetErrorResults (
  PSYSHOST   Host,
  UINT8      socket,
  UINT8      ch
)
{
  return EFI_SUCCESS;
}


VOID
ClearEridLfsrErr (
                   PSYSHOST Host,
                   UINT8    socket,
                   UINT8    ch
)
{

} // ClearEridLfsrErr


VOID
GetEridCombineResult (
                      PSYSHOST Host,
                      UINT8    socket,
                      UINT8    ch,
                      UINT8    *eridCombinedResult
)
{

} // GetEridCombineResult10nm

VOID
SetEridLfsrErrStatus (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT32   Value
  )
{

} // SetEridLfsrErrStatus


VOID
SetupCoarseEridTraining (
                      PSYSHOST Host,
                      UINT8    socket,
                      UINT8    ch,
                      UINT8    dimm
)
{

} // SetupCoarseEridTraining10nm

VOID
DisableEridLfsrMode (
                      PSYSHOST Host,
                      UINT8    socket,
                      UINT8    ch
)
{

} // DisableEridLfsrMode10nm

VOID
SetupCpgcWdbBuffErid (
                   PSYSHOST Host,
                   UINT8    socket,
                   UINT8    ch
)
{

} // SetupCpgcWdbBuffErid10nm


VOID
PerformChipODTMPRDimmActivationStep (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  PEXECUTE_CTL_CLK_TEST_CHIP_STRUCT executeCtlCLKTestChipStruct
  )
{

}

VOID
DisableChipChlRecEnOffsetTrainingMode(
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  PEXECUTE_CTL_CLK_TEST_CHIP_STRUCT executeCtlCLKTestChipStruct
  )
{

}

VOID
AssertChipODTOveride(
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT32 wrOdtTableData
  )
{

}

VOID
DeAssertChipODTOveride(
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch
  )
{

}

VOID
DoFinalChipSpecificWLCleanUp(
  PSYSHOST Host,
  UINT8 socket
  )
{

}

VOID
DisableEridReturn (
            PSYSHOST Host,
            UINT8    socket
)
{

}

/**

  Set the Mpr training mode to filter ACT/PRE.

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] Ch                                - Channel number (0-based)
  @param[in] Enable                            - 1/0 for Enable or not

  @retval N/A

**/
VOID
SetMprModeFilterActPre (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT8       Ch,
  IN UINT8       Enable
  )
{
  UINT8 SubCh = 0;
  UINT8 ChannelLocal = 0;
  CPGC_MISCODTCTL_MCDDC_CTL_STRUCT   cpgcMiscODTCtl;

  for (SubCh = 0 ; SubCh < CpgcGetValidSubCh (); SubCh++) {
    GetChannelMapping (Ch, SubCh, &ChannelLocal);
    cpgcMiscODTCtl.Data = MemReadPciCfgEp (Socket, ChannelLocal, CPGC_MISCODTCTL_MCDDC_CTL_REG);
    cpgcMiscODTCtl.Bits.mpr_train_ddr_on = Enable;
    MemWritePciCfgEp (Socket, ChannelLocal, CPGC_MISCODTCTL_MCDDC_CTL_REG, cpgcMiscODTCtl.Data);
  }
}

/**
  Get current Mpr training mode.

  @param[in] Socket                            - Socket Id
  @param[in] Ch                                - Channel number (0-based)
  @param[in] SubCh                             - Sub-channel number (0-based)

  @retval UINT8                                - Mpr Mode. 0 - off, 1 - on.

**/
UINT8
GetMprModeFilterActPre (
  IN UINT8       Socket,
  IN UINT8       Ch,
  IN UINT8       SubCh
  )
{
  CPGC_MISCODTCTL_MCDDC_CTL_STRUCT   CpgcMiscODTCtl;
  PSYSHOST                           Host;

  Host = GetSysHostPointer();

  CpgcMiscODTCtl.Data = MemReadPciCfgEp (Socket, Ch, CPGC_MISCODTCTL_MCDDC_CTL_REG);

  return (UINT8) CpgcMiscODTCtl.Bits.mpr_train_ddr_on;
}

/**

  Check whether or not the hardware can support the current MAST with defined test size, and
  fixup the cacheline number if it is supported.

  @param[in]     TestSize                      - Test size for the current MAST
  @param[in out] NumOfCacheLine                - Pointer to the number of cacheline

  @retval TRUE                                 - The current MAST can be supported by hardware
          FALSE                                - The current MAST can not be supported by hardware

**/
BOOLEAN
EFIAPI
CheckHwSupportedForMATSAndFixupCacheLine (
  IN     UINT8 TestSize,
  IN OUT UINT8 *NumOfCacheLine
)
{
  if (NumOfCacheLine == NULL) {
    //
    // Fatal error when passing a NULL pointer in an API
    //
    RcDebugPrint (SDBG_MAX,"Error: Accessing a NULL pointer.\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_218);
    return FALSE;
  }

  if (TestSize > MRC_WDB2_LANE_NUM) {
    RcDebugPrint (SDBG_MAX,"Test is not supported by HW since test pattern width %d is greater than %d\n", TestSize, MRC_WDB2_LANE_NUM);
    return FALSE;
  }


  *NumOfCacheLine = *NumOfCacheLine / WDB2_LINES_PER_CACHELINE;

  if (*NumOfCacheLine == 0) {
    //
    // If original NumOfCacheLine is 1, then for SPR DDR5 we can use half of a cacheline to send the pattern
    // Here we need to set to 1 and the effect is lower half of the cacheline and upper half of the cacheline are exactly same
    //
    *NumOfCacheLine = 1;
  }

  if (*NumOfCacheLine > MAX_WDB2_CACHELINE) {
    return FALSE;
  } else {
    return TRUE;
  }
}