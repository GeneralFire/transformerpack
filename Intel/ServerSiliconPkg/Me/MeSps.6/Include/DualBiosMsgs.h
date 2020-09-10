/** @file
  DualBios Messages

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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

#ifndef _DUAL_BIOS_MSGS_H
#define _DUAL_BIOS_MSGS_H

#define HECI_RSP_STATUS_SUCCESS                    0x0

#define SPS_CLIENTID_ME_WATCHDOG_SERVICES          0x20

/// SPS_BIOS_UPDATE_WATCHDOG_CONTROL
#define SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL_REQ   0x05
#define SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL_RSP   0x85

/// Actions
#define BIOS_UPDATE_WATCHDOG_CTRL_FLAG_SUSPEND 0x1
#define BIOS_UPDATE_WATCHDOG_CTRL_FLAG_RESUME  0x2
#define BIOS_UPDATE_WATCHDOG_CTRL_FLAG_STOP    0x4

#pragma pack (1)
typedef struct {
  UINT8           Command;    /// =SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL_REQ
  UINT64          Nonce;      /// BIOS Security Nonce
  UINT8           ActionFlag; /// BIOS_UPDATE_WATCHDOG_CTRL_FLAG_*
} HECI_MSG_BIOS_UPDATE_WATCHDOG_CONTROL_REQ;

typedef struct {
  UINT8           Command; /// =SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL_RSP
  UINT8           Status;  /// BIOS_UPDATE_WATCHDOG_CTRL_STATUS_*
} HECI_MSG_BIOS_UPDATE_WATCHDOG_CONTROL_RSP;

typedef union {
  HECI_MSG_BIOS_UPDATE_WATCHDOG_CONTROL_REQ Req;
  HECI_MSG_BIOS_UPDATE_WATCHDOG_CONTROL_RSP Rsp;
} HECI_MSG_WATCHDOG_CONTROL;
#pragma pack ()

/// SPS_BIOS_UPDATE_GET_IMAGE_INVENTORY HECI message
#define SPS_CMD_BIOS_UPDATE_GET_IMAGE_INVENTORY_REQ   0x06
#define SPS_CMD_BIOS_UPDATE_GET_IMAGE_INVENTORY_RSP   0x86

#pragma pack (1)
typedef struct {
  UINT8           Command; /// =SPS_CMD_BIOS_UPDATE_GET_IMAGE_INVENTORY_REQ
  UINT8           ImageId; /// BIOS_UPDATE_GET_IMAGE_INVENTORY_REGION*
} HECI_MSG_BIOS_UPDATE_GET_IMAGE_INVENTORY_REQ;
#pragma pack ()

#define BIOS_UPDATE_GET_IMAGE_INVENTORY_REGION1             0x10
#define BIOS_UPDATE_GET_IMAGE_INVENTORY_REGION2             0x20

#pragma pack (1)
typedef union {
  UINT8    Byte;
  struct {
    UINT8   Reserved1     : 2,  /// [0:1] Reserved
            IsRunning     : 1,  /// [2]   Image is currently running
            ImageStatus   : 4,  /// [6:3] BIOS_UPDATE_GET_IMAGE_INVENTORY_IMG_STATUS_*
            Reserved2     : 1;  /// [7]   Reserved.
  } Bits;
} IMAGE_INVENTORY;
#pragma pack ()

#pragma pack (1)
typedef struct {
  UINT8           Command; /// =SPS_CMD_BIOS_UPDATE_GET_IMAGE_INVENTORY_RSP
  UINT8           Status;  /// BIOS_UPDATE_GET_IMAGE_INVENTORY_STATUS_*
  IMAGE_INVENTORY ImageInventory;
} HECI_MSG_BIOS_UPDATE_GET_IMAGE_INVENTORY_RSP;
#pragma pack ()

#define BIOS_UPDATE_GET_IMAGE_INVENTORY_IMG_STATUS_ACTIVE     0x1 /* recently used to boot platform */
#define BIOS_UPDATE_GET_IMAGE_INVENTORY_IMG_STATUS_INACTIVE   0x2 /* contains BIOS image but currently not used for booting */
#define BIOS_UPDATE_GET_IMAGE_INVENTORY_IMG_STATUS_BOOTNEXT   0x3 /* ready to boot on next platform reset */
#define BIOS_UPDATE_GET_IMAGE_INVENTORY_IMG_STATUS_INVALID    0x4 /* not used currently */
#define BIOS_UPDATE_GET_IMAGE_INVENTORY_IMG_STATUS_FAILED     0x5 /* the regions contains an image that failed due to timeout */

/// SPS_BIOS_UPDATE_REGION_SELECT HECI message
#define SPS_CMD_BIOS_UPDATE_REGION_SELECT_REQ   0x04
#define SPS_CMD_BIOS_UPDATE_REGION_SELECT_RSP   0x84

#pragma pack (1)
typedef struct {
  UINT8           Command;           /// =SPS_CMD_BIOS_UPDATE_REGION_SELECT_REQ
  UINT64          Nonce;
  UINT16          BootTimeout;       /// The timeout value in 100ms ticks
  UINT8           BootTimeoutAction; /// BIOS_UPDATE_REGION_SELECT_TIMEOUT_*
  UINT8           BiosRegionNum;     /// BIOS_UPDATE_REGION_SELECT_REG_NUM_*
} HECI_MSG_BIOS_UPDATE_REGION_SELECT_REQ;
#pragma pack ()

///BootTimeout actions:
#define BIOS_UPDATE_REGION_SELECT_TIMEOUT_NO_ACTION       0x00
#define BIOS_UPDATE_REGION_SELECT_TIMEOUT_ACTION_RESET    0x01

/// Currently only "other than currently in use region" is supported
#define BIOS_UPDATE_REGION_SELECT_REG_NUM_OTHER           0x00

#pragma pack (1)
typedef struct {
  UINT8           Command;     /// = SPS_CMD_BIOS_UPDATE_REGION_SELECT_RSP
  UINT8           Status;
  UINT8           BiosRegNum;
} HECI_MSG_BIOS_UPDATE_REGION_SELECT_RSP;
#pragma pack ()

#endif /* _DUAL_BIOS_MSGS_H */
