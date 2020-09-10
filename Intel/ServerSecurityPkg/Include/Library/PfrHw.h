/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#ifndef _PFR_HW_H_
#define _PFR_HW_H_

//-----------------------------------------------------------------------------------------------------
#pragma pack (push, 1)
//-----------------------------------------------------------------------------------------------------
//
#define BIT_MASK(x)                       ((UINT32) (1 << (x)))
//
//-----------------------------------------------------------------------------------------------------
//---------------------- PFR Region in SPI FLASH ------------------------------------------------------
#define PFR_FLASH_REGION_NUMBER                 0x0E    // PFR Region# in SPI Flash
//#define PFR_FLASH_REGION_BASE             0x007F0000    // PFR Region Base Address (relative to start of Flash Device)
//#define PFR_FLASH_REGION_SIZE             0x02810000    // PFR Region Base Size in bytes
//#define PFR_STAGING_AREA_BASE             0x007F0000    // PFR Staging Area Base Address (relative to start of Flash Device)
//#define PFR_STAGING_AREA_SIZE             0x01400000    // PFR Staging Area Size in bytes
//#define PFR_RECOVERY_AREA_BASE            0x01BF0000    // PFR Recovery Area Base Address (relative to start of Flash Device)
//#define PFR_RECOVERY_AREA_SIZE            0x01400000    // PFR Recovery Area Size in bytes
//#define PFR_PFM_AREA_BASE                 0x02FF0000    // PFR PFM Area Base Address (relative to start of Flash Device)
#define PFR_PFM_AREA_SIZE                 0x00010000    // PFR PFM Area Size in bytes
//
//-----------------------------------------------------------------------------------------------------
//------------- CPLD SPECIFICATION INFORMATION --------------------------------------------------------
// Number of CPLD Registers
#define NUMBER_OF_CPLD_REGISTERS               0x100    // #of CPLD Registers (1-based value)
// UFM FIFO Size
#define UFM_FIFO_SIZE_B                         0x20    // UFM FIFO size in bytes
//
// UFM FIFO Flush Delay
#define UFM_FIFO_FLUSH_DELAY_MS                 0x01    // Delay in ms after UFM FIFO Flush
//
// CPLD Mailbox Registers
#define CPLD_ROT_STATIC_IDENTIFIER_REGISTER           0x00    // CPLD RoT Static Identifier
#define CPLD_ROT_RELEASE_VERSION_REGISTER             0x01    // Release Version of CPLD RoT Design
#define CPLD_ROT_SVN_REGISTER                         0x02    // SVN of CPLD RoT Design
#define CPLD_PLATFORM_STATE_REGISTER                  0x03    // Platform State
#define CPLD_RECOVERY_COUNT_REGISTER                  0x04    // #of FW Recovery actions since last AC-ON
#define CPLD_LAST_RECOVERY_REASON_REGISTER            0x05    // Last Recovery Reason
#define CPLD_PANIC_EVENT_COUNT_REGISTER               0x06    // #of Panic Events (T0 -> T-1 transition) since last AC-ON
#define CPLD_LAST_PANIC_REASON_REGISTER               0x07    // Last Panic reason
#define CPLD_MAJOR_ERROR_CODE_REGISTER                0x08    // Major error code
#define CPLD_MINOR_ERROR_CODE_REGISTER                0x09    // Minor error code
//
#define CPLD_UFM_STATUS_REGISTER                      0x0A    // UFM/Provisioning Status
#define   US_COMMAND_BUSY_BIT                         0x01    //    Bit-0:  Command Busy
#define   US_COMMAND_DONE_BIT                         0x02    //    Bit-1:  Command Done
#define   US_COMMAND_ERROR_BIT                        0x04    //    Bit-2:  Command Error
                                                              //    Bit-3:  Reserved
#define   US_UFM_LOCKED_BIT                           0x10    //    Bit-4:  UFM Locked
#define   US_UFM_PROVISIONED_BIT                      0x20    //    Bit-5:  UFM Provisioned
#define   US_PIT_LEVEL1_PROTECTION_ENABLED_BIT        0x40    //    Bit-6: PIT Level-1 Protection Enabled
#define   US_PIT_LEVEL2_PROTECTION_ENABLED_BIT        0x80    //    Bit-7: PIT Level-1 Protection Enabled
//
#define CPLD_UFM_COMMAND_REGISTER                     0x0B    // UFM/Provisioning Command
#define   UC_ERASE_CURRENT_PROVISION                  0x00    //    00h: Erase Current Provision (not-locked)
#define   UC_PROVISION_ROOT_KEY                       0x01    //    01h: Provision Root Key
#define   UC_PROVISION_PIT_PASSWORD                   0x02    //    02h: Provision PIT Password
#define      PIT_PASSWORD_SIZE_B                      0x08    //         PIT password size in bytes
                                                              //03h-04h: Reserved
