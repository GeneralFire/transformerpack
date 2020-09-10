/** @file
  SMM SysHost Pointer Library implementation using SMM protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

#include <SysHost.h>
#include <MemMapHost.h>

#include <Library/SysHostPointerLib.h>  // *THIS* library

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SmmMemLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HobLib.h>

#include <Protocol/SmmAccess2.h>
#include <Protocol/SmmReadyToLock.h>

#include "StaticPointerTableHelper.h"

//
// GUID for the private protocol holding this library's data.
//
EFI_GUID mPrivateStaticPointerProtocolGuid = {
  0xB0D35B3D, 0x8673, 0x4DFE, {0xB8, 0x75, 0x85, 0x6E, 0x79, 0x9D, 0xDD, 0x99}
};

//
// This GUID is not intended for use by other code
//
STATIC GUID  mStaticPointerTableGuid          = STATIC_POINTER_TABLE_GUID;

///
/// Private protocol structure for holding this library's private data.
///
typedef struct {
  BOOLEAN               SmmLockDone;        /// TRUE if SMM lock processing is done.
  STATIC_POINTER_TABLE  StaticPointerTable; /// Static pointer table structure.
  UINT32                SmramCount;         /// Array size of SmramRanges.
  EFI_SMRAM_DESCRIPTOR  SmramRanges[1];     /// Variable-size array, MUST be last.
} PRIVATE_STATIC_POINTER_PROTOCOL;

///
/// Macro for calculating the size of the data given the length of the
/// variable-length array at the end.
///
#define PRIVATE_DATA_SIZE_BY_SMRAM_COUNT(c) ( \
  sizeof (PRIVATE_STATIC_POINTER_PROTOCOL)    \
  - sizeof (EFI_SMRAM_DESCRIPTOR)             \
  + (sizeof (EFI_SMRAM_DESCRIPTOR) * (c))     \
  )

PRIVATE_STATIC_POINTER_PROTOCOL *mPrivate = NULL;
VOID                            *mRegistration = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED CHAR8 *mDebugSptStrings[SptMax] = {NULL};

/**
  Is a buffer in SMRAM?

  Check if a buffer's start address is safely within an SMRAM range.

  WARNING: Since we don't know the size, this does not necessarily mean the
  entire buffer is within SMRAM.

  @param[in]  Buffer    Start address of the buffer.

  @retval TRUE          The buffer is in SMRAM.
  @retval FALSE         The buffer is not in SMRAM.
**/
BOOLEAN
IsBufferInSmram (
  IN  EFI_PHYSICAL_ADDRESS  Buffer
  )
{
  UINTN                 Index;
  EFI_PHYSICAL_ADDRESS  Base;
  EFI_PHYSICAL_ADDRESS  Limit;

  for (Index = 0; Index < mPrivate->SmramCount; Index++) {
    Base = mPrivate->SmramRanges[Index].CpuStart;
    Limit = Base + mPrivate->SmramRanges[Index].PhysicalSize;
    if ((Buffer >= Base) && (Buffer < Limit)) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Gets SysHost pointer.

  This function abstracts the retrieval for SysHost pointer.
  This is designed to be very high performance for very high
  frequency operations.

  @return pointer to SysHost

**/
VOID *
EFIAPI
GetSysHostPointer (
  VOID
  )
{
  //
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID *) (UINTN) mPrivate->StaticPointerTable.SptEntries[SptSysHost];
}

/**
  Sets SysHost pointer.

  This function abstracts the setting for SysHost pointer.
  We may have implementation specific way to get this pointer.

  @param[in]   The pointer of SysHost pointer to store.

**/
VOID
EFIAPI
SetSysHostPointer (
  IN VOID *SysHost
  )
{
  ASSERT (SysHost != NULL);
  mPrivate->StaticPointerTable.SptEntries[SptSysHost] = (EFI_PHYSICAL_ADDRESS) (UINTN) SysHost;
  ASSERT (!mPrivate->SmmLockDone || IsBufferInSmram (mPrivate->StaticPointerTable.SptEntries[SptSysHost]));
}

/**
  Gets HBM Host pointer.

  This function abstracts the retrieval for HbmHost pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance for very high
  frequency operations.

  @param    None

  @return   Pointer to HbmHost

**/
VOID *
EFIAPI
GetHbmHostPointer (
  VOID
  )
{
  //
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID *) (UINTN) mPrivate->StaticPointerTable.SptEntries[SptHbmHost];
}

