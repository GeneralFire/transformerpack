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
#include <Library/MemoryCoreLib.h>
#include <Guid/KtiIpInterface.h>
#include <Library/IoAccessLib.h>
#include <Library/KtiApi.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <KtiSetupDefinitions.h>
#include <Library/PreSiliconEnvDetectLib.h>

/**
  Tell the link to report L1 state to the PCU
  Called from DisableLinkForceToLowPower(), KtiPhyLinkAfterWarmReset()

  @param SocketId,
  @param Port

  @retval None
**/
VOID
EFIAPI
SetUpiLinkToL1 (
  UINT8  SocketId,
  UINT8  Port
  )
{
  KTIPHYPWRCTRL_KTI_REUT_STRUCT KtiPhyPwrCtrl;
  KtiPhyPwrCtrl.Data = UsraCsrRead (SocketId, Port, KTIPHYPWRCTRL_KTI_REUT_REG);
  KtiPhyPwrCtrl.Bits.fake_l1 = 1;
  DEBUG ((DEBUG_INFO, "\n  Socket %u Port %u : KTIPHYPWRCTRL write 0x%x\n", SocketId, Port, KtiPhyPwrCtrl.Data));
  UsraCsrWrite (SocketId, Port, KTIPHYPWRCTRL_KTI_REUT_REG, KtiPhyPwrCtrl.Data);
}

/**
  Set c_init_begin to the value
  Called from DisableLinkForceToLowPower()

  @param SocketId,
  @param Port
  @param Value

  @retval None
**/
VOID
EFIAPI
SetCInitBegin (
  UINT8  SocketId,
  UINT8  Port,
  UINT8  Value
  )
{
  KTIREUT_PH_CTR1_KTI_REUT_STRUCT KtiReutPhCtr1;
  KtiReutPhCtr1.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG);
  KtiReutPhCtr1.Bits.c_init_begin = Value;
  DEBUG ((DEBUG_INFO, "\n  Socket %u Port %u : KTIREUT_PH_CTR1 write 0x%x\n", SocketId, Port, KtiReutPhCtr1.Data));
  UsraCsrWrite (SocketId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG, KtiReutPhCtr1.Data);
}

/**
  Set cp_reset
  Called from DisableLinkForceToLowPower() and IssuePhyResetOnCpuLink()

  @param SocketId,
  @param Port

  @retval None
**/
VOID
EFIAPI
SetCpReset (
  UINT8  SocketId,
  UINT8  Port
  )
{
  KTIREUT_PH_CTR1_KTI_REUT_STRUCT KtiReutPhCtr1;
  KtiReutPhCtr1.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG);
  KtiReutPhCtr1.Bits.cp_reset = 1;
  DEBUG ((DEBUG_INFO, "\n  Socket %u Port %u : KTIREUT_PH_CTR1 write 0x%x\n", SocketId, Port, KtiReutPhCtr1.Data));
  UsraCsrWrite (SocketId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG, KtiReutPhCtr1.Data);
}

/**
  Get L0p/L1/FailoverEn capabilities
  Called from PrimeHostStructurePhyLinkLayer()

  @param SocketId,
  @param Port,
  @param *ccl0p,
  @param *ccl1,
  @param *ccfailover

  @retval None
**/
VOID
EFIAPI
GetKtiReutCapabilities (
  UINT8                        SocketId,
  UINT8                        Port,
  OUT UINT32                   *ccl0p,
  OUT UINT32                   *ccl1,
  OUT UINT32                   *ccfailover
  )
{
  KTIREUT_PH_CPR_KTI_REUT_STRUCT  KtiReutPhCpr;
  KtiReutPhCpr.Data  = UsraCsrRead (SocketId, Port, KTIREUT_PH_CPR_KTI_REUT_REG);

  *ccl0p            = KtiReutPhCpr.Bits.ccl0p;
  *ccl1             = KtiReutPhCpr.Bits.ccl1;
  *ccfailover       = KtiReutPhCpr.Bits.ccfailover;
}

/**
  Get current L0p/L1/FailoverEn settings
  Called from PrimeHostStructurePhyLinkLayer()

  @param SocketId,
  @param Port,
  @param *OutKtiLinkL0pEn,
  @param *OutKtiLinkL1En,
  @param *OutKtiFailoverEn

  @retval None
**/
VOID
EFIAPI
GetKtiReutCurrentSettings (
  UINT8                        SocketId,
  UINT8                        Port,
  OUT UINT8                    *OutKtiLinkL0pEn,
  OUT UINT8                    *OutKtiLinkL1En,
  OUT UINT8                    *OutKtiFailoverEn
  )
{
  KTIREUT_PH_CTR1_KTI_REUT_STRUCT KtiReutPhCtr1;
  KtiReutPhCtr1.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG);
  *OutKtiLinkL0pEn    = (UINT8) KtiReutPhCtr1.Bits.c_l0p_en;
  *OutKtiLinkL1En     = (UINT8) KtiReutPhCtr1.Bits.c_l1_en;
  *OutKtiFailoverEn   = (UINT8) KtiReutPhCtr1.Bits.c_failover_en;
}

