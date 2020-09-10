/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2008 - 2013 Intel Corporation. <BR>

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

#ifndef _MEMORY_RAS_H_
#define _MEMORY_RAS_H_

#include "RasInit.h"
#include "MemRas.h"

#define RAS_DBG_LEVEL(x)                (1 << (x))

#define TIMER_RATE                      2

#define LEGACY_REGION                   BIT0
#define TSEG_REGION                     BIT1

#define ALL_SOCKETS                     (UINT8)(-1)
#define ALL_BRANCHES                    (UINT8)(-1)
#define REQ_INVALID                     0xFF

//
// Mirror operation codes
//
#define MIRROR_OPCODE_OFFLINE           0x0
#define MIRROR_OPCODE_ONLINE            0x1

#define MEM_RAS_FLAG_NODE_IDX0          (0x00FF)
#define MEM_RAS_FLAG_NODE_IDX1          (0xFF00)
#define MEM_RAS_FLAG_OPCODE             (BIT24|BIT25)
  #define MEM_RAS_FLAG_NODE_OFF         3
  #define MEM_RAS_FLAG_NODE_ON          2
#define MEM_RAS_FLAG_MPST_REQ           BIT26
#define MEM_RAS_FLAG_OS_IDX             BIT27
#define MEM_RAS_FLAG_KEEP_PWRON         BIT28
#define MEM_RAS_FLAG_NO_OS_SCI          BIT29
#define MEM_RAS_FLAG_RISER_PAIR         BIT30
#define MEM_RAS_FLAG_NO_OS_CORP         BIT31

#define CACHE_LINE_SIZE                 64

#define RAS_NO_ERROR                    0
#define RAS_GENERAL_ERROR               1000

#define RAS_MEM_ERR_ONLINE              1000
#define RAS_MEM_ERR_OFFLINE             2000
#define RAS_MEM_ERR_UPDATE              4000

#define RAS_MEM_ONOFFLINE_INVALID_SETUP    1010
#define RAS_MEM_ONOFFLINE_INVALID_REQUEST  1011
#define RAS_MEM_ONOFFLINE_NO_REQUEST       1012
#define RAS_MEM_ONOFFLINE_REDIRECT         1013

#define RAS_MEM_ERR_COPY                2000

#define RAS_MEM_ERR_NOTIFY_OS           200
#define RAS_MEM_ERR_EXECUTE_REQUEST     201
#define RAS_MEM_ERR_GET_STATE           202

#define CA0_CR_C_PCSR_BIOSEN            0xFEB240D8
#define CA0_CR_C_PCSR_CSEGEN            0xFEB240DC

#define RAS_MIGRATION_WB_SIZE_PER_SMI   0x400000    // 4M  for WB
#define RAS_MIGRATION_UC_SIZE_PER_SMI   0x010000    // 64K  for UC

#define RAS_PAGETABLE_SIZE              0x100000    // 1MB page table for migratoin

#define ADDRESS_4GB                     0x100000000
#define ADDRESS_4TB                     0x40000000000
#define MASK_4KB                        ((UINT64)0xFFF)
#define MASK_2MB                        ((UINT64)0x1FFFFF)
#define MASK_1GB                        ((UINT64)0x3FFFFFFF)

#define DYMANIC_2MB_PAGE_IDX            16
#define POISON_LOG_PAGE_IDX             8

#define MAX_POISON_LOG_ENTRIES          64

typedef struct {
  UINT8  SrcNode;
  UINT8  DstNode;
  UINT8  Mask;
} MIRROR_REPLACE_INFO;

typedef struct {
  SPIN_LOCK ApBusy;
  PSYSHOST Host;
  UINT8 Node;
  UINT8 *MemHpStatus;
  UINT8 Action;
} MEM_ONLINE_PARAMS;

typedef enum {
  EnumUnchanged,
  EnumOnline,
  EnumOffline
} ONLINE_OFFLINE_STATE;

typedef enum {
  EnumMemoryEntry,
  EnumMemoryCheckRequest,
  EnumMemoryGetState,
  EnumMemoryExecuteRequest,
  EnumMemoryCleanup,
  EnumMemoryNotifyOS,
  EnumMemoryExit,

  EnumOsNotifyInit,
  EnumOsNotifyWait,
  EnumOsNotifyDone,

  EnumMemoryLibCallInit,
  EnumMemoryLibCallExecute,
  EnumMemoryLibCallDone,

  EnumMemoryCopyEntry,
  EnumMemoryCopyInit,
  EnumMemoryCopyExecute,
  EnumMemoryCopyTsegRegion,
  EnumMemoryCopySpecialRegion,
  EnumMemoryCopyDone,

  EnumMemoryOnlineOfflineEntry,
  EnumMemorySwitchBranch,
  EnumMemoryOnlining,
  EnumMemoryUpdate,
  EnumMemoryOfflining,
  EnumMemoryPowerOff,
  EnumMemoryOnlineOfflineDone,

  EnumSpareMigrationEntry,
  EnumSpareMigrationDestOnline,
  EnumSpareMigrationStart,
  EnumSpareMigrationSetup,
  EnumSpareMigrationMemoryCopy,
  EnumSpareMigrationDisable,
  EnumSpareMigrationSrcOffline,
  EnumSpareMigrationUpdate,
  EnumSpareMigrationPowerOff,
  EnumSpareMigrationDone,

  EnumMirrorMigrationEntry,
  EnumMirrorMigrateToSpareNode,
  EnumMirrorMigrateFromSpareNode,
  EnumMirrorMigrationDone

} INTERNAL_STATE;

EFI_STATUS
BuildPageTableBelow4G (
  VOID
);

#define IA32_PG_P                   1u
#define IA32_PG_RW                  (1u << 1)
#define IA32_PG_USR                 (1u << 2)
#define IA32_PG_WT                  (1u << 3)
#define IA32_PG_CD                  (1u << 4)
#define IA32_PG_A                   (1u << 5)
#define IA32_PG_D                   (1u << 6)
#define IA32_PG_PS                  (1u << 7)
#define IA32_PG_G                   (1u << 8)
#define IA32_PG_AVL1                (7u << 9)
#define IA32_PG_PAT_2M              (1u << 12)
#define IA32_PG_PAT_4K              IA32_PG_PS

#endif
