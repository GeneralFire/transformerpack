/** @file

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

#include <Library/MemoryCoreLib.h>
#include <Library/MemCallTableLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemThermManageLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MktmeEarlyFlow.h>
#include <Library/SgxPreMemInit.h>
#include <Library/LegacyRmtLib.h>
#include <Library/AdvMemTestLib.h>
#include <Library/MemDecodeLib.h>
#include <Library/SimulationServicesLib.h>
#include <Library/MemPopulationPorLib.h>
#include <Memory/MemoryCheckpointCodes.h>

CallTableEntryType CallTableMain[] = {
  // The functions are executed in the following order, as the policy flag dictates.
  // Mrctask,                     Major POST Code,                    Minor POST Code,                                PolicyFlag,               MP flags,               Debug String
  { ParallelModeDispatch,         CHECKPOINT_MAJOR_PARALLEL_DISPATCH, 0,                                              MRC_PF_ALL,               MRC_MP_PARALLEL,        CALL_TABLE_STRING("Parallel Mode Dispatch") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_AP_BOOT_MODE,             MRC_PF_ALL,               MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync AP Boot Mode") },
  { DetermineBootMode,            CHECKPOINT_MAJOR_SELECT_BOOT_MODE,  0,                                              MRC_PF_ALL,               MRC_MP_PARALLEL_SBSP,   CALL_TABLE_STRING("Select Boot Mode") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_SBSP_BOOT_MODE,           MRC_PF_ALL,               MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync SBSP Boot Mode") },
  { InitStructuresLate,           CHECKPOINT_MAJOR_INIT_STRUCT_LATE,  0,                                              MRC_PF_ALL,               MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Init Structures Late") },
  { DetectDIMMConfig,             CHECKPOINT_MAJOR_DIMM_DETECT,       0,                                              MRC_PF_ALL,               MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Detect DIMM Configuration") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_AP_DATA,                  MRC_PF_ALL,               MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync AP Data") },
  { CheckPORCompat,               CHECKPOINT_MAJOR_CHECK_POR,         0,                                              MRC_PF_ALL,               MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("Check POR Compatibility") },
  { InitClocks,                   CHECKPOINT_MAJOR_CLOCK_INIT,        0,                                              MRC_PF_ALL,               MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("Initialize clocks for all MemSs") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_SBSP_DATA,                MRC_PF_ALL,               MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync SBSP Data") },
  { UnlockMemoryAfterRamp,        CHECKPOINT_MAJOR_UNLOCK_MEM_REGS,   0,                                              MRC_PF_ALL,               MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Unlock Memory") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_AP_DATA,                  MRC_PF_ALL,               MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync AP Data") },
  { CheckStatus,                  CHECKPOINT_MAJOR_CHECK_STATUS,      0,                                              MRC_PF_ALL,               MRC_MP_SERIAL,          CALL_TABLE_STRING("Check Status") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_SBSP_STATUS,              MRC_PF_ALL,               MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync SBSP Status") },
  { CheckXMP,                     CHECKPOINT_MAJOR_CONFIG_XMP,        0,                                              MRC_PF_COLD,              MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Check XMP") },
  { CheckVdd,                     CHECKPOINT_MAJOR_CLOCK_INIT,        CHECKPOINT_MINOR_SET_CLK_VDD,                   MRC_PF_ALL,               MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("Set Vdd") },
  { CheckDimmRanks,               CHECKPOINT_MAJOR_RANK_DETECT,       0,                                              MRC_PF_ALL,               MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("Check DIMM Ranks") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_SBSP_DATA,                MRC_PF_ALL,               MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync SBSP Data") },
  { InitThrottlingEarly,          CHECKPOINT_MAJOR_INIT_THROTTLING,   CHECKPOINT_MINOR_EARLY_DDR_THERM,               MRC_PF_ALL,               MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Initialize Throttling Early") },
  { InitMem,                      CHECKPOINT_MAJOR_EARLY_INIT_MEM,    0,                                              MRC_PF_ALL,               MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Initialize Memory") },
  { GatherSPDData,                CHECKPOINT_MAJOR_SPD_DATA,          0,                                              MRC_PF_ALL,               MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Gather SPD Data") },
#ifdef DEBUG_CODE_BLOCK
  { DIMMInfo,                     CHECKPOINT_MAJOR_DIMM_INFO,         CHECKPOINT_MINOR_DIMM_INFO_EARLY,               MRC_PF_ALL,               MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Socket DIMM Information") },
#endif // DEBUG_CODE_BLOCK
  { PrepareNvdimmNForTraining,    CHECKPOINT_MAJOR_NVDIMM,            CHECKPOINT_MINOR_NVDIMM_STS,                    MRC_PF_ALL,               MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Prep NVDIMM for Training") },
  { EarlyConfig,                  CHECKPOINT_MAJOR_GLOBAL_EARLY,      0,                                              MRC_PF_ALL,               MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Early Configuration") },
  { InitInterface,                CHECKPOINT_MAJOR_DDRIO_INIT,        CHECKPOINT_MINOR_EARLY_INIT,                    MRC_PF_ALL,               MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("DDRIO Initialization") },
  { InitDdrioInterfaceLate,       CHECKPOINT_MAJOR_DDRIO_INIT,        CHECKPOINT_MINOR_LATE_INIT,                     MRC_PF_ALL,               MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("DDRIO Initialization Late") },
  { DdrTraining,                  CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MAJOR_NO_CODE,                       MRC_PF_ALL,               MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("DDR Training") },
#ifdef   DEBUG_CODE_BLOCK
//{ DisplayTrainResults,          CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_DISPLAY_RESULTS,               MRC_PF_COLD,              MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Display Training Results") },
#endif // DEBUG_CODE_BLOCK
  { PostTrainingInit,             CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_POST_TRAINING_INIT,            MRC_PF_ALL,               MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Post-Training Initialization") },
//{ RankMarginTool,               CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_RMT,                           MRC_PF_ALL,               MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Rank Margin Tool") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_AP_SSA_DATA,              MRC_PF_ALL,               MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync AP Pre SSA Data") },
//{ SSAInit,                      CHECKPOINT_MAJOR_SSA_API_INIT,      CHECKPOINT_MINOR_SSA_API_INIT_START,            MRC_PF_ALL,               MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("BIOS SSA Initialization") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_SBSP_SSA_DATA,            MRC_PF_ALL,               MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync SBSP Post SSA Data") },
  { CoreRestoreNVDIMMs,           CHECKPOINT_MAJOR_NVDIMM,            CHECKPOINT_MINOR_NVDIMM_RESTORE,                MRC_PF_ALL,               MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Core Restore NVDIMMs") },
  { CoreArmNVDIMMs,               CHECKPOINT_MAJOR_NVDIMM,            CHECKPOINT_MINOR_NVDIMM_ARM,                    MRC_PF_ALL,               MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Core Arm NVDIMMs") },
  { LateConfig,                   CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_END_TRAINING,                  MRC_PF_ALL,               MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Late Configuration") },
  { InitThrottling,               CHECKPOINT_MAJOR_INIT_THROTTLING,   CHECKPOINT_MINOR_MID_DDR_THERM,                 MRC_PF_ALL,               MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Initialize Throttling") },
  { AdvancedMemTest,              CHECKPOINT_MAJOR_MEMBIST,           0,                                              MRC_PF_ALL,               MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("MemTest") },
  { SetupSVLandScrambling,        CHECKPOINT_MAJOR_SVL_SCRAMBLE,      0,                                              MRC_PF_ALL,               MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Setup SVL and Scrambling") },
  { DdrMemInit,                   CHECKPOINT_MAJOR_MEMINIT,           0,                                              MRC_PF_ALL,               MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("MemInit") },
  { CheckRASSupportAfterMemInit,  CHECKPOINT_MAJOR_CHECK_RAS,         0,                                              MRC_PF_ALL,               MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Check Ras Support After MemInit") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_AP_NVRAM_DATA,            MRC_PF_ALL,               MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync AP NVRAM Data") },
  { EnforcePopulationPor,         CHECKPOINT_MAJOR_NO_CODE,           0,                                              MRC_PF_ALL,               MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("Check Memory Topology") },
  { InitMemoryMap,                CHECKPOINT_MAJOR_DDR_MEMMAP,        0,                                              MRC_PF_ALL,               MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("Initialize Memory Map") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_SBSP_VAR_DATA,            MRC_PF_ALL,               MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync SBSP Variable Data") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_SBSP_MEM_MODE,            MRC_PF_ALL,               MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync SBSP Memory Mode") },
  { MktmeEarlyFlow,               CHECKPOINT_MAJOR_MKTME_EARLY_FLOW,  CHECKPOINT_MINOR_MKTME_EARLY_FLOW,              MRC_PF_ALL,               MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("TME Flow") },
  { SgxPreMemInitEntryPoint,      CHECKPOINT_MAJOR_SGX_PREMEM,        CHECKPOINT_MINOR_SGX_CHECK_CAP_ST,              MRC_PF_ALL,               MRC_MP_SERIAL,          CALL_TABLE_STRING("SGX PreMem Check Capability ST") },
  { SgxPreMemInitEntryPoint,      CHECKPOINT_MAJOR_SGX_PREMEM,        CHECKPOINT_MINOR_SGX_CHECK_CAP_MT,              MRC_PF_ALL,               MRC_MP_PARALLEL,        CALL_TABLE_STRING("SGX PreMem Check Capability MT") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_AP_FINAL_ERROR_SYNC,      MRC_PF_ALL,               MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync AP Final Error Sync") },
  { SgxPreMemInitEntryPoint,      CHECKPOINT_MAJOR_SGX_PREMEM,        CHECKPOINT_MINOR_SGX_PREMEM,                    MRC_PF_ALL,               MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("SGX PreMem Init") },
  { SwitchToNormalMode,           CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_NORMAL_MODE,                   MRC_PF_ALL,               MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Switch to Normal Mode") },
  { InitCmiCreditProgramming,     CHECKPOINT_MAJOR_CMI_CREDIT,        CHECKPOINT_MINOR_CMI_CREDIT_INIT,               MRC_PF_ALL,               MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Init CMI Credit Programming") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_AP_FINAL_ERROR_SYNC,      MRC_PF_ALL,               MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync AP Final Error Sync") },
  { InitADR,                      CHECKPOINT_MAJOR_INIT_ADR,          0,                                              MRC_PF_ALL,               MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("Initialize ADR") },
  { SetRASConfig,                 CHECKPOINT_MAJOR_RAS_CONFIG,        0,                                              MRC_PF_ALL,               MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("Set RAS Configuration") },
  { MemLate,                      CHECKPOINT_MAJOR_MEM_LATE,          0,                                              MRC_PF_ALL,               MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("Memory Late") },
//{InitSADInterfaceDataStruct,    CHECKPOINT_MAJOR_NO_CODE,           0,                                              MRC_PF_ALL,               MRC_MP_BOTH,            CALL_TABLE_STRING("Memory Late")},
  { PrintAllStats,                CHECKPOINT_MAJOR_NO_CODE,           0,                                              MRC_PF_ALL,               MRC_MP_BOTH,            CALL_TABLE_STRING("Print All Stats") },
  { PrintPerformanceSettings,     CHECKPOINT_MAJOR_NO_CODE,           0,                                              MRC_PF_ALL,               MRC_MP_BOTH,            CALL_TABLE_STRING("Print Performance Seetings") },
#ifdef DEBUG_CODE_BLOCK
  { DIMMInfo,                     CHECKPOINT_MAJOR_DIMM_INFO,         CHECKPOINT_MINOR_DIMM_INFO_LATE,                 MRC_PF_ALL,              MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("DIMM Information After MRC") },
#endif // DEBUG_CODE_BLOCK
  { DDRResetLoop,                 CHECKPOINT_MAJOR_NO_CODE,           0,                                               MRC_PF_ALL,              MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("DDR Reset Loop") },
};

UINT8 CallTableMainSize = sizeof(CallTableMain) / sizeof(CallTableEntryType);

CallTableEntryType CallTableDdrTraining[] = {
  { PreTrainingInit,              CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_PRE_TRAINING_INIT,               MRC_PF_ALL,             MRC_MP_BOTH,            CALL_TABLE_STRING("Pre-Training Initialization") },
  { EarlyCtlClk,                  CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_EARLY_CTL_CLK,                   MRC_PF_COLD,            MRC_MP_BOTH,            CALL_TABLE_STRING("Early CTL/CLK Training") },
  { EarlyCmdClk,                  CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_EARLY_CMD_CLK,                   MRC_PF_COLD,            MRC_MP_BOTH,            CALL_TABLE_STRING("Early CMD/CLK Training") },
  { QxCAClk,                      CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_QX_CA_CLK,                       MRC_PF_COLD,            MRC_MP_BOTH,            CALL_TABLE_STRING("RCD QxCA Training") },
#ifdef LRDIMM_SUPPORT
  { Ddr4LrdimmBacksideRxPhase,    CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_RX_BACKSIDE_PHASE_TRAINING,      MRC_PF_COLD,            MRC_MP_BOTH,            CALL_TABLE_STRING("Lrdimm Backside Phase Training(RX)") },
  { Ddr4LrdimmBacksideRxCycle,    CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_RX_BACKSIDE_CYCLE_TRAINING,      MRC_PF_COLD,            MRC_MP_BOTH,            CALL_TABLE_STRING("Lrdimm Backside Cycle Training(RX)") },
  { Ddr4LrdimmBacksideRxDelay,    CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_RX_BACKSIDE_DELAY_TRAINING,      MRC_PF_COLD,            MRC_MP_BOTH,            CALL_TABLE_STRING("Lrdimm Backside Delay Training(RX)") },
#endif
  { ReceiveEnable,                CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_RECEIVE_ENABLE_BASIC,            MRC_PF_COLD,            MRC_MP_BOTH,            CALL_TABLE_STRING("Receive Enable") },
  { ReadDqDqsPerBit,              CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_RX_DQ_DQS_BASIC,                 MRC_PF_COLD,            MRC_MP_BOTH,            CALL_TABLE_STRING("Rx Dq/Dqs Basic Per Bit") },
  { DqSwizzleDiscovery,           CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_DQ_SWIZZLE_DISCOVERY,            MRC_PF_COLD,            MRC_MP_BOTH,            CALL_TABLE_STRING("Dq Swizzle Discovery") },
#ifdef LRDIMM_SUPPORT
  { Ddr4LrdimmBacksideTxFineWL,   CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_TX_BACKSIDE_FINE_WL_TRAINING,    MRC_PF_COLD,            MRC_MP_BOTH,            CALL_TABLE_STRING("Lrdimm Backside Fine WL Training(TX)") },
  { Ddr4LrdimmBacksideTxCoarseWL, CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_TX_BACKSIDE_COARSE_WL_TRAINING,  MRC_PF_COLD,            MRC_MP_BOTH,            CALL_TABLE_STRING("Lrdimm Backside Coarse WL Training(TX)") },
  { Ddr4LrdimmBacksideTxDelay,    CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_TX_BACKSIDE_DELAY_TRAINING,      MRC_PF_COLD,            MRC_MP_BOTH,            CALL_TABLE_STRING("Lrdimm Backside Delay Training(TX)") },
#endif
  { WriteLeveling,                CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_WRITE_LEVELING_BASIC,            MRC_PF_COLD,            MRC_MP_BOTH,            CALL_TABLE_STRING("Write Leveling") },
  { WriteLevelingCleanUp,         CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_WR_FLY_BY,                       MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("Write Fly By") },
  { WriteDqDqsPerBit,             CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_TX_DQ_BASIC,                     MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("Tx Dq/Dqs Basic Per Bit") },
  { DqsSwizzleDiscoveryx16,       CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_DQS_SWIZZLE_DISCOVERY,           MRC_PF_COLD,            MRC_MP_BOTH,            CALL_TABLE_STRING("x16 Dqs Swizzle Discovery")},
  { PostPackageRepairMain,        CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_PPR_FLOW,                        MRC_PF_ALL|MRC_PF_RMT,  MRC_MP_BOTH,            CALL_TABLE_STRING("PPR Flow") },
  { WrEarlyVrefCentering,         CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_E_TRAIN_WR_VREF,                 MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("Wr Early Vref Centering") },
  { RdEarlyVrefCentering,         CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_E_TRAIN_RD_VREF,                 MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("Rd Early Vref Centering") },
  { TrainCmdTxEq,                 CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_T_CMD_TX_EQ,                     MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("Cmd Tx Eq") },
  { CmdVrefCentering,             CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_CMD_VREF_CENTERING,              MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("CMD Vref Centering") },
  { LateCmdClk,                   CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_LATE_CMD_CLK,                    MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("Late Cmd/Clk Training") },
  { EarlyReadID,                  CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_EARLY_RID_FINE,                  MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("Early Read ID") },
#ifdef LRDIMM_SUPPORT
  { TxLRDIMMDqCentering,          CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_TX_LRDIMM_DQ_CENTERING,          MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("Wr LRDIMM DQ Centering") },
  { RxLRDIMMDqCentering,          CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_RX_LRDIMM_DQ_CENTERING,          MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("Rd LRDIMM DQ Centering") },
  { WrLRDIMMVrefCentering,        CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_TRAIN_WR_VREF_LRDIMM,            MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("Wr LRDIMM backside VREF Training") },
  { RdLRDIMMVrefCentering,        CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_TRAIN_RD_VREF_LRDIMM,            MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("Rd LRDIMM backside VREF Training") },
#endif
  { TrainMcOdt,                   CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_T_MC_ODT,                        MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("McOdt") },
  { TrainDramRon,                 CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_T_DRAM_RON,                      MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("DramRon") },
  { TrainMcRon,                   CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_T_MC_RON,                        MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("McRon") },
  { TrainTxEq,                    CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_T_TX_EQ,                         MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("Tx Eq") },
  { TrainDramRxEq,                CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_T_DRAM_RX_EQ,                    MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("Dram Rx Eq") },
  { TrainImode,                   CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_T_IMODE,                         MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("Imode") },
  { TrainCtle,                    CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_T_CTLE,                          MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("CTLE") },
  { TrainTxRiseFallSlewRate,      CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_T_TX_RISE_FALL_SLEW_RATE,        MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("Tx Rise Fall Slew Rate") },
  { TrainTcoComp,                 CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_T_TCO_COMP,                      MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("TCO COMP") },
  { TxPerBitDeskew,               CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_PER_BIT_DESKEW_TX,               MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("Tx Per Bit Deskew Training") },
  { RxPerBitDeskew,               CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_PER_BIT_DESKEW_RX,               MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("Rx Per Bit Deskew Training") },
  { WrVrefCentering,              CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_TRAIN_WR_VREF,                   MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("Wr Vref Centering") },
  { RdVrefCentering,              CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_TRAIN_RD_VREF,                   MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("Rd Vref Centering") },
  { WrAdvancedCentering,          CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_TRAIN_WR_DQS,                    MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("Tx Dq Adv Training") },
  { RdAdvancedCentering,          CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_TRAIN_RD_DQS,                    MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("Rx Dq/Dqs Adv Training") },
  { TrainPXC,                     CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_PXC_TRAINING,                    MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("PXC Training") },
  { TrainTxDdjc,                  CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_DDJC_TRAINING,                   MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("DDJC Training") },
  { TrainRxDfe,                   CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_T_RX_DFE,                        MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("Rx DFE") },
  { RoundTripOptimize,            CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_ROUND_TRIP_OPTIMIZE,             MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,            CALL_TABLE_STRING("Round Trip Optimization") },
};

UINT8 CallTableDdrTrainingSize = sizeof(CallTableDdrTraining) / sizeof(CallTableEntryType);
