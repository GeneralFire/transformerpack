/** @file
  This driver produces the SW SMI handlers that abstracts the WHEA Error
  Record Storage I/F. The runtime driver also publishes the serialization
  actions as ERST.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 - 2020 Intel Corporation. <BR>

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

#include "WheaERST.h"
#include <IndustryStandard/Acpi.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/WheaSupportLib.h>
#include <Library/PlatPolicyLib.h>

#define WHEA_ERROR_END_OP                                       0
#define WHEA_ERROR_GET_RECORD                                   1
#define WHEA_ERROR_SET_RECORD                                   2
#define WHEA_ERROR_CLEAR_RECORD                                 3
#define WHEA_ERROR_DUMMY_WRITE                                  15
#define ACTION_GET_NEXT_RECORD_IDENTIFIER               0x0C

#define MAX_ERROR_LOG_RANGE_SIZE    0x2000
// APTIOV_SERVER_OVERRIDE_RC_START: Using AMI WHEA ERST implementation.

// APTIOV_SERVER_OVERRIDE_RC_START: Security fix
#include <Library/BaseMemoryLib.h>
#include <Library/AmiBufferValidationLib.h>
// APTIOV_SERVER_OVERRIDE_RC_END: Security fix

#include "Token.h"	// Whea Flash Block Porting
#include <Protocol/FlashProtocol.h> // Using AMI WHEA ERST implementation.
// APTIOV_SERVER_OVERRIDE_RC_END: Using AMI WHEA ERST implementation.
//
// Global variables
//
EFI_SMM_SW_DISPATCH2_PROTOCOL       *mSwDispatch  = NULL;
// APTIOV_SERVER_OVERRIDE_RC_START : Using AMI WHEA ERST implementation.
//WHEA_PARAMETER_REGION               *mParamBlock  = NULL; // Params
//UINT8                               *mRecordBlock = NULL; // ELAR
UINT64                               mOsBiosBufferSize = 0;
EFI_SMM_BASE2_PROTOCOL              *mSmmBase =NULL;
EFI_SMM_SYSTEM_TABLE2               *mSmst = NULL;
FLASH_PROTOCOL	                    *Flash=NULL;
FLASH_PROTOCOL	                    *Flash;
WHEA_ERST_REGISTERS                 *mErstReg;
UINT8                               *mPersistentStoreBufferPtr;
UINT32                              mFreeSpaceInPersistentStore;
UINT16                              mTotalErrorRecord = 0;
UEFI_ERROR_RECORD_HEADER            *mElarPtr;

// WHEA Serialization Table
EFI_ACPI_6_2_ERST_SERIALIZATION_INSTRUCTION_ENTRY mERST[16] = {
		  //
		  //ErstBeginWriteEntry [0]
		  //
		    {ERST_BEGIN_WRITE_OPERATION, ERST_WRITE_REGISTER_VALUE, WHEA_CLEAR_FLAG, WHEA_RESERVED,
		     WHEA_ENTRY_ADDRID_SYSMEM, REG_BIT_WIDTH_BYTE, REG_BIT_OFFSET0, WHEA_ACCESS_BYTE, 0,
		     ERST_BEGIN_WRITE_OPERATION, WHEA_BYTE_MASK   },

		    //
		    //ErstBeginReadEntry [1]
		    //
		    {ERST_BEGIN_READ_OPERATION, ERST_WRITE_REGISTER_VALUE, WHEA_CLEAR_FLAG, WHEA_RESERVED,
		     WHEA_ENTRY_ADDRID_SYSMEM,  REG_BIT_WIDTH_BYTE, REG_BIT_OFFSET0, WHEA_ACCESS_BYTE, 0,
		     ERST_BEGIN_READ_OPERATION, WHEA_BYTE_MASK },

		    //
		    //ErstBeginClearEntry [2]
		    //
		    {ERST_BEGIN_CLEAR_OPERATION, ERST_WRITE_REGISTER_VALUE, WHEA_CLEAR_FLAG, WHEA_RESERVED,
		     WHEA_ENTRY_ADDRID_SYSMEM, REG_BIT_WIDTH_BYTE, REG_BIT_OFFSET0, WHEA_ACCESS_BYTE, 0,
		     ERST_BEGIN_CLEAR_OPERATION, WHEA_BYTE_MASK  },

		    //
		    //ErstEndOperationEntry [3]
		    //
		    {ERST_END_OPERATION, ERST_WRITE_REGISTER_VALUE, WHEA_CLEAR_FLAG, WHEA_RESERVED,
		     WHEA_ENTRY_ADDRID_SYSMEM, REG_BIT_WIDTH_BYTE, REG_BIT_OFFSET0, WHEA_ACCESS_BYTE, 0,
		     ERST_END_OPERATION, WHEA_BYTE_MASK          },

		    //
		    //ErstSetRecOffsetEntry [4]
		    //
		    {ERST_SET_RECORD_OFFSET, ERST_WRITE_REGISTER, WHEA_CLEAR_FLAG, WHEA_RESERVED,
		     WHEA_ENTRY_ADDRID_SYSMEM, REG_BIT_WIDTH_DWORD, REG_BIT_OFFSET0, WHEA_ACCESS_DWORD, 0,
		     0x0, WHEA_DWORD_MASK                 },

		    //
		    //ErstExecOperEntry [5]
		    //
		    {ERST_EXECUTE_OPERATION, ERST_WRITE_REGISTER_VALUE, WHEA_CLEAR_FLAG, WHEA_RESERVED,
		     WHEA_ENTRY_ADDRID_SYSIO, REG_BIT_WIDTH_BYTE, REG_BIT_OFFSET0, WHEA_ACCESS_BYTE,SMIPORT,
		     WHEA_ERROR_RECORD_SWSMI, WHEA_BYTE_MASK					                     },

		    //
		    //ErstCheckBusyEntry [6]
		    //
		    {ERST_CHECK_BUSY_STATUS, ERST_READ_REGISTER_VALUE, WHEA_CLEAR_FLAG, WHEA_RESERVED,
		     WHEA_ENTRY_ADDRID_SYSMEM,REG_BIT_WIDTH_BYTE, REG_BIT_OFFSET0, WHEA_ACCESS_BYTE, 0,
		     ERST_BUSY_STS_VALUE, WHEA_BYTE_MASK         },

		    //
		    //ErstGetOperationStatusEntry [7]
		    //
		    {ERST_GET_OPERATION_STATUS, ERST_READ_REGISTER, WHEA_CLEAR_FLAG, WHEA_RESERVED,
		     WHEA_ENTRY_ADDRID_SYSMEM,  REG_BIT_WIDTH_BYTE, REG_BIT_OFFSET0, WHEA_ACCESS_BYTE, 0,
		     0, WHEA_BYTE_MASK                   	    },

		    //
		    //ErstGetRecordIdentifierEntry [8]
		    //
		    {ERST_GET_RECORD_IDENTIFIER, ERST_READ_REGISTER, WHEA_CLEAR_FLAG, WHEA_RESERVED,
		     WHEA_ENTRY_ADDRID_SYSMEM, REG_BIT_WIDTH_QWORD, REG_BIT_OFFSET0, WHEA_ACCESS_QWORD, 0,
		     0, WHEA_QWORD_MASK 	   },

		    //
		    //ErstSetRecordIdentifierEntry [9]
		    //
		    {ERST_SET_RECORD_IDENTIFIER, ERST_WRITE_REGISTER, WHEA_CLEAR_FLAG, WHEA_RESERVED,
		     WHEA_ENTRY_ADDRID_SYSMEM, REG_BIT_WIDTH_QWORD, REG_BIT_OFFSET0, WHEA_ACCESS_QWORD, 0,
		     0, WHEA_QWORD_MASK 	    },

		    //
		    //ErstGetRecordCountEntry [10]
		    //
		    {ERST_GET_RECORD_COUNT, ERST_READ_REGISTER, WHEA_CLEAR_FLAG, WHEA_RESERVED,
		     WHEA_ENTRY_ADDRID_SYSMEM, REG_BIT_WIDTH_WORD, REG_BIT_OFFSET0, WHEA_ACCESS_WORD, 0 ,
		     0, WHEA_WORD_MASK           	   },

		    //
		    //ErstBeginDummeyWriteEntry [11]
		    //
		    {ERST_BEGIN_DUMMY_WRITE_OPERATION, ERST_WRITE_REGISTER_VALUE, 0x0, WHEA_RESERVED,
		     WHEA_ENTRY_ADDRID_SYSMEM, REG_BIT_WIDTH_BYTE, REG_BIT_OFFSET0, WHEA_ACCESS_BYTE, 0,
		     ERST_BEGIN_DUMMY_WRITE_OPERATION, WHEA_BYTE_MASK                                    		   },

		     //
		    //Reserved [12]
		    //
		    // APTIOV_SERVER_OVERRIDE_RC_START
		    {ERST_RESERVED, /*ERST_READ_REGISTER*/ERST_NOOP, WHEA_CLEAR_FLAG, WHEA_RESERVED,
	            // APTIOV_SERVER_OVERRIDE_RC_END : To resolve Error Message while booting to Linux
		    WHEA_ENTRY_ADDRID_SYSMEM, REG_BIT_WIDTH_BYTE, REG_BIT_OFFSET0, WHEA_ACCESS_BYTE, 0,
		    0, WHEA_BYTE_MASK 	   },

		    //
		    //ErstGetElar [13]
		    //
		    {ERST_GET_ERROR_LOG_ADDRESS_RANGE, ERST_READ_REGISTER, WHEA_CLEAR_FLAG, WHEA_RESERVED,
		    WHEA_ENTRY_ADDRID_SYSMEM, REG_BIT_WIDTH_QWORD, REG_BIT_OFFSET0, WHEA_ACCESS_QWORD, 0,
		    0, WHEA_QWORD_MASK 	   },

		    //
		    //ErstGetElarLength [14]
		    //
		    {ERST_GET_ERROR_LOG_ADDRESS_LENGTH, ERST_READ_REGISTER, WHEA_CLEAR_FLAG, WHEA_RESERVED,
		     WHEA_ENTRY_ADDRID_SYSMEM, REG_BIT_WIDTH_DWORD, REG_BIT_OFFSET0, WHEA_ACCESS_DWORD, 0, 0, 0xffffffff       		  },

		    //
		    //ErstGetElarAttributes [15]
		    //
		    {ERST_GET_ERROR_LOG_ADDRESS_ATTR, ERST_READ_REGISTER, WHEA_CLEAR_FLAG, WHEA_RESERVED,
		     WHEA_ENTRY_ADDRID_SYSMEM, REG_BIT_WIDTH_BYTE, REG_BIT_OFFSET0, WHEA_ACCESS_BYTE, 0,
		     0, WHEA_BYTE_MASK           		   }
};
// APTIOV_SERVER_OVERRIDE_RC_END : Using AMI WHEA ERST implementation.

// APTIOV_SERVER_OVERRRIDE_RC_START : Commenting code as AMI code is used for ERST implementation
#if 0
EFI_WHEA_ELOG_STATISTICS  mElog = {
  NULL, // FvBlock
  (EFI_PHYSICAL_ADDRESS)(UINTN)-1,    // BaseAddress of ELOG
  0,    // Length
  0,    // TotalRecordBytes
  0,    // FreeRecordBytes
  (EFI_PHYSICAL_ADDRESS)(UINTN)-1,     // NextRecordAddress
  0
};

