/** @file
  Interface header file for the HBM library class.

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

#ifndef _HBM_CORE_JEDEC_H_
#define _HBM_CORE_JEDEC_H_


//
// Convert from nCK to MRS value
//
#define HBM_WR_LATENCY_NCK2MRS(x)                 (x-1)
#define HBM_RD_LATENCY_NCK2MRS(x)                 (x-2)
//
// Convert from MRS to nCK value (HCLK)
//
#define HBM_WR_LATENCY_MRS2NCK(x)                 (x+1)
#define HBM_RD_LATENCY_MRS2NCK(x)                 (x+2)

//
// IEEE 1500 Test Access Ports Commands (JEDEC )
//

// WIR Channel Selection Definition HBM JEDEC spec rev 1.22 Table 75
#define WIR_CHANNEL_A                             0x0
#define WIR_CHANNEL_B                             0x1
#define WIR_CHANNEL_C                             0x2
#define WIR_CHANNEL_D                             0x3
#define WIR_CHANNEL_E                             0x4
#define WIR_CHANNEL_F                             0x5
#define WIR_CHANNEL_G                             0x6
#define WIR_CHANNEL_H                             0x7
// RESERVED WIR 0x8h to 0xDh
#define WIR_CHANNEL_NONE                          0xE
#define WIR_CHANNEL_ALL                           0xF

// Instruction Register Encodings HBM JEDEC spec rev 1.22 Table 76
#define BYPASS                                    0x0  // Bypass
#define EXTEST_RX                                 0x1  // Microbump boundary scan RX test (open/short)
#define EXTEST_TX                                 0x2  // Microbump boundary scan TX test (open/short)
#define INTEST_RX                                 0x3  // Vendor INTEST for HBM inputs
#define INTEST_TX                                 0x4  // Vendor INTEST for HBM outputs
#define HBM_RESET                                 0x5  // Update initiates DRAM functioanl RESET
                                                       // exluding Wrapper Data Registers (WDRs) and any
                                                       // IEEE std 1500 logic or IOs
#define MBIST                                     0x6  // DRAM resident Memory MBIST engine test
#define SOFT_REPAIR                               0x7  // Soft repair of failing DRAM bit cell
#define HARD_REPAIR                               0x8  // Hard repair of DRAM failing DRAM bit cell
#define DWORD_MISR                                0x9  // Read back for DWORD MISR & optionally write
                                                       // a seed value
#define AWORD_MISR                                0xA  // Read back for AWORD MISR
#define CHANNEL_ID                                0xB  // All TX IOs go high (except MID stack region)
#define MISR_MASK                                 0xC  // Mask MISR bit(s)
#define AWORD_MISR_CONFIG                         0xD  // Allows IEEE std 1500 access to configuration
                                                       // of the AWORD MISR test feature setup
#define DEVICE_ID                                 0xE  // Returns the DRAM's unique identification code
#define TEMPERATURE                               0xF  // Returns an 8-bit binary temperature code
#define MODE_REG_DUMP_SET                         0x10 // Returns and set the DRAM's mode register values
#define READ_LFSR_COMP_STICKY                     0x11  // Reads the sticky bit error for LFSR compare
#define SOFT_LANE_REPAIR                          0x12 // Soft lane Remapping
#define HARD_LANE_REPAIR                          0x13 // Hard lane Remapping
#define RESERVE                                   0x14 // 14h to 7Fh
#define VENDOR_SPECIFIC                           0x80 // 80h to FFh

#define PAYLOAD_BYPASS                            1   // Bypass payload length
#define PAYLOAD_EXTTEST_RX                        215 // EXTEST_RX payload length
#define PAYLOAD_EXTTEST_TX                        215 // EXTEST_TX payload length
#define PAYLOAD_HBM_RESET                         1   // HBM_RESET payload length
#define PAYLOAD_DWORD_MISR                        320 // DWORD_MISR payload length
#define PAYLOAD_AWORD_MISR                        30  // AWORD_MISR payload length
#define PAYLOAD_CHANNEL_ID                        1   // CHANNEL_ID payload length
#define PAYLOAD_AWORD_MISR_CONFIG                 8   // AWORD_MISR_CONFIG  payload length
#define PAYLOAD_DEVICE_ID                         82  // DEVICE_ID  payload length
#define PAYLOAD_TEMPERATURE                       8   // TEMPERATURE  payload length
#define PAYLOAD_MODE_REGISTER_DUMP                128 // TEMPERATURE  payload length
#define PAYLOAD_READ_LFSR_COMP                    175 // READ_LFSR_COMP_STICKY  payload length
#define PAYLOAD_SOFT_LANE_REPAIR                  72  // SOFT_LANE_REPAIR  payload length
#define PAYLOAD_HARD_LANE_REPAIR                  72  // HARD_LANE_REPAIR  payload length

#define HBM_VID_SAMSUNG                           1       // Samsung manufacturer ID
#define HBM_VID_SKHYNIX                           6       // SK Hynix manufacturer ID
#define HBM_VID_MICRON                            15      // Micron manufacturer ID

//
// HBM tCK (CK clock period)
//
#define HBM_TCK_PSECS                             625   // pico seconds

//
// HBM tREFI (Average periodic refresh interval for refresh command)
//
#define HBM_TREFI_NSECS                           3900  // nano seconds
#define HBM_TREFI_DCLKS                           (HBM_TREFI_NSECS * 1000 / HBM_TCK_PSECS)

//
// HBM tRFCSB (Single bank refresh command period - same bank)
//
#define HBM_TRFCSB_NSECS                          160  // nano seconds
#define HBM_TRFCSB_DCLKS                          (HBM_TRFCSB_NSECS * 1000 / HBM_TCK_PSECS)

//
// HBM JEDEC specification timings
//
typedef enum {
  HbmTccdl = 0,
  HbmTccds,
  HbmTccdr,
  HbmTwtrl,
  HbmTwtrs,
  HbmTrcdrd,
  HbmTrcdwr,
  HbmTrrd_l,
  HbmTrrd_s,
  HbmTfaw,
  HbmTrtpl,
  HbmTrtps,
  HbmTwr,
  HbmTrp,
  HbmTrc,
  HbmTras,
  HbmMaxJedecTiming
} HBM_JEDEC_TIMING;

//
// HBM Density Bit field in DEVICE_ID Wrapper Data register from HBM JEDEC Spec rev 3.30 Table 99
// It indicates memory density per HBM channel
//
typedef enum {
  Hbm1Gb         = 0x1,             ///< 1 - 1 Gb
  Hbm2Gb         = 0x2,             ///< 2 - 2 Gb
  Hbm4Gb         = 0x3,             ///< 3 - 4 Gb
  Hbm8Gb8H       = 0x4,             ///< 4 - 8 Gb (8 Gb 8-High)
  Hbm6Gb         = 0x5,             ///< 5 - 6 Gb
  Hbm8Gb         = 0x6,             ///< 6 - 8 Gb
  Hbm12Gb8H      = 0x8,             ///< 8 - 12 Gb (12 Gb 8-High)
  Hbm8Gb12H      = 0x9,             ///< 9 - 12 Gb (8 Gb 12-High)
  Hbm16Gb8H      = 0xA,             ///< 10 - 16 Gb (16 Gb 8-High)
  Hbm12Gb12H     = 0xB,             ///< 11 - 18 Gb (12 Gb 12-High)
  Hbm16Gb12H     = 0xC,             ///< 11 - 24 Gb (16 Gb 12-High)
  HbmDensityMax                     ///< Maximum number of elements in this enum
} HBM_DENSITY;

//
// Column Address width
//
#define CA_WIDTH_HBM_6    0  // 00. DDR4 : 10 bits; DDR5 : 10 bits HBM : 6 bits.
#define CA_WIDTH_DDR5_11  1  // 01 - DDR5 : 11 bits;
// Others - reserved. 10-11 : Reserved

//
// Row Address width
//
#define RA_WIDTH_13  1
#define RA_WIDTH_14  2
#define RA_WIDTH_15  3
#define RA_WIDTH_16  4
#define RA_WIDTH_17  5
#define RA_WIDTH_18  6

#define DDR3_DNSTY_2Gb  1
#define DDR3_DNSTY_4Gb  2
#define DDR3_DNSTY_8Gb  3
#define DDR3_DNSTY_16Gb 4
#define DDR3_DNSTY_12Gb 5
#define DDR3_DNSTY_24Gb 6
#define DDR3_DNSTY_32Gb 7

#define DDR3_WIDTH_x4   0
#define DDR3_WIDTH_x8   1
#define DDR3_WIDTH_x16  2

#define RANK_DISABLE_NO 0  // not disable rank

#define BA_SHRINK_NORMAL  0
#define BA_SHRINK_HBM_BA1 1

#define RANK_CNT_SR  0
#define RANK_CNT_DR  1
#define RANK_CNT_QR  2

//
// Number of sub ranks per chip select
//
#define SUBRANK_CS_1R      0x0
#define SUBRANK_CS_2R      0x1
#define SUBRANK_CS_4R      0x2
#define SUBRANK_CS_8R      0x3

//
// Number of banks
//
#define NUMBER_OF_BANKS_8   0
#define NUMBER_OF_BANKS_16  1
#define NUMBER_OF_BANKS_32  2
#define NUMBER_OF_BANKS_48  3

//
// addresing_mode
//
#define HBM_PSEUDO_CHANNEL_MODE  1
#define HBM_LEGACY_MODE          2

//
// Structure that for HBM MTR
//
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 ca_width : 2;        // Follow the defination in DIMMMTR
    UINT8 ra_width : 3;
    UINT8 number_of_banks : 2; // Follow the defination in SCHEDULER_ENABLES_BS
    UINT8 rsvd : 1;
  } Bits;
  UINT8 Data;

} DIMMMTR_DATA_STRUCT;

typedef struct {
  HBM_DENSITY               HbmDensity;
  DIMMMTR_DATA_STRUCT       DimmMtr;
} HBM_DIMM_MTR_ENCODINGS;

//
// Struct format for DEVICE_ID Wrapper Data Register from HBM JEDEC Spec rev 3.30 Table 99
//
typedef union {
  struct {
    //
    // Fields in Data[0]:
    //
    UINT32 model_part_num : 7;
    /**< Model Part Number - Bits [6:0],
    Vendor Reserved
    */
    UINT32 hbm_stack_height : 1;
    /**< HBM Stack Height - Bits [7],
    0 - 2 or 4 High Stack
    1 - 8 - High Stack
    */
    UINT32 channel_available : 8;
    /**< Channel Available - Bits [15:8],
    0 - Channel not present / not working
    1 - Channel present / working
    Channel encoding (1 bit per channel)
    [0]: channel a
    [1]: channel b
    ...
    [6]: channel g
    [7]: channel h
    */
    UINT32 addresing_mode : 2;
    /**< Addressing Mode Support - Bits [17:16],
    01 - Only Pseudo Channel Mode Supported
    10 - Only Legacy Mode Supported
    00 - Illegal
    11 - Illegal
    */
    UINT32 serial_number_low : 14;
    /**< Serial Number Unique ID per device - Bits [31:18]
    ID per device
    */

    //
    // Fields in Data[1]:
    //
    UINT32 serial_number_hi : 20;
    /**< Serial Number Unique ID per device - Bits [51:32]
    ID per device
    */
    UINT32 manufacturing_week : 8;
    /**< MANUFACTURING_WEEK - Bits [59:52],
    Binary encoded week
    WW52 = 8'b 00110100
    */
    UINT32 manufacturing_year_low : 4;
    /**< MANUFACTURING_YEAR - Bits [63:60],
    Binary encoded year from 2011
    2011 = 8'b 00000000; 2015=8'b 00000100
    */

    //
    // Fields in Data[2]:
    //
    UINT32 manufacturing_year_hi : 4;
    /**< MANUFACTURING_YEAR - Bits [67:64],
    Binary encoded year from 2011
    2011 = 8'b 00000000; 2015=8'b 00000100
    */
    UINT32 manufacturing_loc : 4;
    /**< MANUFACTURING_LOCATION - Bits [71:68],
    Vendor specific
    */
    UINT32 manufacturer_id : 4;
    /**< MANUFACTURER ID - Bits [75:72],
    0000 - Reserved
    0001 - Samsung
    0010 - Reserved
    0011 - Reserved
    0100 - Reserved
    0101 - Reserved
    0110 - SK Hynix
    0111 ... 1110 - Reserved
    1111 - Micron
    */
    UINT32 density : 4;
    /**< DENSITY ID - Bits [79:76],
    Standard Encoded memory density of the HBM device
    0000 - Reserved
    0001 - 1 Gb
    0010 - 2 Gb
    0011 - 4 Gb
    0100 - 8 Gb (8 Gb 8-High)
    0101 - 6 Gb
    0110 - 8 Gb
    1000 - 12 Gb (12 Gb 8-High)
    1001 - 12 Gb (8 Gb 12-High)
    1010 - 16 Gb (16 Gb 8-High)
    1011 - 18 Gb (12 Gb 12-High)
    1100 - 24 Gb (16 Gb 12-High)
    All others - Reserved
    */
    UINT32 ecc : 1;
    /**< ECC support - Bits [80],
    0 - no ECC support
    1 - ECC supported
    */
    UINT32 gen2_test : 1;
    /**< GEN-2 feature support - Bits [81],
    - Lane Remapping modes 1 & 2 with hard/soft repair and readable
    - RD, RR, RC micro-bumps in the BScan chains
    - MISR Preset to 0xAAAAAh / 0x2AAAAAAAh / 0x2AAAAAAAAh
    - MISRs writable for setting seeds (optional)
    - LFSR Compare
    - 3-bit AWORD_MISR_CONFIG
    - MISR/LFSR Compare features expect and preamble clock filter
    0 - features not support
    1 - features supported

    */
    UINT32 reserved : 14;
    /**< reserved - Bits [95:82],
    0 - reserved (default)
    */
  } Bits;
  UINT32 Data[3];
} DEVICE_ID_WDR;

