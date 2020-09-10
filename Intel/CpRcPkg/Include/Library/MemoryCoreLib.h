/** @file
  Interface header file for the Memory Core library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef _MEMORY_CORE_LIB_H_
#define _MEMORY_CORE_LIB_H_

//
// Temporary include for SysHost definition
//

#include <Base.h>
#include <ReferenceCodeDataTypes.h>
#include <SysHost.h>
#include <MrcCommonTypes.h>
#include <Ppi/SsaBiosServicesPpi.h>
#include <SystemInfoVar.h>
#include <Memory/CpgcDefinitions.h>
#include <Library/MemCallTableLib.h>
#include <Memory/MemPorDefinitions.h>

//
// MRC_STATUS definitions
//

#define  SUCCESS            0
#define  FAILURE            1
#define  RETRY              2
#define  INVALID_PARAMETER  3

#define  DIMM0          0
#define  DIMM1          1
#define  DIMM2          2

#define  RANK0          0
#define  RANK1          1
#define  RANK2          2
#define  RANK3          3

#define  DUAL_RANK      2

//
// Side defintions for use with WriteMrsDdr4, SetRankMprDdr4
// and MprWritePatternRankDdr4 Side parameter.
//

#define A_SIDE         0
#define B_SIDE         1

//
// Sub Channel definition.
//

#define SUB_CH_A             0
#define SUB_CH_A_MASK        (1 << SUB_CH_A)
#define SUB_CH_B             1
#define SUB_CH_B_MASK        (1 << SUB_CH_B)
#define TWO_SUBCH_MASK       (SUB_CH_B_MASK | SUB_CH_A_MASK)

#define NO_SUBCH       0xFF
#define ALL_SUBCH      0xFF

#if FixedPcdGetBool(PcdMcrSupport)
//
// Pseudo Channel definitions
//

#define PS_CH0              0
#define PS_CH1              1
#define PS_CH0_MASK         (1 << PS_CH0)
#define PS_CH1_MASK         (1 << PS_CH1)
#define PS_CH_UNUSED        0
#define PS_CH_ALL           (PS_CH0_MASK | PS_CH1_MASK)
#define MAX_PS              2

#endif // FixedPcdGetBool(PcdMcrSupport)

#define MRC_STRING_LENGTH 128 // Don't use magic numbers in MRC string arrays

//
// Vref timing max loop index. Used to replace magic numbers in the vref centering functions.
//
#define MAX_VREF_TIMING_NUMBER  3

/**

  Memory Controller initialization entry point

  @param Host          - Pointer to the system Host (root) structure

  @retval EFI_SUCCESS  - Memory Initialization Complete
  @retval Other        - Did not complete memory initialization

**/
EFI_STATUS
EFIAPI
MemStart (
  PSYSHOST Host          // Pointer to the system Host (root) structure
  );

//
//  Previous Boot Error
//
#define ERROR_ON_PREVIOUS_BOOT    1

/**

  Determine which bootmode to use for all sockets

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval None

**/
UINT32
DetermineBootMode (
  PSYSHOST Host
  );

/**

  Initialize Memory Structure

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval None

**/
UINT32
InitStructuresLate (
  PSYSHOST Host
  );

/**

  Checks to make sure there is still memory available

  @param Host  - Pointer to sysHost, the system Host (root) structure

  @retval TRUE - Memory is present
  @retval FALSE - Memory is not present

**/
BOOLEAN
CheckMemPresentStatus (
  PSYSHOST Host          // Pointer to the system Host (root) structure
  );

VOID
WDBFill (
  PSYSHOST Host,                      // Pointer to the system Host (root) structure
  UINT8    socket,                    // Socket number
  UINT8    ch,                        // Channel number
  TWdbLine *WdbLines,                 // Array that include data to write to the WDB
  UINT8    NumberOfWdbLines,          // The number of cachelines to write
  UINT8    StartCachelineIndex        // Start offset/cacheline number on the WDB.
  );

/**

  Disables the specified channel in software only (Host structure).
  At the end of MRC, the Host structure will be checked for channels that have
  been disabled, and write the appropriate registers to disable them in hardware.

  @param[in] Host     Pointer to sysHost, the system Host (root) structure
  @param[in] Socket   socket number
  @param[in] Ch       channel number

  @retval None

**/
VOID
DisableChannelSw (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
  );

/**

  Check if a DIMM is a dual-rank DIMM.

  @param Socket - Socket number
  @param Ch     - Channel number.
  @param Dimm   - DIMM number.

  @retval TRUE  - Current DIMM is a dual-rank DIMM.
  @retval FALSE - Current DIMM is not a dual-rank DIMM.
**/

BOOLEAN
IsDualRankOnDimm (
  IN       UINT8              Socket,
  IN       UINT8              Ch,
  IN       UINT8              Dimm
  );

/*++

  Returns a bitmask of channels in which the current dimm# are dual-rank DIMMs.

  @param Host   - Pointer to sysHost
  @param Socket - Current processor socket 
  @param Dimm   - DIMM to check

  @retval ChBitmask - Bitmask of channels present for the current rank

**/
UINT32
GetChBitmaskDualRank (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Dimm
  );

/**

  Returns a bitmask of channels that have the current dimm and rank enabled

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - socket number
  @param dimm    - dimm number
  @param rank    - rank number

  @retval chBitmask - Bitmask of channels present for the current rank

**/
UINT32
GetChBitmask (
  PSYSHOST Host,      // Pointer to the system Host (root) structure
  UINT8    socket,    // socket number
  UINT8    dimm,      // dimm number
  UINT8    rank       // rank number
  );

/**

  Returns a bitmask of channels that have the current dimm and rank enabled

  @param Host  - Pointer to sysHost
  @param socket  - socket number

  @retval chBitmask - Bitmask of channels present

**/
UINT32
GetChBitmaskAll (
  PSYSHOST  Host,
  UINT8     socket
);

/**

  Get the closest supported frequency

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param socket - socket number

  @retval frequency

**/
UINT8
GetClosestFreq (
  PSYSHOST Host,              // Pointer to the system Host (root) structure
  UINT8    socket             // socket number
  );

/**
  Return the current encoded DDR frequency (DDR_2400, DDR_2933, etc)

  @param[in]    Socket    - Socket number
  @param[out]   DdrFreq   - Pointer to caller's DdrFreq storage

  @retval EFI_SUCCESS if frequency found
          EFI_INVALID_PARAMTER if passed parameter is bad

**/
EFI_STATUS
EFIAPI
GetEncodedDdrFreq (
  IN     UINT8    Socket,
  OUT    UINT8    *DdrFreq
  );

/**

  Detect DIMM population

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
DetectDIMMConfig (
  PSYSHOST Host               // Pointer to the system Host (root) structure
  );

/**

  MRC Wrapper code

  @param Host        - Pointer to sysHost, the system Host (root) structure
  @param socket      - socket number
  @param dev         - SMB device structure
  @param byteOffset  - offset of data to read
  @param data        - Pointer to data to be returned

  @retval 0 - Hook not implemented (default)
  @retval 1 - Hook implemented and read was successful
  @retval 2 - Hook implemented and read failed / slave device not present
  @retval Return 1 or 2 if this routine should replace ReadSmb

**/
UINT8
MemReadSmb (
  PSYSHOST          Host,           // Pointer to the system Host (root) structure
  UINT8             socket,         // socket number
  SMB_DEVICE_STRUCT dev,            // SMB device structure
  UINT8             byteOffset,     // offset of data to read
  volatile UINT8    *data           // Pointer to data to be returned
  );

/**

  MRC wrapper code

  @param Host        - Pointer to sysHost, the system Host (root) structure
  @param dev         - SMB device structure
  @param byteOffset  - offset of data to write
  @param data        - Pointer to data to be written

  @retval 0 - Hook not implemented (default)
  @retval 1 - Hook implemented and read was successful
  @retval 2 - Hook implemented and read failed / slave device not present
  @retval Return 1 or 2 if this routine should replace ReadSmb

**/
UINT8
MemWriteSmb (
  PSYSHOST          Host,           // Pointer to the system Host (root) structure
  SMB_DEVICE_STRUCT dev,            // SMB device structure
  UINT8             byteOffset,     // offset of data to read
  UINT8             *data           // Pointer to data to be returned
  );

/**

  Converts MTB/FTB to DCLK for DDR4

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param Socket   - Processor socket within the system (0-based)
  @param Ch       - DDR channel number within the processor socket (0-based)
  @param Dimm     - DIMM number within a DDR channel (0-based, farthest slot == 0)
  @param MtbTime  - Time in MTB (Medium Time Base) for DDR4
  @param FtbTime  - Time in FTB (Fine Time Base)   for DDR4

  @retval Time in DCLKs

**/
UINT16
TimeBaseToDCLKDdr4 (
  PSYSHOST  Host,       // Pointer to Host structure (outside scope of this spec)
  UINT8     Socket,     // Processor socket within the system (0-based)
  UINT8     Ch,         // DDR channel number within the processor socket (0-based)
  UINT8     Dimm,       // DIMM number within a DDR channel (0-based, farthest slot == 0)
  UINT32    MtbTime,    // Time in MTB (Medium Time Base)
  UINT16    FtbTime     // Time in FTB (Fine Time Base)
  );

/**

  Read SPD byte from Serial EEPROM

  @param Host       - Pointer to sysHost, the system Host (root) structure
  @param socket     - Socket ID
  @param ch         - Channel on socket
  @param dimm       - DIMM on channel
  @param byteOffset - Byte Offset to read
  @param data       - Pointer to data

  @retval status

**/
UINT32
EFIAPI
ReadSpd (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT16      byteOffset,                       // Byte Offset to read
  UINT8       *data                             // Pointer to data
  );

/**

  Detect DDR4 configuration

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Processor socket within the system (0-based)
  @param ch      - DDR channel number within the processor socket (0-based)
  @param dimm    - DIMM number within a DDR channel (0-based, farthest slot == 0)

  @retval SUCCESS

**/
UINT32
DetectDDR4DIMMConfig (
  PSYSHOST    Host,   // Pointer to Host structure (outside scope of this spec)
  UINT8       socket, // Processor socket within the system (0-based)
  UINT8       ch,     // DDR channel number within the processor socket (0-based)
  UINT8       dimm    // DIMM number within a DDR channel (0-based, farthest slot == 0)
  );

/**

  Collects data from SPD for DDR4 LRDIMM

  @param[in] Host   - Point to sysHost
  @param[in] Socket - CPU socket ID
  @param[in] Ch     - Channel to be accessed
  @param[in] Dimm   - DIMM to be accessed

  @retval SUCCESS

**/
UINT32
GetLrdimmSpdDataDdr4 (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT8       Ch,
  IN UINT8       Dimm
  );

/**

  Collects data from SPD for DDR4

  @param None

  @retval SUCCESS

**/
UINT32
GatherSPDDataDDR4 (
  VOID
  );

/**

  Get the tech index for the current dimm

  @param Socket  - Processor socket within the system (0-based)
  @param Ch      - DDR channel number within the processor socket (0-based)
  @param Dimm    - DIMM number within a DDR channel (0-based, farthest slot == 0)

  @retval Returns the tech index for the current dimm

**/
UINT8
GetTechIndex (
  UINT8     Socket, // Processor socket within the system (0-based)
  UINT8     Ch,     // DDR channel number within the processor socket (0-based)
  UINT8     Dimm    // DIMM number within a DDR channel (0-based, farthest slot == 0)
  );

/**

  Per CMD Group Margin and Group Alignment

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket Id
  @param testCase - Test Case: \n  1 = COMPOSITE_GROUPS\n  0 = Group 0

  @retval UINT32

**/
UINT32
PerCMDGroup (
  PSYSHOST  Host,     // Pointer to the system Host (root) structure
  UINT8     socket,   // Socket Id
  UINT16    testCase  // Test Case:
                      //  1 = COMPOSITE_GROUPS
                      //   0 = Group 0
  );


/**

  Collects the results of the previous test

  @param Host      - Pointer to sysHost, the system Host (root) structure
  @param socket    - Processor socket within the system (0-based)
  @param chToCheck - channel to check
  @param bwSerr    - Test error status; 72 bits per channel for ECC modules

  @retval SUCCESS
  @retval Error Code - Non-zero if any failure was detected

**/
UINT32
CollectTestResults (
  PSYSHOST  Host,             // Pointer to Host structure (outside scope of this spec)
  UINT8     socket,           // Processor socket within the system (0-based)
  UINT32    chToCheck,        // channel to check
  UINT32    bwSerr[MAX_CH][3] // Test error status; 72 bits per channel for ECC modules
  );

/**

  Re-centers read dq timing using a more robust test

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
RdAdvancedCentering (
  PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
  );

/**

  Re-centers tx dq timing using a more robust test

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
WrAdvancedCentering (
  PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
  );

/**

  Re-centers using a more robust test

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
RdVrefCentering (
  PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
  );

/**

  Re-centers using a more robust test

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
WrVrefCentering (
  PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
  );

/**

  Early Vref centering

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
RdEarlyVrefCentering (
  PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
  );

/**

  Early Vref centering

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
WrEarlyVrefCentering (
  PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
  );

/**

  Re-centers using a more robust test

  @param Host    - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
CmdVrefCentering (
  PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
  );

/*++

  This function execute the write leveling Cleanup data evaluation for DDR4 and DDRT.

  @param Host                - Pointer to sysHost
  @param ByteOff             - Byte Offset
  @param ByteSum             - Byte Sum
  @param ByteCount           - Byte Count
  @param Done                - Done indication from centering flow
  @param CRAddDelay          - Additive Delay
  @param WLChipCleanUpStruct - WL Delays
  @param DdrTech             - DDR Technology 0 DDR4 1 DDRT

  @retval NA

--*/
VOID
WriteLevelingCleanUpDataEvalTech (
  IN PSYSHOST                Host,
  INT16                      ByteOff[MAX_CH][MAX_RANK_CH][MAX_STROBE],
  IN INT16                   ByteSum[2][MAX_CH],
  IN UINT16                  ByteCount[2][MAX_CH],
  IN UINT8                   Done,
  IN INT16                   CRAddDelay[MAX_CH],
  IN CHIP_WL_CLEANUP_STRUCT  WLChipCleanUpStruct,
  IN UINT8                   DdrTech
  );

/**

  Perform Write Leveling training

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
WriteLeveling (
  PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
  );

/**

  Perform Write Leveling Cleanup.

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
WriteLevelingCleanUp (
  PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
  );

/**

  Initializes the starting margins

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Processor socket to check

  @retval None

**/
VOID
InitStartingOffset (
  PSYSHOST  Host,  // Pointer to Host structure (outside scope of this spec)
  UINT8     socket // Processor socket to check
  );

/**

  Receive Per Bit Deskew

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
RxPerBitDeskew (
  PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
  );

/**

  Tx Per Bit Deskew

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
TxPerBitDeskew (
  PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
  );

/**

  Perform Write DqDqs Per Bit Training

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval Status

**/
UINT32
WriteDqDqsPerBit (
  PSYSHOST  Host    // Pointer to the system Host (root) structure
  );

