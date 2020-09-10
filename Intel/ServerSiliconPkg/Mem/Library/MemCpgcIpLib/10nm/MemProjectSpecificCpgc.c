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
#include <Library/MemCpgcIpLib.h>
#include "MemCpgcRegs.h"
#include <UncoreCommonIncludes.h>
#include <Library/SysHostPointerLib.h>
#include "CpgcChip.h"
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemDdrioIpLib.h>
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

**/
VOID
SetCpgcPatControl (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT32    ChbitMask,
  UINT8     Mux0,
  UINT8     Mux1,
  UINT8     Mux2,
  UINT32    Patbuf0,
  UINT32    Patbuf1,
  UINT32    Patbuf2
  )
{
  CPGC_PATWDBCL_MUXCTL_MCDDC_DP_STRUCT  PatWDBCLMuxCtl;
  UINT8 Ch, SubCh, ChannelLocal;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if ((ChbitMask & (1 << Ch)) == 0) {
      continue;
    }

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      ChannelLocal = Ch;
      GetChannelMapping (Ch, SubCh, &ChannelLocal);

      PatWDBCLMuxCtl.Data = MemReadPciCfgEp (Socket, ChannelLocal, CPGC_PATWDBCL_MUXCTL_MCDDC_DP_REG);
      PatWDBCLMuxCtl.Bits.mux0 = Mux0;
      PatWDBCLMuxCtl.Bits.mux1 = Mux1;
      PatWDBCLMuxCtl.Bits.mux2 = Mux2;
      MemWritePciCfgEp (Socket, ChannelLocal, CPGC_PATWDBCL_MUXCTL_MCDDC_DP_REG,    PatWDBCLMuxCtl.Data);
      MemWritePciCfgEp (Socket, ChannelLocal, CPGC_PATWDBCL_MUX0_PBRD_MCDDC_DP_REG, Patbuf0);
      MemWritePciCfgEp (Socket, ChannelLocal, CPGC_PATWDBCL_MUX1_PBRD_MCDDC_DP_REG, Patbuf1);
      MemWritePciCfgEp (Socket, ChannelLocal, CPGC_PATWDBCL_MUX2_PBRD_MCDDC_DP_REG, Patbuf2);
      MemWritePciCfgEp (Socket, ChannelLocal, CPGC_PATWDBCL_MUX0_PBWR_MCDDC_DP_REG, Patbuf0);
      MemWritePciCfgEp (Socket, ChannelLocal, CPGC_PATWDBCL_MUX1_PBWR_MCDDC_DP_REG, Patbuf1);
      MemWritePciCfgEp (Socket, ChannelLocal, CPGC_PATWDBCL_MUX2_PBWR_MCDDC_DP_REG, Patbuf2);
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
SetMprTrainModeDdr5 (
  IN        PSYSHOST    Host,
  IN        UINT8       Socket,
  IN        UINT8       Ch,
  IN        UINT8       Enable
  )
{
#ifdef DDR5_SUPPORT
  UINT8 SubCh = 0;
  UINT8 ChannelLocal = 0;

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (IsBrsPresent (Host, Socket)) {
      GetChannelMapping (Ch, SubCh, &ChannelLocal);
    }
    CPGC_MISCODTCTL_MCDDC_CTL_STRUCT   cpgcMiscODTCtl;
    cpgcMiscODTCtl.Data = MemReadPciCfgEp (Socket, ChannelLocal, CPGC_MISCODTCTL_MCDDC_CTL_REG);
    cpgcMiscODTCtl.Bits.mpr_train_ddr_on = Enable;
    MemWritePciCfgEp (Socket, ChannelLocal, CPGC_MISCODTCTL_MCDDC_CTL_REG, cpgcMiscODTCtl.Data);
  }
#endif // #ifdef DDR5_SUPPORT
}

UINT32
GetErrorResults (
  PSYSHOST   Host,
  UINT8      socket,
  UINT8      ch
  )
{
  return MemReadPciCfgEp (socket, ch, CPGC_ERR_BYTE_MCDDC_DP_REG);
}

