/** @file
  This is an implementation of the Asynchronous Machine-check
  Error Injection(AMEI) support using MCEJ ACPI table.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2016 Intel Corporation. <BR>

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

#include "McBankErrorInjection.h"


EFI_ACPI_TABLE_PROTOCOL             *mAcpiDrv = NULL;
MCA_ERR_INJ_PARAM_BUFFER            *mMcejParam = NULL;
EFI_ACPI_MCEJ_TRIGGER_ACTION_TABLE  *mMcaInjTriggerAction = NULL;

//
// MCEJ AcpiTable
//
EFI_ACPI_MCEJ_TABLE mMcejTable = \
{
  //
  // ACPI Standard Header
  //
  {
    EFI_ACPI_MCEJ_SIGNATURE,          // Header Signature
    sizeof (EFI_ACPI_MCEJ_TABLE),     // Length
    EFI_ACPI_MCEJ_REVISION,           // Revision
    0x00,                             // CheckSum
    { EFI_ACPI_OEM_ID },                  // OemId
    0,                                // OemTableId
    EFI_ACPI_OEM_REVISION,            // OemRevision
    EFI_ACPI_CREATOR_ID,              // CreaterId
    EFI_ACPI_CREATOR_REVISION         // CreaterRevision
  },
  //
  // MCA Bank Injection Header
  //
  {
    sizeof (EFI_ACPI_MCEJ_MCA_BANK_ERROR_INJECTION_HEADER),  //InjectionHeaderSize;
    0x00,                                                   //Resv1;
    EFI_ACPI_MCEJ_ACTION_ENTRY_COUNT                       //InstructionEntryCount;
  },
  //
  // MCA bank Injection Action Table
  //
  {
    {   //Action0
      MCA_ERR_INJ_BEGIN_INJECT_OP,        // BEGIN_INJECTION_OPERATION
      INSTRUCTION_WRITE_REGISTER_VALUE,   // WRITE_REGISTER_VALUE
      FLAG_PRESERVE_REGISTER,             // Flags
      0x00,                               // Reserved
      MCE_REGISTER_FILLER,                 // {0x00, 0x20, 0x00, 0x03, -1},    // GAS (DWORD Memory) Address will be filled during boot
      MCA_ERR_INJ_BEGIN_OPERATION,        // Value for InjectError()
      0xffffffff                          // Mask is only valid for 32-bits
    },
    {   //Action1
      MCA_ERR_INJ_GET_TRIGGER_ACTION_TABLE, // GET_TRIGGER_ERROR_STRUC
      INSTRUCTION_READ_REGISTER,          // READ_REGISTER
      FLAG_NOTHING,                       // Flags
      0x00,                               // Reserved
      MCE_REGISTER_FILLER,                 // {0x00, 0x40, 0x00, 0x04, -1},    // GAS (QWORD Memory) Address will be filled during boot
      0,                                  // Value for InjectError()
      0xffffffffffffffff                  // Mask is only valid for 32-bits
    },
    {   //Action2
      MCA_ERR_INJ_GET_CAP,                //  GET_ERROR_TYPE
      INSTRUCTION_READ_REGISTER,          // READ_REGISTER
      FLAG_NOTHING,                       // Flags
      0x00,                               // Reserved
      MCE_REGISTER_FILLER,                 //{0x00, 0x20, 0x00, 0x03, -1},    // GAS (QWORD Memory) Address will be filled during boot
      0,                                  // Value for InjectError()
      0xffffffff                          // Mask is only valid for 32-bits
    },
    {   //Action3
      MCA_ERR_INJ_SET_CMD,                // SET_CMD
      INSTRUCTION_WRITE_REGISTER,         // WRITE_REGISTER
      FLAG_PRESERVE_REGISTER,             // Flags
      0x00,                               // Reserved
      MCE_REGISTER_FILLER,                 // {0x00, 0x20, 0x00, 0x03, -1},    // GAS (QWORD Memory) Address will be filled during boot
      0,                                  // Value for InjectError()
      0xffffffff                          // Mask is only valid for 32-bits
    },
    {   //Action4
      MCA_ERR_INJ_EXECUTE_INJECT_OP,      // EXECUTE_OPERATION
      INSTRUCTION_WRITE_REGISTER_VALUE,   // WRITE_REGISTER_VALUE
      FLAG_PRESERVE_REGISTER,             // Flags
      0x00,                               // Reserved
      MCEJ_ACTION_ITEM_EXECUTE,              // GAS (BYTE IO). Address will be filled in runtime
      RAS_SW_SMI_MCEJ_EXECUTE,            // Value for InjectError()
      0xff                                // Mask is only valid for 16-bits
    },
    {   //Action5
      MCA_ERR_INJ_END_INJECT_OP,          // END_OPERATION
      INSTRUCTION_WRITE_REGISTER_VALUE,   // WRITE_REGISTER_VALUE
      FLAG_PRESERVE_REGISTER,             // Flags
      0x00,                               // Reserved
      MCE_REGISTER_FILLER,                 //{0x00, 0x20, 0x00, 0x03, -1},    // GAS (QWORD Memory) Address will be filled during boot
      MCA_ERR_INJ_END_OPERATION,          // Value for InjectError()
      0xffffffff                          // Mask is only valid for 32-bits
    },
    {   //Action6
      MCA_ERR_INJ_CHECK_BUSY_STATUS,      // CHECK_BUSY_STATUS
      INSTRUCTION_READ_REGISTER_VALUE,    // READ_REGISTER
      FLAG_NOTHING,                       // Flags
      0x00,                               // Reserved
      MCE_REGISTER_FILLER,                 // {0x00, 0x30, 0x00, 0x03, -1},    // GAS (QWORD Memory) Address will be filled during boot
      0x00000001,                         // Value for InjectError()
      0x00000001                          // Mask is only valid for 32-bits
    },
    {   //Action7
      MCA_ERR_INJ_GET_CMD_STATUS,         // GET_OPERATION_STATUS
      INSTRUCTION_READ_REGISTER,          // READ_REGISTER
      FLAG_PRESERVE_REGISTER,             // Flags
      0x00,                               // Reserved
      MCE_REGISTER_FILLER,                 // {0x00, 0x40, 0x00, 0x03, -1},    // GAS (QWORD Memory) Address will be filled during boot
      0,                                  // Value for InjectError()
      0x000001fe                          // Mask is only valid for 32-bits
    }
  }
};

//
// Trigger Action table executed by OS driver.
//
EFI_ACPI_MCEJ_TRIGGER_ACTION_TABLE mTriggerActionTable = \
{
  {
    sizeof (EFI_ACPI_MCEJ_TRIGGER_ACTION_HEADER),
    0,
    sizeof (EFI_ACPI_MCEJ_TRIGGER_ACTION_TABLE),
    4
  },
  {
    { //Action0
      MCA_ERR_INJ_TRIGGER_ERROR_ACTION,
      INSTRUCTION_NO_OPERATION,         // Intialized as NOP. To be filled by the runtime injection code
      FLAG_NOTHING,                     // Flags
      0x00,                             // Reserved
      MCE_REGISTER_FILLER,               // {0x00, 0x20, 0x00, 0x03, -1},    // GAS (DWORD Memory) Address will be filled during boot
      0,                                // Value for InjectError()
      0xffffffffffffffff                // Mask is only valid for 32-bits
    },
    { //Action1
      MCA_ERR_INJ_TRIGGER_ERROR_ACTION,
      INSTRUCTION_NO_OPERATION,         // Intialized as NOP. To be filled by the runtime injection code
      FLAG_NOTHING,                     // Flags
      0x00,                             // Reserved
      MCE_REGISTER_FILLER,               // {0x00, 0x20, 0x00, 0x03, -1},    // GAS (DWORD Memory) Address will be filled during boot
      0,                                // Value for InjectError()
      0xffffffff                        // Mask is only valid for 32-bits
    },
    { //Action2
      MCA_ERR_INJ_TRIGGER_ERROR_ACTION,
      INSTRUCTION_NO_OPERATION,         // Intialized as NOP. To be filled by the runtime injection code
      FLAG_NOTHING,                     // Flags
      0x00,                             // Reserved
      MCE_REGISTER_FILLER,               // {0x00, 0x20, 0x00, 0x03, -1},    // GAS (DWORD Memory) Address will be filled during boot
      0,                                // Value for InjectError()
      0xffffffff                        // Mask is only valid for 32-bits
    },
    { //Action3
      MCA_ERR_INJ_TRIGGER_ERROR_ACTION,
      INSTRUCTION_NO_OPERATION,         // Write register
      FLAG_NOTHING,                     // Flags
      0x00,                             // Reserved
      MCE_REGISTER_FILLER,               // {0x00, 0x20, 0x00, 0x03, -1},    // GAS (DWORD Memory) Address will be filled during boot
      0,                                // Value for InjectError()
      0xffffffff                        // Mask is only valid for 32-bits
    }
  }
};

/**

  This function creates the MCEJ ACPI table entires
  @param i/p : pointer to the McejTable

  @retval Success if MCEJ ACPI table is published else Failure

**/
VOID
CreateMcejAcpiTable (
  EFI_ACPI_MCEJ_TABLE   *McejTable
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;
  UINT8           *Ests = NULL;

  Status = gBS->AllocatePool (
    EfiReservedMemoryType,
    (sizeof (MCA_ERR_INJ_PARAM_BUFFER) + \
    sizeof (EFI_ACPI_MCEJ_TRIGGER_ACTION_TABLE) + 0x80),
    (VOID **) &Ests
    );
  ASSERT_EFI_ERROR (Status);

  ZeroMem (Ests, (sizeof (MCA_ERR_INJ_PARAM_BUFFER) + sizeof (EFI_ACPI_MCEJ_TRIGGER_ACTION_TABLE) + 0x80));

  //
  // Init buffer pointers and data for Einj parameter and Error log address range.
  //
  mMcejParam = (MCA_ERR_INJ_PARAM_BUFFER *) Ests;
  mMcaInjTriggerAction = (EFI_ACPI_MCEJ_TRIGGER_ACTION_TABLE *) (Ests + ((sizeof (MCA_ERR_INJ_PARAM_BUFFER) + 0x3F) / 0x40) * 0x40);

  CopyMem (mMcaInjTriggerAction, &mTriggerActionTable, sizeof (EFI_ACPI_MCEJ_TRIGGER_ACTION_TABLE));

  //
  // Update Mcej parameters to be used in the MCEJ action structures.
  //
  mMcejParam->TriggerActionTable    = (UINTN) mMcaInjTriggerAction;
  mMcejParam->InjectionCap          = MCA_CAP_SUPPORTED | CMCI_CAP_SUPPORTED;
  mMcejParam->OpState               = 0;
  mMcejParam->BusyStatus            = 0;
  mMcejParam->CmdStatus             = 0;
  mMcejParam->CmdValue              = 0;

  //
  // Fill the MCEJ action entries.
  //
  McejTable->actionEntryList[0].Register.Address = (UINTN) &mMcejParam->OpState;              // Update address for BEGIN_OPERATION
  McejTable->actionEntryList[1].Register.Address = (UINTN) &mMcejParam->TriggerActionTable;   // Update address for GET_TRIGGER_ERROR_ACTION_TABLE
  McejTable->actionEntryList[2].Register.Address = (UINTN) &mMcejParam->InjectionCap;         // Update address for GET_CAPABLILITIES
  McejTable->actionEntryList[3].Register.Address = (UINTN) &mMcejParam->CmdValue;             // Update address for SET_COMMAND
  McejTable->actionEntryList[5].Register.Address = (UINTN) &mMcejParam->OpState;              // Update address for END_OPERATION
  McejTable->actionEntryList[6].Register.Address = (UINTN) &mMcejParam->BusyStatus;           // Update address for CHECK_BUSY_STATUS
  McejTable->actionEntryList[7].Register.Address = (UINTN) &mMcejParam->CmdStatus;            // Update address for GET_CMD_STATUS
  //McejTable->actionEntryList[8].Register.Address = (UINTN) &mMcejParam;                     // Debug entry

}