/**

  Detects DQ swizzling on board routing for NVM DIMMs

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
DqSwizzleDiscovery (
  PSYSHOST  Host    // Pointer to the system Host (root) structure
  );

/**

  Discover DQS layout swizzle for x16 DIMMs

  @param[in] Host - Pointer to sysHost, the system host (root) structure

  @retval SUCCESS

**/
UINT32
DqsSwizzleDiscoveryx16 (
  IN  PSYSHOST  Host
  );

/**

  Perform Receive Enable Training

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
ReceiveEnable (
  PSYSHOST  Host    // Pointer to the system Host (root) structure
  );

/*
  Denormalize receive enable by 128 Pi clocks

  @param[in] Socket - Socket number

  @retval EFI_SUCCESS    -  Successfully denormalized
  @retval !EFI_SUCCESS   -  Error during denormalization
*/
EFI_STATUS
EFIAPI
DenormalizeRecvEnable (
  IN UINT8 Socket
  );

/*
  Normalize receive enable

  @param[in] Socket - Socket number

  @retval EFI_SUCCESS    -  Successfully Normalized
  @retval !EFI_SUCCESS   -  Error during Normalization
*/
EFI_STATUS
EFIAPI
RenormalizeRecvEnable (
  IN UINT8 Socket
  );

/**

  Optimizes round trip latency

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
RoundTripOptimize (
  PSYSHOST  Host    // Pointer to the system Host (root) structure
  );

/**

  Check and configure XMP on all sockets

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
CheckXMP (
  PSYSHOST  Host    // Pointer to the system Host (root) structure
  );

/**

  Checks if XMP is supported by all sockets

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
ConfigureXMP (
  PSYSHOST  Host    // Pointer to the system Host (root) structure
  );

/**

  Wait for BSP to send AP continue handshake

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
WaitForConfigureXMP (
  PSYSHOST Host     // Pointer to the system Host (root) structure
  );

/**

  Wrapper code to call an OEM hook to change the socket Vddq

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param socket - processor number

  @retval SUCCESS if the Vddq change was made
  @retval FAILURE if the Vddq change was not made

**/
UINT8
CoreSetVdd (
  PSYSHOST Host,    // Pointer to Host structure (outside scope of this spec)
  UINT8    socket   // processor number
  );

/**

  Wrapper code to call an OEM platform hook call to release any platform clamps affecting CKE and/or DDR Reset

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param socket - processor number

  @retval None

**/
VOID
CoreReleaseADRClamps (
  PSYSHOST Host,    // Pointer to Host structure (outside scope of this spec)
  UINT8 socket      // processor number
  );

/**

  Wrapper code to call an OEM hook to restrict supported configurations if required.

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval None

**/
VOID
CoreCheckPORCompat (
  PSYSHOST Host   // Pointer to Host structure (outside scope of this spec)
  );

/**

  Wrapper code to call an OEM hook for overriding the memory POR frequency table

  @param Host             - Pointer to sysHost, the system Host (root) structure
  @param freqTablePtr     - Pointer to Intel POR memory frequency table.
  @param freqTableLength  - Pointer to number of DimmPOREntryStruct entries in

  @retval None

**/
VOID
CoreLookupFreqTable (
  PSYSHOST Host,                                // Pointer to Host structure (outside scope of this spec)
  struct DimmPOREntryStruct **freqTablePtr,     // Pointer to Intel POR memory frequency table.
  UINT16 *freqTableLength                       // Pointer to number of DimmPOREntryStruct entries in
                                                //    the POR Frequency Table.
  );

/**
  Prepare Nvdimm-N (Jedec Standard) Dimms for training.  Disable Arm triggers and collect status

  @param[in]Host          - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS
**/
UINT32
EFIAPI
PrepareNvdimmNForTraining (
  IN PSYSHOST Host   // Pointer to Host structure (outside scope of this spec)
  );

/**
  Restore Nvdimm-N (Jedec Standard).  Check and perform any required restore operations for Nvdimm-N dimms present

  @param[in] Host          - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS
**/
UINT32
EFIAPI
CoreRestoreNVDIMMs (
  IN PSYSHOST Host   // Pointer to Host structure (outside scope of this spec)
  );

/**

  Arm Nvdimm-N (Jedec Standard).  Check and perform any required ARM operations for Nvdimm-N dimms present.

  @param[in] Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
EFIAPI
CoreArmNVDIMMs (
  PSYSHOST Host   // Pointer to Host structure (outside scope of this spec)
  );

// Structure for providing the means to temporarily return to safe margins during training.
typedef struct _SAFE_MARGINS_ARBITER SAFE_MARGINS_ARBITER;

typedef
MRC_STATUS
(EFIAPI *SET_SAFE_MARGINS)(
  IN  SAFE_MARGINS_ARBITER  *SafeMarginsArbiter
  );

typedef
MRC_STATUS
(EFIAPI *RESTORE_TRAINED_MARGINS)(
  IN  SAFE_MARGINS_ARBITER  *SafeMarginsArbiter
  );

struct _SAFE_MARGINS_ARBITER {
  SET_SAFE_MARGINS        SetSafeMargins;
  RESTORE_TRAINED_MARGINS RestoreTrainedMargins;
  VOID                    *Context;
};

//
// Definition for GetMargins burstLength
// parameter.
//

#define WDB_BURST_LENGTH      32

/**
  Calculates positive and negative margin for the group provided

  @param Host                 Pointer to sysHost
  @param socket               Processor socket to check
  @param level                DDR or LRDIMM Host side, LRDIMM Backside, etc...
  @param group                Parameter to be margined
                                RxDqsDelay  : Margin Rx DqDqs
                                TxDqDelay   : Margin Tx DqDqs
                                RxVref      : Margin Rx Vref
                                TxVref      : Margin Tx Vref
                                RecEnDelay  : Margin Receive Enable
                                WrLvlDelay  : Margin Write Level
                                CmdGrp0     : Margin CMD group 0
                                CmdGrp1     : Margin CMD group 1
                                CmdGrp2     : Margin CMD group 2
                                CmdAll      : Margin all CMD groups
  @param mode                 Test mode to use
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
  @param scope                Margin granularity
                                SCOPE_SOCKET  0: Margin per processor socket
                                SCOPE_CH      1: Margin per channel
                                SCOPE_DIMM    2: Margin per DIMM
                                SCOPE_RANK    3: Margin per rank
                                SCOPE_STROBE  4: Margin per strobe group
                                SCOPE_BIT     5: Margin per bit
  @param mask                 Mask of bits to exclude from testing
  @param marginData           Pointer to the structure to store the margin results. The structure type varies based on scope.
  @param patternLength        RankMarginTest Pattern length
  @param update               Update starting margins so the next pass will run faster or not
  @param chMask               Bit Mask of channels to not be used
  @param rankMask             Bit Mask of ranks to not be used
  @param burstLength          Number of cachelines to test for each write/read operation. This is the repeat count for
                              the WR/RD subsequence.
  @param TestType             Test type
  @param SafeMarginsArbiter   Pointer to a structure containing the context and callbacks used to set safe values for
                              over-margined signals and restore the trained values afterward.

  @returns N/A
**/
VOID
GetMargins (
  PSYSHOST              Host,               // Pointer to sysHost, the system Host (root) structure
  UINT8                 socket,             // Socket Id
  MRC_LT                level,              // DDR or LRDIMM Host side, LRDIMM Backside, etc...
  MRC_GT                group,              // Parameter to be margined:
                                            //   RxDqsDelay : Margin Rx DqDqs
                                            //   TxDqDelay  : Margin Tx DqDqs
                                            //   RxVref     : Margin Rx Vref
                                            //   TxVref     : Margin Tx Vref
                                            //   RecEnDelay : Margin Receive Enable
                                            //   WrLvlDelay : Margin Write Level
                                            //   CmdGrp0    : Margin CMD group 0
                                            //   CmdGrp1    : Margin CMD group 1
                                            //   CmdGrp2    : Margin CMD group 2
                                            //   CmdAll     : Margin all CMD groups
  UINT16                mode,               // Test mode to use
                                            //   MODE_XTALK         BIT0: Enable crosstalk (placeholder)
                                            //   MODE_VIC_AGG       BIT1: Enable victim/aggressor
                                            //   MODE_START_ZERO    BIT2: Start at margin 0
                                            //   MODE_POS_ONLY      BIT3: Margin only the positive side
                                            //   MODE_NEG_ONLY      BIT4: Margin only the negative side
                                            //   MODE_DATA_MPR      BIT5: Enable MPR Data pattern
                                            //   MODE_DATA_FIXED    BIT6: Enable Fixed Data pattern
                                            //   MODE_DATA_LFSR     BIT7: Enable Data LFSR
                                            //   MODE_ADDR_LFSR     BIT8: Enable Address LFSR
                                            //   MODE_CHECK_PARITY  BIT10: Enable parity checking
                                            //   MODE_DESELECT      BIT11: Enable deselect patterns
                                            //   MODE_VA_DESELECT   BIT12: Enable Victim - Aggressor deselect patterns
  UINT8                 scope,              // Margin granularity
                                            //   SCOPE_SOCKET 0: Margin per processor socket
                                            //   SCOPE_CH     1: Margin per channel
                                            //   SCOPE_DIMM   2: Margin per DIMM
                                            //   SCOPE_RANK   3: Margin per rank
                                            //   SCOPE_STROBE 4: Margin per strobe group
                                            //   SCOPE_BIT    5: Margin per bit
  struct bitMask        *mask,              // Mask of bits to exclude from testing
  VOID                  *marginData,        // Pointer to the structure to store the margin results. The structure type
                                            // varies based on scope.
  UINT32                patternLength,      // RankMarginTest Pattern length
  UINT8                 update,             // Update starting margins so the next pass will run faster or not
  UINT8                 chMask,             // Bit Mask of channels to not be used
  UINT8                 rankMask,           // Bit Mask of ranks to not be used
  UINT8                 burstLength,        // Number of cachelines to test for each write/read operation.  It is the
                                            // repeat count for the WR/RD subsequence
  MRC_TT                TestType,           // Test type
  SAFE_MARGINS_ARBITER  *SafeMarginsArbiter // Pointer to a structure containing the context and callbacks to set safe
                                            // values for over-margined signals and restore the trained values
                                            // afterward.
  );

/**

  Set the Offset

  @param Host           - Pointer to sysHost, the system Host (root) structure
  @param socket         - Processor socket to check
  @param ch             - Current Channel
  @param dimm           - Dimm number (0-based)
  @param rank           - Rank number (0-based)
  @param offset         - Pointer to offset
  @param level          - DDR or LRDIMM Host side, LRDIMM Backside, etc...
  @param group          - Test group
  @param previousMargin - Previous Margin

  @retval SUCCESS
  @retval FAILURE

**/
UINT32
SetOffset (
  PSYSHOST  Host,               // Pointer to the system Host (root) structure
  UINT8     socket,             // Processor socket to check
  UINT8     ch,                 // Current Channel
  UINT8     dimm,               // Dimm number (0-based)
  UINT8     rank,               // Rank number (0-based)
  INT16     offset,             // Pointer to offset
  MRC_LT    level,              // DDR or LRDIMM Host side, LRDIMM Backside, etc...
  MRC_GT    group,              // Test group
  INT16     previousMargin      // Previous Margin
  );

/**

  Restore Offset

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket Id
  @param ch      - channel number
  @param dimm    - DIMM number
  @param rank    - rank number
  @param offset  - Offset
  @param level   - DDR or LRDIMM Host side, LRDIMM Backside, etc...
  @param group   - Test group

  @retval None

**/
VOID
RestoreOffset (
  PSYSHOST Host,      // Pointer to the system Host (root) structure
  UINT8    socket,    // Socket Id
  UINT8     ch,       // channel number
  UINT8     dimm,     // DIMM number
  UINT8     rank,     // rank number
  INT16     offset,   // Offset
  MRC_LT    level,    // DDR or LRDIMM Host side, LRDIMM Backside, etc...
  MRC_GT    group     // Test group
  );

/**

  Clears GetMargins filter

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param socket - Socket Id
  @param filter - pointer to filter buffer

  @retval None

**/
VOID
ClearFilter (
  PSYSHOST Host,              // Pointer to the system Host (root) structure
  UINT8           socket,     // Socket Id
  struct bitMask  *filter     // pointer to filter buffer
  );

/**

  This function returns the value of tCK

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket Id

  @retval tCK

**/
UINT32
GettCK (
  PSYSHOST  Host,     // Pointer to the system Host (root) structure
  UINT8     socket    // Socket Id
  );

/**

  This function returns the value of tREFI x 9

  @param[in] Socket - CPU Socket ID
  @param[in] Ch     - Channel number
  @param[in] tREFI  - Refresh rate

  @retval UINT32 Value of tREFI x 9

**/
UINT32
GettREFIx9 (
  IN UINT8     Socket,   // Socket ID
  IN UINT8     Ch,       // Channel number
  IN UINT16    tREFI     // Refresh rate
  );

/**

  This function returns the value of tREFI

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tREFI

**/
UINT32
GettREFIDdr4 (
  IN PSYSHOST  Host,     // Pointer to the system Host (root) structure
  IN UINT8     Socket,   // Socket Id
  IN UINT8     Ch        // Channel number
  );

/**

  This function returns the value of tCKE

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - CPU Socket ID
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tCKE

**/
UINT32
GettCKE (
  IN PSYSHOST  Host,      // Pointer to the system Host (root) structure
  IN UINT8     Socket,    // Socket ID
  IN UINT8     Ch         // Channel number
  );

/**

  This function returns the value of tXSDLL

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Channel - Channel number

  @retval tXSDLLLocal

**/
UINT32
GettXSDLL (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel
  );

/**

  This function returns the value of tXSOFFSET

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tXSOFFSET

**/
UINT32
GettXSOFFSET (
  IN PSYSHOST  Host,     // Pointer to the system Host (root) structure
  IN UINT8     Socket,   // Socket Id
  IN UINT8     Ch        // Channel number
  );


/**

  This function returns the value of Ddr5 tXP

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of Ddr5 tXP

**/
UINT32
GettXPDdr5 (
  IN PSYSHOST  Host,     // Pointer to the system Host (root) structure
  IN UINT8     Socket,   // Socket Id
  IN UINT8     Ch        // Channel number
  );

/**

  This function returns the value of tXP

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tXP

**/
UINT32
GettXP (
  IN PSYSHOST  Host,     // Pointer to the system Host (root) structure
  IN UINT8     Socket,   // Socket Id
  IN UINT8     Ch        // Channel number
  );

/**

  This function returns the value of tCKSRX

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id

  @retval tCKSRX

**/
UINT32
GettCKSRX (
  IN PSYSHOST  Host,     // Pointer to the system Host (root) structure
  IN UINT8     Socket    // Socket Id
  );

/**

  This function returns the value of tSTAB

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tSTAB

**/
UINT32
GettSTAB (
  IN PSYSHOST  Host,     // Pointer to the system Host (root) structure
  IN UINT8     Socket,   // Socket Id
  IN UINT8     Ch        // Channel number
  );

/**

  This function returns the value of tMOD

  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tMOD

**/
UINT32
GettMOD (
  IN UINT8     Socket,   // Socket Id
  IN UINT8     Ch        // Channel number
  );

//
// Page size definitions for use with GettFAW
// PageSize parameter.
//

