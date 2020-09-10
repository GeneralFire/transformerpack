/** @file
  CIO2 policy

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2020 Intel Corporation. <BR>

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

#ifndef _ADR_CONFIG_H_
#define _ADR_CONFIG_H_

#define ADR_CONFIG_REVISION 1
extern EFI_GUID gAdrConfigGuid;

#pragma pack (push,1)

#include <PchLimits.h>

typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header
  UINT8   AdrEn;
  UINT8   AdrTimerEn;
  UINT8   AdrTimerVal;
  UINT8   AdrMultiplierVal;
  UINT8   AdrGpioSel;
  UINT8   AdrSrcSel[PCH_MAX_ADR_SOURCE_NUMBER];
  UINT8   AdrReset;
  UINT8   Rsvd;
} PCH_ADR_CONFIG;

#pragma pack (pop)

#endif // _ADR_CONFIG_H_
