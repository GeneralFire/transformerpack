/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2007 Intel Corporation. <BR>

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

#ifndef _EFI_SCRIPT_H_
#define _EFI_SCRIPT_H_

#define EFI_ACPI_S3_RESUME_SCRIPT_TABLE         0x00

//
// Boot Script Opcode Definitions
//
typedef const UINT16  EFI_BOOT_SCRIPT_OPCODE;

#define EFI_BOOT_SCRIPT_IO_WRITE_OPCODE               0x00
#define EFI_BOOT_SCRIPT_IO_READ_WRITE_OPCODE          0x01
#define EFI_BOOT_SCRIPT_MEM_WRITE_OPCODE              0x02
#define EFI_BOOT_SCRIPT_MEM_READ_WRITE_OPCODE         0x03
#define EFI_BOOT_SCRIPT_PCI_CONFIG_WRITE_OPCODE       0x04
#define EFI_BOOT_SCRIPT_PCI_CONFIG_READ_WRITE_OPCODE  0x05
#define EFI_BOOT_SCRIPT_SMBUS_EXECUTE_OPCODE          0x06
#define EFI_BOOT_SCRIPT_STALL_OPCODE                  0x07
#define EFI_BOOT_SCRIPT_DISPATCH_OPCODE               0x08

//
// Extensions to boot script definitions
//
//#define EFI_BOOT_SCRIPT_MEM_POLL_OPCODE               0x09
#define EFI_BOOT_SCRIPT_INFORMATION_OPCODE            0x0A
#define EFI_BOOT_SCRIPT_PCI_CONFIG2_WRITE_OPCODE      0x0B
#define EFI_BOOT_SCRIPT_PCI_CONFIG2_READ_WRITE_OPCODE 0x0C

#define EFI_BOOT_SCRIPT_TABLE_OPCODE                  0xAA
#define EFI_BOOT_SCRIPT_TERMINATE_OPCODE              0xFF

#pragma pack(1)

//
// EFI Boot Script Width
//
/*typedef enum {
  EfiBootScriptWidthUint8,
  EfiBootScriptWidthUint16,
  EfiBootScriptWidthUint32,
  EfiBootScriptWidthUint64,
  EfiBootScriptWidthFifoUint8,
  EfiBootScriptWidthFifoUint16,
  EfiBootScriptWidthFifoUint32,
  EfiBootScriptWidthFifoUint64,
  EfiBootScriptWidthFillUint8,
  EfiBootScriptWidthFillUint16,
  EfiBootScriptWidthFillUint32,
  EfiBootScriptWidthFillUint64,
  EfiBootScriptWidthMaximum
} EFI_BOOT_SCRIPT_WIDTH;
*/

typedef struct {
  UINT16  OpCode;
  UINT8   Length;
} EFI_BOOT_SCRIPT_GENERIC_HEADER;

typedef struct {
  UINT16  OpCode;
  UINT8   Length;
  UINT16  Version;
  UINT32  TableLength;
  UINT16  Reserved[2];
} EFI_BOOT_SCRIPT_TABLE_HEADER;

typedef struct {
  UINT16                OpCode;
  UINT8                 Length;
  UINT32                Width;
} EFI_BOOT_SCRIPT_COMMON_HEADER;

typedef struct {
  UINT16                OpCode;
  UINT8                 Length;
  UINT32                Width;
  UINT32                Count;
  UINT64                Address;
} EFI_BOOT_SCRIPT_IO_WRITE;

typedef struct {
  UINT16                OpCode;
  UINT8                 Length;
  UINT32                Width;
  UINT64                Address;
} EFI_BOOT_SCRIPT_IO_READ_WRITE;

typedef struct {
  UINT16                OpCode;
  UINT8                 Length;
  UINT32                Width;
  UINT32                Count;
  UINT64                Address;
} EFI_BOOT_SCRIPT_MEM_WRITE;

typedef struct {
  UINT16                OpCode;
  UINT8                 Length;
  UINT32                Width;
  UINT64                Address;
} EFI_BOOT_SCRIPT_MEM_READ_WRITE;

