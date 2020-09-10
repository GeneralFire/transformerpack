/** @file
  ACPI Platform Library HMAT

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

//
// Statements that include other files
//
#include "AcpiPlatformLibLocal.h"
#include <Library/MemTypeLib.h>
#include <Protocol/Smbios.h>
#include <IndustryStandard/SmBios.h>

//
// The represented latency/bandwidth in 'System Locality Latency and Bandwidth
// Information Structure' is expressed in multiples of Entry Base Unit.
// Unit of latency is picoseconds and bandwidth is megabytes per second
// Below #defines are be base units and values for calcualting the latency/bandwidth
// For example: read latency of DDRT is 180ns which is 180000 picoseconds.
// This is expressed as DDRT_LATENCY_BASE_UNIT of 1000 and DDRT_READ_LATENCY of 180
//
#define DDRT_LATENCY_BASE_UNIT 1000
#define DDRT_BANDWIDTH_BASE_UNIT 100
#define DDR_LATENCY_BASE_UNIT 1
#define DDR_BANDWIDTH_BASE_UNIT 1

#define DDRT_ACCESS_LATENCY 0
#define DDRT_READ_LATENCY 180
#define DDRT_WRITE_LATENCY 0
#define DDRT_ACCESS_BANDWIDTH 0
#define DDRT_READ_BANDWIDTH 68
#define DDRT_WRITE_BANDWIDTH 23


#define DDR_ACCESS_LATENCY 0
#define DDR_READ_LATENCY 0
#define DDR_WRITE_LATENCY 0
#define DDR_ACCESS_BANDWIDTH 0
#define DDR_READ_BANDWIDTH 0
#define DDR_WRITE_BANDWIDTH 0

extern struct SystemMemoryMapHob    *mSystemMemoryMap;
extern EFI_IIO_UDS_PROTOCOL         *mIioUds;
extern SOCKET_MEMORY_CONFIGURATION  mSocketMemoryConfiguration;
extern CPU_CSR_ACCESS_VAR           *mCpuCsrAccessVarPtr;

typedef enum {
  DDR = 0x00,
  DDRT
} MemoryType;


/**
  Dump HMAT Header

  @param [in]      HdrPtr       Pointer to HMAT Header

  @retval None

**/
VOID
DumpHeader(
  EFI_ACPI_HETEROGENEOUS_MEMORY_ATTRIBUTE_TABLE_HEADER *HdrPtr
  )
{
  DEBUG((EFI_D_INFO, "=========== HMAT header ==========\n"));
  DEBUG((EFI_D_INFO, " Signature:          %.4a\n",  (CHAR8 *)&HdrPtr->Header.Signature));
  DEBUG((EFI_D_INFO, " Length:             %d\n",    HdrPtr->Header.Length));
  DEBUG((EFI_D_INFO, " Revision:           %Xh\n",   HdrPtr->Header.Revision));
  DEBUG((EFI_D_INFO, " Checksum:           N/A - CHECKSUM ADDED LATER\n"));
  DEBUG((EFI_D_INFO, " OemId:              %.6a\n",  HdrPtr->Header.OemId));
  DEBUG((EFI_D_INFO, " OemTableId:         %.8a\n",  (CHAR8 *)&HdrPtr->Header.OemTableId));
  DEBUG((EFI_D_INFO, " OemRevision:        %Xh\n",   HdrPtr->Header.OemRevision));
  DEBUG((EFI_D_INFO, " CreatorId:          %.4a\n",  (CHAR8 *)&HdrPtr->Header.CreatorId));
  DEBUG((EFI_D_INFO, " CreatorRevision:    %Xh\n",   HdrPtr->Header.CreatorRevision));
  DEBUG((EFI_D_INFO, "==================================\n"));
  DEBUG((EFI_D_INFO, "\n"));
}

/**
  Dump MSARS Structure

  @param [in]      MsarsPtr       Pointer to MSARS Structure

  @retval None

**/
VOID
DumpMsars(
  MEMORY_SUBSYSTEM_ADDRESS_RANGE_STRUCTURE *MsarsPtr
  )
{
  DEBUG((EFI_D_INFO, "=========== MSARS Table =============================\n"));
  DEBUG((EFI_D_INFO, " Type:                               %d\n",    MsarsPtr->Type));
  DEBUG((EFI_D_INFO, " Length:                             %d\n",    MsarsPtr->Length));
  DEBUG((EFI_D_INFO, " ProcessorDomainValid:               %d\n",    MsarsPtr->Flags.Bits.ProcessorDomainValid));
  DEBUG((EFI_D_INFO, " MemoryDomainValid:                  %d\n",    MsarsPtr->Flags.Bits.MemoryDomainValid));
  DEBUG((EFI_D_INFO, " ReservationHint:                    %d\n",    MsarsPtr->Flags.Bits.ReservationHint));
  DEBUG((EFI_D_INFO, " ProcessorProximityDomain:           %Xh\n",    MsarsPtr->ProcessorProximityDomain));
  DEBUG((EFI_D_INFO, " MemoryProximityDomain:              %Xh\n",    MsarsPtr->MemoryProximityDomain));
  DEBUG((EFI_D_INFO, " SystemPhysicalAddressRangeBase:     %llXh\n", MsarsPtr->AddrBase));
  DEBUG((EFI_D_INFO, " SystemPhysicalAddressRangeLength:   %llXh\n", MsarsPtr->AddrLength));
  DEBUG((EFI_D_INFO, "=====================================================\n"));
  DEBUG((EFI_D_INFO, "\n"));
}

/**
  Dump HSCIS Structure

  @param [in]      MscisPtr       Pointer to HSCIS Structure

  @retval None

**/
VOID
DumpMscis(
  MEMORY_SIDE_CACHE_INFORMATION_STRUCTURE *MscisPtr
  )
{
  UINT8     SmbiosHandleIndex;
  DEBUG((EFI_D_INFO, "=========== MSCIS Table =============================\n"));
  DEBUG((EFI_D_INFO, " Type:                               %d\n",    MscisPtr->Type));
  DEBUG((EFI_D_INFO, " Length:                             %d\n",    MscisPtr->Length));
  DEBUG((EFI_D_INFO, " MemoryProximityDomain:              %Xh\n",    MscisPtr->MemoryProximityDomain));
  DEBUG((EFI_D_INFO, " MemorySideCacheSize:                %llXh\n", MscisPtr->MemorySideCacheSize));
  DEBUG((EFI_D_INFO, " TotalCacheLevels:                   %d\n",    MscisPtr->CacheAttributes.Bits.TotalCacheLevels));
  DEBUG((EFI_D_INFO, " CacheLevel:                         %d\n",    MscisPtr->CacheAttributes.Bits.CacheLevel));
  DEBUG((EFI_D_INFO, " CacheAssociativity:                 %d\n",    MscisPtr->CacheAttributes.Bits.CacheAssociativity));
  DEBUG((EFI_D_INFO, " WritePolicy:                        %d\n",    MscisPtr->CacheAttributes.Bits.WritePolicy));
  DEBUG((EFI_D_INFO, " CacheLineSize:                      %d\n",    MscisPtr->CacheAttributes.Bits.CacheLineSize));
  DEBUG((EFI_D_INFO, " NumSmbiosHandles:                   %d\n",    MscisPtr->NumSmbiosHandles));
  for (SmbiosHandleIndex = 0; SmbiosHandleIndex < MscisPtr->NumSmbiosHandles; SmbiosHandleIndex++) {
  DEBUG((EFI_D_INFO, " SmbiosHandle[%d]:                    %xh\n",   SmbiosHandleIndex + 1 ,MscisPtr->SmbiosHandles[SmbiosHandleIndex]));
  }
  DEBUG((EFI_D_INFO, "=====================================================\n"));
  DEBUG((EFI_D_INFO, "\n"));
}

