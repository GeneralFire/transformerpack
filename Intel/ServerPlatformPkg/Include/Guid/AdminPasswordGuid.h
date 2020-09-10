/** @file
  GUIDs used for create HOB for prevent admin password resetting
  during CMOS clear.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2013 Intel Corporation. <BR>

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

#ifndef _ADMIN_PASSWORD_HOB_GUID_H_
#define _ADMIN_PASSWORD_HOB_GUID_H_

//
// Definitions for TCG-Defined Wake Event Data
//
#define EFI_ADMIN_PASSWORD_DATA_HOB_GUID \
    { 0x4E173307, 0x92DB, 0x4a4f, {0x9A, 0x8E, 0x86, 0xA8, 0x56, 0x49, 0xB4, 0x05 }}

extern EFI_GUID gEfiAdminPasswordHobGuid;

#endif // _ADMIN_PASSWORD_HOB_GUID_H_
