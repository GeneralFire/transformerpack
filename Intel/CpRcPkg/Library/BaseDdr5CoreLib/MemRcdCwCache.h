/** @file
  MemRcdCwCache.h
  Structures, definitions and function prototypes needed to cache
  DDR5 Registering Clock Driver (RCD) Control Words

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef  _MEM_RCD_CW_CACHE_H_
#define  _MEM_RCD_CW_CACHE_H_

#include <Library/Ddr5CoreLib.h>
#include <Memory/Ddr5Cmds.h>
#include <Memory/Ddr5RcwRegs.h>
#include <Memory/JedecDefinitions.h>

#pragma pack(push, 1)

#define DEBUG_RCD_CW_CACHE    1

//
// The NVDATA structure for DDR5 Registering ClockDriver (RCD)
// The data stored here is used to cache control words and settings
// and to save them to NVRAM for use in the fast boot and resume paths.
//

#if FixedPcdGetBool(PcdMcrSupport)
#define RDIMM_CW_RSVD_PAGE_LIST \
  RDIMM_CW_PAGE70,  /* MCR Page 70h */

#define RDIMM_CW_RSVD_PAGES     1   // Number of cached MCR RCW Pages
#else
#define RDIMM_CW_RSVD_PAGE_LIST
#define RDIMM_CW_RSVD_PAGES     0
#endif // FixedPcdGetBool(PcdMcrSupport)

//
// List of control word pages cached
//
#define RCD_CW_PAGE_CACHE_LIST \
    RDIMM_CW_RSVD_PAGE_LIST                     \
    RDIMM_CW_PAGE0,                             \
    RDIMM_CW_PAGE1,                             \
    RDIMM_CW_PAGE2,                             \
    RDIMM_CW_PAGE9A,  /* Ddr5RambusD0Errata */  \
    RDIMM_CW_PAGE9B,  /* Ddr5RambusD0Errata */  \
    RDIMM_CW_PAGEFB,  /* Ddr5RambusD0Errata */  \
    RDIMM_CW_PAGEFE   /* Ddr5RambusD0Errata */

#define RCD_CW_PAGE_CACHE_LENGTH (7 + RDIMM_CW_RSVD_PAGES)

//
// List of global control words
//
#define RCD_CW_GLOBAL_CW_LIST \
    RDIMM_RW00,   \
    RDIMM_RW01,   \
    RDIMM_RW02,   \
    RDIMM_RW03,   \
    RDIMM_RW04,   \
    RDIMM_RW05,   \
    RDIMM_RW06,   \
    RDIMM_RW07,   \
    RDIMM_RW10,   \
    RDIMM_RW25,   \
    RDIMM_RW26,   \
    RDIMM_RW27,   \
    RDIMM_RW28,   \
    RDIMM_RW29,   \
    RDIMM_RW2A,   \
    RDIMM_RW30

//
// Description of a paged control word
//
typedef struct {
  UINT8   Page;
  UINT8   Address;
} RCD_PAGED_CW;

typedef enum {
  RcdCwRead,
  RcdCwWrite,
  RcdCwAccessTypeMax
}  RCD_CW_ACCESS_TYPE;

//
// Per-socket cache state bits
//
typedef union {
  struct {
    UINT8 Initialized   : 1; // Bits[0]   - 1 = Cache has been initialized
    UINT8 Reserved      : 7; // Bits[7:1] - Reserved for future use.
  } Bits;
  UINT8 Data;
} RCD_CW_CACHE_STATE;

//
// Single RCD CW cache entry
// All DDR5 RCD Control Words are 8-bits
//
typedef struct {
  UINT8                         Data;
} RCD_CW_REGISTER_CACHE;

//
// The number of instances in the cache for a single control word for the
// entire socket. This assumes there is 1 instance of a CW per Ch/SubCh/Dimm.
// The exception is for Global Control Words which are only addressable on
// Subchannel A and therefore only require 1 instance per Ch/Dimm.
//
// The number is used to define the size of the Valid structure, which has 1 bit
// per CW instance for the entire socket.
//
// SIZE_VALID_ARRAY* converts the number of instances into the number of bytes needed
// to store 1 bit per instance, rounding up to ensure all bits are included.
//
#define RCD_CW_INSTANCES_PER_SOCKET (MAX_CH * SUB_CH * MAX_DIMM)
#define SIZE_VALID_ARRAY_RCD_CW ((RCD_CW_INSTANCES_PER_SOCKET + (MAX_BITS_IN_BYTE - 1)) / MAX_BITS_IN_BYTE)

//
// Definitions to index into the "Valid" bit array
//
#define VALID_BIT_CH_OFFSET_RCD_CW            (SUB_CH * MAX_DIMM)
#define VALID_BIT_SUB_CH_OFFSET_RCD_CW        (MAX_DIMM)
#define VALID_BIT_DIMM_OFFSET_RCD_CW          1

//
// Caching RCD Direct Access CW
//
typedef struct {
  ///
  /// Reserve 1 byte for each instance of the control word
  //
  RCD_CW_REGISTER_CACHE         Cache[MAX_CH][SUB_CH][MAX_DIMM];
  ///
  /// 1 Valid bit per cache entry
  //
  UINT8                         Valid[SIZE_VALID_ARRAY_RCD_CW];
} RCD_DIRECT_CW_CACHE;

//
// Caching RCD Direct Access CW
//
typedef struct {
  ///
  /// The page of the control word
  //
  UINT8                         Page;
  ///
  /// Within a page, the cache structure looks like the Direct CWs
  //
  RCD_DIRECT_CW_CACHE           PageCache[MAX_RCD_CONTROL_WORDS_PER_PAGE];
} RCD_PAGED_CW_CACHE;

//
// Per-socket RCD cache structure
//
typedef struct {
  RCD_CW_CACHE_STATE            State;
  RCD_DIRECT_CW_CACHE           DirectCw[MAX_RCD_DIRECT_CONTROL_WORDS];
  RCD_PAGED_CW_CACHE            PagedCw[RCD_CW_PAGE_CACHE_LENGTH];
} SOCKET_RCD_CW_CACHE;

//
// Structure specifically for DDR5 RCD data that will be saved to NVRAM in DXE phase
// and, if available, restored from NVRAM at the start of MRC.
// The data is used to cache RCD control words in order to restore the DRAM to an operable
// state after a device reset during the JedecInit sequence or fast boot and S3 resume paths.
//
typedef struct {
  SOCKET_RCD_CW_CACHE              RcdCwCache[MAX_SOCKET];
} DDR5_RCD_NVDATA;


/**
  Obtain pointer to RCD CW cache for a socket

  @param[in]  Host    - Pointer to SysHost
  @param[in]  Socket  - Socket number

  @retval Pointer to DDR5_RCD_NVDATA.RcdCwCache[Socket]
**/
SOCKET_RCD_CW_CACHE
*GetRcdCwCache (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  );

#pragma pack(pop)

#endif // _MEM_RCD_CW_CACHE_H_