/**
  Dump LBIS Structure

  @param [in]      LbisPtr        Pointer to LBIS Structure

  @retval None

**/
VOID
DumpLbis(
  LATENCY_BANDWIDTH_INFO_STRUCTURE *LbisPtr
  )
{
  UINTN Index, Index1;
  UINT32  *TargetProximityDomainList;
  UINT16  *Entry;

  DEBUG((EFI_D_INFO, "=========== MLBIS Table =============================\n"));
  DEBUG((EFI_D_INFO, " Type:                               %d\n",    LbisPtr->Type));
  DEBUG((EFI_D_INFO, " Length:                             %d\n",    LbisPtr->Length));
  DEBUG((EFI_D_INFO, " Flags:                              %d\n",    LbisPtr->Flags));
  DEBUG((EFI_D_INFO, " DataType:                           %d\n",    LbisPtr->DataType));
  DEBUG((EFI_D_INFO, " InitiatorProximityDomainsNumber:    %d\n",    LbisPtr->InitiatorProximityDomainsNumber));
  DEBUG((EFI_D_INFO, " TargetProximityDomainsNumber:       %d\n",    LbisPtr->TargetProximityDomainsNumber));
  DEBUG((EFI_D_INFO, " EntryBaseUnit:                      %lXh\n",  LbisPtr->EntryBaseUnit));
  DEBUG((EFI_D_INFO, " InitiatorProximityDomainList:\n"));
  for (Index = 0; Index < LbisPtr->InitiatorProximityDomainsNumber; Index++) {
    DEBUG((EFI_D_INFO, " %d ",  LbisPtr->InitiatorProximityDomainList[Index]));
  }
  TargetProximityDomainList = (UINT32*)&(LbisPtr->InitiatorProximityDomainList[Index]);
  DEBUG((EFI_D_INFO, "\n"));
  DEBUG((EFI_D_INFO, " TargetProximityDomainList:\n"));
  for (Index = 0; Index < LbisPtr->TargetProximityDomainsNumber; Index++) {
    DEBUG((EFI_D_INFO, " %d ",  TargetProximityDomainList[Index]));
  }
  Entry = (UINT16*)(TargetProximityDomainList + Index);
  DEBUG((EFI_D_INFO, "\n"));
  DEBUG((EFI_D_INFO, "RelativeDistanceEntry:\n"));
  for (Index = 0; Index < LbisPtr->InitiatorProximityDomainsNumber; Index++) {
    for (Index1 = 0; Index1 < LbisPtr->TargetProximityDomainsNumber; Index1++) {
      DEBUG((EFI_D_INFO, " %d ",  *(Entry + (Index * LbisPtr->TargetProximityDomainsNumber) + Index1)));
    }
    DEBUG((EFI_D_INFO, "\n"));
  }
  DEBUG((EFI_D_INFO, "\n"));
  DEBUG((EFI_D_INFO, "=====================================================\n"));
  DEBUG((EFI_D_INFO, "\n"));
}


/**
  Dump HMAT table

  @param [in]      HmatAcpiTable       Pointer to HMAT table.

  @retval None

**/
VOID
DumpHmat (
  EFI_ACPI_HETEROGENEOUS_MEMORY_ATTRIBUTE_TABLE *HmatAcpiTable
  )
{
  UINT32  TotalLength = HmatAcpiTable->HmatHeader.Header.Length;
  UINT8   *Table = (UINT8 *)HmatAcpiTable;
  UINT32  Length;
  UINT16  Type;

  DumpHeader(&HmatAcpiTable->HmatHeader);
  Length = sizeof (EFI_ACPI_HETEROGENEOUS_MEMORY_ATTRIBUTE_TABLE_HEADER);

  Table += Length;
  TotalLength -= Length;

  // Dump tables
  while (TotalLength) {
    Type = ((MEMORY_SUBSYSTEM_ADDRESS_RANGE_STRUCTURE *)Table)->Type;
    // For HBM will need to add LBIS structure
    if (Type == MEMORY_SUBSYSTEM_ADDRESS_RANGE_STRUCTURE_TYPE) {
      DumpMsars((MEMORY_SUBSYSTEM_ADDRESS_RANGE_STRUCTURE *)Table);
    } else if (Type == MEMORY_SIDE_CACHE_INFORMATION_STRUCTURE_TYPE)  {
      DumpMscis((MEMORY_SIDE_CACHE_INFORMATION_STRUCTURE *)Table);
    } else {
      DumpLbis((LATENCY_BANDWIDTH_INFO_STRUCTURE *)Table);
    }

    // Goto next entry
    Length = ((MEMORY_SUBSYSTEM_ADDRESS_RANGE_STRUCTURE *)Table)->Length;
    Table += Length;
    TotalLength -= Length;
  }
}

/**
  Update SMBIOS handles and number of SMBIOS handles which is related to specified NodeId to MEMORY_DOMAIN_LIST_INFO

  SMBIOS Type 17 handles are formed in sequence order. First handle is S0 C0 D0, Second handle is S0 C0 D1 and so on.
  So we loop in this order to find the handles of DDR which is part of the specific 2LM cache memory.

  @param [in]  HmatData    Points to HMAT structure
  @param [in]  NodeId      SMBIOS handles related to this NodeId will be retrieved
  
  @retval None
**/
VOID
UpdateSmbiosHandles(
  IN HMAT_PROXIMITY_DOMAIN_DATA_STRUCTURE  *HmatData,
  IN UINT32  NodeId,
  IN UINT8 Socket,
  IN UINT32 ImcBitMap
  )
{
  EFI_STATUS                          Status;
  STATIC  EFI_SMBIOS_PROTOCOL         *Smbios = NULL;
  SMBIOS_TABLE_TYPE17                 *Type17Record;
  EFI_SMBIOS_TABLE_HEADER             *SmbiosRecord;
  EFI_SMBIOS_HANDLE                   SmbiosHandle;
  EFI_SMBIOS_TYPE                     SmbiosType;
  UINT8                               Loop = 1;
  UINT8                               SlotsPerImc = MAX_DIMM * MAX_MC_CH;
  UINT8                               DimmsPerImc = 0;

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, &Smbios);
  if (EFI_ERROR(Status)) {
    return ;
  }

  SmbiosHandle  = SMBIOS_HANDLE_PI_RESERVED;
  SmbiosType    = 17;

  //
  // Adjust ImcBitMap according to the socket number
  //
  if (Socket != 0) {
    ImcBitMap <<= (Socket * SlotsPerImc);
  }

  if (!PcdGetBool (PcdHalfWidth)) {
    DimmsPerImc = SlotsPerImc;
  } else {
    DimmsPerImc = SlotsPerImc - 1;
  }

  do {
    Status = Smbios->GetNext(Smbios, &SmbiosHandle, &SmbiosType, &SmbiosRecord, NULL);
    if (!EFI_ERROR(Status) && (SmbiosHandle != SMBIOS_HANDLE_PI_RESERVED)) {
      Type17Record = (SMBIOS_TABLE_TYPE17 *)SmbiosRecord;

      //
      // 1) check whether the Memory device of this record is cache capable(ie, 2LM DDR cache) and
      // 2) this memory handle is to the current looping IMC
      //
      if ((Type17Record->TypeDetail.CacheDram) && (ImcBitMap & BIT0)) {
          HmatData->MemoryDomainList[NodeId].SmbiosHandles[HmatData->MemoryDomainList[NodeId].NumSmbiosHandles] = Type17Record->Hdr.Handle ;
          HmatData->MemoryDomainList[NodeId].NumSmbiosHandles++;
      }
    }
    //
    // if loop done for this IMC then move for next
    //
    if (Loop % DimmsPerImc == 0) {
      ImcBitMap >>= 1;
    }
    ++Loop;
  } while (!EFI_ERROR(Status) && (SmbiosHandle != SMBIOS_HANDLE_PI_RESERVED) && (ImcBitMap != 0));
}

