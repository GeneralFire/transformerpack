/** @file
  uba config database head file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2017 Intel Corporation. <BR>

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

#ifndef _UBA_CONFIG_DATABASE_HOB_H_
#define _UBA_CONFIG_DATABASE_HOB_H_


#define UBA_CONFIG_HOB_SIGNATURE    SIGNATURE_32('U', 'B', 'A', 'H')
#define UBA_CONFIG_HOB_VERSION      0x01

#define UBA_BOARD_SIGNATURE         SIGNATURE_32('S', 'K', 'U', 'D')
#define UBA_BOARD_VERSION           0x01

//
// Interface data between PEI & DXE
// Should keep same align
//
#pragma pack (1)

typedef struct _UBA_CONFIG_HOB_FIELD {
  UINT32                  Signature;
  UINT32                  Version;
  EFI_GUID                ResId;
  UINT64                  DataOffset;
  UINT32                  Size;
} UBA_CONFIG_HOB_FIELD;

typedef struct _UBA_CONFIG_HOB_HEADER {
  UINT32                  Signature;
  UINT32                  Version;
  EFI_GUID                DataGuid;
  UINT32                  HobLength;
  UINT32                  BoardId;
  EFI_GUID                BoardGuid;
  CHAR8                   BoardName[16];
  UINT32                  DataCount;
  UBA_CONFIG_HOB_FIELD    HobField[1];
} UBA_CONFIG_HOB_HEADER;


typedef struct _UBA_BOARD_NODE {
  UINT32                  Signature;
  UINT32                  Version;
  LIST_ENTRY              DataLinkHead;

  UINT32                  BoardId;
  EFI_GUID                BoardGuid;
  CHAR8                   BoardName[16];
  UINT32                  DataCount;
} UBA_BOARD_NODE;

typedef struct _UBA_CONFIG_NODE {
  UINT32                  Signature;
  UINT32                  Version;
  LIST_ENTRY              DataLink;

  EFI_HANDLE              Handle;
  EFI_GUID                ResId;
  UINT32                  Size;
  VOID                    *Data;
} UBA_CONFIG_NODE;

#pragma pack ()

#define BOARD_NODE_INSTANCE_FROM_THIS(p)       CR(p, UBA_BOARD_NODE, DataLinkHead, UBA_BOARD_SIGNATURE)
#define CONFIG_NODE_INSTANCE_FROM_THIS(p)      CR(p, UBA_CONFIG_NODE, DataLink, UBA_BOARD_SIGNATURE)

#endif // _UBA_CONFIG_DATABASE_HOB_H_
