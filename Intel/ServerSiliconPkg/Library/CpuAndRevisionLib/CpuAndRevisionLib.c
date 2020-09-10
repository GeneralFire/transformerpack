/** @file
  Implementation of CPU and Revision library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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
#include <Uefi.h>
#include <RcRegs.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Register/Intel/Cpuid.h>
#include <Cpu/CpuIds.h>
#include <Library/KtiApi.h>
#include <Library/SystemInfoLib.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/IoLib.h>
#include <IioRegs.h>
#include <Library/PcdLib.h>
#include <Chip/Include/CpuPciAccess.h>

/// Cpu Chop type
/// Enum for comparing CPU chop type.  00:LCC; 01:MCC; 10:HCC,UCC; 11:XCC
typedef enum {
  CPU_CHOP_LCC = 0, ///< Low Core count
  CPU_CHOP_MCC,     ///< Medium Core Count
  CPU_CHOP_HCC,     ///< High Core Count
  CPU_CHOP_UCC = 2, ///< Ultra Core Count
  CPU_CHOP_XCC,     ///< Extreme Core Count
  CPU_CHOP_ANY = 0xFF
} CPU_CHOP_TYPE;

//
// Signature used with the CpuAndRevision structure to
// verify that the structure is valid.
//
#define CPU_AND_REVISION_SIGNATURE        0xFA9A3825

#pragma pack (1)

//
// CpuAndRevision data stored in memory
//
typedef struct _CPU_AND_REVISION_INFO {
  STATIC_POINTER_SMM_SIZE_STRUCT  SmmSize;
  UINT8                           CpuType;
  UINT16                          Revision;
  UINT8                           Index;
  UINT8                           CpuStepping;
  UINT8                           Flag;
  BOOLEAN                         Normalized;
  UINT32                          Signature;
} CPU_AND_REVISION_INFO;

//
// Revision information used internally to pass current revision data
//
typedef struct _REVISION_INFO {
  UINT16          Revision;
  UINT8           Index;
} REVISION_INFO;

//
// CpuAndRevision data used locally in the lookup table
//
typedef struct _CPU_REVISION_LOOKUP {
  UINT8           CpuStepping;
  CPU_CHOP_TYPE   CpuChop;
  UINT8           Flag;
  UINT16          Revision;
} CPU_REVISION_LOOKUP;

typedef struct _CPU_TYPE_LOOKUP {
  UINT8                 CpuType;
  CPU_REVISION_LOOKUP   *LookupPtr;
  UINT8                 EntryCount;
} CPU_TYPE_LOOKUP;

//
// CPU_REVISION_LOOKUP Table is used for both identification and
// revision sequence checking.  When adding new revisions, please keep
// this sequencing aspect in mind with regards to revisions of the same
// CPU type.
//
// Flag is for special cases where additional data is needed to identify
// a silent stepping revision.
//
CPU_REVISION_LOOKUP
SkxLookup[] = {
  //  Stepping ID,   PhysicalChop,    Flag,   Revision
  {           0x0,   CPU_CHOP_ANY,       0,     REV_A0},
  {           0x1,   CPU_CHOP_ANY,       0,     REV_A2},
  {           0x2,   CPU_CHOP_XCC,       0,     REV_B0},
  {           0x2,   CPU_CHOP_HCC,       0,     REV_L0},
  {           0x3,   CPU_CHOP_ANY,       0,     REV_B1},
  {           0x4,   CPU_CHOP_XCC,       0,     REV_H0},
  {           0x4,   CPU_CHOP_HCC,       0,     REV_M0},
  {           0x4,   CPU_CHOP_LCC,       0,     REV_U0},
};

CPU_REVISION_LOOKUP
ClxLookup[] = {
  //  Stepping ID,   PhysicalChop,    Flag,   Revision
  {           0x5,   CPU_CHOP_ANY,       0,     REV_A0},
  {           0x6,   CPU_CHOP_XCC,       0,     REV_B0},
  {           0x6,   CPU_CHOP_HCC,       0,     REV_N0},
  {           0x6,   CPU_CHOP_LCC,       0,     REV_V0},
  {           0x7,   CPU_CHOP_XCC,       0,     REV_B1},
  {           0x7,   CPU_CHOP_HCC,       0,     REV_N1},
  {           0x7,   CPU_CHOP_LCC,       0,     REV_V1},
};

CPU_REVISION_LOOKUP
CpxLookup[] = {
  //  Stepping ID,   PhysicalChop,    Flag,   Revision
  {           0xA,   CPU_CHOP_ANY,       0,     REV_A0},
  {           0xB,   CPU_CHOP_ANY,       0,     REV_A1},
};

CPU_REVISION_LOOKUP
SnrLookup[] = {
  //  Stepping ID,   PhysicalChop,    Flag,   Revision
  {           0x0,   CPU_CHOP_ANY,       0,     REV_A0},
  {           0x1,   CPU_CHOP_ANY,       0,     REV_A1},
  {           0x2,   CPU_CHOP_ANY,       0,     REV_A2},
  {           0x3,   CPU_CHOP_ANY,       0,     REV_A3},
  {           0x4,   CPU_CHOP_ANY,       0,     REV_B0},
  {           0x5,   CPU_CHOP_ANY,       0,     REV_B1},
  {           0x7,   CPU_CHOP_ANY,       0,     REV_C0},
};

CPU_REVISION_LOOKUP
TnrLookup[] = {
  //  Stepping ID,   PhysicalChop,    Flag,   Revision
  {           0xA,   CPU_CHOP_ANY,       0,     REV_A0},
};

CPU_REVISION_LOOKUP
IcxSpLookup[] = {
  //  Stepping ID,   PhysicalChop,    Flag,   Revision
  {           0x0,   CPU_CHOP_ANY,       0,     REV_R0},
  {           0x4,   CPU_CHOP_ANY,       0,     REV_L0},
  {           0x5,   CPU_CHOP_ANY,       0,     REV_C0},
  {           0x6,   CPU_CHOP_XCC,       0,     REV_D0},
  {           0x6,   CPU_CHOP_HCC,       0,     REV_M0},
};

CPU_REVISION_LOOKUP
IcxDLookup[] = {
  //  Stepping ID,   PhysicalChop,    Flag,   Revision
  {           0x0,   CPU_CHOP_ANY,       0,     REV_A0},
  {           0x1,   CPU_CHOP_ANY,       0,     REV_B0},
};

CPU_REVISION_LOOKUP
SprSpLookup[] = {
  //  Stepping ID,   PhysicalChop,    Flag,   Revision
  {           0x0,   CPU_CHOP_ANY,       0,     REV_A0},
  {           0x1,   CPU_CHOP_ANY,       0,     REV_B0},
  {           0x2,   CPU_CHOP_ANY,       0,     REV_C0},
  {           0x3,   CPU_CHOP_XCC,       0,     REV_D0},
  {           0x3,   CPU_CHOP_MCC,       0,     REV_R0},
  {           0x3,   CPU_CHOP_UCC,       0,     REV_U0},
};

CPU_REVISION_LOOKUP
GnrSpLookup[] = {
  //  Stepping ID,   PhysicalChop,    Flag,   Revision
  {           0x0,   CPU_CHOP_ANY,       0,     REV_A0},
};

CPU_REVISION_LOOKUP
SrfSpLookup[] = {
  //  Stepping ID,   PhysicalChop,    Flag,   Revision
  {           0x0,   CPU_CHOP_ANY,       0,     REV_A0},
};

//
// CpuTypeLookup table is used to separate the revisions of specific
// CpuTypes so that the lookup is much faster due to reduced # of entries
// Note: Due to the use, CPU_XXX defines must be sequential 0 to (N-1)
//
CPU_TYPE_LOOKUP
CpuTypeLookup[] = {
  {   CPU_SKX,    SkxLookup     , ARRAY_SIZE (SkxLookup)},
  {   CPU_CLX,    ClxLookup     , ARRAY_SIZE (ClxLookup)},
  {   CPU_CPX,    CpxLookup     , ARRAY_SIZE (CpxLookup)},
  {   CPU_SNR,    SnrLookup     , ARRAY_SIZE (SnrLookup)},
  {   CPU_ICXSP,  IcxSpLookup   , ARRAY_SIZE (IcxSpLookup)},
  {   CPU_ICXD,   IcxDLookup    , ARRAY_SIZE (IcxDLookup)},
  {   CPU_SPRSP,  SprSpLookup   , ARRAY_SIZE (SprSpLookup)},
  {   CPU_TNR,    TnrLookup     , ARRAY_SIZE (TnrLookup)},
  {   CPU_GNRSP,  GnrSpLookup   , ARRAY_SIZE (GnrSpLookup)},
  {   CPU_SRFSP,  SrfSpLookup   , ARRAY_SIZE (SrfSpLookup)},
};

#pragma pack ()

/**

  Allocates the required resources for CpuAndRevision


  @retval     EFI_SUCCESS         This function completed execution with no warnings/errors
  @retval     EFI_OUT_OF_RESOURCES  Pei services not able to allocate resources for HBM Host

**/
EFI_STATUS
AllocateCpuAndRevisionStaticPointer (
  VOID
  )
{
  VOID *CpuAndRevision;

  CpuAndRevision = (VOID *)(UINTN) AllocateZeroPool (sizeof (CPU_AND_REVISION_INFO));

  if (CpuAndRevision == NULL) {
    ASSERT (CpuAndRevision != NULL);
    return EFI_OUT_OF_RESOURCES;
  }

  SetCpuAndRevisionPointer (CpuAndRevision);

  return EFI_SUCCESS;
}

