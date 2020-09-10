/** @file
  High Speed IO (HSIO) SoC layer library for EBG PCH

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/PchPcrLib.h>
#include <Register/PchPcrRegs.h>
#include <Library/PeiHsioLib.h>

/**
  Get HSIO lane representation needed to perform any operation on the lane.

  @param[in]  LaneIndex  HSIO lane number to be examined
  @param[out] HsioLane   HSIO lane representation
**/
VOID
HsioGetLane (
  IN   UINT8       LaneIndex,
  OUT  HSIO_LANE   *HsioLane
  )
{
  // EBG PCH lanes definition:
  // iolane  0 -  7 : 0xAB - 000, 400, 800, c00, 1000, 1400, 1800, 1c00, 2000, 2400
  // iolane  8 - 15 : 0xAA - 000, 400, 800, c00, 1000, 1400, 1800, 1c00, 2000, 2400, 2800, 2c00
  // iolane 16 - 23 : 0xA9 - 000, 400, 800, c00, 1000, 1400, 1800, 1c00
  // iolane 24 - 37 : 0xA8 - 000, 400, 800, c00, 1000, 1400, 1800, 1c00
  static UINT8 IoLanes[]    = {0, 8, 16, 24};
  static UINT8 Pids[]       = {PID_MODPHY0, PID_MODPHY1, PID_MODPHY2};

  UINT8  IndexMax;
  UINT32 Index;

  if (HsioLane == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: NULL pointer detected!\n", __FUNCTION__));
    ASSERT (FALSE);
    return;
  }

  IndexMax  = ARRAY_SIZE (IoLanes) - 1;

  ASSERT (LaneIndex < IoLanes[IndexMax]);

  for (Index = 0; Index < IndexMax; ++Index) {
    if (LaneIndex < IoLanes[Index + 1]) {
      HsioLane->Index = LaneIndex;
      HsioLane->Pid   = Pids[Index];
      HsioLane->Base  = (LaneIndex - IoLanes[Index]) * 0x400;
      return;
    }
  }
  ASSERT (FALSE);
}
