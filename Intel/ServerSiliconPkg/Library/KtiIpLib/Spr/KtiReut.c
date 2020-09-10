/** @file
  KTI *_KTI_REUT_REG related registers access.

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

#include <Uefi.h>
#include <Base.h>
#include "Include/KtiRegs.h"
#include <Library/UsraCsrLib.h>
#include <KtiSetupDefinitions.h>
#include <Library/MemoryCoreLib.h>
#include <Library/KtiApi.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/IoAccessLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Upi/KtiSi.h>
#include <Library/PreSiliconEnvDetectLib.h>

/**
  Set the Freq_Selection of PIPE_CLK_RATE_CTRL_KTI_REUT_REG for UPI speed transition, also the
  c_op_speed according to the target speed

  @param SocketId        - Socket ID
  @param Port            - Port number
  @param LinkSpeed       - Target Link Speed
  @param SpeedMode       - The target speed mode, 1: Fast mode; 0: slow mode

  @retval EFI_SUCCESS - on successful completion
          EFI_UNSUPPORTED - the link speed is not supported.

**/
EFI_STATUS
EFIAPI
ProgramLogPhyFreqSelecton (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port,
  IN  UINT8                    LinkSpeed,
  IN  UINT8                    SpeedMode
  )
{
  PIPE_CLK_RATE_CTRL_KTI_REUT_STRUCT  KtiReutPipeClkRateCtrl;
  KTIREUT_PH_CTR1_KTI_REUT_STRUCT     KtiReutPhCtr1;
  //
  // Set the Freq_Selection for speed transition, also the c_op_speed according to the target speed
  // It is a new request on SPR.
  //
  KtiReutPipeClkRateCtrl.Data = UsraCsrRead (SocketId, Port, PIPE_CLK_RATE_CTRL_KTI_REUT_REG);
  if (SpeedMode == KTI_LINK_FULL_SPEED_MODE) {
    switch (LinkSpeed) {
      case SPEED_REC_128GT:
        KtiReutPipeClkRateCtrl.Bits.cri_freq_select = PIPE_FREQ_SELECT_128GT;
        break;
      case SPEED_REC_144GT:
        KtiReutPipeClkRateCtrl.Bits.cri_freq_select = PIPE_FREQ_SELECT_144GT;
        break;
      case SPEED_REC_160GT:
        KtiReutPipeClkRateCtrl.Bits.cri_freq_select = PIPE_FREQ_SELECT_160GT;
        break;
      default:
        DEBUG ((DEBUG_INFO, "\n  The Speed is not supported!!!\n"));
        return EFI_UNSUPPORTED;
    }
  } else {
    //
    // Slow mode
    //
    KtiReutPipeClkRateCtrl.Bits.cri_freq_select = 0;
  }
  DEBUG ((DEBUG_INFO, "\n  Socket %u Port %u : PIPE_CLK_RATE_CTRL_KTI_REUT write 0x%x\n", SocketId, Port, KtiReutPipeClkRateCtrl.Data));
  UsraCsrWrite (SocketId, Port, PIPE_CLK_RATE_CTRL_KTI_REUT_REG, KtiReutPipeClkRateCtrl.Data);

  //
  // set the c_op_speed according to the target speed
  //
  KtiReutPhCtr1.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG);
  KtiReutPhCtr1.Bits.c_op_speed = SpeedMode;
  UsraCsrWrite (SocketId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG, KtiReutPhCtr1.Data);
  return EFI_SUCCESS;
}

