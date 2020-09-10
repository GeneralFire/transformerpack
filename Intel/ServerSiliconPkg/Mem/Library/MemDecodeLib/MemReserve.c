/** @file

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

#include <PiPei.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Guid/ReservedMemoryData.h>
#include <Library/SysHostPointerLib.h>
#include <Include/SysHost.h>
#include <Library/MemTypeLib.h>
#include <MemDecodeCommonIncludes.h>

/**

  Fetch a pointer to the gEfiReservedMemoryDataGuid GUID HOB data. If the HOB does not exist,
  create a new instance and initialize it.

  @return A pointer to the gEfiReservedMemoryDataGuid GUID HOB data.

**/
RESERVED_MEMORY_DATA_HOB *
GetReservedMemoryDataHob (
  VOID
  )
{
  EFI_HOB_GUID_TYPE             *GuidHob;
  RESERVED_MEMORY_DATA_HOB ReservedHighMemory;

  GuidHob = GetFirstGuidHob (&gEfiReservedMemoryDataGuid);
  if (GuidHob != NULL) {
    return GET_GUID_HOB_DATA (GuidHob);
  }

  //
  // This is the first time we are accessing this HOB. Initalize an instance
  // and use it to create the GUID HOB
  //
  ZeroMem(&ReservedHighMemory, sizeof(ReservedHighMemory));
  return BuildGuidDataHob (
           &gEfiReservedMemoryDataGuid,
           (VOID*) &ReservedHighMemory,
           sizeof (ReservedHighMemory)
           );
}

/**

  Find the highest system address that is local to the input Socket. The limit corresponds to memory
  that matches the input MemType.

  @param[in]  MemMapData             Pointer to the memory map private data structure.
  @param[in]  Socket                 The returned limit will be local to this input Socket
  @param[in]  MemType                The corresponding memory range will match this input MemType
  @param[out] NumaRangeLimit         Pointer to output variable. The referenced variable is populated
                                     with the limit address. The output value is in units of Bytes.
  @param[out] NumaRangeSize          Pointer to output variable. The referenced variable is populated
                                     with the size of the continuous memory region that is local to
                                     the input socket. The ouput value is in units of Bytes.

  @retval EFI_SUCCESS                The Limit and Size were successfully calculated.
  @retval EFI_OUT_OF_RESOURCES       There is no memory that is local to the input Socket.
  @retval EFI_UNSUPPORTED            The requested configuration is not supported.
  @retval EFI_INVALID_PARAMETER      One or more input values are invalid.

**/

