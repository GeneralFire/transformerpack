/** @file
  SpdDecodeLibDdr4.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <Uefi/UefiBaseType.h>
#include <SpdDecodeCommon.h>
#include <Memory/SpdFieldIndex.h>
#include <Memory/Ddr4SpdRegisters.h>
#include <Library/MemoryCoreLib.h>
#include <Library/SpdDecodeLib.h>
#include <Library/SpdAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <MemRegs.h>
#include <Library/MemoryCoreLib.h>
#include <Library/SysHostPointerLib.h>
#include <ReferenceCodeFatalErrors.h>

//
// DDR4 The field index are defined in CpRcPkg/Include/Memory/SpdFieldIndex.h
//
#define SPD_FIELD_MAPPING_TABLE_DDR4_LENGTH (sizeof (SpdFieldMappingTableDdr4) / sizeof (SpdFieldMappingTableDdr4[0]))
//
// Do not change this table manually!
// Use below command to automatic generate: SpdFieldGen Ddr4SpdRegisters.h Ddr5SpdRegisters.h
//
// 1. The order of the items in this table mush match with order of SPD_FIELD enum or macro order!
// 2. EndByte should not less than the StartByte!
// 3. When the StartByte equal to EndByte, the EndByteEndBit should not less than StartByteStartBit!
//
SPD_FIELD_MAPPING SpdFieldMappingTableDdr4[] = {
// StartByte,                                                                    StartByteStartBit,                         EndByte,                                                                        EndByteEndBit,                            FieldIndex
  {SPD_NUMBER_OF_BYTES_USED_NUMBER_OF_BYTES_IN_SPD_DEVICE_REG,                                0,                            SPD_NUMBER_OF_BYTES_USED_NUMBER_OF_BYTES_IN_SPD_DEVICE_REG,                                 3,                            SPD_INDEX_SPD_BYTES_USED                                                                                                                },
  {SPD_NUMBER_OF_BYTES_USED_NUMBER_OF_BYTES_IN_SPD_DEVICE_REG,                                4,                            SPD_NUMBER_OF_BYTES_USED_NUMBER_OF_BYTES_IN_SPD_DEVICE_REG,                                 6,                            SPD_INDEX_SPD_BYTES_TOTAL                                                                                                               },
  {SPD_REVISION_REG,                                                                          0,                            SPD_REVISION_REG,                                                                           3,                            SPD_INDEX_ADDITIONS_LEVEL                                                                                                               },
  {SPD_REVISION_REG,                                                                          4,                            SPD_REVISION_REG,                                                                           7,                            SPD_INDEX_ENCODING_LEVEL                                                                                                                },
  {SPD_KEY_BYTE_DRAM_DEVICE_TYPE_REG,                                                         0,                            SPD_KEY_BYTE_DRAM_DEVICE_TYPE_REG,                                                          7,                            SPD_INDEX_SDRAM_MODULE_TYPE                                                                                                             },
  {SPD_KEY_BYTE_MODULE_TYPE_REG,                                                              0,                            SPD_KEY_BYTE_MODULE_TYPE_REG,                                                               3,                            SPD_INDEX_BASE_MODULE_TYPE                                                                                                              },
  {SPD_KEY_BYTE_MODULE_TYPE_REG,                                                              4,                            SPD_KEY_BYTE_MODULE_TYPE_REG,                                                               6,                            SPD_INDEX_HYBRID_MEDIA                                                                                                                  },
  {SPD_KEY_BYTE_MODULE_TYPE_REG,                                                              7,                            SPD_KEY_BYTE_MODULE_TYPE_REG,                                                               7,                            SPD_INDEX_HYBRID                                                                                                                        },
  {SPD_SDRAM_DENSITY_AND_BANKS_REG,                                                           0,                            SPD_SDRAM_DENSITY_AND_BANKS_REG,                                                            3,                            SPD_INDEX_TOTAL_SDRAM_CAPACITY_PER_DIE_IN_MEGABITS                                                                                      },
  {SPD_SDRAM_DENSITY_AND_BANKS_REG,                                                           4,                            SPD_SDRAM_DENSITY_AND_BANKS_REG,                                                            5,                            SPD_INDEX_BANK_ADDRESS_BITS                                                                                                             },
  {SPD_SDRAM_DENSITY_AND_BANKS_REG,                                                           6,                            SPD_SDRAM_DENSITY_AND_BANKS_REG,                                                            7,                            SPD_INDEX_BANK_GROUP_BITS                                                                                                               },
  {SPD_SDRAM_ADDRESSING_REG,                                                                  0,                            SPD_SDRAM_ADDRESSING_REG,                                                                   2,                            SPD_INDEX_COLUMN_ADDRESS_BITS                                                                                                           },
  {SPD_SDRAM_ADDRESSING_REG,                                                                  3,                            SPD_SDRAM_ADDRESSING_REG,                                                                   5,                            SPD_INDEX_ROW_ADDRESS_BITS                                                                                                              },
  {SPD_PRIMARY_SDRAM_PACKAGE_TYPE_REG,                                                        0,                            SPD_PRIMARY_SDRAM_PACKAGE_TYPE_REG,                                                         1,                            SPD_INDEX_SIGNAL_LOADING_PRIMARY_SDRAM_PACKAGE_TYPE                                                                                     },
  {SPD_PRIMARY_SDRAM_PACKAGE_TYPE_REG,                                                        4,                            SPD_PRIMARY_SDRAM_PACKAGE_TYPE_REG,                                                         6,                            SPD_INDEX_DIE_COUNT_PRIMARY_SDRAM_PACKAGE_TYPE                                                                                          },
  {SPD_PRIMARY_SDRAM_PACKAGE_TYPE_REG,                                                        7,                            SPD_PRIMARY_SDRAM_PACKAGE_TYPE_REG,                                                         7,                            SPD_INDEX_PRIMARY_SDRAM_PACKAGE_TYPE                                                                                                    },
  {SPD_SDRAM_OPTIONAL_FEATURES_REG,                                                           0,                            SPD_SDRAM_OPTIONAL_FEATURES_REG,                                                            3,                            SPD_INDEX_MAXIMUM_ACTIVATE_COUNT_MAC                                                                                                    },
  {SPD_SDRAM_OPTIONAL_FEATURES_REG,                                                           4,                            SPD_SDRAM_OPTIONAL_FEATURES_REG,                                                            5,                            SPD_INDEX_MAXIMUM_ACTIVATE_WINDOW_TMAW                                                                                                  },
  {SPD_OTHER_SDRAM_OPTIONAL_FEATURES_REG,                                                     5,                            SPD_OTHER_SDRAM_OPTIONAL_FEATURES_REG,                                                      5,                            SPD_INDEX_SOFT_PPR                                                                                                                      },
  {SPD_OTHER_SDRAM_OPTIONAL_FEATURES_REG,                                                     6,                            SPD_OTHER_SDRAM_OPTIONAL_FEATURES_REG,                                                      7,                            SPD_INDEX_POST_PACKAGE_REPAIR_PPR                                                                                                       },
  {SPD_SECONDARY_SDRAM_PACKAGE_TYPE_REG,                                                      0,                            SPD_SECONDARY_SDRAM_PACKAGE_TYPE_REG,                                                       1,                            SPD_INDEX_SIGNAL_LOADING_SECONDARY_SDRAM_PACKAGE_TYPE                                                                                   },
  {SPD_SECONDARY_SDRAM_PACKAGE_TYPE_REG,                                                      2,                            SPD_SECONDARY_SDRAM_PACKAGE_TYPE_REG,                                                       3,                            SPD_INDEX_DRAM_DENSITY_RATIO                                                                                                            },
  {SPD_SECONDARY_SDRAM_PACKAGE_TYPE_REG,                                                      4,                            SPD_SECONDARY_SDRAM_PACKAGE_TYPE_REG,                                                       6,                            SPD_INDEX_DIE_COUNT_SECONDARY_SDRAM_PACKAGE_TYPE                                                                                        },
  {SPD_SECONDARY_SDRAM_PACKAGE_TYPE_REG,                                                      7,                            SPD_SECONDARY_SDRAM_PACKAGE_TYPE_REG,                                                       7,                            SPD_INDEX_SDRAM_PACKAGE_TYPE                                                                                                            },
  {SPD_MODULE_NOMINAL_VOLTAGE_VDD_REG,                                                        0,                            SPD_MODULE_NOMINAL_VOLTAGE_VDD_REG,                                                         1,                            SPD_INDEX_DRAM_VDD_1_2_V                                                                                                                },
  {SPD_MODULE_ORGANIZATION_REG,                                                               0,                            SPD_MODULE_ORGANIZATION_REG,                                                                2,                            SPD_INDEX_SDRAM_DEVICE_WIDTH                                                                                                            },
  {SPD_MODULE_ORGANIZATION_REG,                                                               3,                            SPD_MODULE_ORGANIZATION_REG,                                                                5,                            SPD_INDEX_NUMBER_OF_PACKAGE_RANKS_PER_DIMM                                                                                              },
  {SPD_MODULE_ORGANIZATION_REG,                                                               6,                            SPD_MODULE_ORGANIZATION_REG,                                                                6,                            SPD_INDEX_RANK_MIX                                                                                                                      },
  {SPD_MODULE_MEMORY_BUS_WIDTH_REG,                                                           0,                            SPD_MODULE_MEMORY_BUS_WIDTH_REG,                                                            2,                            SPD_INDEX_PRIMARY_BUS_WIDTH_IN_BITS                                                                                                     },
  {SPD_MODULE_MEMORY_BUS_WIDTH_REG,                                                           3,                            SPD_MODULE_MEMORY_BUS_WIDTH_REG,                                                            4,                            SPD_INDEX_BUS_WIDTH_EXTENSION_IN_BITS                                                                                                   },
  {SPD_MODULE_THERMAL_SENSOR_REG,                                                             7,                            SPD_MODULE_THERMAL_SENSOR_REG,                                                              7,                            SPD_INDEX_THERMAL_SENSOR                                                                                                                },
  {SPD_EXTENDED_MODULE_TYPE_REG,                                                              0,                            SPD_EXTENDED_MODULE_TYPE_REG,                                                               3,                            SPD_INDEX_EXTENDED_BASE_MODULE_TYPE                                                                                                     },
  {SPD_TIMEBASES_REG,                                                                         0,                            SPD_TIMEBASES_REG,                                                                          1,                            SPD_INDEX_FINE_TIMEBASE_FTB                                                                                                             },
  {SPD_TIMEBASES_REG,                                                                         2,                            SPD_TIMEBASES_REG,                                                                          3,                            SPD_INDEX_MEDIUM_TIMEBASE_MTB                                                                                                           },
  {SPD_SDRAM_MINIMUM_CYCLE_TIME_TCKAVGMIN_REG,                                                0,                            SPD_SDRAM_MINIMUM_CYCLE_TIME_TCKAVGMIN_REG,                                                 7,                            SPD_INDEX_MINIMUM_SDRAM_CYCLE_TIME_TCKAVGMIN_MTB_UNITS                                                                                  },
  {SPD_SDRAM_MAXIMUM_CYCLE_TIME_TCKAVGMAX_REG,                                                0,                            SPD_SDRAM_MAXIMUM_CYCLE_TIME_TCKAVGMAX_REG,                                                 7,                            SPD_INDEX_MINIMUM_SDRAM_CYCLE_TIME_TCKAVGMAX_MTB_UNITS                                                                                  },
  {SPD_CAS_LATENCIES_SUPPORTED_REG,                                                           0,                            SPD_CAS_LATENCIES_SUPPORTED_REG,                                                            7,                            SPD_INDEX_CL_FIRST_BYTE                                                                                                                 },
  {SPD_CAS_LATENCIES_SUPPORTED_0_REG,                                                         0,                            SPD_CAS_LATENCIES_SUPPORTED_0_REG,                                                          7,                            SPD_INDEX_CL_SECOND_BYTE                                                                                                                },
  {SPD_CAS_LATENCIES_SUPPORTED_1_REG,                                                         0,                            SPD_CAS_LATENCIES_SUPPORTED_1_REG,                                                          7,                            SPD_INDEX_CL_THIRD_BYTE                                                                                                                 },
  {SPD_CAS_LATENCIES_SUPPORTED_2_REG,                                                         7,                            SPD_CAS_LATENCIES_SUPPORTED_2_REG,                                                          7,                            SPD_INDEX_CL_RANGE                                                                                                                      },
  {SPD_CAS_LATENCIES_SUPPORTED_2_REG,                                                         0,                            SPD_CAS_LATENCIES_SUPPORTED_2_REG,                                                          5,                            SPD_INDEX_CL_FOURTH_BYTE                                                                                                                },
  {SPD_MINIMUM_CAS_LATENCY_TIME_TAAMIN_REG,                                                   0,                            SPD_MINIMUM_CAS_LATENCY_TIME_TAAMIN_REG,                                                    7,                            SPD_INDEX_MINIMUM_SDRAM_CAS_LATENCY_TIME_TAAMIN_MTB_UNITS                                                                               },
  {SPD_MINIMUM_RAS_TO_CAS_DELAY_TIME_TRCDMIN_REG,                                             0,                            SPD_MINIMUM_RAS_TO_CAS_DELAY_TIME_TRCDMIN_REG,                                              7,                            SPD_INDEX_MIN_SDRAM_RAS_TO_CAS_DELAY_TIME_TRCD                                                                                          },
  {SPD_MINIMUM_ROW_PRECHARGE_DELAY_TIME_TRPMIN_REG,                                           0,                            SPD_MINIMUM_ROW_PRECHARGE_DELAY_TIME_TRPMIN_REG,                                            7,                            SPD_INDEX_MINIMUM_ROW_PRECHARGE_TIME_TRPMIN_MTB_UNITS                                                                                   },
  {SPD_UPPER_NIBBLES_FOR_TRASMIN_AND_TRCMIN_REG,                                              0,                            SPD_UPPER_NIBBLES_FOR_TRASMIN_AND_TRCMIN_REG,                                               3,                            SPD_INDEX_TRASMIN_MOST_SIGNIFICANT_NIBBLE                                                                                               },
  {SPD_UPPER_NIBBLES_FOR_TRASMIN_AND_TRCMIN_REG,                                              4,                            SPD_UPPER_NIBBLES_FOR_TRASMIN_AND_TRCMIN_REG,                                               7,                            SPD_INDEX_TRCMIN_MOST_SIGNIFICANT_NIBBLE                                                                                                },
  {SPD_MINIMUM_ACTIVE_TO_PRECHARGE_DELAY_TIME_TRASMIN_LSB_REG,                                0,                            SPD_MINIMUM_ACTIVE_TO_PRECHARGE_DELAY_TIME_TRASMIN_LSB_REG,                                 7,                            SPD_INDEX_MINIMUM_ACTIVE_TO_PRECHARGE_TIME_TRASMIN_MTB_UNITS                                                                            },
  {SPD_MIN_ACTIVE_TO_ACTIVE_REFRESH_DELAY_TIME_TRCMIN_LSB_REG,                                0,                            SPD_MIN_ACTIVE_TO_ACTIVE_REFRESH_DELAY_TIME_TRCMIN_LSB_REG,                                 7,                            SPD_INDEX_MIN_ACTIVE_TO_ACTIVE_REFRESH_TIME_TRC                                                                                         },
  {SPD_MINIMUM_REFRESH_RECOVERY_DELAY_TIME_TRFC1MIN_LSB_REG,                                  0,                            SPD_MINIMUM_REFRESH_RECOVERY_DELAY_TIME_TRFC1MIN_LSB_REG,                                   7,                            SPD_INDEX_MIN_SDRAM_REFRESH_RECOVERY_DELAY_TRFC1_LSB                                                                                    },
  {SPD_MINIMUM_REFRESH_RECOVERY_DELAY_TIME_TRFC1MIN_MSB_REG,                                  0,                            SPD_MINIMUM_REFRESH_RECOVERY_DELAY_TIME_TRFC1MIN_MSB_REG,                                   7,                            SPD_INDEX_MIN_SDRAM_REFRESH_RECOVERY_DELAY_TRFC1_MSB                                                                                    },
  {SPD_MINIMUM_REFRESH_RECOVERY_DELAY_TIME_TRFC2MIN_LSB_REG,                                  0,                            SPD_MINIMUM_REFRESH_RECOVERY_DELAY_TIME_TRFC2MIN_LSB_REG,                                   7,                            SPD_INDEX_MIN_SDRAM_REFRESH_RECOVERY_DELAY_TRFC2_LSB                                                                                    },
  {SPD_MINIMUM_REFRESH_RECOVERY_DELAY_TIME_TRFC2MIN_MSB_REG,                                  0,                            SPD_MINIMUM_REFRESH_RECOVERY_DELAY_TIME_TRFC2MIN_MSB_REG,                                   7,                            SPD_INDEX_MIN_SDRAM_REFRESH_RECOVERY_DELAY_TRFC2_MSB                                                                                    },
  {SPD_MINIMUM_REFRESH_RECOVERY_DELAY_TIME_TRFC4MIN_LSB_REG,                                  0,                            SPD_MINIMUM_REFRESH_RECOVERY_DELAY_TIME_TRFC4MIN_LSB_REG,                                   7,                            SPD_INDEX_MIN_SDRAM_REFRESH_RECOVERY_DELAY_TRFC4_LSB                                                                                    },
  {SPD_MINIMUM_REFRESH_RECOVERY_DELAY_TIME_TRFC4MIN_MSB_REG,                                  0,                            SPD_MINIMUM_REFRESH_RECOVERY_DELAY_TIME_TRFC4MIN_MSB_REG,                                   7,                            SPD_INDEX_MIN_SDRAM_REFRESH_RECOVERY_DELAY_TRFC4_MSB                                                                                    },
  {SPD_UPPER_NIBBLE_FOR_TFAW_REG,                                                             0,                            SPD_UPPER_NIBBLE_FOR_TFAW_REG,                                                              3,                            SPD_INDEX_TFAW_MOST_SIGNIFICANT_NIBBLE                                                                                                  },
  {SPD_MINIMUM_FOUR_ACTIVATE_WINDOW_DELAY_TIME_TFAWMIN_LSB_REG,                               0,                            SPD_MINIMUM_FOUR_ACTIVATE_WINDOW_DELAY_TIME_TFAWMIN_LSB_REG,                                7,                            SPD_INDEX_MIN_FOUR_ACTIVATE_WINDOW_DELAY_TFAW                                                                                           },
  {SPD_MIN_ACTIVATE_TO_ACTIVATE_DELAY_TRRDS_DIFF_BANKS_REG,                                   0,                            SPD_MIN_ACTIVATE_TO_ACTIVATE_DELAY_TRRDS_DIFF_BANKS_REG,                                    7,                            SPD_INDEX_MIN_ACTIVE_TO_ACTIVE_REFRESH_TIME_TRRDS                                                                                       },
  {SPD_MIN_ACTIVATE_TO_ACTIVATE_DELAY_TRRDL_SAME_BANKS_REG,                                   0,                            SPD_MIN_ACTIVATE_TO_ACTIVATE_DELAY_TRRDL_SAME_BANKS_REG,                                    7,                            SPD_INDEX_MIN_ACTIVE_TO_ACTIVE_REFRESH_TIME_TRRDL                                                                                       },
  {SPD_MIN_CAS_TO_CAS_DELAY_TCCDL_SAME_BANKS_REG,                                             0,                            SPD_MIN_CAS_TO_CAS_DELAY_TCCDL_SAME_BANKS_REG,                                              7,                            SPD_INDEX_MINIMUM_CAS_TO_CAS_TIME_TCCD_LMIN_MTB_UNITS                                                                                   },
  {SPD_UPPER_NIBBLE_FOR_TWRMIN_REG,                                                           0,                            SPD_UPPER_NIBBLE_FOR_TWRMIN_REG,                                                            3,                            SPD_INDEX_TWRMIN_MOST_SIGNIFICANT_NIBBLE                                                                                                },
  {SPD_MINIMUM_WRITE_RECOVERY_TIME_TWRMIN_REG,                                                0,                            SPD_MINIMUM_WRITE_RECOVERY_TIME_TWRMIN_REG,                                                 7,                            SPD_INDEX_MINIMUM_WRITE_RECOVERY_TIME_TWRMIN_MTB_UNITS                                                                                  },
  {SPD_UPPER_NIBBLES_FOR_TWTRMIN_REG,                                                         0,                            SPD_UPPER_NIBBLES_FOR_TWTRMIN_REG,                                                          3,                            SPD_INDEX_TWTR_SMIN_MOST_SIGNIFICANT_NIBBLE                                                                                             },
  {SPD_UPPER_NIBBLES_FOR_TWTRMIN_REG,                                                         4,                            SPD_UPPER_NIBBLES_FOR_TWTRMIN_REG,                                                          7,                            SPD_INDEX_TWTR_LMIN_MOST_SIGNIFICANT_NIBBLE                                                                                             },
  {SPD_MIN_WRITE_TO_READ_TIME_TWTRS_DIFF_BANKS_REG,                                           0,                            SPD_MIN_WRITE_TO_READ_TIME_TWTRS_DIFF_BANKS_REG,                                            7,                            SPD_INDEX_MIN_WRITE_RECOVERY_TIME_TWTRS_DIFF_BANKS                                                                                      },
  {SPD_MIN_WRITE_TO_READ_TIME_TWTR_LMIN_SAME_BANK_GROUP_REG,                                  0,                            SPD_MIN_WRITE_TO_READ_TIME_TWTR_LMIN_SAME_BANK_GROUP_REG,                                   7,                            SPD_INDEX_MIN_WRITE_RECOVERY_TIME_TWTRL_SAME_BANKS                                                                                      },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_0_REG,                                                  0,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_0_REG,                                                   4,                            SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ0_3                                                                                                      },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_0_REG,                                                  5,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_0_REG,                                                   5,                            SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ0_3                                                                                             },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_0_REG,                                                  6,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_0_REG,                                                   7,                            SPD_INDEX_PACKAGE_RANK_MAP_DQ0_3                                                                                                        },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_1_REG,                                                  0,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_1_REG,                                                   4,                            SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ4_7                                                                                                      },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_1_REG,                                                  5,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_1_REG,                                                   5,                            SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ4_7                                                                                             },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_1_REG,                                                  6,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_1_REG,                                                   7,                            SPD_INDEX_PACKAGE_RANK_MAP_DQ4_7                                                                                                        },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_2_REG,                                                  0,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_2_REG,                                                   4,                            SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ8_11                                                                                                     },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_2_REG,                                                  5,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_2_REG,                                                   5,                            SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ8_11                                                                                            },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_2_REG,                                                  6,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_2_REG,                                                   7,                            SPD_INDEX_PACKAGE_RANK_MAP_DQ8_11                                                                                                       },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_3_REG,                                                  0,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_3_REG,                                                   4,                            SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ12_15                                                                                                    },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_3_REG,                                                  5,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_3_REG,                                                   5,                            SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ12_15                                                                                           },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_3_REG,                                                  6,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_3_REG,                                                   7,                            SPD_INDEX_PACKAGE_RANK_MAP_DQ12_15                                                                                                      },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_4_REG,                                                  0,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_4_REG,                                                   4,                            SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ16_19                                                                                                    },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_4_REG,                                                  5,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_4_REG,                                                   5,                            SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ16_19                                                                                           },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_4_REG,                                                  6,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_4_REG,                                                   7,                            SPD_INDEX_PACKAGE_RANK_MAP_DQ16_19                                                                                                      },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_5_REG,                                                  0,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_5_REG,                                                   4,                            SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ20_23                                                                                                    },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_5_REG,                                                  5,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_5_REG,                                                   5,                            SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ20_23                                                                                           },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_5_REG,                                                  6,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_5_REG,                                                   7,                            SPD_INDEX_PACKAGE_RANK_MAP_DQ20_23                                                                                                      },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_6_REG,                                                  0,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_6_REG,                                                   4,                            SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ24_27                                                                                                    },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_6_REG,                                                  5,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_6_REG,                                                   5,                            SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ24_27                                                                                           },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_6_REG,                                                  6,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_6_REG,                                                   7,                            SPD_INDEX_PACKAGE_RANK_MAP_DQ24_27                                                                                                      },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_7_REG,                                                  0,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_7_REG,                                                   4,                            SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ28_31                                                                                                    },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_7_REG,                                                  5,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_7_REG,                                                   5,                            SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ28_31                                                                                           },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_7_REG,                                                  6,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_7_REG,                                                   7,                            SPD_INDEX_PACKAGE_RANK_MAP_DQ28_31                                                                                                      },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_8_REG,                                                  0,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_8_REG,                                                   4,                            SPD_INDEX_BIT_ORDER_AT_SDRAM_CB0_3                                                                                                      },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_8_REG,                                                  5,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_8_REG,                                                   5,                            SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_CB0_3                                                                                             },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_8_REG,                                                  6,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_8_REG,                                                   7,                            SPD_INDEX_PACKAGE_RANK_MAP_CB0_3                                                                                                        },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_9_REG,                                                  0,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_9_REG,                                                   4,                            SPD_INDEX_BIT_ORDER_AT_SDRAM_CB4_7                                                                                                      },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_9_REG,                                                  5,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_9_REG,                                                   5,                            SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_CB4_7                                                                                             },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_9_REG,                                                  6,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_9_REG,                                                   7,                            SPD_INDEX_PACKAGE_RANK_MAP_CB4_7                                                                                                        },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_10_REG,                                                 0,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_10_REG,                                                  4,                            SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ32_35                                                                                                    },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_10_REG,                                                 5,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_10_REG,                                                  5,                            SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ32_35                                                                                           },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_10_REG,                                                 6,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_10_REG,                                                  7,                            SPD_INDEX_PACKAGE_RANK_MAP_DQ32_35                                                                                                      },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_11_REG,                                                 0,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_11_REG,                                                  4,                            SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ36_39                                                                                                    },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_11_REG,                                                 5,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_11_REG,                                                  5,                            SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ36_39                                                                                           },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_11_REG,                                                 6,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_11_REG,                                                  7,                            SPD_INDEX_PACKAGE_RANK_MAP_DQ36_39                                                                                                      },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_12_REG,                                                 0,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_12_REG,                                                  4,                            SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ40_43                                                                                                    },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_12_REG,                                                 5,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_12_REG,                                                  5,                            SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ40_43                                                                                           },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_12_REG,                                                 6,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_12_REG,                                                  7,                            SPD_INDEX_PACKAGE_RANK_MAP_DQ40_43                                                                                                      },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_13_REG,                                                 0,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_13_REG,                                                  4,                            SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ44_47                                                                                                    },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_13_REG,                                                 5,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_13_REG,                                                  5,                            SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ44_47                                                                                           },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_13_REG,                                                 6,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_13_REG,                                                  7,                            SPD_INDEX_PACKAGE_RANK_MAP_DQ44_47                                                                                                      },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_14_REG,                                                 0,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_14_REG,                                                  4,                            SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ48_51                                                                                                    },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_14_REG,                                                 5,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_14_REG,                                                  5,                            SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ48_51                                                                                           },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_14_REG,                                                 6,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_14_REG,                                                  7,                            SPD_INDEX_PACKAGE_RANK_MAP_DQ48_51                                                                                                      },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_15_REG,                                                 0,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_15_REG,                                                  4,                            SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ52_55                                                                                                    },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_15_REG,                                                 5,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_15_REG,                                                  5,                            SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ52_55                                                                                           },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_15_REG,                                                 6,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_15_REG,                                                  7,                            SPD_INDEX_PACKAGE_RANK_MAP_DQ52_55                                                                                                      },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_16_REG,                                                 0,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_16_REG,                                                  4,                            SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ56_59                                                                                                    },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_16_REG,                                                 5,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_16_REG,                                                  5,                            SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ56_59                                                                                           },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_16_REG,                                                 6,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_16_REG,                                                  7,                            SPD_INDEX_PACKAGE_RANK_MAP_DQ56_59                                                                                                      },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_17_REG,                                                 0,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_17_REG,                                                  4,                            SPD_INDEX_BIT_ORDER_AT_SDRAM_DQ60_63                                                                                                    },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_17_REG,                                                 5,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_17_REG,                                                  5,                            SPD_INDEX_WIRED_TO_UPPER_LOWER_NIBBLE_DQ60_63                                                                                           },
  {SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_17_REG,                                                 6,                            SPD_CONNECTOR_TO_SDRAM_BIT_MAPPING_17_REG,                                                  7,                            SPD_INDEX_PACKAGE_RANK_MAP_DQ60_63                                                                                                      },
  {SPD_FINE_OFFSET_FOR_MIN_CAS_TO_CAS_DELAY_TCCDL_SAME_BANKS_REG,                             0,                            SPD_FINE_OFFSET_FOR_MIN_CAS_TO_CAS_DELAY_TCCDL_SAME_BANKS_REG,                              7,                            SPD_INDEX_SPD_FINE_OFFSET_FOR_MIN_CAS_TO_CAS_DELAY_TCCDL_SAME_BANKS_REG                                                                 },
  {SPD_FINE_OFFSET_FOR_MIN_ACTIVATE_TO_ACTIVATE_DELAY_TRRDL_SAME_BANKS_REG,                   0,                            SPD_FINE_OFFSET_FOR_MIN_ACTIVATE_TO_ACTIVATE_DELAY_TRRDL_SAME_BANKS_REG,                    7,                            SPD_INDEX_SPD_FINE_OFFSET_FOR_MIN_ACTIVATE_TO_ACTIVATE_DELAY_TRRDL_SAME_BANKS_REG                                                       },
  {SPD_FINE_OFFSET_FOR_MIN_ACTIVATE_TO_ACTIVATE_DELAY_TRRDS_DIFF_BANKS_REG,                   0,                            SPD_FINE_OFFSET_FOR_MIN_ACTIVATE_TO_ACTIVATE_DELAY_TRRDS_DIFF_BANKS_REG,                    7,                            SPD_INDEX_SPD_FINE_OFFSET_FOR_MIN_ACTIVATE_TO_ACTIVATE_DELAY_TRRDS_DIFF_BANKS_REG                                                       },
  {SPD_FINE_OFFSET_FOR_MIN_ACTIVE_TO_ACTIVE_REFRESH_DELAY_TRC_REG,                            0,                            SPD_FINE_OFFSET_FOR_MIN_ACTIVE_TO_ACTIVE_REFRESH_DELAY_TRC_REG,                             7,                            SPD_INDEX_SPD_FINE_OFFSET_FOR_MIN_ACTIVE_TO_ACTIVE_REFRESH_DELAY_TRC_REG                                                                },
  {SPD_FINE_OFFSET_FOR_MIN_ROW_PRECHARGE_DELAY_TRP_REG,                                       0,                            SPD_FINE_OFFSET_FOR_MIN_ROW_PRECHARGE_DELAY_TRP_REG,                                        7,                            SPD_INDEX_SPD_FINE_OFFSET_FOR_MIN_ROW_PRECHARGE_DELAY_TRP_REG                                                                           },
  {SPD_FINE_OFFSET_FOR_MIN_RAS_TO_CAS_DELAY_TIME_TRCDMIN_REG,                                 0,                            SPD_FINE_OFFSET_FOR_MIN_RAS_TO_CAS_DELAY_TIME_TRCDMIN_REG,                                  7,                            SPD_INDEX_SPD_FINE_OFFSET_FOR_MIN_RAS_TO_CAS_DELAY_TIME_TRCDMIN_REG                                                                     },
  {SPD_FINE_OFFSET_FOR_MINIMUM_CAS_LATENCY_TIME_TAAMIN_REG,                                   0,                            SPD_FINE_OFFSET_FOR_MINIMUM_CAS_LATENCY_TIME_TAAMIN_REG,                                    7,                            SPD_INDEX_SPD_FINE_OFFSET_FOR_MINIMUM_CAS_LATENCY_TIME_TAAMIN_REG                                                                       },
  {SPD_FINE_OFFSET_FOR_SDRAM_MAXIMUM_CYCLE_TIME_TCKAVGMAX_REG,                                0,                            SPD_FINE_OFFSET_FOR_SDRAM_MAXIMUM_CYCLE_TIME_TCKAVGMAX_REG,                                 7,                            SPD_INDEX_SPD_FINE_OFFSET_FOR_SDRAM_MAXIMUM_CYCLE_TIME_TCKAVGMAX_REG                                                                    },
  {SPD_FINE_OFFSET_FOR_SDRAM_MINIMUM_CYCLE_TIME_TCKAVGMIN_REG,                                0,                            SPD_FINE_OFFSET_FOR_SDRAM_MINIMUM_CYCLE_TIME_TCKAVGMIN_REG,                                 7,                            SPD_INDEX_SPD_FINE_OFFSET_FOR_SDRAM_MINIMUM_CYCLE_TIME_TCKAVGMIN_REG                                                                    },
  {SPD_CRC_FOR_BASE_CONFIGURATION_SECTION_LSB_REG,                                            0,                            SPD_CRC_FOR_BASE_CONFIGURATION_SECTION_LSB_REG,                                             7,                            SPD_INDEX_SPD_CRC_FOR_BASE_CONFIGURATION_SECTION_LSB_REG                                                                                },
  {SPD_CRC_FOR_BASE_CONFIGURATIONSECTION_MSB_REG,                                             0,                            SPD_CRC_FOR_BASE_CONFIGURATIONSECTION_MSB_REG,                                              7,                            SPD_INDEX_SPD_CRC_FOR_BASE_CONFIGURATIONSECTION_MSB_REG                                                                                 },
  {SPD_UDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_REG,                                    0,                            SPD_UDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_REG,                                     4,                            SPD_INDEX_MODULE_NOMINAL_HEIGHT_MAX_UDIMM                                                                                               },
  {SPD_UDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_REG,                                    5,                            SPD_UDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_REG,                                     7,                            SPD_INDEX_RAW_CARD_EXTENSION_UDIMM                                                                                                      },
  {SPD_RDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_REG,                                    0,                            SPD_RDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_REG,                                     4,                            SPD_INDEX_MODULE_NOMINAL_HEIGHT_MAX_RDIMM                                                                                               },
  {SPD_RDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_REG,                                    5,                            SPD_RDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_REG,                                     7,                            SPD_INDEX_RAW_CARD_EXTENSION_RDIMM                                                                                                      },
  {SPD_LRDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_REG,                                   0,                            SPD_LRDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_REG,                                    4,                            SPD_INDEX_MODULE_NOMINAL_HEIGHT_MAX_LRDIMM                                                                                              },
  {SPD_LRDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_REG,                                   5,                            SPD_LRDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_REG,                                    7,                            SPD_INDEX_RAW_CARD_EXTENSION_LRDIMM                                                                                                     },
  {SPD_UDIMM_MODULE_MAXIMUM_THICKNESS_REG,                                                    0,                            SPD_UDIMM_MODULE_MAXIMUM_THICKNESS_REG,                                                     3,                            SPD_INDEX_MODULE_MAXIMUM_THICKNESS_MAX_FRONT_UDIMM                                                                                      },
  {SPD_UDIMM_MODULE_MAXIMUM_THICKNESS_REG,                                                    4,                            SPD_UDIMM_MODULE_MAXIMUM_THICKNESS_REG,                                                     7,                            SPD_INDEX_MODULE_MAXIMUM_THICKNESS_MAX_BACK_UDIMM                                                                                       },
  {SPD_RDIMM_MODULE_MAXIMUM_THICKNESS_REG,                                                    0,                            SPD_RDIMM_MODULE_MAXIMUM_THICKNESS_REG,                                                     3,                            SPD_INDEX_MODULE_MAXIMUM_THICKNESS_MAX_FRONT_RDIMM                                                                                      },
  {SPD_RDIMM_MODULE_MAXIMUM_THICKNESS_REG,                                                    4,                            SPD_RDIMM_MODULE_MAXIMUM_THICKNESS_REG,                                                     7,                            SPD_INDEX_MODULE_MAXIMUM_THICKNESS_MAX_BACK_RDIMM                                                                                       },
  {SPD_LRDIMM_MODULE_MAXIMUM_THICKNESS_REG,                                                   0,                            SPD_LRDIMM_MODULE_MAXIMUM_THICKNESS_REG,                                                    3,                            SPD_INDEX_MODULE_MAXIMUM_THICKNESS_MAX_FRONT_LRDIMM                                                                                     },
  {SPD_LRDIMM_MODULE_MAXIMUM_THICKNESS_REG,                                                   4,                            SPD_LRDIMM_MODULE_MAXIMUM_THICKNESS_REG,                                                    7,                            SPD_INDEX_MODULE_MAXIMUM_THICKNESS_MAX_BACK_LRDIMM                                                                                      },
  {SPD_UDIMM_REFERENCE_RAW_CARD_USED_REG,                                                     0,                            SPD_UDIMM_REFERENCE_RAW_CARD_USED_REG,                                                      4,                            SPD_INDEX_REFERENCE_RAW_CARD_UDIMM                                                                                                      },
  {SPD_UDIMM_REFERENCE_RAW_CARD_USED_REG,                                                     5,                            SPD_UDIMM_REFERENCE_RAW_CARD_USED_REG,                                                      6,                            SPD_INDEX_REFERENCE_RAW_CARD_REVISION_UDIMM                                                                                             },
  {SPD_UDIMM_REFERENCE_RAW_CARD_USED_REG,                                                     7,                            SPD_UDIMM_REFERENCE_RAW_CARD_USED_REG,                                                      7,                            SPD_INDEX_REFERENCE_RAW_CARD_EXTENSION_UDIMM                                                                                            },
  {SPD_RDIMM_REFERENCE_RAW_CARD_USED_REG,                                                     0,                            SPD_RDIMM_REFERENCE_RAW_CARD_USED_REG,                                                      4,                            SPD_INDEX_REFERENCE_RAW_CARD_RDIMM                                                                                                      },
  {SPD_RDIMM_REFERENCE_RAW_CARD_USED_REG,                                                     5,                            SPD_RDIMM_REFERENCE_RAW_CARD_USED_REG,                                                      6,                            SPD_INDEX_REFERENCE_RAW_CARD_REVISION_RDIMM                                                                                             },
  {SPD_RDIMM_REFERENCE_RAW_CARD_USED_REG,                                                     7,                            SPD_RDIMM_REFERENCE_RAW_CARD_USED_REG,                                                      7,                            SPD_INDEX_REFERENCE_RAW_CARD_EXTENSION_RDIMM                                                                                            },
  {SPD_LRDIMM_REFERENCE_RAW_CARD_USED_REG,                                                    0,                            SPD_LRDIMM_REFERENCE_RAW_CARD_USED_REG,                                                     4,                            SPD_INDEX_REFERENCE_RAW_CARD_LRDIMM                                                                                                     },
  {SPD_LRDIMM_REFERENCE_RAW_CARD_USED_REG,                                                    5,                            SPD_LRDIMM_REFERENCE_RAW_CARD_USED_REG,                                                     6,                            SPD_INDEX_REFERENCE_RAW_CARD_REVISION_LRDIMM                                                                                            },
  {SPD_LRDIMM_REFERENCE_RAW_CARD_USED_REG,                                                    7,                            SPD_LRDIMM_REFERENCE_RAW_CARD_USED_REG,                                                     7,                            SPD_INDEX_REFERENCE_RAW_CARD_EXTENSION_LRDIMM                                                                                           },
  {SPD_UDIMM_ADDRESS_MAPPING_FROM_EDGE_CONNECTOR_TO_DRAM_REG,                                 0,                            SPD_UDIMM_ADDRESS_MAPPING_FROM_EDGE_CONNECTOR_TO_DRAM_REG,                                  0,                            SPD_INDEX_RANK_1_MAPPING_UDIMM_ADDRESS_MAPPING_FROM_EDGE_CONNECTOR_TO_DRAM                                                              },
  {SPD_RDIMM_DIMM_ATTRIBUTES_REG,                                                             0,                            SPD_RDIMM_DIMM_ATTRIBUTES_REG,                                                              1,                            SPD_INDEX_OF_REGISTERS_USED_ON_RDIMM                                                                                                    },
  {SPD_RDIMM_DIMM_ATTRIBUTES_REG,                                                             2,                            SPD_RDIMM_DIMM_ATTRIBUTES_REG,                                                              3,                            SPD_INDEX_OF_ROWS_OF_DRAMS_ON_RDIMM                                                                                                     },
  {SPD_RDIMM_DIMM_ATTRIBUTES_REG,                                                             4,                            SPD_RDIMM_DIMM_ATTRIBUTES_REG,                                                              7,                            SPD_INDEX_REGISTER_TYPE                                                                                                                 },
  {SPD_LRDIMM_DIMM_ATTRIBUTES_REG,                                                            0,                            SPD_LRDIMM_DIMM_ATTRIBUTES_REG,                                                             1,                            SPD_INDEX_OF_REGISTERS_USED_ON_LRDIMM                                                                                                   },
  {SPD_LRDIMM_DIMM_ATTRIBUTES_REG,                                                            2,                            SPD_LRDIMM_DIMM_ATTRIBUTES_REG,                                                             3,                            SPD_INDEX_OF_ROWS_OF_DRAMS_ON_LRDIMM                                                                                                    },
  {SPD_LRDIMM_DIMM_ATTRIBUTES_REG,                                                            4,                            SPD_LRDIMM_DIMM_ATTRIBUTES_REG,                                                             7,                            SPD_INDEX_REGISTER_AND_DATA_BUFFER_TYPES                                                                                                },
  {SPD_RDIMM_RDIMM_THERMAL_HEAT_SPREADER_SOLUTION_REG,                                        0,                            SPD_RDIMM_RDIMM_THERMAL_HEAT_SPREADER_SOLUTION_REG,                                         6,                            SPD_INDEX_HEAT_SPREADER_THERMAL_CHARACTERISTICS_RDIMM                                                                                   },
  {SPD_RDIMM_RDIMM_THERMAL_HEAT_SPREADER_SOLUTION_REG,                                        7,                            SPD_RDIMM_RDIMM_THERMAL_HEAT_SPREADER_SOLUTION_REG,                                         7,                            SPD_INDEX_HEAT_SPREADER_SOLUTION                                                                                                        },
  {SPD_LRDIMM_LRDIMM_THERMAL_HEAT_SPREADER_SOLUTION_REG,                                      0,                            SPD_LRDIMM_LRDIMM_THERMAL_HEAT_SPREADER_SOLUTION_REG,                                       6,                            SPD_INDEX_HEAT_SPREADER_THERMAL_CHARACTERISTICS_LRDIMM                                                                                  },
  {SPD_LRDIMM_LRDIMM_THERMAL_HEAT_SPREADER_SOLUTION_REG,                                      7,                            SPD_LRDIMM_LRDIMM_THERMAL_HEAT_SPREADER_SOLUTION_REG,                                       7,                            SPD_INDEX_HEAT_SPREADER                                                                                                                 },
  {SPD_RDIMM_REGISTER_MANUFACTURER_ID_CODE_LSB_REG,                                           0,                            SPD_RDIMM_REGISTER_MANUFACTURER_ID_CODE_LSB_REG,                                            6,                            SPD_INDEX_NUMBER_OF_CONTINUATION_CODES_REGISTER_MANUFACTURER                                                                            },
  {SPD_RDIMM_REGISTER_MANUFACTURER_ID_CODE_LSB_REG,                                           7,                            SPD_RDIMM_REGISTER_MANUFACTURER_ID_CODE_LSB_REG,                                            7,                            SPD_INDEX_ODD_PARITY_FOR_BYTE_133_BITS_6_0_RDIMM                                                                                        },
  {SPD_LRDIMM_REGISTER_AND_DB_MANUFACTURER_ID_CODE_LSB_REG,                                   0,                            SPD_LRDIMM_REGISTER_AND_DB_MANUFACTURER_ID_CODE_LSB_REG,                                    6,                            SPD_INDEX_CONTINUATION_CODES_NUM_MEMORY_BUFFER_MANUFACTURER                                                                             },
  {SPD_LRDIMM_REGISTER_AND_DB_MANUFACTURER_ID_CODE_LSB_REG,                                   7,                            SPD_LRDIMM_REGISTER_AND_DB_MANUFACTURER_ID_CODE_LSB_REG,                                    7,                            SPD_INDEX_ODD_PARITY_FOR_BYTE_133_BITS_6_0_LRDIMM                                                                                       },
  {SPD_RDIMM_REGISTER_MANUFACTURER_ID_CODE_MSB_REG,                                           0,                            SPD_RDIMM_REGISTER_MANUFACTURER_ID_CODE_MSB_REG,                                            7,                            SPD_INDEX_LAST_NON_ZERO_BYTE_REGISTER_MANUFACTURER                                                                                      },
  {SPD_LRDIMM_REGISTER_AND_DB_MANUFACTURER_ID_CODE_MSB_REG,                                   0,                            SPD_LRDIMM_REGISTER_AND_DB_MANUFACTURER_ID_CODE_MSB_REG,                                    7,                            SPD_INDEX_LAST_NON_ZERO_BYTE_MEMORY_BUFFER_MANUFACTURER                                                                                 },
  {SPD_RDIMM_REGISTER_REVISION_NUMBER_REG,                                                    0,                            SPD_RDIMM_REGISTER_REVISION_NUMBER_REG,                                                     7,                            SPD_INDEX_REGISTER_REVISION_NUMBER_RDIMM                                                                                                },
  {SPD_LRDIMM_REGISTER_REVISION_NUMBER_REG,                                                   0,                            SPD_LRDIMM_REGISTER_REVISION_NUMBER_REG,                                                    7,                            SPD_INDEX_REGISTER_REVISION_NUMBER_LRDIMM                                                                                               },
  {SPD_RDIMM_ADDRESS_MAPPING_FROM_REGISTER_TO_DRAM_REG,                                       0,                            SPD_RDIMM_ADDRESS_MAPPING_FROM_REGISTER_TO_DRAM_REG,                                        0,                            SPD_INDEX_RANK_1_MAPPING_RDIMM_ADDRESS_MAPPING_FROMISTER_TO_DRAM                                                                        },
  {SPD_LRDIMM_ADDRESS_MAPPING_FROM_REGISTER_TO_DRAM_REG,                                      0,                            SPD_LRDIMM_ADDRESS_MAPPING_FROM_REGISTER_TO_DRAM_REG,                                       0,                            SPD_INDEX_ODD_RANK_MAPPING                                                                                                              },
  {SPD_RDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CTRL_AND_CMD_ADDR_REG,                                 0,                            SPD_RDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CTRL_AND_CMD_ADDR_REG,                                  1,                            SPD_INDEX_CKE_RDIMM                                                                                                                     },
  {SPD_RDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CTRL_AND_CMD_ADDR_REG,                                 2,                            SPD_RDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CTRL_AND_CMD_ADDR_REG,                                  3,                            SPD_INDEX_ODT_RDIMM                                                                                                                     },
  {SPD_RDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CTRL_AND_CMD_ADDR_REG,                                 4,                            SPD_RDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CTRL_AND_CMD_ADDR_REG,                                  5,                            SPD_INDEX_COMMAND_ADDRESS_RDIMM                                                                                                         },
  {SPD_RDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CTRL_AND_CMD_ADDR_REG,                                 6,                            SPD_RDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CTRL_AND_CMD_ADDR_REG,                                  7,                            SPD_INDEX_CHIP_SELECT_RDIMM                                                                                                             },
  {SPD_LRDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CTRL_AND_CMD_ADDR_REG,                                0,                            SPD_LRDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CTRL_AND_CMD_ADDR_REG,                                 1,                            SPD_INDEX_CKE_LRDIMM                                                                                                                    },
  {SPD_LRDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CTRL_AND_CMD_ADDR_REG,                                2,                            SPD_LRDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CTRL_AND_CMD_ADDR_REG,                                 3,                            SPD_INDEX_ODT_LRDIMM                                                                                                                    },
  {SPD_LRDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CTRL_AND_CMD_ADDR_REG,                                4,                            SPD_LRDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CTRL_AND_CMD_ADDR_REG,                                 5,                            SPD_INDEX_COMMAND_ADDRESS_LRDIMM                                                                                                        },
  {SPD_LRDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CTRL_AND_CMD_ADDR_REG,                                6,                            SPD_LRDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CTRL_AND_CMD_ADDR_REG,                                 7,                            SPD_INDEX_CHIP_SELECT_LRDIMM                                                                                                            },
  {SPD_RDIMM_REGISTER_OUTPUT_DRIVE_STRENGTH_FOR_CLOCK_REG,                                    0,                            SPD_RDIMM_REGISTER_OUTPUT_DRIVE_STRENGTH_FOR_CLOCK_REG,                                     1,                            SPD_INDEX_Y0_Y2_RDIMM                                                                                                                   },
  {SPD_RDIMM_REGISTER_OUTPUT_DRIVE_STRENGTH_FOR_CLOCK_REG,                                    2,                            SPD_RDIMM_REGISTER_OUTPUT_DRIVE_STRENGTH_FOR_CLOCK_REG,                                     3,                            SPD_INDEX_Y1_Y3_RDIMM                                                                                                                   },
  {SPD_LRDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CLOCK_AND_DB_CTRL_REG,                                0,                            SPD_LRDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CLOCK_AND_DB_CTRL_REG,                                 1,                            SPD_INDEX_Y0_Y2_LRDIMM                                                                                                                  },
  {SPD_LRDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CLOCK_AND_DB_CTRL_REG,                                2,                            SPD_LRDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CLOCK_AND_DB_CTRL_REG,                                 3,                            SPD_INDEX_Y1_Y3_LRDIMM                                                                                                                  },
  {SPD_LRDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CLOCK_AND_DB_CTRL_REG,                                4,                            SPD_LRDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CLOCK_AND_DB_CTRL_REG,                                 4,                            SPD_INDEX_BCOM_BODT_BCKE                                                                                                                },
  {SPD_LRDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CLOCK_AND_DB_CTRL_REG,                                5,                            SPD_LRDIMM_OUTPUT_DRIVE_STRENGTH_FOR_CLOCK_AND_DB_CTRL_REG,                                 5,                            SPD_INDEX_BCK                                                                                                                           },
  {SPD_LRDIMM_DB_REVISION_NUMBER_REG,                                                         0,                            SPD_LRDIMM_DB_REVISION_NUMBER_REG,                                                          7,                            SPD_INDEX_DATA_BUFFER_REVISION_NUMBER                                                                                                   },
  {SPD_LRDIMM_DRAM_VREFDQ_FOR_PACKAGE_RANK_0_REG,                                             0,                            SPD_LRDIMM_DRAM_VREFDQ_FOR_PACKAGE_RANK_0_REG,                                              5,                            SPD_INDEX_DRAM_VREFDQ_FOR_PACKAGE_RANK_0_DRAMS                                                                                          },
  {SPD_LRDIMM_DRAM_VREFDQ_FOR_PACKAGE_RANK_1_REG,                                             0,                            SPD_LRDIMM_DRAM_VREFDQ_FOR_PACKAGE_RANK_1_REG,                                              5,                            SPD_INDEX_DRAM_VREFDQ_FOR_PACKAGE_RANK_1_DRAMS                                                                                          },
  {SPD_LRDIMM_DRAM_VREFDQ_FOR_PACKAGE_RANK_2_REG,                                             0,                            SPD_LRDIMM_DRAM_VREFDQ_FOR_PACKAGE_RANK_2_REG,                                              5,                            SPD_INDEX_DRAM_VREFDQ_FOR_PACKAGE_RANK_2_DRAMS                                                                                          },
  {SPD_LRDIMM_DRAM_VREFDQ_FOR_PACKAGE_RANK_3_REG,                                             0,                            SPD_LRDIMM_DRAM_VREFDQ_FOR_PACKAGE_RANK_3_REG,                                              5,                            SPD_INDEX_DRAM_VREFDQ_FOR_PACKAGE_RANK_3_DRAMS                                                                                          },
  {SPD_LRDIMM_DB_VREFDQ_FOR_DRAM_INTERFACE_REG,                                               0,                            SPD_LRDIMM_DB_VREFDQ_FOR_DRAM_INTERFACE_REG,                                                7,                            SPD_INDEX_DATA_BUFFER_VREFDQ_FOR_DRAM_INTERFACE                                                                                         },
  {SPD_LRDIMM_DB_MDQ_DRIVE_STRENGTH_AND_RTT_REG,                                              0,                            SPD_LRDIMM_DB_MDQ_DRIVE_STRENGTH_AND_RTT_REG,                                               3,                            SPD_INDEX_DRAM_INTERFACE_MDQ_READ_TERMINATION_STRENGTH_1866                                                                             },
  {SPD_LRDIMM_DB_MDQ_DRIVE_STRENGTH_AND_RTT_REG,                                              4,                            SPD_LRDIMM_DB_MDQ_DRIVE_STRENGTH_AND_RTT_REG,                                               7,                            SPD_INDEX_DRAM_INTERFACE_MDQ_DRIVE_STRENGTH_1866                                                                                        },
  {SPD_LRDIMM_DB_MDQ_DRIVE_STRENGTH_AND_RTT_0_REG,                                            0,                            SPD_LRDIMM_DB_MDQ_DRIVE_STRENGTH_AND_RTT_0_REG,                                             3,                            SPD_INDEX_DRAM_INTERFACE_MDQ_READ_TERMINATION_STRENGTH_1866_2400                                                                        },
  {SPD_LRDIMM_DB_MDQ_DRIVE_STRENGTH_AND_RTT_0_REG,                                            4,                            SPD_LRDIMM_DB_MDQ_DRIVE_STRENGTH_AND_RTT_0_REG,                                             7,                            SPD_INDEX_DRAM_INTERFACE_MDQ_DRIVE_STRENGTH_1866_2400                                                                                   },
  {SPD_LRDIMM_DB_MDQ_DRIVE_STRENGTH_AND_RTT_1_REG,                                            0,                            SPD_LRDIMM_DB_MDQ_DRIVE_STRENGTH_AND_RTT_1_REG,                                             3,                            SPD_INDEX_DRAM_INTERFACE_MDQ_READ_TERMINATION_STRENGTH_2400_3200                                                                        },
  {SPD_LRDIMM_DB_MDQ_DRIVE_STRENGTH_AND_RTT_1_REG,                                            4,                            SPD_LRDIMM_DB_MDQ_DRIVE_STRENGTH_AND_RTT_1_REG,                                             7,                            SPD_INDEX_DRAM_INTERFACE_MDQ_DRIVE_STRENGTH_2400_3200                                                                                   },
  {SPD_LRDIMM_DRAM_DRIVE_STRENGTH_REG,                                                        0,                            SPD_LRDIMM_DRAM_DRIVE_STRENGTH_REG,                                                         1,                            SPD_INDEX_DRAM_DRIVE_0_1                                                                                                                },
  {SPD_LRDIMM_DRAM_DRIVE_STRENGTH_REG,                                                        2,                            SPD_LRDIMM_DRAM_DRIVE_STRENGTH_REG,                                                         3,                            SPD_INDEX_DRAM_DRIVE_2_3                                                                                                                },
  {SPD_LRDIMM_DRAM_DRIVE_STRENGTH_REG,                                                        4,                            SPD_LRDIMM_DRAM_DRIVE_STRENGTH_REG,                                                         5,                            SPD_INDEX_DRAM_DRIVE_4_5                                                                                                                },
  {SPD_LRDIMM_DRAM_ODT_RTT_WR_AND_RTT_NOM_REG,                                                0,                            SPD_LRDIMM_DRAM_ODT_RTT_WR_AND_RTT_NOM_REG,                                                 2,                            SPD_INDEX_RTT_NOM_1866                                                                                                                  },
  {SPD_LRDIMM_DRAM_ODT_RTT_WR_AND_RTT_NOM_REG,                                                3,                            SPD_LRDIMM_DRAM_ODT_RTT_WR_AND_RTT_NOM_REG,                                                 5,                            SPD_INDEX_RTT_WR_1866                                                                                                                   },
  {SPD_LRDIMM_DRAM_ODT_RTT_WR_AND_RTT_NOM_2_REG,                                              0,                            SPD_LRDIMM_DRAM_ODT_RTT_WR_AND_RTT_NOM_2_REG,                                               2,                            SPD_INDEX_RTT_NOM_1866_2400                                                                                                             },
  {SPD_LRDIMM_DRAM_ODT_RTT_WR_AND_RTT_NOM_2_REG,                                              3,                            SPD_LRDIMM_DRAM_ODT_RTT_WR_AND_RTT_NOM_2_REG,                                               5,                            SPD_INDEX_RTT_WR_1866_2400                                                                                                              },
  {SPD_LRDIMM_DRAM_ODT_RTT_WR_AND_RTT_NOM_3_REG,                                              0,                            SPD_LRDIMM_DRAM_ODT_RTT_WR_AND_RTT_NOM_3_REG,                                               2,                            SPD_INDEX_RTT_NOM_2400_3200                                                                                                             },
  {SPD_LRDIMM_DRAM_ODT_RTT_WR_AND_RTT_NOM_3_REG,                                              3,                            SPD_LRDIMM_DRAM_ODT_RTT_WR_AND_RTT_NOM_3_REG,                                               5,                            SPD_INDEX_RTT_WR_2400_3200                                                                                                              },
  {SPD_LRDIMM_DRAM_ODT_RTT_PARK_FOR_DATA_RATE_1866_REG,                                       0,                            SPD_LRDIMM_DRAM_ODT_RTT_PARK_FOR_DATA_RATE_1866_REG,                                        2,                            SPD_INDEX_RTT_PARK_PACKAGE_RANKS_0_1_1866                                                                                               },
  {SPD_LRDIMM_DRAM_ODT_RTT_PARK_FOR_DATA_RATE_1866_REG,                                       3,                            SPD_LRDIMM_DRAM_ODT_RTT_PARK_FOR_DATA_RATE_1866_REG,                                        5,                            SPD_INDEX_RTT_PARK_PACKAGE_RANKS_2_3_1866                                                                                               },
  {SPD_LRDIMM_DRAM_ODT_RTT_PARK_FOR_1866_DATA_RATE_2400_REG,                                  0,                            SPD_LRDIMM_DRAM_ODT_RTT_PARK_FOR_1866_DATA_RATE_2400_REG,                                   2,                            SPD_INDEX_RTT_PARK_PACKAGE_RANKS_0_1_1866_2400                                                                                          },
  {SPD_LRDIMM_DRAM_ODT_RTT_PARK_FOR_1866_DATA_RATE_2400_REG,                                  3,                            SPD_LRDIMM_DRAM_ODT_RTT_PARK_FOR_1866_DATA_RATE_2400_REG,                                   5,                            SPD_INDEX_RTT_PARK_PACKAGE_RANKS_2_3_1866_2400                                                                                          },
  {SPD_LRDIMM_DRAM_ODT_RTT_PARK_FOR_2400_DATA_RATE_3200_REG,                                  0,                            SPD_LRDIMM_DRAM_ODT_RTT_PARK_FOR_2400_DATA_RATE_3200_REG,                                   2,                            SPD_INDEX_RTT_PARK_PACKAGE_RANKS_0_1_2400_3200                                                                                          },
  {SPD_LRDIMM_DRAM_ODT_RTT_PARK_FOR_2400_DATA_RATE_3200_REG,                                  3,                            SPD_LRDIMM_DRAM_ODT_RTT_PARK_FOR_2400_DATA_RATE_3200_REG,                                   5,                            SPD_INDEX_RTT_PARK_PACKAGE_RANKS_2_3_2400_3200                                                                                          },
  {SPD_NVDIMM_MODULE_PRODUCT_IDENTIFIER_LSB_REG,                                              0,                            SPD_NVDIMM_MODULE_PRODUCT_IDENTIFIER_LSB_REG,                                               7,                            SPD_INDEX_SPD_NVDIMM_MODULE_PRODUCT_IDENTIFIER_LSB_REG                                                                                  },
  {SPD_NVDIMM_MODULE_PRODUCT_IDENTIFIER_MSB_REG,                                              0,                            SPD_NVDIMM_MODULE_PRODUCT_IDENTIFIER_MSB_REG,                                               7,                            SPD_INDEX_SPD_NVDIMM_MODULE_PRODUCT_IDENTIFIER_MSB_REG                                                                                  },
  {SPD_NVDIMM_SUBSYS_CTRL_MANUFACTURERS_ID_CODE_LSB_REG,                                      0,                            SPD_NVDIMM_SUBSYS_CTRL_MANUFACTURERS_ID_CODE_LSB_REG,                                       6,                            SPD_INDEX_NUMBER_OF_CONTINUATION_CODES_FLASH_MANUFACTURER                                                                               },
  {SPD_NVDIMM_SUBSYS_CTRL_MANUFACTURERS_ID_CODE_LSB_REG,                                      7,                            SPD_NVDIMM_SUBSYS_CTRL_MANUFACTURERS_ID_CODE_LSB_REG,                                       7,                            SPD_INDEX_ODD_PARITY_FOR_BYTE_194_BITS_6_0                                                                                              },
  {SPD_NVDIMM_SUBSYS_CTRL_MANUFACTURERS_ID_CODE_MSB_REG,                                      0,                            SPD_NVDIMM_SUBSYS_CTRL_MANUFACTURERS_ID_CODE_MSB_REG,                                       7,                            SPD_INDEX_LAST_NON_ZERO_BYTE_FLASH_MANUFACTURER                                                                                         },
  {SPD_NVDIMM_SUBSYSTEM_CONTROLLER_IDENTIFIER_LSB_REG,                                        0,                            SPD_NVDIMM_SUBSYSTEM_CONTROLLER_IDENTIFIER_LSB_REG,                                         7,                            SPD_INDEX_SPD_NVDIMM_SUBSYSTEM_CONTROLLER_IDENTIFIER_LSB_REG                                                                            },
  {SPD_NVDIMM_SUBSYSTEM_CONTROLLER_IDENTIFIER_MSB_REG,                                        0,                            SPD_NVDIMM_SUBSYSTEM_CONTROLLER_IDENTIFIER_MSB_REG,                                         7,                            SPD_INDEX_SPD_NVDIMM_SUBSYSTEM_CONTROLLER_IDENTIFIER_MSB_REG                                                                            },
  {SPD_NVDIMM_SUBSYSTEM_CONTROLLER_REVISION_CODE_REG,                                         0,                            SPD_NVDIMM_SUBSYSTEM_CONTROLLER_REVISION_CODE_REG,                                          7,                            SPD_INDEX_SPD_NVDIMM_SUBSYSTEM_CONTROLLER_REVISION_CODE_REG                                                                             },
  {SPD_NVDIMM_REFERENCE_RAW_CARD_USED_REG,                                                    0,                            SPD_NVDIMM_REFERENCE_RAW_CARD_USED_REG,                                                     4,                            SPD_INDEX_REFERENCE_RAW_CARD                                                                                                            },
  {SPD_NVDIMM_REFERENCE_RAW_CARD_USED_REG,                                                    5,                            SPD_NVDIMM_REFERENCE_RAW_CARD_USED_REG,                                                     6,                            SPD_INDEX_REFERENCE_RAW_CARD_REVISION                                                                                                   },
  {SPD_NVDIMM_REFERENCE_RAW_CARD_USED_REG,                                                    7,                            SPD_NVDIMM_REFERENCE_RAW_CARD_USED_REG,                                                     7,                            SPD_INDEX_REFERENCE_RAW_CARD_EXTENSION                                                                                                  },
  {SPD_NVDIMM_MODULE_CHARACTERISTICS_REG,                                                     5,                            SPD_NVDIMM_MODULE_CHARACTERISTICS_REG,                                                      7,                            SPD_INDEX_RAW_CARD_EXTENSION                                                                                                            },
  {SPD_NVDIMM_HYBRID_MODULE_MEDIA_TYPES_REG,                                                  2,                            SPD_NVDIMM_HYBRID_MODULE_MEDIA_TYPES_REG,                                                   2,                            SPD_INDEX_NAND_FLASH                                                                                                                    },
  {SPD_NVDIMM_HYBRID_MODULE_MEDIA_TYPES_REG,                                                  1,                            SPD_NVDIMM_HYBRID_MODULE_MEDIA_TYPES_REG,                                                   1,                            SPD_INDEX_SDRAM                                                                                                                         },
  {SPD_NVDIMM_HYBRID_MODULE_MEDIA_TYPES_REG,                                                  0,                            SPD_NVDIMM_HYBRID_MODULE_MEDIA_TYPES_REG,                                                   0,                            SPD_INDEX_UNKNOWN_UNDEFINED                                                                                                             },
  {SPD_NVDIMM_MAXIMUM_INITIALIZATION_TIME_REG,                                                0,                            SPD_NVDIMM_MAXIMUM_INITIALIZATION_TIME_REG,                                                 7,                            SPD_INDEX_MIN_NON_VOLATILE_MEMORY_INITIALIZATION_TIME_SECONDS                                                                           },
  {SPD_FUNCTION_0_LSB_REG,                                                                    0,                            SPD_FUNCTION_0_LSB_REG,                                                                     4,                            SPD_INDEX_FUNCTION_INTERFACE_FUNCTION_0                                                                                                 },
  {SPD_FUNCTION_0_LSB_REG,                                                                    5,                            SPD_FUNCTION_0_LSB_REG,                                                                     7,                            SPD_INDEX_FUNCTION_CLASS_LSB_FUNCTION_0                                                                                                 },
  {SPD_FUNCTION_0_MSB_REG,                                                                    0,                            SPD_FUNCTION_0_MSB_REG,                                                                     1,                            SPD_INDEX_FUNCTION_CLASS_MSB_FUNCTION_0                                                                                                 },
  {SPD_FUNCTION_0_MSB_REG,                                                                    2,                            SPD_FUNCTION_0_MSB_REG,                                                                     5,                            SPD_INDEX_BLOCK_OFFSET_FUNCTION_0                                                                                                       },
  {SPD_FUNCTION_0_MSB_REG,                                                                    7,                            SPD_FUNCTION_0_MSB_REG,                                                                     7,                            SPD_INDEX_IMPLEMENTED_FUNCTION_0                                                                                                        },
  {SPD_FUNCTION_1_LSB_REG,                                                                    0,                            SPD_FUNCTION_1_LSB_REG,                                                                     4,                            SPD_INDEX_FUNCTION_INTERFACE_FUNCTION_1                                                                                                 },
  {SPD_FUNCTION_1_LSB_REG,                                                                    5,                            SPD_FUNCTION_1_LSB_REG,                                                                     7,                            SPD_INDEX_FUNCTION_CLASS_LSB_FUNCTION_1                                                                                                 },
  {SPD_FUNCTION_1_MSB_REG,                                                                    0,                            SPD_FUNCTION_1_MSB_REG,                                                                     1,                            SPD_INDEX_FUNCTION_CLASS_MSB_FUNCTION_1                                                                                                 },
  {SPD_FUNCTION_1_MSB_REG,                                                                    2,                            SPD_FUNCTION_1_MSB_REG,                                                                     5,                            SPD_INDEX_BLOCK_OFFSET_FUNCTION_1                                                                                                       },
  {SPD_FUNCTION_1_MSB_REG,                                                                    7,                            SPD_FUNCTION_1_MSB_REG,                                                                     7,                            SPD_INDEX_IMPLEMENTED_FUNCTION_1                                                                                                        },
  {SPD_FUNCTION_2_LSB_REG,                                                                    0,                            SPD_FUNCTION_2_LSB_REG,                                                                     4,                            SPD_INDEX_FUNCTION_INTERFACE_FUNCTION_2                                                                                                 },
  {SPD_FUNCTION_2_LSB_REG,                                                                    5,                            SPD_FUNCTION_2_LSB_REG,                                                                     7,                            SPD_INDEX_FUNCTION_CLASS_LSB_FUNCTION_2                                                                                                 },
  {SPD_FUNCTION_2_MSB_REG,                                                                    0,                            SPD_FUNCTION_2_MSB_REG,                                                                     1,                            SPD_INDEX_FUNCTION_CLASS_MSB_FUNCTION_2                                                                                                 },
  {SPD_FUNCTION_2_MSB_REG,                                                                    2,                            SPD_FUNCTION_2_MSB_REG,                                                                     5,                            SPD_INDEX_BLOCK_OFFSET_FUNCTION_2                                                                                                       },
  {SPD_FUNCTION_2_MSB_REG,                                                                    7,                            SPD_FUNCTION_2_MSB_REG,                                                                     7,                            SPD_INDEX_IMPLEMENTED_FUNCTION_2                                                                                                        },
  {SPD_FUNCTION_3_LSB_REG,                                                                    0,                            SPD_FUNCTION_3_LSB_REG,                                                                     4,                            SPD_INDEX_FUNCTION_INTERFACE_FUNCTION_3                                                                                                 },
  {SPD_FUNCTION_3_LSB_REG,                                                                    5,                            SPD_FUNCTION_3_LSB_REG,                                                                     7,                            SPD_INDEX_FUNCTION_CLASS_LSB_FUNCTION_3                                                                                                 },
  {SPD_FUNCTION_3_MSB_REG,                                                                    0,                            SPD_FUNCTION_3_MSB_REG,                                                                     1,                            SPD_INDEX_FUNCTION_CLASS_MSB_FUNCTION_3                                                                                                 },
  {SPD_FUNCTION_3_MSB_REG,                                                                    2,                            SPD_FUNCTION_3_MSB_REG,                                                                     5,                            SPD_INDEX_BLOCK_OFFSET_FUNCTION_3                                                                                                       },
  {SPD_FUNCTION_3_MSB_REG,                                                                    7,                            SPD_FUNCTION_3_MSB_REG,                                                                     7,                            SPD_INDEX_IMPLEMENTED_FUNCTION_3                                                                                                        },
  {SPD_FUNCTION_4_LSB_REG,                                                                    0,                            SPD_FUNCTION_4_LSB_REG,                                                                     4,                            SPD_INDEX_FUNCTION_INTERFACE_FUNCTION_4                                                                                                 },
  {SPD_FUNCTION_4_LSB_REG,                                                                    5,                            SPD_FUNCTION_4_LSB_REG,                                                                     7,                            SPD_INDEX_FUNCTION_CLASS_LSB_FUNCTION_4                                                                                                 },
  {SPD_FUNCTION_4_MSB_REG,                                                                    0,                            SPD_FUNCTION_4_MSB_REG,                                                                     1,                            SPD_INDEX_FUNCTION_CLASS_MSB_FUNCTION_4                                                                                                 },
  {SPD_FUNCTION_4_MSB_REG,                                                                    2,                            SPD_FUNCTION_4_MSB_REG,                                                                     5,                            SPD_INDEX_BLOCK_OFFSET_FUNCTION_4                                                                                                       },
  {SPD_FUNCTION_4_MSB_REG,                                                                    7,                            SPD_FUNCTION_4_MSB_REG,                                                                     7,                            SPD_INDEX_IMPLEMENTED_FUNCTION_4                                                                                                        },
  {SPD_FUNCTION_5_LSB_REG,                                                                    0,                            SPD_FUNCTION_5_LSB_REG,                                                                     4,                            SPD_INDEX_FUNCTION_INTERFACE_FUNCTION_5                                                                                                 },
  {SPD_FUNCTION_5_LSB_REG,                                                                    5,                            SPD_FUNCTION_5_LSB_REG,                                                                     7,                            SPD_INDEX_FUNCTION_CLASS_LSB_FUNCTION_5                                                                                                 },
  {SPD_FUNCTION_5_MSB_REG,                                                                    0,                            SPD_FUNCTION_5_MSB_REG,                                                                     1,                            SPD_INDEX_FUNCTION_CLASS_MSB_FUNCTION_5                                                                                                 },
  {SPD_FUNCTION_5_MSB_REG,                                                                    2,                            SPD_FUNCTION_5_MSB_REG,                                                                     5,                            SPD_INDEX_BLOCK_OFFSET_FUNCTION_5                                                                                                       },
  {SPD_FUNCTION_5_MSB_REG,                                                                    7,                            SPD_FUNCTION_5_MSB_REG,                                                                     7,                            SPD_INDEX_IMPLEMENTED_FUNCTION_5                                                                                                        },
  {SPD_FUNCTION_6_LSB_REG,                                                                    0,                            SPD_FUNCTION_6_LSB_REG,                                                                     4,                            SPD_INDEX_FUNCTION_INTERFACE_FUNCTION_6                                                                                                 },
  {SPD_FUNCTION_6_LSB_REG,                                                                    5,                            SPD_FUNCTION_6_LSB_REG,                                                                     7,                            SPD_INDEX_FUNCTION_CLASS_LSB_FUNCTION_6                                                                                                 },
  {SPD_FUNCTION_6_MSB_REG,                                                                    0,                            SPD_FUNCTION_6_MSB_REG,                                                                     1,                            SPD_INDEX_FUNCTION_CLASS_MSB_FUNCTION_6                                                                                                 },
  {SPD_FUNCTION_6_MSB_REG,                                                                    2,                            SPD_FUNCTION_6_MSB_REG,                                                                     5,                            SPD_INDEX_BLOCK_OFFSET_FUNCTION_6                                                                                                       },
  {SPD_FUNCTION_6_MSB_REG,                                                                    7,                            SPD_FUNCTION_6_MSB_REG,                                                                     7,                            SPD_INDEX_IMPLEMENTED_FUNCTION_6                                                                                                        },
  {SPD_FUNCTION_7_LSB_REG,                                                                    0,                            SPD_FUNCTION_7_LSB_REG,                                                                     4,                            SPD_INDEX_FUNCTION_INTERFACE_FUNCTION_7                                                                                                 },
  {SPD_FUNCTION_7_LSB_REG,                                                                    5,                            SPD_FUNCTION_7_LSB_REG,                                                                     7,                            SPD_INDEX_FUNCTION_CLASS_LSB_FUNCTION_7                                                                                                 },
  {SPD_FUNCTION_7_MSB_REG,                                                                    0,                            SPD_FUNCTION_7_MSB_REG,                                                                     1,                            SPD_INDEX_FUNCTION_CLASS_MSB_FUNCTION_7                                                                                                 },
  {SPD_FUNCTION_7_MSB_REG,                                                                    2,                            SPD_FUNCTION_7_MSB_REG,                                                                     5,                            SPD_INDEX_BLOCK_OFFSET_FUNCTION_7                                                                                                       },
  {SPD_FUNCTION_7_MSB_REG,                                                                    7,                            SPD_FUNCTION_7_MSB_REG,                                                                     7,                            SPD_INDEX_IMPLEMENTED_FUNCTION_7                                                                                                        },
  {SPD_UDIMM_CRC_FOR_SPD_BLOCK_1_LSB_REG,                                                     0,                            SPD_UDIMM_CRC_FOR_SPD_BLOCK_1_LSB_REG,                                                      7,                            SPD_INDEX_SPD_UDIMM_CRC_FOR_SPD_BLOCK_1_LSB_REG                                                                                         },
  {SPD_RDIMM_CRC_FOR_SPD_BLOCK_1_LSB_REG,                                                     0,                            SPD_RDIMM_CRC_FOR_SPD_BLOCK_1_LSB_REG,                                                      7,                            SPD_INDEX_SPD_RDIMM_CRC_FOR_SPD_BLOCK_1_LSB_REG                                                                                         },
  {SPD_LRDIMM_CRC_FOR_SPD_BLOCK_1_LSB_REG,                                                    0,                            SPD_LRDIMM_CRC_FOR_SPD_BLOCK_1_LSB_REG,                                                     7,                            SPD_INDEX_SPD_LRDIMM_CRC_FOR_SPD_BLOCK_1_LSB_REG                                                                                        },
  {SPD_NVDIMM_CRC_FOR_SPD_BLOCK_1_LSB_REG,                                                    0,                            SPD_NVDIMM_CRC_FOR_SPD_BLOCK_1_LSB_REG,                                                     7,                            SPD_INDEX_SPD_NVDIMM_CRC_FOR_SPD_BLOCK_1_LSB_REG                                                                                        },
  {SPD_UDIMM_CRC_FOR_SPD_BLOCK_1_MSB_REG,                                                     0,                            SPD_UDIMM_CRC_FOR_SPD_BLOCK_1_MSB_REG,                                                      7,                            SPD_INDEX_SPD_UDIMM_CRC_FOR_SPD_BLOCK_1_MSB_REG                                                                                         },
  {SPD_RDIMM_CRC_FOR_SPD_BLOCK_1_MSB_REG,                                                     0,                            SPD_RDIMM_CRC_FOR_SPD_BLOCK_1_MSB_REG,                                                      7,                            SPD_INDEX_SPD_RDIMM_CRC_FOR_SPD_BLOCK_1_MSB_REG                                                                                         },
  {SPD_LRDIMM_CRC_FOR_SPD_BLOCK_1_MSB_REG,                                                    0,                            SPD_LRDIMM_CRC_FOR_SPD_BLOCK_1_MSB_REG,                                                     7,                            SPD_INDEX_SPD_LRDIMM_CRC_FOR_SPD_BLOCK_1_MSB_REG                                                                                        },
  {SPD_NVDIMM_CRC_FOR_SPD_BLOCK_1_MSB_REG,                                                    0,                            SPD_NVDIMM_CRC_FOR_SPD_BLOCK_1_MSB_REG,                                                     7,                            SPD_INDEX_SPD_NVDIMM_CRC_FOR_SPD_BLOCK_1_MSB_REG                                                                                        },
  {SPD_MODULE_MANUFACTURER_ID_CODE_LSB_REG,                                                   0,                            SPD_MODULE_MANUFACTURER_ID_CODE_LSB_REG,                                                    6,                            SPD_INDEX_NUMBER_OF_CONTINUATION_CODES_MODULE_MANUFACTURER                                                                              },
  {SPD_MODULE_MANUFACTURER_ID_CODE_LSB_REG,                                                   7,                            SPD_MODULE_MANUFACTURER_ID_CODE_LSB_REG,                                                    7,                            SPD_INDEX_PD_INDEX_ODD_PARITY_BITS_6_0                                                                                                  },
  {SPD_MODULE_MANUFACTURER_ID_CODE_MSB_REG,                                                   0,                            SPD_MODULE_MANUFACTURER_ID_CODE_MSB_REG,                                                    7,                            SPD_INDEX_LAST_NON_ZERO_BYTE_MODULE_MANUFACTURER                                                                                        },
  {SPD_MODULE_MANUFACTURING_LOCATION_REG,                                                     0,                            SPD_MODULE_MANUFACTURING_LOCATION_REG,                                                      7,                            SPD_INDEX_SPD_MODULE_MANUFACTURING_LOCATION_REG                                                                                         },
  {SPD_MODULE_MANUFACTURING_DATE_0_REG,                                                       0,                            SPD_MODULE_MANUFACTURING_DATE_1_REG,                                                        7,                            SPD_INDEX_SPD_MODULE_MANUFACTURING_DATE_REG                                                                                             },
  {SPD_MODULE_SERIAL_NUMBER_0_REG,                                                            0,                            SPD_MODULE_SERIAL_NUMBER_3_REG,                                                             7,                            SPD_INDEX_SPD_MODULE_SERIAL_NUMBER_REG                                                                                                  },
  {SPD_MODULE_PART_NUMBER_0_REG,                                                              0,                            SPD_MODULE_PART_NUMBER_19_REG,                                                              7,                            SPD_INDEX_SPD_MODULE_PART_NUMBER_REG                                                                                                    },
  {SPD_MODULE_REVISION_CODE_REG,                                                              0,                            SPD_MODULE_REVISION_CODE_REG,                                                               7,                            SPD_INDEX_SPD_MODULE_REVISION_CODE_REG                                                                                                  },
  {SPD_DRAM_MANUFACTURER_ID_CODE_LSB_REG,                                                     0,                            SPD_DRAM_MANUFACTURER_ID_CODE_LSB_REG,                                                      6,                            SPD_INDEX_NUMBER_OF_CONTINUATION_CODES_DRAM_MANUFACTURER                                                                                },
  {SPD_DRAM_MANUFACTURER_ID_CODE_LSB_REG,                                                     7,                            SPD_DRAM_MANUFACTURER_ID_CODE_LSB_REG,                                                      7,                            SPD_INDEX_ODD_PARITY_FOR_BYTE_350_BITS_6_0                                                                                              },
  {SPD_DRAM_MANUFACTURER_ID_CODE_MSB_REG,                                                     0,                            SPD_DRAM_MANUFACTURER_ID_CODE_MSB_REG,                                                      7,                            SPD_INDEX_LAST_NON_ZERO_BYTE_DRAM_MANUFACTURER                                                                                          },
  {SPD_DRAM_STEPPING_REG,                                                                     0,                            SPD_DRAM_STEPPING_REG,                                                                      7,                            SPD_INDEX_DRAM_STEPPING                                                                                                                 },
  {SPD_MANUFACTURER_SPECIFIC_DATA_0_REG,                                                      0,                            SPD_MANUFACTURER_SPECIFIC_DATA_28_REG,                                                      7,                            SPD_INDEX_SPD_MANUFACTURER_SPECIFIC_DATA_REG                                                                                            },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
};

/**
  GetSpdFieldMappingTable.

  @param[in]  FieldIndex      - SPD Field Index to be read
  @param[out] SpdFieldMapping - Pointer to SpdFieldMapping

  @retval EFI_SUCCESS           The read completed successful
  @retval EFI_INVALID_PARAMETER  Input parameter out of bounds
**/
EFI_STATUS
EFIAPI
GetSpdFieldMappingTable (
  IN     UINT16            FieldIndex,
  IN OUT SPD_FIELD_MAPPING *SpdFieldMapping
  )
{
  if (FieldIndex >= SPD_FIELD_MAPPING_TABLE_DDR4_LENGTH) {
    return EFI_INVALID_PARAMETER;
  }

  *SpdFieldMapping = SpdFieldMappingTableDdr4[FieldIndex];

  return EFI_SUCCESS;
}

