/** @file

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

#ifndef _MEM_DECODE_H_
#define _MEM_DECODE_H_

#include <ReferenceCodeDataTypes.h>
#include <Library/MemoryCoreLib.h>
#include "KtiSetupDefinitions.h"
#include "MaxSocket.h"
#include "SysHost.h"
#include <Guid/IioIpInterface.h>
#include <UncoreCommonIncludes.h>
#include <MemDecodeCommonIncludes.h>
#include <Library/ChaLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/KtiApi.h>
#include <Library/MemRcLib.h>
#include <Library/MemTypeLib.h>
#include <Library/MemDecodeLib.h>
#include <Library/CpuAndRevisionLib.h>

#define MAX_SIZE_INVALID 0xFFFFFFFF

//
// These are the error code that are used to handle Resource unavailability.
// The error codes begin at 0x20 to make sure it doesnt overlap with error codes defined for NGN dimms.
// ERROR_RESOURCE_CALCULATION_CAP_MET indicates that memory has been allocated up
// to the MMIOH base, so any remaining memory will not be mapped
//

#define ERROR_RESOURCE_CALCULATION_COMPLETED           0x020
#define ERROR_RESOURCE_CALCULATION_FAILURE             0x021
#define ERROR_RESOURCE_CALCULATION_CAP_MET             0x022

#define DEFAULT_MEMORY_HOLE_GRANULARITY 0x04  // 256MB

//
// The requested DDR Cache Size is always a power of 2 and is calculated using the formula:
// 2^(33 + DdrCacheSize). This is then converted to 64MB Granularity by right shifting by 26.
//
#define DDR_CACHE_SIZE_NONE         0
#define DDR_CACHE_SIZE_8GB          (BIT0 << 7)
#define DDR_CACHE_SIZE_16GB         (BIT0 << 8)
#define DDR_CACHE_SIZE_32GB         (BIT0 << 9)
#define DDR_CACHE_SIZE_64GB         (BIT0 << 10)
#define DDR_CACHE_SIZE_128GB        (BIT0 << 11)
#define DDR_CACHE_SIZE_256GB        (BIT0 << 12)

#define IS_DDR_2LM_CACHE(Host) ((Host->var.mem.volMemMode == VOL_MEM_MODE_2LM) && (Host->var.mem.CacheMemType == CACHE_TYPE_DDR_CACHE_PMEM))
#define IS_DDR_MIXED_1LM2LM(Host) (Host->var.mem.volMemMode == VOL_MEM_MODE_MIX_1LM2LM)
#define IS_DDR_2LM_OR_MIXED(Host) ((IS_DDR_2LM_CACHE(Host)) || (IS_DDR_MIXED_1LM2LM(Host)))

#define IS_HBM_2LM_CACHE_DDR(Host) (Host->var.mem.CacheMemType == CACHE_TYPE_HBM_CACHE_DDR)
#define IS_SYSTEM_1LM(Host) (Host->var.mem.volMemMode == VOL_MEM_MODE_1LM)
#define IS_SYSTEM_2LM(Host) (Host->var.mem.volMemMode == VOL_MEM_MODE_2LM)

typedef enum _MEMORY_MODE {
  MEMORY_MODE_1LM,
  MEMORY_MODE_2LM,
  MEMORY_MODE_AD,
  MEMORY_MODE_MAX
} MEMORY_MODE;
//
// Function declarations
//

// MemDecode.c

/**

  Get the requested DDR Cache Size in units of 64MB

  The requested DDR Cache Size is always a power of 2 and is calculated using the formula:
  2^(33 + DdrCacheSize). This is then converted to 64MB Granularity by right shifting by 26.

  @param[in] CacheSizeCode  Encoded setup value

  @return The requested DDR Cache Size in units of 64MB

**/
UINT32
DecodeDdrCacheSize (
  IN UINT8 CacheSizeCode
  );

/**

  This routine is to get/retrieve SKU limit value per socket in the system from PCU

  @param[in] Host        - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @retval VOID

**/
VOID
InitializeSkuLimits (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData
  );

/**

  This routine is to check whether there SKU limit violation occurred on any socket after SAD rules creation

  @param[in] Host                - Pointer to sysHost
  @param[in] MemMapData          - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval BOOLEAN                - TRUE (SKU Limit violation) FALSE (No SKU Limit violation)

**/
BOOLEAN
CheckSkuLimitViolation (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData
  );

/**

  This routine is to initilize value per socket for SKU limit values

  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval VOID

**/
VOID
ResetSkuLimitsViolation (
  IN MEMORY_MAP_DATA *MemMapData
  );

/**

  Description: This routine increases Total Memory Allocated amount if SKU Limit violation do not occurs.

  @param[in] Host  - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in] MemoryToBeAllocated - size of memory to be allocated
  @param[in] MemType     - Type of memory
  @param[in] McList      - List of Memory controllers populated

  @retval SUCCESS - Total Memory allocated amount is increased
  @retval FAILURE - Sku limit violation occurs

**/
UINTN
AddTotalMemoryAllocated (
  IN  PSYSHOST            Host,
  IN  MEMORY_MAP_DATA     *MemMapData,
  IN  UINT32              MemoryToBeAllocated,
  IN  UINT16              MemType,
  IN  UINT32              McList[MAX_SOCKET*MAX_IMC]
  );

/**

  This routine abstracts the rank target id for rank interleave list.

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket number
  @param[in] Ch          - Channel number
  @param[in] Dimm        - Dimm number
  @param[in] Rank        - Rank number

**/
UINT8
GetRirRankTarget (
  IN SYSHOST            *Host,
  IN UINT8              Socket,
  IN UINT8              Ch,
  IN UINT8              Dimm,
  IN UINT8              Rank
  );

/**

  This routine initializes memory mapping parameters based on setup options and system capability.

  @param[in] Host        - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @retval VOID

**/
VOID
InitMemMapParams (
  IN PSYSHOST         Host,
  IN MEMORY_MAP_DATA  *MemMapData
  );

