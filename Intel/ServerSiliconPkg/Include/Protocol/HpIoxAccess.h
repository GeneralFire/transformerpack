/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 Intel Corporation. <BR>

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

#ifndef _HPIOXACCESS_PROTOCOL_H_
#define _HPIOXACCESS_PROTOCOL_H_

//
// Forward reference for pure ANSI compatability
//
//EFI_FORWARD_DECLARATION (EFI_HP_IOX_ACCESS_PROTOCOL);


//
// HP IOX Access Protocol GUID
//
// {62652B53-79D9-4CF2-B5AA-AD99810A7F17}
#define EFI_HP_IOX_ACCESS_GUID \
  { \
    0x62652b53, 0x79d9, 0x4cf2, 0xb5, 0xaa, 0xad, 0x99, 0x81, 0x0a, 0x7f, 0x17 \
  }

#define ISSUE_ONLINE                      0
#define ISSUE_OFFLINE                     1
#define ISSUE_WARM_RESET                  2
#define ISSUE_POWER_ON                    3
#define ISSUE_BIOS_READY                  4
#define ISSUE_PLD_IDLE                    5

typedef
EFI_STATUS
(EFIAPI *READ_IOX_AND_UPDATE_HP_REQUEST) (
  IN UINT8      *HpRequest,
  IN BOOLEAN    TimeSliced
  );

typedef
EFI_STATUS
(EFIAPI *ISSUE_PLD_CMD_THRO_IOX) (
  IN UINT8           Request,
  IN UINT8           SocketId,
  IN BOOLEAN         TimeSliced
  );

typedef
EFI_STATUS
(EFIAPI *CLEAR_ATTN_LATCH) (
  IN BOOLEAN    TimeSliced
  );

typedef
EFI_STATUS
(EFIAPI *CLEAR_INTERRUPT) (
  IN  BOOLEAN TimeSliced
  );

typedef
EFI_STATUS
(EFIAPI *HP_SMBUS_READ_WRITE) (
  IN      UINT8    SlaveAddress,
  IN      UINT8    Operation,
  IN      UINT8    Command,
  IN OUT  VOID     *Buffer,
  IN      BOOLEAN  TimeSliced
  );

typedef
BOOLEAN
(EFIAPI *CHECK_ONLINE_OFFLINE_REQUEST) (
  VOID
  );

typedef struct _EFI_HP_IOX_ACCESS_PROTOCOL {
  READ_IOX_AND_UPDATE_HP_REQUEST   ReadIoxAndUpdateHpRequest;
  ISSUE_PLD_CMD_THRO_IOX           IssuePldCmdThroIox;
  CLEAR_ATTN_LATCH                 ClearAttnLatch;
  CLEAR_INTERRUPT                  ClearInterrupt;
  HP_SMBUS_READ_WRITE              HpSmbusReadWrite;
  CHECK_ONLINE_OFFLINE_REQUEST     CheckOnlineOfflineRequest;
} EFI_HP_IOX_ACCESS_PROTOCOL;

extern EFI_GUID         gEfiHpIoxAccessGuid;

#endif