/**
  Program late action register per CPU per link

  @param SocketId   - Socket ID
  @param Port  - Port number
  @param Value - Value to program

  @retval KTI_SUCCESS - on successful completion

**/
VOID
EFIAPI
ProgramLateActionOnCpuLink (
  UINT8                    SocketId,
  UINT8                    Port,
  UINT32                   Value
  )
{
  KTIREUT_PH_CTR1_KTI_REUT_STRUCT  KtiReutPhCtr1;
  KtiReutPhCtr1.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG);
  KtiReutPhCtr1.Bits.c_la_load_disable = Value;
  UsraCsrWrite (SocketId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG, KtiReutPhCtr1.Data);
}

/**
  Program values for KTIREUT_PH_CTR1 bitfields according to KTIVAR and KTISETUP parameters
  called by ProgramPhyLayerControlRegisters()

  @param Cpu             - Socket ID
  @param Link            - Port number
  @param ProgramCNumInit - controls whether c_numinit bit gets programmed
  @param ProgramCOpSpeed - controls whether c_op_speed bit gets programmed
  @param c_l0p_en        - CSR bitfield (of same name) value to be programmed
  @param c_l1_en         - CSR bitfield (of same name) value to be programmed
  @param c_failover_en   - CSR bitfield (of same name) value to be programmed
  @param c_numinit       - CSR bitfield (of same name) value to be programmed

  @retval None

**/
VOID
EFIAPI
ProgramKtiReutPhCtr1 (
  UINT8                    SocketId,
  UINT8                    Port,
  BOOLEAN                  ProgramCNumInit,
  BOOLEAN                  ProgramCOpSpeed,
  UINT32                   c_l0p_en,
  UINT32                   c_l1_en,
  UINT32                   c_failover_en,
  UINT32                   c_numinit
  )
{
  KTIREUT_PH_CTR1_KTI_REUT_STRUCT  KtiReutPhCtr1;
  //
  // Get control register
  //
  KtiReutPhCtr1.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG);

  //
  // Enable phy layer init begin
  //
  KtiReutPhCtr1.Bits.c_init_begin = 1;

  //
  // Enable fast mode
  //
  if (ProgramCOpSpeed) {
    KtiReutPhCtr1.Bits.c_op_speed = 1;
  }

  //
  // Setup L0p, L1 and failover based on output variables and DFX per-link overrides
  //
  KtiReutPhCtr1.Bits.c_l0p_en = c_l0p_en;
  KtiReutPhCtr1.Bits.c_l1_en = c_l1_en;
  KtiReutPhCtr1.Bits.c_failover_en = c_failover_en;
  if (ProgramCNumInit == TRUE) {
    KtiReutPhCtr1.Bits.c_numinit = c_numinit;
  }
  DEBUG ((DEBUG_INFO, "\n  Socket %u Port %u : KTIREUT_PH_CTR1 write 0x%x\n", SocketId, Port, KtiReutPhCtr1.Data));
  UsraCsrWrite (SocketId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG, KtiReutPhCtr1.Data);
}

/**
  Program values for KTIREUT_PH_CTR1 and KTIREUT_PH_CIS.
  called by KtiTransitionFullSpeedOnWarmReset()

  @param SocketId   - Socket ID
  @param Port  - Port number

  @retval NONE

**/
VOID
EFIAPI
ProgramKtiReutPhWA (
  UINT8                    SocketId,
  UINT8                    Port
  )
{
  KTIREUT_PH_CIS_KTI_REUT_STRUCT  KtiReutPhCis;
  KTIREUT_PH_CTR1_KTI_REUT_STRUCT KtiReutPhCtr1;

  KtiReutPhCtr1.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG);
  //
  // Enable fast mode
  //
  KtiReutPhCtr1.Bits.c_op_speed = 1;
  DEBUG ((DEBUG_INFO, "\n  Socket %u Port %u : KTIREUT_PH_CTR1 write 0x%x\n", SocketId, Port, KtiReutPhCtr1.Data));
  UsraCsrWrite (SocketId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG, KtiReutPhCtr1.Data);


  KtiReutPhCis.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_CIS_KTI_REUT_REG);
  //
  // Clear the OC calibration
  //
  KtiReutPhCis.Bits.sp_rx_calib = 1;
  DEBUG ((DEBUG_INFO, "\n  Socket %u Port %u : KTIREUT_PH_CIS write 0x%x\n", SocketId, Port, KtiReutPhCis.Data));
  UsraCsrWrite (SocketId, Port, KTIREUT_PH_CIS_KTI_REUT_REG, KtiReutPhCis.Data);
}

