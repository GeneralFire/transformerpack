/** @file
  Platform specific information

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#include <BackCompatible.h>
#include <Uefi.h>
#include <Library/PcdLib.h>

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

// APTIOV_SERVER_OVERRIDE_RC_START
#include <Token.h>
// APTIOV_SERVER_OVERRIDE_RC_END
//
// Onboard / Integrated Graphics Device
//

#define IGD_VID                         0x102B  // Matrox
#define IGD_DID                         0x0522  // iBMC Video

#define VIDEO_DEV_NUM                   0x00
#define VIDEO_FUNC_NUM                  0x00
#define VIDEO_ROOTPORT_DEV_NUM          PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS
#define VIDEO_ROOTPORT_FUNC_NUM         PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_5

#define ASPEED2500_VID                  0x1A03
#define ASPEED2500_DID                  0x2000

//
// Assigning default ID and base addresses here, these definitions are used by ACPI tables
//

#define PCH_IOAPIC                      (UINT64)BIT0
#define PCH_IOAPIC_ID                   0x08
#define PCH_IOAPIC_ADDRESS              0xFEC00000 // This must get range from Legacy IIO
#define PCH_INTERRUPT_BASE              0

#define PC00_IOAPIC                     (UINT64)BIT1  //Because PCH_IOAPIC gets the first bit, these bit values will be 1+PC number.
#define PC00_IOAPIC_ID                  0x09
#define PC00_INTERRUPT_BASE             24

#define PC01_IOAPIC                     (UINT64)BIT2
#define PC01_IOAPIC_ID                  0x0A
#define PC01_INTERRUPT_BASE             32

#define PC02_IOAPIC                     (UINT64)BIT3
#define PC02_IOAPIC_ID                  0x0B
#define PC02_INTERRUPT_BASE             40

#define PC03_IOAPIC                     (UINT64)BIT4
#define PC03_IOAPIC_ID                  0x0C
#define PC03_INTERRUPT_BASE             48

#define PC04_IOAPIC                     (UINT64)BIT5
#define PC04_IOAPIC_ID                  0x0D
#define PC04_INTERRUPT_BASE             56

#define PC05_IOAPIC                     (UINT64)BIT6
#define PC05_IOAPIC_ID                  0x0E
#define PC05_INTERRUPT_BASE             64

#define PC06_IOAPIC                     (UINT64)BIT7
#define PC06_IOAPIC_ID                  0x0F
#define PC06_INTERRUPT_BASE             72

#define PC07_IOAPIC                     (UINT64)BIT8
#define PC07_IOAPIC_ID                  0x10
#define PC07_INTERRUPT_BASE             80

#define PC08_IOAPIC                     (UINT64)BIT9
#define PC08_IOAPIC_ID                  0x11
#define PC08_INTERRUPT_BASE             88

#define PC09_IOAPIC                     (UINT64)BIT10
#define PC09_IOAPIC_ID                  0x12
#define PC09_INTERRUPT_BASE             96

#define PC10_IOAPIC                     (UINT64)BIT11
#define PC10_IOAPIC_ID                  0x13
#define PC10_INTERRUPT_BASE             104

#define PC11_IOAPIC                     (UINT64)BIT12
#define PC11_IOAPIC_ID                  0x14
#define PC11_INTERRUPT_BASE             112

#define PC12_IOAPIC                     (UINT64)BIT13
#define PC12_IOAPIC_ID                  0x15
#define PC12_INTERRUPT_BASE             120

#define PC13_IOAPIC                     (UINT64)BIT14
#define PC13_IOAPIC_ID                  0x16
#define PC13_INTERRUPT_BASE             128

#define PC14_IOAPIC                     (UINT64)BIT15
#define PC14_IOAPIC_ID                  0x17
#define PC14_INTERRUPT_BASE             136

#define PC15_IOAPIC                     (UINT64)BIT16
#define PC15_IOAPIC_ID                  0x18
#define PC15_INTERRUPT_BASE             144

#define PC16_IOAPIC                     (UINT64)BIT17
#define PC16_IOAPIC_ID                  0x19
#define PC16_INTERRUPT_BASE             152

#define PC17_IOAPIC                     (UINT64)BIT18
#define PC17_IOAPIC_ID                  0x1A
#define PC17_INTERRUPT_BASE             160

#define PC18_IOAPIC                     (UINT64)BIT19
#define PC18_IOAPIC_ID                  0x1B
#define PC18_INTERRUPT_BASE             168

#define PC19_IOAPIC                     (UINT64)BIT20
#define PC19_IOAPIC_ID                  0x1C
#define PC19_INTERRUPT_BASE             176

#define PC20_IOAPIC                     (UINT64)BIT21
#define PC20_IOAPIC_ID                  0x1D
#define PC20_INTERRUPT_BASE             184

#define PC21_IOAPIC                     (UINT64)BIT22
#define PC21_IOAPIC_ID                  0x1E
#define PC21_INTERRUPT_BASE             192

#define PC22_IOAPIC                     (UINT64)BIT23
#define PC22_IOAPIC_ID                  0x1F
#define PC22_INTERRUPT_BASE             200

#define PC23_IOAPIC                     (UINT64)BIT24
#define PC23_IOAPIC_ID                  0x20
#define PC23_INTERRUPT_BASE             208

#define PC24_IOAPIC                     (UINT64)BIT25
#define PC24_IOAPIC_ID                  0x21
#define PC24_INTERRUPT_BASE             216

#define PC25_IOAPIC                     (UINT64)BIT26
#define PC25_IOAPIC_ID                  0x22
#define PC25_INTERRUPT_BASE             224

#define PC26_IOAPIC                     (UINT64)BIT27
#define PC26_IOAPIC_ID                  0x23
#define PC26_INTERRUPT_BASE             232

#define PC27_IOAPIC                     (UINT64)BIT28
#define PC27_IOAPIC_ID                  0x24
#define PC27_INTERRUPT_BASE             240

#define PC28_IOAPIC                     (UINT64)BIT29
#define PC28_IOAPIC_ID                  0x25
#define PC28_INTERRUPT_BASE             248

#define PC29_IOAPIC                     (UINT64)BIT30
#define PC29_IOAPIC_ID                  0x26
#define PC29_INTERRUPT_BASE             256

#define PC30_IOAPIC                     (UINT64)BIT31
#define PC30_IOAPIC_ID                  0x27
#define PC30_INTERRUPT_BASE             264

#define PC31_IOAPIC                     (UINT64)BIT32
#define PC31_IOAPIC_ID                  0x28
#define PC31_INTERRUPT_BASE             272

#define PC32_IOAPIC                     (UINT64)BIT33
#define PC32_IOAPIC_ID                  0x29
#define PC32_INTERRUPT_BASE             280

#define PC33_IOAPIC                     (UINT64)BIT34
#define PC33_IOAPIC_ID                  0x2A
#define PC33_INTERRUPT_BASE             288

#define PC34_IOAPIC                     (UINT64)BIT35
#define PC34_IOAPIC_ID                  0x2B
#define PC34_INTERRUPT_BASE             296

#define PC35_IOAPIC                     (UINT64)BIT36
#define PC35_IOAPIC_ID                  0x2C
#define PC35_INTERRUPT_BASE             304

#define PC36_IOAPIC                     (UINT64)BIT37
#define PC36_IOAPIC_ID                  0x2D
#define PC36_INTERRUPT_BASE             312

#define PC37_IOAPIC                     (UINT64)BIT38
#define PC37_IOAPIC_ID                  0x2E
#define PC37_INTERRUPT_BASE             320

#define PC38_IOAPIC                     (UINT64)BIT39
#define PC38_IOAPIC_ID                  0x2F
#define PC38_INTERRUPT_BASE             328

#define PC39_IOAPIC                     (UINT64)BIT40
#define PC39_IOAPIC_ID                  0x30
#define PC39_INTERRUPT_BASE             336

#define PC40_IOAPIC                     (UINT64)BIT41
#define PC40_IOAPIC_ID                  0x31
#define PC40_INTERRUPT_BASE             344

#define PC41_IOAPIC                     (UINT64)BIT42
#define PC41_IOAPIC_ID                  0x32
#define PC41_INTERRUPT_BASE             352

#define PC42_IOAPIC                     (UINT64)BIT43
#define PC42_IOAPIC_ID                  0x33
#define PC42_INTERRUPT_BASE             360

#define PC43_IOAPIC                     (UINT64)BIT44
#define PC43_IOAPIC_ID                  0x34
#define PC43_INTERRUPT_BASE             368

#define PC44_IOAPIC                     (UINT64)BIT45
#define PC44_IOAPIC_ID                  0x35
#define PC44_INTERRUPT_BASE             376

#define PC45_IOAPIC                     (UINT64)BIT46
#define PC45_IOAPIC_ID                  0x36
#define PC45_INTERRUPT_BASE             384

#define PC46_IOAPIC                     (UINT64)BIT47
#define PC46_IOAPIC_ID                  0x37
#define PC46_INTERRUPT_BASE             392

#define PC47_IOAPIC                     (UINT64)BIT48
#define PC47_IOAPIC_ID                  0x38
#define PC47_INTERRUPT_BASE             400

//
// Define platform base
// Note: All the PCH devices must get Legacy IO resources within first 16KB
// since KTI RC allcoates range 0-16KB for the legacy IIO.
//
#define BMC_KCS_BASE_MASK               0xFFF0
#define PCH_ACPI_BASE_ADDRESS           PcdGet16 (PcdAcpiBaseAddress)      // ACPI Power Management I/O Register Base Address

#define PCH_TCO_BASE_ADDRESS            PcdGet16 (PcdTcoBaseAddress)
#define PCH_PWRM_BASE_ADDRESS           0xFE000000
#define PCH_PWRM_MMIO_SIZE              0x00010000

// APTIOV_SERVER_OVERRIDE_RC_START
// Use SDL token for PM base address.
#ifndef PM_BASE_ADDRESS
#define PM_BASE_ADDRESS                 PCH_ACPI_BASE_ADDRESS
#endif
// APTIOV_SERVER_OVERRIDE_RC_END
#define PCH_ACPI_TIMER_ADDRESS          (PCH_ACPI_BASE_ADDRESS + 0x08)
#define PCH_ACPI_TIMER_MAX_VALUE        0x1000000   // The timer is 24 bit overflow

#define PCH_TBARB_ADDRESS               0xFED08000  // 4K for Pch Thermal Sensor
#define HPET_BASE_ADDRESS                0xFED00000
// APTIOV_SERVER_OVERRIDE_RC_START
#ifndef SIO_GPIO_BASE_ADDRESS
#define SIO_GPIO_BASE_ADDRESS           0x0800
#endif
// APTIOV_SERVER_OVERRIDE_RC_END

//
// Local and I/O APIC addresses.
//
#define EFI_APIC_LOCAL_ADDRESS          0xFEE00000
#define LOCAL_APIC_ADDRESS              0xFEE00000

//
// Constant definition
//
#define  MAX_SMRAM_RANGES    4
//
// This structure stores the base and size of the ACPI reserved memory used when
// resuming from S3.  This region must be allocated by the platform code.
//
typedef struct {
  UINT32  AcpiReservedMemoryBase;
  UINT32  AcpiReservedMemorySize;
  UINT32  SystemMemoryLength;
} RESERVED_ACPI_S3_RANGE;

#define RESERVED_ACPI_S3_RANGE_OFFSET (EFI_PAGE_SIZE - sizeof (RESERVED_ACPI_S3_RANGE))

//
// SMBUS Data
//
#define PCH_SMBUS_BUS_DEV_FUNC          0x1F0300
 // APTIOV_SERVER_OVERRIDE_RC_START : Program GPIO, PMBASE and SMBUS base address from SDL values                            
//#define PCH_SMBUS_BASE_ADDRESS          0x0780
#define PCH_SMBUS_BASE_ADDRESS          SMBS
// APTIOV_SERVER_OVERRIDE_RC_END : Program GPIO, PMBASE and SMBUS base address from SDL values                            
#define PLATFORM_NUM_SMBUS_RSVD_ADDRESSES   4
#define SMBUS_ADDR_CH_A_1                   0xA0
#define SMBUS_ADDR_CH_A_2                   0xA2
#define SMBUS_ADDR_CH_B_1                   0xA4
#define SMBUS_ADDR_CH_B_2                   0xA6
#define SMBUS_IO_EXPANDER_ADDRESS           0x38

//
// CMOS usage
//
#define CMOS_CPU_BSP_SELECT             0x10
#define CMOS_CPU_UP_MODE                0x11
#define CMOS_CPU_RATIO_OFFSET           0x12

#define CMOS_CPU_CORE_HT_OFFSET         0x13
#define CMOS_CPU1_CORE_HT_OFFSET        0x14
#define CMOS_CPU2_CORE_HT_OFFSET        0x15
#define CMOS_CPU3_CORE_HT_OFFSET        0x16

#define CMOS_CPU0_CORE_DISABLE          0x1B            //Processor0Core Offset
#define CMOS_CPU1_CORE_DISABLE           0x1C            //Processor1Core Offset
#define CMOS_CPU2_CORE_DISABLE           0x1D            //Processor2Core Offset
#define CMOS_CPU3_CORE_DISABLE           0x1E            //Processor3Core Offset
#define CMOS_CPUx_CORE8_9_DISABLE        0x1F            //Processor[i]Core[9:8] Offset, Proc[i].Core[9:8] at this CMOS bits[x+1:x], where x = i * 2

// Second bank
//
#define CMOS_CPU_HT_OFFSET              0x14             //Processor Hyperthreading Offset
#define CMOS_CPU_BIST_OFFSET            0x15
#define CMOS_CPU_VMX_OFFSET             0x16
#define CMOS_EFI_DEBUG                  0x17
#define CMOS_PLATFORM_ID_LO             0x18            // Second bank CMOS location of Platform ID
#define CMOS_PLATFORM_ID_HI             0x19            //
#define CMOS_PCH_PORT80_OFFSET          0x1A
#define CMOS_WHEA_OS_ENABLE             0x1B            // Must match WIWH definition in Whea.asi
#define TCG_CMOS_AREA_OFFSET            0x60                          // Also Change in Universal\Security\Tpm\PhysicalPresence\Dxe\PhysicalPresence.c &

#define TCG_CMOS_MOR_AREA_OFFSET        (TCG_CMOS_AREA_OFFSET + 0x05)

#define CMOS_DEBUG_PRINT_LEVEL_REG      0x4C             // Debug Mask saved in CMOS
#define CMOS_DEBUG_PRINT_LEVEL_1_REG    0x4D
#define CMOS_DEBUG_PRINT_LEVEL_2_REG    0x4E
#define CMOS_DEBUG_PRINT_LEVEL_3_REG    0x4F

#define PPR_MRC_STATUS_OFFSET            0x46            // 1 byte for storing PPR flow status
#define CMOS_MRC_STATUS_OFFSET           0x47            // 1 byte for storing the MRC Status

#define CMOS_SVBIOS_LT_CMOS_VALID_FLAG  0xA0    // 4 bytes CMOS space for LT Valid flag
#define CMOS_SVBIOS_LT_MEMORY_LOCKED    0xA4    // 4 bytes CMOS space for LT Memory Locked
#define CMOS_SVBIOS_LT_CRASH0           0xA8    // 4 bytes CMOS space for LT Crash 0 value
#define CMOS_SVBIOS_LT_CRASH1           0xAC    // 4 bytes CMOS space for LT Crash 1 Value
#define CMOS_SVBIOS_LT_ESTS             0xB0    // 4 bytes CMOS space for LT E STS
#define CMOS_SVBIOS_LT_STS              0xB4    // 4 bytes CMOS space for LT STS
#define CMOS_SVBIOS_KNOB_RESET_REG      0xB8    // 1 byte CMOS space for SvBios Knob Reset type register
#define CMOS_SVBIOS_RESET_TEST_REG      0xB9    // 1 byte CMOS Space for enabling SvBios Reset test
#define CMOS_SVBIOS_HIF_RECOVERY_REG    0xBA    // 1 byte CMOS Space for Hif recovery reset counter
#define CMOS_SVBIOS_MAILBOX_REG         0xBB    // 2 bytes CMOS Space for passing the Mailbox address to handler function

#define CMOS_WARM_RESET_COUNTER_OFFSET       0xBD    // 1 byte CMOS Space for passing warm reset counter to Dxe
                                                     //   due to reset in MRC Dxe always thinks that warm reset occurs
                                                     //   counter > 1 -> means WarmReset
#define CMOS_DRAM_SHARED_MAILBOX_ADDR_REG    0xF0    // 2 bytes CMOS Space for the DRAM Share Mailbox address [31:16]

#define CMOS_DIAG_STAT_REG  0x0E  // bit0 used to store Near Memory Flush status

//
// GPIO Index Data Structure
//
typedef struct {
  UINT32  Value;
  UINT8   Register;
} PCH_GPIO_INDEX_DATA;

//
// Define platform type register
// feature space location for GPI register in FWH
//
#define FWH_GPI_BASE_ADDRESS  0xFFBC0100
#define PLATFORM_TYPE_REG     (*(UINT8 volatile *) FWH_GPI_BASE_ADDRESS)

//
// COM (Serial) Port Base address
//
#define  COM1_BASE                      0x3F8
#define  COM2_BASE                      0x2F8

//
// LPT (Parallel) Port Base address
//
#define  LPT1_BASE                      0x378


#define MEM64_LEN                       0x00100000000
#define RES_MEM64_39_BASE               0x08000000000 - MEM64_LEN   // 2^39
#define RES_MEM64_39_LIMIT              0x08000000000 - 1           // 2^39
#define RES_MEM64_40_BASE               0x10000000000 - MEM64_LEN   // 2^40
#define RES_MEM64_40_LIMIT              0x10000000000 - 1           // 2^40

//
// Misc. platform definitions
//
#define CPU_GENERIC_UNCORE_DEV_FUNC     0
#define PCI_REVISION_ID_REG             8

//
// Need min. of 24 MB PEI phase
//
#define PEI_MIN_MEMORY_SIZE             (EFI_PHYSICAL_ADDRESS) ((320 * 0x100000))

#define HPET_BLOCK_ADDRESS                            0x0FED00000

#define PCI_BUS_NUMBER_PCH_HPET                       0x0
#define PCI_DEVICE_NUMBER_PCH_HPET                    0x1F

#define PCI_FUNCTION_NUMBER_PCH_HPET0                 0x00
#define PCI_FUNCTION_NUMBER_PCH_HPET1                 0x01
#define PCI_FUNCTION_NUMBER_PCH_HPET2                 0x02
#define PCI_FUNCTION_NUMBER_PCH_HPET3                 0x03
#define PCI_FUNCTION_NUMBER_PCH_HPET4                 0x04
#define PCI_FUNCTION_NUMBER_PCH_HPET5                 0x05
#define PCI_FUNCTION_NUMBER_PCH_HPET6                 0x06
#define PCI_FUNCTION_NUMBER_PCH_HPET7                 0x07
#define R_PCH_LPC_H0BDF                               0x70
#define R_PCH_LPC_H1BDF                               0x72
#define R_PCH_LPC_H2BDF                               0x74
#define R_PCH_LPC_H3BDF                               0x76
#define R_PCH_LPC_H4BDF                               0x78
#define R_PCH_LPC_H5BDF                               0x7A
#define R_PCH_LPC_H6BDF                               0x7C
#define R_PCH_LPC_H7BDF                               0x7E

#define PCI_BUS_NUMBER_PCH_IOAPIC                     0x00
#define PCI_DEVICE_NUMBER_PCH_IOAPIC                  0x1F

#define PCI_FUNCTION_NUMBER_PCH_IOAPIC                0x0

#define PXE_BASE_OPTION_ROM_FILE_GUID \
  { \
    0x6f2bc426, 0x8d83, 0x4f17, 0xbe, 0x2c, 0x6b, 0x1f, 0xcb, 0xd1, 0x4c, 0x80 \
  }

//
// LT Equates
//
#ifdef LT_FLAG
  #define ACM_BASE                      FixedPcdGet32(PcdFlashFvAcmBase)
  #define ACM_SIZE                      0x00020000
  #define LT_PLATFORM_DPR_SIZE          0x00600000
// APTIOV_SERVER_OVERRIDE_RC_START - USING AMI SDL TOKEN
//  #define CMOS_LTSX_OFFSET              0x2A
  #define CMOS_LTSX_OFFSET                LTSX_TXT_CMOS_ADDRESS
// APTIOV_SERVER_OVERRIDE_RC_END -  - USING AMI SDL TOKEN
  #define ACM_VERSION_OFFSET            0x6E4    //Acm Architectural Header (0x380 = 896) + (0x340 = Scratch Area 4*(2*96 + 16)) + Offset (0x24 = 36)
  #define ACM_VERSION_OFFSET2           0x4E4    //When ACM_HEADER_LENGTH == A1, ACM version is at 0x4e4
  #define ACM_HEADER_LENGTH_OFFSET      0x4      //If ACM_HEADER_LENGTH == A1, ACM version is at 0x4e4. If ACM_HEADER_LENGTH == E0, ACM version is at 0x6e4
#endif // #ifdef LT_FLAG

#define SW_SMI_OS_REQUEST      0x83  // OS transition request.
#define MEM_ADDR_SHFT_VAL         26    // For 64 MB granularity

//
// AHCI port offset values
//
#define EFI_AHCI_PORT_START                           0x0100
#define EFI_AHCI_PORT_REG_WIDTH                       0x0080
#define EFI_AHCI_PORT_CLB                             0x0000
#define EFI_AHCI_PORT_CLBU                            0x0004
#define EFI_AHCI_PORT_FB                              0x0008
#define EFI_AHCI_PORT_FBU                             0x000C
#define EFI_AHCI_PORT_IS                              0x0010
#define EFI_AHCI_PORT_IE                              0x0014
#define EFI_AHCI_PORT_CMD                             0x0018

#endif
