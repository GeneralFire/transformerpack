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
#include "Ddr5Core.h"

/*++
  Collects the results of the previous test

  @param Host         - Pointer to sysHost
  @param socket       - Processor socket to check
  @param chToCheck    - Channel to check
  @param bwSerr       - Bit wise error status

  @retval status      - Non-zero if any failure was detected

--*/
UINT32
CollectTestResultsDdr5 (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN UINT32        ChToCheck,
  IN OUT UINT32    BwSerr[MAX_CH][SUB_CH][3]
  )
{
  return  SUCCESS;
} // CollectTestResultsDdr5

/*++
  Collects the results of the previous test

  @param[in] Host         - Pointer to sysHost
  @param[in] socket       - Processor socket to check
  @param[in] chToCheck    - Channel to check
  @param[in] bwSerr       - Bit wise error status

  @retval status      - Non-zero if any failure was detected

--*/
UINT32
CollectTestResultsEvenOddDdr5 (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN UINT32        ChToCheck,
  IN OUT UINT32    BwSerr[MAX_CH][SUB_CH][2][3]
  )
{
  return  SUCCESS;
} // CollectTestResultsEvenOddDdr5

/*++

  Write DQDQS Pre DFE 2D Centering

  @param[in] Host      - Pointer to Host structure

  @retval EFI_SUCCESS

--*/

UINT32
EFIAPI
WriteDqDqsPreDfe2DCentering (
  IN PSYSHOST  Host
  )
{
  return SUCCESS;
} // WriteDqDqsPreDfe2DCentering

/*++

  Write DQDQS Post DFE 2D Centering

  @param[in] Host      - Pointer to Host structure

  @retval EFI_SUCCESS

--*/

UINT32
EFIAPI
WriteDqDqsPostDfe2DCentering (
  IN PSYSHOST  Host
  )
{
  return SUCCESS;
} // WriteDqDqsPostDfe2DCentering

/*++

  Read DQDQS Pre DFE 2D Centering

  @param[in] Host      - Pointer to Host structure

  @retval EFI_SUCCESS

--*/

UINT32
EFIAPI
ReadDqDqsPreDfe2DCentering (
  IN PSYSHOST  Host
  )
{
  return SUCCESS;
} // ReadDqDqsPreDfe2DCentering

/*++

  Read DQDQS Post DFE 2D Centering

  @param[in] Host      - Pointer to Host structure

  @retval EFI_SUCCESS

--*/

UINT32
EFIAPI
ReadDqDqsPostDfe2DCentering (
  IN PSYSHOST  Host
  )
{
  return SUCCESS;
} // ReadDqDqsPostDfe2DCentering

/**

  Tx DQ and DQS Duty cycle correction

  @param[in] Host       - Pointer to sysHost
  @param[in] socket     - Socket number

  @retval N/A

**/
VOID
EFIAPI
TxDqDqsDutyCycleCorrection (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  return;
}