//
// WHEA Serialization Table
//
EFI_ACPI_6_2_ERST_SERIALIZATION_INSTRUCTION_ENTRY  ERSTTable[16] = {
    {
      EFI_ACPI_6_2_ERST_BEGIN_WRITE_OPERATION,                 // Serialization Action - 0
      EFI_ACPI_6_2_ERST_WRITE_REGISTER_VALUE,             // Instruction
      0x00,                                 // Flags that qualify the instruction
      0x00,                                         // Reserved
      ERST_REGISTER_FILLER,                           // GAS (QWORD Memory) Address will be filled during boot.
      WHEA_ERROR_SET_RECORD,                             // Value for SetErrorRecord()
      0x0000FFFF                                    // Mask is only valid for 32-bits
    },
    {
      EFI_ACPI_6_2_ERST_BEGIN_READ_OPERATION,                  // Serialization Action - 1
      EFI_ACPI_6_2_ERST_WRITE_REGISTER_VALUE,             // Instruction
      0x00,                                 // Flags that qualify the instruction
      0x00,                                         // Reserved
      ERST_REGISTER_FILLER,                           // GAS (QWORD Memory) Address will be filled during boot.
      WHEA_ERROR_GET_RECORD,                             // Value for GetErrorRecord()
      0x0000FFFF                                    // Mask is only valid for 32-bits
    },
    {
      EFI_ACPI_6_2_ERST_BEGIN_CLEAR_OPERATION,                 // Serialization Action - 2
      EFI_ACPI_6_2_ERST_WRITE_REGISTER_VALUE,             // Instruction
      0x00,                                 // Flags that qualify the instruction
      0x00,                                         // Reserved
      ERST_REGISTER_FILLER,                           // GAS (QWORD Memory) Address will be filled during boot.
      WHEA_ERROR_CLEAR_RECORD,                           // Value for SetErrorRecord()
      0x0000FFFF                                    // Mask is only valid for 32-bits
    },
    {
      EFI_ACPI_6_2_ERST_END_OPERATION,                         // Serialization Action - 3
      EFI_ACPI_6_2_ERST_NOOP,                     // Instruction
      EFI_ACPI_6_2_ERST_PRESERVE_REGISTER,                       // Flags that qualify the instruction
      0x00,                                         // Reserved
      ERST_REGISTER_FILLER,                           // GAS (QWORD Memory) Address will be filled during boot.
      WHEA_ERROR_END_OP,                            // Value for SetErrorRecord()
      0x0000FFFF                                    // Mask is only valid for 32-bits
    },
    {
      EFI_ACPI_6_2_ERST_SET_RECORD_OFFSET,                     // Serialization Action - 4
      EFI_ACPI_6_2_ERST_WRITE_REGISTER,                   // Instruction
      0x00,                                 // Flags that qualify the instruction
      0x00,                                         // Reserved
      ERST_REGISTER_FILLER,                           // GAS (QWORD Memory) Address will be filled during boot.
      0,                                            // Value for SetErrorRecord()
      0xFFFFFFFF                                    // Mask is only valid for 32-bits
    },
    {
      EFI_ACPI_6_2_ERST_EXECUTE_OPERATION,                     // Serialization Action - 5
      EFI_ACPI_6_2_ERST_WRITE_REGISTER_VALUE,             // Instruction
      0x00,                                 // Flags that qualify the instruction
      0x00,                                         // Reserved
      ERST_REGISTER_EXECUTE,                          // GAS (QWORD Memory) Address will be filled during boot.
      RAS_SW_SMI_ERST_EXECUTE,                      // Value field provides the data to write to SMI port 0xb2
      0x0000FFFF                                    // Mask  16-bit
    },
    {
      EFI_ACPI_6_2_ERST_CHECK_BUSY_STATUS,                     // Serialization Action - 6
      EFI_ACPI_6_2_ERST_READ_REGISTER_VALUE,                // Instruction
      0x00,                                 // Flags that qualify the instruction
      0x00,                                         // Reserved
      ERST_REGISTER_FILLER,                           // GAS (QWORD Memory) Address will be filled during boot.
      0x00000001,                                            // Value for SetErrorRecord()
      0x00000001                                    // Mask is only valid for 32-bits
    },
    {
      EFI_ACPI_6_2_ERST_GET_COMMAND_STATUS,                    // Serialization Action - 7
      EFI_ACPI_6_2_ERST_READ_REGISTER,                    // Instruction
      0x00,                                 // Flags that qualify the instruction
      0x00,                                         // Reserved
      ERST_REGISTER_FILLER,                           // GAS (QWORD Memory) Address will be filled during boot.
      0,                                            // Value for SetErrorRecord()
      0xffffffff                                    // Mask is only valid for 32-bits
    },
    {
      EFI_ACPI_6_2_ERST_GET_RECORD_IDENTIFIER,                 // Serialization Action - 8
      EFI_ACPI_6_2_ERST_READ_REGISTER,                    // Instruction
      0x00,                                 // Flags that qualify the instruction
      0x00,                                         // Reserved
      ERST_REGISTER_FILLER,                           // GAS (QWORD Memory) Address will be filled during boot.
      0,                                            // Value for SetErrorRecord()
      0xFFFFFFFFFFFFFFFF                            // Mask is only valid for 32-bits
    },
    {
      EFI_ACPI_6_2_ERST_SET_RECORD_IDENTIFIER,                 // Serialization Action - 9
      EFI_ACPI_6_2_ERST_WRITE_REGISTER,                   // Instruction
      0x00,                                 // Flags that qualify the instruction
      0x00,                                         // Reserved
      ERST_REGISTER_FILLER,                           // GAS (QWORD Memory) Address will be filled during boot.
      0,                                            // Value for SetErrorRecord()
      0xFFFFFFFFFFFFFFFF                            // Mask is only valid for 32-bits
    },
    {
      EFI_ACPI_6_2_ERST_GET_RECORD_COUNT,                      // Serialization Action - 10
      EFI_ACPI_6_2_ERST_READ_REGISTER,                    // Instruction
      0x00,                                 // Flags that qualify the instruction
      0x00,                                         // Reserved
      ERST_REGISTER_FILLER,                           // GAS (QWORD Memory) Address will be filled during boot.
      0,                                            // Value for SetErrorRecord()
      0xFFFFFFFF                                    // Mask is only valid for 32-bits
    },
    {
      EFI_ACPI_6_2_ERST_BEGIN_DUMMY_WRITE_OPERATION,           // Serialization Action - 11
      EFI_ACPI_6_2_ERST_WRITE_REGISTER_VALUE,             // Instruction
      0x00,                                 // Flags that qualify the instruction
      0x00,                                         // Reserved
      ERST_REGISTER_FILLER,                           // GAS (QWORD Memory) Address will be filled during boot.
      WHEA_ERROR_DUMMY_WRITE,                       // Value for Dummy write()
      0xFFFF                                        // Mask is only valid for 32-bits
    },
    {
      ACTION_GET_NEXT_RECORD_IDENTIFIER,            // Serialization Action - 12
      EFI_ACPI_6_2_ERST_READ_REGISTER,                    // Instruction
      0x00,                                 // Flags that qualify the instruction
      0x00,                                         // Reserved
      ERST_REGISTER_FILLER,                           // GAS (QWORD Memory) Address will be filled during boot.
      0,                                            // Value for SetErrorRecord()
      0xFFFFFFFF                                    // Mask is only valid for 32-bits
    },
    {
      EFI_ACPI_6_2_ERST_GET_ERROR_LOG_ADDRESS_RANGE,           // Serialization Action - 13
      EFI_ACPI_6_2_ERST_READ_REGISTER,                    // Instruction
      0x00,                                 // Flags that qualify the instruction
      0x00,                                         // Reserved
      ERST_REGISTER_FILLER,                           // GAS (QWORD Memory) Address will be filled during boot.
      0,                                            // Value for SetErrorRecord()
      0xFFFFFFFFFFFFFFFF                            // Mask is only valid for 32-bits
    },
    {
      EFI_ACPI_6_2_ERST_GET_ERROR_LOG_ADDRESS_RANGE_LENGTH,    // Serialization Action - 14
      EFI_ACPI_6_2_ERST_READ_REGISTER,                    // Instruction
      0x00,                                 // Flags that qualify the instruction
      0x00,                                         // Reserved
      ERST_REGISTER_FILLER,                           // GAS (QWORD Memory) Address will be filled during boot.
      0,                                            // Value for SetErrorRecord()
      0xFFFFFFFFFFFFFFFF                            // Mask is only valid for 32-bits
    },
    {
      EFI_ACPI_6_2_ERST_GET_ERROR_LOG_ADDRESS_RANGE_ATTRIBUTES,// Serialization Action - 15
      EFI_ACPI_6_2_ERST_READ_REGISTER,                    // Instruction
      0x00,                                 // Flags that qualify the instruction
      0x00,                                         // Reserved
      ERST_REGISTER_FILLER,                           // GAS (QWORD Memory) Address will be filled during boot.
      0,                                            // Value for SetErrorRecord()
      0xFFFFFFFFFFFFFFFF                            // Mask is only valid for 32-bits
    }
};





//
// Function implementations
//
//EFI_DRIVER_ENTRY_POINT (InitializeWheaElog)

EFI_STATUS
EFIAPI
InitializeWheaERST(
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
/*++

Routine Description:

  Initializes the SMM Handler Driver

Arguments:
  ImageHandle - ImageHandle of the loaded driver
  SystemTable - Pointer to the System Table

Returns:
  EFI_SUCCESS - If all services discovered.
  Other       - Failure in constructor.

--*/
{
  EFI_STATUS                      Status;
  EFI_SMM_SW_REGISTER_CONTEXT     SwContext;
  EFI_HANDLE                      SwHandle;
  EFI_DEVICE_PATH_PROTOCOL        *CompleteFilePath;
  RAS_POLICY                      *PolicyData =NULL;

  CompleteFilePath        = NULL;
  Status                  = EFI_SUCCESS;

  PolicyData = GetRasPolicy ();
  if (PolicyData->WheaPolicy.WheaSupportEn == 0) {
    return EFI_UNSUPPORTED;
  }

    //
    // Publish the ERST, Open necessary mail boxes between BIOS (SMM) and OS
    //
    Status = UpdateERST();
    if (EFI_ERROR(Status)) {
      DEBUG((DEBUG_ERROR, "InitializeWheaElog: UpdateERST returned %r \n",Status));
      return Status;
    }

    //
  // Although PrstInit is a non-SMM POST routine, it sets up a pointer
  // to a firmware volume block protocol which is used in SMM at runtime,
  // therefore we have to set up mSmst before calling PrstInit.
  //


    //
    // Initialize and Configure the Persistent Store for WHEA ERROR LOG
    // If fails Mostly Persistent Store is NOT Available or NOT Respondinh
    //
    Status = PrstInit ();
    if (EFI_ERROR(Status)) {
      DEBUG((DEBUG_ERROR, "InitializeWheaElog: PrstInit returned %r \n",Status));
      return EFI_NO_MEDIA;
    }

    //
    // Persistent Store is Initialised
    //   Defragment the ELOG Store to eliminate space occupied by records marked as cleared
    //   IMPORTANT NOTE: This is for modularity. Later this operation will NOT be done on
    //                   every boot instead will be called based on some statistics. say eg:
    //                   ever 40% of space is occupied by records marked as deleted.
    //
    PrstDefragment ();

    //
    // Install the SW SMI Handler that EXECUTES Serialization
    //
    Status = gSmst->SmmLocateProtocol ( &gEfiSmmSwDispatch2ProtocolGuid, NULL, &mSwDispatch);
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_ERROR, "InitializeWheaElog: Locate SwDispatch2Protocol returned %r \n",Status));
      return Status;
    }

    SwContext.SwSmiInputValue = RAS_SW_SMI_ERST_EXECUTE;
    Status = mSwDispatch->Register (mSwDispatch, WheaElogSwSmiCallback, &SwContext, &SwHandle);
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_ERROR, "InitializeWheaElog: Register RAS_SW_SMI_ERST_EXECUTE returned %r \n",Status));
      return Status;
    }


  return EFI_SUCCESS;
}


EFI_STATUS
UpdateERST (
  VOID
  )
{
  EFI_STATUS                            Status;
  UINTN                                 ElarSize;


  Status = gBS->AllocatePool (
             EfiReservedMemoryType,
             MAX_ERROR_LOG_RANGE_SIZE,
             &mParamBlock
             );

  ElarSize = MAX_ERROR_LOG_RANGE_SIZE;

  if (EFI_ERROR(Status) || (ElarSize < 0x800)) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Set up Mailbox for Record Block / ELAR
  //
  mRecordBlock = (UINT8 *) mParamBlock + sizeof(WHEA_PARAMETER_REGION);
  ElarSize -= sizeof(WHEA_PARAMETER_REGION);
  mOsBiosBufferSize = ElarSize;

// NOTE: Compiler tip - Use (UINT64) (UINTN)(UINTN), when conerting an address to a 64-bit value. Using just (UINT64) (UINTN)
// extends the sign & screws-up the upper dword.
  mParamBlock->ErrorLogAddressRange       = (UINT64) (UINTN)mRecordBlock;
  mParamBlock->ErrorLogAddressLength      = ElarSize;
  mParamBlock->ErrorLogAddressAttributes  = 0;
  mParamBlock->CurRecordID  = (UINT64) (UINTN)-1;
  mParamBlock->RecordID  = 0;
  mParamBlock->RecordCount  = 0;
  mParamBlock->Status = 0;
  mParamBlock->NextRecordID = 0;
  mParamBlock->Command = 0;
  mParamBlock->LogOffset = 0;
  mParamBlock->BusyFlag = 0;

  ERSTTable[0].RegisterRegion.Address   = (UINT64) (UINTN)&mParamBlock->Command;        // BEGIN_WRITE_OPERATION
  ERSTTable[1].RegisterRegion.Address   = (UINT64) (UINTN)&mParamBlock->Command;        // BEGIN_READ_OPERATION
  ERSTTable[2].RegisterRegion.Address   = (UINT64) (UINTN)&mParamBlock->Command;        // BEGIN_CLEAR_OPERATION
  ERSTTable[3].RegisterRegion.Address   = (UINT64) (UINTN)&mParamBlock->Command;        // END_OPERATION
  ERSTTable[4].RegisterRegion.Address   = (UINT64) (UINTN)&mParamBlock->LogOffset;    // SET_RECORD_OFFSET

  //Action5 is port B2. No need to patch
  ERSTTable[6].RegisterRegion.Address   = (UINT64) (UINTN)&mParamBlock->BusyFlag;       // CHECK_BUSY_STATUS
  ERSTTable[7].RegisterRegion.Address   = (UINT64) (UINTN)&mParamBlock->Status;       // GET_OPERATION_STATUS
  ERSTTable[8].RegisterRegion.Address   = (UINT64) (UINTN)&mParamBlock->CurRecordID;    // GET_RECORD_IDENTIFIER
  ERSTTable[9].RegisterRegion.Address   = (UINT64) (UINTN)&mParamBlock->RecordID;       // SET_RECORD_IDENTIFIER
  ERSTTable[10].RegisterRegion.Address  = (UINT64) (UINTN)&mParamBlock->RecordCount;    // GET_RECORD_COUNT
  ERSTTable[11].RegisterRegion.Address  = (UINT64) (UINTN)&mParamBlock->Command;        // BEGIN_DUMMY_WRITE_OPERATION
  ERSTTable[12].RegisterRegion.Address  = (UINT64) (UINTN)&mParamBlock->NextRecordID;   // GET_NEXT_RECORDID / RESERVED

  ERSTTable[13].RegisterRegion.Address  = (UINT64) (UINTN)&mParamBlock->ErrorLogAddressRange;     // GET_ERROR_LOG_ADDRESS_RANGE
  ERSTTable[14].RegisterRegion.Address  = (UINT64) (UINTN)&mParamBlock->ErrorLogAddressLength;  // GET_ERROR_LOG_ADDRESS_RANGE_LENGTH
  ERSTTable[15].RegisterRegion.Address  = (UINT64) (UINTN)&mParamBlock->ErrorLogAddressAttributes;// GET_ERROR_LOG_ADDRESS_RANGE_ATTRIBUTES

  AddERSTInstruction(&ERSTTable[0], sizeof(ERSTTable));

  return Status;
}

