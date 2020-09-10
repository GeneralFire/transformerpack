/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2020 Intel Corporation. <BR>

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
#include <Library/UbaGpioPlatformConfig.h>
#include <Pins/GpioPinsVer3.h>

#define PLATFORM_GPIO_CONFIG_SIGNATURE  SIGNATURE_32 ('P', 'G', 'P', 'O')
#define PLATFORM_GPIO_CONFIG_VERSION    01

//Restricted begin:
//TODO Ebg Gpio: Check config for Ebg
//Restricted end:

STATIC PLATFORM_GPIO_CONFIG_TABLE mGpioPlatformConfigArcherCity = {

  PLATFORM_GPIO_CONFIG_SIGNATURE,
  PLATFORM_GPIO_CONFIG_VERSION,

  // MFG pad
  {
    GPIO_VER3_GPP_L6,
    {GpioPadModeGpio, GpioHostOwnGpio, GpioDirIn, GpioOutDefault, GpioIntDis, GpioPlatformReset, GpioTermNone}
  },

  // DFX jumper pad
  GPIO_VER3_GPP_L3,

  //Recovery jumper pad
  GPIO_VER3_GPP_M11,

  //FM ADR Trigger
  UNUSED_GPIO,

  //ADR enable GPIO output
  UNUSED_GPIO,

  // Force to S1 config mode pad
  UNUSED_GPIO,

  //
  // Used by PC Platforms. QAT pad
  //
  UNUSED_GPIO,

  // Used by PC platforms. This is the first GPIO pad of the pad series to indicate Board ID
  UNUSED_GPIO,

  // WHEA SCI generation pad
  UNUSED_GPIO,

  // Used to generate CPU HP SMI
  UNUSED_GPIO,

  // FPGA error indicator
  UNUSED_GPIO,

  // FPGA error indicator
  UNUSED_GPIO,

  // Flash Security
  UNUSED_GPIO,
};


EFI_STATUS
TypeArcherCityRPInstallGpioPlatformData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
)
{
  EFI_STATUS                            Status;

  DEBUG((DEBUG_INFO,"UBA PEI: %a\n",__FUNCTION__));

  Status = UbaConfigPpi->AddData (
                                 UbaConfigPpi,
                                 &gPlatformGpioPlatformConfigDataGuid,
                                 &mGpioPlatformConfigArcherCity,
                                 sizeof(mGpioPlatformConfigArcherCity)
                                 );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
