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
#include "Chip10nm/Include/MemHostChip10nm.h"
#include <Library/ProcMemInitChipLib.h>
#include <SysHostChip.h>
#include <UncoreCommonIncludes.h>
#include "Include/MemMcRegs.h"
#include "Include/MemMcIpLibInternal.h"
#include <Library/SysHostPointerLib.h>
#include <Library/MemRcLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/KtiApi.h>
#include <Library/SystemInfoLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemMcIpTargetLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemAccessLib.h>
#include <Library/MemCallTableLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/MemRoutingLib.h>
#include <MemCommon.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <MemProjectSpecific.h>
#include <Library/MemTurnaroundTimingLib.h>
#include <Library/MemThermManageLib.h>
#include <Library/Ddr5CoreLib.h>
#include <Library/CpuAndRevisionLib.h>

#define MAXFIELDVAL(bitfield)     (bitfield = 0xfff)
//
// Performance settings
//
#define RMM_OPP_TIMER               0xF
#define RMM_OPP_WRITE_LIMIT         0xF
#define RMM_OPP_TIMER_GRAN          0x2
#define RMM_OPP_WRITE_LIMIT_GRAN    0x0

#define RMM_OPP_TIMER_B0            0x10
#define RMM_OPP_WRITE_LIMIT_B0      0x2
#define RMM_OPP_WRITE_LIMIT_GRAN_B0 0x2

extern UINT8  PiTicks;

// Local Prototypes

VOID
SetRdimmTimingCntl10nm (
  IN PSYSHOST                          Host,
  IN UINT8                             Socket,
  IN UINT8                             Ch,
  IN RDIMMTIMINGCNTL_MCDDC_CTL_STRUCT  *RdimmTimingCntl,
  IN UINT8                             FmcType
); // LOCAL

STATIC UINT32
GettSTABAEP10nm (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    FmcType
); // LOCAL

VOID
Ddr3MrsTimingReg10nm (
  IN PSYSHOST Host,
  IN UINT8 socket,
  IN UINT8 ch
  );

VOID
SaveRefreshRate10nm (
  IN PSYSHOST Host,
  IN UINT8 socket,
  IN UINT8 ch,
  IN UINT8 refreshRate
  );

/**
  Program Stagger Refresh enables

  @param[in] Host     - Point to sysHost
  @param[in] Socket   - Socket number
  @param[in] Ch       - Channel number

  @retval N/A

**/
VOID
SetRefreshMiscEnables (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
  );

/**
  Program tXS_DLL timing for a channel

  @param[in] Host     - Point to sysHost
  @param[in] Socket   - Socket number
  @param[in] Channel  - Channel number

  @retval N/A
**/
VOID
ProgramTxsdll (
  IN  PSYSHOST   Host,
  IN  UINT8      Socket,
  IN  UINT8      Channel
  );

VOID
SetMiscDelay10nm (
  IN PSYSHOST Host,
  IN UINT8 socket,
  IN UINT8 ch,
  IN UINT8 cmdTiming
  );

VOID
SetMasterDllWakeUpTimer10nm (
  IN PSYSHOST Host,
  IN UINT8 socket,
  IN UINT8 ch
  );

UINT8
EFIAPI
GetNumberOfLogicalRanksPerChannel (
  IN UINT8 Socket,
  IN UINT8 Ch
  );


/**
  Program timing registers related to read major mode

  @param Host        - Pointer to sysHost
  @param socket      - Socket number
  @param ch          - Channel number

  @retval N/A
**/
VOID
InitReadMajorMode (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  )
{
  SCHEDULER_RMM_MCDDC_CTL_STRUCT              schedulerRmm;

  schedulerRmm.Data = MemReadPciCfgEp (socket, ch, SCHEDULER_RMM_MCDDC_CTL_REG);

  schedulerRmm.Bits.rmm_write_hit_en             = 1;

  if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) {
#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Woverflow"
#endif  //__GNUC__
// UINT32 rmm_opp_timer : 4, RMM_OPP_TIMER_B0 0x10 rmm_opp_timer can not hold
// > 0xF   0x10 > 0xF causing -Woverflow error by GCC
    schedulerRmm.Bits.rmm_opp_timer                = RMM_OPP_TIMER_B0;
    schedulerRmm.Bits.rmm_opp_write_limit          = RMM_OPP_WRITE_LIMIT_B0;
    schedulerRmm.Bits.rmm_opp_write_limit_gran     = RMM_OPP_WRITE_LIMIT_GRAN_B0;
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif  //__GNUC__
  } else {
    schedulerRmm.Bits.rmm_opp_timer                = RMM_OPP_TIMER;
    schedulerRmm.Bits.rmm_opp_write_limit          = RMM_OPP_WRITE_LIMIT;
    schedulerRmm.Bits.rmm_opp_write_limit_gran     = RMM_OPP_WRITE_LIMIT_GRAN;
  }
  schedulerRmm.Bits.ignore_opp_wr_on_starved_rmm = 1;
  schedulerRmm.Bits.rmm_opp_timer_gran           = RMM_OPP_TIMER_GRAN;
  schedulerRmm.Bits.rpq_empty_opp_wr_en          = 1;
  schedulerRmm.Bits.rmm_leaky_bucket_en          = 0;
#if (MEM_IP_VER >= MEM_IP_17ww37e)
  schedulerRmm.Bits.rmm_starved_exit_selection   = 1;
#endif

  MemWritePciCfgEp (socket, ch, SCHEDULER_RMM_MCDDC_CTL_REG, schedulerRmm.Data);
}

/**

  Sets new tREFI value (refresh interval) and returns previous value

  @param[in]  Host            Pointer to sysHost
  @param[in]  Socket          Socket number
  @param[in]  Ch              Channel number
  @param[in]  Trefi           New tREFI value
  @param[out] *PrevTrefi      Pointer to return previous tREFI value

  @reval      N/A

**/
VOID
GetSetTrefi (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Ch,
  IN  UINT16   Trefi,
  OUT UINT16   *PrevTrefi
  )
{
  TCRFTP_MCDDC_CTL_STRUCT                     tcrftp;

  tcrftp.Data = MemReadPciCfgEp (Socket, Ch, TCRFTP_MCDDC_CTL_REG);
  *PrevTrefi = (UINT16) tcrftp.Bits.t_refi;
  tcrftp.Bits.t_refi = Trefi;
  MemWritePciCfgEp (Socket, Ch, TCRFTP_MCDDC_CTL_REG, tcrftp.Data);
}

/**
  Return override value for tREFI, if any

  @param[out] tREFI     - Set to override value if supported

  @retval MRC_STATUS_SUCCESS if tREFI override had been updated and is valid
  @retval MRC_STATUS_FAILURE otherwise - tREFI has not been updated
**/
MRC_STATUS
GetTrefiOverride (
  OUT UINT32      *tREFI
  )
{
  SYS_SETUP         *Setup;

  Setup = GetSysSetupPointer ();

  if ((PcdGetBool (PcdMrcXmpSupport))         &&
      (Setup->mem.options & MEM_OVERRIDE_EN)  &&
      (Setup->mem.inputMemTime.tREFI)) {

    *tREFI = Setup->mem.inputMemTime.tREFI;
    RcDebugPrint (SDBG_MAX, "tREFI override = %d\n", Setup->mem.inputMemTime.tREFI);
    return MRC_STATUS_SUCCESS;
  }
  return MRC_STATUS_FAILURE;
}

/**
  Initialize the refresh rate based on policy input

  @param[in] Host                - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket              - Socket Id
  @param[in] Ch                  - Channel number (0-based)

  @retval N/A

**/
VOID
InitializeRefreshRate (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch
  )
{
  SYS_SETUP         *Setup;
  UINT8             RefreshRate;

  Setup = GetSysSetupPointer ();

  //
  // Establish Refresh Rate
  // Units in 0.1x of standard 7.8 usec interval
  //
  //
  // Default to legacy setting of 1x
  //
  RefreshRate = 10;

  if (IsCustomRefreshRate () == SUCCESS) {

    if (Disable2xRefreshCh (Host, Socket, Ch)) {

      OutputWarning (WARN_THROT_INSUFFICIENT, WARN_CUSTOM_REFRESH_RATE_REVERTED, Socket, Ch, NO_DIMM, NO_RANK);

    } else {
      //
      // Set refresh rate based on policy input
      //
      RefreshRate = Setup->mem.customRefreshRate;
    }
  }

  //
  // Save Refresh Rate for later use
  //
  SaveRefreshRate10nm (Host, Socket, Ch, RefreshRate);

}

/**
  Program timing registers related to refresh

  @param Host        - Pointer to sysHost
  @param socket      - Socket number
  @param ch          - Channel number

  @retval N/A
**/
VOID
ProgramRefreshTimings (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch
  )
{
  //
  // Program refresh timings based on technology and SOC
  //
  ProgramRefreshTimingsChip (Host, Socket, Ch);

  SetRefreshMiscEnables (Host, Socket, Ch);
  //
  // Timing Constraints Refresh Timing Parameters
  //
  ProgramRefreshTimingsWaterMark (Socket, Ch);

  //
  // Timing for Exit Self-Refresh to next valid command requiring a DLL
  //
  ProgramTxsdll (Host, Socket, Ch);
}

/**
  Return the number of logical ranks active in the input channel

  @param[in] Socket      - Socket number
  @param[in] Ch          - Channel number

  @retval UINT8 number of logical ranks in the channel
**/
UINT8
EFIAPI
GetNumberOfLogicalRanksPerChannel (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
  PSYSHOST Host;
  struct channelNvram   (*ChannelNvList)[MAX_CH];
  struct dimmNvram      (*DimmNvList)[MAX_DIMM];
  UINT8                 NumberOfLogicalRanks = 0;
  UINT8                 Dimm = 0;

  Host = GetSysHostPointer ();

  ChannelNvList = GetChannelNvList (Host, Socket);
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  if ((*ChannelNvList)[Ch].encodedCSMode != 2) {
    //for non-3DS dimms, the number of logical ranks is the same as the number of package ranks
    NumberOfLogicalRanks = (*ChannelNvList)[Ch].numRanks;
  } else {
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      NumberOfLogicalRanks += ((*DimmNvList)[Dimm].numDramRanks * (*DimmNvList)[Dimm].dieCount);
    }
  }

  return NumberOfLogicalRanks;
}


/**
  Overrides the global command timing mode variable setting if required

  Checks if there is any silicon specific requirement to override the
  command timing mode (i.e. a silicon workaround) and overrides the
  timing mode variable if required.

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket
  @param[in] Channel   - Channel

  @retval N/A
**/
VOID
EFIAPI
SiliconSpecificOverrideCommandTimingMode (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Channel
  )
{
  struct channelNvram (*ChannelNvList)[MAX_CH];
  ChannelNvList = GetChannelNvList (Host, Socket);

  if (IsSiliconWorkaroundEnabled ("S1706869133") &&
      ((*ChannelNvList)[Channel].numQuadRanks == 2) &&
      ((*ChannelNvList)[Channel].timingMode < TIMING_2N)) {
    (*ChannelNvList)[Channel].timingMode = TIMING_2N;
  } //Silicon Workaround S1706869133
}