/*
 * Struct format for MR0 extrated from HBM JEDEC spec rev 1.22 Chap 5 Mode Registers
 */
// HBM_MR0
typedef union {
  struct {
    UINT8 dbi_ac_rd : 1;
    /**< Read DBIac Enable - Bits [0], default = 1'h0
      0 - Disable
      1 - Enable (default)
    */
    UINT8 dbi_ac_wr : 1;
    /**< Write DBIac Enable - Bits [1], default = 1'h0
       0 - Disable
       1 - Enable (default)
    */
    UINT8 tcsr : 1;
    /**< Temperature Compesnsated Self Refresh - Bits [2], default = 1'h0
       0 - Disable
       1 - Enable (default)
    */
    UINT8 reserved : 1;
    /**< reserved - Bits [3], default = 0'h0
       0 - reserved (default)
    */
    UINT8 dq_rd_par_en : 1;
    /**< dq_rd_par_en - Bits [4], default = 0'h0
       0 - Disable (default)
       1 - Enable
    */
    UINT8 dq_wr_par_en : 1;
    /**< dq_wr_par_en - Bits [5], default = 0'h0
       0 - Disable (default)
       1 - Enable
    */
    UINT8 add_cmd_par_en : 1;
    /**< add_cmd_par_en - Bits [6], default = 0'h0
       0 - Disable (default)
       1 - Enable
    */
    UINT8 test_mode : 1;
    /**< test_mode - Bits [7], default = 0'h0
       0 - Normal operation (default)
       1 - Test mode (vendor specific)
    */
  } Bits;
  UINT8 Data;
} HBM_MR0_STRUCT;