/**

  Local function to find the chop type of the executing CPU.

  @param   UINT8  CpuType   Value of currently executing CPU

  @retval  UINT8  ChopType  ChopType of the executing CPU.
                            See CPU_CHOP_TYPE define.

**/
UINT8
GetChop (
  UINT8   CpuType
)
{
  MSR_CPU_BUSNUMBER_REGISTER  BusNumber;
  CAPID4_PCU_FUN3_STRUCT      CapId4;
  CPU_CSR_ACCESS_VAR          *CpuCsrAccessVarPtr;
  UINT32                      MmioAddress;
  CSR_OFFSET                  RegOffset;
  UINT8                       Chop = CPU_CHOP_XCC;

  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar ();
  //
  // If prior to KTI RC, check to see if mmfCfgBase is assigned
  // if it is assigned, KtiRC has initialized enough and we can use KtiApiLib
  //
  if (!IsChopTypeValid ()) {
    if (CpuType == CPU_SKX) {
      BusNumber.Uint64 = AsmReadMsr64 (SKX_MSR_CPU_BUSNUMBER);
    } else {
      BusNumber.Uint64 = AsmReadMsr64 (MSR_CPU_BUSNUMBER);
    }
    ASSERT (BusNumber.Bits.Valid == 1);

    RegOffset.Data = CAPID4_PCU_FUN3_REG;
    //
    // Assert if we encounter a Pseudo offset for this define to protect against bad CSR read.
    // This is purely protection for future SOCs that may need to use pseudo, revisit if need arises
    //
    ASSERT (!RegOffset.Bits.pseudo);

    //
    // Kti has not initialized, so use PCIE default base address
    //
    if (BusNumber.Bits.Valid == 1) {
      if (CpuCsrAccessVarPtr->mmCfgBase == 0) {
        MmioAddress = (UINT32) PcdGet64 (PcdPciExpressBaseAddress);
      } else {
        MmioAddress = CpuCsrAccessVarPtr->mmCfgBase;
      }
      MmioAddress |= PCI_PCIE_ADDR (BusNumber.Bits.Cpubusno1, 30, 3, RegOffset.Bits.offset & 0xfff);
      CapId4.Data = MmioRead32 (MmioAddress);
      Chop = (UINT8) CapId4.Bits.physical_chop;
    } else {
      DEBUG ((DEBUG_ERROR, "%a: Error: CPUBUSNO invalid.\n", __FUNCTION__));
    }
  } else {
    //
    // If grabbing the chop type after KTI initializes, use KtiApiLib
    //
    Chop = (UINT8) GetChopType (GetSysSbspSocketIdNv ());
  }

  return Chop;
}

