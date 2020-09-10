/** @file
  Routes to register includes

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
#ifndef _MEM_ADDRESS_TYPES_H_
#define _MEM_ADDRESS_TYPES_H_


//
// Memory mapping defines
//
#define MAX_TWO_LM_COMBINATION  7
#define MAX_TWO_WAY_COMBINATION 3
#define MAX_ONE_WAY_COMBINATION 3

#define MEM_SIZE_16TB (16 * 1024)
#define MEM_SIZE_32TB (32 * 1024)

#define EIGHT_WAY 8
#define SIX_WAY   6
#define FOUR_WAY  4
#define THREE_WAY 3
#define TWO_WAY   2
#define ONE_WAY   1

#define SOCKET_BITMAP_ALL 0xFF

#define INTERLEAVE_LIST_BITS_CHANNEL  6
#define GB_BOUNDARY_ALIGN 0xFFFFFFF0             // Align to GB boundary
#define GB_ALIGN_NVM 0xFFF0

#define MC1 0x1  // to get to proper MC index by adding this value

//
// 10nm specific defines
//
#define RT0   0
#define RT1   1
#define RT2   2
#define RT2LM 3

#define MAX_MODE_TABLES_IMC   2  //Mode0 and Mode1 per IMC

#define INTERLEAVE_MODE_256B      2 // Index = [10:8]
#define INTERLEAVE_MODE_256B_XOR  7 // Index = [10:8] ^ [16:14] ^ [24:22]
#define INTERLEAVE_MODE_4KB       6 // Index = [14:12]

#define INVALID_SAD             0xFF
#define INVALID_TAD             0xFF

#define SAD_GRAN_64MB            64
#define SAD_GRAN_128MB           128

#define MEM_256MB_IN_64MB_UNIT     0x4
#define MEM_32GB_IN_64MB_UNIT      0x200
#define MEM_256GB_IN_4GB_UNIT      0x40

//
// Total 0-7 knobs [0,3,4 - not set by bios, 1,2,5,6,7 set by bios]
// INTEL_PMEM_PERF_KNOBS_CNT - count for knobs that bios has to set
//
#define INTEL_PMEM_PERF_KNOBS_CNT       0x05
#define PERF_KNOB_SUPPORTED             0x01
#define PERF_KNOB_NOT_SUPPORTED         0x00

// FIS 2.0 for Perf knob 7
#define PERF_KNOB7_FW_MAJOR_REVISION     0x02
#define PERF_KNOB7_FW_MINOR_REVISION     0x01
#define PERF_KNOB7_FW_REVISION           0x0492

//
// Memory mode definitions
//
#define MODE_1LM           BIT0
#define MODE_2LM           BIT1
#define MODE_AD            BIT2
#define MODE_NATIVE_ADWB   BIT3

//
// struct definitions
//

typedef struct _ENCODED_INTERLEAVE_GRANULARITY {
  UINT8 chaTgt;
  UINT8 chaCh;
  UINT8 fmCh;
} ENCODED_INTERLEAVE_GRANULARITY;

typedef struct  {
  UINT32 SkuLimit;             // in 64GB units
  UINT32 MemoryAllocated;      // memory mapped to SPA(already allocated)
  UINT8  SkuLimitValid;        // 0-FALSE, 1-TRUE; error/success per socket
  UINT8  Violation;            // 0-FALSE, 1-TRUE
} MEMORY_MAP_SKU_LIMIT;

typedef struct {
  BOOLEAN Enable;
  UINT16  MemType;
  UINT8   ChannelWays;
  UINT8   ChannelInterBitmap;
  UINT8   FmChannelInterBitmap;
  UINT32  InterleaveSize;
} MEMORY_MAP_SNC_SAD;

typedef struct {
  BOOLEAN Calculated;
  MEMORY_MAP_SNC_SAD Sad[SAD_RULES_10NM];
} MEMORY_MAP_SNC_TYPE_DATA;

typedef struct {
  MEMORY_MAP_SNC_TYPE_DATA Ddr4;
  MEMORY_MAP_SNC_TYPE_DATA Ddrt;
} MEMORY_MAP_SNC_DATA;

typedef enum {
  CxlVolatileMem,
  CxlNonVolatileMem,
  MaxCxlMediaType
} CXL_MEDIA_TYPE;

typedef enum {
  CxlNoInterleave,
  Cxl256BInterleave,
  Cxl4KBInterleave,
  MaxCxlInterleave
} CXL_INTERLEAVE;

typedef struct {
  UINT32                   Size;
  CXL_MEDIA_TYPE           MediaType;
  CXL_INTERLEAVE           DesiredInterleave;
  UINT8                    InterleaveWays;
  UINT32                   InterleaveList;
  BOOLEAN                  MemExpander;
  SAD_TABLE                *SadEntry;
} CXL_HDM_DATA;

typedef UINT16                  CXL_STATUS;
#define CXL_STATUS_UNSUPPORTED  BIT0

typedef struct {
  CXL_STATUS   Status;
  UINT16       VendorId;
  UINT64       SerialNumber;
  UINT8        HdmCount;
  BOOLEAN      MemHwInitMode;
  UINT8        SocketBitmap;
  UINT8        StackBitmap[MAX_SOCKET];
  CXL_HDM_DATA HdmData[MAX_CXL_HDM_RANGES];
} CXL_MEM_DEVICE_DATA;

typedef struct {
  CXL_MEM_DEVICE_DATA    CxlMemDeviceData[MAX_CXL_PER_SOCKET * MAX_SOCKET];
  UINT8                  CxlDeviceNum;
} CXL_DATA;

//
//  Memory map private structure for Route Table data.
//
typedef struct {
  UINT8                MaxRtWays;                                                    // Maximum RT interleave ways
  UINT8                RouteTableUsed[MAX_SOCKET][MAX_CLUSTERS][MAX_RT];             // # of RT ways being used by SAD rules.
  UINT8                ChRouteTable[MAX_SOCKET][MAX_CLUSTERS][MAX_RT][MAX_RT_WAYS];  // DDR/NM memory channel ID route table.
  UINT8                TgtRouteTable[MAX_SOCKET][MAX_CLUSTERS][MAX_RT][MAX_RT_WAYS]; // MC/Target ID route table.
} ROUTE_TABLE_DATA;

typedef UINT32         SGX_STATUS;

#define SGX_STATUS_SUCCESS           0
#define SGX_STATUS_UNSUPPORTED       BIT0
#define SGX_STATUS_INVALID_PARAMETER BIT1
#define SGX_STATUS_OUT_OF_RESOURCES  BIT2

typedef struct {
  BOOLEAN              XTile2lm[MAX_SOCKET];
  UINT8                VolMemMode;
} SGX_MCHECK_DATA;

typedef struct {
  UINT64       SeamrrBase; // Base Address of SEAMRR region
} TDX_SEAMRR_DATA;

typedef struct {
  SGX_STATUS           Status;
  SGX_PRMRR_DATA       SgxPrmrrData;
  SGX_MCHECK_DATA      SgxMcheckData;
  TDX_SEAMRR_DATA      TdxSeamrrData;
} SECURITY_MEMMAP_HOB;

typedef struct {
  SECURITY_MEMMAP_HOB  SecurityMemMapHob;
  UINT64               PrmrrSize;
  UINT8                PrmrrCountPerSocket[MAX_SOCKET];
  UINT64               ValidPrmrrSizeBitmap[MAX_SOCKET][MAX_IMC];
  UINT64               SeamrrSize;
} SECURITY_DATA;

typedef struct {
  UINT8  NmChannelWays;
  UINT8  NmTargetWays;
} NM_INTLV_DATA;

typedef struct {
  UINT16 RirLimit[MAX_RIR]; ///< Rank Interleave Register Limits
  UINT8  RirWay[MAX_RIR];   ///< Rank Interleave Register Ways
  UINT8  RirValid[MAX_RIR]; ///< RIR valid bit
  UINT32 RirOffset[MAX_RIR][MAX_RIR_WAYS];
  UINT8  RirIntList[MAX_RIR][MAX_RIR_WAYS];
  UINT32 RirDdrtLimit[MAX_RIR_DDRT];
  UINT8  RirDdrtWay[MAX_RIR_DDRT];
  UINT8  RirDdrtValid[MAX_RIR_DDRT];
  UINT32 RirDdrtOffset[MAX_RIR_DDRT][MAX_RIR_WAYS];
  UINT8  RirDdrtIntList[MAX_RIR_DDRT][MAX_RIR_WAYS];
} MEMMAPDATA_CHANNEL;

typedef struct _RSAD_TABLE {
  UINT8   Enable;         // Rule enable
  UINT8   Attr;           // 00 - Coherent DRAM, 01 - Memory Mapped CFG, 10 - Low Bandwidth Coherent DRAM, 11 - High Bandwidth Coherent DRAM
  UINT32  Base;           // Address of remote DRAM rule base
  UINT8   NodeId;         // Target Node for this DRAM rule
  UINT32  Limit;          // Address of remote DRAM rule limit
} RSAD_TABLE;

#define XTILE_2LM_SNC_LAST    0xFF

typedef struct {
  MEMMAPDATA_CHANNEL MemMapChannel[MAX_CH];
  RSAD_TABLE         RSad[MAX_REMOTE_SAD_RULES];       // Remote SAD rules, always empty for SKX, only rules 0-13 is available for ICX.
  BOOLEAN            XTile2lm;                         // Flag to indicate current 2LM mode is In-Tile or X-Tile
  UINT8              XTile2lmFmTarget[MAX_IMC];        // Far memory target for each memory target (valid only in XTile 2LM mode)
  BOOLEAN            XTile2lmSncFmPartition;           // Flags to indicate whether we need to partition far memory (valid only in XTile 2LM mode in SNC).
  UINT32             XTile2lmSncVolSize[MAX_CH];       // SNC partitioned far memory size in XTile 2LM mode (valid only if XTile2lmSncFmPartition is TRUE).
  UINT8              AppDirectByOneChannelBitmap;      // Channel bitmap for AD x1 interleaves per socket
  UINT8              AppDirectByOneCount;              // Count number of AD x1 interleaves per socket
  NM_INTLV_DATA      NmIntlvValues[MAX_IMC];           // MC NM channel and target interleaves.
  UINT8              Ddr1lmX1IntlvCount[MAX_CLUSTERS]; // Count number of DDR 1LM x1 interleave(s) per socket cluster.
  UINT32             NvMemSadIdxBitMap[MAX_DRAM_CLUSTERS]; /// < Bit map of the sadIdx number to indicate if the sad belongs to NVMEM range.
  BOOLEAN            UseRT0forAdx1;                    // Indicate ADx1 system configuration to use RT0
} MEMMAPDATA_SOCKET;

typedef enum {
  NoPmCaching,
  LegacyPmCaching,     // Pmem caching supported in CR versions lesser than 2.0 (14nm and 10nm Wave1/Wave2). Does not allow PMEM and PMEM$ to co-exist.
  NativePmCaching,     // Native AD-WB caching support introduced in CR 2.0 (10nm Wave3 and beyond)
  MaxPmCacheType
} PMEM_CACHING_TYPE;

//
// Private memory map structure
//
typedef struct {
  BOOLEAN              IsNumaEn;                    // Flag to check NUMA interleaving is enabled
  MEMORY_MAP_SKU_LIMIT MemMapSkuLimit[MAX_SOCKET];  // CPU SKU Limit
//  HBM_MEMORY_MAP_DATA  HbmData;
  CXL_DATA             CxlData;                     // Wave 3 specific data for CXL support
  MEMORY_MAP_SNC_DATA  SncData[MAX_SOCKET][MAX_IMC];
  SECURITY_DATA        SecurityData;
  ROUTE_TABLE_DATA     FarDdrRouteTableData;        // Route Table Data for DDR memory as far memory
  ROUTE_TABLE_DATA     DdrRouteTableData;           // Route Table Data for DDR memory 1LM and 2LM
  ROUTE_TABLE_DATA     DdrtRouteTableData;          // Route Table Data for DDRT memory
  UINT8                UmaBasedClustersPerSocket;   // Number of UMA based clusters per socket
  BOOLEAN              Is2LmUsingRt2lm;             // Flag to check whether 2LM is using separate RT instances for channel and target decoding
  UINT8                DdrtIntlvGranularity;        // DDRT interleave granularity
  UINT8                MaxSadRules;                 // Maximum SAD entries supported by silicon (24 for 14nm silicon, 16 for 10nm silicon)
  UINT8                MaxRemoteSadRules;           // Maximum remote SAD rule entries supported in 10nm (14 for 10nm wave1, 21 for 10nm wave3)
  UINT8                NumberofChaDramClusters;     // Number of CHA DRAM decoder clusters
  UINT8                MemMapDegradeLevel;          // <0 - MEM_MAP_LVL_NORMAL   1 : MEM_MAP_LVL_IGNORE_CFGIN     2 :MEM_MAP_LVL_IGNORE_NEW_DIMMS
  UINT8                MemMapState;                 // <0 - MEM_MAP_STATE_RESOURCE_CALCULATION    1-MEM_MAP_STATE_RESOURCE_CALCULATION_SUCCESS    2-MEM_MAP_STATE_RESOURCE_FAILURE
  PMEM_CACHING_TYPE    PmemCaching;                 // AD-WB caching policy for controlling flows
  BOOLEAN              IsPorConfig;                 // Interleaving policy for controlling flows
  MEMMAPDATA_SOCKET    MemMapSocket[MAX_SOCKET];    // Memory map data per socket
  VOID                 *HbmMemoryMapData;           // HBM private memory
  UINT32               SadLimitAdjustment;          // Limit adjustment of 4G SAD entry for GB alignment. To be non-zero only in 1LM mod3 interleave.
  UINT32               TadLimitAdjustment;          // Size to include along with total-SPA in PCAT table
  UINT8                Below4GBSktIndex;            // Mark skt index which has sad below 4GB
} MEMORY_MAP_DATA;

#endif // _MEM_ADDRESS_TYPES_H_

