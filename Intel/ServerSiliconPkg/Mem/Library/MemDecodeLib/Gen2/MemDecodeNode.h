/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef _MEM_DECODE_NODE_H_
#define _MEM_DECODE_NODE_H_

#include "SysHost.h"
#include <RcRegs.h>
#include <Guid/SocketCommonRcVariable.h>
#include <Library/ReferenceCodePolicyLib.h>
#include <MemDecodeCommonIncludes.h>
#include <MemDecode.h>
#include <Library/HbmCoreLib.h>

//
// 10nm specific defines and structures
//

#define CLUSTER_0 0

// Default Setting for SAD Rule "mirrored" field
#define MIRROR_EN_DEFAULT                 0  // Disable by default

#define FLUSH_HINT_ADDRESS_OFFSET         0x7FFFC00

#define BLOCK_DECODER_SOCKET_TARGET_BITS  3
#define BLOCK_DECODER_MC_ID_BITS          2
#define BLOCK_DECODER_CH_ID_BITS          2
#define BLOCK_DECODER_BITS                (BLOCK_DECODER_SOCKET_TARGET_BITS + BLOCK_DECODER_MC_ID_BITS + \
                                          BLOCK_DECODER_CH_ID_BITS)

#define BLOCK_DECODER_MC_ID_SHIFT         12
#define BLOCK_DECODER_CH_ID_SHIFT         (BLOCK_DECODER_MC_ID_SHIFT + BLOCK_DECODER_MC_ID_BITS)
#define BLOCK_DECODER_SOCKET_TARGET_SHIFT (BLOCK_DECODER_CH_ID_SHIFT + BLOCK_DECODER_CH_ID_BITS)
#define BLOCK_DECODER_LOWER_BITS_MASK     ((BIT0 << BLOCK_DECODER_MC_ID_SHIFT) - 1)

// 16GB (128MB/DCPMM * 1 DCPMM/Channel *  4 Channels/MC * 4 MC/Socket * 8 Sockets)
#define BLOCK_DECODER_SIZE_64MB (2 * 4 * 4 * 8)

#define AD_NONINTERLEAVE_CH_COUNT6        6
#define AD_NONINTERLEAVE_CH_COUNT7        7
#define AD_NONINTERLEAVE_CH_COUNT8        8

/** Get the next volatile memory type to map.

  Get the next volatile memory type to map, based on policy and population.
  The value pointed to by Tracker should be set to zero before the first call
  to get the first volatile memory type to map. If the function's return value
  is zero, then there are no more memory types to map. Otherwise, the return
  value indicates the next volatile memory type to map. The caller should pass
  the returned Tracker value back to this function to get the next volatile
  memory type to map.

  Tracker values other than zero do not have any specified meaning. The caller
  must not set any other value.

  @param[in]      Host        Pointer to the Host structure.
  @param[in]      MemMapData  Pointer to the memory map private data structure.
  @param[in]      Socket      Index of the socket to consider.
  @param[in, out] Tracker     Pointer to tracking data.

  @retval MemType1lmDdr                  Map DDR4 (1LM) next.
  @retval MemType2lmDdrCacheMemoryMode   Map DDR4 caching DDRT (2LM) next.
  @retval MemType2lmDdrWbCacheAppDirect  Map DDR4 write back caching DDRT (2LM) next.
  @retval MemType1lmHbm                  Map flat HBM (1LM) next.
  @retval MemType2lmHbmCacheDdr          Map HBM caching DDR4 (2LM) next.
  @retval MemType2lmHbmCacheMemoryMode   Map HBM caching DDRT (2LM) next.
  @retval MemTypeNone                    No more volatile memory types to map.
**/
MEM_TYPE
GetNextVolMemTypeToMap (
  IN SYSHOST          *Host,
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT8            Socket,
  IN UINTN            *Tracker
  );

#ifdef DDRT_SUPPORT
/** Program block decoder CSRs for memory map from config info in Host structure.

  Read SAD structures filled-in sysHost and program 10nm CHA decoder CSR's.

  @param[in] Sckt          - Socket Id

  @retval None

**/
VOID
WriteBlockDecoderCsrs (
  IN UINT8            Sckt
  );
#endif // DDRT_SUPPORT

/** Create new SAD entries which encompass TOLM/TOHM gap got or non-existant memory if DRAM high address >4GB.

  Create a SAD entry with limit upto the total interleave size and its data can be cloned in other SAD
  rules differentiated by limit.
  If MMIOL gap found, modify only the limit of the SAD entry first created to include memory <= TOLM - 4GB.
  If DRAM high start is > 4GB, create a SAD for non-existant memory.
  If MMIOH gap found, create a SAD rule encomapssing it.
  When split, for succeeding SAD's adjust their SAD limit to include MMIOL/MMIOH hole and NXM before creating
  the last SAD rule dor this interleave.
  If SAD does not split, check for limit <= MMIOH and adjust the limit when >MMIOH.

  @param[in, out] Host                 - Pointer to sysHost.
  @param[in, out] MemMapData           - Pointer to the memory map private data structure.
  @param[in]      SocketNum            - Socket id.
  @param[in]      MemType              - Type of memory that the interleave request corresponds to.
  @param[in]      SocketWays           - Number of sockets participating in the interleave.
  @param[in]      McWays               - Number of IMC's within socket participating in the interleave.
  @param[in]      ChWays               - Number of channels between IMC's within socket participating in the interleave.
  @param[in]      ImcInterBitmap       - Bit map of IMCs participating in the interleave.
  @param[in]      ChannelInterBitmap   - Per IMC Bit map of channels participating in the interleave.
  @param[in]      FmChannelInterBitmap - Per IMC Bit map of channels participating in the interleave.
  @param[in]      SadIndex             - Index of the available SAD table entry to fill data in.
  @param[in, out] InterleaveLimit      - Pointer to total interleave address limit.
  @param[in]      Cluster              - SNC cluster this SAD applies to (ignored if SNC is disabled).
  @param[in]      MirrorEnable         - Enable/Disable mirroring for the interleave's SAD rule

  @retval     SUCCESS               -  Interleave completed.
  @retval     FAILURE               -  Interleave could not be supported for input memory type.

**/
UINT32
AddSadEntries (
  IN OUT SYSHOST         *Host,
  IN OUT MEMORY_MAP_DATA *MemMapData,
  IN     UINT8           SocketNum,
  IN     MEM_TYPE        MemType,
  IN     UINT8           SocketWays,
  IN     UINT8           McWays,
  IN     UINT8           ChWays,
  IN     UINT8           ImcInterBitmap,
  IN     UINT8           NmImcInterBitmap,
  IN     UINT8           ChannelInterBitmap[MAX_IMC],
  IN     UINT8           FmChannelInterBitmap[MAX_IMC],
  IN     UINT8           SadIndex,
  IN OUT UINT32          *InterleaveLimit,
  IN     UINT8           Cluster,
  IN     UINT8           MirrorEnable
  );