#define PAGE_SIZE_1K    0
#define PAGE_SIZE_2K    1
#define PAGE_SIZE_HALFK 2

/**

  This function returns the value of tFAW

  @param[in] Host      - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket    - Socket Id
  @param[in] Ch        - Channel number
  @param[in] PageSize  - Page size

  @retval UINT32 Value of tFAW

**/
UINT32
GettFAW (
  IN PSYSHOST  Host,     // Pointer to the system Host (root) structure
  IN UINT8     Socket,   // Socket Id
  IN UINT8     Ch,       // Channel number
  IN UINT8     PageSize  // Page size
  );

/**

  This function returns the value of tRTP

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Channel - Channel number

  @retval UINT32 Value of tRTP

**/
UINT32
GettRTP (
  IN PSYSHOST  Host,     // Pointer to the system Host (root) structure
  IN UINT8     Socket,   // Socket Id
  IN UINT8     Channel   // Channel number
  );

/**

  This function returns the value of tWTR_S

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tWTR_S

**/
UINT32
GettWTR_S (
  IN PSYSHOST  Host,     // Pointer to the system Host (root) structure
  IN UINT8     Socket,   // Socket Id
  IN UINT8     Ch        // Channel number
  );

/**

  This function returns the value of tWR

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tWR

**/
UINT32
GettWR (
  IN PSYSHOST  Host,     // Pointer to the system Host (root) structure
  IN UINT8     Socket,   // Socket Id
  IN UINT8     Ch        // Channel number
  );

/**

  This function returns the value of tRFC

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tRFC

**/
UINT32
GettRFC (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  );

/**

  This function returns the value of tRFC_dlr1 in ddr clock cycles for 3DS dimms

  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT16 Value of tRFC_dlr1 in units of DCLK

**/
UINT16
EFIAPI
GetTrfcDlr1 (
  IN UINT8     Socket,
  IN UINT8     Ch
  );

/**

  This function returns the value of tRAS

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Channel - Channel number

  @retval UINT32 Value of tRAS

**/
UINT32
GettRAS (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel
  );

/**

  This function returns the value of tCWL

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tCWL

**/
UINT32
GettCWL (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  );

/**

  This function returns the value of tCL

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tCL

**/
UINT32
GettCL (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  );

/**

  This function returns the value of tRP

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tRP

**/
UINT32
GettRP (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  );

/**

  This function returns the value of tRCD

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tRCD

**/
UINT32
GettRCD (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  );

/**

  This function returns the value of tRC

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tRC

**/
UINT32
GettRC (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  );

/**

  This function returns the value of tRRD_L

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tRRD_L

**/
UINT32
GettRRD_L (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  );

/**

  This function returns the value of tRRD

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tRRD

**/
UINT32
GettRRD (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  );

/**

  This function returns the value of tWTR_L

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tWTR_L

**/
UINT32
GettWTR_L (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  );

/**

  This function returns the value of tPRPDEN

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tWTR_L

**/
UINT32
GettPRPDEN (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  );

/**

  This function returns the value of tRDA (RdCAS_AP to ACT)

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tRDA

**/
UINT32
GettRDA (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  );

/**

  This function returns the value of tWRA (WrCAS_AP to ACT)

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tWRA

**/
UINT32
GettWRA (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  );

/**

  This function returns the value of tWRPRE (WrCAS to PRE)

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tWRPRE

**/
UINT32
GettWRPRE (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  );

/**

  This function returns the value of tWRRDA
  (Write CAS to Read CAS with AutoPrecharge)

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tWRRDA

**/
UINT32
GettWRRDA (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  );

/**

  This function returns the value of tRDPDEN (RdCAS to CKE low time)

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tRDPDEN

**/
UINT32
GettRDPDEN (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  );

/**

  This function returns the value of tWRPDEN (WrCAS to CKE low time)

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tWRPDEN

**/
UINT32
GettWRPDEN (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  );

/**

  This function returns the value of tCCD_L

  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval tCCD_L

**/
UINT8
GetTccd_L (
  IN    UINT8     Socket,     // Socket Id,
  IN    UINT8     Ch          // Channel number
  );

/**

  Calculates and returns tCCD_L_WR
  tCCD_WR_L is defined in the JEDEC specification as WRITE CAS_n to WRITE CAS_n
  command delay for same bank group

  @param[in] Socket  - Socket number
  @param[in] Ch      - Channel number

  @retval tCCD_WR_L (in units of DCLK cycles)

**/
UINT8
EFIAPI
GetTccdWr_L (
  IN    UINT8     Socket,
  IN    UINT8     Ch
  );

/**

  Calculates and returns tCCD_S_WR
  tCCD_WR_S is defined in the JEDEC specification as WRITE CAS_n to WRITE CAS_n
  command delay for different bank group

  @param[in] Socket  - Socket number
  @param[in] Ch      - Channel number

  @retval tCCD_WR_S (in units of DCLK cycles)

**/
UINT8
EFIAPI
GetTccdWr_S (
  IN    UINT8     Socket,
  IN    UINT8     Ch
  );

/**
  Restores trained timing values register

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param socket - Processor socket to check
  @param ch     - Current Channel

  @retval None

**/
VOID
RestoreTimings (
  PSYSHOST    Host,             // Pointer to the system Host (root) structure
  UINT8       socket,           // Processor socket to check
  UINT8       ch                // Current Channel
  );

/**

  Checks to see of supplied group is a member of the CMD Group.

  @param group - Group item to check

  @retval 1 - Is a member of CMD Group
  @retval 0 - Is not a member of CMD Group

**/
UINT8
CheckCMDGroup (
  MRC_GT  group // Command Group
  );

/**

  Checks to see of supplied group is a member of the CTL Group.

  @param group - Group item to check

  @retval 1 - Is a member of CTL Group
  @retval 0 - Is not a member of CTL Group

**/
UINT8
CheckCTLGroup (
  MRC_GT  group // Control Group
  );

/**

  Checks to see of supplied group is a member of the "Dimm" parameter group which is adjuested
  by the RCD, DB or DRAM.

  @param group - Group item to check

  @retval TRUE  - Is a member of DIMM Group
  @retval FALSE - Is not a member of DIMM Group

**/
BOOLEAN
CheckDimmParamGroup (
  MRC_GT  Group
  );

/**

  Enables parity checking on the DIMM

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param Socket - Socket Id

  @retval None

**/
VOID
EnableParityChecking (
  PSYSHOST Host,      // Pointer to the system Host (root) structure
  UINT8    Socket     // Socket Id
  );

/**

  Disables parity checking on the DIMM

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param Socket - Socket Id

  @retval None

**/
VOID
DisableParityChecking (
  PSYSHOST Host,      // Pointer to the system Host (root) structure
  UINT8    Socket     // Socket Id
  );

/**

  Enables parity checking on the DIMM

  @param[in] Host     Pointer to sysHost, the system host (root) structure
  @param[in] Socket   Socket Id
  @param[in] Dimm     DIMM number
  @param[in] Rank     rank number

  @retval None

**/
VOID
EnableParityCheckingBackside (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Dimm,
  IN  UINT8    Rank
  );

/**

  Disables parity checking on the DIMM

  @param[in] Host     Pointer to sysHost, the system host (root) structure
  @param[in] Socket   Socket Id
  @param[in] Dimm     DIMM number
  @param[in] Rank     rank number

  @retval None

**/
VOID
DisableParityCheckingBackside (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Dimm,
  IN  UINT8    Rank
  );


/**

  Returns the logical rank number

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - channel number
  @param[in] Dimm    - DIMM number
  @param[in] Rank    - rank number

  @retval UINT8

**/
UINT8
GetLogicalRank (
  IN PSYSHOST Host,      // Pointer to the system Host (root) structure
  IN UINT8    Socket,    // Socket Id
  IN UINT8    Ch,        // channel number
  IN UINT8    Dimm,      // DIMM number
  IN UINT8    Rank       // rank number
  );

/**

  Normalize all PI's to reduce latency

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket Id

  @retval UINT32

**/
UINT32
NormalizeCCC (
  PSYSHOST Host,      // Pointer to the system Host (root) structure
  UINT8    socket     // Socket Id
  );

/**

  Set normal CMD timing

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket Id

  @retval None

**/
VOID
SetNormalCMDTiming (
  PSYSHOST Host,            // Pointer to the system Host (root) structure
  UINT8     socket          // Socket Id
  );

/**

  Updates the composite edges

  @param le      - Left edge
  @param re      - Right edge
  @param compLe  - Composite left edge
  @param compRe  - Composite right edge

  @retval None

**/
VOID
UpdateEdges (
  INT16 le,        // Left edge
  INT16 re,        // Right edge
  INT16 *compLe,   // Composite left edge
  INT16 *compRe    // Composite right edge
  );

/**
  Updates provided min and max values based on an unsigned input

  @param [in    ] Value  - New value to evaluate
  @param [in/out] MinVal - Current minimum value
  @param [in/out] MaxVal - Current maximum value

  @retval None
**/
VOID
UpdateMinMaxUint (
  IN     UINT16 Value,   // New value to evaluate
  IN OUT UINT16 *MinVal, // Current minimum value
  IN OUT UINT16 *MaxVal  // Current maximum value
  );

/**
  Updates minimum and maximum variables based on a signed input

  @param val    - New value to evaluate
  @param minVal - Current minimum value
  @param maxVal - Current maximum value

  @retval None
**/
VOID
UpdateMinMaxInt (
  INT16 val,        // New value to evaluate
  INT16 *minVal,    // Current minimum value
  INT16 *maxVal     // Current maximum value
  );

/**

  Delay time in QCLK units \n
    Note: Max delay is 1.2 seconds

  @param Host - Pointer to sysHost, the system Host (root) structure
  @param QClk - Number of QClks to delay

  @retval None

**/
VOID
FixedQClkDelay (
  PSYSHOST Host,     // Pointer to the system Host (root) structure
  UINT32   QClk      // Number of QClks to delay
  );

/**

  Executes early command/clock training

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
EarlyCmdClk (
  PSYSHOST Host       // Pointer to the system Host (root) structure
  );

/**

  Executes RCD QxCA Training

  @param[in] Host   Pointer to sysHost, the system host (root) structure

  @retval SUCCESS   Training was successful

**/
UINT32
QxCAClk (
  IN  PSYSHOST  Host
  );

/**

  Executes early control/clock training

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
EarlyCtlClk (
  PSYSHOST Host       // Pointer to the system Host (root) structure
  );

/**

  Executes command/clock training

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
LateCmdClk (
  PSYSHOST Host       // Pointer to the system Host (root) structure
  );

/**

  Starts the JEDEC initialization sequence

  @param Host       - Pointer to sysHost, the system Host (root) structure
  @param socket     - Socket to initizlize
  @param chBitMask  - Bit Mask of Channels Present on the Socket

  @retval SUCCESS

**/
UINT32
JedecInitSequence (
  PSYSHOST  Host,         // Pointer to the system Host (root) structure
  UINT8     socket,       // Socket to initizlize
  UINT32    chBitMask     // Bit Mask of Channels Present on the Socket
  );

/**

  @param Host      - Pointer to sysHost, the system Host (root) structure
  @param socket    - Socket to initizlize
  @param chBitMask - Bit Mask of Channels Present on the Socket

  @retval None

**/
VOID
JedecInitDdrAll (
  PSYSHOST Host,           // Pointer to the system Host (root) structure
  UINT8      socket,       // Socket to initizlize
  UINT32     chBitMask     // Bit Mask of Channels Present on the Socket
  );

/**

  Prints all DRAM, Register and Buffer CSR data from all DIMMs on the socket.
  Note: this function assumes that the read training has been successfully
  completed in order to read DRAM and Buffer data.

  @param[in]   Socket  - Socket Id

  @retval MRC_STATUS_FAILURE if the DRAM technology is not supported
          MRC_STATUS_SUCCESS otherwise

**/
MRC_STATUS
EFIAPI
DumpDimmState (
  IN    UINT8     Socket
  );

/**

  Mirrors address bits

  @param Host  - Pointer to sysHost, the system Host (root) structure
  @param data  - Data for the MRS write

  @retval Mirrored data

**/
UINT32
MirrorAddrBits (
  PSYSHOST Host,      // Pointer to the system Host (root) structure
  UINT32   data       // Data for the MRS write
  );

/**
  Mirrors bank address bits

  @param [in] Host - Pointer to sysHost
  @param [in] Ch   - Channel number (0-based)
  @param [in] Bank - Bank address to be mirrored

  @retval BankNew - Mirrored bank address
**/
UINT8
MirrorBankBits (
  IN PSYSHOST Host,
  IN UINT8    Ch,
  IN UINT8    Bank
  );

/**

  Inverts address bits

  @param data - Address to be inverted

  @retval Inverted address

**/
UINT32
InvertAddrBits (
  UINT32 data   // Address to be inverted
  );

/**

  Inverts bank address bits

  @param bank - Bank address to be inverted

  @retval Inverted bank address

**/
UINT8
InvertBankBits (
  UINT8 bank  // Bank address to be inverted
  );

/**

  Program register control words for RDIMMs

  @param Host    - Pointer to sysHost
  @param socket  - Socket Id
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)

  @retval None

**/
VOID
DoRegisterInitDDR4 (
  PSYSHOST Host,            // Pointer to the system Host (root) structure
  UINT8    socket,          // Socket Id
  UINT8    ch,              // Channel number (0-based)
  UINT8    dimm,            // Dimm number (0-based)
  UINT8    rank             // Rank number (0-based)
  );

VOID
DoRegisterInitDDR4_CKE (
  PSYSHOST Host,            // Pointer to the system Host (root) structure
  UINT8    socket,          // Socket Id
  UINT8    ch,              // Channel number (0-based)
  UINT8    dimm,            // Dimm number (0-based)
  UINT8    rank             // Rank number (0-based)
  );

/**

  Read LR Buffer

  @param Host       - Pointer to sysHost, the system Host (root) structure
  @param socket     - Socket Id
  @param ch         - Channel number (0-based)
  @param dimm       - DIMM number (0-based)
  @param lrbufAddr  - Address for the read
  @param lrbufData  - Pointer to the LR Buffer data

  @retval SUCCESS
  @retval FAILURE

**/
UINT32
ReadLrbufSmb (
  PSYSHOST Host,          // Pointer to the system Host (root) structure
  UINT8    socket,        // Socket Id
  UINT8    ch,            // Channel number (0-based)
  UINT8    dimm,          // DIMM number (0-based)
  UINT16   lrbufAddr,     // Address for the read
  UINT32   *lrbufData     // Pointer to the LR Buffer data
  );

/**

  Write LR Buffer

  @param Host      - Pointer to sysHost, the system Host (root) structure
  @param socket    - Socket Id
  @param ch        - Channel number (0-based)
  @param dimm      - DIMM number (0-based)
  @param size      - number of bytes to be written
  @param lrbufAddr - Address for the read
  @param lrbufData - Pointer to the LR Buffer data

  @retval SUCCESS
  @retval FAILURE

**/
UINT32
WriteLrbufSmb (
  PSYSHOST Host,          // Pointer to the system Host (root) structure
  UINT8    socket,        // Socket Id
  UINT8    ch,            // Channel number (0-based)
  UINT8    dimm,          // DIMM number (0-based)
  UINT8    size,          // number of bytes to be written
  UINT16   lrbufAddr,     // Address for the read
  UINT32   *lrbufData     // Pointer to the LR Buffer data
  );

