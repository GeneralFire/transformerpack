/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2020 Intel Corporation. <BR>

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

#ifndef _fnvaccesscommon_h
#define _fnvaccesscommon_h

#include "SysHost.h"
#include "NGNDimmPlatformCfgData.h"
#include "MemHostChipCommon.h"
#include <Library/DdrtCoreLib.h>

//
//  DIMM SKU. Enabled/Capable if set
//
#define DIMM_SKU_MEMORY_MODE                                          BIT0
#define DIMM_SKU_STORAGE_MODE                                         BIT1
#define DIMM_SKU_APP_DIRECT                                           BIT2
#define DIMM_SKU_PACKAGE_SPARING                                      BIT3

//
// Convert FIS bit number to the internal BIOS bit number
// (BIOS converts FIS four byte DIMM Sku value to one byte value)
//
#define DIMM_SKU_ENCRYPTION                                           BIT5

#define DIMM_SKU_MASK                                                 (DIMM_SKU_MEMORY_MODE     |\
                                                                       DIMM_SKU_STORAGE_MODE    |\
                                                                       DIMM_SKU_APP_DIRECT      |\
                                                                       DIMM_SKU_ENCRYPTION)

#define OPCODE_INDEX             0
#define INPUT_PAYLOAD_INDEX      1
#define OUTPUT_PAYLOAD_INDEX     2
#define IMPLEMENTED_INDEX        3
#define RW_INDEX                 4
#define BUSY                     1
#define SMALL_PAYLOAD            1
#define RETRIEVE_PARTITION_DATA  0
#define RETRIEVE_PARTITION_SIZE  1
#define NUM_SPARABLE_RANKS       4


#define DOORBELL_TIMEOUT                      700       // 700 us for simics 190 us actual
#define DOORBELL_TIMEOUT_UPDATE_FNV_FW        2000000   // 2s timeout for update FW
#define COMP_TIMEOUT                          700000    // 700 ms for simics 190 us actual
#define FMC_CONFIG_SMBUS_TIMEOUT_MICROSECONDS 2505300

#define CR_DDRT_PLATFORM_DELAY                                       1400      // 1.4ms platform delay if command send via DDRT
#define CR_BIOS_MB_READY_POLLING_TIME                                30        // 0.03ms doorbell and mb delayed polling time
#define CR_BIOS_POLLING_TIME                                         5000      // 5ms bios polling time
#define CR_MB_DELAYED_TIMEOUT                                        4000      // 2 ms MB delayed timeout + 2 ms margin for safety
#define CR_DOORBELL_TIMEOUT                                          200       // 0.2 ms execution time for most of commands except for these below
#define CR_DOORBELL_TIMEOUT_SECURITY_CMD_TIMEOUT                     50000     // 50ms for all Set Security Info commands (Major code 03h)
#define CR_DOORBELL_TIMEOUT_RESET_ALL_THRESHOLDS                     100000    // 100ms for Reset All Thresholds (Major code 05h, Minor code 00h)
#define CR_DOORBELL_TIMEOUT_GET_PLATFORM_CONFIG_DATA                 10000     // 10ms for GetPCD (Major code 06h, Minor code 01h)
#define CR_DOORBELL_TIMEOUT_SET_PLATFORM_CONFIG_DATA                 10000     // 10ms for SetPCD (Major code 07h, Minor code 01h)
#define CR_DOORBELL_TIMEOUT_SET_DDDRT_IO_INIT_INFO(Fis)              ((Fis) < 0x0106 ? \
                                                                      200000 :         \
                                                                      100000)// 100ms for Set DDRT IO Init Info (Major code 07h, Minor code 06h)
#define CR_DOORBELL_TIMEOUT_GET_FIRMWARE_DEBUG                       80000     // 80ms for Get Firmware Debug Log (Major code 08h, Minor code 02h)
#define CR_DOORBELL_TIMEOUT_GET_COMMAND_EFFECT_LOG                   400       // 0.4ms for Get Command Effect Log (Major code 08h, Minor code FFh)
#define CR_DOORBELL_TIMEOUT_UPDATE_FNV_FW(Fis)                       ((Fis) < 0x0106 ? \
                                                                      4000000 :        \
                                                                      100000)  // 100ms timeout for update FW (Major code 09h, Minor code 00h)
#define CR_DOORBELL_TIMEOUT_INJECT_SW_TRIGGER(Fis)                   ((Fis) < 0x0106 ? \
                                                                      7000000 :        \
                                                                      50000)   // 50ms timeout for Software Triggers (Major code 0Ah, Minor code 03h)


