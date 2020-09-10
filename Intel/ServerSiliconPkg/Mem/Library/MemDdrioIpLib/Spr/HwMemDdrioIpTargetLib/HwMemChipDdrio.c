/** @file Hardware Mem DDR IO Build target library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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
#include <Include/MemIoControl.h>
#include <Library/BaseLib.h>
#include <Library/MemDdrIoIpTargetLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemRoutingLib.h>
#include <Library/Ddr5CoreLib.h>

/**
  Build target function to get the preamble.

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
  )
{
  //
  // longpreambleenable is removed in SPR. It's changed to use Preamble pattern.
  //
  *Preamble = 0;//SPR_TODO.
  return EFI_SUCCESS;

} // GetLongPreambleEnableTarget

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
  )
{
  //
  // No adjustment needed
  //

  return EFI_SUCCESS;

} // AdjustSenseAmpFeedbackTarget

/**
  Sense Amp offset delay build target function.

  @param[in]  Host              Ptr to SysHost.

  @retval EFI_SUCCESS           Delayed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
SenseAmpOffsetDelayTarget (
  IN PSYSHOST  Host
  )
{

  FixedDelayMicroSecond (1);

  return EFI_SUCCESS;

} // SenseAmpOffsetDelayTarget

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
  IN PSYSHOST  Host,
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Strobe
  )
{

  return EFI_SUCCESS;

} // WriteDdr5DqCountingWindowValueTarget

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
  )
{

  //
  // Implemented in WriteDdr5CaTrainingMode
  //
  return EFI_UNSUPPORTED;

} // WriteDdr5CaTrainingModeTarget

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
  )
{

  UINT8 SubCh;
  DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_STRUCT DdrccTrainCntResult1;

  for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {
    DdrccTrainCntResult1.Data = ReadDdrioCcRegister (Host, Socket, Channel,  SubCh, DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_REG);
    DdrccTrainCntResult1.Bits.cmd_train_ctr_en = 1;
    WriteDdrioCcRegister (Host, Socket, Channel,  SubCh, DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_REG, DdrccTrainCntResult1.Data);
  }
  return EFI_SUCCESS;

} // UpdateDdr5IntfCmdTrainCtrlTarget

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
  )
{
  DDRD_N0_TRAIN_CNT_RESULT_MCIO_DDRIO_STRUCT   DdrddTrainCntResult;
  DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_STRUCT         DdrdN0TrainCtl1;

  while (TRUE) {
    DdrdN0TrainCtl1.Data = MemReadPciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_REG));
    if (GetEmulation () & SIMICS_FLAG) {
      break;
    }
    if (DdrdN0TrainCtl1.Bits.cmd_train_ctr_en == 0) {
      break;
    }
    FixedDelayMicroSecond (1);
  }
  //
  // Read the training result.
  //
  DdrddTrainCntResult.Data = MemReadPciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_TRAIN_CNT_RESULT_MCIO_DDRIO_REG));

  *Feedback = DdrddTrainCntResult.Bits.train_cnt;

  return EFI_SUCCESS;

} // GetDdr5DataTrainFeedbackTarget

/**
  Get DDR5 Intf Cmd train build target function.

  @param[in]  Host              Ptr to SysHost.
  @param[in]  Socket            Socket.
  @param[in]  Channel           Channel.
  @param[in]  SubCh             Sub Channel.
  @param[in]  Strobe            Strobe.
  @param[out] Feedback          Feedback.

  @retval EFI_SUCCESS           Wrote successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
GetDdr5IntfCmdTrainCtrlFeedbackTarget (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 SubCh,
  IN UINT8 Strobe,
  IN UINT8 Dimm,
  OUT UINT32 *Feedback
  )
{
  DDRCC_TRAIN_CNT_RESULT_0_MCIO_DDRIOEXT_STRUCT DdrccTrainCntResult0;
  DDRCC_TRAIN_CNT_RESULT_1_MCIO_DDRIOEXT_STRUCT DdrccTrainCntResult1;
  DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_STRUCT         DdrccTrainCtl1;

  while (TRUE) {
    DdrccTrainCtl1.Data = ReadDdrioCcRegister (Host, Socket, Channel, SubCh, DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_REG);
    if (GetEmulation () & SIMICS_FLAG) {
      break;
    }
    if (DdrccTrainCtl1.Bits.cmd_train_ctr_en == 0) {
      break;
    }
    FixedDelayMicroSecond (1);
  }

  //
  // Read the training result.
  //
  if (SubCh == SUB_CH_A) {
    if (Dimm == 0) {
      DdrccTrainCntResult0.Data = ReadDdrioCcRegister (Host, Socket, Channel, SubCh, DDRCC_TRAIN_CNT_RESULT_0_MCIO_DDRIOEXT_REG);
      *Feedback = DdrccTrainCntResult0.Bits.train_cnt;
    } else {
      DdrccTrainCntResult1.Data = ReadDdrioCcRegister (Host, Socket, Channel, SubCh, DDRCC_TRAIN_CNT_RESULT_1_MCIO_DDRIOEXT_REG);
      *Feedback = DdrccTrainCntResult1.Bits.train_cnt;
    }
  } else if (SubCh == SUB_CH_B) {
    if (Dimm == 0) {
      DdrccTrainCntResult0.Data = ReadDdrioCcRegister (Host, Socket, Channel, SubCh, DDRCC_TRAIN_CNT_RESULT_0_MCIO_DDRIOEXT_REG);
      *Feedback = DdrccTrainCntResult0.Bits.train_cnt;
    } else {
      DdrccTrainCntResult1.Data = ReadDdrioCcRegister (Host, Socket, Channel, SubCh, DDRCC_TRAIN_CNT_RESULT_1_MCIO_DDRIOEXT_REG);
      *Feedback = DdrccTrainCntResult1.Bits.train_cnt;
    }
  }
  return EFI_SUCCESS;
} // GetDdr5IntfCmdTrainCtrlFeedbackTarget

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
  )
{
  return EFI_SUCCESS;
} // PollBacksideTrainDoneTarget

/**
  Poll until DDRIO indicates RCOMP cycle is complete.

  @param[in]  Host              Pointer to SysHost
  @param[in]  Socket            Socket number
  @param[in]  McId              Memory Controller ID

  @retval EFI_SUCCESS           RCOMP cycle completed
  @retval !EFI_SUCCESS          Error.
**/
EFI_STATUS
EFIAPI
WaitForDdrioCompCycleTarget (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    McId
  )
{
  DDRINTF_CTL0_MCIO_DDRIO_STRUCT    DdrintfCtl0;

  while (TRUE) {
    DdrintfCtl0.Data = MemReadPciCfgMC (Socket, McId, DDRINTF_CTL0_MCIO_DDRIO_REG);
    if (GetEmulation () & SIMICS_FLAG) {
      break;
    }
    if (DdrintfCtl0.Bits.force_comp_begin != 1) {
      break;
    }
    FixedDelayMicroSecond (1);
  }

  return EFI_SUCCESS;
}

