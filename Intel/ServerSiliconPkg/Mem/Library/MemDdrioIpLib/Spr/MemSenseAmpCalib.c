/** @file
  DDR Phy Sense Amp Calibration Training

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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
#include <SysHostChip.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemDdrIoIpTargetLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/CheckpointLib.h>
#include <Library/CteNetLib.h>
#include <Library/CsrAccessLib.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Include/MemDdrioSpecific.h>
#include <Library/TimerLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemFmcIpLib.h>

#include "Include/MemIoControl.h"
#include "Include/MemDdrioRegs.h"
#include "Include/MemDdrioPrivate.h"
#include "Include/MemCmdControl.h"

//
// Set sample count to 16
// DDRD_N0_HW_TRAIN_CTL0.sample_cnt encoding:
//   0: 16
//   1: 32
//   2: 64
//   3: 2 (Should only used to speed up digitial simulation)
//
#define SENSE_AMP_SAMPLE_COUNT_HW_FSM       0
#define SENSE_AMP_SAMPLE_THRESHOLD_HW_FSM   8
#define SENSE_AMP_SETTLING_TIME_HW_FSM      0
#define SENSE_AMP_TIME_OUT                  100000000 //ns
#define SENSE_AMP_MAX_OFFSET                0x3F

#define EVEN_ODD_SAMPLER 2

/**
  Initialization for Sense Amp Calibration
  Used by both HW FSM and SW based training methods

  @param[in]  Host    - Pointer to SysHost
  @param[in]  Socket  - Socket number
  @param[out] SettingStore  - Pointer to temporary storage for settings modified
                              during this training

  @ret N/A
**/
VOID
DfeSummerOffsetCalibrationSetup (
  IN      PSYSHOST                Host,
  IN      UINT8                   Socket,
     OUT  SENSE_AMP_SETTING_STORE *SettingStore
  )
{
  UINT8                                         McId;
  UINT8                                         Ch;
  UINT8                                         SubCh;
  UINT8                                         Strobe;
  UINT8                                         MSVx4;
  DDRIO_CHANNEL_NVDATA                          (*DdrioChannelNvList) [MAX_CH];
  DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_STRUCT          DdrdN0TrainCtl1;
  DDRCC_COMP_LA0_MCIO_DDRIOEXT_STRUCT           DdrccCompLa0;
  DDRD_N0_COMP_LA0_MCIO_DDRIO_STRUCT            DdrdN0CompLa0;
  DDRCC_RX_CTL0_MCIO_DDRIOEXT_STRUCT            DdrccRxCtl0;
  DDRD_N0_RX_CTL0_MCIO_DDRIO_STRUCT             DdrdN0RxCtl0;
  DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_STRUCT          DdrdTrainCtl2;
  DDRD_N0_DFX_CTL0_MCIO_DDRIO_STRUCT            DdrdN0DfxCtl0;
  DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_STRUCT         DdrccTrainCtl1;
  DDRCC_DFX_CTL0_MCIO_DDRIOEXT_STRUCT           DdrccDfxCtl0;

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);
  MSVx4 = GetMaxStrobeValid (Host);

  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }
      //
      // Disconnect VREF, short the VOC switch and drive the DQ/VREF (set the common mode) to 0x75*VCCD through ODT by doing the following steps:
      //
      // Tri-state VREFINT (ddrd__n[1:0]_train_ctl1.vref_highz = 1)
      // Enable 1 segment pull-up/pull-dn ODT (force_strong_odt_on = 1)
      //
      DdrdN0TrainCtl1.Data = (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe];
      DdrdN0TrainCtl1.Bits.ddr4 = 0;
      DdrdN0TrainCtl1.Bits.dq_vref_en = 1;
      DdrdN0TrainCtl1.Bits.force_sa_on = 1;
      DdrdN0TrainCtl1.Bits.force_rxbias_on = 1;
      DdrdN0TrainCtl1.Bits.vref_highz = 1;
      DdrdN0TrainCtl1.Bits.force_strong_odt_on = 1;
      (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe] = DdrdN0TrainCtl1.Data;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_REG), DdrdN0TrainCtl1.Data);

      //
      // No Vref power saving in Self Refresh. All DQ Vref generators is enabled by dq_vref_en.
      //
      DdrdN0DfxCtl0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_DFX_CTL0_MCIO_DDRIO_REG));
      DdrdN0DfxCtl0.Bits.sr_vref_dis = 0;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_DFX_CTL0_MCIO_DDRIO_REG), DdrdN0DfxCtl0.Data);

      //
      // Enables feedback from DQ SAL for any DDR5 training modes, requiring feedback from DQ Lanes
      //
      DdrdTrainCtl2.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG));
      DdrdTrainCtl2.Bits.sampler_feedback_en = 1;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG), DdrdTrainCtl2.Data);

      //
      // Capture the current values of: odt_static_leg_cfg (Data), rcomp_odt_up (Data),
      // rcomp_odt_dn (Data), rcomp_drv_up (CMD) and rcomp_drv_dn (CMD). These will need to be restored by the end of training.
      //
      DdrdN0RxCtl0.Data = (*DdrioChannelNvList)[Ch].DdrDataRxCtl0[Strobe];
      SettingStore->DataOdtStaticLegCfgStore [Ch][Strobe] = DdrdN0RxCtl0.Bits.odt_static_leg_cfg;
      DdrdN0CompLa0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_COMP_LA0_MCIO_DDRIO_REG));
      SettingStore->DdrcompDataComp0Store [Ch][Strobe].Data = DdrdN0CompLa0.Data;
      //
      // Enable 12 legs for ODT pull-dn and 35 legs for ODT pull-up by setting odt_static_leg_cfg = 0x1c,
      // rcomp_odt_up (Data) and rcomp_drv_up (CMD)= 0x1F, and rcomp_odt_dn (Data) and rcomp_drv_dn (CMD)= 0.
      //
      DdrdN0RxCtl0.Bits.odt_static_leg_cfg = 0x1C;
      (*DdrioChannelNvList)[Ch].DdrDataRxCtl0[Strobe] = DdrdN0RxCtl0.Data;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_RX_CTL0_MCIO_DDRIO_REG), DdrdN0RxCtl0.Data);
      DdrdN0CompLa0.Bits.rcomp_odt_up = 0x1F;
      DdrdN0CompLa0.Bits.rcomp_odt_dn = 0;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_COMP_LA0_MCIO_DDRIO_REG), DdrdN0CompLa0.Data);

    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      //
      // Set DDRCC sr_vref_dis to 0 during Sense Amp Calibration
      //
      DdrccDfxCtl0.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_DFX_CTL0_MCIO_DDRIOEXT_REG);
      DdrccDfxCtl0.Bits.sr_vref_dis = 0;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_DFX_CTL0_MCIO_DDRIOEXT_REG, DdrccDfxCtl0.Data);

      //
      // Capture the current values of: odt_static_leg_cfg (CMD), rcomp_drv_up (CMD) and rcomp_drv_dn (CMD).
      // These will need to be restored by the end of training.
      //
      DdrccRxCtl0.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_RX_CTL0_MCIO_DDRIOEXT_REG);
      SettingStore->CcOdtStaticLegCfgStore [Ch][SubCh] = DdrccRxCtl0.Bits.odt_static_leg_cfg;
      DdrccCompLa0.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_COMP_LA0_MCIO_DDRIOEXT_REG);
      SettingStore->DdrcompCmdComp0Store[Ch][SubCh].Data = DdrccCompLa0.Data;
      //
      // Enable 12 legs for ODT pull-dn and 35 legs for ODT pull-up by setting odt_static_leg_cfg = 0x1c,
      // rcomp_drv_up (CMD)= 0x1F, and rcomp_drv_dn (CMD)= 0.
      //
      DdrccRxCtl0.Bits.odt_static_leg_cfg = 0x1C;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_RX_CTL0_MCIO_DDRIOEXT_REG, DdrccRxCtl0.Data);

      DdrccCompLa0.Bits.rcomp_drv_up = 0x1F;
      DdrccCompLa0.Bits.rcomp_drv_dn = 0;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_COMP_LA0_MCIO_DDRIOEXT_REG, DdrccCompLa0.Data);

      DdrccTrainCtl1.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_REG);
      SettingStore->DdrccTrainCtl1Store[Ch][SubCh] = DdrccTrainCtl1.Data;
      DdrccTrainCtl1.Bits.ddr4 = 0;
      DdrccTrainCtl1.Bits.rx_vref_en = 3;
      DdrccTrainCtl1.Bits.force_sa_on = 1;
      DdrccTrainCtl1.Bits.force_rxbias_on = 1;
      DdrccTrainCtl1.Bits.vref_highz = 1;
      DdrccTrainCtl1.Bits.force_strong_odt_on = 1;
      DdrccTrainCtl1.Bits.rx_path_fb_en = 1;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_REG, DdrccTrainCtl1.Data);

    }//Subch loop
  } //Ch loop
  //
  // Force comp update.
  //
  for (McId = 0; McId < MAX_IMC; McId++) {
    if (IsMcPopulated (Socket, McId) == FALSE) {
      continue;
    }
    DoCompUpdate (Host, Socket, McId);
  } // McId loop
}

