/** @file
  Interface header file for the Mem DDR IO Target library class.

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

#ifndef _MEM_DDRIO_IP_TARGET_LIB_H_
#define _MEM_DDRIO_IP_TARGET_LIB_H_

#include <Base.h>
#include <Library/MemoryCoreLib.h>

//
// ICX_TODO: This API should not expose private register definitions
//
#include <Include/MemDdrioRegs.h>

/**
  Build Target function to get the preamble.

  @param[in]   Host             Ptr to SysHost.
  @param[in]   Socket           Socket Number.
  @param[in]   Channel          Channel.
  @param[out]  Preamble         Preamble value.

  @retval EFI_SUCCESS           Got preamble successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
GetLongPreambleEnableTarget (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  OUT UINT8     *Preamble
  );

/**
  Polling the training done.

  @param[in]  Host              Ptr to SysHost.
  @param[in]  Socket            Socket.
  @param[in]  Channel           Channel.
  @param[in]  SubChannel        SubChannel.
  @param[in]  Strobe            Strobe.

  @retval EFI_SUCCESS           Wrote successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
PollBacksideTrainDoneTarget (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 SubChannel,
  IN UINT8 Strobe
  );

/**
  Adjust CMD Summer feedback build target function.

  @param[in]  SampOffset        A pointer to the target data.
  @param[in]  SubCh             Sub Channel.
  @param[in]  Channel           Channel.
  @param[out] Feedback          Feedback.

  @retval EFI_SUCCESS           Feedback adjusted successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
AdjustCmdSummerOffsetCalibrationFeedbackTarget (
  IN INT16  SampOffset,
  IN UINT8  SubCh,
  IN UINT8  Channel,
  OUT INT32 *Feedback
  );

/**
  Adjust Sense Amp feedback build target function.

  @param[in]  SampOffset        A pointer to the target data.
  @param[in]  Strobe            Strobe.
  @param[in]  Channel           Channel.
  @param[out] Feedback          Feedback.

  @retval EFI_SUCCESS           Feedback adjusted successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
AdjustSenseAmpFeedbackTarget (
  IN INT16 SampOffset,
  IN UINT8 Strobe,
  IN UINT8 Channel,
  OUT INT16 *Feedback
  );

/**
  Sense Amp offset delay abstraction function.

  @param[in]  Host              Ptr to SysHost.

  @retval EFI_SUCCESS           Delayed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
SenseAmpOffsetDelayTarget (
  IN PSYSHOST  Host
  );

/**
  DDR5 counting window write build target function.

  @param[in]  Host              Ptr to SysHost.
  @param[in]  Socket            Socket.
  @param[in]  Channel           Channel.
  @param[in]  Strobe            Strobe.

  @retval EFI_SUCCESS           Wrote successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
WriteDdr5DqCountingWindowValueTarget (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Strobe
  );

/**
  DDR5 training mode write build target function.

  @param[in]  Host              Ptr to SysHost.
  @param[in]  Socket            Socket.
  @param[in]  Channel           Channel.
  @param[in]  Strobe            Strobe.

  @retval EFI_SUCCESS           Wrote successfully.
  @retval EFI_UNSUPPORTED       Function not implemented
**/

EFI_STATUS
EFIAPI
WriteDdr5CaTrainingModeTarget (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Strobe
  );

