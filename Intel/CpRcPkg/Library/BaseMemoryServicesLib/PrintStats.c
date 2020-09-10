/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include <Library/MemoryCoreLib.h>
#include <Library/PipeSyncLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PerformanceTrackerLib.h>

/**

  Print statistics

  @param[in] Host - Pointer to SysHost

  @retval N/A

**/
UINT32
EFIAPI
PrintAllStats (
  IN PSYSHOST Host
  )
{

  if (PcdGetBool (PcdMrcDebugPerformanceStats) == TRUE) {

    UINT8 Socket;

    Socket = Host->var.mem.currentSocket;

    //
    // Return if this socket is disabled
    //
    if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
      return SUCCESS;
    }

    PrintPerformanceStats (Socket, Host->var.mem.FmcMaxCached, Host->var.mem.FmcCachedReads);

    BuildPerfStatsHob ();

  }

  return SUCCESS;

} // PrintStats

