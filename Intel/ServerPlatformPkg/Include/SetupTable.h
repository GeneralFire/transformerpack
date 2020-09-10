/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2019 Intel Corporation. <BR>

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

#ifndef _SETUP_TABLE_H_
#define _SETUP_TABLE_H_
#include <Guid/SocketVariable.h>
#include <Guid/SetupVariable.h>
#if ME_SUPPORT_FLAG
#include <Guid/MeRcVariable.h>
#endif // ME_SUPPORT_FLAG

#ifdef IE_SUPPORT
#include <Guid/IeRcVariable.h>
#endif //IE_SUPPORT

#include <PchSetupVariable.h>
#include <Guid/FpgaSocketVariable.h>
#include <Guid/MemBootHealthGuid.h>

typedef struct {
  SOCKET_CONFIGURATION       SocketConfig;
  SYSTEM_CONFIGURATION       SystemConfig;
  PCH_SETUP                  PchSetup;
  MEM_BOOT_HEALTH_CONFIG     MemBootHealthConfig;
#if ME_SUPPORT_FLAG
  ME_RC_CONFIGURATION        MeRcConfig;
#endif // ME_SUPPORT_FLAG
#ifdef IE_SUPPORT
  IE_RC_CONFIGURATION        IeRcConfig;
#endif //IE_SUPPORT
#if !defined(SNR_HOST) && !defined(ICXDE_HOST) && !defined(SPR_HOST) && !defined(EBG_SC_FLAG)
  FPGA_SOCKET_CONFIGURATION  SocketFpgaConfig;
#endif  // !defined(SNR_HOST) && !defined(ICXDE_HOST) && !defined(SPR_HOST)
// APTIOV_SERVER_OVERRIDE_RC_START: To solve structure redefinition error in AMI.
} INTEL_SETUP_DATA;
// APTIOV_SERVER_OVERRIDE_RC_END: To solve structure redefinition error in AMI.

#endif