/**

  Parses CpuRevision lookup table to find a match with the given inputs.

  @param   CpuType      The determined CpuType of executing CPU
  @param   CpuStepping  Cpu Physical Stepping ID of current CPU.
  @param   Flag         Any special value used to identify silent steppings.

  @retval  REVISION_INFO  Revision info found with given inputs.
                          Zero indicates the revision was not found.

**/
REVISION_INFO
FindRevision (
  UINT8   CpuType,
  UINT8   CpuStepping,
  UINT8   Flag
  )
{
  UINT16          Revision = 0;
  UINT8           Index;
  REVISION_INFO   RevisionInfo;
  CPU_REVISION_LOOKUP *LookupPtr;

  //
  // Verify the CpuTypeLookup table is sequential and the defines are correct
  //
  for (Index = 0; Index < ARRAY_SIZE (CpuTypeLookup); Index++) {
    if (CpuTypeLookup[Index].CpuType != Index ) {
      DEBUG ((DEBUG_ERROR, "%a: Invalid CpuTypeLookup - Index: %d\n", __FUNCTION__, Index));
      ASSERT (FALSE);
    }
  }

  LookupPtr = CpuTypeLookup[CpuType].LookupPtr;

  for (Index = 0; Index < CpuTypeLookup[CpuType].EntryCount; Index++) {
    if ((LookupPtr[Index].CpuStepping == CpuStepping) &&  (LookupPtr[Index].Flag == Flag)) {
      if (LookupPtr[Index].CpuChop == CPU_CHOP_ANY) {
        Revision = LookupPtr [Index].Revision;
        break;
      } else if (LookupPtr[Index].CpuChop == GetChop (CpuType)) {
        Revision = LookupPtr[Index].Revision;
        break;
      }
    }
  }

  if (Revision == 0) {
    DEBUG ((DEBUG_ERROR, "CPU Revision undefined. CpuType: 0x%x, Stepping: 0x%x, Chop: %d, Flag: %d\n", CpuType, CpuStepping, GetChop (CpuType), Flag));
    ASSERT (FALSE);
  }

  DEBUG ((DEBUG_INFO, "CpuType: %d, SteppingID: 0x%x, Revision: 0x%x, Index: %d\n", CpuType, CpuStepping, Revision, Index));

  RevisionInfo.Revision = Revision;
  RevisionInfo.Index = Index;

  return RevisionInfo;
}

