/** @file
  MeFiaMuxStrapPeiLib.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include <Uefi.h>
#include <Library/PciLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MeFiaMuxLib.h>
#include <Protocol/Spi.h>
#include <Sps.h>

/**
  Read FIA MUX configuration from PCH soft straps.

  The function reads FIA MUX configuration from PCH Soft Straps and puts it into
  a table of ME_FIA_CONFIG structure.

  @param[out] FiaConfigSoftStraps Pointer to ME_FIA_CONFIG where data will be put

  @retval EFI_SUCCESS             The function succeeded.
  @retval EFI_UNSUPPORTED         Not supported.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
**/
EFI_STATUS
ReadFiaMuxConfFromSoftStraps (
  OUT ME_FIA_CONFIG *FiaConfigSoftStraps
  )
{
  return EFI_UNSUPPORTED;
}
