/** @file
  Register names for PCH USB devices
  Conventions:
  Register definition format:
  Prefix_[GenerationName]_[ComponentName]_SubsystemName_RegisterSpace_RegisterName
  Prefix:
  Definitions beginning with "R_" are registers
  Definitions beginning with "B_" are bits within registers
  Definitions beginning with "V_" are meaningful values within the bits
  Definitions beginning with "S_" are register size
  Definitions beginning with "N_" are the bit position
  GenerationName]:
  Three letter acronym of the generation is used (e.g. SKL,KBL,CNL etc.).
  Register name without GenerationName applies to all generations.
  ComponentName]:
  This field indicates the component name that the register belongs to (e.g. PCH, SA etc.)
  Register name without ComponentName applies to all components.
  Register that is specific to -H denoted by "_PCH_H_" in component name.
  Register that is specific to -LP denoted by "_PCH_LP_" in component name.
  SubsystemName:
  This field indicates the subsystem name of the component that the register belongs to
  e.g. PCIE, USB, SATA, GPIO, PMC etc.).
  RegisterSpace:
  MEM - MMIO space register of subsystem.
  IO  - IO space register of subsystem.
  PCR - Private configuration register of subsystem.
  CFG - PCI configuration space register of subsystem.
  RegisterName:
  Full register name.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2018 Intel Corporation. <BR>

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
#ifndef _REGS_USB_H_
#define _REGS_USB_H_
//
// USB3 (XHCI) related definitions
//
#define PCI_BUS_NUMBER_PCH_XHCI                                 0
#ifdef EBG_SC_FLAG
#define PCI_DEVICE_NUMBER_PCH_XHCI                              20
#define PCI_FUNCTION_NUMBER_PCH_XHCI                            0
#else
#define PCI_DEVICE_NUMBER_PCH_XHCI                              30
#define PCI_FUNCTION_NUMBER_PCH_XHCI                            0
#endif //EBG_SC_FLAG
//
//
// XHCI PCI Config Space registers
//
#define V_PCH_CDF_USB_DEVICE_ID_XHCI_1                          0x18D0  ///< Cedarfork XHCI
#define B_PCH_XHCI_CMD_SERR                                     BIT8     // PCI_COMMAND_OFFSET         0x04
#define R_XHCI_CFG_BAR0                     PCI_BASE_ADDRESSREG_OFFSET
#define B_XHCI_CFG_ALIGN_MASK                                   0xFFFF
#define R_XHCI_CFG_XHCC1                    0x40
#define B_XHCI_CFG_XHCC1_ACCTRL             BIT31
#define B_XHCI_CFG_XHCC1_URD                BIT23
#define R_XHCI_CFG_XHCC2                    0x44
#define B_XHCI_CFG_XHCC2_OCCFDONE           BIT31
#define B_PCH_XHCI_XHCC2_XHCUPRDROE                             BIT21
#define R_XHCI_CFG_XHCLKGTEN                0x50
#define B_PCH_XHCI_XHCLKGTEN_SRAMPGTEN                          BIT27
#define N_PCH_XHCI_XHCLKGTEN_HSTCGE                             20
#define B_PCH_XHCI_XHCLKGTEN_HSTCGE                             0xF00000
#define V_PCH_XHCI_XHCLKGTEN_HSTCGE                             0xC
#define N_PCH_XHCI_XHCLKGTEN_SSTCGE                             16
#define B_PCH_XHCI_XHCLKGTEN_SSTCGE                             0xF0000
#define V_PCH_XHCI_XHCLKGTEN_SSTCGE                             0xE
#define N_PCH_XHCI_XHCLKGTEN_HSPLLSU                            8
#define B_PCH_XHCI_XHCLKGTEN_HSPLLSU                            0x300
#define V_PCH_XHCI_XHCLKGTEN_HSPLLSU                            0x2
#define N_PCH_XHCI_XHCLKGTEN_SSPLLSUE                           5
#define B_PCH_XHCI_XHCLKGTEN_SSPLLSUE                           0x60
#define V_PCH_XHCI_XHCLKGTEN_SSPLLSUE                           0x2
#define R_XHCI_CFG_PWR_CNTL_STS             0x74
#define B_XHCI_CFG_PWR_CNTL_STS_PWR_STS                         (BIT1 | BIT0)
#define V_XHCI_CFG_PWR_CNTL_STS_PWR_STS_D3                      (BIT1 | BIT0)
#define R_XHCI_CFG_MSI_NEXT                                     0x81
#define R_XHCI_CFG_MSI_MCTL                                     0x82
#define R_XHCI_CFG_PCE                                          0xA2
#define B_XHCI_CFG_PCE_D3HE                                     BIT2
#define B_XHCI_CFG_PCE_HAE                                      BIT5  ///< Hardware Autonomous Enable
#define B_XHCI_CFG_PCE_SE                                       BIT3  ///< Sleep Enable
#define B_XHCI_CFG_PCE_DEVIDLEN                                 BIT1  ///< DEVIDLE Enable
#define B_XHCI_CFG_PCE_PMCRE                                    BIT0  ///< PMC Request Enable
#define R_XHCI_CFG_HSCFG2                                       0xA4
#define R_XHCI_CFG_SSCFG1                                       0xA8
#define R_XHCI_CFG_HSCFG1                                       0xAC
#define R_XHCI_CFG_U2OCM                                        0xB0
#define R_XHCI_CFG_U3OCM                                        0xD0

#define R_XHCI_CFG_FUS                                          0xE0
#define B_XHCI_CFG_FUS_USBR                                     (BIT5)
#define V_XHCI_CFG_FUS_USBR_EN                                  0
#define V_XHCI_CFG_FUS_USBR_DIS                                 (BIT5)
#define R_XHCI_CFG_XHCC3                                        0xFC  ///< XHC System Bus Configuration 3

//
// xHCI MMIO registers
//

//
// 0x00 - 0x1F - Capability Registers
//
#define R_XHCI_MEM_CAPLENGTH                0x00
#define R_XHCI_MEM_HCIVERSION               0x02
#define B_XHCI_MEM_HCIVERSION               0xFFFF
#define V_XHCI_MEM_HCIVERSION               0x0110
#define N_XHCI_MEM_HCIVERSION               16
#define R_XHCI_MEM_HCSPARAMS1               0x04
#define R_XHCI_MEM_HCSPARAMS2               0x08
#define R_XHCI_MEM_HCSPARAMS3               0x0C
#define B_XHCI_MEM_HCSPARAMS3               0x0000FF00
#define B_XHCI_MEM_HCSPARAMS3_U2DEL         0xFFFF0000
#define N_XHCI_MEM_HCSPARAMS3_U2DEL_POS     16
#define B_XHCI_MEM_HCSPARAMS3_U1DEL         0x000000FF
#define R_XHCI_MEM_HCCPARAMS1                                   0x10
#define B_XHCI_MEM_HCCPARAMS1_CFC                               BIT11
#define R_XHCI_MEM_HCCPARAMS2                                   0x1C
#define R_XHCI_MEM_DBOFF                                        0x14
#define R_XHCI_MEM_RTSOFF                                       0x18

//
// 0x80 - 0xBF - Operational Registers
//
#define R_XHCI_MEM_USBCMD                                       0x80
#define B_XHCI_MEM_USBCMD_RS                                    BIT0   ///< Run/Stop
#define B_XHCI_MEM_USBCMD_RST                                   BIT1   ///< HCRST
#define B_XHCI_MEM_USBCMD_HSEE                                  BIT3   //Host System Error Enable
#define R_XHCI_MEM_USBSTS                                       0x84
#define B_XHCI_MEM_USBSTS_HCH                                   BIT0
#define B_XHCI_MEM_USBSTS_CNR                                   BIT11

#define R_XHCI_MEM_PORTSC_START_OFFSET      0x480  ///< Port Status and Control Registers base offset
#define S_XHCI_MEM_PORTSC_PORT_SPACING      0x10   ///< Size of space between PortSC register for each port

#define R_PCH_H_XHCI_PORTSC01USB2                               0x480
#define R_PCH_H_XHCI_PORTSC02USB2                               0x490
#define R_PCH_H_XHCI_PORTSC03USB2                               0x4A0
#define R_PCH_H_XHCI_PORTSC04USB2                               0x4B0

#define R_PCH_H_XHCI_MEM_PORTSC01USB3                           0x4C4
#define R_PCH_H_XHCI_MEM_PORTSC02USB3                           0x4D4
#define R_PCH_H_XHCI_MEM_PORTSC03USB3                           0x4E4
#define R_PCH_H_XHCI_MEM_PORTSC04USB3                           0x4F4

//
// 0x484 - 0x4F4 - Port Power Management Status and Control
//
#define R_PCH_H_XHCI_MEM_PORTPMSC01USB2                         0x484
#define R_PCH_H_XHCI_MEM_PORTPMSC02USB2                         0x494
#define R_PCH_H_XHCI_MEM_PORTPMSC03USB2                         0x4A4
#define R_PCH_H_XHCI_MEM_PORTPMSC04USB2                         0x4B4
#define B_PCH_H_XHCI_MEM_PORTSCXUSB2_HLE                        BIT16

#define B_XHCI_MEM_PORTSCXUSB2_WPR                              BIT31  ///< Warm Port Reset
#define B_PCH_XHCI_PORTSCXUSB2_WDE                              BIT26  ///< Wake on Disconnect Enable
#define B_PCH_XHCI_PORTSCXUSB2_WCE                              BIT25  ///< Wake on Connect Enable
#define B_XHCI_MEM_PORTSCXUSB2_CEC                              BIT23  ///< Port Config Error Change
#define B_XHCI_MEM_PORTSCXUSB2_PLC                              BIT22  ///< Port Link State Change
#define B_XHCI_MEM_PORTSCXUSB2_PRC                              BIT21  ///< Port Reset Change
#define B_XHCI_MEM_PORTSCXUSB2_OCC                              BIT20  ///< Over-current Change
#define B_XHCI_MEM_PORTSCXUSB2_WRC                              BIT19  ///< Warm Port Reset Change
#define B_XHCI_MEM_PORTSCXUSB2_PEC                              BIT18  ///< Port Enabled Disabled Change
#define B_XHCI_MEM_PORTSCXUSB2_CSC                              BIT17  ///< Connect Status Change
#define B_XHCI_MEM_PORTSCXUSB2_LWS                              BIT16  ///< Port Link State Write Strobe
#define B_XHCI_MEM_PORTSCXUSB2_PP                               BIT9
#define B_XHCI_MEM_PORTSCXUSB2_PLS                              (BIT5 | BIT6 | BIT7 | BIT8)  ///< Port Link State
#define B_XHCI_MEM_PORTSCXUSB2_PR                               BIT4   ///< Port Reset
#define B_XHCI_MEM_PORTSCXUSB2_PED                              BIT1   ///< Port Enable/Disabled
#define B_XHCI_MEM_PORTSCXUSB2_CCS                              BIT0   ///< Current Connect Status
#define B_XHCI_MEM_PORT_CHANGE_ENABLE           (B_XHCI_MEM_PORTSCXUSB2_CEC | B_XHCI_MEM_PORTSCXUSB2_PLC | B_XHCI_MEM_PORTSCXUSB2_PRC | B_XHCI_MEM_PORTSCXUSB2_OCC | B_XHCI_MEM_PORTSCXUSB2_WRC | B_XHCI_MEM_PORTSCXUSB2_PEC | B_XHCI_MEM_PORTSCXUSB2_CSC | B_XHCI_MEM_PORTSCXUSB2_PED)
#define B_XHCI_MEM_PORTPMSCXUSB2_PTC                            (BIT28 | BIT29 | BIT30 | BIT31)  ///< Port Test Control

#define B_XHCI_MEM_PORTSCXUSB3_WPR                              BIT31  ///< Warm Port Reset
#define B_PCH_XHCI_PORTSCXUSB3_WDE                              BIT26  ///< Wake on Disconnect Enable
#define B_PCH_XHCI_PORTSCXUSB3_WCE                              BIT25  ///< Wake on Connect Enable
#define B_XHCI_MEM_PORTSCXUSB3_CEC                              BIT23  ///< Port Config Error Change
#define B_XHCI_MEM_PORTSCXUSB3_PLC                              BIT22  ///< Port Link State Change
#define B_XHCI_MEM_PORTSCXUSB3_PRC                              BIT21  ///< Port Reset Change
#define B_XHCI_MEM_PORTSCXUSB3_OCC                              BIT20  ///< Over-current Change
#define B_XHCI_MEM_PORTSCXUSB3_WRC                              BIT19  ///< Warm Port Reset Change
#define B_XHCI_MEM_PORTSCXUSB3_PEC                              BIT18  ///< Port Enabled Disabled Change
#define B_XHCI_MEM_PORTSCXUSB3_CSC                              BIT17  ///< Connect Status Change
#define B_XHCI_MEM_PORTSCXUSB3_LWS                              BIT16  ///< Port Link State Write Strobe
#define B_XHCI_MEM_PORTSCXUSB3_PP                               BIT9   ///< Port Power
#define B_XHCI_MEM_PORTSCXUSB3_PLS                              (BIT8 | BIT7 | BIT6 | BIT5)    ///< Port Link State
#define V_XHCI_MEM_PORTSCXUSB3_PLS_POLLING                      0x000000E0    ///< Link is in the Polling State
#define V_XHCI_MEM_PORTSCXUSB3_PLS_RXDETECT                     0x000000A0    ///< Link is in the RxDetect State
#define V_XHCI_MEM_PORTSCXUSB3_PLS_DISABLED                     0x00000080    ///< Link is in the RxDetect State
#define B_XHCI_MEM_PORTSCXUSB3_PR                               BIT4   ///< Port Reset
#define B_XHCI_MEM_PORTSCXUSB3_PED                              BIT1   ///< Port Enable/Disabled
#define B_XHCI_MEM_PORTSCXUSB3_CHANGE_ENABLE    (B_XHCI_MEM_PORTSCXUSB3_CEC | B_XHCI_MEM_PORTSCXUSB3_PLC | B_XHCI_MEM_PORTSCXUSB3_PRC | B_XHCI_MEM_PORTSCXUSB3_OCC | B_XHCI_MEM_PORTSCXUSB3_WRC | B_XHCI_MEM_PORTSCXUSB3_PEC | B_XHCI_MEM_PORTSCXUSB3_CSC | B_XHCI_MEM_PORTSCXUSB3_PED)
//
// 0x2000 - 0x21FF - Runtime Registers
// 0x3000 - 0x307F - Doorbell Registers
//
#define R_XHCI_MEM_XECP_SUPP_USB2_2                     0x8008
#define R_XHCI_MEM_XECP_SUPP_USB3_2                     0x8028
#define B_XHCI_MEM_XECP_SUPP_USBX_2_CPC                 0xFF00  ///< Mask for Compatible Port Count in Capability
#define N_XHCI_MEM_XECP_SUPP_USBX_2_CPC                 8       ///< Shift for Compatible Port Count
#define R_XHCI_MEM_HOST_CTRL_SCH_REG                    0x8094
#define R_XHCI_MEM_HOST_CTRL_ODMA_REG                   0x8098  ///< Host Control ODMA Register
#define R_XHCI_MEM_HOST_CTRL_IDMA_REG                   0x809C
#define R_XHCI_MEM_PMCTRL                               0x80A4
#define B_XHCI_MEM_PMCTRL_XMPHYSPGDD0I2                 BIT24
#define B_XHCI_MEM_PMCTRL_XMPHYSPGDD0I3                 BIT23
#define B_XHCI_MEM_PMCTRL_XMPHYSPGDRTD3                 BIT22
#define B_XHCI_MEM_PMCTRL_SSU3LFPS_DET                  0xFF00  ///< SS U3 LFPS Detection Threshold Mask
#define B_XHCI_MEM_PMCTRL_AON_LFPS_DETECTOR_EN_MODE     BIT16
#define N_XHCI_MEM_PMCTRL_SSU3LPFS_DET                  8       ///< SS U3 LFPS Detection Threshold position
#define R_XHCI_MEM_PGCBCTRL                             0x80A8  ///< PGCB Control
#define N_XHCI_MEM_PGCBCTRL_IP_INACCESSIBLE_HYSTERESIS_TIMER 25
#define V_XHCI_MEM_PGCBCTRL_IP_INACCESSIBLE_HYSTERESIS_TIMER 6
#define B_XHCI_MEM_PGCBCTRL_OVERRIDE_DISABLE            BIT24
#define R_XHCI_MEM_HOST_CTRL_MISC_REG                   0x80B0  ///< Host Controller Misc Reg
#define R_XHCI_MEM_HOST_CTRL_MISC_REG_2                 0x80B4  ///< Host Controller Misc Reg 2
#define R_XHCI_MEM_SSPE                                 0x80B8  ///< Super Speed Port Enables
#define R_XHCI_MEM_AUX_CTRL_REG                         0x80C0  ///< AUX_CTRL_REG - AUX Reset Control
#define R_XHCI_MEM_HOST_BW_OV_HS_REG                    0x80C8  ///< HOST_BW_OV_HS_REG - High Speed TT Bandwidth Overhead
#define B_XHCI_MEM_HOST_BW_OV_HS_REG_OVHD_HSTTBW        0x0FFF  ///< Mask for Overhead per packet for HS-TT BW calculations value
#define B_XHCI_MEM_HOST_BW_OV_HS_REG_OVHD_HSBW          0xFFF000 ///< Mask for Overhead per packet for HS BW calculations value
#define N_XHCI_MEM_HOST_BW_OV_HS_REG_OVHD_HSBW          12
#define R_XHCI_MEM_DUAL_ROLE_CFG0                       0x80D8
#define R_XHCI_MEM_DUAL_ROLE_CFG1                       0x80DC
#define R_XHCI_MEM_AUX_CTRL_REG1                        0x80E0
#define R_XHCI_MEM_HOST_CTRL_PORT_LINK_REG              0x80EC  ///< SuperSpeed Port Link Control
#define R_XHCI_MEM_USB2_LINK_MGR_CTRL_REG1_DW1          0x80F0  ///< USB2_LINK_MGR_CTRL_REG1 - USB2 Port Link Control 1, 2, 3, 4
#define R_XHCI_MEM_USB2_LINK_MGR_CTRL_REG1_DW2          0x80F4  ///< USB2_LINK_MGR_CTRL_REG1 - USB2 Port Link Control 1, 2, 3, 4
#define R_XHCI_MEM_USB2_LINK_MGR_CTRL_REG1_DW3          0x80F8  ///< USB2_LINK_MGR_CTRL_REG1 - USB2 Port Link Control 1, 2, 3, 4
#define R_XHCI_MEM_USB2_LINK_MGR_CTRL_REG1_DW4          0x80FC  ///< USB2_LINK_MGR_CTRL_REG1 - USB2 Port Link Control 1, 2, 3, 4
#define R_XHCI_MEM_HOST_CTRL_BW_CTRL_REG                0x8100  ///< HOST_CTRL_BW_CTRL_REG - Host Controller Bandwidth Control Register
#define R_XHCI_MEM_HOST_IF_CTRL_REG                     0x8108  ///< HOST_IF_CTRL_REG - Host Controller Interface Control Register
#define R_XHCI_MEM_HOST_CTRL_TRM_REG2                   0x8110  ///< HOST_CTRL_TRM_REG2 - Host Controller Transfer Manager Control 2
#define R_XHCI_MEM_HOST_CTRL_BW_MAX_REG                 0x8128  ///< HOST_CTRL_BW_MAX_REG - Max BW Control Reg 4
#define B_XHCI_MEM_HOST_CTRL_BW_MAX_REG_MAX_HS_BW       0xFFF000///< HOST_CTRL_BW_MAX_REG - Max. Number of BW units for HS ports
#define N_XHCI_MEM_HOST_CTRL_BW_MAX_REG_MAX_HS_BW       12      ///< HOST_CTRL_BW_MAX_REG - Max. Number of BW units for HS ports position
#define R_XHCI_MEM_HOST_CTRL_BW_MAX_REG_HI              R_XHCI_MEM_HOST_CTRL_BW_MAX_REG + 4  ///< HOST_CTRL_BW_MAX_REG, upper part of register
#define B_XHCI_MEM_HOST_CTRL_BW_MAX_REG_HI_TT_MAX_BW    0xFFF0
#define R_XHCI_MEM_HOST_IF_PWR_CTRL_REG0                0x8140  ///< HOST_IF_PWR_CTRL_REG0 - Power Scheduler Control 0
#define R_XHCI_MEM_HOST_IF_PWR_CTRL_REG1                0x8144  ///< HOST_IF_PWR_CTRL_REG1 - Power Scheduler Control 1
#define R_XHCI_MEM_AUX_CTRL_REG2                        0x8154  ///< AUX_CTRL_REG2 - Aux PM Control Register 2
#define R_XHCI_MEM_USB2PHYPM                            0x8164  ///< USB2 PHY Power Management Control
#define R_XHCI_MEM_AUXCLKCTL                            0x816C  ///< xHCI Aux Clock Control Register
#define B_XHCI_MEM_AUXCLKCTL_PARUSB3_ENG_GEN            BIT19  ///< USB3 Partition Engine/Link trunk gating Enable
#define B_XHCI_MEM_AUXCLKCTL_PARUSB3_LINK_GEN           BIT18  ///< USB3 Partition Frame Timer trunk gating Enable
#define B_XHCI_MEM_AUXCLKCTL_PARUSB2_CLK_GEN            BIT17  ///< USB2 link partition clock gating enable
#define B_XHCI_MEM_AUXCLKCTL_USHIP_PCGEN                BIT16  ///< USB2/USHIP 12.5 MHz partition clock gating enable
#define B_XHCI_MEM_AUXCLKCTL_USB3_AC_CGE                BIT14  ///< USB3 Port Aux/Core clock gating enable
#define B_XHCI_MEM_AUXCLKCTL_U2R_BM_CG                  0xF00  ///< U2 Residency Before ModPHY Clock Gating
#define B_XHCI_MEM_AUXCLKCTL_XHCI_AC_GE                 BIT5   ///< XHCI Engine Aux clock gating enable
#define B_XHCI_MEM_AUXCLKCTL_XHCI_APMB_CGE              BIT4   ///< XHCI Aux PM block clock gating enable
#define B_XHCI_MEM_AUXCLKCTL_USB3_AC_TGE                BIT3   ///< USB3 Aux Clock Trunk Gating Enable
#define B_XHCI_MEM_AUXCLKCTL_USB3_AP_CGE                BIT2   ///< USB3 Port Aux/Port clock gating enable
#define R_XHCI_MEM_USBLPM                               0x8170  ///< USB LPM Parameters
#define N_XHCI_MEM_USBLPM_USB2_LPM_REG_RSM_U3_DET_NORM  22
#define B_XHCI_MEM_USBLPM_USB2_LPM_REG_RSM_U3_DET_NORM  0xFFC00000
#define V_XHCI_MEM_USBLPM_USB2_LPM_REG_RSM_U3_DET_NORM  0x32
#define B_XHCI_MEM_USBLPM_MIN_U2_ELFPS_D                (BIT18 | BIT17 | BIT16) ///< Min U2 Exit LFPS Duration
#define R_XHCI_MEM_XHCLTVCTL                            0x8174  ///< xHC Latency Tolerance Parameters - LTV Control
#define B_XHCI_MEM_XHCLTVCTL_USB2_PL0_LTV               0xFFF   ///< USB2 Port L0 LTV
#define R_XHCI_MEM_LTVHIT                               0x817C  ///< xHC Latency Tolerance Parameters - High Idle Time Control
#define R_XHCI_MEM_LTVMIT                               0x8180  ///< xHC Latency Tolerance Parameters - Medium Idle Time Control
#define R_XHCI_MEM_LTVLIT                               0x8184  ///< xHC Latency Tolerance Parameters - Low Idle Time Control
#define R_XHCI_MEM_XECP_CMDM_CTRL_REG1                  0x818C  ///< Command Manager Control 1
#define R_XHCI_MEM_XECP_CMDM_CTRL_REG2                  0x8190  ///< Command Manager Control 2
#define R_XHCI_MEM_XECP_CMDM_CTRL_REG3                  0x8194  ///< Command Manager Control 3
#define R_XHCI_MEM_PDDIS                                0x8198  ///< xHC Pulldown Disable Control
#define R_XHCI_MEM_THROTT                               0x819C  ///< XHCI Throttle Control
#define R_XHCI_MEM_LFPSPM                               0x81A0  ///< LFPS PM Control
#define R_XHCI_MEM_THROTT2                              0x81B4  ///< XHCI Throttle
#define R_XHCI_MEM_LFPSONCOUNT                          0x81B8  ///< LFPS On Count
#define R_XHCI_MEM_D0I2CTRL                             0x81BC  ///< D0I2 Control Register
#define B_XHCI_MEM_D0I2CTRL_MSI_IDLE_THRESHOLD          0xFFF0  ///< Bitmask for MSI Idle Threshold
#define N_XHCI_MEM_D0I2CTRL_MSI_IDLE_THRESHOLD          4       ///< Bitshift for MSI Idle Threshold
#define R_XHCI_MEM_D0I2SCH_ALARM_CTRL                   0x81C0  ///< D0i2 Scheduler Alarm Control Register
#define R_XHCI_MEM_USB2PMCTRL                           0x81C4  ///< USB2 Power Management Control
#define B_XHCI_MEM_USB2PMCTRL_U2PSPGPSCBP               BIT11                  ///< USB2 PHY SUS Power Gate PORTSC Block Policy
#define V_XHCI_MEM_USB2PMCTRL_U2PSPGEHC                 1
#define N_XHCI_MEM_USB2PMCTRL_U2PSPGEHC                 8 
#define B_XHCI_MEM_USB2PMCTRL_U2PSPGEHC                 (BIT10 | BIT9 | BIT8)  ///< USB2 PHY SUS Well Power Gate Entry Hysteresis Count
#define B_XHCI_MEM_USB2PMCTRL_U2PSUSPGP                 (BIT3 | BIT2)          ///< USB2 PHY SUS Well Power Gate Policy
#define B_XHCI_MEM_USB2PMCTRL_U2CLPGEL1L2               BIT1                   ///< USB2 Common Lane Power Gating Enable During L1 to L2 Mapping for USB2 PHY Power Gating
#define R_XHCI_MEM_AUX_CTRL_REG3                        0x81C8  ///< Aux PM Control 3 Register
#define R_XHCI_MEM_TRBPRFCTRLREG1                       0x81D0  ///< TRB Prefetch Control Register 1
#define R_XHCI_MEM_TRBPRFCACHEINVREG                    0x81D8  ///< TRB Prefetch Cache Invalidation Register 1
#define B_XHCI_MEM_TRBPRFCACHEINVREG_EN_TRB_FLUSH       0x7F    ///< TRB Flushing for various commands
#define N_XHCI_MEM_TRBPRFCACHEINVREG_EN_TRB_FLUSH       17      ///< Enable TRB flushing for various command

#define R_XHCI_MEM_DBGDEV_CTRL_REG1             0x8754    ///< Debug Device Control Register 1

#define R_XHCI_MEM_PMCTRL2                      0x8468    ///< PMCTRL2 - Power Management Control 2
#define R_XHCI_MEM_HOST_CTRL_SUS_LINK_PORT_REG  0x81F8
#define R_XHCI_MEM_PMREQ_CTRL_REG               0x83D0    ///< PMREQ Control Register
#define R_XHCI_MEM_ENH_CLK_GATE_CTRL            0x83D8    ///< Enhanced Clock Gate Control Policy Register
#define R_XHCI_MEM_HOST_CTRL_SSP_LINK_REG2      0x8E68
#define R_XHCI_MEM_HOST_CTRL_SSP_LFPS_REG2      0x8E74
#define R_XHCI_MEM_HOST_CTRL_SSP_LFPS_REG3      0x8E78
#define R_XHCI_MEM_HOST_CTRL_SSP_CONFIG_REG1    0x8E80
#define R_XHCI_MEM_HOST_CTRL_SSP_CONFIG_REG2    0x8E9C
#define R_XHCI_MEM_HOST_CTRL_USB3_RECAL         0x8E84

#define R_XHCI_MEM_USBLEGCTLSTS                 0x8470    ///< USB Legacy Support Control Status
#define B_XHCI_MEM_USBLEGCTLSTS_SMIBAR          BIT31     ///< SMI on BAR Status
#define B_XHCI_MEM_USBLEGCTLSTS_SMIPCIC         BIT30     ///< SMI on PCI Command Status
#define B_XHCI_MEM_USBLEGCTLSTS_SMIOSC          BIT29     ///< SMI on OS Ownership Change Status
#define B_XHCI_MEM_USBLEGCTLSTS_SMIBARE         BIT15     ///< SMI on BAR Enable
#define B_XHCI_MEM_USBLEGCTLSTS_SMIPCICE        BIT14     ///< SMI on PCI Command Enable
#define B_XHCI_MEM_USBLEGCTLSTS_SMIOSOE         BIT13     ///< SMI on OS Ownership Enable
#define B_XHCI_MEM_USBLEGCTLSTS_SMIHSEE         BIT4      ///< SMI on Host System Error Enable
#define B_XHCI_MEM_USBLEGCTLSTS_USBSMIE         BIT0      ///< USB SMI Enable

//
// Extended Capability Registers
//
#define R_PCH_XHCI_MEM_USB2PDO                  0x84F8    ///< USB2 Port Disable Override register
#define R_PCH_XHCI_MEM_USB3PDO                  0x84FC    ///< USB3 Port Disable Override register

//
// SOC HW State Save Register1
//
#define R_XHCI_MEM_SOCHWSTSAVE1                 0x8504
#define B_XHCI_MEM_SOCHWSTSAVE1_CMD_SSV         BIT31    ///< CMD save indication that scratchpad data is valid

//
// Debug Capability Descriptor Parameters
//
#define B_XHCI_MEM_CAPABILITY_ID                0xFF      ///< Capability ID
#define B_XHCI_MEM_CAPABILITY_NEXT_CAP_PTR      0xFF00    ///< Next Capability Pointer
#define N_XHCI_MEM_CAPABILITY_NEXT_CAP_PTR      8         ///< Byte shift for next capability pointer
#define V_XHCI_MEM_DBC_DCID                     0x0A      ///< Debug Capability ID
#define R_XHCI_MEM_DBC_DCCTRL                   0x20      ///< Debug Capability Control Register (DCCTRL)
#define B_XHCI_MEM_DBC_DCCTRL_DCR               BIT0      ///< Debug Capability - DbC Run (DCR)
#define R_XHCI_MEM_DBC_DCST                     0x24      ///< Debug Capability Status Register (DCST)
#define B_XHCI_MEM_DBC_DCST_DBG_PORT_NUMBER     0xFF      ///< Debug Port Number Mask
#define N_XHCI_MEM_DBC_DCST_DBG_PORT_NUMBER     24        ///< Debug Port Number Offset in DCST register
#define R_XHCI_MEM_DBC_DBCCTL                   0x8760    ///< DBCCTL - DbC Control
#define B_XHCI_MEM_DBC_DBCCTL_DISC_RXD_CNT      0x1F      ///< Soft Disconnect RX Detect Count mask
#define N_XHCI_MEM_DBC_DBCCTL_DISC_RXD_CNT      2         ///< Soft Disconnect RX Detect Count bitshift

//
// Over Current Mapping registers
//
#define R_XHCI_MEM_U2OCM                        0x90A4    ///< XHC USB2 Overcurrent Pin N Mapping
#define R_XHCI_MEM_U3OCM                        0x9124    ///< XHC USB3 Overcurrent Pin N Mapping

//
// xHCI Private registers
//
#define R_XHCI_PCR_DAP_USB2PORT_STATUS_0        0x508     ///< DAP USB2 Port0 Status 0 Register
#define B_XHCI_PCR_DAP_USB2PORT_STATUS_0_OS     0xFF      ///< Operation State (OS) in DAP USB2 Port<N> Status 0 Register
#define V_XHCI_PCR_DAP_USB2PORT_STATUS_0_OS_DBC 0x40      ///< DBC Operation State

//
// xDCI (OTG) USB Device Controller
//
#define PCI_DEVICE_NUMBER_PCH_XDCI              20
#define PCI_FUNCTION_NUMBER_PCH_XDCI            1

//
// xDCI (OTG) PCI Config Space Registers
//
#define R_XDCI_CFG_MEM_BASE                   0x10
#define V_XDCI_CFG_MEM_LENGTH                 0x200000
#define R_XDCI_CFG_PMCSR                      0x84      ///< Power Management Control and Status Register
#define R_XDCI_CFG_GENERAL_PURPOSER_REG1      0xA0      ///< General Purpose PCI RW Register1
#define R_XDCI_CFG_CPGE                       0xA2      ///< Chassis Power Gate Enable
#define R_XDCI_CFG_GENERAL_PURPOSER_REG4      0xAC      ///< General Purpose PCI RW Register4
#define R_XDCI_CFG_GENERAL_INPUT_REG          0xC0      ///< General Input Register

//
// xDCI (OTG) MMIO registers
//
#define R_XDCI_MEM_GCTL                       0xC110  ///< Xdci Global Ctrl
#define B_XDCI_MEM_GCTL_GHIBEREN              BIT1    ///< Hibernation enable
#define R_XDCI_MEM_GUSB2PHYCFG                0xC200  ///< Global USB2 PHY Configuration Register
#define B_XDCI_MEM_GUSB2PHYCFG_SUSPHY         BIT6    ///< Suspend USB2.0 HS/FS/LS PHY
#define R_XDCI_MEM_GUSB3PIPECTL0              0xC2C0  ///< Global USB3 PIPE Control Register 0
#define B_XDCI_MEM_GUSB3PIPECTL0_SUSPEN_EN    BIT17   ///< Suspend USB3.0 SS PHY (Suspend_en)
#define B_XDCI_MEM_GUSB3PIPECTL0_UX_IN_PX     BIT27   ///< Ux Exit in Px
#define R_XDCI_MEM_APBFC_U3PMU_CFG2           0x10F810
#define R_XDCI_MEM_APBFC_U3PMU_CFG4           0x10F818
#define R_XDCI_MEM_APBFC_U3PMU_CFG5           0x10F81C
#define R_XDCI_MEM_APBFC_U3PMU_CFG6           0x10F820

//
// xDCI (OTG) Private Configuration Registers
// (PID:OTG)
// @todo: Verify PCR vaLidity for CPU
//
#define R_OTG_PCR_IOSF_A2                 0xA2
#define R_OTG_PCR_IOSF_PMCTL              0x1D0
#define R_OTG_PCR_PCICFGCTRL1             0x200
#define B_OTG_PCR_PCICFGCTRL_PCI_IRQ      0x0FF00000
#define N_OTG_PCR_PCICFGCTRL_PCI_IRQ      20
#define B_OTG_PCR_PCICFGCTRL_ACPI_IRQ     0x000FF000
#define N_OTG_PCR_PCICFGCTRL_ACPI_IRQ     12
#define B_OTG_PCR_PCICFGCTRL_INT_PIN      0x00000F00
#define N_OTG_PCR_PCICFGCTRL_INT_PIN      8
#define B_OTG_PCR_PCICFGCTRL_BAR1_DIS     0x00000080
#define B_OTG_PCR_PCICFGCTRL_PME_SUP      0x0000007C
#define B_OTG_PCR_PCICFGCTRL_ACPI_INT_EN  0x00000002
#define B_OTG_PCR_PCICFGCTRL_PCI_CFG_DIS  0x00000001

//
// USB2 Private Configuration Registers
// USB2 HIP design featured
// (PID:USB2)
// Doesn't apply to CPU (only USB3 functionality present)
//
#define R_USB2_PCR_GLOBAL_PORT            0x4001    ///< USB2 GLOBAL PORT
#define R_USB2_PCR_400C                   0x400C
#define R_USB2_PCR_PP_LANE_BASE_ADDR      0x4000    ///< PP LANE base address
#define V_USB2_PCR_PER_PORT               0x00      ///< USB2 PER PORT          Addr[7:2] = 0x00
#define V_USB2_PCR_PER_PORT_RXISET        0x04      ///< PERPORTRXISET bits value in USB2 PER PORT register
#define V_USB2_PCR_UTMI_MISC_PER_PORT     0x08      ///< UTMI MISC REG PER PORT Addr[7:2] = 0x08
#define V_USB2_PCR_PER_PORT_2             0x26      ///< USB2 PER PORT 2        Addr[7:2] = 0x26
#define V_USB2_PCR_PER_PORT_2_HSSKEWSEL   0x01      ///< HSSKEWSEL bits value USB2 PER PORT2 register
#define V_USB2_PCR_PER_PORT_2_SKEWDELAY   0x03      ///< HSNPREDRVSEL bits value USB2 PER PORT2 register
#define R_USB2_PCR_402A                   0x402A
#define R_USB2_PCR_GLB_ADP_VBUS_REG       0x402B    ///< GLB ADP VBUS REG
#define R_USB2_PCR_GLOBAL_PORT_2          0x402C    ///< USB2 GLOBAL PORT 2
#define R_USB2_PCR_PLLDIVRATIOS_0         0x7000    ///< PLLDIVRATIOS_0
#define R_USB2_PCR_CONFIG_0               0x7008    ///< CONFIG_0
#define R_USB2_PCR_CONFIG_3               0x7014    ///< CONFIG_3
#define R_USB2_PCR_DFT_1                  0x7024    ///< DFT_1
#define R_USB2_PCR_SFRCONFIG_0            0x702C    ///< SFRCONFIG_0
#define R_USB2_PCR_7034                   0x7034
#define R_USB2_PCR_7038                   0x7038
#define R_USB2_PCR_703C                   0x703C
#define R_USB2_PCR_7040                   0x7040
#define R_USB2_PCR_7044                   0x7044
#define R_USB2_PCR_7048                   0x7048
#define R_USB2_PCR_704C                   0x704C
#define R_USB2_PCR_PLL1                   0x7F02    ///< USB2 PLL1
#define R_USB2_PCR_CFG_COMPBG             0x7F04    ///< USB2 COMPBG

#define R_PCH_USB2IO_MSG_PLLDIVRATIOS_0                                   0x7000  // PLLDIVRATIOS_0
#define S_PCH_USB2IO_MSG_PLLDIVRATIOS_0                                        4
#define B_PCH_USB2IO_MSG_PLLDIVRATIOS_0_i_fbdivratio                  0xFF000000  //> i_fbdivratio [31:24]
#define N_PCH_USB2IO_MSG_PLLDIVRATIOS_0_i_fbdivratio                          24
#define B_PCH_USB2IO_MSG_PLLDIVRATIOS_0_int_fbdiv                       0xFF0000  //> int_fbdiv [23:16]
#define N_PCH_USB2IO_MSG_PLLDIVRATIOS_0_int_fbdiv                             16
#define B_PCH_USB2IO_MSG_PLLDIVRATIOS_0_i_ndiv                            0xF000  //> i_ndiv [15:12]
#define N_PCH_USB2IO_MSG_PLLDIVRATIOS_0_i_ndiv                                12

#define R_PCH_USB2IO_MSG_CONFIG_0                                         0x7008  // CONFIG_0
#define S_PCH_USB2IO_MSG_CONFIG_0                                              4
#define B_PCH_USB2IO_MSG_CONFIG_0_i_force_tdccalib                    0x80000000  //> i_force_tdccalib [31:31]
#define N_PCH_USB2IO_MSG_CONFIG_0_i_force_tdccalib                            31
#define B_PCH_USB2IO_MSG_CONFIG_0_i_force_pwrgateoff                  0x40000000  //> i_force_pwrgateoff [30:30]
#define N_PCH_USB2IO_MSG_CONFIG_0_i_force_pwrgateoff                          30
#define B_PCH_USB2IO_MSG_CONFIG_0_i_force_clkgateoff                  0x20000000  //> i_force_clkgateoff [29:29]
#define N_PCH_USB2IO_MSG_CONFIG_0_i_force_clkgateoff                          29
#define B_PCH_USB2IO_MSG_CONFIG_0_force_boot                          0x10000000  //> force_boot [28:28]
#define N_PCH_USB2IO_MSG_CONFIG_0_force_boot                                  28
#define B_PCH_USB2IO_MSG_CONFIG_0_force_dcoen                          0x8000000  //> force_dcoen [27:27]
#define N_PCH_USB2IO_MSG_CONFIG_0_force_dcoen                                 27
#define B_PCH_USB2IO_MSG_CONFIG_0_force_digcoreen                      0x4000000  //> force_digcoreen [26:26]
#define N_PCH_USB2IO_MSG_CONFIG_0_force_digcoreen                             26
#define B_PCH_USB2IO_MSG_CONFIG_0_i_lockwdet_win                       0x3000000  //> i_lockwdet_win [25:24]
#define N_PCH_USB2IO_MSG_CONFIG_0_i_lockwdet_win                              24
#define B_PCH_USB2IO_MSG_CONFIG_0_i_lockdet_cnt                         0x800000  //> i_lockdet_cnt [23:23]
#define N_PCH_USB2IO_MSG_CONFIG_0_i_lockdet_cnt                               23
#define B_PCH_USB2IO_MSG_CONFIG_0_i_fine_limit                          0x600000  //> i_fine_limit [22:21]
#define N_PCH_USB2IO_MSG_CONFIG_0_i_fine_limit                                21
#define B_PCH_USB2IO_MSG_CONFIG_0_i_refclkdly_sel                       0x180000  //> i_refclkdly_sel [20:19]
#define N_PCH_USB2IO_MSG_CONFIG_0_i_refclkdly_sel                             19
#define B_PCH_USB2IO_MSG_CONFIG_0_refclk2fnlch_sel                       0x40000  //> refclk2fnlch_sel [18:18]
#define N_PCH_USB2IO_MSG_CONFIG_0_refclk2fnlch_sel                            18
#define B_PCH_USB2IO_MSG_CONFIG_0_i_int_coeff                            0x3E000  //> i_int_coeff [17:13]
#define N_PCH_USB2IO_MSG_CONFIG_0_i_int_coeff                                 13
#define B_PCH_USB2IO_MSG_CONFIG_0_i_prop_coeff                            0x1E00  //> i_prop_coeff [12:9]
#define N_PCH_USB2IO_MSG_CONFIG_0_i_prop_coeff                                 9

#define R_PCH_USB2IO_MSG_CONFIG_1                                         0x700C  // CONFIG_1
#define S_PCH_USB2IO_MSG_CONFIG_1                                              4
#define B_PCH_USB2IO_MSG_CONFIG_1_i_fbclk_extsel                      0x80000000  //> i_fbclk_extsel [31:31]
#define N_PCH_USB2IO_MSG_CONFIG_1_i_fbclk_extsel                              31
#define B_PCH_USB2IO_MSG_CONFIG_1_i_dcoclk_extsel                     0x40000000  //> i_dcoclk_extsel [30:30]
#define N_PCH_USB2IO_MSG_CONFIG_1_i_dcoclk_extsel                             30
#define B_PCH_USB2IO_MSG_CONFIG_1_clkgdly_cnt                         0x38000000  //> clkgdly_cnt [29:27]
#define N_PCH_USB2IO_MSG_CONFIG_1_clkgdly_cnt                                 27
#define B_PCH_USB2IO_MSG_CONFIG_1_config_1_bonus                       0x4000000  //> config_1_bonus [26:26]
#define N_PCH_USB2IO_MSG_CONFIG_1_config_1_bonus                              26
#define B_PCH_USB2IO_MSG_CONFIG_1_i_dcodither_en                       0x2000000  //> i_dcodither_en [25:25]
#define N_PCH_USB2IO_MSG_CONFIG_1_i_dcodither_en                              25
#define B_PCH_USB2IO_MSG_CONFIG_1_i_dcodither_clk_sel                  0x1800000  //> i_dcodither_clk_sel [24:23]
#define N_PCH_USB2IO_MSG_CONFIG_1_i_dcodither_clk_sel                         23
#define B_PCH_USB2IO_MSG_CONFIG_1_dcodither_ovrd                        0x400000  //> dcodither_ovrd [22:22]
#define N_PCH_USB2IO_MSG_CONFIG_1_dcodither_ovrd                              22
#define B_PCH_USB2IO_MSG_CONFIG_1_dcodither_val                         0x200000  //> dcodither_val [21:21]
#define N_PCH_USB2IO_MSG_CONFIG_1_dcodither_val                               21
#define B_PCH_USB2IO_MSG_CONFIG_1_pemod_frq                             0x1E0000  //> pemod_frq [20:17]
#define N_PCH_USB2IO_MSG_CONFIG_1_pemod_frq                                   17
#define B_PCH_USB2IO_MSG_CONFIG_1_pemod_amp                              0x1F800  //> pemod_amp [16:11]
#define N_PCH_USB2IO_MSG_CONFIG_1_pemod_amp                                   11

#define R_PCH_USB2IO_MSG_CONFIG_2                                         0x7010  // CONFIG_2
#define S_PCH_USB2IO_MSG_CONFIG_2                                              4
#define B_PCH_USB2IO_MSG_CONFIG_2_digconfig                           0xFFFF0000  //> digconfig [31:16]
#define N_PCH_USB2IO_MSG_CONFIG_2_digconfig                                   16
#define B_PCH_USB2IO_MSG_CONFIG_2_i_pllconfig                             0xFFFF  //> i_pllconfig [15:0]
#define N_PCH_USB2IO_MSG_CONFIG_2_i_pllconfig                                  0

#define R_PCH_USB2IO_MSG_CONFIG_3                                         0x7014  // CONFIG_3
#define S_PCH_USB2IO_MSG_CONFIG_3                                              4
#define B_PCH_USB2IO_MSG_CONFIG_3_i_sfrcalib_fmincnt                  0xFF000000  //> i_sfrcalib_fmincnt [31:24]
#define N_PCH_USB2IO_MSG_CONFIG_3_i_sfrcalib_fmincnt                          24
#define B_PCH_USB2IO_MSG_CONFIG_3_i_sfrcalib_fmaxcnt                    0xFF0000  //> i_sfrcalib_fmaxcnt [23:16]
#define N_PCH_USB2IO_MSG_CONFIG_3_i_sfrcalib_fmaxcnt                          16
#define B_PCH_USB2IO_MSG_CONFIG_3_i_tdccalib_osccnt                       0xFF00  //> i_tdccalib_osccnt [15:8]
#define N_PCH_USB2IO_MSG_CONFIG_3_i_tdccalib_osccnt                            8
#define B_PCH_USB2IO_MSG_CONFIG_3_i_locktimercnt_th                         0xFE  //> i_locktimercnt_th [7:1]
#define N_PCH_USB2IO_MSG_CONFIG_3_i_locktimercnt_th                            1

#define R_PCH_USB2IO_MSG_DFT_0                                            0x7020  // DFT_0
#define S_PCH_USB2IO_MSG_DFT_0                                                 4
#define B_PCH_USB2IO_MSG_DFT_0_tdctrim_val                            0xF0000000  //> tdctrim_val [31:28]
#define N_PCH_USB2IO_MSG_DFT_0_tdctrim_val                                    28
#define B_PCH_USB2IO_MSG_DFT_0_tdctrim_ovrd                            0x8000000  //> tdctrim_ovrd [27:27]
#define N_PCH_USB2IO_MSG_DFT_0_tdctrim_ovrd                                   27
#define B_PCH_USB2IO_MSG_DFT_0_dcofine_val                             0x7FC0000  //> dcofine_val [26:18]
#define N_PCH_USB2IO_MSG_DFT_0_dcofine_val                                    18
#define B_PCH_USB2IO_MSG_DFT_0_dcofine_ovrd                              0x20000  //> dcofine_ovrd [17:17]
#define N_PCH_USB2IO_MSG_DFT_0_dcofine_ovrd                                   17
#define B_PCH_USB2IO_MSG_DFT_0_dcocoarse_val                             0x1FC00  //> dcocoarse_val [16:10]
#define N_PCH_USB2IO_MSG_DFT_0_dcocoarse_val                                  10
#define B_PCH_USB2IO_MSG_DFT_0_dcocoarse_ovrd                              0x200  //> dcocoarse_ovrd [9:9]
#define N_PCH_USB2IO_MSG_DFT_0_dcocoarse_ovrd                                  9
#define B_PCH_USB2IO_MSG_DFT_0_i_sfr_spare                                 0x1E0  //> i_sfr_spare [8:5]
#define N_PCH_USB2IO_MSG_DFT_0_i_sfr_spare                                     5
#define B_PCH_USB2IO_MSG_DFT_0_fine_minmax_selcnt                           0x10  //> fine_minmax_selcnt [4:4]
#define N_PCH_USB2IO_MSG_DFT_0_fine_minmax_selcnt                              4
#define B_PCH_USB2IO_MSG_DFT_0_fine_minmax_timer                             0x8  //> fine_minmax_timer [3:3]
#define N_PCH_USB2IO_MSG_DFT_0_fine_minmax_timer                               3

#define R_PCH_USB2IO_MSG_DFT_1                                            0x7024  // DFT_1
#define S_PCH_USB2IO_MSG_DFT_1                                                 4
#define B_PCH_USB2IO_MSG_DFT_1_dcolat_byp                             0x80000000  //> dcolat_byp [31:31]
#define N_PCH_USB2IO_MSG_DFT_1_dcolat_byp                                     31
#define B_PCH_USB2IO_MSG_DFT_1_tdcbbpd_en                             0x40000000  //> tdcbbpd_en [30:30]
#define N_PCH_USB2IO_MSG_DFT_1_tdcbbpd_en                                     30
#define B_PCH_USB2IO_MSG_DFT_1_o_dcsense_chopsel                      0x30000000  //> o_dcsense_chopsel [29:28]
#define N_PCH_USB2IO_MSG_DFT_1_o_dcsense_chopsel                              28
#define B_PCH_USB2IO_MSG_DFT_1_force_cloopen                           0x8000000  //> force_cloopen [27:27]
#define N_PCH_USB2IO_MSG_DFT_1_force_cloopen                                  27
#define B_PCH_USB2IO_MSG_DFT_1_force_tdcosc                            0x4000000  //> force_tdcosc [26:26]
#define N_PCH_USB2IO_MSG_DFT_1_force_tdcosc                                   26
#define B_PCH_USB2IO_MSG_DFT_1_i_force_fbdivrstoff                     0x2000000  //> i_force_fbdivrstoff [25:25]
#define N_PCH_USB2IO_MSG_DFT_1_i_force_fbdivrstoff                            25
#define B_PCH_USB2IO_MSG_DFT_1_dcocalib_cnthalf                        0x1000000  //> dcocalib_cnthalf [24:24]
#define N_PCH_USB2IO_MSG_DFT_1_dcocalib_cnthalf                               24
#define B_PCH_USB2IO_MSG_DFT_1_narrow_lockwin                           0x800000  //> narrow_lockwin [23:23]
#define N_PCH_USB2IO_MSG_DFT_1_narrow_lockwin                                 23
#define B_PCH_USB2IO_MSG_DFT_1_fine_pe_minmaxen                         0x400000  //> fine_pe_minmaxen [22:22]
#define N_PCH_USB2IO_MSG_DFT_1_fine_pe_minmaxen                               22
#define B_PCH_USB2IO_MSG_DFT_1_fine_pe_minmaxrst                        0x200000  //> fine_pe_minmaxrst [21:21]
#define N_PCH_USB2IO_MSG_DFT_1_fine_pe_minmaxrst                              21
#define B_PCH_USB2IO_MSG_DFT_1_p2p_fine_pe_select                       0x100000  //> p2p_fine_pe_select [20:20]
#define N_PCH_USB2IO_MSG_DFT_1_p2p_fine_pe_select                             20
#define B_PCH_USB2IO_MSG_DFT_1_dft_1_bonus                               0x80000  //> dft_1_bonus [19:19]
#define N_PCH_USB2IO_MSG_DFT_1_dft_1_bonus                                    19
#define B_PCH_USB2IO_MSG_DFT_1_fine_sweepen                              0x40000  //> fine_sweepen [18:18]
#define N_PCH_USB2IO_MSG_DFT_1_fine_sweepen                                   18
#define B_PCH_USB2IO_MSG_DFT_1_coarsesweep_en                            0x20000  //> coarsesweep_en [17:17]
#define N_PCH_USB2IO_MSG_DFT_1_coarsesweep_en                                 17
#define B_PCH_USB2IO_MSG_DFT_1_tdcsweep_en                               0x10000  //> tdcsweep_en [16:16]
#define N_PCH_USB2IO_MSG_DFT_1_tdcsweep_en                                    16
#define B_PCH_USB2IO_MSG_DFT_1_i_kpscale                                  0xC000  //> i_kpscale [15:14]
#define N_PCH_USB2IO_MSG_DFT_1_i_kpscale                                      14
#define B_PCH_USB2IO_MSG_DFT_1_i_kiscale                                  0x3000  //> i_kiscale [13:12]
#define N_PCH_USB2IO_MSG_DFT_1_i_kiscale                                      12
#define B_PCH_USB2IO_MSG_DFT_1_i_kpkiscale_cnt                             0xC00  //> i_kpkiscale_cnt [11:10]
#define N_PCH_USB2IO_MSG_DFT_1_i_kpkiscale_cnt                                10
#define B_PCH_USB2IO_MSG_DFT_1_fine_pe_minmax_p2p                          0x3FE  //> fine_pe_minmax_p2p [9:1]
#define N_PCH_USB2IO_MSG_DFT_1_fine_pe_minmax_p2p                              1
#define B_PCH_USB2IO_MSG_DFT_1_lock_boost_en                                 0x1  //> lock_boost_en [0:0]
#define N_PCH_USB2IO_MSG_DFT_1_lock_boost_en                                   0

#define R_PCH_USB2IO_MSG_DFT_2                                            0x7028  // DFT_2
#define S_PCH_USB2IO_MSG_DFT_2                                                 4
#define B_PCH_USB2IO_MSG_DFT_2_o_digtop_obs                           0xF0000000  //> o_digtop_obs [31:28]
#define N_PCH_USB2IO_MSG_DFT_2_o_digtop_obs                                   28
#define B_PCH_USB2IO_MSG_DFT_2_tdc_scan_err                            0x8000000  //> tdc_scan_err [27:27]
#define N_PCH_USB2IO_MSG_DFT_2_tdc_scan_err                                   27
#define B_PCH_USB2IO_MSG_DFT_2_narrow_lock                             0x4000000  //> narrow_lock [26:26]
#define N_PCH_USB2IO_MSG_DFT_2_narrow_lock                                    26
#define B_PCH_USB2IO_MSG_DFT_2_lock_fail                               0x2000000  //> lock_fail [25:25]
#define N_PCH_USB2IO_MSG_DFT_2_lock_fail                                      25
#define B_PCH_USB2IO_MSG_DFT_2_sequence_fail                           0x1000000  //> sequence_fail [24:24]
#define N_PCH_USB2IO_MSG_DFT_2_sequence_fail                                  24
#define B_PCH_USB2IO_MSG_DFT_2_fn_ovrfl_pos                             0x800000  //> fn_ovrfl_pos [23:23]
#define N_PCH_USB2IO_MSG_DFT_2_fn_ovrfl_pos                                   23
#define B_PCH_USB2IO_MSG_DFT_2_fn_ovrfl_neg                             0x400000  //> fn_ovrfl_neg [22:22]
#define N_PCH_USB2IO_MSG_DFT_2_fn_ovrfl_neg                                   22
#define B_PCH_USB2IO_MSG_DFT_2_p1_ovrflw_pos                            0x200000  //> p1_ovrflw_pos [21:21]
#define N_PCH_USB2IO_MSG_DFT_2_p1_ovrflw_pos                                  21
#define B_PCH_USB2IO_MSG_DFT_2_p1_ovrflw_neg                            0x100000  //> p1_ovrflw_neg [20:20]
#define N_PCH_USB2IO_MSG_DFT_2_p1_ovrflw_neg                                  20
#define B_PCH_USB2IO_MSG_DFT_2_sfrcalib_ovrfl                            0x80000  //> sfrcalib_ovrfl [19:19]
#define N_PCH_USB2IO_MSG_DFT_2_sfrcalib_ovrfl                                 19
#define B_PCH_USB2IO_MSG_DFT_2_sfrtrim_ovrfl                             0x40000  //> sfrtrim_ovrfl [18:18]
#define N_PCH_USB2IO_MSG_DFT_2_sfrtrim_ovrfl                                  18
#define B_PCH_USB2IO_MSG_DFT_2_obs_sel3                                  0x3C000  //> obs_sel3 [17:14]
#define N_PCH_USB2IO_MSG_DFT_2_obs_sel3                                       14
#define B_PCH_USB2IO_MSG_DFT_2_obs_sel2                                   0x3C00  //> obs_sel2 [13:10]
#define N_PCH_USB2IO_MSG_DFT_2_obs_sel2                                       10
#define B_PCH_USB2IO_MSG_DFT_2_obs_sel1                                    0x3C0  //> obs_sel1 [9:6]
#define N_PCH_USB2IO_MSG_DFT_2_obs_sel1                                        6
#define B_PCH_USB2IO_MSG_DFT_2_obs_sel0                                     0x3C  //> obs_sel0 [5:2]
#define N_PCH_USB2IO_MSG_DFT_2_obs_sel0                                        2
#define B_PCH_USB2IO_MSG_DFT_2_narrow_lock_late                              0x2  //> narrow_lock_late [1:1]
#define N_PCH_USB2IO_MSG_DFT_2_narrow_lock_late                                1

#define R_PCH_USB2IO_MSG_SFRCONFIG_0                                      0x702C  // SFRCONFIG_0
#define S_PCH_USB2IO_MSG_SFRCONFIG_0                                           4
#define B_PCH_USB2IO_MSG_SFRCONFIG_0_sfr_monsel                       0xFF000000  //> sfr_monsel [31:24]
#define N_PCH_USB2IO_MSG_SFRCONFIG_0_sfr_monsel                               24
#define B_PCH_USB2IO_MSG_SFRCONFIG_0_sfrcalib_val                       0xF00000  //> sfrcalib_val [23:20]
#define N_PCH_USB2IO_MSG_SFRCONFIG_0_sfrcalib_val                             20
#define B_PCH_USB2IO_MSG_SFRCONFIG_0_sfrtrim_val                         0xF0000  //> sfrtrim_val [19:16]
#define N_PCH_USB2IO_MSG_SFRCONFIG_0_sfrtrim_val                              16
#define B_PCH_USB2IO_MSG_SFRCONFIG_0_sfrcalib_ovrd                        0x8000  //> sfrcalib_ovrd [15:15]
#define N_PCH_USB2IO_MSG_SFRCONFIG_0_sfrcalib_ovrd                            15
#define B_PCH_USB2IO_MSG_SFRCONFIG_0_sfrtrim_ovrd                         0x4000  //> sfrtrim_ovrd [14:14]
#define N_PCH_USB2IO_MSG_SFRCONFIG_0_sfrtrim_ovrd                             14
#define B_PCH_USB2IO_MSG_SFRCONFIG_0_sfren_ovrd                           0x2000  //> sfren_ovrd [13:13]
#define N_PCH_USB2IO_MSG_SFRCONFIG_0_sfren_ovrd                               13
#define B_PCH_USB2IO_MSG_SFRCONFIG_0_sfren_val                            0x1000  //> sfren_val [12:12]
#define N_PCH_USB2IO_MSG_SFRCONFIG_0_sfren_val                                12
#define B_PCH_USB2IO_MSG_SFRCONFIG_0_sfr_bypass                            0x800  //> sfr_bypass [11:11]
#define N_PCH_USB2IO_MSG_SFRCONFIG_0_sfr_bypass                               11
#define B_PCH_USB2IO_MSG_SFRCONFIG_0_sfrtimer_cnt2x                        0x400  //> sfrtimer_cnt2x [10:10]
#define N_PCH_USB2IO_MSG_SFRCONFIG_0_sfrtimer_cnt2x                           10
#define B_PCH_USB2IO_MSG_SFRCONFIG_0_i_sfrsupplymode                       0x300  //> i_sfrsupplymode [9:8]
#define N_PCH_USB2IO_MSG_SFRCONFIG_0_i_sfrsupplymode                           8
#define B_PCH_USB2IO_MSG_SFRCONFIG_0_i_sfrtimer_coef                        0xF0  //> i_sfrtimer_coef [7:4]
#define N_PCH_USB2IO_MSG_SFRCONFIG_0_i_sfrtimer_coef                           4
#define B_PCH_USB2IO_MSG_SFRCONFIG_0_sfrlpfres_ovrd_sel                      0x8  //> sfrlpfres_ovrd_sel [3:3]
#define N_PCH_USB2IO_MSG_SFRCONFIG_0_sfrlpfres_ovrd_sel                        3
#define B_PCH_USB2IO_MSG_SFRCONFIG_0_sfrlpfres_ovrd_val                      0x4  //> sfrlpfres_ovrd_val [2:2]
#define N_PCH_USB2IO_MSG_SFRCONFIG_0_sfrlpfres_ovrd_val                        2
#define B_PCH_USB2IO_MSG_SFRCONFIG_0_digmode_en                              0x2  //> digmode_en [1:1]
#define N_PCH_USB2IO_MSG_SFRCONFIG_0_digmode_en                                1

//
// xHCI SSIC registers
//
#define R_XHCI_MEM_SSIC_CONF_REG2_PORT_1          0x880C    ///< SSIC Configuration Register 2 Port 1
#define R_XHCI_MEM_SSIC_CONF_REG2_PORT_2          0x883C    ///< SSIC Configuration Register 2 Port 2
#define B_XHCI_MEM_SSIC_CONF_REG2_PORT_UNUSED     BIT31
#define B_XHCI_MEM_SSIC_CONF_REG2_PROG_DONE       BIT30


//
// xHCI DCI MMIO Registers
//
#define R_XHCI_PCR_DCI_DBC_TRACE_IN_PAYLOAD_BP_LOW         0x50
#define R_XHCI_PCR_DCI_DBC_TRACE_IN_PAYLOAD_BP_HIGH        0x54
#define R_XHCI_PCR_DCI_DBC_TRACE_IN_PAYLOAD_QUALIFIERS     0x58
#define R_XHCI_PCR_DCI_DBC_TRACE_IN_STATUS_BP_LOW          0x60
#define R_XHCI_PCR_DCI_DBC_TRACE_IN_STATUS_BP_HIGH         0x64
#define R_XHCI_PCR_DCI_DBC_TRACE_IN_STATUS_QUALIFIERS      0x68
#define V_XHCI_PCR_DCI_DBC_TRACE_QUALIFIERS                0x22B800

//
// USBX DAP Private config registers
// (PID:PID_XHCI)
//
#define R_DAP_USB2_PORT_CONTROL_0_REG_0       0x500       ///< DAP USB2 Port Control 0 Register
#define V_PCR_USB_CONNECTOR_EVENT_MASK        0x000000E0  ///< In connector type aware flow, this field is used in SW mode only
#define V_PCR_USB_CONNECTOR_EVENT_HOST        0x00000000  ///< host subscription.
#define V_PCR_USB_CONNECTOR_EVENT_DEVICE      0x00000060  ///< device subscription.
#define V_PCR_USB_DISCONNRCT_CONNECTOR_EVENT  0x00000020  ///< un-subscription
#define V_PCR_USB_CONNECTOR_EVENT_DBC         0x00000080  ///< DBC subscription.
#define V_PCR_USB_SW_VBUS                     0x00000100  ///< SW_VBUS
#define R_DAP_USB2_PORT_CONTROL_1_REG_0       0x504       ///< DAP USB2 Port Control 1 Register
#define R_DAP_USB2_PORT_STATUS_REG_0          0x508       ///< DAP USB2 Port Status Register
#define V_PCR_USB_OP_STATUS_MASK              0x000000FF  ///< This field reflects live value of the DRD operation states with one-hot encodings
#define V_PCR_USB_OP_STATUS_HOST              0x00000001  ///< host
#define V_PCR_USB_OP_STATUS_DISCONNECT        0x00000002  ///< disconnected
#define V_PCR_USB_OP_STATUS_GUEST             0x00000004  ///< guest
#define V_PCR_USB_OP_STATUS_DEVICE            0x00000008  ///< device
#define V_PCR_USB_OP_STATUS_PHY_INIT          0x00000010  ///< PHY initialization. (default, dummy)
#define V_PCR_USB_OP_STATUS_EXI_BSSB          0x00000020  ///< EXI BSSB adapter connected.
#define V_PCR_USB_OP_STATUS_DBC               0x00000040  ///< DBC
#define V_PCR_USB_OP_STATUS_OV_SUB_DEV        0x00000080  ///< over-subscribed device.
#define V_PCR_USB_OP_STATUS_HW_VBUS           0x00010000  ///< Hardware VBUS
#define V_PCR_USB_OP_MAX_TIMEOUT_COUNT        0x00001000  ///< SPR program max count

#define R_DAP_ESS_PORT_CONTROL_0_REG_0        0x600       ///< DAP eSS Port Control 0 Register
#define R_DAP_ESS_PORT_STATUS_REG_0           0x608       ///< DAP eSS Port Status Register


#endif // _REGS_USB_H_
