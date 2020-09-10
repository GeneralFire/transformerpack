/** @file
  Prototype of the MultiPch library - Base definitions.

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

#ifndef _PCH_MULTI_PCH_BASE_H_
#define _PCH_MULTI_PCH_BASE_H_


#define PCH_LEGACY_ID                     0
#define PCH_MAX                           4

/**
  Function returns PchId for devices of passed Segment and Bus.
  PchId is an index in PCH_IP_INFO table

  @param [in]      Segment  Segment to look for
  @param [in]      Bus      Bas to look for
  @param [in, out] PchId    pointer to variable to return PchId

  @retval EFI_SUCCESS           Function returned valid PchId
  @retval EFI_INVALID_PARAMETER Passed pointer is invalid
  @retval EFI_DEVICE_ERROR      Not found valid PchInfo
  @retval EFI_NOT_FOUND         For given input parameters not found valid PchId
**/
EFI_STATUS
FindMultiPchInfo (
  IN     UINTN       HeciSegment,
  IN     UINTN       HeciBus,
  IN OUT UINTN       *PchId
  );

#endif // _PCH_MULTI_PCH_BASE_H_
