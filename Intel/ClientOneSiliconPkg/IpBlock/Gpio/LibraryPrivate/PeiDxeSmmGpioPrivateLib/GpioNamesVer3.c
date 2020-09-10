/** @file
  This file contains GPIO name library implementation specific to Ver3

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

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
#include <Base.h>
#include <Library/BaseLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/PchInfoLib.h>
#include <Library/GpioPrivateLib.h>
#include <Pins/GpioPinsVer3.h>

STATIC CONST CHAR8*  mGpioGppaNames[] = {
  "ESPI_CLK_LOOPBK"
};

STATIC CONST CHAR8*  mGpioSpiNames[] = {
  "SPI0_IO_2",
  "SPI0_IO_3",
  "SPI0_MOSI_IO_0",
  "SPI0_MOSI_IO_1",
  "SPI0_TPM_CSB",
  "SPI0_FLASH_0_CSB",
  "SPI0_FLASH_1_CSB",
  "SPI0_CLK",
  "GPPC_S0",
  "GPPC_S1",
  "GPPC_S2",
  "GPPC_S3",
  "GPPC_S4",
  "GPPC_S5",
  "GPPC_S6",
  "GPPC_S7",
  "GPPC_S8",
  "GPPC_S9",
  "GPPC_S10",
  "GPPC_S11",
  "SPI0_CLK_LOOPBK"
};

STATIC CONST CHAR8*  mGpioJtagNames[] = {
  "JTAG_TDO",
  "JTAG_TDI",
  "JTAG_TCK",
  "JTAG_TMS",
  "JTAGX",
  "PRDYB",
  "PREQB",
  "GLB_PC_DISABLE",
  "DBG_PMODE",
  "GLB_EXT_ACC_DISABLE"
};

STATIC CONST CHAR8*  mGpioGpjNames[] = {
  "CPUPWRGD",
  "THRMTRIPB",
  "PLTRST_CPUB",
  "TRIGGER0",
  "TRIGGER1",
  "CPUPWRDEBUG",
  "CPUMEMTRIP",
  "CPUMSMI",
  "ME_PECI",
  "NAC_SPARE0",
  "NAC_SPARE1",
  "NAC_SPARE2",
  "CPU_ERR0",
  "CPU_CATERR",
  "CPU_ERR1",
  "CPU_ERR2",
  "NAC_SPARE3",
  "NAC_SPARE4"
};

STATIC CONST CHAR8*  mGpioGpoNames[] = {
  "ACPRESENT",
  "LAN_WAKEB",
  "PWRBTNB",
  "SLP_S3B",
  "SLP_S4B",
  "SLP_AB",
  "GPP_O7",
  "SUSCLK",
  "GPP_O9",
  "SLP_S5B",
  "LANPHYPC",
  "SYS_PWROK",
  "SYS_RESETB",
  "SLP_LANB",
  "WAKEB",
  "SLP_SUSB"
};

STATIC CONST GPIO_GROUP_NAME_INFO  mPchGroupDescriptors[] = {
  GPIO_GROUP_NAME("GPP_A", GPIO_VER3_ESPI_CLK_LOOPBK, mGpioGppaNames),
  GPIO_GROUP_NAME_BASIC("GPP_B"),
  GPIO_GROUP_NAME("SPI", GPIO_VER3_SPI0_IO_2, mGpioSpiNames),
  GPIO_GROUP_NAME_BASIC("GPP_C"),
  GPIO_GROUP_NAME_BASIC("GPP_D"),
  GPIO_GROUP_NAME_BASIC("GPP_E"),
  GPIO_GROUP_NAME("JTAG", GPIO_VER3_JTAG_TDO, mGpioJtagNames),
  GPIO_GROUP_NAME_BASIC("GPP_H"),
  GPIO_GROUP_NAME("GPP_J", GPIO_VER3_GPP_J0, mGpioGpjNames),
  GPIO_GROUP_NAME_BASIC("GPP_I"),
  GPIO_GROUP_NAME_BASIC("GPP_L"),
  GPIO_GROUP_NAME_BASIC("GPP_M"),
  GPIO_GROUP_NAME_BASIC("GPP_N"),
  GPIO_GROUP_NAME("GPP_O", GPIO_VER3_GPP_O1, mGpioGpoNames),
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
  if (GroupIndex < ARRAY_SIZE (mPchGroupDescriptors)) {
    return &mPchGroupDescriptors[GroupIndex];
  } else {
    ASSERT (FALSE);
    return NULL;
  }
}
