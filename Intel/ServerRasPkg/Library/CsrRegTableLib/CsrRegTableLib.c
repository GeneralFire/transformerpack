/** @file
  CSR register table library provided a mechanism to collect BOOT_W policy group CSR write request in SMM,
  then write these CSRs in non-SMM boot phase.
  Security Attribute of the Initiator (SAI) is introduced into 10nm CPU,
  BOOT_W policy group CSR only has HOSTIA_BOOT_SAI security write access attribution,
  but doesn’t have HOSTIA_SMM_SAI, so can’t be written in SMM.

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

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Cpu/CpuCoreRegs.h>
#include "CsrRegTableLibInternal.h"

/**
  Detect IA Untrusted Mode.

  This function detects IA Untrusted Mode.

  @retval TRUE   IA Untrusted Mode - BOOT_W policy group CSR can’t be written in SMM.
  @retval FALSE  IA trusted Mode.

**/
BOOLEAN
IaUntrustedModeDetect (
  VOID
  )
{
  ICX_MSR_BIOS_DONE_REGISTER          BiosDone;

  //
  // Read MSR_BIOS_DONE
  //
  BiosDone.Uint32 = AsmReadMsr32 (ICX_MSR_BIOS_DONE);
  if (BiosDone.Bits.EnableIaUntrusted == 1) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Add an entry in CSR register table.

  This function adds an entry in CSR register table, with register MmioAddr, bit section and value.
  Note: WriteCsrRegTable() only can be used before End of DXE.
        WriteCsrRegTable() only accepts MMIO address, user needs to call GetCpuCsrAddress() to covert CSR crif address to MMIO address.

  @param  MmioAddr           MMIO address of the register to program
  @param  ValidBitStart      Start of the bit section
  @param  ValidBitLength     Length of the bit section
  @param  Value              Value to write

  @retval EFI_SUCCESS            add an entry in CSR register table successfully.
  @retval EFI_OUT_OF_RESOURCES   The memory pages could not be allocated.
  @retval EFI_UNSUPPORTED        WriteCsrRegTable () is unsupported.

**/
EFI_STATUS
EFIAPI
WriteCsrRegTableBitField (
  IN UINT64               MmioAddr,
  IN UINT16               ValidBitStart,
  IN UINT16               ValidBitLength,
  IN UINT32               Value
  )
{
  EFI_STATUS                Status;
  EFI_PHYSICAL_ADDRESS      Address;
  UINTN                     AllocatePages;
  CSR_REGISTER_TABLE        *CsrRegisterTable;
  CSR_REGISTER_TABLE_ENTRY  *CsrRegisterTableEntry;

  if (IaUntrustedModeDetect ()) {
    DEBUG((DEBUG_ERROR, "WriteCsrRegTable() can't be used after End of DXE!!!\n"));
    return EFI_UNSUPPORTED;
  }

  CsrRegisterTable = (CSR_REGISTER_TABLE *) (UINTN) PcdGet64 (PcdCsrRegTableAddress);
  //
  // If register table is full, allocate one more page for it.
  //
  if ((CsrRegisterTable->TableLength + 1) * sizeof (CSR_REGISTER_TABLE_ENTRY) > CsrRegisterTable->AllocatedSize) {
    Address = 0xffffffff;
    AllocatePages = EFI_SIZE_TO_PAGES (CsrRegisterTable->AllocatedSize);
    Status  = gBS->AllocatePages (
                     AllocateMaxAddress,
                     EfiBootServicesData,
                     AllocatePages + 1,
                     &Address
                     );

    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_ERROR, "Can't allocate enough memory space for register table!!!\n"));
      return EFI_OUT_OF_RESOURCES;
    }

    //
    // If there are records existing in the register table, then copy its contents
    // to new region and free the old one.
    //
    if (CsrRegisterTable->AllocatedSize > 0) {
      CopyMem (
        (VOID *) (UINTN) Address,
        (VOID *) (UINTN) CsrRegisterTable->CsrRegisterTableEntry,
        CsrRegisterTable->AllocatedSize
        );

      //
      // CsrRegisterTableEntry is allocated by gBS AllocatePages() service.
      // So, gBS FreePages() servcie is used to free it.
      //
      Status = gBS->FreePages (
                      (EFI_PHYSICAL_ADDRESS) (UINTN) CsrRegisterTable->CsrRegisterTableEntry,
                      AllocatePages
                      );
      ASSERT_EFI_ERROR (Status);
    }

    //
    // Adjust the allocated size and register table base address.
    //
    CsrRegisterTable->AllocatedSize += EFI_PAGE_SIZE;
    CsrRegisterTable->CsrRegisterTableEntry = (CSR_REGISTER_TABLE_ENTRY *) (UINTN) Address;
    DEBUG((DEBUG_ERROR, "PcdCsrRegTableAddress = 0x%x\n",PcdGet64 (PcdCsrRegTableAddress)));
    DEBUG((DEBUG_ERROR, "CsrRegisterTableEntry address = 0x%x\n",CsrRegisterTable->CsrRegisterTableEntry));
  }

  //
  // Append entry in the register table.
  //
  CsrRegisterTableEntry = CsrRegisterTable->CsrRegisterTableEntry;
  CsrRegisterTableEntry[CsrRegisterTable->TableLength].MmioAddr       = MmioAddr;
  CsrRegisterTableEntry[CsrRegisterTable->TableLength].ValidBitStart  = ValidBitStart;
  CsrRegisterTableEntry[CsrRegisterTable->TableLength].ValidBitLength = ValidBitLength;
  CsrRegisterTableEntry[CsrRegisterTable->TableLength].Value          = Value;

  CsrRegisterTable->TableLength++;

  return EFI_SUCCESS;
}

