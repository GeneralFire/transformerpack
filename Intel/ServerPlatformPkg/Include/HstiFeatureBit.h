/** @file
  This file contains various definitions for IHV HSTI implementation
  including error string definitions

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains 'Framework Code' and is licensed as such under the terms
  of your license agreement with Intel or your vendor. This file may not be
  modified, except as allowed by additional terms of your license agreement.

@par Specification
**/

#ifndef __HSTI_FEATURE_BIT_H__
#define __HSTI_FEATURE_BIT_H__

#define HSTI_SECURITY_FEATURE_SIZE        3

typedef enum {
  P2sbEpMask5RemAccMaskBits29To26UnexpSts    = BIT0,
  P2sbEpMask7RemAccMaskBits31To30UnexpSts    = BIT1,
  P2sbLockMaskBitUnexpSts                    = BIT2
} SMM_HSTI_UNEXP_STATUS_DATA;

#define HSTI_STATUS                                                               L" Status "
#define HSTI_UNEXP_STATUS                                                         L" Unexpected Status "
#define HSTI_ERROR                                                                L" Error "
#define HSTI_PLATFORM_SECURITY_SPECIFICATION                                      L" Platform Security Specification"
#define HSTI_BOOTGUARD_CONFIGURATION                                              L" - Boot Guard Configuration -"
#define HSTI_SPI_FLASH_CONFIGURATION                                              L" - SPI Flash Configuration -"
#define HSTI_PCH_SECURITY_CONFIGURATION                                           L" - PCH Security Configuration -"
#define HSTI_BIOS_GUARD_SECURITY_CONFIGURATION                                    L" - Secure Signed Firmware Update Configuration -"
#define HSTI_MEASURED_BOOT_CONFIGURATION                                          L" - Measured Boot Configuration -"
#define HSTI_INTEGRATED_DEVICE_DMA_PROTECTION                                     L" - Integrated Device DMA Protection -"
#define HSTI_DEBUG_INTERFACE_SECURITY_CONFIGURATION                               L" - Debug Interface Security Configuration -"
#define HSTI_CPU_SECURITY_CONFIGURATION                                           L" - CPU Security Configuration -"
#define HSTI_SYSTEM_AGENT_SECURITY_CONFIGURATION                                  L" - Secure System Agent Security Configuration - "
#define HSTI_MEMORY_MAP_SECURITY_CONFIGURATION                                    L" - Memory Map Security Configuration -"
#define HSTI_PROCESSOR_GRAPHICS_SECURITY_CONFIGURATION                            L" - Processor Graphics Security Configuration -"
#define HSTI_PROCESSOR_SPD_SECURITY_CONFIGURATION                                 L" - SPD Security Configuration -"


#define HSTI_BYTE0_HARDWARE_ROOTED_BOOT_INTEGRITY                                 BIT0
#define      HSTI_BYTE0_HARDWARE_ROOTED_BOOT_INTEGRITY_UNEXP_STATUS_CODE_1               L"0x00000001"
#define      HSTI_BYTE0_HARDWARE_ROOTED_BOOT_INTEGRITY_UNEXP_STATUS_STRING_1             L"Boot Guard configured without Verified Boot\r\n"
#define      HSTI_BYTE0_HARDWARE_ROOTED_BOOT_INTEGRITY_UNEXP_STATUS_CODE_2               L"0x00000002"
#define      HSTI_BYTE0_HARDWARE_ROOTED_BOOT_INTEGRITY_UNEXP_STATUS_STRING_2             L"Boot Guard disabled\r\n"

