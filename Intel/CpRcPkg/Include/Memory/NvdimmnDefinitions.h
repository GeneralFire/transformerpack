/** @file
  NVDIMM-N Definitions and declarations.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef _NVDIMMN_DEFINITIONS_H_
#define _NVDIMMN_DEFINITIONS_H_

#define SPECREV                    0x06
#define CAPABILITIES               0x10
#define ERROR_INJECTION_SUPPORTED  (1<<2)
#define CSAVE_FAILURE_COUNT0       0x12
#define CSAVE_FAILURE_COUNT1       0x13
#define ENERGY_SOURCE_POLICY       0x14
#define DEVICE_MANAGED_POLICY      (1<<0)
#define HOST_MANAGED_POLICY        (1<<1)
#define CSAVE_TIMEOUT0             0x18
#define CSAVE_TIMEOUT1             0x19
#define PAGE_SWITCH_LATENCY0       0x1A
#define PAGE_SWITCH_LATENCY1       0x1B
#define RESTORE_TIMEOUT0           0x1C
#define RESTORE_TIMEOUT1           0x1D
#define ERASE_TIMEOUT0             0x1E
#define ERASE_TIMEOUT1             0x1F
#define ARM_TIMEOUT0               0x20
#define ARM_TIMEOUT1               0x21

#define NVDIMM_MGT_CMD0            0x40
#define RST_CTRL                   BIT0;
#define CL_SAVE_STAT               (1<<2)
#define CL_RESTORE_STAT            (1<<3)
#define CL_ERASE_STAT              (1<<4)
#define CL_ARM_STAT                (1<<5)
#define CL_SET_ES_STAT             (1<<7)
#define NVDIMM_MGT_CMD1            0x41
#define NVDIMM_FUNC_CMD0           0x43
#define START_SAVE                 (1<<1)
#define START_RESTORE              (1<<2)
#define START_ERASE                (1<<3)
#define ABORT_CURRENT_OP           (1<<4)
#define ARM_CMD                    0x45
#define ARM_FOR_SAVE_N             (1<<0)

#define SET_EVENT_NOTIFICATION_CMD 0x47
#define SET_ES_POLICY_CMD          0x49
#define DEVICE_MANAGED_POLICY      (1<<0)
#define HOST_MANAGED_POLICY        (1<<1)

#define INJECT_OPS_FAILURES0       0x60
#define INJECT_CSAVE_FAILURE       (1<<0)
#define INJECT_RESTORE_FAILURE     (1<<1)
#define INJECT_ERASE_FAILURE       (1<<2)
#define INJECT_ARM_FAILURE         (1<<3)
#define INJECT_INTERNAL_CONTROLLER_FAILURE (1<<4)
#define INJECT_NVM_LIFETIME_WARNING (1<<5)
#define INJECT_NVM_LIFETIME_ERROR  (1<<6)
#define INJECT_BAD_BLOCKS          (1<<7)

#define INJECT_OPS_FAILURES1       0x61
#define INJECT_PERMANENT_HARDWARE_FAILURE (1<<0)
#define INJECT_FACTORY_DEFAULT_FAILURE    (1<<1)
#define INJECT_SET_EVENT_NOTIFICATION_FAILURE (1<<2)
#define INJECT_SET_ES_POLICY_FAILURE      (1<<3)
#define INJECT_FIRMWARE_OPS_FAILURE       (1<<4)
#define INJECT_OPERATIONAL_UNIT_OPS_FAILURE (1<<5)

#define NVDIMM_READY               0x60
#define NVDIMM_CMD_STATUS          0x61
#define CONTROLLER_BUSY            (1<<0)
#define FACTORY_DEFAULT_IN_PROGRESS (1<<1)
#define SAVE_IN_PROGRESS           (1<<2)
#define RESTORE_IN_PROGRESS        (1<<3)
#define ERASE_IN_PROGRESS          (1<<4)
#define ABORT_IN_PROGRESS          (1<<5)
#define ARM_IN_PROGRESS            (1<<6)
#define FIRMWARE_OPS_IN_PROGRESS   (1<<7)

#define INJECT_ES_FAILURES         0x64 // Page 2
#define INJECT_ES_FAILURE          (1<<0)
#define INJECT_ES_ASSESEMENT_FAILURE (1<<1)
#define INJECT_ES_LIFETIME_WARNING (1<<2)
#define INJECT_ES_LIFETIME_ERROR   (1<<3)
#define INJECT_ES_TEMP_WARNING     (1<<4)
#define INJECT_ES_TEMP_ERROR       (1<<5)

#define SAVE_STATUS                0x64 // Page 0
#define SAVE_SUCCESS               (1<<0)
#define SAVE_ERROR                 (1<<1)
#define ABORT_SUCCESS              (1<<4)
#define ABORT_ERROR                (1<<5)

#define RESTORE_STATUS0            0x66
#define RESTORE_SUCCESS            (1<<0)
#define RESTORE_ERROR              (1<<1)
#define INJECT_ERROR_TYPE          0x68 // Page 2
#define ONE_TIME_USE               (1<<0)
#define ERASE_STATUS0              0x68 // Page 0
#define ERASE_SUCCESS              (1<<0)
#define ERASE_ERROR                (1<<1)
#define ARM_STATUS0                0x6A
#define ARM_SUCCESS                (1<<0)
#define ARM_ERROR                  (1<<1)
#define SAVE_N_ARMED               (1<<2)
#define FACTORY_DEFAULT_STATUS0    0x6C
#define FACTORY_DEFAULT_SUCCESS    (1<<0)
#define FACTORY_DEFAULT_ERROR      (1<<1)
#define SET_EVENT_NOTIFICATION_STATUS0  0x6E
#define SET_EVENT_NOTIFICATION_SUCCESS (1<<0)
#define SET_EVENT_NOTIFICATION_ERROR (1<<1)
#define PERSISTENCY_ENABLED        (1<<2)
#define WARNING_THRESHOLD_ENABLED  (1<<3)
#define VOLTAGE_REGULATOR_FAIL_ENABLED (1<<4)
#define SET_ES_POLICY_STATUS       0x70
#define SET_ES_POLICY_SUCCESS      (1<<0)
#define SET_ES_POLICY_ERROR        (1<<1)
#define DEVICE_MANAGED_POLICY_ENABLED (1<<2)
#define HOST_MANAGED_POLICY_ENABLED (1<<3)

#define FIRMWARE_OPS_STATUS0       0x71
#define FIRMWARE_OPS_SUCCESS       (1<<0)
#define FIRMWARE_OPS_ERROR         (1<<1)
#define FIRMWARE_UPDATE_MODE       (1<<1)
#define ERASE_FAIL_INFO            0x73

#define CSAVE_INFO                 0x80
#define NVM_DATA_VALID             (1<<0)
#define START_SAVE_CMD             (1<<1)
#define SAVE_N                     (1<<2)
#define CSAVE_FAIL_INFO0           0x84
#define VOLTAGE_REGULATOR_FAILED   (1<<0)
#define VDD_LOST                   (1<<1)
#define VPP_LOST                   (1<<2)
#define VTT_LOST                   (1<<3)
#define DRAM_NOT_SELF_REFRESH      (1<<4)
#define CONTROLLER_HARDWARE_ERROR  (1<<5)
#define NVM_CONTROLLER_ERROR       (1<<6)
#define NVM_MEDIA_ERROR            (1<<7)
#define CSAVE_FAIL_INFO1           0x85
#define NOT_ENOUGH_ENERGY_FOR_CSAVE (1<<0)
#define PARIAL_DATA_SAVED          (1<<1)

#define MODULE_HEALTH              0xA0
#define PERSISTENCY_LOST_ERROR     (1<<0)
#define WARNING_THRESHOLD_EXCEEDED (1<<1)
#define PERSISTENCY_RESTORED       (1<<2)
#define BELOW_WARNING_THRESHOLD    (1<<3)
#define PERMANENT_HARDWARE_FAILURE (1<<4)
#define MODULE_HEALTH_STATUS0      0xA1
#define NVM_LIFETIME_ERROR        (1<<7)
#define MODULE_HEALTH_STATUS1     0xA2
#define INVALID_FIRMWARE_ERROR    (1<<1)
#define CONFIG_DATA_ERROR         (1<<2)
#define NO_ES_PRESENT             (1<<3)
#define ES_POLICY_NOT_SET         (1<<4)
#define ES_HARDWARE_FAILURE       (1<<5)
#define ES_HEALTH_ASSESSMENT_ERROR (1<<6)
#define ERROR_THRESHOLD_STATUS    0xA5
#define NVM_LIFETIME_ERROR1       (1<<0)
#define ES_LIFETIME_ERROR         (1<<1)
#define ES_TEMP_ERROR             (1<<2)
#define WARNING_THRESHOLD_STATUS  0xA7
#define NVM_LIFETIME_WARNING      (1<<0)
#define ES_LIFETIME_WARNING       (1<<1)
#define ES_TEMP_WARNING           (1<<2)
#define MODULE_OPS_CONFIG         0xAA


#endif /* _NVDIMMN_DEFINITIONS_H_ */
