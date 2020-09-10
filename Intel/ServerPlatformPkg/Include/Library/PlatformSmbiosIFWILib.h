/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2017 Intel Corporation. <BR>

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

#ifndef _PLATFORM_SMBIOS_IFWI_LIB_H
#define _PLATFORM_SMBIOS_IFWI_LIB_H

#define DFT_ID "SmbiosIFWI"
#define DEBUG_FUN_ENTRY DEBUG((EFI_D_INFO, "["DFT_ID"] %s Entry\n", __FUNCTION__))
#define DEBUG_FUN_EXIT DEBUG((EFI_D_INFO, "["DFT_ID"] %s Exit\n", __FUNCTION__))

/**
  Plat_GetRCVer: Get Platform RC Version

  @param VersionString                    - pointer to RC Version string

  @retval None

**/

VOID
Plat_GetRCVer(
CHAR8 *VersionString
);

/**
  Plat_GetMEVer: Get Platform ME Version

  @param VersionString                    - pointer to ME Version string

  @retval None

**/

VOID
Plat_GetMEVer(
CHAR8 *VersionString
);
#endif


