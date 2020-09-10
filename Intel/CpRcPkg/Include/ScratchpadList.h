/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#ifndef __SCRATCHPAD_LIST__
#define __SCRATCHPAD_LIST__

typedef enum {
  BIOS_STICKY_SCRATCHPAD_0 = 0,
  BIOS_STICKY_SCRATCHPAD_1,
  BIOS_STICKY_SCRATCHPAD_2,
  BIOS_STICKY_SCRATCHPAD_3,
  BIOS_STICKY_SCRATCHPAD_4,
  BIOS_STICKY_SCRATCHPAD_5,
  BIOS_STICKY_SCRATCHPAD_6,
  BIOS_STICKY_SCRATCHPAD_7,
  BIOS_STICKY_SCRATCHPAD_SIZE
} BIOS_STICKY_SCRATCHPAD_NUM;

typedef enum {
  BIOS_NON_SCRATCHPAD_0 = 0,
  BIOS_NON_SCRATCHPAD_1,
  BIOS_NON_SCRATCHPAD_2,
  BIOS_NON_SCRATCHPAD_3,
  BIOS_NON_SCRATCHPAD_4,
  BIOS_NON_SCRATCHPAD_5,
  BIOS_NON_SCRATCHPAD_6,
  BIOS_NON_SCRATCHPAD_7,
  BIOS_NON_SCRATCHPAD_8,
  BIOS_NON_SCRATCHPAD_9,
  BIOS_NON_SCRATCHPAD_10,
  BIOS_NON_SCRATCHPAD_11,
  BIOS_NON_SCRATCHPAD_12,
  BIOS_NON_SCRATCHPAD_13,
  BIOS_NON_SCRATCHPAD_14,
  BIOS_NON_SCRATCHPAD_15,
  BIOS_NON_STICKY_SCRATCHPAD_SIZE
} BIOS_NON_STICKY_SCRATCHPAD_NUM;

typedef enum {
  BIOS_STICKY_LOCKBYPASS_SCRATCHPAD_0 = 0,
  BIOS_STICKY_LOCKBYPASS_SCRATCHPAD_1,
  BIOS_STICKY_LOCKBYPASS_SCRATCHPAD_2,
  BIOS_STICKY_LOCKBYPASS_SCRATCHPAD_3,
  BIOS_STICKY_LOCKBYPASS_SCRATCHPAD_4,
  BIOS_STICKY_LOCKBYPASS_SCRATCHPAD_5,
  BIOS_STICKY_LOCKBYPASS_SCRATCHPAD_6,
  BIOS_STICKY_LOCKBYPASS_SCRATCHPAD_SIZE
} BIOS_STICKY_LOCKBYPASS_SCRATCHPAD_NUM;

typedef enum {
  SCRATCHPAD2_0 = 0,
  SCRATCHPAD2_1,
  SCRATCHPAD2_2,
  SCRATCHPAD2_3,
  SCRATCHPAD2_4,
  SCRATCHPAD2_5,
  SCRATCHPAD2_SIZE
} SCRATCHPAD2_NUM;

typedef enum {
  KTILKSCRATCHCFG32_0 = 0,
  KTILKSCRATCHCFG32_1,
  KTILKSCRATCHCFG32_2,
  KTILKSCRATCHCFG32_SIZE
} KTILKSCRATCHCFG32_NUM;

typedef enum {
  PCIE_SCRATCHPAD_0 = 0,
  PCIE_SCRATCHPAD_1,
  PCIE_SCRATCHPAD_2,
  PCIE_SCRATCHPAD_3,
  PCIE_SCRATCHPAD_4,
  PCIE_SCRATCHPAD_5,
  PCIE_SCRATCHPAD_SIZE
} PCIE_SCRATCHPAD_NUM;

//
// Sticky scratchpad registers
//