VOID
ClearEridLfsrErr (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  )
{
  CPGC_ERID_LFSR_ERR_MC_2LM_STRUCT    errResult;

  errResult.Data = MemReadPciCfgEp (socket, ch, CPGC_ERID_LFSR_ERR_MC_2LM_REG);
  errResult.Bits.status = 0;
  errResult.Bits.count = 0;
  MemWritePciCfgEp (socket, ch, CPGC_ERID_LFSR_ERR_MC_2LM_REG, errResult.Data);
} // ClearEridLfsrErr


VOID
GetEridCombineResult (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    *eridCombinedResult
  )
{
  CPGC_ERID_LFSR_ERR_MC_2LM_STRUCT    errResult;

  errResult.Data = MemReadPciCfgEp (socket, ch, CPGC_ERID_LFSR_ERR_MC_2LM_REG);
  if ((errResult.Bits.status & 0x7F) == 0) {
    *eridCombinedResult = 1;
  } // if status
} // GetEridCombineResult10nm

VOID
SetEridLfsrErrStatus (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT32   Value
  )
{
  CPGC_ERID_LFSR_ERR_MC_2LM_STRUCT    errResult;

  errResult.Data = MemReadPciCfgEp (socket, ch, CPGC_ERID_LFSR_ERR_MC_2LM_REG);
  errResult.Bits.status = (UINT8)Value;
  MemWritePciCfgEp (socket, ch, CPGC_ERID_LFSR_ERR_MC_2LM_REG, errResult.Data);

} // SetEridLfsrErrStatus