/*
  Restore DDR Phy settings after Sense Amp Calibration
  Used by both HW FSM and SW based training methods

  @param[in]  Host          - Pointer to SysHost
  @param[in]  Socket        - Socket number
  @param[out] SettingStore  - Pointer to temporary storage for settings modified
                              during this training

  @ret N/A
*/
VOID
DfeSummerOffsetCalibrationRestore (
  IN  PSYSHOST                Host,
  IN  UINT8                   Socket,
  IN  SENSE_AMP_SETTING_STORE *SettingStore
  )
{
  UINT8                                         McId;
  UINT8                                         Ch;
  UINT8                                         SubCh;
  UINT8                                         Strobe;
  UINT8                                         MSVx4;
  DDRIO_CHANNEL_NVDATA                          (*DdrioChannelNvList) [MAX_CH];
  DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_STRUCT          DdrdN0TrainCtl1;
  DDRCC_COMP_LA0_MCIO_DDRIOEXT_STRUCT           DdrccCompLa0;
  DDRD_N0_COMP_LA0_MCIO_DDRIO_STRUCT            DdrdN0CompLa0;
  DDRCC_RX_CTL0_MCIO_DDRIOEXT_STRUCT            DdrccRxCtl0;
  DDRD_N0_RX_CTL0_MCIO_DDRIO_STRUCT             DdrdN0RxCtl0;
  DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_STRUCT          DdrdTrainCtl2;
  DDRD_N0_DFX_CTL0_MCIO_DDRIO_STRUCT            DdrdN0DfxCtl0;
  DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_STRUCT         DdrccTrainCtl1;
  DDRCC_DFX_CTL0_MCIO_DDRIOEXT_STRUCT           DdrccDfxCtl0;


  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);
  MSVx4 = GetMaxStrobeValid (Host);

  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }

      DdrdN0TrainCtl1.Data = (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe];
      DdrdN0TrainCtl1.Bits.force_strong_odt_on = 0;
      DdrdN0TrainCtl1.Bits.force_sa_on = 0;
      DdrdN0TrainCtl1.Bits.vref_highz = 0;
      (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe] = DdrdN0TrainCtl1.Data;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_REG), DdrdN0TrainCtl1.Data);

      //
      // Return sr_vref_dis to 1 for power savings
      //
      DdrdN0DfxCtl0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_DFX_CTL0_MCIO_DDRIO_REG));
      DdrdN0DfxCtl0.Bits.sr_vref_dis = 1;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_DFX_CTL0_MCIO_DDRIO_REG), DdrdN0DfxCtl0.Data);
      //
      // Restore the Data odt_static_leg_cfg  value.
      //
      DdrdN0RxCtl0.Data = (*DdrioChannelNvList)[Ch].DdrDataRxCtl0[Strobe];
      DdrdN0RxCtl0.Bits.odt_static_leg_cfg = SettingStore->DataOdtStaticLegCfgStore[Ch][Strobe];
      (*DdrioChannelNvList)[Ch].DdrDataRxCtl0[Strobe] = DdrdN0RxCtl0.Data;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_RX_CTL0_MCIO_DDRIO_REG), DdrdN0RxCtl0.Data);
      //
      // restore the current data rcomp_odt_up and  rcomp_odt_dn  value.
      //
      DdrdN0CompLa0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_COMP_LA0_MCIO_DDRIO_REG));
      DdrdN0CompLa0.Bits.rcomp_odt_up = SettingStore->DdrcompDataComp0Store[Ch][Strobe].Bits.rcomp_odt_up;
      DdrdN0CompLa0.Bits.rcomp_odt_dn = SettingStore->DdrcompDataComp0Store[Ch][Strobe].Bits.rcomp_odt_dn;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_COMP_LA0_MCIO_DDRIO_REG), DdrdN0CompLa0.Data);

      //
      // Disable the sampler feedback.
      //
      DdrdTrainCtl2.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG));
      DdrdTrainCtl2.Bits.sampler_feedback_en = 0;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG), DdrdTrainCtl2.Data);

    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      //
      // Restore DDRCC sr_vref_dis to 1 for power savings
      //
      DdrccDfxCtl0.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_DFX_CTL0_MCIO_DDRIOEXT_REG);
      DdrccDfxCtl0.Bits.sr_vref_dis = 1;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_DFX_CTL0_MCIO_DDRIOEXT_REG, DdrccDfxCtl0.Data);

      //
      // Restore the current CMD odt_static_leg_cfg  value.
      //
      DdrccRxCtl0.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_RX_CTL0_MCIO_DDRIOEXT_REG);
      DdrccRxCtl0.Bits.odt_static_leg_cfg = SettingStore->CcOdtStaticLegCfgStore[Ch][SubCh];
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_RX_CTL0_MCIO_DDRIOEXT_REG, DdrccRxCtl0.Data);
      //
      // Restore CMD rcomp_drv_up  and rcomp_drv_dn
      //
      DdrccCompLa0.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_COMP_LA0_MCIO_DDRIOEXT_REG);
      DdrccCompLa0.Bits.rcomp_drv_up = SettingStore->DdrcompCmdComp0Store[Ch][SubCh].Bits.rcomp_drv_up;
      DdrccCompLa0.Bits.rcomp_drv_dn = SettingStore->DdrcompCmdComp0Store[Ch][SubCh].Bits.rcomp_drv_dn;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_COMP_LA0_MCIO_DDRIOEXT_REG, DdrccCompLa0.Data);

      DdrccTrainCtl1.Data = SettingStore->DdrccTrainCtl1Store[Ch][SubCh];
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_REG, DdrccTrainCtl1.Data);
    }//subch loop
  } //Ch loop

  //
  // Force comp update.
  //
  for (McId = 0; McId < MAX_IMC; McId++) {
    if (IsMcPopulated (Socket, McId) == FALSE) {
      continue;
    }
    DoCompUpdate (Host, Socket, McId);
  } // McId loop
}

/*
  Enable Odd or Even Sampler

  @param  Socket Socket number
  @param  Ch     Channel number
  @param  TrainResult   Even and Odd sampler training results.

  @ret N/A
*/
VOID
ProgramEvenOddSamplerFinalResults (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     TrainResult[EVEN_ODD_SAMPLER][MAX_CH][MAX_STROBE_DDR5][BITS_PER_NIBBLE]
  )
{
  UINT8 SamplerType;
  UINT8                                         Ch;
  UINT8                                         Strobe;
  DDRIO_CHANNEL_NVDATA                          (*DdrioChannelNvList) [MAX_CH];
  DDRD_N0_SAMPLER_EVEN_OFFSET_MCIO_DDRIO_STRUCT DdrdN0SamplerEvenOffset;
  DDRD_N0_SAMPLER_ODD_OFFSET_MCIO_DDRIO_STRUCT  DdrdN0SamplerOddOffset;
  DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_STRUCT          DdrdN0TrainCtl1;
  UINT8                                         MSVx4 = GetMaxStrobeValid (Host);

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);
  for (SamplerType = Even; SamplerType < EVEN_ODD_SAMPLER; SamplerType++) {
    //
    // Program the final results.
    //
    for (Ch = 0; Ch < MAX_CH; Ch++) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }
      for (Strobe = 0; Strobe < MSVx4; Strobe++) {
        if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
          continue;
        }
        if (SamplerType == Even) {
          //
          // Disable the calibration mode.
          //
          DdrdN0TrainCtl1.Data = (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe];
          DdrdN0TrainCtl1.Bits.even_sampler_offset_cal_en = 0;
          (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe] = DdrdN0TrainCtl1.Data;
          MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_REG), DdrdN0TrainCtl1.Data);
          //
          // Program the final result.
          //
          DdrdN0SamplerEvenOffset.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_SAMPLER_EVEN_OFFSET_MCIO_DDRIO_REG));
          DdrdN0SamplerEvenOffset.Bits.sampler_even_offset0 = TrainResult[Even][Ch][Strobe][0];
          DdrdN0SamplerEvenOffset.Bits.sampler_even_offset1 = TrainResult[Even][Ch][Strobe][1];
          DdrdN0SamplerEvenOffset.Bits.sampler_even_offset2 = TrainResult[Even][Ch][Strobe][2];
          DdrdN0SamplerEvenOffset.Bits.sampler_even_offset3 = TrainResult[Even][Ch][Strobe][3];
          MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_SAMPLER_EVEN_OFFSET_MCIO_DDRIO_REG), DdrdN0SamplerEvenOffset.Data);

          // Cache the result
          (*DdrioChannelNvList) [Ch].RxSamplerEven[Strobe] = DdrdN0SamplerEvenOffset.Data;
        } else {
          //
          // Disable the calibration mode.
          //
          DdrdN0TrainCtl1.Data = (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe];
          DdrdN0TrainCtl1.Bits.odd_sampler_offset_cal_en = 0;
          (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe] = DdrdN0TrainCtl1.Data;
          MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_REG), DdrdN0TrainCtl1.Data);
          //
          // Program the final result.
          //
          DdrdN0SamplerOddOffset.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_SAMPLER_ODD_OFFSET_MCIO_DDRIO_REG));
          DdrdN0SamplerOddOffset.Bits.sampler_odd_offset0 = TrainResult[Odd][Ch][Strobe][0];
          DdrdN0SamplerOddOffset.Bits.sampler_odd_offset1 = TrainResult[Odd][Ch][Strobe][1];
          DdrdN0SamplerOddOffset.Bits.sampler_odd_offset2 = TrainResult[Odd][Ch][Strobe][2];
          DdrdN0SamplerOddOffset.Bits.sampler_odd_offset3 = TrainResult[Odd][Ch][Strobe][3];
          MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_SAMPLER_ODD_OFFSET_MCIO_DDRIO_REG), DdrdN0SamplerOddOffset.Data);

          // Cache the result
          (*DdrioChannelNvList) [Ch].RxSamplerOdd[Strobe] = DdrdN0SamplerOddOffset.Data;
        }
      }
    } //Ch
  } //SamplerType
}

/*
  Enable Odd or Even Sampler

  @param  Socket Socket number
  @param  Ch     Channel number
  @param  Type   Sampler type
  @param  TrainResult   Even and Odd sampler training results

  @ret N/A
*/
VOID
EnableOddEvenSampler (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN MRC_CT    Type,
  IN UINT8     TrainResult[EVEN_ODD_SAMPLER][MAX_CH][MAX_STROBE_DDR5][BITS_PER_NIBBLE]
  )
{

#ifdef DEBUG_CODE_BLOCK
  CHAR8                                         *PrintString = Type == Even ? "Even" : "Odd";
#endif //DEBUG_CODE_BLOCK
  UINT32                                        SaSampler;
  UINT8                                         Ch;
  UINT8                                         Strobe;
  DDRIO_CHANNEL_NVDATA                          (*DdrioChannelNvList) [MAX_CH];
  DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_STRUCT          DdrdN0TrainCtl1;
  DDRD_N0_SAMPLER_EVEN_OFFSET_MCIO_DDRIO_STRUCT DdrdN0SamplerEvenOffset;
  DDRD_N0_SAMPLER_ODD_OFFSET_MCIO_DDRIO_STRUCT  DdrdN0SamplerOddOffset;
  DDRD_N0_TRAIN_RESULT0_MCIO_DDRIO_STRUCT       DdrdN0TrainResult0;
  UINT8                                         Bit;
  UINT8                                         SampOffset;
  UINT8                                         ResultPassCh[MAX_CH];
  UINT8                                         ResultPassStrobe[MAX_CH][MAX_STROBE_DDR5];
  UINT8                                         ResultPassBit[MAX_CH][MAX_STROBE_DDR5][BITS_PER_NIBBLE];
  UINT8                                         PreviousSampleResult[MAX_CH][MAX_STROBE_DDR5][BITS_PER_NIBBLE];
  UINT8                                         NewSampleResult[MAX_CH][MAX_STROBE_DDR5][BITS_PER_NIBBLE];
  BOOLEAN                                       AllStrobePass = FALSE;
  BOOLEAN                                       AllChPass = FALSE;
  UINT8                                         StrobeBitPassNumber = 0;
  UINT8                                         EvenOffsetValue = 0;
  UINT8                                         OddOffsetValue = 0;
  UINT8                                         MSVx4 = GetMaxStrobeValid (Host);

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);
  ZeroMem ((UINT8 *) ResultPassCh, sizeof (ResultPassCh));
  ZeroMem ((UINT8 *) ResultPassStrobe, sizeof (ResultPassStrobe));
  ZeroMem ((UINT8 *) ResultPassBit, sizeof (ResultPassBit));
  ZeroMem ((UINT8 *) PreviousSampleResult, sizeof (PreviousSampleResult));
  ZeroMem ((UINT8 *) NewSampleResult, sizeof (NewSampleResult));

  if (Type == Even) {
    EvenOffsetValue = 0;
    OddOffsetValue = 0x1F;
  } else {
    EvenOffsetValue = 0x1F;
    OddOffsetValue = 0;
  }
  //
  // Enable Even/Odd sampler
  //
  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }
      DdrdN0TrainCtl1.Data = (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe];
      if (Type == Even) {
        DdrdN0TrainCtl1.Bits.even_sampler_offset_cal_en = 1;
      } else {
        DdrdN0TrainCtl1.Bits.odd_sampler_offset_cal_en = 1;
      }
      (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe] = DdrdN0TrainCtl1.Data;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_REG), DdrdN0TrainCtl1.Data);
    }
  }
  //
  // Program Even/Odd sampler for bit 0 to 3.
  //
  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }
      DdrdN0SamplerEvenOffset.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_SAMPLER_EVEN_OFFSET_MCIO_DDRIO_REG));
      DdrdN0SamplerEvenOffset.Bits.sampler_even_offset0 = EvenOffsetValue;
      DdrdN0SamplerEvenOffset.Bits.sampler_even_offset1 = EvenOffsetValue;
      DdrdN0SamplerEvenOffset.Bits.sampler_even_offset2 = EvenOffsetValue;
      DdrdN0SamplerEvenOffset.Bits.sampler_even_offset3 = EvenOffsetValue;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_SAMPLER_EVEN_OFFSET_MCIO_DDRIO_REG), DdrdN0SamplerEvenOffset.Data);
      //
      // Program odd sampler for bit 0 to 3.
      //
      DdrdN0SamplerOddOffset.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_SAMPLER_ODD_OFFSET_MCIO_DDRIO_REG));
      DdrdN0SamplerOddOffset.Bits.sampler_odd_offset0 = OddOffsetValue;
      DdrdN0SamplerOddOffset.Bits.sampler_odd_offset1 = OddOffsetValue;
      DdrdN0SamplerOddOffset.Bits.sampler_odd_offset2 = OddOffsetValue;
      DdrdN0SamplerOddOffset.Bits.sampler_odd_offset3 = OddOffsetValue;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_SAMPLER_ODD_OFFSET_MCIO_DDRIO_REG), DdrdN0SamplerOddOffset.Data);
    }
  }
  //
  // Sweep the offset
  //
