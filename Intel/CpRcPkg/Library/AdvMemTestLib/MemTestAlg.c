/** @file

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

#include "MemTestFunc.h"
#include <Library/AdvMemTestLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Memory/JedecDefinitions.h>
#include <Library/MemRcLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <ReferenceCodeFatalErrors.h>

#define AMT_RETRY_COUNT   10

//
// Code section
//

/**
  Check whether any Faulty Part is logged from training and skip Advanced Memtest on those channels

  @param[in]  Host    A pointer to the host data
  @param[in]  Socket  The socket to check
  @param[in/out] ddrChEnabled - Pointer to List of channels to be tested

  @retval TRUE        The socket does have a Dimm populated
  @retval FALSE       The socket does not have a dimm populated, or  the socket is disabled
**/

BOOLEAN
EFIAPI
IsFaultyPartLogged (
  IN     PSYSHOST Host,
  IN     UINT8    Socket,
  IN OUT UINT8   *ddrChEnabled
  )
{
  UINT8 ch;
  BOOLEAN status = FALSE;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr();
  for (ch = 0; ch < MaxChDdr; ch++) {
    if (*ddrChEnabled & (1 << ch)) {

      if (Host->nvram.mem.socket[Socket].FaultyPartsFlag[ch] != 0) {

        RcDebugPrintWithDevice (SDBG_MAX, Socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "Skipping Advanced Memtest on channel %d due to Faulty Part log\n", ch);

        *ddrChEnabled &= ~(1 << ch);
        status = TRUE;
      }
    }
  }
  return status;

} // IsFaultyPartLogged

/**

  Runs Advanced Memory Tests based on the legacy Scan algorithm with DDR scrambler enabled

  @param Host                - Pointer to sysHost
  @param socket              - Socket Id
  @param ddrChEnabled        - List of channels to be tested
  @param testType            - Memory test type
  @param testSize            - Memory test size
  @param shiftRate           - Rate to shift the WDB content
  @param repeatTest          - Current test iteration number

  @retval status             - SUCCESS/FAILURE

**/
UINT32
MemTestScram (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ddrChEnabled,
  UINT8    testType,
  UINT8    testSize,
  UINT8    shiftRate,
  UINT32   repeatTest
  )
{
  UINT64_STRUCT    patternQW[MT_PATTERN_DEPTH];
  UINT64_STRUCT    shiftedQW[MT_PATTERN_DEPTH];
  UINT64_STRUCT    basePatternQW[MT_PATTERN_DEPTH];
  UINT8  uiShl[MT_PATTERN_DEPTH];
  UINT8  seqDataInv[MT_MAX_SUBSEQ];
  UINT8  numCl;
  UINT8  TestSizeLocal;
  UINT8  RepairDone;
  UINT32 RetryCount;
  UINT64 startTsc;
  UINT64 endTsc;
  UINT32 status = SUCCESS;

  RcDebugPrintWithDevice (SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "MemTestScram Starts\n");

  if (testType == ADV_MT_TYPE_RANGE_TEST_SCRAM) {
    if (IsFaultyPartLogged (Host, socket, &ddrChEnabled)) {
      return FAILURE;
    }
  }

  basePatternQW[0].hi = 0x55AA55AA;
  basePatternQW[0].lo = 0x55AA55AA;
  numCl = 1;

  //
  // Based on the pattern info, the test size for hardware support check is 16, 55AA 55AA ...
  //
  TestSizeLocal = 16;

  if (!CheckHwSupportedForMATSAndFixupCacheLine (TestSizeLocal, &numCl)) {
    return FAILURE;
  }

  // Add long-term pattern uniqueness
  basePatternQW[0] = AddUINT64x32 (basePatternQW[0], repeatTest >> 7);

  // Rotate pattern based on iteration
  shiftedQW[0] = RotateLeftUINT64 (basePatternQW[0], (repeatTest >> 1) % 64);

  // Invert pattern for odd iterations
  if (repeatTest % 2 == 0) {
    patternQW[0].hi = shiftedQW[0].hi;
    patternQW[0].lo = shiftedQW[0].lo;
  } else {
    patternQW[0].hi = ~shiftedQW[0].hi;
    patternQW[0].lo = ~shiftedQW[0].lo;
  }

  SetMem (uiShl, sizeof (uiShl), 0);
  uiShl[0] = shiftRate;
  uiShl[1] = shiftRate;
  seqDataInv[0] = 0;
  seqDataInv[1] = 0;

  RcDebugPrint (SDBG_MAX,
                          "patternQW[0] = 0x%08x%08x\n",
                          patternQW[0].hi, patternQW[0].lo);

  startTsc = 0;
  endTsc = 0;
  RepairDone = 1;
  RetryCount = 0;
  while (RepairDone && (RetryCount++ < AMT_RETRY_COUNT)) {


    //1. Write pattern to all of memory
    if (status == SUCCESS) {
      OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP1, 0);
      startTsc = GetPerformanceCounter ();
      status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_WRITE, FAST_Y, BG_INTER_2, seqDataInv, patternQW, FALSE, uiShl, numCl, 1, MT_ADDR_DIR_UP);
      endTsc = GetPerformanceCounter ();
      RcDebugPrint (SDBG_MAX,
                      "Step1 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
    }
    if (status == SUCCESS) {
      status = InjectErrorBeforeScramStep2 (Host, socket, ddrChEnabled, testType, patternQW, FALSE, uiShl, numCl);
    }

    //2. Read and compare data pattern
    if (status == SUCCESS) {
      OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP2, 0);
      startTsc = GetPerformanceCounter ();
      status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_READ, FAST_Y, BG_INTER_2, seqDataInv, patternQW, FALSE, uiShl, numCl, 1, MT_ADDR_DIR_UP);
      endTsc = GetPerformanceCounter ();
      RcDebugPrint (SDBG_MAX,
                      "Step2 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
    }
    if (status == SUCCESS) {
      OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP3, 0);
      //
      // Disposition any failures
      //
      CpgcMemTestDispositionFailRange (Host, socket, ddrChEnabled, testType, BG_INTER_2, &RepairDone);
    } else {
      RcDebugPrint (SDBG_MINMAX,
        "MemTest Failed!\n");
    }
  } // while (RepairDone && (RetryCount++ < AMT_RETRY_COUNT))

  RcDebugPrintWithDevice (SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "MemTestScram Ends\n");
  return status;
} // MemTestScram


/**

  Runs optimized Memory Init to 0 data pattern with DDR scrambler enabled

  @param Host                - Pointer to sysHost
  @param socket              - Socket Id
  @param ddrChEnabled        - List of channels to be initialized

  @retval status             - SUCCESS/FAILURE

**/
UINT32
MemInitOpt (
  PSYSHOST Host,
  UINT8    socket,
  UINT32   ddrChEnabled
  )
{
  UINT64_STRUCT    basePatternQW[MT_PATTERN_DEPTH];
  UINT8  uiShl[MT_PATTERN_DEPTH];
  UINT8  seqDataInv[MT_MAX_SUBSEQ];
  UINT8  numCl;
  UINT8  testType = ADV_MT_TYPE_LINIT_SCRAM;
  UINT32 status = SUCCESS;

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "MemInitOpt Starts\n");

  basePatternQW[0].hi = 0;
  basePatternQW[0].lo = 0;
  numCl = 1;
  SetMem (uiShl, sizeof (uiShl), 0);
  seqDataInv[0] = 0;
  seqDataInv[1] = 0;

  RcDebugPrint (SDBG_MAX,
                  "patternQW[0] = 0x%08x%08x\n",
                  basePatternQW[0].hi, basePatternQW[0].lo);

  //1. Write pattern to all of memory
  OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP1, 0);
  status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_WRITE, FAST_Y, BG_INTER_2, seqDataInv, basePatternQW, FALSE, uiShl, numCl, 1, MT_ADDR_DIR_UP);
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "MemInitOpt Ends\n");

  if (status == SUCCESS) {
    Host->var.mem.IsDdrMemInitDone = TRUE;
  }

  return status;
} // MemInitOpt


