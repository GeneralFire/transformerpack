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

#include <SysHostChip.h>
#include <Chip/Include/MemHostChip.h>
#include <Library/MemPorLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryCoreLib.h>

#include <Cpu/CpuIds.h>
#include <Chip/Include/FnvAccessCommon.h>
#include <ScratchpadList.h>

#include <Library/MemFmcIpLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/CheckpointLib.h>
#include <Library/EmulationConfigurationLib.h>

#include "Include/MemMcRegs.h"
#include "Include/MemMcChip.h"
#include "Include/CmiMcRegStruct.h"
#include "Include/MemMcIpLibInternal.h"

#include <Library/MemDdrioIpLib.h>
#include <Library/CteNetLib.h>

#include <Library/SystemInfoLib.h>
#include <Library/SysHostPointerLib.h>

#include "MemHostChipCommon.h"
#include <Library/KtiApi.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemThermManageLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemMcIpTargetLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/OdtTableLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/MemCallTableLib.h>
#include <Library/ProcMemInitTargetLib.h>
#include <Library/MemTypeLib.h>
#include <Memory/JedecDefinitions.h>
#include <Library/UsraCsrLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/PchAdrLib.h>
#include <CpuPciAccess.h>
#include <Library/MemRcLib.h>
#include <Library/MemSmeeIpLib.h>
#include <Library/MemRoutingLib.h>
#include <Library/MemAccessLib.h>
#include <UncoreCommonIncludes.h>
#include <Ras/RcData.h>
#include <Guid/SecurityPolicy.h>
#include <Library/RasRcDataLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/HobLib.h>
#include <Library/SpdDecodeLib.h>
#include <Library/RcMemAllocLib.h>


#define CMD_TO_DATA_DELAY                0x4 // Completion delay for DDR5, DDR4, DDRT2 and HBM with TME disabled.
#define CMD_TO_DATA_DELAY_WITH_TME       0x8 // Completion delay for DDR5, DDR4, DDRT2 and HBM with TME enabled.
#define DFMC_FUSE_OVERCLOCK              0

#define CMD_CMPL_TIME_MASK                0x3FF // CSR field is 11 bits but logic only looks at 10 (9:0)
#define PIPE_DELAY_CONST                  18    // PIPE_DELAY = (cmd_cmpl_time[9:0] / 2) + 18dclks

#define ERF_DDR4_CMD_REG_NUM              16

//
// DCLK ratios
//
extern const UINT8   DCLKs[MAX_SUP_FREQ];

//
// These tables map different DDR frequency encodings to each other and must be in acsending order
//
                       // RefClk=100MHz    Freq(MT/s) EncodedFreq  QclkRatio  PcuDmfc
DDR_FREQ_ENCODINGS  mDdrFreqEncode100[] = {{ 1000,      DDR_1000,      10,       5 },
                                           { 1200,      DDR_1200,      12,       6 },
                                           { 1400,      DDR_1400,      14,       7 },
                                           { 1600,      DDR_1600,      16,       8 },
                                           { 1800,      DDR_1800,      18,       9 },
                                           { 2000,      DDR_2000,      20,      10 },
                                           { 2200,      DDR_2200,      22,      11 },
                                           { 2400,      DDR_2400,      24,      12 },
                                           { 3200,      DDR_3200,      32,      16 },
                                           { 3600,      DDR_3600,      36,      18 },
                                           { 4000,      DDR_4000,      40,      20 },
                                           { 4400,      DDR_4400,      44,      22 },
                                           { 4800,      DDR_4800,      48,      24 }
                                          };

                       // RefClk=133MHz    Freq(MT/s) EncodedFreq  QclkRatio  PcuDmfc
DDR_FREQ_ENCODINGS  mDdrFreqEncode133[] = {{ 1333,      DDR_1333,      10,       5 },
                                           { 1600,      DDR_1600,      12,       6 },
                                           { 1866,      DDR_1866,      14,       7 },
                                           { 2133,      DDR_2133,      16,       8 },
                                           { 2400,      DDR_2400,      18,       9 },
                                           { 2666,      DDR_2666,      20,      10 },
                                           { 2933,      DDR_2933,      22,      11 },
                                           { 3200,      DDR_3200,      24,      12 }
                                          };

//                           {DDRT_CKE, PPD, APD, CKE idle timer}
STATIC const UINT8 CKE_LL0_AUTO[] = {0x80, 0, 0, 0};
STATIC const UINT8 CKE_LL1_AUTO[] = {0x60, 0, 1, 1};
STATIC const UINT8 CKE_LL2_AUTO[] = {0x30, 0, 1, 1};
STATIC const UINT8 PKGC_CKE_AUTO[] = {0x30, 0, 1, 1};

extern UINT8  lrdimmExtraOdtDelay[MAX_SUP_FREQ];

#define DDRT_RETRY_LEVEL_TRIGGER_EN  BIT9  // Described in bios_purley HSD5385403
#define T_PERSIST_DELAY_DEFAULT   0x84     // tCWL + tCWL_add + 8 + 82 nCK = 42 + 0 +8 +82 = 132 nCK
#define DDRT_WRITE_LATENCY_DELAY  0xA
#define DDRT_WRITE_LATENCY_MIN    0x1

// Enable pmon clock running for DRAM RAPL
#define ENABLE_PMON_CLOCK_EVENT   2
#define ENABLE_PMON_CLOCK_UNIT    1


/**
  Get Security policy info into system.

  @retval  EFI_SUCCESS          Get Security policy success.
  @retval  EFI_NOT_FOUND        Can't find the Security Policy.
  @retval  EFI_BUFFER_TOO_SMALL The Buffer's size is too small.
**/
SECURITY_POLICY *
EFIAPI
GetSecurityPolicy2 (
  VOID
  )
{
  VOID *GuidHob = NULL;
  VOID *GuidData = NULL;


  GuidHob = GetFirstGuidHob (&gSecurityPolicyDataGuid);
  if (GuidHob !=  NULL) {
    GuidData = GET_GUID_HOB_DATA (GuidHob);
  }

  return GuidData;
}

/**
  Build target function reset the Pmon unit ctrl counters.

  @param[in] MemTechType  Mem technology type.
  @param[in] Socket       Socket index
  @param[in] Ch           Ch index

  @retval EFI_SUCCESS     Completed successfully.
  @retval !EFI_SUCCESS    Error.
**/
EFI_STATUS
EFIAPI
ResetPmonUnitCtrlCounters (
  IN MEM_TECH_TYPE   MemTechType,
  IN UINT8           Socket,
  IN UINT8           Ch
  )

{
  PMONUNITCTRL_MCDDC_CTL_STRUCT PmonUnitCtrl;

  //
  // Reset counters
  //
  PmonUnitCtrl.Data = 0;
  PmonUnitCtrl.Bits.resetcounterconfigs = 1;
  PmonUnitCtrl.Bits.resetcounters = 1;
  ChRegisterWrite (MemTechType, Socket, Ch, PMONUNITCTRL_MCDDC_CTL_REG, PmonUnitCtrl.Data);

  return EFI_SUCCESS;
} // ResetPmonUnitCtrlCounters

/**

  Enable Pmon Clock running for DRAM RAPL

  @param[in] Socket       - Socket number

  @retval N/A

**/
VOID
EnablePmonClock (
  UINT8      Socket
  )
{
  UINT8                             Ch;
  UINT8                             MaxChDdr;
  PMONCNTRCFG_0_MCDDC_CTL_STRUCT    PmonCntrCfg0;

  if (!IsSiliconWorkaroundEnabled ("S2209684728")) {
    return;
  }

  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    PmonCntrCfg0.Data = MemReadPciCfgEp (Socket, Ch, PMONCNTRCFG_0_MCDDC_CTL_REG);
    PmonCntrCfg0.Bits.eventselect = ENABLE_PMON_CLOCK_EVENT;
    PmonCntrCfg0.Bits.unitmask = ENABLE_PMON_CLOCK_UNIT;
    MemWritePciCfgEp (Socket, Ch, PMONCNTRCFG_0_MCDDC_CTL_REG, PmonCntrCfg0.Data);
  } // Ch loop
} // SetMemDone

/**
  Increase Dcpmm Read Drain timeout to prevent system hangs

  @param [in]   Socket 0 based number

  @retval       N/a
**/
VOID
MaximizeDcpmmReadDrainTimeout (
  IN UINT8  Socket
  )
{
  RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_252);
}

/**

  Set the bit indicating memory initialization is complete

  @param Host  - Pointer to sysHost

  @retval N/A

**/
VOID
SetMemDone (
  PSYSHOST Host
  )
{
  UINT8                             socket;
  UINT8                             ch;
  UINT8                             mcId;
  struct channelNvram               (*channelNvList)[MAX_CH];

  MC_INIT_STATE_G_MC_MAIN_STRUCT    mcInitStateG;
  PMONCNTRCFG_4_MCDDC_CTL_STRUCT    pmonCntrCfg;
  PMONCNTRCFGFIXED_MCDDC_CTL_STRUCT PmonCntrCfgFxied;
  UINT8                             MaxChDdr;
  UINT8                             MaxImc;


  MaxChDdr = GetMaxChDdr ();
  MaxImc = GetMaxImc ();
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    //
    // Continue if this socket is not present
    //
    if (!SocketPresent (socket)) {
      continue;
    }

    channelNvList = GetChannelNvList(Host, socket);

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      //
      // Reset counters
      //
      ResetPmonUnitCtrlCounters (MemTechDdr, socket, ch);

      //
      // Enable error counter
      //
      pmonCntrCfg.Data = 0;
      PmonCntrCfgFxied.Data = MemReadPciCfgEp (socket, ch, PMONCNTRCFGFIXED_MCDDC_CTL_REG);
      pmonCntrCfg.Bits.eventselect = 0x9;
      pmonCntrCfg.Bits.unitmask = 0x1;
      PmonCntrCfgFxied.Bits.counterenable = 1;
      MemWritePciCfgEp (socket, ch, PMONCNTRCFG_4_MCDDC_CTL_REG, pmonCntrCfg.Data);
      MemWritePciCfgEp (socket, ch, PMONCNTRCFGFIXED_MCDDC_CTL_REG, PmonCntrCfgFxied.Data);
    } // ch loop

    //
    // Set MRC_DONE
    //
    for (mcId = 0; mcId < MaxImc; mcId++) {
      if(Host->var.mem.socket[socket].imcEnabled[mcId] == 0) {
        continue;
      }

      mcInitStateG.Data = MemReadPciCfgMC (socket, mcId, MC_INIT_STATE_G_MC_MAIN_REG);
      mcInitStateG.Bits.mrc_done  = 1;
      MemWritePciCfgMC_AllCh (socket, mcId, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG.Data);
    }

  } // socket loop
} // SetMemDone

/**

  Get CMPL program delay.

  @param[in] Socket   CPU Socket number.

  @retval             CMPL delay.

**/
UINT8
GetCmplDelayDdr (
  IN    UINT8     Socket
  )
{
  if (IsTmeEnabled (Socket)) {
    return CMD_TO_DATA_DELAY_WITH_TME;
  } else {
    return CMD_TO_DATA_DELAY;
  }
} // GetCmplDelayDdr

/**
  Determine if the given channel number is valid on current socket.

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Channel       - Channel number

  @retval   TRUE      Channel number is valid
  @retval   FALSE     Channel number is not valid
**/
BOOLEAN
EFIAPI
IsChannelValid (
  IN      PSYSHOST        Host,
  IN      UINT8           Channel
  )
{
  UINT8       Socket;
  UINT8       McId;

  Socket = Host->var.mem.currentSocket;
  McId = GetMCID (Host, Socket, Channel);

  return IsMcValid (Host, McId);
}

/**
  Determine if the given MC number is valid on current socket.

  @param[in]      Host          - Pointer to sysHost
  @param[in]      McId          - MC number

  @retval   TRUE      MC number is valid
  @retval   FALSE     MC number is not valid
**/
BOOLEAN
EFIAPI
IsMcValid (
  IN      PSYSHOST        Host,
  IN      UINT8           McId
  )
{
  UINT8       MaxImc;
  UINT8       Socket;

  MaxImc = GetMaxImc ();
  Socket = Host->var.mem.currentSocket;

  if (McId < MaxImc) {
    return Host->var.mem.socket[Socket].ImcPresent[McId];
  } else {
    return FALSE;
  }
}

/**
  Specify the hold time of DDRT Write data path before it can switch back to DDR4 mode

  @param[in]  Host      Pointer to SysHost
  @param[in]  Socket    The socket containing the timer to initialize
  @param[in]  Channel   The channel containing the timer to initialize
  @param[in]  HoldTime  Time that the Write data path be held

  @retval None

**/
VOID
InitializeDdrtWrPathHoldTime (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     HoldTime
  )
{
  // Not applicable for MC wave3
}

/**
  Specify the delay to switch to DDRT write data path

  @param[in]  Host     Pointer to SysHost
  @param[in]  Socket   The socket containing the timer to initialize
  @param[in]  Channel  The channel containing the timer to initialize
  @param[in]  Delay    Delays to switch to DDRT write data path

  @retval None

**/
VOID
InitializeDdrtWrPathDelay (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     Delay
  )
{
  // Not applicable for MC wave3
}

/*
  Program WPQ thershold to avoid deadlock.

  @param[in] Host          SysHost pointer
  @param[in] Socket        0 based socket number
  @param[in] Channel       0 based channel index
  @param[in] Value         Value to program in bit field

  @retval   EFI_SUCCESS             Value programmed successfully
  @retval   EFI_INVALID_PARAMETER   Invalid value passed
*/
EFI_STATUS
ProgramWPQThershold (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT32 Value
  )
{
  SCHEDULER_ENABLES_MCDDC_CTL_STRUCT SchedulerEnables;
  if (Value != 0 ) {
    if (Value > MAX_DEPRD_WPQ_THERSHOLD) {
      Value = MAX_DEPRD_WPQ_THERSHOLD;
    }
    SchedulerEnables.Data = MemReadPciCfgEp (Socket, Channel, SCHEDULER_ENABLES_MCDDC_CTL_REG);
    SchedulerEnables.Bits.deprd_wpq_threshold = Value;
    MemWritePciCfgEp (Socket, Channel, SCHEDULER_ENABLES_MCDDC_CTL_REG, SchedulerEnables.Data);
    return EFI_SUCCESS;
  }
  return EFI_INVALID_PARAMETER;
}

/**

  Chip specific code path to enable specific error conditions

  @param[in] Host                            - Pointer to sysHost, the system Host (root) structure
  @param[in] socket                          - Socket Id
  @param[in] ch                              - Channel number (0-based)
  @param[in] chipDisableParityCheckingStruct - Pointer to parity checking

  @retval None

**/
VOID
ChipSpecificErrorEnable(
  IN PSYSHOST Host,
  IN UINT8 socket,
  IN UINT8 ch,
  IN PCHIP_PARITY_CHECKING_STRUCT chipDisableParityCheckingStruct
  )
{
  DDR4_CA_CTL_MCDDC_DP_STRUCT               ddr4cactl;
  CMD_PAR_INJ_CTL_N0_MCDDC_CTL_STRUCT       CmdParInjCtlN0;

  ddr4cactl.Data = MemReadPciCfgEp (socket, ch, DDR4_CA_CTL_MCDDC_DP_REG);
  ddr4cactl.Bits.erf_en0 = 0;
  MemWritePciCfgEp (socket, ch, DDR4_CA_CTL_MCDDC_DP_REG, ddr4cactl.Data);

  CmdParInjCtlN0.Data = MemReadPciCfgEp (socket, ch, CMD_PAR_INJ_CTL_N0_MCDDC_CTL_REG);
  CmdParInjCtlN0.Bits.en_ddr4 = 0;
  CmdParInjCtlN0.Bits.en_ddrt_ui0 = 0;
  CmdParInjCtlN0.Bits.en_ddrt_ui1 = 0;
  MemWritePciCfgEp (socket, ch, CMD_PAR_INJ_CTL_N0_MCDDC_CTL_REG, CmdParInjCtlN0.Data);
}

