/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#ifndef _DDR4_SPD_REGS_H_
#define _DDR4_SPD_REGS_H_

//
// DDR4 SPD Spec 4.0 Register Definitions
//

/*  Byte 0 (0x000): Number of Bytes Used / Number of Bytes in SPD Device  */

#define SPD_NUMBER_OF_BYTES_USED_NUMBER_OF_BYTES_IN_SPD_DEVICE_REG       0x0000

typedef union {
  struct {
    UINT8 spd_bytes_used : 4;
                            /* Bits[0:3]
                               SPD Bytes Used
                               Bit [3, 2, 1, 0] :
                               0000 = Undefined
                               0001 = 128
                               0010 = 256
                               0011 = 384
                               0100 = 512 All others reserved
                            */
    UINT8 spd_bytes_total : 3;
                            /* Bits[4:6]
                               SPD Bytes Total
                               Bit [6, 5, 4] :
                               000 = Undefined
                               001 = 256
                               010 = 512 All others reserved
                            */
    UINT8 reserved_7 : 1;
                            /* Bits[7]
                               Reserved
                               Reserved
                               must be coded as 0
                            */
  } Bits;
  UINT8 Data;
} NUMBER_OF_BYTES_USED_NUMBER_OF_BYTES_IN_SPD_DEVICE_STRUCT;

/*  Byte 1 (0x001): Revision  */

#define SPD_REVISION_REG       0x0001

typedef union {
  struct {
    UINT8 additions_level : 4;
                            /* Bits[0:3]
                               Additions Level
                            */
    UINT8 encoding_level : 4;
                            /* Bits[4:7]
                               Encoding Level
                            */
  } Bits;
  UINT8 Data;
} REVISION_STRUCT;

/*  Byte 2 (0x002): Key Byte / DRAM Device Type  */

#define SPD_KEY_BYTE_DRAM_DEVICE_TYPE_REG       0x0002

typedef union {
  struct {
    UINT8 sdram_module_type : 8;
                            /* Bits[0:7]
                               SDRAM / Module Type
                            */
  } Bits;
  UINT8 Data;
} KEY_BYTE_DRAM_DEVICE_TYPE_STRUCT;

/*  Byte 3 (0x003): Key Byte / Module Type  */

#define SPD_KEY_BYTE_MODULE_TYPE_REG       0x0003

typedef union {
  struct {
    UINT8 base_module_type : 4;
                            /* Bits[0:3]
                               Base Module Type
                               Bits [3, 2, 1, 0]:
                               0000 = Extended DIMM type, see byte 15 (0x00F)
                               0001 = RDIMM
                               0010 = UDIMM
                               0011 = SO-DIMM
                               0100 = LRDIMM
                               0101 = Mini-RDIMM
                               0110 = Mini-UDIMM
                               0111 = Reserved
                               1000 = 72b-SO-RDIMM
                               1001 = 72b-SO-UDIMM
                               1010 = Reserved
                               1011 - Reserved
                               1100 = 16b-SO-DIMM
                               1101 = 32b-SO-DIMM
                               1110 = Reserved
                               1111 = Reserved
                            */
    UINT8 hybrid_media : 3;
                            /* Bits[4:6]
                               Hybrid Media
                               Bits [6, 5, 4]:
                               000 = Not hybrid
                               001 = NVDIMM Hybrid
                               All other codes reserved
                            */
    UINT8 hybrid : 1;
                            /* Bits[7]
                               Hybrid
                               0 = Not hybrid
                               (Module is DRAM only)
                               1 = Hybrid module
                               (See bits 6~4 for hybrid type)
                            */
  } Bits;
  UINT8 Data;
} KEY_BYTE_MODULE_TYPE_STRUCT;

/*  Byte 4 (0x004): SDRAM Density and Banks  */

#define SPD_SDRAM_DENSITY_AND_BANKS_REG       0x0004

typedef union {
  struct {
    UINT8 total_sdram_capacity_per_die_in_megabits : 4;
                            /* Bits[0:3]
                               Total SDRAM capacity per die, in megabits
                               Bit [3, 2, 1, 0] :
                               0000 = 256 Mb
                               0001 = 512 Mb
                               0010 = 1 Gb
                               0011 = 2 Gb
                               0100 = 4 Gb
                               0101 = 8 Gb
                               0110 = 16 Gb
                               0111 = 32 Gb
                               1000 = 12 Gb
                               1001 = 24 Gb All others reserved
                            */
    UINT8 bank_address_bits : 2;
                            /* Bits[4:5]
                               Bank Address Bits
                               Bit [5, 4]:
                               00 = 2 (4 banks)
                               01 = 3 (8 banks) All others reserved
                            */
    UINT8 bank_group_bits : 2;
                            /* Bits[6:7]
                               Bank Group Bits
                               Bits [7, 6]:
                               00 = 0 (no bank groups)
                               01 = 1 (2 bank groups)
                               10 = 2 (4 bank groups)
                               11 = reserved
                            */
  } Bits;
  UINT8 Data;
} SDRAM_DENSITY_AND_BANKS_STRUCT;

/*  Byte 5 (0x005): SDRAM Addressing  */

#define SPD_SDRAM_ADDRESSING_REG       0x0005

typedef union {
  struct {
    UINT8 column_address_bits : 3;
                            /* Bits[0:2]
                               Column Address Bits
                               Bit [2, 1, 0] :
                               000 = 9
                               001 = 10
                               010 = 11
                               011 = 12 All others reserved
                            */
    UINT8 row_address_bits : 3;
                            /* Bits[3:5]
                               Row Address Bits
                               Bit [5, 4, 3] :
                               000 = 12
                               001 = 13
                               010 = 14
                               011 = 15
                               100 = 16
                               101 = 17
                               110 = 18 All others reserved
                            */
    UINT8 reserved_6_7 : 2;
                            /* Bits[6:7]
                               Reserved
                               Reserved
                               must be coded as 00
                            */
  } Bits;
  UINT8 Data;
} SDRAM_ADDRESSING_STRUCT;

/*  Byte 6 (0x006): Primary SDRAM Package Type  */

#define SPD_PRIMARY_SDRAM_PACKAGE_TYPE_REG       0x0006

typedef union {
  struct {
    UINT8 signal_loading : 2;
                            /* Bits[0:1]
                               Signal Loading
                               00 = Not specified
                               01 = Multi load stack
                               10 = Single load stack (3DS)
                               11 = Reserved
                            */
    UINT8 reserved_2_3 : 2;
                            /* Bits[2:3]
                               Reserved
                               00
                            */
    UINT8 die_count : 3;
                            /* Bits[4:6]
                               Die Count
                               000 = Single die
                               001 = 2 die
                               010 = 3 die
                               011 = 4 die
                               100 = 5 die
                               101 = 6 die
                               110 = 7 die
                               111 = 8 die
                            */
    UINT8 primary_sdram_package_type : 1;
                            /* Bits[7]
                               Primary SDRAM Package Type
                               0 = Monolithic DRAM Device
                               1 = Non-Monolithic Device
                            */
  } Bits;
  UINT8 Data;
} PRIMARY_SDRAM_PACKAGE_TYPE_STRUCT;

/*  Byte 7 (0x007): SDRAM Optional Features  */

#define SPD_SDRAM_OPTIONAL_FEATURES_REG       0x0007

typedef union {
  struct {
    UINT8 maximum_activate_count_mac : 4;
                            /* Bits[0:3]
                               Maximum Activate Count (MAC)
                               Bits [3, 2, 1, 0] :
                               0000 = Untested MAC
                               0001 = 700 K
                               0010 = 600 K
                               0011 = 500 K
                               0100 = 400 K
                               0101 = 300 K
                               0110 = 200 K
                               0111 = Reserved
                               1000 = Unlimited MAC All others reserved
                            */
    UINT8 maximum_activate_window_tmaw : 2;
                            /* Bits[4:5]
                               Maximum Activate Window (tMAW)
                               Bits [5, 4]:
                               00 = 8192 * tREFI
                               01 = 4096 * tREFI
                               10 = 2048 * tREFI
                               11 = Reserved
                            */
    UINT8 reserved_6_7 : 2;
                            /* Bits[6:7]
                               Reserved
                               Reserved
                               must be coded as 00
                            */
  } Bits;
  UINT8 Data;
} SDRAM_OPTIONAL_FEATURES_STRUCT;

/*  Byte 8 (0x008): SDRAM Thermal and Refresh Options  */

#define SPD_SDRAM_THERMAL_AND_REFRESH_OPTIONS_REG       0x0008

typedef union {
  struct {
    UINT8 reserved_0_7 : 8;
                            /* Bits[0:7]
                               Reserved
                               Reserved
                               must be coded as 0x00
                            */
  } Bits;
  UINT8 Data;
} SDRAM_THERMAL_AND_REFRESH_OPTIONS_STRUCT;

/*  Byte 9 (0x009): Other SDRAM Optional Features  */

#define SPD_OTHER_SDRAM_OPTIONAL_FEATURES_REG       0x0009

typedef union {
  struct {
    UINT8 reserved_0_4 : 5;
                            /* Bits[0:4]
                               Reserved
                               Reserved
                               must be coded as 00000
                            */
    UINT8 soft_ppr : 1;
                            /* Bits[5]
                               Soft PPR
                               0 = Soft PPR not supported
                               1 = Soft PPR supported
                            */
    UINT8 post_package_repair_ppr : 2;
                            /* Bits[6:7]
                               Post Package Repair (PPR)
                               00: PPR not supported
                               01: Post package repair supported, one row per bank group
                               10: Reserved
                               11: Reserved
                            */
  } Bits;
  UINT8 Data;
} OTHER_SDRAM_OPTIONAL_FEATURES_STRUCT;

/*  Byte 10 (0x00A): Secondary SDRAM Package Type  */

#define SPD_SECONDARY_SDRAM_PACKAGE_TYPE_REG       0x000A

typedef union {
  struct {
    UINT8 signal_loading : 2;
                            /* Bits[0:1]
                               Signal Loading
                               00 = Not specified
                               01 = Multi load stack
                               10 = Single load stack (3DS)
                               11 = Reserved
                            */
    UINT8 dram_density_ratio : 2;
                            /* Bits[2:3]
                               DRAM Density Ratio
                               00 = Rank 1 and 3 device densities are the same as rank 0 and 2 densities
                               01 = Rank 1 and 3 are one standard device density smaller than rank 0 and 2
                               10 = Rank 1 and 3 are two standard device densities smaller than rank 0 and 2
                               11 = Reserved
                            */
    UINT8 die_count : 3;
                            /* Bits[4:6]
                               Die Count
                               000 = Single die
                               001 = 2 die
                               010 = 3 die
                               011 = 4 die
                               100 = 5 die
                               101 = 6 die
                               110 = 7 die
                               111 = 8 die
                            */
    UINT8 sdram_package_type : 1;
                            /* Bits[7]
                               SDRAM Package Type
                               0 = Monolithic DRAM Device
                               1 = Non-Monolithic Device
                            */
  } Bits;
  UINT8 Data;
} SECONDARY_SDRAM_PACKAGE_TYPE_STRUCT;

/*  Byte 11 (0x00B): Module Nominal Voltage, VDD  */

#define SPD_MODULE_NOMINAL_VOLTAGE_VDD_REG       0x000B

typedef union {
  struct {
    UINT8 dram_vdd_1_2_v : 2;
                            /* Bits[0:1]
                               DRAM VDD 1.2 V
                               Bit 1:
                               0 = not endurant
                               1 = endurant
                               Bit 0:
                               0 = not operable
                               1 = operable
                            */
    UINT8 reserved_2_7 : 6;
                            /* Bits[2:7]
                               Reserved
                               Reserved
                               must be coded as 000000
                            */
  } Bits;
  UINT8 Data;
} MODULE_NOMINAL_VOLTAGE_VDD_STRUCT;

/*  Byte 12 (0x00C): Module Organization  */

#define SPD_MODULE_ORGANIZATION_REG       0x000C

typedef union {
  struct {
    UINT8 sdram_device_width : 3;
                            /* Bits[0:2]
                               SDRAM Device Width
                               Bit [2, 1, 0] :
                               000 = 4 bits
                               001 = 8 bits
                               010 = 16 bits
                               011 = 32 bits All others reserved
                            */
    UINT8 number_of_package_ranks_per_dimm : 3;
                            /* Bits[3:5]
                               Number of Package Ranks per DIMM
                               Bit [5, 4, 3] :
                               000 = 1 Package Rank
                               001 = 2 Package Ranks
                               010 = 3 Package Ranks
                               011 = 4 Package Ranks
                               100 = 5 Package Ranks
                               101 = 6 Package Ranks
                               110 = 7 Package Ranks
                               111 = 8 Package Ranks
                            */
    UINT8 rank_mix : 1;
                            /* Bits[6]
                               Rank Mix
                               0 = Symmetrical
                               1 = Asymmetrical
                            */
    UINT8 reserved_7 : 1;
                            /* Bits[7]
                               Reserved
                               Reserved
                               must be coded as 0
                            */
  } Bits;
  UINT8 Data;
} MODULE_ORGANIZATION_STRUCT;

/*  Byte 13 (0x00D): Module Memory Bus Width  */

#define SPD_MODULE_MEMORY_BUS_WIDTH_REG       0x000D

typedef union {
  struct {
    UINT8 primary_bus_width_in_bits : 3;
                            /* Bits[0:2]
                               Primary bus width, in bits
                               Bit [2, 1, 0] :
                               000 = 8 bits
                               001 = 16 bits
                               010 = 32 bits
                               011 = 64 bits All others reserved
                            */
    UINT8 bus_width_extension_in_bits : 2;
                            /* Bits[3:4]
                               Bus width extension, in bits
                               Bit [4, 3] :
                               000 = 0 bits (no extension)
                               001 = 8 bits All others reserved
                            */
    UINT8 reserved_5_7 : 3;
                            /* Bits[5:7]
                               Reserved
                               Reserved
                               must be coded as 000
                            */
  } Bits;
  UINT8 Data;
} MODULE_MEMORY_BUS_WIDTH_STRUCT;

/*  Byte 14 (0x00E): Module Thermal Sensor  */

#define SPD_MODULE_THERMAL_SENSOR_REG       0x000E

typedef union {
  struct {
    UINT8 reserved_0_6 : 7;
                            /* Bits[0:6]
                               Reserved
                               0 = Undefined
                               All others reserved
                            */
    UINT8 thermal_sensor : 1;
                            /* Bits[7]
                               Thermal Sensor
                               0 = Thermal sensor not incorporated onto this assembly
                               1 = Thermal sensor incorporated onto this assembly
                            */
  } Bits;
  UINT8 Data;
} MODULE_THERMAL_SENSOR_STRUCT;

/*  Byte 15 (0x00F): Extended Module Type  */

#define SPD_EXTENDED_MODULE_TYPE_REG       0x000F

typedef union {
  struct {
    UINT8 extended_base_module_type : 4;
                            /* Bits[0:3]
                               Extended Base Module Type
                               Bits [3, 2, 1, 0]:
                               0000 = Reserved
                               must be coded as 0000 ...
                               1111 = Reserved
                               must be coded as 0000
                            */
    UINT8 reserved_4_7 : 4;
                            /* Bits[4:7]
                               Reserved
                               Reserved
                               must be coded as 0000
                            */
  } Bits;
  UINT8 Data;
} EXTENDED_MODULE_TYPE_STRUCT;

/*  Byte 17 (0x011): Timebases  */

#define SPD_TIMEBASES_REG       0x0011

typedef union {
  struct {
    UINT8 fine_timebase_ftb : 2;
                            /* Bits[0:1]
                               Fine Timebase (FTB)
                               Bits [0, 1]:
                               00 = 1 ps All others reserved
                            */
    UINT8 medium_timebase_mtb : 2;
                            /* Bits[2:3]
                               Medium Timebase (MTB)
                               Bits [3, 2]:
                               00 = 125 ps All others reserved
                            */
    UINT8 reserved_4_7 : 4;
                            /* Bits[4:7]
                               Reserved
                               Reserved
                               must be coded as 0000
                            */
  } Bits;
  UINT8 Data;
} TIMEBASES_STRUCT;

/*  Byte 18 (0x012): SDRAM Minimum Cycle Time (tCKAVGmin)  */

#define SPD_SDRAM_MINIMUM_CYCLE_TIME_TCKAVGMIN_REG       0x0012

typedef union {
  struct {
    UINT8 minimum_sdram_cycle_time_tckavgmin_mtb_units : 8;
                            /* Bits[0:7]
                               Minimum SDRAM Cycle Time (tCKAVGmin) MTB Units
                               Values defined from 1 to 255
                            */
  } Bits;
  UINT8 Data;
} SDRAM_MINIMUM_CYCLE_TIME_TCKAVGMIN_STRUCT;

/*  Byte 19 (0x013): SDRAM Maximum Cycle Time (tCKAVGmax)  */

#define SPD_SDRAM_MAXIMUM_CYCLE_TIME_TCKAVGMAX_REG       0x0013

typedef union {
  struct {
    UINT8 minimum_sdram_cycle_time_tckavgmax_mtb_units : 8;
                            /* Bits[0:7]
                               Minimum SDRAM Cycle Time (tCKAVGmax) MTB Units
                               Values defined from 1 to 255
                            */
  } Bits;
  UINT8 Data;
} SDRAM_MAXIMUM_CYCLE_TIME_TCKAVGMAX_STRUCT;