/**
  Initialize the CMD timing mode registers
  If skipping training, go straight to the normal CMD timing.
  Otherwise, start with the training timing.

  @param Host        - Pointer to sysHost
  @param Socket      - Socket number
  @param Channel          - Channel number

  @retval N/A
**/
VOID
InitCmdTiming (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Channel
  )
{
  EFI_STATUS                                  Status;
  UINT32                                      StretchMode;
  UINT32                                      OriginalCmdStretch;
  MEMORY_TIMINGS_ADJ_MCDDC_CTL_STRUCT         memTimingsAdj;
  struct channelNvram                         (*channelNvList)[MAX_CH];
  BootMode                                    SysBootMode;

  channelNvList = GetChannelNvList (Host, Socket);

  SysBootMode = GetSysBootMode ();

  memTimingsAdj.Data = MemReadPciCfgEp (Socket, Channel, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG);

  OriginalCmdStretch = memTimingsAdj.Bits.cmd_stretch;

  Status = CteGetOptions ("stretchmode", &StretchMode);
  if (!EFI_ERROR (Status)) {
    if (StretchMode == 1) {
      (*channelNvList) [Channel].trainTimingMode = TIMING_1N;
      (*channelNvList) [Channel].timingMode = TIMING_1N;
    } else if (StretchMode == 2) {
      (*channelNvList) [Channel].trainTimingMode = TIMING_2N;
      (*channelNvList) [Channel].timingMode = TIMING_2N;
    } else if (StretchMode == 3) {
      (*channelNvList) [Channel].trainTimingMode = TIMING_3N;
      (*channelNvList) [Channel].timingMode = TIMING_3N;
    }
    memTimingsAdj.Bits.cmd_stretch = (*channelNvList) [Channel].trainTimingMode;
  } else {
    if (SysBootMode == S3Resume ||
        ((SysBootMode == NormalBoot) && (Host->var.mem.subBootMode != ColdBoot))) {

      memTimingsAdj.Bits.cmd_stretch = (*channelNvList) [Channel].timingMode;

    } else {
      //
      // Set 3N timing for basic training
      //
      (*channelNvList) [Channel].trainTimingMode = TIMING_3N;

#if DDR5_SUPPORT
      if (IsBrsPresent (Host, Socket)) {
        //
        // BRS--set it as 2N SDR
        //
        (*channelNvList) [Channel].trainTimingMode = TIMING_2N;
      } else if (Host->nvram.mem.dimmTypePresent == RDIMM) {
        //
        // SPR RDIMM/LRDIMM--set it as 1N
        //
        (*channelNvList) [Channel].trainTimingMode = TIMING_1N;
      } else if ((Host->nvram.mem.dimmTypePresent == UDIMM) || (Host->nvram.mem.dimmTypePresent == SODIMM)) {
        //
        // SPR UDIMM/SODIMM--set it as TBD SPR_TODO.
        //
        (*channelNvList) [Channel].trainTimingMode = TIMING_2N;
      }
#endif

      memTimingsAdj.Bits.cmd_stretch = (*channelNvList) [Channel].trainTimingMode;
    }
  }

  OverrideMemTimingStretchTarget (Host, Socket, Channel, OriginalCmdStretch, (VOID *)&memTimingsAdj);

  if (UbiosGenerationOrHsleEnabled () || (GetEmulation () == SIMICS_FLAG)) {
    //
    // For Ubios, Hybrid System Level Emulation and Simics, which skip the training steps,
    // go straight to the normal CMD timing.
    //
    memTimingsAdj.Bits.cmd_stretch = (*channelNvList) [Channel].timingMode;
  }

  SetMiscDelay10nm (Host, Socket, Channel, (UINT8) memTimingsAdj.Bits.cmd_stretch);

  //
  // For Ubios, Hybrid System Level Emulation and Simics Emulation, which skip the training steps,
  // the value set here is Noraml CMD Timing, which is also the final command timing.
  //
  // For boot paths which execute training steps, such as RcSim and real post silicon, the value
  // set here is Training Timing (trainTimingMode) for DDR training purpose. Normal CMD Timing
  // will be set by SetNormalCMDTiming at late staget of training, such as at CmdVrefCentering.
  //
  MemWritePciCfgEp (Socket, Channel, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG, memTimingsAdj.Data);

}

/**
  Initialize timings specific to RDIMM

  @param Host        - Pointer to sysHost
  @param socket      - Socket number
  @param ch          - Channel number

  @retval N/A
**/
VOID
InitRdimmTimings (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  )
{
  RDIMMTIMINGCNTL_MCDDC_CTL_STRUCT        RdimmTimingCntl;
  RDIMMTIMINGCNTL2_MCDDC_CTL_STRUCT       RdimmTimingCntl2;
  struct channelNvram                     (*ChannelNvList)[MAX_CH];
  UINT8                                   FmcType;

  ChannelNvList = GetChannelNvList (Host, socket);

  RdimmTimingCntl.Data = MemReadPciCfgEp (socket, ch, RDIMMTIMINGCNTL_MCDDC_CTL_REG);
  RdimmTimingCntl2.Data = MemReadPciCfgEp (socket, ch, RDIMMTIMINGCNTL2_MCDDC_CTL_REG);

  RdimmTimingCntl.Bits.t_stab = GettCKSRX (Host, socket);

  if (Host->nvram.mem.dimmTypePresent == RDIMM) {
    FmcType = GetFmcType (socket, ch);
    SetRdimmTimingCntl10nm (Host, socket, ch, &RdimmTimingCntl, FmcType);
  } // if RDIMM

  //
  // RDIMM Timing Control 2
  //
  if (Host->nvram.mem.DcpmmPresent == 1) {
    RdimmTimingCntl2.Bits.t_ckoff = NVMDIMM_TIMING_CNTL2_TCKOFF;
  } else {
    if (Host->nvram.mem.dimmTypePresent == RDIMM) {
      RdimmTimingCntl2.Bits.t_ckoff = RDIMM_TIMING_CNTL2_TCKOFF;
    } else {
      RdimmTimingCntl2.Bits.t_ckoff = RdimmTimingCntl.Bits.t_stab;
    }
  }

  if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_2133) {
    RdimmTimingCntl2.Bits.t_ckev = RDIMM_TIMING_CNTL2_TCKEV_DDR_2133;
  } else if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_2666){
    RdimmTimingCntl2.Bits.t_ckev = RDIMM_TIMING_CNTL2_TCKEV_DDR_2666;
  } else {
    RdimmTimingCntl2.Bits.t_ckev = RDIMM_TIMING_CNTL2_TCKEV;
  }

  MemWritePciCfgEp (socket, ch, RDIMMTIMINGCNTL2_MCDDC_CTL_REG, RdimmTimingCntl2.Data);
  MemWritePciCfgEp (socket, ch, RDIMMTIMINGCNTL_MCDDC_CTL_REG, RdimmTimingCntl.Data);

  SprTstabDdrt2(Host, socket, ch, RdimmTimingCntl.Bits.t_stab);
}

/**
  Program ZQ calibration timing registers

  @param Host        - Pointer to sysHost
  @param socket      - Socket number
  @param ch          - Channel number

  @retval N/A
**/
VOID
InitZqCalTiming (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  )
{
  SCHEDULER_CMD_OPCODE_DEBUG_MCDDC_CTL_STRUCT SchedulerCmdOpcodeDebug;
  MCMNT_CHKN_BIT_MCDDC_CTL_STRUCT             McntChknBit;
  TCZQCAL_MCDDC_CTL_STRUCT                    tczqcal;
  struct channelNvram                         (*ChannelNvList)[MAX_CH];
  SYS_SETUP                                   *Setup;

  Setup = GetSysSetupPointer ();

  if (Setup->mem.dfxMemSetup.DfxPeriodicZqCal && IsSiliconWorkaroundEnabled ("S14010065547")) {
    //
    // WA for MC scheduler not spacing commands correctly after issuing MPC
    //
    SchedulerCmdOpcodeDebug.Data = MemReadPciCfgEp (socket, ch, SCHEDULER_CMD_OPCODE_DEBUG_MCDDC_CTL_REG);
    SchedulerCmdOpcodeDebug.Bits.cmd_block_act = 3;
    SchedulerCmdOpcodeDebug.Bits.cmd_block_pre = 3;
    SchedulerCmdOpcodeDebug.Bits.cmd_block_rcas = 3;
    SchedulerCmdOpcodeDebug.Bits.cmd_block_rcas_pre = 3;
    SchedulerCmdOpcodeDebug.Bits.cmd_block_wcas = 3;
    SchedulerCmdOpcodeDebug.Bits.cmd_block_wcas_pre = 3;
    MemWritePciCfgEp (socket, ch, SCHEDULER_CMD_OPCODE_DEBUG_MCDDC_CTL_REG, SchedulerCmdOpcodeDebug.Data);
  }

  //
  // Enable/disable Periodic ZQCal according to DFX option
  //
  McntChknBit.Data = MemReadPciCfgEp (socket, ch, MCMNT_CHKN_BIT_MCDDC_CTL_REG);
  if (Setup->mem.dfxMemSetup.DfxPeriodicZqCal) {
    McntChknBit.Bits.dis_zqcal = 0; // enable Periodic ZQCal
  } else {
    McntChknBit.Bits.dis_zqcal = 1; // disable Periodic ZQCal
  }
  MemWritePciCfgEp (socket, ch, MCMNT_CHKN_BIT_MCDDC_CTL_REG, McntChknBit.Data);

  ChannelNvList = GetChannelNvList (Host, socket);

  tczqcal.Data = MemReadPciCfgEp (socket, ch, TCZQCAL_MCDDC_CTL_REG);
  if (IsDdr5Present (Host, socket)) {
    if ((*ChannelNvList)[ch].ddrtEnabled == 1) {
      tczqcal.Bits.t_zqcs = MAX (NanosecondToClocks (socket, DDR5_TZQLAT_NS), DDRT2_TZQLAT_MIN_DCLK);
    } else {
      tczqcal.Bits.t_zqcs = MAX (NanosecondToClocks (socket, DDR5_TZQLAT_NS), DDR5_TZQLAT_MIN_DCLK);
    }
  } else {
    tczqcal.Bits.t_zqcs = tZQCS_DDR4;
  }
  tczqcal.Bits.zqcsperiod = ZQCS_PERIOD;
  MemWritePciCfgEp (socket, ch, TCZQCAL_MCDDC_CTL_REG, tczqcal.Data);

  InitZqCalTimingChip (Host, socket, ch);
}

/**
  Program page policy registers

  @param Host        - Pointer to sysHost
  @param socket      - Socket number
  @param ch          - Channel number

  @retval N/A
**/
VOID
InitPagePolicy (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  )
{
  SCHEDULER_IDLETIME_MCDDC_CTL_STRUCT         IdleTime;
  SCHEDULER_IDLETIME2_MCDDC_CTL_STRUCT        IdleTime2;

  IdleTime.Data = MemReadPciCfgEp (socket, ch, SCHEDULER_IDLETIME_MCDDC_CTL_REG);
  IdleTime2.Data = MemReadPciCfgEp (socket, ch, SCHEDULER_IDLETIME2_MCDDC_CTL_REG);

  IdleTime.Bits.ppc_th = PPC_TH;
  IdleTime.Bits.opc_th = OPC_TH;

  IdleTime2.Bits.idle_page_rst_val = IDLE_PAGE_RST_VAL_MAX;
  IdleTime2.Bits.win_size = WIN_SIZE;
  //
  // Set to open page mode for training
  // Closed page modes: 0x0, Open page modes: 0x1
  //
  IdleTime2.Bits.adapt_pg_clse = 1;
  IdleTime2.Bits.opp_pg_en = 1;
  IdleTime2.Bits.idle_page_max_limit_en = 0;

  MemWritePciCfgEp (socket, ch, SCHEDULER_IDLETIME_MCDDC_CTL_REG, IdleTime.Data);
  MemWritePciCfgEp (socket, ch, SCHEDULER_IDLETIME2_MCDDC_CTL_REG, IdleTime2.Data);
}