/**
  Populates the allocated memory with the CpuType and Stepping ID.

  @param *CpuAndRevision - if not NULL, then populate this pointer,
                           otherwise, find global pointer from static table

  @retval  Status EFI_DEVICE_ERROR - Unable to find revision or CPU family is unknown
                  EFI_SUCCESS      - Found CPU revision and memory is stored

**/
EFI_STATUS
PopulateCpuType (
  CPU_AND_REVISION_INFO  *CpuAndRevision
  )
{
  UINT32                  RegEax = 0;
  UINT32                  CpuFamily;
  UINT8                   CpuType;
  UINT8                   CpuStepping;

  AsmCpuid (CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);
  CpuFamily = (UINT32)((RegEax >> 4) & 0xFFFF);
  CpuStepping = (UINT8)(RegEax & 0xF);

  CpuType = (UINT8) -1;

  switch (CpuFamily) {
    case CPU_FAMILY_SKX:
      if (CpuStepping >= 0xA) {
        CpuType = CPU_CPX;
      } else if (CpuStepping >= 0x5) {
        CpuType = CPU_CLX;
      } else {
        CpuType = CPU_SKX;
      }
      break;

    case CPU_FAMILY_ICX:
      CpuType = CPU_ICXSP;
      break;

    case CPU_FAMILY_ICXD:
      CpuType = CPU_ICXD;
      break;

    case CPU_FAMILY_SNR:
      if (CpuStepping >= 0xA) {
        CpuType = CPU_TNR;
      } else {
        CpuType = CPU_SNR;
      }
      break;

    case CPU_FAMILY_SPRSP:
      CpuType = CPU_SPRSP;
      break;

    case CPU_FAMILY_GNRSP:
      CpuType = CPU_GNRSP;
      break;

    case CPU_FAMILY_SRFSP:
      CpuType = CPU_SRFSP;
      break;

    default:
      DEBUG ((DEBUG_ERROR, "Unknown CPU family 0x%x\n", CpuFamily));
      ASSERT (FALSE);
      return EFI_DEVICE_ERROR;
  }

  //
  // Get a pointer to our static table data if local version not passed in.
  //
  if (CpuAndRevision == NULL) {
    CpuAndRevision = (CPU_AND_REVISION_INFO *)GetCpuAndRevisionPointer ();
  }

  //
  // Now fill in our known data.
  //
  CopyMem (&CpuAndRevision->SmmSize.Guid, &gStaticPointerSmmSizeGuid, sizeof (EFI_GUID));
  CpuAndRevision->SmmSize.Size  = sizeof (CPU_AND_REVISION_INFO);
  CpuAndRevision->CpuType       = CpuType;
  CpuAndRevision->CpuStepping   = CpuStepping;
  CpuAndRevision->Revision      = 0;
  CpuAndRevision->Flag          = 0;
  CpuAndRevision->Index         = 0;
  CpuAndRevision->Normalized    = FALSE;

  CpuAndRevision->Signature = CPU_AND_REVISION_SIGNATURE;

  return EFI_SUCCESS;
}

