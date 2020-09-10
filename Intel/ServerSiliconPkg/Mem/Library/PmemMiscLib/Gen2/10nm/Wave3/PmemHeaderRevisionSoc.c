/** @file

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

#include <Library/PmemMiscLib.h>

/**
  Get CR S-F interface spec. header revision for the type of the CPU.

  @retval  CR S-F interface spec. revision

**/
UINT8
EFIAPI
GetPmemConfigHeaderRevision  (
  VOID
  )
{
  return NGN_CR_SW_FW_INTERFACE_REVISION_3_01_HDR;
}

/**
  Get CR S-F interface spec. header revision of PCDs (CCUR, CIN_, COUT) for the type of the CPU.

  @retval  CR S-F interface spec. PCD revision

**/
UINT8
EFIAPI
GetPmemPcdRecordsHeaderRevision (
  VOID
  )
{
  return NGN_CR_SW_FW_INTERFACE_REVISION_3_01_CFG_HDR;
}

/**
  Get PCAT revision for the type of the CPU

  @retval  PCAT header revision

**/
UINT8
EFIAPI
GetPcatHeaderRevision (
  VOID
  )
{
  return NVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE_3_01_HDR;
}