/**
  Check if the cri_pclkchangeok_flag of PIPE_CLK_RATE_CTRL_KTI_REUT_REG is set.

  @param SocketId        - Socket ID
  @param Port            - Port number

  @retval FALSE - HSPHY clock change failed
          TRUE -  HSPHY clock change is done

**/
BOOLEAN
EFIAPI
CheckPhyClkChangeDone (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port
  )
{
  PIPE_CLK_RATE_CTRL_KTI_REUT_STRUCT  KtiReutPipeClkRateCtrl;
  UINT8                               *RegAddr;
  BOOLEAN                             PowerGood;
  UINT32                              TimeoutCounter;

  TimeoutCounter = 100;   // 2ms delay in real silicon

  if (HybridSystemLevelEmulationEnabled() || IsSimicsEnvironment ()) {
    //Change the time out value to 20s for HSLE and Simics (WB model)
    TimeoutCounter = 1000000;
  }

  //
  // Polling the bit3 to be 1
  //
  if (UbiosGenerationEnabled()) {
    RegAddr = (UINT8 *) UsraGetCsrRegisterAddress (SocketId, Port, PIPE_CLK_RATE_CTRL_KTI_REUT_REG);
    PowerGood = CheckCurrentReset (POST_RESET_POWERGOOD);
    EmulationPolling (RegAddr, BIT3, 1, PowerGood);
  }

  do {
    KtiReutPipeClkRateCtrl.Data = UsraCsrRead (SocketId, Port, PIPE_CLK_RATE_CTRL_KTI_REUT_REG);
    FixedDelayMicroSecond (20);
    TimeoutCounter--;
  } while ((KtiReutPipeClkRateCtrl.Bits.cri_pclkchangeok_flag != 1) && (TimeoutCounter > 0));

  if (TimeoutCounter == 0) {
    DEBUG ((EFI_D_ERROR, "\n\n Polling Kti cri_pclkchangeok failed on Socket %x, Link %x! \n", SocketId, Port));
    return FALSE;
  } else {
    return TRUE;
  }
}

/**
  Clear the cri_pclkchangeok_flag of PIPE_CLK_RATE_CTRL_KTI_REUT_REG.

  @param SocketId        - Socket ID
  @param Port            - Port number

  @retval none

**/
VOID
EFIAPI
ClearPhyClkChangeOkFlag (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port
  )
{
  PIPE_CLK_RATE_CTRL_KTI_REUT_STRUCT  KtiReutPipeClkRateCtrl;

  KtiReutPipeClkRateCtrl.Data = UsraCsrRead (SocketId, Port, PIPE_CLK_RATE_CTRL_KTI_REUT_REG);
  //
  // Clear it by writing 1
  //
  KtiReutPipeClkRateCtrl.Bits.cri_pclkchangeok_flag = 1;
  UsraCsrWrite (SocketId, Port, PIPE_CLK_RATE_CTRL_KTI_REUT_REG, KtiReutPipeClkRateCtrl.Data);

  return;
}

/**
  Program values for KTIREUT_PH_CTR2 register.

  @param SocketId        - Socket ID
  @param KtiPort         - Kti Port number
  @param RedriverStatus  - Kit Port redriver status, TRUE: redriver present; FALSE: no redriver or not care

  @retval None
**/
VOID
EFIAPI
ProgramKtiReutPhCtr2 (
  IN UINT8                    SocketId,
  IN UINT8                    KtiPort,
  IN BOOLEAN                  RedriverStatus
  )
{
  KTIREUT_PH_CTR2_KTI_REUT_STRUCT    KtiReutPhCtr2;

  if ((!RedriverStatus) || IsSiliconWorkaroundEnabled ("S14010086617")) {
    //
    // Get control register
    //
    KtiReutPhCtr2.Data = UsraCsrRead (SocketId, KtiPort, KTIREUT_PH_CTR2_KTI_REUT_REG);
    if (!RedriverStatus) {
      KtiReutPhCtr2.Bits.cecnrdp = 0;
    }

    if (IsSiliconWorkaroundEnabled ("S14010086617")) {
      KtiReutPhCtr2.Bits.c_pipe_clk_gate_override = 1;
    }

    DEBUG ((DEBUG_INFO, "\n  Socket %u Port %u : KTIREUT_PH_CTR2 write 0x%x\n", SocketId, KtiPort, KtiReutPhCtr2.Data));
    UsraCsrWrite (SocketId, KtiPort, KTIREUT_PH_CTR2_KTI_REUT_REG, KtiReutPhCtr2.Data);
  }

  return;
}

