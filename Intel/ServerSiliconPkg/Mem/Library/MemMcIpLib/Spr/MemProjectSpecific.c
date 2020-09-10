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

#include <Library/SysHostPointerLib.h>
#include <SysHostChip.h>
#include "Include/MemMcRegs.h"
#include "Include/MemMcIpLibInternal.h"
#include <Library/PmicAccessLib.h>
#include <Library/MemAccessLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemRcLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <UncoreCommonIncludes.h>
#include <Library/MemRoutingLib.h>
#include <Library/KtiApi.h>

#define READ_WAIT_COUNT             0x32
#define READ_STARVE_COUNT           0x100

#define WMM_ENTRY_DDR               0x1C
#define WMM_EXIT_DDR                0x8
#define WMM_ENTRY_DDR_B0            0xF
#define WMM_EXIT_DDR_B0             0xA

#define WMM_READ_PREEMPTION         0x0
#define RMM_WRITE_PREEMPTION        0x1
#define OMM_MISS_PREEMPTION         0x1
#define OMM_HIT_PREEMPTION          0x0
#define OMM_HIT_PREEMPTION_B0       0x1

/**

  Config baseline performance settings for non-NVMDIMM and NVMDIMM configurations
  NVMDIMM configuration includes two profiles according to two different workload behavior
  - NVMDIMM BW OPTIMIZED
  - NVMDIMM Latency OPTIMIZED

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket to switch

  @retval N/A

**/
VOID
ProgramNvmdimmPerf (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
#ifdef GNRSRF_HOST
  UINT8                                         Ch;
  DDRT_MAJOR_MODE_THRESHOLD1_MC_2LM_STRUCT      DdrtMajorModeTh1;
  DDRT_MAJOR_MODE_THRESHOLD3_MC_2LM_STRUCT      MajorModeThreshold3;
  MODE_STARVE_CYCLE_THRESHOLD_MC_2LM_STRUCT     DdrtModeStrvCycleTh;
  MODE_DELAYS_MC_2LM_STRUCT                     ModeDelays;
  MODE_DDRT_CMD_STARVE_THRESHOLD_MC_2LM_STRUCT  DdrtCmdStarveThld;
  DDRT_MM_DDR4_CMD_THRESH_MCDDC_CTL_STRUCT      DdrtCmdThresh;
  struct channelNvram                           (*ChannelNvList)[MAX_CH];
  SYS_SETUP                                     *Setup;
  UINT8                                         MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    DdrtMajorModeTh1.Data = MemReadPciCfgEp (Socket, Ch, DDRT_MAJOR_MODE_THRESHOLD1_MC_2LM_REG);
    DdrtModeStrvCycleTh.Data = MemReadPciCfgEp (Socket, Ch, MODE_STARVE_CYCLE_THRESHOLD_MC_2LM_REG);
    MajorModeThreshold3.Data = MemReadPciCfgEp (Socket, Ch, DDRT_MAJOR_MODE_THRESHOLD3_MC_2LM_REG);
    ModeDelays.Data = MemReadPciCfgEp (Socket, Ch, MODE_DELAYS_MC_2LM_REG);
    DdrtCmdStarveThld.Data = MemReadPciCfgEp (Socket, Ch, MODE_DDRT_CMD_STARVE_THRESHOLD_MC_2LM_REG);
    DdrtCmdThresh.Data = MemReadPciCfgEp (Socket, Ch, DDRT_MM_DDR4_CMD_THRESH_MCDDC_CTL_REG);

    if ((*ChannelNvList)[Ch].ddrtEnabled == 0) {
      // Non-DCPMM settings
      DdrtCmdStarveThld.Bits.reads = 0x1E;
      DdrtCmdThresh.Bits.ddr4_rd_cmd_threshold = 0x1A;
      DdrtModeStrvCycleTh.Bits.ddr4 = 0x10;
      DdrtModeStrvCycleTh.Bits.ddrt = 0x32;
      MajorModeThreshold3.Bits.ddr4_critical_starve = 0x2;
      MajorModeThreshold3.Bits.ddrt_critical_starve = 0x2;
      ModeDelays.Bits.reqs = 0x10;
      ModeDelays.Bits.wmm_gnt2gnt_delay = 0x8;
    }
#ifdef DDRT_SUPPORT
    else {
      if (Setup->mem.NvmdimmPerfConfig == NVMDIMM_BW_OPTIMIZED) {
        // DCPMM bandwidth-optimized settings
        DdrtCmdStarveThld.Bits.reads = 0x1E;
        DdrtCmdThresh.Bits.ddr4_rd_cmd_threshold = 0x1A;
        DdrtModeStrvCycleTh.Bits.ddr4 = 0x20;
        DdrtModeStrvCycleTh.Bits.ddrt = 0x24;
        MajorModeThreshold3.Bits.ddrt_critical_starve = 0x7;
      } else if (Setup->mem.NvmdimmPerfConfig == NVMDIMM_LATENCY_OPTIMIZED) {
        // DCPMM latency-optimized settings
        DdrtCmdStarveThld.Bits.reads = 0x28;
        DdrtCmdThresh.Bits.ddr4_rd_cmd_threshold = 0x8;
        DdrtModeStrvCycleTh.Bits.ddr4 = 0x50;
        DdrtModeStrvCycleTh.Bits.ddrt = 0xC;
        MajorModeThreshold3.Bits.ddrt_critical_starve = 0xB;
      } else {
        // Assert on unsupported NvmdimmPerfConfig value
        ASSERT (FALSE);
      }

      // Shared DCPMM settings
      MajorModeThreshold3.Bits.ddr4_critical_starve = 0x1;
      ModeDelays.Bits.ddr4_to_ddrt_delay = 0x06;
      ModeDelays.Bits.ddrt_to_ddr4_delay = 0x03;
      if (IsSiliconWorkaroundEnabled ("S14010044313")) {
        ModeDelays.Bits.ddr4_to_ddrt_delay = 0x08;
        ModeDelays.Bits.ddrt_to_ddr4_delay = 0x04;
      }
      ModeDelays.Bits.reqs = 0x16;
      ModeDelays.Bits.wmm_gnt2gnt_delay = 0x13;

      // These fields have been implicitly cleared by casting the read from MODE_DELAYS_MC_2LM_REG as UINT16 since SKX
      // development in 2014 (if not earlier).
      ModeDelays.Bits.reset_timer = 0;
      ModeDelays.Bits.rsvd_0 = 0;
    }
#endif // DDRT_SUPPORT

    // Shared DCPMM and non-DCPMM settings
    DdrtCmdStarveThld.Bits.partials = 0x10;
    DdrtCmdStarveThld.Bits.writes = 0x18;
    DdrtCmdThresh.Bits.ddr4_wr_cmd_threshold = 0x1A;
    DdrtMajorModeTh1.Bits.min_rmm_cycles = 0x1E;
    DdrtMajorModeTh1.Bits.min_wmm_cycles = 0x3C;
    DdrtMajorModeTh1.Bits.wmm_enter = 0x12;
    DdrtMajorModeTh1.Bits.wmm_exit = 0x06;

    MemWritePciCfgEp (Socket, Ch, DDRT_MAJOR_MODE_THRESHOLD1_MC_2LM_REG, DdrtMajorModeTh1.Data);
    MemWritePciCfgEp (Socket, Ch, MODE_STARVE_CYCLE_THRESHOLD_MC_2LM_REG, DdrtModeStrvCycleTh.Data);
    MemWritePciCfgEp (Socket, Ch, DDRT_MAJOR_MODE_THRESHOLD3_MC_2LM_REG, MajorModeThreshold3.Data);
    MemWritePciCfgEp (Socket, Ch, MODE_DELAYS_MC_2LM_REG, ModeDelays.Data);
    MemWritePciCfgEp (Socket, Ch, MODE_DDRT_CMD_STARVE_THRESHOLD_MC_2LM_REG, DdrtCmdStarveThld.Data);
    MemWritePciCfgEp (Socket, Ch, DDRT_MM_DDR4_CMD_THRESH_MCDDC_CTL_REG, DdrtCmdThresh.Data);
  }

