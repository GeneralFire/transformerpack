/** @file

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


#ifndef _fnvopcode_h
#define _fnvopcode_h

//
// NVMCTLR FIS 2.04
//
#define FNV_OPCODE_ARRAY_SIZE                75
#define NULL_COMMAND                         0x0000
#define IDENTIFY_DIMM                        0x0001
#define DEVICE_CHARACTERISTICS               0x0101
#define GET_SECURITY_STATE                   0x0002
#define GET_SECURITY_OPT_IN                  0x0202
#define SET_SECURITY_NONCE                   0x0003
#define SET_MASTERPASSPHRASE                 0xF003
#define SET_SECURITY_OPT_IN                  0x0203
#define OVERWRITE_DIMM                       0x0103
#define SET_PASSPHRASE                       0xF103
#define DISABLE_PASSPHRASE                   0xF203
#define UNLOCK_UNIT                          0xF303
#define SECURE_ERASE_UNIT                    0xF503
#define FREEZE_LOCK                          0xF603
#define SECURE_ERASE_POLICY                  0xF703
#define S3_AUTHENTICATE                      0xF803
#define S3_PASSPHRASE                        0xF903
#define GET_ALARM_THRESHOLDS                 0x0104
#define GET_POWER_MANAGEMENT                 0x0204
#define GET_DIE_SPARING                      0x0304
#define GET_ADDRESS_RANGE_SCRUB              0x0404
#define GET_DDRT_ALERTS                      0x0504
#define GET_CONFIG_DATA_POLICY               0x0604
#define GET_PMON_REGISTERS                   0x0704
#define RESET_ALL_THRESHOLDS                 0x0005
#define SET_ALARM_THRESHOLDS                 0x0105
#define SET_POWER_MANAGEMENT                 0x0205
#define SET_DIE_SPARING                      0x0305
#define SET_ADDRESS_RANGE_SCRUB              0x0405
#define SET_DDRT_ALERTS                      0x0505
#define SET_CONFIG_DATA_POLICY               0x0605
#define SET_PMON_REGISTERS                   0x0705
#define GET_SYSTEM_TIME                      0x0006
#define GET_PLATFORM_CONFIG                  0x0106
#define GET_DIMM_PARTITION                   0x0206
#define GET_CONFIG_LOCKDOWN                  0x0506
#define Get_DDRT_INIT_INFO                   0x0606
#define GET_SUPPORTED_SKU                    0x0706
#define GET_LATCH_SYSTEM_SHUTDOWN_STATUS     0x0906
#define GET_VIRAL_POLICY                     0x0A06
#define GET_COMMAND_ACCESS_POLICY            0xCA06
#define GET_EADR_STATE                       0xEA06
#define GET_ENABLE_DIMM                      0x0806
#define SET_SYSTEM_TIME                      0x0007
#define SET_PLATFORM_CONFIG                  0x0107
#define SET_DIMM_PARTITION                   0x0207
#define SET_CONFIG_LOCKDOWN                  0x0507
#define DDRT_IO_INIT                         0x0607
#define SET_CREATE_SKU                       0x0707
#define SET_ENABLE_DIMM                      0x0807
#define SET_LATCH_SYSTEM_SHUTDOWN_STATUS     0x0907
#define SET_VIRAL_POLICY                     0x0A07
#define CR_FACTORY_RESET                     0x0B07
#define SET_PROTECTED_REG                    0x0C07
#define SET_COMMAND_ACCESS_POLICY            0xCA07
#define SET_EADR_STATE                       0xEA07
#define GET_SMART_HEALTH                     0x0008
#define GET_FIRMWARE_IMAGE                   0x0108
#define GET_FIRMWARE_DEBUG                   0x0208
#define GET_MEMORY_INFO                      0x0308
#define GET_LONG_OPERATION                   0x0408
#define GET_ERROR_LOG                        0x0508
#define GET_COMMAND_EFFECT_LOG               0xFF08
#define UPDATE_FNV_FW                        0x0009
#define EXECUTE_FNV_FW                       0x0109
#define SVN_DOWNGRADE_OPT_IN                 0x0209
#define ENABLE_ERR_INJECTION                 0x000A
#define INJECT_POISON_ERROR                  0x010A
#define INJECT_TEMP_ERROR                    0x020A
#define INJECT_SW_TRIGGER                    0x030A
#define GET_PERFORMANCE_KNOB                 0x0804
#define SET_PERFORMANCE_KNOB                 0x0805
#define SET_ADDITIONAL_SETTINGS              0x00FF
#define INJ_MODULE_FRAMEWORK_UPLOAD          0x00C7
#define INJ_MODULE_FRAMEWORK_EXECUTE         0x01C7
#define INJ_MODULE_FRAMEWORK_STATUS          0x02C7

//
// Major code
//
#define MC_IDENTIFY_DIMM                     0x01
#define MC_GET_SECURITY_INFO                 0x02
#define MC_SET_SECURITY_INFO                 0x03
#define MC_GET_FEATURES                      0x04
#define MC_SET_FEATURES                      0x05
#define MC_GET_ADMIN_FEATURES                0x06
#define MC_SET_ADMIN_FEATURES                0x07
#define MC_GET_LOG_PAGE                      0x08
#define MC_UPDATE_FIRMWARE                   0x09
#define MC_INJECT_ERROR                      0x0A
#define MC_SET_DEBUG_FEATURES                0xE3
#define MC_INJECTION_MODULE_FRAMEWORK        0xC7
#define MC_SET_VENDOR_SPECIFIC_COMMANDS      0xFF


#endif // _fnvopcode_h