/**

  Runs Advanced Memory Tests based on the MATS + algorithm

  @param Host                - Pointer to sysHost
  @param socket              - Socket Id
  @param ddrChEnabled        - List of channels to be tested
  @param testType            - Memory test type
  @param TestSize            - Memory test size
  @param shiftRate           - Rate to shift the WDB content

  @retval status             - SUCCESS/FAILURE

**/
UINT32
MemTestMATSN (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ddrChEnabled,
  UINT8    testType,
  UINT8    TestSize,
  UINT8    shiftRate,
  BOOLEAN  InvertedPassEn
  )
{
  UINT64_STRUCT    patternQW[MT_PATTERN_DEPTH];
  UINT64_STRUCT    inverseQW[MT_PATTERN_DEPTH];
  UINT64_STRUCT    shiftedQW[MT_PATTERN_DEPTH];
  UINT64_STRUCT    basePatternQW[MT_PATTERN_DEPTH];
  UINT8  uiShl[MT_PATTERN_DEPTH];
  UINT8  seqDataInv[MT_MAX_SUBSEQ];
  UINT8  numCl;
  UINT32 TestLoop;
  UINT32 TestBit;
  UINT32 status = SUCCESS;
  UINT64 startTsc = 0;
  UINT64 endTsc = 0;
  UINT8  RepairDone;
  UINT32 RetryCount;
  UINT8  AddressMode = FAST_Y;
  AMT_TEST_CONDITION PrevAmtTestCond;

  RcDebugPrint (SDBG_MINMAX,
                  "\nMemTestMATS%d TestType %d Starts\n", TestSize, testType);

  IsFaultyPartLogged (Host, socket, &ddrChEnabled);

  SetAdvMemTestCondition (Host, socket, ddrChEnabled, testType, AMT_SET_TC, &PrevAmtTestCond);

  /*
  // Patterns
  8-bit Patterns          16-bit Patterns         32-bit Patterns         64-bit Patterns

  0101010101010101h       0001000100010001h       0000000100000001h       0000000000000001h
  0202020202020202h       0002000200020002h       0000000200000002h       0000000000000002h
  0404040404040404h       0004000400040004h       0000000400000004h       0000000000000004h
  0808080808080808h       0008000800080008h       0000000800000008h       0000000000000008h
  1010101010101010h         ...                     ...                     ...
  2020202020202020h
  4040404040404040h       4000400040004000h       4000000040000000h       4000000000000000h
  8080808080808080h       8000800080008000h       8000000080000000h       8000000000000000h
  0101010101010101h       0001000100010001h       0000000100000001h       0000000000000001h
  ...                     ...                     ...                     ...

  // XMATS algorithm
  (1)
    FOR each 64-bit memory location (increasing sequentially through memory)
      Write sliding data pattern to all of memory
    END FOR
  (2)
    FOR each 64-bit memory location (increasing sequentially through memory)
      read and compare pattern
      IF compare fails
        EXIT with failure message
      write inverse pattern
    END FOR
  (3)
    FOR each 64-bit memory location (decreasing sequentially through memory)
      read and compare inverse pattern
      IF compare fails
        EXIT with failure message
      write original pattern
    END FOR
  (4)
    FOR each 64-bit memory location (increasing sequentially through memory)
      write inverse pattern
    END FOR
  (5)
    FOR each 64-bit memory location (increasing sequentially through memory)
      read and compare inverse pattern
      IF compare fails
        EXIT with failure message
      write original pattern
    END FOR
  (6)
    FOR each 64-bit memory location (decreasing sequentially through memory)
      read and compare original pattern
      IF compare fails
        EXIT with failure message
      Write complement pattern
    END FOR
  */

  switch (TestSize) {
  case 1:
  case 8:
    basePatternQW[0].hi = 0x01010101;
    basePatternQW[0].lo = 0x01010101;
    numCl = 1;
    break;
  case 16:
    basePatternQW[0].hi = 0x00010001;
    basePatternQW[0].lo = 0x00010001;
    numCl = 2;
    break;
  case 32:
    basePatternQW[0].hi = 0x00000001;
    basePatternQW[0].lo = 0x00000001;
    numCl = 4;
    break;
  case 64:
    basePatternQW[0].hi = 0x00000000;
    basePatternQW[0].lo = 0x00000001;
    numCl = 8;
    break;
  default:
    numCl = 0;
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_66);
  }

  if (!CheckHwSupportedForMATSAndFixupCacheLine (TestSize, &numCl)) {
    return FAILURE;
  }

  // SW loop for TestSize larger than numCl
  for (TestLoop = 0; TestLoop < TestSize; TestLoop = TestLoop + (numCl * 8)) {
    // Test each bit position per UI
    for (TestBit = 0; (TestBit < (UINT32)(numCl * 8)) && (TestBit + TestLoop < TestSize); TestBit++) {

      //
      // Shift left basePatternQW by TestLoop positions
      //
      shiftedQW[0] = LShiftUINT64 (basePatternQW[0], TestBit);
      //
      // Define the test pattern
      //
      patternQW[0].hi = shiftedQW[0].hi;
      patternQW[0].lo = shiftedQW[0].lo;
      //
      // Set the inverse test pattern
      //
      inverseQW[0].hi = ~shiftedQW[0].hi;
      inverseQW[0].lo = ~shiftedQW[0].lo;
      SetMem (uiShl, sizeof (uiShl), 0);
      uiShl[0] = shiftRate;
      uiShl[1] = shiftRate;
      seqDataInv[0] = 0;
      seqDataInv[1] = 1;

      RcDebugPrint (SDBG_MAX,
                      "patternQW[0] = 0x%08x%08x, inverseQW[0] = 0x%08x%08x\n",
                      patternQW[0].hi, patternQW[0].lo, inverseQW[0].hi, inverseQW[0].lo);

      startTsc = 0;
      endTsc = 0;
      RepairDone = 1;
      RetryCount = 0;
      while (RepairDone && (RetryCount++ < AMT_RETRY_COUNT)) {

        //
        //1. Write sliding data pattern to all of memory
        //
        if (status == SUCCESS) {
          OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP1, (UINT16) (TestLoop + TestBit));
          startTsc = GetPerformanceCounter ();
          status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_WRITE, AddressMode, BG_INTER_2, seqDataInv, patternQW, FALSE, uiShl, numCl, 1, MT_ADDR_DIR_UP);
          endTsc = GetPerformanceCounter ();
          RcDebugPrint (SDBG_MAX,
                          "Step1 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
        }
        if (status == SUCCESS) {
          status = AdvMemTestPauseCondition (Host, socket, ddrChEnabled, testType);
        }
        if (status == SUCCESS) {
          status = InjectErrorBeforeMatsnStep2 (Host, socket, ddrChEnabled, testType, patternQW, FALSE, uiShl, numCl, TestBit);
        }
        //
        //2. Read and compare data pattern
        //   Write inverse pattern
        //
        if (status == SUCCESS) {
          OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP2, (UINT16) (TestLoop + TestBit));
          startTsc = GetPerformanceCounter ();
          if ((InvertedPassEn == TRUE) && (FeaturePcdGet (PcdCteBuild) == FALSE)) {
            status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_READ_WRITE, AddressMode, BG_INTER_2, seqDataInv, patternQW, FALSE, uiShl, numCl, 1, MT_ADDR_DIR_UP);
          } else {
            //
            // Modified step: Use down direction to pressure tWR on upper BG address
            //
            status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_READ_WRITE, AddressMode, BG_INTER_2, seqDataInv, patternQW, FALSE, uiShl, numCl, 1, MT_ADDR_DIR_DN);
          }
          endTsc = GetPerformanceCounter ();
          RcDebugPrint (SDBG_MAX,
                          "Step2 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
        }
        if (status == SUCCESS) {
          status = AdvMemTestPauseCondition (Host, socket, ddrChEnabled, testType);
        }
        if (status == SUCCESS) {
          InjectErrorBeforeMatsnStep3 (Host, socket, ddrChEnabled, testType, inverseQW, TRUE, uiShl, numCl, TestBit);
        }
        //
        //3. Read and compare inverse pattern
        //   Write original pattern
        //
        if (status == SUCCESS) {
          OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP3, (UINT16) (TestLoop + TestBit));
          startTsc = GetPerformanceCounter ();
          if ((InvertedPassEn == TRUE) && (FeaturePcdGet (PcdCteBuild) == FALSE)) {
            status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_READ_WRITE, AddressMode, BG_INTER_2, seqDataInv, inverseQW, TRUE, uiShl, numCl, 1, MT_ADDR_DIR_DN);
          } else {
            //
            // Modified step: Just read and compare the inverse pattern without writting the original pattern
            //
            status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_READ, AddressMode, BG_INTER_2, seqDataInv, inverseQW, TRUE, uiShl, numCl, 1, MT_ADDR_DIR_UP);
          }
          endTsc = GetPerformanceCounter ();
          RcDebugPrint (SDBG_MAX,
                          "Step3 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
        }

        //
        // Make another MATS+ pass with inverted data
        //
        if ((InvertedPassEn == TRUE) && (FeaturePcdGet (PcdCteBuild) == FALSE)) {

          //
          //4. Write inverse pattern to all of memory
          //
          if (status == SUCCESS) {
            OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP4, (UINT16) (TestLoop + TestBit));
            startTsc = GetPerformanceCounter ();
            status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_WRITE, AddressMode, BG_INTER_2, seqDataInv, inverseQW, TRUE, uiShl, numCl, 1, MT_ADDR_DIR_UP);
            endTsc = GetPerformanceCounter ();
            RcDebugPrint (SDBG_MAX,
                            "Step4 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
          }
          if (status == SUCCESS) {
            status = AdvMemTestPauseCondition (Host, socket, ddrChEnabled, testType);
          }
          //
          //5. Read and compare inverse pattern
          //   Write original pattern
          //
          if (status == SUCCESS) {
            OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP5, (UINT16) (TestLoop + TestBit));
            startTsc = GetPerformanceCounter ();
            status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_READ_WRITE, AddressMode, BG_INTER_2, seqDataInv, inverseQW, TRUE, uiShl, numCl, 1, MT_ADDR_DIR_UP);
            endTsc = GetPerformanceCounter ();
            RcDebugPrint (SDBG_MAX,
                            "Step5 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
          }
          if (status == SUCCESS) {
            status = AdvMemTestPauseCondition (Host, socket, ddrChEnabled, testType);
          }
          //
          //6. Read and compare original pattern
          //   Write inverse pattern
          //
          if (status == SUCCESS) {
            OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP6, (UINT16) (TestLoop + TestBit));
            startTsc = GetPerformanceCounter ();
            status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_READ_WRITE, AddressMode, BG_INTER_2, seqDataInv, patternQW, FALSE, uiShl, numCl, 1, MT_ADDR_DIR_DN);
            endTsc = GetPerformanceCounter ();
            RcDebugPrint (SDBG_MAX,
                            "Step6 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
          }
        } // PcdCteBuild

        //
        // Verify that the last test was successful
        //
        if (status == SUCCESS) {
          OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP7, (UINT16) (TestLoop + TestBit));
          //
          // Disposition any failures
          //
          CpgcMemTestDispositionFailRange (Host, socket, ddrChEnabled, testType, BG_INTER_2, &RepairDone);
        } else {
          RcDebugPrint (SDBG_MINMAX,
            "MemTestMATS%d: Test Failed!\n", TestSize);
          goto FailurePath;
        }
      } // while (RepairDone && (RetryCount++ < AMT_RETRY_COUNT))
    } // TestBit
  } // TestLoop

  FailurePath:

  SetAdvMemTestCondition (Host, socket, ddrChEnabled, testType, AMT_RESTORE_TC, &PrevAmtTestCond);

  //
  // Validation hook to Reset row fail list (useful for testing multiple options)
  //
  ResetRowFailureList (Host, socket);

  RcDebugPrint (SDBG_MINMAX,
                  "\nMemTestMATS%d TestType %d Ends\n", TestSize, testType);

  return status;
} // MemTestMATSN


