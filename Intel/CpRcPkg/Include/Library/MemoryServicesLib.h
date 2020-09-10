/** @file
  Interface header file for the Memory MRC service library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#ifndef _MEMORY_SERVICES_LIB_H_
#define _MEMORY_SERVICES_LIB_H_

#include <SysHost.h>
#include <ReferenceCodeDataTypes.h>
#include <PlatformInfoTypes.h>

#define MEMFLOWS_BLOCK_SIZE  32

///
/// MemFlows Enum
/// Used to identify mem flows to enable/disable them, or get their state
///
typedef enum {
  CrossoverCalibration              = 0,
  SenseAmpOffsetTraining            = 1,
  EarlyCmdClkTraining               = 2,
  ReceiveEnableTraining             = 3,
  ReadDqDqsTraining                 = 4,
  WriteLevelingTraining             = 5,
  WriteLevelingFlybyCleanup         = 6,
  WriteDqDqsTraining                = 7,
  LateCmdClkTraining                = 8,
  ReadAdvancedCenteringTraining     = 9,
  WriteAdvancedCenteringTraining    = 10,
  ReadVrefTraining                  = 11,
  WriteVrefTraining                 = 12,
  RoundTripOptimizeTraining         = 13,
  RxTxPerBitDeskewTraining          = 14,
  TxEqTraining                      = 15,
  ImodeTraining                     = 16,
  EarlyReadIdTraining               = 17,
  DqSwizzlingDiscovery              = 18,
  LrdimmBacksideRxTraining          = 19,
  LrdimmBacksideTxTraining          = 20,
  LegacyRankMarginTool              = 21,
  EarlyWriteVrefCenteringTraining   = 22,
  EarlyReadVrefCenteringTraining    = 23,
  LrdimmReadVrefCenteringTraining   = 24,
  MemInitialization                 = 25,
  DqSwizzleDiscoveryTest            = 26,
  NormalModeSwitch                  = 27,
  CmdVrefCenteringTraining          = 28,
  LrdimmWriteVrefCenteringTraining  = 29,
  AdvancedMemoryTest                = 30,
  EarlyCtlClkTraining               = 31,
  RxCtleTraining                    = 32,
  PhaseCrosstalkTraining            = 33,
  CmdNormalization                  = 34,
  LrdimmBacksideDqTraining          = 35,
  DramRxEqTraining                  = 36,
  LrdimmBcomTraining                = 37,
  CheckPorCompatibility             = 38,
  MmrcInitExecution                 = 39,
  EarlyInitThermalThrottling        = 40,
  InitThermalThrottling             = 41,
  PostTrainingInitialization        = 42,
  EarlyMemoryControllerConfig       = 43,
  LateMemoryControllerConfig        = 44,
  SsaRankMarginTool                 = 45,
  McOdtTraining                     = 46,
  McROnTraining                     = 47,
  DramROnTraining                   = 48,
  CaClkTrainingBackSide             = 49,
  DqSwizzlingDiscoveryX16           = 50,
  TcoCompTraining                   = 51,
  TxRiseFallSlewRateTraining        = 52,
  InitializeMemoryMap               = 53,
  CmdTxEqTraining                   = 54,
  RcompStaticLeg                    = 55,
  TxDdjcTraining                    = 56,
  RxDfeTraining                     = 57,
  EarlyCsClkTraining                = 58,
  EarlyBacksideCsClkTraining        = 59,
  CaClkTraining                     = 60,
  CrossoverHwFsmCalibration         = 61,
  InitializeCmiCreditProgramming    = 62,
  BacksideCmdCtlTraining            = 63,
  WriteDqDqsPreDfe2DCenteringTraining = 64,
  WriteDqDqsPostDfe2DCenteringTraining = 65,
  DcaDckDutyCycleTraining              = 66,
  RcdDcaDfeDdr5                        = 67,
  ReadDqDqsPreDfe2DCenteringTraining   = 68,
  ReadDqDqsPostDfe2DCenteringTraining  = 69,
  PeriodicTxReTraining                 = 70,
  CaSlewRateTraining                   = 71,
  DcaTcoTraining                       = 72,
  EarlyReqClkTraining                  = 73,
  TurnaroundTraining                   = 74,
  RxDfeDdr5Training                    = 75,
  TxDfeDdr5Training                    = 76,
  DqSlewRateTraining                   = 77,
  LrdimmDbDfe                          = 78,
  DramDutyCycleAdjusterTraining        = 79,
  TcoDqDqsTraining                     = 80,
  PbaEnumerateId                       = 81,
  DbDfeDdr5Training                    = 82,
  TxOdtLatencyTraining                 = 83,
  MemFlowsMax
} MEMFLOWS;

//
// Error Code used for LogError()
//
#define ERROR_CPU_BIST                             0xC0
#define ERROR_CPU_BIST_MINOR_SOME_SOCKET           0x01
#define ERROR_CPU_BIST_MINOR_SOME_BISTRESULTMASK   0x02
#define ERROR_CPU_BIST_MINOR_ALL                   0x03

//
// Error Codes used for LogError() and LogWarning()
//
#define WARN_CPU_BIST                              0xC0
#define WARN_CPU_BIST_MINOR_LOWER_TILE_RANGE       0x01
#define WARN_CPU_BIST_MINOR_MIDDLE_TILE_RANGE      0x02
#define WARN_CPU_BIST_MINOR_UPPER_TILE_RANGE       0x03
#define WARN_CPU_BIST_MINOR_ALL                    0x04

#define ROTATE_LEFT_UINT16(Data, Count)            (((UINT16) Data >> (16 - Count)) | ((UINT16) Data << Count))
#define ROTATE_RIGHT_UINT16(Data, Count)           (((UINT16) Data << (16 - Count)) | ((UINT16) Data >> Count))

/**

  Adds two UINT64 values

  @param[in] Operand1  - first UINT64 to add
  @param[in] Operand2  - second UINT64 to add

  @retval sum of operand1 and operand2

**/

