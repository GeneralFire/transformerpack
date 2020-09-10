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
#include <SysHostChip.h>
#include <Memory/CpgcDefinitions.h>
#include "Chip10nm/Include/MemHostChip10nm.h"
#include <UncoreCommonIncludes.h>
#include "Include/MemMcRegs.h"
#include "Include/MemMcChip.h"
#include <Library/SystemInfoLib.h>
#include <CmiPerformance.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/Mesh2MemIpLib.h>
#include "Include/MemPerformanceValueTables.h"
#include <Library/CteNetLib.h>
#include <Library/MemMcIpTargetLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/SpdAccessLib.h>
#include <Library/CsrAccessLib.h>
#include "Include/CmiMcRegStruct.h"
#include <Library/SysHostPointerLib.h>
#include <Cpu/CpuIds.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/MemSmeeIpLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include "Include/MemMcIpLibInternal.h"
#include <Library/ProcMemInitTargetLib.h>
#include <Library/MemRcLib.h>
#include <Library/UsraCsrLib.h>
#include <CpuPciAccess.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PmemMiscLib.h>
#include "Include/MemMcCmiCredits.h"
#include <Library/IoAccessLib.h>
#include <Library/MemAccessLib.h>
#include <Library/HbmCoreLib.h>
#include <Library/SpdDecodeLib.h>
#include <Library/MemFmcIpLib.h>
#include <PlatformInfoTypes.h>

//
// NVMDIMM size
//
const UINT16 AepSizeIndex[] = {
  CAPACITYSPD64GB,
  CAPACITYSPD128GB,
  CAPACITYSPD256GB,
  CAPACITYSPD512GB,
  CAPACITYSPD1TB
};

const SPD_AEP_MOD_REVISION_STRUCT MinSpdAepModRevisions[] = {
  //  .Bits.module_type,   .Bits.module_revision
  { { SPD_AEP_MOD_REV_AEP, 2                     } },
  { { SPD_AEP_MOD_REV_APK, 1                     } },
  { { SPD_AEP_MOD_REV_BPK, 1                     } },
  { { SPD_AEP_MOD_REV_BPS, 0                     } },
  { { SPD_AEP_MOD_REV_PPK, 0                     } },
  { { SPD_AEP_MOD_REV_IPK, 0                     } }
};
const UINT8 MinSpdAepModRevisionsSize = sizeof MinSpdAepModRevisions / sizeof MinSpdAepModRevisions[0];
CONST UINT32 DimmMtrRegOffsets[] = { DIMMMTR_0_MC_MAIN_REG, DIMMMTR_1_MC_MAIN_REG };

/**

  Get the DIMM_MTR register offset for a particular DIMM.

  This additionally provides bounds checking for the DIMM value, comparing it against the number of members in the
  DimmMtrRegOffset array.

  @param[in]  Dimm  The target DIMM number

  @retval The DIMM_MTR register offset for the target DIMM

**/
UINT32
GetDimmMtrRegOffset(
  IN UINT8 Dimm
  )
{
  if (Dimm >= ((sizeof DimmMtrRegOffsets) / (sizeof DimmMtrRegOffsets[0]))) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_168);
  }
  return DimmMtrRegOffsets[Dimm];
}

/**

  Get CMI Setup program choice.

  @retval   CMI_AUTO_OPTION - Program values from Excel sheet
            CMI_REGISTER_DEFAULTS Do not program any cmi credit, use register defaults.

**/
UINT8
GetCmiSetupProgramValue (
  VOID
)
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  if (Setup->mem.CmiInitOption == CMI_INIT_RESET) {
    RcDebugPrint (SDBG_MINMAX,
                "Cmi Option reset default Selected\n");
    return CMI_REGISTER_DEFAULTS;
  } else {
    RcDebugPrint (SDBG_MINMAX,
                "Cmi Option Auto Selected\n");
    return CMI_AUTO_OPTION;
  }
}

/**

  Using boot Dimm configuration, find which option of cmi values to program

  @param[in]  Host                       - Pointer to the system Host (root) structure
  @param[in]  Socket                     - Socket Id
  @param[out] ProgramableValue[MAX_CH]   -  Array containing programmable value type for each Channel (From 0 to Max Channel in Socket)
                                            CMI_DDRT_VAL      = 1 - Value for DDRT memory
                                            CMI_1LM_VAL       = 2 - Value for 1LM DDR4
                                            CMI_CR_VAL        = 3 - Value for Crystalridge
                                            CMI_1LM_ISOCH_VAL = 4 - Value for 1LM with ISOCH enabled
                                            CMI_1LM_2WAY_VAL  = 5 - Value for DDR + 2WAY
                                            CMI_CR_2WAY_VAL   = 6 - Value for mixed DDR + DDRT + 2WAY

  @retval N/A

**/
VOID
GetCmiProgramValue (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  OUT UINT8 ProgramableValue[MAX_CH]
)
{
  UINT32 M2MemMode = 0;
  UINT8 Ch = 0;
  UINT8 McId = 0;
  UINT8 ChId = 0;
  UINT8 Dimm = 0;
  UINT8 NumChPerMc;
  UINT8 MaxImc;
  struct      channelNvram (*ChannelNvList)[MAX_CH];
  struct      dimmNvram (*DimmNvList)[MAX_DIMM];
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  ASSERT_EFI_ERROR (ProgramableValue != NULL);
  ChannelNvList = GetChannelNvList (Host, Socket);
  NumChPerMc = GetNumChannelPerMc ();
  MaxImc = GetMaxImc ();

  for (McId = 0; McId < MaxImc; McId++) {
    M2MemMode = GetM2mModeValue (Socket, McId);
    for (Ch = 0; Ch < NumChPerMc; Ch++) {

      ChId = (McId * NumChPerMc) + Ch;
      ProgramableValue[ChId] = CMI_1LM_VAL;  //default 1LM

      if ((M2MemMode & ISOCHRONOUS_FLOW_ENABLED_BIT) != 0) {
        ProgramableValue[ChId] = CMI_1LM_ISOCH_VAL;  //1LM Iso
      } //((M2MemMode & BIT4) != 0)

      if ((*ChannelNvList)[ChId].ddrtEnabled == 1) {
        ProgramableValue[ChId] = CMI_DDRT_VAL;
        DimmNvList = GetDimmNvList (Host, Socket, ChId);
        for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
          if ((*DimmNvList)[Dimm].dimmPresent == 0) {
            continue;
          }
          if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
            //Atleast 1 non-DDRT present, then use CR column value
            ProgramableValue[ChId] = CMI_CR_VAL;
            break;
          } //if ((*DimmNvList)[Dimm].DcpmmPresent == 0)
        } // Dimm Loop
      } // if (*ChannelNvList)[ChId].ddrtEnabled = 1)

      if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
        if (IsTwoWayNmCacheEnabled ()) {
          switch (ProgramableValue[ChId]) {
            case CMI_1LM_VAL:
              ProgramableValue[ChId] =  CMI_1LM_2WAY_VAL;
              break;
            case CMI_CR_VAL:
              ProgramableValue[ChId] =  CMI_CR_2WAY_VAL;
              break;
            default:
              break;
          }
        }
      }
    } // Channel Loop
  } // Mc Loop
}

/**
  Set Long Preamble for write and read in MC

  @param[in] Host               - Point to sysHost
  @param[in] Socket             - Socket number
  @param[in] Ch                 - Channel number
  @param[in] WritePreambleState - PREAMBLE_2TCLK is long write preamble, else 1TCLK preamble is programmed
  @param[in] ReadPreambleState  - PREAMBLE_2TCLK is long read preamble, else 1TCLK preamble is programmed

  @retval N/A
**/
VOID
SetLongPreambleMc (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    WritePreambleState,
  IN      UINT8    ReadPreambleState
  )
{
  MEMORY_TIMINGS_ADJ_MCDDC_CTL_STRUCT         MtAdj;
  MEMORY_TIMINGS_ADJ_MCDDC_CTL_STRUCT         MtAdjCache;
  struct channelNvram                         (*channelNvList)[MAX_CH];
  struct dimmNvram                            (*dimmNvList)[MAX_DIMM];
  UINT8                                       Dimm;
#if (MC_IP_VER < MEM_IP_19ww19e)
  T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_STRUCT     ddrtDimm0BasicTiming;
#else
  T_DDRT_DIMM1_BASIC_TIMING_MC_2LM_STRUCT     ddrtDimm1BasicTiming;
#endif //(MC_IP_VER < MEM_IP_19ww19e)
  channelNvList = GetChannelNvList (Host, Socket);
  MtAdj.Data = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG);
  if ((Host->nvram.mem.dramType == SPD_TYPE_DDR5) && !IsBrsPresent (Host, Socket)) {
    //
    // Set the write_preamble and read_preamble as 0 for SPR DDR5
    //
    MtAdj.Bits.write_preamble = 0;
    MtAdj.Bits.read_preamble = 0;
  } else {
    MtAdj.Bits.write_preamble = (WritePreambleState == PREAMBLE_2TCLK ? 1 : 0);
    MtAdj.Bits.read_preamble = (ReadPreambleState == PREAMBLE_2TCLK ? 1 : 0);
  }
  MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG, MtAdj.Data);

  MtAdjCache.Data = (*channelNvList)[Ch].MemoryTimingsAdj;
  MtAdjCache.Bits.write_preamble = MtAdj.Bits.write_preamble;
  MtAdjCache.Bits.read_preamble = MtAdj.Bits.read_preamble;
  (*channelNvList)[Ch].MemoryTimingsAdj = MtAdjCache.Data;

  // also set preamble_2tck for DDRT
  dimmNvList = GetDimmNvList (Host, Socket, Ch);
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*dimmNvList)[Dimm].DcpmmPresent == 1) {
#if (MC_IP_VER < MEM_IP_19ww19e)
      ddrtDimm0BasicTiming.Data = MemReadPciCfgEp (Socket, Ch, T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_REG);
      ddrtDimm0BasicTiming.Bits.preamble_2tck_wr = (WritePreambleState == PREAMBLE_2TCLK ? 1 : 0);
      ddrtDimm0BasicTiming.Bits.preamble_2tck_rd = (ReadPreambleState == PREAMBLE_2TCLK ? 1 : 0);
      MemWritePciCfgEp (Socket, Ch, T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_REG, ddrtDimm0BasicTiming.Data);
#else
      ddrtDimm1BasicTiming.Data = MemReadPciCfgEp (Socket, Ch, T_DDRT_DIMM1_BASIC_TIMING_MC_2LM_REG);
      ddrtDimm1BasicTiming.Bits.tck2_block_wr = (WritePreambleState == PREAMBLE_2TCLK ? 1 : 0);
      ddrtDimm1BasicTiming.Bits.tck2_block_rd = (ReadPreambleState == PREAMBLE_2TCLK ? 1 : 0);
      MemWritePciCfgEp (Socket, Ch, T_DDRT_DIMM1_BASIC_TIMING_MC_2LM_REG, ddrtDimm1BasicTiming.Data);
#endif //(MC_IP_VER < MEM_IP_19ww19e)
    }
  }
} // SetLongPreamble