#else // GNRSRF_HOST

  UINT8                                          Ch;
  DDRT_MAJOR_MODE_THRESHOLD1_MC_2LM_STRUCT       DdrtMajorModeTh1;
  DDRT_MAJOR_MODE_THRESHOLD3_MC_2LM_STRUCT       MajorModeThreshold3;
  MODE_STARVE_CYCLE_THRESHOLD_MC_2LM_STRUCT      DdrtModeStrvCycleTh;
  MODE_DELAYS_MC_2LM_SPRA0_SPRB0_SPRHBM_STRUCT   ModeDelaysA0B0HBM;
  MODE_DELAYS_MC_2LM_SPRC0_SPRMCC_SPRUCC_STRUCT  ModeDelays;
  MODE_DDRT_CMD_STARVE_THRESHOLD_MC_2LM_STRUCT   DdrtCmdStarveThld;
  DDRT_MM_DDR4_CMD_THRESH_MCDDC_CTL_STRUCT       DdrtCmdThresh;
  struct channelNvram                            (*ChannelNvList)[MAX_CH];
  SYS_SETUP                                      *Setup;
  UINT8                                          MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    DdrtMajorModeTh1.Data = MemReadPciCfgEp (Socket, Ch, DDRT_MAJOR_MODE_THRESHOLD1_MC_2LM_REG);
    DdrtModeStrvCycleTh.Data = MemReadPciCfgEp (Socket, Ch, MODE_STARVE_CYCLE_THRESHOLD_MC_2LM_REG);
    MajorModeThreshold3.Data = MemReadPciCfgEp (Socket, Ch, DDRT_MAJOR_MODE_THRESHOLD3_MC_2LM_REG);
    if (IsCpuAndRevision (CPU_SPRSP, REV_A0) || IsCpuAndRevision (CPU_SPRSP, REV_B0)) {
      ModeDelaysA0B0HBM.Data = MemReadPciCfgEp (Socket, Ch, MODE_DELAYS_MC_2LM_REG);
    } else {
      ModeDelays.Data = MemReadPciCfgEp (Socket, Ch, MODE_DELAYS_MC_2LM_REG);
    }
    DdrtCmdStarveThld.Data = MemReadPciCfgEp (Socket, Ch, MODE_DDRT_CMD_STARVE_THRESHOLD_MC_2LM_REG);
    DdrtCmdThresh.Data = MemReadPciCfgEp (Socket, Ch, DDRT_MM_DDR4_CMD_THRESH_MCDDC_CTL_REG);

    if ((*ChannelNvList)[Ch].ddrtEnabled == 0) {
      // Non-DCPMM settings
      DdrtCmdStarveThld.Bits.reads = 0x1E;
      DdrtCmdThresh.Bits.ddr4_rd_cmd_threshold = 0x1A;
      DdrtModeStrvCycleTh.Bits.ddr4 = 0x10;
      DdrtModeStrvCycleTh.Bits.ddrt = 0x32;
      MajorModeThreshold3.Bits.ddr4_critical_starve = 0x2;
      MajorModeThreshold3.Bits.ddrt_critical_starve = 0x2;
      if (IsCpuAndRevision (CPU_SPRSP, REV_A0) || IsCpuAndRevision (CPU_SPRSP, REV_B0)) {
        ModeDelaysA0B0HBM.Bits.reqs = 0x10;
        ModeDelaysA0B0HBM.Bits.wmm_gnt2gnt_delay = 0x8;
      } else {
        ModeDelays.Bits.reqs = 0x10;
        ModeDelays.Bits.wmm_gnt2gnt_delay = 0x8;
      }
    }
#ifdef DDRT_SUPPORT
    else {
      if (Setup->mem.NvmdimmPerfConfig == NVMDIMM_BW_OPTIMIZED) {
        // DCPMM bandwidth-optimized settings
        DdrtCmdStarveThld.Bits.reads = 0x1E;
        DdrtCmdThresh.Bits.ddr4_rd_cmd_threshold = 0x1A;
        DdrtModeStrvCycleTh.Bits.ddr4 = 0x20;
        DdrtModeStrvCycleTh.Bits.ddrt = 0x24;
        MajorModeThreshold3.Bits.ddrt_critical_starve = 0x7;
      } else if (Setup->mem.NvmdimmPerfConfig == NVMDIMM_LATENCY_OPTIMIZED) {
        // DCPMM latency-optimized settings
        DdrtCmdStarveThld.Bits.reads = 0x28;
        DdrtCmdThresh.Bits.ddr4_rd_cmd_threshold = 0x8;
        DdrtModeStrvCycleTh.Bits.ddr4 = 0x50;
        DdrtModeStrvCycleTh.Bits.ddrt = 0xC;
        MajorModeThreshold3.Bits.ddrt_critical_starve = 0xB;
      } else {
        // Assert on unsupported NvmdimmPerfConfig value
        ASSERT (FALSE);
      }

      // Shared DCPMM settings
      MajorModeThreshold3.Bits.ddr4_critical_starve = 0x1;
      if (IsCpuAndRevision (CPU_SPRSP, REV_A0) || IsCpuAndRevision (CPU_SPRSP, REV_B0)) {
        ModeDelaysA0B0HBM.Bits.ddr4_to_ddrt_delay = 0x06;
        ModeDelaysA0B0HBM.Bits.ddrt_to_ddr4_delay = 0x03;
        if (IsSiliconWorkaroundEnabled ("S14010044313")) {
          ModeDelaysA0B0HBM.Bits.ddr4_to_ddrt_delay = 0x08;
          ModeDelaysA0B0HBM.Bits.ddrt_to_ddr4_delay = 0x04;
        }
        ModeDelaysA0B0HBM.Bits.reqs = 0x16;
        ModeDelaysA0B0HBM.Bits.wmm_gnt2gnt_delay = 0x13;
      } else {
        ModeDelays.Bits.ddr4_to_ddrt_delay = 0x06;
        ModeDelays.Bits.ddrt_to_ddr4_delay = 0x03;
        if (IsSiliconWorkaroundEnabled ("S14010044313")) {
          ModeDelays.Bits.ddr4_to_ddrt_delay = 0x08;
          ModeDelays.Bits.ddrt_to_ddr4_delay = 0x04;
        }
        ModeDelays.Bits.reqs = 0x16;
        ModeDelays.Bits.wmm_gnt2gnt_delay = 0x13;
      }

      // These fields have been implicitly cleared by casting the read from MODE_DELAYS_MC_2LM_REG as UINT16 since SKX
      // development in 2014 (if not earlier).
      if (IsCpuAndRevision (CPU_SPRSP, REV_A0) || IsCpuAndRevision (CPU_SPRSP, REV_B0)) {
        ModeDelaysA0B0HBM.Bits.spare = 0;
        ModeDelaysA0B0HBM.Bits.reset_timer = 0;
        ModeDelaysA0B0HBM.Bits.rsvd = 0;
      } else {
        ModeDelays.Bits.reset_timer = 0;
        ModeDelays.Bits.rsvd = 0;
      }
    }
#endif // DDRT_SUPPORT

    // Shared DCPMM and non-DCPMM settings
    DdrtCmdStarveThld.Bits.partials = 0x10;
    DdrtCmdStarveThld.Bits.writes = 0x18;
    DdrtCmdThresh.Bits.ddr4_wr_cmd_threshold = 0x1A;
    DdrtMajorModeTh1.Bits.min_rmm_cycles = 0x1E;
    DdrtMajorModeTh1.Bits.min_wmm_cycles = 0x3C;
    DdrtMajorModeTh1.Bits.wmm_enter = 0x12;
    DdrtMajorModeTh1.Bits.wmm_exit = 0x06;

    MemWritePciCfgEp (Socket, Ch, DDRT_MAJOR_MODE_THRESHOLD1_MC_2LM_REG, DdrtMajorModeTh1.Data);
    MemWritePciCfgEp (Socket, Ch, MODE_STARVE_CYCLE_THRESHOLD_MC_2LM_REG, DdrtModeStrvCycleTh.Data);
    MemWritePciCfgEp (Socket, Ch, DDRT_MAJOR_MODE_THRESHOLD3_MC_2LM_REG, MajorModeThreshold3.Data);
    if (IsCpuAndRevision (CPU_SPRSP, REV_A0) || IsCpuAndRevision (CPU_SPRSP, REV_B0)) {
      MemWritePciCfgEp (Socket, Ch, MODE_DELAYS_MC_2LM_REG, ModeDelaysA0B0HBM.Data);
    } else {
      MemWritePciCfgEp (Socket, Ch, MODE_DELAYS_MC_2LM_REG, ModeDelays.Data);
    }
    MemWritePciCfgEp (Socket, Ch, MODE_DDRT_CMD_STARVE_THRESHOLD_MC_2LM_REG, DdrtCmdStarveThld.Data);
    MemWritePciCfgEp (Socket, Ch, DDRT_MM_DDR4_CMD_THRESH_MCDDC_CTL_REG, DdrtCmdThresh.Data);
  }
#endif // GNRSRF_HOST
}

/**

  Programs MC to enable Write CRC

  @param[in] Host   - Pointer to sysHost
  @param[in] Socket - Socket number
  @param[in] Ch     - Channel number

  @retval None

**/
VOID
EnableWrCrcMc (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT8     Ch
  )
{
  DDR4_WRCRC_RANK_CTL_MCDDC_DP_STRUCT Ddr4WrcrcRankCtl;

  //
  // "DDR4" register applies to DDR5 as well.
  //
  Ddr4WrcrcRankCtl.Data = MemReadPciCfgEp (Socket, Ch, DDR4_WRCRC_RANK_CTL_MCDDC_DP_REG);
  Ddr4WrcrcRankCtl.Bits.cr_wrcrc_enable = 1;
  MemWritePciCfgEp (Socket, Ch, DDR4_WRCRC_RANK_CTL_MCDDC_DP_REG, Ddr4WrcrcRankCtl.Data);
}