/**

  This routine checks the values of the Memory address mapping setup options.

  @param[in] Host  - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @retval N/A

**/
VOID
CheckMemAddrMapSetupOptionsValues (
  IN PSYSHOST Host,
  IN MEMORY_MAP_DATA *MemMapData
  );

#ifdef DDRT_SUPPORT
/**

  This routine initializes the DDRT structures and partitions the DDRT DIMMs

  @param[in] host        - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval None

**/
VOID
EvaluateAndPartitionDdrt (
  IN  PSYSHOST        host,
  IN  MEMORY_MAP_DATA *MemMapData
  );

/**

  This routine fills in the memory topology for the given memory controller

  @param[in]      Host        - Pointer to sysHost
  @param[in]      Socket      - Socket # of the memory controller
  @param[in]      Imc         - Imc # of the memory controller
  @param[in,out]  *ImcDimmPop - Pointer to the Memory topology structure to populate

  @retval none

**/
VOID
GetImcDdrtTopology (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      UINT8             Imc,
  IN OUT  DDRT_IMC_TOPOLOGY *ImcDimmPop
  );
#endif // DDRT_SUPPORT

/**

  This routine initializes the memory size fields in the structures
  for DIMMs, Channels, and Nodes.  The sizes are calculated from the sizes in
  the rank structures.

  @param[in] Host        - Pointer to sysHost
  @param[in] MemMapData  - Pointer to memory map private data structure.

  @retval VOID

**/
VOID
PopulateMemorySizeFields (
  IN  PSYSHOST  Host,
  IN  MEMORY_MAP_DATA *MemMapData
  );

#ifdef DDRT_SUPPORT
/** Parition DDRT DIMM

  This routine checks if partitioning is requested form BIOS knobs and partitions the dimm as per the requested ratio.

  @param[in] Host        - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in] sckt        - Socket number
  @param[in] ch          - Channel number
  @param[in] dimm        - Dimm number

  @retval SUCCESS - DDRT DIMM partitioned successfully.
  @retval FAILURE - DDRT DIMM could not be partitioned.

**/
UINT32
PartitionDdrt (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           sckt,
  IN UINT8           ch,
  IN UINT8           dimm
  );

/**

  This routine initializes the memory size fields in the structures
  for DIMMs, Channels, and Nodes.  The sizes are calculated from the sizes in
  the rank structures.

  @param[in] Host  - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @retval VOID

**/
VOID
UpdateDdrtFields (
  IN PSYSHOST Host,
  IN MEMORY_MAP_DATA *MemMapData
  );

/**
  This routine checks if the system configuration has capability to run in 2LM mode. If not, it sets the
  volatile memory mode to 1LM

  @param[in] Host        - Pointer to sysHost
  @param[in] MemMapData  - Pointer to memory map private data structure.

**/
VOID
AdjustVolatileMemMode (
  PSYSHOST        Host,
  MEMORY_MAP_DATA *MemMapData
  );

/**

  Init memory size variables for blk window size and/or controil region window size

  @param[in] Host     - Pointer to sysHost

  @retval N/A

**/
UINT8
InitializeDdrtDimmChip (
  IN PSYSHOST Host
  );
#endif // DDRT_SUPPORT

/**
  This routine Creates SAD rules for the memory to be mapped into the system address space.

  @param[in] Host       - Pointer to sysHost
  @param[in] MemMapData - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @retval Status

**/
UINT32
CreateSadRules (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData
  );

/** Interleaves memory on the channel level across sockets.

  This routine figures out the mapping and fills in the structures for the SAD, TAD,
  and SAG tables.  It does not program any registers.

  @param[in] Host       - Pointer to sysHost
  @param[in] MemMapData - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval    Status

**/
UINT32
SocketInterleave (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData
  );

/** Interleaves memory across sockets.

  This routine figures out the mapping and fills in the structures for the SAD tables.  It does not program any registers.

  @param[in]  Host       - Pointer to sysHost
  @param[in]  MemMapData - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in]  MemType    - Type of memory to interleave

  @retval     SUCCESS               -  Interleave completed.
  @retval     FAILURE               -  Interleave could not be supported for input memory type.

**/
UINT32
SadUma (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN MEM_TYPE        MemType
  );

#ifdef DDRT_SUPPORT
/**

  This routine looks for NVM dimms with PMEM partition

  @param Host         - Pointer to sysHost
  @param socketBitMap - BitMap of sockets to process

  @retval SUCCESS

**/
BOOLEAN
IsPmemPossible (
  PSYSHOST  Host,
  UINT8     socketBitMap
  );

/** Get persistent memory type

  This is worker function to get persistent memory types based on setup options.

  @param[in]  Host        - Pointer to sysHost
  @param[in]  MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @return Persistent memory type

**/
MEM_TYPE
GetPerMemType (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData
  );

/**
  Interleave persistent memory on IMC and channel level but not across sockets for NUMA configuration.

  @param[in] Host       - Pointer to sysHost
  @param[in] MemMapData - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in] SocketNum  - Socket number

  @retval Status

**/
UINT32
SocketNonInterleavePerMem (
  IN PSYSHOST         Host,
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT8            SocketNum
  );
#endif // DDRT_SUPPORT

/**

  Interleave volatile (DDR4/HBM) memory on IMC and channel level but not across sockets for NUMA configuration.

  @param[in] Host                - Pointer to sysHost
  @param[in] MemMapData          - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in] socketNum           - Socket number

  @retval status

**/
UINT32
SocketNonInterleaveVolMem (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           socketNum
  );


/** Interleave memory on the MC and channel level but not across sockets for NUMA configuration.

  This routine figures out the mapping and fills in the structures for the SAD and TAD
  tables.  It does not program any registers.

  @param[in]  Host        - Pointer to sysHost
  @param[in]  MemMapData  - Pointer to the memory map private data structure
  @param[in]  socketNum   - Socket number
  @param[in]  MemType     - Type of memory being interleaved
  @param[in]  Cluster     - SNC cluster number

  @retval SUCCESS      -  Memory interleaved.
  @retval FAILURE      -  Interleave could not be supported for input memory type.

**/
UINT32
SadNuma (
  IN PSYSHOST         Host,
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT8            socketNum,
  IN MEM_TYPE         MemType,
  IN UINT8            Cluster
  );

