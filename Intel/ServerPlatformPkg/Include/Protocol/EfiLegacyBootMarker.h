/** @file
  GUID used to detmerine if we are supporting an EFI Optimized boot.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2009 Intel Corporation. <BR>

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

#ifndef _EFI_LEGACY_BOOT_MARKER_GUID_H_
#define _EFI_LEGACY_BOOT_MARKER_GUID_H_

#define EFI_LEGACY_BOOT_MARKER_GUID \
  { \
 0x72e07da8, 0x63a9, 0x42d4,  0x83, 0xd5, 0xb, 0xb5, 0x18, 0x38, 0x44, 0x75 \
  }


#define EFI_WINDOWS_INT10_WORKAROUND \
  { \
  0x387f555, 0x20a8, 0x4fc2,  0xbb, 0x94, 0xcd, 0x30, 0xda, 0x1b, 0x40, 0x8  \
  }


extern EFI_GUID gEfiLegacyBootMarkerGuid;
extern EFI_GUID gEfiWindowsInt10Workaround;
extern EFI_GUID gEfiUsbLegacyStack;

#endif