/**
  get s_op_speed value

  @param SocketId        - Socket ID
  @param Port       - Port number

  @retval 0 = Slow speed, 1 = Fast speed

**/
UINT8
EFIAPI
GetKtiSpeed (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port
  )
{
  UINT8                          KtiCurrentLinkSpeedMode;
  KTIREUT_PH_CIS_KTI_REUT_STRUCT KtiPhCis;

  KtiPhCis.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_CIS_KTI_REUT_REG);
  KtiCurrentLinkSpeedMode = (UINT8) KtiPhCis.Bits.s_op_speed; //  "op speed" field
  return KtiCurrentLinkSpeedMode;
}

/**
  Check the KTIREUT_PH_CIS.s_link_up bitfield value to see if the link is trained
  Called by IssuePhyResetOnCpuLink()

  @param SocketId    - Socket ID
  @param Port        - Port number

  @retval TRUE  - s_link_up from KTIREUT_PH_CIS == 3
  @retval FALSE - S_link_up from KTIREUT_PH_CIS != 3

**/
BOOLEAN
EFIAPI
IsLinkTrained (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port
  )
{
  KTIREUT_PH_CIS_KTI_REUT_STRUCT  KtiReutPhCis;
  KTIREUT_PH_CSS_KTI_REUT_STRUCT  KtiReutPhCss;
  KTIREUT_PH_CTR1_KTI_REUT_STRUCT KtiReutPhCtr1;
  UINT8                           *RegAddr;
  BOOLEAN                         PowerGood;

  //
  //Make sure no reset is taking place
  //
  KtiReutPhCtr1.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG);
  if (KtiReutPhCtr1.Bits.cp_reset == 1) {
    return FALSE;
  }

  //
  //  Polling KtiReutPhCis.Bits.s_link_up (Bit[16:15]) == 3
  //
  if (UbiosGenerationEnabled()) {
    RegAddr = (UINT8 *) UsraGetCsrRegisterAddress (SocketId, Port, KTIREUT_PH_CIS_KTI_REUT_REG);
    PowerGood = CheckCurrentReset (POST_RESET_POWERGOOD);
    EmulationPolling (RegAddr, (BIT15 | BIT16), 1, PowerGood);
  }

  KtiReutPhCis.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_CIS_KTI_REUT_REG);
  KtiReutPhCss.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_CSS_KTI_REUT_REG);

  if (KtiReutPhCis.Bits.s_link_up == 3 &&
      KtiReutPhCss.Bits.s_tx_state >= PHY_STATE_L0 &&
      KtiReutPhCss.Bits.s_rx_state >= PHY_STATE_L0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Check if the KTIREUT_PH_CSS.s_tx_state bitfield value matches the PhyStatus after phy reset when
  c_init_begin is cleared

  @param SocketId    - Socket ID
  @param Port        - Port number
  @param PhyStatus   - KTI Link Phy status

  @retval TRUE  - s_tx_state matches the PhyStatus
  @retval FALSE - s_tx_state doesn't match the PhyStatus

**/
BOOLEAN
EFIAPI
CheckLinkInPhyStatus (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port,
  IN  UINT8                    PhyStatus
  )
{
  KTIREUT_PH_CSS_KTI_REUT_STRUCT  KtiReutPhCss;
  UINT8                           *RegAddr;
  BOOLEAN                         PowerGood;
  UINT32                          LabelNumber;
  UINT32                          TimeoutCounter;

  //
  // txeq adaptation flow (polling L0 slow mode) need to extend the wait duration.
  //
  TimeoutCounter = 10000;   // 200ms delay in real silicon

  if (HybridSystemLevelEmulationEnabled() || IsSimicsEnvironment ()) {
    //Change the time out value to 20s for HSLE and Simics (WB model)
    TimeoutCounter = 1000000;
  }

  if (GetUbiosLoopbackLabelNumber (&LabelNumber) == EFI_SUCCESS) {
    PowerGood = CheckCurrentReset (POST_RESET_POWERGOOD);
    if (PowerGood) {
      DEBUG ((DEBUG_ERROR, "\nLABEL_c_mbox%d:\n", LabelNumber));
    } else {
      DEBUG ((DEBUG_ERROR, "\nLABEL_w_mbox%d:\n", LabelNumber));
    }
    SetUbiosLoopbackLabelNumber (LabelNumber + 1);
    RegAddr = (UINT8 *) UsraGetCsrRegisterAddress (SocketId, Port, KTIREUT_PH_CSS_KTI_REUT_REG);
    DEBUG ((DEBUG_ERROR, "mov eax, DWORD PTR ds:[0%8xh]\n", RegAddr));

    DEBUG ((DEBUG_ERROR, "and eax, 0ffh\n"));
    DEBUG ((DEBUG_ERROR, "cmp eax, 0%08xh\n", PhyStatus));
    if (PowerGood) {
      DEBUG ((DEBUG_ERROR, "jnz LABEL_c_mbox%d\n", LabelNumber));
    } else {
      DEBUG ((DEBUG_ERROR, "jnz LABEL_w_mbox%d\n", LabelNumber));
    }
  }

  do {
    KtiReutPhCss.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_CSS_KTI_REUT_REG);
    FixedDelayMicroSecond (20);
    TimeoutCounter--;
  } while ((KtiReutPhCss.Bits.s_tx_state != PhyStatus) && (TimeoutCounter > 0));

  if (TimeoutCounter == 0) {
    DEBUG ((EFI_D_ERROR, "\n\n Polling Kti Link status %x failed on Socket %x, Link %x! \n", PhyStatus, SocketId, Port));
    return FALSE;
  } else {
    return TRUE;
  }
}