/**
  Get the minimum cycle time for the SDRAM module.

  Spec section:
    Byte 18 (0x012): SDRAM Minimum Cycle Time (tCKAVGmin)

  Description:
    Minimum cycle time applies to all applicable components on the module, in picoseconds (ps).
  It applies to SDRAM and support components as well as the overall capability of the DIMM.

  @param[in]  Host       - Pointer to sysHost
  @param[in]  Socket     - Socket ID
  @param[in]  Chan       - Channel on socket
  @param[in]  Dimm       - DIMM on channel
  @param[out] Val        - Pointer to minimum cycle time value in picosecond

  @retval MRC_STATUS_SUCCESS         Read minimum cycle time successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetMiniCycleTime (
  IN     PSYSHOST   Host,
  IN     UINT8      Socket,
  IN     UINT8      Chan,
  IN     UINT8      Dimm,
     OUT INT32      *Val
  )
{
  UINT8 MinTckMtb, MinTckFtb;
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];
  EFI_STATUS Status;

  DimmNvList = GetDimmNvList (Host, Socket, Chan);

  Status = SpdReadByte (Socket, Chan, Dimm, SPD_SDRAM_MINIMUM_CYCLE_TIME_TCKAVGMIN_REG, &MinTckMtb);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_012);
    return MRC_STATUS_FAILURE;
  }

  Status = SpdReadByte (Socket, Chan, Dimm, SPD_FINE_OFFSET_FOR_SDRAM_MINIMUM_CYCLE_TIME_TCKAVGMIN_REG, &MinTckFtb);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_012);
    return MRC_STATUS_FAILURE;
  }

  if (MinTckFtb & 0x80) {
    *Val = (*DimmNvList)[Dimm].SPDmtb * (INT32) MinTckMtb + (*DimmNvList)[Dimm].SPDftb * ((INT32) MinTckFtb - 256);
  } else {
    *Val = (*DimmNvList)[Dimm].SPDmtb * (INT32) MinTckMtb + (*DimmNvList)[Dimm].SPDftb * (INT32) MinTckFtb;
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Get device type of SDRAM.

  Spec section:
    Byte 6 (0x006): Primary SDRAM Package Type
    Byte 10 (0x00A): Secondary SDRAM Package Type

  Description:
    Dram device type refers to signal loading type, 3ds type, and monolithic device type.

  @param[in]     Socket      - Socket ID
  @param[in]     Ch          - Channel on socket
  @param[in]     Dimm        - Dimm number
  @param[in]     SdramNum    - Target SDRAM number, Primary SDRAM or Secondary SDRAM
  @param[out]    DeviceType  - The pointer to the encoding of device type
                               BIT0: SPD_MULTI_LOAD_TYPE
                               BIT1: SPD_3DS_TYPE
                               BIT7: SPD_NON_MONOLITHIC_TYPE

  @retval MRC_STATUS_SUCCESS  - DeviceType can be valued successful
  @retval MRC_STATUS_FAILURE  - Invalid encoding or SpdReadByte can not be completed successful
**/
MRC_STATUS
EFIAPI
SpdGetDramDeviceType (
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Dimm,
  IN     SDRAM_NUM      SdramNum,
     OUT UINT8          *DeviceType
  )
{
  UINT8       SPDReg = 0;
  UINT8       PrimaryMask;
  EFI_STATUS  Status = EFI_NOT_FOUND;

  //
  // SPD_NON_MONOLITHIC_TYPE: Bit 7
  // SPD_DIE_COUNT_MASK: Bit 4 - Bit6
  // SPD_SIGNAL_LOADING: Bit0 - Bit1
  // Bit2 - Bit3 are reserved for primary package type register on DDR4
  //
  PrimaryMask = SPD_NON_MONOLITHIC_TYPE | SPD_DIE_COUNT_MASK | SPD_SIGNAL_LOADING;

  switch (SdramNum) {
    case PrimarySdramNumber:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_PRIMARY_SDRAM_PACKAGE_TYPE_REG, &SPDReg);
      break;
    case SecondarySdramNumber:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_SECONDARY_SDRAM_PACKAGE_TYPE_REG, &SPDReg);
      break;
    default:
      break;
  }

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_000);
    return MRC_STATUS_FAILURE;
  }

  if (SdramNum == PrimarySdramNumber) {
    *DeviceType = SPDReg & PrimaryMask;
  } else {
    *DeviceType = SPDReg;
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Get die count of SDRAM.

  Spec section:
    Byte 6 (0x006): Primary SDRAM Package Type
    Byte 10 (0x00A): Secondary SDRAM Package Type

  Description:
    Die count counts from single die to 8 die.

  @param[in]     Socket      - Socket ID
  @param[in]     Ch          - Channel on socket
  @param[in]     Dimm        - Dimm number
  @param[in]     SdramNum    - Target SDRAM number, Primary SDRAM or Secondary SDRAM
  @param[out]    DieCount    - The pointer to the variable indicating the actual die count number of SDRAM

  @retval MRC_STATUS_SUCCESS  - DieCount can be valued successful
  @retval MRC_STATUS_FAILURE  - Invalid input or SpdReadByte can not be completed successful
**/
MRC_STATUS
EFIAPI
SpdGetDramDieCount (
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Dimm,
  IN     SDRAM_NUM      SdramNum,
     OUT UINT8          *DieCount
  )
{
  UINT8                                SPDReg = 0;
  EFI_STATUS                           Status;
  PRIMARY_SDRAM_PACKAGE_TYPE_STRUCT    FirstSdramPackageTypeStruct;
  SECONDARY_SDRAM_PACKAGE_TYPE_STRUCT  SecondSdramPackageTypeStruct;

  Status                            = EFI_NOT_FOUND;
  *DieCount                         = 0;
  FirstSdramPackageTypeStruct.Data  = 0;
  SecondSdramPackageTypeStruct.Data = 0;

  switch (SdramNum) {
    case PrimarySdramNumber:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_PRIMARY_SDRAM_PACKAGE_TYPE_REG, &SPDReg);
      break;
    case SecondarySdramNumber:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_SECONDARY_SDRAM_PACKAGE_TYPE_REG, &SPDReg);
      break;
    default:
      break;
  }

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_001);
    return MRC_STATUS_FAILURE;
  }

  if (SdramNum == PrimarySdramNumber) {
    FirstSdramPackageTypeStruct.Data  = SPDReg;
    *DieCount                         = FirstSdramPackageTypeStruct.Bits.die_count + SPD_DIE_BASE;
  } else {
    SecondSdramPackageTypeStruct.Data = SPDReg;
    *DieCount                         = SecondSdramPackageTypeStruct.Bits.die_count + SPD_DIE_BASE;
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Get actual column bits number of SDRAM.

  Spec section:
    Byte 5 (0x005): SDRAM Addressing

  Description:
    Column bits number describes the number of SDRAM column address bits.

  @param[in]     Socket        - Socket ID
  @param[in]     Ch            - Channel on socket
  @param[in]     Dimm          - Dimm number
  @param[in]     SdramNum      - Target SDRAM number, Primary SDRAM or Secondary SDRAM
  @param[out]    NumColBits    - The pointer to the variable indicating the actual column bits number of SDRAM

  @retval MRC_STATUS_SUCCESS  - NumColBits can be valued successful
  @retval MRC_STATUS_FAILURE  - Invalid input or SpdReadByte can not be completed successful
**/
MRC_STATUS
EFIAPI
SpdGetNumColBits (
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Dimm,
  IN     SDRAM_NUM      SdramNum,
     OUT UINT8          *NumColBits
  )
{
  UINT8                    SPDReg = 0;
  EFI_STATUS               Status;
  SDRAM_ADDRESSING_STRUCT  SdramAddressStruct;

  Status                  = EFI_NOT_FOUND;
  *NumColBits             = 0;
  SdramAddressStruct.Data = 0;

  switch (SdramNum) {
    case PrimarySdramNumber:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_SDRAM_ADDRESSING_REG, &SPDReg);
      break;
    default:
      break;
  }

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_002);
    return MRC_STATUS_FAILURE;
  }

  SdramAddressStruct.Data = SPDReg;
  *NumColBits             = SdramAddressStruct.Bits.column_address_bits + SPD_COL_BASE;

  return MRC_STATUS_SUCCESS;
}