/**
  Program the PH_TCC with the safe coeff

  @param SocketId   - Socket ID
  @param Port       - Port number
  @param Data       - Safe coeff for all lanes

  @retval

**/
VOID
EFIAPI
SetKtiPhTcc (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port,
  IN  UINT32                   Data
  )
{
  PH_TCC_KTI_REUT_STRUCT           KtiPhTcc;

  KtiPhTcc.Data = UsraCsrRead (SocketId, Port, PH_TCC_KTI_REUT_REG);
  KtiPhTcc.Bits.ctxcoefficients = Data;
  KtiPhTcc.Bits.ctxccvalid = 1;

  UsraCsrWrite (SocketId, Port, PH_TCC_KTI_REUT_REG, KtiPhTcc.Data);
}

/**
  Program the PH_TCA with the safe coeff for adaptaion

  @param SocketId   - Socket ID
  @param Port       - Port number
  @param Data       - Safe coeff for all lanes

  @retval

**/
VOID
EFIAPI
SetKtiPhTca (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port,
  IN  UINT32                   Data
  )
{
  PH_TCA_KTI_REUT_STRUCT           KtiPhTca;

  KtiPhTca.Data = UsraCsrRead (SocketId, Port, PH_TCA_KTI_REUT_REG);
  KtiPhTca.Bits.ctxcoefficients = Data;
  KtiPhTca.Bits.ctxcavalid = 1;

  UsraCsrWrite (SocketId, Port, PH_TCA_KTI_REUT_REG, KtiPhTca.Data);
}

/**
  Set the c_tx_adapt_enable Bit

  @param SocketId   - Socket ID
  @param Port       - Port number
  @param Data       - Data write to the c_tx_adapt_enable

  @retval

**/
VOID
EFIAPI
SetKtiTxAdaptEn (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port,
  IN  UINT8                    Data
  )
{
  KTIREUT_PH_CTR2_KTI_REUT_STRUCT           KtiPhCtr2;

  KtiPhCtr2.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_CTR2_KTI_REUT_REG);
  KtiPhCtr2.Bits.c_tx_adapt_enable = Data;

  UsraCsrWrite (SocketId, Port, KTIREUT_PH_CTR2_KTI_REUT_REG, KtiPhCtr2.Data);
}

