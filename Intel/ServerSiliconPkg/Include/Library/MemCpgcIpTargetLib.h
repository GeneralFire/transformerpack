/** @file
  Interface header file for the MemCpgcIp build target library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#ifndef _MEM_CPGC_IP_TARGET_LIB_H_
#define _MEM_CPGC_IP_TARGET_LIB_H_

#include <Uefi.h>


/*++

Routine Description:

  Configures the WDB Rd/Wr pointer and Write pointers in fast Mode

  @param[in] Channel          Current Channel (0-Based)
  @param[in] WdbRdWrPointer   Pointer to pointer and subpointers
  @param[in] High             High 32 bits
  @param[in] Low              Low 32 bits

  @retval EFI_SUCCESS     Completed successfully.
  @retval !EFI_SUCCESS    Failure.

--*/

EFI_STATUS
EFIAPI
FastProgramWdbTarget (
  IN UINT8           Channel,
  IN RDWR_POINTER    *WdbRdWrPointer,
  IN UINT32          High,
  IN UINT32          Low
  );

/*++

Routine Description:

  Delay as necessary for a particular build target during the IO test.

  @param[in] Host         Pointer to the Host structure.
  @param[in] Socket       Socket.
  @param[in] Channel      Channel.

  @retval EFI_SUCCESS     Completed successfully.
  @retval !EFI_SUCCESS    Failure.

--*/

EFI_STATUS
EFIAPI
RunIOTest10nmDelayTarget (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel
  );

/*++

Routine Description:

  Returns whether to skip execution of transient err checking based
  on the build target.

  @param[out] SkipExecution   Whether or not to skip execution

  @retval EFI_SUCCESS     Completed successfully.
  @retval !EFI_SUCCESS    Failure.

--*/

EFI_STATUS
EFIAPI
ExecuteCpgcInitializeTransientErrCheckingTarget  (
  OUT BOOLEAN *SkipExecution
  );

/*++

Routine Description:

  Returns whether to break execution of the channel test based
  on the build target.

  @param[out] SkipExecution   Whether or not to break.

  @retval EFI_SUCCESS     Completed successfully.
  @retval !EFI_SUCCESS    Failure.

--*/

EFI_STATUS
EFIAPI
BreakChannelTestTarget (
  OUT BOOLEAN *ShouldBreak
  );

 /**
  Build Target function to setup the fine param.

  @param[out]   LoopCount       Ptr to the loop count.
  @param[out]   NumCl           Ptr to the CL number.
  @param[out]   BurstWait       Ptr to the bursts spacing

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.

**/
EFI_STATUS
EFIAPI
SetupRecEnFineParamTarget (
  OUT UINT8 *LoopCount,
  OUT UINT8 *NumCl,
  OUT UINT8 *BurstWait
  );

#endif  // _MEM_CPGC_IP_TARGET_LIB_H_
