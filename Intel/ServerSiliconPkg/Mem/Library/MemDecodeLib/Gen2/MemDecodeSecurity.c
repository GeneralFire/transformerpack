/** @file

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


#include <MemDecode.h>
#include <MemDecodeCommonIncludes.h>

#include <Library/HobLib.h>
#include <Library/KtiApi.h>
#include <Library/MemRcLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/SecurityPolicyLib.h>
#include <Library/PreSiliconEnvDetectLib.h>

#define MIN_PRMRR_SIZE      SIZE_64MB
#define TDX_SEAMRR_SIZE     SIZE_32MB

/**
  Initializes security memory map private data.

  @param[in, out] MemMapData  Point to memory map private data structure.

**/
VOID
InitializeSecurityData (
  IN OUT MEMORY_MAP_DATA *MemMapData
  )
{
  SECURITY_DATA          *SecurityData;
  SECURITY_POLICY        *SecurityPolicy;
  SYSHOST                *Host;
  UINT8                  SocketId;
  UINT8                  SocketWithMem;

  SecurityData = ZeroMem (&MemMapData->SecurityData, sizeof (*SecurityData));
  if (!MemMapData->IsNumaEn) {
    SecurityData->SecurityMemMapHob.Status |= SGX_STATUS_UNSUPPORTED;
  }

  Host = GetSysHostPointer ();
    // SGX: HW not allow to use non-ECC DIMMs
    if (!Host->nvram.mem.eccEn) {
      RcDebugPrint (SDBG_MINMAX, "SGX supports ECC DIMMs only\n");
      SecurityData->SecurityMemMapHob.Status |= SGX_STATUS_UNSUPPORTED;
    }

  SecurityPolicy = GetSecurityPolicy ();
  ASSERT (SecurityPolicy != NULL);

  if ((SecurityPolicy != NULL) && SecurityPolicy->EnableTdx != SECURITY_POLICY_DISABLE) {
    SecurityData->SeamrrSize = TDX_SEAMRR_SIZE;
    RcDebugPrint (SDBG_MINMAX, "TDX enabled, SEAMRR region to allocate: 0x%x\n", SecurityData->SeamrrSize);
  }

  SocketWithMem = 0;
  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    if (GetSocketMemSize (SocketId) > 0) {
      SocketWithMem++;
    }
  }

  //
  // Determine number of PRMRR registers per package
  //
  if (IfFeatureEnabled (TypeSnc) && SocketWithMem > 0 && SocketWithMem <= 4) {
    // 4S+ configurations do not support memory interleaving
    if ((GetNumOfClusterPerSystem () == KTI_SNC4) && (MAX_PRMRR_COUNT / SocketWithMem < 4)) {
      // Downgrade 4 clusters to 2 clusters in 4S case
      SecurityData->SecurityMemMapHob.SgxPrmrrData.PrmrrCountPerPackage = 2;
    } else {
      SecurityData->SecurityMemMapHob.SgxPrmrrData.PrmrrCountPerPackage = GetNumOfClusterPerSystem ();
    }
  } else if (SocketWithMem <= MAX_PRMRR_COUNT) {
    // Single PRMRR range per socket in NUMA
    SecurityData->SecurityMemMapHob.SgxPrmrrData.PrmrrCountPerPackage = 1;
  } else {
    // Zero PRMRRs if more sockets with memory than MAX_PRMRR_COUNT
    SecurityData->SecurityMemMapHob.SgxPrmrrData.PrmrrCountPerPackage = 0;
    SecurityData->SecurityMemMapHob.Status |= SGX_STATUS_UNSUPPORTED;
    RcDebugPrint (SDBG_MAX, "Error: SocketWithMem = %d > MAX_PRMRR_COUNT = %d!\n", SocketWithMem, MAX_PRMRR_COUNT);
  }

  //
  // Determine PRMRR size based on PRM size input
  //
  if (SecurityPolicy != NULL) {
    if ((SecurityPolicy->SprspOrLaterPrmSize & (SecurityPolicy->SprspOrLaterPrmSize - 1)) != 0) {
      SecurityData->SecurityMemMapHob.Status |= SGX_STATUS_INVALID_PARAMETER;
    } else if (SecurityPolicy->EnableSgx != SECURITY_POLICY_DISABLE) {
      if (SecurityData->SecurityMemMapHob.SgxPrmrrData.PrmrrCountPerPackage > 0) {
        SecurityData->PrmrrSize = DivU64x32 (
                                    SecurityPolicy->SprspOrLaterPrmSize,
                                    SecurityData->SecurityMemMapHob.SgxPrmrrData.PrmrrCountPerPackage
                                    );
      } else {
        SecurityData->PrmrrSize = 0;
      }
      SecurityPolicy->PrmrrSize = SecurityData->PrmrrSize;
    }
  }

  RcDebugPrint (SDBG_MAX, "SGX Status %x PrmrrSize %lx\n", SecurityData->SecurityMemMapHob.Status, SecurityData->PrmrrSize);
}