/**
  Set/Restore Long Read Preamble

  @param[in] Host         - Point to sysHost
  @param[in] Socket       - Socket number
  @param[in] Ch           - Channel number
  @param[in] Set          - if TRUE: Set Long Read Preamble
                            else:    Restore original Read Preamble state

  @retval N/A
**/
VOID
SetLongReadPreamble (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      BOOLEAN  Set
  )
{
  UINT8   WritePreamble;
  UINT8   ReadPreamble;

  //
  // No need to modify DDRIO for Read Preamble - only the MC is modified
  // This function only changes the Read Preamble but the current Write Preamble state is needed when calling SetLongPreambleMc().
  //
  WritePreamble = GetPreambleState (Socket, WRITE_PREAMBLE);

  if (Set) {
    //
    // Set Read Preamble to 2TCLK.
    //
    ReadPreamble = PREAMBLE_2TCLK;
  } else {
    //
    // Find out what the Read Preamble is supposed to be and restore that
    //
    ReadPreamble = GetPreambleState (Socket, READ_PREAMBLE);
  }

  SetLongPreambleMc (Host, Socket, Ch, WritePreamble, ReadPreamble);
}

/**

  Enable DCLK in the MC

  @param MemTechType    - Mem technology type
  @param Socket         - Socket number

  @retval n/a

**/
VOID
SetDclkEnableMc (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          Socket
  )
{
  MC_INIT_STATE_G_MC_MAIN_STRUCT              McInitStateG;
  UINT8                                       Ch;
  UINT8                                       MaxCh;

  MaxCh = GetMaxChPerTechType (MemTechType);

  for (Ch = 0; Ch < MaxCh; Ch++) {
    if (!IsChEnabledPerTechType (MemTechType, Socket, Ch)) {
      continue;
    }
    McInitStateG.Data = ChRegisterRead (MemTechType, Socket, Ch, MC_INIT_STATE_G_MC_MAIN_REG);
    McInitStateG.Bits.dclk_enable = 1;
    ChRegisterWrite (MemTechType, Socket, Ch, MC_INIT_STATE_G_MC_MAIN_REG, McInitStateG.Data);
  }
}


/**

  Disable Rcomp in the MC

  @param Host           - Point to sysHost
  @param Socket         - Socket number
  @param Mcid           - Memory controller

  @retval n/a

**/
VOID
SetRcompDisableMc (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Mcid
)
{
  RCOMP_GLOBALCTRL1_MC_GLOBAL_STRUCT          RcompGlobalCtrl;

  //comp
  RcompGlobalCtrl.Data = MemReadPciCfgMC (Socket, Mcid, RCOMP_GLOBALCTRL1_MC_GLOBAL_REG);
  RcompGlobalCtrl.Bits.dis_rcomp = 1;
  MemWritePciCfgMC (Socket, Mcid, RCOMP_GLOBALCTRL1_MC_GLOBAL_REG, RcompGlobalCtrl.Data);
}

/**

  Clear DDRT MC Req counter in the MC

  @param Host           - Point to sysHost
  @param Socket         - Socket number
  @param Ch             - Channel number

  @retval n/a

**/
VOID
ClearDdrtReqCounterMc (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm
)
{
  DDRT_DEFEATURE_MC_2LM_STRUCT              ddrtDefeature;

  // 2. Tell MC to clear Req Counter
  // Program after 32 read commands
  ddrtDefeature.Data = MemReadPciCfgEp (Socket, Ch, DDRT_DEFEATURE_MC_2LM_REG);
  ddrtDefeature.Bits.ignore_ddrt_req = (1 << Dimm) & 0x3;
  MemWritePciCfgEp (Socket, Ch, DDRT_DEFEATURE_MC_2LM_REG, ddrtDefeature.Data);

  //
  // Delay only if this is a CTE build, otherwise do nothing.
  //

  CteDelayQclk (30);

  ddrtDefeature.Bits.ignore_ddrt_req = 0;
  MemWritePciCfgEp (Socket, Ch, DDRT_DEFEATURE_MC_2LM_REG, ddrtDefeature.Data);
}

/**

  Set timer for Idle Cmd to CKE low

  @param Host           - Point to sysHost
  @param Socket         - Socket number
  @param Ch             - Channel number

  @retval n/a

**/
VOID
SetIdleCmdToCkeLowMc (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
)
{
  SREF_STATIC2_MCDDC_CTL_STRUCT               srefStatic2;

  // program idle_cmd_to_cke_drops_timer value provided by design
  srefStatic2.Data = MemReadPciCfgEp (Socket, Ch, SREF_STATIC2_MCDDC_CTL_REG);
  srefStatic2.Bits.idle_cmd_to_cke_drops_timer = IDLE_CMD_CKE_DROPS_TIMER;
  MemWritePciCfgEp (Socket, Ch, SREF_STATIC2_MCDDC_CTL_REG, srefStatic2.Data);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "idle_cmd_to_cke_drops_timer programmed to: 0x%x\n", srefStatic2.Bits.idle_cmd_to_cke_drops_timer);
}

/**
  Check LPMODE entry latency value to search for constraints and fix them if present

  @param [in] Host   - Point to sysHost
  @param [in] Socket - Socket number
  @param [in] Ch     - Channel number

  @retval n/a
**/
VOID
CheckLpmodeEntryLatencyConstraints (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
  )
{
#if (MC_IP_VER < MEM_IP_19ww19e)
  LPMODE_PM_CTRL_MCDDC_CTL_STRUCT    LpmodePmCtrl;
  UINT32                             LpmodePmCtrlReg = LPMODE_PM_CTRL_MCDDC_CTL_REG;
#else
  LPMODE_PM_CTRL_N0_MCDDC_CTL_STRUCT LpmodePmCtrl;
  UINT32                             LpmodePmCtrlReg = LPMODE_PM_CTRL_N1_MCDDC_CTL_REG;
#endif

  if (IsSiliconWorkaroundEnabled ("S1909161672")) {
    LpmodePmCtrl.Data = MemReadPciCfgEp (Socket, Ch, LpmodePmCtrlReg);

    if (LpmodePmCtrl.Bits.lpmode_entry_latency == LPMODE_ENTRY_LATENCY_LARGE_SNR) {
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_149);
      //
      // Having lpmode_entry_latency LPMODE_ENTRY_LATENCY_LARGE_SNR will make the the silicon bug occur
      // and corrupt memory when t_cke has the low two bits set to 3
      //
      LpmodePmCtrl.Bits.lpmode_entry_latency = LPMODE_ENTRY_LATENCY_SAFE_SNR;
      MemWritePciCfgEp (Socket, Ch, LpmodePmCtrlReg, LpmodePmCtrl.Data);
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "lpmode_entry_latency changed from %d to 0%d\n", LPMODE_ENTRY_LATENCY_LARGE_SNR, LPMODE_ENTRY_LATENCY_SAFE_SNR);
    }
  }
}

/**
  Check DDRT CKE setting value to search for constraints and fix them if present

  @param [in] Host   - Point to sysHost
  @param [in] Socket - Socket number
  @param [in] Ch     - Channel number

  @retval n/a
**/
VOID
CheckTDdrtCkeConstraints (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
  )
{
#if (MC_IP_VER < MEM_IP_19ww19e)
  LPMODE_PM_CTRL_MCDDC_CTL_STRUCT    LpmodePmCtrl;
  UINT32                             LpmodePmCtrlReg = LPMODE_PM_CTRL_MCDDC_CTL_REG;
#else
  LPMODE_PM_CTRL_N0_MCDDC_CTL_STRUCT LpmodePmCtrl;
  UINT32                             LpmodePmCtrlReg = LPMODE_PM_CTRL_N1_MCDDC_CTL_REG;
#endif
  SREF_STATIC2_MCDDC_CTL_STRUCT   SrefStatic2;

  if (IsSiliconWorkaroundEnabled ("S1909161672")) {
    SrefStatic2.Data = MemReadPciCfgEp (Socket, Ch, SREF_STATIC2_MCDDC_CTL_REG);
    LpmodePmCtrl.Data = MemReadPciCfgEp (Socket, Ch, LpmodePmCtrlReg);

    if (SrefStatic2.Bits.t_ddrt_cke == (LpmodePmCtrl.Bits.lpmode_entry_latency - 1)) {
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_150);
      //
      // Internal t_ddrt_cke if set to one less than lpmode_entry_latency will cause silicon bug which will corrupt memory
      //
      SrefStatic2.Bits.t_ddrt_cke = LpmodePmCtrl.Bits.lpmode_entry_latency;
      MemWritePciCfgEp (Socket, Ch, SREF_STATIC2_MCDDC_CTL_REG, SrefStatic2.Data);
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "t_ddrt_cke changed from %d to 0%d\n", LpmodePmCtrl.Bits.lpmode_entry_latency - 1, SrefStatic2.Bits.t_ddrt_cke);
    }
  }
}

/**

  This resets the MC IO module

  @param Host    - Pointer to sysHost
  @param socket    - Socket number

  @retval N/A

**/
VOID
IO_Reset (
  PSYSHOST Host,
  UINT8    socket
  )
{
  MC_INIT_STATE_G_MC_MAIN_STRUCT mcInitStateG[MAX_IMC];
  UINT8 i;
  UINT8 MaxImc;

  MaxImc = GetMaxImc ();

#ifdef MRC_TRACE
  RcDebugPrintWithDevice (SDBG_TRACE, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "IO_Reset Starts\n");
#endif //MRC_TRACE

  for (i = 0; i < MaxImc; i++) {
    if (Host->var.mem.socket[socket].imcEnabled[i]) {
      mcInitStateG[i].Data = MemReadPciCfgMC (socket, i, MC_INIT_STATE_G_MC_MAIN_REG);
    }
  }

  //
  // Reset DDR IO
  //

  //
  // Delay if this is a CTE build, otherwise do nothing.
  //

  CteDelayDclk (5);

  if(Host->DdrioUltSupport) {
    CteSetOptions ("io_reset", 0x3f, 0x3, 0x0, 1);
  }

  for (i = 0; i < MaxImc; i++) {
    if (Host->var.mem.socket[socket].imcEnabled[i]) {
      mcInitStateG[i].Bits.reset_io = 1;
      MemWritePciCfgMC_AllCh (socket, i, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG[i].Data);
    }
  }

  IoResetDelayTarget (Host);

  //
  // Clear DDR IO Reset
  //

  if(Host->DdrioUltSupport) {
    CteSetOptions ("io_reset", 0x3f, 0x3, 0x0, 0);
  }

  for (i = 0; i < MaxImc; i++) {
    if (Host->var.mem.socket[socket].imcEnabled[i]) {
      mcInitStateG[i].Bits.reset_io = 0;
      MemWritePciCfgMC_AllCh (socket, i, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG[i].Data);
    }
  }

  //
  // Wait again 20 QCLK for stable IO.
  //
  FixedQClkDelay (Host, 20);

} // IO_Reset

