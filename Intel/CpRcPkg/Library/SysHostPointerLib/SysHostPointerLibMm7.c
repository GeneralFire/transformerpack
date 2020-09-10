/** @file
  PeiSysHostPointerLibMm7.c library implementation for SysHostPointerLib

  Uses MM7 to store a pointer to a static pointer table that provides
  fast pointer access.

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

#include <Library/BaseLib.h>
#include <PiPei.h>
#include <Ppi/MemoryDiscovered.h>
#include <Ppi/MpServices.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/PeiServicesLib.h>
#include "StaticPointerTableHelper.h"

//
// Global variables
//

//
// This GUID is not intended for use by other code
//
STATIC GUID  mStaticPointerTableGuid          = STATIC_POINTER_TABLE_GUID;

EFI_GUID mPpiGuidList[SptMax] = {
  SPT_SYS_HOST_PPI_GUID,
  SPT_HBM_HOST_PPI_GUID,
  SPT_SYS_INFO_VAR_PPI_GUID,
  SPT_SYS_SETUP_PPI_GUID,
  SPT_SEMAPHORE_DATA_PPI_GUID,
  SPT_SPD_DATA_PPI_GUID,
  SPT_DEBUG_DATA_PPI_GUID,
  SPT_MEM_MAP_HOST_PPI_GUID,
  SPT_CPU_AND_REVISION_PPI_GUID,
  SPT_CPGC_HOST_PPI_GUID
};

static EFI_PEI_NOTIFY_DESCRIPTOR mMpServicesNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMpServicesPpiGuid,
  SyncMm7AllThreads
};

static EFI_PEI_NOTIFY_DESCRIPTOR mMemDiscoveredNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  FixupStaticPointers
};

/**
  Worker function to set static pointer.

  This function updates static pointer table with the specified type.
  It also installs private PPI to ensure that the pointer can be fixed
  after migrating to permanent memory.

  @param StaticPointerTable   The table to set static pointer
  @param Type                 The type of static pointer
  @param Pointer              The new pointer to set to static pointer table

  @return None

**/
VOID
SetStaticPointer (
  IN OUT STATIC_POINTER_TABLE          *StaticPointerTable,
  IN     STATIC_POINTER_TYPE           Type,
  IN     VOID                          *Pointer
  )
{
  EFI_PEI_PPI_DESCRIPTOR               *PpiDescriptor;

  PpiDescriptor = &StaticPointerTable->Ppis[Type];
  PpiDescriptor->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  PpiDescriptor->Guid = &mPpiGuidList[Type];
  PpiDescriptor->Ppi = Pointer;

  if (StaticPointerTable->SptEntries[Type] == 0) {
    //
    // Register the PpiDescriptor into PEI core database so that PEI core can help to convert
    // the static pointer.
    //
    PeiServicesInstallPpi (PpiDescriptor);
  }

  StaticPointerTable->SptEntries[Type] = (UINTN) Pointer;
}


/**
  Gets SysHost pointer.

  This function abstracts the retrieval for SysHost pointer.
  We may have implementation specific way to get this pointer.

  @return Pointer to SysHost

**/
VOID*
EFIAPI
GetSysHostPointer (
  VOID
  )
{
  //
  // Get the pointer from MM7
  // Cast it to STATIC_POINTER_TABLE
  // Return the value of the SysHost entry
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID*) (UINTN) (((STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ())->SptEntries[SptSysHost]);
}