//
// Description: Sticky BIOS Scratchpad 0.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Reserved for Intel use.
//
typedef union {
  struct {
    UINT32 EnableJmpHalt : 1; // Bit[0:0] - Enable JMP $ for various Power Management enabling through PEGA
    UINT32 WarmBoot : 1;      // Bit[1:1] - Used to indicate warm boot. Gets set at the end of MRC once MRC has
                              // completed successfully and a reboot is NOT requested by reference code.
    UINT32 DdrLoopTest : 1;   // Bit[2:2] - When set to 1, MRC will reboot after training infinitely for MRC
                              // cycle testing.
    UINT32 MultiThread : 1;   // Bit[3:3] - Set to 1 to indicate the MRC is running multi-threaded.
                              // Will be 0 if single-threaded.
    UINT32 Available_0 : 1;   // Bit[4:4] - This bit is not reserved.
    UINT32 ResetFatalErr : 1; // Bit[5:5] - This bit is fatal error indicator, for reset or halt on system when fatal error occurs.
                              // Will be 1 if the fatal error is encountered, after reset it shold be set to 0.
    UINT32 Available_1 : 3;   // Reserved Bit[8:6] for backward compatible to CLX SSKU and Force Single Thread MRC feature
    UINT32 TrainingMode: 1;   // Bit[9:9] - This bit is to indicate iMC or NVMDIMM in Training mode
    UINT32 DebugOverride : 3; // Bit[10:12] - These bits allow debug level override
    UINT32 Available_2 : 19;  // Bits[29:13] - These bits are not reserved.
  } Bits;
  UINT32 Data;
} BIOS_SCRATCHPAD0_STRUCT;

//
// Sticky BIOS Scratchpad 1.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Reserved for Intel use.
//
typedef union {
  struct {
    UINT32 MemVolt : 2;            // Bits[1:0] - As below:
                                   // 0 = Not set yet (read before MRC initializes it)
                                   // 1 = 1.5V
                                   // 2 = 1.35V
                                   // 3 = Reserved for 1.2x setting
    UINT32 TurnAroundMatchHas : 1; // Bit[2:2] - Used to indicate if turnarounds times match expected values from HAS
                                   // table. If they match this bit is 0, if they do not match this bit is 1.
    UINT32 TurnAroundIsWorse : 1;  // Bit[3:3] - If bit 2 is 1 (turnarounds do not match), then this bit indicates if
                                   // the times are worse than expected. If the times are worse then this bit is 1,
                                   // if they are better then this bit is 0.
    UINT32 ResetAllowed : 1;       // Bit[4:4] - Used for reset allowed notification.
    UINT32 FpgaBitStreamLoaded : 1;// Bit[5:5] - FPGA BitSream loaded.
    UINT32 Available : 6;          // Bits[11:6] - These bits are not reserved.
    UINT32 SvBiosAddrType : 4;     // Bits[15:12] - SVBIOS Address Type  (0x5 = BITS [31:16] will be treated as higher
                                   // 16 bit of PSMI base address; 0xD = BITS [31:16] will be treated as higher 16 bit
                                   // of Dram Shared Mailbox Address).
    UINT32 High16Bits : 16;        // Used by SVBIOS to Publish higher 16 bits of address.
  } Bits;
  UINT32 Data;
} BIOS_SCRATCHPAD1_STRUCT;

//
// Sticky BIOS Scratchpad 2.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Reserved for Intel use.
//
typedef union {
  struct {
    UINT32 CpuList : 8;   // Bits[7:0] - CPU list; This list reflects
                          // the CPUs after the topology is degraded, if needed.
    UINT32 Cpu0Link0 : 3; // Bits[10:8]  - CPU0 Link 0, 1, 2 Valid
    UINT32 Cpu1Link0 : 3; // Bits[13:11] - CPU1 Link 0, 1, 2 Valid
    UINT32 Cpu2Link0 : 3; // Bits[16:14] - CPU2 Link 0, 1, 2 Valid
    UINT32 Cpu3Link0 : 3; // Bits[19:17] - CPU3 Link 0, 1, 2 Valid
    UINT32 Cpu4Link0 : 3; // Bits[22:20] - CPU4 Link 0, 1, 2 Valid
    UINT32 Cpu5Link0 : 3; // Bits[25:23] - CPU5 Link 0, 1, 2 Valid
    UINT32 Cpu6Link0 : 3; // Bits[28:26] - CPU6 Link 0, 1, 2 Valid
    UINT32 Cpu7Link0 : 3; // Bits[31:29] - CPU7 Link 0, 1, 2 Valid
  } Bits;
  UINT32 Data;
} BIOS_SCRATCHPAD2_STRUCT;

