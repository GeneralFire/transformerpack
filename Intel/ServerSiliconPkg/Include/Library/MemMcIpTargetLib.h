/** @file
  Interface header file for the MemMcIpTargetLib library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 -2018 Intel Corporation. <BR>

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

#ifndef _MEM_MC_IP_TARGET_LIB_H_
#define _MEM_MC_IP_TARGET_LIB_H_

#include <Uefi.h>

/**
  Build target function to set the Fnv Ecc mode.

  @param[out] DdrtEccMode  Ptr to to the DdrtEccMode register structure.

  @retval EFI_SUCCESS     Completed successfully.
  @retval !EFI_SUCCESS    Error.
**/

EFI_STATUS
EFIAPI
SetFnvEccModeTarget (
  OUT VOID *DdrtEccMode
  );

/**
  Build Target function to poll FNV for credit ready.

  @param[in]   Host             Ptr to SysHost.
  @param[in]   Socket           Socket Number.
  @param[in]   SysBootMode      Boot mode.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
FnvPollCreditReadyTarget (
  IN PSYSHOST  Host,
  IN UINT8 Socket,
  IN BootMode SysBootMode
  );

/**
  Build Target function to poll FNV for credit ready.

  @param[in]  EnabledChannel  Channel.
  @param[out] McCreditCount   Ptr to the count.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
GetMcCreditCountEnabledTarget (
  IN UINT8 EnabledChannel,
  OUT UINT8  *McCreditCount
  );

/**
  Build Target function to set the BIOS reset Cpl.

  @param[in]   Host             Ptr to SysHost.
  @param[in]   Socket           Socket Number.
  @param[in]   Phase            Phase.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
SetBiosResetCplTarget (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Phase
  );

/**
  Build Target function to display Pcode mailbox read Mc freq error.

  @param[in]   Socket           Socket Number.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
DisplayPcodeMailboxReadMcFreqErrorTarget (
  IN UINT8 Socket
  );

/**
  Build Target function to display Pcode mailbox Set Mc freq error.

  @param[in]   Host             Ptr to SysHost.
  @param[in]   Socket           Socket Number.
  @param[in]   ReqType          Request type.
  @param[in]   ReqData          Request data.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
DisplayPcodeMailboxSetMcFreqErrorTarget (
  IN PSYSHOST  Host,
  IN UINT8 Socket,
  IN UINT8     ReqType,
  IN UINT8     ReqData
  );

/**
  Build Target function to do an Io reset delay.

  @param[in]   Host             Ptr to SysHost.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
IoResetDelayTarget (
  IN PSYSHOST Host
  );

/**
  Build Target function to wait for a Comp cycle to complete.

  @param[in]   Host             Ptr to SysHost.
  @param[in]   Socket           Socket Number.
  @param[in]   McId             Memory Controller.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
WaitForCompCycleTarget (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 McId
  );

/**
  Build Target function to wait for Cmi misc channels to complete.

  @param[in]   Host             Ptr to SysHost.
  @param[in]   Socket           Socket Number.
  @param[in]   McId             Memory Controller.
  @param[out]  Status           Ptr to the status.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
WaitForCmiMiscConfigChannelsCompleteTarget (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 McId,
  OUT UINT32 *Status
  );

/**
  Build Target function to wait for Cmi misc channels to complete.

  @param[in]   Host                   Ptr to SysHost.
  @param[in]   Socket                 Socket Number.
  @param[in]   McId                   Memory Controller.
  @param[in]   Config                 Config.
  @param[in]   McMiscCmiCreditConfig  Ptr to the Misc Cmi Credit config struct.
  @param[in]   Address                Address.
  @param[out]  Status                 Ptr to the status.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.

**/
EFI_STATUS
EFIAPI
WaitForMiscCmiCreditConfigCompleteTarget (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT8 Config,
  IN VOID *McMiscCmiCreditConfig,
  IN UINT32 Address,
  OUT UINT32 *Status
  );

/**
  Build Target function to wait for Mc Cmi Req Cpcfg to complete.

  @param[in]   Host             Ptr to SysHost.
  @param[in]   Socket           Socket Number.
  @param[in]   Channel          Channel.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
WaitForMcCmiReqCpcfgCompleteTarget (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Channel
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

/**
  Build Target function to get the Chip Ctl Clk feedback.

  @param[in]   Host               Ptr to SysHost.
  @param[in]   Socket             Socket.
  @param[in]   Channel            Channel.
  @param[in]   OriginalCmdStretch Original cmd stretch.
  @param[out]   MemTimingsAdj     Timing adjustment.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.

**/

EFI_STATUS
EFIAPI
OverrideMemTimingStretchTarget (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT32   OriginalCmdStretch,
  OUT VOID    *MemTimingsAdj
  );

#endif  // _MEM_MC_IP_TARGET_LIB_H_