/**

  This routine returns the encoded Tadchnilvoffset_*.chn_ways and NM_DRAM_RULE_*.nm_ch_ways
  register field value for a given "SocketWays" interleave of "Type" memory type

  @param[in] MemMapData  - Pointer to the memory map private data structure.
  @param[in] Type        - The Type of memory to encode for
  @param[in] McBitmap    - Bitmap of MCs in the interleave.
  @param[in] ChWays      - The Number of channel ways per MC

  @retval UINT8 The encoded chn_ways register field value.

**/
UINT8
GetEncodedChannelWays (
  IN MEMORY_MAP_DATA *MemMapData,
  IN MEM_TYPE        Type,
  IN UINT8           McBitmap,
  IN UINT8           ChWays
  );

/**

  This routine returns the encoded Tadchnilvoffset_*.target_ways register field value
  for a given "SocketWays" interleave of "Type" memory type

  @param[in] MemMapData  - Pointer to the memory map private data structure
  @param[in] Sckt        - Socket Id
  @param[in] SadEntry    - SadEntry corresponding to the interleave being encoded

  @retval UINT8 The encoded target_ways register field value.

**/
UINT8
GetEncodedTargetWays (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Sckt,
  IN SAD_TABLE       *SadEntry
  );

/** Encode memory type based on the SAD entry type.

  Traverse SAD entries to set the memory type for the DRAM rules according to the following encoding:
       00 - Coherent DRAM
       01 - Memory Mapped CFG
       10 - Low Bandwidth Coherent DRAM
       11 - High Bandwidth Coherent DRAM

  @param[in] SadType  - The type of SAD entry

  @return The corresponding 10nm DRAM attribute.

**/
UINT8
SadEncodeDramAttribute (
  IN UINT16   SadType
  );

/**
  Get the Near Memory Mc Ways and Channel Ways

  @param[in]      NmMcBitmap            The McBitmap for the interleave.
  @param[in]      NmChBitmap            The Near Memory Channel Bitmap for the interleave.
  @param[out]     NmMcWays              Pointer to the variable to update with the Near Memory MC Ways.
  @param[out]     NmChannelWays         Pointer to the variable to update with the Near Memory Channel Ways.
**/
VOID
GetNearMemoryWays (
  UINT8 NmMcBitmap,
  UINT8 NmChBitmap[MAX_IMC],
  UINT8 *NmMcWays,
  UINT8 *NmChannelWays
  );

/**

  Find the next remote SAD entry that needs to be filled for this Socket.

  Traverse remote SAD entries for this socket and return the Index of the next available entry.

  @param[in] MemMapData - Pointer to memory map private data structure.
  @param[in] Sckt       - Socket ID.

  @retval Index of next available Remote SAD entry

**/
UINT8
FindRemoteSadIndex (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Sckt
  );

/** Get the SAD address limit of the last memory mapped range.

  Find the highest limit of local SAD rules and the highest remote mapped limit.
  The greatest of them is the highest mapped address that can be used to calculate
  the interleave limit for mapping next range of memory.

  @param[in] Host       - Pointer to sysHost.
  @param[in] MemMapData - Pointer to memory map private data structure.
  @param[in] SocketNum  - Socket id
  @param[in] SadIndex   - Index of SAD table entry within socket
  @param[in] Cluster    - SNC cluster this SAD applies to (ignored if SNC is disabled).

  @retval Address of the last mapped memory range.

**/
UINT32
GetSocketSadLimit (
  IN  SYSHOST         *Host,
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           SocketNum,
  IN  UINT8           SadIndex,
  IN  UINT8           ClusterId
  );

/**

  Worker function to get the sad limit value.

  @param[in] Host          - Pointer to sysHost
  @param[in] MemmapData    - Pointer to memory map private data structure

  @return SadLimit value

**/
UINT32
GetSadLimit (
  IN  SYSHOST         *Host,
  IN  MEMORY_MAP_DATA *MemMapData
  );

/** Create remote DRAM rules to target remote memory when that memory is NUMA interleaved

  Add a remote SAD entry in all sockets that are not involved in the SAD interleave.

  @param[in, out] Host       - Pointer to sysHost.
  @param[in, out] MemMapData - Pointer to memory map private data structure.
  @param[in]      SocketNum  - Socket ID.
  @param[in]      MemType    - Type of memory.
  @param[in]      ClusterId  - Cluster Id for the multicast access

  @retval SUCCESS      -  Remote DRAM rule added.
  @retval FAILURE      -  Remote DRAM rules exceed maximum count.

**/
UINT32
AddRemoteSadEntry (
  IN  SYSHOST         *Host,
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           SocketNum,
  IN  MEM_TYPE        MemType,
  IN  UINT8           ClusterId
  );

/** Create remote DRAM rules to target FPGA memory

  Add a remote SAD entry in all sockets.

  @param[in, out] Host        - Pointer to sysHost.
  @param[in, out] MemMapData  - Pointer to memory map private data structure.
  @param[in]      SocketNum   - Socket ID.
  @param[in]      MemType     - Type of memory.
  @param[in]      ClusterId   - Cluster Id for the multicast access
  @param[in]	  FpgaMemSize - FPGA memory Size in 64MB blocks

  @retval SUCCESS      -  Remote DRAM rule added.
  @retval FAILURE      -  Remote DRAM rules exceed maximum count.

**/

UINT32
AddRemoteSadEntryForFPGAMemory (
  IN  SYSHOST         *Host,
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           SocketNum,
  IN  MEM_TYPE        MemType,
  IN  UINT8           ClusterId,
  IN UINT32           FpgaMemSize
  );

