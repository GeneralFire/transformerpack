/** @file
  Mem Power Managment.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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
#include <Library/MemFmcIpLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include "Include/MemMcRegs.h"
#include "Include/MemMcChip.h"
#include "Include/MemMcIpLibInternal.h"
#include <Library/MemDdrioIpLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/SysHostPointerLib.h>
#include "MemHostChipCommon.h"
#include <Library/IoAccessLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemMcIpTargetLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/MemTypeLib.h>
#include <Memory/JedecDefinitions.h>
#include <Library/UsraCsrLib.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/PchAdrLib.h>
#include <CpuPciAccess.h>
#include <Library/MemRcLib.h>
#include <Library/SiliconWorkaroundLib.h>


/**
  Enable Data DLL Off feature, which turns off DDRIO data DLL when MemSS is in CKE
  Power down (Auto Power Down or Pre-Charge Power Down) or Opp Self Refresh mode.

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number
  @param[in] Channel - Memory channel number

  @retval N/A

**/
VOID
PmLpModeDataDllOff (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel
  )
{
  SYS_SETUP                         *Setup;
  BOOLEAN                           DataDllOff;
  LPMODE_PM_CTRL_N0_MCDDC_CTL_STRUCT LpModePmControlN0;
  LPMODE_PM_CTRL_N1_MCDDC_CTL_STRUCT LpModePmControlN1;
  UINT32                            WakeupTimeDclkCycles;
  UINT32                            DataDllWakeupTime;
  UINT32                            MininalTclByHCLK;
  UINT32                            TclByDCLK;
  UINT8                             Index;
  UINT32                            MininalTclByDCLK;
  struct dimmNvram                  (*DimmNvList) [MAX_DIMM];

  Setup = GetSysSetupPointer ();
  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  DataDllOff = FALSE;

  if ((Setup->mem.DataDllOff == DATA_DLL_OFF_DISABLE) || (IsSiliconWorkaroundEnabled ("S1409873794"))) {
    return;
  }

  //
  // If OppSR is enabled on any rank, turn on Data DLL Off feature.
  //
  if (IsOppSrEnabled (Host, Socket, Channel) == TRUE) {
    DataDllOff = TRUE;
  }

  //
  // If CKE Power Down (Auto Power Down or Pre-Charge Power Down) is enabled on any rank, turn on Data DLL Off feature.
  //
  if (IsCkePowerDownEnabled (Host, Socket, Channel) == TRUE) {
    DataDllOff = TRUE;
  }
  if (DataDllOff == TRUE) {

    LpModePmControlN0.Data = MemReadPciCfgEp (Socket, Channel, LPMODE_PM_CTRL_N0_MCDDC_CTL_REG);
    RcDebugPrint (SDBG_DIMM_INFO, "LpModePmControlN0.Data Original = 0x%x \n", (UINT32)(LpModePmControlN0.Data));

    LpModePmControlN1.Data = MemReadPciCfgEp (Socket, Channel, LPMODE_PM_CTRL_N1_MCDDC_CTL_REG);
    RcDebugPrint (SDBG_DIMM_INFO, "LpModePmControlN1.Data Original = 0x%x \n", (UINT32)(LpModePmControlN1.Data));

    //
    // Mscheds_bs.lpmode_pm_ctrl.DataDllOff_Dis [31].
    // Enable bit to enable DataDLL off during CKE powerdown and OSR.
    //
    LpModePmControlN1.Bits.datadlloff_enable = 1;

    //
    // Mscheds_bs.lpmode_pm_ctrl.PHY_Data_DLLWakeTime [30:26].
    // Data dll wake time (22ns as per DDRIO) in number of Hclks.
    // The value should be the Ceil(22ns/(2*Dclk_period)).
    //

    if (((22 * 10000) % GettCK(Host, Socket)) != 0) {
      WakeupTimeDclkCycles = (22 * 10000 / GettCK(Host, Socket)) + 1;
    } else {
      WakeupTimeDclkCycles = (22 * 10000 / GettCK(Host, Socket));
    }

    if ((WakeupTimeDclkCycles % 2) != 0) {
      DataDllWakeupTime = (WakeupTimeDclkCycles / 2) + 1;
    } else {
      DataDllWakeupTime = WakeupTimeDclkCycles / 2;
    }
    LpModePmControlN0.Bits.datadllwakeuptime = DataDllWakeupTime & 0x3; // Part 1 (2 bit) of DataDLL wake up time Number of Hclks
    LpModePmControlN1.Bits.datadllwakeuptime = (DataDllWakeupTime >> 2) & 0x7F; // Part 2 (7 bit) of DataDLL wake up time Number of Hclks

    RcDebugPrint (SDBG_DIMM_INFO, "LpModePmControlN0.Bits.datadllwakeuptime = 0x%x \n", LpModePmControlN0.Bits.datadllwakeuptime);
    RcDebugPrint (SDBG_DIMM_INFO, "LpModePmControlN1.Bits.datadllwakeuptime = 0x%x \n", LpModePmControlN1.Bits.datadllwakeuptime);

    //
    // Data DLL OFF power savings feature uses this value to count the number of
    // cycles to block RDs being issued from scheduler. Value is in Hclks.
    // Accounts for any excess time scheduler needs to wait when Data DLL wake up
    // time exceeds the min Rd return latency.
    // Mscheds_bs.lpmode_pm_ctrl.lpmode_block_Rd [25:21] = \
    // (PHY_Data_DLLWakeTime > Min(All Rd latency per Channel in Hclk)) ? \
    // (PHY_Data_DLLWakeTime - Min(All Rd latency per Channel in Hclk)) : 0;
    //

    MininalTclByDCLK = 0xFF;

    for (Index = 0 ; Index < MAX_DIMM; Index ++ ) {

     if ((*DimmNvList) [Index].dimmPresent == 0) {
      continue;
     }

     if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
        TclByDCLK = PicosecondToClocks (Socket, (*DimmNvList)[Index].tCL);
     } else {
        TclByDCLK = (UINT32) TimeBaseToDCLKDdr4 (Host, Socket, Channel, Index, (*DimmNvList)[Index].tCL, (*DimmNvList)[Index].ftbTAA);
     }

     if (TclByDCLK < MininalTclByDCLK) {
        MininalTclByDCLK = TclByDCLK;
      }
    }

    MininalTclByHCLK = MininalTclByDCLK / 2;

    RcDebugPrint (SDBG_DIMM_INFO, "MininalTclByHCLK = 0x%x \n", (UINT32)MininalTclByHCLK);
    //
    // HSD 1507202367
    //
    LpModePmControlN0.Bits.lpmode_block_rd = (DataDllWakeupTime > MininalTclByHCLK) ? \
                                           (DataDllWakeupTime - MininalTclByHCLK) : 0;

    RcDebugPrint (SDBG_DIMM_INFO, "LpModePmControlN0.Bits.lpmode_block_rd = 0x%x \n", (UINT32)LpModePmControlN0.Bits.lpmode_block_rd);

    //
    // Programe LPMODE_PM_CTRL_N0_MCDDC_CTL_REG and LPMODE_PM_CTRL_N1_MCDDC_CTL_REG.
    //
    RcDebugPrint (SDBG_DIMM_INFO, "LpModePmControlN0.Data New = 0x%x \n", (UINT32)(LpModePmControlN0.Data));
    MemWritePciCfgEp (Socket, Channel, LPMODE_PM_CTRL_N0_MCDDC_CTL_REG, LpModePmControlN0.Data);
    RcDebugPrint (SDBG_DIMM_INFO, "LpModePmControlN1.Data New = 0x%x \n", (UINT32)(LpModePmControlN1.Data));
    MemWritePciCfgEp (Socket, Channel, LPMODE_PM_CTRL_N1_MCDDC_CTL_REG, LpModePmControlN1.Data);
  }

  return;
}

