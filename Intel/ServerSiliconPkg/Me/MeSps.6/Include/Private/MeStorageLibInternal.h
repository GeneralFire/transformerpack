/** @file
  Implements ME Storage HECI Interface Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2018 Intel Corporation.

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

#ifndef _ME_STORAGE_LIB_INTERNAL_H_
#define _ME_STORAGE_LIB_INTERNAL_H_

#define SPS_CLIENTID_ME_STORAGE_SERVICES   0x23

#define SPS_ME_STORAGE_SEQ_MAX             0x7F
#define SPS_ME_STORAGE_SEQ_FLAG            0x80

typedef enum {
  SPS_ME_STORAGE_READ_STORAGE_ENTRY_CMD      = 0,
  SPS_ME_STORAGE_WRITE_STORAGE_ENTRY_CMD     = 1
} ME_STORAGE_CMDS;

#pragma pack(1)
typedef struct {
  UINT8           ApiFunctionSelector;
  UINT8           SeqNumber;
  UINT64          Nonce;
  UINT8           StorageEntryIndex;
} ME_READ_STORAGE_SERVICES_REQ;

typedef struct {
  UINT8           ApiFunctionSelector;
  UINT8           SeqNumber;
  UINT8           OperationStatus;
  UINT8           StorageEntryIndex;
  UINT8           Data[ME_STORAGE_MAX_BUFFER];
} ME_READ_STORAGE_SERVICES_RSP;

typedef struct {
  UINT8           ApiFunctionSelector;
  UINT8           SeqNumber;
  UINT64          Nonce;
  UINT8           StorageEntryIndex;
  UINT8           Data[ME_STORAGE_MAX_BUFFER];
} ME_WRITE_STORAGE_SERVICES_REQ;

typedef struct {
  UINT8           ApiFunctionSelector;
  UINT8           SeqNumber;
  UINT8           OperationStatus;
  UINT8           StorageEntryIndex;
} ME_WRITE_STORAGE_SERVICES_RSP;

typedef union {
    ME_READ_STORAGE_SERVICES_REQ MeStorageSrvReq;
    ME_READ_STORAGE_SERVICES_RSP MeStorageSrvRsp;
} ME_READ_STORAGE_SERVICES;

typedef union {
    ME_WRITE_STORAGE_SERVICES_REQ MeStorageSrvReq;
    ME_WRITE_STORAGE_SERVICES_RSP MeStorageSrvRsp;
} ME_WRITE_STORAGE_SERVICES;
#pragma pack()

#endif // _ME_STORAGE_LIB_INTERNAL_H_