/**
  Discover CXL memory devices.

  This function scans through all enabled sockets for CXL memory devices and constructs CXL
  memory topology in memory map private data structure.

  @param[in, out] MemMapData  Pointer to memory map private data structure.

**/
VOID
DiscoverCxlMemDevices (
  IN OUT MEMORY_MAP_DATA      *MemMapData
  );

/**
  Process CXL memory devices on the socket.

  This function will map CXL memory for a given socket

  @param[in, out] MemMapData  Pointer to memory map private data structure.
  @param[in]      SocketId    Socket number

  @retval SUCCESS The function executes successfully to map all CXL memory attached to the socket.
  @retval others  Some error happens when mapping CXL memory attached the socket.

**/
UINT32
ProcessCxlMemDevices (
  IN OUT MEMORY_MAP_DATA      *MemMapData,
  IN     UINT8                SocketId
  );

/**
  Finds the interleave list for the SAD entry.

  @param[in] MemMapData       Pointer to the memory map private data structure
  @param[in] SadEntry         The SAD entry to match.

  @return The encoded package list for the input SAD entry.

**/
UINT32
GetCxlMemInterleaveList (
  IN MEMORY_MAP_DATA          *MemMapData,
  IN SAD_TABLE                *SadEntry
  );

/**
  Configure CXL memory devices.

  This function configures CXL memory devices after finalizing memory map.

  @param[in] MemMapData       Pointer to memory map private data structure.

**/
VOID
ConfigureCxlMemDevices (
  IN MEMORY_MAP_DATA          *MemMapData
  );

/**
  Exposes CXL node info via HOB for UEFI memory map and ACPI tables

  This function expose CXL node info into MemMapHost so that we can construct UEFI memory
  map and ACPI tables for OS/EFI driver interaction.

  @param[in] MemMapData       Pointer to memory map private data structure.

**/
VOID
PublishCxlNodeHob (
  IN MEMORY_MAP_DATA          *MemMapData
  );

/**
  TAD Interleave for channel address based patrol scrub.

  This function allocates a TAD entry dedicated to 2LM NM to support
  channel address based patrol scrub.

  @param[in] Host          Pointer to the sysHost structure.
  @param[in] MemMapData    Pointer to memory map private data structure.
  @param[in] Socket        Socket index.

  @retval SUCCESS                               TAD rule is allocated successfully.
  @retval ERROR_RESOURCE_CALCULATION_FAILURE    All TAD rules used before all memory allocated.
**/
UINT32
TadInterleaveForChannelAddressPatrolScrub (
  IN PSYSHOST             Host,
  IN MEMORY_MAP_DATA      *MemMapData,
  IN UINT8                Socket
  );

/**
  Return whether memory decoder must program near memory dram rule for the input SAD ID.
  NM_DRAM_RULE is programmed only for SA based patrol/sparing and is not used for CA based patrol/sparing.

  @param[in]  Host        Pointer to the sysHost structure.
  @param[in]  SocketId    Socket number.
  @param[in]  McIndex     Memory controller index.
  @param[in]  SadIndex    Sad table index.

  @retval TRUE            Need to program near memory dram rule.
  @retval FALSE           No need to program near memory dram rule.

**/
BOOLEAN
IsSystemAddressBasedPatrolSparing (
  IN SYSHOST         *Host,
  IN UINT8           SocketId,
  IN UINT8           McIndex,
  IN UINT8           SadIndex
  );

/**
  Check whether the TAD entry is used for near memory patrol sparing.

  @param[in]  Host        Pointer to the sysHost structure.
  @param[in]  SocketId    Socket number.
  @param[in]  McIndex     Memory controller index.
  @param[in]  TadEntry    The input table table structure.

  @retval TRUE            The input TAD entry is for near memory patrol sparing.
  @retval FALSE           The input TAD entry is not for near memory patrol sparing.

**/
BOOLEAN
IsNmTadForPatrolSparing (
  IN SYSHOST         *Host,
  IN UINT8           SocketId,
  IN UINT8           McIndex,
  IN TAD_TABLE       *TadEntry
  );

/**
  This function checks if fADR is supported on system.

  @param[in] Host       Pointer to sysHost.

  @retval N/A
**/
VOID
CheckFadrSupported (
  IN SYSHOST            *Host
  );

/**
  Get DDR patrol sparing limit by passing all TAD tables correspond to the SAD index.
  Zero is returned if no such TAD table is found.

  @param[in]  Host        Pointer to the sysHost structure.
  @param[in]  SocketId    Socket number.
  @param[in]  McIndex     Memory controller index.
  @param[in]  SadIndex    Sad table index.

  @return The patrol sapring limit or 0 if not such TAD is found.

**/
UINT32
GetDdrPsLimit (
  IN SYSHOST           *Host,
  IN UINT8             SocketId,
  IN UINT8             McIndex,
  IN UINT8             SadIndex
  );

/**
  Find the next TAD table entry that needs to be filled for this Socket/IMC.

  @param[in]  Host       - Pointer to sysHost
  @param[in]  SocketNum  - Socket number
  @param[in]  ImcNum     - Memory controller number

  @retval Index of next available TAD table entry (0xFF if table is full)

**/
UINT8
FindTadIndex (
  IN  SYSHOST *Host,
  IN  UINT8    SocketNum,
  IN  UINT8    ImcNum
  );

/**
  Get the TAD Rule Channel Granularity for the input SadIndex.

  @param[in]  MemMapData   Pointer to the memory map private data structure.
  @param[in]  Sckt         The socket index for the input SadIndex.
  @param[in]  SadType      The type of memory corresponding to the TAD rule.
  @param[out] ChGran       Pointer to a variable where the Channel Granularity will be retuned.

  @retval SUCCESS          The Channel Granularity was successfully calculated.
  @retval FAILURE          The operation failed due to an error.
**/
UINT32
GetTadRuleChannelGranularity (
  IN     MEMORY_MAP_DATA    *MemMapData,
  IN     UINT8              Sckt,
  IN     MEM_TYPE           SadType,
     OUT UINT8              *ChGran
  );

/** Program remote DRAM decode CSRs for memory map from config info in Host structure.

  Read remote SAD structures filled-in sysHost and program CHA remote decoder CSR's.

  @param[in] MemMapData    - Pointer to memory map private data structure
  @param[in] Sckt          - Socket Id

  @retval None

**/
VOID
WriteRemoteSadCsrs (
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT8            Sckt
  );

