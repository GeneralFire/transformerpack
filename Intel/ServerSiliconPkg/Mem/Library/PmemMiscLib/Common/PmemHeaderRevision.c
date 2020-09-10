/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <Base.h>
#include <Library/BaseLib.h>
#include <Uefi.h>
#include <Library/PmemMiscLib.h>

/**
  Verifies if CR S-F interface spec. header revision is compatible with currently used
  (for backward compatibility checks).

  @param[in] Revision    The header reversion currently used.

  @retval TRUE    CR S-F interface spec. revision is valid.
          FALSE   CR S-F interface spec. revision is not valid

**/
BOOLEAN
EFIAPI
IsPmemConfigHeaderRevisionCompatible (
  IN UINT8 Revision
  )
{
  BOOLEAN Compatible = FALSE;

  switch (GetPmemConfigHeaderRevision ()) {
  case NGN_CR_SW_FW_INTERFACE_REVISION_2_01_HDR:
    if ((Revision == NGN_CR_SW_FW_INTERFACE_REVISION_1_00_HDR) ||
        (Revision == NGN_CR_SW_FW_INTERFACE_REVISION_2_00_HDR)) {
      Compatible = TRUE;
    }
    break;
  default:
    break;
  }

  return Compatible;
}

/**
  Verifies if CR S-F interface spec. header revision of PCDs (CCUR, CIN_, COUT)
  is compatible with currently used (for backward compatibility checks).

  @param[in] Revision    The header reversion currently used.

  @retval TRUE    CR S-F interface spec. revision is valid
          FALSE   CR S-F interface spec. revision is not valid

**/
BOOLEAN
EFIAPI
IsPmemPcdRecordsHeaderRevisionCompatible (
  IN UINT8 Revision
  )
{
  BOOLEAN Compatible = FALSE;

  switch (GetPmemPcdRecordsHeaderRevision ()) {
  case NGN_CR_SW_FW_INTERFACE_REVISION_2_01_CFG_HDR:
    if ((Revision == NGN_CR_SW_FW_INTERFACE_REVISION_1_00_CFG_HDR) ||
        (Revision == NGN_CR_SW_FW_INTERFACE_REVISION_2_00_CFG_HDR)) {
      Compatible = TRUE;
    }
    break;
  default:
    break;
  }

  return Compatible;
}