UINT64_STRUCT
EFIAPI
AddUINT64 (
  IN UINT64_STRUCT Operand1,
  IN UINT64_STRUCT Operand2
  );

/**

  Add UINT64 and UINT32

  @param[in] Operand1  - UINT64 to add
  @param[in] Operand2  - UINT32 to add

  @retval sum of operand1 and operand2

**/

UINT64_STRUCT
EFIAPI
AddUINT64x32 (
  IN UINT64_STRUCT Operand1,
  IN UINT32        Operand2
  );

/**

  Subtract two UINT64s

  @param[in] Operand1  - first UINT64
  @param[in] Operand2  - second UINT64

  @retval operand1 - operand2

**/

UINT64_STRUCT
EFIAPI
SubUINT64 (
  IN UINT64_STRUCT Operand1,
  IN UINT64_STRUCT Operand2
  );

/**

  Subtract an UINT32 from an UINT64

  @param[in] Operand1  -  UINT64
  @param[in] Operand2  -  UINT32

  @retval operand1 - operand2

**/

UINT64_STRUCT
EFIAPI
SubUINT64x32 (
  IN UINT64_STRUCT Operand1,
  IN UINT32        Operand2
  );

/**

  Multiplies two 32 bit values

  @param[in] Operand1 - first 32 bit value
  @param[in] Operand2 - second 32 bit value

  @retval 64 bit value resulting from the multiplication of two given values

**/

UINT64_STRUCT
EFIAPI
MulDw64 (
  IN UINT32 Operand1,
  IN UINT32 Operand2
  );

/**

  This routine allows a 64 bit value to be multiplied with a 32 bit
  value returns 64bit result. No checking if the result is greater than 64bits.

  @param[in] Multiplicand  - Multiplicand
  @param[in] Multiplier    - what to multiply by

  @retval result of Multiplicand * Multiplier

**/

UINT64_STRUCT
EFIAPI
MultUINT64x32 (
  IN UINT64_STRUCT Multiplicand,
  IN UINT32        Multiplier
  );

/**

  divides 64 bit value by 2

  @param[in] Operand - 64 bit value to divide

  @retval result of Operand / 2

**/

UINT64_STRUCT
EFIAPI
DivBy2Qw64 (
  IN UINT64_STRUCT Operand
  );

/**

  This routine allows a 64 bit value to be divided with a 32 bit value returns
  64bit result and the Remainder.

  @param[in] Dividend  - Value to divide into
  @param[in] Divisor   - Value to divide by
  @param[in] Remainder - Pointer to location to write remainder

  @retval Remainder of Dividend / Divisor

**/

UINT64_STRUCT
EFIAPI
DivUINT64x32 (
  IN UINT64_STRUCT Dividend,
  IN UINT32        Divisor,
  IN UINT32        *Remainder
  );

/**

  This function calculates floor(log2(x)) + 1
  by finding the index of the most significant bit
  and adding 1

  @param[in] Value     - Input value

  @retval Returns floor(log2(x)) + 1 or 0, if the input was 0 or negative

**/

UINT8
EFIAPI
Log2x32 (
  IN UINT32 Value
  );

/**

  This routine allows a 64 bit value to be right shifted by 32 bits and returns the
  shifted value. Count is valid up 63. (Only Bits 0-5 is valid for Count)

  @param[in] Operand - Value to be shifted
  @param[in] Count   - Number of times to shift right.

  @retval Value shifted right identified by the Count.

**/

UINT64_STRUCT
EFIAPI
RShiftUINT64 (
  IN UINT64_STRUCT Operand,
  IN UINT32        Count
  );

/**

  This routine allows a 64 bit value to be left shifted by 32 bits and returns
  the shifted value. Count is valid up 63. (Only Bits 0-5 is valid for Count)

  @param[in] Data  - value to be shifted
  @param[in] Count - number of time to shift

  @retval Value shifted left identified by the Count.

**/

UINT64_STRUCT
EFIAPI
LShiftUINT64 (
  IN UINT64_STRUCT Data,
  IN UINT32        Count
  );

/**

  Rotates 64-bit data left by given count

  @param[in] Data   - 64-bit data to rotate
  @param[in] Count  - Number of bits to rotate

  @retval UINT64_STRUCT - Rotated 64-bit result

**/