/**

  Checks DDR4 DQ swizzling and programs registers to account for it

  @param[in] Host    - Pointer to sysHost
  @param[in] socket  - Socket number

**/
VOID
CheckAndProgramDqSwizzleDdr4 (
  IN PSYSHOST  Host,
  IN UINT8     socket
  )
{
  UINT8                                ch;
  UINT8                                dimm;
  UINT32                               crcDqSwz[5];
  UINT32                               crcDqSwzeven[5];
  UINT8                                i;
  UINT8                                bit0;
  UINT8                                bit1;
  UINT8                                temp;
  UINT32                               enable_nibble_swizzle;
  DDR4_WRCRC_RANK_CTL_MCDDC_DP_STRUCT  WrCrcSwlRankCtl;
  DDR4_WRCRC0_CTL_N0_MCDDC_DP_STRUCT   wrcrc0;
  DDR4_WRCRC1_CTL_N0_MCDDC_DP_STRUCT   wrcrc1;
  DDR4_WRCRC2_CTL_N0_MCDDC_DP_STRUCT   wrcrc2;
  DDR4_WRCRC3_CTL_N0_MCDDC_DP_STRUCT   wrcrc3;
  struct channelNvram                  (*channelNvList)[MAX_CH];
  struct dimmNvram                     (*dimmNvList)[MAX_DIMM];
  UINT8                                CsrTableIndex;
  STATIC UINT32                        Wrcrc0Address[] = { DDR4_WRCRC0_DQ15_DQ0_MCDDC_DP_REG,
                                                           DDR4_WRCRC0_DQ31_DQ16_MCDDC_DP_REG,
                                                           DDR4_WRCRC0_DQ47_DQ32_MCDDC_DP_REG,
                                                           DDR4_WRCRC0_DQ63_DQ48_MCDDC_DP_REG,
                                                           DDR4_WRCRC0_CTL_N0_MCDDC_DP_REG
  };
  STATIC UINT32                        Wrcrc1Address[] = { DDR4_WRCRC1_DQ15_DQ0_MCDDC_DP_REG,
                                                           DDR4_WRCRC1_DQ31_DQ16_MCDDC_DP_REG,
                                                           DDR4_WRCRC1_DQ47_DQ32_MCDDC_DP_REG,
                                                           DDR4_WRCRC1_DQ63_DQ48_MCDDC_DP_REG,
                                                           DDR4_WRCRC1_CTL_N0_MCDDC_DP_REG
  };
  STATIC UINT32                        Wrcrc2Address[] = { DDR4_WRCRC2_DQ15_DQ0_MCDDC_DP_REG,
                                                           DDR4_WRCRC2_DQ31_DQ16_MCDDC_DP_REG,
                                                           DDR4_WRCRC2_DQ47_DQ32_MCDDC_DP_REG,
                                                           DDR4_WRCRC2_DQ63_DQ48_MCDDC_DP_REG,
                                                           DDR4_WRCRC2_CTL_N0_MCDDC_DP_REG
  };
  STATIC UINT32                        Wrcrc3Address[] = { DDR4_WRCRC3_DQ15_DQ0_MCDDC_DP_REG,
                                                           DDR4_WRCRC3_DQ31_DQ16_MCDDC_DP_REG,
                                                           DDR4_WRCRC3_DQ47_DQ32_MCDDC_DP_REG,
                                                           DDR4_WRCRC3_DQ63_DQ48_MCDDC_DP_REG,
                                                           DDR4_WRCRC3_CTL_N0_MCDDC_DP_REG
  };
  UINT8                                MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      dimmNvList = GetDimmNvList(Host, socket, ch);
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      if ((*dimmNvList)[dimm].DcpmmPresent) {
        continue;
      }
      // Program results
      WrCrcSwlRankCtl.Data = MemReadPciCfgEp (socket, ch, DDR4_WRCRC_RANK_CTL_MCDDC_DP_REG);
      WrCrcSwlRankCtl.Bits.rank0 = 0;
      WrCrcSwlRankCtl.Bits.rank1 = 1;
      WrCrcSwlRankCtl.Bits.rank2 = 0;
      WrCrcSwlRankCtl.Bits.rank3 = 1;
      WrCrcSwlRankCtl.Bits.rank4 = 2;
      WrCrcSwlRankCtl.Bits.rank5 = 3;
      WrCrcSwlRankCtl.Bits.rank6 = 2;
      WrCrcSwlRankCtl.Bits.rank7 = 3;
      MemWritePciCfgEp (socket, ch, DDR4_WRCRC_RANK_CTL_MCDDC_DP_REG, WrCrcSwlRankCtl.Data);

      for (i = 0; i < 4; i++) {
        crcDqSwz[i] = 0;
        crcDqSwzeven[i] = 0;
      }
      for (i = 0; i < 64; i++) {
        crcDqSwz[i/16] |= (((*dimmNvList)[dimm].dqSwz[i/2] >> ((i % 2)*4)) & 3) << ((i % 16) * 2);
      }
      //special mapping for bits 63-72, plus enable bits
      //set the enable bits for this swizzle map
      enable_nibble_swizzle = 0;
      for (i = 0; i < 72; i=i+8) {
        bit0 = ((*dimmNvList)[dimm].dqSwz[i / 2] >> ((i % 2) * 4)) & 7;
        if (bit0 > 3) {
          enable_nibble_swizzle |= (1 << (i/8));
        }
      }
      if (dimm == 0) {
        wrcrc0.Data =  MemReadPciCfgEp (socket, ch, DDR4_WRCRC0_CTL_N0_MCDDC_DP_REG);
        wrcrc1.Data = MemReadPciCfgEp (socket, ch, DDR4_WRCRC1_CTL_N0_MCDDC_DP_REG);
        wrcrc0.Bits.enable_nibble_swizzle = enable_nibble_swizzle;
        wrcrc1.Bits.enable_nibble_swizzle = enable_nibble_swizzle;
        MemWritePciCfgEp (socket, ch, DDR4_WRCRC0_CTL_N0_MCDDC_DP_REG, wrcrc0.Data);
        MemWritePciCfgEp (socket, ch, DDR4_WRCRC1_CTL_N0_MCDDC_DP_REG, wrcrc1.Data);
      } else {
        wrcrc2.Data = MemReadPciCfgEp (socket, ch, DDR4_WRCRC2_CTL_N0_MCDDC_DP_REG);
        wrcrc3.Data = MemReadPciCfgEp (socket, ch, DDR4_WRCRC3_CTL_N0_MCDDC_DP_REG);
        wrcrc2.Bits.enable_nibble_swizzle = enable_nibble_swizzle;
        wrcrc3.Bits.enable_nibble_swizzle = enable_nibble_swizzle;
        MemWritePciCfgEp (socket, ch, DDR4_WRCRC2_CTL_N0_MCDDC_DP_REG, wrcrc2.Data);
        MemWritePciCfgEp (socket, ch, DDR4_WRCRC3_CTL_N0_MCDDC_DP_REG, wrcrc3.Data);
      }
      if (dimm == 0) {
        crcDqSwz[4] = (UINT16)MemReadPciCfgEp (socket, ch, DDR4_WRCRC0_CTL_N0_MCDDC_DP_REG);
        crcDqSwzeven[4] = (UINT16)MemReadPciCfgEp (socket, ch, DDR4_WRCRC1_CTL_N0_MCDDC_DP_REG);
      } else {
        crcDqSwz[4] = (UINT16)MemReadPciCfgEp (socket, ch, DDR4_WRCRC2_CTL_N0_MCDDC_DP_REG);
        crcDqSwzeven[4] = (UINT16)MemReadPciCfgEp (socket, ch, DDR4_WRCRC3_CTL_N0_MCDDC_DP_REG);
      }
      for (i = 64; i < 72; i++) {
        crcDqSwz[i/16] |= (((*dimmNvList)[dimm].dqSwz[i/2] >> ((i % 2)*4)) & 3) << (((i+8) % 16) * 2);
      }
      for (i = 0; i < 64; i=i+2) {
        bit0 = ((crcDqSwz[i / 16] >> ((i % 16) * 2)) & 3);
        bit1 = ((crcDqSwz[(i + 1) / 16] >> (((i + 1) % 16) * 2)) & 3);
        temp = bit1;
        bit1 = bit0;
        bit0 = temp;
        crcDqSwzeven[i / 16] |= bit0 << ((i % 16) * 2);
        crcDqSwzeven[(i +1) / 16] |= bit1 << (((i + 1) % 16) * 2);
      }

      for (i = 64; i < 72; i=i+2) {
        bit0 = ((crcDqSwz[i / 16] >> (((i + 8) % 16) * 2)) & 3);
        bit1 = ((crcDqSwz[(i + 1) / 16] >> (((i + 9) % 16) * 2)) & 3);
        temp = bit1;
        bit1 = bit0;
        bit0 = temp;
        crcDqSwzeven[i / 16] |= bit0 << (((i + 8) % 16) * 2);
        crcDqSwzeven[(i + 1) / 16] |= bit1 << (((i + 9) % 16) * 2);
      }

      for (CsrTableIndex = 0; CsrTableIndex < (sizeof (Wrcrc0Address) / sizeof (UINT32)); CsrTableIndex++) {
        if (dimm == 0) {
          MemWritePciCfgEp (socket, ch, Wrcrc0Address[CsrTableIndex], crcDqSwz[CsrTableIndex]);
          MemWritePciCfgEp (socket, ch, Wrcrc1Address[CsrTableIndex], crcDqSwzeven[CsrTableIndex]);
        } else {
          MemWritePciCfgEp (socket, ch, Wrcrc2Address[CsrTableIndex], crcDqSwz[CsrTableIndex]);
          MemWritePciCfgEp (socket, ch, Wrcrc3Address[CsrTableIndex], crcDqSwzeven[CsrTableIndex]);
        } // dimm == 1
      } // CsrTableIndex loop
    } // dimm loop
  } // ch loop
} // CheckAndProgramDqSwizzleDdr4

/**

  Checks DDR5 DQ swizzling and programs registers to account for it

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number

**/
VOID
CheckAndProgramDqSwizzleDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  UINT8                                Ch;
  UINT8                                SubCh;
  UINT8                                Rank;
  UINT8                                Dimm;
  UINT32                               CrcDqSwzOdd[3];
  UINT32                               CrcDqSwzeven[3];
  UINT8                                i;
  DDR4_WRCRC_RANK_CTL_MCDDC_DP_STRUCT  WrCrcSwlRankCtl;
  DDR4_WRCRC0_CTL_N0_MCDDC_DP_STRUCT   Wrcrc;
  struct channelNvram                  (*ChannelNvList)[MAX_CH];
  struct dimmNvram                     (*DimmNvList)[MAX_DIMM];
  struct ddrRank                       (*RankList) [MAX_RANK_DIMM];
  UINT8                                CsrTableIndex;
  UINT8                                MBV;
  UINT8                                MaxChDdr;
  UINT32                               CsrRegister;
  UINT32                               EvenCsrRegister;
  UINT32                               OddCsrRegister;
  UINT8                                CurrentBitPosition;
  UINT32                               CurrentBitValue;
  CONST UINT32                         EnableNibbleSwizzle[MAX_DIMM][2] = {
    {DDR4_WRCRC0_CTL_N0_MCDDC_DP_REG, DDR4_WRCRC1_CTL_N0_MCDDC_DP_REG},
    {DDR4_WRCRC2_CTL_N0_MCDDC_DP_REG, DDR4_WRCRC3_CTL_N0_MCDDC_DP_REG}
  };
  CONST UINT32                         EvenWrcrcAddress[MAX_DIMM][2][3] = {
    {{DDR4_WRCRC0_DQ15_DQ0_MCDDC_DP_REG, DDR4_WRCRC0_DQ31_DQ16_MCDDC_DP_REG, DDR4_WRCRC0_CTL_N0_MCDDC_DP_REG},
     {DDR4_WRCRC1_DQ15_DQ0_MCDDC_DP_REG, DDR4_WRCRC1_DQ31_DQ16_MCDDC_DP_REG, DDR4_WRCRC1_CTL_N0_MCDDC_DP_REG}},
    {{DDR4_WRCRC2_DQ15_DQ0_MCDDC_DP_REG, DDR4_WRCRC2_DQ31_DQ16_MCDDC_DP_REG, DDR4_WRCRC2_CTL_N0_MCDDC_DP_REG},
     {DDR4_WRCRC3_DQ15_DQ0_MCDDC_DP_REG, DDR4_WRCRC3_DQ31_DQ16_MCDDC_DP_REG, DDR4_WRCRC3_CTL_N0_MCDDC_DP_REG}}
  };
  CONST UINT32                         OddWrcrcAddress[MAX_DIMM][2][3] = {
    {{DDR4_WRCRC0_DQ47_DQ32_MCDDC_DP_REG, DDR4_WRCRC0_DQ63_DQ48_MCDDC_DP_REG, DDR4_WRCRC0_CTL_N1_MCDDC_DP_REG},
     {DDR4_WRCRC1_DQ47_DQ32_MCDDC_DP_REG, DDR4_WRCRC1_DQ63_DQ48_MCDDC_DP_REG, DDR4_WRCRC1_CTL_N1_MCDDC_DP_REG}},
    {{DDR4_WRCRC2_DQ47_DQ32_MCDDC_DP_REG, DDR4_WRCRC2_DQ63_DQ48_MCDDC_DP_REG, DDR4_WRCRC2_CTL_N1_MCDDC_DP_REG},
     {DDR4_WRCRC3_DQ47_DQ32_MCDDC_DP_REG, DDR4_WRCRC3_DQ63_DQ48_MCDDC_DP_REG, DDR4_WRCRC3_CTL_N1_MCDDC_DP_REG}}
  };

  MaxChDdr      = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList(Host, Socket);
  MBV = GetMaxBitsValid(Host) / 2; //Maximum bits per sub channel.

  if (Host->nvram.mem.socket[Socket].TrainingStepDone.DqSwzDone == 0) {
    return;
  }

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled(Socket, Ch) == FALSE) {
      continue;
    }
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        DimmNvList = GetDimmNvList(Host, Socket, Ch);
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
          continue;
        }
        if (IsDcpmmPresentDimm (Socket, Ch, Dimm) == TRUE) {
          continue;
        }
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (Rank >= (*DimmNvList) [Dimm].numRanks) {
          continue;
        }

        for (i = 0; i < 3; i++) {
          CrcDqSwzOdd[i] = 0;
          CrcDqSwzeven[i] = 0;
        }

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          //
          // Program results
          //
          WrCrcSwlRankCtl.Data = MemReadPciCfgEp (Socket, Ch, DDR4_WRCRC_RANK_CTL_MCDDC_DP_REG);
          WrCrcSwlRankCtl.Bits.rank0 = 0;
          WrCrcSwlRankCtl.Bits.rank1 = 0;
          WrCrcSwlRankCtl.Bits.rank2 = 1;
          WrCrcSwlRankCtl.Bits.rank3 = 1;
          WrCrcSwlRankCtl.Bits.rank4 = 2;
          WrCrcSwlRankCtl.Bits.rank5 = 2;
          WrCrcSwlRankCtl.Bits.rank6 = 3;
          WrCrcSwlRankCtl.Bits.rank7 = 3;
          MemWritePciCfgEp (Socket, Ch, DDR4_WRCRC_RANK_CTL_MCDDC_DP_REG, WrCrcSwlRankCtl.Data);
          WrCrcSwlRankCtl.Data = MemReadPciCfgEp (Socket, Ch, DDR4_WRCRC_RANK_CTL_MCDDC_DP_REG);
          RcDebugPrint(
            SDBG_MAX,
            "DDR4_WRCRC_RANK_CTL_MCDDC_DP_REG = 0x%08x \n",
            WrCrcSwlRankCtl.Data
          );

          CsrRegister = EnableNibbleSwizzle[Dimm][Rank % ((*DimmNvList) [Dimm].numRanks)];
          Wrcrc.Data = MemReadPciCfgEp (Socket, Ch, CsrRegister);
          Wrcrc.Bits.enable_nibble_swizzle = 0;
          MemWritePciCfgEp (Socket, Ch, CsrRegister, Wrcrc.Data);

          for (i = 0; i < MBV; i++) {
            //
            // DqSwzDdr5 has 20 locations per Dimm and Rank (20 is not related to MAX_STROBES),
            // each memory location is capable to store the result of 2 bits
            // DqSwzDdr5[Rank][SubCh][0] = {Bit1,Bit0}
            // DqSwzDdr5[Rank][SubCh][1] = {Bit3,Bit2}
            // ...
            // DqSwzDdr5[Rank][SubCh][18] = {Bit37,Bit36}
            // DqSwzDdr5[Rank][SubCh][19] = {Bit39,Bit38}
            //
            // CurrentBitPosition - represents the location in the array considering a nibble granularity
            // CurrentBitPosition = 0 -> means Nibble 0 of DqSwzDdr5[Rank][SubCh][0]
            // CurrentBitPosition = 1 -> means Nibble 1 of DqSwzDdr5[Rank][SubCh][0]
            // ...
            // CurrentBitPosition = 38 -> means Nibble 0 of DqSwzDdr5[Rank][SubCh][19]
            // CurrentBitPosition = 39 -> means Nibble 1 of DqSwzDdr5[Rank][SubCh][19]
            //
            CurrentBitPosition = i % BITS_PER_NIBBLE + (i / BITS_PER_NIBBLE) * BITS_PER_NIBBLE;
            CurrentBitValue = (((*RankList) [Rank].DqSwzDdr5[SubCh][CurrentBitPosition / 2] >> ((CurrentBitPosition % 2) * 4)) & 3) << ((CurrentBitPosition % 16) * 2);
            if (SubCh == 0) {
              //
              // for SubCh 0 the xx_swizzle_ctl register field changed its offset position
              //
              if (i >= 32) {
                CrcDqSwzeven[i / 16] |= CurrentBitValue << 16;
              } else {
                CrcDqSwzeven[i / 16] |= CurrentBitValue;
              }
            } else {
              CrcDqSwzOdd[i / 16] |= CurrentBitValue;
            }
          } // i loop ...
        } // SubCh loop

        for (CsrTableIndex = 0; CsrTableIndex < 3; CsrTableIndex ++) {
          EvenCsrRegister = EvenWrcrcAddress [Dimm][Rank % ((*DimmNvList) [Dimm].numRanks)][CsrTableIndex];
          OddCsrRegister  = OddWrcrcAddress [Dimm][Rank % ((*DimmNvList) [Dimm].numRanks)][CsrTableIndex];
          MemWritePciCfgEp (Socket, Ch, EvenCsrRegister, CrcDqSwzeven[CsrTableIndex]);
          MemWritePciCfgEp (Socket, Ch, OddCsrRegister, CrcDqSwzOdd[CsrTableIndex]);
        } // CsrTableIndex loop

      } //dimm loop
    } // Rank loop
  } // ch loop
} // CheckAndProgramDqSwizzleDdr5