/**
  This function verifies that we are compatible with other components.

  It verifies the GUID and size of the HOB that stores the table.
  It fails bluntly with a deadloop.

  @param StaticPointerTable   The table to verify

  @return None

**/
VOID
VerifyStaticPointerTable (
  IN STATIC_POINTER_TABLE   *StaticPointerTable
  )
{
  EFI_HOB_GUID_TYPE       *StaticPointerTableHob;
  BOOLEAN                 DoesGuidMatch;
  BOOLEAN                 DoesSizeMatch;

  //
  // Verify input
  //
  ASSERT (StaticPointerTable != NULL);
  if (StaticPointerTable == NULL) {
    CpuDeadLoop ();
  }

  //
  // Verify that it is a compatible version
  // This is done by verifying GUID and verifying the size of the HOB
  //
  StaticPointerTableHob = (EFI_HOB_GUID_TYPE*) (((UINT8*) StaticPointerTable) - sizeof (EFI_HOB_GUID_TYPE));

  ASSERT(StaticPointerTableHob != NULL);
  if (StaticPointerTableHob == NULL) {
    CpuDeadLoop();
  } else {
    DoesGuidMatch = CompareGuid (&StaticPointerTableHob->Name, &mStaticPointerTableGuid);
    DoesSizeMatch = StaticPointerTableHob->Header.HobLength == (sizeof (EFI_HOB_GUID_TYPE) + sizeof (STATIC_POINTER_TABLE));

    ASSERT (DoesGuidMatch && DoesSizeMatch);
    if (!DoesGuidMatch && !DoesSizeMatch) {
      CpuDeadLoop ();
    }
  }
}