/**

  Writes a nibble to DDR4 LRDIMM buffer control words

  @param Host             - Pointer to sysHost, the system Host (root) structure
  @param socket           - Socket Id
  @param ch               - Channel number (0-based)
  @param dimm             - Dimm number (0-based)
  @param rank             - Rank number (0-based)
  @param controlWordData  - Control Word data
  @param controlWordFunc  - Function index in the LR buffer
  @param controlWordAddr  - Control Word Address

  @retval None

**/
VOID
WriteLrbuf (
  PSYSHOST Host,                  // Pointer to the system Host (root) structure
  UINT8    socket,                // Socket Id
  UINT8    ch,                    // Channel number (0-based)
  UINT8    dimm,                  // Dimm number (0-based)
  UINT8    rank,                  // Rank number (0-based)
  UINT8    controlWordData,       // Control Word data
  UINT8    controlWordFunc,       // Function index in the LR buffer
  UINT8    controlWordAddr        // Control Word Address
  );


/**

  Writes a nibble to LRDIMM register control words using PBA mode

  @param Host                - Pointer to sysHost, the system Host (root) structure
  @param socket              - Socket Id
  @param ch                  - Channel number (0-based)
  @param dimm                - Dimm number (0-based)
  @param rank                - Rank number (0-based)
  @param controlWordDataPtr  - Pointer to a 9 byte array of control word data
  @param controlWordFunc     - Function index in the LR buffer
  @param controlWordAddr     - Control Word Address
  @param bufferAddr          - Address of the individual buffer to target...or 'FF' for all

  @retval None

**/
VOID
WriteLrbufPBA (
  PSYSHOST Host,                    // Pointer to the system Host (root) structure
  UINT8    socket,                  // Socket Id
  UINT8    ch,                      // Channel number (0-based)
  UINT8    dimm,                    // Dimm number (0-based)
  UINT8    rank,                    // Rank number (0-based)
  UINT8    *controlWordDataPtr,     // Pointer to a 9 byte array of control word data
  UINT8    controlWordFunc,         // Function index in the LR buffer
  UINT8    controlWordAddr,         // Control Word Address
  UINT8    bufferAddr               // Address of the individual buffer to target...or 'FF' for all buffers
  );

//
// Definitions for the WriteMRS Bank parameter
//

#define BANK0   0
#define BANK1   BIT0
#define BANK2   BIT1
#define BANK3   BIT0 + BIT1
#define BANK4   BIT2
#define BANK5   BIT2 + BIT0
#define BANK6   BIT2 + BIT1
#define BANK7   BIT2 + BIT1 + BIT0

/**

  Writes DRAM mode registers

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket Id
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param data    - Data for the MRS write
  @param bank    - Bank address for the MRS write

  @retval None

**/
VOID
WriteMRS (
  PSYSHOST Host,      // Pointer to the system Host (root) structure
  UINT8    socket,    // Socket Id
  UINT8    ch,        // Channel number (0-based)
  UINT8    dimm,      // DIMM number (0-based)
  UINT8    rank,      // Rank number (0-based)
  UINT16   data,      // Data for the MRS write
  UINT8    bank       // Bank address for the MRS write
  );

/**

  Writes DRAM mode registers

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param socket - Socket Id
  @param ch     - Channel number (0-based)
  @param dimm   - DIMM number (0-based)
  @param rank   - Rank number (0-based)
  @param data   - Data for the MRS write
  @param bank   - Bank address for the MRS write
  @param side   - Side

  @retval None

**/
VOID
WriteMrsDdr4 (
  PSYSHOST Host,      // Pointer to the system Host (root) structure
  UINT8    socket,    // Socket Id
  UINT8    ch,        // Channel number (0-based)
  UINT8    dimm,      // DIMM number (0-based)
  UINT8    rank,      // Rank number (0-based)
  UINT32   data,      // Data for the MRS write
  UINT8    bank,      // Bank address for the MRS write
  UINT8    side       // Side
  );

/**

  Writes a nibble to RDIMM/LRDIMM register control words

  @param Host             - Pointer to sysHost, the system Host (root) structure
  @param socket           - Socket Id
  @param ch               - Channel number (0-based)
  @param dimm             - DIMM number (0-based)
  @param rank             - Rank number (0-based)
  @param controlWordData  - Control Word data (a nibble)
  @param controlWordAddr  - Control Word Address

  @retval None

**/

VOID
WriteRC (
  PSYSHOST  Host,                 // Pointer to the system Host (root) structure
  UINT8     socket,               // Socket Id
  UINT8     ch,                   // Channel number (0-based)
  UINT8     dimm,                 // DIMM number (0-based)
  UINT8     rank,                 // Rank number (0-based)
  UINT8     controlWordData,      // Control Word data (a nibble)
  UINT8     controlWordAddr       // Control Word Address
  );

/**

  Writes a nibble or byte to DDR4 Buffer Control

  @param Host              - Pointer to sysHost, the system Host (root) structure
  @param socket            - Socket Id
  @param ch                - Channel number (0-based)
  @param dimm              - DIMM number (0-based)
  @param rank              - Rank number (0-based)
  @param controlWordData   - Control Word data (a nibble or byte)
  @param controlWordFunc   - Control Word Function
  @param controlWordAddr   - Control Word Address

  @retval None

**/

VOID
WriteBC (
  PSYSHOST  Host,               // Pointer to the system Host (root) structure
  UINT8     socket,             // Socket Id
  UINT8     ch,                 // Channel number (0-based)
  UINT8     dimm,               // DIMM number (0-based)
  UINT8     rank,               // Rank number (0-based)
  UINT8     controlWordData,    // Control Word data (a nibble or byte)
  UINT8     controlWordFunc,    // Control Word Function
  UINT8     controlWordAddr     // Control Word Address
  );

/**

  Convert ODT MR register bits to readable ohms or vice verse.

  @param[in]   Group       - CSM_GT signal group to program.
  @param[in]   Value       - Decoded RTT value in unit of omhs.
  @param[in]   Operation   - Encode or Decode RTT value.

  @retval The coverted value.
*/
UINT16
EFIAPI
ConvertOdtValueDdr5 (
  IN MRC_GT    Group,
  IN UINT16    Value,
  IN UINT8     Operation
  );

/**

  Convert given value to and from ODT value

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param group    - Group type
  @param value    - Value from Register/Cache
  @param code     - CODE or DECODE ODT
  @param dimmtype - Dimm type

  @retval The converted value

**/
UINT16
ConvertOdtValue (
  PSYSHOST  Host,            // Pointer to sysHos
  MRC_GT    group,           // Group type
  UINT16    value,           // Value from Register/Cache
  UINT8     code,            // CODE or DECODE ODT
  UINT8     dimmtype         // Dimm type
  );

/**
  Read or update multiple ODT values from/to the MRC cache

  @param[in]      Host    - Pointer to sysHost
  @param[in]      Socket  - Socket number
  @param[in]      Ch      - Channel number (0-based)
  @param[in]      Dimm    - DIMM number (0-based)
  @param[in]      Rank    - Rank number (0-based)
  @param[in]      Strobe  - Strobe number (0-based)
  @param[in]      Bit     - Bit number
  @param[in]      Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]      Group   - CSM_GT - Parameter to program
  @param[in]      Mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param[in][out] Value   - Encoded RTT bits for MR register.

  @retval MRC_STATUS_SUCCESS    The function executes sucessfully.
  @retval MRC_STATUS_GSM_LIMIT  There is invalid parameter.

*/
MRC_STATUS
EFIAPI
GetSetDimmRttRonDdr5 (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     Dimm,
  IN     UINT8     Rank,
  IN     UINT8     Strobe,
  IN     UINT8     Bit,
  IN     MRC_LT    Level,
  IN     MRC_GT    Group,
  IN     UINT8     Mode,
  IN OUT INT16     *Value
  );

/**
  Read or update multiple ODT values from/to the MRC cache

  @param[in]      Host    - Pointer to sysHost
  @param[in]      Socket  - Socket number
  @param[in]      Ch      - Channel number (0-based)
  @param[in]      Dimm    - DIMM number (0-based)
  @param[in]      Rank    - Rank number (0-based)
  @param[in]      Strobe  - Strobe number (0-based)
  @param[in]      Bit     - Bit number
  @param[in]      Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]      Group   - CSM_GT - Parameter to program
  @param[in]      Mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param[in][out] Value   - Decoded RTT value in unit of omhs.

  @retval MRC_STATUS_SUCCESS    The function executes sucessfully.
  @retval MRC_STATUS_GSM_LIMIT  There is invalid parameter.

*/
MRC_STATUS
EFIAPI
GetSetDimmOdtDdr4 (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     Dimm,
  IN     UINT8     Rank,
  IN     UINT8     Strobe,
  IN     UINT8     Bit,
  IN     MRC_LT    Level,
  IN     MRC_GT    Group,
  IN     UINT8     Mode,
  IN OUT INT16     *Value
  );

/**

  Encodes tRTP/tWR value to MR0 field

  @param Value - tRTP/tWR value in tCK units
  @param Twr   - FALSE = tRTP; TRUE = tWR
  @param *Mr0Mask  - MR0 bit mask of tRTP/tWR field
  @param *Mr0Value - MR0 encoded value of tRTP/tWR field

  @retval SUCCESS/FAILURE

**/
UINT32
EncodeMr0TrtpTwrDdr4 (
  UINT8     Value,
  BOOLEAN   Twr,
  UINT16    *Mr0Mask,
  UINT16    *Mr0Value
);

/**

  Decodes tRTP/tWR value from MR0 field

  @param Twr      - FALSE = tRTP; TRUE = tWR
  @param Mr0Value - MR0 encoded value of tRTP/tWR field
  @param *Value   - tRTP/tWR value in tCK units

  @retval SUCCESS/FAILURE

**/
UINT32
DecodeMr0TrtpTwrDdr4 (
  BOOLEAN   Twr,
  UINT16    Mr0Value,
  UINT8     *Value
);

/**

  This function dispatches the slave processors to start MemMain

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS - Memory initialization completed successfully.
  @retval FAILURE - A reset is required

**/
UINT32
DispatchSlaves (
  PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
  );

/**

  This function handles SBSP dispatch and the slave processors acknowledgement

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS - Memory initialization completed successfully.
  @retval FAILURE - A reset is required

**/
UINT32
ParallelModeDispatch (
  PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
  );

/**

  This function syncs data across sockets

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS: Sync successfull

**/
UINT32
PipeSync (
  PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
  );

/**

  This function gets data from slave processors

  @retval SUCCESS

**/
UINT32
GetSlaveData (
  VOID
  );

/**

  This function sends data to slave processors

  @retval SUCCESS

**/
UINT32
SendData (
  VOID
  );

/**

  Send the status variable to each socket

  @retval 0 - no reset
  @retval 2 - Power Good
  @retval 3 - Warm reset

**/
UINT32
SendStatus (
  VOID
  );

/**

  This function dispatches the slave processors to start MemMain

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS:  Memory initialization completed successfully.
  @retval FAILURE:  A reset is required

**/
UINT32
SlaveAck (
  PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
  );

/**

  Train IMode

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
TrainImode (
  PSYSHOST  Host    // Pointer to the system Host (root) structure
  );

UINT32
TrainTxEq (
  PSYSHOST  Host    // Pointer to the system Host (root) structure
  );

/**
  Optimizes Power Margin by margining Tx Dq eye width and height at various settings for RxDfe

  @param[in]      Host  - Pointer to sysHost structure

  @retval SUCCESS
**/
UINT32
EFIAPI
TrainLegacyRxDfe (
  IN   PSYSHOST Host
  );

/**
  Optimizes Tx ECHO by margining Tx Dq eye width and height at various settings for Tx ECHO.

  @param[in]      Host  - Pointer to sysHost structure

  @retval SUCCESS
**/
UINT32
EFIAPI
TrainTxEcho (
  IN   PSYSHOST Host
  );

/**
  This function implements the TX Phase-based Xtalk Cancellation (PXC) algorithm.

  @param[in] Host     - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
TrainPXC (
  IN     PSYSHOST   Host
  );

/**
  This function implements the TX Data Dependent Jitter Cancellation (DDJC) algorithm.

  @param[in] Host     - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
TrainTxDdjc (
  IN     PSYSHOST   Host
  );

/**
  Optimizes voltage and timing margins by training the best Rx
  DFE settings.

  @param[in]      Host  - Pointer to sysHost structure

  @retval SUCCESS
**/
UINT32
TrainRxDfe (
  IN     PSYSHOST   Host
  );

/**
  Optimizes CMD voltage and timing margins by training the best CMD TXEQ setting.

  @param[in]      Host  - Pointer to sysHost structure

  @retval SUCCESS
**/
UINT32
TrainCmdTxEq (
  IN     PSYSHOST  Host    // Pointer to the system Host (root) structure
  );

/**

  Determines if this rank is valid

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param socket - socket number
  @param ch     - channel number
  @param dimm   - dimm number
  @param rank   - rank number
  @param flags  - CHECK_MAPOUT

  @retval SUCCESS - Rank is valid
  @retval FAILURE - Rank is not valid

**/
UINT8
CheckRank (
  PSYSHOST Host,                // Pointer to the system Host (root) structure
  UINT8    socket,              // socket number
  UINT8    ch,                  // channel number
  UINT8    dimm,                // dimm number
  UINT8    rank,                // rank number
  UINT8    flags                // CHECK_MAPOUT
  );

/**

  Basic Fine ERID training uses a HW generated pattern from NVMCTLR

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
EarlyReadID (
  PSYSHOST  Host    // Pointer to the system Host (root) structure
  );

/**

  Read and return a linear date based on year, month, and day of the month from RTC

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval linear approximate # of day's since Jan 1 2000

**/
UINT16
MemGetCmosDate (
  PSYSHOST  Host    // Pointer to the system Host (root) structure
  );

/**
Populate and validate the memFunctions pointer table based on the CPUID
and execute the high level ProcMemInit flow

@param Host - Pointer to sysHost structure

@retval N/A
**/
VOID
ProcMemInitMain (
  PSYSHOST    Host
  );

/**

  Returns the minimum margin sugested by spec or EV
  recomendation
  @param Host            - Pointer to sysHost
  @param TrainingSignal  - Training signal to be evaluated

  @retval UINT8

**/
UINT8
MinimumMargin (
  PSYSHOST                          Host,
  UINT8                             TrainingSignal
  );

/**
  Return the preamble timing setting

  @param[in] Socket         - Socket number
  @param[in] PreambleType   - READ_PREAMBLE, WRITE_PREAMBLE or READ_OR_WRITE_PREAMBLE

  @retval   PREAMBLE_1TCLK, PREAMBLE_2TCLK
**/
UINT8
GetPreambleState (
  IN      UINT8   Socket,
  IN      UINT8   PreambleType
  );

/**

  Return the Postamble timing setting

  @param[in] Socket         - Socket number
  @param[in] PostambleType  - READ_POSTAMBLE, WRITE_PREAMBLE or READ_OR_WRITE_PREAMBLE

  @retval POSTAMBLE_0P5TCLK, POSTAMBLE_1P5TCLK

**/
UINT8
GetPostambleState (
  IN      UINT8   Socket,
  IN      UINT8   PostambleType
  );

