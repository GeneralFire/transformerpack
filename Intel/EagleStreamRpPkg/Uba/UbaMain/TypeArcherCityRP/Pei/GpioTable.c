/** @file

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

#include "PeiBoardInit.h"
#include <Library/UbaGpioUpdateLib.h>

#include <Library/UbaGpioInitLib.h>
#include <Library/PcdLib.h>

//
// Board     : Archer City RP
//

static GPIO_INIT_CONFIG mGpioTableArcherCityRP [] =
{
/*
    {GPIO_VER3_GPP_A0,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioLockDefault}},//GPP_A_0_IRQ_LPC_PIRQA_N_ESPI_ALERT0_N
    {GPIO_VER3_GPP_A1,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioLockDefault}},//GPP_A_1_ESPI_ALERT1_N_LPC_RCIN_N
    {GPIO_VER3_GPP_A2,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioLockDefault}},//GPP_A_2_ESPI_IO0_LPC_LAD0
    {GPIO_VER3_GPP_A3,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_A_3_ESPI_IO1_LPC_LAD1
    {GPIO_VER3_GPP_A4,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_A_4_ESPI_IO2_LPC_LAD2
    {GPIO_VER3_GPP_A5,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_A_5_ESPI_IO3_LPC_LAD3
    {GPIO_VER3_GPP_A6,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_A_6_ESPI_CS0_N_LPC_LFRAME_N
    {GPIO_VER3_GPP_A7,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_A_7_IRQ_LPC_SERIRQ_ESPI_CS1_N
    {GPIO_VER3_GPP_A8,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_A_8_RST_ESPI_RESET_N
    {GPIO_VER3_GPP_A9,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_A_9_CLK_24M_66M_LPC0_ESPI
    {GPIO_VER3_GPP_A10,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_A_10_TP_GPP_A_10
    {GPIO_VER3_GPP_A11,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPPC_A_11_HP_LVC3_SLOTC_PWRGD_N
    {GPIO_VER3_GPP_A12,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_A_12_FM_CLKREQ_M2_N
    {GPIO_VER3_GPP_A13,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_A_13_TP_GPP_A_13
    {GPIO_VER3_GPP_A14,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_A_14_FM_CLKREQ_NIC1_N
    {GPIO_VER3_GPP_A15,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_A_15_FM_CLKREQ_NIC2_N
    {GPIO_VER3_GPP_A16,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirInOut,    GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_A_16_FM_PCH_SATA_RAID_KEY
    {GPIO_VER3_GPP_A17,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPPC_A_17_FM_BMC_PCH_SPARE
    {GPIO_VER3_GPP_A18,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_A_18_FM_CPU_INTERPOSER_SEL1
    {GPIO_VER3_GPP_A19,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_A_19_FM_CPU_INTERPOSER_SEL2
    {GPIO_VER3_GPP_B0,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_B_0_FM_PCH_GSXDOUT
    {GPIO_VER3_GPP_B1,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutHigh,    GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_B_1_FM_PCH_GSXSLOAD
    {GPIO_VER3_GPP_B2,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_B_2_FM_PCH_GSXDIN
    {GPIO_VER3_GPP_B3,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_B_3_FM_PCH_GSXRESET
    {GPIO_VER3_GPP_B4,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_B_4_FM_PCH_GSXCLK
    {GPIO_VER3_GPP_B5,       { GpioPadModeNative4, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_B_5_TRC_PCH_PTI4
    {GPIO_VER3_GPP_B6,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_B_6_FM_OC1_USB_N
    {GPIO_VER3_GPP_B7,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_B_7_FM_OC2_USB_N
    {GPIO_VER3_GPP_B8,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_B_8_FM_OC3_USB_N
    {GPIO_VER3_GPP_B9,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_B_9_FM_OC4_USB_N
    {GPIO_VER3_GPP_B10,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_B_10_FM_OC5_USB_N
    {GPIO_VER3_GPP_B11,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_B_11_FM_OC6_USB_N
    {GPIO_VER3_GPP_B16,      { GpioPadModeNative4, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_B_16_TRC_PCH_PTI9
    {GPIO_VER3_GPP_B17,      { GpioPadModeNative4, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_B_17_TRC_PCH_PTI10
    {GPIO_VER3_GPP_B18,      { GpioPadModeNative4, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_B_18_TRC_PCH_PTI11
    {GPIO_VER3_GPP_B19,      { GpioPadModeNative4, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_B_19_TRC_PCH_PTI12
    {GPIO_VER3_GPP_B20,      { GpioPadModeNative4, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_B_20_TRC_PCH_PTI13
    {GPIO_VER3_GPP_B21,      { GpioPadModeNative4, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_B_21_TRC_PCH_PTI14
    {GPIO_VER3_GPP_B22,      { GpioPadModeNative4, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_B_22_TRC_PCH_PTI15
    {GPIO_VER3_GPP_B23,      { GpioPadModeNative4, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_B_23_TRC_PCH_PTI1_CLK
    {GPIO_VER3_GPP_C2,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_C_2_IRQ_SML0_ALERT_N
    {GPIO_VER3_GPP_C5,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_C_5_RST_SMB_HOST_PCH_MUX_N
    {GPIO_VER3_GPP_C17,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_C_17_FM_BIOS_POST_CMPLT_N
    {GPIO_VER3_GPP_C18,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_C_18_FM_PLD_PCH_DATA
    {GPIO_VER3_GPP_C19,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_C_19_SMB_PCH_PEHP_LVC3_R_SCL
    {GPIO_VER3_GPP_C20,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_C_20_SMB_PCH_PEHP_LVC3_R_SDA
    {GPIO_VER3_GPP_C21,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_C_21_FM_PCH_PEHP_LVC3_ALERT_N
    {GPIO_VER3_GPP_D2,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_D_2_TP_PCH_GPP_D2
    {GPIO_VER3_GPP_D3,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_D_3_TP_Inner_GPP_D_3
    {GPIO_VER3_GPP_D4,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_D_4_TP_Inner_GPP_D_4
    {GPIO_VER3_GPP_D5,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_D_5_TP_Inner_GPP_D_5
    {GPIO_VER3_GPP_D6,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_D_6_FM_FLASH_SECURITY_STRAP
    {GPIO_VER3_GPP_D7,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_D_7_FM_ESPI_FLASH_MODE
    {GPIO_VER3_GPP_D8,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_D_8_FM_CRASHLOG_TRIG_N
    {GPIO_VER3_GPP_D9,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_D_9_FM_LPC_PME_N
    {GPIO_VER3_GPP_D10,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_D_10_IRQ_PCH_SCI_WHEA_N
    {GPIO_VER3_GPP_D11,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_D_11_RST_PLTRST_N
    {GPIO_VER3_GPP_D14,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_D_14_FM_ADR_TRIGGER_R_N
#ifdef SPR_HOST
    {GPIO_VER3_GPP_D15,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_D_15_PU_PCH_VRALERT_N
#else
    {GPIO_VER3_GPP_D15,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_D_15_PU_PCH_VRALERT_N
#endif //SPR_HOST
    {GPIO_VER3_GPP_D16,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutHigh,    GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_D_16_FM_ADR_ACK
    {GPIO_VER3_GPP_D17,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_D_17_FM_PLT_BMC_THERMTRIP_R_N
    {GPIO_VER3_GPP_D20,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_D_20_TP_GPP_D_20
    {GPIO_VER3_GPP_D21,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_D_21_FM_PCH_GLB_RST_WARN_N
    {GPIO_VER3_GPP_D22,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_D_22_FM_OC7_USB_N
    {GPIO_VER3_GPP_D23,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutHigh,    GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_D_23_FM_PCH_BOOT_BIOS_STRAP
    {GPIO_VER3_GPP_E0,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_E_0_TP_PCH_GPP_E_0
    {GPIO_VER3_GPP_E1,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_E_1_TP_PCH_GPP_E_1
    {GPIO_VER3_GPP_E2,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_E_2_TP_PCH_GPP_E_2
    {GPIO_VER3_GPP_E3,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutHigh,    GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_E_3_FM_SSATA_PCIE_M2_1_SEL
    {GPIO_VER3_GPP_E4,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_E_4_TP_PCH_GPP_E_4
    {GPIO_VER3_GPP_E5,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_E_5_TP_PCH_GPP_E_5
    {GPIO_VER3_GPP_E6,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_E_6_TP_PCH_GPP_E_6
    {GPIO_VER3_GPP_E7,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_E_7_TP_PCH_GPP_E_7
    {GPIO_VER3_GPP_E10,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_E_10_SGPIO_SATA0_DATAOUT
    {GPIO_VER3_GPP_E12,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutHigh,    GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_E_12_SGPIO_SATA1_LOAD
    {GPIO_VER3_GPP_E14,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_E_14_FP_PWR_LED_N
    {GPIO_VER3_GPP_E15,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutHigh,    GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_E_15_FM_CPU_RMCA_CATERR_DLY_N
    {GPIO_VER3_GPP_E16,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutHigh,    GpioIntLevel | GpioIntNmi,      GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_E_16_IRQ_BMC_PCH_NMI
    {GPIO_VER3_GPP_E19,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutHigh,    GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_E_19_FM_TPM_MOD_PRES_N
    {GPIO_VER3_GPP_E20,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_E_20_TP_Inner_GPP_E_20
    {GPIO_VER3_GPP_E21,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_E_21_TP_Inner_GPP_E_21
    {GPIO_VER3_GPP_E22,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_E_22_TP_Inner_GPP_E_22
    {GPIO_VER3_GPP_E23,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_E_23_TP_Inner_GPP_E_23
    {GPIO_VER3_GPP_H0,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutHigh,    GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_H_0_FM_PCH_VISA_DEFAULT
    {GPIO_VER3_GPP_H1,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutHigh,    GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_H_1_FM_PCH_IO_EXPANDER
    {GPIO_VER3_GPP_H2,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_H_2_TP_Inner_GPP_H_2
    {GPIO_VER3_GPP_H3,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_H_3_TP_Inner_GPP_H_3
    {GPIO_VER3_GPP_H4,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_H_4_TP_Inner_GPP_H_4
    {GPIO_VER3_GPP_H5,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_H_5_TP_Inner_GPP_H_5
    {GPIO_VER3_GPP_H6,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutHigh,    GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_H_6_FM_PCH_EXTERNALCLKMODE
    {GPIO_VER3_GPP_H7,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutHigh,    GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_H_7_FM_PCH_MCRO_LDO
    {GPIO_VER3_GPP_H8,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_H_8_TP_Inner_GPP_H_8
    {GPIO_VER3_GPP_H9,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_H_9_TP_Inner_GPP_H_9
    {GPIO_VER3_GPP_H10,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_H_10_TP_Inner_GPP_H_10
    {GPIO_VER3_GPP_H11,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_H_11_TP_Inner_GPP_H_11
    {GPIO_VER3_GPP_H12,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_H_12_TP_Inner_GPP_H_12
    {GPIO_VER3_GPP_H13,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_H_13_TP_Inner_GPP_H_13
    {GPIO_VER3_GPP_H14,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_H_14_TP_Inner_GPP_H_14
    {GPIO_VER3_GPP_H18,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_H_18_FM_PCH_PMCALERT_N
    {GPIO_VER3_GPP_H19,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_H_19_FM_LT_KEY_DOWNGRADE_N
    {GPIO_VER3_GPP_I0,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_I_0_TP_Inner_GPP_I_0
    {GPIO_VER3_GPP_I1,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_I_1_TP_Inner_GPP_I_1
    {GPIO_VER3_GPP_I2,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_I_2_TP_Inner_GPP_I_2
    {GPIO_VER3_GPP_I3,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_I_3_TP_Inner_GPP_I_3
    {GPIO_VER3_GPP_I4,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_I_4_TP_Inner_GPP_I_4
    {GPIO_VER3_GPP_I5,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_I_5_TP_Inner_GPP_I_5
    {GPIO_VER3_GPP_I6,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_I_6_TP_Inner_GPP_I_6
    {GPIO_VER3_GPP_I7,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_I_7_TP_Inner_GPP_I_7
    {GPIO_VER3_GPP_I8,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_I_8_TP_Inner_GPP_I_8
    {GPIO_VER3_GPP_I9,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_I_9_TP_Inner_GPP_I_9
    {GPIO_VER3_GPP_I10,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_I_10_TP_Inner_GPP_I_10
    {GPIO_VER3_GPP_I11,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_I_11_TP_Inner_GPP_I_11
    {GPIO_VER3_GPP_I18,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_I_18_TP_Inner_GPP_I_18
    {GPIO_VER3_GPP_I19,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_I_19_TP_Inner_GPP_I_19
    {GPIO_VER3_GPP_I20,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_I_20_TP_Inner_GPP_I_20
    {GPIO_VER3_GPP_I21,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_I_21_IRQ_TPM_SPI_N
    {GPIO_VER3_GPP_I22,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutHigh,    GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_I_22_FM_PCH_DFXTESTMODE
    {GPIO_VER3_GPP_I23,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutHigh,    GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_I_23_FM_PCH_XTALSEL
    {GPIO_VER3_GPP_J0,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutHigh,    GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_J_0_PWRGD_CPUPWRGD_GTL
    {GPIO_VER3_GPP_J1,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_J_1_H_THERMTRIP_LVC1_N
#ifdef SPR_HOST
    {GPIO_VER3_GPP_J2,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_J_2_TP_PLTRST_CPU_N
#else
    {GPIO_VER3_GPP_J2,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_J_2_TP_PLTRST_CPU_N
#endif //SPR_HOST
    {GPIO_VER3_GPP_J3,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_J_3_FM_PCH_TRIGGER0_R_N
    {GPIO_VER3_GPP_J4,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_J_4_FM_PCH_TRIGGER1_R_N
    {GPIO_VER3_GPP_J5,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_J_5_FM_PCH_CPU_PWR_DEBUG_N
    {GPIO_VER3_GPP_J6,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_J_6_TP_FM_CPU_MEMTRIP_N
    {GPIO_VER3_GPP_J7,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntLevel | GpioIntSmi,      GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_J_7_TP_CPU_MSMI_N
    {GPIO_VER3_GPP_J9,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_J_9_TP_Inner_GPP_J_9
    {GPIO_VER3_GPP_J10,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_J_10_TP_Inner_GPP_J_10
    {GPIO_VER3_GPP_J11,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_J_11_TP_Inner_GPP_J_11
    {GPIO_VER3_GPP_J12,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermWpu20K, GpioPadConfigLock}},//GPP_J_12_H_CPU_ERR0_PCH_R_N
    {GPIO_VER3_GPP_J13,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutHigh,    GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_J_13_FM_PCIE_FV_BIF_EN
    {GPIO_VER3_GPP_J14,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermWpu20K, GpioPadConfigLock}},//GPP_J_14_H_CPU_ERR1_PCH_R_N
    {GPIO_VER3_GPP_J15,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermWpu20K, GpioPadConfigLock}},//GPP_J_15_H_CPU_ERR2_PCH_R_N
    {GPIO_VER3_GPP_J16,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_J_16_TP_Inner_GPP_J_16
    {GPIO_VER3_GPP_J17,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_J_17_TP_Inner_GPP_J_17
    {GPIO_VER3_GPP_L0,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_L_0_H_CPU0_PMSYNC_PCH_R
    {GPIO_VER3_GPP_L2,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_L_2_TP_PCH_GPP_L_2
    {GPIO_VER3_GPP_L3,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutHigh,    GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_L_3_FM_BIOS_ADV_FUNCTIONS
    {GPIO_VER3_GPP_L5,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_L_5_FM_PASSWORD_CLEAR_N
    {GPIO_VER3_GPP_L6,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutHigh,    GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_L_6_FM_MFG_MODE
    {GPIO_VER3_GPP_L7,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_L_7_TP_PCH_GPP_L_7
    {GPIO_VER3_GPP_L8,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_L_8_TP_PCH_GPP_L_8
    {GPIO_VER3_GPP_L9,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_L_9_TP_Inner_GPP_L_9
    {GPIO_VER3_GPP_L10,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_L_10_TP_Inner_GPP_L_10
    {GPIO_VER3_GPP_L11,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_L_11_TP_Inner_GPP_L_11
    {GPIO_VER3_GPP_L12,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_L_12_TP_Inner_GPP_L_12
    {GPIO_VER3_GPP_L13,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_L_13_TP_Inner_GPP_L_13
    {GPIO_VER3_GPP_L14,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_L_14_TP_Inner_GPP_L_14
    {GPIO_VER3_GPP_L15,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_L_15_TP_Inner_GPP_L_15
    {GPIO_VER3_GPP_L16,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_L_16_TP_Inner_GPP_L_16
    {GPIO_VER3_GPP_L17,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_L_17_TP_Inner_GPP_L_17
    {GPIO_VER3_GPP_M0,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_M_0_FM_BOARD_SKU_ID0
    {GPIO_VER3_GPP_M1,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_M_1_FM_BOARD_SKU_ID1
    {GPIO_VER3_GPP_M2,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_M_2_FM_BOARD_SKU_ID2
    {GPIO_VER3_GPP_M3,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_M_3_FM_BOARD_SKU_ID3
    {GPIO_VER3_GPP_M4,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_M_4_FM_BOARD_SKU_ID4
    {GPIO_VER3_GPP_M5,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_M_5_FM_BOARD_SKU_ID5
    {GPIO_VER3_GPP_M6,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_M_6_FM_BOARD_REV_ID0
    {GPIO_VER3_GPP_M7,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_M_7_FM_BOARD_REV_ID1
    {GPIO_VER3_GPP_M8,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_M_8_FM_BOARD_REV_ID2
    {GPIO_VER3_GPP_M9,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_M_9_TP_Inner_GPP_M_9
    {GPIO_VER3_GPP_M10,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_M_10_TP_Inner_GPP_M_10
    {GPIO_VER3_GPP_M11,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutHigh,    GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_M_11_FM_PCH_BIOS_RCVR_MODE
    {GPIO_VER3_GPP_M12,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_M_12_FM_RISER1_ID_0
    {GPIO_VER3_GPP_M13,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_M_13_TP_Inner_GPP_M_13
    {GPIO_VER3_GPP_M14,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_M_14_TP_Inner_GPP_M_14
    {GPIO_VER3_GPP_M15,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_M_15_FM_RISER1_ID_1
    {GPIO_VER3_GPP_M16,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_M_16_FM_RISER2_ID_0
    {GPIO_VER3_GPP_M17,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_M_17_FM_RISER2_ID_1
    {GPIO_VER3_GPP_N0,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_N_0_TP_Inner_GPP_N_0
    {GPIO_VER3_GPP_N1,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_N_1_TP_PCH_GPP_N_1
    {GPIO_VER3_GPP_N2,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_N_2_TP_Inner_GPP_N_2
    {GPIO_VER3_GPP_N3,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_N_3_TP_Inner_GPP_N_3
    {GPIO_VER3_GPP_N4,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_N_4_TP_PCH_GPP_N_4
    {GPIO_VER3_GPP_N5,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_N_5_TP_Inner_GPP_N_5
    {GPIO_VER3_GPP_N6,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_N_6_TP_Inner_GPP_N_6
    {GPIO_VER3_GPP_N7,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_N_7_TP_Inner_GPP_N_7
    {GPIO_VER3_GPP_N8,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_N_8_TP_Inner_GPP_N_8
    {GPIO_VER3_GPP_N9,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_N_9_TP_Inner_GPP_N_9
    {GPIO_VER3_GPP_N10,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_N_10_TP_Inner_GPP_N_10
    {GPIO_VER3_GPP_N11,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_N_11_TP_Inner_GPP_N_11
    {GPIO_VER3_GPP_N12,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_N_12_TP_Inner_GPP_N_12
    {GPIO_VER3_GPP_N13,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_N_13_TP_Inner_GPP_N_13
    {GPIO_VER3_GPP_N14,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_N_14_TP_Inner_GPP_N_14
    {GPIO_VER3_GPP_N15,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_N_15_TP_Inner_GPP_N_15
    {GPIO_VER3_GPP_N16,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_N_16_TP_Inner_GPP_N_16
    {GPIO_VER3_GPP_N17,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_N_17_TP_Inner_GPP_N_17
#ifdef SPR_HOST
    {GPIO_VER3_GPP_O1,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutHigh,    GpioIntDis,                     GpioDswReset,        GpioTermNone,   GpioPadConfigLock}},//GPP_O_1_PU_ACPRESENT
#else
    {GPIO_VER3_GPP_O1,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutHigh,    GpioIntDis,                     GpioDswReset,        GpioTermNone,   GpioPadConfigLock}},//GPP_O_1_PU_ACPRESENT
#endif //SPR_HOST
    {GPIO_VER3_GPP_O2,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioDswReset,        GpioTermNone,   GpioPadConfigLock}},//GPP_O_2_FM_LAN_WAKE_N
    {GPIO_VER3_GPP_O3,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioDswReset,        GpioTermNone,   GpioPadConfigLock}},//GPP_O_3_FM_PCH_PWRBTN_N
    {GPIO_VER3_GPP_O4,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioDswReset,        GpioTermNone,   GpioPadConfigLock}},//GPP_O_4_FM_SLPS3_N
    {GPIO_VER3_GPP_O5,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioDswReset,        GpioTermNone,   GpioPadConfigLock}},//GPP_O_5_FM_SLPS4_N
    {GPIO_VER3_GPP_O6,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioDswReset,        GpioTermNone,   GpioPadConfigLock}},//GPP_O_6_FM_SLPA_N
    {GPIO_VER3_GPP_O7,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioDswReset,        GpioTermNone,   GpioPadConfigLock}},//GPP_O_7_FM_PCH_SKIP_RTC_CLOCK
#ifdef SPR_HOST
    {GPIO_VER3_GPP_O8,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioDswReset,        GpioTermNone,   GpioPadConfigLock}},//GPP_O_8_PD_SUSCLK
#else
    {GPIO_VER3_GPP_O8,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioDswReset,        GpioTermNone,   GpioPadConfigLock}},//GPP_O_8_PD_SUSCLK
#endif //SPR_HOST
    {GPIO_VER3_GPP_O9,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioDswReset,        GpioTermNone,   GpioPadConfigLock}},//GPP_O_9_TP_Inner_GPP_O_9
    {GPIO_VER3_GPP_O10,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioDswReset,        GpioTermNone,   GpioPadConfigLock}},//GPP_O_10_FM_SLPS5_N
    {GPIO_VER3_GPP_O11,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioDswReset,        GpioTermNone,   GpioPadConfigLock}},//GPP_O_11_FM_PHY_DISABLE_N
    {GPIO_VER3_GPP_O12,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutHigh,    GpioIntDis,                     GpioDswReset,        GpioTermNone,   GpioPadConfigLock}},//GPP_O_12_PWRGD_SYS_PWROK
    {GPIO_VER3_GPP_O13,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioDswReset,        GpioTermNone,   GpioPadConfigLock}},//GPP_O_13_RST_BMC_RSTBTN_OUT_N
    {GPIO_VER3_GPP_O14,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioDswReset,        GpioTermNone,   GpioPadConfigLock}},//GPP_O_14_FM_SLP_LAN_N
    {GPIO_VER3_GPP_O15,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutLow,     GpioIntDis,                     GpioDswReset,        GpioTermNone,   GpioPadConfigLock}},//GPP_O_15_IRQ_LVC3_WAKE_N
    {GPIO_VER3_GPP_O16,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioDswReset,        GpioTermNone,   GpioPadConfigLock}},//GPP_O_16_FM_PCH_SLP_SUS_N
#ifdef SPR_HOST
    {GPIO_VER3_GPPC_S1,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutHigh,    GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_S_1_FM_NO_REBOOT_SPKR
#else
    {GPIO_VER3_GPPC_S1,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutHigh,    GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_S_1_FM_NO_REBOOT_SPKR
#endif //SPR_HOST
    {GPIO_VER3_GPPC_S3,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_S_3_FM_ADR_SMI_GPIO_R_N
    {GPIO_VER3_GPPC_S4,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutHigh,    GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_S_4_FM_SPI_3P3_1P8_VOLTAGE
    {GPIO_VER3_GPPC_S5,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutHigh,    GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_S_5_FM_PCH_CONSENT_STRAP
    {GPIO_VER3_GPPC_S6,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_S_6_FM_EUP_LOT6_N
    {GPIO_VER3_GPPC_S7,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_S_7_FM_SUSACK_N
    {GPIO_VER3_GPPC_S8,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntLevel | GpioIntNmi,      GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_S_8_IRQ_PCH_CPU_NMI_EVENT_R_N
    {GPIO_VER3_GPPC_S9,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntLevel | GpioIntSmi,      GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_S_9_IRQ_SMI_ACTIVE_N
    {GPIO_VER3_GPPC_S10,     { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutHigh,    GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_S_10_
    {GPIO_VER3_GPPC_S11,     { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutHigh,    GpioIntDis,                     GpioHostDeepReset,   GpioTermNone,   GpioPadConfigLock}},//GPP_S_11_
*/

