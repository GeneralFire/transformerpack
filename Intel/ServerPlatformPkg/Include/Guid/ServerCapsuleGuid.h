/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2019 Intel Corporation. <BR>

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

#ifndef _SERVER_CAPSULE_GUID_H_
#define _SERVER_CAPSULE_GUID_H_


#define SMI_INPUT_GET_BOOT_INFO 0x26
#define SMI_INPUT_UPDATE_CAP    0x27
#define SMI_INPUT_GET_CAP       0x28
#define SMI_INPUT_UPDATE_SPI    0x29

#define SMI_CAP_FUNCTION            0xEF

#define UPDATE_STARTED                  0x00
#define UPDATE_COMPLETED                0x01
#define UPDATE_FAILED                   0x02
#define TARGET_BIOS                     0x10

#pragma pack(1)
#define MAX_MAIL_BOX_SIZE  256
typedef struct
{
 UINT16    CmdSize;    //Total Command size
 CHAR8    CmdPointer[MAX_MAIL_BOX_SIZE ];//Use null as the string end for each Command
}MAILBOX_IN;

typedef struct
{
 UINT16    DataSize;    //Total Command size
 CHAR8    DataPointer[MAX_MAIL_BOX_SIZE ];//Use null as the string end for each Command
}MAILBOX_OUT;

//for SMI26, it's out is different for SMI27 and 28
typedef struct
{
 UINT16    Status;    //Command success or error.
 CHAR8    DataPointer[MAX_MAIL_BOX_SIZE ];
                                                       //Use null as the string end for each Command
}MISC_MAILBOX_OUT;


typedef struct {
   UINT64  Address;
   UINT32  BufferOffset;
   UINT32  Size;
   UINT32  Flags;
   UINT32  Reserved;
} CAPSULE_FRAGMENT;

typedef struct {
  UINTN         CapsuleLocation;  // Top of the capsule that point to structure CAPSULE_FRAGMENT
  UINTN         CapsuleSize;    // Size of the capsule
  EFI_STATUS    Status;      // Returned status
  MAILBOX_IN    MailBoxIn;
  UINT8         Revision;
} CAPSULE_INFO_PACKET;

typedef struct {
  UINTN           BlocksCompleted;  // # of blocks processed
  UINTN           TotalBlocks;      // Total # of blocks to be processed
  EFI_STATUS      Status;            // returned status
  MAILBOX_OUT     MailBoxOut;
  UINT8           Revision;
} UPDATE_STATUS_PACKET;

// The STATE_MACHINE_PRIVATE_DATA is changed from the EDKII implementation
// this is due to the fact that ,while using FVB is ideal and most appropriate method
// it poses a challenge in recovery mode where the FV header is invalid or corrupt. so
// we had to revert to a method wherein we do not verify the FV header integrity. To keep
// the design simple and to bring commanality , this design is leveraged from romley.
typedef struct _STATE_MACHINE_PRIVATE_DATA_ {
  UINT64    SourceAddr;
  UINT32    TargetAddr;
  UINT32    Size;
  BOOLEAN   IsValid;
  BOOLEAN   UpdateStatus;
} STATE_MACHINE_PRIVATE_DATA;

typedef struct {
  EFI_GUID  Name;
  UINT32    TargetAddr;
  UINT32    Size;
  UINT32    SourceOffset;
  UINT8     PlatformID;
  UINT8     CustomerID;
  UINT16    Reserved;
} UPDATE_HINT;

#pragma pack()

extern EFI_GUID gEfiServerCapsuleGuid;

#endif