EFI_STATUS
WheaElogSwSmiCallback (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  CONST VOID                    *DispatchContext,
  IN  OUT VOID                      *CommBuffer,
  IN  OUT UINTN                     *CommBufferSize
  )

/*++

Routine Description:

Arguments:

  DispatchHandle
  DispatchContext

Returns:

  None

--*/
{

  //
  // OS to wait for Serialization API to complete
  //
  mParamBlock->BusyFlag = 1;

  switch (mParamBlock->Command) {
  case WHEA_ERROR_SET_RECORD: // ACTION_BEGIN_WRITE_OPERATION:
    PrstWrite(mParamBlock->ErrorLogAddressRange + mParamBlock->LogOffset,
              &mParamBlock->Status);
    break;
  case WHEA_ERROR_GET_RECORD: //ACTION_BEGIN_READ_OPERATION:
    PrstRead(mParamBlock->ErrorLogAddressRange + mParamBlock->LogOffset,
             mParamBlock->RecordID,
             &mParamBlock->Status);

    break;
  case WHEA_ERROR_CLEAR_RECORD: //ACTION_BEGIN_CLEAR_OPERATION:
    PrstClear(mParamBlock->RecordID,
              &mParamBlock->Status);
    break;
  case WHEA_ERROR_DUMMY_WRITE: //ACTION_BEGIN_DUMMY_WRITE_OPERATION:
    // Indicates platform dummy error record write. Platform can set its operational context.
    // No actual record posted by OS in Error Log Address Range
    // Coding Not done for this yet
    break;
  default: // Unknown Serialization Command
    break;
  }

  //
  // To inform OS that Serialization API has completed
  //
  mParamBlock->BusyFlag = 0;

  return EFI_SUCCESS;
}

/**
  Function returns an array of handles that support the SMM FVB protocol
  in a buffer allocated from pool.

  @param[out]  NumberHandles    The number of handles returned in Buffer.
  @param[out]  Buffer           A pointer to the buffer to return the requested
                                array of  handles that support SMM FVB protocol.

  @retval EFI_SUCCESS           The array of handles was returned in Buffer, and the number of
                                handles in Buffer was returned in NumberHandles.
  @retval EFI_NOT_FOUND         No SMM FVB handle was found.
  @retval EFI_OUT_OF_RESOURCES  There is not enough pool memory to store the matching results.
  @retval EFI_INVALID_PARAMETER NumberHandles is NULL or Buffer is NULL.

**/
EFI_STATUS
GetFvbCountAndBuffer (
  OUT UINTN                               *NumberHandles,
  OUT EFI_HANDLE                          **Buffer
  )
{
  EFI_STATUS                              Status;
  UINTN                                   BufferSize;

  if ((NumberHandles == NULL) || (Buffer == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  BufferSize     = 0;
  *NumberHandles = 0;
  *Buffer        = NULL;
  Status = gSmst->SmmLocateHandle (
                    ByProtocol,
                    &gEfiSmmFirmwareVolumeBlockProtocolGuid,
                    NULL,
                    &BufferSize,
                    *Buffer
                    );
  if (EFI_ERROR(Status) && Status != EFI_BUFFER_TOO_SMALL) {
    return EFI_NOT_FOUND;
  }

  Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, BufferSize, (VOID **)Buffer);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = gSmst->SmmLocateHandle (
                    ByProtocol,
                    &gEfiSmmFirmwareVolumeBlockProtocolGuid,
                    NULL,
                    &BufferSize,
                    *Buffer
                    );

  *NumberHandles = BufferSize / sizeof(EFI_HANDLE);
  if (EFI_ERROR(Status)) {
    *NumberHandles = 0;
  }

  return Status;
}

EFI_STATUS
PrstInit (
  VOID
  )
/*++
Routine Description:
    Allocate a 16KB Buffer
    Copy the 16-KB Error Record Region from Flash
    Delete the records that are marked as cleared
    Defragment the records
    Update the Error Record Region in Flash as well

Arguments:
  None

Regurns
  None

--*/
{
  EFI_STATUS                            Status;
  EFI_HANDLE                            *HandleList;
  UINTN                                 HandleCount;
  UINTN                                 HandleIndex;
  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL    *FvBlock;
  STATIC EFI_FIRMWARE_VOLUME_HEADER     FvHeader;
  STATIC EFI_FIRMWARE_VOLUME_EXT_HEADER ExtHeader;
  EFI_PHYSICAL_ADDRESS                  PhyAddress;
  UINTN                                 BlockSize;
  UINTN                                 BlockCount;

   //
  // Grab all Handles that support Firmware Volume Block Protocol
  //

  Status = GetFvbCountAndBuffer (&HandleCount, &HandleList);

  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "PrstInit: No FirmwareVolumeBlockProtocol, %r \n",Status));
    return EFI_NOT_FOUND;
  }

  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++)
  {
    Status = gSmst->SmmHandleProtocol (
                    HandleList[HandleIndex],
                    &gEfiSmmFirmwareVolumeBlockProtocolGuid,
                    &FvBlock
                    );
    if (EFI_ERROR(Status)) {
      continue;
    }

    //
    // Read the Firmware Volume Header
    //
    Status = FvBlock->GetPhysicalAddress(FvBlock, &PhyAddress);
    DEBUG((DEBUG_INFO, "PrstInit: Checking %x\n", PhyAddress));

    MmioReadBuffer8((UINTN)PhyAddress, sizeof (EFI_FIRMWARE_VOLUME_HEADER),(UINT8 *) &FvHeader);

    //
    // Check whether this is the WHEA Error Log Region.
    // We use the FvName guid in the Extended Header, which comes from
    // the "FvNameGuid" directive in PlatformPkg.fdf .  If this guid equals
    // gEfiWheaElogFvGuid (defined in CpPlatPkg.dec), then this FV
    // is the WHEA Error Log.
    //

    if (FvHeader.ExtHeaderOffset == 0) {
      // No extended header
      continue;
    }

    MmioReadBuffer8(
      (UINTN)(PhyAddress+FvHeader.ExtHeaderOffset),
      sizeof (EFI_FIRMWARE_VOLUME_EXT_HEADER),
      (UINT8 *) &ExtHeader);

    if (CompareGuid(&ExtHeader.FvName, &gEfiWheaElogFvGuid)) { // Equal
      DEBUG((DEBUG_INFO, "PrstInit: Found WHEA_FV at %08lx\n", PhyAddress));
      FvBlock->GetBlockSize(FvBlock, 0, &BlockSize,&BlockCount);
      mElog.FvBlock      = FvBlock;
      mElog.HeaderLength = FvHeader.ExtHeaderOffset + (UINT16)ExtHeader.ExtHeaderSize;
      mElog.BaseAddress  = PhyAddress + mElog.HeaderLength;
      mElog.Length       = (UINT32)(BlockSize*BlockCount - mElog.HeaderLength);
      mElog.NextRecordAddress   = (EFI_PHYSICAL_ADDRESS)(UINTN)-1;
      mElog.FreeRecordBytes     = (UINT32)0;
      mElog.TotalRecordBytes    = (UINT32)(BlockSize*BlockCount - mElog.HeaderLength);

      //
      // Success return. We assume Elog is one region with consecutive blocks
      //
      return EFI_SUCCESS;
    }

  }

  DEBUG((DEBUG_INFO, "PrstInit: Did not find WHEA_FV\n"));
  return EFI_NOT_FOUND;

}


EFI_STATUS
PrstDefragment (
  VOID
  )
/*++
Routine Description:
    Allocate a 16KB Buffer
    Copy the 16-KB Error Record Region from Flash
    Delete the records that are marked as cleared
    Defragment the records
    Update the Error Record Region in Flash as well

Arguments:
  None

Regurns
  None

--*/
{
  EFI_STATUS                          Status;
  EFI_PHYSICAL_ADDRESS                SrcAddress;
  EFI_PHYSICAL_ADDRESS                LastUsedAddress;
  EFI_PHYSICAL_ADDRESS                DestAddress;
  UINTN                               DestLen;
  VOID                                *Buffer;
  INTN                                ElogLength;
  EFI_ERROR_RECORD_SERIALIZATION_INFO *SrcRecordHdr;
  UINTN                               SrcRecordLen;
  EFI_LBA                             Lba;
  UINTN                               BlockSize;
  UINTN                               BlockSizeBkp;
  UINTN                               BlockCount;
  UINTN                               TotalBlockSize;
  ERROR_RECORD_HEADER                 *OsRecordHdr  = NULL;
  BOOLEAN                             ForceDefrag = FALSE;

  if (mParamBlock == NULL) {
    DEBUG ((EFI_D_ERROR, "ERROR: mParamBlock is NULL\n"));
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  mParamBlock->CurRecordID  = (UINT64)(UINTN)-1;
  mParamBlock->RecordCount = 0;
  mElog.NextRecordAddress = (EFI_PHYSICAL_ADDRESS)(UINTN)-1;
  mElog.FreeRecordBytes   = 0;

  //
  // Calculate the size of the FV
  //
  if (mElog.FvBlock == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Assumed all blocks are of even size
  //
  Status = mElog.FvBlock->GetBlockSize(mElog.FvBlock, 0, &BlockSize, &BlockCount);
  TotalBlockSize = BlockSize * BlockCount;

  // Allocate Temp Buffer
  Status = gBS->AllocatePages (
                  AllocateAnyPages,
                  EfiBootServicesData,
                  EFI_SIZE_TO_PAGES(TotalBlockSize),
                  (EFI_PHYSICAL_ADDRESS *)&Buffer
                  );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "PrstDefragment: couldn't alloc %x pages! \n",EFI_SIZE_TO_PAGES(TotalBlockSize)));
    return EFI_OUT_OF_RESOURCES;
  }
  ZeroMem (Buffer, TotalBlockSize);

  //
  // Copy the FV Header from FV to the memory buffer
  //
  DestAddress  = (EFI_PHYSICAL_ADDRESS )Buffer;
  mElog.FvBlock->GetPhysicalAddress(mElog.FvBlock, &SrcAddress);

  MmioReadBuffer8(
                 (UINTN)SrcAddress,
                 mElog.HeaderLength,
                 (UINT8 *)(UINTN)DestAddress
                 );

  DestAddress += mElog.HeaderLength;
  DestLen = mElog.HeaderLength;

  //
  // Start copying the actual ELOG (Only the Inuse/Processed) Records
  //
  SrcAddress   = mElog.BaseAddress;
  LastUsedAddress = mElog.BaseAddress;
  ElogLength  = mElog.Length;
  while (ElogLength > 0) {
    if ((*(UINT16 *)(UINTN)(SrcAddress)) == 0xFFFF) {
      DEBUG((DEBUG_INFO, "PrstDefragment: Hit ffff at %x, exiting \n",SrcAddress));
      break; // Reached the end of used space
    }

    OsRecordHdr = (ERROR_RECORD_HEADER *)(UINTN)SrcAddress;
    SrcRecordHdr = (EFI_ERROR_RECORD_SERIALIZATION_INFO *)&OsRecordHdr->OSReserved;
    if (SrcRecordHdr->Signature != SIGNATURE_16('E','R') || \
        OsRecordHdr->Signature != SIGNATURE_32('C','P','E','R')) {
      // Look like corrupted, so erase remaining and compact valid records
      ForceDefrag = TRUE;
      break;
    }

    // Found valid signature
    SrcRecordLen = OsRecordHdr->RecordLength;

    if ((SrcRecordHdr->Attributes & M_ATTRIBUTE_MASK) == B_RECORD_INUSE) {
      if (mParamBlock->RecordCount == 0) {
        mParamBlock->CurRecordID = OsRecordHdr->RecordID;
      }
      mParamBlock->RecordCount++;

    MmioReadBuffer8(
                    (UINTN)SrcAddress,
                    SrcRecordLen,
                    (UINT8 *)(UINTN)DestAddress
                   );

      DestAddress += SrcRecordLen;
      DestLen += SrcRecordLen;
    } else {
    // Skip all the other records - cleared or corrupted attr
    }
    SrcAddress += SrcRecordLen;
    ElogLength -= SrcRecordLen;
    LastUsedAddress = SrcAddress;
  }

