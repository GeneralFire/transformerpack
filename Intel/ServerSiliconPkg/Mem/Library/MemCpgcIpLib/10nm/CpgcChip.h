/** @file
  Memory Initialization Module.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2020 Intel Corporation. <BR>

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

#ifndef _CPGCCHIP_H
#define _CPGCCHIP_H

#include <ReferenceCodeDataTypes.h>
#include "MemCpgcRegs.h"
#include "MrcCommonTypes.h"

//
// CPGC Init modes
//
#define Idle_Mode                           0
#define CPGC_Testing_Mode                   1
#define MRS_Mode                            2
#define NOP_Mode                            3


#define CPGC_ERR_MONITOR_ALL_CL        0xFF // monitor errors on all cachelines
#define CPGC_ERR_MONITOR_NO_CL         0x0  // do not monitor errors on any cacheline
#define CPGC_ERR_MONITOR_ALL_CHUNK     0xFF // monitor errors on all chunks
#define CPGC_ERR_MONITOR_NO_CHUNK      0x0  // do not monitor errors on any chunk
#define CPGC_ERR_NEVER_STOP_ON_ERR     0x0  // Prevent any err from causing a test to stop
#define CPGC_ERR_STOP_ON_ALL_ERR       0x3  // If all data lanes see an err (Data_ Err_Status and ECC_Err_Status) then a test is stopped.

//
// CADB Mux# Pattern/LFSR Buffer initial values
// These patterns are chosen to cause the CADB buffer patterns to appear on the CMD pins in this order: 0 -> 15.
// NOTE: For the values below, must also program MCSCHED_CHKN_BIT.def_1 to 1. Otherwise the patterns will appear
//       on the CMD pins in this order: 1 -> 15, 0.
//
#define CADB_MUX_PATTERN0           0xaaaa
#define CADB_MUX_PATTERN1           0x6666
#define CADB_MUX_PATTERN2           0x1e1e
#define CADB_MUX_PATTERN3           0x01fe

#ifdef DDR5_SUPPORT
#define CS_CLK_CADB_MUX_PATTERN0    0x5555
#define CADB_MUX_PATTERN2_CACLK     0x77777777
#define CADB_MUX_PATTERN3_CACLK     0x00000000
#define DB_DFE_LOOP_COUNT           2048
#define DB_DFE_CAHCE_LINES          32
#endif

#define WL_FLY_BY_LC_DDR5           1  // 2^N
#define WL_NUM_CL_DDR5              1

#define CADB_MUX_PATTERN0_VA           0xEA1
#define CADB_MUX_PATTERN1_VA           0xBEEF
#define CADB_MUX_PATTERN2_VA           0xDEAD
#define CADB_MUX_PATTERN3_VA           0x0

#define CADB_MUX_CTRL_LMN     0
#define CADB_MUX_CTRL_BUFFER  1
#define CADB_MUX_CTRL_LFSR16  2

#define CADB_MUX_LOAD_START   0
#define CADB_MUX_LOAD_END     1

#define   REC_EN_LOOP_COUNT 6  // 2^N
#define   REC_EN_NUM_CL     32
#define   REC_EN_BURST_WAIT 4

//
// RTL Offset Delay Values
//
#define RTL_OFFSET_0   0
#define RTL_OFFSET_1   2

#define CPGC_BASE_RW_SUBSEQ_CHIP   2 // 0010: Base Read Write
#define CPGC_BASE_WR_SUBSEQ_CHIP   3 // 0011: Base Write Read

//
// MISC Definitions
//
#define ENABLE_WAIT_FOR_DDRT_RPQ_EMPTY    1
#define ENABLE_USE_WRITE_TRACKER          1

#define IS_CS_SIGNAL(Signal) (Signal == CS0_A_N) || (Signal == CS0_B_N) || (Signal == CS1_A_N) || (Signal == CS1_B_N) || (Signal == CS2_A_N) || (Signal == CS2_B_N) || (Signal == CS3_A_N) || (Signal == CS3_B_N)

typedef struct CpgcGlobalTrainingSetup {
  UINT8     startDelay;
  UINT8     addrUpdRateMode;
  UINT8     seqLoopCnt;
  UINT8     rwMode;
  UINT8     numCacheLines;
  UINT8     burstSeparation;
  UINT8     errEnChunk;
  UINT8     errEnCl;
  UINT8     stopOnErrCtl;
  UINT8     subseqEndPtr;
  UINT8     useSubSeq1;
  UINT8     cpgcLinkTrain;
  UINT8     back2backWrEn;
  UINT8     mprControl;
  UINT8     refMask;
  UINT8     zqMask;
  UINT8     x8Mode;
} CPGC_GLOBAL_TRAINING_SETUP;

//
// Function Decls
//
VOID
CpgcDisableGlobalStart (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch
  );


/**

  Polls for cpgc test done on ddr channels indicated with mask

  @param Host                        - Pointer to sysHost
  @param socket                        - socket to train
  @param ddrChMask                   - ddr channel mask (1 = active channel)
  @param SubCh                       - Sub channel number

  @retval SUCCESS / FAILURE


**/
UINT32
CpgcPollGlobalTestDoneDdr (
  IN PSYSHOST  Host,
  IN UINT8     socket,
  IN UINT32    ddrChMask,
  IN UINT8     SubCh
  );