UINT64_STRUCT
EFIAPI
RotateLeftUINT64 (
  IN UINT64_STRUCT Data,
  IN UINT32        Count
  );

/**

  Rotates 64-bit data right by given count

  @param[in] Data   - 64-bit data to rotate
  @param[in] Count  - Number of bits to rotate

  @retval UINT64_STRUCT - Rotated 64-bit result

**/

UINT64_STRUCT
EFIAPI
RotateRightUINT64 (
  IN UINT64_STRUCT Data,
  IN UINT32        Count
  );

/**

  Limit check to ensure max value is not exceeded

  @param[in] MaxVal - The largest possible value that val can be
  @param[in] Val    - The value to be tested

  @retval lesser of Val or MaxVal

**/

UINT32
EFIAPI
Saturate (
  IN UINT32    MaxVal,
  IN UINT32    Val
  );

/**

  Install the Mrc Hooks Core Layer Services PPI

  @param[in] Host     - Pointer to sysHost, the system Host (root) structure

  @retval EFI_STATUS

**/
EFI_STATUS
EFIAPI
InstallMrcHooksServicesPpi (
  IN PSYSHOST Host
  );

/**

  Get the memory subsystem type.

  @param[in] Socket  - Socket to check

  @retval memory subsystem type.

**/

MRC_MST
EFIAPI
GetMemSsType (
  IN        UINT8           Socket
  );

/**

  Set the memory subsystem type.

  @param[in] Socket     - Socket to check
  @param[in] MemSsType  - Subsytem type

  @retval None.

**/

VOID
EFIAPI
SetMemSsType (
  IN        UINT8           Socket,
  IN        MRC_MST         MemSsType
  );

/**

  Check if operating in a Simics environment.

  @param None.

  @retval TRUE is Simics, FALSE if not.

**/

BOOLEAN
EFIAPI
IsSimics (
  VOID
  );

/**

  Get the number of memory subsystem types.

  @param[in] Socket  - Socket to check

  @retval number of memory subsystem types.

**/

UINT32
EFIAPI
GetNumMemSs (
  IN        UINT8           Socket
  );

/**

  Get the memory subsystem type from the subsystem list.

  @param[in] Socket  - Socket to check
  @param[in] MemSs   - Subsystem to check

  @retval Memory subsystem type.

**/

MRC_MST
EFIAPI
GetMemSsTypeFromSsList (
  IN        UINT8           Socket,
  IN        UINT8           MemSs
  );

/**

  Get the number of data channels from the subsystem list.

  @param[in] Socket  - Socket to check
  @param[in] MemSs   - Subsystem to check

  @retval Number of channels.

**/

UINT32
EFIAPI
GetNumDataChFromSsList (
  IN        UINT8           Socket,
  IN        UINT8           MemSs
  );

/**

  Get the number of IO FUBs per channel
  from the subsystem list.

  @param[in] Socket  - Socket to check
  @param[in] MemSs   - Subsystem to check

  @retval Number of IO FUBs.

**/

UINT32
EFIAPI
GetNumDataIoFubsPerChFromSsList (
  IN        UINT8           Socket,
  IN        UINT8           MemSs
  );

/**

  Get the number of IO FUBs per subchannel
  from the subsystem list.

  @param[in] Socket  - Socket to check
  @param[in] MemSs   - Subsystem to check

  @retval Number of IO FUBs.

**/

UINT32
EFIAPI
GetNumDataIoFubsPerSubChFromSsList (
  IN        UINT8           Socket,
  IN        UINT8           MemSs
  );

/**

  Get the number of DQ Lanes per channel
  from the subsystem list.

  @param[in] Socket  - Socket to check
  @param[in] MemSs   - Subsystem to check

  @retval Number of DQ Lanes per channel.

**/

UINT32
EFIAPI
GetNumDqLanesPerChFromSsList (
  IN        UINT8           Socket,
  IN        UINT8           MemSs
  );

/**

  Check if running in emulation.

  @param  None

  @retval TRUE - Is running in emulation.

**/

BOOLEAN
EFIAPI
IsEmulation (
  VOID
  );

/**

  Check if we are on the fast boot path.

  @param  None

  @retval TRUE - Is on the fast boot path.

**/

BOOLEAN
EFIAPI
IsFastBootPath (
  VOID
  );

/**
  Check if the system boot mode is cold boot.

  @retval TRUE        Boot mode is cold boot.
  @retval FALSE       Boot mode is not cold boot.

**/
BOOLEAN
EFIAPI
IsSystemColdBoot (
  VOID
  );

/**

  Check if mem size is zero or not.

  @param[in] Socket  - Socket to check

  @retval TRUE - mem size is not zero.

**/

BOOLEAN
EFIAPI
IsMemSizePerSocket (
  IN        UINT8           Socket
  );

/**

  Get the mem size for a memory controller.

  @param[in] Socket  - Socket to check
  @param[in] McIndex - Memory Controller Index

  @retval TRUE - mem size is not zero.

**/

UINT32
EFIAPI
GetMemSize (
  IN        UINT8           Socket,
  IN        UINT8           McIndex
  );

