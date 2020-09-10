/** @file
  This is an implementation of the eMCA driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2019 Intel Corporation. <BR>

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
#include <Library/SynchronizationLib.h>
#include <Library/ProcessorRasLib.h>
#include <Library/RasDebugLib.h>
#include "EnhancedMcaLogPrivateLib.h"

#define L1_DIR_PTR_LEN  8 //bytes
#define ELOG_ENTRY_LEN 0x1000
#define EMCA_L1_VERSION 0x0100
#define OS_VMM_OPT_IN_FLAG BIT0


VOID   *mL1DirHdrBkp = NULL;
VOID   *mElogDirBaseBkp = NULL;
UINT64  mL1DirSizeBkp = 0;
UINT64  mElogDirSizeBkp = 0;


UINTN   mThreadBits = 0;
UINTN   mCoreBits = 0;

typedef struct {
  UINT32 Version;
  UINT32 HdrLen;
  UINT64 L1DirLen;
  UINT64 ElogDirBase;
  UINT64 ElogDirLen;
  UINT32 Flags;
  UINT32 reserved0;
  UINT64 reserved1;
  UINT32 NumL1EntryLp;
  UINT32 reserved2;
  UINT64 reserved3;
} EMCA_L1_HEADER;

/**
  Get the number of L1 Dir Entries required based on the MC Banks available and number of sockets.

  Number of L1 Dir Entries = SOCKET_COUNT * ( 2 ** APICID_BITS_PER_SOCKET) * MCBANKS_PER_LP where:

  SOCKET_COUNT is  the maximum number  of  processor  sockets in  the platform,
  APICID_BITS_PER_SOCKET is the number of APIC ID bits used up by each socket
  MCBANKS_PER_LP = MCBANK_CORE + MCBANKS_UNCORE = MCG_CAP.Count

  @return number of L1 Dir Entries

**/
UINT64
GetNumL1DirEntries (
  VOID
  )
{
  UINT64 Num = 0;
  UINT64 ApicIdBitsPerSkt = 0;

  ApicIdBitsPerSkt = mThreadBits + mCoreBits;

  Num = MultU64x32 (MultU64x64 (LShiftU64 (1 , (UINTN) ApicIdBitsPerSkt), GetNumMcBanksPerLp ()), MAX_SOCKET);

  return Num;
}


/**
  Get the number of Elog Dir(L2) Entries required based on the MC Banks in the system.

 NumElogentries = number of MC BANKS in the system
 = NumSKTSupported * NumMCBanksPerSKT
 = NumSKTSupported * (NumCoreMcBanks *  CORES_PER_SOCKET + NumUncoreMcBanks )

  @return Number of Elog Entries

**/
UINT64
GetNumElogEntries (
  VOID
  )
{
  UINT32 i;
  UINT64 McBanksNum;
  UINT64 PhyMcBankNum = 0;
  UINT16  McScope;

  McBanksNum = GetNumMcBanksPerLp ();

  for (i = 0; i < McBanksNum; i++) {
    if (IsMcBankSupportEmcaLog (i)) {
      McScope = GetMcBankScope (i);
      switch (McScope) {

        case MC_SCOPE_TD:
          PhyMcBankNum += MultU64x32 (MultU64x64 (LShiftU64 (1 , mThreadBits), LShiftU64 (1 , mCoreBits)), MAX_SOCKET);
          break;
        case MC_SCOPE_CORE:
          PhyMcBankNum += MultU64x32 (LShiftU64 (1 , mCoreBits), MAX_SOCKET);
          break;
        case MC_SCOPE_PKG:
          PhyMcBankNum += (MAX_SOCKET);
          break;
        default:
          RAS_ASSERT(FALSE); //should not enter here
          break;
      }
    }
  }

  return PhyMcBankNum;
}