/**

  Runs Advanced Memory Tests based on the MCH algorithm

  @param Host                - Pointer to sysHost
  @param socket              - Socket Id
  @param ddrChEnabled        - List of channels to be tested
  @param testType            - Memory test type
  @param testSize            - Memory test size
  @param mchRepeat           - Number of times to repeat each pattern

  @retval status             - SUCCESS/FAILURE

**/
UINT32
MemTestMCH (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ddrChEnabled,
  UINT8    testType,
  UINT8    testSize,
  UINT8    mchRepeat
  )
{
  UINT64_STRUCT    patternQW[MT_PATTERN_DEPTH];
  UINT64_STRUCT    inverseQW[MT_PATTERN_DEPTH];
  UINT64_STRUCT    shiftedQW[MT_PATTERN_DEPTH];
  UINT64_STRUCT    basePatternQW[MT_PATTERN_DEPTH];
  UINT8  uiShl[MT_PATTERN_DEPTH];
  UINT8  seqDataInv[MT_MAX_SUBSEQ];
  UINT8  numCl;
  UINT8  repeat;
  UINT32 i;
  UINT8  RepairDone;
  UINT32 RetryCount;
  UINT32 status = SUCCESS;
  UINT64 startTsc;
  UINT64 endTsc;
  AMT_TEST_CONDITION PrevAmtTestCond;

  RcDebugPrint (SDBG_MINMAX,
                  "\nMemTestMCH%d Starts\n", testSize);

  IsFaultyPartLogged (Host, socket, &ddrChEnabled);

  SetAdvMemTestCondition (Host, socket, ddrChEnabled, testType, AMT_SET_TC, &PrevAmtTestCond);

  /*
  initialize pattern to 0101010101010101h

  (1)
    FOR each 64-bit memory location (increasing sequentially through memory)
      write pattern
    END FOR
  (2)
    FOR each 64-bit memory location (increasing sequentially through memory)
      read and compare to pattern
      IF compare fails
        EXIT with failure message
      write pattern (purposely same value)
    END FOR
  (3)
    FOR each 64-bit memory location (increasing sequentially through memory)
      read and compare to pattern
      IF compare fails
        EXIT with failure message
    END FOR
  (4)
    FOR each 64-bit memory location (increasing sequentially through memory)
      write pattern
    END FOR
  (5)
    FOR each 64-bit memory location (increasing sequentially through memory)
      write inverse pattern
    END FOR
  (6)
    FOR each 64-bit memory location (increasing sequentially through memory)
      read and compare to inverse pattern
      IF compare fails
        EXIT with failure message
    END FOR
  (7)
    FOR each 64-bit memory location (increasing sequentially through memory)
      write pattern
    END FOR
  (8)
    FOR each 64-bit memory location (decreasing sequentially through memory)
      write inverse pattern
    END FOR
  (9)
    FOR each 64-bit memory location (increasing sequentially through memory)
      read and compare to inverse pattern
      IF compare fails
        EXIT with failure message
    END FOR

  Run the above twice and then rotate the pattern LEFT. Stop after the pattern
  has been rotated 8-bits (16 iterations of the loop).
  */

  switch (testSize) {
  case 8:
    basePatternQW[0].hi = 0x01010101;
    basePatternQW[0].lo = 0x01010101;
    numCl = 1;
    break;
  case 16:
    basePatternQW[0].hi = 0x00010001;
    basePatternQW[0].lo = 0x00010001;
    numCl = 2;
    break;
  case 32:
    basePatternQW[0].hi = 0x00000001;
    basePatternQW[0].lo = 0x00000001;
    numCl = 4;
    break;
  case 64:
    basePatternQW[0].hi = 0x00000000;
    basePatternQW[0].lo = 0x00000001;
    numCl = 8;
    break;
  default:
    numCl = 0;
    basePatternQW[0].hi = 0x00000000;
    basePatternQW[0].lo = 0x00000000;
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_67);
  }

  if (!CheckHwSupportedForMATSAndFixupCacheLine (testSize, &numCl)) {
    return FAILURE;
  }

  for (i = 0; i < testSize; i++) {

    shiftedQW[0] = LShiftUINT64 (basePatternQW[0], i);

    patternQW[0].hi = shiftedQW[0].hi;
    patternQW[0].lo = shiftedQW[0].lo;
    inverseQW[0].hi = ~shiftedQW[0].hi;
    inverseQW[0].lo = ~shiftedQW[0].lo;
    SetMem (uiShl, sizeof (uiShl), 0);
    seqDataInv[0] = 0;
    seqDataInv[1] = 0;

    RcDebugPrint (SDBG_MAX,
                    "patternQW[0] = 0x%08x%08x, inverseQW[0] = 0x%08x%08x\n",
                    patternQW[0].hi, patternQW[0].lo, inverseQW[0].hi, inverseQW[0].lo);

    for (repeat = 0; repeat < mchRepeat; repeat++) {

      startTsc = 0;
      endTsc = 0;
      RepairDone = 1;
      RetryCount = 0;
      while (RepairDone && (RetryCount++ < AMT_RETRY_COUNT)) {

        //1. Write data pattern to all of memory
        if (status == SUCCESS) {
          OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP1, (UINT16) i);
          startTsc = GetPerformanceCounter ();
          status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_WRITE, FAST_Y, BG_INTER_2, seqDataInv, patternQW, FALSE, uiShl, numCl, 1, MT_ADDR_DIR_UP);
          endTsc = GetPerformanceCounter ();
          RcDebugPrint (SDBG_MAX,
                          "Step1 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
        }
        if (status == SUCCESS) {
          status = AdvMemTestPauseCondition (Host, socket, ddrChEnabled, testType);
        }
        //
        //2. Read and compare data pattern
        //   Write same data pattern
        //
        if (status == SUCCESS) {
          OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP2, (UINT16) i);
          startTsc = GetPerformanceCounter ();
          status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_READ_WRITE, FAST_Y, BG_INTER_2, seqDataInv, patternQW, FALSE, uiShl, numCl, 1, MT_ADDR_DIR_UP);
          endTsc = GetPerformanceCounter ();
          RcDebugPrint (SDBG_MAX,
                          "Step2 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
        }
        if (status == SUCCESS) {
          status = AdvMemTestPauseCondition (Host, socket, ddrChEnabled, testType);
        }
        //3. Read and compare data pattern
        if (status == SUCCESS) {
          OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP3, (UINT16) i);
          startTsc = GetPerformanceCounter ();
          status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_READ, FAST_Y, BG_INTER_2, seqDataInv, patternQW, FALSE, uiShl, numCl, 1, MT_ADDR_DIR_UP);
          endTsc = GetPerformanceCounter ();
          RcDebugPrint (SDBG_MAX,
                          "Step3 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
        }
        //4. Write data pattern to all of memory
        if (status == SUCCESS) {
          OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP4, (UINT16) i);
          startTsc = GetPerformanceCounter ();
          status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_WRITE, FAST_Y, BG_INTER_2, seqDataInv, patternQW, FALSE, uiShl, numCl, 1, MT_ADDR_DIR_UP);
          endTsc = GetPerformanceCounter ();
          RcDebugPrint (SDBG_MAX,
                          "Step4 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
        }
        //5. Write inverse pattern to all of memory
        if (status == SUCCESS) {
        OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP5, (UINT16) i);
          startTsc = GetPerformanceCounter ();
          status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_WRITE, FAST_Y, BG_INTER_2, seqDataInv, inverseQW, TRUE, uiShl, numCl, 1, MT_ADDR_DIR_UP);
          endTsc = GetPerformanceCounter ();
          RcDebugPrint (SDBG_MAX,
                          "Step5 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
        }
        if (status == SUCCESS) {
          status = AdvMemTestPauseCondition (Host, socket, ddrChEnabled, testType);
        }
        //6. Read and compare inverse pattern
        if (status == SUCCESS) {
          OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP6, (UINT16) i);
          startTsc = GetPerformanceCounter ();
          status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_READ, FAST_Y, BG_INTER_2, seqDataInv, inverseQW, TRUE, uiShl, numCl, 1, MT_ADDR_DIR_UP);
          endTsc = GetPerformanceCounter ();
          RcDebugPrint (SDBG_MAX,
                          "Step6 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
        }
        //7. Write data pattern to all of memory
        if (status == SUCCESS) {
          OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP7, (UINT16) i);
          startTsc = GetPerformanceCounter ();
          status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_WRITE, FAST_Y, BG_INTER_2, seqDataInv, patternQW, FALSE, uiShl, numCl, 1, MT_ADDR_DIR_UP);
          endTsc = GetPerformanceCounter ();
          RcDebugPrint (SDBG_MAX,
                          "Step7 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
        }
        //8. Write inverse pattern to all of memory
        if (status == SUCCESS) {
          OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP8, (UINT16) i);
          startTsc = GetPerformanceCounter ();
          status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_WRITE, FAST_Y, BG_INTER_2, seqDataInv, inverseQW, TRUE, uiShl, numCl, 1, MT_ADDR_DIR_DN);
          endTsc = GetPerformanceCounter ();
          RcDebugPrint (SDBG_MAX,
                          "Step8 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
        }
        if (status == SUCCESS) {
          status = AdvMemTestPauseCondition (Host, socket, ddrChEnabled, testType);
        }
        //9. Read and compare inverse pattern
        if (status == SUCCESS) {
          OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP9, (UINT16) i);
          startTsc = GetPerformanceCounter ();
          status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_READ, FAST_Y, BG_INTER_2, seqDataInv, inverseQW, TRUE, uiShl, numCl, 1, MT_ADDR_DIR_UP);
          endTsc = GetPerformanceCounter ();
          RcDebugPrint (SDBG_MAX,
                          "Step9 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
        }
        //
        // Verify that the last test was successful
        //
        if (status == SUCCESS) {
          OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP10, (UINT16) i);
          //
          // Disposition any failures
          //
          CpgcMemTestDispositionFailRange (Host, socket, ddrChEnabled, testType, BG_INTER_2, &RepairDone);
        } else {
          RcDebugPrint (SDBG_MINMAX,
            "MemTestMCH%d: Test Failed!\n", testSize);
          goto FailurePath;
        }
      } // while (RepairDone && (RetryCount++ < AMT_RETRY_COUNT))
    } // repeat
  }  // i

  FailurePath:

  SetAdvMemTestCondition (Host, socket, ddrChEnabled, testType, AMT_RESTORE_TC, &PrevAmtTestCond);

  //
  // Validation hook to Reset row fail list (useful for testing multiple options)
  //
  ResetRowFailureList (Host, socket);

  RcDebugPrint (SDBG_MINMAX,
                  "\nMemTestMCH%d Ends\n", testSize);

  return status;
}  //MemTestMCH