#ifdef DDRT_SUPPORT
/**

  This routine verifies if one way IMC interleaving and CH interleaving
  should be forced for DFX PMEM request

  @param[in]  Host     - Pointer to sysHosts

  @retval TRUE  - One way interleave should be forced for DFX PMEM
          FALSE - Interleaving should not be modified for DFX PMEM

**/
BOOLEAN
ForceOneWayInterleaveForDfxPmem (
  IN PSYSHOST Host
  );

/**

  This routine copies the pmem size into the remSize field of each channel in the sockets specified

  @param[in] Host         - Pointer to sysHost
  @param[in] MemMapData   - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in] socketBitMap - BitMap of sockets to process

  @retval VOID

**/
VOID
UpdateHostStructForPmem (
  IN PSYSHOST         Host,
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT8            SocketBitMap
  );
#endif // DDRT_SUPPORT

/**

  Find the next SAD entry that needs to be filled for this Socket.

  @param[in] Host             Pointer to sysHost.
  @param[in] MemMapData       Pointer to the memory map private data structure.
  @param[in] Socket           Socket Index.
  @param[in] ClusterId        Cluster Id for the multicast DRAM rules access

  @retval Index of Next available SAD entry.

**/
UINT8
FindSadIndex (
  IN PSYSHOST         Host,
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT8            SocketId,
  IN UINT8            ClusterId
  );

/**

  Description: Find the NM population in the specified MC and fill the bitmap accordingly

  @param socket  - Socket Id
  @param mcIndex - mc id

  @retval Bit map of NM population in the given MC

**/
UINT8
FindNmBitMap (
  UINT8 sckt,
  UINT8 mc
  );

#ifdef DDRT_SUPPORT
/**
  Get DDRT channel granularity for all CR memory modes

  @retval Channel Granularity for DDRT

**/
UINT8
GetDdrtChannelGranularity (
  VOID
  );
#endif // DDRT_SUPPORT

/** Determine target granularity for SAD entry.

  Determine the target interleave granularity based on the memory type and user selection.

  @param[in]  Host     - Pointer to sysHost.
  @param[in]  MemType  - Memory type

  @retval   -   Target interleave granularity

**/
UINT8
GetTargetInterleaveGranularity (
  IN PSYSHOST  Host,
  IN MEM_TYPE  MemType
  );

#ifdef DDRT_SUPPORT
/**

  Check that NVDIMM is enabled

  @param Host           - Pointer to sysHost
  @param Socket         - Socket number
  @param Ch             - Channel number
  @param Dimm           - DIMM slot

  @retval TRUE /FALSE based on NVMDIMM status

**/
BOOLEAN
IsNvDimmEnabled (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm
  );

/**

  Check that NVDIMM is unmapped

  @param Socket         - Socket number
  @param Ch             - Channel number
  @param Dimm           - DIMM slot
  @param Reason         - Reason of NVDIMM unmapped state
                          (valid only when NVDIMM unmapped)

  @retval TRUE /FALSE based on NVMDIMM status

**/
BOOLEAN
IsNvDimmUnmapped (
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT8     *Reason
  );

/**

  Check if NVDIMM is remapped for ADx1 configuration

  @param Socket         - Socket number
  @param Ch             - Channel number
  @param Dimm           - DIMM slot

  @retval TRUE /FALSE based on NVMDIMM status

**/
BOOLEAN
IsNvDimmRemapped (
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm
  );

/**

  Check that NVDIMM Control Region should be mapped

  @param Socket         - Socket number
  @param Ch             - Channel number
  @param Dimm           - DIMM slot
  @param Reason         - Reason of NVDIMM unmapped state
                          (valid only when NVDIMM unmapped)

  @retval TRUE /FALSE based on NVMDIMM status

**/
BOOLEAN
IsNvDimmCtrLRegionToBeMapped (
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm
  );
#endif // DDRT_SUPPORT

/**
  Get SKU limit default.

  @return SOC specific default SKU limit.

**/
UINT16
EFIAPI
GetSkuLimitDefault (
  VOID
  );

/**
  Worker funtion to adjust Dpa offset if system is runnig in 1LM + PMEM mode

  @param[in]   Host        - Pointer to the Host structure.
  @param[in]   DpaOffset   - original DpaOffset
  @param[in]   Socket      - Socket Index
  @param[in]   Ch          - Channel Index

**/
UINT32
ComputeChannelOffset (
  IN PSYSHOST         Host,
  IN UINT32           DpaOffset,
  IN UINT8            Socket,
  IN UINT8            Ch
  );

/**

  This degrades the memory mapping level and reruns the memory mapping algorithm.


  @param Host       - Pointer to sysHost
  @param MemMapData - Pointer to memory map private data structure

**/
VOID
DegradeMemoryMapLevel (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData
  );

/**

  This routine Creates TAD rules in each IMC for the memory to be mapped into the system address space.

  @param Host  - Pointer to sysHost

  @retval status

**/
UINT32
CreateTadRules (
  PSYSHOST Host,
  MEMORY_MAP_DATA *MemMapData
  );

/**

  This routine Creates RIRs in each channel for the memory to be mapped into the system address space.

  @param Host       - Pointer to sysHost
  @param MemMapData - Pointer to the memory map private data structure

  @retval status

**/
UINT32
CreateRirRules (
  PSYSHOST        Host,
  MEMORY_MAP_DATA *MemMapData
  );

/**

  This routine provides a hook to perform final adjustments to the internal strructure related to memory map.
  One important function that happens here is, that this is where we set the resourceCalculationFlagDone flag and rerun the memap algorithm.

  @param Host       - Pointer to sysHost
  @param MemMapData - Pointer to memory map private data structure

  @retval status

**/
UINT32
FinalizeMemoryMap (
  PSYSHOST Host,
  MEMORY_MAP_DATA *MemMapData
  );

