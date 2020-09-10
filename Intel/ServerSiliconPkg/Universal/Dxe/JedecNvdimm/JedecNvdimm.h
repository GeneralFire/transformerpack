/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2020 Intel Corporation. <BR>

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

#ifndef _JEDECNVDIMM_H_
#define _JEDECNVDIMM_H_

#pragma once

#include <PiDxe.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/MpService.h>
#include <Protocol/CpuCsrAccess.h>
#include <Protocol/SmmSwDispatch2.h>
#include <Protocol/NvdimmSmbusSmmProtocol.h>
#include <IndustryStandard/SmBios.h>
#include <Library/UefiLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/TimerLib.h>
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TimerLib.h>
#include <Library/PlatformHooksLib.h>
#include <Library/LocalApicLib.h>
#include <Library/CpuConfigLib.h>
#include <Guid/MemoryMapData.h>
#include <Guid/HobList.h>
#include <Cpu/CpuCoreRegs.h>
#include <Protocol/Smbios.h>
#include <Protocol/MemRasProtocol.h>
#include <Library/MmPciBaseLib.h>
#include <Library/AddressDecodeLib.h>

#include "RcRegs.h"
#include "PlatformCfgData.h"
#include "NvdimmAcpiSmmInterface.h"
#include <Library/NvdimmCommonLib.h>
#include <Library/SmmServicesTableLib.h>


extern EFI_GUID gEfiVolatileMemModeVariableGuid;

extern EFI_CPU_CSR_ACCESS_PROTOCOL     *mCpuCsrAccess;

#define VERBOSE_DEBUG 1

#define MAX_ROOT_DEVICE_FCNS      5

#define ARS_ALIGNMENT_SIZE             256

#define USE_SMALL_PAYLOAD              0
#define USE_LARGE_PAYLOAD              1
#define VOLATILE_SCRUB_SUPPORTED       BIT0
#define ARS_DSM_HEADER_SIZE            44
#define MAX_ERRORS_PER_DIMM            16
#define ARS_SCRUB_INTERVAL             0x2

#define EXT_STATUS_ARS_IN_PROGRESS     0x00010000
#define TOTAL_PERCENT_COMPLETE         100
#define PMEM_SCRUB_SUPPORTED           BIT1
#define INPUT_BUF_SIZE                 0x1000
#define OUTPUT_BUF_SIZE                0x1000

// ARS Start
#define ARS_START_SUCCESS              0
#define ARS_NOT_SUPPORTED              1
#define ARS_INVALID_PARAM              2
#define ARS_START_HW_ERROR             3
#define ARS_FUNC_SPEC_ERROR            4
#define ARS_START_FAILED               5
#define ARS_IN_PROGRESS                6

// Query ARS status
#define QUERY_ARS_STS_SUCCESS          0
#define QUERY_ARS_STS_NOT_SUPPORTED    1
#define QUERY_ARS_STS_FAILED           5
#define EXT_STS_ARS_COMPLETE           (0 << 16)
#define EXT_STS_ARS_IN_PROGRESS        (1 << 16)
#define EXT_STS_NO_ARS_PERFORMED       (2 << 16)
#define EXT_STS_ARS_STOPPED            (3 << 16)

// Clear Uncorrectable Error
#define CLR_UNCORR_ERR_STS_SUCCESS      0
#define CLR_UNCORR_ERR_NOT_SUPPORTED    1
#define CLR_UNCORR_ERR_INVALID_PARAM    2
#define CLR_UNCORR_ERR_FAILED           3

#define ADMIN_FUNCTIONS     0x02
#define BIOS_EMULATED       BIT0
#define NON_EXISTING_MEM_DEV 0x02
#define ONE_IMC_WAY         1
#define TWO_IMC_WAY         2
#define FOUR_IMC_WAY        4
#define EIGHT_IMC_WAY       8
#define SIXTEEN_IMC_WAY     16

#define MAX_PMEM_REGIONS      24          // Set = max DIMM slots on NC
#define MAX_NVDIMMS           8
#define READ_TYPE_DATA_SIZE   32

#define  SUCCESS  0
#define  FAILURE  1
#define  RETRY    2

#define OPEN_PAGE               0x0
#define SAVE_STATUS             0x64
#define RESTORE_STATUS          0x66
#define ARM_STATUS              0x6A

#define ERROR_THRESHOLD_STATUS    0xA5
#define WARNING_THRESHOLD_STATUS  0xA7
#define DRAM_ECC_ERROR_COUNT      0x80
#define DRAM_THRESHOLD_ECC_COUNT  0x81

#define JEDEC_ACPI_SMM_INTERFACE_UEFI_VARIABLE_GUID  { 0x1E958711, 0xB6C3, 0x49AD, {0xB4, 0x7B, 0x69, 0x37, 0x07, 0x59, 0x8B, 0x4D } }
#define JEDEC_ACPI_SMM_INTERFACE_UEFI_VARIABLE_NAME  L"JEDEC ACPI SMM VARIABLE"