#define   UC_PROVISION_PCH_OFFSETS                    0x05    //    05h: Provision PCH Offsets (Active/PFM, Recovery, Staging)
#define   UC_PROVISION_BMC_OFFSETS                    0x06    //    06h: Provision BMC Offsets (Active/PFM, Recovery, Staging)
#define   UC_END_OF_PROVISION                         0x07    //    07h: End of Provision / Lock UFM
#define   UC_READ_ROOT_KEY                            0x08    //    08h: Read Root Key
#define   UC_READ_PIT_PASSWORD                        0x09    //    09h: Read PIT Password
#define   UC_READ_PIT_PCH_SPI_FLASH_IMAGE_HASH        0x0A    //    0Ah: Read PIT PCH SPI Flash Image Hash
#define   UC_READ_PIT_BMC_SPI_FLASH_IMAGE_HASH        0x0B    //    0Bh: Read PIT BMC SPI Flash Image Hash
#define   UC_READ_PCH_OFFSETS                         0x0C    //    0Ch: Read PCH Offsets
#define   UC_READ_BMC_OFFSETS                         0x0D    //    0Dh: Read BMC Offsets
#define   UC_RECONFIG_CPLD                            0x0E    //    0Eh: Reconfig CPLD
                                                              //    0Fh: Reserved
#define   UC_ENABLE_PIT_LEVEL_1_PROTECTION            0x10    //    10h: Enable PIT Level-1 Protection
#define   UC_ENABLE_PIT_LEVEL_2_PROTECTION            0x11    //    11h: Enable PIT Level-2 Protection
                                                              //12h-FFh: Reserved
//
#define CPLD_UFM_COMMAND_TRIGGER_REGISTER             0x0C    // UFM Command Trigger
#define   UCT_COMMAND_TRIGGER_BIT                     0x01    //    Bit-0:  Execute command
#define   UCT_FLUSH_WRITE_FIFO_BIT                    0x02    //    Bit-1:  Flush Write FIFO
#define   UCT_FLUSH_READ_FIFO_BIT                     0x04    //    Bit-2:  Flush Read FIFO
                                                              //    Bit7-3: Reserved for future use
//
#define CPLD_UFM_WRITE_FIFO_REGISTER                  0x0D    // UFM Write FIFO
#define CPLD_UFM_READ_FIFO_REGISTER                   0x0E    // UFM Read FIFO
#define CPLD_BMC_CHECKPOINT_REGISTER                  0x0F    // Status of BMC, write from BMC allowed only
                                                              //    until BMC signals end of execution,
                                                              //    write from BMC re-allowed on CPU reset
#define CPLD_ACM_CHECKPOINT_REGISTER                  0x10    // Status of ACM, write from CPU allowed only
                                                              //    until ACM signals end of execution,
                                                              //    write from CPU re-allowed on CPU reset
#define CPLD_BIOS_CHECKPOINT_REGISTER                 0x11    // Status of BIOS, write from CPU allowed only
                                                              //    until BIOS signals end of execution,
                                                              //    write from CPU re-allowed on CPU reset
#define CPLD_PCH_UPDATE_INTENT_REGISTER               0x12    // FW Update Intent from CPU, R/W allowed from CPU
#define   PUI_PCH_ACTIVE_IMAGE_BIT                    0x01    //   Bit-0 = Update PCH Active Image
#define   PUI_PCH_RECOVERY_IMAGE_BIT                  0x02    //   Bit-1 = Update PCH Recovery Image
#define   PUI_CPLD_IMAGE_BIT                          0x04    //   Bit-2 = Update CPLD Image
                                                              //   Bit5-3..Reserved
