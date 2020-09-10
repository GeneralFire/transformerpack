/** @file
  Interface header file for the Checkpoint library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

#ifndef _CHECKPOINT_LIB_H_
#define _CHECKPOINT_LIB_H_

#include <Uefi.h>
#include <Memory/MemoryCheckpointCodes.h>

#ifdef DEBUG_CODE_BLOCK
#define OutputExtendedCheckpoint(dbgInfo)
#else //DEBUG_CODE_BLOCK
#define OutputExtendedCheckpoint(dbgInfo) OutputCheckpoint dbgInfo
#endif //DEBUG_CODE_BLOCK

/**

  Socket SBSP writes the checkpoint code to the checkpoint CSR on socket SBSP and calls
  the platform checkpoint routine.

  Socket N reads the breakpoint CSR on socket N. If the breakpoint code matches the
  checkpoint code, then execution will stall in a loop until the breakpoint CSR
  on socket N is changed via ITP or other means.

  @param[in] MajorCode - Major Checkpoint code to write
  @param[in] MinorCode - Minor Checkpoint code to write
  @param[in] Data      - Data specific to the minor checkpoint is written to
                         low word of the checkpoint CSR

  @retval N/A

**/
VOID
EFIAPI
OutputCheckpoint (
  IN UINT8    MajorCode,
  IN UINT8    MinorCode,
  IN UINT16   Data
  );

/**

  Socket SBSP writes the checkpoint code to the checkpoint CSR on socket SBSP and calls
  the platform checkpoint routine.

  Socket N reads the breakpoint CSR on socket N. If the breakpoint code matches the
  checkpoint code, then execution will stall in a loop until the breakpoint CSR
  on socket N is changed via ITP or other means.

  @param[in] Socket    - Socket to write Checkpoint
  @param[in] MajorCode - Major Checkpoint code to write
  @param[in] MinorCode - Minor Checkpoint code to write
  @param[in] Data      - Data specific to the minor checkpoint is written to
                         low word of the checkpoint CSR

  @retval N/A

**/

VOID
EFIAPI
OutputCheckpointSocket (
  IN UINT8    Socket,
  IN UINT8    MajorCode,
  IN UINT8    MinorCode,
  IN UINT16   Data
  );

#endif // #ifndef _CHECKPOINT_LIB_H_
