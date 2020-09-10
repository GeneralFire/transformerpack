/** @file
  This file implements the MemCpgcIpTargetLib library class.

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

#include <Library/BaseLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/CteNetLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemCpgcIpLib.h>
#include "../Include/MemCpgcRegs.h"
#include "../Include/CpgcChip.h"
#include "../Include/MemCpgcTestTable.h"

CPGC_TEST_ENV_PARAM CpgcTestEnvParam[] = {
// TestType, LoopCount, NumCacheLine, BurstWait
  {RcvenFineTest,                                   32, 12, 40}, // Due to the long roundtrip latency, NumCacheLine is 12.
  {MdqsReceiveEnablePhaseTraining,                   4, 32, 16},
  {MdqsCoarseReadDelayTraining,                      8, 32, 16},
  {BacksideDqSwizzleDiscovery,                       1, 32,  0},
  {FrontsideDqSwizzleDiscovery,                      1, 32,  0},
  {MdqsReadDelayDeskew,                              8, 32, 16},
  {MdqsWriteDelay,                                   8, 32, 16},
  {DqSwzDiscTestDdr5,                                1, 32,  0},
  {RdDqTest,                                         1, 64, 64},
  {ReadPreDfe2DCenteringTest,                        1, 64, 64},
  {ReadDfeTest,                                      1, 255, 64},
  {ReadPostDfe2DCenteringTest,                      64, 32, 64},
  {WLDdr5,                                          32,  1, 40},
  {WrDqTest,                                         1, 32, 64},
  {DqSlewRateTest,                                   1, 32, 64},
  {WritePreDfe2DCenteringTest,                       1, 16, 64}, // [16Wr-16Rd] x 1 times
  {WriteDfeTest,                 CPGC_LOOP_COUNT_65536, 32, 64},
  {WriteDbDfeTest,               DB_DFE_LOOP_COUNT, DB_DFE_CAHCE_LINES, 64},
  {WritePostDfe2DCenteringTest,  CPGC_LOOP_COUNT_65536, 32, 64}, // [32Wr-Idle-32Wr-Idle-32Rd-Idle-32Rd-Idle] x 32 times
  {RmtDqTest,                    CPGC_LOOP_COUNT_65536, 32, 64}, // [32Wr-Idle-32Wr-Idle-32Rd-Idle-32Rd-Idle] x 16 times
  {RmtCmdTest,                                      16, 32, 64},
  {TurnAroundSgTest,                                16,  2,  0}, // [4Wr-4Rd] x 2 base repeats x 16 block repeates
  {TurnAroundSrTest,                                16,  2,  0}, // [4Wr-4Rd] x 2 base repeats x 16 block repeates
  {TurnAroundDrDdWwRrWrTest,                        16,  2,  0}, // [8Wr-8Rd] x 2 base repeats x 16 block repeates
  {TurnAroundDrDdRwTest,                            16,  2,  0}, // [8Wr-8Rd] x 2 base repeats x 16 block repeates
};

UINT8 CpgcTestEnvParamSize = sizeof(CpgcTestEnvParam) / sizeof(CPGC_TEST_ENV_PARAM);

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
  )
{

  return EFI_SUCCESS;

} // RunIOTest10nmDelayTarget

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
  )
{
  *SkipExecution = FALSE;

  return EFI_SUCCESS;

} // ExecuteCpgcInitializeTransientErrCheckingTarget

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
  )
{

  *ShouldBreak = FALSE;

  return EFI_SUCCESS;

} // BreakChannelTestTarget
