/** @file Hardware Mem DDR IO Build target library

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

#include <Include/MemDdrioRegs.h>
#include <Library/BaseLib.h>
#include <Library/CteNetLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemDdrIoIpTargetLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <FnvAccessCommon.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemMcIpLib.h>

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
  )
{
  return EFI_SUCCESS;
} // StartingCCCTarget

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
  )
{

  return EFI_SUCCESS;

} // ScadExitTarget

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
  )
{

  if ((SysBootMode == NormalBoot) && (Host->var.mem.subBootMode != WarmBootFast)) {

    FnvDdrtIoInit (Host, Socket, PLL_LOCK);
    WaitForMailboxReady (Host, Socket);

  }

  return EFI_SUCCESS;

} // FnvDdrtIoInitPllTarget

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
  )
{

  if ((SysBootMode == NormalBoot) && (Host->var.mem.subBootMode != WarmBootFast)) {
    FnvDdrtIoInit (Host, Socket, DDRT_FREQ);
  }

  return EFI_SUCCESS;

} // FnvDdrtIoInitDdrtFreqTarget

/**
  Handle WR leveling clear build target function.

  @param[in]  Host              Ptr to SysHost.
  @param[in]  Socket            Socket.
  @param[in]  Channel           Channel.
  @param[in]  Strobe            Strobe.
  @param[in]  DataControl0      Register structure.

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
  )
{

  return EFI_SUCCESS;

} // HandleWrLevelingClearModeTarget

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
  )
{

  return EFI_SUCCESS;

} // SetFeedbackCmdVrefTarget