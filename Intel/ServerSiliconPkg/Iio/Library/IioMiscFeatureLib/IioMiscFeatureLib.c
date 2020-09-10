/** @file
  This file initialize the IIO Misc Feature

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <Library/IioDataHubLib.h>
#include <Library/IioAccessLib.h>
#include <RcRegs.h>
#include <Library/IioSysInfoLib.h>
#include <IioSetupDefinitions.h>

#include <LibraryPrivate/IioDebug.h>

/**

    Iio Misc Featues Init before link training.

    @param IioGlobalData  - Pointer to IIO_GLOBALS
    @param IioIndex       - Index to CPU/IIO

    @retval None

**/
VOID
IioMiscFeatureEarlyInit (
  IN  IIO_GLOBALS                                    *IioGlobalData,
  IN  UINT8                                          IioIndex
  )
{
  return;
}

/**
    Enable Misc Featues by writing to IIOMISCCTRL

    @param IioGlobalData  - Pointer to IIO_GLOBALS
    @param IioIndex       - Index to CPU/IIO

    @retval None
**/
VOID
IioMiscFeatureInit (
  IN  IIO_GLOBALS                                    *IioGlobalData,
  IN  UINT8                                          IioIndex
  )
{
  UINT8                               Stack;
  TOCM_IIO_VTD_STRUCT                 IioTocm;
  IIOMISCCTRL_N0_IIO_VTD_STRUCT       IioMiscCtrl;
  TSWCTL3_IIO_DFX_VTD_STRUCT          TswCtl3;
  UINT8                               MaxStackSocket;

  MaxStackSocket = GetMaxStackNumPerSocket ();
  for (Stack = 0; Stack < MaxStackSocket; Stack++) {
    if (!(IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex] & (1 << Stack))) {
      continue;
    }

    //
    //Setup the EN1K feature of IIOMISCCTRL (Register offset 0x1C0, bit 2 )
    //
    IioMiscCtrl.Data = IioReadCpuCsr32 (IioIndex, Stack, IIOMISCCTRL_N0_IIO_VTD_REG);
    IioMiscCtrl.Bits.en1k = 0;
    if (IioGlobalData->SetupData.EN1K) {
      IioMiscCtrl.Bits.en1k = 1;
    }

    //
    // Remote VGA enable : default for non-legacy sockets
    //
    if ((IioGlobalData->SetupData.LegacyVgaSoc == IioIndex) &&
        (IioGlobalData->SetupData.LegacyVgaStack == Stack)) {
      IioMiscCtrl.Bits.rvgaen = 1;
    }

    // NOTE: We are asking user to explicitly indicate on which socket its VGA card
    // would be connected for the IIO apart from other configuration settings to
    // select onboard video device or detect the VGA device from the PCIe port.....
    IioTocm.Data = IioReadCpuCsr8 (IioIndex, Stack, TOCM_IIO_VTD_REG);
    IioTocm.Bits.tocmvalid = 1;
    IioWriteCpuCsr8 (IioIndex, Stack, TOCM_IIO_VTD_REG, IioTocm.Data);
    IioWriteCpuCsr32 (IioIndex, Stack, IIOMISCCTRL_N0_IIO_VTD_REG, IioMiscCtrl.Data);

    if (IioGlobalData->SetupData.DualCvIoFlow) {
      TswCtl3.Data = IioReadCpuCsr32 (IioIndex,Stack,TSWCTL3_IIO_DFX_VTD_REG);
      TswCtl3.Bits.enable_irp_intlv_iou2 = 0;
      TswCtl3.Bits.enable_irp_intlv_cbdma = 0;
      IioWriteCpuCsr32 (IioIndex, Stack, TSWCTL3_IIO_DFX_VTD_REG, TswCtl3.Data);
    }
  }// End of for (Stack = 0; Stack < MAX_IIO_STACK; Stack++)

}

/**

    Peer 2 peer memory config

    @param IioGlobalData - Pointer to IIO_GLOBALS
    @param IioIndex      - Current socket.
    @param PortIndex     - Port number being initialized.

    @retval None

**/
VOID
Peer2peerMemoryConfig (
  IN  IIO_GLOBALS    *IioGlobalData,
  IN  UINT8          IioIndex,
  IN  UINT8          PortIndex
  )
{
  UINT8                             StackIndex;
  UINT8                             ChannelIndex;
  UINT32                            MiscCtrlStsRegAddr;
  MISCCTRLSTS_CH0_IIO_VTD_STRUCT    MiscCtrlStsCh;

  StackIndex = IioGlobalData->IioVar.IioVData.StackPerPort[IioIndex][PortIndex];
  ChannelIndex = GetMs2iosfChannelIndex(PortIndex);
  MiscCtrlStsRegAddr = MISCCTRLSTS_CH0_IIO_VTD_REG + ChannelIndex*8;

  MiscCtrlStsCh.Data = IioReadCpuCsr32 (IioIndex, StackIndex, MiscCtrlStsRegAddr);
  MiscCtrlStsCh.Bits.problematic_channel_for_lock_flows =1;
  if (IioGlobalData->SetupData.ProblematicPort == 0x01) {
    //
    // If NP_PROBLEMATIC flow,  Disable P2P memory reads
    //
    MiscCtrlStsCh.Bits.peer2peer_memory_read_disable = 1;
  } else {
    //
    // P_PROBLEMATIC flow, Disable P2P memory writes
    MiscCtrlStsCh.Bits.peer2peer_memory_write_disable = 1;
  }
  IioWriteCpuCsr32 (IioIndex, StackIndex, MiscCtrlStsRegAddr, MiscCtrlStsCh.Data);
}