/**
  Force Phy Layer readapt before full-speed transition
  Set sp_rx_calib AND Set sp_tx_calib
  Called from ProgramPhyLayerControlRegisters()

  @param SocketId        - Socket ID
  @param Port            - Port number

  @retval KTI_SUCCESS - on successful completion

**/
VOID
EFIAPI
ForcePhyLayerReadapt (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port
  )
{
  KTIREUT_PH_CIS_KTI_REUT_STRUCT  KtiReutPhCis;
  //
  // Force Phy Layer readapt before full-speed transition
  //
  KtiReutPhCis.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_CIS_KTI_REUT_REG);
  KtiReutPhCis.Bits.sp_rx_calib = 1;
  KtiReutPhCis.Bits.sp_tx_calib = 1;
  DEBUG ((DEBUG_INFO, "\n  Socket %u Port %u : KTIREUT_PH_CIS write 0x%x\n", SocketId, Port, KtiReutPhCis.Data));
  UsraCsrWrite (SocketId, Port, KTIREUT_PH_CIS_KTI_REUT_REG, KtiReutPhCis.Data);
}

/**
  Set Linear time value SKX-workaround s4929026, s5370781
  Called from ProgramPhyLayerControlRegisters()

  set:
  KtiReutPhTL0pExitRx.Bits.t_l0p_est = 0x27
  KtiReutPhTL0pExitTxAct.Bits.t_l0p_tx_exitend2l0c = 0x79;

  @param SocketId   - Socket ID
  @param Link       - Port number
  @param LinkSpeed  - Current link speed mode; 0 - Slow Speed; 1- Full Speed.

  @retval None

**/
VOID
EFIAPI
SetKtiL0pExitWorkaround (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port,
  IN  UINT8                    LinkSpeed
  )
{
  KTIREUT_PH_TL0PEXITRX_KTI_REUT_STRUCT    KtiReutPhTL0pExitRx;
  KTIREUT_PH_TL0PEXITTXACT_KTI_REUT_STRUCT KtiReutPhTL0pExitTxAct;
  KTIREUT_PH_TL0PEXITTXIDL_KTI_REUT_STRUCT KtiReutPhTL0pExitTxIdl;
  KTIREUT_PH_CSS_KTI_REUT_STRUCT           KtiReutPhCss;

  if (IsSiliconWorkaroundEnabled ("S4929026")) {
    KtiReutPhTL0pExitRx.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_TL0PEXITRX_KTI_REUT_REG);
    KtiReutPhTL0pExitRx.Bits.t_l0p_est = 0x27;
    UsraCsrWrite (SocketId, Port, KTIREUT_PH_TL0PEXITRX_KTI_REUT_REG, KtiReutPhTL0pExitRx.Data);
  }

  if (IsSiliconWorkaroundEnabled ("S5370781")) {
    KtiReutPhTL0pExitTxAct.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_TL0PEXITTXACT_KTI_REUT_REG);
    KtiReutPhTL0pExitTxAct.Bits.t_l0p_tx_exitend2l0c = 0x79;
    UsraCsrWrite (SocketId, Port, KTIREUT_PH_TL0PEXITTXACT_KTI_REUT_REG, KtiReutPhTL0pExitTxAct.Data);
  }

  if (IsSiliconWorkaroundEnabled ("S16010697617")) {
    if (LinkSpeed == KTI_LINK_SLOW_SPEED_MODE) {  //Link in slow mode
      KtiReutPhCss.Data = UsraCsrRead(SocketId, Port, KTIREUT_PH_CSS_KTI_REUT_REG);

      if ((KtiReutPhCss.Bits.s_tx_state == PHY_STATE_L0) && (KtiReutPhCss.Bits.s_rx_state == PHY_STATE_L0)) {  //Link in L0 state
        KtiReutPhTL0pExitRx.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_TL0PEXITRX_KTI_REUT_REG);
        KtiReutPhTL0pExitRx.Bits.t_l0p_est = 0xFF;
        UsraCsrWrite (SocketId, Port, KTIREUT_PH_TL0PEXITRX_KTI_REUT_REG, KtiReutPhTL0pExitRx.Data);

        KtiReutPhTL0pExitTxIdl.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_TL0PEXITTXIDL_KTI_REUT_REG);
        KtiReutPhTL0pExitTxIdl.Bits.t_l0p_tx_pat = 0xFF;
        UsraCsrWrite (SocketId, Port, KTIREUT_PH_TL0PEXITTXIDL_KTI_REUT_REG, KtiReutPhTL0pExitTxIdl.Data);
      }
    }
  }
}