/**
  Get actual row bits number of SDRAM.

  Spec section:
    Byte 5 (0x005): SDRAM Addressing

  Description:
    Row bits number describes the number of SDRAM row address bits.

  @param[in]     Socket        - Socket ID
  @param[in]     Ch            - Channel on socket
  @param[in]     Dimm          - Dimm number
  @param[in]     SdramNum      - Target SDRAM number, Primary SDRAM or Secondary SDRAM
  @param[out]    NumRowBits    - The pointer to the variable indicating the actual row bits number of SDRAM

  @retval MRC_STATUS_SUCCESS  - NumRowBits can be valued successful
  @retval MRC_STATUS_FAILURE  - Invalid input or SpdReadByte can not be completed successful
**/
MRC_STATUS
EFIAPI
SpdGetNumRowBits (
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Dimm,
  IN     SDRAM_NUM      SdramNum,
     OUT UINT8          *NumRowBits
  )
{
  UINT8                    SPDReg = 0;
  EFI_STATUS               Status;
  SDRAM_ADDRESSING_STRUCT  SdramAddressStruct;

  Status                  = EFI_NOT_FOUND;
  *NumRowBits             = 0;
  SdramAddressStruct.Data = 0;

  switch (SdramNum) {
    case PrimarySdramNumber:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_SDRAM_ADDRESSING_REG, &SPDReg);
      break;
    default:
      break;
  }

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_003);
    return MRC_STATUS_FAILURE;
  }

  SdramAddressStruct.Data = SPDReg;
  *NumRowBits             = SdramAddressStruct.Bits.row_address_bits + SPD_ROW_BASE;

  return MRC_STATUS_SUCCESS;
}