/**
  Remove slack by moving this substructure lower which removes unused memory.

  @param [in]  SlackSize       Points to cumulative unused memory size
  @param [in]  Src             Points to memory to move
  @param [in]  SrcLen          Size of memory to move

  @retval None

**/
VOID
RemoveSlack (
  IN     UINTN  *SlackSize,
  IN     VOID   *Src,
  IN     UINTN  SrcLen
  )
{
  VOID *Dst;

  Dst = (UINT8 *)Src - (*SlackSize);
  CopyMem (Dst, Src, SrcLen);
}

/**
  Calculate the Memory Proximity Domain Number and generate its associate list.

  @param [in, out]  HmatData             Pointer to HMAT_PROXIMITY_DOMAIN_DATA_STRUCTURE to be filled

  @retval       None.

**/
VOID
GetMemoryDomains (
  IN OUT  HMAT_PROXIMITY_DOMAIN_DATA_STRUCTURE  *HmatData
)
{
  UINT64  MemorySideCacheSize;
  UINT32  NodeId;
  UINT32  PreviousNodeId = 0;
  UINT32  LastDomainId = 0;
  UINT8   Index;
  UINT8   PrevIndex;
  UINT8   LastIndex = 0;
  UINT8   Mc;
  UINT8   Socket;
  UINT8   McBitmap[EFI_ACPI_HMAT_NUMBER_OF_MEMORY_DOMAINS];
  UINT32  PmemEntry[EFI_ACPI_HMAT_NUMBER_OF_MEMORY_DOMAINS];
  UINT64  MemoryAddressStore[EFI_ACPI_HMAT_NUMBER_OF_MEMORY_DOMAINS];
  UINT64  MemoryAddress;
  UINT8   SkippedEntries = 0;
  UINT8   MaxEnabledImc = 0;
  UINTN   ImcIndex;
  UINT8   MemSocketBitmap = 0;
  UINT8   NoMemSocketBitmap;
  BOOLEAN SkipEntry;
  BOOLEAN UpdateSmbiosHandle = FALSE;

  HmatData->MemoryDomainNumber = 0;

  ZeroMem(McBitmap, sizeof(McBitmap));
  ZeroMem(MemoryAddressStore, sizeof(MemoryAddressStore));

  // Memory Proximity Domain must match the Domain in the SRAT Memory Affinity structure
  for (Index = 0; Index < mSystemMemoryMap->numberEntries; Index++) {
    // Skip any memory region marked reserved
    if (IsMemTypeReserved (mSystemMemoryMap->Element[Index].Type)) {
      continue;
    }

    if (!IsMemTypeVolatile (mSystemMemoryMap->Element[Index].Type)) {
      // Skip all non volatile regions
      PmemEntry[SkippedEntries] = Index;
      SkippedEntries++;
      continue;
    }

    SkipEntry = FALSE;

    MemoryAddress = LShiftU64 (mSystemMemoryMap->Element[Index].BaseAddress, MEM_ADDR_SHFT_VAL);

    // Skip duplicate entries
    if (Index) {
      for (PrevIndex = 0; PrevIndex < Index; PrevIndex++) {
        if (MemoryAddress == MemoryAddressStore[PrevIndex]) {
          SkipEntry = TRUE;
          break;
        }
      }
    }
    if (SkipEntry) {
      continue;
    }

    MemoryAddressStore[Index] = MemoryAddress;
    LastIndex = Index;

    for (ImcIndex = 0, MaxEnabledImc = 0; ImcIndex < MAX_IMC; ImcIndex++) {
      if (mSystemMemoryMap->Socket[mSystemMemoryMap->Element[Index].SocketId].imcEnabled[ImcIndex] != 0) {
        MaxEnabledImc ++;
      }
    }

    // Update bitmap for sockets with memory populated
    MemSocketBitmap |= BIT0 << mSystemMemoryMap->Element[Index].SocketId;

    // Get memory domain (must match SRAT memory domain)
    NodeId = ProximityDomainOf (
                mSystemMemoryMap->Element[Index].SocketId,
                mSystemMemoryMap->Element[Index].Type,
                MaxEnabledImc,
                HmatData->SncEnabled,
                HmatData->SncNumOfCluster,
                mSystemMemoryMap->Element[Index].ImcInterBitmap,
                mSystemMemoryMap->volMemMode,
                LastDomainId
                );
    if (LastDomainId < NodeId) {
      LastDomainId = NodeId;
    }

    if (NodeId >= EFI_ACPI_HMAT_NUMBER_OF_MEMORY_DOMAINS) {
      DEBUG ((EFI_D_ERROR, "[ACPI] (HMAT) ERROR: Invalid Memory Proximity Domain (0x%x)\n", NodeId));
      ASSERT (FALSE);
    }

    // Update map of indexes included in this memory domain
    HmatData->MemoryDomainList[NodeId].MemMapIndexMap |= LShiftU64(BIT0, Index);

    // Update Memory domain count and set domain to valid
    // Prevent counting repeated entries with the same Proximity Domain ID
    if ((Index == 0) || (NodeId != PreviousNodeId)) {
      HmatData->MemoryDomainNumber++;
      HmatData->MemoryDomainList[NodeId].Valid = 1;
      McBitmap[NodeId] = 0;
    }

    if (HmatData->MemoryDomainList[NodeId].Valid) {
      if (mSystemMemoryMap->Element[Index].Type == MemType2lmDdrCacheMemoryMode) {
        // Set up side cache info for 2LM
        HmatData->MemoryDomainList[NodeId].Cacheable = 1;

        // Calculate MemorySideCacheSize
        // MemorySideCacheSize for each MC is the DDR4 memSize
        MemorySideCacheSize = 0;
        UpdateSmbiosHandle = FALSE;

        for(Mc = 0; Mc < MAX_IMC; Mc++) {
          if ((mSystemMemoryMap->Element[Index].ImcInterBitmap & (BIT0 << Mc)) && !(McBitmap[NodeId] & (BIT0 << Mc))) {
            UpdateSmbiosHandle = TRUE;
            if (mSystemMemoryMap->volMemMode == VOL_MEM_MODE_2LM) {
              MemorySideCacheSize += mSystemMemoryMap->Socket[mSystemMemoryMap->Element[Index].SocketId].imc[Mc].MemSize;
            } else{ // VOL_MEM_MODE_MIX_1LM2LM
              MemorySideCacheSize += mSystemMemoryMap->DdrCacheSize[mSystemMemoryMap->Element[Index].SocketId][Mc];
            }
          }
        }

        if(UpdateSmbiosHandle) {
          UpdateSmbiosHandles(HmatData,
                              NodeId,
                              mSystemMemoryMap->Element[Index].SocketId,
                              mSystemMemoryMap->Element[Index].ImcInterBitmap);
        }
        HmatData->MemoryDomainList[NodeId].MemorySideCacheSize += MemorySideCacheSize;
      }
      McBitmap[NodeId] |= mSystemMemoryMap->Element[Index].ImcInterBitmap;
    }

    DEBUG ((EFI_D_INFO, "MemoryDomainList[%x] Valid = %x Cacheable = %x MemorySideCacheSize = %x\n",
                        NodeId,
                        HmatData->MemoryDomainList[NodeId].Valid,
                        HmatData->MemoryDomainList[NodeId].Cacheable,
                        HmatData->MemoryDomainList[NodeId].MemorySideCacheSize));

    PreviousNodeId = NodeId; // Refresh PreviousNodeId
  }

  // Update LastDomainId for sockets with no memory
  NoMemSocketBitmap = mCpuCsrAccessVarPtr->socketPresentBitMap & ~MemSocketBitmap;
  if (NoMemSocketBitmap > MemSocketBitmap) {
    for (Socket = MAX_SOCKET - 1; Socket > 0; Socket--) {
      if ((BIT0 << Socket) & NoMemSocketBitmap) {
        // Set LastDomainId to highest populated socket
        if (HmatData->SncEnabled) {
          LastDomainId = (Socket * HmatData->SncNumOfCluster);
        } else {
          LastDomainId = Socket;
        }
        break;
      }
    }
  }

  // Add skipped entries
  for (Index = 0; Index < SkippedEntries; Index++) {
    SkipEntry = FALSE;
    MemoryAddress = LShiftU64 (mSystemMemoryMap->Element[PmemEntry[Index]].BaseAddress, MEM_ADDR_SHFT_VAL);

    // Skip duplicate entries
    for (PrevIndex = 0; PrevIndex < LastIndex; PrevIndex++) {
      if (MemoryAddress == MemoryAddressStore[PrevIndex]) {
        SkipEntry = TRUE;
        break;
      }
    }
    if (SkipEntry) {
      continue;
    }

    MemoryAddressStore[LastIndex++] = MemoryAddress;

    // Get memory domain (must match SRAT memory domain)
    NodeId = ProximityDomainOf (
                mSystemMemoryMap->Element[PmemEntry[Index]].SocketId,
                mSystemMemoryMap->Element[PmemEntry[Index]].Type,
                MaxEnabledImc,
                HmatData->SncEnabled,
                HmatData->SncNumOfCluster,
                mSystemMemoryMap->Element[PmemEntry[Index]].ImcInterBitmap,
                mSystemMemoryMap->volMemMode,
                LastDomainId
                );
    if (LastDomainId < NodeId) {
      LastDomainId = NodeId;
    }

    if (NodeId >= EFI_ACPI_HMAT_NUMBER_OF_MEMORY_DOMAINS) {
      DEBUG ((EFI_D_ERROR, "[ACPI] (HMAT) ERROR: Invalid Memory Proximity Domain (0x%x)\n", NodeId));
      ASSERT (FALSE);
    }

    // Update map of indexes included in this memory domain
    HmatData->MemoryDomainList[NodeId].MemMapIndexMap |= LShiftU64(BIT0, PmemEntry[Index]);

    // Update Memory domain count and set domain to valid
    // Prevent counting repeated entries with the same Proximity Domain ID
    if ((PmemEntry[Index] == 0) || (NodeId != PreviousNodeId)) {
      HmatData->MemoryDomainNumber++;
      HmatData->MemoryDomainList[NodeId].Valid = 1;
    }

    DEBUG ((EFI_D_INFO, "[ACPI] (HMAT) MemoryDomainList[%x] Valid = %x Cacheable = %x MemorySideCacheSize = %x\n", 
                        NodeId,
                        HmatData->MemoryDomainList[NodeId].Valid,
                        HmatData->MemoryDomainList[NodeId].Cacheable,
                        HmatData->MemoryDomainList[NodeId].MemorySideCacheSize));

    PreviousNodeId = NodeId; // Refresh PreviousNodeId
  }
}