/**

  MS2IOSF channel MISCCTRLSTS and and PERFCTRLSTS register initial.

  @param Phase         - Initialization phase (pre, init, post).
  @param IioGlobalData - Pointer to IIO global data/methods
  @param IioIndex      - Current socket.
  @param StackIndex    - Stack number being initialized.

**/
VOID
Ms2iosfChannelMiscInit (
  IN  IIO_INIT_PHASE                 Phase,
  IN  IIO_GLOBALS                    *IioGlobalData,
  IN  UINT8                          IioIndex,
  IN  UINT8                          StackIndex
  )
{
  UINT8     MaxPortNumberPerSocket;
  UINT8     PortIndex = 0;
  UINT8     ChannelIndex;
  UINT32    PerfCtrlStsRegAddr;
  UINT32    MiscCtrlStsRegAddr;
  UINT8     *Channels;
  PERFCTRLSTS_CH0_IIO_VTD_STRUCT    PerfCtrlStsCh;
  MISCCTRLSTS_CH0_IIO_VTD_STRUCT    MiscCtrlStsCh;

  IIO_D_DBG ("Ms2iosfChannelMiscInit : Entry\n");
  PortIndex = GetPortIndexbyStack (StackIndex, PORT_0_INDEX);
  Channels = GetMs2iosfChannels (StackIndex);
  if (Channels == NULL) {
    return;
  }

  //
  // Program all channels connected in the stack
  //
  for (ChannelIndex = 0; Channels[ChannelIndex] < LastChannel; ChannelIndex++) {

    IIO_D_DBG ("\tIioIndex = %d\tPortIndex = %d\tStackIndex = %d\tChannel = %d\n", IioIndex, PortIndex, StackIndex, Channels[ChannelIndex]);

    PerfCtrlStsRegAddr = PERFCTRLSTS_CH0_IIO_VTD_REG + Channels[ChannelIndex]*8;
    PerfCtrlStsCh.Data = IioReadCpuCsr32 (IioIndex, StackIndex, PerfCtrlStsRegAddr);
    MiscCtrlStsRegAddr = MISCCTRLSTS_CH0_IIO_VTD_REG + Channels[ChannelIndex]*8;
    MiscCtrlStsCh.Data = IioReadCpuCsr32 (IioIndex, StackIndex, MiscCtrlStsRegAddr);
    switch (Phase) {
    case IIOInitPhase1:
      if (IioGlobalData->SetupData.EOI[PortIndex]) {
        MiscCtrlStsCh.Bits.eoifd = 1;
      } else {
        MiscCtrlStsCh.Bits.eoifd = 0;
      }

      if (IioGlobalData->SetupData.P2PRdDis[PortIndex]) {
        MiscCtrlStsCh.Bits.peer2peer_memory_read_disable = 1;
      } else {
        MiscCtrlStsCh.Bits.peer2peer_memory_read_disable = 0;
      }
      if (IsVc1OrVcm (IioIndex, StackIndex, ChannelIndex, TRUE, TRUE)) {
        MiscCtrlStsCh.Bits.do_not_block_during_lock = 1;
      }
      break;

    case IIOInitPhase2:
      break;

    case IIOInitPhase3:
      //
      // Relax Ordering override programming
      //
      if (IioGlobalData->SetupData.PcieRelaxedOrdering) {
        PerfCtrlStsCh.Bits.disable_ro = 0;
      } else {
        PerfCtrlStsCh.Bits.disable_ro = 1;
      }

      if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort)) {
        if (IioGlobalData->SetupData.DmiAllocatingFlow) {
          PerfCtrlStsCh.Bits.use_allocating_flow_wr = 1;
        } else {
          PerfCtrlStsCh.Bits.use_allocating_flow_wr = 0;
        }
      } else {
        if (IioGlobalData->SetupData.PcieAllocatingFlow) {
          PerfCtrlStsCh.Bits.use_allocating_flow_wr = 1;
        } else {
          PerfCtrlStsCh.Bits.use_allocating_flow_wr = 0;
        }
      }

      PerfCtrlStsCh.Bits.nosnoopopwren = 1;
      if (IioGlobalData->SetupData.NoSnoopWrCfg == IIO_OPTION_DISABLE &&
          !IsVc1OrVcm (IioIndex, StackIndex, ChannelIndex, TRUE, TRUE)) {
        PerfCtrlStsCh.Bits.nosnoopopwren = 0;
      }

      PerfCtrlStsCh.Bits.partial_cohrd_op = IioGlobalData->SetupData.CoherentReadPart;

      //
      // enable/disable TPH
      //
      if (IsPciePortTPHCapable (IioIndex, PortIndex) == TRUE) {
        PerfCtrlStsCh.Bits.disable_tph = IioGlobalData->SetupData.DisableTPH;
        if (IioGlobalData->SetupData.DisableTPH == FALSE) {
          PerfCtrlStsCh.Bits.prioritize_tph = IioGlobalData->SetupData.PrioritizeTPH;
        }
      }
      MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];
      //
      // Check IODC
      //
      if ((IioGlobalData->SetupData.IODC[((IioIndex) * MaxPortNumberPerSocket) + PortIndex]) == IODC_GLOBAL_KTI_OPTION) {
        //
        // Apply KTI global option
        //
        switch (IioGlobalData->IioVar.IioVData.IODC) {
        case IODC_AUTO:
        case IODC_EN_REM_INVITOM_AND_WCILF:
          PerfCtrlStsCh.Bits.implicit_tph = 1;
          PerfCtrlStsCh.Bits.implicit_nosnoop = 0;
          break;
        case IODC_EN_REM_INVITOM_PUSH:
          PerfCtrlStsCh.Bits.implicit_tph = 1;
          PerfCtrlStsCh.Bits.implicit_nosnoop = 0;
          break;
        case IODC_EN_REM_INVITOM_ALLOCFLOW:
          PerfCtrlStsCh.Bits.implicit_tph = 0;
          PerfCtrlStsCh.Bits.implicit_nosnoop = 0;
          break;
        case IODC_EN_REM_INVITOM_ALLOC_NONALLOC:
          PerfCtrlStsCh.Bits.implicit_tph = 0;
          PerfCtrlStsCh.Bits.implicit_nosnoop = 1;
          break;
        case IODC_DISABLE:
        default:
          break;
        }
      } else {
        switch (IioGlobalData->SetupData.IODC[((IioIndex) * MaxPortNumberPerSocket) + PortIndex]) {
        //
        // Apply particular port configuration
        //
        case IODC_AUTO:
        case IODC_EN_REM_INVITOM_AND_WCILF:
          PerfCtrlStsCh.Bits.implicit_tph = 1;
          PerfCtrlStsCh.Bits.implicit_nosnoop = 0;
          break;
        case IODC_EN_REM_INVITOM_PUSH:
          PerfCtrlStsCh.Bits.implicit_tph = 1;
          PerfCtrlStsCh.Bits.implicit_nosnoop = 0;
          break;
        case IODC_EN_REM_INVITOM_ALLOCFLOW:
          PerfCtrlStsCh.Bits.implicit_tph = 0;
          PerfCtrlStsCh.Bits.implicit_nosnoop = 0;
          break;
        case IODC_EN_REM_INVITOM_ALLOC_NONALLOC:
          PerfCtrlStsCh.Bits.implicit_tph = 0;
          PerfCtrlStsCh.Bits.implicit_nosnoop = 1;
          break;
        case IODC_DISABLE:
        default:
          break;
        }
      }

      if (IioGlobalData->SetupData.MctpEn[(IioIndex * MaxPortNumberPerSocket) + PortIndex]) {
        MiscCtrlStsCh.Bits.disable_mctp_broadcast_to_this_channel = 0;
      } else {
        MiscCtrlStsCh.Bits.disable_mctp_broadcast_to_this_channel = 1;
      }
      break;

    default:
      break;
    }
    IioWriteCpuCsr32 (IioIndex, StackIndex, PerfCtrlStsRegAddr, PerfCtrlStsCh.Data);
    PerfCtrlStsCh.Data = IioReadCpuCsr32 (IioIndex, StackIndex, PerfCtrlStsRegAddr);
    IIO_D_DBG ("\tPERFCTRLSTS_CH%d[0x%08x]: 0x%08x\n", Channels[ChannelIndex], PerfCtrlStsRegAddr, PerfCtrlStsCh.Data);
    IioWriteCpuCsr32 (IioIndex, StackIndex, MiscCtrlStsRegAddr, MiscCtrlStsCh.Data);
    MiscCtrlStsCh.Data = IioReadCpuCsr32 (IioIndex, StackIndex, MiscCtrlStsRegAddr);
    IIO_D_DBG ("\tMISCCTRLSTS_CH%d[0x%08x]: 0x%08x\n", Channels[ChannelIndex], MiscCtrlStsRegAddr, MiscCtrlStsCh.Data);
  }
  IIO_D_DBG ("Ms2iosfChannelMiscInit : Exit\n");
}