/**

  Issue a comp cycle

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/
VOID
DoComp (
  PSYSHOST Host,
  UINT8    socket
  )
{
  EFI_STATUS                         Status;
  UINT8                              McId;
  RCOMP_GLOBALCTRL1_MC_GLOBAL_STRUCT RcompGlobalCtrl;
  RCOMP_GLOBALCTRL1_MC_GLOBAL_STRUCT RcompGlobalCtrlOriginal;
  UINT32                             RunComp;
  UINT8                              MaxImc;

  MaxImc = GetMaxImc ();

  //
  // Always run this, except CTE builds should check to see if
  // it should be run.
  //

  RunComp = 1;

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {

    Status = CteGetOptions ("runcomp", &RunComp);
    if (EFI_ERROR (Status)) {
      RunComp = 1;
    }
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "RUNCOMP=%d\n", RunComp);

  }

  if (RunComp) {

#ifdef MRC_TRACE
    RcDebugPrintWithDevice (SDBG_TRACE, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "DoComp Starts\n");
#endif //MRC_TRACE

    for (McId = 0; McId < MaxImc; McId++) {
      if (Host->var.mem.socket[socket].imcEnabled[McId] == 0) {
        continue;
      }

      // Read RCOMP_GLOBALCTRL1
      RcompGlobalCtrl.Data = MemReadPciCfgMC (socket, McId, RCOMP_GLOBALCTRL1_MC_GLOBAL_REG);
      RcompGlobalCtrlOriginal.Data = RcompGlobalCtrl.Data;

      // Enable RCOMP
      if (RcompGlobalCtrl.Bits.dis_rcomp == 1) {
        RcompGlobalCtrl.Bits.dis_rcomp = 0;
        MemWritePciCfgMC (socket, McId, RCOMP_GLOBALCTRL1_MC_GLOBAL_REG, RcompGlobalCtrl.Data);
      }

      // Start RCOMP (after it has been explicitly enabled)
      RcompGlobalCtrl.Bits.rcomp = 1;
      MemWritePciCfgMC (socket, McId, RCOMP_GLOBALCTRL1_MC_GLOBAL_REG, RcompGlobalCtrl.Data);

      // Wait for the RCOMP cycle to complete
      WaitForCompCycleTarget (Host, socket, McId);

      // Restore the RCOMP enable/disable settings and clear the RCOMP start bit
      // The original dis_rcomp value is used rather than explicitly setting it to 1 primarily due to the DoComp
      // call(s) that occur(s) before RCOMP is globally disabled (by calling SetRcompDisableMc).
      RcompGlobalCtrl.Bits.dis_rcomp = RcompGlobalCtrlOriginal.Bits.dis_rcomp;
      RcompGlobalCtrl.Bits.rcomp = 0;
      MemWritePciCfgMC (socket, McId, RCOMP_GLOBALCTRL1_MC_GLOBAL_REG, RcompGlobalCtrl.Data);

    } // McId loop
  }
} // DoComp


/**

  This function applies a new PXC setup values(enables or disables the PXC training).

  @param[in]  Host           - Pointer to sysHost
  @param[in]  Socket         - Socket
  @param[in]  Ch             - Channel
  @param[in]  PxcEnDisable   - Enable/disable PXC

  @retval none

**/
VOID
PxcSetup (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     PxcEnDisable
  )
{
  MCWDB_CHKN_BIT_MCDDC_DP_STRUCT        McwdbChknBitReg;

  PxcSetupDdrio (Host, Socket, Ch, 0);

  //
  // Set legacy bit dis_ddrio_earlywdata regardless of PXC enable. In all
  // 10nm, t_cwl_adj_neg is used instead.
  //
  McwdbChknBitReg.Data = MemReadPciCfgEp (Socket, Ch, MCWDB_CHKN_BIT_MCDDC_DP_REG);
  McwdbChknBitReg.Bits.dis_ddrio_earlywdata = 1;
  MemWritePciCfgEp (Socket, Ch, MCWDB_CHKN_BIT_MCDDC_DP_REG, McwdbChknBitReg.Data);
} //PxcSetup

/**
  This function returns the first DDR channel found in the configuration

  @param[in    ] Socket - Socket number
  @param[   out] Ch     - Pointer to store the first DDR channel

  @retval TRUE if DDR channel was found
**/
BOOLEAN
FindFirstChannel (
  IN     UINT8 Socket,
     OUT UINT8 *Ch
  )
{
  PSYSHOST            Host;
  UINT8               Channels;
  struct channelNvram (*ChannelNvList)[MAX_CH];

  Host = GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Channels = 0; Channels < MAX_CH; Channels++) {
    if ((*ChannelNvList)[Channels].enabled) {
      *Ch = Channels;
      return TRUE;
    }
  }

  return FALSE;
}

/**
  This function returns the first DIMM found in the specified memory channel

  @param[in    ] Socket - Socket number
  @param[in    ] Ch     - Channel number
  @param[   out] Dimm   - Pointer to store the number of the first DIMM

  @retval TRUE if DIMM was found
**/
BOOLEAN
FindFirstDimmInChannel (
  IN     UINT8 Socket,
  IN     UINT8 Ch,
     OUT UINT8 *Dimm
  )
{
  PSYSHOST         Host;
  UINT8            Dimms;
  struct dimmNvram (*DimmNvList)[MAX_DIMM];

  Host = GetSysHostPointer ();
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  for (Dimms = 0; Dimms < MAX_DIMM; Dimms++) {
    if ((*DimmNvList)[Dimms].dimmPresent) {
      *Dimm = Dimms;
      return TRUE;
    }
  }

  return FALSE;
}

/**
  This function returns true if a previous DIMM than the one
  passed as parameter is present

  @param[in    ] Socket       - Socket number
  @param[in    ] Ch           - Channel number
  @param[in    ] Dimm         - Dimm number
  @param[   out] PreviousDimm - Pointer to store the previous Dimm available

  @retval TRUE if previous DIMM is present
**/
BOOLEAN
IsPreviousDimmPresent (
  IN     UINT8 Socket,
  IN     UINT8 Ch,
  IN     UINT8 Dimm,
     OUT UINT8 *PreviousDimm
)
{
  INT8             Dimms;
  PSYSHOST         Host;
  struct dimmNvram (*DimmNvList)[MAX_DIMM];

  Host = GetSysHostPointer ();
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  if (Dimm > 0) {
    for (Dimms = (Dimm - 1); Dimms >= 0; Dimms--) {
      if ((*DimmNvList)[Dimm - 1].dimmPresent) {
        *PreviousDimm = Dimms;
        return TRUE;
      }
    }
  }

  return FALSE;
}

/**
  This function returns if the memory channel specified is out
  of order or not based on its DIMM population ordering and the
  memory channel topology supported

  @param[in    ] Socket - Socket number
  @param[in    ] Ch     - Channel number

  @retval TRUE if the DIMM configuration is out of order, FALSE
          if not
**/
BOOLEAN
AreDimmSlotsOutOfOrder (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
  EFI_MEMORY_TOPOLOGY_TYPE MemoryTopology;
  BOOLEAN                  ReturnValue = FALSE;
  PSYSHOST                 Host;
  struct dimmNvram         (*DimmNvList)[MAX_DIMM];
  struct channelNvram      (*ChannelNvList)[MAX_CH];

  Host = GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  MemoryTopology = GetMemoryTopologyConfiguration (Host, Socket, Ch);

  //
  // Return FALSE if the memory channel is empty
  //
  if ((*ChannelNvList)[Ch].enabled == 0) {
    return FALSE;
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "Memory Topology: ");
  switch (MemoryTopology) {
    case DaisyChainTopology:
      RcDebugPrint (SDBG_MAX,
                              "DaisyChainTopology\n");
      //
      // First memory slot must be populated
      //
      if ((*DimmNvList)[0].dimmPresent == 0) {
        ReturnValue = TRUE;
      }
      break;

    case InvSlotsDaisyChainTopology:
      RcDebugPrint (SDBG_MAX,
                              "InvSlotsDaisyChainTopology\n");
      //
      // Last memory slot must be populated
      //
      if ((*DimmNvList)[MAX_DIMM - 1].dimmPresent == 0) {
        ReturnValue = TRUE;
      }
      break;

    case TTopology:
      RcDebugPrint (SDBG_MAX,
                              "TTopology\n");
      //
      // No ordering rules
      //
      ReturnValue = FALSE;
      break;
  }

  return ReturnValue;
}

/**
  Update the IMC number based on the CAP ID value

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number

  @retVal         SUCCESS
**/
EFI_STATUS
EFIAPI
UpdateImcNumber (
  IN      PSYSHOST        Host,
  IN      UINT8           Socket
  )
{
  CAPID3_PCU_FUN3_STRUCT  CapId3;
  UINT8                   MaxImcSocket;
  UINT8                   McId;
  CPU_CSR_ACCESS_VAR      *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  CapId3.Data = UsraCsrRead (Socket, 0, CAPID3_PCU_FUN3_REG);
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

  //
  // Do not exceed MAX_IMC.
  //
  if (MaxImcSocket > MAX_IMC) {
    MaxImcSocket = MAX_IMC;
  }

  //
  // Update the system-wide DdrMaxImc for socket 0, or if we need to increase it.
  //
  if (Socket == 0 || MaxImcSocket > CpuCsrAccessVar->DdrMaxImc) {
    CpuCsrAccessVar->DdrMaxImc = MaxImcSocket;
    CpuCsrAccessVar->DdrMaxCh = MaxImcSocket * CpuCsrAccessVar->DdrNumChPerMc;
  }

  //
  // Disable MCs based on what was read.
  //
  for (McId = 0; McId < MAX_IMC; McId++) {
    if ((McId >= MaxImcSocket) ||
        (McId == 0 && CapId3.Bits.capid3_23 == 0) ||
        (McId == 1 && CapId3.Bits.capid3_24 == 0) ||
        (McId == 2 && CapId3.Bits.capid3_25 == 0) ||
        (McId == 3 && CapId3.Bits.capid3_26 == 0)) {
      Host->var.mem.socket[Socket].ImcPresent[McId] = FALSE;
      Host->var.mem.socket[Socket].imcEnabled[McId] = 0;
    }
  }

  return EFI_SUCCESS;
}

