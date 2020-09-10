/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 Intel Corporation. <BR>

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

#ifndef _EPG_STATUS_CODE_H_
#define _EPG_STATUS_CODE_H_

//
// Computing Unit Host Processor Subclass Error Code definitions.
//
#define EFI_CU_HP_EC_MISMATCH_STEPPING              (EFI_SUBCLASS_SPECIFIC | 0x0000000E)
#define EFI_CU_HP_EC_MISMATCH_FAMILY                (EFI_SUBCLASS_SPECIFIC | 0x0000000F)
#define EFI_CU_HP_EC_MISMATCH_MODEL                 (EFI_SUBCLASS_SPECIFIC | 0x00000010)
#define EFI_CU_HP_EC_MISMATCH_FSB                   (EFI_SUBCLASS_SPECIFIC | 0x00000011)
#define EFI_CU_HP_EC_MISMATCH_SPEED                 (EFI_SUBCLASS_SPECIFIC | 0x00000012)
#define EFI_CU_HP_EC_UNSUPPORTED_PROCESSOR_FAMILY   (EFI_SUBCLASS_SPECIFIC | 0x00000013)
#define EFI_CU_HP_EC_MISMATCH_CACHE_SIZE            (EFI_SUBCLASS_SPECIFIC | 0x00000014)
#define EFI_CU_HP_EC_UNSUPPORTED_CHIPSET            (EFI_SUBCLASS_SPECIFIC | 0x00000015)
#define EFI_CU_HP_EC_MISMATCH_VOLTAGE               (EFI_SUBCLASS_SPECIFIC | 0x00000016)

//
// Computing Unit Firmware Processor Subclass Error Code definitions.
//
#define EFI_CU_FP_EC_FRB2_WATCHDOG_TIMEOUT          (EFI_SUBCLASS_SPECIFIC | 0x00000003)
#define EFI_CU_FP_EC_OS_WATCHDOG_TIMEOUT            (EFI_SUBCLASS_SPECIFIC | 0x00000004)
#define EFI_CU_FP_EC_SDR_EMPTY                      (EFI_SUBCLASS_SPECIFIC | 0x00000005)
#define EFI_CU_FP_EC_FORCE_UPDATE_MODE              (EFI_SUBCLASS_SPECIFIC | 0x00000006)
#define EFI_CU_FP_EC_FW_MISMATCH                    (EFI_SUBCLASS_SPECIFIC | 0x00000007)

//
// Computing Unit Memory Subclass Error Code definitions.
//
#define EFI_CU_MEMORY_EC_FAILED                     (EFI_SUBCLASS_SPECIFIC | 0x0000000B)
#define EFI_CU_MEMORY_EC_UNSUPPORTED                (EFI_SUBCLASS_SPECIFIC | 0x0000000C)
#define EFI_CU_MEMORY_EC_CLTT_FAILED                (EFI_SUBCLASS_SPECIFIC | 0x0000000D)

//
// Peripheral Class Error Code definitions.
//
#define EFI_P_EC_FW_CORRUPTED              0x0000000A
#define EFI_P_EC_RESOURCE_CONSUMED_BY_BMC  0x0000000B

//
// IO Bus Subclass definitions.
//
#define EFI_IO_BUS_PCI_EXPRESS                      (EFI_IO_BUS | 0x000D0000)

//
// IO Bus Class PCI Subclass Error Code definitions.
//
#define EFI_IOB_PCI_EC_OUT_OF_RESOURCES             (EFI_SUBCLASS_SPECIFIC | 0x00000002)

//
// IO Bus Class PCI EXPRESS Subclass Error Code definitions.
//
#define EFI_IOB_PCI_EXP_EC_PERR                     (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_IOB_PCI_EXP_EC_SERR                     (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define EFI_IOB_PCI_EXP_EC_TRAINING_FAILED          (EFI_SUBCLASS_SPECIFIC | 0x00000002)
#define EFI_IOB_PCI_EXP_EC_DEGRADED_LINK            (EFI_SUBCLASS_SPECIFIC | 0x00000003)
#define EFI_IOB_PCI_EXP_EC_IBIST_FAILED             (EFI_SUBCLASS_SPECIFIC | 0x00000004)

//
// Software Subclass definitions.
//
#define EFI_SOFTWARE_SYSTEM_ERROR                   (EFI_SOFTWARE | 0x00130000)
#define EFI_SYSTEM_FRONT_PANEL                      (EFI_SOFTWARE | 0x00140000)
#define EFI_SOFTWARE_EFI_BMC                        (EFI_SOFTWARE | 0x00150000)

//
// Software Class DXE BS Driver Subclass Progress Code definitions.
//
#define EFI_SW_DXE_BS_END_OF_POST                   (EFI_SUBCLASS_SPECIFIC | 0x00000007)

//
// Software Class Error Code definitions.
//
#define EFI_SW_EC_PWD_CLEAR_JMPR_SET       0x00000012

//
// Software Class PEI Module Subclass Error Code definitions.
//
#define EFI_SW_PEIM_WATCHDOG_TIMEOUT                (EFI_SUBCLASS_SPECIFIC | 0x00000002)
#define EFI_SW_PEIM_OS_WATCHDOG_TIMEOUT             (EFI_SUBCLASS_SPECIFIC | 0x00000003)

//
// Software Class System Subclass Error Code definitions.
//
#define EFI_SW_EC_CMOS_DATE_TIME_ERROR              (EFI_SUBCLASS_SPECIFIC | 0x00000000)


#endif // _EPG_STATUS_CODE_H_