/**

    CPGC Global Test Start for DDR

    @param Host       - Pointer to sysHost
    @param socket     - socket number
    @param ddrChEnMap - bit mask of channels to use
    @param SubCh      - Sub channel number

    @retval SUCCESS

**/
UINT32
CpgcGlobalTestStartDdr (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    ddrChEnMap,
  UINT8     SubCh
  );

/**

  Detect if there cached and desired(new) values for CADB Mux Pattern are out of synced

  @param[in]                Socket              Current socket under test (0-Based)
  @param[in]                Channel             Current channel under test (0-Based)
  @param[in]                CadbMuxPattern      Pointer to array that contain the desired values for CadbMuxPattern

  @retval                   TRUE                Cache and desired values are out of sync
  @retval                   FALSE               Cache and desired values are in sync
  @retval                   TRUE                MemSsType is HBM, so there are no cache values (Out of sync)

**/
BOOLEAN
EFIAPI
CheckCadbMuxPatternCache (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT32          *CadbMuxPattern
  );

/**

  Update cached with desired(new) values for CADB Mux Pattern

  @param[in]                Socket              Current socket under test (0-Based)
  @param[in]                Channel             Current channel under test (0-Based)
  @param[in]                CadbMuxPattern      Pointer to array that contain the desired values for CadbMuxPattern


**/
VOID
EFIAPI
UpdateCadbMuxPatternCache (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT32          *CadbMuxPattern
  );


/**

Fix up the CS field for programming the CADB

@param cs             - Chip select
@param subrank        - Sub Rank
@param pattern        - Data to program
@param encodedCSMode  - Encoded CS mode

**/
VOID
FixupCS (
  UINT8         cs,
  UINT8         subrank,
  PatCadbProg0  prog0[CADB_LINES],
  PatCadbProg1  prog1[CADB_LINES],
  UINT8         encodedCSMode
  );

/**

Fix up the CS field for programming the CADB

@param cs             - Chip select
@param subrank        - Sub Rank
@param prog0           - Data to program
@param prog1           - Data to program
@param encodedCSMode  - Encoded CS mode

**/
VOID
FixupCSBrs (
  UINT8         cs,
  UINT8         subrank,
  PatCadbProg0  prog0[CADB_LINES],
  PatCadbProg1  prog1[CADB_LINES],
  UINT8         encodedCSMode
  );
