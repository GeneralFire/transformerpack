/** @file
  SwizzleLib.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2018 Intel Corporation. <BR>

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

#ifndef  _SWIZZLE_LIB_H_
#define  _SWIZZLE_LIB_H_


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
  );

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
  );

/*
  Return the indexes of not applicable DDRIO data nibbles.

  @param[in]   Host                   Pointer to sysHost.
  @param[in]   Channel                Current channel under test.
  @param[out]  NibblePtr              The array of indexes of not applicable nibbles.
  @param[out]  Length                 The number of not applcaibel nibbles.

  @return      EFI_INVALID_PARAMETERS There is invalid function parameter.
  @return      EFI_NOT_SUPPORTED      It is not supported memory system.
  @return      EFI_SUCCESS            Sucessfully get the expected nibbles.
*/
EFI_STATUS
EFIAPI
GetNotApplicableNibblesDdr4 (
  IN   PSYSHOST        Host,
  IN   UINT8           Channel,
  OUT  UINT8           **NibblePtr,
  OUT  UINT8           *Length
  );
#endif   // _SWIZZLE_LIB_H_
