/** @file
  SMM driver for SW SMI handling of address translation DSM.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#include <PiSmm.h>
#include <IndustryStandard/Acpi.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/SmmSwDispatch2.h>
#include <Library/AddressDecodeLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/RasDebugLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <RasSwSmi.h>
#include "AddressTranslationDsm.h"

ADDRESS_TRANSLATION_BUFFER  *mBuffer = NULL;

/**
  Patch the SSDT to set the communication buffer address and length.

  @param[in, out] Table         The SSDT data to patch.

  @retval EFI_SUCCESS           The SSDT was successfully patched.
  @retval EFI_NOT_FOUND         The OpRegion was not found.
**/
EFI_STATUS
PatchSsdt (
  IN OUT  EFI_ACPI_DESCRIPTION_HEADER  *Table
  )
{
  EFI_STATUS                    Status;
  UINT8                         *DataStart;
  UINT8                         *DataEnd;
  UINT8                         *BytePointer;
  ADDRESS_TRANSLATION_OP_REGION *OpRegion;

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "[AddressTranslationDsm] Enter PatchSsdt\n"
    ));

  DataStart = (UINT8 *) (Table + 1);
  DataEnd = ((UINT8 *) Table) + Table->Length;

  Status = EFI_NOT_FOUND;

  //
  // Iterate through the data one byte at a time, but stop when the end of the
  // OpRegion starting at the current byte would be past the end of the data.
  //
  for (BytePointer = DataStart;
       BytePointer < DataEnd + 1 - sizeof (ADDRESS_TRANSLATION_OP_REGION);
       BytePointer++) {
    //
    // Search for the expected OpRegion.
    //
    OpRegion = (ADDRESS_TRANSLATION_OP_REGION *) BytePointer;
    if (OpRegion->ExtOpPrefix != AML_EXT_OP) {
      continue;
    }
    if (OpRegion->ExtOpCode != AML_EXT_REGION_OP) {
      continue;
    }
    if (OpRegion->NameString != SIGNATURE_32 ('B', 'U', 'F', 'F')) {
      continue;
    }
    if (OpRegion->RegionSpace != 0x00) {  // 0x00 = SystemMemory
      continue;
    }
    if (OpRegion->RegionOffset.DWordPrefix != AML_DWORD_PREFIX) {
      continue;
    }
    if (OpRegion->RegionLen.DWordPrefix != AML_DWORD_PREFIX) {
      continue;
    }

    //
    // Found it. Update the offset and length, and exit the loop.
    //
    OpRegion->RegionOffset.DWordData = (UINT32) (UINTN) mBuffer;
    OpRegion->RegionLen.DWordData = sizeof (ADDRESS_TRANSLATION_BUFFER);
    Status = EFI_SUCCESS;
    break;
  }

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "[AddressTranslationDsm] Exit PatchSsdt: %r\n",
    Status
    ));

  return Status;
} // PatchSsdt

/**
  Install the SSDT that implements the Address Translation DSM.

  @retval EFI_SUCCESS           The SSDT was successfully installed.
  @retval EFI_OUT_OF_RESOURCES  There is not enough memory.
**/
EFI_STATUS
InstallSsdt (
  VOID
  )
{
  EFI_STATUS                  Status;
  EFI_ACPI_TABLE_PROTOCOL     *AcpiTable;
  EFI_ACPI_DESCRIPTION_HEADER *Table;
  UINTN                       TableSize;
  UINTN                       TableKey;

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "[AddressTranslationDsm] Enter InstallSsdt\n"
    ));

  Status = gBS->LocateProtocol (
    &gEfiAcpiTableProtocolGuid,
    NULL,
    (VOID **) &AcpiTable
    );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = GetSectionFromFv (
    &gEfiCallerIdGuid,
    EFI_SECTION_RAW,
    0,
    (VOID **) &Table,
    &TableSize
    );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = PatchSsdt (Table);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  TableKey = 0;
  Status = AcpiTable->InstallAcpiTable (
                        AcpiTable,
                        Table,
                        TableSize,
                        &TableKey
                        );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "[AddressTranslationDsm] Exit InstallSsdt: %r\n",
    Status
    ));

  return Status;
} // InstallSsdt