/**
  Get bank group bits number of SDRAM.

  Spec section:
    Byte 4 (0x004): SDRAM Density and Banks

  Description:
    It describes the number of bank groups.

  @param[in]     Socket        - Socket ID
  @param[in]     Ch            - Channel on socket
  @param[in]     Dimm          - Dimm number
  @param[in]     SdramNum      - Target SDRAM number, Primary SDRAM or Secondary SDRAM
  @param[out]    BankGroupBits - The pointer to the variable indicating the bank group bits number of SDRAM

  @retval MRC_STATUS_SUCCESS  - BankGroupBits can be valued successful
  @retval MRC_STATUS_FAILURE  - Invalid input or SpdReadByte can not be completed successful
**/
MRC_STATUS
EFIAPI
SpdGetBankGroupBits (
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Dimm,
  IN     SDRAM_NUM      SdramNum,
     OUT UINT8          *BankGroupBits
  )
{
  UINT8                           SPDReg = 0;
  SDRAM_DENSITY_AND_BANKS_STRUCT  DensityAndBankStruct;
  EFI_STATUS                      Status;

  *BankGroupBits            = 0;
  DensityAndBankStruct.Data = 0;
  Status                    = EFI_NOT_FOUND;

  switch (SdramNum) {
    case PrimarySdramNumber:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_SDRAM_DENSITY_AND_BANKS_REG, &SPDReg);
      break;
    default:
      break;
  }

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_004);
    return MRC_STATUS_FAILURE;
  }

  DensityAndBankStruct.Data = SPDReg;
  *BankGroupBits   = DensityAndBankStruct.Bits.bank_group_bits;

  return MRC_STATUS_SUCCESS;
}