/**

  Runs Advanced Memory Tests based on the GNDB algorithm

  @param Host                - Pointer to sysHost
  @param socket              - Socket Id
  @param ddrChEnabled        - List of channels to be tested
  @param testType            - Memory test type
  @param testSize            - Memory test size

  @retval status             - SUCCESS/FAILURE

**/
UINT32
MemTestGNDB (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ddrChEnabled,
  UINT8    testType,
  UINT8    testSize
  )
{
  UINT64_STRUCT    patternQW[MT_PATTERN_DEPTH];
  UINT64_STRUCT    shiftedQW[MT_PATTERN_DEPTH];
  UINT64_STRUCT    basePatternQW[MT_PATTERN_DEPTH];
  UINT8  uiShl[MT_PATTERN_DEPTH];
  UINT8  seqDataInv[MT_MAX_SUBSEQ];
  UINT8  numCl;
  UINT32 i;
  UINT8  pass;
  UINT8  RepairDone;
  UINT32 RetryCount;
  UINT32 status = SUCCESS;
  UINT64 startTsc;
  UINT64 endTsc;
  AMT_TEST_CONDITION PrevAmtTestCond;

  RcDebugPrint (SDBG_MINMAX,
                  "\nMemTestGNDB%d Starts\n", testSize);

  IsFaultyPartLogged (Host, socket, &ddrChEnabled);

  SetAdvMemTestCondition (Host, socket, ddrChEnabled, testType, AMT_SET_TC, &PrevAmtTestCond);

  /*
  pattern set 1   00000000 00000000               ;Full bus-width all low
                  00000000 00000001               ;Bit 0 high
                  00000000 00000000               ;Full bus-width all low
                  00000000 00000002               ;Bit 1 high
                  ...
                  00000000 80000000               ;Bit 31 high
                  00000000 00000000
                  00000001 00000000               ;Bit 32 high
                  00000000 00000000
                  00000002 00000000               ;Bit 33 high
                  ...

  pattern set 2   FFFFFFFF FFFFFFFF               ;Full bus-width all high
                  00000000 00000001               ;Bit 0 high
                  FFFFFFFF FFFFFFFF               ;Full bus-width all high
                  00000000 00000002               ;Bit 1 high
                  ...
                  00000000 80000000               ;Bit 31 high
                  FFFFFFFF FFFFFFFF
                  00000001 00000000               ;Bit 32 high
                  FFFFFFFF FFFFFFFF
                  00000002 00000000               ;Bit 33 high
                  ...

  pattern set 3   00000000 00000000               ;Full bus-width all low
                  FFFFFFFF FFFFFFFE               ;Bit 0 low
                  00000000 00000000               ;Full bus-width all low
                  FFFFFFFF FFFFFFFD               ;Bit 1 low
                  ...
                  FFFFFFFF 7FFFFFFF               ;Bit 31 low
                  00000000 00000000
                  FFFFFFFE FFFFFFFF               ;Bit 32 low
                  00000000 00000000
                  FFFFFFFD FFFFFFFF               ;Bit 33 low
                  ...

  pattern set 4   FFFFFFFF FFFFFFFF               ;Full bus-width all high
                  FFFFFFFF FFFFFFFE               ;Bit 0 low
                  FFFFFFFF FFFFFFFF               ;Full bus-width all high
                  FFFFFFFF FFFFFFFD               ;Bit 1 low
                  ...
                  FFFFFFFF 7FFFFFFF               ;Bit 31 low
                  FFFFFFFF FFFFFFFF
                  FFFFFFFE FFFFFFFF               ;Bit 32 low
                  FFFFFFFF FFFFFFFF
                  FFFFFFFD FFFFFFFF               ;Bit 33 low
                  ...

  Algorithm :

  Run the following loops with 4 sets of patters
    Pass 1 - 64-bit background pattern to 0000000000000000h
       64-bit foreground pattern to 0000000000000001h

    Pass 2 - 64-bit background pattern to FFFFFFFFFFFFFFFFh
       64-bit foreground pattern to 0000000000000001h

    Pass 3 - 64-bit background pattern to 0000000000000000h
       64-bit foreground pattern to FFFFFFFFFFFFFFFEh

    Pass 4 - 64-bit background pattern to FFFFFFFFFFFFFFFFh
       64-bit foreground pattern to FFFFFFFFFFFFFFFEh

  (1)
    FOR each 128-bit memory location
      write background pattern to first 64 bits
      write foreground pattern to second 64 bits
      rotate foreground pattern left one bit position
    END FOR

    re-initialize 64-bit foreground pattern to starting pattern
  (2)
    FOR each 128-bit memory location
      read first 64 bits and compare to background pattern
      IF compare fails
        EXIT with failure message
      read second 64 bits and compare to foreground pattern
      IF compare fails
        EXIT with failure message
      rotate foreground pattern left one bit position
    END FOR
  */

  switch (testSize) {
  case 8:
    basePatternQW[0].hi = 0x01010101;
    basePatternQW[0].lo = 0x01010101;
    numCl = 2;
    break;
  case 16:
    basePatternQW[0].hi = 0x00010001;
    basePatternQW[0].lo = 0x00010001;
    numCl = 4;
    break;
  case 32:
    basePatternQW[0].hi = 0x00000001;
    basePatternQW[0].lo = 0x00000001;
    numCl = 8;
    break;
  case 64:
    basePatternQW[0].hi = 0x00000000;
    basePatternQW[0].lo = 0x00000001;
    numCl = 16;
    break;
  default:
    numCl = 0;
    basePatternQW[0].hi = 0x00000000;
    basePatternQW[0].lo = 0x00000000;
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_68);
  }

  if (!CheckHwSupportedForMATSAndFixupCacheLine (testSize, &numCl)) {
    return FAILURE;
  }

  //
  // Just run one outer loop
  //
  for (i = 0; i < testSize; i = i + testSize) {

    shiftedQW[0] = LShiftUINT64 (basePatternQW[0], i);

    for (pass = 0; pass < 4; pass++) {

      switch (pass) {
      case 0:
        patternQW[0].hi = 0;
        patternQW[0].lo = 0;
        patternQW[1].hi = shiftedQW[0].hi;
        patternQW[1].lo = shiftedQW[0].lo;
        break;
      case 1:
        patternQW[0].hi = 0xffffffff;
        patternQW[0].lo = 0xffffffff;
        patternQW[1].hi = shiftedQW[0].hi;
        patternQW[1].lo = shiftedQW[0].lo;
        break;
      case 2:
        patternQW[0].hi = 0;
        patternQW[0].lo = 0;
        patternQW[1].hi = ~shiftedQW[0].hi;
        patternQW[1].lo = ~shiftedQW[0].lo;
        break;
      case 3:
        patternQW[0].hi = 0xffffffff;
        patternQW[0].lo = 0xffffffff;
        patternQW[1].hi = ~shiftedQW[0].hi;
        patternQW[1].lo = ~shiftedQW[0].lo;
        break;

        default:
        //Automated add of default case. Please review.
        break;
      }
      SetMem (uiShl, sizeof (uiShl), 0);
      uiShl[1] = 1;
      seqDataInv[0] = 0;
      seqDataInv[1] = 0;

      RcDebugPrint (SDBG_MAX,
                      "patternQW[0] = 0x%08x%08x, patternQW[1] = 0x%08x%08x\n",
                      patternQW[0].hi, patternQW[0].lo, patternQW[1].hi, patternQW[1].lo);

      startTsc = 0;
      endTsc = 0;
      RepairDone = 1;
      RetryCount = 0;
      while (RepairDone && (RetryCount++ < AMT_RETRY_COUNT)) {

        if (status == SUCCESS) {
          //1. Write sliding data pattern to all of memory
          OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP1, (UINT16) (i + pass));
          startTsc = GetPerformanceCounter ();
          status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_WRITE, FAST_Y, BG_INTER_2, seqDataInv, patternQW, FALSE, uiShl, numCl, 2, MT_ADDR_DIR_UP);
          endTsc = GetPerformanceCounter ();
          RcDebugPrint (SDBG_MAX,
                          "Step1 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
        }
        if (status == SUCCESS) {
          status = AdvMemTestPauseCondition (Host, socket, ddrChEnabled, testType);
        }
        if (status == SUCCESS) {
          //2. Read and compare data pattern
          OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP2, (UINT16) (i + pass));
          startTsc = GetPerformanceCounter ();
          status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_READ, FAST_Y, BG_INTER_2, seqDataInv, patternQW, FALSE, uiShl, numCl, 2, MT_ADDR_DIR_UP);
          endTsc = GetPerformanceCounter ();
          RcDebugPrint (SDBG_MAX,
                          "Step2 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
        }
        //
        // Verify that the last test was successful
        //
        if (status == SUCCESS) {
          OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP3, (UINT16) (i + pass));
          //
          // Disposition any failures
          //
          CpgcMemTestDispositionFailRange (Host, socket, ddrChEnabled, testType, BG_INTER_2, &RepairDone);
        } else {
          RcDebugPrint (SDBG_MINMAX,
            "MemTestGNDB%d: Test Failed!\n", testSize);
          goto FailurePath;
        }
      } // while (RepairDone && (RetryCount++ < AMT_RETRY_COUNT))
    } // pass
  } // i

  FailurePath:

  SetAdvMemTestCondition (Host, socket, ddrChEnabled, testType, AMT_RESTORE_TC, &PrevAmtTestCond);

  //
  // Validation hook to Reset row fail list (useful for testing multiple options)
  //
  ResetRowFailureList (Host, socket);

  RcDebugPrint (SDBG_MINMAX,
                  "\nMemTestGNDB%d Ends\n", testSize);

  return status;
} // MemTestGNDB