/**

  Checks DQ swizzling and programs registers to account for it

  @param[in] Host    - Pointer to sysHost
  @param[in] socket  - Socket number

**/
VOID
CheckAndProgramDqSwizzle10nm (
  IN PSYSHOST  Host,
  IN UINT8     socket
  )
{
   if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
     CheckAndProgramDqSwizzleDdr5(Host, socket);
   } else {
     CheckAndProgramDqSwizzleDdr4(Host, socket);
   }
}

/**

  Enable or disable CADB

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket number
  @param[in] ChBitmask   - Channel bit mask
  @param[in] Enable      - TRUE - enable CADB; FALSE- disable CADB

  @retval None

**/
VOID
SetCadbModeEn (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT32    ChBitmask,
  IN    BOOLEAN   Enable
  )
{
  MC_CADB_CTL_MCDDC_CTL_STRUCT           McCadbCtl;
  UINT8                                  Ch;

  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if (ChBitmask & (1 << Ch)) {
      McCadbCtl.Data = MemReadPciCfgEp (Socket, Ch, MC_CADB_CTL_MCDDC_CTL_REG);
      McCadbCtl.Bits.cadb_mode_en = Enable;
      MemWritePciCfgEp (Socket, Ch, MC_CADB_CTL_MCDDC_CTL_REG, McCadbCtl.Data);
    }
  }
}

/**
  Return a pointer to the DDR frequency encoding table

  @param[in]    Host        - Pointer to sysHost structure
  @param[in]    Socket      - Socket number
  @param[out]   TablePtr    - A pointer to the caller's table pointer
  @param[out]   TableSize   - Pointer to the caller's table size parameter

  @retval SUCCESS
**/
UINT32
GetDdrFreqEncodingTable (
  IN        PSYSHOST            Host,
  IN        UINT8               Socket,
  OUT       DDR_FREQ_ENCODINGS  **TablePtr,
  OUT       UINT8               *TableSize
  )
{

  if (GetBclkFreq (Socket) == IMC_BCLK_100) {
    *TableSize = sizeof (mDdrFreqEncode100) / sizeof (mDdrFreqEncode100[0]);
    *TablePtr = mDdrFreqEncode100;
  } else {
    *TableSize = sizeof (mDdrFreqEncode133) / sizeof (mDdrFreqEncode133[0]);
    *TablePtr = mDdrFreqEncode133;
  }
  return SUCCESS;
}

/**
  Return the maximum DDR frequency supported by the processor

  @param[in]  Host    - Pointer to sysHost structure
  @param[in]  Socket  - Socket number

  @retval Maximum DDR frequency supported by the processor encoded as DDR_1600, DDR_1866, etc.
**/
UINT8
GetMaxCpuDdrFreq (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
  UINT8                  MaxDdrFreq;
  UINT8                  TableSize;
  UINT8                  Index;
  DDR_FREQ_ENCODINGS     *TablePtr;
  CAPID1_PCU_FUN3_STRUCT Capid1;

#ifdef DDR5_SUPPORT
  if (IsDdr5Present (Host, Socket)) {
    MaxDdrFreq = DDR_3200;
  } else
#endif
  {
    MaxDdrFreq = DDR_2400;
  }

  if (IsHvmModeEnable ()) {
    return MaxDdrFreq;
  }

  Capid1.Data = UsraCsrRead (Socket, PCU_INSTANCE_0, CAPID1_PCU_FUN3_REG);
  if (Capid1.Bits.dmfc == DFMC_FUSE_OVERCLOCK) {
    Host->var.mem.socket[Socket].maxFreq = DDR_OVERCLOCK_FREQ;
    MaxDdrFreq = DDR_OVERCLOCK_FREQ;
  } else {
    //
    // Convert fuse value into a table index
    //
    GetDdrFreqEncodingTable (Host, Socket, &TablePtr, &TableSize);
    for (Index = 0; Index < TableSize; Index++) {
      if (Capid1.Bits.dmfc == TablePtr[Index].PcuDmfc) {
        //
        // for 10nm get the maxfreq from dmfc
        //
        MaxDdrFreq = TablePtr[Index].FreqIndex;
        Host->var.mem.socket[Socket].maxFreq = MaxDdrFreq;
        break;
      }
    }

    if (Index == TableSize) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Could not find valid freq ratio: Capping frequency at %d\n", MaxDdrFreq);
    } else {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "dmfc: %d Clk Type: %d max ratio: %d Expected max ratio: %d\n",
        Capid1.Bits.dmfc, Host->var.mem.socket[Socket].ddrClkType, Host->var.mem.socket[Socket].maxFreq, TablePtr[Index].QclkRatio);
    }
  }

  return MaxDdrFreq;
} //GetMaxCpuDdrFreq

/**
  Get t_ddrt_twl for SPR

  @param[in]  Host    - Pointer to sysHost structure
  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number

  @retval cwl latency
**/
UINT8
GetDdrtTwl(PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch)
{
  UINT8 CWLTemp;

  GetDdrtCasLatencies(Socket, Ch, 0, NULL, NULL, &CWLTemp, NULL);

  return (CWLTemp);
}

/**
  Programs settings for DDRT2

  @param[in]  Host    - Pointer to sysHost structure
  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number

  @retval void
**/
VOID
EarlyDdrt2Config (
  PSYSHOST                  Host,
  UINT8                     Socket,
  UINT8                     Ch
  )
{
  UINT8                                EarlyCompletionsEnabled;
  DDRT_CONFIG_CTL_MC_2LM_STRUCT        DdrtConfigControl;
  DDRT_DATAPATH_DELAY_MCDDC_DP_STRUCT  DataPathDelay;
  DDRT_STARVE_CTL2_MC_2LM_STRUCT       StarveCtl2;
  DDRT_MISC_CTL_MC_2LM_STRUCT          DdrtMiscCtl;

  DdrtConfigControl.Data = MemReadPciCfgEp (Socket, Ch, DDRT_CONFIG_CTL_MC_2LM_REG);
  DdrtConfigControl.Bits.primary_subchn = 0;
  DdrtConfigControl.Bits.single_subchn_mode = 0;
  DdrtConfigControl.Bits.ddrt2_mode = 1;
  MemWritePciCfgEp (Socket, Ch, DDRT_CONFIG_CTL_MC_2LM_REG, DdrtConfigControl.Data);

  //
  // Disable early read completion for systems with DDRT2 DIMM
  //
  EarlyCompletionsEnabled = 0;
  if (IsSiliconWorkaroundEnabled ("S14010448801")) {
    GetSetM2mAkEarlyCompletionsEnabled (Socket, GetMCID (Host, Socket, Ch), GSM_FORCE_WRITE, &EarlyCompletionsEnabled);
  }

  //
  // Enable multi credit when the DDRT schedulers need to handle multple outstanding transactions
  //
  SetCpgcMultiCreditOn (Host, Socket, Ch, TRUE);

  //
  // Programs enable_rid_underfill for DDRT
  //
  DataPathDelay.Data = MemReadPciCfgEp (Socket, Ch, DDRT_DATAPATH_DELAY_MCDDC_DP_REG);
  DataPathDelay.Bits.enable_rid_underfill = 0;
  MemWritePciCfgEp (Socket, Ch, DDRT_DATAPATH_DELAY_MCDDC_DP_REG, DataPathDelay.Data);
  //
  // This field must be programmed to the same value as reg_ddrt_datapath_delay.enable_rid_underfill
  //
  StarveCtl2.Data = MemReadPciCfgEp (Socket, Ch, DDRT_STARVE_CTL2_MC_2LM_REG);
  StarveCtl2.Bits.enable_rid_underfill = DataPathDelay.Bits.enable_rid_underfill;
  MemWritePciCfgEp (Socket, Ch, DDRT_STARVE_CTL2_MC_2LM_REG, StarveCtl2.Data);

  DdrtMiscCtl.Data = MemReadPciCfgEp (Socket, Ch, DDRT_MISC_CTL_MC_2LM_REG);
  DdrtMiscCtl.Bits.dis_ddrt_opp_rd = 1;
  DdrtMiscCtl.Bits.dis_ddrt_opp_ufill_rd = 1;
  MemWritePciCfgEp (Socket, Ch, DDRT_MISC_CTL_MC_2LM_REG, DdrtMiscCtl.Data);
}

/**
  Programs DDRT2 MC settings

  @param[in]  Host    - Pointer to sysHost structure
  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number

  @retval void
**/
VOID
LateDdrt2Config (
  PSYSHOST                  Host,
  UINT8                     Socket,
  UINT8                     Ch
  )
{
  CMPL_TO_DATA_DELAY_MCDDC_DP_STRUCT        CmplDataDelay;
  DDRT_DATAPATH_DELAY_MCDDC_DP_STRUCT       DdrtDatapathDelay;
  T_DDRT_MISC_DELAY_MC_2LM_STRUCT           DdrtMiscDelay;
  T_DDRT_GNT2GNT_CNSTRNT_MC_2LM_STRUCT      DdrtGnt2gntCnstrnt;
  T_PERSIST_TIMING_MC_2LM_STRUCT            PersistTiming;
  MCMNT_CHKN_BIT_MCDDC_CTL_STRUCT           McmntChknBit;
  DDRT_STARVE_CTL2_MC_2LM_STRUCT            StarveCtl2;
  DDRT_MISC_CTL_MC_2LM_STRUCT               DdrtMiscCtl;

  CmplDataDelay.Data = MemReadPciCfgEp (Socket, Ch, CMPL_TO_DATA_DELAY_MCDDC_DP_REG);
  CmplDataDelay.Bits.cmpl_program_delay = GetCmplDelayDdr(Socket);
  MemWritePciCfgEp (Socket, Ch, CMPL_TO_DATA_DELAY_MCDDC_DP_REG, CmplDataDelay.Data);

  DdrtDatapathDelay.Data = MemReadPciCfgEp (Socket, Ch, DDRT_DATAPATH_DELAY_MCDDC_DP_REG);
  DdrtDatapathDelay.Bits.erid_to_rdvalid_delay = CmplDataDelay.Bits.cmpl_program_delay - 4;
  MemWritePciCfgEp (Socket, Ch, DDRT_DATAPATH_DELAY_MCDDC_DP_REG, DdrtDatapathDelay.Data);

  DdrtMiscDelay.Data = MemReadPciCfgEp (Socket, Ch, T_DDRT_MISC_DELAY_MC_2LM_REG);
  DdrtMiscDelay.Bits.wpq_rid_to_rt_ufill = CmplDataDelay.Bits.cmpl_program_delay - 4;
  MemWritePciCfgEp (Socket, Ch, T_DDRT_MISC_DELAY_MC_2LM_REG, DdrtMiscDelay.Data);

  DdrtGnt2gntCnstrnt.Data = MemReadPciCfgEp (Socket, Ch, T_DDRT_GNT2GNT_CNSTRNT_MC_2LM_REG);
  DdrtGnt2gntCnstrnt.Bits.t_ddrt_gntgnt_s            = 4;
  DdrtGnt2gntCnstrnt.Bits.t_ddrt_low_vc_same_subchn  = 4;
  DdrtGnt2gntCnstrnt.Bits.t_ddrt_low_vc_other_subchn = 8;
  MemWritePciCfgEp (Socket, Ch, T_DDRT_GNT2GNT_CNSTRNT_MC_2LM_REG, DdrtGnt2gntCnstrnt.Data);

  PersistTiming.Data = MemReadPciCfgEp (Socket, Ch, T_PERSIST_TIMING_MC_2LM_REG);
#ifndef GNRSRF_HOST
  PersistTiming.Bits.t_gnt_dq = 4;
#endif // !GNRSRF_HOST
  MemWritePciCfgEp (Socket, Ch, T_PERSIST_TIMING_MC_2LM_REG, PersistTiming.Data);

  McmntChknBit.Data = MemReadPciCfgEp (Socket, Ch, MCMNT_CHKN_BIT_MCDDC_CTL_REG);
  McmntChknBit.Bits.dissrclkstop = 1;
  McmntChknBit.Bits.dis_srx_ex_zq = 0;
  MemWritePciCfgEp (Socket, Ch, MCMNT_CHKN_BIT_MCDDC_CTL_REG, McmntChknBit.Data);

  //
  // Programs enable_rid_underfill for DDRT in Normal Mode
  //
  DdrtDatapathDelay.Data = MemReadPciCfgEp (Socket, Ch, DDRT_DATAPATH_DELAY_MCDDC_DP_REG);
  DdrtDatapathDelay.Bits.enable_rid_underfill = 1;
  MemWritePciCfgEp (Socket, Ch, DDRT_DATAPATH_DELAY_MCDDC_DP_REG, DdrtDatapathDelay.Data);
  //
  // This field must be programmed to the same value as reg_ddrt_datapath_delay.enable_rid_underfill
  //
  StarveCtl2.Data = MemReadPciCfgEp (Socket, Ch, DDRT_STARVE_CTL2_MC_2LM_REG);
  StarveCtl2.Bits.enable_rid_underfill = DdrtDatapathDelay.Bits.enable_rid_underfill;
  MemWritePciCfgEp (Socket, Ch, DDRT_STARVE_CTL2_MC_2LM_REG, StarveCtl2.Data);

  DdrtMiscCtl.Data = MemReadPciCfgEp (Socket, Ch, DDRT_MISC_CTL_MC_2LM_REG);
  DdrtMiscCtl.Bits.dis_ddrt_opp_rd = 0;
  DdrtMiscCtl.Bits.dis_ddrt_opp_ufill_rd = 0;
  MemWritePciCfgEp (Socket, Ch, DDRT_MISC_CTL_MC_2LM_REG, DdrtMiscCtl.Data);
}

/**
  Programs tstab for DDRT2

  @param[in]  Host    - Pointer to sysHost structure
  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number
  @param[in]  value   - tstab value;

  @retval void
**/
VOID
SprTstabDdrt2 (
  PSYSHOST                  Host,
  UINT8                     Socket,
  UINT8                     Ch,
  UINT32                    value
  )
{
  MEMORY_TIMINGS_ADJ2_MCDDC_CTL_STRUCT  MemoryTimingsAdj2;
  UINT32                                Trx2srx;
  struct channelNvram                   (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(Host, Socket);

  if ((*channelNvList)[Ch].ddrtEnabled == 0) {
    return;
  }
  //
  // tsrx2srx 10ns or 16Dclk
  //
  Trx2srx = (100000 + (GettCK(Host, Socket) - 100)) / GettCK(Host, Socket);

  if (Trx2srx < 16) {
    Trx2srx = 16;
  }

  MemoryTimingsAdj2.Data = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_ADJ2_MCDDC_CTL_REG);
  MemoryTimingsAdj2.Bits.tstab = value;
  MemoryTimingsAdj2.Bits.tsrx2srx = Trx2srx;
  MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_ADJ2_MCDDC_CTL_REG, MemoryTimingsAdj2.Data);


}