//
// Sticky BIOS Scratchpad 3.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: This entire register is available for OEM usage.
//
typedef union {
  struct {
    UINT32 data : 32; // Bits [31:0] - Reserved for OEM use
  } Bits;
  UINT32 Data;
} BIOS_SCRATCHPAD3_STRUCT;
#define TOPOLOGY_BITMAP_64BITS_AND_MASK           0x00FFFFFFFFFFFFFF  //Note: This is the entire 64bit mask data for KtiPortCnt = 6
#define TOPOLOGY_BITMAP_HIGH_32BITS_AND_MASK      0x00FFFFFF          //Note: This is the high 32bit mask data for KtiPortCnt = 6

//
// Sticky BIOS Scratchpad 4.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: This entire register is available for OEM usage.
//
typedef union {
  struct {
    UINT32 data : 32;
  } Bits;
  UINT32 Data;
} BIOS_SCRATCHPAD4_STRUCT;

//
// Sticky BIOS Scratchpad 5.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Reserved for Intel use.
//
typedef union {
  struct {
    UINT32 DdrLoopCount : 16; // Bits[15:0] - DDR Loop Count - when BIT2 of BIOS_SCRATCHPAD0 is set to 1,
                              // this word will contain the current cycle count.
    UINT32 Bds : 4;           // Bits[19:16] - BDS.
    UINT32 ClearCmos : 1;     // Bit[20:20] - Clear CMOS.
    UINT32 CacheDdrio : 1;    // Bit[21:21] - Cache DDRIO
    UINT32 Available1 : 2;    // Bits[23:22] - These bits are not reserved.
    UINT32 JedecInitSeq : 1;  // Bit[24:24] - JEDEC init sequence
    UINT32 Rmt : 1;           // Bit[25:25] - Rank margin tool
    UINT32 Available2 : 6;    // Bits[31:26] - These bits are not reserved.
  } Bits;
  UINT32 Data;
} BIOS_SCRATCHPAD5_STRUCT;

//
// Sticky BIOS Scratchpad 6.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: This register is used for a validation hook, which is productized in EV/ASSET tools.   It will cause BIOS to stall
//           excution at pre-determined locations defined by the major/minor BIOS POST code.  It will be supported in the SKX
//           reference code package and should be extended to other portions of BIOS.  A BIOS Setup option will be provided to
//           enable usage models via Serial Port.
// Socket usage: EV Tools Breakpoint is only used on the System BSP.
//
typedef union {
  struct {
    UINT32 SsaMailbox : 16;       // Bits[15:0] - SSA Mailbox.
    UINT32 EvToolBreakPoint : 16; // Bits [31:16] - EV Tools Breakpoint - This value will be compared to bits[31:16]
                                  // of the scratchpad register containing the major/minor BIOS POST code.  When
                                  // BIOS writes the POST code, it will stall execution while the EV Tools breakpoint
                                  // equals the major/minor POST code.  Internal BIOS will also begin polling for
                                  // Serial debug commands at this time.  If the EV Tools breakpoint is released via
                                  // remote Host (connected via ITP or Serial port), then BIOS execution will resume.
                                  // A value of 0 is defined to be breakpoint disabled.
  } Bits;
  UINT32 Data;
} BIOS_SCRATCHPAD6_STRUCT;

//
// Sticky BIOS Scratchpad 7.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Reserved for Intel use.
//

#define BIOS_SCRATCHPAD7_COMPLETION_DELAY_RESET_COUNT_SIZE  2