#ifdef DEBUG_CODE_BLOCK
  RcDebugPrint (SDBG_MAX,
    "\nSampler Offset Cancelation Calibration Training: %a sampler\n", PrintString);
  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    RcDebugPrint (SDBG_MAX,
      "Strobe                ");
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }
      RcDebugPrint (SDBG_MAX,
        "      %2d ", Strobe);
    } // Strobe loop
  }
  RcDebugPrint (SDBG_MAX, "\n");

#endif // DEBUG_CODE_BLOCK
  for (SampOffset = 0; SampOffset <= 62; SampOffset++) {
    for (Ch = 0; Ch < MAX_CH; Ch++) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }
      for (Strobe = 0; Strobe < MSVx4; Strobe++) {
        if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
          continue;
        }
        if (Type == Even) {
          DdrdN0SamplerEvenOffset.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_SAMPLER_EVEN_OFFSET_MCIO_DDRIO_REG));
          DdrdN0SamplerEvenOffset.Bits.sampler_even_offset0 = SampOffset;
          DdrdN0SamplerEvenOffset.Bits.sampler_even_offset1 = SampOffset;
          DdrdN0SamplerEvenOffset.Bits.sampler_even_offset2 = SampOffset;
          DdrdN0SamplerEvenOffset.Bits.sampler_even_offset3 = SampOffset;
          MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_SAMPLER_EVEN_OFFSET_MCIO_DDRIO_REG), DdrdN0SamplerEvenOffset.Data);
        } else {
          DdrdN0SamplerOddOffset.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_SAMPLER_ODD_OFFSET_MCIO_DDRIO_REG));
          DdrdN0SamplerOddOffset.Bits.sampler_odd_offset0 = SampOffset;
          DdrdN0SamplerOddOffset.Bits.sampler_odd_offset1 = SampOffset;
          DdrdN0SamplerOddOffset.Bits.sampler_odd_offset2 = SampOffset;
          DdrdN0SamplerOddOffset.Bits.sampler_odd_offset3 = SampOffset;
          MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_SAMPLER_ODD_OFFSET_MCIO_DDRIO_REG), DdrdN0SamplerOddOffset.Data);
        }
      }
    }
    //
    // Tunable delay
    //
    TrainingDelay (Host, SampOffset == 0 ? 600 : 160, 2);
    //
    // Read the result
    //
    AllChPass = TRUE;
    for (Ch = 0; Ch < MAX_CH; Ch++) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "SampOffset %2d ", SampOffset);
      AllStrobePass = TRUE;
      for (Strobe = 0; Strobe < MSVx4; Strobe++) {
        if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
          continue;
        }
        if (ResultPassCh[Ch] == 1) {
          RcDebugPrint (SDBG_MAX, "         ");
          continue;
        }
        DdrdN0TrainResult0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_RESULT0_MCIO_DDRIO_REG));
        StrobeBitPassNumber = 0;
        SaSampler = Type == Even ? DdrdN0TrainResult0.Bits.sa_even_sampler : DdrdN0TrainResult0.Bits.sa_odd_sampler;
        for (Bit = 0; Bit < BITS_PER_NIBBLE; Bit++) {
          if (!((1 << Bit) & SaSampler)) {
            RcDebugPrint (SDBG_MAX,
              " 0");
          } else {
            //
            // Even mode: Found 0 -> 1 transition.
            // Odd mode: Found 0 -> 1 transition.
            //
            if (ResultPassBit[Ch][Strobe][Bit] != 1) {
              TrainResult[Type][Ch][Strobe][Bit] = SampOffset;
            }
            ResultPassBit[Ch][Strobe][Bit] = 1;
            RcDebugPrint (SDBG_MAX,
              " 1");
          }
          if (ResultPassBit[Ch][Strobe][Bit] == 1) {
            StrobeBitPassNumber = StrobeBitPassNumber + 1;
          }
        }
        if (StrobeBitPassNumber == BITS_PER_NIBBLE) {
          ResultPassStrobe[Ch][Strobe] = 1;
          RcDebugPrint (SDBG_MAX, "*");
        }
        if (ResultPassStrobe[Ch][Strobe] == 0) {
          AllStrobePass = FALSE;
          RcDebugPrint (SDBG_MAX, " ");
        }
      }//Strobe loop
      //
      // check if all the bits are passed or not
      //
      if (AllStrobePass == TRUE) {
        ResultPassCh[Ch] = 1;
      }
      if (ResultPassCh[Ch] == 0) {
        AllChPass = FALSE;
      }
    }//Ch loop
    RcDebugPrint (SDBG_MAX,
      "\n");
    if (AllChPass == TRUE) {
      break;
    }
  } // SampOffset
  //
  // Disable Even/Odd sampler
  //
  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }
      DdrdN0TrainCtl1.Data = (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe];
      if (Type == Even) {
        DdrdN0TrainCtl1.Bits.even_sampler_offset_cal_en = 0;
      } else {
        DdrdN0TrainCtl1.Bits.odd_sampler_offset_cal_en = 0;
      }
      (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe] = DdrdN0TrainCtl1.Data;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_REG), DdrdN0TrainCtl1.Data);
    }
  }

}

