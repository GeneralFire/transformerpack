/** @file
  Interface header file for the Enhanced warning log library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef _ENHANCED_WARNING_LOG_LIB_
#define _ENHANCED_WARNING_LOG_LIB_

#include <ReferenceCodeDataTypes.h>
#include <Uefi.h>
#include <MrcCommonTypes.h>

//
// Ehanced warning code major and
// minor warning codes
//

#define  WARN_RDIMM_ON_UDIMM               0x01
  #define WARN_MINOR_CONFIG_NOT_SUPPORTED      0x01
#define  WARN_UDIMM_ON_RDIMM               0x02
#define  WARN_SODIMM_ON_RDIMM              0x03
#define  WARN_4Gb_FUSE                     0x04
#define  WARN_8Gb_FUSE                     0x05
#define  WARN_IMC_DISABLED                 0x06
#define  WARN_DIMM_COMPAT                  0x07
 #define  WARN_DIMM_COMPAT_MINOR_X16_C0MBO              0x01
 #define  WARN_DIMM_COMPAT_MINOR_MAX_RANKS              0x02
 #define  WARN_DIMM_COMPAT_MINOR_QR                     0x03
 #define  WARN_DIMM_COMPAT_MINOR_NOT_SUPPORTED          0x04
 #define  WARN_RANK_NUM                                 0x05
 #define  WARN_TOO_SLOW                                 0x06
 #define  WARN_DIMM_COMPAT_MINOR_ROW_ADDR_ORDER         0x07
 #define  WARN_CHANNEL_CONFIG_NOT_SUPPORTED             0x08
 #define  WARN_CHANNEL_MIX_ECC_NONECC                   0x09
 #define  WARN_DIMM_VOLTAGE_NOT_SUPPORTED               0x0A
 #define  WARN_DIMM_COMPAT_TRP_NOT_SUPPORTED            0x0B
 #define  WARN_DIMM_NONECC                              0x0C
 #define  WARN_DIMM_COMPAT_3DS_RDIMM_NOT_SUPPORTED      0x0D //3DS
 #define  WARN_RANK_COUNT_MISMATCH                      0x0E
 #define  WARN_DIMM_SKU_MISMATCH                        0x0F
 #define  WARN_DIMM_3DS_NOT_SUPPORTED                   0x10
 #define  WARN_DIMM_NVMDIMM_NOT_SUPPORTED               0x11
 #define  WARN_DIMM_16GB_SUPPORTED                      0x12
 #define  WARN_CHANNEL_SKU_NOT_SUPPORTED                0x13
 #define  WARN_DIMM_SKU_NOT_SUPPORTED                   0x14
 #define  WARN_CHANNEL_CONFIG_FREQ_NOT_SUPPORTED        0x15
 #define  WARN_DIMM_SPEED_NOT_SUP                       0x16
 #define  WARN_NO_DDR4_ON_SOCKET                        0x17
 #define  WARN_NO_DDR4_ON_S0C0D0                        0x18
 #define  WARN_DIMM_NOT_IN_DDRT_POR_DDR_TABLE           0x19
 #define  WARN_SOCKET_CPU_DOES_NOT_SUPPORT_NVMDIMM      0x1A
 #define  WARN_SOCKET_SKU_DOES_NOT_SUPPORT_NVMDIMM      0x1B
 #define  WARN_DIMM_IN_DDRT_POR_DDR_TABLE_NOT_VALIDATED 0x1C
 #define  WARN_DDRT_DIMM_NOT_SUPPORTED                  0x1D
 #define  WARN_CHANNEL_WIDTH_MIX_NOT_SUPPORTED          0x1E
 #define  WARN_DIMM_24GB_SUPPORTED                      0x1F
#define  WARN_LOCKSTEP_DISABLE             0x09
 #define  WARN_LOCKSTEP_DISABLE_MINOR_RAS_MODE         0x01
 #define  WARN_LOCKSTEP_DISABLE_MINOR_MISMATCHED       0x02
 #define  WARN_LOCKSTEP_DISABLE_MINOR_MEMTEST_FAILED   0x03
#define  WARN_USER_DIMM_DISABLE            0x0a
 #define  WARN_USER_DIMM_DISABLE_QUAD_AND_3DPC         0x01
 #define  WARN_USER_DIMM_DISABLE_MEMTEST               0x02
 #define  WARN_USER_DIMM_DISABLE_RANK_DISABLED         0x03
 #define  WARN_USER_DIMM_DISABLE_MAPPED_OUT            0x04
#define  WARN_MEMTEST_DIMM_DISABLE         0x0b
#define  WARN_MIRROR_DISABLE               0x0c
 #define  WARN_MIRROR_DISABLE_MINOR_RAS_DISABLED       0x01
 #define  WARN_MIRROR_DISABLE_MINOR_MISMATCH           0x02
 #define  WARN_MIRROR_DISABLE_MINOR_MEMTEST            0x03
#define  WARN_PMIRROR_DISABLE              0x0d
 #define  WARN_PMIRROR_DISABLE_MINOR_RAS_DISABLED      0x01
#define  WARN_INTERLEAVE_FAILURE           0x0e
 #define  WARN_SAD_RULES_EXCEEDED                      0x01
 #define  WARN_TAD_RULES_EXCEEDED                      0x02
 #define  WARN_RIR_RULES_EXCEEDED                      0x03
 #define  WARN_TAD_OFFSET_NEGATIVE                     0x04
 #define  WARN_TAD_LIMIT_ERROR                         0x05
 #define  WARN_INTERLEAVE_3WAY                         0x06
 #define  WARN_A7_MODE_AND_3WAY_CH_INTRLV              0x07
 #define  WARN_INTERLEAVE_EXCEEDED                     0x08
 #define  WARN_DIMM_CAPACITY_MISMATCH                  0x09
 #define  WARN_DIMM_POPULATION_MISMATCH                0x0A
 #define  WARN_NM_MAX_SIZE_EXCEEDED                    0x0B
 #define  WARN_NM_SIZE_BELOW_MIN_LIMIT                 0x0C
 #define  WARN_NM_SIZE_NOT_POWER_OF_TWO                0x0D
 #define  WARN_MAX_INTERLEAVE_SETS_EXCEEDED            0x0E
 #define  WARN_NGN_DIMM_COMM_FAILED                    0x0F
#define  WARN_DIMM_COMM_FAILED             0x0F
 #define WARN_MINOR_DIMM_COMM_START_DOORBELL_TIMEOUT   0x01
 #define WARN_MINOR_DIMM_COMM_FAILED_STATUS            0x02
 #define WARN_MINOR_DIMM_MAILBOX_FAILED                0x03
 #define WARN_MINOR_DIMM_COMM_FINISH_DOORBELL_TIMEOUT  0x04
 #define WARN_MINOR_DIMM_COMM_FINISH_COMPLETE_TIMEOUT  0x05
#define  WARN_SPARE_DISABLE                0x10
  #define WARN_SPARE_DISABLE_MINOR_RK_SPARE            0x01
#define  WARN_PTRLSCRB_DISABLE             0x11
  #define WARN_PTRLSCRB_MINOR_DISABLE                  0x00
#define  WARN_UNUSED_MEMORY                0x12
 #define  WARN_UNUSED_MEMORY_MINOR_MIRROR              0x01
 #define  WARN_UNUSED_MEMORY_MINOR_LOCKSTEP            0x02
#define  WARN_RD_DQ_DQS                    0x13
#define  WARN_RD_RCVEN                     0x14
 #define  WARN_ROUNDTRIP_EXCEEDED                      0x01
 #define  WARN_RCVEN_PI_DELAY_EXCEEDED                 0x02
#define  WARN_WR_LEVEL                     0x15
 #define  WARN_WR_FLYBY_CORR                           0x00
 #define  WARN_WR_FLYBY_UNCORR                         0x01
 #define  WARN_WR_FLYBY_DELAY                          0x02
#define  WARN_WR_DQ_DQS                    0x16
#define  WARN_DIMM_POP_RUL                 0x17
 #define  WARN_DIMM_POP_RUL_MINOR_OUT_OF_ORDER            0x01
 #define  WARN_DIMM_POP_RUL_MINOR_INDEPENDENT_MODE        0x02
 #define  WARN_DIMM_POP_RUL_2_AEP_FOUND_ON_SAME_CH        0x03
 #define  WARN_DIMM_POP_RUL_MINOR_MIXED_RANKS_FOUND       0x04
 #define  WARN_DIMM_POP_RUL_NVMDIMM_OUT_OF_ORDER          0x05
 #define  WARN_DIMM_POP_RUL_UDIMM_POPULATION              BIT5
 #define  WARN_DIMM_POP_RUL_RDIMM_POPULATION              BIT6
 #define  WARN_DIMM_POP_RUL_LRDIMM_DUAL_DIE_POPULATION    BIT7
 #define  WARN_DIMM_POP_RUL_MINOR_NO_DDRT_ON_MC0          0x06
 #define  WARN_DIMM_POP_RUL_MINOR_NO_DDRT_ON_SOCKET0      0x07
 #define  WARN_DIMM_POP_RUL_DDR4_CAP_NOT_POR_FOR_MEM_POP  0x08
 #define  WARN_DIMM_POP_RUL_DDR4_TYPE_NOT_POR_FOR_MM      0x09
 #define  WARN_DIMM_POP_RUL_MEM_TOPOLOGY_NOT_SYMMETRICAL  0x0a
 #define  WARN_DIMM_POP_RUL_NM_FM_RATIO_VIOLATION         0x0b
 #define  WARN_DIMM_POP_RUL_2LM_IMC_MEM_MISMATCH          0x0c
 #define  WARN_DIMM_POP_RUL_DDRT_CAPACITY_MISMATCH        0x0d
 #define  WARN_DIMM_POP_RUL_DDRT_PARTITION_MISMATCH       0x0e
 #define  WARN_DIMM_POP_RUL_AEP_VDD_CHANGED               0x0f
 #define  WARN_DIMM_POP_RUL_SOCKET_MODE_MISMATCH          0x10
 #define  WARN_DIMM_POP_RUL_DCPDIMM_MIXING                0x11
 #define  WARN_DIMM_POP_RUL_DDR_CAPACITY_MISMATCH         0x12
 #define  WARN_DIMM_POP_RUL_POPULATION_POR_MISMATCH       0x13
 #define  WARN_DIMM_POP_RUL_MEM_POP_POR_TBL_INVALID       0x14
 #define  WARN_DIMM_POP_RUL_2LM_FM_CH_NOT_PWR_OF_TWO      0x15
 #define  WARN_DIMM_POP_RUL_POPULATION_POR_NOT_FOUND      0x16
 #define  WARN_DIMM_POP_RUL_PMEM_X1_POPULATION_INVALID    0x17
 #define  WARN_DIMM_POP_RUL_NO_DIMM_ON_SOCKET             0x18
#define  WARN_DIMM_POP_RUL_EXTENDED       0x3e
 #define  WARN_DIMM_POP_RUL_LRDIMM_3DS_POPULATION         BIT1
 #define  WARN_DIMM_POP_RUL_RDIMM_3DS_POPULATION          BIT2

#define  WARN_CLTT_DISABLE                 0x18
 #define  WARN_CLTT_MINOR_NO_TEMP_SENSOR               0x01
 #define  WARN_CLTT_MINOR_CIRCUIT_TST_FAILED           0x02
#define  WARN_THROT_INSUFFICIENT           0x19
  #define WARN_2X_REFRESH_TEMPLO_DISABLED              0x01
  #define WARN_CUSTOM_REFRESH_RATE_REVERTED            0x02
#define  WARN_CLTT_DIMM_UNKNOWN            0x1a
#define  WARN_DQS_TEST                     0x1b
#define  WARN_MEM_TEST                     0x1c
#define  WARN_CLOSED_PAGE_OVERRIDE         0x1d
#define  WARN_DIMM_VREF_NOT_PRESENT        0x1e
#define  WARN_EARLY_RID                    0x1f
 #define  WARN_EARLY_RID_UNCORR                        0x01
 #define  WARN_EARLY_RID_CYCLE_FAIL                    0x02
#define  WARN_LV_STD_DIMM_MIX              0x20
#define  WARN_LV_2QR_DIMM                  0x21
#define  WARN_LV_3DPC                      0x22
#define  WARN_CMD_ADDR_PARITY_ERR          0x23
#define  WARN_DQ_SWIZZLE_DISC              0x24
 #define  WARN_DQ_SWIZZLE_DISC_UNCORR                  0x01
#define  WARN_COD_HA_NOT_ACTIVE            0x25
#define  WARN_CMD_CLK_TRAINING             0x26
 #define  WARN_CMD_PI_GROUP_SMALL_EYE                  0x01
#define  WARN_INVALID_BUS                  0x27
#define  WARN_INVALID_FNV_OPCODE           0x28
 #define  WARN_INVALID_FNV_SUBOPCODE                   0x00
#define  WARN_MEMORY_TRAINING              0x29
 #define  WARN_CTL_CLK_LOOPBACK_TRAINING               0x02
 #define  WARN_ODT_TIMING_OVERFLOW                     0x03
 #define  WARN_CS_CLK_LOOPBACK_TRAINING                0x04
 #define  WARN_CA_CLK_LOOPBACK_TRAINING                0x05
 #define  WARN_REQ_CLK_TRAINING                        0x06
#define  WARN_NO_MEMORY                    0x2a
 #define  WARN_NO_MEMORY_MINOR_NO_MEMORY               0x01
 #define  WARN_NO_MEMORY_MINOR_ALL_CH_DISABLED         0x02
 #define  WARN_NO_MEMORY_MINOR_ALL_CH_DISABLED_MIXED   0x03
 #define  WARN_NO_MEMORY_MINOR_NO_DDR                  0x04
#define  WARN_ROUNDTRIP_ERROR              0x2b
 #define  WARN_RCVNTAP_CMDDELAY_EXCEEDED               0x01
#define  WARN_MEMORY_MODEL_ERROR           0x2c
 #define  WARN_SNC24_MODEL_ERROR                       0x01
 #define  WARN_QUAD_HEMI_MODEL_ERROR                   0x02
 #define  WARN_SNC24_DIMM_POPULATION_MISMATCH          0x03
 #define  WARN_SNC24_INCOMPATIBLE_DDR_CAPACITY         0x04
 #define  WARN_SNC24_INCOMPATIBLE_MCDRAM_CAPACITY      0x05
 #define  WARN_MCDRAM_CONFIG_NOT_SUPPORTED             0x06
 #define  WARN_SNC24_TILE_POPULATION_MISMATCH          0x07
#define  WARN_OVERRIDE_MEMORY_MODE  0x2d
 #define  WARN_OVERRIDE_TO_FLAT_NOT_ENOUGH_NEAR_MEMORY 0x01
 #define  WARN_OVERRIDE_TO_FLAT_NOT_ENOUGH_FAR_MEMORY  0x02
 #define  WARN_OVERRIDE_TO_HYBRID_50_PERCENT           0x03
//
// For backwards compatibility with KNL
//
 #define  WARN_OVERRIDE_TO_FLAT_NO_MCDRAM_MEMORY       WARN_OVERRIDE_TO_FLAT_NOT_ENOUGH_NEAR_MEMORY
 #define  WARN_OVERRIDE_TO_FLAT_NO_DDR_MEMORY          WARN_OVERRIDE_TO_FLAT_NOT_ENOUGH_FAR_MEMORY
 #define  WARN_OVERRIDE_TO_HYBRID_CACHESIZE_1_2        WARN_OVERRIDE_TO_HYBRID_50_PERCENT
#define  WARN_MEM_INIT                     0x2e
#define  WARN_SENS_AMP_TRAINING            0x2f
  #define  WARN_SENS_AMP_CH_FAILIURE                   0x01

#define  WARN_FPT_CORRECTABLE_ERROR        0x30
#define  WARN_FPT_UNCORRECTABLE_ERROR      0x31
  #define  WARN_FPT_MINOR_RD_DQ_DQS                    0x13
  #define  WARN_FPT_MINOR_RD_RCVEN                     0x14
  #define  WARN_FPT_MINOR_WR_LEVEL                     0x15
  #define  WARN_FPT_MINOR_WR_FLYBY                     0x00
  #define  WARN_FPT_MINOR_WR_DQ_DQS                    0x16
  #define  WARN_FPT_MINOR_DQS_TEST                     0x1b
  #define  WARN_FPT_MINOR_MEM_TEST                     0x1c
  #define  WARN_FPT_MINOR_LRDIMM_DWL_EXT_COARSE_TRAINING 0x20
  #define  WARN_FPT_MINOR_LRDIMM_DWL_EXT_FINE_TRAINING   0x21
  #define  WARN_FPT_MINOR_LRDIMM_DWL_INT_COARSE_TRAINING 0x22
  #define  WARN_FPT_MINOR_LRDIMM_DWL_INT_FINE_TRAINING   0x23
  #define  WARN_FPT_MINOR_LRDIMM_TRAINING              0x24
  #define  WARN_FPT_MINOR_VREF_TRAINING                0x25
  #define  WARN_FPT_MINOR_LRDIMM_RCVEN_PHASE_TRAINING  0x26
  #define  WARN_FPT_MINOR_LRDIMM_RCVEN_CYCLE_TRAINING  0x27
  #define  WARN_FPT_MINOR_LRDIMM_READ_DELAY_TRAINING   0x28
  #define  WARN_FPT_MINOR_LRDIMM_WL_TRAINING           0x29
  #define  WARN_FPT_MINOR_LRDIMM_COARSE_WL_TRAINING    0x2A
  #define  WARN_FPT_MINOR_LRDIMM_WRITE_DELAY_TRAINING  0x2B
  #define  WARN_QxCA_CLK_NO_EYE_FOUND                  0x2C
  #define  WARN_FPT_ROW_FAILURE                        0x2D
  #define  WARN_FPT_PPR_ROW_REPAIR                     0x2E
  #define  WARN_FPT_MINOR_WL_EXTERNAL_COARSE           0x30
  #define  WARN_FPT_MINOR_WL_EXTERNAL_FINE             0x31
  #define  WARN_FPT_MINOR_WL_INTERNAL_COARSE           0x32
  #define  WARN_FPT_MINOR_WL_INTERNAL_FINE             0x33
  #define  WARN_FPT_MINOR_WL_INTERNAL_OUT_OF_CYCLE     0x34

#define  WARN_CH_DISABLED                  0x32
  #define  WARN_TWR_LIMIT_REACHED                      0x01
  #define  WARN_TWR_LIMIT_ON_LOCKSTEP_CH               0x02

#define  WARN_MEM_LIMIT                    0x33
#define  WARN_RT_DIFF_EXCEED               0x34
  #define WARN_RT_DIFF_MINOR_EXCEED                    0x01

#define  WARN_PPR_FAILED                   0x35
#define  WARN_REGISTER_OVERFLOW            0x36
  #define  WARN_MINOR_REGISTER_OVERFLOW                      0x01
  #define  WARN_MINOR_REGISTER_UNDERFLOW                     0x02
#define  WARN_SWIZZLE_DISCOVERY_TRAINING   0x37
 #define  WARN_SWIZZLE_PATTERN_MISMATCH                0x01
#define  WARN_WRCRC_DISABLE                0x38
 #define  WARN_TRAIL_ODT_LIMIT_REACHED                 0x01
#define  WARN_FNV_BSR                      0x39
 #define  WARN_DT_ERROR                                0x01
 #define  WARN_MEDIA_READY_ERROR                       0x02
 #define  WARN_POLLING_LOOP_TIMEOUT                    0x03
 #define  WARN_OPCODE_INDEX_LOOKUP                     0x04
 #define  WARN_DR_READY_ERROR                          0x05

#define  WARN_ADDDC_DISABLE                0x3a
  #define  WARN_ADDDC_MINOR_DISABLE                0x01
#define  WARN_SDDC_DISABLE                 0x3b
  #define  WARN_SDDC_MINOR_DISABLE                 0x02

#define WARN_FW_OUT_OF_DATE                0x3c
  #define WARN_FW_CLK_MOVEMENT                         0x00
  #define WARN_FW_BCOM_MARGINING                       0x01
#define WARN_SMBUS_FAULT                   0x3d
  #define WARN_SMBUS_WRITE                             0x01
  #define WARN_SMBUS_READ                              0x02
#define  WARN_COMPLETION_DELAY_ERROR    0x3f
#define  WARN_CMPL_DELAY_BELOW_MIN                   0x02
#define  WARN_CMPL_DELAY_MAX_EXCEEDED                0x03

#define  WARN_MEM_CONFIG_CHANGED           0x40
 #define  WARN_MEM_OVERRIDE_DISABLED                   0x01

#define  WARN_DIMM_MEM_MODE 0x41
 #define  WARN_DIMM_MEM_MODE_NEW_DIMM_2LM_NOT_SUPPORTED 0x01
 #define  WARN_DIMM_MEM_MODE_2LM_NOT_SUPPORTED          0x02

#define  WARN_MCA_UNCORRECTABLE_ERROR   0x50
  #define  WARN_PREVIOUS_BOOT_MCA_MINOR_CODE              0x01

#define  WARN_DM_TEST_ERROR_CODE        0x60
  #define WARN_DM_TEST_PARSE_ERROR_MINOR_CODE          0x01
  #define WARN_DM_TEST_CONFIGURATION_ERROR_MINOR_CODE  0x02
  #define WARN_DM_TEST_EXECUTION_ERROR_MINOR_CODE      0x03
#define WARN_FPGA_NOT_DETECTED          0x75


//
// HBM WARNING CODES
//
#define WARN_FSM_ERROR_CODE             0x80
  #define WARN_FSM_TIMEOUT_ERROR                       0x01

#define WARN_IEEE_1500_ERROR_CODE       0x81
  #define WARN_IEEE_1500_TIMEOUT_ERROR                 0x01

#define  WARN_HBM_CORRECTABLE_ERROR     0x82
#define  WARN_HBM_UNCORRCTABLE_ERROR    0x83
  #define  WARN_HBM_BASIC_XOVER_FSM                    0x01
  #define  WARN_HBM_BASIC_AWORD_FSM                    0x02
  #define  WARN_HBM_BASIC_DWORD_TX_FSM                 0x03
  #define  WARN_HBM_BASIC_DWORD_RX_FSM                 0x04
  #define  WARN_HBM_RX_DQ_DELAY_ADJ                    0x05
  #define  WARN_HBM_ADVANCE_AWORD                      0x06
  #define  WARN_HBM_TX_DS_TCO_TRAINING                 0x07
  #define  WARN_HBM_RX_VREF_TRAINING                   0x08
  #define  WARN_HBM_ADVANCE_RX_DWORD                   0x09
  #define  WARN_HBM_ADVANCE_TX_DWORD                   0x0A
  #define  WARN_HBM_ROUNDTRIP_LATENCY                  0x0B
  #define  WARN_HBM_RX_DELAY_OPT                       0x0C
  #define  WARN_HBM_POR_COMPAT                         0x0D

//
// NVDIMM Status Warning Codes
//
#define WARN_NVMCTRL_MEDIA_STATUS       0x84
  #define WARN_NVMCTRL_MEDIA_NOTREADY        0x02
  #define WARN_NVMCTRL_MEDIA_INERROR         0x03
  #define WARN_NVMCTRL_MEDIA_RESERVED        0x04
  #define WARN_NVMCTRL_MEDIA_DWR             0x05
//
// Current Configuration Warning Codes
//
#define WARN_CFGCUR_STATUS              0x85
  #define WARN_CFGCUR_SIGNATURE_MISMATCH          0x01
  #define WARN_CFGCUR_CHECKSUM_MISMATCH           0x02
  #define WARN_CFGCUR_REVISION_MISMATCH           0x03
  #define WARN_CFGCUR_DATASIZE_EXCEEDED           0x04
  #define WARN_CFGCUR_DATASIZE_OFFSET_EXCEEDED    0x05
//
// Input Configuration Warning Codes
//
#define WARN_CFGIN_STATUS               0x86
  #define WARN_CFGIN_SIGNATURE_MISMATCH      0X01
  #define WARN_CFGIN_CHECKSUM_MISMATCH       0X02
  #define WARN_CFGIN_REVISION_MISMATCH       0X03
  #define WARN_CFGIN_DATASIZE_EXCEEDED       0x04
//
// Output Configuration Warning Codes
//
#define WARN_CFGOUT_STATUS              0x87
  #define WARN_CFGOUT_SIGNATURE_MISMATCH     0X01
  #define WARN_CFGOUT_CHECKSUM_MISMATCH      0X02
  #define WARN_CFGOUT_REVISION_MISMATCH      0X03
  #define WARN_CFGOUT_DATASIZE_EXCEEDED      0x04
//
// BIOS Configuration Header Warning Codes
//
#define WARN_BIOS_CONFIG_HEADER_STATUS              0x88
  #define WARN_BIOS_CONFIG_HEADER_CHECKSUM_MISMATCH      0X01
  #define WARN_BIOS_CONFIG_HEADER_REVISION_MISMATCH      0X02
  #define WARN_BIOS_CONFIG_HEADER_SIGNATURE_MISMATCH     0X03
//
// OS Configuration Header Warning Codes
//
#define WARN_OS_CONFIG_HEADER_STATUS              0x89
  #define WARN_OS_CONFIG_HEADER_CHECKSUM_MISMATCH      0X01
  #define WARN_OS_CONFIG_HEADER_REVISION_MISMATCH      0X02
  #define WARN_OS_CONFIG_HEADER_LENGTH_MISMATCH        0X03
  #define WARN_OS_CONFIG_HEADER_SIGNATURE_MISMATCH     0X04

// NVMDIMM Surprise Clock Warning codes
//
#define WARN_NVDIMM_SURPRISE_CLOCK_STOP       0x8A
  #define  WARN_POWER_CYCLE_POLICY_NOT_APPLY       0x01
  #define  WARN_POWER_CYCLE_POLICY_APPLY           0x02

//
// NVMDIMM FNV Access Codes
//
#define WARN_FNV_ACCESS                 0x8B
  #define  WARN_INVALID_FNV_ACCESS_MODE      0x01

//
// MEMORY Boot Health log Warnings
//
#define WARN_MEMORY_BOOT_HEALTH_CHECK        0x8C
  #define WARN_MEMORY_BOOT_HEALTH_CHECK_MASK_FAIL       0x01
  #define WARN_MEMORY_BOOT_HEALTH_CHECK_CHANNEL_MAP_OUT 0x02

//
// Memory Thermal Management Error.
//
#define WARN_MEMORY_POWER_MANAGEMENT              0x8D
  #define WARN_MEMORY_PM_WEIGHT_TABLE_NOT_FOUND        0x01
  #define WARN_MEMORY_PM_THERMAL_TABLE_NOT_FOUND       0x02

//
// Total Memory Encryption Error.
//
#define WARN_TME_MKTME_FAILURE                    0xE0
  #define MINOR_ERR_SECURITY_POLICY_NOT_FOUND             0x01
  #define MINOR_ERR_PHYSICAL_ADDRESS_BITS_EXCEEDED_MAX    0x02

//
// Silicon capability limitation warning
//
#define WARN_SILICON_CAPABILITY_LIMITATION   0xE1
  #define WARN_TME_ENABLED_CRYSTAL_RIDGE_NOT_SUPPORTED      0x01
  #define WARN_SGX_ENABLED_CRYSTAL_RIDGE_NOT_SUPPORTED      0x02

//
// Round trip calculation for DDRT exceeded valid range
//
#define WARN_ROUNDTRIP_CALCULATION_RANGE_ERROR  0xE2
  #define WARN_DDRT_TO_DDR4_ROUNDTRIP_RANGE_ERROR      0x01
  #define WARN_DDR4_TO_DDRT_ROUNDTRIP_RANGE_ERROR      0x02

#define  WARN_MINOR_WILDCARD  0xff

#pragma pack(1)

///
/// Enhanced Warning Log Identification GUID
/// This GUID is used for HOB, UEFI variables, or UEFI Configuration Table as needed by platform implementations
/// {D8E05800-005E-4462-AA3D-9C6B4704920B}
///
#define EWL_ID_GUID { 0xd8e05800, 0x5e, 0x4462, { 0xaa, 0x3d, 0x9c, 0x6b, 0x47, 0x4, 0x92, 0xb } };

///
/// Enhanced Warning Log Revision GUID
/// Rev 1:  {75713370-3805-46B0-9FED-60F282486CFC}
///
#define EWL_REVISION1_GUID { 0x75713370, 0x3805, 0x46b0, { 0x9f, 0xed, 0x60, 0xf2, 0x82, 0x48, 0x6c, 0xfc } };

///
/// Enhanced Warning Log Header
///
typedef struct {
  EFI_GUID  EwlGuid;      /// GUID that uniquely identifies the EWL revision
  UINT32    Size;         /// Total size in bytes including the header and buffer
  UINT32    FreeOffset;   /// Offset of the beginning of the free space from byte 0
                          /// of the buffer immediately following this structure
                          /// Can be used to determine if buffer has sufficient space for next entry
  UINT32    Crc;          /// 32-bit CRC generated over the whole size minus this crc field
                          /// Note: UEFI 32-bit CRC implementation (CalculateCrc32) (References [7])
                          /// Consumers can ignore CRC check if not needed.
  UINT32    Reserved;     /// Reserved for future use, must be initialized to 0
} EWL_HEADER;

///
/// List of all entry types supported by this revision of EWL
///
typedef enum {
  EwlType0  = 0,
  EwlType1  = 1,
  EwlType2  = 2,
  EwlType3  = 3,
  EwlType4  = 4,
  EwlType5  = 5,
  EwlType6  = 6,
  EwlType7  = 7,
  EwlType8  = 8,
  EwlType9  = 9,
  EwlType10 = 10,
  EwlType11 = 11,
  EwlType12 = 12,
  EwlType13 = 13,
  EwlType14 = 14,
  EwlType15 = 15,
  EwlType16 = 16,
  EwlType17 = 17,
  EwlType18 = 18,
  EwlType19 = 19,
  EwlType20 = 20,
  EwlType21 = 21,
  EwlType22 = 22,
  EwlType23 = 23,
  EwlType24 = 24,
  EwlType25 = 25,
  EwlType26 = 26,
  EwlType27 = 27,
  EwlType28 = 28,
  EwlTypeMax,
  EwlTypeOem = 0x8000,
  EwlTypeDelim = MAX_INT32
  } EWL_TYPE;

///
/// EWL severities
///
typedef enum {
  EwlSeverityInfo,
  EwlSeverityWarning,
  EwlSeverityFatal,
  EwlSeverityMax,
  EwlSeverityDelim = MAX_INT32
  } EWL_SEVERITY;

///
/// EWL Size\Type Structure for error checking
///
typedef struct {
  EWL_TYPE Type;
  UINT16   Size;
} EWL_SIZE_CHECK;

///
/// Generic entry header for parsing the log
///
typedef struct {
  EWL_TYPE      Type;
  UINT16        Size;     /// Entries will be packed by byte in contiguous space
  EWL_SEVERITY  Severity; /// Warning, error, informational, this may be extended in the future
} EWL_ENTRY_HEADER;

///
/// Legacy content provides context of the warning
///
typedef struct {
  UINT8     MajorCheckpoint;  // EWL Spec - Appendix B
  UINT8     MinorCheckpoint;
  UINT8     MajorWarningCode; // EWL Spec - Appendix A
  UINT8     MinorWarningCode;
} EWL_ENTRY_CONTEXT;

///
/// Legacy content to specify memory location
///
typedef struct {
  UINT8     Socket;     /// 0xFF = n/a
  UINT8     Channel;    /// 0xFF = n/a
  UINT8     Dimm;       /// 0xFF = n/a
  UINT8     Rank;       /// 0xFF = n/a
} EWL_ENTRY_MEMORY_LOCATION;

///
/// Type 1 = Legacy memory warning log content plus checkpoint
///
typedef struct {
  EWL_ENTRY_HEADER           Header;
  EWL_ENTRY_CONTEXT          Context;
  EWL_ENTRY_MEMORY_LOCATION  MemoryLocation;
} EWL_ENTRY_TYPE1;

///
/// Type 2 = Enhanced type for data IO errors per device, per bit.
///   Primarily associated with MRC training failures. Checkpoint information provides additional
///   details to identify associated training step.
///
typedef struct {
  EWL_ENTRY_HEADER           Header;
  EWL_ENTRY_CONTEXT          Context;
  EWL_ENTRY_MEMORY_LOCATION  MemoryLocation;
  UINT8                      Strobe;     /// 0xFF = n/a; include mapping of Dqs to Dq bits
  UINT8                      Bit;        /// 0xFF = n/a; Dq bit# within strobe group
  MRC_LT                     Level;      /// MrcGtDelim = n/a; Check BIOS SSA spec (References [1])
  MRC_GT                     Group;      /// MrcGtDelim = n/a; Check BIOS SSA spec (References [1])
  UINT8                      EyeSize;    /// 0xFF = n/a
} EWL_ENTRY_TYPE2;

///
/// Type 3 = Enhanced type for command, control IO errors
///   Primarily associated with MRC training failures. Checkpoint information provides additional
///   details to identify associated training step.
///
typedef struct {
  EWL_ENTRY_HEADER           Header;
  EWL_ENTRY_CONTEXT          Context;
  EWL_ENTRY_MEMORY_LOCATION  MemoryLocation;
  MRC_LT                     Level;       /// MrcGtDelim = n/a; Check BIOS SSA spec (References [1])
  MRC_GT                     Group;       /// MrcGtDelim = n/a; Check BIOS SSA spec (References [1])
  GSM_CSN                    Signal;      /// GsmCsnDelim = n/a
  UINT8                      EyeSize;     /// 0xFF = n/a
} EWL_ENTRY_TYPE3;

///
/// Requisite definitions for Type 4
///
/// Advanced Memtest Types
///
typedef enum {
  AdvMtXmats8     = 0,
  AdvMtXmats16    = 1,
  AdvMtXmats32    = 2,
  AdvMtXmats64    = 3,
  AdvMtWcmats8    = 4,
  AdvMtWcmch8     = 5,
  AdvMtGndb64     = 6,
  AdvMtMarchCm64  = 7,
  AdvMtLtestScram = 8,
  AdvMtLinitScram = 9,
  AdvMtMax,
  AdvMtDelim = MAX_INT32
  } ADV_MT_TYPE;

///
/// Advanced Memtest Error log structure based on processor specific CSR definitions
///
typedef struct {
  UINT32 Dat0S;
  UINT32 Dat1S;
  UINT32 Dat2S;
  UINT32 Dat3S;
  UINT32 EccS;
  UINT32 Chunk;
  UINT32 Column;
  UINT32 ColumnExt;
  UINT32 Row;
  UINT32 RowExt;
  UINT32 Bank;
  UINT32 Rank;
  UINT32 Subrank;
} EWL_ADV_MT_STATUS;

///
/// Type 4 = Enhanced type for DRAM Advanced Memtest errors
///
typedef struct {
  EWL_ENTRY_HEADER           Header;
  EWL_ENTRY_CONTEXT          Context;
  EWL_ENTRY_MEMORY_LOCATION  MemoryLocation;
  ADV_MT_TYPE                MemtestType;
  EWL_ADV_MT_STATUS          AdvMemtestErrorInfo;
  UINT32                     Count;
} EWL_ENTRY_TYPE4;

///
/// Type 5 = Legacy Memtest accumulated DQ errors
///
typedef struct {
  EWL_ENTRY_HEADER           Header;
  EWL_ENTRY_CONTEXT          Context;
  EWL_ENTRY_MEMORY_LOCATION  MemoryLocation;
  UINT8                      SubRank;
  UINT8                      BankAddress;
  UINT8                      DqBytes[9];  /// Byte 0 = DQ[7:0], byte 1 = DQ[15:8], etc.
} EWL_ENTRY_TYPE5;

///
/// Type 6 = Legacy UPI/KTIRC warning log content plus checkpoint
///
typedef struct {
  EWL_ENTRY_HEADER        Header;
  EWL_ENTRY_CONTEXT       Context;
  UINT8                   SocketMask; /// Bitmask of CPU Sockets affected; 0xFF = SystemWide
  UINT8                   SocketType; /// 0 = CPU Socket, 1 = FPGA, 0xFF = System Wide Warning
  UINT8                   Port;       /// 0xFF = n/a; bitmask of affected port(s)
} EWL_ENTRY_TYPE6;

///
/// Type 7 = CPU BIST failures
///
typedef struct{
  EWL_ENTRY_HEADER        Header;
  EWL_ENTRY_CONTEXT       Context;
  UINT8                   Socket;  /// Socket number, 0 based
  UINT32                  Core;    /// Core number, 0 based
} EWL_ENTRY_TYPE7;

///
/// IIO Link Error log structure primary based on PCIE Specification 3.0 (References [8])
///
typedef struct {
  UINT8                   Socket;          /// Socket number, 0 based
  UINT8                   Stack;           /// 0-4, 0 = Cstack, 1-3 = Pstack, 4 MCP-stack (Only SKX-F)
  UINT8                   Port;            /// 0-3
  UINT8                   LtssmMainState;  /// Link state
  UINT8                   LtssmSubState;   /// Check Appendix C to review states definitions
  UINT32                  DidVid;          /// [31:16] DeviceID, [15:0] VendorID of the device
                                           ///   attached to the Root Port
} EWL_IIO_LINK_DESCRIPTION;

///
/// Type 8 = IIO Link Degraded width
///
typedef struct {
  EWL_ENTRY_HEADER         Header;
  EWL_ENTRY_CONTEXT        Context;
  EWL_IIO_LINK_DESCRIPTION LinkDescription;
  UINT8                    ExpectedLinkWidth;  /// Check register "Link Capabilities Register" over
  UINT8                    ActualLinkWidth;    /// PCIE Specification 3.0 (References [8])
} EWL_ENTRY_TYPE8;

///
/// Type 9 = IIO Link Degraded speed
///
typedef struct {
  EWL_ENTRY_HEADER         Header;
  EWL_ENTRY_CONTEXT        Context;
  EWL_IIO_LINK_DESCRIPTION LinkDescription;
  UINT8                    ExpectedLinkSpeed;  /// Check register "Link Capabilities Register" over
  UINT8                    ActualLinkSpeed;    /// PCIE Specification 3.0 (References [8])
} EWL_ENTRY_TYPE9;

///
/// Type 10 = Dq Swizzle Discovery errors
///   Error if 0 or greater than 1 bit set in SwizzledDqLanes per strobe
///
typedef struct {
  EWL_ENTRY_HEADER           Header;
  EWL_ENTRY_CONTEXT          Context;
  EWL_ENTRY_MEMORY_LOCATION  MemoryLocation;
  UINT8                      SwizzlePattern;  /// DQ pattern sent from device
  UINT8                      SwizzledDqLanes; /// DQ pattern received at Host
  UINT8                      LanesPerStrobe;  /// 4 or 8
  UINT8                      Strobe;          /// DQS number to identify device
} EWL_ENTRY_TYPE10;

///
/// Type 11 = NVMDIMM Boot Status Register
///   Reported when status indicates NVMDIMM is not ready
///
typedef struct {
  EWL_ENTRY_HEADER           Header;
  EWL_ENTRY_CONTEXT          Context;
  EWL_ENTRY_MEMORY_LOCATION  MemoryLocation;
  UINT32                     BootStatusRegister; /// Check Appendix C to review status definitions
} EWL_ENTRY_TYPE11;

///
/// Type 12 = NVMDIMM Mailbox Failure
///   Reported when NVMDIMM returns mailbox failure
///
/// Refer to the NVMDIMM Firmware Interface Spec (References [4])
///
typedef struct {
  EWL_ENTRY_HEADER           Header;
  EWL_ENTRY_CONTEXT          Context;
  EWL_ENTRY_MEMORY_LOCATION  MemoryLocation;
  UINT32                     Revision;         /// Rev of NVMDIMM FIS API (References [4])
  UINT16                     Command;          /// Contents of NVMDIMM MB Command register also refer
                                               /// to EWL_Spec_v1_1 Appendix C: Additional Definitions (NVMDIMM)
  UINT8                      Status;           /// Contents of NVMDIMM MB Status register
} EWL_ENTRY_TYPE12;

///
/// Type 13 = NVMDIMM Training Failure
///   Reported when a training issue is encountered
///   Includes additional details on the NVMDIMM SPD and FW revisions
///
typedef struct {
  EWL_ENTRY_HEADER           Header;
  EWL_ENTRY_CONTEXT          Context;
  EWL_ENTRY_MEMORY_LOCATION  MemoryLocation;
  UINT16                     RevisionNvmdimmFw;
  UINT8                      RevisionNvmdimmSpd;
} EWL_ENTRY_TYPE13;

///
/// Type 14 = RST_CPL handshake failure
///   In future this could be managed by creating a new Pcode command returns back the FW version.
///   Requires BIOS to support a timeout break for all PCU polling loops including SetRstCpl,
///   Program_Bios_Reset_Cpl, and all PCU mailbox communication
///
typedef struct {
  EWL_ENTRY_HEADER        Header;
  EWL_ENTRY_CONTEXT       Context;
  UINT8                   Socket;         /// Socket number, 0 based
  UINT32                  Revision;       /// PCU API Revision
  BOOLEAN                 TimeoutError;   /// TRUE if timeout occurred; FALSE if no timeout
  UINT32                  BiosWriteData;  /// To decode the command and response refer to BWG
  UINT32                  PcuReponseData; ///  SKX BWG Chapter 4 (References [2])
} EWL_ENTRY_TYPE14;

///
/// Type 15 = BIOS-PCU mailbox communication failure
/// Structure definition based on usage of WriteBios2PcuMailboxCommand function
///
typedef struct {
  EWL_ENTRY_HEADER        Header;
  EWL_ENTRY_CONTEXT       Context;
  UINT8                   Socket;          /// Socket number, 0 based
  UINT32                  Revision;        /// PCU API Revision
  UINT8                   TimeoutError;    /// 1 if timeout occurred; 0 if no timeout
  UINT32                  DataIn;          /// To decode the command and response refer to BWG
  UINT32                  Command;         ///  SKX BWG Chapter 4 (References [2])
  UINT32                  Status;
  UINT32                  DataOut;
} EWL_ENTRY_TYPE15;

///
/// For ME and IE Communication Errors please also refer to the related CPU's Intel(R) Management Engine
/// (Intel(R) ME) BIOS Specification (BS) e.g. Skylake Platform Intel(R) ME 11 BS (References [3])
/// For IE case, each customer is responsible for defining their own API specification.
///
/// Type 16 = ME state failures
///   Unexpected state of Manageability Engine detected
///
typedef struct {
  EWL_ENTRY_HEADER       Header;
  EWL_ENTRY_CONTEXT      Context;
  UINT32                 Revision; /// ME API Revision
  UINT32                 Mefs1;    /// ME Firmware Status 1 (HECI-1 HFS)
  UINT32                 Mefs2;    /// ME Firmware Status 2 (HECI-1 GS_SHDW)
  UINT32                 H2fs;     /// ME HECI-2 HFS
  UINT32                 H3fs;     /// ME HECI-3 HFS
  UINT32                 H4fs;     /// IE HECI-4 HFS
} EWL_ENTRY_TYPE16;

///
/// Type 17 = ME communication failures
///   Failure to communicate with Manageability Engine
///
typedef struct {
  EWL_ENTRY_HEADER       Header;
  EWL_ENTRY_CONTEXT      Context;
  UINT32                 Revision;       /// ME API Revision
  UINT32                 Mefs1;          /// ME Firmware Status 1 (HECI-1 HFS)
  UINT32                 Mefs2;          /// ME Firmware Status 2 (HECI-1 GS_SHDW)
  UINT8                  HeciDevice;     /// HECI device (1, 2, or 3)
  UINT8                  MeAddress;      /// HECI address of ME entity
  UINT8                  SendStatus;     /// Status of send operation
  UINT8                  ReceiveStatus;  /// Status of receive operation
  UINT64                 Request;        /// First 8 bytes of request message
  UINT32                 Response;       /// First 4 bytes of response message
} EWL_ENTRY_TYPE17;

///
/// Type 18 = IE state failures
///   Unexpected state of Innovation Engine detected
///
typedef struct {
  EWL_ENTRY_HEADER       Header;
  EWL_ENTRY_CONTEXT      Context;
  UINT32                 Revision; /// IE API Revision
  UINT32                 Iefs1;    /// IE Firmware Status 1 (HECI-1 HFS)
  UINT32                 Iefs2;    /// IE Firmware Status 2 (HECI-1 GS_SHDW)
  UINT32                 H2fs;     /// IE HECI-2 HFS
  UINT32                 H3fs;     /// IE HECI-3 HFS
  UINT32                 H4fs;     /// IE HECI-4 HFS
} EWL_ENTRY_TYPE18;

///
/// Type 19 = IE communication failures
///   Failure to communicate with Innovation Engine
///
typedef struct {
  EWL_ENTRY_HEADER       Header;
  EWL_ENTRY_CONTEXT      Context;
  UINT32                 Revision;       /// IE API Revision
  UINT32                 Iefs1;          /// IE Firmware Status 1 (HECI-1 HFS)
  UINT32                 Iefs2;          /// IE Firmware Status 2 (HECI-1 GS_SHDW)
  UINT8                  HeciDevice;     /// HECI device (1, 2, or 3)
  UINT8                  IeAddress;      /// HECI address of IE entity
  UINT8                  SendStatus;     /// Status of send operation
  UINT8                  ReceiveStatus;  /// Status of receive operation
  UINT64                 Request;        /// First 8 bytes of request message
  UINT32                 Response;       /// First 4 bytes of response message
} EWL_ENTRY_TYPE19;

///
/// To get more information about Machine-Check Architecture please check Chapter 15 from Vol. 3B
///  of the Intel(R) 64 and IA-32 Architectures Software Developer's Manual (References [6]) for a
///  general review. Complement this information with Skylake Server Processor External Design
///  Specification (EDS) Volume Two: Registers, Part A (References [5])
///
/// Type 20 = CPU Machine Check Errors
///
typedef struct {
  EWL_ENTRY_HEADER        Header;
  EWL_ENTRY_CONTEXT       Context;
  UINT32                  CpuId;         /// Refer to CPUID(EAX = 1) instruction to get Type, Family,
                                         ///  Model, and Stepping ID from (References [6])
  UINT8                   Socket;        /// Socket number, 0 based
  UINT32                  Core;          /// Core number, 0 based
  UINT32                  McBankNum;     /// Please refer to mcBankTable definition from
                                         /// PurleySktPkg\Library\ProcMemInit\Chip\Common\ErrorChip.c
  UINT32                  McBankStatus;  /// Check register IA32_MCi_STATUS MSRs (References [6]&[5])
  UINT32                  McBankAddr;    /// Check register IA32_MCi_ADDR MSRs (References [6]&[5])
  UINT32                  McBankMisc;    /// Check register IA32_MCi_MISC MSRs (References [6]&[5])
} EWL_ENTRY_TYPE20;

///
/// Requisite definitions for Type 21
///
/// Reasons for Topology degradation
///
typedef enum {
  Undefined           = 0,
  LinkFail            = 1,
  InvalidTopology     = 2,
  FeatureVsTopology   = 3,
  DegradeReasonMax,
  DegradeReasonDelim  = MAX_INT32
  } TOPOLOGY_DEGRADE_REASON;

///
/// Type 21: Warning for tracking changes to KTI/UPI topology
///
/// Topology will be represented with a UINT64 bit array
/// 0 indicates absent or inactive link
/// 1 indicates active KTI/UPI link
///
/// Link Bit array member variables follow this format
/// Each nibble corresponds to a socket:
/// Each socket has MAX_FW_KTI_PORTS bits
/// [(8*MAX_FW_KTI_PORTS - 1):7*MAX_FW_KTI_PORTS] - link bit mask for socket 7
/// [(7*MAX_FW_KTI_PORTS - 1):6*MAX_FW_KTI_PORTS] - link bit mask for socket 6
/// ....
/// [(2*MAX_FW_KTI_PORTS - 1):  MAX_FW_KTI_PORTS] - link bit mask for socket 1
/// [(MAX_FW_KTI_PORTS - 1)  :                 0] - link bit mask for socket 0
///
///   Bit 0 indicates an active link on port socket 0 port 0
///   Bit 1 indicates an active link on port socket 0 port 1
///   and so on.

typedef struct {
  EWL_ENTRY_HEADER        Header;
  TOPOLOGY_DEGRADE_REASON Reason;
  UINT64                  DegradedFrom;     /// Link Bit Array
  UINT64                  NewTopology;      /// Link Bit Array
} EWL_ENTRY_TYPE21;

///
/// To get more information about Machine-Check Architecture please check Chapter 15 from Vol. 3B
///  of the Intel 64 and IA-32 Architectures Software Developer's Manual (References [6]) for a
///  general review. Complement this information with Skylake Server Processor External Design
///  Specification (EDS) Volume Two: Registers, Part A (References [5])
///
/// Type 22 = CPU Machine Check Errors. 2nd Version.
///
typedef struct {
  EWL_ENTRY_HEADER        Header;
  EWL_ENTRY_CONTEXT       Context;
  UINT32                  CpuId;         /// Refer to CPUID(EAX=1) instruction to get Type, Family,
                                         ///  Model, and Stepping ID from (References [6])
  UINT8                   Socket;        /// Socket number, 0 based
  UINT32                  Core;          /// Core number, 0 based
  UINT32                  McBankNum;     /// Please refer to mcBankTable definition from
                                         /// PurleySktPkg\Library\ProcMemInit\Chip\Common\ErrorChip.c
  UINT64                  McBankStatus;  /// Check register IA32_MCi_STATUS MSRs (References [6]&[5])
  UINT64                  McBankAddr;    /// Check register IA32_MCi_ADDR MSRs (References [6]&[5])
  UINT64                  McBankMisc;    /// Check register IA32_MCi_MISC MSRs (References [6]&[5])
} EWL_ENTRY_TYPE22;

///
/// Type 23 = NVMDIMM Boot Status Register. 2nd Version.
///   Reported when status indicates NVMDIMM is not ready
///
typedef struct {
  EWL_ENTRY_HEADER           Header;
  EWL_ENTRY_CONTEXT          Context;
  EWL_ENTRY_MEMORY_LOCATION  MemoryLocation;
  UINT32                     BootStatusRegister;   /// Check Appendix C to review status definitions
  UINT32                     BootStatusRegisterHi; /// Higher 32 bits of AEP BSR.
} EWL_ENTRY_TYPE23;


///
/// Type 24: Warning for tracking TME/MKTME
///
typedef struct {
  EWL_ENTRY_HEADER        Header;
  EWL_ENTRY_CONTEXT       Context;
  UINT8                   Socket;
} EWL_ENTRY_TYPE24;

//
// Memory Boot Health check Warning log.
//
typedef struct {
  EWL_ENTRY_HEADER           Header;
  EWL_ENTRY_CONTEXT          Context;
  EWL_ENTRY_MEMORY_LOCATION  MemoryLocation;
  MRC_GT                     Group;      /// MrcGtDelim = n/a; Check BIOS SSA spec (References [1])
  INT16                      Offset;     /// Signal offset size that caused the error
} EWL_ENTRY_TYPE25;

//
// Memory Power Management Errors
//
typedef struct {
  EWL_ENTRY_HEADER           Header;
  EWL_ENTRY_CONTEXT          Context;
  EWL_ENTRY_MEMORY_LOCATION  MemoryLocation;
} EWL_ENTRY_TYPE26;

///
/// Type 27 = NVMDIMM Media Log
///   Reported NVMDIMM Media log
///
typedef struct {
  EWL_ENTRY_HEADER           Header;
  EWL_ENTRY_CONTEXT          Context;
  EWL_ENTRY_MEMORY_LOCATION  MemoryLocation;
  UINT64                     TimeStamp;
  UINT64                     DPA;
  UINT64                     PDA;
  UINT8                      Range;
  UINT8                      ErrorType;
  UINT8                      ErrorFlag;
  UINT8                      TransacationType;
  UINT16                     SequenceNumber;
  UINT16                     Rsvd;
} EWL_ENTRY_TYPE27;

///
/// Type 28 = NVMDIMM Thermal Log
///   Reported NVMDIMM Thermal log
///
typedef struct {
  EWL_ENTRY_HEADER           Header;
  EWL_ENTRY_CONTEXT          Context;
  EWL_ENTRY_MEMORY_LOCATION  MemoryLocation;
  UINT64                     TimeStamp;
  UINT32                     HostReportedTempData;
  UINT16                     SequenceNumber;
  UINT16                     Rsvd;
} EWL_ENTRY_TYPE28;

#pragma pack()

///
/// Enhanced Warning Log Spec defined data log structure
///
typedef struct {
  EWL_HEADER Header;          /// The size will vary by implementation and should not be assumed
  UINT8      Buffer[4 * 1024];  /// The spec requirement is that the buffer follow the header
} EWL_PUBLIC_DATA;

///
/// EWL private data structure.  This is going to be implementation dependent
///   When we separate OEM hooks via a PPI, we can remove this
///
typedef struct {
  UINT32            bufSizeOverflow;  // Number of bytes that could not be added to buffer
  UINT32            numEntries;       // Number of entries currently logged
  EWL_PUBLIC_DATA   status;           // Spec defined EWL
} EWL_PRIVATE_DATA;

/**

  Report a new entry is available for reporting to
  the Enhanced Warning Log.

  This function will add it to the log or return an error if there is insufficient
  space remaining for the entry.

  @param[in] EwlEntry    - Pointer to new EWL entry buffer
  @retval                - SUCCESS if entry is added; FAILURE if entry is not added

**/
MRC_STATUS
EwlLogEntry (
  IN EWL_ENTRY_HEADER  *EwlEntry
  );

