/** @file
  HwProcSmbChip.c

  Hw Target Lib implementation for ProcSmbIpLib.  This hardware specific implementation
  of the ProcSmbIpTargetLib provides functions for use in a hardware environment for Spr.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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
#include <Library/IoAccessLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/TimerLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/PerformanceTrackerLib.h>
#include <Library/SystemInfoLib.h>
#include "../Include/ProcSmbRegs.h"

/**
  I3C has queues for transactions, and do not need to wait for Host not busy.
  But need Wait for comand queue ready based on target build.
  Command Queue Ready Status field reflects interrupt status
  that is populated upon number of entries in command queue
  is smaller or equal to threshold specified by
  CMD_EMPTY_BUF_THLD in QUEUE_THLD_CTRL
  register. This field is automatically cleared once number of
  entries is greater than the threshold value.

  @param[in] Socket        - Socket number
  @param[in] Instance      - I3C Instance number
  @param[in] I3cStatusReg  - The status register to check

  @retval EFI_SUCCESS     Completed successfully.
  @retval !EFI_SUCCESS    Failure.

**/
EFI_STATUS
EFIAPI
WaitForHostNotBusyTarget (
  IN UINT8   Socket,
  IN UINT8   Instance,
  IN UINT32  I3cStatusReg
  )
{
  UINT32                      Timeout;
  INTR_STATUS_SB_I3C0_STRUCT  I3cStatus;
  EFI_STATUS                  Status = EFI_DEVICE_ERROR;

  Timeout = I3C_TIMEOUT;

  //
  // Wait for there is space in the command queue
  //
  do {
    I3cStatus.Data = UsraCsrRead (Socket, Instance, I3cStatusReg);

    if (I3cStatus.Bits.cmd_queue_ready_stat == 1) {
      Status = EFI_SUCCESS;
      break;
    }

    if (GetEmulation () & SIMICS_FLAG) {
      Status = EFI_SUCCESS;
      break;
    }
    CountTrackingData (PTC_POLLING_COUNT, 1);

    MicroSecondDelay (1);
    Timeout--;

  // Wait for timeout
  } while (Timeout > 0);

  return Status;
}

/**

  Returns whether to break execution of the I3C status test based
  on the build target.

  @param[out] SkipExecution   Whether or not to break.

  @retval EFI_SUCCESS     Completed successfully.
  @retval !EFI_SUCCESS    Failure.
**/
EFI_STATUS
EFIAPI
BreakForSmbDataTarget (
  OUT BOOLEAN  *ShouldBreak
  )
{

  *ShouldBreak = FALSE;

  return EFI_SUCCESS;

} // BreakForSmbDataTarget

/**
  Wait for I3C write to complete based on target build.

  @param[in] Socket       - Socket number
  @param[in] Instance     - I3C Instance number
  @param[in] I3cStatusReg - Status register to check

  @retval EFI_SUCCESS     Completed successfully.
  @retval !EFI_SUCCESS    Failure.
**/
EFI_STATUS
EFIAPI
WaitForWriteToCompleteTarget (
  IN UINT8   Socket,
  IN UINT8   Instance,
  IN UINT32  I3cStatusReg
  )
{
  UINT32                      Timeout;
  INTR_STATUS_SB_I3C0_STRUCT  I3cStatus;
  EFI_STATUS                  Status = EFI_DEVICE_ERROR;

  Timeout = I3C_TIMEOUT;

  //
  // Wait for the write to complete
  //
  do {
    I3cStatus.Data = UsraCsrRead (Socket, Instance, I3cStatusReg);

    if (I3cStatus.Bits.resp_ready_stat == 1) {
      Status = EFI_SUCCESS;
      break;
    }
    CountTrackingData (PTC_POLLING_COUNT, 1);

    MicroSecondDelay (1);
    Timeout--;

  } while (Timeout > 0);

  return Status;
}

/**
  Indicates whether or not I3C transactions are running in a simulated environment.

  @param  None

  @retval TRUE   - running in a Simulated environement
  @retval FALSE  - not running in a simulated environment
**/
BOOLEAN
EFIAPI
IsSmbSimulated (
  )
{
  // Running on hardware
  return FALSE;
}

