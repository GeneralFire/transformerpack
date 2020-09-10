/** @file Hardware implementation of the MemMcIpTargetLib library class.

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

#include <Uefi.h>
#include "Include/MemMcRegs.h"
#include <Library/MemoryCoreLib.h>
#include <Library/MemMcIpTargetLib.h>
#include <Register/Cpuid.h>
#include <Library/SysHostPointerLib.h>
#include <RcRegs.h>
#include <Library/UsraCsrLib.h>
#include <Library/EmulationConfigurationLib.h>

/**
  Build Target function to get the Cmd Margins Sweep Error Res.

  @param[in]   Signal           Signal.
  @param[in]   Dimm             Dimm.
  @param[in]   PiIndex          PiIndex.
  @param[in]   CurError         Current error.
  @param[out]  ErrorResult      Ptr to the ErrorResult.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.

**/

EFI_STATUS
EFIAPI
GetCmdMarginsSweepErrorResFeedbackTarget (
  IN GSM_CSN Signal,
  IN UINT8   Dimm,
  IN INT16   PiIndex,
  IN UINT8   CurError,
  OUT UINT8 *ErrorResult
  )
{
  *ErrorResult = 0;

  return EFI_SUCCESS;

} // GetCmdMarginsSweepErrorResFeedbackTarget

/**

  Provides RcSim feedback for RCD QxCA Training

  @param[in]  signal        Signal being swept
  @param[in]  dimm          Current DIMM
  @param[in]  piIndex       Index to be used in feedback
  @param[in]  CurError      Current Pi Delay feedback error
  @param[out] ErrorResult   Ptr to the ErrorResult

  @retval EFI_SUCCESS       Completed successfully
  @retval !EFI_SUCCESS      Error

**/
EFI_STATUS
GetCmdMarginsBacksideSweepErrorResFeedbackTarget (
  IN  GSM_CSN Signal,
  IN  UINT8   Dimm,
  IN  INT16   PiIndex,
  IN  UINT8   CurError,
  OUT UINT8   *ErrorResult
  )
{
  *ErrorResult = 0;

  return EFI_SUCCESS;

} // GetCmdMarginsBacksideSweepErrorResFeedback

/**
  Build Target function to set the Erid Lfsr Err from feedback.

  @param[in]   Host             Ptr to SysHost.
  @param[in]   Socket           Socket Number.
  @param[in]   Channel          Channel.
  @param[in]   Index            Index.
  @param[in]   EridOffset       Erid offset.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
SetEridLfsrErrFromFeedbackTarget (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Index,
  IN UINT16   EridOffset
  )
{

  return EFI_SUCCESS;

} // SetEridLfsrErrFromFeedbackTarget

/**
  Build Target function to set the Erid Lfsr Err from feedback.

  @param[in]   Strobe           Strobe.
  @param[in]   Dword            Dword.
  @param[in]   PiDelay          PiDelay.
  @param[out]  CsrReg           Csr register.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
GetWlFineResultsFeedbackTarget (
  IN UINT8 Strobe,
  IN UINT8 Dword,
  IN UINT16 PiDelay,
  OUT UINT32 *CsrReg
  )
{

  return EFI_SUCCESS;

} // GetWlFineResultsFeedbackTarget

/**
  Build Target function to cache DdrIo.

  @param[in]   Host             Ptr to SysHost.
  @param[in]   Socket           Socket Number.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.

**/

EFI_STATUS
EFIAPI
CacheDdrIoTarget (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{

  return EFI_SUCCESS;

} // CacheDdrIoTarget

/**
  Build Target function to get the Chip Ctl Clk feedback.

  @param[in]   Host             Ptr to SysHost.
  @param[in]   Channel          Channel.
  @param[in]   Dimm             Dimm.
  @param[in]   Rank             Rank.
  @param[in]   Sign             Sign.
  @param[out]  CtlMargins       Margins.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.

**/

EFI_STATUS
EFIAPI
GetChipCtlClkFeedBackTarget (
  IN PSYSHOST Host,
  IN UINT8 Channel,
  IN UINT8 Dimm,
  IN UINT8 Rank,
  IN UINT8 Sign,
  OUT INT16 *CtlMargins
  )
{

  return EFI_SUCCESS;

} // GetChipCtlClkFeedBackTarget

/**
  Build Target function to populate the Mem Chip policy.

  @param[in]   Host             Ptr to SysHost.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.

**/

EFI_STATUS
EFIAPI
PopulateMemChipPolicyTarget (
  IN PSYSHOST Host
  )
{

  return EFI_SUCCESS;

} // PopulateMemChipPolicyTarget

/**
  Init CAPID's that are used to track nested call levels for semaphores

  @param[in]  SocketId       Socket index.

  @retval None
**/
VOID
EFIAPI
LoadCapId (
  IN  UINT8    SocketId
  )
{
  PSYSHOST     Host;

  Host = GetSysHostPointer ();
  Host->var.common.procCom[SocketId].capid0 = UsraCsrRead (SocketId, 0, CAPID0_PCU_FUN3_REG);
  Host->var.common.procCom[SocketId].capid1 = UsraCsrRead (SocketId, 0, CAPID1_PCU_FUN3_REG);
  Host->var.common.procCom[SocketId].capid2 = UsraCsrRead (SocketId, 0, CAPID2_PCU_FUN3_REG);
  Host->var.common.procCom[SocketId].capid3 = UsraCsrRead (SocketId, 0, CAPID3_PCU_FUN3_REG);
  if (HybridSystemLevelEmulationEnabled ()) {     // force capid3 to 0
    Host->var.common.procCom[SocketId].capid3 = 0;
  }
  Host->var.common.procCom[SocketId].capid4   = UsraCsrRead (SocketId, 0, CAPID4_PCU_FUN3_REG);
  Host->var.common.procCom[SocketId].capid5   = UsraCsrRead (SocketId, 0, CAPID5_PCU_FUN3_REG);
  Host->var.common.procCom[SocketId].capid6lo = UsraCsrRead (SocketId, 0, CAPID6_PCU_FUN3_REG);
  Host->var.common.procCom[SocketId].capid6hi = UsraCsrRead (SocketId, 0, CAPID7_PCU_FUN3_REG);
}

/**
  Function copies CPUID information to syshost from cpuid instruction for given socket

  @param[in] Socket     Socket number

  @retval None
**/
VOID
EFIAPI
LoadCpuId (
  IN UINT8    Socket
  )
{
  PSYSHOST            Host;
  CPUID_REGISTER_INFO CpuIdType;

  Host = GetSysHostPointer ();

  AsmCpuidEx (CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS, 0, &CpuIdType.Eax, &CpuIdType.Ebx, &CpuIdType.Ecx, &CpuIdType.Edx);
  Host->var.common.procCom[Socket].ExtCpuid7.Eax = CpuIdType.Eax;
  Host->var.common.procCom[Socket].ExtCpuid7.Ebx = CpuIdType.Ebx;
  Host->var.common.procCom[Socket].ExtCpuid7.Ecx = CpuIdType.Ecx;
  Host->var.common.procCom[Socket].ExtCpuid7.Edx = CpuIdType.Edx;
}
