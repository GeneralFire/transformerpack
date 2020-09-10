/** @file
  Defines a Tag GUID used to mark a UEFI legacy BIOS thunk driver based
  on legacy BIOS services and legacy option ROM. This Tag GUID must be installed on
  the ImageHandle of any module that follows the EFI Driver Model and uses
  the Int86() or FarCall() services of the Legacy Bios Protocol to produce
  a standard UEFI I/O Protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2010 Intel Corporation. <BR>

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

#ifndef _LEGACY_BIOS_H_
#define _LEGACY_BIOS_H_

///
/// The Global ID for the Legacy BIOS GUID that must be installed onto the ImageHandle
/// of any module follows the EFI Driver Model and uses the Int86() or FarCall()
/// services of the Legacy BIOS Protocol to produce a standard UEFI I/O Protocol.
///
#define EFI_LEGACY_BIOS_GUID \
  { \
    0x2e3044ac, 0x879f, 0x490f, {0x97, 0x60, 0xbb, 0xdf, 0xaf, 0x69, 0x5f, 0x50 } \
  }

extern EFI_GUID gEfiLegacyBiosGuid;

#endif
