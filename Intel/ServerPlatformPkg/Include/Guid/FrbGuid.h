/** @file
  GUID used for IPMI HOB entries in the HOB list.

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

#ifndef _FRB_GUID_H_
#define _FRB_GUID_H_

//
// FRB Common Structures
//
typedef enum {
  EfiFrbReserved,
  EfiFrb2,
  EfiBiosPost,
  EfiOsBootWdt,
  EfiFrbReserved1,
  EfiFrbReserved2,
  EfiFrbReserved3,
  EfiFrbReserved4,
  EfiFrb1NotSupported,
  EfiFrb3NotSupported,
  EfiFrbReservedLast
} EFI_FRB_TYPE;

typedef enum {
  NoTimeoutInterrupt,
  SystemManagementInterrupt,
  DiagnosticInterrupt,
  MessagingInterrupt
} FRB_SYSTEM_TIMEOUT_INTERRUPT;

typedef enum {
  EfiNormalBoot,
  EfiHardReset,
  EfiPowerDown,
  EfiPowerCycle,
  EfiFrbSysReserved1,
  EfiFrbSysReserved2,
  EfiFrbSysReserved3,
  EfiFrbSysReserved4,
  EfiDiagnosticMode,
  EfiPlatformRecovery
} FRB_SYSTEM_BOOT_POLICY;

#endif
