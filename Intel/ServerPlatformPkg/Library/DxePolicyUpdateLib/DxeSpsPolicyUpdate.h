/** @file

@copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains a 'Sample Driver' and is licensed as such under the terms
  of your license agreement with Intel or your vendor. This file may be modified
  by the user, subject to the additional terms of the license agreement.

@par Specification Reference:
**/

#ifndef _DXE_SPS_POLICY_UPDATE_H_
#define _DXE_SPS_POLICY_UPDATE_H_

#include <PiDxe.h>
#include <SetupVariable.h>
#include <Guid/MeRcVariable.h>
#include <Library/SetupLib.h>
#include <Library/MeTypeLib.h>
#include <HeciRegs.h>
#include <Setup/MeSetup.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Guid/EventGroup.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/HeciProtocol.h>
#include <Protocol/SpsPolicyProtocol.h>
#include <Library/HobLib.h>
#include <Library/ConfigBlockLib.h>
#include <ConfigBlock/SpsPeiConfig.h>
#include <Library/DxeMeLib.h>


/**
  Update SPS Policy if Setup variable exists.

  @param[in, out] SpsPolicyInstance     Instance of SPS Policy Protocol

**/
VOID
UpdateSpsPolicyFromSetup (
  IN OUT SPS_POLICY_PROTOCOL     *SpsPolicyInstance
  );

/**
  Functions performs HECI exchange with FW to update SpsPolicy settings.

  @param[in] Event         A pointer to the Event that triggered the callback.
  @param[in] Context       A pointer to private data registered with the callback function.

**/
VOID
EFIAPI
UpdateSpsSetupCallback (
  IN  EFI_EVENT                       Event,
  IN  VOID                            *Context
  );

#endif
