/** @file
  Implementation of generate RAS global data access for silicon library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/RasGlobalDataVariable.h>
#include <Library/RasGlobalDataLib.h>

#define MAX_GLOBAL_DATA_NAME_STRING_LEN 64
//
// Element of Ras global data table entry
//
typedef struct {
  CHAR16 DataName[MAX_GLOBAL_DATA_NAME_STRING_LEN];
  VOID*  Address;
} RAS_GLOBAL_DATA_ENTRY;

//
// Global data table definition, including current table length,
// allocated size of this table, and pointer to the list of table entries.
//
typedef struct {
  //
  // The number of valid entries in the RasGlobalDataEntry buffer
  //
  UINT32 TableLength;
  //
  // The size, in bytes, of the RasGlobalDataEntry buffer
  //
  UINT32 AllocatedSize;
  //
  // Pointer of RAS_GLOBAL_DATA_ENTRY structures.
  //
  RAS_GLOBAL_DATA_ENTRY *RasGlobalDataEntry;
} RAS_GLOBAL_DATA_TABLE;

/**
  The constructor function for Pei Ras global data library.

  @param  FileHandle  Handle of the file being invoked.
  @param  PeiServices Describes the list of possible PEI Services.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
InitializePeiRasGlobalData (
  IN EFI_PEI_FILE_HANDLE     FileHandle,
  IN CONST EFI_PEI_SERVICES  **PeiServices
  )
{
  VOID*                 Buffer;
  UINT64                RasGlobalDataTableAddress;

  //
  // If PcdRasGlobaldataTableAddress = NULL, allocate memory space for Ras global data table structure.
  //
  RasGlobalDataTableAddress = PcdGet64 (PcdRasGlobaldataTableAddress);
  if (RasGlobalDataTableAddress == 0) {
    Buffer = AllocatePool (sizeof(RAS_GLOBAL_DATA_TABLE));
    ZeroMem (Buffer, sizeof(RAS_GLOBAL_DATA_TABLE));
    PcdSet64S (PcdRasGlobaldataTableAddress, (UINT64) Buffer);
  }

  return EFI_SUCCESS;
}

/**

  Get Ras global data pointer.

  This function implements single design pattern to return the system global data for RAS usage.

  @param Name            NULL-terminated Unicode string as the unique key for the global variable.
  @param Size            The size of global data.
  @param NewAllocation   Indicate whether the data is newly allocated or not.

  @return Pointer to the global data or NULL if there is out of resource.

**/
VOID*
EFIAPI
GetRasGlobalData (
  IN  CHAR16         *Name,
  IN  UINTN          Size,
  OUT BOOLEAN        *NewAllocation OPTIONAL
  )
{
  VOID                      *Address;
  UINTN                     AllocatedPages;
  RAS_GLOBAL_DATA_TABLE     *RasGlobalDataTable;
  RAS_GLOBAL_DATA_ENTRY     *RasGlobalDataEntry;
  UINT32                    Index;

  RasGlobalDataTable = (RAS_GLOBAL_DATA_TABLE *) (UINTN) PcdGet64 (PcdRasGlobaldataTableAddress);
  RasGlobalDataEntry = RasGlobalDataTable->RasGlobalDataEntry;

  //
  // Look for global data name in glabal data table
  //
  for (Index = 0; Index < RasGlobalDataTable->TableLength; Index++) {
    if (StrCmp (Name, RasGlobalDataEntry[Index].DataName) == 0) {
      *NewAllocation = FALSE;
      return RasGlobalDataEntry[Index].Address;
    }
  }

  //
  // If glabal data table is full, allocate one more page for it.
  //
  if ((RasGlobalDataTable->TableLength + 1) * sizeof (RAS_GLOBAL_DATA_ENTRY) > RasGlobalDataTable->AllocatedSize) {
    AllocatedPages = EFI_SIZE_TO_PAGES (RasGlobalDataTable->AllocatedSize);
    Address  = AllocatePages (AllocatedPages + 1);

    if (Address == NULL) {
      DEBUG((DEBUG_ERROR, "Can't allocate enough memory space for Ras global data table!!!\n"));
      return NULL;
    }

    //
    // If there are records existing in the glabal table, then copy its contents
    // to new region and free the old one.
    //
    if (RasGlobalDataTable->AllocatedSize > 0) {
      CopyMem (
        Address,
        (VOID *) (UINTN) RasGlobalDataTable->RasGlobalDataEntry,
        RasGlobalDataTable->AllocatedSize
        );

      //
      // RasGlobalDataEntry is allocated by AllocatePages() service.
      // So, FreePages() servcie is used to free it.
      //
      FreePages (RasGlobalDataTable->RasGlobalDataEntry, AllocatedPages);
    }

    //
    // Adjust the allocated size and register table base address.
    //
    RasGlobalDataTable->AllocatedSize += EFI_PAGE_SIZE;
    RasGlobalDataTable->RasGlobalDataEntry = (RAS_GLOBAL_DATA_ENTRY *)Address;
    DEBUG((DEBUG_ERROR, "PcdRasGlobaldataTableAddress = 0x%x\n",PcdGet64 (PcdRasGlobaldataTableAddress)));
    DEBUG((DEBUG_ERROR, "RasGlobalDataEntry address = 0x%x\n",RasGlobalDataTable->RasGlobalDataEntry));
  }

  Address = AllocatePool (Size);
  if (Address == NULL) {
    DEBUG((DEBUG_ERROR, "Can't allocate enough memory space for Ras global data entry!!!\n"));
    return NULL;
  }

  //
  // Append entry in the global data table.
  //
  RasGlobalDataEntry = RasGlobalDataTable->RasGlobalDataEntry;
  StrCpyS (RasGlobalDataEntry[RasGlobalDataTable->TableLength].DataName, MAX_GLOBAL_DATA_NAME_STRING_LEN, Name);
  RasGlobalDataEntry[RasGlobalDataTable->TableLength].Address  = Address;
  RasGlobalDataTable->TableLength++;
  *NewAllocation = TRUE;

  return Address;
}