/**

  This routine provides a hook to perform any request after Sad and Tad are Setup in Memory map.

  @param[in] Host         Pointer to sysHost.
  @param[in] MemMapData   Pointer to the memory map private data structure.

  @retval None

**/
VOID
PostMemoryMapConfig (
  IN PSYSHOST         Host,
  IN MEMORY_MAP_DATA  *MemMapData
  );

#ifdef DDRT_SUPPORT
/**

  This routine checks whether 2LM memory mode request can be supported

  @param[in] Host Pointer to sysHost

  @retval None

**/
VOID
Check2LmRequestCompliance (
  IN PSYSHOST Host
  );

/**

  This routine checks the current memory topology for POR compliance.

  @param[in] Host            Pointer to sysHost.
  @param[in] MemMapData      Pointer to memory map private data structure.

  @retval status

**/
UINT32
CheckDdrtPorCompliance (
  IN  PSYSHOST          Host,
  IN  MEMORY_MAP_DATA   *MemMapData
  );

/**

  This routine checks the current memory topography for POR compliance across the sockets.
  (currently only DCPMM capacity is verified across the sockets)

  @param[in] Host        - Pointer to sysHost

  @retval status

**/
UINT32
CheckDdrtPorComplianceAcrossSockets (
  IN  PSYSHOST  Host
  );
#endif // DDRT_SUPPORT

/**

  Computes a list of ranks sorted by size, largest to smallest.

  @param Host        - Pointer to sysHost
  @param rkSortList  - Rank sort list
  @param sckt      - Socket Id
  @param ch          - Channel number (0-based)

  @retval sortNum - Number of ranks with memory remaining to be allocated
            @retval (sortNum may be 0 indicating all memory allocated)

**/
UINT8
RankSort (
  PSYSHOST Host,
  struct rankSort rkSortList[],
  UINT8 sckt,
  UINT8 ch
  );


/**

  Interleaves memory on the rank level for DDR and NVM memory.

  @param Host       - Pointer to sysHost
  @param MemMapData - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval SUCCESS

**/
UINT32
RankInterleave (
  PSYSHOST        Host,
  MEMORY_MAP_DATA *MemMapData
  );

/**

  Interleaves memory on the rank level. This routine figures out the
   mapping and fills in the structures for the RIR tables for DDR.
  It does not program any registers.

  @param Host       - Pointer to sysHost
  @param MemMapData - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval SUCCESS

**/
UINT32
RankInterleaveDdr (
  PSYSHOST        Host,
  MEMORY_MAP_DATA *MemMapData
  );

#ifdef DDRT_SUPPORT
/**

  Interleaves memory on the rank level. This routine figures out the
  mapping and fills in the structures for the RIR tables for DDRT.
  It does not program any registers.

  @param Host       - Pointer to sysHost
  @param MemMapData - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval SUCCESS

**/
UINT32
RankInterleaveDdrt (
  PSYSHOST        Host,
  MEMORY_MAP_DATA *MemMapData
  );
#endif // DDRT_SUPPORT

/**

  Encode interleave mode granularity

  @param[in] InterleaveMode  DRAM rule target/interleave mode

  @retval Encoded granularity

**/
UINT8
EncodeSadInterleaveModeGranularity (
  IN UINT8 InterleaveMode
  );

/**

  Description: Return interleave granularity supported by the CPU type.

  @param Host - Pointer to sysHost

  @retval SAD/TAD granularity (64MB or 128MB)

**/
UINT16
GetSadTadGranularity (
  PSYSHOST Host
  );

/**

  Description: Encode interleave list for SAD rule

  @param[in]  Host       - Pointer to sysHost
  @param[in]  MemMapData - MemMapData  Pointer to the memory map private data structure
  @param[in]  Sckt       - Socket Id
  @param[in]  SadIndex   - Index of SAD entry to encode
  @param[in]  SadIndex   - Channel interleave that should be used to encode interleave list (NMEM or FMEM)

  @retval Encoded InterleaveList

**/
UINT32
SadEncodeInterleaveList (
  IN PSYSHOST         Host,
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT8            Sckt,
  IN UINT8            SadIndex,
  IN MEM_TYPE         MemType
  );

/**
  Check if 2-way NM$ is supported.

  This function checks if the biased 2-way near memory cache is supported on system.
  The result will be stored in MemMapHost for future consumption.

**/
VOID
CheckTwoWayNmCacheSupported (
  VOID
  );

/**

  Fill in SAD/TAD/RIR CRSs for memory map from config info in Host structure

  @param Host  - Pointer to sysHost

  @retval N/A

**/
VOID
WriteMemoryMapCsrs (
  PSYSHOST Host,
  MEMORY_MAP_DATA *MemMapData
  );

/** Calculate minimum near memory size per socket.

  Based on the 2LM config mask option, calculate minimum per channel NM size in each MC or
  minimum per MC NM size across all MCs in the socket.

  @param[in] Host          - Pointer to sysHost
  @param[in] MemMapData Pointer to the memory map private data structure
  @param[in] Sckt          - Socket Id

  @retval None

**/
UINT32
CalculateMinNmSizeForMask (
  IN  PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           Sckt
  );

/**

  Description: Fill in RIR CRSs for memory map from config info in host
  structure.

  @param[in] Host       - Pointer to sysHost
  @param[in] Sckt       - Socket Id
  @param[in] MemMapData - MemMapData  Pointer to the memory map private data structure

  @retval N/A

**/
VOID
WriteRirCsrs (
  IN PSYSHOST        Host,
  IN UINT8           Sckt,
  IN MEMORY_MAP_DATA *MemMapData
  );

#ifdef DDRT_SUPPORT
/**

  This routine checks SPD bytes 384,385,386,387 for all NVMDIMM dimms in the system.
  If these bytes match in all NVMDIMM dimms, it returns TRUE, FAIL otherwise.

  @param Host  - Pointer to sysHost
  @param sckt  - Pointer to the current socket index
  @param ch    - Pointer to the current ch index
  @param dimm  - Pointer to the current dimm index

  @retval TRUE/FALSE based on the match of the SPD bytes listed above

**/
BOOLEAN
CheckDdrtParitioningSpdBytesDdrt (
  PSYSHOST Host,
  UINT8 *sckt,
  UINT8 *ch,
  UINT8 *dimm
  );
