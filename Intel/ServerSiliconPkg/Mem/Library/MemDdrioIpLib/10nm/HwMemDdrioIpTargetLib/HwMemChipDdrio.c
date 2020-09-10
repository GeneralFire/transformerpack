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
#include <Library/MemDdrIoIpTargetLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/IoAccessLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRoutingLib.h>
#include <Library/CsrAccessLib.h>
#include <Include/MemIoControl.h>
#include <Library/MemRcLib.h>


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
  DATACONTROL0N0_0_MCIO_DDRIO_STRUCT      DataControl0;
  struct channelNvram                     (*ChannelNvList)[MAX_CH];
#ifdef DDR5_SUPPORT
  if (IsBrsPresent (Host, Socket)) {
    DataControl0.Data = ReadDdrioDataRegCh (Host, Socket, Channel, 0, DATACONTROL0N0_0_MCIO_DDRIO_REG); //strobe 0
  } else
#endif //DDR5_SUPPORT
  {
    ChannelNvList = GetChannelNvList (Host, Socket);
    DataControl0.Data = (*ChannelNvList)[Channel].dataControl0;
  }
  *Preamble = (UINT8)DataControl0.Bits.longpreambleenable;

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

#ifdef DDR5_SUPPORT
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
  UINT8                                 SubChannel = 0;
  UINT8                                 ChannelLocal = 0;
  UINT32                                Value = 0;
  MRC_TT                                CurrentTestType;
  DATACONTROL2N0_0_MCIO_DDRIO_STRUCT    dataControl2;

  CurrentTestType = GetCurrentTestType (Socket);
  if (CurrentTestType == RcvenFineTest) {
    Value = 2;
  } else {
    Value = DQ_COUNTING_WINDOW_VALUE/2;
  }

  for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
    ChannelLocal = Channel;
    GetChannelMapping (Channel, SubChannel, &ChannelLocal);
    WriteDdrioDataRegCh (Host, Socket, ChannelLocal, Strobe, DDR5DATATRAINCNTR_MCIO_DDRIO_REG, Value); // DDR5_TODO. Check register name.
    //
    // Enable forcerxon.
    //
    dataControl2.Data = ReadDdrioDataRegCh (Host, Socket, ChannelLocal, Strobe, DATACONTROL2N0_0_MCIO_DDRIO_REG);
    dataControl2.Bits.forcerxon = 1;
    WriteDdrioDataRegCh (Host, Socket, ChannelLocal, Strobe, DATACONTROL2N0_0_MCIO_DDRIO_REG, dataControl2.Data);
  }
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

  UINT8 SubChannel = 0;
  UINT8 ChannelLocal = 0;
  DDR5DATATRAINCTRL_MCIO_DDRIO_STRUCT DataTrainCtrl;

  for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
    ChannelLocal = Channel;
    GetChannelMapping (Channel, SubChannel, &ChannelLocal);
    DataTrainCtrl.Data = ReadDdrioDataRegCh (Host, Socket, ChannelLocal, Strobe, DDR5DATATRAINCTRL_MCIO_DDRIO_REG);
    DataTrainCtrl.Bits.cacsbacksidetrainen = 1;
    WriteDdrioDataRegCh (Host, Socket, ChannelLocal, Strobe, DDR5DATATRAINCTRL_MCIO_DDRIO_REG, DataTrainCtrl.Data);
  }

  return EFI_SUCCESS;

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

  DDR5INTFCMDTRAINCTRL_CH0_MCIO_DDRIO_STRUCT DataTrainCtrl;
  DataTrainCtrl.Data = MemReadPciCfgEp (
                         Socket,
                         Channel,
                         DDR5INTFCMDTRAINCTRL_CH1_MCIO_DDRIO_REG
                         );

  DataTrainCtrl.Bits.cacstrainingmode = 1;

  MemWritePciCfgEp (
    Socket,
    Channel,
    DDR5INTFCMDTRAINCTRL_CH1_MCIO_DDRIO_REG,
    DataTrainCtrl.Data
    );

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
  UINT32 DataTrainFeedback;
  UINT8  ChannelLocal;


  GetChannelMapping (Channel, SubChannel, &ChannelLocal);
  //
  // Read the training result.
  // Use DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG to get the train result for BRS.
  //
  DataTrainFeedback = ReadDdrioDataRegCh (Host, Socket, ChannelLocal, Strobe, DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG) & 0x1FF;

  *Feedback = DataTrainFeedback;


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
  DDR5INTFCMDTRAINFB_CH0_MCIO_DDRIO_STRUCT   Ddr5IntfCmdTrainFb;
  DDR5INTFCMDTRAINCTRL_CH0_MCIO_DDRIO_STRUCT DataTrainCtrl;
  RcDebugPrintWithDevice (SDBG_TRACE, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "GetDdr5IntfCmdTrainCtrlFeedbackTarget starts\n");

  do {
    //
    // Training mode also only uses channel 1.
    //
    DataTrainCtrl.Data = MemReadPciCfgEp (Socket, Channel, DDR5INTFCMDTRAINCTRL_CH1_MCIO_DDRIO_REG);

  } while (DataTrainCtrl.Bits.cacstrainingmode != 0);
  //
  // Read the training result.
  //
  Ddr5IntfCmdTrainFb.Data = MemReadPciCfgEp (Socket, Channel, DDR5INTFCMDTRAINFB_CH1_MCIO_DDRIO_REG);
  *Feedback = Ddr5IntfCmdTrainFb.Bits.trainfeedback;

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
  DDR5DATATRAINCTRL_MCIO_DDRIO_STRUCT DataTrainCtrl;
  UINT8  ChannelLocal = Channel;

  GetChannelMapping (Channel, SubChannel, &ChannelLocal);

  RcDebugPrintWithDevice (SDBG_TRACE, Socket, Channel, NO_DIMM, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
    "Poll the strobe %x to be done\n", Strobe);


  do {
    DataTrainCtrl.Data = ReadDdrioDataRegCh (Host, Socket, ChannelLocal, Strobe, DDR5DATATRAINCTRL_MCIO_DDRIO_REG);
    RcDebugPrintWithDevice (SDBG_TRACE, Socket, Channel, NO_DIMM, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
      "Poll the strobe %x to be done\n", Strobe);

  } while (DataTrainCtrl.Bits.cacsbacksidetrainen != 0);


  return EFI_SUCCESS;

} // PollBacksideTrainDoneTarget
#endif // DDR5_SUPPORT
