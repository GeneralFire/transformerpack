/** @file
  This is a public declarations of the Asynchronous Machine-check
  Error Injection(AMEI) support.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2019 Intel Corporation. <BR>

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

#ifndef _MCEJ_FILE_H_
#define _MCEJ_FILE_H_

//
// Includes
//
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Protocol/SmmSwDispatch2.h>

#include <Library/UefiLib.h>
#include <IndustryStandard/Acpi62.h>
#include <Protocol/AcpiTable.h>

#include <Library/RasDebugLib.h>
#include <Library/PlatPolicyLib.h>

#include <RasSwSmi.h>

#include <Cpu/Msr/CommonMsr.h>

//
// MCEJ Definitions
//
#define EFI_ACPI_MCEJ_SIGNATURE                     0x4A45434D    // "MCEI"
#define EFI_ACPI_MCEJ_REVISION                      0x00000001

//
// ACPI table information used to initialize tables.
//
#define EFI_ACPI_OEM_ID             'I', 'N', 'T', 'E', 'L', ' '        // OEMID 6 bytes long
#define EFI_ACPI_OEM_TABLE_ID       EFI_SIGNATURE_64 ('B', 'R', 'I', 'C', 'K', 'L', 'A', 'N') // OEM table id 8 bytes long
#define EFI_ACPI_OEM_REVISION       0x00000001
#define EFI_ACPI_CREATOR_ID         0x4C544E49      // "INTL"
#define EFI_ACPI_CREATOR_REVISION   0x0100000D
#define EFI_ACPI_TABLE_VERSION_ALL  ( \
  EFI_ACPI_TABLE_VERSION_1_0B |       \
  EFI_ACPI_TABLE_VERSION_2_0 |        \
  EFI_ACPI_TABLE_VERSION_3_0 |        \
  EFI_ACPI_TABLE_VERSION_4_0 |        \
  EFI_ACPI_TABLE_VERSION_5_0          \
  )
#define MCEJ_ACPI_VERSION_SUPPORT   (EFI_ACPI_TABLE_VERSION_ALL)

//
// MCA error injection MSRs
//

#define MCBANK_OVERWRITE_EN             0x01  // BIT0
#define MCA_CMCI_SIGNAL_EN              0x02  // BIT1

#define GENERATE_MCA                    0x01  // BIT0
#define GENERATE_CMCI                   0x02  // BIT1

//
// Commands passed by OS in the set command action entry of MCEJ table
//
#define CMD_MCBANK_OVERWRITE_EN                       0x01
#define CMD_MCA_CMCI_SIGNAL_EN                        0x02
#define CMD_GENERATE_MCA                              0x04
#define CMD_GENERATE_CMCI                             0x08

//
// Capabilities supported in the GET capabilities action entry of MCEJ table
//
#define MCA_CAP_SUPPORTED               0x01
#define CMCI_CAP_SUPPORTED              0x02

//
// MCA error injection action entry types
//
#define MCA_ERR_INJ_BEGIN_INJECT_OP                   0x00
#define MCA_ERR_INJ_GET_TRIGGER_ACTION_TABLE          0x01
#define MCA_ERR_INJ_GET_CAP                           0x02
#define MCA_ERR_INJ_SET_CMD                           0x03
#define MCA_ERR_INJ_EXECUTE_INJECT_OP                 0x04
#define MCA_ERR_INJ_END_INJECT_OP                     0x05
#define MCA_ERR_INJ_CHECK_BUSY_STATUS                 0x06
#define MCA_ERR_INJ_GET_CMD_STATUS                    0x07
#define MCA_ERR_INJ_TRIGGER_ERROR_ACTION              0xFF

//
// Error injection operation definitions
//
#define MCA_ERR_INJ_BEGIN_OPERATION                   0x55AA55AA
#define MCA_ERR_INJ_END_OPERATION                     0x00000000

//
// Error injection operation status
//
#define MCA_ERR_INJ_OPERATION_BUSY                    0x01

//
// Error injection command status
//
#define MCA_ERR_INJ_CMD_SUCCESS                       0x00
#define MCA_ERR_INJ_CMD_UNKNOWN_FAILURE               0x01
#define MCA_ERR_INJ_CMD_INVALID_ACCESS                0x02

#define INSTRUCTION_READ_REGISTER                     0x00
#define INSTRUCTION_READ_REGISTER_VALUE               0x01
#define INSTRUCTION_WRITE_REGISTER                    0x02
#define INSTRUCTION_WRITE_REGISTER_VALUE              0x03
#define INSTRUCTION_NO_OPERATION                      0x04

#define FLAG_NOTHING                                  0x00
#define FLAG_PRESERVE_REGISTER                        0x01
#define SW_SMI_PORT_ADDR                              0xb2 //APM_CNT Register

//
// Action entry count
//
#define EFI_ACPI_MCEJ_ACTION_ENTRY_COUNT        0x08

#define MCE_REGISTER_FILLER { \
  EFI_ACPI_6_2_SYSTEM_MEMORY,\
  64,                        \
  0,                         \
  EFI_ACPI_6_2_QWORD,        \
  0                          \
  }


#define MCEJ_ACTION_ITEM_EXECUTE { \
  EFI_ACPI_6_2_SYSTEM_IO,       \
  8,                            \
  0,                            \
  EFI_ACPI_6_2_BYTE,            \
  SW_SMI_PORT_ADDR              \
  }

#pragma pack (1)
//
// Parametre buffer used in updating the action entries of the MCEJ table.
//
typedef struct {
  UINT64                InjectionCap;
  UINT64                CmdValue;
  UINT64                TriggerActionTable;
  UINT64                CmdStatus;
  UINT64                BusyStatus;
  UINT64                OpState;
} MCA_ERR_INJ_PARAM_BUFFER;

//
// MCA Bank Injection Header in the MCEJ ACPI Table
//
typedef struct {
  UINT32                 InjectionHeaderSize;
  UINT32                 Resv1;
  UINT32                 InstructionEntryCount;
} EFI_ACPI_MCEJ_MCA_BANK_ERROR_INJECTION_HEADER;

//
// Trigger action table Header
//
typedef struct {
  UINT32                HeaderSize;
  UINT32                Revision;
  UINT32                TableSize;
  UINT32                EntryCount;
} EFI_ACPI_MCEJ_TRIGGER_ACTION_HEADER;

//
// MCEJ ACPI Table injection Action entry
//
typedef struct {
  UINT8                                   Operation;
  UINT8                                   Instruction;
  UINT8                                   Flags;
  UINT8                                   Reserved8;
  EFI_ACPI_6_2_GENERIC_ADDRESS_STRUCTURE  Register;
  UINT64                                  Value;
  UINT64                                  Mask;
} EFI_ACPI_MCEJ_ACTION_ITEM;

//
// MCEJ ACPI Trigger action entry table
//
typedef struct {
  EFI_ACPI_MCEJ_TRIGGER_ACTION_HEADER     Header;
  EFI_ACPI_MCEJ_ACTION_ITEM               Trigger[4];
} EFI_ACPI_MCEJ_TRIGGER_ACTION_TABLE;

//
// MCEJ ACPI Table
//
typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER                     Header;           // ACPI Standard Header
  EFI_ACPI_MCEJ_MCA_BANK_ERROR_INJECTION_HEADER   McBankInjHeader;  // MCA Bank Injection Header
  EFI_ACPI_MCEJ_ACTION_ITEM                       actionEntryList[EFI_ACPI_MCEJ_ACTION_ENTRY_COUNT];  // MCA bank Injection Action Table
} EFI_ACPI_MCEJ_TABLE;

#pragma pack ()

EFI_STATUS
EFIAPI
McejSwSmiCallBack (
  IN       EFI_HANDLE                     DispatchHandle,
  IN CONST EFI_SMM_SW_REGISTER_CONTEXT    *DispatchContext, OPTIONAL
  IN OUT   VOID                           *CommBuffer,     OPTIONAL
  IN OUT   UINTN                          *CommBufferSize  OPTIONAL
  );

#endif // _MCEJ_FILE_H_
