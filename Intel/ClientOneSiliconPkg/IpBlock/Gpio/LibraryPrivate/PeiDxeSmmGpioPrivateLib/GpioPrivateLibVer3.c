/** @file
  This file contains VER3 specific GPIO information

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

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
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/PchInfoLib.h>
#include <Library/GpioLib.h>
#include <Library/GpioNativeLib.h>
#include <Library/GpioNativePads.h>
#include <Library/GpioPrivateLib.h>
#include <Library/TcssInitLib.h>
#include <Register/PmcRegsVer3.h>
#include <Pins/GpioPinsVer3.h>
#include <Register/PchPcrRegs.h>
#include <Register/GpioRegsVer3.h>
#include <GpioNativePrivateLibInternal.h>

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_GROUP_INFO mVer3PchGpioGroupInfo[] = {
  {PID_GPIOCOM0, R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_PAD_OWN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_HOSTSW_OWN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_GPI_IS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_GPI_IE, R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_GPI_GPE_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_GPI_GPE_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_SMI_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_SMI_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_NMI_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_NMI_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_PADCFGLOCK, R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_PADCFGLOCKTX, R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_PADCFG_OFFSET, GPIO_VER3_GPIO_GPP_A_PAD_MAX},
  {PID_GPIOCOM0, R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_PAD_OWN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_HOSTSW_OWN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_GPI_IS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_GPI_IE, R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_GPI_GPE_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_GPI_GPE_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_SMI_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_SMI_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_NMI_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_NMI_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_PADCFGLOCK, R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_PADCFGLOCKTX, R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_PADCFG_OFFSET, GPIO_VER3_GPIO_GPP_B_PAD_MAX},
  {PID_GPIOCOM0, R_GPIO_VER3_PCH_GPIO_PCR_SPI_PAD_OWN,   R_GPIO_VER3_PCH_GPIO_PCR_SPI_HOSTSW_OWN,   R_GPIO_VER3_PCH_GPIO_PCR_SPI_GPI_IS,   R_GPIO_VER3_PCH_GPIO_PCR_SPI_GPI_IE,   R_GPIO_VER3_PCH_GPIO_PCR_SPI_GPI_GPE_STS,   R_GPIO_VER3_PCH_GPIO_PCR_SPI_GPI_GPE_EN,   R_GPIO_VER3_PCH_GPIO_PCR_SPI_SMI_STS,   R_GPIO_VER3_PCH_GPIO_PCR_SPI_SMI_EN,   R_GPIO_VER3_PCH_GPIO_PCR_SPI_NMI_STS,   R_GPIO_VER3_PCH_GPIO_PCR_SPI_NMI_EN,   R_GPIO_VER3_PCH_GPIO_PCR_SPI_PADCFGLOCK,   R_GPIO_VER3_PCH_GPIO_PCR_SPI_PADCFGLOCKTX,   R_GPIO_VER3_PCH_GPIO_PCR_SPI_PADCFG_OFFSET,   GPIO_VER3_GPIO_SPI_PAD_MAX},
  {PID_GPIOCOM1, R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_PAD_OWN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_HOSTSW_OWN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_GPI_IS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_GPI_IE, R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_GPI_GPE_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_GPI_GPE_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_SMI_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_SMI_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_NMI_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_NMI_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_PADCFGLOCK, R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_PADCFGLOCKTX, R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_PADCFG_OFFSET, GPIO_VER3_GPIO_GPP_C_PAD_MAX},
  {PID_GPIOCOM1, R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_PAD_OWN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_HOSTSW_OWN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_GPI_IS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_GPI_IE, R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_GPI_GPE_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_GPI_GPE_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_SMI_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_SMI_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_NMI_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_NMI_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_PADCFGLOCK, R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_PADCFGLOCKTX, R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_PADCFG_OFFSET, GPIO_VER3_GPIO_GPP_D_PAD_MAX},
  {PID_GPIOCOM3, R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_PAD_OWN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_HOSTSW_OWN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_GPI_IS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_GPI_IE, R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_GPI_GPE_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_GPI_GPE_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_SMI_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_SMI_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_NMI_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_NMI_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_PADCFGLOCK, R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_PADCFGLOCKTX, R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_PADCFG_OFFSET, GPIO_VER3_GPIO_GPP_E_PAD_MAX},
  {PID_GPIOCOM3, R_GPIO_VER3_PCH_GPIO_PCR_JTAG_PAD_OWN,  R_GPIO_VER3_PCH_GPIO_PCR_JTAG_HOSTSW_OWN,  R_GPIO_VER3_PCH_GPIO_PCR_JTAG_GPI_IS,  R_GPIO_VER3_PCH_GPIO_PCR_JTAG_GPI_IE,  R_GPIO_VER3_PCH_GPIO_PCR_JTAG_GPI_GPE_STS,  R_GPIO_VER3_PCH_GPIO_PCR_JTAG_GPI_GPE_EN,  R_GPIO_VER3_PCH_GPIO_PCR_JTAG_SMI_STS,  R_GPIO_VER3_PCH_GPIO_PCR_JTAG_SMI_EN,  R_GPIO_VER3_PCH_GPIO_PCR_JTAG_NMI_STS,  R_GPIO_VER3_PCH_GPIO_PCR_JTAG_NMI_EN,  R_GPIO_VER3_PCH_GPIO_PCR_JTAG_PADCFGLOCK,  R_GPIO_VER3_PCH_GPIO_PCR_JTAG_PADCFGLOCKTX,  R_GPIO_VER3_PCH_GPIO_PCR_JTAG_PADCFG_OFFSET,  GPIO_VER3_GPIO_JTAG_PAD_MAX},
  {PID_GPIOCOM4, R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_PAD_OWN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_HOSTSW_OWN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_GPI_IS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_GPI_IE, R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_GPI_GPE_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_GPI_GPE_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_SMI_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_SMI_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_NMI_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_NMI_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_PADCFGLOCK, R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_PADCFGLOCKTX, R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_PADCFG_OFFSET, GPIO_VER3_GPIO_GPP_H_PAD_MAX},
  {PID_GPIOCOM4, R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_PAD_OWN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_HOSTSW_OWN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_GPI_IS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_GPI_IE, R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_GPI_GPE_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_GPI_GPE_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_SMI_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_SMI_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_NMI_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_NMI_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_PADCFGLOCK, R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_PADCFGLOCKTX, R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_PADCFG_OFFSET, GPIO_VER3_GPIO_GPP_J_PAD_MAX},
  {PID_GPIOCOM5, R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_PAD_OWN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_HOSTSW_OWN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_GPI_IS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_GPI_IE, R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_GPI_GPE_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_GPI_GPE_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_SMI_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_SMI_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_NMI_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_NMI_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_PADCFGLOCK, R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_PADCFGLOCKTX, R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_PADCFG_OFFSET, GPIO_VER3_GPIO_GPP_I_PAD_MAX},
  {PID_GPIOCOM5, R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_PAD_OWN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_HOSTSW_OWN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_GPI_IS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_GPI_IE, R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_GPI_GPE_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_GPI_GPE_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_SMI_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_SMI_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_NMI_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_NMI_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_PADCFGLOCK, R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_PADCFGLOCKTX, R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_PADCFG_OFFSET, GPIO_VER3_GPIO_GPP_L_PAD_MAX},
  {PID_GPIOCOM5, R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_PAD_OWN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_HOSTSW_OWN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_GPI_IS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_GPI_IE, R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_GPI_GPE_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_GPI_GPE_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_SMI_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_SMI_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_NMI_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_NMI_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_PADCFGLOCK, R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_PADCFGLOCKTX, R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_PADCFG_OFFSET, GPIO_VER3_GPIO_GPP_M_PAD_MAX},
  {PID_GPIOCOM5, R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_PAD_OWN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_HOSTSW_OWN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_GPI_IS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_GPI_IE, R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_GPI_GPE_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_GPI_GPE_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_SMI_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_SMI_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_NMI_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_NMI_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_PADCFGLOCK, R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_PADCFGLOCKTX, R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_PADCFG_OFFSET, GPIO_VER3_GPIO_GPP_N_PAD_MAX},
  {PID_GPIOCOM2, R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_PAD_OWN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_HOSTSW_OWN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_GPI_IS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_GPI_IE, R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_GPI_GPE_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_GPI_GPE_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_SMI_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_SMI_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_NMI_STS, R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_NMI_EN, R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_PADCFGLOCK, R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_PADCFGLOCKTX, R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_PADCFG_OFFSET, GPIO_VER3_GPIO_GPP_O_PAD_MAX},
};


/**
  This procedure will retrieve address and length of GPIO info table

  @param[out]  GpioGroupInfoTableLength   Length of GPIO group table

  @retval Pointer to GPIO group table

**/
CONST GPIO_GROUP_INFO*
GpioGetGroupInfoTable (
  OUT UINT32              *GpioGroupInfoTableLength
  )
{
  *GpioGroupInfoTableLength = ARRAY_SIZE (mVer3PchGpioGroupInfo);
  return mVer3PchGpioGroupInfo;
}

