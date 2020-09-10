/** @file
  GUID used for Memory Setup Data entries in the HOB list.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2006 Intel Corporation. <BR>

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

#ifndef _MEMORY_SETUP_DATA_GUID_H_
#define _MEMORY_SETUP_DATA_GUID_H_

#define EFI_MEMORY_SETUP_GUID \
  { \
    0x3eeff35f,0x147c,0x4cd1,0xa2,0x34,0x92,0xa0,0x69,0x70,0x0d,0xb6 \
  }

extern EFI_GUID gEfiMemorySetupGuid;
extern CHAR16   EfiMemorySetupVariable[];

#endif