/**

  Runs Advanced Memory Tests based on the MarchC- algorithm

  @param Host                - Pointer to sysHost
  @param socket              - Socket Id
  @param ddrChEnabled        - List of channels to be tested
  @param testType            - Memory test type
  @param testSize            - Memory test size

  @retval status             - SUCCESS/FAILURE

**/
UINT32
MemTestMARCHCM (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ddrChEnabled,
  UINT8    testType,
  UINT8    testSize
  )
{
  UINT64_STRUCT    patternQW[MT_PATTERN_DEPTH];
  UINT64_STRUCT    inverseQW[MT_PATTERN_DEPTH];
  UINT8  uiShl[MT_PATTERN_DEPTH];
  UINT8  seqDataInv[MT_MAX_SUBSEQ];
  UINT8  numCl;
  UINT8  TestSizeLocal;
  UINT32 i, n;
  UINT8  RepairDone;
  UINT32 RetryCount;
  UINT32 status = SUCCESS;
  UINT64 startTsc;
  UINT64 endTsc;
  AMT_TEST_CONDITION PrevAmtTestCond;

  RcDebugPrint (SDBG_MINMAX,
                  "\nMemTestMARCHM%d Starts\n", testSize);

  IsFaultyPartLogged (Host, socket, &ddrChEnabled);

  SetAdvMemTestCondition (Host, socket, ddrChEnabled, testType, AMT_SET_TC, &PrevAmtTestCond);

  /*
  Run the following loops for each pattern
    Pass 1 - AAAAAAAAAAAAAAAAh
    Pass 2 - CCCCCCCCCCCCCCCCh
    Pass 3 - F0F0F0F0F0F0F0F0h
    Pass 4 - FF00FF00FF00FF00h
    Pass 5 - FFFF0000FFFF0000h
    Pass 6 - FFFFFFFF00000000h
    Pass 7 - 0000000000000000h

  (1)
    FOR each ascending 64-bit memory location
      write pattern
    END FOR
  (2)
    FOR each ascending 64-bit memory location
      read and compare to pattern
      IF compare fails
        EXIT with failure message
      write inverse pattern
    END FOR
  (3)
    FOR each ascending 64-bit memory location
      read and compare to inverse pattern
      IF compare fails
        EXIT with failure message
      write pattern
    END FOR
  (4)
    FOR each descending 64-bit memory location
      read and compare to pattern
      IF compare fails
        EXIT with failure message
      write inverse pattern
    END FOR
  (5)
    FOR each descending 64-bit memory location
      read and compare to inverse pattern
      IF compare fails
        EXIT with failure message
      write pattern
    END FOR
  (6)
    FOR each ascending 64-bit memory location
      read and compare to pattern
      IF compare fails
        EXIT with failure message
    END FOR
  */

  switch (testSize) {
  case 8:
    n = 3;
    break;
  case 16:
    n = 4;
    break;
  case 32:
    n = 5;
    break;
  case 64:
    n = 6;
    break;
  default:
    n = 0;
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_69);
  }

  for (i = 0; i <= n; i++) {

    switch (i) {
    case 0:
      patternQW[0].hi = 0xaaaaaaaa;
      patternQW[0].lo = 0xaaaaaaaa;
      TestSizeLocal = 8;
      break;
    case 1:
      patternQW[0].hi = 0xcccccccc;
      patternQW[0].lo = 0xcccccccc;
      TestSizeLocal = 8;
      break;
    case 2:
      patternQW[0].hi = 0xf0f0f0f0;
      patternQW[0].lo = 0xf0f0f0f0;
      TestSizeLocal = 8;
      break;
    case 3:
      patternQW[0].hi = 0xff00ff00;
      patternQW[0].lo = 0xff00ff00;
      TestSizeLocal = 16;
      break;
    case 4:
      patternQW[0].hi = 0xffff0000;
      patternQW[0].lo = 0xffff0000;
      TestSizeLocal = 32;
      break;
    case 5:
      patternQW[0].hi = 0xffffffff;
      patternQW[0].lo = 0x00000000;
      TestSizeLocal = 64;
      break;
    case 6:
      patternQW[0].hi = 0x0;
      patternQW[0].lo = 0x0;
      TestSizeLocal = 8;
      break;
    default:
      //Automated add of default case. Please review.
      break;
    }

    inverseQW[0].hi = ~patternQW[0].hi;
    inverseQW[0].lo = ~patternQW[0].lo;

    numCl = 1;
    SetMem (uiShl, sizeof (uiShl), 0);
    seqDataInv[0] = 0;
    seqDataInv[1] = 1;

    if (!CheckHwSupportedForMATSAndFixupCacheLine (TestSizeLocal, &numCl)) {
      return FAILURE;
    }

    RcDebugPrint (SDBG_MAX,
                    "patternQW[0] = 0x%08x%08x, inverseQW[0] = 0x%08x%08x\n",
                    patternQW[0].hi, patternQW[0].lo, inverseQW[0].hi, inverseQW[0].lo);

    startTsc = 0;
    endTsc = 0;
    RepairDone = 1;
    RetryCount = 0;
    while (RepairDone && (RetryCount++ < AMT_RETRY_COUNT)) {

      if (status == SUCCESS) {
        //1. Write data pattern to all of memory
        OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP1, (UINT16) i);
        startTsc = GetPerformanceCounter ();
        status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_WRITE, FAST_Y, BG_INTER_2, seqDataInv, patternQW, FALSE, uiShl, numCl, 1, MT_ADDR_DIR_UP);
        endTsc = GetPerformanceCounter ();
        RcDebugPrint (SDBG_MAX,
                        "Step1 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
      }
      if (status == SUCCESS) {
        status = AdvMemTestPauseCondition (Host, socket, ddrChEnabled, testType);
      }
      if (status == SUCCESS) {
        //2. read and compare data pattern
        //   write inverse pattern
        OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP2, (UINT16) i);
        startTsc = GetPerformanceCounter ();
        status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_READ_WRITE, FAST_Y, BG_INTER_2, seqDataInv, patternQW, FALSE, uiShl, numCl, 1, MT_ADDR_DIR_UP);
        endTsc = GetPerformanceCounter ();
        RcDebugPrint (SDBG_MAX,
                        "Step2 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
      }
      if (status == SUCCESS) {
        status = AdvMemTestPauseCondition (Host, socket, ddrChEnabled, testType);
      }
      if (status == SUCCESS) {
        //3. read and compare inverse pattern
        //   write original pattern
        OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP3, (UINT16) i);
        startTsc = GetPerformanceCounter ();
        status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_READ_WRITE, FAST_Y, BG_INTER_2, seqDataInv, inverseQW, TRUE, uiShl, numCl, 1, MT_ADDR_DIR_UP);
        endTsc = GetPerformanceCounter ();
        RcDebugPrint (SDBG_MAX,
                        "Step3 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
      }
      if (status == SUCCESS) {
        status = AdvMemTestPauseCondition (Host, socket, ddrChEnabled, testType);
      }
      if (status == SUCCESS) {
        //4. read and compare original pattern
        //   write inverse pattern
        OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP4, (UINT16) i);
        startTsc = GetPerformanceCounter ();
        status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_READ_WRITE, FAST_Y, BG_INTER_2, seqDataInv, patternQW, FALSE, uiShl, numCl, 1, MT_ADDR_DIR_DN);
        endTsc = GetPerformanceCounter ();
        RcDebugPrint (SDBG_MAX,
                        "Step4 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
      }
      if (status == SUCCESS) {
        status = AdvMemTestPauseCondition (Host, socket, ddrChEnabled, testType);
      }
      if (status == SUCCESS) {
        //5. read and compare inverse pattern
        //   write original pattern
        OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP5, (UINT16) i);
        startTsc = GetPerformanceCounter ();
        status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_READ_WRITE, FAST_Y, BG_INTER_2, seqDataInv, inverseQW, TRUE, uiShl, numCl, 1, MT_ADDR_DIR_DN);
        endTsc = GetPerformanceCounter ();
        RcDebugPrint (SDBG_MAX,
                        "Step5 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
      }
      if (status == SUCCESS) {
        status = AdvMemTestPauseCondition (Host, socket, ddrChEnabled, testType);
      }
      if (status == SUCCESS) {
        //6. read and compare original pattern
        OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP6, (UINT16) i);
        startTsc = GetPerformanceCounter ();
        status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_READ, FAST_Y, BG_INTER_2, seqDataInv, patternQW, FALSE, uiShl, numCl, 1, MT_ADDR_DIR_UP);
        endTsc = GetPerformanceCounter ();
        RcDebugPrint (SDBG_MAX,
                        "Step6 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
      }
      //
      // Verify that the last test was successful
      //
      if (status == SUCCESS) {
        OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP7, (UINT16) i);
        //
        // Disposition any failures
        //
        CpgcMemTestDispositionFailRange (Host, socket, ddrChEnabled, testType, BG_INTER_2, &RepairDone);
      } else {
        RcDebugPrint (SDBG_MINMAX,
          "MemTestMARCHM%d: Test Failed!\n", testSize);
        goto FailurePath;
      }

    } // while (RepairDone && (RetryCount++ < AMT_RETRY_COUNT))
  } // i

  FailurePath:

  SetAdvMemTestCondition (Host, socket, ddrChEnabled, testType, AMT_RESTORE_TC, &PrevAmtTestCond);

  //
  // Validation hook to Reset row fail list (useful for testing multiple options)
  //
  ResetRowFailureList (Host, socket);

  RcDebugPrint (SDBG_MINMAX,
                  "\nMemTestMARCHM%d Ends\n", testSize);

  return status;
} // MemTestMARCHM