/**

  Print enhanced warning log info.

  @param[in]  Header            Ptr to the EWL header.

  @retval None

**/

VOID
EFIAPI
EwlPrintWarning (
  IN EWL_ENTRY_HEADER *WarningHeader
  );

/**

  Count the number of warnings in the enhanced warning log.

  @param  None

  @retval Number of warnings in the log

**/

UINT32
EFIAPI
CountWarningsInLog (
  VOID
  );

/**

  Construct a EWL type 1 warning.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Socket.
  @param[in]  Channel               Channel.
  @param[in]  Dimm                  Dimm.
  @param[in]  Rank                  Rank.

  @retval None

**/

VOID
EFIAPI
EwlOutputType1 (
  IN UINT8    WarningCode,
  IN UINT8    MinorWarningCode,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  );

/**

  Construct a EWL type 2 warning.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Socket.
  @param[in]  Channel               Channel.
  @param[in]  Dimm                  Dimm.
  @param[in]  Rank                  Rank.
  @param[in]  Severity              Severity.
  @param[in]  Strobe                Strobe.
  @param[in]  Bit                   Bit.
  @param[in]  Group                 Group.
  @param[in]  Level                 Level.
  @param[in]  EyeSize               EyeSize.

  @retval None

**/

VOID
EFIAPI
EwlOutputType2 (
  IN UINT8        WarningCode,
  IN UINT8        MinorWarningCode,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN UINT8        Dimm,
  IN UINT8        Rank,
  IN EWL_SEVERITY Severity,
  IN UINT8        Strobe,
  IN UINT8        Bit,
  IN MRC_GT       Group,
  IN MRC_LT       Level,
  IN UINT8        EyeSize
  );