/**

    Initialize L1 Directory pointers according to eMCA spec. This assumes the McBank Scope is Thread Level so each thread will have its own elog pointer.

    @param L1DirHdr - L1 directory base pointer
    @param McBank   - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.
    @param NxtElog  - Pointer to Next Free Elog memory

    @retval EFI_INVALID_PARAMETER - At least of the input pointers is NULL
    @retval EFI_OUT_OF_RESOURCES  - Elog directory memory is insufficient
    @retval EFI_SUCCESS           - The initialization was successful

**/
EFI_STATUS
InitL1PtrForThreads (
  IN EMCA_L1_HEADER *L1DirHdr,
  IN UINT32 McBank,
  IN OUT VOID **NxtElog
  )
{
  UINT64 s;
  UINT64 c;
  UINT64 t;
  UINT64 ApicId;
  UINT8 *L1DirBase = NULL;
  UINT8 *L1DirEnd = NULL;
  UINT8 *ElogEnd = NULL;
  UINT8 *NxtElogIter = NULL;
  UINT64 *L1DirIter = NULL;

    if (L1DirHdr == NULL) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: L1DirHdr is NULL\n"));
      RAS_ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
    }
    if (NxtElog == NULL) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: NxtElog is NULL\n"));
      RAS_ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
    }
    if (McBank >= L1DirHdr->NumL1EntryLp) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: McBank >= L1DirHdr->NumL1EntryLp \n"));
      RAS_ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
    }

  NxtElogIter = (UINT8 *) *NxtElog;
  ElogEnd = (UINT8 *) (VOID *) (UINTN) (L1DirHdr->ElogDirBase + L1DirHdr->ElogDirLen);
  L1DirBase = (UINT8 *) (VOID *) (UINTN) ((UINT64) L1DirHdr + (UINT64) L1DirHdr->HdrLen);
  L1DirEnd = (UINT8 *) (VOID *) (UINTN) ((UINT64) L1DirHdr + L1DirHdr->L1DirLen);

  for (s = 0; s < MAX_SOCKET; s++) {
    for (c = 0; c < LShiftU64 (1 , mCoreBits); c++) {
      for (t = 0; t < LShiftU64 (1, mThreadBits); t++, NxtElogIter += ELOG_ENTRY_LEN) {
        ApicId = LShiftU64 (s, mThreadBits + mCoreBits) + LShiftU64 (c, mThreadBits) + t;
        L1DirIter = (UINT64 *) (VOID *) (UINTN) ((UINT64) L1DirBase + MultU64x32 (ApicId, L1DirHdr->NumL1EntryLp * sizeof (UINT64)) + (McBank * sizeof (UINT64)));

        if (NxtElogIter < ElogEnd && (UINT8 *) L1DirIter < L1DirEnd) {
          *L1DirIter = BIT63 | (UINT64) NxtElogIter;
        } else {
          return EFI_OUT_OF_RESOURCES;
        }
      }
    }
  }

  *NxtElog = (VOID*) NxtElogIter;
  return EFI_SUCCESS;
}