// Define DSM Function Index Definition

#define DSM_GENERAL_STATUS_SUCCESS                    0x00
#define DSM_GENERAL_STATUS_NOT_SUPPORTED              0x01
#define DSM_GENERAL_STATUS_INVALID_PARAMETERS         0x02
#define DSM_GENERAL_STATUS_I2C_COMMUNICATION_ERROR    0x03
#define DSM_GENERAL_STATUS_FUNCTION_SPECIFIC_ERROR    0x04
#define DSM_GENERAL_STATUS_VENDOR_SPECIFIC_ERROR      0x05


#define CREATE_DSM_STATUS_FUNCTION_SPECIFIC(fsts)     (((fsts & 0xFF) << 16) | DSM_GENERAL_STATUS_FUNCTION_SPECIFIC_ERROR)

// FUNCTION 1
#define NVM_SPECIFICATION_REV                   0x00000006  // Page 0: Offset 06h
#define NVM_NUM_STANDARD_PAGES                  0x00000001  // Page 0: Offset 01h
#define NVM_FIRST_VENDOR_PAGE                   0x00000002  // Page 0: Offset 02h
#define NVM_NUM_VENDOR_PAGES                    0x00000003  // Page 0: Offset 03h
#define NVM_HARDWARE_REV                        0x00000004  // Page 0: Offset 04h
#define NVM_FW_REV_SLOT0                        0x00000007  // Page 0: Offset 07h (0x08/0x07)
#define NVM_FW_REV_SLOT1                        0x00000009  // Page 0: Offset 09h (0x0A/0x09)
#define NVM_CURRENT_FW_SLOT                     0x30000042  // Page 3: Offset 42h
#define NVM_CAPABILITIES                        0x00000010  // Page 0: Offset 10h
#define NVM_SUPPORTED_BACKUP_TRIGGER            0x00000016  // Page 0: Offset 16h
#define NVM_MAX_OP_RETRY_COUNT                  0x00000015  // Page 0: Offset 15h
#define NVM_SUPPORTED_NOTIFY_EVENTS             0x00000017  // Page 0: Offset 17h
#define NVM_SAVE_OP_TIMEOUT                     0x00000018  // Page 0: Offset 18h (0x19/0x18)
#define NVM_RESTORED_OP_TIMEOUT                 0x0000001C  // Page 0: Offset 1Ch (0x1D/0x1C)
#define NVM_ERASED_OP_TIMEOUT                   0x0000001E  // Page 0: Offset 1Eh (0x1F/0x1E)
#define NVM_ARMED_OP_TIMEOUT                    0x00000020  // Page 0: Offset 20h (0x21/0x20)
#define NVM_FW_OP_TIMEOUT                       0x00000022  // Page 0: Offset 22h (0x23/0x22)
#define NVM_ABORT_OP_TIMEOUT                    0x00000024  // Page 0: Offset 24h (0x24)
#define NVM_MIN_OP_TEMP                         0x00000038  // Page 0: Offset 38h (0x38)
#define NVM_MAX_OP_TEMP                         0x0000003A  // Page 0: Offset 3Ah (0x3A)
#define NVM_REGION_BLOCK_SIZE                   0x00000032  // Page 0: Offset 32h (0x32)

// FUNCTION 2
#define NVM_AVG_PWR_REQ                         0x00000029   // Page 0: Offset 29h (0x2A/0x29)
#define NVM_IDLE_PWR_REQ                        0x0000002B   // Page 0: Offset 2Bh (0x2C/0x2B)
#define NVM_MIN_VOLT_REQ                        0x0000002D   // Page 0: Offset 2Dh (0x2E/0x2D)
#define NVM_MAX_VOLT_REQ                        0x0000002F   // Page 0: Offset 2Fh (0x30/0x2F)

// FUNCTION 3

#define NVM_SET_ES_POLICY_STATUS                0x00000070   // Page 0: Offset 70h (RO - Return Policy Status)
                                                                // [0] : SET_ES_POLICY_SUCCESS
                                                                // [1] : SET_ES_POLICY_ERROR
                                                                // [2] : DEVICE_MANAGED_POLICY_ENABLED
                                                                // [3] : HOST_MANAGED_POLICY_ENABLED
#define NVM_SET_ES_POLICY_CMD                   0x00000049   // Page 0: Offset 49h (Bit0:Device Managed Policy, Bit[1]:Host Managed Policy)
#define NVM_ES_POLICY                           0x00000014   // Page 0: Offset 14h
#define NVM_ES_HARDWARE_REV                     0x10000004   // Page 1: Offset 04h (0x05/0x04)
#define NVM_ES_FW_REV                           0x10000006   // Page 1: Offset 06h (0x07/0x06)
#define NVM_ES_HEALTH_CHECK_FREQ                0x000000A9   // Page 0: Offset A9h
#define NVM_ES_CHARGE_TIMEOUT                   0x10000010   // Page 1: Offset 10h (0x11/0x10)
#define NVM_ES_MIN_OP_TEMP                      0x10000012   // Page 1: Offset 12h
#define NVM_ES_MAX_OP_TEMP                      0x10000013   // Page 1: Offset 13h
#define NVM_ES_ATTRIBUTES                       0x10000014   // Page 1: Offset 14h
#define NVM_ES_TECHNOLOGY                       0x10000015   // Page 1: Offset 15h
#define NVM_HOST_MANAGED_ES_ATTRIBUTES          0x20000082   // Page 2: Offset 82h