/**
  Sets HbmHost pointer.

  This function abstracts the setting for HbmHost pointer.
  We may have implementation specific way to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The pointer of HbmHost pointer to store.

  @return     None

**/
VOID
EFIAPI
SetHbmHostPointer (
  IN VOID *HbmHost
  )
{
  ASSERT (HbmHost != NULL);
  mPrivate->StaticPointerTable.SptEntries[SptHbmHost] = (EFI_PHYSICAL_ADDRESS) (UINTN) HbmHost;
  ASSERT (!mPrivate->SmmLockDone || IsBufferInSmram (mPrivate->StaticPointerTable.SptEntries[SptHbmHost]));
}

/**
  Gets CpgcHost pointer.

  This function abstracts the retrieval for CpgcHost pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance for very high
  frequency operations.

  @param    None

  @return   Pointer to CpgcHost

**/
VOID *
EFIAPI
GetCpgcHostPointer (
  VOID
  )
{
  ASSERT (FALSE);
  return NULL;
}

/**
  Sets CpgcHost pointer.

  This function abstracts the setting for CpgcHost pointer.
  We may have implementation specific way to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The pointer of CpgcHost pointer to store.

  @return     None

**/
VOID
EFIAPI
SetCpgcHostPointer (
  IN VOID *CpgcHost
  )
{
  ASSERT (FALSE);
}

/**
  Gets CpuAndRevision pointer.

  This function abstracts the retrieval for CpuAndRevision pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance for very high
  frequency operations.

  @param    None

  @return   Pointer to CpuAndRevision

**/
VOID *
EFIAPI
GetCpuAndRevisionPointer (
  VOID
  )
{
  //
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID *) (UINTN) mPrivate->StaticPointerTable.SptEntries[SptCpuAndRevision];
}

/**
  Sets CpuAndRevision pointer.

  This function abstracts the setting for CpuAndRevision pointer.
  We may have implementation specific way to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The pointer of CpuAndRevision pointer to store.

  @return     None

**/
VOID
EFIAPI
SetCpuAndRevisionPointer (
  IN VOID *CpuAndRevision
  )
{
  ASSERT (CpuAndRevision != NULL);
  mPrivate->StaticPointerTable.SptEntries[SptCpuAndRevision] = (EFI_PHYSICAL_ADDRESS) (UINTN) CpuAndRevision;
  ASSERT (!mPrivate->SmmLockDone || IsBufferInSmram (mPrivate->StaticPointerTable.SptEntries[SptCpuAndRevision]));
}

/**
  Gets SysInfoVar pointer.

  This function abstracts the retrieval for SysInfoVar pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance for very high
  frequency operations.

  @param    None

  @return   Pointer to SysInfoVar

**/
VOID *
EFIAPI
GetSysInfoVarPointer (
  VOID
  )
{
  ASSERT (FALSE);
  return NULL;
}

/**
  Sets SysInfoVar pointer.

  This function abstracts the setting for SysInfoVar pointer.
  We may have implementation specific way to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The pointer of SysInfoVar pointer to store.

  @return     None

**/
VOID
EFIAPI
SetSysInfoVarPointer (
  IN VOID *SysInfoVar
  )
{
  ASSERT (FALSE);
}

/**
  Gets SysSetup pointer.

  This function abstracts the retrieval for SysSetup pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance for very high
  frequency operations.

  @param    None

  @return   Pointer to SysSetup

**/
VOID *
EFIAPI
GetSysSetupPointer (
  VOID
  )
{
  //
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID *) (UINTN) mPrivate->StaticPointerTable.SptEntries[SptSysSetup];
}

/**
  Sets SysSetup pointer.

  This function abstracts the setting for SysSetup pointer.
  We may have implementation specific way to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The pointer of SysSetup pointer to store.

  @return     None

**/
VOID
EFIAPI
SetSysSetupPointer (
  IN VOID *SysSetup
  )
{
  ASSERT (SysSetup != NULL);
  mPrivate->StaticPointerTable.SptEntries[SptSysSetup] = (EFI_PHYSICAL_ADDRESS) (UINTN) SysSetup;
  ASSERT (!mPrivate->SmmLockDone || IsBufferInSmram (mPrivate->StaticPointerTable.SptEntries[SptSysSetup]));
}

