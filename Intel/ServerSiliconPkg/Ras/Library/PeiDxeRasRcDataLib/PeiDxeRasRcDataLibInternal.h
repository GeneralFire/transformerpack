/** @file
  PEI/DXE common non-API RAS RC data functions.

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

#ifndef _PEI_DXE_RAS_RC_DATA_LIB_INTERNAL_H_
#define _PEI_DXE_RAS_RC_DATA_LIB_INTERNAL_H_

/**
  Get RAS RC HOB.

  Helper function for getting one of our GUIDed HOBs.

  @param[in]  HobGuid         The GUID of the HOB to get.

  @return   A pointer to the GUID HOB's data, or NULL if not found.
**/
VOID *
GetRasRcHobInternal (
  IN CONST EFI_GUID *HobGuid
  );

#endif // _PEI_DXE_RAS_RC_DATA_LIB_INTERNAL_H_