/**
  Set Physical Determinism Control
  Called from ProgramPhyLayerControlRegisters()

  @param SocketId        - Socket ID
  @param Port       - Port number

  @retval KTI_SUCCESS - on successful completion

**/
VOID
EFIAPI
SetPhysicalDeterminismControl (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port
  )
{
  KTIREUT_PH_LDC_KTI_REUT_STRUCT           KtiReutPhLdc;
  //
  // Physical Determinism Control
  //
  KtiReutPhLdc.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_LDC_KTI_REUT_REG);
  KtiReutPhLdc.Bits.targetlinklatency = 0x0;
  KtiReutPhLdc.Bits.inidriftbufflatency = 0x8;
  KtiReutPhLdc.Bits.driftbufalarmthr = 0x4;
  DEBUG ((DEBUG_INFO, "\n  Socket %u Port %u : KTIREUT_PH_LDC write 0x%x\n", SocketId, Port, KtiReutPhLdc.Data));
  UsraCsrWrite (SocketId, Port, KTIREUT_PH_LDC_KTI_REUT_REG, KtiReutPhLdc.Data);
}

/**
  Get sp_use_back_channel value

  @param SocketId   - Socket ID
  @param Port       - Port number

  @retval: the value of sp_use_back_channel

**/
UINT8
EFIAPI
GetKtiSpUseBackChannel (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port
  )
{
  UINT8                          SpUseBackChannel;
  KTIREUT_PH_CIS_KTI_REUT_STRUCT KtiPhCis;

  KtiPhCis.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_CIS_KTI_REUT_REG);
  SpUseBackChannel = (UINT8) KtiPhCis.Bits.sp_use_back_channel;
  return SpUseBackChannel;
}

/**
  Set sp_use_back_channel value

  @param SocketId   - Socket ID
  @param Port       - Port number
  @param Data       - Value write to SpUseBackChannel

  @retval

**/
VOID
EFIAPI
SetKtiSpUseBackChannel (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port,
  IN  UINT8                    Data
  )
{
  KTIREUT_PH_CIS_KTI_REUT_STRUCT KtiPhCis;

  KtiPhCis.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_CIS_KTI_REUT_REG);
  KtiPhCis.Bits.sp_use_back_channel = Data;

  UsraCsrWrite (SocketId, Port, KTIREUT_PH_CIS_KTI_REUT_REG, KtiPhCis.Data);
}

/**
  Check if the KTIREUT_PH_CIS.sp_use_back_channel bitfield value be set to 1 by HW in TxeqAdaptation flow

  @param SocketId    - Socket ID
  @param Port        - Port number

  @retval TRUE  - sp_use_back_channel is set to 1 by HW
  @retval FALSE - Timeout

**/
BOOLEAN
EFIAPI
PollKtiSpUseBackChannel (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port
  )
{
  KTIREUT_PH_CIS_KTI_REUT_STRUCT  KtiPhCis;
  UINT8                           *RegAddr;
  BOOLEAN                         PowerGood;
  UINT32                          TimeoutCounter;

  TimeoutCounter = 1500;   // 30ms delay in real silicon

  if (HybridSystemLevelEmulationEnabled() || IsSimicsEnvironment ()) {
    //Change the time out value to 20s for HSLE and Simics (WB model)
    TimeoutCounter = 1000000;
  }

  if (UbiosGenerationEnabled()) {
    RegAddr = (UINT8 *) UsraGetCsrRegisterAddress (SocketId, Port, KTIREUT_PH_CIS_KTI_REUT_REG);
    PowerGood = CheckCurrentReset (POST_RESET_POWERGOOD);
    EmulationPolling (RegAddr, BIT6, 1, PowerGood);
  }

  do {
    KtiPhCis.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_CIS_KTI_REUT_REG);
    FixedDelayMicroSecond (20);    //20us
    TimeoutCounter--;
  } while ((KtiPhCis.Bits.sp_use_back_channel != 1) && (TimeoutCounter > 0));

  if (TimeoutCounter == 0) {
    DEBUG ((EFI_D_ERROR, "\n\n Polling Kti sp_use_back_channel failed on Socket %x, Link %x! \n", SocketId, Port));
    return FALSE;
  } else {
    return TRUE;
  }
}