VOID
SetupCoarseEridTraining (
                      PSYSHOST Host,
                      UINT8    socket,
                      UINT8    ch,
                      UINT8    dimm
)
{
  CPGC_DDRT_MISC_CTL_MC_2LM_STRUCT          cpgcDdrtMiscCtl;
  CPGC_ERID_LFSR_INIT_REQ0_MC_2LM_STRUCT    cpgcEridLfsrInitReq0;
  CPGC_ERID_LFSR_INIT_REQ1_MC_2LM_STRUCT    cpgcEridLfsrInitReq1;
  CPGC_ERID_LFSR_INIT_RESP0_MC_2LM_STRUCT   cpgcEridLfsrInitResp0;
  CPGC_ERID_LFSR_INIT_RESP1_MC_2LM_STRUCT   cpgcEridLfsrInitResp1;
  CPGC_ERID_LFSR_CTL_MC_2LM_STRUCT          cpgcEridLfsrCtl;
  CPGC_ERID_LFSR_ERR_MC_2LM_STRUCT          cpgcEridLfsrErr;

  cpgcDdrtMiscCtl.Data = MemReadPciCfgEp (socket, ch, CPGC_DDRT_MISC_CTL_MC_2LM_REG);
  cpgcDdrtMiscCtl.Bits.cpgc_max_credit = 0x28;  //training doc has value = 32
  cpgcDdrtMiscCtl.Bits.multi_credit_on = 1;
  cpgcDdrtMiscCtl.Bits.enable_erid_return  = 0;
  MemWritePciCfgEp (socket, ch, CPGC_DDRT_MISC_CTL_MC_2LM_REG, cpgcDdrtMiscCtl.Data);

  //ddrtCreditLimit.Data = MemReadPciCfgEp (socket, ch, DDRT_CREDIT_LIMIT_MC_2LM_REG);
  //ddrtCreditLimit.Bits.ddrt_rd_credit = 0;
  //ddrtCreditLimit.Bits.ddrt_wr_credit = 0;
  //MemWritePciCfgEp (socket, ch, DDRT_CREDIT_LIMIT_MC_2LM_REG, ddrtCreditLimit.Data);

  cpgcEridLfsrInitReq0.Data = (UINT16)MemReadPciCfgEp (socket, ch, CPGC_ERID_LFSR_INIT_REQ0_MC_2LM_REG);
  cpgcEridLfsrInitReq0.Bits.patbuffer = 0x05A5A5;
  MemWritePciCfgEp (socket, ch, CPGC_ERID_LFSR_INIT_REQ0_MC_2LM_REG, cpgcEridLfsrInitReq0.Data);
  cpgcEridLfsrInitResp0.Data = (UINT16)MemReadPciCfgEp (socket, ch, CPGC_ERID_LFSR_INIT_RESP0_MC_2LM_REG);
  cpgcEridLfsrInitResp0.Bits.patbuffer = 0x05A5A5;
  MemWritePciCfgEp (socket, ch, CPGC_ERID_LFSR_INIT_RESP0_MC_2LM_REG, cpgcEridLfsrInitResp0.Data);

  cpgcEridLfsrInitReq1.Data = (UINT16)MemReadPciCfgEp (socket, ch, CPGC_ERID_LFSR_INIT_REQ1_MC_2LM_REG);
  cpgcEridLfsrInitReq1.Bits.patbuffer = 0x0C6C6C;
  MemWritePciCfgEp (socket, ch, CPGC_ERID_LFSR_INIT_REQ1_MC_2LM_REG, cpgcEridLfsrInitReq1.Data);
  cpgcEridLfsrInitResp1.Data = (UINT16)MemReadPciCfgEp (socket, ch, CPGC_ERID_LFSR_INIT_RESP1_MC_2LM_REG);
  cpgcEridLfsrInitResp1.Bits.patbuffer = 0x0C6C6C;
  MemWritePciCfgEp (socket, ch, CPGC_ERID_LFSR_INIT_RESP1_MC_2LM_REG, cpgcEridLfsrInitResp1.Data);

  cpgcEridLfsrCtl.Data =  MemReadPciCfgEp (socket, ch, CPGC_ERID_LFSR_CTL_MC_2LM_REG);
  cpgcEridLfsrCtl.Bits.enable_erid_lfsr_training = 1;
  cpgcEridLfsrCtl.Bits.mode0 = 0;
  cpgcEridLfsrCtl.Bits.mode1 = 0;
  cpgcEridLfsrCtl.Bits.stop_on_err0 = 0;
  cpgcEridLfsrCtl.Bits.stop_on_err1 = 0;

  MemWritePciCfgEp (socket, ch, CPGC_ERID_LFSR_CTL_MC_2LM_REG, cpgcEridLfsrCtl.Data);

  cpgcEridLfsrErr.Data = MemReadPciCfgEp (socket, ch, CPGC_ERID_LFSR_ERR_MC_2LM_REG);
  cpgcEridLfsrErr.Bits.mask = 0x80;
  MemWritePciCfgEp (socket, ch, CPGC_ERID_LFSR_ERR_MC_2LM_REG, cpgcEridLfsrErr.Data);
} // SetupCoarseEridTraining10nm

VOID
DisableEridLfsrMode (
                      PSYSHOST Host,
                      UINT8    socket,
                      UINT8    ch
)
{
  CPGC_ERID_LFSR_CTL_MC_2LM_STRUCT    cpgcEridLfsrCtl;
  //
  // Disable ERID LFSR training mode
  //
  cpgcEridLfsrCtl.Data =  MemReadPciCfgEp (socket, ch, CPGC_ERID_LFSR_CTL_MC_2LM_REG);
  cpgcEridLfsrCtl.Bits.enable_erid_lfsr_training = 0;
  MemWritePciCfgEp (socket, ch, CPGC_ERID_LFSR_CTL_MC_2LM_REG, cpgcEridLfsrCtl.Data);
} // DisableEridLfsrMode10nm

