/** @file
  Breakpoint GUID definitions

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2019 Intel Corporation. <BR>

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

#define EFI_AFTER_MRC_GUID \
  { 0x2bac49bc, 0x5628, 0x4f1b, { 0x95, 0x24, 0x86, 0xbd, 0xda, 0x00, 0x1c, 0x47 } }
#define EFI_AFTER_RESOURCE_ALLOC_GUID \
  { 0x76b4d838, 0x6204, 0x4e75, { 0x9b, 0xf9, 0x42, 0x2d, 0x2f, 0x64, 0xbf, 0xa1 } }
#define EFI_AFTER_POST_GUID \
  { 0xf2b76307, 0x0881, 0x4513, { 0xb1, 0xc6, 0xab, 0x5c, 0x7c, 0xc5, 0xbe, 0xe1 } }
#define EFI_AFTER_FULL_SPEED_SETUP_GUID \
  { 0x02812072, 0x6d3d, 0x42b1, { 0x84, 0x24, 0xb3, 0xae, 0x38, 0x05, 0xa0, 0x21 } }
#define EFI_READY_FOR_IBIST_GUID \
  { 0x7bbc005c, 0xab40, 0x4771, { 0x94, 0xc9, 0xe5, 0xa3, 0xa5, 0x05, 0x38, 0x09 } }


#define BP_NONE                     0
#define BP_AFTER_MRC                1
#define BP_AFTER_KTIRC              2
#define BP_AFTER_RESOURCE_ALLOC     3
#define BP_AFTER_POST               4
#define BP_AFTER_FULL_SPEED_SETUP   5
#define BP_READY_FOR_IBIST          6