/**
  SW SMI handler for the Address Translation DSM.

  @param[in]      DispatchHandle  The unique handle assigned to this handler.
  @param[in]      Context         Standard SMI handler parameter; not used here.
  @param[in, out] CommBuffer      Standard SMI handler parameter; not used here.
  @param[in, out] CommBufferSize  Standard SMI handler parameter; not used here.

  @retval EFI_SUCCESS             The interrupt was handled and quiesced.
**/
EFI_STATUS
EFIAPI
SwSmiHandler (
  IN EFI_HANDLE  DispatchHandle,
  IN CONST VOID  *Context         OPTIONAL,
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL
  )
{
  EFI_STATUS          Status;
  TRANSLATED_ADDRESS  TranslatedAddress;

  SetMem (&TranslatedAddress, sizeof (TranslatedAddress), 0xFF);

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "[AddressTranslationDsm] Enter SwSmiHandler\n"
    ));

  mBuffer->Status = ADDRESS_TRANSLATION_INVALID_COMMAND; // updated below

  if (mBuffer->Command == ADDRESS_TRANSLATION_FORWARD_ADDRESS_TRANSLATE) {
    Status = SystemAddressToDimmAddress (
      (UINTN) mBuffer->SystemAddress,
      &TranslatedAddress
      );
    if (EFI_ERROR (Status)) {
      mBuffer->Status = ADDRESS_TRANSLATION_INTERNAL_ERROR;
    } else {
      mBuffer->Status = ADDRESS_TRANSLATION_SUCCESS;
    }
  }

  if (mBuffer->Command == ADDRESS_TRANSLATION_REVERSE_ADDRESS_TRANSLATE) {
    TranslatedAddress.SystemAddress         = (UINT64) mBuffer->SystemAddress;
    TranslatedAddress.NmSystemAddress       = (UINT64) mBuffer->NmSystemAddress;
    TranslatedAddress.SpareSystemAddress    = (UINT64) mBuffer->SpareSystemAddress;
    TranslatedAddress.DPA                   = (UINT64) mBuffer->DevicePhysicalAddress;
    TranslatedAddress.SocketId              = (UINT8)  mBuffer->ProcessorSocketId;
    TranslatedAddress.MemoryControllerId    = (UINT8)  mBuffer->MemoryControllerId;
    TranslatedAddress.NmMemoryControllerId  = (UINT8)  mBuffer->NmMemoryControllerId;
    TranslatedAddress.TargetId              = (UINT8)  mBuffer->TargetId;
    TranslatedAddress.LogChannelId          = (UINT8)  mBuffer->LogicalChannelId;
    TranslatedAddress.ChannelAddress        = (UINT64) mBuffer->ChannelAddress;
    TranslatedAddress.NmChannelAddress      = (UINT64) mBuffer->NmChannelAddress;
    TranslatedAddress.ChannelId             = (UINT8)  mBuffer->ChannelId;
    TranslatedAddress.NmChannelId           = (UINT8)  mBuffer->NmChannelId;
    TranslatedAddress.RankAddress           = (UINT64) mBuffer->RankAddress;
    TranslatedAddress.NmRankAddress         = (UINT64) mBuffer->NmRankAddress;
    TranslatedAddress.PhysicalRankId        = (UINT8)  mBuffer->PhysicalRankId;
    TranslatedAddress.NmPhysicalRankId      = (UINT8)  mBuffer->NmPhysicalRankId;
    TranslatedAddress.DimmSlot              = (UINT8)  mBuffer->DimmSlotId;
    TranslatedAddress.NmDimmSlot            = (UINT8)  mBuffer->NmDimmSlotId;
    TranslatedAddress.DimmRank              = (UINT8)  mBuffer->DimmRankId;
    TranslatedAddress.Row                   = (UINT32) mBuffer->Row;
    TranslatedAddress.NmRow                 = (UINT32) mBuffer->NmRow;
    TranslatedAddress.Col                   = (UINT32) mBuffer->Column;
    TranslatedAddress.NmCol                 = (UINT32) mBuffer->NmCol;
    TranslatedAddress.Bank                  = (UINT8)  mBuffer->Bank;
    TranslatedAddress.NmBank                = (UINT8)  mBuffer->NmBank;
    TranslatedAddress.BankGroup             = (UINT8)  mBuffer->BankGroup;
    TranslatedAddress.NmBankGroup           = (UINT8)  mBuffer->NmBankGroup;
    TranslatedAddress.LockStepRank          = (UINT8)  mBuffer->LockStepRank;
    TranslatedAddress.LockStepPhysicalRank  = (UINT8)  mBuffer->LockStepPhysicalRank;
    TranslatedAddress.LockStepBank          = (UINT8)  mBuffer->LockStepBank;
    TranslatedAddress.LockStepBG            = (UINT8)  mBuffer->LockStepBankGroup;
    TranslatedAddress.ChipSelect            = (UINT8)  mBuffer->ChipSelect;
    TranslatedAddress.NmChipSelect          = (UINT8)  mBuffer->NmChipSelect;
    TranslatedAddress.Node                  = (UINT8)  mBuffer->Node;
    TranslatedAddress.ChipId                = (UINT8)  mBuffer->ChipId;
    TranslatedAddress.NmChipId              = (UINT8)  mBuffer->NmChipId;
    Status = DimmAddressToSystemAddress (&TranslatedAddress);
    if (EFI_ERROR (Status)) {
      if (TranslatedAddress.StatusToDsmInterface != 0) {
        mBuffer->Status = TranslatedAddress.StatusToDsmInterface;
      } else {
        mBuffer->Status = ADDRESS_TRANSLATION_INTERNAL_ERROR;
      }
    } else {
      mBuffer->Status = ADDRESS_TRANSLATION_SUCCESS;
    }
  }

  if (mBuffer->Status == ADDRESS_TRANSLATION_SUCCESS) {
    mBuffer->SystemAddress         = EXTEND_PARAM (UINT64, TranslatedAddress.SystemAddress);
    mBuffer->NmSystemAddress       = EXTEND_PARAM (UINT64, TranslatedAddress.NmSystemAddress);
    mBuffer->SpareSystemAddress    = EXTEND_PARAM (UINT64, TranslatedAddress.SpareSystemAddress);
    mBuffer->DevicePhysicalAddress = EXTEND_PARAM (UINT64, TranslatedAddress.DPA);
    mBuffer->ProcessorSocketId     = EXTEND_PARAM (UINT8,  TranslatedAddress.SocketId);
    mBuffer->MemoryControllerId    = EXTEND_PARAM (UINT8,  TranslatedAddress.MemoryControllerId);
    mBuffer->NmMemoryControllerId  = EXTEND_PARAM (UINT8,  TranslatedAddress.NmMemoryControllerId);
    mBuffer->TargetId              = EXTEND_PARAM (UINT8,  TranslatedAddress.TargetId);
    mBuffer->LogicalChannelId      = EXTEND_PARAM (UINT8,  TranslatedAddress.LogChannelId);
    mBuffer->ChannelAddress        = EXTEND_PARAM (UINT64, TranslatedAddress.ChannelAddress);
    mBuffer->NmChannelAddress      = EXTEND_PARAM (UINT64, TranslatedAddress.NmChannelAddress);
    mBuffer->ChannelId             = EXTEND_PARAM (UINT8,  TranslatedAddress.ChannelId);
    mBuffer->NmChannelId           = EXTEND_PARAM (UINT8,  TranslatedAddress.NmChannelId);
    mBuffer->RankAddress           = EXTEND_PARAM (UINT64, TranslatedAddress.RankAddress);
    mBuffer->NmRankAddress         = EXTEND_PARAM (UINT64, TranslatedAddress.NmRankAddress);
    mBuffer->PhysicalRankId        = EXTEND_PARAM (UINT8,  TranslatedAddress.PhysicalRankId);
    mBuffer->NmPhysicalRankId      = EXTEND_PARAM (UINT8,  TranslatedAddress.NmPhysicalRankId);
    mBuffer->DimmSlotId            = EXTEND_PARAM (UINT8,  TranslatedAddress.DimmSlot);
    mBuffer->NmDimmSlotId          = EXTEND_PARAM (UINT8,  TranslatedAddress.NmDimmSlot);
    mBuffer->DimmRankId            = EXTEND_PARAM (UINT8,  TranslatedAddress.DimmRank);
    mBuffer->Row                   = EXTEND_PARAM (UINT32, TranslatedAddress.Row);
    mBuffer->NmRow                 = EXTEND_PARAM (UINT32, TranslatedAddress.NmRow);
    mBuffer->Column                = EXTEND_PARAM (UINT32, TranslatedAddress.Col);
    mBuffer->NmCol                 = EXTEND_PARAM (UINT32, TranslatedAddress.NmCol);
    mBuffer->Bank                  = EXTEND_PARAM (UINT8,  TranslatedAddress.Bank);
    mBuffer->NmBank                = EXTEND_PARAM (UINT8,  TranslatedAddress.NmBank);
    mBuffer->BankGroup             = EXTEND_PARAM (UINT8,  TranslatedAddress.BankGroup);
    mBuffer->NmBankGroup           = EXTEND_PARAM (UINT8,  TranslatedAddress.NmBankGroup);
    mBuffer->LockStepRank          = EXTEND_PARAM (UINT8,  TranslatedAddress.LockStepRank);
    mBuffer->LockStepPhysicalRank  = EXTEND_PARAM (UINT8,  TranslatedAddress.LockStepPhysicalRank);
    mBuffer->LockStepBank          = EXTEND_PARAM (UINT8,  TranslatedAddress.LockStepBank);
    mBuffer->LockStepBankGroup     = EXTEND_PARAM (UINT8,  TranslatedAddress.LockStepBG);
    mBuffer->ChipSelect            = EXTEND_PARAM (UINT8,  TranslatedAddress.ChipSelect);
    mBuffer->NmChipSelect          = EXTEND_PARAM (UINT8,  TranslatedAddress.NmChipSelect);
    mBuffer->Node                  = EXTEND_PARAM (UINT8,  TranslatedAddress.Node);
    mBuffer->ChipId                = EXTEND_PARAM (UINT8,  TranslatedAddress.ChipId);
    mBuffer->NmChipId              = EXTEND_PARAM (UINT8,  TranslatedAddress.NmChipId);
  }

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "[AddressTranslationDsm] Exit SwSmiHandler\n"
    ));
  return EFI_SUCCESS;
}