VOID
SetupCpgcWdbBuffErid (
                   PSYSHOST Host,
                   UINT8    socket,
                   UINT8    ch
)
{

  UINT32             sequencerPattern[3];
  SequencerMode      sequencerMode;

  sequencerMode.Data = 0;
  sequencerMode.Bits.seqMode0 = BTBUFFER;
  sequencerMode.Bits.seqMode1 = BTBUFFER;
  sequencerMode.Bits.seqMode2 = BTBUFFER;
  sequencerMode.Bits.eccDataSourceSel = 1;

  sequencerPattern[0] = DDR_CPGC_PATBUF_MUX0;
  sequencerPattern[1] = DDR_CPGC_PATBUF_MUX1;
  sequencerPattern[2] = DDR_CPGC_PATBUF_MUX2;

  SetupPatternSequencer (Host, socket, ch, sequencerMode, sequencerPattern);

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
  struct dimmNvram                    (*dimmNvList)[MAX_DIMM];
  CPGC_MISCODTCTL_MCDDC_CTL_STRUCT    cpgcMiscODTCtl;

  dimmNvList  = GetDimmNvList(Host, socket, ch);

  cpgcMiscODTCtl.Data = MemReadPciCfgEp (socket, ch, CPGC_MISCODTCTL_MCDDC_CTL_REG);
  if (!(*dimmNvList)[dimm].DcpmmPresent) {
    cpgcMiscODTCtl.Bits.mpr_train_ddr_on = 1;
  }
  MemWritePciCfgEp (socket, ch, CPGC_MISCODTCTL_MCDDC_CTL_REG, cpgcMiscODTCtl.Data);
  executeCtlCLKTestChipStruct->cpgcMiscODTCtl.Data = cpgcMiscODTCtl.Data;
}

VOID
DisableChipChlRecEnOffsetTrainingMode(
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  PEXECUTE_CTL_CLK_TEST_CHIP_STRUCT executeCtlCLKTestChipStruct
  )
{
  CPGC_MISCODTCTL_MCDDC_CTL_STRUCT  cpgcMiscODTCtl;

  cpgcMiscODTCtl.Data = MemReadPciCfgEp (socket, ch, CPGC_MISCODTCTL_MCDDC_CTL_REG);
  cpgcMiscODTCtl.Bits.mpr_train_ddr_on = 0;
  MemWritePciCfgEp (socket, ch, CPGC_MISCODTCTL_MCDDC_CTL_REG, cpgcMiscODTCtl.Data);
  executeCtlCLKTestChipStruct->cpgcMiscODTCtl.Data = cpgcMiscODTCtl.Data;
}

/**

  Chip specific code path. differences in the sequencer setup were moved to hooks

  @param[in] Host       - Pointer to the system Host (root) structure
  @param[in] Socket     - processor socket ID
  @param[in] ChBitMask  - ch Bit mak for which test should be setup for.
  @param[in] Dimm       - Processor socket channel dimm number
  @param[in] Rank       - Processor socket channel dimm rank number

  @retval - VOID

**/
VOID
SetupChipPatternSequencer (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT32   ChBitmask,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  UINT8  Ch;
  struct dimmNvram        (*DimmNvList)[MAX_DIMM];
  CPGC_PATWDBCL_MUXCTL_MCDDC_DP_STRUCT  PatWDBCLMuxCtl;
  UINT8                                 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ( !((1 << Ch) & ChBitmask )) {
      continue;
    }

    DimmNvList = GetDimmNvList(Host, Socket, Ch);
    if (Rank >= (*DimmNvList)[Dimm].numRanks) {
      continue;
    }
    // ECC, Bit Buffer
    PatWDBCLMuxCtl.Data = 0;
    PatWDBCLMuxCtl.Bits.mux0 = BTBUFFER;
    PatWDBCLMuxCtl.Bits.mux1 = BTBUFFER;
    PatWDBCLMuxCtl.Bits.mux2 = BTBUFFER;
    PatWDBCLMuxCtl.Bits.ecc_datasrc_sel = 1;
    MemWritePciCfgEp (Socket, Ch, CPGC_PATWDBCL_MUXCTL_MCDDC_DP_REG, PatWDBCLMuxCtl.Data);

    MemWritePciCfgEp (Socket, Ch, CPGC_PATWDBCL_MUX0_PBWR_MCDDC_DP_REG, 0xAAAAAA);
    MemWritePciCfgEp (Socket, Ch, CPGC_PATWDBCL_MUX1_PBWR_MCDDC_DP_REG, 0xCCCCCC);
    MemWritePciCfgEp (Socket, Ch, CPGC_PATWDBCL_MUX2_PBWR_MCDDC_DP_REG, 0xF0F0F0);
    MemWritePciCfgEp (Socket, Ch, CPGC_PATWDBCL_MUX0_PBRD_MCDDC_DP_REG, 0xAAAAAA);
    MemWritePciCfgEp (Socket, Ch, CPGC_PATWDBCL_MUX1_PBRD_MCDDC_DP_REG, 0xCCCCCC);
    MemWritePciCfgEp (Socket, Ch, CPGC_PATWDBCL_MUX2_PBRD_MCDDC_DP_REG, 0xF0F0F0);
  } // ch loop
}