/**

  This function publishes the MCEJ ACPI table
   @param i/p : pointer to the McejTable

  @retval Success if MCEJ ACPI table is published else Failure

**/
STATIC
EFI_STATUS
SetMcejAcpiTable (
  EFI_ACPI_MCEJ_TABLE   *McejTable
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;
  UINTN           TabHand = 0;

  mAcpiDrv = NULL;

  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, &mAcpiDrv);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = mAcpiDrv->InstallAcpiTable (
    mAcpiDrv,
    McejTable,
    McejTable->Header.Length,
    &TabHand
    );

  return Status;
}

/**

  This function creates and publishes the MCEJ ACPI table.

   @param none

  @retval Success if MCEJ ACPI table is published else Failure


**/
EFI_STATUS
UpdateMcejTable (
  VOID
  )
{
  EFI_STATUS    Status = EFI_SUCCESS;

  CreateMcejAcpiTable (&mMcejTable);

  Status = SetMcejAcpiTable (&mMcejTable);

  return Status;
}

/**

    Return True if AMEI supported

    @param None

    @retval AMEI - TRUE  if its supported
                 - FALSE if not supported

**/
UINT32
EFIAPI
GetAMEISupp (
  VOID
  )
{
  MSR_SMM_MCA_CAP_REGISTER SmmMcaCapReg;

  SmmMcaCapReg.Uint64 = AsmReadMsr64 (MSR_SMM_MCA_CAP);

  return (BOOLEAN) (SmmMcaCapReg.Bits.ErrorSpoofing);
}