/**

Global training CPGC setup

@param Host                - Pointer to sysHost
@param socket              - socket to train
@param ch                  - ddr channel to setup
@param seqLoopCnt          - Iterations of the complete Sequence loop
@param startDelay          - Delay period (in DClks) for start sequence
@param AddrUpdRateMode     - address update rate mode
@param rwMode              - subsequence type for subseq0:  0000 - base read, 0001 base write
@param numCacheLines       - number of cachelines to be transmitted
@param burstSeparation     - # of DCLK cycles between completion of the current sub-sequence and beginning
the next sub-sequence
@param errEnChunk          - Bit mask to enable any checking on any specific chunk within the Cache Line.
@param errEnCl             - Bit mask to enable what Cachelines are checked within each group of 8 received.
@param stopOnErrCtl        - Defines what type of err is will Stop a test:
00: Never Stop On Any Err
01: Stop on the Nth Any Lane Err
10: Stop on All Byte Groups Err
11: Stop on All Lanes Err
@param subseqEndPtr        - specifies end of subsequence
@param useSubSeq1          - flag to indicate init of subseq1 needed (0 - not used, 1 - base read, 2 - base writes)
@param cpgcLinkTrain       - set cpgc_link_train bit
@param back2backWrEn       - enable back2back writes
@param mprControl          - enable / disable MPR
@param refMask             - self refresh mask
@param zqMask              - zq mask

@retval SUCCESS / FAILURE

**/
UINT32
CpgcGlobalTrainingSetup (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  CPGC_GLOBAL_TRAINING_SETUP *cpgcGblTrainingSetup
  );

/**

Setup WDB for training.

@param Host             - pointer to sysHost
@param socket           - socket to train
@param ch               - ddr channel
@param wdbIncrRate      - increment rate
@param wdbIncrScale     - increment scale
@param wdbStartPntr     - start pointer
@param wdbEndPntr       - end pointer
@param setupPbwr        - flag to indicate setup of Pb write
@param muxCtl           - mux control (0 - LMN, 1 - pat buffer,  2 - LFSR242)
@param colBaInc         - column base addresses increment
@param rowBaInc         - rwo base addresses increment
@param bankBaInc        - bank base addresses increment
@param baseAddressStart - base address start
@param baseAddressWrap  - base address wrap

@retval n/a

**/
VOID
CpgcSetupWdb (
  PSYSHOST                                      Host,
  UINT8                                         socket,
  UINT8                                         ch,
  UINT8                                         wdbIncrRate,
  UINT8                                         wdbIncrScale,
  UINT8                                         wdbStartPntr,
  UINT8                                         wdbEndPntr,
  UINT8                                         setupPbwr,
  UINT8                                         muxCtl,
  CPGC_SEQ_BAINCCTL00_MC_MAIN_STRUCT            seqBaincCtl00,
  CPGC_SEQ_BAINCCTL10_MC_MAIN_STRUCT            seqBaincCtl10,
  CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_STRUCT     seqBaseAddrStartLo0,
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_STRUCT     seqBaseAddrStartHi0,
  CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_STRUCT      seqBaseAddrWrapLo0,
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_STRUCT      seqBaseAddrWrapHi0,
  CPGC_SEQ_BAOCIC0_MC_MAIN_STRUCT               cpgcSeqBaoCic0
  );

/**
  Begin or end the process of loading the CADB buffer write pointer
  The function must be called with CADB_MUX_LOAD_START before the CADB Mux Pattern registers (CPGC_PATCADBMUX[0-3]PB) are programmed.
  This function must be called with CADB_MUX_LOAD_END after the CADB Mux Pattern registers (CPGC_PATCADBMUX[0-3]PB) are programmed.

  @param[in]  Socket      - Socket number
  @param[in]  Ch          - Channel number
  @param[in]  StartStop   - CADB_MUX_LOAD_START or CADB_MUX_LOAD_END

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
CadbMuxPatternLoad (
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  IN    UINT8     StartStop
  );

/**

  Loads the CADB for BRS.

  @param Host       - Pointer to sysHost
  @param socket     - Socket to train
  @param ch         - DDR channel to setup
  @param startLine  - start line number
  @param numLines   - cache line numbers
  @param prog0      - pattern to be programmed
  @param prog1      - pattern to be programmed

  @retval SUCCESS / FAILURE

**/
VOID
ProgramCADBBrs (
  IN PSYSHOST      Host,
  IN UINT8         socket,
  IN UINT8         ch,
  IN UINT8         startLine,
  IN UINT8         numLines,
  IN PatCadbProg0  prog0[CADB_LINES],
  IN PatCadbProg1  prog1[CADB_LINES]
  );

