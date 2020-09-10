/** @file
  MemDramAccess.h
  Structures, definitions and function prototypes needed to send commands to DDR5 DRAMs

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

#ifndef  _MEM_DRAM_ACCESS_H_
#define  _MEM_DRAM_ACCESS_H_

#include <ReferenceCodeDataTypes.h>
#include <Library/Ddr5CoreLib.h>
#include <Memory/Ddr5Cmds.h>
#include <Memory/Ddr5MrRegs.h>

#pragma pack(push, 1)

//
// Set to 1 to include caching for mode registers that are only used
// by x16 DRAMs
//
#define X16_DEVICE_CACHE_SUPPORTED  0

//
// The NVDATA structure for DDR5 DRAM
// The data stored here is used to cache registers and settings and to save
// them to NVRAM for use in the fast boot and resume paths.
//
#define MRW_DEVICE_CACHE_LIST_X4X8X16 \
          DDR5_MODE_REGISTER_3_REG,                                  \
          DDR5_MODE_REGISTER_10_REG,                                 \
          DDR5_MODE_REGISTER_37_REG,                                 \
          DDR5_MODE_REGISTER_43_REG,                                 \
          DDR5_MODE_REGISTER_44_REG,                                 \
          DDR5_MODE_REGISTER_128_REG,   /* 0x80 - DFE DQL0 - Gain */ \
          DDR5_MODE_REGISTER_129_REG,   /* 0x81 - DFE DQL0 - TAP1 */ \
          DDR5_MODE_REGISTER_130_REG,   /* 0x82 - DFE DQL0 - TAP2 */ \
          DDR5_MODE_REGISTER_131_REG,   /* 0x83 - DFE DQL0 - TAP3 */ \
          DDR5_MODE_REGISTER_132_REG,   /* 0x84 - DFE DQL0 - TAP4 */ \
          DDR5_MODE_REGISTER_136_REG,   /* 0x88 - DFE DQL1 - Gain */ \
          DDR5_MODE_REGISTER_137_REG,   /* 0x89 - DFE DQL1 - TAP1 */ \
          DDR5_MODE_REGISTER_138_REG,   /* 0x8A - DFE DQL1 - TAP2 */ \
          DDR5_MODE_REGISTER_139_REG,   /* 0x8B - DFE DQL1 - TAP3 */ \
          DDR5_MODE_REGISTER_140_REG,   /* 0x8C - DFE DQL1 - TAP4 */ \
          DDR5_MODE_REGISTER_144_REG,   /* 0x90 - DFE DQL2 - Gain */ \
          DDR5_MODE_REGISTER_145_REG,   /* 0x91 - DFE DQL2 - TAP1 */ \
          DDR5_MODE_REGISTER_146_REG,   /* 0x92 - DFE DQL2 - TAP2 */ \
          DDR5_MODE_REGISTER_147_REG,   /* 0x93 - DFE DQL2 - TAP3 */ \
          DDR5_MODE_REGISTER_148_REG,   /* 0x94 - DFE DQL2 - TAP4 */ \
          DDR5_MODE_REGISTER_152_REG,   /* 0x98 - DFE DQL3 - Gain */ \
          DDR5_MODE_REGISTER_153_REG,   /* 0x99 - DFE DQL3 - TAP1 */ \
          DDR5_MODE_REGISTER_154_REG,   /* 0x9A - DFE DQL3 - TAP2 */ \
          DDR5_MODE_REGISTER_155_REG,   /* 0x9B - DFE DQL3 - TAP3 */ \
          DDR5_MODE_REGISTER_156_REG    /* 0x9C - DFE DQL3 - TAP4 */

#define   MRW_DEVICE_CACHE_X4X8X16_LENGTH  25