/**
  Set the Start_Test Bit and exploopcnt (optional) for the txeq adaptation flow

  @param SocketId   - Socket ID
  @param Port       - Port number
  @param ExpLoopCnt - Exponential Loop Count Exponential Test Loop Count.

  @retval

**/
VOID
EFIAPI
SetKtiPhTtpgc (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port,
  IN  UINT8                    *ExpLoopCnt OPTIONAL
  )
{
  KTIREUT_PH_TTPGC_KTI_REUT_STRUCT           KtiPhTtpgc;

  KtiPhTtpgc.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_TTPGC_KTI_REUT_REG);
  KtiPhTtpgc.Bits.strttst = 1;

  if ((ExpLoopCnt != NULL) && (*ExpLoopCnt != KtiPhTtpgc.Bits.exploopcnt)) {
    KtiPhTtpgc.Bits.exploopcnt = *ExpLoopCnt;
  }
  UsraCsrWrite (SocketId, Port, KTIREUT_PH_TTPGC_KTI_REUT_REG, KtiPhTtpgc.Data);
}

/**
  Set init_check_mask for the txeq adaptation flow

  @param SocketId      - Socket ID
  @param Port          - Port number
  @param InitCheckMask - Initial Check Mask

  @retval

**/
VOID
EFIAPI
SetKtiPhTtpcc (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port,
  IN  UINT8                    InitCheckMask
  )
{
  KTIREUT_PH_TTPCC_KTI_REUT_STRUCT           KtiPhTtpcc;

  KtiPhTtpcc.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_TTPCC_KTI_REUT_REG);
  KtiPhTtpcc.Bits.init_check_mask = InitCheckMask;
  UsraCsrWrite (SocketId, Port, KTIREUT_PH_TTPCC_KTI_REUT_REG, KtiPhTtpcc.Data);
}

/**
  Set c_init value

  @param SocketId   - Socket ID
  @param Port       - Port number
  @param Data       - Value write to c_init

  @retval

**/
VOID
EFIAPI
SetKtiCInit (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port,
  IN  UINT8                    Data
  )
{
  KTIREUT_PH_CTR1_KTI_REUT_STRUCT KtiPhCtr1;

  KtiPhCtr1.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG);
  KtiPhCtr1.Bits.c_init = Data;
  UsraCsrWrite (SocketId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG, KtiPhCtr1.Data);
}

/**
  Set t_tdst for the slow mode DET_TX secondary timer

  @param SocketId      - Socket ID
  @param Port          - Port number
  @param Ttdst         - Time for Squelch exit detect/debounce

  @retval

**/
VOID
EFIAPI
SetKtiPhTsecSl (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port,
  IN  UINT8                    Ttdst
  )
{
  KTIREUT_PH_TSEC_SL_KTI_REUT_STRUCT           KtiPhTsecSl;

  KtiPhTsecSl.Data = UsraCsrRead (SocketId, Port, KTIREUT_PH_TSEC_SL_KTI_REUT_REG);
  KtiPhTsecSl.Bits.t_tdst = Ttdst;
  UsraCsrWrite (SocketId, Port, KTIREUT_PH_TSEC_SL_KTI_REUT_REG, KtiPhTsecSl.Data);
}

#if defined (IA32)
/**

Polls for cp_reset to complete

@param SocId             - CPU Socket Number, 0 based
@param Port              - CPU Port Number, 0 based

@retval EFI_SUCCESS      - cp_reset was cleared
@retval EFI_DEVICE_ERROR - timeout waiting for cp_reset bit to clear
**/
EFI_STATUS
EFIAPI
KtiPollForLinkResetCpl (
  IN UINT8                      SocId,
  IN UINT8                      Port
  )
{
  KTIREUT_PH_CTR1_KTI_REUT_STRUCT KtiReutPhCtr1;
  UINT8                           WaitTryCount;

  // Wait for the link to train
  for (WaitTryCount = 0; WaitTryCount <= 5; WaitTryCount++) {
    KtiReutPhCtr1.Data = UsraCsrRead (SocId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG);

    if (KtiReutPhCtr1.Bits.cp_reset == 0) {
      return EFI_SUCCESS;
    }
    FixedDelayMicroSecond (100);
  }

  return EFI_DEVICE_ERROR;
}

/**
Clear UPI link errors for SlowMode WA, called per-link

@param SocketId        - Socket ID
@param Link            - Link number

**/
VOID
EFIAPI
ClearSlowModeWaLinkErrors (
  IN UINT8                      Socket,
  IN UINT8                      Link
  )
{
  UsraCsrWrite (Socket, Link, BIOS_KTI_ERR_ST_N0_KTI_LLPMON_REG, 0);
  UsraCsrWrite (Socket, Link, BIOS_KTI_ERR_ST_N1_KTI_LLPMON_REG, 0);
  UsraCsrWrite (Socket, Link, KTICRCERRCNT_KTI_LLPMON_REG, 0x7fffffff);
}