/**

  Construct a EWL type 3 warning.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Socket.
  @param[in]  Channel               Channel.
  @param[in]  Dimm                  Dimm.
  @param[in]  Rank                  Rank.
  @param[in]  Severity              Severity.
  @param[in]  Group                 Group.
  @param[in]  Level                 Level.
  @param[in]  Level                 Signal.
  @param[in]  EyeSize               EyeSize.

  @retval None

**/

VOID
EFIAPI
EwlOutputType3 (
  IN UINT8        WarningCode,
  IN UINT8        MinorWarningCode,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN UINT8        Dimm,
  IN UINT8        Rank,
  IN EWL_SEVERITY Severity,
  IN MRC_GT       Group,
  IN MRC_LT       Level,
  IN GSM_CSN      Signal,
  IN UINT8        EyeSize
  );

/**

  Construct a EWL type 4 warning.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Socket.
  @param[in]  Channel               Channel.
  @param[in]  Dimm                  Dimm.
  @param[in]  Rank                  Rank.
  @param[in]  Severity              Severity.
  @param[in]  MemtestType           MemtestType.
  @param[in]  AdvMemtestErrorInfo   AdvMemtestErrorInfo.
  @param[in]  Count                 Count.

  @retval None

**/

VOID
EFIAPI
EwlOutputType4 (
  IN UINT8             WarningCode,
  IN UINT8             MinorWarningCode,
  IN UINT8             Socket,
  IN UINT8             Channel,
  IN UINT8             Dimm,
  IN UINT8             Rank,
  IN EWL_SEVERITY      Severity,
  IN ADV_MT_TYPE       MemtestType,
  IN EWL_ADV_MT_STATUS AdvMemtestErrorInfo,
  IN UINT32            Count
  );