/**
  Program odt_value to HIGH in certain configurations to save power.

  @param[in] Socket     - Socket number
  @param[in] Ch         - Channel number

  @retval    EFI_SUCCESS If odt_value is successfully set.
**/
EFI_STATUS
EFIAPI
SetPowerSaveOdt (
  IN    UINT8     Socket,
  IN    UINT8     Ch
  )
{
  MEMORY_TIMINGS_ADJ_MCDDC_CTL_STRUCT    MemTimingsAdj;
  UINT8                                  Dimm;
  UINT8                                  Rank;
  UINT16                                 RttValue;
  BOOLEAN                                NonTargetRttEnabled;
  struct dimmNvram                       (*DimmNvList) [MAX_DIMM];
  PSYSHOST                               Host = NULL;

  Host = GetSysHostPointer ();

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  NonTargetRttEnabled = FALSE;

  //
  // If any rank in the channel has RTT_NOM_RD or RTT_NOM_WR set, set NonTargetRttEnabled as TRUE.
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
      continue;
    }

    for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
         continue;
      }

      //
      // Check non-target rank RTT_NOM_RD.
      //
      GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, 0, 0, DdrLevel, RttNomRd, GSM_READ_ONLY, &RttValue);
      if (RttValue != DDR5_RTT_OFF) {
        NonTargetRttEnabled = TRUE;
        break;
      }

      //
      // Check non-target rank RTT_NOM_WR.
      //
      GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, 0, 0, DdrLevel, RttNomWr, GSM_READ_ONLY, &RttValue);
      if (RttValue != DDR5_RTT_OFF) {
        NonTargetRttEnabled = TRUE;
        break;
      }
    }

    if (NonTargetRttEnabled == TRUE) {
      break;
    }
  }

  //
  // (1)When a DDR channel is configured with non-target termination (RTT_NOM_RD and RTT_NOM_WR) AND power down mode is
  //    enabled, MRC shall clear memory_timings_adj.odt_value to 0 (which is also default value). This allows ranks in
  //    power down to still provide non-target termination for read/write commands to other ranks.
  // (2)If power down is disabled or non-target termination is disabled (RTT_NOM_RD and RTT_NOM_WR are zero), then odt_value
  //    could be set to 1 to save some power.
  //
  MemTimingsAdj.Data = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG);
  if ((IsCkePowerDownEnabled (Host, Socket, Ch) == FALSE) || (NonTargetRttEnabled == FALSE)) {
    MemTimingsAdj.Bits.odt_value = 1;
  } else {
    MemTimingsAdj.Bits.odt_value = 0;
  }
  MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG, MemTimingsAdj.Data);

  return EFI_SUCCESS;
}