/*
  CMD DFE Summer Offset Calibration.

  @param[in]  Host     System host pointer.
  @param[in]  Socket   Socket number.

  @ret N/A
*/
VOID
CmdDfeSummerOffsetCalibration (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  UINT8                                                 Ch;
  UINT8                                                 SubCh;
  DDRIO_CHANNEL_NVDATA                                  (*DdrioChannelNvList) [MAX_CH];
  UINT8                                                 TrainResult[MAX_CH][SUB_CH][MAX_DDR_CC_BITS_PER_SUB_CH];
  UINT8                                                 ResultPassSubCh[MAX_CH][SUB_CH];
  UINT8                                                 ResultPassCh[MAX_CH];
  UINT8                                                 ResultPassBit[MAX_CH][SUB_CH][MAX_DDR_CC_BITS_PER_SUB_CH];
  UINT8                                                 SubChBitPassNumber = 0;
  BOOLEAN                                               AllSubChPass;
  BOOLEAN                                               AllChPass;
  UINT8                                                 SampOffset;
  DDRCC_RX_CTL1_MCIO_DDRIOEXT_STRUCT                    DdrCcRxCtl1;
  DDRCC_DFX_CTL0_MCIO_DDRIOEXT_STRUCT                   DdrccDfxCtl0;
  DDRCC_CTL1_BIT_0_MCIO_DDRIOEXT_STRUCT                 DdrccCtl1;
  DDRCC_TRAIN_RESULT0_MCIO_DDRIOEXT_STRUCT              DdrccTrainResult0;
  UINT8                                                 DdrccBit;
  UINT32                                                RegAddress;
  UINT8                                                 RspA;
  UINT8                                                 RspB;
  UINT8                                                 SubChTemp;

  AllSubChPass = FALSE;
  AllChPass = FALSE;
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);
  ZeroMem ((UINT8 *) TrainResult, sizeof (TrainResult));
  ZeroMem ((UINT8 *) ResultPassCh, sizeof (ResultPassCh));
  ZeroMem ((UINT8 *) ResultPassSubCh, sizeof (ResultPassSubCh));
  ZeroMem ((UINT8 *) ResultPassBit, sizeof (ResultPassBit));

  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      //
      // a. Enable RX Amp. Offset
      //
      DdrCcRxCtl1.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_RX_CTL1_MCIO_DDRIOEXT_REG);
      DdrCcRxCtl1.Bits.rx_offset_en  = 1;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_RX_CTL1_MCIO_DDRIOEXT_REG, DdrCcRxCtl1.Data);

      //
      // b. Enable VOC training mode
      //
      DdrccDfxCtl0.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_DFX_CTL0_MCIO_DDRIOEXT_REG);
      DdrccDfxCtl0.Bits.rx_voc_pass_gate_en  = 0x3fffff;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_DFX_CTL0_MCIO_DDRIOEXT_REG, DdrccDfxCtl0.Data);
    }
  } //Ch loop

  //
  // c. Sweep the offset
  //

  RcDebugPrint (SDBG_MAX,
    "\nSampler Offset Cancelation Calibration Training: CMD Summer Offset\n\n");

  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    RcDebugPrint (SDBG_MAX, "CC Bit:                ");
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      for (DdrccBit = 0; DdrccBit < MAX_DDR_CC_BITS_PER_SUB_CH; DdrccBit++) {
        RcDebugPrint (SDBG_MAX, " %2d ", DdrccBit);
      }
    }
  }
  RcDebugPrint (SDBG_MAX, "\n");

  for (SampOffset = 0; SampOffset <= 60; SampOffset++) {
    for (Ch = 0; Ch < MAX_CH; Ch++) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

        for (DdrccBit = 0; DdrccBit < MAX_DDR_CC_BITS_PER_SUB_CH; DdrccBit++) {

           RegAddress = GetDdrCcCtl1RegisterAddress (Host, DdrccBit);

           DdrccCtl1.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, RegAddress);
           DdrccCtl1.Bits.sa_offset  = SampOffset;
           WriteDdrioCcRegister (Host, Socket, Ch, SubCh, RegAddress, DdrccCtl1.Data);
        }
      }
    }

    //
    // Tunable delay
    //
    TrainingDelay (Host, SampOffset == 0 ? 600 : 160, 2);

    //
    // Read the result
    //
    AllChPass = TRUE;
    for (Ch = 0; Ch < MAX_CH; Ch++) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }

      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "SampOffset %2d: ", SampOffset);

      AllSubChPass = TRUE;
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

        DdrccTrainResult0.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_TRAIN_RESULT0_MCIO_DDRIOEXT_REG);

        SubChBitPassNumber = 0;
        for (DdrccBit = 0; DdrccBit < MAX_DDR_CC_BITS_PER_SUB_CH; DdrccBit++) {
          if ((1 << DdrccBit) & DdrccTrainResult0.Bits.sa_train_result) {
            //
            // Feedback is 1.
            //
            RcDebugPrint (SDBG_MAX, "  1 ");
          } else {
            //
            // Found 1 -> 0 transition.
            //
            RcDebugPrint (SDBG_MAX, "  0 ");
            if (ResultPassBit[Ch][SubCh][DdrccBit] != 1) {
              TrainResult[Ch][SubCh][DdrccBit] = SampOffset;
            }
            ResultPassBit[Ch][SubCh][DdrccBit] = 1;
          }
          if (ResultPassBit[Ch][SubCh][DdrccBit] == 1) {
            SubChBitPassNumber = SubChBitPassNumber + 1;
          }
        }
        if (SubChBitPassNumber == MAX_DDR_CC_BITS_PER_SUB_CH) {
          ResultPassSubCh[Ch][SubCh] = 1;
        }
        if (ResultPassSubCh[Ch][SubCh] == 0) {
          AllSubChPass = FALSE;
        }
      }//SubCh loop

      //
      // check if all the bits are passed or not
      //
      if (AllSubChPass == TRUE) {
        ResultPassCh[Ch] = 1;
      }
      if (ResultPassCh[Ch] == 0) {
        AllChPass = FALSE;
      }
    }//Ch loop

    RcDebugPrint (SDBG_MAX, " \n");

    if (AllChPass == TRUE) {
      break;
    }
  } // SampOffset

  //
  // Program the final results.
  //
  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    SignalToRegIndex(Host, Socket, Ch, RSPA_SA, &SubChTemp, &RspA);
    SignalToRegIndex(Host, Socket, Ch, RSPB_SA, &SubChTemp, &RspB);

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

      //
      // d. Disable VOC training mode
      //
      DdrccDfxCtl0.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_DFX_CTL0_MCIO_DDRIOEXT_REG);
      DdrccDfxCtl0.Bits.rx_voc_pass_gate_en  = 0;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_DFX_CTL0_MCIO_DDRIOEXT_REG, DdrccDfxCtl0.Data);
    }

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      for (DdrccBit = 0; DdrccBit < MAX_DDR_CC_BITS_PER_SUB_CH; DdrccBit++) {

        RegAddress = GetDdrCcCtl1RegisterAddress (Host, DdrccBit);

        DdrccCtl1.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, RegAddress);
        DdrccCtl1.Bits.sa_offset  = TrainResult[Ch][SubCh][DdrccBit];
        WriteDdrioCcRegister (Host, Socket, Ch, SubCh, RegAddress, DdrccCtl1.Data);

        //
        // Currently only save RspA and RspB which are required, to save NVM.
        // Note: RspA and RspB pin index are the same on SUB_CH_A and SUB_CH_B.
        //

        if (DdrccBit == RspA) {
          (*DdrioChannelNvList) [Ch].SubChannel[SubCh].RespASaOffset= DdrccCtl1.Bits.sa_offset;
        }

        if (DdrccBit == RspB) {
          (*DdrioChannelNvList) [Ch].SubChannel[SubCh].RespBSaOffset= DdrccCtl1.Bits.sa_offset;
        }

      }
    } // SubCh
  } //Ch

  //
  // Display training result.
  //
  RcDebugPrint (SDBG_MAX, "\nSTART_CMD_RX_SUMMER_OFFSET_CC\n");

  RcDebugPrint (SDBG_MAX, "CC Bit:      ");
  for (DdrccBit = 0; DdrccBit < MAX_DDR_CC_BITS_PER_SUB_CH; DdrccBit++) {
    RcDebugPrint (SDBG_MAX, "  %2d  ", DdrccBit);
  }
  RcDebugPrint (SDBG_MAX, "\n");

  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT, " ");
      for (DdrccBit = 0; DdrccBit < MAX_DDR_CC_BITS_PER_SUB_CH; DdrccBit++) {
        RegAddress = GetDdrCcCtl1RegisterAddress (Host, DdrccBit);
        DdrccCtl1.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, RegAddress);
        RcDebugPrint (SDBG_MAX, "  %2d  ", DdrccCtl1.Bits.sa_offset);
      }
      RcDebugPrint (SDBG_MAX, "\n");
    }
  }
  RcDebugPrint (SDBG_MAX, "\nSTOP_CMD_RX_SUMMER_CC\n");

  return;
}

/*
  DFE Summer Offset Calibration

  @param  Socket Socket number
  @param  Ch     Channel number

  @ret N/A
*/
VOID
DfeSummerOffsetCalibration (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  UINT8                                         Ch;
  UINT8                                         Strobe;
  UINT8                                         LogRank;
  DDRIO_CHANNEL_NVDATA                          (*DdrioChannelNvList) [MAX_CH];
  DDRD_N0_RX_CTL1_MCIO_DDRIO_STRUCT             DdrdN0RxCTL1;
  DDRD_N0_TRAIN_RESULT0_MCIO_DDRIO_STRUCT       DdrdN0TrainResult0;
  UINT8                                         PreviousSampleResult[MAX_CH][MAX_STROBE_DDR5][BITS_PER_NIBBLE];
  UINT8                                         NewSampleResult[MAX_CH][MAX_STROBE_DDR5][BITS_PER_NIBBLE];
  UINT8                                         TrainResult[MAX_CH][MAX_STROBE_DDR5][BITS_PER_NIBBLE];
  UINT8                                         ResultPassStrobe[MAX_CH][MAX_STROBE_DDR5];
  UINT8                                         ResultPassCh[MAX_CH];
  UINT8                                         ResultPassBit[MAX_CH][MAX_STROBE_DDR5][BITS_PER_NIBBLE];
  UINT8                                         StrobeBitPassNumber = 0;
  BOOLEAN                                       AllStrobePass;
  BOOLEAN                                       AllChPass;
  UINT8                                         Bit;
  UINT8                                         SampOffset;
  struct ddrRank                                (*RankList) [MAX_RANK_DIMM];
  struct dimmNvram                              (*DimmNvList) [MAX_DIMM];
  DDRD_N0_SA_OFFSET_RANK_0_MCIO_DDRIO_STRUCT    DdrdN0SaOffsetRank0;
  UINT8                                         Dimm;
  UINT8                                         Rank;
  DDRD_N0_DFX_CTL0_MCIO_DDRIO_STRUCT            DdrdN0DfxCtl0;
  SENSE_AMP_SETTING_STORE                       SettingStore;
  UINT8                                         MSVx4 = GetMaxStrobeValid (Host);

  AllStrobePass = FALSE;
  AllChPass = FALSE;
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);
  ZeroMem ((UINT8 *) TrainResult, sizeof (TrainResult));
  ZeroMem ((UINT8 *) ResultPassCh, sizeof (ResultPassCh));
  ZeroMem ((UINT8 *) ResultPassStrobe, sizeof (ResultPassStrobe));
  ZeroMem ((UINT8 *) ResultPassBit, sizeof (ResultPassBit));
  ZeroMem ((UINT8 *) PreviousSampleResult, sizeof (PreviousSampleResult));
  ZeroMem ((UINT8 *) NewSampleResult, sizeof (NewSampleResult));
  ZeroMem ((UINT8 *) &SettingStore, sizeof (SettingStore));

  //
  // Initialize and save DDR Phy settings
  //
  DfeSummerOffsetCalibrationSetup (Host, Socket, &SettingStore);

  //
  // DATA Summer Offset Cancelation Calibration.
  //

  //
  // Program Even sampler for bit 0 to 3.
  //
  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }
      DdrdN0RxCTL1.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_RX_CTL1_MCIO_DDRIO_REG));
      DdrdN0RxCTL1.Bits.rx_offset_en = 1;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_RX_CTL1_MCIO_DDRIO_REG), DdrdN0RxCTL1.Data);
      //
      // Enable VOC training mode.
      //
      DdrdN0DfxCtl0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_DFX_CTL0_MCIO_DDRIO_REG));
      DdrdN0DfxCtl0.Bits.rx_voc_pass_gate_en = 1;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_DFX_CTL0_MCIO_DDRIO_REG), DdrdN0DfxCtl0.Data);


    } //Strobe loop
  } //Ch loop
  //
  // Sweep the offset
  //
#ifdef DEBUG_CODE_BLOCK
  RcDebugPrint (SDBG_MAX,
    "\nSampler Offset Cancelation Calibration Training: DATA Summer Offset\n");
  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    RcDebugPrint (SDBG_MAX, "Strobe                ");
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }
      RcDebugPrint (SDBG_MAX, "      %2d ", Strobe);
    } // Strobe loop
  }
  RcDebugPrint (SDBG_MAX, "\n");
