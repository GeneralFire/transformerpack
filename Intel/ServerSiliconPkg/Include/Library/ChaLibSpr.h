/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef __CHA_LIB_SPR_H__
#define __CHA_LIB_SPR_H__

#include <Uefi.h>
#include <Base.h>
#include <Upi/KtiHost.h>

/**
  CHA: Configure MMIOL Rule

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param MmiolRuleRegIndex    - MMIOL Rule Register, 0 based
  @param EnableRule           - Enable MMIOL Rule
  @param MmiolBase            - MMIOL Base
  @param MmiolLimit           - MMIOL Limit
  @param MmiolTarget          - MMIOL Target

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureMmiolRule (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT8    MmiolRuleRegIndex,
  UINT32   EnableRule,
  UINT32   MmiolBase,
  UINT32   MmiolLimit,
  UINT32   MmiolTarget
  );
//
// Function pointer definiton for ChaConfigureMmiolRule()
//
typedef
EFI_STATUS
(EFIAPI *CHA_CFG_MMIOL_RULE) (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT8    MmiolRuleRegIndex,
  UINT32   EnableRule,
  UINT32   MmiolBase,
  UINT32   MmiolLimit,
  UINT32   MmiolTarget
  );
#endif // __CHA_LIB_SPR_H__