/**

  Construct a EWL type 5 warning.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Socket.
  @param[in]  Channel               Channel.
  @param[in]  Dimm                  Dimm.
  @param[in]  Rank                  Rank.
  @param[in]  Severity              Severity.
  @param[in]  MemtestType           MemtestType.
  @param[in]  AdvMemtestErrorInfo   AdvMemtestErrorInfo.
  @param[in]  Count                 Count.

  @retval None

**/

VOID
EFIAPI
EwlOutputType5 (
  IN UINT8        WarningCode,
  IN UINT8        MinorWarningCode,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN UINT8        Dimm,
  IN UINT8        Rank,
  IN EWL_SEVERITY Severity,
  IN UINT8        SubRank,
  IN UINT8        BankAddress,
  IN UINT8        *DqBytes
  );

/**

  Construct a EWL type 7 warning.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Socket.
  @param[in]  BistMask              BistMask.

  @retval None

**/

VOID
EFIAPI
EwlOutputType7 (
  IN UINT8        WarningCode,
  IN UINT8        MinorWarningCode,
  IN UINT8        Socket,
  IN UINT64       BistMask
  );

/**

  Construct a EWL type 10 warning.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Socket.
  @param[in]  Channel               Channel.
  @param[in]  Dimm                  Dimm.
  @param[in]  Rank                  Rank.
  @param[in]  Severity              Severity.
  @param[in]  SwizzlePattern        SwizzlePattern.
  @param[in]  SwizzledDqLanes       SwizzledDqLanes.
  @param[in]  LanesPerStrobe        LanesPerStrobe.
  @param[in]  Strobe                Strobe.

  @retval None

**/