/*  Byte 20 (0x014): CAS Latencies Supported  */

#define SPD_CAS_LATENCIES_SUPPORTED_REG       0x0014

typedef union {
  struct {
    UINT8 cl : 8;
                            /* Bits[0:7]
                               cl
                               Low range:
                               Bit[7] CL = 14
                               Bit[6] CL = 13
                               Bit[5] CL = 12
                               Bit[4] CL = 11
                               Bit[3] CL = 10
                               Bit[2] CL = 9
                               Bit[1] CL = 8
                               Bit[0] CL = 7
                                High rang:
                               Bit[7] CL = 30
                               Bit[6] CL = 29
                               Bit[5] CL = 28
                               Bit[4] CL = 27
                               Bit[3] CL = 26
                               Bit[2] CL = 25
                               Bit[1] CL = 24
                               Bit[0] CL = 23
                            */
  } Bits;
  UINT8 Data;
} CAS_LATENCIES_SUPPORTED_STRUCT;

/*  Byte 21 (0x015): CAS Latencies Supported  */

#define SPD_CAS_LATENCIES_SUPPORTED_0_REG       0x0015

typedef union {
  struct {
    UINT8 cl : 8;
                            /* Bits[0:7]
                               cl
                               Low range:
                               Bit[7] CL = 22
                               Bit[6] CL = 21
                               Bit[5] CL = 20
                               Bit[4] CL = 19
                               Bit[3] CL = 18
                               Bit[2] CL = 17
                               Bit[1] CL = 16
                               Bit[0] CL = 15
                               High range:
                               Bit[7] CL = 38
                               Bit[6] CL = 37
                               Bit[5] CL = 36
                               Bit[4] CL = 35
                               Bit[3] CL = 34
                               Bit[2] CL = 33
                               Bit[1] CL = 32
                               Bit[0] CL = 31
                            */
  } Bits;
  UINT8 Data;
} CAS_LATENCIES_SUPPORTED_0_STRUCT;

/*  Byte 22 (0x016): CAS Latencies Supported  */

#define SPD_CAS_LATENCIES_SUPPORTED_1_REG       0x0016

typedef union {
  struct {
    UINT8 cl : 8;
                            /* Bits[0:7]
                               cl
                               Low range:
                               Bit[7] CL = 30
                               Bit[6] CL = 29
                               Bit[5] CL = 28
                               Bit[4] CL = 27
                               Bit[3] CL = 26
                               Bit[2] CL = 25
                               Bit[1] CL = 24
                               Bit[0] CL = 23
                               High range:
                               Bit[7] CL =46
                               Bit[6] CL = 45
                               Bit[5] CL = 44
                               Bit[4] CL = 43
                               Bit[3] CL = 42
                               Bit[2] CL = 41
                               Bit[1] CL = 40
                               Bit[0] CL = 39
                            */
  } Bits;
  UINT8 Data;
} CAS_LATENCIES_SUPPORTED_1_STRUCT;

/*  Byte 23 (0x017): CAS Latencies Supported  */

#define SPD_CAS_LATENCIES_SUPPORTED_2_REG       0x0017

typedef union {
  struct {
    UINT8 cl_range : 1;
                            /* Bits[7]
                               cl_range
                               0 = Low CL range
                               1 = High CL range
                            */
    UINT8 reserved_6 : 1;
                            /* Bits[6]
                               Reserved
                               Bit[6] Rsvd
                            */
    UINT8 cl : 6;
                            /* Bits[0:5]
                               cl
                               Low range:
                               Bit[5] CL = 36
                               Bit[4] CL = 35
                               Bit[3] CL = 34
                               Bit[2] CL = 33
                               Bit[1] CL = 32
                               Bit[0] CL = 31
                               High range:
                               Bit[5] CL = 52
                               Bit[4] CL = 51
                               Bit[3] CL = 50
                               Bit[2] CL = 49
                               Bit[1] CL = 48
                               Bit[0] CL = 47
                            */
  } Bits;
  UINT8 Data;
} CAS_LATENCIES_SUPPORTED_2_STRUCT;

/*  Byte 24 (0x018): Minimum CAS Latency Time (tAAmin)  */

#define SPD_MINIMUM_CAS_LATENCY_TIME_TAAMIN_REG       0x0018

typedef union {
  struct {
    UINT8 minimum_sdram_cas_latency_time_taamin_mtb_units : 8;
                            /* Bits[0:7]
                               Minimum SDRAM CAS Latency Time (tAAmin) MTB Units
                               Values defined from 1 to 255
                            */
  } Bits;
  UINT8 Data;
} MINIMUM_CAS_LATENCY_TIME_TAAMIN_STRUCT;

/*  Byte 25 (0x019): Minimum RAS to CAS Delay Time (tRCDmin)  */

#define SPD_MINIMUM_RAS_TO_CAS_DELAY_TIME_TRCDMIN_REG       0x0019

typedef union {
  struct {
    UINT8 min_sdram_ras_to_cas_delay_time_trcd : 8;
                            /* Bits[0:7]
                               Byte 25: Minimum SDRAM RAS to CAS Delay Time (tRCDmin) MTB Units
                               Values defined from 1 to 255
                            */
  } Bits;
  UINT8 Data;
} MINIMUM_RAS_TO_CAS_DELAY_TIME_TRCDMIN_STRUCT;

/*  Byte 26 (0x01A): Minimum Row Precharge Delay Time (tRPmin)  */

#define SPD_MINIMUM_ROW_PRECHARGE_DELAY_TIME_TRPMIN_REG       0x001A

typedef union {
  struct {
    UINT8 minimum_row_precharge_time_trpmin_mtb_units : 8;
                            /* Bits[0:7]
                               Minimum Row Precharge Time (tRPmin) MTB Units
                               Values defined from 1 to 255
                            */
  } Bits;
  UINT8 Data;
} MINIMUM_ROW_PRECHARGE_DELAY_TIME_TRPMIN_STRUCT;

/*  Byte 27 (0x01B): Upper Nibbles for tRASmin and tRCmin  */

#define SPD_UPPER_NIBBLES_FOR_TRASMIN_AND_TRCMIN_REG       0x001B

typedef union {
  struct {
    UINT8 trasmin_most_significant_nibble : 4;
                            /* Bits[0:3]
                               tRASmin Most Significant Nibble
                               See Byte 29 description
                            */
    UINT8 trcmin_most_significant_nibble : 4;
                            /* Bits[4:7]
                               tRCmin Most Significant Nibble
                               See Byte 28 description
                            */
  } Bits;
  UINT8 Data;
} UPPER_NIBBLES_FOR_TRASMIN_AND_TRCMIN_STRUCT;

/*  Byte 28 (0x01C): Minimum Active to Precharge Delay Time (tRASmin), Least Significant Byte  */

#define SPD_MINIMUM_ACTIVE_TO_PRECHARGE_DELAY_TIME_TRASMIN_LSB_REG       0x001C

typedef union {
  struct {
    UINT8 minimum_active_to_precharge_time_trasmin_mtb_units : 8;
                            /* Bits[0:7]
                               Minimum Active to Precharge Time (tRASmin) MTB Units
                               Values defined from 1 to 4095
                            */
  } Bits;
  UINT8 Data;
} MINIMUM_ACTIVE_TO_PRECHARGE_DELAY_TIME_TRASMIN_LSB_STRUCT;

/*  Byte 29 (0x01D): Minimum Active to Active/Refresh Delay Time (tRCmin), Least Significant Byte  */

#define SPD_MIN_ACTIVE_TO_ACTIVE_REFRESH_DELAY_TIME_TRCMIN_LSB_REG       0x001D

typedef union {
  struct {
    UINT8 min_active_to_active_refresh_time_trc : 8;
                            /* Bits[0:7]
                               Minimum Active to Active/Refresh Time (tRCmin) MTB Units
                               Values defined from 1 to 4095
                            */
  } Bits;
  UINT8 Data;
} MIN_ACTIVE_TO_ACTIVE_REFRESH_DELAY_TIME_TRCMIN_LSB_STRUCT;

/*  Byte 30 (0x01E): Minimum Refresh Recovery Delay Time (tRFC1min), LSB  */

#define SPD_MINIMUM_REFRESH_RECOVERY_DELAY_TIME_TRFC1MIN_LSB_REG       0x001E

typedef union {
  struct {
    UINT8 min_sdram_refresh_recovery_delay_trfc1 : 8;
                            /* Bits[0:7]
                               Minimum SDRAM Refresh Recovery Delay Time (tRFC1min) MTB Units
                            */
  } Bits;
  UINT8 Data;
} MINIMUM_REFRESH_RECOVERY_DELAY_TIME_TRFC1MIN_LSB_STRUCT;

/*  Byte 31 (0x01F): Minimum Refresh Recovery Delay Time (tRFC1min), MSB  */

#define SPD_MINIMUM_REFRESH_RECOVERY_DELAY_TIME_TRFC1MIN_MSB_REG       0x001F

typedef union {
  struct {
    UINT8 min_sdram_refresh_recovery_delay_trfc1 : 8;
                            /* Bits[0:7]
                               Minimum SDRAM Refresh Recovery Delay Time (tRFC1min) MTB Units
                            */
  } Bits;
  UINT8 Data;
} MINIMUM_REFRESH_RECOVERY_DELAY_TIME_TRFC1MIN_MSB_STRUCT;

/*  Byte 32 (0x020): Minimum Refresh Recovery Delay Time (tRFC2min), LSB  */

#define SPD_MINIMUM_REFRESH_RECOVERY_DELAY_TIME_TRFC2MIN_LSB_REG       0x0020

typedef union {
  struct {
    UINT8 min_sdram_refresh_recovery_delay_trfc2 : 8;
                            /* Bits[0:7]
                               Minimum SDRAM Refresh Recovery Delay Time (tRFC2min) MTB Units
                            */
  } Bits;
  UINT8 Data;
} MINIMUM_REFRESH_RECOVERY_DELAY_TIME_TRFC2MIN_LSB_STRUCT;

/*  Byte 33 (0x021): Minimum Refresh Recovery Delay Time (tRFC2min), MSB  */

#define SPD_MINIMUM_REFRESH_RECOVERY_DELAY_TIME_TRFC2MIN_MSB_REG       0x0021

typedef union {
  struct {
    UINT8 min_sdram_refresh_recovery_delay_trfc2 : 8;
                            /* Bits[0:7]
                               Minimum SDRAM Refresh Recovery Delay Time (tRFC2min) MTB Units
                            */
  } Bits;
  UINT8 Data;
} MINIMUM_REFRESH_RECOVERY_DELAY_TIME_TRFC2MIN_MSB_STRUCT;

/*  Byte 34 (0x022): Minimum Refresh Recovery Delay Time (tRFC4min), LSB  */

#define SPD_MINIMUM_REFRESH_RECOVERY_DELAY_TIME_TRFC4MIN_LSB_REG       0x0022

typedef union {
  struct {
    UINT8 min_sdram_refresh_recovery_delay_trfc4 : 8;
                            /* Bits[0:7]
                               Minimum SDRAM Refresh Recovery Delay Time (tRFC4min) MTB Units
                            */
  } Bits;
  UINT8 Data;
} MINIMUM_REFRESH_RECOVERY_DELAY_TIME_TRFC4MIN_LSB_STRUCT;

/*  Byte 35 (0x023): Minimum Refresh Recovery Delay Time (tRFC4min), MSB  */

#define SPD_MINIMUM_REFRESH_RECOVERY_DELAY_TIME_TRFC4MIN_MSB_REG       0x0023

typedef union {
  struct {
    UINT8 min_sdram_refresh_recovery_delay_trfc4 : 8;
                            /* Bits[0:7]
                               Minimum SDRAM Refresh Recovery Delay Time (tRFC4min) MTB Units
                            */
  } Bits;
  UINT8 Data;
} MINIMUM_REFRESH_RECOVERY_DELAY_TIME_TRFC4MIN_MSB_STRUCT;

/*  Byte 36 (0x024): Upper Nibble for tFAW  */

#define SPD_UPPER_NIBBLE_FOR_TFAW_REG       0x0024

typedef union {
  struct {
    UINT8 tfaw_most_significant_nibble : 4;
                            /* Bits[0:3]
                               tFAW Most Significant Nibble
                               See Byte 37 description
                            */
    UINT8 reserved_4_7 : 4;
                            /* Bits[4:7]
                               Reserved
                               Reserved
                               most be coded as 0000
                            */
  } Bits;
  UINT8 Data;
} UPPER_NIBBLE_FOR_TFAW_STRUCT;

/*  Byte 37 (0x025): Minimum Four Activate Window Delay Time (tFAWmin), Least Significant Byte  */

#define SPD_MINIMUM_FOUR_ACTIVATE_WINDOW_DELAY_TIME_TFAWMIN_LSB_REG       0x0025

typedef union {
  struct {
    UINT8 min_four_activate_window_delay_tfaw : 8;
                            /* Bits[0:7]
                               Minimum Four Activate Window Delay Time (tFAW) MTB Units
                               Values defined from 1 to 4095
                            */
  } Bits;
  UINT8 Data;
} MINIMUM_FOUR_ACTIVATE_WINDOW_DELAY_TIME_TFAWMIN_LSB_STRUCT;

/*  Byte 38 (0x026): Minimum Activate to Activate Delay Time (tRRDSmin), different bank group  */

#define SPD_MIN_ACTIVATE_TO_ACTIVATE_DELAY_TRRDS_DIFF_BANKS_REG       0x0026

typedef union {
  struct {
    UINT8 min_active_to_active_refresh_time_trrds : 8;
                            /* Bits[0:7]
                               Minimum Active to Active/Refresh Time (tRRD_Smin) MTB Units
                               Values defined from 1 to 255
                            */
  } Bits;
  UINT8 Data;
} MIN_ACTIVATE_TO_ACTIVATE_DELAY_TRRDS_DIFF_BANKS_STRUCT;

/*  Byte 39 (0x027): Minimum Activate to Activate Delay Time (tRRDLmin), same bank group  */

#define SPD_MIN_ACTIVATE_TO_ACTIVATE_DELAY_TRRDL_SAME_BANKS_REG       0x0027

typedef union {
  struct {
    UINT8 min_active_to_active_refresh_time_trrdl : 8;
                            /* Bits[0:7]
                               Minimum Active to Active/Refresh Time (tRRD_Lmin) MTB Units
                               Values defined from 1 to 255
                            */
  } Bits;
  UINT8 Data;
} MIN_ACTIVATE_TO_ACTIVATE_DELAY_TRRDL_SAME_BANKS_STRUCT;

/*  Byte 40 (0x028): Minimum CAS to CAS Delay Time (tCCDLmin), same bank group  */

#define SPD_MIN_CAS_TO_CAS_DELAY_TCCDL_SAME_BANKS_REG       0x0028

typedef union {
  struct {
    UINT8 minimum_cas_to_cas_time_tccd_lmin_mtb_units : 8;
                            /* Bits[0:7]
                               Minimum CAS to CAS Time (tCCD_Lmin) MTB Units
                               Values defined from 1 to 255
                            */
  } Bits;
  UINT8 Data;
} MIN_CAS_TO_CAS_DELAY_TCCDL_SAME_BANKS_STRUCT;

/*  Byte 41 (0x029): Upper Nibble for tWRmin  */

#define SPD_UPPER_NIBBLE_FOR_TWRMIN_REG       0x0029

typedef union {
  struct {
    UINT8 twrmin_most_significant_nibble : 4;
                            /* Bits[0:3]
                               tWRmin Most Significant Nibble
                               See Byte 42 description
                            */
    UINT8 reserved_4_7 : 4;
                            /* Bits[4:7]
                               Reserved
                               Reserved
                               must be coded as 0000
                            */
  } Bits;
  UINT8 Data;
} UPPER_NIBBLE_FOR_TWRMIN_STRUCT;

/*  Byte 42 (0x02A): Minimum Write Recovery Time (tWRmin)  */

#define SPD_MINIMUM_WRITE_RECOVERY_TIME_TWRMIN_REG       0x002A

typedef union {
  struct {
    UINT8 minimum_write_recovery_time_twrmin_mtb_units : 8;
                            /* Bits[0:7]
                               Minimum Write Recovery Time (tWRmin) MTB Units
                               Values defined from 1 to 4095
                            */
  } Bits;
  UINT8 Data;
} MINIMUM_WRITE_RECOVERY_TIME_TWRMIN_STRUCT;

/*  Byte 43 (0x02B): Upper Nibbles for tWTRmin  */

#define SPD_UPPER_NIBBLES_FOR_TWTRMIN_REG       0x002B

typedef union {
  struct {
    UINT8 twtr_smin_most_significant_nibble : 4;
                            /* Bits[0:3]
                               tWTR_Smin Most Significant Nibble
                               See Byte 44 description
                            */
    UINT8 twtr_lmin_most_significant_nibble : 4;
                            /* Bits[4:7]
                               tWTR_Lmin Most Significant Nibble
                               See Byte 45 description
                            */
  } Bits;
  UINT8 Data;
} UPPER_NIBBLES_FOR_TWTRMIN_STRUCT;

/*  Byte 44 (0x02C): Minimum Write to Read Time (tWTRSmin), different bank group  */

