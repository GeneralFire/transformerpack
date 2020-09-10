/** @file
  This file is I/O Expander library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/PchIoExpander.h>

/**
  Function returns PchId for devices of passed Segment and Bus.
  PchId is an index in IO_EXPANDER_PCH_INFO table incremented by 1

  @param [in]      Segment  Segment to look for
  @param [in]      Bus      Bas to look for
  @param [in, out] PchId    pointer to variable to return PchId

  @retval EFI_SUCCESS           Function returned valid PchId
  @retval EFI_INVALID_PARAMETER Passed pointer is invalid
  @retval EFI_DEVICE_ERROR      Not found valid IoExpanderInfo;
  @retval EFI_NOT_FOUND         For given input parameters not found valid PchId
**/
EFI_STATUS
FindMultiPchInfo (
  IN     UINTN       Segment,
  IN     UINTN       Bus,
  IN OUT UINTN       *PchId
  )
{
  UINTN               IoExpanderIndex;
  IO_EXPANDER_INFO    *IoExpanderInfo;
  EFI_STATUS          Status;

  if (PchId == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  Status = PchGetIoExpanderInfo (&IoExpanderInfo);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  IoExpanderIndex = 0;
  Status = EFI_NOT_FOUND;
  while (IoExpanderIndex < IO_EXPANDER_MAX) {
    if (IoExpanderInfo->Valid[IoExpanderIndex] &&
        (Segment == IoExpanderInfo->Segment[IoExpanderIndex]) &&
        (Bus == IoExpanderInfo->Bus[IoExpanderIndex])) {
      *PchId = IoExpanderIndex + 1; // Adding PCH Legacy
      Status = EFI_SUCCESS;
      break;
    }
    IoExpanderIndex++;
  }

  return Status;
}