/**
  Return the preamble timing in tCK units

  @param[in] Socket         - Socket number
  @param[in] PreambleType   - READ_PREAMBLE, WRITE_PREAMBLE or READ_OR_WRITE_PREAMBLE

  @retval   tWPRE in tCK units
**/
UINT8
GetTWpre (
  IN      UINT8   Socket,
  IN      UINT8   PreambleType
  );

/**
  Indicate if long (2TCLK) preamble is allowed by the configuration

  @param[in] Socket         - Socket number

  @retval   TRUE if long preamble allowed
            FALSE otherwise
**/
BOOLEAN
LongPreambleAllowed (
  IN      UINT8   Socket
  );

/**

  Set First Bus Number in requested Socket
  @param[in] DimmType
  @param[in] StringBuffer

**/
CHAR8 *
GetDimmTypeStr (
  IN  UINT8 DramType,
  IN  UINT8 DimmType,
  OUT CHAR8 *StringBuffer
  );

/**
  Return TRUE if the DIMM has the A17 signal enabled

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket Id
  @param[in] Ch       - Channel number (0-based)
  @param[in] Dimm     - DIMM number (0-based)

  @retval   True      This DIMM has A17 Enabled
  @retval   False     This DIMM does not have A17 Enabled

**/
BOOLEAN
A17Enabled (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    Dimm
  );

/**
  Return TRUE if the DIMM has the A17 signal enabled on Backside

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket Id
  @param[in] Ch       - Channel number (0-based)
  @param[in] Dimm     - DIMM number (0-based)

  @retval   True      This DIMM has A17 Enabled on Backside
  @retval   False     This DIMM does not have A17 Enabled on Backside

**/
BOOLEAN
A17EnabledBackside (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    Dimm
  );


/*++
  Get the maximum number of data groups based on DIMM and group type

  @param Host  - Pointer to sysHost
  @param socket  - Socket
  @param MemSsType - Memory Subsystem Type

  @retval Max channels

--*/
UINT32
GetMaxCh (
  PSYSHOST  Host,
  UINT8     Socket,
  MRC_MST   MemSsType
  );

/**

  MRC wrapper code.

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval None

**/
VOID
MemoryInit (
  PSYSHOST Host        // Pointer to the system Host (root) structure
  );

/**

  Get the Mem Nvram pointer from Host

  @param None

**/
MEM_NVRAM_STRUCT *
GetMemNvramPointer (
  VOID
  );

/**

  Get the Mem Var pointer from Host

  @param None

**/
MEM_VAR_STRUCT *
GetMemVarPointer (
  VOID
  );

/**
  Returns the starting offset for the given test group

  @param[in] Host            - Pointer to sysHost
  @param[in] socket          - Processor socket to check
  @param[in] group           - Test group
  @param[in] level           - DDR or LRDIMM Host side, LRDIMM Backside, etc...
  @param[in] scope           - Margin granularity
  @param[in] marginData      - Pointer to the structure to store the margin results. The structure type varies based on scope.

  @retval N/A
**/
VOID
EFIAPI
UpdateStartingOffset (
  IN    PSYSHOST    Host,
  IN    UINT8       socket,
  IN    MRC_GT      group,
  IN    MRC_LT      level,
  IN    UINT8       scope,
  IN    VOID        *marginData
  );

/**
  Set the starting offsets for CMD timging margin sweeps to be close
  to the failing edges found during training. The purpose is to
  reduce time spent margining CMD timing.
  This routine will be called after Early and Late CMD Training.

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number
  @param[in]  Clk       - Clock number
  @param[in]  Margins   - Pointer to structure with left and right edges

  @retval   EFI_SUCCESS if starting offsets are updated
            EFI_INVALID_PARAMETER otherwise

**/
EFI_STATUS
EFIAPI
UpdateStartingOffsetCmd (
  IN    UINT8               Socket,
  IN    UINT8               Ch,
  IN    UINT8               Clk,
  IN    struct baseMargin   *Margins
  );

/**
  Return pointer to string for a training group

  @param[in]  Group     - The enumerated group value
  @param[out] String    - Pointer to caller's string pointer to be updated

  @retval   EFI_SUCCESS if String is updated
            EFI_NOT_FOUND otherwise
**/
EFI_STATUS
EFIAPI
GetGroupString (
  IN    MRC_GT    Group,
  OUT   CHAR8     **String
  );

/**
  Return pointer to training group string
  This version does not take a string pointer from the caller.
  It can be used inside debug print statements without using a local variable.

  @param[in]  Group   - Training group number

  @return  Pointer to training group string

**/
CHAR8 *
EFIAPI
GetGroupStr (
  IN    MRC_GT  Group
  );

/**

  Get Tx Flyby Delay

  @param[in]  Host    - Pointer to sysHost
  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number (0-based)
  @param[in]  Dimm    - DIMM number (0-based)
  @param[in]  Rank    - Rank number (DIMM rank number)
  @param[in]  Strobe  - Strobe number (0-based)
  @param[in]  Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]  Mode    - Bit-field of different modes
  @param[out] Value   - Pointer to delay value or offset based on mode

  @retval MRC_STATUS_SUCCESS if value found
          MRC_STATUS_LEVEL_NOT_SUPPORTED if Level != DdrLevel
          MRC_STATUS_STROBE_NOT_PRESENT if Strobe number is invalid
          MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
EFIAPI
GetTxFlyByCore (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  IN    UINT8     Dimm,
  IN    UINT8     Rank,
  IN    UINT8     Strobe,
  IN    MRC_LT    Level,
  IN    UINT8     Mode,
  OUT   INT16     *Value
  );

/**

  Get Rx Flyby Delay

  @param[in]  Host    - Pointer to sysHost
  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number (0-based)
  @param[in]  SubCh   - Sub Channel number (0-based)
  @param[in]  Dimm    - DIMM number (0-based)
  @param[in]  Rank    - Rank number (DIMM rank number)
  @param[in]  Strobe  - Strobe number (0-based)
  @param[in]  Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]  Mode    - Bit-field of different modes
  @param[out] Value   - Pointer to delay value or offset based on mode

  @retval MRC_STATUS_SUCCESS if value found
          MRC_STATUS_LEVEL_NOT_SUPPORTED if Level != DdrLevel
          MRC_STATUS_STROBE_NOT_PRESENT if Strobe number is invalid
          MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
EFIAPI
GetRxFlyByCore (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  IN    UINT8     SubCh,
  IN    UINT8     Dimm,
  IN    UINT8     Rank,
  IN    UINT8     Strobe,
  IN    MRC_LT    Level,
  IN    UINT8     Mode,
  OUT   INT16     *Value
  );

/**

  RegisterSimStatusCodeHandler

  @retval None

**/
VOID
RegisterSimStatusCodeHandler (
  VOID
  );

/**

  UnregisterSimStatusCodeHandler

  @retval None

**/
VOID
UnregisterSimStatusCodeHandler (
  VOID
  );

/**
  Determine if the given memory subsystem is DDR type

  @param[in]      MsmListPtr          - Pointer to MemSs structure

  @retVal 1 for DDR type
  @retVal 0 for other type
**/
BOOLEAN
IsMemSsDdrType (
  IN      MRC_MSM       *MsmListPtr
  );

/**
  Determine the maximum number of strobes per DDR channel in current socket

  @param[in]      Host          Pointer to sysHost structure

  @retval UINT32  Number of strobes per DDR channel
**/
UINT8
GetMaxStrobeValid (
  IN      PSYSHOST        Host
  );

/**
  Determine the maximum number of strobes per DDR Subchannel in current Socket for DDR5.

  @param[in]      Host          Pointer to sysHost structure

  @retval UINT32  Number of strobes per DDR subchannel
**/
UINT8
GetSubChMaxStrobeValid (
  IN PSYSHOST        Host
  );

/**
  Determine the maximum number of DQ bits supported per DDR channel in current socket

  @param[in]      Host          - Pointer to sysHost

  @retVal UINT8   Number of DQ bits per DDR channel
**/
UINT8
GetMaxBitsValid (
  IN      PSYSHOST          Host
  );

/**
  Determine if the given Strobe number is valid for ECC mode on current socket

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Strobe        - Strobe number to check

  @retVal   1     Strobe number is not valid
  @retVal   0     Strobe number is valid
**/
BOOLEAN
IsStrobeNotValid (
  IN      PSYSHOST        Host,
  IN      UINT8           Strobe
  );

/**
  Determine if the given Dimm and Strobe number is valid for ECC mode on current socket for DDR5.

  @param[in]  DimmNvramStructPtr  Pointer to dimmNvram structure to check
  @param[in]  Strobe              Strobe number to check

  @retVal   TRUE    Strobe number is not valid
  @retVal   FALSE   Strobe number is valid
**/
BOOLEAN
EFIAPI
IsDimmStrobeNotValid (
  IN struct dimmNvram *DimmNvramStructPtr,
  IN UINT8            Strobe
  );

/**
  Determine if the given DQ nibble number is valid for ECC mode on current socket

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Nibble        - DQ nibble number to check

  @retVal   1     DQ nibble number is not valid
  @retVal   0     DQ nibble number is valid
**/
BOOLEAN
IsNibbleNotValid (
  IN      PSYSHOST        Host,
  IN      UINT8           Nibble
  );

/**
  Determine if the given DQ bit number is valid for ECC mode on current socket

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Bit           - DQ bit number to check

  @retVal   1     DQ bit number is not valid
  @retVal   0     DQ bit number is valid
**/
BOOLEAN
IsBitNotValid (
  IN      PSYSHOST        Host,
  IN      UINT8           Bit
  );

/**
  Determines the DQ nibble number associated with the given strobe number

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Strobe        - Strobe number

  @retVal   UINT8   DQ nibble number
**/
UINT8
GetNibbleFromStrobe (
  IN      PSYSHOST        Host,
  IN      UINT8           Strobe
  );

/**
  Determines the strobe number associated with the given DQ nibble number

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Nibble        - DQ nibble number

  @retVal   UINT8   strobe number
**/
UINT8
GetStrobeFromNibble (
  IN      PSYSHOST        Host,
  IN      UINT8           Nibble
  );

/**
  Determines a bit mask of valid strobe numbers for ECC mode on current socket

  @param[in]      Host          - Pointer to sysHost

  @retVal   UINT32    Bit mask of valid Strobe numbers
**/
UINT32
GetStrobeMask (
  IN      PSYSHOST        Host
  );

/**

  Get Strobe Mapping based on a SubChannel and current strobe within the SubChannel

  DDR5:
  Strobe 0 -  9 in SUCH 0 are logical strobes 00 - 09
  Strobe 0 -  9 in SUCH 1 are logical strobes 10 - 19

  DDR4:
  Strobe 0 - 17 in CH 0 are logical strobes 00 - 17 (SubChannel must be always '0')


  @param[in] Channel      Channel number
  @param[in] SubChannel   Sub Channel number
  @param[in] Strobe       Strobe within the SubChannel

  @return    Strobe Mapping

**/
UINT8
GetStrobeMapping (
  IN     UINT8   Channel,
  IN     UINT8   SubChannel,
  IN     UINT8   Strobe
  );

/**

  Displays per bit margin results

  @param Host        - Pointer to sysHost, the system Host (root) structure
  @param socket      - current socket
  @param resultsBit  - Per bit margin results
  @param group       - Command group

  @retval None

**/
VOID
DisplayPerBitMargins (
  PSYSHOST          Host,           // Pointer to the system Host (root) structure
  UINT8             socket,         // current socket
  struct bitMargin  *resultsBit,    // Per bit margin results
  MRC_GT            group,          // Command group
  MRC_LT            level           // DDR or LRDIMM Host side, LRDIMM Backside, etc...
  );

MRC_STATUS
GetTrainResultStatus (
  PSYSHOST    Host,    // Pointer to Host structure (outside scope of this spec)
  UINT8       Socket,    // Processor socket within the system (0-based)
  UINT32      MemSs,               // MemSS number within the socket (0-based)
  UINT32      ChannelMask,  // Channel within MemSS (0-based)
  UINT8       SubChMask,    // Sub channel mask
  UINT32      StatusArraySize,  // Input giving status array size
  UINT32      *Status,  // Pointer to caller buffer to be filled in with results
  UINT32      *ValidMask, // Each bit represents valid entry in Status array
  UINT8       ChunkMask,   // A set bit indicates the chunk of the cache line will be included
  UINT8       Dimm
  );

/**

  The function gets number of error samples and finds the middle of zero error

  @param Host              - Pointer to sysHost, the system Host (root) structure
  @param socket            - Processor socket to check
  @param ch                - Current Channel
  @param dimm              - Dimm number (0-based)
  @param rank              - Rank number (0-based)
  @param strobe            - Strobe number (0-based)
  @param SdRamErrorSamples - Array with 64 sampes for each dqs pi setting
  @param ErrorSamplesSize  - Include how many sample we have in the array 72 or 64
  @param errorStart        - Pointer to location for error start
  @param errorEnd          - Pointer to location for error end
  @param piPosition        - Pointer to the PI Position

  @retval Status

**/
UINT32
FindErrorGraphMin (
  PSYSHOST  Host,                   // Pointer to the system Host (root) structure
  UINT8     socket,                 // Processor socket to check
  UINT8     ch,                     // Current Channel
  UINT8     dimm,                   // Dimm number (0-based)
  UINT8     rank,                   // Rank number (0-based)
  UINT8     strobe,                 // Strobe number (0-based)
  UINT8     *SdRamErrorSamples,     // Array with 64 sampes for each dqs pi setting
  UINT16    ErrorSamplesSize,       // Include how many sample we have in the array 72 or 64
  UINT16    *errorStart,            // Pointer to location for error start
  UINT16    *errorEnd,              // Pointer to location for error end
  UINT16    *piPosition             // Pointer to the PI Position
  );

VOID
SetRankMPR (
  PSYSHOST Host,            // Pointer to the system Host (root) structure
  UINT8    socket,          // Socket Id
  UINT8    ch,              // Channel number (0-based)
  UINT8    dimm,            // Dimm number (0-based)
  UINT8    rank,            // Rank number (0-based)
  UINT8     mprMode         // Either 0 for Disable or MR3_MPR to Enable
  );

/**

  Detects Low Margins per Training step

  @param Host            - Pointer to sysHost
  @param Channel         - Channel
  @param Dimm            - Dimm
  @param Rank            - Rank
  @param MarginData      - Margin Data to review
  @param TrainingSignal  - Training signal to review margin on
  @param Flags           - Differentiate the same margin signal at different MRC training

  @retval UINT8

**/
UINT8
DetectLowMargin (
  PSYSHOST                          Host,
  UINT8                             Channel,
  UINT8                             Dimm,
  UINT8                             Rank,
  VOID                              *MarginData,
  UINT8                             TrainingSignal,
  UINT8                             Flags
  );

