/** @file
  This code abstracts the generic ELOG Protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2018 Intel Corporation. <BR>

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

#ifndef _GENERIC_ELOG_H_
#define _GENERIC_ELOG_H_

#define EFI_SM_ELOG_PROTOCOL_GUID \
  { \
    0x59d02fcd, 0x9233, 0x4d34, 0xbc, 0xfe, 0x87, 0xca, 0x81, 0xd3, 0xdd, 0xa7 \
  }

#define EFI_SM_ELOG_REDIR_PROTOCOL_GUID \
  { \
    0x16d11030, 0x71ba, 0x4e5e, 0xa9, 0xf9, 0xb4, 0x75, 0xa5, 0x49, 0x4, 0x8a \
  }

typedef struct _EFI_SM_ELOG_PROTOCOL EFI_SM_ELOG_PROTOCOL;

typedef struct _EFI_SM_ELOG_PROTOCOL EFI_SM_ELOG_REDIR_PROTOCOL;

//
// Common Defines
//
typedef enum {
  EfiElogSmSMBIOS,
  EfiElogSmIPMI,
  EfiElogSmMachineCritical,
  EfiElogSmASF,
  EfiElogSmOEM,
  EfiSmElogMax
} EFI_SM_ELOG_TYPE;

//
//  Generic ELOG Function Prototypes
//
typedef
EFI_STATUS
(EFIAPI *EFI_SET_ELOG_DATA) (
  IN EFI_SM_ELOG_PROTOCOL               * This,
  IN  UINT8                             *ElogData,
  IN  EFI_SM_ELOG_TYPE                  DataType,
  IN  BOOLEAN                           AlertEvent,
  IN  UINTN                             DataSize,
  OUT UINT64                            *RecordId
  );

typedef
EFI_STATUS
(EFIAPI *EFI_GET_ELOG_DATA) (
  IN EFI_SM_ELOG_PROTOCOL               * This,
  IN  UINT8                             *ElogData,
  IN  EFI_SM_ELOG_TYPE                  DataType,
  IN  OUT UINTN                         *DataSize,
  IN OUT UINT64                         *RecordId
  );

typedef
EFI_STATUS
(EFIAPI *EFI_ERASE_ELOG_DATA) (
  IN EFI_SM_ELOG_PROTOCOL               * This,
  IN EFI_SM_ELOG_TYPE                   DataType,
  IN OUT UINT64                         *RecordId
  );

typedef
EFI_STATUS
(EFIAPI *EFI_ACTIVATE_ELOG) (
  IN EFI_SM_ELOG_PROTOCOL               * This,
  IN EFI_SM_ELOG_TYPE                   DataType,
  IN BOOLEAN                            *EnableElog,
  OUT BOOLEAN                           *ElogStatus
  );

//
// IPMI TRANSPORT PROTOCOL
//
struct _EFI_SM_ELOG_PROTOCOL {
  EFI_SET_ELOG_DATA   SetEventLogData;
  EFI_GET_ELOG_DATA   GetEventLogData;
  EFI_ERASE_ELOG_DATA EraseEventlogData;
  EFI_ACTIVATE_ELOG   ActivateEventLog;
};

extern EFI_GUID gEfiGenericElogProtocolGuid;
extern EFI_GUID gEfiRedirElogProtocolGuid;
extern EFI_GUID gSmmGenericElogProtocolGuid;
extern EFI_GUID gSmmRedirElogProtocolGuid;

#endif
