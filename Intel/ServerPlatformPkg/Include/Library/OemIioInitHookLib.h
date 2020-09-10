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

#ifndef  _OEM_IIO_INIT_HOOK_LIB_H_
#define  _OEM_IIO_INIT_HOOK_LIB_H_

#define OEM_IIO_VOLATILE_DATA_GUID \
  { \
    0x5F6A2D82, 0xEF7F, 0x4557, 0xB4, 0xD2, 0xEE, 0x4B, 0x79, 0x8C, 0x71, 0x35 \
  }

typedef struct {
  UINT8  IMPWORKAROUND[MAX_SOCKET][NUMBER_PORTS_PER_SOCKET];
} OEM_IIO_VOLATILE;

VOID
OemIioPlatformWorkAround (
  IN  IIO_GLOBALS             *IioGlobalData,
  IN  UINT8                   IioIndex
  );

#endif
