/** @file
  Mem Ras ProtocolCode

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2017 Intel Corporation. <BR>

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

#ifndef _MEM_RAS_PROTOCOL_H_
#define _MEM_RAS_PROTOCOL_H_

#include <Guid/MemoryMapData.h>
#include <Library/MemTypeLib.h>

#define EFI_MEM_RAS_PROTOCOL_GUID \
  {0x6d7e4a32, 0x9a73, 0x46ba, 0x94, 0xa1, 0x5f, 0x2f, 0x25, 0xef, 0x3e, 0x29}

#define EFI_FORWARD_DECLARATION(x) typedef struct _##x x
EFI_FORWARD_DECLARATION (EFI_MEM_RAS_PROTOCOL);

/* #define  RAS_DEBUG(x)   do { \
  if (mRasFlags & RAS_FLAG_RAS_DEBUG) { \
  EfiDebugPrint x; \
  } while (0) */
// APTIOV_SERVER_OVERRIDE_RC_START : Disabled RAS debug messages in Release mode
#ifdef EFI_DEBUG
#define  RAS_DEBUG(x)  DebugPrint x
#else
#define  RAS_DEBUG(x)
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : Disabled RAS debug messages in Release mode
extern UINT64 mRasFlags;

// BIT0-BIT31 will be checked by MRC code
#define  RAS_FLAG_KEEP_POWERON                BIT0
#define  RAS_FLAG_INJ_ECC                     BIT30
#define  RAS_FLAG_RESERVE                     BIT31   // please avoid use this to avoid sign extension

// BIT32-BIT61 will be consumed by RAS only
#define  RAS_FLAG_MRC_DEBUG                   BIT63
#define  RAS_FLAG_TIME_SLICING                BIT62
#define  RAS_FLAG_RAS_DEBUG                   BIT61
#define  RAS_FLAG_QUIESCE_MIGRATION           BIT60


#define  RAS_PARAM_MEASURE_DATA               0
#define  RAS_PARAM_QUICSES_DATA               1

#define  SAVE_DISABLE_CKE_LOW                 1
#define  RESTORE_CKE_LOW                      2



#define PARAM_UNUSED  0xFF // Used in place of ChIdOrBoxInst in MemRead/WritePciCfg for MC functional blocks
                           // that have only 1 instance per MC



typedef
EFI_STATUS
(EFIAPI *EFI_MEM_NODE_ONLINE) (
  IN UINT8 Node,
  IN OUT UINT8 *MemHpStatus
  );

typedef
EFI_STATUS
(EFIAPI *EFI_MEM_NODE_OFFLINE) (
  IN UINT8 Node,
  IN OUT UINT8 *MemHpStatus
  );





typedef
EFI_STATUS
(EFIAPI *EFI_UPDATE_CONFIG_IN_QUIESCE) (
  IN UINT8 Operation
  );






typedef
EFI_STATUS
(EFIAPI *EFI_MIGRATION_POSSIBLE) (
    IN UINT8 MasterNodeId,
    IN UINT8 SlaveNodeId,
    OUT BOOLEAN *MigrationPossibleFlag,
    IN BOOLEAN IsMirrorMigration
    );

typedef
EFI_STATUS
(EFIAPI *EFI_SETUP_MEM_MIGRATION) (
    IN UINT8 SrcNodeId,
    IN UINT8 DestNodeId
    );

typedef
EFI_STATUS
(EFIAPI *EFI_DISABLE_MEM_MIGRATION) (
    IN UINT8 SrcNodeId,
    IN UINT8 DestNodeId
    );

typedef
EFI_STATUS
(EFIAPI *EFI_CALL_MEM_MIGRATION_STATE_MACHINE) (
    IN UINT8 MasterNodeId,
    IN UINT8 SlaveNodeId,
    IN UINT8 CacheLineSize,    // In Bytes
    IN UINT64 BlockSize,       // In Bytes
    OUT UINT8 *MigrationOpFlag
    );

typedef
EFI_STATUS
(EFIAPI *EFI_GET_CURRENT_MEM_NODE_STATE) (
    IN UINT8      Node,
    IN OUT UINT8 *MemState
);

typedef
EFI_STATUS
(EFIAPI *EFI_MODIFY_RAS_FLAG) (
    IN UINT64                         Flag,
    IN UINT8                          Value
);

typedef
EFI_STATUS
(EFIAPI *EFI_GET_PARAMETER) (
    IN UINT64                         ParamId,
    IN UINT64                        *Value
);




typedef
EFI_STATUS
(EFIAPI *EFI_ENABLE_LOGGING_ECC_CORR_ERR) (
    IN UINT8 Node
 );



typedef
EFI_STATUS
( EFIAPI * EFI_MODIFY_CACHE_LINE_AND_LOG_ERR) (
   IN UINT64   Start,         // start  logical(page mapped virtual) address for migration chunk   (do you need physical address here?)
   IN UINT64   Len,           // length for the chunk, must be aligned to CachelineSize
   IN UINT64  *ErrLog,        // The mbox local address log buffer  for error logging (1024 entries available)
                              // Bits 33:0 - MBOX local address, BIT63:56 - NodeIdx
   IN UINT8    CachelineSize, // 8 bytes  for UC;  64 bytes for WB
   IN UINT8    CacheFlush,    //  Is cflush required after each cache line copy
   IN UINT8    Node
   );



typedef
EFI_STATUS
(EFIAPI * EFI_OPEN_PAM_REGION)(
    IN UINT8 SocketId
 );

typedef
EFI_STATUS
(EFIAPI * EFI_RESTORE_PAM_REGION)(
    IN UINT8  SocketId
 );






//
// Memopry errors enbaling
//
typedef
EFI_STATUS
(EFIAPI *EFI_ENABLE_MEM_ERRORS_NODE) (
    IN UINT8 NodeId
  );

typedef
UINT8
(EFIAPI * EFI_GET_SUPPORTED_MEMRAS_MODES)(
    IN UINT8  NodeId,
    IN UINT8  DdrCh,
    IN UINT8  Rank
 );




// Protocol Definition
//
typedef struct _EFI_MEM_RAS_PROTOCOL {
  struct SystemMemoryMapHob         *SystemMemInfo;

  PSYSHOST                          pSysHost;
  UINT32                            Signature;

  EFI_MEM_NODE_ONLINE               MemoryNodeOnline;
  EFI_MEM_NODE_OFFLINE              MemoryNodeOffline;

  EFI_MIGRATION_POSSIBLE            MigrationPossible;
  EFI_SETUP_MEM_MIGRATION           SetupMemoryMigration;
  EFI_DISABLE_MEM_MIGRATION         DisableMemoryMigration;
  EFI_GET_CURRENT_MEM_NODE_STATE    GetCurrentMemoryNodeState;
  EFI_MODIFY_RAS_FLAG               ModifyRasFlag;

  EFI_OPEN_PAM_REGION                  OpenPamRegion;
  EFI_RESTORE_PAM_REGION               RestorePamRegion;

} EFI_MEM_RAS_PROTOCOL;

extern EFI_GUID gEfiMemRasProtocolGuid;

#endif