// FUNCTION 4
#define NVM_TRIGGER_INFO                        0x00000080   // Page 0: Offset 80h
#define NVM_SAVE_FAILURE_INFO                   0x00000084   // Page 0: Offset 84h (0x85/0x84)

// FUNCTION 5/6
#define NVM_LIFETIME_PERCENT_WARN_THRESHOLD     0x00000098   // Page 0: Offset 98h
#define NVM_LIFETIME_PERCENT_ERR_THRESHOLD      0x00000090   // Page 0: Offset 90h

// FUNCTION 7/8/9/10
#define NVM_ES_LIFETIME_PERCENT_WARN_THRESHOLD  0x00000099   // Page 0: Offset 99h
#define NVM_ES_LIFETIME_PERCENT_ERR_THRESHOLD   0x00000091   // Page 0: Offset 91h
#define NVM_ES_LIFETIME_TEMP_WARN_THRESHOLD     0x0000009A   // Page 0: Offset 9Ah
#define NVM_ES_LIFETIME_TEMP_ERR_THRESHOLD      0x00000092   // Page 0: Offset 92h

// FUNCTION 10
#define NVM_CRITICAL_HEALTH_INFO                0x000000A0   // Page 0: Offset A0h

// FUNCTION 11
#define NVM_MODULE_HEALTH                       0x000000A1   // Page 0: Offset (A2h/A1h)
#define NVM_MODULE_CURRENT_TEMP                 0x00000000   // Page 0: Offset N/A <TODO>
#define NVM_ERR_THRESHOLD_STATUS                0x000000A5   // Page 0: Offset A5h
#define NVM_WARN_THRESHOLD_STATUS               0x000000A7   // Page 0: Offset A7h
#define NVM_LIFETIME                            0x000000C0   // Page 0: Offset C0h
#define NVM_UNCORR_ECC_COUNTS                   0x20000080   // Page 2: Offset 80h
#define NVM_CORR_ECC_COUNTS                     0x20000081   // Page 2: Offset 81h

// FUNCTION 12
#define NVM_ES_LIFETIME_PERCENTAGE              0x10000070   // Page 1: Offset 70h
#define NVM_ES_CURRENT_TEMP                     0x10000071   // Page 1: Offset 72h/71h
#define NVM_ES_TOTAL_RUNTIME                    0x10000073   // Page 1: Offset 74h/73h

#define DSM_12_FUNCTION_SPECIFIC_PLATFORM_DOES_NOT_SUPPORT_ES_INFO       0x1

// FUNCTION 13
#define NVM_LAST_SAVE_OP_DURATION               0x20000004   // Page 2: Offset 05h/04h
#define NVM_LAST_RESTORE_OP_DURATION            0x20000006   // Page 2: Offset 07h/06h
#define NVM_LAST_ERASE_OP_DURATION              0x20000008   // Page 2: Offset 09h/08h
#define NVM_COMPLETED_SAVE_OP_NUMBER            0x2000000A   // Page 2: Offset 0Bh/0Ah
#define NVM_COMPLETED_RESTORE_OP_NUMBER         0x2000000C   // Page 2: Offset 0Dh/0Ch
#define NVM_COMPLETED_ERASE_OP_NUMBER           0x2000000E   // Page 2: Offset 0Fh/0Eh
#define NVM_MODULE_PWR_CYCLES                   0x20000010   // Page 2: Offset 11h/10h

// FUNCTION 14/15
#define NVM_VENDOR_PAGE_LOG_SIZE                0x00000031   // Page 0: Offset 31h
#define NVM_VENDOR_LOG_PAGE                     0x00000000   // Page 0: N/A <TODO>

// FUNCTION 16
#define NVM_ERR_INJ_ENABLE                      NVM_CAPABILITIES   // Page 0: Offset 0x10 bit[2] set indicates enabled

// FUNCTION 17/18
#define NVM_INJECT_OP_FAILURES                  0x20000060   // Page 2: Offset 60h
#define NVM_INJECT_BAD_BLOCK_CAP                0x20000067   // Page 2: Offset 67h
#define NVM_INJECT_ES_FAILURES                  0x20000064   // Page 2: Offset 64h
#define NVM_FW_UDPATE_FAILURES                  0x20000065   // Page 2: Offset 65h

// FUNCTION 19
#define NVM_ERASE_IMAGE                         0x00000043   // Page 0: Offset 43h (Bit[3]-Start Erase)

// FUNCTION 20
#define NVM_ARM                                 0x00000045   // Page 0: Offset 45h (Bit[0])

