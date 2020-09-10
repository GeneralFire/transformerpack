/** @file
  Interface source file for the Swizzle library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#include <Library/BaseLib.h>
#include "SysHost.h"
#include <Library/MemoryServicesLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/SwizzleLib.h>

//
// North byte (4-7) Physical Bit swizzling from DDRDATA group to package bumps
//
UINT8 mPhysicalBitSwizzleN[MAX_BITS_IN_BYTE] = {
  4, 5, 2, 3, 6, 7, 0, 1
};
//
// South byte (0-3, 8) Physical Bit swizzling from DDRDATA group to package bumps
// Note: assumes nibble swap in mDdrioStrobeSwizzle table
//
UINT8 mPhysicalBitSwizzleS[MAX_BITS_IN_BYTE] = {
  4, 5, 2, 3, 6, 7, 0, 1
};
//
// North byte (4-7) Logical Bit swizzling from DDRDATA group to package bumps
//
UINT8 mLogicalBitSwizzleN[MAX_BITS_IN_BYTE] = {
  0, 1, 2, 3, 6, 7, 4, 5
};
//
// South byte (0-3, 8) Logical Bit swizzling from DDRDATA group to package bumps
// Note: assumes nibble swap in mDdrioStrobeSwizzle table
//
UINT8 mLogicalBitSwizzleS[MAX_BITS_IN_BYTE] = {
  0, 1, 2, 3, 6, 7, 4, 5
};
//
// South byte (0-3, 8) Logical Attacker Bit swizzling from DDRDATA group to package bumps
// Note: assumes nibble swap in mDdrioStrobeSwizzle table
//
UINT8 mLogicalAttackerBitSwizzleS[MAX_BITS_IN_BYTE] = {
  4, 5, 6, 7, 2, 3, 0, 1
};
//
// Strobe swizzling from MC to DDRDATA registers
// ICX swizzled table
// Byte/nibble to station id mapping:
// byte:      b0    b1    b2    b3    b4    b5    b6    b7    b8
// Nibble 0   0xc   0xe   0x10  0x12  0x0   0x2   0x4   0x6   0x8
// Nibble 1   0xd   0xf   0x11  0x13  0x1   0x3   0x5   0x7   0x9
//
// die to ball mapping (byte/nibble)
// die byte        b0      b1      b2      b3      b4      b5      b6      b7      b8
// die nibble 0    b7/n0   b6/n1   b5/n0   b4/n1   b3/n1   b2/n0   b1/n1   b0/n0   b8/n0
// die nibble 1    b7/n1   b6/n0   b5/n1   b4/n0   b3/n0   b2/n1   b1/n0   b0/n1   b8/n1
//
UINT8 mDdrioStrobeSwizzle[MAX_STROBE] =
{
  9, 11, 13, 15, 6, 4, 2, 0, 17,
  8, 10, 12, 14, 7, 5, 3, 1, 16
};

/*
  Return the value for DQ bit after applying the internal swizzling.

  BUMP --> NIBBLE BUFF --> NIBBLE FLOW --> DDRDATA --> MC2GD
  |---------- Physical ---------|------- Logical --------|

  @param[in]  Channel         Current channel under test.
  @param[in]  TargetStrobe    Current strobe under test. Valid ranges are:
                              On x4 devices.
                                TargetStrobe goes from 0->17
                              On x8/x16 devices.
                                TargetStrobe goes from 0->17
                                TargetStrobe goes from 0->8
  @param[in]  LogicalBit      Current bit under test. Valid ranges are:
                              On x4 devices.
                                LogicalBit goes from 0->3
                              On x8/x16 devices.
                                LogicalBit goes from 0->7 if TargetStrobe goes from 0->8
                                LogicalBit goes from 0->3 if TargetStrobe goes from 0->17
  @param[in]  Level           MRC_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]  Group           MRC_GT - Parameter to program

  @return     Return the value for DQ bit after applying the internal swizzling

*/
UINT8
GetDqSwizzle (
  IN        UINT8           Channel,
  IN        UINT8           TargetStrobe,
  IN        UINT8           LogicalBit,
  IN        MRC_LT          Level,
  IN        MRC_GT          Group
  )
{
  PSYSHOST  Host;
  UINT8     MSVx8;
  UINT8     *DqSwizzleMap;

  Host  = (PSYSHOST) GetSysHostPointer ();
  MSVx8 = GetMaxStrobeValid (Host) / 2;
  DqSwizzleMap = mLogicalBitSwizzleN;
  //
  // On x4 devices, LogicalBit is only allowed to be between 0->3
  //
  if (LogicalBit >= BITS_PER_NIBBLE && TargetStrobe >= MSVx8) {
    RC_FATAL_ERROR (LogicalBit < BITS_PER_NIBBLE, ERR_ARRAY_OUT_OF_BOUNDS, RC_FATAL_ERROR_MINOR_CODE_0);
    return 0;
  }

  if (TargetStrobe >= MSVx8 && LogicalBit < BITS_PER_NIBBLE) {
    LogicalBit += BITS_PER_NIBBLE;
  }

  if (LogicalBit >= MAX_BITS_IN_BYTE) {
    RC_FATAL_ERROR (LogicalBit < MAX_BITS_IN_BYTE, ERR_ARRAY_OUT_OF_BOUNDS, RC_FATAL_ERROR_MINOR_CODE_0);
    return 0;
  }


  if (Group == PhysicalDelay || Group == RxVref || Group == RxSenseAmpOffset) {
    //
    // Physical groups
    //
    if (GetStrobeSwizzle (Host, Channel, TargetStrobe) < 8) {
      //
      // North strobes
      //
      DqSwizzleMap = mPhysicalBitSwizzleN;
    } else {
      //
      // South strobes
      //
      DqSwizzleMap = mPhysicalBitSwizzleS;
    }
  } else if (Group == LogicalDelay || Group == PxcControl ||
             Group == PxcDelta || Group == DdjcDelta ||
             Group == TxDqBitDelay ||
             Group == RxDqsBitDelay || Group == RxDqsPBitDelay || Group == RxDqsNBitDelay
             ) {
    //
    // Logical groups
    //
    if (GetStrobeSwizzle (Host, Channel, TargetStrobe) < 8) {
      //
      // North strobes
      //
      DqSwizzleMap = mLogicalBitSwizzleN;
    } else {
      //
      // South strobes
      //
      if (Group == PxcControl) {
        DqSwizzleMap = mLogicalAttackerBitSwizzleS;
      } else {
        DqSwizzleMap = mLogicalBitSwizzleS;
      }
    }
  } else {
    RC_FATAL_ERROR (FALSE, ERR_UNKNOWN_REG_TYPE, RC_FATAL_ERROR_MINOR_CODE_0);
  }

  return DqSwizzleMap[LogicalBit];
}

/*
  Return the value for strobe after applying the internal MC -> DDRDATA swizzling.

  @param[in]  Host            Pointer to sysHost
  @param[in]  Channel         Current channel under test.
  @param[in]  TargetStrobe    Current strobe under test. Valid ranges are:
                              On x4 devices.
                                TargetStrobe goes from 0->17
                              On x8/x16 devices.
                                TargetStrobe goes from 0->17
                                TargetStrobe goes from 0->8

  @return     Return the value for strobe after applying the internal MC -> DDRDATA swizzling.

*/
UINT8
GetStrobeSwizzle (
  IN        PSYSHOST        Host,
  IN        UINT8           Channel,
  IN        UINT8           TargetStrobe
  )
{
  //
  // Verify the input strobe is within boundaries
  //
  if (TargetStrobe >= MAX_STROBE) {
    RC_FATAL_ERROR (TargetStrobe < MAX_STROBE, ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return 0;
  }
  return mDdrioStrobeSwizzle[TargetStrobe];
}

