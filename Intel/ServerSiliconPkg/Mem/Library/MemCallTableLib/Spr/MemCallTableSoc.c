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
#include <Library/DdrtCoreLib.h>
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
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/MemPopulationPorLib.h>
#include <Library/HbmChipLib.h>
#include <Library/Ddr5CoreLib.h>

CallTableEntryType CallTableMain[] = {
  // The functions are executed in the following order, as the policy flag dictates.
  // Mrctask,                     Major POST Code,                    Minor POST Code,                                  PolicyFlag,             MP flags,               Debug String
  { ParallelModeDispatch,         CHECKPOINT_MAJOR_PARALLEL_DISPATCH, 0,                                                MRC_PF_ALL,             MRC_MP_PARALLEL,        CALL_TABLE_STRING("Parallel Mode Dispatch") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_AP_BOOT_MODE,               MRC_PF_ALL,             MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync AP Boot Mode") },
  { DetermineBootMode,            CHECKPOINT_MAJOR_SELECT_BOOT_MODE,  0,                                                MRC_PF_ALL,             MRC_MP_PARALLEL_SBSP,   CALL_TABLE_STRING("Select Boot Mode") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_SBSP_BOOT_MODE,             MRC_PF_ALL,             MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync SBSP Boot Mode") },
  { InitStructuresLate,           CHECKPOINT_MAJOR_INIT_STRUCT_LATE,  0,                                                MRC_PF_ALL,             MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Init Structures Late") },
  { DetectDIMMConfig,             CHECKPOINT_MAJOR_DIMM_DETECT,       0,                                                MRC_PF_ALL,             MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Detect DIMM Configuration") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_AP_DATA,                    MRC_PF_ALL,             MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync AP Data") },
  { CheckPORCompat,               CHECKPOINT_MAJOR_CHECK_POR,         0,                                                MRC_PF_ALL,             MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("Check POR Compatibility") },
  { HbmPreTraining,               CHECKPOINT_MAJOR_HBM,               CHECKPOINT_MINOR_HBM_PRE_TRAINING,                MRC_PF_ALL,             MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("HBM Pre-Training") },
  { InitClocks,                   CHECKPOINT_MAJOR_CLOCK_INIT,        0,                                                MRC_PF_ALL,             MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("Initialize clocks for all MemSs") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_SBSP_DATA,                  MRC_PF_ALL,             MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync SBSP Data") },
  { UnlockMemoryAfterRamp,        CHECKPOINT_MAJOR_UNLOCK_MEM_REGS,   0,                                                MRC_PF_ALL,             MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Unlock Memory") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_AP_DATA,                    MRC_PF_ALL,             MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync AP Data") },
  { CheckStatus,                  CHECKPOINT_MAJOR_CHECK_STATUS,      0,                                                MRC_PF_ALL,             MRC_MP_SERIAL,          CALL_TABLE_STRING("Check Status") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_SBSP_STATUS,                MRC_PF_ALL,             MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync SBSP Status") },
  { CheckXMP,                     CHECKPOINT_MAJOR_CONFIG_XMP,        0,                                                MRC_PF_COLD,            MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Check XMP") },
  { CheckVdd,                     CHECKPOINT_MAJOR_CLOCK_INIT,        CHECKPOINT_MINOR_SET_CLK_VDD,                     MRC_PF_ALL,             MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("Set Vdd") },
  { CheckDimmRanks,               CHECKPOINT_MAJOR_RANK_DETECT,       0,                                                MRC_PF_ALL,             MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("Check DIMM Ranks") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_SBSP_DATA,                  MRC_PF_ALL,             MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync SBSP Data") },
  { InitThrottlingEarly,          CHECKPOINT_MAJOR_INIT_THROTTLING,   CHECKPOINT_MINOR_EARLY_DDR_THERM,                 MRC_PF_ALL,             MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Initialize Throttling Early") },
  { InitMem,                      CHECKPOINT_MAJOR_EARLY_INIT_MEM,    0,                                                MRC_PF_ALL,             MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Initialize Memory") },
  { GatherSPDData,                CHECKPOINT_MAJOR_SPD_DATA,          0,                                                MRC_PF_ALL,             MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Gather SPD Data") },
#ifdef DEBUG_CODE_BLOCK
  { DIMMInfo,                     CHECKPOINT_MAJOR_DIMM_INFO,         CHECKPOINT_MINOR_DIMM_INFO_EARLY,                 MRC_PF_ALL,             MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Socket DIMM Information") },
#endif // DEBUG_CODE_BLOCK
  { EarlyConfig,                  CHECKPOINT_MAJOR_GLOBAL_EARLY,      0,                                                MRC_PF_ALL,             MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Early Configuration") },
  { InitInterface,                CHECKPOINT_MAJOR_DDRIO_INIT,        CHECKPOINT_MINOR_EARLY_INIT,                      MRC_PF_ALL,             MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("DDRIO Initialization") },
  { InitDdrioInterfaceLate,       CHECKPOINT_MAJOR_DDRIO_INIT,        CHECKPOINT_MINOR_LATE_INIT,                       MRC_PF_ALL,             MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("DDRIO Initialization Late") },
  { DdrTraining,                  CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MAJOR_NO_CODE,                         MRC_PF_ALL,             MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("DDR Training") },
  { DisplayTrainResultsDdr5,      CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_DISPLAY_RESULTS,                 MRC_PF_COLD,            MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Display Training Results") },
  { HbmTraining,                  CHECKPOINT_MAJOR_HBM,               CHECKPOINT_MINOR_HBM_TRAINING,                    MRC_PF_ALL,             MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("HBM Training") },
  { PostTrainingInit,             CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_POST_TRAINING_INIT,              MRC_PF_ALL,             MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Post-Training Initialization") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_AP_SSA_DATA,                MRC_PF_ALL,             MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync AP Pre SSA Data") },
  //HSD 1507151709
  { ExecuteSsaRmt,                CHECKPOINT_MAJOR_SSA_API_INIT,      CHECKPOINT_MINOR_SSA_API_INIT_START,              MRC_PF_ALL,             MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("BIOS SSA Initialization") },
  { OffsetTrainingResult,         CHECKPOINT_MAJOR_OFFSET_TRAIN_RESULT, 0,                                              MRC_PF_ALL,             MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("Offset training results") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_SBSP_SSA_DATA,              MRC_PF_ALL,             MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync SBSP Post SSA Data") },
  { HbmPostTraining,              CHECKPOINT_MAJOR_HBM,               CHECKPOINT_MINOR_HBM_POST_TRAINING,               MRC_PF_ALL,             MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("HBM Post-Training") },
  { LateConfig,                   CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_END_TRAINING,                    MRC_PF_ALL,             MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Late Configuration") },
  { InitThrottling,               CHECKPOINT_MAJOR_INIT_THROTTLING,   CHECKPOINT_MINOR_MID_DDR_THERM,                   MRC_PF_ALL,             MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Initialize Throttling") },
  { AdvancedMemTest,              CHECKPOINT_MAJOR_MEMBIST,           0,                                                MRC_PF_ALL,             MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("MemTest") },
  { SetupSVLandScrambling,        CHECKPOINT_MAJOR_SVL_SCRAMBLE,      0,                                                MRC_PF_ALL,             MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Setup SVL and Scrambling") },
  { DdrMemInit,                   CHECKPOINT_MAJOR_MEMINIT,           0,                                                MRC_PF_ALL,             MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("MemInit") },
  { CheckRASSupportAfterMemInit,  CHECKPOINT_MAJOR_CHECK_RAS,         0,                                                MRC_PF_ALL,             MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Check Ras Support After MemInit") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_AP_NVRAM_DATA,              MRC_PF_ALL,             MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync AP NVRAM Data") },
  { EnforcePopulationPor,         CHECKPOINT_MAJOR_NO_CODE,           0,                                                MRC_PF_ALL,             MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("Check Memory Topology") },
  { InitMemoryMap,                CHECKPOINT_MAJOR_DDR_MEMMAP,        0,                                                MRC_PF_ALL,             MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("Initialize Memory Map") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_SBSP_VAR_DATA,              MRC_PF_ALL,             MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync SBSP Variable Data") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_SBSP_MEM_MODE,              MRC_PF_ALL,             MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync SBSP Memory Mode") },
  { IsTmeCapSupported,            CHECKPOINT_MAJOR_MKTME_EARLY_FLOW,  CHECKPOINT_MINOR_TME_CHECK_CAP_ST,                MRC_PF_ALL,             MRC_MP_SERIAL,          CALL_TABLE_STRING("TME Capability check") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_AP_FINAL_ERROR_SYNC,        MRC_PF_ALL,             MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync AP Final Error Sync") },
  { MktmeEarlyFlow,               CHECKPOINT_MAJOR_MKTME_EARLY_FLOW,  CHECKPOINT_MINOR_MKTME_EARLY_FLOW,                MRC_PF_ALL,             MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("TME Flow") },
  { SgxPreMemInitEntryPoint,      CHECKPOINT_MAJOR_SGX_PREMEM,        CHECKPOINT_MINOR_SGX_CHECK_CAP_ST,                MRC_PF_ALL,             MRC_MP_SERIAL,          CALL_TABLE_STRING("SGX PreMem Check Capability ST") },
  { SgxPreMemInitEntryPoint,      CHECKPOINT_MAJOR_SGX_PREMEM,        CHECKPOINT_MINOR_SGX_CHECK_CAP_MT,                MRC_PF_ALL,             MRC_MP_PARALLEL,        CALL_TABLE_STRING("SGX PreMem Check Capability MT") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_AP_FINAL_ERROR_SYNC,        MRC_PF_ALL,             MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync AP Final Error Sync") },
  { SgxPreMemInitEntryPoint,      CHECKPOINT_MAJOR_SGX_PREMEM,        CHECKPOINT_MINOR_SGX_PREMEM,                      MRC_PF_ALL,             MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("SGX PreMem Init") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_AP_FINAL_ERROR_SYNC,        MRC_PF_ALL,             MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync AP Final Error Sync") },
  { HbmSwitchToNormalMode,        CHECKPOINT_MAJOR_HBM,               CHECKPOINT_MINOR_HBM_NORMAL_MODE,                 MRC_PF_ALL,             MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("HBM Normal Mode") },
  { SwitchToNormalMode,           CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_NORMAL_MODE,                     MRC_PF_ALL,             MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Switch to Normal Mode") },
  { InitCmiCreditProgramming,     CHECKPOINT_MAJOR_CMI_CREDIT,        CHECKPOINT_MINOR_CMI_CREDIT_INIT,                 MRC_PF_ALL,             MRC_MP_BOTH_LOOP,       CALL_TABLE_STRING("Init CMI Credit Programming") },
  { PipeSync,                     CHECKPOINT_MAJOR_PIPE_SYNC,         CHECKPOINT_MINOR_PIPE_AP_FINAL_ERROR_SYNC,        MRC_PF_ALL,             MRC_MP_PARALLEL,        CALL_TABLE_STRING("Pipe Sync AP Final Error Sync") },
  { InitADR,                      CHECKPOINT_MAJOR_INIT_ADR,          0,                                                MRC_PF_ALL,             MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("Initialize ADR") },
  { SetRASConfig,                 CHECKPOINT_MAJOR_RAS_CONFIG,        0,                                                MRC_PF_ALL,             MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("Set RAS Configuration") },
  { MemLate,                      CHECKPOINT_MAJOR_MEM_LATE,          0,                                                MRC_PF_ALL,             MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("Memory Late") },
//{InitSADInterfaceDataStruct,    CHECKPOINT_MAJOR_NO_CODE,           0,                                                MRC_PF_ALL,             MRC_MP_BOTH,            CALL_TABLE_STRING("Memory Late")},
  { PrintAllStats,                CHECKPOINT_MAJOR_NO_CODE,           0,                                                MRC_PF_ALL,             MRC_MP_BOTH,            CALL_TABLE_STRING("Print All Stats") },
  { PrintPerformanceSettings,     CHECKPOINT_MAJOR_NO_CODE,           0,                                                MRC_PF_ALL,             MRC_MP_BOTH,            CALL_TABLE_STRING("Print Performance Seetings") },
#ifdef DEBUG_CODE_BLOCK
  { DIMMInfo,                     CHECKPOINT_MAJOR_DIMM_INFO,         CHECKPOINT_MINOR_DIMM_INFO_LATE,                  MRC_PF_ALL,             MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("DIMM Information After MRC") },
#endif // DEBUG_CODE_BLOCK
  { DDRResetLoop,                 CHECKPOINT_MAJOR_NO_CODE,           0,                                                MRC_PF_ALL,             MRC_MP_BOTH_SBSP,       CALL_TABLE_STRING("DDR Reset Loop") },
};

UINT8 CallTableMainSize = sizeof(CallTableMain) / sizeof(CallTableEntryType);

CallTableEntryType CallTableDdrTraining[] = {
  { PreTrainingInit,                 CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_PRE_TRAINING_INIT,                  MRC_PF_ALL,             MRC_MP_BOTH,      CALL_TABLE_STRING("Pre-Training Initialization") },
  { EarlyCsClk,                      CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_EARLY_CS_CLK,                       MRC_PF_COLD,            MRC_MP_BOTH,      CALL_TABLE_STRING("Early CS/CLK Training") },
  { EarlyCaClk,                      CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_EARLY_CA_CLK,                       MRC_PF_COLD,            MRC_MP_BOTH,      CALL_TABLE_STRING("Early CA/CLK Training") },
  { CaSlewRate,                      CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_CA_SLEW_RATE,                       MRC_PF_COLD,            MRC_MP_BOTH,      CALL_TABLE_STRING("CA Slew Rate Training") },
  { TrainDcaTco,                     CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_DCA_TCO,                            MRC_PF_COLD,            MRC_MP_BOTH,      CALL_TABLE_STRING("DCA TCO Training") },
  { RcdDcaDckDutyCycle,              CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_RCD_DCA_DCK_DUTY_CYCLE_TRAINING,    MRC_PF_COLD,            MRC_MP_BOTH,      CALL_TABLE_STRING("RCD DCA/DCK Duty Cycle Training") },
  { DcaDfeDdr5,                      CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_DCA_DFE,                            MRC_PF_COLD,            MRC_MP_BOTH,      CALL_TABLE_STRING("DCA DFE Training") },
#ifdef LRDIMM_SUPPORT
  { BcomTraining,                    CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_EARLY_BCOM_CLK,                     MRC_PF_COLD,            MRC_MP_BOTH,      CALL_TABLE_STRING("Lrdimm Bcom Training") },
#endif
  { EarlyCsClkBackside,              CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_EARLY_CS_CLK_BACKSIDE,              MRC_PF_COLD,            MRC_MP_BOTH,      CALL_TABLE_STRING ("Early CS/CLK Backside Training") },
  { EarlyCaClkBackside,              CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_EARLY_CA_CLK_BACKSIDE,              MRC_PF_COLD,            MRC_MP_BOTH,      CALL_TABLE_STRING("Early CA/CLK Backside Training") },
#ifdef LRDIMM_SUPPORT
  { PbaEnumerateIdDdr5,              CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_PBA_ENUMERATE_ID,                   MRC_PF_COLD,            MRC_MP_BOTH,      CALL_TABLE_STRING("Lrdimm Pba Enumerate ID") },
  { EarlyReqClk,                     CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_REQ_CLK,                            MRC_PF_COLD,            MRC_MP_BOTH,      CALL_TABLE_STRING("Early Req Clk Training") },
  { Ddr5LrdimmMrePhaseCycleTraining, CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_RX_BACKSIDE_PHASE_TRAINING,         MRC_PF_COLD,            MRC_MP_BOTH,      CALL_TABLE_STRING("Lrdimm Backside Receive Enable Training(RX)") },
  { Ddr5LrdimmMrdReadDelayTraining,  CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_RX_BACKSIDE_DELAY_TRAINING,         MRC_PF_COLD,            MRC_MP_BOTH,      CALL_TABLE_STRING("Lrdimm Backside MDQS Read Delay Training(RX)") },
#endif
  { ReceiveEnableDdr5,               CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_RECEIVE_ENABLE_BASIC,               MRC_PF_COLD,            MRC_MP_BOTH,      CALL_TABLE_STRING("Receive Enable Ddr5") },
  { CoarseReadDqDqsPerBit,           CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_RX_DQ_DQS_BASIC,                    MRC_PF_COLD,            MRC_MP_BOTH,      CALL_TABLE_STRING("DDR5 Rx Dq/Dqs Basic Per Bit") },
  { DqSwizzleDiscoveryDdr5,          CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_DQ_SWIZZLE_DISCOVERY,               MRC_PF_COLD,            MRC_MP_BOTH,      CALL_TABLE_STRING("Dq Swizzle Discovery DDR5") },
  { ReadDqDqsPreDfe2DCentering,      CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_READ_DQDQS_PRE_DFE_2D_CENTERING,    MRC_PF_COLD ,           MRC_MP_BOTH,      CALL_TABLE_STRING("Read DQDQS Pre Dfe 2D Centering") },
  { DramDutyCycleAdjuster,           CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_DRAM_DUTY_CYCLE_ADJUSTER,           MRC_PF_COLD ,           MRC_MP_BOTH,      CALL_TABLE_STRING("Dram Duty Cycle Adjuster") },
  { RxDfeDdr5,                       CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_READ_DFE,                           MRC_PF_COLD,            MRC_MP_BOTH,      CALL_TABLE_STRING("DDR5 Read DFE") },
  { ReadDqDqsPostDfe2DCentering,     CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_READ_DQDQS_POST_DFE_2D_CENTERING,   MRC_PF_COLD ,           MRC_MP_BOTH,      CALL_TABLE_STRING("Read DQDQS Post Dfe 2D Centering") },
#ifdef DDRT_SUPPORT
  { SwitchToDdrt2Mode,               CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_DDRT2_MODE,                         MRC_PF_COLD,            MRC_MP_BOTH,      CALL_TABLE_STRING("Switch to DDRT2 Mode") },
#endif
#ifdef LRDIMM_SUPPORT
  { LrdimmBacksideWriteLeveling,     CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_LRDIMM_EXTERNAL_WRITE_LEVEL_COARSE, MRC_PF_COLD,            MRC_MP_BOTH,      CALL_TABLE_STRING("Lrdimm Backside Write Leveling Training") },
  { LrdimmBacksideWriteDelay,        CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_LRDIMM_EXTERNAL_WRITE_DELAY,        MRC_PF_COLD,            MRC_MP_BOTH,      CALL_TABLE_STRING("Lrdimm Backside Write Delay Training") },
#endif
  { WriteLevelingDdr5,               CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_WRITE_LEVELING_BASIC_DDR5,          MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,      CALL_TABLE_STRING("DDR5 Write Leveling") },
  { WriteDqDqsPerBitDdr5,            CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_TX_DQ_BASIC,                        MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,      CALL_TABLE_STRING("Tx Dq/Dqs Basic Per Bit") },
  { WriteDqDqsPreDfe2DCentering,     CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_WRITE_DQDQS_PRE_DFE_2D_CENTERING,   MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,      CALL_TABLE_STRING("Tx Dq/Dqs Pre Dfe 2D Centering") },
  { TxDqSlewRate,                    CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_DQ_SLEW_RATE,                       MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,      CALL_TABLE_STRING("Tx Dq Slew Rate") },
  { TrainTcoDqDqs,                   CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_TCO_DQ,                             MRC_PF_COLD,            MRC_MP_BOTH,      CALL_TABLE_STRING("TCO Dq/Dqs Training") },
  { TxDfeDdr5,                       CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_WRITE_DFE,                          MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,      CALL_TABLE_STRING("DDR5 Write DFE") },
#ifdef LRDIMM_SUPPORT
  { TxDbDfeDdr5,                     CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_WRITE_DB_DFE,                       MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,      CALL_TABLE_STRING("DDR5 Write DB DFE") },
#endif
  { WriteOdtLatencyTraining,         CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_WRITE_ODT_LATENCY,                  MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,      CALL_TABLE_STRING("Write ODT Latency") },
  { WriteDqDqsPostDfe2DCentering,    CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_WRITE_DQDQS_POST_DFE_2D_CENTERING,  MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,      CALL_TABLE_STRING("Tx Dq/Dqs Post Dfe 2D Centering") },
  { NormalizeCmd,                    CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_NORMALIZE_CMD,                      MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,      CALL_TABLE_STRING("Normalize CMD Training") },
  { PeriodicTxRetrain,               CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_PERIODIC_TX_RETRAIN,                MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,      CALL_TABLE_STRING("Tx Periodic Retraining") },
#if !defined (SPR_PO_FLAG) && !defined (CWV_A0_REMOVE_POST_PO)
  { PostPackageRepairMain,           CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_PPR_FLOW,                           MRC_PF_ALL|MRC_PF_RMT,  MRC_MP_BOTH,      CALL_TABLE_STRING("PPR Flow") },
#endif
  { RoundTripOptimize,               CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_ROUND_TRIP_OPTIMIZE,                MRC_PF_COLD|MRC_PF_RMT, MRC_MP_BOTH,      CALL_TABLE_STRING("Round Trip Optimization") },
  { TurnaroundTrain,                 CHECKPOINT_MAJOR_CHANNEL_TRAINING,  CHECKPOINT_MINOR_TURNAROUND,                         MRC_PF_COLD,            MRC_MP_BOTH,      CALL_TABLE_STRING("Turn Around Training") },
};

UINT8 CallTableDdrTrainingSize = sizeof(CallTableDdrTraining) / sizeof(CallTableEntryType);