#define SPD_MIN_WRITE_TO_READ_TIME_TWTRS_DIFF_BANKS_REG       0x002C

typedef union {
  struct {
    UINT8 min_write_recovery_time_twtrs_diff_banks : 8;
                            /* Bits[0:7]
                               Minimum Write Recovery Time (tWTR_Smin), different bank group MTB Units
                               Values defined from 1 to 4095
                            */
  } Bits;
  UINT8 Data;
} MIN_WRITE_TO_READ_TIME_TWTRS_DIFF_BANKS_STRUCT;

/*  Byte 45 (0x02D): Minimum Write to Read Time (tWTRLmin), same bank group  */

#define SPD_MIN_WRITE_TO_READ_TIME_TWTR_LMIN_SAME_BANK_GROUP_REG       0x002D

typedef union {
  struct {
    UINT8 min_write_recovery_time_twtrl_same_banks : 8;
                            /* Bits[0:7]
                               Minimum Write Recovery Time (tWTR_Lmin), same bank group MTB Units
                               Values defined from 1 to 4095
                            */
  } Bits;
  UINT8 Data;
} MIN_WRITE_TO_READ_TIME_TWTR_LMIN_SAME_BANK_GROUP_STRUCT;

/*  Bytes 46~59 (0x02E~0x03B): Reserved, Base Configuration Section
Byte 46 (0x2E)  Reserved, Base Configuration Section  */

#define SPD_RESERVED_BASE_CONFIGURATION_SECTION_0_REG       0x002E

typedef union {
  struct {
    UINT8 reserved_0_7 : 8;
                            /* Bits[0:7]
                               Reserved
                               Must be coded as 0x00
                            */
  } Bits;
  UINT8 Data;
} RESERVED_BASE_CONFIGURATION_SECTION_0_STRUCT;

/*  Bytes 46~59 (0x02E~0x03B): Reserved, Base Configuration Section
Byte 47 (0x2F)  Reserved, Base Configuration Section  */

#define SPD_RESERVED_BASE_CONFIGURATION_SECTION_1_REG       0x002F

typedef union {
  struct {
    UINT8 reserved_0_7 : 8;
                            /* Bits[0:7]
                               Reserved
                               Must be coded as 0x00
                            */
  } Bits;
  UINT8 Data;
} RESERVED_BASE_CONFIGURATION_SECTION_1_STRUCT;

/*  Bytes 46~59 (0x02E~0x03B): Reserved, Base Configuration Section
Byte 48 (0x30)  Reserved, Base Configuration Section  */

#define SPD_RESERVED_BASE_CONFIGURATION_SECTION_2_REG       0x0030

typedef union {
  struct {
    UINT8 reserved_0_7 : 8;
                            /* Bits[0:7]
                               Reserved
                               Must be coded as 0x00
                            */
  } Bits;
  UINT8 Data;
} RESERVED_BASE_CONFIGURATION_SECTION_2_STRUCT;

/*  Bytes 46~59 (0x02E~0x03B): Reserved, Base Configuration Section
Byte 49 (0x31)  Reserved, Base Configuration Section  */

#define SPD_RESERVED_BASE_CONFIGURATION_SECTION_3_REG       0x0031

typedef union {
  struct {
    UINT8 reserved_0_7 : 8;
                            /* Bits[0:7]
                               Reserved
                               Must be coded as 0x00
                            */
  } Bits;
  UINT8 Data;
} RESERVED_BASE_CONFIGURATION_SECTION_3_STRUCT;

/*  Bytes 46~59 (0x02E~0x03B): Reserved, Base Configuration Section
Byte 50 (0x32)  Reserved, Base Configuration Section  */

#define SPD_RESERVED_BASE_CONFIGURATION_SECTION_4_REG       0x0032

typedef union {
  struct {
    UINT8 reserved_0_7 : 8;
                            /* Bits[0:7]
                               Reserved
                               Must be coded as 0x00
                            */
  } Bits;
  UINT8 Data;
} RESERVED_BASE_CONFIGURATION_SECTION_4_STRUCT;

/*  Bytes 46~59 (0x02E~0x03B): Reserved, Base Configuration Section
Byte 51 (0x33)  Reserved, Base Configuration Section  */

#define SPD_RESERVED_BASE_CONFIGURATION_SECTION_5_REG       0x0033

typedef union {
  struct {
    UINT8 reserved_0_7 : 8;
                            /* Bits[0:7]
                               Reserved
                               Must be coded as 0x00
                            */
  } Bits;
  UINT8 Data;
} RESERVED_BASE_CONFIGURATION_SECTION_5_STRUCT;

/*  Bytes 46~59 (0x02E~0x03B): Reserved, Base Configuration Section
Byte 52 (0x34)  Reserved, Base Configuration Section  */

#define SPD_RESERVED_BASE_CONFIGURATION_SECTION_6_REG       0x0034

typedef union {
  struct {
    UINT8 reserved_0_7 : 8;
                            /* Bits[0:7]
                               Reserved
                               Must be coded as 0x00
                            */
  } Bits;
  UINT8 Data;
} RESERVED_BASE_CONFIGURATION_SECTION_6_STRUCT;

/*  Bytes 46~59 (0x02E~0x03B): Reserved, Base Configuration Section
Byte 53 (0x35)  Reserved, Base Configuration Section  */

#define SPD_RESERVED_BASE_CONFIGURATION_SECTION_7_REG       0x0035

typedef union {
  struct {
    UINT8 reserved_0_7 : 8;
                            /* Bits[0:7]
                               Reserved
                               Must be coded as 0x00
                            */
  } Bits;
  UINT8 Data;
} RESERVED_BASE_CONFIGURATION_SECTION_7_STRUCT;

/*  Bytes 46~59 (0x02E~0x03B): Reserved, Base Configuration Section
Byte 54 (0x36)  Reserved, Base Configuration Section  */

#define SPD_RESERVED_BASE_CONFIGURATION_SECTION_8_REG       0x0036

typedef union {
  struct {
    UINT8 reserved_0_7 : 8;
                            /* Bits[0:7]
                               Reserved
                               Must be coded as 0x00
                            */
  } Bits;
  UINT8 Data;
} RESERVED_BASE_CONFIGURATION_SECTION_8_STRUCT;

/*  Bytes 46~59 (0x02E~0x03B): Reserved, Base Configuration Section
Byte 55 (0x37)  Reserved, Base Configuration Section  */

#define SPD_RESERVED_BASE_CONFIGURATION_SECTION_9_REG       0x0037

typedef union {
  struct {
    UINT8 reserved_0_7 : 8;
                            /* Bits[0:7]
                               Reserved
                               Must be coded as 0x00
                            */
  } Bits;
  UINT8 Data;
} RESERVED_BASE_CONFIGURATION_SECTION_9_STRUCT;

/*  Bytes 46~59 (0x02E~0x03B): Reserved, Base Configuration Section
Byte 56 (0x38)  Reserved, Base Configuration Section  */

#define SPD_RESERVED_BASE_CONFIGURATION_SECTION_10_REG       0x0038

typedef union {
  struct {
    UINT8 reserved_0_7 : 8;
                            /* Bits[0:7]
                               Reserved
                               Must be coded as 0x00
                            */
  } Bits;
  UINT8 Data;
} RESERVED_BASE_CONFIGURATION_SECTION_10_STRUCT;

/*  Bytes 46~59 (0x02E~0x03B): Reserved, Base Configuration Section
Byte 57 (0x39)  Reserved, Base Configuration Section  */

#define SPD_RESERVED_BASE_CONFIGURATION_SECTION_11_REG       0x0039

typedef union {
  struct {
    UINT8 reserved_0_7 : 8;
                            /* Bits[0:7]
                               Reserved
                               Must be coded as 0x00
                            */
  } Bits;
  UINT8 Data;
} RESERVED_BASE_CONFIGURATION_SECTION_11_STRUCT;

/*  Bytes 46~59 (0x02E~0x03B): Reserved, Base Configuration Section
Byte 58 (0x3A)  Reserved, Base Configuration Section  */

#define SPD_RESERVED_BASE_CONFIGURATION_SECTION_12_REG       0x003A

typedef union {
  struct {
    UINT8 reserved_0_7 : 8;
                            /* Bits[0:7]
                               Reserved
                               Must be coded as 0x00
                            */
  } Bits;
  UINT8 Data;
} RESERVED_BASE_CONFIGURATION_SECTION_12_STRUCT;

/*  Bytes 46~59 (0x02E~0x03B): Reserved, Base Configuration Section
Byte 59 (0x3B)  Reserved, Base Configuration Section  */

#define SPD_RESERVED_BASE_CONFIGURATION_SECTION_13_REG       0x003B

typedef union {
  struct {
    UINT8 reserved_0_7 : 8;
                            /* Bits[0:7]
                               Reserved
                               Must be coded as 0x00
                            */
  } Bits;
  UINT8 Data;
} RESERVED_BASE_CONFIGURATION_SECTION_13_STRUCT;

/*  Byte 60 (0x3C) Connector to SDRAM Bit Mapping 0  */

#define SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_0_REG       0x003C

typedef union {
  struct {
    UINT8 bit_order_at_sdram : 5;
                            /* Bits[0:4]
                               Bit Order at SDRAM
                               See Nibble Map table
                            */
    UINT8 wired_to_upper_lower_nibble : 1;
                            /* Bits[5]
                               Wired to Upper/Lower Nibble
                               0 = lower nibble at SDRAM
                               1 == upper nibble at SDRAM
                            */
    UINT8 package_rank_map : 2;
                            /* Bits[6:7]
                               Package Rank Map
                               See Package Rank Map table
                            */
  } Bits;
  UINT8 Data;
} CONNECTOR_TO_SDRAM_BIT_MAPPING_0_STRUCT;

/*  Byte 61 (0x3D) Connector to SDRAM Bit Mapping 1  */

#define SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_1_REG       0x003D

typedef union {
  struct {
    UINT8 bit_order_at_sdram : 5;
                            /* Bits[0:4]
                               Bit Order at SDRAM
                               See Nibble Map table
                            */
    UINT8 wired_to_upper_lower_nibble : 1;
                            /* Bits[5]
                               Wired to Upper/Lower Nibble
                               0 = lower nibble at SDRAM
                               1 == upper nibble at SDRAM
                            */
    UINT8 package_rank_map : 2;
                            /* Bits[6:7]
                               Package Rank Map
                               See Package Rank Map table
                            */
  } Bits;
  UINT8 Data;
} CONNECTOR_TO_SDRAM_BIT_MAPPING_1_STRUCT;

/*  Byte 62 (0x3E) Connector to SDRAM Bit Mapping 2  */

#define SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_2_REG       0x003E

typedef union {
  struct {
    UINT8 bit_order_at_sdram : 5;
                            /* Bits[0:4]
                               Bit Order at SDRAM
                               See Nibble Map table
                            */
    UINT8 wired_to_upper_lower_nibble : 1;
                            /* Bits[5]
                               Wired to Upper/Lower Nibble
                               0 = lower nibble at SDRAM
                               1 == upper nibble at SDRAM
                            */
    UINT8 package_rank_map : 2;
                            /* Bits[6:7]
                               Package Rank Map
                               See Package Rank Map table
                            */
  } Bits;
  UINT8 Data;
} CONNECTOR_TO_SDRAM_BIT_MAPPING_2_STRUCT;

/*  Byte 63 (0x3F) Connector to SDRAM Bit Mapping 3  */

#define SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_3_REG       0x003F

typedef union {
  struct {
    UINT8 bit_order_at_sdram : 5;
                            /* Bits[0:4]
                               Bit Order at SDRAM
                               See Nibble Map table
                            */
    UINT8 wired_to_upper_lower_nibble : 1;
                            /* Bits[5]
                               Wired to Upper/Lower Nibble
                               0 = lower nibble at SDRAM
                               1 == upper nibble at SDRAM
                            */
    UINT8 package_rank_map : 2;
                            /* Bits[6:7]
                               Package Rank Map
                               See Package Rank Map table
                            */
  } Bits;
  UINT8 Data;
} CONNECTOR_TO_SDRAM_BIT_MAPPING_3_STRUCT;

/*  Byte 64 (0x40) Connector to SDRAM Bit Mapping 4  */

#define SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_4_REG       0x0040

typedef union {
  struct {
    UINT8 bit_order_at_sdram : 5;
                            /* Bits[0:4]
                               Bit Order at SDRAM
                               See Nibble Map table
                            */
    UINT8 wired_to_upper_lower_nibble : 1;
                            /* Bits[5]
                               Wired to Upper/Lower Nibble
                               0 = lower nibble at SDRAM
                               1 == upper nibble at SDRAM
                            */
    UINT8 package_rank_map : 2;
                            /* Bits[6:7]
                               Package Rank Map
                               See Package Rank Map table
                            */
  } Bits;
  UINT8 Data;
} CONNECTOR_TO_SDRAM_BIT_MAPPING_4_STRUCT;

/*  Byte 65 (0x41) Connector to SDRAM Bit Mapping 5  */

#define SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_5_REG       0x0041

typedef union {
  struct {
    UINT8 bit_order_at_sdram : 5;
                            /* Bits[0:4]
                               Bit Order at SDRAM
                               See Nibble Map table
                            */
    UINT8 wired_to_upper_lower_nibble : 1;
                            /* Bits[5]
                               Wired to Upper/Lower Nibble
                               0 = lower nibble at SDRAM
                               1 == upper nibble at SDRAM
                            */
    UINT8 package_rank_map : 2;
                            /* Bits[6:7]
                               Package Rank Map
                               See Package Rank Map table
                            */
  } Bits;
  UINT8 Data;
} CONNECTOR_TO_SDRAM_BIT_MAPPING_5_STRUCT;

/*  Byte 66 (0x42) Connector to SDRAM Bit Mapping 6  */

#define SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_6_REG       0x0042

typedef union {
  struct {
    UINT8 bit_order_at_sdram : 5;
                            /* Bits[0:4]
                               Bit Order at SDRAM
                               See Nibble Map table
                            */
    UINT8 wired_to_upper_lower_nibble : 1;
                            /* Bits[5]
                               Wired to Upper/Lower Nibble
                               0 = lower nibble at SDRAM
                               1 == upper nibble at SDRAM
                            */
    UINT8 package_rank_map : 2;
                            /* Bits[6:7]
                               Package Rank Map
                               See Package Rank Map table
                            */
  } Bits;
  UINT8 Data;
} CONNECTOR_TO_SDRAM_BIT_MAPPING_6_STRUCT;

/*  Byte 67 (0x43) Connector to SDRAM Bit Mapping 7  */

#define SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_7_REG       0x0043

typedef union {
  struct {
    UINT8 bit_order_at_sdram : 5;
                            /* Bits[0:4]
                               Bit Order at SDRAM
                               See Nibble Map table
                            */
    UINT8 wired_to_upper_lower_nibble : 1;
                            /* Bits[5]
                               Wired to Upper/Lower Nibble
                               0 = lower nibble at SDRAM
                               1 == upper nibble at SDRAM
                            */
    UINT8 package_rank_map : 2;
                            /* Bits[6:7]
                               Package Rank Map
                               See Package Rank Map table
                            */
  } Bits;
  UINT8 Data;
} CONNECTOR_TO_SDRAM_BIT_MAPPING_7_STRUCT;

/*  Byte 68 (0x44) Connector to SDRAM Bit Mapping 8  */

#define SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_8_REG       0x0044

typedef union {
  struct {
    UINT8 bit_order_at_sdram : 5;
                            /* Bits[0:4]
                               Bit Order at SDRAM
                               See Nibble Map table
                            */
    UINT8 wired_to_upper_lower_nibble : 1;
                            /* Bits[5]
                               Wired to Upper/Lower Nibble
                               0 = lower nibble at SDRAM
                               1 == upper nibble at SDRAM
                            */
    UINT8 package_rank_map : 2;
                            /* Bits[6:7]
                               Package Rank Map
                               See Package Rank Map table
                            */
  } Bits;
  UINT8 Data;
} CONNECTOR_TO_SDRAM_BIT_MAPPING_8_STRUCT;

/*  Byte 69 (0x45) Connector to SDRAM Bit Mapping 9  */

#define SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_9_REG       0x0045

typedef union {
  struct {
    UINT8 bit_order_at_sdram : 5;
                            /* Bits[0:4]
                               Bit Order at SDRAM
                               See Nibble Map table
                            */
    UINT8 wired_to_upper_lower_nibble : 1;
                            /* Bits[5]
                               Wired to Upper/Lower Nibble
                               0 = lower nibble at SDRAM
                               1 == upper nibble at SDRAM
                            */
    UINT8 package_rank_map : 2;
                            /* Bits[6:7]
                               Package Rank Map
                               See Package Rank Map table
                            */
  } Bits;
  UINT8 Data;
} CONNECTOR_TO_SDRAM_BIT_MAPPING_9_STRUCT;

/*  Byte 70 (0x46) Connector to SDRAM Bit Mapping 10  */

#define SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_10_REG       0x0046