/**
  Get GPIO Chipset ID specific to PCH generation and series
**/
UINT32
GpioGetThisChipsetId (
  VOID
  )
{
  return GPIO_VER3_CHIPSET_ID;
}

/**
  This internal procedure will check if group is within DeepSleepWell.

  @param[in]  Group               GPIO Group

  @retval GroupWell               TRUE:  This is DSW Group
                                  FALSE: This is not DSW Group
**/
BOOLEAN
GpioIsDswGroup (
  IN  GPIO_GROUP         Group
  )
{
  if (Group == GPIO_VER3_GROUP_GPP_O) {
    return TRUE;
  } else {
    return FALSE;
  }
}


/**
  This procedure will perform special handling of GPP_A_12.

  @param[in]  None

  @retval None
**/
VOID
GpioA12SpecialHandling (
  VOID
  )
{
  return;
}

GLOBAL_REMOVE_IF_UNREFERENCED PCH_SBI_PID mGpioComSbiIds[] =
{
  PID_GPIOCOM0, PID_GPIOCOM1, PID_GPIOCOM2, PID_GPIOCOM3, PID_GPIOCOM4, PID_GPIOCOM5
};

/**
  This function provides GPIO Community PortIDs

  @param[out] NativePinsTable                Table with GPIO COMMx SBI PortIDs

  @retval     Number of communities
**/
UINT32
GpioGetComSbiPortIds (
  OUT PCH_SBI_PID    **GpioComSbiIds
  )
{
  *GpioComSbiIds = mGpioComSbiIds;
  return ARRAY_SIZE (mGpioComSbiIds);
}