#endif // DDRT_SUPPORT

/** Interleaves normal DDR4 memory (Non NVMem) on the channel level across sockets.

  This routine figures out if there is any normal DDR4 memory present in the system and calls SAD interleave function.
  This function works for cases where normal dimms are mixed with Type 01 NVDIMMs and also when AEP dimms are present.
  This function does not intereleave memory if the only memory presetn in the system is NVMem.

  @param[in]  Host       - Pointer to sysHost
  @param[in]  MemMapData - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in]  MemType    - Type of memory to interleave

  @retval     SUCCESS               -  Interleave completed.
  @retval     FAILURE               -  Interleave could not be supported for input memory type.

**/
UINT32
SocketInterleaveNormalMem (
  IN PSYSHOST         Host,
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT16           MemType
  );

/** Interleaves NVMem (Type01) on the channel level across sockets.

  This routine figures out if there is any NVMem and then calls SAD interleave function.
  If there no Type01 NVMEM in the system this function does nothing.

  @param[in]  Host       - Pointer to sysHost
  @param[in]  MemMapData - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in]  MemType    - Type of memory to interleave

  @retval     SUCCESS               -  Interleave completed.
  @retval     FAILURE               -  Interleave could not be supported for input memory type.

**/
UINT32
SocketInterleaveNVMem (
  IN PSYSHOST         Host,
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT16           MemType
  );

/** Interleaves normal DDR4 memory if present on the channel level but not across sockets.

  This routine figures out the mapping and fills in the structures for the SAD, TAD, tables.
  It does not program any registers.

  @param[in]  Host       - Pointer to sysHost
  @param[in]  MemMapData - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in]  socketNum  - Socket ID
  @param[in]  MemType    - Type of memory to interleave
  @param[in]  Cluster    - Cluster in socket for SNC, or 0 if not SNC.

  @retval     SUCCESS               -  Interleave completed.
  @retval     FAILURE               -  Interleave could not be supported for input memory type.

**/
UINT32
SocketNonInterleaveNormalMem (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           socketNum,
  IN UINT16          MemType,
  IN UINT8           Cluster
  );

/** Interleaves Type01 NVMEM if present on the channel level but not across sockets.

  This routine figures out the mapping and fills in the structures for the SAD, TAD, tables.
  It does not program any registers.

  @param[in]  Host       - Pointer to sysHost
  @param[in]  MemMapData - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in]  SocketNum  - Socket ID
  @param[in]  MemType    - Type of memory to interleave
  @param[in]  Cluster    - Cluster in socket for SNC, or 0 if not SNC.

  @retval     SUCCESS    - Interleave completed
  @retval     FAILURE    - Interleave could not be supported for input memory type

**/
UINT32
SocketNonInterleaveNVMem (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8            SocketNum,
  IN UINT16           MemType,
  IN UINT8            Cluster
  );

/**

  Find minimum NM size per channel for this MC to nearest power of 2.

  @param[in] Host  Pointer to sysHost
  @param[in] Sckt  Socket number
  @param[in] Mc    MC index

  @retval Min NM size per channel for this MC

**/
UINT32
FindMinNmChSizePwrOf2 (
  IN PSYSHOST  Host,
  IN UINT8     Sckt,
  IN UINT8     Mc
  );

/**

  Find minimum NM size per channel  for this MC.

  @param[in] Host  Pointer to sysHost
  @param[in] MemMapData Pointer to the memory map private data structure
  @param[in] Sckt  Socket number
  @param[in] Mc    MC index

  @retval Min NM size per channel for this MC

**/
UINT32
FindMinNmSize (
  IN PSYSHOST  Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8     Sckt,
  IN UINT8     Mc
  );

/**

  Find NM interleave ways.

  @param sckt  - Socket number
  @param mc    - MC index

  @retval NM interleave ways

**/
UINT8
FindNmInterleaveWays (
  UINT8     sckt,
  UINT8     mc
  );

/**

  Encode RIR "ways" for RIR rule CSR.

  @param ways  - ways to encode

  @retval Encoded ways

**/
UINT8
RirEncodeRirWays (
  UINT8 ways
  );

#ifdef DDRT_SUPPORT
/**

  Description: Get the first SAD rule of the specified type that the MC is part of.

  @param[in] Host        - Pointer to sysHost
  @param[in] sckt        - Socket Id
  @param[in] mc          - IMC Id
  @param[in] ch          - Chennal id
  @param[in] Dimm        - Dimm num
  @param[in] nodeLimit   - Limit of SAD Rule related to the request

  @retval Partition offset value for the dimm int he Interleave request corresponding to the SAD rule.

**/
UINT32
GetInterReqOffset (
  IN PSYSHOST Host,
  IN UINT8    sckt,
  IN UINT8    mc,
  IN UINT8    ch,
  IN UINT8    dimm,
  IN UINT32   nodeLimit
  );
/** This routine encodes the interleave granularity.

  Encode interleave granularity based on the memory type and Interleave granularities in cofig. data.

  @param[in] Host             - Pointer to sysHost
  @param[in] MemType          - Memory type
  @param[in] InterleaveSize   - Interleave size

  @retval   -   Encoded interleave granularities based on the memory type.

**/
UINT8
EncodeInterleaveGranularity (
  IN SYSHOST  *Host,
  IN UINT16    MemType,
  IN UINT32    InterleaveSize
  );