/**

  Check if the vol size is zero or not.

  @param[in] Socket  - Socket to check

  @retval TRUE - vol size is not zero.

**/

BOOLEAN
EFIAPI
IsVolSizePerSocket (
  IN        UINT8           Socket
  );

/**

  Get the vol size for a memory controller.

  @param[in] Socket  - Socket to check
  @param[in] McIndex - Memory Controller Index

  @retval Vol size.

**/

UINT32
EFIAPI
GetVolSize (
  IN        UINT8           Socket,
  IN        UINT8           McIndex
  );

/**

  Get a SAD Table entry.

  @param[in] Socket   - Socket to check
  @param[in] SadIndex - SAD Index

  @retval SAD Table entry.

**/

SAD_TABLE *
EFIAPI
GetSadTableEntry (
  IN        UINT8           Socket,
  IN        UINT8           SadIndex
  );

/**

  Get if ECC is enabled.

  @param None.

  @retval TRUE - ECC is enabled.

**/

BOOLEAN
EFIAPI
IsEccEn (
  VOID
  );

/**

  Get if Wipe is enabled.

  @param None.

  @retval TRUE - Wipe is enabled.

**/

BOOLEAN
EFIAPI
IsWipeEn (
  VOID
  );

/**

  Get if Skip is enabled.

  @param None.

  @retval TRUE - Skip is enabled.

**/

BOOLEAN
EFIAPI
IsSkipEn (
  VOID
  );

/**

  Set Syshost pointer to indicate memory needs to be wiped

  retval N/A

**/

VOID
EFIAPI
SetWipeMemory (
  VOID
  );

/**

  Get RMT pattern length.

  @param None.

  @retval RMT pattern length.

**/

UINT32
EFIAPI
GetRmtPatternLength (
  VOID
  );

/**
  This function returns the memory topology configuration for the channel
  specified in the parameters list

  @param[in] Host   - Pointer to sysHost structure on stack
  @param[in] Socket - Socket
  @param[in] Channel     - Channel

  @retval Memory Topology present in the channel:
          DaisyChainTopology
          InvSlotsDaisyChainTopology
          TTopology
**/

EFI_MEMORY_TOPOLOGY_TYPE
EFIAPI
GetMemoryTopologyConfiguration (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel
  );

/*
  This function returns the memory channel connector type
  for the channel specified in the parameters list

  @param[in] Host   - Pointer to sysHost structure on stack
  @param[in] Socket - Socket
  @param[in] Channel     - Channel

  @retval Memory connectory type present in the channel:
          DimmConnectorPth        - Through hole connector
          DimmConnectorSmt        - Surface mount connector
          DimmConnectorMemoryDown - Platform soldered DRAMs
**/

EFI_MEMORY_DIMM_CONNECTOR_TYPE
EFIAPI
GetMemoryConnectorType (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel
  );

/**

  Log warning to the warning log

  @param[in] WarningCode       - Major warning code
  @param[in] MinorWarningCode  - Minor warning code
  @param[in] Socket            - socket number
  @param[in] Channel           - channel number
  @param[in] Dimm              - dimm number
  @param[in] Rank              - rank number

  @retval None

**/

VOID
EFIAPI
OutputWarning (
  IN UINT8    WarningCode,
  IN UINT8    MinorWarningCode,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  );

/**

  Log a Warning to the Host structure

  @param[in] MajorCode  - Major error code
  @param[in] MinorCode  - Minor error code
  @param[in] LogData    - 32 bit error specific data to log

  @retval None

**/

VOID
EFIAPI
LogWarning (
  IN UINT8    MajorCode,
  IN UINT8    MinorCode,
  IN UINT32   LogData
  );

/**

  Log Error to the warning log

  @param[in] ErrorCode         - Major warning code
  @param[in] MinorErrorCode    - Minor warning code
  @param[in] Socket            - socket number
  @param[in] Channel           - channel number
  @param[in] Dimm              - dimm number
  @param[in] Rank              - rank number

  @retval None

**/

VOID
EFIAPI
OutputError (
  IN UINT8    ErrorCode,
  IN UINT8    MinorErrorCode,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  );

/**

  Log an Error to the Host structure

  @param[in] MajorCode  - Major error code
  @param[in] MinorCode  - Minor error code
  @param[in] Socket     - Socket error occurred on
  @param[in] BistMask   - 64 bit BIST Mask results

  @retval None

**/

VOID
EFIAPI
LogCpuError (
  IN UINT8    MajorCode,
  IN UINT8    MinorCode,
  IN UINT8    Socket,
  IN UINT64   BistMask
  );

/**

  Log a warning to the Host structure

  @param[in] MajorCode - Major error code to log
  @param[in] MinorCode - Minor error code to log
  @param[in] Socket    - Socket

  @retval VOID

**/

VOID
EFIAPI
LogCpuWarning (
  IN UINT8    MajorCode,
  IN UINT8    MinorCode,
  IN UINT8    Socket
  );

