/** @file
  Contains definitions relevant to the Processor Error Handler.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2020 Intel Corporation. <BR>

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

#ifndef _CORE_ERROR_HANDLER_H_
#define _CORE_ERROR_HANDLER_H_

#include <PiSmm.h>

#include <IndustryStandard/Acpi.h>

#include <Emca.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SmmMemLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/ResetSystemLib.h>
#include <Library/SmmCpuPlatformHookLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Library/ProcessorRasLib.h>
#include <Library/MpExtensionLib.h>
#include <Library/RasDebugLib.h>
#include <Library/PlatMemErrorDataLib.h>
#include <Library/ErrorReportLib.h>
#include <Library/EmcaRecoveryLib.h>
#include <Library/PlatPolicyLib.h>
#include <Library/RasSiliconLib.h>
#include <Library/IehRasLib.h>
#include <Protocol/SmmCpuService.h>
#include <Register/Msr.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/WorkaroundLib.h>
#include <Library/OscRasLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Include/UncoreCommonIncludes.h>

/**
  MCA handler for SMM use.

**/
VOID
EFIAPI
SmiMcaHandler (
  IN EFI_EXCEPTION_TYPE   InterruptType,
  IN EFI_SYSTEM_CONTEXT   SystemContext
  );

typedef struct _PROCESSOR_RAS_CAPABILITY  PROCESSOR_RAS_CAPABILITY;

struct _PROCESSOR_RAS_CAPABILITY {
  BOOLEAN      EmcaGen1Cap;
  BOOLEAN      EmcaGen2Cap;
  BOOLEAN      LmceCap;
};

typedef struct _PROCESSOR_RAS_SETUP PROCESSOR_RAS_SETUP;

struct _PROCESSOR_RAS_SETUP {
  BOOLEAN EmcaEn;
  UINT8   CsmiEn;
  UINT8   CsmiDynamicDis;
  UINT16  CsmiThreshold;
  UINT8   MsmiEn;
  UINT8   IoMcaEn;
  BOOLEAN LmceEn;
  BOOLEAN CloakingEn;
  BOOLEAN PoisonEn;
  BOOLEAN SystemErrorEn;
  BOOLEAN FatalErrSpinLoopEn;
  UINT32  MsmiBankBitFieldEn;
  UINT32  CsmiBankBitFieldEn;
  UINT8   OscEn;
  BOOLEAN McaSpinLoop;
  UINT8   UboxErrorMask;
};


#define SEVERITY_MAP_FATAL        2
#define SEVERITY_MAP_RECOVERABLE  1
#define SEVERITY_MAP_CORRECTED    0
#define MAX_ERROR_SEVERITY        3

typedef enum {
  SmmMceClearedState = 0,
  SmmMceReadingState,
  SmmMceProcessingState,
  SmmMceWritingState,
  SmmMceLoggingState,
  SmmMceReadingInterrupted,
  SmmMceWritingInterrupted,
  SmmMceProcessingInterrupted,
} SMM_MCE_BANK_SYNC_STATE;


typedef struct {
  UINT32 BankIdx;
  UINT32 ApicId;
} SMM_ERROR_EVENT;

typedef struct {
  BOOLEAN ErrorsFound;
  UINT32 SeverityMap;
  SMM_ERROR_EVENT Log[MAX_ERROR_SEVERITY];
} SMM_ERROR_EVENT_LOG;

typedef struct {
  volatile UINT64 CoreProcessingBank;
  volatile UINT64 UncoreProcessingBank;
  volatile UINT64 SmmMceSyncState;
  volatile SMM_ERROR_EVENT_LOG EventLog;
  volatile UINT8 CmciMcBankErrCount[MCBANK_MAX];
} SMM_ERROR_CPU_DATA_BLOCK;


extern  SMM_ERROR_CPU_DATA_BLOCK                *gSmmErrorCpuData;

typedef struct {
  UINT32                    Package;
  UINTN                     CpuIndex;
} SMM_MCA_CPU_INFO;

#endif