/**

  Configure memory mode DDR5 or DDR4

  @param[in] Host   - Pointer to sysHost
  @param[in] Socket - Socket number

  @retval None

**/
VOID
ConfigureMemoryMode (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket
  )
{
  UINT8 Ch;
  AMAP_MC_MAIN_STRUCT AmapMcMain;
  MEMORY_TIMINGS_ADJ_MCDDC_CTL_STRUCT MemoryTimingsAdj;
  struct channelNvram                           (*ChannelNvList) [MAX_CH];
  UINT8                                         MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList) [Ch].enabled == 0) {
      continue;
    }
    if (IsDdr5Present (Host, Socket)) {
      //
      // Configure to DDR5 mode
      //
      AmapMcMain.Data = MemReadPciCfgEp (Socket, Ch, AMAP_MC_MAIN_REG);
      AmapMcMain.Bits.device_type = 1;
      MemWritePciCfgEp (Socket, Ch, AMAP_MC_MAIN_REG , AmapMcMain.Data);

      MemoryTimingsAdj.Data = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG);
      MemoryTimingsAdj.Bits.dram_device_type = 1;
      if (Host->nvram.mem.dimmTypePresent == RDIMM) {
        MemoryTimingsAdj.Bits.rcd_on_dimm = 1;
      }
      MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG , MemoryTimingsAdj.Data);
    }
  } //ch loop
}

/**
  Get the maximum number of data groups based on DIMM and group type

  @param[in] Host  - Pointer to sysHost
  @param[in] Socket  - Socket
  @param[in] Ch    - Channel
  @param[in] Dimm  - DIMM
  @param[in] Group - Group type
  @param[in] Level - Level type

  @retval Max strobe

**/
UINT8
GetMaxStrobeDdr5(
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN MRC_GT    Group,
  IN MRC_LT    Level
  )
{
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];
  UINT8 MSVx4 = GetSubChMaxStrobeValid(Host);
  UINT8 MSVx8 = GetSubChMaxStrobeValid(Host) / 2;

  DimmNvList = &Host->nvram.mem.socket[Socket].channelList[Ch].dimmList;

#ifdef LRDIMM_SUPPORT
  if ((IsLrdimmPresent (Socket, Ch, Dimm)) && (Group == TxVref) && (Level == DdrLevel)) {
    return MSVx8;
  }
  if ((IsLrdimmPresent (Socket, Ch, Dimm)) && (Group == TxVref) && (Level == LrbufLevel)) {
    return MSVx4;
  }
  if ((IsLrdimmPresent (Socket, Ch, Dimm)) && (Group == RxVref) && (Level == LrbufLevel)) {
    return MSVx8;
  }
#endif
  //
  // Setup variables based on x4 or x8 DIMM or chipset specific
  //
  if ((Group == TxEq) || (Group == TxImode) || (Group == RxEq)  ||
      (Group == TxTco) || (Group == RxCtleC) || (Group == RxCtleR) ||
      (Group == TxEqTap2) || ((Group == RxVref) && (Level == DdrLevel))) {
    return MSVx8;
  } else if ((*DimmNvList)[Dimm].x4Present ||
      (Group == RxDqsPDelay) || (Group == RxDqsNDelay) || (Group == RxDqDqsDelay) ||
      (Group == RxDqDelay) || (Group == RxDqsDelay) || (Group == RecEnDelay) ||
      (Group == TxDqDelay) || (Group == RxVref) || (Group == RxEq) ||
      (Group == TxEq) || (Group == TxEqTap2) || (Group == RxFlyBy) || (Group == TxFlyBy)) {
    return MSVx4;
  } else {
    return MSVx8;
  }
} // GetMaxStrobeDdr5

/**

  Apply DDRT CMI Workaround
  2S Supercollider causes TOR Time Out - Stuck FMFILLRD on DDRT Channel

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Current Socket

  @retval N/A

**/
VOID
DdrtCmi2SSuperColliderWa (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  return;
} // DdrtCmi2SSuperColliderWa

/**

  Ddrt Clear Ck Idle Mask

  @param[in] Socket    - Current Socket

  @retval N/A

**/
VOID
DdrtClearCkIdleMask (
  IN UINT8     Socket
  )
{
  FnvEnableResyncFsm (Socket);
} // DdrtClearCkIdleMask

/**

  PMIC Wrapper Function
  Enables the PMIC on the specified DIMM

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket number

  @retval SUCCESS;

**/
UINT32
PmicEnableChip (
  IN PSYSHOST  Host,
  IN UINT8     Socket
)
{
  UINT8                             Ch;
  UINT8                             Dimm;
  struct channelNvram               (*ChannelNvList)[MAX_CH];
  struct dimmNvram                  (*DimmNvList)[MAX_DIMM];
  UINT8                             MaxChDdr;
  MaxChDdr      = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Ch, Dimm) && (!(IsDcpmmPresentDimm (Socket, Ch, Dimm)))) {
        PmicEnable (Socket, Ch, Dimm);
      }
    }
  }

  return SUCCESS;

} // PmicEnableChip

/**
  Configure idle bypass timings

  @param[in] MemTechType  - Memory Technology Type
  @param[in] Socket       - Socket number
  @param[in] Ch           - Channel number

  @retval N/A

**/
VOID
EnableBsIdleBypass (
  IN  MEM_TECH_TYPE  MemTechType,
  IN  UINT8          Socket,
  IN  UINT8          Ch
  )
{
  SCHEDULER_ENABLES_BS_MCDDC_CTL_STRUCT                   SchedulerEnablesBs;

  SchedulerEnablesBs.Data = ChRegisterRead (MemTechType, Socket, Ch, SCHEDULER_ENABLES_BS_MCDDC_CTL_REG);
  // Scheduler Enables
  SchedulerEnablesBs.Bits.idle_bypass_en = 1;
  ChRegisterWrite (MemTechType, Socket, Ch, SCHEDULER_ENABLES_BS_MCDDC_CTL_REG, SchedulerEnablesBs.Data);
}


/**
  Get the silicon specific scheduler read starve count and wait count value

  @param[out] StarveCount - scheduler read starve count
  @param[out] StarveCount - scheduler read wait count

  @retval   N/A
**/
VOID
GetReadStarveAndWaitCount (
  OUT UINT32*  StarveCount,
  OUT UINT32*  WaitCount
  )
{
  *StarveCount = READ_STARVE_COUNT;
  *WaitCount = READ_WAIT_COUNT;
}

/**
  Get the silicon specific scheduler WMM enter and exit threshold

  @param[out] Enter - enter threshold
  @param[out] Exit - exit threshold

  @retval   N/A
**/
VOID
GetWmmThreshold (
  OUT UINT32*  EnterThreshold,
  OUT UINT32*  ExitThreshold
  )
{
  if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) {
    *EnterThreshold = WMM_ENTRY_DDR_B0;
    *ExitThreshold = WMM_EXIT_DDR_B0;
  } else {
    *EnterThreshold = WMM_ENTRY_DDR;
    *ExitThreshold = WMM_EXIT_DDR;
  }
}

/**
  Get the silicon specific scheduler preemption enable setting.

  @param[out] Read  - enable WMM read preemption
  @param[out] Write - enable RMM write preemption
  @param[out] Miss  - enable WMM read preemption
  @param[out] Hit   - enable RMM write preemption

  @retval   N/A
**/
VOID
GetPreemptionEnableSetting (
  OUT UINT32*  Read,
  OUT UINT32*  Write,
  OUT UINT32*  Miss,
  OUT UINT32*  Hit
  )
{
  *Read = WMM_READ_PREEMPTION;
  *Write = RMM_WRITE_PREEMPTION;
  *Miss = OMM_MISS_PREEMPTION;
  if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) {
    *Hit = OMM_HIT_PREEMPTION_B0;
  } else {
    *Hit = OMM_HIT_PREEMPTION;
  }
}

/**
  Enables Wait DDRT Ref Idle for all channels with DDRT DIMMs installed if the SI
  workaround is enabled

  @param[in]  Host    Pointer to SysHost
  @param[in]  Socket  Socket ID

  @retval     None
**/
VOID
EFIAPI
SetWaitDdrtRefIdle (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  return;
}