/**
  Gets Semaphore Data pointer.

  This function abstracts the retrieval for the SemaphoreData pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance.

  @param    None

  @return   Pointer to SemaphoreData

**/
VOID *
EFIAPI
GetSemaphoreDataPointer (
  VOID
  )
{
  //
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID *) (UINTN) mPrivate->StaticPointerTable.SptEntries[SptSemaphoreData];
}

/**
  Sets the Semaphore Data pointer.

  This function abstracts the setting of the SemaphoreData pointer.
  We may have implementation specific ways to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The pointer of SemaphoreData pointer to store.

  @return     None

**/
VOID
EFIAPI
SetSemaphoreDataPointer (
  IN VOID *SemaphoreData
  )
{
  ASSERT (SemaphoreData != NULL);
  mPrivate->StaticPointerTable.SptEntries[SptSemaphoreData] = (EFI_PHYSICAL_ADDRESS) (UINTN) SemaphoreData;
  ASSERT (!mPrivate->SmmLockDone || IsBufferInSmram (mPrivate->StaticPointerTable.SptEntries[SptSemaphoreData]));
}

/**
  Gets SPD Data pointer.

  This function abstracts the retrieval for the SpdData pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance.

  @param    None

  @return   Pointer to SpdData

**/
VOID *
EFIAPI
GetSpdDataPointer (
  VOID
  )
{
  //
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID *) (UINTN) mPrivate->StaticPointerTable.SptEntries[SptSpdData];
}

/**
  Sets the SPD Data pointer.

  This function abstracts the setting of the SpdData pointer.
  We may have implementation specific ways to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The pointer of SpdData pointer to store.

  @return     None

**/
VOID
EFIAPI
SetSpdDataPointer (
  IN VOID *SpdData
  )
{
  ASSERT (SpdData != NULL);
  mPrivate->StaticPointerTable.SptEntries[SptSpdData] = (EFI_PHYSICAL_ADDRESS) (UINTN) SpdData;
  ASSERT (!mPrivate->SmmLockDone || IsBufferInSmram (mPrivate->StaticPointerTable.SptEntries[SptSpdData]));
}

/**
  Gets DebugData pointer.

  This function abstracts the retrieval for the DebugData pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance.

  @param    None

  @return   Pointer to DebugData

**/
VOID *
EFIAPI
GetDebugDataPointer (
  VOID
  )
{
  //
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID *) (UINTN) mPrivate->StaticPointerTable.SptEntries[SptDebugData];
}

/**
  Sets the DebugData pointer.

  This function abstracts the setting of the DebugData pointer.
  We may have implementation specific ways to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The pointer of DebugData pointer to store.

  @return     None

**/
VOID
EFIAPI
SetDebugDataPointer (
  IN VOID *DebugData
  )
{
  ASSERT (DebugData != NULL);
  mPrivate->StaticPointerTable.SptEntries[SptDebugData] = (EFI_PHYSICAL_ADDRESS) (UINTN) DebugData;
  ASSERT (!mPrivate->SmmLockDone || IsBufferInSmram (mPrivate->StaticPointerTable.SptEntries[SptDebugData]));
}

/**
  Gets MemMapHost pointer.

  This function abstracts the retrieval for MemMapHost pointer.
  This is designed to be very high performance for very high
  frequency operations.

  @return pointer to MemMapHost

**/
VOID *
EFIAPI
GetMemMapHostPointer (
  VOID
  )
{
  //
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID *) (UINTN) mPrivate->StaticPointerTable.SptEntries[SptMemMapHost];
}

/**
  Sets MemMapHost pointer.

  This function abstracts the setting for MemMapHost pointer.
  We may have implementation specific way to get this pointer.

  @param[in]   The pointer of MemMapHost pointer to store.

**/
VOID
EFIAPI
SetMemMapHostPointer (
  IN VOID *MemMapHost
  )
{
  ASSERT (MemMapHost != NULL);
  mPrivate->StaticPointerTable.SptEntries[SptMemMapHost] = (EFI_PHYSICAL_ADDRESS) (UINTN) MemMapHost;
  ASSERT (!mPrivate->SmmLockDone || IsBufferInSmram (mPrivate->StaticPointerTable.SptEntries[SptMemMapHost]));
}