typedef union {
  struct {
    UINT32 Available : 4;                   // Bits[3:0] - These bits are not reserved.
    UINT32 EnteredColdResetFlow : 1;        // Bits[4:4] - Entered cold reset flow.
    UINT32 CompletedColdResetFlow : 1;      // Bits[5:5] - Completed cold reset flow.
    UINT32 CompletionDelayResetCount : BIOS_SCRATCHPAD7_COMPLETION_DELAY_RESET_COUNT_SIZE;
                                            // Bits[7:6] - The number of resets requested because the calculated
                                            // completion delay was out of bounds. Only the socket 0 instance of this is
                                            // used. This bitfield is used with workaround S1409301288.
    UINT32 Available1 : 8;                  // Bits[15:8] - These bits are not reserved.
    UINT32 SbspSocketId : 4;                // Bits[19:16] - SBSP socket id
    UINT32 PrefetchFailRecovery : 1;        // Bits[20:20] - Prefetch failure/recovery.
    UINT32 UmaBasedClusteringDowngrade : 2; // Bits[22:21] - Indicate UMA based clusting downgrade
                                            // 0:default; 1: Quad-> Hemi 2: Quad-> Disable 3: Hemi-> Disable
    UINT32 MarginTestfailure : 1;           // Bits[23:23] - This bit is set when Margin Test Fails
    UINT32 DcuModeSelect : 1;               // Bits [24:24] - DCU_MODE select 0/1: 32KB 8-way no-ECC (hardware default) /
                                            // 16KB 4-way with ECC.
    UINT32 DwrBiosStall : 1;                // Bits[25:25] - BIOS Stall if enter DWR.
    UINT32 InDwr : 1;                       // Bits[26:26] - In DWR.
    UINT32 FailMemChkFastColdBoot : 1;      // Bits[27:27] - Bit set when setup option "DEBUG INTERFACE" is enabled
    UINT32 BistFrbEventLastBoot : 1;        // Bits[28:28] - BIST/FRB event occured during the last boot.
    UINT32 RemoteSocketReleased : 1;        // Bits[29:29] - Remote socket released in LT enabled system.
    UINT32 SncFailRecovery : 1;             // Bits[30:30] - Snc failure/recovery.
    UINT32 AepDimmPresent : 1;              // Bits[31:31] - AEP Dimm Present
  } Bits;
  UINT32 Data;
} BIOS_SCRATCHPAD7_STRUCT;

//
// Non sticky Scratchpad registers
//

#define LLC_BIST_COMPLETE    2

//
// Non sticky BIOS Scratchpad 0.  This struct contains the bit definitions for this Scratchpad.
// Used by pCode to indicate progress/status code.
//
// Comments: Reserved for Intel use. Bits [25:0] free for reuse after KTIRC completes.
//
typedef union {
  struct {
    UINT32 BootProgress      : 8;  // Bits[7:0] - Boot Progress.
    UINT32 LlcBistResults    : 8;  // Bits[15:8] - LLC BIST Results.
    UINT32 Adddc             : 10; // Bits[25:16] - ADDDC use.  DfxManualDeviceSpare.
    UINT32 Available         : 4;  // Bits[29:26] - These bits are not reserved.
    UINT32 SlowModeLinkTrain : 1;  // Bit[30] - Set to 1 if links are trained in slow mode. This bit is set by pcode.
    UINT32 UcodeRsvd         : 1;  // Bit [31] - Reserved for ucode use
  } Bits;
  UINT32 Data;
} BIOS_NON_STICKY_SCRATCHPAD0_STRUCT;

#define BOOT_GO_CMD 0x02
//
// Non sticky BIOS Scratchpad 1.  This struct contains the bit definitions for this Scratchpad.
// Used by KTIRC to issue commands such as BOOT_GO to pCode
//
// Comments: Reserved for Intel use.
//
typedef union {
  struct {
    UINT32 ReqToPcode : 8;  // Bits[7:0] - Request to pCode.
    UINT32 Available  : 16; // Bits[23:8] - These bits are not reserved.
    UINT32 RcVersionRevision : 8;  // Bits[31:24] - "rev" part of RC version (major.minor.rev.build)
  } Bits;
  UINT32 Data;
} BIOS_NON_STICKY_SCRATCHPAD1_STRUCT;

//
// Non sticky BIOS Scratchpad 2.  This struct contains the bit definitions for this Scratchpad.
// Stepping and APIC ID of PBSP  /  PBSP check-in
//
// Comments: Reserved for Intel use.
//
typedef union {
  struct {
    UINT32 CheckInBit  : 1;  // Bits[0:0]   - Check in bit local to socket.
    UINT32 InHltState  : 1;  // Bits[1:1]   - Put CPU in halt state
    UINT32 Available1  : 6;  // Bits[7:2]   - These bits are not reserved.
    UINT32 ApicId      : 16; // Bits[23:8]  - APIC ID
    UINT32 Available2  : 3;  // Bits[26:24] = These bits are not reserved.
    UINT32 CpuStepping : 4;  // Bits[30:27] - CPU Stepping
    UINT32 Available   : 1;  // Bits[31]    - This bit is not reserved.
  } Bits;
  UINT32 Data;
} BIOS_NON_STICKY_SCRATCHPAD2_STRUCT;

