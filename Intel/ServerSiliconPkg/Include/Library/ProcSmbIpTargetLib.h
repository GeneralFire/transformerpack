/** @file
  ProcSmbIpTargetLib.h

  Target library API function header for ProcSmbIpLib.  This API provides definitions
  for functions related to the Processor SMBus Library. These functions provide specific
  functionality in a simulated/hardware environment.

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

#ifndef _PROC_SMB_IP_TARGET_LIB_H_
#define _PROC_SMB_IP_TARGET_LIB_H_

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
  );

/**

  Returns whether to break execution of the Smbus status test based
  on the build target.

  @param[out] SkipExecution   Whether or not to break.

  @retval EFI_SUCCESS     Completed successfully.
  @retval !EFI_SUCCESS    Failure.
**/
EFI_STATUS
EFIAPI
BreakForSmbDataTarget (
  OUT BOOLEAN  *ShouldBreak
  );

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
  );

/**
  Indicates whether or not SMBus transactions are running in a simulated environment.

  @param  None

  @retval TRUE   - running in a Simulated environement
  @retval FALSE  - not running in a simulated environment
**/
BOOLEAN
EFIAPI
IsSmbSimulated (
  );

#endif //_PROC_SMB_IP_TARGET_LIB_H_