/**
  This function provides list of GPIO for which IO Standby State configuration
  has to be set as 'Masked'

  @param[out] GpioPadsList                Table with pads

  @retval      Number of pads
**/
UINT32
GpioGetIoStandbyStateConfigurationPadsList (
  OUT GPIO_PAD_NATIVE_FUNCTION      **GpioPadsList
  )
{
  *GpioPadsList = NULL;
  return 0;
}

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD mGpioStandbyStateMaskedPads[] =
{
  GPIO_VER3_JTAGX,
  GPIO_VER3_PRDYB,
  GPIO_VER3_PREQB,
  GPIO_VER3_GPP_D11,
  GPIO_VER3_GPP_J3,
  GPIO_VER3_GPP_J4
};

/**
  This function provides list of GPIO for which IO Standby State configuration
  has to be set as 'Masked'

  @param[out] GpioPadsList                Table with pads

  @retval      Number of pads
**/
UINT32
GpioGetDFxStandbyStateMaskedPadsList (
  OUT GPIO_PAD      **GpioPadsList
  )
{
  *GpioPadsList = mGpioStandbyStateMaskedPads;
  return ARRAY_SIZE (mGpioStandbyStateMaskedPads);
}

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_GROUP_TO_GPE_MAPPING mPchGpioGroupToGpeMapping[] = {
    {GPIO_VER3_GROUP_GPP_A,   0, V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_A,   V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_A},
    {GPIO_VER3_GROUP_GPP_B,   0, V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_B,   V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_B},
    {GPIO_VER3_GROUP_SPI,     0, V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_SPI, V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_SPI},
    {GPIO_VER3_GROUP_GPP_C,   0, V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_C,   V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_C},
    {GPIO_VER3_GROUP_GPP_D,   0, V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_D,   V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_D},
    {GPIO_VER3_GROUP_GPP_E,   0, V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_E,   V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_E},
    {GPIO_VER3_GROUP_JTAG,    0, V_VER3_PCH_PMC_PWRM_GPIO_CFG_JTAG,    V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_JTAG},
    {GPIO_VER3_GROUP_GPP_H,   0, V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_H,   V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_H},
    {GPIO_VER3_GROUP_GPP_I,   0, V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_I,   V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_I},
    {GPIO_VER3_GROUP_GPP_L,   0, V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_L,   V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_L},
    {GPIO_VER3_GROUP_GPP_M,   0, V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_M,   V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_M},
    {GPIO_VER3_GROUP_GPP_N,   0, V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_N,   V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_N},
    {GPIO_VER3_GROUP_GPP_O,   0, V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_O,   V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_O}
};