//
// Non sticky BIOS Scratchpad 3.  This struct contains the bit definitions for this Scratchpad.
// Used by python IOT tools.
//
// Comments: Reserved for Intel use.
// Socket usage: Implemented on each socket.
//
typedef union {
  struct {
    UINT32 IedMemBase : 32; // Bits[31:0] - Ied Memory Base
  } Bits;
  UINT32 Data;
} BIOS_NON_STICKY_SCRATCHPAD3_STRUCT;

//
// Non sticky BIOS Scratchpad 4.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: This entire register is available for OEM usage.
//
typedef union {
  struct {
    UINT32 data : 32; // Bits[31:0] - Reserved for OEM usage.
  } Bits;
  UINT32 Data;
} BIOS_NON_STICKY_SCRATCHPAD4_STRUCT;

//
// Non sticky BIOS Scratchpad 5.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Reserved for Intel MRC use. This register is used to store SPD page caching data.
//
// Socket usage: Implemented on each socket.
//
typedef union {
  struct {
    UINT32 Seg0CachedPage : 2; // Bits[1:0] - SPD Page Cache for Bus Segment 0.
    UINT32 Seg1CachedPage : 2; // Bits[3:2] - SPD Page Cache for Bus Segment 1.
    UINT32 Available      : 28; // Bits[31:4] - Reserved for Intel MRC use.
  } Bits;
  UINT32 Data;
} BIOS_NON_STICKY_SCRATCHPAD5_STRUCT;

//
// Non sticky BIOS Scratchpad 6.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: This entire register is available for OEM usage.
//
typedef union {
  struct {
    UINT32 data : 32; // Bits[31:0] - Reserved for OEM usage.
  } Bits;
  UINT32 Data;
} BIOS_NON_STICKY_SCRATCHPAD6_STRUCT;

//
// Non sticky BIOS Scratchpad 7.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Progress Field (POST Code) - So the main idea is to have a unique POST code at every place and
//           to minimize duplication.  The ultimate goal is to avoid it all together but that gets a bit more tricky.
//           However the primary objective is to enhance debug ability.  So we will have a 32 bit address that will
//           contain the entire POST code. The entire 32 bit address will always be written out to a scratchpad
//           register. This will allow anyone debugging to break at a specific address or IO location.  This can also
//           be outputted to the serial port for debug as well.
//
typedef union {
  struct {
    UINT32 ImpSpecData   : 16; // Bits[15:0] - Implementation specific.
    UINT32 MinorPostCode : 8; // Bits[23:16] - Minor Post code.
    UINT32 MajorPostCode : 8; // Bits[31:24] - Major POST code.
  } Bits;
  UINT32 Data;
} BIOS_NON_STICKY_SCRATCHPAD7_STRUCT;

//
// Non sticky BIOS Scratchpad 8.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Error Code (Optional)
//
typedef union {
  struct {
    UINT32 SubErrCode : 8;    // Bits[7:0] - Minor or sub error code.
    UINT32 MajorErrCode: 8;   // Bits[15:8] - Major Error code.
    UINT32 MinorPostCode : 8; // Bits[23:16] - Minor POST code error occurred.
    UINT32 MajorPostCode : 8; // Bits[31:24] - Major POST Code where error occurred.
  } Bits;
  UINT32 Data;
} BIOS_NON_STICKY_SCRATCHPAD8_STRUCT;

//
// Non sticky BIOS Scratchpad 9.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: This entire register is available for OEM usage.
//
typedef union {
  struct {
    UINT32 data : 32; // Bits[31:0] - Reserved for OEM usage.
  } Bits;
  UINT32 Data;
} BIOS_NON_STICKY_SCRATCHPAD9_STRUCT;