/**
  Calculates valid SGX PRMRR size bitmap on the memory controller.

  This function calculates valid PRMRR bitmap size for the given SAD entry and checks
  PRMRR natural alignment requirement.

  @param[in, out] MemMapData           Point to memory map private data structure.
  @param[in]      SocketId             Socket ID.
  @param[in]      McId                 The memory controller ID.
  @param[in]      InterleaveSize       The interleave size in 64MB unit.
  @param[in]      InterleaveBase       The base address of interleave start.

  @return Valid PRMRR size bitmap for the memory controller for the SAD interleave.

**/
UINT64
GetSgxPrmrrBitmap (
  IN OUT MEMORY_MAP_DATA *MemMapData,
  IN     UINT8           SocketId,
  IN     UINT8           McId,
  IN     UINT32          InterleaveSize,
  IN     UINT32          InterleaveBase
  )
{
  UINT64                 StartAddr;
  UINT64                 LimitAddr;
  UINT64                 SupportedPrmrrSize;
  SECURITY_DATA          *SecurityData;

  SecurityData = &MemMapData->SecurityData;
  StartAddr = LShiftU64 (InterleaveBase, SAD_UNIT);
  LimitAddr = StartAddr + LShiftU64 (InterleaveSize, SAD_UNIT);
  SupportedPrmrrSize = LShiftU64 (1, HighBitSet32 (InterleaveSize) + SAD_UNIT);
  while (SupportedPrmrrSize >= MIN_PRMRR_SIZE) {
    if (ALIGN_VALUE (StartAddr, SupportedPrmrrSize) + SupportedPrmrrSize <= LimitAddr) {
      SecurityData->ValidPrmrrSizeBitmap[SocketId][McId] |= SupportedPrmrrSize;
    }
    SupportedPrmrrSize = RShiftU64 (SupportedPrmrrSize, 1);
  }
  return SecurityData->ValidPrmrrSizeBitmap[SocketId][McId];
}

/**
  Allocate SGX PRMRR region on the memory controller.

  This function allocates the PRMRR region with PRMRR size specified in memory map private data structure.

  @param[in, out] MemMapData           Point to memory map private data structure.
  @param[in]      SocketId             Socket ID.
  @param[in]      McBitMap             Socket MC bitmap.
  @param[in]      InterleaveLimit      The limit address of interleave start.

  @return The allocated SGX PRMRR base address or 0 if PRMRR base is not allocated.

**/
UINT64
AllocateSgxPrmrr (
  IN OUT MEMORY_MAP_DATA *MemMapData,
  IN     UINT8           SocketId,
  IN     UINT8           McBitMap,
  IN     UINT32          InterleaveLimit
  )
{
  SECURITY_DATA              *SecurityData;
  SGX_PRMRR_DATA             *SgxPrmrrData;
  UINT64                     LimitAddr;
  UINT64                     PrmrrBase;

  PrmrrBase = 0;
  SecurityData = &MemMapData->SecurityData;
  SgxPrmrrData = &SecurityData->SecurityMemMapHob.SgxPrmrrData;
  if ((SecurityData->PrmrrCountPerSocket[SocketId] < SgxPrmrrData->PrmrrCountPerPackage) &&
      (SgxPrmrrData->PrmrrCount < ARRAY_SIZE (SgxPrmrrData->PrmrrBase))) {
    //
    // Allocate SGX PRMRR from the interleave limit to align with current ICX implementation.
    //
    LimitAddr = LShiftU64 (InterleaveLimit, SAD_UNIT) & ~(SecurityData->PrmrrSize - 1);
    SgxPrmrrData->McBitMap[SocketId][SgxPrmrrData->PrmrrCount] = McBitMap;
    PrmrrBase = LimitAddr - SecurityData->PrmrrSize;
    SgxPrmrrData->PrmrrBase[SgxPrmrrData->PrmrrCount++] = PrmrrBase;
    SecurityData->PrmrrCountPerSocket[SocketId]++;
  }

  return PrmrrBase;
}


