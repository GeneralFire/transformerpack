/** @file

  MeUmaLib.h

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

#ifndef _ME_UMA_LIB_H_
#define _ME_UMA_LIB_H_

#include <SysHost.h>
#include <MemHost.h>

typedef struct {
  UINT32 NcMemLowBaseAddr;   // base address to provide to ME/IE
  UINT32 NcMemHighBaseAddr;  // base address to provide to ME/IE
  UINT32 NcMemLowLimit;      // Memory limit for testing memory size allocation
  UINT32 NcMemHighLimit;     // Memory limit for testing memory size allocation
} UMA_MEMORY_INFORMATION;

/**
  ME-BIOS hook before checking and triggering the proper type of reset.
**/
VOID
MeUmaHandleResetRequests (
  VOID
  );

/**
  Get the size of ME Segment from the ME FW
  and
  Get the size of IE Segment from the IE FW
  Gathered sizes are set in to environment
**/
VOID
MeUmaConfigureRequestedSegmentSize (
  VOID
  );

/**
  Functions sends DramInitDone messages to ME and IE FWs
  according to passed parameters

  @param [in] MrcStatus             - Return status from MRC code execution
  @param [in] MemSize               - System memory size
  @param [in] PreviousBootError     - Previous boot error
  @param [in] SubBootMode           - SubBoot mode defined in MRC Header
  @param [in] MeMemoryInformation   - Pointer to ME memory information
  @param [in] IeMemoryInformation   - Pointer to ME memory information
**/
VOID
MeUmaSendDramInitDone (
  IN EFI_STATUS             MrcStatus,
  IN UINT32                 MemSize,
  IN UINT8                  PreviousBootError,
  IN SUB_BOOT_MODE          SubBootMode, // Defined in MRC Header
  IN UMA_MEMORY_INFORMATION *MeMemoryInformation,
  IN UMA_MEMORY_INFORMATION *IeMemoryInformation
  );

#endif // _ME_UMA_LIB_H_