/** Initialize DDR and DDRT route tables

  Initialize route tables to a known value (that does not match a channel number)

  @param[in] Host       - Pointer to sysHost.
  @param[in] MemMapData - Pointer to the memory map private data structure

**/
VOID
InitializeRouteTables (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData
  );

#ifdef DDRT_SUPPORT
/**
  Enables block decoder if DDRT DIMM is installed.

  @param[in] Host             Pointer to the host structure.
  @param[in] MemMapData       Pointer to the memory map private data structure.

  @retval SUCCESS             BlockDecoder is successfully enabled.

**/
UINT32
EnableBlockDecoder (
  IN  SYSHOST              *Host,
  IN  MEMORY_MAP_DATA      *MemMapData
  );

/** Calculate the far memory controller target.

  The function calculates the far memory controller target for the input memory controller for a
  SAD entry.
  If the input memory controller is not involved in 2LM mode or In-Tile cache, itself is returned.
  If the input memory controller is involved in 2LM and it is X-tile cache mode, it returns the
  corresponding far memory controller ID.

  @param[in]  MemMapData     - Pointer to the memory map private data structure.
  @param[in]  SocketId       - The socket index.
  @param[in]  McId           - The MC index.

  @return The far memory target ID in a socket.

**/
UINT8
GetFmTarget (
  IN  MEMORY_MAP_DATA    *MemMapData,
  IN  UINT8              SocketId,
  IN  UINT8              McId
  );

/**
  This routine checks the POR configuration for X-tile 2LM for mod3

  @param[in] McBitmap         Far memory memory controller bit map.
  @param[in] ChannelBitmap    Far memory channel bit map.
  @param[in] NmMcBitmap       Near memory memory controller bit map.
  @param[in] NmChannelBitmap  Near memory channel bit map.

  @retval TRUE         The memory configuration is mod3 X-tile 2LM configuration.
  @retval FALSE        The memory configuration is not mod3 X-tile 2LM configuration.

**/
BOOLEAN
CheckMod3XTile2lmPorConfig (
  IN UINT8    McBitmap,
  IN UINT8    ChannelBitmap[MAX_IMC],
  IN UINT8    NmMcBitmap,
  IN UINT8    NmChannelBitmap[MAX_IMC]
  );

/**
  This routine checks the POR configurations for X-tile 2LM

  @param[in] Host         Pointer to host pointer.
  @param[in] MemMapData   Pointer to the memory map private data structure.
  @param[in] SocketId     Id for the socket to check.

  @retval TRUE         The memory configuration is POR X-tile 2LM configuration.
  @retval FALSE        The memory configuration is not POR X-tile 2LM configuration.

**/
BOOLEAN
CheckXTile2lmPorConfig (
  IN SYSHOST          *Host,
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT8            SocketId
  );


/**
  This routine does pre-calculation of far memory partition in XTile 2LM in SNC mode.
  The related near memory bitmap for each cluster and far memory partition info is saved in
  memory map private data structure in future interleave.

  @param[in]      Host         Pointer to host pointer.
  @param[in,out]  MemMapData   Pointer to memory map private data structure.
  @param[in]      SocketId     Id for the socket to check.

**/
VOID
CalculateXTile2lmSncFarMemoryPartition (
  IN     SYSHOST          *Host,
  IN OUT MEMORY_MAP_DATA  *MemMapData,
  IN     UINT8            SocketId
  );
#endif // DDRT_SUPPORT

/**
  Initializes security memory map private data.

  @param[in, out] MemMapData  Point to memory map private data structure.

**/
VOID
InitializeSecurityData (
  IN OUT MEMORY_MAP_DATA *MemMapData
  );

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
  );

/**

  This routine performs SGX data after memory map.

  @param[in] MemMapData   Pointer to the memory map private data structure.

**/
VOID
PostMemMapSgxData (
  IN MEMORY_MAP_DATA  *MemMapData
  );

/**
  Get a McBitmap bitmask for MC's in a given cluster. Assumes that the ClustersPerSocket is a value
  of "2" or "4".

  @param[in] Cluster           - The cluster number for the requested bitmask
  @param[in] ClustersPerSocket - The number of clusters per socket

  @return The MC bitmask for MC's in the input cluster. A value of "1" is retuned if the maximum
  IMC is 1.
**/
UINT8
GetMcBitMaskForCluster (
  IN UINT8 Cluster,
  IN UINT8 ClustersPerSocket
  );

/**
  Create SAD rules for NUMA/UMA on many-MC silicon.

  This routine interleaves memory on the MC and channel level but not across
  sockets for NUMA configuration and across sockets for UMA configuration. It
  figures out the mapping and fills in the structures for the SAD table,
  but does not program any CSRs.

  @param[in, out] Host            Pointer to the host structure.
  @param[in, out] MemMapData      Pointer to the memory map data structure.
  @param[in]      SocketBitmap    Bitmap of sockets requested for interleave.
  @param[in]      Cluster         Cluster in socket for SNC, or 0 if not SNC.
  @param[in]      MemType         Type of memory being interleaved.

  @retval SUCCESS       Memory was successfully interleaved.
  @retval FAILURE       Interleave could not be supported for memory type.
**/
UINT32
SadInterleaveConfig (
  IN OUT  SYSHOST         *Host,
  IN OUT  MEMORY_MAP_DATA *MemMapData,
  IN      UINT8           SocketBitmap,
  IN      UINT8           Cluster,
  IN      UINT16          MemType
  );

/** Calculate interleave wayness from the bitmap.

  Calculate interleave wayness from the input Socket, MC or channel bitmaps.

  @param[in] Bitmap     - Bitmap of the interleave.

  @retval Ways          - Interleave ways

**/
UINT8
CalculateInterleaveWaysfromBitmap (
  IN UINT8 Bitmap
  );

/** Get a McBitmap bitmask for MC's in a given socket half.

  @param[in] Half - The half to process

  @retval 0x3       The bitmask for MC0 and MC1 in RtHalf0
  @retval 0xC       The bitmask for MC2 and MC3 in RtHalf1
**/
UINT8
GetMcBitMaskForHalf (
  IN ROUTE_TABLE_HALVES  Half
  );

