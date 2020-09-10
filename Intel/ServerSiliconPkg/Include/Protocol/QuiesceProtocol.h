/** @file
  GUIDs used for Quiesce support

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

#ifndef _QUIESCE_SUPPORT_PROTOCOL_H_
#define _QUIESCE_SUPPORT_PROTOCOL_H_

//
// Includes
//

//#include "Tiano.h"

//
// Forward reference for pure ANSI compatability
//
//EFI_FORWARD_DECLARATION (EFI_QUIESCE_SUPPORT_PROTOCOL);

#define QUIESCE_SUPPORT_GUID \
  { \
    0x20d6e759, 0x4c4a, 0x40c0, 0x95, 0x33, 0x2b, 0xf0, 0x6, 0x68, 0x50, 0xfd \
  }


#define SYSTEM_QUIESCE    1
#define SYSTEM_UNQUIESCE  0


#pragma pack(1)

typedef enum {
  EnumQuiesceWrite = 1,
  EnumQuiescePoll,
  EnumQuiesceRunSvQuiesceCode,
  EnumQuiesceOperationEnd = 0xFF,
} QUIESCE_OPERATION;

typedef struct {
  UINT8                 Operation;
  UINT8                 AccessWidth;
  UINT64                CsrAddress;
  UINT32                AndMask;
  union {
    UINT32              Data;
    UINT32              OrMask;
  } DataMask;
} QUIESCE_DATA_TABLE_ENTRY;


typedef struct {
  UINT32                        GetQuiesceTable;
  UINT32                        Entries;
  QUIESCE_DATA_TABLE_ENTRY      *DataBuffer;
} QUIESCE_TABLE_ENTRY;

#pragma pack()

//
// Protocol Data Structures
//
typedef
EFI_STATUS
(EFIAPI *EFI_GET_QUIESCE_CODE_MEMORY_INFO) (
  IN OUT UINTN *QuieceCodeBaseAddress,
  IN OUT UINTN *QuiesceCodeSize
);

typedef
EFI_STATUS
(EFIAPI *EFI_GET_QUIESCE_DATA_MEMORY_INFO) (
  IN OUT UINTN *QuieceDataBaseAddress,
  IN OUT UINTN *QuiesceDataSize
);


typedef
EFI_STATUS
(EFIAPI *EFI_SET_BUILD_DATA_MEMORY_INFO) (
  IN UINTN BuildDataBaseAddress,
  IN UINTN BuildDataSize
);

typedef
EFI_STATUS
(EFIAPI *EFI_QUIESCE_AP_PROC) (
  VOID
);

typedef
EFI_STATUS
(EFIAPI *EFI_QUIESCE_AQUIRE_DATA_BUFFER) (
  IN OUT EFI_PHYSICAL_ADDRESS   *BufferAddress
);


typedef
EFI_STATUS
(EFIAPI *EFI_QUIESCE_RELEASE_DATA_BUFFER) (
  IN EFI_PHYSICAL_ADDRESS  BaseAddress
);


typedef
EFI_STATUS
(EFIAPI *EFI_QUIESCE_UNQUIESCE) (
  UINT8 QuiesceFlag
);

typedef
EFI_STATUS
(EFIAPI *EFI_QUIESCE_MONARCHAP_CHECKIN) (
  IN BOOLEAN  CheckIn
);

typedef
EFI_STATUS
(EFIAPI *EFI_QUIESCE_MAIN) (
  VOID
);


typedef struct _EFI_QUIESCE_SUPPORT_PROTOCOL {

  EFI_GET_QUIESCE_CODE_MEMORY_INFO  GetQuiesceCodeMemoryInfo;
  EFI_GET_QUIESCE_DATA_MEMORY_INFO  GetQuiesceDataMemoryInfo;
  EFI_QUIESCE_AP_PROC               QuiesceApProc;
  EFI_QUIESCE_AQUIRE_DATA_BUFFER    AquireDataBuffer;
  EFI_QUIESCE_RELEASE_DATA_BUFFER   ReleaseDataBuffer;
  EFI_QUIESCE_MONARCHAP_CHECKIN     MonarchAPCheckIn;
  EFI_QUIESCE_MAIN                  QuiesceMain;
  EFI_QUIESCE_UNQUIESCE             SystemQuiesceUnQuiesce;


} EFI_QUIESCE_SUPPORT_PROTOCOL;

extern EFI_GUID gEfiQuiesceProtocolGuid;

#endif