/**
  Populates the allocated memory with Revision and Flag information

  @retval  None

**/
VOID
PopulateRevision (
  CPU_AND_REVISION_INFO   *CpuAndRevision
)
{
  REVISION_INFO           RevisionInfo;
  UINT8                   Flag = 0;

  RevisionInfo = FindRevision (CpuAndRevision->CpuType, CpuAndRevision->CpuStepping, Flag);

  CpuAndRevision->Revision    = RevisionInfo.Revision;
  CpuAndRevision->Index       = RevisionInfo.Index;
  CpuAndRevision->Flag        = Flag;

  return;
}

/**
  Initialize the CpuAndRevision information in the static pointer table.

  @param      None

  @return     EFI_SUCCESS

**/
EFI_STATUS
InitializeCpuAndRevision (
  VOID
  )
{
  EFI_STATUS Status;

  Status = AllocateCpuAndRevisionStaticPointer ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = PopulateCpuType (NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;

}

/**
  Finds the CpuAndRevision pointer and returns it.  If the global static
  pointer is not available, then the local CpuAndRevision variable passed
  in is populated and a pointer to the local version is returned.

  @param  CPU_AND_REVISION_INFO   *LocalCpuAndRevision - pointer to a local
                                    buffer of this variable.

  @retval CPU_AND_REVISION_INFO - Pointer to valid CpuAndRevision information

**/
CPU_AND_REVISION_INFO *
GetCpuAndRevisionInfoPointer (
  CPU_AND_REVISION_INFO   *LocalCpuAndRevision
)
{
  CPU_AND_REVISION_INFO   *CpuAndRevision;
  EFI_STATUS              Status;

  CpuAndRevision = (CPU_AND_REVISION_INFO *)GetCpuAndRevisionPointer ();
  if ((CpuAndRevision == NULL) ||
      (CpuAndRevision->Signature != CPU_AND_REVISION_SIGNATURE)) {
    //
    // Unable to find static pointer due to constructor not run yet
    // Use local structure to populate and use
    //
    Status = PopulateCpuType (LocalCpuAndRevision);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: Unable to create buffer, %r\n", __FUNCTION__, Status));
      ASSERT (FALSE);
    }
    //
    // Update pointer passed back to be the local version.
    //
    CpuAndRevision = LocalCpuAndRevision;
  }
  return CpuAndRevision;
}

