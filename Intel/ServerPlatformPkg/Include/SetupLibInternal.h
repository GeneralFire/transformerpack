/** @file
  This library abstracts read/write access for basic type data those values may be
  stored into the different media.

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

#ifndef _SETUP_LIB_INTERNAL_H_
#define _SETUP_LIB_INTERNAL_H_

#include <Library/SetupLib.h>
GROUP_INFO mSetupInfo[] = {
  {&gEfiSocketIioVariableGuid,                     SOCKET_IIO_CONFIGURATION_NAME,               sizeof(SOCKET_IIO_CONFIGURATION)},
  {&gEfiSocketCommonRcVariableGuid,                SOCKET_COMMONRC_CONFIGURATION_NAME,          sizeof(SOCKET_COMMONRC_CONFIGURATION)},
  {&gEfiSocketMpLinkVariableGuid,                  SOCKET_MP_LINK_CONFIGURATION_NAME,           sizeof(SOCKET_MP_LINK_CONFIGURATION)},
  {&gEfiSocketMemoryVariableGuid,                  SOCKET_MEMORY_CONFIGURATION_NAME,            sizeof(SOCKET_MEMORY_CONFIGURATION)},
  {&gEfiSocketPowermanagementVarGuid,              SOCKET_POWERMANAGEMENT_CONFIGURATION_NAME,   sizeof(SOCKET_POWERMANAGEMENT_CONFIGURATION)},
  {&gEfiSocketProcessorCoreVarGuid,                SOCKET_PROCESSORCORE_CONFIGURATION_NAME,     sizeof(SOCKET_PROCESSORCORE_CONFIGURATION)},
  {&gEfiSetupVariableGuid,                         PLATFORM_SETUP_VARIABLE_NAME,                sizeof(SYSTEM_CONFIGURATION)},
  {&gPchSetupVariableGuid,                         PCH_SETUP_NAME,                              sizeof(PCH_SETUP)},
  {&gMemBootHealthGuid,                            MEM_BOOT_HEALTH_SETUP_STR,                   sizeof(MEM_BOOT_HEALTH_CONFIG)},
#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
  {&gEfiMeRcVariableGuid,                          ME_RC_CONFIGURATION_NAME,                    sizeof(ME_RC_CONFIGURATION)},
#endif // ME_SUPPORT_FLAG
#ifdef IE_SUPPORT
  {&gEfiIeRcVariableGuid,                          IE_RC_CONFIGURATION_NAME,                    sizeof(IE_RC_CONFIGURATION)},
#endif //IE_SUPPORT
#if !defined(SNR_HOST) && !defined(ICXDE_HOST) && !defined(SPR_HOST)  && !defined(EBG_SC_FLAG)
  {&gFpgaSocketVariableGuid,                       FPGA_SOCKET_CONFIGURATION_NAME,              sizeof(FPGA_SOCKET_CONFIGURATION)},
#endif  // !defined(SNR_HOST) && !defined(ICXDE_HOST) && !defined(SPR_HOST)
  {NULL,                                           NULL}
};

#endif