UINT64 AmtCacheLine1[MT_PATTERN_DEPTH] = {
  0x5555555555555555, 0x5555555555555555,
  0xAAAAAAAAAAAAAAAA, 0xAAAAAAAAAAAAAAAA,
  0x5555555555555555, 0x5555555555555555,
  0xAAAAAAAAAAAAAAAA, 0xAAAAAAAAAAAAAAAA
};

UINT64 AmtCacheLine2[MT_PATTERN_DEPTH] = {
  0x0000000000000000, 0x0000000000000000,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
  0x0000000000000000, 0x0000000000000000,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF
};

UINT64 AmtCacheLine3[MT_PATTERN_DEPTH] = {
  0x7878787878787878, 0xB4B4B4B4B4B4B4B4,
  0xD2D2D2D2D2D2D2D2, 0xE1E1E1E1E1E1E1E1,
  0x7878787878787878, 0xB4B4B4B4B4B4B4B4,
  0xD2D2D2D2D2D2D2D2, 0xE1E1E1E1E1E1E1E1
};


/**

  Runs Advanced Memory Test targetting tWR

  @param Host                - Pointer to sysHost
  @param socket              - Socket Id
  @param ddrChEnabled        - List of channels to be tested
  @param testType            - Memory test type
  @param testSize            - Memory test size

  @retval status             - SUCCESS/FAILURE

**/
UINT32
MemTestTWR (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ddrChEnabled,
  UINT8    testType,
  UINT8    TestSize
)
{
  UINT64_STRUCT    patternQW[MT_PATTERN_DEPTH];
  UINT64_STRUCT    inverseQW[MT_PATTERN_DEPTH];
  UINT64_STRUCT    shiftedQW[MT_PATTERN_DEPTH];
  UINT64_STRUCT    basePatternQW[MT_PATTERN_DEPTH];
  UINT8  uiShl[MT_PATTERN_DEPTH];
  UINT8  seqDataInv[MT_MAX_SUBSEQ];
  UINT8  numCl;
  UINT8  shiftRate = 1;
  UINT32 TestLoop;
  UINT32 TestBit;
  UINT32 status = SUCCESS;
  UINT64 startTsc = 0;
  UINT64 endTsc = 0;
  UINT8  RepairDone;
  UINT32 RetryCount;
  AMT_TEST_CONDITION PrevAmtTestCond;

  RcDebugPrint (SDBG_MINMAX,
    "\nMemTestTWR Starts\n");

  IsFaultyPartLogged (Host, socket, &ddrChEnabled);

  SetAdvMemTestCondition (Host, socket, ddrChEnabled, testType, AMT_SET_TC, &PrevAmtTestCond);

  /*
  // Patterns
  8-bit Patterns

  0101010101010101h
  0202020202020202h
  0404040404040404h
  0808080808080808h
  1010101010101010h
  2020202020202020h
  4040404040404040h
  8080808080808080h
  0101010101010101h
  ...
  */

  switch (TestSize) {
  case 1:
  case 8:
    basePatternQW[0].hi = 0x01010101;
    basePatternQW[0].lo = 0x01010101;
    numCl = 1;
    break;
  default:
    numCl = 0;
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_66);
  }

  if (!CheckHwSupportedForMATSAndFixupCacheLine (TestSize, &numCl)) {
    return FAILURE;
  }

  // SW loop for TestSize larger than numCl
  for (TestLoop = 0; TestLoop < TestSize; TestLoop = TestLoop + (numCl * 8)) {
    // Test each bit position per UI
    for (TestBit = 0; (TestBit < (UINT32)(numCl * 8)) && (TestBit + TestLoop < TestSize); TestBit++) {

      //
      // Shift left basePatternQW by TestLoop positions
      //
      shiftedQW[0] = LShiftUINT64 (basePatternQW[0], TestBit);
      //
      // Define the test pattern
      //
      patternQW[0].hi = shiftedQW[0].hi;
      patternQW[0].lo = shiftedQW[0].lo;
      //
      // Set the inverse test pattern
      //
      inverseQW[0].hi = ~shiftedQW[0].hi;
      inverseQW[0].lo = ~shiftedQW[0].lo;
      SetMem (uiShl, sizeof (uiShl), 0);
      uiShl[0] = shiftRate;
      uiShl[1] = shiftRate;
      seqDataInv[0] = 0;
      seqDataInv[1] = 1;

      RcDebugPrint (SDBG_MAX,
        "patternQW[0] = 0x%08x%08x, inverseQW[0] = 0x%08x%08x\n",
        patternQW[0].hi, patternQW[0].lo, inverseQW[0].hi, inverseQW[0].lo);

      startTsc = 0;
      endTsc = 0;
      RepairDone = 1;
      RetryCount = 0;
      while (RepairDone && (RetryCount++ < AMT_RETRY_COUNT)) {

        if (status == SUCCESS) {
          //1. Write data pattern to all of memory
          OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP1, (UINT16)(TestLoop + TestBit));
          startTsc = GetPerformanceCounter ();
          status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_WRITE, FAST_X, BG_INTER_2, seqDataInv, patternQW, FALSE, uiShl, numCl, 1, MT_ADDR_DIR_UP);
          endTsc = GetPerformanceCounter ();
          RcDebugPrint (SDBG_MAX,
            "Step1 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
        }
        if (status == SUCCESS) {
          status = AdvMemTestPauseCondition (Host, socket, ddrChEnabled, testType);
        }
        if (status == SUCCESS) {
          //2. read and compare data pattern
          OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP2, (UINT16)(TestLoop + TestBit));
          startTsc = GetPerformanceCounter ();
          status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_READ, FAST_Y, BG_INTER_2, seqDataInv, patternQW, FALSE, uiShl, numCl, 1, MT_ADDR_DIR_UP);
          endTsc = GetPerformanceCounter ();
          RcDebugPrint (SDBG_MAX,
            "Step2 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
        }
        if (status == SUCCESS) {
          //3. Write inverse pattern to all of memory
          OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP3, (UINT16)(TestLoop + TestBit));
          startTsc = GetPerformanceCounter ();
          status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_WRITE, FAST_X, BG_INTER_2, seqDataInv, inverseQW, TRUE, uiShl, numCl, 1, MT_ADDR_DIR_DN);
          endTsc = GetPerformanceCounter ();
          RcDebugPrint (SDBG_MAX,
            "Step3 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
        }
        if (status == SUCCESS) {
          status = AdvMemTestPauseCondition (Host, socket, ddrChEnabled, testType);
        }
        if (status == SUCCESS) {
          //4. read and compare inverse pattern
          OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP4, (UINT16)(TestLoop + TestBit));
          startTsc = GetPerformanceCounter ();
          status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_READ, FAST_Y, BG_INTER_2, seqDataInv, inverseQW, TRUE, uiShl, numCl, 1, MT_ADDR_DIR_DN);
          endTsc = GetPerformanceCounter ();
          RcDebugPrint (SDBG_MAX,
            "Step4 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
        }
        //
        // Verify that the last test was successful
        //
        if (status == SUCCESS) {
          OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP5, (UINT16)(TestLoop + TestBit));
          //
          // Disposition any failures
          //
          CpgcMemTestDispositionFailRange (Host, socket, ddrChEnabled, testType, BG_INTER_2, &RepairDone);
        }
        else {
          RcDebugPrint (SDBG_MINMAX,
            "MemTestTWR: Test Failed!\n");
          goto FailurePath;
        }
      } // while (RepairDone && (RetryCount++ < AMT_RETRY_COUNT))
    } // TestBit
  } // TestLoop

  FailurePath:

  SetAdvMemTestCondition (Host, socket, ddrChEnabled, testType, AMT_RESTORE_TC, &PrevAmtTestCond);

  //
  // Validation hook to Reset row fail list (useful for testing multiple options)
  //
  ResetRowFailureList (Host, socket);

  RcDebugPrint (SDBG_MINMAX,
    "\nMemTestTWR Ends\n");

  return status;
} // MemTestTWR


