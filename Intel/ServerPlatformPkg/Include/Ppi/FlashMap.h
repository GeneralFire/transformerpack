/** @file
  FlashMap PPI defined in Tiano
  This code abstracts FlashMap access

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 Intel Corporation. <BR>

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

#ifndef _PEI_FLASH_MAP_PPI_H_
#define _PEI_FLASH_MAP_PPI_H_

#define PEI_FLASH_MAP_PPI_GUID \
  { \
    0xf34c2fa0, 0xde88, 0x4270, {0x84, 0x14, 0x96, 0x12, 0x22, 0xf4, 0x52, 0x1c} \
  }

#include "EfiFlashMap.h"




extern EFI_GUID gPeiFlashMapPpiGuid;

#endif // _PEI_FLASH_MAP_PPI_H_
