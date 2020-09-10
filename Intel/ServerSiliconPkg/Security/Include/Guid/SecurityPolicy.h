/** @file
  Provides data structure information used by ServerSecurity features in Mtkme etc.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef _SECURITY_POLICY_H_
#define _SECURITY_POLICY_H_

extern EFI_GUID gSecurityPolicyDataGuid;
#include <Library/SecurityPolicyDefinitions.h>

#pragma pack(1)

typedef struct {
  /**
   * Please put common definitions inside the SecurityPolicy_Flat.h *
   Header is flat and injected directly in SecurityPolicy sructuture and SOCKET_PROCESSORCORE_CONFIGURATION.
   There is a possibility to use the definition via hierarchical structures as shown in SecurityPolicyDefinitions.h
   SECURITY_POLICY_HII_NO_PRAGMA_PACK structure as VFR compile won't recognize 'pragma'.
   XmlCli will accept embeded structures but references them with '_' /underscore/ rather than '.' /dot/.
  **/
  #include "SecurityPolicy_Flat.h"
} SECURITY_POLICY;

#pragma pack()
#endif