typedef union {
  struct {
    UINT8 bit_order_at_sdram : 5;
                            /* Bits[0:4]
                               Bit Order at SDRAM
                               See Nibble Map table
                            */
    UINT8 wired_to_upper_lower_nibble : 1;
                            /* Bits[5]
                               Wired to Upper/Lower Nibble
                               0 = lower nibble at SDRAM
                               1 == upper nibble at SDRAM
                            */
    UINT8 package_rank_map : 2;
                            /* Bits[6:7]
                               Package Rank Map
                               See Package Rank Map table
                            */
  } Bits;
  UINT8 Data;
} CONNECTOR_TO_SDRAM_BIT_MAPPING_10_STRUCT;

/*  Byte 71 (0x47) Connector to SDRAM Bit Mapping 11  */

#define SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_11_REG       0x0047

typedef union {
  struct {
    UINT8 bit_order_at_sdram : 5;
                            /* Bits[0:4]
                               Bit Order at SDRAM
                               See Nibble Map table
                            */
    UINT8 wired_to_upper_lower_nibble : 1;
                            /* Bits[5]
                               Wired to Upper/Lower Nibble
                               0 = lower nibble at SDRAM
                               1 == upper nibble at SDRAM
                            */
    UINT8 package_rank_map : 2;
                            /* Bits[6:7]
                               Package Rank Map
                               See Package Rank Map table
                            */
  } Bits;
  UINT8 Data;
} CONNECTOR_TO_SDRAM_BIT_MAPPING_11_STRUCT;

/*  Byte 72 (0x48) Connector to SDRAM Bit Mapping 12  */

#define SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_12_REG       0x0048

typedef union {
  struct {
    UINT8 bit_order_at_sdram : 5;
                            /* Bits[0:4]
                               Bit Order at SDRAM
                               See Nibble Map table
                            */
    UINT8 wired_to_upper_lower_nibble : 1;
                            /* Bits[5]
                               Wired to Upper/Lower Nibble
                               0 = lower nibble at SDRAM
                               1 == upper nibble at SDRAM
                            */
    UINT8 package_rank_map : 2;
                            /* Bits[6:7]
                               Package Rank Map
                               See Package Rank Map table
                            */
  } Bits;
  UINT8 Data;
} CONNECTOR_TO_SDRAM_BIT_MAPPING_12_STRUCT;

/*  Byte 73 (0x49) Connector to SDRAM Bit Mapping 13  */

#define SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_13_REG       0x0049

typedef union {
  struct {
    UINT8 bit_order_at_sdram : 5;
                            /* Bits[0:4]
                               Bit Order at SDRAM
                               See Nibble Map table
                            */
    UINT8 wired_to_upper_lower_nibble : 1;
                            /* Bits[5]
                               Wired to Upper/Lower Nibble
                               0 = lower nibble at SDRAM
                               1 == upper nibble at SDRAM
                            */
    UINT8 package_rank_map : 2;
                            /* Bits[6:7]
                               Package Rank Map
                               See Package Rank Map table
                            */
  } Bits;
  UINT8 Data;
} CONNECTOR_TO_SDRAM_BIT_MAPPING_13_STRUCT;

/*  Byte 74 (0x4A) Connector to SDRAM Bit Mapping 14  */

#define SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_14_REG       0x004A

typedef union {
  struct {
    UINT8 bit_order_at_sdram : 5;
                            /* Bits[0:4]
                               Bit Order at SDRAM
                               See Nibble Map table
                            */
    UINT8 wired_to_upper_lower_nibble : 1;
                            /* Bits[5]
                               Wired to Upper/Lower Nibble
                               0 = lower nibble at SDRAM
                               1 == upper nibble at SDRAM
                            */
    UINT8 package_rank_map : 2;
                            /* Bits[6:7]
                               Package Rank Map
                               See Package Rank Map table
                            */
  } Bits;
  UINT8 Data;
} CONNECTOR_TO_SDRAM_BIT_MAPPING_14_STRUCT;

/*  Byte 75 (0x4B) Connector to SDRAM Bit Mapping 15  */

#define SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_15_REG       0x004B

typedef union {
  struct {
    UINT8 bit_order_at_sdram : 5;
                            /* Bits[0:4]
                               Bit Order at SDRAM
                               See Nibble Map table
                            */
    UINT8 wired_to_upper_lower_nibble : 1;
                            /* Bits[5]
                               Wired to Upper/Lower Nibble
                               0 = lower nibble at SDRAM
                               1 == upper nibble at SDRAM
                            */
    UINT8 package_rank_map : 2;
                            /* Bits[6:7]
                               Package Rank Map
                               See Package Rank Map table
                            */
  } Bits;
  UINT8 Data;
} CONNECTOR_TO_SDRAM_BIT_MAPPING_15_STRUCT;

/*  Byte 76 (0x4C) Connector to SDRAM Bit Mapping 16  */

#define SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_16_REG       0x004C

typedef union {
  struct {
    UINT8 bit_order_at_sdram : 5;
                            /* Bits[0:4]
                               Bit Order at SDRAM
                               See Nibble Map table
                            */
    UINT8 wired_to_upper_lower_nibble : 1;
                            /* Bits[5]
                               Wired to Upper/Lower Nibble
                               0 = lower nibble at SDRAM
                               1 == upper nibble at SDRAM
                            */
    UINT8 package_rank_map : 2;
                            /* Bits[6:7]
                               Package Rank Map
                               See Package Rank Map table
                            */
  } Bits;
  UINT8 Data;
} CONNECTOR_TO_SDRAM_BIT_MAPPING_16_STRUCT;

/*  Byte 77 (0x4D) Connector to SDRAM Bit Mapping 17  */

#define SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_17_REG       0x004D

typedef union {
  struct {
    UINT8 bit_order_at_sdram : 5;
                            /* Bits[0:4]
                               Bit Order at SDRAM
                               See Nibble Map table
                            */
    UINT8 wired_to_upper_lower_nibble : 1;
                            /* Bits[5]
                               Wired to Upper/Lower Nibble
                               0 = lower nibble at SDRAM
                               1 == upper nibble at SDRAM
                            */
    UINT8 package_rank_map : 2;
                            /* Bits[6:7]
                               Package Rank Map
                               See Package Rank Map table
                            */
  } Bits;
  UINT8 Data;
} CONNECTOR_TO_SDRAM_BIT_MAPPING_17_STRUCT;

/*  Byte 117 (0x075): Fine Offset for Minimum CAS to CAS Delay Time(tCCDLmin), same bank group  */

#define SPD_FINE_OFFSET_FOR_MIN_CAS_TO_CAS_DELAY_TCCDL_SAME_BANKS_REG       0x0075

/*  Byte 118 (0x076): Fine Offset for Minimum Activate to Activate Delay Time(tRRDLmin), same bank group  */

#define SPD_FINE_OFFSET_FOR_MIN_ACTIVATE_TO_ACTIVATE_DELAY_TRRDL_SAME_BANKS_REG       0x0076

/*  Byte 119 (0x077): Fine Offset for Minimum Activate to Activate Delay Time (tRRDSmin), different bank group  */

#define SPD_FINE_OFFSET_FOR_MIN_ACTIVATE_TO_ACTIVATE_DELAY_TRRDS_DIFF_BANKS_REG       0x0077

/*  Byte 120 (0x078): Fine Offset for Minimum Active to Active/Refresh Delay Time (tRCmin)  */

#define SPD_FINE_OFFSET_FOR_MIN_ACTIVE_TO_ACTIVE_REFRESH_DELAY_TRC_REG       0x0078

/*  Byte 121 (0x079): Fine Offset for Minimum Row Precharge Delay Time (tRPmin)  */

#define SPD_FINE_OFFSET_FOR_MIN_ROW_PRECHARGE_DELAY_TRP_REG       0x0079

/*  Byte 122 (0x07A): Fine Offset for Minimum RAS to CAS Delay Time (tRCDmin)  */

#define SPD_FINE_OFFSET_FOR_MIN_RAS_TO_CAS_DELAY_TIME_TRCDMIN_REG       0x007A

/*  Byte 123 (0x07B): Fine Offset for Minimum CAS Latency Time (tAAmin)  */

#define SPD_FINE_OFFSET_FOR_MINIMUM_CAS_LATENCY_TIME_TAAMIN_REG       0x007B

/*  Byte 124 (0x07C): Fine Offset for SDRAM Maximum Cycle Time (tCKAVGmax)  */

#define SPD_FINE_OFFSET_FOR_SDRAM_MAXIMUM_CYCLE_TIME_TCKAVGMAX_REG       0x007C

/*  Byte 125 (0x07D): Fine Offset for SDRAM Minimum Cycle Time (tCKAVGmin)  */

#define SPD_FINE_OFFSET_FOR_SDRAM_MINIMUM_CYCLE_TIME_TCKAVGMIN_REG       0x007D

/*  Byte 126 (0x07E): Cyclical Redundancy Code (CRC) for Base Configuration Section, LSB  */

#define SPD_CRC_FOR_BASE_CONFIGURATION_SECTION_LSB_REG       0x007E

/*  Byte 127 (0x07F): Cyclical Redundancy Code (CRC) for Base ConfigurationSection, MSB  */

#define SPD_CRC_FOR_BASE_CONFIGURATIONSECTION_MSB_REG       0x007F

/*  Byte 128 (0x080) (Unbuffered): Raw Card Extension, Module Nominal Height  */

#define SPD_UDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_REG       0x0080

typedef union {
  struct {
    UINT8 module_nominal_height_max : 5;
                            /* Bits[0:4]
                               Module Nominal Height max, in mm (baseline height = 15 mm)
                               00000 = height <= 15 mm
                               00001 = 15 < height <= 16 mm
                               00010 = 16 < height <= 17 mm
                               00011 = 17 < height <= 18 mm
                               00100 = 18 < height <= 19 mm
                               ...
                               01010 = 24 < height <= 25 mm
                               01011 = 25 < height <= 26 mm
                               ...
                               01111 = 29 < height <= 30 mm
                               10000 = 30 < height <= 31 mm
                               10001 = 31 < height <= 32 mm
                               ...
                               11111 = 45 mm < height
                            */
    UINT8 raw_card_extension : 3;
                            /* Bits[5:7]
                               Raw Card Extension
                               000 = raw card revisions 0 to 3
                               see byte 130
                               001 = raw card revision 4
                               010 = raw card revision 5
                               011 = raw card revision 6
                               100 = raw card revision 7
                               101 = raw card revision 8
                               110 = raw card revision 9
                               111 = raw card revision 10
                            */
  } Bits;
  UINT8 Data;
} UDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_STRUCT;

/*  Byte 128 (0x080) (Registered): Raw Card Extension, Module Nominal Height  */

#define SPD_RDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_REG       0x0080

typedef union {
  struct {
    UINT8 module_nominal_height_max : 5;
                            /* Bits[0:4]
                               Module Nominal Height max, in mm (baseline height = 15 mm)
                               00000 = height <= 15 mm
                               00001 = 15 < height <= 16 mm
                               00010 = 16 < height <= 17 mm
                               00011 = 17 < height <= 18 mm
                               00100 = 18 < height <= 19 mm
                               ...
                               01010 = 24 < height <= 25 mm
                               01011 = 25 < height <= 26 mm
                               ...
                               01111 = 29 < height <= 30 mm
                               10000 = 30 < height <= 31 mm
                               10001 = 31 < height <= 32 mm
                               ...
                               11111 = 45 mm < height
                            */
    UINT8 raw_card_extension : 3;
                            /* Bits[5:7]
                               Raw Card Extension
                               000 = raw card revisions 0 to 3
                               see byte 130
                               001 = raw card revision 4
                               010 = raw card revision 5
                               011 = raw card revision 6
                               100 = raw card revision 7
                               101 = raw card revision 8
                               110 = raw card revision 9
                               111 = raw card revision 10
                            */
  } Bits;
  UINT8 Data;
} RDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_STRUCT;

/*  Byte 128 (0x080) (Load Reduced): Raw Card Extension, Module Nominal Height  */

#define SPD_LRDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_REG       0x0080