VOID
AssertChipODTOveride(
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT32 wrOdtTableData
  )
{
  CPGC_MISCODTCTL_MCDDC_CTL_STRUCT cpgcMiscODTCtl;

  cpgcMiscODTCtl.Data = MemReadPciCfgEp (socket, ch, CPGC_MISCODTCTL_MCDDC_CTL_REG);
  cpgcMiscODTCtl.Bits.odt_override  = wrOdtTableData;
  cpgcMiscODTCtl.Bits.odt_on        = wrOdtTableData;
  MemWritePciCfgEp (socket, ch, CPGC_MISCODTCTL_MCDDC_CTL_REG, cpgcMiscODTCtl.Data);
}

VOID
DeAssertChipODTOveride(
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch
  )
{
  CPGC_MISCODTCTL_MCDDC_CTL_STRUCT cpgcMiscODTCtl;

  cpgcMiscODTCtl.Data = MemReadPciCfgEp (socket, ch, CPGC_MISCODTCTL_MCDDC_CTL_REG);
  cpgcMiscODTCtl.Bits.odt_override  = 0;
  cpgcMiscODTCtl.Bits.odt_on        = 0;
  MemWritePciCfgEp (socket, ch, CPGC_MISCODTCTL_MCDDC_CTL_REG, cpgcMiscODTCtl.Data);
}

VOID
DoFinalChipSpecificWLCleanUp(
  PSYSHOST Host,
  UINT8 socket
  )
{
  UINT8 ch;
  struct channelNvram  (*channelNvList)[MAX_CH];
  UINT8                MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    MemWritePciCfgEp (socket, ch, CPGC_PATWDB_INV_MCDDC_DP_REG, 0);
    MemWritePciCfgEp (socket, ch, CPGC_PATWDB_INV0_MCDDC_DP_REG, 0);
    MemWritePciCfgEp (socket, ch, CPGC_PATWDB_INV1_MCDDC_DP_REG, 0);
  } // ch loop
}

VOID
DisableEridReturn (
            PSYSHOST Host,
            UINT8    socket
)
{
  UINT8 ch;
  struct channelNvram               (*channelNvList)[MAX_CH];
  CPGC_DDRT_MISC_CTL_MC_2LM_STRUCT  cpgcDdrtMiscCtl;
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    cpgcDdrtMiscCtl.Data = MemReadPciCfgEp (socket, ch, CPGC_DDRT_MISC_CTL_MC_2LM_REG);
    cpgcDdrtMiscCtl.Bits.enable_erid_return  = 1;
    MemWritePciCfgEp (socket, ch, CPGC_DDRT_MISC_CTL_MC_2LM_REG, cpgcDdrtMiscCtl.Data);
  }
}

/**

  Set the Mpr Training mode to filter ACT/PRE.

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

  ChannelLocal = Ch;
  for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {
    if (IsBrsPresent (Host, Socket)) {
      GetChannelMapping (Ch, SubCh, &ChannelLocal);
    }
    cpgcMiscODTCtl.Data = MemReadPciCfgEp (Socket, ChannelLocal, CPGC_MISCODTCTL_MCDDC_CTL_REG);
    cpgcMiscODTCtl.Bits.mpr_train_ddr_on = Enable;
    MemWritePciCfgEp (Socket, ChannelLocal, CPGC_MISCODTCTL_MCDDC_CTL_REG, cpgcMiscODTCtl.Data);
  }
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
  // Always support current MATS and no need to fixup cacheline number
  return TRUE;
}