VOID
EFIAPI
EwlOutputType10 (
  IN UINT8        WarningCode,
  IN UINT8        MinorWarningCode,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN UINT8        Dimm,
  IN UINT8        Rank,
  IN EWL_SEVERITY Severity,
  IN UINT8        SwizzlePattern,
  IN UINT8        SwizzledDqLanes,
  IN UINT8        LanesPerStrobe,
  IN UINT8        Strobe
  );

/**

  Construct a EWL type 11 warning.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Socket.
  @param[in]  Channel               Channel.
  @param[in]  Dimm                  Dimm.
  @param[in]  Rank                  Rank.
  @param[in]  Severity              Severity.
  @param[in]  Data                  Data.

  @retval None

**/

VOID
EFIAPI
EwlOutputType11 (
  IN UINT8        WarningCode,
  IN UINT8        MinorWarningCode,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN UINT8        Dimm,
  IN UINT8        Rank,
  IN EWL_SEVERITY Severity,
  IN UINT32       Data
  );

/**

  Construct a EWL type 12 warning.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Socket.
  @param[in]  Channel               Channel.
  @param[in]  Dimm                  Dimm.
  @param[in]  Rank                  Rank.
  @param[in]  Severity              Severity.
  @param[in]  Command               Command.
  @param[in]  Status                Status.

  @retval None

**/