/**

  Log an Error to the Host structure

  @param[in] MajorCode - Major error code to log
  @param[in] MinorCode - Minor error code to log
  @param[in] LogData   - 32 bit error specific data to log

  @retval N/A

**/

VOID
EFIAPI
LogError (
  IN UINT8    MajorCode,
  IN UINT8    MinorCode,
  IN UINT32   LogData
  );

/**

  Checks setup option for halt on error and if set halts the platform

  @param[in] MajorCode  - Major error code
  @param[in] MinorCode  - Minor error code

  @retval None

**/

VOID
EFIAPI
HaltOnError (
  IN UINT8    MajorCode,
  IN UINT8    MinorCode
  );

/**

  Adds error/exception to the promoteWarningList in Host structure if possible

  @param[in] MajorCode - Major error code to log
  @param[in] MinorCode - Minor error code to log

  @retval 0 - Successfully added to log
  @retval 1 - Log full

**/

UINT32
EFIAPI
SetPromoteWarningException (
  IN UINT8    MajorCode,
  IN UINT8    MinorCode
  );

/**

  Removes a warning from the promoteWarningList in Host structure if present

  @param[in] MajorCode - Major error code to log
  @param[in] MinorCode - Minor error code to log

  @retval 0 - Successfully cleared from log
  @retval 1 - Warning/Error not in log

**/

UINT32
EFIAPI
ClearPromoteWarningException (
  IN UINT8    MajorCode,
  IN UINT8    MinorCode
  );

/**

  Checks that index is less than arraySize to avoid array out of bounds access

  @param[in] Index          - The index to check
  @param[in] ArrayBoundary  - The boundary of the array

  @retval None
**/

VOID
EFIAPI
MemCheckIndex (
  IN UINT32 Index,
  IN UINT32 ArrayBoundary
  );

/**

  Finds greatest common denominator

  @param[in] X - First Value
  @param[in] Y - Second Value

  @retval Greatest common denominator

**/

UINT32
EFIAPI
GetGCD (
  IN UINT32 X,
  IN UINT32 Y
  );

/**

  Calculates the largest integer that is both
  a power of two and less than Input

  @param[in] Input - value to calculate power of two

  @retval The largest integer that is both  a power of two and less than Input

**/

UINT64_STRUCT
EFIAPI
GetPowerOfTwo (
  IN UINT64_STRUCT Input
  );

/**

  Calculates the largest integer that is both
  a power of two and less than or equal to input

  @param[in] Input - value to calculate power of two

  @retval The largest integer that is both a power of two and less than or equal to Input

**/

UINT32
EFIAPI
GetPowerOfTwox32 (
  IN UINT32 Input
  );

/**

  Raise 10 to the power of Power, and multiply the result with Operand

  @param[in] Operand - multiplicand
  @param[in] Power   - value for 10 ^ Power

  @retval Operand * (10 ^ Power)

**/

UINT64_STRUCT
EFIAPI
Power10UINT64 (
  IN UINT64_STRUCT Operand,
  IN UINT32        Power
  );

/**

  This function shifts a 32 bit INT32 either positive or negative

  @param[in] Value     - Input value to be shifted
  @param[in] Shift     - Number of bits places to be shifted.

  @retval Value    - Updated 32 bit value.

**/

UINT32
EFIAPI
BitShift (
  IN UINT32 Value,
  IN INT8 ShiftAmount
  );

/**

  This function Sign extends 7 to 8 Bits (Bit 6 to Bit 7)

  @param[in] CurrentValue     - Input value to be shifted

  @retval CurrentValue    - Updated 32 bit value.

**/

UINT8
EFIAPI
SignExt (
  IN UINT8 CurrentValue
  );

/**

  This function calculates the Natural Log of the Input parameter using integers

  @param[in] Input        - 100 times a number to get the Natural log from.
                      - Max Input Number is 40,000 (without 100x)

  @retval Output      - 100 times the actual result. Accurate within +/- 2

**/

UINT32
EFIAPI
NaturalLog (
  IN UINT32 Input
  );

/**

  This function calculates e to the power of of the Input parameter using integers

  @param[in] Input        - 100 times a number to elevate e to.

  @retval Output      - 100 times the actual result. Accurate within +/- 2

**/

UINT32
EFIAPI
Eexp (
  IN UINT32 Input
  );

/**

  Find if a value is a power of 2

  @param[in] Value  - value to be examined

  @retval TRUE  - the value is a power of two
  @retval FALSE - the value is not a power of two

**/

BOOLEAN
EFIAPI
IsPowerOfTwo (
  IN UINT32 Value
  );

/**

  Print the Boot Perfomance Statics Data for each Phase of MRC.

  @param[in] Host - Pointer to sysHost, the system Host (root) structure

  @retval None

**/

UINT32
EFIAPI
PrintAllStats (
  IN PSYSHOST Host
  );

/**

  Determines if a mem flow is enabled or disabled

  @param[in]  Flow    Mem flow to check for enable state

  @retval     True    Flow is enabled
  @retval     False   Flow is disabled

**/

BOOLEAN
EFIAPI
IsMemFlowEnabled (
  IN  MEMFLOWS  Flow
  );

