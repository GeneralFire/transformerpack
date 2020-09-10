/** @file
  Server Management Setup Variable definitions.

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

#ifndef _SERVER_MGMT_SETUP_VARIABLE_H_
#define _SERVER_MGMT_SETUP_VARIABLE_H_

#define ERROR_LOGGING_SETUP_KEY_ID   0xF0

#define SERVER_MGMT_CONFIGURATION_GUID \
  { 0x1239999, 0xfc0e, 0x4b6e, 0x9e, 0x79, 0xd5, 0x4d, 0x5d, 0xb6, 0xcd, 0x20 }


#pragma pack(1)
typedef struct {
  UINT8         PropagatePerr;
  UINT8         PropagateSerr;
  UINT8         SvrMngmntResumeAcPowerLoss;//DO NOT MODIFY
  UINT8         SvrMngmntSelLogClear;//DO NOT MODIFY
  UINT8         SvrMngmntFrb2Enable;//DO NOT MODIFY
  UINT8         OSBootWDTimer;//DO NOT MODIFY
  UINT16        OSBootWDTimerTimeout;//DO NOT MODIFY
  UINT8         OSBootWDTimerPolicy;//DO NOT MODIFY
  UINT8         SvrMngmntAcpiIpmi; // DO NOT MODIFY
  UINT8         SerialOverLanEnable;      //DO NOT MODIFY
  UINT8         SvrMngmntConsRedirSerialEnable;//DO NOT MODIFY
  UINT8         SvrMngmntConsRedirSerial1ConnectionType;//DO NOT MODIFY
  UINT8         SvrMngmntConsRedirSerial1FlowCtrl;//DO NOT MODIFY
  UINT8         SvrMngmntConsRedirSerial1BitRate;//DO NOT MODIFY
  UINT8         SvrMngmntConsRedirSerial2ConnectionType;//DO NOT MODIFY
  UINT8         SvrMngmntConsRedirSerial2FlowCtrl;//DO NOT MODIFY
  UINT8         SvrMngmntConsRedirSerial2BitRate;//DO NOT MODIFY
} SERVER_MGMT_CONFIGURATION_DATA;
#pragma pack()

#define SERVER_MGMT_CLASS_ID      20
#define SERVER_MGMT_SUBCLASS_ID    0x00

#define  TIME_3MIN    180  //Time in seconds
#define  TIME_4MIN    240
#define  TIME_5MIN    300
#define  TIME_6MIN    360
#define  TIME_10MIN   600
#define  TIME_15MIN   900
#define  TIME_20MIN  1200

#define VAR_EQ_ADMIN_NAME               0x0041  // A
#define VAR_EQ_ADMIN_DECIMAL_NAME       L"65"
#define VAR_EQ_USER_NAME                0x0055  // U
#define VAR_EQ_USER_DECIMAL_NAME        L"85"
#define VAR_EQ_CONFIG_MODE_NAME         0x0043  // C
#define VAR_EQ_CONFIG_MODE_DECIMAL_NAME L"67"
#define VAR_EQ_HT_MODE_NAME             0x0048  // H
#define VAR_EQ_HT_MODE_DECIMAL_NAME     L"72"
#define VAR_EQ_RAID_MODE_NAME           0x0052  // R
#define VAR_EQ_RAID_MODE_DECIMAL_NAME   L"82"

//
// Error Logging Specific Setup Variable Default Values
//
#define ERROR_LOGGING_SETUP_DATA_DEFAULT_SMBIOSLOGGING       0x01
#define ERROR_LOGGING_SETUP_DATA_DEFAULT_SMBIOSEVENTLOGFULL  0x00
#define ERROR_LOGGING_SETUP_DATA_DEFAULT_SMBIOSERASE         0x00
#define ERROR_LOGGING_SETUP_DATA_DEFAULT_LOGEFISTATUSCODES   0x01
#define ERROR_LOGGING_SETUP_DATA_DEFAULT_METW                0x60
#define ERROR_LOGGING_SETUP_DATA_DEFAULT_MECI                0x01
#define ERROR_LOGGING_SETUP_DATA_DEFAULT_CONVERTEFITOLEGACY  0x00
#define ERROR_LOGGING_SETUP_DATA_DEFAULT_SELLOGGING          0x00
#define ERROR_LOGGING_SETUP_DATA_DEFAULT_SELERASE            0x00
#define ERROR_LOGGING_SETUP_DATA_DEFAULT_SELFULL             0x00
#define ERROR_LOGGING_SETUP_DATA_DEFAULT_LOGSELSTATUSCODES   0x01

#ifndef VFRCOMPILE
extern EFI_GUID gEfiServerMgmtSetupVariableGuid;
#endif

#define PLATFORM_SETUP_NVDATA_NVNAME  L"Setup"

#endif // #ifndef _EL_SETUP_VARIABLE_H_

