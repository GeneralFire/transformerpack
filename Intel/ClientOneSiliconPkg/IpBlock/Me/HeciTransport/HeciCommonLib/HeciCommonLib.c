/** @file
  HECI common library.

 @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2019 - 2020 Intel Corporation.

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

**/
#include <PiPei.h>
#include <Uefi.h>
#include <Library/DebugLib.h>

/**
  Return HECI number based on HECI Handle

  @param[in]      PciFunction    Pci function to be converted to HECI number
  @param[in,out]  HeciNumber     Pointer to returned Heci number
                                 Returned HECI communication number 1..4
                                 0 is returned for non communication devices
                                 (SOL and IDEr)

  @retval EFI_INVALID_PARAMETER Invalid input parameters
  @retval EFI_DEVICE_ERROR      Passed function not supported
  @retval EFI_SUCCESS           Returned valid HECI number
**/
EFI_STATUS
GetHeciNumber (
  IN     UINTN        PciFunction,
  IN OUT UINTN        *HeciNumber
  )
{
  EFI_STATUS   ReturnStatus;
  CONST UINT8  HeciFn2No[] = { 1, 2, 0, 0, 3, 4 };

  if (HeciNumber == NULL) {
    ASSERT (HeciNumber != NULL);
    return EFI_INVALID_PARAMETER;
  }
  if (PciFunction < sizeof (HeciFn2No)/sizeof (HeciFn2No[0])) {
    *HeciNumber = HeciFn2No[PciFunction];
    ReturnStatus = EFI_SUCCESS;
  } else {
    *HeciNumber = MAX_UINTN;
    ReturnStatus = EFI_DEVICE_ERROR;
  }

  return ReturnStatus;
}