/** Check CPU capable of 3 channel interleave across two MC's.

  Check the installed CPU is a one supporting channel interleave across MC's.

  @retval TRUE     -  CPU capable of supporting the feature.
  @retval FALSE    -  CPU cannot support the feature.

**/
BOOLEAN
IsCpuCapableThreeChannelInterleaveAcrossMcs (
  VOID
  );

/** Fill Sad2tad mappings and TAD regions types CSRs from config. info in Host structure.

  Read SAD and TAD structures filled-in sysHost and program Mesh2Mem decoder CSR's.

  @param[in] Host          - Pointer to sysHost
  @param[in] MemMapData    - MemMapData  Pointer to the memory map private data structure.
  @param[in] Sckt          - Socket Id

  @retval None

**/
VOID
WriteMesh2MemCsrs (
  IN  SYSHOST         *Host,
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           Sckt
  );

/** Get the half (H0 or H1) corresponding to the MC.

  Returns a value of zero if max number of memory controller per socket is less than 2. Assumes that the number of MCs per half is symmetric
  and that the total number of MCs is an even number.

  @param[in] McIndex - The MC index to process

  @retval RtHalf0       The input MC resides in Socket Half H0
  @retval RtHalf1       The input MC resides in Socket Half H1
**/
ROUTE_TABLE_HALVES
GetHalfForMc (
  IN UINT8 McIndex
  );

/** Get a McBitmap bitmask for MC's in a given socket half.

  @param[in] Half - The half to process

  @retval 0x3       The bitmask for MC0 and MC1 in RtHalf0
  @retval 0xC       The bitmask for MC2 and MC3 in RtHalf1
**/
UINT8
GetMcBitMaskForHalf (
  IN ROUTE_TABLE_HALVES  Half
  );

/** Interleave memory between IMC's within a socket or within an IMC.

  This routine figures out the interleavable memory and its mapping. The structures for SAD, TAD are filled.
  It does not program any CSR's.
  The order of mapping will first be volatile memory of all sockets followed by persistent memory of all sockets.

  @param[in] Host       - Pointer to sysHost.
  @param[in] MemMapData - Pointer to the memory map private data structure

  @retval SUCCESS      -  Memory NUMA interleaved.
  @retval FAILURE      -  Interleave failed.

**/
UINT32
SocketNonInterleave (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData
  );


/**
  Returns a pointer to the Route Table Data corresponding to the input memory type. This pointer is only valid as long
  as the MemMapData is in scope. This function is intended to be called as part of the 4MC NUMA Interleaving algorithm.
  When called for configs with less than 2 MCs, then the DDR route table pointer is returned by default.

  Note that the distinction between silicon route tables (i.e. RT1\RT2\RT2LM) is done later in the flow during the
  CSR write phase. The DDR route table is used for 1LM or 2LM volatile memory modes and can populate RT1 in 1LM & x-tile 2LM
  or RT1 and RT2LM for in-tile 2LM. The DDRT table is used to populate RT2.

  @param[in]      MemMapData         Pointer to the memory map data structure.
  @param[in]      SocketId           The index of socket.
  @param[in]      MemType            Type of memory Route Tables to search

  @retval ROUTE_TABLE_DATA* A Pointer to the Route Table Data.
**/
ROUTE_TABLE_DATA*
GetRouteTableDataForMemType (
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           SocketId,
  IN  MEM_TYPE        MemType
  );

/**

  Get cluster for MC

  @param[in]      MemMapData  Pointer to the memory map private data structure.
  @param[in]      McIndex     MC index to get the hemisphere for.

  @return Cluster index for the MC.

**/
UINT8
GetClusterForMc (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           McIndex
  );

/**

  Get the size of the DDR Memory that will be used as 2LM cache.
  The value returned will be guaranteed to be a power of 2 and in 4GB Granularity.

  @param[in] Sckt        - Socket number
  @param[in] McIndex     - Memory controller
  @param[in] ChIndex     - Channel controller

  @retval DDR Cache Size for the input channel as a power of 2 (4GB Granularity)

**/
UINT32
GetNmCacheSize (
  IN UINT8 Sckt,
  IN UINT8 McIndex,
  IN UINT8 ChIndex
  );

/**
  Determine if the CPU cluster is based on north-south or west-east.

  This function depends on CpuTypeLib to get the cpu type and subtype
  and determine the cluster type.
    1. For SPR HCC        It is north-south.
    2. For SPR XCC & LCC  It is west-east.
    3. For ICX CPU        It is always west-east.

  @retval TRUE      CPU cluster is based on north-south.
  @retval FALSE     CPU cluster is based on east-west.

**/
BOOLEAN
IsNorthSouthCluster (
  VOID
  );

/**
  Fill in 10nm wave X specific CSRs for memory map from config info in Host structure besides
  the common part implemented in WriteMemoryMapCsrs().

  @param[in] Host         Pointer to sysHost
  @param[in] MemMapData   Pointer to the memory map private data structure

  @retval N/A

**/
VOID
WriteMemoryMapCsrsEx (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData
  );

/**
  Find a DDR route table for the given interleave.

  Optionally, return the matching route table (H0/H1) for the route table that
  matched, and the index for single target interleaves (always zero if not
  single-target).

  @param[in]      MemMapData         Pointer to the memory map data structure.
  @param[in]      Socket             Socket index.
  @param[in]      MemType            Type of memory Route Tables to search
  @param[in]      Cluster            Cluster in socket for SNC, or 0 if not SNC.
  @param[in]      McBitmap           Bitmap of MCs in the interleave.
  @param[in]      ChannelBitmap      Array of bitmaps of channels.
  @param[out]     MatchingHalfBitmap Return location for the matching half
  @param[out]     SingleTarget       Index for single target interleaves, or zero.

  @retval TRUE          A matching DDR route table was found.
  @retval FALSE         No matching DDR route table was found.
**/
BOOLEAN
FindRouteTable (
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           Socket,
  IN  MEM_TYPE        MemType,
  IN  UINT8           Cluster,
  IN  UINT8           McBitmap,
  IN  UINT8           ChannelBitmap[MAX_IMC],
  OUT UINT8           *MatchingHalfBitmap,    OPTIONAL
  OUT UINT8           *SingleTargetIndex      OPTIONAL
  );

/**

  Returns the number of clusters at system level for volatile memory for route tables.

  @param[in] MemMapData Pointer to the memory map private data structure.
  @param[in] Sckt       Socket Id

  @return System cluster level

**/
UINT8
RtGetTotalClusterVolMem (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Sckt
  );