/**
  Update DDR5 Intf Cmd build target function.

  @param[in]  Host              Ptr to SysHost.
  @param[in]  Socket            Socket.
  @param[in]  Channel           Channel.
  @param[in]  Strobe            Strobe.

  @retval EFI_SUCCESS           Wrote successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
UpdateDdr5IntfCmdTrainCtrlTarget (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Strobe
  );

/**
  Get DDR5 data train build target function.

  @param[in]  Host              Ptr to SysHost.
  @param[in]  Socket            Socket.
  @param[in]  Channel           Channel.
  @param[in]  Strobe            Strobe.
  @param[out] Feedback          Feedback.

  @retval EFI_SUCCESS           Wrote successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
GetDdr5DataTrainFeedbackTarget (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 SubChannel,
  IN UINT8 Strobe,
  OUT UINT32 *Feedback
  );

/**
  Starting CCC build target function.

  @param[in]  Host              Ptr to SysHost.
  @param[in]  Socket            Socket.

  @retval EFI_SUCCESS           Started successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
StartingCCCTarget (
  IN PSYSHOST  Host,
  IN UINT8 Socket
  );

/**
  Scad exit build target function.

  @param[in]  Host              Ptr to SysHost.
  @param[in]  Socket            Socket.

  @retval EFI_SUCCESS           Handled successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
ScadExitTarget (
  IN PSYSHOST    Host,
  IN UINT8       Socket
  );

/**
  Hvm exit build target function.

  @param[in]  Host              Ptr to SysHost.
  @param[in]  Socket            Socket.

  @retval EFI_SUCCESS           Handled successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
HvmExitTarget (
  IN PSYSHOST    Host,
  IN UINT8       Socket
  );

/**
  Fnv Ddrt Init PLL build target function.

  @param[in]  Host              Ptr to SysHost.
  @param[in]  Socket            Socket.
  @param[in]  SysBootMode       Boot Mode.

  @retval EFI_SUCCESS           Handled successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
FnvDdrtIoInitPllTarget (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN BootMode  SysBootMode
  );

/**
  Fnv Ddrt Init Freq build target function.

  @param[in]  Host              Ptr to SysHost.
  @param[in]  Socket            Socket.
  @param[in]  SysBootMode       Boot Mode.

  @retval EFI_SUCCESS           Handled successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
FnvDdrtIoInitDdrtFreqTarget (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN BootMode  SysBootMode
  );

/**
  Fnv Tx Vref build target function.

  @param[in]  Host              Ptr to SysHost.
  @param[in]  Socket            Socket.
  @param[in]  SysBootMode       Boot Mode.

  @retval EFI_SUCCESS           Handled successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
CacheTxVrefFnvTarget (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN BootMode  SysBootMode
  );

/**
  Handle WR leveling clear build target function.

  @param[in]  Host              Ptr to SysHost.
  @param[in]  Socket            Socket.
  @param[in]  Channel           Channel.
  @param[in]  Strobe            Strobe.
  @param[in]  DataControl0      Ptr to register structure.

  @retval EFI_SUCCESS           Handled successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
HandleWrLevelingClearModeTarget (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Strobe,
  IN VOID *DataControl0
  );

/**
  Build target function to set the feedback Cmd Vref.

  @param[in]   Host             Ptr to SysHost.
  @param[in]   Socket           Socket Number.
  @param[in]   SysBootMode      Boot mode.

  @retval EFI_SUCCESS     Completed successfully.
  @retval !EFI_SUCCESS    Error.
**/

EFI_STATUS
EFIAPI
SetFeedbackCmdVrefTarget (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN BootMode SysBootMode
  );

/**
  RcSim Clk Xover Feedback build target function.

  @param[in]  PiDelay             PiDelay.
  @param[out]  DdrCRClkTraining   Ptr to training struct.

  @retval EFI_SUCCESS           Handled successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
GetPclkNclkClkXoverFeedbackTarget (
  IN UINT16 PiDelay,
  OUT VOID *DdrCRClkTraining
  );

/**
  RcSim Clk Xover Feedback build target function.

  @param[in]  PiDelay             PiDelay.
  @param[out]  DdrCRClkTraining   Ptr to training struct.

  @retval EFI_SUCCESS           Handled successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
GetClkXoverPhaseDetFeedbackTarget (
  IN UINT16 PiDelay,
  OUT VOID *DdrCRClkTraining
  );

/**
  RcSim Data Xover Feedback build target function.

  @param[in]  PiDelay           PiDelay.
  @param[in]  Strobe            Strobe.
  @param[out] Feedback          Ptr to Feedback.

  @retval EFI_SUCCESS           Handled successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
RcSimDataXoverFeedbackTarget (
  IN UINT16 PiDelay,
  IN UINT8  Strobe,
  OUT UINT32 *Feedback
  );

/**
  Poll until DDRIO indicates RCOMP cycle is complete.

  @param[in]  Host              Pointer to SysHost
  @param[in]  Socket            Socket number
  @param[in]  McId              Memory Controller ID

  @retval EFI_SUCCESS           RCOMP cycle completed
  @retval !EFI_SUCCESS          Error
**/
EFI_STATUS
EFIAPI
WaitForDdrioCompCycleTarget (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    McId
  );

#endif  // _MEM_DDRIO_IP_TARGET_LIB_H_