VOID
EFIAPI
EwlOutputType12 (
  IN UINT8        WarningCode,
  IN UINT8        MinorWarningCode,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN UINT8        Dimm,
  IN UINT8        Rank,
  IN EWL_SEVERITY Severity,
  IN UINT16       Command,
  IN UINT8        Status
  );

/**

  Construct a EWL type 13 warning.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Socket.
  @param[in]  Channel               Channel.
  @param[in]  Dimm                  Dimm.
  @param[in]  Rank                  Rank.
  @param[in]  Severity              Severity.
  @param[in]  RevisionNvmdimmFw     RevisionNvmdimmFw.
  @param[in]  RevisionNvmdimmSpd    RevisionNvmdimmSpd.

  @retval None

**/

VOID
EFIAPI
EwlOutputType13 (
  IN UINT8        WarningCode,
  IN UINT8        MinorWarningCode,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN UINT8        Dimm,
  IN UINT8        Rank,
  IN EWL_SEVERITY Severity,
  IN UINT16       RevisionNvmdimmFw,
  IN UINT8        RevisionNvmdimmSpd
  );

VOID
EwlOutputType23 (
  UINT8     WarningCode,
  UINT8     MinorWarningCode,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT32    Bsr,
  UINT32    BsrHi
  );

  /**

  Construct a EWL type 24 warning.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Severity              Severity.

  @retval None

**/