// Free offset for new record from last used space and remaining length
  mElog.NextRecordAddress = (UINT64) (UINTN) LastUsedAddress;
  mElog.FreeRecordBytes   = (UINT32)ElogLength;

  // If last used record space beyond 3/4 of the WHEA elog space then defrag, otherwise skip.
  if (mElog.FreeRecordBytes < (TotalBlockSize/4)) {
      ForceDefrag = TRUE;
  }

  if (!ForceDefrag) {
//    gBS->FreePages ((EFI_PHYSICAL_ADDRESS)(UINTN)Buffer, EFI_SIZE_TO_PAGES(TotalBlockSize));
    return EFI_SUCCESS;
  }

// Free offset for new record is starting of the compacted used space and remaining length
  Status = mElog.FvBlock->GetPhysicalAddress(mElog.FvBlock, &SrcAddress);
  mElog.NextRecordAddress = SrcAddress + DestLen;
  mElog.FreeRecordBytes   = (UINT32)(TotalBlockSize - DestLen);

  //
  // Erase WHEA Blocks in FV and Write them from Buffer
  //
  SrcAddress = (EFI_PHYSICAL_ADDRESS)(UINTN) Buffer;
  Status = EFI_SUCCESS;
  Status = mElog.FvBlock->GetBlockSize(mElog.FvBlock, 0, &BlockSize, &BlockCount);
  Status = mElog.FvBlock->EraseBlocks (
            mElog.FvBlock,
            (EFI_LBA)0,
            (UINT32)BlockCount,
            EFI_LBA_LIST_TERMINATOR);
  DEBUG((DEBUG_INFO, "PrstDefragment: EraseBlocks returned %r \n",Status));
  for (Lba=0; (Lba < BlockCount) && (!EFI_ERROR(Status)) && (DestLen!=0); Lba++) {
      //
      // Erase is block erase, so obliged to write entire block
      // But assumption is all blocks are of same size
      //
      if (DestLen >= BlockSize) {
        BlockSizeBkp = BlockSize;
      } else {
        BlockSizeBkp = DestLen;
        BlockSize = DestLen;
      }
      Status = mElog.FvBlock->Write (
                mElog.FvBlock,
                Lba,
                0,
                &BlockSizeBkp,
                (UINT8 *)(UINTN)SrcAddress);
      DEBUG((DEBUG_INFO, "PrstDefragment: WriteBlocks returned %r \n",Status));
      AsmWbinvd ();
      if(BlockSizeBkp != BlockSize) {
        break;
      }
      DestLen -= BlockSize;
      SrcAddress += BlockSize;
  }

//  gBS->FreePages ((EFI_PHYSICAL_ADDRESS)(UINTN)Buffer, EFI_SIZE_TO_PAGES(TotalBlockSize));
  return Status;
}

VOID
PrstSearch (
  UINT64                RecordID,
  EFI_PHYSICAL_ADDRESS  *PhyAddress,
  UINT64                *Status,
  BOOLEAN               FindNextRecordID
  )
/*++
Routine Description:
  Find a ELOG record using RecordID as Key

Arguments:
  RecordBuffer - Buffer where the record should be returned
  RecordID     - Identifier of the record that should be searched and read to buffer
  Status       - Defined by WHEA Specification as below
                    0x00  Success
                    0x04  Record Store Empty
                    0x05  Record Not Found
Returns
  None

--*/
{
  EFI_PHYSICAL_ADDRESS                Record;
  EFI_ERROR_RECORD_SERIALIZATION_INFO *RecordHdr;
  UINT32                              RecordLen;
  INTN                                ElogLen;
  ERROR_RECORD_HEADER                 *OsRecordHdr;

  *PhyAddress = 0;  // not found, null address
  *Status = 5;  // Record not found
  if (mElog.FvBlock == NULL) {
    return;
  }

  Record = mElog.BaseAddress;
  OsRecordHdr = (ERROR_RECORD_HEADER *)(UINTN)mElog.BaseAddress;
  RecordHdr = (EFI_ERROR_RECORD_SERIALIZATION_INFO *)&OsRecordHdr->OSReserved;
  if (RecordHdr->Signature != SIGNATURE_16('E', 'R')) {
    *Status = 4; // ELOG empty
    return ;
  }

  ElogLen = mElog.Length;
  while ((ElogLen > 0)
    && ((RecordHdr->Signature == SIGNATURE_16('E', 'R'))
      &&(OsRecordHdr->Signature == SIGNATURE_32('C', 'P', 'E', 'R')))) {
    if ((RecordHdr->Attributes & M_ATTRIBUTE_MASK) == B_RECORD_INUSE) {
      if (*PhyAddress != 0) {
      // Looking for next record only
        mParamBlock->CurRecordID = OsRecordHdr->RecordID;
        return;
      }
      if ((OsRecordHdr->RecordID == RecordID)) {
        *PhyAddress = Record;
        *Status = 0;  // Success, Record found
        if (!FindNextRecordID) {
          return ;
        }
      }
    }

    //
    // Jump to next record
    //
    RecordLen = OsRecordHdr->RecordLength;
    Record += RecordLen;
    ElogLen -= RecordLen;
    OsRecordHdr = (ERROR_RECORD_HEADER *)(UINTN)Record;
    RecordHdr = (EFI_ERROR_RECORD_SERIALIZATION_INFO *)&OsRecordHdr->OSReserved;
  }

  if (*PhyAddress != 0) {
  // Try to find next record ID, but none found
    mParamBlock->CurRecordID = (UINT64) (UINTN)-1;
    return;
  }

  *Status = 5;  // Record not found
  return ;
}

VOID
PrstRead (
  EFI_PHYSICAL_ADDRESS  RecordBuffer,
  UINT64                RecordID,
  UINT64                *Status
  )
/*++
Routine Description:
  Read a ELOG record in to the buffer provided

Arguments:
  RecordBuffer - Buffer where the record should be returned
  RecordID     - Identifier of the record that should be searched and read to buffer
  Status       - Defined by WHEA Specification as below
                    0x00  Success
                    0x04  Record Store Empty
                    0x05  Record Not Found
Returns
  None

--*/
{
  EFI_PHYSICAL_ADDRESS                  PhyAddress;
  ERROR_RECORD_HEADER                   *OsRecordHdr;
  EFI_ERROR_RECORD_SERIALIZATION_INFO   *RecordHdr;

  if (mElog.FvBlock == NULL) {
    *Status = 3; //failed
    return;
  }

  if (RecordID == 0) {
    if (mParamBlock->CurRecordID == (UINTN)-1) {
      *Status = 5; //not found
      return;
    }
    RecordID = mParamBlock->CurRecordID; // read the first good record
  }

  PrstSearch(RecordID, &PhyAddress, Status, TRUE);
  if (*Status == 0) {
    OsRecordHdr = (ERROR_RECORD_HEADER *)(UINTN)PhyAddress;

    if ((RecordBuffer < (UINT64) (UINTN)mRecordBlock) ||
      ((RecordBuffer + OsRecordHdr->RecordLength) >  ((UINT64) (UINTN)mRecordBlock + mOsBiosBufferSize))) {
      return;
    }                                                                                      

    MmioReadBuffer8((UINTN)PhyAddress,OsRecordHdr->RecordLength,(UINT8 *)(UINTN)RecordBuffer);

//////
// FIX: 11/30/07 :
//      Clear out the Platform serialization data (Psd) that's used in persistence
//      flash by BIOS. This is not required by the specification, but Win2008 expects
//      it to be zero. Microsoft may fix it in next version.

    //
    // Serialization Header is part of OS Error Record, use it
    //
    OsRecordHdr = (ERROR_RECORD_HEADER *)(UINTN)RecordBuffer;
    RecordHdr = (EFI_ERROR_RECORD_SERIALIZATION_INFO *)&OsRecordHdr->OSReserved;
    RecordHdr->Attributes = 0;    // Clear out internal attribute mask used in Psd.
//////
  }
  return ;
}

VOID
PrstWrite(
  EFI_PHYSICAL_ADDRESS  NewRecord,
  UINT64                *Status
  )
/*++
Routine Description:
  If the record already exists, mark it as cleared.
  Always append the new record in the end

Arguments:
  Record - Buffer where the record should be returned
  Status - Defined by WHEA Specification as below
              0x00  Success
              0x01  Not Enough Space
              0x02  Hardware Not Available
              0x03  Failed

Returns
  None

--*/
{
  EFI_STATUS                          EfiStatus;
  EFI_ERROR_RECORD_SERIALIZATION_INFO *RecordHdr;
  ERROR_RECORD_HEADER                 *OsRecordHdr;
  UINT32                              NumBytes;
  EFI_PHYSICAL_ADDRESS                PhyAddress;
  EFI_LBA                             Lba;
  UINTN                               Offset;
  UINT8                               Attributes;
  UINT32                              BytesWritten;

  *Status = 3;  // Failed

  if (mElog.FvBlock == NULL) {
    return;
  }

// Clear record if it exist
  OsRecordHdr = (ERROR_RECORD_HEADER *)(UINTN)NewRecord;
  if (OsRecordHdr->RecordID == 0) {
    return;
  }
  if (OsRecordHdr->Signature != SIGNATURE_32('C','P','E','R')) {
    return;
  }
  PrstSearch(OsRecordHdr->RecordID, &PhyAddress, Status, FALSE);
                    //0x00  Success
                    //0x04  Record Store Empty
                    //0x05  Record Not Found
  if (*Status == 0) {
    // mark the record deleted
    OsRecordHdr = (ERROR_RECORD_HEADER *)(UINTN)PhyAddress;
    RecordHdr = (EFI_ERROR_RECORD_SERIALIZATION_INFO *)&OsRecordHdr->OSReserved;
    PhyAddress = (EFI_PHYSICAL_ADDRESS)(UINTN) &RecordHdr->Attributes;
    ConvertPhyAddressToLbaOffset(PhyAddress, &Lba, &Offset);
    ASSERT (Lba != -1);
    // clear BIT[1:0] to mark it deleted
    Attributes = RecordHdr->Attributes & (~M_ATTRIBUTE_MASK);
    NumBytes = 1; // UINT8
    EfiStatus = mElog.FvBlock->Write (
                mElog.FvBlock,
                Lba,
                Offset,
                (UINTN *)(UINTN)&NumBytes,
                &Attributes);
    AsmWbinvd ();
    if (EFI_ERROR (EfiStatus) || (NumBytes != 1)) {
      *Status = 3;  // Failed
      return ;
    }
    mParamBlock->RecordCount--;
  }

  //
  // Append a new copy of the new record
  //
  OsRecordHdr = (ERROR_RECORD_HEADER *)(UINTN)NewRecord;
  // Check if we have enough space
  NumBytes = OsRecordHdr->RecordLength;
  if (NumBytes > mElog.FreeRecordBytes) {
    //
    // Not enough space? Try defrafment/cleanup
    //
///Cannot do Defrag fn is not designed to run at runtime.
//    EfiStatus = PrstDefragment();
//    if (EFI_ERROR(EfiStatus)) {
      //
      // No other way, return failure
      //
      *Status = 1; // not enough space
      return ;
//    }

  }

  //
  // Serialization Header is part of OS Error Record, use it
  //
  RecordHdr = (EFI_ERROR_RECORD_SERIALIZATION_INFO *)&OsRecordHdr->OSReserved;

  //
  // If the Serialization Header is missing in OS Record, better make one
  //
  if (RecordHdr->Signature != SIGNATURE_16('E','R')) {
    RecordHdr->Signature = SIGNATURE_16('E','R');
  }
  RecordHdr->Attributes = (UINT8)(~M_ATTRIBUTE_MASK) | B_RECORD_INUSE; // BUT[1:0] = 11 means in use

  BytesWritten = 0;
  do {
    //
    // Place the new OS Error Record at the Bottom of WHEA Flash ELOG
    //
    ConvertPhyAddressToLbaOffset(mElog.NextRecordAddress, &Lba, &Offset);
    ASSERT (Lba != -1);

    EfiStatus = mElog.FvBlock->Write (
                mElog.FvBlock,
                Lba,
                Offset,
                (UINTN *)(UINTN) &NumBytes,
                (UINT8 *) OsRecordHdr + BytesWritten
                );
    AsmWbinvd ();
    mElog.NextRecordAddress += NumBytes;
    mElog.FreeRecordBytes -= NumBytes;
    BytesWritten += NumBytes;

    if (EFI_ERROR (EfiStatus) && (EfiStatus != EFI_BAD_BUFFER_SIZE)) {
      *Status = 3;  // Failed
      return ;
    }

    NumBytes = OsRecordHdr->RecordLength - BytesWritten;
  } while (NumBytes > 0);

  if (mParamBlock->RecordCount == 0){
    mParamBlock->CurRecordID = OsRecordHdr->RecordID;
  }
  mParamBlock->RecordCount++;
  *Status = 0;  // Success, Record written
  return ;
}