#ifdef DDRT_SUPPORT
/**

  Returns the number of clusters at system level for PMem for route tables.

  @param[in] MemMapData Pointer to the memory map private data structure.
  @param[in] Sckt       Socket Id

  @return System cluster level

**/
UINT8
RtGetTotalClusterPMem (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Sckt
  );
#endif

/**

  Process value based on platform and config

  @param Host         Pointer to sysHost
  @param Sckt         Socket number
  @param[in] InValue  Value to consider.

  @return  Updated value based on platform and config

**/
UINT32
GetAdjustedRirValue (
  PSYSHOST Host,
  UINT8    Sckt,
  UINT32   InValue
  );

/**

  set MC.AMAP and force latency on Wave 1 and Wave 2 platforms if PMem present

  @param[in] Socket     Socket number.
  @param[in] McIndex    Index of memory controller.
  @param[in] NumChPerMc Number of channels per memory controller.

  @retval None

**/
VOID
ForceLatencyIfRequired (
  IN UINT8  Sckt,
  IN UINT8  McIndex,
  IN UINT8  NumChPerMc
  );

/**
  This function writes Mesh2Mem and MC CSRs required to DISABLE critical chunk support.

  @param[in] SocketId    Socket Index.

  @retval N/A
**/
VOID
WriteCriticalChunkCsrs (
  IN UINT8      SocketId
  );

/**
  Create SAD entries for FPGA's present in the platform

  @param [in] Host         Pointer to sysHost
  @param [in] MemMapData   MemMapData  Pointer to the memory map private data structure
  @param [in] MemType      Type of memory region to be mapped

  @retval N/A
**/
UINT32
CreateNewRegionSadFPGA (
  IN PSYSHOST         Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT16           MemType
  );

/**

  Check if the input memory type can support UMA-Based Clustering on the SOC.

  @param[in]  MemMapData    Pointer to the memory map private data structure.

  @retval TRUE  The current memory type is allowed for UMA-Based Clustering.
  @retval FALSE The current memory type is not allowed for UMA-Based Clustering.

**/
BOOLEAN
IsUbcSupportedMemType (
  IN MEM_TYPE MemType
  );

/**

  Adjust SAD indexes used across SNC clusters to insure that there are not any disabled local DRAM rules between any
  enabled local DRAM rules.

  DRAM multicast feature is used to enable local DRAM rules for memory that resides in SNC clusters.
  DRAM broadcast feature to enable local DRAM rules for memory that does not reside in SNC clusters.
  Where there exists both SNC and non-SNC ranges the function patches NXM(s) to insure Local DRAM Rules
  are Contiguous when Mapped Memory is split between SNC and Non-SNC Ranges.

  @param[in] Host                - Pointer to sysHost
  @param[in] MemMapData          - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in] socketNum           - Socket number

  @retval Status

**/
UINT32
AdjustSadTableForSncMulticast (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           SocketNum
  );

/**
  Write FZM related CSRs.

  This function checks if the boot-time fast zero memory (aka FZM or BZM) feature is supported on
  each DRAM rule. And the FZM related CHA registers will be programmed accordingly.

  @param[in] Host                 Pointer to the sysHost structure.
  @param[in] MemMapData           Pointer to the memory map private data structure.
  @param[in] SocketId             Socket index.
  @param[in] BeginSadId           Index of the first SAD entry on cluster.

**/
VOID
WriteFzmCsrs (
  IN SYSHOST          *Host,
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT8            SocketId,
  IN UINT8            BeginSadId
  );


/**

  Check if PSMI Hob exist and valid trace sizes are requested. Validate size requested against 1LM memory mapped
  above 4GB and allocate memory buffers. Fill base address and allocation error in HOB.

  @param[in] Host                    Pointer to sysHost.
  @param[in] MemMapData              Pointer to the memory map private data structure.

  @retval EFI_SUCCESS                The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER      The function failed due to some illegal input.

**/
EFI_STATUS
EFIAPI
MemReservePsmiBuffers (
  IN SYSHOST          *Host,
  IN MEMORY_MAP_DATA  *MemMapData
  );

/**

  Sort channels in MC based on remsize value in descending order.

  @param[in]  Host             - Pointer to sysHost
  @param[in]  SocketNum        - Socket Id
  @param[in]  ChIndex          - Index of first channel in the IMC that needs to be sorted
  @param[out] ChannelSortList  - Channels per MC in order of decreasing size.

**/
VOID
ChannelSortMc (
  IN  PSYSHOST            Host,
  IN  UINT8               SocketNum,
  IN  UINT8               ChIndex,
  OUT CHANNEL_SORT_STRUCT ChannelSortList[]
  );

/** Find if the input MC Bitmap describes a cross-half MC population.

  @param[in] McBitmap - The MC Bitmap to process

  @retval TRUE        The input McBitmap describes a cross-half MC population
  @retval FALSE       The input McBitmap does not describe a cross-half MC population
**/
BOOLEAN
IsOneMcOnEachSide (
  IN      UINT8               McBitmap
  );

/** Get the number of MCs per half.

  Returns a value of one if max number of memory controller per socket is 1. Assumes that the number of MCs per half is symmetric
  and that the total number of MCs is an even number.

  @param[in] McIndex - The MC index to process

  @retval RtHalf0       Number of MCs enabled per half.
**/
UINT8
GetNumMcPerHalf (
  VOID
  );

/**
  Create SAD rules for NUMA/UMA on many-MC silicon.

  This routine interleaves memory on the MC and channel level but not across
  sockets for NUMA configuration and across sockets for UMA configuration. It
  figures out the mapping and fills in the structures for the SAD table,
  but does not program any CSRs.

  @param[in, out] Host            Pointer to the host structure.
  @param[in, out] MemMapData      Pointer to the memory map data structure.
  @param[in]      SocketBitmap    Bitmap of sockets requested for interleave.
  @param[in]      Cluster         Cluster in socket for SNC, or 0 if not SNC.
  @param[in]      MemType         Type of memory being interleaved.

  @retval SUCCESS       Memory was successfully interleaved.
  @retval FAILURE       Interleave could not be supported for memory type.
**/
UINT32
SadInterleave (
  IN OUT  SYSHOST         *Host,
  IN OUT  MEMORY_MAP_DATA *MemMapData,
  IN      UINT8           SocketBitmap,
  IN      UINT8           Cluster,
  IN      MEM_TYPE        MemType
  );