/**
  Configure idle bypass timings

  @param[in] Socket - Socket number
  @param[in] Ch     - Channel number

  @retval N/A

**/
VOID
DdrSchedulerIdleBypassConfig (
  IN    UINT8    Socket,
  IN    UINT8    Ch
  )
{
  PSYSHOST                                                Host;
  SCHEDULER_ENABLES_MCDDC_CTL_STRUCT                      SchedulerEnables;
  MEMORY_TIMINGS_TRRD_IDLE_BYPASS_MCDDC_CTL_STRUCT        MtTrrdIdleBypass;
  MEMORY_TIMINGS_BANK_IDLE_BYPASS_MCDDC_CTL_STRUCT        MtBankIdleBypass;
  IDLE_BYPASS_OFFSETS                                     *IdleBypassOffsets;

  Host = GetSysHostPointer ();

  SchedulerEnables.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_ENABLES_MCDDC_CTL_REG);
  MtTrrdIdleBypass.Data = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_TRRD_IDLE_BYPASS_MCDDC_CTL_REG);
  MtBankIdleBypass.Data = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_BANK_IDLE_BYPASS_MCDDC_CTL_REG);

  GetIdleBypassOffsets (MemTechDdr, &IdleBypassOffsets);

  // Scheduler Enables
  SchedulerEnables.Bits.idle_bypass_en = 1;

  //
  // The programming for the idle bypass timings comes directly from the register definitions
  //
  MtTrrdIdleBypass.Bits.t_rrd_s_bypass  = GettRRD (Host, Socket, Ch)   - IdleBypassOffsets->t_rrd_s_bypass;
  MtTrrdIdleBypass.Bits.t_rrd_l_bypass  = GettRRD_L (Host, Socket, Ch) - IdleBypassOffsets->t_rrd_l_bypass;
  MtBankIdleBypass.Bits.t_rcd_wr_bypass = GettRCD (Host, Socket, Ch)   - IdleBypassOffsets->t_rcd_wr_bypass;
  MtBankIdleBypass.Bits.t_rcd_rd_bypass = GettRCD (Host, Socket, Ch)   - IdleBypassOffsets->t_rcd_rd_bypass;
  MtBankIdleBypass.Bits.t_rc_bypass     = GettRC (Host, Socket, Ch)    - IdleBypassOffsets->t_rc_bypass;
  MtBankIdleBypass.Bits.t_ras_bypass    = GettRAS (Host, Socket, Ch)   - IdleBypassOffsets->t_ras_bypass;

  MemWritePciCfgEp (Socket, Ch, SCHEDULER_ENABLES_MCDDC_CTL_REG, SchedulerEnables.Data);
  MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_TRRD_IDLE_BYPASS_MCDDC_CTL_REG, MtTrrdIdleBypass.Data);
  MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_BANK_IDLE_BYPASS_MCDDC_CTL_REG, MtBankIdleBypass.Data);
  EnableBsIdleBypass (MemTechDdr, Socket, Ch);
}

/**
  Program scheduler preemption registers

  @param[in] Socket - Socket number
  @param[in] Ch     - Channel number

  @retval   MRC_STATUS_SUCCESS
**/
MRC_STATUS
DdrSchedulerPreemptionConfig (
  IN    UINT8    Socket,
  IN    UINT8    Ch
  )
{
  PSYSHOST                                    Host;
  UINT32                                      ReadPreemption;
  UINT32                                      WritePreemption;
  UINT32                                      MissPreemption;
  UINT32                                      HitPreemption;
  struct channelNvram                         (*ChannelNvList)[MAX_CH];
  SCHEDULER_PREEMPTION_MCDDC_CTL_STRUCT       SchedulerPreemption;

  Host = GetSysHostPointer ();
  ChannelNvList = GetChannelNvList(Host, Socket);

  //
  // Scheduler Preemption rules from Memory Performance CSR Settings 04/09/2020
  //
  SchedulerPreemption.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_PREEMPTION_MCDDC_CTL_REG);

  SchedulerPreemption.Bits.rpq_preempt_watermark        = 1;
  if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) {
    SchedulerPreemption.Bits.wpq_preempt_watermark        = 0x1C;
  } else {
    SchedulerPreemption.Bits.wpq_preempt_watermark        = 1;
  }
  SchedulerPreemption.Bits.enable_critical_promotion    = 0;
  SchedulerPreemption.Bits.enable_critical_preemption   = 0;

  GetPreemptionEnableSetting (&ReadPreemption, &WritePreemption, &MissPreemption, &HitPreemption);
  SchedulerPreemption.Bits.enable_wmm_read_preemption   = ReadPreemption;
  SchedulerPreemption.Bits.enable_rmm_write_preemption  = WritePreemption;
  SchedulerPreemption.Bits.enable_omm_miss_preemption   = MissPreemption;
  SchedulerPreemption.Bits.enable_omm_hit_preemption    = HitPreemption;
  //
  // Enable Refresh Preemption - preempt transaction if bank-scheduler is blocked by refresh.
  // Enable on DDR4 configurations with more than 2 ranks.
  //
  if (GetNumberOfLogicalRanksPerChannel (Socket, Ch) > 2) {
    SchedulerPreemption.Bits.enable_refresh_preemption  = 1;
  }
  SchedulerPreemption.Bits.enable_rdb_preemption        = 1;
  SchedulerPreemption.Bits.rdb_preempt_watermark        = 5;

  MemWritePciCfgEp (Socket, Ch, SCHEDULER_PREEMPTION_MCDDC_CTL_REG, SchedulerPreemption.Data);

  return MRC_STATUS_SUCCESS;
}
/**
  Program scheduler registers after training

  @param[in] Socket - Socket number

  @retval   MRC_STATUS_SUCCESS
**/
MRC_STATUS
DdrSchedulerConfigLate (
  IN    UINT8    Socket
  )
{
  PSYSHOST                    Host;
  UINT8                       Ch;
  struct channelNvram         (*ChannelNvList)[MAX_CH];
  UINT8                       MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    //
    // Scheduler Blocking Rules programming
    //
    DdrSchedulerBlockingRulesConfig (Socket, Ch);

    //
    // Scheduler Preemption programming
    //
    DdrSchedulerPreemptionConfig (Socket, Ch);

    //
    // Scheduler idle bypass programming
    //
    DdrSchedulerIdleBypassConfig (Socket, Ch);

    //
    // Page policy programming
    //
    DdrSchedulerPagePolicyConfigLate (Socket, Ch);

    //
    // Chip specific settings
    //
    DdrSchedulerConfigLateChip (Socket, Ch);
  } // Ch

  return MRC_STATUS_SUCCESS;
}

VOID
ProgramTimings10nm (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch
  )
/*++

Programs timing parameters

@param Host        - Pointer to sysHost
@param socket      - Socket number
@param ch          - Channel number

@retval N/A

--*/
{
  //
  // Program Refresh timing parameters
  //
  ProgramRefreshTimings (Host, Socket, Ch);

  //
  // Initialize command timing mode
  //
  InitCmdTiming (Host, Socket, Ch);

  //
  // Program JEDEC timings contraints
  //
  ProgramJedecTimings (Host, Socket, Ch);

  //
  // Initial turnaround times
  //
  InitRelaxTurnaroundTimes (Socket, Ch);

  //
  // ZQ Calibration Timing Parameters
  //
  InitZqCalTiming (Host, Socket, Ch);

  //
  // Initial Page Policy and Timing Parameters
  //
  InitPagePolicy (Host, Socket, Ch);

  //
  // Scheduler Configuration
  //
  DdrSchedulerConfig (Host, Socket, Ch);

  InitMcForCpgcTraining (Socket, Ch);
  //
  // Initialize RDIMM specific timings
  //
  InitRdimmTimings (Host, Socket, Ch);

  //
  // MRS Timing Register
  //
  Ddr3MrsTimingReg10nm (Host, Socket, Ch);

  //
  // Tcpded, Tcssr, and Tcpded2srx for DDRT2
  //
  SprTimingsDdrt2(Host, Socket, Ch);

  //
  // Initialize Read/Write Major Modes and Partial Write Starvation
  //
  InitWriteMajorMode (Host, Socket, Ch);
  InitReadMajorMode (Host, Socket, Ch);

  //
  // Initialize Read/Write/CAS starvation counters
  //
  InitStarvationCounters (Host, Socket, Ch);

  SetMasterDllWakeUpTimer10nm (Host, Socket, Ch);

  return;
} // ProgramTimings10nm

VOID
RestoreTimingsChip (
               PSYSHOST Host,
               UINT8    Socket,
               UINT8    Ch
               )
/*++

  Restores other timing parameters

  @param Host        - Pointer to sysHost
  @param Socket      - Socket number
  @param Ch          - Channel number

  @retval N/A

--*/
{
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];

  ChannelNvList = GetChannelNvList(Host, Socket);
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG, (*ChannelNvList)[Ch].MemoryTimingsAdj);

  MemWritePciCfgEp (Socket, Ch, T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_REG, (*DimmNvList)[0].DdrtDimmBasicTiming);
  MemWritePciCfgEp (Socket, Ch, T_DDRT_DIMM1_BASIC_TIMING_MC_2LM_REG, (*DimmNvList)[1].DdrtDimmBasicTiming);

  RestoreTurnaroundTimings (Host, Socket, Ch);
} // RestoreTimingsChip

  /**

  Indicates whether 2x Refresh needs to be disabled for the entire channel

  @param[in] Host     - Pointer to SysHost
  @param[in] Socket   - Socket number
  @param[in] Ch       - Channel number

  @retval Status      - TRUE /FALSE to disable 2x Refresh for channel

  **/
BOOLEAN
EFIAPI
Disable2xRefreshCh (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
  )
{
  UINT8              Dimm;
  UINT8              Status;
  SYS_SETUP          *Setup;


  Status = FALSE;

  Setup = GetSysSetupPointer ();

  if ((Setup->mem.options & ALLOW2XREF_EN) == 0) {
    Status = TRUE;
  }

  if (Status == FALSE) {
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (Is3Ds4H16GbDimm (Socket, Ch, Dimm)) {
        Status = TRUE;
        break;
      }
    } // Dimm loop
  } // if Status == FALSE


  return Status;
} // Disable2xRefreshCh

