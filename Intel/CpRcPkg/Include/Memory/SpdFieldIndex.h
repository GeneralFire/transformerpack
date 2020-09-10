/** @file
  SpdFieldIndex.h

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

#ifndef  _SPD_FIELD_INDEX_H_
#define  _SPD_FIELD_INDEX_H_

//
// SPD field index definations
//
#define   SPD_INDEX_SPD_BYTES_USED                                                                                                                                   0        //ddr4  spd bytes used 
#define   SPD_INDEX_SPD_BYTES_TOTAL                                                                                                                                  1        //ddr4&ddr5  spd bytes total 
#define   SPD_INDEX_ADDITIONS_LEVEL                                                                                                                                  2        //ddr4&ddr5  additions level 
#define   SPD_INDEX_ENCODING_LEVEL                                                                                                                                   3        //ddr4&ddr5  encoding level 
#define   SPD_INDEX_SDRAM_MODULE_TYPE                                                                                                                                4        //ddr4&ddr5  sdram module type 
#define   SPD_INDEX_BASE_MODULE_TYPE                                                                                                                                 5        //ddr4&ddr5  base module type 
#define   SPD_INDEX_HYBRID_MEDIA                                                                                                                                     6        //ddr4&ddr5  hybrid media 
#define   SPD_INDEX_HYBRID                                                                                                                                           7        //ddr4&ddr5  hybrid 
#define   SPD_INDEX_TOTAL_SDRAM_CAPACITY_PER_DIE_IN_MEGABITS                                                                                                         8        //ddr4  total sdram capacity per die in megabits 
#define   SPD_INDEX_BANK_ADDRESS_BITS                                                                                                                                9        //ddr4  bank address bits 
#define   SPD_INDEX_BANK_GROUP_BITS                                                                                                                                  10        //ddr4  bank group bits 
#define   SPD_INDEX_COLUMN_ADDRESS_BITS                                                                                                                              11        //ddr4  column address bits 
#define   SPD_INDEX_ROW_ADDRESS_BITS                                                                                                                                 12        //ddr4  row address bits 
#define   SPD_INDEX_SIGNAL_LOADING_PRIMARY_SDRAM_PACKAGE_TYPE                                                                                                        13        //ddr4  signal loading 
#define   SPD_INDEX_DIE_COUNT_PRIMARY_SDRAM_PACKAGE_TYPE                                                                                                             14        //ddr4  die count 
#define   SPD_INDEX_PRIMARY_SDRAM_PACKAGE_TYPE                                                                                                                       15        //ddr4  primary sdram package type 
#define   SPD_INDEX_MAXIMUM_ACTIVATE_COUNT_MAC                                                                                                                       16        //ddr4  maximum activate count mac 
#define   SPD_INDEX_MAXIMUM_ACTIVATE_WINDOW_TMAW                                                                                                                     17        //ddr4  maximum activate window tmaw 
#define   SPD_INDEX_SOFT_PPR                                                                                                                                         18        //ddr4&ddr5  soft ppr 
#define   SPD_INDEX_POST_PACKAGE_REPAIR_PPR                                                                                                                          19        //ddr4&ddr5  post package repair ppr 
#define   SPD_INDEX_SIGNAL_LOADING_SECONDARY_SDRAM_PACKAGE_TYPE                                                                                                      20        //ddr4  signal loading 
#define   SPD_INDEX_DRAM_DENSITY_RATIO                                                                                                                               21        //ddr4  dram density ratio 
#define   SPD_INDEX_DIE_COUNT_SECONDARY_SDRAM_PACKAGE_TYPE                                                                                                           22        //ddr4  die count 
#define   SPD_INDEX_SDRAM_PACKAGE_TYPE                                                                                                                               23        //ddr4  sdram package type 
#define   SPD_INDEX_DRAM_VDD_1_2_V                                                                                                                                   24        //ddr4  dram vdd 1 2 v 
#define   SPD_INDEX_SDRAM_DEVICE_WIDTH                                                                                                                               25        //ddr4  sdram device width 
#define   SPD_INDEX_NUMBER_OF_PACKAGE_RANKS_PER_DIMM                                                                                                                 26        //ddr4  number of package ranks per dimm 
#define   SPD_INDEX_RANK_MIX                                                                                                                                         27        //ddr4&ddr5  rank mix 
#define   SPD_INDEX_PRIMARY_BUS_WIDTH_IN_BITS                                                                                                                        28        //ddr4  primary bus width in bits 
#define   SPD_INDEX_BUS_WIDTH_EXTENSION_IN_BITS                                                                                                                      29        //ddr4  bus width extension in bits 
#define   SPD_INDEX_THERMAL_SENSOR                                                                                                                                   30        //ddr4  thermal sensor 
#define   SPD_INDEX_EXTENDED_BASE_MODULE_TYPE                                                                                                                        31        //ddr4  extended base module type 
#define   SPD_INDEX_FINE_TIMEBASE_FTB                                                                                                                                32        //ddr4  fine timebase ftb 
#define   SPD_INDEX_MEDIUM_TIMEBASE_MTB                                                                                                                              33        //ddr4  medium timebase mtb 
#define   SPD_INDEX_MINIMUM_SDRAM_CYCLE_TIME_TCKAVGMIN_MTB_UNITS                                                                                                     34        //ddr4  minimum sdram cycle time tckavgmin mtb units 
#define   SPD_INDEX_MINIMUM_SDRAM_CYCLE_TIME_TCKAVGMAX_MTB_UNITS                                                                                                     35        //ddr4  minimum sdram cycle time tckavgmax mtb units 
#define   SPD_INDEX_CL_FIRST_BYTE                                                                                                                                    36        //ddr4&ddr5  cl first byte 
#define   SPD_INDEX_CL_SECOND_BYTE                                                                                                                                   37        //ddr4&ddr5  cl second byte 
#define   SPD_INDEX_CL_THIRD_BYTE                                                                                                                                    38        //ddr4&ddr5  cl third byte 
#define   SPD_INDEX_CL_RANGE                                                                                                                                         39        //ddr4  cl range 
#define   SPD_INDEX_CL_FOURTH_BYTE                                                                                                                                   40        //ddr4&ddr5  cl fourth byte 
#define   SPD_INDEX_MINIMUM_SDRAM_CAS_LATENCY_TIME_TAAMIN_MTB_UNITS                                                                                                  41        //ddr4  minimum sdram cas latency time taamin mtb units 
#define   SPD_INDEX_MIN_SDRAM_RAS_TO_CAS_DELAY_TIME_TRCD                                                                                                             42        //ddr4  min sdram ras to cas delay time trcd 
#define   SPD_INDEX_MINIMUM_ROW_PRECHARGE_TIME_TRPMIN_MTB_UNITS                                                                                                      43        //ddr4  minimum row precharge time trpmin mtb units 
#define   SPD_INDEX_TRASMIN_MOST_SIGNIFICANT_NIBBLE                                                                                                                  44        //ddr4  trasmin most significant nibble 
#define   SPD_INDEX_TRCMIN_MOST_SIGNIFICANT_NIBBLE                                                                                                                   45        //ddr4  trcmin most significant nibble 
#define   SPD_INDEX_MINIMUM_ACTIVE_TO_PRECHARGE_TIME_TRASMIN_MTB_UNITS                                                                                               46        //ddr4  minimum active to precharge time trasmin mtb units 
#define   SPD_INDEX_MIN_ACTIVE_TO_ACTIVE_REFRESH_TIME_TRC                                                                                                            47        //ddr4  min active to active refresh time trc 
#define   SPD_INDEX_MIN_SDRAM_REFRESH_RECOVERY_DELAY_TRFC1_LSB                                                                                                       48        //ddr4  min sdram refresh recovery delay trfc1 
#define   SPD_INDEX_MIN_SDRAM_REFRESH_RECOVERY_DELAY_TRFC1_MSB                                                                                                       49        //ddr4  min sdram refresh recovery delay trfc1 
#define   SPD_INDEX_MIN_SDRAM_REFRESH_RECOVERY_DELAY_TRFC2_LSB                                                                                                       50        //ddr4  min sdram refresh recovery delay trfc2 
#define   SPD_INDEX_MIN_SDRAM_REFRESH_RECOVERY_DELAY_TRFC2_MSB                                                                                                       51        //ddr4  min sdram refresh recovery delay trfc2 
#define   SPD_INDEX_MIN_SDRAM_REFRESH_RECOVERY_DELAY_TRFC4_LSB                                                                                                       52        //ddr4  min sdram refresh recovery delay trfc4 
#define   SPD_INDEX_MIN_SDRAM_REFRESH_RECOVERY_DELAY_TRFC4_MSB                                                                                                       53        //ddr4  min sdram refresh recovery delay trfc4 
#define   SPD_INDEX_TFAW_MOST_SIGNIFICANT_NIBBLE                                                                                                                     54        //ddr4  tfaw most significant nibble 
#define   SPD_INDEX_MIN_FOUR_ACTIVATE_WINDOW_DELAY_TFAW                                                                                                              55        //ddr4  min four activate window delay tfaw 
#define   SPD_INDEX_MIN_ACTIVE_TO_ACTIVE_REFRESH_TIME_TRRDS                                                                                                          56        //ddr4  min active to active refresh time trrds 
#define   SPD_INDEX_MIN_ACTIVE_TO_ACTIVE_REFRESH_TIME_TRRDL                                                                                                          57        //ddr4  min active to active refresh time trrdl 
#define   SPD_INDEX_MINIMUM_CAS_TO_CAS_TIME_TCCD_LMIN_MTB_UNITS                                                                                                      58        //ddr4  minimum cas to cas time tccd lmin mtb units 
#define   SPD_INDEX_TWRMIN_MOST_SIGNIFICANT_NIBBLE                                                                                                                   59        //ddr4  twrmin most significant nibble 
#define   SPD_INDEX_MINIMUM_WRITE_RECOVERY_TIME_TWRMIN_MTB_UNITS                                                                                                     60        //ddr4  minimum write recovery time twrmin mtb units 
#define   SPD_INDEX_TWTR_SMIN_MOST_SIGNIFICANT_NIBBLE                                                                                                                61        //ddr4  twtr smin most significant nibble 
#define   SPD_INDEX_TWTR_LMIN_MOST_SIGNIFICANT_NIBBLE                                                                                                                62        //ddr4  twtr lmin most significant nibble 
#define   SPD_INDEX_MIN_WRITE_RECOVERY_TIME_TWTRS_DIFF_BANKS                                                                                                         63        //ddr4  min write recovery time twtrs diff banks 
#define   SPD_INDEX_MIN_WRITE_RECOVERY_TIME_TWTRL_SAME_BANKS                                                                                                         64        //ddr4  min write recovery time twtrl same banks 
#define   SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ0_3                                                                                                                         65        //ddr4  bit order at sdram 
#define   SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ0_3                                                                                                                66        //ddr4  wired to upper lower nibble 
#define   SPD_INDEX_PACKAGE_RANK_MAP_DQ0_3                                                                                                                           67        //ddr4  package rank map 
#define   SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ4_7                                                                                                                         68        //ddr4  bit order at sdram 
#define   SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ4_7                                                                                                                69        //ddr4  wired to upper lower nibble 
#define   SPD_INDEX_PACKAGE_RANK_MAP_DQ4_7                                                                                                                           70        //ddr4  package rank map 
#define   SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ8_11                                                                                                                        71        //ddr4  bit order at sdram 
#define   SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ8_11                                                                                                               72        //ddr4  wired to upper lower nibble 
#define   SPD_INDEX_PACKAGE_RANK_MAP_DQ8_11                                                                                                                          73        //ddr4  package rank map 
#define   SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ12_15                                                                                                                       74        //ddr4  bit order at sdram 
#define   SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ12_15                                                                                                              75        //ddr4  wired to upper lower nibble 
#define   SPD_INDEX_PACKAGE_RANK_MAP_DQ12_15                                                                                                                         76        //ddr4  package rank map 
#define   SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ16_19                                                                                                                       77        //ddr4  bit order at sdram 
#define   SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ16_19                                                                                                              78        //ddr4  wired to upper lower nibble 
#define   SPD_INDEX_PACKAGE_RANK_MAP_DQ16_19                                                                                                                         79        //ddr4  package rank map 
#define   SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ20_23                                                                                                                       80        //ddr4  bit order at sdram 
#define   SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ20_23                                                                                                              81        //ddr4  wired to upper lower nibble 
#define   SPD_INDEX_PACKAGE_RANK_MAP_DQ20_23                                                                                                                         82        //ddr4  package rank map 
#define   SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ24_27                                                                                                                       83        //ddr4  bit order at sdram 
#define   SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ24_27                                                                                                              84        //ddr4  wired to upper lower nibble 
#define   SPD_INDEX_PACKAGE_RANK_MAP_DQ24_27                                                                                                                         85        //ddr4  package rank map 
#define   SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ28_31                                                                                                                       86        //ddr4  bit order at sdram 
#define   SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ28_31                                                                                                              87        //ddr4  wired to upper lower nibble 
#define   SPD_INDEX_PACKAGE_RANK_MAP_DQ28_31                                                                                                                         88        //ddr4  package rank map 
#define   SPD_INDEX_BIT_ORDER_AT_SDRAM_CB0_3                                                                                                                         89        //ddr4  bit order at sdram 
#define   SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_CB0_3                                                                                                                90        //ddr4  wired to upper lower nibble 
#define   SPD_INDEX_PACKAGE_RANK_MAP_CB0_3                                                                                                                           91        //ddr4  package rank map 
#define   SPD_INDEX_BIT_ORDER_AT_SDRAM_CB4_7                                                                                                                         92        //ddr4  bit order at sdram 
#define   SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_CB4_7                                                                                                                93        //ddr4  wired to upper lower nibble 
#define   SPD_INDEX_PACKAGE_RANK_MAP_CB4_7                                                                                                                           94        //ddr4  package rank map 
#define   SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ32_35                                                                                                                       95        //ddr4  bit order at sdram 
#define   SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ32_35                                                                                                              96        //ddr4  wired to upper lower nibble 
#define   SPD_INDEX_PACKAGE_RANK_MAP_DQ32_35                                                                                                                         97        //ddr4  package rank map 
#define   SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ36_39                                                                                                                       98        //ddr4  bit order at sdram 
#define   SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ36_39                                                                                                              99        //ddr4  wired to upper lower nibble 
#define   SPD_INDEX_PACKAGE_RANK_MAP_DQ36_39                                                                                                                         100        //ddr4  package rank map 
#define   SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ40_43                                                                                                                       101        //ddr4  bit order at sdram 
#define   SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ40_43                                                                                                              102        //ddr4  wired to upper lower nibble 
#define   SPD_INDEX_PACKAGE_RANK_MAP_DQ40_43                                                                                                                         103        //ddr4  package rank map 
#define   SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ44_47                                                                                                                       104        //ddr4  bit order at sdram 
#define   SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ44_47                                                                                                              105        //ddr4  wired to upper lower nibble 
#define   SPD_INDEX_PACKAGE_RANK_MAP_DQ44_47                                                                                                                         106        //ddr4  package rank map 
#define   SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ48_51                                                                                                                       107        //ddr4  bit order at sdram 
#define   SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ48_51                                                                                                              108        //ddr4  wired to upper lower nibble 
#define   SPD_INDEX_PACKAGE_RANK_MAP_DQ48_51                                                                                                                         109        //ddr4  package rank map 
#define   SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ52_55                                                                                                                       110        //ddr4  bit order at sdram 
#define   SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ52_55                                                                                                              111        //ddr4  wired to upper lower nibble 
#define   SPD_INDEX_PACKAGE_RANK_MAP_DQ52_55                                                                                                                         112        //ddr4  package rank map 
#define   SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ56_59                                                                                                                       113        //ddr4  bit order at sdram 
#define   SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ56_59                                                                                                              114        //ddr4  wired to upper lower nibble 
#define   SPD_INDEX_PACKAGE_RANK_MAP_DQ56_59                                                                                                                         115        //ddr4  package rank map 
#define   SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ60_63                                                                                                                       116        //ddr4  bit order at sdram 
#define   SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ60_63                                                                                                              117        //ddr4  wired to upper lower nibble 
#define   SPD_INDEX_PACKAGE_RANK_MAP_DQ60_63                                                                                                                         118        //ddr4  package rank map 
#define   SPD_INDEX_SPD_FINE_OFFSET_FOR_MIN_CAS_TO_CAS_DELAY_TCCDL_SAME_BANKS_REG                                                                                    119        //ddr4  spd fine offset for min cas to cas delay tccdl same banks reg 
#define   SPD_INDEX_SPD_FINE_OFFSET_FOR_MIN_ACTIVATE_TO_ACTIVATE_DELAY_TRRDL_SAME_BANKS_REG                                                                          120        //ddr4  spd fine offset for min activate to activate delay trrdl same banks reg 
#define   SPD_INDEX_SPD_FINE_OFFSET_FOR_MIN_ACTIVATE_TO_ACTIVATE_DELAY_TRRDS_DIFF_BANKS_REG                                                                          121        //ddr4  spd fine offset for min activate to activate delay trrds diff banks reg 
#define   SPD_INDEX_SPD_FINE_OFFSET_FOR_MIN_ACTIVE_TO_ACTIVE_REFRESH_DELAY_TRC_REG                                                                                   122        //ddr4  spd fine offset for min active to active refresh delay trc reg 
#define   SPD_INDEX_SPD_FINE_OFFSET_FOR_MIN_ROW_PRECHARGE_DELAY_TRP_REG                                                                                              123        //ddr4  spd fine offset for min row precharge delay trp reg 
#define   SPD_INDEX_SPD_FINE_OFFSET_FOR_MIN_RAS_TO_CAS_DELAY_TIME_TRCDMIN_REG                                                                                        124        //ddr4  spd fine offset for min ras to cas delay time trcdmin reg 
#define   SPD_INDEX_SPD_FINE_OFFSET_FOR_MINIMUM_CAS_LATENCY_TIME_TAAMIN_REG                                                                                          125        //ddr4  spd fine offset for minimum cas latency time taamin reg 
#define   SPD_INDEX_SPD_FINE_OFFSET_FOR_SDRAM_MAXIMUM_CYCLE_TIME_TCKAVGMAX_REG                                                                                       126        //ddr4  spd fine offset for sdram maximum cycle time tckavgmax reg 
#define   SPD_INDEX_SPD_FINE_OFFSET_FOR_SDRAM_MINIMUM_CYCLE_TIME_TCKAVGMIN_REG                                                                                       127        //ddr4  spd fine offset for sdram minimum cycle time tckavgmin reg 
#define   SPD_INDEX_SPD_CRC_FOR_BASE_CONFIGURATION_SECTION_LSB_REG                                                                                                   128        //ddr4&ddr5  spd crc for base configuration section lsb reg 
#define   SPD_INDEX_SPD_CRC_FOR_BASE_CONFIGURATIONSECTION_MSB_REG                                                                                                    129        //ddr4  spd crc for base configurationsection msb reg 
#define   SPD_INDEX_MODULE_NOMINAL_HEIGHT_MAX_UDIMM                                                                                                                  130        //ddr4  module nominal height max 
#define   SPD_INDEX_RAW_CARD_EXTENSION_UDIMM                                                                                                                         131        //ddr4  raw card extension 
#define   SPD_INDEX_MODULE_NOMINAL_HEIGHT_MAX_RDIMM                                                                                                                  132        //ddr4  module nominal height max 
#define   SPD_INDEX_RAW_CARD_EXTENSION_RDIMM                                                                                                                         133        //ddr4  raw card extension 
#define   SPD_INDEX_MODULE_NOMINAL_HEIGHT_MAX_LRDIMM                                                                                                                 134        //ddr4  module nominal height max 
#define   SPD_INDEX_RAW_CARD_EXTENSION_LRDIMM                                                                                                                        135        //ddr4  raw card extension 
#define   SPD_INDEX_MODULE_MAXIMUM_THICKNESS_MAX_FRONT_UDIMM                                                                                                         136        //ddr4  module maximum thickness max front 
#define   SPD_INDEX_MODULE_MAXIMUM_THICKNESS_MAX_BACK_UDIMM                                                                                                          137        //ddr4  module maximum thickness max back 
#define   SPD_INDEX_MODULE_MAXIMUM_THICKNESS_MAX_FRONT_RDIMM                                                                                                         138        //ddr4  module maximum thickness max front 
#define   SPD_INDEX_MODULE_MAXIMUM_THICKNESS_MAX_BACK_RDIMM                                                                                                          139        //ddr4  module maximum thickness max back 
#define   SPD_INDEX_MODULE_MAXIMUM_THICKNESS_MAX_FRONT_LRDIMM                                                                                                        140        //ddr4  module maximum thickness max front 
#define   SPD_INDEX_MODULE_MAXIMUM_THICKNESS_MAX_BACK_LRDIMM                                                                                                         141        //ddr4  module maximum thickness max back 
#define   SPD_INDEX_REFERENCE_RAW_CARD_UDIMM                                                                                                                         142        //ddr4  reference raw card 
#define   SPD_INDEX_REFERENCE_RAW_CARD_REVISION_UDIMM                                                                                                                143        //ddr4  reference raw card revision 
#define   SPD_INDEX_REFERENCE_RAW_CARD_EXTENSION_UDIMM                                                                                                               144        //ddr4  reference raw card extension 
#define   SPD_INDEX_REFERENCE_RAW_CARD_RDIMM                                                                                                                         145        //ddr4  reference raw card 
#define   SPD_INDEX_REFERENCE_RAW_CARD_REVISION_RDIMM                                                                                                                146        //ddr4  reference raw card revision 
#define   SPD_INDEX_REFERENCE_RAW_CARD_EXTENSION_RDIMM                                                                                                               147        //ddr4  reference raw card extension 
#define   SPD_INDEX_REFERENCE_RAW_CARD_LRDIMM                                                                                                                        148        //ddr4  reference raw card 
#define   SPD_INDEX_REFERENCE_RAW_CARD_REVISION_LRDIMM                                                                                                               149        //ddr4  reference raw card revision 
#define   SPD_INDEX_REFERENCE_RAW_CARD_EXTENSION_LRDIMM                                                                                                              150        //ddr4  reference raw card extension 
#define   SPD_INDEX_RANK_1_MAPPING_UDIMM_ADDRESS_MAPPING_FROM_EDGE_CONNECTOR_TO_DRAM                                                                                 151        //ddr4  rank 1 mapping 
#define   SPD_INDEX_OF_REGISTERS_USED_ON_RDIMM                                                                                                                       152        //ddr4  of registers used on rdimm 
#define   SPD_INDEX_OF_ROWS_OF_DRAMS_ON_RDIMM                                                                                                                        153        //ddr4  of rows of drams on rdimm 
#define   SPD_INDEX_REGISTER_TYPE                                                                                                                                    154        //ddr4  register type 
#define   SPD_INDEX_OF_REGISTERS_USED_ON_LRDIMM                                                                                                                      155        //ddr4  of registers used on lrdimm 
#define   SPD_INDEX_OF_ROWS_OF_DRAMS_ON_LRDIMM                                                                                                                       156        //ddr4  of rows of drams on lrdimm 
#define   SPD_INDEX_REGISTER_AND_DATA_BUFFER_TYPES                                                                                                                   157        //ddr4  register and data buffer types 
#define   SPD_INDEX_HEAT_SPREADER_THERMAL_CHARACTERISTICS_RDIMM                                                                                                      158        //ddr4  heat spreader thermal characteristics 
#define   SPD_INDEX_HEAT_SPREADER_SOLUTION                                                                                                                           159        //ddr4  heat spreader solution 
#define   SPD_INDEX_HEAT_SPREADER_THERMAL_CHARACTERISTICS_LRDIMM                                                                                                     160        //ddr4  heat spreader thermal characteristics 
#define   SPD_INDEX_HEAT_SPREADER                                                                                                                                    161        //ddr4&ddr5  heat spreader 
#define   SPD_INDEX_NUMBER_OF_CONTINUATION_CODES_REGISTER_MANUFACTURER                                                                                               162        //ddr4  number of continuation codes register manufacturer 
#define   SPD_INDEX_ODD_PARITY_FOR_BYTE_133_BITS_6_0_RDIMM                                                                                                           163        //ddr4  odd parity for byte 133 bits 6 0 
#define   SPD_INDEX_CONTINUATION_CODES_NUM_MEMORY_BUFFER_MANUFACTURER                                                                                                164        //ddr4  continuation codes num memory buffer manufacturer 
#define   SPD_INDEX_ODD_PARITY_FOR_BYTE_133_BITS_6_0_LRDIMM                                                                                                          165        //ddr4  odd parity for byte 133 bits 6 0 
#define   SPD_INDEX_LAST_NON_ZERO_BYTE_REGISTER_MANUFACTURER                                                                                                         166        //ddr4  last non zero byte register manufacturer 
#define   SPD_INDEX_LAST_NON_ZERO_BYTE_MEMORY_BUFFER_MANUFACTURER                                                                                                    167        //ddr4  last non zero byte memory buffer manufacturer 
#define   SPD_INDEX_REGISTER_REVISION_NUMBER_RDIMM                                                                                                                   168        //ddr4  register revision number 
#define   SPD_INDEX_REGISTER_REVISION_NUMBER_LRDIMM                                                                                                                  169        //ddr4  register revision number 
#define   SPD_INDEX_RANK_1_MAPPING_RDIMM_ADDRESS_MAPPING_FROMISTER_TO_DRAM                                                                                           170        //ddr4  rank 1 mapping 
#define   SPD_INDEX_ODD_RANK_MAPPING                                                                                                                                 171        //ddr4  odd rank mapping 
#define   SPD_INDEX_CKE_RDIMM                                                                                                                                        172        //ddr4  cke 
#define   SPD_INDEX_ODT_RDIMM                                                                                                                                        173        //ddr4  odt 
#define   SPD_INDEX_COMMAND_ADDRESS_RDIMM                                                                                                                            174        //ddr4  command address 
#define   SPD_INDEX_CHIP_SELECT_RDIMM                                                                                                                                175        //ddr4  chip select 
#define   SPD_INDEX_CKE_LRDIMM                                                                                                                                       176        //ddr4  cke 
#define   SPD_INDEX_ODT_LRDIMM                                                                                                                                       177        //ddr4  odt 
#define   SPD_INDEX_COMMAND_ADDRESS_LRDIMM                                                                                                                           178        //ddr4  command address 
#define   SPD_INDEX_CHIP_SELECT_LRDIMM                                                                                                                               179        //ddr4  chip select 
#define   SPD_INDEX_Y0_Y2_RDIMM                                                                                                                                      180        //ddr4  y0 y2 
#define   SPD_INDEX_Y1_Y3_RDIMM                                                                                                                                      181        //ddr4  y1 y3 
#define   SPD_INDEX_Y0_Y2_LRDIMM                                                                                                                                     182        //ddr4  y0 y2 
#define   SPD_INDEX_Y1_Y3_LRDIMM                                                                                                                                     183        //ddr4  y1 y3 
#define   SPD_INDEX_BCOM_BODT_BCKE                                                                                                                                   184        //ddr4  bcom bodt bcke 
#define   SPD_INDEX_BCK                                                                                                                                              185        //ddr4  bck 
#define   SPD_INDEX_DATA_BUFFER_REVISION_NUMBER                                                                                                                      186        //ddr4  data buffer revision number 
#define   SPD_INDEX_DRAM_VREFDQ_FOR_PACKAGE_RANK_0_DRAMS                                                                                                             187        //ddr4  dram vrefdq for package rank 0 drams 
#define   SPD_INDEX_DRAM_VREFDQ_FOR_PACKAGE_RANK_1_DRAMS                                                                                                             188        //ddr4  dram vrefdq for package rank 1 drams 
#define   SPD_INDEX_DRAM_VREFDQ_FOR_PACKAGE_RANK_2_DRAMS                                                                                                             189        //ddr4  dram vrefdq for package rank 2 drams 
#define   SPD_INDEX_DRAM_VREFDQ_FOR_PACKAGE_RANK_3_DRAMS                                                                                                             190        //ddr4  dram vrefdq for package rank 3 drams 
#define   SPD_INDEX_DATA_BUFFER_VREFDQ_FOR_DRAM_INTERFACE                                                                                                            191        //ddr4  data buffer vrefdq for dram interface 
#define   SPD_INDEX_DRAM_INTERFACE_MDQ_READ_TERMINATION_STRENGTH_1866                                                                                                192        //ddr4  dram interface mdq read termination strength 
#define   SPD_INDEX_DRAM_INTERFACE_MDQ_DRIVE_STRENGTH_1866                                                                                                           193        //ddr4  dram interface mdq drive strength 
#define   SPD_INDEX_DRAM_INTERFACE_MDQ_READ_TERMINATION_STRENGTH_1866_2400                                                                                           194        //ddr4  dram interface mdq read termination strength 
#define   SPD_INDEX_DRAM_INTERFACE_MDQ_DRIVE_STRENGTH_1866_2400                                                                                                      195        //ddr4  dram interface mdq drive strength 
#define   SPD_INDEX_DRAM_INTERFACE_MDQ_READ_TERMINATION_STRENGTH_2400_3200                                                                                           196        //ddr4  dram interface mdq read termination strength 
#define   SPD_INDEX_DRAM_INTERFACE_MDQ_DRIVE_STRENGTH_2400_3200                                                                                                      197        //ddr4  dram interface mdq drive strength 
#define   SPD_INDEX_DRAM_DRIVE_0_1                                                                                                                                   198        //ddr4  dram drive 0 1 
#define   SPD_INDEX_DRAM_DRIVE_2_3                                                                                                                                   199        //ddr4  dram drive 2 3 
#define   SPD_INDEX_DRAM_DRIVE_4_5                                                                                                                                   200        //ddr4  dram drive 4 5 
#define   SPD_INDEX_RTT_NOM_1866                                                                                                                                     201        //ddr4  rtt nom 
#define   SPD_INDEX_RTT_WR_1866                                                                                                                                      202        //ddr4  rtt wr 
#define   SPD_INDEX_RTT_NOM_1866_2400                                                                                                                                203        //ddr4  rtt nom 
#define   SPD_INDEX_RTT_WR_1866_2400                                                                                                                                 204        //ddr4  rtt wr 
#define   SPD_INDEX_RTT_NOM_2400_3200                                                                                                                                205        //ddr4  rtt nom 
#define   SPD_INDEX_RTT_WR_2400_3200                                                                                                                                 206        //ddr4  rtt wr 
#define   SPD_INDEX_RTT_PARK_PACKAGE_RANKS_0_1_1866                                                                                                                  207        //ddr4  rtt park package ranks 0 1 
#define   SPD_INDEX_RTT_PARK_PACKAGE_RANKS_2_3_1866                                                                                                                  208        //ddr4  rtt park package ranks 2 3 
#define   SPD_INDEX_RTT_PARK_PACKAGE_RANKS_0_1_1866_2400                                                                                                             209        //ddr4  rtt park package ranks 0 1 
#define   SPD_INDEX_RTT_PARK_PACKAGE_RANKS_2_3_1866_2400                                                                                                             210        //ddr4  rtt park package ranks 2 3 
#define   SPD_INDEX_RTT_PARK_PACKAGE_RANKS_0_1_2400_3200                                                                                                             211        //ddr4  rtt park package ranks 0 1 
#define   SPD_INDEX_RTT_PARK_PACKAGE_RANKS_2_3_2400_3200                                                                                                             212        //ddr4  rtt park package ranks 2 3 
#define   SPD_INDEX_SPD_NVDIMM_MODULE_PRODUCT_IDENTIFIER_LSB_REG                                                                                                     213        //ddr4  spd nvdimm module product identifier lsb reg 
#define   SPD_INDEX_SPD_NVDIMM_MODULE_PRODUCT_IDENTIFIER_MSB_REG                                                                                                     214        //ddr4  spd nvdimm module product identifier msb reg 
#define   SPD_INDEX_NUMBER_OF_CONTINUATION_CODES_FLASH_MANUFACTURER                                                                                                  215        //ddr4  number of continuation codes flash manufacturer 
#define   SPD_INDEX_ODD_PARITY_FOR_BYTE_194_BITS_6_0                                                                                                                 216        //ddr4  odd parity for byte 194 bits 6 0 
#define   SPD_INDEX_LAST_NON_ZERO_BYTE_FLASH_MANUFACTURER                                                                                                            217        //ddr4  last non zero byte flash manufacturer 
#define   SPD_INDEX_SPD_NVDIMM_SUBSYSTEM_CONTROLLER_IDENTIFIER_LSB_REG                                                                                               218        //ddr4  spd nvdimm subsystem controller identifier lsb reg 
#define   SPD_INDEX_SPD_NVDIMM_SUBSYSTEM_CONTROLLER_IDENTIFIER_MSB_REG                                                                                               219        //ddr4  spd nvdimm subsystem controller identifier msb reg 
#define   SPD_INDEX_SPD_NVDIMM_SUBSYSTEM_CONTROLLER_REVISION_CODE_REG                                                                                                220        //ddr4  spd nvdimm subsystem controller revision code reg 
#define   SPD_INDEX_REFERENCE_RAW_CARD                                                                                                                               221        //ddr4  reference raw card 
#define   SPD_INDEX_REFERENCE_RAW_CARD_REVISION                                                                                                                      222        //ddr4  reference raw card revision 
#define   SPD_INDEX_REFERENCE_RAW_CARD_EXTENSION                                                                                                                     223        //ddr4  reference raw card extension 
#define   SPD_INDEX_RAW_CARD_EXTENSION                                                                                                                               224        //ddr4  raw card extension 
#define   SPD_INDEX_NAND_FLASH                                                                                                                                       225        //ddr4  nand flash 
#define   SPD_INDEX_SDRAM                                                                                                                                            226        //ddr4  sdram 
#define   SPD_INDEX_UNKNOWN_UNDEFINED                                                                                                                                227        //ddr4  unknown undefined 
#define   SPD_INDEX_MIN_NON_VOLATILE_MEMORY_INITIALIZATION_TIME_SECONDS                                                                                              228        //ddr4  min non volatile memory initialization time seconds 
#define   SPD_INDEX_FUNCTION_INTERFACE_FUNCTION_0                                                                                                                    229        //ddr4  function interface 
#define   SPD_INDEX_FUNCTION_CLASS_LSB_FUNCTION_0                                                                                                                    230        //ddr4  function class lsb 
#define   SPD_INDEX_FUNCTION_CLASS_MSB_FUNCTION_0                                                                                                                    231        //ddr4  function class msb 
#define   SPD_INDEX_BLOCK_OFFSET_FUNCTION_0                                                                                                                          232        //ddr4  block offset 
#define   SPD_INDEX_IMPLEMENTED_FUNCTION_0                                                                                                                           233        //ddr4  implemented 
#define   SPD_INDEX_FUNCTION_INTERFACE_FUNCTION_1                                                                                                                    234        //ddr4  function interface 
#define   SPD_INDEX_FUNCTION_CLASS_LSB_FUNCTION_1                                                                                                                    235        //ddr4  function class lsb 
#define   SPD_INDEX_FUNCTION_CLASS_MSB_FUNCTION_1                                                                                                                    236        //ddr4  function class msb 
#define   SPD_INDEX_BLOCK_OFFSET_FUNCTION_1                                                                                                                          237        //ddr4  block offset 
#define   SPD_INDEX_IMPLEMENTED_FUNCTION_1                                                                                                                           238        //ddr4  implemented 
#define   SPD_INDEX_FUNCTION_INTERFACE_FUNCTION_2                                                                                                                    239        //ddr4  function interface 
#define   SPD_INDEX_FUNCTION_CLASS_LSB_FUNCTION_2                                                                                                                    240        //ddr4  function class lsb 
#define   SPD_INDEX_FUNCTION_CLASS_MSB_FUNCTION_2                                                                                                                    241        //ddr4  function class msb 
#define   SPD_INDEX_BLOCK_OFFSET_FUNCTION_2                                                                                                                          242        //ddr4  block offset 
#define   SPD_INDEX_IMPLEMENTED_FUNCTION_2                                                                                                                           243        //ddr4  implemented 
#define   SPD_INDEX_FUNCTION_INTERFACE_FUNCTION_3                                                                                                                    244        //ddr4  function interface 
#define   SPD_INDEX_FUNCTION_CLASS_LSB_FUNCTION_3                                                                                                                    245        //ddr4  function class lsb 
#define   SPD_INDEX_FUNCTION_CLASS_MSB_FUNCTION_3                                                                                                                    246        //ddr4  function class msb 
#define   SPD_INDEX_BLOCK_OFFSET_FUNCTION_3                                                                                                                          247        //ddr4  block offset 
#define   SPD_INDEX_IMPLEMENTED_FUNCTION_3                                                                                                                           248        //ddr4  implemented 
#define   SPD_INDEX_FUNCTION_INTERFACE_FUNCTION_4                                                                                                                    249        //ddr4  function interface 
#define   SPD_INDEX_FUNCTION_CLASS_LSB_FUNCTION_4                                                                                                                    250        //ddr4  function class lsb 
#define   SPD_INDEX_FUNCTION_CLASS_MSB_FUNCTION_4                                                                                                                    251        //ddr4  function class msb 
#define   SPD_INDEX_BLOCK_OFFSET_FUNCTION_4                                                                                                                          252        //ddr4  block offset 
#define   SPD_INDEX_IMPLEMENTED_FUNCTION_4                                                                                                                           253        //ddr4  implemented 
#define   SPD_INDEX_FUNCTION_INTERFACE_FUNCTION_5                                                                                                                    254        //ddr4  function interface 
#define   SPD_INDEX_FUNCTION_CLASS_LSB_FUNCTION_5                                                                                                                    255        //ddr4  function class lsb 
#define   SPD_INDEX_FUNCTION_CLASS_MSB_FUNCTION_5                                                                                                                    256        //ddr4  function class msb 
#define   SPD_INDEX_BLOCK_OFFSET_FUNCTION_5                                                                                                                          257        //ddr4  block offset 
#define   SPD_INDEX_IMPLEMENTED_FUNCTION_5                                                                                                                           258        //ddr4  implemented 
#define   SPD_INDEX_FUNCTION_INTERFACE_FUNCTION_6                                                                                                                    259        //ddr4  function interface 
#define   SPD_INDEX_FUNCTION_CLASS_LSB_FUNCTION_6                                                                                                                    260        //ddr4  function class lsb 
#define   SPD_INDEX_FUNCTION_CLASS_MSB_FUNCTION_6                                                                                                                    261        //ddr4  function class msb 
#define   SPD_INDEX_BLOCK_OFFSET_FUNCTION_6                                                                                                                          262        //ddr4  block offset 
#define   SPD_INDEX_IMPLEMENTED_FUNCTION_6                                                                                                                           263        //ddr4  implemented 
#define   SPD_INDEX_FUNCTION_INTERFACE_FUNCTION_7                                                                                                                    264        //ddr4  function interface 
#define   SPD_INDEX_FUNCTION_CLASS_LSB_FUNCTION_7                                                                                                                    265        //ddr4  function class lsb 
#define   SPD_INDEX_FUNCTION_CLASS_MSB_FUNCTION_7                                                                                                                    266        //ddr4  function class msb 
#define   SPD_INDEX_BLOCK_OFFSET_FUNCTION_7                                                                                                                          267        //ddr4  block offset 
#define   SPD_INDEX_IMPLEMENTED_FUNCTION_7                                                                                                                           268        //ddr4  implemented 
#define   SPD_INDEX_SPD_UDIMM_CRC_FOR_SPD_BLOCK_1_LSB_REG                                                                                                            269        //ddr4  spd udimm crc for spd block 1 lsb reg 
#define   SPD_INDEX_SPD_RDIMM_CRC_FOR_SPD_BLOCK_1_LSB_REG                                                                                                            270        //ddr4  spd rdimm crc for spd block 1 lsb reg 
#define   SPD_INDEX_SPD_LRDIMM_CRC_FOR_SPD_BLOCK_1_LSB_REG                                                                                                           271        //ddr4  spd lrdimm crc for spd block 1 lsb reg 
#define   SPD_INDEX_SPD_NVDIMM_CRC_FOR_SPD_BLOCK_1_LSB_REG                                                                                                           272        //ddr4  spd nvdimm crc for spd block 1 lsb reg 
#define   SPD_INDEX_SPD_UDIMM_CRC_FOR_SPD_BLOCK_1_MSB_REG                                                                                                            273        //ddr4  spd udimm crc for spd block 1 msb reg 
#define   SPD_INDEX_SPD_RDIMM_CRC_FOR_SPD_BLOCK_1_MSB_REG                                                                                                            274        //ddr4  spd rdimm crc for spd block 1 msb reg 
#define   SPD_INDEX_SPD_LRDIMM_CRC_FOR_SPD_BLOCK_1_MSB_REG                                                                                                           275        //ddr4  spd lrdimm crc for spd block 1 msb reg 
#define   SPD_INDEX_SPD_NVDIMM_CRC_FOR_SPD_BLOCK_1_MSB_REG                                                                                                           276        //ddr4  spd nvdimm crc for spd block 1 msb reg 
#define   SPD_INDEX_NUMBER_OF_CONTINUATION_CODES_MODULE_MANUFACTURER                                                                                                 277        //ddr4&ddr5  number of continuation codes module manufacturer 
#define   SPD_INDEX_PD_INDEX_ODD_PARITY_BITS_6_0                                                                                                                     278        //ddr4&ddr5  pd index odd parity bits 6 0 
#define   SPD_INDEX_LAST_NON_ZERO_BYTE_MODULE_MANUFACTURER                                                                                                           279        //ddr4&ddr5  last non zero byte module manufacturer 
#define   SPD_INDEX_SPD_MODULE_MANUFACTURING_LOCATION_REG                                                                                                            280        //ddr4&ddr5  spd module manufacturing location reg 
#define   SPD_INDEX_SPD_MODULE_MANUFACTURING_DATE_REG                                                                                                                281        //ddr4&ddr5  spd module manufacturing date reg 
#define   SPD_INDEX_SPD_MODULE_SERIAL_NUMBER_REG                                                                                                                     282        //ddr4&ddr5  spd module serial number reg 
#define   SPD_INDEX_SPD_MODULE_PART_NUMBER_REG                                                                                                                       283        //ddr4&ddr5  spd module part number reg 
#define   SPD_INDEX_SPD_MODULE_REVISION_CODE_REG                                                                                                                     284        //ddr4&ddr5  spd module revision code reg 
#define   SPD_INDEX_NUMBER_OF_CONTINUATION_CODES_DRAM_MANUFACTURER                                                                                                   285        //ddr4&ddr5  number of continuation codes dram manufacturer 
#define   SPD_INDEX_ODD_PARITY_FOR_BYTE_350_BITS_6_0                                                                                                                 286        //ddr4  odd parity for byte 350 bits 6 0 
#define   SPD_INDEX_LAST_NON_ZERO_BYTE_DRAM_MANUFACTURER                                                                                                             287        //ddr4&ddr5  last non zero byte dram manufacturer 
#define   SPD_INDEX_DRAM_STEPPING                                                                                                                                    288        //ddr4&ddr5  dram stepping 
#define   SPD_INDEX_SPD_MANUFACTURER_SPECIFIC_DATA_REG                                                                                                               289        //ddr4&ddr5  spd manufacturer specific data reg 
#define   SPD_INDEX_SDRAM_DENSITY_PER_DIE_FIRST_SDRAM_DENSITY_AND_PACKAGE                                                                                            290        //ddr5  sdram density per die 
#define   SPD_INDEX_DIE_PER_PACKAGE_FIRST_SDRAM_DENSITY_AND_PACKAGE                                                                                                  291        //ddr5  die per package 
#define   SPD_INDEX_FIRST_SDRAM_ROW_ADDRESS_BITS                                                                                                                     292        //ddr5  first sdram row address bits 
#define   SPD_INDEX_FIRST_SDRAM_COLUMN_ADDRESS_BITS                                                                                                                  293        //ddr5  first sdram column address bits 
#define   SPD_INDEX_SDRAM_I_O_WIDTH_FIRST_SDRAM_I_O_WIDTH                                                                                                            294        //ddr5  sdram i o width 
#define   SPD_INDEX_FIRST_SDRAM_BANKS_PER_BANK_GROUP                                                                                                                 295        //ddr5  first sdram banks per bank group 
#define   SPD_INDEX_FIRST_SDRAM_BANK_GROUPS                                                                                                                          296        //ddr5  first sdram bank groups 
#define   SPD_INDEX_SDRAM_DENSITY_PER_DIE_SECOND_SDRAM_DENSITY_AND_PACKAGE                                                                                           297        //ddr5  sdram density per die 
#define   SPD_INDEX_DIE_PER_PACKAGE_SECOND_SDRAM_DENSITY_AND_PACKAGE                                                                                                 298        //ddr5  die per package 
#define   SPD_INDEX_SECOND_SDRAM_ROW_ADDRESS_BITS                                                                                                                    299        //ddr5  second sdram row address bits 
#define   SPD_INDEX_SECOND_SDRAM_COLUMN_ADDRESS_BITS                                                                                                                 300        //ddr5  second sdram column address bits 
#define   SPD_INDEX_SDRAM_I_O_WIDTH_SECONDARY_SDRAM_I_O_WIDTH                                                                                                        301        //ddr5  sdram i o width 
#define   SPD_INDEX_SECOND_SDRAM_BANKS_PER_BANK_GROUP                                                                                                                302        //ddr5  second sdram banks per bank group 
#define   SPD_INDEX_SECOND_SDRAM_BANK_GROUPS                                                                                                                         303        //ddr5  second sdram bank groups 
#define   SPD_INDEX_BL32                                                                                                                                             304        //ddr5  bl32 
#define   SPD_INDEX_ENDURANT_NOMINAL_VOLTAGE_VDD                                                                                                                     305        //ddr5  endurant 
#define   SPD_INDEX_OPERABLE_NOMINAL_VOLTAGE_VDD                                                                                                                     306        //ddr5  operable 
#define   SPD_INDEX_NOMINAL_NOMINAL_VOLTAGE_VDD                                                                                                                      307        //ddr5  nominal 
#define   SPD_INDEX_ENDURANT_NOMINAL_VOLTAGE_VDDQ                                                                                                                    308        //ddr5  endurant 
#define   SPD_INDEX_OPERABLE_NOMINAL_VOLTAGE_VDDQ                                                                                                                    309        //ddr5  operable 
#define   SPD_INDEX_NOMINAL_NOMINAL_VOLTAGE_VDDQ                                                                                                                     310        //ddr5  nominal 
#define   SPD_INDEX_ENDURANT_NOMINAL_VOLTAGE_VPP                                                                                                                     311        //ddr5  endurant 
#define   SPD_INDEX_OPERABLE_NOMINAL_VOLTAGE_VPP                                                                                                                     312        //ddr5  operable 
#define   SPD_INDEX_NOMINAL_NOMINAL_VOLTAGE_VPP                                                                                                                      313        //ddr5  nominal 
#define   SPD_INDEX_SDRAM_MINIMUM_CYCLE_TIME_TCKAVGMIN_PS_LSB                                                                                                        314        //ddr5  sdram minimum cycle time tckavgmin ps 
#define   SPD_INDEX_SDRAM_MINIMUM_CYCLE_TIME_TCKAVGMIN_PS_MSB                                                                                                        315        //ddr5  sdram minimum cycle time tckavgmin ps 
#define   SPD_INDEX_SDRAM_MAXIMUM_CYCLE_TIME_TCKAVGMAX_PS_LSB                                                                                                        316        //ddr5  sdram maximum cycle time tckavgmax ps 
#define   SPD_INDEX_SDRAM_MAXIMUM_CYCLE_TIME_TCKAVGMAX_PS_MSB                                                                                                        317        //ddr5  sdram maximum cycle time tckavgmax ps 
#define   SPD_INDEX_CL_FIFTH_BYTE                                                                                                                                    318        //ddr5  cl 
#define   SPD_INDEX_SDRAM_MINIMUM_CAS_LATENCY_TIME_TAAMIN_PS_LSB                                                                                                     319        //ddr5  sdram minimum cas latency time taamin ps 
#define   SPD_INDEX_SDRAM_MINIMUM_CAS_LATENCY_TIME_TAAMIN_PS_MSB                                                                                                     320        //ddr5  sdram minimum cas latency time taamin ps 
#define   SPD_INDEX_SDRAM_MINIMUM_RAS_TO_CAS_DELAY_TIME_TRCDMIN_PS_LSB                                                                                               321        //ddr5  sdram minimum ras to cas delay time trcdmin ps 
#define   SPD_INDEX_SDRAM_MINIMUM_RAS_TO_CAS_DELAY_TIME_TRCDMIN_PS_MSB                                                                                               322        //ddr5  sdram minimum ras to cas delay time trcdmin ps 
#define   SPD_INDEX_SDRAM_MINIMUM_ROW_PRECHARGE_DELAY_TIME_TRPMIN_PS_LSB                                                                                             323        //ddr5  sdram minimum row precharge delay time trpmin ps 
#define   SPD_INDEX_SDRAM_MINIMUM_ROW_PRECHARGE_DELAY_TIME_TRPMIN_PS_MSB                                                                                             324        //ddr5  sdram minimum row precharge delay time trpmin ps 
#define   SPD_INDEX_SDRAM_MIN_ACTIVE_TO_PRECHARGE_DELAY_TRAS_LSB                                                                                                     325        //ddr5  sdram min active to precharge delay tras 
#define   SPD_INDEX_SDRAM_MIN_ACTIVE_TO_PRECHARGE_DELAY_TRAS_MSB                                                                                                     326        //ddr5  sdram min active to precharge delay tras 
#define   SPD_INDEX_SDRAM_MIN_ACTIVE_TO_ACTIVE_REFRESH_DELAY_TRC_LSB                                                                                                 327        //ddr5  sdram min active to active refresh delay trc 
#define   SPD_INDEX_SDRAM_MIN_ACTIVE_TO_ACTIVE_REFRESH_DELAY_TRC_MSB                                                                                                 328        //ddr5  sdram min active to active refresh delay trc 
#define   SPD_INDEX_SDRAM_MINIMUM_WRITE_RECOVERY_TIME_TWRMIN_PS_LSB                                                                                                  329        //ddr5  sdram minimum write recovery time twrmin ps 
#define   SPD_INDEX_SDRAM_MINIMUM_WRITE_RECOVERY_TIME_TWRMIN_PS_MSB                                                                                                  330        //ddr5  sdram minimum write recovery time twrmin ps 
#define   SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC1_SLR_LSB                                                                                                   331        //ddr5  sdram min refresh recovery delay trfc1 slr 
#define   SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC1_SLR_MSB                                                                                                   332        //ddr5  sdram min refresh recovery delay trfc1 slr 
#define   SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC2_SLR_LSB                                                                                                   333        //ddr5  sdram min refresh recovery delay trfc2 slr 
#define   SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC2_SLR_MSB                                                                                                   334        //ddr5  sdram min refresh recovery delay trfc2 slr 
#define   SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFCSB_LSB                                                                                                      335        //ddr5  sdram min refresh recovery delay trfcsb 
#define   SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFCSB_MSB                                                                                                      336        //ddr5  sdram min refresh recovery delay trfcsb 
#define   SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC1_DLR_LSB                                                                                                   337        //ddr5  sdram min refresh recovery delay trfc1 dlr 
#define   SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC1_DLR_MSB                                                                                                   338        //ddr5  sdram min refresh recovery delay trfc1 dlr 
#define   SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC2_DLR_LSB                                                                                                   339        //ddr5  sdram min refresh recovery delay trfc2 dlr 
#define   SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC2_DLR_MSB                                                                                                   340        //ddr5  sdram min refresh recovery delay trfc2 dlr 
#define   SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFCSB_DLR_LSB                                                                                                  341        //ddr5  sdram min refresh recovery delay trfcsb dlr 
#define   SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFCSB_DLR_MSB                                                                                                  342        //ddr5  sdram min refresh recovery delay trfcsb dlr 
#define   SPD_INDEX_RFM_REQUIRED_PRIMARY_SDRAM_REFRESH_MANAGEMENT_0                                                                                                  343        //ddr5  rfm required 
#define   SPD_INDEX_ROLLING_ACCUMULATED_ACT_INITIAL_MANAGEMENT_THRESHOLD_RAAIMT_PRIMARY_SDRAM_REFRESH_MANAGEMENT_0                                                   344        //ddr5  rolling accumulated act initial management threshold raaimt 
#define   SPD_INDEX_ROLLING_ACCUMULATED_ACT_MAXIMUM_MANAGEMENT_THRESHOLD_RAAMMT_PRIMARY_SDRAM_REFRESH_MANAGEMENT_0                                                   345        //ddr5  rolling accumulated act maximum management threshold raammt 
#define   SPD_INDEX_RFM_RAA_COUNTER_DECREMENT_PER_REF_COMMAND_PRIMARY_SDRAM_REFRESH_MANAGEMENT_1                                                                     346        //ddr5  rfm raa counter decrement per ref command 
#define   SPD_INDEX_RFM_REQUIRED_SECONDARY_SDRAM_REFRESH_MANAGEMENT_0                                                                                                347        //ddr5  rfm required 
#define   SPD_INDEX_ROLLING_ACCUMULATED_ACT_INITIAL_MANAGEMENT_THRESHOLD_RAAIMT_SECONDARY_SDRAM_REFRESH_MANAGEMENT_0                                                 348        //ddr5  rolling accumulated act initial management threshold raaimt 
#define   SPD_INDEX_ROLLING_ACCUMULATED_ACT_MAXIMUM_MANAGEMENT_THRESHOLD_RAAMMT_SECONDARY_SDRAM_REFRESH_MANAGEMENT_0                                                 349        //ddr5  rolling accumulated act maximum management threshold raammt 
#define   SPD_INDEX_RFM_RAA_COUNTER_DECREMENT_PER_REF_COMMAND_SECONDARY_SDRAM_REFRESH_MANAGEMENT_1                                                                   350        //ddr5  rfm raa counter decrement per ref command 
#define   SPD_INDEX_SPD_CRC_FOR_BASE_CONFIGURATION_SECTION_MSB_REG                                                                                                   351        //ddr5  spd crc for base configuration section msb reg 
#define   SPD_INDEX_SPD_COMMON_SPD_REVISION_FOR_MODULE_INFORMATION_REG                                                                                               352        //ddr5  spd common spd revision for module information reg 
#define   SPD_INDEX_SPD_COMMON_SPD_MANUFACTURER_ID_CODE_0_REG                                                                                                        353        //ddr5  spd common spd manufacturer id code 0 reg 
#define   SPD_INDEX_SPD_COMMON_SPD_MANUFACTURER_ID_CODE_1_REG                                                                                                        354        //ddr5  spd common spd manufacturer id code 1 reg 
#define   SPD_INDEX_DEVICE_TYPE_COMMON_DEVICE_TYPE                                                                                                                   355        //ddr5  device type 
#define   SPD_INDEX_DEVICES_INSTALLED_COMMON_DEVICE_TYPE                                                                                                             356        //ddr5  devices installed 
#define   SPD_INDEX_MINOR_REVISION_COMMON_DEVICE_REVISION                                                                                                            357        //ddr5  minor revision 
#define   SPD_INDEX_MAJOR_REVISION_COMMON_DEVICE_REVISION                                                                                                            358        //ddr5  major revision 
#define   SPD_INDEX_SPD_COMMON_PMIC_0_MANUFACTURER_ID_CODE_0_REG                                                                                                     359        //ddr5  spd common pmic 0 manufacturer id code 0 reg 
#define   SPD_INDEX_SPD_COMMON_PMIC_0_MANUFACTURER_ID_CODE_1_REG                                                                                                     360        //ddr5  spd common pmic 0 manufacturer id code 1 reg 
#define   SPD_INDEX_DEVICE_TYPE_COMMON_PMIC_0_DEVICE_TYPE                                                                                                            361        //ddr5  device type 
#define   SPD_INDEX_DEVICES_INSTALLED_COMMON_PMIC_0_DEVICE_TYPE                                                                                                      362        //ddr5  devices installed 
#define   SPD_INDEX_MINOR_REVISION_COMMON_PMIC_0_DEVICE_REVISION                                                                                                     363        //ddr5  minor revision 
#define   SPD_INDEX_MAJOR_REVISION_COMMON_PMIC_0_DEVICE_REVISION                                                                                                     364        //ddr5  major revision 
#define   SPD_INDEX_SPD_COMMON_PMIC_1_MANUFACTURER_ID_CODE_0_REG                                                                                                     365        //ddr5  spd common pmic 1 manufacturer id code 0 reg 
#define   SPD_INDEX_SPD_COMMON_PMIC_1_MANUFACTURER_ID_CODE_1_REG                                                                                                     366        //ddr5  spd common pmic 1 manufacturer id code 1 reg 
#define   SPD_INDEX_DEVICE_TYPE_COMMON_PMIC_1_DEVICE_TYPE                                                                                                            367        //ddr5  device type 
#define   SPD_INDEX_DEVICES_INSTALLED_COMMON_PMIC_1_DEVICE_TYPE                                                                                                      368        //ddr5  devices installed 
#define   SPD_INDEX_MINOR_REVISION_COMMON_PMIC_1_DEVICE_REVISION                                                                                                     369        //ddr5  minor revision 
#define   SPD_INDEX_MAJOR_REVISION_COMMON_PMIC_1_DEVICE_REVISION                                                                                                     370        //ddr5  major revision 
#define   SPD_INDEX_SPD_COMMON_PMIC_2_MANUFACTURER_ID_CODE_0_REG                                                                                                     371        //ddr5  spd common pmic 2 manufacturer id code 0 reg 
#define   SPD_INDEX_SPD_COMMON_PMIC_2_MANUFACTURER_ID_CODE_1_REG                                                                                                     372        //ddr5  spd common pmic 2 manufacturer id code 1 reg 
#define   SPD_INDEX_DEVICE_TYPE_COMMON_PMIC_2_DEVICE_TYPE                                                                                                            373        //ddr5  device type 
#define   SPD_INDEX_DEVICES_INSTALLED_COMMON_PMIC_2_DEVICE_TYPE                                                                                                      374        //ddr5  devices installed 
#define   SPD_INDEX_MINOR_REVISION_COMMON_PMIC_2_DEVICE_REVISION                                                                                                     375        //ddr5  minor revision 
#define   SPD_INDEX_MAJOR_REVISION_COMMON_PMIC_2_DEVICE_REVISION                                                                                                     376        //ddr5  major revision 
#define   SPD_INDEX_SPD_COMMON_THERMAL_SENSORS_TS_MANUFACTURER_ID_CODE_0_REG                                                                                         377        //ddr5  spd common thermal sensors ts manufacturer id code 0 reg 
#define   SPD_INDEX_SPD_COMMON_THERMAL_SENSORS_TS_MANUFACTURER_ID_CODE_1_REG                                                                                         378        //ddr5  spd common thermal sensors ts manufacturer id code 1 reg 
#define   SPD_INDEX_DEVICE_TYPE_COMMON_THERMAL_SENSORS_TS_DEVICE_TYPE                                                                                                379        //ddr5  device type 
#define   SPD_INDEX_DEVICES_INSTALLED_COMMON_THERMAL_SENSORS_TS_DEVICE_TYPE                                                                                          380        //ddr5  devices installed 
#define   SPD_INDEX_MINOR_REVISION_COMMON_THERMAL_SENSORS_TS_DEVICE_REVISION                                                                                         381        //ddr5  minor revision 
#define   SPD_INDEX_MAJOR_REVISION_COMMON_THERMAL_SENSORS_TS_DEVICE_REVISION                                                                                         382        //ddr5  major revision 
#define   SPD_INDEX_MODULE_NOMINAL_HEIGHT_MAX                                                                                                                        383        //ddr5  module nominal height max in mm baseline height 15 mm 
#define   SPD_INDEX_MODULE_MAXIMUM_THICKNESS_MAX_FRONT                                                                                                               384        //ddr5  module maximum thickness max front in mm baseline thickness 1 mm 
#define   SPD_INDEX_MODULE_MAXIMUM_THICKNESS_MAX_BACK                                                                                                                385        //ddr5  module maximum thickness max back in mm baseline thickness 1 mm 
#define   SPD_INDEX_REFERENCE_DESIGN                                                                                                                                 386        //ddr5  reference design 
#define   SPD_INDEX_DESIGN_REVISION                                                                                                                                  387        //ddr5  design revision 
#define   SPD_INDEX_OF_ROWS_OF_DRAMS_ON_MODULE                                                                                                                       388        //ddr5  of rows of drams on module 
#define   SPD_INDEX_TEMPERATURE_GRADE                                                                                                                                389        //ddr5  temperature grade 
#define   SPD_INDEX_NUMBER_OF_PACKAGE_RANKS_PER_CHANNEL                                                                                                              390        //ddr5  number of package ranks per channel 
#define   SPD_INDEX_PRIMARY_BUS_WIDTH_PER_CHANNEL_IN_BITS                                                                                                            391        //ddr5  primary bus width per channel in bits 
#define   SPD_INDEX_BUS_WIDTH_EXTENSION_PER_CHANNEL_IN_BITS                                                                                                          392        //ddr5  bus width extension per channel in bits 
#define   SPD_INDEX_NUMBER_OF_CHANNELS_PER_DIMM                                                                                                                      393        //ddr5  number of channels per dimm 
#define   SPD_INDEX_SPD_RDIMM_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_0_REG                                                                                394        //ddr5  spd rdimm registering clock driver rcd manufacturer id code 0 reg 
#define   SPD_INDEX_SPD_LRDIMM_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_0_REG                                                                               395        //ddr5  spd lrdimm registering clock driver rcd manufacturer id code 0 reg 
#define   SPD_INDEX_SPD_RDIMM_DIFFERENTIAL_MEMORY_BUFFER_DMB_MANUFACTURER_ID_CODE_0_REG                                                                              396        //ddr5  spd rdimm differential memory buffer dmb manufacturer id code 0 reg 
#define   SPD_INDEX_SPD_NVDIMM_N_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_0_REG                                                                             397        //ddr5  spd nvdimm n registering clock driver rcd manufacturer id code 0 reg 
#define   SPD_INDEX_SPD_NVDIMM_P_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_0_REG                                                                             398        //ddr5  spd nvdimm p registering clock driver rcd manufacturer id code 0 reg 
#define   SPD_INDEX_SPD_RDIMM_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_1_REG                                                                                399        //ddr5  spd rdimm registering clock driver rcd manufacturer id code 1 reg 
#define   SPD_INDEX_SPD_LRDIMM_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_1_REG                                                                               400        //ddr5  spd lrdimm registering clock driver rcd manufacturer id code 1 reg 
#define   SPD_INDEX_SPD_RDIMM_DIFFERENTIAL_MEMORY_BUFFER_DMB_MANUFACTURER_ID_CODE_1_REG                                                                              401        //ddr5  spd rdimm differential memory buffer dmb manufacturer id code 1 reg 
#define   SPD_INDEX_SPD_NVDIMM_N_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_1_REG                                                                             402        //ddr5  spd nvdimm n registering clock driver rcd manufacturer id code 1 reg 
#define   SPD_INDEX_SPD_NVDIMM_P_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_1_REG                                                                             403        //ddr5  spd nvdimm p registering clock driver rcd manufacturer id code 1 reg 
#define   SPD_INDEX_DEVICE_TYPE_RDIMM_ING_CLOCK_DRIVER_RCD_DEVICE_TYPE                                                                                               404        //ddr5  device type 
#define   SPD_INDEX_DEVICES_INSTALLED_RDIMM_ING_CLOCK_DRIVER_RCD_DEVICE_TYPE                                                                                         405        //ddr5  devices installed 
#define   SPD_INDEX_DEVICE_TYPE_LRDIMM_ING_CLOCK_DRIVER_RCD_DEVICE_TYPE                                                                                              406        //ddr5  device type 
#define   SPD_INDEX_DEVICES_INSTALLED_LRDIMM_ING_CLOCK_DRIVER_RCD_DEVICE_TYPE                                                                                        407        //ddr5  devices installed 
#define   SPD_INDEX_DEVICE_TYPE_RDIMM_DIFFERENTIAL_MEMORY_BUFFER_DMB_DEVICE_TYPE                                                                                     408        //ddr5  device type 
#define   SPD_INDEX_DEVICES_INSTALLED_RDIMM_DIFFERENTIAL_MEMORY_BUFFER_DMB_DEVICE_TYPE                                                                               409        //ddr5  devices installed 
#define   SPD_INDEX_DEVICE_TYPE_NVDIMM_N_ING_CLOCK_DRIVER_RCD_DEVICE_TYPE                                                                                            410        //ddr5  device type 
#define   SPD_INDEX_DEVICES_INSTALLED_NVDIMM_N_ING_CLOCK_DRIVER_RCD_DEVICE_TYPE                                                                                      411        //ddr5  devices installed 
#define   SPD_INDEX_DEVICE_TYPE_NVDIMM_P_ING_CLOCK_DRIVER_RCD_DEVICE_TYPE                                                                                            412        //ddr5  device type 
#define   SPD_INDEX_DEVICES_INSTALLED_NVDIMM_P_ING_CLOCK_DRIVER_RCD_DEVICE_TYPE                                                                                      413        //ddr5  devices installed 
#define   SPD_INDEX_MINOR_REVISION_RDIMM_ING_CLOCK_DRIVER_RCD_DEVICE_REVISION                                                                                        414        //ddr5  minor revision 
#define   SPD_INDEX_MAJOR_REVISION_RDIMM_ING_CLOCK_DRIVER_RCD_DEVICE_REVISION                                                                                        415        //ddr5  major revision 
#define   SPD_INDEX_MINOR_REVISION_LRDIMM_ING_CLOCK_DRIVER_RCD_DEVICE_REVISION                                                                                       416        //ddr5  minor revision 
#define   SPD_INDEX_MAJOR_REVISION_LRDIMM_ING_CLOCK_DRIVER_RCD_DEVICE_REVISION                                                                                       417        //ddr5  major revision 
#define   SPD_INDEX_MINOR_REVISION_RDIMM_DIFFERENTIAL_MEMORY_BUFFER_DMB_DEVICE_REVISION                                                                              418        //ddr5  minor revision 
#define   SPD_INDEX_MAJOR_REVISION_RDIMM_DIFFERENTIAL_MEMORY_BUFFER_DMB_DEVICE_REVISION                                                                              419        //ddr5  major revision 
#define   SPD_INDEX_MINOR_REVISION_NVDIMM_N_ING_CLOCK_DRIVER_RCD_DEVICE_REVISION                                                                                     420        //ddr5  minor revision 
#define   SPD_INDEX_MAJOR_REVISION_NVDIMM_N_ING_CLOCK_DRIVER_RCD_DEVICE_REVISION                                                                                     421        //ddr5  major revision 
#define   SPD_INDEX_MINOR_REVISION_NVDIMM_P_ING_CLOCK_DRIVER_RCD_DEVICE_REVISION                                                                                     422        //ddr5  minor revision 
#define   SPD_INDEX_MAJOR_REVISION_NVDIMM_P_ING_CLOCK_DRIVER_RCD_DEVICE_REVISION                                                                                     423        //ddr5  major revision 
#define   SPD_INDEX_SPD_LRDIMM_DBS_DB_MANUFACTURER_ID_CODE_0_REG                                                                                                     424        //ddr5  spd lrdimm dbs db manufacturer id code 0 reg 
#define   SPD_INDEX_SPD_NVDIMM_N_DBS_DB_MANUFACTURER_ID_CODE_0_REG                                                                                                   425        //ddr5  spd nvdimm n dbs db manufacturer id code 0 reg 
#define   SPD_INDEX_SPD_NVDIMM_P_DBS_DB_MANUFACTURER_ID_CODE_0_REG                                                                                                   426        //ddr5  spd nvdimm p dbs db manufacturer id code 0 reg 
#define   SPD_INDEX_SPD_LRDIMM_DBS_DB_MANUFACTURER_ID_CODE_1_REG                                                                                                     427        //ddr5  spd lrdimm dbs db manufacturer id code 1 reg 
#define   SPD_INDEX_SPD_NVDIMM_N_DBS_DB_MANUFACTURER_ID_CODE_1_REG                                                                                                   428        //ddr5  spd nvdimm n dbs db manufacturer id code 1 reg 
#define   SPD_INDEX_SPD_NVDIMM_P_DBS_DB_MANUFACTURER_ID_CODE_1_REG                                                                                                   429        //ddr5  spd nvdimm p dbs db manufacturer id code 1 reg 
#define   SPD_INDEX_DEVICE_TYPE_LRDIMM_DBS_DB_DEVICE_TYPE                                                                                                            430        //ddr5  device type 
#define   SPD_INDEX_DEVICES_INSTALLED_LRDIMM_DBS_DB_DEVICE_TYPE                                                                                                      431        //ddr5  devices installed 
#define   SPD_INDEX_DEVICE_TYPE_NVDIMM_N_DBS_DB_DEVICE_TYPE                                                                                                          432        //ddr5  device type 
#define   SPD_INDEX_DEVICES_INSTALLED_NVDIMM_N_DBS_DB_DEVICE_TYPE                                                                                                    433        //ddr5  devices installed 
#define   SPD_INDEX_DEVICE_TYPE_NVDIMM_P_DBS_DB_DEVICE_TYPE                                                                                                          434        //ddr5  device type 
#define   SPD_INDEX_DEVICES_INSTALLED_NVDIMM_P_DBS_DB_DEVICE_TYPE                                                                                                    435        //ddr5  devices installed 
#define   SPD_INDEX_MINOR_REVISION_LRDIMM_DBS_DB_DEVICE_REVISION                                                                                                     436        //ddr5  minor revision 
#define   SPD_INDEX_MAJOR_REVISION_LRDIMM_DBS_DB_DEVICE_REVISION                                                                                                     437        //ddr5  major revision 
#define   SPD_INDEX_MINOR_REVISION_NVDIMM_N_DBS_DB_DEVICE_REVISION                                                                                                   438        //ddr5  minor revision 
#define   SPD_INDEX_MAJOR_REVISION_NVDIMM_N_DBS_DB_DEVICE_REVISION                                                                                                   439        //ddr5  major revision 
#define   SPD_INDEX_MINOR_REVISION_NVDIMM_P_DBS_DB_DEVICE_REVISION                                                                                                   440        //ddr5  minor revision 
#define   SPD_INDEX_MAJOR_REVISION_NVDIMM_P_DBS_DB_DEVICE_REVISION                                                                                                   441        //ddr5  major revision 
#define   SPD_INDEX_ODD_PARITY_FOR_BYTE_552_BITS_6_0                                                                                                                 442        //ddr5  odd parity for byte 552 bits 6 0 
#define   SPD_FIELD_MAX_INDEX                                                                                                                                        443        //Max number of the SPD Field 

#endif // _SPD_FIELD_INDEX_H_