VOID
PrstClear(
  UINT64                RecordID,
  UINT64                *Status
  )
/*++
Routine Description:
  search for the record with ID
  If there is NO single record in ELOG write the record in the beginning
  If record not found go to end of ELOG
  If space is available append and return 0 (success)
  If space is NOT available return 1 (not enough space)
  If record found update the record and return 0, success
Arguments:
  Record - Buffer where the record should be returned
  Status - Defined by WHEA Specification as below
              0x00  Success
              0x01  Not Enough Space
              0x02  Hardware Not Available
              0x03  Failed

Returns
  None

--*/
{
  EFI_STATUS                          EfiStatus;
  EFI_ERROR_RECORD_SERIALIZATION_INFO *RecordHdr;
  EFI_PHYSICAL_ADDRESS                PhyAddress;
  ERROR_RECORD_HEADER                 *OsRecordHdr;
  UINT8                               Attributes;
  EFI_LBA                             Lba;
  UINTN                               Offset;
  UINTN                               NumBytes;

  *Status = 3;  // Failed

  if (RecordID == 0) {
    return;
  }
  if (mElog.FvBlock == NULL) {
    return;
  }

  PrstSearch(RecordID, &PhyAddress, Status, FALSE);
                    //0x00  Success
                    //0x04  Record Store Empty
                    //0x05  Record Not Found
  if (*Status == 0) {
    // mark the record deleted
    // mark the record deleted
    OsRecordHdr = (ERROR_RECORD_HEADER *)(UINTN)PhyAddress;
    RecordHdr = (EFI_ERROR_RECORD_SERIALIZATION_INFO *)&OsRecordHdr->OSReserved;
    PhyAddress = (EFI_PHYSICAL_ADDRESS)(UINTN) &RecordHdr->Attributes;
    ConvertPhyAddressToLbaOffset(PhyAddress, &Lba, &Offset);
    Attributes = RecordHdr->Attributes & (~M_ATTRIBUTE_MASK);  // clear BIT[1:0]
    NumBytes = 1; // UINT8
    EfiStatus = mElog.FvBlock->Write (
                mElog.FvBlock,
                Lba,
                Offset,
                &NumBytes,
                &Attributes);
    AsmWbinvd ();
    if (EFI_ERROR (EfiStatus) || (NumBytes != 1)) {
      *Status = 3;  // Failed
      return ;
    }
    if (mParamBlock->RecordCount != 0) {
      mParamBlock->RecordCount--;
    }
  }

  if (mParamBlock->CurRecordID == RecordID) {
    mParamBlock->CurRecordID = (UINT64) (UINTN)-1;
  }
  return ;
}


EFI_STATUS
ConvertPhyAddressToLbaOffset (
  IN  EFI_PHYSICAL_ADDRESS   Address,
  OUT EFI_LBA                *Lba,
  OUT UINTN                  *Offset
  )
/*++
Routine Description:
  Converts the Physical Address in to LBA and Offset in the LBA

Arguments:
  Address - 32-bit Physical Address
  Lba     - Address of EFI_LBA where the LBA number is copied to
  Offset  - Address of UINTN variable where the offset is copied to
  Status  - EFI_SUCCESS - lba/offset conversion successful
            EFI_INVALID_PARAMETER - Address is OUT OF RANGE

Returns
  None

--*/
{
  EFI_FV_BLOCK_MAP_ENTRY      *FvbMapEntry;
  EFI_PHYSICAL_ADDRESS        WheaBlockAddress;
  UINT32                      LbaIndex;
  EFI_PHYSICAL_ADDRESS        LbaBaseAddress;
  EFI_PHYSICAL_ADDRESS        LbaLimit; //SrcAddress;
  EFI_FIRMWARE_VOLUME_HEADER  *FvHdr;

  if (mElog.FvBlock == NULL) {
    return EFI_NOT_FOUND;
  }

  Address &= 0xFFFFFFFF; // Flash is mapped just below 4GB (32-bit)

  mElog.FvBlock->GetPhysicalAddress(mElog.FvBlock, &WheaBlockAddress);
  FvHdr = (EFI_FIRMWARE_VOLUME_HEADER *)(UINTN)WheaBlockAddress;

  //
  // Get the (LBA, Offset) of Address
  //
  //
  // Assume all LBA in same FVBlock has same BlockLength
  //
  FvbMapEntry = &FvHdr->BlockMap[0];
  LbaBaseAddress = WheaBlockAddress;
  LbaLimit = WheaBlockAddress;
  for (LbaIndex = 0; LbaIndex < FvbMapEntry->NumBlocks; LbaIndex++) {
    //if (Address < (SrcAddress + FvbMapEntry->BlockLength * LbaIndex)) {
    LbaLimit += FvbMapEntry->Length;
    if ((Address >= LbaBaseAddress) && (Address < LbaLimit)) {
      //
      // Found the (Lba, Offset)
      //
      *Lba = LbaIndex;
      *Offset = (UINTN)(Address - LbaBaseAddress);
      return EFI_SUCCESS;
    }
    LbaBaseAddress += FvbMapEntry->Length;
  }

  *Lba = (EFI_LBA)(-1);
  *Offset = 0;
  return EFI_ABORTED;
}
#endif
// APTIOV_SERVER_OVERRRIDE_RC_END : Commenting code as AMI code is used for ERST implementation

// APTIOV_SERVER_OVERRIDE_RC_START : Using AMI WHEA ERST implementation.
/** @internal

  Initializes services required for  Flash Functions
   @param[in]       void
   @return VOID     Nothing.
**/
EFI_STATUS
InitFlash (
)
{
  BOOLEAN InSmm = FALSE;
  EFI_SMM_BASE2_PROTOCOL    *mSmmBase2;
  EFI_STATUS Status;

  Status = gBS->LocateProtocol (
                  &gEfiSmmBase2ProtocolGuid,
                  NULL,
                  (VOID **) &mSmmBase2
                  );

  if(!EFI_ERROR(Status)) {

     mSmmBase2->InSmm (mSmmBase2, &InSmm);
  } else {

      return EFI_NOT_FOUND;
  }

  if(InSmm) {
    return gBS->LocateProtocol (&gFlashSmmProtocolGuid, NULL, &Flash);


  } else {
    return gBS->LocateProtocol (&gFlashProtocolGuid, NULL, &Flash);
  }
}

/**
   Driver Entry Point: Creates Error Log Address Range to be used by ERST serial instruction and
   Persistent Store Buffer.Update ERST Table, Copy ERST ACPI table to boot memory
   space and Update WHEA private data structure

  @param[out]  NumberHandles    The number of handles returned in Buffer.
  @param[out]  Buffer           A pointer to the buffer to return the requested
                                array of  handles that support SMM FVB protocol.

  @retval EFI_SUCCESS           The array of handles was returned in Buffer, and the number of
                                handles in Buffer was returned in NumberHandles.
  @retval EFI_NOT_FOUND         No SMM FVB handle was found.
  @retval EFI_OUT_OF_RESOURCES  There is not enough pool memory to store the matching results.
  @retval EFI_INVALID_PARAMETER NumberHandles is NULL or Buffer is NULL.

**/

EFI_STATUS
InitializeWheaERST (
    IN EFI_HANDLE                     ImageHandle,
    IN EFI_SYSTEM_TABLE               *SystemTable
)
{

  EFI_STATUS                     Status;
  PERSISTENT_ERROR_RECORD        *PersistentErrorRecordPtr;
  UINT64                         TempAddr;
  VOID                           *FvWheaBase;
  EFI_SMM_SW_REGISTER_CONTEXT     SwContext;
  EFI_HANDLE                      SwHandle;
  BOOLEAN                         InSmm;
  static EFI_SMM_BASE2_PROTOCOL   *mSmmBase = NULL;

  DEBUG((DEBUG_INFO,"%a ENTRY \n", __FUNCTION__));
  
  InitFlash();

  //
  //Create Error Log Address Range to be used by ERST serial instruction
  //
  Status = gBS->AllocatePool (EfiReservedMemoryType,ELAR_SIZE + sizeof(WHEA_ERST_REGISTERS), &mElarPtr);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  gBS->SetMem(mElarPtr, ELAR_SIZE + sizeof(WHEA_ERST_REGISTERS), 0);

  Status = gBS->LocateProtocol (
                  &gEfiSmmBase2ProtocolGuid,
                  NULL,
                  (VOID **) &mSmmBase
                  );

  if (mSmmBase == NULL) {
    InSmm = FALSE;
  } else {
    mSmmBase->InSmm (mSmmBase, &InSmm);
  }

  if (InSmm) {

      TempAddr = FV_WHEA_BASE;
      FvWheaBase = (VOID*)TempAddr;

      mErstReg = (WHEA_ERST_REGISTERS*)mElarPtr;
      mElarPtr = (UEFI_ERROR_RECORD_HEADER*)((UINT8*)mElarPtr + sizeof(WHEA_ERST_REGISTERS));

      mERST[0].RegisterRegion.Address   = (UINT64)((UINTN)&mErstReg->CmdReg);       // BEGIN_WRITE_OPERATION
      mERST[1].RegisterRegion.Address   = (UINT64)((UINTN)&mErstReg->CmdReg);       // BEGIN_READ_OPERATION
      mERST[2].RegisterRegion.Address   = (UINT64)((UINTN)&mErstReg->CmdReg);       // BEGIN_CLEAR_OPERATION
      mERST[3].RegisterRegion.Address   = (UINT64)((UINTN)&mErstReg->CmdReg);       // END_OPERATION
      mERST[4].RegisterRegion.Address   = (UINT64)((UINTN)&mErstReg->RecordOffset);     // SET_RECORD_OFFSET

      //Action5 is port B2. No need to patch
      mERST[6].RegisterRegion.Address   = (UINT64)((UINTN)&mErstReg->BusyStatus);       // CHECK_BUSY_STATUS
      mERST[7].RegisterRegion.Address   = (UINT64)((UINTN)&mErstReg->OperationStatus);          // GET_OPERATION_STATUS
      mERST[8].RegisterRegion.Address   = (UINT64)((UINTN)&mErstReg->ValidRecordIdentifier);    // GET_RECORD_IDENTIFIER
      mERST[9].RegisterRegion.Address   = (UINT64)((UINTN)&mErstReg->OSRecordIdentifier);       // SET_RECORD_IDENTIFIER
      mERST[10].RegisterRegion.Address  = (UINT64)((UINTN)&mErstReg->RecordCount);    // GET_RECORD_COUNT
      mERST[11].RegisterRegion.Address  = (UINT64)((UINTN)&mErstReg->CmdReg);       // BEGIN_DUMMY_WRITE_OPERATION
    //  mERST[12].RegisterRegion.Address  = (UINT64) (UINTN)&mParamBlock->NextRecordID;     // GET_NEXT_RECORDID / RESERVED

      mERST[13].RegisterRegion.Address  = (UINT64)((UINTN)&mErstReg->ElarAddress);        // GET_ERROR_LOG_ADDRESS_RANGE
      mERST[14].RegisterRegion.Address  = (UINT64)((UINTN)&mErstReg->ElarLength); // GET_ERROR_LOG_ADDRESS_RANGE_LENGTH
      mERST[15].RegisterRegion.Address  = (UINT64)((UINTN)&mErstReg->ElarAttributes); // GET_ERROR_LOG_ADDRESS_RANGE_ATTRIBUTES

      AddERSTInstruction(&mERST[0], sizeof(mERST));
      
      mSmmBase->GetSmstLocation (mSmmBase, &mSmst);
      //
      //Create Persistent Store Buffer
      //
      Status = mSmst->SmmAllocatePool (EfiRuntimeServicesData, FV_WHEA_SIZE, &mPersistentStoreBufferPtr);
        if (EFI_ERROR(Status)) {
          return Status;
        }
      SetMem (mPersistentStoreBufferPtr, FV_WHEA_SIZE, 0);

      mErstReg->ElarAddress= (UINT64)((UINTN)mElarPtr);
      mErstReg->ElarLength= ELAR_SIZE;
      mErstReg->ElarAttributes= 0;

      mFreeSpaceInPersistentStore= FV_WHEA_SIZE;
      mErstReg->RecordCount= 0;
      mErstReg->ValidRecordIdentifier= 0xffffffffffffffff;

      Flash->Read((UINT8*)FvWheaBase, FV_WHEA_SIZE,(UINT8*)mPersistentStoreBufferPtr);

      PersistentErrorRecordPtr= (PERSISTENT_ERROR_RECORD*)mPersistentStoreBufferPtr;

      if (PersistentErrorRecordPtr->NvPrependData.MagicNum == WHEA_PREPEND_MAGIC_NUMBER) {

        while ( (PersistentErrorRecordPtr->NvPrependData.MagicNum == WHEA_PREPEND_MAGIC_NUMBER)
                  && ((UINT64)((UINTN)PersistentErrorRecordPtr) < (UINT64)((UINTN)mPersistentStoreBufferPtr+FV_WHEA_SIZE-PS_PREPEND_DATA_SIZE)))
        {

          if (PersistentErrorRecordPtr->NvPrependData.RecordValidity == FLASH_ERASE_VALUE) {

             mErstReg->ValidRecordIdentifier= PersistentErrorRecordPtr->UefiErrorRecord.RecordID;
             mErstReg->RecordCount= mErstReg->RecordCount+1;
          }

          mTotalErrorRecord= mTotalErrorRecord + 1;
          mFreeSpaceInPersistentStore= mFreeSpaceInPersistentStore-(PersistentErrorRecordPtr->UefiErrorRecord.RecLength+PS_PREPEND_DATA_SIZE+PS_APPEND_DATA_SIZE);
          PersistentErrorRecordPtr= (PERSISTENT_ERROR_RECORD*)((UINT8*)PersistentErrorRecordPtr +(PersistentErrorRecordPtr->UefiErrorRecord.RecLength+PS_PREPEND_DATA_SIZE+PS_APPEND_DATA_SIZE));

        }

      }

  	  //
  	  // Install the SW SMI Handler that EXECUTES Serialization
  	  //
  	  Status = mSmst->SmmLocateProtocol ( &gEfiSmmSwDispatch2ProtocolGuid, NULL, &mSwDispatch);
      DEBUG((DEBUG_INFO,"gEfiSmmSwDispatch2ProtocolGuid: %r \n", Status));
  	  if (EFI_ERROR (Status)) {
  		  return Status;
  	  }
  	  SwContext.SwSmiInputValue = WHEA_ERROR_RECORD_SWSMI;
  	  Status = mSwDispatch->Register (mSwDispatch, ErstSWSMIHandler, &SwContext, &SwHandle);
      DEBUG((DEBUG_INFO,"mSwDispatch: %r\n", Status));
  	  if (EFI_ERROR (Status)) {
  		  return Status;
  	  }

  }
  DEBUG((DEBUG_INFO,"%a EXIT \n", __FUNCTION__));
  return EFI_SUCCESS;
}