EFI_STATUS
GetSocketHighMemNumaLimit (
  IN     MEMORY_MAP_DATA      *MemMapData,
  IN     UINT8                Socket,
  IN     MEM_TYPE             MemType,
     OUT EFI_PHYSICAL_ADDRESS *NumaRangeLimit,
     OUT EFI_PHYSICAL_ADDRESS *NumaRangeSize
  )
{
  SYSHOST            *Host;
  UINT8              SadIndex;
  SAD_TABLE          *SadEntry;
  BOOLEAN            SadEntryFound;
  UINT32             MaxSadLimit;
  UINT64             NumaRangeBase;

  Host = GetSysHostPointer ();
  SadEntryFound  = FALSE;
  NumaRangeBase = MAX_UINT64;

  //
  // Only 1LM Memory Type Reservation is supported
  //
  if (MemType != MemType1lmDdr) {
    return EFI_UNSUPPORTED;
  }

  //
  // Make sure the output variable pointers are not NULL
  //
  if (NumaRangeLimit == NULL || NumaRangeSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (SadIndex = 0; SadIndex < MemMapData->MaxSadRules; SadIndex++) {
    SadEntry = &Host->var.mem.socket[Socket].SAD[SadIndex];
    if (!SadEntry->local) {
      continue;
    }

    //
    // Only check volatile memory type for ddr4
    //
    if (!IsMemType1lm (SadEntry->type)) {
      continue;
    }

    //
    // Don't report memory below 4GB
    //
    if (NumaRangeBase == MAX_UINT64 && SadEntry->Base >= FOUR_GB_MEM) {
      NumaRangeBase = LShiftU64 (SadEntry->Base, CONVERT_64MB_TO_B);
    }

    //
    // Use the SAD limit of the Socket's highest DDR4 entry in the SAD table
    //
    MaxSadLimit = SadEntry->Limit;

    //
    //Flag to track if local Sad entries present for the Socket
    //
    SadEntryFound = TRUE;
  }

  if (!SadEntryFound) {
    //
    // No local memory available for the requested socket
    //
    return EFI_OUT_OF_RESOURCES;
  }

  *NumaRangeLimit = LShiftU64 (MaxSadLimit, CONVERT_64MB_TO_B); //Convert to Bytes

  RcDebugPrint (SDBG_MAX, "\n NumaRangeLimit %llx\n", *NumaRangeLimit);

  //
  // Calculate the Maximum available memory for this socket
  //
  *NumaRangeSize = *NumaRangeLimit - NumaRangeBase;

  return EFI_SUCCESS;
}

/**

  Reserve system memory that is local to the input Socket. This function will only reserve
  memory in the high address (>4GB) region.

  @param[in]  MemMapData             Pointer to the memory map private data structure.
  @param[in]  Socket                 Allocated memory will be local to this input Socket index.
  @param[in]  MemType                The type of memory to use for the reserved memory region
  @param[in]  RequestedSize          The size of the memory range to reserve in units of Bytes.
  @param[out] AllocatedAddress       Pointer to output variable. The referenced variable is populated
                                     with the base address of the newly reserved memory region. The
                                     output value is in units of bytes.

  @retval EFI_SUCCESS                The memory range was successfully reserved.
  @retval EFI_OUT_OF_RESOURCES       There are insufficient resources to process the request.
  @retval EFI_UNSUPPORTED            The requested configuration is not supported.
  @retval EFI_INVALID_PARAMETER      One or more input values are invalid.

**/
EFI_STATUS
ReserveLocalSocketMemory (
  IN     MEMORY_MAP_DATA      *MemMapData,
  IN     UINT8                Socket,
  IN     MEM_TYPE             MemType,
  IN     UINT64               RequestedSize,
     OUT EFI_PHYSICAL_ADDRESS *AllocatedAddress
  )
{
  EFI_STATUS               Status;
  UINT64                   ReservedMemoryBaseAddress;
  UINT64                   ReservedMemorySize;
  UINT64                   TotalReservableMemory;
  UINT64                   RemainingReservableMemory;
  RESERVED_MEMORY_DATA_HOB *ReservedHighMemory;

  ReservedHighMemory = GetReservedMemoryDataHob ();
  if (ReservedHighMemory == NULL) {
    //
    // Something went wrong accessing the Reserved Memory Data
    //
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Only 1LM Memory Type Reservation is supported
  //
  if (MemType != MemType1lmDdr) {
    return EFI_UNSUPPORTED;
  }

  if (RequestedSize == 0 || AllocatedAddress == NULL) {
      return EFI_INVALID_PARAMETER;
  }

  *AllocatedAddress = (UINT64) NULL;
  //
  // Check if there's memory already reseverd on this socket
  //
  if (ReservedHighMemory->IsMemoryReserved[Socket]) {
    ReservedMemoryBaseAddress = ReservedHighMemory->ReservedMemoryBaseAddress[Socket];
    ReservedMemorySize = ReservedHighMemory->ReservedMemorySize[Socket];
    TotalReservableMemory = ReservedHighMemory->TotalReservableMemory[Socket];
  } else {
    // If there's no reserved memory present, find the socket NUMA range limit and use that as the Base Adddress
    // when growing the reserved memory. The reserved memory block will grow downwards (towards zero) in the
    // address space
    ReservedMemorySize = 0;
    Status = GetSocketHighMemNumaLimit(MemMapData, Socket, MemType, &ReservedMemoryBaseAddress, &TotalReservableMemory);
    if (EFI_ERROR(Status)) {
      return Status;
    }
  }

  RemainingReservableMemory = (TotalReservableMemory - ReservedMemorySize);

  //
  // Check if there's enough reservable memory left to allocate the requested memory size
  //
  if (RemainingReservableMemory < RequestedSize) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Allocate reserved memory
  //
  ReservedHighMemory->ReservedMemoryBaseAddress[Socket] = ReservedMemoryBaseAddress - RequestedSize;
  ReservedHighMemory->ReservedMemorySize[Socket] = ReservedMemorySize + RequestedSize;
  ReservedHighMemory->TotalReservableMemory[Socket] = TotalReservableMemory;
  ReservedHighMemory->IsMemoryReserved[Socket] = TRUE;

  //
  // Return the base address
  //
  *AllocatedAddress = ReservedHighMemory->ReservedMemoryBaseAddress[Socket];
  return EFI_SUCCESS;
}