#define MRW_DEVICE_CACHE_LIST_X8X16 \
          DDR5_MODE_REGISTER_160_REG,   /* 0xA0 - DFE DQL4 - Gain */ \
          DDR5_MODE_REGISTER_161_REG,   /* 0xA1 - DFE DQL4 - TAP1 */ \
          DDR5_MODE_REGISTER_162_REG,   /* 0xA2 - DFE DQL4 - TAP2 */ \
          DDR5_MODE_REGISTER_163_REG,   /* 0xA3 - DFE DQL4 - TAP3 */ \
          DDR5_MODE_REGISTER_164_REG,   /* 0xA4 - DFE DQL4 - TAP4 */ \
          DDR5_MODE_REGISTER_168_REG,   /* 0xA8 - DFE DQL5 - Gain */ \
          DDR5_MODE_REGISTER_169_REG,   /* 0xA9 - DFE DQL5 - TAP1 */ \
          DDR5_MODE_REGISTER_170_REG,   /* 0xAA - DFE DQL5 - TAP2 */ \
          DDR5_MODE_REGISTER_171_REG,   /* 0xAB - DFE DQL5 - TAP3 */ \
          DDR5_MODE_REGISTER_172_REG,   /* 0xAC - DFE DQL5 - TAP4 */ \
          DDR5_MODE_REGISTER_176_REG,   /* 0xB0 - DFE DQL6 - Gain */ \
          DDR5_MODE_REGISTER_177_REG,   /* 0xB1 - DFE DQL6 - TAP1 */ \
          DDR5_MODE_REGISTER_178_REG,   /* 0xB2 - DFE DQL6 - TAP2 */ \
          DDR5_MODE_REGISTER_179_REG,   /* 0xB3 - DFE DQL6 - TAP3 */ \
          DDR5_MODE_REGISTER_180_REG,   /* 0xB4 - DFE DQL6 - TAP4 */ \
          DDR5_MODE_REGISTER_184_REG,   /* 0xB8 - DFE DQL7 - Gain */ \
          DDR5_MODE_REGISTER_185_REG,   /* 0xB9 - DFE DQL7 - TAP1 */ \
          DDR5_MODE_REGISTER_186_REG,   /* 0xBA - DFE DQL7 - TAP2 */ \
          DDR5_MODE_REGISTER_187_REG,   /* 0xBB - DFE DQL7 - TAP3 */ \
          DDR5_MODE_REGISTER_188_REG    /* 0xBC - DFE DQL7 - TAP4 */

#define   MRW_DEVICE_CACHE_X8X16_LENGTH  20