/** @internal

  Find valid record and deletes records which are marked for delete.
  Gets called when pstore is full.

   @param[in]       void
   @return VOID     Nothing.
**/
UINT8
GetNextValidRecord (
  UINT16                   *NumOfRecords,
  PERSISTENT_ERROR_RECORD  *PersErrorRecordPtr,
  PERSISTENT_ERROR_RECORD  **PersErrorRecordPtr2 )
{

    UINT8 FoundValidRecord = 0;


    for (*NumOfRecords=*NumOfRecords-1; *NumOfRecords>0; *NumOfRecords=*NumOfRecords-1) {

      //
      // Move ptr to immediately next record following the current record
      //
      PersErrorRecordPtr = (PERSISTENT_ERROR_RECORD*)((UINT8*)PersErrorRecordPtr + PersErrorRecordPtr->UefiErrorRecord.RecLength + PS_PREPEND_DATA_SIZE  + PS_APPEND_DATA_SIZE);

      //
      // if record is a valid record, copy record to a buffer and break the loop.
      //
      if (PersErrorRecordPtr->NvPrependData.RecordValidity ==  FLASH_ERASE_VALUE) {
        *PersErrorRecordPtr2 = PersErrorRecordPtr;
        FoundValidRecord= 1;
        break;
      }

      //
      // if record is not valid, decrease record count by one, free space by record size and keep looping
      //
      mTotalErrorRecord = mTotalErrorRecord - 1;
      mFreeSpaceInPersistentStore = mFreeSpaceInPersistentStore
                                 + PersErrorRecordPtr->UefiErrorRecord.RecLength + PS_PREPEND_DATA_SIZE + PS_APPEND_DATA_SIZE;

    }

    return FoundValidRecord;
}

/** @internal

   When ERST persistent store is full, this method is called.
   This deletes error records marked for delete from peristent store.

   @param[in]       void
   @return VOID     Nothing.
**/
VOID
DeleteMarkedErrorRecord ( void )
{
    PERSISTENT_ERROR_RECORD  *PersErrorRecordPtr;
    PERSISTENT_ERROR_RECORD  *PersErrorRecordPtr2;
    EFI_STATUS               Status;
    UINT16                   NumOfRecords;
    UINT8                    PersistentStoreModified=0;
    UINT64                   TempAddress;
    VOID                     *FvWheaBase;
    UINT32                   TotalRecToCopy,RecBefDel=0;
    const UINT8              *EndOfPersistentRecordBuffer = (UINT8*)mPersistentStoreBufferPtr + FV_WHEA_SIZE; 

    TempAddress = FV_WHEA_BASE;
    FvWheaBase = (VOID*)TempAddress;

    //
    // Pointing to beginning of persistent store
    //
    PersErrorRecordPtr = (PERSISTENT_ERROR_RECORD*)mPersistentStoreBufferPtr;

    //
    // Looping through all records in pstore
    //
    for (NumOfRecords=mTotalErrorRecord; NumOfRecords>0; NumOfRecords--) {
        
        /// AmiValidateSmramBuffer(): Restricting UefiErrorRecord.RecLength for access to any value outside of SMRAM.
        Status = AmiValidateSmramBuffer((UINT8*)PersErrorRecordPtr, PersErrorRecordPtr->UefiErrorRecord.RecLength);
        if (EFI_ERROR(Status)){
            DEBUG((DEBUG_ERROR, "%a() Exit-1: PersErrorRecordPtr: RecLength outside of SMRAM\n", __FUNCTION__));
            break;
        }
        
        /// Restricting that UefiErrorRecord.RecLength should not cross the WHEA Buffer.
        if(((UINT8*)PersErrorRecordPtr + PersErrorRecordPtr->UefiErrorRecord.RecLength) < EndOfPersistentRecordBuffer) {

            //
            // Checking if record is marked for delete. If so, free that space used by this record, 
            // decrease rec count by one and calculate total record to be finally copied to pstore.
            //
            if (PersErrorRecordPtr->NvPrependData.RecordValidity ==  MARKED_AS_DELETE ) {
                PersistentStoreModified = 1;
                mTotalErrorRecord = mTotalErrorRecord - 1;
                mFreeSpaceInPersistentStore = mFreeSpaceInPersistentStore
                                         + PersErrorRecordPtr->UefiErrorRecord.RecLength + PS_PREPEND_DATA_SIZE + PS_APPEND_DATA_SIZE;
                TotalRecToCopy = FV_WHEA_SIZE - mFreeSpaceInPersistentStore - RecBefDel;
                
                //
                // Get next valid record from remaining records in pstore
                //
                if (GetNextValidRecord(&NumOfRecords, PersErrorRecordPtr, &PersErrorRecordPtr2) == 1) {
                    
                    /// Restricting RecLength of next record should not cross the WHEA Buffer.
                    if(((UINT8*)PersErrorRecordPtr2 + PersErrorRecordPtr2->UefiErrorRecord.RecLength) > EndOfPersistentRecordBuffer) {
                        DEBUG((DEBUG_ERROR, "Exit-3: PersErrorRecordPtr2: RecLength Went out of Bounds\n"));
                        break;
                    }
                
                    if ( mFreeSpaceInPersistentStore <= FV_WHEA_SIZE ) {
                     TotalRecToCopy = FV_WHEA_SIZE - mFreeSpaceInPersistentStore - RecBefDel;
                     PersistentStoreModified = 1;
                     
                     /// AmiValidateSmramBuffer(): Restricting TotalRecToCopy for access to any value outside of SMRAM.
                     Status = AmiValidateSmramBuffer((UINT8*)PersErrorRecordPtr2, TotalRecToCopy);
                     if (EFI_ERROR(Status)){
                         DEBUG((DEBUG_ERROR, "%a() Exit-4: TotalRecToCopy: RecLength outside of SMRAM\n", __FUNCTION__));
                         break;
                     }

                     //
                     // PersErrorRecordPtr2 will contain updated pstore error records from GetNextValidRecord method.
                     //
                     CopyMem(PersErrorRecordPtr, PersErrorRecordPtr2, TotalRecToCopy);
                     PersErrorRecordPtr = (PERSISTENT_ERROR_RECORD*)((UINT8*)PersErrorRecordPtr + PersErrorRecordPtr->UefiErrorRecord.RecLength + PS_PREPEND_DATA_SIZE  + PS_APPEND_DATA_SIZE);
                    }
                }
                break;
                }
                RecBefDel = RecBefDel + PersErrorRecordPtr->UefiErrorRecord.RecLength + PS_PREPEND_DATA_SIZE + PS_APPEND_DATA_SIZE;
                PersErrorRecordPtr = (PERSISTENT_ERROR_RECORD*)((UINT8*)PersErrorRecordPtr + PersErrorRecordPtr->UefiErrorRecord.RecLength + PS_PREPEND_DATA_SIZE + PS_APPEND_DATA_SIZE);
        } else {
            DEBUG((DEBUG_ERROR, "Exit-2: PersErrorRecordPtr: RecLength outside of SMRAM\n"));
            break;
        }
    }

    if (PersistentStoreModified == 1) {
      if ( mFreeSpaceInPersistentStore <= FV_WHEA_SIZE ) {

        //
        // Erasing block from base upto whea size
        //
        Status = Flash->Erase((UINT8*) FvWheaBase,(UINTN)FV_WHEA_SIZE);

        //
        // Writing record only if Erase is successful
        //
        if( Status == EFI_SUCCESS ) {

          //
          // Checking if record count register is not having zero and free space variable does not exceed FV_WHEA_SIZE
          //
          if ( mErstReg->RecordCount != 0 ) {
            Flash->Write((UINT8*)FvWheaBase, FV_WHEA_SIZE - mFreeSpaceInPersistentStore ,(UINT8*)mPersistentStoreBufferPtr);
          }
        }
      }
    }
}

/** @internal
   Searches the persistent store for given record id.If found then
   mark the error record as cleared.Subtract RecordCount by 1, if
   GetRecordId is pointing to SetRecordId, then make sure GetRecordId
   points to a valid error record
   
   @param[in]       void
   @return VOID     Nothing.

**/

