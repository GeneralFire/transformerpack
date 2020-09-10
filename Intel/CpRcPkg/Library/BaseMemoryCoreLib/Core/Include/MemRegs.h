/** @file
  MemRegs.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#ifndef _memregs_h
#define _memregs_h

//
// #define UNCORE_CR_UNCORE_MC_CFG_CONTROL   0x2E3
//
//
// Serial Presence Detect bytes (JEDEC revision 1.0)
//
#define SPD_SIZE              0     // Bytes used, Device size, CRC coverage
#define SPD_REVISION          1     // SPD Encoding Revision
#define SPD_KEY_BYTE          2     // DRAM Device Type  //BYTE 2 - keyByte
#define SPD_TYPE_DDR4         0x0C  // DDR4 SDRAM
#define SPD_TYPE_DDR5         0x12  // DDR5 SDRAM
#define SPD_TYPE_AEP          0x0D  // DDRT SDRAM
#define SPD_KEY_BYTE2         3     // Module Type and Thickness (RDIMM or UDIMM)  //BYTE 3 keyByte2
  #define SPD_RDIMM           1     // Module type is RDIMM
  #define SPD_UDIMM           2     // Module type is UDIMM
  #define SPD_SODIMM          3     // Module type is SODIMM
  #define SPD_MICRO_DIMM      4     // Module type is Micro-DIMM
  #define SPD_LRDIMM_DDR4     4     // Module type is LRDIMM (DDR4)
  #define SPD_MINI_RDIMM      5     // Module type is Mini-RDIMM
  #define SPD_MINI_UDIMM      6     // Module type is Mini-UDIMM
  #define SPD_MINI_CDIMM      7     // Module type is Mini-CDIMM
  #define SPD_ECC_SO_UDIMM    8     // Module type is 72b-SO-UDIMM
  #define SPD_ECC_SO_RDIMM    9     // Module type is 72b-SO-RDIMM
  #define SPD_ECC_SO_RDIMM_DDR4  8  // Module type is 72b-SO-RDIMM
  #define SPD_ECC_SO_UDIMM_DDR4  9  // Module type is 72b-SO-UDIMM
  #define SPD_ECC_SO_CDIMM    10    // Module type is 72b-SO-CDIMM
  #define SPD_LRDIMM          11    // Module type is LRDIMM
  #define SPD_LRDIMM_DDR5     4     // Module type is LRDIMM (DDR5)
#if FixedPcdGetBool(PcdMcrSupport)
  #define SPD_MCRDIMM_DDR5    5     // SPD key byte to identify MCR DIMM (MCR_TODO: Fix temp SPD key byte for MCR ID)
#endif // FixedPcdGetBool(PcdMcrSupport)
  #define SPD_16b_SO_DIMM     12    // Module type is 16b_SO_DIMM
  #define SPD_32b_SO_DIMM     13    // Module type is 32b_SO_DIMM
  #define SPD_NVM_DIMM        13    // JEDEC (place holder)
  #define SPD_UDIMM_ECC       18    // Module type is UDIMM-ECC
  #define SPD_HYBRID_MASK     BIT7  // Module type is Hybrid
  #define SPD_HYBRID_TYPE_MASK (BIT6 + BIT5 + BIT4) // type of Hybrid
  #define SPD_BASE_TYPE_MASK (BIT3 + BIT2 + BIT1 + BIT0) // Module base type
  #define SPD_HYBRID_TYPE     BIT7  //Module type is a Hybrid
  #define SPD_HYBRID_TYPE_NVDIMM (1 << 4) //Module type is a hybrid NVDIMM
#define SPD_VDD_SUPPORT       6     // Vdd DIMM supports
  #define SPD_VDD_125         BIT2  // Module Supports 1.25V
#define SPD_MODULE_ORG        7     // Number of Ranks and SDRAM device width
  #define SPD_NUM_RANKS_1   0
  #define SPD_NUM_RANKS_2   1
  #define SPD_NUM_RANKS_4   3
  #define SPD_NUM_RANKS_8   7
#define SPD_MEM_BUS_WID     8     // Width of SDRAM memory bus
#define SPD_FTB             9     // Timebase for fine grain timing calculations
#define SPD_MTB_DIVEND      10    // Medium Time Base Dividend
#define SPD_MTB_DIVISOR     11    // Medium Time Base Divisor
#define SPD_MIN_TCK         12    // Minimum cycle time (at max CL)
  #define SPD_TCKMIN_800      20    // tCK(MTB)=20, tCK(ns)=2.5
  #define SPD_TCKMIN_1066     15    // tCK(MTB)=15, tCK(ns)=1.875
  #define SPD_TCKMIN_1333     12    // tCK(MTB)=12, tCK(ns)=1.5
  #define SPD_TCKMIN_1600     10    // tCK(MTB)=10, tCK(ns)=1.25
  #define SPD_TCKMIN_1866     9     // tCK(MTB)=9, tCK(ns)=1.07
  #define SPD_TCKMIN_2133     8     // tCK(MTB)=8, tCK(ns)=0.9375
  #define SPD_TCKMIN_2400     7     // tCK(MTB)=7, tCK(ns)=.833
#define SPD_CAS_LT_SUP_LSB  14    // CAS Latencies Supported, Least Significant Byte
#define SPD_CAS_LT_SUP_MSB  15    // CAS Latencies Supported, Most Significant Byte
#define SPD_MIN_TAA         16    // Minimum CAS Latency Time (tAAmin)
#define SPD_MIN_TWR         17    // Minimum Write Recovery Time
#define SPD_MIN_TRCD        18    // Minimum RAS to CAS delay
#define SPD_MIN_TRRD        19    // Minimum Row active to row active delay
#define SPD_MIN_TRP         20    // Minimum Row Precharge time
#define SPD_EXT_TRC_TRAS    21    // Upper nibbles for min tRAS and tRC
#define SPD_MIN_TRAS        22    // Minimum Active to Precharge time
#define SPD_MIN_TRC         23    // Minimum Active to Active/Refresh time
#define SPD_MIN_TRFC_LSB    24    // Minimum Refresh Recovery time least-significant byte
#define SPD_MIN_TRFC_MSB    25    // Minimum Refresh Recovery time most-significant byte
#define SPD_MIN_TWTR        26    // Minimum Internal Write to Read command delay
#define SPD_MIN_TRTP        27    // Minimum Internal Read to Precharge command delay
#define SPD_UN_TFAW         28    // Upper Nibble for tFAW
#define SPD_MIN_TFAW        29    // Minimum Four Activate Window Delay Time (tFAWmin)
#define SPD_OD_SUP          30    // SDRAM Output Drivers Supported
#define SPD_DIMM_TS         32    // Module Temperature Sensor
#define SPD_SDRAM_TYPE      33    // SDRAM device type
#define SPD_FTB_TCK         34    // Fine Offset for SDRAM tCK
#define SPD_FTB_TAA         35    // Fine Offset for SDRAM tAA
#define SPD_FTB_TRCD        36    // Fine Offset for SDRAM tRCD
#define SPD_FTB_TRP         37    // Fine Offset for SDRAM tRP
#define SPD_FTB_TRC         38    // Fine Offset for SDRAM tRC
#define SPD_OPT_FEAT        41    // SDRAM Optional Features
  #define SPD_PTRR          BIT7  // Indicates if the DIMM is pTRR compliant

#define SPD_DIMM_TS_MASK_DDR5 (BIT7 | BIT6) // Bit 7 - TS0; Bit 6 - T10

// Begin module specific section
#define SPD_MODULE_NH       60    // Module Nominal Height
#define SPD_MODULE_MT       61    // Module Maximum Thickness
#define SPD_REF_RAW_CARD    62    // Reference Raw Card Used
 #define RAW_CARD_A         0
 #define RAW_CARD_B         1
 #define RAW_CARD_C         2
 #define RAW_CARD_D         3
 #define RAW_CARD_E         4
 #define RAW_CARD_F         5
 #define RAW_CARD_G         6
 #define RAW_CARD_H         7
 #define RAW_CARD_J         8
 #define RAW_CARD_K         9
 #define RAW_CARD_L         10
 #define RAW_CARD_M         11
 #define RAW_CARD_N         12
 #define RAW_CARD_P         13
 #define RAW_CARD_R         14
 #define RAW_CARD_T         15
 #define RAW_CARD_U         16
 #define RAW_CARD_V         17
 #define RAW_CARD_W         18
 #define RAW_CARD_Y         19
 #define RAW_CARD_AA        20
 #define RAW_CARD_AB        21
 #define RAW_CARD_AC        22
 #define RAW_CARD_AD        23
 #define RAW_CARD_AE        24
 #define RAW_CARD_AF        25
 #define RAW_CARD_AG        26
 #define RAW_CARD_AH        27
 #define RAW_CARD_AJ        28
 #define RAW_CARD_AK        29
 #define RAW_CARD_AL        30
 #define RAW_CARD_ZZ        0x1F
 #define RAW_CARD_REV_MASK  (BIT6 + BIT5)
 #define RAW_CARD_EXT       BIT7
 #define RAW_CARD_AM        (RAW_CARD_EXT + 0)
 #define RAW_CARD_AN        (RAW_CARD_EXT + 1)
 #define RAW_CARD_AP        (RAW_CARD_EXT + 2)
 #define RAW_CARD_AR        (RAW_CARD_EXT + 3)
 #define RAW_CARD_AT        (RAW_CARD_EXT + 4)
 #define RAW_CARD_AU        (RAW_CARD_EXT + 5)
 #define RAW_CARD_AV        (RAW_CARD_EXT + 6)
 #define RAW_CARD_AW        (RAW_CARD_EXT + 7)
 #define RAW_CARD_AY        (RAW_CARD_EXT + 8)
 #define RAW_CARD_BA        (RAW_CARD_EXT + 9)
 #define RAW_CARD_BB        (RAW_CARD_EXT + 10)
 #define RAW_CARD_BC        (RAW_CARD_EXT + 11)
 #define RAW_CARD_BD        (RAW_CARD_EXT + 12)
 #define RAW_CARD_BE        (RAW_CARD_EXT + 13)
 #define RAW_CARD_BF        (RAW_CARD_EXT + 14)
 #define RAW_CARD_BG        (RAW_CARD_EXT + 15)
 #define RAW_CARD_BH        (RAW_CARD_EXT + 16)
 #define RAW_CARD_BJ        (RAW_CARD_EXT + 17)
 #define RAW_CARD_BK        (RAW_CARD_EXT + 18)
 #define RAW_CARD_BL        (RAW_CARD_EXT + 19)
 #define RAW_CARD_BM        (RAW_CARD_EXT + 20)
 #define RAW_CARD_BN        (RAW_CARD_EXT + 21)
 #define RAW_CARD_BP        (RAW_CARD_EXT + 22)
 #define RAW_CARD_BR        (RAW_CARD_EXT + 23)
 #define RAW_CARD_BT        (RAW_CARD_EXT + 24)
 #define RAW_CARD_BU        (RAW_CARD_EXT + 25)
 #define RAW_CARD_BV        (RAW_CARD_EXT + 26)
 #define RAW_CARD_BW        (RAW_CARD_EXT + 27)
 #define RAW_CARD_BY        (RAW_CARD_EXT + 28)
 #define RAW_CARD_CA        (RAW_CARD_EXT + 29)
 #define RAW_CARD_CB        (RAW_CARD_EXT + 30)

#define SPD_MM_LOC          119   // Module Manufacturing Location
#define SPD_MM_DATE         120   // Module Manufacturing Date 120-121
#define SPD_MODULE_SN       122   // Module Serial Number 122-125
#define SPD_CRC_LSB         126   // LSB of 16-bit CRC
#define SPD_CRC_MSB         127   // MSB of 16-bit CRC

#define SPD_MODULE_PN       128   // Module Part Number 128-145
#define SPD_MODULE_RC       146   // Module Revision Code 146-147
#define SPD_DRAM_MIDC_LSB   148   // DRAM Manufacturer ID Code, Least Significant Byte
#define SPD_DRAM_MIDC_MSB   149   // DRAM Manufacturer ID Code, Most Significant Byte
#define SPD_BYTE_200        200   // Fixed value 0xBE

//
// DDR4 Specific Bytes
//
  #define SPD_SIGNAL_LOADING       (BIT0 | BIT1)         // Signal Loading Mask
  #define SPD_MULTI_LOAD_TYPE      BIT0
  #define SPD_3DS_TYPE             BIT1                  // Encoding for Single load stack (3DS)
  #define SPD_DIE_COUNT_MASK       (BIT4 | BIT5 | BIT6)  // Die Count Mask
  #define SPD_NON_MONOLITHIC_TYPE  BIT7
  #define SPD_MONOLITHIC_TYPE      0
#define SPD_OPT_FEAT_DDR4     7     // SDRAM Optional Features (DDR4)
  #define SPD_MAC_MASK        BIT0 | BIT1 | BIT2  // Mask for Maximum Active Count field
  #define SPD_TRR_IMMUNE      BIT3  // Indicates this DIMM does not require DRAM Maintenance
#define SPD_RFSH_OPT_DDR4     8     // SDRAM Refresh Options (DDR4)
#define SPD_OTH_OP_FEAT_DDR4  9     // SDRAM Optional features (DDR4)
  #define SPD_PPR_MASK        (BIT7 | BIT6) // Mask for PPR capability
    #define SPD_PPR_NOT_SUP   0     // PPR not supported
    #define SPD_PPR_HARD_1R   BIT6  // Hard PPR with 1 row per BG
  #define SPD_SOFT_PPR        BIT5  // Soft PPR capability

  #define SPD_HALF_SIZE_SECOND_RANK BIT2
  #define SPD_QUARTER_SIZE_SECOND_RANK BIT3
#define SPD_VDD_120           3     // Module operable and endurant 1.20V
#define SPD_VDD_110           0x0  // module Nominal, Operable and Endurant 1.10V, DDR5 SPD spec proposed
#define SPD_MEM_BUS_WID_DDR4  13    // Width of SDRAM memory bus
  #define SPD_BUS_WIDTH_EXT   (BIT3 | BIT4)  // Bus width extension mask (01 = 8 bit ECC)
  #define SPD_BUS_WIDTH_EXT_BIT_OFFSET   3   // Bit [4, 3]
  #define SPD_BUS_WIDTH_EXT_4_BITS_DDR5  1   // DDR5 only, No 4 bits ECC for DDR4
  #define SPD_BUS_WIDTH_EXT_8_BITS_DDR5  2   // DDR5 only, 8 bits ECC for DDR4 is encoded as 1
#define SPD_DIMM_TS_DDR4      14    // Module Thermal Sensor
#define SPD_TB_DDR4           17    // Timebase [3:2] MTB, [1:0] FTB
#define SPD_MIN_TCK_DDR4      18    // Minimum cycle time
  #define SPD_TCKMIN_DDR4_1600      10  // tCK(MTB)=10, tCK(ns)=1.25
  #define SPD_TCKMIN_DDR4_1866      9   // tCK(MTB)=9, tCK(ns)=1.071
  #define SPD_TCKMIN_DDR4_2133      8   // tCK(MTB)=8, tCK(ns)=.938
  #define SPD_TCKMIN_DDR4_2400      7   // tCK(MTB)=7, tCK(ns)=.833
#define SPD_MAX_TCK_DDR4      19    // Maximum cycle time
#define SPD_CAS_LT_SUP_1_DDR4 20    // CAS Latencies Supported, first byte
#define SPD_CAS_LT_SUP_2_DDR4 21    // CAS Latencies Supported, second byte
#define SPD_CAS_LT_SUP_3_DDR4 22    // CAS Latencies Supported, third byte
#define SPD_CAS_LT_SUP_4_DDR4 23    // CAS Latencies Supported, fourth byte
#define SPD_MIN_TAA_DDR4      24    // Minimum CAS Latency Time (tAAmin)
#define SPD_MIN_TRCD_DDR4     25    // Minimum RAS to CAS delay
#define SPD_MIN_TRP_DDR4      26    // Minimum Row Precharge time
#define SPD_EXT_TRC_TRAS_DDR4 27    // Upper nibbles for min tRAS and tRC
#define SPD_MIN_TRAS_DDR4     28    // Minimum Active to Precharge time
#define SPD_MIN_TRC_DDR4      29    // Minimum Active to Active/Refresh time
#define SPD_MIN_TRFC1_LSB_DDR4 30    // Minimum Refresh Recovery time least-significant byte
#define SPD_MIN_TRFC1_MSB_DDR4 31    // Minimum Refresh Recovery time most-significant byte
#define SPD_MIN_TRFC2_LSB_DDR4 32    // Minimum Refresh Recovery time least-significant byte
#define SPD_MIN_TRFC2_MSB_DDR4 33    // Minimum Refresh Recovery time most-significant byte
#define SPD_MIN_TRFC3_LSB_DDR4 34    // Minimum Refresh Recovery time least-significant byte
#define SPD_MIN_TRFC3_MSB_DDR4 35    // Minimum Refresh Recovery time most-significant byte
#define SPD_TFAW_UPPER_DDR4   36    // Upper nibble for tFAW
#define SPD_MIN_TFAW_DDR4     37    // Minimum For Active Window Delay Time (tFAW)
#define SPD_MIN_TRRDS_DDR4    38    // Minimum Active to Active Delay Time tRRD_S Different Bank Group
#define SPD_MIN_TRRDL_DDR4    39    // Minimum Active to Active Delay Time tRRD_L Same Bank Group
#define SPD_FTB_TRRDL_DDR4    118   // Fine offset for tRRD_L
#define SPD_FTB_TRRDS_DDR4    119   // Fine offset for tRRD_S
#define SPD_FTB_TRC_DDR4      120   // Fine offset for TRC
#define SPD_FTB_TRP_DDR4      121   // Fine offset for TRP
#define SPD_FTB_TRCD_DDR4     122   // Fine offset for TRCD
#define SPD_FTB_TAA_DDR4      123   // Fine offset for TAA
#define SPD_FTB_MAX_TCK_DDR4  124   // Fine offset for max TCK
#define SPD_FTB_MIN_TCK_DDR4  125   // Fine offset for min TCK

#define SPD_CRC_LSB_BLOCK1    254   // LSB of 16-bit CRC
#define SPD_CRC_MSB_BLOCK1    255   // MSB of 16-bit CRC

#define SPD_MMID_LSB_DDR4     320   // Module Manufacturer ID Code, Least Significant Byte
#define SPD_MMID_MSB_DDR4     321   // Module Manufacturer ID Code, Most Significant Byte
#define SPD_MM_LOC_DDR4       322   // Module Manufacturing Location
#define SPD_MM_DATE_YR_DDR4   323   // Module Manufacturing Date Year
#define SPD_MM_DATE_WK_DDR4   324   // Module Manufacturing Date Week
#define SPD_MODULE_SN_DDR4    325   // Module Serial Number 325-328
#define SPD_MODULE_PN_DDR4    329   // Module Part Number 329-348
#define SPD_MODULE_RC_DDR4    349   // Module Revision Code
#define SPD_DRAM_MIDC_LSB_DDR4 350  // DRAM Manufacturer ID Code, Least Significant Byte
#define SPD_DRAM_MIDC_MSB_DDR4 351  // DRAM Manufacturer ID Code, Most Significant Byte
#define SPD_DRAM_REV_DDR4     352   // DRAM Revision ID
#define SPD_CRC_LSB_DDR4      382   // LSB of 16-bit CRC
#define SPD_CRC_MSB_DDR4      383   // MSB of 16-bit CRC

// Begin DDR4 module specific section
#define SPD_MODULE_NH_DDR4    128    // Module Nominal Height
#define SPD_MODULE_MT_DDR4    129    // Module Maximum Thickness
#define SPD_REF_RAW_CARD_DDR4 130    // Reference Raw Card Used

// UDIMM specific bytes
// Applicable when Module Type (key byte 3) = 2
#define SPD_ADDR_MAP_FECTD_DDR4 131    // Address Mapping from Edge Connector to DRAM

// RDIMM specific bytes
// Applicable when Module Type (key byte 3) = 1
#define SPD_RDIMM_ATTR_DDR4    131   // RDIMM module attributes
#define SPD_DIMM_HS_DDR4       132   // Module Heat Spreader Solution
#define SPD_REG_VEN_LSB_DDR4   133   // Register Vendor ID LSB
#define SPD_REG_VEN_MSB_DDR4   134   // Register Vendor ID MSB
#define SPD_REG_REV_DDR4       135   // Register Revision
#define SPD_ADD_MAPPING_DDR4   136   // Address mapping from Reg to DRAM
#define SPD_REG_OD_CTL_DDR4    137   // Register Output Drive Strength for Control
#define SPD_REG_OD_CK_DDR4     138   // Register Output Drive Strength for Clock

//DIMM revision types
#define RCD_REV_1 0
#define RCD_REV_2 1

// LRDIMM specific bytes
// Applicable when Module Type (key byte 3) = 0x4
#define SPD_LRDIMM_ATTR_DDR4   131   // LRDIMM module attributes
#define SPD_LRBUF_HS_DDR4      132   // LR Buffer Heat Spreader Solution
#define SPD_LRBUF_VEN_LSB_DDR4 133   // LR Buffer Vendor ID LSB
#define SPD_LRBUF_VEN_MSB_DDR4 134   // LR Buffer Vendor ID MSB
#define SPD_LRBUF_REV_DDR4     135   // LR Buffer Register Revision
#define SPD_LRBUF_ADD_MAP_DDR4 136   // LR Buffer Address Mapping from Register to DRAM
#define SPD_LRBUF_DB_REV_DDR4  139   // LR Buffer Data Buffer Revision
#define SPD_LRBUF_DRAM_VREFDQ_R0_DDR4 140 // LR Buffer DRAM VrefDQ for Package Rank 0
#define SPD_LRBUF_DRAM_VREFDQ_R1_DDR4 141 // LR Buffer DRAM VrefDQ for Package Rank 1
#define SPD_LRBUF_DRAM_VREFDQ_R2_DDR4 142 // LR Buffer DRAM VrefDQ for Package Rank 2
#define SPD_LRBUF_DRAM_VREFDQ_R3_DDR4 143 // LR Buffer DRAM VrefDQ for Package Rank 3
#define SPD_LRBUF_DB_VREFDQ_DDR4 144 // LR Data Buffer VrefDQ for DRAM Interface
#define SPD_LRBUF_DB_DS_RTT_LE1866_DDR4 145 // LR Data Buffer MDQ Drive Strength and RTT for data rate <= 1866
#define SPD_LRBUF_DB_DS_RTT_GT1866_LE2400_DDR4 146 // LR Data Buffer MDQ Drive Strength and RTT for data rate > 1866 and <= 2400
#define SPD_LRBUF_DB_DS_RTT_GT2400_LE3200_DDR4 147 // LR Data Buffer MDQ Drive Strength and RTT for data rate > 2400 and <= 3200
#define SPD_LRBUF_DRAM_DS_DDR4 148   // LR Buffer DRAM Drive Strength (for data rates <= 1866, 1866 < data rate <= 2400, and 2400 < data rate <= 3200)
#define SPD_LRBUF_DRAM_ODT_WR_NOM_LE1866_DDR4 149 // LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate <= 1866
#define SPD_LRBUF_DRAM_ODT_WR_NOM_GT1866_LE2400_DDR4 150 // LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate > 1866 and <= 2400
#define SPD_LRBUF_DRAM_ODT_WR_NOM_GT2400_LE3200_DDR4 151 // LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate > 2400 and <= 3200
#define SPD_LRBUF_DRAM_ODT_PARK_LE1866_DDR4 152 // LR Buffer DRAM ODT (RTT_PARK) for data rate <= 1866
#define SPD_LRBUF_DRAM_ODT_PARK_GT1866_LE2400_DDR4 153 // LR Buffer DRAM ODT (RTT_PARK) for data rate > 1866 and <= 2400
#define SPD_LRBUF_DRAM_ODT_PARK_GT2400_LE3200_DDR4 154 // LR Buffer DRAM ODT (RTT_PARK) for data rate > 2400 and <= 3200

// DDR4 NVDIMM specific bytes
// Applicable when Module Type (key byte 3) = 0x9M, where M is the base type
#define SPD_NVDIMM_MAX_INIT_TIME 203  // NVDIMM Maximum Non-Volatile Memory Initialization Time
#define SPD_NVDIMM_FN0_FID_LSB  204   // NVDIMM Function Interface Descriptor for Function 0, LSB
#define SPD_NVDIMM_FN0_FID_MSB  205   // NVDIMM Function Interface Descriptor for Function 0, MSB
#define SPD_NVDIMM_FN1_FID_LSB  206   // NVDIMM Function Interface Descriptor for Function 1, LSB
#define SPD_NVDIMM_FN1_FID_MSB  207   // NVDIMM Function Interface Descriptor for Function 1, MSB
#define SPD_NVDIMM_FN2_FID_LSB  208   // NVDIMM Function Interface Descriptor for Function 2, LSB
#define SPD_NVDIMM_FN2_FID_MSB  209   // NVDIMM Function Interface Descriptor for Function 2, MSB
#define SPD_NVDIMM_FN3_FID_LSB  210   // NVDIMM Function Interface Descriptor for Function 3, LSB
#define SPD_NVDIMM_FN3_FID_MSB  211   // NVDIMM Function Interface Descriptor for Function 3, MSB
#define SPD_NVDIMM_FN4_FID_LSB  212   // NVDIMM Function Interface Descriptor for Function 4, LSB
#define SPD_NVDIMM_FN4_FID_MSB  213   // NVDIMM Function Interface Descriptor for Function 4, MSB
#define SPD_NVDIMM_FN5_FID_LSB  214   // NVDIMM Function Interface Descriptor for Function 5, LSB
#define SPD_NVDIMM_FN5_FID_MSB  215   // NVDIMM Function Interface Descriptor for Function 5, MSB
#define SPD_NVDIMM_FN6_FID_LSB  216   // NVDIMM Function Interface Descriptor for Function 6, LSB
#define SPD_NVDIMM_FN6_FID_MSB  217   // NVDIMM Function Interface Descriptor for Function 6, MSB
#define SPD_NVDIMM_FN7_FID_LSB  218   // NVDIMM Function Interface Descriptor for Function 7, LSB
#define SPD_NVDIMM_FN7_FID_MSB  219   // NVDIMM Function Interface Descriptor for Function 7, MSB
#define SPD_NVDIMM_FI_MASK (BIT4 + BIT3 + BIT2 + BIT1 + BIT0) //Function Interface Mask
#define SPD_NVDIMM_STANDARD_INTERFACE_1 1 //Standard Interface 1
#define SPD_NVDIMM_FC_MASK (BIT9 + BIT8 + BIT7 + BIT6 + BIT5) //Function Class Mask
#define SPD_NVDIMM_BYTE_ADDR_ENERGY_BACKED (1 << 5) //Function Class byte addressible energy backed
//
// End DDR4 Specific Bytes
//

//
// NVM DIMM Specific Bytes
//
#define SPD_AEP_MOD_REVISION                        130
  #define SPD_AEP_MOD_REV_AEP                         0x1
  #define SPD_AEP_MOD_REV_APK                         0x2
  #define SPD_AEP_MOD_REV_BPK                         0x4
  #define SPD_AEP_MOD_REV_BPS                         0x9
  #define SPD_AEP_MOD_REV_PPK                         0xA
  #define SPD_AEP_MOD_REV_IPK                         0xC

//
// NVM DIMM Reg Structs
//

typedef union {
  struct {
    UINT8 module_type     : 4;
    UINT8 module_revision : 4;
  } Bits;
  UINT8 Data;
} SPD_AEP_MOD_REVISION_STRUCT;

//
// Dcpmm DIMM SPD revision types
//
#define SPD_DCPMM_REV_0P0                          0x00
#define SPD_DCPMM_REV_0P1                          0x01
#define SPD_DCPMM_REV_0P2                          0x02
#define SPD_DCPMM_REV_1P0                          0x10
#define SPD_DCPMM_REV_1P1                          0x11
#define SPD_DCPMM_REV_1P2                          0x12
#define SPD_DCPMM_REV_1P4                          0x14

//
// AEP Dimm Specific Bytes Rev 2
//

#define SPD_AEP_DEV_EKV_STEPPING                    135
#define SPD_AEP_DEV_DENSITY                         176
#define SPD_AEP_DEV_TYPE                            178
#define SPD_AEP_MOD_ORGANIZATION                    180
#define SPD_AEP_MOD_BUS_WIDTH                       181
#define SPD_AEP_THERMAL_SENSOR                      182
#define SPD_AEP_LRBUF_DB_VREFDQ                     144
#define SPD_AEP_LRBUF_DS_RTT_LE1866                 145
#define SPD_AEP_LRBUF_DS_RTT_GT1866_LE2400          146
#define SPD_AEP_LRBUF_DS_RTT_GT2400_LE3200          147
#define SPD_AEP_LRBUF_DRAM_DS                       148
#define SPD_AEP_LRBUF_DRAM_ODT_WR_NOM_LE1866        149
#define SPD_AEP_LRBUF_DRAM_ODT_WR_NOM_GT1866_LE2400 150
#define SPD_AEP_LRBUF_DRAM_ODT_WR_NOM_GT2400_LE3200 151
#define SPD_AEP_LRBUF_DRAM_ODT_PARK_LE1866          152
#define SPD_AEP_LRBUF_DRAM_ODT_PARK_GT1866_LE2400   153
#define SPD_AEP_LRBUF_DRAM_ODT_PARK_GT2400_LE3200   154
#define SPD_AEP_BACKSIDE_SWIZZLE_0                  234
#define SPD_AEP_BACKSIDE_SWIZZLE_1                  233
#define SPD_AEP_NVM_STEPPING                        237
#define SPD_AEP_WRITE_CREDIT                        170
#define SPD_AEP_READ_CREDIT                         171
#define SPD_AEP_GNT2ERID_1333_1600                  186
#define SPD_AEP_GNT2ERID_1866_2133                  187
#define SPD_AEP_GNT2ERID_2400_2666                  188
#define SPD_AEP_GNT2ERID_3200                       189
#define SPD_AEP_LSB_FMC_TYPE                        196
#define SPD_AEP_FMC_REV                             198
#define FMC_NONE_TYPE                              0x00
#define FMC_FNV_TYPE                               0x79
#define FMC_EKV_TYPE                               0x7A
#define FMC_BWV_TYPE                               0x7B
#define FMC_CWV_TYPE                               0x7C

#define SPD_AEP_PARTITION_RATIO_SKT0 384  // NVM DIMM partitionRatio[0], low nibble. NVM DIMM partitionRatio[1], high nibble
#define SPD_AEP_PARTITION_RATIO_SKT1 385  // NVM DIMM partitionRatio[2], low nibble. NVM DIMM partitionRatio[3], high nibble
#define SPD_AEP_PARTITION_RATIO_SKT2 386  // NVM DIMM partitionRatio[4], low nibble. NVM DIMM partitionRatio[5], high nibble
#define SPD_AEP_PARTITION_RATIO_SKT3 387  // NVM DIMM partitionRatio[6], low nibble. NVM DIMM partitionRatio[7], high nibble

#define SPD_VENDORID_BYTE0 320
#define SPD_VENDORID_BYTE1 321
#define SPD_DEVICEID_BYTE0 192
#define SPD_DEVICEID_BYTE1 193
#define SPD_RID 349
#define SPD_SUBSYSTEM_VENDORID_BYTE0 194
#define SPD_SUBSYSTEM_VENDORID_BYTE1 195
#define SPD_SUBSYSTEM_DEVICEID_BYTE0 196
#define SPD_SUBSYSTEM_DEVICEID_BYTE1 197
#define SPD_SUBSYSTEM_REVISIONID 198

#define SPD_XMP_ID            176 // XMP Id String
#define SPD_XMP_ORG           178 // XMP Organization and Configuration
#define SPD_XMP_REV           179 // XMP Revision
#define SPD_XMP_MTB_DIVEND1   180 // XMP Medium Timebase Dividend Profile 1
#define SPD_XMP_MTB_DIVISOR1  181 // XMP Medium Timebase Divisor Profile 1
#define SPD_XMP_MTB_DIVEND2   182 // XMP Medium Timebase Dividend Profile 2
#define SPD_XMP_MTB_DIVISOR2  183 // XMP Medium Timebase Divisor Profile 2
#define SPD_XMP_VDD           185 // XMP Module VDD Voltage Level
#define SPD_XMP_TCK           186 // XMP Minimum cycle time (at max CL)
#define SPD_XMP_TAA           187 // XMP Minimum CAS Latency Time (tAAmin)
#define SPD_XMP_CAS_SUP_LSB   188 // XMP CAS Latencies Supported, Least Significant Byte
#define SPD_XMP_CAS_SUP_MSB   189 // XMP CAS Latencies Supported, Most Significant Byte
#define SPD_XMP_TCWL          190 // XMP Minimum CAS Write Latency Time(tCWLmin)
#define SPD_XMP_TRP           191 // XMP Minimum Row Precharge time (tRPmin)
#define SPD_XMP_TRCD          192 // XMP Minimum RAS to CAS delay (tRCDmin)
#define SPD_XMP_TWR           193 // XMP Minimum Write Recovery Time
#define SPD_XMP_TRC_TRAS      194 // XMP Upper nibbles for min tRAS and tRC
#define SPD_XMP_TRAS          195 // XMP Minimum Active to Precharge time
#define SPD_XMP_TRC           196 // XMP Minimum Active to Active/Refresh time
#define SPD_XMP_TREFI_LSB     197 // XMP Maximum tREFI Time Least Significant Byte
#define SPD_XMP_TREFI_MSB     198 // XMP Maximum tREFI Time Most Significant Byte
#define SPD_XMP_TRFC_LSB      199 // XMP Minimum Refresh Recovery Delay Time Least Significant Byte
#define SPD_XMP_TRFC_MSB      200 // XMP Minimum Refresh Recovery Delay Time Most Significant Byte
#define SPD_XMP_TRTP          201 // XMP Minimum Internal Read to Precharge command delay
#define SPD_XMP_TRRD          202 // XMP Minimum Row active to row active delay
#define SPD_XMP_UN_TFAW       203 // XMP Upper Nibble for tFAW
#define SPD_XMP_TFAW          204 // XMP Minimum Four Activate Window Delay Time (tFAWmin)
#define SPD_XMP_TWTR          205 // XMP Minimum Internal Write to Read command delay
#define SPD_XMP_CMD           208 // XMP System CMD Rate Mode
#define SPD_XMP_ASR_PERF      209 // XMP SDRAM Auto Self Refresh Performance
#define SPD_XMP_MC_VOLT       210 // XMP Memory Controller Voltage Level
#define SPD_XMP_FTB_TCK       211 // XMP Fine Offset for Minimum Cycle Time (tCK)
#define SPD_XMP_FTB_TAA       212 // XMP Fine Offset for Minimum CAS Latency Time (tAA)
#define SPD_XMP_FTB_TRP       213 // XMP Fine Offset for Minimum Row Precharge Delay Time (tRP)
#define SPD_XMP_FTB_TRCD      214 // XMP Fine Offset for Minimum RAS to CAS Delay Time (tRCD)
#define SPD_XMP_FTB_TRC       215 // XMP Fine Offset for Minimum Active to Active/Refresh Delay Time (tRC)

//
// DDR4 XMP Offsets
//
#define SPD_XMP_ID_DDR4       384 // XMP Id String
#define SPD_XMP_ORG_DDR4      386 // XMP Organization and Configuration
#define SPD_XMP_REV_DDR4      387 // XMP Revision
#define SPD_XMP_TB_PROFILE_1  388 // XMP Timebase for Pofile 1 (MTB of 125ps, and FTP of 1ps)
#define SPD_XMP_TB_PROFILE_2  389 // XMP Timebase for Pofile 2 (MTB of 125ps, and FTP of 1ps)
#define SPD_XMP_VDD_DDR4      393 // XMP Module VDD Voltage Level
#define SPD_XMP_TCK_DDR4      396 // XMP Minimum cycle time (at max CL)
#define SPD_XMP_CAS_SUP_1     397 // XMP CAS Latencies Supported, First Byte
#define SPD_XMP_CAS_SUP_2     398 // XMP CAS Latencies Supported, Second Byte
#define SPD_XMP_CAS_SUP_3     399 // XMP CAS Latencies Supported, Third Byte
#define SPD_XMP_CAS_SUP_4     400 // XMP CAS Latencies Supported, Forth Byte
#define SPD_XMP_TAA_DDR4      401 // XMP Minimum CAS Latency Time (tAAmin)
#define SPD_XMP_TRCD_DDR4     402 // XMP Minimum RAS to CAS delay (tRCDmin)
#define SPD_XMP_TRP_DDR4      403 // XMP Minimum Row Precharge time (tRPmin)
#define SPD_XMP_TRC_TRAS_DDR4 404 // XMP Upper nibbles for min tRAS and tRC
#define SPD_XMP_TRAS_DDR4     405 // XMP Minimum Active to Precharge time
#define SPD_XMP_TRC_DDR4      406 // XMP Minimum Active to Active/Refresh time
#define SPD_XMP_TRFC1_LSB     407 // XMP Minimum Refresh Recovery Delay 1 Time Least Significant Byte
#define SPD_XMP_TRFC1_MSB     408 // XMP Minimum Refresh Recovery Delay 1 Time Most Significant Byte
#define SPD_XMP_TRFC2_LSB     409 // XMP Minimum Refresh Recovery Delay 2 Time Least Significant Byte
#define SPD_XMP_TRFC2_MSB     410 // XMP Minimum Refresh Recovery Delay 2 Time Most Significant Byte
#define SPD_XMP_TRFC4_LSB     411 // XMP Minimum Refresh Recovery Delay 4 Time Least Significant Byte
#define SPD_XMP_TRFC4_MSB     412 // XMP Minimum Refresh Recovery Delay 4 Time Most Significant Byte
#define SPD_XMP_UN_TFAW_DDR4  413 // XMP Upper Nibble for tFAW
#define SPD_XMP_TFAW_DDR4     414 // XMP Minimum Four Activate Window Delay Time (tFAWmin)
#define SPD_XMP_TRRDS_DDR4    415 // XMP Minimum Row Active to Row Active Delay Time, same bank group
#define SPD_XMP_TRRDL_DDR4    416 // XMP Minimum Row Active to Row Active Delay Time, different bank group
#define SPD_XMP_FTB_TRRDL     425 // XMP Fine Offset for Minimum Row Active to Row Active Delay Time, same bank group
#define SPD_XMP_FTB_TRRDS     426 // XMP Fine Offset for Minimum Row Active to Row Active Delay Time, different bank group
#define SPD_SMP_FTB_TRC_DDR4  427 // XMP Fine Offset for Minimum Active/Refresh Delay Time (tRC)
#define SPD_XMP_FTB_TRP_DDR4  428 // XMP Fine Offset for Minimum Row Precharge Delay Time (tRP)
#define SPD_XMP_FTB_TRCD_DDR4 429 // XMP Fine Offset for Minimum RAS to CAS Delay Time (tRCD)
#define SPD_XMP_FTB_TAA_DDR4  430 // XMP Fine Offset for Minimum CAS Latency Time (tAA)
#define SPD_XMP_FTB_TCK_DDR4  431 // XMP Fine Offset for Minimum Cycle Time (tCK)

#define SPD_INTEL_ID          250 // Intel DIMM serial number
#define SPD_INTEL_ID_DDR4     507 // Intel DIMM serial number DDR4
#define SPD_INTEL_DIMM_SERIAL_NUMBER   0x3FB  // Intel Dimm Serial Number DDR5 (Bytes 1019~1023)

//
// TS5111, TS5110 Serial Bus Thermal Sensor Specification (JC40.1 July 25, 2019 Rev 0.76 JESD302-1 JEDEC STANDARD)
//
#define TS5_MR0_DEVICE_TYPE_MSB        0x00  // "MR0"  0x00 ROE: Device Type MSB
#define TS5_MR1_DEVICE_TYPE_LSB        0x01  // "MR1"  0x01 ROE: Device Type LSB
#define TS5_MR2_DEVICE_REVISION        0x02  // "MR2"  0x02 ROE: Device Revision
#define TS5_MR3_VENDOR_ID_0            0x03  // "MR3"  0x03 ROE: Vendor ID Byte 0
#define TS5_MR4_VENDOR_ID_1            0x04  // "MR4"  0x04 ROE: Vendor ID Byte 1
#define TS5_MR7_DEV_HID                0x07  // "MR7"  0x07 RW: Device Configuration - HID
#define TS5_MR18_DEVICE_CONFIGURATION  0x12  // "MR18" 0x12 RW, RO Device Configuration
#define TS5_MR19_CLEAR_ERROR_CMD       0x13  // "MR19" 0x13 1O: Clear Register MR51 Error Status Command
#define TS5_MR27_INTEERUPT_CONFIG      0x1B  // "MR27" 0x1B 1O/RO/RW: Interrupt Configurations
#define TS5_MR28_TEMP_HIGH_0           0x1C  // "MR28" 0x1C RW: TS Temp High Limit Configuration - Low Byte
#define TS5_MR29_TEMP_HIGH_1           0x1D  // "MR29" 0x1D RW: TS Temp High Limit Configuration - High Byte
#define TS5_MR32_CRITICAL_TEMP_HIGH_0  0x20  // "MR32" 0x20 RW: TS Critical Temp High Limit Configuration - Low Byte
#define TS5_MR33_CRITICAL_TEMP_HIGH_1  0x21  // "MR33" 0x21 RW: TS Critical Temp High Limit Configuration - High Byte
#define TS5_MR34_CRITICAL_LOW_LIMIT_0  0x22  // "MR34" 0x22 RW: TS Critical Temp Low Limit Configuration - Low Byte
#define TS5_MR35_CRITICAL_LOW_LIMIT_1  0x23  // "MR35" 0x23 RW: TS Critical Temp Low Limit Configuration - High Byte
#define TS5_MR49_TEMP                  0x31  // "MR49" 0x31 RO: TS Current Sensed Temperature

typedef union {
  struct {
    UINT8 Reserved : 1;
    /* RV 0 MR18[0]: Reserved */

    UINT8 DEF_RD_ADDR_POINT_BL : 1;
    /*  RW 0
        MR18[1]: DEF_RD_ADDR_POINT_BL
        Burst Length for Read Pointer Address for PEC Calculation
        0 = 2 Bytes
        1 = 4 Bytes
    */

    UINT8 DEF_RD_ADDR_POINT_START : 2;
    /*  RW 0
        MR18[3:2]: DEF_RD_ADDR_POINT_START
        Default Read Pointer Starting Address7
        00 = Table 93, "MR49"
        01 = Reserved
        10 = Reserved
        11 = Reserved
    */

    UINT8 DEF_RD_ADDR_POINT_EN : 1;
    /*  RW 0
        MR18[4]: DEF_RD_ADDR_POINT_EN
        Default Read Address Pointer Enable
        0 = Disable Default Read Address Pointer (Address pointer is set by the Host)
        1 = Enable Default Read Address Pointer;Address selected by register bits [3:2]
    */

    UINT8 INF_SEL : 1;
    /*  RO 0
        MR18[5]: INF_SEL
        Interface Selection
        0 = I2C Protocol (Max speed of 1 MHz)
        1 = I3C Basic Protocol
    */

    UINT8 PAR_DIS : 1;
    /*  RW 0
        MR18[6]: PAR_DIS
        Parity (T bit) Disable
        0 = Enable
        1 = Disable
    */
    UINT8 PEC_EN : 1;
    /*  RW 0
        MR18[7]: PEC_EN
        PEC Enable
        0 = Disable
        1 = Enable
    */

  } Bits;
  UINT8 Data;
} TS5_MR18_DEVICE_CONFIGURATION_STRUCT;

//
// Mobile Platform Memory Module Thermal Sensor C-spec (JC-42.4 November 2004)
//
#define MTS_CAPABILITY  0
#define MTS_CONFIG      1
  #define MTS_CFG_EVENT_MODE  BIT0
  #define MTS_CFG_EVENT_POL   BIT1
  #define MTS_CFG_TCRIT_ONLY  BIT2
  #define MTS_CFG_EVENT_CTRL  BIT3

#define MTS_ALARM_UPPER 2
#define MTS_ALARM_LOWER 3
#define MTS_CRITICAL    4
#define MTS_TEMP        5
#define MTS_MFGID       6
#define MTS_DID_RID     7

#endif // _memregs_h