/**

    Initialize L1 Directory pointers according to eMCA spec. This assumes the McBank Scope is Core Level so each Thread within the Core will share elog pointer.

    @param L1DirHdr - L1 directory base pointer
    @param McBank   - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.
    @param NxtElog  - Pointer to Next Free Elog memory

    @retval EFI_INVALID_PARAMETER - At least of the input pointers is NULL
    @retval EFI_OUT_OF_RESOURCES  - Elog directory memory is insufficient
    @retval EFI_SUCCESS           - The initialization was successful

**/
EFI_STATUS
InitL1PtrForCores (
  IN EMCA_L1_HEADER *L1DirHdr,
  IN UINT32 McBank,
  IN OUT VOID **NxtElog
  )
{
  UINT64 s;
  UINT64 c;
  UINT64 t;
  UINT64 ApicId;
  UINT8 *L1DirBase = NULL;
  UINT8 *L1DirEnd = NULL;
  UINT8 *ElogEnd = NULL;
  UINT8 *NxtElogIter = NULL;
  UINT64 *L1DirIter = NULL;


    if (L1DirHdr == NULL) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: L1DirHdr is NULL\n"));
      RAS_ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
    }
    if (NxtElog == NULL) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: NxtElog is NULL\n"));
      RAS_ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
    }
    if (McBank >= L1DirHdr->NumL1EntryLp) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: McBank >= L1DirHdr->NumL1EntryLp \n"));
      RAS_ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
    }
      NxtElogIter = (UINT8 *) *NxtElog;
      ElogEnd = (UINT8 *) (VOID *) (UINTN) (L1DirHdr->ElogDirBase + L1DirHdr->ElogDirLen);
      L1DirBase = (UINT8 *) (VOID *) (UINTN) ((UINT64) L1DirHdr + (UINT64) L1DirHdr->HdrLen);
      L1DirEnd = (UINT8 *) (VOID *) (UINTN) ((UINT64) L1DirHdr + L1DirHdr->L1DirLen);

  for (s = 0; s < MAX_SOCKET; s++) {
    for (c = 0; c < LShiftU64 (1 , mCoreBits); c++, NxtElogIter += ELOG_ENTRY_LEN) {
      for (t = 0; t < LShiftU64 (1, mThreadBits); t++) {
        ApicId = LShiftU64 (s, mThreadBits + mCoreBits) + LShiftU64 (c, mThreadBits) + t;
        L1DirIter = (UINT64 *) (VOID *) (UINTN) ((UINT64) L1DirBase + MultU64x32 (ApicId, L1DirHdr->NumL1EntryLp * sizeof (UINT64)) + (McBank * sizeof (UINT64)));

        if (NxtElogIter < ElogEnd && (UINT8 *) L1DirIter < L1DirEnd) {
          *L1DirIter = BIT63 | (UINT64) NxtElogIter;
        } else {
          return EFI_OUT_OF_RESOURCES;
        }
      }
    }
  }

  *NxtElog = (VOID*) NxtElogIter;
  return EFI_SUCCESS;
}

/**

    Initialize L1 Directory pointers according to eMCA spec. This assumes the McBank Scope is Package Level so all the threads within the socket will share the elog pointer.

    @param L1DirHdr - L1 directory base pointer
    @param McBank   - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.
    @param NxtElog  - Pointer to Next Free Elog memory

    @retval EFI_INVALID_PARAMETER - At least of the input pointers is NULL
    @retval EFI_OUT_OF_RESOURCES  - Elog directory memory is insufficient
    @retval EFI_SUCCESS           - The initialization was successful

**/
EFI_STATUS
InitL1PtrForSockets (
  IN EMCA_L1_HEADER *L1DirHdr,
  IN UINT32 McBank,
  IN OUT VOID **NxtElog
  )
{
  UINT64 s;
  UINT64 c;
  UINT64 t;
  UINT64 ApicId;
  UINT8 *L1DirBase = NULL;
  UINT8 *L1DirEnd = NULL;
  UINT8 *ElogEnd = NULL;
  UINT8 *NxtElogIter = NULL;
  UINT64 *L1DirIter = NULL;

    if (L1DirHdr == NULL) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: L1DirHdr is NULL\n"));
      RAS_ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
    }
    if (NxtElog == NULL) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: NxtElog is NULL\n"));
      RAS_ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
    }
    if (McBank >= L1DirHdr->NumL1EntryLp) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: McBank >= L1DirHdr->NumL1EntryLp \n"));
      RAS_ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
    }

  NxtElogIter = (UINT8 *) *NxtElog;
  ElogEnd = (UINT8 *) (VOID *) (UINTN) (L1DirHdr->ElogDirBase + L1DirHdr->ElogDirLen);
  L1DirBase = (UINT8 *) (VOID *) (UINTN) ((UINT64) L1DirHdr + (UINT64) L1DirHdr->HdrLen);
  L1DirEnd = (UINT8 *) (VOID *) (UINTN) ((UINT64) L1DirHdr + L1DirHdr->L1DirLen);

  for (s = 0; s < MAX_SOCKET; s++, NxtElogIter += ELOG_ENTRY_LEN) {
    for (c = 0; c < LShiftU64 (1 , mCoreBits); c++) {
      for (t = 0; t < LShiftU64 (1, mThreadBits); t++) {
        ApicId = LShiftU64 (s, mThreadBits + mCoreBits) + LShiftU64 (c, mThreadBits) + t;
        L1DirIter = (UINT64 *) (VOID *) (UINTN) ((UINT64) L1DirBase + MultU64x32 (ApicId, L1DirHdr->NumL1EntryLp * sizeof (UINT64)) + (McBank * sizeof (UINT64)));

        if (NxtElogIter < ElogEnd && (UINT8 *) L1DirIter < L1DirEnd) {
          *L1DirIter = BIT63 | (UINT64) NxtElogIter;
        } else {
          return EFI_OUT_OF_RESOURCES;
        }
      }
    }
  }
  *NxtElog = (VOID*) NxtElogIter;
  return EFI_SUCCESS;
}