#define MRW_DEVICE_CACHE_LIST_X16 \
          DDR5_MODE_REGISTER_192_REG,   /* 0xC0 - DFE DQU0 - Gain */ \
          DDR5_MODE_REGISTER_193_REG,   /* 0xC1 - DFE DQU0 - TAP1 */ \
          DDR5_MODE_REGISTER_194_REG,   /* 0xC2 - DFE DQU0 - TAP2 */ \
          DDR5_MODE_REGISTER_195_REG,   /* 0xC3 - DFE DQU0 - TAP3 */ \
          DDR5_MODE_REGISTER_196_REG,   /* 0xC4 - DFE DQU0 - TAP4 */ \
          DDR5_MODE_REGISTER_200_REG,   /* 0xC8 - DFE DQU1 - Gain */ \
          DDR5_MODE_REGISTER_201_REG,   /* 0xC9 - DFE DQU1 - TAP1 */ \
          DDR5_MODE_REGISTER_202_REG,   /* 0xCA - DFE DQU1 - TAP2 */ \
          DDR5_MODE_REGISTER_203_REG,   /* 0xCB - DFE DQU1 - TAP3 */ \
          DDR5_MODE_REGISTER_204_REG,   /* 0xCC - DFE DQU1 - TAP4 */ \
          DDR5_MODE_REGISTER_208_REG,   /* 0xD0 - DFE DQU2 - Gain */ \
          DDR5_MODE_REGISTER_209_REG,   /* 0xD1 - DFE DQU2 - TAP1 */ \
          DDR5_MODE_REGISTER_210_REG,   /* 0xD2 - DFE DQU2 - TAP2 */ \
          DDR5_MODE_REGISTER_211_REG,   /* 0xD3 - DFE DQU2 - TAP3 */ \
          DDR5_MODE_REGISTER_212_REG,   /* 0xD4 - DFE DQU2 - TAP4 */ \
          DDR5_MODE_REGISTER_216_REG,   /* 0xD8 - DFE DQU3 - Gain */ \
          DDR5_MODE_REGISTER_217_REG,   /* 0xD9 - DFE DQU3 - TAP1 */ \
          DDR5_MODE_REGISTER_218_REG,   /* 0xDA - DFE DQU3 - TAP2 */ \
          DDR5_MODE_REGISTER_219_REG,   /* 0xDB - DFE DQU3 - TAP3 */ \
          DDR5_MODE_REGISTER_220_REG,   /* 0xDC - DFE DQU3 - TAP4 */ \
          DDR5_MODE_REGISTER_224_REG,   /* 0xE0 - DFE DQU4 - Gain */ \
          DDR5_MODE_REGISTER_225_REG,   /* 0xE1 - DFE DQU4 - TAP1 */ \
          DDR5_MODE_REGISTER_226_REG,   /* 0xE2 - DFE DQU4 - TAP2 */ \
          DDR5_MODE_REGISTER_227_REG,   /* 0xE3 - DFE DQU4 - TAP3 */ \
          DDR5_MODE_REGISTER_228_REG,   /* 0xE4 - DFE DQU4 - TAP4 */ \
          DDR5_MODE_REGISTER_232_REG,   /* 0xE8 - DFE DQU5 - Gain */ \
          DDR5_MODE_REGISTER_233_REG,   /* 0xE9 - DFE DQU5 - TAP1 */ \
          DDR5_MODE_REGISTER_234_REG,   /* 0xEA - DFE DQU5 - TAP2 */ \
          DDR5_MODE_REGISTER_235_REG,   /* 0xEB - DFE DQU5 - TAP3 */ \
          DDR5_MODE_REGISTER_236_REG,   /* 0xEC - DFE DQU5 - TAP4 */ \
          DDR5_MODE_REGISTER_240_REG,   /* 0xF0 - DFE DQU6 - Gain */ \
          DDR5_MODE_REGISTER_241_REG,   /* 0xF1 - DFE DQU6 - TAP1 */ \
          DDR5_MODE_REGISTER_242_REG,   /* 0xF2 - DFE DQU6 - TAP2 */ \
          DDR5_MODE_REGISTER_243_REG,   /* 0xF3 - DFE DQU6 - TAP3 */ \
          DDR5_MODE_REGISTER_244_REG,   /* 0xF4 - DFE DQU6 - TAP4 */ \
          DDR5_MODE_REGISTER_248_REG,   /* 0xF8 - DFE DQU7 - Gain */ \
          DDR5_MODE_REGISTER_249_REG,   /* 0xF9 - DFE DQU7 - TAP1 */ \
          DDR5_MODE_REGISTER_250_REG,   /* 0xFA - DFE DQU7 - TAP2 */ \
          DDR5_MODE_REGISTER_251_REG,   /* 0xFB - DFE DQU7 - TAP3 */ \
          DDR5_MODE_REGISTER_252_REG    /* 0xFC - DFE DQU7 - TAP4 */

#define   MRW_DEVICE_CACHE_X16_LENGTH  40

#ifdef USE_LATEST_DRAM_SPEC

#define MRW_RANK_CACHE_LIST \
          DDR5_MODE_REGISTER_0_REG,   \
          DDR5_MODE_REGISTER_2_REG,   \
          DDR5_MODE_REGISTER_4_REG,   \
          DDR5_MODE_REGISTER_5_REG,   \
          DDR5_MODE_REGISTER_6_REG,   \
          DDR5_MODE_REGISTER_8_REG,   \
          DDR5_MODE_REGISTER_9_REG,   \
          DDR5_MODE_REGISTER_12_REG,  \
          DDR5_MODE_REGISTER_14_REG,  \
          DDR5_MODE_REGISTER_25_REG,  \
          DDR5_MODE_REGISTER_26_REG,  \
          DDR5_MODE_REGISTER_27_REG,  \
          DDR5_MODE_REGISTER_28_REG,  \
          DDR5_MODE_REGISTER_29_REG,  \
          DDR5_MODE_REGISTER_30_REG,  \
          DDR5_MODE_REGISTER_33_REG,  \
          DDR5_MODE_REGISTER_34_REG,  \
          DDR5_MODE_REGISTER_35_REG,  \
          DDR5_MODE_REGISTER_37_REG,  \
          DDR5_MODE_REGISTER_40_REG,  \
          DDR5_MODE_REGISTER_42_REG,  \
          DDR5_MODE_REGISTER_45_REG,  \
          DDR5_MODE_REGISTER_50_REG,  \
          DDR5_MODE_REGISTER_111_REG, \
          DDR5_MODE_REGISTER_112_REG