/**
  Get bank address bits number of SDRAM.

  @param[in]     Socket          - Socket ID
  @param[in]     Ch              - Channel on socket
  @param[in]     Dimm            - Dimm number
  @param[in]     SdramNum        - Target SDRAM number, Primary SDRAM or Secondary SDRAM
  @param[out]    BankAddressBits - The pointer to the variable indicating the bank address bits number of SDRAM

  @retval MRC_STATUS_SUCCESS  - BankAddressBits can be valued successful
  @retval MRC_STATUS_FAILURE  - Invalid input or SpdReadByte can not be completed successful
**/
MRC_STATUS
EFIAPI
SpdGetBankAddressBits (
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Dimm,
  IN     SDRAM_NUM      SdramNum,
     OUT UINT8          *BankAddressBits
  )
{
  UINT8                           SPDReg = 0;
  SDRAM_DENSITY_AND_BANKS_STRUCT  DensityAndBankStruct;
  EFI_STATUS                      Status;

  *BankAddressBits          = 0;
  DensityAndBankStruct.Data = 0;
  Status                    = EFI_NOT_FOUND;

  switch (SdramNum) {
    case PrimarySdramNumber:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_SDRAM_DENSITY_AND_BANKS_REG, &SPDReg);
      break;
    default:
      break;
  }

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_005);
    return MRC_STATUS_FAILURE;
  }

  DensityAndBankStruct.Data = SPDReg;
  *BankAddressBits = DensityAndBankStruct.Bits.bank_address_bits + SPD_BANK_ADDRESS_BASE;

  return MRC_STATUS_SUCCESS;
}

