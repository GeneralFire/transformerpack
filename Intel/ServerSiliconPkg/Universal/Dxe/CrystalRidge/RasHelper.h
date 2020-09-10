/** @file RasHelper.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#ifndef _RASHELPER_H_
#define _RASHELPER_H_

/**
  Checks if gPrevBootErrSrcHobGuid HOB contains fatal errors within PMEM regions.

  @param[out]  Found                 Pointer to a BOOLEAN to store result in.
                                     TRUE:  fatal error in PMEM region found.
                                     FALSE: no error in PMEM regions found.

  @retval      EFI_SUCCESS           HOB examined completely.
  @retval      EFI_NOT_FOUND         HOB not found.
  @retval      EFI_COMPROMISED_DATA  HOB corrupted.

**/
EFI_STATUS
CheckPreviousDcpmmError (
  OUT BOOLEAN * Found
  );

#endif // _RASHELPER_H_