#define   MRW_RANK_CACHE_LENGTH    25

#else //USE_LATEST_DRAM_SPEC

#define MRW_RANK_CACHE_LIST \
          DDR5_MODE_REGISTER_0_REG,   \
          DDR5_MODE_REGISTER_2_REG,   \
          DDR5_MODE_REGISTER_3_REG,   \
          DDR5_MODE_REGISTER_4_REG,   \
          DDR5_MODE_REGISTER_5_REG,   \
          DDR5_MODE_REGISTER_6_REG,   \
          DDR5_MODE_REGISTER_8_REG,   \
          DDR5_MODE_REGISTER_9_REG,   \
          DDR5_MODE_REGISTER_12_REG,  \
          DDR5_MODE_REGISTER_14_REG,  \
          DDR5_MODE_REGISTER_25_REG,  \
          DDR5_MODE_REGISTER_26_REG,  \
          DDR5_MODE_REGISTER_27_REG,  \
          DDR5_MODE_REGISTER_28_REG,  \
          DDR5_MODE_REGISTER_29_REG,  \
          DDR5_MODE_REGISTER_30_REG,  \
          DDR5_MODE_REGISTER_33_REG,  \
          DDR5_MODE_REGISTER_34_REG,  \
          DDR5_MODE_REGISTER_35_REG,  \
          DDR5_MODE_REGISTER_37_REG,  \
          DDR5_MODE_REGISTER_40_REG,  \
          DDR5_MODE_REGISTER_45_REG,  \
          DDR5_MODE_REGISTER_50_REG,  \
          DDR5_MODE_REGISTER_112_REG

#define   MRW_RANK_CACHE_LENGTH    25
#endif //USE_LATEST_DRAM_SPEC

//
// Per-socket cache state bits
//
typedef union {
  struct {
    UINT8 Initialized   : 1; // Bits[0]   - 1 = Cache has been initialized
    UINT8 Reserved      : 7; // Bits[7:1] - Reserved for future use.
  } Bits;
  UINT8 Data;
} MRW_CACHE_STATE;

//
// Single MRW cache entry
// All DDR5 Mode Registers are 8-bits
//
typedef struct {
  UINT8                         Data;
} MODE_REGISTER_CACHE;

//
// MODE_REGISTER_INSTANCES_PER_SOCKET is the number of instances in the cache for
// a single mode register for the entire socket. This assumes there is 1 instance
// of a mode register per Ch/SubCh/Dimm/Rank.
// This number, therefore, does not account for per-device (PDA) instances.
//
// The number is used to define the size of the Valid structure, which has 1 bit
// per MR instance for the entire socket.
// Because the number does not include per-device (PDA) instances, an assumption
// is made here that if any PDA instance in a rank is valid, all instances are valid.
//
// SIZE_VALID_ARRAY converts the number of instances into the number of bytes needed
// to store 1 bit per instance, rounding up to ensure all bits are included.
//
#define MODE_REGISTER_INSTANCES_PER_SOCKET (MAX_CH * SUB_CH * MAX_DIMM * MAX_RANK_DIMM)
#define SIZE_VALID_ARRAY ((MODE_REGISTER_INSTANCES_PER_SOCKET + (MAX_BITS_IN_BYTE - 1)) / MAX_BITS_IN_BYTE)