#endif // DEBUG_CODE_BLOCK
  for (SampOffset = 0; SampOffset <= 60; SampOffset++) {
    for (Ch = 0; Ch < MAX_CH; Ch++) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }
      for (Strobe = 0; Strobe < MSVx4; Strobe++) {
        if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
          //
          // Skip if no DIMM present
          //
          if ((*DimmNvList) [Dimm].dimmPresent == 0) {
            continue;
          }
          RankList = GetRankNvList (Host, Socket, Ch, Dimm);
          for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
            //
            // Skip if no Rank
            //
            if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
              continue;
            }
            DdrdN0SaOffsetRank0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegister10nm (Host, Ch, (*RankList) [Rank].logicalRank, Strobe, DDRD_N0_SA_OFFSET_RANK_0_MCIO_DDRIO_REG));
            DdrdN0SaOffsetRank0.Bits.sa_offset0 = SampOffset;
            DdrdN0SaOffsetRank0.Bits.sa_offset1 = SampOffset;
            DdrdN0SaOffsetRank0.Bits.sa_offset2 = SampOffset;
            DdrdN0SaOffsetRank0.Bits.sa_offset3 = SampOffset;
            MemWritePciCfgEp (Socket, Ch, UpdateIoRegister10nm (Host, Ch, (*RankList) [Rank].logicalRank, Strobe, DDRD_N0_SA_OFFSET_RANK_0_MCIO_DDRIO_REG), DdrdN0SaOffsetRank0.Data);
          } //Rank
        } //Dimm
      }
    }
    //
    // Tunable delay
    //
    TrainingDelay (Host, SampOffset == 0 ? 600 : 160, 2);
    //
    // Read the result
    //
    AllChPass = TRUE;
    for (Ch = 0; Ch < MAX_CH; Ch++) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "SampOffset %2d ", SampOffset);
      AllStrobePass = TRUE;
      for (Strobe = 0; Strobe < MSVx4; Strobe++) {
        if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
          continue;
        }
        if (ResultPassCh[Ch] == 1) {
          RcDebugPrint (SDBG_MAX, "         ");
          continue;
        }
        DdrdN0TrainResult0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_RESULT0_MCIO_DDRIO_REG));

        StrobeBitPassNumber = 0;
        for (Bit = 0; Bit < BITS_PER_NIBBLE; Bit++) {
          if ((1 << Bit) & DdrdN0TrainResult0.Bits.sa_even_sampler) {
            RcDebugPrint (SDBG_MAX,
              " 1");
          } else {
            //
            // Found 1 -> 0 transition.
            //
            if (ResultPassBit[Ch][Strobe][Bit] != 1) {
              TrainResult[Ch][Strobe][Bit] = SampOffset;
            }
            ResultPassBit[Ch][Strobe][Bit] = 1;
            RcDebugPrint (SDBG_MAX,
              " 0");
          }
          if (ResultPassBit[Ch][Strobe][Bit] == 1) {
            StrobeBitPassNumber = StrobeBitPassNumber + 1;
          }
        }
        if (StrobeBitPassNumber == BITS_PER_NIBBLE) {
          ResultPassStrobe[Ch][Strobe] = 1;
          RcDebugPrint (SDBG_MAX, "*");
        }
        if (ResultPassStrobe[Ch][Strobe] == 0) {
          AllStrobePass = FALSE;
          RcDebugPrint (SDBG_MAX, " ");
        }
      }//Strobe loop
      //
      // check if all the bits are passed or not
      //
      if (AllStrobePass == TRUE) {
        ResultPassCh[Ch] = 1;
      }
      if (ResultPassCh[Ch] == 0) {
        AllChPass = FALSE;
      }
    }//Ch loop
    RcDebugPrint (SDBG_MAX,
      "\n");
    if (AllChPass == TRUE) {
      break;
    }
  } // SampOffset
  //
  // Program the final results.
  //
  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }
      //
      // Disable VOC training mode.
      //
      DdrdN0DfxCtl0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_DFX_CTL0_MCIO_DDRIO_REG));
      DdrdN0DfxCtl0.Bits.rx_voc_pass_gate_en = 0;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_DFX_CTL0_MCIO_DDRIO_REG), DdrdN0DfxCtl0.Data);
      //
      // Program the final result.
      //
      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        //
        // Skip if no DIMM present
        //
        if ((*DimmNvList) [Dimm].dimmPresent == 0) {
          continue;
        }
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
          //
          // Skip if no Rank
          //
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          DdrdN0SaOffsetRank0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegister10nm (Host, Ch, (*RankList) [Rank].logicalRank, Strobe, DDRD_N0_SA_OFFSET_RANK_0_MCIO_DDRIO_REG));
          DdrdN0SaOffsetRank0.Bits.sa_offset0 = TrainResult[Ch][Strobe][0];
          DdrdN0SaOffsetRank0.Bits.sa_offset1 = TrainResult[Ch][Strobe][1];
          DdrdN0SaOffsetRank0.Bits.sa_offset2 = TrainResult[Ch][Strobe][2];
          DdrdN0SaOffsetRank0.Bits.sa_offset3 = TrainResult[Ch][Strobe][3];
          MemWritePciCfgEp (Socket, Ch, UpdateIoRegister10nm (Host, Ch, (*RankList) [Rank].logicalRank, Strobe, DDRD_N0_SA_OFFSET_RANK_0_MCIO_DDRIO_REG), DdrdN0SaOffsetRank0.Data);

          //
          // Cache DDRD_N0_SA_OFFSET_RANK_0_MCIO_DDRIO_REG to DdrioChannelNvList
          //
          LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
          (*DdrioChannelNvList) [Ch].rxOffset[LogRank][Strobe] = DdrdN0SaOffsetRank0.Data;
        } //rank loop
      }//dimm loop
    }
  } //Ch

  //
  // CMD DFE Summer Offset Cancelation Calibration.
  //
  CmdDfeSummerOffsetCalibration (Host, Socket);

  //
  // Restore saved settings
  //
  DfeSummerOffsetCalibrationRestore (Host, Socket, &SettingStore);

  //
  // Delay if this is a CTE build, otherwise do nothing.
  //
  CteDelayDclk (4);
  IO_Reset (Host, Socket);
}

/**

  This is the SW based Sense Amp Calibration training.
  SenseAmp offset cancellation

  @param Host     - Pointer to sysHost
  @param Socket   - Socket number

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
SenseAmpOffsetSw (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  UINT8                                         Ch;
  UINT8                                         Strobe;
  DDRIO_CHANNEL_NVDATA                          (*DdrioChannelNvList) [MAX_CH];
  UINT8                                         LoopCount;
  UINT8                                         MaxLoopCount;
  DDRD_N0_RX_CTL1_MCIO_DDRIO_STRUCT             DdrdN0RxCTL1;
  DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_STRUCT          DdrdN0TrainCtl1;
  UINT8                                         SamplerLoop;
  DDRD_N0_DFX_CTL0_MCIO_DDRIO_STRUCT            DdrdN0DfxCtl0;
  DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_STRUCT          DdrdTrainCtl2;
  UINT8                                         MSVx4;
  UINT8                                         TrainResult[EVEN_ODD_SAMPLER][MAX_CH][MAX_STROBE_DDR5][BITS_PER_NIBBLE];

  MSVx4 = GetMaxStrobeValid (Host);
  ZeroMem ((UINT8 *) TrainResult, sizeof (TrainResult));

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  MaxLoopCount = 1;
  if (PcdGetBool (PcdMrcDebugSenseAmp) == TRUE) {
    MaxLoopCount = 100;
  }

  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }
      DdrdN0TrainCtl1.Data = (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe];
      //
      // Init other bits.
      //
      DdrdN0TrainCtl1.Bits.force_rxbias_on = 1;
      DdrdN0TrainCtl1.Bits.force_sa_on = 1;
      DdrdN0TrainCtl1.Bits.dq_vref_en = 1;
      (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe] = DdrdN0TrainCtl1.Data;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_REG), DdrdN0TrainCtl1.Data);

      DdrdN0DfxCtl0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_DFX_CTL0_MCIO_DDRIO_REG));
      DdrdN0DfxCtl0.Bits.sr_vref_dis = 0;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_DFX_CTL0_MCIO_DDRIO_REG), DdrdN0DfxCtl0.Data);

      DdrdTrainCtl2.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG));
      DdrdTrainCtl2.Bits.sampler_feedback_en = 1;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG), DdrdTrainCtl2.Data);

    }
  }
  for (LoopCount = 0; LoopCount < MaxLoopCount; LoopCount++) {
    OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, LoopCount + 1, Socket);
    for (SamplerLoop = 0; SamplerLoop < 2; SamplerLoop++) {
      // enable senseamp training mode
      for (Ch = 0; Ch < MAX_CH; Ch++) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }
        for (Strobe = 0; Strobe < MSVx4; Strobe++) {
          if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
            continue;
          }
          //
          // Enable sampler offset.
          //
          DdrdN0RxCTL1.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_RX_CTL1_MCIO_DDRIO_REG));
          DdrdN0RxCTL1.Bits.rx_sampler_offset_en = 1;
          MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_RX_CTL1_MCIO_DDRIO_REG), DdrdN0RxCTL1.Data);
        }
      } //Ch loop
      EnableOddEvenSampler (Host, Socket, SamplerLoop == 0 ? Even : Odd, TrainResult);
    } //samlerloop
    //
    // Program the final training results.
    //
    ProgramEvenOddSamplerFinalResults (Host, Socket,TrainResult);

    //
    // Delay if this is a CTE build, otherwise do nothing.
    //
    CteDelayDclk (4);
    IO_Reset (Host, Socket);
    DfeSummerOffsetCalibration (Host, Socket);
  } //loopcount
  return MRC_STATUS_SUCCESS;
}

/**
  Return TRUE if HW FSM based Sense Amp Offset Calibration training is to be used.

  @retval TRUE  - Use HW FSM based Sense Amp Offset Calibration training
  @retval FALSE - Use SW based Sense Amp Offset Calibration training

  **/
BOOLEAN
SenseAmpOffsetHwFsmEnabled (
  VOID
  )
{
  EFI_STATUS  Status;
  BOOLEAN     RetVal;
  UINT32      CteOption;
  SYS_SETUP   *Setup;

  Setup = GetSysSetupPointer ();

  //
  // Check the Setup policy to determine if the HW FSM based Sense Amp Calibration should be used
  //
  RetVal = FALSE;
  if (Setup->mem.dfxMemSetup.SenseAmpCalibHwSwOption == DFX_SENSE_AMP_CALIB_HW_FSM) {
    RetVal = TRUE;
  }

  //
  // Allow CTE to override the policy
  //
  if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    Status = CteGetOptions ("SenseAmpHwFsm", &CteOption);
    if (!EFI_ERROR (Status)) {
      if (CteOption == 1) {
        RetVal = TRUE;
      } else {
        RetVal = FALSE;
      }
    }
  } // PcdCteBuild

  return RetVal;
}