/**
  Gets Emulation Setting.

  This function abstracts the retrieval for the EmulationSetting pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance.

  @param    None

  @return   Pointer to EmulationSetting

**/
VOID *
EFIAPI
GetEmulationSettingPointer (
  VOID
  )
{
  //
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID *) (UINTN) mPrivate->StaticPointerTable.SptEntries[SptEmulationSetting];
}

/**
  Sets the Emulation Setting.

  This function abstracts the setting of the EmulationSetting pointer.
  We may have implementation specific ways to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The pointer of EmulationSetting pointer to store.

  @return     None

**/
VOID
EFIAPI
SetEmulationSettingPointer (
  IN VOID *EmulationSetting
  )
{
  ASSERT (EmulationSetting != NULL);
  mPrivate->StaticPointerTable.SptEntries[SptEmulationSetting] = (EFI_PHYSICAL_ADDRESS) (UINTN) EmulationSetting;
  ASSERT (!mPrivate->SmmLockDone || IsBufferInSmram (mPrivate->StaticPointerTable.SptEntries[SptEmulationSetting]));
}

/**
  Gets MrcPerformanceData pointer.

  This function abstracts the retrieval for the MrcPerformanceData pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance.

  @param    None

  @return   Pointer to MrcPerformanceData

**/
VOID*
EFIAPI
GetMrcPerformanceDataPointer (
  VOID
  )
{
  //
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID *) (UINTN) mPrivate->StaticPointerTable.SptEntries[SptMrcPerformanceData];
}

/**
  Sets the MrcPerformanceData pointer.

  This function abstracts the setting of the MrcPerformanceData pointer.
  We may have implementation specific ways to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The MrcPerformanceData pointer to store.

  @return     None

**/
VOID
EFIAPI
SetMrcPerformanceDataPointer (
  IN VOID *MrcPerformanceData
  )
{
  ASSERT (MrcPerformanceData != NULL);
  mPrivate->StaticPointerTable.SptEntries[SptMrcPerformanceData] = (EFI_PHYSICAL_ADDRESS) (UINTN) MrcPerformanceData;
  ASSERT (!mPrivate->SmmLockDone || IsBufferInSmram (mPrivate->StaticPointerTable.SptEntries[SptMrcPerformanceData]));
}

/**
  Gets Ddr5LrdimmTrainingData pointer.

  This function abstracts the retrieval for the Ddr5LrdimmTrainingData pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance.

  @param    None

  @return   Pointer to Ddr5LrdimmTrainingData

**/
VOID*
EFIAPI
GetDdr5LrdimmTrainingDataPointer (
  VOID
  )
{
  //
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID *) (UINTN) mPrivate->StaticPointerTable.SptEntries[SptDdr5LrdimmTrainingData];
}

/**
  Sets the Ddr5LrdimmTrainingData pointer.

  This function abstracts the setting of the Ddr5LrdimmTrainingData pointer.
  We may have implementation specific ways to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The Ddr5LrdimmTrainingData pointer to store.

  @return     None

**/
VOID
EFIAPI
SetDdr5LrdimmTrainingDataPointer (
  IN VOID *Ddr5LrdimmTrainingData
  )
{
  ASSERT (Ddr5LrdimmTrainingData != NULL);
  mPrivate->StaticPointerTable.SptEntries[SptDdr5LrdimmTrainingData] = (EFI_PHYSICAL_ADDRESS) (UINTN) Ddr5LrdimmTrainingData;
  ASSERT (!mPrivate->SmmLockDone || IsBufferInSmram (mPrivate->StaticPointerTable.SptEntries[SptDdr5LrdimmTrainingData]));
}