UINT8
ClearErrorRecord (
  UINT64 RecordId )
{

    PERSISTENT_ERROR_RECORD *PersErrorRecordPtr;
    UINT16                   NumOfRecords;
    UINT16                   TmpNumOfRecords;
    EFI_STATUS               Status;
    UINT8                    CommandStatus = ERST_RECORD_NOT_FOUND;
    const UINT8              *EndOfPersistentRecordBuffer = (UINT8*)mPersistentStoreBufferPtr + FV_WHEA_SIZE; 

    UINT64               TempAddress;
    VOID                 *FvWheaBase;

    TempAddress = FV_WHEA_BASE;
    FvWheaBase = (VOID*)TempAddress;

    PersErrorRecordPtr = (PERSISTENT_ERROR_RECORD*)mPersistentStoreBufferPtr;

    for (NumOfRecords=mTotalErrorRecord;NumOfRecords>0;NumOfRecords--) {
        
        /// AmiValidateSmramBuffer(): Restricting UefiErrorRecord.RecLength for access to any value outside of SMRAM.
        Status = AmiValidateSmramBuffer((UINT8*)PersErrorRecordPtr, PersErrorRecordPtr->UefiErrorRecord.RecLength);
        if (EFI_ERROR(Status)){
            CommandStatus = ERST_FAILED;
            DEBUG((DEBUG_ERROR, "%a() Exit-1: PersErrorRecordPtr: Buffer outside of SMRAM\n", __FUNCTION__));
            break;
        }
        
        /// Restricting that UefiErrorRecord.RecLength should not cross the WHEA Buffer.
        if(((UINT8*)PersErrorRecordPtr + PersErrorRecordPtr->UefiErrorRecord.RecLength) > EndOfPersistentRecordBuffer) {
            DEBUG((DEBUG_ERROR, "%a() Exit-2: PersErrorRecordPtr: RecLength outside of SMRAM\n", __FUNCTION__));
            CommandStatus = ERST_FAILED;
            break;
        }

      if (PersErrorRecordPtr->UefiErrorRecord.RecordID == RecordId && PersErrorRecordPtr->NvPrependData.RecordValidity ==  FLASH_ERASE_VALUE) {

        mErstReg->RecordCount = mErstReg->RecordCount-1;
        PersErrorRecordPtr->NvPrependData.RecordValidity = MARKED_AS_DELETE;
        Status = Flash->Write((UINT8*)FvWheaBase+ ((UINT64)(UINTN)&PersErrorRecordPtr->NvPrependData.RecordValidity) - (UINT64)((UINTN)mPersistentStoreBufferPtr),1, (UINT8*)(&PersErrorRecordPtr->NvPrependData.RecordValidity));

        if( EFI_ERROR(Status) ) {
          CommandStatus = ERST_FAILED;  // Clearing failed
          break;
        }

        if (RecordId == mErstReg->ValidRecordIdentifier) {
          if ( mErstReg->RecordCount == 0) {
            mErstReg->ValidRecordIdentifier = 0xffffffffffffffff;
          } else {
              PersErrorRecordPtr = (PERSISTENT_ERROR_RECORD*)((UINT8*)PersErrorRecordPtr + PersErrorRecordPtr->UefiErrorRecord.RecLength + PS_PREPEND_DATA_SIZE + PS_APPEND_DATA_SIZE);
              NumOfRecords = NumOfRecords-1;
              TmpNumOfRecords = NumOfRecords;
              for (;NumOfRecords>0;NumOfRecords--) {
                if (PersErrorRecordPtr->NvPrependData.RecordValidity ==  FLASH_ERASE_VALUE) {
                  mErstReg->ValidRecordIdentifier = PersErrorRecordPtr->UefiErrorRecord.RecordID;
                  break;
                }
              }
              if( RecordId == mErstReg->ValidRecordIdentifier) {
                PersErrorRecordPtr = (PERSISTENT_ERROR_RECORD*)mPersistentStoreBufferPtr;
                for (;TmpNumOfRecords>0;TmpNumOfRecords--) {
                  if (PersErrorRecordPtr->NvPrependData.RecordValidity ==  FLASH_ERASE_VALUE) {
                    mErstReg->ValidRecordIdentifier = PersErrorRecordPtr->UefiErrorRecord.RecordID;
                    break;
                  }
                }
              }
          }
        }
        CommandStatus = ERST_SUCCESS;
        break;
      }
    PersErrorRecordPtr = (PERSISTENT_ERROR_RECORD*)((UINT8*)PersErrorRecordPtr + PersErrorRecordPtr->UefiErrorRecord.RecLength + PS_PREPEND_DATA_SIZE + PS_APPEND_DATA_SIZE);

    }

    return CommandStatus;
}


/** @internal
   Clears the given error record if it is already available in the persistent store,
   If there is no space in persistent store then delete all records marked as clear,
   Check again if there is free space in persistent store to write a error record, 
   If so append the error record to the persistent store.Increment RecordCount by 1,
   If previously persistent store is empty, then GetRecordId should point to the 
   newly written record, Gets error record from error log address range,if its a
   valid record write the error record to persistent store
   @param[in] void
   @return VOID    Nothing.

**/
VOID
ErstWriteErrorRecord ( void )
{
    UEFI_ERROR_RECORD_HEADER  *ErrorRecordPtr;
    EFI_STATUS                 Status;
    UINT8                      CommandStatus = ERST_SUCCESS;
    PERSISTENT_ERROR_RECORD   *PersistentErrorRecordPtr;
    NV_APPEND_DATA            *NvAppendDataPtr;

    UINT64                     TempAddress;
    VOID                       *FvWheaBase;
    const UINT8                *EndOfElarRegionBuffer = (UINT8*)mElarPtr + ELAR_SIZE; 

    TempAddress = FV_WHEA_BASE;
    FvWheaBase = (VOID*)TempAddress;

    //
    //Set Busy Status
    //
    mErstReg->BusyStatus = 1;

    /// Restricting RecordOffset should not be outside ELAR Region.
    if (((UINT8*)mElarPtr + mErstReg->RecordOffset) > EndOfElarRegionBuffer) {
        DEBUG((DEBUG_ERROR, "%a() Exit-1: mErstReg->RecordOffset: outside of ELAR Region\n", __FUNCTION__));
        CommandStatus = ERST_FAILED;
        goto Exit;
    }
    
    //
    //Get Error Record from Error Log Address Range
    //
    ErrorRecordPtr = (UEFI_ERROR_RECORD_HEADER*)((UINT8*)mElarPtr + mErstReg->RecordOffset);

    /// Restricting RecLength should not be outside ELAR Region.
    if (((UINT8*)ErrorRecordPtr + ErrorRecordPtr->RecLength) > EndOfElarRegionBuffer) {
        DEBUG((DEBUG_ERROR, "%a() Exit-2: ErrorRecordPtr->RecLength: outside of ELAR Region\n", __FUNCTION__));
        CommandStatus = ERST_FAILED;
        goto Exit;
    }
    
    //
    //Check if Error Record is a valid error record
    //
    if (ErrorRecordPtr->SignatureStart  == 'REPC' && ErrorRecordPtr->SignatureEnd == 0xffffffff) {

      //
      // Checks if record is already present in pstore. If so, then it will be cleared and this new record will be written instead.
      //
      ClearErrorRecord(ErrorRecordPtr->RecordID);

      //
      // Checks for free space in pstore. If free space is not available, records marked for delete will
      // will be permanently removed from pstore.
      //
      if (mFreeSpaceInPersistentStore < (ErrorRecordPtr->RecLength+PS_PREPEND_DATA_SIZE+PS_APPEND_DATA_SIZE)) {
        DeleteMarkedErrorRecord();
      }

      //
      // Check if free space is exceeding flash whea size allocated and free space is enough to hold the current record size
      //
      if ( ( mFreeSpaceInPersistentStore >= (ErrorRecordPtr->RecLength+PS_PREPEND_DATA_SIZE+PS_APPEND_DATA_SIZE) ) && ( mFreeSpaceInPersistentStore <= FV_WHEA_SIZE ) ) {

        //
        // For the first valid record to be written, update ValidRecordIdentifier field with current record's id
        //
        if (mErstReg->ValidRecordIdentifier == 0xffffffffffffffff) {
          mErstReg->ValidRecordIdentifier = ErrorRecordPtr->RecordID;
        }

        Status = AmiValidateMemoryBuffer((UINT8*)ErrorRecordPtr,  ErrorRecordPtr->RecLength);
        if (EFI_ERROR(Status)){
            DEBUG((DEBUG_ERROR, "%a() Exit-3: ErrorRecordPtr->RecLength: outside of ELAR Region\n", __FUNCTION__));
            CommandStatus = ERST_FAILED;
            goto Exit;
        }
        
        //
        // Update persistent store buffer with header and footer(Magic Num, RecordValidity field)
        //
        CopyMem ((UINT8*)mPersistentStoreBufferPtr + (FV_WHEA_SIZE - mFreeSpaceInPersistentStore+PS_PREPEND_DATA_SIZE), ErrorRecordPtr, ErrorRecordPtr->RecLength);
        PersistentErrorRecordPtr = (PERSISTENT_ERROR_RECORD*)((UINT8*)mPersistentStoreBufferPtr + (FV_WHEA_SIZE - mFreeSpaceInPersistentStore));
        PersistentErrorRecordPtr->NvPrependData.MagicNum = WHEA_PREPEND_MAGIC_NUMBER;
        PersistentErrorRecordPtr->NvPrependData.RecordValidity = FLASH_ERASE_VALUE;
        NvAppendDataPtr = (NV_APPEND_DATA*)((UINT8*)PersistentErrorRecordPtr + ErrorRecordPtr->RecLength + PS_PREPEND_DATA_SIZE);
        NvAppendDataPtr->MagicNum = WHEA_APPEND_MAGIC_NUMBER;

          Status = Flash->Write((UINT8*)FvWheaBase+(FV_WHEA_SIZE - mFreeSpaceInPersistentStore), ErrorRecordPtr->RecLength + PS_PREPEND_DATA_SIZE + PS_APPEND_DATA_SIZE, (UINT8*)PersistentErrorRecordPtr);
          //  DEBUG ((0xffffffff, "EXIT ErstWriteErrorRecordflash %r\n", Status));
          if( !(EFI_ERROR(Status)) ) {
           //
           // Increment number of records, total records and modifying free space in pstore only if write to flash is successful
           //
            mErstReg->RecordCount = mErstReg->RecordCount + 1;
            mTotalErrorRecord = mTotalErrorRecord + 1;
            mFreeSpaceInPersistentStore = mFreeSpaceInPersistentStore - (ErrorRecordPtr->RecLength + PS_PREPEND_DATA_SIZE + PS_APPEND_DATA_SIZE);
          } else {
              CommandStatus = ERST_FAILED;  // Write Failed
          }
      } else {
          CommandStatus = ERST_NOT_ENOUGH_SPACE;  //Free space is still not enough.
      }

    } else {
        CommandStatus = ERST_RECORD_NOT_FOUND;  // Proper Error Record is not provided in ELAR area
    }

Exit:     
    
    mErstReg->OperationStatus = CommandStatus;
    mErstReg->BusyStatus = 0;
    
    return;
}