/**
  Set post-refresh-enable chicken bits.

  For a given channel, set any chicken bits for the chip which need to happen
  after refresh is enabled.

  @param[in]  Host      Pointer to sysHost
  @param[in]  Socket    Socket number
**/
VOID
SetPostRefreshEnableChickenBits (
  IN PSYSHOST   Host,
  IN UINT8      Socket
  )
{
  struct channelNvram               (*ChannelNvList)[MAX_CH];
  UINT8                             MaxChDdr;
  UINT8                             Channel;
  MCMNT_CHKN_BIT2_MCDDC_CTL_STRUCT  McMntChknBit2;

  //
  // If not DDR5, nothing to do.
  //
  if (!IsDdr5Present (Host, Socket)) {
    return;
  }

  ChannelNvList = GetChannelNvList (Host, Socket);
  MaxChDdr = GetMaxChDdr ();

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if ((*ChannelNvList)[Channel].enabled == 0) {
      continue;
    }

    //
    // Set chicken bits in the channel.
    //
    // For SPR:
    //   1. Enable DQS retraining (dqsosc_en = 1, dqsosc_runtime = MR45 value, dqsosc_blkchl = 1),
    //      except in emulation
    //      If DQS Retraining is enabled (dqsosc_en = 1), then we need to be blocking the whole channel
    //      while it runs (by setting dqsosc_blkchl = 1) otherwise DQS MR reads will collide with other reads.
    //   2. If DDRT2 is present in the channel, disable blocking SRE entry
    //      between ZQCalStart/Latch pair (zqcal_block_sre_dis = 1)
    //
    McMntChknBit2.Data = MemReadPciCfgEp (Socket, Channel, MCMNT_CHKN_BIT2_MCDDC_CTL_REG);
    if (!UbiosGenerationOrHsleEnabled ()) {
      if (IsMemFlowEnabled (PeriodicTxReTraining)) {
        RcDebugPrintWithDevice (
          SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "dqsosc_en = 1\n"
          );
        McMntChknBit2.Bits.dqsosc_en = 1;
      } else {
        RcDebugPrintWithDevice (
          SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "dqsosc_en = 0\n"
          );
        McMntChknBit2.Bits.dqsosc_en = 0;
      }
      McMntChknBit2.Bits.dqsosc_blkchl = 1;
      McMntChknBit2.Bits.dqsosc_runtime = DDR5_DQS_INTERVAL_TIMER_RUNTIME;
    }
    if ((*ChannelNvList)[Channel].ddrtEnabled == 1) {
      McMntChknBit2.Bits.zqcal_block_sre_dis = 1;
    }
    MemWritePciCfgEp (Socket, Channel, MCMNT_CHKN_BIT2_MCDDC_CTL_REG, McMntChknBit2.Data);
  }
}

/**
  Checks the support for widths mix withing a channel

  @param[in] Socket - Socket number
  @param[in] Ch     - Channel number

  @return TRUE if the mixing is supported,
          FALSE if not
**/
BOOLEAN
IsMixingWidthsSupported (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
  return TRUE;
}

/**

  Get CkIndex and CsIndex per Soc

  @param[in]  Socket   - Socket Id
  @param[in]  Ch       - Channel number (0-based)
  @param[in]  Dimm     - DIMM number (0-based)
  @param[in]  Rank     - Rank number (0-based)
  @param[in]  DimmType - Dimm Type
  @param[out] CkIndex  - Pointer to CkIndex
  @param[out] CsIndex  - Pointer to CsIndex

  @retval N/A

**/
VOID
GetClkCsIndexSoc (
  IN     UINT8   Socket,
  IN     UINT8   Ch,
  IN     UINT8   Dimm,
  IN     UINT8   Rank,
  IN     UINT8   DimmType,
  IN OUT UINT8   *CkIndex,
  IN OUT UINT8   *CsIndex
  )
{
  //
  // Get CK Index
  // CkIndex in SPR
  // - RDIMM
  //   Dimm 0: CkIndex 0
  //   Dimm 1: CkIndex 1
  //
  *CkIndex = Dimm;

  //
  // Get CS Index
  // CsIndex in SPR
  //  Dimm 0 Rank 0: CsIndex 0
  //  Dimm 0 Rank 1: CsIndex 1
  //  Dimm 1 Rank 0: CsIndex 2
  //  Dimm 1 Rank 1: CsIndex 3
  //
  *CsIndex = (Dimm << 1) | Rank;
}

/**
  Disable Open Page Mode.

  @param[in] MemTechType  Memory technology type.
  @param[in] Socket       Socket number.
  @param[in] Ch           Channel number within a socket.

  @retVal   None

**/
VOID
DisableOpenPageMode (
  IN MEM_TECH_TYPE   MemTechType,
  IN UINT8           Socket,
  IN UINT8           Ch
  )
{
  SCHEDULER_ENABLES_BS_MCDDC_CTL_STRUCT         SchedulerEnablesBs;
  MCMTR_MC_MAIN_STRUCT                          McmtrReg;

  McmtrReg.Data = ChRegisterRead (MemTechType, Socket, Ch, MCMTR_MC_MAIN_REG);
  McmtrReg.Bits.close_pg = CLOSED_PAGE_EN;
  ChRegisterWrite (MemTechType, Socket, Ch, MCMTR_MC_MAIN_REG, McmtrReg.Data);

  SchedulerEnablesBs.Data   = ChRegisterRead (MemTechType, Socket, Ch, SCHEDULER_ENABLES_BS_MCDDC_CTL_REG);
  SchedulerEnablesBs.Bits.open_page_mode_en = OPEN_PAGE_DIS;
  ChRegisterWrite (MemTechType, Socket, Ch, SCHEDULER_ENABLES_BS_MCDDC_CTL_REG, SchedulerEnablesBs.Data);

  return;
}

