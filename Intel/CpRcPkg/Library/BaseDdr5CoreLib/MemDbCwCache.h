/** @file
  MemDbCwCache.h
  Structures, definitions and function prototypes needed to cache
  DDR5 Registering Data Buffer (DB) Control Words

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

#ifndef  _MEM_DB_CW_CACHE_H_
#define  _MEM_DB_CW_CACHE_H_

#include <Memory/Ddr5DbRegs.h>
#include <Library/Ddr5CoreLib.h>
#include <Memory/Ddr5Cmds.h>
#include <Memory/Ddr5RcwRegs.h>
#include <Memory/JedecDefinitions.h>

#pragma pack(push, 1)


#define PBA_SELECT_ID_BITS                       0x0F

#if MAX_SOCKET <= 4
//
// The NVDATA structure for DDR5 RData Buffer (DB)
// The data stored here is used to cache control words and settings
// and to save them to NVRAM for use in the fast boot and resume paths.
//

//
// Direct Cws Cache List
//
#define DB_CW_DIRECT_CACHE_LIST \
    DDR5_DATA_BUFFER_RW81_REG,                 \
    DDR5_DATA_BUFFER_RW82_REG,                 \
    DDR5_DATA_BUFFER_RW83_REG,                 \
    DDR5_DATA_BUFFER_RW84_REG,                 \
    DDR5_DATA_BUFFER_RW85_REG,                 \
    DDR5_DATA_BUFFER_RW86_REG,                 \
    DDR5_DATA_BUFFER_RW87_REG,                 \
    DDR5_DATA_BUFFER_RW88_REG,                 \
    DDR5_DATA_BUFFER_RW8A_REG,                 \
    DDR5_DATA_BUFFER_RW8B_REG,                 \
    DDR5_DATA_BUFFER_RW8C_REG,                 \
    DDR5_DATA_BUFFER_RW8F_REG,                 \
    DDR5_DATA_BUFFER_RW92_REG,                 \
    DDR5_DATA_BUFFER_RW93_REG,                 \
    DDR5_DATA_BUFFER_RW97_REG,                 \
    DDR5_DATA_BUFFER_RWA0_REG,                 \
    DDR5_DATA_BUFFER_RWA1_REG,                 \
    DDR5_DATA_BUFFER_RWA2_REG,                 \
    DDR5_DATA_BUFFER_RWB0_REG,                 \
    RDIMM_RWDF

#define DB_CW_DIRECT_CACHE_LENGTH     20


//
// List of cached control word
//
#define PAGE0_CW_CACHE_LIST \
    LRDIMM_RWE0,                 \
    LRDIMM_RWE1,                 \
    LRDIMM_RWE2,                 \
    LRDIMM_RWE3,                 \
    LRDIMM_RWE4,                 \
    LRDIMM_RWE5,                 \
    LRDIMM_RWE6,                 \
    LRDIMM_RWE7,                 \
    LRDIMM_RWE8,                 \
    LRDIMM_RWE9,                 \
    LRDIMM_RWEA,                 \
    LRDIMM_RWEB,                 \
    LRDIMM_RWEC,                 \
    LRDIMM_RWED,                 \
    LRDIMM_RWEE,                 \
    LRDIMM_RWEF,                 \
    LRDIMM_RWF0,                 \
    LRDIMM_RWF1

#define PAGE0_CW_LIST_LENGTH    18

#define PAGE1_CW_CACHE_LIST \
    LRDIMM_RWE0,                 \
    LRDIMM_RWE1,                 \
    LRDIMM_RWE2,                 \
    LRDIMM_RWE3,                 \
    LRDIMM_RWE4,                 \
    LRDIMM_RWE5,                 \
    LRDIMM_RWE6,                 \
    LRDIMM_RWE7,                 \
    LRDIMM_RWE8,                 \
    LRDIMM_RWE9,                 \
    LRDIMM_RWEA,                 \
    LRDIMM_RWEB,                 \
    LRDIMM_RWEC,                 \
    LRDIMM_RWED,                 \
    LRDIMM_RWEE,                 \
    LRDIMM_RWEF,                 \
    LRDIMM_RWF0,                 \
    LRDIMM_RWF1

#define PAGE1_CW_LIST_LENGTH     18


#define PAGE2_CW_CACHE_LIST \
    LRDIMM_RWE0,                 \
    LRDIMM_RWE1,                 \
    LRDIMM_RWE2,                 \
    LRDIMM_RWE3,                 \
    LRDIMM_RWE4,                 \
    LRDIMM_RWE5,                 \
    LRDIMM_RWE6,                 \
    LRDIMM_RWE7,                 \
    LRDIMM_RWF0,                 \
    LRDIMM_RWF1,                 \
    LRDIMM_RWFA

#define PAGE2_CW_LIST_LENGTH     11


#define PAGE4_CW_CACHE_LIST \
    LRDIMM_RWE0,                 \
    LRDIMM_RWE1,                 \
    LRDIMM_RWE2,                 \
    LRDIMM_RWE3,                 \
    LRDIMM_RWE4,                 \
    LRDIMM_RWEA,                 \
    LRDIMM_RWEB,                 \
    LRDIMM_RWEC,                 \
    LRDIMM_RWE8,                 \
    LRDIMM_RWE9,                 \
    LRDIMM_RWF0,                 \
    LRDIMM_RWF1,                 \
    LRDIMM_RWF2,                 \
    LRDIMM_RWF3,                 \
    LRDIMM_RWF4,                 \
    LRDIMM_RWF8,                 \
    LRDIMM_RWF9,                 \
    LRDIMM_RWFA,                 \
    LRDIMM_RWFB,                 \
    LRDIMM_RWFC

#define PAGE4_CW_LIST_LENGTH    20


#define PAGE5_CW_CACHE_LIST \
    LRDIMM_RWE0,                 \
    LRDIMM_RWE1,                 \
    LRDIMM_RWE2,                 \
    LRDIMM_RWE3,                 \
    LRDIMM_RWE4,                 \
    LRDIMM_RWE8,                 \
    LRDIMM_RWE9,                 \
    LRDIMM_RWEA,                 \
    LRDIMM_RWEB,                 \
    LRDIMM_RWEC,                 \
    LRDIMM_RWF0,                 \
    LRDIMM_RWF1,                 \
    LRDIMM_RWF2,                 \
    LRDIMM_RWF3,                 \
    LRDIMM_RWF4,                 \
    LRDIMM_RWF9,                 \
    LRDIMM_RWFA,                 \
    LRDIMM_RWFB,                 \
    LRDIMM_RWFC,                 \
    LRDIMM_RWF8

#define PAGE5_CW_LIST_LENGTH     20

#define PAGE6_CW_CACHE_LIST \
    LRDIMM_RWE2,                 \
    LRDIMM_RWE3,                 \
    LRDIMM_RWE6,                 \
    LRDIMM_RWE7,                 \
    LRDIMM_RWEA,                 \
    LRDIMM_RWEB,                 \
    LRDIMM_RWEE,                 \
    LRDIMM_RWEF,                 \
    LRDIMM_RWF2,                 \
    LRDIMM_RWF3,                 \
    LRDIMM_RWF6,                 \
    LRDIMM_RWF7,                 \
    LRDIMM_RWFA,                 \
    LRDIMM_RWFB,                 \
    LRDIMM_RWFE,                 \
    LRDIMM_RWFF

#define PAGE6_CW_LIST_LENGTH    16

#define PAGE8_CW_CACHE_LIST \
    LRDIMM_RWE0,                 \
    LRDIMM_RWE1,                 \
    LRDIMM_RWE2,                 \
    LRDIMM_RWE3,                 \
    LRDIMM_RWE4,                 \
    LRDIMM_RWE5,                 \
    LRDIMM_RWE6,                 \
    LRDIMM_RWE7

#define PAGE8_CW_LIST_LENGTH   8

#define PAGE9_CW_CACHE_LIST \
    LRDIMM_RWE0,                 \
    LRDIMM_RWE1,                 \
    LRDIMM_RWE2,                 \
    LRDIMM_RWE3,                 \
    LRDIMM_RWE7,                 \
    LRDIMM_RWE8,                 \
    LRDIMM_RWE9,                 \
    LRDIMM_RWEA,                 \
    LRDIMM_RWEE,                 \
    LRDIMM_RWEF

#define PAGE9_CW_LIST_LENGTH      10

#define PAGEB_CW_CACHE_LIST \
    LRDIMM_RWE0,                 \
    LRDIMM_RWE1,                 \
    LRDIMM_RWE2,                 \
    LRDIMM_RWE3

#define PAGEB_CW_LIST_LENGTH     4

//
// Cache Data buffer Registers for Montage Data Buffer B0
//
#define PAGE83_CW_CACHE_LIST \
    LRDIMM_RWEE

#define PAGE83_CW_LIST_LENGTH     1

#define PAGECC_CW_CACHE_LIST \
    LRDIMM_RWE6

#define PAGECC_CW_LIST_LENGTH     1


typedef enum {
  DbCwRead,
  DbCwWrite,
  DbCwAccessTypeMax
}  DB_CW_ACCESS_TYPE;

//
// Per-socket cache state bits
//
typedef union {
  struct {
    UINT8 Initialized   : 1; // Bits[0]   - 1 = Cache has been initialized
    UINT8 Reserved      : 7; // Bits[7:1] - Reserved for future use.
  } Bits;
  UINT8 Data;
} DB_CW_CACHE_STATE;



//
// There is 1 instance of a CW per Ch/SubCh/Dimm/Strobe.
// There is 1 bit Valid falg Per Cw, DB_CW_INSTANCES_PER_SOCKET is number of bits needed for valid,
//
#define DB_CW_INSTANCES_PER_SOCKET (MAX_CH * SUB_CH * MAX_DIMM  * MAX_STROBES_ENUMRATE)

//
// SIZE_VALID_ARRAY* is the  number of Bytes needed for Valid
//
#define SIZE_VALID_ARRAY_DB_CW     ((DB_CW_INSTANCES_PER_SOCKET + (MAX_BITS_IN_BYTE - 1)) / MAX_BITS_IN_BYTE)

// Data buffer is x8 device, there is 5 strobe per sub channel
//
#define BUFFERS_PER_SUBCH         (MAX_STROBE / 4)
#define MAX_STROBE_BITS            3  // 5 Needs 3 Bits
#define MAX_STROBES_ENUMRATE       8  // 2^MAX_STROBE_BITS
//
// Definitions to index into the "Valid" bit array
//
//
#define VALID_BIT_CH_OFFSET_DB_CW            (SUB_CH * MAX_DIMM * MAX_STROBES_ENUMRATE)
#define VALID_BIT_SUB_CH_OFFSET_DB_CW        (MAX_STROBES_ENUMRATE * MAX_DIMM )
#define VALID_BIT_DIMM_OFFSET_DB_CW          (MAX_STROBES_ENUMRATE)
#define VALID_BIT_STROBE_OFFSET_DB_CW         1

//
// Single DB CW cache entry
// All DDR5 DB Control Words are 8-bits
//
typedef struct {
  UINT8                         Data;
} DB_CW_REGISTER;



//
// Caching DB Direct Access CW
//
typedef struct {

  DB_CW_REGISTER              Cache[MAX_CH][SUB_CH][MAX_DIMM][BUFFERS_PER_SUBCH];
  ///
  /// 1 Valid bit per cache entry
  //
  UINT8                        Valid[SIZE_VALID_ARRAY_DB_CW];
  ///
  /// Offset of Direct Cws
  //
  UINT8                        Offset;
} DB_CW_REG;

//
// Per-socket DB cache structure
// Direct Cws: 0x80~0xDF; Paged Cws: 0xE0~0xFF(page 0~0xffff)
// Direct Cws: Data buffer Control words which can be accessed directly
// Paged  Cws: Data buffer Control Words which can be accessed after set page number to LRDIMM_RWDF
// If you want to add a new Data buffer control words to a existed list:
// 1.Update address to that list
// 2.Change the list length
// if you want to add a new page y and its Control words to cache:
// 1.Define a new Cw list PAGEy_CW_CACHE_LIST, define PAGEY_CW_LIST_LENGTH
// 2.Add  "DB_CW_REG  PagedyCw[PAGEY_CW_LIST_LENGTH] into SOCKET_DB_CW struct
// 3.Add list PAGEy_CW_CACHE_LIST into DB_CW_INIT_OFFSET_LIST,
// 4.Add page number into DB_CW_PAGE_LIST
// 5.Add page case into function GetCwListPointer
// you need to put list into DB_CW_INIT_OFFSET_LIST by same page order with PagedxCw in SOCKET_DB_CW
//
typedef struct {
  DB_CW_CACHE_STATE     State;
  DB_CW_REG             DirectCw[DB_CW_DIRECT_CACHE_LENGTH];
  DB_CW_REG             Paged0Cw[PAGE0_CW_LIST_LENGTH];
  DB_CW_REG             Paged1Cw[PAGE1_CW_LIST_LENGTH];
  DB_CW_REG             Paged2Cw[PAGE2_CW_LIST_LENGTH];
  DB_CW_REG             Paged4Cw[PAGE4_CW_LIST_LENGTH];
  DB_CW_REG             Paged5Cw[PAGE5_CW_LIST_LENGTH];
  DB_CW_REG             Paged6Cw[PAGE6_CW_LIST_LENGTH];
  DB_CW_REG             Paged8Cw[PAGE8_CW_LIST_LENGTH];
  DB_CW_REG             Paged9Cw[PAGE9_CW_LIST_LENGTH];
  DB_CW_REG             PagedBCw[PAGEB_CW_LIST_LENGTH];
//
// Cache Data buffer Registers for Montage Data Buffer B0
//
  DB_CW_REG             Paged83Cw[PAGE83_CW_LIST_LENGTH];
  DB_CW_REG             PagedCCCw[PAGECC_CW_LIST_LENGTH];

} SOCKET_DB_CW;

//
// Please define CW lis by same order with SOCKET_DB_CW
//
//
#define DB_CW_INIT_OFFSET_LIST        \
    DB_CW_DIRECT_CACHE_LIST,    \
    PAGE0_CW_CACHE_LIST,        \
    PAGE1_CW_CACHE_LIST,        \
    PAGE2_CW_CACHE_LIST,        \
    PAGE4_CW_CACHE_LIST,        \
    PAGE5_CW_CACHE_LIST,        \
    PAGE6_CW_CACHE_LIST,        \
    PAGE8_CW_CACHE_LIST,        \
    PAGE9_CW_CACHE_LIST,        \
    PAGEB_CW_CACHE_LIST,        \
    PAGE83_CW_CACHE_LIST,       \
    PAGECC_CW_CACHE_LIST
//
// {DB_CW_PAGE_LIST}[0] is used to map DirectCw list {DB_CW_INIT_OFFSET_LIST}[0]
// Don't care value {DB_CW_PAGE_LIST}[0] for DirectCw
// Page number {DB_CW_INIT_OFFSET_LIST}[0] maps Paged control words list {DB_CW_INIT_OFFSET_LIST}[0]
//
#define DB_CW_PAGE_LIST \
    0,                                        \
    LRDIMM_DB_PAGE0,                          \
    LRDIMM_DB_PAGE1,                          \
    LRDIMM_DB_PAGE2,                          \
    LRDIMM_DB_PAGE4,                          \
    LRDIMM_DB_PAGE5,                          \
    LRDIMM_DB_PAGE6,                          \
    LRDIMM_DB_PAGE8,                          \
    LRDIMM_DB_PAGE9,                          \
    LRDIMM_DB_PAGEB,                          \
    LRDIMM_DB_PAGE83,                         \
    LRDIMM_DB_PAGECC

//
// Structure specifically for DDR5 DB data that will be saved to NVRAM in DXE phase
// and, if available, restored from NVRAM at the start of MRC.
// The data is used to cache DB control words in order to restore the DRAM to an operable
// state after a device reset during the JedecInit sequence or fast boot
//
typedef struct {
  SOCKET_DB_CW              DbCwCache[MAX_SOCKET];
} DDR5_DB_NVDATA;

#pragma pack(pop)

/**
  Obtain pointer to DB CW cache for a socket

  @param[in]  Host    - Pointer to SysHost
  @param[in]  Socket  - Socket number

  @retval Pointer to DDR5_DB_NVDATA.DbCwCache[Socket]
**/
SOCKET_DB_CW
*GetDbCwCache (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  );
#endif // MAX_SOCKET <= 4

#endif // _MEM_DB_CW_CACHE_H_
