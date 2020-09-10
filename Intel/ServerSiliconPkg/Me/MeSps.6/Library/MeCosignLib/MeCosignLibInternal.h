/** @file
  HECI messages in context of cosigning feature.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation.

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

#ifndef _COSIGN_HECI_REQ_H_
#define _COSIGN_HECI_REQ_H_

#include <MkhiMsgs.h>

#define MKHI_SECURE_BOOT_GROUP_ID         0xC
#define MKHI_SB_VERIFY_MANIFEST_CMD_ID    0x1

#define MKHI_ARBH_SVN_GET_INFO_CMD_ID     0x1C
#define MKHI_ARBH_SVN_COMMIT_NONCE_CMD_ID 0x1D

#define MAX_ARBH_SVN_ENTRIES_NUM          20

#define MAX_CHUNK_SIZE                    484
#define MAX_MANIFEST_SIZE                 8000
#define MAX_METADATA_SIZE                 MAX_CHUNK_SIZE

#pragma pack(1)
typedef union _SB_VERIFY_MANIFEST_FLAGS {
  UINT8  Data;
  struct {
    UINT8 ManifestDataInChunk :1;
    UINT8 MetadataDataInChunk :1;
    UINT8 StartOverVerify     :1;
    UINT8 LastChunk           :1;
    UINT8 Reserved            :4;
  } Flags;
} SB_VERIFY_MANIFEST_FLAGS;

typedef struct _SB_VERIFY_MANIFEST_REQ {
  MKHI_MESSAGE_HEADER          MkhiHeader;
  UINT32                       UsageIndex;
  UINT32                       ManifestSize;
  UINT32                       MetaDataSize;
  SB_VERIFY_MANIFEST_FLAGS     Flags;
  UINT16                       ChunkSize;
  UINT8                        Reserved;
} SB_VERIFY_MANIFEST_REQ;

#define SB_VERIFY_MANIFEST_RSP_OK_CHUNK   0x01 /* Successful chunk process */
#define SB_VERIFY_MANIFEST_RSP_EMANHDR    0x0B /* Invalid Manifest header */
#define SB_VERIFY_MANIFEST_RSP_EINVSIG    0x0C /* Invalid Manifest signature */

typedef struct _SB_VERIFY_MANIFEST_RSP {
  MKHI_MESSAGE_HEADER          MkhiHeader;
  SB_VERIFY_MANIFEST_FLAGS     Flags;
  UINT16                       ChunkSize;
  UINT8                        Reserved;
} SB_VERIFY_MANIFEST_RSP;

#define ARBH_SVN_COMMIT_BSMM              0x20 /* Commit Bsmm SVN */
#define ARBH_SVN_COMMIT_SBKM              0x36 /* Commit SbKm SVN */
#define ARBH_SVN_COMMIT_UCODE             0x37 /* Commit uCode SVN */
#define ARBH_SVN_COMMIT_ACM               0x38 /* Commit ACM SVN */
#define ARBH_SVN_COMMIT_ALL               0xFF /* Commit all SVNs */

typedef struct _SB_ARBH_SVN_COMMIT_NONCE_REQ {
  MKHI_MESSAGE_HEADER           MkhiHeader;
  UINT64                        Nonce;
  UINT8                         UsageId;
  UINT8                         Reserved1;
  UINT16                        Reserved2;
} SB_ARBH_SVN_COMMIT_NONCE_REQ;

#define ARBH_SVN_COMMIT_INVALID_INPUT      0x0B /* Invalid command format */
#define ARBH_SVN_COMMIT_COMMIT_NOT_ALLOWED 0x19 /* Bad Nonce received */
#define ARBH_SVN_COMMIT_MAX_SVN_REACHED    0x1F /* Max SVN reached */

typedef struct _SB_ARBH_SVN_COMMIT_NONCE_RSP {
  MKHI_MESSAGE_HEADER           MkhiHeader;
} SB_ARBH_SVN_COMMIT_NONCE_RSP;
#pragma pack()

#endif // _COSIGN_HECI_REQ_H_