/**
  Calculate the Processor Proximity Domain Number and generate its associate list.

  @param [in, out]  HmatData             Pointer to HMAT_PROXIMITY_DOMAIN_DATA_STRUCTURE to be filled

  @retval       None.

**/
VOID
GetProcessorDomains (
  IN OUT  HMAT_PROXIMITY_DOMAIN_DATA_STRUCTURE  *HmatData
  )
{
  UINT32  NodeId;
  UINT32  PreviousNodeId = 0;
  UINT8   Index;
  INTN    FirstImc;

  HmatData->ProcessorDomainNumber = 0;

  // Processor Proximity Domain must match the Domain in the SRAT APIC or X2APIC Affinity Structure
  for (Index = 0; Index < mSystemMemoryMap->numberEntries; Index++) {
    // Skip any memory region marked reserved
    if (IsMemTypeReserved (mSystemMemoryMap->Element[Index].Type)) {
      continue;
    }

    // Get processor proximity domain
    if (HmatData->SncEnabled) {
      FirstImc = LowBitSet32 (mSystemMemoryMap->Element[Index].ImcInterBitmap);
      if (FirstImc == -1) {
        FirstImc = 0;
      }
      //
      // Find the 1st IMC accornding the interbitmap
      //
      if (MAX_IMC <= HmatData->SncNumOfCluster) {
        NodeId = (mSystemMemoryMap->Element[Index].SocketId * HmatData->SncNumOfCluster) + (UINT32)FirstImc;
      } else {
        NodeId = (mSystemMemoryMap->Element[Index].SocketId * HmatData->SncNumOfCluster) + ((UINT32)FirstImc)/HmatData->SncNumOfCluster;
      }
      DEBUG ((EFI_D_INFO, "%a: SocketId: 0x%x SncNumOfCluster: 0x%x ImcInterBitmap:0x%x and  NodeId:0x%x\n",
          __FUNCTION__, mSystemMemoryMap->Element[Index].SocketId, HmatData->SncNumOfCluster, mSystemMemoryMap->Element[Index].ImcInterBitmap, NodeId)); 
    } else {
      NodeId = mSystemMemoryMap->Element[Index].SocketId;
    }

    if (NodeId >= EFI_ACPI_HMAT_NUMBER_OF_PROCESSOR_DOMAINS) {
      DEBUG ((EFI_D_ERROR, "[ACPI] (HMAT) ERROR: Invalid Processor Proximity Domain (0x%x)\n", NodeId));
      ASSERT (FALSE);
    }

    // Update processor domain count and set domain to valid
    // Prevent counting repeated entries with the same Proximity Domain ID
    if ((Index == 0) || (NodeId != PreviousNodeId)) {
      HmatData->ProcessorDomainNumber++;
      HmatData->ProcessorDomainList[NodeId] = 1; // Domain is valid
    }

    DEBUG ((EFI_D_INFO, "[ACPI] (HMAT) ProcessorDomainList[%x] Valid = %x\n", NodeId, HmatData->ProcessorDomainList[NodeId]));
    PreviousNodeId = NodeId; // Refresh PreviousNodeId
  }
}