/**
  Initializes SMM Static Pointer Table.

  This function is used by the library constructor to initialize
  the SMM Static Pointers from PEI/DXE data structures.

  @return     None

**/
VOID
InitializeStaticPointers (
  VOID
  )
{
  VOID                            *DataPool;
  VOID                            *SmmDataPool;
  VOID                            *HobPtr;
  UINT8                           Index;
  STATIC_POINTER_SMM_SIZE_STRUCT  SmmSize;
  STATIC_POINTER_TABLE            *StaticPointerTable = NULL;

  //
  // Grab the HOB of static pointers from PEI
  //
  HobPtr = GetFirstGuidHob (&mStaticPointerTableGuid);
  ASSERT (HobPtr != NULL);
  if (HobPtr != NULL) {
    //
    // HOB found - use it to copy existing data struct to SMM
    //
    StaticPointerTable = GET_GUID_HOB_DATA (HobPtr);

    for (Index = 0; Index < SptMax; Index++) {
      if (StaticPointerTable->SptEntries[Index] != (EFI_PHYSICAL_ADDRESS) (UINTN) NULL) {
        DataPool = (VOID *)(UINTN) StaticPointerTable->SptEntries[Index];
        SmmSize = *(STATIC_POINTER_SMM_SIZE_STRUCT *)DataPool;
        //
        // If Static Pointer SMM Size structure is present and size is not zero,
        // create SMM pool of memory to copy the data to from Ring 0.
        //
        if (CompareGuid (&SmmSize.Guid, &gStaticPointerSmmSizeGuid) && (SmmSize.Size != 0)) {
          SmmDataPool = AllocatePool (SmmSize.Size);
          if (SmmDataPool != NULL) {
            CopyMem (SmmDataPool, DataPool, SmmSize.Size);
            mPrivate->StaticPointerTable.SptEntries[Index] = (EFI_PHYSICAL_ADDRESS) (UINTN) SmmDataPool;
          } else {
            DEBUG ((DEBUG_ERROR, "ERROR: %a: Could not allocate SMM memory for pointer type %x\n",
              __FUNCTION__, Index
              ));
          }
        } else {
          //
          // If Static Pointer SMM Size structure is not present, use Ring 0 pointer directly.
          //
          mPrivate->StaticPointerTable.SptEntries[Index] = (EFI_PHYSICAL_ADDRESS) (UINTN) DataPool;
        }
      }
    }
  }

}

/**
  Get SMRAM ranges.

  The Ranges pointer is allocated by this function. The caller is responsible
  for freeing that memory.

  @param[out] Ranges    Pointer to SMRAM descriptors.
  @param[out] Count     Number of SMRAM descriptors.

  @retval EFI_SUCCESS   Ranges and Count are successfully updated.
  @retval other         An error occurred; Ranges and Count are not modified.
**/
EFI_STATUS
GetSmramRanges (
  OUT EFI_SMRAM_DESCRIPTOR  **Ranges,
  OUT UINTN                 *Count
  )
{
  EFI_STATUS                Status;
  EFI_SMM_ACCESS2_PROTOCOL  *SmmAccess2;
  UINTN                     Size;
  EFI_SMRAM_DESCRIPTOR      *InternalRanges;

  Status = gBS->LocateProtocol (
    &gEfiSmmAccess2ProtocolGuid,
    NULL,
    (VOID **) &SmmAccess2
    );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "ERROR: %a: Unable to locate SmmAccess2 protocol: %r\n",
      gEfiCallerBaseName,
      Status
      ));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Get the size in bytes of the capabilities. We expect EFI_BUFFER_TOO_SMALL
  // to be returned, and Size will have been updated with the correct value.
  //
  Size = 0;
  Status = SmmAccess2->GetCapabilities (SmmAccess2, &Size, NULL);
  if (Status == EFI_BUFFER_TOO_SMALL) {
    ASSERT (Size != 0);
  } else {
    DEBUG ((
      DEBUG_ERROR,
      "ERROR: %a: Unexpected status for getting SMM capabilities size: %r\n",
      gEfiCallerBaseName,
      Status
      ));
    ASSERT_EFI_ERROR (Status);
    ASSERT (Status == EFI_BUFFER_TOO_SMALL);
    if (EFI_ERROR (Status)) {
      return Status;
    } else {
      return EFI_BAD_BUFFER_SIZE; // change unexpected "success" to failure
    }
  }

  //
  // Allocate space to get the ranges.
  //
  InternalRanges = AllocatePool (Size);
  if (InternalRanges == NULL) {
    DEBUG ((
      DEBUG_ERROR,
      "ERROR: %a: Could not allocate memory for SMM ranges\n",
      gEfiCallerBaseName
      ));
    ASSERT (InternalRanges != NULL);
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Get the ranges (capabilities).
  //
  Status = SmmAccess2->GetCapabilities (SmmAccess2, &Size, InternalRanges);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "ERROR: %a: Unable to get SMM capabilities: %r\n",
      gEfiCallerBaseName,
      Status
      ));
    ASSERT_EFI_ERROR (Status);
    FreePool (InternalRanges);
    return Status;
  }

  //
  // Return the ranges pointer and count through OUT parameters.
  //
  *Ranges = InternalRanges;
  *Count = Size / sizeof (EFI_SMRAM_DESCRIPTOR);
  return EFI_SUCCESS;
}

