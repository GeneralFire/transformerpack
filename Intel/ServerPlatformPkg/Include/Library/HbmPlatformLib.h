/** @file
  Interface header file for the HBM library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2017 Intel Corporation. <BR>

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

#ifndef _HBM_PLATFORM_LIB_H_
#define _HBM_PLATFORM_LIB_H_

//
// Public HBM Platform interfaces
//

/**
  Populates the system memory map for HBM devices

  @param[in, out]           SystemMemoryMap     Pointer to system memory map

  @retval                   EFI_SUCCESS             Function populated data for all HBM devices
  @retval                   EFI_INVALID_PARAMETER   Null pointer to system memory map
**/
EFI_STATUS
EFIAPI
PopulateHbmSystemMemoryMap (
  IN  OUT   SYSTEM_MEMORY_MAP_HOB   *SystemMemoryMap
  );

/**

  Get platform setup values for HBM settings.

  @param[in]                SetupData           Pointer to SETUP_DATA structure.

**/
VOID
EFIAPI
OemGetPlatformSetupValueforHbm (
  // APTIOV_SERVER_OVERRIDE_RC_START : To solve structure redefinition error in AMI.
  IN  OUT   INTEL_SETUP_DATA      *SetupData
  // APTIOV_SERVER_OVERRIDE_RC_END : To solve structure redefinition error in AMI.
  );

/**
  Populates the system memory map for HBM devices

  @retval                   EFI_SUCCESS             Function populated data for all HBM devices
**/
EFI_STATUS
EFIAPI
SaveHbmHostToMemory (
  VOID
  );

#endif // _HBM_PLATFORM_LIB_H_