//
// Non sticky BIOS Scratchpad 10.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: This entire register is available for OEM usage.
//
typedef union {
  struct {
    UINT32 data : 32; // Bits[31:0] - Reserved for OEM usage.
  } Bits;
  UINT32 Data;
} BIOS_NON_STICKY_SCRATCHPAD10_STRUCT;

//
// Non sticky BIOS Scratchpad 11.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Reserved for Intel use.
//
typedef union {
  struct {
    UINT32 IedMemSize : 32; // Bits[31:0] - Ied Memory Size Used by python IOT tools
  } Bits;
  UINT32 Data;
} BIOS_NON_STICKY_SCRATCHPAD11_STRUCT;

//
// Non sticky BIOS Scratchpad 12.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: RC version number. The "rev" part of RC verison is in BIOS_NON_STICKY_SCRATCHPAD1_STRUCT.
//
typedef union {
  struct {
    UINT32 RcVersionBuildNumber : 16; // Bits[15:0] - "build" part of RC version (major.minor.rev.build)
    UINT32 RcVersionMinor       : 8;  // Bits[23:16] - "minor" part of RC version (major.minor.rev.build)
    UINT32 RcVersionMajor       : 8;  // Bits[31:24] - "major" part of RC version (major.minor.rev.build)
  } Bits;
  UINT32 Data;
} BIOS_NON_STICKY_SCRATCHPAD12_STRUCT;

//
// Non sticky BIOS Scratchpad 13.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Registers is free for General use after PEI phase. More specifically this register
// is available once memory is available.
//
typedef union {
  struct {
    UINT32 data : 32; // Bits[31:0] - PIPE command/data mailbox
  } Bits;
  UINT32 Data;
} BIOS_NON_STICKY_SCRATCHPAD13_STRUCT;

//
// Non sticky BIOS Scratchpad 14.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Registers is free for General use after PEI phase. More specifically this register is available once memory is available.
//
typedef union {
  struct {
    UINT32 data : 32; // Bits[31:0] - PIPE command/data mailbox
  } Bits;
  UINT32 Data;
} BIOS_NON_STICKY_SCRATCHPAD14_STRUCT;

//
// Non sticky BIOS Scratchpad 15.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Registers is free for General use after PEI phase. More specifically this register is available once memory is available.
//
typedef union {
  struct {
    UINT32 data : 32; // Bits[31:0] - PIPE command/data mailbox
  } Bits;
  UINT32 Data;
} BIOS_NON_STICKY_SCRATCHPAD15_STRUCT;

//
// Sticky LockBypass BIOS Scratchpad registers
//

//
// Sticky LockBypass BIOS Scratchpad 0.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Registers is free for General use after PEI phase. More specifically this register is available once memory is available.
//
typedef union {
  struct {
    UINT32 data : 32; // Bits[31:0] - Used by both KTI and MRC. KTI Adaptation Link 0. SSA Usage during MRC - SSA restores value when done.
  } Bits;
  UINT32 Data;
} BIOS_STICKY_LOCKBYPASS_SCRATCHPAD0_STRUCT;

//
// Sticky LockBypass BIOS Scratchpad 1.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Registers is free for General use after PEI phase. More specifically this register is available once memory is available.
//
typedef union {
  struct {
    UINT32 data : 32; // Bits[31:0] - Used by both KTI and MRC. KTI Adaptation Link 1. SSA Usage during MRC - SSA restores value when done.
  } Bits;
  UINT32 Data;
} BIOS_STICKY_LOCKBYPASS_SCRATCHPAD1_STRUCT;

//
// Sticky LockBypass BIOS Scratchpad 2.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Registers is free for General use after PEI phase. More specifically this register is available once memory is available.
//
typedef union {
  struct {
    UINT32 data : 32; // Bits[31:0] - KTI Adaptation Link 2.
  } Bits;
  UINT32 Data;
} BIOS_STICKY_LOCKBYPASS_SCRATCHPAD2_STRUCT;

//
// Sticky LockBypass BIOS Scratchpad 3.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Reserved for Intel use.
//
typedef union {
  struct {
    UINT32 data : 32; // Bits[31:0] - These bits are not reserved.
  } Bits;
  UINT32 Data;
} BIOS_STICKY_LOCKBYPASS_SCRATCHPAD3_STRUCT;