#define HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION                                 BIT1
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_1               L"0x00010001"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_1             L" SPI Flash not write protected\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_2               L"0x00010002"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_2             L" SPI Flash descriptor overridden\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_3               L"0x00010003"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_3             L" SPI Controller configuration unlocked\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_4               L"0x00010004"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_4             L" SPI Controller BIOS Interface unlocked\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_5               L"0x00010005"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_5             L" Top Swap enabled\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_6               L"0x00010006"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_6             L" SPI Vendor Specific Component Capabilities unlocked\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_7               L"0x00010007"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_7             L" CPPT invalid on secondary flash part 0\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_8               L"0x00010008"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_8             L" ME FW not in Normal Working State\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_9               L"0x00010009"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_9             L" Flash Descriptor Invalid\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_A               L"0x0001000A"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_A             L" BIOS Region Flash Write Access\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_B               L"0x0001000B"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_B             L" BIOS Region Flash Read Access\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_C               L"0x0001000C"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_C             L" BIOS Master Read Access\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_D               L"0x0001000D"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_D             L" BIOS Master Write Access\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_E               L"0x0001000E"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_E             L" GbE Region Access\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_F               L"0x0001000F"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_F             L" CSME Region Access\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_10              L"0x00010010"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_10            L" BMC Region Access\r\n"

#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_11              L"0x00010011"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_11            L" SPI Region Access Rights Invalid\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_12              L"0x00010012"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_12            L" SPI SW Sequencing OPCODEs insufficiently restrained\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_13              L"0x00010013"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_13            L" Allowed SPI SW Sequencing OPCODE configuration incorrect\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_14              L"0x00010014"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_14            L" Global SMI not enabled\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_15              L"0x00010015"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_15            L" Global SMI not locked\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_16              L"0x00010016"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_16            L" TCO SMI not enabled and locked\r\n"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_17              L"0x00010017"
#define      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_17            L" RTC Bios Interface Lock\r\n"

#define HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE                                         BIT2
#define      HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE_UNEXP_STATUS_CODE_1                       L"0x00020001"
#define      HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE_UNEXP_STATUS_STRING_1                     L" BIOS Guard unsupported\r\n"
#define      HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE_UNEXP_STATUS_CODE_2                       L"0x00020002"
#define      HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE_UNEXP_STATUS_STRING_2                     L" BIOS Guard configuration unlocked\r\n"
#define      HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE_UNEXP_STATUS_CODE_3                       L"0x00020003"
#define      HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE_UNEXP_STATUS_STRING_3                     L" BIOS Guard disabled\r\n"
#define      HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE_UNEXP_STATUS_CODE_4                       L"0x00020004"
#define      HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE_UNEXP_STATUS_STRING_4                     L"BIOS Flash Protection disabled\r\n"

#define HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT                                      BIT3
#define      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_UNEXP_STATUS_CODE_1                    L"0x00030001"
#define      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_UNEXP_STATUS_STRING_1                  L" Chipset supports FW TPM, however no TPM enabled\r\n"

#define      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_UNEXP_STATUS_CODE_2                    L"0x00030002"
#define      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_UNEXP_STATUS_STRING_2                  L" PCR[7] is not initialized\r\n"

#define      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_UNEXP_STATUS_CODE_3                    L"0x00030003"
#define      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_UNEXP_STATUS_STRING_3                  L" Event Log is not published or invalid\r\n"

#define      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_UNEXP_STATUS_CODE_4                    L"0x00030004"
#define      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_UNEXP_STATUS_STRING_4                  L" Platform Auth accessible via 0x00000000\r\n"


#define HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION                               BIT4
#define      HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_UNEXP_STATUS_CODE_1             L"0x00040001"
#define      HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_UNEXP_STATUS_STRING_1           L" Bus Mastering Enabled for non-boot, integrated device\r\n"

#define      HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_UNEXP_STATUS_CODE_2             L"0x00040002"
#define      HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_UNEXP_STATUS_STRING_2           L" VTd locked\r\n"

#define      HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_UNEXP_STATUS_CODE_3             L"0x00040003"
#define      HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_UNEXP_STATUS_STRING_3           L" Incorrect VTd configuration\r\n"

#define HSTI_BYTE0_DEBUG_INTERFACE_SECURITY_CONFIGURATION                         BIT5
#define      HSTI_BYTE0_DEBUG_INTERFACE_SECURITY_UNEXP_STATUS_CODE_1                     L"0x00050001"
#define      HSTI_BYTE0_DEBUG_INTERFACE_SECURITY_UNEXP_STATUS_STRING_1                   L" Debug MSR enabled\r\n"