/**
  Get RCD Vendor information

  Spec section:
    DDR4:
    Byte 133 (0x085) (Registered): Register Manufacturer ID Code, LSB
    Byte 134 (0x086) (Registered): Register Manufacturer ID Code, MSB
    Byte 133 (0x085) (Load Reduced): Register and Data Buffer Manufacturer ID Code, LSB
    Byte 134 (0x086) (Load Reduced): Register and Data Buffer Manufacturer ID Code, MSB

  Description:
    RCD vendor information refers to RCD manufacturer ID.

  @param[in]     Socket          - Socket ID
  @param[in]     Channel         - Channel on socket
  @param[in]     Dimm            - Dimm number
  @param[out] RcdVendor       - The pointer to the variable indicating to RCD Vendor

  @retval MRC_FUNCTION_NOT_IMPLEMENTED
**/
MRC_STATUS
EFIAPI
SpdGetRcdVendor (
  IN     UINT8          Socket,
  IN     UINT8          Channel,
  IN     UINT8          Dimm,
     OUT UINT16         *RcdVendor
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/**
  Get RCD Vendor Revision

  Spec section:
    Byte 135 (0x087) (Registered): Register Revision Number
    Byte 135 (0x087) (Load Reduced): Register Revision Number

  Description:
    RCD vendor information refers to RCD revision number, expressed in BCD.

  @param[in]     Socket          - Socket ID
  @param[in]     Channel         - Channel on socket
  @param[in]     Dimm            - Dimm number
  @param[out] RcdRevision     - The pointer to the variable indicating to RCD Revision

  @retval MRC_FUNCTION_NOT_IMPLEMENTED
**/
MRC_STATUS
EFIAPI
SpdGetRcdRevision (
  IN     UINT8          Socket,
  IN     UINT8          Channel,
  IN     UINT8          Dimm,
     OUT UINT8          *RcdRevision
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/**
  Get SDRAM base module type.

  Spec section:
    "Byte 3 (0x003): Key Byte / Module Type"

  Description:
    Base module type identifies the SDRAM memory module type, such as SPD_RDIMM or SPD_LRDIMM.

  @param[in]  Socket     - Socket ID
  @param[in]  Ch         - Channel on socket
  @param[in]  Dimm       - DIMM on channel
  @param[out] Type       - Pointer to base module type

  @retval MRC_STATUS_SUCCESS         Read successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetBaseModuleType (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        Dimm,
     OUT UINT8        *Type
  )
{
  KEY_BYTE_MODULE_TYPE_STRUCT ModuleTypeReg;
  EFI_STATUS Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_KEY_BYTE_MODULE_TYPE_REG, &ModuleTypeReg.Data);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_013);
    return MRC_STATUS_FAILURE;
  }

  *Type = ModuleTypeReg.Bits.base_module_type;

  return MRC_STATUS_SUCCESS;
}

/**
  Get the encoded value of field specifying the sdram capacity per die based on DDR4 encoding rule.

  Spec section:
    Byte 4 (0x004): SDRAM Density and Banks

  Description:
    It describes device density, which refers to sdram capacity per die, in Gbits.

  @param[in]     Socket                      - Socket ID
  @param[in]     Ch                          - Channel on socket
  @param[in]     Dimm                        - Dimm number
  @param[in]     SdramNum                    - Target SDRAM number, Primary SDRAM or Secondary SDRAM
  @param[out]    EncondedDramCapacityDdr4    - The pointer to the variable indicating the encoded value of field specifying the sdram capacity per die

  @retval MRC_STATUS_SUCCESS  - EncodedDramCapacityDdr4 can be valued successful
  @retval MRC_STATUS_FAILURE  - Invalid input or SpdReadByte can not be completed successful
**/
MRC_STATUS
EFIAPI
SpdGetEncodedDramCapacity (
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Dimm,
  IN     SDRAM_NUM      SdramNum,
     OUT UINT8          *EncodedDramCapacityDdr4
  )
{
  UINT8                           SPDReg = 0;
  EFI_STATUS                      Status;
  SDRAM_DENSITY_AND_BANKS_STRUCT  DensityAndBankStruct;

  Status                       = EFI_NOT_FOUND;
  *EncodedDramCapacityDdr4     = 0;
  DensityAndBankStruct.Data    = 0;

  switch (SdramNum) {
    case PrimarySdramNumber:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_SDRAM_DENSITY_AND_BANKS_REG, &SPDReg);
      break;
    default:
      break;
  }

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_008);
    return MRC_STATUS_FAILURE;
  }

  DensityAndBankStruct.Data = SPDReg;
  *EncodedDramCapacityDdr4  = DensityAndBankStruct.Bits.total_sdram_capacity_per_die_in_megabits;

  return MRC_STATUS_SUCCESS;
}

/**
  Get Operable and Endurant encoding VDD.

  Spec section:
    Byte 11 (0x00B): Module Nominal Voltage, VDD

  Description:
    Operable: VDD voltage at which module operation is allowed using the performance values porgrammed in the SPD.
    Endurant: VDD voltage at which the module may be powered without adversely affecting the life expectancy or reliability.

  @param[in]  Socket     - Socket ID
  @param[in]  Ch         - Channel on socket
  @param[in]  Dimm       - DIMM on channel
  @param[out] OperableEndurant     - Pointer to the value of Operable and Endurant

  @retval MRC_STATUS_SUCCESS         Read Operable and Endurant successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetOperableEndurant (
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Dimm,
     OUT UINT8          *OperableEndurant
  )
{
  UINT8             Data = 0;
  EFI_STATUS        Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_MODULE_NOMINAL_VOLTAGE_VDD_REG, &Data);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_014);
    return MRC_STATUS_FAILURE;
  }

  *OperableEndurant = Data;

  return MRC_STATUS_SUCCESS;
}

/**
  Get encoded value of dram IO width bits.

  Spec section:
    Byte 12 (0x00C): Module Organization

  Description:
    It describes the number of SDRAM I/O bits (DQ).

  @param[in]     Socket                  - Socket ID
  @param[in]     Ch                      - Channel on socket
  @param[in]     Dimm                    - Dimm number
  @param[in]     SdramNum                - Targrt SDRAM number, Primary SDRAM or Secondary SDRAM
  @param[out]    EncodedDramIOWidth      - The pointer to the variable indicating SPD encoded value of dram IO width bits

  @retval MRC_STATUS_SUCCESS  - EncodedDramIOWidth can be valued successful
  @retval MRC_STATUS_FAILURE  - Invalid input or SpdReadByte can not be completed successful
**/
MRC_STATUS
EFIAPI
SpdGetEncodedDramIOWidth (
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Dimm,
  IN     SDRAM_NUM      SdramNum,
     OUT UINT8          *EncodedDramIOWidth
  )
{
  UINT8                        SPDReg = 0;
  EFI_STATUS                   Status;
  MODULE_ORGANIZATION_STRUCT   ModuleOrganizationStruct;

  Status                        = EFI_NOT_FOUND;
  *EncodedDramIOWidth           = 0;
  ModuleOrganizationStruct.Data = 0;

  switch (SdramNum) {
    case PrimarySdramNumber:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_MODULE_ORGANIZATION_REG, &SPDReg);
      break;
    default:
      break;
  }

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_009);
    return MRC_STATUS_FAILURE;
  }

  ModuleOrganizationStruct.Data = SPDReg;
  *EncodedDramIOWidth           = ModuleOrganizationStruct.Bits.sdram_device_width;

  return MRC_STATUS_SUCCESS;
}

/**
  Get rank mix indicator of DIMM.

  Spec section:
    Byte 12 (0x00C): Module Organization

  Description:
    It describes whether the assembly has the same SDRAM density on all ranks or has different SDRAM densities on even and odd ranks.

  @param[in]     Socket              - Socket ID
  @param[in]     Ch                  - Channel on socket
  @param[in]     Dimm                - Dimm number
  @param[out]    RankMixIndicator    - The pointer to the variable indicating whether the target DIMM is asymmetirc

  @retval MRC_STATUS_SUCCESS  - RankMixIndicator can be valued successful
  @retval MRC_STATUS_FAILURE  - Invalid input or SpdReadByte can not be completed successful
**/
MRC_STATUS
EFIAPI
SpdGetRankMixIndicator (
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Dimm,
     OUT BOOLEAN        *RankMixIndicator
  )
{
  UINT8                        SPDReg;
  EFI_STATUS                   Status;
  MODULE_ORGANIZATION_STRUCT   ModuleOrganizationStruct;

  *RankMixIndicator             = 0;
  ModuleOrganizationStruct.Data = 0;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_MODULE_ORGANIZATION_REG, &SPDReg);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_010);
    return MRC_STATUS_FAILURE;
  }

  ModuleOrganizationStruct.Data = SPDReg;
  *RankMixIndicator             = (BOOLEAN) ModuleOrganizationStruct.Bits.rank_mix;

  return MRC_STATUS_SUCCESS;
}

