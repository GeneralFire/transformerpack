/** @file
  This code abstracts the generic FRU Protocol.

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

#ifndef _REDIR_FRU_H_
#define _REDIR_FRU_H_

typedef struct _EFI_SM_FRU_REDIR_PROTOCOL EFI_SM_FRU_REDIR_PROTOCOL;

#define EFI_SM_FRU_REDIR_PROTOCOL_GUID \
  { \
    0x28638cfa, 0xea88, 0x456c, 0x92, 0xa5, 0xf2, 0x49, 0xca, 0x48, 0x85, 0x35 \
  }

// {41F49AE4-7FB0-4c54-994E-EA199171B0AC}
#define EFI_PRE_FRU_SMBIOS_DATA_GUID \
  { \
    0x41f49ae4, 0x7fb0, 0x4c54, 0x99, 0x4e, 0xea, 0x19, 0x91, 0x71, 0xb0, 0xac \
  }

#define EFI_SM_FRU_REDIR_SIGNATURE  SIGNATURE_32 ('f', 'r', 'r', 'x')


//
//  Redir FRU Function Prototypes
//
typedef
EFI_STATUS
(EFIAPI *EFI_GET_FRU_REDIR_INFO) (
  IN EFI_SM_FRU_REDIR_PROTOCOL            * This,
  IN  UINTN                               FruSlotNumber,
  OUT EFI_GUID                            * FruFormatGuid,
  OUT UINTN                               *DataAccessGranularity,
  OUT CHAR16                              **FruInformationString
  );

typedef
EFI_STATUS
(EFIAPI *EFI_GET_FRU_SLOT_INFO) (
  IN EFI_SM_FRU_REDIR_PROTOCOL            * This,
  OUT EFI_GUID                            * FruTypeGuid,
  OUT UINTN                               *StartFruSlotNumber,
  OUT UINTN                               *NumSlots
  );

typedef
EFI_STATUS
(EFIAPI *EFI_GET_FRU_REDIR_DATA) (
  IN EFI_SM_FRU_REDIR_PROTOCOL            * This,
  IN  UINTN                               FruSlotNumber,
  IN  UINTN                               FruDataOffset,
  IN  UINTN                               FruDataSize,
  IN  UINT8                               *FruData
  );

typedef
EFI_STATUS
(EFIAPI *EFI_SET_FRU_REDIR_DATA) (
  IN EFI_SM_FRU_REDIR_PROTOCOL            * This,
  IN  UINTN                               FruSlotNumber,
  IN  UINTN                               FruDataOffset,
  IN  UINTN                               FruDataSize,
  IN  UINT8                               *FruData
  );

//
// REDIR FRU PROTOCOL
//
struct _EFI_SM_FRU_REDIR_PROTOCOL {
  EFI_GET_FRU_REDIR_INFO  GetFruRedirInfo;
  EFI_GET_FRU_SLOT_INFO   GetFruSlotInfo;
  EFI_GET_FRU_REDIR_DATA  GetFruRedirData;
  EFI_SET_FRU_REDIR_DATA  SetFruRedirData;
};

extern EFI_GUID gEfiRedirFruProtocolGuid;
extern EFI_GUID gEfiPreFruSmbiosDataGuid;

#endif