/**
  Initialize HMAT Data to be consumed when populating tables.
  This Functions allocates buffer for HMAT Data, so it is caller responsibility to free it.

  @param            None.

  @retval           Pointer to allocated HMAT Data if it was initialized correctly.
  @retval           NULL if not allocated and not initialized correctly.

**/
UINTN *
InitializeHmatData (
  VOID
)
{
  HMAT_PROXIMITY_DOMAIN_DATA_STRUCTURE  *HmatData = NULL;

  HmatData = AllocateZeroPool (sizeof (HMAT_PROXIMITY_DOMAIN_DATA_STRUCTURE));
  if (HmatData == NULL) {
    DEBUG ((EFI_D_ERROR, "[ACPI] (HMAT) ERROR: Could not allocate HmatData structure pointer\n"));
    return (UINTN *) HmatData;
  }

  HmatData->SncEnabled = mIioUds->IioUdsPtr->SystemStatus.OutSncEn;
  HmatData->SncNumOfCluster = mIioUds->IioUdsPtr->SystemStatus.OutNumOfCluster;
  GetProcessorDomains (HmatData);
  GetMemoryDomains (HmatData);

  return (UINTN *)HmatData;
}

/**
  Patch HMAT MSARS substructure.

  @param [in, out]  HmatAcpiTable   Points to HMAT table to be modified
  @param [in, out]  SlackSize       Points to cumulative slack size
  @param [in]       HmatData        HMAT_PROXIMITY_DOMAIN_DATA_STRUCTURE to be consumed

  @retval None

**/
VOID
PatchHmatMsars (
  IN OUT EFI_ACPI_HETEROGENEOUS_MEMORY_ATTRIBUTE_TABLE  *HmatAcpiTable,
  IN OUT UINTN                                          *SlackSize,
  IN     HMAT_PROXIMITY_DOMAIN_DATA_STRUCTURE           *HmatData
  )
{
  UINT32  Index;
  UINT32  PrevIndex;
  UINT32  MemoryNodeId;
  UINT32  ProcessorNodeId;
  UINT32  Count = 0;
  UINT32  UnusedCount;
  UINT8   MemoryNodeValid;
  UINT16  ProcessorNodeValid;
  UINT64  MemoryAddress;
  BOOLEAN SkipEntry;
  INTN    FirstImc;

  DEBUG ((EFI_D_INFO, "[ACPI] (HMAT) Populating Memory Subsystem Address Range Structure\n"));
  for (Index = 0; Index < mSystemMemoryMap->numberEntries; Index++) {
    // Skip any memory region marked reserved
    DEBUG ((EFI_D_INFO, "[ACPI] mSystemMemoryMap->Element[%d].Type = 0x%x\n",Index, mSystemMemoryMap->Element[Index].Type));
    if (IsMemTypeReserved (mSystemMemoryMap->Element[Index].Type)) {
      continue;
    }

    if (Count >= EFI_ACPI_HMAT_MSARS_COUNT) {
      DEBUG ((EFI_D_INFO, "[ACPI] (HMAT) Invalid MSARS entry with Index = 0x%x\n", Count));
      ASSERT (FALSE);
      break;
    }

    SkipEntry = FALSE;

    // Skip duplicate entries
    MemoryAddress = LShiftU64 (mSystemMemoryMap->Element[Index].BaseAddress, MEM_ADDR_SHFT_VAL);
    if (Count) {
      for (PrevIndex = 0; PrevIndex < Count; PrevIndex++) {
        if (MemoryAddress == HmatAcpiTable->Msars[PrevIndex].AddrBase) {
          SkipEntry = TRUE;
          break;
        }
      }
    }
    if (SkipEntry) {
      continue;
    }

    // Find memory domain for this index
    for (MemoryNodeId = 0; MemoryNodeId < EFI_ACPI_HMAT_NUMBER_OF_MEMORY_DOMAINS; MemoryNodeId++) {
      if (HmatData->MemoryDomainList[MemoryNodeId].MemMapIndexMap & LShiftU64(BIT0, Index)) {
        break;
      }
    }

    // Fill in valid field
    if (MemoryNodeId < EFI_ACPI_HMAT_NUMBER_OF_MEMORY_DOMAINS) {
      MemoryNodeValid = HmatData->MemoryDomainList[MemoryNodeId].Valid;
    } else {
      MemoryNodeValid = 0;
    }

    // Get processor domain for this index
    if (HmatData->SncEnabled) {
      FirstImc = LowBitSet32 (mSystemMemoryMap->Element[Index].ImcInterBitmap);
      if (FirstImc == -1) {
        FirstImc = 0;
      }
      //
      // Find the 1st IMC accornding the interbitmap
      //
      if (MAX_IMC <= HmatData->SncNumOfCluster) {
        ProcessorNodeId = (mSystemMemoryMap->Element[Index].SocketId * HmatData->SncNumOfCluster) + (UINT32)FirstImc;
      } else {
        ProcessorNodeId = (mSystemMemoryMap->Element[Index].SocketId * HmatData->SncNumOfCluster) + ((UINT32)FirstImc)/HmatData->SncNumOfCluster;
      }
      DEBUG ((EFI_D_INFO, "%a: SocketId: 0x%x SncNumOfCluster: 0x%x ImcInterBitmap:0x%x and  NodeId:0x%x \n",
         __FUNCTION__, mSystemMemoryMap->Element[Index].SocketId, HmatData->SncNumOfCluster, mSystemMemoryMap->Element[Index].ImcInterBitmap, ProcessorNodeId)); 
    } else {
      ProcessorNodeId = mSystemMemoryMap->Element[Index].SocketId;
    }

    // Fill in valid field
    if (ProcessorNodeId < EFI_ACPI_HMAT_NUMBER_OF_PROCESSOR_DOMAINS) {
      ProcessorNodeValid = HmatData->ProcessorDomainList[ProcessorNodeId];
    } else {
      ProcessorNodeValid = 0;
    }

    //
    // The HOB has base addr in 64 MB chunks
    //
    HmatAcpiTable->Msars[Count].AddrBase  = MemoryAddress;
    HmatAcpiTable->Msars[Count].AddrLength = LShiftU64 (mSystemMemoryMap->Element[Index].ElementSize, MEM_ADDR_SHFT_VAL);

    HmatAcpiTable->Msars[Count].Flags.Value = 0;

    //
    // Fill in Proximity Domains
    //
    HmatAcpiTable->Msars[Count].ProcessorProximityDomain = ProcessorNodeId;
    HmatAcpiTable->Msars[Count].Flags.Bits.ProcessorDomainValid = ProcessorNodeValid;
    HmatAcpiTable->Msars[Count].MemoryProximityDomain = MemoryNodeId;
    HmatAcpiTable->Msars[Count].Flags.Bits.MemoryDomainValid = MemoryNodeValid;

    DEBUG ((EFI_D_INFO, "[ACPI] (HMAT) MSARS[%x] AddrBase = 0x%lx, AddrLength = 0x%lx, MemDomain = %x, ProcDomain = %x\n", 
                          Count,
                          HmatAcpiTable->Msars[Count].AddrBase,
                          HmatAcpiTable->Msars[Count].AddrLength,
                          MemoryNodeId,
                          ProcessorNodeId));
    Count++;
  }

  // Update SlackSize for unused MSARS entries
  UnusedCount = EFI_ACPI_HMAT_MSARS_COUNT - Count;
  *SlackSize += UnusedCount * sizeof(HmatAcpiTable->Msars[0]);
}