#define      HSTI_BYTE0_DEBUG_INTERFACE_SECURITY_UNEXP_STATUS_CODE_2                     L"0x00050002"
#define      HSTI_BYTE0_DEBUG_INTERFACE_SECURITY_UNEXP_STATUS_STRING_2                   L" Debug interface unlocked\r\n"

#define HSTI_BYTE0_SECURE_CPU_CONFIGURATION                                       BIT6
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_1                     L"0x00060001"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_1                   L" Minimum uCode patch revision not met\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_2                     L"0x00060002"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_2                   L" Pre-production silicon in use\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_3                     L"0x00060003"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_3                   L" IA32_FEATURE_CONTROL MSR not locked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_4                     L"0x00060004"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_4                   L" SMM Code Fetch feature configuration unlocked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_5                     L"0x00060005"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_5                   L" SMM Code Fetch outside SMRAM detection feature is disabled\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_6                     L"0x00060006"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_6                   L" AES-NI Feature configuration unlocked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_7                     L"0x00060007"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_7                   L" FIT table not present\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_8                     L"0x00060008"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_8                   L" FIT table Header Incorrect\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_9                     L"0x00060009"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_9                   L" FIT table sanity check failed\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_A                     L"0x0006000A"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_A                   L" MicrocodeRangeRegisters improperly configured or unlocked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_B                     L"0x0006000B"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_B                   L" SMM Configuration Unlocked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_C                     L"0x0006000C"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_C                   L" TSEGMB not locked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_D                     L"0x0006000D"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_D                   L" TSEG not naturally aligned\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_E                     L"0x0006000E"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_E                   L" Improper SMRR configuration\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_F                     L"0x0006000E"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_F                   L" SMRR not supported\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_10                    L"0x000600010"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_10                  L" SMRR not programmed correctly across all cores\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_11                    L"0x000600011"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_11                  L" SMRR1 and TSEGMB mismatch \r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_12                    L"0x000600012"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_12                  L" SMRR1 and TSEGMB size mismatch \r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_13                    L"0x000600013"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_13                  L" SMRR1 not working \r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_14                    L"0x000600014"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_14                  L" C-State Configuration unlocked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_15                    L"0x000600015"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_15                  L" Flex Ratio Configuration unlocked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_16                    L"0x000600016"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_16                  L" Sub-Numa Clustering Configuration unlocked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_17                    L"0x000600017"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_17                  L" Prochot# Configuration unlocked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_18                    L"0x000600018"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_18                  L" P-State Limit configuration unlocked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_19                    L"0x000600019"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_19                  L" Package RAPL Limit Configuration unlocked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_1A                    L"0x00060001A"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_1A                  L" PMAX configuration unlocked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_1B                    L"0x00060001B"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_1B                  L" Core Enable/Disable unlocked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_1C                    L"0x00060001C"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_1C                  L" SAPM configuration unlocked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_1D                    L"0x00060001D"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_1D                  L" Power Plane Limit configuration unlocked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_1E                    L"0x00060001E"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_1E                  L" Package Power Limit configuration unlocked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_1F                    L"0x00060001F"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_1F                  L" Thermal Monitoring Interrupts configuration unlocked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_20                    L"0x000600020"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_20                  L" TDP Control unlocked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_21                    L"0x000600021"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_21                  L" Memory Controller CH0 Scrambling Seed unlocked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_22                    L"0x000600022"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_22                  L" Memory Controller CH1 Scrambling Seed unlocked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_23                    L"0x000600023"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_23                  L" Memory Controller CH2 Scrambling Seed unlocked\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_24                    L"0x000600024"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_24                  L" PECI is Trusted is not set\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_25                    L"0x000600025"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_25                  L" Fit table does not contain valid ucode entry\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_26                    L"0x000600026"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_26                  L" BIOS Done Bit or CFG lock not set\r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_27                    L"0x000600027"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_27                  L" PMAX config unlocked \r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_28                    L"0x000600028"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_28                  L" Core Enable/Disable unlocked \r\n"