/**

Checks the link in slow mode and attempts to retrain if link failed

@param SocId             - CPU Socket Number, 0 based
@param Port              - CPU Port Number, 0 based

@retval TRUE             - Repaired
@retval FALSE            - Failed to repaired

**/
BOOLEAN
EFIAPI
ApplySlowModePhyRepair (
  UINT8                      SocId,
  UINT8                      Port
  )
{
  KTIREUT_PH_PSS_KTI_REUT_STRUCT  KtiReutPhPss;
  KTIREUT_PH_CTR1_KTI_REUT_STRUCT KtiReutPhCtr1;
  KTIREUT_PH_PIS_KTI_REUT_STRUCT  KtiReutPhPis;
  KTIREUT_PH_CSS_KTI_REUT_STRUCT  KtiReutPhCss;
  KTICRCERRCNT_KTI_LLPMON_STRUCT  KtiCrcErrCnt;
  BOOLEAN                         LinkRepaired;
  UINT8                           RetryCount;
  UINT8                           Idx1;

  LinkRepaired = FALSE;
  RetryCount = 20;

  KtiReutPhPss.Data = UsraCsrRead (SocId, Port, KTIREUT_PH_PSS_KTI_REUT_REG);
  KtiReutPhCss.Data = UsraCsrRead (SocId, Port, KTIREUT_PH_CSS_KTI_REUT_REG);
  KtiCrcErrCnt.Data = UsraCsrRead (SocId, Port, KTICRCERRCNT_KTI_LLPMON_REG);

  while ((LinkRepaired == FALSE) && (RetryCount > 0)) {

    RetryCount = RetryCount - 1;

    KtiReutPhCtr1.Data = UsraCsrRead (SocId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG);
    KtiReutPhCtr1.Bits.c_init_begin = 0;
    UsraCsrWrite (SocId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG, KtiReutPhCtr1.Data);

    KtiReutPhCtr1.Data = UsraCsrRead (SocId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG);
    KtiReutPhCtr1.Bits.cp_reset = 1;
    UsraCsrWrite (SocId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG, KtiReutPhCtr1.Data);

    if (KtiPollForLinkResetCpl (SocId, Port) == KTI_FAILURE) {
      break;
    }

    KtiReutPhCtr1.Data = UsraCsrRead (SocId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG);
    KtiReutPhCtr1.Bits.c_op_speed = 1;
    UsraCsrWrite (SocId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG, KtiReutPhCtr1.Data);

    KtiReutPhCtr1.Data = UsraCsrRead (SocId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG);
    KtiReutPhCtr1.Bits.cp_reset = 1;
    UsraCsrWrite (SocId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG, KtiReutPhCtr1.Data);

    if (KtiPollForLinkResetCpl (SocId, Port) == KTI_FAILURE) {
      return LinkRepaired;
    }
    KtiReutPhCtr1.Data = UsraCsrRead (SocId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG);
    KtiReutPhCtr1.Bits.c_op_speed = 0;
    UsraCsrWrite (SocId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG, KtiReutPhCtr1.Data);

    KtiReutPhCtr1.Data = UsraCsrRead (SocId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG);
    KtiReutPhCtr1.Bits.cp_reset = 1;
    UsraCsrWrite (SocId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG, KtiReutPhCtr1.Data);

    if (KtiPollForLinkResetCpl (SocId, Port) == KTI_FAILURE) {
      break;
    }

    //
    // Clear previous status
    //
    UsraCsrWrite (SocId, Port, KTIREUT_PH_PIS_KTI_REUT_REG, 0xFFFFFFFF);
    UsraCsrWrite (SocId, Port, KTICRCERRCNT_KTI_LLPMON_REG, 0x7fffffff);

    //
    // Restart link training
    //
    KtiReutPhCtr1.Data = UsraCsrRead (SocId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG);
    KtiReutPhCtr1.Bits.c_init_begin = 1;
    UsraCsrWrite (SocId, Port, KTIREUT_PH_CTR1_KTI_REUT_REG, KtiReutPhCtr1.Data);

    //
    //wait for link to train or fail
    //retry full sequence if failed
    //detect and wait
    //
    KtiReutPhCss.Data = 0;
    KtiReutPhPis.Data = 0;
    Idx1 = 10;

    while ((Idx1 > 0) && (KtiReutPhCss.Bits.s_tx_state != PHY_STATE_L0) && (KtiReutPhCss.Bits.s_tx_state != PHY_STATE_L0C) && (KtiReutPhPis.Bits.s_init_fail_p != 1)) {
      FixedDelayMicroSecond (100);
      KtiReutPhCss.Data = UsraCsrRead (SocId, Port, KTIREUT_PH_CSS_KTI_REUT_REG);
      KtiReutPhPis.Data = UsraCsrRead (SocId, Port, KTIREUT_PH_PIS_KTI_REUT_REG);
      Idx1 = Idx1 - 1;
    }

    KtiCrcErrCnt.Data = 0;
    if (((KtiReutPhCss.Bits.s_tx_state == PHY_STATE_L0) || (KtiReutPhCss.Bits.s_tx_state == PHY_STATE_L0C)) && (KtiCrcErrCnt.Bits.counter < 10)) {
      LinkRepaired = TRUE;
    }
  }

  return LinkRepaired;
}