/**
  Programs Tcpded, Tcssr, and Tcpded2srx for DDRT2

  @param[in]  Host    - Pointer to sysHost structure
  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number

  @retval void
**/
VOID
SprTimingsDdrt2 (
  PSYSHOST                  Host,
  UINT8                     Socket,
  UINT8                     Ch
  )
{
  SREF_STATIC2_MCDDC_CTL_STRUCT               srefStatic2;
  MEMORY_TIMINGS_ADJ_MCDDC_CTL_STRUCT         MemoryTimingsAdj;
  UINT32                                      Tcpded;
  UINT32                                      Tcssr;
  UINT32                                      Tcped2srx;
  struct channelNvram                         (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(Host, Socket);

  if ((*channelNvList)[Ch].ddrtEnabled == 0) {
    return;
  }
  //
  // Tcpded 5ns or 8Dclk
  //
  Tcpded = (50000 + (GettCK(Host, Socket) - 100)) / GettCK(Host, Socket);

  if (Tcpded < 8) {
    Tcpded = 8;
  }

  //
  // Tcssr 15ns or 24Dclk
  //
  Tcssr = (150000 + (GettCK(Host, Socket) - 100)) / GettCK(Host, Socket);

  if (Tcssr < 24) {
    Tcssr = 24;
  }

  //
  // Tcped2srx 10ns or 16Dclk
  //
  Tcped2srx = (100000 + (GettCK(Host, Socket) - 100)) / GettCK(Host, Socket);

  if (Tcped2srx < 16) {
    Tcped2srx = 16;
  }

  srefStatic2.Data = MemReadPciCfgEp (Socket, Ch, SREF_STATIC2_MCDDC_CTL_REG);
  //
  // The value programmed should be tCPDED-2
  //
  srefStatic2.Bits.tcpded = (Tcpded - 2);
  MemWritePciCfgEp (Socket, Ch, SREF_STATIC2_MCDDC_CTL_REG, srefStatic2.Data);

  MemoryTimingsAdj.Data = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG);
  MemoryTimingsAdj.Bits.tcssr = Tcssr;
  MemoryTimingsAdj.Bits.tcped2srx = Tcped2srx;
  MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG, MemoryTimingsAdj.Data);
}

/**

  Config x4 SDDC 128b ECC mode. This mode starts support since SPR-A0 stepping.
  this mode is support when all the three featured disabled - directory/poison/sgx

  @param Host  - Pointer to sysHost
  @param socket  - Socket number
  @param ChOnSkt - socket chn

  @retval N/A

**/
VOID
EFIAPI
X4Sddc128bMode (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    ChOnSkt
  )
{
  X4MODESEL_MCDDC_DP_STRUCT         X4ModeSel;
  MC0_DP_CHKN_BIT3_MCDDC_DP_STRUCT  ChikenBit3;

  X4ModeSel.Data = MemReadPciCfgEp (Socket, ChOnSkt, X4MODESEL_MCDDC_DP_REG);
  X4ModeSel.Bits.enable_esddc = 1;
  MemWritePciCfgEp (Socket, ChOnSkt, X4MODESEL_MCDDC_DP_REG, X4ModeSel.Data);

  ChikenBit3.Data =  MemReadPciCfgEp (Socket, ChOnSkt, MC0_DP_CHKN_BIT3_MCDDC_DP_REG);
  ChikenBit3.Bits.dis_esddc_poison = 1;
  ChikenBit3.Bits.dis_esddc_dir = 1;
  ChikenBit3.Bits.dis_esddc_ns = 1;
  MemWritePciCfgEp (Socket, ChOnSkt, MC0_DP_CHKN_BIT3_MCDDC_DP_REG, ChikenBit3.Data);

}




/**

  Config x4 SDDC 125b ECC mode. This mode starts support since SPR-A0 stepping.
  this mode is support when any of the three featured enabled - directory/poison/sgx

  @param Host  - Pointer to sysHost
  @param socket  - Socket number
  @param ChOnSkt - socket chn

  @retval N/A

**/
VOID
EFIAPI
X4Sddc125bMode (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    ChOnSkt
  )
{
  X4MODESEL_MCDDC_DP_STRUCT         X4ModeSel;
  MC0_DP_CHKN_BIT3_MCDDC_DP_STRUCT  ChikenBit3;

  X4ModeSel.Data = MemReadPciCfgEp (Socket, ChOnSkt, X4MODESEL_MCDDC_DP_REG);
  X4ModeSel.Bits.enable_esddc = 1;
  MemWritePciCfgEp (Socket, ChOnSkt, X4MODESEL_MCDDC_DP_REG, X4ModeSel.Data);

  ChikenBit3.Data =  MemReadPciCfgEp (Socket, ChOnSkt, MC0_DP_CHKN_BIT3_MCDDC_DP_REG);
  ChikenBit3.Bits.dis_esddc_poison = 0;
  ChikenBit3.Bits.dis_esddc_dir = 0;
  ChikenBit3.Bits.dis_esddc_ns = 0;
  MemWritePciCfgEp (Socket, ChOnSkt, MC0_DP_CHKN_BIT3_MCDDC_DP_REG, ChikenBit3.Data);

}



/**

  Config x4 SDDC 96 ECC mode under following mode.
  1LM with integrity
  Mixed mode(NM region) with integrity disabled
  2LM


  @param Host  - Pointer to sysHost
  @param socket  - Socket number
  @param ChOnSkt - socket chn

  @retval N/A

**/
VOID
EFIAPI
X4Sddc96b (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    ChOnSkt
  )
{
#ifdef GNRSRF_HOST
  X4MODESEL_MCDDC_DP_STRUCT                 X4ModeSel;
#else // GNRSRF_HOST
  X4MODESEL_MCDDC_DP_SPRB0_SPRHBM_STRUCT    X4ModeSel;
#endif //GNRSRF_HOST

  if ((IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) || (IsCpuAndRevision (CPU_GNRSP, REV_ALL))) {
    X4ModeSel.Data = MemReadPciCfgEp (Socket, ChOnSkt, X4MODESEL_MCDDC_DP_REG);
    X4ModeSel.Bits.enable_esddc = 0;
    X4ModeSel.Bits.enable_nsddc = 1;
    MemWritePciCfgEp (Socket, ChOnSkt, X4MODESEL_MCDDC_DP_REG, X4ModeSel.Data);
  }
}

/**

  Config x4 SDDC 125-96 ECC mode for mixed mode.
   2LM (NM region) -- SDDC 96B
   1LM  - SDDC 125b


  @param Host  - Pointer to sysHost
  @param socket  - Socket number
  @param ChOnSkt - socket chn

  @retval N/A

**/
VOID
EFIAPI
X4Sddc125b96bMode (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    ChOnSkt
  )
{
#ifdef GNRSRF_HOST
  X4MODESEL_MCDDC_DP_STRUCT                 X4ModeSel;
#else // GNRSRF_HOST
  X4MODESEL_MCDDC_DP_SPRB0_SPRHBM_STRUCT    X4ModeSel;
#endif //GNRSRF_HOST

  if ((IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) || (IsCpuAndRevision (CPU_GNRSP, REV_ALL))) {
    X4ModeSel.Data = MemReadPciCfgEp (Socket, ChOnSkt, X4MODESEL_MCDDC_DP_REG);
    X4ModeSel.Bits.enable_esddc = 1;
    X4ModeSel.Bits.enable_nsddc = 1;
    MemWritePciCfgEp (Socket, ChOnSkt, X4MODESEL_MCDDC_DP_REG, X4ModeSel.Data);
  }
}

/**

  Config x4 SDDC 48b ECC mode for 9X4. This mode starts support since SPR-A0 stepping.
  It use SKX tree.

  @param Host  - Pointer to sysHost
  @param socket  - Socket number
  @param ChOnSkt - socket chn

  @retval N/A

**/
VOID
EFIAPI
X4Sddc48bTree (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    ChOnSkt
  )
{
  MCMTR_MC_MAIN_STRUCT                 McMtr;

  McMtr.Data = MemReadPciCfgEp (Socket, ChOnSkt, MCMTR_MC_MAIN_REG);
  McMtr.Bits.ddr5_9x4_en  = 1;
  MemWritePciCfgEp (Socket, ChOnSkt, MCMTR_MC_MAIN_REG, McMtr.Data);
}


/**

  Config x4 SDDC 48b ECC mode for 9X4. This mode starts support since SPR-b0 stepping.
  It use ICX new tree.

  @param Host  - Pointer to sysHost
  @param socket  - Socket number
  @param ChOnSkt - socket chn

  @retval N/A

**/
VOID
EFIAPI
X4Sddc48bNewTree (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    ChOnSkt
  )
{
  MCMTR_MC_MAIN_STRUCT                        McMtr;
#ifdef GNRSRF_HOST
  X4MODESEL_MCDDC_DP_STRUCT                   X4ModeSel;
#else // GNRSRF_HOST
  X4MODESEL_MCDDC_DP_SPRB0_SPRHBM_STRUCT      X4ModeSel;
#endif //GNRSRF_HOST

  McMtr.Data = MemReadPciCfgEp (Socket, ChOnSkt, MCMTR_MC_MAIN_REG);
  McMtr.Bits.ddr5_9x4_en  = 1;
  MemWritePciCfgEp (Socket, ChOnSkt, X4MODESEL_MCDDC_DP_REG, McMtr.Data);

  if ((IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) || (IsCpuAndRevision (CPU_GNRSP, REV_ALL))) {
    X4ModeSel.Data = MemReadPciCfgEp (Socket, ChOnSkt, X4MODESEL_MCDDC_DP_REG);
    X4ModeSel.Bits.enable_nsddc = 1;
    MemWritePciCfgEp (Socket, ChOnSkt, X4MODESEL_MCDDC_DP_REG, X4ModeSel.Data);
  }
}

/**

  Config PFD mode according PfdFlag. This feature is design since b0 stepping.

  @param Host  - Pointer to sysHost
  @param socket  - Socket number
  @param ChOnSkt - socket chn
  @param PfdFlag  -- PFD mode enable

  @retval N/A

**/
VOID
EFIAPI
SetPfd (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    ChOnSkt,
  BOOLEAN  PfdFlag
  )
{
#ifdef GNRSRF_HOST
  ECC_CORR_BIST_MCDDC_DP_STRUCT                                  EccBist;
#else // GNRSRF_HOST
  ECC_CORR_BIST_MCDDC_DP_SPRB0_SPRHBM_SPRC0_SPRMCC_SPRUCC_STRUCT EccBist;
#endif //GNRSRF_HOST
  UINT32                                                         TempData;
  MC0_DP_CHKN_BIT3_MCDDC_DP_STRUCT                               ChknBitReg;

  if (PfdFlag == FALSE) {
    return;
  }

  if ((IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) || (IsCpuAndRevisionOrLater (CPU_GNRSP, REV_ALL))) {
    if (IsSiliconWorkaroundEnabled ("S14011347981")) {
    //Disable metadata ECC correction when PFD is enabled
     TempData =  MemReadPciCfgEp (Socket, ChOnSkt, MC0_DP_CHKN_BIT3_MCDDC_DP_REG);
     ChknBitReg.Bits.dis_meta_ecc_corr = 1;
     MemWritePciCfgEp (Socket, ChOnSkt, MC0_DP_CHKN_BIT3_MCDDC_DP_REG, TempData);
    }
    TempData =  MemReadPciCfgEp (Socket, ChOnSkt, ECC_CORR_BIST_MCDDC_DP_REG);
    EccBist.Data = (UINT8)TempData;
    EccBist.Bits.bist_en = 1;
    EccBist.Bits.bist_corr_mode  = 1;
    EccBist.Bits.bist_err_report_mode  = 0;
    TempData &= 0xFFFFFF00;
    TempData |= EccBist.Data;
    MemWritePciCfgEp (Socket, ChOnSkt, ECC_CORR_BIST_MCDDC_DP_REG, TempData);
  }
}

/**

  Enable nSddc mode

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/
VOID
EnableNsddcMode (
  PSYSHOST Host,
  UINT8    socket
  )
{
  return;
}


/**

  Enable Enhanced SDDC

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/
VOID
EnableEnhancedSddc (
  PSYSHOST Host,
  UINT8    socket
  )
{
  UINT8                             mcId;
  UINT8                             ChIndex;
  UINT8                             ChOnSkt;
  RAS_RC_POLICY                     *Policy;
  SECURITY_POLICY                   *SecurityPolicy = NULL;
  BOOLEAN                           PoisonFlag = FALSE;
  BOOLEAN                           DirectoryFlag;
  BOOLEAN                           SgxFlag = FALSE;
  BOOLEAN                           PfdFlag = FALSE;
  BOOLEAN                           TmeEnabled = FALSE;
  UINT8                             MemoryMode;


  Policy = GetRasRcPolicy ();
  if (Policy != NULL) {
    PoisonFlag = (Policy->PoisonEn)?TRUE:FALSE;
    PfdFlag =  (Policy->PfdEn)?TRUE:FALSE;
  }

  SecurityPolicy = GetSecurityPolicy2 ();
  if (SecurityPolicy != NULL) {
    SgxFlag = (SecurityPolicy->EnableSgx == SECURITY_POLICY_ENABLE) ? TRUE : FALSE;
  }

  DirectoryFlag = IfFeatureEnabled (TypeDirectoryMode);
  MemoryMode = GetVolMemMode ();

  TmeEnabled = IsTmeEnabled (socket);

  RcDebugPrint (SDBG_MAX, "EnableEnhancedSddc Poison:%d  Directory:%d   Sgx:%d  Tme:%d  Pfd:%d\n",
             PoisonFlag,  DirectoryFlag, SgxFlag, TmeEnabled, PfdFlag);

  for (mcId = 0; mcId < MAX_IMC; mcId++) {
    if (Host->var.mem.socket[socket].imcEnabled[mcId] == 0) {
      continue;
    }

    for (ChIndex = 0; ChIndex < MAX_MC_CH; ChIndex++) {
      ChOnSkt = NODECH_TO_SKTCH (mcId, ChIndex);

      if (Host->nvram.mem.socket[socket].channelList[ChOnSkt].enabled == 0) {
        continue;
      }


     if (Check9x4Ddr5DimmPresentPerChn (socket, ChOnSkt) == FALSE) {
      //
      //10x4 or 8x5.
      //
      if (MemoryMode == VOL_MEM_MODE_1LM) {
        if ((DirectoryFlag == FALSE) && (SgxFlag == FALSE) && (PoisonFlag == FALSE)) {
          X4Sddc128bMode (Host, socket, ChOnSkt);
          RcDebugPrintWithDevice (SDBG_MAX, socket, ChOnSkt, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "1LM -x4 SDDC 128bit\n");
        } else {
          if (TmeEnabled == TRUE) {
            if (IsSiliconWorkaroundEnabled ("S14011156662")) {
              PfdFlag = FALSE;
            }
            X4Sddc96b (Host, socket, ChOnSkt);
            SetPfd (Host, socket, ChOnSkt, PfdFlag);
            RcDebugPrintWithDevice (SDBG_MAX, socket, ChOnSkt, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "1LM - x4 SDDC 96bit with PFD:%d\n", PfdFlag);
          } else {
            X4Sddc125bMode (Host, socket, ChOnSkt);
            SetPfd (Host, socket, ChOnSkt, PfdFlag);
            RcDebugPrintWithDevice (SDBG_MAX, socket, ChOnSkt, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "1LM - x4 SDDC 125bit with PFD:%d\n", PfdFlag);
          }
        }
      }

      if (MemoryMode == VOL_MEM_MODE_MIX_1LM2LM) {
        if (IsSiliconWorkaroundEnabled ("S14011154764")) {
          PfdFlag = FALSE;
        }
        if (TmeEnabled == TRUE) {
            X4Sddc96b (Host, socket, ChOnSkt);
            SetPfd (Host, socket, ChOnSkt, PfdFlag);
            RcDebugPrintWithDevice (SDBG_MAX, socket, ChOnSkt, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "Mixed mode - x4 SDDC 96bit with PFD:%d\n", PfdFlag);
          } else {
            X4Sddc125b96bMode (Host, socket, ChOnSkt);
            SetPfd (Host, socket, ChOnSkt, PfdFlag);
            RcDebugPrintWithDevice (SDBG_MAX, socket, ChOnSkt, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "Mixed mode - x4 SDDC 125bit(NM), 96b(1LM) with PFD:%d\n", PfdFlag);
          }
      }

      if ((MemoryMode == VOL_MEM_MODE_2LM) && (GetCacheMemType () == CACHE_TYPE_DDR_CACHE_PMEM)) {
        X4Sddc96b (Host, socket, ChOnSkt);
        if (IsSiliconWorkaroundEnabled ("S14011154764")) {
          PfdFlag = FALSE;
        }
        SetPfd (Host, socket, ChOnSkt, PfdFlag);
        RcDebugPrintWithDevice (SDBG_MAX, socket, ChOnSkt, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "2LM - x4 SDDC 96bit with PFD:%d\n", PfdFlag);
      }
    } else {
      //
      //9x4
      //
      if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) {
        X4Sddc48bNewTree (Host, socket, ChOnSkt);
        RcDebugPrintWithDevice (SDBG_MAX, socket, ChOnSkt, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                " x4 SDDC NEW 48bit \n");
      } else {
        X4Sddc48bTree (Host, socket, ChOnSkt);
        RcDebugPrintWithDevice (SDBG_MAX, socket, ChOnSkt, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                " x4 SDDC 48bit \n");
      }
    }
   }
  }
}

/**

  Configure DDRT persistent domain delay
  Executed in late MRC when training is complete and switching the MC to normal mode

  @param[in]  Host    - Pointer to SysHost
  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number

  @return   N/A
**/
VOID
DdrtPersistentDomainDelayLate (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT8     Ch
  )
{
  struct dimmNvram                (*DimmNvList)[MAX_DIMM];
  T_PERSIST_TIMING_MC_2LM_STRUCT  PersistTiming;

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  PersistTiming.Data = MemReadPciCfgEp (Socket, Ch, T_PERSIST_TIMING_MC_2LM_REG);
  PersistTiming.Bits.t_persist_delay = T_PERSIST_DELAY_DEFAULT;
  MemWritePciCfgEp (Socket, Ch, T_PERSIST_TIMING_MC_2LM_REG, PersistTiming.Data);
}

