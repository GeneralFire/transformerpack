/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#ifndef _SYS_CONFIG_SETUP_H_
#define _SYS_CONFIG_SETUP_H_

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/DebugLib.h>
#include <Guid/SetupVariable.h>
#include <Guid/SocketVariable.h>
#if ME_SUPPORT_FLAG
#include <Guid/MeRcVariable.h>
#endif // ME_SUPPORT_FLAG

#ifdef IE_SUPPORT
#include <Guid/IeRcVariable.h>
#endif //IE_SUPPORT
#include <PchSetupVariable.h>

EFI_STATUS
EFIAPI
SetupSysConfig (
    IN EFI_PEI_SERVICES     **PeiServices,
#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
    IN OUT   ME_RC_CONFIGURATION                     *MeRcConfiguration,
#endif // ME_SUPPORT_FLAG
#ifdef IE_SUPPORT
    IN OUT   IE_RC_CONFIGURATION                     *IeRcConfiguration,
#endif // IE_SUPPORT
    IN OUT   SYSTEM_CONFIGURATION  *Setup,
    IN OUT   PCH_SETUP  *PchSetup,
    IN OUT   SOCKET_IIO_CONFIGURATION                *SocketIioConfig,
    IN OUT   SOCKET_COMMONRC_CONFIGURATION           *SocketCommonRcConfig,
    IN OUT   SOCKET_MP_LINK_CONFIGURATION            *SocketMpLinkConfig,
    IN OUT   SOCKET_MEMORY_CONFIGURATION             *SocketMemoryConfig,
    IN OUT   SOCKET_POWERMANAGEMENT_CONFIGURATION    *SocketPowerManagementConfig,
    IN OUT   SOCKET_PROCESSORCORE_CONFIGURATION      *SocketProcessorCoreConfig
)
  /*++

Routine Description:

  Setup SystemConfiguration structure with default values.

Arguments:

  PeiServices:         PEI Services Table.
  SYSTEM_CONFIGURATION: Pointer to structure to be initialised.
  PCH_RC_CONFIGURATION: Pointer to structure to be initialised.
  TODO: Update params

Returns:

  !EFI_SUCCESS      - if any failure
  EFI_SUCCESS       - Successful

--*/
;

#endif