/**
  Enable the desired stage, in this case the samplers and DFE summer stages for Data,
  and DFE summer only for CMD/CTL

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket number

  @retval N/A
**/
VOID
SenseAmpOffsetHwFsmEnableStages (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  UINT8                                         Ch;
  UINT8                                         SubCh;
  UINT8                                         Strobe;
  UINT8                                         MSVx4;
  UINT8                                         MaxChDdr;
  DDRD_N0_HW_TRAIN_CTL0_MCIO_DDRIO_STRUCT       DdrdN0HwTrainCtl0;
  DDRCC_HW_TRAIN_CTL0_MCIO_DDRIOEXT_STRUCT      DdrccHwTrainCtl0;

  MSVx4 = GetMaxStrobeValid (Host);
  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    //
    // Enable the samplers and DFE summer stages for Data
    // Program the number of samples
    //
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }
      DdrdN0HwTrainCtl0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_HW_TRAIN_CTL0_MCIO_DDRIO_REG));
      DdrdN0HwTrainCtl0.Bits.dfe_summer_offset_stage_en = 1;
      DdrdN0HwTrainCtl0.Bits.sampler_offset_stage_en    = 1;
      DdrdN0HwTrainCtl0.Bits.xover_stage_en             = 0;
      DdrdN0HwTrainCtl0.Bits.sample_cnt                 = SENSE_AMP_SAMPLE_COUNT_HW_FSM;
      DdrdN0HwTrainCtl0.Bits.sample_thresh              = SENSE_AMP_SAMPLE_THRESHOLD_HW_FSM;
      DdrdN0HwTrainCtl0.Bits.settling_time_x2           = SENSE_AMP_SETTLING_TIME_HW_FSM;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_HW_TRAIN_CTL0_MCIO_DDRIO_REG), DdrdN0HwTrainCtl0.Data);
    } // Strobe

    //
    // Enable DFE summer only for CMD/CTL
    //
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      DdrccHwTrainCtl0.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_HW_TRAIN_CTL0_MCIO_DDRIOEXT_REG);
      DdrccHwTrainCtl0.Bits.dfe_summer_offset_stage_en = 1;
      DdrccHwTrainCtl0.Bits.xover_stage_en             = 0;
      DdrccHwTrainCtl0.Bits.sample_cnt                 = SENSE_AMP_SAMPLE_COUNT_HW_FSM;
      DdrccHwTrainCtl0.Bits.sample_thresh              = SENSE_AMP_SAMPLE_THRESHOLD_HW_FSM;
      DdrccHwTrainCtl0.Bits.settling_time_x2           = SENSE_AMP_SETTLING_TIME_HW_FSM;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_HW_TRAIN_CTL0_MCIO_DDRIOEXT_REG, DdrccHwTrainCtl0.Data);
    }//Subch

  } // Ch
}

/**
  Clear HW FSM Sense Amp Offset Calibration error registers

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket number

  @retval N/A
**/
VOID
SenseAmpOffsetHwFsmClearErrors (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  UINT8                                       Ch;
  UINT8                                       SubCh;
  UINT8                                       Strobe;
  UINT8                                       RegIndex;
  UINT8                                       MSVx4;
  UINT8                                       MaxChDdr;
  DDRD_N0_HW_TRAIN_CTL0_MCIO_DDRIO_STRUCT     DdrdN0HwTrainCtl0;
  DDRCC_HWCAL_STS0_0_MCIO_DDRIOEXT_STRUCT     DdrccHwcalSts;
  UINT32                                      DdrccHwcalStsReg[] = {DDRCC_HWCAL_STS0_0_MCIO_DDRIOEXT_REG,
                                                                    DDRCC_HWCAL_STS0_1_MCIO_DDRIOEXT_REG,
                                                                    DDRCC_HWCAL_STS0_2_MCIO_DDRIOEXT_REG,
                                                                    DDRCC_HWCAL_STS0_3_MCIO_DDRIOEXT_REG};

  MSVx4 = GetMaxStrobeValid (Host);
  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    //
    // Clear DFE summer, odd sampler and even sampler errors for Data
    //
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }
      DdrdN0HwTrainCtl0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_HW_TRAIN_CTL0_MCIO_DDRIO_REG));
      DdrdN0HwTrainCtl0.Bits.dfe_summer_offset_err      = 1;
      DdrdN0HwTrainCtl0.Bits.odd_sampler_offset_err     = 1;
      DdrdN0HwTrainCtl0.Bits.even_sampler_offset_err    = 1;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_HW_TRAIN_CTL0_MCIO_DDRIO_REG), DdrdN0HwTrainCtl0.Data);
    } // Strobe

    //
    // Clear DFE summer errors for CMD/CTL
    //
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      for (RegIndex = 0; RegIndex < ARRAY_SIZE (DdrccHwcalStsReg); RegIndex++) {
        DdrccHwcalSts.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DdrccHwcalStsReg[RegIndex]);
        DdrccHwcalSts.Bits.dfe_summer_offset_err        = 1;
        WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DdrccHwcalStsReg[RegIndex], DdrccHwcalSts.Data);
      } // RegIndex
    }//Subch
  } // Ch
} // SenseAmpOffsetHwFsmClearErrors

/**
  Generate RcSim Feedback for Sense Amp Calibration by modifying the error registers

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket number

  @retval - N/A
**/
VOID
SenseAmpOffsetHwFsmErrorsRcSimFeedback (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
}

/**
  Read HW FSM Sense Amp Offset Calibration error registers
  Display error message if needed

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket number

  @retval EFI_SUCCESS       HW FSM was successful
  @retval EFI_DEVICE_ERROR  An error was reported by HW
**/
MRC_STATUS
SenseAmpOffsetHwFsmGetAndDisplayErrors (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  EFI_STATUS                                  Status;
  UINT8                                       Ch;
  UINT8                                       Strobe;
  UINT8                                       MSVx4;
  UINT8                                       MaxChDdr;
  DDRD_N0_HW_TRAIN_CTL0_MCIO_DDRIO_STRUCT     DdrdN0HwTrainCtl0;

  Status = MRC_STATUS_SUCCESS;
  MaxChDdr = GetMaxChDdr ();
  MSVx4 = GetMaxStrobeValid (Host);

  SenseAmpOffsetHwFsmErrorsRcSimFeedback (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    //
    // Clear DFE summer, odd sampler and even sampler errors for Data
    //
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }
      DdrdN0HwTrainCtl0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_HW_TRAIN_CTL0_MCIO_DDRIO_REG));
      if (DdrdN0HwTrainCtl0.Bits.dfe_summer_offset_err !=0) {
        Status = MRC_STATUS_FAILURE;
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, Strobe, NO_BIT,
          "ERROR: DATA DFE Summer Offset\n");
      }
      if (DdrdN0HwTrainCtl0.Bits.odd_sampler_offset_err !=0) {
        Status = MRC_STATUS_FAILURE;
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, Strobe, NO_BIT,
          "ERROR: DATA Odd Sampler Offset\n");
      }
      if (DdrdN0HwTrainCtl0.Bits.even_sampler_offset_err !=0) {
        Status = MRC_STATUS_FAILURE;
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, Strobe, NO_BIT,
          "ERROR: DATA Even Sampler Offset\n");
      }
    } // Strobe
  } // Ch
  return Status;
}

/**
  Configure and start the HW FSM Sense Amp Offset Calibration test

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket number
  @param[in] Ch       - Channel number

  @retval N/A
**/
VOID
SenseAmpConfigureStartHwFsm (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch
  )
{
  UINT8                                       SubCh;
  UINT8                                       Strobe;
  UINT8                                       MSVx4;
  DDRD_N0_HW_TRAIN_CTL0_MCIO_DDRIO_STRUCT     DdrdN0HwTrainCtl0;
  DDRCC_HW_TRAIN_CTL0_MCIO_DDRIOEXT_STRUCT    DdrccHwTrainCtl0;

  MSVx4 = GetMaxStrobeValid (Host);
  //
  // Start the FSM for DATA
  // HW calibation enable control per FSM (DATA has 1 per strobe). Set to 1 to enable HW calibration.
  //
  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
      continue;
    }
    DdrdN0HwTrainCtl0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_HW_TRAIN_CTL0_MCIO_DDRIO_REG));
    DdrdN0HwTrainCtl0.Bits.hw_cal_en                  = 1;
    MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_HW_TRAIN_CTL0_MCIO_DDRIO_REG), DdrdN0HwTrainCtl0.Data);
  } // Strobe

  //
  // Start the FSM for CMD/CTL
  // HW calibation enable control per FSM (CMD/CTL has 4). Set to 1 to enable the HW calibration.
  //
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    DdrccHwTrainCtl0.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_HW_TRAIN_CTL0_MCIO_DDRIOEXT_REG);
    DdrccHwTrainCtl0.Bits.hw_cal_en       = 0xf;
    WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_HW_TRAIN_CTL0_MCIO_DDRIOEXT_REG, DdrccHwTrainCtl0.Data);
  }//Subch

} // SenseAmpConfigureStartHwFsm

/**
  Return list of CMD/CTL signals to train for Sense Amp Calibration

  @param[out] CcSignals   - Pointer to signal list returned
  @param[out] NumSignals  - Pointer to length of list returned

  @retval N/A
**/
VOID
GetCcSignalsToTrain (
  OUT  GSM_CSN   **CcSignals,
  OUT  UINT8     *NumSignals
  )
{
  STATIC GSM_CSN     CcSignalsToTrain[] = CC_SENSE_AMP_SIGNAL_LIST;

  ASSERT (CC_SENSE_AMP_SIGNAL_LIST_SIZE == ARRAY_SIZE (CcSignalsToTrain));
  *CcSignals = CcSignalsToTrain;
  *NumSignals = ARRAY_SIZE (CcSignalsToTrain);
  return;
}

/**
  Print HW FSM Sense Amp Offset Calibration training results

  @param[in] Host           - Pointer to sysHost
  @param[in] Socket         - Socket number
  @param[in] TrainResults   - Training results for all channels

  @retval N/A
**/
VOID
SenseAmpOffsetHwFsmDisplayResults (
  IN  PSYSHOST                  Host,
  IN  UINT8                     Socket,
  IN  SENSE_AMP_HWFSM_RESULTS   *TrainResults
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8           Ch;
  UINT8           MSVx4;
  UINT8           MaxChDdr;
  GSM_CSN         *CcSignals;
  UINT8           NumCcSignals;
  UINT8           SignalIndex;

  MaxChDdr = GetMaxChDdr ();
  MSVx4 = GetMaxStrobeValid (Host);
  RcDebugPrint (SDBG_MAX, "\nSTART_DATA_RX_SUMMER_CC\n");
  GetCcSignalsToTrain (&CcSignals, &NumCcSignals);
  RcDebugPrint (SDBG_MAX, "PARAM:   ");
  for (SignalIndex = 0; SignalIndex < NumCcSignals/2; SignalIndex++) {
    if (CcSignals[SignalIndex] == RSPA_SA) {
      RcDebugPrint (SDBG_MAX, "    RSPA");
    } else if (CcSignals[SignalIndex] == RSPB_SA) {
      RcDebugPrint (SDBG_MAX, "    RSPB");
    }
  } // SignalIndex
  RcDebugPrint (SDBG_MAX, "\n");
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SUB_CH_A, NO_RANK, NO_STROBE, NO_BIT,
      "");
    for (SignalIndex = 0; SignalIndex < NumCcSignals; SignalIndex++) {
      if (SignalIndex == NumCcSignals/2) {
        RcDebugPrint (SDBG_MAX, "\n");
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SUB_CH_B, NO_RANK, NO_STROBE, NO_BIT,
          "");
      }
      RcDebugPrint (SDBG_MAX, "  %3d   ", TrainResults->CcSaOffset[Ch][SignalIndex]);
    } // SignalIndex
    RcDebugPrint (SDBG_MAX, "\n");
  } // Ch
  RcDebugPrint (SDBG_MAX, "STOP_DATA_RX_SUMMER_CC\n");


#endif // DEBUG_CODE_BLOCK
}