/**
  Get CKE idle timer.

  Get the appropriate value to write to the cke_idle_timer field of CKE_LLn and
  PKGC_CKE CSRs.

  @param[in]  Host          Pointer to sysHost.
  @param[in]  Socket        Socket number to convert for.
  @param[in]  Nanoseconds   Number of nanoseconds to convert.

  @return     Value to write to cke_idle_timer CSR field(s).
**/
UINT8
GetCkeIdleTimer (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Nanoseconds
  )
{
  UINT32  DclkCycle;
  UINT32  HclkCycle;

  DclkCycle = GettCK (Host, Socket);  // DCLK cycles/ns * 10000
  HclkCycle = DclkCycle / 2;          // HCLK cycles/ns * 10000

  //
  // Return the value in HCLKs for SPR.
  //
  return (UINT8) ((Nanoseconds * HclkCycle) / 10000);
}

/**
  Program DDRT RD/WR/GNT to Power Down Timing Constraints.

  @param[in]  Host             Pointer to sysHost.
  @param[in]  Socket           Socket number.
  @param[in]  Ch               Channel number.
  @param[in]  RdPdSeparation   Minimum READ to PD command separation (in DCLK).
                               Ignore when it is 0.
  @param[in]  WrPdSeparation   Minimum WRITE to PD command separation (in DCLK).
                               Ignore when it is 0.
  @param[in]  GntPdSeparation  Minimum GRANT to PD command separation (in DCLK).
                               Ignore when it is 0.

  @return     EFI_SUCCESS      Successfully programmed Power Down timing constraints.
*/
EFI_STATUS
ConfigDdrtPowerDownTiming (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     RdPdSeparation,
  IN  UINT8     WrPdSeparation,
  IN  UINT8     GntPdSeparation
  )
{
  T_DDRT_PD_MC_2LM_STRUCT   DdrtPd;

  DdrtPd.Data = MemReadPciCfgEp (Socket, Ch, T_DDRT_PD_MC_2LM_REG);

  if (RdPdSeparation != 0) {
    DdrtPd.Bits.t_ddrt_rdpden = RdPdSeparation;
  }

  if (WrPdSeparation != 0) {
    DdrtPd.Bits.t_ddrt_wrpden = WrPdSeparation;
  }

  if (GntPdSeparation != 0) {
    DdrtPd.Bits.t_ddrt_gntpden = GntPdSeparation;
  }

  MemWritePciCfgEp (Socket, Ch, T_DDRT_PD_MC_2LM_REG, DdrtPd.Data);

  return EFI_SUCCESS;
}

/**
  Set Write CRC chicken bits when Write CRC is disabled.

  The chicken bits are written for every enabled channel in the socket.

  @param[in]  Host      Pointer to sysHost.
  @param[in]  Socket    Socket number.
**/
VOID
DisableWriteCrcChickenBits (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  MC0_DP_CHKN_BIT3_MCDDC_DP_STRUCT  Mc0DpChknBit3;
  UINT8                             MaxChDdr;
  UINT8                             Channel;

  //
  // Workaround for a scrambling logic corruption silicon bug.
  //
  if (IsSiliconWorkaroundEnabled ("S14010043192")) {
    MaxChDdr = GetMaxChDdr ();
    for (Channel = 0; Channel < MaxChDdr; Channel++) {
      if (IsChannelEnabled (Socket, Channel) == FALSE) {
        continue;
      }
      Mc0DpChknBit3.Data = MemReadPciCfgEp (Socket, Channel, MC0_DP_CHKN_BIT3_MCDDC_DP_REG);
      Mc0DpChknBit3.Bits.dis_hphase_and_odd_delay_fix_for_wrcrc = 1;
      MemWritePciCfgEp (Socket, Channel, MC0_DP_CHKN_BIT3_MCDDC_DP_REG, Mc0DpChknBit3.Data);
    }
  }
}

/**

  Disable Clock Gate in Scheduler

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number

  @retval N/A

**/
VOID
DisClkGateSchedMc (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  UINT8                               Ch;
  struct channelNvram                 (*ChannelNvList)[MAX_CH];
  MCSCHED_CHKN_BIT_MCDDC_CTL_STRUCT   ChknBit;

  ChannelNvList = GetChannelNvList(Host, Socket);

  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    //
    // Disable Clock Gate in Scheduler
    //
    ChknBit.Data = MemReadPciCfgEp (Socket, Ch, MCSCHED_CHKN_BIT_MCDDC_CTL_REG);
    ChknBit.Bits.dis_ck_gate_sched = 0;
    MemWritePciCfgEp (Socket, Ch, MCSCHED_CHKN_BIT_MCDDC_CTL_REG, ChknBit.Data);
  }
  //
  // Delay if this is a CTE build, otherwise do nothing.
  //

  CteDelayDclk (10);
}

/**
  Disable wait ST_REF_DQSOSC_STRT_REQ state during retry

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number

  @retval N/A

**/
VOID
DisRefDqsOscWaitDuringRetry (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  UINT8                               Ch;
  struct channelNvram                 (*ChannelNvList)[MAX_CH];
  MCSCHED_CHKN_BIT_MCDDC_CTL_STRUCT   ChknBit;

  ChannelNvList = GetChannelNvList(Host, Socket);

  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    //
    // Disable wait during ST_REF_DQSOSC_STRT_REQ state during retry and instead go back to FREE state;
    // default value should be 1'b1 (i.e. skip DQSOSC and return to REF_FREE during retry).
    //
    ChknBit.Data = MemReadPciCfgEp (Socket, Ch, MCSCHED_CHKN_BIT_MCDDC_CTL_REG);
    ChknBit.Bits.dis_ref_dqsosc_wait_during_retry = 1;
    MemWritePciCfgEp (Socket, Ch, MCSCHED_CHKN_BIT_MCDDC_CTL_REG, ChknBit.Data);
  }
}

/**

  SPR A0 workaround cas2cad_dd timing restriction

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number

  @retval N/A

**/
VOID
TrrddDdrt2ChickenBit (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
#ifndef GNRSRF_HOST
  UINT8                               Ch;
  struct channelNvram                 (*ChannelNvList)[MAX_CH];
#if MC_IP_VER > MEM_IP_19ww33b
  MCSCHED_CHKN_BIT_MCDDC_CTL_SPRA0_STRUCT   ChknBit;
#endif

  if (IsSiliconWorkaroundEnabled("S1409883828")) {
    ChannelNvList = GetChannelNvList(Host, Socket);

    for (Ch = 0; Ch < MAX_CH; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }

      if ((*ChannelNvList)[Ch].ddrtEnabled == 1 && (*ChannelNvList)[Ch].maxDimm == 2) {
        continue;
      }

#if MC_IP_VER > MEM_IP_19ww33b
      ChknBit.Data = MemReadPciCfgEp (Socket, Ch, MCSCHED_CHKN_BIT_MCDDC_CTL_REG);
      ChknBit.Bits.unused_bits12_2 |= BIT0;
      MemWritePciCfgEp (Socket, Ch, MCSCHED_CHKN_BIT_MCDDC_CTL_REG, ChknBit.Data);
#endif

    }
  }
  return;
#endif // !GNRSRF_HOST
}

/**

  Config served MRR entry in RDB

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number
  @param[in] Ch      - Channel number
  @param[in] Enable  - 1: Enable served MRR entry in RDB;  0: Disable served MRR entry in RDB

  @retval N/A

**/
VOID
ConfigRidMrrEntry (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN BOOLEAN  Enable
  )
{
  MCMNT_CHKN_BIT2_MCDDC_CTL_STRUCT McMntChknBit2;

  McMntChknBit2.Data = MemReadPciCfgEp (Socket, Ch, MCMNT_CHKN_BIT2_MCDDC_CTL_REG);
  McMntChknBit2.Bits.enable_rid_mrr = (Enable) ? 1 : 0;
  MemWritePciCfgEp (Socket, Ch, MCMNT_CHKN_BIT2_MCDDC_CTL_REG, McMntChknBit2.Data);
}

/**

  Disable corresponding error in internal_error_dp register when DDRT populated

  @param[in] Socket  - Socket number
  @param[in] Ch      - Channel number

  @retval N/A

**/
VOID
DisRtUnexpSchedActive (
  IN UINT8    Socket,
  IN UINT8    Ch
  )
{
  PSYSHOST                                    Host;
  DIS_INTERNAL_ERROR_DP_N0_MCDDC_DP_STRUCT    DisInternalErrorDpN0;

  if (IsSiliconWorkaroundEnabled ("S1409773441")) {

    Host = GetSysHostPointer ();

    if (!IsDcpmmPresentChannel (Socket, Ch)) {
      return;
    }

    DisInternalErrorDpN0.Data = MemReadPciCfgEp (Socket, Ch, DIS_INTERNAL_ERROR_DP_N0_MCDDC_DP_REG);
    DisInternalErrorDpN0.Bits.dis_rt_unexp_sched_active = 1;
    MemWritePciCfgEp (Socket, Ch, DIS_INTERNAL_ERROR_DP_N0_MCDDC_DP_REG, DisInternalErrorDpN0.Data);
  }
}

/**
  Configure the command to data completion delay

  @param[in]  Socket    - Socket number

  @return None
**/
VOID
EarlyConfigCmdToDataCompletionDelay (
  IN    UINT8     Socket
  )
{
  ProgramCmdToDataCompletionDelay (Socket);
}

/**
  Configure CTE environment

  @param[in] Host          - Pointer to sysHost
  @param[in] Socket        - Socket number
  @param[in] Ch            - Channel number
  @param[in] ConfigType    - Config type
  @param[in] ConfigData    - Config data

  @return None
**/
VOID
CteConfiguration (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN CTE_CONFIG ConfigType,
  IN UINT32     ConfigData
  )
{
  struct channelNvram               (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList(Host, Socket);

  switch (ConfigType) {
    case CTE_CONFIG_UPDATE_CL:
      if (FeaturePcdGet (PcdCteBuild) == TRUE) {
         (*ChannelNvList)[Ch].common.nCL = (UINT8) ConfigData;
      }
    break;
    case CTE_CONFIG_UPDATE_TCL:
      if (Host->DdrioUltSupport) {
        //
        // Notify ULT to change tcl
        //
        CteSetOptions ("set_tcl", 1 << Ch, TWO_SUBCH_MASK, 0, ConfigData);
      }
    break;
    case CTE_CONFIG_UPDATE_TCWL:
      if (Host->DdrioUltSupport) {
        //
        // Notify ULT to change tcwl
        //
        CteSetOptions ("set_tcwl", 1 << Ch, TWO_SUBCH_MASK, 0, ConfigData);
      }
    break;
    case CTE_CONFIG_UPDATE_TIMING_MODE:
      if (Host->DdrioUltSupport) {
        //
        // Notify ULT to change the timing mode.
        //
        if (ConfigData == TIMING_1N) {
          CteSetOptions ("cmd_stretch", 0x3, TWO_SUBCH_MASK, 0, 1);
          CteSetOptions ("cmd_stretch", 0x3, TWO_SUBCH_MASK, 4, 1);
        } else {
          CteSetOptions ("cmd_stretch", 0x3, TWO_SUBCH_MASK, 0, 2);
          CteSetOptions ("cmd_stretch", 0x3, TWO_SUBCH_MASK, 4, 2);
        }
      }
    break;
    default:
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Unsupported CTE config type: %d\n",ConfigType);
    break;
  }
}

/**

  Enable Cpgc Cmi Credit for memory training.

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number

  @retval N/A

**/
VOID
EnableCpgcCmiCredit (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  UINT32                                          UseCmi;
  BOOLEAN                                         EnableUseCmi = TRUE;
  EFI_STATUS                                      Status;
  UINT8                                           Ch;
  struct channelNvram                             (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList(Host, Socket);

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    Status = CteGetOptions ("use_cmi", &UseCmi);
    if (!EFI_ERROR (Status)) {
      if (UseCmi != 0) {
        EnableUseCmi = TRUE;
      } else {
        EnableUseCmi = FALSE;
      }
    } else {
      EnableUseCmi = FALSE;
    }
  }

  //
  // Enable Cpgc Cmi Credit for memory training. Skipp for ULT or use_cmi  == 0.
  //
  if ((Host->DdrioUltSupport == 0) && (EnableUseCmi == TRUE)) {
    //
    // Save credit map default values
    // These must be restored before switching to normal traffic
    //
    SaveCmiCreditConfigDefault (Host, Socket);

    for (Ch = 0; Ch < MAX_CH; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }
      EarlyConfigMcCmi (Socket, Ch);
      EarlyConfigCpgcCmi (Socket, Ch);

      // Delay 80 Dclk @ 3200
      // (80 / ((3200/2) * 10+e6)) * 10+e9 = 50 ns
      // This is a safe delay for higher freq
      NanoSecondDelay (50);
      CteDelayDclk (80);

      SetMcReqCreditCfgDone (Socket, Ch);
      SetCpgcCreditConfigDone (Socket, Ch);
    }

    NanoSecondDelay (50); // 80 Dclk @ 3200
    CteDelayDclk (80);

    for (Ch = 0; Ch < MAX_CH; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }
      PollCpgcReqCreditsInitialized (Socket, Ch);
      PollCpgcIsmIdle (Socket, Ch);
      PollCpgcCreditsInitialized (Socket, Ch);
    }
  }
}

/**
  Prepare requesters for initializing CMI Credit registers

  @param[in] Socket     - Socket number

  @retVal  MRC_STATUS_SUCCESS if the programming occured
  @retVal  MRC_FUNCTION_NOT_IMPLEMENTED otherwise
**/
MRC_STATUS
PrepareForCmiCreditProgramming (
  IN UINT8    Socket
)
{
  UINT8               Ch;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  //
  // Before CMI credit initialization, clear "cfg done" bits
  // for the MC and CPGC
  //
  for (Ch = 0; Ch <MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch)) {
      InitMcCmiReqCpCfg (Socket, Ch);
      InitCpgcCreditConfigDone (Socket, Ch);
    }
  } // Ch
  return MRC_STATUS_SUCCESS;
}

/**
  Configure DQS CAP error wait time.

  @param[in]  Host      Pointer to sysHost
  @param[in]  Socket    Socket number
  @param[in]  Channel   Channel number
**/
VOID
ConfigDqsCapErrorWaitTime (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Channel
  )
{
  LINK_RETRY_TIMER2_MCDDC_DP_STRUCT   LinkRetryTimer2;
  MCMNT_RCB_GATE_CTL_MCDDC_CTL_STRUCT McmntRcbGateCtl;
  UINT32                              PipeDelay;

  LinkRetryTimer2.Data = MemReadPciCfgEp (Socket, Channel, LINK_RETRY_TIMER2_MCDDC_DP_REG);
  PipeDelay = ((LinkRetryTimer2.Bits.cmd_cmpl_time & CMD_CMPL_TIME_MASK) / 2) + PIPE_DELAY_CONST;

  McmntRcbGateCtl.Data = MemReadPciCfgEp (Socket, Channel, MCMNT_RCB_GATE_CTL_MCDDC_CTL_REG);
  McmntRcbGateCtl.Bits.dqscaperrorwaittime = (
    GettCWL (Host, Socket, Channel) +
    GettCL (Host, Socket, Channel) +
    PipeDelay
    );
  MemWritePciCfgEp (Socket, Channel, MCMNT_RCB_GATE_CTL_MCDDC_CTL_REG, McmntRcbGateCtl.Data);
}

/**
  Program Ecc Retry Fail Limit

  @param[in] Socket     - Socket number
  @param[in] Ch         - Channel number

  @retVal   None
**/
VOID
ProgramEccRetryFailLimit (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
  PSYSHOST                                Host;
  MCSCHEDS_ECC_RETRY_MCDDC_CTL_STRUCT     EccRetryCfg;

  Host = GetSysHostPointer ();

  EccRetryCfg.Data = MemReadPciCfgEp (Socket, Ch, MCSCHEDS_ECC_RETRY_MCDDC_CTL_REG);
  EccRetryCfg.Bits.ecc_retry_fail_limit = ECC_RETRY_FAIL_LIMIT;
  MemWritePciCfgEp (Socket, Ch, MCSCHEDS_ECC_RETRY_MCDDC_CTL_REG, EccRetryCfg.Data);
}