/**

  Program DRAM RAPL Values such as TDP, Min power and Max Power

  @param[in] Socket        - Socket Number, 0 based
  @param[in] DramTpd       - The Spec power allowed for DRAM. The TDP setting
                             is typical (not guaranteed). The units for this
                             value are defined in Watt.
  @param[in] DramMinPwr    - The minimal power setting allowed for DRAM.
                             Lower values will be clamped to this value. The
                             minimum setting is typical (not guaranteed). The
                             units for this value are defined in Watt.
  @param[in] DramMaxPwr    - The maximal power setting allowed for DRAM.
                             Higher values will be clamped to this value. The
                             maximum setting is typical (not guaranteed). The
                             units for this value are defined in Watt.

  @retval N/A

**/
VOID
EFIAPI
ProgramDramRaplValues (
  IN UINT8  Socket,
  IN UINT16 DramTdp,
  IN UINT16 DramMinPwr,
  IN UINT16 DramMaxPwr
  )
{
  DRAM_POWER_INFO_N0_PCU_FUN2_STRUCT  DramPowerInfoLow;
  DRAM_POWER_INFO_N1_PCU_FUN2_STRUCT  DramPowerInfoHigh;

  DramPowerInfoLow.Data = UsraCsrRead (Socket, 0, DRAM_POWER_INFO_N0_PCU_FUN2_REG);
  DramPowerInfoLow.Bits.dram_tdp = DramTdp;
  // dram_min_pwr: This field is deprecated.
  UsraCsrWrite (Socket, 0, DRAM_POWER_INFO_N0_PCU_FUN2_REG, DramPowerInfoLow.Data);

  DramPowerInfoHigh.Data = UsraCsrRead (Socket, 0, DRAM_POWER_INFO_N1_PCU_FUN2_REG);
  DramPowerInfoHigh.Bits.dram_max_pwr = DramMaxPwr;
  UsraCsrWrite (Socket, 0, DRAM_POWER_INFO_N1_PCU_FUN2_REG, DramPowerInfoHigh.Data);
}
