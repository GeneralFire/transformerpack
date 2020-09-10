/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#ifndef __SSA_BIOS_H__
#define __SSA_BIOS_H__

#undef _DEBUG // to avoid warning C4005: '_DEBUG' : macro redefinition (see .\EdkCompatibilityPkg\Foundation\Library\EdkIIGlueLib\Include\Library\EdkIIGlueDebugLib.h line #238)

#ifndef SUPPORT_FOR_RDIMM
#define SUPPORT_FOR_RDIMM (1)
#endif // SUPPORT_FOR_RDIMM

#ifndef SUPPORT_FOR_EXTENDED_RANGE
#define SUPPORT_FOR_EXTENDED_RANGE (1)
#endif

#ifdef LRDIMM_SUPPORT
#define SUPPORT_FOR_LRDIMM (1)
#else
#define SUPPORT_FOR_LRDIMM (0)
#endif // LRDIMM_SUPPORT

#ifdef DDRT_SUPPORT
#define SUPPORT_FOR_DDRT (1)
#else
#define SUPPORT_FOR_DDRT (0)
#endif // DDRT_SUPPORT


#include "SysHost.h"

#include <Ppi/SsaBiosServicesPpi.h>
#include <Library/RcDebugLib.h>

#define BIOS_SSA_VER_MAJOR    1
#define BIOS_SSA_VER_MINOR    0
#define BIOS_SSA_VER_REVISION 0

typedef
EFI_STATUS
(EFIAPI * BIOS_SSA_TEST_ENTRY_POINT) (
  SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  VOID                  *Config
);

#endif
