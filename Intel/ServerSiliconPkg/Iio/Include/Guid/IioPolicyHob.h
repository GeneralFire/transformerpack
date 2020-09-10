/** @file
  This file contains definitions of IIO Policy HOB.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#ifndef _IIO_POLICY_HOB_H_
#define _IIO_POLICY_HOB_H_

extern EFI_GUID gIioPolicyHobGuid;
extern EFI_GUID gEfiOpaSocketMapHobGuid;

typedef struct {
    UINT8 Valid;
    UINT8 SocketId;
    UINT8 Segment;
    UINT8 BusBase;
    UINT8 BusLimit;
    UINT8 Reserved;
    UINT8 Reserved2;
    UINT8 Reserved3;
} EFI_OPA_SOCKET_MAP;

typedef struct {
  EFI_HOB_GUID_TYPE   EfiHobGuidType; // mandatory field
  EFI_OPA_SOCKET_MAP  Info[MaxIIO];
} OPA_SOCKET_MAP_HOB;

#endif // _IIO_POLICY_HOB_H_