// FUNCTION 21
#define NVM_RESET_FACTORY_DEFAULT               0x00000043   // Page 0: Offset 43h (Bit[0]-Start Factory Default)

// FUNCTION 22/24
#define NVM_START_FW_UPDATE                     0x0000004A   // Page 0: Offset 4Ah (Bit[0] = 1 enable fw update mode)

// FUNCTION 23/29/30
// NOTE: SMART Module does not support I2C block write operation
#define NVM_REGION_ID                           0x30000005   // Page 3: Offset 06h/05h
#define NVM_BLOCK_ID                            0x30000007   // Page 3: Offset 07h
#define NVM_DATA_TYPE                           0x30000004   // Page 3: Offset 04h
#define NVM_DATA_RD_WR                          0x30000080   // Page 3: Offset 9Fh-80h

// FUNCTION 25/26
#define NVM_FW_SLOT                             0x30000042   // Page 3: Offset 42h (Bit[3:0]-Select FW SLOT)
#define NVM_FW_SLOT0_REV                        0x00000007   // Page 0: Offset 08h/07h
#define NVM_FW_SLOT1_REV                        0x00000009   // Page 0: Offset 0Ah/09h

// FUNCTION 27/28
#define NVM_PAGE                                0x00000000   // Page 0: Offset 00h (Read to return open page number)
#define NVM_PAGE_OFFSET                         0x00000000   // Page 0: Offset <TODO>

// FUNCTION 31
#define NVM_UCECC_COUNT_REG                     0x20000080   // Page 2: Offset 80h
#define NVM_CECC_COUNT_REG                      0x20000081   // Page 2: Offset 81h

#define NIG_FUNCTION_INDEX                      0xFFFFFFFB
#define NIC_FUNCTION_INDEX                      0xFFFFFFFC
#define NIH_FUNCTION_INDEX                      0xFFFFFFFD
#define NBS_FUNCTION_INDEX                      0xFFFFFFFE
#define NCH_FUNCTION_INDEX                      0xFFFFFFFF

#define NIH_INPUT_MODE_INJECT_ERRORS            1
#define NIH_INPUT_MODE_CLEAR_ERRORS             2

#define NIH_EXT_STS_SUBSET_OF_ERRORS            1 // only subset of requested errors are injected or cleared

#define CREATE_DSM_EXT_STATUS(sts, extSts) (((sts) & 0xFFFF) | (((extSts) & 0xFFFF)<<16))
// NVDIMM Device Method Return Status Code
#define NVDIMM_STS_SUCCESS                      0
#define NVDIMM_STS_NOT_IMPLEMENTED              1
#define NVDIMM_STS_INVALID_INPUT_PARAMETERS     2
#define NVDIMM_STS_HARDWARE_ERROR               3
#define NVDIMM_STS_RETRY_SUGGESTED              4
#define NVDIMM_STS_ERROR_UNKNOWN_REASON         5
#define NVDIMM_STS_METHOD_SPECIFIC_ERROR_CODE   6

#define SPEC_REVISION_2_1                       0x21

#define ESINFO_HOST_MANAGED_POLICY              1
#define ESINFO_DEVICE_MANAGED_POLICY            0

#define MAX_SADS                                64

#pragma pack(1)

//
// DSM Function Index Definition
//

// FUNC-1: Get NVDIMM-N Identification
typedef struct FUNC_INDEX_1 {
  UINT8   SpecificationRevision;
  UINT8   StandardPageNum;
  UINT8   FirstVendorPage;
  UINT8   VendorPageNum;
  UINT32  HardwareRevision;
  UINT16  FirmwareRevision;
  UINT8   CurrentFirmwareSlot;
  UINT8   FirmwareSlotCount;
  UINT8   Capabilities;
  UINT8   SupportedBackupTriggers;
  UINT8   MaxOpRetryCount;
  UINT8   SupportedNotificationEvents;
  UINT32  SaveOpTimeout;
  UINT32  RestoreOpTimeout;
  UINT32  EraseOpTimeout;
  UINT32  ArmOpTimeout;
  UINT32  FwOpTimeout;
  UINT32  AbortOpTimeout;
  UINT8   MinOpTemp;
  UINT8   MaxOpTemp;
  UINT32  RegionBlockSize;

} NVDIMM_IDENTIFICATION;

// FUNC-2: Get Save Operation Requirements
typedef struct FUNC_INDEX_2 {
  UINT16  AveragePwrReq;
  UINT16  IdlePwrReg;
  UINT16  MinVoltReq;
  UINT16  MaxVoltReg;
} SAVE_OPERATION_REQ;

struct DEVICE_ES_IDENTIFICATION {
  UINT16  ESHardwareRev;
  UINT16  ESFirmwareRev;
  UINT8   ESHealthChkFreq;
  UINT8   ESChargeTimeout0;
  UINT8   ESChargeTimeout1;
  UINT8   ESMinOpTemp;
  UINT8   ESMaxOpTemp;
  UINT8   ESAttribute;
  UINT8   ESTechnology;         // = 1 for supercapacitor
};

