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

//
// Include files
//
#include <Library/MemoryCoreLib.h>
#include <Library/MemCpgcIpLib.h>
#include "MemCpgcRegs.h"
#include <UncoreCommonIncludes.h>
#include <Library/SysHostPointerLib.h>
#include "CpgcChip.h"
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemRcLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CheckpointLib.h>
#include <Chip/Include/CpuPciAccess.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/CteNetLib.h>
#include <Library/MemCpgcIpTargetLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/TimerLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/MemRoutingLib.h>
#include <Library/AdvMemTestLib.h>
#include <Memory/JedecDefinitions.h>
#include <Memory/MemCmdCtlClkCommon.h>
#include <Library/MemoryEvContentLib.h>
#include <Library/RcMemAllocLib.h>
#include <Library/SpdDecodeLib.h>
#include <Library/PerformanceTrackerLib.h>

#ifdef DDR5_SUPPORT
#include <Memory/Ddr5RcwRegs.h>
#endif // #ifdef DDR5_SUPPORT

extern UINT8  tWL_DDR4[MAX_SUP_FREQ];
extern UINT8 columnBitsDDR4[MAX_TECH];     // Number of column address bits

//
// Constants used for CPGC synchronized start
// Units in nanoseconds
//
#define CSR_READ_RETURN_MAX         500
#define BACK_TO_BACK_CSR_WRITE_DIFF 300
#define SYNC_OFFSET_PADDING         120
//
// Maximum value supported by CPGC_GLOBAL_SYNC.trigger_val
// The field width is 10 bits
//
#define TRIGGER_VAL_MAX  ((1 << 10) - 1)

//
// ERID error status mapping. Even UIs maps to Bit0. Odd UIs maps to Bit1
//
#define ERID_BIT0_MASK 0x55
#define ERID_BIT1_MASK 0xAA

//
// Array to indicate how to map the LFSR # with the Lane and UI
// The different LFSR selection on each Lane for a given UI
// There are 4 patterns used for advanced backside ddr training
//
#define DDR_CPGC_NUM_PATTERNS       4
STATIC UINT32 CpgcAdvCmdPatternSetup (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 Dimm,UINT8 rank, UINT8 dqLoopCount);
STATIC UINT32 CpgcAdvDqSetup (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 rank, UINT8 subRank, UINT8 dqLoopCount);
STATIC INT16  GetMinDwl (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 dimm);
STATIC UINT32 CpgcFillWdbDqVa (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 mode);

#ifdef DDR5_SUPPORT
VOID SetupWriteLevellingParamDdr5 (PSYSHOST Host, TCPGCAddress *CPGCAddress, TWDBPattern *WDBPattern, UINT8 *LoopCount, UINT8 *NumCl, UINT8 *BurstWait);
VOID SetupDqSwizzleDiscoveryParamDdr5 (PSYSHOST Host, UINT8 Socket, TCPGCAddress *CPGCAddress, TWDBPattern *WDBPattern, UINT8 *LoopCount, UINT8 *NumCl, UINT8 *BurstWait);
#endif

STATIC VOID
CpgcSetupAddrGen (
  PSYSHOST                                      Host,
  UINT8                                         socket,
  UINT8                                         ch,
  CPGC_SEQ_BAINCCTL00_MC_MAIN_STRUCT            seqBaincCtl00,
  CPGC_SEQ_BAINCCTL10_MC_MAIN_STRUCT            seqBaincCtl10,
  CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_STRUCT     seqBaseAddrStartLo0,
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_STRUCT     seqBaseAddrStartHi0,
  CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_STRUCT      seqBaseAddrWrapLo0,
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_STRUCT      seqBaseAddrWrapHi0,
  CPGC_SEQ_BAOCIC0_MC_MAIN_STRUCT               cpgcSeqBaoCic0
  );

/**
  Fix up the address bit field for programming the CADB

  @param[in] Signal        - Signal to test
  @param[in, out] Prog0    - CADB data to program
  @param[in, out] Prog1    - CADB data to program
  @param[in] CmdTimingMode - Command Timing Mode

**/
STATIC VOID
FixupAddrBits (
  IN     GSM_CSN       Signal,
  IN OUT PatCadbProg0  Prog0[CADB_LINES],
  IN OUT PatCadbProg1  Prog1[CADB_LINES],
  IN     UINT8         CmdTimingMode
  );

/**

  Fix up the address bit field for a backside address for programming the CADB

  @param[in]      Rank    Physical rank to train (0-based within the channel)
  @param[in]      sig     Signal to test
  @param[in, out] prog0   Data to program
  @param[in, out] prog1   Data to program

**/
STATIC VOID
FixupBacksideAddrBits (
  IN      UINT8         Rank,
  IN      GSM_CSN       Sig,
  IN OUT  PatCadbProg0  Prog0[CADB_LINES],
  IN OUT  PatCadbProg1  Prog1[CADB_LINES]
  );

/**

  Fix up the Chip Select field for backside CS for programming the CADB

  @param[in]        Cs     Rank chip select
  @param[in]        EncodedCsMode - Encoded CS mode value for the channel
  @param[in, out]   Prog1  Data to program

**/
STATIC VOID
FixupBacksideCS (
  IN      UINT8         Cs,
  IN      UINT8         EncodedCsMode,
  IN OUT  PatCadbProg1  Prog1[CADB_LINES]
  );

STATIC UINT32
GetPmonCounter(
               PSYSHOST  Host,
               UINT8     socket,
               UINT8     ch
);

STATIC UINT8
IsAepRank(
          PSYSHOST Host,
          UINT8    socket,
          UINT8    ch,
          UINT8    logRank
);

STATIC VOID
ProgCADBRow (
  PSYSHOST      Host,
  UINT8         socket,
  UINT8         ch,
  UINT8         dimm,
  UINT8         rank,
  UINT8         bank,
  UINT32        data,
  UINT8         side,
  UINT8         pdaTag,
  UINT8         control,
  PatCadbProg0  *patCADBProg0,
  PatCadbProg1  *patCADBProg1
  );

STATIC VOID
SetupPmon(
  PSYSHOST        Host,
  UINT8           socket,
  UINT8           ch,
  UINT8           eventSelect,
  UINT8           unitMask
);

/** Set up the ERID pattern and its validation mask.

@param[in] Host         Pointer to Host structure
@param[in] Socket       Processor socket within the system (0-based)
@param[in] Ch           Channel number inside the socket

@retval N/A

**/
STATIC VOID
SetupErid (
  IN PSYSHOST        Host,
  IN UINT8           Socket,
  IN UINT8           Ch
  );

/** Returns whether or not a channel is active.

  @param[in]  ChannelMask   Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[in]  Channel       The channel to check against

  @retval FALSE The channel is not active
  @retval TRUE  The channel is active

**/
BOOLEAN
ChannelActive (
  IN  UINT32  ChannelMask,
  IN  UINT8   Channel
  );

/** Set the DDRT Read Pending Queue (RPQ) credit limit.

  @param[in] Host         Pointer to Host structure
  @param[in] Socket       Processor socket within the system (0-based)
  @param[in] CreditLimit  The maximum number of transactions in the RPQ before
                          the DDRT scheduler stops returning credits to the CPGC
                          engine and logs an error.

  @retval N/A

**/
VOID
SetCpgcRpqCreditLimit (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT32    CreditLimit
  );

/** Perform setup required before polling on CPGC test done registers.

  The following are initialized for each channel on the indicated memory controller:
    - DDRT Read Pending Queue timeout (occurred) indicators are cleared.
    - DDRT Read Pending Queue timeout enabled indicators are set.

  @param[in]  Host                                Pointer to SysHost
  @param[in]  Socket                              Socket being trained
  @param[in]  MemoryController                    Memory controller to be polled on
  @param[in]  ChannelMask                         Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[out] DdrtReadPendingQueueTimeoutEnabled  Array indicating whether the Read Pending Queue timeout is enabled
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

/** Check DDRT-specific conditions to determine whether a channel is done being tested.

  This checks for Read Pending Queue timeouts on DDRT ranks.

  @param[in]  Host                                Pointer to SysHost
  @param[in]  Socket                              Socket being trained
  @param[in]  Channel                             Channel being trained
  @param[in]  DdrtReadPendingQueueTimeoutEnabled  Array indicating whether the Read Pending Queue timeout is enabled
                                                  for a given channel
  @param[out] ChannelDone                         Whether or not the channel is done testing.

  @retval EFI_SUCCESS   No errors were encountered.
  @retval !EFI_SUCCESS  An error was encountered.

**/
EFI_STATUS
CheckDdrtChannelDone (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  BOOLEAN   DdrtReadPendingQueueTimeoutEnabled[MAX_CH],
  OUT BOOLEAN   *ChannelDone
  );

/** Poll on channel-specific indicators of test completion.

  Polling will end when all active channels on the indicated memory controller have either finished testing.

  @param[in]  Host                                Pointer to SysHost
  @param[in]  Socket                              Socket being trained
  @param[in]  MemoryController                    Memory controller to be polled on
  @param[in]  ChannelMask                         Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[in]  DdrtReadPendingQueueTimeoutEnabled  Array indicating whether the Read Pending Queue timeout is enabled
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

/** Poll on the global (memory controller-level) clear errors bit until it deasserts.

  @param[in]  Host              Pointer to SysHost
  @param[in]  Socket            Socket being trained
  @param[in]  MemoryController  Memory controller to be polled on

  @retval EFI_SUCCESS   No errors were encountered
  @retval !EFI_SUCCESS  An error was encountered

**/
EFI_STATUS
PollOnGlobalClearErrors (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     MemoryController
  );

/** Conclude testing/polling at the memory controller-level.

  If the indicated memory controller hasn't timed out, polling will occur until the memory controller indicates that
  testing. However, if the memory controller *has* timed out, CPGC will be forced to stop.

  @param[in]  Host              Pointer to SysHost
  @param[in]  Socket            Socket being trained
  @param[in]  MemoryController  Memory controller to be polled on

  @retval EFI_SUCCESS   No errors were encountered
  @retval !EFI_SUCCESS  An error was encountered

**/
EFI_STATUS
ConcludeMemoryControllerTesting (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     MemoryController
  );

/** Clean up the DDRT Read Pending Queue state following a test.

  This clears bits related to timeout errors and drains the DDRT Read Pending Queue if a timeout occurred.

  @param[in]  Host                                Pointer to SysHost
  @param[in]  Socket                              Socket being trained
  @param[in]  MemoryController                    Memory controller to be polled on
  @param[in]  ChannelMask                         Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[in]  DdrtReadPendingQueueTimeoutEnabled  Array indicating whether the Read Pending Queue timeout is enabled
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

/** Set whether a given channel has hit a DDRT Read Pending Queue timeout.

  @param[in] Host     Pointer to sysHost
  @param[in] Socket   Processor socket within the system (0-based)
  @param[in] Channel  Number of the channel to check
  @param[in] TimedOut Whether or not the channel has hit a DDRT Read Pending Queue timeout.

**/
VOID
SetDdrtReadPendingQueueTimedOut (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  BOOLEAN   TimedOut
  );

/**
  Calculate even parity

  @param[in]  Field   Bit field to calculate the parity over

  @retval     0x0     There is an even numer of ones in the input field
  @retval     0x1     There is an odd number of ones in the input field
**/
UINT32
CalculateEvenParity (
  IN UINT32   Field
  );

/**
  Calculate CADB-related parity (without accounting for the slot number)

  @param[in]  channelNvList   Pointer to the Channel NV list
  @param[in]  Prog0           Pointer to PatCADBProg0 structure
  @param[in]  Prog1           Pointer to PatCADBProg1 structure
  @param[in]  Ch              Channel number
**/
UINT8
CalculateCadbParity (
  IN struct channelNvram  (*ChannelNvList)[MAX_CH],
  IN PatCadbProg0         *Prog0,
  IN PatCadbProg1         *Prog1,
  IN UINT8                Ch
  );

/**

  Use CPGC Global Synchronous Control to start the test on multiple MCs at the same time.

  @param[in] Host       - Pointer to SysHost
  @param[in] Socket     - Socket number
  @param[in] DdrChEnMap - Map of socket channels to run the test on
  @param[in]  SubCh     - Subchannel number

  @retval MRC_STATUS_SUCCESS if CPGC Global Synchronous Control is enabled
          MRC_STATUS_FAILURE if CPGC Global Synchronous Control is not enabled

**/
MRC_STATUS
CpgcGlobalTestSynchronousStart (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT32    DdrChEnMap,
  UINT8     SubCh
  );

STATIC PatCadbProg0 CtlClkPattern0[CADB_LINES] = {
// addr, cid, bank, pdatag
  {0x0,   0x0, 0x0, 0x0}, // Row 0
  {0x0,   0x0, 0x0, 0x0}, // Row 1
  {0x0,   0x0, 0x0, 0x0}, // Row 2
  {0x0,   0x0, 0x0, 0x0}, // Row 3
  {0x0,   0x0, 0x0, 0x0}, // Row 4
  {0x0,   0x0, 0x0, 0x0}, // Row 5
  {0x0,   0x0, 0x0, 0x0}, // Row 6
  {0x0,   0x0, 0x0, 0x0}, // Row 7
  {0x0,   0x0, 0x0, 0x0}, // Row 8
  {0x0,   0x0, 0x0, 0x0}, // Row 9
  {0x0,   0x0, 0x0, 0x0}, // Row 10
  {0x0,   0x0, 0x0, 0x0}, // Row 11
  {0x0,   0x0, 0x0, 0x0}, // Row 12
  {0x0,   0x0, 0x0, 0x0}, // Row 13
  {0x0,   0x0, 0x0, 0x0}, // Row 14
  {0x0,   0x0, 0x0, 0x0}, // Row 15
};

STATIC PatCadbProg1 CtlClkPattern1[CADB_LINES] = {
// CS, Control, ODT,  CKE, Par
  {0x3FF, 0, 0x3F, 0x3F, 0}, // Row 0
  {0x0,   0,  0x0,  0x0, 0}, // Row 1
  {0x3FF, 0, 0x3F, 0x3F, 0}, // Row 2
  {0x0,   0,  0x0,  0x0, 0}, // Row 3
  {0x3FF, 0, 0x3F, 0x3F, 0}, // Row 4
  {0x0,   0,  0x0,  0x0, 0}, // Row 5
  {0x3FF, 0, 0x3F, 0x3F, 0}, // Row 6
  {0x0,   0,  0x0,  0x0, 0}, // Row 7
  {0x3FF, 0, 0x3F, 0x3F, 0}, // Row 8
  {0x0,   0,  0x0,  0x0, 0}, // Row 9
  {0x3FF, 0, 0x3F, 0x3F, 0}, // Row 10
  {0x0,   0,  0x0,  0x0, 0}, // Row 11
  {0x3FF, 0, 0x3F, 0x3F, 0}, // Row 12
  {0x0,   0,  0x0,  0x0, 0}, // Row 13
  {0x3FF, 0, 0x3F, 0x3F, 0}, // Row 14
  {0x0,   0,  0x0,  0x0, 0}, // Row 15
};

                                                // addr, cid, bank, pdatag
STATIC PatCadbProg0 ParityPattern0[CADB_LINES] = {{0x0,   0x0, 0x1, 0x0}, // Row 0
                                                  {0x0,   0x0, 0x1, 0x0}, // Row 1
                                                  {0x0,   0x0, 0x1, 0x0}, // Row 2
                                                  {0x0,   0x0, 0x1, 0x0}, // Row 3
                                                  {0x0,   0x0, 0x1, 0x0}, // Row 4
                                                  {0x0,   0x0, 0x1, 0x0}, // Row 5
                                                  {0x0,   0x0, 0x1, 0x0}, // Row 6
                                                  {0x0,   0x0, 0x1, 0x0}, // Row 7
                                                  {0x0,   0x0, 0x0, 0x0}, // Row 8
                                                  {0x0,   0x0, 0x0, 0x0}, // Row 9
                                                  {0x0,   0x0, 0x0, 0x0}, // Row 10
                                                  {0x0,   0x0, 0x0, 0x0}, // Row 11
                                                  {0x0,   0x0, 0x0, 0x0}, // Row 12
                                                  {0x0,   0x0, 0x0, 0x0}, // Row 13
                                                  {0x0,   0x0, 0x0, 0x0}, // Row 14
                                                  {0x0,   0x0, 0x0, 0x0}, // Row 15
                                                 };

                                                  // CS  , Control, ODT,  CKE, Par
STATIC PatCadbProg1 ParityPattern1[CADB_LINES] = {{0x3FF,     0xF, 0x0, 0x3F, 0}, // Row 0
                                                  {0x3FF,     0xF, 0x0, 0x3F, 0}, // Row 1
                                                  {0x3FF,     0xF, 0x0, 0x3F, 0}, // Row 2
                                                  {0x3FF,     0xF, 0x0, 0x3F, 0}, // Row 3
                                                  {0x3FE,     0xF, 0x0, 0x3F, 1}, // Row 4*
                                                  {0x3FF,     0xF, 0x0, 0x3F, 0}, // Row 5
                                                  {0x3FF,     0xF, 0x0, 0x3F, 0}, // Row 6
                                                  {0x3FF,     0xF, 0x0, 0x3F, 0}, // Row 7
                                                  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 8
                                                  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 9
                                                  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 10
                                                  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 11
                                                  {0x3FE,     0xF, 0x0, 0x3F, 0}, // Row 12*
                                                  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 13
                                                  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 14
                                                  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 15
                                                 };

                                                 // CS  , Control, ODT,  CKE, Par
STATIC PatCadbProg1 ParityPattern12N[CADB_LINES] = {{0x3FF,     0xF, 0x0, 0x3F, 0}, // Row 0
                                                    {0x3FF,     0xF, 0x0, 0x3F, 0}, // Row 1
                                                    {0x3FF,     0xF, 0x0, 0x3F, 0}, // Row 2
                                                    {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 3
                                                    {0x3FE,     0xF, 0x0, 0x3F, 1}, // Row 4*
                                                    {0x3FF,     0xF, 0x0, 0x3F, 0}, // Row 5
                                                    {0x3FF,     0xF, 0x0, 0x3F, 0}, // Row 6
                                                    {0x3FF,     0xF, 0x0, 0x3F, 0}, // Row 7
                                                    {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 8
                                                    {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 9
                                                    {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 10
                                                    {0x3FF,     0xF, 0x0, 0x3F, 0}, // Row 11
                                                    {0x3FE,     0xF, 0x0, 0x3F, 0}, // Row 12*
                                                    {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 13
                                                    {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 14
                                                    {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 15
                                                   };


STATIC PatCadbProg0 BacksideParityPattern0[CADB_LINES] = {
  // addr,  cid, bank, pdatag
  {0x0, 0x0, 0x8, 0x0}, // Row 0
  {0x0, 0x0, 0x8, 0x0}, // Row 1
  {0x0, 0x0, 0x8, 0x0}, // Row 2
  {0x0, 0x0, 0x8, 0x0}, // Row 3
  {0x0, 0x0, 0x8, 0x0}, // Row 4
  {0x0, 0x0, 0x8, 0x0}, // Row 5
  {0x0, 0x0, 0x8, 0x0}, // Row 6
  {0x0, 0x0, 0x8, 0x0}, // Row 7
  {0x0, 0x0, 0x0, 0x0}, // Row 8
  {0x0, 0x0, 0x0, 0x0}, // Row 9
  {0x0, 0x0, 0x0, 0x0}, // Row 10
  {0x0, 0x0, 0x0, 0x0}, // Row 11
  {0x0, 0x0, 0x0, 0x0}, // Row 12
  {0x0, 0x0, 0x0, 0x0}, // Row 13
  {0x0, 0x0, 0x0, 0x0}, // Row 14
  {0x0, 0x0, 0x0, 0x0}, // Row 15
};

STATIC PatCadbProg1 BacksideParityPattern1[CADB_LINES] = {
  // CS, Control, ODT,  CKE, Par
  {0x3FF, 0xF, 0x0, 0x3F, 0}, // Row 0
  {0x3FD, 0xF, 0x0, 0x3F, 0}, // Row 1
  {0x3FE, 0xF, 0x0, 0x3F, 1}, // Row 2*
  {0x3FD, 0xF, 0x0, 0x3F, 0}, // Row 3
  {0x3FF, 0xF, 0x0, 0x3F, 0}, // Row 4
  {0x3FF, 0xF, 0x0, 0x3F, 0}, // Row 5
  {0x3FF, 0xF, 0x0, 0x3F, 0}, // Row 6
  {0x3FF, 0xF, 0x0, 0x3F, 0}, // Row 7
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 8
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 9
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 10
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 11
  {0x3FD, 0xF, 0x0, 0x3F, 1}, // Row 12
  {0x3FE, 0xF, 0x0, 0x3F, 0}, // Row 13*
  {0x3FD, 0xF, 0x0, 0x3F, 1}, // Row 14
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 15
};

STATIC PatCadbProg0 BacksideActivatePattern0[CADB_LINES] = {
  // addr,  cid, bank, pdatag
  {0x0, 0x0, 0x0, 0x0}, // Row 0
  {0x0, 0x0, 0x0, 0x0}, // Row 1
  {BIT14 | BIT15 | BIT16, 0x0, 0x0, 0x0}, // Row 2*
  {0x0, 0x0, 0x0, 0x0}, // Row 3
  {0x0, 0x0, 0x0, 0x0}, // Row 4
  {0x0, 0x0, 0x0, 0x0}, // Row 5
  {0x0, 0x0, 0x0, 0x0}, // Row 6
  {0x0, 0x0, 0x0, 0x0}, // Row 7
  {0x0, 0x0, 0x0, 0x0}, // Row 8
  {0x0, 0x0, 0x0, 0x0}, // Row 9
  {0x0, 0x0, 0x0, 0x0}, // Row 10
  {0x0, 0x0, 0x0, 0x0}, // Row 11
  {0x0, 0x0, 0x0, 0x0}, // Row 12
  {0x0, 0x0, 0x0, 0x0}, // Row 13*
  {0x0, 0x0, 0x0, 0x0}, // Row 14
  {0x0, 0x0, 0x0, 0x0}, // Row 15
};

STATIC PatCadbProg1 BacksideActivatePattern1[CADB_LINES] = {
  // CS  , Control, ODT,  CKE, Par
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 0
  {0x3FD, 0xF, 0x0, 0x3F, 1}, // Row 1
  {0x3FE, 0x3, 0x0, 0x3F, 1}, // Row 2*
  {0x3FD, 0xF, 0x0, 0x3F, 1}, // Row 3
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 4
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 5
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 6
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 7
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 8
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 9
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 10
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 11
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 12
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 13*
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 14
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 15
};

STATIC PatCadbProg0 BacksideCommandPattern0[CADB_LINES] = {
  // addr,  cid, bank, pdatag
  {0x0, 0x0, 0x8, 0x0}, // Row 0
  {0x0, 0x0, 0x8, 0x0}, // Row 1
  {BIT14, 0x0, 0x8, 0x0}, // Row 2*
  {0x0, 0x0, 0x8, 0x0}, // Row 3
  {0x0, 0x0, 0x8, 0x0}, // Row 4
  {0x0, 0x0, 0x8, 0x0}, // Row 5
  {0x0, 0x0, 0x8, 0x0}, // Row 6
  {0x0, 0x0, 0x8, 0x0}, // Row 7
  {0x0, 0x0, 0x0, 0x0}, // Row 8
  {0x0, 0x0, 0x0, 0x0}, // Row 9
  {0x0, 0x0, 0x0, 0x0}, // Row 10
  {0x0, 0x0, 0x0, 0x0}, // Row 11
  {0x0, 0x0, 0x0, 0x0}, // Row 12
  {BIT15 | BIT16, 0x0, 0x0, 0x0}, // Row 13*
  {0x0, 0x0, 0x0, 0x0}, // Row 14
  {0x0, 0x0, 0x0, 0x0}, // Row 15
};

STATIC PatCadbProg1 BacksideCommandPattern1[CADB_LINES] = {
  // CS  , Control, ODT,  CKE, Par
  {0x3FF, 0x0, 0x0, 0x3F, 1}, // Row 0
  {0x3FD, 0x0, 0x0, 0x3F, 1}, // Row 1
  {0x3FE, 0x3, 0x0, 0x3F, 0}, // Row 2*
  {0x3FD, 0x0, 0x0, 0x3F, 1}, // Row 3
  {0x3FF, 0x0, 0x0, 0x3F, 1}, // Row 4
  {0x3FF, 0x0, 0x0, 0x3F, 1}, // Row 5
  {0x3FF, 0x0, 0x0, 0x3F, 1}, // Row 6
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 7
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 8
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 9
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 10
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 11
  {0x3FD, 0xF, 0x0, 0x3F, 1}, // Row 12
  {0x3FE, 0x3, 0x0, 0x3F, 0}, // Row 13*
  {0x3FD, 0xF, 0x0, 0x3F, 1}, // Row 14
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 15
};

STATIC PatCadbProg0 BacksideAddressPattern0[CADB_LINES] = {
  // addr,  cid, bank, pdatag
  {0x0, 0x0, 0x0, 0x0}, // Row 0
  {0x0, 0x0, 0x0, 0x0}, // Row 1
  {0x1, 0x0, 0x0, 0x0}, // Row 2*
  {0x0, 0x0, 0x0, 0x0}, // Row 3
  {0x0, 0x0, 0x0, 0x0}, // Row 4
  {0x0, 0x0, 0x0, 0x0}, // Row 5
  {0x0, 0x0, 0x0, 0x0}, // Row 6
  {0x0, 0x0, 0x0, 0x0}, // Row 7
  {0x1, 0x0, 0x0, 0x0}, // Row 8
  {0x1, 0x0, 0x0, 0x0}, // Row 9
  {0x1, 0x0, 0x0, 0x0}, // Row 10
  {0x1, 0x0, 0x0, 0x0}, // Row 11
  {0x1, 0x0, 0x0, 0x0}, // Row 12
  {0x0, 0x0, 0x0, 0x0}, // Row 13*
  {0x1, 0x0, 0x0, 0x0}, // Row 14
  {0x1, 0x0, 0x0, 0x0}, // Row 15
};

STATIC PatCadbProg1 BacksideAddressPattern1[CADB_LINES] = {
  // CS, Control, ODT,  CKE, Par
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 0
  {0x3FD, 0xF, 0x0, 0x3F, 1}, // Row 1
  {0x3FE, 0xF, 0x0, 0x3F, 1}, // Row 2*
  {0x3FD, 0xF, 0x0, 0x3F, 1}, // Row 3
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 4
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 5
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 6
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 7
  {0x3FF, 0xF, 0x0, 0x3F, 0}, // Row 8
  {0x3FF, 0xF, 0x0, 0x3F, 0}, // Row 9
  {0x3FF, 0xF, 0x0, 0x3F, 0}, // Row 10
  {0x3FF, 0xF, 0x0, 0x3F, 0}, // Row 11
  {0x3FD, 0xF, 0x0, 0x3F, 0}, // Row 12
  {0x3FE, 0xF, 0x0, 0x3F, 0}, // Row 13*
  {0x3FD, 0xF, 0x0, 0x3F, 0}, // Row 14
  {0x3FF, 0xF, 0x0, 0x3F, 0}, // Row 15
};

                                            // addr, cid, bank, pdatag
STATIC PatCadbProg0 ActivatePattern0[CADB_LINES] = {{BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 0
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 1
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 2
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 3
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 4*
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 5
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 6
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 7
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 8
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 9
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 10
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 11
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 12
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 13
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 14
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 15
                                            };

                                            // CS  , Control, ODT,  CKE, Par
STATIC PatCadbProg1 ActivatePattern1[CADB_LINES] = {{0x3FF,     0xB, 0x0, 0x3F, 0}, // Row 0
                                             {0x3FF,     0xB, 0x0, 0x3F, 0}, // Row 1
                                             {0x3FF,     0xB, 0x0, 0x3F, 0}, // Row 2
                                             {0x3FF,     0xB, 0x0, 0x3F, 0}, // Row 3
                                             {0x3FE,     0x3, 0x0, 0x3F, 0}, // Row 4*
                                             {0x3FF,     0xB, 0x0, 0x3F, 0}, // Row 5
                                             {0x3FF,     0xB, 0x0, 0x3F, 0}, // Row 6
                                             {0x3FF,     0xB, 0x0, 0x3F, 0}, // Row 7
                                             {0x3FF,     0xB, 0x0, 0x3F, 0}, // Row 8
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 9
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 10
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 11
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 12
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 13
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 14
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 15
                                            };

                                                     // CS  , Control, ODT,  CKE, Par
STATIC PatCadbProg1 ActivatePattern12N[CADB_LINES] = {{0x3FF,     0xB, 0x0, 0x3F, 0}, // Row 0
                                                      {0x3FF,     0xB, 0x0, 0x3F, 0}, // Row 1
                                                      {0x3FF,     0xB, 0x0, 0x3F, 0}, // Row 2
                                                      {0x3FF,     0x3, 0x0, 0x3F, 0}, // Row 3
                                                      {0x3FE,     0x3, 0x0, 0x3F, 0}, // Row 4*
                                                      {0x3FF,     0xB, 0x0, 0x3F, 0}, // Row 5
                                                      {0x3FF,     0xB, 0x0, 0x3F, 0}, // Row 6
                                                      {0x3FF,     0xB, 0x0, 0x3F, 0}, // Row 7
                                                      {0x3FF,     0xB, 0x0, 0x3F, 0}, // Row 8
                                                      {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 9
                                                      {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 10
                                                      {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 11
                                                      {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 12
                                                      {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 13
                                                      {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 14
                                                      {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 15
                                                     };

                                                     // addr, cid, bank, pdatag
STATIC PatCadbProg0 PrechargePattern0[CADB_LINES] = {{0x400,   0x0, 0x0, 0x0}, // Row 0
                                                     {0x400,   0x0, 0x0, 0x0}, // Row 1
                                                     {0x400,   0x0, 0x0, 0x0}, // Row 2
                                                     {0x400,   0x0, 0x0, 0x0}, // Row 3
                                                     {0x400,   0x0, 0x0, 0x0}, // Row 4
                                                     {0x400,   0x0, 0x0, 0x0}, // Row 5
                                                     {0x400,   0x0, 0x0, 0x0}, // Row 6
                                                     {0x400,   0x0, 0x0, 0x0}, // Row 7
                                                     {0x400,   0x0, 0x0, 0x0}, // Row 8
                                                     {0x0,   0x0, 0x0, 0x0}, // Row 9
                                                     {0x0,   0x0, 0x0, 0x0}, // Row 10
                                                     {0x0,   0x0, 0x0, 0x0}, // Row 11
                                                     {0x0,   0x0, 0x0, 0x0}, // Row 12
                                                     {0x0,   0x0, 0x0, 0x0}, // Row 13
                                                     {0x0,   0x0, 0x0, 0x0}, // Row 14
                                                     {0x0,   0x0, 0x0, 0x0}, // Row 15
                                                    };

                                                    // CS  , Control, ODT,  CKE, Par
STATIC PatCadbProg1 PrechargePattern1[CADB_LINES] = {{0x3FF,     0xA, 0x0, 0x3F, 1}, // Row 0
                                                     {0x3FF,     0xA, 0x0, 0x3F, 1}, // Row 1
                                                     {0x3FF,     0xA, 0x0, 0x3F, 1}, // Row 2
                                                     {0x3FF,     0xA, 0x0, 0x3F, 1}, // Row 3
                                                     {0x3FE,     0xA, 0x0, 0x3F, 1}, // Row 4*
                                                     {0x3FF,     0xA, 0x0, 0x3F, 1}, // Row 5
                                                     {0x3FF,     0xA, 0x0, 0x3F, 1}, // Row 6
                                                     {0x3FF,     0xA, 0x0, 0x3F, 1}, // Row 7
                                                     {0x3FF,     0xA, 0x0, 0x3F, 1}, // Row 8
                                                     {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 9
                                                     {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 10
                                                     {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 11
                                                     {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 12
                                                     {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 13
                                                     {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 14
                                                     {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 15
                                                    };

                                        // addr, cid, bank, pdatag
STATIC PatCadbProg0 FnvGoodParity0[CADB_LINES] = {{0x0,   0x0, 0x0, 0x0}, // Row 0
                                          {0x0,   0x0, 0x0, 0x0}, // Row 1
                                          {0x0,   0x0, 0x0, 0x0}, // Row 2
                                          {0x0,   0x0, 0x7, 0x0}, // Row 3*
                                          {0x0,   0x0, 0x7, 0x0}, // Row 4*
                                          {0x0,   0x0, 0x7, 0x0}, // Row 5*
                                          {0x0,   0x0, 0x0, 0x0}, // Row 6
                                          {0x0,   0x0, 0x0, 0x0}, // Row 7
                                          {0x0,   0x0, 0x0, 0x0}, // Row 8
                                          {0x0,   0x0, 0x0, 0x0}, // Row 9
                                          {0x0,   0x0, 0x0, 0x0}, // Row 10
                                          {0x0,   0x0, 0x0, 0x0}, // Row 11
                                          {0x0,   0x0, 0x0, 0x0}, // Row 12
                                          {0x0,   0x0, 0x0, 0x0}, // Row 13
                                          {0x0,   0x0, 0x0, 0x0}, // Row 14
                                          {0x0,   0x0, 0x0, 0x0}, // Row 15
                                        };

                                          // CS  , Control, ODT,  CKE, Par
STATIC PatCadbProg1 FnvGoodParity1[CADB_LINES] = {{0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 0
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 1
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 2
                                          {0x3FF,     0x8, 0x0, 0x3F, 0}, // Row 3*
                                          {0x3FE,     0x8, 0x0, 0x3F, 0}, // Row 4*
                                          {0x3FF,     0x8, 0x0, 0x3F, 0}, // Row 5*
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 6
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 7
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 8
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 9
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 10
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 11
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 12
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 13
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 14
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 15
                                        };

                                            // CS  , Control, ODT,  CKE, Par
STATIC PatCadbProg1 DeselectPattern1[CADB_LINES] = {{0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 0
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 1
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 2
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 3
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 4
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 5
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 6
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 7
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 8
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 9
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 10
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 11
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 12
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 13
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 14
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 15
                                            };


  // CS  , Control, ODT,  CKE, Par
STATIC PatCadbProg1 GntPattern1[CADB_LINES] = {
  {0x3FE,     0xF, 0x0, 0x3F, 1}, // Row 0  //GNT
  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 1
  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 2
  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 3
  {0x3FE,     0xF, 0x0, 0x3F, 1}, // Row 4  // GNT
  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 5
  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 6
  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 7
  {0x3FE,     0xF, 0x0, 0x3F, 1}, // Row 8  // GNT
  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 9
  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 10
  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 11
  {0x3FE,     0xF, 0x0, 0x3F, 1}, // Row 12  //GNT
  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 13
  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 14
  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 15
};
                                         // addr, cid, bank, pdatag
STATIC PatCadbProg0 AddressPattern0[CADB_LINES] = {{0x0,   0x0, 0x0, 0x0}, // Row 0
                                          {0x0,   0x0, 0x0, 0x0}, // Row 1
                                          {0x0,   0x0, 0x0, 0x0}, // Row 2
                                          {0x0,   0x0, 0x0, 0x0}, // Row 3
                                          {0x0,   0x0, 0x8, 0x0}, // Row 4
                                          {0x0,   0x0, 0x0, 0x0}, // Row 5
                                          {0x0,   0x0, 0x0, 0x0}, // Row 6
                                          {0x0,   0x0, 0x0, 0x0}, // Row 7
                                          {0x0,   0x0, 0x0, 0x0}, // Row 8
                                          {0x0,   0x0, 0x8, 0x0}, // Row 9
                                          {0x0,   0x0, 0x8, 0x0}, // Row 10
                                          {0x0,   0x0, 0x8, 0x0}, // Row 11
                                          {0x0,   0x0, 0x0, 0x0}, // Row 12
                                          {0x0,   0x0, 0x8, 0x0}, // Row 13
                                          {0x0,   0x0, 0x8, 0x0}, // Row 14
                                          {0x0,   0x0, 0x8, 0x0}, // Row 15
                                         };

                                         // CS  , Control, ODT,  CKE, Par
STATIC PatCadbProg1 AddressPattern1[CADB_LINES] = {{0x3FF,     0x3, 0x0, 0x3F, 1}, // Row 0
                                          {0x3FF,     0x3, 0x0, 0x3F, 1}, // Row 1
                                          {0x3FF,     0x3, 0x0, 0x3F, 1}, // Row 2
                                          {0x3FF,     0x3, 0x0, 0x3F, 1}, // Row 3
                                          {0x3FE,     0x3, 0x0, 0x3F, 1}, // Row 4
                                          {0x3FF,     0x3, 0x0, 0x3F, 1}, // Row 5
                                          {0x3FF,     0x3, 0x0, 0x3F, 1}, // Row 6
                                          {0x3FF,     0x3, 0x0, 0x3F, 1}, // Row 7
                                          {0x3FF,     0x3, 0x0, 0x3F, 1}, // Row 8
                                          {0x3FF,     0x3, 0x0, 0x3F, 0}, // Row 9
                                          {0x3FF,     0x3, 0x0, 0x3F, 0}, // Row 10
                                          {0x3FF,     0x3, 0x0, 0x3F, 0}, // Row 11
                                          {0x3FE,     0x3, 0x0, 0x3F, 0}, // Row 12
                                          {0x3FF,     0x3, 0x0, 0x3F, 0}, // Row 13
                                          {0x3FF,     0x3, 0x0, 0x3F, 0}, // Row 14
                                          {0x3FF,     0x3, 0x0, 0x3F, 0}, // Row 15
                                         };

                                         // addr, cid, bank, pdatag
STATIC PatCadbProg0 AdvCmdPattern0[CADB_LINES] = {{0x00000,   0x0, 0x0, 0x0}, // Row 0
                                          {0x22222,   0x0, 0x08, 0x0}, // Row 1
                                          {0x11111,   0x2, 0x04, 0x0}, // Row 2
                                          {0x33333,   0x2, 0x0C, 0x0}, // Row 3
                                          {0x08888,   0x1, 0x02, 0x0}, // Row 4
                                          {0x2AAAA,   0x1, 0x0A, 0x0}, // Row 5
                                          {0x19999,   0x3, 0x06, 0x0}, // Row 6
                                          {0x3BBBB,   0x3, 0x0E, 0x0}, // Row 7
                                          {0x04444,   0x0, 0x01, 0x0}, // Row 8
                                          {0x26666,   0x0, 0x09, 0x0}, // Row 9
                                          {0x15555,   0x2, 0x05, 0x0}, // Row 10
                                          {0x37777,   0x2, 0x0D, 0x0}, // Row 11
                                          {0x0CCCC,   0x1, 0x03, 0x0}, // Row 12
                                          {0x2EEEE,   0x1, 0x0B, 0x0}, // Row 13
                                          {0x1DDDD,   0x3, 0x07, 0x0}, // Row 14
                                          {0x3FFFF,   0x3, 0x0F, 0x0}, // Row 15
                                         };

                                         // CS  , Control, ODT,  CKE, Par
STATIC PatCadbProg1 AdvCmdPattern1[CADB_LINES] = {{0x0,     0x00, 0x0, 0x0, 0x0}, // Row 0
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 1
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 2
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 3
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 4
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 5
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 6
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 7
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 8
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 9
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 10
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 11
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 12
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 13
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 14
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 15
                                         };

                                         // addr, cid, bank, pdatag
STATIC PatCadbProg0 AggAdvCmdPattern0[CADB_LINES] = {{0x00000,   0x0, 0x0, 0x0}, // Row 0
                                          {0x02040,   0x0, 0x00, 0x0}, // Row 1
                                          {0x3DFBF,   0x7, 0x0F, 0x0}, // Row 2
                                          {0x3FFFF,   0x7, 0x0F, 0x0}, // Row 3
                                          {0x00000,   0x0, 0x00, 0x0}, // Row 4
                                          {0x02040,   0x0, 0x00, 0x0}, // Row 5
                                          {0x3DFBF,   0x7, 0x0F, 0x0}, // Row 6
                                          {0x3FFFF,   0x7, 0x0F, 0x0}, // Row 7
                                          {0x00000,   0x0, 0x00, 0x0}, // Row 8
                                          {0x02040,   0x0, 0x00, 0x0}, // Row 9
                                          {0x3DFBF,   0x7, 0x0F, 0x0}, // Row 10
                                          {0x3FFFF,   0x7, 0x0F, 0x0}, // Row 11
                                          {0x00000,   0x0, 0x00, 0x0}, // Row 12
                                          {0x02040,   0x0, 0x00, 0x0}, // Row 13
                                          {0x3DFBF,   0x7, 0x0F, 0x0}, // Row 14
                                          {0x3FFFF,   0x7, 0x0F, 0x0}, // Row 15
                                         };

                                         // CS  , Control, ODT,  CKE, Par
STATIC PatCadbProg1 AggAdvCmdPattern1[CADB_LINES] = {{0x0,     0x00, 0x0, 0x0, 0x0}, // Row 0
                                          {0x0,     0x08, 0x0, 0x0, 0x0}, // Row 1
                                          {0x0,     0x07, 0x0, 0x0, 0x0}, // Row 2
                                          {0x0,     0x0F, 0x0, 0x0, 0x0}, // Row 3
                                          {0x0,     0x00, 0x0, 0x0, 0x0}, // Row 4
                                          {0x0,     0x08, 0x0, 0x0, 0x0}, // Row 5
                                          {0x0,     0x07, 0x0, 0x0, 0x0}, // Row 6
                                          {0x0,     0x0F, 0x0, 0x0, 0x0}, // Row 7
                                          {0x0,     0x00, 0x0, 0x0, 0x0}, // Row 8
                                          {0x0,     0x08, 0x0, 0x0, 0x0}, // Row 9
                                          {0x0,     0x07, 0x0, 0x0, 0x0}, // Row 10
                                          {0x0,     0x0F, 0x0, 0x0, 0x0}, // Row 11
                                          {0x0,     0x00, 0x0, 0x0, 0x0}, // Row 12
                                          {0x0,     0x08, 0x0, 0x0, 0x0}, // Row 13
                                          {0x0,     0x07, 0x0, 0x0, 0x0}, // Row 14
                                          {0x0,     0x0F, 0x0, 0x0, 0x0}, // Row 15
                                         };

STATIC PatCadbProg0 ReadCommandsMRSMode0[CADB_LINES] = {{0x4000,   0x0, 0x0, 0x0}, // Row 0
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 1
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 2
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 3
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 4
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 5
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 6
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 7
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 8
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 9
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 10
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 11
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 12
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 13
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 14
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 15
                                               };

                                         // CS  , Control, ODT,  CKE, Par
STATIC PatCadbProg1 ReadCommandsMRSMode1[CADB_LINES] = {{0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 0
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 1
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 2
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 3
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 4*
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 5
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 6
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 7
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 8
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 9
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 10
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 11
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 12*
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 13
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 14
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 15
                                               };

// wdb chunks 0-3, 4-7
/*
const UINT32 CpgcWdbDqVaPat [8] =     { 0xfffe0100,           // wdb 0 for dq/va
                                        0xfffd0200,           // wdb 1 for dq/va
                                        0xfffb0400,           // wdb 2 for dq/va
                                        0xfff70800,           // wdb 3 for dq/va
                                        0xffef1000,           // wdb 4 for dq/va
                                        0xffdf2000,           // wdb 5 for dq/va
                                        0xffbf4000,           // wdb 6 for dq/va
                                        0xff7f8000,           // wdb 7 for dq/va
                                      };
 */
STATIC const UINT8 CpgcWdbDqVaPat [8][8] =  {
                                        {0, 0x01, 0xFE, 0xFF, 0, 0x01, 0xFE, 0xFF}, //0xfffe0100,           // wdb 0 for dq/va
                                        {0, 0x02, 0xFD, 0xFF, 0, 0x02, 0xFD, 0xFF}, //0xfffd0200,           // wdb 1 for dq/va
                                        {0, 0x04, 0xFB, 0xFF, 0, 0x04, 0xFB, 0xFF}, //0xfffb0400,           // wdb 2 for dq/va
                                        {0, 0x08, 0xF7, 0xFF, 0, 0x08, 0xF7, 0xFF}, //0xfff70800,           // wdb 3 for dq/va
                                        {0, 0x10, 0xEF, 0xFF, 0, 0x10, 0xEF, 0xFF}, //0xffef1000,           // wdb 4 for dq/va
                                        {0, 0x20, 0xDF, 0xFF, 0, 0x20, 0xDF, 0xFF}, //0xffdf2000,           // wdb 5 for dq/va
                                        {0, 0x40, 0xBF, 0xFF, 0, 0x40, 0xBF, 0xFF}, //0xffbf4000,           // wdb 6 for dq/va
                                        {0, 0x80, 0x7F, 0xFF, 0, 0x80, 0x7F, 0xFF}  //0xff7f8000,           // wdb 7 for dq/va
                                      };

STATIC const UINT8 CpgcWdbDqVaPat2 [8][8] =  {
                                        {0, 0xEE, 0x11, 0xFF, 0, 0xEE, 0x11, 0xFF}, //          // wdb 0 for dq/va
                                        {0, 0xEE, 0x11, 0xFF, 0, 0xEE, 0x11, 0xFF}, //          // wdb 1 for dq/va
                                        {0, 0xEE, 0x11, 0xFF, 0, 0xEE, 0x11, 0xFF}, //          // wdb 2 for dq/va
                                        {0, 0xEE, 0x11, 0xFF, 0, 0xEE, 0x11, 0xFF}, //          // wdb 3 for dq/va
                                        {0, 0xEE, 0x11, 0xFF, 0, 0xEE, 0x11, 0xFF}, //          // wdb 4 for dq/va
                                        {0, 0xEE, 0x11, 0xFF, 0, 0xEE, 0x11, 0xFF}, //          // wdb 5 for dq/va
                                        {0, 0xEE, 0x11, 0xFF, 0, 0xEE, 0x11, 0xFF}, //          // wdb 6 for dq/va
                                        {0, 0xEE, 0x11, 0xFF, 0, 0xEE, 0x11, 0xFF} //           // wdb 7 for dq/va

};

/*
*/
STATIC VOID
CADBPointerWorkaround (
                       PatCadbProg0 prog0[],
                       PatCadbProg1 prog1[]
                       )
{
}

/**
  Calculate even parity

  @param[in]  Field   Bit field to calculate the parity over

  @retval     0x0     There is an even numer of ones in the input field
  @retval     0x1     There is an odd number of ones in the input field
**/
UINT32
CalculateEvenParity (
  IN UINT32   Field
  )
{
  const UINT32 Length = 32;
  UINT32 Shift;

  for (Shift = Length / 2; Shift > 0; Shift >>= 1) {
    Field ^= Field >> Shift;
  }

  return Field & BIT0;
}

/**
  Calculate CADB-related parity (without accounting for the slot number)

  @param[in]  channelNvList   Pointer to the Channel NV list
  @param[in]  Prog0           Pointer to PatCADBProg0 structure
  @param[in]  Prog1           Pointer to PatCADBProg1 structure
  @param[in]  Ch              Channel number
**/
UINT8
CalculateCadbParity (
  IN struct channelNvram  (*ChannelNvList)[MAX_CH],
  IN PatCadbProg0         *Prog0,
  IN PatCadbProg1         *Prog1,
  IN UINT8                Ch
  )
{
  UINT32 Parity = 0;

  //
  // Calculate parity of CID bits if they are to be included
  //
  if ((*ChannelNvList)[Ch].encodedCSMode == 2) {
    Parity = ((*ChannelNvList)[Ch].cidBitMap & BIT2) && ((*Prog0).cid & BIT2);
    Parity ^= ((*ChannelNvList)[Ch].cidBitMap & BIT1) && ((*Prog0).cid & BIT1);
    Parity ^= ((*ChannelNvList)[Ch].cidBitMap & BIT0) && ((*Prog0).cid & BIT0);
  }

  //
  // Address bit parity
  //
  Parity ^= CalculateEvenParity ((*Prog0).address);

  //
  // Bank bit parity
  //
  Parity ^= CalculateEvenParity ((*Prog0).bank & 0xF);

  //
  // Control bit parity
  //
  Parity ^= CalculateEvenParity ((*Prog1).control & 0xF);

  return Parity & BIT0;
}

/**
  Calculate address bit mask based on whether or not A17 is enabled

  @param[in]  Host        - Pointer to SysHost
  @param[in]  Socket      - Socket number
  @param[in]  Ch          - Channel number
  @param[in]  Dimm        - Dimm number
  @param[out] AddressMask - Mask to use to zero out unused address bits
**/
VOID
CalculateAddressMask (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  OUT UINT32    *AddressMask
  )
{

  if (A17EnabledBackside (Host, Socket, Ch, Dimm)) {
    *AddressMask = 0x3FFFF;
  } else {
    *AddressMask = 0x1FFFF;
  }

  return;
}


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
  )
{
  UINT32                            Def1Value;
  PSYSHOST                          Host;
  PWMM_STARV_CNTR_PRESCALER_MCDDC_CTL_STRUCT  PwmmStarvCntrPrescaler;

  Host = GetSysHostPointer ();
  //
  // Program cadb write pointer bit with 0 at the start of CADB MUX programming and 1 after programming
  //
  Def1Value = 0;
  if (StartStop == CADB_MUX_LOAD_END) {
    Def1Value = 1;
  }

  //
  // Delay if this is a CTE build, otherwise do nothing.
  //
  PwmmStarvCntrPrescaler.Data = MemReadPciCfgEp (Socket, Ch, PWMM_STARV_CNTR_PRESCALER_MCDDC_CTL_REG);
  PwmmStarvCntrPrescaler.Bits.en_wptr_cadb = Def1Value;
  MemWritePciCfgEp (Socket, Ch, PWMM_STARV_CNTR_PRESCALER_MCDDC_CTL_REG, PwmmStarvCntrPrescaler.Data);

  //
  // Delay if this is a CTE build, otherwise do nothing.
  //

  CteDelayDclk (10);

  return MRC_STATUS_SUCCESS;

}

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
  )
{
  PSYSHOST              Host;
  MRC_MST               MemSsType;
  struct channelNvram   (*ChannelNvList) [MAX_CH];

  Host = GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  MemSsType = GetMemSsType (Socket);

  if (MemSsType == Hbm2MemSs) {
    return TRUE;
  }

  if (((*ChannelNvList)[Channel].cadbMuxPattern[0] != CadbMuxPattern[0]) ||
      ((*ChannelNvList)[Channel].cadbMuxPattern[1] != CadbMuxPattern[1]) ||
      ((*ChannelNvList)[Channel].cadbMuxPattern[2] != CadbMuxPattern[2]) ||
      ((*ChannelNvList)[Channel].cadbMuxPattern[3] != CadbMuxPattern[3])) {
    return TRUE;
  }
  //BRS TODO 1507399397: this change is required to make MRW commands reliable... needs further debug
  if (IsBrsPresent (Host, Socket)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

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
  )
{
  PSYSHOST              Host;
  MRC_MST               MemSsType;
  struct channelNvram   (*ChannelNvList) [MAX_CH];

  Host = GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  MemSsType = GetMemSsType (Socket);

  if (MemSsType == Hbm2MemSs) {
    return;
  }

  (*ChannelNvList)[Channel].cadbMuxPattern[0] = CadbMuxPattern[0];
  (*ChannelNvList)[Channel].cadbMuxPattern[1] = CadbMuxPattern[1];
  (*ChannelNvList)[Channel].cadbMuxPattern[2] = CadbMuxPattern[2];
  (*ChannelNvList)[Channel].cadbMuxPattern[3] = CadbMuxPattern[3];

}

/**
  Program the CADB MUX Pattern and Control registers

  @param[in]                Socket              Socket number
  @param[in]                Channel             Channel number
  @param[in]                SubChannel          Sub-Channel number
  @param[in]                CadbMuxPattern      Pointer of CADB MUX Patterns
  @param[in]                CadbMuxCtl          Pointer of CADB MUX Controls

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
ProgramCadbMux (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT8           SubChannel,
  IN        UINT32          *CadbMuxPattern,
  IN        UINT8           *CadbMuxCtl
  )
{
  PSYSHOST  Host;
  UINT8     ChannelLocal;
  CPGC_PATCADBMUXCTL_MCDDC_CTL_STRUCT           PatCadbMuxCtl;

  ChannelLocal = Channel;
  Host = GetSysHostPointer ();
  if(IsBrsPresent(Host, Socket) && (SubChannel!= NO_SUBCH)) {
   GetChannelMapping(Channel, SubChannel, &ChannelLocal);
  }
  if (CheckCadbMuxPatternCache (Socket, ChannelLocal, CadbMuxPattern)) {

    CadbMuxPatternLoad (Socket, ChannelLocal, CADB_MUX_LOAD_START);

    //
    // Program CADB pattern buffer
    //
    MemWritePciCfgEp (Socket, ChannelLocal, CPGC_PATCADBMUX0PB_MCDDC_CTL_REG, CadbMuxPattern[0]);
    MemWritePciCfgEp (Socket, ChannelLocal, CPGC_PATCADBMUX1PB_MCDDC_CTL_REG, CadbMuxPattern[1]);
    MemWritePciCfgEp (Socket, ChannelLocal, CPGC_PATCADBMUX2PB_MCDDC_CTL_REG, CadbMuxPattern[2]);
    MemWritePciCfgEp (Socket, ChannelLocal, CPGC_PATCADBMUX3PB_MCDDC_CTL_REG, CadbMuxPattern[3]);

    UpdateCadbMuxPatternCache (Socket, ChannelLocal, CadbMuxPattern);

    //
    // Delay if this is a CTE build, otherwise do nothing.
    //

    CteDelayDclk (10);

    //
    // Program CADB mux control
    //
    PatCadbMuxCtl.Data = 0;
    PatCadbMuxCtl.Bits.mux0_control = CadbMuxCtl[0];
    PatCadbMuxCtl.Bits.mux1_control = CadbMuxCtl[1];
    PatCadbMuxCtl.Bits.mux2_control = CadbMuxCtl[2];
    PatCadbMuxCtl.Bits.mux3_control = CadbMuxCtl[3];
    MemWritePciCfgEp (Socket, ChannelLocal, CPGC_PATCADBMUXCTL_MCDDC_CTL_REG, PatCadbMuxCtl.Data);

    CadbMuxPatternLoad (Socket, ChannelLocal, CADB_MUX_LOAD_END);
  }

  return EFI_SUCCESS;
}

/*
      Write VA pattern in CADB
      Use basic VA pattern for CADB with 2 LFSRs. Rotation is manual
      Bit Order is  [CKE[3:0], ODT[3:0], CMD[2:0], CS[3:0], BA[2:0], MA[15:0]]
                      [59:56]  [51:48]  [42:40] [35:32]  [26:24]  [15:0]

      NOTE: CKE, ODT and CS are not used in functional mode and are ignored

       @param Host         - Include all MRC global data.
       @param socket         - Memory Controller
       @param VicSpread      -
       @param VicBit            -
       @param LMNEn           - To enable LMN counter

     @retval N/A
*/
STATIC VOID
SetupCADB(
         PSYSHOST  Host,
         UINT8     socket,
         UINT8     ch,
         UINT8     VicSpread,
         UINT8     VicBit,
         UINT8     LMNEn
         )
{
  UINT8         r;
  UINT8         b;
  UINT8         lfsr0;
  UINT8         lfsr1;
  UINT8         bremap;
  UINT8         Fine;
  UINT64_STRUCT data;
  UINT64_STRUCT datax;
  UINT64_STRUCT lfsr0_64;
  UINT64_STRUCT lfsr1_64;
  // struct RankCh (*rankPerCh)[MAX_RANK_CH];
  CPGC_PATCADBCLMUX1LMN_MCDDC_CTL_STRUCT patCadbClMux1Lmn;
  UINT32                                 CadbMuxPattern[CADB_MUX_MAX];
  UINT8                                  CadbMuxCtl[CADB_MUX_MAX];

  RcDebugPrintWithDevice (SDBG_CPGC, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "SetupCADB Starts\n");
  // Currently, always start writing at CADB row0.  Could add Start point in future.
  MemWritePciCfgEp (socket, ch, CPGC_PATCADBWRPNTR_MCDDC_CTL_REG, 0);

  // Plan to use VicSpread of 7 bits
  // Walk through CADB rows, assigning bit for 1 VA pattern
  // rankPerCh = &Host->nvram.mem.socket[socket].channelList[ch].rankPerCh;
  for (r = 0; r < MAX_RANK_CH; r++) {
    //<VM> - Chris, Should we check for validRankBitmask?
    //if (CheckRankAbsentDisabled(Host, socket, ch, (*rankPerCh)[r].dimm, (*rankPerCh)[r].rank)) continue;

    lfsr0 = (r & 0x1);      // 0, 1, 0, 1 for r = 0,1,2 & 3
    lfsr1 = (r / 2);        // 0, 0, 1, 1 for r = 0,1,2 & 3

    // Assign Victirm/Aggressor Bits
    data.lo = 0;
    data.hi = 0;
    for (b = 0; b < 22; b++) { // b in range(22)
      Fine  = b % VicSpread;
      if (b >= 19) {
        bremap = b + 21;
      } // b = [40-42]
      else if (b >= 16) bremap = b + 8; // b = [24-26]
      else         bremap = b; // b = [0-15]
      lfsr0_64.lo = lfsr0;
      lfsr0_64.hi = 0;
      lfsr1_64.lo = lfsr1;
      lfsr1_64.hi = 0;
      if (Fine == VicBit) {
        datax = LShiftUINT64(lfsr0_64, bremap);
      }
      else                 datax = LShiftUINT64(lfsr1_64, bremap);
      data.lo |= datax.lo;
      data.hi |= datax.hi;
    }

    // Write Row. CADB is auto incremented after every write
    MemWritePciCfgEp (socket, ch, CPGC_PATCADBPROG0_MCDDC_CTL_REG, data.lo);
#ifndef DDR5_SUPPORT
    MemWritePciCfgEp (socket, ch, CPGC_PATCADBPROG1_MCDDC_CTL_REG, data.hi);
#endif //DDR5_SUPPORT
  } // r loop

  //
  // Setup CADB in terms of LFSR selects, LFSR seeds, LMN constants and overall control
  //

  MemWritePciCfgEp (socket, ch, CPGC_PATCADBCLMUX0LMN_MCDDC_CTL_REG, 0);
  patCadbClMux1Lmn.Data = 0;
  patCadbClMux1Lmn.Bits.sweep_freq = 1;
  patCadbClMux1Lmn.Bits.l_counter = 1;
  patCadbClMux1Lmn.Bits.m_counter = 1;
  patCadbClMux1Lmn.Bits.n_counter = 6;
  MemWritePciCfgEp (socket, ch, CPGC_PATCADBCLMUX1LMN_MCDDC_CTL_REG, patCadbClMux1Lmn.Data);
  MemWritePciCfgEp (socket, ch, CPGC_PATCADBCLMUX2LMN_MCDDC_CTL_REG, 0);

  CadbMuxPattern[0] = CADB_MUX_PATTERN0_VA;
  CadbMuxPattern[1] = CADB_MUX_PATTERN1_VA;
  CadbMuxPattern[2] = CADB_MUX_PATTERN2_VA;
  CadbMuxPattern[3] = CADB_MUX_PATTERN3_VA;

  CadbMuxCtl[0] = CADB_MUX_CTRL_LFSR16;
  CadbMuxCtl[1] = LMNEn ? CADB_MUX_CTRL_LMN : CADB_MUX_CTRL_LFSR16;
  CadbMuxCtl[2] = CADB_MUX_CTRL_LMN;
  CadbMuxCtl[3] = CADB_MUX_CTRL_LMN;

  ProgramCadbMux (socket, ch, NO_SUBCH, CadbMuxPattern, CadbMuxCtl);

} // SetupCADB

/**
     Programs all the key registers to define a CPCG test

      @param Host           - Include all MRC global data.
      @param socket         - Memory Controller
      @param ChbitMask      - ch Bit mak for which test should be setup for.
      @param CmdPat         = [0: PatWrRd (Standard Write/Read Loopback),
                                     1: PatWr (Write Only),
                                     2: PatRd (Read Only),
                                     3: PatRdWrTA (ReadWrite Turnarounds),
                                     4: PatWrRdTA (WriteRead Turnarounds),
                                     5: PatODTTA (ODT Turnaround]
      @param NumCL          = Number of Cache lines
      @param LC             = Loop Count
      @param CPGCAddress    = Structure that stores start, stop and increment details for address
      @param SOE            = [0: Never Stop, 1: Stop on Any Lane, 2: Stop on All Byte, 3: Stop on All Lane]
      @param WDBPattern     - Structure that stores start, Stop, IncRate and Dqpat for pattern.
      @param EnCADB         - Enable test to write random deselect packages on bus to create xtalk/isi
      @param EnCKE          - Enable CKE power down by adding 64
      @param SubSeqWait     - # of Dclks to stall at the end of a sub-sequence

    @retval N/A
**/
VOID
SetupIOTest (
  PSYSHOST      Host,
  UINT8         socket,
  UINT32        ChbitMask,
  UINT8         CmdPat,
  UINT16        NumCL,
  UINT32        LC,
  TCPGCAddress  *CPGCAddress,
  UINT8         SOE,
  TWDBPattern   *WDBPattern,
  UINT8         EnCADB,
  UINT8         EnCKE,
  UINT16        SubSeqWait
  )
{
  UINT8                                     ch;
  UINT8                                     Dimm;
  UINT8                                     Mux0;
  UINT8                                     Reload;
  UINT8                                     mcId;
  TCPGCAddress                              *a;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT          PatCadbCtl;
  CPGC_SEQCTL0_MC_MAIN_STRUCT               seqCtl0;
  CPGC_SUBSEQ0_CTL0_MC_MAIN_STRUCT          subSeqCtl0;
  CPGC_SUBSEQ0_CTL0_MC_MAIN_STRUCT          subSeqCtl1;
  CPGC_SUBSEQ0_CTL0_MC_MAIN_STRUCT          subSeqCtl2;
  CPGC_SUBSEQ0_CTL0_MC_MAIN_STRUCT          subSeqCtl3;
  CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_STRUCT seqBaseAddrStartLo0;
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_STRUCT seqBaseAddrStartHi0;
  CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_STRUCT  seqBaseAddrWrapLo0;
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_STRUCT  seqBaseAddrWrapHi0;
  CPGC_SEQ_BAOCIC0_MC_MAIN_STRUCT           seqBAOCIC0;
  CPGC_SEQ_BAINCCTL00_MC_MAIN_STRUCT        seqBAIncCtl00;
  CPGC_SEQ_BAINCCTL10_MC_MAIN_STRUCT        seqBAIncCtl10;
  CPGC_PATWDBCLCTL_MCDDC_CTL_STRUCT         patWDBCLCtl;
  CPGC_PATWDBCL_MUXCTL_MCDDC_DP_STRUCT      patWDBCLMuxCtl;
  CPGC_ERR_CTL_MCDDC_CTL_STRUCT             errCtl;
  UINT32                                    DataMask;
  CPGC_GLOBALCTRL_MC_GLOBAL_STRUCT          CpgcGlobalCtrlReg;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT          CpgcPatCadbCtlReg;
  CPGC_DDRT_MIX_CFG_MC_MAIN_STRUCT          CpgcMixCfg;
#ifdef DDR5_SUPPORT
  CPGC_ERR_CTL2_MCDDC_DP_STRUCT             ErrCtl2;
#endif // DDR5_SUPPORT
  UINT8                                     MaxChDdr;
  MRC_TT                                    CurrentTestType;

  CurrentTestType = GetCurrentTestType (socket);
  MaxChDdr = GetMaxChDdr ();
  RcDebugPrintWithDevice (SDBG_CPGC, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "SetupIOTest10nm Starts\n");


  //if ((LC - Log2x32(NumCL - 1) + 1) < 1)  LCeff = 1;
  //else                                    LCeff = LC - Log2x32(NumCL - 1) + 1;
  if ((IsBrsPresent (Host, socket)) && (((CurrentTestType == RcvenFineTest)) ||
                                          (CurrentTestType == RdDqTest) ||
                                          (CurrentTestType == MdqsCoarseReadDelayTraining) ||
                                          (CurrentTestType == BacksideDqSwizzleDiscovery))) {
    for (ch = 0; ch < MaxChDdr; ch += GetChInc (Host)) {
      //
      //Check if this channel has been masked off as each BSSA Call is per MC
      //
      if ((ChbitMask & (1 << ch)) == 0) {
        continue;
      }
      CpgcGlobalCtrlReg.Data = MemReadPciCfgEp (socket, ch, CPGC_GLOBALCTRL_MC_GLOBAL_REG);
      CpgcGlobalCtrlReg.Bits.use_bl4_col_addr = 1;
      MemWritePciCfgEp (socket, ch, CPGC_GLOBALCTRL_MC_GLOBAL_REG, CpgcGlobalCtrlReg.Data);
    }
  }
  if (!IsBrsPresent (Host, socket)) {
    Reload = Log2x32 (WDBPattern->IncRate - 1);
    //Save = Reload + Log2x32((WDBPattern->Stop - WDBPattern->Start - 1) & 0xFF);

    if (WDBPattern->IncRate > 31) {
      WDBPattern->IncRate = Reload;
    }
    else {
      WDBPattern->IncRate += 32;
    }
  }
  //
  // Program Sequence
  //
  seqCtl0.Data = 0;
  switch (CmdPat) {
    case PatWrRd:
      seqCtl0.Bits.subseq_start_pointer = 0;
      seqCtl0.Bits.subseq_end_pointer = 1;
      break;
    case PatWr:
    case PatRdWrTA:
      seqCtl0.Bits.subseq_start_pointer = 0;
      seqCtl0.Bits.subseq_end_pointer = 0;
      break;
    case PatRd:
      seqCtl0.Bits.subseq_start_pointer = 1;
      seqCtl0.Bits.subseq_end_pointer = 1;
      break;
    case PatWrRdTA:
      seqCtl0.Bits.subseq_start_pointer = 0;
      seqCtl0.Bits.subseq_end_pointer = 7;
      break;
    case PatODTTA:
    case PatWrRdFlyb:
      seqCtl0.Bits.subseq_start_pointer = 0;
      seqCtl0.Bits.subseq_end_pointer = 3;
      break;
    case PatRdWrRd:
      seqCtl0.Bits.subseq_start_pointer = 0;
      seqCtl0.Bits.subseq_end_pointer = 2;
      break;
    default:
      RcDebugPrintWithDevice (SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "\nSetupIOTest10nm: Unknown Pattern\n");
      break;
  }

  seqCtl0.Bits.local_clear_errs     = 1; // Clear Local Errors
  seqCtl0.Bits.global_control       = 1; // Enable Global Control
  seqCtl0.Bits.initialization_mode  = CPGC_Testing_Mode; // Test Mode
  //seqCtl0.Bits.loopcount            = LCeff;  // Loopcount
  seqCtl0.Bits.loopcount            = LC;  // Loopcount
  seqCtl0.Bits.start_test_delay     = 2;      // Delay the start of the test by 2 clocks

  // Inc address based on LC
  if (CmdPat == DimmTest) {
    seqCtl0.Bits.address_update_rate_mode = 1;
  }
  //
  // Program Sub Sequences
  //
  subSeqCtl0.Data = 0;
  subSeqCtl1.Data = 0;
  subSeqCtl2.Data = 0;
  subSeqCtl3.Data = 0;
  // Reset Current Base address to start
  if (CmdPat == PatWrRdFlyb) {
    subSeqCtl1.Bits.rstcurr_ba_start = 1;
  } else {
    subSeqCtl0.Bits.rstcurr_ba_start = 1;
  }
  subSeqCtl2.Bits.rstcurr_ba_start = 1;
  subSeqCtl3.Bits.rstcurr_ba_start = 1;
  subSeqCtl0.Bits.num_cachelines = NumCL;
  subSeqCtl0.Bits.num_cachelines_scale = 1;
  subSeqCtl1.Bits.num_cachelines = NumCL;
  subSeqCtl1.Bits.num_cachelines_scale = 1;
  subSeqCtl2.Bits.num_cachelines = NumCL;
  subSeqCtl2.Bits.num_cachelines_scale = 1;
  subSeqCtl3.Bits.num_cachelines = NumCL;
  subSeqCtl3.Bits.num_cachelines_scale = 1;
  if (CmdPat == PatWrRdFlyb) {
    subSeqCtl0.Bits.subseq_type = 6;
    subSeqCtl1.Bits.subseq_type = 1;
    subSeqCtl2.Bits.subseq_type = 6;
    subSeqCtl3.Bits.subseq_type = 0;
    subSeqCtl0.Bits.inv_dataecc = 1;
    subSeqCtl1.Bits.inv_dataecc = 0;
    subSeqCtl2.Bits.inv_dataecc = 1;
    subSeqCtl3.Bits.inv_dataecc = 0;
    subSeqCtl0.Bits.savecurr_ba_start = 1;
    subSeqCtl1.Bits.savecurr_ba_start = 0;
    subSeqCtl2.Bits.savecurr_ba_start = 0;
    subSeqCtl3.Bits.savecurr_ba_start = 0;
    subSeqCtl0.Bits.num_cachelines = NumCL + 4;
    subSeqCtl1.Bits.num_cachelines = NumCL + 4;
    subSeqCtl2.Bits.num_cachelines = NumCL + 4;
    subSeqCtl3.Bits.num_cachelines = NumCL + 4;
  }

  for (ch = 0; ch < MaxChDdr; ch++) {

    mcId = (UINT8) GetMCID (Host, socket, ch);

    if (!(ChbitMask & (1 << ch))) {
      MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, 0);   // Clear global control
      continue;
    }
    if (IsBrsPresent (Host, socket)) {
      //
      // Disable CADB Always on mode.
      //
      CpgcPatCadbCtlReg.Data = MemReadPciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG);
      CpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 0;
      CpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 0;
      CpgcMixCfg.Data = MemReadPciCfgEp (socket, ch, CPGC_DDRT_MIX_CFG_MC_MAIN_REG);
#ifdef DDR5_SUPPORT
      CpgcPatCadbCtlReg.Bits.enable_twopasscadb_always_on = 0;
      CpgcMixCfg.Bits.enable_twopasscadb_always_on_shadow = 0;
#endif //DDR5_SUPPORT
      MemWritePciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG, CpgcPatCadbCtlReg.Data);
      MemWritePciCfgEp (socket, ch, CPGC_DDRT_MIX_CFG_MC_MAIN_REG, CpgcMixCfg.Data);
    }

    //
    // Program CADB
    //
    if (EnCADB) {
      PatCadbCtl.Data = MemReadPciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG);

      // DDRT exception, do not use deselect on DDRT
      if (AepDimmPresent (socket, ch, &Dimm)) {
        PatCadbCtl.Bits.enable_cadb_on_deselect = 0;
      } else {
        PatCadbCtl.Bits.enable_cadb_on_deselect = 1;
      }
      MemWritePciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG, PatCadbCtl.Data);

      SetupCADB(Host, socket, ch, 7, 8, 0);   // LMNEn = 0
    }

    MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, seqCtl0.Data);

    if (SubSeqWait > 0xFF) {
      SubSeqWait = 0xFF;
    }
    if (CmdPat == PatWrRdFlyb) {
      subSeqCtl0.Bits.subseq_wait = 0;
      subSeqCtl1.Bits.subseq_wait = 0;
      subSeqCtl2.Bits.subseq_wait = SubSeqWait;
      subSeqCtl3.Bits.subseq_wait = SubSeqWait;
    } else {
      subSeqCtl0.Bits.subseq_wait = SubSeqWait;
    }

    if (CmdPat == PatWrRdTA) {
      MemWritePciCfgEp (socket, ch, CPGC_SUBSEQ0_CTL0_MC_MAIN_REG, subSeqCtl0.Data + (BWr  << 20)); // Write CMD
      MemWritePciCfgEp (socket, ch, CPGC_SUBSEQ1_CTL0_MC_MAIN_REG, subSeqCtl0.Data + (BRdWr  << 20)); // Read-Write CMD
      MemWritePciCfgEp (socket, ch, CPGC_SUBSEQ2_CTL0_MC_MAIN_REG, subSeqCtl0.Data + (BRdWr  << 20)); // Read-Write CMD
      MemWritePciCfgEp (socket, ch, CPGC_SUBSEQ3_CTL0_MC_MAIN_REG, subSeqCtl0.Data + (BRdWr  << 20)); // Read-Write CMD
    } else if (CmdPat == PatRdWrTA) {
      MemWritePciCfgEp (socket, ch, CPGC_SUBSEQ0_CTL0_MC_MAIN_REG, subSeqCtl0.Data + (BWrRd << 20)); // Write-Read CMD
    } else if (CmdPat == PatODTTA) {
      MemWritePciCfgEp (socket, ch, CPGC_SUBSEQ0_CTL0_MC_MAIN_REG, subSeqCtl0.Data + (BWr  << 20)); // Write CMD
      MemWritePciCfgEp (socket, ch, CPGC_SUBSEQ1_CTL0_MC_MAIN_REG, subSeqCtl0.Data + (BRd  << 20)); // Read CMD
      MemWritePciCfgEp (socket, ch, CPGC_SUBSEQ2_CTL0_MC_MAIN_REG, subSeqCtl0.Data + (BWrRd<< 20)); // Write-Read CMD
      MemWritePciCfgEp (socket, ch, CPGC_SUBSEQ3_CTL0_MC_MAIN_REG, subSeqCtl0.Data + (BWr  << 20)); // Write CMD
      MemWritePciCfgEp (socket, ch, CPGC_SUBSEQ4_CTL0_MC_MAIN_REG, subSeqCtl0.Data + (BRdWr<< 20)); // Read-Write CMD
      MemWritePciCfgEp (socket, ch, CPGC_SUBSEQ5_CTL0_MC_MAIN_REG, subSeqCtl0.Data + (BRdWr  << 20)); // Read-Write CMD
    } else if (CmdPat == PatRdWrRd) {
      subSeqCtl0.Bits.savecurr_ba_start = 0;
      subSeqCtl0.Bits.rstcurr_ba_start = 0;
      subSeqCtl0.Bits.num_cachelines = 4;
      MemWritePciCfgEp (socket, ch, CPGC_SUBSEQ0_CTL0_MC_MAIN_REG, subSeqCtl0.Data + (BRd << 20)); // Read CMD

      subSeqCtl0.Bits.savecurr_ba_start = 1;
      subSeqCtl0.Bits.rstcurr_ba_start = 0;
      subSeqCtl0.Bits.num_cachelines = 4;
      MemWritePciCfgEp (socket, ch, CPGC_SUBSEQ1_CTL0_MC_MAIN_REG, subSeqCtl0.Data + (BWr << 20)); // Write CMD

      subSeqCtl0.Bits.savecurr_ba_start = 0;
      subSeqCtl0.Bits.rstcurr_ba_start = 1;
      subSeqCtl0.Bits.num_cachelines = 4;
      MemWritePciCfgEp (socket, ch, CPGC_SUBSEQ2_CTL0_MC_MAIN_REG, subSeqCtl0.Data + (BRd << 20)); // Read CMD

    } else if (CmdPat == PatWrRdFlyb) {
      MemWritePciCfgEp (socket, ch, CPGC_SUBSEQ0_CTL0_MC_MAIN_REG, subSeqCtl0.Data); // No Chip Select Base Write
      MemWritePciCfgEp (socket, ch, CPGC_SUBSEQ1_CTL0_MC_MAIN_REG, subSeqCtl1.Data); // Write CMD
      MemWritePciCfgEp (socket, ch, CPGC_SUBSEQ2_CTL0_MC_MAIN_REG, subSeqCtl2.Data); // No Chip Select Base Write
      MemWritePciCfgEp (socket, ch, CPGC_SUBSEQ3_CTL0_MC_MAIN_REG, subSeqCtl3.Data); // Read CMD

    } else if (CmdPat == PatWr) {
      MemWritePciCfgEp (socket, ch, CPGC_SUBSEQ0_CTL0_MC_MAIN_REG, subSeqCtl0.Data + (BWr  << 20)); // Write CMD
    } else {
      MemWritePciCfgEp (socket, ch, CPGC_SUBSEQ0_CTL0_MC_MAIN_REG, subSeqCtl0.Data + (BWr  << 20)); // Write CMD
      MemWritePciCfgEp (socket, ch, CPGC_SUBSEQ1_CTL0_MC_MAIN_REG, subSeqCtl0.Data + (BRd  << 20)); // Read CMD
    }

    //
    // Program Sequence Address
    //
    a = CPGCAddress;

    seqBaseAddrStartLo0.Data              = 0;
    seqBaseAddrStartLo0.Bits.column_addr  = a->Start[3];
    seqBaseAddrStartLo0.Bits.bank_addr    = a->Start[1];
    MemWritePciCfgEp (socket, ch, CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_REG, seqBaseAddrStartLo0.Data);

    seqBaseAddrStartHi0.Data            = 0;
    seqBaseAddrStartHi0.Bits.row_addr   = a->Start[2];
    seqBaseAddrStartHi0.Bits.rank_addr  = a->Start[0];
    MemWritePciCfgEp (socket, ch, CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_REG, seqBaseAddrStartHi0.Data);

    seqBaseAddrWrapLo0.Data             = 0;
    seqBaseAddrWrapLo0.Bits.column_addr = a->Stop[3];
    seqBaseAddrWrapLo0.Bits.bank_addr   = a->Stop[1];
    MemWritePciCfgEp (socket, ch, CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_REG, seqBaseAddrWrapLo0.Data);

    seqBaseAddrWrapHi0.Data           = 0;
    seqBaseAddrWrapHi0.Bits.row_addr  = a->Stop[2];
    seqBaseAddrWrapHi0.Bits.rank_addr = a->Stop[0];
    MemWritePciCfgEp (socket, ch, CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_REG, seqBaseAddrWrapHi0.Data);

    // Do not bother with 23:13 as this is all DIMM testing
    seqBAOCIC0.Data = 0;
    seqBAOCIC0.Bits.column_addr_order = a->Order[3];
    seqBAOCIC0.Bits.row_addr_order    = a->Order[2];
    seqBAOCIC0.Bits.bank_addr_order   = a->Order[1];
    seqBAOCIC0.Bits.rank_addr_order   = a->Order[0];
    MemWritePciCfgEp (socket, ch, CPGC_SEQ_BAOCIC0_MC_MAIN_REG, seqBAOCIC0.Data);

    seqBAIncCtl00.Data = 0;
    seqBAIncCtl10.Data = 0;
    // Rank
    if (a->IncRate[0] > 31) {
      seqBAIncCtl10.Bits.rank_ba_updrate = Log2x32(a->IncRate[0] - 1);
    } else {
      seqBAIncCtl10.Bits.rank_ba_updrate = 128 + a->IncRate[0];
    }
    seqBAIncCtl10.Bits.rank_ba_inc = a->IncVal[0] & 0x7;

    // Bank
    if (a->IncRate[1] > 31) {
      seqBAIncCtl10.Bits.bank_ba_updrate = Log2x32(a->IncRate[1] - 1);
    } else {
      seqBAIncCtl10.Bits.bank_ba_updrate = 128 + a->IncRate[1];
    }
    seqBAIncCtl10.Bits.bank_ba_inc = a->IncVal[1] & 0x7;

    // Row
    if (a->IncRate[2] > 15) {
      seqBAIncCtl00.Bits.row_ba_updrate = Log2x32(a->IncRate[2] - 1);
    } else {
      seqBAIncCtl00.Bits.row_ba_updrate = 32 + a->IncRate[2];
    }
    seqBAIncCtl00.Bits.row_ba_inc = a->IncVal[2] & 0xFFF;

    // Col
    if (a->IncRate[3] > 31) {
      seqBAIncCtl00.Bits.column_ba_updrate = Log2x32(a->IncRate[3] - 1);
      seqBAIncCtl00.Bits.column_ba_updscale = 0;
    } else {
//      seqBAIncCtl00.Bits.column_ba_updrate = 128 + a->IncRate[3];
      seqBAIncCtl00.Bits.column_ba_updrate = a->IncRate[3];
      seqBAIncCtl00.Bits.column_ba_updscale = 1;
    }
    seqBAIncCtl00.Bits.column_ba_inc = a->IncVal[3] & 0x7F;

    MemWritePciCfgEp (socket, ch, CPGC_SEQ_BAINCCTL00_MC_MAIN_REG, seqBAIncCtl00.Data);
    MemWritePciCfgEp (socket, ch, CPGC_SEQ_BAINCCTL10_MC_MAIN_REG, seqBAIncCtl10.Data);

    //
    // Program Write Data Buffer Related Entries
    //
    patWDBCLCtl.Data = 0;
    patWDBCLCtl.Bits.incr_rate  = WDBPattern->IncRate;
    patWDBCLCtl.Bits.start_pntr = WDBPattern->Start;
    patWDBCLCtl.Bits.end_pntr   = WDBPattern->Stop;
    MemWritePciCfgEp (socket, ch, CPGC_PATWDBCLCTL_MCDDC_CTL_REG, patWDBCLCtl.Data);

    //MemWritePciCfgEp (socket, ch, CPGC_PATWDB_WR0_MCDDC_CTL_HSX_REG, 0x55555555);
    //MemWritePciCfgEp (socket, ch, CPGC_PATWDB_WR1_MCDDC_CTL_HSX_REG, 0xAAAAAAAA);

    // Enable LMN in either LMN mode or CADB -to create lots of supply noise
    Mux0 = ((WDBPattern->DQPat == LMNVa) || (WDBPattern->DQPat == CADB)) ? LMNMode : LFSRMode;

    // ECC, Select LFSR
    patWDBCLMuxCtl.Data = 0;
    patWDBCLMuxCtl.Bits.mux0 = Mux0;
    patWDBCLMuxCtl.Bits.mux1 = LFSRMode;
    patWDBCLMuxCtl.Bits.mux2 = LFSRMode;
    patWDBCLMuxCtl.Bits.ecc_datasrc_sel = 1;
    MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUXCTL_MCDDC_DP_REG, patWDBCLMuxCtl.Data);

    // Currently, not planning to use the Inverstion Mask
    MemWritePciCfgEp (socket, ch, CPGC_PATWDB_INV_MCDDC_DP_REG, 0);

    // Write these registers when you first setup test
    MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX0_PBWR_MCDDC_DP_REG, 0x0EA10CA1);
    MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX1_PBWR_MCDDC_DP_REG, 0xDEAD0A1E);
    MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX2_PBWR_MCDDC_DP_REG, 0xBEEF0D08);
    MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX0_PBRD_MCDDC_DP_REG, 0x0EA10CA1);
    MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX1_PBRD_MCDDC_DP_REG, 0xDEAD0A1E);
    MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX2_PBRD_MCDDC_DP_REG, 0xBEEF0D08);

    //
    // Program Error Checking
    //

    //
    // Enable selective_error_enable_chunk and selective_error_enable_cacheline, mask later
    // the bits we don't want to check.
    //
    errCtl.Data                         = 0;
    errCtl.Bits.stop_on_ntherr          = 1;
    errCtl.Bits.stop_on_errcontrol      = SOE;
    errCtl.Bits.selective_err_enb_chunk = 0xFF;
    if ((CmdPat == PatWrRdFlyb) || (CmdPat == PatRdWrRd)) {
      errCtl.Bits.selective_err_enb_cl    = 0xF0;
    } else {
      errCtl.Bits.selective_err_enb_cl    = 0xFF;
    }
    MemWritePciCfgEp (socket, ch, CPGC_ERR_CTL_MCDDC_CTL_REG, errCtl.Data);

    //
    // Do not mask off errors
    //
    MemWritePciCfgEp (socket, ch, CPGC_ERR_DATA0_MCDDC_CTL_REG, 0);

    DataMask = 0;
#ifdef DDR5_SUPPORT
    if (IsDdr5Present (Host, socket)) {
      // Account for CPGC mapping
      DataMask = 0xFFFFFFFF;

      ErrCtl2.Data = MemReadPciCfgEp (socket, ch, CPGC_ERR_CTL2_MCDDC_DP_REG);
      ErrCtl2.Bits.selective_err_enb_chunk2 = 0xFF;
      MemWritePciCfgEp (socket, ch, CPGC_ERR_CTL2_MCDDC_DP_REG, ErrCtl2.Data);

    }
#endif // DDR5_SUPPORT
    MemWritePciCfgEp (socket, ch, CPGC_ERR_DATA1_MCDDC_CTL_REG, DataMask);

    //
    // Do not check ECC bits if ECC is disabled
    //
    if (Host->nvram.mem.eccEn) {
      MemWritePciCfgEp (socket, ch, CPGC_ERR_ECC_MCDDC_CTL_REG, 0);
    } else {
      MemWritePciCfgEp (socket, ch, CPGC_ERR_ECC_MCDDC_CTL_REG, 0xFF);
    }
  } // ch loop

  CteDelayDclk (800);
  // Always do a ZQ Short before the beginning of a test
  //CpgcIssueZQ (Host, socket, 0);

} // SetupIOTest

/**

  Setup IO Test Basic VA

  @param Host      - Pointer to sysHost
  @param socket    - Socket from which we are going to read the registers
  @param ChbitMask - ch Bit mask for which test should be setup for.
  @param LC        - Loop Count
  @param SOE       - [0: Never Stop, 1: Stop on Any Lane, 2:Stop on All Byte, 3: Stop on All Lane]
  @param EnCADB    - Enable test to write random deselect packages on bus to create xtalk/isi
  @param EnCKE     - Enable CKE power down by adding 64
  @param Spread    - Pattern Spread

  @retval N/A

**/
VOID
SetupIOTestBasicVA (
                  PSYSHOST  Host,
                  UINT8     socket,
                  UINT32    ChbitMask,
                  UINT8     LC,
                  UINT8     SOE,
                  UINT8     EnCADB,
                  UINT8     EnCKE,
                  UINT32    Spread
                  )
{
  TCPGCAddress CPGCAddress = {{0, 0, 0, 0},    // Start
    {0, 0, 0, 1023}, // Stop
    {0, 0, 0, 0},    // Order
    {511, 0, 0, 0},  // IncRate
    {1, 0, 0, 1}};   // IncValue
  //                             IncRate,  Start, Stop, DQPat
  TWDBPattern  WDBPattern = {     16,     0,    9, BasicVA};
  struct  TrainingVariable *TrainingVar;

  TrainingVar = MemTrainingVar ();

  WDBPattern.Stop = (Spread-1);

  SetupIOTest (Host, socket, ChbitMask, PatWrRd, 8, LC, &CPGCAddress, SOE, \
              &WDBPattern, EnCADB, EnCKE, 0);

  TrainingVar->DQPatLC = LC - 8 + 1;
  if (TrainingVar->DQPatLC < 1) {
    TrainingVar->DQPatLC = 1;
  }
  TrainingVar->DQPat = BasicVA;
} // SetupIOTestBasicVA10nm

/**

  Setup IO Test MPR

  @param Host      - Pointer to sysHost
  @param socket    - Socket from which we are going to read the registers
  @param ChbitMask - ch Bit mask for which test should be setup for.
  @param LC        - Loop Count
  @param SOE       - [0: Never Stop, 1: Stop on Any Lane, 2:Stop on All Byte, 3: Stop on All Lane]
  @param EnCADB    - Enable test to write random deselect packages on bus to create xtalk/isi
  @param EnCKE     - Enable CKE power down by adding 64

  @retval N/A

**/
VOID
SetupIOTestMPR (
              PSYSHOST  Host,
              UINT8     socket,
              UINT32    ChbitMask,
              UINT8     LC,
              UINT8     SOE,
              UINT8     EnCADB,
              UINT8     EnCKE
              )
{
  TCPGCAddress CPGCAddress = {{0, 0, 0, 0},    // Start
    {0, 0, 0, 1023}, // Stop
    {0, 0, 0, 0},    // Order
    {0, 0, 0, 0},    // IncRate
    {0, 0, 0, 1}};   // IncValue
//    {0, 0, 0, 0}};   // IncValue

  //                         IncRate,  Start, Stop,  DQPat
  //TWDBPattern  WDBPattern = { 32, 0, 9, BasicVA};
  TWDBPattern  WDBPattern = { 32, 0, 0, BasicVA};
  struct TrainingVariable *TrainingVar;

//  SetupIOTest (Host, socket, ChbitMask, PatRd, (1 << LC), LC, &CPGCAddress, SOE, &WDBPattern, EnCADB, EnCKE, 0);
  SetupIOTest (Host, socket, ChbitMask, PatRd, 32, LC, &CPGCAddress, SOE, &WDBPattern, EnCADB, EnCKE, 0);

  TrainingVar = MemTrainingVar ();

  TrainingVar->DQPatLC = 1;
  // if (Host->var.mem.socket[socket].DQPatLC < 1) Host->var.mem.socket[socket].DQPatLC = 1;
  TrainingVar->DQPat = BasicVA;
} // SetupIOTestMPR10nm

/**

  Runs one or more CPGC tests (based on TestType) and returns ch errors

  @param Host        - Pointer to sysHost
  @param socket      - Socket from which we are going to read the registers
  @param ChBitMask   - ch Bit mask for which test should be setup for.
  @param DQPat       - DQPat = [0: BasicVA, 1: SegmentWDB, 2: CADB, 3: TurnAround, 4: LMNVa]
  @param SeqLCs      - SeqLCs is an array of one or more loopcounts.
  @param ClearErrors - BOOLEAN to clear or not clear errors
  @param Mode        - mode allows for different types of modes for margining:

  @retval SUCCESS
  @retval FAILURE

**/
UINT32
RunIOTest (
         PSYSHOST  Host,
         UINT8     socket,
         UINT32    ChBitMask,
         UINT8     DQPat,
         UINT8     *SeqLCs,
         UINT8     ClearErrors,
         UINT16    Mode
         )
{
  UINT8   ch = 0;
  UINT8   Reload;
  UINT8   NumTests, t;
  UINT8   mcId;
  UINT32  TestRand;
  UINT32  TRand;
  CPGC_PATWDBCLCTL_MCDDC_CTL_STRUCT     patWDBCLCtl;
  CPGC_SUBSEQ0_CTL0_MC_MAIN_STRUCT      subSeqCtl0;
  CPGC_SEQCTL0_MC_MAIN_STRUCT           seqCtl0;
#ifdef DDR5_SUPPORT
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT CpgcPatCadbCtlReg;
#endif
// Runs one or more CPGC tests (based on TestType) and returns ch errors
// WDBIncRates is an array of one or more increments rates for the write data buffer
// SeqLCs is an array of one or more loopcounts.
// DQPat = [0: BasicVA, 1: SegmentWDB, 2: CADB, 3: TurnAround, 4: LMNVa]
// mode allows for different types of modes for margining:
//       mode is {Bit0: PhLock (keep all bytes within in ch in phase),
//               Bit1: Ch2Ch Data out of phase (LFSR seed), Bit 15:2: Reserved}

  UINT8 WDBIncRates[8] = {1, 1, 1, 1, 1, 1, 1, 1};
  UINT8 WDBStart[8]    = {0, 0, 0, 0, 0, 0, 0, 0};
  UINT8 WDBStop[8]     = {9, 9, 9, 9, 9, 9, 9, 9};
  UINT8 pollOnTestComplete = 1;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  RcDebugPrintWithDevice (SDBG_CPGC, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "RunIOTest Starts\n");

  TestRand     = 0xBAD00451;
  NumTests     = 1;

  if (DQPat == SegmentWDB) {
    NumTests = 4;
    // WDBIncRates[3:0]  = [25, 25, 19, 10]
    // WDBStart[3:2]     = 10
    // WDBStop[3:2]      = 63

    WDBIncRates[3] = 10;
    WDBIncRates[2] = 19;
    WDBIncRates[1] = 25;
    WDBIncRates[0] = 25;

    WDBStart[3]    = 10;
    WDBStart[2]    = 10;
    WDBStop[3]     = 63;
    WDBStop[2]     = 63;
  } else if (DQPat == CADB) {
    NumTests = 7;
  } else if (DQPat == TurnAroundWR) {
    NumTests = 8;
  } else if (DQPat == TurnAroundODT) {
    NumTests = 4;
  }
#ifdef DDR5_SUPPORT
  for (ch = 0; ch < MAX_CH; ch++) {
    if ( !((1 << ch) & ChBitMask )) {
      continue;
    }
    if (IsBrsPresent(Host, socket)) {
      CpgcPatCadbCtlReg.Data = MemReadPciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG);
      CpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 0;
      CpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 0;
      CpgcPatCadbCtlReg.Bits.enable_twopasscadb_always_on = 0;
      MemWritePciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG, CpgcPatCadbCtlReg.Data);
    }
  }
#endif
  for (t = 0; t < NumTests; t++) {
    Reload = Log2x32 (WDBIncRates[t]-1);
    if (WDBIncRates[t] > 31) {
      WDBIncRates[t] = Reload;
    }
    else                     WDBIncRates[t] += 32 - 1;

    for (ch = 0; ch < MaxChDdr; ch++) {

      if ( !((1 << ch) & ChBitMask )) {
        continue;
      }

      //
      // Check for SOE
      //
      MemReadPciCfgEp (socket, ch, CPGC_ERR_CTL_MCDDC_CTL_REG);

      if (Mode & BIT1) {
        // Circular shift by 9*ch
        TRand  = (TestRand >> (9 * ch)) | (TestRand << (32 - (9 * ch)));

        MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX0_PBWR_MCDDC_DP_REG, (0x0EA10CA1 ^ TRand));
        MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX1_PBWR_MCDDC_DP_REG, (0xDEAD0A1E ^ TRand));
        MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX2_PBWR_MCDDC_DP_REG, (0xBEEF0D08 ^ TRand));
        MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX0_PBRD_MCDDC_DP_REG, (0x0EA10CA1 ^ TRand));
        MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX1_PBRD_MCDDC_DP_REG, (0xDEAD0A1E ^ TRand));
        MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX2_PBRD_MCDDC_DP_REG, (0xBEEF0D08 ^ TRand));
      }

      if (DQPat == SegmentWDB) {
        patWDBCLCtl.Data = 0;
        patWDBCLCtl.Bits.incr_rate  = WDBIncRates[t];
        patWDBCLCtl.Bits.start_pntr = WDBStart[t];
        patWDBCLCtl.Bits.end_pntr   = WDBStop[t];
        MemWritePciCfgEp (socket, ch, CPGC_PATWDBCLCTL_MCDDC_CTL_REG, patWDBCLCtl.Data);

        mcId = GetMCID(Host, socket, ch);

        seqCtl0.Data = MemReadPciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4);
        MemWritePciCfgMC (socket, mcId, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG, 0x4);
        // Program desired loopcount
        seqCtl0.Bits.loopcount = ((SeqLCs[t] + 1) & 0x1F);
        // Set MRS Mode and start test
        MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, seqCtl0.Data);
      } else if (DQPat == CADB) {
        SetupCADB(Host, socket, ch, NumTests, t, 0);   // LMNEn=0
      }

      subSeqCtl0.Data = MemReadPciCfgMC_Ch (socket, ch, CPGC_SUBSEQ0_CTL0_MC_MAIN_REG, 0x50); // Reading Subseq
      mcId = GetMCID(Host, socket, ch);
      seqCtl0.Data = MemReadPciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4);
      if (seqCtl0.Bits.loopcount == 0 || (subSeqCtl0.Bits.num_cachelines==0 && seqCtl0.Bits.loopcount==1)) {
        pollOnTestComplete = 0;
      }

    } // ch loop

    //
    // Start Test and Poll on completion
    //

    IO_Reset (Host, socket);

    //
    // Global_Start_Test
    //
    //
    // Start test
    //
    CpgcGlobalTestStart (Host, socket, ChBitMask, NO_SUBCH);

    if (pollOnTestComplete) {
      //
      // Poll test completion
      //
      CpgcPollGlobalTestDone (Host, socket, ChBitMask, NO_SUBCH);
    } else {

      //
      // Delay if we don't poll/run in infinite loop
      //

      //
      // Delay if this is a CTE build, otherwise do nothing.
      //

      CteDelayDclk (500);

    }
  } // t loop

  RunIOTest10nmDelayTarget (Host, socket, ch);

  return SUCCESS;
} // RunIOTest

/**
  Programs CPGC to run on the selected logical ranks

  @param Host              - Pointer to sysHost
  @param socket            - Socket from which we are going to read the registers
  @param ch                - Channel
  @param RankBitMask       - Bit mask of logical ranks to work on
  @param RankFeatureEnable - RankFeatureEnable is a bit mask that can enable CKE, Refresh or ZQ
  @param SubRank           - Sub-Rank number when 3DS dimm. It applies to all the ranks specified by RankBitMask

  @retval 0
  @retval (UINT8) (1 << ch)

**/
UINT8
SelectCPGCRanks (
               PSYSHOST  Host,
               UINT8     socket,
               UINT8     ch,
               UINT8     RankBitMask,
               UINT8     RankFeatureEnable,
               UINT8     SubRank
               )
{
  UINT32                                    CRValue;
  UINT8                                     En;
  UINT8                                     dimm;
  UINT8                                     rank;
  UINT8                                     rankCount;
  UINT8                                     mcId;
  UINT8                                     trngTargetSelected;
  UINT8                                     NumChPerMc;
  CPGC_SEQCTL0_MC_MAIN_STRUCT               seqCtl0;
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_STRUCT  seqBaseAddrWrapHi0;
  CPGC_MISCREFCTL_MCDDC_CTL_STRUCT          miscRefCtl;
  CPGC_MISCZQCTL_MCDDC_CTL_STRUCT           miscZqCtl;
  CPGC_MISCCKECTL_MCDDC_CTL_STRUCT          miscCKECtl;
  CPGC_SEQ_LRRANKLG2PHYMAP0_MC_MAIN_STRUCT  seqLrRankLg2PhyMap0;
  struct dimmNvram                          (*dimmNvList)[MAX_DIMM];
  struct channelNvram                       (*channelNvList)[MAX_CH];
  UINT8                                     ddrtTarget = 0;
  ALIAS_CPGC_GLOBALCTRL_MC_MAIN_STRUCT      cpgcGlobalCtrl;

  RcDebugPrintWithDevice (SDBG_CPGC, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "SelectCPGCRanks Starts\n");

  channelNvList = GetChannelNvList(Host, socket);
  NumChPerMc    = GetNumChannelPerMc ();
  dimmNvList    = GetDimmNvList(Host, socket, ch);
  mcId          = GetMCID(Host, socket, ch);

  // Programs CPGC to run on the selected logical ranks
  // RankFeatureEnable is a bit mask that can enable CKE, Refresh or ZQ
  // RankFeatureEnable[0] enables Refresh on all non-selected ranks
  // RankFeatureEnable[1] enables Refresh on all ranks
  // RankFeatureEnable[2] enables ZQ on all non-selected ranks
  // RankFeatureEnable[3] enables ZQ on all ranks
  // RankFeatureEnable[4] enables CKE on all non-selected ranks
  // RankFeatureEnable[5] enables CKE on all ranks

  // Check if nothing is selected
  if ((RankBitMask & 0xFF) == 0) {
    MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQ_RANKLG2PHYMAP0_MC_MAIN_REG, 4, 0);

    if ((Host->nvram.mem.socket[socket].channelList[ch].lrDimmPresent == 1)) {
      MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQ_LRRANKLG2PHYMAP0_MC_MAIN_REG, 4, 0);
    }

    // Disable ch by clearing global start bit in change config
    seqCtl0.Data = MemReadPciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4);
    seqCtl0.Bits.global_control = 0;
    MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, seqCtl0.Data);

    return 0;

  } else {
    // Normal case
    // Setup GPGC Test to iteration through appropriate ranks during test
    CRValue  = 0;
    rankCount = 0;

    // Prepare Rank Mapping and Max Rank
    trngTargetSelected = 0;
    for (rank = 0; rank < MAX_RANK_CH; rank++) { //rank in range(4):
      if (!((1 << rank) & RankBitMask)) {
        continue;
      }

      //Set TRNG_TARGET on selected NVMDIMM ranks; else clear TRNG_TARGET on selected DDR4 ranks
      //Note: SKX design assumes NVMDIMM and DDR4 ranks cannot be selected in the same CPGC sequence
      if ((*dimmNvList)[rank / MAX_RANK_DIMM].DcpmmPresent) {
        if (trngTargetSelected && (ddrtTarget == 0)) {
          RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_45);
        } else {
          ddrtTarget = 1;
        }
      } else {
        if (trngTargetSelected && (ddrtTarget == 1)) {
          RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_46);
        } else {
          ddrtTarget = 0;
        }
      }
      trngTargetSelected = 1;

      CRValue |= (rank << (4 * rankCount));
      rankCount += 1;
    } // rank loop

    cpgcGlobalCtrl.Data = MemReadPciCfgMC (socket, mcId, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG);

    if (ddrtTarget) {
      cpgcGlobalCtrl.Bits.ddrt_addr_mode |= (1 << (ch % NumChPerMc));
    } else {
      cpgcGlobalCtrl.Bits.ddrt_addr_mode &= ~(1 << (ch % NumChPerMc));
    }


    SetCpgcTrainTarget (socket, ch, (BOOLEAN)ddrtTarget);

    MemWritePciCfgMC (socket, mcId, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG, cpgcGlobalCtrl.Data);
    SetDdrtForceMode (socket, ch, (BOOLEAN)ddrtTarget);

    // Write New Rank Mapping and Max Rank
    MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQ_RANKLG2PHYMAP0_MC_MAIN_REG, 4, CRValue);


    dimmNvList  = GetDimmNvList(Host, socket, ch);
    for (rank = 0; rank < MAX_RANK_CH; rank++) { //rank in range(4):
      if (!((1 << rank) & RankBitMask)) {
        continue;
      }
      //
      // skip if not in encoded mode
      //
      if ((*channelNvList)[ch].encodedCSMode == 0) {
        continue;
      }

      // convert logical rank mask to dimm number 0/1 -> 0, 4/5 ->1, 6/7 ->2
      dimm = rank / 4;
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }

      if ((IsLrdimmPresent (socket, ch, dimm) && (*dimmNvList)[dimm].DcpmmPresent == 0) || ((*channelNvList)[ch].encodedCSMode == 2)) {
        seqLrRankLg2PhyMap0.Data = 0;
        //
        // figure out where to append the subranks
        //
        // if the dimm has 16 row address bits [0:15] start appending the subrank after a[15]
        if ((*dimmNvList)[dimm].numRowBits == 17) {
          seqLrRankLg2PhyMap0.Bits.lrdimm_rank_mapping = 4;
        } else if ((*dimmNvList)[dimm].numRowBits == 16) {
          seqLrRankLg2PhyMap0.Bits.lrdimm_rank_mapping = 2;
        } else if ((*dimmNvList)[dimm].numRowBits == 15) {
          seqLrRankLg2PhyMap0.Bits.lrdimm_rank_mapping = 1;
        }
        //add for 3DS support
        if ((*channelNvList)[ch].encodedCSMode == 2) {
          if ((*dimmNvList)[dimm].numRowBits  == 16) {
            seqLrRankLg2PhyMap0.Bits.lrdimm_rank_mapping = 2;
          }
        }

        seqLrRankLg2PhyMap0.Bits.lr0_mapping = SubRank;
        MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQ_LRRANKLG2PHYMAP0_MC_MAIN_REG, 4, seqLrRankLg2PhyMap0.Data);
      }
    } //rank

    seqBaseAddrWrapHi0.Data = MemReadPciCfgMC_Ch (socket, ch, CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_REG, 4);
    seqBaseAddrWrapHi0.Bits.rank_addr = rankCount - 1;
    MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_REG, 4, seqBaseAddrWrapHi0.Data);

    // Make sure channel is enabled
    seqCtl0.Data = MemReadPciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4);
    seqCtl0.Bits.global_control = 1;
    MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, seqCtl0.Data);
  }

  //<VM>- Need to convert RankFeatureEnable as an input parameter so we don't pass it all the time
  if (RankFeatureEnable != 0) {

    //
    // Enable Refresh and ZQ - 0's to the the desired ranks
    //
    // Refresh
    En = RankFeatureEnable & 0x3;
    miscRefCtl.Data = 0;
    if (En == 1) {
      miscRefCtl.Bits.ref_rankmask &= ~RankBitMask; // Enable all non-selected ranks
    } else if (En > 1) {
      miscRefCtl.Bits.ref_rankmask &= 0; // Enable all ranks
    }
    MemWritePciCfgEp (socket, ch, CPGC_MISCREFCTL_MCDDC_CTL_REG, miscRefCtl.Data);

    //
    // ZQ
    //
    En = (RankFeatureEnable >> 2) & 0x3;
    miscZqCtl.Data = 0;
    miscZqCtl.Bits.always_do_zq = 0;
    miscZqCtl.Bits.zq_rankmask = 0xF;
    if (En == 1) {
      miscZqCtl.Bits.zq_rankmask &= ~RankBitMask; // Enable all non-selected ranks
    } else if (En > 1) {
      miscZqCtl.Bits.zq_rankmask &= 0; // Enable all ranks
    }
    MemWritePciCfgEp (socket, ch, CPGC_MISCZQCTL_MCDDC_CTL_REG, miscZqCtl.Data);

    //
    // Enable CKE ranks - 1's to enable desired ranks
    //
    En = (RankFeatureEnable >> 4) & 0x3;
    if(!IsBrsPresent(Host, socket )){
    miscCKECtl.Data = MemReadPciCfgEp (socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG);
    }
    // Assume no override
    miscCKECtl.Bits.cke_on = 0xFF;
    if (En == 1) {
      // Enable all non-selected ranks
      miscCKECtl.Bits.cke_override = ~RankBitMask;
      miscCKECtl.Bits.cke_on = ~RankBitMask;
    } else if (En > 1) {
      // Enable all ranks.
      miscCKECtl.Bits.cke_override = 0xFF;
      miscCKECtl.Bits.cke_on = 0xFF;
    }
    if(!IsBrsPresent(Host, socket )){
    MemWritePciCfgEp (socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG, miscCKECtl.Data);
    }
  } else {
    /* TODO: Check if we need to clear these
    MemWritePciCfgEp (socket, ch, CPGC_MISCREFCTL_MCDDC_CTL_REG, 0);
    MemWritePciCfgEp (socket, ch, CPGC_MISCZQCTL_MCDDC_CTL_REG, 0);
    MemWritePciCfgEp (socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG, 0);
     */
  }

  return(UINT8) (1 << ch);

} // SelectGPGCRanks

/**

  Perform RPQ drain operation

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Ch              - Channel number
  @param[in] Dimm            - DIMM number
  @param[in] Rank            - Rank number

  @retval N/A

**/
VOID
RPQDrainCadbMRSMode (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      UINT8     Rank
  )
{
  CPGC_DDRT_MISC_CTL_MC_2LM_STRUCT      MiscCtlMc2LM;
  UINT32                                ReadCount;
  UINT32                                CRValueSave;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT      CpgcPatCadbCtlReg;
  CPGC_PATCADBMRS_MCDDC_CTL_STRUCT      PatCADBMrs;
  CPGC_SEQCTL0_MC_MAIN_STRUCT           SeqCtl0;
  PatCadbProg0                          ReadCommandsMRSModeLocal0[CADB_LINES];
  PatCadbProg1                          ReadCommandsMRSModeLocal1[CADB_LINES];
  struct channelNvram                   (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "RPQDrainCadbMRSMode10nm Starts\n");

  ReadCount = GetPmonCounter (Host, Socket, Ch);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "RPQ occupancy count is 0x%x\n", ReadCount);

  //
  // Issue single Rd at a time till Pmon count back to 0
  //
  while (ReadCount != 0) {
    CpgcPatCadbCtlReg.Data = MemReadPciCfgEp (Socket, Ch, CPGC_PATCADBCTL_MCDDC_CTL_REG);
    CpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
    CpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 0;
    CpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 0;
    MemWritePciCfgEp (Socket, Ch, CPGC_PATCADBCTL_MCDDC_CTL_REG, CpgcPatCadbCtlReg.Data);

    CopyMem (ReadCommandsMRSModeLocal0, ReadCommandsMRSMode0, CADB_LINES);
    CopyMem (ReadCommandsMRSModeLocal1, ReadCommandsMRSMode1, CADB_LINES);

    FixupCS (GetPhyRank (Dimm, Rank), 0, ReadCommandsMRSModeLocal0, ReadCommandsMRSModeLocal1, (*ChannelNvList)[Ch].encodedCSMode);
    ProgramCADB (Host, Socket, Ch, NO_SUBCH, 0, CADB_LINES, ReadCommandsMRSModeLocal0, ReadCommandsMRSModeLocal1);

    // Issuing only one read at a time
    PatCADBMrs.Data                     = 0;
    PatCADBMrs.Bits.mrs_gap             = 4;
    PatCADBMrs.Bits.mrs_start_pointer   = 0;
    PatCADBMrs.Bits.mrs_end_pointer     = 0;

    MemWritePciCfgEp (Socket, Ch, CPGC_PATCADBMRS_MCDDC_CTL_REG, PatCADBMrs.Data);

    CRValueSave = MemReadPciCfgMC_Ch (Socket, Ch, CPGC_SEQCTL0_MC_MAIN_REG, 4);
    SeqCtl0.Data                      = 0;
    SeqCtl0.Bits.initialization_mode  = MRS_Mode;
    MemWritePciCfgMC_Ch (Socket, Ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, SeqCtl0.Data);

    //
    // Delay only if this is a CTE build, otherwise do nothing.
    //
    CteDelayQclk (30);

    //
    // Start test and clear errors
    //
    SeqCtl0.Bits.local_clear_errs  = 1;
    SeqCtl0.Bits.local_start_test  = 1;
    MemWritePciCfgMC_Ch (Socket, Ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, SeqCtl0.Data);

    FixedDelayMicroSecond (10);

    //
    // Restore after MR command
    //
    SeqCtl0.Data = MemReadPciCfgMC_Ch (Socket, Ch, CPGC_SEQCTL0_MC_MAIN_REG, 4);
    SeqCtl0.Bits.local_start_test = 0;
    SeqCtl0.Bits.local_clear_errs = 1;
    MemWritePciCfgMC_Ch (Socket, Ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, SeqCtl0.Data);

    //
    // Delay only if this is a CTE build, otherwise do nothing.
    //
    CteDelayQclk (30);

    MemWritePciCfgMC_Ch (Socket, Ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, CRValueSave);

    //
    // Delay only if this is a CTE build, otherwise do nothing.
    //
    CteDelayQclk (30);

    // Check if more reads need to be issued
    ReadCount = GetPmonCounter (Host, Socket, Ch);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "RPQ occupancy count inside do-while is 0x%x\n", ReadCount);
  }

  //
  // Toggle force RPQ Drain
  //
  MiscCtlMc2LM.Data = MemReadPciCfgEp (Socket, Ch, CPGC_DDRT_MISC_CTL_MC_2LM_REG);
  MiscCtlMc2LM.Bits.cpgc_rpq_force_drain = 1;
  MemWritePciCfgEp (Socket, Ch, CPGC_DDRT_MISC_CTL_MC_2LM_REG, MiscCtlMc2LM.Data);

  //
  // Delay only if this is a CTE build, otherwise do nothing.
  //
  CteDelayQclk (50);

  MiscCtlMc2LM.Bits.cpgc_rpq_force_drain = 0;
  MemWritePciCfgEp (Socket, Ch, CPGC_DDRT_MISC_CTL_MC_2LM_REG, MiscCtlMc2LM.Data);
}

/**
  This function evaluates if MRS Stacking is enabled for DDR,
  DDRT or NVDIMMs

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number
  @param[in] Channel - Channel number
  @param[in] Dimm    - DIMM number

  @retval TRUE or FALSE
*/
BOOLEAN
IsMrsStackingEnabled (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm
  )
{
  struct dimmNvram (*DimmNvList)[MAX_DIMM];

  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  if (((FeaturePcdGet (MrsStackingDdr) == FALSE) && !(*DimmNvList)[Dimm].DcpmmPresent && !(*DimmNvList)[Dimm].NvDimmNPresent) ||
      ((FeaturePcdGet (MrsStackingDdrt) == FALSE) && (*DimmNvList)[Dimm].DcpmmPresent) ||
      ((FeaturePcdGet (MrsStackingNvdimm) == FALSE) && (*DimmNvList)[Dimm].NvDimmNPresent)) {
    return FALSE;
  }

  return TRUE;
}

/**

  this function writes to CADB

  @param Host            - include all the MRC data
  @param socket          - Memory Controller
  @param ch              - ch to send command to
  @param rank            - CS to send the command to
  @param Command         - 0: MRS, 1: REF, 2: PRE, 3: ACT, 4: WR, 5: RD, 6: ZQ, 7: NOP
  @param Bank            - MRS command to be sent
  @param Data            - Value to be sent
  @param Delay           - Delay in Dclocks

@retval UINT32 -  SUCCESS if the reset is succeded

**/
UINT32
WriteCADBCmd (
             PSYSHOST  Host,
             UINT8     socket,
             UINT8     ch,
             UINT8     dimm,
             UINT8     rank,
             UINT8     Command,
             UINT8     Bank,
             UINT32    Data,
             UINT8     Delay
             )
{
  UINT8                                 currentRow;
  UINT32                                status = SUCCESS;
  EFI_STATUS                            CteStatus;
  UINT32                                CRValueSave;
  PatCadbProg0                          patCADBProg0;
  PatCadbProg1                          patCADBProg1;
  CPGC_PATCADBMRS_MCDDC_CTL_STRUCT      patCADBMrs;
  CPGC_SEQCTL0_MC_MAIN_STRUCT           seqCtl0;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT      cpgcPatCadbCtlReg;
  CPGC_MISCCKECTL_MCDDC_CTL_STRUCT      miscCKECtl;
  struct ddrRank                        (*rankList)[MAX_RANK_DIMM];
  struct dimmNvram                      (*dimmNvList)[MAX_DIMM];
  struct channelNvram                   (*channelNvList)[MAX_CH];
  UINT32                                AddressMask;
  BootMode                              SysBootMode;

  SysBootMode = GetSysBootMode ();

  CalculateAddressMask (Host, socket, ch, dimm, &AddressMask);

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {

    INT32  FakeMrs = 0;

    if (MRS_CMD == Command) {

      if(Host->DdrioUltSupport){

        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
          "ULT MRS%d data 0x%04X\n", Bank, Data);
        CteDdrCmd ("mrs", 1 << ch, 1, (UINT32)rank, (INT32)Bank, (UINT32)Data); //set the sub channel mask as 1 for ddr4.
        return SUCCESS;

      } else {
        status = SUCCESS;
        CteStatus = CteGetOptions ("fakemrs", &FakeMrs);
        if (EFI_ERROR (CteStatus)) {
          if (EFI_UNSUPPORTED != CteStatus) {
            status = FAILURE;
          }
          FakeMrs = 0;
        }
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
          "MRS%d data 0x%04X, fake=%d\n", Bank, Data, FakeMrs);

        if (FakeMrs) {
          CteStatus = CteMrsWrite (ch, rank, Bank, Data);
          if (CteStatus != EFI_SUCCESS) {
            if (EFI_UNSUPPORTED != CteStatus) {
              status = FAILURE;
              RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                "WriteCADBCmd: Failure writing CteMrsWrite with status %d\n", CteStatus);
            }
          }
          return status;
        }
      }
    }
  }

#ifdef MRC_TRACE
  RcDebugPrintWithDevice (SDBG_TRACE, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                 "WriteCADBCmd Starts: Command 0x%02x, Bank 0x%02x, Data 0x%04x\n", Command, Bank, Data);
#endif // #ifdef MRC_TRACE

  rankList = GetRankNvList(Host, socket, ch, dimm);
  dimmNvList = GetDimmNvList (Host, socket, ch);
  channelNvList = GetChannelNvList(Host, socket);
  currentRow = 0;

  //
  // Delay if this is a CTE build, otherwise do nothing.
  //

  CteDelayDclk (10);  // this might be needed?: delay after changing init mode, before programming registers.

  //
  // Pointer will be dynamically incremented after a write to CADB_PROG register
  //
  MemWritePciCfgEp (socket, ch, CPGC_PATCADBWRPNTR_MCDDC_CTL_REG, 0);
  if (!IsMrsStackingEnabled (Host, socket, ch, dimm) ||
      (IsMrsStackingEnabled (Host, socket, ch, dimm) && ((Host->var.mem.socket[socket].cadbMRSMode == NORMAL_MRS) || (Host->var.mem.socket[socket].cadbMRSMode & EXECUTE_MRS))))
  {
    cpgcPatCadbCtlReg.Data = MemReadPciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG);
    cpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
    cpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 0;
    cpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 0;
    MemWritePciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG, cpgcPatCadbCtlReg.Data);
  }

  if (!IsMrsStackingEnabled (Host, socket, ch, dimm) ||
      (IsMrsStackingEnabled (Host, socket, ch, dimm) && ((Host->var.mem.socket[socket].cadbMRSMode == NORMAL_MRS) || (Host->var.mem.socket[socket].cadbMRSMode & LOAD_MRS))))
  {
    patCADBProg0.address = (Data & AddressMask);
    patCADBProg0.bank    = Bank;
    patCADBProg0.cid     = 0;
    patCADBProg0.pdatag  = 0;

    if (Command == RCW_CMD) {
      patCADBProg1.cs      = ~((1 << (*rankList)[rank].CSIndex) + (1 << ((*rankList)[rank].CSIndex + 1)));
    } else {
      patCADBProg1.cs      = ~(1 << (*rankList)[rank].CSIndex);
    }
    if (Command == RCW_CMD) {
      patCADBProg1.control = 0;
    } else {
      patCADBProg1.control = Command;
    }

    // zero out CID0/1 as CS's bits
    if ((*channelNvList)[ch].encodedCSMode == 2) {
      patCADBProg1.cs = patCADBProg1.cs  & ~(0xc<<(dimm*4));
    }

    patCADBProg1.odt = 0;
    patCADBProg1.cke = 0;
    miscCKECtl.Data = MemReadPciCfgEp (socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG);
    if ((miscCKECtl.Bits.cke_override == 0) || (miscCKECtl.Bits.cke_override & miscCKECtl.Bits.cke_on)) {
      patCADBProg1.cke = 0x3F;
    }

    // Turn off non-ddrt dimm CKE during DDRT S3 resume.
    if (SysBootMode == S3Resume) {
      if ((*dimmNvList)[dimm].DcpmmPresent == 1 && (miscCKECtl.Bits.cke_on != 0)) {
        if (dimm == 0) {
          patCADBProg1.cke = BIT0;
        }
        else {
          patCADBProg1.cke = BIT2;
        }
      }
    }

    //
    // Assert ACT bit
    //
    patCADBProg1.control |= BIT3;

    //
    // Calculate and set parity
    //
    patCADBProg1.par = CalculateCadbParity (channelNvList, &patCADBProg0, &patCADBProg1, ch);
  }

  if (IsMrsStackingEnabled (Host, socket, ch, dimm) && (Host->var.mem.socket[socket].cadbMRSMode & LOAD_MRS)) {
    ProgramCADB (Host, socket, ch, NO_SUBCH, Host->var.mem.socket[socket].cadbMRSIndex[ch], Host->var.mem.socket[socket].cadbMRSIndex[ch] + 1, &patCADBProg0, &patCADBProg1);
    if (Host->var.mem.socket[socket].cadbMRSIndex[ch] >= CADB_CACHELINE_LIMIT) {
      Host->var.mem.socket[socket].cadbMRSMode = EXECUTE_MRS;
    }
    Host->var.mem.socket[socket].cadbMRSIndex[ch]++;
  }

  if (!IsMrsStackingEnabled (Host, socket, ch, dimm) ||
      (IsMrsStackingEnabled (Host, socket, ch, dimm) && (Host->var.mem.socket[socket].cadbMRSMode == NORMAL_MRS)))
  {
    if (Host->var.mem.socket[socket].hostRefreshStatus && ((Command == RCW_CMD) || (Command == MRS_CMD) || (Command == ZQ_CMD))) {
      //
      // Insert deselect before RCW or MRS or ZQCL commands when Host refresh is enabled to make sure
      // we do not violate tRFC
      //
      ProgramCADB (Host, socket, ch, NO_SUBCH, currentRow, currentRow + 1, &patCADBProg0, &DeselectPattern1[0]);
      currentRow = currentRow + 1;
    }
    ProgramCADB (Host, socket, ch, NO_SUBCH, currentRow, currentRow + 1, &patCADBProg0, &patCADBProg1);
    currentRow = currentRow + 1;
    if (Host->var.mem.socket[socket].hostRefreshStatus && ((Command == RCW_CMD) || (Command == MRS_CMD) || (Command == ZQ_CMD))) {
      //
      // Add a deselect after RCW or MRS or ZQCL commands when Host refresh is enabled to make sure
      // we do not violate tMOD
      //
      ProgramCADB (Host, socket, ch, NO_SUBCH, currentRow, currentRow + 1, &patCADBProg0, &DeselectPattern1[0]);
      currentRow = currentRow + 1;
    }
  }

  if (!IsMrsStackingEnabled (Host, socket, ch, dimm) ||
      (IsMrsStackingEnabled (Host, socket, ch, dimm) && ((Host->var.mem.socket[socket].cadbMRSMode == NORMAL_MRS) || (Host->var.mem.socket[socket].cadbMRSMode & EXECUTE_MRS))))
  {
    //
    // Execute MRS Mode
    //
    if (Delay < 6) {
      Delay = 6;
    }

    patCADBMrs.Data                     = 0;
    if (Host->var.mem.socket[socket].hostRefreshStatus == 1) {
      patCADBMrs.Bits.mrs_gap             = 9;
    } else {
      patCADBMrs.Bits.mrs_gap = Delay;
    }

    patCADBMrs.Bits.mrs_start_pointer   = 0;
    if (IsMrsStackingEnabled (Host, socket, ch, dimm)) {
      if (Host->var.mem.socket[socket].cadbMRSIndex[ch] != 0) {
        patCADBMrs.Bits.mrs_end_pointer     = Host->var.mem.socket[socket].cadbMRSIndex[ch] - 1;
      } else {
        patCADBMrs.Bits.mrs_end_pointer     = 0;
      }
    } else {
      patCADBMrs.Bits.mrs_end_pointer     = currentRow - 1;
    }
    MemWritePciCfgEp (socket, ch, CPGC_PATCADBMRS_MCDDC_CTL_REG, patCADBMrs.Data);

    //
    // Save before MR command
    //
    CRValueSave = MemReadPciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4);
    seqCtl0.Data                      = 0;
    seqCtl0.Bits.initialization_mode  = MRS_Mode;
    MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, seqCtl0.Data);

    //
    // Delay only if this is a CTE build, otherwise do nothing.
    //

    CteDelayQclk (10);  //add delay before start after we changed init mode

    //
    // Start test and clear errors
    //
    seqCtl0.Bits.local_clear_errs  = 1;
    seqCtl0.Bits.local_start_test  = 1;
    MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, seqCtl0.Data);

    //
    // Poll test completion
    //
    CpgcPollGlobalTestDone (Host, socket, 1 << ch, NO_SUBCH);

    //
    // Restore after MR command
    //
    seqCtl0.Data = MemReadPciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4);
    seqCtl0.Bits.local_start_test = 0;
    seqCtl0.Bits.local_clear_errs = 1;
    MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, seqCtl0.Data);

    //
    // Delay if this is a CTE build, otherwise do nothing.
    //

    CteDelayDclk (10);  // this might be needed?

    MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, CRValueSave);

    if (IsMrsStackingEnabled (Host, socket, ch, dimm) && (Host->var.mem.socket[socket].cadbMRSIndex[ch] > CADB_CACHELINE_LIMIT)) {
      Host->var.mem.socket[socket].cadbMRSMode = LOAD_MRS;
      Host->var.mem.socket[socket].cadbMRSIndex[ch] = 0;
    }
  }

  return status;
} // WriteCADBCmd

/**

Routine Description:

  This function allows stacking of MRS commands in cadb buffer in WriteCADBCmd  function. Stacking commands in CADB
  is more efficient as the MRS are only ~32 clks apart instead of spread out by a bunch of CSR accesses

Arguments:
  Host            - include all the MRC data
  socket          - Memory Controller
  ch              - ch to send command to
  dimm            - dimm to send command to
  Command         - NORMAL_MRS  - How WriteCADBCmd  usually works
                  - LOAD_MRS - WriteCADBCmd  will load commands into cadb buffer but not execute
                  - EXECUTE_MRS - WriteCADBCmd  will execute the stack of commands


Returns:
 UINT32 -  SUCCESS if the reset is succeded

--*/
VOID
MRSCmdStacking (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     Command
  )
{
  if (IsMrsStackingEnabled (Host, socket, ch, dimm)) {
    Host->var.mem.socket[socket].cadbMRSMode = Command;

    if (Command == LOAD_MRS) {
      Host->var.mem.socket[socket].cadbMRSIndex[ch]=0;
    }
    if (Command == EXECUTE_MRS) {
      if(Host->var.mem.socket[socket].cadbMRSIndex[ch] != 0) {  //Check for exactly 16 CADB commands
        //WriteCADBCmd (Host, socket, ch, dimm, rank, Command, Bank, Data, Delay);
        WriteCADBCmd (Host, socket, ch, dimm, 0, 0, 0, 0, 6); //Delay =2**6 Need to support NVMCTLR
      }
      Host->var.mem.socket[socket].cadbMRSMode = NORMAL_MRS;                 // return to normal MRS mode
      Host->var.mem.socket[socket].cadbMRSIndex[ch]=0;                           // reset index after execute
    }
  }
} //MRSCmdStacking10nm

/**

  this function writes to CADB

  @param Host            - include all the MRC data
  @param socket          - Memory Controller
  @param ch              - ch to send command to
  @param dimm            - dimm to send command to
  @param rank            - CS to send the command to
  @param Command         - 0: MRS, 1: REF, 2: PRE, 3: ACT, 4: WR, 5: RD, 6: ZQ, 7: NOP
  @param Bank            - MRS command to be sent
  @param Data            - Value to be sent

**/
VOID
WriteMRSPDA (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     dram,
  UINT16    data,
  UINT8     bank
  )
{
#ifndef DDR5_SUPPORT
  UINT8                             side;
  UINT8                             maxSide;
  UINT8                             currentRow;
  UINT16                            tempPrk;
  UINT32                            CRValueSave = 0;
  PatCadbProg0                      patCADBProg0[7];
  PatCadbProg1                      patCADBProg1[7];
  CPGC_PATCADBMRS_MCDDC_CTL_STRUCT  patCADBMrs;
  CPGC_SEQCTL0_MC_MAIN_STRUCT       seqCtl0;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT  cpgcPatCadbCtlReg;
  CPGC_PDA_MCDDC_DP_STRUCT          cpgcPDA;
  CPGC_PDA_SHADOW_MCDDC_CTL_STRUCT  cpgcPDAShadow;
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                 (*rankStruct)[MAX_RANK_DIMM];
  struct channelNvram               (*channelNvList)[MAX_CH];
  BOOLEAN                           CpgcForDdrtOld;

  if (Host->var.mem.socket[socket].hostRefreshStatus == 1) {
    FixedDelayMicroSecond (10);
  }

  channelNvList = GetChannelNvList(Host, socket);
  rankList      = GetRankNvList(Host, socket, ch, dimm);
  rankStruct    = GetRankStruct(Host, socket, ch, dimm);

#ifdef MRC_TRACE
  RcDebugPrintWithDevice (SDBG_TRACE, socket, ch, dimm, NO_SUBCH, rank, dram, NO_BIT,
                 "DDR4 PDA write: bank 0x%02X, data 0x%04X\n", bank, data);
#endif //MRC_TRACE

  // Override PRK to WR
  tempPrk = (*rankStruct)[rank].MR5 & ~0x1c0; // zero out bits, 6,7,8 - RTT_PRK
  switch ((*rankList)[rank].RttWr >> DDR4_RTT_WR_ODT_SHIFT) {
    case 0:
      tempPrk |= DDR4_RTT_PARK_DIS<<6; // disable/off
      break;
    case 1:
      tempPrk |= DDR4_RTT_PARK_120<<6;   // PRK =120
      break;
    case 2:
      tempPrk |= DDR4_RTT_PARK_240<<6;   // PRK = 240
      break;
    case 3:
      tempPrk |= DDR4_RTT_PARK_INF<<6;  // PRK = disable
      break;
    default:
      //Automated add of default case. Please review.
      break;
  }
  WriteMRS (Host, socket, ch, dimm, rank, tempPrk, BANK5);

  // HSD 5330727 - PDA not driving DQ low when scheduler is set to DDRT target
  CpgcForDdrtOld = GetCpgcTrainTarget (socket, ch);
  SetCpgcTrainTarget (socket, ch, FALSE);

  //
  // Pointer will be dynamically incremented after a write to CADB_PROG register
  //
  MemWritePciCfgEp (socket, ch, CPGC_PATCADBWRPNTR_MCDDC_CTL_REG, 0);

  //
  // Save before MR command
  //
  CRValueSave = MemReadPciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4);
  seqCtl0.Data                      = 0;
  seqCtl0.Bits.initialization_mode  = MRS_Mode;
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, seqCtl0.Data);

  cpgcPatCadbCtlReg.Data = MemReadPciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG);
  cpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
  cpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 0;
  cpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 0;
  MemWritePciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG, cpgcPatCadbCtlReg.Data);

  if ((Host->nvram.mem.dimmTypePresent == RDIMM) && (bank != 7)) {
    maxSide = SIDE_B;
  } else {
    maxSide = SIDE_A;
  }

  cpgcPDA.Data       = 0;
  cpgcPDAShadow.Data = 0;

  for (side = SIDE_A; side <= maxSide; side++) {
    //
    // Select the device(s) to be written to
    //
    if (dram == ALL_DRAMS) {
      cpgcPDA.Bits.device_select = 0;
    } else {
      cpgcPDA.Bits.device_select = ~(1 << dram) & ((1 << 18) - 1);
    }

    //
    // Select the rank to be written to
    //
    cpgcPDA.Bits.rank_select        = (*rankList)[rank].logicalRank;
    cpgcPDAShadow.Bits.rank_select  = (*rankList)[rank].logicalRank;

    MemWritePciCfgEp (socket, ch, CPGC_PDA_MCDDC_DP_REG, cpgcPDA.Data);
    MemWritePciCfgEp (socket, ch, CPGC_PDA_SHADOW_MCDDC_CTL_REG, cpgcPDAShadow.Data);

    ZeroMem (patCADBProg0, sizeof (patCADBProg0));
    ZeroMem (patCADBProg1, sizeof (patCADBProg1));

    currentRow = 0;
    if (Host->var.mem.socket[socket].hostRefreshStatus) {
      //
      // Placeholder for deselect
      //
      currentRow = currentRow + 1;
    }

    //
    // Program CADB row 0 (or row 1 if hostRefreshStatus is enabled) to enter PDA mode
    //
    ProgCADBRow (Host, socket, ch, dimm, rank, BANK3, (*rankStruct)[rank].MR3 | MR3_PDA, side, ENTER_PDA, MRS_CMD, &patCADBProg0[currentRow], &patCADBProg1[currentRow]);
    currentRow = currentRow + 1;

    // Enable VrefDQ Training for MR6
    if (bank == BANK6) {
      //
      // Program CADB row 1 (or row 2 if hostRefreshStatus is enabled) to enter Vref training mode
      //
      ProgCADBRow (Host, socket, ch, dimm, rank, bank, data | BIT7, side, ENTER_PDA, MRS_CMD, &patCADBProg0[currentRow], &patCADBProg1[currentRow]);
      currentRow = currentRow + 1;

      //
      // Program CADB row 2 (or row 3 if hostRefreshStatus is enabled) to execute the PDA write
      //
      ProgCADBRow (Host, socket, ch, dimm, rank, bank, data | BIT7, side, ENTER_PDA, MRS_CMD, &patCADBProg0[currentRow], &patCADBProg1[currentRow]);
      currentRow = currentRow + 1;

      //
      // Program CADB row 3 (or row 4 if hostRefreshStatus is enabled) to exit Vref training mode
      //
      ProgCADBRow (Host, socket, ch, dimm, rank, bank, data & ~BIT7, side, ENTER_PDA, MRS_CMD, &patCADBProg0[currentRow], &patCADBProg1[currentRow]);
      currentRow = currentRow + 1;
    } else {
      ProgCADBRow (Host, socket, ch, dimm, rank, bank, data, side, ENTER_PDA, MRS_CMD, &patCADBProg0[currentRow], &patCADBProg1[currentRow]);
      currentRow = currentRow + 1;
    }

    //
    // Program CADB row 4 (or row 5 if hostRefreshStatus is enabled) to exit PDA mode
    //
    ProgCADBRow (Host, socket, ch, dimm, rank, BANK3, (*rankStruct)[rank].MR3, side, EXIT_PDA, MRS_CMD, &patCADBProg0[currentRow], &patCADBProg1[currentRow]);
    currentRow = currentRow + 1;

    if (Host->var.mem.socket[socket].hostRefreshStatus) {
      //
      // Placeholder for deselect
      //
      currentRow = currentRow + 1;
    }

    ProgramCADB (Host, socket, ch, NO_SUBCH, 0, currentRow, patCADBProg0, patCADBProg1);       // program actual CADB registers

    if (Host->var.mem.socket[socket].hostRefreshStatus) {
      //
      // Add a deselect before and after MRS commands when Host controlled refreshes are enabled to make sure
      // we do not violate timings
      //
      ProgramCADB (Host, socket, ch, NO_SUBCH, 0, 1, &patCADBProg0[0], &DeselectPattern1[0]);
      ProgramCADB (Host, socket, ch, NO_SUBCH, currentRow - 1, currentRow, &patCADBProg0[0], &DeselectPattern1[0]);
    }

    patCADBMrs.Data = 0;
      //
      // Use a larger gap when refreshes are enabled to ensure we do not violate refresh timings
      //
    patCADBMrs.Bits.mrs_gap = 9;

    patCADBMrs.Bits.mrs_start_pointer = 0;
    patCADBMrs.Bits.mrs_end_pointer = currentRow - 1;
    MemWritePciCfgEp (socket, ch, CPGC_PATCADBMRS_MCDDC_CTL_REG, patCADBMrs.Data);

    // Start test and clear errors
    seqCtl0.Bits.local_clear_errs  = 1;
    seqCtl0.Bits.local_start_test  = 1;
    MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, seqCtl0.Data);

    //
    // Poll test completion
    //
    CpgcPollGlobalTestDone (Host, socket, 1 << ch, NO_SUBCH);

  } // side loop

  //
  // Restore after MR command
  //
  seqCtl0.Data = MemReadPciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4);
  seqCtl0.Bits.local_start_test = 0;
  seqCtl0.Bits.local_clear_errs = 1;
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, seqCtl0.Data);
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, CRValueSave);

  // HSD 5330727 - PBA not driving DQ low when scheduler is set to DDRT target
  // Restore original value
  SetCpgcTrainTarget (socket, ch, CpgcForDdrtOld);

  //Undo PRK override
  WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR5, BANK5);
#endif // !DDR5_SUPPORT
} // WriteMRSPDA10nm

VOID
WriteMR6 (
PSYSHOST  Host,
UINT8     socket,
UINT8     ch,
UINT8     dimm,
UINT8     rank,
UINT16    data,
UINT8     bank
)
/*++

this function writes to CADB

@param Host            - include all the MRC data
@param socket          - Memory Controller
@param ch              - ch to send command to
@param dimm            - dimm to send command to
@param rank            - CS to send the command to
@param Bank            - MRS command to be sent
@param Data            - Value to be sent

--*/
{
#ifndef DDR5_SUPPORT
  UINT8                                   side;
  UINT8                                   maxSide;
  UINT8                                   currentRow;
  UINT16                                  tempPrk;
  UINT32                                  CRValueSave = 0;
  struct ddrRank(*rankList)[MAX_RANK_DIMM];
  struct rankDevice(*rankStruct)[MAX_RANK_DIMM];
  PatCadbProg0                            patCADBProg0[16];
  PatCadbProg1                            patCADBProg1[16];
  CPGC_PATCADBMRS_MCDDC_CTL_STRUCT        patCADBMrs;
  CPGC_SEQCTL0_MC_MAIN_STRUCT             seqCtl0;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT        cpgcPatCadbCtlReg;
  struct channelNvram(*channelNvList)[MAX_CH];
  BOOLEAN                                 CpgcForDdrtOld;


  if (Host->var.mem.socket[socket].hostRefreshStatus == 1) {
    FixedDelayMicroSecond (10);
  }
  channelNvList = GetChannelNvList(Host, socket);
  rankList = GetRankNvList(Host, socket, ch, dimm);
  rankStruct = GetRankStruct(Host, socket, ch, dimm);

#ifdef MRC_TRACE
  RcDebugPrintWithDevice (SDBG_TRACE, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
    "DDR4 PDA write: bank 0x%02X, data 0x%04X\n", bank, data);
#endif //MRC_TRACE

  //override PRK to WR
  tempPrk = (*rankStruct)[rank].MR5 & ~0x1c0; // zero out bits, 6,7,8 - RTT_PRK
  switch ((*rankList)[rank].RttWr >> DDR4_RTT_WR_ODT_SHIFT) {
  case 0:
    tempPrk |= DDR4_RTT_PARK_DIS << 6; // disable/off
    break;
  case 1:
    tempPrk |= DDR4_RTT_PARK_120 << 6;   // PRK =120
    break;
  case 2:
    tempPrk |= DDR4_RTT_PARK_240 << 6;   // PRK = 240
    break;
  case 3:
    tempPrk |= DDR4_RTT_PARK_INF << 6;  // PRK = disable
    break;
    default:
      //Automated add of default case. Please review.
      break;
  }
  WriteMRS(Host, socket, ch, dimm, rank, tempPrk, BANK5);

  // HSD 5330727 - PDA not driving DQ low when scheduler is set to DDRT target
  CpgcForDdrtOld = GetCpgcTrainTarget (socket, ch);
  SetCpgcTrainTarget (socket, ch, FALSE);

  //
  // Pointer will be dynamically incremented after a write to CADB_PROG register
  //
  MemWritePciCfgEp (socket, ch, CPGC_PATCADBWRPNTR_MCDDC_CTL_REG, 0);

  //
  // Save before MR command
  //
  CRValueSave = MemReadPciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4);
  seqCtl0.Data = 0;
  seqCtl0.Bits.initialization_mode = MRS_Mode;
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, seqCtl0.Data);

  cpgcPatCadbCtlReg.Data = MemReadPciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG);
  cpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
  cpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 0;
  cpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 0;
  MemWritePciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG, cpgcPatCadbCtlReg.Data);

  if ((Host->nvram.mem.dimmTypePresent == RDIMM) && (bank != 7)) {
    maxSide = SIDE_B;
  } else {
    maxSide = SIDE_A;
  }

  for (side = SIDE_A; side <= maxSide; side++) {

    ZeroMem (patCADBProg0, sizeof (patCADBProg0));
    ZeroMem (patCADBProg1, sizeof (patCADBProg1));

    currentRow = 0;
    if (Host->var.mem.socket[socket].hostRefreshStatus) {
      //
      // Placeholder for deselect on entry
      //
      currentRow = currentRow + 1;
    }

    //
    // Program CADB next row to execute (row 0 in case of normal or row 1 when hostRefreshStatus is enabled)
    //
    ProgCADBRow(Host, socket, ch, dimm, rank, bank, data | BIT7, side, NO_PDA, MRS_CMD, &patCADBProg0[currentRow], &patCADBProg1[currentRow]);
    currentRow = currentRow + 1;

    //
    // Program CADB next row to execute (row 1 in case of normal or row 2 when hostRefreshStatus is enabled)
    //
    ProgCADBRow(Host, socket, ch, dimm, rank, bank, data | BIT7, side, NO_PDA, MRS_CMD, &patCADBProg0[currentRow], &patCADBProg1[currentRow]);
    currentRow = currentRow + 1;

    //
    // Program CADB next row to execute (row 2 in case of normal or row 3 when hostRefreshStatus is enabled)
    //
    ProgCADBRow(Host, socket, ch, dimm, rank, bank, data & ~BIT7, side, NO_PDA, MRS_CMD, &patCADBProg0[currentRow], &patCADBProg1[currentRow]);
    currentRow = currentRow + 1;

    if (Host->var.mem.socket[socket].hostRefreshStatus) {
      //
      // Placeholder for deselect on exit
      //
      currentRow = currentRow + 1;
    }

    ProgramCADB (Host, socket, ch, NO_SUBCH, 0, currentRow, patCADBProg0, patCADBProg1);

    if (Host->var.mem.socket[socket].hostRefreshStatus) {

      //
      // Add a deselect after RCW or MRS commands when Host controlled refreshes are enabled to make sure
      // we do not violate tMOD
      //
      ProgramCADB (Host, socket, ch, NO_SUBCH, 0, 1, &patCADBProg0[0], &DeselectPattern1[0]);
      ProgramCADB (Host, socket, ch, NO_SUBCH, currentRow - 1, currentRow, &patCADBProg0[0], &DeselectPattern1[0]);
    }

    //
    // Execute MRS Mode
    //
    patCADBMrs.Data = 0;
    patCADBMrs.Bits.mrs_gap = 9;

    patCADBMrs.Bits.mrs_start_pointer = 0;
    patCADBMrs.Bits.mrs_end_pointer = currentRow - 1;
    MemWritePciCfgEp (socket, ch, CPGC_PATCADBMRS_MCDDC_CTL_REG, patCADBMrs.Data);

    // Start test and clear errors
    seqCtl0.Bits.local_clear_errs = 1;
    seqCtl0.Bits.local_start_test = 1;
    MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, seqCtl0.Data);

    //
    // Poll test completion
    //
    CpgcPollGlobalTestDone (Host, socket, 1 << ch, NO_SUBCH);
  } // side loop

  //
  // Restore after MR command
  //
  seqCtl0.Data = MemReadPciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4);
  seqCtl0.Bits.local_start_test = 0;
  seqCtl0.Bits.local_clear_errs = 1;
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, seqCtl0.Data);
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, CRValueSave);

  // HSD 5330727 - PBA not driving DQ low when scheduler is set to DDRT target
  // Restore original value
  SetCpgcTrainTarget (socket, ch, CpgcForDdrtOld);

  //Undo PRK override
  WriteMRS(Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR5, BANK5);
#endif // !DDR5_SUPPORT
} // WriteMR610nm


#ifdef LRDIMM_SUPPORT

STATIC INT16
GetMinDwl (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm
)
{
  INT16                                   cwlAdj;

  cwlAdj = -3; // write absolute minimum CWL_ADJ of -3

  return cwlAdj;
}

VOID
WritePBA (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     Dimm,
  UINT8     rank,
  UINT8     *controlWordDataPtr,
  UINT8     controlWordFunc,
  UINT8     controlWordAddr,
  UINT8     bufferAddr
  )
/*++

Routine Description:

  this function writes to CADB

Arguments:
  Host               - include all the MRC data
  socket             - Memory Controller
  ch                 - ch to send command to
  dimm               - dimm to send command to
  rank               - CS to send the command to
  controlWordDataPtr - Pointer to a 9 byte array of control word data
  controlWordFunc    - Function index in the LR buffer
  controlWordAddr    - Control Word Address
  bufferAddr         - Address of the individual buffer to target...or 'FF' for all buffers

Returns:
  N/A

--*/
{
#ifndef DDR5_SUPPORT
  UINT32                                  CRValueSave = 0;
  struct dimmNvram                        (*dimmNvList)[MAX_DIMM];
  struct ddrRank                          (*rankList)[MAX_RANK_DIMM];
  struct channelNvram                     (*channelNvList)[MAX_CH];
  PatCadbProg0                            patCADBProg0[3];
  PatCadbProg1                            patCADBProg1[3];
  CPGC_PATCADBMRS_MCDDC_CTL_STRUCT        patCADBMrs;
  CPGC_PDA_MCDDC_DP_STRUCT                cpgcPDA;
  CPGC_PDA_SHADOW_MCDDC_CTL_STRUCT        cpgcPDAShadow;
  CPGC_SEQCTL0_MC_MAIN_STRUCT             seqCtl0;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT        cpgcPatCadbCtlReg;
  UINT16                                  tempData;
  UINT8                                   index;
  UINT8                                   maxIndex;
  INT16                                   CRAddDelay;
  INT16                                   CRAddDelayOrg;
  UINT8                                   MSVx8 ;
  BOOLEAN                                 CpgcForDdrtOld;
  UINT8                                   SetType;
  UINT32                                  CurrentTxOn = 0;

  MSVx8 = GetMaxStrobeValid(Host) / 2;

  RcDebugPrintWithDevice (SDBG_CPGC, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "WritePBA10nm Starts\n");

  channelNvList = GetChannelNvList(Host, socket);
  dimmNvList  = GetDimmNvList(Host, socket, ch);
  rankList    = GetRankNvList(Host, socket, ch, Dimm);

  // override PRK to WR
  WriteLrbuf(Host, socket, ch, Dimm, 0, (*dimmNvList)[Dimm].lrBuf_BC01, LRDIMM_F0, LRDIMM_BC02);

  if ((*channelNvList) [ch].dimmList[Dimm].DcpmmPresent) {
    SetType = CWLADJ_DDRT;
  } else {
    SetType = CWLADJ_DDR4;
  }
  GetCwlAdj(Host, socket, ch, Dimm, &CRAddDelayOrg);
  CRAddDelay = GetMinDwl (Host, socket, ch, Dimm);
  SetCwlAdj (Host, socket, ch, SetType, CRAddDelay);

  //
  // Force TxOn
  //
  CurrentTxOn = SetDataTxOnDdrio (Host, socket, ch, 1);

  // HSD 5330727 - PBA not driving DQ low when scheduler is set to DDRT target
  CpgcForDdrtOld = GetCpgcTrainTarget (socket, ch);
  SetCpgcTrainTarget (socket, ch, FALSE);

  //
  // Pointer will be dynamically incremented after a write to CADB_PROG register
  //
  MemWritePciCfgEp (socket, ch, CPGC_PATCADBWRPNTR_MCDDC_CTL_REG, 0);

  //
  // Save before MR command
  //
  CRValueSave = MemReadPciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4);
  seqCtl0.Data                      = 0;
  seqCtl0.Bits.initialization_mode  = MRS_Mode;
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, seqCtl0.Data);

  cpgcPatCadbCtlReg.Data = MemReadPciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG);
  cpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
  cpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 0;
  cpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 0;
  cpgcPatCadbCtlReg.Bits.pda_length = 2;

  MemWritePciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG, cpgcPatCadbCtlReg.Data);

  cpgcPDA.Data = 0;
  cpgcPDAShadow.Data = 0;

  cpgcPDA.Bits.device_select = 0;
  //
  // Select the rank to be written to
  //
  cpgcPDA.Bits.rank_select        = (*rankList)[rank].logicalRank;
  cpgcPDAShadow.Bits.rank_select  = (*rankList)[rank].logicalRank;

  MemWritePciCfgEp (socket, ch, CPGC_PDA_MCDDC_DP_REG, cpgcPDA.Data);
  MemWritePciCfgEp (socket, ch, CPGC_PDA_SHADOW_MCDDC_CTL_REG, cpgcPDAShadow.Data);

  //
  // Program CADB row 0, BCW change function (func 0)
  //
  ProgCADBRow (Host, socket, ch, Dimm, rank, BANK7, (BIT12 | (LRDIMM_BC7x << 4)), SIDE_A, NO_PDA, MRS_CMD, &patCADBProg0[0], &patCADBProg1[0]);
  //
  // Program CADB row 1 to enter PBA mode
  //
  ProgCADBRow (Host, socket, ch, Dimm, rank, BANK7, (BIT12 | (LRDIMM_BC1x << 4) | ((*dimmNvList)[Dimm].lrBuf_BC1x & LRDIMM_ONE_RANK_TIMING_MODE) | LRDIMM_PBA_MODE), SIDE_A, ENTER_PDA, MRS_CMD, &patCADBProg0[1], &patCADBProg1[1]);
  //
  // Program CADB row 2, BCW change function (input func)
  //

  ProgCADBRow (Host, socket, ch, Dimm, rank, BANK7, (BIT12 | (LRDIMM_BC7x << 4) | controlWordFunc), SIDE_A, EXIT_PDA, MRS_CMD, &patCADBProg0[2], &patCADBProg1[2]);

  ProgramCADB (Host, socket, ch, NO_SUBCH, 0, 3, patCADBProg0, patCADBProg1);

  //
  // Execute MRS Mode
  //
  patCADBMrs.Data                     = 0;
  patCADBMrs.Bits.mrs_gap             = 5;
  patCADBMrs.Bits.mrs_start_pointer   = 0;
  patCADBMrs.Bits.mrs_end_pointer     = 2;
  MemWritePciCfgEp (socket, ch, CPGC_PATCADBMRS_MCDDC_CTL_REG, patCADBMrs.Data);

  //
  // Delay only if this is a CTE build, otherwise do nothing.
  //

  CteDelayQclk (10); // slight delay to allow everything to settle before starting.

  // Start test and clear errors
  seqCtl0.Bits.local_clear_errs  = 1;
  seqCtl0.Bits.local_start_test  = 1;
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, seqCtl0.Data);

  //
  // Poll test completion
  //
  CpgcPollGlobalTestDone (Host, socket, 1 << ch, NO_SUBCH);

  //
  // Execute MRS Mode (2 rows)
  //
  patCADBMrs.Data                     = 0;
  patCADBMrs.Bits.mrs_gap             = 5;
  patCADBMrs.Bits.mrs_start_pointer   = 0;
  patCADBMrs.Bits.mrs_end_pointer     = 1;
  MemWritePciCfgEp (socket, ch, CPGC_PATCADBMRS_MCDDC_CTL_REG, patCADBMrs.Data);

  //PBA Mode enabled, now loop through all 9 buffers
  if (bufferAddr == ALL_DATABUFFERS) {
    index = 0;
    maxIndex = MSVx8;
  } else if (bufferAddr > MSVx8){
#ifdef DEBUG
    RcDebugPrint (SDBG_DEFAULT, "\nERROR: WritePBA () - IVALID PARAMETER\n");
#endif //DEBUG
    return;
  } else {
    index = bufferAddr;
    maxIndex = bufferAddr +1;
  }

  //
  // controlWordDataPtr is a pointer to a size 9 array
  //
  if (maxIndex > (MAX_STROBE/2)) {
    return;
  }

  for (; index < maxIndex; index++) {
    //RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, index, NO_BIT,
    //             "#Debug DDR4 PBA write: Func 0x%02X, Addr 0x%02X,  Data 0x%02X\n", controlWordFunc, controlWordAddr, controlWordDataPtr[index]);

    if ((bufferAddr != ALL_DATABUFFERS) && (bufferAddr != index)) {
      continue;
    }
    cpgcPDA.Bits.device_select      = ~(3 << (index*2)) & ((1 << 18) - 1);
    //cpgcPDA.Bits.rank_select        = (*rankList)[rank].logicalRank;
    MemWritePciCfgEp (socket, ch, CPGC_PDA_MCDDC_DP_REG, cpgcPDA.Data);

    if ((controlWordAddr >= 0x10) || (controlWordFunc != 0)) {
      tempData = ((controlWordAddr & 0xF0) << 4) | controlWordDataPtr[index];
    } else {
      tempData = (controlWordAddr << 4) | controlWordDataPtr[index];
    }
    //
    // Program CADB row 0, BCW change function to get pdatag working properly
    //
    ProgCADBRow (Host, socket, ch, Dimm, rank, BANK7, (BIT12 | (LRDIMM_BC7x << 4) | controlWordFunc), SIDE_A, ENTER_PDA, MRS_CMD, &patCADBProg0[0], &patCADBProg1[0]);
    //
    // Program CADB row 1 to write desired PBA command
    //
    ProgCADBRow (Host, socket, ch, Dimm, rank, BANK7, (BIT12 | tempData), SIDE_A, ENTER_PDA, MRS_CMD, &patCADBProg0[1], &patCADBProg1[1]);

    ProgramCADB (Host, socket, ch, NO_SUBCH, 0, 2, patCADBProg0, patCADBProg1); // for 2 rows

    //
    // Delay only if this is a CTE build, otherwise do nothing.
    //

    CteDelayQclk (10); // slight delay to allow everything to settle before starting.

    // Start test and clear errors
    seqCtl0.Bits.local_clear_errs  = 1;
    seqCtl0.Bits.local_start_test  = 1;
    MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, seqCtl0.Data);

    //
    // Poll test completion
    //
    CpgcPollGlobalTestDone (Host, socket, 1 << ch, NO_SUBCH);
#ifdef MRC_TRACE
    RcDebugPrintWithDevice (SDBG_TRACE, socket, ch, Dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                   "DDR4 PBA write: buffer %d, bank 0x%02X, data 0x%04X\n", index, BANK7, (BIT12 | tempData));
#endif //MRC_TRACE
  } //buffer index

  //
  // PBA exit
  //
  cpgcPDA.Bits.device_select = 0;
  //cpgcPDA.Bits.rank_select        = (*rankList)[rank].logicalRank;
  MemWritePciCfgEp (socket, ch, CPGC_PDA_MCDDC_DP_REG, cpgcPDA.Data);
  //
  // Program CADB row 0, BCW change function (func 0)
  //
  ProgCADBRow (Host, socket, ch, Dimm, rank, BANK7, (BIT12 | (LRDIMM_BC7x << 4)), SIDE_A, ENTER_PDA, MRS_CMD, &patCADBProg0[0], &patCADBProg1[0]);
  //
  // Program CADB row 1, BCW change function (func 0)
  //
  ProgCADBRow (Host, socket, ch, Dimm, rank, BANK7, (BIT12 | (LRDIMM_BC7x << 4)), SIDE_A, ENTER_PDA, MRS_CMD, &patCADBProg0[1], &patCADBProg1[1]);
  //
  // Program CADB row 2 to exit PBA mode
  //

  ProgCADBRow (Host, socket, ch, Dimm, rank, BANK7, (BIT12 | (LRDIMM_BC1x << 4) | ((*dimmNvList)[Dimm].lrBuf_BC1x & LRDIMM_ONE_RANK_TIMING_MODE)), SIDE_A, EXIT_PDA, MRS_CMD, &patCADBProg0[2], &patCADBProg1[2]);

  ProgramCADB (Host, socket, ch, NO_SUBCH, 0, 3, patCADBProg0, patCADBProg1);

  //
  // Execute MRS Mode
  //
  patCADBMrs.Data                     = 0;
  patCADBMrs.Bits.mrs_gap             = 5;
  patCADBMrs.Bits.mrs_start_pointer   = 0;
  patCADBMrs.Bits.mrs_end_pointer     = 2;
  MemWritePciCfgEp (socket, ch, CPGC_PATCADBMRS_MCDDC_CTL_REG, patCADBMrs.Data);

  //
  // Delay only if this is a CTE build, otherwise do nothing.
  //

  CteDelayQclk (10); // slight delay to allow everything to settle before starting.

  // Start test and clear errors
  seqCtl0.Bits.local_clear_errs  = 1;
  seqCtl0.Bits.local_start_test  = 1;
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, seqCtl0.Data);

  //
  // Poll test completion
  //
  CpgcPollGlobalTestDone (Host, socket, 1 << ch, NO_SUBCH);

  //
  // Restore after MR command
  //
  seqCtl0.Data = MemReadPciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4);
  seqCtl0.Bits.local_start_test = 0;
  seqCtl0.Bits.local_clear_errs = 1;
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, seqCtl0.Data);
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, CRValueSave);

  cpgcPatCadbCtlReg.Data = MemReadPciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG);
  cpgcPatCadbCtlReg.Bits.pda_length = 0;

  MemWritePciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG, cpgcPatCadbCtlReg.Data);

  SetCwlAdj (Host, socket, ch, SetType, CRAddDelayOrg);

  // HSD 5330727 - PBA not driving DQ low when scheduler is set to DDRT target
  // Restore original value
  SetCpgcTrainTarget (socket, ch, CpgcForDdrtOld);

  //Undo PRK override
  WriteLrbuf(Host, socket, ch, Dimm, 0, (*dimmNvList)[Dimm].lrBuf_BC02, LRDIMM_F0, LRDIMM_BC02);

  // Restore TxOn
  SetDataTxOnDdrio (Host, socket, ch, CurrentTxOn);
#endif // !DDR5_SUPPORT
} // WritePBA10nm
#endif //LRDIMM_SUPPORT

/**

  this function write the MR2 register for all the ranks

  @param Host            - sysHost data structure
  @param socket          - Processor socket
  @param ch              - Channel to issue the ZQ to
  @param zqType          - Type of ZQ Calibration: Long or Short

@retval UINT32 - SUCCESS if the reset is succeded

**/
UINT32
CpgcIssueZQ (
            PSYSHOST  Host,
            UINT8     socket,
            UINT8     ch,
            UINT8     zqType
            )
{
  UINT8               dimm;
  UINT8               rank;
  UINT8               delay = 1;
  UINT8               cmdTiming;
  UINT32              status = SUCCESS;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];

  RcDebugPrintWithDevice (SDBG_CPGC, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CpgcIssueZQ Starts\n");
  channelNvList = GetChannelNvList(Host, socket);

  if ((*channelNvList)[ch].enabled == 0) {
    return status;
  }

  cmdTiming = GetCurrentTimingMode (Host, socket, ch);

  if (zqType == ZQ_SHORT) {
    RcDebugPrintWithDevice (SDBG_NONE, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Issue ZQCS\n");
  } else {
    RcDebugPrintWithDevice (SDBG_NONE, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Issue ZQCL\n");
  }

  dimmNvList = GetDimmNvList(Host, socket, ch);

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    if ((*dimmNvList)[dimm].dimmPresent == 0) {
      continue;
    }
    if (CheckS3Jedec(Host, socket, ch, dimm) == 0) {
      continue;
    }

    for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
      if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
        continue;
      }

      if ((*dimmNvList)[dimm].DcpmmPresent) {
        //NVMDIMM_IF protocol does not support ZQCL or ZQCS in 3N mode
        //NVMCTLR will do internal calibration as part of power-up sequence
        if (cmdTiming != TIMING_3N) {
          status = WriteCADBCmd (Host, socket, ch, dimm, rank, ZQ_CMD, 0, (BIT7 | zqType), delay);
        } else {
          //Issue DB ZQCL by writing BC06 = 1
          WriteBC(Host, socket, ch, dimm, 0, 1, LRDIMM_F0, LRDIMM_BC06);
        }
      } else {
        status = WriteCADBCmd (Host, socket, ch, dimm, rank, ZQ_CMD, 0, (zqType << 10), delay);
      }
      //
      // Wait tZQoper, 512 DCLKs for tZQCL and 128 DCLKs for tZQCS
      // 1600: 512DCLKs -> 640ns (Lower frequency, worst case)
      // ...
      // 3200: 512DCLKs -> 320ns
      // Minimum possible delay is 1uS
      //
      FixedDelayMicroSecond (1);
    } // rank loop
  } // dimm loop

  return status;
} // CpgcIssueZQ10nm

/**

  DDR advanced training CPGC setup

  @param Host        - Pointer to sysHost
  @param socket      - Socket to train
  @param ddrChEnMap  - 4-bit bit mask of ddr channels to test - (1 = channel is selected to run tests)
  @param Dimm        - Target dimm
  @param rank        - logical rank number (0-7)
  @param group       - MRC_GT value to store
  @param pattern     - ddr pattern (0-3)
  @param dqLoopCount - dq sequence loop count - number of times dq sequence is executed (2^(dqLoopCount - 1))

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcAdvTrainingSetup (
                     PSYSHOST  Host,
                     UINT8     socket,
                     UINT32    ddrChEnMap,
                     UINT8     Dimm,
                     UINT8     rank,
                     UINT8     subRank,
                     UINT32    pattern,
                     UINT8     dqLoopCount
                     )
{

  UINT8               ch;
  UINT32              status;
  UINT8               MaxChDdr;

  struct channelNvram (*channelNvList)[MAX_CH];

  RcDebugPrintWithDevice (SDBG_CPGC, socket, NO_CH, NO_DIMM, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                 "CpgcAdvTrainingSetup: ChEnMap = 0x%x, pattern = %d\n", ddrChEnMap, pattern);

  status = SUCCESS;
  channelNvList = GetChannelNvList(Host, socket);
  MaxChDdr = GetMaxChDdr ();
  //
  // CPGC setup
  //

  for (ch = 0; ch < MaxChDdr; ch++) {

    // skip ddr channels not selected
    if (!(ddrChEnMap & (1 << ch))) {
      continue;
    }

    // skip disabled ddr channels
    if (((*channelNvList)[ch].enabled == 0)) {
      continue;
    }

    switch (pattern & DDR_CPGC_PATTERN_MASK) {
    case DDR_CPGC_DQ_PATTERN:
      CpgcAdvDqSetup (Host, socket, ch, rank, subRank, dqLoopCount);
      break;

    case DDR_CPGC_ADV_CMD_PATTERN:
      CpgcAdvCmdPatternSetup (Host, socket, ch, Dimm, rank, dqLoopCount);
      break;

      /*
      case DDR_CPGC_CMD_G2_PATTERN: // cmd group 2
        CpgcAdvCmdSetup2 (Host, socket, ch, rank);
        break;

      case DDR_CPGC_CMD_G3MPR_PATTERN: // cmd group 3
        CpgcAdvCmdSetup3 (Host, socket, ch, rank);
        break;

      case DDR_CPGC_MEM_TEST_PATTERN: // mem test pattern
        CpgcMemTestGlobalSetup (Host, socket, ch, rank, dqLoopCount, pattern);
        break;
      */
    default:
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_47);
      status = FAILURE;
    }
  }

  return status;

} // CpgcAdvTrainingSetup

UINT32
CpgcAdvTrainingModifyRank (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    ddrChEnMap,
  UINT8     Dimm,
  UINT8     rank,
  UINT8     subRank,
  UINT32    pattern
  )
{

  UINT8               ch;
  UINT32              status;
  struct channelNvram (*channelNvList) [MAX_CH];
  UINT8               MaxChDdr;

  RcDebugPrintWithDevice (SDBG_CPGC, socket, NO_CH, NO_DIMM, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                  "CpgcAdvTrainingSetup: ChEnMap = 0x%x, pattern = %d\n", ddrChEnMap, pattern);

  status = SUCCESS;
  channelNvList = GetChannelNvList (Host, socket);
  MaxChDdr = GetMaxChDdr ();
  //
  // CPGC setup
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    // skip ddr channels not selected
    if (!(ddrChEnMap & (1 << ch))) {
      continue;
    }
    // skip disabled ddr channels
    if (((*channelNvList) [ch].enabled == 0)) {
      continue;
    }

    switch (pattern & DDR_CPGC_PATTERN_MASK) {
      case DDR_CPGC_DQ_PATTERN:
        //CpgcAdvDqSetup(Host, socket, ch, rank, subRank, dqLoopCount);
        SelectCPGCRanks (Host, socket, ch, 1 << rank, 0, subRank);
        break;

      case DDR_CPGC_ADV_CMD_PATTERN:
        //CpgcAdvCmdPatternSetup(Host, socket, ch, Dimm, rank, dqLoopCount);
        SelectCPGCRanks (Host, socket, ch, 1 << rank, 0, 0);
        break;

      default:
        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_47);
    }
  }
  return status;
} // CpgcAdvTrainingModifyRank10nm

/**

  DDR DQ victim aggressor CPGC setup.

  @param Host        - Pointer to sysHost
  @param socket      - Socket to train
  @param ch          - ddr channel to setup
  @param logRank     - logical rank number
  @param dqLoopCount - dq sequence loop count - number of times dq sequence is executed (2^(dqLoopCount - 1))
  @param victimBit   - dq victimBit

  @retval SUCCESS / FAILURE

**/
STATIC UINT32
CpgcAdvDqSetup (
               PSYSHOST  Host,
               UINT8     socket,
               UINT8     ch,
               UINT8     logRank,
               UINT8     subRank,
               UINT8     dqLoopCount
               )
{
  UINT32                                        status;
  UINT8                                         wdbIncrRate;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT              cpgcPatCadbCtlReg;
  CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_STRUCT     seqBaseAddrStartLo0;
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_STRUCT     seqBaseAddrStartHi0;
  CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_STRUCT      seqBaseAddrWrapLo0;
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_STRUCT      seqBaseAddrWrapHi0;
  CPGC_GLOBAL_TRAINING_SETUP                    cpgcGblTrainingSetup;
  CPGC_SEQ_BAINCCTL00_MC_MAIN_STRUCT            seqBaincCtl00;
  CPGC_SEQ_BAINCCTL10_MC_MAIN_STRUCT            seqBaincCtl10;
  CPGC_SEQ_BAOCIC0_MC_MAIN_STRUCT               cpgcSeqBaoCic0;

  RcDebugPrintWithDevice (SDBG_CPGC, socket, ch, NO_DIMM, NO_SUBCH, logRank, NO_STROBE, NO_BIT,
                 "CpgcAdvDqSetup\n");

  status = SUCCESS;

  seqBaseAddrWrapLo0.Data                 = 0;
  seqBaseAddrWrapHi0.Data                 = 0;
  seqBaincCtl00.Data                      = 0;
  seqBaincCtl10.Data                      = 0;


  cpgcGblTrainingSetup.seqLoopCnt         = dqLoopCount;                      // test will end after (2^(n-1)) loops through the sub-sequence
  if (IsAepRank(Host, socket, ch, logRank)) {
    cpgcGblTrainingSetup.burstSeparation  = CPGC_SUBSEQ_WAIT_DDRT;            // (sub_seqwait) - need to add delay to drain credits if doing write / read subseq
  } else {
    cpgcGblTrainingSetup.burstSeparation  = CPGC_SUBSEQ_WAIT_DEFAULT;         // (sub_seqwait) - need to add delay to drain credits if doing write / read subseq
  }
  seqBaseAddrWrapLo0.Bits.column_addr     = 0x1FF;
  seqBaseAddrWrapLo0.Bits.bank_addr       = 0x4;
  seqBaseAddrWrapHi0.Bits.row_addr        = 0xFFFF;
  seqBaseAddrWrapHi0.Bits.rank_addr       = 0;    // Use logical to physical rank mapping
  seqBaincCtl00.Bits.row_ba_updrate       = 0;
  seqBaincCtl10.Bits.bank_ba_updscale     = 0;
  seqBaincCtl10.Bits.rank_ba_updrate      = 0;


  cpgcGblTrainingSetup.startDelay        = 0;                                // Delay start
  cpgcGblTrainingSetup.addrUpdRateMode   = CPGC_UPDATE_BY_CL;                // Update based on the # of read and write cacheline
  cpgcGblTrainingSetup.rwMode            = CPGC_BASE_WRITE_SUBSEQ;           // Base Read or write - subseq0
  cpgcGblTrainingSetup.numCacheLines     = 5;                                // Number of Cachelines (2^N)
  cpgcGblTrainingSetup.errEnChunk        = CPGC_ERR_MONITOR_ALL_CHUNK;       // monitor errors on any chunk
  cpgcGblTrainingSetup.errEnCl           = CPGC_ERR_MONITOR_ALL_CL;          // monitor errors on any cacheline
  cpgcGblTrainingSetup.stopOnErrCtl      = CPGC_ERR_STOP_ON_ALL_ERR;         // Stop on all lane error
  cpgcGblTrainingSetup.subseqEndPtr      = 1;                                // subseq end pointer
  cpgcGblTrainingSetup.useSubSeq1        = CPGC_BASE_READ_SUBSEQ;            // use sub seq1 (base read)
  cpgcGblTrainingSetup.cpgcLinkTrain     = 0;                                // do not use cpgc link train
  cpgcGblTrainingSetup.back2backWrEn     = 1;                                // enable b2b writes
  cpgcGblTrainingSetup.mprControl        = 0;                                // MPR disable
  cpgcGblTrainingSetup.refMask           = 0xFF;                             // disable cpgc self refresh
  cpgcGblTrainingSetup.zqMask            = 0xFF;                             // disable cpgc zq
  //if(x8) {
  cpgcGblTrainingSetup.x8Mode           = 0;
  //} else {
  // cpgcGblTrainingSetup.x8Mode           = 0;
  //}

  CpgcGlobalTrainingSetup (Host, socket, ch, &cpgcGblTrainingSetup);

  //
  // WDB programming
  //
  seqBaseAddrStartLo0.Data            = 0;
  seqBaseAddrStartHi0.Data            = 0;
  seqBaseAddrStartHi0.Bits.rank_addr  = 0;  // Use logical to physical rank mapping

  // Program logical to physical rank mapping
  SelectCPGCRanks (Host, socket, ch, 1 << logRank, 0, subRank);

  //
  // Sequence BAIN control
  //
  if (IsBrsPresent(Host, socket)) {
    seqBaincCtl00.Bits.column_ba_inc = 4;
  } else {
    seqBaincCtl00.Bits.column_ba_inc = 1;
  }
  seqBaincCtl00.Bits.column_ba_updrate  = 0;
  seqBaincCtl00.Bits.column_ba_updscale = 1;
  seqBaincCtl00.Bits.row_ba_inc         = 0;
  seqBaincCtl00.Bits.row_ba_updscale    = 0;

  if (IsBrsPresent(Host, socket)) {
    seqBaseAddrWrapLo0.Bits.bank_addr   = 8;
  }
  seqBaincCtl10.Bits.bank_ba_inc        = 4;

  seqBaincCtl10.Bits.bank_ba_updrate    = 0;
  seqBaincCtl10.Bits.rank_ba_inc        = 0;
  seqBaincCtl10.Bits.rank_ba_updscale   = 0;
  seqBaincCtl10.Bits.column_ba_inc_msb  = 0;

  //
  // Set wrap order
  //
  cpgcSeqBaoCic0.Data = 0;

  if (IsBrsPresent (Host, socket)) {
    wdbIncrRate = 0;
  } else {
    wdbIncrRate = 5;
  }

  CpgcSetupWdb (Host, socket, ch,
                wdbIncrRate,                 // wdbIncrRate
                0,                           // wdbIncrScale
                0,                           // wdbStartPntr (victim bit 0)
                7,                           // wdbEndPntr (victim bit 0)
                1,                           // setupPbwr
                CPGC_WDB_MUX_CTL_LFSR24,     // mux control
                seqBaincCtl00,
                seqBaincCtl10,
                seqBaseAddrStartLo0,
                seqBaseAddrStartHi0,
                seqBaseAddrWrapLo0,
                seqBaseAddrWrapHi0,
                cpgcSeqBaoCic0
               );
  CpgcFillWdbDqVa (Host, socket, ch, 0);

  //
  // Disable CADB
  //
  cpgcPatCadbCtlReg.Data = 0;
  MemWritePciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG, cpgcPatCadbCtlReg.Data);

  return status;

} // CpgcAdvDqSetup

/**

  DDR Advanced CMD Pattern CPGC setup.

  @param Host        - Pointer to sysHost
  @param socket      - Socket to train
  @param ch          - ddr channel to setup
  @param Dimm        - Target dimm
  @param rank        - logical rank number

  @retval SUCCESS / FAILURE

**/
STATIC UINT32
CpgcAdvCmdPatternSetup (
                       PSYSHOST  Host,
                       UINT8     socket,
                       UINT8     ch,
                       UINT8     Dimm,
                       UINT8     rank,
                       UINT8     dqLoopCount
                       )
{
  UINT32                                        status;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT              cpgcPatCadbCtlReg;
  CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_STRUCT     seqBaseAddrStartLo0;
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_STRUCT     seqBaseAddrStartHi0;
  CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_STRUCT      seqBaseAddrWrapLo0;
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_STRUCT      seqBaseAddrWrapHi0;
  CPGC_GLOBAL_TRAINING_SETUP                    cpgcGblTrainingSetup;
  CPGC_SEQ_BAINCCTL00_MC_MAIN_STRUCT            seqBaincCtl00;
  CPGC_SEQ_BAINCCTL10_MC_MAIN_STRUCT            seqBaincCtl10;
  CPGC_SEQ_BAOCIC0_MC_MAIN_STRUCT               cpgcSeqBaoCic0;
  struct dimmNvram                              (*dimmNvList)[MAX_DIMM];
  UINT32                                        CadbMuxPattern[CADB_MUX_MAX];
  UINT8                                         CadbMuxCtl[CADB_MUX_MAX];
  UINT32                                        AddressMask;

  CalculateAddressMask (Host, socket, ch, Dimm, &AddressMask);

  RcDebugPrintWithDevice (SDBG_CPGC, socket, ch, NO_DIMM, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                 "CpgcAdvCmdPatternSetup\n");
  //
  // Make sure pages are closed
  //
  FixedDelayMicroSecond (50);

  status = SUCCESS;

  cpgcGblTrainingSetup.seqLoopCnt        = dqLoopCount;                                // test will end after (2^(n-1)) loops through the sub-sequence
  cpgcGblTrainingSetup.startDelay        = 0;                                // Delay start
  cpgcGblTrainingSetup.addrUpdRateMode   = CPGC_UPDATE_BY_CL;                // Update based on the # of read and write cacheline
  cpgcGblTrainingSetup.rwMode            = CPGC_BASE_WRITE_SUBSEQ;           // Base Read or write - subseq0
  cpgcGblTrainingSetup.numCacheLines     = 5;                                // Number of Cachelines (2^N)

  cpgcGblTrainingSetup.burstSeparation   = CPGC_SUBSEQ_WAIT_DDRT;            // (sub_seqwait) - need to add delay to drain credits if doing write / read subseq
  cpgcGblTrainingSetup.errEnChunk        = CPGC_ERR_MONITOR_ALL_CHUNK;       // monitor errors on any chunk
  cpgcGblTrainingSetup.errEnCl           = CPGC_ERR_MONITOR_ALL_CL;          // monitor errors on any cacheline
  cpgcGblTrainingSetup.stopOnErrCtl      = CPGC_ERR_STOP_ON_ALL_ERR;         // Stop on all lane error
  cpgcGblTrainingSetup.subseqEndPtr      = 1;                                // subseq end pointer
  cpgcGblTrainingSetup.useSubSeq1        = CPGC_BASE_READ_SUBSEQ;            // use sub seq1 (base read)
  cpgcGblTrainingSetup.cpgcLinkTrain     = 0;                                // do not use cpgc link train
  cpgcGblTrainingSetup.back2backWrEn     = 0;                                // enable b2b writes
  cpgcGblTrainingSetup.mprControl        = 0;                                // MPR disable
  cpgcGblTrainingSetup.refMask           = 0xFF;                             // disable cpgc self refresh
  cpgcGblTrainingSetup.zqMask            = 0xFF;                             // disable cpgc zq
  cpgcGblTrainingSetup.x8Mode           = 0;

  CpgcGlobalTrainingSetup (Host, socket, ch, &cpgcGblTrainingSetup);

  // Program logical to physical rank mapping
  SelectCPGCRanks (Host, socket, ch, 1 << rank, 0, 0);
  //
  // WDB programming
  //
  seqBaseAddrStartLo0.Data            = 0;
  seqBaseAddrStartHi0.Data            = 0;
  seqBaseAddrStartHi0.Bits.rank_addr  = 0;

  seqBaseAddrWrapLo0.Data             = 0;
  seqBaseAddrWrapLo0.Bits.column_addr = 0x1F;
  if (IsThereAnyDimmX16InChannel (socket, ch)) {
    seqBaseAddrWrapLo0.Bits.bank_addr = 0x7;
  } else {
    seqBaseAddrWrapLo0.Bits.bank_addr = 0xF;
  }

  seqBaseAddrWrapHi0.Data = 0;


  if (IsThereAnyDimmX16InChannel (socket, ch)) {
    seqBaseAddrWrapHi0.Bits.row_addr = AddressMask & 0xFFFF;
  } else {
    seqBaseAddrWrapHi0.Bits.row_addr = AddressMask;
  }


  seqBaseAddrWrapHi0.Bits.rank_addr = 0;

  //
  // Sequence Base Address Increment control
  //
  seqBaincCtl00.Data = 0;
  seqBaincCtl00.Bits.column_ba_inc      = 43;
  seqBaincCtl00.Bits.column_ba_updrate  = 0;
  seqBaincCtl00.Bits.column_ba_updscale = 1;
  if (IsAepRank(Host, socket, ch, rank)) {
    seqBaincCtl00.Bits.row_ba_inc       = 0;
  } else {
    seqBaincCtl00.Bits.row_ba_inc       = 1553;
  }
  seqBaincCtl00.Bits.row_ba_updrate     = 0;
  seqBaincCtl00.Bits.row_ba_updscale    = 1;

  seqBaincCtl10.Data = 0;
  if (IsThereAnyDimmX16InChannel (socket, ch)) {
    seqBaincCtl10.Bits.bank_ba_inc      = 1;
  } else {
    seqBaincCtl10.Bits.bank_ba_inc      = 3;
  }
  seqBaincCtl10.Bits.bank_ba_updrate    = 0;
  seqBaincCtl10.Bits.bank_ba_updscale   = 1;

  seqBaincCtl10.Bits.rank_ba_inc        = 0;
  seqBaincCtl10.Bits.rank_ba_updrate    = 0;

  seqBaincCtl10.Bits.rank_ba_updscale   = 0;
  seqBaincCtl10.Bits.column_ba_inc_msb  = 0;

  //
  // Set wrap order
  //
  cpgcSeqBaoCic0.Data = 0;

  CpgcSetupWdb (Host, socket, ch,
                5,                           // wdbIncrRate
                0,                           // wdbIncrScale - Exponential
                0,                           // wdbStartPntr (victim bit 0)
                7,                           // wdbEndPntr (victim bit 0)
                1,                           // setupPbwr
                CPGC_WDB_MUX_CTL_LFSR24_PATBUF,     // mux control
                seqBaincCtl00,
                seqBaincCtl10,
                seqBaseAddrStartLo0,
                seqBaseAddrStartHi0,
                seqBaseAddrWrapLo0,
                seqBaseAddrWrapHi0,
                cpgcSeqBaoCic0
               );

  //
  // Program WDB patterns (dq/va pattern)
  //
  CpgcFillWdbDqVa (Host, socket, ch, 1);


  {
    //
    // Program WDB patterns
    //
    ProgramCADB (Host, socket, ch, NO_SUBCH, 0, CADB_LINES, AdvCmdPattern0, AdvCmdPattern1);
  }
  //
  // Enable CADB on deselect
  //
  cpgcPatCadbCtlReg.Data = MemReadPciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG);
  //
  // DDRT does not support deselect mode
  //
  dimmNvList = &Host->nvram.mem.socket[socket].channelList[ch].dimmList;
  if ((*dimmNvList)[Dimm].DcpmmPresent) {
    cpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
  } else {
    cpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 1;
  }
  cpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 0;
  cpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 0;
  if ((Host->nvram.mem.dimmTypePresent == UDIMM) || (Host->nvram.mem.dimmTypePresent == SODIMM)) {
    cpgcPatCadbCtlReg.Bits.enable_cadb_parityntiming = 1;
  } else {
    cpgcPatCadbCtlReg.Bits.enable_cadb_parityntiming = 0;
  }
  MemWritePciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG, cpgcPatCadbCtlReg.Data);

  //
  // Program CADB pattern buffer
  //
  CadbMuxPattern[0] = CADB_MUX_PATTERN0;
  CadbMuxPattern[1] = CADB_MUX_PATTERN1;
  CadbMuxPattern[2] = CADB_MUX_PATTERN2;
  CadbMuxPattern[3] = CADB_MUX_PATTERN3;

  //
  // Enable LFSR
  //
  CadbMuxCtl[0] = CADB_MUX_CTRL_LFSR16;
  CadbMuxCtl[1] = CADB_MUX_CTRL_LFSR16;
  CadbMuxCtl[2] = CADB_MUX_CTRL_LFSR16;
  CadbMuxCtl[3] = CADB_MUX_CTRL_LFSR16;

  ProgramCadbMux (socket, ch, NO_SUBCH, CadbMuxPattern, CadbMuxCtl);

  //
  // Enable Pmon to measure MC_PMON_SUBEVENT_DDRT_RPQ_OCC_NOGNT event
  //
  SetupPmon(Host, socket, ch, 0xE0, 1);
  return status;

} // CpgcAdvCmdPatternSetup

/**

  Enable deselect patterns

  @param Host        - Pointer to sysHost
  @param socket      - Socket to train
  @param ddrChEnMap  - DDR channels to setup
  @param Dimm        - Dimm to train
  @param mode        - MODE_DESELECT
                     - MODE_VA_DESELECT

  @retval SUCCESS / FAILURE

**/
VOID
EnableCADBDeselect (
                   PSYSHOST  Host,
                   UINT8     socket,
                   UINT32    ddrChEnMap,
                   UINT8     Dimm,
                   UINT16    mode
                   )
{
  UINT8                                   ch;
  struct channelNvram                     (*channelNvList)[MAX_CH];
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT        cpgcPatCadbCtlReg;
  struct dimmNvram                        (*DimmNvList)[MAX_DIMM];
  UINT32                                  CadbMuxPattern[CADB_MUX_MAX];
  UINT8                                   CadbMuxCtl[CADB_MUX_MAX];
  UINT8                                   MaxChDdr;

  RcDebugPrintWithDevice (SDBG_CPGC, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "EnableCADBDeselect Starts\n");
  channelNvList = GetChannelNvList(Host, socket);
  MaxChDdr = GetMaxChDdr ();

  for (ch = 0; ch < MaxChDdr; ch++) {

    // skip ddr channels not selected
    if (!(ddrChEnMap & (1 << ch))) {
      continue;
    }


    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }     // skip disabled ddr channels

    //
    // Program WDB patterns
    //

    if (mode & MODE_DESELECT) {
      ProgramCADB (Host, socket, ch, NO_SUBCH, 0, CADB_LINES, AdvCmdPattern0, AdvCmdPattern1);
    } else {
      ProgramCADB (Host, socket, ch, NO_SUBCH, 0, CADB_LINES, AggAdvCmdPattern0, AggAdvCmdPattern1);
    }
    //
    // Enable CADB on deselect
    //
    cpgcPatCadbCtlReg.Data = MemReadPciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG);
    //
    // DDRT exception, do not use deselect on DDRT
    //
    DimmNvList = &Host->nvram.mem.socket[socket].channelList[ch].dimmList;
    if ((*DimmNvList)[Dimm].DcpmmPresent) {
      cpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
    } else {
      cpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 1;
    }
    cpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 0;
    cpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 0;
    if ((Host->nvram.mem.dimmTypePresent == UDIMM) || (Host->nvram.mem.dimmTypePresent == SODIMM)) {
      cpgcPatCadbCtlReg.Bits.enable_cadb_parityntiming = 1;
    } else {
      cpgcPatCadbCtlReg.Bits.enable_cadb_parityntiming = 0;
    }
    MemWritePciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG, cpgcPatCadbCtlReg.Data);

    //
    // Program CADB pattern buffer
    //
    CadbMuxPattern[0] = CADB_MUX_PATTERN0;
    CadbMuxPattern[1] = CADB_MUX_PATTERN1;
    CadbMuxPattern[2] = CADB_MUX_PATTERN2;
    CadbMuxPattern[3] = CADB_MUX_PATTERN3;

    CadbMuxCtl[0] = CADB_MUX_CTRL_LFSR16;
    CadbMuxCtl[1] = CADB_MUX_CTRL_LFSR16;
    CadbMuxCtl[2] = CADB_MUX_CTRL_LFSR16;
    CadbMuxCtl[3] = CADB_MUX_CTRL_LFSR16;

    if ((*channelNvList) [ch].enabled == 0) {
      continue;
    }

    ProgramCadbMux (socket, ch, NO_SUBCH, CadbMuxPattern, CadbMuxCtl);
  } // ch loop
} // EnableCADBDeselect

/**

  DDR CPGC fill wdb for dq/va pattern

  @param Host    - Pointer to sysHost
  @param socket  - Socket to train
  @param ch      - ddr channel to setup

  @retval SUCCESS / FAILURE

**/
STATIC UINT32
CpgcFillWdbDqVa (
                PSYSHOST  Host,
                UINT8     socket,
                UINT8     ch,
                UINT8     mode
                )
{

  UINT8     i;
  UINT8     wdbIndex;
  UINT8     numLines;
  UINT32    status;
  TWdbLine  WdbLines[MRC_WDB_LINES];

  RcDebugPrintWithDevice (SDBG_CPGC, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "CpgcFillWdbDqVa\n");

  status = SUCCESS;
  numLines = 8;
  for (wdbIndex = 0; wdbIndex < numLines; wdbIndex++) {
    for (i = 0; i < MRC_WDB_LINE_SIZE; i++) {
      if ((mode == 1) && (Host->nvram.mem.dramType == SPD_TYPE_DDR4)) {
        WdbLines[wdbIndex].WdbLine[i] = CpgcWdbDqVaPat2[wdbIndex][i / 8];
      } else {
        WdbLines[wdbIndex].WdbLine[i] = CpgcWdbDqVaPat[wdbIndex][i / 8];
      }
    } // i loop
  } // wdbIndex loop

  WDBFill (Host, socket, ch, WdbLines, numLines, 0);

  return status;

} // CpgcFillWdbDqVa


/**

  CPGC setup per rank for Advanced Memory test

  @param[in] Host              - Pointer to sysHost
  @param[in] socket            - Memory controller to train
  @param[in] ddrChEnMap        - ddr channels to setup
  @param[in] logRank           - logical rank to setup
  @param[in] logSubRank        - logical subrank number to test
  @param[in] colAddressBits    - number of DRAM column address bits to test
  @param[in] rowAddressBits    - number of DRAM row address bits to test
  @param[in] bankAddressBits   - number of DRAM bank address bits to test
  @param[in] mode              - Type of sequence MT_CPGC_WRITE, MT_CPGC_READ, or MT_CPGC_READ_WRITE
  @param[in] AddressMode       - FAST_Y for column first, FAST_X for row first
  @param[in] direction         - Sequential address direction MT_ADDR_DIR_UP, MT_ADDR_DIR_DN
  @param[in] bank              - Base bank address number to test
  @param[in] baseBits          - Number of bank address bits to skip on increment (for BG interleave)
  @param[in] rowAddr[MAX_CH]   - Base row address to start testing
  @param[in] rowSize[MAX_CH]   - Number of rows to test
  @param[in] deviceMask[MAX_CH][3] - Bitmask of DQ lanes to not test
  @param[in] LineMask[MAX_CH]      - Each bit corresponds to a cache line; 1 means test, 0 do not test
  @param[in] StopOnErr[MAX_CH]     - 1 = stop on 1st error; 0 = stop on all DQ lanes error
  @param[in] seqDataInv[MT_MAX_SUBSEQ] - Enables pattern inversion per subsequence

  @retval None

**/
VOID
CpgcMemTestRankSetupMATSRowRange (
                     PSYSHOST  Host,
                     UINT8     socket,
                     UINT32    ddrChEnMap,
                     UINT8     logRank,
                     UINT8     logSubRank,
                     UINT8     colAddressBits[MAX_CH],
                     UINT8     rowAddressBits[MAX_CH],
                     UINT8     bankAddressBits[MAX_CH],
                     UINT8     mode,
                     UINT8     AddressMode,
                     UINT8     direction,
                     UINT8     bank,
                     UINT8     baseBankBits,
                     UINT32    rowAddr[MAX_CH],
                     UINT32    rowSize[MAX_CH],
                     UINT32    deviceMask[MAX_CH][3],
                     UINT8     LineMask[MAX_CH],
                     UINT8     StopOnErr[MAX_CH],
                     UINT8     seqDataInv[MT_MAX_SUBSEQ]
                     )
{
  UINT8                                       i;
  UINT8                                       sizeFound;
  UINT8                                       ch;
  UINT8                                       cpgcRowAddressBits;
  UINT8                                       cpgcColAddressBits;
  UINT8                                       cpgcBankAddressBits;
  UINT8                                       LoopCount = 0;
  UINT8                                       mcId;
  INT32                                       matsBurstBits = 0;
  UINT32                                      tempAddr;
  UINT8                                       ColIncSize;
  struct channelNvram                         (*channelNvList)[MAX_CH];
  CPGC_SUBSEQ0_CTL0_MC_MAIN_STRUCT            cpgcSubseq0Ctl0Reg;
  CPGC_SUBSEQ1_CTL0_MC_MAIN_STRUCT            cpgcSubseq1Ctl0Reg;
  CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_STRUCT   seqBaseAddrStartLo0;
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_STRUCT   seqBaseAddrStartHi0;
  CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_STRUCT    seqBaseAddrWrapLo0;
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_STRUCT    seqBaseAddrWrapHi0;
  CPGC_SEQ_BAINCCTL00_MC_MAIN_STRUCT          seqBaincCtl00;
  CPGC_SEQ_BAINCCTL10_MC_MAIN_STRUCT          seqBaincCtl10;
  CPGC_SEQ_BAOCIC0_MC_MAIN_STRUCT             cpgcSeqBaoCic0;
  ALIAS_CPGC_GLOBALCTRL_MC_MAIN_STRUCT        cpgcGlobalCtrlReg;
  CPGC_SEQCTL0_MC_MAIN_STRUCT                 cpgcSeqCtl0Reg;
  CPGC_ERR_CTL_MCDDC_CTL_STRUCT               errCtl;
  UINT8                                       MaxChDdr;
  MaxChDdr      = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);

  RcDebugPrintWithDevice (SDBG_CPGC, socket, NO_CH, NO_DIMM, NO_SUBCH, logRank, NO_STROBE, NO_BIT,
                 "CpgcMemTestRankSetupMATSRowRange: ddrChEnMap = 0x%x\n", ddrChEnMap);

  //
  // CPGC setup
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    mcId = GetMCID(Host, socket, ch);
    //
    // skip ddr channels not selected or disabled ddr channels
    // Must disable global start so start test does not activate cpgc on disabled channel for this rank
    //
    if (!(ddrChEnMap & (1 << ch)) ||
        (rowSize[ch] == 0) ||
        (Host->nvram.mem.socket[socket].channelList[ch].enabled == 0) ||
        (((*channelNvList)[ch].encodedCSMode == 0) && (logSubRank != 0))) {
        CpgcDisableGlobalStart (Host, socket, ch);
        continue;
    }

    if (direction == MT_ADDR_DIR_UP) {
      tempAddr = rowAddr[ch];
    } else {
      tempAddr = rowAddr[ch] + rowSize[ch] - 1;
    }
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, logRank, NO_STROBE, NO_BIT,
                   "colBits = %d, rowBits = %d, bankBits = %d, bank = %2d, row = 0x%08x, size = 0x%08x, dir = %d, devMask = 0x%02x%08x%08x, lineMask = 0x%02x\n",
                   colAddressBits[ch], rowAddressBits[ch], bankAddressBits[ch], bank, tempAddr, rowSize[ch], direction,
                   deviceMask[ch][2], deviceMask[ch][1], deviceMask[ch][0], LineMask[ch]);

    //
    // Adjust number of cachelines for size of rank (use 2^n) scale
    //

    //Assert that rowSize[ch] is 2^n
    sizeFound = 0;
    cpgcRowAddressBits = 0;
    for (i = 0; i < 32; i++) {
      if (rowSize[ch] & (1 << i)) {
        sizeFound++;
        cpgcRowAddressBits = i;
      }
    }
    RC_FATAL_ERROR (sizeFound < 2, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_153);

    //
    // discover burst length setting
    //
    cpgcGlobalCtrlReg.Data = MemReadPciCfgMC (socket, mcId, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG);

    if (IsDdr5Present (Host, socket)) {
      //
      // DDR5: Assume Burst Length 16 with use_bl4_col_addr = 1
      // Column address  4  3  2  1  0
      // CPGC address    2  1  0
      // BL16 low addres 0
      //
      ColIncSize = 4;
      if (cpgcGlobalCtrlReg.Bits.use_bl4_col_addr == 1) {
        cpgcColAddressBits = colAddressBits[ch] - 4;
      } else {
        RC_FATAL_ERROR (sizeFound < 2, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_153);
      }
    } else {
      //
      // DDR4: if Burst Length 4, subtract 2 column bits, else subtract 3
      //
      ColIncSize = 1;
      if (cpgcGlobalCtrlReg.Bits.use_bl4_col_addr == 1) {
        cpgcColAddressBits = colAddressBits[ch] - 2;
      } else {
        cpgcColAddressBits = colAddressBits[ch] - 3;
      }
    }

    cpgcBankAddressBits = bankAddressBits[ch];


    //
    // AddressMode: X, Y, Z = Column, Row, Bank
    //
    switch (AddressMode) {
    case FAST_Y:
      matsBurstBits = cpgcColAddressBits + (cpgcBankAddressBits - baseBankBits);
      LoopCount = cpgcRowAddressBits;
      break;
    case FAST_X:
      matsBurstBits = cpgcRowAddressBits + (cpgcBankAddressBits - baseBankBits);
      LoopCount = cpgcColAddressBits;
      break;
    default:
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_170);
      break;
    }

    //
    // Program mask registers
    // 0 = no mask
    // 0xF = mask error checking on the DQ nibble
    //
    MemWritePciCfgEp (socket, ch, CPGC_ERR_DATA0_MCDDC_CTL_REG, deviceMask[ch][0]);
#ifdef DDR5_SUPPORT
    if (IsBrsPresent(Host, socket)) {
      // Account for CPGC mapping on BRS
      MemWritePciCfgEp (socket, ch, CPGC_ERR_DATA1_MCDDC_CTL_REG, 0xFFFFFFFF);
    } else
#endif
    {
      MemWritePciCfgEp (socket, ch, CPGC_ERR_DATA1_MCDDC_CTL_REG, deviceMask[ch][1]);
    }
    //
    // Do not check ECC bits if ECC is disabled
    //
    if (Host->nvram.mem.eccEn) {
      MemWritePciCfgEp (socket, ch, CPGC_ERR_ECC_MCDDC_CTL_REG, deviceMask[ch][2]);
    } else {
      MemWritePciCfgEp (socket, ch, CPGC_ERR_ECC_MCDDC_CTL_REG, 0xFF);
    }

    //
    // Set cache line mask and stop on error control
    //
    errCtl.Data = MemReadPciCfgEp (socket, ch, CPGC_ERR_CTL_MCDDC_CTL_REG);
    errCtl.Bits.selective_err_enb_cl = LineMask[ch];
    if (StopOnErr[ch]) {
      errCtl.Bits.stop_on_errcontrol = 1;                                // Stop on first error
    } else {
      errCtl.Bits.stop_on_errcontrol = CPGC_ERR_STOP_ON_ALL_ERR;         // Stop on all lane error
    }
    MemWritePciCfgEp (socket, ch, CPGC_ERR_CTL_MCDDC_CTL_REG, errCtl.Data);

    cpgcSeqCtl0Reg.Data = MemReadPciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4);
    cpgcSeqCtl0Reg.Bits.loopcount = LoopCount + 1;
    cpgcSeqCtl0Reg.Bits.address_update_rate_mode  = 0;
    if (mode == MT_CPGC_READ_WRITE) {
      cpgcSeqCtl0Reg.Bits.subseq_end_pointer = 1;
    } else {
      cpgcSeqCtl0Reg.Bits.subseq_end_pointer = 0;
    }
    MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, cpgcSeqCtl0Reg.Data);

    //Subsequence 0
    cpgcSubseq0Ctl0Reg.Data = MemReadPciCfgMC_Ch (socket, ch, CPGC_SUBSEQ0_CTL0_MC_MAIN_REG, 0);
    cpgcSubseq0Ctl0Reg.Bits.num_cachelines = matsBurstBits;

    //
    // Hook for error injection
    //
    if (GetInjectErrorMATS (Host, ch)) {
      cpgcSubseq0Ctl0Reg.Bits.num_cachelines = 0;
    }

    cpgcSubseq0Ctl0Reg.Bits.rstcurr_ba_start = 0;
    cpgcSubseq0Ctl0Reg.Bits.savecurr_ba_start = 1;
    cpgcSubseq0Ctl0Reg.Bits.num_cachelines_scale = 0;
    if ((mode == MT_CPGC_READ_WRITE) && StopOnErr[ch]) {
      //
      // Workaround for CPGC Stop on error = 1 not stopping during read subsequence and allowing write subsequence to execute with inverted data.
      //
      cpgcSubseq0Ctl0Reg.Bits.subseq_wait = (*channelNvList)[ch].rankPerCh[logRank].Roundtrip / 2;
    } else {
      cpgcSubseq0Ctl0Reg.Bits.subseq_wait = 0;
    }
    if (seqDataInv[0]) {
      cpgcSubseq0Ctl0Reg.Bits.inv_dataecc = 1;
    } else {
      cpgcSubseq0Ctl0Reg.Bits.inv_dataecc = 0;
    }
    MemWritePciCfgMC_Ch (socket, ch, CPGC_SUBSEQ0_CTL0_MC_MAIN_REG, 0, cpgcSubseq0Ctl0Reg.Data);

    //Subsequence 1
    cpgcSubseq1Ctl0Reg.Data = MemReadPciCfgMC_Ch (socket, ch, CPGC_SUBSEQ1_CTL0_MC_MAIN_REG, 0);
    cpgcSubseq1Ctl0Reg.Bits.num_cachelines = matsBurstBits;
    cpgcSubseq1Ctl0Reg.Bits.rstcurr_ba_start = 1;
    cpgcSubseq1Ctl0Reg.Bits.savecurr_ba_start = 0;
    cpgcSubseq1Ctl0Reg.Bits.num_cachelines_scale = 0;
    cpgcSubseq1Ctl0Reg.Bits.subseq_wait = 0;
    if (seqDataInv[1]) {
      cpgcSubseq1Ctl0Reg.Bits.inv_dataecc = 1;
    } else {
      cpgcSubseq1Ctl0Reg.Bits.inv_dataecc = 0;
    }
    MemWritePciCfgMC_Ch (socket, ch, CPGC_SUBSEQ1_CTL0_MC_MAIN_REG, 0, cpgcSubseq1Ctl0Reg.Data);

    // Wrap at end of col, row, bank
    if (direction == MT_ADDR_DIR_UP) {
      seqBaseAddrStartLo0.Data             = 0;
      seqBaseAddrStartHi0.Data             = 0;
      seqBaseAddrWrapLo0.Data              = 0;
      seqBaseAddrWrapHi0.Data              = 0;

      seqBaseAddrStartLo0.Bits.bank_addr   = bank;
      seqBaseAddrWrapLo0.Bits.bank_addr    = (1 << cpgcBankAddressBits) - (1 << baseBankBits) + bank;

      seqBaseAddrWrapLo0.Bits.column_addr  = (1 << cpgcColAddressBits) - ColIncSize;

      seqBaseAddrStartHi0.Bits.row_addr    = rowAddr[ch];
      seqBaseAddrWrapHi0.Bits.row_addr     = rowAddr[ch] + rowSize[ch] - 1;

    } else {

      seqBaseAddrStartLo0.Data             = 0;
      seqBaseAddrStartHi0.Data             = 0;
      seqBaseAddrWrapLo0.Data              = 0;
      seqBaseAddrWrapHi0.Data              = 0;

      seqBaseAddrStartLo0.Bits.bank_addr   = (1 << cpgcBankAddressBits) - (1 << baseBankBits) + bank;
      seqBaseAddrWrapLo0.Bits.bank_addr    = bank;

      seqBaseAddrStartLo0.Bits.column_addr = (1 << cpgcColAddressBits) - ColIncSize;

      seqBaseAddrStartHi0.Bits.row_addr    = rowAddr[ch] + rowSize[ch] - 1;
      seqBaseAddrWrapHi0.Bits.row_addr     = rowAddr[ch];
    }

    // Programs GPGC to run on the selected physical ranks
    SelectCPGCRanks (Host, socket, ch, 1 << logRank, BIT3 | BIT1, logSubRank);
    ClearDdrtForceMode (socket, ch);

    //
    // Sequence BAIN control
    //
    // bank, row, column
    seqBaincCtl00.Data = 0;
    seqBaincCtl10.Data = 0;
    seqBaincCtl00.Bits.column_ba_updscale = 0;
    seqBaincCtl00.Bits.row_ba_updscale    = 0;
    seqBaincCtl10.Bits.bank_ba_updscale   = 0;
    seqBaincCtl10.Bits.rank_ba_inc        = 0;
    seqBaincCtl10.Bits.rank_ba_updrate    = 0;
    seqBaincCtl10.Bits.rank_ba_updscale   = 0;
    seqBaincCtl10.Bits.bank_ba_updrate    = 0;

    if (direction == MT_ADDR_DIR_UP) {
      seqBaincCtl10.Bits.column_ba_inc_msb  = 0;
      seqBaincCtl00.Bits.column_ba_inc      = ColIncSize;
      seqBaincCtl00.Bits.row_ba_inc         = 1;
      seqBaincCtl10.Bits.bank_ba_inc        = (1 << baseBankBits);
    } else {
      seqBaincCtl10.Bits.column_ba_inc_msb  = (UINT32)-1;
      seqBaincCtl00.Bits.column_ba_inc      = (UINT32)-ColIncSize;
      seqBaincCtl00.Bits.row_ba_inc         = (UINT32)-1;
      seqBaincCtl10.Bits.bank_ba_inc        = (UINT32)-(1 << baseBankBits);
    }

    switch (AddressMode) {
    case FAST_Y:
      seqBaincCtl00.Bits.row_ba_updrate = cpgcColAddressBits + (cpgcBankAddressBits - baseBankBits);
      seqBaincCtl00.Bits.column_ba_updrate = (cpgcBankAddressBits - baseBankBits);
      break;
    case FAST_X:
      seqBaincCtl00.Bits.row_ba_updrate = (cpgcBankAddressBits - baseBankBits);
      seqBaincCtl00.Bits.column_ba_updrate = cpgcRowAddressBits + (cpgcBankAddressBits - baseBankBits);
      break;
    }

    //
    // Hook for error injection
    //
    if (GetInjectErrorMATS(Host, ch)) {
      seqBaseAddrStartLo0.Bits.column_addr  = 0x20;
      seqBaseAddrStartLo0.Bits.bank_addr = bank;
      seqBaseAddrWrapLo0.Bits.column_addr   = 0x0;
      seqBaseAddrWrapLo0.Bits.bank_addr = bank;
      seqBaincCtl10.Bits.column_ba_inc_msb  = 0;
      seqBaincCtl00.Bits.column_ba_inc      = 0;
      seqBaincCtl00.Bits.row_ba_inc         = 0;
      seqBaincCtl10.Bits.bank_ba_inc        = 0;
      seqBaincCtl10.Bits.bank_ba_updrate    = 0;
      seqBaincCtl00.Bits.column_ba_updrate  = 0;
      seqBaincCtl00.Bits.row_ba_updrate     = 0;
    }

    cpgcSeqBaoCic0.Data = 0;

    CpgcSetupAddrGen (Host, socket, ch,
                      seqBaincCtl00,
                      seqBaincCtl10,
                      seqBaseAddrStartLo0,
                      seqBaseAddrStartHi0,
                      seqBaseAddrWrapLo0,
                      seqBaseAddrWrapHi0,
                      cpgcSeqBaoCic0
                     );


  } // ch
  return;
} // CpgcMemTestRankSetupMATSRowRange


/**

  CPGC address setup for Advanced memory test

  @param Host             - Pointer to sysHost
  @param socket           - Memory controller to train
  @param ch               - ddr channel to setup
  @param wdbIncrRate      - increment rate
  @param wdbIncrScale     - increment scale
  @param wdbStartPntr     - start pointer
  @param wdbEndPntr       - end pointer
  @param setupPbwr        - flag to indicate setup of Pb write
  @param muxCtl           - mux control (0 - LMN, 1 - pat buffer,  2 - LFSR242)
  @param seqBaincCtl00       - base addresses increment fields
  @param seqBaincCtl10       - base addresses increment fields
  @param seqBaseAddrStartLo0 - base address start fields
  @param seqBaseAddrStartHi0 - base address start fields
  @param seqBaseAddrWrapLo0  - base address wrap fields
  @param seqBaseAddrWrapHi0  - base address wrap fields
  @param cpgcSeqBaoCic0      - base address update rate, carry fields
  @param mode                - Type of sequence MT_CPGC_WRITE, MT_CPGC_READ, or MT_CPGC_READ_WRITE

  @retval None

**/
STATIC VOID
CpgcSetupWdbMATS (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     wdbIncrRate,
  UINT8     wdbIncrScale,
  UINT8     wdbStartPntr,
  UINT8     wdbEndPntr,
  UINT8     setupPbwr,
  UINT8     muxCtl,
  CPGC_SEQ_BAINCCTL00_MC_MAIN_STRUCT          seqBaincCtl00,
  CPGC_SEQ_BAINCCTL10_MC_MAIN_STRUCT          seqBaincCtl10,
  CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_STRUCT   seqBaseAddrStartLo0,
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_STRUCT   seqBaseAddrStartHi0,
  CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_STRUCT    seqBaseAddrWrapLo0,
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_STRUCT    seqBaseAddrWrapHi0,
  CPGC_SEQ_BAOCIC0_MC_MAIN_STRUCT             cpgcSeqBaoCic0,
  UINT8     mode
  )
{
  UINT8 lfsr2code;
  UINT32 patBuf0rd;
  UINT32 patBuf1rd;
  UINT32 patBuf2rd;
  UINT32 patBuf0wr;
  UINT32 patBuf1wr;
  UINT32 patBuf2wr;
  CPGC_PATWDBCL_MUX0_PBRD_MCDDC_DP_STRUCT cpgcPatWdbclMux0Pbrd;
  CPGC_PATWDBCL_MUX1_PBRD_MCDDC_DP_STRUCT cpgcPatWdbclMux1Pbrd;
  CPGC_PATWDBCL_MUX2_PBRD_MCDDC_DP_STRUCT cpgcPatWdbclMux2Pbrd;
  CPGC_PATWDBCL_MUX0_PBWR_MCDDC_DP_STRUCT cpgcPatWdbclMux0Pbwr;
  CPGC_PATWDBCL_MUX1_PBWR_MCDDC_DP_STRUCT cpgcPatWdbclMux1Pbwr;
  CPGC_PATWDBCL_MUX2_PBWR_MCDDC_DP_STRUCT cpgcPatWdbclMux2Pbwr;
  CPGC_PATWDBCL_MUXCTL_MCDDC_DP_STRUCT    cpgcPatWdbClMuxCtl;
  CPGC_PATWDBCLCTL_MCDDC_CTL_STRUCT       cpgcPatWdbClCtl;

  RcDebugPrintWithDevice (SDBG_CPGC, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "CpgcSetupWdbMATS\n");

  //
  // Program the fixed patterns to emulate normal functional mode. The 3 patterns emulates
  // counter counting from 0-7.
  //
  patBuf0rd = DDR_CPGC_PATBUF_MUX0;
  patBuf1rd = DDR_CPGC_PATBUF_MUX1;
  patBuf2rd = DDR_CPGC_PATBUF_MUX2;

  patBuf0wr = DDR_CPGC_PATBUF_MUX0;
  patBuf1wr = DDR_CPGC_PATBUF_MUX1;
  patBuf2wr = DDR_CPGC_PATBUF_MUX2;

  lfsr2code = 0;

  //
  // Program CPGC WDB Mux Selection
  //
  // select 32-bit buffer/LFSR24
  // replace ECC byte with ecc_datasrc_sel
  // select BL0 to be used as the ECC data byte for comparison
  //
  cpgcPatWdbClMuxCtl.Data = 0;
  cpgcPatWdbClMuxCtl.Bits.mux0 = muxCtl;
  cpgcPatWdbClMuxCtl.Bits.mux1 = muxCtl;
  cpgcPatWdbClMuxCtl.Bits.mux2 = muxCtl;

  cpgcPatWdbClMuxCtl.Bits.ecc_replace_byte = 1;
  cpgcPatWdbClMuxCtl.Bits.ecc_datasrc_sel = 0;    // Use byte 0 to be compatible with PPR flow
  cpgcPatWdbClMuxCtl.Bits.save_lfsr_seedrate = 0;
  cpgcPatWdbClMuxCtl.Bits.rld_lfsr_seedrate = 0;
  cpgcPatWdbClMuxCtl.Bits.cpgc_lfsr_2code_mode = lfsr2code;
  MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUXCTL_MCDDC_DP_REG, cpgcPatWdbClMuxCtl.Data);

  //
  // Program CPGC WDB Pointer Control
  //
  cpgcPatWdbClCtl.Data = 0;
  cpgcPatWdbClCtl.Bits.incr_rate = wdbIncrRate;
  cpgcPatWdbClCtl.Bits.incr_scale = wdbIncrScale;
  cpgcPatWdbClCtl.Bits.start_pntr = wdbStartPntr;
  cpgcPatWdbClCtl.Bits.end_pntr = wdbEndPntr;
  MemWritePciCfgEp (socket, ch, CPGC_PATWDBCLCTL_MCDDC_CTL_REG, cpgcPatWdbClCtl.Data);

  //
  // Setup wr / rd buffers
  //
  // Program the fixed patterns to emulate normal functional mode. The 3 patterns emulates
  // counter counting from 0-7.
  //
  if (setupPbwr) {
    cpgcPatWdbclMux0Pbwr.Data = 0;
    cpgcPatWdbclMux0Pbwr.Bits.patbuffer = patBuf0wr;
    MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX0_PBWR_MCDDC_DP_REG, cpgcPatWdbclMux0Pbwr.Data);

    cpgcPatWdbclMux1Pbwr.Data = 0;
    cpgcPatWdbclMux1Pbwr.Bits.patbuffer = patBuf1wr;
    MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX1_PBWR_MCDDC_DP_REG, cpgcPatWdbclMux1Pbwr.Data);

    cpgcPatWdbclMux2Pbwr.Data = 0;
    cpgcPatWdbclMux2Pbwr.Bits.patbuffer = patBuf2wr;
    MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX2_PBWR_MCDDC_DP_REG, cpgcPatWdbclMux2Pbwr.Data);
  }

  cpgcPatWdbclMux0Pbrd.Data = 0;
  cpgcPatWdbclMux0Pbrd.Bits.patbuffer = patBuf0rd;
  MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX0_PBRD_MCDDC_DP_REG, cpgcPatWdbclMux0Pbrd.Data);

  cpgcPatWdbclMux1Pbrd.Data = 0;
  cpgcPatWdbclMux1Pbrd.Bits.patbuffer = patBuf1rd;
  MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX1_PBRD_MCDDC_DP_REG, cpgcPatWdbclMux1Pbrd.Data);

  cpgcPatWdbclMux2Pbrd.Data = 0;
  cpgcPatWdbclMux2Pbrd.Bits.patbuffer = patBuf2rd;
  MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX2_PBRD_MCDDC_DP_REG, cpgcPatWdbclMux2Pbrd.Data);

  //
  // Setup CPGC Address Generation
  //
  CpgcSetupAddrGen (Host, socket, ch,
                    seqBaincCtl00,
                    seqBaincCtl10,
                    seqBaseAddrStartLo0,
                    seqBaseAddrStartHi0,
                    seqBaseAddrWrapLo0,
                    seqBaseAddrWrapHi0,
                    cpgcSeqBaoCic0
                   );
  return;
} // CpgcSetupWdbMATS


/**

  CPGC global setup for Advanced memory test

  @param Host                         - Pointer to sysHost
  @param socket                       - Memory controller to train
  @param ch                           - ddr channel to setup
  @param rank                         - Logical rank number
  @param dqLoopCount                  - CPGC sequence loop count - number of times sequence is executed (2^(dqLoopCount - 1))
  @param testType                     - Advanced MemTest type
  @param mode                         - Type of sequence MT_CPGC_WRITE, MT_CPGC_READ, or MT_CPGC_READ_WRITE
  @param seqDataInv[MT_MAX_SUBSEQ]    - Enables pattern inversion per subsequence
  @param patternQW[MT_PATTERN_DEPTH]  - 64-bit wide data pattern to use per UI
  @param IsUseInvtPat                 - Info to indicate whether or not patternQW is inverted by comparing original pattern
  @param uiShl[MT_PATTERN_DEPTH]      - Enables pattern rotation per UI
  @param numCacheLines                - Number of cachelines to use in WDB
  @param patternDepth                 - Number of UI to use in patternQW
  @param direction                    - Sequential address direction MT_ADDR_DIR_UP, MT_ADDR_DIR_DN

  @retval SUCCESS

**/
UINT32
CpgcMemTestGlobalSetupMATS (
                       PSYSHOST  Host,
                       UINT8     socket,
                       UINT8     ch,
                       UINT8     rank,
                       UINT8     dqLoopCount,
                       UINT8     testType,
                       UINT8     mode,
                       UINT8     seqDataInv[MT_MAX_SUBSEQ],
                       UINT64_STRUCT patternQW[MT_PATTERN_DEPTH],
                       BOOLEAN   IsUseInvtPat,
                       UINT8     uiShl[MT_PATTERN_DEPTH],
                       UINT8     numCacheLines,
                       UINT8     patternDepth,
                       UINT8     direction
                       )
{

  UINT32                                        status;
  UINT8                                       mcId;
  UINT8                                       WdbIncrRate;
  ALIAS_CPGC_GLOBALCTRL_MC_MAIN_STRUCT        cpgcGlobalCtrlReg;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT            cpgcPatCadbCtlReg;
  CPGC_GLOBAL_TRAINING_SETUP                  cpgcGblTrainingSetup;
  CPGC_SEQ_BAINCCTL00_MC_MAIN_STRUCT          seqBaincCtl00;
  CPGC_SEQ_BAINCCTL10_MC_MAIN_STRUCT          seqBaincCtl10;
  CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_STRUCT   seqBaseAddrStartLo0;
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_STRUCT   seqBaseAddrStartHi0;
  CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_STRUCT    seqBaseAddrWrapLo0;
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_STRUCT    seqBaseAddrWrapHi0;
  CPGC_SEQ_BAOCIC0_MC_MAIN_STRUCT             cpgcSeqBaoCic0;

  RcDebugPrintWithDevice (SDBG_CPGC, socket, ch, NO_DIMM, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                 "CpgcMemTestSetupMATS:\n  pattern[0] = 0x%08x%08x\n  pattern[1] = 0x%08x%08x\n",
                 patternQW[0].hi, patternQW[0].lo, patternQW[1].hi, patternQW[1].lo);

  status = SUCCESS;

  cpgcGblTrainingSetup.seqLoopCnt        = dqLoopCount;                      // test will end after (2^(n-1)) loops through the sequence
  cpgcGblTrainingSetup.startDelay        = 0;                                // Delay start
  cpgcGblTrainingSetup.addrUpdRateMode   = CPGC_UPDATE_BY_CL;                // Update based on the # of read and write cacheline


  // Default to WRITE
  cpgcGblTrainingSetup.rwMode            = CPGC_BASE_WRITE_SUBSEQ;           // Base Read or write - subseq0

  // If MEM_TEST is read, set to READ
  if (mode == MT_CPGC_READ_WRITE || mode == MT_CPGC_READ) {
    cpgcGblTrainingSetup.rwMode          = CPGC_BASE_READ_SUBSEQ;            // Base Read or write - subseq0
  }

  cpgcGblTrainingSetup.numCacheLines     = 0;                                // Number of Cachelines (2^N)
  cpgcGblTrainingSetup.burstSeparation   = 0;                                // (sub_seqwait) - need to add delay to drain credits if doing write / read subseq
  cpgcGblTrainingSetup.errEnChunk        = CPGC_ERR_MONITOR_ALL_CHUNK;       // monitor errors on any chunk
  cpgcGblTrainingSetup.errEnCl           = CPGC_ERR_MONITOR_ALL_CL;          // monitor errors on any cacheline
  if (testType == ADV_MT_TYPE_LTEST_SCRAM) {
    cpgcGblTrainingSetup.stopOnErrCtl      = CPGC_ERR_STOP_ON_ALL_ERR;         // Stop on all lane error
  } else {
    cpgcGblTrainingSetup.stopOnErrCtl      = 1;                                // Stop on first error
  }

  if (mode == MT_CPGC_READ_WRITE) {
    cpgcGblTrainingSetup.subseqEndPtr    = 1;                                // subseq end pointer
    cpgcGblTrainingSetup.useSubSeq1      = CPGC_BASE_WRITE_SUBSEQ;
  } else {
    cpgcGblTrainingSetup.subseqEndPtr    = 0;                                // subseq end pointer
    cpgcGblTrainingSetup.useSubSeq1      = CPGC_NO_SUBSEQ1;                  // do not use sub seq1 (base read)
  }
  cpgcGblTrainingSetup.cpgcLinkTrain     = 0;                                // do not use cpgc link train
  cpgcGblTrainingSetup.back2backWrEn     = 1;                                // enable b2b writes
  cpgcGblTrainingSetup.mprControl        = 0;                                // MPR disable
  cpgcGblTrainingSetup.refMask           = 0;                                // enable cpgc self refresh
  cpgcGblTrainingSetup.zqMask            = 0;                                // enable cpgc zq
  cpgcGblTrainingSetup.x8Mode            = 0;

  CpgcGlobalTrainingSetup (Host, socket, ch, &cpgcGblTrainingSetup);

  //
  // Set WDB address order
  //
  seqBaincCtl00.Data = 0;
  seqBaincCtl10.Data = 0;
  cpgcSeqBaoCic0.Data = 0;
  seqBaseAddrStartLo0.Data = 0;
  seqBaseAddrStartHi0.Data = 0;
  seqBaseAddrWrapLo0.Data = 0;
  seqBaseAddrWrapHi0.Data = 0;

  if (testType == ADV_MT_TYPE_DATA_RET) {
    //
    // If Burst Length 4 do - 2 column bits, else -3
    // Assume all DDR4 tech has 10 column bits
    //
    mcId = GetMCID (Host, socket, ch);
    cpgcGlobalCtrlReg.Data = MemReadPciCfgMC (socket, mcId, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG);
    if (cpgcGlobalCtrlReg.Bits.use_bl4_col_addr == 1) {
      WdbIncrRate = 8;
    } else {
      WdbIncrRate = 7;
    }
  } else {
    WdbIncrRate = 0;
  }

  CpgcSetupWdbMATS (Host, socket, ch,
                WdbIncrRate,                 // wdbIncrRate
                0,                           // wdbIncrScale
                0,                           // wdbStartPntr (victim bit 0)
                numCacheLines - 1,           // wdbEndPntr (victim bit 0)
                1,                           // setupPbwr
                CPGC_WDB_MUX_CTL_PATBUF,     // mux control
                seqBaincCtl00,               // column base address incr
                seqBaincCtl10,
                seqBaseAddrStartLo0,
                seqBaseAddrStartHi0,
                seqBaseAddrWrapLo0,
                seqBaseAddrWrapHi0,
                cpgcSeqBaoCic0,
                mode
               );

  //
  // Disable CADB
  //
  cpgcPatCadbCtlReg.Data = 0;
  MemWritePciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG, cpgcPatCadbCtlReg.Data);

  return status;

} // CpgcMemTestGlobalSetupMATS


/**

  Collect DDR advanced command CPGC error status

  @param Host                - Pointer to sysHost
  @param socket              - Memory controller number
  @param ddrChEnMap          - channel mask
  @param cpgcErrorStatus[MAX_CH] - array of error status

  @retval   SUCCESS/FAILURE

**/
UINT32
CpgcAdvTrainingErrorStatusMATS (
  PSYSHOST          Host,
  UINT8             socket,
  UINT32            ddrChEnMap,
  CPGC_ERROR_STATUS_MATS cpgcErrorStatus[MAX_CH]
  )
{

  UINT32 status;
  UINT8 mcId;
  UINT8 ch;
  CPGC_ERROR_STATUS                         cpgcErrorStatusLegacy[MAX_CH];
  CPGC_ERR_ADDR0_MCDDC_CTL_STRUCT           cpgcErrAddr0;
  CPGC_ERR_ADDR1_MCDDC_CTL_STRUCT           cpgcErrAddr1;
  CPGC_SEQ_RANKLG2PHYMAP0_MC_MAIN_STRUCT    seqRankLg2PhyMap0;
  CPGC_SEQ_LRRANKLG2PHYMAP0_MC_MAIN_STRUCT  seqLrRankLg2PhyMap0;
  UINT8                                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  SetMem(cpgcErrorStatus, sizeof(CPGC_ERROR_STATUS_MATS) * MaxChDdr, 0);

  status = CpgcAdvTrainingErrorStatusWorker (Host, socket, ddrChEnMap, cpgcErrorStatusLegacy);

  for (ch = 0; ch < MaxChDdr; ch++) {

    // skip inactive channels
    if (!(ddrChEnMap & (1 << ch))) {
      continue;
    }

    cpgcErrorStatus[ch].cpgcErrDat0S = cpgcErrorStatusLegacy[ch].cpgcErrDat0S;
    cpgcErrorStatus[ch].cpgcErrDat1S = cpgcErrorStatusLegacy[ch].cpgcErrDat1S;
    cpgcErrorStatus[ch].cpgcErrDat2S = cpgcErrorStatusLegacy[ch].cpgcErrDat2S;
    cpgcErrorStatus[ch].cpgcErrDat3S = cpgcErrorStatusLegacy[ch].cpgcErrDat3S;
    cpgcErrorStatus[ch].cpgcErrEccS  = cpgcErrorStatusLegacy[ch].cpgcErrEccS;

    mcId = GetMCID (Host, socket, ch);
    seqLrRankLg2PhyMap0.Data = MemReadPciCfgMC_Ch (socket, ch, CPGC_SEQ_LRRANKLG2PHYMAP0_MC_MAIN_REG, 4);
    cpgcErrorStatus[ch].cpgcSubrank = seqLrRankLg2PhyMap0.Bits.lr0_mapping;

    seqRankLg2PhyMap0.Data = MemReadPciCfgMC_Ch (socket, ch, CPGC_SEQ_RANKLG2PHYMAP0_MC_MAIN_REG, 4);
    cpgcErrorStatus[ch].cpgcErrRank = seqRankLg2PhyMap0.Bits.rank0_mapping;

    cpgcErrorStatus[ch].cpgcErrChunk = MemReadPciCfgEp (socket, ch, CPGC_ERR_CHUNK_MCDDC_DP_REG);
    cpgcErrAddr0.Data = MemReadPciCfgEp (socket, ch, CPGC_ERR_ADDR0_MCDDC_CTL_REG);
    cpgcErrorStatus[ch].cpgcErrRow = cpgcErrAddr0.Bits.row & CPGC_ERR_ADDR0_ROW_BITS_MASK;
    cpgcErrAddr1.Data = MemReadPciCfgEp (socket, ch, CPGC_ERR_ADDR1_MCDDC_CTL_REG);

    // Assume that bit-1 maps to CA[2] in DRAM spec
    cpgcErrorStatus[ch].cpgcErrColumn = (cpgcErrAddr1.Bits.column & ~0x400) << 2;

    //
    // Swap CPGC bank[3:2] with bank[1:0] based on empirical results of testing
    //
    cpgcErrorStatus[ch].cpgcErrBank = ((cpgcErrAddr1.Bits.bank & 0x3) << 2) | ((cpgcErrAddr1.Bits.bank >> 2) & 3);

    RcDebugPrintWithDevice (SDBG_CPGC, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "CpgcAdvTrainingErrorStatusMATS: Row = 0x%06x, Column = 0x%04x, Bank = 0x%02x, Chunk = 0x%02x, cpgcErr0 = 0x%08x, cpgcErr1 = 0x%08x, cpgcErr2 = 0x%08x, cpgcErr3 = 0x%08x, cpgcErrEccS = 0x%04x\n",
                    cpgcErrorStatus[ch].cpgcErrRow,
                    cpgcErrorStatus[ch].cpgcErrColumn,
                    cpgcErrorStatus[ch].cpgcErrBank,
                    cpgcErrorStatus[ch].cpgcErrChunk,
                    cpgcErrorStatus[ch].cpgcErrDat0S,
                    cpgcErrorStatus[ch].cpgcErrDat1S,
                    cpgcErrorStatus[ch].cpgcErrDat2S,
                    cpgcErrorStatus[ch].cpgcErrDat3S,
                    cpgcErrorStatus[ch].cpgcErrEccS
      );

  } // ch

  return status;
} // CpgcAdvTrainingErrorStatusMATS

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
  )
{
return FAILURE;
} // CpgcMemTestGlobalSetup


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
             PSYSHOST  Host,
             UINT8     socket,
             UINT8     ch,
             UINT8     wdbIncrRate,
             UINT8     wdbIncrScale,
             UINT8     wdbStartPntr,
             UINT8     wdbEndPntr,
             UINT8     setupPbwr,
             UINT8     muxCtl,
             CPGC_SEQ_BAINCCTL00_MC_MAIN_STRUCT            seqBaincCtl00,
             CPGC_SEQ_BAINCCTL10_MC_MAIN_STRUCT            seqBaincCtl10,
             CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_STRUCT     seqBaseAddrStartLo0,
             CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_STRUCT     seqBaseAddrStartHi0,
             CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_STRUCT      seqBaseAddrWrapLo0,
             CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_STRUCT      seqBaseAddrWrapHi0,
             CPGC_SEQ_BAOCIC0_MC_MAIN_STRUCT               cpgcSeqBaoCic0
             )
{
  UINT8 scheduler;
  UINT8 lfsr2code;
  UINT32 lfsrSeedOffset;
  UINT32 patBuf0;
  UINT32 patBuf1;
  UINT32 patBuf2;
  CPGC_PATWDBCL_MUXCTL_MCDDC_DP_STRUCT    cpgcPatWdbClMuxCtl;
  CPGC_PATWDBCLCTL_MCDDC_CTL_STRUCT       cpgcPatWdbClCtl;
  CPGC_PATWDBCL_MUX0_PBRD_MCDDC_DP_STRUCT cpgcPatWdbclMux0Pbrd;
  CPGC_PATWDBCL_MUX1_PBRD_MCDDC_DP_STRUCT cpgcPatWdbclMux1Pbrd;
  CPGC_PATWDBCL_MUX2_PBRD_MCDDC_DP_STRUCT cpgcPatWdbclMux2Pbrd;
  CPGC_PATWDBCL_MUX0_PBWR_MCDDC_DP_STRUCT cpgcPatWdbclMux0Pbwr;
  CPGC_PATWDBCL_MUX1_PBWR_MCDDC_DP_STRUCT cpgcPatWdbclMux1Pbwr;
  CPGC_PATWDBCL_MUX2_PBWR_MCDDC_DP_STRUCT cpgcPatWdbclMux2Pbwr;

  RcDebugPrintWithDevice (SDBG_CPGC, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "CpgcSetupWdb\n");

  // TODO: update this. does not work the same for EP
  scheduler = ch % 2;

  //
  // Program the fixed patterns to emulate normal functional mode. The 3 patterns emulates
  // counter counting from 0-7.
  //
  patBuf0 = DDR_CPGC_PATBUF_MUX0;
  patBuf1 = DDR_CPGC_PATBUF_MUX1;
  patBuf2 = DDR_CPGC_PATBUF_MUX2;
  lfsr2code = 0;

  //
  // patbuffer is either fixed pattern buffer or LFSR24 seed depending on mux control
  //
  if (muxCtl == CPGC_WDB_MUX_CTL_LFSR24) {
    //
    // Program LFSR seeds. Seed are different for each scheduler. Also seeds change for each test run.
    //
    lfsrSeedOffset = Host->var.mem.initialLFSRSeed++;
    patBuf0 = CPGC_LFSR0_INIT_SEED_HSX (scheduler) + lfsrSeedOffset;
    patBuf1 = CPGC_LFSR1_INIT_SEED_HSX (scheduler) + lfsrSeedOffset;
    patBuf2 = CPGC_LFSR2_INIT_SEED_HSX (scheduler) + lfsrSeedOffset;
  } else if (muxCtl == CPGC_WDB_MUX_CTL_LFSR24_PATBUF){
      lfsrSeedOffset = Host->var.mem.initialLFSRSeed++;
      patBuf0 = CPGC_LFSR0_INIT_SEED_HSX (scheduler) + lfsrSeedOffset;
      patBuf1 = DDR_CPGC_PATBUF_MUX3;
      patBuf2 = CPGC_LFSR1_INIT_SEED_HSX (scheduler) + lfsrSeedOffset;
  }

  //
  // Program CPGC WDB Mux Selection
  //
  // select 32-bit buffer/LFSR24
  // replace ECC byte with ecc_datasrc_sel
  // select BL0 to be used as the ECC data byte for comparison
  //
  cpgcPatWdbClMuxCtl.Data = 0;
  if (muxCtl == CPGC_WDB_MUX_CTL_LFSR24_PATBUF) {
    cpgcPatWdbClMuxCtl.Bits.mux0 = CPGC_WDB_MUX_CTL_LFSR24;
    cpgcPatWdbClMuxCtl.Bits.mux1 = CPGC_WDB_MUX_CTL_PATBUF;
    cpgcPatWdbClMuxCtl.Bits.mux2 = CPGC_WDB_MUX_CTL_LFSR24;
  } else {
    cpgcPatWdbClMuxCtl.Bits.mux0 = muxCtl;
    cpgcPatWdbClMuxCtl.Bits.mux1 = muxCtl;
    cpgcPatWdbClMuxCtl.Bits.mux2 = muxCtl;
  }
  cpgcPatWdbClMuxCtl.Bits.ecc_replace_byte = 1;
  cpgcPatWdbClMuxCtl.Bits.ecc_datasrc_sel = 0;    // Use byte 0 to be compatible with PPR flow
  cpgcPatWdbClMuxCtl.Bits.save_lfsr_seedrate = 0;
  cpgcPatWdbClMuxCtl.Bits.rld_lfsr_seedrate = 0;
  cpgcPatWdbClMuxCtl.Bits.cpgc_lfsr_2code_mode = lfsr2code;
  MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUXCTL_MCDDC_DP_REG, cpgcPatWdbClMuxCtl.Data);

  //
  // Program CPGC WDB Pointer Control
  //
  cpgcPatWdbClCtl.Data = 0;
  cpgcPatWdbClCtl.Bits.incr_rate = wdbIncrRate;
  cpgcPatWdbClCtl.Bits.incr_scale = wdbIncrScale;
  cpgcPatWdbClCtl.Bits.start_pntr = wdbStartPntr;
  cpgcPatWdbClCtl.Bits.end_pntr = wdbEndPntr;
  MemWritePciCfgEp (socket, ch, CPGC_PATWDBCLCTL_MCDDC_CTL_REG, cpgcPatWdbClCtl.Data);

  //
  // Setup wr / rd buffers
  //
  // Program the fixed patterns to emulate normal functional mode. The 3 patterns emulates
  // counter counting from 0-7.
  //
  if (setupPbwr) {
    cpgcPatWdbclMux0Pbwr.Data = 0;
    cpgcPatWdbclMux0Pbwr.Bits.patbuffer = patBuf0;
    MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX0_PBWR_MCDDC_DP_REG, cpgcPatWdbclMux0Pbwr.Data);

    cpgcPatWdbclMux1Pbwr.Data = 0;
    cpgcPatWdbclMux1Pbwr.Bits.patbuffer = patBuf1;
    MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX1_PBWR_MCDDC_DP_REG, cpgcPatWdbclMux1Pbwr.Data);

    cpgcPatWdbclMux2Pbwr.Data = 0;
    cpgcPatWdbclMux2Pbwr.Bits.patbuffer = patBuf2;
    MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX2_PBWR_MCDDC_DP_REG, cpgcPatWdbclMux2Pbwr.Data);
  }

  cpgcPatWdbclMux0Pbrd.Data = 0;
  cpgcPatWdbclMux0Pbrd.Bits.patbuffer = patBuf0;
  MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX0_PBRD_MCDDC_DP_REG, cpgcPatWdbclMux0Pbrd.Data);

  cpgcPatWdbclMux1Pbrd.Data = 0;
  cpgcPatWdbclMux1Pbrd.Bits.patbuffer = patBuf1;
  MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX1_PBRD_MCDDC_DP_REG, cpgcPatWdbclMux1Pbrd.Data);

  cpgcPatWdbclMux2Pbrd.Data = 0;
  cpgcPatWdbclMux2Pbrd.Bits.patbuffer = patBuf2;
  MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX2_PBRD_MCDDC_DP_REG, cpgcPatWdbclMux2Pbrd.Data);

  //
  // Setup CPGC Address Generation
  //
  CpgcSetupAddrGen (Host, socket, ch,
                    seqBaincCtl00,
                    seqBaincCtl10,
                    seqBaseAddrStartLo0,
                    seqBaseAddrStartHi0,
                    seqBaseAddrWrapLo0,
                    seqBaseAddrWrapHi0,
                    cpgcSeqBaoCic0
                   );
  return;
} // CpgcSetupWdb

/**

  Setup CPGC base address generation

  @param Host             - pointer to sysHost
  @param socket           - socket to train
  @param ch               - ddr channel

  @retval n/a

**/
VOID
CpgcSetupAddrGen (
                 PSYSHOST                                      Host,
                 UINT8                                         socket,
                 UINT8                                         ch,
                 CPGC_SEQ_BAINCCTL00_MC_MAIN_STRUCT            seqBaincCtl00,
                 CPGC_SEQ_BAINCCTL10_MC_MAIN_STRUCT            seqBaincCtl10,
                 CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_STRUCT     seqBaseAddrStartLo0,
                 CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_STRUCT     seqBaseAddrStartHi0,
                 CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_STRUCT      seqBaseAddrWrapLo0,
                 CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_STRUCT      seqBaseAddrWrapHi0,
                 CPGC_SEQ_BAOCIC0_MC_MAIN_STRUCT               cpgcSeqBaoCic0
                 )
{

  RcDebugPrintWithDevice (SDBG_CPGC, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "CpgcSetupAddrGen\n");

  //
  // CPGC Address Generation
  //

  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_REG, 4, seqBaseAddrStartLo0.Data);
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_REG, 4, seqBaseAddrStartHi0.Data);

  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_REG, 4, seqBaseAddrWrapLo0.Data);
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_REG, 4, seqBaseAddrWrapHi0.Data);

  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQ_BAINCCTL00_MC_MAIN_REG, 8, seqBaincCtl00.Data);
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQ_BAINCCTL10_MC_MAIN_REG, 8, seqBaincCtl10.Data);

  //
  // Note: For the MPR read, the row/column addresses does not matter.
  // When the DRAM device is in that mode, all reads return the MPR pattern - alternating 0 and 1 on DQ lines.
  // That is regardless of what row/column addresses of the read command.
  //
  // Disable the carry enable for column address wrap. As it is eanbled by default.
  // If enabled, Row address will get incremented when column wrap address is hit.
  //
  //
  // Note: A0 bug cryen is broken. Disable cryen for all cases
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQ_BAOCIC0_MC_MAIN_REG, 4, cpgcSeqBaoCic0.Data);
  return;
} // CpgcSetupAddrGen


STATIC UINT32
CpgcInitializeTransientErrChecking (
  PSYSHOST          Host,
  UINT8             socket,
  UINT8             ch
  )
/*++

Routine Description:

  Initialize DDR advanced command CPGC error status based err counters to eliminate transient errors.

Arguments:

  Host                - Pointer to sysHost
  socket              - Socket to train
  ch                  - Channel ID

Returns:

  SUCCESS / FAILURE

Notes:

    0 = no error
    1 = error


--*/
{
  UINT8     strobe;
  UINT32    status;
  CPGC_ERR_COUNTER0_MCDDC_CTL_STRUCT  cpgcErrCounter0;
  UINT32    csrBase;
  BOOLEAN   SkipExecution;
  UINT8 MSVx8;
  MSVx8 = GetMaxStrobeValid(Host) / 2;
  RcDebugPrintWithDevice (SDBG_CPGC, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CpgcInitializeTransientErrChecking Starts\n");
  status = SUCCESS;
  csrBase = MemPciCfgOffset (socket, ch, CPGC_ERR_COUNTER0_MCDDC_CTL_REG);

  ExecuteCpgcInitializeTransientErrCheckingTarget  (&SkipExecution);
  if (SkipExecution == TRUE) {
    return SUCCESS;
  }

  for (strobe = 0; strobe < MSVx8; strobe++) {
    cpgcErrCounter0.Data = MemReadPciCfgEp (socket, ch, csrBase + (8 * strobe));

    // Count bytes
    cpgcErrCounter0.Bits.ctl  = 2;
    cpgcErrCounter0.Bits.pntr = 2 * strobe;

    MemWritePciCfgEp (socket, ch, csrBase + (8 * strobe), cpgcErrCounter0.Data);
  } // for strobe

  return status;

} // CpgcInitializeTransientErrChecking

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
                        PSYSHOST                    Host,
                        UINT8                       socket,
                        UINT8                       ch,
                        CPGC_GLOBAL_TRAINING_SETUP  *cpgcGblTrainingSetup
                        )
{

  UINT32                              status;
  UINT8                               startDelay;
  UINT8                               addrUpdRateMode;
  UINT8                               seqLoopCnt;
  UINT8                               rwMode;
  UINT8                               numCacheLines;
  UINT8                               burstSeparation;
  UINT8                               errEnChunk;
  UINT8                               errEnCl;
  UINT8                               stopOnErrCtl;
  UINT8                               subseqEndPtr;
  UINT8                               useSubSeq1;
  UINT8                               refMask;
  UINT8                               zqMask;
  UINT8                               mcId;
  CPGC_SEQCTL0_MC_MAIN_STRUCT         cpgcSeqCtl0Reg;
  CPGC_MISCCKECTL_MCDDC_CTL_STRUCT    cpgcMiscCkeCtlReg;
  CPGC_MISCREFCTL_MCDDC_CTL_STRUCT    cpgcMiscRefCtlReg;
  CPGC_MISCZQCTL_MCDDC_CTL_STRUCT     cpgcMiscZQCtlReg;
  CPGC_MISCODTCTL_MCDDC_CTL_STRUCT    cpgcMiscOdtCtlReg;
  CPGC_SUBSEQ0_CTL0_MC_MAIN_STRUCT    cpgcSubseq0Ctl0Reg;
  CPGC_SUBSEQ1_CTL0_MC_MAIN_STRUCT    cpgcSubseq1Ctl0Reg;
  CPGC_ERR_CTL_MCDDC_CTL_STRUCT       cpgcErrCtlReg;
  ALIAS_CPGC_GLOBALCTRL_MC_MAIN_STRUCT cpgcGlobalCtrlReg;

  mcId = GetMCID (Host, socket, ch);

  status = SUCCESS;


  RcDebugPrintWithDevice (SDBG_CPGC, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "CpgcGlobalTrainingSetup\n");

  startDelay          = cpgcGblTrainingSetup->startDelay;
  addrUpdRateMode     = cpgcGblTrainingSetup->addrUpdRateMode;
  seqLoopCnt          = cpgcGblTrainingSetup->seqLoopCnt;
  rwMode              = cpgcGblTrainingSetup->rwMode;
  numCacheLines       = cpgcGblTrainingSetup->numCacheLines;
  burstSeparation     = cpgcGblTrainingSetup->burstSeparation;
  errEnChunk          = cpgcGblTrainingSetup->errEnChunk;
  errEnCl             = cpgcGblTrainingSetup->errEnCl;
  stopOnErrCtl        = cpgcGblTrainingSetup->stopOnErrCtl;
  subseqEndPtr        = cpgcGblTrainingSetup->subseqEndPtr;
  useSubSeq1          = cpgcGblTrainingSetup->useSubSeq1;
  refMask             = cpgcGblTrainingSetup->refMask;
  zqMask              = cpgcGblTrainingSetup->zqMask;

  //
  // program scheduler (based on ch)
  //

  //
  // MC Controller Init - disable miscellaneous events
  // functional CKE logic, do not disable CKE powerdown,
  // disable refreshes for all ranks
  // disable ZQCal for all ranks
  //
  cpgcMiscCkeCtlReg.Data = MemReadPciCfgEp (socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG);
  cpgcMiscRefCtlReg.Data = MemReadPciCfgEp (socket, ch, CPGC_MISCREFCTL_MCDDC_CTL_REG);
  cpgcMiscZQCtlReg.Data  = MemReadPciCfgEp (socket, ch, CPGC_MISCZQCTL_MCDDC_CTL_REG);
  cpgcMiscOdtCtlReg.Data  = MemReadPciCfgEp (socket, ch, CPGC_MISCODTCTL_MCDDC_CTL_REG);

  cpgcMiscCkeCtlReg.Bits.cke_override     = 0;
  cpgcMiscRefCtlReg.Bits.ref_rankmask     = refMask;
  cpgcMiscZQCtlReg.Bits.zq_rankmask       = zqMask;
  cpgcMiscOdtCtlReg.Bits.odt_override     = 0;
  cpgcMiscOdtCtlReg.Bits.mpr_train_ddr_on = 0;

  if (!IsBrsPresent(Host, socket)) {
  MemWritePciCfgEp (socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG, cpgcMiscCkeCtlReg.Data);
  }
  MemWritePciCfgEp (socket, ch, CPGC_MISCREFCTL_MCDDC_CTL_REG, cpgcMiscRefCtlReg.Data);
  MemWritePciCfgEp (socket, ch, CPGC_MISCZQCTL_MCDDC_CTL_REG, cpgcMiscZQCtlReg.Data);
  MemWritePciCfgEp (socket, ch, CPGC_MISCODTCTL_MCDDC_CTL_REG, cpgcMiscOdtCtlReg.Data);

  //
  // Write CPGC Registers
  //

  //
  // enables this sequencer to be controlled by global controls
  // REUT loopback mode, enter loopback.marker state
  // delay the start sequence, !! make training pref
  //

  cpgcSeqCtl0Reg.Data = MemReadPciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4);
  cpgcSeqCtl0Reg.Bits.global_control            = 1;
  cpgcSeqCtl0Reg.Bits.initialization_mode       = CPGC_Testing_Mode; // Test Mode
  cpgcSeqCtl0Reg.Bits.loopcount                 = seqLoopCnt;
  cpgcSeqCtl0Reg.Bits.address_update_rate_mode  = addrUpdRateMode;
  cpgcSeqCtl0Reg.Bits.subseq_start_pointer      = 0;
  cpgcSeqCtl0Reg.Bits.subseq_end_pointer        = subseqEndPtr;
  cpgcSeqCtl0Reg.Bits.start_test_delay          = startDelay;
  cpgcSeqCtl0Reg.Bits.link_wrtrn_enb            = 0;

  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, cpgcSeqCtl0Reg.Data);

  //
  // Subsequence scheduler Control:
  // exponential num cacheline scale
  // set base read /write mode
  //

  cpgcSubseq0Ctl0Reg.Data = MemReadPciCfgMC_Ch (socket, ch, CPGC_SUBSEQ0_CTL0_MC_MAIN_REG, 0x50);
  cpgcSubseq0Ctl0Reg.Bits.subseq_type           = rwMode;
  cpgcSubseq0Ctl0Reg.Bits.num_cachelines        = numCacheLines;
  cpgcSubseq0Ctl0Reg.Bits.num_cachelines_scale  = 0;
  cpgcSubseq0Ctl0Reg.Bits.subseq_wait           = burstSeparation;
  cpgcSubseq0Ctl0Reg.Bits.rstcurr_ba_start      = 1; //reset address for next test
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SUBSEQ0_CTL0_MC_MAIN_REG, 0x50, cpgcSubseq0Ctl0Reg.Data);

  if (useSubSeq1 != CPGC_NO_SUBSEQ1) {
    cpgcSubseq1Ctl0Reg.Data = MemReadPciCfgMC_Ch (socket, ch, CPGC_SUBSEQ1_CTL0_MC_MAIN_REG, 0x50);
    cpgcSubseq1Ctl0Reg.Bits.subseq_type           = useSubSeq1;
    cpgcSubseq1Ctl0Reg.Bits.num_cachelines        = numCacheLines;
    cpgcSubseq1Ctl0Reg.Bits.num_cachelines_scale  = 0;
    cpgcSubseq1Ctl0Reg.Bits.subseq_wait           = burstSeparation;
    cpgcSubseq1Ctl0Reg.Bits.rstcurr_ba_start      = 1; //reset address for next test
    MemWritePciCfgMC_Ch (socket, ch, CPGC_SUBSEQ1_CTL0_MC_MAIN_REG, 0x50, cpgcSubseq1Ctl0Reg.Data);
  }

  //
  // Error Control Registers
  //  stop on nth error (must be 1)
  //  do not stop on error
  //  configure monitor errors on all cachelines
  //  do not monitor errors on all chunks
  //
  cpgcErrCtlReg.Data = 0;
  cpgcErrCtlReg.Bits.stop_on_ntherr           = 1;
  cpgcErrCtlReg.Bits.stop_on_errcontrol       = 0;
  cpgcErrCtlReg.Bits.selective_err_enb_cl     = errEnCl;

  cpgcGlobalCtrlReg.Data = MemReadPciCfgMC (socket, mcId, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG);
  if (cpgcGlobalCtrlReg.Bits.use_bl4_col_addr) {
    //
    // Mask off the odd chunks if in burst length 4 mode
    //
    cpgcErrCtlReg.Bits.selective_err_enb_chunk  = errEnChunk & 3;
  } else {
    cpgcErrCtlReg.Bits.selective_err_enb_chunk  = errEnChunk;
  }
  cpgcErrCtlReg.Bits.stop_on_errcontrol       = stopOnErrCtl;
  cpgcErrCtlReg.Bits.cpgc_cntr_x8_mode        = cpgcGblTrainingSetup->x8Mode;
  MemWritePciCfgEp (socket, ch, CPGC_ERR_CTL_MCDDC_CTL_REG, cpgcErrCtlReg.Data);

  // This feature not enabled yet
  //CpgcInitializeTransientErrChecking (Host, socket, ch);
  return status;
} // CpgcGlobalTrainingSetup


/**

  Collect DDR advanced command CPGC error status

  @param Host                - Pointer to sysHost
  @param Socket              - Socket to train
  @param DdrChEnMap          - 4-bit bit mask of ddr channels to test - (1 = active channel)
  @param CpgcErrorStatus     - error status for each channel

  @retval SUCCESS / FAILURE

  Notes:

  0 = no error
  1 = error

**/
UINT32
CpgcAdvTrainingErrorStatusWorker (
  PSYSHOST          Host,
  UINT8             Socket,
  UINT32            DdrChEnMap,
  CPGC_ERROR_STATUS CpgcErrorStatus[MAX_CH]
  )
{

  UINT32 Status;
  UINT8 Ch;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Status = SUCCESS;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {

    // skip inactive channels
    if (!(DdrChEnMap & (1 << Ch))) {
      continue;
    }

    CpgcErrorStatus[Ch].cpgcErrDat0S = MemReadPciCfgEp (Socket, Ch, CPGC_ERR_DATA0_S_MCDDC_CTL_REG);
    CpgcErrorStatus[Ch].cpgcErrDat1S = MemReadPciCfgEp (Socket, Ch, CPGC_ERR_DATA1_S_MCDDC_CTL_REG);
    CpgcErrorStatus[Ch].cpgcErrDat2S = MemReadPciCfgEp (Socket, Ch, CPGC_ERR_DATA2_S_MCDDC_CTL_REG);
    CpgcErrorStatus[Ch].cpgcErrDat3S = MemReadPciCfgEp (Socket, Ch, CPGC_ERR_DATA3_S_MCDDC_CTL_REG);
    if (Host->nvram.mem.eccEn) {
      CpgcErrorStatus[Ch].cpgcErrEccS = MemReadPciCfgEp (Socket, Ch, CPGC_ERR_ECC_S_MCDDC_CTL_REG);
    } else {
      CpgcErrorStatus[Ch].cpgcErrEccS = 0;
    }

  } // Ch

  return Status;

} // CpgcAdvTrainingErrorStatusWorker


/**

  Collect DDR advanced command CPGC error status

  @param[in]  Host                - Pointer to sysHost
  @param[in]  socket              - Socket to train
  @param[in]  ddrChEnMap          - 4-bit bit mask of ddr channels to test - (1 = active channel)
  @param[out] cpgcErrorStatus     - error status for each channel

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcAdvTrainingErrorStatus (
  IN     PSYSHOST          Host,
  IN     UINT8             socket,
  IN     UINT32            ddrChEnMap,
     OUT CPGC_ERROR_STATUS cpgcErrorStatus[MAX_CH]
  )
{

  UINT32 status;
  UINT8 ch;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  status = CpgcAdvTrainingErrorStatusWorker (Host, socket, ddrChEnMap, cpgcErrorStatus);

  for (ch = 0; ch < MaxChDdr; ch++) {

    // skip inactive channels
    if (!(ddrChEnMap & (1 << ch))) {
      continue;
    }

#ifdef DDR5_SUPPORT
    if (IsBrsPresent (Host, socket)) {
      //
      // HSD-ES: 1706873035 & 1707047993
      //
      BrsCpgcErrorStatusDeswizzle (&cpgcErrorStatus[ch].cpgcErrDat0S, 0);
      BrsCpgcErrorStatusDeswizzle (&cpgcErrorStatus[ch].cpgcErrDat2S, 0);
      if ((ch % 2) == 0) {
        //
        // Original code used IF (SubCh == 1)
        // During 40b Mode execution Ch0 is SubCh1 in DDR5
        //
        BrsCpgcErrorStatusDeswizzle (&cpgcErrorStatus[ch].cpgcErrEccS, 1);
      } else {
        BrsCpgcErrorStatusDeswizzle (&cpgcErrorStatus[ch].cpgcErrEccS, 0);
      }
      //
      // Account for CPGC mapping on BRS
      // Move byte 8 into byte 4 position
      //
      cpgcErrorStatus[ch].cpgcErrDat1S = cpgcErrorStatus[ch].cpgcErrEccS & 0xFF;
      cpgcErrorStatus[ch].cpgcErrDat3S = (cpgcErrorStatus[ch].cpgcErrEccS >> 8) & 0xFF;
    }
#endif // #ifdef DDR5_SUPPORT
    RcDebugPrintWithDevice (SDBG_CPGC, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "CpgcAdvTrainingErrorStatus: cpgcErr0 = 0x%x, cpgcErr1 = 0x%x, cpgcErr2 = 0x%x, cpgcErr3 = 0x%x, cpgcErrEccS = 0x%x\n",
                   cpgcErrorStatus[ch].cpgcErrDat0S,
                   cpgcErrorStatus[ch].cpgcErrDat1S,
                   cpgcErrorStatus[ch].cpgcErrDat2S,
                   cpgcErrorStatus[ch].cpgcErrDat3S,
                   cpgcErrorStatus[ch].cpgcErrEccS);
  } // ch

  return status;

} // CpgcAdvTrainingErrorStatus

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
  )
{
  return;
} // CpgcMemTestRankSetup


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
  )
{
  return FAILURE;
}


/**

  Loop based on PatternLength

  @param PatternLength - Number of loops

  @retval dqLoopCount

**/
UINT8
PatternLengthToCpgcLoopCnt (
                          UINT32 PatternLength
                          )
{
  UINT8 dqLoopCount;
  UINT32 j;
  INT32 i;

  // Convert repeat into 2^(n - 1) - Round Up
  // PatternLength = 0 1 2 3 4 5 6 7 8 9  10 11 12 13 14 15 16 17 ...
  // RoundUp       = 1 1 2 4 4 8 8 8 8 16 16 16 16 16 16 16 16 32 ...
  // 2^n where n   = 0 0 1 2 2 3 3 3 3 4  4  4  4  4  4  4  4  5
  i = -1;
  j = 0;

  // repeat = 1, i=0 j=1+1=2, 1>=2 ?;
  // repeat = 2, i=0 j=1+1=2, 2>=2 ?; i=1 j=1+2=3, 2>=3 ?;
  // repeat = 3, i=0 j=1+1=2, 3>=2 ?; i=1 j=1+2=3, 3>=3 ?; i=2 j=5, 3>=5?
  // repeat = 4, i=0 j=1+1=2, 4>=2 ?; i=1 j=1+2=3, 4>=3 ?; i=2 j=5, 4>=5?
  // repeat = [7-5], i=0 j=1+1=2, [7-5]>=2 ?; i=1 j=1+2=3, [7-5]>=3 ?; i=2 j=5, [7-5]>=5?; i=3 j=9, [7-5]>=9?
  // repeat = 8, i=0 j=1+1=2, 8>=2 ?; i=1 j=1+2=3, 8>=3 ?; i=2 j=5, 8>=5?; i=3 j=9, 8>=9?
  // repeat = [15-9], i=0 j=1+1=2, [15-9]>=2 ?; i=1 j=1+2=3, [15-9]>=3 ?; i=2 j=5, [15-9]>=5?; i=3 j=9, [15-9]>=9; i=4 j=17, [15-9]>=17;
  // repeat = 16, i=0 j=1+1=2, 16>=2 ?; i=1 j=1+2=3, 16>=3 ?; i=2 j=5, 16>=5?; i=3 j=9, 16>=9; i=4 j=17, 16>=17;
  while (PatternLength >= j) {
    i++;
    j = 1 + (1 << i);               // This is equivalent to (1 + 2^i)
  }

  dqLoopCount = (UINT8)(i + 1);     // ~2^(n+1) ==> passed down to offset 2^(n-1)
  return dqLoopCount;
}

/**

  DDR advanced command CPGC execution

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Memory controller to train
  @param[in] DdrChEnMap  - 4-bit bit mask of ddr channels to test - (1 = channel is selected to run tests)
  @param[in] Pattern     - ddr pattern (0-3) ( 0 = dq, 1 = cmd group 1, 2 = cmd group 2, 3 = cmd group 3)

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcAdvTrainingExecTest (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    DdrChEnMap,
  IN UINT32    Pattern
)
{

  UINT32  Status;

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "CpgcAdvTrainingExecuteTest: ChEnMap = 0x%x, pattern = %d\n", DdrChEnMap, Pattern);

  //
  // Notes:
  // Test are run in parallel on channels indicated by channel mask
  // In 1:1 lockstep only run tests on primary lockstep channels (ddr channels 0, 2)
  //

  Status = SUCCESS;

  {
    //
    // Start test
    //
    //
    // Use CPGC Global Synchronous Control if it is enabled
    // The function will return MRC_STATUS_SUCCESS only if the feature is enabled.
    //
    if (MRC_STATUS_SUCCESS != CpgcGlobalTestSynchronousStart (Host, Socket, DdrChEnMap, NO_SUBCH)) {
      CpgcGlobalTestStart (Host, Socket, DdrChEnMap, NO_SUBCH);
    }

    //
    // Poll test completion
    //
    Status = CpgcPollGlobalTestDone (Host, Socket, DdrChEnMap, NO_SUBCH);
  }


  return Status;

} // CpgcAdvTrainingExecTest10nm


UINT32
EFIAPI
CpgcAdvTrainingStartTest (
  IN PSYSHOST  Host,
  IN UINT8     socket,
  IN UINT32    ddrChEnMap,
  IN UINT16    pattern,
  IN UINT8     SubCh
  )
{

  UINT32  status;

  RcDebugPrintWithDevice (SDBG_CPGC, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "CpgcAdvTrainingExecuteTest: ChEnMap = 0x%x, SuCh = %d, pattern = %d\n", ddrChEnMap, SubCh, pattern);

  //
  // Notes:
  // Test are run in parallel on channels indicated by channel mask
  // In 1:1 lockstep only run tests on primary lockstep channels (ddr channels 0, 2)
  //

  status = SUCCESS;
  {
    //
    // Start test
    //
    CpgcGlobalTestStart (Host, socket, ddrChEnMap, SubCh);

  }
  return status;
} // CpgcAdvTrainingExecTest10nm

/**

  DDR advanced command CPGC execution

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Memory controller to train
  @param[in] DdrChEnMap  - 4-bit bit mask of ddr channels to test - (1 = channel is selected to run tests)
  @param[in] Pattern     - ddr pattern (0-3) ( 0 = dq, 1 = cmd group 1, 2 = cmd group 2, 3 = cmd group 3)

  @retval SUCCESS
  @retval FAILURE

**/
UINT32
CpgcAdvTrainingExecTestGroup (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT32    DdrChEnMap,
  IN    UINT32    Pattern
  )
{
 return CpgcAdvTrainingExecTest (Host, Socket, DdrChEnMap, 0);
} // CpgcAdvTrainingExecTestGroup

/** Returns whether or not a channel is active.

  @param[in]  ChannelMask   Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[in]  Channel       The channel to check against

  @retval FALSE The channel is not active
  @retval TRUE  The channel is active

**/
BOOLEAN
ChannelActive (
  UINT32  ChannelMask,
  UINT8   Channel
  )
{
  return (ChannelMask & (1 << Channel)) != 0;
}

/** Returns whether or not a memory controller is active.

  @param[in]  ChannelMask       Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[in]  MemoryController  The memory controller to check against

  @retval FALSE The memory controller is not active
  @retval TRUE  The memory controller is active

**/
BOOLEAN
MemoryControllerActive (
  UINT32  ChannelMask,
  UINT8   MemoryController
  )
{
  UINT8 Channel;
  UINT8 NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();

  for (Channel = MemoryController * NumChPerMc;
       Channel < (MemoryController + 1) * NumChPerMc;
       Channel++)
  {
    if (ChannelActive (ChannelMask, Channel)) {
      // Return TRUE if any channel in the memory controller is active.
      return TRUE;
    }
  }

  // Return FALSE if no channels in the memory controller are active.
  return FALSE;
}

/** Perform setup required before polling on CPGC test done registers.

  The following are initialized for each channel on the indicated memory controller:
    - DDRT Read Pending Queue timeout (occurred) indicators are cleared.
    - DDRT Read Pending Queue timeout enabled indicators are set.

  @param[in]  Host                                Pointer to SysHost
  @param[in]  Socket                              Socket being trained
  @param[in]  MemoryController                    Memory controller to be polled on
  @param[in]  ChannelMask                         Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[out] DdrtReadPendingQueueTimeoutEnabled  Array indicating whether the Read Pending Queue timeout is enabled
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
  )
{
  UINT8                                Channel;
  CPGC_DDRT_ERR_CTL_STS_MC_2LM_STRUCT  CpgcDdrtErrCtlSts;
  UINT8                                NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();

  // Initialize timeout indicators.
  for (Channel = MemoryController * NumChPerMc;
       Channel < (MemoryController + 1) * NumChPerMc;
       Channel++)
  {

    // Clear the previous timeout indicator for the channel.
    SetDdrtReadPendingQueueTimedOut (Host, Socket, Channel, FALSE);

    // Determine whether timeouts are enabled (or even relevant) for the channel.
    if (ChannelActive (ChannelMask, Channel) &&
        IsDdrtDimm (Host, Socket, Channel))
    {
      CpgcDdrtErrCtlSts.Data = MemReadPciCfgEp (Socket, Channel, CPGC_DDRT_ERR_CTL_STS_MC_2LM_REG);
      DdrtReadPendingQueueTimeoutEnabled[Channel] = CpgcDdrtErrCtlSts.Bits.timeout_base_clk != 0 &&
                                                CpgcDdrtErrCtlSts.Bits.timeout_threshold != 0;
    } else {
      DdrtReadPendingQueueTimeoutEnabled[Channel] = FALSE;
    }

  } // for (Channel)

  return EFI_SUCCESS;

} // SetUpTestDonePolling

/** Check DDRT-specific conditions to determine whether a channel is done being tested.

  This checks for Read Pending Queue timeouts on DDRT ranks.

  @param[in]  Host                                Pointer to SysHost
  @param[in]  Socket                              Socket being trained
  @param[in]  Channel                             Channel being trained
  @param[in]  DdrtReadPendingQueueTimeoutEnabled  Array indicating whether the Read Pending Queue timeout is enabled
                                                  for a given channel
  @param[out] ChannelDone                         Whether or not the channel is done testing.

  @retval EFI_SUCCESS   No errors were encountered.
  @retval !EFI_SUCCESS  An error was encountered.

**/
EFI_STATUS
CheckDdrtChannelDone (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  BOOLEAN   DdrtReadPendingQueueTimeoutEnabled[MAX_CH],
  OUT BOOLEAN   *ChannelDone
  )
{
  CPGC_DDRT_ERR_CTL_STS_MC_2LM_STRUCT  CpgcDdrtErrCtlSts;

  // Note if a DDRT rank with the Read Pending Queue timeout enabled is hung, marking test done for the channel.
  if (DdrtReadPendingQueueTimeoutEnabled[Channel] &&
      IsDdrtDimm (Host, Socket, Channel))
  {

    // Mark the channel as hung if a) it hasn't already timed out, b) it isn't already marked as done, and c) the
    // status register indicates a timeout has occurred.
    if (!DdrtReadPendingQueueTimedOut (Host, Socket, Channel) &&
        !*ChannelDone)
    {
      CpgcDdrtErrCtlSts.Data = MemReadPciCfgEp (Socket, Channel, CPGC_DDRT_ERR_CTL_STS_MC_2LM_REG);
      if (CpgcDdrtErrCtlSts.Bits.timeout_error == 1) {
        SetDdrtReadPendingQueueTimedOut (Host, Socket, Channel, TRUE);
      }
    }

    // If a timeout has occurred (either in this do/while iteration or a previous one), mark testing as being done
    // for the channel.
    if (DdrtReadPendingQueueTimedOut (Host, Socket, Channel)) {
      *ChannelDone = TRUE;
    }

  } // if (DdrtReadPendingQueueTimeoutEnabled && IsDdrtDimm)

  return EFI_SUCCESS;
}

/** Poll on channel-specific indicators of test completion.

  Polling will end when all active channels on the indicated memory controller have either finished testing.

  @param[in]  Host                                Pointer to SysHost
  @param[in]  Socket                              Socket being trained
  @param[in]  MemoryController                    Memory controller to be polled on
  @param[in]  ChannelMask                         Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[in]  DdrtReadPendingQueueTimeoutEnabled  Array indicating whether the Read Pending Queue timeout is enabled
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
  )
{
  BOOLEAN                              AllChannelsDone;
  UINT8                                Channel;
  BOOLEAN                              ChannelDone;
  ALIAS_CPGC_GLOBALERR_MC_MAIN_STRUCT  CpgcGlobalErrReg;
  EFI_STATUS                           EfiStatus = EFI_SUCCESS;
  SYS_SETUP                            *Setup;
  BOOLEAN                              ShouldBreak;
  EFI_STATUS                           TemporaryEfiStatus;
  UINT8                                NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();
  Setup = GetSysSetupPointer ();

  // Poll on all channels until the test is done and errors are cleared.
  do { // while (!AllChannelsDone)

    AllChannelsDone = TRUE;

    CpgcGlobalErrReg.Data = MemReadPciCfgMC (Socket, MemoryController, ALIAS_CPGC_GLOBALERR_MC_MAIN_REG);

    // For each active channel in the MC, check whether the test is done or the DDRT RPQ is hung.
    for (Channel = MemoryController * NumChPerMc;
         Channel < (MemoryController + 1) * NumChPerMc;
         Channel++)
    {

      if (!ChannelActive (ChannelMask, Channel)) {
        continue;
      }

      // Mark the channel as done if its status bit is set or the channel isn't valid.
      switch (Channel % NumChPerMc) {
        case 0:
          ChannelDone = (BOOLEAN) CpgcGlobalErrReg.Bits.channel_test_done_status_0;
          break;
        case 1:
          ChannelDone = (BOOLEAN) CpgcGlobalErrReg.Bits.channel_test_done_status_1;
          break;
        case 2:
          ChannelDone = (BOOLEAN) CpgcGlobalErrReg.Bits.channel_test_done_status_2;
          break;
        default:
          // This shouldn't be reachable, so mark a fail if it *is* reached.
          RcDebugPrintWithDevice (
            SDBG_MAX | SDBG_CPGC,
            Socket,
            Channel,
            NO_DIMM, NO_SUBCH,
            NO_RANK,
            NO_STROBE,
            NO_BIT,
            "Invalid channel: %d\n",
            Channel % NumChPerMc
            );
          ChannelDone = TRUE; // TRUE to avoid a hang.
          if (EfiStatus == EFI_SUCCESS) {
            EfiStatus = EFI_UNSUPPORTED;
          }
          break;
      } // switch (Channel % NumChPerMc)

      // Mark the channel as done if DDRT-specific conditions are met.
      TemporaryEfiStatus = CheckDdrtChannelDone (
        Host,
        Socket,
        Channel,
        DdrtReadPendingQueueTimeoutEnabled,
        &ChannelDone
        );
      if (EfiStatus == EFI_SUCCESS) {
        EfiStatus = TemporaryEfiStatus;
      }

      // Accumulate done statuses for channels in the memory controller.
      AllChannelsDone = AllChannelsDone && ChannelDone;

    } // for (Channel)

    BreakChannelTestTarget (&ShouldBreak);
    if (ShouldBreak) {
      break;
    }

    // If this is a CTE build, delay between reads so we don't poll so often.
    CteDelayDclk (10);

    CountTrackingData (PTC_POLLING_COUNT, 1);

  } while (!AllChannelsDone);

  // Generate uBIOS assembly.
  if (Setup->common.ddrtXactor == 0 &&
      GetUbiosFnvAccessValue ())
  {

    for (Channel = MemoryController * NumChPerMc;
         Channel < (MemoryController + 1) * NumChPerMc;
         Channel++)
    {

      if (!ChannelActive (ChannelMask, Channel)) {
        continue;
      }

      // Poll on per-channel test done status bits.
      switch (Channel % NumChPerMc) {
        case 0:
          // CPGC_GLOBALERR_MC_MAIN_REG[16] is channel_test_done_status_0.
          EmulationPoll (Host, Socket, MemoryController, ALIAS_CPGC_GLOBALERR_MC_MAIN_REG, BIT16, 1);
          break;
        case 1:
          // CPGC_GLOBALERR_MC_MAIN_REG[17] is channel_test_done_status_1.
          EmulationPoll (Host, Socket, MemoryController, ALIAS_CPGC_GLOBALERR_MC_MAIN_REG, BIT17, 1);
          break;
        case 2:
          // CPGC_GLOBALERR_MC_MAIN_REG[18] is channel_test_done_status_2.
          EmulationPoll (Host, Socket, MemoryController, ALIAS_CPGC_GLOBALERR_MC_MAIN_REG, BIT18, 1);
          break;
        default:
          // This shouldn't be reachable, so mark a fail if it *is* reached.
          RcDebugPrintWithDevice (
            SDBG_MAX | SDBG_CPGC,
            Socket,
            Channel,
            NO_DIMM, NO_SUBCH,
            NO_RANK,
            NO_STROBE,
            NO_BIT,
            "Invalid channel: %d\n",
            Channel % NumChPerMc
            );
          if (EfiStatus == EFI_SUCCESS) {
            EfiStatus = EFI_UNSUPPORTED;
          }
          break;
      } // switch (Channel % NumChPerMc)

    } // for (Channel)

  } // if (Setup->common.ddrtXactor == 0 && GetUbiosFnvAccessValue ())

  return EfiStatus;

} // PollUntilChannelTestingDone

/** Poll on the global (memory controller-level) clear errors bit until it deasserts.

  @param[in]  Host              Pointer to SysHost
  @param[in]  Socket            Socket being trained
  @param[in]  MemoryController  Memory controller to be polled on

  @retval EFI_SUCCESS   No errors were encountered
  @retval !EFI_SUCCESS  An error was encountered

**/
EFI_STATUS
PollOnGlobalClearErrors (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     MemoryController
  )
{
  ALIAS_CPGC_GLOBALCTRL_MC_MAIN_STRUCT CpgcGlobalCtrlReg;
  SYS_SETUP                            *Setup;
  BOOLEAN                              ShouldBreak;

  Setup = GetSysSetupPointer ();

  do {

    CpgcGlobalCtrlReg.Data = MemReadPciCfgMC (Socket, MemoryController, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG);

    BreakChannelTestTarget (&ShouldBreak);
    if (ShouldBreak) {
      break;
    }

    // If this is a CTE build, delay between reads so we don't poll so often.
    CteDelayDclk (10);

    CountTrackingData (PTC_POLLING_COUNT, 1);

  } while (CpgcGlobalCtrlReg.Bits.clear_errs == 1);

  // Generate uBIOS assembly.
  if (Setup->common.ddrtXactor == 0 &&
      GetUbiosFnvAccessValue ()) {

    // CPGC_GLOBALCTRL_MC_MAIN_REG[2] is clear_errs.
    EmulationPoll (Host, Socket, MemoryController, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG, BIT2, 0);

  }

  return EFI_SUCCESS;

} // PollOnGlobalClearErrors

/** Conclude testing/polling at the memory controller-level.

  If the indicated memory controller hasn't timed out, polling will occur until the memory controller indicates that
  testing. However, if the memory controller *has* timed out, CPGC will be forced to stop.

  @param[in]  Host              Pointer to SysHost
  @param[in]  Socket            Socket being trained
  @param[in]  MemoryController  Memory controller to be polled on

  @retval EFI_SUCCESS   No errors were encountered
  @retval !EFI_SUCCESS  An error was encountered

**/
EFI_STATUS
ConcludeMemoryControllerTesting (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     MemoryController
  )
{
  UINT8                                Channel;
  ALIAS_CPGC_GLOBALCTRL_MC_MAIN_STRUCT CpgcGlobalCtrlReg;
  BOOLEAN                              MemoryControllerTimedOut = FALSE;
  SYS_SETUP                            *Setup;
  BOOLEAN                              ShouldBreak;
  UINT8                                NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();
  Setup = GetSysSetupPointer ();

  // Determine whether any channel in the memory controller timed out.
  for (Channel = MemoryController * NumChPerMc;
       Channel < (MemoryController + 1) * NumChPerMc;
       Channel++)
  {
    if (DdrtReadPendingQueueTimedOut (Host, Socket, Channel)) {
      MemoryControllerTimedOut = TRUE;
      break;
    }
  }

  // Perform final recovery or polling.
  if (MemoryControllerTimedOut) {

    // Completely stop CPGC if the memory controller timed out.
    CpgcGlobalCtrlReg.Data = MemReadPciCfgMC (Socket, MemoryController, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG);
    CpgcGlobalCtrlReg.Bits.stop_test = 1;
    CpgcGlobalCtrlReg.Bits.start_test = 0;
    MemWritePciCfgMC (Socket, MemoryController, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG, CpgcGlobalCtrlReg.Data);

  } else {

    // If the memory controller didn't time out, wait for the memory controller to indicate that the test is done. This
    // prevents a race condition/timing issue based on using only the channel-level bits. (Note:
    // CPGC_GLOBALCTRL.start_test won't go low if a timeout occurred.)
    do {

      CpgcGlobalCtrlReg.Data = MemReadPciCfgMC (Socket, MemoryController, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG);

      BreakChannelTestTarget (&ShouldBreak);
      if (ShouldBreak) {
        break;
      }

      // If this is a CTE build, delay between reads so we don't poll so often.
      CteDelayDclk (10);

      CountTrackingData (PTC_POLLING_COUNT, 1);

    } while (CpgcGlobalCtrlReg.Bits.start_test == 1);

  } // if (MemoryControllerTimedOut) ... else

  // Generate uBIOS assembly.
  if (Setup->common.ddrtXactor == 0 &&
      GetUbiosFnvAccessValue ())
  {
    // CPGC_GLOBALCTRL_MC_MAIN_REG[0] is start_test.
    EmulationPoll (Host, Socket, MemoryController, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG, BIT0, 0);
  }

  return EFI_SUCCESS;

} // ConcludeMemoryControllerTesting

/** Clean up the DDRT Read Pending Queue state following a test.

  This clears bits related to timeout errors and drains the DDRT Read Pending Queue if a timeout occurred.

  @param[in]  Host                                Pointer to SysHost
  @param[in]  Socket                              Socket being trained
  @param[in]  MemoryController                    Memory controller to be polled on
  @param[in]  ChannelMask                         Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[in]  DdrtReadPendingQueueTimeoutEnabled  Array indicating whether the Read Pending Queue timeout is enabled
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
  )
{
  UINT8                                Channel;
  CPGC_DDRT_ERR_CTL_STS_MC_2LM_STRUCT  CpgcDdrtErrCtlSts;
  UINT8                                Dimm;
  UINT8                                NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();

  // Perform cleanup on active channels with the credit limit set if the rank under test has DDRT.
  for (Channel = MemoryController * NumChPerMc;
       Channel < (MemoryController + 1) * NumChPerMc;
       Channel++)
  {

    if (ChannelActive (ChannelMask, Channel) &&
        DdrtReadPendingQueueTimeoutEnabled[Channel] &&
        IsDdrtDimm (Host, Socket, Channel))
    {

      // If the channel is done being tested, clear the DDRT scheduler's error bits.
      CpgcDdrtErrCtlSts.Data = MemReadPciCfgEp (Socket, Channel, CPGC_DDRT_ERR_CTL_STS_MC_2LM_REG);
      CpgcDdrtErrCtlSts.Bits.crdt_limit_hit = 0;
      CpgcDdrtErrCtlSts.Bits.timeout_error = 0;
      MemWritePciCfgEp (Socket, Channel, CPGC_DDRT_ERR_CTL_STS_MC_2LM_REG, CpgcDdrtErrCtlSts.Data);

      // Drain the Read Pending Queue if a timeout has occurred.
      if (DdrtReadPendingQueueTimedOut (Host, Socket, Channel)) {
        if (Host->var.mem.DdrtSkipRpqDrainInCpgcPolling[Socket] == FALSE) {
          for (Dimm = 0; Dimm < MAX_DIMM; ++Dimm) {
            RPQDrain (Host, Socket, Channel, Dimm, 0); // AEP only has rank 0.
          }
        } else {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "Skipping RPQ drain after timeout\n");
        }
      } // if (DdrtReadPendingQueueTimedOut (Host, Socket, Channel))

    } // if (ChannelActive && DdrtReadPendingQueueTimeoutEnabled && IsDdrtDimm)

  } // Channel loop

  return EFI_SUCCESS;

} // CleanUpDdrtReadPendingQueue

/** CPGC Global Poll function

  @param[in]  Host      Pointer to sysHost
  @param[in]  Socket    Socket number
  @param[in]  ChMask    Bit mask of channels to use
  @param[in]  SubCh     Sub channel number

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcPollGlobalTestDone (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT32    ChMask,
  IN  UINT8     SubCh
  )
{
  EFI_STATUS  EfiStatus = EFI_SUCCESS;
  UINT8       MemoryController;
  BOOLEAN     DdrtReadPendingQueueTimeoutEnabled[MAX_CH];
  UINT8       Status = SUCCESS;
  UINT8       Ch = 0;
  UINT8       ChannelLocal = 0;
  UINT32      ChannelMaskLocal = 0;
  UINT8       MaxImc;
  UINT8       MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  MaxImc = GetMaxImc ();
  if (IsBrsPresent (Host, Socket) && (SubCh != NO_SUBCH )) {
    for (Ch = 0; Ch < MaxChDdr; Ch ++) {
      if (ChMask & (1 << Ch)) {
        GetChannelMapping (Ch, SubCh, &ChannelLocal);
        ChannelMaskLocal = ChannelMaskLocal | (1 << ChannelLocal);
      }
    }
  } else {
    ChannelMaskLocal = ChMask;
  }

  RcDebugPrintWithDevice (
    SDBG_CPGC,
    Socket,
    NO_CH,
    NO_DIMM, NO_SUBCH,
    NO_RANK,
    NO_STROBE,
    NO_BIT,
    "CpgcPollGlobalTestDone Starts\n"
    );

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    if (Host->DdrioUltSupport) {
      CteWaitPatternDone ();
      return Status;
    }
  }

  // Delay if this is a CTE build, otherwise do nothing.
  CteDelayDclk (10);

  // Poll each memory controller until errors are cleared and tests (loopback) are started.
  for (MemoryController = 0; MemoryController < MaxImc; MemoryController++) {

    if (!MemoryControllerActive (ChannelMaskLocal, MemoryController)) {
      continue;
    }

    // Setup

    EfiStatus = SetUpTestDonePolling (
      Host,
      Socket,
      MemoryController,
      ChannelMaskLocal,
      DdrtReadPendingQueueTimeoutEnabled
      );
    if (EfiStatus != EFI_SUCCESS) {
      Status = FAILURE;
    }

    // Execution

    EfiStatus = PollUntilChannelTestingDone (
      Host,
      Socket,
      MemoryController,
      ChannelMaskLocal,
      DdrtReadPendingQueueTimeoutEnabled
      );
    if (EfiStatus != EFI_SUCCESS) {
      Status = FAILURE;
    }

    EfiStatus = PollOnGlobalClearErrors (Host, Socket, MemoryController);
    if (EfiStatus != EFI_SUCCESS) {
      Status = FAILURE;
    }

    EfiStatus = ConcludeMemoryControllerTesting (Host, Socket, MemoryController);
    if (EfiStatus != EFI_SUCCESS) {
      Status = FAILURE;
    }

    // Cleanup

    EfiStatus = CleanUpDdrtReadPendingQueue (
      Host,
      Socket,
      MemoryController,
      ChannelMaskLocal,
      DdrtReadPendingQueueTimeoutEnabled
      );
    if (EfiStatus != EFI_SUCCESS) {
      Status = FAILURE;
    }

  } // for (MemoryController)

  // Delay only if this is a CTE build, otherwise do nothing.
  CteDelayQclk (30);

  if (Status == FAILURE) {
    RcDebugPrintWithDevice (
      SDBG_MAX | SDBG_CPGC,
      NO_SOCKET,
      NO_CH,
      NO_DIMM, NO_SUBCH,
      NO_RANK,
      NO_STROBE,
      NO_BIT,
      "CpgcPollGlobalTestDone: Exit -- Return FAILURE\n"
      );
  } else {
    RcDebugPrintWithDevice (
      SDBG_CPGC,
      NO_SOCKET,
      NO_CH,
      NO_DIMM, NO_SUBCH,
      NO_RANK,
      NO_STROBE,
      NO_BIT,
      "CpgcPollGlobalTestDone: Exit -- Return SUCCESS\n"
      );
  }

  return Status;

} // CpgcPollGlobalTestDone

/**

  Disable global start control

  @param Host                - Pointer to sysHost
  @param socket              - socket number
  @param ch                  - ddr channel number

  @retval n/a

**/
VOID
CpgcDisableGlobalStart (
                       PSYSHOST  Host,
                       UINT8     socket,
                       UINT8     ch
                       )
{
  CPGC_SEQCTL0_MC_MAIN_STRUCT   cpgcSeqCtl0Reg;
  RcDebugPrintWithDevice (SDBG_CPGC, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "CpgcDisableGlobalStart10nm\n");

  cpgcSeqCtl0Reg.Data = MemReadPciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4);
  cpgcSeqCtl0Reg.Bits.global_control = 0;
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, cpgcSeqCtl0Reg.Data);
  return;

} // CpgcDisableGlobalStart10nm


/**
  Return a list of enabled MC IDs from a channel map for a socket.

  McIdList[0] contains the McId of the first enabled MC based on DdrChEnMap.
  McIdList[1] contains the McId of the second enabled MC based on DdrChEnMap.
  etc.
  ListLength will be set to the size of the list.

  Examples for a processors with 4 MCs and 2 channels per MC:
    DdrChEnMap = 0b0000000001 results in McIdList[0] = 0, ListLength = 1
    DdrChEnMap = 0b0000101100 results in McIdList[0] = 1, McIdList[1] = 2, ListLength = 2
    DdrChEnMap = 0b0100000000 results in McIdList[0] = 3, ListLength = 1

  @param[in]  Host       - Pointer to SysHost
  @param[in]  Socket     - Socket number
  @param[in]  DdrChEnMap - Map of socket channels to run the test on
  @param[in]  SubCh      - Subchannel number
  @param[out] McIdList   - McId list to be populated by this routine
  @param[out] ListLength - Set to the number of MCs in the array

  @retval SUCCESS

**/
MRC_STATUS
McIdListFromChMap (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT32    DdrChEnMap,
  IN  UINT8     SubCh,
  OUT UINT8     McIdList[MAX_IMC],
  OUT UINT8     *ListLength
  )
{
  UINT8                                 Ch;
  UINT8                                 ChannelLocal;
  UINT8                                 McId;
  UINT8                                 McIndex;
  UINT8                                 McDone;
  MRC_STATUS                            Status = MRC_STATUS_SUCCESS;

  ZeroMem (McIdList, sizeof (McIdList));
  McIndex = 0;
  McDone  = 0;
  *ListLength = 0;

  //
  // Discover which MCs are included in DdrChEnMap
  //
  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if (!(DdrChEnMap & (1 << Ch))) {
      continue;
    }

    ChannelLocal = Ch;
    if (SubCh != NO_SUBCH) {
      GetChannelMapping (Ch, SubCh, &ChannelLocal);
    }

    McId = GetMCID (Host, Socket, Ch);
    if ((McId >= MAX_IMC) || (McIndex >= MAX_IMC)) {
      Status = MRC_STATUS_FAILURE;
      ASSERT (FALSE);
      break;
    }
    if (McDone & (1 << McId)) {
      continue;
    }
    McDone |= (1 << McId);
    McIdList[McIndex] = McId;
    McIndex++;
  }
  *ListLength = McIndex;
  return Status;
}

/**
  Returns the CPGC Global Synchronous Control delay offset

  @retval - Sync offset in units of 120ns

**/
UINT32
GetCpgcSyncOffset (
  VOID
  )
{
  UINT32    SyncOffsetNanoSeconds;
  UINT32    SyncOffset;

  //
  // Determine sync offset based on UCLK Frequency
  //   =(ceiling((Offset is CSR Read return max + B2B CSR Writes diff max + Extra)/120ns)) + 1
  //
  SyncOffsetNanoSeconds = CSR_READ_RETURN_MAX + BACK_TO_BACK_CSR_WRITE_DIFF * MAX_IMC + SYNC_OFFSET_PADDING;
  SyncOffset = (SyncOffsetNanoSeconds + (120 - 1)) / 120;
  SyncOffset += 1;

  return SyncOffset;
}

/**

  Use CPGC Global Synchronous Control to start the test on multiple MCs at the same time.

  @param[in] Host       - Pointer to SysHost
  @param[in] Socket     - Socket number
  @param[in] DdrChEnMap - Map of socket channels to run the test on
  @param[in]  SubCh     - Subchannel number

  @retval MRC_STATUS_SUCCESS if CPGC Global Synchronous Control is enabled
          MRC_STATUS_FAILURE if CPGC Global Synchronous Control is not enabled

**/
MRC_STATUS
CpgcGlobalTestSynchronousStart (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT32    DdrChEnMap,
  UINT8     SubCh
  )
{
  UINT8                                 McIdList[MAX_IMC];
  UINT8                                 McIndex;
  UINT8                                 ListLength = 0;
  UINT32                                SyncOffset;
  UINT32                                CounterVal;
  UINT8                                 PostedMethodStateSave;
  SYS_SETUP                            *Setup;
  ALIAS_CPGC_GLOBALCTRL_MC_MAIN_STRUCT  CpgcGlobalCtrl;
  CPGC_GLOBAL_SYNC_MC_GLOBAL_STRUCT     CpgcGlobalSync;

  Setup = GetSysSetupPointer ();

  if (!(PcdGetBool (PcdCpgcGlobalSyncCtrlSupported) && Setup->mem.dfxMemSetup.DfxCpgcGlobalSyncCtrlEnabled)) {
    RcDebugPrintWithDevice (SDBG_CPGC, Socket, NO_CH, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT, "CpgcGlobalTestSyncStart not supported\n");
    return MRC_STATUS_FAILURE;
  }

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, NO_CH, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT, "CpgcGlobalTestSyncStart Starts - Channel Map=0x%x\n", DdrChEnMap);

  //
  //  BIOS Flow
  //    1. Determine offset based on UCLK Frequency
  //        a. =(ceiling((Offset is CSR Read return max + B2B CSR Writes diff max + Extra)/120ns)) + 1
  //    2. Per run using trigger
  //        a. Read cpgc_global_sync.counter_val from first MC
  //        b. Add offset to counter value
  //        c. Back to back (posted) CSR writes to cpgc_global_sync {trigger_enable, trigger_arm, trigger_val} on each tiles
  //        d. Wait for test to complete as usual (Poll (read) for Global Start clear, or test done)
  //

  //
  // Discover which MCs are included in DdrChEnMap
  //
  McIdListFromChMap (Host, Socket, DdrChEnMap, SubCh, McIdList, &ListLength);

  //
  // Check if no MCs are in the list - this is not a failure, it just means no work to be done
  //
  if (ListLength == 0) {
    return MRC_STATUS_SUCCESS;
  }

  //
  // ListLength cannot be bigger than MAX_MC
  //
  ListLength = MIN (ListLength, MAX_IMC);

  //
  // Clear errors for each MC in the list
  //
  for (McIndex = 0; McIndex < ListLength; McIndex++) {

    CpgcGlobalCtrl.Data = MemReadPciCfgMC (Socket, McIdList[McIndex], ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG);
    //
    // Clear errors but don't start the test - test is started using the CPGC_GLOBAL_SYNC register below
    //
    CpgcGlobalCtrl.Bits.start_test = 0;
    CpgcGlobalCtrl.Bits.clear_errs = 1;
    MemWritePciCfgMC (Socket, McIdList[McIndex], ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG, CpgcGlobalCtrl.Data);
  }

  //
  // This procedure requires posted CSR writes to be enabled
  // If not already enabled, enable them temporarily
  //
  PostedMethodStateSave = GetPostedMethodState (NULL);
  if (PostedMethodStateSave != POSTED_METHOD_ENABLED) {
    if (EFI_ERROR (PostedMethodEnable ())) {
      RcDebugPrint (SDBG_ERROR, "CpgcGlobalTestSyncStart FAILED - Unable to enable posted CSR writes\n");
      return MRC_STATUS_FAILURE;
    }
  }

  //
  //  Get sync offset in units of 120ns
  //
  SyncOffset = GetCpgcSyncOffset ();

  //
  // Read counter_val from the first MC in the list
  //
  CpgcGlobalSync.Data = MemReadPciCfgMC (Socket, McIdList[0], CPGC_GLOBAL_SYNC_MC_GLOBAL_REG);

  //
  // Add the sync offset to the counter value from the first MC, in units of 120ns
  // Handle the register wrap
  //
  CounterVal = SyncOffset + CpgcGlobalSync.Bits.counter_val;
  if (CounterVal > TRIGGER_VAL_MAX) {
    CounterVal = CounterVal - TRIGGER_VAL_MAX;
  }
  CpgcGlobalSync.Bits.trigger_val    = CounterVal;
  CpgcGlobalSync.Bits.trigger_arm    = 1;
  CpgcGlobalSync.Bits.trigger_enable = 1;

  //
  // Back to back (posted) CSR writes to cpgc_global_sync {trigger_enable, trigger_arm, trigger_val} on each MC
  //
  for (McIndex = 0; McIndex < ListLength; McIndex++) {

    MemWritePciCfgMC (Socket, McIdList[McIndex], CPGC_GLOBAL_SYNC_MC_GLOBAL_REG, CpgcGlobalSync.Data);
  }

  //
  // If posted CSR writes were not previously enabled, disable them before exiting
  //
  if (PostedMethodStateSave != POSTED_METHOD_ENABLED) {
    PostedMethodDisable ();
  }
  //
  // Log the number of times the above loop executed in the performance tracker
  //
  CountTrackingData (PTC_CPGC_COUNT, McIndex);

  return MRC_STATUS_SUCCESS;
}

/**

    CPGC Global Test Start for DDR

    @param Host       - Pointer to sysHost
    @param socket     - socket number
    @param ddrChEnMap - bit mask of channels to use

    @retval SUCCESS

**/
UINT32
CpgcGlobalTestStartDdr (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    ddrChEnMap,
  UINT8     SubCh
  )
{
  UINT8                             ch;
  UINT8                             mcDone;
  UINT8                             mcId;
  UINT8                             ChannelLocal = 0;
  ALIAS_CPGC_GLOBALCTRL_MC_MAIN_STRUCT    cpgcGlobalCtrlReg;
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  mcDone = 0;
  RcDebugPrintWithDevice (SDBG_CPGC, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CpgcGlobalTestStartDdr Starts\n");
  //
  // Delay if this is a CTE build, otherwise do nothing.
  //

  CteDelayDclk (4);  // what is this for??  can we remove it?

  for (ch = 0; ch < MaxChDdr; ch++) {
    if (!(ddrChEnMap & (1 << ch))) {
      continue;
    }
    ChannelLocal = ch;
    if (IsBrsPresent (Host, socket) && (SubCh != NO_SUBCH)) {
      GetChannelMapping (ch, SubCh, &ChannelLocal);
    }
    mcId = GetMCID (Host, socket, ChannelLocal);

    if (mcDone & (1 << mcId)) {
      continue;
    }

    //
    // Fence for posted CSR writes
    // If posted CSR writes enabled, then read from each endpoint behind MEM SS bridge
    // The CPGC_GLOBALCTRL_MC_GLOBAL_REG read acts as feance for MC posted writes
    //
    FencePostedCsrWritesDdrio (Host, socket, mcId);
    cpgcGlobalCtrlReg.Data = MemReadPciCfgMC (socket, mcId, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG);

    //
    // Delay only if this is a CTE build, otherwise do nothing.
    //

    CteDelayQclk (10); // slight delay to allow everything to settle before starting.

    cpgcGlobalCtrlReg.Bits.start_test = 1;
    // clear errors
    cpgcGlobalCtrlReg.Bits.clear_errs = 1;
    MemWritePciCfgMC (socket, mcId, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG, cpgcGlobalCtrlReg.Data);

    CountTrackingData (PTC_CPGC_COUNT, 1);

    //
    // Indicate the test has been started on this MC already
    //
    mcDone |= (1 << mcId);
  } // ch loop

  //
  // Delay if this is a CTE build, otherwise do nothing.
  //

  CteDelayDclk (10); // slight delay to allow everything to settle before starting.

  return SUCCESS;
}

/**

  CPGC Global Start Stop

  @param[in]  Host      Pointer to sysHost
  @param[in]  Socket    Socket number
  @param[in]  ChMask    Bit mask of channels to use

  @retval     SUCCESS

**/
UINT32
CpgcGlobalTestStart (
  IN        PSYSHOST        Host,
  IN        UINT8           Socket,
  IN        UINT32          ChMask,
  IN        UINT8           SubCh
)
{
  CpgcGlobalTestStartDdr (Host, Socket, ChMask, SubCh);

  return SUCCESS;
}

/**

    CPGC Global Test Stop

    @param Host       - Pointer to sysHost
    @param socket     - socket number
    @param ddrChEnMap - bit mask of channels to use

    @retval SUCCESS

**/
UINT32
CpgcGlobalTestStop (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    ddrChEnMap,
  UINT8     SubCh
  )
{
  UINT8                             ch;
  UINT8                             mcDone;
  UINT8                             mcId;
  UINT8                             ChannelLocal = 0;
  ALIAS_CPGC_GLOBALCTRL_MC_MAIN_STRUCT    cpgcGlobalCtrlReg;
  UINT8                             MaxChDdr;
  MaxChDdr = GetMaxChDdr ();
  RcDebugPrintWithDevice (SDBG_CPGC, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CpgcGlobalTestStop Starts\n");
  mcDone = 0;

  for (ch = 0; ch < MaxChDdr; ch += GetChInc (Host)) {
    if (!(ddrChEnMap & (1 << ch))) {
      continue;
    }
    ChannelLocal = ch;
    if (IsBrsPresent (Host, socket) && (SubCh != NO_SUBCH)) {
      GetChannelMapping (ch, SubCh, &ChannelLocal);
    }
    mcId = GetMCID (Host, socket, ChannelLocal);

    if (mcDone & (1 << mcId)) {
      continue;
    }
    cpgcGlobalCtrlReg.Data = MemReadPciCfgMC (socket, mcId, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG);
    cpgcGlobalCtrlReg.Bits.stop_test = 1;
    // clear errors
    MemWritePciCfgMC (socket, mcId, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG, cpgcGlobalCtrlReg.Data);

    //
    // Wait for test to complete
    //
    CpgcPollGlobalTestDone (Host, socket, 1 << ch, SubCh);
    //
    // Indicate the test has been started on this MC already
    //
    mcDone |= (1 << mcId);
  } // ch loop
  return SUCCESS;
}

/**

  DDR loopback control training

  @param Host    - Pointer to sysHost
  @param socket  - Socket to train
  @param ch      - DDR channel to setup
  @param rank    - Logical rank number

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcCtlClkPattern (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  GSM_CSN   signal
  )

{
  UINT32                                        status;
  CPGC_GLOBAL_TRAINING_SETUP                    cpgcGblTrainingSetup;
  CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_STRUCT     seqBaseAddrStartLo0;
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_STRUCT     seqBaseAddrStartHi0;
  CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_STRUCT      seqBaseAddrWrapLo0;
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_STRUCT      seqBaseAddrWrapHi0;
  CPGC_SEQ_BAINCCTL00_MC_MAIN_STRUCT            seqBaincCtl00;
  CPGC_SEQ_BAOCIC0_MC_MAIN_STRUCT               cpgcSeqBaoCic0;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT              cpgcPatCadbCtlReg;
  CPGC_SEQ_BAINCCTL10_MC_MAIN_STRUCT            seqBaincCtl10;
  struct channelNvram                           (*channelNvList)[MAX_CH];
  PatCadbProg0 CtlClkPattern0Buf[CADB_LINES];
  PatCadbProg1 CtlClkPattern1Buf[CADB_LINES];
  UINT16 cs;
  UINT32 i;
  UINT32 address;
  UINT8  bank, cid, cmd, par;
  UINT32                                  CadbMuxPattern[CADB_MUX_MAX];
  UINT8                                   CadbMuxCtl[CADB_MUX_MAX];

  RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "CpgcCtlClkPattern10nm\n");
  channelNvList = GetChannelNvList(Host, socket);


  status = SUCCESS;

  cpgcGblTrainingSetup.seqLoopCnt        = 1;                                // test will end after one loop through the sub-sequence
  cpgcGblTrainingSetup.startDelay        = 0;                                // Delay start
  cpgcGblTrainingSetup.addrUpdRateMode   = CPGC_UPDATE_BY_CL;                // Update based on the # of read and write cacheline
  cpgcGblTrainingSetup.rwMode            = CPGC_BASE_WRITE_SUBSEQ;           // Base Read or write - subseq0
  cpgcGblTrainingSetup.numCacheLines     = 2;                                // Number of Cachelines (2^N)
  cpgcGblTrainingSetup.burstSeparation   = 0;                                // Burst separation based on training step
  cpgcGblTrainingSetup.errEnChunk        = CPGC_ERR_MONITOR_NO_CHUNK;        // Do not monitor errors on chunks
  cpgcGblTrainingSetup.errEnCl           = CPGC_ERR_MONITOR_NO_CL;           // Do not monitor errors on cacheline
  cpgcGblTrainingSetup.stopOnErrCtl      = CPGC_ERR_NEVER_STOP_ON_ERR;       // Do not Stop on all lane error
  cpgcGblTrainingSetup.subseqEndPtr      = 1;                                // subseq end pointer
  cpgcGblTrainingSetup.useSubSeq1        = CPGC_BASE_READ_SUBSEQ;            // use sub seq1 (base read)
  cpgcGblTrainingSetup.cpgcLinkTrain     = 0;                                // do not use cpgc link train
  cpgcGblTrainingSetup.back2backWrEn     = 0;                                // enable b2b writes
  cpgcGblTrainingSetup.mprControl        = 0;                                // MPR disable
  cpgcGblTrainingSetup.refMask           = 0xFF;                             // disable cpgc self refresh
  cpgcGblTrainingSetup.zqMask            = 0xFF;                             // disable cpgc zq
  cpgcGblTrainingSetup.x8Mode            = 0;
  CpgcGlobalTrainingSetup (Host, socket, ch, &cpgcGblTrainingSetup);

  //
  // WDB programming
  //
  seqBaseAddrStartLo0.Data            = 0;
  seqBaseAddrStartHi0.Data            = 0;
  seqBaseAddrStartHi0.Bits.rank_addr  = 0;

  seqBaseAddrWrapLo0.Data             = 0;
  seqBaseAddrWrapLo0.Bits.column_addr = 0x1FF;
  seqBaseAddrWrapLo0.Bits.bank_addr   = 0x7;

  seqBaseAddrWrapHi0.Data           = 0;
  seqBaseAddrWrapHi0.Bits.row_addr  = 0x1000;
  seqBaseAddrWrapHi0.Bits.rank_addr = 0;

  //
  // Sequence Bank Address Increment control
  //
  seqBaincCtl00.Data = 0;
  seqBaincCtl00.Bits.column_ba_inc      = 0;
  seqBaincCtl00.Bits.column_ba_updrate  = 0;
  seqBaincCtl00.Bits.column_ba_updscale = 0;
  seqBaincCtl00.Bits.row_ba_inc         = 12;
  seqBaincCtl00.Bits.row_ba_updrate     = 0;
  seqBaincCtl00.Bits.row_ba_updscale    = 1;

  seqBaincCtl10.Data = 0;
  seqBaincCtl10.Bits.bank_ba_inc        = 4;
  seqBaincCtl10.Bits.bank_ba_updrate    = 0;
  seqBaincCtl10.Bits.bank_ba_updscale   = 1;
  seqBaincCtl10.Bits.rank_ba_inc        = 0;
  seqBaincCtl10.Bits.rank_ba_updrate    = 0;
  seqBaincCtl10.Bits.rank_ba_updscale   = 0;
  seqBaincCtl10.Bits.column_ba_inc_msb  = 0;

  //
  // Set wrap order
  //
  cpgcSeqBaoCic0.Data = 0;

  CpgcSetupWdb (Host, socket, ch,
                   0,                             // wdbIncrRate
                   0,                             // wdbIncrScale
                   0,                             // wdbStartPntr
                   3,                             // wdbEndPntr
                   1,                             // setupPbwr
                   CPGC_WDB_MUX_CTL_LFSR24,       // mux = pattern buffer
                   seqBaincCtl00,
                   seqBaincCtl10,
                   seqBaseAddrStartLo0,
                   seqBaseAddrStartHi0,
                   seqBaseAddrWrapLo0,
                   seqBaseAddrWrapHi0,
                   cpgcSeqBaoCic0
                  );


  //
  // Program CADB entries
  //
  // Fixup CA pattern based on signal
  address = 0;
  bank = 0;
  cid = 0;
  cmd = 0x3;  // DDR3 ACT encoding causes address[16:14] to be used
  cs = 0x3FF;
  par = 0;
  switch(signal) {
    case CS2_N:
    case CS3_N:
    case CS6_N:
    case CS7_N:
      cs = (1 << (signal - CS0_N));
      break;
    case C0:
    case C1:
    case C2:
      cid = (1 << (signal - C0));
      break;
    case A0:
      address = BIT0;
      break;
    case A1:
      address = BIT1;
      break;
    case A2:
      address = BIT2;
      break;
    case A3:
      address = BIT3;
      break;
    case A4:
      address = BIT4;
      break;
    case A5:
      address = BIT5;
      break;
    case A6:
      address = BIT6;
      break;
    case A7:
      address = BIT7;
      break;
    case A8:
      address = BIT8;
      break;
    case A9:
      address = BIT9;
      break;
    case A10:
      address = BIT10;
      break;
    case A11:
      address = BIT11;
      break;
    case A12:
      address = BIT12;
      break;
    case A13:
      address = BIT13;
      break;
    case A14:
      address = BIT14;
      break;
    case A15:
      address = BIT15;
      break;
    case A16:
      address = BIT16;
      break;
    case A17:
      address = BIT17;
      break;
    case BA0:
      bank = BIT0;
      break;
    case BA1:
      bank = BIT1;
      break;
    case BG0:
      bank = BIT2;
      break;
    case BG1:
      bank = BIT3;
      break;
    case PAR:
      par = 1;
      break;
    case ACT_N:
      cmd = BIT3;
      break;
    default:
      break;
  }
  for (i = 0; i < CADB_LINES; i++) {
    CtlClkPattern0Buf[i] = CtlClkPattern0[i];
    CtlClkPattern1Buf[i] = CtlClkPattern1[i];
    if ((i & 1) == 0) {
      CtlClkPattern0Buf[i].address = address;
      CtlClkPattern0Buf[i].bank = bank;
      CtlClkPattern0Buf[i].cid = cid;
      CtlClkPattern1Buf[i].cs = cs;
      CtlClkPattern1Buf[i].par = par;
      CtlClkPattern1Buf[i].control = cmd;
    }
  }

  ProgramCADB (Host, socket, ch, NO_SUBCH, 0, 2, CtlClkPattern0Buf, CtlClkPattern1Buf);

  //
  // Enable CADB
  //
  cpgcPatCadbCtlReg.Data = MemReadPciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG);
  cpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
  cpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 1;
  cpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 0;
  MemWritePciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG, cpgcPatCadbCtlReg.Data);

  //
  // Program CADB pattern buffer
  //
  CadbMuxPattern[0] = CADB_MUX_PATTERN0;
  CadbMuxPattern[1] = 0;
  CadbMuxPattern[2] = 0;
  CadbMuxPattern[3] = 0;

  CadbMuxCtl[0] = CADB_MUX_CTRL_BUFFER;
  CadbMuxCtl[1] = CADB_MUX_CTRL_BUFFER;
  CadbMuxCtl[2] = CADB_MUX_CTRL_BUFFER;
  CadbMuxCtl[3] = CADB_MUX_CTRL_BUFFER;

  ProgramCadbMux (socket, ch, NO_SUBCH, CadbMuxPattern, CadbMuxCtl);

  return status;
}



/**

  DDR parity based command training

  @param[in] Host     Pointer to sysHost
  @param[in] Socket   Socket to train
  @param[in] Ch       DDR channel to setup
  @param[in] Rank     Logical rank number
  @param[in] SubRank  Sub-rank number
  @param[in] Signal   Signal to test
  @param[in] Mode     CADB Mode (NormalCadbSetup, BacksideCadbSetup, or RestoreCadbPattern)

  @retval SUCCESS   All operations successful
  @retval FAILURE   Failure(s) occurred

**/
UINT32
CpgcAdvCmdParity (
  IN  PSYSHOST                  Host,
  IN  UINT8                     Socket,
  IN  UINT8                     Ch,
  IN  UINT8                     Rank,
  IN  UINT8                     SubRank,
  IN  GSM_CSN                   Signal,
  IN  CPGC_ADV_CMD_PARITY_MODE  Mode
  )
{
  UINT32                                        LineIndex;
  UINT32                                        Status;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT              CpgcPatCadbCtlReg;
  CPGC_GLOBAL_TRAINING_SETUP                    CpgcGblTrainingSetup;
  CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_STRUCT     SeqBaseAddrStartLo0;
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_STRUCT     SeqBaseAddrStartHi0;
  CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_STRUCT      SeqBaseAddrWrapLo0;
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_STRUCT      SeqBaseAddrWrapHi0;
  CPGC_SEQ_BAINCCTL00_MC_MAIN_STRUCT            SeqBaincCtl00;
  CPGC_SEQ_BAINCCTL10_MC_MAIN_STRUCT            SeqBaincCtl10;
  CPGC_SEQ_BAOCIC0_MC_MAIN_STRUCT               CpgcSeqBaoCic0;
  struct channelNvram                           (*ChannelNvList)[MAX_CH];
  UINT32                                        CadbMuxPattern[CADB_MUX_MAX];
  UINT8                                         CadbMuxCtl[CADB_MUX_MAX];
  PatCadbProg0                                  ParityPattern0Local[CADB_LINES];
  PatCadbProg1                                  ParityPattern1Local[CADB_LINES];
  PatCadbProg0                                  ActivatePattern0Local[CADB_LINES];
  PatCadbProg1                                  ActivatePattern1Local[CADB_LINES];
  PatCadbProg0                                  PrechargePattern0Local[CADB_LINES];
  PatCadbProg1                                  PrechargePattern1Local[CADB_LINES];
  PatCadbProg0                                  FnvGoodParity0Local[CADB_LINES];
  PatCadbProg1                                  FnvGoodParity1Local[CADB_LINES];
  PatCadbProg0                                  AddressPattern0Local[CADB_LINES];
  PatCadbProg1                                  AddressPattern1Local[CADB_LINES];
  PatCadbProg1                                  DeselectPattern1Local[CADB_LINES];
  PatCadbProg1                                  GntPattern1Local[CADB_LINES];

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                 "CpgcAdvCmdParity\n");
  ChannelNvList = GetChannelNvList(Host, Socket);

  //
  // Program CADB pattern buffer
  //
  CadbMuxPattern[0] = CADB_MUX_PATTERN0;
  CadbMuxPattern[1] = CADB_MUX_PATTERN1;
  CadbMuxPattern[2] = CADB_MUX_PATTERN2;
  CadbMuxPattern[3] = CADB_MUX_PATTERN3;

  CadbMuxCtl[0] = CADB_MUX_CTRL_BUFFER;
  CadbMuxCtl[1] = CADB_MUX_CTRL_BUFFER;
  CadbMuxCtl[2] = CADB_MUX_CTRL_BUFFER;
  CadbMuxCtl[3] = CADB_MUX_CTRL_BUFFER;

  ProgramCadbMux (Socket, Ch, NO_SUBCH, CadbMuxPattern, CadbMuxCtl);

  for (LineIndex = 0; LineIndex < CADB_LINES; LineIndex++) {

    if (Mode != BacksideCadbSetup) {
      CopyMem (&ParityPattern0Local[LineIndex], &ParityPattern0[LineIndex], sizeof (PatCadbProg0));
      if ((*ChannelNvList)[Ch].timingMode == TIMING_2N){
        CopyMem (&ParityPattern1Local[LineIndex], &ParityPattern12N[LineIndex], sizeof (PatCadbProg1));
      } else {
        CopyMem (&ParityPattern1Local[LineIndex], &ParityPattern1[LineIndex], sizeof (PatCadbProg1));
      }
      CopyMem (&ActivatePattern0Local[LineIndex], &ActivatePattern0[LineIndex], sizeof (PatCadbProg0));

      if ((*ChannelNvList)[Ch].timingMode == TIMING_2N){
        CopyMem (&ActivatePattern1Local[LineIndex], &ActivatePattern12N[LineIndex], sizeof (PatCadbProg1));
      } else {
        CopyMem (&ActivatePattern1Local[LineIndex], &ActivatePattern1[LineIndex], sizeof (PatCadbProg1));
      }
      CopyMem (&AddressPattern0Local[LineIndex], &AddressPattern0[LineIndex], sizeof (PatCadbProg0));
      CopyMem (&AddressPattern1Local[LineIndex], &AddressPattern1[LineIndex], sizeof (PatCadbProg1));

    } else {

      CopyMem (&ParityPattern0Local[LineIndex], &BacksideParityPattern0[LineIndex], sizeof (PatCadbProg0));
      CopyMem (&ParityPattern1Local[LineIndex], &BacksideParityPattern1[LineIndex], sizeof (PatCadbProg1));
      CopyMem (&ActivatePattern0Local[LineIndex], &BacksideActivatePattern0[LineIndex], sizeof (PatCadbProg0));
      CopyMem (&ActivatePattern1Local[LineIndex], &BacksideActivatePattern1[LineIndex], sizeof (PatCadbProg1));

      if ((Signal == A14) || (Signal == A15) || (Signal == A16)) {
        CopyMem (&AddressPattern0Local[LineIndex], &BacksideCommandPattern0[LineIndex], sizeof (PatCadbProg0));
        CopyMem (&AddressPattern1Local[LineIndex], &BacksideCommandPattern1[LineIndex], sizeof (PatCadbProg1));
      } else {
        CopyMem (&AddressPattern0Local[LineIndex], &BacksideAddressPattern0[LineIndex], sizeof (PatCadbProg0));
        CopyMem (&AddressPattern1Local[LineIndex], &BacksideAddressPattern1[LineIndex], sizeof (PatCadbProg1));
      }
    }

    CopyMem (&PrechargePattern0Local[LineIndex], &PrechargePattern0[LineIndex], sizeof (PatCadbProg0));
    CopyMem (&PrechargePattern1Local[LineIndex], &PrechargePattern1[LineIndex], sizeof (PatCadbProg1));
    CopyMem (&FnvGoodParity0Local[LineIndex], &FnvGoodParity0[LineIndex], sizeof (PatCadbProg0));
    CopyMem (&FnvGoodParity1Local[LineIndex], &FnvGoodParity1[LineIndex], sizeof (PatCadbProg1));
    CopyMem (&DeselectPattern1Local[LineIndex], &DeselectPattern1[LineIndex], sizeof (PatCadbProg1));
    CopyMem (&GntPattern1Local[LineIndex], &GntPattern1[LineIndex], sizeof (PatCadbProg1));
  }

  Status = SUCCESS;

  if (Mode != RestoreCadbPattern) {
    if (Signal == GNT) {
      CpgcGblTrainingSetup.seqLoopCnt      = 0;                                // test will end after one loop through the sub-sequence //?? Incorrect comment
    } else {
      CpgcGblTrainingSetup.seqLoopCnt      = 1;                                // test will end after one loop through the sub-sequence
    }
    CpgcGblTrainingSetup.startDelay        = 0;                                // Delay start
    CpgcGblTrainingSetup.addrUpdRateMode   = CPGC_UPDATE_BY_CL;                // Update based on the # of read and write cacheline
    CpgcGblTrainingSetup.rwMode            = CPGC_BASE_WRITE_SUBSEQ;           // Base Read or write - subseq0
    CpgcGblTrainingSetup.numCacheLines     = 2;                                // Number of Cachelines (2^N)
    CpgcGblTrainingSetup.burstSeparation   = 0;                                // Burst separation based on training step
    CpgcGblTrainingSetup.errEnChunk        = CPGC_ERR_MONITOR_ALL_CHUNK;       // monitor errors on any chunk
    CpgcGblTrainingSetup.errEnCl           = CPGC_ERR_MONITOR_ALL_CL;          // monitor errors on any cacheline
    CpgcGblTrainingSetup.stopOnErrCtl      = CPGC_ERR_STOP_ON_ALL_ERR;         // Stop on all lane error
    CpgcGblTrainingSetup.subseqEndPtr      = 1;                                // subseq end pointer
    CpgcGblTrainingSetup.useSubSeq1        = CPGC_BASE_READ_SUBSEQ;            // use sub seq1 (base read)
    CpgcGblTrainingSetup.cpgcLinkTrain     = 0;                                // do not use cpgc link train
    CpgcGblTrainingSetup.back2backWrEn     = 0;                                // enable b2b writes
    CpgcGblTrainingSetup.mprControl        = 0;                                // MPR disable
    CpgcGblTrainingSetup.refMask           = 0xFF;                             // disable cpgc self refresh
    CpgcGblTrainingSetup.zqMask            = 0xFF;                             // disable cpgc zq
    CpgcGblTrainingSetup.x8Mode            = 0;

    CpgcGlobalTrainingSetup (Host, Socket, Ch, &CpgcGblTrainingSetup);

    //
    // WDB programming
    //
    SeqBaseAddrStartLo0.Data            = 0;
    SeqBaseAddrStartHi0.Data            = 0;
    SeqBaseAddrStartHi0.Bits.rank_addr  = Rank;

    SeqBaseAddrWrapLo0.Data             = 0;
    SeqBaseAddrWrapLo0.Bits.column_addr = 0x1FF;
    SeqBaseAddrWrapLo0.Bits.bank_addr   = 0x7;

    SeqBaseAddrWrapHi0.Data           = 0;
    SeqBaseAddrWrapHi0.Bits.row_addr  = 0x1000;
    SeqBaseAddrWrapHi0.Bits.rank_addr = Rank;

    //
    // Sequence Bank Address Increment control
    //
    SeqBaincCtl00.Data = 0;
    SeqBaincCtl00.Bits.column_ba_inc      = 0;
    SeqBaincCtl00.Bits.column_ba_updrate  = 0;
    SeqBaincCtl00.Bits.column_ba_updscale = 0;
    SeqBaincCtl00.Bits.row_ba_inc         = 12;
    SeqBaincCtl00.Bits.row_ba_updrate     = 0;
    SeqBaincCtl00.Bits.row_ba_updscale    = 1;

    SeqBaincCtl10.Data = 0;
    SeqBaincCtl10.Bits.bank_ba_inc        = 4;
    SeqBaincCtl10.Bits.bank_ba_updrate    = 0;
    SeqBaincCtl10.Bits.bank_ba_updscale   = 1;
    SeqBaincCtl10.Bits.rank_ba_inc        = 0;
    SeqBaincCtl10.Bits.rank_ba_updrate    = 0;
    SeqBaincCtl10.Bits.rank_ba_updscale   = 0;
    SeqBaincCtl10.Bits.column_ba_inc_msb  = 0;

    //
    // Set wrap order
    //
    CpgcSeqBaoCic0.Data = 0;

    CpgcSetupWdb (
      Host,
      Socket,
      Ch,
      0,                             // wdbIncrRate
      0,                             // wdbIncrScale
      0,                             // wdbStartPntr
      3,                             // wdbEndPntr
      1,                             // setupPbwr
      CPGC_WDB_MUX_CTL_LFSR24,       // mux = pattern buffer
      SeqBaincCtl00,
      SeqBaincCtl10,
      SeqBaseAddrStartLo0,
      SeqBaseAddrStartHi0,
      SeqBaseAddrWrapLo0,
      SeqBaseAddrWrapHi0,
      CpgcSeqBaoCic0
      );

  } // if !RestoreCadbPattern
  //
  // CADB entries for ddr cmd group 1
  //

  switch (Signal) {
  case PAR:

    if (Mode == BacksideCadbSetup) {
      FixupBacksideCS (Rank, (*ChannelNvList)[Ch].encodedCSMode, ParityPattern1Local);
    } else {
      FixupCS (Rank, SubRank, ParityPattern0Local, ParityPattern1Local, (*ChannelNvList)[Ch].encodedCSMode);
    }
    ProgramCADB (Host, Socket, Ch, NO_SUBCH, 0, CADB_LINES, ParityPattern0Local, ParityPattern1Local);
    break;

  case ACT_N:

    if (Mode == BacksideCadbSetup) {
      FixupBacksideCS (Rank, (*ChannelNvList)[Ch].encodedCSMode, ActivatePattern1Local);
    } else {
      FixupCS (Rank, SubRank, ActivatePattern0Local, ActivatePattern1Local, (*ChannelNvList)[Ch].encodedCSMode);
    }
    ProgramCADB (Host, Socket, Ch, NO_SUBCH, 0, CADB_LINES, ActivatePattern0Local, ActivatePattern1Local);
    break;

  case RAS_N:
  case CAS_N:
  case WE_N:
  case BA0:
  case BA1:
  case BA2:
  case BG0:
  case BG1:
  case A0:
  case A1:
  case A2:
  case A3:
  case A4:
  case A5:
  case A6:
  case A7:
  case A8:
  case A9:
  case A10:
  case A11:
  case A12:
  case A13:
  case A14:
  case A15:
  case A16:
  case A17:
  case C0:
  case C1:
  case C2:
  case CS2_N:
  case CS6_N:
  case CS3_N:
  case CS7_N:

    if (Mode == BacksideCadbSetup) {
      FixupBacksideCS (Rank, (*ChannelNvList)[Ch].encodedCSMode, AddressPattern1Local);
      FixupBacksideAddrBits (Rank, Signal, AddressPattern0Local, AddressPattern1Local);
    } else {
      FixupCS (Rank, SubRank, AddressPattern0Local, AddressPattern1Local, (*ChannelNvList)[Ch].encodedCSMode);
      FixupAddrBits (Signal, AddressPattern0Local, AddressPattern1Local, (*ChannelNvList)[Ch].timingMode);
    }
    ProgramCADB (Host, Socket, Ch, NO_SUBCH, 0, CADB_LINES, AddressPattern0Local, AddressPattern1Local);
    break;

  case PRECHARGE:  // Precharge

    if (Mode == BacksideCadbSetup) {
      FixupBacksideCS (Rank, (*ChannelNvList)[Ch].encodedCSMode, PrechargePattern1Local);
    } else {
      FixupCS (Rank, SubRank, PrechargePattern0Local, PrechargePattern1Local, (*ChannelNvList)[Ch].encodedCSMode);
    }
    ProgramCADB (Host, Socket, Ch, NO_SUBCH, 0, CADB_LINES, PrechargePattern0Local, PrechargePattern1Local);
    break;

  case FNV_GOOD_PARITY:
    FixupCS (Rank, SubRank, FnvGoodParity0Local, FnvGoodParity1Local, (*ChannelNvList)[Ch].encodedCSMode);
    ProgramCADB (Host, Socket, Ch, NO_SUBCH, 0, CADB_LINES, FnvGoodParity0Local, FnvGoodParity1Local);
    break;
  case DESELECT:  // Deselect
    ProgramCADB (Host, Socket, Ch, NO_SUBCH, 0, CADB_LINES, PrechargePattern0Local, DeselectPattern1Local);
    break;
  case GNT:
    FixupCS (Rank + 1, 0, ActivatePattern0Local, GntPattern1Local, (*ChannelNvList)[Ch].encodedCSMode); // GNT is on the 2nd CS per dimm
    ProgramCADB (Host, Socket, Ch, NO_SUBCH, 0, CADB_LINES, ActivatePattern0Local, GntPattern1Local);
    break;
  default:        // Deselect
    ProgramCADB (Host, Socket, Ch, NO_SUBCH, 0, CADB_LINES, PrechargePattern0Local, DeselectPattern1Local);
    break;
  }

  if (Mode != RestoreCadbPattern) {
    //
    // Enable CADB
    //
    CpgcPatCadbCtlReg.Data = MemReadPciCfgEp (Socket, Ch, CPGC_PATCADBCTL_MCDDC_CTL_REG);
    CpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
    CpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 1;
    if (Signal == GNT) {
      CpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 0;
    } else {
      CpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 1;
    }
    if ((Host->nvram.mem.dimmTypePresent == UDIMM) || (Host->nvram.mem.dimmTypePresent == SODIMM)) {
      CpgcPatCadbCtlReg.Bits.enable_cadb_parityntiming = 1;
    }
    MemWritePciCfgEp (Socket, Ch, CPGC_PATCADBCTL_MCDDC_CTL_REG, CpgcPatCadbCtlReg.Data);
  }

  return Status;

} // CpgcAdvCmdParity

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
        )
{
  UINT8 cadbLine;
  UINT8 dimm;

  for (cadbLine = 0; cadbLine < CADB_LINES; cadbLine++) {
    if (prog1[cadbLine].cs != 0x3FF) {
      prog1[cadbLine].cs =  (~(1 << cs)) & 0x3FF;
    }
    if (encodedCSMode == 2) {
        dimm = (cs / 4) * 4;
        prog1[cadbLine].cs = prog1[cadbLine].cs & ~(0xc<<(dimm));
      prog1[cadbLine].cs |= (subrank << (dimm +2)); // program subranks CID0/CID1
      prog0[cadbLine].cid = subrank;  // program CID2 if its needed
    }
  }
} // FixupCS

/**

Fix up the CS field for programming the CADB

@param cs             - Chip select
@param subrank        - Sub Rank
@param prog0          - Data to program
@param prog1          - Data to program
@param encodedCSMode  - Encoded CS mode

**/
VOID
FixupCSBrs (
  UINT8         cs,
  UINT8         subrank,
  PatCadbProg0  prog0[CADB_LINES],
  PatCadbProg1  prog1[CADB_LINES],
  UINT8         encodedCSMode
  )
{
  UINT8 cadbLine;
  UINT8 dimm;

  for (cadbLine = 0; cadbLine < CADB_LINES; cadbLine++) {
    if (prog1[cadbLine].cs != 0x3FF) {
      prog1[cadbLine].cs = (~((1 << cs) | (1 << (cs + 2)))) & 0x3FF;
    }
    if (encodedCSMode == 2) {
      dimm = (cs / 4) * 4;
      prog1[cadbLine].cs = prog1[cadbLine].cs & ~(0xc<< (dimm));
      prog1[cadbLine].cs |= (subrank << (dimm + 2)); // program subranks CID0/CID1
      prog0[cadbLine].cid = subrank;  // program CID2 if its needed
    }
  }
} // FixupCSBrs

/**

  Fix up the Chip Select field for backside CS for programming the CADB

  @param[in]        Cs            - Rank chip select
  @param[in]        EncodedCsMode - Encoded CS mode value for the channel
  @param[in, out]   Prog1         - Data to program

**/
STATIC VOID
FixupBacksideCS (
  IN      UINT8         Cs,
  IN      UINT8         EncodedCsMode,
  IN OUT  PatCadbProg1  Prog1[CADB_LINES]
  )
{
  UINT8   CadbLine;
  UINT16  CidMask = 0xFFFF;

  if (EncodedCsMode == 2) {
    //
    // For 3DS DIMMs, CID signals are set to 0 to avoid being included in the parity calculation
    // If the signal is being tested, FixupBacksideAddr will set the signal high when needed
    //
    CidMask &= ~((1 << (CS2_N - CS0_N)) | (1 << (CS3_N - CS0_N)) | (1 << (CS6_N - CS0_N)) | (1 << (CS7_N - CS0_N)));
  }

  for (CadbLine = 0; CadbLine < CADB_LINES; CadbLine++) {
    if (Prog1[CadbLine].cs == 0x3FE) {
      // target CS
      Prog1[CadbLine].cs = (~(1 << Cs)) & 0x3FF;
    } else if (Prog1[CadbLine].cs == 0x3FD) {
      // non-target CS
      Prog1[CadbLine].cs = (~(1 << (Cs ^ 0x1))) & 0x3FF;
    }
    Prog1[CadbLine].cs &= CidMask;
  } // cadbLine
} // FixupBacksideCS


/**
  Fix up the address bit field for programming the CADB

  @param[in] Signal        - Signal to test
  @param[in, out] prog0    - CADB data to program
  @param[in, out] prog1    - CADB data to program
  @param[in] CmdTimingMode - Command Timing Mode (i.e. 1N or 2N)

**/
STATIC VOID
FixupAddrBits (
  IN     GSM_CSN       Signal,
  IN OUT PatCadbProg0  Prog0[CADB_LINES],
  IN OUT PatCadbProg1  Prog1[CADB_LINES],
  IN     UINT8         CmdTimingMode
  )
{
  UINT8                CadbLine;
  UINT8                Pattern[CADB_LINES]   = {0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1};
  UINT8                Pattern2N[CADB_LINES] = {0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1};

  if (CmdTimingMode == TIMING_2N) {
    for (CadbLine = 0; CadbLine < CADB_LINES; CadbLine++){
      Pattern[CadbLine] = Pattern2N[CadbLine];
    }
  }
  for (CadbLine = 0; CadbLine < CADB_LINES; CadbLine++) {
    if ((Signal >= A0) && (Signal <= A17)) {
      Prog0[CadbLine].address = Pattern[CadbLine] << (Signal - A0);
    } else {
      Prog0[CadbLine].address  = 0;
    }

    if ((Signal >= BA0) && (Signal <= BA2)) {
      Prog0[CadbLine].bank  = Pattern[CadbLine] << (Signal - BA0);
    } else {
      Prog0[CadbLine].bank  = 0;
    }

    if ((Signal >= BG0) && (Signal <= BG1)) {
      Prog0[CadbLine].bank  = (Pattern[CadbLine] << (Signal - BG0)) << 2;
    } else {
      Prog0[CadbLine].bank  &= 3;
    }

    if ((Signal >= C0) && (Signal <= C2)) {
      Prog0[CadbLine].cid  = (Pattern[CadbLine] << (Signal - C0));
    } else {
      Prog0[CadbLine].cid  = 0;
    }

    if (Signal == RAS_N) {
      Prog1[CadbLine].control  = Pattern[CadbLine] << 2;
    } else {
      Prog1[CadbLine].control  &= ~BIT2;
    }

    if (Signal == CAS_N) {
      Prog1[CadbLine].control  = Pattern[CadbLine] << 1;
    } else {
      Prog1[CadbLine].control  &= ~BIT1;
    }

    if (Signal == WE_N) {
      Prog1[CadbLine].control  = Pattern[CadbLine];
    } else {
      Prog1[CadbLine].control  &= ~BIT0;
    }

    // CS#/CIDs only trained during earlycmdclk for 3DS DIMMs
    if ((Signal == CS2_N) || (Signal == CS3_N) || (Signal == CS6_N) || (Signal == CS7_N)) {
      Prog1[CadbLine].cs &= ((~(1 << (Signal - CS0_N))) & 0x3FF); // Clears Target Chip Select
      Prog1[CadbLine].cs |= ((Pattern[CadbLine]) << (Signal - CS0_N)); // Programs Pattern on Target Chip Select
      if (Signal == CS2_N || Signal == CS6_N) {
        Prog1[CadbLine].cs &= ((~(1 << (Signal - CS0_N + 1))) & 0x3FF); // Clears CS3 or CS7
      } else if (Signal == CS3_N || Signal == CS7_N) {
        Prog1[CadbLine].cs &= ((~(1 << (Signal - CS0_N - 1))) & 0x3FF); // Clears CS2 or CS6
      }
    }

  } // CadbLine loop
} // FixupAddrBits

/**

  Fix up the address bit field for a backside address for programming the CADB

  @param[in]      Rank    Physical rank to test (0-based within the channel)
  @param[in]      sig     Signal to test
  @param[in, out] prog0   Data to program
  @param[in, out] prog1   Data to program

**/
STATIC VOID
FixupBacksideAddrBits (
  IN      UINT8         Rank,
  IN      GSM_CSN       Sig,
  IN OUT  PatCadbProg0  Prog0[CADB_LINES],
  IN OUT  PatCadbProg1  Prog1[CADB_LINES]
  )
{
  UINT8     CadbLine;
  GSM_CSN   CidToCsSig;

  for (CadbLine = 0; CadbLine < CADB_LINES; CadbLine++) {
    if (Prog0[CadbLine].address == 0x1) {
      if ((Sig >= A0) && (Sig <= A13)) {
        Prog0[CadbLine].address = 1 << (Sig - A0);
      }
      if ((Sig == A17)) {
        Prog0[CadbLine].address = 1 << (Sig - A0);
      }
      if ((Sig >= BA0) && (Sig <= BA2)) {
        Prog0[CadbLine].bank = 1 << (Sig - BA0);
        Prog0[CadbLine].address = 0;
      }
      if ((Sig >= BG0) && (Sig <= BG1)) {
        Prog0[CadbLine].bank = 1 << (Sig - BG0) << 2;
        Prog0[CadbLine].address = 0;
      }
      //
      // This CADB does not provide direct control of C0 and C1 using the CID field.
      // Instead, we use CS2/6_N and CS3/7_N in the CS field
      // For DIMM0 - C0 -> CS2_N and C1 -> CS3_N
      // For DIMM1 - C0 -> CS6_N and C1 -> CS7_N
      //
      if (Sig == C0) {
        if (Rank < MAX_RANK_DIMM) {
          CidToCsSig = CS2_N;
        } else {
          CidToCsSig = CS6_N;
        }
        Prog1[CadbLine].cs |= 1 << (CidToCsSig - CS0_N);
        Prog0[CadbLine].address = 0;
      }
      if (Sig == C1) {
        if (Rank < MAX_RANK_DIMM) {
          CidToCsSig = CS3_N;
        } else {
          CidToCsSig = CS7_N;
        }
        Prog1[CadbLine].cs |= 1 << (CidToCsSig - CS0_N);
        Prog0[CadbLine].address = 0;
      }
      //
      // C2 is controlled from the CADB CID field
      //
      if (Sig == C2) {
        Prog0[CadbLine].cid = 1 << (Sig - C0);
        Prog0[CadbLine].address = 0;
      }
    }

    if (Prog0[CadbLine].address == BIT14) {
      if (Sig == A14) {
        Prog0[CadbLine].address = BIT14;
      }
      if (Sig == A15) {
        Prog0[CadbLine].address = BIT15;
      }
      if (Sig == A16) {
        Prog0[CadbLine].address = BIT16;
      }
    }

    if (Prog0[CadbLine].address == (BIT15 | BIT16)) {
      if (Sig == A14) {
        Prog0[CadbLine].address = (BIT15 | BIT16);
      }
      if (Sig == A15) {
        Prog0[CadbLine].address = (BIT14 | BIT16);
      }
      if (Sig == A16) {
        Prog0[CadbLine].address = (BIT14 | BIT15);
      }
    }
  }
} // FixupBacksideAddrBits

/**

  Loads the CADB

  @param Host    - Pointer to sysHost
  @param socket  - Socket to train
  @param ch      - DDR channel to setup
  @param Subch   - Sub channel number
  @param rank    - Logical rank number
  @param pattern - Data to program

  @retval SUCCESS / FAILURE

**/
VOID
ProgramCADB (
  IN PSYSHOST      Host,
  IN UINT8         socket,
  IN UINT8         ch,
  IN UINT8         subCh,
  IN UINT8         startLine,
  IN UINT8         numLines,
  IN PatCadbProg0  *prog0,
  IN PatCadbProg1  *prog1
  )
{
  EFI_STATUS                          Status;
  UINT8                               cadbLine;
  CPGC_PATCADBPROG0_MCDDC_CTL_STRUCT  patCADBProg0;
#ifndef DDR5_SUPPORT
  CPGC_PATCADBPROG1_MCDDC_CTL_STRUCT  patCADBProg1;
#endif//DDR5_SUPPORT
  UINT32                              FastCadb = FALSE;

#ifdef DDR5_SUPPORT
  if (IsBrsPresent (Host, socket)) {
    ProgramCADBBrs (Host, socket, ch, startLine, numLines, prog0, prog1);
    return;
  }
#endif//DDR5_SUPPORT

  Status = CteGetOptions ("fastcadb", &FastCadb);
  if (EFI_ERROR (Status)) {
    FastCadb = FALSE;
  }

  if (FastCadb == FALSE) {
    RcDebugPrintWithDevice (SDBG_CPGC, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "ProgramCADB: using normal cadb programing\n");
    //
    // Set the write pointer to 0
    //
    MemWritePciCfgEp (socket, ch, CPGC_PATCADBWRPNTR_MCDDC_CTL_REG, startLine);
  } else {
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "ProgramCADB10nm: using fast cadb programing\n");
  }

  //
  // Program 16 lines in the CADB
  //
  for (cadbLine = startLine; cadbLine < numLines; cadbLine++) {
    patCADBProg0.Data          = 0;
    patCADBProg0.Bits.address  = prog0[cadbLine - startLine].address;
#ifndef DDR5_SUPPORT
    patCADBProg0.Bits.cid      = prog0[cadbLine - startLine].cid;
    patCADBProg0.Bits.bank     = prog0[cadbLine - startLine].bank;
#endif//DDR5_SUPPORT
    patCADBProg0.Bits.pdatag   = prog0[cadbLine - startLine].pdatag;
#ifndef DDR5_SUPPORT
    patCADBProg1.Data          = 0;
    patCADBProg1.Bits.cs       = prog1[cadbLine - startLine].cs;
    patCADBProg1.Bits.control  = prog1[cadbLine - startLine].control;
    patCADBProg1.Bits.odt      = prog1[cadbLine - startLine].odt;
    patCADBProg1.Bits.cke      = prog1[cadbLine - startLine].cke;
    patCADBProg1.Bits.par      = prog1[cadbLine - startLine].par;
#endif//DDR5_SUPPORT
    if (FastCadb == FALSE) {
      MemWritePciCfgEp (socket, ch, CPGC_PATCADBPROG0_MCDDC_CTL_REG, patCADBProg0.Data);
#ifndef DDR5_SUPPORT
      MemWritePciCfgEp (socket, ch, CPGC_PATCADBPROG1_MCDDC_CTL_REG, patCADBProg1.Data);
#endif //DDR5_SUPPORT
    } else {
#ifndef DDR5_SUPPORT
      CteProgramCadb (ch, cadbLine, patCADBProg0.Data, patCADBProg1.Data);
#endif //DDR5_SUPPORT
    }
  } // cadbLine loop

  return;

} // ProgramCADB10nm

/**

  Populates the provided CADB row

  @param Host    - Pointer to sysHost
  @param socket  - Socket to train
  @param ch      - DDR channel to setup
  @param rank    - Logical rank number
  @param bank    - MRS to write to
  @param data    - Data to write
  @param side    - Side of the register (A or B)
  @param pdaTag  - PDA

**/
STATIC VOID
ProgCADBRow (
            PSYSHOST      Host,
            UINT8         socket,
            UINT8         ch,
            UINT8         dimm,
            UINT8         rank,
            UINT8         bank,
            UINT32        data,
            UINT8         side,
            UINT8         pdaTag,
            UINT8         control,
            PatCadbProg0  *patCADBProg0,
            PatCadbProg1  *patCADBProg1
            )
{
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  struct channelNvram (*channelNvList)[MAX_CH];
  UINT32              AddressMask;
  BootMode            SysBootMode;

  SysBootMode = GetSysBootMode ();

  CalculateAddressMask (Host, socket, ch, dimm, &AddressMask);

  channelNvList = GetChannelNvList(Host, socket);
  dimmNvList = GetDimmNvList(Host, socket, ch);
  rankList = GetRankNvList(Host, socket, ch, dimm);

  //
  // Mirror address/bank bits
  //
  if ((rank & BIT0) && (*dimmNvList)[dimm].SPDAddrMapp) {
    data  = MirrorAddrBits (Host, data);
    bank  = MirrorBankBits (Host, ch, bank);
  }

  //
  // Invert address/bank bits
  //
  if (side == SIDE_B) {
    data  = InvertAddrBits (data);
    bank  = InvertBankBits (bank);
  }

  (*patCADBProg0).address = (data & AddressMask);
  (*patCADBProg0).bank    = bank;
  (*patCADBProg0).pdatag  = pdaTag;
  (*patCADBProg0).cid     = 0;
  if (IsLrdimmPresent (socket, ch, dimm) && (*channelNvList)[ch].encodedCSMode) {
    // here we are in encoded quad CS mode
    (*patCADBProg0).cid     = (rank >> 1) | 0x6;
  }

  (*patCADBProg1).cs      = ~(1 << (*rankList)[rank].CSIndex) & ((1 << 11) - 1);
  // zero out CID0/1 as CS's bits
  if ((*channelNvList)[ch].encodedCSMode == 2) {
    (*patCADBProg1).cs = (*patCADBProg1).cs   & ~(0xc<<(dimm*4));
  }

  (*patCADBProg1).control = control;

  //
  // Assert ACT bit
  //
  (*patCADBProg1).control |= BIT3;

  //
  // Calculate and set parity
  //
  (*patCADBProg1).par = CalculateCadbParity (channelNvList, patCADBProg0, patCADBProg1, ch);

  (*patCADBProg1).cke = 0x3F;

  // Turn off non-ddrt dimm CKE during DDRT S3 resume.
  if ((*dimmNvList)[dimm].DcpmmPresent == 1 && (SysBootMode == S3Resume)) {
    if (dimm == 0) {
      (*patCADBProg1).cke = BIT0;
    }
    else {
      (*patCADBProg1).cke = BIT2;
    }
  }

  (*patCADBProg1).odt = 0;

} // ProgCADBRow


//-----------------------------------------------------------------------------------------------
/**

  This function loads the WDB using PCI accesses

  @param Host              - Pointer to sysHost
  @param socket            - Socket number
  @param ch                - Channel number
  @param NumberOfWdbLines  - Number of cachelines to load
  @param WdbLines          - Pointer to data to write to the WDB

  @retval N/A

**/
STATIC VOID
WdbPreLoadChunk (
                PSYSHOST  Host,
                UINT8     socket,
                UINT8     ch,
                UINT32    *wdbLines
                )
{
  RcDebugPrintWithDevice (SDBG_CPGC, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "WdbPreLoadChunk Starts\n");
  // Setting rdwr_pntr and rdwr_subpntr to 0
  // cache line 0 and chunk 0
  MemWritePciCfgEp (socket, ch, CPGC_PATWDB_RDWR_PNTR_MCDDC_DP_REG, 0);
  MemWritePciCfgEp (socket, ch, CPGC_PATWDB_WR0_MCDDC_DP_REG, *(wdbLines));
  MemWritePciCfgEp (socket, ch, CPGC_PATWDB_WR1_MCDDC_DP_REG, *(wdbLines + 1));

  //
  // Delay if this is a CTE build, otherwise do nothing.
  //

  CteDelayDclk (4); // 2 didn't work.

} // WdbPreLoadChunk

//-----------------------------------------------------------------------------------------------
/**

Arguments:

  Host        - Pointer to the system Host (root) structure
  ch          - DDR Channel ID

  @retval 0 - success
  @retval 1 - failure

**/
VOID
CpgcSetup64 (
            PSYSHOST Host,
            UINT8  socket,
            UINT8  ch,
            UINT8  dimm,
            UINT32 dpa,
            UINT8  mode,
            UINT8  uiBitMask,
            UINT64_STRUCT *data
            )

{
  UINT8   c;
  UINT8   mcId;
  UINT8   logRank;
  UINT8   NumChPerMc;
  UINT8   MaxChDdr;
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  CPGC_SEQCTL0_MC_MAIN_STRUCT                   seqCtl0;
  CPGC_SUBSEQ0_CTL0_MC_MAIN_STRUCT              subSeqCtl0;
  CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_STRUCT     seqBaseAddrStartLo0;
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_STRUCT     seqBaseAddrStartHi0;
  CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_STRUCT      seqBaseAddrWrapLo0;
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_STRUCT      seqBaseAddrWrapHi0;
  CPGC_SEQ_BAOCIC0_MC_MAIN_STRUCT               seqBAOCIC0;
  CPGC_SEQ_BAINCCTL00_MC_MAIN_STRUCT            seqBAIncCtl00;
  CPGC_SEQ_BAINCCTL10_MC_MAIN_STRUCT            seqBAIncCtl10;
  CPGC_PATWDBCLCTL_MCDDC_CTL_STRUCT             patWDBCLCtl;
  CPGC_PATWDBCL_MUXCTL_MCDDC_DP_STRUCT          patWDBCLMuxCtl;
  CPGC_ERR_CTL_MCDDC_CTL_STRUCT                 errCtl;
  ALIAS_CPGC_GLOBALCTRL_MC_MAIN_STRUCT          cpgcGlobalCtrl;

  NumChPerMc = GetNumChannelPerMc ();
  MaxChDdr = GetMaxChDdr ();

  RcDebugPrintWithDevice (SDBG_CPGC, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CpgcSetup6410nm Starts\n");
  // Disable CPGC on other channels
  for (c = 0; c < MaxChDdr; c++) {
    if (c != ch) {
      MemWritePciCfgMC_Ch (socket, c, CPGC_SEQCTL0_MC_MAIN_REG, 4, 0);   // Clear global control
    }
  }

  dimmNvList = GetDimmNvList(Host, socket, ch);
  mcId = GetMCID (Host, socket, ch);
  logRank = GetLogicalRank(Host, socket, ch, dimm, 0);

  WdbPreLoadChunk (Host, socket, ch, (UINT32 *)data);

  //
  // Program Sequence
  //
  seqCtl0.Data = 0;
  seqCtl0.Bits.local_clear_errs     = 1;
  seqCtl0.Bits.subseq_start_pointer = 0;
  seqCtl0.Bits.subseq_end_pointer   = 0;
  seqCtl0.Bits.global_control       = 1;
  seqCtl0.Bits.initialization_mode  = 1;
  seqCtl0.Bits.loopcount            = 1;
  seqCtl0.Bits.start_test_delay     = 2;      // Delay the start of the test by 2 clocks

  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, seqCtl0.Data);

  //
  // Program Sub Sequences
  //
  subSeqCtl0.Data = 0;
  subSeqCtl0.Bits.rstcurr_ba_start = 1;
  subSeqCtl0.Bits.num_cachelines = 1;
  subSeqCtl0.Bits.num_cachelines_scale = 1;
  subSeqCtl0.Bits.subseq_wait = 0;

  // Choose read vs. write command
  if (mode == READ_ACCESS) {
    MemWritePciCfgMC_Ch (socket, ch, CPGC_SUBSEQ0_CTL0_MC_MAIN_REG, 0x50, subSeqCtl0.Data + (BRd  << 20)); // Read CMD
  } else {
    MemWritePciCfgMC_Ch (socket, ch, CPGC_SUBSEQ0_CTL0_MC_MAIN_REG, 0x50, subSeqCtl0.Data + (BWr  << 20)); // Write CMD
  }

  //
  // Program Sequence Address
  //

  /*
  Address mapping:
              Bank      Row       Column
  CPGC CSR    3:0       16:0      17:0
  NVMDIMM DPA    42:39     38:22   RFU,RFU,21:6

  Note: NVMCTLR DPA CSR aperture is assumed to fit within 32 bit address space,
        so this function does not support access to DPA 4GB and above.
  */

  seqBaseAddrStartLo0.Data              = 0;
  seqBaseAddrStartLo0.Bits.column_addr  = (dpa >> 6) & 0xFFFF;
  seqBaseAddrStartLo0.Bits.bank_addr    = 0;
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_REG, 4, seqBaseAddrStartLo0.Data);
  seqBaseAddrStartHi0.Data              = 0;
  seqBaseAddrStartHi0.Bits.row_addr     = (dpa >> 22) & 0x3FF;
  seqBaseAddrStartHi0.Bits.rank_addr    = 0;                // Use CPGC_SEQ_RANKLG2PHYMAP0 to select physical rank
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_REG, 4, seqBaseAddrStartHi0.Data);

  seqBaseAddrWrapLo0.Data               = 0;
  seqBaseAddrWrapLo0.Bits.column_addr   = seqBaseAddrStartLo0.Bits.column_addr;
  seqBaseAddrWrapLo0.Bits.bank_addr     = seqBaseAddrStartLo0.Bits.bank_addr;
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_REG, 4, seqBaseAddrWrapLo0.Data);
  seqBaseAddrWrapHi0.Data               = 0;
  seqBaseAddrWrapHi0.Bits.row_addr      = seqBaseAddrStartHi0.Bits.row_addr;
  seqBaseAddrWrapHi0.Bits.rank_addr     = 0;
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_REG, 4, seqBaseAddrWrapHi0.Data);

  seqBAOCIC0.Data = 0;
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQ_BAOCIC0_MC_MAIN_REG, 4, seqBAOCIC0.Data);

  seqBAIncCtl00.Data = 0;
  seqBAIncCtl10.Data = 0;
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQ_BAINCCTL00_MC_MAIN_REG, 8, seqBAIncCtl00.Data);
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQ_BAINCCTL10_MC_MAIN_REG, 8, seqBAIncCtl10.Data);

  //
  // Program Write Data Buffer Related Entries
  //
  patWDBCLCtl.Data = 0;
  MemWritePciCfgEp (socket, ch, CPGC_PATWDBCLCTL_MCDDC_CTL_REG, patWDBCLCtl.Data);

  // Select LFSR
  patWDBCLMuxCtl.Data = 0;
  patWDBCLMuxCtl.Bits.mux0 = LFSRMode;
  patWDBCLMuxCtl.Bits.mux1 = LFSRMode;
  patWDBCLMuxCtl.Bits.mux2 = LFSRMode;
  MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUXCTL_MCDDC_DP_REG, patWDBCLMuxCtl.Data);

  // Currently, not planning to use the Inverstion Mask
  //MemWritePciCfgEp (socket, ch, CPGC_PATWDB_INV_MCDDC_DP_REG, 0);

  if (mode == READ_ACCESS) {
    //
    // Program Error Checking
    //
    errCtl.Data                         = 0;
    errCtl.Bits.stop_on_ntherr          = 1;
    errCtl.Bits.stop_on_errcontrol      = 1;
    errCtl.Bits.selective_err_enb_chunk = uiBitMask;
    errCtl.Bits.selective_err_enb_cl    = 0xFF;
    MemWritePciCfgEp (socket, ch, CPGC_ERR_CTL_MCDDC_CTL_REG, errCtl.Data);

    //
    // Do not mask off errors
    //
    MemWritePciCfgEp (socket, ch, CPGC_ERR_DATA0_MCDDC_CTL_REG, 0);

#ifdef DDR5_SUPPORT
    if (IsBrsPresent(Host, socket)) {
      // Account for CPGC mapping on BRS
      MemWritePciCfgEp (socket, ch, CPGC_ERR_DATA1_MCDDC_CTL_REG, 0xFFFFFFFF);
    } else
#endif
    {
      MemWritePciCfgEp (socket, ch, CPGC_ERR_DATA1_MCDDC_CTL_REG, 0);
    }
  }

  // Select target rank type

  cpgcGlobalCtrl.Data = MemReadPciCfgMC (socket, mcId, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG);

  if ((*dimmNvList)[dimm].DcpmmPresent) {
    cpgcGlobalCtrl.Bits.ddrt_addr_mode |= (1 << (ch % NumChPerMc));
  } else {
    cpgcGlobalCtrl.Bits.ddrt_addr_mode &= ~(1 << (ch % NumChPerMc));
  }
  SetCpgcTrainTarget (socket, ch, (*dimmNvList)[dimm].DcpmmPresent);
  SetDdrtForceMode (socket, ch, (*dimmNvList)[dimm].DcpmmPresent);
  MemWritePciCfgMC (socket, mcId, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG, cpgcGlobalCtrl.Data);

  // Physical Rank Mapping
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQ_RANKLG2PHYMAP0_MC_MAIN_REG, 4, logRank);
}

VOID
SetupPatternSequencer (
  PSYSHOST      Host,
  UINT8         socket,
  UINT8         ch,
  SequencerMode sequencerMode,
  UINT32        *sequencerPattern
  )
/*++

Routine Description:

  Setup Data Pattern Configuration and Data Pattern Sequencer registers

Arguments:

  Host                - Pointer to sysHost
  socket              - Memory controller number
  ch                  - channel
  sequencerMode       - SequencerMode struct
  sequencerPattern    - Pointer to array of pattern sequences
                        that are to be programmed

Returns:

  n/a
--*/
{
  CPGC_PATWDBCL_MUXCTL_MCDDC_DP_STRUCT  patWDBCLMuxCtl;
  RcDebugPrintWithDevice (SDBG_CPGC, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "SetupPatternSequencer10nm Starts\n");
  // ECC, Bit Buffer
  patWDBCLMuxCtl.Data = MemReadPciCfgEp (socket, ch, CPGC_PATWDBCL_MUXCTL_MCDDC_DP_REG);
  patWDBCLMuxCtl.Bits.mux0  = sequencerMode.Bits.seqMode0;
  patWDBCLMuxCtl.Bits.mux1  = sequencerMode.Bits.seqMode1;
  patWDBCLMuxCtl.Bits.mux2  = sequencerMode.Bits.seqMode2;
  MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUXCTL_MCDDC_DP_REG, patWDBCLMuxCtl.Data);

  MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX0_PBRD_MCDDC_DP_REG, sequencerPattern[0]);
  MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX1_PBRD_MCDDC_DP_REG, sequencerPattern[1]);
  MemWritePciCfgEp (socket, ch, CPGC_PATWDBCL_MUX2_PBRD_MCDDC_DP_REG, sequencerPattern[2]);
} //SetupPatternSequencer10nm

/**
  Clears the bitwise error mask registers

  @param Host:         Pointer to sysHost
  @param socket:       Processor socket to clear
  @param ch:           Channel to clear

**/
VOID
ClearBwErrorMask (
                 PSYSHOST        Host,
                 UINT8           socket
                 )
{
  UINT8             ch;
  UINT8             dimm;
  UINT8             rank;
  struct bitMask    filter;
  UINT8             MaxChDdr;

  MaxChDdr  = GetMaxChDdr ();

  //
  // Clear the filter bits to enable error checking on every bit
  //
  ClearFilter(Host, socket, &filter);

  // Setup bitMask
  for (ch = 0; ch < MaxChDdr; ch++) {
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      // Loop for each rank
      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {

        if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        SetBwErrorMask (Host, socket, dimm, rank, &filter);
      } // rank loop
    } // dimm loop
  } // ch loop
} // ClearBwErrorMask10nm

/**
  Sets the IOSAV_CH_X_BW_SERR registers to mask off data bits from reporting failures

  @param Host:         Pointer to sysHost
  @param socket:         Processor socket to mask
  @param dimm:         DIMM to mask
  @param rank:         Rank to mask
  @param bitMask:      Bits to be masked

**/
VOID
SetBwErrorMask (
              PSYSHOST        Host,
              UINT8           socket,
              UINT8           dimm,
              UINT8           rank,
              struct bitMask  *mask
              )
{
  UINT8                 ch;
  UINT32                csrReg;
  UINT8                 MaxChDdr;
  struct  channelNvram  (*channelNvList)[MAX_CH];
  struct  ddrRank       (*rankList)[MAX_RANK_DIMM];
  RcDebugPrintWithDevice (SDBG_CPGC, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "SetBwErrorMask10nm Starts\n");
  channelNvList = GetChannelNvList(Host, socket);
  MaxChDdr        = GetMaxChDdr ();

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    rankList = GetRankNvList(Host, socket, ch, dimm);

    csrReg = (UINT32)mask->bits[ch][(*rankList)[rank].rankIndex][0];
    csrReg |= ((UINT32)mask->bits[ch][(*rankList)[rank].rankIndex][1] << 8);
    csrReg |= ((UINT32)mask->bits[ch][(*rankList)[rank].rankIndex][2] << 16);
    csrReg |= ((UINT32)mask->bits[ch][(*rankList)[rank].rankIndex][3] << 24);

    MemWritePciCfgEp (socket, ch, CPGC_ERR_DATA0_MCDDC_CTL_REG, csrReg);

    csrReg = (UINT32)mask->bits[ch][(*rankList)[rank].rankIndex][4];
    csrReg |= ((UINT32)mask->bits[ch][(*rankList)[rank].rankIndex][5] << 8);
    csrReg |= ((UINT32)mask->bits[ch][(*rankList)[rank].rankIndex][6] << 16);
    csrReg |= ((UINT32)mask->bits[ch][(*rankList)[rank].rankIndex][7] << 24);

#ifdef DDR5_SUPPORT
    if (IsBrsPresent(Host, socket)) {
      // Account for CPGC mapping on BRS
      csrReg = 0xFFFFFFFF;
    }
#endif
    MemWritePciCfgEp (socket, ch, CPGC_ERR_DATA1_MCDDC_CTL_REG, csrReg);

    csrReg = (UINT32)mask->bits[ch][(*rankList)[rank].rankIndex][8];
    MemWritePciCfgEp (socket, ch, CPGC_ERR_ECC_MCDDC_CTL_REG, csrReg);

  } // ch loop
} // SetBwErrorMask10nm

STATIC VOID
FillWdbPprPattern (
    PSYSHOST  Host,
    UINT8     socket,
    UINT8     ch,
    UINT8     dimm,
    UINT32    dramMask
 )
/*++

  Setup WDB pattern for PPR

  @param Host        - Pointer to sysHost
  @param socket      - Memory controller to train
  @param ch          - ddr channel to setup
  @param dimm        - DIMM id
  @param dram        - dram that is target of PPR sequence

  @retval None

--*/
{
  UINT8                                         i;
  UINT8                                         maxBits;
  TWdbLine                                      WdbLines[MRC_WDB_LINES];
  CPGC_PATWDB_INV_MCDDC_DP_STRUCT               patwdbInvReg;
  struct dimmNvram                              (*dimmNvList)[MAX_DIMM];
  UINT8                                         dram;
  UINT8 MSVx4 = GetMaxStrobeValid(Host);
  RcDebugPrintWithDevice (SDBG_CPGC, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "FillWdbPprPattern Starts\n");
  dimmNvList    = GetDimmNvList(Host, socket, ch);

  if ((*dimmNvList)[dimm].x4Present) {
    maxBits = 4;
  } else {
    maxBits = 8;
  }

  //
  // Program WDB data for PPR pattern
  //
  for (i = 0; i < MRC_WDB_LINE_SIZE; i++) {
    WdbLines[0].WdbLine[i] = 0xFF;
    for (dram = 0; dram < MSVx4; dram++) {
      if ((~dramMask) & (1 << dram)) {
        continue;
      }

      if (((dram * maxBits) / 8) == (i % 8)) {
        //
        // Insert '0' pattern for target DRAM
        //
        if (maxBits == 4) {
          WdbLines[0].WdbLine[i] ^= (0xF << (4 * (dram % 2)));
        } else {
          WdbLines[0].WdbLine[i] = 0;
        }
      }
    }
  }

  WDBFill (Host, socket, ch, WdbLines, 1, 0);

  //
  // ECC bits 0..7 mirror DQ0..7
  // We need to invert ECC if x8 and DRAM 0/8 or if x4 and DRAM0/16 or DRAM1/17 is the target
  //
  if (Host->nvram.mem.eccEn) {
    patwdbInvReg.Data = 0;
    for (dram = 0; dram < MSVx4; dram++) {
      if ((~dramMask) & (1 << dram)) {
        continue;
      }
      if ((maxBits == 4) && (((dram / 2) == 0) || ((dram / 2) == 8))) {
        patwdbInvReg.Bits.eccinv_or_dcenb ^= (0xF << (4 * (dram % 2)));
      } else if ((maxBits == 8) && ((dram == 0) || (dram == 8))) {
        patwdbInvReg.Bits.eccinv_or_dcenb ^= 0xFF;
      }
    }
    MemWritePciCfgEp (socket, ch, CPGC_PATWDB_INV_MCDDC_DP_REG, patwdbInvReg.Data);
  } // if ecc en
}


UINT32
CpgcSetupPprPattern (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     subRank,
  UINT32    dramMask,
  UINT8     bank,
  UINT32    row
  )
/*++

  CPGC setup for PPR Sequence

  @param Host        - Pointer to sysHost
  @param socket      - Memory controller to train
  @param ch          - ddr channel to setup
  @param dimm        - DIMM id
  @param rank        - rank id
  @param subRank     - sub rank ID
  @param dramMask    - bitmap of dram that is target of PPR sequence
  @param bank        - bank id
  @param row         - row id

  @retval SUCCESS
  @retval FAILURE

--*/
{
  UINT32                                        status = SUCCESS;
  CPGC_GLOBAL_TRAINING_SETUP                    cpgcGblTrainingSetup;
  CPGC_SEQ_BAINCCTL00_MC_MAIN_STRUCT            seqBaincCtl00;
  CPGC_SEQ_BAINCCTL10_MC_MAIN_STRUCT            seqBaincCtl10;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT              cpgcPatCadbCtlReg;
  CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_STRUCT     seqBaseAddrStartLo0;
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_STRUCT     seqBaseAddrStartHi0;
  CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_STRUCT      seqBaseAddrWrapLo0;
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_STRUCT      seqBaseAddrWrapHi0;
  CPGC_SEQ_BAOCIC0_MC_MAIN_STRUCT               cpgcSeqBaoCic0;
  UINT32                                        AddressMask;

  CalculateAddressMask(Host, socket, ch, dimm, &AddressMask);

  RcDebugPrintWithDevice (SDBG_CPGC, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CpgcSetupPprPattern10nm Starts\n");
  cpgcGblTrainingSetup.seqLoopCnt        = 1;                      // test will end after (2^(n-1)) loops through the sub-sequence
  cpgcGblTrainingSetup.startDelay        = 0;                      // Delay start
  cpgcGblTrainingSetup.addrUpdRateMode   = CPGC_UPDATE_BY_CL;      // Update based on the # of read and write cacheline

  // Default to WRITE
  cpgcGblTrainingSetup.rwMode            = CPGC_BASE_WRITE_SUBSEQ;           // Base Read or write - subseq0

  cpgcGblTrainingSetup.numCacheLines     = 0;                                // Number of Cachelines (2^N)
  cpgcGblTrainingSetup.burstSeparation   = 0;                                // (sub_seqwait) - need to add delay to drain credits if doing write / read subseq
  cpgcGblTrainingSetup.errEnChunk        = CPGC_ERR_MONITOR_ALL_CHUNK;       // monitor errors on any chunk
  cpgcGblTrainingSetup.errEnCl           = CPGC_ERR_MONITOR_ALL_CL;          // monitor errors on any cacheline
  cpgcGblTrainingSetup.stopOnErrCtl      = CPGC_ERR_STOP_ON_ALL_ERR;         // Stop on all lane error
  cpgcGblTrainingSetup.subseqEndPtr      = 0;                                // subseq end pointer
  cpgcGblTrainingSetup.useSubSeq1        = CPGC_NO_SUBSEQ1;                  // do not use sub seq1 (base read)
  cpgcGblTrainingSetup.cpgcLinkTrain     = 0;                                // do not use cpgc link train
  cpgcGblTrainingSetup.back2backWrEn     = 1;                                // enable b2b writes
  cpgcGblTrainingSetup.mprControl        = 0;                                // MPR disable
  cpgcGblTrainingSetup.refMask           = 0xFF;                             // disable cpgc self refresh
  cpgcGblTrainingSetup.zqMask            = 0xFF;                             // disable cpgc zq
  cpgcGblTrainingSetup.x8Mode            = 0;

  CpgcGlobalTrainingSetup (Host, socket, ch, &cpgcGblTrainingSetup);

  //
  // Set WDB address order
  //
  seqBaincCtl00.Data = 0;
  seqBaincCtl10.Data = 0;

  seqBaseAddrStartLo0.Data = 0;
  seqBaseAddrStartLo0.Bits.bank_addr = bank;

  seqBaseAddrStartHi0.Data = 0;
  seqBaseAddrStartHi0.Bits.row_addr = row;

  seqBaseAddrWrapLo0.Data = 0;
  seqBaseAddrWrapLo0.Bits.bank_addr = 0xf;
  seqBaseAddrWrapLo0.Bits.column_addr = 0x7f;

  seqBaseAddrWrapHi0.Data = 0;
  seqBaseAddrWrapHi0.Bits.row_addr = AddressMask;
  seqBaseAddrWrapHi0.Bits.rank_addr = 0;

  SelectCPGCRanks (Host, socket, ch, 1 << GetLogicalRank (Host, socket, ch, dimm, rank), 0, subRank);

  cpgcSeqBaoCic0.Data = 0;

  CpgcSetupWdb (Host, socket, ch,
                0,                           // wdbIncrRate
                0,                           // wdbIncrScale
                0,                           // wdbStartPntr (victim bit 0)
                0,                           // wdbEndPntr (victim bit 0)
                1,                           // setupPbwr
                CPGC_WDB_MUX_CTL_PATBUF,     // mux control
                seqBaincCtl00,               // column base address incr
                seqBaincCtl10,
                seqBaseAddrStartLo0,
                seqBaseAddrStartHi0,
                seqBaseAddrWrapLo0,
                seqBaseAddrWrapHi0,
                cpgcSeqBaoCic0
               );

  //
  // Disable CADB
  //
  cpgcPatCadbCtlReg.Data = 0;
  MemWritePciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG, cpgcPatCadbCtlReg.Data);

  //
  // Program WDB data for PPR pattern
  //
  FillWdbPprPattern (Host, socket, ch, dimm, dramMask);
  return status;
}


UINT32
CpgcPprTestGlobalSetup (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     subRank,
  UINT32    dramMask,
  UINT8     bank,
  UINT32    row,
  UINT32    pattern
  )
/*++

  DDR Mem test / Mem Init CPGC setup.

  @param Host        - Pointer to sysHost
  @param socket      - Memory controller to train
  @param ch          - ddr channel to setup
  @param dimm        - DIMM id
  @param rank        - rank id
  @param subRank     - sub rank ID
  @param dram        - dram that is target of PPR sequence
  @param bank        - bank id
  @param row         - row id
  @param pattern     - flag to indicate mem init vs. mem test and read vs. write and data pattern

  @retval SUCCESS
  @retval FAILURE

--*/
{

  UINT32                                        status;
  UINT32                                        wdbData;
  UINT8                                         mcId;
  CPGC_GLOBAL_TRAINING_SETUP                    cpgcGblTrainingSetup;
  CPGC_SEQ_BAINCCTL00_MC_MAIN_STRUCT            seqBaincCtl00;
  CPGC_SEQ_BAINCCTL10_MC_MAIN_STRUCT            seqBaincCtl10;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT              cpgcPatCadbCtlReg;
  CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_STRUCT     seqBaseAddrStartLo0;
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_STRUCT     seqBaseAddrStartHi0;
  CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_STRUCT      seqBaseAddrWrapLo0;
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_STRUCT      seqBaseAddrWrapHi0;
  CPGC_SEQ_BAOCIC0_MC_MAIN_STRUCT               cpgcSeqBaoCic0;

  struct dimmNvram                              (*dimmNvList)[MAX_DIMM];
  ALIAS_CPGC_GLOBALCTRL_MC_MAIN_STRUCT          cpgcGlobalCtrlReg;
  UINT32                                        AddressMask;

  CalculateAddressMask(Host, socket, ch, dimm, &AddressMask);

  RcDebugPrintWithDevice (SDBG_CPGC, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CpgcPprTestSetup Starts\n");
  RcDebugPrintWithDevice (SDBG_CPGC, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                 "CpgcPprTestSetup: pattern = 0x%x\n", pattern);

  status = SUCCESS;

  dimmNvList    = GetDimmNvList(Host, socket, ch);

  cpgcGblTrainingSetup.seqLoopCnt        = 1;                      // test will end after (2^(n-1)) loops through the sub-sequence
  cpgcGblTrainingSetup.startDelay        = 0;                                // Delay start
  cpgcGblTrainingSetup.addrUpdRateMode   = CPGC_UPDATE_BY_CL;                // Update based on the # of read and write cacheline

  // Default to WRITE
  cpgcGblTrainingSetup.rwMode            = CPGC_BASE_WRITE_SUBSEQ;           // Base Read or write - subseq0

  // If MEM_TEST is read, set to READ
  if ((pattern & (DDR_CPGC_MEM_TEST_READ | DDR_CPGC_MEM_TEST_WRITE)) == DDR_CPGC_MEM_TEST_READ) {
    cpgcGblTrainingSetup.rwMode          = CPGC_BASE_READ_SUBSEQ;            // Base Read or write - subseq0
  }

  //
  // Calculate # of cache lines required from the # of column bits we need
  // to sweep in the row
  // Number of Cachelines (2^N)
  //
  mcId = GetMCID (Host, socket, ch);
  cpgcGlobalCtrlReg.Data = MemReadPciCfgMC (socket, mcId, ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG);
  if (cpgcGlobalCtrlReg.Bits.use_bl4_col_addr == 1) {
    cpgcGblTrainingSetup.numCacheLines     =  columnBitsDDR4[(*dimmNvList)[dimm].techIndex] - 2;
  } else {
    cpgcGblTrainingSetup.numCacheLines     =  columnBitsDDR4[(*dimmNvList)[dimm].techIndex] - 3;
  }

  cpgcGblTrainingSetup.burstSeparation   = 0;                                // (sub_seqwait) - need to add delay to drain credits if doing write / read subseq
  cpgcGblTrainingSetup.errEnChunk        = CPGC_ERR_MONITOR_ALL_CHUNK;       // monitor errors on any chunk
  cpgcGblTrainingSetup.errEnCl           = CPGC_ERR_MONITOR_ALL_CL;          // monitor errors on any cacheline
  cpgcGblTrainingSetup.stopOnErrCtl      = CPGC_ERR_STOP_ON_ALL_ERR;         // Stop on all lane error
  cpgcGblTrainingSetup.subseqEndPtr      = 0;                                // subseq end pointer
  if ((pattern & (DDR_CPGC_MEM_TEST_READ | DDR_CPGC_MEM_TEST_WRITE)) == (DDR_CPGC_MEM_TEST_READ | DDR_CPGC_MEM_TEST_WRITE)) {
    cpgcGblTrainingSetup.useSubSeq1        = CPGC_BASE_READ_SUBSEQ;          // use sub seq1 (base read
  } else {
    cpgcGblTrainingSetup.useSubSeq1        = CPGC_NO_SUBSEQ1;                // do not use sub seq1 (base read)
  }
  cpgcGblTrainingSetup.cpgcLinkTrain     = 0;                                // do not use cpgc link train
  cpgcGblTrainingSetup.back2backWrEn     = 1;                                // enable b2b writes
  cpgcGblTrainingSetup.mprControl        = 0;                                // MPR disable
  cpgcGblTrainingSetup.refMask           = 0;                                // enable cpgc self refresh
  cpgcGblTrainingSetup.zqMask            = 0;                                // enable cpgc zq
  cpgcGblTrainingSetup.x8Mode            = 0;

  CpgcGlobalTrainingSetup (Host, socket, ch, &cpgcGblTrainingSetup);

  //
  // Set WDB address order
  //
  seqBaincCtl00.Data = 0;
  seqBaincCtl10.Data = 0;
  seqBaincCtl00.Bits.column_ba_inc = 1;

  seqBaseAddrStartLo0.Data = 0;
  seqBaseAddrStartLo0.Bits.bank_addr = bank;

  seqBaseAddrStartHi0.Data = 0;
  seqBaseAddrStartHi0.Bits.row_addr = row;

  seqBaseAddrWrapLo0.Data = 0;
  seqBaseAddrWrapLo0.Bits.bank_addr = 0xf;
  seqBaseAddrWrapLo0.Bits.column_addr = 0x7f;

  seqBaseAddrWrapHi0.Data = 0;
  seqBaseAddrWrapHi0.Bits.row_addr = AddressMask;
  seqBaseAddrWrapHi0.Bits.rank_addr = 0;

  cpgcSeqBaoCic0.Data = 0;
  cpgcSeqBaoCic0.Bits.column_bw_cryen = 1;

  SelectCPGCRanks (Host, socket, ch, 1 << GetLogicalRank (Host, socket, ch, dimm, rank), 0, subRank);

  cpgcSeqBaoCic0.Data = 0;


  CpgcSetupWdb (Host, socket, ch,
                0,                           // wdbIncrRate
                0,                           // wdbIncrScale
                0,                           // wdbStartPntr (victim bit 0)
                0,                           // wdbEndPntr (victim bit 0)
                1,                           // setupPbwr
                CPGC_WDB_MUX_CTL_PATBUF,     // mux control
                seqBaincCtl00,               // column base address incr
                seqBaincCtl10,
                seqBaseAddrStartLo0,
                seqBaseAddrStartHi0,
                seqBaseAddrWrapLo0,
                seqBaseAddrWrapHi0,
                cpgcSeqBaoCic0
               );

  //
  // Disable CADB
  //
  cpgcPatCadbCtlReg.Data = 0;
  MemWritePciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG, cpgcPatCadbCtlReg.Data);

  //
  // Program WDB data with constant 0's or 1's
  //
  if ((~pattern) & DDR_CPGC_PPR_TEST) {
    wdbData = 0;
    if (pattern & DDR_CPGC_MEM_TEST_ALL0) {
      wdbData = 0;
    } else if (pattern & DDR_CPGC_MEM_TEST_A5) {
      wdbData = 0xA5A5A5A5;
    }
    // Invert pattern
    if (pattern & DDR_CPGC_MEM_TEST_INVERTPAT) {
      wdbData = ~wdbData;
    }

    WriteWDBFixedPattern(Host, socket, ch, wdbData, 8, 0);
  } else {
    // PPR test pattern
    if (pattern & DDR_CPGC_MEM_TEST_INVERTPAT) {
      FillWdbPprPattern (Host, socket, ch, dimm, ~dramMask);
    } else {
      FillWdbPprPattern (Host, socket, ch, dimm, dramMask);
    }
  }

  return status;

} // CpgcPprTestGlobalSetup


UINT32
CpgcPprCleanup (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    chBitMask
  )
/*++

  Clean-up after PPR sequence is done

  @param Host        - Pointer to sysHost
  @param socket      - Memory controller to train
  @param chBitMask   - Bit map of channels to mask off
--*/
{
  UINT8 ch;
  UINT8 MaxChDdr;
  struct channelNvram  (*channelNvList)[MAX_CH];
  RcDebugPrintWithDevice (SDBG_CPGC, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CpgcPprCleanup10nm Starts\n");
  channelNvList = GetChannelNvList(Host, socket);
  MaxChDdr      = GetMaxChDdr ();

  for (ch = 0; ch < MaxChDdr; ch++) {
    //Check if this channel has been masked off as each BSSA Call is per MC
    if ((chBitMask & (1 << ch)) == 0) {
      continue;
    }
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    MemWritePciCfgEp (socket, ch, CPGC_PATWDB_INV_MCDDC_DP_REG, 0);
    MemWritePciCfgEp (socket, ch, CPGC_PATWDB_INV0_MCDDC_DP_REG, 0);
    MemWritePciCfgEp (socket, ch, CPGC_PATWDB_INV1_MCDDC_DP_REG, 0);
  } // ch loop
  return SUCCESS;
}

/**
  This function determines if cpgc is targeting an NVMDIMM Rank

  @param Host:         Pointer to sysHost
  @param socket:       Socket Id
  @param ch:           DDR channel number within the processor socket

  @retval 1:           Rank belongs to AEP Dimm.
  @retval 0:           Rank belongs to DDR4 Dimm
**/

STATIC UINT8
IsAepRank(
          PSYSHOST Host,
          UINT8    socket,
          UINT8    ch,
          UINT8    logRank
)
{
  UINT8            dimm;
  struct dimmNvram (*dimmNvList)[MAX_DIMM];

  dimmNvList = GetDimmNvList(Host, socket, ch);
  dimm = logRank / MAX_RANK_DIMM;
  return ((*dimmNvList)[dimm].DcpmmPresent);
}

STATIC VOID
SetupPmon(
  PSYSHOST        Host,
  UINT8           socket,
  UINT8           ch,
  UINT8           eventSelect,
  UINT8           unitMask
)
{
  PMONCNTRCFG_0_MCDDC_CTL_STRUCT  pmonCntrCfg;
  PMONCNTR_0_N0_MCDDC_CTL_STRUCT                pmonCntr;

  pmonCntr.Data = 0;
  MemWritePciCfgEp (socket, ch, PMONCNTR_0_N0_MCDDC_CTL_REG, pmonCntr.Data);
  MemWritePciCfgEp (socket, ch, PMONCNTR_4_N0_MCDDC_CTL_REG, pmonCntr.Data);


  pmonCntrCfg.Data = 0;
  pmonCntrCfg.Bits.eventselect = eventSelect;
  pmonCntrCfg.Bits.unitmask = unitMask;
  pmonCntrCfg.Bits.counterenable = 1; // enable counter
  MemWritePciCfgEp (socket, ch, PMONCNTRCFG_0_MCDDC_CTL_REG, pmonCntrCfg.Data);
  MemWritePciCfgEp (socket, ch, PMONCNTRCFG_4_MCDDC_CTL_REG, pmonCntrCfg.Data);

}

STATIC UINT32
GetPmonCounter(
               PSYSHOST  Host,
               UINT8     socket,
               UINT8     ch
)
{
  UINT32                          count;
  PMONCNTRCFG_4_MCDDC_CTL_STRUCT  pmonCntrCfg;
  RcDebugPrintWithDevice (SDBG_CPGC, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "GetPmonCounter Starts\n");
  pmonCntrCfg.Data = MemReadPciCfgEp (socket, ch, PMONCNTRCFG_4_MCDDC_CTL_REG);
  pmonCntrCfg.Bits.counterreset = 1;
  MemWritePciCfgEp (socket, ch, PMONCNTRCFG_4_MCDDC_CTL_REG, pmonCntrCfg.Data);

  //
  // Delay only if this is a CTE build, otherwise do nothing.
  //

  CteDelayQclk (50);

  // read counter
  count = MemReadPciCfgEp (socket, ch, PMONCNTR_4_N0_MCDDC_CTL_REG);
  return count;
}

UINT8
isHynix2013 (
PSYSHOST Host,
UINT8    socket,
UINT8    ch
)
{
  UINT8 dimm;
  CPGC_SEQ_RANKLG2PHYMAP0_MC_MAIN_STRUCT cpgcSeqRanklg2phymap;
  struct dimmNvram (*dimmNvList)[MAX_DIMM];

  dimmNvList = GetDimmNvList(Host, socket, ch);
  cpgcSeqRanklg2phymap.Data = MemReadPciCfgMC_Ch (socket, ch, CPGC_SEQ_RANKLG2PHYMAP0_MC_MAIN_REG, 4);
  dimm = (UINT8) cpgcSeqRanklg2phymap.Bits.rank0_mapping / MAX_RANK_DIMM;

  if (((*dimmNvList)[dimm].SPDDramMfgId == MFGID_HYNIX) && (((*dimmNvList)[dimm].SPDOtherOptFeatures & SPD_PPR_MASK) == 0) && (IsLrdimmPresent (socket, ch, dimm) == 0)) {
    return 1;
  } else {
    return 0;
  }
}

/**

  Determine if this DIMM is DDRT or DDR4

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Ch              - Channel number

  @retval 1 - DIMM belongs to AEP Dimm; 0 - DIMM belongs to DDR4 Dimm

**/
UINT8
IsDdrtDimm (
           PSYSHOST Host,
           UINT8    Socket,
           UINT8    Ch
)
{
  CPGC_SEQ_RANKLG2PHYMAP0_MC_MAIN_STRUCT cpgcSeqRanklg2phymap;

  cpgcSeqRanklg2phymap.Data = MemReadPciCfgMC_Ch (Socket, Ch, CPGC_SEQ_RANKLG2PHYMAP0_MC_MAIN_REG, 4);
  return (IsAepRank(Host, Socket, Ch, (UINT8) cpgcSeqRanklg2phymap.Bits.rank0_mapping));
}

/**
  Disable CADB AlwaysOn Mode

  @param Socket - Socket number
  @param Ch     - Channel number

  @retval VOID
**/
VOID
DisableCADBAlwaysOnMode (
  IN UINT8 Socket,
  IN UINT8 Ch
)
{
  PSYSHOST                         Host;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT CpgcPatCadbCtlReg;

  Host = GetSysHostPointer ();

  CpgcPatCadbCtlReg.Data = MemReadPciCfgEp (Socket, Ch, CPGC_PATCADBCTL_MCDDC_CTL_REG);
  CpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 0;
  CpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 0;
  MemWritePciCfgEp (Socket, Ch, CPGC_PATCADBCTL_MCDDC_CTL_REG, CpgcPatCadbCtlReg.Data);
}
#ifdef DDR5_SUPPORT
/**
  Program CPGC to enable or disable IN-ORDER execution


  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Ch                  Current Channel (0-Based)
  @param[in]                Enable              TRUE: Enable IN-ORDER, FALSE: Disable IN-ORDER

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
CpgcInorderModeEnableDdr5 (
  IN        UINT8           Socket,
  IN        UINT8           Ch,
  IN        BOOLEAN         Enable
  )
{
  SCHEDULER_ENABLES_MCDDC_CTL_STRUCT    SchedulerEnables;
  SYSHOST                               *Host;
  UINT8 SubCh;
  UINT8 ChannelLocal;

  Host = GetSysHostPointer ();
  for (SubCh = 0; SubCh < 2; SubCh++){
    GetChannelMapping (Ch, SubCh, &ChannelLocal);

  SchedulerEnables.Data = MemReadPciCfgEp (Socket, ChannelLocal, SCHEDULER_ENABLES_MCDDC_CTL_REG);
  if (Enable) {
    SchedulerEnables.Bits.cpgc_inorder_mode_en = 1;
  } else {
    SchedulerEnables.Bits.cpgc_inorder_mode_en = 0;
  }
  MemWritePciCfgEp (Socket, ChannelLocal, SCHEDULER_ENABLES_MCDDC_CTL_REG, SchedulerEnables.Data);
  }
  return EFI_SUCCESS;
} //CpgcInorderModeEnableDdr5
#endif //DDR5_SUPPORT
/**
  Program CPGC to enable or disable IN-ORDER execution


  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Ch                  Current Channel (0-Based)
  @param[in]                Enable              TRUE: Enable IN-ORDER, FALSE: Disable IN-ORDER

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
CpgcInorderModeEnable (
  IN        UINT8           Socket,
  IN        UINT8           Ch,
  IN        BOOLEAN         Enable
  )
{
  SCHEDULER_ENABLES_MCDDC_CTL_STRUCT    SchedulerEnables;
  SYSHOST                               *Host;

  Host = GetSysHostPointer ();
#ifdef DDR5_SUPPORT
  if (IsBrsPresent (Host, Socket)) {
    return CpgcInorderModeEnableDdr5 (Socket, Ch, Enable);
  }
#endif //DDR5_SUPPORT
  SchedulerEnables.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_ENABLES_MCDDC_CTL_REG);
  if (Enable) {
    SchedulerEnables.Bits.cpgc_inorder_mode_en = 1;
  } else {
    SchedulerEnables.Bits.cpgc_inorder_mode_en = 0;
  }
  MemWritePciCfgEp (Socket, Ch, SCHEDULER_ENABLES_MCDDC_CTL_REG, SchedulerEnables.Data);

  return EFI_SUCCESS;
}

/**
  Configure Uniseq Mode Before Training.

  @param[in]  Socket              Socket number
  @param[in]  Ch                  Channel number

  @retval N/A
**/
VOID
EFIAPI
ConfigureUniseqModeBeforeTraining (
  IN UINT8         Socket,
  IN UINT8         Ch
  )
{
    return;
} //ConfigureUniseqModeBeforeTraining

/**

  This function loads the WDB using PCI accesses

  @param Host                 - Pointer to sysHost
  @param socket               - Socket number
  @param ch                   - Channel number
  @param NumberOfWdbLines     - Number of cachelines to load
  @param WdbLines             - Pointer to data to write to the WDB
  @param StartCachelineIndex  - Start offset/cacheline number on the WDB.

  @retval N/A

**/
VOID
WdbPreLoadChip (
           PSYSHOST  Host,
           UINT8     socket,
           UINT8     ch,
           UINT8     NumberOfWdbLines,
           UINT8     *wdbLines,
           UINT8     StartCachelineIndex
  )
{
  EFI_STATUS                        Status;
  UINT8                             cacheline;
  UINT8                             totalCachelineCount;
  UINT8                             byte;
  UINT8                             chunk;
  UINT8                             MaxChunks;
  CPGC_PATWDB_RDWR_PNTR_MCDDC_DP_STRUCT patWDBRdWrPntr;
  UINT64_STRUCT                         patWDBWr;
  UINT32                            FastWdb = 0;
  UINT8 SubCh;
  UINT8 SubChLoop;
  UINT8 ChannelLocal = 0;
  Status = CteGetOptions ("fastwdb", &FastWdb);
  if (EFI_ERROR (Status)) {
    FastWdb = 0;
  }
  if (IsBrsPresent (Host, socket)) {
    SubChLoop = SUB_CH;
  } else {
    SubChLoop = 1;
  }

  for(SubCh = 0; SubCh < SubChLoop; SubCh++) {
    ChannelLocal = ch;
    if (IsBrsPresent (Host, socket)) {
      GetChannelMapping (ch, SubCh, &ChannelLocal);
    }
   MaxChunks = 8;
   patWDBRdWrPntr.Data = MemReadPciCfgEp (socket, ChannelLocal, CPGC_PATWDB_RDWR_PNTR_MCDDC_DP_REG);

  cacheline = StartCachelineIndex;
  totalCachelineCount = cacheline + NumberOfWdbLines;

  // Loop for each cacheline
  for (; cacheline < totalCachelineCount; cacheline++) {
    if (cacheline >= MRC_WDB_LINES) {
      break;
    }

    // Point to the WDB cacheline entry
    patWDBRdWrPntr.Bits.rdwr_pntr = cacheline;

    // Loop for each data byte - 64bytes per CacheLine, 8bytes per chunk
    for (chunk = 0; chunk < MaxChunks; chunk++) {

      // Point to the WDB cacheline entry
      patWDBRdWrPntr.Bits.rdwr_subpntr = chunk;

      patWDBWr.lo = 0;
      patWDBWr.hi = 0;

      // lower 32 bits -> 0-3bytes
      for (byte = 0; byte < 4; byte++) {
        patWDBWr.lo |= *(wdbLines + (cacheline * MRC_WDB_LINE_SIZE) + (chunk * MRC_WDB_TRANSFERS_PER_LINE) + byte) << (byte * 8);
      } // byte loop
      // upper 32 bits -> 4-7bytes
      for (byte = 4; byte < MRC_WDB_BYTES_PER_TRANSFER; byte++) {
        patWDBWr.hi |= *(wdbLines + (cacheline * MRC_WDB_LINE_SIZE) + (chunk * MRC_WDB_TRANSFERS_PER_LINE) + byte) << ((byte - 4) * 8);
      } // byte loop


      if (FastWdb) {
        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                       "ProgramWDB: using fast wdb programing\n");

        CteProgramWdb (ChannelLocal, patWDBRdWrPntr.Data, patWDBWr.hi, patWDBWr.lo);

      } else {

        MemWritePciCfgEp (socket, ChannelLocal, CPGC_PATWDB_RDWR_PNTR_MCDDC_DP_REG, patWDBRdWrPntr.Data);

        MemWritePciCfgEp (socket, ChannelLocal, CPGC_PATWDB_WR0_MCDDC_DP_REG, patWDBWr.lo);
        MemWritePciCfgEp (socket, ChannelLocal, CPGC_PATWDB_WR1_MCDDC_DP_REG, patWDBWr.hi);

      }

      //
      // Delay if this is a CTE build, otherwise do nothing.
      //
      TrainingDelay (Host, 4, 10);
    }
    } // chunk loop
  } // cacheline loop

} // WdbPreLoadChip10nm


/**
  Initialize CADB pattern buffer

  @param[in]  Host      - Pointer to SysHost
  @param[in]  Socket    - Socket instance

  @retval N/A
**/
VOID
CADBChickenBit (
  IN    PSYSHOST Host,
  IN    UINT8    Socket
  )
{
  UINT8                               Ch;
  UINT32                              CadbMuxPattern[CADB_MUX_MAX];
  UINT8                               CadbMuxCtl[CADB_MUX_MAX];
  struct channelNvram                 (*ChannelNvList)[MAX_CH];
  UINT8                               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // Program CADB pattern buffer
  //
  CadbMuxPattern[0] = CADB_MUX_PATTERN0;
  CadbMuxPattern[1] = CADB_MUX_PATTERN1;
  CadbMuxPattern[2] = CADB_MUX_PATTERN2;
  CadbMuxPattern[3] = CADB_MUX_PATTERN3;

  CadbMuxCtl[0] = CADB_MUX_CTRL_BUFFER;
  CadbMuxCtl[1] = CADB_MUX_CTRL_BUFFER;
  CadbMuxCtl[2] = CADB_MUX_CTRL_BUFFER;
  CadbMuxCtl[3] = CADB_MUX_CTRL_BUFFER;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    //
    // This is the first opportunity to program the CADB Mux.
    // Initialize the cached cadbMuxPattern values to force writes to HW.
    //
    (*ChannelNvList)[Ch].cadbMuxPattern[0] = 0;
    (*ChannelNvList)[Ch].cadbMuxPattern[1] = 0;
    (*ChannelNvList)[Ch].cadbMuxPattern[2] = 0;
    (*ChannelNvList)[Ch].cadbMuxPattern[3] = 0;

    ProgramCadbMux (Socket, Ch, NO_SUBCH, CadbMuxPattern, CadbMuxCtl);

  }
}


/**

  this function writes to CADB

  @param Host            - include all the MRC data
  @param socket             - Memory Controller
  @param ch              - ch to send command to
  @param rank            - CS to send the command to

**/
VOID
ReadMprCADBSeq (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     side,
  UINT8     mprPage,
  UINT8     mprLoc
  )
{
#ifndef DDR5_SUPPORT
  UINT32                                  CRValueSave = 0;
  UINT8                                   bankMR3;
  UINT32                                  dataMR3;
  UINT8                                   bank;
  UINT32                                  data;
  UINT8                                   slot;
  struct dimmNvram                        (*dimmNvList)[MAX_DIMM];
  struct ddrRank                          (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                       (*rankStruct)[MAX_RANK_DIMM];
  PatCadbProg0                            patCADBProg0[CADB_LINES];
  PatCadbProg1                            patCADBProg1[CADB_LINES];
  CPGC_PATCADBMRS_MCDDC_CTL_STRUCT        patCADBMrs;
  CPGC_SEQCTL0_MC_MAIN_STRUCT             seqCtl0;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT        cpgcPatCadbCtlReg;
  struct channelNvram                     (*channelNvList)[MAX_CH];
  UINT32                                  AddressMask;

  CalculateAddressMask (Host, socket, ch, dimm, &AddressMask);

  if (Host->var.mem.socket[socket].hostRefreshStatus == 1) {
    FixedDelayMicroSecond (10);
  }

  channelNvList = GetChannelNvList(Host, socket);
  dimmNvList = GetDimmNvList(Host, socket, ch);
  rankList = GetRankNvList(Host, socket, ch, dimm);
  rankStruct  = GetRankStruct(Host, socket, ch, dimm);

  //
  // Pointer will be dynamically incremented after a write to CADB_PROG register
  //
  MemWritePciCfgEp (socket, ch, CPGC_PATCADBWRPNTR_MCDDC_CTL_REG, 0);
  //
  // Save before MR command
  //
  CRValueSave = MemReadPciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4);
  seqCtl0.Data                      = 0;
  seqCtl0.Bits.initialization_mode  = MRS_Mode;
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, seqCtl0.Data);

  cpgcPatCadbCtlReg.Data = MemReadPciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG);
  cpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
  cpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 0;
  cpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 0;
  MemWritePciCfgEp (socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG, cpgcPatCadbCtlReg.Data);

  ZeroMem (patCADBProg0, sizeof (patCADBProg0));
  ZeroMem (patCADBProg1, sizeof (patCADBProg1));

  // Command 0 - MR3 write to side B if present, or side A if not
  slot = 0;
  if (Host->var.mem.socket[socket].hostRefreshStatus) {
    //
    // Placeholder for deselect on entry
    //
    slot = slot + 1;
  }

  dataMR3 = (*rankStruct)[rank].MR3 | MR3_MPR | mprPage;
  bankMR3 = BANK3;
  //
  // Mirror address/bank bits
  //
  if ((rank & BIT0) && (*dimmNvList)[dimm].SPDAddrMapp) {
    dataMR3  = MirrorAddrBits (Host, dataMR3);
    bankMR3  = MirrorBankBits (Host, ch, bankMR3);
  }
  //
  // Invert address/bank bits
  //
  if (Host->nvram.mem.dimmTypePresent == RDIMM) {
    dataMR3  = InvertAddrBits (dataMR3);
    bankMR3  = InvertBankBits (bankMR3);
  }

  patCADBProg0[slot].address = (dataMR3 & AddressMask);
  patCADBProg0[slot].bank    = bankMR3;
  patCADBProg0[slot].pdatag  = NO_PDA;
  patCADBProg0[slot].cid     = 0;
  patCADBProg1[slot].cs      = ~(1 << (*rankList)[rank].CSIndex) & ((1 << 11) - 1);
  patCADBProg1[slot].control = MRS_CMD;

  patCADBProg1[slot].cke = 0x3F;
  patCADBProg1[slot].odt = 0;

  //
  // Assert ACT bit
  //
  patCADBProg1[slot].control |= BIT3;

  patCADBProg1[slot].par = CalculateCadbParity (channelNvList, &(patCADBProg0[slot]), &(patCADBProg1[slot]), ch);

  // Command 1 - MR3 write to side A
  slot = slot + 1;
  dataMR3 = (*rankStruct)[rank].MR3 | MR3_MPR | mprPage;
  bankMR3 = BANK3;

  //
  // Mirror address/bank bits
  //
  if ((rank & BIT0) && (*dimmNvList)[dimm].SPDAddrMapp) {
    dataMR3  = MirrorAddrBits (Host, dataMR3);
    bankMR3  = MirrorBankBits (Host, ch, bankMR3);
  }

  patCADBProg0[slot].address = (dataMR3 & AddressMask);
  patCADBProg0[slot].bank    = bankMR3;
  patCADBProg0[slot].pdatag  = NO_PDA;
  patCADBProg0[slot].cid     = 0;
  patCADBProg1[slot].cs      = ~(1 << (*rankList)[rank].CSIndex) & ((1 << 11) - 1);
  patCADBProg1[slot].control = MRS_CMD;

  //
  // Assert ACT bit
  //
  patCADBProg1[slot].control |= BIT3;

  patCADBProg1[slot].par = CalculateCadbParity (channelNvList, &(patCADBProg0[slot]), &(patCADBProg1[slot]), ch);
  patCADBProg1[slot].cke = 0x3F;
  patCADBProg1[slot].odt = 0;

  // Command 2 - RD from given side
  slot = slot + 1;
  data = 0;
  bank = mprLoc;

  //
  // Mirror address/bank bits
  //
  if ((rank & BIT0) && (*dimmNvList)[dimm].SPDAddrMapp) {
    data  = MirrorAddrBits (Host, data);
    bank  = MirrorBankBits (Host, ch, bank);
  }

  //
  // Invert address/bank bits
  //
  if (side == SIDE_B) {
    data  = InvertAddrBits (data);
    bank  = InvertBankBits (bank);
  }

  patCADBProg0[slot].address = (data & AddressMask);
  patCADBProg0[slot].bank    = bank;
  patCADBProg0[slot].pdatag  = NO_PDA;
  patCADBProg0[slot].cid     = 0;
  patCADBProg1[slot].cs      = ~(1 << (*rankList)[rank].CSIndex) & ((1 << 11) - 1);
  patCADBProg1[slot].control = RD_CMD;

  //
  // Assert ACT bit
  //
  patCADBProg1[slot].control |= BIT3;

  patCADBProg1[slot].par = CalculateCadbParity (channelNvList, &(patCADBProg0[slot]), &(patCADBProg1[slot]), ch);
  patCADBProg1[slot].cke = 0x3F;
  patCADBProg1[slot].odt = 0;

  // Command 3 - MR3 write to side B if present, or side A if not
  slot = slot + 1;
  dataMR3  = (*rankStruct)[rank].MR3;
  bankMR3 = BANK3;

  //
  // Mirror address/bank bits
  //
  if ((rank & BIT0) && (*dimmNvList)[dimm].SPDAddrMapp) {
    dataMR3  = MirrorAddrBits (Host, dataMR3);
    bankMR3  = MirrorBankBits (Host, ch, bankMR3);
  }

  //
  // Invert address/bank bits
  //
  if (Host->nvram.mem.dimmTypePresent == RDIMM) {
    dataMR3  = InvertAddrBits (dataMR3);
    bankMR3  = InvertBankBits (bankMR3);
  }

  patCADBProg0[slot].address = (dataMR3 & AddressMask);
  patCADBProg0[slot].bank    = bankMR3;
  patCADBProg0[slot].pdatag  = NO_PDA;
  patCADBProg0[slot].cid     = 0;

  patCADBProg1[slot].cs      = ~(1 << (*rankList)[rank].CSIndex) & ((1 << 11) - 1);
  patCADBProg1[slot].control = MRS_CMD;

  //
  // Assert ACT bit
  //
  patCADBProg1[slot].control |= BIT3;

  patCADBProg1[slot].par = CalculateCadbParity (channelNvList, &(patCADBProg0[slot]), &(patCADBProg1[slot]), ch);
  patCADBProg1[slot].cke = 0x3F;
  patCADBProg1[slot].odt = 0;

  // Command 4 - MR3 write to side A
  slot = slot + 1;
  dataMR3  = (*rankStruct)[rank].MR3;
  bankMR3 = BANK3;

  //
  // Mirror address/bank bits
  //
  if ((rank & BIT0) && (*dimmNvList)[dimm].SPDAddrMapp) {
    dataMR3  = MirrorAddrBits (Host, dataMR3);
    bankMR3  = MirrorBankBits (Host, ch, bankMR3);
  }

  patCADBProg0[slot].address = (dataMR3 & AddressMask);
  patCADBProg0[slot].bank    = bankMR3;
  patCADBProg0[slot].pdatag  = NO_PDA;
  patCADBProg0[slot].cid     = 0;
  if (IsLrdimmPresent (socket, ch, dimm) && (*channelNvList)[ch].encodedCSMode) {
    // here we are in encoded quad CS mode
    patCADBProg0[slot].cid     = (rank >> 1) | 0x6;
  }
  patCADBProg1[slot].cs      = ~(1 << (*rankList)[rank].CSIndex) & ((1 << 11) - 1);
  patCADBProg1[slot].control = MRS_CMD;

  //
  // Assert ACT bit
  //
  patCADBProg1[slot].control |= BIT3;

  patCADBProg1[slot].par = CalculateCadbParity (channelNvList, &(patCADBProg0[slot]), &(patCADBProg1[slot]), ch);
  patCADBProg1[slot].cke = 0x3F;
  patCADBProg1[slot].odt = 0;

  if (Host->var.mem.socket[socket].hostRefreshStatus) {
    //
    // Placeholder for deselect on exit
    //
    slot = slot + 1;
  }
  // Program commands into CADB

  ProgramCADB (Host, socket, ch, NO_SUBCH, 0, slot + 1, patCADBProg0, patCADBProg1);

  if (Host->var.mem.socket[socket].hostRefreshStatus) {
    //
    // Add a deselect
    //
    ProgramCADB (Host, socket, ch, NO_SUBCH, 0, 1, &patCADBProg0[0], &DeselectPattern1[0]);
    ProgramCADB (Host, socket, ch, NO_SUBCH, slot, slot + 1, &patCADBProg0[0], &DeselectPattern1[0]);
  }

  //
  // Execute MRS Mode
  //
  patCADBMrs.Data                     = 0;
  if (Host->var.mem.socket[socket].hostRefreshStatus == 1) {
    patCADBMrs.Bits.mrs_gap = 9;
  }
  else {
    patCADBMrs.Bits.mrs_gap = 5;
  }
  patCADBMrs.Bits.mrs_start_pointer = 0;
  patCADBMrs.Bits.mrs_end_pointer = slot;
  MemWritePciCfgEp (socket, ch, CPGC_PATCADBMRS_MCDDC_CTL_REG, patCADBMrs.Data);

  // Start test and clear errors
  seqCtl0.Bits.local_clear_errs  = 1;
  seqCtl0.Bits.local_start_test  = 1;
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, seqCtl0.Data);

  //
  // Poll test completion
  //
  CpgcPollGlobalTestDone (Host, socket, 1 << ch, NO_SUBCH);

  //
  // Restore after MR command
  //
  seqCtl0.Data = MemReadPciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4);
  seqCtl0.Bits.local_start_test = 0;
  seqCtl0.Bits.local_clear_errs = 1;
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, seqCtl0.Data);
  MemWritePciCfgMC_Ch (socket, ch, CPGC_SEQCTL0_MC_MAIN_REG, 4, CRValueSave);

#endif // !DDR5_SUPPORT
} // ReadMprCADBSeq

/**

  Reads MPR page/location from given rank and updates associated MprData per DRAM device

  @param Host    - Pointer to sysHost
  @param socket  - Socket Id
  @param MprData - each byte is updated with UI[0-7] data per DRAM device

**/
VOID
ReadMprRankDdr4 (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     mprPage,
  UINT8     mprLoc,
  UINT8     MprData[MAX_STROBE]
  )
{
  MR_READ_RESULT_DQ12TO0_MCDDC_DP_STRUCT  mrReadResultDq12to0;
  MR_READ_RESULT_DQ28TO16_MCDDC_DP_STRUCT mrReadResultDq28to16;
  MR_READ_RESULT_DQ44TO32_MCDDC_DP_STRUCT mrReadResultDq44to32;
  MR_READ_RESULT_DQ60TO48_MCDDC_DP_STRUCT mrReadResultDq60to48;
  MR_READ_RESULT_DQ68TO64_MCDDC_DP_STRUCT mrReadResultDq68to64;
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];


  dimmNvList    = GetDimmNvList(Host, socket, ch);

  ReadMprCADBSeq (Host, socket, ch, dimm, rank, SIDE_A, mprPage, mprLoc);

  // Get side A results
  mrReadResultDq12to0.Data  = MemReadPciCfgEp (socket, ch, MR_READ_RESULT_DQ12TO0_MCDDC_DP_REG);
  mrReadResultDq28to16.Data = MemReadPciCfgEp (socket, ch, MR_READ_RESULT_DQ28TO16_MCDDC_DP_REG);
  mrReadResultDq44to32.Data = MemReadPciCfgEp (socket, ch, MR_READ_RESULT_DQ44TO32_MCDDC_DP_REG);
  mrReadResultDq60to48.Data = MemReadPciCfgEp (socket, ch, MR_READ_RESULT_DQ60TO48_MCDDC_DP_REG);
  mrReadResultDq68to64.Data = MemReadPciCfgEp (socket, ch, MR_READ_RESULT_DQ68TO64_MCDDC_DP_REG);

  MprData[0] = (UINT8)((mrReadResultDq12to0.Data  >> 0)  & 0xFF);
  MprData[1] = (UINT8)((mrReadResultDq12to0.Data  >> 16) & 0xFF);
  MprData[2] = (UINT8)((mrReadResultDq28to16.Data >> 0)  & 0xFF);
  MprData[3] = (UINT8)((mrReadResultDq28to16.Data >> 16) & 0xFF);
  MprData[4] = (UINT8)((mrReadResultDq44to32.Data >> 0)  & 0xFF);
  MprData[5] = (UINT8)((mrReadResultDq44to32.Data >> 16) & 0xFF);
  MprData[6] = (UINT8)((mrReadResultDq60to48.Data >> 0)  & 0xFF);
  MprData[7] = (UINT8)((mrReadResultDq60to48.Data >> 16) & 0xFF);
  MprData[8] = (UINT8)((mrReadResultDq68to64.Data >> 0)  & 0xFF);

  if ((*dimmNvList)[dimm].x4Present) {

    MprData[9]  = (UINT8)((mrReadResultDq12to0.Data  >> 8)  & 0xFF);
    MprData[10] = (UINT8)((mrReadResultDq12to0.Data  >> 24) & 0xFF);
    MprData[11] = (UINT8)((mrReadResultDq28to16.Data >> 8)  & 0xFF);
    MprData[12] = (UINT8)((mrReadResultDq28to16.Data >> 24) & 0xFF);
    MprData[13] = (UINT8)((mrReadResultDq44to32.Data >> 8)  & 0xFF);
    MprData[14] = (UINT8)((mrReadResultDq44to32.Data >> 24) & 0xFF);
    MprData[15] = (UINT8)((mrReadResultDq60to48.Data >> 8)  & 0xFF);
    MprData[16] = (UINT8)((mrReadResultDq60to48.Data >> 24) & 0xFF);
    MprData[17] = (UINT8)((mrReadResultDq68to64.Data >> 8)  & 0xFF);

  } //x4

  if (Host->nvram.mem.dimmTypePresent == RDIMM) {

    ReadMprCADBSeq (Host, socket, ch, dimm, rank, SIDE_B, mprPage, mprLoc);

    // Get side B results
    mrReadResultDq12to0.Data  = MemReadPciCfgEp (socket, ch, MR_READ_RESULT_DQ12TO0_MCDDC_DP_REG);
    mrReadResultDq28to16.Data = MemReadPciCfgEp (socket, ch, MR_READ_RESULT_DQ28TO16_MCDDC_DP_REG);
    mrReadResultDq44to32.Data = MemReadPciCfgEp (socket, ch, MR_READ_RESULT_DQ44TO32_MCDDC_DP_REG);
    mrReadResultDq60to48.Data = MemReadPciCfgEp (socket, ch, MR_READ_RESULT_DQ60TO48_MCDDC_DP_REG);
    mrReadResultDq68to64.Data = MemReadPciCfgEp (socket, ch, MR_READ_RESULT_DQ68TO64_MCDDC_DP_REG);

    // Update results
    // TODO: implement side B discovery of DRAM mapping
    MprData[4] = (UINT8)((mrReadResultDq44to32.Data >> 0)  & 0xFF);
    MprData[5] = (UINT8)((mrReadResultDq44to32.Data >> 16) & 0xFF);
    MprData[6] = (UINT8)((mrReadResultDq60to48.Data >> 0)  & 0xFF);
    MprData[7] = (UINT8)((mrReadResultDq60to48.Data >> 16) & 0xFF);

    if ((*dimmNvList)[dimm].x4Present) {

      MprData[13] = (UINT8)((mrReadResultDq44to32.Data >> 8)  & 0xFF);
      MprData[14] = (UINT8)((mrReadResultDq44to32.Data >> 24) & 0xFF);
      MprData[15] = (UINT8)((mrReadResultDq60to48.Data >> 8)  & 0xFF);
      MprData[16] = (UINT8)((mrReadResultDq60to48.Data >> 24) & 0xFF);

    } //x4
  } // RDIMM

} // ReadMprRankDdr4

#ifdef LRDIMM_SUPPORT
/**

  Reads MPR page/location from given rank and updates associated MprData per DRAM device

  @param Host    - Pointer to sysHost
  @param socket  - Socket Id
  @param MprData - each byte is updated with UI[0-7] data per DRAM device

**/
VOID
ReadBcwDimmDdr4 (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     bcwFunc,
  UINT8     bcwByte,
  UINT8     bcwNibble,
  UINT8     MprData[MAX_STROBE]
  )
{
  MR_READ_RESULT_DQ12TO0_MCDDC_DP_STRUCT  mrReadResultDq12to0;
  MR_READ_RESULT_DQ28TO16_MCDDC_DP_STRUCT mrReadResultDq28to16;
  MR_READ_RESULT_DQ44TO32_MCDDC_DP_STRUCT mrReadResultDq44to32;
  MR_READ_RESULT_DQ60TO48_MCDDC_DP_STRUCT mrReadResultDq60to48;
  MR_READ_RESULT_DQ68TO64_MCDDC_DP_STRUCT mrReadResultDq68to64;
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  UINT8 controlWordData;

  dimmNvList    = GetDimmNvList(Host, socket, ch);

  // Montage requires BCW function select inside the BCW Read protocol and does not use BC7x for BCW Read
  // Write BC7x = 0 to be on safe side.
  WriteLrbuf(Host, socket, ch, dimm, 0, 0, 0, LRDIMM_BC7x);

  if (!(*dimmNvList)[dimm].DcpmmPresent) {

    // CW Source selection
    controlWordData = (bcwFunc << 5) | BIT4 | bcwByte;
    WriteRC(Host, socket, ch, dimm, 0, controlWordData, RDIMM_RC4x);

    // CW Source selection
    controlWordData = bcwNibble << 4;
    WriteRC (Host, socket, ch, dimm, 0, controlWordData, RDIMM_RC6x);

    // CW Read operation
    controlWordData = 4;    //CMD 4 - CW Read Operation
    WriteRC (Host, socket, ch, dimm, 0, controlWordData, RDIMM_RC06);

    //
    // Read the data
    //
    ReadMprCADBSeq (Host, socket, ch, dimm, 0, SIDE_A, 0, 0);

    // Get side A results
    mrReadResultDq12to0.Data = MemReadPciCfgEp (socket, ch, MR_READ_RESULT_DQ12TO0_MCDDC_DP_REG);
    mrReadResultDq28to16.Data = MemReadPciCfgEp (socket, ch, MR_READ_RESULT_DQ28TO16_MCDDC_DP_REG);
    mrReadResultDq44to32.Data = MemReadPciCfgEp (socket, ch, MR_READ_RESULT_DQ44TO32_MCDDC_DP_REG);
    mrReadResultDq60to48.Data = MemReadPciCfgEp (socket, ch, MR_READ_RESULT_DQ60TO48_MCDDC_DP_REG);
    mrReadResultDq68to64.Data = MemReadPciCfgEp (socket, ch, MR_READ_RESULT_DQ68TO64_MCDDC_DP_REG);

    MprData[0] = (UINT8)((mrReadResultDq12to0.Data >> 0) & 0xFF);
    MprData[1] = (UINT8)((mrReadResultDq12to0.Data >> 16) & 0xFF);
    MprData[2] = (UINT8)((mrReadResultDq28to16.Data >> 0) & 0xFF);
    MprData[3] = (UINT8)((mrReadResultDq28to16.Data >> 16) & 0xFF);
    MprData[4] = (UINT8)((mrReadResultDq44to32.Data >> 0) & 0xFF);
    MprData[5] = (UINT8)((mrReadResultDq44to32.Data >> 16) & 0xFF);
    MprData[6] = (UINT8)((mrReadResultDq60to48.Data >> 0) & 0xFF);
    MprData[7] = (UINT8)((mrReadResultDq60to48.Data >> 16) & 0xFF);
    MprData[8] = (UINT8)((mrReadResultDq68to64.Data >> 0) & 0xFF);

    if ((*dimmNvList)[dimm].x4Present) {

      MprData[9] = (UINT8)((mrReadResultDq12to0.Data >> 8) & 0xFF);
      MprData[10] = (UINT8)((mrReadResultDq12to0.Data >> 24) & 0xFF);
      MprData[11] = (UINT8)((mrReadResultDq28to16.Data >> 8) & 0xFF);
      MprData[12] = (UINT8)((mrReadResultDq28to16.Data >> 24) & 0xFF);
      MprData[13] = (UINT8)((mrReadResultDq44to32.Data >> 8) & 0xFF);
      MprData[14] = (UINT8)((mrReadResultDq44to32.Data >> 24) & 0xFF);
      MprData[15] = (UINT8)((mrReadResultDq60to48.Data >> 8) & 0xFF);
      MprData[16] = (UINT8)((mrReadResultDq60to48.Data >> 24) & 0xFF);
      MprData[17] = (UINT8)((mrReadResultDq68to64.Data >> 8) & 0xFF);

    } //x4
  } else {

    ReadBcwDimmFmc (Host, socket, ch, dimm, bcwFunc, bcwByte, bcwNibble, MprData);
  }

  // Disable DB function override
  WriteLrbuf(Host, socket, ch, dimm, 0, 0, 0, LRDIMM_BC7x);

} // ReadBcwRankDdr4
#endif //LRDIMM_SUPPORT

VOID
InitRtlOffsetChip (
  UINT8     rtlOffset[2]
  )
{
  rtlOffset[0] = RTL_OFFSET_0;
  rtlOffset[1] = RTL_OFFSET_1;
  return;
}

/**

  This routine to set CpgcDdrtMiscCtl register
  @param  Host                  - Host pointer
  @param  Socket                - Socket index
  @param  Ch                    - Channel index
  @param  CpgcMaxCredit
  @param  DisableMaxCreditCheck - boolean
  @param  EnableEridReturn      - boolean
  @param  MultiCreditOn         - boolean

  @retval N/A

**/
VOID
SetCpgcDdrtMiscCtl (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch,
  BOOLEAN  ChangeMaxCredit,
  UINT8    CpgcMaxCredit,
  BOOLEAN  DisableMaxCreditCheck,
  BOOLEAN  EnableEridReturn,
  BOOLEAN  MultiCreditOn
  )
{
  CPGC_DDRT_MISC_CTL_MC_2LM_STRUCT CpgcDdrtMiscCtl;

  CpgcDdrtMiscCtl.Data = MemReadPciCfgEp (Socket, Ch, CPGC_DDRT_MISC_CTL_MC_2LM_REG);
  if (ChangeMaxCredit) {
    CpgcDdrtMiscCtl.Bits.cpgc_max_credit         = CpgcMaxCredit;
  }
  CpgcDdrtMiscCtl.Bits.disable_max_credit_check  = DisableMaxCreditCheck;
  CpgcDdrtMiscCtl.Bits.enable_erid_return        = EnableEridReturn;
  CpgcDdrtMiscCtl.Bits.multi_credit_on           = MultiCreditOn;
  CpgcDdrtMiscCtl.Bits.cpgc_wait_for_rpq_empty   = ENABLE_WAIT_FOR_DDRT_RPQ_EMPTY;
  CpgcDdrtMiscCtl.Bits.use_wr_fifo               = ENABLE_USE_WRITE_TRACKER;
  MemWritePciCfgEp (Socket, Ch, CPGC_DDRT_MISC_CTL_MC_2LM_REG, CpgcDdrtMiscCtl.Data);

}

/**

  This routine to set  multi_credit_on field in register CpgcDdrtMiscCtl
  @param  Host                  - Host pointer
  @param  Socket                - Socket index
  @param  Ch                    - Channel index
  @param  MultiCreditOn         - boolean

  @retval N/A

**/
VOID
SetCpgcMultiCreditOn (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch,
  BOOLEAN  MultiCreditOn
  )
{

  CPGC_DDRT_MISC_CTL_MC_2LM_STRUCT CpgcDdrtMiscCtl;

  CpgcDdrtMiscCtl.Data = MemReadPciCfgEp (Socket, Ch, CPGC_DDRT_MISC_CTL_MC_2LM_REG);
  CpgcDdrtMiscCtl.Bits.multi_credit_on           = MultiCreditOn;
  MemWritePciCfgEp (Socket, Ch, CPGC_DDRT_MISC_CTL_MC_2LM_REG, CpgcDdrtMiscCtl.Data);
}

/**

  This routine to set CpgcRpqForceDrain in CpgcDdrtMiscCtl register
  @param  Host                  - Host pointer
  @param  Socket                - Socket index
  @param  Ch                    - Channel index
  @param  CpgcRpqForceDrain     - boolean

  @retval N/A

**/

VOID
SetCpgcRpqForceDrain (
  PSYSHOST Host,
  UINT8 Socket,
  UINT8 Ch,
  BOOLEAN  CpgcRpqForceDrain
  )
{
  CPGC_DDRT_MISC_CTL_MC_2LM_STRUCT  CpgcDdrtMiscCtl;

  CpgcDdrtMiscCtl.Data = MemReadPciCfgEp (Socket, Ch, CPGC_DDRT_MISC_CTL_MC_2LM_REG);
  CpgcDdrtMiscCtl.Bits.cpgc_rpq_force_drain      = CpgcRpqForceDrain;
  MemWritePciCfgEp (Socket, Ch, CPGC_DDRT_MISC_CTL_MC_2LM_REG, CpgcDdrtMiscCtl.Data);

}

/**

  This routine gets/sets CSRs required by BiosSetZQCalConfig

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param Controller                 - Controller number (0-based)
  @param Enable                     - Specifies whether ZQCal is enabled.  TRUE enables ZQCal; FALSE disables it
  @param PrevEnable                 - Pointer to where the previous ZQCal enable setting will be stored. This value may be NULL

  @retval N/A

**/
VOID
BiosSetZQCalConfigChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Controller,
  BOOLEAN               Enable,
  BOOLEAN               *PrevEnable
  )
{
  UINT8                                     channelInSocket = 0;
  UINT8                                     maxChannel = 0;
  UINT8                                     iteration = 0;
  struct channelNvram                       (*channelNvList)[MAX_CH];
  CPGC_MISCZQCTL_MCDDC_CTL_STRUCT           miscZqCtl;

  channelNvList = GetChannelNvList(Host, Socket);

  channelInSocket = (Controller * GetNumChannelPerMc());
  maxChannel = (Controller+1) * GetNumChannelPerMc() ;

  for (channelInSocket; channelInSocket < maxChannel; channelInSocket++) {
    if ((*channelNvList)[channelInSocket].enabled == 0) {
      continue;
    }

    miscZqCtl.Data  = MemReadPciCfgEp (Socket, channelInSocket, CPGC_MISCZQCTL_MCDDC_CTL_REG);

    if ((PrevEnable != NULL) && (iteration == 0)) {  //Get Case. Save Previous value for only the First Enabled Channel
      if (miscZqCtl.Bits.zq_rankmask == 0) {  //Enable ZQ Cal
        *PrevEnable = TRUE;
      } else {
        *PrevEnable = FALSE;
      }
    }

     miscZqCtl.Bits.zq_rankmask = (Enable) ? 0 : 0xFF;  // 0xFF --> disable ZQ to all the Ranks on the Channel
     MemWritePciCfgEp (Socket, channelInSocket, CPGC_MISCZQCTL_MCDDC_CTL_REG, miscZqCtl.Data);

     iteration = 1;
  } //for-loop
}


/**

This function applies the given ZQ mask

@param Host      - Pointer to sysHost
@param socket    - Socket
@param ch        - Channel
@param ZQMask    - ZQ mask to apply

@retval none

**/
VOID
SetZQMask (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     ZQMask
  )
{
  CPGC_MISCZQCTL_MCDDC_CTL_STRUCT  miscZqCtl;

  miscZqCtl.Data = MemReadPciCfgEp (socket, ch, CPGC_MISCZQCTL_MCDDC_CTL_REG);
  //
  // ZQ
  //
  miscZqCtl.Bits.always_do_zq = 0; // ZQ operation will take place at the normal programmed period after a refresh occurs as long as the DIMM/rank does not appear in the DimmRankPairs array
  miscZqCtl.Bits.zq_rankmask = ZQMask;
  MemWritePciCfgEp (socket, ch, CPGC_MISCZQCTL_MCDDC_CTL_REG, miscZqCtl.Data);

  return;
} // SetZQMask
/**

This function applies the given Refresh mask

@param Host      - Pointer to sysHost
@param socket    - Socket
@param ch        - Channel
@param refMask   - Refresh mask to apply

@retval none

**/
VOID
SetRefreshMask (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     refMask
  )
{
  CPGC_MISCREFCTL_MCDDC_CTL_STRUCT  miscRefCtl;

  miscRefCtl.Data = MemReadPciCfgEp (socket, ch, CPGC_MISCREFCTL_MCDDC_CTL_REG);
  //
  // Enable Refresh - 0's to the the desired ranks
  //
  miscRefCtl.Bits.ref_rankmask = refMask; // Enable all non-selected ranks
  MemWritePciCfgEp (socket, ch, CPGC_MISCREFCTL_MCDDC_CTL_REG, miscRefCtl.Data);

  return;
} // SetRefreshMask

/**

  This routine to sets or clears refresh enable based on RefreshEnable
  In 10nm version, it introduced stagger host refresh enable feature

  @param[in]  Host                    - Host pointer
  @param[in]  Socket                  - Socket index
  @param[in]  TrefiPerAviableChannel  - Enbable Stragger Trefi Per Available Channel
  @param[in]  NumberOfRank            - Select number of rank to keep stagger host refresh apart
  @param[in]  TrefiDelay              - Apply addition Trefi Delay when enable Host Refresh across different iMC Group
  @retval N/A

**/
VOID
SetRefreshEnable10nm (
  IN   PSYSHOST  Host,
  IN   UINT8     Socket,
  IN   UINT8     TrefiPerAviableChannel,
  IN   UINT8     NumberOfRank,
  IN   UINT16    TrefiDelay
  )
{
  UINT8                             Ch;
  CPGC_MISCCKECTL_MCDDC_CTL_STRUCT  MiscCKECtl[MAX_CH];
  struct channelNvram               (*ChannelNvList)[MAX_CH];
  UINT8                             MaxChDdr;
  BOOLEAN                           FirstActiveCh;
  UINT8                             PrevActiveCh = 0xFF;
  UINT64                            StartTime = 0;
  UINT64                            PrevStartTime = 0;
  UINT32                            TimeDifference;
  UINT32                            StartWriteTime[MAX_CH];
  UINT32                            HostRefreshStartTime[MAX_CH];
  SYS_SETUP                         *Setup;
  UINT8                             NumOfAvailableCh = 0;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  Setup = GetSysSetupPointer ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    NumOfAvailableCh ++;

    //
    // Cached refresh_enable setting
    //
    MiscCKECtl[Ch].Data = MemReadPciCfgEp (Socket, Ch, CPGC_MISCCKECTL_MCDDC_CTL_REG);
    MemWritePciCfgEp (Socket, Ch, CPGC_MISCCKECTL_MCDDC_CTL_REG, MiscCKECtl[Ch].Data);
    MiscCKECtl[Ch].Bits.refresh_enable = 1;
  }

  //
  // Initial array to 0
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    StartWriteTime[Ch] = 0;
    HostRefreshStartTime[Ch] = 0;
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    if (TrefiPerAviableChannel == 1) {
      HostRefreshStartTime[Ch] = REFRESH_SYNC_TIME / (NumberOfRank * NumOfAvailableCh) + REFRESH_SYNC_TIME;
    } else {
      HostRefreshStartTime[Ch] = REFRESH_SYNC_TIME;
    }
    RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "REFRESH_SYNC_TIME_PerCh= %d\n", HostRefreshStartTime[Ch]);
  } // Ch loop

  //
  // Apply additional Trefi delay when enable Host Refresh across different iMC 01/23 Group
  //
  if ((TrefiDelay > 0) && (MaxChDdr > FirstChanneliMc23)) {
    for (Ch = FirstChanneliMc23; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }
      HostRefreshStartTime[Ch] -= TrefiDelay;
      RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "FirstAvailableChBehindImc23 REFRESH_SYNC_TIME_PerCh= %d\n", HostRefreshStartTime[Ch]);
      break;
    }
  } else {
    //
    // To maintain TREFI apart, compensate the write latency in between previous active channel and current active channel
    // Formula will be REFRESH_SYNC_TIME + (write_delay[prev_active_channel] - write_delay[curr_active_channel]) / 2
    //
    FirstActiveCh = TRUE;
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }

      if (!FirstActiveCh) {
        //
        // While Multi-Socket MRC is enabled or SBPS, CSR write delay of each socket will be consumed directly
        //
        if ((Socket == 0) || ((Setup->mem.options & MULTI_THREAD_MRC_EN) == MULTI_THREAD_MRC_EN)) {
          if (Host->nvram.mem.socket[Socket].CsrWriteLatency[PrevActiveCh] >= Host->nvram.mem.socket[Socket].CsrWriteLatency[Ch]) {
            HostRefreshStartTime[Ch] = HostRefreshStartTime[Ch] + ((Host->nvram.mem.socket[Socket].CsrWriteLatency[PrevActiveCh] - Host->nvram.mem.socket[Socket].CsrWriteLatency[Ch]) / 2);
          } else {
            HostRefreshStartTime[Ch] = HostRefreshStartTime[Ch] - ((Host->nvram.mem.socket[Socket].CsrWriteLatency[Ch] - Host->nvram.mem.socket[Socket].CsrWriteLatency[PrevActiveCh]) / 2);
          }
        } else {
          //
          // For Single-Socket MRC Mode, consume CSR read delay instead of CSR write for remote sockets since CSR write will be Posted Write when executed code in SBSP
          //
          if (Host->nvram.mem.socket[Socket].CsrReadLatency[PrevActiveCh] >= Host->nvram.mem.socket[Socket].CsrReadLatency[Ch]) {
            HostRefreshStartTime[Ch] = HostRefreshStartTime[Ch] + ((Host->nvram.mem.socket[Socket].CsrReadLatency[PrevActiveCh] - Host->nvram.mem.socket[Socket].CsrReadLatency[Ch]) / 2);
          } else {
            HostRefreshStartTime[Ch] = HostRefreshStartTime[Ch] - ((Host->nvram.mem.socket[Socket].CsrReadLatency[Ch] - Host->nvram.mem.socket[Socket].CsrReadLatency[PrevActiveCh]) / 2);
          }
        }
      } else {
        FirstActiveCh = FALSE;
      }
      RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "HostRefreshStartTime= %d\n", HostRefreshStartTime[Ch]);
      PrevActiveCh = Ch;
    }
  }

  //
  // Enable periodic refresh engine
  //
  FirstActiveCh = TRUE;
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (((*ChannelNvList)[Ch].enabled == 1) || (((*ChannelNvList)[Ch].ddrtEnabled == 1) && ((*ChannelNvList)[Ch].mb3trainingfailure == 0))) {
      //
      // Inject delay between refresh enables
      //
      if(!FirstActiveCh) {
        do {
          StartTime = GetPerformanceCounter ();
          TimeDifference = TimeDiff (PrevStartTime, StartTime, TDIFF_UNIT_NS);
          if (!(GetEmulation () & SIMICS_FLAG)) {
            TimeDifference = HostRefreshStartTime[Ch];
          }
        } while (TimeDifference < HostRefreshStartTime[Ch]);
      } else {
        StartTime = GetPerformanceCounter ();
      }

      MemWritePciCfgEp (Socket, Ch, CPGC_MISCCKECTL_MCDDC_CTL_REG, MiscCKECtl[Ch].Data);

      //
      // Log times since previous refresh enable
      //
      if(!FirstActiveCh) {
        StartWriteTime[Ch] = TimeDiff (PrevStartTime, StartTime, TDIFF_UNIT_NS);
      } else {
        FirstActiveCh = FALSE;
      }
      //
      // Save the current start and end times
      //
      PrevStartTime = StartTime;
    }
  } // Ch loop

  //
  // Print the timing details between mc host refresh enables
  //
  FirstActiveCh = TRUE;
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (((*ChannelNvList)[Ch].enabled == 1) || (((*ChannelNvList)[Ch].ddrtEnabled == 1) && ((*ChannelNvList)[Ch].mb3trainingfailure == 0))) {
      if (FirstActiveCh) {
        FirstActiveCh = FALSE;
      } else {
        RcDebugPrint (SDBG_MINMAX, "EnableHostRefresh Start Write Time diff[%d]=%d ns\n", Ch, StartWriteTime[Ch]);
      }
    }
  } // Priting host refresh enable timing details
}

/**

  This function needs to immediately follow the
  SetRefreshEnable10nm() funtion.  This provides an end label that C
  can use to read the entire function (ExitSelfRefresh10nm) into
  cache before executing it.

**/
UINT8
SetRefreshEnable10nmEndMarker (
  VOID
  )
{
  UINT8 Status;

  Status = 0;
  return Status;
}

/**

This function asserts CKE on each channel and unmasks periodic refresh

@param Host    - Pointer to sysHost
@param socket  - Socket

@retval none

**/
VOID
ExitSelfRefreshCriticalSection (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  UINT8                             Ch;
  CPGC_MISCCKECTL_MCDDC_CTL_STRUCT  MiscCKECtl;
  CPGC_MISCCKECTL_MCDDC_CTL_STRUCT  MiscCKECtl1[MAX_CH];
  CPGC_MISCREFCTL_MCDDC_CTL_STRUCT  MiscRefCtl;
  struct channelNvram               (*ChannelNvList)[MAX_CH];
  struct dimmNvram                  (*DimmNvList)[MAX_DIMM];
  UINT8                             MaxChDdr;
  BOOLEAN                           FirstActiveCh;
  UINT8                             PrevActiveCh = 0xFF;
  UINT64                            StartTime = 0;
  UINT64                            PrevStartTime = 0;
  UINT32                            TimeDifference;
  UINT32                            StartWriteTime[MAX_CH];
  UINT32                            HostRefreshStartTime[MAX_CH];
  SYS_SETUP                         *Setup;
  UINT8                              NumOfAvailableCh = 0;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, socket);
  Setup = GetSysSetupPointer ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    NumOfAvailableCh ++;

    //
    // Cached refresh_enable setting
    //
    MiscCKECtl1[Ch].Data = MemReadPciCfgEp (socket, Ch, CPGC_MISCCKECTL_MCDDC_CTL_REG);
    MemWritePciCfgEp (socket, Ch, CPGC_MISCCKECTL_MCDDC_CTL_REG, MiscCKECtl1[Ch].Data);
    MiscCKECtl1[Ch].Bits.refresh_enable = 1;
  }

  //
  // Initial array to 0
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    StartWriteTime[Ch] = 0;
    HostRefreshStartTime[Ch] = 0;
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    if (Setup->mem.TrefiPerChannel == 1) {
      HostRefreshStartTime[Ch] = REFRESH_SYNC_TIME / (Setup->mem.TrefiNumofRank * NumOfAvailableCh) + REFRESH_SYNC_TIME;
    } else {
      HostRefreshStartTime[Ch] = REFRESH_SYNC_TIME;
    }
    RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "REFRESH_SYNC_TIME_PerCh= %d\n", HostRefreshStartTime[Ch]);
  } // Ch loop

  //
  // Apply additional Trefi delay when enable Host Refresh across different iMC 01/23 Group
  //
  if ((Setup->mem.TrefiDelay > 0) && (MaxChDdr > FirstChanneliMc23)) {
    for (Ch = FirstChanneliMc23; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }
      HostRefreshStartTime[Ch] -= Setup->mem.TrefiDelay;
      RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "FirstAvailableChBehindImc23 REFRESH_SYNC_TIME_PerCh= %d\n", HostRefreshStartTime[Ch]);
      break;
    }
  } else {
    //
    // To maintain TREFI apart, compensate the write latency in between previous active channel and current active channel
    // Formula will be REFRESH_SYNC_TIME + (write_delay[prev_active_channel] - write_delay[curr_active_channel]) / 2
    //
    FirstActiveCh = TRUE;
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }

      if (!FirstActiveCh) {
        //
        // While Multi-Socket MRC is enabled or SBPS, CSR write delay of each socket will be consumed directly
        //
        if ((socket == 0) || ((Setup->mem.options & MULTI_THREAD_MRC_EN) == MULTI_THREAD_MRC_EN)) {
          if (Host->nvram.mem.socket[socket].CsrWriteLatency[PrevActiveCh] >= Host->nvram.mem.socket[socket].CsrWriteLatency[Ch]) {
            HostRefreshStartTime[Ch] = REFRESH_SYNC_TIME + ((Host->nvram.mem.socket[socket].CsrWriteLatency[PrevActiveCh] - Host->nvram.mem.socket[socket].CsrWriteLatency[Ch]) / 2);
          } else {
            HostRefreshStartTime[Ch] = REFRESH_SYNC_TIME - ((Host->nvram.mem.socket[socket].CsrWriteLatency[Ch] - Host->nvram.mem.socket[socket].CsrWriteLatency[PrevActiveCh]) / 2);
          }
        } else {
          //
          // For Single-Socket MRC Mode, consume CSR read delay instead of CSR write for remote sockets since CSR write will be Posted Write when executed code in SBSP
          //
          if (Host->nvram.mem.socket[socket].CsrReadLatency[PrevActiveCh] >= Host->nvram.mem.socket[socket].CsrReadLatency[Ch]) {
            HostRefreshStartTime[Ch] = REFRESH_SYNC_TIME + ((Host->nvram.mem.socket[socket].CsrReadLatency[PrevActiveCh] - Host->nvram.mem.socket[socket].CsrReadLatency[Ch]) / 2);
          } else {
            HostRefreshStartTime[Ch] = REFRESH_SYNC_TIME - ((Host->nvram.mem.socket[socket].CsrReadLatency[Ch] - Host->nvram.mem.socket[socket].CsrReadLatency[PrevActiveCh]) / 2);
          }
        }
      } else {
        FirstActiveCh = FALSE;
      }
      RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "HostRefreshStartTime= %d\n", HostRefreshStartTime[Ch]);
      PrevActiveCh = Ch;
    }
  }
  //
  // Enable periodic refresh engine
  //
  FirstActiveCh = TRUE;
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (((*ChannelNvList)[Ch].enabled == 1) || (((*ChannelNvList)[Ch].ddrtEnabled == 1) && ((*ChannelNvList)[Ch].mb3trainingfailure == 0))) {
      //
      // Inject delay between refresh enables
      //
      if(!FirstActiveCh) {
        do {
          StartTime = GetPerformanceCounter ();
          TimeDifference = TimeDiff (PrevStartTime, StartTime, TDIFF_UNIT_NS);
          if (!(GetEmulation () & SIMICS_FLAG)) {
            TimeDifference = HostRefreshStartTime[Ch];
          }
        } while (TimeDifference < HostRefreshStartTime[Ch]);
      } else {
        StartTime = GetPerformanceCounter ();
      }

      MemWritePciCfgEp (socket, Ch, CPGC_MISCCKECTL_MCDDC_CTL_REG, MiscCKECtl1[Ch].Data);

      //
      // Log times since previous refresh enable
      //
      if(!FirstActiveCh) {
        StartWriteTime[Ch] = TimeDiff (PrevStartTime, StartTime, TDIFF_UNIT_NS);
      } else {
        FirstActiveCh = FALSE;
      }
      //
      // Save the current start and end times
      //
      PrevStartTime = StartTime;
    }
  } // Ch loop

  //
  // Print the timing details between mc host refresh enables
  //
  FirstActiveCh = TRUE;
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (((*ChannelNvList)[Ch].enabled == 1) || (((*ChannelNvList)[Ch].ddrtEnabled == 1) && ((*ChannelNvList)[Ch].mb3trainingfailure == 0))) {
      if (FirstActiveCh) {
        FirstActiveCh = FALSE;
      } else {
        RcDebugPrint (SDBG_MINMAX, "EnableHostRefresh Start Write Time diff[%d]=%d ns\n", Ch, StartWriteTime[Ch]);
      }
    }
  } // Priting host refresh enable timing details

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, socket, Ch);

    // Set up CKE overrides
    MiscCKECtl.Data = MemReadPciCfgEp (socket, Ch, CPGC_MISCCKECTL_MCDDC_CTL_REG);
    MiscCKECtl.Bits.cke_override |= 0xFF;
    MiscCKECtl.Bits.cke_on |= 0xFF;

    // Clear refresh mask
    MiscRefCtl.Data = MemReadPciCfgEp (socket, Ch, CPGC_MISCREFCTL_MCDDC_CTL_REG);
    MiscRefCtl.Bits.ref_rankmask = 0;

    // Exit Self-refresh state by taking CKE high
    MemWritePciCfgEp (socket, Ch, CPGC_MISCCKECTL_MCDDC_CTL_REG, MiscCKECtl.Data);

    //
    // txS = Max (5nCK, tRFC_sir(min) + 10ns)
    //       8Gb tRFC_sir(min) = 350ns
    //       16Gb tRFC_sir(min) = 550ns
    //
    if (((*DimmNvList)[0].sdramCapacity == SPD_16Gb) || ((*DimmNvList)[1].sdramCapacity == SPD_16Gb)) {
      NanoSecondDelay (560); //16Gb DIMMs
    } else {
      NanoSecondDelay (360); //8Gb DIMMs
    }

    // Begin periodic auto-refresh (timing critical)
    // Note that one refresh command should already be queued and will occur immediately
    MemWritePciCfgEp (socket, Ch, CPGC_MISCREFCTL_MCDDC_CTL_REG, MiscRefCtl.Data);

  } // Ch loop

  //
  // Set Panic/High WM = 2/1 when 16Gb DIMM is present on channel
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    ProgramRefreshTimingsWaterMark (socket, Ch);
  } // Ch loop

  //
  // Indicate that Host periodic auto-refresh is now enabled
  //
  Host->var.mem.socket[socket].hostRefreshStatus = 1;
}

/**

  This function needs to immediately follow the
  ExitSelfRefresh10nm() funtion.  This provides an end label that C
  can use to read the entire function (ExitSelfRefresh10nm) into
  cache before executing it.

**/
UINT8
ExitSelfRefreshCriticalSectionEndMarker (
  VOID
  )
{
  UINT8 status;
  status = 0;
  return status;
}


/**

  Return address of CPGC_ERR_DATA0_S_MCDDC_CTL_REG

  @param Host           - Point to sysHost
  @param Socket         - Socket number
  @param Ch             - Channel number

  @retval  Physical address of CPGC_ERR_DATA0_S_MCDDC_CTL_REG

**/
UINT8 *
GetCpgcErrData0sAddr (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
)
{
  CSR_OFFSET RegOffset;
  UINT8      Size;

  RegOffset.Data = CPGC_ERR_DATA0_S_MCDDC_CTL_REG;
  Size = (UINT8) RegOffset.Bits.size;
  return (UINT8 *) UsraGetCsrRegisterAddress (Socket, Ch, RegOffset.Data);
}

/** Return whether a given channel has hit a DDRT Read Pending Queue timeout.

  @param[in] Host     Pointer to sysHost
  @param[in] Socket   Processor socket within the system (0-based)
  @param[in] Channel  Number of the channel to check

  @retval FALSE The timeout has not been hit.
  @retval TRUE  The timeout has been hit.

**/
BOOLEAN
EFIAPI
DdrtReadPendingQueueTimedOut (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  return Host->var.mem.DdrtReadPendingQueueTimedOut[Socket][Channel];
}

/** Set whether a given channel has hit a DDRT Read Pending Queue timeout.

  @param[in] Host     Pointer to sysHost
  @param[in] Socket   Processor socket within the system (0-based)
  @param[in] Channel  Number of the channel to check
  @param[in] TimedOut Whether or not the channel has hit a Read Pending Queue timeout.

**/
VOID
SetDdrtReadPendingQueueTimedOut (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  BOOLEAN   TimedOut
  )
{
  Host->var.mem.DdrtReadPendingQueueTimedOut[Socket][Channel] = TimedOut;
}

/** Set the DDRT Read Pending Queue (RPQ) credit limit with the default value.

  If implemented with a non-null library, the credit limit is set for all
  channels with a DDRT DIMM.  However, this should only be implemented with a
  non-null library for projects that need to alternately set and clear the
  credit limit for specific training steps.

  @param[in] Host         Pointer to Host structure
  @param[in] Socket       Processor socket within the system (0-based)

  @retval N/A

**/
VOID
EFIAPI
SetDefaultCpgcRpqCreditLimitIfNeeded (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  // Do nothing. (No functionality needed.)
}

/** Clear the DDRT Read Pending Queue (RPQ) credit limit.

  If implemented with a non-null library, the credit limit is cleared for all
  channels with a DDRT DIMM.  However, this should only be implemented with a
  non-null library for projects that need to alternately set and clear the
  credit limit for specific training steps.

  @param[in] Host     Pointer to Host structure
  @param[in] Socket   Processor socket within the system (0-based)

  @retval N/A

**/
VOID
EFIAPI
ClearCpgcRpqCreditLimitIfNeeded (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  // Do nothing. (No functionality needed.)
}

/**

  Finds command margins for the given command group

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Socket number
  @param[in] ChBitmask    - Channel bit mask
  @param[in] SubCh        - Sub channel number
  @retval N/A

**/
VOID
ExecuteCmdSignalTest (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT32    ChBitmask,
  IN  UINT8     SubCh
  )
{

  //
  // Start test
  //
  CpgcGlobalTestStart (Host, Socket, ChBitmask, SubCh);

  //
  // Poll test completion
  //
  CpgcPollGlobalTestDone (Host, Socket, ChBitmask, SubCh);

  //
  // Delay if this is a CTE build, otherwise do nothing.
  //

  CteDelayDclk (10);

} // ExecuteCmdSignalTest

/**
This function is used to start a test, stop a test, and clear errors.
This function does not assume that all tests are CPGC tests (i.e. WL
fine test, which is executed in the IO only). This function will use
the last Level and TestType parameters given by SetupTest for the channels.
  @param[in] Host:           Pointer to sysHost
  @param[in] Socket:         Processor socket within the system (0-based)
  @param[in] MemSs           MemSS number within the socket (0-based)
  @param[in] ChannelMask:    Channel mask within the MemSS
  @param[in] SubCh:          Sub channel number
  @param[in] TestType:       Indicates the test type to be configured
  @param[in] ClearErrors:    Clear the error status
  @param[in] StartTest:      Start the test: 1: start, 0: not start.
  @param[in] StopTest:       Stop the test: 1: stop, 0: not stop.

  @retval MRC_STATUS

**/
MRC_STATUS
TestControl (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN UINT32        MemSs,
  IN UINT32        ChannelMask,
  IN UINT8         SubCh,
  IN MRC_TT        TestType,
  IN BOOLEAN       ClearErrors,
  IN BOOLEAN       StartTest,
  IN BOOLEAN       StopTest
  )
{
  UINT32 Status = SUCCESS;
  UINT8  DumArr[7] = {1,1,1,1,1,1,1};

  if (StartTest) {
    switch (TestType) {
    case  RcvenFineTest:
    case  WrLvlCoarseTest:
#ifdef DDR5_SUPPORT
    case  WLDdr5:
    case  DqSwzDiscTestDdr5:
    case  WrDqTest:
    case  DqSlewRateTest:
#endif
      Status = RunIOTest (Host, Socket, ChannelMask, BasicVA, DumArr, ClearErrors, 0);
      break;
    case EarlyCsLoopbackTest:
    case EarlyCaLoopbackTest:
    case EarlyCaBacksideLoopbackTest:
    case BcomTrainingTest:
    default:
      FifoTrainReset (Host, Socket);
      Status = CpgcAdvTrainingStartTest (Host, Socket, ChannelMask, 0, SubCh);
      break;
    }

  }
  if (StopTest) {
    Status = CpgcGlobalTestStop (Host, Socket, ChannelMask, SubCh);
  }
  return Status;
}

/**

  Set minimum good samples for receive enable feedback

  @param Host         - Pointer to sysHost, the system Host (root) structure

  @retval UINT8       - The number of minimum good samples

**/
UINT8
SetMinGoodInit (
                PSYSHOST Host
)
{
  return (UINT8)(1 << (REC_EN_LOOP_COUNT - 1));
} // SetMinGoodInit

/**

Setup Receive Enable fine parameter

@param[out] CPGCAddress  - pointer for CPGC address array
@param[out] WDBPattern   - pointer for WDB Pattern
@param[out] LoopCount    - pointer for Number of cycles
@param[out] NumCl        - pointer for Number of cache lines
@param[out] BurstWait    - pointer for Number of DCLK spacing between bursts

@retval None

**/
VOID
EFIAPI
SetupRecEnFineParam (
  OUT TCPGCAddress *CPGCAddress,
  OUT TWDBPattern  *WDBPattern,
  OUT UINT8        *LoopCount,
  OUT UINT8        *NumCl,
  OUT UINT8        *BurstWait
  )
{

  //                                      Rank, Bank, Row, Col
  TCPGCAddress        RecEnCpgcAddress = { { 0, 0, 0, 0 },     // Start
                                           { 0, 4, 0, 1023 },  // Stop
                                           { 0, 0, 0, 0 },     // Order
                                           { 0, 0, 0, 0 },     // IncRate
                                           { 0, 4, 0, 1 } };   // IncValue
  //                                     IncRate, Start, Stop, DQPat
  TWDBPattern         RecEnWdbPattern = { 32, 0, 9, BasicVA }; // Should not matter what this is. We do not compare data.

  CopyMem (CPGCAddress, &RecEnCpgcAddress, sizeof(TCPGCAddress));
  CopyMem (WDBPattern, &RecEnWdbPattern, sizeof(TWDBPattern));

  SetupRecEnFineParamTarget (LoopCount, NumCl, BurstWait);

} // SetupRecEnFineParam

/**
  Executes an based memory test for advance trainings

  @param Host:         Pointer to sysHost
  @param Socket:       Processor socket to check
  @param DdrChEnMap    Bit-mask of enabled channels to test. 1 = Enabled, 0 = Skipped
  @param Dimm:         Target DIMM to test
  @param Rank:         Target Rank to test
  @param Group:        Parameter to be margined
                         RxDqsDelay    : Margin Rx DqDqs
                         TxDqDelay     : Margin Tx DqDqs
                         RxVref         : Margin Rx Vref
                         TxVref         : Margin Tx Vref
                         RecEnDelay    : Margin Receive Enable
                         WrLvlDelay    : Margin Write Level
                         CmdGrp0        : Margin CMD group 0
                         CmdGrp1        : Margin CMD group 1
                         CmdGrp2        : Margin CMD group 2
                         CmdAll         : Margin all CMD groups
  @param Mode:         Margin mode to use
                         MODE_XTALK            BIT0: Enable crosstalk (placeholder)
                         MODE_VIC_AGG          BIT1: Enable victim/aggressor
                         MODE_START_ZERO       BIT2: Start at margin 0
                         MODE_POS_ONLY         BIT3: Margin only the positive side
                         MODE_NEG_ONLY         BIT4: Margin only the negative side
                         MODE_DATA_MPR         BIT5: Enable MPR Data pattern
                         MODE_DATA_FIXED       BIT6: Enable Fixed Data pattern
                         MODE_DATA_LFSR        BIT7: Enable Data LFSR
                         MODE_ADDR_LFSR        BIT8: Enable Address LFSR
                         MODE_ADDR_CMD0_SETUP  BIT9
                         MODE_CHECK_PARITY     BIT10: Enable parity checking
                         MODE_DESELECT         BIT11: Enable deselect patterns
                         MODE_VA_DESELECT      BIT12: Enable Victim - Aggressor deselect patterns

  @param PatternLength:  Number of sequences to test (units of burstLength cache lines)
  @param BurstLength:    Number of cachelines to test for each write/read operation
                         It is the repeat count for the WR/RD subsequence

  @retval FAILURE: CPGC testing was unsuccessful.
  @retval SUCCESS: CPGC testing was successful or channel was not enabled.

**/
UINT32
ExecuteTest (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT32    DdrChEnMap,
  UINT8     Dimm,
  UINT8     Rank,
  MRC_GT    Group,              // Don't care since CPGC runs only dq pattern
  UINT16    Mode,
  UINT32    PatternLength,
  UINT8     BurstLength
  )
{
  UINT8 LogRank = 0;
  UINT8 LogSubRank = 0;
  //UINT8 vaLoopMax;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT8 Ch;
  UINT32 Status = SUCCESS;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  if (DdrChEnMap != 0) {

    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((DdrChEnMap & (1 << Ch)) == 0) {
        continue;
      }
      // Convert physical DIMM/physical rank into VMSE logical rank
      LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
      if ((*ChannelNvList)[Ch].encodedCSMode) {
        LogSubRank = (Rank >> 1);
      }
      if (CheckCMDGroup (Group) || CheckCTLGroup (Group)) {
        CpgcAdvTrainingSetup (Host, Socket, (1 << Ch), Dimm, LogRank, 0,  DDR_CPGC_ADV_CMD_PATTERN, PatternLengthToCpgcLoopCnt (PatternLength));
        RcDebugPrintWithDevice (SDBG_CPGC, NO_SOCKET, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                 "CMD/CTL patternlength = %d\n",PatternLengthToCpgcLoopCnt(PatternLength));
      } else {
        RcDebugPrintWithDevice (SDBG_CPGC, NO_SOCKET, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                 "RMT patternlength = %d\n",PatternLengthToCpgcLoopCnt(PatternLength) );
        CpgcAdvTrainingSetup (Host, Socket, (1 << Ch), Dimm, LogRank, LogSubRank,  DDR_CPGC_DQ_PATTERN, PatternLengthToCpgcLoopCnt (PatternLength));
      } // if check CMD CTL grp
    } //ch

    //
    // Set up the CADB if necessary
    //
    if (Mode & (MODE_DESELECT | MODE_VA_DESELECT)) {
      EnableCADBDeselect (Host, Socket, DdrChEnMap, Dimm, Mode);
    }

    // Setup for Victim/Aggressor
    /*
    if (Mode & MODE_VIC_AGG) {
      // Loop for 8 victim bits
      vaLoopMax = 8;
    } else if (mode & MODE_DATA_LFSR) {
      // Loop count depends on the WDB range. We always start from 0 so it really depends on the end point.
      vaLoopMax = WDB_END + 1;     // Use default only (wdbEnd) = 7
    } else {
      // Only one loop
      vaLoopMax = 1;
    }*/

    // Run CPGC training test for the selected channel(s)
    Status = CpgcAdvTrainingExecTestGroup (Host, Socket, DdrChEnMap, 0);
  }

  return Status;
} // ExecuteTest

/**
  Set up the memory training or memory test.

  @param[in] Host:           Pointer to sysHost
  @param[in] Socket:         Processor socket within the system (0-based)
  @param[in] MemSs:          MemSS number within the socket (0-based)
  @param[in] ChannelMask:    Channel mask within the MemSS
  @param[in] RankListSize:   Number of items in RankList array
  @param[in] *RankList:      List of ranks, subranks to operate on
  @param[in] Level:          Provides context for LRDIMM buffer back-side vs. Host-side test
  @param[in] TestType:       Indicates the test type to be configured
  @param[in] TargetSignal:   Required for some early command training pattern test types
  @param[in] SeqCnt:         Optional to override default loopcount for given test type
                         This setting impacts the number of times the sequence repeats.
                         This is a linear value. This does not impact number of cachelines
                         per subsequence.  Value of 0 means "infinite" setting.
  @param[in] Patternloop:    Pattern loop

  @retval MRC_STATUS

**/
MRC_STATUS
SetupTest (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN MRC_MST     MemSs,
  IN UINT32      ChannelMask,
  IN UINT32      RankListSize,
  IN MRC_RT      *RankList,
  IN MRC_LT      Level,
  IN MRC_TT      TestType,
  IN MRC_CSN     TargetSignal,
  IN UINT32      SeqCnt,
  IN UINT8       Patternloop
  )
{
  UINT32                           Status = SUCCESS;
  UINT8                            BurstWait;
  TCPGCAddress                     CpgcAddress;
  TWDBPattern                      WdbPattern;
  UINT8                            LoopCount;
  UINT8                            NumCl;
  UINT8                            MaxChDdr;
#ifdef DDR5_SUPPORT
  UINT8                            Dimm = 0;
  UINT8                            Rank = 0;
  UINT8                            Ch;
  struct TrainingVariable          *TrainingVar;
  CPGC_GLOBALCTRL_MC_GLOBAL_STRUCT CpgcGlobalCtrlReg;

  if (RankList != NULL) {
    RC_FATAL_ERROR ((RankListSize == 1), ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_209);
    Dimm = (RankList + RankListSize - 1)->Dimm;
    Rank = (RankList + RankListSize - 1)->Rank;
  }
#endif
  MaxChDdr = GetMaxChDdr ();
  if (ChannelMask != 0) {
    switch (TestType) {
#ifdef DDR5_SUPPORT
      case EarlyCsLoopbackTest:
        Status = CpgcCsClkPattern (Host, Socket, ChannelMask, TargetSignal);
        return Status;
      case EarlyCaBacksideLoopbackTest:
        Status = CpgcQcaClkPattern (Host, Socket, ChannelMask, GetPhyRank(Dimm, Rank), 0, TargetSignal, NormalCadbSetup);
        return Status;
      case EarlyCsBacksideLoopbackTest:
        Status = CpgcQcsClkPattern (Host, Socket, ChannelMask, Dimm, Rank);
        return Status;
      case EarlyCaLoopbackTest:
        Status = CpgcCaClkPattern (Host, Socket, ChannelMask, GetPhyRank (Dimm, Rank), 0, TargetSignal, NormalCadbSetup, FIRST_ITERATION_LOOP);
        return Status;
      case WLDdr5:
        SetupWriteLevellingParamDdr5 (Host, &CpgcAddress, &WdbPattern, &LoopCount, &NumCl, &BurstWait);
        //
        // The sequence doesn't matter.The wdb pattern also doesn't matter.Inside this function, it will setup the test for all the channels.
        //
        SetupIOTest (Host, Socket, ChannelMask, PatWr, NumCl, LoopCount, &CpgcAddress, NSOE, &WdbPattern, 0, 0, BurstWait);
        SetupChipPatternSequencer (Host, Socket, ChannelMask, Dimm, Rank);
        return Status;
      case  DqSwzDiscTestDdr5:
        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          if ((ChannelMask & (1 << Ch)) == 0) {
            continue;
          }
          CpgcGlobalCtrlReg.Data = MemReadPciCfgEp (Socket, Ch, CPGC_GLOBALCTRL_MC_GLOBAL_REG);
          CpgcGlobalCtrlReg.Bits.use_bl4_col_addr = 1;
          MemWritePciCfgEp (Socket, Ch, CPGC_GLOBALCTRL_MC_GLOBAL_REG, CpgcGlobalCtrlReg.Data);
        }
        SetupDqSwizzleDiscoveryParamDdr5 (Host, Socket, &CpgcAddress, &WdbPattern, &LoopCount, &NumCl, &BurstWait);
        SetupIOTest (Host, Socket, ChannelMask, PatRd, NumCl, LoopCount, &CpgcAddress, NSOE, &WdbPattern, 0, 0, BurstWait);
        SetCpgcPatControl (Host, Socket, ChannelMask, BTBUFFER, BTBUFFER, BTBUFFER, 0, 0, 0);
        TrainingVar = MemTrainingVar ();
        TrainingVar->DQPatLC = 1;
        TrainingVar->DQPat = BasicVA;
        return Status;
      case BcomTrainingTest:
        Status = CpgcBcomClkPattern (Host, Socket, ChannelMask, GetPhyRank (Dimm, Rank), TargetSignal);
        return Status;
#endif
    case WrLvlCoarseTest:
      SetupWriteLevellingCoarseParam (Host, &CpgcAddress, &WdbPattern, &LoopCount, &NumCl, &BurstWait);
      SetupIOTest (Host, Socket, ChannelMask, PatWrRdFlyb, NumCl, LoopCount, &CpgcAddress, NSOE, &WdbPattern, 0, 0, BurstWait);
      SetupChipPatternSequencer (Host, Socket, ChannelMask, RankList->Dimm, RankList->Rank);
      break;
    case RcvenFineTest:
      SetupRecEnFineParam (&CpgcAddress, &WdbPattern, &LoopCount, &NumCl, &BurstWait);
      // CmdPat = PatRd, NumCL = 8, LC = 7, CPGCAddress, SOE = 0,
      // WDBPattern, EnCADB = 0, EnCKE = 0, SubSeqWait = 8
      SetupIOTest (Host, Socket, ChannelMask, PatRd, NumCl, LoopCount, &CpgcAddress, NSOE, &WdbPattern, 0, 0, BurstWait);
      return Status;
    default:
      break;
    }
  }

  return Status;
} // setuptest

/**
Modify memory training or memory test set up by the setuptest function.
  @param Host:           Pointer to sysHost
  @param Socket:         Processor socket within the system (0-based)
  @param MemSs           MemSS number within the socket (0-based)
  @param ChannelMask:    Channel mask within the MemSS
  @param RankListSize:   Number of items in RankList array
  @param *RankList:      List of ranks, subranks to operate on
  @param TestType:       Indicates the test type to be configured

@retval MRC_STATUS

**/
MRC_STATUS
ModifyTest (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN UINT32        MemSs,
  IN UINT32        ChannelMask,
  IN UINT32        RankListSize,
  IN MRC_RT        *RankList,
  IN MRC_TT        TestType
  )
{
  UINT8    Ch;
  UINT8    Dimm;
  UINT8    Rank;
  UINT8    LogRank = 0;
  UINT8    LogSubRank = 0;
  struct   channelNvram (*ChannelNvList)[MAX_CH];
  UINT32   Status = SUCCESS;
  UINT8    MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  Dimm = (RankList + RankListSize - 1)->Dimm;
  Rank = (RankList + RankListSize - 1)->Rank;

  if (ChannelMask != 0) {
    // Setup CPGC training pattern for specific group for all channels (parallel)
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((ChannelMask & (1 << Ch)) == 0) {
        continue;
      }

      //
      // Convert physical DIMM/physical rank into logical rank
      //
      LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
      if ((*ChannelNvList)[Ch].encodedCSMode) {
        LogSubRank = (Rank >> 1);
      }

      if (TestType == LateCmdTest) {
        CpgcAdvTrainingModifyRank (Host, Socket, (1 << Ch), Dimm, LogRank, 0, DDR_CPGC_ADV_CMD_PATTERN);
      } else {
        CpgcAdvTrainingModifyRank (Host, Socket, (1 << Ch), Dimm, LogRank, LogSubRank, DDR_CPGC_DQ_PATTERN);
      }
    } //ch
  }
  return Status;
}

/**

  This function is used to get the lane/bit test status for BSSA test.

  If there are multiple SubChannels inside a Channel, then the LaneErroStatus
  of SubChannels are concatenated.

  For DDR5:
  SUB_CH0 32bits lane error status are stored to the LaneErrorStatus[3:0]
  SUB_CH0 8bits ECC lane error status are stored to the LaneErorStatus[4]
  SUB_CH1 32bits lane error status are stored to the LaneErrorStatus[8:5]
  SUB_CH1 8bits ECC lane error status are storeed to the LaneErrorStatus[9]

  @param[in] Host:            Pointer to sysHost
  @param[in] Socket:          Processor socket within the system (0-based)
  @param[in] MemSs            MemSS type within the socket
  @param[in] Channel          Channel number within socket (0-based)
  @param[in] TestType         Indicates the test type
  @param[in] ChunkErrSel      Select the chunck/UIs of the cache line to be checked
  @param[in] Size             The size of the Status array
  @param[out] LaneErrorStatus Pointer to array where the lane error status values will be stored.
                              The number of array elements is (MAX_BITS / 8).
                              Bit value 1 = at least one error occurred; bit value 0 = no errors occurred.

  @retval MRC_STATUS_SUCCESS, MRC_STATUS_FAILURE, MRC_FUNCTION_NOT_IMPLEMENTED
**/
MRC_STATUS
EFIAPI
GetCpgcLaneErrorStatus (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      MRC_MST   MemSs,
  IN      UINT8     Channel,
  IN      MRC_TT    TestType,
  IN      MRC_CT    ChunkErrSel,
  IN      UINT8     Size,
  OUT     UINT8     *LaneErrorStatus
  )
{
  CPGC_ERROR_STATUS     CpgcErrorStatus[MAX_CH];
  UINT32                BwSerr[3];
  UINT8                 i;
  UINT8                 FaultyStrobe;
  UINT32                FaultyPartsFlag;
  CPGC_ERID_LFSR_ERR_MC_2LM_STRUCT  CpgcEridLfsrErr;

  ZeroMem (CpgcErrorStatus, sizeof (CpgcErrorStatus));
  ZeroMem (LaneErrorStatus, Size);
  ZeroMem (BwSerr, sizeof (BwSerr));

  if (!IsChannelEnabled (Socket, Channel)) {
    return MRC_STATUS_FAILURE;
  }

  //
  // EIRD test result is different from general CPGC test.
  //
  if (TestType == EarlyEridTest) {

      CpgcEridLfsrErr.Data = MemReadPciCfgEp (Socket, Channel, CPGC_ERID_LFSR_ERR_MC_2LM_REG);

      //
      // Convert the error status back to lane status
      // even and odd bits correspond to ERID0 and 1
      //
      if (CpgcEridLfsrErr.Bits.status & ERID_BIT0_MASK) {
        BwSerr[0] |= BIT0;
      }
      if (CpgcEridLfsrErr.Bits.status & ERID_BIT1_MASK) {
        BwSerr[0] |= BIT1;
      }

  } else {
    //
    // Collect CPGC raw test results
    //
    CpgcAdvTrainingErrorStatus (Host, Socket, 0x1 << Channel, CpgcErrorStatus);

    if ((ChunkErrSel == Even) || (ChunkErrSel == All)) {
      BwSerr[0] |= CpgcErrorStatus[Channel].cpgcErrDat0S;
      BwSerr[1] |= CpgcErrorStatus[Channel].cpgcErrDat1S;
    }
    if ((ChunkErrSel == Odd) || (ChunkErrSel == All)) {
      BwSerr[0] |= CpgcErrorStatus[Channel].cpgcErrDat2S;
      BwSerr[1] |= CpgcErrorStatus[Channel].cpgcErrDat3S;
    }

    if (Host->nvram.mem.eccEn) {
      if ((ChunkErrSel == Even) || (ChunkErrSel == All)) {
        BwSerr[2] |= (CpgcErrorStatus[Channel].cpgcErrEccS & 0xFF);
      }
      if ((ChunkErrSel == Odd) || (ChunkErrSel == All)) {
        BwSerr[2] |= ((CpgcErrorStatus[Channel].cpgcErrEccS >> 8) & 0xFF);
      }
    }

    //
    // Take care of faulty part
    //
    if (Host->nvram.mem.socket[Socket].FaultyPartsFlag[Channel] != 0) {
      FaultyPartsFlag = Host->nvram.mem.socket[Socket].FaultyPartsFlag[Channel];
      FaultyStrobe = 0;
      do {
        if ((FaultyPartsFlag >> FaultyStrobe) & BIT0) {
          break;
        } else {
          FaultyStrobe++;
        }
      } while (FaultyPartsFlag != 0);

      if ((FaultyStrobe % 9) < 4) {
        //Override bwserr0 when error happen in strobe 0/1/2/3, 9/10/11/12
        BwSerr[0] &= ~(0xFF << ((FaultyStrobe % 9) * 8));
      } else if (((FaultyStrobe % 9) >= 4) && ((FaultyStrobe % 9) < 8)) {
        // Override bwserr1 when error happen in strobe 4/5/6/7, 13/14/15/16
        BwSerr[1] &= ~(0xFF << (((FaultyStrobe - 4) % 9) * 8));
      } else {
        // override bwserr2 when error happen in strobe 8/17
        BwSerr[2] &= ~(0xFF);
      }
    }

    //
    // Indicate a failure on all available bits if the RPQ had a timeout on this channel,
    //
    if (DdrtReadPendingQueueTimedOut (Host, Socket, Channel)) {
      BwSerr[0] = (UINT32)-1;
      BwSerr[1] = (UINT32)-1;
      BwSerr[2] = (UINT32)-1;
    }
  } // ! (Host->CurrentTestType == EarlyEridTest)


  for (i = 0; i < Size; i++) {
    if (i < 4) {
      *(LaneErrorStatus + i) = (UINT8)((BwSerr[0] >> i * 8) & 0xFF);
    } else if (i < 8) {
      *(LaneErrorStatus + i) = (UINT8)((BwSerr[1] >> (i - 4) * 8) & 0xFF);
    } else {
      *(LaneErrorStatus + i) = (UINT8) (BwSerr[2] & 0xFF);
    }
  } // i

  return  MRC_STATUS_SUCCESS;

}
/**

  This function is used to get the CPGC error counter values for BSSA test.

  If there are multiple SubChannels inside a Channel, then the error count
  of SubChannels are added together.

  @param[in] Host:            Pointer to sysHost
  @param[in] Socket:          Processor socket within the system (0-based)
  @param[in] MemSsType        MemSS type within the socket
  @param[in] Channel          Channel number within socket (0-based)
  @param[in] TestType         Indicates the test type
  @param[in] Counter,         Counter ID number
  @param[in,out] CountStatus  Pointer to the erorr count status

  @retval MRC_STATUS_SUCCESS, MRC_STATUS_FAILURE, MRC_FUNCTION_NOT_IMPLEMENTED

**/
MRC_STATUS
EFIAPI
GetErrorCount (
  IN      PSYSHOST                 Host,
  IN      UINT8                    Socket,
  IN      MRC_MST                  MemSs,
  IN      UINT8                    Channel,
  IN      MRC_TT                   TestType,
  IN      UINT8                    Counter,
  OUT     CPGC_ERROR_COUNT_STATUS *CountStatus
  )
{
  CPGC_ERID_LFSR_ERR_MC_2LM_STRUCT                 CpgcEridLfsrErr;
  CPGC_ERR_COUNTER0_S_MCDDC_CTL_STRUCT             ErrCounter;
  CPGC_ERR_COUNTER_OVERFLOW_MCDDC_DP_STRUCT        ErrCounterOverflow;
  UINT32 ErrCounterDataReg[MAX_STROBE] = {
    CPGC_ERR_COUNTER0_S_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER1_S_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER2_S_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER3_S_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER4_S_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER5_S_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER6_S_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER7_S_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER8_S_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER9_S_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER10_S_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER11_S_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER12_S_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER13_S_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER14_S_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER15_S_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER16_S_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER17_S_MCDDC_CTL_REG
  };

  if (!IsChannelEnabled (Socket, Channel)) {
    return MRC_STATUS_FAILURE;
  }

  //
  // EIRD test result is different from general CPGC test.
  //
  if (TestType == EarlyEridTest) {
    CpgcEridLfsrErr.Data = MemReadPciCfgEp (Socket, Channel, CPGC_ERID_LFSR_ERR_MC_2LM_REG);
    CountStatus->Count = CpgcEridLfsrErr.Bits.count;
  } else {
    ErrCounter.Data = MemReadPciCfgMC_Ch (Socket, Channel, ErrCounterDataReg[Counter], 0);
    CountStatus->Count = ErrCounter.Bits.status;

    ErrCounterOverflow.Data = MemReadPciCfgMC_Ch (Socket, Channel, CPGC_ERR_COUNTER_OVERFLOW_MCDDC_DP_REG, 0);
    if (((ErrCounterOverflow.Bits.status >> Counter) & BIT0) == BIT0) {
      CountStatus->Overflow = TRUE;
    } else {
      CountStatus->Overflow = FALSE;
    }

    if (DdrtReadPendingQueueTimedOut (Host, Socket, Channel)) {
      CountStatus->Count = (BIT0 << 23) - 1;  // CPGC 1.0 error counter size is 23 bits [22:0]
      CountStatus->Overflow = TRUE;
    }

  }

  return  MRC_STATUS_SUCCESS;
}

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
SetupWriteLevellingCoarseParam (
  IN  PSYSHOST     Host,
  OUT TCPGCAddress *CPGCAddress,
  OUT TWDBPattern  *WDBPattern,
  OUT UINT8        *LoopCount,
  OUT UINT8        *NumCl,
  OUT UINT8        *BurstWait
)
{                                                       // Rank,  Bank,  Row,    Col
  STATIC TCPGCAddress WriteLevellingCoarseCpgcAddress = {{ 0,     0,     0xFFFF, 0    },   // Start
                                                         { 0,     4,     0,      0x1F },   // Stop
                                                         { 0,     0,     0,      0    },   // Order
                                                         { 0,     0,     0,      0    },   // IncRate
                                                         { 0,     4,     0,      1    }};  // IncValue, change bank group with each command for B2B transactions
  //                                     IncRate, Start, Stop, DQPat
  TWDBPattern  WriteLevellingCoarseWdbPattern = { 0, 0, 7, BasicVA }; // Should not matter what this is. We do not compare data.

  CopyMem (CPGCAddress, &WriteLevellingCoarseCpgcAddress, sizeof(TCPGCAddress));
  CopyMem (WDBPattern, &WriteLevellingCoarseWdbPattern, sizeof(TWDBPattern));

  *LoopCount = FLY_BY_LC;
  *NumCl = FLY_BY_CACHELINES;
  *BurstWait = CPGC_SUBSEQ_WAIT_DDRT;

} // SetupWriteLevellingCoarseParam

/**

  Setup coarse write dq-dqs parameter

  @param[in]  Host         - Pointer to sysHost, the system Host (root) structure
  @param[out] CPGCAddress  - CPGC address array
  @param[out] WDBPattern   - WDB Pattern
  @param[out] LoopCount    - Number of cycles
  @param[out] NumCl        - Number of cache lines
  @param[out] BurstWait    - Number of DCLK spacing between bursts

  @retval None

**/
VOID
SetupCoarseWriteDqDqsParam (
  IN  PSYSHOST     Host,
  OUT TCPGCAddress *CPGCAddress,
  OUT TWDBPattern  *WDBPattern,
  OUT UINT8        *LoopCount,
  OUT UINT8        *NumCl,
  OUT UINT8        *BurstWait
  )
{
                                              // Rank, Bank, Row, Col
  TCPGCAddress CoarseWriteDqDqsCpgcAddress = {{0, 0, 0, 0},      // Start
                                              {0, 0xF, 0, 0x1F}, // Stop
                                              {0, 0, 0, 0},      // Order
                                              {0, 0, 0, 0},      // IncRate
                                              {0, 4, 0, 4}};     // IncValue
  //                                               IncRate, Start, Stop, DQPat
  TWDBPattern         CoarseWriteDqDqsWdbPattern = { 0, 0, 1, BasicVA };

  CopyMem (CPGCAddress, &CoarseWriteDqDqsCpgcAddress, sizeof (TCPGCAddress));
  CopyMem (WDBPattern, &CoarseWriteDqDqsWdbPattern, sizeof (TWDBPattern));

  *LoopCount = 1;
  *NumCl = 1;
  *BurstWait = CPGC_SUBSEQ_WAIT_DDRT;

} // SetupCoarseWriteDqDqsParam

/** Set whether a given channel has had its DDRT Read Pending Queue credit limit set.

  @param[in] Host           Pointer to sysHost
  @param[in] Socket         Processor socket within the system (0-based)
  @param[in] Channel        Number of the channel to check
  @param[in] CreditLimitSet Whether or not the channel has had its Read Pending Queue credit limit set.

**/
VOID
SetDdrtReadPendingQueueCreditLimitSet (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  BOOLEAN   CreditLimitSet
  )
{
  Host->var.mem.DdrtReadPendingQueueCreditLimitSet[Socket][Channel] = CreditLimitSet;
}

/** Set the DDRT Read Pending Queue (RPQ) credit limit.

  @param[in] Host         Pointer to Host structure
  @param[in] Socket       Processor socket within the system (0-based)
  @param[in] CreditLimit  The maximum number of transactions in the RPQ before
                          the DDRT scheduler stops returning credits to the CPGC
                          engine and logs an error.

  @retval N/A

**/
VOID
SetCpgcRpqCreditLimit (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT32    CreditLimit
  )
{
  UINT8                               Channel;
  CHANNEL_NVRAM_STRUCT                (*ChannelNvList)[MAX_CH];
  CPGC_DDRT_ERR_CTL_STS_MC_2LM_STRUCT CpgcDdrtErrCtlSts;
  UINT8                               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  // For all enabled channels with a DDRT DIMM...
  ChannelNvList = GetChannelNvList (Host, Socket);
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (!(*ChannelNvList)[Channel].enabled ||
        !(*ChannelNvList)[Channel].ddrtEnabled) {
      continue;
    }

    // Set credit limit.
    CpgcDdrtErrCtlSts.Data = MemReadPciCfgEp (Socket, Channel, CPGC_DDRT_ERR_CTL_STS_MC_2LM_REG);
    CpgcDdrtErrCtlSts.Bits.crdt_limit = CreditLimit;
    MemWritePciCfgEp (Socket, Channel, CPGC_DDRT_ERR_CTL_STS_MC_2LM_REG, CpgcDdrtErrCtlSts.Data);

    // Set or clear flag indicating the credit limit has been set on this channel.
    if (CreditLimit == 0) {
      SetDdrtReadPendingQueueCreditLimitSet (Host, Socket, Channel, FALSE);
    } else {
      SetDdrtReadPendingQueueCreditLimitSet (Host, Socket, Channel, TRUE);
    }

  } // for (Channel)
}

/**

  Disable the DDRT Read Pending Queue (RPQ) credit limit for all channels with a DDRT DIMM.

  @param[in] Host             - Pointer to Host structure
  @param[in] Socket           - Processor socket within the system (0-based)

  @retval N/A

**/
VOID
EFIAPI
DisableCpgcRpqCreditLimit (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
    SetCpgcRpqCreditLimit (Host, Socket, 0);
}

/**
  Set the CPGC lock state.

  Refer to the enum CPGC_LOCK_STATE for available options. Note that this is not available on all steppings of all
  processors.

  @param[in]  Socket            The target socket
  @param[in]  CpgcLockState     The desired lock state

  @retval   EFI_STATUS  Whether or not setting the lock state was successful.
**/
EFI_STATUS
EFIAPI
SetCpgcLock (
  IN  UINT8           Socket,
  IN  CPGC_LOCK_STATE CpgcLockState
  )
{
#if (defined(ICX_HOST) && !defined(BRS_HOST) && !defined(DDR5_SUPPORT)) || defined(SNR_HOST) || defined(ICXDE_HOST)
  UINT8                           Channel;
  CHANNEL_NVRAM_STRUCT            (*ChannelNvList)[MAX_CH];
  PSYSHOST                        Host;
  LT_CPGC_CONTROL_MC_MAIN_STRUCT  LtCpgcControl;
  UINT8                           MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer();
  ChannelNvList = GetChannelNvList (Host, Socket);

  // Loop for each channel
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if ((*ChannelNvList)[Channel].enabled == 0) {
      continue;
    }

    LtCpgcControl.Data = MemReadPciCfgEp (Socket, Channel, LT_CPGC_CONTROL_MC_MAIN_REG);
    if (LtCpgcControl.Bits.res_lock != (UINT32)CpgcLockState) {
      LtCpgcControl.Bits.res_lock = (UINT32)CpgcLockState;
      MemWritePciCfgEp (Socket, Channel, LT_CPGC_CONTROL_MC_MAIN_REG, LtCpgcControl.Data);
    }
  } // Channel loop
#endif // ICX_HOST or SNR_HOST or ICXDE_HOST

  return EFI_SUCCESS;
}

/**

  This function sets CPGC register used to enable/disable refresh per rank

  @param Host:           Pointer to sysHost
  @param Socket:         Processor socket within the system (0-based)
  @param RefMaskValue:   Bit-field per physical chip-select rank. 0 = refresh commands masked; 0xFF = refresh commands not masked

  @retval n/a

**/
VOID
SetRefresh (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     RefMaskValue
)
{
  UINT8                               ch;
  CPGC_MISCREFCTL_MCDDC_CTL_STRUCT    CpgcMiscRefCtlReg;
  CPGC_MISCCKECTL_MCDDC_CTL_STRUCT    MiscCKECtl;
  UINT8                               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  for (ch = 0; ch < MaxChDdr; ch++) {
    CpgcMiscRefCtlReg.Data = MemReadPciCfgEp (socket, ch, CPGC_MISCREFCTL_MCDDC_CTL_REG);
    CpgcMiscRefCtlReg.Bits.ref_rankmask = RefMaskValue;
    MemWritePciCfgEp (socket, ch, CPGC_MISCREFCTL_MCDDC_CTL_REG, CpgcMiscRefCtlReg.Data);

    MiscCKECtl.Data = MemReadPciCfgEp (socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG);
    MiscCKECtl.Bits.refresh_enable = !RefMaskValue;
    MemWritePciCfgEp (socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG, MiscCKECtl.Data);
  } // ch loop

  if (RefMaskValue == 0) {
    host->var.mem.socket[socket].hostRefreshStatus = 1;
  } else {
    host->var.mem.socket[socket].hostRefreshStatus = 0;
  }

} // SetRefresh

/**
  Polling whether or not CPGC credit ISM is in the ISM_IDLE state

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval EFI_UNSUPPORTED        If CPGC doesn't support this
          EFI_TIMEOUT            Polling timeout
          EFI_SUCCESS            Otherwise
**/
EFI_STATUS
EFIAPI
PollCpgcIsmIdle (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Polling whether or not CPGC credit negotiation process is complete.

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval EFI_UNSUPPORTED        If CPGC doesn't support this
          EFI_TIMEOUT            Polling timeout
          EFI_SUCCESS            Otherwise
**/
EFI_STATUS
EFIAPI
PollCpgcCreditsInitialized (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Initialize CPGC Credit Config Done bit to force re-negotiation for credits

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval EFI_UNSUPPORTED        If CPGC doesn't support this
          EFI_SUCCESS            Otherwise
**/
EFI_STATUS
EFIAPI
InitCpgcCreditConfigDone (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Set Credit Config Done bit once the desired number of credits have been placed in the CREDIT_CFG register

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval EFI_UNSUPPORTED        If CPGC doesn't support this
          EFI_SUCCESS            Otherwise
**/
EFI_STATUS
EFIAPI
SetCpgcCreditConfigDone (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Early config on Cpgc CMI Credit to enable CPGC for memory training

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval EFI_UNSUPPORTED        If CPGC doesn't require early config of CMI credit
          EFI_SUCCESS            If CPGC requires early config of CMI credit
**/
EFI_STATUS
EFIAPI
EarlyConfigCpgcCmi (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Set up the ERID pattern and its validation mask.

  @param[in] Host         Pointer to Host structure
  @param[in] Socket       Processor socket within the system (0-based)
  @param[in] Ch           Channel number inside the socket

  @retval N/A

**/
STATIC VOID
SetupErid (
  IN PSYSHOST        Host,
  IN UINT8           Socket,
  IN UINT8           Ch
  )
{
  BiosSetEridPatternChip (Host, Socket, Ch, EridSignal0, EridLfsrMode, 0, 0, 0x05A5A5);
  BiosSetEridPatternChip (Host, Socket, Ch, EridSignal1, EridLfsrMode, 0, 0, 0x0C6C6C);

  BiosSetEridValidationMaskChip (Host, Socket, Ch, 0x80); // invert of 0x7F
}

/**

  This routine configures the error controller register

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param Channel                           - Channel number (0-based)
  @param StopOnErr                         - Stop on error condition
  @param ChunkPairsToCheck                 - One bit for each chunk of a cacheline. Used to identify each pair of chunks to be checked.
  The register has only 4 bits set to track the 8 chunk's. So, each bit corresponds to a ChunkPair
  Set ChunkPairsToCheck BIT0 to 1 if chunk 0,1 to be enabled,  BIT1 to 1 if chunk 2,3 enabled so on...
  @param CachelineMask                     - Mask for cachelines to be enabled.  Bit value 1 = perform validation; bit value 0 = do not perform validation.  Bit position 0 = cacheline 0; bit position 1 = cacheline 1, etc.  Note that this may not be the same logic as the hardware.

  @retval N/A

**/
VOID
ConfigureErrCtl (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 Channel,
  IN TStopOnError          StopOnErr,
  IN UINT16                ChunkPairsToCheck,
  IN UINT8                 CachelineMask
  )
{
  CPGC_ERR_CTL_MCDDC_CTL_STRUCT  ErrCtlReg;

  ErrCtlReg.Data = 0;
  ErrCtlReg.Bits.stop_on_ntherr = 1;
  ErrCtlReg.Bits.stop_on_errcontrol = StopOnErr;
  ErrCtlReg.Bits.selective_err_enb_chunk = ChunkPairsToCheck;
  ErrCtlReg.Bits.selective_err_enb_cl = CachelineMask;
  MemWritePciCfgEp (Socket, Channel, CPGC_ERR_CTL_MCDDC_CTL_REG, ErrCtlReg.Data);
}

/**

  This routine configures the per bit error checking registers

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param Channel                           - Channel number (0-based)
  @param DqLow                             - Lower 32 bits of the DQ Mask, "1" means the corresponding bit "masked" and not check error
  @param DqHigh                            - Upper 32 bits of the DQ Mask, "1" means the corresponding bit "masked" and not check error
  @param Ecc                               - ECC Mask, "1" means the corresponding bit "masked" and not check error

  @retval N/A

**/
VOID
ConfigurePerBitErrMask (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 Channel,
  IN UINT32                DqLow,
  IN UINT32                DqHigh,
  IN UINT8                 Ecc
  )
{
  MemWritePciCfgEp (Socket, Channel, CPGC_ERR_DATA0_MCDDC_CTL_REG, DqLow);  //31:0
  MemWritePciCfgEp (Socket, Channel, CPGC_ERR_DATA1_MCDDC_CTL_REG, DqHigh);  // 63:32
  MemWritePciCfgEp (Socket, Channel, CPGC_ERR_ECC_MCDDC_CTL_REG, Ecc);  //71:64 - ECC
}

/**

  This routine configures DQ WDB control

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param Controller                 - Controller number (0-based)
  @param Channel                    - Channel number (0-based)
  @param StartIndex                 - Starting index in the DqDB.  The value must be less than or equal to the MaxNumberDqdbCachelines value from the GetCpgcInfo() function.
  @param EndIndex                   - Ending index in the DqDB.  The value must be less than or equal to the StartIndex value and less than or equal to the MaxNumberDqdbCachelines value from the GetCpgcInfo() function.
  @param IncRateMode                - Increment rate mode.
  @param IncRate                    - Increment rate value.  If the IncRateMode is ExponentialDqdbIncRateMode then the IncRate value must be less than or equal to the MaxDqdbIncRateExponentVal from the GetCpgcInfo() function.  If the IncRateMode is LinearDqdbIncRateMode then the IncRate value must be less than or equal to the MaxDqdbIncRateLinearVal from the GetCpgcInfo() function.

  @retval N/A

**/
VOID
ConfigureDqWdbCtl (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 Channel,
  IN UINT8                 StartIndex,
  IN UINT8                 EndIndex,
  IN DQDB_INC_RATE_MODE    IncRateMode,
  IN UINT8                 IncRate
  )
{
  CPGC_PATWDBCLCTL_MCDDC_CTL_STRUCT PatWDBCLCtl;

  //Program Write Data Buffer Related Entries
  PatWDBCLCtl.Data = 0;
  PatWDBCLCtl.Bits.incr_rate = IncRate;
  PatWDBCLCtl.Bits.start_pntr = StartIndex;
  PatWDBCLCtl.Bits.end_pntr = EndIndex;
  PatWDBCLCtl.Bits.incr_scale = (IncRateMode == LinearDqdbIncRateMode) ? 1 : 0;
  MemWritePciCfgEp (Socket, Channel, CPGC_PATWDBCLCTL_MCDDC_CTL_REG, PatWDBCLCtl.Data);
}

/**
  This routine configures the DQ WDB unisequencer mode and ECC lane data source

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param Channel                    - Channel number (0-based)
  @param Mode                       - DqDB unisequencer mode array.
  @param EccDataSource              - Specifies the ECC signal data source

  @retval N/A

**/
VOID
ConfigureDqdbUniseqModeAndErrDataSource (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 Channel,
  IN DQDB_UNISEQ_MODE      (*Mode)[3],
  IN ECC_DATA_SOURCE       EccDataSource
  )
{
  CPGC_PATWDBCL_MUXCTL_MCDDC_DP_STRUCT  PatWDBCLMuxCtl;

  PatWDBCLMuxCtl.Data = 0;
  PatWDBCLMuxCtl.Bits.mux0 = (*Mode)[0];
  PatWDBCLMuxCtl.Bits.mux1 = (*Mode)[1];
  PatWDBCLMuxCtl.Bits.mux2 = (*Mode)[2];
  PatWDBCLMuxCtl.Bits.ecc_datasrc_sel = (EccDataSource == ByteGroup0) ? 0 : 1;
  MemWritePciCfgEp (Socket, Channel, CPGC_PATWDBCL_MUXCTL_MCDDC_DP_REG, PatWDBCLMuxCtl.Data);
}

/**
  This routine sets DQ WDB unisequencer write seeds

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param Channel                    - Channel number (0-based)
  @param Seed                       - DqDB write unisequencer seed value array.

  @retval N/A

**/
VOID
SetDqWdbUniseqWrSeed (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 Channel,
  IN UINT32                (*Seed)[3]
  )
{
  MemWritePciCfgEp (Socket, Channel, CPGC_PATWDBCL_MUX0_PBWR_MCDDC_DP_REG, (*Seed)[0]);
  MemWritePciCfgEp (Socket, Channel, CPGC_PATWDBCL_MUX1_PBWR_MCDDC_DP_REG, (*Seed)[1]);
  MemWritePciCfgEp (Socket, Channel, CPGC_PATWDBCL_MUX2_PBWR_MCDDC_DP_REG, (*Seed)[2]);
}


/**
  This routine gets DQ WDB uniseqencer write seeds

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param Channel                    - Channel number (0-based)
  @param Seed                       - DqDB write unisequencer seed value array.

  @retval N/A

**/
VOID
GetDqWdbUniseqWrSeed (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 Channel,
  IN OUT UINT32            (*Seed)[3]
  )
{
  (*Seed)[0] = MemReadPciCfgEp (Socket, Channel, CPGC_PATWDBCL_MUX0_PBWR_MCDDC_DP_REG);
  (*Seed)[1] = MemReadPciCfgEp (Socket, Channel, CPGC_PATWDBCL_MUX1_PBWR_MCDDC_DP_REG);
  (*Seed)[2] = MemReadPciCfgEp (Socket, Channel, CPGC_PATWDBCL_MUX2_PBWR_MCDDC_DP_REG);
}

/**
  This routine sets DQ WDB unisequencer read seeds

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param Channel                    - Channel number (0-based)
  @param Seed                       - DqDB write unisequencer seed value array.

  @retval N/A

**/
VOID
SetDqWdbUniseqRdSeed (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 Channel,
  IN UINT32                (*Seed)[3]
  )
{
  MemWritePciCfgEp (Socket, Channel, CPGC_PATWDBCL_MUX0_PBRD_MCDDC_DP_REG, (*Seed)[0]);
  MemWritePciCfgEp (Socket, Channel, CPGC_PATWDBCL_MUX1_PBRD_MCDDC_DP_REG, (*Seed)[1]);
  MemWritePciCfgEp (Socket, Channel, CPGC_PATWDBCL_MUX2_PBRD_MCDDC_DP_REG, (*Seed)[2]);
}

/**
  This routine configures the address generator

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param Channel                    - Channel number (0-based)
  @param CPGCAddress                - Pointer to CPGC address structure.
  @param ColUpateScale              - Col update scale
  @param RowUpateScale              - Row update scale
  @param BankUpateScale             - Bank update scale
  @param RankUpateScale             - Rank update scale
  @retval N/A

**/
VOID
ConfigureSeqAddr (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 Channel,
  IN TCPGCAddress          *CPGCAddress,
  IN UINT8                 ColUpateScale,
  IN UINT8                 RowUpateScale,
  IN UINT8                 BankUpateScale,
  IN UINT8                 RankUpateScale
  )
{
  CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_STRUCT SeqBaseAddrStartLo0;
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_STRUCT SeqBaseAddrStartHi0;
  CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_STRUCT  SeqBaseAddrWrapLo0;
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_STRUCT  SeqBaseAddrWrapHi0;
  CPGC_SEQ_BAOCIC0_MC_MAIN_STRUCT           SeqBAOCIC0;
  CPGC_SEQ_BAINCCTL00_MC_MAIN_STRUCT        SeqBAIncCtl00;
  CPGC_SEQ_BAINCCTL10_MC_MAIN_STRUCT        SeqBAIncCtl10;

  SeqBaseAddrStartLo0.Data = 0;
  SeqBaseAddrStartLo0.Bits.column_addr = CPGCAddress->Start[3];
  SeqBaseAddrStartLo0.Bits.bank_addr = CPGCAddress->Start[1];
  MemWritePciCfgMC_Ch (Socket, Channel, CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_REG, 0, SeqBaseAddrStartLo0.Data);

  SeqBaseAddrStartHi0.Data = 0;
  SeqBaseAddrStartHi0.Bits.row_addr = CPGCAddress->Start[2];
  SeqBaseAddrStartHi0.Bits.rank_addr = CPGCAddress->Start[0];
  MemWritePciCfgMC_Ch (Socket, Channel, CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_REG, 0, SeqBaseAddrStartHi0.Data);

  SeqBaseAddrWrapLo0.Data = 0;
  SeqBaseAddrWrapLo0.Bits.column_addr = CPGCAddress->Stop[3];
  SeqBaseAddrWrapLo0.Bits.bank_addr = CPGCAddress->Stop[1];
  MemWritePciCfgMC_Ch (Socket, Channel, CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_REG, 0, SeqBaseAddrWrapLo0.Data);

  SeqBaseAddrWrapHi0.Data = 0;
  SeqBaseAddrWrapHi0.Bits.row_addr = CPGCAddress->Stop[2];
  SeqBaseAddrWrapHi0.Bits.rank_addr = CPGCAddress->Stop[0];
  MemWritePciCfgMC_Ch (Socket, Channel, CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_REG, 0, SeqBaseAddrWrapHi0.Data);

  // Do not bother with 23:13 as this is all DIMM testing
  SeqBAOCIC0.Data = 0;
  SeqBAOCIC0.Bits.column_addr_order = CPGCAddress->Order[3];
  SeqBAOCIC0.Bits.row_addr_order = CPGCAddress->Order[2];
  SeqBAOCIC0.Bits.bank_addr_order = CPGCAddress->Order[1];
  SeqBAOCIC0.Bits.rank_addr_order = CPGCAddress->Order[0];
  MemWritePciCfgMC_Ch (Socket, Channel, CPGC_SEQ_BAOCIC0_MC_MAIN_REG, 0, SeqBAOCIC0.Data);

  SeqBAIncCtl00.Data = 0;
  SeqBAIncCtl10.Data = 0;
  // Rank
  SeqBAIncCtl10.Bits.rank_ba_updscale = RankUpateScale;
  SeqBAIncCtl10.Bits.rank_ba_updrate = CPGCAddress->IncRate[0];
  SeqBAIncCtl10.Bits.rank_ba_inc = CPGCAddress->IncVal[0] & 0x7;

  // Bank
  SeqBAIncCtl10.Bits.bank_ba_updscale = BankUpateScale;
  SeqBAIncCtl10.Bits.bank_ba_updrate = CPGCAddress->IncRate[1];
  SeqBAIncCtl10.Bits.bank_ba_inc = CPGCAddress->IncVal[1] & 0x7;

  // Row
  SeqBAIncCtl00.Bits.row_ba_updscale = RowUpateScale;
  SeqBAIncCtl00.Bits.row_ba_updrate = CPGCAddress->IncRate[2];
  SeqBAIncCtl00.Bits.row_ba_inc = CPGCAddress->IncVal[2] & 0xFFF;

  // Col
  SeqBAIncCtl00.Bits.column_ba_updscale = ColUpateScale;;
  SeqBAIncCtl00.Bits.column_ba_updrate = CPGCAddress->IncRate[3];
  SeqBAIncCtl00.Bits.column_ba_inc = CPGCAddress->IncVal[3] & 0x7F;

  MemWritePciCfgMC_Ch (Socket, Channel, CPGC_SEQ_BAINCCTL00_MC_MAIN_REG, 0, SeqBAIncCtl00.Data);
  MemWritePciCfgMC_Ch (Socket, Channel, CPGC_SEQ_BAINCCTL10_MC_MAIN_REG, 0, SeqBAIncCtl10.Data);

}

/**
  This routine removes a given channel from binding to the CPGC global start control so that the channel
  will not generate traffic.

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param Channel                    - Channel number (0-based)

  @retval N/A

**/
VOID
RemoveChFromGolbalStart (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 Channel
  )
{
  CPGC_SEQCTL0_MC_MAIN_STRUCT CpgcSeqCtl0Reg;

  CpgcSeqCtl0Reg.Data = MemReadPciCfgMC_Ch (Socket, Channel, CPGC_SEQCTL0_MC_MAIN_REG, 4);
  CpgcSeqCtl0Reg.Bits.global_control = 0;
  MemWritePciCfgMC_Ch (Socket, Channel, CPGC_SEQCTL0_MC_MAIN_REG, 4, CpgcSeqCtl0Reg.Data);
}

/**
  This routine configures the error counter

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param Channel                           - Channel number (0-based)
  @param Counter                           - Zero based error counter number.  The value must be less than or equal to the MaxNumberErrorCounters value from the GetCpgcInfo() function.
  @param Mode                              - Error counter mode.
  @param ModeIndex                         - Extra index used to provide additional information if needed by the mode selected. This indicates which lane, nibble, byte, or chunk has been selected.  If CounterMode value is LaneErrorCounterMode then the ModeIndex value must be less than the BusWidth value from the GetSystemInfo() function.  If CounterMode is NibbleErrorCounterMode then the ModeIndex value must be less than the BusWidth/4 value from the GetSystemInfo() function.  If CounterMode is ByteErrorCounterMode then the ModeIndex value must be less than the BusWidth/8 value from the GetSystemInfo() function.  If CounterMode is ChunkErrorCounterMode then the ModeIndex value must be less than 8.

  @retval N/A

**/
VOID
ConfigureErrorCounter (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Channel,
  UINT8                 Counter,
  ERROR_COUNTER_MODE    Mode,
  UINT32                ModeIndex
  )
{
  CPGC_ERR_COUNTER0_MCDDC_CTL_STRUCT      ErrCounterReg;
  UINT32 SubseqReg[MAX_STROBE] = {
    CPGC_ERR_COUNTER0_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER1_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER2_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER3_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER4_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER5_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER6_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER7_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER8_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER9_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER10_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER11_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER12_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER13_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER14_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER15_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER16_MCDDC_CTL_REG,
    CPGC_ERR_COUNTER17_MCDDC_CTL_REG
  };

  // NotAvailable if the given Mode value is not supported
  ErrCounterReg.Data = 0; // We are initializing to 0 all the error counter bits/registers as per the I/P
  ErrCounterReg.Bits.ctl = Mode;
  ErrCounterReg.Bits.pntr = ModeIndex; //Counter_Pointer, to indicate what lane or byte group is desired for tracking Errors.. This indicates which lane, nibble, byte, or chunk has been selected.

  MemWritePciCfgMC_Ch (Socket, Channel, SubseqReg[Counter], 0, ErrCounterReg.Data); //31:0
}

/**

  This routine configure the unisequencer LMN mode

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param ChannelInSocket            - Channel number (0-based)
  @param Uniseq                     - Zero based DqDB unisequencer number(0-based)
  @param L                          - DqDB unisequencer L counter value.  The value must be less than or equal to the MaxDqdbUniseqLVal value from the GetCpgcInfo() function.
  @param M                          - DqDB unisequencer M counter value.  The value must be less than or equal to the MaxDqdbUniseqMVal value from the GetCpgcInfo() function.
  @param N                          - DqDB unisequencer N counter value.  The value must be less than or equal to the MaxDqdbUniseqNVal value from the GetCpgcInfo() function.
  @param LDataSel                   - DqDB unisequencer L data select value.  The value must be 0 or 1.
  @param EnableFreqSweep            - Enables the Frequency Sweep feature.

  @retval N/A

**/
VOID
ConfigureDqWdbUniseqLmn (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Channel,
  UINT8                 Uniseq,
  UINT16                L,
  UINT16                M,
  UINT16                N,
  UINT8                 LDataSel,
  BOOLEAN               EnableFreqSweep
  )
{
  CPGC_PATWDBCLMUX_LMN_MCDDC_DP_STRUCT       CpgcLmnMuxReg;

  CpgcLmnMuxReg.Data = 0;
  CpgcLmnMuxReg.Bits.sweep_freq = (EnableFreqSweep) ? 1 : 0;
  CpgcLmnMuxReg.Bits.l_cnt = L;
  CpgcLmnMuxReg.Bits.m_cnt = M;
  CpgcLmnMuxReg.Bits.n_cnt = N;
  CpgcLmnMuxReg.Bits.l_datsel = LDataSel;
  MemWritePciCfgEp (Socket, Channel, CPGC_PATWDBCLMUX_LMN_MCDDC_DP_REG, CpgcLmnMuxReg.Data);
}

/**

  This routine configure the DQ WDB DC mask

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param Channel                    - Channel number (0-based)
  @param Mode                       - Invert/DC mode.
  @param DcPolarity                 - DC polarity (when operating in the DC mode). 0 = DC low. 1 = DC high.
  @param ShiftRateExponent          - Exponent of the bitmask shift rate.  The value must be less than or equal to the MaxDqdbInvertDcShiftRateExponentVal value from the GetCpgcInfo() function.
  @param DqMaskLow                 - Lower 32 bits of the DQ Mask (0-based)
  @param DqMaskHigh                - Upper 32 bits of the DQ Mask (0-based)
  @param EccMask                    - Ecc Mask (0-based)

  @retval N/A

**/
VOID
ConfigureDqWdbInvertDc (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Channel,
  INVERT_DC_MODE        Mode,
  UINT8                 DcPolarity,
  UINT8                 ShiftRateExponent,
  UINT32                DqMaskLow,
  UINT32                DqMaskHigh,
  UINT8                 EccMask
  )
{
  CPGC_PATWDB_INV_MCDDC_DP_STRUCT       CpgcPatWdbInvReg;

  //Setting the DQ lane and ECC bits as per the bitMask for the Channel no. entered
  MemWritePciCfgEp (Socket, Channel, CPGC_PATWDB_INV0_MCDDC_DP_REG, DqMaskLow); //31:0
  MemWritePciCfgEp (Socket, Channel, CPGC_PATWDB_INV1_MCDDC_DP_REG, DqMaskHigh); // 63:32

  CpgcPatWdbInvReg.Data = 0;
  CpgcPatWdbInvReg.Bits.eccinv_or_dcenb = EccMask;  //64:71 ECC lanes
  CpgcPatWdbInvReg.Bits.dc_polarity = (DcPolarity == 0) ? 0 : 1;
  CpgcPatWdbInvReg.Bits.invordc_ctl = (Mode == InvertMode) ? 0 : 1;
  CpgcPatWdbInvReg.Bits.invordc_shft_rate = ShiftRateExponent;

  MemWritePciCfgEp (Socket, Channel, CPGC_PATWDB_INV_MCDDC_DP_REG, CpgcPatWdbInvReg.Data);
}

/**

  This routine configures the CADB controller

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param Channe                     - Channel number (0-based)
  @param EnableAlwaysOn             - Specifies whether CADB patterns will be driven out on all command and address pins every Dclk while a test is running.
  @param EnableOnDeselect           - Specifies whether CADB patterns will be driven out during deselect packets.
  @param EnableParityNTiming        - Specifies parity timing.  If this parameter is TRUE, CADB parity will be driven on same clock as CMD/Address (i.e., N timing for UDIMM DDR4 only).  If this parameter is FALSE, CADB parity will be driven on the clock after CMD/Address (i.e,. N+1 timing for all other devices).

  @retval N/A

**/
VOID
SetCadbCtl (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 Channel,
  IN BOOLEAN               EnableAlwaysOn,
  IN BOOLEAN               EnableOnDeselect,
  IN BOOLEAN               EnableParityNTiming
  )
{
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT CadbCtlReg;

  CadbCtlReg.Data = MemReadPciCfgEp (Socket, Channel, CPGC_PATCADBCTL_MCDDC_CTL_REG);

  CadbCtlReg.Bits.enable_cadb_always_on = (EnableAlwaysOn) ? 1 : 0;
  CadbCtlReg.Bits.enable_cadb_on_deselect = (EnableOnDeselect) ? 1 : 0;
  CadbCtlReg.Bits.enable_cadb_parityntiming = (EnableParityNTiming) ? 1 : 0;

  MemWritePciCfgEp (Socket, Channel, CPGC_PATCADBCTL_MCDDC_CTL_REG, CadbCtlReg.Data);
}

/**
  Progrem CPGC loopcount.

  @param[in] Host:           Pointer to sysHost
  @param[in] Socket:         Processor socket within the system (0-based)
  @param[in] MemSs           MemSS number within the socket (0-based)
  @param[in] ChannelMask:    Channel mask within the MemSS
  @param[in] Loopcount:      CPGC loopcount.

  @retval
    MRC_STATUS_SUCCESS
    MRC_STATUS_FAILURE
    MRC_FUNCTION_NOT_IMPLEMENTED

**/
MRC_STATUS
EFIAPI
SetupTestLoopCount (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    MemSs,
  IN UINT32    ChannelMask,
  IN UINT32    LoopCount
  )
{
  UINT8                            Ch;
  UINT8                            MaxChDdr;
  CPGC_SEQCTL0_MC_MAIN_STRUCT      SeqCtl0;

  MaxChDdr = GetMaxChDdr ();
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (!IsChannelEnabled (Socket, Ch)) {
      continue;
    }

    if ((ChannelMask & (1 << Ch)) == 0) {
      continue;
    }

    SeqCtl0.Data = MemReadPciCfgMC_Ch (Socket, Ch, CPGC_SEQCTL0_MC_MAIN_REG, 0);
    SeqCtl0.Bits.loopcount = LoopCount;
    MemWritePciCfgMC_Ch (Socket, Ch, CPGC_SEQCTL0_MC_MAIN_REG, 0, SeqCtl0.Data);
  } // Ch

  return MRC_STATUS_SUCCESS;
}

/**
  Set up the CPGC WDB Pattern for BSSA test.

  @param Host:           Pointer to sysHost
  @param Socket:         Processor socket within the system (0-based)
  @param Channel:        Channel within socket
  @param CpgcConfig:     Pointer to a CPGC configuration structure.
  @param Start:          Pointer to the start cachelibe index of the WDB
  @param End:            Pointer to the end cachelibe index of the WDB

  @retval MRC_STATUS

**/
MRC_STATUS
FillupWdbPattern (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN CPGC1_CONFIG    *CpgcConfig,
  OUT UINT8    *Start,
  OUT UINT8    *End
  )
{
  UINT8 MaxChPerMc;
  UINT8 McCh;
  UINT8 Mc;
  UINT8 Chunk;
  UINT8 RotationIdx;
  UINT64* Pattern;
  UINT8 ChunksPerCacheline;

  MaxChPerMc = GetNumChannelPerMc ();
  Mc = Channel / MaxChPerMc;
  McCh = Channel % MaxChPerMc;
  ChunksPerCacheline = MRC_WDB_LINE_SIZE / 8;

  *Start = 0;

  // IF the user defined DqDB pattern is not enabled
  if (!CpgcConfig->EnableUserDefinedDqdbPatterns) {
    //
    // fill with default VA pattern for now
    //
    CpgcFillWdbDqVa (Host, Socket, Channel, 0);
    *End = 7;
    return SUCCESS;
  }

  //
  // User define WDB pattern
  //
  *End = CpgcConfig->UserDefinedDqdbPatternLengths[McCh] * (CpgcConfig->UserDefinedDqdbPatternRotationCnts[McCh] + 1) - 1;

  Pattern = (UINT64*)RcAllocatePool ((*End - *Start + 1) * ChunksPerCacheline * sizeof (UINT64));

  CopyMem (Pattern, CpgcConfig->UserDefinedDqdbPatterns[McCh],
    (CpgcConfig->UserDefinedDqdbPatternLengths[McCh] * ChunksPerCacheline * sizeof (UINT64)));

  for (RotationIdx = 1; RotationIdx <= CpgcConfig->UserDefinedDqdbPatternRotationCnts[McCh]; RotationIdx++) {
    for (Chunk = 0; Chunk < (CpgcConfig->UserDefinedDqdbPatternLengths[McCh] * ChunksPerCacheline); Chunk++) {
      Pattern[(RotationIdx * CpgcConfig->UserDefinedDqdbPatternLengths[McCh] * ChunksPerCacheline) + Chunk] = \
        LShiftU64 (*((UINT64*)CpgcConfig->UserDefinedDqdbPatterns[Channel][Chunk / ChunksPerCacheline][Chunk % ChunksPerCacheline]), RotationIdx);
    }
  }

  WriteWDBMultiChunkPattern (Host, Socket, Channel, Pattern, (*End - *Start + 1), *Start);

  RcFreePool (Pattern);
  return SUCCESS;
}

/**
  Both pattern and checker setting are "static" which not change during the test even switch to different tested DIMM / Rank.

  @param Host:           Pointer to sysHost
  @param socket : Processor socket within the system (0 - based)
  @param MemSs           MemSS number within the socket (0 - based)
  @param ChannelMask:    Channel mask within the MemSS
  @param CpgcConfig : Pointer to a CPGC configuration structure.

  @retval MRC_STATUS

**/
MRC_STATUS
SetupPatternAndCheckerForBssaCpgcTest (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT32    MemSs,
  UINT32    ChannelMask,
  CPGC1_CONFIG    *CpgcConfig
  )
{
  UINT8 Ch;
  UINT8 MaxChPerMc;
  UINT8 McCh;
  UINT8 Mc;
  UINT8 i;
  UINT8 MaxChDdr;
  UINT8 StartIndex;
  UINT8 EndIndex;
  UINT32 DqMaskLow;
  UINT32 DqMaskHigh;
  UINT8  EccMask;
  UINT8  CadbMuxCtl[CADB_MUX_MAX];
  BOOLEAN EnableAlwaysOn;
  BOOLEAN EnableOnDeselect;
  BOOLEAN EnableParityNTiming = FALSE;

  MaxChDdr = GetMaxChDdr ();
  MaxChPerMc = GetNumChannelPerMc ();
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (!IsChannelEnabled (Socket, Ch)) {
      continue;
    }

    if ((ChannelMask & (1 << Ch)) == 0) {
      continue;
    }

    Mc = Ch / MaxChPerMc;
    McCh = Ch % MaxChPerMc;

    //
    // Fill WDB
    //
    FillupWdbPattern (Host, Socket, Ch, CpgcConfig, &StartIndex, &EndIndex);

    //
    // WDB increment rate, etc.
    //
    ConfigureDqWdbCtl (Host, Socket, Ch, StartIndex, EndIndex, CpgcConfig->DqdbIncRateModes[McCh], CpgcConfig->DqdbIncRates[McCh]);

    ConfigureDqdbUniseqModeAndErrDataSource (Host, Socket, Ch, &(CpgcConfig->DqdbUniseqModes[Mc][McCh]), CpgcConfig->EccDataSource[McCh]);
    SetDqWdbUniseqWrSeed (Host, Socket, Ch, &CpgcConfig->DqdbUniseqSeeds[Mc][McCh]);
    SetDqWdbUniseqRdSeed (Host, Socket, Ch, &CpgcConfig->DqdbUniseqSeeds[Mc][McCh]);

    DqMaskLow = 0;
    DqMaskHigh = 0;
    EccMask = 0;
    for (i = 0; i < NUMBER_BYTE_OF_LANE_MASK; i++) {
      if (i < 4) {
        DqMaskLow |= CpgcConfig->DcBitmasks[McCh][i] << (i * 8);
      } else if (i < 8) {
        DqMaskHigh |= CpgcConfig->DcBitmasks[McCh][i] << ((i - 4) * 8);
      } else {
        EccMask = CpgcConfig->DcBitmasks[McCh][i];
      }
    }

    ConfigureDqWdbInvertDc (Host, Socket, Ch, DcMode, CpgcConfig->DcPolarity[McCh], 0, DqMaskLow, DqMaskHigh, EccMask);

    ConfigureDqWdbUniseqLmn (Host, Socket, Ch, 0,
      CpgcConfig->DqdbLmnValues[Mc][McCh][0][0],
      CpgcConfig->DqdbLmnValues[Mc][McCh][0][1],
      CpgcConfig->DqdbLmnValues[Mc][McCh][0][2],
      0, FALSE);

    // CADB
    switch (CpgcConfig->CadbModes[Mc][McCh]) {
    case OnDeselectCadbMode:
      EnableAlwaysOn = FALSE;
      EnableOnDeselect = TRUE;
      break;
    case AlwaysOnCadbMode:
      EnableAlwaysOn = TRUE;
      EnableOnDeselect = FALSE;
      break;
    case NormalCadbMode:
    default:
      EnableAlwaysOn = FALSE;
      EnableOnDeselect = FALSE;
      break;
    }

    SetCadbCtl (Host, Socket, Ch, EnableAlwaysOn, EnableOnDeselect, EnableParityNTiming);

    if (CpgcConfig->CadbModes[Mc][McCh] != NormalCadbMode) {
      ProgramCADB (Host, Socket, Ch, NO_SUBCH, 0, CADB_LINES, AdvCmdPattern0, AdvCmdPattern1);

      for (i = 0; i < CADB_MUX_MAX; i++) {
        CadbMuxCtl[i] = (UINT8)CpgcConfig->CadbUniseqModes[Mc][McCh][i];
      }
      ProgramCadbMux (Socket, Ch, NO_SUBCH, CpgcConfig->CadbUniseqSeeds[Mc][McCh], CadbMuxCtl);
    }

    // ERID
    if (CpgcConfig->EnableErid) {
      SetupErid (Host, Socket, Ch);
    }

    // Error checker
    ConfigureErrCtl (Host, Socket, Ch, CpgcConfig->CpgcStopMode, CpgcConfig->ChunkValBitmasks[Mc][McCh], 0xFF);

    DqMaskLow = 0;
    DqMaskHigh = 0;
    EccMask = 0;
    for (i = 0; i < NUMBER_BYTE_OF_LANE_MASK; i++) {
      if (i < 4) {
        DqMaskLow |= CpgcConfig->LaneValBitmasks[Mc][McCh][i] << (i * 8);
      } else if (i < 8) {
        DqMaskHigh |= CpgcConfig->LaneValBitmasks[Mc][McCh][i] << ((i - 4) * 8);
      } else {
        EccMask = CpgcConfig->LaneValBitmasks[Mc][McCh][i];
      }
    }

    // invert becasue "1" means not checking error
    DqMaskLow = ~DqMaskLow;
    DqMaskHigh = ~DqMaskHigh;
    EccMask = ~EccMask;

    ConfigurePerBitErrMask (Host, Socket, Ch, DqMaskLow, DqMaskHigh, EccMask);

    ConfigureErrorCounter (Host, Socket, Ch, 0, ChannelErrorCounterMode, 0);

  } // Ch

  return SUCCESS;
}

/**
  Detect if the target RankList contains DDRT dimm

  @param Host:           Pointer to sysHost
  @param Socket:         Processor socket within the system (0-based)
  @param Ch:             Channel number within the socket
  @param RankListSize:   Number of items in RankList array
  @param RankList:       List of ranks, subranks to operate on

  @retval TRUE: DDRT FALSE: NO DDRT dimm

**/
BOOLEAN
IsTargetedRankDdrt (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT32    RankListSize,
  IN MRC_RT    *RankList
  )
{
  UINT8    Dimm;
  UINT8    Rank;
  UINT8    LogRank = 0;
  UINT8    LogSubRank = 0;
  struct   channelNvram (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // Currently only support single rank test!
  //
  RC_FATAL_ERROR ((RankListSize == 1), ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_209);

  Dimm = RankList[0].Dimm;
  Rank = RankList[0].Rank;

  //
  // Convert physical DIMM/physical rank into logical rank
  //
  LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
  if ((*ChannelNvList)[Ch].encodedCSMode) {
    LogSubRank = (Rank >> 1);
  }

  return  (IsAepRank (Host, Socket, Ch, LogRank));
}

/**
  Set up the subsequencer wait time which depends on the targeted dimm type.

  @param Host:           Pointer to sysHost
  @param socket:         Processor socket within the system (0-based)
  @param MemSs           MemSS number within the socket (0-based)
  @param ChannelMask:    Channel mask within the MemSS
  @param RankListSize:   Number of items in RankList array
  @param *RankList:      List of ranks, subranks to operate on
  @param CpgcConfig:     Pointer to a CPGC configuration structure.
  param CpgcGblTrainingSetup: Pointer to CPGC global training setup structure

  @retval MRC_STATUS

**/
MRC_STATUS
UpdateSubSeqWait (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT32    RankListSize,
  MRC_RT    *RankList,
  CPGC1_CONFIG    *CpgcConfig,
  CPGC_GLOBAL_TRAINING_SETUP *CpgcGblTrainingSetup
  )
{
  UINT8    MaxChPerMc;
  UINT8    McCh;
  UINT8    Mc;

  MaxChPerMc = GetNumChannelPerMc ();
  Mc = Ch / MaxChPerMc;
  McCh = Ch % MaxChPerMc;

  //
  // Update subseq_wait based on the dimm type DDRT vs DDR4
  //
  if (IsTargetedRankDdrt (Host, Socket, Ch, RankListSize, RankList)) {
    CpgcGblTrainingSetup->burstSeparation = CPGC_SUBSEQ_WAIT_DDRT;
  } else {
    CpgcGblTrainingSetup->burstSeparation = (UINT8)CpgcConfig->InterSubseqWaits[Mc][McCh];
  }

  return SUCCESS;
}


//
// SubSequencer set up for different traffic mode
//
STATIC CPGC_SUBSEQ_SETUP SubSequencersByTrafficMode[TrafficModeMax] = {
  // SubseqenceStart, SubsequenceStop, SubsequenceType
  { 0, 1,{ BWr, BRd } }, // LoopbackTrafficMode
  { 0, 1,{ BWr, BRd } }, // WritesAndReadsTrafficMode  NOT SUPPORTED! Because the CPGC_GLOBAL_TRANING_SETUP only support two subsequencers.
  { 0, 0,{ BWr, BRd } }, // WriteOnlyTrafficMode
  { 0, 0,{ BRd, BRd } }, // ReadOnlyTrafficMode
  { 0, 0,{ BWr, BRd } }, // IdleTrafficMode -> No traffic will be generated for the given channel.
  { 0, 1,{ BWr, BRd } }, // Wr2WrAndRd2RdTrafficMode
  { 0, 0,{ BWrRd, BRd } }, // Rd2WrTrafficMode
  { 0, 1,{ BWr, BRdWr } }  // Wr2RdTrafficMode
};


//
// Address schemes
//
// single rank, linear address test (for dq signals)
STATIC TCPGCAddress LinearCpgcAddr = {
  //                 Rank,              Bank,              Row,               Col
  /* Start       */{ 0,                 0,                 0,                 0 },
  /* Stop        */{ 0,                 4,                 0xFFFF ,           0x7F }, // the row wrap address will be runtime updated based on the DIMM info
  /* Order       */{ 0,                 0,                 0,                 0 },    // carry increment order = bank->col->row
  /* IncRate     */{ 0,                 0,                 0,                 0 },
  /* IncVal      */{ 0,                 4,                 0,                 1 }
};

// DDR4 Cmd/CmdVref/Ctl test address scheme
STATIC TCPGCAddress ActiveCpgcAddr = {
  //               Rank,              Bank,              Row,               Col
  /* Start       */{ 0,                 0,                 0,                 0 },
  /* Stop        */{ 0,                 0xF,               0x1FFF,            0x1F },  // the row wrap address will be runtime updated based on the DIMM info
  /* Order       */{ 0,                 0,                 0,                 0 },
  /* IncRate     */{ 0,                 0,                 0,                 0 },
  /* IncVal      */{ 0,                 3,                 1553,              43 }
};

// DDR4 Cmd/CmdVref/Ctl test address scheme for x16 device, CPGC engine needs to avoid any access to BG1 toggling during test
STATIC TCPGCAddress ActiveCpgcAddr_x16 = {
  //               Rank,              Bank,              Row,               Col
  /* Start       */{ 0,                 0,                 0,                 0 },
  /* Stop        */{ 0,                 0x7,               0xFFFF,            0x1F },
  /* Order       */{ 0,                 0,                 0,                 0 },
  /* IncRate     */{ 0,                 0,                 0,                 0 },
  /* IncVal      */{ 0,                 1,                 1553,              43 }
};

// DDRT Cmd/CmdVref/Ctl address scheme is slightly different from DDR4
STATIC TCPGCAddress ActiveCpgcAddr_DDRT = {
  //                 Rank,              Bank,              Row,               Col
  /* Start       */{ 0,                 0,                 0,                 0 },
  /* Stop        */{ 0,                 0xC,               0x1FFF,            0x1F },  // the row wrap address will be runtime updated based on the DIMM info
  /* Order       */{ 0,                 0,                 0,                 0 },
  /* IncRate     */{ 0,                 0,                 0,                 0 },
  /* IncVal      */{ 0,                 4,                 0,                 43 }
};


/**
  Update the row wrapper value based on the target RankList

  @param Host:           Pointer to sysHost
  @param Socket:         Processor socket within the system (0-based)
  @param Ch:             Channel number within the socket
  @param RankListSize:   Number of items in RankList array
  @param RankList:       List of ranks, subranks to operate on
  @param AddressSetup    Pointer to the CPGC address setup structure

  @retval MRC_STATUS

**/
BOOLEAN
UpdateRowWrapperAddress (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT32    RankListSize,
  IN MRC_RT    *RankList,
  OUT  TCPGCAddress  *AddressSetup
  )
{
  UINT8    Dimm;
  UINT32   AddressMask;

  RC_FATAL_ERROR ((RankListSize == 1), ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_209);

  Dimm = RankList[0].Dimm;

  CalculateAddressMask (Host, Socket, Ch, Dimm, &AddressMask);

  AddressSetup->Stop[2] = AddressMask;

  if (IsThereAnyDimmX16InChannel (Socket, Ch)) {
    AddressSetup->Stop[2] = AddressMask & 0xFFFF;
  }

  return SUCCESS;
}

/**
  Set up the subsequencer type which depends on the traffic mode

  @param Host:           Pointer to sysHost
  @param Socket:         Processor socket within the system (0-based)
  @param Ch:             Channel number inside the socket
  @param RankListSize:   Number of items in RankList array
  @param RankList:       List of ranks, subranks to operate on
  @param CpgcConfig:     Pointer to a CPGC configuration structure.
  @param CpgcGblTrainingSetup: Pointer to CPGC global training setup structure

  @retval MRC_STATUS

**/
MRC_STATUS
UpdateSubSeq (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT32    RankListSize,
  IN MRC_RT    *RankList,
  IN CPGC1_CONFIG    *CpgcConfig,
  OUT CPGC_GLOBAL_TRAINING_SETUP *CpgcGblTrainingSetup
  )
{
  UINT8    MaxChPerMc;
  UINT8    McCh;
  UINT8    Mc;
  CPGC_SUBSEQ_SETUP *SubSeqSetup;
  BOOLEAN  IsDdrtDimm = FALSE;
  TRAFFIC_MODE TrafficMode;

  IsDdrtDimm = IsTargetedRankDdrt (Host, Socket, Ch, RankListSize, RankList);

  MaxChPerMc = GetNumChannelPerMc ();
  Mc = Ch / MaxChPerMc;
  McCh = Ch % MaxChPerMc;

  TrafficMode = CpgcConfig->TrafficModes[Mc][McCh];

  SubSeqSetup = &SubSequencersByTrafficMode[TrafficMode];

  // Update the Subseq0 and 1.
  CpgcGblTrainingSetup->rwMode = SubSeqSetup->SubSequenceType[0];

  CpgcGblTrainingSetup->subseqEndPtr = SubSeqSetup->SubSequenceEnd;

  if (CpgcGblTrainingSetup->subseqEndPtr == 1) {
    CpgcGblTrainingSetup->useSubSeq1 = SubSeqSetup->SubSequenceType[1];
  }

  return SUCCESS;
}


/**
  Set up the Address scheme which depends on the traffic mode

  @param Host:           Pointer to sysHost
  @param Socket:         Processor socket within the system (0-based)
  @param Ch:             Channel number inside the socket
  @param RankListSize:   Number of items in RankList array
  @param RankList:       List of ranks, subranks to operate on
  @param CpgcConfig:     Pointer to a CPGC configuration structure.

  @retval MRC_STATUS

**/
MRC_STATUS
SetupAddressForBssaTest (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT32    RankListSize,
  IN MRC_RT    *RankList,
  IN CPGC1_CONFIG    *CpgcConfig
  )
{
  UINT8    MaxChPerMc;
  UINT8    McCh;
  UINT8    Mc;
  TCPGCAddress  AddressSetup;
  TCPGCAddress  *SelectedAddressSetup;
  BOOLEAN  IsDdrtDimm = FALSE;

  IsDdrtDimm = IsTargetedRankDdrt (Host, Socket, Ch, RankListSize, RankList);

  MaxChPerMc = GetNumChannelPerMc ();
  Mc = Ch / MaxChPerMc;
  McCh = Ch % MaxChPerMc;


  SelectedAddressSetup = &LinearCpgcAddr;
  if (CpgcConfig->AddrModes[Mc][McCh] == ActiveAddrMode) {
    if (IsDdrtDimm) {
      SelectedAddressSetup = &ActiveCpgcAddr_DDRT;
    } else {
      if (IsThereAnyDimmX16InChannel (Socket, Ch)) {
        SelectedAddressSetup = &ActiveCpgcAddr_x16;
      } else {
        SelectedAddressSetup = &ActiveCpgcAddr;
      }
    }
  }

  CopyMem (&AddressSetup, SelectedAddressSetup, sizeof (TCPGCAddress));

  // Update row address
  UpdateRowWrapperAddress (Host, Socket, Ch, RankListSize, RankList, &AddressSetup);

  // call API prorgam registers
  // Use "linear increment scale" - 1
  ConfigureSeqAddr (Host, Socket, Ch, &AddressSetup, 1, 1, 1, 1);

  return SUCCESS;
}


/**
  Set up CPGC rank mapping.

  @param Host:           Pointer to sysHost
  @param Socket:         Processor socket within the system (0-based)
  @param Ch:             Channel number within the socket
  @param RankListSize:   Number of items in RankList array
  @param RankList:       List of ranks, subranks to operate on

  @retval TRUE: DDRT FALSE: NO DDRT dimm

  @retval MRC_STATUS

**/
MRC_STATUS
SetupRankMapping (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN UINT8         Ch,
  IN UINT32        RankListSize,
  IN MRC_RT        *RankList
  )
{
  UINT8    Dimm;
  UINT8    Rank;
  UINT8    LogRank = 0;
  UINT8    LogSubRank = 0;
  struct   channelNvram (*ChannelNvList)[MAX_CH];
  UINT32   Status = SUCCESS;

  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // Currently only support single rank test! Two support mutliple ranks, we need to
  // udpate "SelectCPGCRanks" or create a new function.
  //
  RC_FATAL_ERROR (RankListSize == 1, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
  Dimm = RankList[0].Dimm;
  Rank = RankList[0].Rank;

  //
  // Convert physical DIMM/physical rank into logical rank
  //
  LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
  if ((*ChannelNvList)[Ch].encodedCSMode) {
    LogSubRank = (Rank >> 1);
  }

  //
  // Program logical to physical rank mapping
  //
  SelectCPGCRanks (Host, Socket, Ch, 1 << LogRank, 0, LogSubRank);

  return Status;
}

/**
  Set up the CPGC Sequence and address scheme for BSSA.

  Both pattern and checker setting are "static" which not change during the test even switch to different tested DIMM/Rank.

  @param Host:           Pointer to sysHost
  @param socket:         Processor socket within the system (0-based)
  @param MemSs           MemSS number within the socket (0-based)
  @param ChannelMask:    Channel mask within the MemSS
  @param RankListSize:   Number of items in RankList array
  @param RankList:       List of ranks, subranks to operate on
  @param CpgcConfig:     Pointer to a CPGC configuration structure.

  @retval MRC_STATUS

**/
MRC_STATUS
SetupSequencerAndAddressForBssaCpgcTest (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN MRC_MST   MemSs,
  IN UINT32    ChannelMask,
  IN UINT32    RankListSize,
  IN MRC_RT    *RankList,
  IN CPGC1_CONFIG    *CpgcConfig
  )
{
  UINT8 Ch;
  UINT8 MaxChPerMc;
  UINT8 McCh;
  UINT8 Mc;
  UINT8 MaxChDdr;

  BOOLEAN EnableAlwaysOn;
  BOOLEAN EnableOnDeselect;
  BOOLEAN EnableParityNTiming = FALSE;
  CPGC_GLOBAL_TRAINING_SETUP CpgcGblTrainingSetup;

  MaxChDdr = GetMaxChDdr ();
  MaxChPerMc = GetNumChannelPerMc ();
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (!IsChannelEnabled (Socket, Ch)) {
      continue;
    }

    if ((ChannelMask & (1 << Ch)) == 0) {
      continue;
    }

    Mc = Ch / MaxChPerMc;
    McCh = Ch % MaxChPerMc;

    if (CpgcConfig->TrafficModes[Mc][McCh] == IdleTrafficMode) {
      RemoveChFromGolbalStart (Host, Socket, Ch);
      continue;
    }

    //
    // Configure global training structure, use the CpgcGlobalTrainingSetup() to set up subsequencer and sequencer
    //
    CpgcGblTrainingSetup.seqLoopCnt = (UINT8)CpgcConfig->LoopCounts[Mc][McCh];
    CpgcGblTrainingSetup.startDelay = (UINT8)CpgcConfig->StartDelays[Mc][McCh];
    CpgcGblTrainingSetup.addrUpdRateMode = CPGC_UPDATE_BY_CL;       // Update based on the # of read and write cacheline
    CpgcGblTrainingSetup.numCacheLines = Log2x32 (CpgcConfig->BurstLengths[Mc][McCh]);
    CpgcGblTrainingSetup.errEnChunk = CpgcConfig->ChunkValBitmasks[Mc][McCh];
    CpgcGblTrainingSetup.errEnCl = 0xFF;                           // Check error on all cachelines
    CpgcGblTrainingSetup.stopOnErrCtl = CpgcConfig->CpgcStopMode;
    CpgcGblTrainingSetup.useSubSeq1 = CPGC_NO_SUBSEQ1;
    CpgcGblTrainingSetup.cpgcLinkTrain = 0;                         // do not use cpgc link train
    CpgcGblTrainingSetup.back2backWrEn = 1;                         // enable b2b writes
    CpgcGblTrainingSetup.mprControl = 0;                            // MPR disable
    CpgcGblTrainingSetup.refMask = 0xFF;                            // disable cpgc self refresh
    CpgcGblTrainingSetup.zqMask = 0xFF;                             // disable cpgc zq
    CpgcGblTrainingSetup.x8Mode = 0;

    UpdateSubSeqWait (Host, Socket, Ch, RankListSize, RankList, CpgcConfig, &CpgcGblTrainingSetup);
    UpdateSubSeq (Host, Socket, Ch, RankListSize, RankList, CpgcConfig, &CpgcGblTrainingSetup);

    CpgcGlobalTrainingSetup (Host, Socket, Ch, &CpgcGblTrainingSetup);

    //
    // Address scheme.
    //
    SetupAddressForBssaTest (Host, Socket, Ch, RankListSize, RankList, CpgcConfig);

    // CADB
    switch (CpgcConfig->CadbModes[Mc][McCh]) {
    case OnDeselectCadbMode:
      EnableAlwaysOn = FALSE;
      EnableOnDeselect = TRUE;
      break;
    case AlwaysOnCadbMode:
      EnableAlwaysOn = TRUE;
      EnableOnDeselect = FALSE;
      break;
    case NormalCadbMode:
    default:
      EnableAlwaysOn = FALSE;
      EnableOnDeselect = FALSE;
      break;
    }

    SetCadbCtl (Host, Socket, Ch, EnableAlwaysOn, EnableOnDeselect, EnableParityNTiming);

    SetupRankMapping (Host, Socket, Ch, RankListSize, RankList);

  } // Ch

  return SUCCESS;
}
/**
  Set up the CPGC for BSSA.
  @param Host:           Pointer to sysHost
  @param socket:         Processor socket within the system (0-based)
  @param MemSs           MemSS number within the socket (0-based)
  @param ChannelMask:    Channel mask within the MemSS
  @param RankListSize:   Number of items in RankList array
  @param RankList:       List of ranks, subranks to operate on
  @param CpgcConfig:     Pointer to a CPGC configuration structure.
  @param UpdatePatternAndChecker: Flag to update the CPGC pattern and error checker settings.
  @param UpdateSequenceAndAddress: Flag to update the CPGC sequencers and address generation settings.

  @retval MRC_STATUS_SUCCESS, MRC_STATUS_FAILURE, MRC_FUNCTION_NOT_IMPLEMENTED
**/
MRC_STATUS
EFIAPI
SetupBssaCpgcTest (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN MRC_MST   MemSs,
  IN UINT32    ChannelMask,
  IN UINT32    RankListSize,
  IN MRC_RT    *RankList,
  IN CPGC_CONFIGURE_HEADER *CpgcConfig,
  IN BOOLEAN   UpdatePatternAndChecker,
  IN BOOLEAN   UpdateSequenceAndAddress
  )
{
  CPGC1_CONFIG *Cpgc1Config;

  // Check the version of the config to ensure it is CPGC1
  if (CpgcConfig->MajorVersion != 1) {
    return MRC_STATUS_FAILURE;
  }

  Cpgc1Config = (CPGC1_CONFIG*)CpgcConfig;

  // Get the config and call support funciton to set up the CPGC engine
  if (UpdatePatternAndChecker) {
    SetupPatternAndCheckerForBssaCpgcTest (Host, Socket, MemSs, ChannelMask, Cpgc1Config);
  }

  if (UpdateSequenceAndAddress) {
    // Test sequence
    // Address scheme
    SetupSequencerAndAddressForBssaCpgcTest (Host, Socket, MemSs, ChannelMask, RankListSize, RankList, Cpgc1Config);
  }

  return MRC_STATUS_SUCCESS;
}

/**

  Initialize CPGC controller rank mask registers

  @param[in] Socket  - Socket number

  @retval n/a

**/
VOID
ConfigureCpgcRankMask (
  IN UINT8 Socket
  )
{
  //Not Implemented
}

/**

  This function is used to configure error check mask for chunks

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Processor socket within the system (0-based)
  @param[in] Ch               - Channel number
  @param[in] SubCh            - Sub-channel number
  @param[in] MaskChunk        - Bit mask for chunks

  @retval N/A

**/
VOID
SetCpgcErrChkMaskChunks (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubCh,
  IN UINT32   MaskChunk
  )
{
  //Not Implemented
}