#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_CODE_29                    L"0x000600029"
#define      HSTI_BYTE0_SECURE_CPU_CONFIGURATION_UNEXP_STATUS_STRING_29                  L" P-State Limits unlocked \r\n"

#define HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION                              BIT7
#define      HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION_UNEXP_STATUS_CODE_1            L"0x00070001"
#define      HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION_UNEXP_STATUS_STRING_1          L" Intel TXT configuration unlocked\r\n"
#define      HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION_UNEXP_STATUS_CODE_2            L"0x00070002"
#define      HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION_UNEXP_STATUS_STRING_2          L" Memory Controller configuration unlocked\r\n"
#define      HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION_UNEXP_STATUS_CODE_3            L"0x00070003"
#define      HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION_UNEXP_STATUS_STRING_3          L" System Agent configuration unlocked\r\n"
#define      HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION_UNEXP_STATUS_CODE_4            L"0x00070004"
#define      HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION_UNEXP_STATUS_STRING_4          L" Graphics memory configuration unlocked\r\n"
#define      HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION_UNEXP_STATUS_CODE_5            L"0x00070005"
#define      HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION_UNEXP_STATUS_STRING_5          L" VT-d configuration unlocked\r\n"
#define      HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION_UNEXP_STATUS_CODE_6            L"0x00070006"
#define      HSTI_BYTE0_SECURE_SYSTEM_AGENT_CONFIGURATION_UNEXP_STATUS_STRING_6          L" Power Plane 0 Current Config unlocked\r\n"

#define HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION                                BIT0
#define      HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION_UNEXP_STATUS_CODE_1              L"0x00080001"
#define      HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION_UNEXP_STATUS_STRING_1            L" Memory BAR configuration unlocked\r\n"

#define      HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION_UNEXP_STATUS_CODE_2              L"0x00080002"
#define      HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION_UNEXP_STATUS_STRING_2            L" BIOS done unlocked\r\n"

#define      HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION_UNEXP_STATUS_CODE_3              L"0x00080003"
#define      HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION_UNEXP_STATUS_STRING_3            L" Fixed MMIO regions overlap\r\n"

#define      HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION_UNEXP_STATUS_CODE_4              L"0x00080004"
#define      HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION_UNEXP_STATUS_STRING_4            L" Non lockable MMIO ranges overlap other critical regions\r\n"

#define      HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION_UNEXP_STATUS_CODE_5              L"0x00080005"
#define      HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION_UNEXP_STATUS_STRING_5            L" SPD not write protected\r\n"

#define      HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION_UNEXP_STATUS_CODE_6              L"0x00080006"
#define      HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION_UNEXP_STATUS_STRING_6            L" LT_CONTROL lock or BIOS DONE bit not set \r\n"

#define HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION                       BIT1
#define      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_1     L"0x00090001"
#define      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_1   L" Graphics configuration unlocked\r\n"

#define      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_2     L"0x00090002"
#define      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_2   L"Invalid Graphics Memory Alignment\r\n"

#define      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_3     L"0x00090003"
#define      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_3   L" Reserved Check failed\r\n"

#define HSTI_BYTE1_SECURE_PCH_CONFIGURATION                                       BIT2
#define      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_1                     L"0x000A0001"
#define      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_1                   L" Thermal Configuration unlocked\r\n"
#define      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_2                     L"0x000A0002"
#define      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_2                   L" BAR's unlocked\r\n"
#define      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_3                     L"0x000A0003"
#define      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_3                   L" ACPI Base not lockedd\r\n"
#define      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_4                     L"0x000A0004"
#define      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_4                   L" Reserved Check failure\r\n"
#define      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_5                     L"0x000A0005"
#define      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_5                   L" P2SB SBI lock not set\r\n"
#define      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_6                     L"0x00010006"
#define      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_6                   L" PMC not lockedd a\r\n"
#define      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_7                     L"0x00010007"
#define      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_7                   L" Global SMI not enabled a\r\n"
#define      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_8                     L"0x00010008"
#define      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_8                   L" SMI not locked a\r\n"
#endif