/**
  Get information for GPIO Group required to program GPIO and PMC for desired 1-Tier GPE mapping

  @param[out] GpioGroupToGpeMapping        Table with GPIO Group to GPE mapping
  @param[out] GpioGroupToGpeMappingLength  GPIO Group to GPE mapping table length
**/
VOID
GpioGetGroupToGpeMapping (
  OUT GPIO_GROUP_TO_GPE_MAPPING  **GpioGroupToGpeMapping,
  OUT UINT32                     *GpioGroupToGpeMappingLength
  )
{
  *GpioGroupToGpeMapping = mPchGpioGroupToGpeMapping;
  *GpioGroupToGpeMappingLength = ARRAY_SIZE (mPchGpioGroupToGpeMapping);
}

/**
  Check if 0x13 opcode supported for writing to GPIO lock unlock register

  @retval TRUE                It's supported
  @retval FALSE               It's not supported
**/
BOOLEAN
IsGpioLockOpcodeSupported (
  VOID
  )
{
  return FALSE;
}

/**
  This procedure will set GPIO pad to native function based on provided native function
  and platform muxing selection (if needed).

  @param[in]  PadFunction         PadMode for a specific native signal. Please refer to GpioNativePads.h
  @param[in]  PinMux              GPIO Native pin mux platform config.
                                  This argument is optional and needs to be
                                  provided only if feature can be enabled
                                  on multiple pads

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioSetNativePadByFunction (
  IN UINT32  PadFunction,
  IN UINT32  PinMux
  )
{
  GPIO_NATIVE_PAD GpioNativePad;
  EFI_STATUS      Status;

  GpioNativePad = GpioGetNativePadByFunctionAndPinMux (PadFunction, PinMux);

  if (GpioNativePad == GPIO_PAD_NONE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GpioSetPadMode (GpioNativePad, GPIO_NATIVE_GET_PAD_MODE (GpioNativePad));
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  Status = GpioConfigurePadIoStandbyByFunction (PadFunction, PinMux);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
  This function performs basic initialization IOM for AUX Layout in PEI phase after Policy produced at Post-Mem phase.

  For those GPIO pins used for DP Aux orientation control and enabled, BIOS is responsible to -
  1. Configure Pad Mode (PMode) to function# associated with IOM_GPP*_*
     The Pad Mode here could be various per PCH design.
  2. BIOS Provide the following information for the DP Aux orientation bias control for all supported Type-C ports
     to IOM FW
     I. GPIO endpoint IOSF-SB port ID (CPU local port ID)
     II. VW index and data bit position

  @param[in] ConfigTablePtr - Instance pointer of IOM_AUX_ORI_PAD_CONFIG_HEAD

**/
VOID
GpioIomAuxOriSetting (
  IN TCSS_IOM_PEI_CONFIG      *ConfigTablePtr
  )
{
  return;
}