/**
  Check if the current CPU executing is this type and revision.

  @param[in] CpuType      UINT8 of the CpuType define
  @param[in] Revision     UINT16 of the Revision to check against

  @retval  TRUE    Is the requested CPU and Revision
           FALSE   Is not the requested CPU and Revision

**/
BOOLEAN
EFIAPI
IsCpuAndRevision (
  IN UINT8  CpuType,
  IN UINT16 Revision
  )
{
  CPU_AND_REVISION_INFO   *CpuAndRevision = NULL;
  CPU_AND_REVISION_INFO   LocalCpuAndRevision;
  BOOLEAN                 RevisionMatch = FALSE;

  if (Revision < REV_A0) {
    DEBUG ((DEBUG_ERROR, "%a: Invalid Revision input: 0x%x\n", __FUNCTION__, Revision));
    ASSERT (FALSE);
    return FALSE;
  }

  CpuAndRevision = GetCpuAndRevisionInfoPointer (&LocalCpuAndRevision);

  if (CpuAndRevision->CpuType == CpuType) {
    if (Revision != REV_ALL && CpuAndRevision->Revision == 0) {
      PopulateRevision (CpuAndRevision);
    }
    if (CpuAndRevision->Revision == Revision || Revision == REV_ALL) {
      RevisionMatch = TRUE;
    } else if (((Revision & MINOR_REV_MASK) == MINOR_REV_MASK) &&
      ((Revision & MAJOR_REV_MASK) == (CpuAndRevision->Revision & MAJOR_REV_MASK))) {
      //
      // Check for wildcard revision match
      //
      RevisionMatch = TRUE;
    }
  }

  return RevisionMatch;
}

/**
  Check if the current CPU executing is this CPU type and this revision or
  a later revision.  This would be used if code needs to apply for one
  revision and all future revisions of the same CPU type.

  @param[in] CpuType      UINT8 of the CpuType define
  @param[in] Revision     UINT16 of the Revision to start with

  @retval  TRUE    Is the requested CPU and Revision
           FALSE   Is not the requested CPU and Revision

**/
BOOLEAN
EFIAPI
IsCpuAndRevisionOrLater (
  IN UINT8  CpuType,
  IN UINT16 Revision
  )
{
  CPU_AND_REVISION_INFO   *CpuAndRevision = NULL;
  CPU_AND_REVISION_INFO   LocalCpuAndRevision;
  BOOLEAN                 RevisionMatch = FALSE;
  BOOLEAN                 RevisionFound = FALSE;
  CPU_REVISION_LOOKUP     *LookupPtr;
  UINT8                   Index;

  //
  // Do not allow wildcard revisions or REV_ALL for trying to
  // determine the sequencing of revisions.  Need specific revision
  // input from caller.
  //
  if (Revision < REV_A0 || Revision == REV_ALL ||
    ((Revision & MINOR_REV_MASK) == MINOR_REV_MASK)) {
    DEBUG ((DEBUG_ERROR, "%a: Invalid Revision input: 0x%x\n", __FUNCTION__, Revision));
    ASSERT (FALSE);
    return FALSE;
  }

  CpuAndRevision = GetCpuAndRevisionInfoPointer (&LocalCpuAndRevision);

  if (CpuAndRevision->CpuType == CpuType) {
    if (CpuAndRevision->Revision == 0) {
      PopulateRevision (CpuAndRevision);
    }
    if (CpuAndRevision->Revision == Revision) {
      //
      // Exact match, skip the look up table search.
      //
      RevisionMatch = TRUE;
    } else {
      //
      // Parse the look up table to look for the requested revision
      // When requested revision is found, check to make sure it is less/earlier
      // than the currently executing CPU.
      //
      LookupPtr = CpuTypeLookup[CpuType].LookupPtr;

      for (Index = 0; Index < CpuTypeLookup[CpuType].EntryCount; Index++) {
        if (LookupPtr[Index].Revision == Revision) {
          RevisionFound = TRUE;
          if (Index < CpuAndRevision->Index) {
            RevisionMatch = TRUE;
          }
          break;
        }
      }
      if (!RevisionFound) {
        //
        // The revision requested was not found in the lookup table, return FALSE in production
        //
        RevisionMatch = FALSE;
        DEBUG ((DEBUG_ERROR, "ERROR: CpuType: %d, Revision 0x%x undefined!\n", CpuType, Revision));
        ASSERT (FALSE);
      }
    }
 }

  return RevisionMatch;
}