/**

  This function find the logic delay max and logic delay min

  @param Host            - Pointer to sysHost, the system Host (root) structure
  @param socket          - Processor socket to check
  @param ch              - Current Channel
  @param dimm            - Dimm number (0-based)
  @param rank            - Rank number (0-based)
  @param MaxSdramInDimm  - Number of sdram in the current DIMM
  @param MaxLogicDelay   - Return logic delay max value
  @param MinLogicDelay   - Return logic delay min value

  @retval None

**/
VOID
FindMaxMinLogic (
  PSYSHOST  Host,                 // Pointer to the system Host (root) structure
  UINT8     socket,               // Processor socket to check
  UINT8     ch,                   // Current Channel
  UINT8     dimm,                 // Dimm number (0-based)
  UINT8     rank,                 // Rank number (0-based)
  UINT8     MaxSdramInDimm,       // Number of sdram in the current DIMM
  UINT8     *MaxLogicDelay,       // Return logic delay max value
  UINT8     *MinLogicDelay        // Return logic delay min value
  );

/**

  Displays the DRAM manufacturer

  @param Host  - Pointer to sysHost
  @param MfgId - DRAM Manufacturer Id

  @retval N/A

**/
VOID
DisplayManf (
  PSYSHOST Host,
  UINT16 MfgId
  );


/**

  This function returns the reset status

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
CheckStatus (
  PSYSHOST Host
  );

/**

  This function checks if Vdd needs to be set

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
CheckVdd (
  PSYSHOST Host
  );

/**
  Get MC BCLK.

  @param Socket           - Socket number

  @retval - BCLK Frequency for given Socket

**/
UINT8
EFIAPI
GetBclkFreq (
  IN  UINT8    Socket
);

/**

  Determines which frequency system needs to run
  across differentt memory sub systems and programs it


  @param Host  - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS - Clock is set properly
  @retval FAILURE - A reset is required to set the clock

**/
UINT32
InitClocks (
  PSYSHOST Host
  );

/**

  Initialize rank structures.  This is based on the requested
  RAS mode and what RAS modes the configuration supports. This
  routine also evalues the requested RAS modes to ensure they
  are supported by the system configuration.

  @param Host  - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
CheckDimmRanks (
  PSYSHOST Host
  );

/**



  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS
  @retval FAILURE

**/
UINT32
PostPackageRepairMain (
  PSYSHOST  Host
  );

/**

  Routine that runs Post Package Repair for all valid entries in error address list

  @param[in]          Host            Pointer to SYSHOST
  @param[in]          Socket          Socket number
  @param[in]          ChBitMask       Bit mask of enabled channels
  @param[in]          PprTypeIn       Hard or Soft PPR type
  @param[in, out]     *SoftPprDone    Global status of Soft PPR repair done
  @param[in]          NumEntries      Number of entries in PprAddrSetup and PprStatusOut arrays
  @param[in]          PprAddrSetup    Array of DRAM addresses to repair
  @param[out]         *PprStatusOut   Array of repair status for each address in PprAddrSetup array

  @retval status   - SUCCESS / FAILURE

**/
UINT32
PostPackageRepair (
  IN      PSYSHOST Host,
  IN      UINT8 Socket,
  IN      UINT32 ChBitMask,
  IN      UINT8 PprTypeIn,
  IN OUT  UINT8 *SoftPprDone,
  IN      UINT32 NumEntries,
  IN      PPR_ADDR_MRC_SETUP *PprAddrSetup,
  OUT     UINT8 *PprStatusOut
);

/**

  Determine if a given dimm supports programable CTLE / Rx Eq.  Data from Intel PMO

  @param[in] Host        - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket      - Socket Id
  @param[in] Ch          - Channel number
  @param[in] Dimm        - DIMM number (0-based)

  @retval:   TRUE if input Dimm supports programable CTLE / Rx Eq
  @retval:   FALSE if input Dimm does NOT support programable CTLE / Rx Eq

**/
BOOLEAN
EFIAPI
DimmSupportForProgrammableCtle (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     Dimm
  );

/**
  Multi-use function to either get or set clock delays based on the provided clock number

  @param[in]      Host      Pointer to SYSHOST
  @param[in]      Socket    Socket number
  @param[in]      Ch        Channel number
  @param[in]      Dimm      Dimm number
  @param[in]      Level     Level of the bus
  @param[in]      Clk       Clock number
  @param[in]      Mode      GSM_READ_CSR - Read the data from hardware and not cache
                            GSM_READ_ONLY - Do not write
                            GSM_WRITE_OFFSET - Write offset and not value
                            GSM_FORCE_WRITE - Force the write
  @param[in, out] Value     Value to program

  @retval MRC_STATUS_SUCCESS    All operations completed successfully
  @retval MRC_STATUS_FAILURE    Failure(s) occurred during operation
**/
MRC_STATUS
GetSetClkDelayCore (
  IN     PSYSHOST Host,
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
  IN     MRC_LT   Level,
  IN     UINT8    Clk,
  IN     UINT8    Mode,
  IN OUT INT16    *Value
  );

/**

  Multi-use function to either get or set control delays based on the provided group number

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket number
  @param ch       - Channel number
  @param dimm     - Dimm number
  @param level    - Level of the bus
  @param group    - Group number
  @param mode     - GSM_READ_CSR - Read the data from hardware and not cache
                    GSM_READ_ONLY - Do not write
                    GSM_WRITE_OFFSET - Write offset and not value
                    GSM_FORCE_WRITE - Force the write
  @param value    - Value to program
  @param *minVal  - Current minimum control delay
  @param *maxVal  - Current maximum control delay

  @retval status

**/
MRC_STATUS
GetSetCtlGroupDelayCore (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  MRC_LT level,
  MRC_GT group,
  UINT8 mode,
  INT16 *value,
  UINT16 *minVal,
  UINT16 *maxVal
  );

/**

  Return the time required for the parity recovery for DDR4 in DCLKs

  @param[in]    Socket        Current socket under test (0-based)
  @param[in]    RatioIndex    Current ratio index (frequency)

  @return       return the parity recovery in DCLKs

**/
UINT32
EFIAPI
GetParityRecoveryDdr4 (
  IN        UINT8           Socket,
  IN        UINT8           RatioIndex
);

/**
  Multi-use function to either get or set command delays based on the provided group number

  @param[in]      Host      Pointer to SYSHOST
  @param[in]      Socket    Socket number
  @param[in]      Ch        Channel number
  @param[in]      Dimm      Dimm number
  @param[in]      Level     Level of the bus
  @param[in]      Group     Group number
  @param[in]      Mode      GSM_READ_CSR - Read the data from hardware and not cache
                            GSM_READ_ONLY - Do not write
                            GSM_WRITE_OFFSET - Write offset and not value
                            GSM_FORCE_WRITE - Force the write
  @param[in, out] Value     Value to program
  @param[in, out] *MinVal   Current minimum command delay
  @param[in, out] *MaxVal   Current maximum command delay

  @retval MRC_STATUS_SUCCESS    All operations completed successfully
  @retval MRC_STATUS_FAILURE    Failure(s) occurred during operation
**/
MRC_STATUS
GetSetCmdGroupDelayCore (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value,
  IN OUT  UINT16    *MinVal,
  IN OUT  UINT16    *MaxVal
  );

/**

  Multi-use function to either get or set command vref

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket number
  @param ch       - Channel number
  @param dimm     - Dimm number
  @param level    - Level of the bus
  @param mode     - GSM_READ_CSR - Read the data from hardware and not cache
                    GSM_READ_ONLY - Do not write
                    GSM_WRITE_OFFSET - Write offset and not value
                    GSM_FORCE_WRITE - Force the write
  @param value    - Value to program or offset

  @retval status

**/
MRC_STATUS
GetSetCmdVrefCore (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  MRC_LT level,
  UINT8 mode,
  INT16 *value
  );

/**

  Determine if Backside CMD Margin is enabled for DDR4

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket number

  @retval TRUE  - Backside CMD Margin is enabled
  @retval FALSE - Backside CMD Margin is not enabled

**/
BOOLEAN
IsDdr4BacksideCmdMarginEnabled (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  );

/**

  Determine if the DDR4 register is Rev 2

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param socket - Socket number
  @param ch     - Channel number
  @param dimm   - Dimm number

  @retval TRUE - Register is Rev 2
  @retval FALSE - Register is not Rev 2

**/
UINT8
IsDdr4RegisterRev2 (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm
  );

/**
  Function to shift backside Cmd relative to Clk & Ctl to maintain Cmd - Clk = trained offset
  and shift TxDq and TxDqs by +/- 64 depending on direction

  @param[in] Host       Pointer to SYSHOST
  @param[in] Socket     Socket number
  @param[in] Direction  +/- direction to move timings

  @retval SUCCESS
**/
UINT32
BacksideShift (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  INT8      Direction
  );

#ifdef LRDIMM_SUPPORT
/**

  Re-centers using a more robust test

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
TxLRDIMMDqCentering (
  PSYSHOST Host
  );
#endif // LRDIMM_SUPPORT

/**

  Optimizes RX CTLE settings by margining Rx Dqs eye width and height at various settings for the RX CTLE controls.

  TrainCtleLegacy sweeps RxEq, CTLE-C and CTLE-R together. This is the original implementation of CTLE training.
  The new version - TrainCtle - sweeps RxEq separately before sweeping CTLE-C and CTLE-R, resulting in
  potentially better RX CTLE settings, but at the cost of additional execution time.

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
TrainCtleLegacy (
  PSYSHOST Host
  );

/**

  Optimizes RX CTLE settings by margining Rx Dqs eye width and height at various settings for the RX CTLE controls.

  TrainCtle sweeps RxEq first, then CTLE-C and CTLE-R together. This version of CTLE training takes longer to execute
  than TrainCtleLegacy, but may result in improved RX CTLE settings compared to TrainCtleLegacy.

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
TrainCtle (
  PSYSHOST Host
  );

/**

  Train Tx Rise Fall Slew Rate

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
TrainTxRiseFallSlewRate (
  PSYSHOST Host
  );

/**

  Train TCO COMP

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
TrainTcoComp (
  PSYSHOST Host
  );

/**

  Train McRON

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
TrainMcRon (
  PSYSHOST Host
  );

/**

  Train McODT

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
TrainMcOdt (
  PSYSHOST Host
  );

/*

 This function is used to verify if the bit swizzling used for Rx and Tx is correct.
 This function does not do the check automatically, but it shows data margin on per bits
 basis in the serial to inspect if the swizzling look correct.

 @param[in]         Host        Pointer to structure

 @retval            SUCCESS     Always return success execution

*/
UINT32
BitSwizzlingInspection (
  IN        PSYSHOST    Host
  );

/**

  Train DRAM RON

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
TrainDramRon (
  PSYSHOST Host
  );

/**

  Train DRAM Rx Eq

  @param[in] Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
EFIAPI
TrainDramRxEq (
  IN PSYSHOST Host
  );

/**

  Perform DDR4 LRDIMM Backside Read Training (RX)

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
Ddr4LrdimmBacksideRxPhase (
  PSYSHOST Host
  );

/**

  Perform DDR4 LRDIMM Backside Read Training (RX)

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
Ddr4LrdimmBacksideRxCycle (
  PSYSHOST Host
  );

/**

  Perform DDR4 LRDIMM Backside Read Training (RX)

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
Ddr4LrdimmBacksideRxDelay (
  PSYSHOST Host
  );

/**

  Perform LRDIMM Backside Training (TX)

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
Ddr4LrdimmBacksideTxFineWL (
  PSYSHOST Host
  );

/**

  Perform LRDIMM Backside Training (TX)

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
Ddr4LrdimmBacksideTxCoarseWL (
  PSYSHOST Host
  );

/**

  Perform LRDIMM Backside Training (TX)

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
Ddr4LrdimmBacksideTxDelay (
  PSYSHOST Host
  );

/**

  Re-centers using a more robust test

  @param host - Pointer to sysHost, the system host (root) structure

  @retval SUCCESS

**/
UINT32
WrLRDIMMVrefCentering (
  PSYSHOST  Host
  );

/**

  Re-centers using a more robust test

  @param host - Pointer to sysHost, the system host (root) structure

  @retval SUCCESS

**/
UINT32
RdLRDIMMVrefCentering (
  PSYSHOST Host
  );

/**

  Re-centers using a more robust test

  @param host - Pointer to sysHost, the system host (root) structure

  @retval SUCCESS

**/
UINT32
RxLRDIMMDqCentering (
  PSYSHOST Host
  );

#ifdef LRDIMM_SUPPORT
/*++

  Data Buffer DFE training for LRDIMM

  @param Host                - Pointer to sysHost

  @retval SUCCESS

--*/
UINT32
TrainLrdimmDataBufferDfe (
  IN  PSYSHOST    Host
  );

/**
  This function restore Data Buffer DFE Coefficient Setting

  @param[in] Host     - Pointer to sysHost
  @param[in] Channel  - Channel number

  @retval SUCCESS

**/
MRC_STATUS
EFIAPI
RestoreDBDfeCoef(
  IN  PSYSHOST    Host,
  IN  UINT8       Channel
  );
#endif //LRDIMM_SUPPORT

/**

  Displays information about the DIMMs present

  @param Host - Pointer to sysHost, the system Host (root) structure struct

  @retval SUCCESS

**/
UINT32
DIMMInfo (
  PSYSHOST Host
  );

/**

  This routine prints out memory configuration of each socket at channel level.

  @param Host - Pointer to sysHost, the system Host (root) structure struct

  @retval None

**/
VOID
PrintDimmConfig (
  PSYSHOST Host
  );

/**

  Displays the training results for all parameters

  @param Host - Pointer to sysHost, the system Host (root) structure struct

  @retval SUCCESS

**/
UINT32
DisplayTrainResults (
  PSYSHOST Host
  );

/**

  Displays Advanced Centering data

  @param[in] Host           Pointer to sysHost
  @param[in] Socket         Processor socket
  @param[in] Ch             Channel number
  @param[in] Sub-Ch         SubChannel number
  @param[in] Dimm           DIMM number
  @param[in] Rank           Rank number
  @param[in] Group          Parameter to center
  @param[in] Results        Pointer to the structure to store the margin results
  @param[in] Delay          Pointer to the delay value
  @param[in] OldDelay       Pointer to the old delay value
  @param[in] MaxVref        Max vref
  @param[in] StepSize       Setp size
  @param[in] NumPoints      Num points

  @retval N/A

**/
VOID
DisplayAdvancedCenteringResults (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Subchannel,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN MRC_GT    Group,
  IN VOID      *Results,
  IN INT16     *Delay,
  IN INT16     *OldDelay,
  IN UINT8     MaxVref,
  IN UINT8     StepSize,
  IN UINT8     NumPoints
  );

/**

  Displays And Adjust Vref Centering data

  @param[in] Host            Pointer to sysHost
  @param[in] Socket          Processor socket
  @param[in] Group           Parameter to group
  @param[in] BitResults      Pointer to the bit results
  @param[in] StrobeResults   Pointer to the strobe results
  @param[in] PerBit          Vref training mode perbit

  @retval N/A

**/
VOID
DisplayAndAdjustVrefCenteringResults (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN MRC_GT    Group,
  IN VOID      *BitResults,
  IN VOID      *StrobeResults,
  IN UINT8     PerBit
  );