/**
  Build this library's private data.

  Allocate and initialize private data for this library. Install it as an SMM
  protocol so other instances of this library can find it.

  @retval EFI_SUCCESS     The private data was built successfully.
  @retval other           An error occurred.
**/
EFI_STATUS
BuildPrivateData (
  VOID
  )
{
  EFI_STATUS            Status;
  EFI_SMRAM_DESCRIPTOR  *SmramRanges;
  UINTN                 SmramCount;
  UINTN                 Index;
  EFI_HANDLE            Handle;

  DEBUG ((
    DEBUG_INFO,
    "%a: Building data for SysHostPointerLib\n",
    gEfiCallerBaseName
    ));

  Status = GetSmramRanges (&SmramRanges, &SmramCount);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  mPrivate = (PRIVATE_STATIC_POINTER_PROTOCOL *) AllocatePool (
    PRIVATE_DATA_SIZE_BY_SMRAM_COUNT (SmramCount)
    );
  ASSERT (mPrivate != NULL);
  if (mPrivate == NULL) {
    DEBUG ((
      DEBUG_ERROR,
      "ERROR: %a: Could not allocate memory for SysHostPointerLib data!\n",
      gEfiCallerBaseName
      ));
    FreePool (SmramRanges);
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Successfully allocated, now initialize.
  //
  mPrivate->SmmLockDone = FALSE;
  for (Index = 0; Index < SptMax; Index++) {
    mPrivate->StaticPointerTable.SptEntries[Index] = (EFI_PHYSICAL_ADDRESS) (UINTN) NULL;
  }
  InitializeStaticPointers ();
  mPrivate->SmramCount = (UINT32) SmramCount;
  CopyMem (&mPrivate->SmramRanges, SmramRanges, SmramCount * sizeof (EFI_SMRAM_DESCRIPTOR));
  FreePool (SmramRanges);

  //
  // Install the protocol for other consumers of this library.
  //
  Handle = NULL;  // auto-allocate a new handle
  Status = gSmst->SmmInstallProtocolInterface (
    &Handle,
    &mPrivateStaticPointerProtocolGuid,
    EFI_NATIVE_INTERFACE,
    mPrivate
    );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "ERROR: %a: Could not install static pointer table protocol: %r!\n",
      gEfiCallerBaseName,
      Status
      ));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Callback registered for SmmReadyToLock.

  This function sanitizes any non-SMRAM pointers in the table by setting them
  to NULL. Such pointers are not expected to be used at runtime.

  @param[in]  Protocol    Points to the protocol's unique identifier.
  @param[in]  Interface   Points to the interface instance.
  @param[in]  Handle      The handle on which the interface was installed.

  @retval EFI_SUCCESS     The callback completed successfully.