/**
  Get Number of package ranks per channel.

  Spec section:
    Byte 12 (0x00C): Module Organization

  Description:
    Package ranks per channel refers to the collections of devices on the module sharing common chip select signals.

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[out] PackageRanks     - Pointer to the number of package ranks

  @retval MRC_STATUS_SUCCESS         Read package ranks number successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetPackageRanks (
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Dimm,
     OUT UINT8          *PackageRanks
  )
{
  MODULE_ORGANIZATION_STRUCT ModuleOrgReg;
  EFI_STATUS        Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_MODULE_ORGANIZATION_REG, &ModuleOrgReg.Data);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_011);
    return MRC_STATUS_FAILURE;
  }

  *PackageRanks = ModuleOrgReg.Bits.number_of_package_ranks_per_dimm;

  return MRC_STATUS_SUCCESS;
}

/**
  Get raw card reference design.

  Spec section:
    Byte 130 (0x082) (Unbuffered): Reference Raw Card Used
    Byte 130 (0x082) (Registered): Reference Raw Card Used
    Byte 130 (0x082) (Load Reduced): Reference Raw Card Used

  Description:

  RawCardRefDesign encoding example:
  0b00000 - A
  0b00001 - B
  ...
  0b10011 - Y
  0b10100 - AA
  ...
  0b11101 - AK
  0b11110 - Reserved
  0b11111 - no reference used.

  @param[in]  Socket             - Socket ID
  @param[in]  Ch                 - Channel on socket
  @param[in]  Dimm               - DIMM on channel
  @param[out] RawCardRefDesign   - Pointer to the raw card reference design.

  @retval MRC_STATUS_SUCCESS         Read raw card reference design successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetRawCardRefDesign (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT8    *RawCardRefDesign
  )
{
  UDIMM_REFERENCE_RAW_CARD_USED_STRUCT UdimmRawCardRefReg;
  RDIMM_REFERENCE_RAW_CARD_USED_STRUCT RdimmRawCardRefReg;
  LRDIMM_REFERENCE_RAW_CARD_USED_STRUCT LrdimmRawCardRefReg;
  UINT8 BaseModuleType = 0;
  EFI_STATUS Status;

  SpdGetBaseModuleType (Socket, Ch, Dimm, &BaseModuleType);

  switch (BaseModuleType) {
    case SPD_UDIMM:
    case SPD_SODIMM:
    case SPD_MINI_UDIMM:
    case SPD_ECC_SO_UDIMM_DDR4:
    case SPD_16b_SO_DIMM:
    case SPD_32b_SO_DIMM:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_UDIMM_REFERENCE_RAW_CARD_USED_REG, &UdimmRawCardRefReg.Data);

      if (EFI_ERROR (Status)) {
        RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_015);
        return MRC_STATUS_FAILURE;
      }

      *RawCardRefDesign = UdimmRawCardRefReg.Bits.reference_raw_card;
    break;

    case SPD_RDIMM:
    case SPD_MINI_RDIMM:
    case SPD_ECC_SO_RDIMM_DDR4:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_RDIMM_REFERENCE_RAW_CARD_USED_REG, &RdimmRawCardRefReg.Data);

      if (EFI_ERROR (Status)) {
        RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_015);
        return MRC_STATUS_FAILURE;
      }

      *RawCardRefDesign = RdimmRawCardRefReg.Bits.reference_raw_card;
    break;

    case SPD_LRDIMM_DDR4:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_LRDIMM_REFERENCE_RAW_CARD_USED_REG, &LrdimmRawCardRefReg.Data);

      if (EFI_ERROR (Status)) {
        RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_015);
        return MRC_STATUS_FAILURE;
      }

      *RawCardRefDesign = LrdimmRawCardRefReg.Bits.reference_raw_card;
    break;

    default:
      RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_UNSUPPORTED_MINOR_CODE_107);
      return MRC_STATUS_UNSUPPORTED;
    break;
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Get raw card reference design Revision.
  Spec section:
    Byte 130 (0x082) (Unbuffered): Reference Raw Card Used
    Byte 130 (0x082) (Registered): Reference Raw Card Used
    Byte 130 (0x082) (Load Reduced): Reference Raw Card Used
    Byte 128 (0x080) (Unbuffered): Raw Card Extension, Module Nominal Height
    Byte 128 (0x080) (Registered): Raw Card Extension, Module Nominal Height
    Byte 128 (0x080) (Load Reduced): Raw Card Extension, Module Nominal Height

  Description:
    RawCardRefDesignRev directly refers to revision number. For example, RawCardRefDesignRev = 6 stands for Revision 6.

  @param[in]  Socket               - Socket ID
  @param[in]  Ch                   - Channel on socket
  @param[in]  Dimm                 - DIMM on channel
  @param[out] RawCardRefDesignRev  - Pointer to the raw card reference design Revision.

  @retval MRC_STATUS_SUCCESS         Read raw card reference design revision successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetRawCardRefDesignRev (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT8    *RawCardRefDesignRev
  )
{
  UDIMM_REFERENCE_RAW_CARD_USED_STRUCT UdimmRawCardRefReg;
  RDIMM_REFERENCE_RAW_CARD_USED_STRUCT RdimmRawCardRefReg;
  LRDIMM_REFERENCE_RAW_CARD_USED_STRUCT LrdimmRawCardRefReg;

  UDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_STRUCT UdimmRawCardExtReg;
  RDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_STRUCT RdimmRawCardExtReg;
  LRDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_STRUCT LrdimmRawCardExtReg;

  UINT8 BaseModuleType = 0;
  UINT8 Rev;
  UINT8 RevExt;
  EFI_STATUS Status;

  SpdGetBaseModuleType (Socket, Ch, Dimm, &BaseModuleType);

  switch (BaseModuleType) {
    case SPD_UDIMM:
    case SPD_SODIMM:
    case SPD_MINI_UDIMM:
    case SPD_ECC_SO_UDIMM_DDR4:
    case SPD_16b_SO_DIMM:
    case SPD_32b_SO_DIMM:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_UDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_REG, &UdimmRawCardExtReg.Data);

      if (EFI_ERROR (Status)) {
        RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_016);
        return MRC_STATUS_FAILURE;
      }
      RevExt = UdimmRawCardExtReg.Bits.raw_card_extension;

      Status = SpdReadByte (Socket, Ch, Dimm, SPD_UDIMM_REFERENCE_RAW_CARD_USED_REG, &UdimmRawCardRefReg.Data);

      if (EFI_ERROR (Status)) {
        RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_016);
        return MRC_STATUS_FAILURE;
      }
      Rev = UdimmRawCardRefReg.Bits.reference_raw_card_revision;
    break;

    case SPD_RDIMM:
    case SPD_MINI_RDIMM:
    case SPD_ECC_SO_RDIMM_DDR4:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_RDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_REG, &RdimmRawCardExtReg.Data);

      if (EFI_ERROR (Status)) {
        RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_016);
        return MRC_STATUS_FAILURE;
      }
      RevExt = RdimmRawCardExtReg.Bits.raw_card_extension;

      Status = SpdReadByte (Socket, Ch, Dimm, SPD_RDIMM_REFERENCE_RAW_CARD_USED_REG, &RdimmRawCardRefReg.Data);

      if (EFI_ERROR (Status)) {
        RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_016);
        return MRC_STATUS_FAILURE;
      }
      Rev = RdimmRawCardRefReg.Bits.reference_raw_card_revision;
    break;

    case SPD_LRDIMM_DDR4:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_LRDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_REG, &LrdimmRawCardExtReg.Data);

      if (EFI_ERROR (Status)) {
        RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_016);
        return MRC_STATUS_FAILURE;
      }
      RevExt = LrdimmRawCardExtReg.Bits.raw_card_extension;

      Status = SpdReadByte (Socket, Ch, Dimm, SPD_LRDIMM_REFERENCE_RAW_CARD_USED_REG, &LrdimmRawCardRefReg.Data);

      if (EFI_ERROR (Status)) {
        RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_016);
        return MRC_STATUS_FAILURE;
      }
      Rev = LrdimmRawCardRefReg.Bits.reference_raw_card_revision;
    break;

    default:
      RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_UNSUPPORTED_MINOR_CODE_108);
      return MRC_STATUS_UNSUPPORTED;
    break;
  }

  if (RevExt == 0x000) {
    *RawCardRefDesignRev = Rev;
  } else {
    *RawCardRefDesignRev = RevExt + 3;
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Get module manufacturing location.

  Spec section:
    Byte 322 (0x142): Module Manufacturing Location

  Description:
    The individual manufacturer keeps track of manufacturing location and its appropriate decode
  represented in ModLoc.

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[out] ModLoc           - Pointer to module manufacturing location

  @retval MRC_STATUS_SUCCESS         Read module manufacturing location successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetModuleManufacturingLocation (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT8    *ModLoc
  )
{
  UINT8             RegData;
  EFI_STATUS        Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_MODULE_MANUFACTURING_LOCATION_REG, &RegData);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_030);
    return MRC_STATUS_FAILURE;
  }

  *ModLoc = RegData;

  return MRC_STATUS_SUCCESS;
}

/**
  Get module manufacturing date.

  Spec section:
    Bytes 323~324 (0x143~0x144): Module Manufacturing Date

  Description:
    ModDate is represented in Binary Coded Decimal (BCD).
  For example, 0x4714 stands for week47 in year 2014.

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[out] ModDate          - Pointer to module manufacturing date

  @retval MRC_STATUS_SUCCESS         Read module manufacturing date successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetModuleManufacturingDate (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT16   *ModDate
  )
{
  UINT8 YearByte;
  UINT8 WeekByte;
  EFI_STATUS Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_MODULE_MANUFACTURING_DATE_0_REG, &YearByte);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_028);
    return MRC_STATUS_FAILURE;
  }

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_MODULE_MANUFACTURING_DATE_1_REG, &WeekByte);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_028);
    return MRC_STATUS_FAILURE;
  }

  *ModDate = (WeekByte << 8) | YearByte;

  return MRC_STATUS_SUCCESS;
}

/**
  Get module serial number.

  Spec section:
    Bytes 325~328 (0x145~0x148): Module Serial Number

  Description:
    ModSerNum refers to the unique serial number for the module.
  ModSerNum has to point to an array with size of SPD_MODULE_SERIAL.

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[out] ModSerNum        - Pointer to module serial number

  @retval MRC_STATUS_SUCCESS         Read module serial number successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetModuleSerialNumber (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT8    *ModSerNum
  )
{
  EFI_STATUS        Status;
  UINT8             RegData[SPD_MODULE_SERIAL];
  UINT8             Index;

  for (Index = 0; Index < SPD_MODULE_SERIAL; Index++) {
    Status = SpdReadByte (Socket, Ch, Dimm, SPD_MODULE_SERIAL_NUMBER_0_REG + Index, &RegData[Index]);

    if (EFI_ERROR (Status)) {
      RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_027);
      return MRC_STATUS_FAILURE;
    }
  }

  CopyMem (ModSerNum, RegData, SPD_MODULE_SERIAL);

  return MRC_STATUS_SUCCESS;
}

/**
  Get module manufacturer ID.

  Spec section:
    Byte 320 (0x140): Module Manufacturer ID Code, LSB
    Byte 321 (0x141): Module Manufacturer ID Code, MSB

  Description:
    ManufacturerId consists of 2 bytes, following encoding format in JEP-106.
  For example, 0x0400 refers to Fujitsu.

  @param[in]  Socket          - Socket ID
  @param[in]  Ch              - Channel on socket
  @param[in]  Dimm            - DIMM on channel
  @param[out] ManufacturerId  - Pointer to manufacturer ID

  @retval MRC_STATUS_SUCCESS         Read successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetModuleManufacturerId (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        Dimm,
     OUT UINT16       *ManufacturerId
  )
{
  MODULE_MANUFACTURER_ID_CODE_LSB_STRUCT ManufactureIdLsbReg;
  MODULE_MANUFACTURER_ID_CODE_MSB_STRUCT ManufactureIdMsbReg;
  EFI_STATUS Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_MODULE_MANUFACTURER_ID_CODE_LSB_REG, &ManufactureIdLsbReg.Data);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_025);
    return MRC_STATUS_FAILURE;
  }

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_MODULE_MANUFACTURER_ID_CODE_MSB_REG, &ManufactureIdMsbReg.Data);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_025);
    return MRC_STATUS_FAILURE;
  }

  *ManufacturerId = (ManufactureIdMsbReg.Bits.last_non_zero_byte_module_manufacturer << 8)
                      | (ManufactureIdLsbReg.Bits.number_of_continuation_codes_module_manufacturer);

  return MRC_STATUS_SUCCESS;
}

/**
  Get module manufacturer part number.

  Spec section:
    Bytes 329~348 (0x149~15C): Module Part Number

  Description:
    Part number is encoded in ASCII format.
  PartNum has to point to an array. Array size is defined by SPD_MODULE_PART_DDR4 for DDR4, SPD_MODULE_PART_DDR5 for DDR5.

  @param[in]  Socket     - Socket ID
  @param[in]  Ch         - Channel on socket
  @param[in]  Dimm       - DIMM on channel
  @param[out] PartNum    - Pointer to manufacturer part number array

  @retval MRC_STATUS_SUCCESS         Read successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetModulePartNum (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        Dimm,
     OUT UINT8        *PartNum
  )
{
  EFI_STATUS        Status;
  UINT8             RegData[SPD_MODULE_PART_DDR4];
  UINT8             Index;

  for (Index = 0; Index < SPD_MODULE_PART_DDR4; Index++) {
    Status = SpdReadByte (Socket, Ch, Dimm, SPD_MODULE_PART_NUMBER_0_REG + Index, &RegData[Index]);

    if (EFI_ERROR (Status)) {
      RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_023);
      return MRC_STATUS_FAILURE;
    }
  }

  CopyMem (PartNum, RegData, SPD_MODULE_PART_DDR4);

  return MRC_STATUS_SUCCESS;
}

/**
  Get dram stepping info.

  Spec section:
    Byte 352 (0x160): DRAM Stepping

  Description:
    Byte defines the vendor die revision level of the DRAMs.

  Examples:
    0x00 - Stepping 0
    0x01 - Stepping 1
    0x31 - Stepping 3.1
    0xA3 - Stepping A3
    0xB1 - Stepping B1
    0xFF - Stepping information not provided

  @param[in]  Socket     - Socket ID
  @param[in]  Ch         - Channel on socket
  @param[in]  Dimm       - DIMM on channel
  @param[out] Stepping   - Pointer to dram stepping info

  @retval MRC_STATUS_SUCCESS         Read successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetDramStepping (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        Dimm,
     OUT UINT8        *Stepping
  )
{
  DRAM_STEPPING_STRUCT DramSteppingReg;
  EFI_STATUS        Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_DRAM_STEPPING_REG, &DramSteppingReg.Data);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_022);
    return MRC_STATUS_FAILURE;
  }

  *Stepping = DramSteppingReg.Bits.dram_stepping;

  return MRC_STATUS_SUCCESS;
}

/**
  Get RCD device type.

  Spec section:
    not supported

  Description:
  Not supported on DDR4.

  @param[in]  Socket     - Socket ID
  @param[in]  Ch         - Channel on socket
  @param[in]  Dimm       - DIMM on channel
  @param[out] RcdType    - Pointer to RCD device type

  @retval MRC_FUNCTION_NOT_IMPLEMENTED
**/
MRC_STATUS
EFIAPI
SpdGetRcdDeviceType (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        Dimm,
     OUT UINT8        *RcdType
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/**
  Get SDRAM module nominal height.

  Spec section:
    Byte 128 (0x080) (Unbuffered): Raw Card Extension, Module Nominal Height
    Byte 128 (0x080): (Registered): Raw Card Extension, Module Nominal Height
    Byte 128 (0x080) (Load Reduced): Raw Card Extension, Module Nominal Height

  Description:
    This byte defines the nominal height (A dimension) in millimeters of the fully assembled module
  including heat spreaders or other added components.

  @param[in]  Socket     - Socket ID
  @param[in]  Ch         - Channel on socket
  @param[in]  Dimm       - DIMM on channel
  @param[out] Height     - Pointer to nominal height in milimeters (mm)

  @retval MRC_STATUS_SUCCESS         Read successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetNominalHeight (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        Dimm,
     OUT UINT8        *Height
  )
{
  UDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_STRUCT UdimmNominalHeightReg;
  RDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_STRUCT RdimmNominalHeightReg;
  LRDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_STRUCT LrdimmNominalHeightReg;
  UINT8 BaseModuleType = 0;
  EFI_STATUS        Status;

  SpdGetBaseModuleType (Socket, Ch, Dimm, &BaseModuleType);

  switch (BaseModuleType) {
    case SPD_UDIMM:
    case SPD_SODIMM:
    case SPD_MINI_UDIMM:
    case SPD_ECC_SO_UDIMM_DDR4:
    case SPD_16b_SO_DIMM:
    case SPD_32b_SO_DIMM:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_UDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_REG, &UdimmNominalHeightReg.Data);

      if (EFI_ERROR (Status)) {
        RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_017);
        return MRC_STATUS_FAILURE;
      }

      *Height = UdimmNominalHeightReg.Bits.module_nominal_height_max;
    break;

    case SPD_RDIMM:
    case SPD_MINI_RDIMM:
    case SPD_ECC_SO_RDIMM_DDR4:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_RDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_REG, &RdimmNominalHeightReg.Data);

      if (EFI_ERROR (Status)) {
        RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_017);
        return MRC_STATUS_FAILURE;
      }

      *Height = RdimmNominalHeightReg.Bits.module_nominal_height_max;
    break;

    case SPD_LRDIMM_DDR4:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_LRDIMM_RAW_CARD_EXTENSION_MODULE_NOMINAL_HEIGHT_REG, &LrdimmNominalHeightReg.Data);

      if (EFI_ERROR (Status)) {
        RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_017);
        return MRC_STATUS_FAILURE;
      }

      *Height = LrdimmNominalHeightReg.Bits.module_nominal_height_max;
    break;

    default:
      RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_UNSUPPORTED_MINOR_CODE_109);
      return MRC_STATUS_UNSUPPORTED;
    break;
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Get SDRAM module type.

  Spec section:
    Byte 2 (0x002): Key Byte / DRAM Device Type

  Description:
    This byte is the key byte used by the system BIOS to determine how to interpret all other bytes in
  the SPD EEPROM.

  @param[in]  Socket     - Socket ID
  @param[in]  Ch         - Channel on socket
  @param[in]  Dimm       - DIMM on channel
  @param[out] Type       - Pointer to module type

  @retval MRC_STATUS_SUCCESS         Read successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetModuleType (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        Dimm,
     OUT UINT8        *Type
  )
{
  KEY_BYTE_DRAM_DEVICE_TYPE_STRUCT KeyByteDramDeviceTypeReg;
  EFI_STATUS        Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_KEY_BYTE_DRAM_DEVICE_TYPE_REG, &KeyByteDramDeviceTypeReg.Data);

  if (EFI_ERROR (Status)) {
    return MRC_STATUS_FAILURE;
  }

  *Type = KeyByteDramDeviceTypeReg.Bits.sdram_module_type;

  return MRC_STATUS_SUCCESS;
}

/**
  Get primay bus width in bits.

  Spec Section:
    Byte 13 (0x00D): Module Memory Bus Width

  Descirption:
    It describes the primary bus width.

  @param[in]  Socket              - Socket ID
  @param[in]  Ch                  - Channel on socket
  @param[in]  Dimm                - DIMM on channel
  @param[out] PrimaryBusWidth     - Pointer to the primary bus width, in bits

  @retval MRC_STATUS_SUCCESS         Read primary bus width successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetPrimaryBusWidth (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT8    *PrimaryBusWidth
  )
{
  MODULE_MEMORY_BUS_WIDTH_STRUCT ModuleBusWidthReg;
  EFI_STATUS        Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_MODULE_MEMORY_BUS_WIDTH_REG, &ModuleBusWidthReg.Data);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_018);
    return MRC_STATUS_FAILURE;
  }

  *PrimaryBusWidth = ModuleBusWidthReg.Bits.primary_bus_width_in_bits;

  return MRC_STATUS_SUCCESS;
}

/**
  Get bus width extension in bits.

  Spec Section:
    Byte 13 (0x00D): Module Memory Bus Width

  Descirption:
    It describes the bus width extention, such as parity or ECC.

  @param[in]  Socket                 - Socket ID
  @param[in]  Ch                     - Channel on socket
  @param[in]  Dimm                   - DIMM on channel
  @param[out] BusWidthExtension      - Pointer to the bus width extension, in bits

  @retval MRC_STATUS_SUCCESS         Read bus width extension successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetBusWidthExtension (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT8    *BusWidthExtension
  )
{
  MODULE_MEMORY_BUS_WIDTH_STRUCT ModuleBusWidthReg;
  EFI_STATUS        Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_MODULE_MEMORY_BUS_WIDTH_REG, &ModuleBusWidthReg.Data);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_019);
    return MRC_STATUS_FAILURE;
  }

  *BusWidthExtension = ModuleBusWidthReg.Bits.bus_width_extension_in_bits;

  return MRC_STATUS_SUCCESS;
}

/**
  Get channel number per dimm.

  Spec Section:
    Not supported

  Descirption:
    It describes the number of channels on each module.

  @param[in]  Socket                 - Socket ID
  @param[in]  Ch                     - Channel on socket
  @param[in]  Dimm                   - DIMM on channel
  @param[out] ChannelNumPerDimm      - Pointer to the number of channels per dimm

  @retval MRC_STATUS_SUCCESS         Read number of channels per dimm successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetChannelNumPerDimm (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT8    *ChannelNumPerDimm
  )
{
  //
  // Not supported
  //
  return MRC_STATUS_SUCCESS;
}

/**
  Get SPD spec revision.

  Spec section:
    Byte 1 (0x001): SPD Revision

  Description:
    SpecRevision is encoded with upper/lower nibble.

  Example:
  0x00 - Revision 0.0
  0x01 - Revision 0.1
  0x09 - Revision 0.9
  0x10 - Revision 1.0
  0x11 - Revision 1.1
  0xFF - Undefined.

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[out] SpecRevision     - Pointer to the value of spec revision

  @retval MRC_STATUS_SUCCESS         Read revision successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetSpecRevision (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT8    *SpecRevision
  )
{
  UINT8             SPDReg = 0;
  EFI_STATUS        Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_REVISION_REG, &SPDReg);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_024);
    return MRC_STATUS_FAILURE;
  }

  *SpecRevision = SPDReg;

  return MRC_STATUS_SUCCESS;
}

/**
  Get the minimum SDRAM Row Precharge Delay Time.

  Spec section:
    Byte 26 (0x01A): Minimum Row Precharge Delay Time (tRPmin)
    Byte 121 (0x079): Fine Offset for Minimum Row Precharge Delay Time (tRPmin)

  Description:
    It defines the minimum SDRAM Row Precharge Delay Time in picoseconds (ps).

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[out] TrpVal           - Pointer to the minimum SDRAM Row Precharge Delay Time in picoseconds (ps).

  @retval MRC_STATUS_SUCCESS         Read delay time successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetTrp (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT16   *TrpVal
  )
{
  UINT8 TrpMtb, TrpFtb;
  EFI_STATUS Status;
  PSYSHOST   Host;
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];

  Host = GetSysHostPointer ();
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_MINIMUM_ROW_PRECHARGE_DELAY_TIME_TRPMIN_REG, &TrpMtb);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_026);
    return MRC_STATUS_FAILURE;
  }

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_FINE_OFFSET_FOR_MIN_ROW_PRECHARGE_DELAY_TRP_REG, &TrpFtb);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_026);
    return MRC_STATUS_FAILURE;
  }

  if (TrpFtb & 0x80) {
    *TrpVal = (*DimmNvList)[Dimm].SPDmtb * (INT16) TrpMtb + (*DimmNvList)[Dimm].SPDftb * ((INT16) TrpFtb - 256);
  } else {
    *TrpVal = (*DimmNvList)[Dimm].SPDmtb * (INT16) TrpMtb + (*DimmNvList)[Dimm].SPDftb * (INT16) TrpFtb;
  }
  return MRC_STATUS_SUCCESS;
}

/**
  Get the Minimum SDRAM Active to Precharge Delay Time.

  Spec section:
    Byte 27 (0x01B): Upper Nibbles for tRASmin and tRCmin
    Byte 28 (0x01C): Minimum Active to Precharge Delay Time (tRASmin), Least Significant Byte

    Description:
    It defines the minimum SDRAM Active to Precharge Delay Time in picoseconds (ps).

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[out] TrasVal          - Pointer to the Minimum SDRAM Active to Precharge Delay Time in picoseconds (ps).

  @retval MRC_STATUS_SUCCESS         Read delay time successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetTras (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT16   *TrasVal
  )
{
  EFI_STATUS Status;
  PSYSHOST   Host;
  UPPER_NIBBLES_FOR_TRASMIN_AND_TRCMIN_STRUCT UpperTrasMinReg;
  MINIMUM_ACTIVE_TO_PRECHARGE_DELAY_TIME_TRASMIN_LSB_STRUCT TrasMinReg;
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];

  Host = GetSysHostPointer ();
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_UPPER_NIBBLES_FOR_TRASMIN_AND_TRCMIN_REG, &UpperTrasMinReg.Data);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_029);
    return MRC_STATUS_FAILURE;
  }

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_MINIMUM_ACTIVE_TO_PRECHARGE_DELAY_TIME_TRASMIN_LSB_REG, &TrasMinReg.Data);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_029);
    return MRC_STATUS_FAILURE;
  }

  *TrasVal = ((UpperTrasMinReg.Bits.trasmin_most_significant_nibble << 8)
    | TrasMinReg.Bits.minimum_active_to_precharge_time_trasmin_mtb_units)
    * (*DimmNvList)[Dimm].SPDmtb;

  return MRC_STATUS_SUCCESS;
}

/**
  Get the minimum SDRAM Active to Active/Refresh Delay Time.

  Spec section:
    Byte 27 (0x01B): Upper Nibbles for tRASmin and tRCmin
    Byte 29 (0x01D): Minimum Active to Active/Refresh Delay Time (tRCmin), Least Significant Byte
    Byte 120 (0x078): Fine Offset for Minimum Active to Active/Refresh Delay Time (tRCmin)

  Description:
    It defines the minimum SDRAM Active to Active/Refresh Delay Time in picoseconds (ps).

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[out] TrcVal           - Pointer to the minimum SDRAM Active to Active/Refresh Delay Time in picoseconds (ps).

  @retval MRC_STATUS_SUCCESS         Read delay time successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetTrc (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT16   *TrcVal
  )
{
  UINT8      TrcFtb;
  UINT8      TrcMtb;
  EFI_STATUS Status;
  PSYSHOST   Host;
  UPPER_NIBBLES_FOR_TRASMIN_AND_TRCMIN_STRUCT                UpperTrcMinReg;
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];

  Host = GetSysHostPointer ();
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_UPPER_NIBBLES_FOR_TRASMIN_AND_TRCMIN_REG, &UpperTrcMinReg.Data);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_031);
    return MRC_STATUS_FAILURE;
  }

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_MIN_ACTIVE_TO_ACTIVE_REFRESH_DELAY_TIME_TRCMIN_LSB_REG, &TrcMtb);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_031);
    return MRC_STATUS_FAILURE;
  }

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_FINE_OFFSET_FOR_MIN_ACTIVE_TO_ACTIVE_REFRESH_DELAY_TRC_REG, &TrcFtb);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_031);
    return MRC_STATUS_FAILURE;
  }

  if (TrcFtb & 0x80) {
    *TrcVal = (*DimmNvList)[Dimm].SPDmtb * (INT16) (UpperTrcMinReg.Bits.trcmin_most_significant_nibble << 8 | TrcMtb)
      + (*DimmNvList)[Dimm].SPDftb * ((INT16) TrcFtb - 256);
  } else {
    *TrcVal = (*DimmNvList)[Dimm].SPDmtb * (INT16) (UpperTrcMinReg.Bits.trcmin_most_significant_nibble << 8 | TrcMtb)
      + (*DimmNvList)[Dimm].SPDftb * (INT16) TrcFtb;
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Get the minimum SDRAM RAS to CAS Delay Time.

  Spec section:
    Byte 25 (0x019): Minimum RAS to CAS Delay Time (tRCDmin)
    Byte 122 (0x07A): Fine Offset for Minimum RAS to CAS Delay Time (tRCDmin)

  Description:
    It defines the minimum SDRAM RAS to CAS Delay Time in picoseconds (ps).

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[out] TrcdVal          - Pointer to the minimum SDRAM RAS to CAS Delay Time in picoseconds (ps).

  @retval MRC_STATUS_SUCCESS         Read delay time successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetTrcd (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT16   *TrcdVal
  )
{
  UINT8         TrcdMtb, TrcdFtb;
  EFI_STATUS    Status;
  PSYSHOST      Host;
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];

  Host = GetSysHostPointer ();
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_MINIMUM_RAS_TO_CAS_DELAY_TIME_TRCDMIN_REG, &TrcdMtb);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_032);
    return MRC_STATUS_FAILURE;
  }

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_FINE_OFFSET_FOR_MIN_RAS_TO_CAS_DELAY_TIME_TRCDMIN_REG, &TrcdFtb);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_032);
    return MRC_STATUS_FAILURE;
  }

  if (TrcdFtb & 0x80) {
    *TrcdVal = (*DimmNvList)[Dimm].SPDmtb * (INT16) TrcdMtb + (*DimmNvList)[Dimm].SPDftb * ((INT16) TrcdFtb - 256);
  } else {
    *TrcdVal = (*DimmNvList)[Dimm].SPDmtb * (INT16) TrcdMtb + (*DimmNvList)[Dimm].SPDftb * (INT16) TrcdFtb;
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Get the the minimum CAS Latency.

  Spec section:
    Byte 24 (0x018): Minimum CAS Latency Time (tAAmin)
    Byte 123 (0x07B): Fine Offset for Minimum CAS Latency Time (tAAmin)

  Description:
    It defines the minimum CAS Latency in picoseconds (ps).

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[out] TaaVal           - Pointer to the minimum CAS Latency in picoseconds (ps).

  @retval MRC_STATUS_SUCCESS         Read CAS Latency successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetTaa (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT16   *TaaVal
  )
{
  UINT8         TaaMtb, TaaFtb;
  EFI_STATUS    Status;
  PSYSHOST      Host;
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];

  Host = GetSysHostPointer ();
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_MINIMUM_CAS_LATENCY_TIME_TAAMIN_REG, &TaaMtb);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_033);
    return MRC_STATUS_FAILURE;
  }

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_FINE_OFFSET_FOR_MIN_RAS_TO_CAS_DELAY_TIME_TRCDMIN_REG, &TaaFtb);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_033);
    return MRC_STATUS_FAILURE;
  }

  if (TaaFtb & 0x80) {
    *TaaVal = (*DimmNvList)[Dimm].SPDmtb * (INT16) TaaMtb + (*DimmNvList)[Dimm].SPDftb * ((INT16) TaaFtb - 256);
  } else {
    *TaaVal = (*DimmNvList)[Dimm].SPDmtb * (INT16) TaaMtb + (*DimmNvList)[Dimm].SPDftb * (INT16) TaaFtb;
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Get supported CAS Latency (CL) values with one bit per possible CAS Latency.
  Spec section:
    Byte 20 (0x014): CAS Latencies Supported, First Byte
    Byte 21 (0x015): CAS Latencies Supported, Second Byte
    Byte 22 (0x016): CAS Latencies Supported, Third Byte
    Byte 23 (0x017): CAS Latencies Supported, Fourth Byte

  Description:
    It define which CAS Latency (CL) values are supported with one bit per possible CAS Latency.
    0 : means this CAS Latency is not supported.
    1 : means this CAS Latency is supported.

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[out] CasSup           - Pointer to supported CAS Latency bitmap values

  @retval MRC_STATUS_SUCCESS         Read supported CAS Latency values successful
  @retval MRC_STATUS_FAILURE         Read failed
**/
MRC_STATUS
EFIAPI
SpdGetCasSupported (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
     OUT UINT64   *CasSup
  )
{
  UINT8             SPDReg = 0;
  EFI_STATUS        Status;

  //
  // Get byte 4 of CasSup
  //
  Status = SpdReadByte (Socket, Ch, Dimm, SPD_CAS_LATENCIES_SUPPORTED_2_REG, &SPDReg);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_034);
    return MRC_STATUS_FAILURE;
  }

  *CasSup = (UINT64) (SPDReg << 24);

  //
  // Get byte 3 of CasSup
  //
  Status = SpdReadByte (Socket, Ch, Dimm, SPD_CAS_LATENCIES_SUPPORTED_1_REG, &SPDReg);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_034);
    return MRC_STATUS_FAILURE;
  }

  *CasSup |= (UINT64) (SPDReg << 16);

  //
  // Get byte 2 of CasSup
  //
  Status = SpdReadByte (Socket, Ch, Dimm, SPD_CAS_LATENCIES_SUPPORTED_0_REG, &SPDReg);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_034);
    return MRC_STATUS_FAILURE;
  }

  *CasSup |= (UINT64) (SPDReg << 8);

  //
  // Get byte 1 of CasSup
  //
  Status = SpdReadByte (Socket, Ch, Dimm, SPD_CAS_LATENCIES_SUPPORTED_REG, &SPDReg);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_034);
    return MRC_STATUS_FAILURE;
  }

  *CasSup |= (UINT64) SPDReg;

  return MRC_STATUS_SUCCESS;
}