#define   PUI_PCH_DYNAMIC_REGION_BIT                  0x40    //   Bit-6 = Update PCH Dynamic Region (Valid for Update PCH Active Image i.e. Bit-0 = 1)
#define   PUI_DEFER_TILL_NEXT_RESET_BIT               0x80    //   Bit-7 = Defer update till next reset
//
#define CPLD_BMC_UPDATE_INTENT_REGISTER               0x13    // FW Update Intent from BMC, R/W allowed from BMC
#define CPLD_PCH_PFR_ACTIVE_SVN_REGISTER              0x14    // SVN of PCH PFR Active
#define CPLD_PCH_PFM_ACTIVE_MAJOR_VERISON_REGISTER    0x15    // Major Version of PCH PFM Active
#define CPLD_PCH_PFM_ACTIVE_MINOR_VERISON_REGISTER    0x16    // Minor Version of PCH PFM Active
#define CPLD_BMC_PFR_ACTIVE_SVN_REGISTER              0x17    // SVN of BMC PFR Active
#define CPLD_BMC_PFM_ACTIVE_MAJOR_VERISON_REGISTER    0x18    // Major Version of BMC PFM Active
#define CPLD_BMC_PFM_ACTIVE_MINOR_VERISON_REGISTER    0x19    // Minor Version of BMC PFM Active
#define CPLD_PCH_PFR_RECOVERY_SVN_REGISTER            0x1A    // SVN of PCH PFR Recovery
#define CPLD_PCH_PFM_RECOVERY_MAJOR_VERISON_REGISTER  0x1B    // Major Version of PCH PFM Recovery
#define CPLD_PCH_PFM_RECOVERY_MINOR_VERISON_REGISTER  0x1C    // Minor Version of PCH PFM Recovery
#define CPLD_BMC_PFR_RECOVERY_SVN_REGISTER            0x1D    // SVN of BMC PFR PFR Recovery
#define CPLD_BMC_PFM_RECOVERY_MAJOR_VERISON_REGISTER  0x1E    // Major Version of BMC PFM Recovery
#define CPLD_BMC_PFM_RECOVERY_MINOR_VERISON_REGISTER  0x1F    // Minor Version of BMC PFM Recovery
#define CPLD_ROT_HASH_REGISTER                        0x20    // CPLD RoT Hash 0x20-0x5F
#define   NUM_CPLD_ROT_HASH_REGISTERS                 0x40    //   #of CPLD RoT Hash Register
                                                              // 0x60-0x7F: Reserved, R/O for CPU/BMC
#define CPLD_ACM_BIOS_SCRATCH_PAD_REGISTER            0x80    // 0x80-0xBF: Scratchpad for ACM/BIOS,
                                                              //    persists across DC-Off/On cycles, R/W allowed from CPU, R/O from BMC
#define CPLD_BMC_SCRATCH_PAD_REGISTER                 0xC0    // 0xC0-0xFF: Scratchpad for BMC,
                                                              //    persists across DC-Off/On cycles, R/W allowed from BMC, R/O from CPU
//
// Checkpoint Code: BIOS -> CPLD
#define PFR_STATUS_BLOCK_EXEC_START             0x01    // 01h = Block Execution Entry Point
#define PFR_STATUS_BLOCK_AUTH_SUCCESS           0x02    // 02h = Block Authentication Successful
#define PFR_STATUS_BLOCK_AUTH_ERROR             0x03    // 03h = Block Authentication Error
#define PFR_STATUS_PLATFORM_AUTHORITY_EXIT      0x04    // 04h = Control flow about to go outside of platform authority
#define PFR_STATUS_EXT_BLOCK_EXEC_START         0x05    // 05h = Control flow about to xfer to third party code
#define PFR_STATUS_EXT_BLOCK_EXEC_END           0x06    // 06h = Control flow returned from third party code
#define PFR_STATUS_BLOCK_EXEC_PAUSE             0x07    // 07h = Block Execution paused due to human intervention (e.g. going to Setup)
#define PFR_STATUS_BLOCK_EXEC_RESUME            0x08    // 08h = Block Execution resumed from human intervention (e.g. returned from Setup)
#define PFR_STATUS_BLOCK_EXEC_COMPLETE          0x09    // 09h = Block Execution about launch OS or Runtime Block
#define PFR_STATUS_MANAGEMENT_MODE_START        0x0A    // 0Ah = Management Mode (SMM) Execution Start
#define PFR_STATUS_MANAGEMENT_MODE_END          0x0B    // 0Bh = Management Mode (SMM) Execution End
                                                        // 0Ch-7Fh.....Reserved