//Transformers_JK004+++>>
    {GPIO_VER3_GPP_A0,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioLockDefault}},//GPP_A_0_PCH_ESPI_BMC_ALERT0_N
    {GPIO_VER3_GPP_A1,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioLockDefault}},//GPP_A_1_PCH_ESPI_ALERT1_N (Reserve)
    {GPIO_VER3_GPP_A2,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioLockDefault}},//GPP_A_2_PCH_ESPI_IO_0_R
    {GPIO_VER3_GPP_A3,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_A_3_PCH_ESPI_IO_1_R
    {GPIO_VER3_GPP_A4,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_A_4_PCH_ESPI_IO_2_R
    {GPIO_VER3_GPP_A5,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_A_5_PCH_ESPI_IO_3_R
    {GPIO_VER3_GPP_A6,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_A_6_PCH_ESPI_BMC_CS0_R_N
    {GPIO_VER3_GPP_A7,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_A_7_PCH_ESPI_CS1_N
    {GPIO_VER3_GPP_A8,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_A_8_RST_PCH_ESPI_N
    {GPIO_VER3_GPP_A9,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_A_9_CLK_50M_PCH_ESPI
    {GPIO_VER3_GPP_A10,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_A_10_PCH_BMC_CLKREQ_N
    {GPIO_VER3_GPP_A11,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_A_11_PCH_M2_CLKREQ_N
    {GPIO_VER3_GPP_A12,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutHigh,    GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_A_12_PCH_IERR_LOG_N
    // {GPIO_VER3_GPP_A13,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_A_13_NC_PCH_GPPC_A13
    // {GPIO_VER3_GPP_A14,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_A_14_NC_PCH_GPPC_A14
    {GPIO_VER3_GPP_A15,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_A_15_PCH_FD0V_ALERT
    {GPIO_VER3_GPP_A16,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirInOut,    GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_A_16_PCH_SATA_RAID_KEY
    // {GPIO_VER3_GPP_A17,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntLevel | GpioIntNmi,      GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_A_17_NC_PCH_GPPC_A17
    {GPIO_VER3_GPP_A18,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutHigh,    GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_A_18_BIOS_POST_START_N
    // {GPIO_VER3_GPP_A19,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_A_19_ME_HEARTBEAT
    {GPIO_VER3_GPP_B0,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_B_0_PCH_PTI0_CLK_R
    {GPIO_VER3_GPP_B1,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_B_1_PCH_PTI_R<0>
    {GPIO_VER3_GPP_B2,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_B_2_PCH_PTI_R<1>
    {GPIO_VER3_GPP_B3,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_B_3_PCH_PTI_R<2>
    {GPIO_VER3_GPP_B4,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_B_4_PCH_PTI_R<3>
    // {GPIO_VER3_GPP_B5,       { GpioPadModeNative4, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_B_5_PCH_PTI_R<4>
    {GPIO_VER3_GPP_B6,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_B_6_PCH_USB3_OC_FRONT_N
    {GPIO_VER3_GPP_B7,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_B_7_PCA_REV_ID0
    {GPIO_VER3_GPP_B8,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_B_8_PCA_REV_ID1
    {GPIO_VER3_GPP_B9,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_B_9_PCH_USB3_OC_INT_N
    {GPIO_VER3_GPP_B10,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_B_10_PCA_REV_ID2
    // {GPIO_VER3_GPP_B11,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutLow,     GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_B_11_ME_FW_RECOVERY_N
    // {GPIO_VER3_GPP_B12,      { GpioPadModeNative4, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_B_12_PCH_PTI<5>
    {GPIO_VER3_GPP_B13,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_B_13_PCH_PTI<6>
    // {GPIO_VER3_GPP_B14,      { GpioPadModeNative4, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_B_14_PCH_PTI<7>
    // {GPIO_VER3_GPP_B15,      { GpioPadModeNative4, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_B_15_PCH_PTI<8>
    // {GPIO_VER3_GPP_B16,      { GpioPadModeNative4, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_B_16_PCH_PTI<9>
    // {GPIO_VER3_GPP_B17,      { GpioPadModeNative4, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_B_17_PCH_PTI<10>
    // {GPIO_VER3_GPP_B18,      { GpioPadModeNative4, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_B_18_PCH_PTI<11>
    // {GPIO_VER3_GPP_B19,      { GpioPadModeNative4, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_B_19_PCH_PTI<12>
    // {GPIO_VER3_GPP_B20,      { GpioPadModeNative4, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_B_20_PCH_PTI<13>
    // {GPIO_VER3_GPP_B21,      { GpioPadModeNative4, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_B_21_PCH_PTI<14>
    // {GPIO_VER3_GPP_B22,      { GpioPadModeNative4, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_B_22_PCH_PTI<15>
    {GPIO_VER3_GPP_B23,      { GpioPadModeNative4, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_B_23_PCH_PTI1_CLK
    {GPIO_VER3_GPP_C0,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_C_0_I2C_PCH_SMLINK_0_SCL
    {GPIO_VER3_GPP_C1,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_C_1_I2C_PCH_SMLINK_0_SDA
    {GPIO_VER3_GPP_C2,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_C_2_I2C_PCH_SMLINK_0_ALERT_R_N
    // {GPIO_VER3_GPP_C3,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_C_3_I2C_PCH_SMLINK_0B_SDA
    // {GPIO_VER3_GPP_C4,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_C_4_I2C_PCH_SMLINK_0B_SCL
    // {GPIO_VER3_GPP_C5,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_C_5_NC_PCH_GPPC_C5
    {GPIO_VER3_GPP_C6,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_C_6_I2C_PCH_SMLINK_1_SCL_R
    {GPIO_VER3_GPP_C7,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_C_7_I2C_PCH_SMLINK_1_SDA_R
    
    //PSU alert with SMI. Comment out to prevent potential system hang.
    // {GPIO_VER3_GPP_C8,       { GpioPadModeGpio,    GpioHostOwnAcpi,       GpioDirInInv,    GpioOutDefault, GpioIntEdge | GpioIntSmi,       GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_C_8_PSU1_ALERT_PCH_N
    
    {GPIO_VER3_GPP_C9,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_C_9_I2C_PCH_SMLINK_2_SCL_R
    {GPIO_VER3_GPP_C10,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_C_10_I2C_PCH_SMLINK_2_SDA_R
    {GPIO_VER3_GPP_C11,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_C_11_PCH_PS_ALERT_EN
    // {GPIO_VER3_GPP_C12,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_C_12_NC_I2C_PCH_SMLINK_3_SCL
    // {GPIO_VER3_GPP_C13,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_C_13_NC_I2C_PCH_SMLINK_3_SDA
    {GPIO_VER3_GPP_C14,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutHigh,    GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_C_14_PCH_THROTTLE_N
    // {GPIO_VER3_GPP_C15,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_C_15_NC_I2C_PCH_SMLINK_4_SCL
    // {GPIO_VER3_GPP_C16,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_C_16_NC_I2C_PCH_SMLINK_4_SDA
    {GPIO_VER3_GPP_C17,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutHigh,    GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_C_17_BIOS_POST_CMPLT_N
    {GPIO_VER3_GPP_C18,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutLow,     GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_C_18_PCH_CPLD_DATA_R
    {GPIO_VER3_GPP_C19,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_C_19_I2C_PCH_PEHP_SCL_R
    {GPIO_VER3_GPP_C20,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_C_20_I2C_PCH_PEHP_SDA_R
    // {GPIO_VER3_GPP_C21,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_C_21_I2C_PCH_PEHP_ALERT_N
    {GPIO_VER3_GPP_D0,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_D_0_I2C_PCH_HOST_STBY_SCL
    {GPIO_VER3_GPP_D1,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_D_1_I2C_PCH_HOST_STBY_SDA
    // {GPIO_VER3_GPP_D2,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_D_2_NC_PCH_GPP_D2
    // {GPIO_VER3_GPP_D3,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_D_3
    // {GPIO_VER3_GPP_D4,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_D_4
    // {GPIO_VER3_GPP_D5,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirInOut,    GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_D_5
    {GPIO_VER3_GPP_D6,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_D_6_PCH_ME_DESC_OVERRIDE
    {GPIO_VER3_GPP_D7,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_D_7_PCH_ESPI_FLASH_MODE
    {GPIO_VER3_GPP_D8,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_D_8_PCH_CRASHLOG_TRIG_N
    // {GPIO_VER3_GPP_D9,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_D_9_PU_LPC_PME_N
    // {GPIO_VER3_GPP_D10,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_D_10_PU_IRQ_PCH_SCI_WHEA_N
    {GPIO_VER3_GPP_D11,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_D_11_RST_PCH_PLTRST_R_N
    {GPIO_VER3_GPP_D12,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_D_12_PCH_HOT_R_N
    {GPIO_VER3_GPP_D13,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_D_13_PCH_ADR_COMPLETE
    {GPIO_VER3_GPP_D14,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_D_14_PCH_ADR_TRIGGER_N
    // {GPIO_VER3_GPP_D15,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_D_15_PU_PCH_VR_ALERT_N
    {GPIO_VER3_GPP_D16,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_D_16_PCH_ADR_ACK
    {GPIO_VER3_GPP_D17,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_D_17_PCH_THERMTRIP_N
    {GPIO_VER3_GPP_D18,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_D_18_CPU_CATERR_DLY_BUF_N
    {GPIO_VER3_GPP_D19,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_D_19_PCH_BIOS_IMAGE_SWAP_N
    // {GPIO_VER3_GPP_D20,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_D_20_NC_PCH_GPP_D20
    {GPIO_VER3_GPP_D21,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_D_21_PCH_GLB_RST_WARN_R_N
    {GPIO_VER3_GPP_D22,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_D_22_NC_PCH_GPP_D22
    {GPIO_VER3_GPP_D23,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_D_23_PCH_BIOS_BOOT_MODE
    {GPIO_VER3_GPP_E0,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_E_0_PCH_CPU_MIX_STATUS_0
    {GPIO_VER3_GPP_E1,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutHigh,    GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_E_1_PCH_OCP_BIF0_N
    {GPIO_VER3_GPP_E2,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutHigh,    GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_E_2_PCH_OCP_BIF1_N
    {GPIO_VER3_GPP_E3,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutHigh,    GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_E_3_PCH_OCP_BIF2_N
    {GPIO_VER3_GPP_E4,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_E_4_PRSNT_B0_OCP_N
    {GPIO_VER3_GPP_E5,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_E_5_PRSNT_B1_OCP_N
    {GPIO_VER3_GPP_E6,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_E_6_PRSNT_B2_OCP_N
    {GPIO_VER3_GPP_E7,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_E_7_PRSNT_B3_OCP_N
    {GPIO_VER3_GPP_E8,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_E_8_SGPIO_SATA0_CLOCK_R
    {GPIO_VER3_GPP_E9,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_E_9_SGPIO_SATA0_LOAD_R
    {GPIO_VER3_GPP_E10,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_E_10_SGPIO_SATA0_DATA_R
    {GPIO_VER3_GPP_E11,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_E_11_SGPIO_SATA1_CLOCK_R
    {GPIO_VER3_GPP_E12,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_E_12_SGPIO_SATA1_LOAD_R
    {GPIO_VER3_GPP_E13,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_E_13_SGPIO_SATA1_DATA_R
    {GPIO_VER3_GPP_E14,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_E_14_PCH_CPU_MIX_STATUS_1
    // {GPIO_VER3_GPP_E15,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_E_15_PCH_ME_AUTHN_FAIL
    // {GPIO_VER3_GPP_E16,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_E_16_PCH_ME_BT_DONE
    {GPIO_VER3_GPP_E17,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_E_17_PCH_CPU_MIX_STATUS_2
    {GPIO_VER3_GPP_E18,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_E_18_PS_PWROK_DLY_SEL_R
    {GPIO_VER3_GPP_E19,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_E_19_TPM_PRSNT_N
    {GPIO_VER3_GPP_H0,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_H_0_PCH_VISA_DEFAULT
    {GPIO_VER3_GPP_H1,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_H_1_PCH_IO_EXPANDER
    // {GPIO_VER3_GPP_H2,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_H_2
    // {GPIO_VER3_GPP_H3,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_H_3
    // {GPIO_VER3_GPP_H4,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_H_4
    // {GPIO_VER3_GPP_H5,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_H_5
    {GPIO_VER3_GPP_H6,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_H_6_PCH_EXTCLK_MODE
    {GPIO_VER3_GPP_H7,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_H_7_RISER1_ID2
    // {GPIO_VER3_GPP_H8,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_H_8
    // {GPIO_VER3_GPP_H9,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_H_9
    // {GPIO_VER3_GPP_H10,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_H_10
    // {GPIO_VER3_GPP_H11,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_H_11
    // {GPIO_VER3_GPP_H12,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_H_12
    // {GPIO_VER3_GPP_H13,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_H_13
    // {GPIO_VER3_GPP_H14,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_H_14
    {GPIO_VER3_GPP_H15,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_H_15_RISER2_ID0
    {GPIO_VER3_GPP_H16,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_H_16_RISER2_ID1
    {GPIO_VER3_GPP_H17,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_H_17_RISER2_ID2
    // {GPIO_VER3_GPP_H18,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_H_18_NC_PCH_GPPC_H18
    {GPIO_VER3_GPP_H19,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_H_19_PCH_TESTMODE_0
    // {GPIO_VER3_GPP_I1,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_I_1
    // {GPIO_VER3_GPP_I2,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_I_2
    // {GPIO_VER3_GPP_I3,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_I_3
    // {GPIO_VER3_GPP_I4,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_I_4
    // {GPIO_VER3_GPP_I5,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_I_5
    // {GPIO_VER3_GPP_I6,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_I_6
    // {GPIO_VER3_GPP_I7,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_I_7
    // {GPIO_VER3_GPP_I8,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_I_8
    // {GPIO_VER3_GPP_I9,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_I_9
    // {GPIO_VER3_GPP_I10,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_I_10
    // {GPIO_VER3_GPP_I11,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_I_11
    {GPIO_VER3_GPP_I12,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_I_12_RISER3_ID0
    {GPIO_VER3_GPP_I13,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_I_13_RISER3_ID1
    {GPIO_VER3_GPP_I14,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_I_14_RISER3_ID2
    {GPIO_VER3_GPP_I15,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_I_15_RISER1_ID0
    {GPIO_VER3_GPP_I16,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_I_16_RISER1_ID1
    {GPIO_VER3_GPP_I17,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_I_17_PCH_MFG_MODE
    // {GPIO_VER3_GPP_I18,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_I_18_PCH_MFG_MODE
    // {GPIO_VER3_GPP_I19,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_I_19_PCH_MFG_MODE
    // {GPIO_VER3_GPP_I20,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_I_20_PCH_MFG_MODE
   
    //TPM IRQ. Comment out to prevent potential system hang.
    // {GPIO_VER3_GPP_I21,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntLevel | GpioIntApic,     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_I_21_TPM_IRQ_N

    {GPIO_VER3_GPP_I22,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_I_22_PCH_TESTMODE_3
    {GPIO_VER3_GPP_I23,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_I_23_PCH_TESTMODE_2
    // {GPIO_VER3_GPP_J0,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_J_0
    // {GPIO_VER3_GPP_J1,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_J_1
    // {GPIO_VER3_GPP_J2,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_J_2
    // {GPIO_VER3_GPP_J3,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_J_3
    // {GPIO_VER3_GPP_J4,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_J_4
    // {GPIO_VER3_GPP_J5,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_J_5
    // {GPIO_VER3_GPP_J6,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_J_6
    // {GPIO_VER3_GPP_J7,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_J_7
    // {GPIO_VER3_GPP_J8,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_J_8
    // {GPIO_VER3_GPP_J9,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_J_9
    // {GPIO_VER3_GPP_J10,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_J_10
    // {GPIO_VER3_GPP_J11,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_J_11
    // {GPIO_VER3_GPP_J12,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_J_12
    // {GPIO_VER3_GPP_J13,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_J_13
    // {GPIO_VER3_GPP_J14,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_J_14
    // {GPIO_VER3_GPP_J15,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_J_15
    // {GPIO_VER3_GPP_J16,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_J_16
    // {GPIO_VER3_GPP_J17,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_J_17
    {GPIO_VER3_GPP_L0,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_L_0_CPU0_PMSYNC_PCH
    {GPIO_VER3_GPP_L1,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_L_1_CPU0_PMDOWN_PCH_R
    // {GPIO_VER3_GPP_L2,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_L_2_NC_PCH_GPP_L_2
    // {GPIO_VER3_GPP_L3,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_L_3_NC_PCH_GPP_L_3
    // {GPIO_VER3_GPP_L4,       { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_L_4_NC_PCH_GPP_L_4
    {GPIO_VER3_GPP_L5,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_L_5_PASSWORD_CLEAR_N
    // {GPIO_VER3_GPP_L6,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_L_6_NC_PCH_GPP_L_6
    // {GPIO_VER3_GPP_L7,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_L_7_NC_PCH_GPP_L_7
    // {GPIO_VER3_GPP_L8,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_L_8_NC_PCH_GPP_L_8
    // {GPIO_VER3_GPP_L9,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_L_9
    // {GPIO_VER3_GPP_L10,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_L_10
    // {GPIO_VER3_GPP_L11,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_L_11
    // {GPIO_VER3_GPP_L12,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_L_12
    // {GPIO_VER3_GPP_L13,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_L_13
    // {GPIO_VER3_GPP_L14,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_L_14
    // {GPIO_VER3_GPP_L15,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_L_15
    // {GPIO_VER3_GPP_L16,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_L_16
    // {GPIO_VER3_GPP_L17,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_L_17
    // {GPIO_VER3_GPP_M0,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_M_0_NC_PCH_GPP_M_0
    // {GPIO_VER3_GPP_M1,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_M_1_NC_PCH_GPP_M_1
    // {GPIO_VER3_GPP_M2,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_M_2_NC_PCH_GPP_M_2
    // {GPIO_VER3_GPP_M3,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_M_3_NC_PCH_GPP_M_3
    // {GPIO_VER3_GPP_M4,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_M_4_NC_PCH_GPP_M_4
    // {GPIO_VER3_GPP_M5,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_M_5_NC_PCH_GPP_M_5
    // {GPIO_VER3_GPP_M6,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_M_6_NC_PCH_GPP_M_6
    // {GPIO_VER3_GPP_M7,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_M_7_NC_PCH_GPP_M_7
    // {GPIO_VER3_GPP_M8,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_M_8_NC_PCH_GPP_M_8
    // {GPIO_VER3_GPP_M9,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_M_9
    // {GPIO_VER3_GPP_M10,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_M_10
    {GPIO_VER3_GPP_M11,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_M_11_PCH_BIOS_RCVR_MODE
    // {GPIO_VER3_GPP_M12,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_M_12_NC_PCH_GPP_M_12
    // {GPIO_VER3_GPP_M13,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_M_13
    // {GPIO_VER3_GPP_M14,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_M_14
    // {GPIO_VER3_GPP_M15,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_M_15_NC_PCH_GPP_M_15
    // {GPIO_VER3_GPP_M16,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_M_16_NC_PCH_GPP_M_16
    // {GPIO_VER3_GPP_M17,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_M_17_NC_PCH_GPP_M_17
    // {GPIO_VER3_GPP_N0,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_N_0
    // {GPIO_VER3_GPP_N1,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_N_1_NC_PCH_GPP_N_1
    // {GPIO_VER3_GPP_N2,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_N_2
    // {GPIO_VER3_GPP_N3,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_N_3
    // {GPIO_VER3_GPP_N4,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_N_4_NC_PCH_GPP_N_4
    // {GPIO_VER3_GPP_N5,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_N_5
    // {GPIO_VER3_GPP_N6,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_N_6
    // {GPIO_VER3_GPP_N7,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_N_7
    // {GPIO_VER3_GPP_N8,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_N_8
    // {GPIO_VER3_GPP_N9,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_N_9
    // {GPIO_VER3_GPP_N10,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_N_10
    // {GPIO_VER3_GPP_N11,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_N_11
    // {GPIO_VER3_GPP_N12,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_N_12
    // {GPIO_VER3_GPP_N13,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_N_13
    // {GPIO_VER3_GPP_N14,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_N_14
    // {GPIO_VER3_GPP_N15,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_N_15
    // {GPIO_VER3_GPP_N16,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_N_16
    // {GPIO_VER3_GPP_N17,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_N_17
    // {GPIO_VER3_GPP_O0,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_O_0
    // {GPIO_VER3_GPP_O1,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_O_1
    // {GPIO_VER3_GPP_O2,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_O_2
    // {GPIO_VER3_GPP_O3,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_O_3
    // {GPIO_VER3_GPP_O4,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_O_4
    // {GPIO_VER3_GPP_O5,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_O_5
    // {GPIO_VER3_GPP_O6,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_O_6
    // {GPIO_VER3_GPP_O7,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_O_7
    // {GPIO_VER3_GPP_O8,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_O_8
    // {GPIO_VER3_GPP_O9,       { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_O_9
    // {GPIO_VER3_GPP_O10,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_O_10
    // {GPIO_VER3_GPP_O11,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_O_11
    // {GPIO_VER3_GPP_O12,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_O_12
    // {GPIO_VER3_GPP_O13,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_O_13
    // {GPIO_VER3_GPP_O14,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_O_14
    // {GPIO_VER3_GPP_O15,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_O_15
    // {GPIO_VER3_GPP_O16,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_O_16
   
    //PCH_SMI_N. This GPIO pin don't have capability for SMI/ NMI function. EE: Please ignore this signal at present.
    // {GPIO_VER3_GPPC_S0,      { GpioPadModeGpio,    GpioHostOwnAcpi,       GpioDirInInv,    GpioOutDefault, GpioIntEdge | GpioIntSmi,       GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_S_0_PCH_SMI_N
    
    {GPIO_VER3_GPPC_S1,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_S_1_PCH_NO_REBOOT
    
    //PCH_NMI_N. Comment out to prevent potential system hang.
    // {GPIO_VER3_GPPC_S2,      { GpioPadModeGpio,    GpioHostOwnAcpi,       GpioDirInInv,    GpioOutDefault, GpioIntEdge | GpioIntNmi,       GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_S_2_PCH_NMI_N
 
    //CPLD_ADR_SMI_N. Comment out to prevent potential system hang.
    // {GPIO_VER3_GPPC_S3,      { GpioPadModeGpio,    GpioHostOwnAcpi,       GpioDirInInv,    GpioOutDefault, GpioIntEdge | GpioIntSmi,       GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_S_3_CPLD_ADR_SMI_N
    
    {GPIO_VER3_GPPC_S4,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_S_4_PCH_SPI_BUFFER
    {GPIO_VER3_GPPC_S5,      { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_S_5_MIPI_DEBUG_CONSENT_N
    
    //PCH_SCI_N. Comment out to prevent potential system hang.
    // {GPIO_VER3_GPPC_S6,      { GpioPadModeGpio,    GpioHostOwnAcpi,       GpioDirInInv,    GpioOutDefault, GpioIntEdge | GpioIntSci,       GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_S_6_PCH_SCI_N
    
    {GPIO_VER3_GPPC_S7,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_S_7_PU_PCH_SUSACK_N
    {GPIO_VER3_GPPC_S8,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_S_8_PCH_NMI_ACTIVE_N
    {GPIO_VER3_GPPC_S9,      { GpioPadModeNative1, GpioHostOwnDefault,    GpioDirOut,      GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_S_9_PCH_SMI_ACTIVE_N
    {GPIO_VER3_GPPC_S10,     { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_S_10_PCH_JTAG_ODT_ENABLE
    {GPIO_VER3_GPPC_S11,     { GpioPadModeGpio,    GpioHostOwnDefault,    GpioDirIn,       GpioOutDefault, GpioIntDis,                     GpioResumeReset,   GpioTermNone, GpioPadConfigLock}},//GPP_S_11_PCH_ESPI_CS_SWIZZLE
//Transformers_JK004+++<<

};



EFI_STATUS
TypeArcherCityRPInstallGpioData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
)
{
  EFI_STATUS                            Status;

  DEBUG((DEBUG_INFO,"UBA PEI: %a\n",__FUNCTION__));

  Status = UbaConfigPpi->AddData (
                                 UbaConfigPpi,
                                 &gPlatformGpioInitDataGuid,
                                 &mGpioTableArcherCityRP,
                                 sizeof(mGpioTableArcherCityRP)
                                 );
  Status = PcdSet32S (PcdOemSku_GPIO_TABLE_SIZE, sizeof (mGpioTableArcherCityRP));
  ASSERT_EFI_ERROR (Status);

  return Status;
}
