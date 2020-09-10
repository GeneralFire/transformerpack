/** @file
  eADR SMM driver private header file.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

  Unless otherwise agreed by Intel in writing, you may not remove or alter  this notice or any other notice embedded in Materials by Intel or
  Intel's suppliers or licensors in any way.
**/

#ifndef _FADR_H_
#define _FADR_H_

#include <PiSmm.h>
#include <RcRegs.h>
#include <SysHost.h>
#include <MemHostChipCommon.h>
#include <MaxSocket.h>
#include <Register/ArchitecturalMsr.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/BaseLib.h>
#include <Library/CpuLib.h>
#include <Library/LocalApicLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/IoLib.h>
#include <Library/PcuIpLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/MemMapDataLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/TimerLib.h>
#include <Library/CpuPpmLib.h>
#include <Library/MemTypeLib.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/SmmSwDispatch2.h>
#include <Protocol/SmmSxDispatch2.h>
#include <Protocol/SmmCpuService.h>
#include <Protocol/SmmIoTrapDispatch2.h>
#include <Protocol/PchSmmIoTrapControl.h>
#include <Protocol/CrystalRidge.h>
#include <Mem/Library/MemMcIpLib/Include/MemMcRegs.h>

typedef struct FADR_SMI_DATA {
  EFI_HANDLE ResetHandle;
  EFI_HANDLE S3EntryHandle;
  EFI_HANDLE S4EntryHandle;
  EFI_HANDLE S5EntryHandle;
} FADR_SMI_DATA;

typedef struct _FADR_DATA {
  FADR_SMI_DATA     Smi;
} FADR_DATA;

typedef struct _FADR_TASK_DATA {
  UINTN NumOfScheduled;
  UINTN NumOfCompleted;
} FADR_TASK_DATA;

#define FADR_DEBUG(...) do { \
            AcquireSpinLock (&mFadrDbgMsgLock); \
            DEBUG (__VA_ARGS__); \
            ReleaseSpinLock (&mFadrDbgMsgLock); \
            } while (0)

#define FADR_TIME_NS_TO_US(Time) ((Time) / 1000)

typedef enum _FADR_FLOWS {
  FADR_PCU_ASSISTED_IIO_CPU_CACHE_FLUSH,
  FADR_RESET_CPU_CACHE_FLUSH,
  FADR_S3_ENTRY_CPU_CACHE_FLUSH,
  FADR_S4_ENTRY_CPU_CACHE_FLUSH,
  FADR_S5_ENTRY_CPU_CACHE_FLUSH,
  FADR_FLOWS_MAX
} FADR_FLOWS;

#define FADR_ALL_FLOWS_DISABLED 0

#define FADR_ALL_FLOWS_ENABLED \
  ((UINT8)((BIT0 << FADR_PCU_ASSISTED_IIO_CPU_CACHE_FLUSH) | (BIT0 << FADR_RESET_CPU_CACHE_FLUSH) | \
           (BIT0 << FADR_S3_ENTRY_CPU_CACHE_FLUSH) | (BIT0 << FADR_S4_ENTRY_CPU_CACHE_FLUSH) | \
           (BIT0 << FADR_S5_ENTRY_CPU_CACHE_FLUSH)))

//
// fADR stages (not all are used by fADR SMM driver)
//
typedef enum _FADR_STAGE {
  FADR_STAGE_0_ADR_ONLY,
  FADR_STAGE_1_PHASE_1_START,
  FADR_STAGE_2_RESERVED,
  FADR_STAGE_3_CPU_PIPE_FLUSH_START,
  FADR_STAGE_4_RESERVED,
  FADR_STAGE_5_IIO_FLUSH_START,
  FADR_STAGE_6_RESERVED,
  FADR_STAGE_7_CPU_CACHE_FLUSH_START,
  FADR_STAGE_8_RESERVED,
  FADR_STAGE_9_PHASE_1_END,
  FADR_STAGE_10_AD_WB_NM_FLUSH_START,
  FADR_STAGE_11_RESERVED,
  FADR_STAGE_12_RESERVED,
  FADR_STAGE_13_RESERVED,
  FADR_STAGE_14_RESERVED,
  FADR_STAGE_15_EXTENDED_FLUSH_COMPLETION
} FADR_STAGE;

#define FadrIsStageSupported(Stage) \
  (((Stage) == FADR_STAGE_1_PHASE_1_START) || \
   ((Stage) == FADR_STAGE_7_CPU_CACHE_FLUSH_START) || \
   ((Stage) == FADR_STAGE_15_EXTENDED_FLUSH_COMPLETION))

#define FADR_CPU_CACHE_FLUSH_HALT         0
#define FADR_CPU_CACHE_FLUSH_RUN_BSP      1
#define FADR_CPU_CACHE_FLUSH_RUN_NON_BSP  2

#define FADR_PCH_IO_CF9_TRAP_ADDRESS    0xCF9
#define FADR_PCH_IO_CF9_TRAP_LENGTH     0x1
#define FADR_PCH_IO_CF9_TRAP_WARM_RESET 0x6
#define FADR_PCH_IO_CF9_TRAP_COLD_RESET 0xE

#define FADR_PCH_IO_CF9_TRAP_IO_OPS_CNT 1

#define FADR_PCH_IO_CF9_RESET_TRAP_SHIFT 8

#define FadrIsResetTypeSupported(ResetType) \
  (((ResetType) == FADR_PCH_IO_CF9_TRAP_COLD_RESET) || \
   ((ResetType) == FADR_PCH_IO_CF9_TRAP_WARM_RESET))

#define FadrIsSxTypeEntrySupported(SxType) \
  (((SxType) >= SxS3) && ((SxType) <=SxS5))

#define FADR_SUBSYSTEM_DEV_ID_CWV 0x97C

#endif  // _FADR_H_