VOID
EFIAPI
EwlOutputType24 (
  IN UINT8        WarningCode,
  IN UINT8        MinorWarningCode,
  IN EWL_SEVERITY Severity
  );

  /**

  Construct a EWL type 25 warning. This warning logs BOOT Health check errors.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  MemoryLocation        Memory location where the error occured.
  @param[in]  Severity              Severity.
  @param[in]  Group                 Group.
  @param[in]  Offset                Margin Offset.

  @retval None

**/
VOID
EFIAPI
EwlOutputType25 (
  IN UINT8                      WarningCode,
  IN UINT8                      MinorWarningCode,
  IN EWL_ENTRY_MEMORY_LOCATION  MemoryLocation,
  IN EWL_SEVERITY               Severity,
  IN MRC_GT                     Group,
  IN INT16                      Offset
  );

/**

  Construct a EWL type 26 warning. This warning logs Memory Power Management errors.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  MemoryLocation        Memory location where the error occured.
  @param[in]  Severity              Severity.

  @retval None

**/
VOID
EFIAPI
EwlOutputType26 (
  IN UINT8                      WarningCode,
  IN UINT8                      MinorWarningCode,
  IN EWL_ENTRY_MEMORY_LOCATION  MemoryLocation,
  IN EWL_SEVERITY               Severity
  );