/**

  Programs the MR2 shadow register for 10nm
  Gather the data from MR0,2,3,4 to program MR2 shadow register according to 10nm MR2 shadow register definition as follows
  - TwoMrsEn and AddrBitSwizzle
  - MR0[1:0]: Burst Length
  - MR2[7:6]: SRT & ASR
  - MR3[3]: Geardown mode
  - MR3[5]: Temperature Sensor Readout
  - MR3[8:6]: Fine Granularityy Refresh Mode
  - MR3[10:9]: Write CMD Latency
  - MR4[12:11]: Write Preamble & Read Preamble


  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - CPU Socket Node number (Socket ID)
  @param ch      - Channel number (0-based)
  @param dimm    - Dimm number (0-based)
  @param data    - MRS Data to set
  @param bank    - Bank number

  @retval None
**/
VOID
SetMRSShadow (
  IN      PSYSHOST      Host,
  IN      UINT8         Socket,
  IN      UINT8         Ch,
  IN      UINT8         Dimm,
  IN      UINT32        Data,
  IN      UINT8         Bank
  )
{
  struct dimmNvram       (*DimmNvList)[MAX_DIMM];
  ICXKNHMR2SHADOW_STRUCT TCMr2Shadow;
  struct channelNvram    (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);
  DimmNvList        = GetDimmNvList(Host, Socket, Ch);
  TCMr2Shadow.Data  = MemReadPciCfgEp (Socket, Ch, TCMR2SHADOW_MCDDC_CTL_REG);

  //
  // If inversion required
  //
  if ((Host->nvram.mem.dimmTypePresent != UDIMM) && (Host->nvram.mem.dramType == SPD_TYPE_DDR4)) {
    TCMr2Shadow.Bits.TwoMrsEn = 1;
  } else {
    TCMr2Shadow.Bits.TwoMrsEn = 0;
  }

  //
  // Are the address and bank mappings mirrored?
  //
  if ((*DimmNvList)[Dimm].SPDAddrMapp) {
    //
    // Set if not swizzled
    //
    TCMr2Shadow.Bits.AddrBitSwizzle |= (1 << Dimm);
  } else {
    //
    // Clear if not swizzled
    //
    TCMr2Shadow.Bits.AddrBitSwizzle &= ~(1 << Dimm);
  }

  switch (Bank) {
  case BANK0:
    TCMr2Shadow.Bits.Bl = (Data & (MR0_B4 + MR0_ONTHEFLY));
    break;

  case BANK2:
    TCMr2Shadow.Bits.Srt = (Data & MR2_SRT);
    TCMr2Shadow.Bits.Asr = (Data & MR2_ASR);
    break;

  case BANK3:
    TCMr2Shadow.Bits.WrCmdLatency = (Data & MR3_WCL);
    TCMr2Shadow.Bits.FineGranRefreshMode = (Data & MR3_FGRM);
    TCMr2Shadow.Bits.TempSensorReadout = (Data & MR3_TSR);
    TCMr2Shadow.Bits.GeardownMode = (Data & MR3_GD);
    break;

  case BANK4:
    TCMr2Shadow.Bits.TempCtlRefreshMode = (Data & MR4_WP);
    TCMr2Shadow.Bits.TempCtlRefreshMode = (Data & MR4_RP);
    break;

  default:
    break;
  }
  MemWritePciCfgEp (Socket, Ch, TCMR2SHADOW_MCDDC_CTL_REG, TCMr2Shadow.Data);
  //
  // Save it to be restored on S3 resume.
  //
  (*ChannelNvList)[Ch].TCMr2Shadow = TCMr2Shadow.Data;
}

/**

  Get the CWL adjustment

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - CPU Socket Node number (Socket ID)
  @param[in] Ch      - Channel number (0-based)
  @param[in] Dimm    - Dimm number (0-based)
  @param[out] CwlAdj - Ptr to the adjustment

  @retval None

**/

VOID
GetCwlAdj (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  OUT INT16   *CwlAdj
  )
{
  MEMORY_TIMINGS_ADJ_MCDDC_CTL_STRUCT       mtAdj;
  T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_STRUCT   DdrtDimm0BasicTiming;
  struct channelNvram                       (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(Host, Socket);

  if ((*channelNvList)[Ch].dimmList[Dimm].DcpmmPresent) {
    DdrtDimm0BasicTiming.Data = MemReadPciCfgEp (Socket, Ch, T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_REG);
    *CwlAdj = (INT16)DdrtDimm0BasicTiming.Bits.t_ddrt_twl_adj - (INT16)DdrtDimm0BasicTiming.Bits.t_ddrt_twl_adj_neg;
  } else {
    mtAdj.Data = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG);
    *CwlAdj = (INT16)mtAdj.Bits.t_cwl_adj - (INT16)mtAdj.Bits.t_cwl_adj_neg;
  }
}

#ifdef DDR5_SUPPORT

/**

  Get the DDR5 CWL adjustment

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - CPU Socket Node number (Socket ID)
  @param[in] Ch      - Channel number (0-based)
  @param[in] SubCh   - Channel number (0-based)
  @param[in] Dimm    - Dimm number (0-based)
  @param[out] CwlAdj - Ptr to the adjustment

  @retval None

**/

VOID
GetCwlAdjDdr5 (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubCh,
  IN UINT8    Dimm,
  OUT INT16   *CwlAdj
  )
{
  UINT8 ChannelLocal = 0;
  MEMORY_TIMINGS_ADJ_MCDDC_CTL_STRUCT       mtAdj;
  T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_STRUCT   DdrtDimm0BasicTiming;
  struct channelNvram (*channelNvList) [MAX_CH];
  ChannelLocal = Ch;

  channelNvList = GetChannelNvList (Host, Socket);

  if ((*channelNvList) [Ch].dimmList[Dimm].DcpmmPresent) {
    DdrtDimm0BasicTiming.Data = MemReadPciCfgEp (Socket, Ch, T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_REG);
    *CwlAdj = (INT16) DdrtDimm0BasicTiming.Bits.t_ddrt_twl_adj - (INT16) DdrtDimm0BasicTiming.Bits.t_ddrt_twl_adj_neg;
  } else {
    GetChannelMapping (Ch, SubCh, &ChannelLocal);
    mtAdj.Data = MemReadPciCfgEp (Socket, ChannelLocal, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG);
    *CwlAdj = (INT16) mtAdj.Bits.t_cwl_adj - (INT16) mtAdj.Bits.t_cwl_adj_neg;
  }

}

#endif // #ifdef DDR5_SUPPORT

/**
  Program WR data delay for a channel

  @param[in] Host     - Point to sysHost
  @param[in] Socket   - Socket number
  @param[in] Ch       - Channel number
  @param[in] SetType  - Specifies technologies - DDR4, DDRT or ALL
  @param[in] CwlAdj   - Signed value to set

  @retval N/A
**/
VOID
SetCwlAdj (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     SetType,
  INT16     CwlAdj
  )
{
  struct channelNvram                     (*ChannelNvList)[MAX_CH];
  T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_STRUCT DdrtDimm0BasicTiming;
  struct dimmNvram                        (*DimmNvList)[MAX_DIMM];
  MEMORY_TIMINGS_ADJ_MCDDC_CTL_STRUCT     mtAdj;
  UINT8                                   MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  if (ch >= MaxChDdr) {
    // invalid ch specified
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_122);
    return;
  }

  ChannelNvList = GetChannelNvList (Host, socket);
  DimmNvList = GetDimmNvList (Host, socket, ch);

  if (CwlAdj < MIN_CWL_ADJ) {
    CwlAdj = MIN_CWL_ADJ;
  }
  if (CwlAdj > MAX_CWL_ADJ) {
    CwlAdj = MAX_CWL_ADJ;
  }

  if ((SetType == CWLADJ_ALL) || (SetType == CWLADJ_DDR4)) {
    mtAdj.Data = MemReadPciCfgEp (socket, ch, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG);

    if (CwlAdj <= 0) {
      mtAdj.Bits.t_cwl_adj = 0;
      mtAdj.Bits.t_cwl_adj_neg = ABS (CwlAdj);
    } else {
      mtAdj.Bits.t_cwl_adj = ABS (CwlAdj);
      mtAdj.Bits.t_cwl_adj_neg = 0;
    }

    MemWritePciCfgEp (socket, ch, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG, mtAdj.Data);
    (*ChannelNvList)[ch].MemoryTimingsAdj = mtAdj.Data;
  }

  if ((SetType == CWLADJ_ALL) || (SetType == CWLADJ_DDRT)) {
    DdrtDimm0BasicTiming.Data = MemReadPciCfgEp (socket, ch, T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_REG);

    if (CwlAdj <= 0) {
      DdrtDimm0BasicTiming.Bits.t_ddrt_twl_adj = 0;
      DdrtDimm0BasicTiming.Bits.t_ddrt_twl_adj_neg = ABS (CwlAdj);
    } else {
      DdrtDimm0BasicTiming.Bits.t_ddrt_twl_adj = ABS (CwlAdj);
      DdrtDimm0BasicTiming.Bits.t_ddrt_twl_adj_neg = 0;
    }

    MemWritePciCfgEp (socket, ch, T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_REG, DdrtDimm0BasicTiming.Data);
    (*DimmNvList)[0].DdrtDimmBasicTiming = DdrtDimm0BasicTiming.Data;
  }

  CteConfiguration (Host, socket, ch, CTE_CONFIG_UPDATE_TCL, (*ChannelNvList) [ch].common.nCL);
  CteConfiguration (Host, socket, ch, CTE_CONFIG_UPDATE_TCWL, (*ChannelNvList) [ch].common.nCL - 2 + CwlAdj);
  SetCwlAdjDdrio (Host, socket, ch, SetType, CwlAdj);
}

VOID
EffectiveBusDelayInit(
  PSYSHOST    Host,
  UINT8       socket,
  UINT8       ch,
  UINT8       Dimm,
  INT16       *CwlAdj,
  UINT32      *IOComp,
  UINT16      *IntDelta,
  UINT32      *longPreambleEnable
  )
{
  UINT32                              ioLatIoComp = 0;
  struct channelNvram                 (*channelNvList)[MAX_CH];
  struct dimmNvram                    (*dimmNvList)[MAX_DIMM];

  channelNvList = GetChannelNvList(Host, socket);
  dimmNvList    = GetDimmNvList(Host, socket, ch);

  GetCwlAdj(Host, socket, ch, Dimm, CwlAdj);

  GetIoLatencyIoComp (Host, socket, ch, SUB_CH_A, &ioLatIoComp, 0);

  *CwlAdj = *CwlAdj * (2 * PiTicks);

  *longPreambleEnable = GetLongPreambleDdrio (Host, socket, ch);

  *IOComp      = ioLatIoComp * PiTicks;

  *IntDelta    = (8 * PiTicks);        //equates to 8 UI
} // EffectiveBusDelayInit



