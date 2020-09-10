/** @file
  UBA GPIO Initializtion Library Header File.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation. <BR>

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

#ifndef _UBA_GPIO_INIT_LIB_H
#define _UBA_GPIO_INIT_LIB_H

#include <Base.h>
#include <Uefi.h>

// {9282563E-AE17-4E12-B1DC-070F29F37120}
#define   PLATFORM_GPIO_INIT_DATA_GUID \
{ 0x9282563e, 0xae17, 0x4e12, { 0xb1, 0xdc, 0x7, 0xf, 0x29, 0xf3, 0x71, 0x20 } }

EFI_STATUS
PlatformInitGpios (
  VOID
);

#endif //_UBA_GPIO_INIT_LIB_H