/**
Initializes MC for cpgc training

  @param[in] Socket     - Socket number
  @param[in] Ch         - Channel number

  @retVal   None

**/
VOID
InitMcForCpgcTraining (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
#ifdef GNRSRF_HOST
  MCDP_CMI_QDEPTH_CFG_MCDDC_DP_STRUCT                                  McDpCmiQdepthCreditConfig;
  PSYSHOST                                                             Host;
  MC_CPGC_CTL_MC_MAIN_STRUCT                                           McCpgcCtl;
  CPGC_MISCREFCTL_MCDDC_CTL_STRUCT                                     CpgcMiscRefCtlReg;
  MC_CPGC_RANK_MAP_MC_MAIN_STRUCT                                      McCpgcRankMap;
  MC_CPGC_SUBRANK_MAP_MC_MAIN_STRUCT                                   McCpgcSubrankMap;
  SCHEDULER_ENABLES_BS_MCDDC_CTL_STRUCT                                SchedulerEnablesBs;
  SCHEDULER_ENABLES_MCDDC_CTL_STRUCT                                   SchedulerEnables;
  MCSCRAMBLECONFIG_MCDDC_DP_STRUCT                                     ScrambleConfig;
  SCHEDULER_RID2_MCDDC_CTL_STRUCT                                      SchedulerRid2;
  SCHEDULER_RID_STATUS_MCDDC_CTL_STRUCT                                SchedulerRidStatus;
  MCMTR_MC_MAIN_STRUCT                                                 McMtr;
  MCMTR2_MC_MAIN_STRUCT                                                McMtr2;
  CPGC_MISCZQCTL_MCDDC_CTL_STRUCT                                      CpgcMiscZqCtlReg;

  Host = GetSysHostPointer ();

  // per ch
  McDpCmiQdepthCreditConfig.Data = MemReadPciCfgEp (Socket, Ch, MCDP_CMI_QDEPTH_CFG_MCDDC_DP_REG);
  McDpCmiQdepthCreditConfig.Bits.vc0_rd_cpl_credits = 0;
  McDpCmiQdepthCreditConfig.Bits.vc1_rd_cpl_credits = 0;
  McDpCmiQdepthCreditConfig.Bits.vc2_rd_cpl_credits = 0;
  McDpCmiQdepthCreditConfig.Bits.vc3_rd_cpl_credits = 0;
  McDpCmiQdepthCreditConfig.Bits.rd_cpl_shared_credits = 0;
  McDpCmiQdepthCreditConfig.Bits.rd_cpl_qdepth_vc_vec = 0;
  MemWritePciCfgEp (Socket, Ch, MCDP_CMI_QDEPTH_CFG_MCDDC_DP_REG, McDpCmiQdepthCreditConfig.Data);

  McCpgcCtl.Data = MemReadPciCfgEp (Socket, Ch, MC_CPGC_CTL_MC_MAIN_REG);
  McCpgcCtl.Bits.cpgc_safe_mode = 0;
  McCpgcCtl.Bits.cpgc_reset = 0;
  McCpgcCtl.Bits.cpgc_htid_replace = 0;
  McCpgcCtl.Bits.cpgc_htid_reset = 0;
  McCpgcCtl.Bits.cpgc_reqlen_replace = 0;
  McCpgcCtl.Bits.cpgc_disable_sb_wakeup = 0;
  McCpgcCtl.Bits.cpgc_nocs_rank_en = 0;
  McCpgcCtl.Bits.cpgc_nocs_rank_match = 0;
  McCpgcCtl.Bits.cpgc_mixed_ddrt_rank_en = 0;
  McCpgcCtl.Bits.cpgc_mixed_ddrt_rank_match = 0;
  McCpgcCtl.Bits.cpgc_bank_map = 1;
  McCpgcCtl.Bits.cpgc_use_chan_id = 1;
  MemWritePciCfgEp (Socket, Ch, MC_CPGC_CTL_MC_MAIN_REG , McCpgcCtl.Data);

  McCpgcRankMap.Data = MemReadPciCfgEp (Socket, Ch, MC_CPGC_RANK_MAP_MC_MAIN_REG);
  McCpgcRankMap.Bits.rank0_mapping = 0;
  McCpgcRankMap.Bits.rank1_mapping = 1;
  McCpgcRankMap.Bits.rank2_mapping = 2;
  McCpgcRankMap.Bits.rank3_mapping = 3;
  McCpgcRankMap.Bits.rank4_mapping = 4;
  McCpgcRankMap.Bits.rank5_mapping = 5;
  McCpgcRankMap.Bits.rank6_mapping = 6;
  McCpgcRankMap.Bits.rank7_mapping = 7;
  MemWritePciCfgEp (Socket, Ch, MC_CPGC_RANK_MAP_MC_MAIN_REG , McCpgcRankMap.Data);

  McCpgcSubrankMap.Data = MemReadPciCfgEp (Socket, Ch, MC_CPGC_SUBRANK_MAP_MC_MAIN_REG);
  McCpgcSubrankMap.Bits.rank0_mapping = 0;
  McCpgcSubrankMap.Bits.rank1_mapping = 0;
  McCpgcSubrankMap.Bits.rank2_mapping = 0;
  McCpgcSubrankMap.Bits.rank3_mapping = 0;
  McCpgcSubrankMap.Bits.rank4_mapping = 0;
  McCpgcSubrankMap.Bits.rank5_mapping = 0;
  McCpgcSubrankMap.Bits.rank6_mapping = 0;
  McCpgcSubrankMap.Bits.rank7_mapping = 0;
  MemWritePciCfgEp (Socket, Ch, MC_CPGC_SUBRANK_MAP_MC_MAIN_REG , McCpgcSubrankMap.Data);

  CpgcMiscRefCtlReg.Data  = MemReadPciCfgEp (Socket, Ch, CPGC_MISCREFCTL_MCDDC_CTL_REG);
  CpgcMiscRefCtlReg.Bits.ref_rankmask    = 0;
  CpgcMiscRefCtlReg.Bits.panic_ref_only  = 0;
  MemWritePciCfgEp (Socket, Ch, CPGC_MISCREFCTL_MCDDC_CTL_REG, CpgcMiscRefCtlReg.Data);

  CpgcMiscZqCtlReg.Data   = MemReadPciCfgEp (Socket, Ch, CPGC_MISCZQCTL_MCDDC_CTL_REG);
  CpgcMiscZqCtlReg.Bits.zq_rankmask       = 0;
  CpgcMiscZqCtlReg.Bits.always_do_zq      = 0;
  MemWritePciCfgEp (Socket, Ch, CPGC_MISCZQCTL_MCDDC_CTL_REG, CpgcMiscZqCtlReg.Data);

  SchedulerEnablesBs.Data   = MemReadPciCfgEp (Socket, Ch, SCHEDULER_ENABLES_BS_MCDDC_CTL_REG);
  SchedulerEnablesBs.Bits.llt_page_mode_en       = 0;
  SchedulerEnablesBs.Bits.link_major_mode_en     = 1;
  SchedulerEnablesBs.Bits.open_page_mode_en      = 1;
  SchedulerEnablesBs.Bits.idle_bypass_en         = 0;
  SchedulerEnablesBs.Bits.llt_page_mode_enhanced = 0;
  SchedulerEnablesBs.Bits.disable_buddy_mm_col   = 1;
  SchedulerEnablesBs.Bits.disable_buddy_mm_row   = 1;
  SchedulerEnablesBs.Bits.number_of_banks        = 2;
  SchedulerEnablesBs.Bits.high_rank_support      = 0;
  SchedulerEnablesBs.Bits.cpgc_inorder_mode_en   = 1;
  SchedulerEnablesBs.Bits.cpgc_second_cas_en     = 0;
  SchedulerEnablesBs.Bits.implicit_pre_en        = 0;
  SchedulerEnablesBs.Bits.page_table_index_selection = 0;
  SchedulerEnablesBs.Bits.bank_scheduler_selection   = 0;
  SchedulerEnablesBs.Bits.pseudo_channel_selection   = 0;
  MemWritePciCfgEp (Socket, Ch, SCHEDULER_ENABLES_BS_MCDDC_CTL_REG, SchedulerEnablesBs.Data);

  SchedulerEnables.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_ENABLES_MCDDC_CTL_REG);
  SchedulerEnables.Bits.idle_bypass_en      = 0;
  SchedulerEnables.Bits.bank_idle_bypass_en = 0;
  SchedulerEnables.Bits.xpq_llt_en          = 0;
  SchedulerEnables.Bits.rpq_vc1_llt_en      = 0;
  SchedulerEnables.Bits.deprd_after_write   = 0;
  SchedulerEnables.Bits.deprd_wpq_threshold = 1;
  SchedulerEnables.Bits.cpgc_inorder_mode_en = 1;
  SchedulerEnables.Bits.cpgc_second_cas_en  = 0;
  SchedulerEnables.Bits.page_table_index_selection = 0;
  SchedulerEnables.Bits.bank_scheduler_selection   = 0;
  SchedulerEnables.Bits.pseudo_channel_selection   = 0;
  MemWritePciCfgEp (Socket, Ch, SCHEDULER_ENABLES_MCDDC_CTL_REG, SchedulerEnables.Data);

  ScrambleConfig.Data = MemReadPciCfgEp (Socket, Ch, MCSCRAMBLECONFIG_MCDDC_DP_REG);
  ScrambleConfig.Bits.tx_enable = 0;
  ScrambleConfig.Bits.rx_enable = 0;
  ScrambleConfig.Bits.ch_enable = 0;
  ScrambleConfig.Bits.seed_lock = 0;
  ScrambleConfig.Bits.tx_enable_ddrt = 0;
  ScrambleConfig.Bits.rx_enable_ddrt = 0;
  ScrambleConfig.Bits.ch_enable_ddrt = 0;
  ScrambleConfig.Bits.scr_dis_rank = 0;
  MemWritePciCfgEp (Socket, Ch, MCSCRAMBLECONFIG_MCDDC_DP_REG, ScrambleConfig.Data);

  SchedulerRid2.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_RID2_MCDDC_CTL_REG);
  SchedulerRid2.Bits.enable_rid  = 0xff;
  MemWritePciCfgEp (Socket, Ch, SCHEDULER_RID2_MCDDC_CTL_REG, SchedulerRid2.Data);

  SchedulerRidStatus.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_RID_STATUS_MCDDC_CTL_REG );
  SchedulerRidStatus.Bits.rid_monitor_selection  = 0;
  MemWritePciCfgEp (Socket, Ch, SCHEDULER_RID_STATUS_MCDDC_CTL_REG , SchedulerRidStatus.Data);

  McMtr.Data = MemReadPciCfgEp (Socket, Ch, MCMTR_MC_MAIN_REG );
  McMtr.Bits.close_pg           = 0x0;
  McMtr.Bits.ecc_en             = 0x0;
  McMtr.Bits.dir_en             = 0x1;
  McMtr.Bits.read_dbi_en        = 0x0;
  McMtr.Bits.write_dbi_en       = 0x0;
  McMtr.Bits.read_parity_en     = 0x0;
  McMtr.Bits.write_parity_en    = 0x0;
  McMtr.Bits.normal             = 0x0;
  McMtr.Bits.bank_xor_enable    = 0x0;
  McMtr.Bits.trng_mode          = 0x1;
  McMtr.Bits.ddr5_9x4_en        = 0x0;
  McMtr.Bits.dimm_xor_enable    = 0x0;
  McMtr.Bits.chn_disable        = 0x0;
  McMtr.Bits.pch_xor_enable     = 0x0;
  McMtr.Bits.channel_xor_enable = 0x1;
  McMtr.Bits.cluster_xor_enable = 0x0;
  McMtr.Bits.ch23cmd_ctl_delay  = 0x0;
  McMtr.Bits.rank_xor_enable    = 0x0;
  McMtr.Bits.sid_xor_enable     = 0x0;
  McMtr.Bits.trng_target        = 0x0;
  McMtr.Bits.num_cluster        = 0x0;
  McMtr.Bits.hbm_mc_mode        = 0x0;
  McMtr.Bits.cpgc_active        = 0x0;
  MemWritePciCfgEp (Socket, Ch, MCMTR_MC_MAIN_REG , McMtr.Data);

  McMtr2.Data = MemReadPciCfgEp (Socket, Ch, MCMTR2_MC_MAIN_REG);
  McMtr2.Bits.monroe_chn_force_sr  = 0;
  McMtr2.Bits.adddc_exclusive2     = 1;
  MemWritePciCfgEp (Socket, Ch, MCMTR2_MC_MAIN_REG, McMtr2.Data);

