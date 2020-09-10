/** @file
  This file implements an API for enabling IIO Sierra Peak Initialization.

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

#include <Base.h>
#include <PiDxe.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Library/DebugLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/MemMapDataLib.h>
#include <Library/BaseMemoryLib.h>

#include <IioSetupDefinitions.h>
#include <IioPlatformData.h>
#include <Library/IioDataHubLib.h>

#include <LibraryPrivate/IioSpkConfigure.h>
#include <LibraryPrivate/IioDebug.h>

#include "IioSpkInitDefs.h"

/**
  This routine returns the size of requested SPK buffer in bytes.

  @param[in] *IioGlobalData      - Pointer to Global Data Structure.
  @param[in] SocketId            - Index of the socket

  @return - size of the requested SPK buffer
 */
UINT32
IioSpkGetBufferSizeInBytes (
  IN IIO_GLOBALS          *IioGlobalData,
  IN UINT8                 SocketId
  )
{
  switch (IioGlobalData->SetupData.SierraPeakMemBufferSize[SocketId]) {
  case 0: return 0; // Buffer size = None
  case 1: return SIZE_1MB;
  case 2: return SIZE_8MB;
  case 3: return SIZE_64MB;
  case 4: return SIZE_128MB;
  case 5: return SIZE_256MB;
  case 6: return SIZE_512MB;
  case 7: return SIZE_1GB;
  default:
    IIO_D_SPKERR ("%a: Incorrect buffer size in setup! buff = %X\n",
        __FUNCTION__, IioGlobalData->SetupData.SierraPeakMemBufferSize[SocketId]);

    return 0;
  }
}

/**
  This function allocates Sierra Peak trace buffer memory.

  @param[in]       SocketId          - Index of the socket
  @param[in, out] *BufferBase        - Pointer to store base memory address of the allocated buffer
  @param[in]       BufSize           - Size of the single SPK instance buffer in bytes
  @param[in]      *SpkInstancesCount - Number of detected SPK instances

  @retval EFI_SUCCESS              The function completed successfully
  @retval EFI_OUT_OF_RESOURCES     Allocated memory failure
**/
EFI_STATUS
IioSpkAllocMemoryOnSocket (
  IN     UINT8                     SocketId,
  IN OUT EFI_PHYSICAL_ADDRESS     *BufferBase,
  IN     UINT32                    BufSize,
  IN     UINT8                    *SpkInstancesCount
  )
{
  struct SystemMemoryMapHob       *SystemMemoryMap;
  EFI_STATUS                       Status;
  UINT32                           Index;
  UINT8                            SpkIndex;
  EFI_PHYSICAL_ADDRESS             CurrentElementBegin;
  UINT8                            NotAllocatedIndex;

  IIO_D_SPKLOG ("%a: bytes to allocate: %X on socket:%d\n", __FUNCTION__, BufSize, SocketId);

  SystemMemoryMap = GetSystemMemoryMapData ();
  if (SystemMemoryMap == NULL) {
    ASSERT_EFI_ERROR (EFI_NOT_FOUND);
    return EFI_UNSUPPORTED;
  }

  NotAllocatedIndex = 0;
  Status = EFI_OUT_OF_RESOURCES;

  //
  // Iterate over memory elements
  //
  for (Index = 0; Index < SystemMemoryMap->numberEntries; Index++) {

    if (SystemMemoryMap->Element[Index].BaseAddress < MEM_4GB_AT_64MB_GRAN) {
      //
      // Skip the element contains memory < 4G
      //
      continue;
    }

    if ((SystemMemoryMap->Element[Index].Type == MemType1lmDdr || SystemMemoryMap->Element[Index].Type == MemType2lmDdrCacheMemoryMode) &&
        (SystemMemoryMap->Element[Index].SocketId == SocketId || SocketId == ANY_SOCKET_ID) &&
        LShiftU64 (SystemMemoryMap->Element[Index].ElementSize, CONVERT_64MB_TO_B) > BufSize) {

      //
      // This is memory element with memory on requested socket - try to allocate memory here
      //
      CurrentElementBegin = LShiftU64 (SystemMemoryMap->Element[Index].BaseAddress, CONVERT_64MB_TO_B);

      //
      // Set baseAddress at the end of this elements
      //
      BufferBase[NotAllocatedIndex] = CurrentElementBegin + LShiftU64 (SystemMemoryMap->Element[Index].ElementSize, CONVERT_64MB_TO_B);

      for (SpkIndex = NotAllocatedIndex; SpkIndex < *SpkInstancesCount; SpkIndex++) {

        if (SpkIndex != NotAllocatedIndex) {
          //
          // Set baseAddress to the beginning of the previous buffer in the same memory element
          //
          BufferBase[SpkIndex] = BufferBase[SpkIndex-1];
        }

        //
        // Seek for the free space to allocate from the end to beginning of memory element
        //
        do {
          BufferBase[SpkIndex] -= BufSize;                                  // Step back by the buffer size
          BufferBase[SpkIndex] &= ~( (EFI_PHYSICAL_ADDRESS) EFI_PAGE_MASK); // BaseAddress must be aligned to 4k pages

          if (CurrentElementBegin > BufferBase[SpkIndex]) {
            //
            // End of this memory chunk reached
            //
            Status = EFI_OUT_OF_RESOURCES;
            break;
          }

          IIO_D_SPKLOG ("BaseAddress for request: 0x%p, bufferSize: 0x%x\n", BufferBase[SpkIndex], BufSize);

          Status = gBS->AllocatePages (
                      AllocateAddress,
                      EfiReservedMemoryType,
                      EFI_SIZE_TO_PAGES(BufSize),
                      &BufferBase[SpkIndex]
                      );

          if (!EFI_ERROR (Status)) {
            //
            // Buffer allocated
            //
            IIO_D_SPKLOG ("%a: buffer for spk%d ready: %p\n", __FUNCTION__, SpkIndex, BufferBase[SpkIndex]);
            break;
          }
        } while (EFI_ERROR (Status));
     } // for (iteration of SPK instances)

     //
     // Save the last not allocated index to not start from the beginning
     //
     NotAllocatedIndex = SpkIndex;
    }
  } // for (iteration of memory elements)

  if (EFI_ERROR (Status)) {
    //
    // Free allocated memory if returns with error
    //
    for (SpkIndex = 0; SpkIndex < NotAllocatedIndex; SpkIndex++) {
      gBS->FreePages (BufferBase[SpkIndex], EFI_SIZE_TO_PAGES(BufSize));
    }
  }

  return Status;
}