typedef union {
  struct {
    UINT8 module_nominal_height_max : 5;
                            /* Bits[0:4]
                               Module Nominal Height max, in mm (baseline height = 15 mm)
                               00000 = height <= 15 mm
                               00001 = 15 < height <= 16 mm
                               00010 = 16 < height <= 17 mm
                               00011 = 17 < height <= 18 mm
                               00100 = 18 < height <= 19 mm
                               ...
                               01010 = 24 < height <= 25 mm
                               01011 = 25 < height <= 26 mm
                               ...
                               01111 = 29 < height <= 30 mm
                               10000 = 30 < height <= 31 mm
                               ...
                               11111 = 45 mm < height
                            */
    UINT8 raw_card_extension : 3;
                            /* Bits[5:7]
                               Raw Card Extension
                               000 = raw card revisions 0 to 3
                               see byte 130
                               001 = raw card revision 4
                               010 = raw card revision 5
                               011 = raw card revision 6
                               100 = raw card revision 7
                               101 = raw card revision 8
                               110 = raw card revision 9
                               111 = raw card revision 10
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_STRUCT;

/*  Byte 129 (0x081) (Unbuffered): Module Maximum Thickness  */

#define SPD_UDIMM_MODULE_MAXIMUM_THICKNESS_REG       0x0081

typedef union {
  struct {
    UINT8 module_maximum_thickness_max_front : 4;
                            /* Bits[0:3]
                               Module Maximum Thickness max, Front, in mm (baseline thickness = 1 mm)
                               0000 = thickness <= 1 mm
                               0001 = 1 < thickness <= 2 mm
                               0010 = 2 < thickness <= 3 mm
                               0011 = 3 < thickness <= 4 mm
                               ...
                               1110 = 14 < thickness <= 15 mm
                               1111 = 15 < thickness
                            */
    UINT8 module_maximum_thickness_max_back : 4;
                            /* Bits[4:7]
                               Module Maximum Thickness max, Back, in mm (baseline thickness = 1 mm)
                               0000 = thickness <= 1 mm
                               0001 = 1 < thickness <= 2 mm
                               0010 = 2 < thickness <= 3 mm
                               0011 = 3 < thickness <= 4 mm
                               ...
                               1110 = 14 < thickness <= 15 mm
                               1111 = 15 < thickness
                            */
  } Bits;
  UINT8 Data;
} UDIMM_MODULE_MAXIMUM_THICKNESS_STRUCT;

/*  Byte 129 (0x081) (Registered): Module Maximum Thickness  */

#define SPD_RDIMM_MODULE_MAXIMUM_THICKNESS_REG       0x0081

typedef union {
  struct {
    UINT8 module_maximum_thickness_max_front : 4;
                            /* Bits[0:3]
                               Module Maximum Thickness max, Front, in mm (baseline thickness = 1 mm)
                               0000 = thickness <= 1 mm
                               0001 = 1 < thickness <= 2 mm
                               0010 = 2 < thickness <= 3 mm
                               0011 = 3 < thickness <= 4 mm
                               ...
                               1110 = 14 < thickness <= 15 mm
                               1111 = 15 < thickness
                            */
    UINT8 module_maximum_thickness_max_back : 4;
                            /* Bits[4:7]
                               Module Maximum Thickness max, Back, in mm (baseline thickness = 1 mm)
                               0000 = thickness <= 1 mm
                               0001 = 1 < thickness <= 2 mm
                               0010 = 2 < thickness <= 3 mm
                               0011 = 3 < thickness <= 4 mm
                               ...
                               1110 = 14 < thickness <= 15 mm
                               1111 = 15 < thickness
                            */
  } Bits;
  UINT8 Data;
} RDIMM_MODULE_MAXIMUM_THICKNESS_STRUCT;

/*  Byte 129 (0x081) (Load Reduced): Module Maximum Thickness  */

#define SPD_LRDIMM_MODULE_MAXIMUM_THICKNESS_REG       0x0081

typedef union {
  struct {
    UINT8 module_maximum_thickness_max_front : 4;
                            /* Bits[0:3]
                               Module Maximum Thickness max, Front, in mm (baseline thickness = 1 mm)
                               0000 = thickness <= 1 mm
                               0001 = 1 < thickness <= 2 mm
                               0010 = 2 < thickness <= 3 mm
                               0011 = 3 < thickness <= 4 mm
                               ...
                               1110 = 14 < thickness <= 15 mm
                               1111 = 15 < thickness
                            */
    UINT8 module_maximum_thickness_max_back : 4;
                            /* Bits[4:7]
                               Module Maximum Thickness max, Back, in mm (baseline thickness = 1 mm)
                               0000 = thickness <= 1 mm
                               0001 = 1 < thickness <= 2 mm
                               0010 = 2 < thickness <= 3 mm
                               0011 = 3 < thickness <= 4 mm
                               ...
                               1110 = 14 < thickness <= 15 mm
                               1111 = 15 < thickness
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_MODULE_MAXIMUM_THICKNESS_STRUCT;

/*  Byte 130 (0x082) (Unbuffered): Reference Raw Card Used  */

#define SPD_UDIMM_REFERENCE_RAW_CARD_USED_REG       0x0082

typedef union {
  struct {
    UINT8 reference_raw_card : 5;
                            /* Bits[0:4]
                               Reference Raw Card
                               When bit 7 = 0,
                               00000 = Reference raw card A
                               00001 = Reference raw card B
                               00010 = Reference raw card C
                               00011 = Reference raw card D
                               00100 = Reference raw card E
                               00101 = Reference raw card F
                               00110 = Reference raw card G
                               00111 = Reference raw card H
                               01000 = Reference raw card J
                               01001 = Reference raw card K
                               01010 = Reference raw card L
                               01011 = Reference raw card M
                               01100 = Reference raw card N
                               01101 = Reference raw card P
                               01110 = Reference raw card R
                               01111 = Reference raw card T
                               10000 = Reference raw card U
                               10001 = Reference raw card V
                               10010 = Reference raw card W
                               10011 = Reference raw card Y
                               10100 = Reference raw card AA
                               10101 = Reference raw card AB
                               10110 = Reference raw card AC
                               10111 = Reference raw card AD
                               11000 = Reference raw card AE
                               11001 = Reference raw card AF
                               11010 = Reference raw card AG
                               11011 = Reference raw card AH
                               11100 = Reference raw card AJ
                               11101 = Reference raw card AK
                               11110 = Reference raw card AL
                               11111 = ZZ (no JEDEC reference raw card design used)
                            */
    UINT8 reference_raw_card_revision : 2;
                            /* Bits[5:6]
                               Reference Raw Card Revision
                               00 = revision 0
                               01 = revision 1
                               10 = revision 2
                               11 = revision 3
                               See byte 128 for extensions beyond revision 3
                               these bits must be coded as 11 for all revisions greater than 3
                            */
    UINT8 reference_raw_card_extension : 1;
                            /* Bits[7]
                               Reference Raw Card Extension
                               0 = Reference raw cards A through AL
                               1 = Reference raw cards AM throughCB
                            */
  } Bits;
  UINT8 Data;
} UDIMM_REFERENCE_RAW_CARD_USED_STRUCT;

/*  Byte 130 (0x082) (Registered): Reference Raw Card Used  */

#define SPD_RDIMM_REFERENCE_RAW_CARD_USED_REG       0x0082

typedef union {
  struct {
    UINT8 reference_raw_card : 5;
                            /* Bits[0:4]
                               Reference Raw Card
                               When bit 7 = 0,
                               00000 = Reference raw card A
                               00001 = Reference raw card B
                               00010 = Reference raw card C
                               00011 = Reference raw card D
                               00100 = Reference raw card E
                               00101 = Reference raw card F
                               00110 = Reference raw card G
                               00111 = Reference raw card H
                               01000 = Reference raw card J
                               01001 = Reference raw card K
                               01010 = Reference raw card L
                               01011 = Reference raw card M
                               01100 = Reference raw card N
                               01101 = Reference raw card P
                               01110 = Reference raw card R
                               01111 = Reference raw card T
                               10000 = Reference raw card U
                               10001 = Reference raw card V
                               10010 = Reference raw card W
                               10011 = Reference raw card Y
                               10100 = Reference raw card AA
                               10101 = Reference raw card AB
                               10110 = Reference raw card AC
                               10111 = Reference raw card AD
                               11000 = Reference raw card AE
                               11001 = Reference raw card AF
                               11010 = Reference raw card AG
                               11011 = Reference raw card AH
                               11100 = Reference raw card AJ
                               11101 = Reference raw card AK
                               11110 = Reference raw card AL
                               11111 = ZZ (no JEDEC reference raw card design used)
                            */
    UINT8 reference_raw_card_revision : 2;
                            /* Bits[5:6]
                               Reference Raw Card Revision
                               00 = revision 0
                               01 = revision 1
                               10 = revision 2
                               11 = revision 3
                               See byte 128 for extensions beyond revision 3
                               these bits must be coded as 11 for all revisions greater than 3
                            */
    UINT8 reference_raw_card_extension : 1;
                            /* Bits[7]
                               Reference Raw Card Extension
                               0 = Reference raw cards A through AL
                               1 = Reference raw cards AM throughCB
                            */
  } Bits;
  UINT8 Data;
} RDIMM_REFERENCE_RAW_CARD_USED_STRUCT;

/*  Byte 130 (0x082) (Load Reduced): Reference Raw Card Used  */

#define SPD_LRDIMM_REFERENCE_RAW_CARD_USED_REG       0x0082

typedef union {
  struct {
    UINT8 reference_raw_card : 5;
                            /* Bits[0:4]
                               Reference Raw Card
                               When bit 7 = 0,
                               00000 = Reference raw card A
                               00001 = Reference raw card B
                               00010 = Reference raw card C
                               00011 = Reference raw card D
                               00100 = Reference raw card E
                               00101 = Reference raw card F
                               00110 = Reference raw card G
                               00111 = Reference raw card H
                               01000 = Reference raw card J
                               01001 = Reference raw card K
                               01010 = Reference raw card L
                               01011 = Reference raw card M
                               01100 = Reference raw card N
                               01101 = Reference raw card P
                               01110 = Reference raw card R
                               01111 = Reference raw card T
                               10000 = Reference raw card U
                               10001 = Reference raw card V
                               10010 = Reference raw card W
                               10011 = Reference raw card Y
                               10100 = Reference raw card AA
                               10101 = Reference raw card AB
                               10110 = Reference raw card AC
                               10111 = Reference raw card AD
                               11000 = Reference raw card AE
                               11001 = Reference raw card AF
                               11010 = Reference raw card AG
                               11011 = Reference raw card AH
                               11100 = Reference raw card AJ
                               11101 = Reference raw card AK
                               11110 = Reference raw card AL
                               11111 = ZZ (no JEDEC reference raw card design used)
                            */
    UINT8 reference_raw_card_revision : 2;
                            /* Bits[5:6]
                               Reference Raw Card Revision
                               00 = revision 0
                               01 = revision 1
                               10 = revision 2
                               11 = revision 3
                               See byte 128 for extensions beyond revision 3
                               these bits must be coded as 11 for all revisions greater than 3
                            */
    UINT8 reference_raw_card_extension : 1;
                            /* Bits[7]
                               Reference Raw Card Extension
                               0 = Reference raw cards A through AL
                               1 = Reference raw cards AM throughCB
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_REFERENCE_RAW_CARD_USED_STRUCT;

/*  Byte 131 (0x083) (Unbuffered): Address Mapping from Edge Connector to DRAM  */

#define SPD_UDIMM_ADDRESS_MAPPING_FROM_EDGE_CONNECTOR_TO_DRAM_REG       0x0083

typedef union {
  struct {
    UINT8 rank_1_mapping : 1;
                            /* Bits[0]
                               Rank 1 Mapping
                               0 = standard
                               1 = mirrored
                            */
    UINT8 reserved_1_7 : 7;
                            /* Bits[1:7]
                               Reserved
                               Reserved
                               must bo coded as 0000000
                            */
  } Bits;
  UINT8 Data;
} UDIMM_ADDRESS_MAPPING_FROM_EDGE_CONNECTOR_TO_DRAM_STRUCT;

/*  Byte 131 (0x083) (Registered): DIMM Attributes  */

#define SPD_RDIMM_DIMM_ATTRIBUTES_REG       0x0083

typedef union {
  struct {
    UINT8 of_registers_used_on_rdimm : 2;
                            /* Bits[0:1]
                               # of Registers used on RDIMM
                               00 = Undefined
                               01 = 1 register
                               10 = 2 registers
                               11 = 4 registers
                            */
    UINT8 of_rows_of_drams_on_rdimm : 2;
                            /* Bits[2:3]
                               # of rows of DRAMs on RDIMM
                               00 = Undefined
                               01 = 1 row
                               10 = 2 rows
                               11 = 4 rows
                            */
    UINT8 register_type : 4;
                            /* Bits[4:7]
                               Register Type
                               0000 = DDR4RCD01
                               0001 = DDR4RCD02
                               All other values reserved
                            */
  } Bits;
  UINT8 Data;
} RDIMM_DIMM_ATTRIBUTES_STRUCT;

/*  Byte 131 (0x083) (Load Reduced): DIMM Attributes  */

#define SPD_LRDIMM_DIMM_ATTRIBUTES_REG       0x0083

typedef union {
  struct {
    UINT8 of_registers_used_on_lrdimm : 2;
                            /* Bits[0:1]
                               # of Registers used on LRDIMM
                               00 = Undefined
                               01 = 1 register
                               10 = Reserved
                               11 = Reserved
                            */
    UINT8 of_rows_of_drams_on_lrdimm : 2;
                            /* Bits[2:3]
                               # of rows of DRAMs on LRDIMM
                               00 = Undefined
                               01 = 1 row
                               10 = 2 rows
                               11 = Reserved
                            */
    UINT8 register_and_data_buffer_types : 4;
                            /* Bits[4:7]
                               Register and Data Buffer Types
                               0000 = DDR4RCD01 and DDR4DB01
                               0001 = DDR4RCD02 and DDR4DB02
                               All other codes reserved
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_DIMM_ATTRIBUTES_STRUCT;

/*  Byte 132 (0x084) (Registered): RDIMM Thermal Heat Spreader Solution  */

#define SPD_RDIMM_RDIMM_THERMAL_HEAT_SPREADER_SOLUTION_REG       0x0084

typedef union {
  struct {
    UINT8 heat_spreader_thermal_characteristics : 7;
                            /* Bits[0:6]
                               Heat Spreader Thermal Characteristics
                               0 = Undefined
                               All other settings to be defined
                            */
    UINT8 heat_spreader_solution : 1;
                            /* Bits[7]
                               Heat Spreader Solution
                               0 = Heat spreader solution is not incorporated onto this assembly
                               1 = Heat spreader solution is incorporated onto this assembly
                            */
  } Bits;
  UINT8 Data;
} RDIMM_RDIMM_THERMAL_HEAT_SPREADER_SOLUTION_STRUCT;

/*  Byte 132 (0x084) (Load Reduced): LRDIMM Thermal Heat Spreader Solution  */

#define SPD_LRDIMM_LRDIMM_THERMAL_HEAT_SPREADER_SOLUTION_REG       0x0084

typedef union {
  struct {
    UINT8 heat_spreader_thermal_characteristics : 7;
                            /* Bits[0:6]
                               Heat Spreader Thermal Characteristics
                               Undefined
                               must be coded as 0000000
                            */
    UINT8 heat_spreader : 1;
                            /* Bits[7]
                               Heat Spreader
                               0 = Heat spreader solution is not incorporated onto this assembly
                               1 = Heat spreader solution is incorporated onto this assembly
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_LRDIMM_THERMAL_HEAT_SPREADER_SOLUTION_STRUCT;

/*  Byte 133 (0x085) (Registered): Register Manufacturer ID Code, LSB  */

#define SPD_RDIMM_REGISTER_MANUFACTURER_ID_CODE_LSB_REG       0x0085

typedef union {
  struct {
    UINT8 number_of_continuation_codes_register_manufacturer : 7;
                            /* Bits[0:6]
                               Number of continuation codes, Register Manufacturer
                               See JEP-106
                            */
    UINT8 odd_parity_for_byte_133_bits_6_0 : 1;
                            /* Bits[7]
                               Odd parity for Byte 133, bits 6~0
                            */
  } Bits;
  UINT8 Data;
} RDIMM_REGISTER_MANUFACTURER_ID_CODE_LSB_STRUCT;

/*  Byte 133 (0x085) (Load Reduced): Register and Data Buffer Manufacturer ID Code, LSB  */

#define SPD_LRDIMM_REGISTER_AND_DB_MANUFACTURER_ID_CODE_LSB_REG       0x0085

typedef union {
  struct {
    UINT8 continuation_codes_num_memory_buffer_manufacturer : 7;
                            /* Bits[0:6]
                               Number of continuation codes, Memory Buffer Manufacturer
                               See JEP-106
                            */
    UINT8 odd_parity_for_byte_133_bits_6_0 : 1;
                            /* Bits[7]
                               Odd parity for Byte 133, bits 6 ~ 0
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_REGISTER_AND_DB_MANUFACTURER_ID_CODE_LSB_STRUCT;

/*  Byte 134 (0x086) (Registered): Register Manufacturer ID Code, MSB  */

#define SPD_RDIMM_REGISTER_MANUFACTURER_ID_CODE_MSB_REG       0x0086

typedef union {
  struct {
    UINT8 last_non_zero_byte_register_manufacturer : 8;
                            /* Bits[0:7]
                               Last non-zero byte, Register Manufacturer
                               See JEP-106
                            */
  } Bits;
  UINT8 Data;
} RDIMM_REGISTER_MANUFACTURER_ID_CODE_MSB_STRUCT;

/*  Byte 134 (0x086) (Load Reduced): Register and Data Buffer Manufacturer ID Code, MSB  */

#define SPD_LRDIMM_REGISTER_AND_DB_MANUFACTURER_ID_CODE_MSB_REG       0x0086

typedef union {
  struct {
    UINT8 last_non_zero_byte_memory_buffer_manufacturer : 8;
                            /* Bits[0:7]
                               Last non-zero byte, Memory Buffer Manufacturer
                               See JEP-106
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_REGISTER_AND_DB_MANUFACTURER_ID_CODE_MSB_STRUCT;

/*  Byte 135 (0x087) (Registered): Register Revision Number  */

#define SPD_RDIMM_REGISTER_REVISION_NUMBER_REG       0x0087

typedef union {
  struct {
    UINT8 register_revision_number : 8;
                            /* Bits[0:7]
                               Register Revision Number
                               Programmed in straight Hex format - no conversion needed. 00 - Valid
                               01 - Valid
                               ..
                               FE - Valid
                               FF - Undefined (No Revision Number Provided)
                            */
  } Bits;
  UINT8 Data;
} RDIMM_REGISTER_REVISION_NUMBER_STRUCT;

/*  Byte 135 (0x087) (Load Reduced): Register Revision Number  */

#define SPD_LRDIMM_REGISTER_REVISION_NUMBER_REG       0x0087

typedef union {
  struct {
    UINT8 register_revision_number : 8;
                            /* Bits[0:7]
                               Register Revision Number
                               Programmed in straight Hex format - no conversion needed. 00 - Valid
                               01 - Valid
                               ..
                               FE - Valid
                               FF - Undefined
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_REGISTER_REVISION_NUMBER_STRUCT;

/*  Byte 136 (0x088) (Registered): Address Mapping from Register to DRAM  */

#define SPD_RDIMM_ADDRESS_MAPPING_FROM_REGISTER_TO_DRAM_REG       0x0088

typedef union {
  struct {
    UINT8 rank_1_mapping : 1;
                            /* Bits[0]
                               Rank 1 Mapping
                               0 = standard
                               1 = mirrored
                            */
    UINT8 reserved_1_7 : 7;
                            /* Bits[1:7]
                               Reserved
                               Reserved
                               must be coded as 0000000
                            */
  } Bits;
  UINT8 Data;
} RDIMM_ADDRESS_MAPPING_FROM_REGISTER_TO_DRAM_STRUCT;

/*  Byte 136 (0x088) (Load Reduced): Address Mapping from Register to DRAM  */

#define SPD_LRDIMM_ADDRESS_MAPPING_FROM_REGISTER_TO_DRAM_REG       0x0088

typedef union {
  struct {
    UINT8 odd_rank_mapping : 1;
                            /* Bits[0]
                               Odd Rank Mapping
                               0 = standard
                               1 = mirrored
                            */
    UINT8 reserved_1_7 : 7;
                            /* Bits[1:7]
                               Reserved
                               Reserved
                               must be coded as 0000000
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_ADDRESS_MAPPING_FROM_REGISTER_TO_DRAM_STRUCT;

/*  Byte 137 (0x089) (Registered): Register Output Drive Strength for Control and Command/Address  */

#define SPD_RDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CTRL_AND_CMD_ADDR_REG       0x0089

typedef union {
  struct {
    UINT8 cke : 2;
                            /* Bits[0:1]
                               CKE
                               00 = Light Drive
                               01 = Moderate Drive
                               10 = Strong Drive
                               11 = Very Strong Drive
                            */
    UINT8 odt : 2;
                            /* Bits[2:3]
                               ODT
                               00 = Light Drive
                               01 = Moderate Drive
                               10 = Strong Drive
                               11 = Very Strong Drive
                            */
    UINT8 command_address : 2;
                            /* Bits[4:5]
                               Command/Address
                               00 = Light Drive
                               01 = Moderate Drive
                               10 = Strong Drive
                               11 = Very Strong Drive
                            */
    UINT8 chip_select : 2;
                            /* Bits[6:7]
                               Chip Select
                               00 = Light Drive
                               01 = Moderate Drive
                               10 = Strong Drive
                               11 = Very Strong Drive
                            */
  } Bits;
  UINT8 Data;
} RDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CTRL_AND_CMD_ADDR_STRUCT;

/*  Byte 137 (0x089) (Load Reduced): Register Output Drive Strength for Control and Command/Address  */

#define SPD_LRDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CTRL_AND_CMD_ADDR_REG       0x0089

typedef union {
  struct {
    UINT8 cke : 2;
                            /* Bits[0:1]
                               CKE
                               00 = Light Drive
                               01 = Moderate Drive
                               10 = Strong Drive
                               11 = Very Strong Drive
                            */
    UINT8 odt : 2;
                            /* Bits[2:3]
                               ODT
                               00 = Light Drive
                               01 = Moderate Drive
                               10 = Strong Drive
                               11 = Very Strong Drive
                            */
    UINT8 command_address : 2;
                            /* Bits[4:5]
                               Command/Address
                               00 = Light Drive
                               01 = Moderate Drive
                               10 = Strong Drive
                               11 = Very Strong Drive
                            */
    UINT8 chip_select : 2;
                            /* Bits[6:7]
                               Chip Select
                               00 = Light Drive
                               01 = Moderate Drive
                               10 = Strong Drive
                               11 = Very Strong Drive
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CTRL_AND_CMD_ADDR_STRUCT;

/*  Byte 138 (0x08A) (Registered): Register Output Drive Strength for Clock  */

#define SPD_RDIMM_REGISTER_OUTPUT_DRIVE_STRENGTH_FOR_CLOCK_REG       0x008A

typedef union {
  struct {
    UINT8 y0_y2 : 2;
                            /* Bits[0:1]
                               Y0, Y2
                               00 = Light Drive
                               01 = Moderate Drive
                               10 = Strong Drive
                               11 = Very Strong Drive
                            */
    UINT8 y1_y3 : 2;
                            /* Bits[2:3]
                               Y1, Y3
                               00 = Light Drive
                               01 = Moderate Drive
                               10 = Strong Drive
                               11 = Very Strong Drive
                            */
    UINT8 reserved_4_7 : 4;
                            /* Bits[4:7]
                               Reserved
                               Reserved
                               must be coded as 0000
                            */
  } Bits;
  UINT8 Data;
} RDIMM_REGISTER_OUTPUT_DRIVE_STRENGTH_FOR_CLOCK_STRUCT;

/*  Byte 138 (0x08A) (Load Reduced): Register Output Drive Strength for Clock and Data Buffer Control  */

#define SPD_LRDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CLOCK_AND_DB_CTRL_REG       0x008A

typedef union {
  struct {
    UINT8 y0_y2 : 2;
                            /* Bits[0:1]
                               Y0, Y2
                               00 = Light Drive
                               01 = Moderate Drive
                               10 = Strong Drive
                               11 = Very Strong Drive
                            */
    UINT8 y1_y3 : 2;
                            /* Bits[2:3]
                               Y1, Y3
                               00 = Light Drive
                               01 = Moderate Drive
                               10 = Strong Drive
                               11 = Very Strong Drive
                            */
    UINT8 bcom_bodt_bcke : 1;
                            /* Bits[4]
                               BCOM, BODT, BCKE
                               0 = Moderate Drive
                               1 = Strong Drive
                            */
    UINT8 bck : 1;
                            /* Bits[5]
                               BCK
                               0 = Moderate Drive
                               1 = Strong Drive
                            */
    UINT8 reserved_6_7 : 2;
                            /* Bits[6:7]
                               Reserved
                               Reserved
                               must be coded as 00
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CLOCK_AND_DB_CTRL_STRUCT;

/*  Byte 139 (0x08B) (Load Reduced): Data Buffer Revision Number  */

#define SPD_LRDIMM_DB_REVISION_NUMBER_REG       0x008B

typedef union {
  struct {
    UINT8 data_buffer_revision_number : 8;
                            /* Bits[0:7]
                               Data Buffer Revision Number
                               Programmed in straight Hex format - no conversion needed. 00 - Valid
                               01 - Valid
                               ..
                               FE - Valid
                               FF - Undefined
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_DB_REVISION_NUMBER_STRUCT;

/*  Byte 140 (0x08C) (Load Reduced): DRAM VrefDQ for Package Rank 0  */

#define SPD_LRDIMM_DRAM_VREFDQ_FOR_PACKAGE_RANK_0_REG       0x008C

typedef union {
  struct {
    UINT8 dram_vrefdq_for_package_rank_0_drams : 6;
                            /* Bits[0:5]
                               DRAM VrefDQ for package rank 0 DRAMs
                               Encoding matches MR6 A5:A0 encoding in the JESD79-4 specification.
                            */
    UINT8 reserved_6_7 : 2;
                            /* Bits[6:7]
                               Reserved
                               Reserved
                               must be coded as 00
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_DRAM_VREFDQ_FOR_PACKAGE_RANK_0_STRUCT;

/*  Byte 141 (0x08D) (Load Reduced): DRAM VrefDQ for Package Rank 1  */

#define SPD_LRDIMM_DRAM_VREFDQ_FOR_PACKAGE_RANK_1_REG       0x008D

typedef union {
  struct {
    UINT8 dram_vrefdq_for_package_rank_1_drams : 6;
                            /* Bits[0:5]
                               DRAM VrefDQ for package rank 1 DRAMs
                               Encoding matches MR6 A5:A0 encoding in the JESD79-4 specification.
                            */
    UINT8 reserved_6_7 : 2;
                            /* Bits[6:7]
                               Reserved
                               Reserved
                               must be coded as 00
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_DRAM_VREFDQ_FOR_PACKAGE_RANK_1_STRUCT;

/*  Byte 142 (0x08E) (Load Reduced): DRAM VrefDQ for Package Rank 2  */

#define SPD_LRDIMM_DRAM_VREFDQ_FOR_PACKAGE_RANK_2_REG       0x008E

typedef union {
  struct {
    UINT8 dram_vrefdq_for_package_rank_2_drams : 6;
                            /* Bits[0:5]
                               DRAM VrefDQ for package rank 2 DRAMs
                               Encoding matches MR6 A5:A0 encoding in the JESD79-4 specification.
                            */
    UINT8 reserved_6_7 : 2;
                            /* Bits[6:7]
                               Reserved
                               Reserved
                               must be coded as 00
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_DRAM_VREFDQ_FOR_PACKAGE_RANK_2_STRUCT;

/*  Byte 143 (0x08F) (Load Reduced): DRAM VrefDQ for Package Rank 3  */

#define SPD_LRDIMM_DRAM_VREFDQ_FOR_PACKAGE_RANK_3_REG       0x008F

typedef union {
  struct {
    UINT8 dram_vrefdq_for_package_rank_3_drams : 6;
                            /* Bits[0:5]
                               DRAM VrefDQ for package rank 3 DRAMs
                            */
    UINT8 reserved_6_7 : 2;
                            /* Bits[6:7]
                               Reserved
                               Encoding matches MR6 A5:A0 encoding in the JESD79-4 specification.
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_DRAM_VREFDQ_FOR_PACKAGE_RANK_3_STRUCT;

/*  Byte 144 (0x090) (Load Reduced): Data Buffer VrefDQ for DRAM Interface  */

#define SPD_LRDIMM_DB_VREFDQ_FOR_DRAM_INTERFACE_REG       0x0090

typedef union {
  struct {
    UINT8 data_buffer_vrefdq_for_dram_interface : 8;
                            /* Bits[0:7]
                               Data Buffer VrefDQ for DRAM Interface
                               For encoding see definition of F5BC6x (DRAM Interface VREF Control word) in the DDR4DB01 specification.
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_DB_VREFDQ_FOR_DRAM_INTERFACE_STRUCT;

/*  Byte 145 (0x091) (Load Reduced): Data Buffer MDQ Drive Strength and RTT for data rate < 1866  */

#define SPD_LRDIMM_DB_MDQ_DRIVE_STRENGTH_AND_RTT_REG       0x0091

typedef union {
  struct {
    UINT8 dram_interface_mdq_read_termination_strength : 4;
                            /* Bits[0:3]
                               DRAM Interface MDQ Read Termination Strength
                               Reserved
                               must be coded as 0
                               000 = Disabled
                               001 = RZQ/4 (60)
                               010 = RZQ/2 (120)
                               011 = RZQ/6 (40)
                               100 = RZQ (240)
                               101 = RZQ/5 (48)
                               110 = RZQ/3 (80)
                               111 = RZQ/7 (34)
                            */
    UINT8 dram_interface_mdq_drive_strength : 4;
                            /* Bits[4:7]
                               DRAM Interface MDQ Drive Strength
                               Reserved
                               must be coded as 0
                               000 = RZQ/6 (40)
                               001 = RZQ/7 (34)
                               010 = RZQ/5 (48)
                               011 = Reserved
                               100 = Reserved
                               101 = RZQ/4 (60)
                               110 = Reserved
                               111 = Reserved
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_DB_MDQ_DRIVE_STRENGTH_AND_RTT_STRUCT;

/*  Byte 146 (0x092) (Load Reduced): Data Buffer MDQ Drive Strength and RTT for 1866 < data rate < 2400  */

#define SPD_LRDIMM_DB_MDQ_DRIVE_STRENGTH_AND_RTT_0_REG       0x0092

typedef union {
  struct {
    UINT8 dram_interface_mdq_read_termination_strength : 4;
                            /* Bits[0:3]
                               DRAM Interface MDQ Read Termination Strength
                               Reserved
                               must be coded as 0
                               000 = Disabled
                               001 = RZQ/4 (60)
                               010 = RZQ/2 (120)
                               011 = RZQ/6 (40)
                               100 = RZQ (240)
                               101 = RZQ/5 (48)
                               110 = RZQ/3 (80)
                               111 = RZQ/7 (34)
                            */
    UINT8 dram_interface_mdq_drive_strength : 4;
                            /* Bits[4:7]
                               DRAM Interface MDQ Drive Strength
                               Reserved
                               must be coded as 0
                               000 = RZQ/6 (40)
                               001 = RZQ/7 (34)
                               010 = RZQ/5 (48)
                               011 = Reserved
                               100 = Reserved
                               101 = RZQ/4 (60)
                               110 = Reserved
                               111 = Reserved
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_DB_MDQ_DRIVE_STRENGTH_AND_RTT_0_STRUCT;

/*  Byte 147 (0x093) (Load Reduced): Data Buffer MDQ Drive Strength and RTT for 2400 < data rate < 3200  */

#define SPD_LRDIMM_DB_MDQ_DRIVE_STRENGTH_AND_RTT_1_REG       0x0093

typedef union {
  struct {
    UINT8 dram_interface_mdq_read_termination_strength : 4;
                            /* Bits[0:3]
                               DRAM Interface MDQ Read Termination Strength
                               Reserved
                               must be coded as 0
                               000 = Disabled
                               001 = RZQ/6 (60)
                               010 = RZQ/2 (120)
                               011 = RZQ/4 (40)
                               100 = RZQ (240)
                               101 = RZQ/5 (48)
                               110 = RZQ/3 (80)
                               111 = RZQ/7 (34)
                            */
    UINT8 dram_interface_mdq_drive_strength : 4;
                            /* Bits[4:7]
                               DRAM Interface MDQ Drive Strength
                               Reserved
                               must be coded as 0
                               000 = RZQ/6 (40)
                               001 = RZQ/7 (34)
                               010 = RZQ/5 (48)
                               011 = Reserved
                               100 = Reserved
                               101 = RZQ/4 (60)
                               110 = Reserved
                               111 = Reserved
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_DB_MDQ_DRIVE_STRENGTH_AND_RTT_1_STRUCT;

/*  Byte 148 (0x094) (Load Reduced): DRAM Drive Strength (for data rates < 1866, 1866 < data rate < 2400, and 2400 < data rate < 3200)  */

#define SPD_LRDIMM_DRAM_DRIVE_STRENGTH_REG       0x0094

typedef union {
  struct {
    UINT8 dram_drive_0_1 : 2;
                            /* Bits[0:1]
                               Data rate < 1866
                               00 = RZQ/7 (34)
                               01 = RZQ/5 (48)
                               10 = Reserved
                               11 = Reserved
                            */
    UINT8 dram_drive_2_3 : 2;
                            /* Bits[2:3]
                               1866 < Data rate < 2400
                               00 = RZQ/7 (34)
                               01 = RZQ/5 (48)
                               10 = Reserved
                               11 = Reserved
                            */
    UINT8 dram_drive_4_5 : 2;
                            /* Bits[4:5]
                               2400 < Data rate < 3200
                               00 = RZQ/7 (34)
                               01 = RZQ/5 (48)
                               10 = Reserved
                               11 = Reserved
                            */
    UINT8 reserved_6_7 : 2;
                            /* Bits[6:7]
                               Reserved
                               Reserved
                               must be coded as 00
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_DRAM_DRIVE_STRENGTH_STRUCT;

/*  Byte 149 (0x095) (Load Reduced): DRAM ODT (RTTWR and RTTNOM) for data rate < 1866  */

#define SPD_LRDIMM_DRAM_ODT_RTT_WR_AND_RTT_NOM_REG       0x0095

typedef union {
  struct {
    UINT8 rtt_nom : 3;
                            /* Bits[0:2]
                               RTT_NOM
                               000 = Disabled
                               001 = RZQ/4 (60)
                               010 = RZQ/2 (120)
                               011 = RZQ/6 (40)
                               100 = RZQ (240)
                               101 = RZQ/5 (48)
                               110 = RZQ/3 (80)
                               111 = RZQ/7 (34)
                            */
    UINT8 rtt_wr : 3;
                            /* Bits[3:5]
                               RTT_WR
                               000 = Dynamic ODT Off
                               001 = RZQ/2 (120)
                               010 = RZQ (240)
                               011 = Hi-Impedance
                               100 = RZQ/3 (80)
                               101 = Reserved
                               110 = Reserved
                               111 = Reserved
                            */
    UINT8 reserved_6_7 : 2;
                            /* Bits[6:7]
                               Reserved
                               Reserved
                               must be coded as 00
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_DRAM_ODT_RTT_WR_AND_RTT_NOM_STRUCT;

/*  Byte 150 (0x096) (Load Reduced): DRAM ODT (RTTWR and RTTNOM) for 1866 < data rate < 2400  */

#define SPD_LRDIMM_DRAM_ODT_RTT_WR_AND_RTT_NOM_2_REG       0x0096

typedef union {
  struct {
    UINT8 rtt_nom : 3;
                            /* Bits[0:2]
                               RTT_NOM
                               000 = Disabled
                               001 = RZQ/4 (60)
                               010 = RZQ/2 (120)
                               011 = RZQ/6 (40)
                               100 = RZQ (240)
                               101 = RZQ/5 (48)
                               110 = RZQ/3 (80)
                               111 = RZQ/7 (34)
                            */
    UINT8 rtt_wr : 3;
                            /* Bits[3:5]
                               RTT_WR
                               000 = Dynamic ODT Off
                               001 = RZQ/2 (120)
                               010 = RZQ (240)
                               011 = Hi-Impedance
                               100 = RZQ/3 (80)
                               101 = Reserved
                               110 = Reserved
                               111 = Reserved
                            */
    UINT8 reserved_6_7 : 2;
                            /* Bits[6:7]
                               Reserved
                               Reserved
                               must be coded as 00
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_DRAM_ODT_RTT_WR_AND_RTT_NOM_2_STRUCT;

/*  Byte 151 (0x097) (Load Reduced): DRAM ODT (RTTWR and RTTNOM) for 2400 < data rate < 3200  */

#define SPD_LRDIMM_DRAM_ODT_RTT_WR_AND_RTT_NOM_3_REG       0x0097

typedef union {
  struct {
    UINT8 rtt_nom : 3;
                            /* Bits[0:2]
                               RTT_NOM
                               000 = Disabled
                               001 = RZQ/4 (60)
                               010 = RZQ/2 (120)
                               011 = RZQ/6 (40)
                               100 = RZQ (240)
                               101 = RZQ/5 (48)
                               110 = RZQ/3 (80)
                               111 = RZQ/7 (34)
                            */
    UINT8 rtt_wr : 3;
                            /* Bits[3:5]
                               RTT_WR
                               000 = Dynamic ODT Off
                               001 = RZQ/2 (120)
                               010 = RZQ (240)
                               011 = Hi-Impedance
                               100 = RZQ/3 (80)
                               101 = Reserved
                               110 = Reserved
                               111 = Reserved
                            */
    UINT8 reserved_6_7 : 2;
                            /* Bits[6:7]
                               Reserved
                               Reserved
                               must be coded as 00
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_DRAM_ODT_RTT_WR_AND_RTT_NOM_3_STRUCT;

/*  Byte 152 (0x098) (Load Reduced): DRAM ODT (RTTPARK) for data rate < 1866  */

#define SPD_LRDIMM_DRAM_ODT_RTT_PARK_FOR_DATA_RATE_1866_REG       0x0098

typedef union {
  struct {
    UINT8 rtt_park_package_ranks_0_1 : 3;
                            /* Bits[0:2]
                               RTT_PARK, Package Ranks 0 & 1
                               000 = Disabled
                               001 = RZQ/4 (60)
                               010 = RZQ/2 (120)
                               011 = RZQ/6 (40)
                               100 = RZQ (240)
                               101 = RZQ/5 (48)
                               110 = RZQ/3 (80)
                               111 = RZQ/7 (34)
                            */
    UINT8 rtt_park_package_ranks_2_3 : 3;
                            /* Bits[3:5]
                               RTT_PARK, Package Ranks 2 & 3
                               000 = Disabled
                               001 = RZQ/4 (60)
                               010 = RZQ/2 (120)
                               011 = RZQ/6 (40)
                               100 = RZQ (240)
                               101 = RZQ/5 (48)
                               110 = RZQ/3 (80)
                               111 = RZQ/7 (34)
                            */
    UINT8 reserved_6_7 : 2;
                            /* Bits[6:7]
                               Reserved
                               Reserved
                               must be coded as 00
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_DRAM_ODT_RTT_PARK_FOR_DATA_RATE_1866_STRUCT;

/*  Byte 153 (0x099) (Load Reduced): DRAM ODT (RTTPARK) for 1866 < data rate < 2400  */

#define SPD_LRDIMM_DRAM_ODT_RTT_PARK_FOR_1866_DATA_RATE_2400_REG       0x0099

typedef union {
  struct {
    UINT8 rtt_park_package_ranks_0_1 : 3;
                            /* Bits[0:2]
                               RTT_PARK, Package Ranks 0 & 1
                               000 = Disabled
                               001 = RZQ/4 (60)
                               010 = RZQ/2 (120)
                               011 = RZQ/6 (40)
                               100 = RZQ (240)
                               101 = RZQ/5 (48)
                               110 = RZQ/3 (80)
                               111 = RZQ/7 (34)
                            */
    UINT8 rtt_park_package_ranks_2_3 : 3;
                            /* Bits[3:5]
                               RTT_PARK, Package Ranks 2 & 3
                               000 = Disabled
                               001 = RZQ/4 (60)
                               010 = RZQ/2 (120)
                               011 = RZQ/6 (40)
                               100 = RZQ (240)
                               101 = RZQ/5 (48)
                               110 = RZQ/3 (80)
                               111 = RZQ/7 (34)
                            */
    UINT8 reserved_6_7 : 2;
                            /* Bits[6:7]
                               Reserved
                               Reserved
                               must be coded as 00
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_DRAM_ODT_RTT_PARK_FOR_1866_DATA_RATE_2400_STRUCT;

/*  Byte 154 (0x09A) (Load Reduced): DRAM ODT (RTTPARK) for 2400 < data rate < 3200  */

#define SPD_LRDIMM_DRAM_ODT_RTT_PARK_FOR_2400_DATA_RATE_3200_REG       0x009A

typedef union {
  struct {
    UINT8 rtt_park_package_ranks_0_1 : 3;
                            /* Bits[0:2]
                               RTT_PARK, Package Ranks 0 & 1
                               000 = Disabled
                               001 = RZQ/4 (60)
                               010 = RZQ/2 (120)
                               011 = RZQ/6 (40)
                               100 = RZQ (240)
                               101 = RZQ/5 (48)
                               110 = RZQ/3 (80)
                               111 = RZQ/7 (34)
                            */
    UINT8 rtt_park_package_ranks_2_3 : 3;
                            /* Bits[3:5]
                               RTT_PARK, Package Ranks 2 & 3
                               000 = Disabled
                               001 = RZQ/4 (60)
                               010 = RZQ/2 (120)
                               011 = RZQ/6 (40)
                               100 = RZQ (240)
                               101 = RZQ/5 (48)
                               110 = RZQ/3 (80)
                               111 = RZQ/7 (34)
                            */
    UINT8 reserved_6_7 : 2;
                            /* Bits[6:7]
                               Reserved
                               Reserved
                               must be coded as 00
                            */
  } Bits;
  UINT8 Data;
} LRDIMM_DRAM_ODT_RTT_PARK_FOR_2400_DATA_RATE_3200_STRUCT;

/*  Byte 192 (0x0C0) (NVDIMM): Module Product Identifier, LSB  */

#define SPD_NVDIMM_MODULE_PRODUCT_IDENTIFIER_LSB_REG       0x00C0

/*  Byte 193 (0x0C1) (NVDIMM): Module Product Identifier, MSB  */

#define SPD_NVDIMM_MODULE_PRODUCT_IDENTIFIER_MSB_REG       0x00C1

/*  Byte 194 (0x0C2) (NVDIMM): Non-Volatile Memory Subsystem Controller Manufacturers ID Code, LSB  */

#define SPD_NVDIMM_SUBSYS_CTRL_MANUFACTURERS_ID_CODE_LSB_REG       0x00C2

typedef union {
  struct {
    UINT8 number_of_continuation_codes_flash_manufacturer : 7;
                            /* Bits[0:6]
                               Number of continuation codes, Flash Manufacturer
                               See JEP-106
                            */
    UINT8 odd_parity_for_byte_194_bits_6_0 : 1;
                            /* Bits[7]
                               Odd Parity for Byte 194, bits 6~0
                            */
  } Bits;
  UINT8 Data;
} NVDIMM_SUBSYS_CTRL_MANUFACTURERS_ID_CODE_LSB_STRUCT;

/*  Byte 195 (0x0C3) (NVDIMM): Non-Volatile Memory Subsystem Controller Manufacturers ID Code, MSB  */

#define SPD_NVDIMM_SUBSYS_CTRL_MANUFACTURERS_ID_CODE_MSB_REG       0x00C3

typedef union {
  struct {
    UINT8 last_non_zero_byte_flash_manufacturer : 8;
                            /* Bits[0:7]
                               Last non-zero byte, Flash Manufacturer
                               See JEP-106
                            */
  } Bits;
  UINT8 Data;
} NVDIMM_SUBSYS_CTRL_MANUFACTURERS_ID_CODE_MSB_STRUCT;

/*  Byte 196 (0x0C4) (NVDIMM): Non-Volatile Memory Subsystem Controller Identifier, LSB  */

#define SPD_NVDIMM_SUBSYSTEM_CONTROLLER_IDENTIFIER_LSB_REG       0x00C4

/*  Byte 197 (0x0C5) (NVDIMM): Non-Volatile Memory Subsystem Controller Identifier, MSB  */

#define SPD_NVDIMM_SUBSYSTEM_CONTROLLER_IDENTIFIER_MSB_REG       0x00C5

/*  Byte 198 (0x0C6) (NVDIMM): Non-Volatile Memory Subsystem Controller Revision Code  */

#define SPD_NVDIMM_SUBSYSTEM_CONTROLLER_REVISION_CODE_REG       0x00C6

/*  Byte 199 (0x0C7) (NVDIMM): Reference Raw Card Used  */

#define SPD_NVDIMM_REFERENCE_RAW_CARD_USED_REG       0x00C7

typedef union {
  struct {
    UINT8 reference_raw_card : 5;
                            /* Bits[0:4]
                               Reference Raw Card
                               When bit 7 = 0,
                               00000 = Reference raw card A
                               00001 = Reference raw card B
                               00010 = Reference raw card C
                               00011 = Reference raw card D
                               00100 = Reference raw card E
                               00101 = Reference raw card F
                               00110 = Reference raw card G
                               00111 = Reference raw card H
                               01000 = Reference raw card J
                               01001 = Reference raw card K
                               01010 = Reference raw card L
                               01011 = Reference raw card M
                               01100 = Reference raw card N
                               01101 = Reference raw card P
                               01110 = Reference raw card R
                               01111 = Reference raw card T
                               10000 = Reference raw card U
                               10001 = Reference raw card V
                               10010 = Reference raw card W
                               10011 = Reference raw card Y
                               10100 = Reference raw card AA
                               10101 = Reference raw card AB
                               10110 = Reference raw card AC
                               10111 = Reference raw card AD
                               11000 = Reference raw card AE
                               11001 = Reference raw card AF
                               11010 = Reference raw card AG
                               11011 = Reference raw card AH
                               11100 = Reference raw card AJ
                               11101 = Reference raw card AK
                               11110 = Reference raw card AL
                               11111 = ZZ (no JEDEC reference raw card design used)
                            */
    UINT8 reference_raw_card_revision : 2;
                            /* Bits[5:6]
                               Reference Raw Card Revision
                               00 = revision 0
                               01 = revision 1
                               10 = revision 2
                               11 = revision 3
                               See byte 200 for extensions beyond revision 3
                               these bits must be coded as 11 for all revisions greater than 3
                            */
    UINT8 reference_raw_card_extension : 1;
                            /* Bits[7]
                               Reference Raw Card Extension
                               0 = Reference raw cards A through AL
                               1 = Reference raw cards AM throughCB
                            */
  } Bits;
  UINT8 Data;
} NVDIMM_REFERENCE_RAW_CARD_USED_STRUCT;

/*  Byte 200 (0x0C8) (NVDIMM): Module Characteristics  */

#define SPD_NVDIMM_MODULE_CHARACTERISTICS_REG       0x00C8

typedef union {
  struct {
    UINT8 reserved_0_4 : 5;
                            /* Bits[0:4]
                               Reserved
                               Reserved
                               must be coded as 00000
                            */
    UINT8 raw_card_extension : 3;
                            /* Bits[5:7]
                               Raw Card Extension
                               000 = raw card revisions 0 to 3
                               see byte 199
                               001 = raw card revision 4
                               010 = raw card revision 5
                               011 = raw card revision 6
                               100 = raw card revision 7
                               101 = raw card revision 8
                               110 = raw card revision 9
                               111 = raw card revision 10
                            */
  } Bits;
  UINT8 Data;
} NVDIMM_MODULE_CHARACTERISTICS_STRUCT;

/*  Byte 201 (0x0C9)(NVDIMM): Hybrid Module Media Types  */

#define SPD_NVDIMM_HYBRID_MODULE_MEDIA_TYPES_REG       0x00C9

typedef union {
  struct {
    UINT8 reserved_3_7 : 5;
                            /* Bits[3:7]
                               Reserved
                            */
    UINT8 nand_flash : 1;
                            /* Bits[2]
                               NAND Flash
                            */
    UINT8 sdram : 1;
                            /* Bits[1]
                               SDRAM
                            */
    UINT8 unknown_undefined : 1;
                            /* Bits[0]
                               Unknown/undefined
                            */
  } Bits;
  UINT8 Data;
} NVDIMM_HYBRID_MODULE_MEDIA_TYPES_STRUCT;

/*  Byte 202 (0x0CA)(NVDIMM): Hybrid Module Media Types  */

#define SPD_NVDIMM_HYBRID_MODULE_MEDIA_TYPES_3_REG       0x00CA

typedef union {
  struct {
    UINT8 reserved_0_7 : 8;
                            /* Bits[0:7]
                               Reserved
                            */
  } Bits;
  UINT8 Data;
} NVDIMM_HYBRID_MODULE_MEDIA_TYPES_3_STRUCT;

/*  Byte 203 (0x0CB) (NVDIMM): Maximum Non-Volatile Memory Initialization Time  */

#define SPD_NVDIMM_MAXIMUM_INITIALIZATION_TIME_REG       0x00CB

typedef union {
  struct {
    UINT8 min_non_volatile_memory_initialization_time_seconds : 8;
                            /* Bits[0:7]
                               Minimum Non-Volatile Memory Initialization Time Seconds
                               Values defined from 0 to 254
                            */
  } Bits;
  UINT8 Data;
} NVDIMM_MAXIMUM_INITIALIZATION_TIME_STRUCT;

/*  Byte 204 (0x0CC): Function 0 LSB  */

#define SPD_FUNCTION_0_LSB_REG       0x00CC

typedef union {
  struct {
    UINT8 function_interface : 5;
                            /* Bits[0:4]
                               Function Interface
                               See table for Function Classes and Function Interfaces
                            */
    UINT8 function_class_lsb : 3;
                            /* Bits[5:7]
                               Function Class lsb
                               See table for Function Classes and Function Interfaces
                            */
  } Bits;
  UINT8 Data;
} FUNCTION_0_LSB_STRUCT;

/*  Byte 205 (0x0CD): Function 0 MSB  */

#define SPD_FUNCTION_0_MSB_REG       0x00CD

typedef union {
  struct {
    UINT8 function_class_msb : 2;
                            /* Bits[0:1]
                               Function Class msb
                               See table for Function Classes and Function Interfaces
                            */
    UINT8 block_offset : 4;
                            /* Bits[2:5]
                               Block Offset
                               Offset of additional information in Extended Function Parameter Block.
                            */
    UINT8 reserved_6 : 1;
                            /* Bits[6]
                               Reserved
                               Reserved
                               must be coded as 0
                            */
    UINT8 implemented : 1;
                            /* Bits[7]
                               Implemented
                               0 = Not implemented
                               1 = Implemented
                            */
  } Bits;
  UINT8 Data;
} FUNCTION_0_MSB_STRUCT;

/*  Byte 206 (0x0CE): Function 1 LSB  */

#define SPD_FUNCTION_1_LSB_REG       0x00CE

typedef union {
  struct {
    UINT8 function_interface : 5;
                            /* Bits[0:4]
                               Function Interface
                               See table for Function Classes and Function Interfaces
                            */
    UINT8 function_class_lsb : 3;
                            /* Bits[5:7]
                               Function Class lsb
                               See table for Function Classes and Function Interfaces
                            */
  } Bits;
  UINT8 Data;
} FUNCTION_1_LSB_STRUCT;

/*  Byte 207 (0x0CF): Function 1 MSB  */

#define SPD_FUNCTION_1_MSB_REG       0x00CF

typedef union {
  struct {
    UINT8 function_class_msb : 2;
                            /* Bits[0:1]
                               Function Class msb
                               See table for Function Classes and Function Interfaces
                            */
    UINT8 block_offset : 4;
                            /* Bits[2:5]
                               Block Offset
                               Offset of additional information in Extended Function Parameter Block.
                            */
    UINT8 reserved_6 : 1;
                            /* Bits[6]
                               Reserved
                               Reserved
                               must be coded as 0
                            */
    UINT8 implemented : 1;
                            /* Bits[7]
                               Implemented
                               0 = Not implemented
                               1 = Implemented
                            */
  } Bits;
  UINT8 Data;
} FUNCTION_1_MSB_STRUCT;

/*  Byte 208 (0x0D0): Function 2 LSB  */

#define SPD_FUNCTION_2_LSB_REG       0x00D0

typedef union {
  struct {
    UINT8 function_interface : 5;
                            /* Bits[0:4]
                               Function Interface
                               See table for Function Classes and Function Interfaces
                            */
    UINT8 function_class_lsb : 3;
                            /* Bits[5:7]
                               Function Class lsb
                               See table for Function Classes and Function Interfaces
                            */
  } Bits;
  UINT8 Data;
} FUNCTION_2_LSB_STRUCT;

/*  Byte 209 (0x0D1): Function 2 MSB  */

#define SPD_FUNCTION_2_MSB_REG       0x00D1

typedef union {
  struct {
    UINT8 function_class_msb : 2;
                            /* Bits[0:1]
                               Function Class msb
                               See table for Function Classes and Function Interfaces
                            */
    UINT8 block_offset : 4;
                            /* Bits[2:5]
                               Block Offset
                               Offset of additional information in Extended Function Parameter Block.
                            */
    UINT8 reserved_6 : 1;
                            /* Bits[6]
                               Reserved
                               Reserved
                               must be coded as 0
                            */
    UINT8 implemented : 1;
                            /* Bits[7]
                               Implemented
                               0 = Not implemented
                               1 = Implemented
                            */
  } Bits;
  UINT8 Data;
} FUNCTION_2_MSB_STRUCT;

/*  Byte 210 (0x0D2): Function 3 LSB  */

#define SPD_FUNCTION_3_LSB_REG       0x00D2

typedef union {
  struct {
    UINT8 function_interface : 5;
                            /* Bits[0:4]
                               Function Interface
                               See table for Function Classes and Function Interfaces
                            */
    UINT8 function_class_lsb : 3;
                            /* Bits[5:7]
                               Function Class lsb
                               See table for Function Classes and Function Interfaces
                            */
  } Bits;
  UINT8 Data;
} FUNCTION_3_LSB_STRUCT;

/*  Byte 211 (0x0D3): Function 3 MSB  */

#define SPD_FUNCTION_3_MSB_REG       0x00D3

typedef union {
  struct {
    UINT8 function_class_msb : 2;
                            /* Bits[0:1]
                               Function Class msb
                               See table for Function Classes and Function Interfaces
                            */
    UINT8 block_offset : 4;
                            /* Bits[2:5]
                               Block Offset
                               Offset of additional information in Extended Function Parameter Block.
                            */
    UINT8 reserved_6 : 1;
                            /* Bits[6]
                               Reserved
                               Reserved
                               must be coded as 0
                            */
    UINT8 implemented : 1;
                            /* Bits[7]
                               Implemented
                               0 = Not implemented
                               1 = Implemented
                            */
  } Bits;
  UINT8 Data;
} FUNCTION_3_MSB_STRUCT;

/*  Byte 212 (0x0D4): Function 4 LSB  */

#define SPD_FUNCTION_4_LSB_REG       0x00D4

typedef union {
  struct {
    UINT8 function_interface : 5;
                            /* Bits[0:4]
                               Function Interface
                               See table for Function Classes and Function Interfaces
                            */
    UINT8 function_class_lsb : 3;
                            /* Bits[5:7]
                               Function Class lsb
                               See table for Function Classes and Function Interfaces
                            */
  } Bits;
  UINT8 Data;
} FUNCTION_4_LSB_STRUCT;

/*  Byte 213 (0x0D5): Function 4 MSB  */

#define SPD_FUNCTION_4_MSB_REG       0x00D5

typedef union {
  struct {
    UINT8 function_class_msb : 2;
                            /* Bits[0:1]
                               Function Class msb
                               See table for Function Classes and Function Interfaces
                            */
    UINT8 block_offset : 4;
                            /* Bits[2:5]
                               Block Offset
                               Offset of additional information in Extended Function Parameter Block.
                            */
    UINT8 reserved_6 : 1;
                            /* Bits[6]
                               Reserved
                               Reserved
                               must be coded as 0
                            */
    UINT8 implemented : 1;
                            /* Bits[7]
                               Implemented
                               0 = Not implemented
                               1 = Implemented
                            */
  } Bits;
  UINT8 Data;
} FUNCTION_4_MSB_STRUCT;

/*  Byte 214 (0x0D6): Function 5 LSB  */

#define SPD_FUNCTION_5_LSB_REG       0x00D6

typedef union {
  struct {
    UINT8 function_interface : 5;
                            /* Bits[0:4]
                               Function Interface
                               See table for Function Classes and Function Interfaces
                            */
    UINT8 function_class_lsb : 3;
                            /* Bits[5:7]
                               Function Class lsb
                               See table for Function Classes and Function Interfaces
                            */
  } Bits;
  UINT8 Data;
} FUNCTION_5_LSB_STRUCT;

/*  Byte 215 (0x0D7): Function 5 MSB  */

#define SPD_FUNCTION_5_MSB_REG       0x00D7

typedef union {
  struct {
    UINT8 function_class_msb : 2;
                            /* Bits[0:1]
                               Function Class msb
                               See table for Function Classes and Function Interfaces
                            */
    UINT8 block_offset : 4;
                            /* Bits[2:5]
                               Block Offset
                               Offset of additional information in Extended Function Parameter Block.
                            */
    UINT8 reserved_6 : 1;
                            /* Bits[6]
                               Reserved
                               Reserved
                               must be coded as 0
                            */
    UINT8 implemented : 1;
                            /* Bits[7]
                               Implemented
                               0 = Not implemented
                               1 = Implemented
                            */
  } Bits;
  UINT8 Data;
} FUNCTION_5_MSB_STRUCT;

/*  Byte 216 (0x0D8): Function 6 LSB  */

#define SPD_FUNCTION_6_LSB_REG       0x00D8

typedef union {
  struct {
    UINT8 function_interface : 5;
                            /* Bits[0:4]
                               Function Interface
                               See table for Function Classes and Function Interfaces
                            */
    UINT8 function_class_lsb : 3;
                            /* Bits[5:7]
                               Function Class lsb
                               See table for Function Classes and Function Interfaces
                            */
  } Bits;
  UINT8 Data;
} FUNCTION_6_LSB_STRUCT;

/*  Byte 217 (0x0D9): Function 6 MSB  */

#define SPD_FUNCTION_6_MSB_REG       0x00D9

typedef union {
  struct {
    UINT8 function_class_msb : 2;
                            /* Bits[0:1]
                               Function Class msb
                               See table for Function Classes and Function Interfaces
                            */
    UINT8 block_offset : 4;
                            /* Bits[2:5]
                               Block Offset
                               Offset of additional information in Extended Function Parameter Block.
                            */
    UINT8 reserved_6 : 1;
                            /* Bits[6]
                               Reserved
                               Reserved
                               must be coded as 0
                            */
    UINT8 implemented : 1;
                            /* Bits[7]
                               Implemented
                               0 = Not implemented
                               1 = Implemented
                            */
  } Bits;
  UINT8 Data;
} FUNCTION_6_MSB_STRUCT;

/*  Byte 218 (0x0DA): Function 7 LSB  */

#define SPD_FUNCTION_7_LSB_REG       0x00DA

typedef union {
  struct {
    UINT8 function_interface : 5;
                            /* Bits[0:4]
                               Function Interface
                               See table for Function Classes and Function Interfaces
                            */
    UINT8 function_class_lsb : 3;
                            /* Bits[5:7]
                               Function Class lsb
                               See table for Function Classes and Function Interfaces
                            */
  } Bits;
  UINT8 Data;
} FUNCTION_7_LSB_STRUCT;

/*  Byte 219 (0x0DB): Function 7 MSB  */

#define SPD_FUNCTION_7_MSB_REG       0x00DB

typedef union {
  struct {
    UINT8 function_class_msb : 2;
                            /* Bits[0:1]
                               Function Class msb
                               See table for Function Classes and Function Interfaces
                            */
    UINT8 block_offset : 4;
                            /* Bits[2:5]
                               Block Offset
                               Offset of additional information in Extended Function Parameter Block.
                            */
    UINT8 reserved_6 : 1;
                            /* Bits[6]
                               Reserved
                               Reserved
                               must be coded as 0
                            */
    UINT8 implemented : 1;
                            /* Bits[7]
                               Implemented
                               0 = Not implemented
                               1 = Implemented
                            */
  } Bits;
  UINT8 Data;
} FUNCTION_7_MSB_STRUCT;

/*  Byte 254 (0x0FE) (Unbuffered): Cyclical Redundancy Code (CRC) for SPD Block 1, LSB  */

#define SPD_UDIMM_CRC_FOR_SPD_BLOCK_1_LSB_REG       0x00FE

/*  Byte 254 (0x0FE) (Registered): Cyclical Redundancy Code (CRC) for SPD Block 1, LSB  */

#define SPD_RDIMM_CRC_FOR_SPD_BLOCK_1_LSB_REG       0x00FE

/*  Byte 254 (0x0FE) (Load Reduced): Cyclical Redundancy Code (CRC) for SPD Block 1, LSB  */

#define SPD_LRDIMM_CRC_FOR_SPD_BLOCK_1_LSB_REG       0x00FE

/*  Byte 254 (0x0FE) (NVDIMM): Cyclical Redundancy Code (CRC) for SPD Block 1, LSB  */

#define SPD_NVDIMM_CRC_FOR_SPD_BLOCK_1_LSB_REG       0x00FE

/*  Byte 255 (0x0FF) (Unbuffered): Cyclical Redundancy Code (CRC) for SPD Block 1, MSB  */

#define SPD_UDIMM_CRC_FOR_SPD_BLOCK_1_MSB_REG       0x00FF

/*  Byte 255 (0x0FF) (Registered): Cyclical Redundancy Code (CRC) for SPD Block 1, MSB  */

#define SPD_RDIMM_CRC_FOR_SPD_BLOCK_1_MSB_REG       0x00FF

/*  Byte 255 (0x0FF) (Load Reduced): Cyclical Redundancy Code (CRC) for SPD Block 1, MSB  */

#define SPD_LRDIMM_CRC_FOR_SPD_BLOCK_1_MSB_REG       0x00FF

/*  Byte 255 (0x0FF) (NVDIMM): Cyclical Redundancy Code (CRC) for SPD Block 1, MSB  */

#define SPD_NVDIMM_CRC_FOR_SPD_BLOCK_1_MSB_REG       0x00FF

/*  Byte 320 (0x140): Module Manufacturer ID Code, LSB  */

#define SPD_MODULE_MANUFACTURER_ID_CODE_LSB_REG       0x0140

typedef union {
  struct {
    UINT8 number_of_continuation_codes_module_manufacturer : 7;
                            /* Bits[0:6]
                               Number of continuation codes, Module Manufacturer
                               See JEP-106
                            */
    UINT8 odd_parity_for_byte_320_bits_6_0 : 1;
                            /* Bits[7]
                               Odd Parity for Byte 320, bits 6~0
                            */
  } Bits;
  UINT8 Data;
} MODULE_MANUFACTURER_ID_CODE_LSB_STRUCT;

/*  Byte 321 (0x141): Module Manufacturer ID Code, MSB  */

#define SPD_MODULE_MANUFACTURER_ID_CODE_MSB_REG       0x0141

typedef union {
  struct {
    UINT8 last_non_zero_byte_module_manufacturer : 8;
                            /* Bits[0:7]
                               Last non-zero byte, Module Manufacturer
                               See JEP-106
                            */
  } Bits;
  UINT8 Data;
} MODULE_MANUFACTURER_ID_CODE_MSB_STRUCT;

/*  Byte 322 (0x142): Module Manufacturing Location  */

#define SPD_MODULE_MANUFACTURING_LOCATION_REG       0x0142

/*  Bytes 323~324 (0x143~0x144): Module Manufacturing Date
Byte 323 (0x143)  Module Manufacturing Date  */

#define SPD_MODULE_MANUFACTURING_DATE_0_REG       0x0143

/*  Bytes 323~324 (0x143~0x144): Module Manufacturing Date
Byte 324 (0x144)  Module Manufacturing Date  */

#define SPD_MODULE_MANUFACTURING_DATE_1_REG       0x0144

/*  Bytes 325~328 (0x145~0x148): Module Serial Number
Byte 325 (0x145)  Module Serial Number  */

#define SPD_MODULE_SERIAL_NUMBER_0_REG       0x0145

/*  Bytes 325~328 (0x145~0x148): Module Serial Number
Byte 326 (0x146)  Module Serial Number  */

#define SPD_MODULE_SERIAL_NUMBER_1_REG       0x0146

/*  Bytes 325~328 (0x145~0x148): Module Serial Number
Byte 327 (0x147)  Module Serial Number  */

#define SPD_MODULE_SERIAL_NUMBER_2_REG       0x0147

/*  Bytes 325~328 (0x145~0x148): Module Serial Number
Byte 328 (0x148)  Module Serial Number  */

#define SPD_MODULE_SERIAL_NUMBER_3_REG       0x0148

/*  Bytes 329~348 (0x149~15C): Module Part Number
Byte 329 (0x149)  Module Part Number  */

#define SPD_MODULE_PART_NUMBER_0_REG       0x0149

/*  Bytes 329~348 (0x149~15C): Module Part Number
Byte 330 (0x14A)  Module Part Number  */

#define SPD_MODULE_PART_NUMBER_1_REG       0x014A

/*  Bytes 329~348 (0x149~15C): Module Part Number
Byte 331 (0x14B)  Module Part Number  */

#define SPD_MODULE_PART_NUMBER_2_REG       0x014B

/*  Bytes 329~348 (0x149~15C): Module Part Number
Byte 332 (0x14C)  Module Part Number  */

#define SPD_MODULE_PART_NUMBER_3_REG       0x014C

/*  Bytes 329~348 (0x149~15C): Module Part Number
Byte 333 (0x14D)  Module Part Number  */

#define SPD_MODULE_PART_NUMBER_4_REG       0x014D

/*  Bytes 329~348 (0x149~15C): Module Part Number
Byte 334 (0x14E)  Module Part Number  */

#define SPD_MODULE_PART_NUMBER_5_REG       0x014E

/*  Bytes 329~348 (0x149~15C): Module Part Number
Byte 335 (0x14F)  Module Part Number  */

#define SPD_MODULE_PART_NUMBER_6_REG       0x014F

/*  Bytes 329~348 (0x149~15C): Module Part Number
Byte 336 (0x150)  Module Part Number  */

#define SPD_MODULE_PART_NUMBER_7_REG       0x0150

/*  Bytes 329~348 (0x149~15C): Module Part Number
Byte 337 (0x151)  Module Part Number  */

#define SPD_MODULE_PART_NUMBER_8_REG       0x0151

/*  Bytes 329~348 (0x149~15C): Module Part Number
Byte 338 (0x152)  Module Part Number  */

#define SPD_MODULE_PART_NUMBER_9_REG       0x0152

/*  Bytes 329~348 (0x149~15C): Module Part Number
Byte 339 (0x153)  Module Part Number  */

#define SPD_MODULE_PART_NUMBER_10_REG       0x0153

/*  Bytes 329~348 (0x149~15C): Module Part Number
Byte 340 (0x154)  Module Part Number  */

#define SPD_MODULE_PART_NUMBER_11_REG       0x0154

/*  Bytes 329~348 (0x149~15C): Module Part Number
Byte 341 (0x155)  Module Part Number  */

#define SPD_MODULE_PART_NUMBER_12_REG       0x0155

/*  Bytes 329~348 (0x149~15C): Module Part Number
Byte 342 (0x156)  Module Part Number  */

#define SPD_MODULE_PART_NUMBER_13_REG       0x0156

/*  Bytes 329~348 (0x149~15C): Module Part Number
Byte 343 (0x157)  Module Part Number  */

#define SPD_MODULE_PART_NUMBER_14_REG       0x0157

/*  Bytes 329~348 (0x149~15C): Module Part Number
Byte 344 (0x158)  Module Part Number  */

#define SPD_MODULE_PART_NUMBER_15_REG       0x0158

/*  Bytes 329~348 (0x149~15C): Module Part Number
Byte 345 (0x159)  Module Part Number  */

#define SPD_MODULE_PART_NUMBER_16_REG       0x0159

/*  Bytes 329~348 (0x149~15C): Module Part Number
Byte 346 (0x15A)  Module Part Number  */

#define SPD_MODULE_PART_NUMBER_17_REG       0x015A

/*  Bytes 329~348 (0x149~15C): Module Part Number
Byte 347 (0x15B)  Module Part Number  */

#define SPD_MODULE_PART_NUMBER_18_REG       0x015B

/*  Bytes 329~348 (0x149~15C): Module Part Number
Byte 348 (0x15C)  Module Part Number  */

#define SPD_MODULE_PART_NUMBER_19_REG       0x015C

/*  Byte 349 (0x15D): Module Revision Code  */

#define SPD_MODULE_REVISION_CODE_REG       0x015D

/*  Byte 350 (0x15E): DRAM Manufacturer ID Code, LSB  */

#define SPD_DRAM_MANUFACTURER_ID_CODE_LSB_REG       0x015E

typedef union {
  struct {
    UINT8 number_of_continuation_codes_dram_manufacturer : 7;
                            /* Bits[0:6]
                               Number of continuation codes, DRAM Manufacturer
                               See JEP-106
                            */
    UINT8 odd_parity_for_byte_350_bits_6_0 : 1;
                            /* Bits[7]
                               Odd Parity for Byte 350, bits 6~0
                            */
  } Bits;
  UINT8 Data;
} DRAM_MANUFACTURER_ID_CODE_LSB_STRUCT;

/*  Byte 351 (0x15F): DRAM Manufacturer ID Code, MSB  */

#define SPD_DRAM_MANUFACTURER_ID_CODE_MSB_REG       0x015F

typedef union {
  struct {
    UINT8 last_non_zero_byte_dram_manufacturer : 8;
                            /* Bits[0:7]
                               Last non-zero byte, DRAM Manufacturer
                               See JEP-106
                            */
  } Bits;
  UINT8 Data;
} DRAM_MANUFACTURER_ID_CODE_MSB_STRUCT;

/*  Byte 352 (0x160): DRAM Stepping  */

#define SPD_DRAM_STEPPING_REG       0x0160

typedef union {
  struct {
    UINT8 dram_stepping : 8;
                            /* Bits[0:7]
                               DRAM Stepping
                               Programmed in straight Hex format - no conversion needed. 00 - Valid
                               01 - Valid
                               ..
                               FE - Valid
                               FF - Undefined (No Stepping Number Provided)
                            */
  } Bits;
  UINT8 Data;
} DRAM_STEPPING_STRUCT;

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 353 (0x161)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_0_REG       0x0161

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 354 (0x162)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_1_REG       0x0162

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 355 (0x163)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_2_REG       0x0163

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 356 (0x164)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_3_REG       0x0164

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 357 (0x165)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_4_REG       0x0165

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 358 (0x166)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_5_REG       0x0166

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 359 (0x167)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_6_REG       0x0167

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 360 (0x168)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_7_REG       0x0168

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 361 (0x169)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_8_REG       0x0169

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 362 (0x16A)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_9_REG       0x016A

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 363 (0x16B)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_10_REG       0x016B

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 364 (0x16C)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_11_REG       0x016C

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 365 (0x16D)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_12_REG       0x016D

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 366 (0x16E)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_13_REG       0x016E

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 367 (0x16F)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_14_REG       0x016F

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 368 (0x170)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_15_REG       0x0170

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 369 (0x171)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_16_REG       0x0171

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 370 (0x172)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_17_REG       0x0172

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 371 (0x173)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_18_REG       0x0173

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 372 (0x174)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_19_REG       0x0174

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 373 (0x175)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_20_REG       0x0175

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 374 (0x176)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_21_REG       0x0176

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 375 (0x177)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_22_REG       0x0177

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 376 (0x178)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_23_REG       0x0178

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 377 (0x179)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_24_REG       0x0179

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 378 (0x17A)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_25_REG       0x017A

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 379 (0x17B)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_26_REG       0x017B

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 380 (0x17C)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_27_REG       0x017C

/*  Bytes 353~381 (0x161~0x17D): Manufacturer Specific Data
Byte 381 (0x17D)  Manufacturer Specific Data  */

#define SPD_MANUFACTURER_SPECIFIC_DATA_28_REG       0x017D

/*  Byte 16 (0x010):Reserved, must be coded as 0x00  */

/*  Bytes 78~116 (0x04E~0x074): Reserved, Base Configuration Section:must be coded as 0x00  */

/*  Bytes 132~191 (0x084~0x0BF) (Unbuffered):Reserved, must be coded as 0x00  */

/*  Bytes 139~191 (0x08B~0x0BF) (Registered):Reserved, must be coded as 0x00  */

/*  Bytes 155~191 (0x09B~0x0BF) (Load Reduced):Reserved, must be coded as 0x00  */

/*  Bytes 220~253 (0x0DC~0x0FD) (NVDIMM):Reserved, must be coded as 0x00  */

/*  Byte 382~383 (0x17E~0x17F): Reserved, must be coded as 0x00  */

/*  Module-Specific Section: Bytes 128~191 (0x080~0x0BF)  */

/*  Hybrid Memory Architecture Specific Parameters: Bytes 192~255 (0x0C0~0x0FF)  */

/*  Extended Function Parameter Block: Bytes 256~319 (0x100~0x13F)  */

#endif  // #ifndef _DDR4_SPD_REGS_H_
