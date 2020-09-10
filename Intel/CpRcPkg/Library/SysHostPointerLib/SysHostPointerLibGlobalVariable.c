/** @file
  SysHostPointerLibGlobalVariable.c Library Implementation

  C global variable based implementation for Sys host pointer.  This will not work
  for execute in place code.

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

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/HobLib.h>
#include "StaticPointerTableHelper.h"

//
// Global static pointer table.
// There is not actually much benefit from using the table, it could
// more simply be a set of pointers, but this helps verify that design.
//
STATIC STATIC_POINTER_TABLE  *mStaticPointerTable = NULL;

//
// This GUID is not intended for use by other code
//
STATIC GUID  mStaticPointerTableGuid          = STATIC_POINTER_TABLE_GUID;

/**
  Gets SysHost pointer.

  This function abstracts the retrieval for SysHost pointer.
  This is designed to be very high performance for very high
  frequency operations.

  @return pointer to SysHost

**/
VOID*
EFIAPI
GetSysHostPointer (
  VOID
  )
{
  //
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID*) (UINTN) mStaticPointerTable->SptEntries[SptSysHost];
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
  ASSERT (FALSE);
  return;
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
VOID*
EFIAPI
GetHbmHostPointer (
  VOID
  )
{
  //
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID*) (UINTN) mStaticPointerTable->SptEntries[SptHbmHost];
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
  ASSERT (FALSE);
  return;
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
  ASSERT (FALSE);
  return;
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
VOID*
EFIAPI
GetCpuAndRevisionPointer (
  VOID
  )
{
  //
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID*) (UINTN) mStaticPointerTable->SptEntries[SptCpuAndRevision];
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
VOID*
EFIAPI
GetSysInfoVarPointer (
  VOID
  )
{
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
  return;
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
VOID*
EFIAPI
GetSysSetupPointer (
  VOID
  )
{
  //
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID*) (UINTN) mStaticPointerTable->SptEntries[SptSysSetup];
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
  ASSERT (FALSE);
  return;
}

/**
  Gets Semaphore Data pointer.

  This function abstracts the retrieval for the SemaphoreData pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance.

  @param    None

  @return   Pointer to SemaphoreData

**/
VOID*
EFIAPI
GetSemaphoreDataPointer (
  VOID
  )
{
  //
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID*) (UINTN) mStaticPointerTable->SptEntries[SptSemaphoreData];
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
  ASSERT (FALSE);
  return;
}

/**
  Gets SPD Data pointer.

  This function abstracts the retrieval for the SpdData pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance.

  @param    None

  @return   Pointer to SpdData

**/
VOID*
EFIAPI
GetSpdDataPointer (
  VOID
  )
{
  //
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID*) (UINTN) mStaticPointerTable->SptEntries[SptSpdData];
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
  ASSERT (FALSE);
  return;
}

/**
  Gets DebugData pointer.

  This function abstracts the retrieval for the DebugData pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance.

  @param    None

  @return   Pointer to DebugData

**/
VOID*
EFIAPI
GetDebugDataPointer (
  VOID
  )
{
  //
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID*) (UINTN) mStaticPointerTable->SptEntries[SptDebugData];
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
  ASSERT (FALSE);
  return;
}

/**
  This function verifies that we are compatible with other components.

  It verifies the GUID and size of the HOB that stores the table.
  It fails bluntly with a deadloop.

  @param  None

  @return None

**/
VOID
VerifyStaticPointerTable (
  VOID
  )
{
  EFI_HOB_GUID_TYPE       *StaticPointerTableHob;
  BOOLEAN                 DoesGuidMatch;

  //
  // Verify input
  //
  ASSERT (mStaticPointerTable != NULL);
  if (mStaticPointerTable == NULL) {
    CpuDeadLoop ();
  }

  //
  // Verify that it is a compatible version
  // This is done by verifying GUID and verifying the size of the HOB
  //
  StaticPointerTableHob = (EFI_HOB_GUID_TYPE*) (((UINT8*) mStaticPointerTable) - sizeof (EFI_HOB_GUID_TYPE));

  ASSERT(StaticPointerTableHob != NULL);
  if (StaticPointerTableHob == NULL) {
    CpuDeadLoop();
  } else {
    //
    // No need to check the size as EFI_PEI_PPI_DESCRIPTOR is different from PEI to DXE
    // Only check GUID
    //
    DoesGuidMatch = CompareGuid (&StaticPointerTableHob->Name, &mStaticPointerTableGuid);

    ASSERT (DoesGuidMatch);
    if (!DoesGuidMatch) {
      CpuDeadLoop ();
    }
  }
}

/**
  This library constructor initializes the global variable instance
  of the static pointer table.

  @param      None

  @return     RETURN_SUCCESS              Library is ready for use

**/
RETURN_STATUS
EFIAPI
StaticPointerTableLibConstructor (
  VOID
  )
{
  VOID        *HobPtr;

  if (mStaticPointerTable == NULL) {
    //
    // Do not create another table if one has already been created
    //
    HobPtr = GetFirstGuidHob (&mStaticPointerTableGuid);
    ASSERT (HobPtr != NULL);
    if (HobPtr == NULL) {
      return EFI_NOT_FOUND;
    }

    //
    // Existing HOB found - in DXE phase, only SptEntries are used
    //
    mStaticPointerTable = GET_GUID_HOB_DATA (HobPtr);
    //
    // Verify compatibility with existing table
    // Since this is a static library, the table may be produced from other
    // components built with other versions of the HOB.
    //
    VerifyStaticPointerTable ();
  }

  return RETURN_SUCCESS;
}

/**
  Gets MemMapHost pointer.

  This function abstracts the retrieval for MemMapHost pointer.
  This is designed to be very high performance for very high
  frequency operations.

  @return pointer to MemMapHost

**/
VOID*
EFIAPI
GetMemMapHostPointer (
  VOID
  )
{
  //
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID*) (UINTN) mStaticPointerTable->SptEntries[SptMemMapHost];
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
  ASSERT (FALSE);
  return;
}

/**
  Gets Emulation Setting.

  This function abstracts the retrieval for the EmulationSetting pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance.

  @param    None

  @return   Pointer to EmulationSetting

**/
VOID*
EFIAPI
GetEmulationSettingPointer (
  VOID
  )
{
  //
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID*) (UINTN) mStaticPointerTable->SptEntries[SptEmulationSetting];
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
  ASSERT (FALSE);
  return;
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
  return (VOID*) (UINTN) mStaticPointerTable->SptEntries[SptMrcPerformanceData];
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
  ASSERT (FALSE);
  return;
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
  return (VOID*) (UINTN) mStaticPointerTable->SptEntries[SptDdr5LrdimmTrainingData];
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
  ASSERT (FALSE);
  return;
}