#define LARGE_PAYLOAD                         0
#define SMALL_PAYLOAD                         1
#define TEN_MICRO_SECOND_DELAY                10
#define SECURITY_ENABLED                      0x02
#define SECURITY_LOCKED                       0x04
#define SECURITY_FROZEN                       0x08
#define MASTER_PASSPHRASE_ENABLED             BIT8
#define SECURE_ERASE_POLICY_OPT_IN_STATUS     BIT0
#define SECURITY_EXPIRED                      4

#define UNC_ERROR_INTERRUPT      BIT7
#define UNC_ERROR_VIRAL          BIT6
#define DPA_ERROR_INTERRUPT      BIT5
#define DPA_ERROR_VIRAL          BIT4
#define AIT_ERROR_INTERRUPT      BIT3
#define AIT_ERROR_VIRAL          BIT2
#define NO_LOG                   0
#define LOW_PRIORITY             1
#define HIGH_PRIORITY            2
#define FNV_IO_READ              0
#define FNV_IO_WRITE             1
#define FMC_RESPONSE_TIMEOUT_MICROSECONDS_HARDWARE  357900
#define FMC_RESPONSE_TIMEOUT_MICROSECONDS_SIMICS    358
#define BYTE0    0x1
#define BYTE1    0x2
#define BYTE2    0x4
#define BYTE3    0x8
#define ALL_BYTE 0xF
#define MAJOR_CHECKPOINT 0x00FF
#define MINOR_CHECKPOINT 0xFF00
#define COMMAND_LATENCY_DEFAULT 0x02
#define PASSPHRASE_COUNTER  8
#define PASSPHRASE_LENGTH 32
#define NEW_PASSPHRASE_START 16   //new passphrase start in 4 bytes unit

#define FNV_MB_READY_TIMEOUT     5000000    // 5 sec
#define FNV_DRAM_READY_TIMEOUT   60000000  // 60 sec
#define  FNV_DDRTIO_INIT_TIMEOUT  500000    // 500 msec
#define  FNV_MEDIA_READY_TIMEOUT  500000    // 500 msec
#define  FNV_CREDIT_READY_TIMEOUT 500000    // 500 msec

// Boot Status Register bits
#define FNV_MEDIA_DONE     (BIT17 | BIT16) // Media Ready           BIT 17:16 - 00 - Not ready, 01 - Ready, 10 - Error, 11 - Reserved
#define FNV_MEDIA_READY    BIT16
#define FNV_MEDIA_ERROR    BIT17
#define FNV_DT_DONE        BIT18           // DDRT IO Init Complete BIT 18    -  0 - Not ready,  1 - Ready
#define FNV_CSR_UNLOCK     BIT19
#define FNV_MB_READY       BIT20           // Mailbox Ready         BIT 20    -  0 - Not ready,  1 - Ready
#define FNV_CREDIT_READY   BIT23           // Cready Ready          BIT 23    -  0 - Not ready,  1 - Ready
#define FNV_MEDIA_DISABLED BIT24           // Media Disabled        BIT 24    -  0 - Enabled,    1 - Disabled
#define FNV_DRAM_READY     (BIT28 | BIT27) // DRAM Ready            BIT 28:27 - 00 - Not Trained/Not Loaded, 01 - Trained/Not Loaded, 11 - Trained/Loaded, 10 - Error
#define FNV_DRAM_ERROR     BIT28
#define FNV_DRAM_TRAINED   BIT27
#define FNV_LINKFAILONPREVBOOT BIT30

#define FNV_MEDIA_IS_USABLE(Bsr)  ((Bsr).Bits.MediaReady == MEDIA_READY && !(Bsr).Bits.MediaDisabled)
#define FNV_PREV_BOOT_ERROR(Bsr)  ((Bsr).Bits.LinkFailureOnPreviousBoot == TRUE)

#define PLL_LOCK                 1
#define DDRT_FREQ                2
#define DDRT_TRAINING_COMPLETE   3
#define DDRT_TRAINING_S3_COMPLETE 4
#define DDRT_ENTER_NORMAL_MODE   5

#define MAJOR_VERSION_INDEX           0
#define MINOR_VERSION_INDEX           1
#define HOTFIX_INDEX                  2
#define BUILD_INDEX                   3
#define NUM_FW_REVISION_FIELDS        4
#define NUM_FW_REVISION               2

#define CLK_MOVEMENT_FW               0
#define MAJOR_VERSION_CLK_MOVEMENT    1
#define MINOR_VERSION_CLK_MOVEMENT    0
#define HOTFIX_CLK_MOVEMENT           0
#define BUILD_CLK_MOVEMENT       0x2651
#define BCOM_MARGINING_FW             1
#define MAJOR_VERSION_BCOM_MARGINING  1
#define MINOR_VERSION_BCOM_MARGINING  0
#define HOTFIX_BCOM_MARGINING         0
#define BUILD_BCOM_MARGINING     0x3044

