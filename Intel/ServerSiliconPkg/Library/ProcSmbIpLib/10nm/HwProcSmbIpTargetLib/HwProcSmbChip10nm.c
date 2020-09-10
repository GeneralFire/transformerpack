/** @file
  HwProcSmbChip.c

  Hw Target Lib implementation for ProcSmbIpLib.  This hardware specific implementation
  of the ProcSmbIpTargetLib provides functions for use in a hardware environment for 10nm.

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
#include <Library/CsrAccessLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/PerformanceTrackerLib.h>
#include <Library/SystemInfoLib.h>
#include "../Include/ProcSmbRegs.h"

#define SMB_TIMEOUT       100000   // 100 ms

/**
  Wait for smbus host not busy. The function waits based on the build
  target.

  @param[in] Socket      - Socket number
  @param[in] Instance    - SmBus Instance number
  @param[in] SmbStatReg  - The status register to check

  @retval EFI_SUCCESS     Completed successfully.
  @retval !EFI_SUCCESS    Failure.

**/
EFI_STATUS
EFIAPI
WaitForHostNotBusyTarget (
  IN UINT8   Socket,
  IN UINT8   Instance,
  IN UINT32  SmbStatReg
  )
{
  UINT64                      StartCount;
  SMB_STATUS_SMB_SPD0_STRUCT  SmbStat;
  EFI_STATUS                  Status = EFI_SUCCESS;
  PSYSHOST                    Host;

  Host = GetSysHostPointer ();

  //
  // Wait for Host not busy
  //
  StartCount = GetCountTsc ();
  do {
    SmbStat.Data = UsraCsrRead (Socket, Instance, SmbStatReg);
    if (!SmbStat.Bits.smb_busy) {
      break;
    }
    if (GetEmulation () & SIMICS_FLAG) {
      break;
    }

    CountTrackingData (PTC_POLLING_COUNT, 1);

  // Wait for timeout
  } while (GetDelayTsc (StartCount) < SMB_TIMEOUT);
  return Status;
}

/*++

  Returns whether to break execution of the Smbus status test based
  on the build target.

  @param[out] SkipExecution   Whether or not to break.

  @retval EFI_SUCCESS     Completed successfully.
  @retval !EFI_SUCCESS    Failure.

--*/
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
  Wait for smbus write to complete based on target build.

  @param[in] Socket       - Socket number
  @param[in] Instance     - SmBus Instance number
  @param[in] SmbStatusReg - Status register to check

  @retval EFI_SUCCESS     Completed successfully.
  @retval !EFI_SUCCESS    Failure.
**/
EFI_STATUS
EFIAPI
WaitForWriteToCompleteTarget (
  IN UINT8   Socket,
  IN UINT8   Instance,
  IN UINT32  SmbStatusReg
  )
{
  SMB_STATUS_SMB_SPD0_STRUCT  SmbStatus;
  EFI_STATUS                  Status;
  PSYSHOST                    Host;

  Host = GetSysHostPointer ();

  //
  // Wait for the write to complete
  //
  do {
    SmbStatus.Data = UsraCsrRead (Socket, Instance, SmbStatusReg);
    CountTrackingData (PTC_POLLING_COUNT, 1);

  } while (!SmbStatus.Bits.smb_wod && !SmbStatus.Bits.smb_sbe);

  // If SMB Status indicates write completed with no error, return success
  if ((SmbStatus.Bits.smb_wod == 1) && (SmbStatus.Bits.smb_sbe == 0)) {
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Indicates whether or not SMBus transactions are running in a simulated environment.

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