//
// Sticky LockBypass BIOS Scratchpad 4.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Reserved for Intel use.
//
typedef union {
  struct {
    UINT32 data : 32; // Bits[31:0] - These bits are not reserved.
  } Bits;
  UINT32 Data;
} BIOS_STICKY_LOCKBYPASS_SCRATCHPAD4_STRUCT;

//
// Sticky LockBypass BIOS Scratchpad 5.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Reserved for Intel use. Currently used for CPX to save topology bitmap/type/valid etc.
// Also used in SPR to save topology bitmap.
//
typedef union {
  struct {
    UINT32 Cpu4Link0 : 6;     // Bits[5:0]   - For CPX: CPU4 Link 0, 1, 2, 3, 4, 5 Valid
    UINT32 Cpu5Link0 : 6;     // Bits[11:6]  - For CPX: CPU5 Link 0, 1, 2, 3, 4, 5 Valid
    UINT32 Cpu6Link0 : 6;     // Bits[17:12] - For CPX: CPU6 Link 0, 1, 2, 3, 4, 5 Valid
    UINT32 Cpu7Link0 : 6;     // Bits[23:18] - For CPX: CPU7 Link 0, 1, 2, 3, 4, 5 Valid
    UINT32 TopoType : 1;      // Bits[24]    - For CPX: Topology Type: 0b=DPW; 1b=AFC
    UINT32 TopoTypeValid : 1; // Bits[25]    - For CPX: Topology Type valid or not: 0b=Invalid; 1b=Valid
    UINT32 Rsvd : 6;          // Bits[31:26] - Reserved for OEM use
  } Bits;
  UINT32 Data;
} BIOS_STICKY_LOCKBYPASS_SCRATCHPAD5_STRUCT;

//
// Sticky LockBypass BIOS Scratchpad 6.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Reserved for Intel use.
//
typedef union {
  struct {
    UINT32 data : 32; // Bits[31:0] - These bits are not reserved.
  } Bits;
  UINT32 Data;
} BIOS_STICKY_LOCKBYPASS_SCRATCHPAD6_STRUCT;

//
// SCRATCHPAD2 (MC_MAIN)
//

//
// SCRATCHPAD2 0.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Reserved for Intel use.
//
typedef union {
  struct {
    UINT32 RankSpareCh0 : 8; // Bits[8:0] - Rank spare infor CH 0
    UINT32 Available : 24;    // Bits[31:9] - These bits are not reserved.
  } Bits;
  UINT32 Data;
} SCRATCHPAD2_0_STRUCT;

//
// SCRATCHPAD2 1.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Reserved for Intel use.
//
typedef union {
  struct {
    UINT32 RankSpareCh1 : 8;  // Bits[8:0] - Rank spare infor CH 1
    UINT32 Available : 24;    // Bits[31:9] - These bits are not reserved.
  } Bits;
  UINT32 Data;
} SCRATCHPAD2_1_STRUCT;

//
// SCRATCHPAD2 2.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Reserved for Intel use.
//
typedef union {
  struct {
    UINT32 RankSpareCh2 : 8; // Bits[8:0] - Rank spare infor CH 2
    UINT32 Available : 24;    // Bits[31:9] - These bits are not reserved.
  } Bits;
  UINT32 Data;
} SCRATCHPAD2_2_STRUCT;

//
// SCRATCHPAD2 3.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Reserved for Intel use.
//
typedef union {
  struct {
    UINT32 RankSpareCh3 : 8; // Bits[8:0] - Rank spare infor CH 3
    UINT32 Available : 24;    // Bits[31:9] - These bits are not reserved.
  } Bits;
  UINT32 Data;
} SCRATCHPAD2_3_STRUCT;

//
// SCRATCHPAD2 4.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Reserved for Intel use.
//
typedef union {
  struct {
    UINT32 RankSpareCh4 : 8; // Bits[8:0] - Rank spare infor CH 4
    UINT32 Available : 24;    // Bits[31:9] - These bits are not reserved.
  } Bits;
  UINT32 Data;
} SCRATCHPAD2_4_STRUCT;