/**
  Check if DIMM Capacity is supported or not by CPU SKU.

  @param[in]  Host       Pointer to sysHost.
  @param[in]  Socket     Socket number.
  @param[in]  Ch         Channel number.
  @param[in]  Dimm       Dimm number.

  @retVal   TRUE      DIMM Capacity is supported by CPU SKU
  @retVal   FALSE     DIMM Capacity is NOT supported by CPU SKU
**/
BOOLEAN
CheckIsDramSupportByCpuSku (
  IN PSYSHOST                  Host,
  IN UINT8                     Socket,
  IN UINT8                     Ch,
  IN UINT8                     Dimm
  )
{
  //
  // Skipping for headless MRC because PythonSv is not able to access this fuse register.
  //
  UINT8                                   SdramCapacity;
  IMC_FUSE_DOWNLOAD_SHADOW_MC_MAIN_STRUCT FuseShadow;
  struct dimmNvram                        (*dimmNvList)[MAX_DIMM];

  dimmNvList = GetDimmNvList(Host, Socket, Ch);
  SdramCapacity = (*dimmNvList)[Dimm].sdramCapacity;

  //
  // Check if DIMM is supported for specific CPU SKU
  //
  FuseShadow.Data = MemReadPciCfgMC (Socket, GetMCID(Host, Socket, Ch), IMC_FUSE_DOWNLOAD_SHADOW_MC_MAIN_REG);

  //
  // DDR5 - 24GB - SKU  or DDR4 - 16GB
  //
  if (FuseShadow.Bits.fuse_shadow_disable_16gbit == 1) {
    if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
      // DDR5
      if (EncodedSpdToDramDensity(SdramCapacity) >= EncodedSpdToDramDensity (SPD_24Gb)) {
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "24Gb or higher Capacity DRAMs not supported with this SKU!\n");
        OutputWarning (WARN_DIMM_COMPAT, WARN_DIMM_24GB_SUPPORTED, Socket, Ch, Dimm, NO_RANK);
        return FALSE;
      }
    } else {
      // DDR4
      if (EncodedSpdToDramDensity(SdramCapacity) >= EncodedSpdToDramDensity (SPD_16Gb)) {
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "16Gb DRAMs not supported with this SKU!\n");
        OutputWarning (WARN_DIMM_COMPAT, WARN_DIMM_16GB_SUPPORTED, Socket, Ch, Dimm, NO_RANK);
        return FALSE;
      }
    }
  }

  return TRUE;
}

/**
  Set the Rcomp Timer configuration after training is complete.

  @param[in] Host         - Pointer to sysHost.
  @param[in] Socket       - Socket Id
  @param[in] Ch           - Channel number within the socket

  @retval N/A

**/
VOID
RcompTimerConfig (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  RCOMP_TIMER_MC_MAIN_STRUCT  RcompTimer;

  RcompTimer.Data = MemReadPciCfgEp (Socket, Ch, RCOMP_TIMER_MC_MAIN_REG);
  //
  // Set increase_rcomp to 1 for all configs to prevent read ecc errors on configs with
  // larger round trip latencies
  //
  RcompTimer.Bits.increase_rcomp = 1;
  //
  // The Reset value of rcomp_timer, 0x2C, does not program the timer long enough
  // to avoid some cases where the DDRIO might receive a transaction while expecting
  // the MC bus to be quiet during an Rcomp flow.
  // A value of 0x3A is needed to give enough time in the worst case, to ensure that
  // the MC truly is quiet and not sending transactions
  //
  RcompTimer.Bits.rcompblock = 0x3A;
  MemWritePciCfgEp (Socket, Ch, RCOMP_TIMER_MC_MAIN_REG, RcompTimer.Data);
}

/**

  Check if the DDR5/NM or DDRT/FM channel is in 2LM - cross channel system (DDRT2/FM on one channel and DDR5/NM on another channel)

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Current Socket
  @param[in] Channel   - Current Channel
  @param[in] Mode      - Check DDR5/NM or DDRT/FM channel

  @retval TRUE   - DDR5/NM or DDRT/FM channel is in 2LM - cross channel system
  @retval FALSE  - DDR5/NM or DDRT/FM channel is not in 2LM - cross channel system

**/
BOOLEAN
IsChIn2lmCrossChannelSystem (
  IN PSYSHOST          Host,
  IN UINT8             Socket,
  IN UINT8             Channel,
  IN MODE_2LM_CROSS_CH Mode
  )
{
  UINT8          Mc;
  UINT8          NumChPerMc;
  UINT8          McChannel;
  SAD_TABLE      *SadEntry;
  UINT8          ClusterIdx;
  UINT8          SadRuleIdxInCluster;
  UINT8          SadIndex;
  BOOLEAN        UnusedSadClusterFound = FALSE;

  NumChPerMc = GetNumChannelPerMc ();
  Mc = GetMCID (Host, Socket, Channel);

  //
  // In 2LM - cross channel system (DDRT2/FM on one channel and DDR5/NM on another channel)
  // disable the DDRT_Cmpl_fifo_Overflow case by setting this bit to 1 in NM MC channel
  //
  if (IsMcPopulated (Socket, Mc) && IsMemTypePresentMcEnum (Socket, Mc, MemType2lmDdrCacheMemoryMode)) {

    //
    // Go through the SAD rules
    //
    for (ClusterIdx = 0; ClusterIdx < MAX_DRAM_CLUSTERS; ClusterIdx++) {
      if (UnusedSadClusterFound) {
        break;
      }

      for (SadRuleIdxInCluster = 0; SadRuleIdxInCluster < MAX_SAD_RULES; SadRuleIdxInCluster++) {
        SadIndex = ClusterIdx * MAX_SAD_RULES + SadRuleIdxInCluster;
        SadEntry = &Host->var.mem.socket[Socket].SAD[SadIndex];

        if (SadEntry->Enable == 0) {
          if (SadRuleIdxInCluster == 0) {
            UnusedSadClusterFound = TRUE;
          }
          break; // end of SAD table
        }

        if (SadEntry->type == MemType2lmDdrCacheMemoryMode) {
          McChannel = (Channel % NumChPerMc);

          if (Mode == DdrIn2lmCrossCh) {
            //
            // Check if current channel is DDR5/NM and another channel is DDRT/FM
            //
              if (((McChannel == 0) && ((SadEntry->channelInterBitmap[Mc] & BITMAP_CH0) != 0) && ((SadEntry->FMchannelInterBitmap[Mc] & BITMAP_CH1) != 0)) ||
                  ((McChannel == 1) && ((SadEntry->channelInterBitmap[Mc] & BITMAP_CH1) != 0) && ((SadEntry->FMchannelInterBitmap[Mc] & BITMAP_CH0) != 0))) {
              return TRUE;
            }
          } else if (Mode == DdrtIn2lmCrossCh) {
            //
            // Check if current channel is DDRT/FM and another channel is DDR5/NM
            //
            if (((McChannel == 0) && ((SadEntry->FMchannelInterBitmap[Mc] & BITMAP_CH0) != 0) && ((SadEntry->channelInterBitmap[Mc] & BITMAP_CH1) != 0)) ||
                ((McChannel == 1) && ((SadEntry->FMchannelInterBitmap[Mc] & BITMAP_CH1) != 0) && ((SadEntry->channelInterBitmap[Mc] & BITMAP_CH0) != 0))) {
              return TRUE;
            }
          } else {
            RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "ERROR: Unsupported Mode 0x%x!\n", Mode);
          }
        }
      }  // SadRuleIdxInCluster
    }  // ClusterIdx
  }

  return FALSE;
}

/**

  Late SbMcaCtl Register configuration

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - current socket #

  @return N/A
**/
VOID
SetSbMcaCtl (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  UINT8                             Channel;
  SBMCACTL_MC_2LM_STRUCT            SbMcaCtl;
  UINT8                             MaxChDdr;

  if (!IsSiliconWorkaroundEnabled ("S14010293656")) {
    return;
  }

  if (!IsDdr5Present (Host, Socket)) {
    return;
  }

  MaxChDdr = GetMaxChDdr ();
  //
  // Disable 2LM completion fifo overflow MCA in 2LM - cross channel system
  //
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    if (IsChIn2lmCrossChannelSystem (Host, Socket, Channel, DdrIn2lmCrossCh)) {

      SbMcaCtl.Data = MemReadPciCfgEp (Socket, Channel, SBMCACTL_MC_2LM_REG);
      SbMcaCtl.Bits.ddrtwrcmpfifoerrdis = 1;
      MemWritePciCfgEp (Socket, Channel, SBMCACTL_MC_2LM_REG, SbMcaCtl.Data);
    }
  }
}

/**
  Program delay from ZQ to anycmd delay

  @param[in] Socket     - Socket number

  @retVal   None
**/
VOID
SetDdrtZqToAnyCmdDelay (
  IN UINT8 Socket
  )
{
  PSYSHOST                                Host;
  UINT8                                   Ch;
  UINT8                                   MaxChDdr;
  BOOLEAN                                 ZqStartFlag = FALSE;
  BOOLEAN                                 ZqLatchFlag = FALSE;
  T_DDRT_MNT_CNSTRNT_MC_2LM_STRUCT        DdrtMntCnstrnt;

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();

  if (IsSiliconWorkaroundEnabled ("S14010123163")) {
    ZqStartFlag = TRUE;
  }

  if (IsSiliconWorkaroundEnabled ("S16010813320")) {
    ZqLatchFlag = TRUE;
  }

  if (ZqStartFlag || ZqLatchFlag) {
    for (Ch = 0; Ch < MaxChDdr; Ch++) {

      if (IsDcpmmPresentChannel (Socket, Ch)) {
        DdrtMntCnstrnt.Data = MemReadPciCfgEp (Socket, Ch, T_DDRT_MNT_CNSTRNT_MC_2LM_REG);

        if (ZqStartFlag) {
          // tMPC will follow tMRD, which is max(TMRD_NS, TMRD_MIN_DCLK) in DDR5 Spec
          DdrtMntCnstrnt.Bits.t_ddrt_zqs_anycmd = MAX (NanosecondToClocks (Socket, TMRD_NS), TMRD_MIN_DCLK);
        }

        if (ZqLatchFlag) {
          DdrtMntCnstrnt.Bits.t_ddrt_zql_anycmd = DDRT2_TZQLAT_ANYCMD_MAX_DCLK;
        }

        MemWritePciCfgEp (Socket, Ch, T_DDRT_MNT_CNSTRNT_MC_2LM_REG, DdrtMntCnstrnt.Data);
      }
    }
  }
}

/**
  Switch to DDRT2 mode for Tx training

  @param[in]  Host    Pointer to SysHost

  @retval Success
**/
UINT32
EFIAPI
SwitchToDdrt2Mode (
  IN  PSYSHOST  Host
  )
{
  UINT8                     Socket;
  UINT8                     Channel;
  UINT8                     Dimm;
  MCDDRTCFG_MC_MAIN_STRUCT  McDdrtCfg;
  UINT8                     MaxChDdr;
  struct dimmNvram         (*DimmNvList) [MAX_DIMM];
  struct channelNvram      (*ChannelNvList)[MAX_CH];
  Socket = Host->var.mem.currentSocket;
  MaxChDdr = GetMaxChDdr ();

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (IsDcpmmPresentChannel (Socket, Channel) == FALSE) {
      continue;
    }

    McDdrtCfg.Data = MemReadPciCfgEp (Socket, Channel, MCDDRTCFG_MC_MAIN_REG);
    McDdrtCfg.Bits.slot0 = 0;
    McDdrtCfg.Bits.slot1 = 0;
    DimmNvList = GetDimmNvList (Host, Socket, Channel);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDcpmmPresentDimm (Socket, Channel, Dimm) == TRUE) {

        if (Dimm == 0) {
          McDdrtCfg.Bits.slot0 = 1;
        } else if (Dimm == 1) {
          McDdrtCfg.Bits.slot1 = 1;
        } else {
          RcDebugPrintWithDevice (SDBG_ERROR, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "\nInvalid DIMM slot encountered during SwitchToDdrt2Mode");
          ASSERT (FALSE);
        }
      }
    }
    MemWritePciCfgEp (Socket, Channel, MCDDRTCFG_MC_MAIN_REG, McDdrtCfg.Data);
  }
  IO_Reset (Host, Socket);

  return SUCCESS;
}