/**

  Function to offset the final training results to facilitate customer debug.
  After offsetting training results, RMT will be re-run if it is enabled. Final training result
  will be displayed as well.

  @param[in] Host  - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
EFIAPI
OffsetTrainingResult(
  IN PSYSHOST Host
  );

/**

  Function to offset the final RecEn training results before entering the OS that would facilitate customer debug.

  @param[in] Host - Pointer to sysHost, the system host (root) structure struct
  @param[in] Socket - 0 based socket number

  @retval SUCCESS

**/
UINT32
EFIAPI
TrainingResultRecEnOffsetCfg (
  IN PSYSHOST Host,
  IN UINT8    Socket
  );
/**

  Perform main loop of READ DqDqs algorithm.

  @param Host - Pointer to sysHost, the system Host (root) structure struct

  @retval SUCCESS

**/
UINT32
ReadDqDqsPerBit (
  PSYSHOST Host
  );

/**

  DeNormalize all PI's so CMD/CTL's can be margined

  @param Host       - Pointer to sysHost, the system Host (root) structure struct
  @param socket     - current socket
  @param direction  - DENORMALIZE (-1) or RENORMALIZE (1)

  @retval SUCCESS

**/
UINT32
DeNormalizeCCC (
  PSYSHOST Host,
  UINT8 socket,
  INT8 direction
  );

/**

  Returns the physical rank number

  @param dimm - Current dimm
  @param rank - Rank number (0-based)

  @retval UINT8

**/
UINT8
GetCpu (
  PSYSHOST Host,
  UINT8 socket
  );

struct imcNvram (*GetImcNvList (
  PSYSHOST Host,
  UINT8    socket
  ))[MAX_IMC];

struct channelNvram (*GetChannelNvList (
  PSYSHOST Host,
  UINT8 socket))[MAX_CH];

struct dimmNvram (*GetDimmNvList (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch))[MAX_DIMM];

struct ddrRank (*GetRankNvList (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm))[MAX_RANK_DIMM];

struct rankDevice (*GetRankStruct (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm))[MAX_RANK_DIMM];

struct TrainingVariable*
MemTrainingVar (
  VOID
  );

struct DdrChannelTraining (*GetChannelListTraining (
  VOID))[MAX_CH];

struct DimmDeviceTraining (*GetChDimmListTraining (
  UINT8 Ch))[MAX_DIMM];

struct dimmDevice (*GetChDimmList (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch))[MAX_DIMM];

/**
  Displays the Power/Margin Training results

  @param[in]  Host        Pointer to sysHost
  @param[in]  Socket      Processor socket
  @param[in]  SignalName  Signal group name
**/
MRC_STATUS
DisplayPowerTrainingResults (
  PSYSHOST          Host,
  UINT8             Socket,
  MRC_GT            SignalName
  );

/**

  Displays the CMD Vref training results

  @param Host   - Pointer to sysHost, the system Host (root) structure struct
  @param socket - Socket Id

  @retval None

**/
VOID
DisplayCMDVrefResults (
  PSYSHOST Host,
  UINT8 socket
  );

/**

  Displays information about the SPD contents

  @param[in] Socket  - Processor Socket

  @retval N/A

**/
VOID
DisplaySpdContents (
  IN  UINT8  Socket
  );

/**

  Main function used to initialize the memory controler on each processor.

  @param Host              - Pointer to sysHost, the system Host (root) structure struct
  @param ctptrOrg          - Pointer to the CallTable to be used
  @param callTableEntries  - Size of the CallTable
  @param TargetSocket        Socket number to excute the CallTable

  @retval SUCCESS -  Memory initialization completed successfully.
  @retval FAILURE -  A reset is required

**/
UINT32
MemMain (
  PSYSHOST Host,
  CallTableEntryType *CtptrOrg,
  UINT8 CallTableEntries,
  UINT8 TargetSocket
  );

/**

  Detect if current DIMM is LRDIMM

  @param Socket  - Socket Id
  @param Ch      - Channel number (0-based)
  @param Dimm    - Current dimm

  @retval 1 LRDIMM is present
  @retval 0 LRDIMM is not present

**/
UINT8
IsLrdimmPresent (
  UINT8 Socket,
  UINT8 Ch,
  UINT8 Dimm
  );

/**

  Detect if any LRDIMM is present on a given socket

  @param[in] Socket  - Socket Id

  @retval 1 LRDIMM is present
  @retval 0 LRDIMM is not present

**/
UINT8
EFIAPI
IsLrdimmPresentOnSocket (
  IN UINT8 Socket
  );

/**
  Determine whether the socket contains any DIMMs that are not LDRIMMs (RDIMMs, UDIMMs, etc.).

  @param[in] Socket   The socket to search for non-LRDIMMs

  @retval FALSE   All DIMMs on the socket are LRDIMMs
  @retval TRUE    The socket contains at least one DIMM that is not an LRDIMM
**/
BOOLEAN
EFIAPI
IsNonLrdimmPresentOnSocket (
  IN  UINT8 Socket
  );

/**

  Disables the specified DIMM

  @param Host    - Pointer to sysHost, the system Host (root) structure struct
  @param socket  - Socket Id
  @param ch      - Channel number (0-based)
  @param dimm    - Current dimm

  @retval None

**/
VOID
DisableDIMM (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm
  );

/**

  Maps out the specified DIMM

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel Id
  @param[in] Dimm    - DIMM Id
  @param[in] Reason  - Rank map out reason

  @retval None

**/
VOID
MapOutDimm (
  IN PSYSHOST                 Host,
  IN UINT8                    Socket,
  IN UINT8                    Ch,
  IN UINT8                    Dimm,
  IN DIMM_RANK_MAP_OUT_REASON Reason
  );

/**

  Disables the specified rank

  @param Host        - Pointer to sysHost, the system Host (root) structure struct
  @param socket      - Socket Id
  @param ch          - Channel number (0-based)
  @param dimm        - Current dimm
  @param rank        - Rank to disable

  @retval None

**/
VOID
DisableRank (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank
  );

/**

  Maps out the specified rank

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel Id
  @param[in] Dimm    - DIMM Id
  @param[in] Rank    - Rank to map out
  @param[in] Reason  - Rank map out reason

  @retval None

**/
VOID
MapOutRank (
  IN PSYSHOST                 Host,
  IN UINT8                    Socket,
  IN UINT8                    Ch,
  IN UINT8                    Dimm,
  IN UINT8                    Rank,
  IN DIMM_RANK_MAP_OUT_REASON Reason
  );

/**

  Store the common DDR4 DIMM configuration in the Host structure

  @param Host    - Pointer to sysHost, the system Host (root) structure struct
  @param socket  - Socket Id
  @param ch      - Channel number (0-based)
  @param dimm    - Current dimm

  @retval None

**/
VOID
GetCommonDDR4DIMMConfig (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm
  );

/**

  Perform a DLL reset

  @param Host   - Pointer to sysHost, the system Host (root) structure struct
  @param socket - Socket Id

  @retval None

**/
VOID
IssueDllReset (
  PSYSHOST Host,
  UINT8 socket
  );

/**

  This function gets the test engine ready to run tests for Receive Enable Fine training

  @param[in] Host       - Pointer to sysHost, the system Host (root) structure struct
  @param[in] Socket     - Current Socket
  @param[in] ChBitmask  - Bitmask of channels to test
  @param[in] Dimm       - Dimm to test
  @param[in] Rank       - Rank to test

  @retval None

**/
VOID
SetupRecEnFineTest (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT32   ChBitmask,
  IN UINT8    Dimm,
  IN UINT8    Rank
  );

/**

  @param Host          - Pointer to sysHost
  @param socket        - Processor socket to check
  @param level         - DDR or LRDIMM Host side, LRDIMM Backside, etc...
  @param group         - Parameter to be margined
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
  @param mode          - Test mode to use
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
  @param scope         - Margin granularity
                           SCOPE_SOCKET      0: Margin per processor socket
                           SCOPE_CH        1: Margin per channel
                           SCOPE_DIMM      2: Margin per DIMM
                           SCOPE_RANK      3: Margin per rank
                           SCOPE_STROBE    4: Margin per strobe group
                           SCOPE_BIT       5: Margin per bit
  @param patternLength - RankMarginTest Pattern length
  @param chMask        - Bit Mask of channels to not be used
  @param rankMask      - Bit Mask of ranks to not be used
  @param dataMask      - Bit Mask for data

  @retval None

**/
VOID
EyeDiagram (
  PSYSHOST Host,
  UINT8 socket,
  MRC_LT level,
  MRC_GT group,
  UINT16 mode,
  UINT8 scope,
  UINT32 patternLength,
  UINT8 chMask,
  UINT8 rankMask,
  UINT8 dataMask[MAX_STROBE / 2]
  );

/**

  Displays the training results

  @param Host    - Pointer to sysHost, the system Host (root) structure struct
  @param socket  - Socket Id
  @param group   - MRC_GT group

  @retval None

**/
VOID
DisplayResults (
  PSYSHOST Host,
  UINT8 socket,
  MRC_GT group
  );

/**

  Evaluates (based on ODT table) if there are any detected issues
  with strobes and if so whether the set of issues form correctable
  or non-correctable error

  @param Host    - Pointer to sysHost, the system Host (root) structure struct
  @param socket  - Socket
  @param ch      - Channel

  @retval FPT_NO_ERROR
  @retval FPT_CORRECTABLE_ERROR
  @retval FPT_NO_CORRECTABLE_ERROR

**/
UINT8
EvaluateFaultyParts (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch
  );

/**
  Calculate the encode strobe value according to DQS and DQ FUBs

  @param[in]  Socket    The socket containing the strobe of interest
  @param[in]  Channel   The channel containing the strobe of interest
  @param[in]  Dimm      The DIMM containing the strobe of interest
  @param[in]  Strobe    The strobe of interest

  @retval   The encoded strobe number, if it could be calculated
  @retval   NO_STROBE, if the encoded value could not be calculated
**/
UINT8
EFIAPI
StrobeEncoding (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  IN  UINT8 Dimm,
  IN  UINT8 Strobe
  );

/**

  Executes the JEDEC initialization sequence for the given channel

  @param Host  - Pointer to sysHost
  @param socket  - Socket Id
  @param ch    - Channel number (0-based)

  @retval N/A

**/
VOID
JedecInitDdrChannel (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  );

/**

  Clears map out flags for DIMMs mapped out during
  memory decode run on previous boot

  @param[in] Host - Pointer to Host System Data

  @return None

**/
VOID
EFIAPI
ClearMemMapMappedOutDimms (
  IN PSYSHOST Host
  );

/**

  Detects Mapped out DIMMs and issues EWL accordingly

  @param[in] Host - Pointer to Host System Data

  @return None

**/
VOID
EFIAPI
CheckForMappedOutDimms (
  IN PSYSHOST Host
  );

VOID
CheckPORMixingWithinSocket (
  PSYSHOST Host
  );

/**

  Executes early control/clock training with Loopback mode

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval MRC_STATUS_SUCCESS
  @retval MRC_STATUS_SIGNAL_NOT_SUPPORTED

**/
UINT32
EarlyCtlClkLoopback (
  PSYSHOST Host
  );

/**

  Prints the edges found

  @param Host          - Pointer to sysHost, the system Host (root) structure struct
  @param socket        - Current Socket
  @param ch            - Channel number
  @param pulseWidth    - Pointer to save the pulse width
  @param fallingEdge   - Pointer to save the falling edge
  @param risingEdge    - Pointer to save the rising edge
  @param centerPoint   - Pointer to save the center point
  @param maxStrobe     - Maximum number of strobes

  @retval None

**/
VOID
DisplayEdges (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT16 pulseWidth[MAX_STROBE],
  UINT16 fallingEdge[MAX_STROBE],
  UINT16 risingEdge[MAX_STROBE],
  UINT16 centerPoint[MAX_STROBE],
  UINT8 maxStrobe
  );

/**
  Get PreviousBootError data from  Host->var.mem.previousBootError

  @param PreviousBootErrorValue:         Pointer to data buffer to return previousBootError data

  @retval SUCCESS
**/
UINT32
EFIAPI
GetPreviousBootError (
  OUT UINT8 *PreviousBootErrorValue
  );

/**
  Save PreviousBootError data to  Host->var.mem.previousBootError

  @param PreviousBootErrorValue:         value of PreviousBootError would be setting to

  @retval SUCCESS
**/
UINT32
EFIAPI
SetPreviousBootError (
  IN UINT8 PreviousBootErrorValue
  );

/**
  Get ImcEnabled data from Host->var.mem.socket[socket].imcEnabled[mcId]

  @param socket:         value of the socket number
  @param mcId:           value of the iMC ID
  @ImcEnabledData:       Pointer to data buffter to return corresponding imcEnabled value with socket and mcId number

  @retval SUCCESS
**/
UINT32
EFIAPI
GetImcEnabledData (
  IN UINT8 socket,
  IN UINT8 mcId,
  OUT UINT8 *ImcEnabledData
  );

/**
  Set ImcEnabled data to Host->var.mem.socket[socket].imcEnabled[mcId]

  @param socket:         value of the socket number
  @param mcId:           value of the iMC ID
  @ImcEnabledData:       data to be set to corresponding imcEnabled with socket and mcId number

  @retval SUCCESS
**/
UINT32
EFIAPI
SetImcEnabledData (
  IN UINT8 socket,
  IN UINT8 mcId,
  IN UINT8 ImcEnabledData
  );

/**
  Get Memory controller number data from Host->var.mem.socket[socket].channelList[channel].mcId

  @param socket:         value of the socket number
  @param channel:        value of the channel number
  @iMcIdData:            pointer to iMcIdData to get Memory controller number data (mcId) from sysHost with socket and channel number

  @retval SUCCESS
**/
UINT32
EFIAPI
GetMcIdData (
  UINT8 socket,
  UINT8 channel,
  UINT8 *iMcIdData
  );

/**
  Set Memory controller number data to Host->var.mem.socket[socket].channelList[channel].mcId

  @param socket:         value of the socket number
  @param channel:        value of the channel number
  @iMcIdData:            Memory controller numberdata to be set to corresponding mcId  with socket and channel number

  @retval SUCCESS
**/
UINT32
EFIAPI
SetMcIdData (
  UINT8 socket,
  UINT8 channel,
  UINT8 iMcIdData
  );

/**
  Get DdrFreqCheckFlag data from Host->var.mem.socket[socket].ddrFreqCheckFlag

  @param socket:            value of the socket number
  @ddrFreqCheckFlagData:    pointer to buffer to get ddrFreqCheckFlagData from Host->var.mem.socket[socket].ddrFreqCheckFlag

  @retval SUCCESS
**/
UINT32
EFIAPI
GetDdrFreqCheckFlagData (
  UINT8 socket,
  UINT8 *ddrFreqCheckFlagData
  );

/**
  Set DdrFreqCheckFlag data to Host->var.mem.socket[socket].ddrFreqCheckFlag

  @param socket:         value of the socket number
  @ddrFreqCheckFlagData: ddrFreqCheckFlag data to be set to corresponding ddrFreqCheckFlag  with socket  number in sysHost

  @retval SUCCESS
**/
UINT32
EFIAPI
SetDdrFreqCheckFlagData (
  UINT8 socket,
  UINT8 ddrFreqCheckFlagData
  );