/**
  Get memory size in bytes required to store the L1 Directory.

  L1 Dir Size = Number of L1 Dir entries * 8 + 0x40

  Number of L1 Dir Entries = SOCKET_COUNT * ( 2 ** APICID_BITS_PER_SOCKET) * MCBANKS_PER_LP where:

  SOCKET_COUNT is  the maximum number  of  processor  sockets in  the platform,
  APICID_BITS_PER_SOCKET is the number of APIC ID bits used up by each socket
  MCBANKS_PER_LP = MCBANK_CORE + MCBANKS_UNCORE = MCG_CAP.Count

  @return required sized in bytes

**/
UINT64
EFIAPI
GetL1DirSize (
  VOID
  )
{
  return MultU64x32 (GetNumL1DirEntries (), L1_DIR_PTR_LEN) + sizeof (EMCA_L1_HEADER);
}

/**
  Get memory size in bytes required to store the Elog Directory(L2).

  NumElogEntries * 0x400

  @return required sized in bytes

**/
UINT64
EFIAPI
GetElogDirSize (
  VOID
  )
{
  return MultU64x32 (GetNumElogEntries (), ELOG_ENTRY_LEN);
}

/**

    Initialize L1 Directory pointers according to eMCA spec. This will take the elog directory
    memory and the l1 directory memory and initialize the elog directory entry pointers depending
    on the processor topology and mc bank structure.

    @param L1Dir - Pointer to the L1 Directory Header

    @retval EFI_SUCCESS - The initialization was successful
    @retval EFI_INVALID_PARAMETER - L1 Pointer is null or invalid
    @retval EFI_OUT_OF_RESOURCES - Elog directory memory is insufficient

**/
EFI_STATUS
EFIAPI
InitL1Dir (
  IN VOID                         *L1Dir
  )
{
  EMCA_L1_HEADER *L1DirHdr;
  UINT32 i;
  UINT16  McScope;
  UINT8 *NxtElog;
  EFI_STATUS Status = EFI_SUCCESS;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "InitL1Dir++\n"));

  L1DirHdr = (EMCA_L1_HEADER *) L1Dir;
  NxtElog = (UINT8 *) (VOID *) (UINTN) L1DirHdr->ElogDirBase;

  for (i = 0; i < L1DirHdr->NumL1EntryLp; i++) {
    if (IsMcBankSupportEmcaLog (i)) {
      McScope = GetMcBankScope (i);

      switch (McScope) {
        case MC_SCOPE_TD:
          Status = InitL1PtrForThreads (L1DirHdr, i, &NxtElog);
          ASSERT_EFI_ERROR (Status);
          if (EFI_ERROR (Status)) {
            return Status;
          }
          break;
        case MC_SCOPE_CORE:
          Status = InitL1PtrForCores (L1DirHdr, i, &NxtElog);
          ASSERT_EFI_ERROR (Status);
          if (EFI_ERROR (Status)) {
            return Status;
          }
          break;
        case MC_SCOPE_PKG:
          Status = InitL1PtrForSockets (L1DirHdr, i, &NxtElog);
          ASSERT_EFI_ERROR (Status);
          if (EFI_ERROR (Status)) {
            return Status;
          }
          break;
        default:
          RAS_ASSERT(FALSE); //should not enter here
          break;
      }
    }
  }

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "InitL1Dir--. %r\n", Status));

  return Status;
}