/**
  SW SMI handler for the Address Translation DSM.

  @retval EFI_SUCCESS           The SW SMI was successfully registered.
  @retval EFI_UNSUPPORTED       The SW SMI was invalid or in use.
  @retval EFI_DEVICE_ERROR      The SW SMI could not be enabled.
  @retval EFI_OUT_OF_RESOURCES  There is not enough memory.
**/
EFI_STATUS
RegisterSwSmi (
  VOID
  )
{
  EFI_STATUS                    Status;
  EFI_SMM_SW_REGISTER_CONTEXT   SwContext;
  EFI_SMM_SW_DISPATCH2_PROTOCOL *SwDispatch;
  EFI_HANDLE                    SwSmiHandle;

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "[AddressTranslationDsm] Enter RegisterSwSmi\n"
    ));

  Status = gSmst->SmmLocateProtocol (
    &gEfiSmmSwDispatch2ProtocolGuid,
    NULL,
    &SwDispatch
    );
  ASSERT_EFI_ERROR (Status);

  if (!EFI_ERROR (Status)) {
    SwContext.SwSmiInputValue = RAS_SW_SMI_ADDRESS_TRANSLATION_DSM;
    ASSERT (SwContext.SwSmiInputValue < SwDispatch->MaximumSwiValue);

    Status = SwDispatch->Register (
      SwDispatch,
      SwSmiHandler,
      &SwContext,
      &SwSmiHandle
      );
    ASSERT_EFI_ERROR (Status);

    ASSERT (SwDispatch->MaximumSwiValue <= MAX_UINT32);
    mBuffer->SwSmi = (UINT32) SwContext.SwSmiInputValue;

    //
    // Change "invalid parameter" into "unsupported" for return.
    //
    if (Status == EFI_INVALID_PARAMETER) {
      Status = EFI_UNSUPPORTED;
    }
  }

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "[AddressTranslationDsm] Exit RegisterSwSmi: %r\n",
    Status
    ));

  return Status;
} // RegisterSwSmi

/**
  The driver entry point.

  @param[in]  ImageHandle       The firmware allocated handle for the EFI image.
  @param[in]  SystemTable       A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The entry point executed successfully.
  @retval EFI_UNSUPPORTED       One or more SW SMIs are invalid or in use.
  @retval EFI_DEVICE_ERROR      One or more SW SMIs could not be enabled.
  @retval EFI_OUT_OF_RESOURCES  There is not enough memory to register SW SMIs.
--*/
EFI_STATUS
EFIAPI
AddressTranslationDsmEntry (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS  Status;

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "[AddressTranslationDsm] Enter AddressTranslationDsmEntry\n"
    ));

  Status = gBS->AllocatePool (
    EfiReservedMemoryType,
    sizeof (ADDRESS_TRANSLATION_BUFFER),
    &mBuffer
    );
  ASSERT_EFI_ERROR (Status);

  if (!EFI_ERROR (Status)) {
    Status = RegisterSwSmi ();
  }

  if (!EFI_ERROR (Status)) {
    Status = InstallSsdt ();
  }

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "[AddressTranslationDsm] Exit AddressTranslationDsmEntry: %r\n",
    Status
    ));

  return Status;
}
