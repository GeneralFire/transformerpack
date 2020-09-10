/** @file
  Null implementation of Mp extension library.

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

#include <Library/BaseLib.h>
#include <Library/MpExtensionLib.h>
#include <Uefi/UefiBaseType.h>

/**
  Run simultaneously or one by one under SMM mode on specific package/core or whole system.

  This service lets the caller get all enabled APs to execute a caller-provided function. This function will
  ensure all ApProcedure will exit.

  Note that BSP itself might also execute SmmProcedure if BSP package/core meet the requirement.
  If ApRunType is larger or equal to MaxApRunType, then ASSERT().
  If ApProcedure is NULL, then ASSERT()

  @param  ApRunType             Specify AP run type: All APs, one AP per core or one AP per package.
  @param  Package               Specific package number.  ALL_PACKAGE means the whole system.
  @param  Core                  Specific core number.  ALL_CORE means all cores in the package.
  @param  SingleThread          If TRUE, then all the enabled APs execute the function specified by Procedure one by one.
                                If FALSE, then all the enabled APs execute the function specified by Procedure simultaneously
  @param  ApProcedure           A pointer to the function to be run on enabled APs of the system.
  @param  ProcArguments         Pointer to the optional parameter of the assigned function.

  @retval RETRURN_SUCCESS       In blocking mode, all APs have finished before the timeout expired.
  @retval RETRURN_SUCCESS       In non-blocking mode, function has been dispatched to all enabled APs.

**/
RETURN_STATUS
EFIAPI
MultiProcessorExecute (
  IN AP_RUN_TYPE                  ApRunType,
  IN UINT32                       Package,
  IN UINT32                       Core,
  IN BOOLEAN                      SingleThread,
  IN AP_PROCEDURE                 ApProcedure,
  IN OUT VOID                     *ProcArguments
  )
{
  return EFI_SUCCESS;
}

/**
  Return current CPU logic index and optionally get it location. This function can be invoked by either BSP or AP

  @param CpuInfo   A pointer to the buffer where information for the requested processor is deposited

  @return current ProcessorNumber 

**/
UINTN
EFIAPI
GetProcessorInfo (
  OUT MP_CPU_INFO  *CpuInfo OPTIONAL
  )
{
  return 0;
}
