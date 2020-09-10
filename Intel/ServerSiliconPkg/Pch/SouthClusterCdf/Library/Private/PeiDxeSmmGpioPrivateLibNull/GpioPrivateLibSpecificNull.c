/** @file
  This file contains CDF specific GPIO information

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2018 Intel Corporation. <BR>

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

#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/PchInfoLib.h>
#include <Library/GpioLib.h>
#include <Library/GpioNativeLib.h>
#include <Private/Library/GpioPrivateLib.h>
#include <Register/PchRegsGpioCnl.h>
#include <Register/PchRegsGpioCdf.h>
#include <Register/PchRegsPmcCdf.h>
#include <Register/PchRegsPmc.h>
#include <GpioPinsCnlLp.h>
#include <GpioPinsCnlH.h>
#include <GpioPinsCdf.h>
#include <GpioConfig.h>
#include <Register/PchRegsPcr.h>

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_GROUP_INFO mPchHGpioGroupInfo[] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
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
  *GpioGroupInfoTableLength = ARRAY_SIZE (mPchHGpioGroupInfo);
  return mPchHGpioGroupInfo;
}

/**
  Get GPIO Chipset ID specific to PCH generation and series
**/
UINT32
GpioGetThisChipsetId (
  VOID
  )
{
  return 0;
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
  return FALSE;
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


/**
  This function provides GPIO Community PortIDs

  @param[out] NativePinsTable                Table with GPIO COMMx SBI PortIDs

  @retval      Number of communities
**/
UINT32
GpioGetComSbiPortIds (
  OUT PCH_SBI_PID    **GpioComSbiIds
  )
{
  return 0;
}

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

}