#define PFR_STATUS_READY_TO_BOOT                0x80    // 80h = About to xfer control to OS Loader
#define PFR_STATUS_EXIT_BOOT_SERVICES           0x81    // 81h = About to end ExitBootServices()
#define PFR_STATUS_RESET_HOST                   0x82    // 82h = About to reset Host using BIOS Checkpoint Register
#define PFR_STATUS_RESET_ME                     0x83    // 83h = About to reset ME using BIOS Checkpoint Register
#define PFR_STATUS_RESET_BMC                    0x84    // 84h = About to reset BMC using BIOS Checkpoint Register
//
//-----------------------------------------------------------------------------------------------------
//-------------	IMPLEMENTATION SPECIFIC INFORMATION ---------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//
// Max #of sockets supported
#define MAX_NUM_OF_SOCKETS                       0x08   // Max #of Sockets supported
//
// #of sockets per motherboard
#define NUM_OF_SOCKETS_PER_BOARD                 0x02   // #of sockets per motherboard
//
// Max #of times to check Checkpoint Register being reset to 0
#define MAX_NUM_READ_CHECKPOINT                   100   // Max #of times to check [Checkpoint Register] being resset to 0
//
// Wait time for UFM command completion
#define UFM_COMMAND_COMPLETION_TIMEOUT_MS         100   // Max Wait time in ms for UFM Command Completion
#define UFM_COMMAND_COMPLETION_INTERVAL_DELAY_US 1000   // Delay in us between successive checking of UFM command completion
//
// Wait time for UFM to be free
#define UFM_FREE_TIMEOUT_MS                       100   // Max Wait time in ms for UFM to be free
#define UFM_FREE_INTERVAL_DELAY_US               1000   // Delay in us between successive checking of UFM to be free
//
// Wait time for UFM to be ready to accept command
#define UFM_COMMAND_READY_TIMEOUT_MS              100   // Max Wait time in ms for UFM to be ready to accept command
#define UFM_COMMAND_READY_INTERVAL_DELAY_US      1000   // Delay in us between successive checking of UFM readiness to accept command
//
// PFR Data Buffer used to supply data to UFM Write FIFO and receive data from UFM Read FIFO.
//	Length indicates the #of bytes (excluding Length field) that follows the Length field.
typedef struct {
  UINT32    Length;                                     // Size (in bytes) of data that follows this field
                                                        // **** NOTE: Length does NOT include Length field ****
  UINT8     Data [2 * UFM_FIFO_SIZE_B];                 // Data bytes of variable length (size determined by Length field)
} PFR_DATA_BUFFER;
//
// PFR_LIB_DATA: Contains different information used by Library functions
//
typedef    struct {                           // Offset  Description
  UINT8    Attribute;                         // 0x00    Bit-mapped info of differnt platform information
// CPUID
  UINT32   CpuIdEAX;                          // 0x01    Output EAX of CPUID with EAX = 1
  UINT32   CpuIdEBX;                          // 0x05    Output EBX of CPUID with EAX = 1
  UINT32   CpuIdECX;                          // 0x09    Output ECX of CPUID with EAX = 1
  UINT32   CpuIdEDX;                          // 0x0D    Output EDX of CPUID with EAX = 1
// SMBUS
  UINT32   SmbusCommandRegister;              // 0x11    SMBUS Command Register
  UINT32   SmbusStatusRegister;               // 0x15    SMBUS Status Register
  UINT32   SmbusDataRegister;                 // 0x19    SMBUS Data Register
// ACM
  UINT32   AcmPolicyStatus;                   // 0x1D    [ACM Policy Status Register]
  UINT32   AcmBtgBootIndicator;               // 0x21    [ACM BtG Boot Indicator MSR]
// Board
  UINT16   PlatformType;                      // 0x25    Platform Type
  UINT8    BoardId;                           // 0x27    Board Identifier
  UINT32   BoardRevisionId;                   // 0x28    Board Revision Identifier
  UINT8    NumCpld;                           // 0x2C    #of (1-based) of active CPLD
  UINT32   SocketActive;                      // 0x2D    Bit-mapped info of active socket: Bit-n = 0/1, Socket-n Not/Yes active
// PFR
  UINT8    UfmStatusRegister;                 // 0x31    [UFM Status Register]
// BPM
  UINT32   KeyManifest;                       // 0x32    Ptr to Key Manifest
  UINT32   KeyManifestSize;                   // 0x36    Key Manifest size in bytes
  UINT32   BootPolicyManifest;                // 0x3A    Ptr to Boot Policy Manifest
  UINT32   BootPolicyManifestSize;            // 0x3E    Boot Policy Manifest size in bytes
  UINT32   RootKeyHash;                       // 0x42    Ptr to Root Key Hash
  UINT32   RootKeyHashSize;                   // 0x46    Root Key Hash size in bytes
  UINT32   PchOffsets;                        // 0x4A    Ptr to PCH Offsets
  UINT32   PchOffsetSize;                     // 0x4E    PCH Offsets size in bytes
  UINT32   BmcOffsets;                        // 0x52    Ptr to BMC Offsets
  UINT32   BmcOffsetSize;                     // 0x56    BMC Offsets size in bytes
  UINT8    SmbusDeviceAddressCpld;            // 0x5A    CPLD SMBUS Device Address
} PFR_LIB_DATA;
//
// Attribute: Bit-mapped information indicating which group of fields are valid
#define   PA_CPUID_BIT                  0x01  // Bit-0 = 0/1, CPUID info field not/yes valid
#define   PA_SMBUS_BIT                  0x02  // Bit-1 = 0/1, SMBUS info field not/yes valid
#define   PA_ACM_BIT                    0x04  // Bit-2 = 0/1, ACM info field not/yes valid
#define   PA_BOARD_BIT                  0x08  // Bit-3 = 0/1, Board info field not/yes valid
#define   PA_PFR_BIT                    0x10  // Bit-4 = 0/1, PFR info field not/yes valid
#define   PA_BPM_BIT                    0x20  // Bit-5 = 0/1, BPM info field not/yes valid
#define   PA_UNDEFINED_BIT_MASK         0xC0  // Bit7-6.......Reserved for future use (MUST be 0)
//
//
// Platform State based on PFR_EN, PFR_LOCK, PFR_PROVISIONED, BtG Profile (>=3)
//
#define PS_BTG_PROFILE_345_BIT                  0x01    // Bit-0 = 1, BtG Profile >=3 (as determine from ACM)
#define PS_PFR_PROVISIONED_BIT                  0x02    // Bit-1 = 1, PFR Provisioned (as determined from PFR HW)
#define PS_PFR_LOCKED_BIT                       0x04    // Bit-1 = 1, PFR Locked (as determined from PFR HW)
#define PS_PFR_EN_BIT                           0x08    // Bit-1 = 1, PFR_EN enabled (as determined from ACM)
                                                        // Bit7-4 = Not used