/**

  Runs Advanced Memory Test targetting Data retention between refresh

  @param Host                - Pointer to sysHost
  @param socket              - Socket Id
  @param ddrChEnabled        - List of channels to be tested
  @param testType            - Memory test type
  @param testSize            - Memory test size

  @retval status             - SUCCESS/FAILURE

**/
UINT32
MemTestDataRetention (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ddrChEnabled,
  UINT8    testType,
  UINT8    testSize
)
{
  UINT64_STRUCT    patternQW[MT_PATTERN_DEPTH];
  UINT8  uiShl[MT_PATTERN_DEPTH];
  UINT8  seqDataInv[MT_MAX_SUBSEQ];
  UINT8  numCl;
  UINT8  TestSizeLocal;
  UINT32 i;
  UINT8  RepairDone;
  UINT32 RetryCount;
  UINT32 status = SUCCESS;
  UINT64 startTsc;
  UINT64 endTsc;
  UINT32 index;
  AMT_TEST_CONDITION PrevAmtTestCond;

  RcDebugPrint (SDBG_MINMAX,
    "\nMemTestDataRetention Starts\n");

  IsFaultyPartLogged (Host, socket, &ddrChEnabled);

  SetAdvMemTestCondition (Host, socket, ddrChEnabled, testType, AMT_SET_TC, &PrevAmtTestCond);

  for (i = 0; i < 2; i++) {

    switch (i) {
    case 0:
      //
      // Copy cacheline1 into patternQW
      //
      CopyMem (patternQW, AmtCacheLine1, sizeof (patternQW));
      break;
    case 1:
      //
      // Copy cacheline2 into patternQW
      //
      CopyMem (patternQW, AmtCacheLine2, sizeof (patternQW));
      break;
    default:
      break;
    }

    SetMem (uiShl, sizeof (uiShl), 0);
    numCl = 2;
    seqDataInv[0] = 0;
    seqDataInv[1] = 0;

    for (index = 0; index < 8; index++) {

      patternQW[index + 8].hi = ~patternQW[index].hi;
      patternQW[index + 8].lo = ~patternQW[index].lo;

      RcDebugPrint (SDBG_MAX,
        "patternQW[%d] = 0x%08x%08x\n", index, patternQW[index].hi, patternQW[index].lo);
    }

    //
    // Based on the pattern info, the test size for hardware support check is 8
    //
    TestSizeLocal = 8;

    if (!CheckHwSupportedForMATSAndFixupCacheLine (TestSizeLocal, &numCl)) {
      return FAILURE;
    }

    startTsc = 0;
    endTsc = 0;
    RepairDone = 1;
    RetryCount = 0;
    while (RepairDone && (RetryCount++ < AMT_RETRY_COUNT)) {

      if (status == SUCCESS) {
        //1. Write data pattern to all of memory
        OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP1, (UINT16)i);
        startTsc = GetPerformanceCounter ();
        status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_WRITE, FAST_Y, BG_INTER_2, seqDataInv, patternQW, FALSE, uiShl, numCl, 16, MT_ADDR_DIR_UP);
        endTsc = GetPerformanceCounter ();
        RcDebugPrint (SDBG_MAX,
          "Step1 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
      }
      if (status == SUCCESS) {
        status = AdvMemTestPauseCondition (Host, socket, ddrChEnabled, testType);
      }
      if (status == SUCCESS) {
        //2. read and compare data pattern
        OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP2, (UINT16)i);
        startTsc = GetPerformanceCounter ();
        status = CpgcMemTestMATS (Host, socket, ddrChEnabled, testType, MT_CPGC_READ, FAST_Y, BG_INTER_2, seqDataInv, patternQW, FALSE, uiShl, numCl, 16, MT_ADDR_DIR_UP);
        endTsc = GetPerformanceCounter ();
        RcDebugPrint (SDBG_MAX,
          "Step2 latency = %d ms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
      }

      //
      // Verify that the last test was successful
      //
      if (status == SUCCESS) {
        OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP3, (UINT16)i);
        //
        // Disposition any failures
        //
        CpgcMemTestDispositionFailRange (Host, socket, ddrChEnabled, testType, BG_INTER_2, &RepairDone);
      } else {
        RcDebugPrint (SDBG_MINMAX,
          "MemTestDataRetention: Test Failed!\n");
        goto FailurePath;
      }
    } // while (RepairDone && (RetryCount++ < AMT_RETRY_COUNT))
  } //i

  FailurePath:

  SetAdvMemTestCondition (Host, socket, ddrChEnabled, testType, AMT_RESTORE_TC, &PrevAmtTestCond);

  //
  // Validation hook to Reset row fail list (useful for testing multiple options)
  //
  ResetRowFailureList (Host, socket);

  RcDebugPrint (SDBG_MINMAX,
    "\nMemTestDataRetention Ends\n");

  return status;
} // MemTestDataRetention


/**

  Prints the elapsed time for given Test Type

  @param Host                - Pointer to sysHost
  @param StartTsc            - Initial timestamp counter measurement
  @param TestType            - AdvMemTest Type

  @retval - None

**/
VOID
PrintAdvMemtestLatency (
  PSYSHOST Host,
  UINT64   StartTsc,
  UINT8    TestType
  )
{
  UINT64 EndTsc;

  EndTsc = GetPerformanceCounter ();
  RcDebugPrint (SDBG_MIN,
    "TestType %d Latency = %d sec\n",
    TestType, TimeDiff (StartTsc, EndTsc, TDIFF_UNIT_S));
}