/** @internal
   Search the persistent store for given record id.If the error record is found
   then transfer the record to the error log address range update GetRecordId
   register to point to next error record

    @param[in] void
    @return VOID    Nothing.

**/
VOID
ErstReadErrorRecord ( void )
{

    PERSISTENT_ERROR_RECORD          *PersErrorRecordPtr;
    UINT16                            NumOfRecords;
    UINT16                            TmpNumOfRecords;
    UINT8                             CommandStatus = ERST_RECORD_NOT_FOUND;
    BOOLEAN                           RecFound = FALSE, ValidIdSet = FALSE;
    ERROR_RECORD_SERIALIZATION_INFO  *ErrorRecordSerInfoPtr;
    UEFI_ERROR_RECORD_HEADER         *UefiErrorRecordPtr;
    EFI_STATUS                       Status;
    const UINT8                      *EndOfPersistentRecordBuffer = (UINT8*)mPersistentStoreBufferPtr + FV_WHEA_SIZE; 
    const UINT8                      *EndOfElarRegionBuffer = (UINT8*)mElarPtr + ELAR_SIZE; 

//
// 1. ACPI SPEC50  - 18.5.5.2 Reading
// Sets some internal state to indicate that it is busy. OSPM polls by executing a
// CHECK_BUSY_STATUS action until the operation is completed.
//
    //
    //Set Busy Status
    //
    mErstReg->BusyStatus = 1;

//
// 2.a. ACPI SPEC50  - 18.5.5.2 Reading
// If the identifier is 0x0 (unspecified), the platform reads the `first' error record from its persistent store. 
// First, is first valid error record in pstore.
//
    PersErrorRecordPtr = (PERSISTENT_ERROR_RECORD*)mPersistentStoreBufferPtr;

    if (mErstReg->OSRecordIdentifier==0 && mErstReg->RecordCount != 0) {

      for (NumOfRecords=mTotalErrorRecord;NumOfRecords>0;NumOfRecords--) {
          
          /// AmiValidateSmramBuffer(): Restricting UefiErrorRecord.RecLength for access to any value outside of SMRAM.
          Status = AmiValidateSmramBuffer((UINT8*)PersErrorRecordPtr, PersErrorRecordPtr->UefiErrorRecord.RecLength);
          if (EFI_ERROR(Status)){
              CommandStatus = ERST_FAILED;
              DEBUG((DEBUG_ERROR, "%a() Exit-1: PersErrorRecordPtr: Buffer outside of SMRAM\n", __FUNCTION__));
              break;
          }          
          /// Restricting that UefiErrorRecord.RecLength should not cross the WHEA Buffer.
          if(((UINT8*)PersErrorRecordPtr + PersErrorRecordPtr->UefiErrorRecord.RecLength) > EndOfPersistentRecordBuffer) {
              DEBUG((DEBUG_ERROR, "%a() Exit-2: PersErrorRecordPtr: RecLength outside of SMRAM\n", __FUNCTION__));
              CommandStatus = ERST_FAILED;
              break;
          }
          
        if (PersErrorRecordPtr->NvPrependData.RecordValidity ==  FLASH_ERASE_VALUE) {
            
            /// Restricting RecordOffset outside ELAR Region. It should avoid underflow as well.
            if (((UINT8*)mElarPtr + mErstReg->RecordOffset) > EndOfElarRegionBuffer) {
                DEBUG((DEBUG_ERROR, "%a() Exit-2: RecordOffset: outside of ELAR Region\n", __FUNCTION__));
                CommandStatus = ERST_FAILED;
                break;
            }
            
          if ( ( ELAR_SIZE - mErstReg->RecordOffset ) >=  PersErrorRecordPtr->UefiErrorRecord.RecLength) {
              Status = AmiValidateMemoryBuffer((UINT8*)mElarPtr + mErstReg->RecordOffset, PersErrorRecordPtr->UefiErrorRecord.RecLength);
              if (EFI_ERROR(Status)){
                  CommandStatus = ERST_FAILED;
                  break;
              }
              CopyMem ((UINT8*)mElarPtr + mErstReg->RecordOffset, &PersErrorRecordPtr->UefiErrorRecord, PersErrorRecordPtr->UefiErrorRecord.RecLength);
              RecFound = TRUE;
              CommandStatus = ERST_SUCCESS;
              break;
          } else {
              CommandStatus = ERST_NOT_ENOUGH_SPACE;
              break;
          }
        }
        PersErrorRecordPtr = (PERSISTENT_ERROR_RECORD*)((UINT8*)PersErrorRecordPtr + PersErrorRecordPtr->UefiErrorRecord.RecLength + PS_PREPEND_DATA_SIZE + PS_APPEND_DATA_SIZE);
      }

//
// 2.b. ACPI SPEC50  - 18.5.5.2 Reading
// If the identifier is non-zero, the platform attempts to locate the specified error record on the persistent store.
//
    } else {

        for (NumOfRecords=mTotalErrorRecord; NumOfRecords>0; NumOfRecords--) {
            
            /// Restricting that UefiErrorRecord.RecLength should not cross the WHEA Buffer.
            if(((UINT8*)PersErrorRecordPtr + PersErrorRecordPtr->UefiErrorRecord.RecLength) > EndOfPersistentRecordBuffer) {
                DEBUG((DEBUG_ERROR, "%a() Exit-3: PersErrorRecordPtr: RecLength outside of SMRAM\n", __FUNCTION__));
                CommandStatus = ERST_FAILED;
                break;
            }
            
            /// Restricting RecordOffset outside ELAR Region. It should avoid underflow as well.
            if (((UINT8*)mElarPtr + mErstReg->RecordOffset) > EndOfElarRegionBuffer) {
                DEBUG((DEBUG_ERROR, "%a() Exit-4: RecordOffset: outside of ELAR Region\n", __FUNCTION__));
                CommandStatus = ERST_FAILED;
                break;
            }
            
          if ( ( PersErrorRecordPtr->UefiErrorRecord.RecordID == mErstReg->OSRecordIdentifier ) && ( PersErrorRecordPtr->NvPrependData.RecordValidity ==  FLASH_ERASE_VALUE ) ) {
            if ( ELAR_SIZE - mErstReg->RecordOffset >=  PersErrorRecordPtr->UefiErrorRecord.RecLength) {

//
// 3. ACPI SPEC50  - 18.5.5.2 Reading
// Transfer the record from the persistent store to the offset specified by OSPM from the base of
// the Error Log Address Range.
//
              Status = AmiValidateMemoryBuffer((UINT8*)mElarPtr + mErstReg->RecordOffset, PersErrorRecordPtr->UefiErrorRecord.RecLength);
              if (EFI_ERROR(Status)){
                  CommandStatus = ERST_FAILED;
                  break;
              }
              CopyMem ((UINT8*)mElarPtr + mErstReg->RecordOffset, &PersErrorRecordPtr->UefiErrorRecord, PersErrorRecordPtr->UefiErrorRecord.RecLength);
              RecFound = TRUE;
              CommandStatus = ERST_SUCCESS;
              break;
            } else {
                CommandStatus = ERST_NOT_ENOUGH_SPACE;
                break;
            }
          }
         PersErrorRecordPtr = (PERSISTENT_ERROR_RECORD*)((UINT8*)PersErrorRecordPtr + PersErrorRecordPtr->UefiErrorRecord.RecLength + PS_PREPEND_DATA_SIZE + PS_APPEND_DATA_SIZE);
        }

    } // End of else

//
//  4. ACPI SPEC50  - 18.5.5.2 Reading
//  Record the Identifier of the `next' valid error record that resides on the persistent store.
//
        if ( (RecFound == TRUE) && (mErstReg->OSRecordIdentifier == mErstReg->ValidRecordIdentifier) ) {
              TmpNumOfRecords= mTotalErrorRecord - NumOfRecords;

              //
              // Looping from record following current record till end of pstore
              //
              for (;(NumOfRecords-1)>0;NumOfRecords--) {
                  /// Restricting that UefiErrorRecord.RecLength should not cross the WHEA Buffer.
                  if(((UINT8*)PersErrorRecordPtr + PersErrorRecordPtr->UefiErrorRecord.RecLength) > EndOfPersistentRecordBuffer) {
                      DEBUG((DEBUG_ERROR, "%a() Exit-5: PersErrorRecordPtr: RecLength outside of SMRAM\n", __FUNCTION__));
                      break;
                  }
                  
                PersErrorRecordPtr = (PERSISTENT_ERROR_RECORD*)((UINT8*)PersErrorRecordPtr + PersErrorRecordPtr->UefiErrorRecord.RecLength + PS_PREPEND_DATA_SIZE + PS_APPEND_DATA_SIZE);
                if (PersErrorRecordPtr->NvPrependData.RecordValidity ==  FLASH_ERASE_VALUE) {
                  mErstReg->ValidRecordIdentifier = PersErrorRecordPtr->UefiErrorRecord.RecordID;
                  ValidIdSet = TRUE;
                  break;
                }
              }

              //
              // If search from current record to remaining records is done and no valid record was found,
              // move pointer again to beg of pstore and begin search for valid rec.
              //
              if( ValidIdSet == FALSE ) {
                  
                  PersErrorRecordPtr = (PERSISTENT_ERROR_RECORD*)mPersistentStoreBufferPtr;
                  for (;TmpNumOfRecords>0;TmpNumOfRecords--) {

                      /// Restricting that UefiErrorRecord.RecLength should not cross the WHEA Buffer.
                      if(((UINT8*)PersErrorRecordPtr + PersErrorRecordPtr->UefiErrorRecord.RecLength) > EndOfPersistentRecordBuffer) {
                          DEBUG((DEBUG_ERROR, "%a() Exit-6: PersErrorRecordPtr: RecLength outside of SMRAM\n", __FUNCTION__));
                          break;
                      }
                    
                      if ( PersErrorRecordPtr->NvPrependData.RecordValidity ==  FLASH_ERASE_VALUE ) {
                        mErstReg->ValidRecordIdentifier = PersErrorRecordPtr->UefiErrorRecord.RecordID;
                        ValidIdSet = TRUE;
                        break;
                      }
                      PersErrorRecordPtr = (PERSISTENT_ERROR_RECORD*)((UINT8*)PersErrorRecordPtr + PersErrorRecordPtr->UefiErrorRecord.RecLength + PS_PREPEND_DATA_SIZE + PS_APPEND_DATA_SIZE);
                  }
              }

              //
              // If ValidId is still false, it means there are no valid records in pstore other than the currently read one.
              //
              if( ValidIdSet == FALSE ) {
                mErstReg->ValidRecordIdentifier = 0xffffffffffffffff;
              }
        } // End if RecFound == TRUE

//
// 2.d. Reading ACPI SPEC50
//If the specified error record does not exist, set the status register
//Status to Record Not Found (0x05), and update the status register's Identifier field with the identifier of the first error record.
//
        if (( CommandStatus == ERST_RECORD_NOT_FOUND ) && ( mErstReg->RecordCount != 0 ) ){
             PersErrorRecordPtr = (PERSISTENT_ERROR_RECORD*)mPersistentStoreBufferPtr;
             TmpNumOfRecords= mTotalErrorRecord;
                for (;TmpNumOfRecords>0;TmpNumOfRecords--) {
                    
                    
                    /// Restricting that UefiErrorRecord.RecLength should not cross the WHEA Buffer.
                    if(((UINT8*)PersErrorRecordPtr + PersErrorRecordPtr->UefiErrorRecord.RecLength) > EndOfPersistentRecordBuffer) {
                        DEBUG((DEBUG_ERROR, "%a() Exit-7: PersErrorRecordPtr: RecLength outside of SMRAM\n", __FUNCTION__));
                        break;
                    }
                    
                    if (PersErrorRecordPtr->NvPrependData.RecordValidity ==  FLASH_ERASE_VALUE) {
                        mErstReg->ValidRecordIdentifier = PersErrorRecordPtr->UefiErrorRecord.RecordID;
                        ValidIdSet = TRUE;
                        break;
                     }
                     PersErrorRecordPtr = (PERSISTENT_ERROR_RECORD*)((UINT8*)PersErrorRecordPtr + PersErrorRecordPtr->UefiErrorRecord.RecLength + PS_PREPEND_DATA_SIZE + PS_APPEND_DATA_SIZE);
               }
        }

        //
        //BUG FIX
        //
        //Windows 2008 server rc1 BUG, windows 2008 server expects Atributes field to be 0.
        //This bug may be fixed in future version of windows, until then need this code here.
        //
        
        if (((UINT8*)mElarPtr + mErstReg->RecordOffset) <  EndOfElarRegionBuffer) {
            
            UefiErrorRecordPtr= (UEFI_ERROR_RECORD_HEADER*)((UINT8*)mElarPtr + mErstReg->RecordOffset);
            ErrorRecordSerInfoPtr = (ERROR_RECORD_SERIALIZATION_INFO *) &UefiErrorRecordPtr->OSReserved;
            ErrorRecordSerInfoPtr->Attributes = 0;
        } else {
            DEBUG((DEBUG_ERROR, "%a() Exit-8: RecordOffset: outside of ELAR Region\n", __FUNCTION__));
            CommandStatus = ERST_FAILED;
        }
        

    if (mErstReg->RecordCount == 0) {
      mErstReg->OperationStatus = ERST_RECORD_STORE_EMPTY;
    } else {
        mErstReg->OperationStatus = CommandStatus;
    }
    mErstReg->BusyStatus = 0;

}

/**@internal
  Clears the Error record from persistent store.Calls ClearErrorRecord function.

  @param[in] Void        Protocol unique ID.

  @retval    Void.
**/
VOID 
ErstClearErrorRecord ( void )
{
    UINT8                CommandStatus;

    //
    //Set Busy Status
    //
    mErstReg->BusyStatus = 1;
    CommandStatus = ClearErrorRecord(mErstReg->OSRecordIdentifier);
    mErstReg->OperationStatus = CommandStatus;
    mErstReg->BusyStatus = 0;

}

/** @internal
    Updates the OperationStatus and Clears BusyStatus.
    @param[in] void
    @return VOID    Nothing.

**/
VOID 
ErstDummyWriteErrorRecord ( void )
{

    mErstReg->OperationStatus = ERST_SUCCESS;
    mErstReg->BusyStatus = 0;

}

/**
  ERST SMI Handler.

  @param[in]       DispatchMesaHandle  Dispatcher Handler
  @param[in]       Context         A pointer to the dispatch function's context.
  @param[in, out]  SwContext       A pointer to the Sw Context
  @param[in, out]  CommBufferSize  A pointer to the size of communication buffor

  @retval  EFI_SUCCESS
  @retval  EFI_INVALID_PARAMETER  Index out of range or null pointer passed to the function
  @retval  EFI_PROTOCOL_ERROR     ME reports error, detailed status in pStatus
  @retval  EFI_BAD_BUFFER_SIZE    Buffer size out of range
  @retval  EFI_DEVICE_ERROR       HECI communication error
  @retval  EFI_UNSUPPORTED        Implementation not supported
**/

EFI_STATUS
EFIAPI
ErstSWSMIHandler(
  IN EFI_HANDLE                        DispatchHandle,
  IN CONST EFI_SMM_SW_REGISTER_CONTEXT *RegisterContext,
  IN OUT EFI_SMM_SW_CONTEXT            *SwContext,
  IN OUT UINTN                         *CommBufferSize
)
{
    DEBUG((DEBUG_INFO,"%a ENTRY \n", __FUNCTION__));
    DEBUG((DEBUG_INFO,"mErstReg->CmdReg:%d \n", mErstReg->CmdReg));
    
    if (mErstReg->CmdReg == ERST_BEGIN_WRITE_OPERATION) {
      ErstWriteErrorRecord();

    } else if(mErstReg->CmdReg == ERST_BEGIN_READ_OPERATION) {
       ErstReadErrorRecord();

    } else if(mErstReg->CmdReg == ERST_BEGIN_CLEAR_OPERATION) {
       ErstClearErrorRecord();

    } else if(mErstReg->CmdReg == ERST_BEGIN_DUMMY_WRITE_OPERATION) {
       ErstDummyWriteErrorRecord();
    }
    
    DEBUG((DEBUG_INFO,"%a EXIT \n", __FUNCTION__));
    return EFI_SUCCESS;

}
// APTIOV_SERVER_OVERRIDE_RC_END : Using AMI WHEA ERST implementation.