#else // GNRSRF_HOST

  MCDP_CMI_QDEPTH_CFG_MCDDC_DP_STRUCT                                     McDpCmiQdepthCreditConfig;
  PSYSHOST                                                                Host;
  MC_CPGC_CTL_MC_MAIN_STRUCT                                              McCpgcCtl;
  CPGC_MISCREFCTL_MCDDC_CTL_STRUCT                                        CpgcMiscRefCtlReg;
  MC_CPGC_RANK_MAP_MC_MAIN_STRUCT                                         McCpgcRankMap;
  MC_CPGC_SUBRANK_MAP_MC_MAIN_STRUCT                                      McCpgcSubrankMap;
  SCHEDULER_ENABLES_BS_MCDDC_CTL_SPRA0_STRUCT                             SchedulerEnablesBsA0;
  SCHEDULER_ENABLES_BS_MCDDC_CTL_SPRB0_SPRHBM_SPRC0_SPRMCC_SPRUCC_STRUCT  SchedulerEnablesBs;
  SCHEDULER_ENABLES_MCDDC_CTL_STRUCT                                      SchedulerEnables;
  MCSCRAMBLECONFIG_MCDDC_DP_STRUCT                                        ScrambleConfig;
  SCHEDULER_RID2_MCDDC_CTL_STRUCT                                         SchedulerRid2;
  SCHEDULER_RID_STATUS_MCDDC_CTL_STRUCT                                   SchedulerRidStatus;
  MCMTR_MC_MAIN_STRUCT                                                    McMtr;
  MCMTR2_MC_MAIN_STRUCT                                                   McMtr2;
  CPGC_MISCZQCTL_MCDDC_CTL_STRUCT                                         CpgcMiscZqCtlReg;

  Host = GetSysHostPointer ();

  // per ch
  McDpCmiQdepthCreditConfig.Data = MemReadPciCfgEp (Socket, Ch, MCDP_CMI_QDEPTH_CFG_MCDDC_DP_REG);
  McDpCmiQdepthCreditConfig.Bits.vc0_rd_cpl_credits = 0;
  McDpCmiQdepthCreditConfig.Bits.vc1_rd_cpl_credits = 0;
  McDpCmiQdepthCreditConfig.Bits.vc2_rd_cpl_credits = 0;
  McDpCmiQdepthCreditConfig.Bits.vc3_rd_cpl_credits = 0;
  McDpCmiQdepthCreditConfig.Bits.rd_cpl_shared_credits = 0;
  McDpCmiQdepthCreditConfig.Bits.rd_cpl_qdepth_vc_vec = 0;
  MemWritePciCfgEp (Socket, Ch, MCDP_CMI_QDEPTH_CFG_MCDDC_DP_REG, McDpCmiQdepthCreditConfig.Data);

  McCpgcCtl.Data = MemReadPciCfgEp (Socket, Ch, MC_CPGC_CTL_MC_MAIN_REG);
  McCpgcCtl.Bits.cpgc_safe_mode = 0;
  McCpgcCtl.Bits.cpgc_reset = 0;
  McCpgcCtl.Bits.cpgc_htid_replace = 0;
  McCpgcCtl.Bits.cpgc_htid_reset = 0;
  McCpgcCtl.Bits.cpgc_reqlen_replace = 0;
  McCpgcCtl.Bits.cpgc_disable_sb_wakeup = 0;
  McCpgcCtl.Bits.cpgc_nocs_rank_en = 0;
  McCpgcCtl.Bits.cpgc_nocs_rank_match = 0;
  McCpgcCtl.Bits.cpgc_mixed_ddrt_rank_en = 0;
  McCpgcCtl.Bits.cpgc_mixed_ddrt_rank_match = 0;
  McCpgcCtl.Bits.cpgc_bank_map = 1;
  McCpgcCtl.Bits.cpgc_use_chan_id = 1;
  MemWritePciCfgEp (Socket, Ch, MC_CPGC_CTL_MC_MAIN_REG , McCpgcCtl.Data);

  McCpgcRankMap.Data = MemReadPciCfgEp (Socket, Ch, MC_CPGC_RANK_MAP_MC_MAIN_REG);
  McCpgcRankMap.Bits.rank0_mapping = 0;
  McCpgcRankMap.Bits.rank1_mapping = 1;
  McCpgcRankMap.Bits.rank2_mapping = 2;
  McCpgcRankMap.Bits.rank3_mapping = 3;
  McCpgcRankMap.Bits.rank4_mapping = 4;
  McCpgcRankMap.Bits.rank5_mapping = 5;
  McCpgcRankMap.Bits.rank6_mapping = 6;
  McCpgcRankMap.Bits.rank7_mapping = 7;
  MemWritePciCfgEp (Socket, Ch, MC_CPGC_RANK_MAP_MC_MAIN_REG , McCpgcRankMap.Data);

  McCpgcSubrankMap.Data = MemReadPciCfgEp (Socket, Ch, MC_CPGC_SUBRANK_MAP_MC_MAIN_REG);
  McCpgcSubrankMap.Bits.rank0_mapping = 0;
  McCpgcSubrankMap.Bits.rank1_mapping = 0;
  McCpgcSubrankMap.Bits.rank2_mapping = 0;
  McCpgcSubrankMap.Bits.rank3_mapping = 0;
  McCpgcSubrankMap.Bits.rank4_mapping = 0;
  McCpgcSubrankMap.Bits.rank5_mapping = 0;
  McCpgcSubrankMap.Bits.rank6_mapping = 0;
  McCpgcSubrankMap.Bits.rank7_mapping = 0;
  MemWritePciCfgEp (Socket, Ch, MC_CPGC_SUBRANK_MAP_MC_MAIN_REG , McCpgcSubrankMap.Data);

  CpgcMiscRefCtlReg.Data  = MemReadPciCfgEp (Socket, Ch, CPGC_MISCREFCTL_MCDDC_CTL_REG);
  CpgcMiscRefCtlReg.Bits.ref_rankmask    = 0;
  CpgcMiscRefCtlReg.Bits.panic_ref_only  = 0;
  MemWritePciCfgEp (Socket, Ch, CPGC_MISCREFCTL_MCDDC_CTL_REG, CpgcMiscRefCtlReg.Data);

  CpgcMiscZqCtlReg.Data   = MemReadPciCfgEp (Socket, Ch, CPGC_MISCZQCTL_MCDDC_CTL_REG);
  CpgcMiscZqCtlReg.Bits.zq_rankmask       = 0;
  CpgcMiscZqCtlReg.Bits.always_do_zq      = 0;
  MemWritePciCfgEp (Socket, Ch, CPGC_MISCZQCTL_MCDDC_CTL_REG, CpgcMiscZqCtlReg.Data);

  if (IsCpuAndRevision (CPU_SPRSP, REV_A0)) {
    SchedulerEnablesBsA0.Data   = MemReadPciCfgEp (Socket, Ch, SCHEDULER_ENABLES_BS_MCDDC_CTL_REG);
    SchedulerEnablesBsA0.Bits.llt_page_mode_en       = 0;
    SchedulerEnablesBsA0.Bits.link_major_mode_en     = 1;
    SchedulerEnablesBsA0.Bits.open_page_mode_en      = 1;
    SchedulerEnablesBsA0.Bits.idle_bypass_en         = 0;
    SchedulerEnablesBsA0.Bits.llt_page_mode_enhanced = 0;
    SchedulerEnablesBsA0.Bits.disable_buddy_mm       = 1;
    SchedulerEnablesBsA0.Bits.number_of_banks        = 2;
    SchedulerEnablesBsA0.Bits.high_rank_support      = 0;
    SchedulerEnablesBsA0.Bits.cpgc_inorder_mode_en   = 1;
    SchedulerEnablesBsA0.Bits.cpgc_second_cas_en     = 0;
    SchedulerEnablesBsA0.Bits.implicit_pre_en        = 0;
    SchedulerEnablesBsA0.Bits.page_table_index_selection = 0;
    SchedulerEnablesBsA0.Bits.bank_scheduler_selection   = 0;
    SchedulerEnablesBsA0.Bits.pseudo_channel_selection   = 0;
    MemWritePciCfgEp (Socket, Ch, SCHEDULER_ENABLES_BS_MCDDC_CTL_REG, SchedulerEnablesBsA0.Data);
  } else if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) {
    SchedulerEnablesBs.Data   = MemReadPciCfgEp (Socket, Ch, SCHEDULER_ENABLES_BS_MCDDC_CTL_REG);
    SchedulerEnablesBs.Bits.llt_page_mode_en       = 0;
    SchedulerEnablesBs.Bits.link_major_mode_en     = 1;
    SchedulerEnablesBs.Bits.open_page_mode_en      = 1;
    SchedulerEnablesBs.Bits.idle_bypass_en         = 0;
    SchedulerEnablesBs.Bits.llt_page_mode_enhanced = 0;
    SchedulerEnablesBs.Bits.disable_buddy_mm_col   = 1;
    SchedulerEnablesBs.Bits.disable_buddy_mm_row   = 1;
    SchedulerEnablesBs.Bits.number_of_banks        = 2;
    SchedulerEnablesBs.Bits.high_rank_support      = 0;
    SchedulerEnablesBs.Bits.cpgc_inorder_mode_en   = 1;
    SchedulerEnablesBs.Bits.cpgc_second_cas_en     = 0;
    SchedulerEnablesBs.Bits.implicit_pre_en        = 0;
    SchedulerEnablesBs.Bits.page_table_index_selection = 0;
    SchedulerEnablesBs.Bits.bank_scheduler_selection   = 0;
    SchedulerEnablesBs.Bits.pseudo_channel_selection   = 0;
    MemWritePciCfgEp (Socket, Ch, SCHEDULER_ENABLES_BS_MCDDC_CTL_REG, SchedulerEnablesBs.Data);
  }

  SchedulerEnables.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_ENABLES_MCDDC_CTL_REG);
  SchedulerEnables.Bits.idle_bypass_en      = 0;
  SchedulerEnables.Bits.bank_idle_bypass_en = 0;
  SchedulerEnables.Bits.xpq_llt_en          = 0;
  SchedulerEnables.Bits.rpq_vc1_llt_en      = 0;
  SchedulerEnables.Bits.deprd_after_write   = 0;
  SchedulerEnables.Bits.deprd_wpq_threshold = 1;
  SchedulerEnables.Bits.cpgc_inorder_mode_en = 1;
  SchedulerEnables.Bits.cpgc_second_cas_en  = 0;
  SchedulerEnables.Bits.page_table_index_selection = 0;
  SchedulerEnables.Bits.bank_scheduler_selection   = 0;
  SchedulerEnables.Bits.pseudo_channel_selection   = 0;
  MemWritePciCfgEp (Socket, Ch, SCHEDULER_ENABLES_MCDDC_CTL_REG, SchedulerEnables.Data);

  ScrambleConfig.Data = MemReadPciCfgEp (Socket, Ch, MCSCRAMBLECONFIG_MCDDC_DP_REG);
  ScrambleConfig.Bits.tx_enable = 0;
  ScrambleConfig.Bits.rx_enable = 0;
  ScrambleConfig.Bits.ch_enable = 0;
  ScrambleConfig.Bits.seed_lock = 0;
  ScrambleConfig.Bits.tx_enable_ddrt = 0;
  ScrambleConfig.Bits.rx_enable_ddrt = 0;
  ScrambleConfig.Bits.ch_enable_ddrt = 0;
  ScrambleConfig.Bits.scr_dis_rank = 0;
  MemWritePciCfgEp (Socket, Ch, MCSCRAMBLECONFIG_MCDDC_DP_REG, ScrambleConfig.Data);

  SchedulerRid2.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_RID2_MCDDC_CTL_REG);
  SchedulerRid2.Bits.enable_rid  = 0xff;
  MemWritePciCfgEp (Socket, Ch, SCHEDULER_RID2_MCDDC_CTL_REG, SchedulerRid2.Data);

  SchedulerRidStatus.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_RID_STATUS_MCDDC_CTL_REG );
  SchedulerRidStatus.Bits.rid_monitor_selection  = 0;
  MemWritePciCfgEp (Socket, Ch, SCHEDULER_RID_STATUS_MCDDC_CTL_REG , SchedulerRidStatus.Data);

  McMtr.Data = MemReadPciCfgEp (Socket, Ch, MCMTR_MC_MAIN_REG );
  McMtr.Bits.close_pg           = 0x0;
  McMtr.Bits.ecc_en             = 0x0;
  McMtr.Bits.dir_en             = 0x1;
  McMtr.Bits.read_dbi_en        = 0x0;
  McMtr.Bits.write_dbi_en       = 0x0;
  McMtr.Bits.read_parity_en     = 0x0;
  McMtr.Bits.write_parity_en    = 0x0;
  McMtr.Bits.normal             = 0x0;
  McMtr.Bits.bank_xor_enable    = 0x0;
  McMtr.Bits.trng_mode          = 0x1;
  McMtr.Bits.ddr5_9x4_en        = 0x0;
  McMtr.Bits.dimm_xor_enable    = 0x0;
  McMtr.Bits.chn_disable        = 0x0;
  McMtr.Bits.pch_xor_enable     = 0x0;
  McMtr.Bits.channel_xor_enable = 0x1;
  McMtr.Bits.cluster_xor_enable = 0x0;
  McMtr.Bits.ch23cmd_ctl_delay  = 0x0;
  McMtr.Bits.rank_xor_enable    = 0x0;
  McMtr.Bits.sid_xor_enable     = 0x0;
  McMtr.Bits.trng_target        = 0x0;
  McMtr.Bits.num_cluster        = 0x0;
  McMtr.Bits.hbm_mc_mode        = 0x0;
  McMtr.Bits.cpgc_active        = 0x0;
  MemWritePciCfgEp (Socket, Ch, MCMTR_MC_MAIN_REG , McMtr.Data);

  McMtr2.Data = MemReadPciCfgEp (Socket, Ch, MCMTR2_MC_MAIN_REG);
  McMtr2.Bits.monroe_chn_force_sr  = 0;
  McMtr2.Bits.adddc_exclusive2     = 1;
  MemWritePciCfgEp (Socket, Ch, MCMTR2_MC_MAIN_REG, McMtr2.Data);