/**
  Get DdrClk data from Host->var.mem.socket[socket].ddrClkData

  @param socket:            value of the socket number
  @DdrClkData:              pointer to buffer to get DdrClkData from Host->var.mem.socket[socket].ddrClkData

  @retval SUCCESS
**/
UINT32
EFIAPI
GetDdrClkData (
  UINT8 socket,
  UINT8 *DdrClkData
  );

/**
  Set DdrClk data to Host->var.mem.socket[socket].ddrClkData

  @param socket:         value of the socket number
  @DdrClkData:           DdrClk data to be set to corresponding DdrClkData  with socket  number in sysHost

  @retval SUCCESS
**/
UINT32
EFIAPI
SetDdrClkData (
  UINT8 socket,
  UINT8 DdrClkData
  );

/**
  Get DdrClkType data from Host->var.mem.socket[socket].ddrClkType

  @param socket:            value of the socket number
  @DdrClkType:              pointer to buffer to get DdrClkType from Host->var.mem.socket[socket].DdrClkType

  @retval SUCCESS
**/
UINT32
EFIAPI
GetDdrClkType (
  UINT8 socket,
  UINT8 *DdrClkType
  );

/**
  Set DdrClkType data to Host->var.mem.socket[socket].DdrClkType

  @param socket:         value of the socket number
  @DdrClkType:           DdrClkType data to be set to corresponding DdrClkType  with socket  number in sysHost

  @retval SUCCESS
**/
UINT32
EFIAPI
SetDdrClkType (
  UINT8 socket,
  UINT8 DdrClkType
  );

/**
  Get MaxFreq data from Host->var.mem.socket[socket].maxFreq

  @param socket:            value of the socket number
  @MaxFreqData:             pointer to buffer to get MaxFreqData from Host->var.mem.socket[socket].maxFreq

  @retval SUCCESS
**/
UINT32
EFIAPI
GetMaxFreqData (
  UINT8 socket,
  UINT8 *MaxFreqData
  );

/**
  Set MaxFreq data to Host->var.mem.socket[socket].maxFreq

  @param socket:         value of the socket number
  @MaxFreqData:          MaxFreq data to be set to corresponding MaxFreqData  with socket  number in sysHost

  @retval SUCCESS
**/
UINT32
EFIAPI
SetMaxFreqData (
  UINT8 socket,
  UINT8 MaxFreqData
  );

/**
  Map out ranks based on Policy rankmask.

  @param[in]  Host              A pointer to host data.

  @retval EFI_SUCCESS           Success.
  @retval !EFI_SUCCESS          Failure.

**/

EFI_STATUS
EFIAPI
CheckRankMapout (
  IN PSYSHOST Host
  );

/**

  Get/Set DDR4 DRAM Rx Eq (DRAM CTLE) per DDR4 standard

  @param[in]      Host     - Pointer to sysHost, the system Host (root) structure
  @param[in]      Socket   - Socket Id
  @param[in]      Ch       - Channel number
  @param[in]      Dimm     - DIMM number (0-based)
  @param[in]      Rank     - Rank number (0-based)
  @param[in]      Strobe   - Strobe number (0-based)
  @param[in]      Bit      - Bit number
  @param[in]      Level    - MRC_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]      Group    - MRC_GT - Parameter to program
  @param[in]      Mode     - GSM_READ_ONLY - skip write,
  @param[in, out] *Value   - Pointer to int16 to return read data or to use for write data

  @retval         MRC_STATUS_SUCCESS

**/
MRC_STATUS
EFIAPI
GetSetDdr4DramRxEq (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     Dimm,
  IN     UINT8     Rank,
  IN     UINT8     Strobe,
  IN     UINT8     Bit,
  IN     MRC_LT    Level,
  IN     MRC_GT    Group,
  IN     UINT8     Mode,
  IN OUT INT16     *Value
  );

/**
  Retrieve the cached MRS value for a given Strobe

  @param[in] Socket
  @param[in] Ch
  @param[in] Dimm
  @param[in] Rank
  @param[in] Strobe
  @param[in] Bank

  @retval UINT16 Cached value for given setting
**/
UINT16
EFIAPI
GetPerStrobeMrsValue (
  IN        UINT8           Socket,
  IN        UINT8           Ch,
  IN        UINT8           Dimm,
  IN        UINT8           Rank,
  IN        UINT8           Strobe,
  IN        UINT8           Bank
  );

/** Return whether a Get/Set Mode indicates to read the CSR.

  @param[in]  Mode  The Get/Set Mode to evaluate.

  @retval FALSE The mode indicates to read the cache.
  @retval TRUE  The mode indicates to read the CSR.

**/
BOOLEAN
EFIAPI
GsmReadCsr (
  IN  UINT8 Mode
  );

/** Return whether a Get/Set Mode indicates read-only.

  @param[in]  Mode  The Get/Set Mode to evaluate.

  @retval FALSE The mode indicates read/write.
  @retval TRUE  The mode indicates read-only.

**/
BOOLEAN
EFIAPI
GsmReadOnly (
  IN  UINT8 Mode
  );

/** Return whether a Get/Set Mode indicates to write an offset.

  @param[in]  Mode  The Get/Set Mode to evaluate.

  @retval FALSE The mode indicates the associated value is absolute.
  @retval TRUE  The mode indicates the associated values is an offset.

**/
BOOLEAN
EFIAPI
GsmWriteOffset (
  IN  UINT8 Mode
  );

/** Return whether a Get/Set Mode indicates to force a write.

  @param[in]  Mode  The Get/Set Mode to evaluate.

  @retval FALSE The mode indicates to write a CSR only if a value has changed.
  @retval TRUE  The mode indicates to write a CSR regardless of the value.

**/
BOOLEAN
EFIAPI
GsmForceWrite (
  IN  UINT8 Mode
  );

/** Return whether a Get/Set Mode indicates the cache is to be updated.

  @param[in]  Mode  The Get/Set Mode to evaluate.

  @retval FALSE The mode indicates to leave the cache untouched.
  @retval TRUE  The mode indicates to update the cache.

**/
BOOLEAN
EFIAPI
GsmUpdateCache (
  IN  UINT8 Mode
  );

/** Return whether a Get/Set Mode indicates to combine edges.

  @param[in]  Mode  The Get/Set Mode to evaluate.

  @retval FALSE The mode indicates to use a single values as center.
  @retval TRUE  The mode indicates to use two values as opposite edges.

**/
BOOLEAN
EFIAPI
GsmCombineEdges (
  IN  UINT8 Mode
  );

/**
  Return a pointer to a signal name string

  @param[in]  SignalName    - The signal name

  @retval Pointer to the string

**/
CHAR8*
GetSignalStr (
  IN  GSM_CSN   SignalName
  );

/**

  Return a pointer to a signal name string

  @param[in]  SignalName    - The signal name

  @retval Pointer to the string

**/
CHAR8*
GetSignalStrDdr5 (
  IN  GSM_CSN   SignalName
  );

/**

  Return a pointer to a signal name string

  @param[in]  SignalName    - The signal name
  @param[in]  Copy          - Copy A or B of backside signal

  @retval Pointer to the string

**/
CHAR8*
GetQcsSignalStrDdr5 (
  IN  GSM_CSN   SignalName,
  IN  UINT8     Copy
  );

/**
  This function is used to get a string associated with the given I/O level.

  @param[in]   Level   I/O level.

  @retval   String describing the I/O level.
**/
CHAR8*
EFIAPI
GetLevelStr (
  MRC_LT      level
  );

/**
  Return a pointer to a CMD group string

  @param[in]  Group   - MRC_GT Group value

  @retval   String pointer corresponding to Group
**/
CHAR8*
GetPlatformGroupStr (
  IN  MRC_GT  Group
  );

/** Return the number of sockets in the system.

  @retval The number of sockets in the system.

**/
UINT8
EFIAPI
GetNumSockets (
  VOID
  );

/**
  Convert a timing parameter in picosecond (ps) units to number of clocks (nCK) units

  @param[in] Socket           - Socket number
  @param[in] TimePicosecond   - Time in ps

  @retval Time in clock units
**/
UINT16
EFIAPI
PicosecondToClocks (
  IN UINT8     Socket,
  IN UINT32    TimePicosecond
  );

/**
  Convert a timing parameter in nanosecond (ns) units to number of clocks (nCK) units

  @param[in] Socket   - Socket number
  @param[in] Time     - Time in ns

  @retval Time in clock units
**/
UINT16
EFIAPI
NanosecondToClocks (
  IN UINT8     Socket,
  IN UINT32    TimeNanosecond
  );

/**
  Get the system DDR read burst length, that ignores WrCRC padding

  @param n/a

  @retval Burst Length in UINT32
**/
UINT8
EFIAPI
GetReadBurstLength (
  VOID
  );

/**
  Get the system DDR write burst length, that considers WrCRC padding

  @param[in] Channel - DDR channel number (0-based) in the current socket

  @retval Burst Length in UINT32
**/
UINT8
EFIAPI
GetWriteBurstLength (
  IN UINT8 Channel
  );

/**

  Effective Bus Delay

  @param[in]  Host                 - Point to sysHost
  @param[in]  Socket               - Socket number
  @param[in]  Channel              - Channel number
  @param[out] EffectiveBusDelaySr  - effective bus delay for same rank (SR)
  @param[out] EffectiveBusDelayDr  - effective bus delay for different rank same DIMM (DR)
  @param[out  EffectiveBusDelayDd  - effective bus delay for different DIMM (DD)
  @param[in]  DdrtTurnaroundFlag   - DDRT Turnaround flag

  @retval N/A

**/
VOID
EFIAPI
EffectiveBusDelay (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  OUT INT32   *EffectiveBusDelaySr,
  OUT INT32   *EffectiveBusDelayDr,
  OUT INT32   *EffectiveBusDelayDd,
  IN BOOLEAN   DdrtTurnaroundFlag
  );

/**

  CAS2RCVEN: The time from sending CAS command @ IO till RCV_EN is asserted per this command.
  It should be calculated per byte as
  CAS2RCVEN(rank, byte) = 1/2 * (round_trip_latency(rank) + RxRcvEnLogicDelay(rank, byte)
        - IO_latency(rank) + RcvPIsetting(rank, byte) / 64) - CONST
  The CONST is the latency from pads to MC. It doesn't affect the further calculations because these will be the differences
  of CAS2RCVEN of different ranks same byte.
  The gap between two data transfers is determined by two limits - drive period and ODT period. The limiting factor is
  different for different rank and for different DIMM.
  R2RDR_GAP: On different ranks same DIMM the ODT value doesn't change in the turnaround, so the limiters are derived from
  need to keep non-overlap between the two drivers. For this purpose 0.6 DCLK cycle is enough. The final equation is
            tRRDR-D0 = max(Byte = 0 to 7, ECC) |CAS2RCVEN(R0, byte) - CAS2RCVEN(R1, byte)| + 0.25
            tRRDR-D2 = max(Byte = 0 to 7, ECC) |CAS2RCVEN(R2, byte) - CAS2RCVEN(R3, byte)| + 0.25
            tRRDR = round-up (max (tRRDR-D0, tRRDR-D2)) -1

  @param[in]  Host    - Point to sysHost
  @param[in]  Socket  - Socket number
  @param[in]  Channel - Channel number
  @param[in]  Mode    - Timing Mode: SR_MODE, DR_MODE, or DD_MODE

  @retval  gap

**/
UINT8
EFIAPI
Cas2RecEnGap (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Mode,
  IN BOOLEAN   DdrtTurnaroundFlag
  );

/**

  Get Cas2DrvEnGap

  @param[in]  Host                - Point to sysHost
  @param[in]  Socket              - Socket number
  @param[in]  Channel             - Channel number
  @param[in]  Mode                - Mode
  @param[in]  DdrtTurnaroundFlag  - DDRT turnaround flag

  @retval N/A

**/
UINT8
EFIAPI
Cas2DrvEnGap (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Mode,
  IN BOOLEAN   DdrtTurnaroundFlag
  );

/**

  @param[in]  Host    - Pointer to sysHost
  @param[in]  Socket  - Socket number
  @param[in]  Channel - Channel number
  @param[in]  Mode    - Timing Mode: SR_MODE, DR_MODE, or DD_MODE

  @retval Maximum delay between clocks across DIMMs in Pi ticks

**/
UINT8
EFIAPI
CkDiffGap (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Mode
  );

/**

  Returns a bitmask of channels that have the current lrdimm and rank enabled

  @param[in] Socket  - Socket to check
  @param[in] Dimm    - DIMM to check
  @param[in] Rank    - Rank to check

  @retval Bitmask of channels present for the current rank

**/
UINT32
EFIAPI
GetChBitmaskLrdimm (
  IN UINT8     Socket,
  IN UINT8     Dimm,
  IN UINT8     Rank
  );

/**

  Used to add warnings to the promote warning exception list

  @param[in] Host  - pointer to sysHost

  @retval None

**/
UINT32
MemInitDefaultWarnings (
  IN PSYSHOST Host
  );

/**
  Load saved training data.

  @param[in] None

  @retval !EFI_SUCCESS on failure

**/

EFI_STATUS
EFIAPI
LoadNvramData (
  VOID
  );

/**
  Determine if the given Strobe number is an ECC Strobe for Ddr5 or Ddr4

  @param[in]                Host          Pointer to sysHost
  @param[in]                Strobe        Strobe number to check

  @retVal                   TRUE          Strobe is ECC
  @retVal                   FALSE         Strobe is not ECC

**/
BOOLEAN
IsEccStrobe (
  IN        PSYSHOST        Host,
  IN        UINT8           Strobe
);

/*++

  Re-centers rx or tx dq timing using a more robust test

  @param[in] Host    - Pointer to sysHost
  @param[in] socket  - Socket number
  @param[in] group   - Parameter to center
                       RxDqsDelay    1: Center Rx DqDqs
                       RxDqsPDelay   2: Center Rx DqDqsp
                       RxDqsNDelay   3: Center Rx DqDqsn
                       TxDqDelay     12: Center Tx DqDqs

  @retval N/A

--*/
VOID
EFIAPI
AdvancedCentering (
  IN PSYSHOST  Host,
  IN UINT8     socket,
  IN MRC_GT    group
  );

/**
  Convert the encoded DRAM density from the SPD to Gb units

  @param[in]  SpdEncodedDensity   - SPD density value to convert to Gb units

  @retval DRAM density in Gb

**/
UINT16
EFIAPI
EncodedSpdToDramDensity (
  IN  UINT8         SpdEncodedDensity
  );

/**

  Enables Write CRC in the DRAM

  @param[in] Socket   - Socket number
  @param[in] Ch       - Channel number within the socket
  @param[in] Dimm     - DIMM number within the channel
  @param[in] Rank     - Rank number within the DIMM

  @retval N/A

**/
VOID
EFIAPI
DramEnableWrCrc (
  IN  UINT8    Socket,
  IN  UINT8    Ch,
  IN  UINT8    Dimm,
  IN  UINT8    Rank
  );
/*++

  Re-centers using a more robust test

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param group   - Parameter to center
                   RxDqsDelay   0: Center Rx DqDqs
                   TxDqDelay    1: Center Tx DqDqs

  @retval SUCCESS

--*/
UINT32
EFIAPI
VrefCentering (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     perBit,
  MRC_GT    group
  );

#endif  // #ifndef _MEMORY_CORE_LIB_H_
