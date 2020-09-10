/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#include <Library/HbmMemLib.h>
#include <Library/RcDebugLib.h>
#include <Library/HbmCoreLib.h>
#include <Library/SystemInfoLib.h>

/**
  This function gets the HBM stack information for the specified socket.

  @param[in]  SocketId              Socket index.
  @param[out] HbmStackInfo          HBM stack information.

  @retval EFI_SUCCESS               This function got the register data successfully.
  @retval EFI_UNSUPPORTED           HBM is not supported on this socket.
  @retval EFI_INVALID_PARAMETER     Some of input parameters are invalid.
**/
EFI_STATUS
EFIAPI
GetHbmStackInfo (
  IN  UINT8               SocketId,
  OUT HBM_STACK_INFO      *HbmStackInfo
  )
{
  UINT8                   HbmIoId;
  UINT8                   MaxHbmIo;
  UINT32                  StackMemSize;

  if ((SocketId >= MAX_SOCKET) || (HbmStackInfo == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  MaxHbmIo = GetMaxIoHbm ();
  HbmStackInfo->MemSizePerStack      = 0;
  HbmStackInfo->HbmStackValidBitMask = 0;

  //
  // Get the HBM stack memory size and collect the bit mask of valid HBM stacks.
  // Check if the memory sizes are identical for each HBM stack.
  //
  for (HbmIoId = 0; HbmIoId < MaxHbmIo; HbmIoId++) {
    if (!IsHbmMemSsEnabled (SocketId, HbmIoId)) {
      continue;
    }

    StackMemSize = GetHbmCacheMemSize (SocketId, HbmIoId);
    HbmStackInfo->HbmStackValidBitMask |= (BIT0 << HbmIoId);

    if (HbmStackInfo->MemSizePerStack == 0) {
      HbmStackInfo->MemSizePerStack = StackMemSize;
    } else if (HbmStackInfo->MemSizePerStack != StackMemSize) {
      RcDebugPrintWithDevice (SDBG_MAX, SocketId, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "HBM: Stack memory size is mismatched: 0x%x != 0x%x (64MB)! Force stack memory size to 0x%x (64MB)\n",
        HbmStackInfo->MemSizePerStack,
        StackMemSize,
        HbmStackInfo->MemSizePerStack
        );
    }
  }

  RcDebugPrintWithDevice (SDBG_MAX, SocketId, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "HBM: MemSizePerStack: 0x%x (64MB), StackValidBitMask: 0x%x\n",
    HbmStackInfo->MemSizePerStack,
    HbmStackInfo->HbmStackValidBitMask
    );

  return EFI_SUCCESS;
}
