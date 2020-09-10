/** @file Hardware Mem DDR IO Build target library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 Intel Corporation. <BR>

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
#include <Include/MemDdrioRegs.h>
#include <Library/BaseLib.h>
#include <Library/CteNetLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemDdrIoIpTargetLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <FnvAccessCommon.h>
#include <Library/ProcMemInitChipLib.h>

/**
  RcSim Cmd Xover Feedback build target function.

  @param[in]  PiDelay             PiDelay.
  @param[out]  DdrCrCmdNTraining  Ptr to training struct.
  @param[out]  DdrCRCmdSTraining  Ptr to training struct.
  @param[out]  DdrCRCtlTraining   Ptr to training struct.
  @param[out]  DdrCRCkeTraining   Ptr to training struct.

  @retval EFI_SUCCESS           Handled successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
RcSimCmdXoverFeedbackTarget (
  IN UINT16 PiDelay,
  OUT VOID *DdrCrCmdNTraining,
  OUT VOID *DdrCRCmdSTraining,
  OUT VOID  *DdrCRCtlTraining,
  OUT VOID  *DdrCRCkeTraining
  )
{

  return EFI_SUCCESS;

} // RcSimCmdXoverFeedbackTarget

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
  )
{

  return EFI_SUCCESS;

} // GetPclkNclkClkXoverFeedbackTarget

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
  )
{

  return EFI_SUCCESS;

} // GetClkXoverPhaseDetFeedbackTarget

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
  )
{

  return EFI_SUCCESS;

} // RcSimDataXoverFeedbackTarget