// HBM_MR1
typedef union {
  struct {
    UINT8 wr_recov : 5;
    /**< Write Recovery (WR) - Bits [4:0], default = X'h0
       00000 - reserved
       00001 - reserved
       00010 - reserved
       00011 - 3 nCK
       00100 - 4 nCK
       00101 - 5 nCK
       00110 - 6 nCK
       00111 - 7 nCK
       01000 - 8 nCK
       ...
       11111 - 31 nCK
    */
    UINT8 nom_drv_strg : 3;
    /**< Nominal Driver Strength - Bits [7:5], default = 000'h0
       000 - 6 mA driver (default)
       001 - 9 mA driver
       010 - 12 mA driver
       011 - 15 mA driver
       100 - 18 mA driver
       All other encodings are reserved
    */
  } Bits;
  UINT8 Data;
} HBM_MR1_STRUCT;

// HBM_MR2
typedef union {
  struct {
    UINT8 wr_lat : 3;
    /**< Write Latency - Bits [2:0], default = X'h0
    000 - 1 nCK (minimum)
    001 - 2 nCK
    010 - 3 nCK
    011 - 4 nCK
    100 - 5 nCK
    101 - 6 nCK
    110 - 7 nCK
    111 - 8 nCK
    */
    UINT8 rd_lat : 5;
    /**< Read Latency - Bits [7:3], default = X'h0
    00000 - 2 nCK (minimum)
    00001 - 3 nCK
    00010 - 4 nCK
    00011 - 5 nCK
    00100 - 6 nCK
    00101 - 7 nCK
    00110 - 8 nCK
    00111 - 9 nCK
    ...
    11111 - 33 nCK
    */
  } Bits;
  UINT8 Data;
} HBM_MR2_STRUCT;

