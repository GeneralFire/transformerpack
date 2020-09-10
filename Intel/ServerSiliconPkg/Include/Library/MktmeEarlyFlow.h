/** @file
MKTME Early flow

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

#ifndef _MKTME_EARLY_INIT_H_
#define _MKTME_EARLY_INIT_H_

#include <Register/Cpuid.h>
#include <Base.h>
#include <PiPei.h>
#include <Uefi.h>
#include <Uefi/UefiBaseType.h>
#include <Ppi/MpServices.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PcdLib.h>
#include <Library/KtiApi.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/MemoryCoreLib.h>
#include <Library/SystemInfoLib.h>
#include <Guid/MktmeInitData.h>
#include <Guid/MemoryMapData.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/BaseLibFcCrb/BaseLibInternals.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemMapDataLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/SiliconWorkaroundLib.h>

/**

Reports whether TME capability is present in the system or not. Thread safe.

@retval TRUE  - TME capability is present in the system
        FALSE - TME capability is NOT present in the system
**/
BOOLEAN
ExtIsTmeCapSupported ();

MKTME_INIT_DATA_HOB *
GetMkTmeInitHob ();

/**
This is SPR implementation only, do not use for other projects
**/
UINT32
EFIAPI
IsTmeCapSupported (IN PSYSHOST Host);

UINT32
EFIAPI
MktmeEarlyFlow (IN PSYSHOST Host);

/**

Entry point for MKTME Early Init

@retval Status

**/
UINT32
EFIAPI
TmeEarlyFlow (IN PSYSHOST Host);

UINT32
EFIAPI
MkTmeEarlyFlowEntry (IN PSYSHOST Host);

UINT32
EFIAPI
TmeInitFlow (IN PSYSHOST Host);

#endif // _MKTME_EARLY_INIT_H_