#endif // GNRSRF_HOST
}

/**
  Set memory controller values for performance, to be performed around the switch to normal mode.

  @param[in]  Host    A pointer to the sys host structure
  @param[in]  Socket  The target socket

  @return N/A
**/
VOID
SwitchToNormalModeMcPerformance (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  // No specific settings for this program.
}

/**

  Mc Cmd Type Override Enable And Encoding Program

  @param[in]  Socket                   Current Socket (0-Based)
  @param[in]  Ch                       Current Ch (0-Based)
  @param[in]  CmdTypeOverrideEnable    Command Type Override Enable Value
  @param[in]  CmdTypeEncoding          Command Type Encoding Value

  @retval  NA

**/
VOID
EFIAPI
McCmdTypeOverrideEnableAndEncoding (
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 CmdTypeOverrideEnable,
  IN UINT8 CmdTypeEncoding
  )
{
  PSYSHOST                                                Host;

  Host = GetSysHostPointer ();

  SCHEDULER_CMD_DEBUG_MCDDC_CTL_STRUCT SchedulerCmdDebug;

  SchedulerCmdDebug.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_CMD_DEBUG_MCDDC_CTL_REG);
  RcDebugPrintWithDevice (
    SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "CmdTypeOverride = 0x%02x CmdTypeEncoding = 0x%02x\n",
    CmdTypeOverrideEnable,
    CmdTypeEncoding
    );
  SchedulerCmdDebug.Bits.cmd_type_on = CmdTypeOverrideEnable;
  SchedulerCmdDebug.Bits.cmd_type_oe = CmdTypeEncoding;
  MemWritePciCfgEp (Socket, Ch, SCHEDULER_CMD_DEBUG_MCDDC_CTL_REG, SchedulerCmdDebug.Data);
}

/**

  Chip specific code path to write Chip Wr Addr delays for DDR5

  @param[in] Host                - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket              - Socket Id
  @param[in] Ch                  - Channel number (0-based)
  @param[in] SubCh               - Sub Channel number (0-based)
  @param[in] SetType             - Set type. 0 = everything
  @param[in] OffsetValue         - Offset for byte lanes
  @param[in] CRAddDelay          - Add delays

  @retval None

**/
VOID
EFIAPI
WriteChipWrADDDelaysDdr5 (
  IN  PSYSHOST                Host,
  IN  UINT8                   Socket,
  IN  UINT8                   Ch,
  IN  UINT8                   Subch,
  IN  UINT8                   SetType,
  IN  INT16                   OffsetValue,
  IN  INT16                   CRAddDelay,
  IN  PCHIP_WL_CLEANUP_STRUCT WLChipCleanUpStruct
  )
{
  UINT8   ChannelLocal = Ch;
  GetChannelMapping (Ch, Subch, &ChannelLocal);
  SetCwlAdj (Host, Socket, ChannelLocal, SetType, CRAddDelay);
}