/**

  Enables a mem flow

  @param[in]  Flow    Mem flow to enable

  @retval N/A

**/

VOID
EFIAPI
EnableMemFlow (
  IN  MEMFLOWS  Flow
  );

/**

  Disables a mem flow

  @param[in]  Flow    Mem flow to enable

  @retval N/A

**/

VOID
EFIAPI
DisableMemFlow (
  IN  MEMFLOWS  Flow
  );

/**

  Enables all mem flows

  @retval N/A

**/

VOID
EFIAPI
EnableAllMemFlows (
  VOID
  );

/**

  Disables all mem flows

  @retval N/A

**/

VOID
EFIAPI
DisableAllMemFlows (
  VOID
  );

/**

  Check to see if any mem flow is enabled.

  @retval TRUE - some mem flow is enabled.

**/

BOOLEAN
EFIAPI
IsAnyMemFlowEnabled (
  VOID
  );

/**

  Outputs error code hand halts if halt on fatal error, else exits reference code

  @param[in] MajorCode - Major error code to log
  @param[in] MinorCode - Minor error code to log

  @retval None

**/

VOID
EFIAPI
FatalError (
  IN UINT8       MajorCode,
  IN UINT8       MinorCode
  );

/**
  Collect each bit in the bit mask into adjacent least significant bits

  @param[in]  BitMask
  @param[in]  InputValue

  @retval[out] right justified bits

**/

UINT16
EFIAPI
RightJustifySparse16BitField (
  IN  UINT16 BitMask,
  IN  UINT16 InputValue
  );

/**
  Distribute each bit in a 16 bit right justified value into a sparse bit mask

  @param[in]  BitMask
  @param[in]  InputValue

  @retval[out] value with distributed bits

**/

UINT16
EFIAPI
DistributeSparse16BitField (
  IN  UINT16 BitMask,
  IN  UINT16 InputValue
  );

/*
  Determines if there is any ECC Dimm populated in the channel

  @param[in] Socket - Socket under test (Based-0)
  @param[in] Ch     - Channel under test (Based-0)

  @retval TRUE  When a ECC device is detected in the channel
  @retval FALSE Any other Dimm or not populated
*/
BOOLEAN
EFIAPI
IsThereAnyEccDimmInChannel (
  IN  UINT8   Socket,
  IN  UINT8   Ch
  );

/*
  Determines if there is any x16 Dimm populated in the channel

  @param[in] Socket - Socket under test (Based-0)
  @param[in] Ch     - Channel under test (Based-0)

  @retval TRUE  When a x16 device is detected in the channel
  @retval FALSE Any other Dimm or not populated
*/
BOOLEAN
EFIAPI
IsThereAnyDimmX16InChannel (
  IN  UINT8   Socket,
  IN  UINT8   Channel
  );

/**
  This function is for debug use only and should not normally be called.

  When this function is called, execution will halt for the socket until the user
  clears BIOS_SCRATCHPAD6_STRUCT.EvToolBreakPoint to 0 (Sticky scratchpad 6 BIT[31:16]).
  The contents of the EvToolBreakPoint field will be restored to its original value
  before the function exits.

  @param[in]  Socket      - Socket number to halt

  @retval   EFI_SUCCESS

**/

EFI_STATUS
EFIAPI
FixedBreakpoint (
  IN  UINT8   Socket
  );

/**
  Check to see if a given socket has a Dimm populated.

  @param[in]  Host    A pointer to the host data.
  @param[in]  Socket  The socket to check.

  @retval TRUE        The socket does have a Dimm populated
  @retval FALSE       The socket does not have a dimm populated, or the socket is disabled

**/

BOOLEAN
EFIAPI
IsSocketDimmPopulated (
  IN PSYSHOST Host,
  IN UINT8 Socket
  );

/**
  Determine whether a channel is enabled.

  @param[in]  Socket  The socket to check on.
  @param[in]  Channel The channel to check.

  @retval TRUE        The channel is enabled.
  @retval FALSE       The channel is not enabled.

**/

BOOLEAN
EFIAPI
IsChannelEnabled (
  IN UINT8 Socket,
  IN UINT8 Channel
  );

/**
  Determine whether a Dimm is present.

  @param[in]  Socket  The socket to check on.
  @param[in]  Channel The channel to check on.
  @param[in]  Dimm    The dimm to check.

  @retval TRUE        The dimm is present.
  @retval FALSE       The dimm is not present.

**/

BOOLEAN
EFIAPI
IsDimmPresent (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm
  );

/**
  Determine whether a rank is present.

  @param[in]  Socket  The socket to check on.
  @param[in]  Channel The channel to check on.
  @param[in]  Dimm    The dimm to check.
  @param[in]  Rank    The rank to check.

  @retval TRUE        The rank is present.
  @retval FALSE       The rank is not present.

**/

BOOLEAN
EFIAPI
IsRankPresent (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm,
  IN UINT8 Rank
  );