/**
  Prints HMAT LATENCY_BANDWIDTH_INFO_STRUCTURE for debug purposes.

  @param [in]       Lbis        Points to HMAT LATENCY_BANDWIDTH_INFO_STRUCTURE to be printed.

  @retval None.

**/
VOID
PrintLbisHmat (
  IN     LATENCY_BANDWIDTH_INFO_STRUCTURE  *Lbis
)
{
  UINT32  Row = 0;
  UINT32  Col = 0;
  UINT32  RowBaseIndex = 0;
  UINT16  *Entry;

  Entry = (UINT16 *) Lbis->RelativeDistanceEntry;
  DEBUG ((DEBUG_INFO, "[ACPI] (HMAT) SLLBIS Data: "));
  DEBUG ((DEBUG_INFO, "InitiatorNumber = %d, TargetNumber = %d. ", Lbis->InitiatorProximityDomainsNumber, Lbis->TargetProximityDomainsNumber));
  DEBUG ((DEBUG_INFO, "Entry Base Unit = %d \n", Lbis->EntryBaseUnit));

  DEBUG ((DEBUG_INFO, "    "));
  for (Col = 0; Col < Lbis->TargetProximityDomainsNumber; Col++) {
    DEBUG ((DEBUG_INFO, "%02d        ", Lbis->TargetProximityDomainList[Col]));
  }
  DEBUG ((DEBUG_INFO, "\n"));
  for (Row = 0; Row < Lbis->InitiatorProximityDomainsNumber; Row++) {
    RowBaseIndex = Row * Lbis->TargetProximityDomainsNumber;
    DEBUG ((DEBUG_INFO, "%02d  ", Lbis->InitiatorProximityDomainList[Row]));
    for (Col = 0; Col < Lbis->TargetProximityDomainsNumber; Col++) {
      DEBUG ((DEBUG_INFO, "%08d  ", Entry[RowBaseIndex + Col]));
    }
    DEBUG ((DEBUG_INFO, "\n"));
  }
}

/**
  Get the Read/Write/Access bandwidth value of the memory type.

  @param [in]       DataType         Type of bandwidth value that is required. (Read/Write/Access)
  @param [in]       Type             Type of memory for which Bandwidth is required. (DDR/DDRT)

  @retval Value of the bandwidth associated with memory type.
  @0xFF   Input parameters are invalid.

**/

UINT16
GetMemoryBandWidth(
  IN     UINT8                                 DataType,
  IN     MemoryType                            Type
)
{
  if (Type == DDRT) {  //MemoryType == DDRT
    switch (DataType) {
      case EFI_ACPI_HMAT_ACCESS_BANDWIDTH :
        return DDRT_ACCESS_BANDWIDTH;
      case EFI_ACPI_HMAT_READ_BANDWIDTH :
        return DDRT_READ_BANDWIDTH;
      case EFI_ACPI_HMAT_WRITE_BANDWIDTH :
        return DDRT_WRITE_BANDWIDTH;
      default:
        return 0xFF;
    }
  } else {
    switch (DataType) {
      case EFI_ACPI_HMAT_ACCESS_BANDWIDTH :
        return DDR_ACCESS_BANDWIDTH;
      case EFI_ACPI_HMAT_READ_BANDWIDTH :
        return DDR_READ_BANDWIDTH;
      case EFI_ACPI_HMAT_WRITE_BANDWIDTH :
        return DDR_WRITE_BANDWIDTH;
      default:
        return 0xFF;
    }
  }
}

/**
  Get the Read/Write/Access Latency value of the memory type.

  @param [in]       DataType         Type of latency value that is required (Read/Write/Access)
  @param [in]       Type             Type of memory for which Latency is required. (DDR/DDRT)

  @retval Value of the Latency associated with memory type.
  @0xFF   Input parameters are invalid.

**/

UINT16
GetMemoryLatency(
  IN     UINT8                                 DataType,
  IN     MemoryType                            Type
)
{
  if (Type == DDRT) {  //MemoryType == DDRT
    switch (DataType) {
      case EFI_ACPI_HMAT_ACCESS_LATENCY :
        return DDRT_ACCESS_LATENCY;
      case EFI_ACPI_HMAT_READ_LATENCY :
        return DDRT_READ_LATENCY;
      case EFI_ACPI_HMAT_WRITE_LATENCY :
        return DDRT_WRITE_LATENCY;
      default:
        return 0xFF;
    }
  } else {
    switch (DataType) {
      case EFI_ACPI_HMAT_ACCESS_LATENCY :
        return DDR_ACCESS_LATENCY;
      case EFI_ACPI_HMAT_READ_LATENCY :
        return DDR_READ_LATENCY;
      case EFI_ACPI_HMAT_WRITE_LATENCY :
        return DDR_WRITE_LATENCY;
      default:
        return 0xFF;
    }
  }
}

/**
  Patch HMAT LATENCY_BANDWIDTH_INFO_STRUCTURE structure.

  @param [in]       Lbis             Points to HMAT LATENCY_BANDWIDTH_INFO_STRUCTURE to be modified
  @param [in, out]  LibsSlackSize    Points to cummulative LIBS slack size
  @param [in]       HmatData         HMAT_PROXIMITY_DOMAIN_DATA_STRUCTURE to be consumed

  @retval None

**/
VOID
PatchBandWidthLatencyLbis (
  IN     LATENCY_BANDWIDTH_INFO_STRUCTURE      **LbisPtr,
  IN OUT UINTN                                 *LibsSlackSize,
  IN     HMAT_PROXIMITY_DOMAIN_DATA_STRUCTURE  *HmatData,
  IN     UINT8                                 DataType,
  IN     MemoryType                            Type
)
{
  UINT32  Row = 0;
  UINT32  Col = 0;
  UINT32  RowBaseIndex = 0;
  UINT32  StructSize = 0;
  LATENCY_BANDWIDTH_INFO_STRUCTURE *Lbis  = *LbisPtr;
  UINT32  *InitiatorProximityDomainList   = (UINT32*)Lbis;
  UINT32  *TargetProximityDomainList      = (UINT32*)Lbis;
  UINT16  *RelativeDistanceEntry          = (UINT16*)Lbis;

  InitiatorProximityDomainList = (UINT32*)&(Lbis->InitiatorProximityDomainList[0]);
  StructSize = (UINT32)((UINT8*)InitiatorProximityDomainList - (UINT8*)Lbis);
  StructSize += (HmatData->ProcessorDomainNumber * (sizeof (UINT32)));
  TargetProximityDomainList = (UINT32*)((UINT8*)TargetProximityDomainList + StructSize);
  StructSize += (HmatData->MemoryDomainNumber * (sizeof (UINT32)));
  RelativeDistanceEntry = (UINT16*)((UINT8*)RelativeDistanceEntry + StructSize);
  StructSize += (HmatData->ProcessorDomainNumber * HmatData->MemoryDomainNumber * (sizeof (UINT16)));

  Lbis->Type = SYSTEM_LOCALITY_LATENCY_BANDWIDTH_INFORMATION_STRUCTURE_TYPE;
  Lbis->Length = StructSize;
  Lbis->Flags = EFI_ACPI_HMAT_MEMORY_HIERACHY_MEMORY;
  Lbis->DataType = DataType;
  if (Type == DDRT) {
    Lbis->EntryBaseUnit = DDRT_BANDWIDTH_BASE_UNIT;
  } else {
    Lbis->EntryBaseUnit = DDR_BANDWIDTH_BASE_UNIT;
  }
  Lbis->InitiatorProximityDomainsNumber = HmatData->ProcessorDomainNumber;
  Lbis->TargetProximityDomainsNumber = HmatData->MemoryDomainNumber;

  for (Row = 0; Row < Lbis->InitiatorProximityDomainsNumber; Row++) {
    *(InitiatorProximityDomainList + Row)  = Row;
  }
  for (Row = 0; Row < Lbis->TargetProximityDomainsNumber; Row++) {
    *(TargetProximityDomainList + Row) = Row;
  }
  for (Row = 0; Row < Lbis->InitiatorProximityDomainsNumber; Row++) {
    RowBaseIndex = Row * Lbis->TargetProximityDomainsNumber;
    for (Col = 0; Col < Lbis->TargetProximityDomainsNumber; Col++) {
      RelativeDistanceEntry[RowBaseIndex + Col] = GetMemoryBandWidth (DataType, Type);
    }
  }

  *LibsSlackSize += StructSize;
  *LbisPtr = (LATENCY_BANDWIDTH_INFO_STRUCTURE *)((UINT8*)Lbis + StructSize);
}