// HBM_MR3
typedef union {
  struct {
    UINT8 act2pre_ras : 6;
    /**< Activate to Precharge RAS- Bits [5:0], default = X'h0
    000000 - reserved
    000001 - reserved
    000010 - reserved
    000011 - 3 nCK
    000100 - 4 nCK
    000101 - 5 nCK
    000110 - 6 nCK
    000111 - 7 nCK
    001000 - 8 nCK
    ...
    111111 - 63 nCK
    */
    UINT8 bge : 1;
    /**< Bank Group Enable - Bits [6], default = X'h0
    0 - Disable
    1 - Enable
    */
    UINT8 bl : 1;
    /**< Burst Length - Bits [7], default = X'h0
    0 - BL2
    1 - BL4
    */
  } Bits;
  UINT8 Data;
} HBM_MR3_STRUCT;

// HBM_MR4
typedef union {
  struct {
    UINT8 ecc_en : 1;
    /**< ECC enable - Bits [0], default = X'h0
    0 - Enable
    1 - Disable
    NOTE: DM and ECC cannot be enabled simultaneously
    i.e., OP [1:0] = 01 is not allowed.
    */
    UINT8 dm_dis : 1;
    /**< Data Mask Disable - Bits [1], default = X'h0
    0 - Enable
    1 - Disable
    NOTE: DM and ECC cannot be enabled simultaneously
    i.e., OP [1:0] = 01 is not allowed.
    */
    UINT8 par_lat : 2;
    /**< Parity Latency - Bits [3:2], default = X'h0
    00 - 0 nCK
    01 - 1 nCK
    10 - 2 nCK
    11 - 3 nCK
    */
    UINT8 reserved : 4;
    /**< Burst Length - Bits [7:4], default = X'h0
    0x0
    */
  } Bits;
  UINT8 Data;
} HBM_MR4_STRUCT;