/**
  Sets SysHost pointer.

  This function abstracts the setting for SysHost pointer.
  This is not a performance sensitive function.
  This function also calls the constructor to force MM7 update
  in order to ensure SBSP MM7 registers are configured properly.

  @param[in]   The pointer of SysHost pointer to store.

**/
VOID
EFIAPI
SetSysHostPointer (
  IN VOID *SysHost
  )
{
  STATIC_POINTER_TABLE    *StaticPointerTable;

  //
  // Verify input and retrieve the static pointer table
  // This will force reconfiguration of MM7 to ensure SBSP are configured
  // properly.  This is done by calling the constructor.
  //
  ASSERT (SysHost != NULL);
  StaticPointerTableLibConstructor ();
  StaticPointerTable = (STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ();

  //
  // Verify compatibility with existing table
  // Since this is a static library, the table may be produced from other
  // components built with other versions of the HOB.
  //
  VerifyStaticPointerTable (StaticPointerTable);

  //
  // Add pointer to table
  //
  SetStaticPointer (StaticPointerTable, SptSysHost, SysHost);
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
  // Get the pointer from MM7
  // Cast it to STATIC_POINTER_TABLE
  // Return the value of the HbmHost entry
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID*) (UINTN) (((STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ())->SptEntries[SptHbmHost]);
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
  STATIC_POINTER_TABLE    *StaticPointerTable;

  //
  // Verify input and retrieve the static pointer table
  // This will force reconfiguration of MM7 to ensure SBSP are configured
  // properly.  This is done by calling the constructor.
  //
  ASSERT (HbmHost != NULL);
  StaticPointerTableLibConstructor ();
  StaticPointerTable = (STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ();

  //
  // Verify compatibility with existing table
  // Since this is a static library, the table may be produced from other
  // components built with other versions of the HOB.
  //
  VerifyStaticPointerTable (StaticPointerTable);

  //
  // Add pointer to table
  //
  SetStaticPointer (StaticPointerTable, SptHbmHost, HbmHost);
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
  //
  // Get the pointer from MM7
  // Cast it to STATIC_POINTER_TABLE
  // Return the value of the CpgcHost entry
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID*) (UINTN) (((STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ())->SptEntries[SptCpgcHost]);
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
  STATIC_POINTER_TABLE    *StaticPointerTable;

  //
  // Verify input and retrieve the static pointer table
  // This will force reconfiguration of MM7 to ensure SBSP are configured
  // properly.  This is done by calling the constructor.
  //
  ASSERT (CpgcHost != NULL);
  StaticPointerTableLibConstructor ();
  StaticPointerTable = (STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ();

  //
  // Verify compatibility with existing table
  // Since this is a static library, the table may be produced from other
  // components built with other versions of the HOB.
  //
  VerifyStaticPointerTable (StaticPointerTable);

  //
  // Add pointer to table
  //
  SetStaticPointer (StaticPointerTable, SptCpgcHost, CpgcHost);
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
  // Get the pointer from MM7
  // Cast it to STATIC_POINTER_TABLE
  // Return the value of the HbmHost entry
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID*) (UINTN) (((STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ())->SptEntries[SptCpuAndRevision]);
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
  STATIC_POINTER_TABLE    *StaticPointerTable;

  //
  // Verify input and retrieve the static pointer table
  // This will force reconfiguration of MM7 to ensure SBSP are configured
  // properly.  This is done by calling the constructor.
  //
  ASSERT (CpuAndRevision != NULL);
  StaticPointerTableLibConstructor ();
  StaticPointerTable = (STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ();

  //
  // Verify compatibility with existing table
  // Since this is a static library, the table may be produced from other
  // components built with other versions of the HOB.
  //
  VerifyStaticPointerTable (StaticPointerTable);

  //
  // Add pointer to table
  //
  SetStaticPointer (StaticPointerTable, SptCpuAndRevision, CpuAndRevision);
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
  //
  // Get the pointer from MM7
  // Cast it to STATIC_POINTER_TABLE
  // Return the value of the SysInfoVar entry
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID*) (UINTN) (((STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ())->SptEntries[SptSysInfoVar]);
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
  STATIC_POINTER_TABLE    *StaticPointerTable;

  //
  // Verify input and retrieve the static pointer table
  // This will force reconfiguration of MM7 to ensure SBSP are configured
  // properly.  This is done by calling the constructor.
  //
  ASSERT (SysInfoVar != NULL);
  StaticPointerTableLibConstructor ();
  StaticPointerTable = (STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ();

  //
  // Verify compatibility with existing table
  // Since this is a static library, the table may be produced from other
  // components built with other versions of the HOB.
  //
  VerifyStaticPointerTable (StaticPointerTable);

  //
  // Add pointer to table
  //
  SetStaticPointer (StaticPointerTable, SptSysInfoVar, SysInfoVar);
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
  // Get the pointer from MM7
  // Cast it to STATIC_POINTER_TABLE
  // Return the value of the SysSetup entry
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID*) (UINTN) (((STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ())->SptEntries[SptSysSetup]);
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
  STATIC_POINTER_TABLE    *StaticPointerTable;

  //
  // Verify input and retrieve the static pointer table
  // This will force reconfiguration of MM7 to ensure SBSP are configured
  // properly.  This is done by calling the constructor.
  //
  ASSERT (SysSetup != NULL);
  StaticPointerTableLibConstructor ();
  StaticPointerTable = (STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ();

  //
  // Verify compatibility with existing table
  // Since this is a static library, the table may be produced from other
  // components built with other versions of the HOB.
  //
  VerifyStaticPointerTable (StaticPointerTable);

  //
  // Add pointer to table
  //
  SetStaticPointer (StaticPointerTable, SptSysSetup, SysSetup);
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
  // Get the pointer from MM7
  // Cast it to STATIC_POINTER_TABLE
  // Return the value of the SemaphoreData entry
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID*) (UINTN) (((STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ())->SptEntries[SptSemaphoreData]);
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
  STATIC_POINTER_TABLE    *StaticPointerTable;

  //
  // Verify input and retrieve the static pointer table
  // This will force reconfiguration of MM7 to ensure SBSP are configured
  // properly.  This is done by calling the constructor.
  //
  ASSERT (SemaphoreData != NULL);
  StaticPointerTableLibConstructor ();
  StaticPointerTable = (STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ();

  //
  // Verify compatibility with existing table
  // Since this is a static library, the table may be produced from other
  // components built with other versions of the HOB.
  //
  VerifyStaticPointerTable (StaticPointerTable);

  //
  // Add pointer to table
  //
  SetStaticPointer (StaticPointerTable, SptSemaphoreData, SemaphoreData);
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
  // Get the pointer from MM7
  // Cast it to STATIC_POINTER_TABLE
  // Return the value of the SpdData entry
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID*) (UINTN) (((STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ())->SptEntries[SptSpdData]);
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
  STATIC_POINTER_TABLE    *StaticPointerTable;

  //
  // Verify input and retrieve the static pointer table
  // This will force reconfiguration of MM7 to ensure SBSP are configured
  // properly.  This is done by calling the constructor.
  //
  ASSERT (SpdData != NULL);
  StaticPointerTableLibConstructor ();
  StaticPointerTable = (STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ();

  //
  // Verify compatibility with existing table
  // Since this is a static library, the table may be produced from other
  // components built with other versions of the HOB.
  //
  VerifyStaticPointerTable (StaticPointerTable);

  //
  // Add pointer to table
  //
  SetStaticPointer (StaticPointerTable, SptSpdData, SpdData);
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
  // Get the pointer from MM7
  // Cast it to STATIC_POINTER_TABLE
  // Return the value of the DebugData entry
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID*) (UINTN) (((STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ())->SptEntries[SptDebugData]);
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
  STATIC_POINTER_TABLE    *StaticPointerTable;

  //
  // Verify input and retrieve the static pointer table
  // This will force reconfiguration of MM7 to ensure SBSP are configured
  // properly.  This is done by calling the constructor.
  //
  ASSERT (DebugData != NULL);
  StaticPointerTableLibConstructor ();
  StaticPointerTable = (STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ();

  //
  // Verify compatibility with existing table
  // Since this is a static library, the table may be produced from other
  // components built with other versions of the HOB.
  //
  VerifyStaticPointerTable (StaticPointerTable);

  //
  // Add pointer to table
  //
  SetStaticPointer (StaticPointerTable, SptDebugData, DebugData);
}

/**
  Gets MemMapHost pointer.

  This function abstracts the retrieval for the MemMapHost pointer.
  There are implementation specific ways to get this pointer.
  This is designed to be very high performance.

  @param    None

  @return   Pointer to MemMapHost

**/
VOID*
EFIAPI
GetMemMapHostPointer (
  VOID
  )
{
  //
  // Get the pointer from MM7
  // Cast it to STATIC_POINTER_TABLE
  // Return the value of the MemMapHost entry
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID*) (UINTN) (((STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ())->SptEntries[SptMemMapHost]);
}

/**
  Sets the MemMapHost pointer.

  This function abstracts the setting of the MemMapHost pointer.
  We may have implementation specific ways to get this pointer.
  This is not expected to be a high performance function, so
  should not be called with very high frequency.

  @param[in]  The pointer of MemMapHost pointer to store.

  @return     None

**/
VOID
EFIAPI
SetMemMapHostPointer (
  IN VOID *MemMapHost
  )
{
  STATIC_POINTER_TABLE    *StaticPointerTable;

  //
  // Verify input and retrieve the static pointer table
  // This will force reconfiguration of MM7 to ensure SBSP are configured
  // properly.  This is done by calling the constructor.
  //
  ASSERT (MemMapHost != NULL);
  StaticPointerTableLibConstructor ();
  StaticPointerTable = (STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ();

  //
  // Verify compatibility with existing table
  // Since this is a static library, the table may be produced from other
  // components built with other versions of the HOB.
  //
  VerifyStaticPointerTable (StaticPointerTable);

  //
  // Add pointer to table
  //
  SetStaticPointer (StaticPointerTable, SptMemMapHost, MemMapHost);
}

/**
  AP function to perform the write of MM7

  @param[in out]  *Buffer    Pointer to MM7 value to program in this thread.

  @retval     None
**/
VOID
EFIAPI
SyncMm7Register (
  IN OUT VOID  *Buffer
  )
{
  AsmWriteMm7 (*(UINT64 *) Buffer);
}

/**
  Syncs MM7 register from BSP across all APs for use in multithreaded functions.

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
SyncMm7AllThreads (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDesc,
  IN VOID                       *Ppi
)
{
  EFI_STATUS               Status;
  EFI_PEI_MP_SERVICES_PPI  *PeiMpServices;
  UINT64                   Mm7Value;

  PeiMpServices = Ppi;

  Mm7Value = AsmReadMm7 ();

  DEBUG ((DEBUG_INFO, "Sync MM7: %x\n", Mm7Value));

  Status = PeiMpServices->StartupAllAPs (
                            PeiServices,
                            PeiMpServices,
                            SyncMm7Register,
                            FALSE,
                            0,
                            (VOID*)&Mm7Value
                            );

  return Status;
}

/**
  Fix up static pointers once PEI core has migrated NEM data to permanent memory.

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
FixupStaticPointers (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  STATIC_POINTER_TABLE          *StaticPointerTable;
  EFI_HOB_GUID_TYPE             *GuidHob;
  UINTN                         Index;
  EFI_PEI_MP_SERVICES_PPI       *PeiMpServices;
  EFI_STATUS                    Status;

  GuidHob = GetFirstGuidHob (&mStaticPointerTableGuid);
  //
  // HOB must exist as the notify PPI is installed after the HOB is successfully build.
  //
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  StaticPointerTable = GET_GUID_HOB_DATA (GuidHob);
  VerifyStaticPointerTable (StaticPointerTable);
  //
  // Migration static table pointer itself
  //
  DEBUG ((DEBUG_INFO, "Reload MM7 with new pointer: %p => %p\n", (UINTN) AsmReadMm7 (), (UINTN) StaticPointerTable));
  AsmWriteMm7 ((UINTN) StaticPointerTable);

  for (Index = 0; Index < SptMax; Index++) {
    if (StaticPointerTable->SptEntries[Index] != 0) {
      DEBUG ((DEBUG_INFO, "Migration pointer type %x: %p => %p\n",
        Index, (UINTN) StaticPointerTable->SptEntries[Index], StaticPointerTable->Ppis[Index].Ppi
        ));

      StaticPointerTable->SptEntries[Index] = (UINTN) StaticPointerTable->Ppis[Index].Ppi;
    }
  }

  Status = (*PeiServices)->LocatePpi (
                      PeiServices,
                      &gEfiPeiMpServicesPpiGuid,
                      0,
                      NULL,
                      &PeiMpServices
                      );

  if (EFI_ERROR (Status)) {
    //
    // Register MpServices PPI notification to sync MM7 if PPI not present
    //
    Status = PeiServicesNotifyPpi (&mMpServicesNotifyList);
    ASSERT_EFI_ERROR (Status);
  } else {
    //
    // PPI is already present.  Sync MM7 register on all threads.
    //
    SyncMm7AllThreads (PeiServices, NULL, (VOID *)PeiMpServices);
  }

  return EFI_SUCCESS;
}


/**
  This library constructor creates a HOB if the table does not already exist and
  updates MM7 to point at the new table in the HOB.

  @param      None

  @return     RETURN_SUCCESS              Library is ready for use
  @return     RETURN_OUT_OF_RESOURCES     Insufficient resources, library is not usable

**/
RETURN_STATUS
EFIAPI
StaticPointerTableLibConstructor (
  VOID
  )
{
  VOID                    *HobPtr;
  STATIC_POINTER_TABLE    *StaticPointerTable;
  EFI_STATUS              Status;

  StaticPointerTable = NULL;

  //
  // Do not create another table if one has already been created
  //
  HobPtr = GetFirstGuidHob (&mStaticPointerTableGuid);

  if (HobPtr == NULL) {
    //
    // Allocate HOB
    // This will set GUID and size properly
    //
    StaticPointerTable = (STATIC_POINTER_TABLE*) BuildGuidHob (&mStaticPointerTableGuid, sizeof (STATIC_POINTER_TABLE));
    ASSERT (StaticPointerTable != NULL);
    if (StaticPointerTable == NULL) {
      return RETURN_OUT_OF_RESOURCES;
    }

    //
    // Initialize the table
    //
    ZeroMem (StaticPointerTable, sizeof (STATIC_POINTER_TABLE));
    //
    // Register memory discovery PPI notification to fix up pointers
    //
    Status = PeiServicesNotifyPpi (&mMemDiscoveredNotifyList);
    ASSERT_EFI_ERROR (Status);
  } else {
    //
    // Existing HOB found
    //
    StaticPointerTable = (STATIC_POINTER_TABLE*) (((UINT8*) HobPtr) + sizeof (EFI_HOB_GUID_TYPE));
    //
    // Verify compatibility with existing table
    // Since this is a static library, the table may be produced from other
    // components built with other versions of the HOB.
    //
    VerifyStaticPointerTable (StaticPointerTable);
  }

  //
  // Update MM7 register
  // This is done on any call to handle the SBSP cases where PipeInit
  // will set the pointers, but MM7 will not have been set.
  // Since the HOB is published and heap has been copied, this will update
  // MM7 to point at the SBSP instance of the StaticPointerTable
  //
  AsmWriteMm7 ((UINT64) (UINTN) StaticPointerTable);

  return RETURN_SUCCESS;
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
  // Get the pointer from MM7
  // Cast it to STATIC_POINTER_TABLE
  // Return the value of the EmulationSetting entry
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID*) (UINTN) (((STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ())->SptEntries[SptEmulationSetting]);
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
  STATIC_POINTER_TABLE    *StaticPointerTable;

  //
  // Verify input and retrieve the static pointer table
  // This will force reconfiguration of MM7 to ensure SBSP are configured
  // properly.  This is done by calling the constructor.
  //
  ASSERT (EmulationSetting != NULL);
  StaticPointerTableLibConstructor ();
  StaticPointerTable = (STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ();

  //
  // Verify compatibility with existing table
  // Since this is a static library, the table may be produced from other
  // components built with other versions of the HOB.
  //
  VerifyStaticPointerTable (StaticPointerTable);

  //
  // Add pointer to table
  //
  SetStaticPointer (StaticPointerTable, SptEmulationSetting, EmulationSetting);
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
  // Get the pointer from MM7
  // Cast it to STATIC_POINTER_TABLE
  // Return the value of the EmulationSetting entry
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID*) (UINTN) (((STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ())->SptEntries[SptMrcPerformanceData]);
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
  STATIC_POINTER_TABLE    *StaticPointerTable;

  //
  // Verify input and retrieve the static pointer table
  // This will force reconfiguration of MM7 to ensure SBSP are configured
  // properly.  This is done by calling the constructor.
  //
  ASSERT (MrcPerformanceData != NULL);
  StaticPointerTableLibConstructor ();
  StaticPointerTable = (STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ();

  //
  // Verify compatibility with existing table
  // Since this is a static library, the table may be produced from other
  // components built with other versions of the HOB.
  //
  VerifyStaticPointerTable (StaticPointerTable);

  //
  // Add pointer to table
  //
  SetStaticPointer (StaticPointerTable, SptMrcPerformanceData, MrcPerformanceData);
}

/**
  Gets Ddr5LrdimmTrainingData pointer.

  This function abstracts the retrieval for the Ddr5LdrimmTrainingData pointer.
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
  // Get the pointer from MM7
  // Cast it to STATIC_POINTER_TABLE
  // Return the value of the EmulationSetting entry
  // This code must be performance optimized, hence the difficulty in readability
  // and lack of error checking
  //
  return (VOID*) (UINTN) (((STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ())->SptEntries[SptDdr5LrdimmTrainingData]);
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
  STATIC_POINTER_TABLE    *StaticPointerTable;

  //
  // Verify input and retrieve the static pointer table
  // This will force reconfiguration of MM7 to ensure SBSP are configured
  // properly.  This is done by calling the constructor.
  //
  ASSERT (Ddr5LrdimmTrainingData != NULL);
  StaticPointerTableLibConstructor ();
  StaticPointerTable = (STATIC_POINTER_TABLE*) (UINTN) AsmReadMm7 ();

  //
  // Verify compatibility with existing table
  // Since this is a static library, the table may be produced from other
  // components built with other versions of the HOB.
  //
  VerifyStaticPointerTable (StaticPointerTable);

  //
  // Add pointer to table
  //
  SetStaticPointer (StaticPointerTable, SptDdr5LrdimmTrainingData, Ddr5LrdimmTrainingData);
}