//
//------------- CPUID INFORMATION ----------------------------------------------------------------------
//
// Output EAX of CPUID (with EAX = 1)
//        Bit31-4 = Processor Type (stored as CpuType field in EFI_PLATFORM_INFO)
//        Bit3-0  = Stepping (stored as CpuStepping field in EFI_PLATFORM_INFO)
//
#define CPUID_EAX_01                      0x00000001    // CPU Leaf 01
#define PFR_CPX_CPUID	                     0x5065A    // CPX: Output EAX of CPUID (EAX = 1)
#define PFR_ICX_CPUID	                     0x606A0    // ICX: Output EAX of CPUID (EAX = 1)
#define PFR_ICXD_CPUID	                     0x606C0    // ICX-D: Output EAX of CPUID (EAX = 1)
//
//------------- ACM INFORMATION ------------------------------------------------------------------------
//
#define ACM_POLICY_STATUS_REGISTER        0xFED30378    // ACM_POLICY_STATUS register
#define   APSR_PFR_SUPPORT_BIT          BIT_MASK(17)    // Bit-17 = 0/1, PFR_EN Not/Yes enabled
//
#define ACM_BTG_BOOT_INDICATOR_MSR            0x013A    // ACM_BTG_BOOT_INDICATOR register -> MSR 13A
#define   ABBIM_VERIFIED_BIT             BIT_MASK(6)    // Bit-6  = 0/1, BtG Profile Not/Yes (>= 3)
//
//-----------------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------------
#pragma pack (pop)
//-----------------------------------------------------------------------------------------------------

#endif