/**

  Verifies current population does not validate POR restrictions

  @param Host  - Pointer to sysHost

  @retval N/A

**/
UINT32
CheckPORCompat (
  PSYSHOST Host
  )
{
  BOOLEAN                                 IsFirstDimm;
  UINT8                                   socket;
  UINT8                                   mcId;
  UINT8                                   ch;
  UINT8                                   dimm;
  UINT8                                   PreviousDimm = 0xFF;
  INT32                                   minTCKcommon;
  UINT8                                   chMemPresent = 0;
  BOOLEAN                                 SystemMemPresent;
  UINT8                                   mapOutCh;
  UINT8                                   DimmTypeStatus = 0;
  UINT8                                   DimmType;
  BOOLEAN                                 DimmSlotsOutOfOrder;
  UINT8                                   maxDimm;
  UINT8                                   ti;
  UINT8                                   PackageRanks;
  UINT8                                   numRanks;
  UINT8                                   ECCDimmSupport = 0;
  UINT8                                   NvmCapNotSupport;
#ifdef DDRT_SUPPORT
  UINT8                                   ci;
  UINT16                                  NvmCapacity;
  UINT8                                   i;
#endif // DDRT_SUPPORT
  struct dimmDevice                       (*chdimmList)[MAX_DIMM];
  struct dimmNvram                        (*dimmNvList)[MAX_DIMM];
  struct channelNvram                     (*channelNvList)[MAX_CH];
  SMB_DEVICE_STRUCT                       spd;
  CHAR8                                   StringBuffer[MRC_STRING_LENGTH];
  CHAR8                                   SecondStringBuffer[MRC_STRING_LENGTH];
  DIMMMTR_0_MC_MAIN_STRUCT                dimmMtr;
  CAPID3_PCU_FUN3_STRUCT                  capId3;
  CAPID1_PCU_FUN3_STRUCT                  capId1;
  BootMode                                SysBootMode;
  EFI_STATUS                              DimmSmbAddressStatus;
  SYS_SETUP                               *Setup;
  UINT8                                   DdrPresent;
  UINT8                                   DdrtPresent[MAX_IMC];
  UINT8                                   DdrInSystem = 0;
  UINT8                                   NumChPerMc;
  UINT8                                   MaxChDdr;
  UINT8                                   MaxImc;

  MaxChDdr = GetMaxChDdr ();
  NumChPerMc = GetNumChannelPerMc ();
  Setup = GetSysSetupPointer ();
  MaxImc = GetMaxImc ();

  // Indicate Dimm Detection is done
  SpdDimmDetectDone ();

  SysBootMode = GetSysBootMode ();

  if (!IsMemFlowEnabled (CheckPorCompatibility)) {
    return SUCCESS;
  }

  SystemMemPresent = FALSE;
  DimmType   = 0;

  //
  // Initialize common parts of the smbDevice structure for all SPD devices
  //
  spd.compId = SPD;
  spd.address.controller = PLATFORM_SMBUS_CONTROLLER_PROCESSOR;
  spd.address.deviceType = DTI_EEPROM;

  if ((SysBootMode == NormalBoot) && (Host->var.mem.subBootMode == ColdBoot) ) {
    //
    // Initialize ECC support to enabled
    //
    Host->nvram.mem.eccEn = 1;
    //
    // Detect DIMMs on each Socket
    //
    for (socket = 0; socket < MAX_SOCKET; socket++) {
      //
      // Loop for each CPU socket
      //
      if (Host->nvram.mem.socket[socket].enabled == 0) {
        continue;
      }
      if (IsHvmModeEnable ()) {
        capId3.Data = 0;
        capId1.Data = 0;
      } else {
        capId3.Data = UsraCsrRead (socket, 0, CAPID3_PCU_FUN3_REG);
        capId1.Data = UsraCsrRead (socket, PCU_INSTANCE_0, CAPID1_PCU_FUN3_REG);
      }
      //
      // Disable ECC if option requires it
      //
      if (Setup->mem.options & DISABLE_ECC_SUPPORT) {
        Host->nvram.mem.eccEn = 0;
      }

      //
      // Initial DcpmmPresent for system level on SBSP Host
      //
      if (Host->nvram.mem.socket[socket].DcpmmPresent == 1) {
        Host->nvram.mem.DcpmmPresent = 1;
      }

      //
      // Initialize to 0
      //
      Host->nvram.mem.socket[socket].maxDimmPop = 0;

      //
      // Save the original value
      //

      //
      // Initialize to 0
      //
      minTCKcommon  = 0;

      channelNvList = GetChannelNvList(Host, socket);

      DdrPresent = 0;

      ZeroMem (DdrtPresent, sizeof (DdrtPresent));

      //
      // Initialize to 0 for all channels
      //
      chMemPresent = 0;

      //
      // Initialize flag used to map out the channels
      //
      mapOutCh = 0;

      //
      // Detect DIMMs on each channel
      //
      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }

        (*channelNvList)[ch].lrRankMultEnabled = 0;
        (*channelNvList)[ch].encodedCSMode = 0;
        (*channelNvList)[ch].cidBitMap = 0;

        //
        // Initialize the maximum number of DIMMs on this channel to 0
        //
        maxDimm = (*channelNvList)[ch].maxDimm;

        chdimmList  = &Host->var.mem.socket[socket].channelList[ch].dimmList;

        //
        // Detect DIMMs in each slot
        //
        DimmTypeStatus = 0;
        NvmCapNotSupport = 1;
        dimmNvList = GetDimmNvList(Host, socket, ch);
        IsFirstDimm = TRUE;

        //
        // Check if the memory population is valid
        //
        DimmSlotsOutOfOrder = AreDimmSlotsOutOfOrder (socket, ch);

        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                "Memory connector: %d\n", GetMemoryConnectorType (Host, socket, ch));

        for (dimm = 0; dimm < Host->var.mem.socket[socket].channelList[ch].numDimmSlots; dimm++) {
          //
          // Clear data for each dimm register
          //
          dimmMtr.Data    = 0;

          DimmSmbAddressStatus = GetSmbAddress (socket, ch, dimm, &spd);
          if (EFI_ERROR (DimmSmbAddressStatus)) {
            RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "Error retrieving DIMM SMBus address. Treating slot as empty.\n");
          }


          //
          // If DIMM slot is empty, continue
          //
          if (((*dimmNvList)[dimm].dimmPresent == 0) || (EFI_ERROR (DimmSmbAddressStatus))) {
            continue;
          }

#ifdef DDRT_SUPPORT
          if ((*dimmNvList)[dimm].DcpmmPresent == 1) {
            // If AEP (EKV) DIMM is present disable slot and issue warning
            if ((*dimmNvList)[dimm].SubsystemDeviceID == EKV) {
              RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                           "PMem DIMM not supported\n");
              OutputWarning (WARN_DIMM_COMPAT, WARN_DDRT_DIMM_NOT_SUPPORTED, socket, ch, dimm, NO_RANK);
              DisableDIMM (Host, socket, ch, dimm);
              continue;
            }

            DdrtPresent[ch / NumChPerMc] |= BIT0 << (ch % NumChPerMc);
          }
#endif // DDRT_SUPPORT
          if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
            DdrPresent |= BIT0 << ch;
          }

          //
          // Log a warning and continue if a DIMM is populated out of order
          //
          if (DimmSlotsOutOfOrder) {
            RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                           "DIMM out of order\nSMBUS Instance - %d, DIMM Address - 0x%x\n",
                           spd.address.SmbInstance, spd.address.strapAddress);
            OutputWarning (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_MINOR_OUT_OF_ORDER, socket, ch, dimm, NO_RANK);
            DisableDIMM (Host, socket, ch, dimm);
            continue;
          }

#ifdef DDRT_SUPPORT
          if (!IsFirstDimm) {
            if ((*dimmNvList)[dimm].DcpmmPresent && (*dimmNvList)[dimm-1].DcpmmPresent) {
              OutputWarning (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_2_AEP_FOUND_ON_SAME_CH, socket, ch, dimm, NO_RANK);
              mapOutCh |= (BIT0 << ch);
              continue;
            }

            if ((*dimmNvList)[0].DcpmmPresent == 1) {
              RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                             "NON POR Config: PMem Dimm in slot 0\n");
              OutputWarning (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_NVMDIMM_OUT_OF_ORDER, socket, ch, NO_DIMM, NO_RANK);

              mapOutCh |= (BIT0 << ch);
              continue;
            }
          }