/**
  Read and return the sa_offset for a CMD/CTL signal

  @param[in]  Host            - Pointer to SYSHOST
  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number
  @param[in]  SignalName      - Name of the CMD/CTL signal
  @param[out] SubChannel      - SubChannel number associated with signal
  @param[out] RegisterOffset  - Register offset for given signal

  @retval MRC_STATUS_SUCCESS if the signal is found and *SaOffset
                             has been updated with the register value
  @retval MRC_STATUS_SIGNAL_NOT_SUPPORTED otherwise

**/
MRC_STATUS
GetCcSaRegOffset (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  GSM_CSN   SignalName,
  UINT8     *SubChannel,
  UINT32    *RegisterOffset
  )
{
  MRC_STATUS                              Status;
  UINT8                                   RegisterIndex;
  UINT32                                  DdrccCtl1BitReg[] = {DDRCC_CTL1_BIT_0_MCIO_DDRIOEXT_REG,
                                                               DDRCC_CTL1_BIT_1_MCIO_DDRIOEXT_REG,
                                                               DDRCC_CTL1_BIT_2_MCIO_DDRIOEXT_REG,
                                                               DDRCC_CTL1_BIT_3_MCIO_DDRIOEXT_REG,
                                                               DDRCC_CTL1_BIT_4_MCIO_DDRIOEXT_REG,
                                                               DDRCC_CTL1_BIT_5_MCIO_DDRIOEXT_REG,
                                                               DDRCC_CTL1_BIT_6_MCIO_DDRIOEXT_REG,
                                                               DDRCC_CTL1_BIT_7_MCIO_DDRIOEXT_REG,
                                                               DDRCC_CTL1_BIT_8_MCIO_DDRIOEXT_REG,
                                                               DDRCC_CTL1_BIT_9_MCIO_DDRIOEXT_REG,
                                                               DDRCC_CTL1_BIT_10_MCIO_DDRIOEXT_REG,
                                                               DDRCC_CTL1_BIT_11_MCIO_DDRIOEXT_REG,
                                                               DDRCC_CTL1_BIT_12_MCIO_DDRIOEXT_REG,
                                                               DDRCC_CTL1_BIT_13_MCIO_DDRIOEXT_REG,
                                                               DDRCC_CTL1_BIT_14_MCIO_DDRIOEXT_REG,
                                                               DDRCC_CTL1_BIT_15_MCIO_DDRIOEXT_REG,
                                                               DDRCC_CTL1_BIT_16_MCIO_DDRIOEXT_REG,
                                                               DDRCC_CTL1_BIT_17_MCIO_DDRIOEXT_REG,
                                                               DDRCC_CTL1_BIT_18_MCIO_DDRIOEXT_REG,
                                                               DDRCC_CTL1_BIT_19_MCIO_DDRIOEXT_REG,
                                                               DDRCC_CTL1_BIT_20_MCIO_DDRIOEXT_REG,
                                                               DDRCC_CTL1_BIT_21_MCIO_DDRIOEXT_REG};

  //
  // Get pi register index. This has taken lane reversal into consideration.
  //
  Status = SignalToRegIndex (Host, Socket, Ch, SignalName, SubChannel, &RegisterIndex);

  if ((Status == MRC_STATUS_SUCCESS) &&
      (RegisterIndex < ARRAY_SIZE (DdrccCtl1BitReg)) &&
      (*SubChannel < SUB_CH)) {

    *RegisterOffset = DdrccCtl1BitReg[RegisterIndex];

  } else {
    Status = MRC_STATUS_SIGNAL_NOT_SUPPORTED;
  }
  return Status;
} // GetSetCmdSaOffset
/**
  Clear HW FSM Sense Amp Offset Calibration error registers

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket number

  @retval N/A
**/
VOID
DisplaySenseAmpTrainingResults (
  IN  UINT8     Socket
  )
{
  UINT8                                         Ch;
  UINT8                                         MaxChDdr;
  GSM_CSN                                       *CcSignals;
  UINT8                                         NumCcSignals;
  UINT8                                         SignalIndex;
  SENSE_AMP_HWFSM_RESULTS                       TrainResults;
  DDRCC_CTL1_BIT_0_MCIO_DDRIOEXT_STRUCT         DdrccCtl1Bit;
  UINT32                                        DdrccCtl1RegOffset;
  UINT8                                         MSVx4;
  PSYSHOST  Host;
  DDRIO_CHANNEL_NVDATA                          (*DdrioChannelNvList)[MAX_CH];
  UINT8                                         SubCh;

  Host = GetSysHostPointer ();
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);
  ZeroMem ((UINT8 *) &TrainResults, sizeof (TrainResults));
  MaxChDdr = GetMaxChDdr ();
  MSVx4 = GetMaxStrobeValid (Host);

  DisplayResultsDdr5 (Host, Socket, RxSamplerOdd);
  DisplayResultsDdr5 (Host, Socket, RxSamplerEven);
  DisplayResultsDdr5 (Host, Socket, RxSummer);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    //
    // Read results for CMD/CTL
    //
    GetCcSignalsToTrain (&CcSignals, &NumCcSignals);
    for (SignalIndex = 0; SignalIndex < NumCcSignals; SignalIndex++) {
      GetCcSaRegOffset (Host, Socket, Ch, CcSignals[SignalIndex], &SubCh, &DdrccCtl1RegOffset);

      // Read values from cache
      switch (CcSignals[SignalIndex]) {
        case RSPA_SA:
          DdrccCtl1Bit.Data = (*DdrioChannelNvList) [Ch].SubChannel[SubCh].RespASaOffset;
          break;

        case RSPB_SA:
          DdrccCtl1Bit.Data = (*DdrioChannelNvList) [Ch].SubChannel[SubCh].RespBSaOffset;
          break;

        case RSPA_SB:
          DdrccCtl1Bit.Data = (*DdrioChannelNvList) [Ch].SubChannel[SubCh].RespASaOffset;
          break;

        case RSPB_SB:
          DdrccCtl1Bit.Data = (*DdrioChannelNvList) [Ch].SubChannel[SubCh].RespBSaOffset;
          break;

        default:
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "ERROR: CMD/CTL Invalid signal!\n");
          break;
      } // SignalIndex

      TrainResults.CcSaOffset[Ch][SignalIndex] = (UINT8) DdrccCtl1Bit.Bits.sa_offset;
    }
  } // Ch

  SenseAmpOffsetHwFsmDisplayResults (Host, Socket, &TrainResults);

}

/**

  Retrieves results from HW FSM based Sense Amp Calibration training and stores
  them in NVRAM. Also checks for errors while reading CMD/CTL results.

  @retval EFI_SUCCESS       - No errors detected
  @retval EFI_DEVICE_ERROR  - Error detected in hardware

**/
MRC_STATUS
CacheSenseAmpSettings (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  UINT8                                         Ch;
  UINT8                                         Dimm;
  UINT8                                         Rank;
  UINT8                                         Strobe;
  UINT8                                         SubChannel;
  UINT8                                         MaxChDdr;
  UINT8                                         LogRank = 0;
  GSM_CSN                                       *CcSignals;
  UINT8                                         NumCcSignals;
  UINT8                                         SignalIndex;
  SENSE_AMP_HWFSM_RESULTS                       TrainResults;
  DDRD_N0_SAMPLER_ODD_OFFSET_MCIO_DDRIO_STRUCT  DdrdN0SamplerOddOffset;
  DDRD_N0_SAMPLER_EVEN_OFFSET_MCIO_DDRIO_STRUCT DdrdN0SamplerEvenOffset;
  DDRD_N0_SA_OFFSET_RANK_0_MCIO_DDRIO_STRUCT    DdrdN0SaOffsetRank0;
  DDRCC_CTL1_BIT_0_MCIO_DDRIOEXT_STRUCT         DdrccCtl1Bit;
  UINT32                                        DdrccCtl1RegOffset;
  UINT8                                         MSVx4;
  DDRIO_CHANNEL_NVDATA                          (*DdrioChannelNvList)[MAX_CH];
  MRC_STATUS                                    Status = MRC_STATUS_SUCCESS;

  Host = GetSysHostPointer ();
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);
  ZeroMem ((UINT8 *) &TrainResults, sizeof (TrainResults));
  MaxChDdr = GetMaxChDdr ();
  MSVx4 = GetMaxStrobeValid (Host);
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    //
    // Clear DFE summer, odd sampler and even sampler errors for Data
    //
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }

      //
      // Read and cache sampler results
      //
      DdrdN0SamplerOddOffset.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_SAMPLER_ODD_OFFSET_MCIO_DDRIO_REG));
      (*DdrioChannelNvList) [Ch].RxSamplerOdd[Strobe] = DdrdN0SamplerOddOffset.Data;
      DdrdN0SamplerEvenOffset.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_SAMPLER_EVEN_OFFSET_MCIO_DDRIO_REG));
      (*DdrioChannelNvList) [Ch].RxSamplerEven[Strobe] = DdrdN0SamplerEvenOffset.Data;

      //
      // Summer results
      // Only read rank 0 - all ranks are equal
      //
      Rank = 0;
      DdrdN0SaOffsetRank0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegister10nm (Host, Ch, Rank, Strobe, DDRD_N0_SA_OFFSET_RANK_0_MCIO_DDRIO_REG));

      //
      // Cache results to DdrioChannelNvList
      //
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
          (*DdrioChannelNvList) [Ch].rxOffset[LogRank][Strobe] = DdrdN0SaOffsetRank0.Data;
        } // Rank
      } // Dimm
    } // Strobe

    //
    // Read results for CMD/CTL
    //
    GetCcSignalsToTrain (&CcSignals, &NumCcSignals);
    for (SignalIndex = 0; SignalIndex < NumCcSignals; SignalIndex++) {
      GetCcSaRegOffset (Host, Socket, Ch, CcSignals[SignalIndex], &SubChannel, &DdrccCtl1RegOffset);
      DdrccCtl1Bit.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubChannel, DdrccCtl1RegOffset);

      // Save CMD/CTL to cache
      switch (CcSignals[SignalIndex]) {
        case RSPA_SA:
          (*DdrioChannelNvList) [Ch].SubChannel[SubChannel].RespASaOffset = DdrccCtl1Bit.Data;
          break;

        case RSPB_SA:
          (*DdrioChannelNvList) [Ch].SubChannel[SubChannel].RespBSaOffset = DdrccCtl1Bit.Data;
          break;

        case RSPA_SB:
          (*DdrioChannelNvList) [Ch].SubChannel[SubChannel].RespASaOffset = DdrccCtl1Bit.Data;
          break;

        case RSPB_SB:
          (*DdrioChannelNvList) [Ch].SubChannel[SubChannel].RespBSaOffset = DdrccCtl1Bit.Data;
          break;

        default:
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, Strobe, NO_BIT,
          "ERROR: CMD/CTL Invalid signal!\n");
          Status |= MRC_STATUS_FAILURE;
      }

      // Make sure offset does not exceed maximum
      if (DdrccCtl1Bit.Bits.sa_offset >= SENSE_AMP_MAX_OFFSET) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, Strobe, NO_BIT,
          "ERROR: CMD/CTL DFE Signal %d Summer Offset\n", SignalIndex);
        Status |= MRC_STATUS_FAILURE;
      }
    } // SignalIndex
  } // Ch

  return Status;
}