/**
Check if this link trained to L0

@param SocId             - CPU Socket Number, 0 based
@param Port              - CPU Port Number, 0 based

@retval EFI_SUCCESS      - UPI Link trained
@retval EFI_DEVICE_ERROR - UPI Link failed to train in slow mode

**/
EFI_STATUS
EFIAPI
IsLinkTrainedInSlowMode (
  UINT8                      SocId,
  UINT8                      Port
  )
{
  KTIREUT_PH_PSS_KTI_REUT_STRUCT  KtiReutPhPss;
  KTIREUT_PH_CSS_KTI_REUT_STRUCT  KtiReutPhCss;
  KTICRCERRCNT_KTI_LLPMON_STRUCT  KtiCrcErrCnt;

  KtiReutPhPss.Data = UsraCsrRead (SocId, Port, KTIREUT_PH_PSS_KTI_REUT_REG);
  KtiReutPhCss.Data = UsraCsrRead (SocId, Port, KTIREUT_PH_CSS_KTI_REUT_REG);
  KtiCrcErrCnt.Data = UsraCsrRead (SocId, Port, KTICRCERRCNT_KTI_LLPMON_REG);

  DEBUG ((DEBUG_INFO, "S%d P%d KtiReutPhPss=%x, KtiReutPhCss=%x, KtiCrcErrCnt=%x\n",
          SocId, Port, KtiReutPhPss.Data, KtiReutPhCss.Data, KtiCrcErrCnt.Data));

  if (((KtiReutPhPss.Bits.s_rx_state_p != PHY_STATE_RESETP) && (KtiReutPhCss.Bits.s_tx_state != PHY_STATE_L0) && (KtiReutPhCss.Bits.s_tx_state != PHY_STATE_L0C))) {
    return EFI_DEVICE_ERROR;
  } else {
    return EFI_SUCCESS;
  }
}

/**
Check if other side of link recovered

@param SocId             - CPU Socket Number, 0 based
@param Port              - CPU Port Number, 0 based

@retval TRUE             - other side of link recovered, force use of KTILS
@retval FALSE            - link is fine, return
**/
BOOLEAN
EFIAPI
IsOtherSideOfLinkRecovered (
  UINT8                      SocId,
  UINT8                      Port
  )
{
  KTIPCSTS_KTI_LLPMON_STRUCT      PcSts;
  //
  // Check for cases of other side of link recovered and force use of KTILS
  //
  PcSts.Data = UsraCsrRead (SocId, Port, KTIPCSTS_KTI_LLPMON_REG);
  if (PcSts.Bits.ll_status < LINK_SM_CRD_RETURN_STALL) {
    return TRUE;
  } else {
    //Link is fine, return
    DEBUG ((DEBUG_INFO, " Return FAILURE, PcSts=%x\n", PcSts.Data));
    return FALSE;
  }
}

/**
Check for any indications of a link layer problem

@param SocId             - CPU Socket Number, 0 based
@param Port              - CPU Port Number, 0 based

@retval TRUE             - Link layer problem detected
@retval FALSE            - Link layer problem not detected
**/
BOOLEAN
EFIAPI
DetectLinkLayerProblem (
  UINT8                      SocId,
  UINT8                      Port
  )
{
  KTILS_KTI_LLPMON_STRUCT         KtiLs;
  KTIACKCNT_KTI_CIOPHYDFX_STRUCT  KtiAckCnt;
  KTICTRTXPF_KTI_LLPMON_STRUCT    KtiCtrTxPf;

  KtiLs.Data = UsraCsrRead (SocId, Port, KTILS_KTI_LLPMON_REG);
  KtiAckCnt.Data = UsraCsrRead (SocId, Port, KTIACKCNT_KTI_CIOPHYDFX_REG);
  KtiCtrTxPf.Data = UsraCsrRead (SocId, Port, KTICTRTXPF_KTI_LLPMON_REG);

  DEBUG ((DEBUG_INFO, "\n KtiLs=%x KtiAckCnt=%x\n", KtiLs.Data, KtiAckCnt.Data));

  //
  // Check for indications of LL problem, and apply phy wa if not previously applied
  // 0x4=Retry_Abort
  //
  if (((KtiLs.Bits.init_state != LINK_SM_INIT_DONE) && (KtiLs.Bits.init_state != LINK_SM_NOT_RDY_FOR_INIT)) || (KtiLs.Bits.retry_state == 0x4) || (((KtiAckCnt.Bits.llwptr == KtiLs.Bits.link_layer_retry_queue_consumed)) && (KtiCtrTxPf.Bits.count_tx_protocol_flit < 32))) {
    //Link layer problem detected
    return TRUE;
  }
  return FALSE;
}
#endif //IA32