/**
  Patch HMAT LATENCY_BANDWIDTH_INFO_STRUCTURE structure.

  @param [in]       Lbis        Points to HMAT LATENCY_BANDWIDTH_INFO_STRUCTURE to be modified
  @param [in, out]  LibsSlackSize   Points to cummulative LIBS slack size
  @param [in]       HmatData    HMAT_PROXIMITY_DOMAIN_DATA_STRUCTURE to be consumed

  @retval None

**/
VOID
PatchMemoryLatencyLbis (
  IN     LATENCY_BANDWIDTH_INFO_STRUCTURE      **LbisPtr,
  IN OUT UINTN                                 *LibsSlackSize,
  IN     HMAT_PROXIMITY_DOMAIN_DATA_STRUCTURE  *HmatData,
  IN     UINT8                                 DataType,
  IN     MemoryType                            Type
)
{
  UINT32  Row = 0;
  UINT32  Col = 0;
  UINT32  RowBaseIndex = 0;
  UINT32  StructSize = 0;
  LATENCY_BANDWIDTH_INFO_STRUCTURE *Lbis  = *LbisPtr;
  UINT32  *InitiatorProximityDomainList   = (UINT32*)Lbis;
  UINT32  *TargetProximityDomainList      = (UINT32*)Lbis;
  UINT16  *RelativeDistanceEntry          = (UINT16*)Lbis;

  InitiatorProximityDomainList = (UINT32*)&(Lbis->InitiatorProximityDomainList[0]);
  StructSize = (UINT32)((UINT8*)InitiatorProximityDomainList - (UINT8*)Lbis);
  StructSize += (HmatData->ProcessorDomainNumber * (sizeof (UINT32)));
  TargetProximityDomainList = (UINT32*)((UINT8*)TargetProximityDomainList + StructSize);
  StructSize += (HmatData->MemoryDomainNumber * (sizeof (UINT32)));
  RelativeDistanceEntry = (UINT16*)((UINT8*)RelativeDistanceEntry + StructSize);
  StructSize += (HmatData->ProcessorDomainNumber * HmatData->MemoryDomainNumber * (sizeof (UINT16)));

  Lbis->Type = SYSTEM_LOCALITY_LATENCY_BANDWIDTH_INFORMATION_STRUCTURE_TYPE;
  Lbis->Length = StructSize;
  Lbis->Flags = EFI_ACPI_HMAT_MEMORY_HIERACHY_MEMORY;
  Lbis->DataType = DataType;
  if (Type == DDRT) {
    Lbis->EntryBaseUnit = DDRT_LATENCY_BASE_UNIT;
  } else {
    Lbis->EntryBaseUnit = DDR_LATENCY_BASE_UNIT;
  }
  Lbis->InitiatorProximityDomainsNumber = HmatData->ProcessorDomainNumber;
  Lbis->TargetProximityDomainsNumber = HmatData->MemoryDomainNumber;

  for (Row = 0; Row < Lbis->InitiatorProximityDomainsNumber; Row++) {
    *(InitiatorProximityDomainList + Row)  = Row;
  }
  for (Row = 0; Row < Lbis->TargetProximityDomainsNumber; Row++) {
    *(TargetProximityDomainList + Row) = Row;
  }
  for (Row = 0; Row < Lbis->InitiatorProximityDomainsNumber; Row++) {
    RowBaseIndex = Row * Lbis->TargetProximityDomainsNumber;
    for (Col = 0; Col < Lbis->TargetProximityDomainsNumber; Col++) {
      RelativeDistanceEntry[RowBaseIndex + Col] = GetMemoryLatency (DataType, Type);
    }
  }

  *LibsSlackSize += StructSize;
  *LbisPtr = (LATENCY_BANDWIDTH_INFO_STRUCTURE *)((UINT8*)Lbis + StructSize);
}

/**
  Patch HMAT LATENCY_BANDWIDTH_INFO_STRUCTURE structure.

  @param [in]       Lbis        Points to HMAT LATENCY_BANDWIDTH_INFO_STRUCTURE to be modified
  @param [in, out]  SlackSize   Points to cummulative slack size
  @param [in]       HmatData    HMAT_PROXIMITY_DOMAIN_DATA_STRUCTURE to be consumed

  @retval None

**/
VOID
PatchHmatLbis (
  IN     LATENCY_BANDWIDTH_INFO_STRUCTURE      *Lbis,
  IN OUT UINTN                                 *SlackSize,
  IN     HMAT_PROXIMITY_DOMAIN_DATA_STRUCTURE  *HmatData
)
{

  UINTN    LibsStructSize=0;
  LATENCY_BANDWIDTH_INFO_STRUCTURE *OrginalLbis = Lbis;

  DEBUG ((EFI_D_INFO, "[ACPI] (HMAT) Populating System Locality Latency and Bandwidth Information Structure\n"));

  PatchMemoryLatencyLbis(&Lbis,&LibsStructSize,HmatData, EFI_ACPI_HMAT_READ_LATENCY, DDR);
  PatchMemoryLatencyLbis(&Lbis,&LibsStructSize,HmatData, EFI_ACPI_HMAT_WRITE_LATENCY, DDR);
  PatchBandWidthLatencyLbis(&Lbis,&LibsStructSize,HmatData, EFI_ACPI_HMAT_READ_BANDWIDTH, DDR);
  PatchBandWidthLatencyLbis(&Lbis,&LibsStructSize,HmatData, EFI_ACPI_HMAT_WRITE_BANDWIDTH, DDR);
  if (mSystemMemoryMap->volMemMode != VOL_MEM_MODE_1LM) {
    PatchMemoryLatencyLbis(&Lbis,&LibsStructSize,HmatData, EFI_ACPI_HMAT_READ_LATENCY, DDRT);
    PatchMemoryLatencyLbis(&Lbis,&LibsStructSize,HmatData, EFI_ACPI_HMAT_WRITE_LATENCY, DDRT);
    PatchBandWidthLatencyLbis(&Lbis,&LibsStructSize,HmatData, EFI_ACPI_HMAT_READ_BANDWIDTH, DDRT);
    PatchBandWidthLatencyLbis(&Lbis,&LibsStructSize,HmatData, EFI_ACPI_HMAT_WRITE_BANDWIDTH, DDRT);
  }

  RemoveSlack (SlackSize, OrginalLbis, LibsStructSize);
  *SlackSize += EFI_ACPI_HMAT_LBIS_COUNT * sizeof(Lbis[0]) - LibsStructSize;

}
/**
  Get Type 17 Handles for HBM devices used as cache.

  @param [in, out]  SmbiosHandles    Points to SmbiosHandles array to be modified
  @param [in]       NumaNode         Value for NUMA Node whose memory is being cached for the specific HBM devices
  @retval None

**/
VOID
GetHbmCacheHandles (
  IN     UINT16   *SmbiosHandles,
  IN     UINT8    NumaNode
  )
{
  EFI_STATUS                              Status = EFI_SUCCESS;
  SMBIOS_TABLE_TYPE17                     *SmbiosType17Record;
  EFI_SMBIOS_HANDLE                       SmbiosHandle = 0;
  EFI_SMBIOS_PROTOCOL                     *mSmbios;
  EFI_SMBIOS_TYPE                         SmbiosType = EFI_SMBIOS_TYPE_MEMORY_DEVICE;
  EFI_SMBIOS_TABLE_HEADER                 *SmbiosRecord;
  UINT8                                   HbmHandleCount = 0;

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **) &mSmbios);
  if (!(EFI_ERROR(Status))) {
    while ((!(EFI_ERROR(Status))) && (SmbiosHandle != SMBIOS_HANDLE_PI_RESERVED) ){
      Status = mSmbios->GetNext (mSmbios, &SmbiosHandle, &SmbiosType, &SmbiosRecord, NULL);
      SmbiosType17Record = (SMBIOS_TABLE_TYPE17 *) SmbiosRecord;
      if ((SmbiosType17Record->TypeDetail.CacheDram == 1)  && (SmbiosType17Record->DeviceSet == NumaNode) ) {
        SmbiosHandles[HbmHandleCount] = SmbiosType17Record->Hdr.Handle;
        HbmHandleCount++;
      }
    }
  }
}