/**

  HW FSM based Sense Amp Calibration training.

  @param Host     - Pointer to sysHost
  @param Socket   - Socket number

  @retval EFI_SUCCESS       HW FSM was successful
  @retval EFI_TIMEOUT       Timed out while polling for completion
  @retval EFI_DEVICE_ERROR  An error was reported by HW

**/
MRC_STATUS
SenseAmpOffsetHwFsm (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  MRC_STATUS                      Status;
  UINT8                           Ch;
  UINT8                           MaxChDdr;
  SENSE_AMP_SETTING_STORE         SettingStore;

  Status   = MRC_STATUS_SUCCESS;
  MaxChDdr = GetMaxChDdr ();
  ZeroMem ((UINT8 *) &SettingStore, sizeof (SettingStore));

  //
  // Initialize and save DDR Phy settings
  //
  DfeSummerOffsetCalibrationSetup (Host, Socket, &SettingStore);

  //
  // Enable stages
  //
  SenseAmpOffsetHwFsmEnableStages (Host, Socket);

  //
  // Clear errors
  //
  SenseAmpOffsetHwFsmClearErrors (Host, Socket);

  //
  // Start the test and poll for completion
  // One channel at a time
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    SenseAmpConfigureStartHwFsm (Host, Socket, Ch);
    Status = PollHwFsmComplete (Host, Socket, 1 << Ch, SENSE_AMP_TIME_OUT);
  } // Ch

  //
  // Read error status and report any errors
  //
  Status |= SenseAmpOffsetHwFsmGetAndDisplayErrors (Host, Socket);

  Status |= CacheSenseAmpSettings (Host, Socket);

  //
  // Restore saved settings
  //
  DfeSummerOffsetCalibrationRestore (Host, Socket, &SettingStore);
  return Status;
}

/**

  This function implements Sense Amp Offset training.
  SenseAmp offset cancellation

  @param Host    - Pointer to sysHost

  @retval SUCCESS  - Sense Amp Offset training succeded
  @retval !SUCCESS - Sense Amp Offset training failed

**/
UINT32
EFIAPI
SenseAmpOffset (
  PSYSHOST  Host
  )
{
  MRC_STATUS        Status = MRC_STATUS_SUCCESS;
  UINT8             Socket;
#if defined(CWV_A0_REFACTOR_POST_PO)
  UINT8             Ch;
  UINT8             MaxChDdr;
  struct channelNvram (*channelNvList) [MAX_CH];
#endif

  if (!IsMemFlowEnabled (SenseAmpOffsetTraining)) {
    return SUCCESS;
  }

  Socket = Host->var.mem.currentSocket;

  //
  // Return if this Socket is disabled
  //
  if (IsSocketDimmPopulated (Host, Socket) != TRUE) {
    return SUCCESS;
  }

  OutputCheckpoint (CHECKPOINT_MAJOR_DDRIO_INIT, CHECKPOINT_MINOR_SENSE_AMP_CAL, Socket);

#if defined(CWV_A0_REFACTOR_POST_PO)
  //
  // The problem is that in sense amp training, we need the SPR inputs to be tri-stated
  // But the DDR5 RCD will tri-state RespA and RespB only when DRST is asserted
  // when calling it after FnvDdrtIoInit, DRST is de-asserted high, not meet the requirement.
  // so we need to assert DRST after FnvDdrtIoInit, run sense amp calibration, then de-assert DRST
  //
  channelNvList = GetChannelNvList (Host, Socket);
  MaxChDdr      = GetMaxChDdr ();
  SetDramResetN (Socket, CH_BITMASK, DRAM_RESET_ASSERT);
  FixedDelayMicroSecond (2);
  //
  // De-assert CKE
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*channelNvList) [Ch].enabled == 0) {
      continue;
    }
    if ((CH_BITMASK & (1 << Ch)) == 0) {
      continue;
    }
    OverrideCke (Socket, Ch, (*channelNvList) [Ch].ckeMask, 0);
    RcDebugPrint (SDBG_MAX, "\n Deassert CKE to drive CS low\n");
  } // Ch loop
  //
  // Reset should be held for JEDEC mandated 200us, but a 2 usec
  // fixed delay in MRC should be plenty for stable power DDR reset flow.
  //
  if (Host->var.mem.socket[Socket].firstJEDECDone) {
    FixedDelayMicroSecond (2);
  } else {
    FixedDelayMicroSecond (200);
    Host->var.mem.socket[Socket].firstJEDECDone = 1;
  }
#endif

  if (SenseAmpOffsetHwFsmEnabled ()) {
    Status = SenseAmpOffsetHwFsm (Host, Socket);
    if (Status != MRC_STATUS_SUCCESS) {
      // PollHwFsmComplete returns EFI_SUCCESS (0) or EFI_TIMEOUT. Check for this EFI_STATUS here and report appropriate error.
      if (Status == EFI_TIMEOUT) {
        FatalError (ERR_DDRIO_HWFSM, ERR_SENSEAMP_HWFSM_TIMEOUT);
      } else {
        FatalError (ERR_DDRIO_HWFSM, ERR_SENSEAMP_HWFSM_FAILURE);
      }
    }
  } else {
    Status = SenseAmpOffsetSw (Host, Socket);
  }

#if defined(CWV_A0_REFACTOR_POST_PO)
  SetDramResetN (Socket, CH_BITMASK, DRAM_RESET_DEASSERT);
  FixedDelayMicroSecond (2);
  //
  // assert CKE
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*channelNvList) [Ch].enabled == 0) {
      continue;
    }
    if ((CH_BITMASK & (1 << Ch)) == 0) {
      continue;
    }
    RcDebugPrint (SDBG_MAX, "\n Assert CKE to drive CS high\n");
    OverrideCke (Socket, Ch, (*channelNvList) [Ch].ckeMask, (*channelNvList) [Ch].ckeMask);
  } // Ch loop
#endif

  //
  // BIOS shall drive tristate on RespA and RespB
  //
  DdrtDriveRespARespBHigh (Socket);

  if (Status == MRC_STATUS_SUCCESS) {
    //
    // Display training results
    //
    DisplaySenseAmpTrainingResults (Socket);

    return SUCCESS;
  } else {
    return (UINT32) Status;
  }
}

/**

  Restores the Sense Amp Calibration settings for a given channel.

  @param[in] Socket   - Socket number
  @param[in] Channel  - Channel number

  @retval N/A

**/
VOID
RestoreSenseAmpCalib (
  IN UINT8  Socket,
  IN UINT8  Channel
  )
{
  PSYSHOST                                       Host;
  DDRD_N0_SAMPLER_ODD_OFFSET_MCIO_DDRIO_STRUCT   DdrdN0SamplerOddOffset;
  DDRD_N0_SAMPLER_EVEN_OFFSET_MCIO_DDRIO_STRUCT  DdrdN0SamplerEvenOffset;
  DDRD_N0_SA_OFFSET_RANK_0_MCIO_DDRIO_STRUCT     DdrdN0SaOffsetRank0;
  DDRCC_CTL1_BIT_0_MCIO_DDRIOEXT_STRUCT          DdrccCtl1Bit;
  UINT32                                         DdrccCtl1RegOffset;
  struct ddrRank                                 (*RankList) [MAX_RANK_DIMM];
  DDRIO_CHANNEL_NVDATA                           (*DdrioChannelNvList)[MAX_CH];
  UINT8                                          Dimm;
  UINT8                                          SubChannel;
  UINT8                                          Rank;
  UINT8                                          LogicalRank;
  UINT8                                          Strobe;
  UINT8                                          MSVx4;
  GSM_CSN                                        *CcSignals;
  UINT8                                          NumCcSignals;
  UINT8                                          SignalIndex;

  Host = GetSysHostPointer ();
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);
  MSVx4 = GetMaxStrobeValid (Host);

  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
      continue;
    }

    // Restore sampler results
    DdrdN0SamplerOddOffset.Data = (*DdrioChannelNvList) [Channel].RxSamplerOdd[Strobe];
    MemWritePciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_SAMPLER_ODD_OFFSET_MCIO_DDRIO_REG), DdrdN0SamplerOddOffset.Data);

    DdrdN0SamplerEvenOffset.Data = (*DdrioChannelNvList) [Channel].RxSamplerEven[Strobe];
    MemWritePciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_SAMPLER_EVEN_OFFSET_MCIO_DDRIO_REG), DdrdN0SamplerEvenOffset.Data);

    // Restore summer results
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
        continue;
      }
      RankList = GetRankNvList (Host, Socket, Channel, Dimm);
      for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
        //
        // Skip if no Rank
        //
        if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        LogicalRank = GetLogicalRank (Host, Socket, Channel, Dimm, Rank);
        DdrdN0SaOffsetRank0.Data = (*DdrioChannelNvList) [Channel].rxOffset[LogicalRank][Strobe];

        MemWritePciCfgEp (Socket, Channel, UpdateIoRegister10nm (Host, Channel, (*RankList) [Rank].logicalRank, Strobe, DDRD_N0_SA_OFFSET_RANK_0_MCIO_DDRIO_REG), DdrdN0SaOffsetRank0.Data);

      } //rank loop
    }//dimm loop
  } // Strobe

  // Restore results for CMD/CTL
  GetCcSignalsToTrain (&CcSignals, &NumCcSignals);
  for (SignalIndex = 0; SignalIndex < NumCcSignals; SignalIndex++) {
    GetCcSaRegOffset (Host, Socket, Channel, CcSignals[SignalIndex], &SubChannel, &DdrccCtl1RegOffset);

    // Read values from cache
    switch (CcSignals[SignalIndex]) {
      case RSPA_SA:
        DdrccCtl1Bit.Data = (*DdrioChannelNvList) [Channel].SubChannel[SubChannel].RespASaOffset;
        break;

      case RSPB_SA:
        DdrccCtl1Bit.Data = (*DdrioChannelNvList) [Channel].SubChannel[SubChannel].RespBSaOffset;
        break;

      case RSPA_SB:
        DdrccCtl1Bit.Data = (*DdrioChannelNvList) [Channel].SubChannel[SubChannel].RespASaOffset;
        break;

      case RSPB_SB:
        DdrccCtl1Bit.Data = (*DdrioChannelNvList) [Channel].SubChannel[SubChannel].RespBSaOffset;
        break;

      default:
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, Strobe, NO_BIT,
        "ERROR: CMD/CTL Invalid signal!\n");
        break;
    }

    // Write value
    WriteDdrioCcRegister (Host, Socket, Channel, SubChannel, DdrccCtl1RegOffset, DdrccCtl1Bit.Data);
  } // SignalIndex
}