/**

    Validate the Elog pointer, McBankNum parameters

    @param ElogPointer - Pointer to be validated
    @param McBankNum - MC Bank Number
    @param ApicId - ApiC ID

    @retval TRUE  - ElogPointer is valid for this Bank Number
    @retval FALSE - ElogPointer is invalid for this Bank Number

**/
BOOLEAN
EFIAPI
ValidateElogPointer (
  IN VOID                         *ElogPointer,
  IN UINT32                        McBankNum,
  IN UINT64                        ApicId
  )
{
  EMCA_L1_HEADER *L1DirHdr;
  UINT64         *L1DirIter = NULL;

  L1DirHdr = (EMCA_L1_HEADER *) mL1DirHdrBkp;

    if (McBankNum >= (UINT32) GetNumMcBanksPerLp ()) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: McBankNum < (UINT32) GetNumMcBanksPerLp () \n"));
      RAS_ASSERT (FALSE);
      return FALSE;
    }

  // Validate the L1 Dir Header date
  if ((L1DirHdr->HdrLen != sizeof (EMCA_L1_HEADER)) || \
      (L1DirHdr->L1DirLen != mL1DirSizeBkp) || \
      (L1DirHdr->ElogDirBase != (UINT64)mElogDirBaseBkp) || \
      (L1DirHdr->ElogDirLen != mElogDirSizeBkp))
{
    return FALSE;
  }

  // Validate if the ElogPointer is in valid range
  if (((UINT64) ElogPointer < (UINT64) mElogDirBaseBkp) || \
      ((UINT64)ElogPointer > ((UINT64)mElogDirBaseBkp + mElogDirSizeBkp))){
    return FALSE;
  }

  if (!IsMcBankSupportEmcaLog (McBankNum)) {
    return FALSE;
  }

  L1DirIter = (UINT64 *) (VOID *) (UINTN) ( \
                ((UINT64)mL1DirHdrBkp + sizeof(EMCA_L1_HEADER)) + \
                MultU64x32(ApicId, L1DirHdr->NumL1EntryLp * sizeof(UINT64)) + \
                (McBankNum * sizeof(UINT64)));

  if ((UINT64) ElogPointer == *L1DirIter) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**

    This routine takes the L1 directory pointer and initialize the header with correct values

    @param L1DirHdr    - L1 directory base pointer
    @param ElogDirBase - Elog directory base pointer
    @param L1DirSize   - L1 directory size in bytes
    @param ElogSize    - Elog directory size in bytes

    @retval EFI_INVALID_PARAMETER - At least one of the input pointers was NULL
    @retval EFI_SUCCESS           - The directory was initialized succesfully

**/
EFI_STATUS
EFIAPI
CreateL1DirHdr (
  IN OUT EMCA_L1_HEADER *L1DirHdr,
  IN VOID *ElogDirBase,
  IN UINT64 L1DirSize,
  IN UINT64 ElogSize
  )
{
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "CreateL1DirHdr++\n"));

  if (L1DirHdr == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "ERROR: L1DirHdr is NULL\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (ElogDirBase == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "ERROR: ElogDirBase is NULL\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  L1DirHdr->Version = EMCA_L1_VERSION;
  L1DirHdr->HdrLen = sizeof (EMCA_L1_HEADER);
  L1DirHdr->L1DirLen = L1DirSize;
  L1DirHdr->ElogDirBase = (UINT64) ElogDirBase;
  L1DirHdr->ElogDirLen = ElogSize;
  L1DirHdr->NumL1EntryLp = (UINT32) GetNumMcBanksPerLp ();

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "L1DirHdr: Version: %X, HdrLen: %X, L1DirLen: %X, ElogDirBase: %X, ElogDirLen: %X, NumL1EntryLp: %X\n", \
             L1DirHdr->Version, \
             L1DirHdr->HdrLen, \
             L1DirHdr->L1DirLen, \
             L1DirHdr->ElogDirBase, \
             L1DirHdr->ElogDirLen, \
             L1DirHdr->NumL1EntryLp));

  return EFI_SUCCESS;
}