/**

  Runs Advanced Memory Test algorithms with CPGC and logs any failures

  @param Host                - Pointer to sysHost
  @param AdvMemTestOptions   - Bit field of Advanced Memtest algorithms to run

  @retval - SUCCESS

**/
UINT32
AdvancedMemTestWorker (
  PSYSHOST Host,
  UINT32   AdvMemTestOptions
  )
{
  UINT8               socket;
  UINT8               ch;
  UINT8               ddrChEnabled;
  UINT32              maxRepeatTest;
  UINT32              repeatTest;
  UINT32              scrambleConfigOrg[MAX_CH];
  UINT32              status;
  SYS_SETUP *Setup;
  UINT64 StartTsc = 0;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  ZeroMem ((UINT8 *) scrambleConfigOrg, sizeof (scrambleConfigOrg));

  if (GetSysBootMode () == NormalBoot) {
    if (((Host->var.mem.subBootMode == ColdBootFast) &&
         (Setup->mem.options & MEMORY_TEST_COLD_FAST_BOOT_EN)) ||
        (!(Host->var.mem.subBootMode == WarmBootFast ||
           Host->var.mem.subBootMode == ColdBootFast) &&
         (Setup->mem.options & MEMORY_TEST_EN))) {

      socket = Host->var.mem.currentSocket;

      InitAmtDataStructures (Host, socket);

      ClearWdbCacheValidAllChannels (socket);

      //
      // Return if this socket is disabled
      //
      if (IsSocketDimmPopulated (Host, socket) != TRUE) {
        return SUCCESS;
      }

      //
      // if we are running memory test, we need to not skip the memory init
      //
      Host->var.mem.skipMemoryInit = 0;

      SetUbiosOutputMode (ASM_OUTPUT_DISABLE);

      // Create the Channel Mask
      ddrChEnabled = 0;

      if (Setup->mem.memTestLoops) {
        maxRepeatTest = (UINT32) Setup->mem.memTestLoops;
      } else {
        maxRepeatTest = 0xFFFFFFFF;   //used for infinite loop
      }
      for (repeatTest = 0; repeatTest < maxRepeatTest; repeatTest++) {
        // Chip hook to disable scrambling and other stuff that interferes with Adv memtest
        for (ch = 0; ch < MaxChDdr; ch++) {
          if (IsChannelEnabled (socket, ch) == FALSE) {
            continue;
          }
          ddrChEnabled |= 1 << ch;

          scrambleConfigOrg[ch] = PreAdvancedMemTest (Host, socket, ch);
        }

        //
        // MATS+ algorithm under different test conditions
        //
        if (AdvMemTestOptions & ADV_MT_MATS8_TC1) {
          StartTsc = GetPerformanceCounter ();
          MemTestMATSN (Host, socket, ddrChEnabled, ADV_MT_TYPE_MATS8_TC1, 8, 1, FALSE);
          PrintAdvMemtestLatency (Host, StartTsc, ADV_MT_TYPE_MATS8_TC1);
        }
        if (AdvMemTestOptions & ADV_MT_MATS8_TC2) {
          StartTsc = GetPerformanceCounter ();
          MemTestMATSN (Host, socket, ddrChEnabled, ADV_MT_TYPE_MATS8_TC2, 8, 1, FALSE);
          PrintAdvMemtestLatency (Host, StartTsc, ADV_MT_TYPE_MATS8_TC2);
        }
        if (AdvMemTestOptions & ADV_MT_MATS8_TC3) {
          StartTsc = GetPerformanceCounter ();
          MemTestMATSN (Host, socket, ddrChEnabled, ADV_MT_TYPE_MATS8_TC3, 8, 1, FALSE);
          PrintAdvMemtestLatency (Host, StartTsc, ADV_MT_TYPE_MATS8_TC3);
        }

        // XMATS
        if (AdvMemTestOptions & ADV_MT_XMATS8) {
          StartTsc = GetPerformanceCounter ();
          MemTestMATSN (Host, socket, ddrChEnabled, ADV_MT_TYPE_XMATS8, 8, 1, TRUE);
          PrintAdvMemtestLatency (Host, StartTsc, ADV_MT_TYPE_XMATS8);
        }
        if (AdvMemTestOptions & ADV_MT_XMATS16) {
          StartTsc = GetPerformanceCounter ();
          MemTestMATSN (Host, socket, ddrChEnabled, ADV_MT_TYPE_XMATS16, 16, 1, TRUE);
          PrintAdvMemtestLatency (Host, StartTsc, ADV_MT_TYPE_XMATS16);
        }
        if (AdvMemTestOptions & ADV_MT_XMATS32) {
          StartTsc = GetPerformanceCounter ();
          MemTestMATSN (Host, socket, ddrChEnabled, ADV_MT_TYPE_XMATS32, 32, 1, TRUE);
          PrintAdvMemtestLatency (Host, StartTsc, ADV_MT_TYPE_XMATS32);
        }
        if (AdvMemTestOptions & ADV_MT_XMATS64) {
          StartTsc = GetPerformanceCounter ();
          MemTestMATSN (Host, socket, ddrChEnabled, ADV_MT_TYPE_XMATS64, 64, 1, TRUE);
          PrintAdvMemtestLatency (Host, StartTsc, ADV_MT_TYPE_XMATS64);
        }

        // Worst-case MATS (no sliding pattern)
        if (AdvMemTestOptions & ADV_MT_WCMATS8) {
          StartTsc = GetPerformanceCounter ();
          MemTestMATSN (Host, socket, ddrChEnabled, ADV_MT_TYPE_WCMATS8, 8, 0, TRUE);
          PrintAdvMemtestLatency (Host, StartTsc, ADV_MT_TYPE_WCMATS8);
        }

        // Worst-case March (repeat 2 times)
        if (AdvMemTestOptions & ADV_MT_WCMCH8) {
          StartTsc = GetPerformanceCounter ();
          MemTestMCH (Host, socket, ddrChEnabled, ADV_MT_TYPE_WCMCH8, 8, 2);
          PrintAdvMemtestLatency (Host, StartTsc, ADV_MT_TYPE_WCMCH8);
        }

        // Ground Bounce
        if (AdvMemTestOptions & ADV_MT_GNDB64) {
          StartTsc = GetPerformanceCounter ();
          MemTestGNDB (Host, socket, ddrChEnabled, ADV_MT_TYPE_GNDB64, 64);
          PrintAdvMemtestLatency (Host, StartTsc, ADV_MT_TYPE_GNDB64);
        }

        // March C-
        if (AdvMemTestOptions & ADV_MT_MARCHCM64) {
          StartTsc = GetPerformanceCounter ();
          MemTestMARCHCM (Host, socket, ddrChEnabled, ADV_MT_TYPE_MARCHCM64, 64);
          PrintAdvMemtestLatency (Host, StartTsc, ADV_MT_TYPE_MARCHCM64);
        }

        // Data Retention
        if (AdvMemTestOptions & ADV_MT_DATA_RET) {
          StartTsc = GetPerformanceCounter ();
          MemTestDataRetention (Host, socket, ddrChEnabled, ADV_MT_TYPE_DATA_RET, 64);
          PrintAdvMemtestLatency (Host, StartTsc, ADV_MT_TYPE_DATA_RET);
        }

        // Write Recovery time
        if (AdvMemTestOptions & ADV_MT_TWR) {
          StartTsc = GetPerformanceCounter ();
          MemTestTWR (Host, socket, ddrChEnabled, ADV_MT_TYPE_TWR, 8);
          PrintAdvMemtestLatency (Host, StartTsc, ADV_MT_TYPE_TWR);
        }

        //
        // Chip hook to enable scrambling
        //
        for (ch = 0; ch < MaxChDdr; ch++) {
          if (IsChannelEnabled (socket, ch) == FALSE) {
            continue;
          }
          PostAdvancedMemTestRestoreScrambler (Host, socket, ch, scrambleConfigOrg[ch]);
        }
        //
        // Legacy Memtest with support for row fail ranges
        // This option uses the row range list to track failures and make more precise decision on correctable vs. uncorrectable errors
        // The row range list can also be used to invoke PPR flow
        //
        if (AdvMemTestOptions & ADV_MT_RANGE_TEST_SCRAM) {

          StartTsc = GetPerformanceCounter ();
          status = MemTestScram (Host, socket, ddrChEnabled, ADV_MT_TYPE_RANGE_TEST_SCRAM, 64, 0, repeatTest);
          PrintAdvMemtestLatency (Host, StartTsc, ADV_MT_TYPE_RANGE_TEST_SCRAM);

          //
          // Use legacy path if row failure tracking list filled up (the only reason status != SUCCESS)
          //
          if (status != SUCCESS) {
            //
            // Reset row failure tracking info for subsequent steps
            //
            Host->var.mem.FailRangeInitDone[socket] = 0;
            InitAmtDataStructures (Host, socket);

            StartTsc = GetPerformanceCounter ();
            MemTestScram (Host, socket, ddrChEnabled, ADV_MT_TYPE_LTEST_SCRAM, 64, 0, repeatTest);
            PrintAdvMemtestLatency (Host, StartTsc, ADV_MT_TYPE_LTEST_SCRAM);
          }
        }

        // Chip hook to re-enable scrambling and other stuff that was modified for Adv memtest
        for (ch = 0; ch < MaxChDdr; ch++) {
          if (IsChannelEnabled (socket, ch) == FALSE) {
            continue;
          }
          PostAdvancedMemTest (Host, socket, ch, scrambleConfigOrg[ch]);
        }
        //
        //incorporates infinite loop accordingly
        //
        if (!Setup->mem.memTestLoops && (repeatTest == 0xFFFFFFFE)) {
          repeatTest = 0;
        }
      }

      SetUbiosOutputMode (ASM_OUTPUT_ENABLE);

    } // if MemTest enabled
  } // if NormalBoot

  return SUCCESS;
} // AdvancedMemTestWorker


/**

  Runs Advanced Memory Test algorithms with CPGC and logs any failures

  @param Host                - Pointer to sysHost

  @retval - SUCCESS

**/
UINT32
AdvancedMemTest (
  PSYSHOST Host
  )
{
  SYS_SETUP                   *Setup;
  UINT32                      AdvMemTestOptions;
  UINT8                       Socket;
  MEM_TEST_CONFIG             *MemTestConfig;

  if (!IsMemFlowEnabled (AdvancedMemoryTest)) {
    return SUCCESS;
  }

  Socket = Host->var.mem.currentSocket;
  Setup = GetSysSetupPointer();
  AdvMemTestOptions = Setup->mem.AdvMemTestOptions & ~(ADV_MT_RANGE_TEST_SCRAM | ADV_MT_LTEST_SCRAM | ADV_MT_LINIT_SCRAM);

  RcDebugPrint (SDBG_MAX, "AdvMemTestOptions: 0x%08x\n", AdvMemTestOptions);

  PreMemTest (Socket, &MemTestConfig);

  if (AdvMemTestOptions) {
    AdvancedMemTestWorker (Host, AdvMemTestOptions);
  }

  DdrMemTest (Host);

  PostMemTest (Socket, &MemTestConfig);

  return SUCCESS;

} // AdvancedMemTest

/**

  Runs optimized MemTest with tCCD_S

  @param Host  - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
DdrMemTest (
  PSYSHOST Host
  )
{
  return AdvancedMemTestWorker (Host, ADV_MT_RANGE_TEST_SCRAM);
} // DdrMemTest


/**
  Runs MemInit

  @param Host  - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
DdrMemInit (
  PSYSHOST Host
  )
{
  UINT8               socket;
  UINT8               ch;
  UINT32              ddrChEnabled;
  UINT8               MaxChDdr;

  if (!IsMemFlowEnabled (MemInitialization)) {
    return SUCCESS;
  }

  //
  // Skip MemInit if the skip meminit flag is set and memory does not need to be wiped
  //
  if ((Host->var.mem.skipMemoryInit) && (Host->var.mem.wipeMemory == 0)) {
    return SUCCESS;
  }

  //
  // Execute MemInit only when ECC is enabled or memory needs to be wiped
  //
  if ((Host->nvram.mem.eccEn == 0) && (Host->var.mem.wipeMemory == 0)) {
    return SUCCESS;
  }

  //
  // Skip if not a normal boot
  //
  if ((GetSysBootMode () != NormalBoot)) {
    return SUCCESS;
  }

  MaxChDdr = GetMaxChDdr ();
  //
  // Return if this socket is disabled
  //
  socket = Host->var.mem.currentSocket;
  if (IsSocketDimmPopulated (Host, socket) != TRUE) {
    return SUCCESS;
  }

  SetUbiosOutputMode (ASM_OUTPUT_DISABLE);

  // Create the Channel Mask
  ddrChEnabled = 0;

  for (ch = 0; ch < MaxChDdr; ch++) {
    if (IsChannelEnabled (socket, ch) == FALSE) {
      continue;
    }
    ddrChEnabled |= 1 << ch;
  }

  ClearWdbCacheValidAllChannels (socket);

  //
  // Call any pre-Meminit SOC hooks
  //
  PreMemInit (socket);

  //
  // Step 1: Write all 0's pattern
  //
  MemInitOpt (Host, socket, ddrChEnabled);

  SetUbiosOutputMode (ASM_OUTPUT_ENABLE);

  return SUCCESS;
} // DdrMemInit
