/** @file
  RAS RC mirror library class header file.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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
**/

#ifndef _RAS_RC_MIRROR_LIB_H_
#define _RAS_RC_MIRROR_LIB_H_

#include <Uefi/UefiBaseType.h>

#define RAS_IS_CAPABLE                            1
#define RAS_NOT_CAPABLE                           2

/**
  Check if low memory (below 4GB) mirror is enabled for a given memory type.

  @param[in]      MemoryType        Type of memory to check enable for.

  @retval TRUE                      Enabled for the given memory type.
  @retval FALSE                     Disabled for the given memory type.
**/
BOOLEAN
EFIAPI
IsLowMemoryMirrorEnabled (
  IN        UINTN     MemoryType
  );

/**
  Check if mirror by ratio is enabled for a given memory type.

  If this function returns TRUE, GetMirrorRatio should be called to determine
  what ratio to use.

  @param[in]      MemoryType        Type of memory to check enable for.

  @retval TRUE                      Enabled for the given memory type.
  @retval FALSE                     Disabled for the given memory type.
**/
BOOLEAN
EFIAPI
IsMirrorByRatioEnabled (
  IN        UINTN     MemoryType
  );

/**
  Check if mirror by size is enabled for a given memory type.

  If this function returns TRUE, GetMirrorSizes should be called to determine
  what block sizes to use.

  @param[in]      MemoryType        Type of memory to check enable for.

  @retval TRUE                      Enabled for the given memory type.
  @retval FALSE                     Disabled for the given memory type.
**/
BOOLEAN
EFIAPI
IsMirrorBySizeEnabled (
  IN        UINTN     MemoryType
  );

/**
  Get the mirror ratio for a given memory type.

  This function should not be called and will fail if IsMirrorByRatioEnabled
  returns FALSE.

  Given the Numerator (N) and Denominator (D) returned by this function, and
  the total memory size (T), the mirror size (M) should be computed as follows:

  M = (T * N) / D

  @param[in]      MemoryType        Type of memory to get ratio for.
  @param[out]     Numerator         Numerator of the ratio.
  @param[out]     Denominator       Demoninator of the ratio.

  @retval EFI_SUCCESS               Data successfully returned.
  @retval EFI_UNSUPPORTED           Unsupported for the given memory type.
  @retval EFI_UNSUPPORTED           Ratio greater than 1/2 was requested.
  @retval EFI_INVALID_PARAMETER     A required pointer parameter was NULL.
**/
EFI_STATUS
EFIAPI
GetMirrorRatio (
  IN        UINTN     MemoryType,
  OUT       UINT32    *Numerator,
  OUT       UINT32    *Denominator
  );

/**
  Get the mirror sizes for a given memory type.

  This function should not be called and will fail if IsMirrorBySizeEnabled
  returns FALSE.

  To get the sizes, the caller should provide an array pointed to by Sizes,
  with *Count set to the length of the array. If the number of sizes supported
  is less than or equal to *Count, this function will update *Count to the
  number supported and return EFI_SUCCESS.

  To query the total number of sizes supported for the given memory type, the
  caller should set *Count to 0. Sizes may be NULL in this case only. This
  function returns EFI_BUFFER_TOO_SMALL with *Count updated to the number of
  supported sizes. The caller may then allocate an array of appropriate size
  before calling this function again.

  @param[in]      MemoryType        Type of memory to get ratio for.
  @param[in, out] Count             Number of sizes provided.
  @param[out]     Sizes             Array of *Count mirror sizes.

  @retval EFI_SUCCESS               Data successfully returned.
  @retval EFI_UNSUPPORTED           Unsupported for the given memory type.
  @retval EFI_INVALID_PARAMETER     A required pointer parameter was NULL.
  @retval EFI_BUFFER_TOO_SMALL      Array too small on input, *Count updated.
**/
EFI_STATUS
EFIAPI
GetMirrorSizes (
  IN        UINTN     MemoryType,
  IN OUT    UINT32    *Count,
  OUT       UINT32    *Sizes      OPTIONAL
  );

#endif // _RAS_RC_MIRROR_LIB_H_
