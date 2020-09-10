/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2018 Intel Corporation. <BR>

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

#ifndef _IEHECIMSGS_H_
#define _IEHECIMSGS_H_

#pragma pack(1)

#define BIOS_FIXED_HOST_ADDR          0
#define HECI_CORE_MESSAGE_ADDR        0x07

///
/// BUP group IDs
///
typedef enum {
  BUP_COMMON_GROUP_ID = 0xF0,
} BUP_GROUP_ID;



typedef union _IE_HECI_MESSAGE_HEADER {
  UINT32  Data;
  struct {
    UINT32  GroupId    : 8;
    UINT32  Command    : 7;
    UINT32  IsResponse : 1;
    UINT32  Reserved   : 8;
    UINT32  Result     : 8;
  } Fields;
} IE_HECI_MESSAGE_HEADER;

//
// Typedef for Result field of MHKI Header
//
typedef enum {
  MkhiStatusSuccess,
  MkhiStatusInvalidState,
  MkhiStatusMessageSkipped,
  MkhiStatusSizeError       = 0x5,
  MkhiStatusInvalidAccess   = 0x84,
  MkhiStatusInvalidParams   = 0x85,
  MkhiStatusNotReady        = 0x88,
  MkhiStatusNotSupported    = 0x89,
  MkhiStatusInvalidAddress  = 0x8C,
  MkhiStatusInvalidCommand  = 0x8D,
  MkhiStatusFailure         = 0x9E,
  MkhiStatusInvalidResource = 0xE4,
  MkhiStatusResourceInUse   = 0xE5,
  MkhiStatusNoResource      = 0xE6,
  MkhiStatusGeneralError    = 0xFF
} MKHI_RESULT;

#define DRAM_INIT_DONE_CMD  0x01

///
/// BIOS action codes
///
#define DID_ACK_NON_PCR       0x1
#define DID_ACK_PCR           0x2
#define DID_ACK_RSVD3         0x3
#define DID_ACK_RSVD4         0x4
#define DID_ACK_RSVD5         0x5
#define DID_ACK_GRST          0x6
#define DID_ACK_CONTINUE_POST 0x7

///
/// DRAM Init Done MKHI Result
///
typedef enum {
  DidSuccess           = 0,
  DidInvalidProtection = 0xFFFA,
  DidBadAlignment,
  DidInternalError,
  DidUnexpectedRequest,
  DidSizeInvalid,
  DidUnhandled
} DID_MKHI_RESULT;

typedef struct _DRAM_INIT_DONE_DATA {
  UINT32  BiosImrsBaLow;
  UINT32  BiosImrsBaHigh;
} DRAM_INIT_DONE_DATA;

typedef struct _DRAM_INIT_DONE_CMD_REQ {
  IE_HECI_MESSAGE_HEADER  Header;
  DRAM_INIT_DONE_DATA     ImrData;
  UINT32                  Flags;
  UINT8                   Reserved1[2];
  UINT8                   MemStatus;
  UINT8                   Reserved2;
} DRAM_INIT_DONE_CMD_REQ;

///
/// Memory Init Status codes
///
#define BIOS_MSG_DID_SUCCESS              0
#define BIOS_MSG_DID_NO_MEMORY            0x1
#define BIOS_MSG_DID_INIT_ERROR           0x2
#define BIOS_MSG_DID_MEM_NOT_PRESERVED    0x3

typedef struct {
  UINT32  AdrLow;
  UINT32  AdrHigh;
} PCI_2_PRIV_BASE;

typedef struct _DRAM_INIT_DONE_CMD_RSP {
  IE_HECI_MESSAGE_HEADER  Header;
  UINT32                  Reserved1[4];
  PCI_2_PRIV_BASE         Pci2PrivBase;
  UINT8                   Flags;
  UINT8                   Action;
  UINT32                  AliasCheck;
  UINT8                   Reserved2[2];
} DRAM_INIT_DONE_CMD_RSP;

typedef union _DRAM_INIT_DONE_MSG {
  DRAM_INIT_DONE_CMD_REQ  Request;
  DRAM_INIT_DONE_CMD_RSP  Response;
} DRAM_INIT_DONE_MSG;

#define GET_IMR_SIZE_CMD         0x0C
#define ICC_STATUS_SUCCESS       0x00
///
/// Get IMR size message
///
typedef struct {
  UINT32        BiosImrDisableMask0;
  UINT32        BiosImrDisableMask1;
  UINT32        Pci2PrivSize;
} GET_IMR_CMD_REQ_DATA;

typedef struct {
  IE_HECI_MESSAGE_HEADER Header;
  GET_IMR_CMD_REQ_DATA   Data;
} GET_IMR_CMD_REQ;

///
/// Get IMR MKHI Result
///
typedef enum {
  GetImrSuccess            = 0,
  GetImrParameterTooBig    = 0xFFF7,
  GetImrResourceConflict,
  GetImrUnsupportedFeature,
  GetImrUnknownFeature,
  GetImrInconsistentRequest,
  GetImrInternalError,
  GetImrUnexpectedRequest,
  GetImrSizeInvalid,
  GetImrUnhandled
} GET_IMR_MKHI_RESULT;

typedef struct {
  UINT32        ImrsSize;
  UINT32        Alignment;
  UINT32        Flags;
} GET_IMR_CMD_RESP_DATA;

typedef struct {
  IE_HECI_MESSAGE_HEADER Header;
  GET_IMR_CMD_RESP_DATA  Data;
} GET_IMR_CMD_RESP;

typedef union {
  GET_IMR_CMD_REQ   Request;
  GET_IMR_CMD_RESP  Response;
} GET_IMR_CMD_BUFFER;

#pragma pack()

#endif /* _IEHECIMSGS_H_ */
