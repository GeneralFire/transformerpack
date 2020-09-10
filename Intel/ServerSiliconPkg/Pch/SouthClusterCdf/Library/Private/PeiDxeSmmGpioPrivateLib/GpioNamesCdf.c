/** @file
  This file contains GPIO name library implementation specific to CDF

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

#include <Base.h>
#include <Library/BaseLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/PchInfoLib.h>
#include <Private/Library/GpioPrivateLib.h>

#include <GpioPinsCdf.h>

STATIC CONST CHAR8*  mGpioGppaNames[] = {
  "GBE_SDP_TIMESYNC0",
  "GBE_SDP_TIMESYNC1",
  "GBE_SDP_TIMESYNC2",
  "GBE_SDP_TIMESYNC3",
  "GBE0_I2C_CLK",
  "GBE0_I2C_DATA",
  "GBE1_I2C_CLK",
  "GBE1_I2C_DATA",
  "GBE2_I2C_CLK",
  "GBE2_I2C_DATA",
  "GBE3_I2C_CLK",
  "GBE3_I2C_DATA",
  "GBE0_LED0",
  "GBE0_LED1",
  "GBE0_LED2",
  "GBE1_LED0",
  "GBE1_LED1",
  "GBE1_LED2",
  "GBE2_LED0",
  "GBE2_LED1",
  "GBE2_LED2",
  "GBE3_LED0",
  "GBE3_LED1",
  "GBE3_LED2"
};

STATIC CONST CHAR8*  mGpioGppbNames[] = {
  "NCSI_RXD0",
  "NCSI_CLK_IN",
  "NCSI_RXD1",
  "NCSI_CRS_DV",
  "NCSI_ARB_IN",
  "NCSI_TX_EN",
  "NCSI_TXD0",
  "NCSI_TXD1",
  "NCSI_ARB_OUT",
  "GBE_SMB_CLK",
  "GBE_SMB_DATA",
  "GBE_SMB_ALRT_N",
  "THERMTRIP_N",
  "PCHHOT_N",
  "ERROR0_N",
  "ERROR1_N",
  "ERROR2_N",
  "MSMI_N",
  "CATERR_N",
  "MEMTRIP_N",
  "UART0_RXD",
  "UART0_TXD",
  "UART1_RXD",
  "UART1_TXD"
};

STATIC CONST CHAR8*  mGpioGppcNames[] = {
  "CPU_GP_0",
  "CPU_GP_1",
  "CPU_GP_2",
  "CPU_GP_3",
  "FAN_PWM_0",
  "FAN_PWM_1",
  "FAN_PWM_2",
  "FAN_PWM_3",
  "FAN_TACH_0",
  "FAN_TACH_1",
  "FAN_TACH_2",
  "FAN_TACH_3",
  "ME_SMB0_CLK",
  "ME_SMB0_DATA",
  "ME_SMB0_ALRT_N",
  "ME_SMB1_CLK",
  "ME_SMB1_DATA",
  "ME_SMB1_ALRT_N",
  "ME_SMB2_CLK",
  "ME_SMB2_DATA",
  "ME_SMB2_ALRT_N",
  "GBE_MNG_I2C_CLK",
  "GBE_MNG_I2C_DATA"
};

STATIC CONST CHAR8*  mGpioGppdNames[] = {
  "IE_UART_RXD",
  "IE_UART_TXD",
  "VPP_SMB_CLK",
  "VPP_SMB_DATA",
  "VPP_SMB_ALRT_N",
  "PCIE_CLKREQ0_N",
  "PCIE_CLKREQ1_N",
  "PCIE_CLKREQ2_N",
  "PCIE_CLKREQ3_N",
  "PCIE_CLKREQ4_N",
  "PCIE_CLKREQ5_N",
  "PCIE_CLKREQ6_N",
  "PCIE_CLKREQ7_N",
  "PCIE_CLKREQ8_N",
  "PCIE_CLKREQ9_N",
  "FLEX_CLK_SE0",
  "FLEX_CLK_SE1",
  "FLEX_CLK1_50",
  "FLEX_CLK2_50",
  "FLEX_CLK_125"
};

STATIC CONST CHAR8*  mGpioGppeNames[] = {
  "TCK_PCH",
  "JTAGX_PCH",
  "TRST_N_PCH",
  "TMS_PCH",
  "TDI_PCH",
  "TDO_PCH"
};

STATIC CONST CHAR8*  mGpioGppfNames[] = {
  "CX_PRDY_N",
  "CX_PREQ_N",
  "CPU_FBREAK_OUT_N",
  "TRIGGER0_N",
  "TRIGGER1_N"
};

STATIC CONST CHAR8*  mGpioGppgNames[] = {
  "DBG_PTI_CLK0",
  "DBG_PTI_CLK3",
  "DBG_PTI_DATA0",
  "DBG_PTI_DATA1",
  "DBG_PTI_DATA2",
  "DBG_PTI_DATA3",
  "DBG_PTI_DATA4",
  "DBG_PTI_DATA5",
  "DBG_PTI_DATA6",
  "DBG_PTI_DATA7"
};

STATIC CONST CHAR8*  mGpioGpphNames[] = {
  "DBG_PTI_DATA8",
  "DBG_PTI_DATA9",
  "DBG_PTI_DATA10",
  "DBG_PTI_DATA11",
  "DBG_PTI_DATA12",
  "DBG_PTI_DATA13",
  "DBG_PTI_DATA14",
  "DBG_PTI_DATA15",
  "DBG_SPARE0",
  "DBG_SPARE1",
  "DBG_SPARE2",
  "DBG_SPARE3"
};

STATIC CONST CHAR8*  mGpioGppiNames[] = {
  "CPU_PWR_GOOD",
  "PLTRST_CPU_N",
  "NAC_RESET_NAC_N",
  "PCH_SBLINK_RX",
  "PCH_SBLINK_TX",
  "PMSYNC_CLK",
  "CPU_ERR0_N",
  "CPU_ERR1_N",
  "CPU_ERR2_N",
  "CPU_THERMTRIP_N",
  "CPU_MSMI_N",
  "CPU_CATERR_N",
  "CPU_MEMTRIP_N",
  "NAC_GR_N",
  "NAC_XTAL_VALID",
  "NAC_WAKE_N",
  "NAC_SBLINK_CLK_S2N",
  "NAC_SBLINK_N2S",
  "NAC_SBLINK_S2N",
  "NAC_SBLINK_CLK_N2S"
};

STATIC CONST CHAR8*  mGpioGppjNames[] = {
  "PECI_PCH"
};

STATIC CONST CHAR8*  mGpioGppkNames[] = {
  "NAC_RMII_CLK",
  "NAC_RGMII_CLK",
  "NAC_SPARE0",
  "NAC_SPARE1",
  "NAC_SPARE2",
  "NAC_INIT_SX_WAKE_N",
  "NAC_GBE_GPIO0",
  "NAC_GBE_GPIO1",
  "NAC_GBE_GPIO2",
  "NAC_GBE_GPIO3",
  "NAC_NCSI_RXD0",
  "NAC_NCSI_CLK_IN",
  "NAC_NCSI_RXD1",
  "NAC_NCSI_CRS_DV",
  "NAC_NCSI_ARB_IN",
  "NAC_NCSI_TX_EN",
  "NAC_NCSI_TXD0",
  "NAC_NCSI_TXD1",
  "NAC_NCSI_ARB_OUT",
  "NAC_NCSI_OE_N",
  "NAC_GBE_SMB_CLK",
  "NAC_GBE_SMB_DATA",
  "NAC_GBE_SMB_ALRT_N"
};

STATIC CONST CHAR8*  mGpioGpplNames[] = {
  "USB_OC0_N",
  "GPIO_0",
  "GPIO_1",
  "GPIO_2",
  "GPIO_3",
  "GPIO_4",
  "GPIO_5",
  "GPIO_6",
  "GPIO_7",
  "GPIO_8",
  "GPIO_9",
  "GPIO_10",
  "GPIO_11",
  "GPIO_12",
  "PECI_SMB_DATA",
  "SATA0_LED_N",
  "SATA1_LED_N",
  "SATA_PDETECT0",
  "SATA_PDETECT1",
  "SATA0_SDOUT",
  "SATA1_SDOUT",
  "SATA2_LED_N",
  "SATA_PDETECT2",
  "SATA2_SDOUT"
};

STATIC CONST CHAR8*  mGpioGppmNames[] = {
  "ESPI_IO0",
  "ESPI_IO1",
  "ESPI_IO2",
  "ESPI_IO3",
  "ESPI_CLK",
  "ESPI_RST_N",
  "ESPI_CS0_N",
  "ESPI_ALRT0_N",
  "ESPI_CS1_N",
  "ESPI_ALRT1_N",
  "ESPI_CLK_LOOPBK"
};

STATIC CONST CHAR8*  mGpioGppnNames[] = {
  "SPI_CS0_N",
  "SPI_CS1_N",
  "SPI_MOSI_IO0",
  "SPI_MISO_IO1",
  "SPI_IO2",
  "SPI_IO3",
  "SPI_CLK",
  "SPI_CLK_LOOPBK",
  "SUSPWRDNACK",
  "PMU_SUSCLK",
  "ADR_COMPLETE",
  "ADR_TRIGGER_N",
  "PMU_SLP_S45_N",
  "PMU_SLP_S3_N",
  "PMU_WAKE_N",
  "PMU_PWRBTN_N",
  "PMU_RESETBUTTON_N",
  "PMU_PLTRST_N",
  "SUS_STAT_N",
  "PMU_I2C_CLK",
  "PMU_I2C_DATA",
  "PECI_SMB_CLK",
  "PECI_SMB_ALRT_N"
};

STATIC CONST CHAR8*  mGpioGppoNames[] = {
  "EMMC_CMD",
  "EMMC_STROBE",
  "EMMC_CLK",
  "EMMC_D0",
  "EMMC_D1",
  "EMMC_D2",
  "EMMC_D3",
  "EMMC_D4",
  "EMMC_D5",
  "EMMC_D6",
  "EMMC_D7"
};
STATIC CONST GPIO_GROUP_NAME_INFO  mPchHGroupDescriptors[] = {
  GPIO_GROUP_NAME("GPP_A", GPIO_CDF_GPP_A0, mGpioGppaNames),
  GPIO_GROUP_NAME("GPP_B", GPIO_CDF_GPP_B0, mGpioGppbNames),
  GPIO_GROUP_NAME("GPP_C", GPIO_CDF_GPP_C0, mGpioGppcNames),
  GPIO_GROUP_NAME("GPP_D", GPIO_CDF_GPP_D0, mGpioGppdNames),
  GPIO_GROUP_NAME("GPP_E", GPIO_CDF_GPP_E0, mGpioGppeNames),
  GPIO_GROUP_NAME("GPP_F", GPIO_CDF_GPP_F0, mGpioGppfNames),
  GPIO_GROUP_NAME("GPP_G", GPIO_CDF_GPP_G0, mGpioGppgNames),
  GPIO_GROUP_NAME("GPP_H", GPIO_CDF_GPP_H0, mGpioGpphNames),
  GPIO_GROUP_NAME("GPP_I", GPIO_CDF_GPP_I0, mGpioGppiNames),
  GPIO_GROUP_NAME("GPP_J", GPIO_CDF_GPP_J0, mGpioGppjNames),
  GPIO_GROUP_NAME("GPP_K", GPIO_CDF_GPP_K0, mGpioGppkNames),
  GPIO_GROUP_NAME("GPP_L", GPIO_CDF_GPP_L0, mGpioGpplNames),
  GPIO_GROUP_NAME("GPP_M", GPIO_CDF_GPP_M0, mGpioGppmNames),
  GPIO_GROUP_NAME("GPP_N", GPIO_CDF_GPP_N0, mGpioGppnNames),
  GPIO_GROUP_NAME("GPP_O", GPIO_CDF_GPP_O0, mGpioGppoNames),
};
/**
  Returns GPIO_GROUP_NAME_INFO corresponding to the given GpioPad

  @param[in] GroupIndex  Group index

  @retval GPIO_GROUP_NAME_INFO*  Pointer to the GPIO_GROUP_NAME_INFO
  @reval  NULL                   If no group descriptor was found
**/
CONST
GPIO_GROUP_NAME_INFO*
GpioGetGroupNameInfo (
  IN UINT32  GroupIndex
  )
{
  if (IsPchH ()) {
    if (GroupIndex < ARRAY_SIZE (mPchHGroupDescriptors)) {
      return &mPchHGroupDescriptors[GroupIndex];
    } else {
      ASSERT (FALSE);
      return NULL;
    }
  } else {
    ASSERT (FALSE);
    return NULL;
  }
}