struct HOST_ES_IDENTIFICATION {
  UINT8 ESHealthChkFreq;
  UINT8 ESAttribute;
  UINT8 ESTechnology;           // = 2 for Battery
};

union ENERGY_SOURCE {
  struct DEVICE_ES_IDENTIFICATION DevESId;      // This field contains valid data if the ES policy is 0
  struct HOST_ES_IDENTIFICATION   HostESId;     // This field contains valid data if the ES policy is 1
};

// FUNC-3: Get Energy Source Identification
typedef struct FUNC_INDEX_3 {
  UINT8                             ESPolicy;
  struct DEVICE_ES_IDENTIFICATION   DevESId;    // This field contains valid data if the ES policy is 0
  struct HOST_ES_IDENTIFICATION     HostESId;   // This field contains valid data if the ES policy is 1
} ENERGY_SOURCE_IDENTIFICATION;

// FUNC-4: Get Last Backup Information
typedef struct FUNC_INDEX_4 {
  UINT32  TriggerInformation;   // BYTE0 - CSAVE_INFO0
  UINT32  SaveFailInformation;  // BYTE0 - CSAVE_FAIL_INFO0 BYTE1 - CSAVE_FAIL_INFO1 (Byte 2 and Byte 3 reserved)
} NVDIMM_BACKUP_INFO;

// FUNC-5: Get NVDIMM Thresholds
typedef struct FUNC_INDEX_5 {
  UINT8 NVDIMMLifetimePercentWarnTheshold;
  UINT8 NVDIMMLifetimePercentErrorTheshold;
} NVDIMM_THRESHOLD;

// FUNC-6: Set NVDIMM Lifetime Percentage Warning Threshold
typedef struct FUNC_INDEX_6 {
  UINT8 NVDIMMLifetimeWarnTheshold;
} NVDIMM_LIFETIME_WARN_THRESHOLD;

// FUNC-7: Get Energy Source Threshold
typedef struct FUNC_INDEX_7 {
  UINT8 ESLifetimePercentWarnTheshold;
  UINT8 ESLifetimePercentErrorThreshold;
  UINT8 ESTempWarnTheshold;
  UINT8 ESTempErrorTheshold;
} ENERGY_SOURCE_THRESHOLD;

// FUNC-8: Set Energy Source Lifetime Warning Threshold
typedef struct FUNC_INDEX_8 {
  UINT8 ESLifetimeWarnTheshold; // The new value in Celsius of warning threshold. Platform shall write this value to the "ES_TEMP_THRESHOLD"
} ENERGY_SOURCE_LIFETIME_WARN_THRESHOLD;

// FUNC-9: Set Energy Source Temperature Warning Threshold
typedef struct FUNC_INDEX_9 {
  UINT8 ESTempWarnTheshold; // The new value in Celsius of warning threshold. Platform shall write this value to the "ES_TEMP_THRESHOLD"
} ENERGY_SOURCE_TEMPERATURE_WARN_THRESHOLD;

// FUNC-10: Get Critical NVDIMM-N Health Info
typedef struct FUNC_INDEX_10 {
  UINT8 CriticalHealthInfo; // A high level status report of any issues with NVDIMM-N module
} CRITICAL_HEALTH_INFO;

// FUNC-11: Get NVDIMM-N Health Info
typedef struct FUNC_INDEX_11 {
  UINT16  ModuleHealthStatus; // The NVDIMM-N module's health. Byte0-MODULE HEALTH STATUS0 and Byte1-MODULE HEALTH STATUS1
  UINT16  ModuleTemp;         // The NVDIMM-N module's current temperature
  UINT8   ErrorThresStatus;   // The status regarding the error thresholds on the NVDIMM-N module
  UINT8   ErrorThresWarn;     // The status regarding the warning thresholds on the NVDIMM-N module
  UINT8   NVMLifetime;        // The last known memory lifetime percentage value
  UINT8   UECount;            // The number of uncorrectable ECC errors detected by the platform
  UINT8   CECount;            // The number of correctable ECC errors detected by the platform
  //UINT8   CEThresholdCounter; // DRAM_THRESHOLD_ECC_COUNT
} NVDIMM_HEALTH_INFO;


// FUNC-12: Get Energy Source Health Info
typedef struct FUNC_INDEX_12 {
  UINT8 ESLifeTime;     // The last known ES lifetime percentage
  UINT8 ESTemp0;        // The ES temperature in Celsius. Min value is 0
  UINT8 ESTemp1;
  UINT8 ESRuntime0;     // The time (in hours) the ES has been operational since it was manufactured
  UINT8 ESRuntime1;
  UINT8 Rsvd[2];
} ENERGY_STORAGE_HEALTH_INFO;