VOID
SaveRefreshRate10nm(
               PSYSHOST Host,
               UINT8    socket,
               UINT8    ch,
               UINT8 refreshRate
               )
{
  struct channelNvram  (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(Host, socket);
  //
  // Save Refresh Rate for later use
  //
  Host->nvram.mem.socket[socket].refreshRate = refreshRate;
  (*channelNvList)[ch].twoXRefreshSetPerCh = refreshRate;
}

/**
  Program Stagger Refresh enables

  @param[in] Host     - Point to sysHost
  @param[in] Socket   - Socket number
  @param[in] Ch       - Channel number

  @retval N/A

**/
VOID
SetRefreshMiscEnables (
  IN  PSYSHOST    Host,
  IN  UINT8       Socket,
  IN  UINT8       Ch
)
{
  MCMNT_CHKN_BIT_MCDDC_CTL_STRUCT ChknBitMcddcCtl;

  ChknBitMcddcCtl.Data = MemReadPciCfgEp (Socket, Ch, MCMNT_CHKN_BIT_MCDDC_CTL_REG);
  ChknBitMcddcCtl.Bits.stagger_ref_en          = 1;
#ifdef DDR5_SUPPORT
  ChknBitMcddcCtl.Bits.dis_opp_ref             = 1;
  ChknBitMcddcCtl.Bits.dis_refresh_start_delay = 1;
#endif // DDR5_SUPPORT
  MemWritePciCfgEp (Socket, Ch, MCMNT_CHKN_BIT_MCDDC_CTL_REG, ChknBitMcddcCtl.Data);
}

/**
  Program tXS_DLL timing for a channel

  @param[in] Host     - Point to sysHost
  @param[in] Socket   - Socket number
  @param[in] Channel  - Channel number

  @retval N/A
**/
VOID
ProgramTxsdll (
  IN  PSYSHOST   Host,
  IN  UINT8      Socket,
  IN  UINT8      Channel
)
{
  TCSRFTP_MCDDC_CTL_STRUCT  Tcsrftp;
  UINT32                    TxsOffset;

  Tcsrftp.Data = MemReadPciCfgEp (Socket, Channel, TCSRFTP_MCDDC_CTL_REG);
  Tcsrftp.Bits.t_xsdll = GettXSDLL (Host, Socket, Channel);
  Tcsrftp.Bits.t_zqoper = tZQOPER_DDR4;
  Tcsrftp.Bits.t_mod = GettMOD (Socket, Channel);

  //
  // GettXSOFFSET returns tXOFFSET in DCLKs, but the hardware will shift the programmed value of t_xsoffset to the
  // left by one bit, so t_xsoffset needs to be shifted right by one bit before being programmed. Before performing
  // the shift, we need to add 1 to the offset if it is odd, so that the division doesn't result in too low of a
  // value.
  //
  TxsOffset = GettXSOFFSET (Host, Socket, Channel);
  if ((TxsOffset & BIT0) != 0) {
    TxsOffset += 1;
  }
  Tcsrftp.Bits.t_xsoffset = TxsOffset >> 1;

  MemWritePciCfgEp (Socket, Channel, TCSRFTP_MCDDC_CTL_REG, Tcsrftp.Data);

}


VOID
SetMiscDelay10nm(
             PSYSHOST Host,
             UINT8    socket,
             UINT8    ch,
             UINT8    cmdTiming
)
{
#ifndef GNRSRF_HOST
  // ICX_TODO
  T_DDRT_MISC_DELAY_MC_2LM_STRUCT             ddrtMiscDelay;

  ddrtMiscDelay.Data = MemReadPciCfgEp (socket, ch, T_DDRT_MISC_DELAY_MC_2LM_REG);
  if (cmdTiming == TIMING_2N)
  {
    ddrtMiscDelay.Bits.enable_ddrt_2n_timing = 1;
  } else {
    ddrtMiscDelay.Bits.enable_ddrt_2n_timing = 0;
  }
  MemWritePciCfgEp (socket, ch, T_DDRT_MISC_DELAY_MC_2LM_REG, ddrtMiscDelay.Data);
#endif // !GNRSRF_HOST
}

/**

  Gather t_stab timing base on channel configuration

  @param[in]  Host              - Pointer to sysHost
  @param[in]  Socket            - Socket ID
  @param[in]  Ch                - Current channel
  @param[in]  *RdimmTimingCntl  - Pointer to Timing Control Structure
  @param[in]  FmcType           - 0x00: FMC_NONE_TYPE
                                  0x7A: FMC_EKV_TYPE
                                  0x7A: FMC_BWV_TYPE
  @retval N/A

**/
VOID
SetRdimmTimingCntl10nm(
  IN PSYSHOST                          Host,
  IN UINT8                             Socket,
  IN UINT8                             Ch,
  IN RDIMMTIMINGCNTL_MCDDC_CTL_STRUCT  *RdimmTimingCntl,
  IN UINT8                             FmcType
)
{
  //
  // Program t_stab
  // - 5us for RDimm Only channel
  // - 10.3us for EKV channel
  // - 9.19us for BWV channel
  //
  if (FmcType != FMC_NONE_TYPE) {
    RdimmTimingCntl->Bits.t_stab = RdimmTimingCntl->Bits.t_stab + (UINT16) GettSTABAEP10nm (Host, Socket, Ch, FmcType);
  } else {
    RdimmTimingCntl->Bits.t_stab = RdimmTimingCntl->Bits.t_stab + (UINT16) GettSTAB (Host, Socket, Ch);
  }
}

VOID
SetMasterDllWakeUpTimer10nm(
             PSYSHOST                                    Host,
             UINT8                                       socket,
             UINT8                                       ch
)
{
  SREF_STATIC_MCDDC_CTL_STRUCT                sref_static;
  SREF_LL0_MCDDC_CTL_STRUCT                   sref_ll0;
  struct channelNvram                         (*channelNvList)[MAX_CH];
  SYS_SETUP                                   *Setup;

  Setup = GetSysSetupPointer ();

  channelNvList = GetChannelNvList(Host, socket);

  //
  // PM DLL Control
  // Master DLL wake up timer = 500ns
  //
  sref_static.Data            = MemReadPciCfgEp (socket, ch, SREF_STATIC_MCDDC_CTL_REG);
  sref_static.Bits.mdll_timer = Saturate(MAXFIELDVAL(sref_static.Bits.mdll_timer), (5000000 + (GettCK(Host, socket) - 1)) / GettCK(Host, socket));
  sref_ll0.Data    = MemReadPciCfgEp (socket, ch, SREF_LL0_MCDDC_CTL_REG);
  if ((Setup->mem.options & MDLL_SHUT_DOWN_EN) && ((*channelNvList)[ch].lrDimmPresent == 0)) {
    sref_ll0.Bits.mdll_off_en = 1;
  } else {
    sref_ll0.Bits.mdll_off_en = 0;
  }
  MemWritePciCfgEp (socket, ch, SREF_STATIC_MCDDC_CTL_REG, sref_static.Data);
  MemWritePciCfgEp (socket, ch, SREF_LL0_MCDDC_CTL_REG, sref_ll0.Data);
}

VOID
Ddr3MrsTimingReg10nm(
                PSYSHOST                                    Host,
                UINT8                                       socket,
                UINT8                                       ch
                )
{
  TCMRS_MCDDC_CTL_STRUCT                 tcmrs;

  tcmrs.Data            = MemReadPciCfgEp (socket, ch, TCMRS_MCDDC_CTL_REG);

  if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
    //
    // For DDR5, tMRD = max(14ns, 16dclk)
    //
    tcmrs.Bits.tmrd_ddr3  = MAX (NanosecondToClocks (socket, TMRD_NS), TMRD_MIN_DCLK);
  } else {
    tcmrs.Bits.tmrd_ddr3  = tMRD;
  }
  MemWritePciCfgEp (socket, ch, TCMRS_MCDDC_CTL_REG, tcmrs.Data);
}

VOID
CalcFinalGap(
            UINT16 *gap,
            UINT16 maxGap,
            UINT16 gapDimm
            )
{
  if (maxGap >= 16) {
    gapDimm = maxGap - 16;
  } else {
    gapDimm = 0;
  }

  if ((gapDimm % (MAX_PHASE_IN_FINE_ADJUSTMENT * 2)) > 0) {
    *gap = (gapDimm / (MAX_PHASE_IN_FINE_ADJUSTMENT * 2)) + 1;
  } else {
    *gap = gapDimm / (MAX_PHASE_IN_FINE_ADJUSTMENT * 2);
  }
}

/**

  Restore MC timing parameters for fast boot or resume paths

  @param[in]  Host    - Pointer to sysHost
  @param[in]  Socket  - Processor to initialize
  @param[in]  Ch      - Channel number

  @retval N/A

**/
VOID
SetRestoreTimingsMc (
  IN    PSYSHOST  Host,
  IN    UINT8     socket,
  IN    UINT8     ch
  )
{
}

VOID
GetOdtTiming(
             PSYSHOST   Host,
             UINT8      socket,
             UINT8      ch,
             ODT_TIMING *odtTiming
             )
{
  RD_ODT_TBL2_MCDDC_CTL_STRUCT  rdOdtTiming;
  WR_ODT_TBL2_MCDDC_CTL_STRUCT  wrOdtTiming;

  rdOdtTiming.Data = MemReadPciCfgEp (socket, ch, RD_ODT_TBL2_MCDDC_CTL_REG);
  wrOdtTiming.Data = MemReadPciCfgEp (socket, ch, WR_ODT_TBL2_MCDDC_CTL_REG);

  odtTiming->odtRdLeading  = (UINT8)rdOdtTiming.Bits.extraleadingodt;
  odtTiming->odtRdTrailing = (UINT8)rdOdtTiming.Bits.extratrailingodt;
  odtTiming->odtWrLeading  = (UINT8)wrOdtTiming.Bits.extra_leading_odt;
  odtTiming->odtWrTrailing = (UINT8)wrOdtTiming.Bits.extra_trailing_odt;
}

/**
  Check t_cke value to search for constraints and fix them if present

  @param [in    ] Timing    - Timing parameter to check for tCKE
  @param [   out] TCkeValue - t_cke value to check for constrains

  @retval n/a
**/
VOID
CheckTCkeValueConstraints (
  IN     MEM_TIMING_TYPE Timing,
     OUT UINT32          *TCkeValue
  )
{
  if ((Timing == tCKE) && IsSiliconWorkaroundEnabled ("S1909161672")) {
    //
    // This will make sure the timer is set to the maximum value to allow more flexibility
    // in the programming values of lpmode_entry_latency and t_ddrt_cke.  The upper bits of
    // the CSR are ignored due to the bug.
    //
    *TCkeValue |= 0x3;
  }
}

BOOLEAN
CheckDdrtApplicable (
  IN      UINT8             Socket,
  IN      UINT8             Ch,
  IN      MEM_TIMING_TYPE   Timing
  )
{
  PSYSHOST                Host;
  UINT8                   Index;
  struct channelNvram     (*ChannelNvList)[MAX_CH];
  MEM_TIMING_TYPE         DdrtNotApplicableTiming[5] = {tRRD, tRRD_L, tFAW, tCCD_L, tCCD};
  BOOLEAN                 ShowWarning = TRUE;

  Host = (PSYSHOST) GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // Do not show the warning for timings is not applicable to DDRT only channel
  //
  if (((*ChannelNvList)[Ch].maxDimm == 1) && ((*ChannelNvList)[Ch].ddrtEnabled  == 1)) {
    for (Index = 0; Index < sizeof (DdrtNotApplicableTiming) / sizeof (DdrtNotApplicableTiming[0]); Index++ ) {
      if (Timing == DdrtNotApplicableTiming[Index]) {
         ShowWarning = FALSE;
        break;
      }
    }
  }

  return ShowWarning;
}

