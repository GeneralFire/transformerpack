/** @file
  Silicon Policy PPI is used for specifying platform
  related Intel silicon information and policy setting.
  This PPI is consumed by the silicon PEI modules and carried
  over to silicon DXE modules.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2020 Intel Corporation.

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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

#ifndef _SI_POLICY_PPI_H_
#define _SI_POLICY_PPI_H_

#include <SiPolicyStruct.h>
#include <PchPolicyCommon.h>
#include <PchPreMemPolicyCommon.h>
#include <MePolicyCommon.h>
#include <CpuPolicyCommon.h>
#include <AmtConfig.h>
#include <FusaConfig.h>
#include <Uefi.h>

#ifndef DISABLED
#define DISABLED  0
#endif
#ifndef ENABLED
#define ENABLED   1
#endif

extern EFI_GUID gSiPreMemPolicyPpiGuid;
extern EFI_GUID gSiPolicyPpiGuid;


#include <GraphicsConfig.h>
extern EFI_GUID gGraphicsPeiPreMemConfigGuid;
extern EFI_GUID gGraphicsPeiConfigGuid;

#include <VtdConfig.h>
extern EFI_GUID gVtdConfigGuid;

#include <GnaConfig.h>
extern EFI_GUID gGnaConfigGuid;



#include <CpuPcieConfigGen3.h>
#include <CpuPcieConfig.h>
extern EFI_GUID gCpuPciePeiPreMemConfigGuid;
extern EFI_GUID gCpuPcieRpConfigGuid;

#include <HybridGraphicsConfig.h>
extern EFI_GUID gHybridGraphicsConfigGuid;

#include <ConfigBlock/PramPreMemConfig.h>
#include <MemoryConfig.h>
extern EFI_GUID gMemoryConfigGuid;
extern EFI_GUID gMemoryConfigNoCrcGuid;

#include <ConfigBlock/SaMiscPeiPreMemConfig.h>
extern EFI_GUID gSaMiscPeiPreMemConfigGuid;

#include <ConfigBlock/SaMiscPeiConfig.h>
extern EFI_GUID gSaMiscPeiConfigGuid;


#include <TraceHubConfig.h>
extern EFI_GUID gCpuTraceHubConfigGuid;

#include <HostBridgeConfig.h>
extern EFI_GUID gHostBridgePeiPreMemConfigGuid;
extern EFI_GUID gHostBridgePeiConfigGuid;

#include <CpuDmiPreMemConfig.h>
extern EFI_GUID gCpuDmiPreMemConfigGuid;

#include <VpuConfig.h>
extern EFI_GUID gVpuPeiConfigGuid;

typedef struct _SI_PREMEM_POLICY_STRUCT SI_PREMEM_POLICY_PPI;
typedef struct _SI_POLICY_STRUCT SI_POLICY_PPI;

#endif // _SI_POLICY_PPI_H_