/**
  Function for a very specific case where mixed steppings are populated
  on the board.  This will align CPU data so that the lowest stepping Id
  is used for all CPU and revision checks.  This should only be called
  by KTI code.

  @param   UINT8       SteppingId - Physical Stepping Id to change to.

  @retval  EFI_STATUS  Status - EFI_SUCCESS: Revision updated
                                EFI_ALREADY_STARTED: Already called

**/
EFI_STATUS
EFIAPI
NormalizeCpuAndRevision (
  UINT8     SteppingId
  )
{
  CPU_AND_REVISION_INFO   *CpuAndRevision = NULL;
  CPU_AND_REVISION_INFO   LocalCpuAndRevision;
  EFI_STATUS              Status = EFI_SUCCESS;
  REVISION_INFO           RevisionInfo;

  CpuAndRevision = GetCpuAndRevisionInfoPointer (&LocalCpuAndRevision);

  if (CpuAndRevision->Normalized) {
    DEBUG ((DEBUG_ERROR, "%a called twice!\n", __FUNCTION__));
    return EFI_ALREADY_STARTED;
  }

  if (CpuAndRevision->Revision == 0) {
    PopulateRevision (CpuAndRevision);
  }

  if (SteppingId < CpuAndRevision->CpuStepping) {
    //
    // Remote socket stepping ID is lower than BSP.  Update the Revision to lowest
    //
    DEBUG((DEBUG_ERROR, "%a: CpuRevision updated! (0x%x -> 0x%x)!\n", __FUNCTION__, CpuAndRevision->CpuStepping, SteppingId));
    CpuAndRevision->CpuStepping = SteppingId;
    RevisionInfo = FindRevision (CpuAndRevision->CpuType, SteppingId, CpuAndRevision->Flag);
    CpuAndRevision->Revision = RevisionInfo.Revision;
    CpuAndRevision->Index = RevisionInfo.Index;
  }

  CpuAndRevision->Normalized = TRUE;

  return Status;
}

/**
  Set CPU type and revision data. Only valid for simulation
  build targets, will assert on hardware targets.  Passing
  in REV_ALL as the Revision will select first Revision for
  the CpuType passed in.

  @param[in] CpuType      UINT8 of the CpuType define
  @param[in] Revision     UINT16 of the Revision to use

  @retval    None

**/
VOID
EFIAPI
SetCpuAndRevision (
  IN UINT8  CpuType,
  IN UINT16 Revision
  )
{
  ASSERT (FALSE);
  return;
}

/**
  Constructor for this library that does the first initialization
  of the data pool and gathers CpuType information

  @param    None

  @retval   EFI_SUCCESS

**/
RETURN_STATUS
EFIAPI
CpuAndRevisionConstructor (
  VOID
  )
{
  if (GetCpuAndRevisionPointer () == NULL) {
    InitializeCpuAndRevision ();
  }
  return EFI_SUCCESS;
}