//
// Definitions to index into the "Valid" bit array
//
#define VALID_BIT_CH_OFFSET     (SUB_CH * MAX_DIMM * MAX_RANK_DIMM)
#define VALID_BIT_SUB_CH_OFFSET (MAX_DIMM * MAX_RANK_DIMM)
#define VALID_BIT_DIMM_OFFSET   (MAX_RANK_DIMM)
#define VALID_BIT_RANK_OFFSET   1

//
// Caching MRW per device- x4, x8 and x16 devices
//
typedef struct {
  ///
  /// The address of the mode register
  //
  UINT8                         Address;
  ///
  /// Some registers are used by all device types - x4, x8 and x16
  /// Caching of these registers is needed for all strobes 0-9
  //
  MODE_REGISTER_CACHE           Cache[MAX_CH][SUB_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_STROBE / 2];
  ///
  /// 1 Valid bit per instance, with all devices counted as a single instance
  //
  UINT8                         Valid[SIZE_VALID_ARRAY];
} MRW_CACHE_BY_DEVICE_X4X8X16;

//
// Caching MRW per device - x8 and x16 devices only
//
typedef struct {
  ///
  /// The address of the mode register
  //
  UINT8                         Address;
  ///
  /// Some registers are only used by x8 and x16 devices
  /// Caching of these registers is needed only for strobes 0-4
  //
  MODE_REGISTER_CACHE           Cache[MAX_CH][SUB_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_STROBE / 4];
  ///
  /// 1 Valid bit per instance, with all devices counted as a single instance
  //
  UINT8                         Valid[SIZE_VALID_ARRAY];
} MRW_CACHE_BY_DEVICE_X8X16;

//
// Caching MRW per device - x16 devices only
//
typedef struct {
  ///
  /// The address of the mode register
  //
  UINT8                         Address;
  ///
  /// Reserve 1 byte for each instance of the mode register
  /// Some registers are only used by x16 devices
  /// Caching of these registers is needed only for strobes 0-4
  //
  MODE_REGISTER_CACHE           Cache[MAX_CH][SUB_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_STROBE / 4];
  ///
  /// 1 Valid bit per instance, with all devices counted as a single instance
  //
  UINT8                         Valid[SIZE_VALID_ARRAY];
} MRW_CACHE_BY_DEVICE_X16;

typedef struct {
#if defined (X16_DEVICE_CACHE_SUPPORTED) && (X16_DEVICE_CACHE_SUPPORTED == 1)
  MRW_CACHE_BY_DEVICE_X16       Devicesx16[MRW_DEVICE_CACHE_X16_LENGTH];
#endif // X16_DEVICE_CACHE_SUPPORTED
  MRW_CACHE_BY_DEVICE_X8X16     Devicesx8x16[MRW_DEVICE_CACHE_X8X16_LENGTH];
  MRW_CACHE_BY_DEVICE_X4X8X16   Devicesx4x8x16[MRW_DEVICE_CACHE_X4X8X16_LENGTH];
} MRW_CACHE_BY_DEVICE;

//
// Caching MRW per rank
//
typedef struct {
  ///
  /// The address of the mode register
  //
  UINT8                         Address;
  ///
  /// Reserve 1 byte for each instance of the mode register
  //
  MODE_REGISTER_CACHE           Cache[MAX_CH][SUB_CH][MAX_DIMM][MAX_RANK_DIMM];
  ///
  /// 1 Valid bit per cache entry
  //
  UINT8                         Valid[SIZE_VALID_ARRAY];
} MRW_CACHE_BY_RANK;

//
// Per-socket MRW cache structure
//
typedef struct {
  MRW_CACHE_STATE               State;
  MRW_CACHE_BY_RANK             ByRank[MRW_RANK_CACHE_LENGTH];
  MRW_CACHE_BY_DEVICE           ByDevice;
} SOCKET_MRW_CACHE;

//
// Per-device CA and CS Vref storage
//
typedef struct {
  UINT16                        CsVref[MAX_CH][SUB_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_STROBE / 2];
  UINT16                        CaVref[MAX_CH][SUB_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_STROBE / 2];
} SOCKET_DRAM_VREF;