/** Add SAD entry for far memory (PMEM/PMEM$) type

  This routine creates a DRAM rule for a specific interleave request and updates the SADTable struct
  in corresponding sockets

  @param[in] Host                  - Pointer to sysHost
  @param[in] MemMapData            - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in] ScktInterleave        - Structure with list of MCs that are part of this interleave
  @param[in] SktWays               - Number of IMCs involved in this interleave
  @param[in] FMChInterleaveBitMap  - Far memory channel Interleave bitmap
  @param[in] InterSizePerCh        - Interleave size for each channel
  @param[in] MemType               - PMEM / PMEM$
  @param[in] MirrorEnable          - Enable/Disable mirroring for this SAD rule
  @param[in] InterleaveSize        - Interleave granularities for this SAD rule
  @param[in] NodeLimit             - Pointer to return the limit of SAD created

  @retval    SUCCESS               - SAD rule added
  @retval    FAILURE               - SAD rules exceed maximum count

**/
UINT32
AddSadRule (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT32          ScktInterleave[MC_MAX_NODE],
  IN UINT8           SktWays,
  IN UINT8           FMChInterleaveBitMap,
  IN UINT8           NMBitmap,
  IN UINT16          InterSizePerCh,
  IN MEM_TYPE        MemType,
  IN UINT8           MirrorEnable,
  IN UINT32          InterleaveSize,
  IN UINT32          *NodeLimit
  );

/**

  This routine is to update CCUR tables after memory mapping to reflect the updated dimm configuration wherever applicable.

  @param[in] Host        - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval
**/
VOID
UpdateCfgCurAfterMemmap (
  IN  PSYSHOST        Host,
  IN  MEMORY_MAP_DATA *MemMapData
  );

/**

  This routine is to update CfgOut tables after memory mapping to update the validation status.
  It copies the contents of CFGIN requests (Partition and Interleave) into CfgOut and updates the
  response status in the cfgOut structure.

  @param[in] Host        - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval
**/
VOID
UpdateCfgOutAfterMemmap (
  IN  PSYSHOST        Host,
  IN  MEMORY_MAP_DATA *MemMapData
  );

/**

  This routine is to update failure condition to all the request we have received from the NVMDIMM management software.
  This routine is called during memory mapping due to silicon resource deficit.
  MEM_MAP_LVL_NORMAL - NORMAL memory mapping
  MEM_MAP_LVL_IGNORE_CFGIN   - Will not honor requests from NVMDIMM management software.
  MEM_MAP_LVL_IGNORE_NEW_DIMMS   - MEM_MAP_LVL_IGNORE_CFGIN + Will not mapping volatile region of new dimms added to the system

  @param Host     - Pointer to sysHost

  @retval SUCCESS
**/
UINT32
UpdateRequestforDegradedMemoryMapping (
  PSYSHOST Host
  );
#endif // DDRT_SUPPORT

/**
  This routine checks if memory mode or app direct mode is enabled on the dimm.

  The routine assumes all dimms have the same Sku values.
  It performs following action if memomry mode or app direct mode sku bits are disabled:
  - Downgrade memory mapping to 1LM if memory mode is disabled in SKU (Bit 0)
  - Downgrade to NON_PER_MEM_MODE if app direct mode is disabled in SKU (Bit 2)

  @param Host - Pointer to sysHost

  @retval
**/
VOID
CheckSkuForSupportedMode (
  IN  SYSHOST  *Host
  );

#ifdef DDRT_SUPPORT
/**
  This routine initializes the memory size fields in the structures
  for all the NGN DIMMs in the system

  @param[in] Host        - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval 0   - SUCCESS
**/
UINT32
InitializeNGNDIMM (
  IN  PSYSHOST        Host,
  IN  MEMORY_MAP_DATA *MemMapData
  );

/**

  This routine goes through bothe CfgCur and CfgIn and validates the current config and config requests.

  @param [in] Host        - Pointer to sysHost
  @param [in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval SUCCESS

**/
UINT32
ValidateNGNDimmData (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData
  );

/**

  This routine propery handles DCPMMs unmapped due to population violation.

  @param[in] Host        - Pointer to sysHost

**/
VOID
HandleUnmappedDcpmms (
  IN SYSHOST *Host
  );

/**

  This routines clears the Interleaverequests related to a specific dimm

  @param Host     - Pointer to sysHost
  @param sckt   - socket number
  @param ch       - Channel number
  @param dimm     -  dimm number

  @retval SUCCESS

**/
UINT32
HandlePartitionFailure (
  PSYSHOST Host,
  UINT8 sckt,
  UINT8 ch,
  UINT8 dimm,
  UINT32 nvmDimmStatusCode
  );

/**

  This routine goes through CfgIn, gets at the paririon requests and applies them to the dimm.

  @param[in] Host  - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval SUCCESS

**/
UINT32
HandlePartitionRequests (
  IN PSYSHOST Host,
  IN MEMORY_MAP_DATA *MemMapData
  );

/**
  This routine goes through each NVMDIMM on the socket and if there are no CFGIn requests, it will apply the
  current config based on CCUR data retreived from the DIMM.

  @param[in, out] Host       - Pointer to sysHost
  @param[in, out] MemMapData - Pointer to the memory map private data structure
  @param[in]      SocketNum  - Socket number

  @retval Status

**/
UINT32
ApplyCcurRecords (
  IN OUT PSYSHOST        Host,
  IN OUT MEMORY_MAP_DATA *MemMapData,
  IN     UINT8           SocketNum
  );

/**
  This routine goes through each NVMDIMM in the socket and if there are CFGIn requests, it will apply the
  interleave requests retrieved from the DIMM

  @param[in, out] Host       - Pointer to sysHost
  @param[in, out] MemMapData - Pointer to the memory map private data structure
  @param[in]      SocketNum  - Socket number

  @retval Status

**/
UINT32
ApplyInterleaveRequests (
  IN OUT PSYSHOST        Host,
  IN OUT MEMORY_MAP_DATA *MemMapData,
  IN     UINT8           SocketNum
  );

/*
  This routine sets the system time for NVM dimms populated. If secure erase is enabled, sanitize the NVM dimms

  @param[in] Host  - Pointer to sysHost

  @retval None
*/
VOID
InitNvmDimmSystemCommands (
  IN PSYSHOST Host
  );
#endif // DDRT_SUPPORT