#endif // DDRT_SUPPORT

          // Change to addressing to 3DS support
          if (((*dimmNvList)[dimm].SPDDeviceType & SPD_SIGNAL_LOADING) == SPD_3DS_TYPE) {
            (*channelNvList)[ch].encodedCSMode = 2;
          }


          //
          // Get number of ranks
          //
          (*dimmNvList)[dimm].numRanks = ((*dimmNvList)[dimm].SPDModuleOrg >> 3) & 7;
          (*dimmNvList)[dimm].numRanks++;
          (*dimmNvList)[dimm].numDramRanks = (*dimmNvList)[dimm].numRanks;

          // Determine DDR4 LRDIMM CS/ADDR mapping here
          if (IsLrdimmPresent (socket, ch, dimm) || ((*dimmNvList)[dimm].SPDDeviceType & SPD_SIGNAL_LOADING) == SPD_3DS_TYPE) {
            //
            // Reuse byte for Rank Multiplication factor as rank aliasing in DDR4
            // if any dimm on this channel is in encoded mode, then all dimms must be in encoded mode
            (*dimmNvList)[dimm].lrRankMult = 1;
            numRanks = ((*dimmNvList)[dimm].actSPDModuleOrg >> 3) & 7;

            // Update logical ranks and DRAM row bits / density
            (*dimmNvList)[dimm].SPDModuleOrg = ((*dimmNvList)[dimm].actSPDModuleOrg & ~(BIT4 + BIT3)) | (numRanks << 3);
          }

          //
          // Comparison of DIMM's present on each slot to see if the DimmRank[slot] > DimmRank[slot-1]
          // Map out channel if Non POR Config detected (Ranks not placed in descending order in DIMM slots)
          //
          if (!IsFirstDimm){
            if (DimmType != SPD_TYPE_AEP) {
              if (IsPreviousDimmPresent (socket, ch, dimm, &PreviousDimm)) {
                if (((*dimmNvList)[dimm].SPDModuleOrg >> 3) > ((*dimmNvList)[PreviousDimm].SPDModuleOrg >> 3)) {
                  RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "DIMM config Not POR Config Slot%d SPD Num Rank= %d Slot%d SPD Num Rank= %d",
                    PreviousDimm, ((*dimmNvList)[PreviousDimm].SPDModuleOrg >> 3),
                    dimm, ((*dimmNvList)[dimm].SPDModuleOrg >> 3));
                  OutputWarning (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_MINOR_OUT_OF_ORDER, socket, ch, dimm, NO_RANK);
                  mapOutCh |= (BIT0 << ch);
                  continue;
                }
              }
            }
          }

          //
          // Lookup geometry in DIMM technology table
          //
          ti = GetTechIndex (socket, ch, dimm);

          //
          // Check if a valid tech index was found
          //
          if (ti == 0xFF) {
            RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                           "DIMM not supported!\n");
            OutputWarning (WARN_DIMM_COMPAT, WARN_DIMM_COMPAT_MINOR_NOT_SUPPORTED, socket, ch, dimm, NO_RANK);
            mapOutCh |= (BIT0 << ch);
            continue;
          }

          //
          // 1, 2 or 4 Ranks
          //
          if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
            if (!((((*dimmNvList)[dimm].SPDModuleOrg >> 3) == SPD_NUM_RANKS_1) ||
                  (((*dimmNvList)[dimm].SPDModuleOrg >> 3) == SPD_NUM_RANKS_2) ||
                  (((*dimmNvList)[dimm].SPDModuleOrg >> 3) == SPD_NUM_RANKS_4))) {
              RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                             "Number of ranks on device not supported!\n");
              OutputWarning (WARN_DIMM_COMPAT, WARN_DIMM_COMPAT_MINOR_MAX_RANKS, socket, ch, dimm, NO_RANK);
              mapOutCh |= (BIT0 << ch);
              continue;
            }
          }
          //
          // Check if an RDIMM is plugged in to a UDIMM only board or connected to a cpu that does not support it
          //
          if (((*dimmNvList)[dimm].keyByte2 == SPD_RDIMM) &&
              ((Setup->mem.dimmTypeSupport == UDIMM) )
             ) {
            RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "%a in UDIMM only board!\n",
                           GetDimmTypeStr((*dimmNvList)[dimm].keyByte, (*dimmNvList)[dimm].actKeyByte2, (CHAR8 *)&StringBuffer));
            OutputWarning (WARN_RDIMM_ON_UDIMM, WARN_MINOR_CONFIG_NOT_SUPPORTED, socket, ch, dimm, NO_RANK);
            mapOutCh |= (BIT0 << ch);
            continue;
          }

          //
          // Check if an UDIMM is plugged in to a RDIMM only board or connected to a cpu that does not support it
          //
          if (((*dimmNvList)[dimm].keyByte2 == SPD_UDIMM) &&
              ((Setup->mem.dimmTypeSupport == RDIMM) || capId3.Bits.disable_udimm)
             ) {
            RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                           "UDIMM in RDIMM only board!\n");
            OutputWarning (WARN_UDIMM_ON_RDIMM, WARN_MINOR_CONFIG_NOT_SUPPORTED, socket, ch, dimm, NO_RANK);
            mapOutCh |= (BIT0 << ch);
            continue;
          }

          //
          // Check if an SODIMM is plugged in to a RDIMM only board or connected to a cpu that does not support it
          //
          if (((*dimmNvList)[dimm].keyByte2 == SPD_SODIMM) &&
              ((Setup->mem.dimmTypeSupport == RDIMM) || capId3.Bits.disable_udimm)
             ) {
            RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                           "SODIMM in RDIMM only board!\n");
            OutputWarning (WARN_SODIMM_ON_RDIMM, WARN_MINOR_CONFIG_NOT_SUPPORTED, socket, ch, dimm, NO_RANK);
            mapOutCh |= (BIT0 << ch);
            continue;
          }

          //
          // Check if DIMM is supported for specific CPU SKU
          //

          // 3ds - SKU
          if (((*dimmNvList)[dimm].SPDDeviceType & SPD_SIGNAL_LOADING) == SPD_3DS_TYPE) {
            if (capId1.Bits.capid1_19 == 1) {
              RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "3DS not supported with this SKU!\n");
              OutputWarning (WARN_DIMM_COMPAT, WARN_DIMM_3DS_NOT_SUPPORTED, socket, ch, dimm, NO_RANK);
              mapOutCh |= (BIT0 << ch);
              continue;
           }
          }

          //
          // Check if DIMM Capacity is supported for specific CPU SKU
          //
          if (CheckIsDramSupportByCpuSku (Host, socket, ch, dimm) != TRUE) {
            mapOutCh |= (BIT0 << ch);
            continue;
          }

          // Dimm disable - SKU
          if ((capId1.Bits.capid1_20 == 1) && ((*channelNvList)[ch].maxDimm == 2) && (dimm == 1)) {
            RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "2 Dimms per channel not supported with this SKU!\n");
            OutputWarning (WARN_DIMM_COMPAT, WARN_DIMM_SKU_NOT_SUPPORTED, socket, ch, dimm, NO_RANK);
            mapOutCh |= (BIT0 << ch);
            continue;
          }

#ifdef DDRT_SUPPORT
          // NVMDIMM - SKU
          if ((*dimmNvList)[dimm].DcpmmPresent) {
            if (capId1.Bits.disable_ddrt) {
              RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "PMem not supported with this SKU!\n");
              OutputWarning (WARN_DIMM_COMPAT, WARN_DIMM_NVMDIMM_NOT_SUPPORTED, socket, ch, dimm, NO_RANK);
              mapOutCh |= (BIT0 << ch);
              continue;
            }
          }

          // NVMDIMM - Enforcement Policy
          if (((*dimmNvList)[dimm].fmcType != FMC_CWV_TYPE) && ((*dimmNvList)[dimm].FmcModRev.Bits.module_type < SPD_AEP_MOD_REV_BPS) && ((*dimmNvList)[dimm].DcpmmPresent == 1) && (!((GetEmulation() & SIMICS_FLAG) == SIMICS_FLAG))) {
            //
            // Check SXP Stepping Rev by SPD byte 237
            //
            if (((*dimmNvList)[dimm].NvmStepping & 0x3f) < 0x10) {
              RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "PMem stepping 0x%x is not supported!\n", (*dimmNvList)[dimm].NvmStepping);
              OutputWarning (WARN_DIMM_COMPAT, WARN_DIMM_NVMDIMM_NOT_SUPPORTED, socket, ch, dimm, NO_RANK);
              mapOutCh |= (BIT0 << ch);
              continue;
            }

            //
            // Check Fab Rev by SPD byte 130
            //
            for (i = 0; i < MinSpdAepModRevisionsSize; ++i) {
              if ((*dimmNvList)[dimm].FmcModRev.Bits.module_type == MinSpdAepModRevisions[i].Bits.module_type) {
                if ((*dimmNvList)[dimm].FmcModRev.Bits.module_revision < MinSpdAepModRevisions[i].Bits.module_revision) {
                  // We've found the target module type, but the revision is too low.
                  mapOutCh |= (BIT0 << ch);
                  break;
                } else {
                  // We've found the target module type and the revision is okay.
                  break;
                }
              } else if (i >= MinSpdAepModRevisionsSize - 1) {
                // We haven't found the target module type, despite being at the last index (or above--for paranoia's sake), so the type is invalid.
                mapOutCh |= (BIT0 << ch);
                break;
              }
            }
            if (mapOutCh & (BIT0 << ch)) {
              RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "PMem Fab Type 0x%x with Rev 0x%x is not supported!\n", (*dimmNvList)[dimm].FmcModRev.Bits.module_type, (*dimmNvList)[dimm].FmcModRev.Bits.module_revision);
              OutputWarning (WARN_DIMM_COMPAT, WARN_DIMM_NVMDIMM_NOT_SUPPORTED, socket, ch, dimm, NO_RANK);
              continue;
            }

            //
            // Check capacity by SPD byte 176 and 178
            // 176[3:0] + 178[6:2]
            // - 0x100: 64GB
            // - 0x120: 128GB
            // - 0x124: 256GB
            // - 0x12C: 512GB
            //
            NvmCapacity = (UINT16)(((*dimmNvList)[dimm].NvmDevDensity & 0xf) << 5) + (((*dimmNvList)[dimm].NvmDevType & 0x7c) >> 2);
            for(ci = 0; ci < ARRAY_SIZE (AepSizeIndex); ci++){
              if (NvmCapacity == AepSizeIndex[ci]) {
                NvmCapNotSupport = 0;
                break;
              }
            }

            if (NvmCapNotSupport == 1) {
              RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "PMem Capacity 0x%x is not supported!\n", NvmCapacity);
              OutputWarning (WARN_DIMM_COMPAT, WARN_DIMM_NVMDIMM_NOT_SUPPORTED, socket, ch, dimm, NO_RANK);
              mapOutCh |= (BIT0 << ch);
              continue;
            }
          } else if ((*dimmNvList)[dimm].fmcType == FMC_CWV_TYPE) {

            NvmCapacity = (UINT16) (((*dimmNvList)[dimm].NvmDevDensity & 0xf) << 5) + (((*dimmNvList)[dimm].NvmDevType & 0x7c) >> 2);

            if (DdrtPorCheck (((*dimmNvList)[dimm].NvmStepping & 0x3f), (*dimmNvList)[dimm].FmcModRev, NvmCapacity) == FALSE) {
              RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "PMem Stepping 0x%x, FmcModRev 0x%x, Capacity 0x%x is not supported!\n",
                                      (*dimmNvList)[dimm].NvmStepping, (*dimmNvList)[dimm].FmcModRev, NvmCapacity);
              OutputWarning (WARN_DIMM_COMPAT, WARN_DIMM_NVMDIMM_NOT_SUPPORTED, socket, ch, dimm, NO_RANK);
              mapOutCh |= (BIT0 << ch);
              continue;
            }
          }