typedef struct {
  UINT16                OpCode;
  UINT8                 Length;
  UINT32                Width;
  UINT32                Count;
  UINT64                Address;
} EFI_BOOT_SCRIPT_PCI_CONFIG_WRITE;

typedef struct {
  UINT16                OpCode;
  UINT8                 Length;
  UINT32                Width;
  UINT32                Count;
  UINT64                Address;
  UINT16                Segment;
} EFI_BOOT_SCRIPT_PCI_CONFIG2_WRITE;

typedef struct {
  UINT16                OpCode;
  UINT8                 Length;
  UINT32                Width;
  UINT64                Address;
} EFI_BOOT_SCRIPT_PCI_CONFIG_READ_WRITE;

typedef struct {
  UINT16                OpCode;
  UINT8                 Length;
  UINT32                Width;
  UINT64                Address;
  UINT16                Segment;
} EFI_BOOT_SCRIPT_PCI_CONFIG2_READ_WRITE;


typedef struct {
  UINT16                    OpCode;
  UINT8                     Length;
  UINT64                    SlaveAddress;
  UINT64                    Command;
  UINT32                    Operation;
  BOOLEAN                   PecCheck;
  UINT32                    DataSize;
} EFI_BOOT_SCRIPT_SMBUS_EXECUTE;

typedef struct {
  UINT16  OpCode;
  UINT8   Length;
  UINT64  Duration;
} EFI_BOOT_SCRIPT_STALL;

typedef struct {
  UINT16                OpCode;
  UINT8                 Length;
  EFI_PHYSICAL_ADDRESS  EntryPoint;
} EFI_BOOT_SCRIPT_DISPATCH;

typedef struct {
  UINT16  OpCode;
  UINT8   Length;
  UINT32  Width;
  UINT64  Address;
  UINT64  BitMask;
  UINT64  BitValue;
  UINT64  Duration;
  UINT64  LoopTimes;
} EFI_BOOT_SCRIPT_MEM_POLL;

typedef struct {
  UINT16                OpCode;
  UINT8                 Length;
  UINT32                InformationLength;
  EFI_PHYSICAL_ADDRESS  Information;
} EFI_BOOT_SCRIPT_INFORMATION;

typedef struct {
  UINT16  OpCode;
  UINT8   Length;
} EFI_BOOT_SCRIPT_TERMINATE;

typedef union {
  EFI_BOOT_SCRIPT_GENERIC_HEADER        *Header;
  EFI_BOOT_SCRIPT_TABLE_HEADER          *TableInfo;
  EFI_BOOT_SCRIPT_IO_WRITE              *IoWrite;
  EFI_BOOT_SCRIPT_IO_READ_WRITE         *IoReadWrite;
  EFI_BOOT_SCRIPT_MEM_WRITE             *MemWrite;
  EFI_BOOT_SCRIPT_MEM_READ_WRITE        *MemReadWrite;
  EFI_BOOT_SCRIPT_PCI_CONFIG_WRITE      *PciWrite;
  EFI_BOOT_SCRIPT_PCI_CONFIG_READ_WRITE *PciReadWrite;
  EFI_BOOT_SCRIPT_PCI_CONFIG2_WRITE      *PciWrite2;
  EFI_BOOT_SCRIPT_PCI_CONFIG2_READ_WRITE *PciReadWrite2;
  EFI_BOOT_SCRIPT_SMBUS_EXECUTE         *SmbusExecute;
  EFI_BOOT_SCRIPT_STALL                 *Stall;
  EFI_BOOT_SCRIPT_DISPATCH              *Dispatch;
  EFI_BOOT_SCRIPT_MEM_POLL              *MemPoll;
  EFI_BOOT_SCRIPT_INFORMATION           *Information;
  EFI_BOOT_SCRIPT_TERMINATE             *Terminate;
  EFI_BOOT_SCRIPT_COMMON_HEADER         *CommonHeader;
  UINT8                                 *Raw;
} BOOT_SCRIPT_POINTERS;

#pragma pack()

#endif