**/
EFI_STATUS
EFIAPI
SmmSysHostPointerLibProtocolReadyToLockCallBack (
  IN  CONST EFI_GUID  *Protocol,
  IN  VOID            *Interface,
  IN  EFI_HANDLE      Handle
  )
{
  UINTN   Index;

  ASSERT (mPrivate != NULL);
  if (mPrivate == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (mPrivate->SmmLockDone) {
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "%a: Sanitizing non-SMRAM pointers in SysHostPointerLib\n", gEfiCallerBaseName));

  for (Index = 0; Index < SptMax; Index++) {
    if (mPrivate->StaticPointerTable.SptEntries[Index] == (EFI_PHYSICAL_ADDRESS) (UINTN) NULL) {
      continue; // skip NULL pointers
    }
    if (IsBufferInSmram (mPrivate->StaticPointerTable.SptEntries[Index])) {
      continue; // skip SMRAM pointers
    }

    DEBUG ((
      DEBUG_INFO,
      "  Setting entry %a (%d) to NULL; was 0x%X\n",
      mDebugSptStrings[Index],
      Index,
      mPrivate->StaticPointerTable.SptEntries[Index]
      ));
    mPrivate->StaticPointerTable.SptEntries[Index] = (EFI_PHYSICAL_ADDRESS) (UINTN) NULL;
  }

  DEBUG ((DEBUG_INFO, "%a: Done sanitizing non-SMRAM pointers\n", gEfiCallerBaseName));

  mPrivate->SmmLockDone = TRUE;
  return EFI_SUCCESS;
}

/**
  The constructor for this library.

  @param[in]  ImageHandle   Handle to the image of this driver.
  @param[in]  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The constructor executed successfully.
  @retval other             An error occurred.
**/
EFI_STATUS
EFIAPI
SmmSysHostPointerLibProtocolConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  DEBUG ((
    DEBUG_VERBOSE,
    "%a: In SmmSysHostPointerLibProtocolConstructor\n",
    gEfiCallerBaseName
    ));

  DEBUG_CODE (
    mDebugSptStrings[SptSysHost]                = "SptSysHost";
    mDebugSptStrings[SptHbmHost]                = "SptHbmHost";
    mDebugSptStrings[SptCpuAndRevision]         = "SptCpuAndRevision";
    mDebugSptStrings[SptSysInfoVar]             = "SptSysInfoVar";
    mDebugSptStrings[SptSysSetup]               = "SptSysSetup";
    mDebugSptStrings[SptSemaphoreData]          = "SptSemaphoreData";
    mDebugSptStrings[SptSpdData]                = "SptSpdData";
    mDebugSptStrings[SptDebugData]              = "SptDebugData";
    mDebugSptStrings[SptMemMapHost]             = "SptMemMapHost";
    mDebugSptStrings[SptEmulationSetting]       = "SptEmulationSetting";
    mDebugSptStrings[SptMrcPerformanceData]     = "SptMrcPerformanceData";
    mDebugSptStrings[SptDdr5LrdimmTrainingData] = "SptDdr5LrdimmTrainingData";
    mDebugSptStrings[SptCpgcHost]               = "SptCpgcHost";
    );

  Status = gSmst->SmmLocateProtocol (
    &mPrivateStaticPointerProtocolGuid,
    NULL,
    (VOID **) &mPrivate
    );
  if (EFI_ERROR (Status)) {
    //
    // Failed to locate, so build it.
    //
    Status = BuildPrivateData ();
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  Status = gSmst->SmmRegisterProtocolNotify (
    &gEfiSmmReadyToLockProtocolGuid,
    SmmSysHostPointerLibProtocolReadyToLockCallBack,
    &mRegistration
    );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "ERROR: %a: Unable to register notify for SmmReadyToLock: %r\n",
      gEfiCallerBaseName,
      Status
      ));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  ASSERT (mPrivate != NULL);  // sanity-check the pointer
  return EFI_SUCCESS;
}

/**
  The destructor for this library.

  @param[in]  ImageHandle   Handle to the image of this driver.
  @param[in]  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The destructor executed successfully.
  @retval other             An error occurred.
**/
EFI_STATUS
EFIAPI
SmmSysHostPointerLibProtocolDestructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  DEBUG ((
    DEBUG_VERBOSE,
    "%a: In SmmSysHostPointerLibProtocolDestructor\n",
    gEfiCallerBaseName
    ));

  if (mRegistration == NULL) {
    return EFI_SUCCESS; // nothing to unregister
  }

  //
  // Unregister by calling with a null function pointer.
  //
  Status = gSmst->SmmRegisterProtocolNotify (
    &gEfiSmmReadyToLockProtocolGuid,
    NULL,
    &mRegistration
    );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "ERROR: %a: Unable to unregister notify for SmmReadyToLock: %r\n",
      gEfiCallerBaseName,
      Status
      ));
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}