/**
  This function allocates Sierra Peak trace buffer memory.

  @param[in]      *IioGlobaldata      - Pointer to Global Data Structure
  @param[in]       SocketId           - Index of the socket
  @param[in, out] *BufferBase         - Pointer to store base memory address of the allocated buffer
  @param[in]       BufSize            - Size of the single SPK instance buffer in bytes
  @param[in]      *SpkInstancesCount  - Number of detected SPK instances

  @retval EFI_SUCCESS              The function completed successfully
  @retval EFI_OUT_OF_RESOURCES     Allocated memory failure
**/
EFI_STATUS
IioSpkAllocMemory (
  IN     IIO_GLOBALS               *IioGlobalData,
  IN     UINT8                      SocketId,
  IN OUT EFI_PHYSICAL_ADDRESS      *BufferBase,
  IN     UINT32                     BufSize,
  IN     UINT8                     *SpkInstancesCount
  )
{
  UINT8                   AllocStage;
  UINT8                   Index;
  UINT8                   LastPresentSocketId;
  UINT8                   AllocateOnSocketId;
  EFI_STATUS              Status;

  enum AllocStageEnum {
    AllocOnLocalSocket = 0,
    AllocOnPreviousSocket,
    AllocOnNextSocket,
    AllocOnAnySocket
  };

  //
  // Find last present socket Id for current configuration
  //
  LastPresentSocketId = SocketId;
  for (Index = SocketId; Index < MAX_SOCKET; Index++) {
    if (IioGlobalData->IioVar.IioVData.SocketPresent[Index] != 0) {
      LastPresentSocketId = Index;
    }
  }

  Status = EFI_UNSUPPORTED;
  AllocateOnSocketId = SocketId; // default value for KW
  for (AllocStage = AllocOnLocalSocket; AllocStage <= AllocOnAnySocket; AllocStage++) {
    switch (AllocStage) {
      case AllocOnLocalSocket:
        AllocateOnSocketId = SocketId;
        IIO_D_SPKDBG ("Trying to allocate buffers on local socket=%d.\n", AllocateOnSocketId);
        break;

      case AllocOnPreviousSocket:
        //
        // Check if there are more sockets to try
        //
        if (LastPresentSocketId == 0) { // only one socket is present
          return Status;
        }
        AllocateOnSocketId = (SocketId != 0 )? SocketId - 1 : LastPresentSocketId;
        IIO_D_SPKDBG ("Trying to allocate buffers on previous socket=%d.\n", AllocateOnSocketId);
        break;

      case AllocOnNextSocket:
        //
        // Check if there are more sockets to try
        //
        if (LastPresentSocketId == 1) { //only two sockets are present
          return Status;
        }
        AllocateOnSocketId = (SocketId != LastPresentSocketId ) ? SocketId + 1 : 0;
        IIO_D_SPKDBG ("Trying to allocate buffers on next socket=%d.\n", AllocateOnSocketId);
        break;

      case AllocOnAnySocket:
        AllocateOnSocketId = ANY_SOCKET_ID;
        IIO_D_SPKDBG ("Finally trying to allocate buffers on any socket.\n");
        break;
    }

    Status = IioSpkAllocMemoryOnSocket (AllocateOnSocketId, BufferBase, BufSize, SpkInstancesCount);
    if (!EFI_ERROR (Status)) {
      //
      // Allocation is done.
      //
      return Status;
    }
  }

  return Status;
}

