/** @file
  GPIO pins for EBG-PCH,

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

#ifndef _GPIO_PINS_VER3_H_
#define _GPIO_PINS_VER3_H_


///
/// Unique ID used in GpioPad defines
///
#define GPIO_VER3_CHIPSET_ID      0x3

#define GPIO_VER3_GROUP_GPP_A  0x0300
#define GPIO_VER3_GROUP_GPP_B  0x0301
#define GPIO_VER3_GROUP_SPI    0x0302
#define GPIO_VER3_GROUP_GPP_C  0x0303
#define GPIO_VER3_GROUP_GPP_D  0x0304
#define GPIO_VER3_GROUP_GPP_E  0x0305
#define GPIO_VER3_GROUP_JTAG   0x0306
#define GPIO_VER3_GROUP_GPP_H  0x0307
#define GPIO_VER3_GROUP_GPP_J  0x0308
#define GPIO_VER3_GROUP_GPP_I  0x0309
#define GPIO_VER3_GROUP_GPP_L  0x030A
#define GPIO_VER3_GROUP_GPP_M  0x030B
#define GPIO_VER3_GROUP_GPP_N  0x030C
#define GPIO_VER3_GROUP_GPP_O  0x030D


#define GPIO_VER3_GPP_A0                                     0x03000000 //ESPI_ALERT0B
#define GPIO_VER3_GPP_A1                                     0x03000001 //ESPI_ALERT1B
#define GPIO_VER3_GPP_A2                                     0x03000002 //ESPI_IO0
#define GPIO_VER3_GPP_A3                                     0x03000003 //ESPI_IO1
#define GPIO_VER3_GPP_A4                                     0x03000004 //ESPI_IO2
#define GPIO_VER3_GPP_A5                                     0x03000005 //ESPI_IO3
#define GPIO_VER3_GPP_A6                                     0x03000006 //ESPI_CS0B
#define GPIO_VER3_GPP_A7                                     0x03000007 //ESPI_CS1B
#define GPIO_VER3_GPP_A8                                     0x03000008 //ESPI_RESETB
#define GPIO_VER3_GPP_A9                                     0x03000009 //ESPI_CLK
#define GPIO_VER3_GPP_A10                                    0x0300000A //SRCCLKREQB_0
#define GPIO_VER3_GPP_A11                                    0x0300000B //SRCCLKREQB_1
#define GPIO_VER3_GPP_A12                                    0x0300000C //SRCCLKREQB_2
#define GPIO_VER3_GPP_A13                                    0x0300000D //SRCCLKREQB_3
#define GPIO_VER3_GPP_A14                                    0x0300000E //SRCCLKREQB_4
#define GPIO_VER3_GPP_A15                                    0x0300000F //SRCCLKREQB_5
#define GPIO_VER3_GPP_A16                                    0x03000010 //SRCCLKREQB_6
#define GPIO_VER3_GPP_A17                                    0x03000011 //SRCCLKREQB_7
#define GPIO_VER3_GPP_A18                                    0x03000012 //SRCCLKREQB_8
#define GPIO_VER3_GPP_A19                                    0x03000013 //SRCCLKREQB_9
#define GPIO_VER3_ESPI_CLK_LOOPBK                            0x03000014 //ESPI_CLK_LOOPBK

#define GPIO_VER3_GPP_B0                                     0x03010000 //GSXDOUT
#define GPIO_VER3_GPP_B1                                     0x03010001 //GSXSLOAD
#define GPIO_VER3_GPP_B2                                     0x03010002 //GSXDIN
#define GPIO_VER3_GPP_B3                                     0x03010003 //GSXSRESETB
#define GPIO_VER3_GPP_B4                                     0x03010004 //GSXCLK
#define GPIO_VER3_GPP_B5                                     0x03010005 //USB2_OCB_0
#define GPIO_VER3_GPP_B6                                     0x03010006 //USB2_OCB_1
#define GPIO_VER3_GPP_B7                                     0x03010007 //USB2_OCB_2
#define GPIO_VER3_GPP_B8                                     0x03010008 //USB2_OCB_3
#define GPIO_VER3_GPP_B9                                     0x03010009 //USB2_OCB_4_GBE0_LED0
#define GPIO_VER3_GPP_B10                                    0x0301000A //USB2_OCB_5_GBE0_LED1
#define GPIO_VER3_GPP_B11                                    0x0301000B //USB2_OCB_6_GBE0_LED2
#define GPIO_VER3_GPP_B12                                    0x0301000C //HS_UART0_RXD
#define GPIO_VER3_GPP_B13                                    0x0301000D //HS_UART0_TXD
#define GPIO_VER3_GPP_B14                                    0x0301000E //HS_UART0_RTSB
#define GPIO_VER3_GPP_B15                                    0x0301000F //HS_UART0_CTSB
#define GPIO_VER3_GPP_B16                                    0x03010010 //HS_UART1_RXD
#define GPIO_VER3_GPP_B17                                    0x03010011 //HS_UART1_TXD
#define GPIO_VER3_GPP_B18                                    0x03010012 //HS_UART1_RTSB
#define GPIO_VER3_GPP_B19                                    0x03010013 //HS_UART1_CTSB
#define GPIO_VER3_GPP_B20                                    0x03010014 //EC_GPPB_20
#define GPIO_VER3_GPP_B21                                    0x03010015 //EC_GPPB_21
#define GPIO_VER3_GPP_B22                                    0x03010016 //EC_GPPB_22
#define GPIO_VER3_GPP_B23                                    0x03010017 //EC_GPPB_23

#define GPIO_VER3_SPI0_IO_2                                  0x03020000 //SPI0_IO_2
#define GPIO_VER3_SPI0_IO_3                                  0x03020001 //SPI0_IO_3
#define GPIO_VER3_SPI0_MOSI_IO_0                             0x03020002 //SPI0_MOSI_IO_0
#define GPIO_VER3_SPI0_MISO_IO_1                             0x03020003 //SPI0_MISO_IO_1
#define GPIO_VER3_SPI0_TPM_CSB                               0x03020004 //SPI0_TPM_CSB
#define GPIO_VER3_SPI0_FLASH_0_CSB                           0x03020005 //SPI0_FLASH_0_CSB
#define GPIO_VER3_SPI0_FLASH_1_CSB                           0x03020006 //SPI0_FLASH_1_CSB
#define GPIO_VER3_SPI0_CLK                                   0x03020007 //SPI0_CLK
#define GPIO_VER3_GPPC_S0                                    0x03020008 //TIME_SYNC_0
#define GPIO_VER3_GPPC_S1                                    0x03020009 //SPKR_TIME_SYNC_1
#define GPIO_VER3_GPPC_S2                                    0x03020008 //CPU_GP_0_GBE_GPIO0
#define GPIO_VER3_GPPC_S3                                    0x03020009 //CPU_GP_1_GBE_GPIO1
#define GPIO_VER3_GPPC_S4                                    0x0302000A //CPU_GP_2_GBE_GPIO2
#define GPIO_VER3_GPPC_S5                                    0x0302000B //CPU_GP_3_GBE_GPIO3
#define GPIO_VER3_GPPC_S6                                    0x0302000C //SUSWARNB_SUSPWRDNACK
#define GPIO_VER3_GPPC_S7                                    0x0302000D //SUSACKB
#define GPIO_VER3_GPPC_S8                                    0x0302000E //NMIB
#define GPIO_VER3_GPPC_S9                                    0x0302000F //SMIB
#define GPIO_VER3_GPPC_S10                                   0x03020010
#define GPIO_VER3_GPPC_S11                                   0x03020011
#define GPIO_VER3_SPI0_CLK_LOOPBK                            0x03020012 //SPI0_CLK_LOOPBK

#define GPIO_VER3_GPP_C0                                     0x03030000 //ME_SML0CLK
#define GPIO_VER3_GPP_C1                                     0x03030001 //ME_SML0DATA
#define GPIO_VER3_GPP_C2                                     0x03030002 //ME_SML0ALERTB
#define GPIO_VER3_GPP_C3                                     0x03030003 //ME_SML0BDATA
#define GPIO_VER3_GPP_C4                                     0x03030004 //ME_SML0BCLK
#define GPIO_VER3_GPP_C5                                     0x03030005 //ME_SMLB0ALERTB
#define GPIO_VER3_GPP_C6                                     0x03030006 //ME_SML1CLK
#define GPIO_VER3_GPP_C7                                     0x03030007 //ME_SML1DATA
#define GPIO_VER3_GPP_C8                                     0x03030008 //ME_SML1ALERTB
#define GPIO_VER3_GPP_C9                                     0x03030009 //ME_SML2CLK
#define GPIO_VER3_GPP_C10                                    0x0303000A //ME_SML2DATA
#define GPIO_VER3_GPP_C11                                    0x0303000B //ME_SML2ALERTB
#define GPIO_VER3_GPP_C12                                    0x0303000C //ME_SML3CLK
#define GPIO_VER3_GPP_C13                                    0x0303000D //ME_SML3DATA
#define GPIO_VER3_GPP_C14                                    0x0303000E //ME_SML3ALERTB
#define GPIO_VER3_GPP_C15                                    0x0303000F //ME_SML4CLK
#define GPIO_VER3_GPP_C16                                    0x03030010 //ME_SML4DATA
#define GPIO_VER3_GPP_C17                                    0x03030011 //ME_SML4ALERTB
#define GPIO_VER3_GPP_C18                                    0x03030012 //EC_GPPC_18
#define GPIO_VER3_GPP_C19                                    0x03030013 //MC_SMBCLK
#define GPIO_VER3_GPP_C20                                    0x03030014 //MC_SMBDATA
#define GPIO_VER3_GPP_C21                                    0x03030015 //MC_SMBALERTB

#define GPIO_VER3_GPP_D0                                     0x03040000 //HS_SMBCLK_DMA_SMBCLK
#define GPIO_VER3_GPP_D1                                     0x03040001 //HS_SMBDATA_DMA_SMBDATA
#define GPIO_VER3_GPP_D2                                     0x03040002 //HS_MBALERTB_DMA_SMBALERTB
#define GPIO_VER3_GPP_D3                                     0x03040003 //GBE_SMB_ALRT_N
#define GPIO_VER3_GPP_D4                                     0x03040004 //GBE_SMB_CLK
#define GPIO_VER3_GPP_D5                                     0x03040005 //GBE_SMB_DATA
#define GPIO_VER3_GPP_D6                                     0x03040006 //GBE_GPIO10
#define GPIO_VER3_GPP_D7                                     0x03040007 //GBE_GPIO11
#define GPIO_VER3_GPP_D8                                     0x03040008 //CRASHLOG_TRIG_N
#define GPIO_VER3_GPP_D9                                     0x03040009 //PMEB
#define GPIO_VER3_GPP_D10                                    0x0304000A //BM_BUSYB_SX_EXIT_HOLDOFFB
#define GPIO_VER3_GPP_D11                                    0x0304000B //PLTRSTB
#define GPIO_VER3_GPP_D12                                    0x0304000C //PCHHOTB
#define GPIO_VER3_GPP_D13                                    0x0304000D //ADR_COMPLETE
#define GPIO_VER3_GPP_D14                                    0x0304000E //PCIE_CLKREQ9_N
#define GPIO_VER3_GPP_D15                                    0x0304000F //VRALERTB
#define GPIO_VER3_GPP_D16                                    0x03040010 //ADR_ACK
#define GPIO_VER3_GPP_D17                                    0x03040011 //THERMTRIP_N
#define GPIO_VER3_GPP_D18                                    0x03040012 //MEMTRIP_N
#define GPIO_VER3_GPP_D19                                    0x03040013 //MSMI_N
#define GPIO_VER3_GPP_D20                                    0x03040014 //CATERR_N
#define GPIO_VER3_GPP_D21                                    0x03040015 //GLB_RST_WARN_B
#define GPIO_VER3_GPP_D22                                    0x03040016 //USB2_OCB_7_GBE1_LED0
#define GPIO_VER3_GPP_D23                                    0x03040017

#define GPIO_VER3_GPP_E0                                     0x03050000 //SATA2_XPCIE_0
#define GPIO_VER3_GPP_E1                                     0x03050001 //SATA2_XPCIE_1
#define GPIO_VER3_GPP_E2                                     0x03050002 //SATA2_XPCIE_2
#define GPIO_VER3_GPP_E3                                     0x03050003 //SATA2_XPCIE_3
#define GPIO_VER3_GPP_E4                                     0x03050004 //SATA1_XPCIE_2
#define GPIO_VER3_GPP_E5                                     0x03050005 //SATA1_XPCIE_3
#define GPIO_VER3_GPP_E6                                     0x03050006 //SATA1_XPCIE_0
#define GPIO_VER3_GPP_E7                                     0x03050007 //SATA1_USB3_XPCIE_1
#define GPIO_VER3_GPP_E8                                     0x03050008 //SATA1_SCLOCK_SATA1_LEDB
#define GPIO_VER3_GPP_E9                                     0x03050009 //SATA1_SLOAD_SATA1_DEVSLP
#define GPIO_VER3_GPP_E10                                    0x0305000A //SATA1_SDATAOUT_SATA1_GP
#define GPIO_VER3_GPP_E11                                    0x0305000B //SATA2_SCLOCK_SATA2_LEDB
#define GPIO_VER3_GPP_E12                                    0x0305000C //SATA2_SLOAD_SATA2_GP
#define GPIO_VER3_GPP_E13                                    0x0305000D //SATA2_SDATAOUT_SATA2_DEVSLP
#define GPIO_VER3_GPP_E14                                    0x0305000E //SATA3_SCLOCK_SATA3_LEDB
#define GPIO_VER3_GPP_E15                                    0x0305000F //SATA3_SLOAD_SATA3_GP
#define GPIO_VER3_GPP_E16                                    0x03050010 //SATA3_SDATAOUT_SATA3_DEVSLP
#define GPIO_VER3_GPP_E17                                    0x03050011 //ERR0_N
#define GPIO_VER3_GPP_E18                                    0x03050012 //ERR1_N
#define GPIO_VER3_GPP_E19                                    0x03050013 //ERR2_N
#define GPIO_VER3_GPP_E20                                    0x03050014 //GBE_UART_RXD
#define GPIO_VER3_GPP_E21                                    0x03050015 //GBE_UART_TXD
#define GPIO_VER3_GPP_E22                                    0x03050016 //GBE_UART_RTSB
#define GPIO_VER3_GPP_E23                                    0x03050017 //GBE_UART_CTSB

#define GPIO_VER3_JTAG_TDO                                   0x03060000 //JTAG_TDO
#define GPIO_VER3_JTAG_TDI                                   0x03060001 //JTAG_TDI
#define GPIO_VER3_JTAG_TCK                                   0x03060002 //JTAG_TCK
#define GPIO_VER3_JTAG_TMS                                   0x03060003 //JTAG_TMS
#define GPIO_VER3_JTAGX                                      0x03060004 //JTAGX
#define GPIO_VER3_PRDYB                                      0x03060005 //PRDYB
#define GPIO_VER3_PREQB                                      0x03060006 //PREQB
#define GPIO_VER3_GLB_PC_DISABLE                             0x03060007 //GLB_PC_DISABLE
#define GPIO_VER3_DBG_PMODE                                  0x03060008 //DBG_PMODE
#define GPIO_VER3_GLB_EXT_ACC_DISABLE                        0x03060009 //GLB_EXT_ACC_DISABLE

#define GPIO_VER3_GPP_H0                                     0x03070000 //GBE_GPIO12
#define GPIO_VER3_GPP_H1                                     0x03070001 //GBE_GPIO13
#define GPIO_VER3_GPP_H2                                     0x03070002 //GBE_SDP_TIMESYNC0_S2N_GBE_SDP_TIMESYNC0_N2S
#define GPIO_VER3_GPP_H3                                     0x03070003 //GBE_SDP_TIMESYNC1_S2N_GBE_SDP_TIMESYNC1_N2S
#define GPIO_VER3_GPP_H4                                     0x03070004 //GBE_SDP_TIMESYNC2_S2N_GBE_SDP_TIMESYNC2_N2S
#define GPIO_VER3_GPP_H5                                     0x03070005 //GBE_SDP_TIMESYNC3_S2N_GBE_SDP_TIMESYNC3_N2S
#define GPIO_VER3_GPP_H6                                     0x03070006
#define GPIO_VER3_GPP_H7                                     0x03070007
#define GPIO_VER3_GPP_H8                                     0x03070008 //NCSI_CLK_IN
#define GPIO_VER3_GPP_H9                                     0x03070009 //NCSI_CRS_DV
#define GPIO_VER3_GPP_H10                                    0x0307000A //NCSI_RXD0
#define GPIO_VER3_GPP_H11                                    0x0307000B //NCSI_RXD1
#define GPIO_VER3_GPP_H12                                    0x0307000C //NCSI_TX_EN
#define GPIO_VER3_GPP_H13                                    0x0307000D //NCSI_TXD0
#define GPIO_VER3_GPP_H14                                    0x0307000E //NCSI_TXD1
#define GPIO_VER3_GPP_H15                                    0x0307000F //FLEX_CLK_OUT_0
#define GPIO_VER3_GPP_H16                                    0x03070010 //FLEX_CLK_OUT_1
#define GPIO_VER3_GPP_H17                                    0x03070011 //FLEX_CLK_OUT_2
#define GPIO_VER3_GPP_H18                                    0x03070012 //FLEX_CLK_PMCALERTB
#define GPIO_VER3_GPP_H19                                    0x03070013

#define GPIO_VER3_GPP_J0                                     0x03080000 //CPUPWRGD
#define GPIO_VER3_GPP_J1                                     0x03080001 //CPU_THRMTRIP_N
#define GPIO_VER3_GPP_J2                                     0x03080002 //PLTRST_CPUB
#define GPIO_VER3_GPP_J3                                     0x03080003 //TRIGGER0_N
#define GPIO_VER3_GPP_J4                                     0x03080004 //TRIGGER1_N
#define GPIO_VER3_GPP_J5                                     0x03080005 //CPU_PWR_DEBUG_N
#define GPIO_VER3_GPP_J6                                     0x03080006 //CPU_MEMTRIP_N
#define GPIO_VER3_GPP_J7                                     0x03080007 //CPU_MSMI_N
#define GPIO_VER3_GPP_J8                                     0x03080008 //ME_PECI
#define GPIO_VER3_GPP_J9                                     0x03080009 //NAC_SPARE0
#define GPIO_VER3_GPP_J10                                    0x0308000A //NAC_SPARE1
#define GPIO_VER3_GPP_J11                                    0x0308000B //NAC_SPARE2
#define GPIO_VER3_GPP_J12                                    0x0308000C //CPU_ERR0_N
#define GPIO_VER3_GPP_J13                                    0x0308000D //CPU_CATERR_N
#define GPIO_VER3_GPP_J14                                    0x0308000E //CPU_ERR1_N
#define GPIO_VER3_GPP_J15                                    0x0308000F //CPU_ERR2_N
#define GPIO_VER3_GPP_J16                                    0x03080010
#define GPIO_VER3_GPP_J17                                    0x03080011

#define GPIO_VER3_GPP_I0                                     0x03090000 //GBE_GPIO4_GBE5_I2C_CLK
#define GPIO_VER3_GPP_I1                                     0x03090001 //GBE_GPIO5_GBE5_I2C_DATA
#define GPIO_VER3_GPP_I2                                     0x03090002 //GBE_GPIO6
#define GPIO_VER3_GPP_I3                                     0x03090003 //GBE_GPIO7
#define GPIO_VER3_GPP_I4                                     0x03090004 //GBE1_LED1
#define GPIO_VER3_GPP_I5                                     0x03090005 //GBE1_LED2
#define GPIO_VER3_GPP_I6                                     0x03090006 //GBE2_LED0
#define GPIO_VER3_GPP_I7                                     0x03090007 //GBE2_LED1
#define GPIO_VER3_GPP_I8                                     0x03090008 //GBE2_LED2
#define GPIO_VER3_GPP_I9                                     0x03090009 //GBE3_LED0
#define GPIO_VER3_GPP_I10                                    0x0309000A //GBE3_LED1
#define GPIO_VER3_GPP_I11                                    0x0309000B //GBE3_LED2
#define GPIO_VER3_GPP_I12                                    0x0309000C //GBE0_I2C_CLK_HDA_BCLK
#define GPIO_VER3_GPP_I13                                    0x0309000D //GBE0_I2C_DATA_HDA_RSTB
#define GPIO_VER3_GPP_I14                                    0x0309000E //GBE1_I2C_CLK_HDA_SYNC
#define GPIO_VER3_GPP_I15                                    0x0309000F //GBE1_I2C_DATA_HDA_SDO
#define GPIO_VER3_GPP_I16                                    0x03090010 //GBE2_I2C_CLK_HDA_SDI_0
#define GPIO_VER3_GPP_I17                                    0x03090011 //GBE2_I2C_DATA_HDA_SDI_1
#define GPIO_VER3_GPP_I18                                    0x03090012 //GBE3_I2C_CLK
#define GPIO_VER3_GPP_I19                                    0x03090013 //GBE3_I2C_DATA
#define GPIO_VER3_GPP_I20                                    0x03090014 //GBE4_I2C_CLK
#define GPIO_VER3_GPP_I21                                    0x03090015 //GBE4_I2C_DATA
#define GPIO_VER3_GPP_I22                                    0x03090016 //GBE_GPIO8
#define GPIO_VER3_GPP_I23                                    0x03090017 //GBE_GPIO9

#define GPIO_VER3_GPP_L0                                     0x030A0000 //PM_SYNC_0
#define GPIO_VER3_GPP_L1                                     0x030A0001 //PM_DOWN_0
#define GPIO_VER3_GPP_L2                                     0x030A0002 //PM_SYNC_CLK_0
#define GPIO_VER3_GPP_L3                                     0x030A0003 //PM_SYNC_1
#define GPIO_VER3_GPP_L4                                     0x030A0004 //PM_DOWN_1
#define GPIO_VER3_GPP_L5                                     0x030A0005 //PM_SYNC_CLK_1
#define GPIO_VER3_GPP_L6                                     0x030A0006 //PM_SYNC_2
#define GPIO_VER3_GPP_L7                                     0x030A0007 //PM_DOWN_2
#define GPIO_VER3_GPP_L8                                     0x030A0008 //PM_SYNC_CLK_2
#define GPIO_VER3_GPP_L9                                     0x030A0009 //NAC_GBE_GPIO0_S2N_NAC_GBE_GPIO0_N2S
#define GPIO_VER3_GPP_L10                                    0x030A000A //NAC_GBE_GPIO1_S2N_NAC_GBE_GPIO1_N2S
#define GPIO_VER3_GPP_L11                                    0x030A000B //NAC_GBE_GPIO2_S2N_NAC_GBE_GPIO2_N2S
#define GPIO_VER3_GPP_L12                                    0x030A000C //NAC_GBE_GPIO3_S2N_NAC_GBE_GPIO3_N2S
#define GPIO_VER3_GPP_L13                                    0x030A000D //NAC_GBE_SMB_DATA_IN
#define GPIO_VER3_GPP_L14                                    0x030A000E //NAC_GBE_SMB_DATA_OUT
#define GPIO_VER3_GPP_L15                                    0x030A000F //NAC_GBE_SMB_ALRT_IN
#define GPIO_VER3_GPP_L16                                    0x030A0010 //NAC_GBE_SMB_CLK_IN
#define GPIO_VER3_GPP_L17                                    0x030A0011 //NAC_GBE_SMB_CLK_OUT

#define GPIO_VER3_GPP_M0                                     0x030B0000 //PM_SYNC_3
#define GPIO_VER3_GPP_M1                                     0x030B0001 //PM_DOWN_3
#define GPIO_VER3_GPP_M2                                     0x030B0002 //PM_SYNC_CLK_3
#define GPIO_VER3_GPP_M3                                     0x030B0003 //PM_SYNC_4
#define GPIO_VER3_GPP_M4                                     0x030B0004 //PM_DOWN_4_NAC_WAKE_N
#define GPIO_VER3_GPP_M5                                     0x030B0005 //PM_SYNC_CLK_4
#define GPIO_VER3_GPP_M6                                     0x030B0006 //PM_SYNC_5
#define GPIO_VER3_GPP_M7                                     0x030B0007 //PM_DOWN_5
#define GPIO_VER3_GPP_M8                                     0x030B0008 //PM_SYNC_CLK_5
#define GPIO_VER3_GPP_M9                                     0x030B0009 //NAC_SBLINK_S2N
#define GPIO_VER3_GPP_M10                                    0x030B000A //NAC_SBLINK_N2S
#define GPIO_VER3_GPP_M11                                    0x030B000B //NAC_SBLINK_CLK_N2S
#define GPIO_VER3_GPP_M12                                    0x030B000C //NAC_SBLINK_CLK_S2N
#define GPIO_VER3_GPP_M13                                    0x030B000D //NAC_XTAL_VALID
#define GPIO_VER3_GPP_M14                                    0x030B000E //NAC_RESET_NAC_N
#define GPIO_VER3_GPP_M15                                    0x030B000F
#define GPIO_VER3_GPP_M16                                    0x030B0010
#define GPIO_VER3_GPP_M17                                    0x030B0011

#define GPIO_VER3_GPP_N0                                     0x030C0000 //PM_SYNC_6
#define GPIO_VER3_GPP_N1                                     0x030C0001 //PM_DOWN_6_NAC_NCSI_TXD0
#define GPIO_VER3_GPP_N2                                     0x030C0002 //PM_SYNC_CLK_6
#define GPIO_VER3_GPP_N3                                     0x030C0003 //PM_SYNC_7
#define GPIO_VER3_GPP_N4                                     0x030C0004 //PM_DOWN_7_NAC_NCSI_REFCLK_IN
#define GPIO_VER3_GPP_N5                                     0x030C0005 //PM_SYNC_CLK_7
#define GPIO_VER3_GPP_N6                                     0x030C0006
#define GPIO_VER3_GPP_N7                                     0x030C0007
#define GPIO_VER3_GPP_N8                                     0x030C0008 //NAC_NCSI_RXD0
#define GPIO_VER3_GPP_N9                                     0x030C0009 //NAC_NCSI_RXD1
#define GPIO_VER3_GPP_N10                                    0x030C000A //NAC_NCSI_CRS_DV
#define GPIO_VER3_GPP_N11                                    0x030C000B //NAC_NCSI_CLK_IN
#define GPIO_VER3_GPP_N12                                    0x030C000C //NAC_NCSI_REFCLK_OUT
#define GPIO_VER3_GPP_N13                                    0x030C000D //NAC_NCSI_TX_EN
#define GPIO_VER3_GPP_N14                                    0x030C000E //NAC_NCSI_TXD1
#define GPIO_VER3_GPP_N15                                    0x030C000F //NAC_NCSI_OE_N
#define GPIO_VER3_GPP_N16                                    0x030C0010 //NAC_GR_N
#define GPIO_VER3_GPP_N17                                    0x030C0011 //NAC_INIT_SX_WAKE_N

#define GPIO_VER3_GPP_O0                                     0x030D0000
#define GPIO_VER3_GPP_O1                                     0x030D0001 //ACPRESENT
#define GPIO_VER3_GPP_O2                                     0x030D0002 //LAN_WAKEB
#define GPIO_VER3_GPP_O3                                     0x030D0003 //PWRBTNB
#define GPIO_VER3_GPP_O4                                     0x030D0004 //SLP_S3B
#define GPIO_VER3_GPP_O5                                     0x030D0005 //SLP_S4B
#define GPIO_VER3_GPP_O6                                     0x030D0006 //SLP_AB
#define GPIO_VER3_GPP_O7                                     0x030D0007 //GPP_O_7
#define GPIO_VER3_GPP_O8                                     0x030D0008 //SUSCLK
#define GPIO_VER3_GPP_O9                                     0x030D0009 //GPP_O_9
#define GPIO_VER3_GPP_O10                                    0x030D000A //SLP_S5B
#define GPIO_VER3_GPP_O11                                    0x030D000B //LANPHYPC
#define GPIO_VER3_GPP_O12                                    0x030D000C //SYS_PWROK
#define GPIO_VER3_GPP_O13                                    0x030D000D //SYS_RESETB
#define GPIO_VER3_GPP_O14                                    0x030D000E //SLP_LANB
#define GPIO_VER3_GPP_O15                                    0x030D000F //WAKEB
#define GPIO_VER3_GPP_O16                                    0x030D0010 //SLP_SUSB

#endif //_GPIO_PINS_VER3_H_
