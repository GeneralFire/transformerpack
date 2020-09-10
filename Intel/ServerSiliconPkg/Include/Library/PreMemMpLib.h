/** @file
  Pre-Memory MP library class header file.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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
**/

#ifndef _PRE_MEM_MP_LIB_H_
#define _PRE_MEM_MP_LIB_H_

/**
  AP task function pointer type.

  @param[in]      Context       Optional context pointer.
**/
typedef
VOID
(EFIAPI *PRE_MEM_MP_AP_TASK) (
  IN      VOID                *Context        OPTIONAL
  );

/**
  Initialize the pre-memory MP service.

  @retval EFI_SUCCESS               Initialized successfully.
  @retval EFI_ALREADY_STARTED       Initialization has already been done.
  @retval EFI_UNSUPPORTED           NBSP's logical ID is out of range.
**/
EFI_STATUS
EFIAPI
InitPreMemMp (
  VOID
  );

/**
  Get the AP list for the running socket.

  @param[in]      IncludeNbsp   TRUE to include the NBSP in the list;
                                FALSE to omit the NBSP from the list.
  @param[in]      AllThreads    TRUE to include all threads of each core;
                                FALSE to include only one thread from each core.
  @param[in, out] ApCount       If LogicalIdList is null, outputs the total.
                                Otherwise, inputs the size of the list provided,
                                and outputs the size used (up to input size).
  @param[out]     LogicalIdList Optional pointer to output the list.

  @retval EFI_SUCCESS               List (or count) returned successfully.
  @retval EFI_INVALID_PARAMETER     Required pointer parameter is null.
  @retval EFI_BUFFER_TOO_SMALL      List storage provided, but count is zero.
  @retval EFI_WARN_BUFFER_TOO_SMALL Partial list returned.
  @retval EFI_NOT_FOUND             No applicable APs found, count set to zero.
**/
EFI_STATUS
EFIAPI
GetApList (
  IN      BOOLEAN             IncludeNbsp,
  IN      BOOLEAN             AllThreads,
  IN OUT  UINT32              *ApCount,
  OUT     UINT32              *LogicalIdList  OPTIONAL
  );

/**
  Dispatch an AP.

  If the ApTask is provided, the AP calls it and passes ApContext to it.
  ApContext is intended to be a pointer to a data structure understood by both
  the caller and the ApTask, or NULL if no context is needed. However, no
  checking is done.

  If the ApDoneCount is provided, the AP will increment the memory it points to
  after returning from the ApTask. The caller is expected to set the count to
  zero before calling this function. It may then dispatch one or more APs, and
  then wait for the count to be equal to the number of APs dispatched (see
  WaitForApDoneCount).

  If either ApContext or ApDoneCount point to memory that is part of the
  caller's stack frame (i.e. local variables), as opposed to the heap
  (i.e. allocated memory), then the caller should wait for any APs it dispatches
  to be done before the stack frame is popped.

  @param[in]      LogicalId     Logical ID of the AP to dispatch.
  @param[in]      ApTask        Optional function pointer for the AP to execute.
  @param[in]      ApContext     Optional context pointer to pass to the task.
  @param[in]      ApDoneCount   Optional pointer to a value to increment.

  @retval EFI_SUCCESS               AP dispatched successfully.
  @retval EFI_INVALID_PARAMETER     Logical ID is out of range.
  @retval EFI_UNSUPPORTED           Logical ID is the NBSP's.
  @retval EFI_NOT_FOUND             AP is not found.
  @retval EFI_NOT_STARTED           AP is asleep.
  @retval EFI_NOT_READY             AP is busy.
**/
EFI_STATUS
EFIAPI
DispatchAp (
  IN      UINT32              LogicalId,
  IN      PRE_MEM_MP_AP_TASK  ApTask,         OPTIONAL
  IN      VOID                *ApContext,     OPTIONAL
  IN      UINT32              *ApDoneCount    OPTIONAL
  );

/**
  Wait for AP done count.

  If ApDoneCount has been zeroed and then passed to one or more APs via
  DispatchAp, this function can be called to wait until a given number of APs
  have incremented it.

  @param[in]      ApDoneCount   Pointer to the value being incremented by APs.
  @param[in]      WaitCount     Specific count to wait for.
  @param[in]      TimeLimit     Maximum microseconds to wait, or 0 if no limit.

  @retval EFI_SUCCESS               Value met or exceeded WaitCount.
  @retval EFI_INVALID_PARAMETER     Required pointer parameter is null.
  @retval EFI_TIMEOUT               Wait time exceeded TimeLimit.
**/
EFI_STATUS
EFIAPI
WaitForApDoneCount (
  IN      volatile UINT32     *ApDoneCount,
  IN      UINT32              WaitCount,
  IN      UINT32              TimeLimit
  );

#endif // _PRE_MEM_MP_LIB_H_