// FUNC-13: Get Operational Statistic
typedef struct FUNC_INDEX_13 {
  UINT8 LastSaveDuration0;
  UINT8 LastSaveDuration1;
  UINT8 Rsvd0[2];
  UINT8 LastRestoreDuration0;
  UINT8 LastRestoreDuration1;
  UINT8 Rsvd1[2];
  UINT8 LastEraseDuration0;
  UINT8 LastEraseDuration1;
  UINT8 Rsvd2[2];
  UINT8 NumSaveOpsCount0;
  UINT8 NumSaveOpsCount1;
  UINT8 Rsvd3[2];
  UINT8 NumRestoreOpsCount0;
  UINT8 NumRestoreOpsCount1;
  UINT8 Rsvd4[2];
  UINT8 NumEraseOpsCount0;
  UINT8 NumEraseOpsCount1;
  UINT8 Rsvd5[2];
  UINT8 NumModulePwrCycle0;
  UINT8 NumModulePwrCycle1;
  UINT8 Rsvd6[2];
} OPERATION_STATISTIC_INFO;

// FUNC-14: Get Vendor Log Page Size
typedef struct FUNC_INDEX_14 {
  UINT8 VendorLogPageSize;
  UINT8 Rsvd[3];
} VENDOR_LOG_PAGE_SIZE;

// FUNC-15: Get Vendor Log Page
typedef struct FUNC_INDEX_15 {
  UINT8 VendorLogPage;
  UINT8 Rsvd[3];
} VENDOR_LOG_PAGE;

// FUNC-16: Query Error Injection Status
typedef struct FUNC_INDEX_16 {
  UINT8 ErrorInjEnable;       // NVDIMM CTRL : 0x16 Error Control Reg - Bit[7] ERR_INJ_ENABLE (R/W)
} QUERY_ERROR_INJ_STATUS;

// FUNC-17/18: Inject Error/ Get Inject Error Failures
typedef struct FUNC_INDEX_17 {
  UINT8 InjectOpsFailures;
  UINT8 InjectBadBlocks;
  UINT8 InjectESFailures;
  UINT8 InjectFwFailures;
} INJECT_ERROR, INJECT_ERROR_FAILURE_STATUS;

// FUNC-19: Erase NVDIMM STATUS as returned
// 1:Timeout or DSM Method Output

// FUNC-20: Arm NVDIMM STATUS as returned
// 1:Timeout or DSM Method Output

// FUNC-21: Reset Factory Default STATUS as returned
// 1:Timeout or DSM Method Output

// FUNC-22: Reset Factory Default
// FUNC-25: Select Firmware Image Slot
typedef struct FUNC_INDEX_22 {
  UINT8 FwSlot;
} START_FW_UDPATE, SELECT_FW_IMAGE_SLOT;

// FUNC-23: Send Firmware Update Data
typedef struct FUNC_INDEX_23 {
  UINT32  RegionLength;
  UINT16  RegionId;
  UINT8   BlockId;
  UINT8   FwData[READ_TYPE_DATA_SIZE];
} SEND_FW_UPDATE_DATA;

// FUNC-24: Finish Firmware Update STATUS as returned
// 1: There is no firmware update operation in progress.
// 2: Invalid firmware image.

// FUNC-26: Get Firmware Info
typedef struct FUNC_INDEX_26 {
  UINT8 SlotFwVer0;
  UINT8 SlotFwVer1;
} FW_INFO;

// FUNC-27: I2C Read
// FUNC-28: I2C Write
typedef struct FUNC_INDEX_27 {
  UINT8 Page;
  UINT8 Offset;
  UINT8 Data;
} I2C_READ, I2C_WRITE;

// FUNC-29: Read Typed Data
// FUNC-30: Write Typed Data
typedef struct FUNC_INDEX_29 {
  UINT8 Type;
  UINT8 RegionId[2];
  UINT8 BlockId;
  UINT8 Data[READ_TYPE_DATA_SIZE];
} READ_TYPE_DATA, WRITE_TYPE_DATA;

// FUNC-31: Set Memory Error Counters
typedef struct FUNC_INDEX_31 {
  UINT8 UECounter;            // DRAM_ECC_ERROR_COUNT
  UINT8 CEThresholdCounter;   // DRAM_THRESHOLD_ECC_COUNT
} SET_MEMORY_ERROR_COUNTER;

typedef struct _ES_LIFETIME_INFO {
  UINT8     ESLifetimePercentWarnTheshold;
  UINT8     ESLifetimePercentErrorThreshold;
  UINT8     ESLifetimeTempWarnTheshold;
  UINT8     ESLifetimeTempErrorTheshold;
  UINT8     ESLifeTime;
  UINT8     ESTemp0;
  UINT8     ESTemp1;
  UINT8     ESRuntime0;
  UINT8     ESRuntime1;
} ES_LIFETIME_INFO;


typedef struct _MODULE_INFO {
  NVDIMM_HEALTH_INFO  Health;
  UINT8       CriticalHealthInfo;
  UINT8       LifetimePercentWarnTheshold;
  UINT8       LifetimePercentErrorTheshold;
  UINT8       TriggerInformation;
  UINT8       SaveFailInformation;
  UINT8       VendorLogPageSize;
  UINT8       VendorLogPage;
  UINT8       ErrorInjEnable;
} MODULE_INFO;