// HBM_MR5
typedef union {
  struct {
    UINT8 trr_ban : 4;
    /**< TRR Mode BAn - Bits [3:0] = X'h0
    0000 - Bank 0
    ...
    1111 - Bank 15
    */
    UINT8 reserved : 2;
    /**< Reserved - Bits [5:4], default = 0'h0
    00
    */
    UINT8 trr_pseudo_ch : 1;
    /**< TRR Mode -Pseudo Channel Select - Bits [6], default = X'h0
    0 - Enable TRR mode for Pseudo Channel 0
    1 - Enable TRR mode for Pseudo Channel 1
    */
    UINT8 trr_en : 1;
    /**< TRR Mode Enable - Bits [7], default = 0'h0
    0 - Disable (default)
    1 - Enable
    */
  } Bits;
  UINT8 Data;
} HBM_MR5_STRUCT;

// HBM_MR6
typedef union {
  struct {
    UINT8 reserved : 3;
    /**< Reserved - Bits [2:0] = 0'h0
    000
    */
    UINT8 impre_trp : 5;
    /**< imPRE tRP Value - Bits [7:3], default = x'h0
    00000 - 2 nCK (Minimum)
    00001 - 3 nCK
    00010 - 4 nCK
    00011 - 5 nCK
    ...
    11111 - 33 nCK
    */
  } Bits;
  UINT8 Data;
} HBM_MR6_STRUCT;