/**

  This routine is to store/pass MemMapData items(required in later phase) to Host in the system

  @param [IN] Host             - Pointer to sysHost
  @param [IN] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval void

**/
VOID
AddMemMapDatatoHost (
  IN SYSHOST  *Host,
  IN MEMORY_MAP_DATA *MemMapData
  );

/**
  Get memory size in 64MB unit for a specified channel of near memory

  @param[in] Host       Pointers to global Host data structure.
  @param[in] MemMapData Pointer to the memory map private data structure
  @param[in] Sckt       The socket id
  @param[in] Ch         The channel id on socket

  @return the memory size in 64MB unit for the specified channel

**/
UINT32
GetNmChannelMemSize (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Sckt,
  IN UINT8           Ch
  );

/**

  Description: Provide total NM size in 2LM mode per socket

  @param[in] MemMapData The private data structure for memory map.
  @param [IN] socketNum socket id

  @retval TotalMemSize  Total Near Memory size per socket

**/
UINT32
TotalNmSize (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           socketNum
  );

/** Performs CPU specific functions to initialize memory map parameters

  Performs SKX and 10nm CPU specific memory map private data structure initialization besides the common part implemented in InitMemMapParams ().

  @param[in] Host       - Pointers to global Host data structure.
  @param[in] MemMapData - The private data structure for memory map.

  @retval    None

**/
VOID
InitMemMapParamsEx (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData
  );

/** Performs CPU specific functions to check memory address map setup options

  Performs CPU specific setup option check besides the common part implemented in CheckMemAddrMapSetupOptionsValues().

  @param[in] Host       - Pointers to global Host data structure.
  @param[in] MemMapData - The private data structure for memory map.

  @retval    None

**/
VOID
CheckMemAddrMapSetupOptionsValuesEx (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData
  );

/** Program MC decode CSRs for memory map from config info in Host structure.

Read TAD structures filled-in sysHost and program MC decoder CSR's.

@param Host  - Pointer to sysHost
@param Sckt  - Socket number
@param MemMapData - MemMapData Pointer to the memory map private data structure.

@retval N/A

**/
VOID
WriteTadCsrs (
  IN PSYSHOST        Host,
  IN UINT8           Sckt,
  IN MEMORY_MAP_DATA *MemMapData
  );

/**

Write the RIR CSRs

@param Host       - Pointer to sysHost
@param MemMapData - MemMapData Pointer to the memory map private data structure.
@param Sckt       - Socket number
@param Ch         - Channel number (0-based)

@retval N/A

**/
VOID
WriteRirForChannel (
  PSYSHOST        Host,
  MEMORY_MAP_DATA *MemMapData,
  UINT8           Sckt,
  UINT8           Ch
  );

/**

  Program CSRs that are suggested as workarounds.

  @param[in] Host         Pointer to sysHost
  @param[in] MemMapData   Pointer to memory map private data structure
  @param[in] Sckt         Socket index

  @retval N/A

**/
VOID
WriteWorkAroundCsrs (
  IN SYSHOST            *Host,
  IN MEMORY_MAP_DATA    *MemMapData,
  IN UINT8              Sckt
  );

/** Pogram MC features which BIOS need to set differently based on
the system topology/configuration or for performance/power reasons

@param[in] Host          - Pointer to sysHost
@param[in] Sckt          - Socket Id

@retval N/A

**/
VOID
SetSysFeatures0Mc (
  IN  PSYSHOST        Host,
  IN  UINT8           Sckt
  );

/**

  Get the ChannelInterBitmap for the input SAD_TABLE Entry

  @param SadEntry - Pointer to the SAD Table entry to process
  @param McIndex  - The McIndex for the requested ChannelInterBitmap

  @return The Channel Interleave Bitmap for the input SadEntry at the requested McIndex

**/
UINT8
GetChannelInterBitmap (
  SAD_TABLE *SadEntry,
  UINT8     McIndex
  );

#ifdef DDRT_SUPPORT
/**

  Description: When there is a change in goal, perform meminit

  @param [IN] Host  - Pointer to sysHost
  @param [IN] Socket - socket id

  @retval VOID

**/
VOID
MemoryInitDdrProvision (
  IN SYSHOST  *Host,
  IN UINT8    Socket
  );
#endif // DDRT_SUPPORT

/**

Find the interleave ways from the bitmap

@param bitmap : bitmap of the interleave
@retval number of ways interleaved

**/
UINT8
CalculateWaysfromBitmap (
  UINT8 bitmap
  );

/**

  Calculate the TAD Offset value which will ultimately be used to program the
  MC TADCHNILVOFFSET "tad_offset" and "tad_offset_sign" register fields.
  The calculated TAD Offset value may be a negative 2'complement integer value.

  @param Mirrored       - Indicates if Mirroring is enabled
  @param SocketWays     - Socket Interleave ways for the TAD entry
  @param ChannelWays    - The Channel Interleave ways for the corresponding SAD
  @param ChannelMemUsed - The total channel size of each channel participating
                          in the interleave
  @param TadLimit       - Limit of the TAD entry

  @return The TAD Offset

**/
INT32
CalculateTadOffset (
  IN BOOLEAN Mirrored,
  IN UINT8   SocketWays,
  IN UINT8   ChannelWays,
  IN UINT32  ChannelMemUsed,
  IN UINT32  TadLimit
  );

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
  );

/**
  Calculate Cached Range Base Address

  Calculate the cached range base address for the input SAD Index

  @param[in] SadIndex    - The SAD index corresponding to the cached range
  @param[in] BaseAddress - The base address for the input SAD index.

  @return The Cached Range Base Address

**/
UINT32
CalculateCacheRangeLow (
  IN UINT8  SadIndex,
  IN UINT32 BaseAddress
  );

#ifdef DDRT_SUPPORT
/**
  Set the Intel PMem performance knobs as recommended FW Interface Specification(FIS) for all Memory Modes
  @param [IN] Host       - Pointer to sysHost
  @param [IN] Socket     - socket id

  @retval VOID

**/
VOID
HostAccessiblePerfKnob (
  IN SYSHOST          *Host,
  IN UINT8            Socket
  );