/**

  Construct a EWL type 27 warning. This warning logs NVMDIMM Media errors.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Socket.
  @param[in]  Channel               Channel.
  @param[in]  Dimm                  Dimm.
  @param[in]  Log                   Log.

  @retval None

**/
VOID
EwlOutputType27 (
  UINT8               WarningCode,
  UINT8               MinorWarningCode,
  UINT8               Socket,
  UINT8               Channel,
  UINT8               Dimm,
  UINT8               Log[32]
  );

/**

  Construct a EWL type 28 warning. This warning logs NVMDIMM Thermal errors.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Socket.
  @param[in]  Channel               Channel.
  @param[in]  Dimm                  Dimm.
  @param[in]  Log                   Log.

  @retval None

**/
VOID
EwlOutputType28 (
  UINT8                 WarningCode,
  UINT8                 MinorWarningCode,
  UINT8                 Socket,
  UINT8                 Channel,
  UINT8                 Dimm,
  UINT8                 Log[16]
  );

/**

  Initialize the enhanced warning log.

  This function will initialize the enhanced warning log public
  and private data structures.

  @param  - None

  @retval - SUCCESS

**/

UINT32
EFIAPI
EwlInit (
  VOID
  );

#endif // #ifndef _ENHANCED_WARNING_LOG_LIB_