/**

  Collect DDR advanced command CPGC error status

  @param Host                - Pointer to sysHost
  @param socket              - Socket to train
  @param ddrChEnMap          - 4-bit bit mask of ddr channels to test - (1 = active channel)
  @param cpgcErrorStatus     - error status for each channel

  @retval SUCCESS / FAILURE

  Notes:

  0 = no error
  1 = error

**/
UINT32
CpgcAdvTrainingErrorStatusWorker (
  PSYSHOST          Host,
  UINT8             socket,
  UINT32            ddrChEnMap,
  CPGC_ERROR_STATUS cpgcErrorStatus[MAX_CH]
);

/**

  This function deswizzles CPGC error status relative to DQ bit lanes per byte.

  @param[in] CpgcError      - Dword of CPGC error status
  @param[in] SwapNibbles    - Flag to indicate if nibbles need to be swapped


**/
VOID
BrsCpgcErrorStatusDeswizzle (
  IN OUT  UINT32  *CpgcError,
  IN      UINT8   SwapNibbles
);


/**

  Setup write levelling coarse parameter

  @param[in]  Host         - Pointer to sysHost, the system Host (root) structure
  @param[out] CPGCAddress  - CPGC address array
  @param[out] WDBPattern   - WDB Pattern
  @param[out] LoopCount    - Number of cycles
  @param[out] NumCl        - Number of cache lines
  @param[out] BurstWait    - Number of DCLK spacing between bursts

  @retval None

**/
VOID
SetupWriteLevellingCoarseParam(
  IN  PSYSHOST     Host,                            // Pointer to sysHost, the system Host (root) structure
  OUT TCPGCAddress *CPGCAddress,                    // CPGC address array
  OUT TWDBPattern  *WDBPattern,                     // WDB Pattern
  OUT UINT8        *LoopCount,                      // Number of cycles
  OUT UINT8        *NumCl,                          // Number of cache lines
  OUT UINT8        *BurstWait                       // Number of DCLK spacing between bursts
  );

/**

  Setup the pattern sequencer

  @param[in] Host       - Pointer to the system Host (root) structure
  @param[in] Socket     - Processor socket ID
  @param[in] ChBitMask  - Ch Bit mak for which test should be setup for.
  @param[in] Dimm       - Processor socket channel dimm number
  @param[in] Rank       - Processor socket channel dimm rank number

**/
VOID
SetupChipPatternSequencer (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT32   ChBitMask,
  IN UINT8    Dimm,
  IN UINT8    Rank
  );

/**

  This function fills the WDB buffer

  @param[in] Host                - Pointer to sysHost
  @param[in] Socket              - Socket number
  @param[in] Ch                  - Channel number
  @param[in] WdbLines            - Array that include data to write to the WDB
  @param[in] NumberOfWdbLines    - The number of cachelines to write
  @param[in] StartCachelineIndex - Start offset/cacheline number on the WDB.

  @retval N/A

**/
VOID
WDBFill (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN TWdbLine *WdbLines,
  IN UINT8    NumberOfWdbLines,
  IN UINT8    StartCachelineIndex
  );