#define MINOR_VERSION_CLK_MOVEMENT_TEST    0x99
#define HOTFIX_CLK_MOVEMENT_TEST           0x99
#define BUILD_CLK_MOVEMENT_TEST            0x9999

#define MEDIA_READY     0x1
#define MEDIA_NOTREADY  0x0
#define MEDIA_INERROR   0x2
#define MEDIA_RESERVED  0x3
#define MEDIA_UNKNOWN   0xFF

// Feature Status Register Bits
#define BZM_DISABLED    0x0
#define BZM_ENABLED     0x1

// FIS 1.4 FW mailbox command status codes
#define MB_STAT_SUCCESS                                 0x00
#define MB_STAT_INVALID_COMMAND_PARAMETER               0x01
#define MB_STAT_DATA_TRANSFER_ERROR                     0x02
#define MB_STAT_INTERNAL_DEVICE_ERROR                   0x03
#define MB_STAT_UNSUPPORTED_COMMAND                     0x04
#define MB_STAT_DEVICE_BUSY                             0x05
#define MB_STAT_INCORRECT_PASSPHRASE_OR_SECURITY_NONCE  0x06
#define MB_STAT_FW_AUTHENTICATION_FAILED                0x07
#define MB_STAT_INVALID_SECURITY_STATE                  0x08
#define MB_STAT_SYSTEM_TIME_NOT_SET                     0x09
#define MB_STAT_DATA_NOT_SET                            0x0A
#define MB_STAT_ABORTED                                 0x0B
#define MB_STAT_NO_NEW_FW_TO_EXECUTE                    0x0C
#define MB_STAT_REVISION_FAILURE                        0x0D
#define MB_STAT_INJECTION_NOT_ENABLED                   0x0E
#define MB_STAT_CONFIG_LOCKED                           0x0F
#define MB_STAT_INVALID_ALIGNMENT                       0x10
#define MB_STAT_INCOMPATIBLE_DIMM_TYPE                  0x11
#define MB_STAT_TIMEOUT_OCCURRED                        0x12
#define MB_STAT_INVALID_COMMAND_VERSION                 0x13
#define MB_STAT_MEDIA_DISABLED                          0x14
#define MB_STAT_FW_UPDATE_ALREADY_OCCURRED              0x15
#define MB_STAT_NO_RESOURCES_AVAILABLE                  0x16
#define MB_STAT_UNDEFINED                               0xFF

// FIS 1.11 AIT DRAM Polling related macros
#define FIS_AIT_DRAM_POLL_ENABLE     0x0111              // AIT DRAM polling supported from FIS 1.11
#define FIS_AIT_DRAM_MGC_NUMBER      0xABCDEFAA          // Magic number defined in FIS 1.11
#define AIT_DRAM_POLL_ENABLE         0x1
#define AIT_DRAM_POLL_DISABLE        0x0
#define AIT_DRAM_FLAG_CHECK_ENABLE   0X1
#define AIT_DRAM_FLAG_CHECK_DISABLE  0X0

//
// FIS 1.1
//
#define FIS_114_VERSION   0x0114

//
// FIS 2.0
//
#define FIS_200_VERSION   0x0200

//
// FIS 2.02
//
#define FIS_202_VERSION   0x0202

//
// FIS 2.03
//
#define FIS_203_VERSION   0x0203

//
// FIS 2.04
//
#define FIS_204_VERSION   0x0204

//
// FIS 3.00
//
#define FIS_300_VERSION   0x0300

// Currently used FIS revision for FNV (used for BIOS - FW API revision handshake)
#define FIS_CURR_VERSION_FNV             FIS_204_VERSION

// Currently used FIS revision for CWV (used for BIOS - FW API revision handshake)
#define FIS_CURR_VERSION_CWV             FIS_300_VERSION

// FIS revision major number
#define FIS_VERSION_MAJOR(FisVersion)    ((UINT8)((FisVersion) >> 8))

// FIS revision minor number
#define FIS_VERSION_MINOR(FisVersion)    ((UINT8)((FisVersion) & 0x00FF))

// FIS revision trace message
#define FIS_VERSION_DBG_MSG(FisVersion)  "BIOS FIS rev %x.%x\n", FIS_VERSION_MAJOR (FisVersion), FIS_VERSION_MINOR (FisVersion)

#ifdef DDRT_SUPPORT
extern UINT32 FnvPayloadLookupTable [FNV_OPCODE_ARRAY_SIZE][5];
#endif // DDRT_SUPPORT

#endif // _fnvaccesscommon_h
