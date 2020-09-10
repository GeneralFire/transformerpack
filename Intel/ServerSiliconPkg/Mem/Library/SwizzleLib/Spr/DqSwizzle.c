/** @file
  Interface source file for the Swizzle library class.

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

#include <Library/BaseLib.h>
#include "SysHost.h"
#include <Library/MemoryServicesLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/SwizzleLib.h>
#include <Library/Ddr5CoreLib.h>
#include <Library/MemDdrioIpLib.h>
//
// Strobe swizzling from MC SPID to DDRDATA registers(fixed 1:1 mapping to bump) for DDR5 with DDRIO lane reversal disabled.
//
UINT8 mDdrioStrobeSwizzleDdr5Normal[MAX_STROBE_DDR5] =
{
   0,  2,  4,  6,  8,  1,  3,  5,  7,  9,       //< DDRIO_Wave3_Swizzling Version 2.pptx (Page 16)
  17, 15, 13, 11, 18, 16, 14, 12,  10, 19       //< DDRIO_Wave3_Swizzling Version 2.pptx (Page 22)
};

//
// Strobe swizzling from MC SPID to DDRDATA registers(fixed 1:1 mapping to bump) for DDR5 with DDRIO lane reversal enabled.
//
UINT8 mDdrioStrobeSwizzleDdr5LaneReversal[MAX_STROBE_DDR5] =
{
  10, 12, 14, 16, 18, 11, 13, 15, 17, 19,       //< DDRIO_Wave3_Swizzling Version 2.pptx (Page 22)
   7,  5,  3,  1,  8,  6,  4,  2,  0,  9        //< DDRIO_Wave3_Swizzling Version 2.pptx (Page 16)
};


//
// DDRIO_Wave3_Swizzling Version 2.pptx (Page 11-15) Table should be used for Physical Strobes (0-9)
// DDRIO_Wave3_Swizzling Version 2.pptx (Page 17-21) Table should be used for Physical Strobes (10-19) when reversal is enabled
//
UINT8 mSAIx[BITS_PER_NIBBLE] = {
  0, 1, 2, 3
};

//
// DDRIO_Wave3_Swizzling Version 2.pptx (Page 17-21) Table should be used for Physical Strobes (10-19)
// DDRIO_Wave3_Swizzling Version 2.pptx (Page 11-15) Table should be used for Physical Strobes (0-9) when reversal is enabled
//
UINT8 mSBIx[BITS_PER_NIBBLE] = {
  1, 0, 3, 2
};


//
// Strobe swizzling from MC SPID to DDRDATA registers(fixed 1: 1 mapping to bump) for DDR4 with DDRIO lane reversal disabled.
// Data nibble register sequence: {0, 2, 4, 6, 17, 15, 13, 11}  | {1, 3, 5, 7, 16, 14, 12, 10}
// ECC  nibble registers: 8, 9.
//
UINT8 mDdrioStrobeSwizzleDdr4Normal[MAX_STROBE] =
{
  0, 2, 4, 6, 17, 15, 13, 11, 8,
  1, 3, 5, 7, 16, 14, 12, 10, 9
};

//
// Strobe swizzling from MC to DDRDATA registers (1: 1 mapping to bump) for DDR4 with DDRIO lane reversal enabled.
// Data strobe register sequence: {10, 12, 14, 16, 7, 5, 3, 1}  | {11, 13, 15, 17, 6, 4, 2, 0}
// ECC strobe registers: 18, 19.
//
UINT8 mDdrioStrobeSwizzleDdr4LaneReversal[MAX_STROBE] =
{
  10, 12, 14, 16, 7, 5, 3, 1, 18,
  11, 13, 15, 17, 6, 4, 2, 0, 19
};

//
// i4_ch0_sb_ddrd.N0 and i4_ch0_sb_ddrd.N1 are not accplicable for DDR4 when lane reversal is disabled.
//
UINT8 mNotApplicableNibblesDdr4Normal[MAX_STROBE_DDR5 - MAX_STROBE_DDR4] = {
  18, 19
};

//
//
// i4_ch0_sa_ddrd.N0 and i4_ch0_sa_ddrd.N1 are not accplicable for DDR4 when lane reversal is enabled.
//
//
UINT8 mNotApplicableNibblesDdr4LaneReversal[MAX_STROBE_DDR5 - MAX_STROBE_DDR4] = {
  8, 9
};

/*
  Return the value for DQ bit after applying the internal swizzling.
  Swizzling refers to mapping between a physical pin and a logical function.

  @param[in]  Channel         Current channel under test.
  @param[in]  TargetStrobe    Current strobe under test. Valid ranges are:
                              TargetStrobe goes from 0->19
  @param[in]  LogicalBit      Current bit under test. Valid ranges are:
                              LogicalBit goes from 0->7

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
  UINT8     MSVx4;
  UINT8     MSVx8;
  UINT8     MSVx4SbCh;
  UINT8     MSVx8SubCh;
  UINT8     PhysicalStrobe;
  UINT8     *mSxIx;

  Host  = GetSysHostPointer ();
  MSVx4 = GetMaxStrobeValid (Host);
  MSVx8 = MSVx4 / 2;
  MSVx4SbCh = GetSubChMaxStrobeValid (Host);

  MSVx8SubCh = MSVx4SbCh / 2;

  if (LogicalBit >= MAX_BITS_IN_BYTE) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_229);
    return 0;
  }

  if (TargetStrobe >= MSVx4) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_230);
    return 0;
  }

  if (LogicalBit >= BITS_PER_NIBBLE) {

    LogicalBit = LogicalBit % BITS_PER_NIBBLE;

    if ((TargetStrobe % MSVx4SbCh) >= MSVx8SubCh) {
      //
      // If logicalbit >= 4
      // TargetStrobe MUST NOT be 5-9 nor 15-19
      //
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_231);
      return 0;
    } else {

      TargetStrobe += MSVx8SubCh;

    }
  }

  PhysicalStrobe = GetStrobeSwizzle (Host, Channel, TargetStrobe);

  if (IsLaneReversalEnabled (Host, Host->var.mem.currentSocket, Channel)) {

    if (PhysicalStrobe < MSVx8) {

      mSxIx = mSBIx;

    } else {

      mSxIx = mSAIx;

    }

  } else {

    if (PhysicalStrobe < MSVx8) {

      mSxIx = mSAIx;

    } else {

      mSxIx = mSBIx;

    }

  }

  return mSxIx[LogicalBit];

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
  UINT8  Socket;

  Socket = Host->var.mem.currentSocket;

  //
  // Verify the input strobe is within boundaries
  //
  if (TargetStrobe >= MAX_STROBE) {
    RC_FATAL_ERROR (TargetStrobe < MAX_STROBE, ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return 0;
  }

  if (IsDdr5Present (Host, Socket)) {
    if (IsLaneReversalEnabled(Host, Socket, Channel) == FALSE) {
      return mDdrioStrobeSwizzleDdr5Normal[TargetStrobe];
    } else {
      return mDdrioStrobeSwizzleDdr5LaneReversal[TargetStrobe];
    }
  } else {
    if (IsLaneReversalEnabled(Host, Socket, Channel) == FALSE) {
      return mDdrioStrobeSwizzleDdr4Normal[TargetStrobe];
    } else {
      return mDdrioStrobeSwizzleDdr4LaneReversal[TargetStrobe];
    }

  }
}

/*
  Get the indexes of not applicable DDRIO data nibbles for SPR DDR4. These not applicable nibbles
  should be disabled.

  @param[in]   Host                   Pointer to sysHost
  @param[in]   Channel                Current channel under test.
  @param[out]  NibblePtr              The array of indexes of not applicable nibbles.
  @param[out]  Length                 The number of not applcaibel nibbles.

  @return      EFI_INVALID_PARAMETERS There is invalid function parameter.
  @return      EFI_NOT_SUPPORTED      It is not DDR4 memory system.
  @return      EFI_SUCCESS            Sucessfully get the expected nibbles.
*/
EFI_STATUS
EFIAPI
GetNotApplicableNibblesDdr4 (
  IN   PSYSHOST        Host,
  IN   UINT8           Channel,
  OUT  UINT8           **NibblePtr,
  OUT  UINT8           *Length
 )
{
  UINT8  Socket;

  Socket = Host->var.mem.currentSocket;

  if ((NibblePtr == NULL) || (Length == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (IsDdr5Present (Host, Socket) == TRUE) {
    *Length = 0;
    return EFI_UNSUPPORTED;
  }

  if (IsLaneReversalEnabled(Host, Socket, Channel)) {
    *NibblePtr = mNotApplicableNibblesDdr4LaneReversal;
  } else {
    *NibblePtr = mNotApplicableNibblesDdr4Normal;
  }

  *Length = MAX_STROBE_DDR5 - MAX_STROBE_DDR4;

  return EFI_SUCCESS;
}
