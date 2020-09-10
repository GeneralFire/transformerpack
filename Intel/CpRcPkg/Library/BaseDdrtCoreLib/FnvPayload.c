/** @file
  FalconValley payload table

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

#include "SysHost.h"
#include <Library/DdrtCoreLib.h>

/*
*Updated 10/01/2019*
*NVMCTLR FIS 2.02*
*/

UINT32 FnvPayloadLookupTable [FNV_OPCODE_ARRAY_SIZE][5] = {

    //Opcode                            Input Payload Registers       Output Payload Registers   Implemented Command    R/W                       Description
    {NULL_COMMAND,                      0,                            0,                         0,                     0},                      // 0. null command
    {IDENTIFY_DIMM,                     2,                           21,                         1,                     0},                      // 1. Identify Dimm
    {DEVICE_CHARACTERISTICS,            0,                            8,                         1,                     0},                      // 2. Device Characteristics
    {GET_SECURITY_STATE,                0,                            2,                         1,                     0},                      // 3. Get Security state
    {SET_SECURITY_NONCE,                2,                            0,                         1,                     0},                      // 4. Set SMM Nonce
    {OVERWRITE_DIMM,                    8,                            0,                         1,                     0},                      // 5. Overwrite DIMM
    {SET_MASTERPASSPHRASE,             32,                            0,                         0,                     1},                      // 6. Set Master Passphrase
    {SET_PASSPHRASE,                   32,                            0,                         0,                     1},                      // 7. Set Passphrase
    {DISABLE_PASSPHRASE,               32,                            0,                         0,                     1},                      // 8. Disable Passphrase
    {UNLOCK_UNIT,                      32,                            0,                         0,                     1},                      // 9. Unlock Unit
    {SECURE_ERASE_UNIT,                32,                            0,                         1,                     1},                      // 10. Secure Erase Prepare *Not Listed*
    {FREEZE_LOCK,                       1,                            0,                         0,                     1},                      // 11. Freeze Lock *Not Listed*
    {SECURE_ERASE_POLICY,               1,                            0,                         0,                     1},                      // 12. Secure Erase Policy
    {GET_ALARM_THRESHOLDS,              0,                            2,                         0,                     0},                      // 13. Get Alarm Thresholds
    {GET_POWER_MANAGEMENT,              0,                            4,                         1,                     0},                      // 14. Get Power Management Policy *Not Listed*
    {GET_DIE_SPARING,                   0,                            1,                         0,                     0},                      // 15. Get Die Sparring Policy *Not Listed*
    {GET_ADDRESS_RANGE_SCRUB,           1,                            0,                         0,                     0},                      // 16. Get Address Range Srcub Policy *Not Listed*
    {GET_DDRT_ALERTS,                   0,                           32,                         1,                     0},                      // 17. Get NVMDIMM Alerts
    {GET_CONFIG_DATA_POLICY,            1,                            0,                         0,                     0},                      // 18. Get Configuration Data Policy
    {GET_PMON_REGISTERS,                1,                           32,                         0,                     0},                      // 19. Get PMON registers
    {RESET_ALL_THRESHOLDS,              2,                            0,                         0,                     1},                      // 20. Set Reset to Defaults
    {SET_ALARM_THRESHOLDS,              2,                            0,                         0,                     1},                      // 21. Set Alarm Thresholds
    {SET_POWER_MANAGEMENT,              4,                            0,                         1,                     1},                      // 22. Set Power Management Policy *Not Listed*
    {SET_DIE_SPARING,                   1,                            0,                         0,                     1},                      // 23. Set Die Sparring Policy
    {SET_ADDRESS_RANGE_SCRUB,           5,                            0,                         0,                     1},                      // 24. Set Address Range Scrub Policy *Not Listed*
    {SET_DDRT_ALERTS,                  32,                            0,                         1,                     1},                      // 25. Set NVMDIMM Alerts
    {SET_CONFIG_DATA_POLICY,            1,                            0,                         0,                     0},                      // 26. Set Configuration Data Policy
    {SET_PMON_REGISTERS,               32,                            0,                         0,                     0},                      // 27. Set PMON registers
    {GET_SYSTEM_TIME,                   2,                            2,                         1,                     0},                      // 28. Get Admin System Time
    {GET_PLATFORM_CONFIG,               2,                           32,                         1,                     0},                      // 29. Get Admin Platform Config Data
    {GET_DIMM_PARTITION,                0,                            9,                         1,                     0},                      // 30. Get Admin Dimm Partition Info
    {(0x06) | (0x04 << 8),              0,                            0,                         0,                     0},                      // 31. Reserved
    {GET_CONFIG_LOCKDOWN,               1,                            1,                         0,                     0},                      // 32. Get Admin DPA Lockdown
    {Get_DDRT_INIT_INFO,                1,                            1,                         0,                     0},                      // 33. Get Admin NVMDIMM IO Init Info
    {GET_SUPPORTED_SKU,                 0,                            1,                         0,                     0},                      // 34. Get Supported SKU Features
    {GET_LATCH_SYSTEM_SHUTDOWN_STATUS,  0,                            1,                         0,                     0},                      // 35. Get Latch System Shutdown Status
    {GET_COMMAND_ACCESS_POLICY,         1,                            1,                         0,                     0},                      // 36. Get Command Access Policy
    {SET_SYSTEM_TIME,                   2,                            0,                         1,                     1},                      // 37. Set Admin System Time
    {SET_PLATFORM_CONFIG,              32,                            0,                         1,                     1},                      // 38. Set Admin Platform Config Data
    {SET_DIMM_PARTITION,                2,                            0,                         1,                     1},                      // 39. Set Admin Dimm Partition Info
    {(0x07) | (0x04 << 8),              0,                            0,                         0,                     0},                      // 40. Reserved
    {SET_CONFIG_LOCKDOWN,               1,                            0,                         0,                     1},                      // 41. Set Admin DPA Lockdown
    {DDRT_IO_INIT,                      1,                            0,                         0,                     1},                      // 42. Set Admin NVMCTLR NVMDIMM IO init
    {SET_CREATE_SKU,                    1,                            0,                         0,                     1},                      // 43. Set Create SKU
    {SET_LATCH_SYSTEM_SHUTDOWN_STATUS,  1,                            0,                         0,                     1},                      // 44. Set Latch System Shutdown Status
    {GET_SMART_HEALTH,                  0,                           32,                         0,                     0},                      // 45. Get Log Page Smart & Health Info
    {GET_FIRMWARE_IMAGE,                0,                           32,                         0,                     0},                      // 46. Get Log Page Firmware Image Information
    {GET_FIRMWARE_DEBUG,                3,                           32,                         0,                     0},                      // 47. Get Log Page Firmware Debug Log
    {GET_MEMORY_INFO,                   1,                           32,                         0,                     0},                      // 48. Get Log Page Memory Test Log
    {GET_LONG_OPERATION,                1,                           32,                         0,                     0},                      // 49. Get Log Page Long Operation Status
    {GET_ERROR_LOG,                    32,                           32,                         1,                     0},                      // 50. Get Log Page Error Log
    {GET_COMMAND_EFFECT_LOG,            1,                           32,                         0,                     0},                      // 51. Get Command Effect Log
    {UPDATE_FNV_FW,                    17,                            0,                         0,                     1},                      // 52. Update Firmware Update NVMCTLR FW
    {EXECUTE_FNV_FW,                    1,                            0,                         0,                     1},                      // 53. Update Firmware Execute NVMCTLR FW
    {SVN_DOWNGRADE_OPT_IN,              1,                            0,                         0,                     1},                      // 54. SVN Downgrade Opt-In
    {ENABLE_ERR_INJECTION,              1,                            0,                         0,                     1},                      // 55. Inject Error Enable Injection
    {INJECT_POISON_ERROR,               4,                            4,                         0,                     1},                      // 56. Inject Error Poison Error
    {INJECT_TEMP_ERROR,                 1,                            0,                         0,                     1},                      // 57. Inject Error Temperature Error
    {INJECT_SW_TRIGGER,                 4,                            0,                         0,                     1},                      // 58. Inject Software Triggers
    {GET_VIRAL_POLICY,                  0,                            2,                         1,                     0},                      // 59. Get Viral Policy
    {SET_VIRAL_POLICY,                  2,                            0,                         1,                     1},                      // 60. Set Viral Policy
    {GET_EADR_STATE,                    0,                            1,                         1,                     0},                      // 61. Get eADR state
    {SET_EADR_STATE,                    1,                            0,                         1,                     1},                      // 62. Set eADR state
    {GET_PERFORMANCE_KNOB,              1,                            1,                         1,                     0},                      // 63. Get Performance Knob
    {SET_PERFORMANCE_KNOB,              1,                            0,                         1,                     1},                      // 64. Set Performance Knob
    {CR_FACTORY_RESET,                  0,                            0,                         1,                     1},                      // 65. Factory reset/clear
    {SET_PROTECTED_REG,                 4,                            0,                         1,                     1},                      // 66. Write to protected registers
    {SET_ADDITIONAL_SETTINGS,           1,                            0,                         0,                     1},                      // 67. Set Additional Settings
    {INJ_MODULE_FRAMEWORK_UPLOAD,      32,                           32,                         1,                     1},                      // 68. Injection Module Framework Upload
    {INJ_MODULE_FRAMEWORK_EXECUTE,     32,                           32,                         1,                     1},                      // 69. Injection Module Framework Execute
    {INJ_MODULE_FRAMEWORK_STATUS,      32,                           32,                         1,                     1},                      // 70. Injection Module Framework Status
    {S3_AUTHENTICATE,                  8,                            0,                          1,                     1},                      // 71. S3 Authenticate
    {S3_PASSPHRASE,                    24,                           0,                          1,                     1},                      // 72. S3 Passphrase
    {SET_SECURITY_OPT_IN,              2,                            0,                          1,                     1},                      // 73. Set Security Opt-In
    {GET_SECURITY_OPT_IN,              1,                            32,                         1,                     1},                      // 74. Get Security Opt-In
};