// HBM_MR7
typedef union {
  struct {
    UINT8 dword_loopback_en : 1;
    /**< DWORD Read Mux Control - Bits [0] = X'h0
    00 - Reserved
    01 - Return data from MISR registers
    10 - Return data from Rx path sampler
    11 - Return LFSR_COMPARE_STICKY (optional)
    */
    UINT8 dword_read_mux_ctrl : 2;
    /**< DWORD Read Mux Control - Bits [2:1] = X'h0
    00 - Reserved
    01 - Return data from MISR registers
    10 - Return data from Rx path sampler
    11 - Return LFSR_COMPARE_STICKY (optional)
    */
    UINT8 dword_misr_ctrl : 3;
    /**< DWORD MISR Control - Bits [5:3] = X'h0
    Only aplicable if Loopback is enabled in OP[0]
    000 - Preset
    The DWORD MISR/LFSSRs are set to AAAAAh, and the
    DWORD LFSR_COMPARE_STICKY bits are set all to 0s
    001 - LFSR mode (read direction)
    010 - Register mode (read and write directions)
    DWORD writes are captured directly into the MISR
    registers without compression. The MISR registers
    will contain the most recent data.
    011 - MISR mode (write direction)
    100 - LFSR Compare mode (write direction)
    */
    UINT8 reserved : 1;
    /**< Reserved - Bits [6] = 0'h0
    0
    */
    UINT8 cattrip : 1;
    /**< CATTRIP - Bits [7], default = 0'h0
    0 - Clear CATTRIP pin (default)
    1 - Assert CATTRIP pin to "1"
    */
  } Bits;
  UINT8 Data;
} HBM_MR7_STRUCT;

// HBM_MR8
typedef union {
  struct {
    UINT8 reserved : 8;
    /**< Reserved - Bits [7:0] = 0'h0
    0x00
    */
  } Bits;
  UINT8 Data;
} HBM_MR8_STRUCT;

// HBM_MR9
typedef union {
  struct {
    UINT8 reserved : 8;
    /**< Reserved - Bits [7:0] = 0'h0
    0x00
    */
  } Bits;
  UINT8 Data;
} HBM_MR9_STRUCT;

// HBM_MR10
typedef union {
  struct {
    UINT8 reserved : 8;
    /**< Reserved - Bits [7:0] = 0'h0
    0x00
    */
  } Bits;
  UINT8 Data;
} HBM_MR10_STRUCT;

// HBM_MR11
typedef union {
  struct {
    UINT8 reserved : 8;
    /**< Reserved - Bits [7:0] = 0'h0
    0x00
    */
  } Bits;
  UINT8 Data;
} HBM_MR11_STRUCT;

// HBM_MR12
typedef union {
  struct {
    UINT8 reserved : 8;
    /**< Reserved - Bits [7:0] = 0'h0
    0x00
    */
  } Bits;
  UINT8 Data;
} HBM_MR12_STRUCT;

// HBM_MR13
typedef union {
  struct {
    UINT8 reserved : 8;
    /**< Reserved - Bits [7:0] = 0'h0
    0x00
    */
  } Bits;
  UINT8 Data;
} HBM_MR13_STRUCT;

// HBM_MR14
typedef union {
  struct {
    UINT8 reserved : 8;
    /**< Reserved - Bits [7:0] = 0'h0
    0x00
    */
  } Bits;
  UINT8 Data;
} HBM_MR14_STRUCT;

// HBM_MR15
typedef union {
  struct {
    UINT8 internal_vref : 3;
    /**< Internal Vref - Bits [2:0] = X'h0
    Optional HBM feature.
    HBM may simply ignore these bits if there is
    no internal Vref generator.
    If there is internal Vref, these bits are programemed
    appropriately. Applies to both DQ bus as well as
    Row/Column address/cmd bus. HBM has only one internal
    Vref generator across all 8 channels. These bits must
    programmed indentically in all eight mode registers.
    000 - 50% VDD
    001 - 46% VDD
    010 - 42% VDD
    011 - 38% VDD
    100 - 54% VDD
    101 - 58% VDD
    110 - 62% VDD
    111 - 66% VDD
    */
    UINT8 reserved : 5;
    /**< Reserved - Bits [7:3] = 0'h0
    0x00
    */
  } Bits;
  UINT8 Data;
} HBM_MR15_STRUCT;

#endif // _HBM_CORE_JEDEC_H_

