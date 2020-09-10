/** @file

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

#ifndef __MEMORY_EVCONTENT_LIB_H__
#define __MEMORY_EVCONTENT_LIB_H__

#include "SysHost.h"
#include <Ppi/SsaBiosServicesPpi.h>
#include <MaxSocket.h>
#include <Library/MemCpgcIpLib.h>

//
// Values of the size of some CPGC test configuration fields
//
#define NUMBER_UNISEQ_SUPPORT_LMN                  1
#define NUMBER_L_M_N_ENTRIES                       3
#define NUMBER_USER_DEFINED_DQDB_CACHELINES        2
#define NUMBER_USER_DEFINED_DQDB_CACHELINES_CPGC2  4
#define NUMBER_CHUNKS_PER_DQDB_CACHELINE           8
#define NUMBER_BYTE_PER_DQDB_CHUNK                 8
#define NUMBER_BYTE_PER_DQDB_CHUNK_CPGC2           2
#define NUMBER_BYTE_PER_CADB_ROW                   8
#define NUMBER_BYTE_OF_LANE_MASK                   (MAX_BITS / 8)
#define NUMBER_ERID_SIGNAL                         2

//
// CPGC configuration header version definition
//
#define CPGC1_MAJOR_VERSION                         1
#define CPGC2_MAJOR_VERSION                         2

#pragma pack (push, 1)

//
// Scrambler Modes
//

typedef enum {
  DisableScrambler              = 0,        // disable the scrambler during setup; restore it during cleanup
  EnableScrambler               = 1,        // enable the scrambler during setup; restore it during cleanup
  DontTouchScrambler            = 2,        // don't touch the scrambler value during setup and cleanup
  OverrideScramblerMax,                     // SCRAMBLER_OVERRIDE_MODE enumeration maximum value.
  ScrambleOverrideModeDelim = MAX_INT32     // This value ensures the enum size is consistent on both sides of the PPI.
} SCRAMBLER_OVERRIDE_MODE;

//
// Shmoo direction
//
typedef enum {
  LoDirection,                            // low side
  HiDirection,                            // high side
  HiThenLoDirection,                      // high then low side
  LoThenHiDirection,                      // low then high side
  ShmooDirectionMax,                      // SHMOO_DIRECTION enumeration maximum value.
  ShmooDirectionDelim = MAX_INT32         // SHMOO_DIRECTION enumeration delimiter value.
} SHMOO_DIRECTION;

//
// lane/rank/channel shmoo stop mode
//
typedef enum {
  AnyShmooStopMode,                       // stop once any lane/rank/channel fails
  AllShmooStopMode,                       // stop once all lanes/ranks/channels fail
  DoNoStopShmooStopMode,                  // do not stop
  ShmooStopModeMax,                       // SHMOO_STOP_MODE enumeration maximum value.
  ShmooStopModeDelim = MAX_INT32          // SHMOO_STOP_MODE enumeration delimiter value.
} SHMOO_STOP_MODE;

//
// Rank combinations
//
typedef enum {
  Singles,                                // Single rank selection.
  Turnarounds,                            // Turnaround ranks selection.
  RankTestModeMax,                        // RANK_TEST_MODE enumeration maximum value.
  RankTestModeDelim = MAX_INT32           // RANK_TEST_MODE enumeration delimiter value.
} RANK_TEST_MODE;

//
// Memory traffic mode.
//
typedef enum {
  LoopbackTrafficMode,                    // This traffic mode produces a simple burst of N cacheline writes followed by a burst on N cacheline reads where N is the corresponding BurstLength parameter value.
  WritesAndReadsTrafficMode,              // This traffic mode produces a repeating series of the following sequence of operations: four bursts of write operations followed by four bursts of read operations.
  WriteOnlyTrafficMode,                   // This traffic mode produces a series of bursts of write operations.
  ReadOnlyTrafficMode,                    // This traffic mode produces a repeating series of bursts of read operations.
  IdleTrafficMode,                        // No traffic will be generated for the given channel.
  Wr2WrAndRd2RdTrafficMode,               // This traffic mode produces write-to-write AND read-to-read rank-to-rank turn-around operations.
  Rd2WrTrafficMode,                       // This traffic mode produces read-to-write rank-to-rank turn-around operations.
  Wr2RdTrafficMode,                       // This traffic mode produces write-to-read rank-to-rank turn-around operations.
  TrafficModeMax,                         // TRAFFIC_MODE enumeration maximum value.
  TrafficModeDelim = MAX_INT32            // This values ensures that different environments use the same size for enum values.
} TRAFFIC_MODE;

//
// Memory addressing mode.
//
typedef enum {
  LinearAddrMode,                         // Within a burst, the addresses will increment linearly.
  ActiveAddrMode,                         // Within a burst, the addresses will increment non-linearly. The idea is to make all of the address signals wiggle.
  AddrModeMax,                            // ADDR_MODE structure maximum value.
  AddrModeDelim = MAX_INT32               // ADDR_MODE structure delimiter value.
} ADDR_MODE;

//
// CADB modes.
//
typedef enum {
  NormalCadbMode,                         // Normal CADB mode.
  OnDeselectCadbMode,                     // On deselect CADB mode.
  AlwaysOnCadbMode,                       // Always on CADB mode.
  CadbModeMax,                            // CADB mode selection structure maximum value.
  CadbModeDelim = MAX_INT32               // CADB mode selection structure delimiter value.
} CADB_MODE;

//
// DC_Polarity
//
typedef enum {
  DcLow,
  DcHigh,
  DcPolarityMax,                          // DC_POLARITY enumeration maximum value.
  DcPolarityDelim = MAX_INT32             // This values ensures that different environments use the same size for enum values.
} DC_POLARITY;

//
// Page open mode
//
typedef enum {
  DisablePageOpen = 0,                    // disable the page-open policy during setup; restore it during cleanup
  EnablePageOpen = 1,                     // enable the page-open policy during setup; restore it during cleanup
  DontTouchPageOpen = 2,                  // don't touch the page-open policy value during setup and cleanup
  OverridePageOpenMax,                    // PAGE_OPEN_OVERRIDE_MODE enumeration maximum value.
  PageOpenOverrideModeDelim = MAX_INT32
} PAGE_OPEN_OVERRIDE_MODE;

//
// CPGC1 configure structure. It is used by ICX/SNR/SKX/CPX
// Please refer to CPGC point test configuration table of the BSSA interface spec for field definition of the strcutre.
// https://sharepoint.amr.ith.intel.com/sites/ServerMRC/Shared%20Documents/Spec/BSSA/BSSA_design.html#cpgc-point-test-type-3
//
typedef struct {
  CPGC_CONFIGURE_HEADER Header;
  CPGC_STOP_MODE        CpgcStopMode;
  TRAFFIC_MODE          TrafficModes[MAX_IMC][MAX_MC_CH];
  ADDR_MODE             AddrModes[MAX_IMC][MAX_MC_CH];
  UINT16                BurstLengths[MAX_IMC][MAX_MC_CH];
  UINT32                LoopCounts[MAX_IMC][MAX_MC_CH];
  UINT32                BinarySearchLoopCounts[MAX_IMC][MAX_MC_CH];
  UINT16                StartDelays[MAX_IMC][MAX_MC_CH];
  UINT16                InterSubseqWaits[MAX_IMC][MAX_MC_CH];
  ECC_DATA_SOURCE       EccDataSource[MAX_MC_CH];
  DC_POLARITY           DcPolarity[MAX_MC_CH];
  UINT8                 DcBitmasks[MAX_MC_CH][NUMBER_BYTE_OF_LANE_MASK];
  BOOLEAN               EnableUserDefinedDqdbPatterns;
  UINT8                 UserDefinedDqdbPatternLengths[MAX_MC_CH];
  UINT8                 UserDefinedDqdbPatternRotationCnts[MAX_MC_CH];
  UINT8                 UserDefinedDqdbPatterns[MAX_MC_CH][NUMBER_USER_DEFINED_DQDB_CACHELINES][NUMBER_CHUNKS_PER_DQDB_CACHELINE][NUMBER_BYTE_PER_DQDB_CHUNK];
  DQDB_INC_RATE_MODE    DqdbIncRateModes[MAX_MC_CH];
  UINT8                 DqdbIncRates[MAX_MC_CH];
  DQDB_UNISEQ_MODE      DqdbUniseqModes[MAX_IMC][MAX_MC_CH][MAX_NUMBER_DQDB_UNISEQS];
  UINT32                DqdbUniseqSeeds[MAX_IMC][MAX_MC_CH][MAX_NUMBER_DQDB_UNISEQS];
  UINT16                DqdbLmnValues[MAX_IMC][MAX_MC_CH][NUMBER_UNISEQ_SUPPORT_LMN][NUMBER_L_M_N_ENTRIES];
  CADB_MODE             CadbModes[MAX_IMC][MAX_MC_CH];
  BOOLEAN               EnableUserDefinedCadbPatterns;
  UINT8                 UserDefinedCadbPatterns[MAX_MC_CH][MAX_NUMBER_CADB_ROWS][NUMBER_BYTE_PER_CADB_ROW];
  CADB_UNISEQ_MODE      CadbUniseqModes[MAX_IMC][MAX_MC_CH][MAX_NUMBER_CADB_UNISEQS];
  UINT32                CadbUniseqSeeds[MAX_IMC][MAX_MC_CH][MAX_NUMBER_CADB_UNISEQS];
  UINT16                CadbLmnValues[MAX_IMC][MAX_MC_CH][NUMBER_UNISEQ_SUPPORT_LMN][NUMBER_L_M_N_ENTRIES];
  BOOLEAN               EnableErid;
  UINT32                EridPatternSeeds[MAX_IMC][MAX_MC_CH][NUMBER_ERID_SIGNAL];
  BOOLEAN               EnableTimeout;
  UINT8                 LaneValBitmasks[MAX_IMC][MAX_MC_CH][NUMBER_BYTE_OF_LANE_MASK];
  UINT8                 ChunkValBitmasks[MAX_IMC][MAX_MC_CH];
} CPGC1_CONFIG;

//
// CPGC2.0 configure structure. It is used by SPR.
//
typedef struct {
  CPGC_CONFIGURE_HEADER Header;
  CPGC_STOP_MODE        CpgcStopMode;
  TRAFFIC_MODE          TrafficModes[MAX_IMC][MAX_MC_CH];
  ADDR_MODE             AddrModes[MAX_IMC][MAX_MC_CH];
  UINT16                BurstLengths[MAX_IMC][MAX_MC_CH];
  UINT32                LoopCounts[MAX_IMC][MAX_MC_CH];
  UINT32                BinarySearchLoopCounts[MAX_IMC][MAX_MC_CH];
  UINT16                StartDelays[MAX_IMC][MAX_MC_CH];
  UINT16                InterSubseqWaits[MAX_IMC][MAX_MC_CH];
  ECC_DATA_SOURCE       EccDataSource[MAX_MC_CH];
  DC_POLARITY           DcPolarity[MAX_MC_CH];
  UINT8                 DcBitmasks[MAX_MC_CH][NUMBER_BYTE_OF_LANE_MASK];
  BOOLEAN               EnableUserDefinedDqdbPatterns;
  UINT8                 UserDefinedDqdbPatternLengths[MAX_MC_CH];
  UINT8                 UserDefinedDqdbPatterns[MAX_MC_CH][NUMBER_USER_DEFINED_DQDB_CACHELINES_CPGC2][NUMBER_CHUNKS_PER_DQDB_CACHELINE][NUMBER_BYTE_PER_DQDB_CHUNK_CPGC2];
  DQDB_INC_RATE_MODE    DqdbIncRateModes[MAX_MC_CH];
  UINT8                 DqdbIncRates[MAX_MC_CH];
  UINT8                 DqdbLaneRotationRates[MAX_MC_CH];
  DQDB_UNISEQ_MODE      DqdbUniseqModes[MAX_IMC][MAX_MC_CH][MAX_NUMBER_DQDB_UNISEQS];
  UINT32                DqdbUniseqSeeds[MAX_IMC][MAX_MC_CH][MAX_NUMBER_DQDB_UNISEQS];
  UINT16                DqdbLmnValues[MAX_IMC][MAX_MC_CH][NUMBER_UNISEQ_SUPPORT_LMN][NUMBER_L_M_N_ENTRIES];
  CADB_MODE             CadbModes[MAX_IMC][MAX_MC_CH];
  UINT8                 LaneValBitmasks[MAX_IMC][MAX_MC_CH][NUMBER_BYTE_OF_LANE_MASK];
  UINT16                ChunkValBitmasks[MAX_IMC][MAX_MC_CH];
} CPGC2_CONFIG;

//
// RMT test configuration.
//
typedef struct {
  UINT32                  SocketBitmask;
  UINT8                   ControllerBitmask;
  UINT8                   IoLevelBitmask;
  BOOLEAN                 EnableRankMargin;
  BOOLEAN                 EnableCtlAllMargin;
  BOOLEAN                 EnableEridMargin;
  BOOLEAN                 EnableLaneMargin;
  BOOLEAN                 EnableTurnaroundMargin;
  UINT8                   RxDqsStepSize;
  UINT8                   TxDqStepSize;
  UINT8                   RxVrefStepSize;
  UINT8                   TxVrefStepSize;
  UINT8                   CmdAllStepSize;
  UINT8                   CmdVrefStepSize;
  UINT8                   CtlAllStepSize;
  UINT8                   EridDelayStepSize;
  UINT8                   EridVrefStepSize;
  UINT16                  SinglesBurstLength;
  UINT32                  SinglesLoopCount;
  UINT32                  SinglesBinarySearchLoopCount;
  UINT16                  TurnaroundsBurstLength;
  UINT32                  TurnaroundsLoopCount;
  UINT32                  TurnaroundsBinarySearchLoopCount;
  UINT16                  OptimizedShmooBitmask;
  SCRAMBLER_OVERRIDE_MODE ScramblerOverrideMode;
  BOOLEAN                 EnableExtendedRange;
  BOOLEAN                 EnableTestResultLogging;
  BOOLEAN                 EnableLaneMarginPlot;
  UINT8                   TestStatusLogLevel;
  BOOLEAN                 EnableChannelSpecificLoopCountsBurstlengths;
  INT16                   BurstLengths[MAX_MC_CH];
  UINT32                  LoopCounts[MAX_MC_CH];
} RMT_CONFIG;

//
// CPGC1.0 version Margin1D test configuration. It contains CPGC 1.0 specific configurature.
// It contains 4 sections: Margin Parameter, Shmoo/Margin control, Target and checker selection, CPGC configuration
//
typedef struct {
  MRC_LT                  IoLevel;
  MRC_GT                  MarginGroup;
  SHMOO_DIRECTION         ShmooDirection;
  UINT8                   StepSize;
  UINT8                   GuardBandSize;
  SHMOO_STOP_MODE         LaneShmooStopMode;
  SHMOO_STOP_MODE         RankShmooStopMode;
  SHMOO_STOP_MODE         ChannelShmooStopMode;
  SHMOO_STOP_MODE         ControllerShmooStopMode;
  SHMOO_STOP_MODE         SocketShmooStopMode;
  BOOLEAN                 EnableOptimizedShmoo;
  UINT32                  SocketBitmask;
  UINT8                   ControllerBitmask;
  UINT8                   ChannelBitmasks[MAX_IMC];
  RANK_TEST_MODE          RankTestModes[MAX_IMC][MAX_MC_CH];
  UINT8                   DimmABitmasks[MAX_IMC][MAX_MC_CH];
  UINT8                   RankABitmasks[MAX_IMC][MAX_MC_CH][MAX_DIMM];
  UINT8                   DimmBBitmasks[MAX_IMC][MAX_MC_CH];
  UINT8                   RankBBitmasks[MAX_IMC][MAX_MC_CH][MAX_DIMM];
  UINT8                   LaneValBitmasks[MAX_IMC][MAX_MC_CH][NUMBER_BYTE_OF_LANE_MASK];
  UINT8                   ChunkValBitmasks[MAX_IMC][MAX_MC_CH];
  CPGC_STOP_MODE          CpgcStopMode;
  TRAFFIC_MODE            TrafficModes[MAX_IMC][MAX_MC_CH];
  ADDR_MODE               AddrModes[MAX_IMC][MAX_MC_CH];
  UINT16                  BurstLengths[MAX_IMC][MAX_MC_CH];
  UINT32                  LoopCounts[MAX_IMC][MAX_MC_CH];
  UINT32                  BinarySearchLoopCounts[MAX_IMC][MAX_MC_CH];
  UINT16                  StartDelays[MAX_IMC][MAX_MC_CH];
  UINT16                  InterSubseqWaits[MAX_IMC][MAX_MC_CH];
  ECC_DATA_SOURCE         EccDataSource[MAX_MC_CH];
  DC_POLARITY             DcPolarity[MAX_MC_CH];
  UINT8                   DcBitmasks[MAX_MC_CH][NUMBER_BYTE_OF_LANE_MASK];
  BOOLEAN                 EnableUserDefinedDqdbPatterns;
  UINT8                   UserDefinedDqdbPatternLengths[MAX_MC_CH];
  UINT8                   UserDefinedDqdbPatternRotationCnts[MAX_MC_CH];
  UINT8                   UserDefinedDqdbPatterns[MAX_MC_CH][NUMBER_USER_DEFINED_DQDB_CACHELINES][NUMBER_CHUNKS_PER_DQDB_CACHELINE][NUMBER_BYTE_PER_DQDB_CHUNK];
  DQDB_INC_RATE_MODE      DqdbIncRateModes[MAX_MC_CH];
  UINT8                   DqdbIncRates[MAX_MC_CH];
  DQDB_UNISEQ_MODE        DqdbUniseqModes[MAX_IMC][MAX_MC_CH][MAX_NUMBER_DQDB_UNISEQS];
  UINT32                  DqdbUniseqSeeds[MAX_IMC][MAX_MC_CH][MAX_NUMBER_DQDB_UNISEQS];
  UINT16                  DqdbLmnValues[MAX_IMC][MAX_MC_CH][NUMBER_UNISEQ_SUPPORT_LMN][NUMBER_L_M_N_ENTRIES];
  CADB_MODE               CadbModes[MAX_IMC][MAX_MC_CH];
  BOOLEAN                 EnableUserDefinedCadbPatterns;
  UINT8                   UserDefinedCadbPatterns[MAX_MC_CH][MAX_NUMBER_CADB_ROWS][NUMBER_BYTE_PER_CADB_ROW];
  CADB_UNISEQ_MODE        CadbUniseqModes[MAX_IMC][MAX_MC_CH][MAX_NUMBER_CADB_UNISEQS];
  UINT32                  CadbUniseqSeeds[MAX_IMC][MAX_MC_CH][MAX_NUMBER_CADB_UNISEQS];
  UINT16                  CadbLmnValues[MAX_IMC][MAX_MC_CH][NUMBER_UNISEQ_SUPPORT_LMN][NUMBER_L_M_N_ENTRIES];
  UINT32                  EridPatternSeeds[MAX_IMC][MAX_MC_CH][MAX_NO_ERID_SIG];
  SCRAMBLER_OVERRIDE_MODE ScramblerOverrideMode;
  BOOLEAN                 EnableExtendedRange;
  BOOLEAN                 EnableMarginPlot;
  UINT8                   TestStatusLogLevel;
} MARGIN_1D_CONFIG_CPGC1;

//
// CPGC1.0 version Margin2D test configuration. It contains CPGC 1.0 specific configurature.
// It contains 4 sections: Margin Parameters, Shmoo/Margin control, Target and checker selection, CPGC configuration
//
typedef struct {
  MRC_LT                  IoLevel;
  MRC_GT                  SlowMarginGroup;
  MRC_GT                  FastMarginGroup;
  UINT8                   SlowStepSize;
  UINT8                   FastStepSize;
  UINT8                   GuardBandSize;
  UINT8                   TargetSocket;
  UINT8                   TargetController;
  UINT8                   TargetChannel;
  UINT8                   TargetDimmA;
  UINT8                   TargetRankA;
  UINT8                   TargetDimmB;
  UINT8                   TargetRankB;
  UINT8                   LaneValBitmasks[NUMBER_BYTE_OF_LANE_MASK];
  UINT8                   ChunkValBitmask;
  UINT32                  SocketBitmask;
  UINT8                   ControllerBitmask;
  UINT8                   ChannelBitmasks[MAX_IMC];
  RANK_TEST_MODE          RankTestModes[MAX_IMC][MAX_MC_CH];
  UINT8                   DimmABitmasks[MAX_IMC][MAX_MC_CH];
  UINT8                   RankABitmasks[MAX_IMC][MAX_MC_CH][MAX_DIMM];
  UINT8                   DimmBBitmasks[MAX_IMC][MAX_MC_CH];
  UINT8                   RankBBitmasks[MAX_IMC][MAX_MC_CH][MAX_DIMM];
  CPGC_STOP_MODE          CpgcStopMode;
  TRAFFIC_MODE            TrafficModes[MAX_IMC][MAX_MC_CH];
  ADDR_MODE               AddrModes[MAX_IMC][MAX_MC_CH];
  UINT16                  BurstLengths[MAX_IMC][MAX_MC_CH];
  UINT32                  LoopCounts[MAX_IMC][MAX_MC_CH];
  UINT32                  BinarySearchLoopCounts[MAX_IMC][MAX_MC_CH];
  UINT16                  InterSubseqWaits[MAX_IMC][MAX_MC_CH];
  ECC_DATA_SOURCE         EccDataSource[MAX_MC_CH];
  DC_POLARITY             DcPolarity[MAX_MC_CH];
  UINT8                   DcBitmasks[MAX_MC_CH][NUMBER_BYTE_OF_LANE_MASK];
  BOOLEAN                 EnableUserDefinedDqdbPatterns;
  UINT8                   UserDefinedDqdbPatternLengths[MAX_MC_CH];
  UINT8                   UserDefinedDqdbPatternRotationCnts[MAX_MC_CH];
  UINT8                   UserDefinedDqdbPatterns[MAX_MC_CH][NUMBER_USER_DEFINED_DQDB_CACHELINES][NUMBER_CHUNKS_PER_DQDB_CACHELINE][NUMBER_BYTE_PER_DQDB_CHUNK];
  DQDB_INC_RATE_MODE      DqdbIncRateModes[MAX_MC_CH];
  UINT8                   DqdbIncRates[MAX_MC_CH];
  DQDB_UNISEQ_MODE        DqdbUniseqModes[MAX_IMC][MAX_MC_CH][MAX_NUMBER_DQDB_UNISEQS];
  UINT32                  DqdbUniseqSeeds[MAX_IMC][MAX_MC_CH][MAX_NUMBER_DQDB_UNISEQS];
  UINT16                  DqdbLmnValues[MAX_IMC][MAX_MC_CH][NUMBER_UNISEQ_SUPPORT_LMN][NUMBER_L_M_N_ENTRIES];
  CADB_MODE               CadbModes[MAX_IMC][MAX_MC_CH];
  BOOLEAN                 EnableUserDefinedCadbPatterns;
  UINT8                   UserDefinedCadbPatterns[MAX_MC_CH][MAX_NUMBER_CADB_ROWS][NUMBER_BYTE_PER_CADB_ROW];
  CADB_UNISEQ_MODE        CadbUniseqModes[MAX_IMC][MAX_MC_CH][MAX_NUMBER_CADB_UNISEQS];
  UINT32                  CadbUniseqSeeds[MAX_IMC][MAX_MC_CH][MAX_NUMBER_CADB_UNISEQS];
  UINT16                  CadbLmnValues[MAX_IMC][MAX_MC_CH][NUMBER_UNISEQ_SUPPORT_LMN][NUMBER_L_M_N_ENTRIES];
  SCRAMBLER_OVERRIDE_MODE ScramblerOverrideMode;
  BOOLEAN                 EnableMarginPlot;
  UINT8                   TestStatusLogLevel;
} MARGIN_2D_CONFIG_CPGC1;

//
// CPGC1.0 version RunCpgcPointTest test configuration. It contains CPGC 1.0 specific configurature.
// It contains 2 sections: Target and checker selection, CPGC configuration
//
typedef struct {
  MRC_LT                  IoLevel;
  UINT8                   RankIterationIndex;
  UINT32                  SocketBitmask;
  UINT8                   ControllerBitmasks[MAX_SOCKET];
  UINT8                   ChannelBitmasks[MAX_SOCKET][MAX_IMC];
  RANK_TEST_MODE          RankTestModes[MAX_SOCKET][MAX_IMC][MAX_MC_CH];
  UINT8                   DimmABitmasks[MAX_SOCKET][MAX_IMC][MAX_MC_CH];
  UINT8                   RankABitmasks[MAX_SOCKET][MAX_IMC][MAX_MC_CH][MAX_DIMM];
  UINT8                   DimmBBitmasks[MAX_SOCKET][MAX_IMC][MAX_MC_CH];
  UINT8                   RankBBitmasks[MAX_SOCKET][MAX_IMC][MAX_MC_CH][MAX_DIMM];
  UINT8                   LaneValBitmasks[MAX_IMC][MAX_MC_CH][NUMBER_BYTE_OF_LANE_MASK];
  UINT8                   ChunkValBitmasks[MAX_IMC][MAX_MC_CH];
  CPGC_STOP_MODE          CpgcStopMode;
  TRAFFIC_MODE            TrafficModes[MAX_IMC][MAX_MC_CH];
  ADDR_MODE               AddrModes[MAX_IMC][MAX_MC_CH];
  UINT16                  BurstLengths[MAX_IMC][MAX_MC_CH];
  UINT32                  LoopCounts[MAX_IMC][MAX_MC_CH];
  UINT16                  StartDelays[MAX_IMC][MAX_MC_CH];
  UINT16                  InterSubseqWaits[MAX_IMC][MAX_MC_CH];
  ECC_DATA_SOURCE         EccDataSource[MAX_MC_CH];
  DC_POLARITY             DcPolarity[MAX_MC_CH];
  UINT8                   DcBitmasks[MAX_MC_CH][NUMBER_BYTE_OF_LANE_MASK];
  BOOLEAN                 EnableUserDefinedDqdbPatterns;
  UINT8                   UserDefinedDqdbPatternLengths[MAX_MC_CH];
  UINT8                   UserDefinedDqdbPatternRotationCnts[MAX_MC_CH];
  UINT8                   UserDefinedDqdbPatterns[MAX_MC_CH][NUMBER_USER_DEFINED_DQDB_CACHELINES][NUMBER_CHUNKS_PER_DQDB_CACHELINE][NUMBER_BYTE_PER_DQDB_CHUNK];
  DQDB_INC_RATE_MODE      DqdbIncRateModes[MAX_MC_CH];
  UINT8                   DqdbIncRates[MAX_MC_CH];
  DQDB_UNISEQ_MODE        DqdbUniseqModes[MAX_IMC][MAX_MC_CH][MAX_NUMBER_DQDB_UNISEQS];
  UINT32                  DqdbUniseqSeeds[MAX_IMC][MAX_MC_CH][MAX_NUMBER_DQDB_UNISEQS];
  UINT16                  DqdbLmnValues[MAX_IMC][MAX_MC_CH][NUMBER_UNISEQ_SUPPORT_LMN][NUMBER_L_M_N_ENTRIES];
  CADB_MODE               CadbModes[MAX_IMC][MAX_MC_CH];
  BOOLEAN                 EnableUserDefinedCadbPatterns;
  UINT8                   UserDefinedCadbPatterns[MAX_MC_CH][MAX_NUMBER_CADB_ROWS][NUMBER_BYTE_PER_CADB_ROW];
  CADB_UNISEQ_MODE        CadbUniseqModes[MAX_IMC][MAX_MC_CH][MAX_NUMBER_CADB_UNISEQS];
  UINT32                  CadbUniseqSeeds[MAX_IMC][MAX_MC_CH][MAX_NUMBER_CADB_UNISEQS];
  UINT16                  CadbLmnValues[MAX_IMC][MAX_MC_CH][NUMBER_UNISEQ_SUPPORT_LMN][NUMBER_L_M_N_ENTRIES];
  BOOLEAN                 EnableErid;
  UINT32                  EridPatternSeeds[MAX_IMC][MAX_MC_CH][MAX_NO_ERID_SIG];
  PAGE_OPEN_OVERRIDE_MODE PageOpenOverrideMode;
  SCRAMBLER_OVERRIDE_MODE ScramblerOverrideMode;
  UINT8                   TestStatusLogLevel;
} RUN_CPGC_POINT_TEST_CONFIG_CPGC1;

//
//
// CPGC2.0 version Margin1D test configuration. It contains CPGC 2.0 specific configurature.
// It contains 4 sections: Margin Parameter, Shmoo/Margin control, Target and checker selection, CPGC configuration
//
typedef struct {
  MRC_LT                  IoLevel;
  MRC_GT                  MarginGroup;
  SHMOO_DIRECTION         ShmooDirection;
  UINT8                   StepSize;
  UINT8                   GuardBandSize;
  SHMOO_STOP_MODE         LaneShmooStopMode;
  SHMOO_STOP_MODE         RankShmooStopMode;
  SHMOO_STOP_MODE         ChannelShmooStopMode;
  SHMOO_STOP_MODE         ControllerShmooStopMode;
  SHMOO_STOP_MODE         SocketShmooStopMode;
  BOOLEAN                 EnableOptimizedShmoo;
  UINT32                  SocketBitmask;
  UINT8                   ControllerBitmask;
  UINT8                   ChannelBitmasks[MAX_IMC];
  RANK_TEST_MODE          RankTestModes[MAX_IMC][MAX_MC_CH];
  UINT8                   DimmABitmasks[MAX_IMC][MAX_MC_CH];
  UINT8                   RankABitmasks[MAX_IMC][MAX_MC_CH][MAX_DIMM];
  UINT8                   DimmBBitmasks[MAX_IMC][MAX_MC_CH];
  UINT8                   RankBBitmasks[MAX_IMC][MAX_MC_CH][MAX_DIMM];
  UINT8                   LaneValBitmasks[MAX_IMC][MAX_MC_CH][NUMBER_BYTE_OF_LANE_MASK];
  UINT16                  ChunkValBitmasks[MAX_IMC][MAX_MC_CH];
  CPGC_STOP_MODE          CpgcStopMode;
  TRAFFIC_MODE            TrafficModes[MAX_IMC][MAX_MC_CH];
  ADDR_MODE               AddrModes[MAX_IMC][MAX_MC_CH];
  UINT16                  BurstLengths[MAX_IMC][MAX_MC_CH];
  UINT32                  LoopCounts[MAX_IMC][MAX_MC_CH];
  UINT32                  BinarySearchLoopCounts[MAX_IMC][MAX_MC_CH];
  UINT16                  StartDelays[MAX_IMC][MAX_MC_CH];
  UINT16                  InterSubseqWaits[MAX_IMC][MAX_MC_CH];
  ECC_DATA_SOURCE         EccDataSource[MAX_MC_CH];
  DC_POLARITY             DcPolarity[MAX_MC_CH];
  UINT8                   DcBitmasks[MAX_MC_CH][NUMBER_BYTE_OF_LANE_MASK];
  BOOLEAN                 EnableUserDefinedDqdbPatterns;
  UINT8                   UserDefinedDqdbPatternLengths[MAX_MC_CH];
  UINT8                   UserDefinedDqdbPatterns[MAX_MC_CH][NUMBER_USER_DEFINED_DQDB_CACHELINES_CPGC2][NUMBER_CHUNKS_PER_DQDB_CACHELINE][NUMBER_BYTE_PER_DQDB_CHUNK_CPGC2];
  DQDB_INC_RATE_MODE      DqdbIncRateModes[MAX_MC_CH];
  UINT8                   DqdbIncRates[MAX_MC_CH];
  UINT8                   DqdbLaneRotationRates[MAX_MC_CH];
  DQDB_UNISEQ_MODE        DqdbUniseqModes[MAX_IMC][MAX_MC_CH][MAX_NUMBER_DQDB_UNISEQS];
  UINT32                  DqdbUniseqSeeds[MAX_IMC][MAX_MC_CH][MAX_NUMBER_DQDB_UNISEQS];
  UINT16                  DqdbLmnValues[MAX_IMC][MAX_MC_CH][NUMBER_UNISEQ_SUPPORT_LMN][NUMBER_L_M_N_ENTRIES];
  CADB_MODE               CadbModes[MAX_IMC][MAX_MC_CH];
  SCRAMBLER_OVERRIDE_MODE ScramblerOverrideMode;
  BOOLEAN                 EnableExtendedRange;
  BOOLEAN                 EnableMarginPlot;
  UINT8                   TestStatusLogLevel;
} MARGIN_1D_CONFIG_CPGC2;

//
// CPGC2.0 version Margin2D test configuration. It contains CPGC 2.0 specific configurature.
// It contains 4 sections: Margin Parameters, Shmoo/Margin control, Target and checker selection, CPGC configuration
//
typedef struct {
  MRC_LT                  IoLevel;
  MRC_GT                  SlowMarginGroup;
  MRC_GT                  FastMarginGroup;
  UINT8                   SlowStepSize;
  UINT8                   FastStepSize;
  UINT8                   GuardBandSize;
  UINT8                   TargetSocket;
  UINT8                   TargetController;
  UINT8                   TargetChannel;
  UINT8                   TargetDimmA;
  UINT8                   TargetRankA;
  UINT8                   TargetDimmB;
  UINT8                   TargetRankB;
  UINT8                   LaneValBitmasks[NUMBER_BYTE_OF_LANE_MASK];
  UINT16                  ChunkValBitmask;
  UINT32                  SocketBitmask;
  UINT8                   ControllerBitmask;
  UINT8                   ChannelBitmasks[MAX_IMC];
  RANK_TEST_MODE          RankTestModes[MAX_IMC][MAX_MC_CH];
  UINT8                   DimmABitmasks[MAX_IMC][MAX_MC_CH];
  UINT8                   RankABitmasks[MAX_IMC][MAX_MC_CH][MAX_DIMM];
  UINT8                   DimmBBitmasks[MAX_IMC][MAX_MC_CH];
  UINT8                   RankBBitmasks[MAX_IMC][MAX_MC_CH][MAX_DIMM];
  CPGC_STOP_MODE          CpgcStopMode;
  TRAFFIC_MODE            TrafficModes[MAX_IMC][MAX_MC_CH];
  ADDR_MODE               AddrModes[MAX_IMC][MAX_MC_CH];
  UINT16                  BurstLengths[MAX_IMC][MAX_MC_CH];
  UINT32                  LoopCounts[MAX_IMC][MAX_MC_CH];
  UINT32                  BinarySearchLoopCounts[MAX_IMC][MAX_MC_CH];
  UINT16                  InterSubseqWaits[MAX_IMC][MAX_MC_CH];
  ECC_DATA_SOURCE         EccDataSource[MAX_MC_CH];
  DC_POLARITY             DcPolarity[MAX_MC_CH];
  UINT8                   DcBitmasks[MAX_MC_CH][NUMBER_BYTE_OF_LANE_MASK];
  BOOLEAN                 EnableUserDefinedDqdbPatterns;
  UINT8                   UserDefinedDqdbPatternLengths[MAX_MC_CH];
  UINT8                   UserDefinedDqdbPatterns[MAX_MC_CH][NUMBER_USER_DEFINED_DQDB_CACHELINES_CPGC2][NUMBER_CHUNKS_PER_DQDB_CACHELINE][NUMBER_BYTE_PER_DQDB_CHUNK_CPGC2];
  DQDB_INC_RATE_MODE      DqdbIncRateModes[MAX_MC_CH];
  UINT8                   DqdbIncRates[MAX_MC_CH];
  UINT8                   DqdbLaneRotationRates[MAX_MC_CH];
  DQDB_UNISEQ_MODE        DqdbUniseqModes[MAX_IMC][MAX_MC_CH][MAX_NUMBER_DQDB_UNISEQS];
  UINT32                  DqdbUniseqSeeds[MAX_IMC][MAX_MC_CH][MAX_NUMBER_DQDB_UNISEQS];
  UINT16                  DqdbLmnValues[MAX_IMC][MAX_MC_CH][NUMBER_UNISEQ_SUPPORT_LMN][NUMBER_L_M_N_ENTRIES];
  CADB_MODE               CadbModes[MAX_IMC][MAX_MC_CH];
  SCRAMBLER_OVERRIDE_MODE ScramblerOverrideMode;
  BOOLEAN                 EnableMarginPlot;
  UINT8                   TestStatusLogLevel;
} MARGIN_2D_CONFIG_CPGC2;

//
// CPGC2.0 version RunCpgcPointTest test configuration. It contains CPGC 2.0 specific configurature.
// It contains 2 sections: Target and checker selection, CPGC configuration
//
typedef struct {
  MRC_LT                  IoLevel;
  UINT8                   RankIterationIndex;
  UINT32                  SocketBitmask;
  UINT8                   ControllerBitmasks[MAX_SOCKET];
  UINT8                   ChannelBitmasks[MAX_SOCKET][MAX_IMC];
  RANK_TEST_MODE          RankTestModes[MAX_SOCKET][MAX_IMC][MAX_MC_CH];
  UINT8                   DimmABitmasks[MAX_SOCKET][MAX_IMC][MAX_MC_CH];
  UINT8                   RankABitmasks[MAX_SOCKET][MAX_IMC][MAX_MC_CH][MAX_DIMM];
  UINT8                   DimmBBitmasks[MAX_SOCKET][MAX_IMC][MAX_MC_CH];
  UINT8                   RankBBitmasks[MAX_SOCKET][MAX_IMC][MAX_MC_CH][MAX_DIMM];
  UINT8                   LaneValBitmasks[MAX_IMC][MAX_MC_CH][NUMBER_BYTE_OF_LANE_MASK];
  UINT16                  ChunkValBitmasks[MAX_IMC][MAX_MC_CH];
  CPGC_STOP_MODE          CpgcStopMode;
  TRAFFIC_MODE            TrafficModes[MAX_IMC][MAX_MC_CH];
  ADDR_MODE               AddrModes[MAX_IMC][MAX_MC_CH];
  UINT16                  BurstLengths[MAX_IMC][MAX_MC_CH];
  UINT32                  LoopCounts[MAX_IMC][MAX_MC_CH];
  UINT16                  StartDelays[MAX_IMC][MAX_MC_CH];
  UINT16                  InterSubseqWaits[MAX_IMC][MAX_MC_CH];
  ECC_DATA_SOURCE         EccDataSource[MAX_MC_CH];
  DC_POLARITY             DcPolarity[MAX_MC_CH];
  UINT8                   DcBitmasks[MAX_MC_CH][NUMBER_BYTE_OF_LANE_MASK];
  BOOLEAN                 EnableUserDefinedDqdbPatterns;
  UINT8                   UserDefinedDqdbPatternLengths[MAX_MC_CH];
  UINT8                   UserDefinedDqdbPatterns[MAX_MC_CH][NUMBER_USER_DEFINED_DQDB_CACHELINES_CPGC2][NUMBER_CHUNKS_PER_DQDB_CACHELINE][NUMBER_BYTE_PER_DQDB_CHUNK_CPGC2];
  DQDB_INC_RATE_MODE      DqdbIncRateModes[MAX_MC_CH];
  UINT8                   DqdbIncRates[MAX_MC_CH];
  UINT8                   DqdbLaneRotationRates[MAX_MC_CH];
  DQDB_UNISEQ_MODE        DqdbUniseqModes[MAX_IMC][MAX_MC_CH][MAX_NUMBER_DQDB_UNISEQS];
  UINT32                  DqdbUniseqSeeds[MAX_IMC][MAX_MC_CH][MAX_NUMBER_DQDB_UNISEQS];
  UINT16                  DqdbLmnValues[MAX_IMC][MAX_MC_CH][NUMBER_UNISEQ_SUPPORT_LMN][NUMBER_L_M_N_ENTRIES];
  CADB_MODE               CadbModes[MAX_IMC][MAX_MC_CH];
  PAGE_OPEN_OVERRIDE_MODE PageOpenOverrideMode;
  SCRAMBLER_OVERRIDE_MODE ScramblerOverrideMode;
  UINT8                   TestStatusLogLevel;
} RUN_CPGC_POINT_TEST_CONFIG_CPGC2;

#if FixedPcdGet8 (PcdSsaCpgcConfigVersion) == 1
  typedef CPGC1_CONFIG                     CPGC_POINT_TEST_CONFIG;
  typedef MARGIN_1D_CONFIG_CPGC1           MARGIN_1D_CONFIG;
  typedef MARGIN_2D_CONFIG_CPGC1           MARGIN_2D_CONFIG;
  typedef RUN_CPGC_POINT_TEST_CONFIG_CPGC1 RUN_CPGC_POINT_TEST_CONFIG;
#else
  typedef CPGC2_CONFIG                     CPGC_POINT_TEST_CONFIG;
  typedef MARGIN_1D_CONFIG_CPGC2           MARGIN_1D_CONFIG;
  typedef MARGIN_2D_CONFIG_CPGC2           MARGIN_2D_CONFIG;
  typedef RUN_CPGC_POINT_TEST_CONFIG_CPGC2 RUN_CPGC_POINT_TEST_CONFIG;
#endif

//
// SetMarginParam test configuration.
//
typedef struct {
  UINT32 SocketBitMask;
  UINT8 ControllerBitMask;
  UINT8 ChannelBitMask;
  UINT8 DimmBitMask;
  UINT8 RankBitMask;
  UINT8 BusWith;
  UINT8 LaneBitMask[NUMBER_BYTE_OF_LANE_MASK];
  UINT8 ParameterCnt;
  MRC_LT IoLevel;
  MRC_GT MarginGroup;
  INT16  Offset;
} SET_MARGIN_PARAM_CONFIG;

//
// MemBootHealth test configuration.
//
#define    SIGNAL_GROUP_TO_TEST     FixedPcdGet8 (PcdMemBootSignalsToTest)

typedef struct {
  MRC_GT MarginGroups[SIGNAL_GROUP_TO_TEST];
  INT16 WarningMask[SIGNAL_GROUP_TO_TEST * 2][SIGNAL_GROUP_TO_TEST];
  INT16 CriticalMask[SIGNAL_GROUP_TO_TEST * 2][SIGNAL_GROUP_TO_TEST];
  UINT32 ChannelsFailedCriticalMask[MAX_SOCKET];
  UINT8 TestStatusLogLevel;
  BOOLEAN CriticalTestStatus;
} BOOT_RAS_CONFIG;

#pragma pack (pop)

/**
  This function is the RMT test entry point.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       Config             Pointer to test configuration structure.

  @retval EFI_SUCCESS
          EFI_UNSUPPORTED
**/
EFI_STATUS
EFIAPI
RmtTestEntryPoint (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN RMT_CONFIG *Config
  );