/**
  Initialize or cleanup MC settings for DRAM Duty Cycle Adjuster training

  @param[in] Socket             - Socket to initialize
  @param[in] ChannelMask        - One-hot encoding to mask a channel (0: Configure channel, 1: Skip channel)
  @param[in] DoInitNotRestore   - TRUE to perform initialization needed for DRAM DCA training
                                  FALSE to restore MC to original settings

  @retval MRC_FUNCTION_NOT_IMPLEMENTED if function not implemented
  @retval MRC_STATUS_SUCCESS otherwise

**/
MRC_STATUS
EFIAPI
DramDcaMemoryControllerConfiguration (
  IN    UINT8     Socket,
  IN    UINT32    ChannelMask,
  IN    BOOLEAN   DoInitNotRestore
  )
{
  PSYSHOST                                Host;
  UINT8                                   Channel;
  UINT8                                   MaxChDdr;
  SCHEDULER_CAS_TIMING_MCDDC_CTL_STRUCT   SchedCasTiming;

  Host      = GetSysHostPointer ();
  MaxChDdr  = GetMaxChDdr ();

  for (Channel = 0; Channel < MaxChDdr; Channel++) {

    if ((IsChannelEnabled (Socket, Channel) == FALSE) ||
        (((1 << Channel) & ChannelMask) != 0))
    {
      continue;
    }
    //
    // Prepare memory controller for DRAM DCA training
    //
    SchedCasTiming.Data = MemReadPciCfgEp (Socket, Channel, SCHEDULER_CAS_TIMING_MCDDC_CTL_REG);
    if (DoInitNotRestore) {
      //
      // Force all read commands to go out on the same MC CLK phase
      //
      SchedCasTiming.Bits.disable_rcas_lp = 1;
      SchedCasTiming.Bits.disable_rcas_hp = 0;
    } else {
      //
      // Restore to default
      //
      SchedCasTiming.Bits.disable_rcas_lp = 0;
      SchedCasTiming.Bits.disable_rcas_hp = 0;
    }
    MemWritePciCfgEp (Socket, Channel, SCHEDULER_CAS_TIMING_MCDDC_CTL_REG, SchedCasTiming.Data);

  } // Channel

  return MRC_STATUS_SUCCESS;
}

/**
  Override memory controller count based on the die count.
  The GetMaxImc() always returns MAX_IMC regardless the die count.

  @param[in]     Socket  The target socket
  @param[in out] MaxImc  MC count

  @return N/A
**/
VOID
EFIAPI
UpdateMaxImcByDieCount (
  IN     UINT8     Socket,
  IN OUT UINT8     *MaxImc
  )
{
  CAPID3_PCU_FUN3_STRUCT  CapId3;
  UINT8                   MaxImcSocket;
  PSYSHOST                Host;

  Host = GetSysHostPointer ();

  //
  // capid3 can be overridden in the RcSim .ini file. This allows us to configure
  // the die count to match that of the tested processor in headless MRC.
  // If capid3 is not overridden by user, the default returned MaxImc is the MAX_IMC.
  //
  CapId3.Data = Host->var.common.procCom[Socket].capid3;
  MaxImcSocket = MAX_IMC;

  //
  // Find the highest order enabled MC to determine DdrMaxImc.
  //
  if (CapId3.Bits.capid3_26 == 1) {
    MaxImcSocket = 4;
  } else if (CapId3.Bits.capid3_25 == 1) {
    MaxImcSocket = 3;
  } else if (CapId3.Bits.capid3_24 == 1) {
    MaxImcSocket = 2;
  } else if (CapId3.Bits.capid3_23 == 1) {
    MaxImcSocket = 1;
  }

  *MaxImc = MaxImcSocket;
}

/**

  Set Open page mode in the MC

  @param Host           - Point to sysHost
  @param Socket         - Socket number
  @param Mcid           - Memory controller
  @param PageMode       - Closed or Open page mode

  @retval n/a

**/
VOID
SetPageModeMc (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Mcid,
  IN UINT8     PageMode
  )
{
  MCMTR_MC_MAIN_STRUCT                      McMtr;
  SCHEDULER_ENABLES_BS_MCDDC_CTL_STRUCT     SchedulerEnablesBs;

  UINT8                                  Ch;
  UINT8                                  NumChPerMc;
  NumChPerMc = GetNumChannelPerMc ();

  for (Ch = (NumChPerMc * Mcid); Ch < (NumChPerMc * (Mcid + 1)); Ch++){
    //
    // Clear to set open page mode
    //
    McMtr.Data = MemReadPciCfgEp (Socket, Ch, MCMTR_MC_MAIN_REG);
    McMtr.Bits.close_pg = PageMode;
    MemWritePciCfgEp (Socket, Ch, MCMTR_MC_MAIN_REG, McMtr.Data);

    SchedulerEnablesBs.Data   = MemReadPciCfgEp (Socket, Ch, SCHEDULER_ENABLES_BS_MCDDC_CTL_REG);
    SchedulerEnablesBs.Bits.open_page_mode_en = !PageMode;
    MemWritePciCfgEp (Socket, Ch, SCHEDULER_ENABLES_BS_MCDDC_CTL_REG, SchedulerEnablesBs.Data);
  } //Ch loop
}

/**

  This routine gets/sets CSRs required by BiosSetPageOpenConfig

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param Controller                 - Controller number (0-based)
  @param Enable                     - Specifies whether page-open is enabled.  TRUE enables page-open; FALSE disables it
  @param PrevEnable                 - Pointer to where the previous page-open enable setting will be stored.  This value may be NULL

  @retval N/A

**/
VOID
BiosSetPageOpenConfigChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Controller,
  BOOLEAN               Enable,
  BOOLEAN               *PrevEnable
  )
{
  MCMTR_MC_MAIN_STRUCT                          McmtrReg;
  SCHEDULER_ENABLES_BS_MCDDC_CTL_STRUCT         SchedulerEnablesBs;
  UINT8                                         Channel;
  UINT8                                         NumChPerMc;
  NumChPerMc = GetNumChannelPerMc ();

  for (Channel = Controller * NumChPerMc; Channel < ((Controller + 1) * NumChPerMc); Channel++ ) {
    McmtrReg.Data             = MemReadPciCfgEp (Socket, Channel, MCMTR_MC_MAIN_REG);
    SchedulerEnablesBs.Data   = MemReadPciCfgEp (Socket, Channel, SCHEDULER_ENABLES_BS_MCDDC_CTL_REG);
    //
    // Set to Open Page Mode
    //
    if (PrevEnable != NULL) {  //Get Case
      if (McmtrReg.Bits.close_pg == CLOSED_PAGE_EN) {
        *PrevEnable = FALSE;
      } else {
        *PrevEnable = TRUE;
      }
    }

    if (Enable) {
      McmtrReg.Bits.close_pg = CLOSED_PAGE_DIS;
      SchedulerEnablesBs.Bits.open_page_mode_en = OPEN_PAGE_EN;
    } else {
      McmtrReg.Bits.close_pg = CLOSED_PAGE_EN;  //Enabling Closed Page Mode
      SchedulerEnablesBs.Bits.open_page_mode_en = OPEN_PAGE_DIS;
    }
    MemWritePciCfgEp (Socket, Channel, MCMTR_MC_MAIN_REG, McmtrReg.Data);
    MemWritePciCfgEp (Socket, Channel, SCHEDULER_ENABLES_BS_MCDDC_CTL_REG, SchedulerEnablesBs.Data);
  }
}

/**

  Cache and Clear RDB entries for LRDIMM and DDRT

  @param[in]  Socket                   Current Socket (0-Based)

  @retval  NA

**/
VOID
EFIAPI
CacheAndClearRidEntries (
  IN  UINT8    Socket
  )
{
  // Null
}


/**

  Restore RDB entries for LRDIMM and DDRT

  @param[in]  Socket                   Current Socket (0-Based)

  @retval  NA

**/
VOID
EFIAPI
RestoreRidEntries (
  IN  UINT8    Socket
  )
{
  // Null
}

/**
  Get Swizzled Strobe

  @param[in]  Host      - Pointer to SysHost
  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number
  @param[in]  Dimm      - DIMM number
  @param[in]  Strobe    - Strobe number inside sub channel a (0 ~ 9)

  @retval Swizzled Strobe
**/
UINT8
CheckBacksideSwizzle (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch,
  UINT8    Dimm,
  UINT8    Strobe
  )
{
  UINT8            MSVx8;  // Max Strobe Valid x8
  struct dimmNvram (*DimmNvList)[MAX_DIMM];

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  if ((*DimmNvList)[Dimm].DcpmmPresent) {

    //
    // DDRT2 is Per Sub Channel Strobe
    //
    // Backside Swizzle Mapping
    // sub channel b use the same swizzle mapping as sub channel a
    //                 |            sub channel a          |
    //                 |            |ecc |            |ecc |
    // Strobe          | 0  1  2  3 | 4  | 5  6  7  8 | 9  |
    // Swizzled Strobe | 5  6  7  8 | 9  | 0  1  2  3 | 4  |
    //
    MSVx8 = GetSubChMaxStrobeValid (Host) / 2;

    if ((*DimmNvList)[Dimm].lrbufswizzle & (1 << (Strobe % MSVx8))) {

      if (Strobe >= MSVx8) {
        return Strobe - MSVx8;
      } else {
        return Strobe + MSVx8;
      }
    } else {
      return Strobe;
    }

  } else {
    return Strobe;
  }
}