/**

  This routine supports any SOC-specific restrictions on DCPMM AppDirect interleaving.

  @param [in] MemMapData - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param [in] Socket     - Socket number
  @param [in] Channel    - Channel number
  @param [in] Dimm       - Dimm number
  @param [in] Interleave - Interleave request id
  @param [in] RecordType - Type of the record validated CFG_CUR/CFG_IN

  @retval N/A

**/
VOID
CountAdx1DcpmmInterleave (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Socket,
  IN UINT8           Channel,
  IN UINT8           Dimm,
  IN UINT8           Interleave,
  IN UINT8           RecordType
  );


/**

  This routine supports any SOC-specific restrictions on DCPMM AppDirect DFX interleaving.

  @param[in] Host         - Pointer to sysHost
  @param[in] MemMapData   - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval N/A

**/
VOID
CountAdx1DcpmmInterleaveDfxSetupOption (
  IN PSYSHOST         Host,
  IN MEMORY_MAP_DATA  *MemMapData
  );

/**

  This function adds SoC-specific Intel PMem performance knobs into the performance knob list.

  @param[in]     Host           Pointer to the sysHost structure.
  @param[in]     SocketId       Socket index.
  @param[in]     ChId           Channel index within socket.
  @param[in]     DimmId         DIMM index within channel.
  @param[out]    SetPerfKnob    The list of performance knobs to be set on PMem module.
  @param[in,out] PerfKnobNum    The number of performance knobs to be set on PMem module.

**/
VOID
AddPmemPerfKnobs(
  IN     SYSHOST             *Host,
  IN     UINT8               SocketId,
  IN     UINT8               ChId,
  IN     UINT8               DimmId,
     OUT PERFORMANCE_KNOB    SetPerfKnob[INTEL_PMEM_PERF_KNOBS_CNT],
  IN OUT UINT8               *PerfKnobNum
  );
#endif // DDRT_SUPPORT

/**

  Adjust the Memory Address Map for any required silicon workarounds.

  @param[in, out]  Host          Pointer to sysHost.
  @param[in, out]  MemMapData    Pointer to the memory map private data structure.

  @retval SUCCESS                The function is executed successfully without any error.
  @retval FAILURE                The function failed to apply a workaround.

**/
UINT32
AdjustMemAddrMapForSiliconWorkaround (
  IN OUT SYSHOST          *Host,
  IN OUT MEMORY_MAP_DATA  *MemMapData
  );

/**
  This routine returns the number of CHA DRAM decoder clusters supported.

  @retval Number of clusters for CHA DRAM CSR's

**/
UINT8
EFIAPI
GetNumOfDramClustersPerSystem (
  VOID
  );

/**

  Calculate the total reserved DDR Cache Size on the system.

  @param[in] Sckt - The socket index to use for calculating the socket DdrCacheSize

  @return The total reserved DDR Cache Size for the input socket index

**/
UINT32
GetSocketDdrCacheSize (
  IN UINT8 Sckt
  );

/**

  Get the requested DDR Cache Size in units of 64MB

  @param VOID

  @return The requested DDR Cache Size in units of 64MB

**/
UINT32
GetRequestedDdrCacheSize (
  VOID
  );

/**
  Test if HBM is enabled.

  @param[in] MemMapData  Pointer to the memory map private data structure.

  @retval TRUE
  @retval FALSE
**/
BOOLEAN
HbmEnabledEx (
  IN  MEMORY_MAP_DATA *MemMapData
  );

/**
  Populate HBM size fields.

  Populate the HBM memory map private data structure with the size of available
  HBM in the system, by reading the output data from HBM IO training.

  @param[in] MemMapData  Pointer to the memory map private data structure.

  @return                Total amount of HBM in system expressed in
                         units of 64MB.
**/
UINT32
PopulateHbmSizeFieldsEx (
  IN  MEMORY_MAP_DATA *MemMapData
  );

/**

  Displays the TAD table data structure in the Host structure.

  @param[in] Socket       Current Socket (0-Based)
  @param[in] MemMapData   Pointer to the memory map private data structure.

  @retval None

**/
VOID
DisplayTadTableEx (
  IN  UINT8           Socket,
  IN MEMORY_MAP_DATA  *MemMapData
  );

/**
  This routine checks SOC specific values of the Memory address mapping setup options.

  @param[in] Host        - Pointer to sysHost

  @retval N/A

**/
VOID
ReInitializeMemAddrMapInterleavingValues (
  IN SYSHOST            *Host
  );


#ifdef DDRT_SUPPORT
/**
  read the digests in fw_digest_csr[0-7] and fw_digest1_csr[0-7]

  @param[in] Host             Pointer to the sysHost Structure.

  @retval MRC_STATUS_SUCCESS  This function is executed successfully.

**/
MRC_STATUS
FfwDigestRead (
  IN PSYSHOST        Host
  );

/**

  Check if 1LM memory has multiple x1 interleaves before mapping AD x1.

  @param[in]  MemMapData    Pointer to the memory map private data structure.
  @param[in]  Sckt          Socket instance

  @retval TRUE  1LM memory has >1 x1 interleaves.
  @retval FALSE System is in UMA or 2LM mode.

**/
BOOLEAN
Check1lmX1IntlvForAdX1 (
  IN  MEMORY_MAP_DATA         *MemMapData,
  IN  UINT8                    Sckt
  );
#endif //DDRT_SUPPORT

/** Get the SAD address base of the requested SAD table entry

  Use the local and remote 10nm SAD tables to calculate the requested SAD entry base address.

  @param[in] Host          - Pointer to sysHost.
  @param[in] MemMapData    - Pointer to memory map private data structure.
  @param[in] SocketNum     - Socket id
  @param[in] CurrSadIndex  - Index of SAD table entry within socket

  @retval Base address of the requested SAD table entry

**/
UINT32
GetSadBase (
  IN  SYSHOST         *Host,
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           SocketNum,
  IN  UINT8           CurrSadIndex
  );

#endif //_MEM_DECODE_H_