/**
  This function is the Margin1D test entry point.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       Config             Pointer to test configuration structure.

  @retval EFI_SUCCESS
          EFI_UNSUPPORTED
**/
EFI_STATUS
EFIAPI
Margin1DTestEntryPoint (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN MARGIN_1D_CONFIG *Config
  );

/**
  @brief
  This function is the Margin2D test entry point.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       Config             Pointer to test configuration structure.

  @retval EFI_SUCCESS
          EFI_UNSUPPORTED
**/
EFI_STATUS
EFIAPI
Margin2DTestEntryPoint (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN MARGIN_2D_CONFIG *Config
  );


/**
  This function is the Run CPGC Point Test test entry point.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       Config             Pointer to test configuration structure.

  @retval EFI_SUCCESS
          EFI_UNSUPPORTED
**/
EFI_STATUS
EFIAPI
RunCpgcPointTestEntryPoint (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN RUN_CPGC_POINT_TEST_CONFIG *Config
  );

/**
  This function is the Set Margin Param test entry point.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       Config             Pointer to test configuration structure.

  @retval EFI_SUCCESS
          EFI_UNSUPPORTED
**/
EFI_STATUS
EFIAPI
SetMarginParamTestEntryPoint (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN SET_MARGIN_PARAM_CONFIG *Config
  );

/**
  This function is the memory interface health margin Test test entry point.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       Config             Pointer to test configuration structure.

  @retval  0  success
  @retval  1  error occurred
**/
EFI_STATUS
EFIAPI
MemHealthCheckTestEntry (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN BOOT_RAS_CONFIG *Config
  );

#endif // __MEMORY_EVCONTENT_LIB_H__