#define GET_DRAM_VREF     0
#define SAVE_DRAM_VREF    1

//
// Structure specifically for DDR5 DRAM data that will be saved to NVRAM in DXE phase
// and, if available, restored from NVRAM at the start of MRC.
// The data is used to cache DRAM registers in order to restore the DRAM to an operable
// state after a device reset during the JedecInit sequence or fas boot and S3 resume paths.
//
typedef struct {
  SOCKET_MRW_CACHE              MrwCache[MAX_SOCKET];
  SOCKET_DRAM_VREF              DramVref[MAX_SOCKET];
} DRAM_NVDATA;


/**
  Obtain pointer to DRAM MRW cache for a socket

  @param[in]  Host    - Pointer to SysHost
  @param[in]  Socket  - Socket number

  @retval Pointer to DDR5_DRAM_MRW_NVDATA.MrwCache[Socket]
**/
SOCKET_MRW_CACHE
*GetMrwCache (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  );

/**

  Save DRAM CS VREF setting to NVRAM

  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (socket-based)
  @param[in] SubChMask  - Mask of subchannel numbers to be updated
  @param[in] Dimm       - DIMM number (Channel-based)
  @param[in] Rank       - Rank number (DIMM-based)
  @param[in] Dram       - Dram number in the subchannel
  @param[in] CsVref     - Data to save

  @retval MRC_STATUS_SUCCESS if operation is successful
  @retval MRC_STATUS_FAILURE otherwise
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
SaveDramVrefCs (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Dram,
  IN UINT16   CsVref
  );

/**

  Return the saved DRAM CS VREF setting to NVRAM

  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (socket-based)
  @param[in] SubChMask  - Mask of subchannel numbers to be updated
  @param[in] Dimm       - DIMM number (Channel-based)
  @param[in] Rank       - Rank number (DIMM-based)
  @param[in] Dram       - Dram number in the subchannel
  @param[in] CsVref     - Pointer to caller's byte

  @retval MRC_STATUS_SUCCESS if operation is successful
  @retval MRC_STATUS_FAILURE otherwise
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
GetSavedDramVrefCs (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Dram,
  IN UINT16   *CsVref
  );

/**

  Save DRAM CA VREF setting to NVRAM

  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (socket-based)
  @param[in] SubChMask  - Mask of subchannel numbers to be updated
  @param[in] Dimm       - DIMM number (Channel-based)
  @param[in] Rank       - Rank number (DIMM-based)
  @param[in] Dram       - Dram number in the subchannel
  @param[in] CaVref     - Data to save

  @retval MRC_STATUS_SUCCESS if operation is successful
  @retval MRC_STATUS_FAILURE otherwise
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
SaveDramVrefCa (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Dram,
  IN UINT16   CaVref
  );

/**

  Return the saved DRAM CA VREF setting to NVRAM

  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (socket-based)
  @param[in] SubChMask  - Mask of subchannel numbers to be updated
  @param[in] Dimm       - DIMM number (Channel-based)
  @param[in] Rank       - Rank number (DIMM-based)
  @param[in] Dram       - Dram number in the subchannel
  @param[in] CaVref     - Pointer to caller's byte

  @retval MRC_STATUS_SUCCESS if operation is successful
  @retval MRC_STATUS_FAILURE otherwise
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
GetSavedDramVrefCa (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Dram,
  IN UINT16   *CaVref
  );

/**
  Restore DRAM mode registers from cache

  @param[in]  Host       - Pointer to SysHost
  @param[in]  Socket     - Socket number
  @param[in]  Channel    - Channel number (socket-based)

  @retval N/A

**/
VOID
RestoreDramModeRegisters (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  );

/**
  Display Mode Register contents by reading directly using MRR command

  @param[in]  Socket      - Socket number

  @retval   N/A
**/
VOID
EFIAPI
DumpDramModeRegisters (
  IN  UINT8     Socket
  );

#pragma pack(pop)

#endif // _MEM_DRAM_ACCESS_H_