/**
  Allocate TDX SEAMRR region.

  This function allocates the TDX SEAMRR region with SEAMRR size specified in memory map private data structure.

  @param[in, out] MemMapData           Point to memory map private data structure.
  @param[in]      InterleaveBase       The base address of SAD interleave.
  @param[in]      InterleaveLimit      The limit address of SAD interleave.
  @param[in]      PrmrrBase            PRMRR base address allocated in the SAD interleave.

**/
VOID
AllocateTdxSeamrr (
  IN OUT MEMORY_MAP_DATA *MemMapData,
  IN     UINT32          InterleaveBase,
  IN     UINT32          InterleaveLimit,
  IN     UINT64          PrmrrBase
  )
{
  SECURITY_DATA              *SecurityData;
  TDX_SEAMRR_DATA            *TdxSeamrrData;
  UINT64                     BaseAddr;
  UINT64                     LimitAddr;

  SecurityData = &MemMapData->SecurityData;
  if (SecurityData->SeamrrSize == 0) {
    return; // TDX has not been enabled in setup knob.
  }
  TdxSeamrrData = &SecurityData->SecurityMemMapHob.TdxSeamrrData;
  if (TdxSeamrrData->SeamrrBase != 0) {
    return; // TDX SEAMRR has already been allocated.
  }

  BaseAddr = LShiftU64 (InterleaveBase, SAD_UNIT);
  LimitAddr = LShiftU64 (InterleaveLimit, SAD_UNIT);

  if (PrmrrBase == 0) {
    //
    // SGX PRMRR is not allocated in the SAD, allocate TDX at the staring address of SAD entry.
    //
    if (LimitAddr - BaseAddr >= SecurityData->SeamrrSize) {
      TdxSeamrrData->SeamrrBase = BaseAddr;
    }
  } else {
    if (PrmrrBase - BaseAddr >= SecurityData->SeamrrSize) {
      //
      // First attempt to allocate TDX SEAMRR right before PRMRR region.
      //
      TdxSeamrrData->SeamrrBase = PrmrrBase - SecurityData->SeamrrSize;
    } else if (PrmrrBase + SecurityData->PrmrrSize + SecurityData->SeamrrSize <= LimitAddr) {
      //
      // Second attempt to allocate TDX SEAMRR right after PRMRR region.
      //
      TdxSeamrrData->SeamrrBase = PrmrrBase + SecurityData->PrmrrSize;
    }
  }
}

/**
  Handles security request during SAD interleave algorithm.

  This function calculates valid PRMRR bitmap size for the given SAD interleave.
  If PrmrrSize in private DS is 0, it just calculates all possible PRMRR size with 64MB as minimal size.
  If PrmrrSize in private DS is not 0, it allocates the PRMRR region with that size.
  If this function does actual allocation of PRMRR region for the SAD interleave, InterleaveBase
  field will be adjusted to skip PRMRR region.

  @param[in, out] MemMapData           Point to memory map private data structure.
  @param[in]      SocketId             Socket id.
  @param[in]      MemType              Type of memory that the interleave request corresponds to.
  @param[in]      ImcInterBitmap       Bit map of IMCs participating in the interleave.
  @param[in]      InterleaveSize       The interleave size in 64MB unit.
  @param[in]      InterleaveBase       The base address of interleave start.

**/
VOID
HandleSecurityRequest (
  IN OUT MEMORY_MAP_DATA *MemMapData,
  IN     UINT8           SocketId,
  IN     UINT16          MemType,
  IN     UINT8           ImcInterBitmap,
  IN     UINT32          InterleaveSize,
  IN     UINT32          InterleaveBase
  )
{
  SECURITY_DATA          *SecurityData;
  UINT8                  MaxImc;
  UINT8                  McId;
  UINT64                 ValidPrmrrBitmap;
  UINT64                 PrmrrBase;

  if (MemType != MemType1lmDdr) {
    return;
  }

  SecurityData = &MemMapData->SecurityData;
  if ((SecurityData->SecurityMemMapHob.Status & SGX_STATUS_UNSUPPORTED) != 0) {
    return;
  }

  MaxImc = GetMaxImc ();
  ValidPrmrrBitmap = 0;
  for (McId = 0; McId < MaxImc; McId++) {
    if (((BIT0 << McId) & ImcInterBitmap) == 0) {
      continue;
    }
    if (SecurityData->ValidPrmrrSizeBitmap[SocketId][McId] == 0) {
      //
      // Each memory controller can have at most one PRMRR range.
      //
      ValidPrmrrBitmap |= GetSgxPrmrrBitmap (MemMapData, SocketId, McId, InterleaveSize, InterleaveBase);
    }
  }
  PrmrrBase = 0;
  if ((SecurityData->PrmrrSize & ValidPrmrrBitmap) != 0) {
    PrmrrBase = AllocateSgxPrmrr (MemMapData, SocketId, ImcInterBitmap, InterleaveBase + InterleaveSize);
  }

  AllocateTdxSeamrr (MemMapData, InterleaveBase, InterleaveSize, PrmrrBase);
}

