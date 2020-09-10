/** @file
  GPIO pins for CDF-PCH-H,

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2018 Intel Corporation. <BR>

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

#ifndef _GPIO_PINS_CDF_
#define _GPIO_PINS_CDF_


//Restricted begin:
//Based on cedarfork_A0-17ww11b
//Restricted end:

///
/// Unique ID used in GpioPad defines
///
#define GPIO_CDF_H_CHIPSET_ID       0x3

#define GPIO_CDF_GROUP_GPP_A  0x0300
#define GPIO_CDF_GROUP_GPP_B  0x0301
#define GPIO_CDF_GROUP_GPP_C  0x0302
#define GPIO_CDF_GROUP_GPP_D  0x0303
#define GPIO_CDF_GROUP_GPP_E  0x0304
#define GPIO_CDF_GROUP_GPP_F  0x0305
#define GPIO_CDF_GROUP_GPP_G  0x0306
#define GPIO_CDF_GROUP_GPP_H  0x0307
#define GPIO_CDF_GROUP_GPP_I  0x0308
#define GPIO_CDF_GROUP_GPP_J  0x0309
#define GPIO_CDF_GROUP_GPP_K  0x030A
#define GPIO_CDF_GROUP_GPP_L  0x030B
#define GPIO_CDF_GROUP_GPP_M  0x030C



#define GPIO_CDF_GPP_A0                                     0x03000000 //GBE_SDP_TIMESYNC0
#define GPIO_CDF_GPP_A1                                     0x03000001 //GBE_SDP_TIMESYNC1
#define GPIO_CDF_GPP_A2                                     0x03000002 //GBE_SDP_TIMESYNC2
#define GPIO_CDF_GPP_A3                                     0x03000003 //GBE_SDP_TIMESYNC3
#define GPIO_CDF_GPP_A4                                     0x03000004 //GBE0_I2C_CLK
#define GPIO_CDF_GPP_A5                                     0x03000005 //GBE0_I2C_DATA
#define GPIO_CDF_GPP_A6                                     0x03000006 //GBE1_I2C_CLK
#define GPIO_CDF_GPP_A7                                     0x03000007 //GBE1_I2C_DATA
#define GPIO_CDF_GPP_A8                                     0x03000008 //GBE2_I2C_CLK
#define GPIO_CDF_GPP_A9                                     0x03000009 //GBE2_I2C_DATA
#define GPIO_CDF_GPP_A10                                    0x0300000a //GBE3_I2C_CLK
#define GPIO_CDF_GPP_A11                                    0x0300000b //GBE3_I2C_DATA
#define GPIO_CDF_GPP_A12                                    0x0300000c //GBE0_LED0
#define GPIO_CDF_GPP_A13                                    0x0300000d //GBE0_LED1
#define GPIO_CDF_GPP_A14                                    0x0300000e //GBE0_LED2
#define GPIO_CDF_GPP_A15                                    0x0300000f //GBE1_LED0
#define GPIO_CDF_GPP_A16                                    0x03000010 //GBE1_LED1
#define GPIO_CDF_GPP_A17                                    0x03000011 //GBE1_LED2
#define GPIO_CDF_GPP_A18                                    0x03000012 //GBE2_LED0
#define GPIO_CDF_GPP_A19                                    0x03000013 //GBE2_LED1
#define GPIO_CDF_GPP_A20                                    0x03000014 //GBE2_LED2
#define GPIO_CDF_GPP_A21                                    0x03000015 //GBE3_LED0
#define GPIO_CDF_GPP_A22                                    0x03000016 //GBE3_LED1
#define GPIO_CDF_GPP_A23                                    0x03000017 //GBE3_LED2
#define GPIO_CDF_GPP_B0                                     0x03010000 //NCSI_RXD0
#define GPIO_CDF_GPP_B1                                     0x03010001 //NCSI_CLK_IN
#define GPIO_CDF_GPP_B2                                     0x03010002 //NCSI_RXD1
#define GPIO_CDF_GPP_B3                                     0x03010003 //NCSI_CRS_DV
#define GPIO_CDF_GPP_B4                                     0x03010004 //NCSI_ARB_IN
#define GPIO_CDF_GPP_B5                                     0x03010005 //NCSI_TX_EN
#define GPIO_CDF_GPP_B6                                     0x03010006 //NCSI_TXD0
#define GPIO_CDF_GPP_B7                                     0x03010007 //NCSI_TXD1
#define GPIO_CDF_GPP_B8                                     0x03010008 //NCSI_ARB_OUT
#define GPIO_CDF_GPP_B9                                     0x03010009 //GBE_SMB_CLK
#define GPIO_CDF_GPP_B10                                    0x0301000a //GBE_SMB_DATA
#define GPIO_CDF_GPP_B11                                    0x0301000b //GBE_SMB_ALRT_N
#define GPIO_CDF_GPP_B12                                    0x0301000c //THERMTRIP_N
#define GPIO_CDF_GPP_B13                                    0x0301000d //PCHHOT_N
#define GPIO_CDF_GPP_B14                                    0x0301000e //ERROR0_N
#define GPIO_CDF_GPP_B15                                    0x0301000f //ERROR1_N
#define GPIO_CDF_GPP_B16                                    0x03010010 //ERROR2_N
#define GPIO_CDF_GPP_B17                                    0x03010011 //MSMI_N
#define GPIO_CDF_GPP_B18                                    0x03010012 //CATERR_N
#define GPIO_CDF_GPP_B19                                    0x03010013 //MEMTRIP_N
#define GPIO_CDF_GPP_B20                                    0x03010014 //UART0_RXD
#define GPIO_CDF_GPP_B21                                    0x03010015 //UART0_TXD
#define GPIO_CDF_GPP_B22                                    0x03010016 //UART1_RXD
#define GPIO_CDF_GPP_B23                                    0x03010017 //UART1_TXD
#define GPIO_CDF_GPP_C0                                     0x03020000 //CPU_GP_0
#define GPIO_CDF_GPP_C1                                     0x03020001 //CPU_GP_1
#define GPIO_CDF_GPP_C2                                     0x03020002 //CPU_GP_2
#define GPIO_CDF_GPP_C3                                     0x03020003 //CPU_GP_3
#define GPIO_CDF_GPP_C4                                     0x03020004 //FAN_PWM_0
#define GPIO_CDF_GPP_C5                                     0x03020005 //FAN_PWM_1
#define GPIO_CDF_GPP_C6                                     0x03020006 //FAN_PWM_2
#define GPIO_CDF_GPP_C7                                     0x03020007 //FAN_PWM_3
#define GPIO_CDF_GPP_C8                                     0x03020008 //FAN_TACH_0
#define GPIO_CDF_GPP_C9                                     0x03020009 //FAN_TACH_1
#define GPIO_CDF_GPP_C10                                    0x0302000a //FAN_TACH_2
#define GPIO_CDF_GPP_C11                                    0x0302000b //FAN_TACH_3
#define GPIO_CDF_GPP_C12                                    0x0302000c //ME_SMB0_CLK
#define GPIO_CDF_GPP_C13                                    0x0302000d //ME_SMB0_DATA
#define GPIO_CDF_GPP_C14                                    0x0302000e //ME_SMB0_ALRT_N
#define GPIO_CDF_GPP_C15                                    0x0302000f //ME_SMB1_CLK
#define GPIO_CDF_GPP_C16                                    0x03020010 //ME_SMB1_DATA
#define GPIO_CDF_GPP_C17                                    0x03020011 //ME_SMB1_ALRT_N
#define GPIO_CDF_GPP_C18                                    0x03020012 //ME_SMB2_CLK
#define GPIO_CDF_GPP_C19                                    0x03020013 //ME_SMB2_DATA
#define GPIO_CDF_GPP_C20                                    0x03020014 //ME_SMB2_ALRT_N
#define GPIO_CDF_GPP_C21                                    0x03020015 //GBE_MNG_I2C_CLK
#define GPIO_CDF_GPP_C22                                    0x03020016 //GBE_MNG_I2C_DATA
#define GPIO_CDF_GPP_D0                                     0x03030000 //IE_UART_RXD
#define GPIO_CDF_GPP_D1                                     0x03030001 //IE_UART_TXD
#define GPIO_CDF_GPP_D2                                     0x03030002 //VPP_SMB_CLK
#define GPIO_CDF_GPP_D3                                     0x03030003 //VPP_SMB_DATA
#define GPIO_CDF_GPP_D4                                     0x03030004 //VPP_SMB_ALRT_N
#define GPIO_CDF_GPP_D5                                     0x03030005 //PCIE_CLKREQ0_N
#define GPIO_CDF_GPP_D6                                     0x03030006 //PCIE_CLKREQ1_N
#define GPIO_CDF_GPP_D7                                     0x03030007 //PCIE_CLKREQ2_N
#define GPIO_CDF_GPP_D8                                     0x03030008 //PCIE_CLKREQ3_N
#define GPIO_CDF_GPP_D9                                     0x03030009 //PCIE_CLKREQ4_N
#define GPIO_CDF_GPP_D10                                    0x0303000a //PCIE_CLKREQ5_N
#define GPIO_CDF_GPP_D11                                    0x0303000b //PCIE_CLKREQ6_N
#define GPIO_CDF_GPP_D12                                    0x0303000c //PCIE_CLKREQ7_N
#define GPIO_CDF_GPP_D13                                    0x0303000d //PCIE_CLKREQ8_N
#define GPIO_CDF_GPP_D14                                    0x0303000e //PCIE_CLKREQ9_N
#define GPIO_CDF_GPP_D15                                    0x0303000f //FLEX_CLK_SE0
#define GPIO_CDF_GPP_D16                                    0x03030010 //FLEX_CLK_SE1
#define GPIO_CDF_GPP_D17                                    0x03030011 //FLEX_CLK1_50
#define GPIO_CDF_GPP_D18                                    0x03030012 //FLEX_CLK2_50
#define GPIO_CDF_GPP_D19                                    0x03030013 //FLEX_CLK_125
#define GPIO_CDF_GPP_E0                                     0x03040000 //TCK_PCH
#define GPIO_CDF_GPP_E1                                     0x03040001 //JTAGX_PCH
#define GPIO_CDF_GPP_E2                                     0x03040002 //TRST_N_PCH
#define GPIO_CDF_GPP_E3                                     0x03040003 //TMS_PCH
#define GPIO_CDF_GPP_E4                                     0x03040004 //TDI_PCH
#define GPIO_CDF_GPP_E5                                     0x03040005 //TDO_PCH
#define GPIO_CDF_GPP_F0                                     0x03050000 //CX_PRDY_N
#define GPIO_CDF_GPP_F1                                     0x03050001 //CX_PREQ_N
#define GPIO_CDF_GPP_F2                                     0x03050002 //CPU_FBREAK_OUT_N
#define GPIO_CDF_GPP_F3                                     0x03050003 //TRIGGER0_N
#define GPIO_CDF_GPP_F4                                     0x03050004 //TRIGGER1_N
#define GPIO_CDF_GPP_G0                                     0x03060000 //DBG_PTI_CLK0
#define GPIO_CDF_GPP_G1                                     0x03060001 //DBG_PTI_CLK3
#define GPIO_CDF_GPP_G2                                     0x03060002 //DBG_PTI_DATA0
#define GPIO_CDF_GPP_G3                                     0x03060003 //DBG_PTI_DATA1
#define GPIO_CDF_GPP_G4                                     0x03060004 //DBG_PTI_DATA2
#define GPIO_CDF_GPP_G5                                     0x03060005 //DBG_PTI_DATA3
#define GPIO_CDF_GPP_G6                                     0x03060006 //DBG_PTI_DATA4
#define GPIO_CDF_GPP_G7                                     0x03060007 //DBG_PTI_DATA5
#define GPIO_CDF_GPP_G8                                     0x03060008 //DBG_PTI_DATA6
#define GPIO_CDF_GPP_G9                                     0x03060009 //DBG_PTI_DATA7
#define GPIO_CDF_GPP_H0                                     0x03070000 //DBG_PTI_DATA8
#define GPIO_CDF_GPP_H1                                     0x03070001 //DBG_PTI_DATA9
#define GPIO_CDF_GPP_H2                                     0x03070002 //DBG_PTI_DATA10
#define GPIO_CDF_GPP_H3                                     0x03070003 //DBG_PTI_DATA11
#define GPIO_CDF_GPP_H4                                     0x03070004 //DBG_PTI_DATA12
#define GPIO_CDF_GPP_H5                                     0x03070005 //DBG_PTI_DATA13
#define GPIO_CDF_GPP_H6                                     0x03070006 //DBG_PTI_DATA14
#define GPIO_CDF_GPP_H7                                     0x03070007 //DBG_PTI_DATA15
#define GPIO_CDF_GPP_H8                                     0x03070008 //DBG_SPARE0
#define GPIO_CDF_GPP_H9                                     0x03070009 //DBG_SPARE1
#define GPIO_CDF_GPP_H10                                    0x0307000a //DBG_SPARE2
#define GPIO_CDF_GPP_H11                                    0x0307000b //DBG_SPARE3
#define GPIO_CDF_GPP_I0                                     0x03080000 //CPU_PWR_GOOD
#define GPIO_CDF_GPP_I1                                     0x03080001 //PLTRST_CPU_N
#define GPIO_CDF_GPP_I2                                     0x03080002 //NAC_RESET_NAC_N
#define GPIO_CDF_GPP_I3                                     0x03080003 //PCH_SBLINK_RX
#define GPIO_CDF_GPP_I4                                     0x03080004 //PCH_SBLINK_TX
#define GPIO_CDF_GPP_I5                                     0x03080005 //PMSYNC_CLK
#define GPIO_CDF_GPP_I6                                     0x03080006 //CPU_ERR0_N
#define GPIO_CDF_GPP_I7                                     0x03080007 //CPU_ERR1_N
#define GPIO_CDF_GPP_I8                                     0x03080008 //CPU_ERR2_N
#define GPIO_CDF_GPP_I9                                     0x03080009 //CPU_THERMTRIP_N
#define GPIO_CDF_GPP_I10                                    0x0308000a //CPU_MSMI_N
#define GPIO_CDF_GPP_I11                                    0x0308000b //CPU_CATERR_N
#define GPIO_CDF_GPP_I12                                    0x0308000c //CPU_MEMTRIP_N
#define GPIO_CDF_GPP_I13                                    0x0308000d //NAC_GR_N
#define GPIO_CDF_GPP_I14                                    0x0308000e //NAC_XTAL_VALID
#define GPIO_CDF_GPP_I15                                    0x0308000f //NAC_WAKE_N
#define GPIO_CDF_GPP_I16                                    0x03080010 //NAC_SBLINK_CLK_S2N
#define GPIO_CDF_GPP_I17                                    0x03080011 //NAC_SBLINK_N2S
#define GPIO_CDF_GPP_I18                                    0x03080012 //NAC_SBLINK_S2N
#define GPIO_CDF_GPP_I19                                    0x03080013 //NAC_SBLINK_CLK_N2S
#define GPIO_CDF_GPP_J0                                     0x03090000 //PECI_PCH
#define GPIO_CDF_GPP_K0                                     0x030a0000 //NAC_RMII_CLK
#define GPIO_CDF_GPP_K1                                     0x030a0001 //NAC_RGMII_CLK
#define GPIO_CDF_GPP_K2                                     0x030a0002 //NAC_SPARE0
#define GPIO_CDF_GPP_K3                                     0x030a0003 //NAC_SPARE1
#define GPIO_CDF_GPP_K4                                     0x030a0004 //NAC_SPARE2
#define GPIO_CDF_GPP_K5                                     0x030a0005 //NAC_INIT_SX_WAKE_N
#define GPIO_CDF_GPP_K6                                     0x030a0006 //NAC_GBE_GPIO0
#define GPIO_CDF_GPP_K7                                     0x030a0007 //NAC_GBE_GPIO1
#define GPIO_CDF_GPP_K8                                     0x030a0008 //NAC_GBE_GPIO2
#define GPIO_CDF_GPP_K9                                     0x030a0009 //NAC_GBE_GPIO3
#define GPIO_CDF_GPP_K10                                    0x030a000a //NAC_NCSI_RXD0
#define GPIO_CDF_GPP_K11                                    0x030a000b //NAC_NCSI_CLK_IN
#define GPIO_CDF_GPP_K12                                    0x030a000c //NAC_NCSI_RXD1
#define GPIO_CDF_GPP_K13                                    0x030a000d //NAC_NCSI_CRS_DV
#define GPIO_CDF_GPP_K14                                    0x030a000e //NAC_NCSI_ARB_IN
#define GPIO_CDF_GPP_K15                                    0x030a000f //NAC_NCSI_TX_EN
#define GPIO_CDF_GPP_K16                                    0x030a0010 //NAC_NCSI_TXD0
#define GPIO_CDF_GPP_K17                                    0x030a0011 //NAC_NCSI_TXD1
#define GPIO_CDF_GPP_K18                                    0x030a0012 //NAC_NCSI_ARB_OUT
#define GPIO_CDF_GPP_K19                                    0x030a0013 //NAC_NCSI_OE_N
#define GPIO_CDF_GPP_K20                                    0x030a0014 //NAC_GBE_SMB_CLK
#define GPIO_CDF_GPP_K21                                    0x030a0015 //NAC_GBE_SMB_DATA
#define GPIO_CDF_GPP_K22                                    0x030a0016 //NAC_GBE_SMB_ALRT_N
#define GPIO_CDF_GPP_L0                                     0x030b0000 //USB_OC0_N
#define GPIO_CDF_GPP_L1                                     0x030b0001 //GPIO_0
#define GPIO_CDF_GPP_L2                                     0x030b0002 //GPIO_1
#define GPIO_CDF_GPP_L3                                     0x030b0003 //GPIO_2
#define GPIO_CDF_GPP_L4                                     0x030b0004 //GPIO_3
#define GPIO_CDF_GPP_L5                                     0x030b0005 //GPIO_4
#define GPIO_CDF_GPP_L6                                     0x030b0006 //GPIO_5
#define GPIO_CDF_GPP_L7                                     0x030b0007 //GPIO_6
#define GPIO_CDF_GPP_L8                                     0x030b0008 //GPIO_7
#define GPIO_CDF_GPP_L9                                     0x030b0009 //GPIO_8
#define GPIO_CDF_GPP_L10                                    0x030b000a //GPIO_9
#define GPIO_CDF_GPP_L11                                    0x030b000b //GPIO_10
#define GPIO_CDF_GPP_L12                                    0x030b000c //GPIO_11
#define GPIO_CDF_GPP_L13                                    0x030b000d //GPIO_12
#define GPIO_CDF_GPP_L14                                    0x030b000e //PECI_SMB_DATA
#define GPIO_CDF_GPP_L15                                    0x030b000f //SATA0_LED_N
#define GPIO_CDF_GPP_L16                                    0x030b0010 //SATA1_LED_N
#define GPIO_CDF_GPP_L17                                    0x030b0011 //SATA_PDETECT0
#define GPIO_CDF_GPP_L18                                    0x030b0012 //SATA_PDETECT1
#define GPIO_CDF_GPP_L19                                    0x030b0013 //SATA0_SDOUT
#define GPIO_CDF_GPP_L20                                    0x030b0014 //SATA1_SDOUT
#define GPIO_CDF_GPP_L21                                    0x030b0015 //SATA2_LED_N
#define GPIO_CDF_GPP_L22                                    0x030b0016 //SATA_PDETECT2
#define GPIO_CDF_GPP_L23                                    0x030b0017 //SATA2_SDOUT
#define GPIO_CDF_GPP_M0                                     0x030c0000 //ESPI_IO0
#define GPIO_CDF_GPP_M1                                     0x030c0001 //ESPI_IO1
#define GPIO_CDF_GPP_M2                                     0x030c0002 //ESPI_IO2
#define GPIO_CDF_GPP_M3                                     0x030c0003 //ESPI_IO3
#define GPIO_CDF_GPP_M4                                     0x030c0004 //ESPI_CLK
#define GPIO_CDF_GPP_M5                                     0x030c0005 //ESPI_RST_N
#define GPIO_CDF_GPP_M6                                     0x030c0006 //ESPI_CS0_N
#define GPIO_CDF_GPP_M7                                     0x030c0007 //ESPI_ALRT0_N
#define GPIO_CDF_GPP_M8                                     0x030c0008 //ESPI_CS1_N
#define GPIO_CDF_GPP_M9                                     0x030c0009 //ESPI_ALRT1_N
#define GPIO_CDF_GPP_M10                                    0x030c000a //ESPI_CLK_LOOPBK
#define GPIO_CDF_GPP_N0                                     0x030d0000 //SPI_CS0_N
#define GPIO_CDF_GPP_N1                                     0x030d0001 //SPI_CS1_N
#define GPIO_CDF_GPP_N2                                     0x030d0002 //SPI_MOSI_IO0
#define GPIO_CDF_GPP_N3                                     0x030d0003 //SPI_MISO_IO1
#define GPIO_CDF_GPP_N4                                     0x030d0004 //SPI_IO2
#define GPIO_CDF_GPP_N5                                     0x030d0005 //SPI_IO3
#define GPIO_CDF_GPP_N6                                     0x030d0006 //SPI_CLK
#define GPIO_CDF_GPP_N7                                     0x030d0007 //SPI_CLK_LOOPBK
#define GPIO_CDF_GPP_N8                                     0x030d0008 //SUSPWRDNACK
#define GPIO_CDF_GPP_N9                                     0x030d0009 //PMU_SUSCLK
#define GPIO_CDF_GPP_N10                                    0x030d000a //ADR_COMPLETE
#define GPIO_CDF_GPP_N11                                    0x030d000b //ADR_TRIGGER_N
#define GPIO_CDF_GPP_N12                                    0x030d000c //PMU_SLP_S45_N
#define GPIO_CDF_GPP_N13                                    0x030d000d //PMU_SLP_S3_N
#define GPIO_CDF_GPP_N14                                    0x030d000e //PMU_WAKE_N
#define GPIO_CDF_GPP_N15                                    0x030d000f //PMU_PWRBTN_N
#define GPIO_CDF_GPP_N16                                    0x030d0010 //PMU_RESETBUTTON_N
#define GPIO_CDF_GPP_N17                                    0x030d0011 //PMU_PLTRST_N
#define GPIO_CDF_GPP_N18                                    0x030d0012 //SUS_STAT_N
#define GPIO_CDF_GPP_N19                                    0x030d0013 //PMU_I2C_CLK
#define GPIO_CDF_GPP_N20                                    0x030d0014 //PMU_I2C_DATA
#define GPIO_CDF_GPP_N21                                    0x030d0015 //PECI_SMB_CLK
#define GPIO_CDF_GPP_N22                                    0x030d0016 //PECI_SMB_ALRT_N
#define GPIO_CDF_GPP_O0                                     0x030e0000 //EMMC_CMD
#define GPIO_CDF_GPP_O1                                     0x030e0001 //EMMC_STROBE
#define GPIO_CDF_GPP_O2                                     0x030e0002 //EMMC_CLK
#define GPIO_CDF_GPP_O3                                     0x030e0003 //EMMC_D0
#define GPIO_CDF_GPP_O4                                     0x030e0004 //EMMC_D1
#define GPIO_CDF_GPP_O5                                     0x030e0005 //EMMC_D2
#define GPIO_CDF_GPP_O6                                     0x030e0006 //EMMC_D3
#define GPIO_CDF_GPP_O7                                     0x030e0007 //EMMC_D4
#define GPIO_CDF_GPP_O8                                     0x030e0008 //EMMC_D5
#define GPIO_CDF_GPP_O9                                     0x030e0009 //EMMC_D6
#define GPIO_CDF_GPP_O10                                    0x030e000a //EMMC_D7
#endif //_GPIO_PINS_CDF_