#endif // DDRT_SUPPORT

          //
          // Make sure RDIMMs and UDIMMs are not mixed. They are not electrically compatible
          // DimmTypeStatus is initialized to zero and will get set to the DIMM type of the DIMM
          // detected on the first pass. On subsequent passes keyByte2 must equal DimmTypeStatus or
          // else there is a mismatch and the system should halt to prevent electrical damage.
          // SODIMMs are not expected to be mixed with either RDIMM or UDIMM.
          //
          if (DimmTypeStatus != 0) {
            if (((*dimmNvList)[0].keyByte == SPD_TYPE_AEP) || ((*dimmNvList)[dimm].keyByte == SPD_TYPE_AEP)) {
              if (((*dimmNvList)[0].keyByte2 != SPD_RDIMM) || ((*dimmNvList)[dimm].keyByte2 != SPD_RDIMM)) {
                //
                // Rdimm and Lrdimm mixing allowed, checking dimm 1
                //
                RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                "PMem is mixed with UDIMM!\n");
                FatalError (ERR_DIMM_COMPAT, ERR_MIXED_MEM_AEP_AND_UDIMM);
                break;
              }
            } else if (DimmTypeStatus != (*dimmNvList)[dimm].actKeyByte2) {
              RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "%a is mixed with %a!\n",
                             GetDimmTypeStr((*dimmNvList)[dimm].keyByte, (*dimmNvList)[dimm].actKeyByte2, (CHAR8 *)&StringBuffer),
                             GetDimmTypeStr((*dimmNvList)[dimm].keyByte, DimmTypeStatus, (CHAR8 *)&SecondStringBuffer));
              FatalError (ERR_DIMM_COMPAT, ERR_MIXED_MEM_TYPE);
              break;
              //
              // Break incase the user does not want to halt
              //
            }
          }

          //
          // Disable ECC for the entire system if one DIMM is found that does not support it
          // TODO: Support multiple Bus Width Extension
          //

          if (IsFirstDimm) {
            ECCDimmSupport = ((*dimmNvList)[dimm].SPDMemBusWidth & SPD_BUS_WIDTH_EXT);
            if (ECCDimmSupport == 0) {
              Host->nvram.mem.eccEn = 0;
            }
          } else {
            if (IsPreviousDimmPresent(socket, ch, dimm, &PreviousDimm)) {
              ECCDimmSupport =((*dimmNvList)[PreviousDimm].SPDMemBusWidth & SPD_BUS_WIDTH_EXT);
            }
          }

          if (((*dimmNvList)[dimm].SPDMemBusWidth & SPD_BUS_WIDTH_EXT) != ECCDimmSupport) {

            OutputWarning (WARN_DIMM_COMPAT,  WARN_CHANNEL_MIX_ECC_NONECC, socket, ch, dimm, NO_RANK);

            if (!(Setup->mem.options & ECC_MIX_EN)) {
              Host->nvram.mem.eccEn = 0;
            } else {
              Host->nvram.mem.eccEn = 1;
              DisableChannelSw (Host, socket, ch);
            }
          }

          //
          // Set DIMM Type status to the first type of DIMM we find. If a different type is detect later
          // a fatal error will be logged.  Mixed DIMM types are not supported.
          // DimmTypeStatus = SPD_X definition = keyByte2 (1,2 & 3 = RDIMM, UDIMM & SODIMM)
          // dimmTypePresent is zero based so = keyByte2 -1
          // Set DRAM type to first type found
          //
          if (DimmTypeStatus == 0) {
            Host->nvram.mem.dimmTypePresent = ((*dimmNvList)[dimm].keyByte2 - 1);
            DimmTypeStatus                  = (*dimmNvList)[dimm].actKeyByte2;
            DimmType                        = (*dimmNvList)[dimm].keyByte;
          }

          if (PcdGetBool (PcdMrcQrDimmSupport) == TRUE) {

            //
            // Check for quad rank DIMM
            //

            if ((*dimmNvList)[dimm].numRanks == 4) {

              //
              // Update number of QR DIMMs
              //
              (*channelNvList)[ch].numQuadRanks++;
            }

          }

          // Check if number of ranks on this channel has been exceeded
          //
          if (((*channelNvList)[ch].numRanks + (*dimmNvList)[dimm].numRanks) > MAX_RANK_CH) {
            RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "Max ranks per ch exceeded\n");
            OutputWarning (WARN_DIMM_COMPAT, WARN_RANK_NUM, socket, ch, NO_DIMM, NO_RANK);

            //
            // Indicate this channel needs to be mapped out after detecting all populated DIMMs
            //
            mapOutCh |= (BIT0 << ch);
            continue;
          } else {
            //
            // Update ranks on this channel
            //
            (*channelNvList)[ch].numRanks = (*channelNvList)[ch].numRanks + (*dimmNvList)[dimm].numRanks;

            //
            // Check if this is a x4 DIMM
            //
            if ((*dimmNvList)[dimm].dramIOWidth == SPD_DRAM_IO_WIDTH_X4) {
              if (((*channelNvList)[ch].features & X8_PRESENT)) {
                //
                // Set flag to enable workaround
                //
                (*channelNvList)[ch].features |= X4_AND_X8_PRESENT;
              }
              (*dimmNvList)[dimm].x4Present = 1;
              (*channelNvList)[ch].features |= X4_PRESENT;
            }
            //
            // Check if this is a x8 DIMM
            //
            if ((*dimmNvList)[dimm].dramIOWidth == SPD_DRAM_IO_WIDTH_X8) {
              if (((*channelNvList)[ch].features & X4_PRESENT)) {
                //
                // Set flag to enable workaround
                //
                (*channelNvList)[ch].features |= X4_AND_X8_PRESENT;
              }
              (*channelNvList)[ch].features |= X8_PRESENT;
            }

            // Check if this is a x16 DIMM
            if ((*dimmNvList)[dimm].dramIOWidth == SPD_DRAM_IO_WIDTH_X16) {
              (*channelNvList)[ch].features |= X16_PRESENT;
              Host->nvram.mem.socket[socket].x16DimmPresent = TRUE;
              if (Setup->mem.PdaModeX16 == SAFE_PDA_MODE_X16) {
                Setup->mem.optionsExt &= ~PDA_EN;
              }
            }

            //
            // Save DIMM technology
            //
            (*dimmNvList)[dimm].techIndex = ti;

            //
            // Update the fastest common tCK
            //
            if (minTCKcommon < (*chdimmList)[dimm].minTCK) {
              minTCKcommon = (*chdimmList)[dimm].minTCK;
            }

            //
            // Indicate there is memory present on this channel
            //
            chMemPresent |= (BIT0 << ch);

            //
            // Check that the DIMM supports the expected voltage (1.2V for DDR4 or 1.1V for DDR5)
            // If not supported, map out the channel so voltage level does not get set.
            //
            if ((*dimmNvList)[dimm].keyByte == SPD_TYPE_DDR4) {
              if (Host->nvram.mem.socket[socket].channelList[ch].v120NotSupported) {
                RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                "DDR4 DIMM does not support 1.2V!\n");
                OutputWarning (WARN_DIMM_COMPAT, WARN_DIMM_VOLTAGE_NOT_SUPPORTED, socket, ch, dimm, NO_RANK);
                mapOutCh |= (BIT0 << ch);
              }
            } else if ((*dimmNvList)[dimm].keyByte == SPD_TYPE_DDR5) {
              if (Host->nvram.mem.socket[socket].channelList[ch].v110NotSupported) {
                RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                "DDR5 DIMM does not support 1.1V!\n");
                OutputWarning (WARN_DIMM_COMPAT, WARN_DIMM_VOLTAGE_NOT_SUPPORTED, socket, ch, dimm, NO_RANK);
                mapOutCh |= (BIT0 << ch);
              }
            }

            if (((*dimmNvList)[dimm].keyByte == SPD_TYPE_DDR4) || ((*dimmNvList)[dimm].keyByte == SPD_TYPE_DDR5)) {
              //
              // Indicate this DIMM is populated
              //
              dimmMtr.Bits.dimm_pop = 1;
            }

            //
            // Set rank count
            //
            PackageRanks = ((*dimmNvList)[dimm].SPDModuleOrg >> 3) & 0x7;

            //
            // Get Num of Ranks
            //
            if (PackageRanks == SPD_NUM_RANKS_4) {
              PackageRanks = 2; // Convert the SPD encoding to the register encoding
            }

            dimmMtr.Bits.rank_cnt = PackageRanks;

            //
            // 3DS and non-3DS lrdimms cannot be mixed.  If a one type was previously detected
            // on the current channel and the current dimm is not the same the system should halt.
            //
            if (!IsFirstDimm) {
              if (IsPreviousDimmPresent(socket, ch, dimm, &PreviousDimm)) {
                if (((((*dimmNvList)[dimm].SPDDeviceType & SPD_SIGNAL_LOADING) == SPD_3DS_TYPE) && (((*dimmNvList)[PreviousDimm].SPDDeviceType & SPD_SIGNAL_LOADING) != SPD_3DS_TYPE)) ||
                    ((((*dimmNvList)[PreviousDimm].SPDDeviceType & SPD_SIGNAL_LOADING) == SPD_3DS_TYPE) && (((*dimmNvList)[dimm].SPDDeviceType & SPD_SIGNAL_LOADING) != SPD_3DS_TYPE))) {
                  if (((*dimmNvList)[dimm].keyByte != SPD_TYPE_AEP) && ((*dimmNvList)[PreviousDimm].keyByte != SPD_TYPE_AEP)) {
                    RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "Invalid mix of DIMMs on channel!\n");
                    FatalError (ERR_DIMM_COMPAT, ERR_MISMATCH_DIMM_TYPE);
                    mapOutCh |= (BIT0 << ch);
                    break;
                    //
                    // Map out the channel and break in case the user does not want to halt
                    //
                  }
                }
              }
            }

            //
            // Check if we are in direct or encoded mode
            //
            if (((*dimmNvList)[dimm].SPDDeviceType & SPD_SIGNAL_LOADING) == SPD_3DS_TYPE) {
              switch ((*dimmNvList)[dimm].dieCount) {
                case 2:
                  dimmMtr.Bits.ddr4_3dsnumranks_cs = 1; //2**1 = 2 ranks
                  (*channelNvList)[ch].cidBitMap |= BIT0;
                  (*dimmNvList)[dimm].cidBitMap |= BIT0;
                  break;

                case 3:
                case 4:
                  dimmMtr.Bits.ddr4_3dsnumranks_cs = 2; //2**2 = 4 ranks
                  (*channelNvList)[ch].cidBitMap |= (BIT0 + BIT1);
                  (*dimmNvList)[dimm].cidBitMap |= (BIT0 + BIT1);
                  break;

                case 5:
                case 6:
                case 7:
                case 8:
                  dimmMtr.Bits.ddr4_3dsnumranks_cs = 3; //2**3 = 8 ranks
                  (*channelNvList)[ch].cidBitMap |= (BIT0 + BIT1 + BIT2);
                  (*dimmNvList)[dimm].cidBitMap |= (BIT0 + BIT1 + BIT2);
                  break;

                default:
                  dimmMtr.Bits.ddr4_3dsnumranks_cs = 0; // no CIDs
                  break;
              }
            } else {
              dimmMtr.Bits.ddr4_3dsnumranks_cs = 0;
            }

            //
            // Set DIMM width (not used by the hardware)
            //
            dimmMtr.Bits.ddr3_width = (*dimmNvList)[dimm].SPDModuleOrg & 3;
            //
            // Set DIMM BA Shrink, remove BG1 when a x16 device is present
            //
            if ((*dimmNvList)[dimm].dramIOWidth == SPD_DRAM_IO_WIDTH_X16) {
              dimmMtr.Bits.ba_shrink  = 1;
            } else {
              dimmMtr.Bits.ba_shrink  = 0;
            }
            //
            // Set DRAM density (not used by the hardware)
            //
            dimmMtr.Bits.ddr3_dnsty =  (*dimmNvList)[dimm].sdramCapacity - 2;

            //
            // Set Row Address width
            //
            dimmMtr.Bits.ra_width = (*dimmNvList)[dimm].numRowBits - 12;

            //
            // Set Column Address width
            //
            dimmMtr.Bits.ca_width = (*dimmNvList)[dimm].numColBits - 10;

            //
            // Save for later
            //
            (*dimmNvList)[dimm].dimmMemTech = dimmMtr.Data;
          }

          RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "%a population\n", GetDimmTypeStr((*dimmNvList)[dimm].keyByte, (*dimmNvList)[dimm].actKeyByte2, (CHAR8 *)&StringBuffer));

          //Clear the first DIMM flag when the end of the code in the loop has been finished once
          IsFirstDimm = FALSE;
        } // dimm loop

        //
        // Check if memory widths mixing is supported, if not, map-out the channel
        //
        if (!IsMixingWidthsSupported (socket, ch)) {
          mapOutCh |= (BIT0 << ch);
        }

        //
        // Map out this channel if an invalid DIMM or configuration was detected
        //
        if (mapOutCh & (BIT0 << ch)) {
          //
          // Disable this channel
          //
          (*channelNvList)[ch].enabled = 0;

          //
          // Indicate no memory is available on this channel
          //
          chMemPresent &= ~(BIT0 << ch);

          //
          // Mapping out DIMM 0 maps out all other DIMMs on this channel
          // MapOutDIMM(Host, socket, ch, 0, ch);
          //
          maxDimm = 0;

          DdrtPresent[ch / NumChPerMc] &= ~(BIT0 << (ch % NumChPerMc));
          DdrPresent &= ~(BIT0 << ch);
        }

        //
        // Save the max number of DIMMs on this channel to enforce population rules
        //
        (*channelNvList)[ch].maxDimm = maxDimm;

        //
        // Disable this channel if there is no memory installed
        //
        if (maxDimm == 0) {
          DisableChannelSw (Host, socket, ch);
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "Channel disabled in MemSPD: ch = %d\n", ch);
        }
        //
        // Keep track of the largest number of DIMMs in a channel on this socket
        //
        if (Host->nvram.mem.socket[socket].maxDimmPop < maxDimm) {
          Host->nvram.mem.socket[socket].maxDimmPop = maxDimm;
        }

      } // ch loop

      if (chMemPresent != 0) {
        SystemMemPresent = TRUE;
      }
      if (Host->nvram.mem.socket[socket].minTCK < minTCKcommon) {
        Host->nvram.mem.socket[socket].minTCK = minTCKcommon;
      }

      if (DdrPresent != 0) {
        DdrInSystem = 1;
      }
    } // socket loop

    CheckPORMixingWithinSocket (Host);

    //
    // Check for no memory error
    //
    if (!SystemMemPresent) {
      RcDebugPrint (SDBG_MINMAX,
                      "No memory found!\n");
      OutputWarning (WARN_NO_MEMORY, WARN_NO_MEMORY_MINOR_NO_MEMORY, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK);
    } else if ((DdrInSystem == 0) && (Setup->mem.enforcePOR == ENFORCE_POR_EN)) {
        if (FeaturePcdGet(PcdCteBuild) == FALSE) {
          RcDebugPrint (SDBG_MINMAX,
                         "No DDR found in system!\n");
#ifndef CWV_A0_REMOVE_POST_PO
          OutputWarning (WARN_NO_MEMORY, WARN_NO_MEMORY_MINOR_NO_DDR, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK);
#endif
        }
    }
  } // if (Host->var.common.bootMode == NormalBoot) && (Host->var.mem.subBootMode == ColdBoot)

  ClearMemMapMappedOutDimms (Host);
  CheckForMappedOutDimms (Host);

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    //
    // Loop for each CPU socket
    //
    if (Host->nvram.mem.socket[socket].enabled == 0) {
      continue;
    }
    //SKX change
    channelNvList = &Host->nvram.mem.socket[socket].channelList;
    for (mcId = 0; mcId < MaxImc; mcId++) {
      Host->var.mem.socket[socket].imcEnabled[mcId] = 0;
      if (!Host->var.mem.socket[socket].ImcPresent[mcId]) {
        continue;
      }
      for (ch = 0; ch < NumChPerMc; ch++) {
        if ((*channelNvList)[ch + mcId*(NumChPerMc)].enabled) {
          Host->var.mem.socket[socket].imcEnabled[mcId] = 1;
        }
      } //ch loop
    } //mcId loop
  }  //socket loop

  DetectSurpriseClockStop ();

  UpdateCpuCsrAccessVar_PEI (GetSysCpuCsrAccessVar());

  CoreCheckPORCompat(Host);

  return SUCCESS;
}