typedef struct {
  UINT8                         ImcId;              // iMC Num (either iMC0 or iMC1, i.e, o or 1)
  UINT8                         SocketId;           // Socket Num
  UINT8                         Dimm;               // Dimm #
  UINT8                         Ch;                 // Channel # where this Dimm resides
  UINT8                         CntrlRegionIndex;   // CONTROL INDEX
  UINT8                         EnergyType;         // EnergyType of NVDIMM.
  UINT8                         nvCtrlSmbAddr;      // NVDIMM SMBUS ADDRESS
  UINT8                         funcdesc;           // NVDIMM FUNCTION DESCRIPTOR
  UINT32                        nvDimmStatus;       // NVDIMMSTATUS
  NVDIMM_IDENTIFICATION         DimmId;
  SAVE_OPERATION_REQ            SaveOpReg;
  ENERGY_SOURCE_IDENTIFICATION  ESInfo;
  ES_LIFETIME_INFO              ESLifeInfo;
  MODULE_INFO                   ModuleInfo;
  OPERATION_STATISTIC_INFO      OpInfo;
  INJECT_ERROR_FAILURE_STATUS   InjStatus;
  UINT64                        SpaBase;            // Spa Base Addr
  UINT64                        Size;               // Size of DIMM
  UINT16                        DeviceID;           // Device ID of this dimm
  UINT16                        VendorID;           // Vendor ID of the dimm
  UINT32                        SerialNum;          // Serial Number of the Dimm
} JEDEC_NVDIMM;

typedef struct {
  UINT64      BaseAddress;          // PMEM region base address
  UINT64      Size;                 // PMEM region size
  UINT8       SadIndex;             // SAD index for this PMEM region
  UINT8       SocketNum;            // Socket number for this PMEM region
  UINT8       NumNvDimms;           // Number of NVDIMMs for this PMEM region
  JEDEC_NVDIMM      NvDimms[MAX_NVDIMMS]; // List of NVDIMMs behind this PMEM region
} PMEM_REGION;

typedef struct _ERROR_RECORD {
  UINT32      NfitHandle;          // Nfit Handle that defines the dimm that is part of the error recorc
  UINT32      Flags;               // If Bit0 is set, indicates an overflow condition for the error range indicated below
  UINT64      SpaOfErrLoc;         // Start SPA of the error location.
  UINT64      Length;              // Length indicates the consecutive bytes from Start SPA of Error Location that are in error.
} ERROR_RECORD, *PERROR_RECORD;

typedef struct _ARS_HEADER {
  UINT32      OutputLength;        // Output size of the ARS Output Header
  UINT64      StartSpa;            // Start System Physical Address
  UINT64      Length;              // ARS Length
  UINT64      RestartArsStartSpa;  // Restart ARS Start System Physical Address
  UINT64      RestartArsLength;    // Restart ARS Length
  UINT16      Type;                // if Bit0 is set, it is Volatile & if Bit1 is set it is Non-Volatile, both if both the bits are set
  UINT16      Flags;               // Bit[0] - If set to 1, indicates an overflow condition has occurred.   This means that more errors
                                   // were reported in the error log than will fit in the maximum total buffer size of Max Query ARS Status
                                   // Data Size from the Query ARS Capabilities.  The returned Extended Status should be ARS Stopped
                                   // Prematurely when this bit is set to 1
  UINT32      NumErrorRecords;     // Number of error records defined in this output
  ERROR_RECORD ErrRecords[MAX_SYS_DIMM * MAX_ERRORS_PER_DIMM];
} ARS_DSM_HEADER, *PARS_DSM_HEADER;

#pragma pack()


// DSM: _NCH, _NBS, _NIC, _NIH, _NIG
#pragma pack(1)

typedef struct {
  UINT32  MaintenanceNeeded             : 1,
          PerformanceDegraded           : 1,
          Reserved                      : 6,
          WritePersistencyPowerLoss     : 1,
          WritePersistencyLossOffline   : 1,
          WritePersistencyLossImminent  : 1,
          Reserved1                     : 5,
          AllDataLossPowerLoss          : 1,
          AllDataLossOffline            : 1,
          AllDataLossImminent           : 1,
          Reserved2                     : 13;
  UINT32  PermanentHealthCondition      : 1,
          Reserved3                     : 31;
} DSM_HEALTH_STATUS_COMMON;
typedef struct {
  // Validation Flags
  UINT16  OverallHealthStatusFlagValid  : 1,
          OverallHealthStatusAttrValid  : 1,
          Reserved                      : 14;
  DSM_HEALTH_STATUS_COMMON HealthStatus;
  UINT8   Reserved1[50];
} DSM_FNOUT_NCH;

typedef struct {
  // Validation Flags
  UINT16  DataLossCountValid            : 1,
          Reserved                      : 15;
  UINT32  DataLossCount;
  UINT8   Reserved1[54];
} DSM_FNOUT_NBS;

