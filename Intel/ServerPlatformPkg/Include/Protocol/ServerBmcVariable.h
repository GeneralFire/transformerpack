/** @file
  This File abstracts all the Server related Baseboard Controller Variables.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#ifndef _SM_SERVER_BMC_VARIABLE_H_
#define _SM_SERVER_BMC_VARIABLE_H_

#define EFI_SM_VARIABLES_PROTOCOL_GUID \
  { \
    0x5b525f29, 0x827d, 0x4ca3, 0xbe, 0xb5, 0x89, 0x54, 0xb5, 0x6c, 0x5, 0x73 \
  }

typedef struct _EFI_SM_VARIABLES_PROTOCOL EFI_SM_VARIABLES_PROTOCOL;

//
// System Event Log Class Variables
//
typedef struct {
  EFI_SM_ELOG_TYPE  SelType;
  BOOLEAN           ClearLog;
  BOOLEAN           SelStatus;
} EFI_SM_SEL_VARIABLES;

//
// Power Restore Class Variables
//
typedef enum {
  IpmiPowerOff,
  IpmiLastState,
  IpmiPowerOn,
  IpmiGetPresentPolicy
} EFI_POWER_RESTORE_POLICY;

typedef struct {
  EFI_POWER_RESTORE_POLICY  AcPowerRestorePolicy;
} EFI_POWER_RESTORE_VARIABLES;

//
// BMC Controller
//
typedef struct {
  EFI_SM_CTRL_INFO  DeviceId;
} EFI_SM_BMC_FRU;

typedef struct {
  UINT16  *BoardPartNo;
  UINT16  *BoardSerialNo;
  UINT16  *SystemPartNo;
  UINT16  *SystemSerialNo;
  UINT16  *ChassisPartNo;
  UINT16  *ChassisSerialNo;
  UINT16  *AssetTagNo;
} EFI_SM_SYSTEM_FRU;

typedef struct {
  UINT16  *SdrVersion;
} EFI_SM_SYSTEM_SDR;

//
// Hotswap Controller
//
typedef struct {
  UINT8 SerialNumber[16];
  UINT8 MajorFwRevID;
  UINT8 MinorFwRevID;
} EFI_SM_HOTSWAP_FRU;

//
// Serial Over Lan variable
//
typedef enum {
  BiosBaud9600,
  BiosBaud19200,
  BiosBaud38400,
  BiosBaud57600,
  BiosBaud115200
} EFI_BIOS_BAUD_RATE;

typedef struct {
  EFI_BIOS_BAUD_RATE  BaudRate;
} EFI_SM_SOL_VARIABLE;

//
// Management Engine Firmware
//
typedef struct {
  UINT8 MajorFwRevID;
  UINT8 MinorFwRevID;
  UINT8 BuildFwRevID;
  UINT8 PatchFwRevID;
  UINT8 MajorProductID;
  UINT8 MinorProductID;
} EFI_SM_ME_FW_VARIABLE;

//
// Server Management Variable types
//
typedef enum {
  EfiSelVariable,
  EfiBmcFruVariable,
  EfiHotSwapFruVariable,
  EfiSystemFruVariable,
  EfiSystemSdrVariable,
  EfiPowerRestoreVariable,
  EfiSolVariable,
  EfiMeFwVariable
} EFI_SM_VARIABLE_CLASS;

//
// One variable at a time (a union).
//
typedef union {
  EFI_SM_SEL_VARIABLES        SelVariable;
  EFI_SM_BMC_FRU              BmcFruVariable;
  EFI_SM_HOTSWAP_FRU          HotSwapFruVariable;
  EFI_SM_SYSTEM_FRU           SystemFruVariable;
  EFI_SM_SYSTEM_SDR           SystemSdrVariable;
  EFI_POWER_RESTORE_VARIABLES PowerRestoreVariable;
  EFI_SM_SOL_VARIABLE         SolVariable;
  EFI_SM_ME_FW_VARIABLE       MeFwVariable;
} EFI_SM_VARIABLE;

//
// All variables (a structure).
//
typedef struct {
  EFI_SM_SEL_VARIABLES        SelVariable;
  EFI_SM_BMC_FRU              BmcFruVariable;
  EFI_SM_HOTSWAP_FRU          HotSwapFruVariable;
  EFI_SM_SYSTEM_FRU           SystemFruVariable;
  EFI_SM_SYSTEM_SDR           SystemSdrVariable;
  EFI_POWER_RESTORE_VARIABLES PowerRestoreVariable;
  EFI_SM_SOL_VARIABLE         SolVariable;
} EFI_SM_VARIABLES;

//
// Server Management Variable Protocol
//
typedef
EFI_STATUS
(EFIAPI *EFI_SM_SET_VARIABLES) (
  IN  EFI_SM_VARIABLES_PROTOCOL            *This,
  IN  EFI_SM_VARIABLE_CLASS                VariableClass,
  IN  UINTN                                VariableInstance,
  IN  VOID                                 *VariableData
  );

typedef
EFI_STATUS
(EFIAPI *EFI_SM_GET_VARIABLES) (
  IN  EFI_SM_VARIABLES_PROTOCOL           *This,
  IN  EFI_SM_VARIABLE_CLASS               VariableClass,
  IN  UINTN                               VariableInstance,
  OUT VOID                                *VariableData
  );

typedef
EFI_STATUS
(EFIAPI *EFI_SM_EXIT_SETUP) (
  IN  EFI_SM_VARIABLES_PROTOCOL           *This
  );

//
// PROTOCOL
//
struct _EFI_SM_VARIABLES_PROTOCOL {
  EFI_SM_GET_VARIABLES  GetVariable;
  EFI_SM_SET_VARIABLES  SetVariable;
  EFI_SM_EXIT_SETUP     ExitSetup;
};

extern EFI_GUID gEfiSmBmcVariableGuid;

#endif