/**

  This function applies the given CKE override

  @param Host    - Pointer to sysHost
  @param socket  - Socket
  @param ch      - Channel
  @param ckeMask    - CKE settings to apply

@retval none

**/
VOID
SetCkeOverride (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     ckeMask
)
{
  OverrideCke (socket, ch, ckeMask, ckeMask);

} // SetCkeOverride

/**
  Initializes Req_cmd_Fab_Credit Bit in MC CMI CFG register

  @param[in] Socket   -  Zero based Socket number
  @param[in] McId     -  Zero based Memory controller number

  retval N/A

**/
VOID
InitReqCmdFabCredit (
  IN UINT8 Socket,
  IN UINT8 McId
  )
{
#if (MEM_IP_VER >= MEM_IP_18ww21d)
  BOOLEAN SetValue = FALSE;
  MCCMICFG_N1_MC_2LM_STRUCT McCmiCfgN1;
  UINT8 ChannelInSocket = 0;
  UINT8 ChId = 0;
  UINT8 NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();
  SetValue = IsTmeEnabled (Socket);

// Loop through channel 0 to Maximum channel in MC
  for (ChId = 0; ChId < NumChPerMc; ChId++) {
    // Convert channel number in Socket.
    ChannelInSocket = (McId * NumChPerMc) + ChId;
    McCmiCfgN1.Data = MemReadPciCfgEp (Socket, ChannelInSocket, MCCMICFG_N1_MC_2LM_REG);
    if (SetValue) {
      McCmiCfgN1.Bits.req_cmd_fab_credit = 1;
    } else {
      McCmiCfgN1.Bits.req_cmd_fab_credit = 0;
    }
    MemWritePciCfgEp (Socket, ChannelInSocket, MCCMICFG_N1_MC_2LM_REG, McCmiCfgN1.Data);
  }
#endif
} // InitReqCmdFabCredit

/**
  Initializes CMI Credit registers

  @param[in] Host     - Pointer to sysHost structure

  @retVal   1 CMI Registers initialized
            0 CMI Registers not initialized
**/
UINT32
EFIAPI
InitCmiCreditProgramming (
  IN PSYSHOST Host
  )
{
  UINT8                                         Socket;
  UINT8                                         McId;
  CMI_MAIN_REG_INFO                             CmiMainRegInfo;
  UINT8                                         Ch;
  UINT8                                         ChId;
  UINT8                                         ProgramableValue[MAX_CH];
  UINT8                                         CmiSetupValue;
  UINT32                                        Status;
  UINT8                                         NumChPerMc;
  UINT8                                         MaxImc;

  MaxImc = GetMaxImc ();

  struct channelNvram (*ChannelNvList)[MAX_CH];

  //Initialize Defaults
  Status              = 0;

  Socket = Host->var.mem.currentSocket;
  ChannelNvList = GetChannelNvList (Host, Socket);
  NumChPerMc = GetNumChannelPerMc ();

  if (IsSiliconWorkaroundEnabled ("S14011736925")) {
    ProgramWpqPchEntryEnBits32To39 (Socket);
  }

  if (!IsMemFlowEnabled (InitializeCmiCreditProgramming)) {
    return SUCCESS;
  }
  CmiSetupValue = GetCmiSetupProgramValue ();

  if (Host->nvram.mem.socket[Socket].enabled == 0) {
    return Status;
  }

  if (Host->nvram.mem.socket[Socket].maxDimmPop == 0) {
    return Status;
  }

  ZeroMem (ProgramableValue, sizeof (ProgramableValue));

  PrepareForCmiCreditProgramming (Socket);

  if (CmiSetupValue == CMI_AUTO_OPTION)  {
    GetCmiProgramValue (Host, Socket, ProgramableValue);
  } else {
    //If Auto, then program the value we calculated, else program default value
    SetMem (ProgramableValue, sizeof (ProgramableValue), CMI_REGISTER_DEFAULTS);
  }

  Status = ProgramMemMcCmiRegisters (ProgramableValue, Socket);

  for (McId = 0; McId < MaxImc; McId++) {

    if (Host->var.mem.socket[Socket].imcEnabled[McId] == 0) {
      continue;
    }

#ifndef SPR_MCDDR // this workaroud is tracked by 1507320568
    ProgramMesh2MemCmiRegisters (ProgramableValue, Socket, McId);
#endif //SPR_MCDDR
    InitReqCmdFabCredit (Socket, McId);

    for (ChId = 0; ChId < NumChPerMc; ChId++) {
      Ch = (McId * NumChPerMc) + ChId;
      if (IsChannelEnabled (Socket, Ch)) {
        ProgramTmeCmiRegisters (Socket, Ch, ProgramableValue[Ch]);
      }
    }
  }

  //
  // Output Checkpoint before setting CMI credit done bits
  //
  OutputCheckpoint (CHECKPOINT_MAJOR_CMI_CREDIT, CHECKPOINT_MINOR_CMI_SET_DONE_BITS, Socket);
  if (IsSiliconWorkaroundEnabled ("S1408860212")) {
    DdrtCmi2SSuperColliderWa (Host, Socket);
  }

  //set done bits here
  for (McId = 0; McId < MaxImc; McId++) {

    if (Host->var.mem.socket[Socket].imcEnabled[McId] == 0) {
      continue;
    }
    ProgramTmeCmiConfigDoneAllCh (Socket, McId);
    if (!IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
      //Set Initialized bit and wait for done
      for (ChId = 0; ChId < NumChPerMc; ChId++) {
        Ch = (McId * NumChPerMc) + ChId;
        if ((*ChannelNvList)[Ch].enabled == 1) {
          SetM2mCmiEgressCreditChnlConfigDone (Socket, McId, ChId, TRUE);
        }
      }
    }
    SetM2mCmiMiscConfigDoneAllCh (Socket, McId, TRUE);

    for (ChId = 0; ChId < NumChPerMc; ChId++) {
      Ch = (McId * NumChPerMc) + ChId;
      if ((*ChannelNvList)[Ch].enabled == 1) {
        CmiMainRegInfo.McMiscCmiCreditConfig[ChId].Data = MemReadPciCfgMC (Socket, McId, McMiscCmiCreditConfigRegAddr[ChId]);
        CmiMainRegInfo.McMiscCmiCreditConfig[ChId].Bits.credit_init_done = 1;
        MemWritePciCfgMC (Socket, McId, McMiscCmiCreditConfigRegAddr[ChId], CmiMainRegInfo.McMiscCmiCreditConfig[ChId].Data);
      }
    }
    for (ChId = 0; ChId < NumChPerMc; ChId++) {
      Ch = (McId * NumChPerMc) + ChId;
      if ((*ChannelNvList)[Ch].enabled == 1) {
        CmiMainRegInfo.McCmiReqCpcfg.Data = MemReadPciCfgEp (Socket, Ch, MCCMIREQCPCFG_N1_MC_2LM_REG);
        CmiMainRegInfo.McCmiReqCpcfg.Bits.req_credit_cfg_done = 1;
        MemWritePciCfgEp (Socket, Ch, MCCMIREQCPCFG_N1_MC_2LM_REG, CmiMainRegInfo.McCmiReqCpcfg.Data);
      }
    }
  }

//Start Pollig here
  for (McId = 0; McId < MaxImc; McId++) {

    if (Host->var.mem.socket[Socket].imcEnabled[McId] == 0) {
      continue;
    }
    WaitForCmiMiscConfigChannelsCompleteTarget (Host, Socket, McId, &Status);
    for (ChId = 0; ChId < NumChPerMc; ChId++) {
      Ch = (McId * NumChPerMc) + ChId;
      if ((*ChannelNvList)[Ch].enabled == 1) {
        WaitForMiscCmiCreditConfigCompleteTarget (Host,
          Socket,
          McId,
          ChId,
          (VOID *)&(CmiMainRegInfo.McMiscCmiCreditConfig[ChId]),
          McMiscCmiCreditConfigRegAddr[ChId],
          &Status
        );
      }
    }
    for (ChId = 0; ChId < NumChPerMc; ChId++) {
      Ch = (McId * NumChPerMc) + ChId;
      if ((*ChannelNvList)[Ch].enabled == 1) {
        WaitForMcCmiReqCpcfgCompleteTarget (Host, Socket, Ch);
      }
    }
    WaitForTmeCmiConfigCompleteAllCh (Socket, McId);
  }
  return Status;
}