/**

  This function loads the WDB using PCI accesses

  @param[in] Host                 - Pointer to sysHost
  @param[in] Socket               - Socket number
  @param[in] Ch                   - Channel number
  @param[in] NumberOfWdbLines     - Number of cachelines to load
  @param[in] WdbLines             - Pointer to data to write to the WDB
  @param[in] StartCachelineIndex  - Start offset/cacheline number on the WDB.

  @retval N/A

**/
VOID
WdbPreLoadChip (
  IN   PSYSHOST  Host,
  IN   UINT8     Socket,
  IN   UINT8     Ch,
  IN   UINT8     NumberOfWdbLines,
  IN   UINT8     *WdbLines,
  IN   UINT8     StartCachelineIndex
  );

/**

  Perform setup required before polling on CPGC test done registers.
  The following are initialized for each channel on the indicated memory controller:
    - DDRT Read Pending Queue timeout (occurred) indicators are cleared.
    - DDRT Read Pending Queue timeout enabled indicators are set.

  @param[in]  Host                                - Pointer to SysHost
  @param[in]  Socket                              - Socket being trained
  @param[in]  MemoryController                    - Memory controller to be polled on
  @param[in]  ChannelMask                         - Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[out] DdrtReadPendingQueueTimeoutEnabled  - Array indicating whether the Read Pending Queue timeout is enabled
                                                    for a given channel

  @retval EFI_SUCCESS   No errors were encountered
  @retval !EFI_SUCCESS  An error was encountered

**/
EFI_STATUS
SetUpTestDonePolling (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     MemoryController,
  IN  UINT32    ChannelMask,
  OUT BOOLEAN   DdrtReadPendingQueueTimeoutEnabled[MAX_CH]
  );

/**

  Poll on channel-specific indicators of test completion.
  Polling will end when all active channels on the indicated memory controller have either finished testing.

  @param[in]  Host                                - Pointer to SysHost
  @param[in]  Socket                              - Socket being trained
  @param[in]  MemoryController                    - Memory controller to be polled on
  @param[in]  ChannelMask                         - Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[in]  DdrtReadPendingQueueTimeoutEnabled  - Array indicating whether the Read Pending Queue timeout is enabled
                                                    for a given channel

  @retval EFI_SUCCESS   No errors were encountered.
  @retval !EFI_SUCCESS  An error was encountered.

**/
EFI_STATUS
PollUntilChannelTestingDone (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     MemoryController,
  IN  UINT32    ChannelMask,
  IN  BOOLEAN   DdrtReadPendingQueueTimeoutEnabled[MAX_CH]
  );

/**

  Poll on the global (memory controller-level) clear errors bit until it deasserts.

  @param[in]  Host              - Pointer to SysHost
  @param[in]  Socket            - Socket being trained
  @param[in]  MemoryController  - Memory controller to be polled on

  @retval EFI_SUCCESS   No errors were encountered
  @retval !EFI_SUCCESS  An error was encountered

**/
EFI_STATUS
PollOnGlobalClearErrors (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     MemoryController
  );

/**

  Conclude testing/polling at the memory controller-level.

  If the indicated memory controller hasn't timed out, polling will occur until the memory controller indicates that
  testing. However, if the memory controller *has* timed out, CPGC will be forced to stop.

  @param[in]  Host              - Pointer to SysHost
  @param[in]  Socket            - Socket being trained
  @param[in]  MemoryController  - Memory controller to be polled on

  @retval EFI_SUCCESS   No errors were encountered
  @retval !EFI_SUCCESS  An error was encountered

**/
EFI_STATUS
ConcludeMemoryControllerTesting (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     MemoryController
  );