/** Create remote SAD entry.

  Add a SAD entry in all sockets that are not involved in the SAD interleave.

  @param[in, out] Host       Pointer to sysHost
  @param[in]      MemMapData Pointer to the memory map private data structure.
  @param[in]      McBitmap   Map of all the IMCs that are part of the interleave.
  @param[in]      NodeLimit  Limit value for the SAD entry
  @param[in]      MemType    Type of memory being interleaved.

  @retval  SUCCESS                             -  Remote SAD entry created for every other socket.
  @retval  ERROR_RESOURCE_CALCULATION_FAILURE  -  SAD rules exceed maximum count.

**/
UINT32
AddUmaRemoteSadEntry (
  IN OUT SYSHOST          *Host,
  IN     MEMORY_MAP_DATA  *MemMapData,
  IN     UINT8            McBitmap[MAX_SOCKET],
  IN     UINT32           NodeLimit,
  IN     MEM_TYPE         MemType
  );

/**
  Do all possible interleaving with given ways for many (>2) MCs. If requested interleave is
  across sockets then the SocketBitmap output represents the sockets that can be interleaved.

  @param[in, out] Host                Pointer to the host structure.
  @param[in, out] MemMapData          Pointer to the memory map data structure.
  @param[in, out] SocketBitmap        Bitmap of sockets requested/possible for interleave.
  @param[in]      Cluster             Cluster in socket for SNC, or 0 if not SNC.
  @param[in]      MemType             Type of memory being interleaved.
  @param[in]      McWays              Number of MCs to interleave.
  @param[in]      ChannelWays         Number of channels to interleave per MC.
  @param[out]     Possible            Requested interleave can be done or not possible.
  @param[in]      AllowAcrossHalves   Requested interleave is across halves or within a half.

  @retval SUCCESS       Requested interleave can be done or not possible.
  @retval FAILURE       Interleaving should have been possible, but failed.
**/
UINT32
IsInterleavePossible (
  IN OUT  SYSHOST         *Host,
  IN OUT  MEMORY_MAP_DATA *MemMapData,
  IN      UINT8            SocketBitmap,
  IN      UINT8            Cluster,
  IN      MEM_TYPE         MemType,
  IN      UINT8            McWays,
  IN      UINT8            ChannelWays,
  OUT     BOOLEAN          *Possible,
  IN      BOOLEAN          AllowAcrossHalves
  );

/**
  Check if 3 channel interleaving is possible for many (>2) MCs.

  @param[in]      Host              Pointer to the host structure.
  @param[in]      MemMapData        Pointer to the memory map data structure.
  @param[in, out] SocketBitmap      Bitmap of sockets requested/possible for interleave.
  @param[in]      Cluster           Cluster in socket for SNC, or 0 if not SNC.
  @param[in]      MemType           Type of memory being interleaved.
  @param[in]      McWays            Number of MCs to interleave.
  @param[in]      ChannelWays       Number of channels to interleave per MC.
  @param[out]     McBitmap          Bitmap of MCs in the interleave.
  @param[out]     ChannelBitmap     Array of bitmaps of channels.
  @param[out]     FmChannelBitmap   Array of bitmaps of channels of Far Memory type.
  @param[out]     ChannelSortList   Channels per MC in order of decreasing size.
  @param[in]      AllowAcrossHalves Specify if the interleave can be checked within or across half.

  @retval TRUE          The given interleave is possible.
  @retval FALSE         The given interleave is impossible.
**/
BOOLEAN
IsMod3InterleavePossible (
  IN  SYSHOST             *Host,
  IN  MEMORY_MAP_DATA     *MemMapData,
  IN OUT UINT8            *SocketBitmap,
  IN  UINT8               Cluster,
  IN  MEM_TYPE            MemType,
  IN  UINT8               McWays,
  IN  UINT8               ChannelWays,
  OUT UINT8               McBitmap[MAX_SOCKET],
  OUT UINT8               ChannelBitmap[MAX_SOCKET][MAX_IMC],
  OUT UINT8               FmChannelBitmap[MAX_SOCKET][MAX_IMC],
  OUT CHANNEL_SORT_STRUCT ChannelSortList[MAX_SOCKET][MAX_CH],
  IN  BOOLEAN             AllowAcrossHalves
  );

/**
  Check if interleaving is possible for many (>2) MCs.

  @param[in]      Host              Pointer to the host structure.
  @param[in]      MemMapData        Pointer to the memory map data structure.
  @param[in, out] SocketBitmap      Bitmap of sockets requested/possible for interleave.
  @param[in]      Cluster           Cluster in socket for SNC, or 0 if not SNC.
  @param[in]      MemType           Type of memory being interleaved.
  @param[in]      McWays            Number of MCs to interleave.
  @param[in]      ChannelWays       Number of channels to interleave per MC.
  @param[out]     McBitmap          Bitmap of MCs in the interleave.
  @param[out]     NmMcBitmap        Bitmap of MCs in the ineterleave as near memory.
  @param[out]     ChannelBitmap     Array of bitmaps of channels.
  @param[out]     FmChannelBitmap   Array of bitmaps of channels of Far Memory type.
  @param[out]     ChannelSortList   Channels per MC in order of decreasing size.
  @param[in]      AllowAcrossHalves Specify if the interleave can be checked within or across half.

  @retval TRUE          The given interleave is possible.
  @retval FALSE         The given interleave is impossible.
**/
BOOLEAN
IsXorInterleavePossible (
  IN  SYSHOST             *Host,
  IN  MEMORY_MAP_DATA     *MemMapData,
  IN OUT UINT8            *SocketBitmap,
  IN  UINT8               Cluster,
  IN  MEM_TYPE            MemType,
  IN  UINT8               McWays,
  IN  UINT8               ChannelWays,
  OUT UINT8               McBitmap[MAX_SOCKET],
  OUT UINT8               NmMcBitmap[MAX_SOCKET],
  OUT UINT8               ChannelBitmap[MAX_SOCKET][MAX_IMC],
  OUT UINT8               FmChannelBitmap[MAX_SOCKET][MAX_IMC],
  OUT CHANNEL_SORT_STRUCT ChannelSortList[MAX_SOCKET][MAX_CH],
  IN  BOOLEAN             AllowAcrossHalves
  );