/**
  Determine whether a timing value is out of bounds, taking appropriate actions if it is.

  If Value is out of bounds, it will be changed to the respective MinValue or MaxValue. Additionally, if ShowWarning is
  TRUE, a message will be printed and a warning will be raised.

  @param[in]      Socket        The socket where the value comes from
  @param[in]      Channel       The channel where the value comes from
  @param[in]      ShowWarning   Whether or not to print a message and raise a warning if Value is out of bounds
  @param[in]      TimingString  The name of the timing parameter
  @param[in]      MinValue      The minimum in-bounds value
  @param[in]      MaxValue      The maximum in-bounds value
  @param[in,out]  Value         The timing value to check

  @return   EFI_INVALID_PARAMETER   Value is out of bounds
            EFI_SUCCESS             Value is not out of bounds
**/
EFI_STATUS
HandleOutOfBoundsTiming (
  IN      UINT8   Socket,
  IN      UINT8   Channel,
  IN      BOOLEAN ShowWarning,
  IN      CHAR8   *TimingString,
  IN      UINT32  MinValue,
  IN      UINT32  MaxValue,
  IN  OUT UINT32  *Value
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  if (*Value > MaxValue) {
    if (ShowWarning) {
      RcDebugPrint (SDBG_MINMAX, "%a -- Register Overflow! MaxValue = 0x%x, Value = 0x%x, adjusted to MaxVal.\n",
                    TimingString, MaxValue, *Value);
      OutputWarning (WARN_REGISTER_OVERFLOW, WARN_MINOR_REGISTER_OVERFLOW, Socket, Channel, NO_DIMM, NO_RANK);
    }
    *Value = MaxValue;
    Status = EFI_INVALID_PARAMETER;
  } else if (*Value < MinValue) {
    if (ShowWarning) {
      RcDebugPrint (SDBG_MINMAX, "%a -- Register Underflow! MinVal = 0x%x, value = 0x%x, adjusted to MinVal.\n",
                    TimingString, MinValue, *Value);
      OutputWarning (WARN_REGISTER_OVERFLOW, WARN_MINOR_REGISTER_UNDERFLOW, Socket, Channel, NO_DIMM, NO_RANK);
    }
    *Value = MinValue;
    Status = EFI_INVALID_PARAMETER;
  }

  return Status;
}

/**

  This function ensures we do not overflow/underflow a timing parameter.
  If *Value is outside of the limits for the processor, a warning will be
  logged and *Value will be updated with the max or min value allowed.

  @param[in]      MemTechType  - Memory Technology Type
  @param[in]      Socket       - CPU Socket number
  @param[in]      Ch           - Channel number
  @param[in]      Timing       - Timing parameter
  @param[in,out]  *Value       - Pointer to the value to check

  @retval EFI_SUCCESS           - Value is in bounds
          EFI_INVALID_PARAMETER - Value is out of bounds, *Value will be updated
                                  with the maximum supported value
          EFI_NOT_FOUND         - Timing parameter not supported

**/
EFI_STATUS
EFIAPI
CheckBoundsChip (
  IN      MEM_TECH_TYPE     MemTechType,
  IN      UINT8             Socket,
  IN      UINT8             Ch,
  IN      MEM_TIMING_TYPE   Timing,
  IN OUT  UINT32            *Value
  )
{
  UINT8                   Index;
  UINT16                  NumTimings;
  MEM_TIMING_LIMITS       *MemTimingLimitsTable;

  if (MemTechType == MemTechHbm) {
    return EFI_SUCCESS;
  }

  if (GetMemTimingLimitsTable (&MemTimingLimitsTable, &NumTimings) != EFI_SUCCESS) {
    return EFI_NOT_FOUND;
  }

  for (Index = 0; Index < NumTimings; Index++) {
    if (Timing == MemTimingLimitsTable[Index].Timing) {
      CheckTCkeValueConstraints (Timing, Value);
      break;
    }
  }

  if (Index >= NumTimings) {
    // The timing parameter is not in the list
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_128);
    return EFI_NOT_FOUND;
  }

  return HandleOutOfBoundsTiming (
    Socket,
    Ch,
    CheckDdrtApplicable (Socket, Ch, Timing),
    MemTimingLimitsTable[Index].TimingString,
    MemTimingLimitsTable[Index].MinMax.MinValue,
    MemTimingLimitsTable[Index].MinMax.MaxValue,
    Value
  );
} // CheckBoundsChip

/**

  This function returns the value of tSTAB of EKV/BWV

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number
  @param[in] FmcType - 0x00: FMC_NONE_TYPE
                       0x7A: FMC_EKV_TYPE
                       0x7A: FMC_BWV_TYPE

  @retval UINT32 Value of tSTAB of EKV/BWV

**/
UINT32
GettSTABAEP10nm (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    FmcType
)
{
  UINT32  tSTABLocal;

  //
  // Set 10.3us for EKV
  // Set 5.927us for BWV
  //
  if (FmcType == FMC_BWV_TYPE) {
    tSTABLocal = (59270000 + (GettCK (Host, Socket) - 100)) / GettCK (Host, Socket);
  } else {
    tSTABLocal = (103000000 + (GettCK (Host, Socket) - 100)) / GettCK (Host, Socket);
  }

  CheckBoundsChip (MemTechDdr, Socket, Ch, tSTAB, &tSTABLocal);

  return tSTABLocal;
} // GettSTABAEP10nm

#ifdef DDR5_SUPPORT
UINT32
Ddr5PreTrainingInit (
  PSYSHOST Host
  )
{
  UINT8                                 Ch;
  UINT8                                 Socket = Host->var.mem.currentSocket;
  struct channelNvram                   (*ChannelNvList)[MAX_CH];
  MEMORY_TIMINGS_ADJ_MCDDC_CTL_STRUCT   MemoryTimingsAdjCtl;
  UINT8                                 SubCh;
  UINT8                                 ChannelLocal;

  ChannelNvList = GetChannelNvList(Host, Socket);
  if (!IsBrsPresent(Host, Socket)) {
    return SUCCESS;
  }

  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    for (SubCh = 0; SubCh < 2; SubCh++) {
      GetChannelMapping(Ch, SubCh, &ChannelLocal);

      MemoryTimingsAdjCtl.Data = MemReadPciCfgEp (Socket, ChannelLocal, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG);
#ifndef SPR_HOST
      MemoryTimingsAdjCtl.Bits.enable_ddr5 = 1;
#endif
      MemoryTimingsAdjCtl.Bits.rcd_on_dimm = 1;
      MemoryTimingsAdjCtl.Bits.cmd_stretch = 2;
      MemWritePciCfgEp (Socket, ChannelLocal, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG, MemoryTimingsAdjCtl.Data);
    }
  }
  return SUCCESS;
} // Ddr5PreTrainingInit

#endif

/**

  Programs turnaround times. Unused in 10nm.

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket number

  @retval SUCCESS

**/
UINT32
TurnAroundTimings (
  IN PSYSHOST Host,
  IN UINT8 Socket
  )
{
  ASSERT (FALSE);

  return 0;

} // TurnAroundTimings

/**

  Compares turnaround times with HAS table of expected values
  reports via scratchpad if worse than expected.

  Unused in 10nm.

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param Socket  - Socket number

  @retval SUCCESS

**/
UINT32
CheckTurnAroundTimings (
  IN PSYSHOST Host,
  IN UINT8 Socket
  )
{
  ASSERT (FALSE);

  return 0;

} // CheckTurnAroundTimings

/**

  Restores memory controller turnaround timing parameters

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param Socket  - Socket number
  @param Ch      - Channel number

  @retval None

**/
VOID
RestoreTurnaroundTimings (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
  )
{
  struct channelNvram                        (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);

  MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_CAS2CAS_DR_MCDDC_CTL_REG, (*ChannelNvList)[Ch].mtCas2CasDr);
  MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_CAS2CAS_DD_MCDDC_CTL_REG, (*ChannelNvList)[Ch].mtCas2CasDd);
  MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_CAS2CAS_SR_MCDDC_CTL_REG, (*ChannelNvList)[Ch].mtCas2CasSr);
  MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_CAS2CAS_SG_MCDDC_CTL_REG, (*ChannelNvList)[Ch].mtCas2CasSg);
  MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_CAS2CAS_DS_MCDDC_CTL_REG, (*ChannelNvList)[Ch].mtCas2CasDs);

}

