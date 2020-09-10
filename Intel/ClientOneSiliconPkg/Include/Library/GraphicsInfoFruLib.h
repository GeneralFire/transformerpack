/** @file
  Header file for Graphics Info Fru Lib implementation.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation.

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

#ifndef _GRAPHICS_INFO_FRU_LIB_H_
#define _GRAPHICS_INFO_FRU_LIB_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci22.h>
#include <Register/SaRegsHostBridge.h>

/**
  Checks if SKU is DisplayOnly

  @retval FALSE  SKU is not DisplayOnly
  @retval TRUE   SKU is DisplayOnly
**/
BOOLEAN
EFIAPI
IsDisplayOnlySku (
  VOID
  );

/**
  Check for Gen9 Graphics Supported Platforms

  @retval FALSE  Gen9 not Supported
  @retval TRUE   Gen9 Supported
**/
BOOLEAN
EFIAPI
IsGraphicsGen9 (
  VOID
  );

/**
  Program the Display Power 2 Wells supported by platform

  @param[in] GttMmAdr            - Base Address of IGFX MMIO BAR

  @retval EFI_SUCCESS            - Power well programming finished successfully
  @retval EFI_UNSUPPORTED        - Power well programming failed
  @retval EFI_TIMEOUT            - Timed out
**/
EFI_STATUS
EnablePowerWell2 (
  IN  UINT32     GttMmAdr
);

/**
  Program the Display Power Wells 3 supported by platform

  @param[in] GttMmAdr            - Base Address of IGFX MMIO BAR

  @retval EFI_SUCCESS            - Power well programming finished successfully
  @retval EFI_UNSUPPORTED        - Power well programming failed
  @retval EFI_TIMEOUT            - Timed out
**/
EFI_STATUS
EnablePowerWell3 (
  IN  UINT32     GttMmAdr
);

#endif
