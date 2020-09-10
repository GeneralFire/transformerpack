/** @file
MKTME Late Init

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

#ifndef _MKTME_LATE_INIT_H_
#define _MKTME_LATE_INIT_H_

#include <Library/UefiDriverEntryPoint.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/MpService.h>
#include <Register/ArchitecturalMsr.h>
#include <Cpu/CpuCoreRegs.h>
#include <Guid/SgxInitData.h>
#include <SysHost.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/VariableLock.h>
//APTIOV_SERVER_OVERRIDE_RC_START: Remove header in EDKII SecurityPkg
//#include <Guid/PhysicalPresenceData.h>
//APTIOV_SERVER_OVERRIDE_RC_END: Remove header in EDKII SecurityPkg
#include <Register/Cpuid.h>
#include <Library/SecurityPolicyLib.h>
#include <Library/TpmMeasurementLib.h>
#include <IndustryStandard/UefiTcgPlatform.h>

#define MSR_TME_KEY0                    0x992
#define MSR_TME_KEY1                    0x993

// define mktme_tpm strings
#define FEATURE_ENABLE_TME_STRING     "FeatureTME=1"
#define FEATURE_DISABLE_TME_STRING    "FeatureTME=0"
#define FEATURE_ENABLE_MKTME_STRING   "FeatureMKT=1"
#define FEATURE_DISABLE_MKTME_STRING  "FeatureMKT=0"

#define MSR_TME_ACTIVATE_STRING       "MSR_TME_ACTIVATE"
#define MSR_TME_CAPABILITY_STRING     "MSR_TME_CAPABILITY"
#define MSR_TME_EXCLUDE_BASE_STRING   "MSR_TME_EXCLUDE_BASE"
#define MSR_TME_EXCLUDE_MASK_STRING   "MSR_TME_EXCLUDE_MASK"

#pragma pack(1)
typedef struct {
  struct {
    UINT64 Key0;
    UINT64 Key1;
  }
  TmeKey[MAX_SOCKET];
} EFI_TMEKEY;

typedef struct _EFI_MKTME_EVENT_DATA{
  VOID  *MsrName;
  UINT64 MsrContent;
} EFI_MKTME_EVENT_DATA;

#pragma pack()

#endif