/**
  Get whether thermal sensor is installed.

  Spec section:
    Byte 14 (0x00E): Module Thermal Sensor

  Description:
    SensorIndex refers to TS0 or TS1 on DDR5. Its value is ignored for DDR4.

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[in]  SensorIndex      - 0: TS0; 1: TS1

  @retval TRUE         - installed
  @retval FALSE        - not installed
**/
BOOLEAN
EFIAPI
SpdIsThermalSensorInstalled (
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
  IN     UINT8    SensorIndex
  )
{
  MODULE_THERMAL_SENSOR_STRUCT  ThermalSensorReg;
  EFI_STATUS        Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_MODULE_THERMAL_SENSOR_REG, &ThermalSensorReg.Data);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_035);
    return FALSE;
  }

  return ((ThermalSensorReg.Bits.thermal_sensor) == 1);
}

/**
  Get whether a specific PPR feature is supported.

  Spec section:
    Byte 9 (0x009): Other SDRAM Optional Features

  Description:
    PPR: Post package repair
  PprType refers to either PprSoft or PprOneRowPerBankGroup.

  @param[in]  Socket           - Socket ID
  @param[in]  Ch               - Channel on socket
  @param[in]  Dimm             - DIMM on channel
  @param[in]  PprType          - PPR type in PPR_TYPE_LIST

  @retval TRUE        - supported
  @retval FALSE       - not supported
**/
BOOLEAN
EFIAPI
SpdIsPprSupported (
  IN     UINT8         Socket,
  IN     UINT8         Ch,
  IN     UINT8         Dimm,
  IN     PPR_TYPE_LIST PprType
  )
{
  OTHER_SDRAM_OPTIONAL_FEATURES_STRUCT OptionalFeaturesReg;
  EFI_STATUS        Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_OTHER_SDRAM_OPTIONAL_FEATURES_REG, &OptionalFeaturesReg.Data);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_036);
    return FALSE;
  }

  if (PprType == PprSoft) {
    return OptionalFeaturesReg.Bits.soft_ppr == 1;

  } else if (PprType == PprOneRowPerBankGroup) {
    return OptionalFeaturesReg.Bits.post_package_repair_ppr == 1;

  } else {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_UNSUPPORTED_MINOR_CODE_106);
    return FALSE;
  }
}

/**
  Get manufacturer ID of the DRAM.

  Spec section:
    Byte 350 (0x15E): DRAM Manufacturer ID Code, LSB
    Byte 351 (0x15F): DRAM Manufacturer ID Code, MSB

  Description:
    This two-byte field indicates the manufacturer of the DRAM on the module, and shall be encoded
  as follows: the first byte is the number of continuation bytes indicated in JEP-106; the second byte
  is the last non-zero byte of the manufacturer ID code, again as indicated in JEP-106.
**/
MRC_STATUS
EFIAPI
SpdGetDramManufacturerId (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        Dimm,
     OUT UINT16       *DramManufacturerId
  )
{
  EFI_STATUS Status;
  DRAM_MANUFACTURER_ID_CODE_LSB_STRUCT DramManufactureIdLsbReg;
  DRAM_MANUFACTURER_ID_CODE_MSB_STRUCT DramManufactureIdMsbReg;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_DRAM_MANUFACTURER_ID_CODE_LSB_REG, &DramManufactureIdLsbReg.Data);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_037);
    return MRC_STATUS_FAILURE;
  }

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_DRAM_MANUFACTURER_ID_CODE_MSB_REG, &DramManufactureIdMsbReg.Data);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_037);
    return MRC_STATUS_FAILURE;
  }

  *DramManufacturerId = (UINT16) (DramManufactureIdMsbReg.Bits.last_non_zero_byte_dram_manufacturer << 8)
                      | (UINT16) (DramManufactureIdLsbReg.Bits.number_of_continuation_codes_dram_manufacturer);

  return MRC_STATUS_SUCCESS;
}