/**
  Get the sleffm of PH_FOM_LANE

  @param SocketId   - Socket ID
  @param Port       - Port number
  @param Lane       - Lane number

  @retval: TRUE  -- The FOM value of the Socket x Port y Lane z returned is valid
           FALSE -- The FOM value of the Socket x Port y Lane z returned is invalid

**/
BOOLEAN
EFIAPI
GetKtiPhFomLane (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port,
  IN  UINT8                    Lane,
  OUT UINT8                    *FomOfLane
  )
{
  UINT32 KtiPhFomLanesRegOffset[MAX_LANES_PER_LINK_SPR] = {
                                                            PH_FOM_LANE0_KTI_REUT_REG,
                                                            PH_FOM_LANE1_KTI_REUT_REG,
                                                            PH_FOM_LANE2_KTI_REUT_REG,
                                                            PH_FOM_LANE3_KTI_REUT_REG,
                                                            PH_FOM_LANE4_KTI_REUT_REG,
                                                            PH_FOM_LANE5_KTI_REUT_REG,
                                                            PH_FOM_LANE6_KTI_REUT_REG,
                                                            PH_FOM_LANE7_KTI_REUT_REG,
                                                            PH_FOM_LANE8_KTI_REUT_REG,
                                                            PH_FOM_LANE9_KTI_REUT_REG,
                                                            PH_FOM_LANE10_KTI_REUT_REG,
                                                            PH_FOM_LANE11_KTI_REUT_REG,
                                                            PH_FOM_LANE12_KTI_REUT_REG,
                                                            PH_FOM_LANE13_KTI_REUT_REG,
                                                            PH_FOM_LANE14_KTI_REUT_REG,
                                                            PH_FOM_LANE15_KTI_REUT_REG,
                                                            PH_FOM_LANE16_KTI_REUT_REG,
                                                            PH_FOM_LANE17_KTI_REUT_REG,
                                                            PH_FOM_LANE18_KTI_REUT_REG,
                                                            PH_FOM_LANE19_KTI_REUT_REG,
                                                            PH_FOM_LANE20_KTI_REUT_REG,
                                                            PH_FOM_LANE21_KTI_REUT_REG,
                                                            PH_FOM_LANE22_KTI_REUT_REG,
                                                            PH_FOM_LANE23_KTI_REUT_REG
                                                          };
  PH_FOM_LANE0_KTI_REUT_STRUCT      KtiPhFomLane;

  KtiPhFomLane.Data = UsraCsrRead (SocketId, Port, KtiPhFomLanesRegOffset[Lane]);
  if (KtiPhFomLane.Bits.sleffmvalid == 1) {
    *FomOfLane = (UINT8) KtiPhFomLane.Bits.sleffm;
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Set the ctxcoefficientperlane of PH_TCL_LANE

  @param SocketId   - Socket ID
  @param Port       - Port number
  @param Lane       - Lane number
  @param Data       - value write to ctxcoefficientperlane

  @retval

**/
VOID
EFIAPI
SetKtiPhTclLane (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port,
  IN  UINT8                    Lane,
  IN  UINT32                   Data
  )
{
  UINT32 KtiPhTclLanesRegOffset[MAX_LANES_PER_LINK_SPR] = {
                                                            PH_TCL_LANE0_KTI_REUT_REG,
                                                            PH_TCL_LANE1_KTI_REUT_REG,
                                                            PH_TCL_LANE2_KTI_REUT_REG,
                                                            PH_TCL_LANE3_KTI_REUT_REG,
                                                            PH_TCL_LANE4_KTI_REUT_REG,
                                                            PH_TCL_LANE5_KTI_REUT_REG,
                                                            PH_TCL_LANE6_KTI_REUT_REG,
                                                            PH_TCL_LANE7_KTI_REUT_REG,
                                                            PH_TCL_LANE8_KTI_REUT_REG,
                                                            PH_TCL_LANE9_KTI_REUT_REG,
                                                            PH_TCL_LANE10_KTI_REUT_REG,
                                                            PH_TCL_LANE11_KTI_REUT_REG,
                                                            PH_TCL_LANE12_KTI_REUT_REG,
                                                            PH_TCL_LANE13_KTI_REUT_REG,
                                                            PH_TCL_LANE14_KTI_REUT_REG,
                                                            PH_TCL_LANE15_KTI_REUT_REG,
                                                            PH_TCL_LANE16_KTI_REUT_REG,
                                                            PH_TCL_LANE17_KTI_REUT_REG,
                                                            PH_TCL_LANE18_KTI_REUT_REG,
                                                            PH_TCL_LANE19_KTI_REUT_REG,
                                                            PH_TCL_LANE20_KTI_REUT_REG,
                                                            PH_TCL_LANE21_KTI_REUT_REG,
                                                            PH_TCL_LANE22_KTI_REUT_REG,
                                                            PH_TCL_LANE23_KTI_REUT_REG
                                                          };
  PH_TCL_LANE0_KTI_REUT_STRUCT      KtiPhTclLane;

  KtiPhTclLane.Data = UsraCsrRead (SocketId, Port, KtiPhTclLanesRegOffset[Lane]);
  KtiPhTclLane.Bits.ctxcoefficientperlane = Data;
  KtiPhTclLane.Bits.ctclvalid = 1;

  UsraCsrWrite (SocketId, Port, KtiPhTclLanesRegOffset[Lane], KtiPhTclLane.Data);
}

/**
Get KTI Link Frequency

@param SocketId              - Socket Id to program
@param Port                  - Port to program

@retval KTI Pipe rate for the KTI port
**/
UINT8
EFIAPI
GetKtiPipePhRate (
  IN UINT8                    SocketId,
  IN UINT8                    Port
  )
{
  PIPE_CLK_RATE_CTRL_KTI_REUT_STRUCT PipeClkRateCtrl;
  PipeClkRateCtrl.Data = UsraCsrRead (SocketId, Port, PIPE_CLK_RATE_CTRL_KTI_REUT_REG);
  return (UINT8) PipeClkRateCtrl.Bits.cri_freq_select;
}