/**

  Entry point of the Asynchronous Machine-check Error Injection support.

  @param ImageHandle   EFI_HANDLE: A handle for the image that is initializing this driver
  @param SystemTable   EFI_SYSTEM_TABLE: A pointer to the EFI system table

  @retval EFI_SUCCESS:              Driver initialized successfully
  @retval EFI_LOAD_ERROR:           Failed to Initialize or has been loaded
  @retval EFI_OUT_OF_RESOURCES:     Could not allocate needed resources

**/
EFI_STATUS
EFIAPI
McejSmmEntry (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  RAS_POLICY                        *PolicyData;
  EFI_STATUS                        Status = EFI_SUCCESS;
  EFI_HANDLE                        SwHandle;
  EFI_SMM_SW_REGISTER_CONTEXT       SwContext;
  EFI_SMM_SW_DISPATCH2_PROTOCOL     *SwDispatch = 0;

  RAS_DEBUG((LEVEL_FUNC_FLOW, "McejSmmEntry!\n"));

  PolicyData = GetRasPolicy ();
  if (PolicyData->McBankErrorInjection) {
    RAS_DEBUG((LEVEL_FUNC_FLOW, "AMEI Enabled!\n"));

    if (GetAMEISupp () != 0) {
      Status = UpdateMcejTable ();
      if (EFI_ERROR (Status)) {
        return Status;
      }

      Status = gSmst->SmmLocateProtocol (
        &gEfiSmmSwDispatch2ProtocolGuid,
        NULL,
        &SwDispatch
        );

      ASSERT_EFI_ERROR (Status);

      SwContext.SwSmiInputValue = RAS_SW_SMI_MCEJ_EXECUTE;
      Status = SwDispatch->Register (
        SwDispatch,
        McejSwSmiCallBack,
        &SwContext,
        &SwHandle
        );
      RAS_DEBUG((LEVEL_FUNC_FLOW, "SwDispatch->Register: McejSwSmiCallBack, %r\n", Status));
    } else {
      RAS_DEBUG((LEVEL_FUNC_FLOW, "AMEI is not supported by CPU!\n"));
    }
  }

  return EFI_SUCCESS;
}
