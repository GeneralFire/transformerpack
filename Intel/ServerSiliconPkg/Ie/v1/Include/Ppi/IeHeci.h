/** @file
  IeHeci PPI as defined in EFI 2.0

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

#ifndef _PEI_IE_HECI_PPI_H_
#define _PEI_IE_HECI_PPI_H_

//
// PEI Timeout values
//
#define PEI_HECI_WAIT_DELAY    1000    // 1 ms
#define PEI_HECI_INIT_TIMEOUT  2000000 // 2 s
#define PEI_HECI_READ_TIMEOUT  500000  // 0.5 s
#define PEI_HECI_SEND_TIMEOUT  500000  // 0.5 s
#define PEI_HECI_RESET_TIMEOUT 2000000 // 2 sec


//
// Heci PPI definitions
//
#define PEI_IE_HECI_PPI_GUID \
  { \
     0x86d2df6a, 0x6f0d, 0x4d61, {0xbc, 0x3c, 0xb1, 0xbe, 0x2d, 0x2c, 0x1f, 0x61 }\
  }

typedef struct _PEI_IE_HECI_PPI PEI_IE_HECI_PPI;

typedef
EFI_STATUS
(EFIAPI *PEI_IE_HECI_SENDWACK) (
  IN      PEI_IE_HECI_PPI        *This,
  IN OUT  UINT32                 *Message,
  IN OUT  UINT32                 *Length,
  IN      UINT8                  HostAddress,
  IN      UINT8                  IeAddress,
  IN OUT  EFI_STATUS             *SendStatus,
  IN OUT  EFI_STATUS             *ReceiveStatus
  );

typedef
EFI_STATUS
(EFIAPI *PEI_IE_HECI_READ_MESSAGE) (
  IN      PEI_IE_HECI_PPI        *This,
  IN      UINT32                 Blocking,
  IN      UINT32                 *MessageBody,
  IN OUT  UINT32                 *Length
  );

typedef
EFI_STATUS
(EFIAPI *PEI_IE_HECI_SEND_MESSAGE) (
  IN      PEI_IE_HECI_PPI        *This,
  IN      UINT32                 *Message,
  IN      UINT32                 Length,
  IN      UINT8                  HostAddress,
  IN      UINT8                  IeAddress
  );

typedef
EFI_STATUS
(EFIAPI *PEI_IE_HECI_INITIALIZE) (
  IN      PEI_IE_HECI_PPI           *This
  );

//
// Memory initialization status passed to IE using DramInitDoneMessage
//
#define IE_MEMORY_INITIALIZATION_SUCCESSS                        0x00
#define IE_MEMORY_INITIALIZATION_NO_MEMORY_IN_CHANNELS           0x01
#define IE_MEMORY_INITIALIZATION_ERROR                           0x02

typedef
EFI_STATUS
(EFIAPI *PEI_IE_HECI_MEMORY_INIT_DONE_NOTIFY) (
  IN      PEI_IE_HECI_PPI        *This,
  IN      UINT32                 ImrBaseLow,
  IN      UINT32                 ImrBaseHigh,
  IN      UINT8                  MrcStatus,
  IN OUT  UINT8                  *Action
  );

typedef
EFI_STATUS
(EFIAPI *PEI_IE_HECI_GET_UMA_SIZE) (
  IN      PEI_IE_HECI_PPI        *This,
  OUT     UINT32                 *ImrsSize,
  OUT     UINT32                 *Alignment
  );

typedef
BOOLEAN
(EFIAPI *PEI_IE_HECI_UMA_ENABLED) (
  IN      PEI_IE_HECI_PPI        *This
  );

typedef
EFI_STATUS
(EFIAPI *PEI_IE_HECI_ACTION) (
  IN       PEI_IE_HECI_PPI       *This,
  IN OUT   UINT8                 BiosAction
  );

typedef struct _PEI_IE_HECI_PPI {

  PEI_IE_HECI_SENDWACK       SendwAck;
  PEI_IE_HECI_READ_MESSAGE   ReadMsg;
  PEI_IE_HECI_SEND_MESSAGE   SendMsg;
  PEI_IE_HECI_INITIALIZE     InitializeHeci;

  PEI_IE_HECI_UMA_ENABLED             IsIeUmaEnabled;
  PEI_IE_HECI_MEMORY_INIT_DONE_NOTIFY MemoryInitDoneNotify;
  PEI_IE_HECI_ACTION                  MemoryInitDoneAction;
  PEI_IE_HECI_GET_UMA_SIZE            GetUmaSize;
  UINT32                              IeUmaSize;
  UINT32                              IeAlignment;
} PEI_IE_HECI_PPI;

extern EFI_GUID gPeiIeHeciPpiGuid;


#endif