/**
  Allocates L1 & Elog(L2) memory and the pointer is stored in the parameter.

  @param  Buffer                A pointer to a pointer to the allocated buffer if the call succeeds;
                                undefined otherwise.

  @retval EFI_SUCCESS           The requested number of bytes was allocated.
  @retval EFI_OUT_OF_RESOURCES  The pool requested could not be allocated.
  @retval EFI_INVALID_PARAMETER PoolType was invalid.

**/
EFI_STATUS
AllocateL1DirElogDir (
  OUT VOID                         **L1Dir,
  OUT UINT64                        *L1Size,
  OUT VOID                         **ElogDir,
  OUT UINT64                        *ElogSize
  )
{
  //UseAllocatepages, below 4GB, Memory reserved
  UINTN Pages = 0;
  EFI_STATUS Status;
  EFI_PHYSICAL_ADDRESS MaxAddress;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "AllocateL1DirElogDir++\n"));

  if (L1Dir == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "ERROR: L1Dir is NULL\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (L1Size == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "ERROR: L1Size is NULL\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (ElogDir == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "ERROR: ElogDir is NULL\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (ElogSize == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "ERROR: ElogSize is NULL\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  *L1Size = GetL1DirSize ();
  Pages = (UINTN) EFI_SIZE_TO_PAGES (*L1Size);
  MaxAddress = 0xffffffff; //less or equal address for the space allocated. It must be under 4GB

  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiReservedMemoryType,
                  Pages,
                  &MaxAddress
                  );

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "AllocatePages for L1Dir, L1Size: %lX, Pages: %lX, %r\n", *L1Size, Pages, Status));

  if (!EFI_ERROR (Status)) {
    *L1Dir = (VOID *) (UINTN) MaxAddress;
    ZeroMem (*L1Dir,EFI_PAGES_TO_SIZE (Pages));
  } else {
    return EFI_OUT_OF_RESOURCES;
  }

  *ElogSize = GetElogDirSize ();
  Pages = (UINTN) EFI_SIZE_TO_PAGES (*ElogSize);
  MaxAddress = 0xffffffff; //less or equal address for the space allocated. It must be under 4GB

  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiReservedMemoryType,
                  Pages,
                  &MaxAddress
                  );

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "AllocatePages for ElogDir, ElogSize: %lX, Pages: %lX, %r\n", *ElogSize, Pages, Status));

  if (!EFI_ERROR (Status)) {
    *ElogDir = (VOID *) (UINTN) MaxAddress;
    ZeroMem (*ElogDir,EFI_PAGES_TO_SIZE (Pages));
  } else {
    gBS->FreePages ((EFI_PHYSICAL_ADDRESS) (UINTN)*L1Dir, (UINTN) EFI_SIZE_TO_PAGES (*L1Size));
    return EFI_OUT_OF_RESOURCES;
  }

  return Status;
}



/**
  Initialize directories

  a.  Allocate and zero out memory
  b.  Setup L1 directory  header  and clear all the L1 directory entries
  c.  Setup valid L1 entries
  d.  Bring all the threads to SMM and update Enhanced_MCA_LOG(63)

  @param  L1Dir                 Pointer to Pointer to save the L1 Directory Memory Location
  @param  ElogDir               Pointer to Pointer to save Elog(L2) Directory Memory Location ;
  undefined otherwise.

  @retval EFI_SUCCESS
  @retval EFI_INVALID_PARAMETER

**/
EFI_STATUS
EFIAPI
InitLogDirs (
  IN OUT VOID                         **L1DirHdr,
  IN OUT VOID                         **ElogDirBase
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT64 L1DirSize;
  UINT64 ElogSize;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "InitLogDirs++\n"));

  GetCoreAndSmtBits (&mThreadBits, &mCoreBits);

  Status = AllocateL1DirElogDir (L1DirHdr,&L1DirSize, ElogDirBase, &ElogSize);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = CreateL1DirHdr ((EMCA_L1_HEADER *)*L1DirHdr,*ElogDirBase,L1DirSize,ElogSize);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = InitL1Dir (*L1DirHdr);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  mL1DirHdrBkp = *L1DirHdr;
  mElogDirBaseBkp = *ElogDirBase;
  mL1DirSizeBkp = L1DirSize;
  mElogDirSizeBkp = ElogSize;

  return Status;
}