/**
  Determine whether a Data Center Persistent
  Memory Module is present in the given DIMM slot

  @param[in]  Socket  Socket ID to check
  @param[in]  Channel Channel ID to check
  @param[in]  Dimm    Slot number to check

  @retval     TRUE    DCPMM DIMM is present
  @retval     FALSE   DCPMM DIMM is not present
**/
BOOLEAN
EFIAPI
IsDcpmmPresentDimm (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm
  );

/**

  Determine whether a Data Center Persistent
  Memory Module is present in the given channel

  @param[in]  Socket  Socket ID to check
  @param[in]  Channel Channel ID to check

  @retval     TRUE    DCPMM DIMM is present
  @retval     FALSE   DCPMM DIMM is not present

**/

BOOLEAN
EFIAPI
IsDcpmmPresentChannel (
  IN UINT8 Socket,
  IN UINT8 Channel
  );

/**

  Determine whether a Data Center Persistent
  Memory Module is present in the given socket

  @param[in]  Socket  Socket ID to check

  @retval     TRUE    DCPMM DIMM is present
  @retval     FALSE   DCPMM DIMM is not present

**/

BOOLEAN
EFIAPI
IsDcpmmPresentSocket (
  IN UINT8 Socket
  );

/**

  Return the number of DDR dimms present on a channel excluding
  Data Center Persistent Memory Modules.

  @param[in]  Socket  The socket to check on.
  @param[in]  Channel The channel to check on.

  @retval UINT8 Ddr Dimms Per Channel

**/

UINT8
EFIAPI
DdrDimmsPerChannel (
  IN UINT8 Socket,
  IN UINT8 Channel
  );

/**

  Determines whether the specified memory controller is populated

  @param[in]  Socket  The socket to check on
  @param[in]  McId    The memory controller to check on

  @retval TRUE        At least one DIMM is present on the memory controller
  @retval FALSE       No DIMMs are present on the memory controller

**/

BOOLEAN
EFIAPI
IsMcPopulated (
  IN UINT8 Socket,
  IN UINT8 McId
  );

/**

  Reset the platform.

  @param[in] ResetType - Warm == 0x02, Cold == 0x04

  @retval None

**/

VOID
EFIAPI
IssueReset (
  IN UINT8 ResetType
  );

/**

  Get Last posted Major checkpoint value
  Gets value from SysHost variable Host->var.mem.lastCheckpoint

  @param[in] Socket - 0 based Socket number

  @retval UINT8 - Major checkpoint

**/

UINT8
EFIAPI
GetLastMajorCheckPoint (
  IN UINT8 Socket
  );

/**

  Get Last posted Minor checkpoint value
  Gets value from SysHost variable Host->var.mem.lastCheckpoint

  @param[in] Socket - 0 based Socket number

  @retval UINT8 - Minor checkpoint

**/

UINT8
EFIAPI
GetLastMinorCheckPoint (
  IN UINT8 Socket
  );

/**

  This API gets the value for Current socket being initialized

  @param None

  @retval UINT8  The Socket number that is currently initialized.

**/

UINT8
EFIAPI
GetCurrentSocketId (
  VOID
  );

/**

  This API Check and returns status of Turnaround Initialization completion

  @param None

  @retval TRUE  - Turnaround Init done
  @retval FALSE - Turnaround Init not done

**/

BOOLEAN
EFIAPI
IsTurnAroundInitDone (
  IN UINT8 Socket
  );

/**
  Return the last known training step running on the socket.

  @param[in] Socket   - The socket number

  @retval The last known training step run on this socket

**/
MRC_TT
EFIAPI
GetCurrentTestType (
  IN  UINT8   Socket
  );

/**
  Set the current training step for the socket.

  @param[in] Socket   - The socket number
  @param[in] TestType - The test type to set

  @retval N/A

**/
VOID
EFIAPI
SetCurrentTestType (
  IN  UINT8   Socket,
  IN  MRC_TT  TestType
  );

/**

  Determines if this rank is valid.

  @param[in] Socket           - Socket to check
  @param[in] Channel          - Channel to check
  @param[in] Dimm             - DIMM to check
  @param[in] Rank             - Rank to check
  @param[in] IncludeMappedOut - Whether to include mapped out ranks

  @retval TRUE - Rank is valid
  @retval FALSE - Rank is not valid

**/

BOOLEAN
EFIAPI
IsRankValid (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN BOOLEAN   IncludeMappedOut
  );

/**

  Returns the number of ranks for a dimm.

  @param[in] Socket           - Socket to check
  @param[in] Channel          - Channel to check
  @param[in] Dimm             - DIMM to check

  @retval Number of Ranks

**/

UINT8
EFIAPI
GetNumberOfRanksOnDimm (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm
  );

/**

  Check if a socket is enabled.

  @param[in] Socket - Socket to check

  @retval Whether socket is enabled or not

**/

BOOLEAN
EFIAPI
IsSocketEnabled (
  IN UINT8     Socket
  );

/**

  Check if a DIMM is X4.

  @param[in] Socket   - Socket to check
  @param[in] Channel  - Channel to check
  @param[in] Dimm     - DIMM number in the channel

  @retval Whether dimm is X4 or not

**/

BOOLEAN
EFIAPI
IsX4Dimm (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm
  );