//
// SCRATCHPAD2 5.  This struct contains the bit definitions for this Scratchpad.
//
// Comments: Reserved for Intel use.
//
typedef union {
  struct {
    UINT32 RankSpareCh5 : 8; // Bits[8:0] - Rank spare infor CH 5
    UINT32 Available : 24;    // Bits[31:9] - These bits are not reserved.
  } Bits;
  UINT32 Data;
} SCRATCHPAD2_5_STRUCT;

//
// KTILKSCRATCHCFG32
//

//
// KTILKSCRATCHCFG32 0.  This struct contains the bit definitions for this Scratchpad.
//
typedef union {
  struct {
    UINT32 UniphyVer : 16; // Bits[15:0] - Uniphy recipe version
    UINT32 Available : 16; // Bits[31:16] - These bits are not reserved.
  } Bits;
  UINT32 Data;
} KTILKSCRATCHCFG32_0_STRUCT;

//
// KTILKSCRATCHCFG32 1.  This struct contains the bit definitions for this Scratchpad.
//
typedef union {
  struct {
    UINT32 UniphyVer : 16; // Bits[15:0] - Uniphy recipe version
    UINT32 Available : 16; // Bits[31:16] - These bits are not reserved.
  } Bits;
  UINT32 Data;
} KTILKSCRATCHCFG32_1_STRUCT;

//
// KTILKSCRATCHCFG32 2.  This struct contains the bit definitions for this Scratchpad.
//
typedef union {
  struct {
    UINT32 UniphyVer : 16; // Bits[15:0] - Uniphy recipe version
    UINT32 Available : 16; // Bits[31:16] - These bits are not reserved.
  } Bits;
  UINT32 Data;
} KTILKSCRATCHCFG32_2_STRUCT;

//
// PCIE SCRATCHPAD
//

//
// PCIE_SCRATCHPAD0.  This struct contains the bit definitions for this Scratchpad.
//
typedef union {
  struct {
    UINT32 UniphyVer : 16; // Bits[15:0] - Uniphy recipe version
    UINT32 Available : 16; // Bits[31:16] - These bits are not reserved.
  } Bits;
  UINT32 Data;
} PCIE_SCRATCHPAD0_STRUCT;

//
// PCIE_SCRATCHPAD1.  This struct contains the bit definitions for this Scratchpad.
//
typedef union {
  struct {
    UINT32 UniphyVer : 16; // Bits[15:0] - Uniphy recipe version
    UINT32 Available : 16; // Bits[31:16] - These bits are not reserved.
  } Bits;
  UINT32 Data;
} PCIE_SCRATCHPAD1_STRUCT;

//
// PCIE_SCRATCHPAD2.  This struct contains the bit definitions for this Scratchpad.
//
typedef union {
  struct {
    UINT32 UniphyVer : 16; // Bits[15:0] - Uniphy recipe version
    UINT32 Available : 16; // Bits[31:16] - These bits are not reserved.
  } Bits;
  UINT32 Data;
} PCIE_SCRATCHPAD2_STRUCT;

//
// PCIE_SCRATCHPAD3.  This struct contains the bit definitions for this Scratchpad.
//
typedef union {
  struct {
    UINT32 UniphyVer : 16; // Bits[15:0] - Uniphy recipe version
    UINT32 Available : 16; // Bits[31:16] - These bits are not reserved.
  } Bits;
  UINT32 Data;
} PCIE_SCRATCHPAD3_STRUCT;

//
// PCIE_SCRATCHPAD4.  This struct contains the bit definitions for this Scratchpad.
//
typedef union {
  struct {
    UINT32 UniphyVer : 16; // Bits[15:0] - Uniphy recipe version
    UINT32 Available : 16; // Bits[31:16] - These bits are not reserved.
  } Bits;
  UINT32 Data;
} PCIE_SCRATCHPAD4_STRUCT;

//
// PCIE_SCRATCHPAD5.  This struct contains the bit definitions for this Scratchpad.
//
typedef union {
  struct {
    UINT32 UniphyVer : 16; // Bits[15:0] - Uniphy recipe version
    UINT32 Available : 16; // Bits[31:16] - These bits are not reserved.
  } Bits;
  UINT32 Data;
} PCIE_SCRATCHPAD5_STRUCT;

#endif // #ifndef __SCRATCHPAD_LIST__
