/** @file
  Definitions for SPS ME-BIOS support in DXE.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2020 Intel Corporation. <BR>

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

#ifndef _SPSDXE_H_
#define _SPSDXE_H_

#include <ConfigBlock/Sps/SpsDxeConfig.h>

//
// Data shared between SPS DXE files.
//
typedef struct
{
  BOOLEAN                 IsDualBiosWatchdogRunning;
  SPS_INFO_HOB            *pSpsHob;
  SPS_DXE_CONFIG          *pSpsPolicy;
} SPS_DXE_CONTEXT;

VOID
EFIAPI
SpsEndOfDxeCallback (IN EFI_EVENT Event, IN VOID *pContext);

VOID
EFIAPI
SpsReadyToBootCallback (IN EFI_EVENT Event, IN VOID *pContext);

EFI_STATUS SpsNmSendHostCfgInfo (SPS_DXE_CONTEXT*);

VOID
EFIAPI
DfxReadyToBootCallback (
  IN EFI_EVENT      Event,
  IN VOID           *Context
  );

// Part of private interface PmcPrivateLib.h
/**
  This function enables triggering Global Reset of both
  the Host and the ME partitions after CF9h write of 6h or Eh
**/
VOID
PmcEnableCf9GlobalReset (
  VOID
  );

/**
  This function disables triggering Global Reset of both
  the Host and the ME partitions after CF9h write of 6h or Eh.
  Global Reset configuration is locked after programming
**/
VOID
PmcDisableCf9GlobalResetWithLock (
  VOID
  );

#endif // _SPSDXE_H_