/**

  Get or Set all types of turnaround timing for specified channel.

  If Mode is GSM_READ_CSR on input, read turnaround related CSRs,
  adjust the data to be non SOC specific, and return
  If the Mode is GSM_FORCE_WRITE, adjust the data in a turnaround structure
  as needed for a given SOC, and write the CSRs
  Other input is not supported.
  Other Mode input settings are not supported by this function

  @param[in]       MemTechType         - Memory Technology Type
  @param[in]       Socket              - Socket number
  @param[in]       Ch                  - Channel number
  @param[in]       Mode                - Get/Set mode flags
  @param[in, out]  TurnaroundSettings  - Pointer to Turnaround settings

  @retval EFI_SUCCESS   Turnaround CSRs read/write successful
**/
EFI_STATUS
GetSetTurnaroundSettings (
  IN     MEM_TECH_TYPE  MemTechType,
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Mode,
  IN OUT UINT32         TurnaroundSettingsTable[]
  )
{
  PSYSHOST                                   Host;
  struct channelNvram                        (*ChannelNvList)[MAX_CH];
  EFI_STATUS                                 ReturnStatus = EFI_SUCCESS;
  MEMORY_TIMINGS_CAS2CAS_DR_MCDDC_CTL_STRUCT TCas2CasDr = {0};
  MEMORY_TIMINGS_CAS2CAS_DD_MCDDC_CTL_STRUCT TCas2CasDd = {0};
  MEMORY_TIMINGS_CAS2CAS_SR_MCDDC_CTL_STRUCT TCas2CasSr = {0};
  MEMORY_TIMINGS_CAS2CAS_SG_MCDDC_CTL_STRUCT TCas2CasSg = {0};
  MEMORY_TIMINGS_CAS2CAS_DS_MCDDC_CTL_STRUCT TCas2CasDs = {0};
  MEMORY_TIMINGS_RANK_TRRD_3DS_MCDDC_CTL_STRUCT TRrd3ds = {0};
  UINT8                                      LoopLatency;


  Host          = (PSYSHOST) GetSysHostPointer ();

  if (MemTechType == MemTechHbm) {
    LoopLatency = 0;  // No need to - 2 dclks for HBM, LoopLatency is 0
  } else {
    ChannelNvList = GetChannelNvList (Host, Socket);
    LoopLatency = GetLoopLatency ();
  }

  if ((GsmReadCsr (Mode)) || (GsmForceWrite (Mode))) {
    ReturnStatus = EFI_SUCCESS;
  } else {
    ASSERT (FALSE);
    ReturnStatus = EFI_INVALID_PARAMETER;
  }
  if (TurnaroundSettingsTable == NULL) {
    ASSERT (FALSE);
    ReturnStatus = EFI_INVALID_PARAMETER;
  }
  if (ReturnStatus == EFI_SUCCESS) {
    if (GsmReadCsr (Mode)) {
      TCas2CasSg.Data = ChRegisterRead (MemTechType, Socket, Ch, MEMORY_TIMINGS_CAS2CAS_SG_MCDDC_CTL_REG);
      TCas2CasSr.Data = ChRegisterRead (MemTechType, Socket, Ch, MEMORY_TIMINGS_CAS2CAS_SR_MCDDC_CTL_REG);
      TCas2CasDr.Data = ChRegisterRead (MemTechType, Socket, Ch, MEMORY_TIMINGS_CAS2CAS_DR_MCDDC_CTL_REG);
      TCas2CasDd.Data = ChRegisterRead (MemTechType, Socket, Ch, MEMORY_TIMINGS_CAS2CAS_DD_MCDDC_CTL_REG);
      TCas2CasDs.Data = ChRegisterRead (MemTechType, Socket, Ch, MEMORY_TIMINGS_CAS2CAS_DS_MCDDC_CTL_REG);
      TRrd3ds.Data    = ChRegisterRead (MemTechType, Socket, Ch, MEMORY_TIMINGS_RANK_TRRD_3DS_MCDDC_CTL_REG);

      TurnaroundSettingsTable[t_RRSG] = TCas2CasSg.Bits.t_rrsg + LoopLatency;
      TurnaroundSettingsTable[t_WWSG] = TCas2CasSg.Bits.t_wwsg + LoopLatency;
      TurnaroundSettingsTable[t_RWSG] = TCas2CasSg.Bits.t_rwsg + LoopLatency;
      TurnaroundSettingsTable[t_WRSG] = TCas2CasSg.Bits.t_wrsg + LoopLatency;
      TurnaroundSettingsTable[t_RRSR] = TCas2CasSr.Bits.t_rrsr + LoopLatency;
      TurnaroundSettingsTable[t_WWSR] = TCas2CasSr.Bits.t_wwsr + LoopLatency;
      TurnaroundSettingsTable[t_RWSR] = TCas2CasSr.Bits.t_rwsr + LoopLatency;
      TurnaroundSettingsTable[t_WRSR] = TCas2CasSr.Bits.t_wrsr + LoopLatency;
      TurnaroundSettingsTable[t_RRDR] = TCas2CasDr.Bits.t_rrdr + LoopLatency;
      TurnaroundSettingsTable[t_WWDR] = TCas2CasDr.Bits.t_wwdr + LoopLatency;
      TurnaroundSettingsTable[t_RWDR] = TCas2CasDr.Bits.t_rwdr + LoopLatency;
      TurnaroundSettingsTable[t_WRDR] = TCas2CasDr.Bits.t_wrdr + LoopLatency;
      TurnaroundSettingsTable[t_RRDD] = TCas2CasDd.Bits.t_rrdd + LoopLatency;
      TurnaroundSettingsTable[t_WWDD] = TCas2CasDd.Bits.t_wwdd + LoopLatency;
      TurnaroundSettingsTable[t_RWDD] = TCas2CasDd.Bits.t_rwdd + LoopLatency;
      TurnaroundSettingsTable[t_WRDD] = TCas2CasDd.Bits.t_wrdd + LoopLatency;
      TurnaroundSettingsTable[t_RRDS] = TCas2CasDs.Bits.t_rrds + LoopLatency;
      TurnaroundSettingsTable[t_WWDS] = TCas2CasDs.Bits.t_wwds + LoopLatency;
      TurnaroundSettingsTable[t_RWDS] = TCas2CasDs.Bits.t_rwds + LoopLatency;
      TurnaroundSettingsTable[t_WRDS] = TCas2CasDs.Bits.t_wrds + LoopLatency;
      TurnaroundSettingsTable[t_RRDLR] = TRrd3ds.Bits.t_rrd_dlr + LoopLatency;
    } else {

      if (IsSiliconWorkaroundEnabled ("S1408945244") && !IsBrsPresent (Host, Socket)) {
        if (TurnaroundSettingsTable[t_RRDS] > TurnaroundSettingsTable[t_RRSR]) {
          TurnaroundSettingsTable[t_RWDS] = MAX (TurnaroundSettingsTable[t_RWSR], TurnaroundSettingsTable[t_RWDS]);
        }
      }

      if ((IsDdr5Present (Host, Socket) == TRUE) && (IsBrsPresent (Host, Socket) == FALSE)) {
        CheckBoundsChip (MemTechType, Socket, Ch, tRRSG, &(TurnaroundSettingsTable[t_RRSG]));
        CheckBoundsChip (MemTechType, Socket, Ch, tWWSG, &(TurnaroundSettingsTable[t_WWSG]));
        CheckBoundsChip (MemTechType, Socket, Ch, tRRSR, &(TurnaroundSettingsTable[t_RRSR]));
        CheckBoundsChip (MemTechType, Socket, Ch, tWWDS, &(TurnaroundSettingsTable[t_WWDS]));
      } else {
        CheckBoundsChip (MemTechType, Socket, Ch, tCCD_L, &(TurnaroundSettingsTable[t_RRSG]));
        // Note that the below code assumes the range check for wwsg is the
        // same as for rrsg, which is true for all 10nm wave 1 SOCs
        CheckBoundsChip (MemTechType, Socket, Ch, tCCD_L, &(TurnaroundSettingsTable[t_WWSG]));
        CheckBoundsChip (MemTechType, Socket, Ch, tCCD, &(TurnaroundSettingsTable[t_RRSR]));
      }
      CheckBoundsChip (MemTechType, Socket, Ch, tRWSG, &(TurnaroundSettingsTable[t_RWSG]));
      CheckBoundsChip (MemTechType, Socket, Ch, tWRSG, &(TurnaroundSettingsTable[t_WRSG]));
      CheckBoundsChip (MemTechType, Socket, Ch, tWWSR, &(TurnaroundSettingsTable[t_WWSR]));
      CheckBoundsChip (MemTechType, Socket, Ch, tRWSR, &(TurnaroundSettingsTable[t_RWSR]));
      CheckBoundsChip (MemTechType, Socket, Ch, tWRSR, &(TurnaroundSettingsTable[t_WRSR]));
      CheckBoundsChip (MemTechType, Socket, Ch, tRRDR, &(TurnaroundSettingsTable[t_RRDR]));
      CheckBoundsChip (MemTechType, Socket, Ch, tWWDR, &(TurnaroundSettingsTable[t_WWDR]));
      CheckBoundsChip (MemTechType, Socket, Ch, tRWDR, &(TurnaroundSettingsTable[t_RWDR]));
      CheckBoundsChip (MemTechType, Socket, Ch, tWRDR, &(TurnaroundSettingsTable[t_WRDR]));
      CheckBoundsChip (MemTechType, Socket, Ch, tRRDD, &(TurnaroundSettingsTable[t_RRDD]));
      CheckBoundsChip (MemTechType, Socket, Ch, tWWDD, &(TurnaroundSettingsTable[t_WWDD]));
      CheckBoundsChip (MemTechType, Socket, Ch, tRWDD, &(TurnaroundSettingsTable[t_RWDD]));
      CheckBoundsChip (MemTechType, Socket, Ch, tWRDD, &(TurnaroundSettingsTable[t_WRDD]));
      CheckBoundsChip (MemTechType, Socket, Ch, tRRDS, &(TurnaroundSettingsTable[t_RRDS]));
      CheckBoundsChip (MemTechType, Socket, Ch, tRWDS, &(TurnaroundSettingsTable[t_RWDS]));
      CheckBoundsChip (MemTechType, Socket, Ch, tWRDS, &(TurnaroundSettingsTable[t_WRDS]));
      // Note that the below code assumes the range check for rrd_dlr
      // is the same as for rrds, which is true for all 10nm wave 1 SOCs
      CheckBoundsChip (MemTechType, Socket, Ch, tRRDS, &(TurnaroundSettingsTable[t_RRDLR]));

      TCas2CasSg.Bits.t_rrsg = TurnaroundSettingsTable[t_RRSG] - LoopLatency;
      TCas2CasSg.Bits.t_wwsg = TurnaroundSettingsTable[t_WWSG] - LoopLatency;
      TCas2CasSg.Bits.t_rwsg = TurnaroundSettingsTable[t_RWSG] - LoopLatency;
      TCas2CasSg.Bits.t_wrsg = TurnaroundSettingsTable[t_WRSG] - LoopLatency;
      TCas2CasSr.Bits.t_rrsr = TurnaroundSettingsTable[t_RRSR] - LoopLatency;
      TCas2CasSr.Bits.t_wwsr = TurnaroundSettingsTable[t_WWSR] - LoopLatency;
      TCas2CasSr.Bits.t_rwsr = TurnaroundSettingsTable[t_RWSR] - LoopLatency;
      TCas2CasSr.Bits.t_wrsr = TurnaroundSettingsTable[t_WRSR] - LoopLatency;
      TCas2CasDr.Bits.t_rrdr = TurnaroundSettingsTable[t_RRDR] - LoopLatency;
      TCas2CasDr.Bits.t_wwdr = TurnaroundSettingsTable[t_WWDR] - LoopLatency;
      TCas2CasDr.Bits.t_rwdr = TurnaroundSettingsTable[t_RWDR] - LoopLatency;
      TCas2CasDr.Bits.t_wrdr = TurnaroundSettingsTable[t_WRDR] - LoopLatency;
      TCas2CasDd.Bits.t_rrdd = TurnaroundSettingsTable[t_RRDD] - LoopLatency;
      TCas2CasDd.Bits.t_wwdd = TurnaroundSettingsTable[t_WWDD] - LoopLatency;
      TCas2CasDd.Bits.t_rwdd = TurnaroundSettingsTable[t_RWDD] - LoopLatency;
      TCas2CasDd.Bits.t_wrdd = TurnaroundSettingsTable[t_WRDD] - LoopLatency;
      TCas2CasDs.Bits.t_rrds = TurnaroundSettingsTable[t_RRDS] - LoopLatency;
      TCas2CasDs.Bits.t_wwds = TurnaroundSettingsTable[t_WWDS] - LoopLatency;
      TCas2CasDs.Bits.t_rwds = TurnaroundSettingsTable[t_RWDS] - LoopLatency;
      TCas2CasDs.Bits.t_wrds = TurnaroundSettingsTable[t_WRDS] - LoopLatency;
      TRrd3ds.Bits.t_rrd_dlr = TurnaroundSettingsTable[t_RRDLR] - LoopLatency;

      //
      // Skip for HBM
      //
      if (MemTechType != MemTechHbm) {
        (*ChannelNvList)[Ch].mtCas2CasDr = TCas2CasDr.Data;
        (*ChannelNvList)[Ch].mtCas2CasDd = TCas2CasDd.Data;
        (*ChannelNvList)[Ch].mtCas2CasSr = TCas2CasSr.Data;
        (*ChannelNvList)[Ch].mtCas2CasSg = TCas2CasSg.Data;
        (*ChannelNvList)[Ch].mtCas2CasDs = TCas2CasDs.Data;
      }

      ChRegisterWrite (MemTechType, Socket, Ch, MEMORY_TIMINGS_CAS2CAS_DR_MCDDC_CTL_REG, TCas2CasDr.Data);
      ChRegisterWrite (MemTechType, Socket, Ch, MEMORY_TIMINGS_CAS2CAS_DD_MCDDC_CTL_REG, TCas2CasDd.Data);
      ChRegisterWrite (MemTechType, Socket, Ch, MEMORY_TIMINGS_CAS2CAS_SR_MCDDC_CTL_REG, TCas2CasSr.Data);
      ChRegisterWrite (MemTechType, Socket, Ch, MEMORY_TIMINGS_CAS2CAS_SG_MCDDC_CTL_REG, TCas2CasSg.Data);
      ChRegisterWrite (MemTechType, Socket, Ch, MEMORY_TIMINGS_CAS2CAS_DS_MCDDC_CTL_REG, TCas2CasDs.Data);
      ChRegisterWrite (MemTechType, Socket, Ch, MEMORY_TIMINGS_RANK_TRRD_3DS_MCDDC_CTL_REG, TRrd3ds.Data);
    }
  }
  if (ReturnStatus == EFI_INVALID_PARAMETER) {

  }
  return ReturnStatus;
} // GetSetTurnaroundSettings