/**
  This routine initialize Sierra Peak Trace Buffer (SPK).
  It should be called at End-of-PEI.

  @param[in] *IioGlobaldata      - Pointer to Global Data Structure
  @param[in]  SocketId           - Index of the socket

  @retval EFI_SUCCESS       - SPK Trace Buffer configured successfully.
  @retval EFI_UNSUPPORTED   - If SPK not initialized.
**/
EFI_STATUS
IioSpkSocketInitialize (
  IN IIO_GLOBALS         *IioGlobalData,
  IN UINT8                SocketId
  )
{
  EFI_STATUS              Status;
  EFI_PHYSICAL_ADDRESS    BufferBase[MAX_SPK_INSTANCES_COUNT];
  UINT32                  BufSize;
  UINT8                   SpkInstancesCount;
  UINT32                  SpkOffsets[MAX_SPK_INSTANCES_COUNT];

  IIO_D_SPKLOG ("%a: SocketId: %d\n", __FUNCTION__, SocketId);

  if (IioGlobalData->SetupData.CpuTraceHubConfig[SocketId][0].EnableMode == TraceHubModeDisabled) {
    IIO_D_SPKDBG ("Trace Hub is disabled for socket=%d. Initializing Sierra Peak skipped.\n", SocketId);
    return EFI_UNSUPPORTED;
  }

  BufSize = IioSpkGetBufferSizeInBytes (IioGlobalData, SocketId);
  if (BufSize == 0) {
    IIO_D_SPKDBG ("Buffer size=NONE for socket=%d. Initializing Sierra Peak skipped.\n", SocketId);
    return EFI_SUCCESS;
  }

  //
  // Initialize array
  //
  ZeroMem (&BufferBase, sizeof(EFI_PHYSICAL_ADDRESS) * (MAX_SPK_INSTANCES_COUNT));

  Status = IioSpkDetectInstances (SocketId, &SpkInstancesCount, SpkOffsets);
  if (EFI_ERROR (Status)) {
    IIO_D_SPKERR ("IioIndex: %d SPK instances detection failed! (%r)\n", SocketId, Status);
    return EFI_UNSUPPORTED;
  }

  Status = IioSpkAllocMemory (IioGlobalData, SocketId, BufferBase, BufSize, &SpkInstancesCount);
  if (EFI_ERROR (Status)) {
    IIO_D_SPKERR ("%a: Can't allocate memory buffers for SPK for socket: %d\n", __FUNCTION__, SocketId);
    return Status;
  }

  Status = IioSpkConfigureTraceBuffers (IioGlobalData, SocketId, BufferBase, BufSize, &SpkInstancesCount, SpkOffsets);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  IIO_D_SPKLOG ("%a: Initialize successful for socket: %d\n", __FUNCTION__, SocketId);

  return EFI_SUCCESS;
}

/**
  Sierra Peak initialization.
**/
VOID
IioSpkInitialize (
  VOID
  )
{
  UINT8             IioIndex;
  IIO_GLOBALS      *IioGlobalData;

  if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    //
    // SPK is not supported by SKX
    //
    return;
  }

  if (EFI_ERROR (GetIioGlobalData (&IioGlobalData))) {
    IIO_D_SPKERR ("Failed to get IioGlobalData. \n");
    return;
  }

  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {
     if (IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex] == 0) {
      continue;
    }

    IioSpkSocketInitialize (IioGlobalData, IioIndex);
  }
}