/**

  Apply WA settings before MemTest starts

  @param[in]     Socket                 - Socket number

  @retval EFI_UNSUPPORTED - Function is not supported
  @retval EFI_SUCCESS     - Otherwise

**/
EFI_STATUS
EFIAPI
MemTestWaInOrderMode (
  IN  UINT8    Socket
  )
{
  PSYSHOST                                  Host;
  UINT8                                     Ch;
  struct channelNvram                       (*ChannelNvList)[MAX_CH];
  MCCMIREQCPCFG_N0_MC_2LM_STRUCT           McCmiReqCpcfgN0;
  MCCMIREQCPCFG_N1_MC_2LM_STRUCT           McCmiReqCpcfgN1;

  MCSTRUCTCMICREDITCFG_N0_MC_2LM_STRUCT     McStructCmiCreditCfgN0;
  MCCMISTRUCTRDCREDITCFG_N0_MC_2LM_STRUCT   McCmiStructRdCreditCfgN0;
  MCCMISTRUCTRDCREDITCFG_N1_MC_2LM_STRUCT   McCmiStructRdCreditCfgN1;
  MCCMISTRUCTWRCREDITCFG_N0_MC_2LM_STRUCT   McCmiStructWrCreditCfgN0;
  MCCMISTRUCTWRCREDITCFG_N1_MC_2LM_STRUCT   McCmiStructWrCreditCfgN1;

  Host = GetSysHostPointer ();

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < GetMaxChDdr (); Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    // mc_2lmcntl_crnode.McCmiReqCpCfg.req_credit_cfg_done            <= 'b0
    // mc_2lmcntl_crnode.McCmiReqCpCfg.cpgc_req_credits_initialized   <= 'b0
    InitMcCmiReqCpCfg (Socket, Ch);

    // cpgc_v.CPGC2_CREDIT_CFG.CREDITS_CONFIG_DONE                    <= 'b0
    InitCpgcCreditConfigDone (Socket, Ch);

    // mcscheds_pq_crnode.scheduler_enables.cpgc_inorder_mode_en      <= 'b1
    // mcscheds_bs_crnode.scheduler_enables_bs.cpgc_inorder_mode_en   <= 'b1
    CpgcInorderModeEnable (Socket, Ch, TRUE);

    // mc_2lmcntl_crnode.McStructCmiCreditCfg.wpq4_1_shared_cnt       <= 16'h0
    // mc_2lmcntl_crnode.McStructCmiCreditCfg.wpq4_0_shared_cnt       <= 16'h0
    // mc_2lmcntl_crnode.McStructCmiCreditCfg.rpq4_1_shared_cnt       <= 16'h0
    // mc_2lmcntl_crnode.McStructCmiCreditCfg.rpq4_0_shared_cnt       <= 16'h0
    McStructCmiCreditCfgN0.Data = MemReadPciCfgEp (Socket, Ch, MCSTRUCTCMICREDITCFG_N0_MC_2LM_REG);
    McStructCmiCreditCfgN0.Bits.rpq4_0_shared_cnt = 0;
    McStructCmiCreditCfgN0.Bits.rpq4_1_shared_cnt = 0;
    McStructCmiCreditCfgN0.Bits.wpq4_0_shared_cnt = 0;
    McStructCmiCreditCfgN0.Bits.wpq4_1_shared_cnt = 0;
    MemWritePciCfgEp (Socket, Ch, MCSTRUCTCMICREDITCFG_N0_MC_2LM_REG, McStructCmiCreditCfgN0.Data);

    McCmiReqCpcfgN0.Data = MemReadPciCfgEp (Socket, Ch, MCCMIREQCPCFG_N0_MC_2LM_REG);
    McCmiReqCpcfgN1.Data = MemReadPciCfgEp (Socket, Ch, MCCMIREQCPCFG_N1_MC_2LM_REG);
    //1 credit
    McCmiReqCpcfgN0.Bits.rd_vc0_floor = 0x1;
    McCmiReqCpcfgN0.Bits.rd_vc1_floor = 0x0;
    McCmiReqCpcfgN0.Bits.rd_vc2_floor = 0x0;
    McCmiReqCpcfgN0.Bits.rd_vc3_floor = 0x0;
    McCmiReqCpcfgN0.Bits.wr_vc0_floor = 0x1;
    McCmiReqCpcfgN0.Bits.wr_vc1_floor = 0x0;
    McCmiReqCpcfgN0.Bits.wr_vc2_floor = 0x0;
    McCmiReqCpcfgN1.Bits.wr_vc2_floor = 0x0;
    McCmiReqCpcfgN1.Bits.wr_vc3_floor = 0x0;
    MemWritePciCfgEp (Socket, Ch, MCCMIREQCPCFG_N0_MC_2LM_REG, McCmiReqCpcfgN0.Data);
    MemWritePciCfgEp (Socket, Ch, MCCMIREQCPCFG_N1_MC_2LM_REG, McCmiReqCpcfgN1.Data);

    // mc_2lmcntl_crnode.McCmiStructRdCreditCfg.vc0_struct_credit_map <= 'h13
    // mc_2lmcntl_crnode.McCmiStructRdCreditCfg.vc1_struct_credit_map <= 'h0
    // mc_2lmcntl_crnode.McCmiStructRdCreditCfg.vc2_struct_credit_map <= 'h0
    // mc_2lmcntl_crnode.McCmiStructRdCreditCfg.vc3_struct_credit_map <= 'h0
    McCmiStructRdCreditCfgN0.Data = MemReadPciCfgEp (Socket, Ch, MCCMISTRUCTRDCREDITCFG_N0_MC_2LM_REG);
    McCmiStructRdCreditCfgN1.Data = MemReadPciCfgEp (Socket, Ch, MCCMISTRUCTRDCREDITCFG_N1_MC_2LM_REG);
    McCmiStructRdCreditCfgN0.Bits.vc0_struct_credit_map = 0x13;
    McCmiStructRdCreditCfgN0.Bits.vc1_struct_credit_map = 0x0;
    McCmiStructRdCreditCfgN0.Bits.vc2_struct_credit_map = 0x0;
    McCmiStructRdCreditCfgN1.Bits.vc2_struct_credit_map = 0x0;
    McCmiStructRdCreditCfgN1.Bits.vc3_struct_credit_map = 0x0;
    MemWritePciCfgEp (Socket, Ch, MCCMISTRUCTRDCREDITCFG_N0_MC_2LM_REG, McCmiStructRdCreditCfgN0.Data);
    MemWritePciCfgEp (Socket, Ch, MCCMISTRUCTRDCREDITCFG_N1_MC_2LM_REG, McCmiStructRdCreditCfgN1.Data);

    // mc_2lmcntl_crnode.McCmiStructWrCreditCfg.vc0_struct_credit_map <= 'h23
    // mc_2lmcntl_crnode.McCmiStructWrCreditCfg.vc1_struct_credit_map <= 'h0
    // mc_2lmcntl_crnode.McCmiStructWrCreditCfg.vc2_struct_credit_map <= 'h0
    // mc_2lmcntl_crnode.McCmiStructWrCreditCfg.vc3_struct_credit_map <= 'h0
    McCmiStructWrCreditCfgN0.Data = MemReadPciCfgEp (Socket, Ch, MCCMISTRUCTWRCREDITCFG_N0_MC_2LM_REG);
    McCmiStructWrCreditCfgN1.Data = MemReadPciCfgEp (Socket, Ch, MCCMISTRUCTWRCREDITCFG_N1_MC_2LM_REG);
    McCmiStructWrCreditCfgN0.Bits.vc0_struct_credit_map = 0x23;
    McCmiStructWrCreditCfgN0.Bits.vc1_struct_credit_map = 0x0;
    McCmiStructWrCreditCfgN0.Bits.vc2_struct_credit_map = 0x0;
    McCmiStructWrCreditCfgN1.Bits.vc2_struct_credit_map = 0x0;
    McCmiStructWrCreditCfgN1.Bits.vc3_struct_credit_map = 0x0;
    MemWritePciCfgEp (Socket, Ch, MCCMISTRUCTWRCREDITCFG_N0_MC_2LM_REG, McCmiStructWrCreditCfgN0.Data);
    MemWritePciCfgEp (Socket, Ch, MCCMISTRUCTWRCREDITCFG_N1_MC_2LM_REG, McCmiStructWrCreditCfgN1.Data);

    //mc_2lmcntl_crnode.McCmiReqCpCfg.req_credit_cfg_done            <= 'b1
    SetMcReqCreditCfgDone (Socket, Ch);

    //cpgc_v.CPGC2_CREDIT_CFG.CREDITS_CONFIG_DONE                    <= 'b1
    SetCpgcCreditConfigDone (Socket, Ch);
    // Wait until initialized by
    // Polling on mc_2lmcntl_crnode.McCmiReqCpCfg.cpgc_req_credits_initialized
    PollCpgcReqCreditsInitialized (Socket, Ch);

  } //Ch

  return EFI_SUCCESS;
}


/**

  Apply workarounds needed before MemInit

  @paramin] Socket      - Socket number

  @retval EFI_UNSUPPORTED - Function is not supported
  @retval EFI_SUCCESS     - Otherwise

**/
EFI_STATUS
EFIAPI
PreMemInit (
  IN  UINT8       Socket
  )
{
  PSYSHOST                                  Host;
  UINT8                                     MaxChDdr;
  UINT8                                     Ch;
  MCSTRUCTCMICREDITCFG_N0_MC_2LM_STRUCT     McStructCmiCreditCfgN0;
  MCCMISTRUCTRDCREDITCFG_N0_MC_2LM_STRUCT   McCmiStructRdCreditCfgN0;
  MCCMISTRUCTRDCREDITCFG_N1_MC_2LM_STRUCT   McCmiStructRdCreditCfgN1;
  MCCMISTRUCTWRCREDITCFG_N0_MC_2LM_STRUCT   McCmiStructWrCreditCfgN0;
  MCCMISTRUCTWRCREDITCFG_N1_MC_2LM_STRUCT   McCmiStructWrCreditCfgN1;

  //
  // For impacted silicon, CPGC writes leave corrupted state around
  // in mc_wdb when EnableCpgcInOrder=1.
  // Workaround is to disable in-order mode and leave it disabled
  // during MemInit (TestType = ADV_MT_TYPE_LINIT_SCRAM).
  //
  if (IsSiliconWorkaroundEnabled ("S14010044298")) {

    Host = GetSysHostPointer ();
    MaxChDdr = GetMaxChDdr ();
    //
    // For each populated channel, go to CPGC out-of-order mode
    //
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }
      // mc_2lmcntl_crnode.McCmiReqCpCfg.req_credit_cfg_done            <= 'b0
      // mc_2lmcntl_crnode.McCmiReqCpCfg.cpgc_req_credits_initialized   <= 'b0
      InitMcCmiReqCpCfg (Socket, Ch);

      // cpgc_v.CPGC2_CREDIT_CFG.CREDITS_CONFIG_DONE                    <= 'b0
      InitCpgcCreditConfigDone (Socket, Ch);

      // mcscheds_pq_crnode.scheduler_enables.cpgc_inorder_mode_en      <= 'b0
      // mcscheds_bs_crnode.scheduler_enables_bs.cpgc_inorder_mode_en   <= 'b0
      CpgcInorderModeEnable (Socket, Ch, FALSE);

      // mc_2lmcntl_crnode.McStructCmiCreditCfg.wpq4_1_shared_cnt       <= 16'h17
      // mc_2lmcntl_crnode.McStructCmiCreditCfg.wpq4_0_shared_cnt       <= 16'h17
      // mc_2lmcntl_crnode.McStructCmiCreditCfg.rpq4_1_shared_cnt       <= 16'h10
      // mc_2lmcntl_crnode.McStructCmiCreditCfg.rpq4_0_shared_cnt       <= 16'h10
      McStructCmiCreditCfgN0.Data = MemReadPciCfgEp (Socket, Ch, MCSTRUCTCMICREDITCFG_N0_MC_2LM_REG);
      McStructCmiCreditCfgN0.Bits.rpq4_0_shared_cnt = 0x10;
      McStructCmiCreditCfgN0.Bits.rpq4_1_shared_cnt = 0x10;
      McStructCmiCreditCfgN0.Bits.wpq4_0_shared_cnt = 0x17;
      McStructCmiCreditCfgN0.Bits.wpq4_1_shared_cnt = 0x17;
      MemWritePciCfgEp (Socket, Ch, MCSTRUCTCMICREDITCFG_N0_MC_2LM_REG, McStructCmiCreditCfgN0.Data);

      // mc_2lmcntl_crnode.McCmiStructRdCreditCfg.vc0_struct_credit_map <= 'h13
      // mc_2lmcntl_crnode.McCmiStructRdCreditCfg.vc1_struct_credit_map <= 'h0
      // mc_2lmcntl_crnode.McCmiStructRdCreditCfg.vc2_struct_credit_map <= 'h0
      // mc_2lmcntl_crnode.McCmiStructRdCreditCfg.vc3_struct_credit_map <= 'h0
      McCmiStructRdCreditCfgN0.Data = MemReadPciCfgEp (Socket, Ch, MCCMISTRUCTRDCREDITCFG_N0_MC_2LM_REG);
      McCmiStructRdCreditCfgN1.Data = MemReadPciCfgEp (Socket, Ch, MCCMISTRUCTRDCREDITCFG_N1_MC_2LM_REG);
      McCmiStructRdCreditCfgN0.Bits.vc0_struct_credit_map = 0x13;
      McCmiStructRdCreditCfgN0.Bits.vc1_struct_credit_map = 0x0;
      McCmiStructRdCreditCfgN0.Bits.vc2_struct_credit_map = 0x0;
      McCmiStructRdCreditCfgN1.Bits.vc2_struct_credit_map = 0x0;
      McCmiStructRdCreditCfgN1.Bits.vc3_struct_credit_map = 0x0;
      MemWritePciCfgEp (Socket, Ch, MCCMISTRUCTRDCREDITCFG_N0_MC_2LM_REG, McCmiStructRdCreditCfgN0.Data);
      MemWritePciCfgEp (Socket, Ch, MCCMISTRUCTRDCREDITCFG_N1_MC_2LM_REG, McCmiStructRdCreditCfgN1.Data);

      // mc_2lmcntl_crnode.McCmiStructWrCreditCfg.vc0_struct_credit_map <= 'h2c
      // mc_2lmcntl_crnode.McCmiStructWrCreditCfg.vc1_struct_credit_map <= 'h0
      // mc_2lmcntl_crnode.McCmiStructWrCreditCfg.vc2_struct_credit_map <= 'h0
      // mc_2lmcntl_crnode.McCmiStructWrCreditCfg.vc3_struct_credit_map <= 'h0
      McCmiStructWrCreditCfgN0.Data = MemReadPciCfgEp (Socket, Ch, MCCMISTRUCTWRCREDITCFG_N0_MC_2LM_REG);
      McCmiStructWrCreditCfgN1.Data = MemReadPciCfgEp (Socket, Ch, MCCMISTRUCTWRCREDITCFG_N1_MC_2LM_REG);
      McCmiStructWrCreditCfgN0.Bits.vc0_struct_credit_map = 0x2c;
      McCmiStructWrCreditCfgN0.Bits.vc1_struct_credit_map = 0x0;
      McCmiStructWrCreditCfgN0.Bits.vc2_struct_credit_map = 0x0;
      McCmiStructWrCreditCfgN1.Bits.vc2_struct_credit_map = 0x0;
      McCmiStructWrCreditCfgN1.Bits.vc3_struct_credit_map = 0x0;
      MemWritePciCfgEp (Socket, Ch, MCCMISTRUCTWRCREDITCFG_N0_MC_2LM_REG, McCmiStructWrCreditCfgN0.Data);
      MemWritePciCfgEp (Socket, Ch, MCCMISTRUCTWRCREDITCFG_N1_MC_2LM_REG, McCmiStructWrCreditCfgN1.Data);

      //mc_2lmcntl_crnode.McCmiReqCpCfg.req_credit_cfg_done            <= 'b1
      SetMcReqCreditCfgDone (Socket, Ch);

      //cpgc_v.CPGC2_CREDIT_CFG.CREDITS_CONFIG_DONE                    <= 'b1
      SetCpgcCreditConfigDone (Socket, Ch);
      // Wait until initialized by
      // Polling on mc_2lmcntl_crnode.McCmiReqCpCfg.cpgc_req_credits_initialized
      PollCpgcReqCreditsInitialized (Socket, Ch);
    } // Ch
  }
  return EFI_SUCCESS;
}

/**

  Save the default value of scheduler enables registers

  @param[in]       Socket                     -  Socket number
  @param[in,out]   SchedulerEnablesDefault    -  Pointer to a SCHEDULER_ENABLES_CONFIG_DEFAULT buffer

  @retval  N/A

**/
VOID
EFIAPI
SaveSchedulerEnablesConfig (
  IN      UINT8                             Socket,
  IN OUT  SCHEDULER_ENABLES_CONFIG_DEFAULT  *SchedulerEnablesDefault
  )
{
  UINT8      Ch;

  if (SchedulerEnablesDefault == NULL) {
    RcDebugPrint (SDBG_MINMAX, "Warning: SaveSchedulerEnablesConfig pointer is NULL\n");
    return;
  }

  for (Ch = 0; Ch < GetMaxChDdr (); Ch++) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    SchedulerEnablesDefault[Ch].SchedulerEnables   = MemReadPciCfgEp (Socket, Ch, SCHEDULER_ENABLES_MCDDC_CTL_REG);
    SchedulerEnablesDefault[Ch].SchedulerEnablesBs = MemReadPciCfgEp (Socket, Ch, SCHEDULER_ENABLES_BS_MCDDC_CTL_REG);
    SchedulerEnablesDefault[Ch].Valid = TRUE;
  }
}

/**

  Configure bank scheduler selection in scheduler enables registers

  @param[in]   Socket       -  Socket number
  @param[in]   Selection    -  Bank Scheduler Selection

  @retval  N/A

**/
VOID
EFIAPI
ConfigBankSchedulerSelection (
  IN  UINT8  Socket,
  IN  UINT8  Selection
  )
{
  UINT8                                   Ch;
  SCHEDULER_ENABLES_MCDDC_CTL_STRUCT      SchedulerEnables;
  SCHEDULER_ENABLES_BS_MCDDC_CTL_STRUCT   SchedulerEnablesBs;

  for (Ch = 0; Ch < GetMaxChDdr (); Ch++) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    SchedulerEnables.Data   = MemReadPciCfgEp (Socket, Ch, SCHEDULER_ENABLES_MCDDC_CTL_REG);
    SchedulerEnables.Bits.bank_scheduler_selection   = Selection;
    MemWritePciCfgEp (Socket, Ch, SCHEDULER_ENABLES_MCDDC_CTL_REG, SchedulerEnables.Data);

    SchedulerEnablesBs.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_ENABLES_BS_MCDDC_CTL_REG);
    SchedulerEnablesBs.Bits.bank_scheduler_selection = Selection;
    MemWritePciCfgEp (Socket, Ch, SCHEDULER_ENABLES_BS_MCDDC_CTL_REG, SchedulerEnablesBs.Data);
  }
}

/**

  Restore the default value of scheduler enables registers

  @param[in]   Socket                     -  Socket number
  @param[in]   SchedulerEnablesDefault    -  Pointer to a SCHEDULER_ENABLES_CONFIG_DEFAULT buffer

  @retval  N/A

**/
VOID
EFIAPI
RestoreSchedulerEnablesConfig (
  IN  UINT8                             Socket,
  IN  SCHEDULER_ENABLES_CONFIG_DEFAULT  *SchedulerEnablesDefault
  )
{
  UINT8      Ch;

  if (SchedulerEnablesDefault == NULL) {
    return;
  }

  for (Ch = 0; Ch < GetMaxChDdr (); Ch++) {

    if ((IsChannelEnabled (Socket, Ch) == FALSE) ||
        (SchedulerEnablesDefault[Ch].Valid == FALSE)) {
      continue;
    }
    MemWritePciCfgEp (Socket, Ch, SCHEDULER_ENABLES_MCDDC_CTL_REG, SchedulerEnablesDefault[Ch].SchedulerEnables);
    MemWritePciCfgEp (Socket, Ch, SCHEDULER_ENABLES_BS_MCDDC_CTL_REG, SchedulerEnablesDefault[Ch].SchedulerEnablesBs);
  }
}

/**

  Program bank refresh mode. It applies for all channels.

  @param[in] Socket            - Socket number
  @param[in] RefreshMode       - Bank refresh mode

  @retval  N/A

**/
VOID
SetPerBankMode (
  IN    UINT8      Socket,
  IN    UINT8      RefreshMode
)
{
  TCSTAGGER_REF_MCDDC_CTL_STRUCT        Tcstagger;
  UINT8                                 Ch;

  for (Ch = 0; Ch < GetMaxChDdr (); Ch++) {
    if (!IsChannelEnabled (Socket, Ch)) {
      continue;
    }
    Tcstagger.Data = MemReadPciCfgEp (Socket, Ch, TCSTAGGER_REF_MCDDC_CTL_REG);
    Tcstagger.Bits.perbank_mode = RefreshMode;
    MemWritePciCfgEp (Socket, Ch, TCSTAGGER_REF_MCDDC_CTL_REG, Tcstagger.Data);
  }
}