typedef struct {
  DSM_HEALTH_STATUS_COMMON HealthStatus;
  UINT8   Reserved[52];
} DSM_FNOUT_NIC;

typedef struct {
  UINT8   Mode;
  UINT8   Reserved[3];
  DSM_HEALTH_STATUS_COMMON HealthStatus;
  UINT8   Reserved1[52];
} DSM_FNINP_NIH;

typedef struct {
  // Validation Flags
  UINT16  OverallHealthStatusFlagValid  : 1,
          OverallHealthStatusAttrValid  : 1,
          Reserved                      : 14;
  DSM_HEALTH_STATUS_COMMON HealthStatus;
  UINT8   Reserved1[50];
} DSM_FNOUT_NIG;
#pragma pack()



typedef struct {
  UINT64                NextBaseAddress;                // Next ARS base address to scrub
  UINT64                SizeLeftToScrub;                // ARS region remaining
  UINT8                 CurrentNodeScrub[MC_MAX_NODE];  // Current node that has a running scrub
  UINT8                 CurrentPmemIndex;
  TRANSLATED_ADDRESS    Ta;
  UINT8                 NumDimmsLeft;                   // Number of NVDIMMs left to scrub
  INT32                 PmemIndex[MAX_SYS_DIMM * MAX_PMEM_RGN_PER_DIMM];  // Order of NVDIMMs found during ComputeDPAsForARS
} ARS_SCRUB_DATA;

typedef struct _SMBIOS_HANDLE_INFO_
{
  UINT8        Socket;
  UINT8        Channel;
  UINT8        Dimm;
  UINT16       SMBiosType17Handle;
} SMBIOS_HANDLE_INFO, *PSMBIOS_HANDLE_INFO;

/******************************************************************************
 * Module global variables.
 ******************************************************************************/
extern BOOLEAN                      mInSmm;
extern EFI_SMM_SYSTEM_TABLE2        *mSmst;
extern struct SystemMemoryMapHob    *mSystemMemoryMap;
extern NVDIMM_SMBUS_SMM_INTERFACE   *mNvdimmSmbusSmmInterface;
extern NVDIMM_ACPI_SMM_INTERFACE    *mNvdimmAcpiSmmInterface;
extern PMEM_REGION                  *mPmemRegions;

// JedecNvdimm.c
EFI_STATUS EFIAPI UpdateJedecNvdimmAcpiTable(UINT64 *JedecNvdimmTablePtr);
BOOLEAN IsNvDimm(UINT8 socket, UINT8 ch, UINT8 dimm);
EFI_STATUS CheckIfMemRangePersistent(UINT32 *Result, UINT64 StartAddr, UINT64 *Size);
EFI_STATUS XlateDpaToSpa(NVDIMM *pDimm, UINT64 StartDpa, UINT64 Dpa, UINT64 *pSpa);
EFI_STATUS FillPmemRegions(VOID);
UINT8 GetChInterBitmap(struct SADTable *pSAD);
UINT32 GetChannelGranularity(struct SADTable *pSAD);
UINT8 GetImcWays(UINT8 Socket, UINT8  SadInx);
UINT8 GetChWays(UINT8 socket, UINT8 SadIndex);
EFI_STATUS GetDimmInfoFromSpa(UINT64 Spa, UINT64 *SADSpaStart, UINT8 *Soc, UINT8 *Channel, UINT8 *Dimm);
EFI_STATUS JedecSpaToDpa(UINT64 Spa, UINT8 *Skt, UINT8 *Ch, UINT8 *Dimm, UINT64 *pDpa);
UINT8 GetChInterleave (IN UINT8 Socket, IN UINT8 SadInx);

// NvdimmAccess.c
EFI_STATUS
ReadSmb (
  JEDEC_NVDIMM                       *Nvdimm,
  UINT8                        byteOffset,
  UINT8                        *data
  );

EFI_STATUS
WriteSmb (
  JEDEC_NVDIMM                       *Nvdimm,
  UINT8                        byteOffset,
  UINT8                        *data
  );

EFI_STATUS
JedecSwitchPage (
  JEDEC_NVDIMM                       *Nvdimm,
  UINT8                        Page
  );

VOID
GetDimmId (
  JEDEC_NVDIMM *Nvdimm
  );

VOID
GetDimmPwrInfo (
  JEDEC_NVDIMM *Nvdimm
  );

VOID
GetDimmESInfo (
  JEDEC_NVDIMM *Nvdimm
  );

VOID
GetDimmOpStatistics (
  JEDEC_NVDIMM *Nvdimm
  );

VOID
GetDimmIModuleInfo (
  JEDEC_NVDIMM *Nvdimm
  );

EFI_STATUS
GetNextValidSmbiosHandle (
  EFI_SMBIOS_PROTOCOL *SmbiosProtocol,
  EFI_SMBIOS_HANDLE *SmbiosHandle,
  EFI_SMBIOS_TABLE_HEADER  **SmbiosRecord
  );

#endif // _JEDECNVDIMM_H_