/**
  Add an entry in CSR register table.

  This function adds an entry in CSR register table, with register MmioAddr and value.
  Note: WriteCsrRegTable() only can be used before End of DXE.
        WriteCsrRegTable() only accepts MMIO address, user needs to call GetCpuCsrAddress() to covert CSR crif address to MMIO address.

  @param  MmioAddr           MMIO address of the register to program
  @param  ValidBitStart      Start of the bit section
  @param  ValidBitLength     Length of the bit section
  @param  Value              Value to write

  @retval EFI_SUCCESS            add an entry in CSR register table successfully.
  @retval EFI_OUT_OF_RESOURCES   The memory pages could not be allocated.
  @retval EFI_UNSUPPORTED        WriteCsrRegTable () is unsupported.

**/
EFI_STATUS
EFIAPI
WriteCsrRegTable (
  IN UINT64               MmioAddr,
  IN UINT32               Value
  )
{
  EFI_STATUS                Status;

  Status = WriteCsrRegTableBitField (MmioAddr, 0, 32, Value);
  return Status;
}

/**
  Programs registers according to register table.

  This function Programs CSR according to CSR register table.

**/
VOID
EFIAPI
SaiCsrInBatchWrite (
  VOID
  )
{
  CSR_REGISTER_TABLE        *CsrRegisterTable;
  CSR_REGISTER_TABLE_ENTRY  *RegisterTableEntry;
  UINTN                     Index;

  CsrRegisterTable = (CSR_REGISTER_TABLE *) (UINTN) PcdGet64 (PcdCsrRegTableAddress);
  RegisterTableEntry = (CSR_REGISTER_TABLE_ENTRY *) (UINTN) CsrRegisterTable->CsrRegisterTableEntry;

  for (Index = 0; Index < CsrRegisterTable->TableLength; Index++, RegisterTableEntry++) {
    //
    // MemoryMapped operations
    //
    MmioBitFieldWrite32 (
      (UINTN) RegisterTableEntry->MmioAddr,
      RegisterTableEntry->ValidBitStart,
      RegisterTableEntry->ValidBitStart + RegisterTableEntry->ValidBitLength - 1,
      RegisterTableEntry->Value
      );
  }

  //
  // Set PcdCsrRegTableAddress = 0 a to indicate CSR register table is not valid after End of DXE.
  //
  PcdSet64S (PcdCsrRegTableAddress, 0x00);
}

/**
  The constructor function of CSR Register Table Library.

  The constructor function allocates memory space for CSR register table structure.


  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
CsrRegTableLibConstructor (
  VOID
  )
{
  EFI_STATUS            Status;
  VOID*                 Buffer;
  UINT64                CsrRegTableAddress;

  //
  // If PcdCsrRegTableAddress = 0xFFFFFFFFFFFFFFFF, allocate memory space for CSR register table structure.
  //
  CsrRegTableAddress = PcdGet64 (PcdCsrRegTableAddress);
  if (CsrRegTableAddress == MAX_UINT64) {
    Status = gBS->AllocatePool (EfiBootServicesData, sizeof(CSR_REGISTER_TABLE), &Buffer);
    ASSERT_EFI_ERROR (Status);

    ZeroMem (Buffer, sizeof(CSR_REGISTER_TABLE));

    Status = PcdSet64S (PcdCsrRegTableAddress, (UINT64) Buffer);
  }

  return EFI_SUCCESS;
}