/**
  Print memory performance settings

  @param Host   - Pointer to sysHost
  @param Socket - Socket value

  @retval N/A
**/
VOID
PrintMemPerformanceSettings (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
#if defined(DEBUG_CODE_BLOCK)
  if (PcdGetBool (PcdMrcDisplayPerformanceValues) == TRUE) {
    UINT8               Ch;
    UINT8               Index;
    UINT32              Address = 0;
    UINT32              Value = 0;
    struct channelNvram (*ChannelNvList)[MAX_CH];
    UINT8               MaxChDdr;
    UINT8               MaxIndex;

    MaxChDdr = GetMaxChDdr ();
    ChannelNvList = GetChannelNvList (Host, Socket);

    MaxIndex = (sizeof (MemTimingRegList) / sizeof (MemTimingRegList[0]));
    // Memory Timing registers
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }

      for (Index = 0; Index < MaxIndex; Index++) {
        if (Address != MemTimingRegList[Index].Address) {
          Address = MemTimingRegList[Index].Address;
          Value = MemReadPciCfgEp (Socket, Ch, Address);
        }

        PrintPerformanceRegisters (Host, Socket, Value, MemTimingRegList, Index, MemTimingFieldList, MaxIndex, "MEM_TIMINGS", Ch);
      }
    }

    MaxIndex = (sizeof (SchRegList) / sizeof (SchRegList[0]));
    // Scheduler registers
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }

      for (Index = 0; Index < MaxIndex; Index++) {
        if (Address != SchRegList[Index].Address) {
          Address = SchRegList[Index].Address;
          Value = MemReadPciCfgEp (Socket, Ch, SchRegList[Index].Address);
        }

        PrintPerformanceRegisters (Host, Socket, Value, SchRegList, Index, SchFieldList, MaxIndex, "SCHEDULER", Ch);
      }
    }
  }
#endif // DEBUG_CODE_BLOCK

  return;

}

/**
   Synchronize Mcmtr register of two sockets.

  @param SbspId      -- SBSP index
  @param PendingSoc  -- Socket to synchronize index
  @param McId        -- Memory Controller id

  @retval none

**/
VOID
EFIAPI
MemSyncPerMcCsrs (
  IN  UINT8     SbspId,
  IN  UINT8     PendingSocId,
  IN  UINT8     McId
  )
{
  UINT32 Data32;
  UINT8 ChOffset;
  UINT8 NumChPerMc;
  NumChPerMc = GetNumChannelPerMc ();

  for (ChOffset = (NumChPerMc * McId); ChOffset < (NumChPerMc * (McId + 1)); ChOffset++) {
    Data32 = MemReadPciCfgEp (SbspId, ChOffset, MCMTR_MC_MAIN_REG);
    MemWritePciCfgEp (PendingSocId, ChOffset, MCMTR_MC_MAIN_REG, Data32);
  }

}

/**

  Set MCA DDRT Thermal Event

  @param[in] Socket                  - Socket number
  @param[in] Channel                 - Channel number
  @param[in] Value                   - TRUE: Disable
                                       FALSE: Enable
  @retval n/a

**/
VOID
EFIAPI
SetMcaDdrtThermalEvent (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN BOOLEAN   Value
)
{
  PSYSHOST                                  Host;
  DDRT_MCA_CTL_MC_2LM_STRUCT                DdrtMcaCtl;

  Host = GetSysHostPointer ();

  DdrtMcaCtl.Data = MemReadPciCfgEp (Socket, Channel, DDRT_MCA_CTL_MC_2LM_REG);
  if (Value) {
    DdrtMcaCtl.Bits.dis_fnv_thermal_error = 1;
  } else {
    DdrtMcaCtl.Bits.dis_fnv_thermal_error = 0;
  }
  MemWritePciCfgEp (Socket, Channel, DDRT_MCA_CTL_MC_2LM_REG, DdrtMcaCtl.Data);
}

/**

  Disable DDRT Thermal Event in defeature register

  @param[in] Socket                  - Socket number
  @param[in] Channel                 - Channel number
  @param[in] InNormalMode            - True: In normal mode
                                       False: Not in normal mode

  @retval n/a

**/
VOID
EFIAPI
SetDefeatureThermalEvent (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN BOOLEAN   InNormalMode
)
{
  PSYSHOST                                  Host;
  DDRT_DEFEATURE_MC_2LM_STRUCT              DdrtDefeature;

  Host = GetSysHostPointer ();

  if (InNormalMode) {
    //it needs to go 0->1->0, is needed so that it can clear what the MC detected
    DdrtDefeature.Data = MemReadPciCfgEp (Socket, Channel, DDRT_DEFEATURE_MC_2LM_REG);
    DdrtDefeature.Bits.dis_thermal_event = 0;
    MemWritePciCfgEp (Socket, Channel, DDRT_DEFEATURE_MC_2LM_REG, DdrtDefeature.Data);

    FixedDelayMicroSecond (1);

    DdrtDefeature.Data = MemReadPciCfgEp (Socket, Channel, DDRT_DEFEATURE_MC_2LM_REG);
    DdrtDefeature.Bits.dis_thermal_event = 1;
    MemWritePciCfgEp (Socket, Channel, DDRT_DEFEATURE_MC_2LM_REG, DdrtDefeature.Data);

    FixedDelayMicroSecond (1);

    DdrtDefeature.Data = MemReadPciCfgEp (Socket, Channel, DDRT_DEFEATURE_MC_2LM_REG);
    DdrtDefeature.Bits.dis_thermal_event = 0;
    MemWritePciCfgEp (Socket, Channel, DDRT_DEFEATURE_MC_2LM_REG, DdrtDefeature.Data);
  } else {
    DdrtDefeature.Data = MemReadPciCfgEp (Socket, Channel, DDRT_DEFEATURE_MC_2LM_REG);
    DdrtDefeature.Bits.dis_thermal_event = 1;
    MemWritePciCfgEp (Socket, Channel, DDRT_DEFEATURE_MC_2LM_REG, DdrtDefeature.Data);
  }
}

/*++

  This routine determines that the various indexes associated with each rank

  @param[in] Host  - Pointer to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch    - Channel number (0-based)
  @param[in] Dimm  - DIMM number (0-based)
  @param[in] Rank  - Rank number (0-based)

  @retval N/A

--*/
VOID
EFIAPI
SetRankIndexes (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  struct RankCh       (*RankPerCh)[MAX_RANK_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct channelNvram (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);
  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  RankList = GetRankNvList (Host, Socket, Ch, Dimm);
  RankPerCh = &(*ChannelNvList)[Ch].rankPerCh;

  //
  // Enforce that rankIndex == logicalRank
  //
  (*RankList)[Rank].rankIndex = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);

  if (((*ChannelNvList)[Ch].encodedCSMode != 1) || (Rank < 2)) {
    (*RankPerCh)[ (*RankList)[Rank].rankIndex].dimm = Dimm;
    (*RankPerCh)[ (*RankList)[Rank].rankIndex].rank = Rank;
  }

  //
  // Get physical rank
  //
  (*RankList)[Rank].phyRank = (Dimm * 4) + Rank;

  //
  // Get logical rank
  //
  (*RankList)[Rank].logicalRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);

  //
  // Get ODT Index
  //
  (*RankList)[Rank].ODTIndex = (Dimm << 1) | (Rank & 1);

  //
  // Get CKE Index
  //
  (*RankList)[Rank].CKEIndex = (*RankList)[Rank].logicalRank;

  // Get CTL Index
  if (Host->nvram.mem.dimmTypePresent == RDIMM) {
    (*RankList)[Rank].ctlIndex = (Dimm << 1);
  } else {
    (*RankList)[Rank].ctlIndex = (Dimm << 1) | Rank;
  }

  //
  // Get CK Index; Get CS Index
  //
  GetClkCsIndexSoc (Socket, Ch, Dimm, Rank, Host->nvram.mem.dimmTypePresent, &(*RankList)[Rank].ckIndex, &(*RankList)[Rank].CSIndex);
} // SetRankIndexes

/**

  Set MCA CTL for Unexpected Pkg CMI Idle event

  @param[in] Socket                  - Socket number
  @param[in] Channel                 - Channel number
  @param[in] Value                   - 1: Disable
                                       0: Enable

  @retval n/a

**/
VOID
EFIAPI
SetMcaCmiIdleEvent (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Value
)
{
  PSYSHOST                                  Host;
  DDRT_MCA_CTL_MC_2LM_STRUCT                McaCtl;

  Host = GetSysHostPointer ();

  McaCtl.Data = MemReadPciCfgEp (Socket, Channel, DDRT_MCA_CTL_MC_2LM_REG);
  McaCtl.Bits.dis_unexp_pkt_cmi_idle = Value;
  MemWritePciCfgEp (Socket, Channel, DDRT_MCA_CTL_MC_2LM_REG, McaCtl.Data);
}
