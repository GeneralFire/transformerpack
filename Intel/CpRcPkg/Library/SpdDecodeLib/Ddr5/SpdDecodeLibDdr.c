/** @file
  SpdDecodeLibDdr5.c

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
#include <Memory/Ddr5SpdRegisters.h>
#include <Library/SpdDecodeLib.h>
#include <Library/SpdAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <MemRegs.h>
#include <Library/MemoryCoreLib.h>
#include <ReferenceCodeFatalErrors.h>

//
// DDR5 The field index are defined in CpRcPkg/Include/Memory/SpdFieldIndex.h
//
#define SPD_FIELD_MAPPING_TABLE_DDR5_LENGTH (sizeof (SpdFieldMappingTableDdr5) / sizeof (SpdFieldMappingTableDdr5[0]))
//
// Do not change this table manually!
// Use below command to automatic generate: SpdFieldGen Ddr4SpdRegisters.h Ddr5SpdRegisters.h
//
// 1. The order of the items in this table mush match with order of SPD_FIELD enum or macro order!
// 2. EndByte should not less than the StartByte!
// 3. When the StartByte equal to EndByte, the EndByteEndBit should not less than StartByteStartBit!
//
SPD_FIELD_MAPPING SpdFieldMappingTableDdr5[] = {
// StartByte,                                                                    StartByteStartBit,                         EndByte,                                                                        EndByteEndBit,                            FieldIndex
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {SPD_NUMBER_OF_BYTES_IN_SPD_DEVICE_REG,                                                     4,                            SPD_NUMBER_OF_BYTES_IN_SPD_DEVICE_REG,                                                      6,                            SPD_INDEX_SPD_BYTES_TOTAL                                                                                                               },
  {SPD_SPD_REVISION_FOR_BASE_CONFIGURATION_PARAMETERS_REG,                                    0,                            SPD_SPD_REVISION_FOR_BASE_CONFIGURATION_PARAMETERS_REG,                                     3,                            SPD_INDEX_ADDITIONS_LEVEL                                                                                                               },
  {SPD_SPD_REVISION_FOR_BASE_CONFIGURATION_PARAMETERS_REG,                                    4,                            SPD_SPD_REVISION_FOR_BASE_CONFIGURATION_PARAMETERS_REG,                                     7,                            SPD_INDEX_ENCODING_LEVEL                                                                                                                },
  {SPD_KEY_BYTE_HOST_BUS_COMMAND_PROTOCOL_TYPE_REG,                                           0,                            SPD_KEY_BYTE_HOST_BUS_COMMAND_PROTOCOL_TYPE_REG,                                            7,                            SPD_INDEX_SDRAM_MODULE_TYPE                                                                                                             },
  {SPD_KEY_BYTE_MODULE_TYPE_REG,                                                              0,                            SPD_KEY_BYTE_MODULE_TYPE_REG,                                                               3,                            SPD_INDEX_BASE_MODULE_TYPE                                                                                                              },
  {SPD_KEY_BYTE_MODULE_TYPE_REG,                                                              4,                            SPD_KEY_BYTE_MODULE_TYPE_REG,                                                               6,                            SPD_INDEX_HYBRID_MEDIA                                                                                                                  },
  {SPD_KEY_BYTE_MODULE_TYPE_REG,                                                              7,                            SPD_KEY_BYTE_MODULE_TYPE_REG,                                                               7,                            SPD_INDEX_HYBRID                                                                                                                        },
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
  {SPD_SDRAM_OPTIONAL_FEATURES_REG,                                                           5,                            SPD_SDRAM_OPTIONAL_FEATURES_REG,                                                            5,                            SPD_INDEX_SOFT_PPR                                                                                                                      },
  {SPD_SDRAM_OPTIONAL_FEATURES_REG,                                                           6,                            SPD_SDRAM_OPTIONAL_FEATURES_REG,                                                            7,                            SPD_INDEX_POST_PACKAGE_REPAIR_PPR                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {SPD_COMMON_MODULE_ORGANIZATION_REG,                                                        6,                            SPD_COMMON_MODULE_ORGANIZATION_REG,                                                         6,                            SPD_INDEX_RANK_MIX                                                                                                                      },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {SPD_SDRAM_CAS_LATENCIES_SUPPORTED_0_REG,                                                   0,                            SPD_SDRAM_CAS_LATENCIES_SUPPORTED_0_REG,                                                    7,                            SPD_INDEX_CL_FIRST_BYTE                                                                                                                 },
  {SPD_SDRAM_CAS_LATENCIES_SUPPORTED_1_REG,                                                   0,                            SPD_SDRAM_CAS_LATENCIES_SUPPORTED_1_REG,                                                    7,                            SPD_INDEX_CL_SECOND_BYTE                                                                                                                },
  {SPD_SDRAM_CAS_LATENCIES_SUPPORTED_2_REG,                                                   0,                            SPD_SDRAM_CAS_LATENCIES_SUPPORTED_2_REG,                                                    7,                            SPD_INDEX_CL_THIRD_BYTE                                                                                                                 },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {SPD_SDRAM_CAS_LATENCIES_SUPPORTED_3_REG,                                                   0,                            SPD_SDRAM_CAS_LATENCIES_SUPPORTED_3_REG,                                                    7,                            SPD_INDEX_CL_FOURTH_BYTE                                                                                                                },
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
  {SPD_CRC_FOR_BASE_CONFIGURATION_SECTION_LSB_REG,                                            0,                            SPD_CRC_FOR_BASE_CONFIGURATION_SECTION_LSB_REG,                                             7,                            SPD_INDEX_SPD_CRC_FOR_BASE_CONFIGURATION_SECTION_LSB_REG                                                                                },
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
  {SPD_COMMON_DIMM_ATTRIBUTES_REG,                                                            2,                            SPD_COMMON_DIMM_ATTRIBUTES_REG,                                                             2,                            SPD_INDEX_HEAT_SPREADER                                                                                                                 },
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
  {SPD_MODULE_MANUFACTURER_ID_CODE_0_REG,                                                     0,                            SPD_MODULE_MANUFACTURER_ID_CODE_0_REG,                                                      6,                            SPD_INDEX_NUMBER_OF_CONTINUATION_CODES_MODULE_MANUFACTURER                                                                              },
  {SPD_MODULE_MANUFACTURER_ID_CODE_0_REG,                                                     7,                            SPD_MODULE_MANUFACTURER_ID_CODE_0_REG,                                                      7,                            SPD_INDEX_PD_INDEX_ODD_PARITY_BITS_6_0                                                                                                  },
  {SPD_MODULE_MANUFACTURER_ID_CODE_1_REG,                                                     0,                            SPD_MODULE_MANUFACTURER_ID_CODE_1_REG,                                                      7,                            SPD_INDEX_LAST_NON_ZERO_BYTE_MODULE_MANUFACTURER                                                                                        },
  {SPD_MODULE_MANUFACTURING_LOCATION_REG,                                                     0,                            SPD_MODULE_MANUFACTURING_LOCATION_REG,                                                      7,                            SPD_INDEX_SPD_MODULE_MANUFACTURING_LOCATION_REG                                                                                         },
  {SPD_MODULE_MANUFACTURING_DATE_0_REG,                                                       0,                            SPD_MODULE_MANUFACTURING_DATE_1_REG,                                                        7,                            SPD_INDEX_SPD_MODULE_MANUFACTURING_DATE_REG                                                                                             },
  {SPD_MODULE_SERIAL_NUMBER_0_REG,                                                            0,                            SPD_MODULE_SERIAL_NUMBER_3_REG,                                                             7,                            SPD_INDEX_SPD_MODULE_SERIAL_NUMBER_REG                                                                                                  },
  {SPD_MODULE_PART_NUMBER_0_REG,                                                              0,                            SPD_MODULE_PART_NUMBER_29_REG,                                                              7,                            SPD_INDEX_SPD_MODULE_PART_NUMBER_REG                                                                                                    },
  {SPD_MODULE_REVISION_CODE_REG,                                                              0,                            SPD_MODULE_REVISION_CODE_REG,                                                               7,                            SPD_INDEX_SPD_MODULE_REVISION_CODE_REG                                                                                                  },
  {SPD_DRAM_MANUFACTURER_ID_CODE_0_REG,                                                       0,                            SPD_DRAM_MANUFACTURER_ID_CODE_0_REG,                                                        6,                            SPD_INDEX_NUMBER_OF_CONTINUATION_CODES_DRAM_MANUFACTURER                                                                                },
  {NO_SPD_BYTE_NUMBER,                                                                        NO_SPD_START_BIT,             NO_SPD_BYTE_NUMBER,                                                                         NO_SPD_END_BIT,               NO_SPD_FIELD_INDEX                                                                                                                       },
  {SPD_DRAM_MANUFACTURER_ID_CODE_1_REG,                                                       0,                            SPD_DRAM_MANUFACTURER_ID_CODE_1_REG,                                                        7,                            SPD_INDEX_LAST_NON_ZERO_BYTE_DRAM_MANUFACTURER                                                                                          },
  {SPD_DRAM_STEPPING_REG,                                                                     0,                            SPD_DRAM_STEPPING_REG,                                                                      7,                            SPD_INDEX_DRAM_STEPPING                                                                                                                 },
  {SPD_MODULE_MANUFACTURING_DATE_0_REG,                                                       0,                            SPD_MODULE_MANUFACTURING_DATE_0_REG,                                                        7,                            SPD_INDEX_SPD_MANUFACTURER_SPECIFIC_DATA_REG                                                                                            },
  {SPD_FIRST_SDRAM_DENSITY_AND_PACKAGE_REG,                                                   0,                            SPD_FIRST_SDRAM_DENSITY_AND_PACKAGE_REG,                                                    4,                            SPD_INDEX_SDRAM_DENSITY_PER_DIE_FIRST_SDRAM_DENSITY_AND_PACKAGE                                                                         },
  {SPD_FIRST_SDRAM_DENSITY_AND_PACKAGE_REG,                                                   5,                            SPD_FIRST_SDRAM_DENSITY_AND_PACKAGE_REG,                                                    7,                            SPD_INDEX_DIE_PER_PACKAGE_FIRST_SDRAM_DENSITY_AND_PACKAGE                                                                               },
  {SPD_FIRST_SDRAM_ADDRESSING_REG,                                                            0,                            SPD_FIRST_SDRAM_ADDRESSING_REG,                                                             4,                            SPD_INDEX_FIRST_SDRAM_ROW_ADDRESS_BITS                                                                                                  },
  {SPD_FIRST_SDRAM_ADDRESSING_REG,                                                            5,                            SPD_FIRST_SDRAM_ADDRESSING_REG,                                                             7,                            SPD_INDEX_FIRST_SDRAM_COLUMN_ADDRESS_BITS                                                                                               },
  {SPD_FIRST_SDRAM_I_O_WIDTH_REG,                                                             5,                            SPD_FIRST_SDRAM_I_O_WIDTH_REG,                                                              7,                            SPD_INDEX_SDRAM_I_O_WIDTH_FIRST_SDRAM_I_O_WIDTH                                                                                         },
  {SPD_FIRST_SDRAM_BANK_GROUPS_BANKS_PER_BANK_GROUP_REG,                                      0,                            SPD_FIRST_SDRAM_BANK_GROUPS_BANKS_PER_BANK_GROUP_REG,                                       2,                            SPD_INDEX_FIRST_SDRAM_BANKS_PER_BANK_GROUP                                                                                              },
  {SPD_FIRST_SDRAM_BANK_GROUPS_BANKS_PER_BANK_GROUP_REG,                                      5,                            SPD_FIRST_SDRAM_BANK_GROUPS_BANKS_PER_BANK_GROUP_REG,                                       7,                            SPD_INDEX_FIRST_SDRAM_BANK_GROUPS                                                                                                       },
  {SPD_SECOND_SDRAM_DENSITY_AND_PACKAGE_REG,                                                  0,                            SPD_SECOND_SDRAM_DENSITY_AND_PACKAGE_REG,                                                   4,                            SPD_INDEX_SDRAM_DENSITY_PER_DIE_SECOND_SDRAM_DENSITY_AND_PACKAGE                                                                        },
  {SPD_SECOND_SDRAM_DENSITY_AND_PACKAGE_REG,                                                  5,                            SPD_SECOND_SDRAM_DENSITY_AND_PACKAGE_REG,                                                   7,                            SPD_INDEX_DIE_PER_PACKAGE_SECOND_SDRAM_DENSITY_AND_PACKAGE                                                                              },
  {SPD_SECOND_SDRAM_ADDRESSING_REG,                                                           0,                            SPD_SECOND_SDRAM_ADDRESSING_REG,                                                            4,                            SPD_INDEX_SECOND_SDRAM_ROW_ADDRESS_BITS                                                                                                 },
  {SPD_SECOND_SDRAM_ADDRESSING_REG,                                                           5,                            SPD_SECOND_SDRAM_ADDRESSING_REG,                                                            7,                            SPD_INDEX_SECOND_SDRAM_COLUMN_ADDRESS_BITS                                                                                              },
  {SPD_SECONDARY_SDRAM_I_O_WIDTH_REG,                                                         5,                            SPD_SECONDARY_SDRAM_I_O_WIDTH_REG,                                                          7,                            SPD_INDEX_SDRAM_I_O_WIDTH_SECONDARY_SDRAM_I_O_WIDTH                                                                                     },
  {SPD_SECOND_SDRAM_BANK_GROUPS_BANKS_PER_BANK_GROUP_REG,                                     0,                            SPD_SECOND_SDRAM_BANK_GROUPS_BANKS_PER_BANK_GROUP_REG,                                      2,                            SPD_INDEX_SECOND_SDRAM_BANKS_PER_BANK_GROUP                                                                                             },
  {SPD_SECOND_SDRAM_BANK_GROUPS_BANKS_PER_BANK_GROUP_REG,                                     5,                            SPD_SECOND_SDRAM_BANK_GROUPS_BANKS_PER_BANK_GROUP_REG,                                      7,                            SPD_INDEX_SECOND_SDRAM_BANK_GROUPS                                                                                                      },
  {SPD_SDRAM_OPTIONAL_FEATURES_REG,                                                           4,                            SPD_SDRAM_OPTIONAL_FEATURES_REG,                                                            4,                            SPD_INDEX_BL32                                                                                                                          },
  {SPD_SDRAM_NOMINAL_VOLTAGE_VDD_REG,                                                         0,                            SPD_SDRAM_NOMINAL_VOLTAGE_VDD_REG,                                                          1,                            SPD_INDEX_ENDURANT_NOMINAL_VOLTAGE_VDD                                                                                                  },
  {SPD_SDRAM_NOMINAL_VOLTAGE_VDD_REG,                                                         2,                            SPD_SDRAM_NOMINAL_VOLTAGE_VDD_REG,                                                          3,                            SPD_INDEX_OPERABLE_NOMINAL_VOLTAGE_VDD                                                                                                  },
  {SPD_SDRAM_NOMINAL_VOLTAGE_VDD_REG,                                                         4,                            SPD_SDRAM_NOMINAL_VOLTAGE_VDD_REG,                                                          7,                            SPD_INDEX_NOMINAL_NOMINAL_VOLTAGE_VDD                                                                                                   },
  {SPD_SDRAM_NOMINAL_VOLTAGE_VDDQ_REG,                                                        0,                            SPD_SDRAM_NOMINAL_VOLTAGE_VDDQ_REG,                                                         1,                            SPD_INDEX_ENDURANT_NOMINAL_VOLTAGE_VDDQ                                                                                                 },
  {SPD_SDRAM_NOMINAL_VOLTAGE_VDDQ_REG,                                                        2,                            SPD_SDRAM_NOMINAL_VOLTAGE_VDDQ_REG,                                                         3,                            SPD_INDEX_OPERABLE_NOMINAL_VOLTAGE_VDDQ                                                                                                 },
  {SPD_SDRAM_NOMINAL_VOLTAGE_VDDQ_REG,                                                        4,                            SPD_SDRAM_NOMINAL_VOLTAGE_VDDQ_REG,                                                         7,                            SPD_INDEX_NOMINAL_NOMINAL_VOLTAGE_VDDQ                                                                                                  },
  {SPD_SDRAM_NOMINAL_VOLTAGE_VPP_REG,                                                         0,                            SPD_SDRAM_NOMINAL_VOLTAGE_VPP_REG,                                                          1,                            SPD_INDEX_ENDURANT_NOMINAL_VOLTAGE_VPP                                                                                                  },
  {SPD_SDRAM_NOMINAL_VOLTAGE_VPP_REG,                                                         2,                            SPD_SDRAM_NOMINAL_VOLTAGE_VPP_REG,                                                          3,                            SPD_INDEX_OPERABLE_NOMINAL_VOLTAGE_VPP                                                                                                  },
  {SPD_SDRAM_NOMINAL_VOLTAGE_VPP_REG,                                                         4,                            SPD_SDRAM_NOMINAL_VOLTAGE_VPP_REG,                                                          7,                            SPD_INDEX_NOMINAL_NOMINAL_VOLTAGE_VPP                                                                                                   },
  {SPD_SDRAM_MINIMUM_CYCLE_TIME_TCKAVGMIN_LSB_REG,                                            0,                            SPD_SDRAM_MINIMUM_CYCLE_TIME_TCKAVGMIN_LSB_REG,                                             7,                            SPD_INDEX_SDRAM_MINIMUM_CYCLE_TIME_TCKAVGMIN_PS_LSB                                                                                     },
  {SPD_SDRAM_MINIMUM_CYCLE_TIME_TCKAVGMIN_MSB_REG,                                            0,                            SPD_SDRAM_MINIMUM_CYCLE_TIME_TCKAVGMIN_MSB_REG,                                             7,                            SPD_INDEX_SDRAM_MINIMUM_CYCLE_TIME_TCKAVGMIN_PS_MSB                                                                                     },
  {SPD_SDRAM_MAXIMUM_CYCLE_TIME_TCKAVGMAX_LSB_REG,                                            0,                            SPD_SDRAM_MAXIMUM_CYCLE_TIME_TCKAVGMAX_LSB_REG,                                             7,                            SPD_INDEX_SDRAM_MAXIMUM_CYCLE_TIME_TCKAVGMAX_PS_LSB                                                                                     },
  {SPD_SDRAM_MAXIMUM_CYCLE_TIME_TCKAVGMAX_MSB_REG,                                            0,                            SPD_SDRAM_MAXIMUM_CYCLE_TIME_TCKAVGMAX_MSB_REG,                                             7,                            SPD_INDEX_SDRAM_MAXIMUM_CYCLE_TIME_TCKAVGMAX_PS_MSB                                                                                     },
  {SPD_SDRAM_CAS_LATENCIES_SUPPORTED_4_REG,                                                   0,                            SPD_SDRAM_CAS_LATENCIES_SUPPORTED_4_REG,                                                    7,                            SPD_INDEX_CL_FIFTH_BYTE                                                                                                                 },
  {SPD_SDRAM_MINIMUM_CAS_LATENCY_TIME_TAAMIN_LSB_REG,                                         0,                            SPD_SDRAM_MINIMUM_CAS_LATENCY_TIME_TAAMIN_LSB_REG,                                          7,                            SPD_INDEX_SDRAM_MINIMUM_CAS_LATENCY_TIME_TAAMIN_PS_LSB                                                                                  },
  {SPD_SDRAM_MINIMUM_CAS_LATENCY_TIME_TAAMIN_MSB_REG,                                         0,                            SPD_SDRAM_MINIMUM_CAS_LATENCY_TIME_TAAMIN_MSB_REG,                                          7,                            SPD_INDEX_SDRAM_MINIMUM_CAS_LATENCY_TIME_TAAMIN_PS_MSB                                                                                  },
  {SPD_SDRAM_MINIMUM_RAS_TO_CAS_DELAY_TIME_TRCDMIN_LSB_REG,                                   0,                            SPD_SDRAM_MINIMUM_RAS_TO_CAS_DELAY_TIME_TRCDMIN_LSB_REG,                                    7,                            SPD_INDEX_SDRAM_MINIMUM_RAS_TO_CAS_DELAY_TIME_TRCDMIN_PS_LSB                                                                            },
  {SPD_SDRAM_MINIMUM_RAS_TO_CAS_DELAY_TIME_TRCDMIN_MSB_REG,                                   0,                            SPD_SDRAM_MINIMUM_RAS_TO_CAS_DELAY_TIME_TRCDMIN_MSB_REG,                                    7,                            SPD_INDEX_SDRAM_MINIMUM_RAS_TO_CAS_DELAY_TIME_TRCDMIN_PS_MSB                                                                            },
  {SPD_SDRAM_MINIMUM_ROW_PRECHARGE_DELAY_TIME_TRPMIN_LSB_REG,                                 0,                            SPD_SDRAM_MINIMUM_ROW_PRECHARGE_DELAY_TIME_TRPMIN_LSB_REG,                                  7,                            SPD_INDEX_SDRAM_MINIMUM_ROW_PRECHARGE_DELAY_TIME_TRPMIN_PS_LSB                                                                          },
  {SPD_SDRAM_MINIMUM_ROW_PRECHARGE_DELAY_TIME_TRPMIN_MSB_REG,                                 0,                            SPD_SDRAM_MINIMUM_ROW_PRECHARGE_DELAY_TIME_TRPMIN_MSB_REG,                                  7,                            SPD_INDEX_SDRAM_MINIMUM_ROW_PRECHARGE_DELAY_TIME_TRPMIN_PS_MSB                                                                          },
  {SPD_SDRAM_MIN_ACTIVE_TO_PRECHARGE_DELAY_TRAS_LSB_REG,                                      0,                            SPD_SDRAM_MIN_ACTIVE_TO_PRECHARGE_DELAY_TRAS_LSB_REG,                                       7,                            SPD_INDEX_SDRAM_MIN_ACTIVE_TO_PRECHARGE_DELAY_TRAS_LSB                                                                                  },
  {SPD_SDRAM_MIN_ACTIVE_TO_PRECHARGE_DELAY_TRAS_MSB_REG,                                      0,                            SPD_SDRAM_MIN_ACTIVE_TO_PRECHARGE_DELAY_TRAS_MSB_REG,                                       7,                            SPD_INDEX_SDRAM_MIN_ACTIVE_TO_PRECHARGE_DELAY_TRAS_MSB                                                                                  },
  {SPD_SDRAM_MIN_ACTIVE_TO_ACTIVE_REFRESH_DELAY_TRC_LSB_REG,                                  0,                            SPD_SDRAM_MIN_ACTIVE_TO_ACTIVE_REFRESH_DELAY_TRC_LSB_REG,                                   7,                            SPD_INDEX_SDRAM_MIN_ACTIVE_TO_ACTIVE_REFRESH_DELAY_TRC_LSB                                                                              },
  {SPD_SDRAM_MIN_ACTIVE_TO_ACTIVE_REFRESH_DELAY_TRC_MSB_REG,                                  0,                            SPD_SDRAM_MIN_ACTIVE_TO_ACTIVE_REFRESH_DELAY_TRC_MSB_REG,                                   7,                            SPD_INDEX_SDRAM_MIN_ACTIVE_TO_ACTIVE_REFRESH_DELAY_TRC_MSB                                                                              },
  {SPD_SDRAM_MINIMUM_WRITE_RECOVERY_TIME_TWRMIN_LSB_REG,                                      0,                            SPD_SDRAM_MINIMUM_WRITE_RECOVERY_TIME_TWRMIN_LSB_REG,                                       7,                            SPD_INDEX_SDRAM_MINIMUM_WRITE_RECOVERY_TIME_TWRMIN_PS_LSB                                                                               },
  {SPD_SDRAM_MINIMUM_WRITE_RECOVERY_TIME_TWRMIN_MSB_REG,                                      0,                            SPD_SDRAM_MINIMUM_WRITE_RECOVERY_TIME_TWRMIN_MSB_REG,                                       7,                            SPD_INDEX_SDRAM_MINIMUM_WRITE_RECOVERY_TIME_TWRMIN_PS_MSB                                                                               },
  {SPD_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC1_SLR_LSB_REG,                                    0,                            SPD_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC1_SLR_LSB_REG,                                     7,                            SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC1_SLR_LSB                                                                                },
  {SPD_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC1_SLR_MSB_REG,                                    0,                            SPD_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC1_SLR_MSB_REG,                                     7,                            SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC1_SLR_MSB                                                                                },
  {SPD_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC2_SLR_LSB_REG,                                    0,                            SPD_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC2_SLR_LSB_REG,                                     7,                            SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC2_SLR_LSB                                                                                },
  {SPD_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC2_SLR_MSB_REG,                                    0,                            SPD_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC2_SLR_MSB_REG,                                     7,                            SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC2_SLR_MSB                                                                                },
  {SPD_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFCSB_SLR_LSB_REG,                                   0,                            SPD_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFCSB_SLR_LSB_REG,                                    7,                            SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFCSB_LSB                                                                                   },
  {SPD_SDRAM_MIN_REFRESH_RECOVERY_DELAYE_TRFCSB_SLR_MSB_REG,                                  0,                            SPD_SDRAM_MIN_REFRESH_RECOVERY_DELAYE_TRFCSB_SLR_MSB_REG,                                   7,                            SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFCSB_MSB                                                                                   },
  {SPD_MIN_REFRESH_RECOVERY_DELAY_3DS_DIFF_LOGIC_RANK_TRFC1_LSB_REG,                          0,                            SPD_MIN_REFRESH_RECOVERY_DELAY_3DS_DIFF_LOGIC_RANK_TRFC1_LSB_REG,                           7,                            SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC1_DLR_LSB                                                                                },
  {SPD_MIN_REFRESH_RECOVERY_DELAY_3DS_DIFF_LOGIC_RANK_TRFC1_MSB_REG,                          0,                            SPD_MIN_REFRESH_RECOVERY_DELAY_3DS_DIFF_LOGIC_RANK_TRFC1_MSB_REG,                           7,                            SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC1_DLR_MSB                                                                                },
  {SPD_MIN_REFRESH_RECOVERY_DELAY_3DS_DIFF_LOGIC_RANK_TRFC2_LSB_REG,                          0,                            SPD_MIN_REFRESH_RECOVERY_DELAY_3DS_DIFF_LOGIC_RANK_TRFC2_LSB_REG,                           7,                            SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC2_DLR_LSB                                                                                },
  {SPD_MIN_REFRESH_RECOVERY_DELAY_3DS_DIFF_LOGIC_RANK_TRFC2_MSB_REG,                          0,                            SPD_MIN_REFRESH_RECOVERY_DELAY_3DS_DIFF_LOGIC_RANK_TRFC2_MSB_REG,                           7,                            SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC2_DLR_MSB                                                                                },
  {SPD_MIN_REFRESH_RECOVERY_DELAY_3DS_DIFF_LOGIC_RANK_TRFCSB_LSB_REG,                         0,                            SPD_MIN_REFRESH_RECOVERY_DELAY_3DS_DIFF_LOGIC_RANK_TRFCSB_LSB_REG,                          7,                            SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFCSB_DLR_LSB                                                                               },
  {SPD_MIN_REFRESH_RECOVERY_DELAY_3DS_DIFF_LOGIC_RANK_TRFCSB_MSB_REG,                         0,                            SPD_MIN_REFRESH_RECOVERY_DELAY_3DS_DIFF_LOGIC_RANK_TRFCSB_MSB_REG,                          7,                            SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFCSB_DLR_MSB                                                                               },
  {SPD_PRIMARY_SDRAM_REFRESH_MANAGEMENT_0_REG,                                                0,                            SPD_PRIMARY_SDRAM_REFRESH_MANAGEMENT_0_REG,                                                 0,                            SPD_INDEX_RFM_REQUIRED_PRIMARY_SDRAM_REFRESH_MANAGEMENT_0                                                                               },
  {SPD_PRIMARY_SDRAM_REFRESH_MANAGEMENT_0_REG,                                                1,                            SPD_PRIMARY_SDRAM_REFRESH_MANAGEMENT_0_REG,                                                 4,                            SPD_INDEX_ROLLING_ACCUMULATED_ACT_INITIAL_MANAGEMENT_THRESHOLD_RAAIMT_PRIMARY_SDRAM_REFRESH_MANAGEMENT_0                                },
  {SPD_PRIMARY_SDRAM_REFRESH_MANAGEMENT_0_REG,                                                5,                            SPD_PRIMARY_SDRAM_REFRESH_MANAGEMENT_0_REG,                                                 7,                            SPD_INDEX_ROLLING_ACCUMULATED_ACT_MAXIMUM_MANAGEMENT_THRESHOLD_RAAMMT_PRIMARY_SDRAM_REFRESH_MANAGEMENT_0                                },
  {SPD_PRIMARY_SDRAM_REFRESH_MANAGEMENT_1_REG,                                                6,                            SPD_PRIMARY_SDRAM_REFRESH_MANAGEMENT_1_REG,                                                 7,                            SPD_INDEX_RFM_RAA_COUNTER_DECREMENT_PER_REF_COMMAND_PRIMARY_SDRAM_REFRESH_MANAGEMENT_1                                                  },
  {SPD_SECONDARY_SDRAM_REFRESH_MANAGEMENT_0_REG,                                              0,                            SPD_SECONDARY_SDRAM_REFRESH_MANAGEMENT_0_REG,                                               0,                            SPD_INDEX_RFM_REQUIRED_SECONDARY_SDRAM_REFRESH_MANAGEMENT_0                                                                             },
  {SPD_SECONDARY_SDRAM_REFRESH_MANAGEMENT_0_REG,                                              1,                            SPD_SECONDARY_SDRAM_REFRESH_MANAGEMENT_0_REG,                                               4,                            SPD_INDEX_ROLLING_ACCUMULATED_ACT_INITIAL_MANAGEMENT_THRESHOLD_RAAIMT_SECONDARY_SDRAM_REFRESH_MANAGEMENT_0                              },
  {SPD_SECONDARY_SDRAM_REFRESH_MANAGEMENT_0_REG,                                              5,                            SPD_SECONDARY_SDRAM_REFRESH_MANAGEMENT_0_REG,                                               7,                            SPD_INDEX_ROLLING_ACCUMULATED_ACT_MAXIMUM_MANAGEMENT_THRESHOLD_RAAMMT_SECONDARY_SDRAM_REFRESH_MANAGEMENT_0                              },
  {SPD_SECONDARY_SDRAM_REFRESH_MANAGEMENT_1_REG,                                              6,                            SPD_SECONDARY_SDRAM_REFRESH_MANAGEMENT_1_REG,                                               7,                            SPD_INDEX_RFM_RAA_COUNTER_DECREMENT_PER_REF_COMMAND_SECONDARY_SDRAM_REFRESH_MANAGEMENT_1                                                },
  {SPD_CRC_FOR_BASE_CONFIGURATION_SECTION_MSB_REG,                                            0,                            SPD_CRC_FOR_BASE_CONFIGURATION_SECTION_MSB_REG,                                             7,                            SPD_INDEX_SPD_CRC_FOR_BASE_CONFIGURATION_SECTION_MSB_REG                                                                                },
  {SPD_COMMON_SPD_REVISION_FOR_MODULE_INFORMATION_REG,                                        0,                            SPD_COMMON_SPD_REVISION_FOR_MODULE_INFORMATION_REG,                                         7,                            SPD_INDEX_SPD_COMMON_SPD_REVISION_FOR_MODULE_INFORMATION_REG                                                                            },
  {SPD_COMMON_SPD_MANUFACTURER_ID_CODE_0_REG,                                                 0,                            SPD_COMMON_SPD_MANUFACTURER_ID_CODE_0_REG,                                                  7,                            SPD_INDEX_SPD_COMMON_SPD_MANUFACTURER_ID_CODE_0_REG                                                                                     },
  {SPD_COMMON_SPD_MANUFACTURER_ID_CODE_1_REG,                                                 0,                            SPD_COMMON_SPD_MANUFACTURER_ID_CODE_1_REG,                                                  7,                            SPD_INDEX_SPD_COMMON_SPD_MANUFACTURER_ID_CODE_1_REG                                                                                     },
  {SPD_COMMON_SPD_DEVICE_TYPE_REG,                                                            0,                            SPD_COMMON_SPD_DEVICE_TYPE_REG,                                                             3,                            SPD_INDEX_DEVICE_TYPE_COMMON_DEVICE_TYPE                                                                                                },
  {SPD_COMMON_SPD_DEVICE_TYPE_REG,                                                            6,                            SPD_COMMON_SPD_DEVICE_TYPE_REG,                                                             7,                            SPD_INDEX_DEVICES_INSTALLED_COMMON_DEVICE_TYPE                                                                                          },
  {SPD_COMMON_SPD_DEVICE_REVISION_REG,                                                        0,                            SPD_COMMON_SPD_DEVICE_REVISION_REG,                                                         3,                            SPD_INDEX_MINOR_REVISION_COMMON_DEVICE_REVISION                                                                                         },
  {SPD_COMMON_SPD_DEVICE_REVISION_REG,                                                        4,                            SPD_COMMON_SPD_DEVICE_REVISION_REG,                                                         7,                            SPD_INDEX_MAJOR_REVISION_COMMON_DEVICE_REVISION                                                                                         },
  {SPD_COMMON_PMIC_0_MANUFACTURER_ID_CODE_0_REG,                                              0,                            SPD_COMMON_PMIC_0_MANUFACTURER_ID_CODE_0_REG,                                               7,                            SPD_INDEX_SPD_COMMON_PMIC_0_MANUFACTURER_ID_CODE_0_REG                                                                                  },
  {SPD_COMMON_PMIC_0_MANUFACTURER_ID_CODE_1_REG,                                              0,                            SPD_COMMON_PMIC_0_MANUFACTURER_ID_CODE_1_REG,                                               7,                            SPD_INDEX_SPD_COMMON_PMIC_0_MANUFACTURER_ID_CODE_1_REG                                                                                  },
  {SPD_COMMON_PMIC_0_DEVICE_TYPE_REG,                                                         0,                            SPD_COMMON_PMIC_0_DEVICE_TYPE_REG,                                                          3,                            SPD_INDEX_DEVICE_TYPE_COMMON_PMIC_0_DEVICE_TYPE                                                                                         },
  {SPD_COMMON_PMIC_0_DEVICE_TYPE_REG,                                                         6,                            SPD_COMMON_PMIC_0_DEVICE_TYPE_REG,                                                          7,                            SPD_INDEX_DEVICES_INSTALLED_COMMON_PMIC_0_DEVICE_TYPE                                                                                   },
  {SPD_COMMON_PMIC_0_DEVICE_REVISION_REG,                                                     0,                            SPD_COMMON_PMIC_0_DEVICE_REVISION_REG,                                                      3,                            SPD_INDEX_MINOR_REVISION_COMMON_PMIC_0_DEVICE_REVISION                                                                                  },
  {SPD_COMMON_PMIC_0_DEVICE_REVISION_REG,                                                     4,                            SPD_COMMON_PMIC_0_DEVICE_REVISION_REG,                                                      7,                            SPD_INDEX_MAJOR_REVISION_COMMON_PMIC_0_DEVICE_REVISION                                                                                  },
  {SPD_COMMON_PMIC_1_MANUFACTURER_ID_CODE_0_REG,                                              0,                            SPD_COMMON_PMIC_1_MANUFACTURER_ID_CODE_0_REG,                                               7,                            SPD_INDEX_SPD_COMMON_PMIC_1_MANUFACTURER_ID_CODE_0_REG                                                                                  },
  {SPD_COMMON_PMIC_1_MANUFACTURER_ID_CODE_1_REG,                                              0,                            SPD_COMMON_PMIC_1_MANUFACTURER_ID_CODE_1_REG,                                               7,                            SPD_INDEX_SPD_COMMON_PMIC_1_MANUFACTURER_ID_CODE_1_REG                                                                                  },
  {SPD_COMMON_PMIC_1_DEVICE_TYPE_REG,                                                         0,                            SPD_COMMON_PMIC_1_DEVICE_TYPE_REG,                                                          3,                            SPD_INDEX_DEVICE_TYPE_COMMON_PMIC_1_DEVICE_TYPE                                                                                         },
  {SPD_COMMON_PMIC_1_DEVICE_TYPE_REG,                                                         6,                            SPD_COMMON_PMIC_1_DEVICE_TYPE_REG,                                                          7,                            SPD_INDEX_DEVICES_INSTALLED_COMMON_PMIC_1_DEVICE_TYPE                                                                                   },
  {SPD_COMMON_PMIC_1_DEVICE_REVISION_REG,                                                     0,                            SPD_COMMON_PMIC_1_DEVICE_REVISION_REG,                                                      3,                            SPD_INDEX_MINOR_REVISION_COMMON_PMIC_1_DEVICE_REVISION                                                                                  },
  {SPD_COMMON_PMIC_1_DEVICE_REVISION_REG,                                                     4,                            SPD_COMMON_PMIC_1_DEVICE_REVISION_REG,                                                      7,                            SPD_INDEX_MAJOR_REVISION_COMMON_PMIC_1_DEVICE_REVISION                                                                                  },
  {SPD_COMMON_PMIC_2_MANUFACTURER_ID_CODE_0_REG,                                              0,                            SPD_COMMON_PMIC_2_MANUFACTURER_ID_CODE_0_REG,                                               7,                            SPD_INDEX_SPD_COMMON_PMIC_2_MANUFACTURER_ID_CODE_0_REG                                                                                  },
  {SPD_COMMON_PMIC_2_MANUFACTURER_ID_CODE_1_REG,                                              0,                            SPD_COMMON_PMIC_2_MANUFACTURER_ID_CODE_1_REG,                                               7,                            SPD_INDEX_SPD_COMMON_PMIC_2_MANUFACTURER_ID_CODE_1_REG                                                                                  },
  {SPD_COMMON_PMIC_2_DEVICE_TYPE_REG,                                                         0,                            SPD_COMMON_PMIC_2_DEVICE_TYPE_REG,                                                          3,                            SPD_INDEX_DEVICE_TYPE_COMMON_PMIC_2_DEVICE_TYPE                                                                                         },
  {SPD_COMMON_PMIC_2_DEVICE_TYPE_REG,                                                         6,                            SPD_COMMON_PMIC_2_DEVICE_TYPE_REG,                                                          7,                            SPD_INDEX_DEVICES_INSTALLED_COMMON_PMIC_2_DEVICE_TYPE                                                                                   },
  {SPD_COMMON_PMIC_2_DEVICE_REVISION_REG,                                                     0,                            SPD_COMMON_PMIC_2_DEVICE_REVISION_REG,                                                      3,                            SPD_INDEX_MINOR_REVISION_COMMON_PMIC_2_DEVICE_REVISION                                                                                  },
  {SPD_COMMON_PMIC_2_DEVICE_REVISION_REG,                                                     4,                            SPD_COMMON_PMIC_2_DEVICE_REVISION_REG,                                                      7,                            SPD_INDEX_MAJOR_REVISION_COMMON_PMIC_2_DEVICE_REVISION                                                                                  },
  {SPD_COMMON_THERMAL_SENSORS_TS_MANUFACTURER_ID_CODE_0_REG,                                  0,                            SPD_COMMON_THERMAL_SENSORS_TS_MANUFACTURER_ID_CODE_0_REG,                                   7,                            SPD_INDEX_SPD_COMMON_THERMAL_SENSORS_TS_MANUFACTURER_ID_CODE_0_REG                                                                      },
  {SPD_COMMON_THERMAL_SENSORS_TS_MANUFACTURER_ID_CODE_1_REG,                                  0,                            SPD_COMMON_THERMAL_SENSORS_TS_MANUFACTURER_ID_CODE_1_REG,                                   7,                            SPD_INDEX_SPD_COMMON_THERMAL_SENSORS_TS_MANUFACTURER_ID_CODE_1_REG                                                                      },
  {SPD_COMMON_THERMAL_SENSORS_TS_DEVICE_TYPE_REG,                                             0,                            SPD_COMMON_THERMAL_SENSORS_TS_DEVICE_TYPE_REG,                                              3,                            SPD_INDEX_DEVICE_TYPE_COMMON_THERMAL_SENSORS_TS_DEVICE_TYPE                                                                             },
  {SPD_COMMON_THERMAL_SENSORS_TS_DEVICE_TYPE_REG,                                             6,                            SPD_COMMON_THERMAL_SENSORS_TS_DEVICE_TYPE_REG,                                              7,                            SPD_INDEX_DEVICES_INSTALLED_COMMON_THERMAL_SENSORS_TS_DEVICE_TYPE                                                                       },
  {SPD_COMMON_THERMAL_SENSORS_TS_DEVICE_REVISION_REG,                                         0,                            SPD_COMMON_THERMAL_SENSORS_TS_DEVICE_REVISION_REG,                                          3,                            SPD_INDEX_MINOR_REVISION_COMMON_THERMAL_SENSORS_TS_DEVICE_REVISION                                                                      },
  {SPD_COMMON_THERMAL_SENSORS_TS_DEVICE_REVISION_REG,                                         4,                            SPD_COMMON_THERMAL_SENSORS_TS_DEVICE_REVISION_REG,                                          7,                            SPD_INDEX_MAJOR_REVISION_COMMON_THERMAL_SENSORS_TS_DEVICE_REVISION                                                                      },
  {SPD_COMMON_MODULE_NOMINAL_HEIGHT_REG,                                                      0,                            SPD_COMMON_MODULE_NOMINAL_HEIGHT_REG,                                                       4,                            SPD_INDEX_MODULE_NOMINAL_HEIGHT_MAX                                                                                                     },
  {SPD_COMMON_MODULE_MAXIMUM_THICKNESS_REG,                                                   0,                            SPD_COMMON_MODULE_MAXIMUM_THICKNESS_REG,                                                    3,                            SPD_INDEX_MODULE_MAXIMUM_THICKNESS_MAX_FRONT                                                                                            },
  {SPD_COMMON_MODULE_MAXIMUM_THICKNESS_REG,                                                   4,                            SPD_COMMON_MODULE_MAXIMUM_THICKNESS_REG,                                                    7,                            SPD_INDEX_MODULE_MAXIMUM_THICKNESS_MAX_BACK                                                                                             },
  {SPD_COMMON_REFERENCE_RAW_CARD_USED_REG,                                                    0,                            SPD_COMMON_REFERENCE_RAW_CARD_USED_REG,                                                     4,                            SPD_INDEX_REFERENCE_DESIGN                                                                                                              },
  {SPD_COMMON_REFERENCE_RAW_CARD_USED_REG,                                                    5,                            SPD_COMMON_REFERENCE_RAW_CARD_USED_REG,                                                     7,                            SPD_INDEX_DESIGN_REVISION                                                                                                               },
  {SPD_COMMON_DIMM_ATTRIBUTES_REG,                                                            0,                            SPD_COMMON_DIMM_ATTRIBUTES_REG,                                                             1,                            SPD_INDEX_OF_ROWS_OF_DRAMS_ON_MODULE                                                                                                    },
  {SPD_COMMON_DIMM_ATTRIBUTES_REG,                                                            4,                            SPD_COMMON_DIMM_ATTRIBUTES_REG,                                                             7,                            SPD_INDEX_TEMPERATURE_GRADE                                                                                                             },
  {SPD_COMMON_MODULE_ORGANIZATION_REG,                                                        3,                            SPD_COMMON_MODULE_ORGANIZATION_REG,                                                         5,                            SPD_INDEX_NUMBER_OF_PACKAGE_RANKS_PER_CHANNEL                                                                                           },
  {SPD_COMMON_MEMORY_CHANNEL_BUS_WIDTH_REG,                                                   0,                            SPD_COMMON_MEMORY_CHANNEL_BUS_WIDTH_REG,                                                    2,                            SPD_INDEX_PRIMARY_BUS_WIDTH_PER_CHANNEL_IN_BITS                                                                                         },
  {SPD_COMMON_MEMORY_CHANNEL_BUS_WIDTH_REG,                                                   3,                            SPD_COMMON_MEMORY_CHANNEL_BUS_WIDTH_REG,                                                    4,                            SPD_INDEX_BUS_WIDTH_EXTENSION_PER_CHANNEL_IN_BITS                                                                                       },
  {SPD_COMMON_MEMORY_CHANNEL_BUS_WIDTH_REG,                                                   5,                            SPD_COMMON_MEMORY_CHANNEL_BUS_WIDTH_REG,                                                    6,                            SPD_INDEX_NUMBER_OF_CHANNELS_PER_DIMM                                                                                                   },
  {SPD_RDIMM_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_0_REG,                         0,                            SPD_RDIMM_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_0_REG,                          7,                            SPD_INDEX_SPD_RDIMM_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_0_REG                                                             },
  {SPD_LRDIMM_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_0_REG,                        0,                            SPD_LRDIMM_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_0_REG,                         7,                            SPD_INDEX_SPD_LRDIMM_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_0_REG                                                            },
  {SPD_RDIMM_DIFFERENTIAL_MEMORY_BUFFER_DMB_MANUFACTURER_ID_CODE_0_REG,                       0,                            SPD_RDIMM_DIFFERENTIAL_MEMORY_BUFFER_DMB_MANUFACTURER_ID_CODE_0_REG,                        7,                            SPD_INDEX_SPD_RDIMM_DIFFERENTIAL_MEMORY_BUFFER_DMB_MANUFACTURER_ID_CODE_0_REG                                                           },
  {SPD_NVDIMM_N_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_0_REG,                      0,                            SPD_NVDIMM_N_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_0_REG,                       7,                            SPD_INDEX_SPD_NVDIMM_N_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_0_REG                                                          },
  {SPD_NVDIMM_P_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_0_REG,                      0,                            SPD_NVDIMM_P_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_0_REG,                       7,                            SPD_INDEX_SPD_NVDIMM_P_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_0_REG                                                          },
  {SPD_RDIMM_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_1_REG,                         0,                            SPD_RDIMM_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_1_REG,                          7,                            SPD_INDEX_SPD_RDIMM_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_1_REG                                                             },
  {SPD_LRDIMM_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_1_REG,                        0,                            SPD_LRDIMM_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_1_REG,                         7,                            SPD_INDEX_SPD_LRDIMM_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_1_REG                                                            },
  {SPD_RDIMM_DIFFERENTIAL_MEMORY_BUFFER_DMB_MANUFACTURER_ID_CODE_1_REG,                       0,                            SPD_RDIMM_DIFFERENTIAL_MEMORY_BUFFER_DMB_MANUFACTURER_ID_CODE_1_REG,                        7,                            SPD_INDEX_SPD_RDIMM_DIFFERENTIAL_MEMORY_BUFFER_DMB_MANUFACTURER_ID_CODE_1_REG                                                           },
  {SPD_NVDIMM_N_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_1_REG,                      0,                            SPD_NVDIMM_N_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_1_REG,                       7,                            SPD_INDEX_SPD_NVDIMM_N_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_1_REG                                                          },
  {SPD_NVDIMM_P_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_1_REG,                      0,                            SPD_NVDIMM_P_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_1_REG,                       7,                            SPD_INDEX_SPD_NVDIMM_P_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_1_REG                                                          },
  {SPD_RDIMM_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_TYPE_REG,                                    0,                            SPD_RDIMM_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_TYPE_REG,                                     3,                            SPD_INDEX_DEVICE_TYPE_RDIMM_ING_CLOCK_DRIVER_RCD_DEVICE_TYPE                                                                            },
  {SPD_RDIMM_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_TYPE_REG,                                    6,                            SPD_RDIMM_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_TYPE_REG,                                     7,                            SPD_INDEX_DEVICES_INSTALLED_RDIMM_ING_CLOCK_DRIVER_RCD_DEVICE_TYPE                                                                      },
  {SPD_LRDIMM_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_TYPE_REG,                                   0,                            SPD_LRDIMM_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_TYPE_REG,                                    3,                            SPD_INDEX_DEVICE_TYPE_LRDIMM_ING_CLOCK_DRIVER_RCD_DEVICE_TYPE                                                                           },
  {SPD_LRDIMM_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_TYPE_REG,                                   6,                            SPD_LRDIMM_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_TYPE_REG,                                    7,                            SPD_INDEX_DEVICES_INSTALLED_LRDIMM_ING_CLOCK_DRIVER_RCD_DEVICE_TYPE                                                                     },
  {SPD_RDIMM_DIFFERENTIAL_MEMORY_BUFFER_DMB_DEVICE_TYPE_REG,                                  0,                            SPD_RDIMM_DIFFERENTIAL_MEMORY_BUFFER_DMB_DEVICE_TYPE_REG,                                   3,                            SPD_INDEX_DEVICE_TYPE_RDIMM_DIFFERENTIAL_MEMORY_BUFFER_DMB_DEVICE_TYPE                                                                  },
  {SPD_RDIMM_DIFFERENTIAL_MEMORY_BUFFER_DMB_DEVICE_TYPE_REG,                                  6,                            SPD_RDIMM_DIFFERENTIAL_MEMORY_BUFFER_DMB_DEVICE_TYPE_REG,                                   7,                            SPD_INDEX_DEVICES_INSTALLED_RDIMM_DIFFERENTIAL_MEMORY_BUFFER_DMB_DEVICE_TYPE                                                            },
  {SPD_NVDIMM_N_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_TYPE_REG,                                 0,                            SPD_NVDIMM_N_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_TYPE_REG,                                  3,                            SPD_INDEX_DEVICE_TYPE_NVDIMM_N_ING_CLOCK_DRIVER_RCD_DEVICE_TYPE                                                                         },
  {SPD_NVDIMM_N_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_TYPE_REG,                                 6,                            SPD_NVDIMM_N_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_TYPE_REG,                                  7,                            SPD_INDEX_DEVICES_INSTALLED_NVDIMM_N_ING_CLOCK_DRIVER_RCD_DEVICE_TYPE                                                                   },
  {SPD_NVDIMM_P_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_TYPE_REG,                                 0,                            SPD_NVDIMM_P_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_TYPE_REG,                                  3,                            SPD_INDEX_DEVICE_TYPE_NVDIMM_P_ING_CLOCK_DRIVER_RCD_DEVICE_TYPE                                                                         },
  {SPD_NVDIMM_P_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_TYPE_REG,                                 6,                            SPD_NVDIMM_P_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_TYPE_REG,                                  7,                            SPD_INDEX_DEVICES_INSTALLED_NVDIMM_P_ING_CLOCK_DRIVER_RCD_DEVICE_TYPE                                                                   },
  {SPD_RDIMM_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_REVISION_REG,                                0,                            SPD_RDIMM_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_REVISION_REG,                                 3,                            SPD_INDEX_MINOR_REVISION_RDIMM_ING_CLOCK_DRIVER_RCD_DEVICE_REVISION                                                                     },
  {SPD_RDIMM_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_REVISION_REG,                                4,                            SPD_RDIMM_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_REVISION_REG,                                 7,                            SPD_INDEX_MAJOR_REVISION_RDIMM_ING_CLOCK_DRIVER_RCD_DEVICE_REVISION                                                                     },
  {SPD_LRDIMM_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_REVISION_REG,                               0,                            SPD_LRDIMM_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_REVISION_REG,                                3,                            SPD_INDEX_MINOR_REVISION_LRDIMM_ING_CLOCK_DRIVER_RCD_DEVICE_REVISION                                                                    },
  {SPD_LRDIMM_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_REVISION_REG,                               4,                            SPD_LRDIMM_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_REVISION_REG,                                7,                            SPD_INDEX_MAJOR_REVISION_LRDIMM_ING_CLOCK_DRIVER_RCD_DEVICE_REVISION                                                                    },
  {SPD_RDIMM_DIFFERENTIAL_MEMORY_BUFFER_DMB_DEVICE_REVISION_REG,                              0,                            SPD_RDIMM_DIFFERENTIAL_MEMORY_BUFFER_DMB_DEVICE_REVISION_REG,                               3,                            SPD_INDEX_MINOR_REVISION_RDIMM_DIFFERENTIAL_MEMORY_BUFFER_DMB_DEVICE_REVISION                                                           },
  {SPD_RDIMM_DIFFERENTIAL_MEMORY_BUFFER_DMB_DEVICE_REVISION_REG,                              4,                            SPD_RDIMM_DIFFERENTIAL_MEMORY_BUFFER_DMB_DEVICE_REVISION_REG,                               7,                            SPD_INDEX_MAJOR_REVISION_RDIMM_DIFFERENTIAL_MEMORY_BUFFER_DMB_DEVICE_REVISION                                                           },
  {SPD_NVDIMM_N_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_REVISION_REG,                             0,                            SPD_NVDIMM_N_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_REVISION_REG,                              3,                            SPD_INDEX_MINOR_REVISION_NVDIMM_N_ING_CLOCK_DRIVER_RCD_DEVICE_REVISION                                                                  },
  {SPD_NVDIMM_N_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_REVISION_REG,                             4,                            SPD_NVDIMM_N_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_REVISION_REG,                              7,                            SPD_INDEX_MAJOR_REVISION_NVDIMM_N_ING_CLOCK_DRIVER_RCD_DEVICE_REVISION                                                                  },
  {SPD_NVDIMM_P_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_REVISION_REG,                             0,                            SPD_NVDIMM_P_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_REVISION_REG,                              3,                            SPD_INDEX_MINOR_REVISION_NVDIMM_P_ING_CLOCK_DRIVER_RCD_DEVICE_REVISION                                                                  },
  {SPD_NVDIMM_P_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_REVISION_REG,                             4,                            SPD_NVDIMM_P_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_REVISION_REG,                              7,                            SPD_INDEX_MAJOR_REVISION_NVDIMM_P_ING_CLOCK_DRIVER_RCD_DEVICE_REVISION                                                                  },
  {SPD_LRDIMM_DBS_DB_MANUFACTURER_ID_CODE_0_REG,                                              0,                            SPD_LRDIMM_DBS_DB_MANUFACTURER_ID_CODE_0_REG,                                               7,                            SPD_INDEX_SPD_LRDIMM_DBS_DB_MANUFACTURER_ID_CODE_0_REG                                                                                  },
  {SPD_NVDIMM_N_DBS_DB_MANUFACTURER_ID_CODE_0_REG,                                            0,                            SPD_NVDIMM_N_DBS_DB_MANUFACTURER_ID_CODE_0_REG,                                             7,                            SPD_INDEX_SPD_NVDIMM_N_DBS_DB_MANUFACTURER_ID_CODE_0_REG                                                                                },
  {SPD_NVDIMM_P_DBS_DB_MANUFACTURER_ID_CODE_0_REG,                                            0,                            SPD_NVDIMM_P_DBS_DB_MANUFACTURER_ID_CODE_0_REG,                                             7,                            SPD_INDEX_SPD_NVDIMM_P_DBS_DB_MANUFACTURER_ID_CODE_0_REG                                                                                },
  {SPD_LRDIMM_DBS_DB_MANUFACTURER_ID_CODE_1_REG,                                              0,                            SPD_LRDIMM_DBS_DB_MANUFACTURER_ID_CODE_1_REG,                                               7,                            SPD_INDEX_SPD_LRDIMM_DBS_DB_MANUFACTURER_ID_CODE_1_REG                                                                                  },
  {SPD_NVDIMM_N_DBS_DB_MANUFACTURER_ID_CODE_1_REG,                                            0,                            SPD_NVDIMM_N_DBS_DB_MANUFACTURER_ID_CODE_1_REG,                                             7,                            SPD_INDEX_SPD_NVDIMM_N_DBS_DB_MANUFACTURER_ID_CODE_1_REG                                                                                },
  {SPD_NVDIMM_P_DBS_DB_MANUFACTURER_ID_CODE_1_REG,                                            0,                            SPD_NVDIMM_P_DBS_DB_MANUFACTURER_ID_CODE_1_REG,                                             7,                            SPD_INDEX_SPD_NVDIMM_P_DBS_DB_MANUFACTURER_ID_CODE_1_REG                                                                                },
  {SPD_LRDIMM_DBS_DB_DEVICE_TYPE_REG,                                                         0,                            SPD_LRDIMM_DBS_DB_DEVICE_TYPE_REG,                                                          3,                            SPD_INDEX_DEVICE_TYPE_LRDIMM_DBS_DB_DEVICE_TYPE                                                                                         },
  {SPD_LRDIMM_DBS_DB_DEVICE_TYPE_REG,                                                         6,                            SPD_LRDIMM_DBS_DB_DEVICE_TYPE_REG,                                                          7,                            SPD_INDEX_DEVICES_INSTALLED_LRDIMM_DBS_DB_DEVICE_TYPE                                                                                   },
  {SPD_NVDIMM_N_DBS_DB_DEVICE_TYPE_REG,                                                       0,                            SPD_NVDIMM_N_DBS_DB_DEVICE_TYPE_REG,                                                        3,                            SPD_INDEX_DEVICE_TYPE_NVDIMM_N_DBS_DB_DEVICE_TYPE                                                                                       },
  {SPD_NVDIMM_N_DBS_DB_DEVICE_TYPE_REG,                                                       6,                            SPD_NVDIMM_N_DBS_DB_DEVICE_TYPE_REG,                                                        7,                            SPD_INDEX_DEVICES_INSTALLED_NVDIMM_N_DBS_DB_DEVICE_TYPE                                                                                 },
  {SPD_NVDIMM_P_DBS_DB_DEVICE_TYPE_REG,                                                       0,                            SPD_NVDIMM_P_DBS_DB_DEVICE_TYPE_REG,                                                        3,                            SPD_INDEX_DEVICE_TYPE_NVDIMM_P_DBS_DB_DEVICE_TYPE                                                                                       },
  {SPD_NVDIMM_P_DBS_DB_DEVICE_TYPE_REG,                                                       6,                            SPD_NVDIMM_P_DBS_DB_DEVICE_TYPE_REG,                                                        7,                            SPD_INDEX_DEVICES_INSTALLED_NVDIMM_P_DBS_DB_DEVICE_TYPE                                                                                 },
  {SPD_LRDIMM_DBS_DB_DEVICE_REVISION_REG,                                                     0,                            SPD_LRDIMM_DBS_DB_DEVICE_REVISION_REG,                                                      3,                            SPD_INDEX_MINOR_REVISION_LRDIMM_DBS_DB_DEVICE_REVISION                                                                                  },
  {SPD_LRDIMM_DBS_DB_DEVICE_REVISION_REG,                                                     4,                            SPD_LRDIMM_DBS_DB_DEVICE_REVISION_REG,                                                      7,                            SPD_INDEX_MAJOR_REVISION_LRDIMM_DBS_DB_DEVICE_REVISION                                                                                  },
  {SPD_NVDIMM_N_DBS_DB_DEVICE_REVISION_REG,                                                   0,                            SPD_NVDIMM_N_DBS_DB_DEVICE_REVISION_REG,                                                    3,                            SPD_INDEX_MINOR_REVISION_NVDIMM_N_DBS_DB_DEVICE_REVISION                                                                                },
  {SPD_NVDIMM_N_DBS_DB_DEVICE_REVISION_REG,                                                   4,                            SPD_NVDIMM_N_DBS_DB_DEVICE_REVISION_REG,                                                    7,                            SPD_INDEX_MAJOR_REVISION_NVDIMM_N_DBS_DB_DEVICE_REVISION                                                                                },
  {SPD_NVDIMM_P_DBS_DB_DEVICE_REVISION_REG,                                                   0,                            SPD_NVDIMM_P_DBS_DB_DEVICE_REVISION_REG,                                                    3,                            SPD_INDEX_MINOR_REVISION_NVDIMM_P_DBS_DB_DEVICE_REVISION                                                                                },
  {SPD_NVDIMM_P_DBS_DB_DEVICE_REVISION_REG,                                                   4,                            SPD_NVDIMM_P_DBS_DB_DEVICE_REVISION_REG,                                                    7,                            SPD_INDEX_MAJOR_REVISION_NVDIMM_P_DBS_DB_DEVICE_REVISION                                                                                },
  {SPD_DRAM_MANUFACTURER_ID_CODE_0_REG,                                                       7,                            SPD_DRAM_MANUFACTURER_ID_CODE_0_REG,                                                        7,                            SPD_INDEX_ODD_PARITY_FOR_BYTE_552_BITS_6_0                                                                                              },
};

SPD_DIECOUNT_MAP  SpdDieCountMapTable[] = {
// Die Type,          Die Num,  Device Type
  {SPD_1DIE_DDR5,     1,        SPD_MONOLITHIC_TYPE                          },
  {SPD_2DIE_DDR5,     2,        SPD_NON_MONOLITHIC_TYPE | SPD_MULTI_LOAD_TYPE},
  {SPD_2DIE_3DS_DDR5, 2,        SPD_NON_MONOLITHIC_TYPE | SPD_3DS_TYPE       },
  {SPD_4DIE_DDR5,     4,        SPD_NON_MONOLITHIC_TYPE | SPD_3DS_TYPE       },
  {SPD_8DIE_DDR5,     8,        SPD_NON_MONOLITHIC_TYPE | SPD_3DS_TYPE       },
  {SPD_16DIE_DDR5,   16,        SPD_NON_MONOLITHIC_TYPE | SPD_3DS_TYPE       }
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
  if (FieldIndex >= SPD_FIELD_MAPPING_TABLE_DDR5_LENGTH) {
    return EFI_INVALID_PARAMETER;
  }

  *SpdFieldMapping = SpdFieldMappingTableDdr5[FieldIndex];

  return EFI_SUCCESS;
}


/**
  Get the minimum cycle time for the SDRAM module.

  Spec section:
    SDRAM Minimum Cycle Time (tCKAVGmin)
    Byte 20 (0x014): Least Significant Byte
    Byte 21 (0x015): Most Significant Byte

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
  IN     PSYSHOST     Host,
  IN     UINT8        Socket,
  IN     UINT8        Chan,
  IN     UINT8        Dimm,
     OUT INT32        *Val
  )
{
  UINT8 Data = 0;
  EFI_STATUS Status;

  Status = SpdReadByte (Socket, Chan, Dimm, SPD_SDRAM_MINIMUM_CYCLE_TIME_TCKAVGMIN_LSB_REG, &Data);
  *Val = Data;
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_012);
    return MRC_STATUS_FAILURE;
  }

  Status = SpdReadByte (Socket, Chan, Dimm, SPD_SDRAM_MINIMUM_CYCLE_TIME_TCKAVGMIN_MSB_REG, &Data);
  *Val |= Data << 8;
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_012);
    return MRC_STATUS_FAILURE;
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Get encoded die count of SDRAM.

  @param[in]     Socket             - Socket ID
  @param[in]     Ch                 - Channel on socket
  @param[in]     Dimm               - Dimm number
  @param[in]     SdramNum           - Target SDRAM number, Primary SDRAM or Secondary SDRAM
  @param[out]    EncodedDieCount    - The pointer to the variable indicating the encoded die count number of SDRAM

  @retval MRC_STATUS_SUCCESS  - EncodedDieCount can be valued successful
  @retval MRC_STATUS_FAILURE  - Invalid input or SpdReadByte can not be completed successful
**/
MRC_STATUS
SpdGetEncodedDieCount (
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Dimm,
  IN     SDRAM_NUM      SdramNum,
     OUT UINT8          *EncodedDieCount
  )
{
  UINT8                                     SPDReg = 0;
  EFI_STATUS                                Status = EFI_NOT_FOUND;
  FIRST_SDRAM_DENSITY_AND_PACKAGE_STRUCT    FirstDensityPackageStruct;
  SECOND_SDRAM_DENSITY_AND_PACKAGE_STRUCT   SecondDensityPackageStruct;

  switch (SdramNum) {
    case PrimarySdramNumber:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_FIRST_SDRAM_DENSITY_AND_PACKAGE_REG, &SPDReg);
      break;
    case SecondarySdramNumber:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_SECOND_SDRAM_DENSITY_AND_PACKAGE_REG, &SPDReg);
      break;
    default:
      break;
  }

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_001);
    return MRC_STATUS_FAILURE;
  }

  if (SdramNum == PrimarySdramNumber) {
    FirstDensityPackageStruct.Data  = SPDReg;
    *EncodedDieCount                = FirstDensityPackageStruct.Bits.die_per_package;
  } else {
    SecondDensityPackageStruct.Data = SPDReg;
    *EncodedDieCount                = SecondDensityPackageStruct.Bits.die_per_package;
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Get device type of SDRAM.

  Spec section:
    Byte 4 (0x004): First SDRAM Density and Package
    Byte 8 (0x008): Second SDRAM Density and Package

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
  UINT8 EncodedDieCount = 0;
  UINT8 Index;
  UINT8 MaxIndex = ARRAY_SIZE (SpdDieCountMapTable);

  SpdGetEncodedDieCount (Socket, Ch, Dimm, SdramNum, &EncodedDieCount);

  for (Index = 0; Index < ARRAY_SIZE (SpdDieCountMapTable); Index++) {
    if (SpdDieCountMapTable[Index].SpdEncodedDie == EncodedDieCount) {
      *DeviceType = SpdDieCountMapTable[Index].DeviceType;
      break;
    }
  }

  if (Index == MaxIndex) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_UNSUPPORTED_MINOR_CODE_100);
    return MRC_STATUS_FAILURE;
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Get die count of SDRAM.

  Spec section:
    Byte 4 (0x004): First SDRAM Density and Package
    Byte 8 (0x008): Second SDRAM Density and Package

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
  UINT8 EncodedDieCount = 0;
  UINT8 Index;
  UINT8 MaxIndex = ARRAY_SIZE (SpdDieCountMapTable);

  SpdGetEncodedDieCount (Socket, Ch, Dimm, SdramNum, &EncodedDieCount);

  for (Index = 0; Index < ARRAY_SIZE (SpdDieCountMapTable); Index++) {
    if (SpdDieCountMapTable[Index].SpdEncodedDie == EncodedDieCount) {
      *DieCount = SpdDieCountMapTable[Index].DramDieCount;
      break;
    }
  }

  if (Index == MaxIndex) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_UNSUPPORTED_MINOR_CODE_101);
    return MRC_STATUS_FAILURE;
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Get actual column bits number of SDRAM.

  Spec section:
    Byte 5 (0x005): First SDRAM Addressing
    Byte 9 (0x009): Second SDRAM Addressing

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
  UINT8                           SPDRevision = 0;
  UINT8                           SPDReg = 0;
  UINT8                           EncodedValue = 0;
  EFI_STATUS                      Status;
  MRC_STATUS                      MrcStatus;
  FIRST_SDRAM_ADDRESSING_STRUCT   FirstSdramAddressStruct;
  SECOND_SDRAM_ADDRESSING_STRUCT  SecondSdramAddressStruct;

  Status                        = EFI_NOT_FOUND;
  MrcStatus                     = MRC_STATUS_FAILURE;
  *NumColBits                   = 0;
  FirstSdramAddressStruct.Data  = 0;
  SecondSdramAddressStruct.Data = 0;

  SpdReadByte (Socket, Ch, Dimm, SPD_SPD_REVISION_FOR_BASE_CONFIGURATION_PARAMETERS_REG, &SPDRevision);

  switch (SdramNum) {
    case PrimarySdramNumber:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_FIRST_SDRAM_ADDRESSING_REG, &SPDReg);
      break;
    case SecondarySdramNumber:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_SECOND_SDRAM_ADDRESSING_REG, &SPDReg);
      break;
    default:
      break;
  }

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_002);
    return MRC_STATUS_FAILURE;
  }

  if (SdramNum == PrimarySdramNumber) {
    FirstSdramAddressStruct.Data  = SPDReg;
    EncodedValue                  = FirstSdramAddressStruct.Bits.first_sdram_column_address_bits;
  } else {
    SecondSdramAddressStruct.Data = SPDReg;
    EncodedValue                  = SecondSdramAddressStruct.Bits.second_sdram_column_address_bits;
  }

  if (SPDRevision >= SPD_REVISION_09_DDR5) {
    *NumColBits = EncodedValue + SPD_COL_BASE_DDR5;
    MrcStatus   = MRC_STATUS_SUCCESS;
  } else {
    if (EncodedValue == SPD_COL_10_DDR5) {
       *NumColBits = SPD_ACTUAL_COL_10_DDR5;
       MrcStatus   = MRC_STATUS_SUCCESS;
    } else if (EncodedValue == SPD_COL_11_DDR5) {
       *NumColBits = SPD_ACTUAL_COL_11_DDR5;
       MrcStatus   = MRC_STATUS_SUCCESS;
    } else {
      RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_UNSUPPORTED_MINOR_CODE_102);
    }
  }

  return MrcStatus;
}