/**

    Perform eMCA L1 directory boot time initialization

    @param VOID

    @retval EFI_SUCCESS - The Allocation and Initialization was successful
    @retval EFI_UNSUPPORTED - EMCA Logging is not supported
    @retval EFI_OUT_OF_RESOURCES - The directories could not be allocated.
    @retval EFI_INVALID_PARAMETER - At least one of the pointers is NULL or invalid

**/
EFI_STATUS
InitEMCA (
  IN OUT VOID                         **L1Dir,
  IN OUT VOID                         **ElogDir
  )
{
  BOOLEAN EmcaCap;
  EFI_STATUS Status = EFI_SUCCESS;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "InitEMCA++\n"));

  EmcaCap = (IsEmcaLogCap () || IsEmcaGen2Cap ());

  if (EmcaCap) {
    Status = InitLogDirs(L1Dir, ElogDir);
    ASSERT_EFI_ERROR(Status);
    //Status = RegisterEmcaOSOptinSmi();
    //ASSERT_EFI_ERROR(Status);
  } else {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "EMCA logging is not supported\n"));
    Status = EFI_UNSUPPORTED;
  }

  return Status;
}

/**

    Get the pointer for the L1 Directory Header

    @param Hdr - It is a Pointer to a Pointer where to store the address for L1 Directory if
                         successful. Undefined otherwise.

    @retval EFI_INVALID_PARAMETER - Hdr Pointer is NULL
    @retval EFI_SUCCESS - The Header was located successfully

**/
EFI_STATUS
GetL1DirHdr (
  OUT VOID **Hdr
  )
{

  if (Hdr == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "ERROR: Hdr is NULL\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  *Hdr = mL1DirHdrBkp;
  return EFI_SUCCESS;
}

/**

    Return True if OS has set the opt in flag in the L1 Directory header or False if it is not set

    @param None

    @retval optin - TRUE if optin flag is set, False if cleared

**/
BOOLEAN
IsEmcaOptInFlagSet (
  VOID
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  BOOLEAN    Optin;
  EMCA_L1_HEADER *L1Hdr = NULL;

  Status = GetL1DirHdr (&L1Hdr);
  if (EFI_ERROR (Status)) {
    Optin = FALSE;
  } else {
    Optin = ((L1Hdr->Flags & OS_VMM_OPT_IN_FLAG) != 0);
  }

  return Optin;
}

/**

    It receives the generic error status record and stores it in the appropriate elog entry.

    @param McBankNum - The MC Bank Number which registered the error. It starts from 0 until
                                     MCG_CAP.Count.
    @param McSig     - A pointer to Machine Check Signature Error Record. See Related
                                     definitions and EMCA SAS.
    @param Log       - A pointer to an ACPI Generic Error Status Block completely filled along
                                     with its UEFI error section. This function just stores the log. See
                                     ACPI Specification for details of the record format.

    @retval EFI_INVALID_PARAMETER - At least one of the pointers is invalid or McBank number
                                     points to an unavailable bank.
    @retval EFI_OUT_OF_RESOURCES  - The corresponding Elog Entry pointer for the bank and
                                     apicid is invalid. There is no memory to store the log.
    @retval Status                - The log was successfully stored in the elog directory.

**/
EFI_STATUS
LogEmcaRecord (
  IN UINT32                                 McBankNum,
  IN CONST EMCA_MC_SIGNATURE_ERR_RECORD     *McSig,
  IN CONST EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE             *Log
  )
{
  EFI_STATUS                  Status = EFI_SUCCESS;
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE       *GenericErrorStatusPtrEntry = NULL;
  EMCA_L1_HEADER             *L1Hdr = NULL;
  UINT8                      *L1Base = NULL;
  UINT8                      *RawData = NULL;
  UINT32                     *BlockStatus = NULL;
  UINT64                      L1Offset;
  UINT32                      BlkLen;
  UINT32                      CompareValue;
  UINT32                      ExchangeValue;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%s Entry, McBankNum: %d\n", __FUNCTION__, McBankNum));

    if (Log == NULL) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: Log is NULL\n"));
      RAS_ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
    }

  if (McSig == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: McSig is NULL\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Status = GetL1DirHdr (&L1Hdr);
  ASSERT_EFI_ERROR (Status);

  if (L1Hdr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (McBankNum >= L1Hdr->NumL1EntryLp) {
    return EFI_INVALID_PARAMETER;
  }

  L1Base = (VOID *) (UINTN) EMCA_GET_L1_BASE (L1Hdr);
  L1Offset = EMCA_GET_L1_OFFSET (L1Hdr, McSig->ApicId, McBankNum);
  GenericErrorStatusPtrEntry = (VOID *) (UINTN) EMCA_GET_ELOG_ENTRY (L1Base, L1Offset);

  //Check for Valid bit in pointer and mask 63:52 bits which are not part of the address.
  if (((UINT64) GenericErrorStatusPtrEntry & BIT63) != 0) {
    GenericErrorStatusPtrEntry = (EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE *) (VOID *) (UINTN)
      ((UINT64) GenericErrorStatusPtrEntry & 0x000FFFFFFFFFFFFF);
  } else {
    return EFI_OUT_OF_RESOURCES;
  }

  if (ValidateElogPointer ((VOID *) GenericErrorStatusPtrEntry, \
                             McBankNum, \
                             McSig->ApicId))
{
    return EFI_INVALID_PARAMETER;
  }

  BlkLen = sizeof (EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE) + Log->DataLength;
  BlockStatus = (UINT32*) &(GenericErrorStatusPtrEntry->BlockStatus);

  CompareValue = 0;
  ExchangeValue = *((UINT32*) &(Log->BlockStatus));

  if (InterlockedCompareExchange32 (BlockStatus, CompareValue, ExchangeValue) == 0) {
    //If blockstatus is 0 then the record is either invalid or the OS has consumed it,
    //we can just write the record to memory
    CopyMem (GenericErrorStatusPtrEntry, Log, BlkLen);
    GenericErrorStatusPtrEntry->RawDataOffset = BlkLen;
    GenericErrorStatusPtrEntry->RawDataLength = sizeof (EMCA_MC_SIGNATURE_ERR_RECORD);
    RawData = ((UINT8 *) GenericErrorStatusPtrEntry) + GenericErrorStatusPtrEntry->RawDataOffset;
    CopyMem (RawData, McSig, GenericErrorStatusPtrEntry->RawDataLength);

  } else {
    //There is already a log in memory
    //Use MCA Overwrite Rules to either overwrite the existing record or leave it alone.
    if ((GenericErrorStatusPtrEntry->ErrorSeverity == EFI_GENERIC_ERROR_CORRECTED) ||
        (GenericErrorStatusPtrEntry->ErrorSeverity == EFI_GENERIC_ERROR_RECOVERABLE && \
                 Log->ErrorSeverity == EFI_GENERIC_ERROR_FATAL)
        ) {
      //Overwrite elog
      CopyMem (GenericErrorStatusPtrEntry, Log, BlkLen);
      GenericErrorStatusPtrEntry->RawDataOffset = BlkLen;
      GenericErrorStatusPtrEntry->RawDataLength = sizeof (EMCA_MC_SIGNATURE_ERR_RECORD);
      RawData = ((UINT8 *) GenericErrorStatusPtrEntry) + GenericErrorStatusPtrEntry->RawDataOffset;
      CopyMem (RawData, McSig, GenericErrorStatusPtrEntry->RawDataLength);
    } else {
      if (McSig->Signature.McSts & BIT62) {
        //Over bit is set, update MC Signature
        RawData = ((UINT8 *) GenericErrorStatusPtrEntry) + GenericErrorStatusPtrEntry->RawDataOffset;
        CopyMem (RawData, McSig, GenericErrorStatusPtrEntry->RawDataLength);
      }
    }
  }

  return Status;
}
