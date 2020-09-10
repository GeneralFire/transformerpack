/** @file
  MemBootHealthGuid.h

 Header for using Structured PCD in MemBootHealth

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

#ifndef _MEM_BOOT_HEALTH_GUID_H_
#define _MEM_BOOT_HEALTH_GUID_H_

// Enable Mem boot health check feature
#define MEM_BOOT_HEALTH_ENABLE 1
// Disable Mem boot health check feature
#define MEM_BOOT_HEALTH_DISABLE 0

// Mem Boot Health check option
// Option to choose Mem Boot Health configuration type.
// 00=>Auto (Use defaults)
#define MEM_BOOT_HEALTH_CONFIG_AUTO 0
// 01=>Manual(Override defaults with setup option)
#define MEM_BOOT_HEALTH_CONFIG_MANUAL 1
// 02=>Disable (Disable feature)
#define MEM_BOOT_HEALTH_CONFIG_DISABLE 2

#define ENABLE_REBOOT_ON_CRITICAL_FAILURE 1
#define DISABLE_REBOOT_ON_CRITICAL_FAILURE 0

#pragma pack(1)
typedef struct {

  //
  // Memory Boot Health Check
  //
  UINT8                   MemBootHealthVisible;    // 0 => Hide Memory boot health check option, 1 => Enable the option in setup
  
  UINT8                   MemBootHealthCheck;     // 0=>Auto, 1=>Manual and 2=>Disabled Memory Boot Health Check

  UINT8                   ResetOnCriticalError;   // 0 => Dont reboot on critical error, 1 = Reboot on critical error
  //
  // Memory Boot Health check parameters
  //
  UINT8                   WarningTxDqDelayLeftEdge;
  UINT8                   WarningTxDqDelayRightEdge;
  UINT8                   WarningTxVrefLeftEdge;
  UINT8                   WarningTxVrefRightEdge;
  UINT8                   WarningRxDqsDelayLeftEdge;
  UINT8                   WarningRxDqsDelayRightEdge;
  UINT8                   WarningRxVrefLeftEdge;
  UINT8                   WarningRxVrefRightEdge;

  UINT8                   CriticalTxDqDelayLeftEdge;
  UINT8                   CriticalTxDqDelayRightEdge;
  UINT8                   CriticalTxVrefLeftEdge;
  UINT8                   CriticalTxVrefRightEdge;
  UINT8                   CriticalRxDqsDelayLeftEdge;
  UINT8                   CriticalRxDqsDelayRightEdge;
  UINT8                   CriticalRxVrefLeftEdge;
  UINT8                   CriticalRxVrefRightEdge;
} MEM_BOOT_HEALTH_CONFIG;
#pragma pack()

#define MEM_BOOT_HEALTH_GUID { 0xACD56900, 0xDEFC, 0x4127, { 0xDE, 0x12, 0x32, 0xA0, 0xD2, 0x69, 0x46, 0x2F } }

#define MEM_BOOT_HEALTH_SETUP_STR  L"MemBootHealthConfig"

extern EFI_GUID gMemBootHealthGuid;
#endif // _MEM_BOOT_HEALTH_GUID_H_