/**
  Collect Memory check data for memory map SGX interface.

  @param[in]  MemMapData       Pointer to memory map private data structure.
  @param[out] McheckData       Pointer to output data to store MCHECK private data structure.

**/
VOID
CollectMcheckData (
  IN  MEMORY_MAP_DATA  *MemMapData,
  OUT SGX_MCHECK_DATA  *McheckData
  )
{
   SYSHOST             *Host;
   UINT8               SocketId;

   Host = GetSysHostPointer ();
   McheckData->VolMemMode = Host->var.mem.volMemMode;
   for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
     if (!SocketPresent (SocketId)) {
       continue;
     }
     McheckData->XTile2lm[SocketId] = MemMapData->MemMapSocket[SocketId].XTile2lm;
   }
}

/**

  This routine performs SGX data after memory map.

  @param[in] MemMapData   Pointer to the memory map private data structure.

**/
VOID
PostMemMapSgxData (
  IN MEMORY_MAP_DATA  *MemMapData
  )
{
  SECURITY_MEMMAP_HOB *SecurityMemMapHob;
  UINT8               SocketId;
  UINT8               McId;
  UINT8               MaxImc;
  UINTN               Index;

  SecurityMemMapHob = BuildGuidDataHob (&gSecurityMemMapHobGuid, &MemMapData->SecurityData.SecurityMemMapHob, sizeof (SECURITY_MEMMAP_HOB));
  if (SecurityMemMapHob == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return;
  }

  if ((SecurityMemMapHob->Status & SGX_STATUS_UNSUPPORTED) == 0) {
    MaxImc = GetMaxImc ();
    for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
      if (!SocketPresent (SocketId)) {
        continue;
      }
      if ((GetSocketXorMode (SocketId) & CHANNEL_XOR) == 0) {
        SecurityMemMapHob->Status |= SGX_STATUS_UNSUPPORTED; // PRMRR region does not support mod 3 interleave.
        break;
      }
      for (McId = 0; McId < MaxImc; McId++) {
        SecurityMemMapHob->SgxPrmrrData.ValidPrmrrBitMap |= MemMapData->SecurityData.ValidPrmrrSizeBitmap[SocketId][McId];
      }
    }

    if (SecurityMemMapHob->SgxPrmrrData.ValidPrmrrBitMap == 0) {
      SecurityMemMapHob->Status |= SGX_STATUS_UNSUPPORTED;
    } else if ((MemMapData->SecurityData.PrmrrSize > 0) && (SecurityMemMapHob->SgxPrmrrData.PrmrrCount == 0)) {
      SecurityMemMapHob->Status |= SGX_STATUS_OUT_OF_RESOURCES;
    }
  }
  RcDebugPrint (SDBG_MAX, "SGX memory map status: %x\n", SecurityMemMapHob->Status);
  RcDebugPrint (SDBG_MAX, "ValidPrmrrBitMap: 0x%lx\n", SecurityMemMapHob->SgxPrmrrData.ValidPrmrrBitMap);
  RcDebugPrint (SDBG_MAX, "PrmrrCountPerPackage: %d\n", SecurityMemMapHob->SgxPrmrrData.PrmrrCountPerPackage);
  for (Index = 0; Index < SecurityMemMapHob->SgxPrmrrData.PrmrrCount; Index++) {
    RcDebugPrint (SDBG_MAX, "PrmrrBase %d: 0x%lx", Index, SecurityMemMapHob->SgxPrmrrData.PrmrrBase[Index]);
    for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
      if (SecurityMemMapHob->SgxPrmrrData.McBitMap[SocketId][Index] != 0) {
        RcDebugPrint (SDBG_MAX, " Socket ID %d, McBitmap 0x%x\n", SocketId, SecurityMemMapHob->SgxPrmrrData.McBitMap[SocketId][Index]);
        break; // One SGX range cannot span across sockets.
      }
    }
  }
  if (MemMapData->SecurityData.SeamrrSize > 0) {
    RcDebugPrint (SDBG_MAX, "SeamrrBase: 0x%lx\n", SecurityMemMapHob->TdxSeamrrData.SeamrrBase);
  }
  CollectMcheckData (MemMapData, &SecurityMemMapHob->SgxMcheckData);
}
