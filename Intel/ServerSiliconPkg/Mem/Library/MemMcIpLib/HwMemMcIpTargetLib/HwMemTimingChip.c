/** @file Hardware implementation of the MemMcIpTargetLib library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 Intel Corporation. <BR>

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

#include <Uefi.h>
#include "Include/MemMcRegs.h"
#include <Library/MemoryCoreLib.h>
#include <Library/MemMcIpTargetLib.h>

/**
  Build Target function to get the Chip Ctl Clk feedback.

  @param[in]   Host               Ptr to SysHost.
  @param[in]   Socket             Socket.
  @param[in]   Channel            Channel.
  @param[in]   OriginalCmdStretch Original cmd stretch.
  @param[out]   MemTimingsAdj     Timing adjustment.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.

**/

EFI_STATUS
EFIAPI
OverrideMemTimingStretchTarget (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT32 OriginalCmdStretch,
  OUT VOID *MemTimingsAdj
  )
{

  return EFI_SUCCESS;

} // OverrideMemTimingStretchTarget