/**

  Clean up the DDRT Read Pending Queue state following a test.

  This clears bits related to timeout errors and drains the DDRT Read Pending Queue if a timeout occurred.

  @param[in]  Host                                - Pointer to SysHost
  @param[in]  Socket                              - Socket being trained
  @param[in]  MemoryController                    - Memory controller to be polled on
  @param[in]  ChannelMask                         - Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[in]  DdrtReadPendingQueueTimeoutEnabled  - Array indicating whether the Read Pending Queue timeout is enabled
                                                    for a given channel

  @retval EFI_SUCCESS   No errors were encountered.
  @retval !EFI_SUCCESS  An error was encountered.

**/
EFI_STATUS
CleanUpDdrtReadPendingQueue (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     MemoryController,
  IN  UINT32    ChannelMask,
  IN  BOOLEAN   DdrtReadPendingQueueTimeoutEnabled[MAX_CH]
  );

  /**

This function writes Vref CS/CA Command by CADB

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Ch              - Ch to send command to
  @param[in] Dimm            - Dimm to send command to
  @param[in] Subchannelmask  - Sub channel mask
  @param[in] Rank            - CS to send the command to
  @param[in] Opcode          - Opcode to use
  @param[in] Timingmode      - Supports TIMING_SINGLE_CS, TIMING_MULTI_CS
  @param[in] PassThroughMode - PassThrough mode or not
  @param[in] CS/CA Mode      - CA or CS command

  @retval N/A

**/
VOID
WriteCadbVrefCsCaCmd (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Subchannelmask,
  IN UINT8     Rank,
  IN UINT16    Opcode,
  IN UINT8     Timingmode,
  IN BOOLEAN   PassThroughMode,
  IN UINT8     CsCaMode
);

/**

  DDR Mem test / Mem Init CPGC setup.

  @param[in] host        - Pointer to sysHost
  @param[in] socket      - Memory controller to train
  @param[in] ch          - ddr channel to setup
  @param[in] DqLoopCount - dq sequence loop count - number of times dq sequence is executed (2^(DqLoopCount - 1))
  @param[in] pattern     - flag to indicate mem init vs. mem test and read vs. write and data pattern

  @retval SUCCESS / FAILURE

**/
UINT32
EFIAPI
CpgcMemTestGlobalSetup (
  IN PSYSHOST  host,
  IN UINT8     socket,
  IN UINT8     ch,
  IN UINT8     rank,
  IN UINT8     DqLoopCount,
  IN UINT32    pattern
  );

/**

  DDR Mem test / Mem Init CPGC setup for logical rank.

  @param[in] host              - Pointer to sysHost
  @param[in] socket            - Memory controller to train
  @param[in] DdrChEnMap        - ddr channels to setup
  @param[in] LogRank           - logical rank to setup
  @param[in] LogSubRank        - logical subrank number to setup
  @param[in] ColAddressBits    - number of DRAM column address bits to test
  @param[in] RowAddressBits    - number of DRAM row address bits to test
  @param[in] BankAddressBits   - number of DRAM bank address bits to test

   @retval N/A

**/
VOID
EFIAPI
CpgcMemTestRankSetup (
  IN PSYSHOST  host,
  IN UINT8     socket,
  IN UINT32    DdrChEnMap,
  IN UINT8     LogRank,
  IN UINT8     LogSubRank,
  IN UINT8     ColAddressBits[MAX_CH],
  IN UINT8     RowAddressBits[MAX_CH],
  IN UINT8     BankAddressBits[MAX_CH]
  );

/**

  CPGC Clear Global Error Status

  @param[in] host              - Pointer to sysHost
  @param[in] socket            - Memory controller to train

  @retval SUCCESS

**/
UINT32
EFIAPI
CpgcClearGlobalErrorStatus (
  IN PSYSHOST  host,
  IN UINT8     socket
  );

/**
  Program the CADB MUX Pattern and Control registers

  @param[in]   Socket            - Socket number
  @param[in]   Channel           - Channel number
  @param[in]   SubChannel        - Sub Channel number
  @param[in]   CadbMuxPattern    - Pointer of CADB MUX Patterns
  @param[in]   CadbMuxCtl        - Pointer of CADB MUX Controls

  @retval      EFI_SUCCESS         This function completed execution with no warnings/errors

**/
EFI_STATUS
ProgramCadbMux (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT8           SubChannel,
  IN        UINT32          *CadbMuxPattern,
  IN        UINT8           *CadbMuxCtl
  );
#endif