/**
  Get or Set specified single type of turnaround timing for specified channel.

  If Mode is GSM_READ_CSR on input, read turnaround related CSRs,
  adjust the data to be non SOC specific, and return
  If the Mode is GSM_FORCE_WRITE, adjust the data in a turnaround structure
  as needed for a given SOC, and write the CSRs
  Other input is not supported.
  Other Mode input settings are not supported by this function

  @param[in]       MemTechType         - Memory Technology Type
  @param[in]       Socket              - Socket number
  @param[in]       Ch                  - Channel number
  @param[in]       Mode                - Get/Set mode flags
  @param[in]       TurnaroundType      - Turn around type.
  @param[in, out]  TurnaroundValue     - Pointer to Turnaround setting

  @retval MRC_STATUS_SUCCESS   Turnaround CSRs read/write successful
  @retval MRC_STATUS_SUCCESS   Turnaround CSRs read/write fail
**/
EFI_STATUS
GetSetSingleTurnaroundSetting (
  IN     MEM_TECH_TYPE    MemTechType,
  IN     UINT8            Socket,
  IN     UINT8            Ch,
  IN     UINT8            Mode,
  IN     UINT8            TurnaroundType,
  IN OUT UINT32           *TurnaroundValue
  )
{
  PSYSHOST                                   Host;
  struct channelNvram                        (*ChannelNvList)[MAX_CH];
  MEMORY_TIMINGS_CAS2CAS_DR_MCDDC_CTL_STRUCT TCas2CasDr = {0};
  MEMORY_TIMINGS_CAS2CAS_DD_MCDDC_CTL_STRUCT TCas2CasDd = {0};
  MEMORY_TIMINGS_CAS2CAS_SR_MCDDC_CTL_STRUCT TCas2CasSr = {0};
  MEMORY_TIMINGS_CAS2CAS_SG_MCDDC_CTL_STRUCT TCas2CasSg = {0};
  INT32                                      TurnaroundAbsoluteValue;

  Host          = (PSYSHOST) GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  if ((GsmReadOnly (Mode) == FALSE) && (GsmReadCsr (Mode) == FALSE) && (GsmForceWrite(Mode) == FALSE)) {
    return MRC_STATUS_FAILURE;
  }

  //
  // Read out original register value.
  //

  if (GsmReadCsr (Mode)) {
    //
    // Read from CSR.
    //

    if ((TurnaroundType == t_RRSG) || (TurnaroundType == t_WWSG) || (TurnaroundType == t_RWSG) || (TurnaroundType == t_WRSG)) {
      TCas2CasSg.Data = ChRegisterRead (MemTechType, Socket, Ch, MEMORY_TIMINGS_CAS2CAS_SG_MCDDC_CTL_REG);
    }

    if ((TurnaroundType == t_RRSR) || (TurnaroundType == t_WWSR) || (TurnaroundType == t_RWSR) || (TurnaroundType == t_WRSR)) {
      TCas2CasSr.Data = ChRegisterRead (MemTechType, Socket, Ch, MEMORY_TIMINGS_CAS2CAS_SR_MCDDC_CTL_REG);
    }

    if ((TurnaroundType == t_RRDR) || (TurnaroundType == t_WWDR) || (TurnaroundType == t_RWDR) || (TurnaroundType == t_WRDR)) {
      TCas2CasDr.Data = ChRegisterRead (MemTechType, Socket, Ch, MEMORY_TIMINGS_CAS2CAS_DR_MCDDC_CTL_REG);
    }

    if ((TurnaroundType == t_RRDD) || (TurnaroundType == t_WWDD) || (TurnaroundType == t_RWDD) || (TurnaroundType == t_WRDD)) {
      TCas2CasDd.Data = ChRegisterRead (MemTechType, Socket, Ch, MEMORY_TIMINGS_CAS2CAS_DD_MCDDC_CTL_REG);
    }
  } else {
    //
    // Read from cache.
    //
    if ((TurnaroundType == t_RRSG) || (TurnaroundType == t_WWSG) || (TurnaroundType == t_RWSG) || (TurnaroundType == t_WRSG)) {
      TCas2CasSg.Data = (*ChannelNvList)[Ch].mtCas2CasSg;
    }

    if ((TurnaroundType == t_RRSR) || (TurnaroundType == t_WWSR) || (TurnaroundType == t_RWSR) || (TurnaroundType == t_WRSR)) {
      TCas2CasSr.Data = (*ChannelNvList)[Ch].mtCas2CasSr;
    }

    if ((TurnaroundType == t_RRDR) || (TurnaroundType == t_WWDR) || (TurnaroundType == t_RWDR) || (TurnaroundType == t_WRDR)) {
      TCas2CasDr.Data = (*ChannelNvList)[Ch].mtCas2CasDr;
    }

    if ((TurnaroundType == t_RRDD) || (TurnaroundType == t_WWDD) || (TurnaroundType == t_RWDD) || (TurnaroundType == t_WRDD)) {
      TCas2CasDd.Data = (*ChannelNvList)[Ch].mtCas2CasDd;
    }
  }

  switch (TurnaroundType) {
    case t_RRSG:
      TurnaroundAbsoluteValue = TCas2CasSg.Bits.t_rrsg;
      break;
    case t_WWSG:
      TurnaroundAbsoluteValue = TCas2CasSg.Bits.t_wwsg;
      break;
    case t_RWSG:
      TurnaroundAbsoluteValue = TCas2CasSg.Bits.t_rwsg;
      break;
    case t_WRSG:
      TurnaroundAbsoluteValue = TCas2CasSg.Bits.t_wrsg;
      break;
    case t_RRSR:
      TurnaroundAbsoluteValue = TCas2CasSr.Bits.t_rrsr;
      break;
    case t_WWSR:
      TurnaroundAbsoluteValue = TCas2CasSr.Bits.t_wwsr;
      break;
    case t_RWSR:
      TurnaroundAbsoluteValue = TCas2CasSr.Bits.t_rwsr;
      break;
    case t_WRSR:
      TurnaroundAbsoluteValue = TCas2CasSr.Bits.t_wrsr;
      break;
    case t_RRDR:
      TurnaroundAbsoluteValue = TCas2CasDr.Bits.t_rrdr;
      break;
    case t_WWDR:
      TurnaroundAbsoluteValue = TCas2CasDr.Bits.t_wwdr;
      break;
    case t_RWDR:
      TurnaroundAbsoluteValue = TCas2CasDr.Bits.t_rwdr;
      break;
    case t_WRDR:
      TurnaroundAbsoluteValue = TCas2CasDr.Bits.t_wrdr;
      break;
    case t_RRDD:
      TurnaroundAbsoluteValue = TCas2CasDd.Bits.t_rrdd;
      break;
    case t_WWDD:
      TurnaroundAbsoluteValue = TCas2CasDd.Bits.t_wwdd;
      break;
    case t_RWDD:
      TurnaroundAbsoluteValue = TCas2CasDd.Bits.t_rwdd;
      break;
    case t_WRDD:
      TurnaroundAbsoluteValue = TCas2CasDd.Bits.t_wrdd;
      break;
    default:
      TurnaroundAbsoluteValue = 0;
  }

  //
  // Read only.
  //
  if (GsmReadOnly (Mode)) {
    *TurnaroundValue = TurnaroundAbsoluteValue;
    return MRC_STATUS_SUCCESS;
  }

  //
  // Write to CSR.
  //
  if (GsmForceWrite (Mode) || GsmWriteOffset (Mode)){

    if (GsmWriteOffset (Mode)){
      //
      // If the input value is offset.
      //
     TurnaroundAbsoluteValue = TurnaroundAbsoluteValue + *TurnaroundValue;
    } else {
      //
      // Force write the inupt value.
      //
      TurnaroundAbsoluteValue = *TurnaroundValue;
    }

    switch (TurnaroundType) {
      case t_RRSG:
        TCas2CasSg.Bits.t_rrsg = TurnaroundAbsoluteValue;
        break;
      case t_WWSG:
        TCas2CasSg.Bits.t_wwsg = TurnaroundAbsoluteValue;
        break;
      case t_RWSG:
        TCas2CasSg.Bits.t_rwsg = TurnaroundAbsoluteValue;
        break;
      case t_WRSG:
        TCas2CasSg.Bits.t_wrsg = TurnaroundAbsoluteValue;
        break;
      case t_RRSR:
        TCas2CasSr.Bits.t_rrsr = TurnaroundAbsoluteValue;
        break;
      case t_WWSR:
        TCas2CasSr.Bits.t_wwsr = TurnaroundAbsoluteValue;
        break;
      case t_RWSR:
        TCas2CasSr.Bits.t_rwsr = TurnaroundAbsoluteValue;
        break;
      case t_WRSR:
        TCas2CasSr.Bits.t_wrsr = TurnaroundAbsoluteValue;
        break;
      case t_RRDR:
        TCas2CasDr.Bits.t_rrdr = TurnaroundAbsoluteValue;
        break;
      case t_WWDR:
        TCas2CasDr.Bits.t_wwdr = TurnaroundAbsoluteValue;
        break;
      case t_RWDR:
        TCas2CasDr.Bits.t_rwdr = TurnaroundAbsoluteValue;
        break;
      case t_WRDR:
        TCas2CasDr.Bits.t_wrdr = TurnaroundAbsoluteValue;
        break;
      case t_RRDD:
        TCas2CasDd.Bits.t_rrdd = TurnaroundAbsoluteValue;
        break;
      case t_WWDD:
        TCas2CasDd.Bits.t_wwdd = TurnaroundAbsoluteValue;
        break;
      case t_RWDD:
        TCas2CasDd.Bits.t_rwdd = TurnaroundAbsoluteValue;
        break;
      case t_WRDD:
        TCas2CasDd.Bits.t_wrdd = TurnaroundAbsoluteValue;
        break;
    }

    //
    // Update register and cache.
    //

    if ((TurnaroundType == t_RRSG) || (TurnaroundType == t_WWSG) || (TurnaroundType == t_RWSG) || (TurnaroundType == t_WRSG)) {
      ChRegisterWrite (MemTechType, Socket, Ch, MEMORY_TIMINGS_CAS2CAS_SG_MCDDC_CTL_REG, TCas2CasSg.Data);
      (*ChannelNvList)[Ch].mtCas2CasSg = TCas2CasSg.Data;
    }

    if ((TurnaroundType == t_RRSR) || (TurnaroundType == t_WWSR) || (TurnaroundType == t_RWSR) || (TurnaroundType == t_WRSR)) {
      ChRegisterWrite (MemTechType, Socket, Ch, MEMORY_TIMINGS_CAS2CAS_SR_MCDDC_CTL_REG, TCas2CasSr.Data);
      (*ChannelNvList)[Ch].mtCas2CasSr = TCas2CasSr.Data;
    }

    if ((TurnaroundType == t_RRDR) || (TurnaroundType == t_WWDR) || (TurnaroundType == t_RWDR) || (TurnaroundType == t_WRDR)) {
      ChRegisterWrite (MemTechType, Socket, Ch, MEMORY_TIMINGS_CAS2CAS_DR_MCDDC_CTL_REG, TCas2CasDr.Data);
      (*ChannelNvList)[Ch].mtCas2CasDr = TCas2CasDr.Data;
    }

    if ((TurnaroundType == t_RRDD) || (TurnaroundType == t_WWDD) || (TurnaroundType == t_RWDD) || (TurnaroundType == t_WRDD)) {
      ChRegisterWrite (MemTechType, Socket, Ch, MEMORY_TIMINGS_CAS2CAS_DD_MCDDC_CTL_REG, TCas2CasDd.Data);
      (*ChannelNvList)[Ch].mtCas2CasDd = TCas2CasDd.Data;
    }

    return MRC_STATUS_SUCCESS;
  }

  return MRC_STATUS_SUCCESS;
}
