/** @file
  Platform specific flash layout.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2005 Intel Corporation. <BR>

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

#ifndef _EFI_FLASH_LAYOUT_H_
#define _EFI_FLASH_LAYOUT_H_

#include "EfiFlashMap.h"


//
// Length of Efi Runtime Updatable Firmware Volume Header
//
#define EFI_RUNTIME_UPDATABLE_FV_HEADER_LENGTH  0x48
//
// APTIOV_SERVER_OVERRIDE_RC_START : Added to resolve redefinition error. 
//
#ifndef FLASH_BLOCK_SIZE
#define FLASH_BLOCK_SIZE            FixedPcdGet32 (PcdFlashBlockSize)
#endif
//
// APTIOV_SERVER_OVERRIDE_RC_END : Added to resolve redefinition error. 

//
// Flash supports 64K block size
//
#define FVB_MEDIA_BLOCK_SIZE        FLASH_BLOCK_SIZE

#define RUNTIME_FV_BASE_ADDRESS     FLASH_REGION_RUNTIME_UPDATABLE_BASE
#define RUNTIME_FV_LENGTH           (FLASH_REGION_RUNTIME_UPDATABLE_SIZE + FLASH_REGION_NV_FTW_SPARE_SIZE)
#define RUNTIME_FV_BLOCK_NUM        (RUNTIME_FV_LENGTH / FVB_MEDIA_BLOCK_SIZE)

#define FV_MAIN_BASE_ADDRESS        FLASH_REGION_FVMAIN_BASE
#define MAIN_BIOS_BLOCK_NUM         (FLASH_REGION_FVMAIN_SIZE / FVB_MEDIA_BLOCK_SIZE)

#define FV_MICROCODE_BASE_ADDRESS   FLASH_REGION_MICROCODE_FV_BASE
#define SYSTEM_MICROCODE_BLOCK_NUM  (FLASH_REGION_MICROCODE_FV_SIZE / FVB_MEDIA_BLOCK_SIZE)

#endif
