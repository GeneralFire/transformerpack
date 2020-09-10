/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2018 Intel Corporation. <BR>

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

#include <Uefi.h>
#include "Library/SmmScriptLib.h"


//
// internal functions
//
EFI_STATUS
BootScriptIoWrite (
  IN EFI_SMM_SCRIPT_TABLE     *ScriptTable,
  IN VA_LIST                  Marker
  );

EFI_STATUS
BootScriptPciCfgWrite (
  IN EFI_SMM_SCRIPT_TABLE     *ScriptTable,
  IN VA_LIST                  Marker
  );

EFI_STATUS
BootScriptMemWrite (
  IN EFI_SMM_SCRIPT_TABLE     *ScriptTable,
  IN VA_LIST                  Marker
  );

VOID
SmmCopyMem (
  IN  UINT8    *Destination,
  IN  UINT8    *Source,
  IN  UINTN    ByteCount
  );

//
// Function implementations
//
EFI_STATUS
EFIAPI
SmmBootScriptWrite (
  IN OUT   EFI_SMM_SCRIPT_TABLE        *ScriptTable,
  IN       UINTN                       Type,
  IN       UINT16                      OpCode,
  ...
  )
/*++

Routine Description:

  Write to boot script table. Format is defined in AcpiScritSave Protocol.
  Currently it supports MemWrite, IoWrite, PciCfgWrite.

Arguments:

  ScriptTable  -  Pointer to the script table to write to.
  Type         -  Not used.
  OpCode       -  Table write's Opcode.
  ...          -  Parameter of the OpCode.

Returns:

  EFI_INVALID_PARAMETER  -  Invalid parameter passed in detected.
  EFI_SUCCESS            -  Function completed successfully.
  Other                  -  Error occurred during execution.

--*/
{
  EFI_STATUS  Status;
  VA_LIST     Marker;

  if (ScriptTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Build script according to opcode
  //
  switch (OpCode) {

  case EFI_BOOT_SCRIPT_IO_WRITE_OPCODE:
    VA_START (Marker, OpCode);
    Status = BootScriptIoWrite (ScriptTable, Marker);
    VA_END (Marker);
    break;

  case EFI_BOOT_SCRIPT_PCI_CONFIG_WRITE_OPCODE:
    VA_START (Marker, OpCode);
    Status = BootScriptPciCfgWrite (ScriptTable, Marker);
    VA_END (Marker);
    break;

  case EFI_BOOT_SCRIPT_MEM_WRITE_OPCODE:
    VA_START (Marker, OpCode);
    Status = BootScriptMemWrite (ScriptTable, Marker);
    VA_END (Marker);
    break;

  default:
    Status = EFI_INVALID_PARAMETER;
    break;
  };

  return Status;
}

EFI_STATUS
EFIAPI
InitializeSmmScriptLib (
  IN  EFI_SYSTEM_TABLE     *SystemTable,
  IN  UINTN                SmmScriptTablePages,
  OUT EFI_PHYSICAL_ADDRESS *SmmScriptTableBase
  )
/*++

Routine Description:

  Intialize Boot Script table.

Arguments:

  SystemTable         -  Pointer to the EFI sytem table
  SmmScriptTablePages -  The expected ScriptTable page number
  SmmScriptTableBase  -  The returned ScriptTable base address

Returns:

  EFI_OUT_OF_RESOURCES   -  No resource to do the initialization.
  EFI_SUCCESS            -  Function has completed successfully.

--*/
{
  EFI_STATUS        Status;
  ACPI_VARIABLE_SET_COMPATIBILITY *AcpiVariableBase;
  UINTN             VarSize;
  UINT32            VarAttrib;

  //
  // Allocate runtime ACPI script table space. We need it to save some
  // settings done by CSM, which runs after normal script table closed
  //
  *SmmScriptTableBase = 0xFFFFFFFF;
  Status = SystemTable->BootServices->AllocatePages (
                                        AllocateMaxAddress,
                                        EfiReservedMemoryType,
                                        SmmScriptTablePages,
                                        SmmScriptTableBase
                                        );
  if (EFI_ERROR (Status)) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Save runtime script table base into global ACPI variable
  //
  // APTIOV_SERVER_OVERRIDE_RC_START : Remove NVRAM RT Attribute
  VarAttrib = EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE;
  // APTIOV_SERVER_OVERRIDE_RC_END : Remove NVRAM RT Attribute
  VarSize   = sizeof (UINTN);
  Status = SystemTable->RuntimeServices->GetVariable (
                                           ACPI_GLOBAL_VARIABLE,
                                           &gEfiAcpiVariableCompatiblityGuid,
                                           &VarAttrib,
                                           &VarSize,
                                           &AcpiVariableBase
                                           );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  AcpiVariableBase->RuntimeScriptTableBase = *SmmScriptTableBase;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SmmBootScriptCreateTable (
  IN OUT   EFI_SMM_SCRIPT_TABLE        *ScriptTable,
  IN       UINTN                       Type
  )
/*++

Routine Description:

  Create Boot Script table.

Arguments:

  ScriptTable  -  Pointer to the boot script table to create.
  Type         -  The type of table to creat.


Returns:

  EFI_INVALID_PARAMETER  -  Invalid parameter detected.
  EFI_SUCCESS            -  Function has completed successfully.

--*/
{
  BOOT_SCRIPT_POINTERS  Script;
  UINT8                 *Buffer;

  if (ScriptTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Buffer = (UINT8 *) ((UINTN) (*ScriptTable));

  //
  // Fill Table Header
  //
  Script.Raw                    = Buffer;
  Script.TableInfo->OpCode      = EFI_BOOT_SCRIPT_TABLE_OPCODE;
  Script.TableInfo->Length      = sizeof (EFI_BOOT_SCRIPT_TABLE_HEADER);
  Script.TableInfo->TableLength = sizeof (EFI_BOOT_SCRIPT_TABLE_HEADER);

  //
  // Update current table pointer
  //
  *ScriptTable = *ScriptTable + sizeof (EFI_BOOT_SCRIPT_TABLE_HEADER);
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SmmBootScriptCloseTable (
  IN EFI_SMM_SCRIPT_TABLE        ScriptTableBase,
  IN EFI_SMM_SCRIPT_TABLE        ScriptTablePtr,
  IN UINTN                       Type
  )
/*++

Routine Description:

  Close Boot Script table.

Arguments:

  ScriptTableBase  -  Pointer to the boot script table to create.
  ScriptTablePtr   -  Pointer to the script table to write to.
  Type             -  The type of table to creat.


Returns:

  EFI_SUCCESS            -  Function has completed successfully.

--*/
{
  BOOT_SCRIPT_POINTERS  Script;

  //
  // Add final "termination" node to script table
  //
  Script.Raw                = (UINT8 *) ((UINTN) ScriptTablePtr);
  Script.Terminate->OpCode  = EFI_BOOT_SCRIPT_TERMINATE_OPCODE;
  Script.Terminate->Length  = sizeof (EFI_BOOT_SCRIPT_TERMINATE);
  ScriptTablePtr += sizeof (EFI_BOOT_SCRIPT_TERMINATE);

  //
  // Update Table Header
  //
  Script.Raw                    = (UINT8 *) ((UINTN) ScriptTableBase);
  Script.TableInfo->OpCode      = EFI_BOOT_SCRIPT_TABLE_OPCODE;
  Script.TableInfo->Length      = sizeof (EFI_BOOT_SCRIPT_TABLE_HEADER);
  Script.TableInfo->TableLength = (UINT32) (ScriptTablePtr - ScriptTableBase);

  return EFI_SUCCESS;
}

EFI_STATUS
BootScriptIoWrite (
  IN EFI_SMM_SCRIPT_TABLE     *ScriptTable,
  IN VA_LIST                  Marker
  )
{
  BOOT_SCRIPT_POINTERS  Script;
  EFI_BOOT_SCRIPT_WIDTH Width;
  UINT64                Address;
  UINTN                 Count;
  UINT8                 *Buffer;
  UINTN                 NodeLength;
  UINT8                 WidthInByte;

  Width       = VA_ARG (Marker, EFI_BOOT_SCRIPT_WIDTH);
  Address     = VA_ARG (Marker, UINT64);
  Count       = VA_ARG (Marker, UINTN);
  Buffer      = VA_ARG (Marker, UINT8 *);

  WidthInByte = (UINT8) (0x01 << (Width & 0x03));
  Script.Raw  = (UINT8 *) ((UINTN) (*ScriptTable));
  NodeLength  = sizeof (EFI_BOOT_SCRIPT_IO_WRITE) + (WidthInByte * Count);

  //
  // Build script data
  //
  Script.IoWrite->OpCode  = EFI_BOOT_SCRIPT_IO_WRITE_OPCODE;
  Script.IoWrite->Length  = (UINT8) (NodeLength);
  Script.IoWrite->Width   = Width;
  Script.IoWrite->Address = Address;
  Script.IoWrite->Count   = (UINT32)Count;
  SmmCopyMem (
    (UINT8 *) (Script.Raw + sizeof (EFI_BOOT_SCRIPT_IO_WRITE)),
    Buffer,
    WidthInByte * Count
    );

  //
  // Update Script table pointer
  //
  *ScriptTable = *ScriptTable + NodeLength;
  return EFI_SUCCESS;
}

EFI_STATUS
BootScriptPciCfgWrite (
  IN EFI_SMM_SCRIPT_TABLE        *ScriptTable,
  IN VA_LIST                     Marker
  )
{
  BOOT_SCRIPT_POINTERS  Script;
  EFI_BOOT_SCRIPT_WIDTH Width;
  UINT64                Address;
  UINTN                 Count;
  UINT8                 *Buffer;
  UINTN                 NodeLength;
  UINT8                 WidthInByte;

  Width       = VA_ARG (Marker, EFI_BOOT_SCRIPT_WIDTH);
  Address     = VA_ARG (Marker, UINT64);
  Count       = VA_ARG (Marker, UINTN);
  Buffer      = VA_ARG (Marker, UINT8 *);

  WidthInByte = (UINT8) (0x01 << (Width & 0x03));
  Script.Raw  = (UINT8 *) ((UINTN) (*ScriptTable));
  NodeLength  = sizeof (EFI_BOOT_SCRIPT_PCI_CONFIG_WRITE) + (WidthInByte * Count);

  //
  // Build script data
  //
  Script.PciWrite->OpCode   = EFI_BOOT_SCRIPT_PCI_CONFIG_WRITE_OPCODE;
  Script.PciWrite->Length   = (UINT8) (NodeLength);
  Script.PciWrite->Width    = Width;
  Script.PciWrite->Address  = Address;
  Script.PciWrite->Count    = (UINT32)Count;
  SmmCopyMem (
    (UINT8 *) (Script.Raw + sizeof (EFI_BOOT_SCRIPT_PCI_CONFIG_WRITE)),
    Buffer,
    WidthInByte * Count
    );

  //
  // Update Script table pointer
  //
  *ScriptTable = *ScriptTable + NodeLength;
  return EFI_SUCCESS;
}

EFI_STATUS
BootScriptMemWrite (
  IN EFI_SMM_SCRIPT_TABLE          *ScriptTable,
  IN VA_LIST                       Marker
  )
{
  BOOT_SCRIPT_POINTERS  Script;
  EFI_BOOT_SCRIPT_WIDTH Width;
  UINT64                Address;
  UINTN                 Count;
  UINT8                 *Buffer;
  UINTN                 NodeLength;
  UINT8                 WidthInByte;

  Width       = VA_ARG (Marker, EFI_BOOT_SCRIPT_WIDTH);
  Address     = VA_ARG (Marker, UINT64);
  Count       = VA_ARG (Marker, UINTN);
  Buffer      = VA_ARG (Marker, UINT8 *);

  WidthInByte = (UINT8) (0x01 << (Width & 0x03));
  Script.Raw  = (UINT8 *) ((UINTN) (*ScriptTable));
  NodeLength  = sizeof (EFI_BOOT_SCRIPT_PCI_CONFIG_WRITE) + (WidthInByte * Count);

  //
  // Build script data
  //
  Script.MemWrite->OpCode   = EFI_BOOT_SCRIPT_MEM_WRITE_OPCODE;
  Script.MemWrite->Length   = (UINT8) (sizeof (EFI_BOOT_SCRIPT_MEM_WRITE) + (WidthInByte * Count));
  Script.MemWrite->Width    = Width;
  Script.MemWrite->Address  = Address;
  Script.MemWrite->Count    = (UINT32)Count;
  SmmCopyMem ((UINT8 *) (Script.Raw + sizeof (EFI_BOOT_SCRIPT_MEM_WRITE)), Buffer, WidthInByte * Count);

  *ScriptTable = *ScriptTable + NodeLength;

  return EFI_SUCCESS;
}

VOID
SmmCopyMem (
  IN  UINT8    *Destination,
  IN  UINT8    *Source,
  IN  UINTN    ByteCount
  )
{
  UINTN Index;

  for (Index = 0; Index < ByteCount; Index++, Destination++, Source++) {
    *Destination = *Source;
  }
}