/**
  Check Xor Interleave Symmetry Requirement

  Evaluate DIMM population symmetry to see if satisfies POR requirement or not.
  POR configuration is performance recommended and should be evaluated with priority.
  Non-POR configuration is not performance recommended and would go through speical handling flow.

  The implementation of this routine is wave specific.

  @param[in]      Host                                Pointer to the host structure.
  @param[in]      MemMapData                          Pointer to the memory map data structure.
  @param[in, out] SocketBitmap                        Bitmap of sockets requested/possible for interleave.
  @param[in]      Cluster                             Cluster in socket for SNC, or 0 if not SNC.
  @param[in]      MemType                             Type of memory being interleaved.
  @param[in]      McBitmap[MAX_SOCKET]                Mc bit map of the interleave to be evaluated.
  @param[in]      ChannelBitmap[MAX_SOCKET][MAX_IMC]  Channel bit map of the interleave to be evaluated.
  @param[in]      McWays                              Ways of mc of the interleave to be evaluated.
  @param[in]      ChannelWays                         Ways of channel of the interleave to be evaluated.

  @retval SUCCESS          The DIMM population symmetry is POR.
  @retval RETRY            The DIMM population symmetry is Non-POR, but should re-evaluate the same interleave
                           configuration by asymmetric flow.
  @retval FAILURE          The DIMM population symmetry is non-POR, and should try smaller interleave configurations.
**/
UINT32
CheckXorInterleaveSymmetryRequirement (
  IN  SYSHOST             *Host,
  IN  MEMORY_MAP_DATA     *MemMapData,
  IN  UINT8               SocketBitmap,
  IN  UINT8               Cluster,
  IN  MEM_TYPE            MemType,
  IN  UINT8               McBitmap[MAX_SOCKET],
  IN  UINT8               ChannelBitmap[MAX_SOCKET][MAX_IMC],
  IN  UINT8               McWays,
  IN  UINT8               ChannelWays
  );

/**
  Do interleaving across sockets for many (>2) MCs.

  @param[in, out] Host            Pointer to the host structure.
  @param[in, out] MemMapData      Pointer to the memory map data structure.
  @param[in]      SocketBitmap    Bitmap of sockets to interleave.
  @param[in]      Cluster         Cluster in socket for SNC, or 0 if not SNC.
  @param[in]      MemType         Type of memory being interleaved.
  @param[in]      McWays          Number of MCs to interleave.
  @param[in]      ChannelWays     Number of channels to interleave per MC.
  @param[in]      McBitmap        Bitmap of the MCs to be interleaved.
  @param[in]      ChannelBitmap   Array of Bitmaps of the DDR Channel population within the MCs to be interleaved.
  @param[in]      FmChannelBitmap Array of Bitmaps of the DDRT Channel population within the MCs to be interleaved.
  @param[in]      ChannelSortList Channels per MC in order of decreasing size.
  @param[out]     NodeLimit       Pointer to return the limit of SAD entry created.
  @param[in]      MirrorEnable    Enable/Disable mirroring for the interleave's SAD rule.

  @retval SUCCESS       Memory was successfully interleaved.
  @retval FAILURE       Interleave could not be supported for memory type.
**/
UINT32
ExecuteMod3Interleave (
  IN OUT  SYSHOST             *Host,
  IN OUT  MEMORY_MAP_DATA     *MemMapData,
  IN      UINT8               SocketBitmap,
  IN      UINT8               Cluster,
  IN      MEM_TYPE            MemType,
  IN      UINT8               McWays,
  IN      UINT8               ChannelWays,
  IN      UINT8               McBitmap[MAX_SOCKET],
  IN      UINT8               ChannelBitmap[MAX_SOCKET][MAX_IMC],
  IN      UINT8               FmChannelBitmap[MAX_SOCKET][MAX_IMC],
  IN      CHANNEL_SORT_STRUCT ChannelSortList[MAX_SOCKET][MAX_CH],
  OUT     UINT32              *InterleaveLimit,
  IN      UINT8               MirrorEnable
  );

/**
  Do interleaving across sockets for many (>2) MCs.

  @param[in, out] Host            Pointer to the host structure.
  @param[in, out] MemMapData      Pointer to the memory map data structure.
  @param[in]      SocketBitmap    Bitmap of sockets to interleave.
  @param[in]      Cluster         Cluster in socket for SNC, or 0 if not SNC.
  @param[in]      MemType         Type of memory being interleaved.
  @param[in]      McWays          Number of MCs to interleave.
  @param[in]      ChannelWays     Number of channels to interleave per MC.
  @param[in]      McBitmap        Bitmap of the MCs to be interleaved
  @param[in]      ChannelBitmap   Array of Bitmaps of the DDR Channel population within the MCs to be interleaved
  @param[in]      FmChannelBitmap Array of Bitmaps of the DDRT Channel population within the MCs to be interleaved
  @param[in]      ChannelSortList Channels per MC in order of decreasing size.
  @param[out]     InterleaveLimit Pointer to return the limit of SAD entry created.
  @param[in]      MirrorEnable    Enable/Disable mirroring for the interleave's SAD rule

  @retval SUCCESS       Memory was successfully interleaved.
  @retval FAILURE       Interleave could not be supported for memory type.
**/
UINT32
ExecuteXorInterleave (
  IN OUT  SYSHOST             *Host,
  IN OUT  MEMORY_MAP_DATA     *MemMapData,
  IN      UINT8               SocketBitmap,
  IN      UINT8               Cluster,
  IN      MEM_TYPE            MemType,
  IN      UINT8               McWays,
  IN      UINT8               ChannelWays,
  IN      UINT8               McBitmap[MAX_SOCKET],
  IN      UINT8               NmMcBitmap[MAX_SOCKET],
  IN      UINT8               ChannelBitmap[MAX_SOCKET][MAX_IMC],
  IN      UINT8               FmChannelBitmap[MAX_SOCKET][MAX_IMC],
  IN      CHANNEL_SORT_STRUCT ChannelSortList[MAX_SOCKET][MAX_CH],
  OUT     UINT32              *InterleaveLimit,
  IN      UINT8               MirrorEnable
  );
#endif //_MEM_DECODE_NODE_H_