/**

  Get bank refresh mode setting. All channels have same bank refresh mode.

  @param[in]     Socket            - Socket number
  @param[in,out] RefreshMode       - Pointer to bank refresh mode setting

  @retval  N/A

**/
VOID
GetPerBankMode (
  IN      UINT8      Socket,
  IN OUT  UINT8      *RefreshMode
)
{
  TCSTAGGER_REF_MCDDC_CTL_STRUCT        Tcstagger;
  UINT8                                 Ch;

  for (Ch = 0; Ch < GetMaxChDdr (); Ch++) {
    if (!IsChannelEnabled (Socket, Ch)) {
      continue;
    }
    Tcstagger.Data = MemReadPciCfgEp (Socket, Ch, TCSTAGGER_REF_MCDDC_CTL_REG);
    *RefreshMode = (UINT8) Tcstagger.Bits.perbank_mode;
    return;
  }
}

/**

  Apply settings before Mem Test

  @param[in]      Socket         - Socket number
  @param[in, out] MemTestConfig  - Address of a pointer to MemTestConfig structure

  @retval  MRC_FUNCTION_NOT_IMPLEMENTED     - Function not implemented
           MRC_STATUS_FAILURE               - Pre-mem test execution failure
           MRC_STATUS_SUCCESS               - Otherwise

**/
MRC_STATUS
EFIAPI
PreMemTest (
  IN     UINT8            Socket,
  IN OUT MEM_TEST_CONFIG  **MemTestConfig
  )
{
  SCHEDULER_ENABLES_CONFIG_DEFAULT  *SchedulerEnablesDefault;
  CMI_CREDIT_CONFIG_DEFAULT         *CmiCreditConfigDefault;
  UINT8                             *RefreshModeDefault;

  *MemTestConfig = (MEM_TEST_CONFIG *) RcAllocatePool (sizeof (MEM_TEST_CONFIG));

  if (*MemTestConfig == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return MRC_STATUS_FAILURE;
  }

  ZeroMem ((UINT8 *) (*MemTestConfig), sizeof (MEM_TEST_CONFIG));

  RefreshModeDefault      = &((*MemTestConfig)->RefreshModeDefault);
  SchedulerEnablesDefault = &((*MemTestConfig)->SchedulerEnablesDefault[0]);
  CmiCreditConfigDefault  = &((*MemTestConfig)->CmiCreditConfigDefault);

  GetPerBankMode (Socket, RefreshModeDefault);
  SaveSchedulerEnablesConfig (Socket, SchedulerEnablesDefault);

  if (IsSiliconWorkaroundEnabled ("S1507902816")) {
    SaveCmiCreditConfig (Socket, CmiCreditConfigDefault);
    SetPerBankMode (Socket, REFRESH_ALL_BANK_MODE);
    ConfigBankSchedulerSelection (Socket, BANK_SCHEDULER_SELECTION_0);
    MemTestWaInOrderMode (Socket);
  } else {
    SetPerBankMode (Socket, REFRESH_PER_BANK_MODE);
    ConfigBankSchedulerSelection (Socket, BANK_SCHEDULER_SELECTION_1);
  }

  return MRC_STATUS_SUCCESS;
}

/**

  Apply settings after Mem Test

  @param[in] Socket         - Socket number
  @param[in] MemTestConfig  - Address of a pointer to MemTestConfig structure

  @retval  MRC_FUNCTION_NOT_IMPLEMENTED     - Function not implemented
           MRC_STATUS_FAILURE               - Post-mem test execution failure
           MRC_STATUS_SUCCESS               - Otherwise

**/
MRC_STATUS
EFIAPI
PostMemTest (
  IN  UINT8            Socket,
  IN  MEM_TEST_CONFIG  **MemTestConfig
  )
{
  SCHEDULER_ENABLES_CONFIG_DEFAULT  *SchedulerEnablesDefault;
  CMI_CREDIT_CONFIG_DEFAULT         *CmiCreditConfigDefault;
  UINT8                             *RefreshModeDefault;

  if (*MemTestConfig == NULL) {
    RcDebugPrint (SDBG_MINMAX, "Warning: MemTestConfig pointer is NULL\n");
    return MRC_STATUS_FAILURE;
  }

  RefreshModeDefault =      &((*MemTestConfig)->RefreshModeDefault);
  SchedulerEnablesDefault = &((*MemTestConfig)->SchedulerEnablesDefault[0]);
  CmiCreditConfigDefault =  &((*MemTestConfig)->CmiCreditConfigDefault);

  if (IsSiliconWorkaroundEnabled ("S1507902816")) {
    RestoreCmiCreditConfig (Socket, CmiCreditConfigDefault);
  }

  RestoreSchedulerEnablesConfig (Socket, SchedulerEnablesDefault);
  SetPerBankMode (Socket, *RefreshModeDefault);
  RcFreePool (*MemTestConfig);

  return MRC_STATUS_SUCCESS;
}

/**
  Set Burst Chop based on ADDDCEn value

  @param Host  - Pointer to sysHost

  @retval N/A
**/
VOID
SetBurstChop (
  PSYSHOST Host
  )
{
  //
  // keep DDR5 macro here to workaround KTISIM compiling errors.
  // TODO: remove macro after code enhancement later.
  // described in HSD: 1507862549
  //
#ifdef DDR5_SUPPORT
  UINT8                                        Socket;
  UINT8                                        Ch;
  UINT8                                        Dimm;
  UINT8                                        Rank;
  UINT8                                        MaxChDdr;
  struct channelNvram                          (*ChannelNvList)[MAX_CH];
  BOOLEAN                                      ScratchSet;
  TCMR2SHADOW_MCDDC_CTL_STRUCT                 TCMr2ShadowReg;
  DDR5_MODE_REGISTER_0_STRUCT                  Ddr5Mr0;

  Socket = Host->var.mem.currentSocket;
  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  if ((Host->nvram.mem.RASmodeEx & ADDDC_EN) == ADDDC_EN) {
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
          continue;
        }

        for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          SetBurstChopDdr5 (Socket, Ch, Dimm, Rank);

          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "MR0 = on-the-fly-burst\n");

        } // Rank loop
      } // Dimm loop
    } // Ch loop
  } // ADDDC_EN

  //
  // Set TCMR2SHADOW.SCRATCH[1:0] with BL value from MR0 for validation purpose
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    ScratchSet = FALSE;
    for (Dimm = 0; Dimm < MAX_DIMM && !ScratchSet; Dimm++) {
      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

      for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm && !ScratchSet; Rank++) {
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        ReadDramModeRegCachePerRank (Socket, Ch, SUB_CH_A, Dimm, Rank, DDR5_MODE_REGISTER_0_REG, &Ddr5Mr0.Data);
        TCMr2ShadowReg.Data = (*ChannelNvList)[Ch].TCMr2Shadow;
        TCMr2ShadowReg.Bits.scratch &= ~ (BIT1 | BIT0);
        TCMr2ShadowReg.Bits.scratch |= Ddr5Mr0.Bits.burst_length & (BIT1 | BIT0);
        (*ChannelNvList)[Ch].TCMr2Shadow = TCMr2ShadowReg.Data;
        MemWritePciCfgEp (Socket, Ch, TCMR2SHADOW_MCDDC_CTL_REG, (*ChannelNvList)[Ch].TCMr2Shadow);

        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Set MR0 to TCMr2Shadow\n");

        ScratchSet = TRUE;
      } // Rank loop
    } // Dimm loop
  } // Ch loop
#endif
}

/**

  Set for DDRT mode.

  @param Socket         - Socket Id
  @param Ch             - channel Id
  @param ForceDdrt      - BOOLEAN TRUE: Force to DDRT mode; FALSE: Force DDR mode


  @retval N/A

**/
VOID
SetDdrtForceMode (
  UINT8           Socket,
  UINT8           Ch,
  BOOLEAN         ForceDdrt
  )
{
  T_DDRT_MISC_DELAY_MC_2LM_STRUCT   DdrtMiscDelay;
  MC_CPGC_CTL_MC_MAIN_STRUCT        McCpgcCtl;

  DdrtMiscDelay.Data  = MemReadPciCfgEp (Socket, Ch, T_DDRT_MISC_DELAY_MC_2LM_REG);
  McCpgcCtl.Data      = MemReadPciCfgEp (Socket, Ch, MC_CPGC_CTL_MC_MAIN_REG);

  if (ForceDdrt) {
    DdrtMiscDelay.Bits.force_ddr4_mode = 0;
    DdrtMiscDelay.Bits.force_ddrt_mode = 1;
    McCpgcCtl.Bits.cpgc_mixed_ddrt_rank_en = 1;
  } else {
    DdrtMiscDelay.Bits.force_ddr4_mode = 1;
    DdrtMiscDelay.Bits.force_ddrt_mode = 0;
    McCpgcCtl.Bits.cpgc_mixed_ddrt_rank_en = 0;
  }

  MemWritePciCfgEp (Socket, Ch, T_DDRT_MISC_DELAY_MC_2LM_REG, DdrtMiscDelay.Data);
  MemWritePciCfgEp (Socket, Ch, MC_CPGC_CTL_MC_MAIN_REG , McCpgcCtl.Data);
}

/**

  Return TRUE or FALSE if in DDRT2 Mode

  @param Host           - Pointer to sysHost
  @param Socket         - Socket Id
  @param Ch             - channel Id

  @retval TRUE/FALSE

**/
BOOLEAN
GetDdrt2Mode (
  PSYSHOST        Host,
  UINT8           Socket,
  UINT8           Ch
  )
{
  T_DDRT_MISC_DELAY_MC_2LM_STRUCT   DdrtMiscDelay;

  DdrtMiscDelay.Data  = MemReadPciCfgEp (Socket, Ch, T_DDRT_MISC_DELAY_MC_2LM_REG);

  if (DdrtMiscDelay.Bits.force_ddrt_mode == 1) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**

  clear for mode in the misc reg.

  @param Socket         - Socket Id
  @param Ch             - channel Id

  @retval N/A

**/
VOID
ClearDdrtForceMode (
  UINT8           Socket,
  UINT8           Ch
  )
{
  T_DDRT_MISC_DELAY_MC_2LM_STRUCT   DdrtMiscDelay;
  MC_CPGC_CTL_MC_MAIN_STRUCT        McCpgcCtl;

  DdrtMiscDelay.Data  = MemReadPciCfgEp (Socket, Ch, T_DDRT_MISC_DELAY_MC_2LM_REG);
  McCpgcCtl.Data      = MemReadPciCfgEp (Socket, Ch, MC_CPGC_CTL_MC_MAIN_REG);

  DdrtMiscDelay.Bits.force_ddr4_mode = 0;
  DdrtMiscDelay.Bits.force_ddrt_mode = 0;
  McCpgcCtl.Bits.cpgc_mixed_ddrt_rank_en = 0;

  MemWritePciCfgEp (Socket, Ch, T_DDRT_MISC_DELAY_MC_2LM_REG, DdrtMiscDelay.Data);
  MemWritePciCfgEp (Socket, Ch, MC_CPGC_CTL_MC_MAIN_REG , McCpgcCtl.Data);
}


/**
  Get the ERF_DDR4_CMD_REG address.

  @param[in] Index  ERF_DDR4_CMD_REG register index.

  @retval N/A
**/
UINT32
GetErfDdr4CmdRegAddress (
  UINT8  Index
  )
{
  UINT32 Registers [ERF_DDR4_CMD_REG_NUM] = {
           ERF_DDR4_CMD_REG0_MCDDC_CTL_REG,
           ERF_DDR4_CMD_REG1_MCDDC_CTL_REG,
           ERF_DDR4_CMD_REG2_MCDDC_CTL_REG,
           ERF_DDR4_CMD_REG3_MCDDC_CTL_REG,
           ERF_DDR4_CMD_REG4_MCDDC_CTL_REG,
           ERF_DDR4_CMD_REG5_MCDDC_CTL_REG,
           ERF_DDR4_CMD_REG6_MCDDC_CTL_REG,
           ERF_DDR4_CMD_REG7_MCDDC_CTL_REG,
           ERF_DDR4_CMD_REG8_MCDDC_CTL_REG,
           ERF_DDR4_CMD_REG9_MCDDC_CTL_REG,
           ERF_DDR4_CMD_REG10_MCDDC_CTL_REG,
           ERF_DDR4_CMD_REG11_MCDDC_CTL_REG,
           ERF_DDR4_CMD_REG12_MCDDC_CTL_REG,
           ERF_DDR4_CMD_REG13_MCDDC_CTL_REG,
           ERF_DDR4_CMD_REG14_MCDDC_CTL_REG,
           ERF_DDR4_CMD_REG15_MCDDC_CTL_REG
           };

  return Registers[Index];
}

/**
  Get the ERF_DDR4_CMD_REG register numbers.

  @param  N/A

  @retval N/A
**/
UINT32
GetErfDdr4CmdRegNum (
  )
{
  return ERF_DDR4_CMD_REG_NUM;
}

/**
  Program 32 to 39 bits of Wpq Pch Entry Enable registers

  @param[in] Socket   -  Zero based Socket number

  retval N/A
**/
VOID
EFIAPI
ProgramWpqPchEntryEnBits32To39 (
  IN UINT8 Socket
  )
{
#ifndef GNRSRF_HOST
  UINT8                                                  MemoryMode = GetVolMemMode ();
  UINT8                                                  MaxChDdr = GetMaxChDdr ();
  UINT8                                                  MaxChPerMc = GetNumChannelPerMc ();
  PSYSHOST                                               Host = GetSysHostPointer ();
  UINT8                                                  Ch;
  UINT8                                                  Dimm;
  BOOLEAN                                                AllDdr5Dimm;
  struct dimmNvram                                       (*DimmNvList) [MAX_DIMM];
  SCHEDULER_WPQ_PCH0_ENTRY_EN_N1_MCDDC_CTL_STRUCT        WpqPch0EntryEnN1Ctl;
  SCHEDULER_WPQ_PCH1_ENTRY_EN_N1_MCDDC_CTL_STRUCT        WpqPch1EntryEnN1Ctl;
  MCWDB_CHKN_BIT_MCDDC_DP_SPRC0_SPRMCC_SPRUCC_STRUCT     McwdbChknBitMcddcDpSprC0;

  for (Ch = 0; Ch < MaxChDdr; Ch ++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    if (MemoryMode == VOL_MEM_MODE_1LM) {
      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      AllDdr5Dimm = TRUE;

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }

        if ((*DimmNvList)[Dimm].DcpmmPresent) {
          AllDdr5Dimm = FALSE;
          break;
        }
      }

      if (AllDdr5Dimm) {
        PcuReadCrProxyReg (Socket, Ch / MaxChPerMc, Ch % MaxChPerMc, MAILBOX_BIOS_CMD_CR_PROXY_WPQ_PCH0_ENTRY_EN_N1_CTL_INDEX, &WpqPch0EntryEnN1Ctl.Data);
        PcuReadCrProxyReg (Socket, Ch / MaxChPerMc, Ch % MaxChPerMc, MAILBOX_BIOS_CMD_CR_PROXY_WPQ_PCH1_ENTRY_EN_N1_CTL_INDEX, &WpqPch1EntryEnN1Ctl.Data);
        McwdbChknBitMcddcDpSprC0.Data = MemReadPciCfgEp (Socket, Ch, MCWDB_CHKN_BIT_MCDDC_DP_REG);
        WpqPch0EntryEnN1Ctl.Bits.wpq_pch0_entry_en = 0xFF;
        WpqPch1EntryEnN1Ctl.Bits.wpq_pch1_entry_en = 0xFF;
        McwdbChknBitMcddcDpSprC0.Bits.dis_ddrt_wdb = 1;
        PcuWriteCrProxyReg (Socket, Ch / MaxChPerMc, Ch % MaxChPerMc, MAILBOX_BIOS_CMD_CR_PROXY_WPQ_PCH0_ENTRY_EN_N1_CTL_INDEX, WpqPch0EntryEnN1Ctl.Data);
        PcuWriteCrProxyReg (Socket, Ch / MaxChPerMc, Ch % MaxChPerMc, MAILBOX_BIOS_CMD_CR_PROXY_WPQ_PCH1_ENTRY_EN_N1_CTL_INDEX, WpqPch1EntryEnN1Ctl.Data);
        MemWritePciCfgEp (Socket, Ch, MCWDB_CHKN_BIT_MCDDC_DP_REG, McwdbChknBitMcddcDpSprC0.Data);
      }
    }
  }
#endif //GNRSRF_HOST
}