/**
  Get actual row bits number of SDRAM.

  Spec section:
    Byte 5 (0x005): First SDRAM Addressing
    Byte 9 (0x009): Second SDRAM Addressing

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
  UINT8                           SPDRevision = 0;
  UINT8                           SPDReg = 0;
  UINT8                           EncodedValue = 0;
  EFI_STATUS                      Status;
  MRC_STATUS                      MrcStatus;
  FIRST_SDRAM_ADDRESSING_STRUCT   FirstSdramAddressStruct;
  SECOND_SDRAM_ADDRESSING_STRUCT  SecondSdramAddressStruct;

  Status                        = EFI_NOT_FOUND;
  MrcStatus                     = MRC_STATUS_FAILURE;
  *NumRowBits                   = 0;
  FirstSdramAddressStruct.Data  = 0;
  SecondSdramAddressStruct.Data = 0;

  SpdReadByte (Socket, Ch, Dimm, SPD_SPD_REVISION_FOR_BASE_CONFIGURATION_PARAMETERS_REG, &SPDRevision);

  switch (SdramNum) {
    case PrimarySdramNumber:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_FIRST_SDRAM_ADDRESSING_REG, &SPDReg);
      break;
    case SecondarySdramNumber:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_SECOND_SDRAM_ADDRESSING_REG, &SPDReg);
      break;
    default:
      break;
  }

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_003);
    return MRC_STATUS_FAILURE;
  }

  if (SdramNum == PrimarySdramNumber) {
    FirstSdramAddressStruct.Data  = SPDReg;
    EncodedValue                  = FirstSdramAddressStruct.Bits.first_sdram_row_address_bits;
  } else {
    SecondSdramAddressStruct.Data = SPDReg;
    EncodedValue                  = SecondSdramAddressStruct.Bits.second_sdram_row_address_bits;
  }

  if (SPDRevision >= SPD_REVISION_09_DDR5) {
    *NumRowBits = EncodedValue + SPD_ROW_BASE_DDR5;
    MrcStatus   = MRC_STATUS_SUCCESS;
  } else {
    switch (EncodedValue) {
      case SPD_ROW_16_DDR5:
        *NumRowBits = SPD_ACTUAL_ROW_16_DDR5;
        MrcStatus   = MRC_STATUS_SUCCESS;
        break;
      case SPD_ROW_17_DDR5:
        *NumRowBits = SPD_ACTUAL_ROW_17_DDR5;
        MrcStatus   = MRC_STATUS_SUCCESS;
        break;
      case SPD_ROW_18_DDR5:
        *NumRowBits = SPD_ACTUAL_ROW_18_DDR5;
        MrcStatus   = MRC_STATUS_SUCCESS;
        break;
      default:
        RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_UNSUPPORTED_MINOR_CODE_103);
        break;
    }
  }

  return MrcStatus;
}


/**
  Get bank group bits number of SDRAM.

  Spec section:
    Byte 7 (0x007): First SDRAM Bank Groups & Banks Per Bank Group
    Byte 11 (0x00B): Second SDRAM Bank Groups & Banks Per Bank Group

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
  UINT8                                                  SPDReg = 0;
  FIRST_SDRAM_BANK_GROUPS_BANKS_PER_BANK_GROUP_STRUCT    FirstBankGroupAndBankStruct;
  SECOND_SDRAM_BANK_GROUPS_BANKS_PER_BANK_GROUP_STRUCT   SecondBankGroupAndBankStruct;
  EFI_STATUS                                             Status;

  *BankGroupBits                    = 0;
  FirstBankGroupAndBankStruct.Data  = 0;
  SecondBankGroupAndBankStruct.Data = 0;
  Status                            = EFI_NOT_FOUND;

  switch (SdramNum) {
    case PrimarySdramNumber:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_FIRST_SDRAM_BANK_GROUPS_BANKS_PER_BANK_GROUP_REG, &SPDReg);
      break;
    case SecondarySdramNumber:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_SECOND_SDRAM_BANK_GROUPS_BANKS_PER_BANK_GROUP_REG, &SPDReg);
      break;
    default:
      break;
  }

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_004);
    return MRC_STATUS_FAILURE;
  }

  if (SdramNum == PrimarySdramNumber) {
    FirstBankGroupAndBankStruct.Data = SPDReg;
    *BankGroupBits = FirstBankGroupAndBankStruct.Bits.first_sdram_bank_groups;
  } else {
    SecondBankGroupAndBankStruct.Data = SPDReg;
    *BankGroupBits = SecondBankGroupAndBankStruct.Bits.second_sdram_bank_groups;
  }

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
  UINT8                                                  SPDReg = 0;
  FIRST_SDRAM_BANK_GROUPS_BANKS_PER_BANK_GROUP_STRUCT    FirstBankGroupAndBankStruct;
  SECOND_SDRAM_BANK_GROUPS_BANKS_PER_BANK_GROUP_STRUCT   SecondBankGroupAndBankStruct;
  EFI_STATUS                                             Status;

  *BankAddressBits                  = 0;
  FirstBankGroupAndBankStruct.Data  = 0;
  SecondBankGroupAndBankStruct.Data = 0;
  Status                            = EFI_NOT_FOUND;

  switch (SdramNum) {
    case PrimarySdramNumber:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_FIRST_SDRAM_BANK_GROUPS_BANKS_PER_BANK_GROUP_REG, &SPDReg);
      break;
    case SecondarySdramNumber:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_SECOND_SDRAM_BANK_GROUPS_BANKS_PER_BANK_GROUP_REG, &SPDReg);
      break;
    default:
      break;
  }

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_005);
    return MRC_STATUS_FAILURE;
  }

  if (SdramNum == PrimarySdramNumber) {
    FirstBankGroupAndBankStruct.Data = SPDReg;
    *BankAddressBits = FirstBankGroupAndBankStruct.Bits.first_sdram_banks_per_bank_group;
  } else {
    SecondBankGroupAndBankStruct.Data = SPDReg;
    *BankAddressBits = SecondBankGroupAndBankStruct.Bits.second_sdram_banks_per_bank_group;
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Get RCD Vendor information

  Spec section:
    (Registered): Module Specific Device Information
    (Load Reduced): Module Specific Device Information

  Description:
    RCD vendor information refers to RCD manufacturer ID.

  @param[in]     Socket          - Socket ID
  @param[in]     Channel         - Channel on socket
  @param[in]     Dimm            - Dimm number
  @param[out] RcdVendor       - The pointer to the variable indicating to RCD Vendor

  @retval MRC_STATUS_SUCCESS  - RCD Vendor can be valued successful
  @retval MRC_STATUS_FAILURE  - Invalid input or SpdReadByte can not be completed successful
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
  UINT8                           SPDReg = 0;
  MRC_STATUS                      MrcStatus = MRC_STATUS_FAILURE;

  *RcdVendor = 0;
#ifdef  LRDIMM_SUPPORT
  if (IsLrdimmPresent (Socket, Channel, Dimm)) {
    //
    // Second Byte of the register ID
    //
    if (SpdReadByte (Socket, Channel, Dimm, SPD_LRDIMM_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_1_REG, &SPDReg) == EFI_SUCCESS) {
      *RcdVendor = (UINT16) (SPDReg << 8);
      MrcStatus = MRC_STATUS_SUCCESS;
    } else {
      RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_006);
    }

    //
    // First Byte of the register ID
    //
    if (SpdReadByte (Socket, Channel, Dimm, SPD_LRDIMM_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_0_REG, &SPDReg) == EFI_SUCCESS) {
      *RcdVendor |= (UINT16) (SPDReg & ~BIT7);  // UINT8 continuation_codes_num_mem_buff_manufacturer : 7; Bits[0:6]
      MrcStatus = MRC_STATUS_SUCCESS;
    } else {
      RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_006);
    }
  } else
#endif // LRDIMM_SUPPORT
  {
    //
    // Second Byte of the register ID
    //
    if (SpdReadByte (Socket, Channel, Dimm, SPD_RDIMM_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_1_REG, &SPDReg) == EFI_SUCCESS) {
      *RcdVendor = (UINT16) (SPDReg << 8);
      MrcStatus = MRC_STATUS_SUCCESS;
    } else {
      RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_006);
    }
    //
    // First Byte of the register ID
    //
    if (SpdReadByte (Socket, Channel, Dimm, SPD_RDIMM_REGISTERING_CLOCK_DRIVER_RCD_MANUFACTURER_ID_CODE_0_REG, &SPDReg) == EFI_SUCCESS) {
      *RcdVendor |= (UINT16) (SPDReg & ~BIT7);  // UINT8 continuation_codes_num_mem_buff_manufacturer : 7; Bits[0:6]
      MrcStatus = MRC_STATUS_SUCCESS;
    } else {
      RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_006);
    }
  }
  return MrcStatus;
}

/**
  Get RCD Vendor Revision

  Spec section:
    (Registered): Module Specific Device Information
    (Load Reduced): Module Specific Device Information

  Description:
    RCD vendor information refers to RCD revision number, expressed in BCD.

  @param[in]     Socket          - Socket ID
  @param[in]     Channel         - Channel on socket
  @param[in]     Dimm            - Dimm number
  @param[out]    RcdRevision     - The pointer to the variable indicating to RCD Revision

  @retval MRC_STATUS_SUCCESS  - RCD Revision can be valued successful
  @retval MRC_STATUS_FAILURE  - Invalid input or SpdReadByte can not be completed successful
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
  UINT8                           SPDReg = 0;
  MRC_STATUS                      MrcStatus = MRC_STATUS_FAILURE;

  *RcdRevision = 0;
#ifdef  LRDIMM_SUPPORT
  if (IsLrdimmPresent (Socket, Channel, Dimm)) {
    //
    // Register Revision
    //
    if (SpdReadByte (Socket, Channel, Dimm, SPD_LRDIMM_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_REVISION_REG, &SPDReg) == EFI_SUCCESS) {
      *RcdRevision = SPDReg;
      MrcStatus = MRC_STATUS_SUCCESS;
    } else {
      RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_007);
    }
  } else
#endif // LRDIMM_SUPPORT
  {
    //
    // Register Revision
    //
    if (SpdReadByte (Socket, Channel, Dimm, SPD_RDIMM_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_REVISION_REG, &SPDReg) == EFI_SUCCESS) {
      *RcdRevision = SPDReg;
      MrcStatus = MRC_STATUS_SUCCESS;
    } else {
      RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_007);
    }
  }
  return MrcStatus;
}

/**
  Get SDRAM base module type.

  Spec section:
    Byte 3 (0x003): Key Byte / Module Type

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
    Byte 4 (0x004): First SDRAM Density and Package
    Byte 8 (0x008): Second SDRAM Density and Package

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
  //
  // SdramMbBackwardDdr4Compatibility is used to do a translation
  // between DDR5 SPD byte value to DDR4 equivalent value.
  // This LUT is used while we do the full transition to a more abstract SPD byte data usage in our code
  // e.g:
  // In DDR4 0=256Mb, 1=512Mb, 4=4Gb, 5=8Gb, 6=16Gb, 7=32Gb, 8=12Gb, 9=24Gb
  // In DDR5 0=NoMem, 1=4Gb, 2=8Gb, 3=12Gb, 4=16Gb, 5=24Gb, 6=32Gb, 7=48Gb, 8=64Gb
  // So if in DDR5 we have a Dimm that supports 24Gb(SPD encode = 5) we convert it to DDR4 SPD encoding 9
  // Encoding for 48Gb and 64Gb do not exist in DDR4
  //                                                            NoMem  1  2   3   4   5   6  7   8
  //
  UINT8                                    SdramMbBackwardDdr4Compatibility[]      = {0xFF, 4, 5,  8,  6,  9,  7, 10, 11};
  UINT8                                    SPDReg = 0;
  UINT8                                    EncodedDramCapacity = 0;
  FIRST_SDRAM_DENSITY_AND_PACKAGE_STRUCT   FirstDensityAndPackageStruct;
  SECOND_SDRAM_DENSITY_AND_PACKAGE_STRUCT  SecondDensityAndPackageStruct;

  EFI_STATUS          Status;

  Status                              = EFI_NOT_FOUND;
  *EncodedDramCapacityDdr4            = 0;
  FirstDensityAndPackageStruct.Data   = 0;
  SecondDensityAndPackageStruct.Data  = 0;

  switch (SdramNum) {
    case PrimarySdramNumber:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_FIRST_SDRAM_DENSITY_AND_PACKAGE_REG, &SPDReg);
      break;
    case SecondarySdramNumber:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_SECOND_SDRAM_DENSITY_AND_PACKAGE_REG, &SPDReg);
      break;
    default:
      break;
  }

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_008);
    return MRC_STATUS_FAILURE;
  }

  if (SdramNum == PrimarySdramNumber) {
    FirstDensityAndPackageStruct.Data  = SPDReg;
    EncodedDramCapacity                = FirstDensityAndPackageStruct.Bits.sdram_density_per_die;
  } else {
    SecondDensityAndPackageStruct.Data = SPDReg;
    EncodedDramCapacity                = SecondDensityAndPackageStruct.Bits.sdram_density_per_die;
  }
  *EncodedDramCapacityDdr4 = SdramMbBackwardDdr4Compatibility[EncodedDramCapacity % sizeof (SdramMbBackwardDdr4Compatibility)];

  return MRC_STATUS_SUCCESS;
}

/**
  Get Operable and Endurant encoding VDD.

  Spec section:
    Byte 16 (0x010): SDRAM Nominal Voltage, VDD

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
  UINT8             Endurant = 0;
  UINT8             Operable = 0;
  EFI_STATUS        Status;
  SDRAM_NOMINAL_VOLTAGE_VDD_STRUCT    SdramNomVolVdd;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_SDRAM_NOMINAL_VOLTAGE_VDD_REG, &SdramNomVolVdd.Data);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_014);
    return MRC_STATUS_FAILURE;
  }

  Operable = SdramNomVolVdd.Bits.operable;
  Endurant = SdramNomVolVdd.Bits.endurant;
  *OperableEndurant = (Operable << 2) | Endurant;

  return MRC_STATUS_SUCCESS;
}

/**
  Get encoded value of dram IO width bits.

  Spec section:
    Byte 6 (0x006): First SDRAM I/O Width
    Byte 10 (0x00A): Secondary SDRAM I/O Width

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
  UINT8                             SPDReg = 0;
  EFI_STATUS                        Status;
  FIRST_SDRAM_I_O_WIDTH_STRUCT      FirstIOWidthStuct;
  SECONDARY_SDRAM_I_O_WIDTH_STRUCT  SecondIOWidthStuct;

  Status                   = EFI_NOT_FOUND;
  *EncodedDramIOWidth      = 0;
  FirstIOWidthStuct.Data   = 0;
  SecondIOWidthStuct.Data  = 0;

  switch (SdramNum) {
    case PrimarySdramNumber:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_FIRST_SDRAM_I_O_WIDTH_REG, &SPDReg);
      break;
    case SecondarySdramNumber:
      Status = SpdReadByte (Socket, Ch, Dimm, SPD_SECONDARY_SDRAM_I_O_WIDTH_REG, &SPDReg);
      break;
    default:
      break;
  }

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_009);
    return MRC_STATUS_FAILURE;
  }

  if (SdramNum == PrimarySdramNumber) {
    FirstIOWidthStuct.Data  = SPDReg;
    *EncodedDramIOWidth     = FirstIOWidthStuct.Bits.sdram_i_o_width;
  } else {
    SecondIOWidthStuct.Data = SPDReg;
    *EncodedDramIOWidth     = SecondIOWidthStuct.Bits.sdram_i_o_width;
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Get rank mix indicator of DIMM.

  Spec section:
    (Common): Module Organization Byte 234 (0x0EA)

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
  UINT8                               SPDReg;
  EFI_STATUS                          Status;
  COMMON_MODULE_ORGANIZATION_STRUCT   CommonModuleOrganizationStruct;

  *RankMixIndicator                   = 0;
  CommonModuleOrganizationStruct.Data = 0;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_COMMON_MODULE_ORGANIZATION_REG, &SPDReg);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_010);
    return MRC_STATUS_FAILURE;
  }

  CommonModuleOrganizationStruct.Data = SPDReg;
  *RankMixIndicator                   = (BOOLEAN) CommonModuleOrganizationStruct.Bits.rank_mix;

  return MRC_STATUS_SUCCESS;
}

/**
  Get Number of package ranks per channel.

  Spec section:
    (Common): Module Organization Byte 234 (0x0EA)

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
  COMMON_MODULE_ORGANIZATION_STRUCT ModuleOrgReg;
  EFI_STATUS        Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_COMMON_MODULE_ORGANIZATION_REG, &ModuleOrgReg.Data);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_011);
    return MRC_STATUS_FAILURE;
  }

  *PackageRanks = ModuleOrgReg.Bits.number_of_package_ranks_per_channel;

  return MRC_STATUS_SUCCESS;
}

/**
  Get raw card reference design.

  Spec section:
    (Common): Reference Raw Card Used Byte 232 (0x0E8)

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
  COMMON_REFERENCE_RAW_CARD_USED_STRUCT RawCardRefReg;
  EFI_STATUS Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_COMMON_REFERENCE_RAW_CARD_USED_REG, &RawCardRefReg.Data);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_015);
    return MRC_STATUS_FAILURE;
  }

  *RawCardRefDesign = RawCardRefReg.Bits.reference_design;

  return MRC_STATUS_SUCCESS;
}

/**
  Get raw card reference design Revision.

  Spec section:
    (Common): Reference Raw Card Used Byte 232 (0x0E8)

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
  COMMON_REFERENCE_RAW_CARD_USED_STRUCT RawCardRefReg;
  EFI_STATUS Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_COMMON_REFERENCE_RAW_CARD_USED_REG, &RawCardRefReg.Data);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_016);
    return MRC_STATUS_FAILURE;
  }

  *RawCardRefDesignRev = RawCardRefReg.Bits.design_revision;

  return MRC_STATUS_SUCCESS;
}

/**
  Get whether a specific PPR feature is supported.

  Spec section:
    Byte 12 (0x00C): SDRAM Optional Features

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
  SDRAM_OPTIONAL_FEATURES_STRUCT OptionalFeaturesReg;
  EFI_STATUS        Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_SDRAM_OPTIONAL_FEATURES_REG, &OptionalFeaturesReg.Data);

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
  Get module manufacturing location.

  Spec section:
    Byte 514 (0x202): Module Manufacturing Location

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
    Bytes 515~516 (0x203~0x204): Module Manufacturing Date

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
    Bytes 517~520 (0x205~0x208): Module Serial Number

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
    Byte 512 (0x200): Module Manufacturer ID Code, First Byte
    Byte 513 (0x201): Module Manufacturer ID Code, Second Byte

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
  MODULE_MANUFACTURER_ID_CODE_0_STRUCT ManufactureId0Reg;
  MODULE_MANUFACTURER_ID_CODE_1_STRUCT ManufactureId1Reg;
  EFI_STATUS Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_MODULE_MANUFACTURER_ID_CODE_0_REG, &ManufactureId0Reg.Data);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_025);
    return MRC_STATUS_FAILURE;
  }

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_MODULE_MANUFACTURER_ID_CODE_1_REG, &ManufactureId1Reg.Data);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_025);
    return MRC_STATUS_FAILURE;
  }

  *ManufacturerId = (UINT16) (ManufactureId1Reg.Bits.last_non_zero_byte_module_manufacturer << 8)
                      | (UINT16) (ManufactureId0Reg.Bits.number_of_continuation_codes_module_manufacturer);

  return MRC_STATUS_SUCCESS;
}

/**
  Get module manufacturer part number.

  Spec section:
    Bytes 521~550 (0x209~0x226): Module Part Number

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
  UINT8             RegData[SPD_MODULE_PART_DDR5];
  UINT8             Index;

  for (Index = 0; Index < SPD_MODULE_PART_DDR5; Index++) {
    Status = SpdReadByte (Socket, Ch, Dimm, SPD_MODULE_PART_NUMBER_0_REG + Index, &RegData[Index]);

    if (EFI_ERROR (Status)) {
      RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_023);
      return MRC_STATUS_FAILURE;
    }
  }

  CopyMem (PartNum, RegData, SPD_MODULE_PART_DDR5);

  return MRC_STATUS_SUCCESS;
}

/**
  Get dram stepping info.

  Spec section:
    Byte 554 (0x22A): DRAM Stepping

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
    (Registered): Module Specific Device Information
    (Load Reduced): Module Specific Device Information

  Description:
    Function always returns 0x0000 - DDR5RCD01 (JESD82-511).

  @param[in]  Socket     - Socket ID
  @param[in]  Ch         - Channel on socket
  @param[in]  Dimm       - DIMM on channel
  @param[out] RcdType    - Pointer to RCD device type

  @retval MRC_STATUS_SUCCESS         Read successful
  @retval MRC_STATUS_FAILURE         Read failed
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
  RDIMM_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_TYPE_STRUCT RdimmRcdDeviceTypeReg;
  EFI_STATUS        Status;

#ifdef LRDIMM_SUPPORT
  LRDIMM_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_TYPE_STRUCT LrdimmRcdDeviceTypeReg;

  if (IsLrdimmPresent (Socket, Ch, Dimm)) {
    Status = SpdReadByte (Socket, Ch, Dimm, SPD_LRDIMM_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_TYPE_REG, &LrdimmRcdDeviceTypeReg.Data);

    if (EFI_ERROR (Status)) {
      RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_021);
      return MRC_STATUS_FAILURE;
    }

    *RcdType = LrdimmRcdDeviceTypeReg.Bits.device_type;
  } else
#endif // LRDIMM_SUPPORT
  {
    Status = SpdReadByte (Socket, Ch, Dimm, SPD_RDIMM_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_TYPE_REG, &RdimmRcdDeviceTypeReg.Data);

    if (EFI_ERROR (Status)) {
      RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_021);
      return MRC_STATUS_FAILURE;
    }

    *RcdType = RdimmRcdDeviceTypeReg.Bits.device_type;
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Get SDRAM module nominal height.

  Spec section:
    Byte 230 (0x0E6): (Common) Module Nominal Height

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
  COMMON_MODULE_NOMINAL_HEIGHT_STRUCT NominalHeightReg;
  EFI_STATUS        Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_COMMON_MODULE_NOMINAL_HEIGHT_REG, &NominalHeightReg.Data);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_017);
    return MRC_STATUS_FAILURE;
  }

  *Height = NominalHeightReg.Bits.module_nominal_height_max;

  return MRC_STATUS_SUCCESS;
}

/**
  Get SDRAM module type.

  Spec section:
    Byte 2 (0x002): Key Byte / Host Bus Command Protocol Type

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
  KEY_BYTE_HOST_BUS_COMMAND_PROTOCOL_TYPE_STRUCT BusCommandProtocolTypeReg;
  EFI_STATUS        Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_KEY_BYTE_HOST_BUS_COMMAND_PROTOCOL_TYPE_REG, &BusCommandProtocolTypeReg.Data);

  if (EFI_ERROR (Status)) {
    return MRC_STATUS_FAILURE;
  }

  *Type = BusCommandProtocolTypeReg.Bits.sdram_module_type;

  return MRC_STATUS_SUCCESS;
}

/**
  Get primay bus width in bits.

  Spec Section:
    (Common): Memory Channel Bus Width Byte 235 (0x0F3)

  Descirption:
    It describes the primary bus width.

  @param[in]  Socket              - Socket ID
  @param[in]  Ch                  - Channel on socket
  @param[in]  Dimm                - DIMM on channel
  @param[out] PrimaryBusWidthPerCh  - Pointer to the primary bus width, in bits

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
  COMMON_MEMORY_CHANNEL_BUS_WIDTH_STRUCT ChannelBusWidthReg;
  EFI_STATUS        Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_COMMON_MEMORY_CHANNEL_BUS_WIDTH_REG, &ChannelBusWidthReg.Data);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_018);
    return MRC_STATUS_FAILURE;
  }

  *PrimaryBusWidth = ChannelBusWidthReg.Bits.primary_bus_width_per_channel_in_bits;

  return MRC_STATUS_SUCCESS;
}

/**
  Get bus width extension in bits.

  Spec Section:
    (Common): Memory Channel Bus Width Byte 235 (0x0F3)

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
  COMMON_MEMORY_CHANNEL_BUS_WIDTH_STRUCT ChannelBusWidthReg;
  EFI_STATUS        Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_COMMON_MEMORY_CHANNEL_BUS_WIDTH_REG, &ChannelBusWidthReg.Data);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_019);
    return MRC_STATUS_FAILURE;
  }

  *BusWidthExtension = ChannelBusWidthReg.Bits.bus_width_extension_per_channel_in_bits;

  return MRC_STATUS_SUCCESS;
}

/**
  Get channel number per dimm.

  Spec Section:
    (Common): Memory Channel Bus Width Byte 235 (0x0F3)

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
  COMMON_MEMORY_CHANNEL_BUS_WIDTH_STRUCT ChannelBusWidthReg;
  EFI_STATUS        Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_COMMON_MEMORY_CHANNEL_BUS_WIDTH_REG, &ChannelBusWidthReg.Data);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_020);
    return MRC_STATUS_FAILURE;
  }

  *ChannelNumPerDimm = ChannelBusWidthReg.Bits.number_of_channels_per_dimm;

  return MRC_STATUS_SUCCESS;
}

/**
  Get SPD spec revision.

  Spec section:
    Byte 1 (0x001): SPD Revision for Base Configuration Parameters

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

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_SPD_REVISION_FOR_BASE_CONFIGURATION_PARAMETERS_REG, &SPDReg);

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
    SDRAM Minimum Row Precharge Delay Time (tRPmin)
    Byte 34 (0x022): Least Significant Byte
    Byte 35 (0x023): Most Significant Byte

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
  UINT8             SPDReg = 0;
  EFI_STATUS        Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_SDRAM_MINIMUM_ROW_PRECHARGE_DELAY_TIME_TRPMIN_LSB_REG, &SPDReg);
  *TrpVal = SPDReg;

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_026);
    return MRC_STATUS_FAILURE;
  }

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_SDRAM_MINIMUM_ROW_PRECHARGE_DELAY_TIME_TRPMIN_MSB_REG, &SPDReg);
  *TrpVal |= SPDReg << 8;

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_026);
    return MRC_STATUS_FAILURE;
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Get the Minimum SDRAM Active to Precharge Delay Time.

  Spec section:
    SDRAM Minimum Active to Precharge Delay Time (tRASmin)
    Byte 36 (0x024): Least Significant Byte
    Byte 37 (0x025): Most Significant Byte

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
  UINT8             SPDReg = 0;
  EFI_STATUS        Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_SDRAM_MIN_ACTIVE_TO_PRECHARGE_DELAY_TRAS_LSB_REG, &SPDReg);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_029);
    return MRC_STATUS_FAILURE;
  }

  *TrasVal = SPDReg;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_SDRAM_MIN_ACTIVE_TO_PRECHARGE_DELAY_TRAS_MSB_REG, &SPDReg);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_029);
    return MRC_STATUS_FAILURE;
  }

  *TrasVal |= SPDReg << 8;

  return MRC_STATUS_SUCCESS;
}

/**
  Get the minimum SDRAM Active to Active/Refresh Delay Time.

  Spec section:
    SDRAM Minimum Active to Active/Refresh Delay Time (tRCmin)
    Byte 38 (0x026): Least Significant Byte
    Byte 39 (0x027): Most Significant Byte

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
  UINT8             SPDReg = 0;
  EFI_STATUS        Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_SDRAM_MIN_ACTIVE_TO_ACTIVE_REFRESH_DELAY_TRC_LSB_REG, &SPDReg);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_031);
    return MRC_STATUS_FAILURE;
  }

  *TrcVal = SPDReg;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_SDRAM_MIN_ACTIVE_TO_ACTIVE_REFRESH_DELAY_TRC_MSB_REG, &SPDReg);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_031);
    return MRC_STATUS_FAILURE;
  }

  *TrcVal |= SPDReg << 8;

  return MRC_STATUS_SUCCESS;
}

/**
  Get the minimum SDRAM RAS to CAS Delay Time.

  Spec section:
    SDRAM Minimum RAS to CAS Delay Time (tRCDmin)
    Byte 32 (0x020): Least Significant Byte
    Byte 33 (0x021): Most Significant Byte

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
  UINT8             SPDReg = 0;
  EFI_STATUS        Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_SDRAM_MINIMUM_RAS_TO_CAS_DELAY_TIME_TRCDMIN_LSB_REG, &SPDReg);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_032);
    return MRC_STATUS_FAILURE;
  }

  *TrcdVal = SPDReg;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_SDRAM_MINIMUM_RAS_TO_CAS_DELAY_TIME_TRCDMIN_MSB_REG, &SPDReg);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_032);
    return MRC_STATUS_FAILURE;
  }

  *TrcdVal |= SPDReg << 8;

  return MRC_STATUS_SUCCESS;
}

/**
  Get the the minimum CAS Latency.

  Spec section:
    SDRAM Minimum CAS Latency Time (tAAmin)
    Byte 30 (0x01E): Least Significant Byte
    Byte 31 (0x01F): Most Significant Byte

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
  UINT8             SPDReg = 0;
  EFI_STATUS        Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_SDRAM_MINIMUM_CAS_LATENCY_TIME_TAAMIN_LSB_REG, &SPDReg);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_033);
    return MRC_STATUS_FAILURE;
  }

  *TaaVal = SPDReg;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_SDRAM_MINIMUM_CAS_LATENCY_TIME_TAAMIN_MSB_REG, &SPDReg);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_033);
    return MRC_STATUS_FAILURE;
  }

  *TaaVal |= SPDReg << 8;

  return MRC_STATUS_SUCCESS;
}

/**
  Get supported CAS Latency (CL) values with one bit per possible CAS Latency.

  Spec section:
    SDRAM CAS Latencies Supported
    Byte 24 (0x018): First Byte
    Byte 25 (0x019): Second Byte
    Byte 26 (0x01A): Third Byte
    Byte 27 (0x01B): Fourth Byte
    Byte 28 (0x01C): Fifth Byte

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
  // Get byte 5 of CasSup
  //
  Status = SpdReadByte (Socket, Ch, Dimm, SPD_SDRAM_CAS_LATENCIES_SUPPORTED_4_REG, &SPDReg);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_034);
    return MRC_STATUS_FAILURE;
  }

  *CasSup = (UINT64)LShiftU64 (SPDReg, 32);

  //
  // Get byte 4 of CasSup
  //
  Status = SpdReadByte (Socket, Ch, Dimm, SPD_SDRAM_CAS_LATENCIES_SUPPORTED_3_REG, &SPDReg);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_034);
    return MRC_STATUS_FAILURE;
  }

  *CasSup |= (UINT64)LShiftU64 (SPDReg, 24);

  //
  // Get byte 3 of CasSup
  //
  Status = SpdReadByte (Socket, Ch, Dimm, SPD_SDRAM_CAS_LATENCIES_SUPPORTED_2_REG, &SPDReg);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_034);
    return MRC_STATUS_FAILURE;
  }

  *CasSup |= (UINT64)LShiftU64 (SPDReg, 16);

  //
  // Get byte 2 of CasSup
  //
  Status = SpdReadByte (Socket, Ch, Dimm, SPD_SDRAM_CAS_LATENCIES_SUPPORTED_1_REG, &SPDReg);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_034);
    return MRC_STATUS_FAILURE;
  }

  *CasSup |= (UINT64)LShiftU64 (SPDReg, 8);

  //
  // Get byte 1 of CasSup
  //
  Status = SpdReadByte (Socket, Ch, Dimm, SPD_SDRAM_CAS_LATENCIES_SUPPORTED_0_REG, &SPDReg);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_034);
    return MRC_STATUS_FAILURE;
  }

  *CasSup |= (UINT64)SPDReg;

  return MRC_STATUS_SUCCESS;
}

/**
  Get whether thermal sensor is installed.

  Spec section:
    (Common): Module Device Information

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
  COMMON_THERMAL_SENSORS_TS_DEVICE_TYPE_STRUCT ThermalSensorTypeReg;
  EFI_STATUS        Status;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_COMMON_THERMAL_SENSORS_TS_DEVICE_TYPE_REG, &ThermalSensorTypeReg.Data);
  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_035);
    return FALSE;
  }

  //
  // TSO/TS1 installed or not
  //
  if (SensorIndex == 0) {
    return ((ThermalSensorTypeReg.Bits.devices_installed & BIT1) == BIT1);
  } else if (SensorIndex == 1) {
    return ((ThermalSensorTypeReg.Bits.devices_installed & BIT0) == BIT0);
  } else {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_UNSUPPORTED_MINOR_CODE_105);
    return FALSE;
  }
}

/**
  Get manufacturer ID of the DRAM.

  Spec section:
    Byte 552 (0x228): DRAM Manufacturer ID Code, First Byte
    Byte 553 (0x229): DRAM Manufacturer ID Code, Second Byte

  Description:
    This two-byte field indicates the manufacturer of the DRAM on the module, and shall be encoded
  as follows: the first byte is the number of continuation bytes indicated in JEP-106; the second byte
  is the last non-zero byte of the manufacturer ID code, again as indicated in JEP-106.

  @param[in]  Socket              - Socket ID
  @param[in]  Ch                  - Channel on socket
  @param[in]  Dimm                - DIMM on channel
  @param[out] DramManufacturerId  - Pointer to DRAM manufacturer ID

  @retval MRC_STATUS_SUCCESS         Read successful
  @retval MRC_STATUS_FAILURE         Read failed
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
  DRAM_MANUFACTURER_ID_CODE_0_STRUCT DramManufactureId0Reg;
  DRAM_MANUFACTURER_ID_CODE_1_STRUCT DramManufactureId1Reg;

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_DRAM_MANUFACTURER_ID_CODE_0_REG, &DramManufactureId0Reg.Data);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_037);
    return MRC_STATUS_FAILURE;
  }

  Status = SpdReadByte (Socket, Ch, Dimm, SPD_DRAM_MANUFACTURER_ID_CODE_1_REG, &DramManufactureId1Reg.Data);

  if (EFI_ERROR (Status)) {
    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_READ_MINOR_CODE_037);
    return MRC_STATUS_FAILURE;
  }

  *DramManufacturerId = (UINT16) (DramManufactureId1Reg.Bits.last_non_zero_byte_dram_manufacturer << 8)
                      | (UINT16) (DramManufactureId0Reg.Bits.number_of_continuation_codes_dram_manufacturer);

  return MRC_STATUS_SUCCESS;
}