/**
  Patch HMAT MEMORY_SIDE_CACHE_INFORMATION_STRUCTURE substructure.

  @param [in, out]  Mscis        Points to HMAT MEMORY_SIDE_CACHE_INFORMATION_STRUCTURE to be modified
  @param [in, out]  SlackSize    Points to cummulative slack size
  @param [in]       HmatData     HMAT_PROXIMITY_DOMAIN_DATA_STRUCTURE to be consumed

  @retval None

**/
VOID
PatchHmatMscis (
  IN OUT MEMORY_SIDE_CACHE_INFORMATION_STRUCTURE  *Mscis,
  IN OUT UINTN                                    *SlackSize,
  IN      HMAT_PROXIMITY_DOMAIN_DATA_STRUCTURE    *HmatData
  )
{
  UINT32  Index;
  UINT32  Count = 0;
  UINT32  UnusedCount;
  UINT8   SmbiosHandleIndex;

  DEBUG ((EFI_D_INFO, "[ACPI] (HMAT) Populating Memory Side Cache Information Structure\n"));

  for (Index = 0; Index < EFI_ACPI_HMAT_NUMBER_OF_MEMORY_DOMAINS; Index++) {
    if ((HmatData->MemoryDomainList[Index].Valid == 0) || (HmatData->MemoryDomainList[Index].Cacheable == 0)) {
      continue;
    }

    //
    // Fill in fields for Cached Memory Domains
    //
    Mscis[Count].MemoryProximityDomain = Index;
    Mscis[Count].CacheAttributes.Bits.TotalCacheLevels = EFI_ACPI_HMAT_ONE_LEVEL_CACHE;
    Mscis[Count].CacheAttributes.Bits.CacheLevel = EFI_ACPI_HMAT_ONE_LEVEL_CACHE;
    Mscis[Count].CacheAttributes.Bits.CacheLineSize = DDR4_CACHE_LINE_SIZE;
    Mscis[Count].CacheAttributes.Bits.CacheAssociativity = EFI_ACPI_HMAT_CACHE_ASSOCIATIVITY_DIRECT_MAPPED;
    Mscis[Count].CacheAttributes.Bits.WritePolicy = EFI_ACPI_HMAT_WRITE_POLICY_WB;
    Mscis[Count].MemorySideCacheSize = LShiftU64 (HmatData->MemoryDomainList[Index].MemorySideCacheSize, MEM_ADDR_SHFT_VAL);
    Mscis[Count].NumSmbiosHandles = HmatData->MemoryDomainList[Index].NumSmbiosHandles;
    for(SmbiosHandleIndex = 0 ; SmbiosHandleIndex < HmatData->MemoryDomainList[Index].NumSmbiosHandles; SmbiosHandleIndex++){
      Mscis[Count].SmbiosHandles[SmbiosHandleIndex] = HmatData->MemoryDomainList[Index].SmbiosHandles[SmbiosHandleIndex];
    }

    DEBUG ((EFI_D_INFO, "[ACPI] (HMAT) MSCIS[%x] MemorySideCacheSize = 0x%lx, ProximityDomain = %x\n", Count, Mscis[Count].MemorySideCacheSize, Index));

    Count++;
  }

  // Move MSCIS entries up to replace removed MSARS entries
  if (*SlackSize && Count) {
    RemoveSlack (SlackSize, Mscis, Count * sizeof(Mscis[0]));
  }

  // Update SlackSize for unused MSCIS entries
  UnusedCount = EFI_ACPI_HMAT_MSCIS_COUNT - Count;
  *SlackSize += UnusedCount * sizeof(Mscis[0]);
}

/**
  Update the HMAT table.

  @param [in, out]      HmatAcpiTable       The table to be set.

  @retval EFI SUCCESS   Procedure returned successfully.
**/
EFI_STATUS
PatchHmatAcpiTable (
  IN OUT  EFI_ACPI_COMMON_HEADER  *Table
  )
{
  UINTN                                         SlackSize = 0;
  HMAT_PROXIMITY_DOMAIN_DATA_STRUCTURE          *HmatData = NULL;
  EFI_ACPI_HETEROGENEOUS_MEMORY_ATTRIBUTE_TABLE *HmatAcpiTable;

  if (mSystemMemoryMap == NULL) {

    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  HmatAcpiTable = (EFI_ACPI_HETEROGENEOUS_MEMORY_ATTRIBUTE_TABLE *)Table;

  HmatData = (HMAT_PROXIMITY_DOMAIN_DATA_STRUCTURE *) InitializeHmatData ();
  if (HmatData == NULL) {
    DEBUG ((EFI_D_ERROR, "ACPI (HMAT) HMAT Data could not be initialized... skipping HMAT Patch\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Patch the MSARS, SLLBIS, and MSCIS Structure from HMAT Table.
  //
  PatchHmatMsars (HmatAcpiTable, &SlackSize, HmatData);
  PatchHmatLbis (HmatAcpiTable->Lbis, &SlackSize, HmatData);
  PatchHmatMscis (HmatAcpiTable->MemSideCache, &SlackSize, HmatData);

  if (HmatData != NULL) {
    FreePool (HmatData);
  }

  //
  // Update HMAT table size
  //
  HmatAcpiTable->HmatHeader.Header.Length -= (UINT32)SlackSize;

  // Dump HMAT
  DumpHmat (HmatAcpiTable);

  return EFI_SUCCESS;
}