/**

  Check if a DIMM is X16

  @param[in] Socket   - Socket to check
  @param[in] Channel  - Channel to check
  @param[in] Dimm     - DIMM number in the channel

  @retval Whether dimm is X16 or not

**/
BOOLEAN
EFIAPI
IsX16Dimm (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm
  );

/**

  Get the number of dimms in a socket.

  @param[in] Socket  - Socket to check

  @retval Number of dimms in a socket.

**/

UINT8
EFIAPI
GetMaxDimmPop (
  IN UINT8     Socket
  );

/**

  Get maximum channel number of socket per Memory technology type

  @param[in] MemTechType    - Memory technology type

  @return the maximum channel number of socket

**/
UINT8
EFIAPI
GetMaxChPerTechType (
  IN MEM_TECH_TYPE  MemTechType
  );

/**

  Determine whether a channel is enabled per Memory technology type

  @param[in]  MemTechType - Memory technology type
  @param[in]  Socket      - The socket to check on.
  @param[in]  Channel     - The channel to check.

  @retval TRUE        The channel is enabled.
  @retval FALSE       The channel is not enabled.

**/
BOOLEAN
EFIAPI
IsChEnabledPerTechType (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          Socket,
  IN UINT8          Channel
  );

/**
  Get the rank index for a DIMM rank.

  @param[in] Socket           - Socket to check.
  @param[in] Channel          - Channel to check.
  @param[in] Dimm             - DIMM to check.
  @param[in] Rank             - Rank to check.

  @return   Rank index of the DIMM rank.
**/
UINT8
GetRankIndex (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     Rank
  );

/**
  Function to check if Both DDR and DDRT present in given channel

  @param[in] Socket - Socket number
  @param[in] Ch     - Channel number

  @retval TRUE  - DDRT and DDR4 both present
          FALSE  - Otherwise

**/
BOOLEAN
EFIAPI
IsDdrAndDdrtPresent (
  IN UINT8 Socket,
  IN UINT8 Ch
  );

/**
  If 3DS dimm present in given Socket\Channel return TRUE
  otherwise return FALSE

  @param[in] Socket   - Socket number
  @param[in] Channel  - Channel number

  @retval TRUE  - 3DS present
          FALSE - 3DS not present
**/
BOOLEAN
EFIAPI
Is3dsDimmPresentInChannel (
  IN UINT8 Socket,
  IN UINT8 Channel
  );

/**
  Returns SDRAM Capacity of given dimm

  @param[in] Socket   - Socket number
  @param[in] Channel  - Channel number
  @param[in] Dimm     - Dimm number

  @retval UINT8 - SDRAM Capacity defines
**/
UINT8
EFIAPI
GetDramCapacity (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm
  );

/**
  Check if 16GB 3DS dimm present in given channel

  @param[in] Socket  - Socket number
  @param[in] Channel - Channel number

  @retval  TRUE  - 16GB3DS dimm present
           FALSE - 16GB3DS dimm not present

**/
BOOLEAN
EFIAPI
Is16Gb3DsDimmPresent (
  IN UINT8 Socket,
  IN UINT8 Channel
  );

/**
  Get QCS QCA copy number of the DIMM.

  @param[in] Socket           - Socket to check.
  @param[in] Channel          - Channel to check.
  @param[in] Dimm             - DIMM to check.

  @return   Copy number of the DIMM.
**/
UINT8
EFIAPI
GetMaxBacksideCopyNum (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm
  );

/**
  Function to return Minimum of Non Zero value in given Array

  @param[in] InputValue - An array consisting of input values
  @param[in] InputSize  - Array Size

  @retval Non Zero - Found minimum Value
  @retval Zero     - Array content is 0

**/
UINT32
EFIAPI
GetMinimumOfNonZeroNumberN (
  IN UINT32 InputValue [],
  IN UINT32 InputSize
  );

/**

  Determines if CMD signals are normalized or not

  @param[in] Socket    - Current Socket

  @retval TRUE         - CMD signals are normalized
  @retval FALSE        - CMD signals are not normalized

**/
BOOLEAN
EFIAPI
IsCmdNormalized (
  IN UINT8    Socket
  );

/**

  Set NormalizationFactorEn

  @param[in] Socket    - Current Socket
  @param[in] FactorEn  - Normalization Status
                         1 : normalized
                         0 : not normalized

  @retval N/A

**/
VOID
EFIAPI
SetNormalizationFactorEn (
  IN UINT8    Socket,
  IN INT8     FactorEn
  );

/**

  Converts any raw number (in Two's Complement format) to a signed number

  @param[in]    RawValue          Number in complement's 2 format
  @param[in]    SignBitPosition   Bit position that represents the sign

  @return       Signed number that represents the raw value

**/
INT32
EFIAPI
ConvertTwosComplementToInt (
  IN        UINT32          RawValue,
  IN        UINT8           SignBitPosition
  );
/**
   Get Max valid Sub channel mask

  @retval MaxValidSubChMask Bitmap of enabled Subchannel

**/

UINT8
EFIAPI
GetMaxValidSubChannelMask (
  );
#endif // _MEMORY_SERVICES_LIB_H_
