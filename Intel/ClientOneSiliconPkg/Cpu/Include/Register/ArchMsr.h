
/** @file
  ArchMsr.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

#ifndef _ARCHMSR_h
#define _ARCHMSR_h
#include <Base.h>

/**
  Machine Check Status Register
**/
#define MSR_P5_MC_ADDR 0x00000001

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Mcacod : 9;

                            /* Bits[8:0], Access Type=RW_V, default=None*/

                            /* Machine Check Architecture Error Code */
    UINT32 EnhMcaAvail0 : 7;

                            /* Bits[15:9], Access Type=RW_V, default=None*/

                            /* Reserved bits */
    UINT32 Mscod : 4;

                            /* Bits[19:16], Access Type=RW_V, default=None*/

                            /* Machine Specific Architecture Error Code */
    UINT32 EnhMcaAvail10 : 12;

                            /* Bits[31:20], Access Type=RW_V, default=None*/

                            /* Reserved bits */
    UINT32 EnhMcaAvail11 : 6;

                            /* Bits[37:32], Access Type=RW_V, default=None*/

                            /* Reserved bits */
    UINT32 CorrectedErrCnt : 14;

                            /* Bits[51:38], Access Type=RW_V, default=None*/

                            /*
                               Corrected error count since last clear of mc
                               regs
                            */
    UINT32 Sticky : 1;

                            /* Bits[52:52], Access Type=RW_V, default=None*/

                            /* Sticky bit set when error count overflows */
    UINT32 EnhMcaAvail2 : 2;

                            /* Bits[54:53], Access Type=RW_V, default=None*/

                            /* Reserved bits */
    UINT32 Ar : 1;

                            /* Bits[55:55], Access Type=RW_V, default=None*/

                            /* Action required */
    UINT32 S : 1;

                            /* Bits[56:56], Access Type=RW_V, default=None*/

                            /* Signaling */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RW_V, default=None*/

                            /* Processor Context Corrupt */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RW_V, default=None*/

                            /* MC0_ADDR register valid */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RW_V, default=None*/

                            /* MC0_MISC register valid */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RW_V, default=None*/

                            /* Error Enabled */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RW_V, default=None*/

                            /* Uncorrectable Error */
    UINT32 Over : 1;

                            /* Bits[62:62], Access Type=RW_V, default=None*/

                            /* Machine Check Overflow */
    UINT32 Val : 1;

                            /* Bits[63:63], Access Type=RW_V, default=None*/

                            /* Status register Valid */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_P5_MC_ADDR_REGISTER;

/**
  Used by the OS or BIOS to program to the maximum MONITOR line size in the system. CPUID will return the maximum line size of the processor and the system for the purposes of Monitor and MWait.  Software can use this information to pad data appropriately to avoid cross-cluster hot-spots.
**/
#define MSR_MONITOR_FILTER_SIZE 0x00000006

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LineSize : 16;

                            /* Bits[15:0], Access Type=RW, default=None*/

                            /*
                               Specifies the number of bytes in a cache line or
                               chipset line buffer. BIOS reads this field and
                               the chipset line buffer register. BIOS then
                               programs this register field with the larger of
                               the two values.
                            */
    UINT32 Rsvd16 : 16;

                            /* Bits[31:16], Access Type=RW, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MONITOR_FILTER_SIZE_REGISTER;
/**
  The time-stamp counter is a 64-bit counter that is set to 0 following a RESET of the processor. Following a RESET, the counter increments even when the processor is halted by the HLT instruction or the external STPCLK# pin. Note that the assertion of the external DPSLP# pin may cause the time-stamp counter to stop.
**/
#define MSR_TIME_STAMP_COUNTER 0x00000010

/**
  Indicates the platform that the processor is intended for.
**/
#define MSR_PLATFORM_ID 0x00000017

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 32;

                            /* Bits[31:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 18;

                            /* Bits[49:32], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Platformid : 3;

                            /* Bits[52:50], Access Type=RWS, default=0x00000000*/

                            /*
                               The field gives information concerning the
                               intended platform for the processor.
                            */
    UINT32 Rsvd53 : 11;

                            /* Bits[63:53], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PLATFORM_ID_REGISTER;

/**
  This register holds the APIC base address, permitting the relocation of the APIC memory map
**/
#define MSR_APIC_BASE 0x0000001B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 8;

                            /* Bits[7:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Bsp : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Set if the processor is the Boot Strap
                               Processor.
                            */
    UINT32 Rsvd9 : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ExtendedModeEnable : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Enables or Disables APIC extensions. Enable = 1,
                               Disable = 0
                            */
    UINT32 Enable : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Enables or Disables the APIC. Enable = 1,
                               Disable = 0
                            */
    UINT32 BaseAddress0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x000FEE00*/

                            /*
                               The base address of the xAPIC memory map. Note:
                               bits above the supported physical address size
                               reported by CPUID are reserved.
                            */
    UINT32 BaseAddress1 : 20;

                            /* Bits[51:32], Access Type=RW, default=0x00000000*/

                            /*
                               The base address of the xAPIC memory map. Note:
                               bits above the supported physical address size
                               reported by CPUID are reserved.
                            */
    UINT32 Rsvd52 : 12;

                            /* Bits[63:52], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_APIC_BASE_REGISTER;

/**
  Control Features in Intel 64 Processor. see Appendix G.3.1
**/
#define MSR_OPTIN_FEATURE_CONTROL 0x0000003A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Lock : 1;

                            /* Bits[0:0], Access Type=RW, default=None*/

                            /*
                               When set, locks this MSR from being written,
                               writes to this bit will result in GP(0) until an
                               S5 reset occurs. BIOS must ensure this lock bit
                               is set before boot to OS. Lock bit for
                               protecting subsequent writes to this MSR. Once
                               set, any subsequent WRMSR attempt will #GP.
                            */
    UINT32 VmxEnableSmx : 1;

                            /* Bits[1:1], Access Type=RW, default=None*/

                            /*
                               Enables VMXON when the secure environment is
                               active.
                            */
    UINT32 VmxEnableNosmx : 1;

                            /* Bits[2:2], Access Type=RW, default=None*/

                            /*
                               Enables VMXON when no secure environment is
                               active.
                            */
    UINT32 Reserved3 : 5;

                            /* Bits[7:3], Access Type=RW, default=None*/

                            /* reserved */
    UINT32 SenterParameterControl : 7;

                            /* Bits[14:8], Access Type=RW, default=None*/

                            /*
                               Controls which corresponding parameter bits in
                               EDX[6:0] can be set with execution of SENTER.
                               The corresponding EDX bits are the opposite
                               polarity (disables) of the enables bits for this
                               field.
                            */
    UINT32 SenterEnable : 1;

                            /* Bits[15:15], Access Type=RW, default=None*/

                            /* Enables SENTER when set to 1. */
    UINT32 Reserved16 : 1;

                            /* Bits[16:16], Access Type=RW, default=None*/

                            /* reserved */
    UINT32 LeWr : 1;

                            /* Bits[17:17], Access Type=RW, default=None*/

                            /* reserved */
    UINT32 SeEnable : 1;

                            /* Bits[18:18], Access Type=RW, default=None*/

                            /* new SKL bit issue 1349784 - SE */
    UINT32 PcommitEnable : 1;

                            /* Bits[19:19], Access Type=RW, default=None*/

                            /* new CNL bit 2LM - PCOMMIT */
    UINT32 Lmce : 1;

                            /* Bits[20:20], Access Type=RW, default=None*/

                            /* Local MCE */
    UINT32 FzmEnable : 1;

                            /* Bits[21:21], Access Type=RW, default=None*/

                            /* new CNL bit 2LM - FZM */
    UINT32 Reserved22 : 10;

                            /* Bits[31:22], Access Type=RW, default=None*/

                            /* reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_OPTIN_FEATURE_CONTROL_REGISTER;
/**
  Thread specific offset used in RDTSC computation
**/
#define MSR_TSC_THREAD_OFFSET 0x0000003B

/**
  Virtual MSR to set/clear IBRS commands
**/
#define MSR_SPEC_CTRL 0x00000048

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Ibrs : 1;

                            /* Bits[0:0], Access Type=RW, default=None*/

                            /* IBRS mode active */
    UINT32 Stibp : 1;

                            /* Bits[1:1], Access Type=RW, default=None*/

                            /* STIBP mode active */
    UINT32 Ssbd : 1;

                            /* Bits[2:2], Access Type=RW, default=None*/

                            /* Memory Disambiguation Disable */
    UINT32 Rsvd3 : 29;

                            /* Bits[31:3], Access Type=RW, default=None*/

                            /* reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RW, default=None*/

                            /* reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SPEC_CTRL_REGISTER;

/**
  Write-only virtual MSR to activate BPU flush command
**/
#define MSR_PREDICT_CMD 0x00000049

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Ibpb : 1;

                            /* Bits[0:0], Access Type=RW, default=None*/

                            /* reserved */
    UINT32 Rsvd1 : 31;

                            /* Bits[31:1], Access Type=RW, default=None*/

                            /* reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RW, default=None*/

                            /* reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PREDICT_CMD_REGISTER;
/**
  This is a read only BIOS MSR to return the dynamic update(aka push patch) pointer stored in the PCH.
**/
#define MSR_BIOS_DYNAMIC_UPDATE_PTR 0x00000078
/**
  Executing a WRMSR instruction to this MSR causes a
  microcode update to be loaded into the processor. A processor may
  prevent writing to this MSR when loading guest states on VM entries
  or saving guest states on VM exits.
**/
#define MSR_BIOS_UPDT_TRIG 0x00000079

/**
  Implements Feature Activation command. WRMSR to this address
  activates all 'activatible' features on this thread.
  EDX:EAX = 0x1 (SE only at this point)
**/
#define MSR_BIOS_UPGD_TRIG 0x0000007A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Se : 1;

                            /* Bits[0:0], Access Type=RO_V, default=None*/

                            /* SE feature */
    UINT32 Rsvd1 : 31;

                            /* Bits[31:1], Access Type=RO_V, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO_V, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_BIOS_UPGD_TRIG_REGISTER;

/**
  FZM Range Index register to R/W Domain|Index
**/
#define MSR_FZM_RANGE_INDEX 0x00000082

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 DomainIndex : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               Holds the Index of domain or special encoding
                               CONFIG = 64
                            */
    UINT32 DomainHandle0 : 24;

                            /* Bits[31:8], Access Type=RW, default=0x00000000*/

                            /* Holds the Domain Handle */
    UINT32 DomainHandle1 : 32;

                            /* Bits[63:32], Access Type=RW, default=0x00000000*/

                            /* Holds the Domain Handle */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_FZM_RANGE_INDEX_REGISTER;
/**
  FZM Domain ConfigValue Read Only
**/
#define MSR_FZM_DOMAIN_CONFIG 0x00000083
/**
  FZM Domain Start Address - Ready Only
**/
#define MSR_FZM_RANGE_STARTADDR 0x00000084
/**
  FZM Domain End Address - Ready Only
**/
#define MSR_FZM_RANGE_ENDADDR 0x00000085

/**
  FZM Domain Write Status - Ready Only
**/
#define MSR_FZM_RANGE_WRITESTATUS 0x00000086

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 WriteStatus : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /*
                               Read the write status of the specified domain in
                               Index MSR
                            */
    UINT32 ReadStatus : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /*
                               Read the read status of the specified domain in
                               Index MSR
                            */
    UINT32 Rsvd2 : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_FZM_RANGE_WRITESTATUS_REGISTER;

/**
  Returns the microcode update signature following the execution of CPUID.01H.
**/
#define MSR_BIOS_SIGN_ID 0x0000008B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* RESERVED */
    UINT32 PatchId : 32;

                            /* Bits[63:32], Access Type=RW, default=0x00000000*/

                            /*
                               It is recommended that this field be pre-loaded
                               with 0 prior to executing CPUID. If the field
                               remains 0 following the execution of CPUID; this
                               indicates that no microcode update is loaded.
                               Any non-zero value is the microcode update
                               signature.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_BIOS_SIGN_ID_REGISTER;

/**
  Returns indication per thread whether PPPE load was successful or not (partial success)
**/
#define MSR_BIOS_DEBUG 0x000000A7

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 31;

                            /* Bits[30:0], Access Type=RW, default=0x00000000*/

                            /* RESERVED */
    UINT32 PatchRevIdStatusPppeLoadFail : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               indication whether PPPE load was successful or
                               not (partial success)
                            */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RW, default=0x00000000*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_BIOS_DEBUG_REGISTER;

/**
  GENERAL PERFORMANCE COUNTER
**/
#define MSR_EMON_GEN_CTR0 0x000000C1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Lower : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Lower Half of GENERAL PERFMON CNTR */
    UINT32 Upper : 16;

                            /* Bits[47:32], Access Type=RW, default=0x00000000*/

                            /* Upper Half of GENERAL PERFMON CNTR */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_GEN_CTR0_REGISTER;

/**
  GENERAL PERFORMANCE COUNTER
**/
#define MSR_EMON_GEN_CTR1 0x000000C2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Lower : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Lower Half of GENERAL PERFMON CNTR */
    UINT32 Upper : 16;

                            /* Bits[47:32], Access Type=RW, default=0x00000000*/

                            /* Upper Half of GENERAL PERFMON CNTR */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_GEN_CTR1_REGISTER;

/**
  GENERAL PERFORMANCE COUNTER
**/
#define MSR_EMON_GEN_CTR2 0x000000C3

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Lower : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Lower Half of GENERAL PERFMON CNTR */
    UINT32 Upper : 16;

                            /* Bits[47:32], Access Type=RW, default=0x00000000*/

                            /* Upper Half of GENERAL PERFMON CNTR */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_GEN_CTR2_REGISTER;

/**
  GENERAL PERFORMANCE COUNTER
**/
#define MSR_EMON_GEN_CTR3 0x000000C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Lower : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Lower Half of GENERAL PERFMON CNTR */
    UINT32 Upper : 16;

                            /* Bits[47:32], Access Type=RW, default=0x00000000*/

                            /* Upper Half of GENERAL PERFMON CNTR */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_GEN_CTR3_REGISTER;

/**
  GENERAL PERFORMANCE COUNTER
**/
#define MSR_EMON_GEN_CTR4 0x000000C5

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Lower : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Lower Half of GENERAL PERFMON CNTR */
    UINT32 Upper : 16;

                            /* Bits[47:32], Access Type=RW, default=0x00000000*/

                            /* Upper Half of GENERAL PERFMON CNTR */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_GEN_CTR4_REGISTER;

/**
  GENERAL PERFORMANCE COUNTER
**/
#define MSR_EMON_GEN_CTR5 0x000000C6

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Lower : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Lower Half of GENERAL PERFMON CNTR */
    UINT32 Upper : 16;

                            /* Bits[47:32], Access Type=RW, default=0x00000000*/

                            /* Upper Half of GENERAL PERFMON CNTR */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_GEN_CTR5_REGISTER;

/**
  GENERAL PERFORMANCE COUNTER
**/
#define MSR_EMON_GEN_CTR6 0x000000C7

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Lower : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Lower Half of GENERAL PERFMON CNTR */
    UINT32 Upper : 16;

                            /* Bits[47:32], Access Type=RW, default=0x00000000*/

                            /* Upper Half of GENERAL PERFMON CNTR */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_GEN_CTR6_REGISTER;

/**
  GENERAL PERFORMANCE COUNTER
**/
#define MSR_EMON_GEN_CTR7 0x000000C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Lower : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Lower Half of GENERAL PERFMON CNTR */
    UINT32 Upper : 16;

                            /* Bits[47:32], Access Type=RW, default=0x00000000*/

                            /* Upper Half of GENERAL PERFMON CNTR */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_GEN_CTR7_REGISTER;

/**
  architectural MSR to enumerate model-specific features
**/
#define MSR_CORE_CAPABILITIES 0x000000CF

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 StlbQosSupported : 1;

                            /* Bits[0:0], Access Type=RO_V, default=0x00000000*/

                            /*
                               Indicates that processor supports STLB QoS
                               functionality
                            */
    UINT32 RarSupported : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Indicates that processor supports Remote Action
                               Request functionality
                            */
    UINT32 FusaSupported : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Indicates that processor supports Functional
                               Safety
                            */
    UINT32 RsmInCpl0Only : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Indicates that RSM will only be allowed in CPL0
                               and #GP for all non-CPL0 privelege levels
                            */
    UINT32 VmdosSupported : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Indicates that VM-DOS feature is supported */
    UINT32 SplitLockDisableSupported : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Indicates that there is core support for #AC on
                               split lock detection feature
                            */
    UINT32 SnoopFilterQosSupported : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Indicates that snoop filter quality of service
                               MSRs are supported on this core. This is based
                               upon the existence of a non-inclusive cache and
                               the L2 / MLC QoS feature supported
                            */
    UINT32 Rsvd7 : 25;

                            /* Bits[31:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_CORE_CAPABILITIES_REGISTER;
/**
  Maximum Qualified Performance Clock Counter
**/
#define MSR_MLC_MCNT_COUNTER 0x000000E7
/**
  Actual Performance Clock Counter
**/
#define MSR_MLC_ACNT_COUNTER 0x000000E8

/**
  MTRR Capabilities MSR
**/
#define MSR_MTRRCAP 0x000000FE

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Vcnt : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Number of variable range registers */
    UINT32 Fix : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /* Fixed range registers supported if set */
    UINT32 Rsvd9 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 Wc : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Write-Combining memory type supported if set. */
    UINT32 Smrr : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* SMRRs are supported if set. */
    UINT32 Prmrr : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /* PRMRR MSRs are supported. */
    UINT32 Smrr2 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /* SMRR2 is supported if set. */
    UINT32 SmrrLock : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* SMRRs can be locked if set. */
    UINT32 Rsvd15 : 17;

                            /* Bits[31:15], Access Type=RW, default=0x00000000*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRCAP_REGISTER;

/**
  Write-only virtual MSR to activate DCU flush command
**/
#define MSR_DCU_FLUSH_CMD 0x0000010B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 DcuFlush : 1;

                            /* Bits[0:0], Access Type=RW, default=None*/

                            /* reserved */
    UINT32 Rsvd1 : 31;

                            /* Bits[31:1], Access Type=RW, default=None*/

                            /* reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RW, default=None*/

                            /* reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_DCU_FLUSH_CMD_REGISTER;
/**
  SYSENTER_CS_MSR
**/
#define MSR_SYSENTER_CS 0x00000174
/**
  SYSENTER ESP Linear Address
**/
#define MSR_SYSENTER_ESP 0x00000175
/**
  SYSENTER ESI Linear Address
**/
#define MSR_SYSENTER_EIP 0x00000176

/**
  Machine Check Capabilities register
**/
#define MSR_MCG_CAP 0x00000179

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Count : 8;

                            /* Bits[7:0], Access Type=RO_V, default=None*/

                            /*
                               Indicates the number of hardware unit error-
                               reporting banks available in a particular
                               processor implementation.
                            */
    UINT32 McgCtlP : 1;

                            /* Bits[8:8], Access Type=RO_V, default=None*/

                            /*
                               Indicates that the processor implements the
                               IA32_MCG_CTL MSR when set.
                            */
    UINT32 McgExtP : 1;

                            /* Bits[9:9], Access Type=RO_V, default=None*/

                            /*
                               Indicates that the processor implements the
                               extended machine-check state registers found
                               starting at MSR address 180H; these registers
                               are absent when clear.
                            */
    UINT32 McgExtErrorCntSig : 1;

                            /* Bits[10:10], Access Type=RO_V, default=None*/

                            /* Support for corrected MC error event is present. */
    UINT32 OvercapacitySig : 1;

                            /* Bits[11:11], Access Type=RO_V, default=None*/

                            /*
                               Threshold-based error status register are
                               present if this bit is set.
                            */
    UINT32 Rsvd12 : 4;

                            /* Bits[15:12], Access Type=RO_V, default=None*/

                            /* Reserved */
    UINT32 McgExtCnt : 8;

                            /* Bits[23:16], Access Type=RO_V, default=None*/

                            /*
                               Indicates the number of extended machinecheck
                               state registers present. This field is
                               meaningful only when the MCG_EXT_P flag is set.
                            */
    UINT32 Gcm : 1;

                            /* Bits[24:24], Access Type=RO_V, default=None*/

                            /*
                               The processor supports software error recovery
                               if this bit is set.
                            */
    UINT32 EnhancedMca : 1;

                            /* Bits[25:25], Access Type=RO_V, default=None*/

                            /* Set if Enhanced MCA is enabled */
    UINT32 ErrLog : 1;

                            /* Bits[26:26], Access Type=RO_V, default=None*/

                            /*
                               When set, EXTENDED_MCG_PTR MSR (0x793) is
                               available
                            */
    UINT32 Lmce : 1;

                            /* Bits[27:27], Access Type=RO_V, default=None*/

                            /* Set if Local Machine Check is Enabled */
    UINT32 Rsvd28 : 4;

                            /* Bits[31:28], Access Type=RO_V, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MCG_CAP_REGISTER;

/**
  Global Machine Check Status
**/
#define MSR_MCG_STATUS 0x0000017A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Ripv : 1;

                            /* Bits[0:0], Access Type=RW, default=None*/

                            /* Restart IP valid */
    UINT32 Eipv : 1;

                            /* Bits[1:1], Access Type=RW, default=None*/

                            /* Error IP valid */
    UINT32 Mcip : 1;

                            /* Bits[2:2], Access Type=RW, default=None*/

                            /* Machine Check In Progress. */
    UINT32 Lmce : 1;

                            /* Bits[3:3], Access Type=RW, default=None*/

                            /*
                               When this bit is set, it indicates to the OS
                               that the error logged was NOT broadcast
                               (localized). When this bit is clear, it
                               indicates to the OS that the error was broadcast
                               to all threads.
                            */
    UINT32 Rsvd4 : 28;

                            /* Bits[31:4], Access Type=RW, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MCG_STATUS_REGISTER;

/**
  This is a PerfMon register that is used to control the behavior
  of an associated performance counter and to selecting events for that counter.
**/
#define MSR_PERMON_EVNTSEL_CNTR0 0x00000186

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 EventSelect : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               Selects the event logic unit to detect certain
                               microarchitectural conditions
                            */
    UINT32 UnitMask : 8;

                            /* Bits[15:8], Access Type=RW, default=None*/

                            /*
                               Further qualifies the event logic unit selected
                               in the event select field to detect a specific
                               microarchitectural condition.
                            */
    UINT32 Usr : 1;

                            /* Bits[16:16], Access Type=RW, default=None*/

                            /*
                               This bit specifies that events are counted only
                               when the processor is operating at current
                               privilege levels 1, 2 or 3 (CPL != 0). This flag
                               can be used in conjunction with the OS flag.
                            */
    UINT32 Os : 1;

                            /* Bits[17:17], Access Type=RW, default=None*/

                            /*
                               Specifies that events are counted only when the
                               processor is operating at current privilege
                               level 0 (CPL=0). This flag can be used in
                               conjunction with the USR flag.
                            */
    UINT32 EdgeDetect : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Enables (when set) edge detection of events. The
                               processor counts the number of deasserted to
                               asserted transitions of any condition that can
                               be expressed by the other fields. The mechanism
                               is limited in that it does not permit back-to-
                               back assertions to be distinguished.
                            */
    UINT32 Rsvd19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Pmi : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               APIC interrupt enable. When set, the processor
                               generates an exception through its local APIC on
                               counter overflow for this counter's thread.
                            */
    UINT32 MythrAllthr : 1;

                            /* Bits[21:21], Access Type=RW, default=None*/

                            /*
                               When set to 1, it enables counting the
                               associated event conditions occurring across all
                               logical processors sharing a processor core.
                               When set to 0, the counter only increments the
                               associated event conditions occurring in the
                               logical processor which programmed the MSR.
                            */
    UINT32 EnableBit : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Enables the corresponding performance counter to
                               commence counting when this bit is set
                            */
    UINT32 Invert : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               This bit indicates how the CMASK/Threshold field
                               will be compared to the incoming event. When
                               '0', the comparison that will be done is:
                               threshold >= event. When set to '1', the
                               comparison that will be done is inverted from
                               the case where this bit is set to '0': threshold
                               less than event.
                            */
    UINT32 Threshold : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /*
                               When CMASK is not zero, the corresponding
                               performance counter increments each cycle if the
                               event count is greater than or equal to the
                               CMASK.
                            */
    UINT32 HleOnly : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to '1 restricts the counter to
                               only incrementing for the programmed event
                               during speculative and non-speculative HLE mode.
                               This bit will treated as reserved bit if HLE is
                               fuse disabled.
                            */
    UINT32 HleCheckpointEn : 1;

                            /* Bits[33:33], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to '1 enables the event
                               programmed into the counter to only non-aborted
                               HLE execution i.e. the event count will exclude
                               or not count events that occurred on an aborted
                               HLE path. Only counter 2 supports this
                               capability. This bit will treated as reserved
                               bit if HLE is fuse disabled.
                            */
    UINT32 AdaptiveRecord : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /*
                               for Adaptive pebs feature in SNC if
                               ADAPTIVE_RECORD bit is set, Ucode will consider
                               MSR_PEBS_DATA_CFG and emit the requested fields
                            */
    UINT32 Rsvd35 : 27;

                            /* Bits[61:35], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Forceubrkpt : 1;

                            /* Bits[62:62], Access Type=RW, default=None*/

                            /*
                               When set a MicroBreakPoint occurs each time a
                               none zero Event enters the counter.
                            */
    UINT32 Rsvd63 : 1;

                            /* Bits[63:63], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PERMON_EVNTSEL_CNTR0_REGISTER;

/**
  This is a PerfMon register that is used to control the behavior
  of an associated performance counter and to selecting events for that counter.
**/
#define MSR_PERMON_EVNTSEL_CNTR1 0x00000187

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 EventSelect : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               Selects the event logic unit to detect certain
                               microarchitectural conditions
                            */
    UINT32 UnitMask : 8;

                            /* Bits[15:8], Access Type=RW, default=None*/

                            /*
                               Further qualifies the event logic unit selected
                               in the event select field to detect a specific
                               microarchitectural condition.
                            */
    UINT32 Usr : 1;

                            /* Bits[16:16], Access Type=RW, default=None*/

                            /*
                               This bit specifies that events are counted only
                               when the processor is operating at current
                               privilege levels 1, 2 or 3 (CPL != 0). This flag
                               can be used in conjunction with the OS flag.
                            */
    UINT32 Os : 1;

                            /* Bits[17:17], Access Type=RW, default=None*/

                            /*
                               Specifies that events are counted only when the
                               processor is operating at current privilege
                               level 0 (CPL=0). This flag can be used in
                               conjunction with the USR flag.
                            */
    UINT32 EdgeDetect : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Enables (when set) edge detection of events. The
                               processor counts the number of deasserted to
                               asserted transitions of any condition that can
                               be expressed by the other fields. The mechanism
                               is limited in that it does not permit back-to-
                               back assertions to be distinguished.
                            */
    UINT32 Rsvd19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Pmi : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               APIC interrupt enable. When set, the processor
                               generates an exception through its local APIC on
                               counter overflow for this counter's thread.
                            */
    UINT32 MythrAllthr : 1;

                            /* Bits[21:21], Access Type=RW, default=None*/

                            /*
                               When set to 1, it enables counting the
                               associated event conditions occurring across all
                               logical processors sharing a processor core.
                               When set to 0, the counter only increments the
                               associated event conditions occurring in the
                               logical processor which programmed the MSR.
                            */
    UINT32 EnableBit : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Enables the corresponding performance counter to
                               commence counting when this bit is set
                            */
    UINT32 Invert : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               This bit indicates how the CMASK/Threshold field
                               will be compared to the incoming event. When
                               '0', the comparison that will be done is:
                               threshold >= event. When set to '1', the
                               comparison that will be done is inverted from
                               the case where this bit is set to '0': threshold
                               less than event.
                            */
    UINT32 Threshold : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /*
                               When CMASK is not zero, the corresponding
                               performance counter increments each cycle if the
                               event count is greater than or equal to the
                               CMASK.
                            */
    UINT32 HleOnly : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to '1 restricts the counter to
                               only incrementing for the programmed event
                               during speculative and non-speculative HLE mode.
                               This bit will treated as reserved bit if HLE is
                               fuse disabled.
                            */
    UINT32 HleCheckpointEn : 1;

                            /* Bits[33:33], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to '1 enables the event
                               programmed into the counter to only non-aborted
                               HLE execution i.e. the event count will exclude
                               or not count events that occurred on an aborted
                               HLE path. Only counter 2 supports this
                               capability. This bit will treated as reserved
                               bit if HLE is fuse disabled.
                            */
    UINT32 AdaptiveRecord : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /*
                               for Adaptive pebs feature in SNC if
                               ADAPTIVE_RECORD bit is set, Ucode will consider
                               MSR_PEBS_DATA_CFG and emit the requested fields
                            */
    UINT32 Rsvd35 : 27;

                            /* Bits[61:35], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Forceubrkpt : 1;

                            /* Bits[62:62], Access Type=RW, default=None*/

                            /*
                               When set a MicroBreakPoint occurs each time a
                               none zero Event enters the counter.
                            */
    UINT32 Rsvd63 : 1;

                            /* Bits[63:63], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PERMON_EVNTSEL_CNTR1_REGISTER;

/**
  This is a PerfMon register that is used to control the behavior
  of an associated performance counter and to selecting events for that counter.
**/
#define MSR_PERMON_EVNTSEL_CNTR2 0x00000188

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 EventSelect : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               Selects the event logic unit to detect certain
                               microarchitectural conditions
                            */
    UINT32 UnitMask : 8;

                            /* Bits[15:8], Access Type=RW, default=None*/

                            /*
                               Further qualifies the event logic unit selected
                               in the event select field to detect a specific
                               microarchitectural condition.
                            */
    UINT32 Usr : 1;

                            /* Bits[16:16], Access Type=RW, default=None*/

                            /*
                               This bit specifies that events are counted only
                               when the processor is operating at current
                               privilege levels 1, 2 or 3 (CPL != 0). This flag
                               can be used in conjunction with the OS flag.
                            */
    UINT32 Os : 1;

                            /* Bits[17:17], Access Type=RW, default=None*/

                            /*
                               Specifies that events are counted only when the
                               processor is operating at current privilege
                               level 0 (CPL=0). This flag can be used in
                               conjunction with the USR flag.
                            */
    UINT32 EdgeDetect : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Enables (when set) edge detection of events. The
                               processor counts the number of deasserted to
                               asserted transitions of any condition that can
                               be expressed by the other fields. The mechanism
                               is limited in that it does not permit back-to-
                               back assertions to be distinguished.
                            */
    UINT32 Rsvd19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Pmi : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               APIC interrupt enable. When set, the processor
                               generates an exception through its local APIC on
                               counter overflow for this counter's thread.
                            */
    UINT32 MythrAllthr : 1;

                            /* Bits[21:21], Access Type=RW, default=None*/

                            /*
                               When set to 1, it enables counting the
                               associated event conditions occurring across all
                               logical processors sharing a processor core.
                               When set to 0, the counter only increments the
                               associated event conditions occurring in the
                               logical processor which programmed the MSR.
                            */
    UINT32 EnableBit : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Enables the corresponding performance counter to
                               commence counting when this bit is set
                            */
    UINT32 Invert : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               This bit indicates how the CMASK/Threshold field
                               will be compared to the incoming event. When
                               '0', the comparison that will be done is:
                               threshold >= event. When set to '1', the
                               comparison that will be done is inverted from
                               the case where this bit is set to '0': threshold
                               less than event.
                            */
    UINT32 Threshold : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /*
                               When CMASK is not zero, the corresponding
                               performance counter increments each cycle if the
                               event count is greater than or equal to the
                               CMASK.
                            */
    UINT32 HleOnly : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to '1 restricts the counter to
                               only incrementing for the programmed event
                               during speculative and non-speculative HLE mode.
                               This bit will treated as reserved bit if HLE is
                               fuse disabled.
                            */
    UINT32 HleCheckpointEn : 1;

                            /* Bits[33:33], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to '1 enables the event
                               programmed into the counter to only non-aborted
                               HLE execution i.e. the event count will exclude
                               or not count events that occurred on an aborted
                               HLE path. Only counter 2 supports this
                               capability. This bit will treated as reserved
                               bit if HLE is fuse disabled.
                            */
    UINT32 AdaptiveRecord : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /*
                               for Adaptive pebs feature in SNC if
                               ADAPTIVE_RECORD bit is set, Ucode will consider
                               MSR_PEBS_DATA_CFG and emit the requested fields
                            */
    UINT32 Rsvd35 : 27;

                            /* Bits[61:35], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Forceubrkpt : 1;

                            /* Bits[62:62], Access Type=RW, default=None*/

                            /*
                               When set a MicroBreakPoint occurs each time a
                               none zero Event enters the counter.
                            */
    UINT32 Rsvd63 : 1;

                            /* Bits[63:63], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PERMON_EVNTSEL_CNTR2_REGISTER;

/**
  This is a PerfMon register that is used to control the behavior
  of an associated performance counter and to selecting events for that counter.
**/
#define MSR_PERMON_EVNTSEL_CNTR3 0x00000189

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 EventSelect : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               Selects the event logic unit to detect certain
                               microarchitectural conditions
                            */
    UINT32 UnitMask : 8;

                            /* Bits[15:8], Access Type=RW, default=None*/

                            /*
                               Further qualifies the event logic unit selected
                               in the event select field to detect a specific
                               microarchitectural condition.
                            */
    UINT32 Usr : 1;

                            /* Bits[16:16], Access Type=RW, default=None*/

                            /*
                               This bit specifies that events are counted only
                               when the processor is operating at current
                               privilege levels 1, 2 or 3 (CPL != 0). This flag
                               can be used in conjunction with the OS flag.
                            */
    UINT32 Os : 1;

                            /* Bits[17:17], Access Type=RW, default=None*/

                            /*
                               Specifies that events are counted only when the
                               processor is operating at current privilege
                               level 0 (CPL=0). This flag can be used in
                               conjunction with the USR flag.
                            */
    UINT32 EdgeDetect : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Enables (when set) edge detection of events. The
                               processor counts the number of deasserted to
                               asserted transitions of any condition that can
                               be expressed by the other fields. The mechanism
                               is limited in that it does not permit back-to-
                               back assertions to be distinguished.
                            */
    UINT32 Rsvd19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Pmi : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               APIC interrupt enable. When set, the processor
                               generates an exception through its local APIC on
                               counter overflow for this counter's thread.
                            */
    UINT32 MythrAllthr : 1;

                            /* Bits[21:21], Access Type=RW, default=None*/

                            /*
                               When set to 1, it enables counting the
                               associated event conditions occurring across all
                               logical processors sharing a processor core.
                               When set to 0, the counter only increments the
                               associated event conditions occurring in the
                               logical processor which programmed the MSR.
                            */
    UINT32 EnableBit : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Enables the corresponding performance counter to
                               commence counting when this bit is set
                            */
    UINT32 Invert : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               This bit indicates how the CMASK/Threshold field
                               will be compared to the incoming event. When
                               '0', the comparison that will be done is:
                               threshold >= event. When set to '1', the
                               comparison that will be done is inverted from
                               the case where this bit is set to '0': threshold
                               less than event.
                            */
    UINT32 Threshold : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /*
                               When CMASK is not zero, the corresponding
                               performance counter increments each cycle if the
                               event count is greater than or equal to the
                               CMASK.
                            */
    UINT32 HleOnly : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to '1 restricts the counter to
                               only incrementing for the programmed event
                               during speculative and non-speculative HLE mode.
                               This bit will treated as reserved bit if HLE is
                               fuse disabled.
                            */
    UINT32 HleCheckpointEn : 1;

                            /* Bits[33:33], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to '1 enables the event
                               programmed into the counter to only non-aborted
                               HLE execution i.e. the event count will exclude
                               or not count events that occurred on an aborted
                               HLE path. Only counter 2 supports this
                               capability. This bit will treated as reserved
                               bit if HLE is fuse disabled.
                            */
    UINT32 AdaptiveRecord : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /*
                               for Adaptive pebs feature in SNC if
                               ADAPTIVE_RECORD bit is set, Ucode will consider
                               MSR_PEBS_DATA_CFG and emit the requested fields
                            */
    UINT32 Rsvd35 : 27;

                            /* Bits[61:35], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Forceubrkpt : 1;

                            /* Bits[62:62], Access Type=RW, default=None*/

                            /*
                               When set a MicroBreakPoint occurs each time a
                               none zero Event enters the counter.
                            */
    UINT32 Rsvd63 : 1;

                            /* Bits[63:63], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PERMON_EVNTSEL_CNTR3_REGISTER;

/**
  This is a PerfMon register that is used to control the behavior
  of an associated performance counter and to selecting events for that counter.
**/
#define MSR_PERMON_EVNTSEL_CNTR4 0x0000018A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 EventSelect : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               Selects the event logic unit to detect certain
                               microarchitectural conditions
                            */
    UINT32 UnitMask : 8;

                            /* Bits[15:8], Access Type=RW, default=None*/

                            /*
                               Further qualifies the event logic unit selected
                               in the event select field to detect a specific
                               microarchitectural condition.
                            */
    UINT32 Usr : 1;

                            /* Bits[16:16], Access Type=RW, default=None*/

                            /*
                               This bit specifies that events are counted only
                               when the processor is operating at current
                               privilege levels 1, 2 or 3 (CPL != 0). This flag
                               can be used in conjunction with the OS flag.
                            */
    UINT32 Os : 1;

                            /* Bits[17:17], Access Type=RW, default=None*/

                            /*
                               Specifies that events are counted only when the
                               processor is operating at current privilege
                               level 0 (CPL=0). This flag can be used in
                               conjunction with the USR flag.
                            */
    UINT32 EdgeDetect : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Enables (when set) edge detection of events. The
                               processor counts the number of deasserted to
                               asserted transitions of any condition that can
                               be expressed by the other fields. The mechanism
                               is limited in that it does not permit back-to-
                               back assertions to be distinguished.
                            */
    UINT32 Rsvd19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Pmi : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               APIC interrupt enable. When set, the processor
                               generates an exception through its local APIC on
                               counter overflow for this counter's thread.
                            */
    UINT32 MythrAllthr : 1;

                            /* Bits[21:21], Access Type=RW, default=None*/

                            /*
                               When set to 1, it enables counting the
                               associated event conditions occurring across all
                               logical processors sharing a processor core.
                               When set to 0, the counter only increments the
                               associated event conditions occurring in the
                               logical processor which programmed the MSR.
                            */
    UINT32 EnableBit : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Enables the corresponding performance counter to
                               commence counting when this bit is set
                            */
    UINT32 Invert : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               This bit indicates how the CMASK/Threshold field
                               will be compared to the incoming event. When
                               '0', the comparison that will be done is:
                               threshold >= event. When set to '1', the
                               comparison that will be done is inverted from
                               the case where this bit is set to '0': threshold
                               less than event.
                            */
    UINT32 Threshold : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /*
                               When CMASK is not zero, the corresponding
                               performance counter increments each cycle if the
                               event count is greater than or equal to the
                               CMASK.
                            */
    UINT32 HleOnly : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to '1 restricts the counter to
                               only incrementing for the programmed event
                               during speculative and non-speculative HLE mode.
                               This bit will treated as reserved bit if HLE is
                               fuse disabled.
                            */
    UINT32 HleCheckpointEn : 1;

                            /* Bits[33:33], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to '1 enables the event
                               programmed into the counter to only non-aborted
                               HLE execution i.e. the event count will exclude
                               or not count events that occurred on an aborted
                               HLE path. Only counter 2 supports this
                               capability. This bit will treated as reserved
                               bit if HLE is fuse disabled.
                            */
    UINT32 AdaptiveRecord : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /*
                               for Adaptive pebs feature in SNC if
                               ADAPTIVE_RECORD bit is set, Ucode will consider
                               MSR_PEBS_DATA_CFG and emit the requested fields
                            */
    UINT32 Rsvd35 : 27;

                            /* Bits[61:35], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Forceubrkpt : 1;

                            /* Bits[62:62], Access Type=RW, default=None*/

                            /*
                               When set a MicroBreakPoint occurs each time a
                               none zero Event enters the counter.
                            */
    UINT32 Rsvd63 : 1;

                            /* Bits[63:63], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PERMON_EVNTSEL_CNTR4_REGISTER;

/**
  This is a PerfMon register that is used to control the behavior
  of an associated performance counter and to selecting events for that counter.
**/
#define MSR_PERMON_EVNTSEL_CNTR5 0x0000018B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 EventSelect : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               Selects the event logic unit to detect certain
                               microarchitectural conditions
                            */
    UINT32 UnitMask : 8;

                            /* Bits[15:8], Access Type=RW, default=None*/

                            /*
                               Further qualifies the event logic unit selected
                               in the event select field to detect a specific
                               microarchitectural condition.
                            */
    UINT32 Usr : 1;

                            /* Bits[16:16], Access Type=RW, default=None*/

                            /*
                               This bit specifies that events are counted only
                               when the processor is operating at current
                               privilege levels 1, 2 or 3 (CPL != 0). This flag
                               can be used in conjunction with the OS flag.
                            */
    UINT32 Os : 1;

                            /* Bits[17:17], Access Type=RW, default=None*/

                            /*
                               Specifies that events are counted only when the
                               processor is operating at current privilege
                               level 0 (CPL=0). This flag can be used in
                               conjunction with the USR flag.
                            */
    UINT32 EdgeDetect : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Enables (when set) edge detection of events. The
                               processor counts the number of deasserted to
                               asserted transitions of any condition that can
                               be expressed by the other fields. The mechanism
                               is limited in that it does not permit back-to-
                               back assertions to be distinguished.
                            */
    UINT32 Rsvd19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Pmi : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               APIC interrupt enable. When set, the processor
                               generates an exception through its local APIC on
                               counter overflow for this counter's thread.
                            */
    UINT32 MythrAllthr : 1;

                            /* Bits[21:21], Access Type=RW, default=None*/

                            /*
                               When set to 1, it enables counting the
                               associated event conditions occurring across all
                               logical processors sharing a processor core.
                               When set to 0, the counter only increments the
                               associated event conditions occurring in the
                               logical processor which programmed the MSR.
                            */
    UINT32 EnableBit : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Enables the corresponding performance counter to
                               commence counting when this bit is set
                            */
    UINT32 Invert : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               This bit indicates how the CMASK/Threshold field
                               will be compared to the incoming event. When
                               '0', the comparison that will be done is:
                               threshold >= event. When set to '1', the
                               comparison that will be done is inverted from
                               the case where this bit is set to '0': threshold
                               less than event.
                            */
    UINT32 Threshold : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /*
                               When CMASK is not zero, the corresponding
                               performance counter increments each cycle if the
                               event count is greater than or equal to the
                               CMASK.
                            */
    UINT32 HleOnly : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to '1 restricts the counter to
                               only incrementing for the programmed event
                               during speculative and non-speculative HLE mode.
                               This bit will treated as reserved bit if HLE is
                               fuse disabled.
                            */
    UINT32 HleCheckpointEn : 1;

                            /* Bits[33:33], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to '1 enables the event
                               programmed into the counter to only non-aborted
                               HLE execution i.e. the event count will exclude
                               or not count events that occurred on an aborted
                               HLE path. Only counter 2 supports this
                               capability. This bit will treated as reserved
                               bit if HLE is fuse disabled.
                            */
    UINT32 AdaptiveRecord : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /*
                               for Adaptive pebs feature in SNC if
                               ADAPTIVE_RECORD bit is set, Ucode will consider
                               MSR_PEBS_DATA_CFG and emit the requested fields
                            */
    UINT32 Rsvd35 : 27;

                            /* Bits[61:35], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Forceubrkpt : 1;

                            /* Bits[62:62], Access Type=RW, default=None*/

                            /*
                               When set a MicroBreakPoint occurs each time a
                               none zero Event enters the counter.
                            */
    UINT32 Rsvd63 : 1;

                            /* Bits[63:63], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PERMON_EVNTSEL_CNTR5_REGISTER;

/**
  This is a PerfMon register that is used to control the behavior
  of an associated performance counter and to selecting events for that counter.
**/
#define MSR_PERMON_EVNTSEL_CNTR6 0x0000018C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 EventSelect : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               Selects the event logic unit to detect certain
                               microarchitectural conditions
                            */
    UINT32 UnitMask : 8;

                            /* Bits[15:8], Access Type=RW, default=None*/

                            /*
                               Further qualifies the event logic unit selected
                               in the event select field to detect a specific
                               microarchitectural condition.
                            */
    UINT32 Usr : 1;

                            /* Bits[16:16], Access Type=RW, default=None*/

                            /*
                               This bit specifies that events are counted only
                               when the processor is operating at current
                               privilege levels 1, 2 or 3 (CPL != 0). This flag
                               can be used in conjunction with the OS flag.
                            */
    UINT32 Os : 1;

                            /* Bits[17:17], Access Type=RW, default=None*/

                            /*
                               Specifies that events are counted only when the
                               processor is operating at current privilege
                               level 0 (CPL=0). This flag can be used in
                               conjunction with the USR flag.
                            */
    UINT32 EdgeDetect : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Enables (when set) edge detection of events. The
                               processor counts the number of deasserted to
                               asserted transitions of any condition that can
                               be expressed by the other fields. The mechanism
                               is limited in that it does not permit back-to-
                               back assertions to be distinguished.
                            */
    UINT32 Rsvd19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Pmi : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               APIC interrupt enable. When set, the processor
                               generates an exception through its local APIC on
                               counter overflow for this counter's thread.
                            */
    UINT32 MythrAllthr : 1;

                            /* Bits[21:21], Access Type=RW, default=None*/

                            /*
                               When set to 1, it enables counting the
                               associated event conditions occurring across all
                               logical processors sharing a processor core.
                               When set to 0, the counter only increments the
                               associated event conditions occurring in the
                               logical processor which programmed the MSR.
                            */
    UINT32 EnableBit : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Enables the corresponding performance counter to
                               commence counting when this bit is set
                            */
    UINT32 Invert : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               This bit indicates how the CMASK/Threshold field
                               will be compared to the incoming event. When
                               '0', the comparison that will be done is:
                               threshold >= event. When set to '1', the
                               comparison that will be done is inverted from
                               the case where this bit is set to '0': threshold
                               less than event.
                            */
    UINT32 Threshold : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /*
                               When CMASK is not zero, the corresponding
                               performance counter increments each cycle if the
                               event count is greater than or equal to the
                               CMASK.
                            */
    UINT32 HleOnly : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to '1 restricts the counter to
                               only incrementing for the programmed event
                               during speculative and non-speculative HLE mode.
                               This bit will treated as reserved bit if HLE is
                               fuse disabled.
                            */
    UINT32 HleCheckpointEn : 1;

                            /* Bits[33:33], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to '1 enables the event
                               programmed into the counter to only non-aborted
                               HLE execution i.e. the event count will exclude
                               or not count events that occurred on an aborted
                               HLE path. Only counter 2 supports this
                               capability. This bit will treated as reserved
                               bit if HLE is fuse disabled.
                            */
    UINT32 AdaptiveRecord : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /*
                               for Adaptive pebs feature in SNC if
                               ADAPTIVE_RECORD bit is set, Ucode will consider
                               MSR_PEBS_DATA_CFG and emit the requested fields
                            */
    UINT32 Rsvd35 : 27;

                            /* Bits[61:35], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Forceubrkpt : 1;

                            /* Bits[62:62], Access Type=RW, default=None*/

                            /*
                               When set a MicroBreakPoint occurs each time a
                               none zero Event enters the counter.
                            */
    UINT32 Rsvd63 : 1;

                            /* Bits[63:63], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PERMON_EVNTSEL_CNTR6_REGISTER;

/**
  This is a PerfMon register that is used to control the behavior
  of an associated performance counter and to selecting events for that counter.
**/
#define MSR_PERMON_EVNTSEL_CNTR7 0x0000018D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 EventSelect : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               Selects the event logic unit to detect certain
                               microarchitectural conditions
                            */
    UINT32 UnitMask : 8;

                            /* Bits[15:8], Access Type=RW, default=None*/

                            /*
                               Further qualifies the event logic unit selected
                               in the event select field to detect a specific
                               microarchitectural condition.
                            */
    UINT32 Usr : 1;

                            /* Bits[16:16], Access Type=RW, default=None*/

                            /*
                               This bit specifies that events are counted only
                               when the processor is operating at current
                               privilege levels 1, 2 or 3 (CPL != 0). This flag
                               can be used in conjunction with the OS flag.
                            */
    UINT32 Os : 1;

                            /* Bits[17:17], Access Type=RW, default=None*/

                            /*
                               Specifies that events are counted only when the
                               processor is operating at current privilege
                               level 0 (CPL=0). This flag can be used in
                               conjunction with the USR flag.
                            */
    UINT32 EdgeDetect : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Enables (when set) edge detection of events. The
                               processor counts the number of deasserted to
                               asserted transitions of any condition that can
                               be expressed by the other fields. The mechanism
                               is limited in that it does not permit back-to-
                               back assertions to be distinguished.
                            */
    UINT32 Rsvd19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Pmi : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               APIC interrupt enable. When set, the processor
                               generates an exception through its local APIC on
                               counter overflow for this counter's thread.
                            */
    UINT32 MythrAllthr : 1;

                            /* Bits[21:21], Access Type=RW, default=None*/

                            /*
                               When set to 1, it enables counting the
                               associated event conditions occurring across all
                               logical processors sharing a processor core.
                               When set to 0, the counter only increments the
                               associated event conditions occurring in the
                               logical processor which programmed the MSR.
                            */
    UINT32 EnableBit : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Enables the corresponding performance counter to
                               commence counting when this bit is set
                            */
    UINT32 Invert : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               This bit indicates how the CMASK/Threshold field
                               will be compared to the incoming event. When
                               '0', the comparison that will be done is:
                               threshold >= event. When set to '1', the
                               comparison that will be done is inverted from
                               the case where this bit is set to '0': threshold
                               less than event.
                            */
    UINT32 Threshold : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /*
                               When CMASK is not zero, the corresponding
                               performance counter increments each cycle if the
                               event count is greater than or equal to the
                               CMASK.
                            */
    UINT32 HleOnly : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to '1 restricts the counter to
                               only incrementing for the programmed event
                               during speculative and non-speculative HLE mode.
                               This bit will treated as reserved bit if HLE is
                               fuse disabled.
                            */
    UINT32 HleCheckpointEn : 1;

                            /* Bits[33:33], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to '1 enables the event
                               programmed into the counter to only non-aborted
                               HLE execution i.e. the event count will exclude
                               or not count events that occurred on an aborted
                               HLE path. Only counter 2 supports this
                               capability. This bit will treated as reserved
                               bit if HLE is fuse disabled.
                            */
    UINT32 AdaptiveRecord : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /*
                               for Adaptive pebs feature in SNC if
                               ADAPTIVE_RECORD bit is set, Ucode will consider
                               MSR_PEBS_DATA_CFG and emit the requested fields
                            */
    UINT32 Rsvd35 : 27;

                            /* Bits[61:35], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Forceubrkpt : 1;

                            /* Bits[62:62], Access Type=RW, default=None*/

                            /*
                               When set a MicroBreakPoint occurs each time a
                               none zero Event enters the counter.
                            */
    UINT32 Rsvd63 : 1;

                            /* Bits[63:63], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PERMON_EVNTSEL_CNTR7_REGISTER;

/**
  Current performance status
**/
#define MSR_PERF_STATUS 0x00000198

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 8;

                            /* Bits[7:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Ratio : 7;

                            /* Bits[14:8], Access Type=RO_V, default=0x00000000*/

                            /* Current core P-state ratio value 14:8 */
    UINT32 Rsvd15 : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Voltage : 16;

                            /* Bits[47:32], Access Type=RO_V, default=0x00000000*/

                            /*
                               Indicates the current operating voltage in 3.13
                               format i.e. current voltage = float([47:32] /
                               (2^13)).
                            */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PERF_STATUS_REGISTER;

/**
  Legacy Performance Control MSR. SW requests a new Performance state (P-State) by writing to this MSR.
  [IntelRsvd]ARCH MSR in core - P-state control register reflected into P-STATE_REQ[/IntelRsvd]
**/
#define MSR_PERF_CTL 0x00000199

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 VReq : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Voltage request - Not used = legacy */
    UINT32 PReq : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000000*/

                            /*
                               Requested P-state ratio value for this logical
                               processor.
                            */
    UINT32 Rsvd15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 Pqosofst : 6;

                            /* Bits[21:16], Access Type=RW, default=0x00000000*/

                            /* P/H P-State QoS offset */
    UINT32 Rsvd22 : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 TurboDis : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /*
                               1 = Turbo Mode disabled, 0=Turbo Mode enabled.
                               Note: This Turbo Mode Disable exists on each
                               logical processor; however, setting the Turbo
                               Mode Disable on any logical processor forces
                               Turbo Mode to be disabled for all logical
                               processors in the entire processor package. BIOS
                               should not use this Turbo Mode Disable since it
                               does not change the Turbo Mode feature flag.
                               This Turbo Mode Disable is intended for OS use
                               only.
                            */
    UINT32 Rsvd33 : 31;

                            /* Bits[63:33], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PERF_CTL_REGISTER;

/**
  This register is used to read and write the duty cycle bits, and to enable the on-demand
  clock modulation feature of the Intel Thermal Monitor.
**/
#define MSR_CLOCK_MODULATION 0x0000019A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 TStateReq : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               Selects the on-demand clock modulation duty
                               cycle. This field is only active when the on-
                               demand clock modulation enable flag is set.
                            */
    UINT32 ThrottleActive : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Enables on-demand software controlled clock
                               modulation when set; disables software-
                               controlled clock modulation when clear
                            */
    UINT32 Rsvd5 : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_CLOCK_MODULATION_REGISTER;

/**
  Enables and disables the generation of an interrupt on temperature transitions detected with the processor's thermal sensors and thermal monitor.
**/
#define MSR_THERM_INTERRUPT 0x0000019B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 HighTempIntEnable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Enables a thermal interrupt to be generated on
                               the transition from a low-temperature to a high-
                               temperature when set, where 'high temperature'
                               is dictated by the thermal monitor trip
                               temperature.
                            */
    UINT32 LowTempIntEnable : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Enables a thermal interrupt to be generated on
                               the transition from a high-temperature to a low-
                               temperature when set, where 'high temperature'
                               is dictated by the thermal monitor trip
                               temperature.
                            */
    UINT32 ProchotIntEnable : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Bidirectional PROCHOT# assertion interrupt
                               enable. If set, a thermal interrupt is delivered
                               on the rising edge of xxPROCHOT#.
                            */
    UINT32 Rsvd3 : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 OutOfSpecIntEnable : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Thermal interrupt enable for the critical
                               temperature condition which is stored in the
                               Critical Temperature Status bit in
                               IA32_THERM_STATUS.
                            */
    UINT32 Rsvd5 : 3;

                            /* Bits[7:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Threshold1RelTemp : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000000*/

                            /*
                               This value indicates the offset in degrees below
                               TJ Max Temperature that should trigger a Thermal
                               Threshold 1 trip.
                            */
    UINT32 Threshold1IntEnable : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Controls the generation of a thermal interrupt
                               whenever the Thermal Threshold 1 Temperature is
                               crossed.
                            */
    UINT32 Threshold2RelTemp : 7;

                            /* Bits[22:16], Access Type=RW, default=0x00000000*/

                            /*
                               This value indicates the offset in degrees below
                               TJ Max Temperature that should trigger a Thermal
                               Threshold 2 trip.
                            */
    UINT32 Threshold2IntEnable : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Controls the generation of a thermal interrupt
                               whenever the Thermal Threshold 2 Temperature is
                               crossed.
                            */
    UINT32 PowerIntEnable : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               When this bit is set, a thermal interrupt will
                               be sent upon throttling due to power
                               limitations.
                            */
    UINT32 Rsvd25 : 7;

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_THERM_INTERRUPT_REGISTER;

/**
  Contains status information about the processor's thermal sensor and automatic thermal monitoring facilities.
**/
#define MSR_THERM_STATUS 0x0000019C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ThermalMonitorStatus : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Status bit indicating that the Thermal Monitor
                               has tripped and is currently thermally
                               throttling.
                            */
    UINT32 ThermalMonitorLog : 1;

                            /* Bits[1:1], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Sticky log bit indicating that the core has seen
                               a thermal monitor event since the last time SW
                               cleared this bit. This bit is set by HW on a 0
                               to 1 transition of THERMAL_MONITOR_STATUS.
                            */
    UINT32 ProchotStatus : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Status bit indicating that xxPROCHOT# is
                               currently being asserted.
                            */
    UINT32 ProchotLog : 1;

                            /* Bits[3:3], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Sticky log bit indicating that xxPROCHOT# has
                               been asserted since the last time this bit was
                               cleared by SW. This bit is set by HW on a 0 to 1
                               transition of PROCHOT_STATUS.
                            */
    UINT32 OutOfSpecStatus : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Status bit indicating that the processor is
                               operating out of its thermal specification. Once
                               set, this bit should only clear on a reset.
                            */
    UINT32 OutOfSpecLog : 1;

                            /* Bits[5:5], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Sticky log bit indicating that the processor
                               operating out of its thermal specification since
                               the last time this bit was cleared. This bit is
                               set by HW on a 0 to 1 transition of
                               OUT_OF_SPEC_STATUS.
                            */
    UINT32 Threshold1Status : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Indicates that the current temperature is higher
                               than or equal to Threshold 1 temperature.
                            */
    UINT32 Threshold1Log : 1;

                            /* Bits[7:7], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Sticky log bit that asserts on a 0 to 1 or a 1
                               to 0 transition of the THRESHOLD1_STATUS bit.
                               This bit is set by HW and cleared by SW.
                            */
    UINT32 Threshold2Status : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Indicates that the current temperature is higher
                               than or equal to Threshold 2 temperature.
                            */
    UINT32 Threshold2Log : 1;

                            /* Bits[9:9], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Sticky log bit that asserts on a 0 to 1 or a 1
                               to 0 transition of the THRESHOLD2_STATUS bit.
                               This bit is set by HW and cleared by SW.
                            */
    UINT32 PowerLimitationStatus : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               RO - Indicates whether the current P-state is
                               limited by power limitation. For legacy P state
                               method, this bit will be set only if the P-state
                               is limit below the guaranty level
                            */
    UINT32 PowerLimitationLog : 1;

                            /* Bits[11:11], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               R/WC0 - Sticky bit which indicates whether the
                               current P-state is limited by power limitation
                               since the last clearing of this bit or a reset.
                               SW may clear this bit by writing a zero (0). For
                               legacy P state method, this bit will be set only
                               if the P-state is limit below the guaranty level
                            */
    UINT32 CurrentLimitStatus : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               RO - If set (1), indicates an electrical current
                               limit (e.g. Electrical Design Point/IccMax) is
                               being exceeded and is adversely impacting energy
                               efficiency optimizations
                            */
    UINT32 CurrentLimitLog : 1;

                            /* Bits[13:13], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               R/WC0 - If set (1), an electrical current limit
                               has been exceeded that has adversely impacted
                               energy efficiency optimizations since the last
                               clearing of this bit or a reset. This bit is
                               sticky, software may clear this bit by writing a
                               zero (0).
                            */
    UINT32 CrossDomainLimitStatus : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               RO - If set (1), indicates another hardware
                               domain (e.g. processor graphics) is currently
                               limiting energy efficiency optimizations in the
                               processor core domain.
                            */
    UINT32 CrossDomainLimitLog : 1;

                            /* Bits[15:15], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               R/WC0 - If set (1), indicates another hardware
                               domain (e.g. processor graphics) has limited
                               energy efficiency optimizations in the processor
                               core domain since the last clearing of this bit
                               or a reset. This bit is sticky, software may
                               clear this bit by writing a zero (0).
                            */
    UINT32 Temperature : 7;

                            /* Bits[22:16], Access Type=RO, default=0x00000000*/

                            /*
                               This is a temperature offset in degrees C below
                               theTJ Max temperature. This number is meaningful
                               only if VALID bit in this register is set.
                            */
    UINT32 Rsvd23 : 4;

                            /* Bits[26:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Resolution : 4;

                            /* Bits[30:27], Access Type=RO, default=0x00000001*/

                            /* Supported resolution in degrees C. */
    UINT32 Valid : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               This bit indicates that the TEMPERATURE field is
                               valid. It is set by PCODE if the temperature is
                               within valid thermal sensor range.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_THERM_STATUS_REGISTER;

/**
  Allows a variety of processor functions to be enabled and disabled.
**/
#define MSR_MISC_ENABLES 0x000001A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 FastStrings : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               When 1, enables fast strings for MOVS and STOS
                               with the threshold values defined in
                               MOVS_THRESHOLD and STOS_THRESHOLD. when 0, uses
                               compatible strings.
                            */
    UINT32 Rsvd1 : 1;

                            /* Bits[1:1], Access Type=RW, default=None*/

                            /* Reserved */
    UINT32 Rsvd2 : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ThermalMonitorEnable : 1;

                            /* Bits[3:3], Access Type=RW, default=None*/

                            /*
                               Automatic Thermal Control Circuit Enable. 1 =
                               Setting this bit enables the thermal control
                               circuit (TCC) portion of the Intel Thermal
                               Monitor feature. This allows the processor to
                               automatically reduce power consumption in
                               response to TCC activation. 0 = Disabled
                               (default). Note: In some products clearing this
                               bit might be ignored in critical thermal
                               conditions, and TM1, TM2 and adaptive thermal
                               throttling will still be activated.
                            */
    UINT32 Rsvd4 : 3;

                            /* Bits[6:4], Access Type=RW, default=None*/

                            /* Reserved */
    UINT32 EmonAvailable : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               1 = Performance monitoring enabled 0 =
                               Performance monitoring disabled
                            */
    UINT32 Rsvd8 : 3;

                            /* Bits[10:8], Access Type=RW, default=None*/

                            /* Reserved */
    UINT32 BtsUnavailable : 1;

                            /* Bits[11:11], Access Type=RW, default=None*/

                            /*
                               1 = Processor doesn't support branch trace
                               storage (BTS) 0 = BTS is supported
                            */
    UINT32 PebsUnavailable : 1;

                            /* Bits[12:12], Access Type=RW, default=None*/

                            /*
                               1 = Processor doesn't support PEBS 0 = PEBS is
                               supported
                            */
    UINT32 Rsvd13 : 3;

                            /* Bits[15:13], Access Type=RW, default=None*/

                            /* Reserved */
    UINT32 EnableGv3 : 1;

                            /* Bits[16:16], Access Type=RW, default=None*/

                            /*
                               1 = Enhanced Intel SpeedStep Technology enabled
                               0 = Enhanced Intel SpeedStep Technology disabled
                            */
    UINT32 RsvdBit17 : 1;

                            /* Bits[17:17], Access Type=RW, default=None*/

                            /* Reserved */
    UINT32 EnableMonitorFsm : 1;

                            /* Bits[18:18], Access Type=RW, default=None*/

                            /*
                               Enables the Monitor/Mwait FSM. This is a thread-
                               specific bit
                            */
    UINT32 Rsvd19 : 3;

                            /* Bits[21:19], Access Type=RW, default=None*/

                            /* Reserved */
    UINT32 BootNt4 : 1;

                            /* Bits[22:22], Access Type=RW, default=None*/

                            /*
                               Limit CPUID Maxval. When this bit is set to 1,
                               CPUID.00H returns a maximum value in EAX[7:0] of
                               3. BIOS should contain a setup question that
                               allows users to specify when the installed OS
                               does not support CPUID functions greater than 3.
                               Before setting this bit, BIOS must execute the
                               CPUID.0H and examine the maximum value returned
                               in EAX[7:0]. If the maximum value is greater
                               than 3, the bit is supported. Otherwise, the bit
                               is not supported. Writing to this bit when the
                               maximum value is greater than 3 may generate a
                               #GP exception. Setting this bit may cause
                               unexpected behavior in software that depends on
                               the availability of CPUID leaves greater than 3.
                            */
    UINT32 TprMessageDisable : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               When set, disables update of periodic TPR
                               updates to the I/O apic. Default = 1 (disabled).
                               This is a thread-specific bit
                            */
    UINT32 Rsvd24 : 3;

                            /* Bits[26:24], Access Type=RW, default=None*/

                            /* Reserved */
    UINT32 Rsvd27 : 1;

                            /* Bits[27:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 HlepDisable : 1;

                            /* Bits[28:28], Access Type=RW, default=None*/

                            /* When set, disables HLE+. */
    UINT32 Rsvd29 : 3;

                            /* Bits[31:29], Access Type=RW, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 6;

                            /* Bits[37:32], Access Type=RW, default=None*/

                            /* Reserved */
    UINT32 TurboModeDisable : 1;

                            /* Bits[38:38], Access Type=RW, default=None*/

                            /* Disables turbo mode when set to 1. */
    UINT32 Rsvd39 : 25;

                            /* Bits[63:39], Access Type=RW, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MISC_ENABLES_REGISTER;

/**
  Performance Energy Bias Hint MSR. This register allows software to control the Energy efficiency Policy in the CPU.
**/
#define MSR_ENERGY_PERFORMANCE_BIAS 0x000001B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 EnergyPolicy : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               The energy efficiency policy chosen by Operating
                               System software. This bit is only writable when
                               POWER_CTL[18] == 1. 0 indicates preference to
                               highest performance. 15 indicates preference to
                               maximize energy saving.
                            */
    UINT32 Rsvd4 : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_ENERGY_PERFORMANCE_BIAS_REGISTER;

/**
  Contains status information about the package's thermal sensor.
**/
#define MSR_PACKAGE_THERM_STATUS 0x000001B1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ThermalMonitorStatus : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Status bit indicating that any of the package
                               thermal monitor have tripped and the package is
                               currently thermally throttling.
                            */
    UINT32 ThermalMonitorLog : 1;

                            /* Bits[1:1], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Sticky log bit indicating that the package has
                               seen a thermal monitor event since the last time
                               SW cleared this bit. Set by HW on a 0 to 1
                               transition of Thermal_Monitor_Status.
                            */
    UINT32 ProchotStatus : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Status bit indicating that xxPROCHOT# is
                               currently being asserted.
                            */
    UINT32 ProchotLog : 1;

                            /* Bits[3:3], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Sticky log bit indicating that xxPROCHOT# has
                               been asserted since the last time this bit was
                               cleared by SW. Set by HW on a 0 to 1 transition
                               of Prochot_Status.
                            */
    UINT32 OutOfSpecStatus : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Status bit indicating that the processor is
                               operating out of its thermal specification.
                            */
    UINT32 OutOfSpecLog : 1;

                            /* Bits[5:5], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Sticky log bit indicating that the processor
                               operating out of its thermal specification since
                               the last time this bit was cleared. Set by HW on
                               a 0 to 1 transition of Out_of_Spec_Status
                            */
    UINT32 Threshold1Status : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Indicates that the current temperature (bits
                               22:16 in this register) is higher than or equal
                               to the Threshold1 defined in the
                               IA32_PACKAGE_THERM_INTERRUPT MSR. Note that
                               because temperature and thresholds are defined
                               as negative offsets, a higher number means a
                               lower temperature.
                            */
    UINT32 Threshold1Log : 1;

                            /* Bits[7:7], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Sticky log bit that asserts on a 0 to 1 or 1 to
                               0 transition of the Threshold1_Status bit. HW
                               controls this transition.
                            */
    UINT32 Threshold2Status : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Indicates that the current temperature (bits
                               22:16 of this register) is higher than or equal
                               to the Threshold2 defined in the
                               IA32_PACKAGE_THERM_INTERRUPT MSR. Note that
                               because temperature and thresholds are defined
                               as negative offsets, a higher number means a
                               lower temperature.
                            */
    UINT32 Threshold2Log : 1;

                            /* Bits[9:9], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Sticky log bit that asserts on a 0 to 1 or 1 to
                               0 transition of the Threshold2_Status bit. HW
                               controls this transition.
                            */
    UINT32 PowerLimitationStatus : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Status log bit that notifies that either IA is
                               running at P-state below the (max P-state -
                               offset) or that GT is running at P-state below
                               its P1 frequency.
                            */
    UINT32 PowerLimitationLog : 1;

                            /* Bits[11:11], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Sticky log bit that asserts when either IA is
                               running at P-state below the (max P-state -
                               offset) or that Graphics is running at P-state
                               below its P1 frequency. Set by HW on a 0 to 1
                               transition of Power Limitation Status. Cleared
                               by SW.
                            */
    UINT32 PmaxStatus : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Status bit for the PMAX detector. An assertion
                               means the PMAX detector circuit has asserted (up
                               to a 1ms delay). The status bit is deasserted
                               when the core throttle is released to indicate
                               the PMAX condition has been dealt with
                            */
    UINT32 PmaxLog : 1;

                            /* Bits[13:13], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Sticky log bit indicating the PMAX detector has
                               been asserted since the last time this bit was
                               cleared by SW. Set by HW on a 0 to 1 transition
                               of PMAX_STATUS
                            */
    UINT32 Rsvd14 : 2;

                            /* Bits[15:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Temperature : 7;

                            /* Bits[22:16], Access Type=RO, default=0x00000000*/

                            /*
                               Temperature in degrees C, relative to the
                               thermal monitor trip temperature.
                            */
    UINT32 Rsvd23 : 3;

                            /* Bits[25:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 HwFeedbackNotificationLog : 1;

                            /* Bits[26:26], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Sticky log bit that indicates whether a HW
                               FEEDBACK Change has occurred (e.g. due to
                               power/thermal limits) since the last clearing of
                               this bit or reset. SW may clear this bit by
                               writing a zero (0).
                            */
    UINT32 Resolution : 4;

                            /* Bits[30:27], Access Type=RO, default=0x00000000*/

                            /* Supported resolution in degrees C. */
    UINT32 Valid : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               Set if temperature is within the valid thermal
                               sensor range.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PACKAGE_THERM_STATUS_REGISTER;

/**
  Thermal Limitation Interrupt Control.
**/
#define MSR_PACKAGE_THERM_INTERRUPT 0x000001B2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 HighTempIntEnable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Enables a thermal interrupt to be generated on
                               the transition from a low-temperature to a high-
                               temperature when set, where 'high temperature'
                               is dicated by the thermal monitor trip
                               temperature minus offset as defined in
                               IA32_TEMPERATURE_TARGET.
                            */
    UINT32 LowTempIntEnable : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Enables a thermal interrupt to be generated on
                               the transition from a high-temperature to a low-
                               temperature when set, where 'high temperature'
                               is dicated by the thermal monitor trip
                               temperature minus offset as defined in
                               IA32_TEMPERATURE_TARGET.
                            */
    UINT32 ProchotIntEnable : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Bidirectional PROCHOT# assertion interrupt
                               enable. If set, a thermal interrupt is delivered
                               on the rising edge of xxPROCHOT#.
                            */
    UINT32 PmaxIntEnable : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               PMAX detector interrupt enable. If set, a PMAX
                               interrupt is processed on a 0 to 1 transition.
                            */
    UINT32 OutOfSpecIntEnable : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Thermal interrupt enable for the Out Of Spec
                               condition which is stored in the Out Of
                               Spec/Critical Temperature status bit in
                               PACKAGE_THERM_STATUS.
                            */
    UINT32 Rsvd5 : 3;

                            /* Bits[7:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Threshold1RelTemp : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000000*/

                            /*
                               This value indicates the offset in degrees below
                               TJ Max Temperature that should trigger a Thermal
                               Threshold 1 trip.
                            */
    UINT32 Threshold1IntEnable : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Controls the generation of a thermal interrupt
                               whenever the Thermal Threshold 1 Temperature is
                               crossed.
                            */
    UINT32 Threshold2RelTemp : 7;

                            /* Bits[22:16], Access Type=RW, default=0x00000000*/

                            /*
                               This value indicates the offset in degrees below
                               TJ Max Temperature that should trigger a Thermal
                               Threshold 2 trip.
                            */
    UINT32 Threshold2IntEnable : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Controls the generation of a thermal interrupt
                               whenever the Thermal Threshold 2 Temperature is
                               crossed.
                            */
    UINT32 PowerIntEnable : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               When this bit is set, a thermal interrupt will
                               be sent upon throttling due to power
                               limitations.
                            */
    UINT32 HwFeedbackNotificationEnable : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Enables generation of an interrupt for HW
                               FEEDBACK change occurs (eg. due to thermal/power
                               limits)
                            */
    UINT32 Rsvd26 : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PACKAGE_THERM_INTERRUPT_REGISTER;

/**
  Debug MSR for model specific debug features.
**/
#define MSR_DEBUGCTLMSR 0x000001D9

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Lbr : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* LBR Enable/Disable Last Branch Records. */
    UINT32 Btf : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* BTF Branch Trap Flag. */
    UINT32 Reserved2 : 4;

                            /* Bits[5:2], Access Type=RW, default=0x00000000*/

                            /* These bits are reserved for future use. */
    UINT32 Tr : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* TR Enable/Disable Execution Trace Messages. */
    UINT32 Bts : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Branch trace store */
    UINT32 Btint : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /* Branch trace interrupt */
    UINT32 BtsOffOs : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* BTS_OFF_OS */
    UINT32 BtsOffUsr : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* BTS_OFF_USR */
    UINT32 FreezeLbrsOnPmi : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* FREEZE_LBRS_ON_PMI */
    UINT32 FreezePerfmonCtrsOnPmi : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /* FREEZE_PERFMON_CTRS_ON_PMI */
    UINT32 EnablePmiFromUncore : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit enables a PMI for this thread
                               when the Uncore signals a PMI. For PERFMON
                            */
    UINT32 MachineMonitoringOffInSmm : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit disables Machine Monitoring in
                               SMM
                            */
    UINT32 HlepDebug : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* Enables DR7 debug bit on XBEGIN */
    UINT32 Rsvd16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_DEBUGCTLMSR_REGISTER;

/**
  DCA CPU capability register
**/
#define MSR_CPU_DCA_CAP 0x000001F9

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Type0 : 1;

                            /* Bits[0:0], Access Type=RO_V, default=None*/

                            /* If set, CPU supports Prefetch-Hint type. */
    UINT32 Rsvd1 : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_CPU_DCA_CAP_REGISTER;

/**
  Variable MTRR Entry 0 PhysBase register
**/
#define MSR_MTRRVARBASE0 0x00000200

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Memtype : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR MemType */
    UINT32 Rsvd3 : 7;

                            /* Bits[9:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Inc1g : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Variable Base inconsistency bit for 1G pages */
    UINT32 Inc : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Variable Base inconsistency bit */
    UINT32 Base0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* Variable Base Address */
    UINT32 Base1 : 20;

                            /* Bits[51:32], Access Type=RW, default=0x00000000*/

                            /* Variable Base Address */
    UINT32 Rsvd52 : 12;

                            /* Bits[63:52], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRVARBASE0_REGISTER;

/**
  Variable MTRR Entry 0 PhysMask register
**/
#define MSR_MTRRVARMASK0 0x00000201

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 11;

                            /* Bits[10:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Vld : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR valid bit */
    UINT32 Mask0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR Mask */
    UINT32 Mask1 : 20;

                            /* Bits[51:32], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR Mask */
    UINT32 Rsvd52 : 12;

                            /* Bits[63:52], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRVARMASK0_REGISTER;

/**
  Variable MTRR Entry 1 PhysBase register
**/
#define MSR_MTRRVARBASE1 0x00000202

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Memtype : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR MemType */
    UINT32 Rsvd3 : 7;

                            /* Bits[9:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Inc1g : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Variable Base inconsistency bit for 1G pages */
    UINT32 Inc : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Variable Base inconsistency bit */
    UINT32 Base0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* Variable Base Address */
    UINT32 Base1 : 20;

                            /* Bits[51:32], Access Type=RW, default=0x00000000*/

                            /* Variable Base Address */
    UINT32 Rsvd52 : 12;

                            /* Bits[63:52], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRVARBASE1_REGISTER;

/**
  Variable MTRR Entry 1 PhysMask register
**/
#define MSR_MTRRVARMASK1 0x00000203

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 11;

                            /* Bits[10:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Vld : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR valid bit */
    UINT32 Mask0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR Mask */
    UINT32 Mask1 : 20;

                            /* Bits[51:32], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR Mask */
    UINT32 Rsvd52 : 12;

                            /* Bits[63:52], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRVARMASK1_REGISTER;

/**
  Variable MTRR Entry 2 PhysBase register
**/
#define MSR_MTRRVARBASE2 0x00000204

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Memtype : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR MemType */
    UINT32 Rsvd3 : 7;

                            /* Bits[9:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Inc1g : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Variable Base inconsistency bit for 1G pages */
    UINT32 Inc : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Variable Base inconsistency bit */
    UINT32 Base0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* Variable Base Address */
    UINT32 Base1 : 20;

                            /* Bits[51:32], Access Type=RW, default=0x00000000*/

                            /* Variable Base Address */
    UINT32 Rsvd52 : 12;

                            /* Bits[63:52], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRVARBASE2_REGISTER;

/**
  Variable MTRR Entry 2 PhysMask register
**/
#define MSR_MTRRVARMASK2 0x00000205

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 11;

                            /* Bits[10:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Vld : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR valid bit */
    UINT32 Mask0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR Mask */
    UINT32 Mask1 : 20;

                            /* Bits[51:32], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR Mask */
    UINT32 Rsvd52 : 12;

                            /* Bits[63:52], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRVARMASK2_REGISTER;

/**
  Variable MTRR Entry 3 PhysBase register
**/
#define MSR_MTRRVARBASE3 0x00000206

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Memtype : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR MemType */
    UINT32 Rsvd3 : 7;

                            /* Bits[9:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Inc1g : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Variable Base inconsistency bit for 1G pages */
    UINT32 Inc : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Variable Base inconsistency bit */
    UINT32 Base0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* Variable Base Address */
    UINT32 Base1 : 20;

                            /* Bits[51:32], Access Type=RW, default=0x00000000*/

                            /* Variable Base Address */
    UINT32 Rsvd52 : 12;

                            /* Bits[63:52], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRVARBASE3_REGISTER;

/**
  Variable MTRR Entry 3 PhysMask register
**/
#define MSR_MTRRVARMASK3 0x00000207

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 11;

                            /* Bits[10:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Vld : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR valid bit */
    UINT32 Mask0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR Mask */
    UINT32 Mask1 : 20;

                            /* Bits[51:32], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR Mask */
    UINT32 Rsvd52 : 12;

                            /* Bits[63:52], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRVARMASK3_REGISTER;

/**
  Variable MTRR Entry 4 PhysBase register
**/
#define MSR_MTRRVARBASE4 0x00000208

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Memtype : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR MemType */
    UINT32 Rsvd3 : 7;

                            /* Bits[9:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Inc1g : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Variable Base inconsistency bit for 1G pages */
    UINT32 Inc : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Variable Base inconsistency bit */
    UINT32 Base0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* Variable Base Address */
    UINT32 Base1 : 20;

                            /* Bits[51:32], Access Type=RW, default=0x00000000*/

                            /* Variable Base Address */
    UINT32 Rsvd52 : 12;

                            /* Bits[63:52], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRVARBASE4_REGISTER;

/**
  Variable MTRR Entry 4 PhysMask register
**/
#define MSR_MTRRVARMASK4 0x00000209

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 11;

                            /* Bits[10:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Vld : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR valid bit */
    UINT32 Mask0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR Mask */
    UINT32 Mask1 : 20;

                            /* Bits[51:32], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR Mask */
    UINT32 Rsvd52 : 12;

                            /* Bits[63:52], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRVARMASK4_REGISTER;

/**
  Variable MTRR Entry 5 PhysBase register
**/
#define MSR_MTRRVARBASE5 0x0000020A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Memtype : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR MemType */
    UINT32 Rsvd3 : 7;

                            /* Bits[9:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Inc1g : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Variable Base inconsistency bit for 1G pages */
    UINT32 Inc : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Variable Base inconsistency bit */
    UINT32 Base0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* Variable Base Address */
    UINT32 Base1 : 20;

                            /* Bits[51:32], Access Type=RW, default=0x00000000*/

                            /* Variable Base Address */
    UINT32 Rsvd52 : 12;

                            /* Bits[63:52], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRVARBASE5_REGISTER;

/**
  Variable MTRR Entry 5 PhysMask register
**/
#define MSR_MTRRVARMASK5 0x0000020B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 11;

                            /* Bits[10:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Vld : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR valid bit */
    UINT32 Mask0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR Mask */
    UINT32 Mask1 : 20;

                            /* Bits[51:32], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR Mask */
    UINT32 Rsvd52 : 12;

                            /* Bits[63:52], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRVARMASK5_REGISTER;

/**
  Variable MTRR Entry 6 PhysBase register
**/
#define MSR_MTRRVARBASE6 0x0000020C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Memtype : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR MemType */
    UINT32 Rsvd3 : 7;

                            /* Bits[9:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Inc1g : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Variable Base inconsistency bit for 1G pages */
    UINT32 Inc : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Variable Base inconsistency bit */
    UINT32 Base0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* Variable Base Address */
    UINT32 Base1 : 20;

                            /* Bits[51:32], Access Type=RW, default=0x00000000*/

                            /* Variable Base Address */
    UINT32 Rsvd52 : 12;

                            /* Bits[63:52], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRVARBASE6_REGISTER;

/**
  Variable MTRR Entry 6 PhysMask register
**/
#define MSR_MTRRVARMASK6 0x0000020D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 11;

                            /* Bits[10:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Vld : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR valid bit */
    UINT32 Mask0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR Mask */
    UINT32 Mask1 : 20;

                            /* Bits[51:32], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR Mask */
    UINT32 Rsvd52 : 12;

                            /* Bits[63:52], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRVARMASK6_REGISTER;

/**
  Variable MTRR Entry 7 PhysBase register
**/
#define MSR_MTRRVARBASE7 0x0000020E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Memtype : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR MemType */
    UINT32 Rsvd3 : 7;

                            /* Bits[9:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Inc1g : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Variable Base inconsistency bit for 1G pages */
    UINT32 Inc : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Variable Base inconsistency bit */
    UINT32 Base0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* Variable Base Address */
    UINT32 Base1 : 20;

                            /* Bits[51:32], Access Type=RW, default=0x00000000*/

                            /* Variable Base Address */
    UINT32 Rsvd52 : 12;

                            /* Bits[63:52], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRVARBASE7_REGISTER;

/**
  Variable MTRR Entry 7 PhysMask register
**/
#define MSR_MTRRVARMASK7 0x0000020F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 11;

                            /* Bits[10:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Vld : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR valid bit */
    UINT32 Mask0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR Mask */
    UINT32 Mask1 : 20;

                            /* Bits[51:32], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR Mask */
    UINT32 Rsvd52 : 12;

                            /* Bits[63:52], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRVARMASK7_REGISTER;

/**
  Variable MTRR Entry 8 PhysBase register
**/
#define MSR_MTRRVARBASE8 0x00000210

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Memtype : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR MemType */
    UINT32 Rsvd3 : 7;

                            /* Bits[9:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Inc1g : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Variable Base inconsistency bit for 1G pages */
    UINT32 Inc : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Variable Base inconsistency bit */
    UINT32 Base0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* Variable Base Address */
    UINT32 Base1 : 20;

                            /* Bits[51:32], Access Type=RW, default=0x00000000*/

                            /* Variable Base Address */
    UINT32 Rsvd52 : 12;

                            /* Bits[63:52], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRVARBASE8_REGISTER;

/**
  Variable MTRR Entry 8 PhysMask register
**/
#define MSR_MTRRVARMASK8 0x00000211

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 11;

                            /* Bits[10:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Vld : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR valid bit */
    UINT32 Mask0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR Mask */
    UINT32 Mask1 : 20;

                            /* Bits[51:32], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR Mask */
    UINT32 Rsvd52 : 12;

                            /* Bits[63:52], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRVARMASK8_REGISTER;

/**
  Variable MTRR Entry 9 PhysBase register
**/
#define MSR_MTRRVARBASE9 0x00000212

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Memtype : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR MemType */
    UINT32 Rsvd3 : 7;

                            /* Bits[9:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Inc1g : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Variable Base inconsistency bit for 1G pages */
    UINT32 Inc : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Variable Base inconsistency bit */
    UINT32 Base0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* Variable Base Address */
    UINT32 Base1 : 20;

                            /* Bits[51:32], Access Type=RW, default=0x00000000*/

                            /* Variable Base Address */
    UINT32 Rsvd52 : 12;

                            /* Bits[63:52], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRVARBASE9_REGISTER;

/**
  Variable MTRR Entry 9 PhysMask register
**/
#define MSR_MTRRVARMASK9 0x00000213

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 11;

                            /* Bits[10:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Vld : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR valid bit */
    UINT32 Mask0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR Mask */
    UINT32 Mask1 : 20;

                            /* Bits[51:32], Access Type=RW, default=0x00000000*/

                            /* Variable MTRR Mask */
    UINT32 Rsvd52 : 12;

                            /* Bits[63:52], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRVARMASK9_REGISTER;

/**
  Maps 512K address range from 0H-7FFFFH
**/
#define MSR_MTRRFIX64K 0x00000250

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Range0 : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Maps 64K addr range from 00000-0FFFF */
    UINT32 Rsvd3 : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range1 : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /* Maps 64K addr range from 10000-1FFFF */
    UINT32 Rsvd11 : 5;

                            /* Bits[15:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range2 : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /* Maps 64K addr range from 20000-2FFFF */
    UINT32 Rsvd19 : 5;

                            /* Bits[23:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range3 : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* Maps 64K addr range from 30000-3FFFF */
    UINT32 Rsvd27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range4 : 3;

                            /* Bits[34:32], Access Type=RW, default=0x00000000*/

                            /* Maps 64K addr range from 40000-4FFFF */
    UINT32 Rsvd35 : 5;

                            /* Bits[39:35], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range5 : 3;

                            /* Bits[42:40], Access Type=RW, default=0x00000000*/

                            /* Maps 64K addr range from 50000-5FFFF */
    UINT32 Rsvd43 : 5;

                            /* Bits[47:43], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range6 : 3;

                            /* Bits[50:48], Access Type=RW, default=0x00000000*/

                            /* Maps 64K addr range from 60000-6FFFF */
    UINT32 Rsvd51 : 5;

                            /* Bits[55:51], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range7 : 3;

                            /* Bits[58:56], Access Type=RW, default=0x00000000*/

                            /* Maps 64K addr range from 70000-7FFFF */
    UINT32 Rsvd59 : 5;

                            /* Bits[63:59], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRFIX64K_REGISTER;

/**
  Maps 128K address range from 80000H-9FFFFH
**/
#define MSR_MTRRFIX16K8 0x00000258

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Range0 : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from 80000-83FFF */
    UINT32 Rsvd3 : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range1 : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from 84000-87FFF */
    UINT32 Rsvd11 : 5;

                            /* Bits[15:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range2 : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from 88000-8BFFF */
    UINT32 Rsvd19 : 5;

                            /* Bits[23:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range3 : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from 8C000-8FFFF */
    UINT32 Rsvd27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range4 : 3;

                            /* Bits[34:32], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from 90000-93FFF */
    UINT32 Rsvd35 : 5;

                            /* Bits[39:35], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range5 : 3;

                            /* Bits[42:40], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from 94000-97FFF */
    UINT32 Rsvd43 : 5;

                            /* Bits[47:43], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range6 : 3;

                            /* Bits[50:48], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from 98000-9BFFF */
    UINT32 Rsvd51 : 5;

                            /* Bits[55:51], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range7 : 3;

                            /* Bits[58:56], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from 9C000-9FFFF */
    UINT32 Rsvd59 : 5;

                            /* Bits[63:59], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRFIX16K8_REGISTER;

/**
  Maps 128K address range from A0000H-BFFFFH
**/
#define MSR_MTRRFIX16KA 0x00000259

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Range0 : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from A0000-A3FFF */
    UINT32 Rsvd3 : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range1 : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from A4000-A7FFF */
    UINT32 Rsvd11 : 5;

                            /* Bits[15:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range2 : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from A8000-ABFFF */
    UINT32 Rsvd19 : 5;

                            /* Bits[23:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range3 : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from AC000-AFFFF */
    UINT32 Rsvd27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range4 : 3;

                            /* Bits[34:32], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from B0000-B3FFF */
    UINT32 Rsvd35 : 5;

                            /* Bits[39:35], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range5 : 3;

                            /* Bits[42:40], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from B4000-B7FFF */
    UINT32 Rsvd43 : 5;

                            /* Bits[47:43], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range6 : 3;

                            /* Bits[50:48], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from B8000-BBFFF */
    UINT32 Rsvd51 : 5;

                            /* Bits[55:51], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range7 : 3;

                            /* Bits[58:56], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from BC000-BFFFF */
    UINT32 Rsvd59 : 5;

                            /* Bits[63:59], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRFIX16KA_REGISTER;

/**
  Maps 32K address range from C0000H-C7FFFH
**/
#define MSR_MTRRFIX4KC0 0x00000268

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Range0 : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from C0000-C0FFF */
    UINT32 Rsvd3 : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range1 : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from C1000-C1FFF */
    UINT32 Rsvd11 : 5;

                            /* Bits[15:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range2 : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from C2000-C2FFF */
    UINT32 Rsvd19 : 5;

                            /* Bits[23:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range3 : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from C3000-C3FFF */
    UINT32 Rsvd27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range4 : 3;

                            /* Bits[34:32], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from C4000-C4FFF */
    UINT32 Rsvd35 : 5;

                            /* Bits[39:35], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range5 : 3;

                            /* Bits[42:40], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from C5000-C5FFF */
    UINT32 Rsvd43 : 5;

                            /* Bits[47:43], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range6 : 3;

                            /* Bits[50:48], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from C6000-C6FFF */
    UINT32 Rsvd51 : 5;

                            /* Bits[55:51], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range7 : 3;

                            /* Bits[58:56], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from C7000-C7FFF */
    UINT32 Rsvd59 : 5;

                            /* Bits[63:59], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRFIX4KC0_REGISTER;

/**
  Maps 32K address range from C8000H-CFFFFH
**/
#define MSR_MTRRFIX4KC8 0x00000269

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Range0 : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from C8000-C8FFF */
    UINT32 Rsvd3 : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range1 : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from C9000-C8FFF */
    UINT32 Rsvd11 : 5;

                            /* Bits[15:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range2 : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from CA000-CAFFF */
    UINT32 Rsvd19 : 5;

                            /* Bits[23:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range3 : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from CB000-CBFFF */
    UINT32 Rsvd27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range4 : 3;

                            /* Bits[34:32], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from CC000-CCFFF */
    UINT32 Rsvd35 : 5;

                            /* Bits[39:35], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range5 : 3;

                            /* Bits[42:40], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from CD000-CDFFF */
    UINT32 Rsvd43 : 5;

                            /* Bits[47:43], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range6 : 3;

                            /* Bits[50:48], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from CE000-CEFFF */
    UINT32 Rsvd51 : 5;

                            /* Bits[55:51], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range7 : 3;

                            /* Bits[58:56], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from CF000-CFFFF */
    UINT32 Rsvd59 : 5;

                            /* Bits[63:59], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRFIX4KC8_REGISTER;

/**
  Maps 32K address range from D0000H-D7FFFH
**/
#define MSR_MTRRFIX4KD0 0x0000026A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Range0 : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from D0000-D0FFF */
    UINT32 Rsvd3 : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range1 : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from D1000-D1FFF */
    UINT32 Rsvd11 : 5;

                            /* Bits[15:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range2 : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from D2000-D2FFF */
    UINT32 Rsvd19 : 5;

                            /* Bits[23:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range3 : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from D3000-D3FFF */
    UINT32 Rsvd27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range4 : 3;

                            /* Bits[34:32], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from D4000-D4FFF */
    UINT32 Rsvd35 : 5;

                            /* Bits[39:35], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range5 : 3;

                            /* Bits[42:40], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from D5000-D5FFF */
    UINT32 Rsvd43 : 5;

                            /* Bits[47:43], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range6 : 3;

                            /* Bits[50:48], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from D6000-D6FFF */
    UINT32 Rsvd51 : 5;

                            /* Bits[55:51], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range7 : 3;

                            /* Bits[58:56], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from D7000-D7FFF */
    UINT32 Rsvd59 : 5;

                            /* Bits[63:59], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRFIX4KD0_REGISTER;

/**
  Maps 32K address range from D8000H-DFFFFH
**/
#define MSR_MTRRFIX4KD8 0x0000026B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Range0 : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from D8000-D8FFF */
    UINT32 Rsvd3 : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range1 : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from D9000-D9FFF */
    UINT32 Rsvd11 : 5;

                            /* Bits[15:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range2 : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from DA000-DAFFF */
    UINT32 Rsvd19 : 5;

                            /* Bits[23:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range3 : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from DB000-DBFFF */
    UINT32 Rsvd27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range4 : 3;

                            /* Bits[34:32], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from DC000-DCFFF */
    UINT32 Rsvd35 : 5;

                            /* Bits[39:35], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range5 : 3;

                            /* Bits[42:40], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from DD000-DDFFF */
    UINT32 Rsvd43 : 5;

                            /* Bits[47:43], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range6 : 3;

                            /* Bits[50:48], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from DE000-DEFFF */
    UINT32 Rsvd51 : 5;

                            /* Bits[55:51], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range7 : 3;

                            /* Bits[58:56], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from DF000-DFFFF */
    UINT32 Rsvd59 : 5;

                            /* Bits[63:59], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRFIX4KD8_REGISTER;

/**
  Maps 32K address range from E0000H-E7FFFH
**/
#define MSR_MTRRFIX4KE0 0x0000026C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Range0 : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from E0000-E0FFF */
    UINT32 Rsvd3 : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range1 : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from E1000-E1FFF */
    UINT32 Rsvd11 : 5;

                            /* Bits[15:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range2 : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from E2000-E2FFF */
    UINT32 Rsvd19 : 5;

                            /* Bits[23:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range3 : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from E3000-E3FFF */
    UINT32 Rsvd27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range4 : 3;

                            /* Bits[34:32], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from E4000-E4FFF */
    UINT32 Rsvd35 : 5;

                            /* Bits[39:35], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range5 : 3;

                            /* Bits[42:40], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from E5000-E5FFF */
    UINT32 Rsvd43 : 5;

                            /* Bits[47:43], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range6 : 3;

                            /* Bits[50:48], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from E6000-E6FFF */
    UINT32 Rsvd51 : 5;

                            /* Bits[55:51], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range7 : 3;

                            /* Bits[58:56], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from E7000-E7FFF */
    UINT32 Rsvd59 : 5;

                            /* Bits[63:59], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRFIX4KE0_REGISTER;

/**
  Maps 32K address range from E8000H-EFFFFH
**/
#define MSR_MTRRFIX4KE8 0x0000026D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Range0 : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from E8000-E8FFF */
    UINT32 Rsvd3 : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range1 : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from E9000-E9FFF */
    UINT32 Rsvd11 : 5;

                            /* Bits[15:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range2 : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from EA000-EAFFF */
    UINT32 Rsvd19 : 5;

                            /* Bits[23:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range3 : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from EB000-EBFFF */
    UINT32 Rsvd27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range4 : 3;

                            /* Bits[34:32], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from EC000-ECFFF */
    UINT32 Rsvd35 : 5;

                            /* Bits[39:35], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range5 : 3;

                            /* Bits[42:40], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from ED000-EDFFF */
    UINT32 Rsvd43 : 5;

                            /* Bits[47:43], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range6 : 3;

                            /* Bits[50:48], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from EE000-EEFFF */
    UINT32 Rsvd51 : 5;

                            /* Bits[55:51], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range7 : 3;

                            /* Bits[58:56], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from EF000-EFFFF */
    UINT32 Rsvd59 : 5;

                            /* Bits[63:59], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRFIX4KE8_REGISTER;

/**
  Maps 32K address range from F0000H-F7FFFH
**/
#define MSR_MTRRFIX4KF0 0x0000026E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Range0 : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from F0000-F0FFF */
    UINT32 Rsvd3 : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range1 : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from F1000-F1FFF */
    UINT32 Rsvd11 : 5;

                            /* Bits[15:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range2 : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from F2000-F2FFF */
    UINT32 Rsvd19 : 5;

                            /* Bits[23:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range3 : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from F3000-F3FFF */
    UINT32 Rsvd27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range4 : 3;

                            /* Bits[34:32], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from F4000-F4FFF */
    UINT32 Rsvd35 : 5;

                            /* Bits[39:35], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range5 : 3;

                            /* Bits[42:40], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from F5000-F5FFF */
    UINT32 Rsvd43 : 5;

                            /* Bits[47:43], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range6 : 3;

                            /* Bits[50:48], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from F6000-F6FFF */
    UINT32 Rsvd51 : 5;

                            /* Bits[55:51], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range7 : 3;

                            /* Bits[58:56], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from F7000-F7FFF */
    UINT32 Rsvd59 : 5;

                            /* Bits[63:59], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRFIX4KF0_REGISTER;

/**
  Maps 32K address range from F8000H-FFFFFH
**/
#define MSR_MTRRFIX4KF8 0x0000026F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Range0 : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from F8000-F8FFF */
    UINT32 Rsvd3 : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range1 : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from F9000-F9FFF */
    UINT32 Rsvd11 : 5;

                            /* Bits[15:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range2 : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from FA000-FAFFF */
    UINT32 Rsvd19 : 5;

                            /* Bits[23:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range3 : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from FB000-FBFFF */
    UINT32 Rsvd27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range4 : 3;

                            /* Bits[34:32], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from FC000-FCFFF */
    UINT32 Rsvd35 : 5;

                            /* Bits[39:35], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range5 : 3;

                            /* Bits[42:40], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from FD000-FDFFF */
    UINT32 Rsvd43 : 5;

                            /* Bits[47:43], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range6 : 3;

                            /* Bits[50:48], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from FE000-FEFFF */
    UINT32 Rsvd51 : 5;

                            /* Bits[55:51], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Range7 : 3;

                            /* Bits[58:56], Access Type=RW, default=0x00000000*/

                            /* Maps 16K addr range from FF000-FFFFF */
    UINT32 Rsvd59 : 5;

                            /* Bits[63:59], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRFIX4KF8_REGISTER;

/**
  PAT memtypes register. 8 seperate entries
**/
#define MSR_PAT 0x00000277

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Entry0 : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000006*/

                            /* PAT Memtype entry-0 */
    UINT32 Rsvd3 : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Entry1 : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000004*/

                            /* PAT Memtype entry-1 */
    UINT32 Rsvd11 : 5;

                            /* Bits[15:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Entry2 : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000007*/

                            /* PAT Memtype entry-2 */
    UINT32 Rsvd19 : 5;

                            /* Bits[23:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Entry3 : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* PAT Memtype entry-3 */
    UINT32 Rsvd27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Entry4 : 3;

                            /* Bits[34:32], Access Type=RW, default=0x00000006*/

                            /* PAT Memtype entry-4 */
    UINT32 Rsvd35 : 5;

                            /* Bits[39:35], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Entry5 : 3;

                            /* Bits[42:40], Access Type=RW, default=0x00000004*/

                            /* PAT Memtype entry-5 */
    UINT32 Rsvd43 : 5;

                            /* Bits[47:43], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Entry6 : 3;

                            /* Bits[50:48], Access Type=RW, default=0x00000007*/

                            /* PAT Memtype entry-6 */
    UINT32 Rsvd51 : 5;

                            /* Bits[55:51], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Entry7 : 3;

                            /* Bits[58:56], Access Type=RW, default=0x00000000*/

                            /* PAT Memtype entry-7 */
    UINT32 Rsvd59 : 5;

                            /* Bits[63:59], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PAT_REGISTER;

/**
  Machine Check CTL2 Register
**/
#define MSR_MC0_MISC2 0x00000280

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 CorrectedErrThrsh : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* corrected error count threshold */
    UINT32 Rsvd15 : 15;

                            /* Bits[29:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CorrectedErrEn : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* Corrected error interupt enable/disable */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 29;

                            /* Bits[63:35], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC0_MISC2_REGISTER;

/**
  Machine Check CTL2 Register
**/
#define MSR_MC1_MISC2 0x00000281

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Cet : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /*
                               Correctable error threshold selects the
                               threshold at which the corrected error count
                               logged in the status register will signal an
                               interrupt
                            */
    UINT32 Reserved15 : 15;

                            /* Bits[29:15], Access Type=WO, default=0x00000000*/

                            /*
                               All the RESERVED bits described here are not
                               implemented in the DCU. They can be hardwired to
                               0s
                            */
    UINT32 E : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* The corrected error threshold enable/disable bit */
    UINT32 Reserved31 : 1;

                            /* Bits[31:31], Access Type=WO, default=0x00000000*/

                            /*
                               All the RESERVED bits described here are not
                               implemented in the DCU. They can be hardwired to
                               0s.
                            */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 1;

                            /* Bits[35:35], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Reserved36 : 28;

                            /* Bits[63:36], Access Type=WO, default=0x00000000*/

                            /*
                               All the RESERVED bits described here are not
                               implemented in the DCU. They can be hardwired to
                               0s.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC1_MISC2_REGISTER;

/**
  Machine Check CTL2 Register
**/
#define MSR_MC2_MISC2 0x00000282

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 CorErrThres : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* Correctable error count threshold */
    UINT32 Reserved15 : 15;

                            /* Bits[29:15], Access Type=RW, default=None*/

                            /*
                               Unimplemented - will return all '0s when read.
                               writes will be dropped on the floor
                            */
    UINT32 CmciEn : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* CMCI Enable */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 29;

                            /* Bits[63:35], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC2_MISC2_REGISTER;

/**
  Machine Check CTL2 Register
**/
#define MSR_MC3_MISC2 0x00000283

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Cet : 15;

                            /* Bits[14:0], Access Type=RW, default=None*/

                            /*
                               The default value for the corrected error
                               threshold bits is 0. Software can set a
                               threshold value by writing to this field.
                            */
    UINT32 ReservedBits15 : 15;

                            /* Bits[29:15], Access Type=RW, default=None*/

                            /* Reserved bits not implemented */
    UINT32 En : 1;

                            /* Bits[30:30], Access Type=RW, default=None*/

                            /*
                               The EN bit is set by software to enable the
                               generation of corrected error threshold
                               interrupts corrected error threshold
                            */
    UINT32 ReservedBit31 : 1;

                            /* Bits[31:31], Access Type=RW, default=None*/

                            /* Reserved bits not implemented */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=None*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=None*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 29;

                            /* Bits[63:35], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC3_MISC2_REGISTER;

/**
  IA32_MEE_MC_CTL2 MC MSR aliased to IA32_PCU5_MC_CTL2
**/
#define MSR_MC4_CTL2 0x00000284

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Threshold : 15;

                            /* Bits[14:0], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 Rsvd15 : 15;

                            /* Bits[29:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Enable : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 29;

                            /* Bits[63:35], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC4_CTL2_REGISTER;

/**
  Machine check CTL2 MSR
**/
#define MSR_MC5_CTL2 0x00000285

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Threshold : 15;

                            /* Bits[14:0], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 Rsvd15 : 15;

                            /* Bits[29:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Enable : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 29;

                            /* Bits[63:35], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC5_CTL2_REGISTER;

/**
  Machine Check CTL2 Register
**/
#define MSR_MC6_CTL2 0x00000286

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Threshold : 15;

                            /* Bits[14:0], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 Rsvd15 : 15;

                            /* Bits[29:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Enable : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 29;

                            /* Bits[63:35], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC6_CTL2_REGISTER;

/**
  LLC MCA bank: Control 2 register
**/
#define MSR_MC7_CTL2 0x00000287

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 CorCntrThreshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* Corrected Error Counter Threshold */
    UINT32 Rsvd15 : 15;

                            /* Bits[29:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 En : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* EN: Enable corrected error interrupts */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 29;

                            /* Bits[63:35], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC7_CTL2_REGISTER;

/**
  LLC MCA bank: Control 2 register
**/
#define MSR_MC8_CTL2 0x00000288

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 CorCntrThreshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* Corrected Error Counter Threshold */
    UINT32 Rsvd15 : 15;

                            /* Bits[29:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 En : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* EN: Enable corrected error interrupts */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 29;

                            /* Bits[63:35], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC8_CTL2_REGISTER;

/**
  LLC MCA bank: Control 2 register
**/
#define MSR_MC9_CTL2 0x00000289

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 CorCntrThreshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* Corrected Error Counter Threshold */
    UINT32 Rsvd15 : 15;

                            /* Bits[29:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 En : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* EN: Enable corrected error interrupts */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 29;

                            /* Bits[63:35], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC9_CTL2_REGISTER;

/**
  LLC MCA bank: Control 2 register
**/
#define MSR_MC10_CTL2 0x0000028A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 CorCntrThreshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* Corrected Error Counter Threshold */
    UINT32 Rsvd15 : 15;

                            /* Bits[29:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 En : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* EN: Enable corrected error interrupts */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 29;

                            /* Bits[63:35], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC10_CTL2_REGISTER;

/**
  LLC MCA bank: Control 2 register
**/
#define MSR_MC11_CTL2 0x0000028B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 CorCntrThreshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* Corrected Error Counter Threshold */
    UINT32 Rsvd15 : 15;

                            /* Bits[29:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 En : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* EN: Enable corrected error interrupts */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 29;

                            /* Bits[63:35], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC11_CTL2_REGISTER;

/**
  LLC MCA bank: Control 2 register
**/
#define MSR_MC12_CTL2 0x0000028C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 CorCntrThreshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* Corrected Error Counter Threshold */
    UINT32 Rsvd15 : 15;

                            /* Bits[29:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 En : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* EN: Enable corrected error interrupts */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 29;

                            /* Bits[63:35], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC12_CTL2_REGISTER;

/**
  LLC MCA bank: Control 2 register
**/
#define MSR_MC13_CTL2 0x0000028D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 CorCntrThreshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* Corrected Error Counter Threshold */
    UINT32 Rsvd15 : 15;

                            /* Bits[29:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 En : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* EN: Enable corrected error interrupts */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 29;

                            /* Bits[63:35], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC13_CTL2_REGISTER;

/**
  LLC MCA bank: Control 2 register
**/
#define MSR_MC14_CTL2 0x0000028E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 CorCntrThreshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* Corrected Error Counter Threshold */
    UINT32 Rsvd15 : 15;

                            /* Bits[29:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 En : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* EN: Enable corrected error interrupts */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 29;

                            /* Bits[63:35], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC14_CTL2_REGISTER;

/**
  LLC MCA bank: Control 2 register
**/
#define MSR_MC15_CTL2 0x0000028F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 CorCntrThreshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* Corrected Error Counter Threshold */
    UINT32 Rsvd15 : 15;

                            /* Bits[29:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 En : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* EN: Enable corrected error interrupts */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 29;

                            /* Bits[63:35], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC15_CTL2_REGISTER;

/**
  LLC MCA bank: Control 2 register
**/
#define MSR_MC16_CTL2 0x00000290

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 CorCntrThreshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* Corrected Error Counter Threshold */
    UINT32 Rsvd15 : 15;

                            /* Bits[29:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 En : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* EN: Enable corrected error interrupts */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 29;

                            /* Bits[63:35], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC16_CTL2_REGISTER;

/**
  LLC MCA bank: Control 2 register
**/
#define MSR_MC17_CTL2 0x00000291

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 CorCntrThreshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* Corrected Error Counter Threshold */
    UINT32 Rsvd15 : 15;

                            /* Bits[29:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 En : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* EN: Enable corrected error interrupts */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 29;

                            /* Bits[63:35], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC17_CTL2_REGISTER;

/**
  LLC MCA bank: Control 2 register
**/
#define MSR_MC18_CTL2 0x00000292

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 CorCntrThreshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* Corrected Error Counter Threshold */
    UINT32 Rsvd15 : 15;

                            /* Bits[29:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 En : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* EN: Enable corrected error interrupts */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 29;

                            /* Bits[63:35], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC18_CTL2_REGISTER;

/**
  EDRAM 0 Machine Check Architecture Control 2 MSR
**/
#define MSR_MC19_CTL2 0x00000293

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* Corrected Error Counter Threshold */
    UINT32 Rsvd15 : 15;

                            /* Bits[29:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 En : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* EN: Enable corrected error interrupts */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 29;

                            /* Bits[63:35], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC19_CTL2_REGISTER;

/**
  EDRAM 1 Machine Check Architecture Control 2 MSR
**/
#define MSR_MC20_CTL2 0x00000294

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* Corrected Error Counter Threshold */
    UINT32 Rsvd15 : 15;

                            /* Bits[29:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 En : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* EN: Enable corrected error interrupts */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 29;

                            /* Bits[63:35], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC20_CTL2_REGISTER;

/**
  2LM MCA bank: Control 2 register
**/
#define MSR_MC21_CTL2 0x00000295

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* Corrected Error Counter Threshold */
    UINT32 Rsvd15 : 15;

                            /* Bits[29:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 En : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* EN: Enable corrected error interrupts */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 29;

                            /* Bits[63:35], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC21_CTL2_REGISTER;

/**
  2LM MCA bank: Control 2 register
**/
#define MSR_MC22_CTL2 0x00000296

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* Corrected Error Counter Threshold */
    UINT32 Rsvd15 : 15;

                            /* Bits[29:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 En : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* EN: Enable corrected error interrupts */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 29;

                            /* Bits[63:35], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC22_CTL2_REGISTER;

/**
  2LM MCA bank: Control 2 register
**/
#define MSR_MC23_CTL2 0x00000297

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* Corrected Error Counter Threshold */
    UINT32 Rsvd15 : 15;

                            /* Bits[29:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 En : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* EN: Enable corrected error interrupts */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 29;

                            /* Bits[63:35], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC23_CTL2_REGISTER;

/**
  2LM MCA bank: Control 2 register
**/
#define MSR_MC24_CTL2 0x00000298

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Threshold : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000000*/

                            /* Corrected Error Counter Threshold */
    UINT32 Rsvd15 : 15;

                            /* Bits[29:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 En : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* EN: Enable corrected error interrupts */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CmciCtl : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /* Controls CMCI Remapping 0 CMCI 1 CSMI */
    UINT32 Rsvd33 : 1;

                            /* Bits[33:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MceCtl : 1;

                            /* Bits[34:34], Access Type=RW, default=0x00000000*/

                            /* Controls MCE Remapping 0 MCE 1 MSMI */
    UINT32 Rsvd35 : 29;

                            /* Bits[63:35], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC24_CTL2_REGISTER;

/**
  Default MTRR related Memory Type and Enables, Details Vol. 3A 11.11.2.3
**/
#define MSR_MTRRDEFAULT 0x000002FF

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Memtype : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Default MTRR MemType */
    UINT32 Rsvd3 : 7;

                            /* Bits[9:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Fe : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Default Fix MTRR Enable */
    UINT32 En : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Default MTRR Enable */
    UINT32 Rsvd12 : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MTRRDEFAULT_REGISTER;

/**
  PERFCNTD OCCUPANCY COUNTER
**/
#define MSR_PERF_OCCUPANCY_CTRS 0x00000305

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Ctr0 : 6;

                            /* Bits[5:0], Access Type=RW_V, default=0x00000000*/

                            /* OCCUPANCY CTR 0 */
    UINT32 Ctr1 : 6;

                            /* Bits[11:6], Access Type=RW_V, default=0x00000000*/

                            /* OCCUPANCY CTR 1 */
    UINT32 Ctr2 : 6;

                            /* Bits[17:12], Access Type=RW_V, default=0x00000000*/

                            /* OCCUPANCY CTR 2 */
    UINT32 Ctr3 : 6;

                            /* Bits[23:18], Access Type=RW_V, default=0x00000000*/

                            /* OCCUPANCY CTR 3 */
    UINT32 Rsvd24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PERF_OCCUPANCY_CTRS_REGISTER;

/**
  FIXED PERFORMANCE COUNTER
**/
#define MSR_EMON_FIXED_CTR0 0x00000309

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Lower : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Lower Half of PERFMON FIXED CNTR */
    UINT32 Upper : 16;

                            /* Bits[47:32], Access Type=RW, default=0x00000000*/

                            /* Upper Half of PERFMON FIXED CNTR */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_FIXED_CTR0_REGISTER;

/**
  FIXED PERFORMANCE COUNTER
**/
#define MSR_EMON_FIXED_CTR1 0x0000030A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Lower : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Lower Half of PERFMON FIXED CNTR */
    UINT32 Upper : 16;

                            /* Bits[47:32], Access Type=RW, default=0x00000000*/

                            /* Upper Half of PERFMON FIXED CNTR */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_FIXED_CTR1_REGISTER;

/**
  FIXED PERFORMANCE COUNTER
**/
#define MSR_EMON_FIXED_CTR2 0x0000030B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Lower : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Lower Half of PERFMON FIXED CNTR */
    UINT32 Upper : 16;

                            /* Bits[47:32], Access Type=RW, default=0x00000000*/

                            /* Upper Half of PERFMON FIXED CNTR */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_FIXED_CTR2_REGISTER;

/**
  FIXED PERFORMANCE COUNTER
**/
#define MSR_EMON_FIXED_CTR3 0x0000030C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Lower : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Lower Half of PERFMON FIXED CNTR */
    UINT32 Upper : 16;

                            /* Bits[47:32], Access Type=RW, default=0x00000000*/

                            /* Upper Half of PERFMON FIXED CNTR */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_FIXED_CTR3_REGISTER;

/**
  Perfmon msr for exposure metrics on several emons
**/
#define MSR_PERF_METRICS 0x00000329

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Retiring : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* retired */
    UINT32 BadSpeculation : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /* bad speculation */
    UINT32 FrontendBound : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /* frontend bound */
    UINT32 BackendBound : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /* backend bound */
    UINT32 Reserved32 : 32;

                            /* Bits[63:32], Access Type=RW, default=0x00000000*/

                            /* frontend bound */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PERF_METRICS_REGISTER;

/**
  This register is a Read Only MSR that enumerates the existence of certain
  debug features in PerfMon.
**/
#define MSR_EMON_PERF_CAPABILITIES 0x00000345

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Lbrformat : 6;

                            /* Bits[5:0], Access Type=RO_V, default=None*/

                            /*
                               Last Branch Record Format. 0=32-bit, 1=64-bit
                               LIP, 2=64-bit EIP
                            */
    UINT32 Pebstrap : 1;

                            /* Bits[6:6], Access Type=RO_V, default=None*/

                            /*
                               When set, PEBS recording is trap-like. After the
                               PEBS-enabled counter has overflowed, PEBS record
                               is recorded for the next PEBS-able event at the
                               completion of the sampled instruction causing
                               the PEBS event. When clear, PEBS recording is
                               fault-like. The PEBS record is recorded before
                               the sampled instruction causing the PEBS event.
                            */
    UINT32 Pebssavearchreg : 1;

                            /* Bits[7:7], Access Type=RO_V, default=None*/

                            /*
                               When set, PEBS will save architectural register
                               and state information according to the encoded
                               value of the PEBSRecordFormat field. On
                               processors based on Intel Core
                               microarchitecture, this bit is always 1.
                            */
    UINT32 Pebsrecordformat : 4;

                            /* Bits[11:8], Access Type=RO_V, default=None*/

                            /*
                               Valid encodings are: 0000B: Only general-purpose
                               registers, instruction pointer and RFLAGS
                               registers are saved in each PEBS record.
                            */
    UINT32 Disablemmduringsmm : 1;

                            /* Bits[12:12], Access Type=RO_V, default=None*/

                            /*
                               If set this indicates the ability to disable
                               machine monitoring in SMM.
                            */
    UINT32 Perfmonfullwidthwrite : 1;

                            /* Bits[13:13], Access Type=RO_V, default=None*/

                            /* Indicates support of full-width writes. */
    UINT32 Pebsadaptivedata : 1;

                            /* Bits[14:14], Access Type=RO_V, default=None*/

                            /*
                               Indicates PEBS support adaptive records (SNC and
                               on).
                            */
    UINT32 Perfmetricsavailable : 1;

                            /* Bits[15:15], Access Type=RO_V, default=None*/

                            /*
                               Indicates PERF METRICS is supported (SNC and
                               on).
                            */
    UINT32 Rsvd16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_PERF_CAPABILITIES_REGISTER;

/**
  This register provides enabling for the fixed performance counters
  and enabling for PMI and also provides a the MYTHREADS/ALLTHREADS
  for each counter as well.
**/
#define MSR_EMON_FIXED_CTR_CTRL 0x0000038D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 EnCtr0 : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               This bit is the local enable for the Fixed
                               counter 0. Disabling these bits or the bits in
                               ia32_cr_perf_global_ctrl[35:32] enable bit will
                               cause the counters to halt. Counter is enabled
                               by setting CPL Level: 00 - Disable 01 - Ring 0
                               (OS) 10 - Ring 1,2,3 (User) 11 - All ring levels
                               (OS or User) when CPL changes to one of the
                               above the counter gets enabled.
                            */
    UINT32 MyallthrCtr0 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               When this bit is set fixed counter 0 will count
                               instructions retired for both threads.
                            */
    UINT32 EnPmiCtr0 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1 will cause a PMI when the 48 bit
                               Fixed counter overflows.
                            */
    UINT32 EnCtr1 : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000000*/

                            /*
                               This bit is the local enable for the Fixed
                               counter 1. Disabling these bits or the bits in
                               ia32_cr_perf_global_ctrl[35:32] enable bit will
                               cause the counters to halt. Counter is enabled
                               by setting CPL Level: 00 - Disable 01 - Ring 0
                               (OS) 10 - Ring 1,2,3 (User) 11 - All ring levels
                               (OS or User) when CPL changes to one of the
                               above the counter gets enabled.
                            */
    UINT32 MyallthrCtr1 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               When this bit is set fixed counter 1 will count
                               instructions retired for both threads.
                            */
    UINT32 EnPmiCtr1 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1 will cause a PMI when the 48 bit
                               Fixed counter overflows.
                            */
    UINT32 EnCtr2 : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /*
                               This bit is the local enable for the Fixed
                               counter 2. Disabling these bits or the bits in
                               ia32_cr_perf_global_ctrl[35:32] enable bit will
                               cause the counters to halt. Counter is enabled
                               by setting CPL Level: 00 - Disable 01 - Ring 0
                               (OS) 10 - Ring 1,2,3 (User) 11 - All ring levels
                               (OS or User) when CPL changes to one of the
                               above the counter gets enabled.
                            */
    UINT32 MyallthrCtr2 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               When this bit is set fixed counter 2 will count
                               instructions retired for both threads.
                            */
    UINT32 EnPmiCtr2 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1 will cause a PMI when the 48 bit
                               Fixed counter overflows.
                            */
    UINT32 EnCtr3 : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000000*/

                            /*
                               This bit is the local enable for the Fixed
                               counter 3. Disabling these bits or the bits in
                               ia32_cr_perf_global_ctrl[35:32] enable bit will
                               cause the counters to halt. Counter is enabled
                               by setting CPL Level: 00 - Disable 01 - Ring 0
                               (OS) 10 - Ring 1,2,3 (User) 11 - All ring levels
                               (OS or User) when CPL changes to one of the
                               above the counter gets enabled.
                            */
    UINT32 Rsvd14 : 1;

                            /* Bits[14:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 EnPmiCtr3 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1 will cause a PMI when the 48 bit
                               Fixed counter overflows.
                            */
    UINT32 Rsvd16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 AdaptiveRecordCtr0 : 1;

                            /* Bits[32:32], Access Type=RW, default=0x00000000*/

                            /*
                               for adaptive pebs feature in SNC if
                               adaptive_record bit is set, Ucode will consider
                               MSR_PEBS_DATA_CFG and emit the requested fields
                            */
    UINT32 Rsvd33 : 3;

                            /* Bits[35:33], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 AdaptiveRecordCtr1 : 1;

                            /* Bits[36:36], Access Type=RW, default=0x00000000*/

                            /*
                               for adaptive pebs feature in SNC if
                               adaptive_record bit is set, Ucode will consider
                               MSR_PEBS_DATA_CFG and emit the requested fields
                            */
    UINT32 Rsvd37 : 3;

                            /* Bits[39:37], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 AdaptiveRecordCtr2 : 1;

                            /* Bits[40:40], Access Type=RW, default=0x00000000*/

                            /*
                               for adaptive pebs feature in SNC if
                               adaptive_record bit is set, Ucode will consider
                               MSR_PEBS_DATA_CFG and emit the requested fields
                            */
    UINT32 Rsvd41 : 3;

                            /* Bits[43:41], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 AdaptiveRecordCtr3 : 1;

                            /* Bits[44:44], Access Type=RW, default=0x00000000*/

                            /*
                               for adaptive pebs feature in SNC if
                               adaptive_record bit is set, Ucode will consider
                               MSR_PEBS_DATA_CFG and emit the requested fields
                            */
    UINT32 Rsvd45 : 19;

                            /* Bits[63:45], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_FIXED_CTR_CTRL_REGISTER;

/**
  Global Performance Counter Status
  A write of '1' to any of the bits in this register causes the corresponding bit
  in the IA32_CR_PERF_GLOBAL_STATUS register to set to '1'. Write to '0 ignored.
**/
#define MSR_EMON_PERF_GLOBAL_STATUS 0x0000038E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 OvfGenCtr : 8;

                            /* Bits[7:0], Access Type=RW1S, default=0x00000000*/

                            /*
                               This bit range 7:0 represents the overflow of
                               general counter 7 to 0 respecitively. These
                               status bits normally gets set whenever general
                               counters 7:0 overflow. Since the fixed and
                               general counters contain only positive 48 bit
                               count values, an overflow is the case where the
                               counter generates a carryout.
                            */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 OvfFixedCtr : 4;

                            /* Bits[35:32], Access Type=RW1S, default=0x00000000*/

                            /*
                               This bit range 35:32 represents the overflow of
                               fixed counters 3-0 respecitively. This status
                               bit normally gets set whenever the associated
                               fixed cntr overflows. Since the fixed and
                               general counters contain only positive 48 bit
                               count values, an overflow is the case where the
                               counter generates a carryout. Note, Only fixed
                               counter 0 can cause a PEBS thus writing bit for
                               fixed counter 0 will enable a PEBS on the next
                               occurrence of a PEBS-able event.
                            */
    UINT32 Rsvd36 : 12;

                            /* Bits[47:36], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 OvfPerfMetricsCtr : 1;

                            /* Bits[48:48], Access Type=RW1S, default=0x00000000*/

                            /*
                               This bit is asserted when there is a overflow on
                               any perf_metric counters.
                            */
    UINT32 Rsvd49 : 6;

                            /* Bits[54:49], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 RtitTopaPmi : 1;

                            /* Bits[55:55], Access Type=RW1S, default=0x00000000*/

                            /*
                               RTIT ToPA PMI bit is an indication that a ToPA
                               entry tagged with INTERRUPT was filled up. RTIT
                               continues tracing and no resource/counter will
                               be frozen regardless of
                               IA32_DEBUGCTL.Freeze_Perfmon_on_PMI setting.
                               This bit remains set until cleared by a WRMSR to
                               IA32_PERF_GLOBAL_STATUS_RESET.
                            */
    UINT32 Rsvd56 : 2;

                            /* Bits[57:56], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 LbrsFrozen : 1;

                            /* Bits[58:58], Access Type=RW1S, default=0x00000000*/

                            /*
                               DEBUGCTL.FREEZE_LBRS_ON_PMI is used, and there
                               was PMI HW sets a new bit, called LBRS_FROZEN
                               LBR effective enable become: DEBUGCTL.LBR and
                               ~LBRS_FROZEN
                            */
    UINT32 CtrsFrozen : 1;

                            /* Bits[59:59], Access Type=RW1S, default=0x00000000*/

                            /*
                               DEBUGCTL.FREEZE_PERFMON_ON_PMI is used, and
                               there was PMI ROB sets a new CTRS_FROZEN bit 59
                               in PERF_GLOBAL_STATUS Counter i effective
                               enable: PERF_GLOBAL_CTRL[i] and PerfEvntSeli.EN
                               and ~CTRS_FROZEN
                            */
    UINT32 Interference : 1;

                            /* Bits[60:60], Access Type=RW1S, default=0x00000000*/

                            /*
                               communicate to performance monitoring software
                               that the performance measurements by various
                               counters may not be completely accurate due to
                               interference from some other blinding technology
                               (SE).
                            */
    UINT32 OvfUncore : 1;

                            /* Bits[61:61], Access Type=RW1S, default=0x00000000*/

                            /*
                               Overflow Uncore bit is an indication that there
                               was an overflow of a counter in the UNCORE that
                               was set to cause a PMI. This bit remains set
                               until cleared by IA32_CR_PERF_GLOBAL_RESET. This
                               bit is also routed to the PMI logic. See PMI
                               section.
                            */
    UINT32 OvfPebs : 1;

                            /* Bits[62:62], Access Type=RW1S, default=0x00000000*/

                            /*
                               Overflow PEBS bit is an indication that there
                               was an PEBS overflow of a counter that cause a
                               PMI. This bit remains set until cleared by
                               writing a '0.
                            */
    UINT32 CondChanged : 1;

                            /* Bits[63:63], Access Type=RW1S, default=0x00000000*/

                            /*
                               The Condition Changed bit is set in case of a
                               change to IA32_MISC_ENABLES.PERFMON_AVAILABLE
                               (bit 7). In SNC and on, we can't clear
                               PERFMON_AVAILABLE, thus cond_changed will
                               allways be 0
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_PERF_GLOBAL_STATUS_REGISTER;

/**
  The IA32_CR_PERF_GLOBAL_CTRL MSR is used to globally
  control all the PERFMON counters for a given thread at once.
  This allows instant on/off of all PERFMON counters.
**/
#define MSR_EMON_PERF_GLOBAL_CTRL 0x0000038F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 EnGenCtr : 8;

                            /* Bits[7:0], Access Type=RW, default=0x000000FF*/

                            /*
                               Global Enables for PerfMon Counters 7:0
                               respectively. When the appropriate bit gets set
                               and the local enable of counter is enabled the
                               PerfMon counter will be allowed to counter. If
                               either the global or local enables are not
                               asserted, the counters will be disabled. The
                               global enables allow for simultaneous enabling
                               or disabling of all or some ofthe Core PerfMon
                               counters.
                            */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 EnFixedCtr : 4;

                            /* Bits[35:32], Access Type=RW, default=0x00000000*/

                            /*
                               Global Enable for Fixed Counter 3, 2, 1, 0
                               respecitvely to bit range 35:32. Both this
                               enable and the local enable for the fixed
                               counter needs to be asserted for the fixed
                               counter to be counting.
                            */
    UINT32 Rsvd36 : 12;

                            /* Bits[47:36], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 EnPerfMetricsCtr : 1;

                            /* Bits[48:48], Access Type=RW, default=0x00000000*/

                            /*
                               This is the global enable for the 4 internal
                               perf metric counters.
                            */
    UINT32 Rsvd49 : 15;

                            /* Bits[63:49], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_PERF_GLOBAL_CTRL_REGISTER;

/**
  Global Performance Counter Reset Control
**/
#define MSR_EMON_PERF_GLOBAL_RESET 0x00000390

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ClrOvfGenCtr : 8;

                            /* Bits[7:0], Access Type=WO, default=None*/

                            /*
                               Range 7:0 relates to General 7:0 respecitively.
                               Writes of '1' to any of the bits in this field
                               causes the corresponding bits in the 'Overflow
                               General Counter' field in the
                               IA32_CR_PERF_GLOBAL_STATUS register to clear to
                               '0'.
                            */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ClrOvfFixedCtr : 4;

                            /* Bits[35:32], Access Type=WO, default=None*/

                            /*
                               Range 35:32 relates to Fixed Cntr 3, 2 ,1, 0
                               respecitively. Writes of '1' to any of the bits
                               in this field causes the corresponding bits in
                               the 'Overflow Fixed Counter' field of
                               IA32_CR_PERF_GLOBAL_STATUS to clear to '0'.
                            */
    UINT32 Rsvd36 : 12;

                            /* Bits[47:36], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ClrOvfPerfMetricsCtr : 1;

                            /* Bits[48:48], Access Type=WO, default=None*/

                            /*
                               Write 1'b1 to this bit will clear the proper
                               ovf_perm_metrics_ctr bit in perf_global_status.
                            */
    UINT32 Rsvd49 : 6;

                            /* Bits[54:49], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ClrRtitTopaPmi : 1;

                            /* Bits[55:55], Access Type=WO, default=None*/

                            /*
                               Writes of '1' to this bit causes the
                               'rtit_topa_pmi' bit in the global status
                               register to clear to '0'.
                            */
    UINT32 Rsvd56 : 2;

                            /* Bits[57:56], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ClrLbrsFrozen : 1;

                            /* Bits[58:58], Access Type=WO, default=None*/

                            /*
                               Writes of '1' to this bit causes the
                               'lbrs_frozen' bit in the global status register
                               to clear to '0'.
                            */
    UINT32 ClrCtrsFrozen : 1;

                            /* Bits[59:59], Access Type=WO, default=None*/

                            /*
                               Writes of '1' to this bit causes the
                               'ctrs_frozen' bit in the global status register
                               to clear to '0'.
                            */
    UINT32 ClrInterference : 1;

                            /* Bits[60:60], Access Type=WO, default=None*/

                            /*
                               Writes of '1' to this bit causes the
                               'interference' bit in the global status register
                               to clear to '0'.
                            */
    UINT32 ClrOvfUncore : 1;

                            /* Bits[61:61], Access Type=WO, default=None*/

                            /*
                               Writes of '1' to this bit causes the 'Overflow
                               UnCore' bit in the global status register to
                               clear to '0'.
                            */
    UINT32 ClrOvfPebs : 1;

                            /* Bits[62:62], Access Type=WO, default=None*/

                            /*
                               Writes of '1' to this bit causes the 'Overflow
                               pebs' bit in the global status register to clear
                               to '0'.
                            */
    UINT32 ClrCondChanged : 1;

                            /* Bits[63:63], Access Type=WO, default=None*/

                            /*
                               Writes of '1' to this bit causes the 'Condition
                               Change' bit (bit 63) in the
                               IA32_CR_PERF_GLOBAL_STATUS to clear to '0'.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_PERF_GLOBAL_RESET_REGISTER;

/**
  The Core PerfMon global overflow set MSR is writable only command
  register. A read from this register will always return zeroes. A write
  of '1' to any of the bits in this register causes the corresponding bit
  in the IA32_CR_PERF_GLOBAL_STATUS register to set to '1'.
**/
#define MSR_EMON_PERF_GLOBAL_SET 0x00000391

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 SetOvfGenCtr : 8;

                            /* Bits[7:0], Access Type=WO, default=None*/

                            /*
                               Range 7:0 relates to General 7-0 respecitively.
                               Writes of '1' to any of the bits in this field
                               causes the corresponding bits in the 'Overflow
                               General Counter' field in the
                               IA32_CR_PERF_GLOBAL_STATUS register to set to
                               '1'.
                            */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 SetOvfFixedCtr : 4;

                            /* Bits[35:32], Access Type=WO, default=None*/

                            /*
                               Range 35:32 relates to Fixed Cntr 3, 2 ,1, 0
                               respecitively. Writes of '1' to any of the bits
                               in this field causes the corresponding bits in
                               the 'Overflow Fixed Counter' field of
                               IA32_CR_PERF_GLOBAL_STATUS to set to '1'.
                            */
    UINT32 Rsvd36 : 12;

                            /* Bits[47:36], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 SetOvfPerfMetricsCtr : 1;

                            /* Bits[48:48], Access Type=WO, default=None*/

                            /*
                               Write to this bit will set the ovf_status to
                               1'b1
                            */
    UINT32 Rsvd49 : 6;

                            /* Bits[54:49], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 SetRtitTopaPmi : 1;

                            /* Bits[55:55], Access Type=WO, default=0x00000000*/

                            /*
                               RTIT ToPA PMI bit is an indication that a ToPA
                               entry tagged with INTERRUPT was filled up. RTIT
                               continues tracing and no resource/counter will
                               be frozen regardless of
                               IA32_DEBUGCTL.Freeze_Perfmon_on_PMI setting.
                               wrmsr of this bit sets the corresponding bit in
                               IA32_PERF_GLOBAL_STATUS_RESET.
                            */
    UINT32 Rsvd56 : 2;

                            /* Bits[57:56], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 SetLbrsFrozen : 1;

                            /* Bits[58:58], Access Type=WO, default=None*/

                            /*
                               Writes of '1' to this bit causes the
                               'clr_lbrs_frozen' bit in the global status
                               register to set to '1'.
                            */
    UINT32 SetCtrsFrozen : 1;

                            /* Bits[59:59], Access Type=WO, default=None*/

                            /*
                               Writes of '1' to this bit causes the
                               'clr_ctrs_frozen' bit in the global status
                               register to set to '1'.
                            */
    UINT32 SetInterference : 1;

                            /* Bits[60:60], Access Type=WO, default=None*/

                            /*
                               Writes of '1' to this bit causes the
                               'clr_interference' bit in the global status
                               register to set to '1'.
                            */
    UINT32 SetOvfUncore : 1;

                            /* Bits[61:61], Access Type=WO, default=None*/

                            /*
                               Writes of '1' to this bit causes the 'Overflow
                               UnCore' bit in the global status register to set
                               to '1'.
                            */
    UINT32 SetOvfPebs : 1;

                            /* Bits[62:62], Access Type=WO, default=None*/

                            /*
                               Writes of '1' to this bit causes the 'Overflow
                               pebs' bit in the global status register to set
                               to '1'.
                            */
    UINT32 SetCondChanged : 1;

                            /* Bits[63:63], Access Type=WO, default=None*/

                            /*
                               Writes of '1' to this bit causes the 'Condition
                               Change' bit (bit 63) in the
                               IA32_CR_PERF_GLOBAL_STATUS to set to '1'.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_PERF_GLOBAL_SET_REGISTER;

/**
  Precise Event Based Sampling controlling register
**/
#define MSR_PEBS_ENABLE 0x000003F1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 CntrEnable : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Set to enable the general PEBS counters */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 FixedCntrEnable : 4;

                            /* Bits[35:32], Access Type=RW, default=0x00000000*/

                            /*
                               Set to enable the fixed counters 3:0 PEBS
                               counters
                            */
    UINT32 Rsvd36 : 28;

                            /* Bits[63:36], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PEBS_ENABLE_REGISTER;

/**
  PEBS DATA CFG
**/
#define MSR_PEBS_DATA_CFG 0x000003F2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 MemoryInfo : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Memory info enable bit */
    UINT32 GprRegs : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* gpr regs enable bit */
    UINT32 XmmRegs : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* xmm regs enable bit */
    UINT32 Lbrs : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* lbrs enable bit */
    UINT32 Rsvd4 : 20;

                            /* Bits[23:4], Access Type=RW, default=0x00000000*/

                            /* reserved bits */
    UINT32 NumLbrs : 5;

                            /* Bits[28:24], Access Type=RW, default=0x00000000*/

                            /* Num lbrs bits */
    UINT32 Rsvd29 : 3;

                            /* Bits[31:29], Access Type=RW, default=0x00000000*/

                            /* reserved bits */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RW, default=0x00000000*/

                            /* reserved bits */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PEBS_DATA_CFG_REGISTER;

/**
  Machine Check Control Register
**/
#define MSR_MC0_CTL 0x00000400

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 EnableRob : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable ROB MCA Logging */
    UINT32 EnableMs : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable MS MCA Logging */
    UINT32 EnableIq : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enable IQ MCA Logging */
    UINT32 EnableIfu : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable IFU MCA Logging */
    UINT32 EnableDsb : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Enable DSB MCA Logging */
    UINT32 EnableIdq : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Enable IDQ MCA Logging */
    UINT32 EnableUrom : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Enable UROM MCA Logging */
    UINT32 EnableBit : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Enable BIT MCA Logging */
    UINT32 Reserved8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Reserved bit previously used for SCP parity in
                               FIT but no longer needed
                            */
    UINT32 EnableSdb : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* Enable SDB MCA Logging */
    UINT32 EnableExe : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Enable EXE MCA Logging */
    UINT32 EnableRs : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Enable RS MCA Logging */
    UINT32 Rsvd12 : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC0_CTL_REGISTER;

/**
  Machine Check Status Register
**/
#define MSR_MC0_STATUS 0x00000401

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Mcacod : 11;

                            /* Bits[10:0], Access Type=RW_V, default=None*/

                            /* Machine Check Architecture Error Code */
    UINT32 EnhMcaAvail0 : 5;

                            /* Bits[15:11], Access Type=RW_V, default=None*/

                            /* Reserved bits */
    UINT32 Mscod : 4;

                            /* Bits[19:16], Access Type=RW_V, default=None*/

                            /* Machine Specific Architecture Error Code */
    UINT32 EnhMcaAvail10 : 12;

                            /* Bits[31:20], Access Type=RW_V, default=None*/

                            /* Reserved bits */
    UINT32 EnhMcaAvail11 : 6;

                            /* Bits[37:32], Access Type=RW_V, default=None*/

                            /* Reserved bits */
    UINT32 CorrectedErrCnt : 14;

                            /* Bits[51:38], Access Type=RW_V, default=None*/

                            /*
                               Corrected error count since last clear of mc
                               regs
                            */
    UINT32 Sticky : 1;

                            /* Bits[52:52], Access Type=RW_V, default=None*/

                            /* Sticky bit set when error count overflows */
    UINT32 EnhMcaAvail2 : 2;

                            /* Bits[54:53], Access Type=RW_V, default=None*/

                            /* Reserved bits */
    UINT32 Ar : 1;

                            /* Bits[55:55], Access Type=RW_V, default=None*/

                            /* Action required */
    UINT32 S : 1;

                            /* Bits[56:56], Access Type=RW_V, default=None*/

                            /* Signaling */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RW_V, default=None*/

                            /* Processor Context Corrupt */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RW_V, default=None*/

                            /* MC0_ADDR register valid */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RW_V, default=None*/

                            /* MC0_MISC register valid */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RW_V, default=None*/

                            /* Error Enabled */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RW_V, default=None*/

                            /* Uncorrectable Error */
    UINT32 Over : 1;

                            /* Bits[62:62], Access Type=RW_V, default=None*/

                            /* Machine Check Overflow */
    UINT32 Val : 1;

                            /* Bits[63:63], Access Type=RW_V, default=None*/

                            /* Status register Valid */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC0_STATUS_REGISTER;

/**
  Machine Check Address Register
**/
#define MSR_MC0_ADDR 0x00000402

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Addr0 : 32;

                            /* Bits[31:0], Access Type=RW_V, default=None*/

                            /* LIP of access */
    UINT32 Addr1 : 26;

                            /* Bits[57:32], Access Type=RW_V, default=None*/

                            /* LIP of access */
    UINT32 EnhMcaAvail0 : 6;

                            /* Bits[63:58], Access Type=RW_V, default=None*/

                            /* Reserved bits */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC0_ADDR_REGISTER;

/**
  Machine Check Misc Register
**/
#define MSR_MC0_MISC 0x00000403

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 IfuAddrLsb : 6;

                            /* Bits[5:0], Access Type=RW, default=None*/

                            /*
                               address mode (physical,linear,segment
                               offset,memory,generic)
                            */
    UINT32 AddressMode : 3;

                            /* Bits[8:6], Access Type=RW, default=None*/

                            /*
                               address mode (physical,linear,segment
                               offset,memory,generic)
                            */
    UINT32 EnhMcaAvail00 : 23;

                            /* Bits[31:9], Access Type=RW, default=None*/

                            /* Reserved bits */
    UINT32 EnhMcaAvail01 : 32;

                            /* Bits[63:32], Access Type=RW, default=None*/

                            /* Reserved bits */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC0_MISC_REGISTER;

/**
  Machine Check Control Register
**/
#define MSR_MC1_CTL 0x00000404

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 En : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Enable or Disable all error signaling in the MC
                               bank1
                            */
    UINT32 Rsvd1 : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC1_CTL_REGISTER;

/**
  Machine Check Status Register
**/
#define MSR_MC1_STATUS 0x00000405

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Mcacod : 16;

                            /* Bits[15:0], Access Type=RW_V, default=None*/

                            /*
                               Machine Check Architecture Error Code: Ld,St,
                               Apic, Eviction, Posion or Snoop Error
                            */
    UINT32 Mscod0 : 16;

                            /* Bits[31:16], Access Type=RW_V, default=None*/

                            /* Model Specific Error Code. */
    UINT32 Mscod1 : 6;

                            /* Bits[37:32], Access Type=RW_V, default=None*/

                            /* Model Specific Error Code. */
    UINT32 Cecnt : 15;

                            /* Bits[52:38], Access Type=RW_V, default=None*/

                            /*
                               This field increments for every correctable
                               error detected in the DCU even if an uncorrected
                               error is already logged. If the counter
                               overflows the bit 52 of this register will be
                               sticky. This bit will only cleared by software.
                            */
    UINT32 EnhMcaAvail : 2;

                            /* Bits[54:53], Access Type=RW_V, default=None*/

                            /* Available when Enhnaced MCA is in use */
    UINT32 Ar : 1;

                            /* Bits[55:55], Access Type=RW_V, default=None*/

                            /*
                               AR stands for Action Required. When set it means
                               that error logged requires handling by SW before
                               it can resume execution otherwise it will have
                               to fail if it is not able to handle it. When
                               clear it means that error recovery action is
                               optional for the SW and if it can not handle it
                               there is no need to bring down the sytem.
                            */
    UINT32 S : 1;

                            /* Bits[56:56], Access Type=RW_V, default=None*/

                            /*
                               S bit: When set it means that the error logged
                               was signaled via MCE. Clear means that the error
                               was signled via CMCI.
                            */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RW_V, default=None*/

                            /*
                               Processor Context Corrupt indicates if machine
                               state damage has occurred as a result of the
                               error.
                            */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RW_V, default=None*/

                            /*
                               Indicates if there is an address associated with
                               the reported error.
                            */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RW_V, default=None*/

                            /*
                               Indicates if the MC1_MISC register contains
                               valid data
                            */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RW_V, default=None*/

                            /* The enabled error bit. */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RW_V, default=None*/

                            /*
                               The uncorrected error bit. It indicates that the
                               error is uncorrected when set.
                            */
    UINT32 O : 1;

                            /* Bits[62:62], Access Type=RW_V, default=None*/

                            /*
                               The overflow bit. It indicates that an error has
                               occurred while the valid bit is set.
                            */
    UINT32 V : 1;

                            /* Bits[63:63], Access Type=RW_V, default=None*/

                            /*
                               The valid bit. It indicates if the error
                               information in this register is Valid ('1) or
                               Invalid ('0). This bit is set by hardware when
                               an error (correctable or uncorrectable) is
                               written into the register. It is cleared only by
                               software.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC1_STATUS_REGISTER;

/**
  Machine Check Address Register
**/
#define MSR_MC1_ADDR 0x00000406

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Low : 32;

                            /* Bits[31:0], Access Type=RW_V, default=None*/

                            /*
                               Lower physical address of a uop which either a
                               correctable/uncorrectable parity err
                            */
    UINT32 High : 20;

                            /* Bits[51:32], Access Type=RW_V, default=None*/

                            /*
                               Upper physical address of a uop which either a
                               correctable/uncorrectable parity err
                            */
    UINT32 EnhMcaAvail : 12;

                            /* Bits[63:52], Access Type=RW_V, default=None*/

                            /*
                               Available to software when Enhnaced MCA is in
                               use
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC1_ADDR_REGISTER;

/**
  Machine Check Status Register
**/
#define MSR_MC1_MISC 0x00000407

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Recaddrlsb : 6;

                            /* Bits[5:0], Access Type=RW_V, default=None*/

                            /*
                               OS should treat Machine check address from
                               resolution of LSB (ignore LSB-1:0 bits of given
                               address in MC0_ADDR)
                            */
    UINT32 Addrmode : 3;

                            /* Bits[8:6], Access Type=RW_V, default=None*/

                            /*
                               OS should treat Machine check address from
                               resolution of LSB (ignore LSB-1:0 bits of given
                               address in MC0_ADDR)
                            */
    UINT32 EnhMcaAvail0 : 23;

                            /* Bits[31:9], Access Type=RW_V, default=None*/

                            /*
                               Available to software when Enhnaced MCA is in
                               use.
                            */
    UINT32 EnhMcaAvail1 : 32;

                            /* Bits[63:32], Access Type=RW_V, default=None*/

                            /*
                               Available to software when Enhnaced MCA is in
                               use.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC1_MISC_REGISTER;

/**
  Machine Check Control Register
**/
#define MSR_MC2_CTL 0x00000408

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 EnDtlb : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 EnFscp : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 EnSrf : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 Rsvd3 : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC2_CTL_REGISTER;

/**
  DTLB Machine Check Status register
**/
#define MSR_MC2_STATUS 0x00000409

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Mcacod : 16;

                            /* Bits[15:0], Access Type=RW_V, default=None*/

                            /* Model Specific Error Code */
    UINT32 Mscod : 16;

                            /* Bits[31:16], Access Type=RW_V, default=None*/

                            /*
                               Error type - Dat-parity = 0x0001 Tag-parity =
                               0x0000
                            */
    UINT32 EnhMcaAvail0 : 6;

                            /* Bits[37:32], Access Type=RW_V, default=None*/

                            /* Available when Enhanced MCA is in use */
    UINT32 CorErrCnt : 15;

                            /* Bits[52:38], Access Type=RW_V, default=None*/

                            /* Correctable error counter */
    UINT32 EnhMcaAvail1 : 4;

                            /* Bits[56:53], Access Type=RW_V, default=None*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RW_V, default=None*/

                            /*
                               Processor Context Corrupt - State damage has
                               occurred
                            */
    UINT32 Adrv : 1;

                            /* Bits[58:58], Access Type=RW_V, default=None*/

                            /*
                               MCi_ADDR register is valid for signalled MCA
                               error
                            */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RW_V, default=None*/

                            /*
                               MCi_MISC register is valid for signalled MCA
                               error
                            */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RW_V, default=None*/

                            /* Error Enabled */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RW_V, default=None*/

                            /* Uncorrectable Error */
    UINT32 Over : 1;

                            /* Bits[62:62], Access Type=RW_V, default=None*/

                            /*
                               Set if MCA error detected and valid bit already
                               set for previous error
                            */
    UINT32 Valid : 1;

                            /* Bits[63:63], Access Type=RW_V, default=None*/

                            /* Set if MCA error detected */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC2_STATUS_REGISTER;
/**
  Machine Check Address Register
**/
#define MSR_MC2_ADDR 0x0000040A
/**
  Machine Check Misc Register
**/
#define MSR_MC2_MISC 0x0000040B

/**
  Machine Check Control Register
**/
#define MSR_MC3_CTL 0x0000040C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 MlcTagError : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable machine check signaling */
    UINT32 MlcSqdbIdiParError : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable machine check signaling */
    UINT32 MlcDataError : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enable machine check signaling */
    UINT32 MlcMesiStateError : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable machine check signaling */
    UINT32 MlcDpfPoisonError : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Enable machine check signaling */
    UINT32 MlcIdiAddrParError : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Enable machine check signaling */
    UINT32 Rsvd6 : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC3_CTL_REGISTER;

/**
  Machine Check Status Register
**/
#define MSR_MC3_STATUS 0x0000040D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Mcacod : 16;

                            /* Bits[15:0], Access Type=RW_V, default=None*/

                            /* Machine Check Architecture Error Code */
    UINT32 Mscod : 16;

                            /* Bits[31:16], Access Type=RW_V, default=None*/

                            /* Model Specific Error Code */
    UINT32 EnhMcaAvail0 : 6;

                            /* Bits[37:32], Access Type=RW_V, default=None*/

                            /* Available when Enhnaced MCA is in use */
    UINT32 Cec : 15;

                            /* Bits[52:38], Access Type=RW_V, default=None*/

                            /* Correctable Error Count */
    UINT32 BitfixAllocated : 1;

                            /* Bits[53:53], Access Type=RW_V, default=None*/

                            /*
                               Status tracking is Green if this bit is set when
                               a logged error is allocated into bitfix CAM
                            */
    UINT32 BitfixOvercapacity : 1;

                            /* Bits[54:54], Access Type=RW_V, default=None*/

                            /*
                               Status tracking is Yellow if this bit is set
                               when a logged error overflows bitfix CAM (and
                               bitfixing is enabled)
                            */
    UINT32 EnhMcaAvail1 : 2;

                            /* Bits[56:55], Access Type=RW_V, default=None*/

                            /* Available when Enhnaced MCA is in use */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RW_V, default=None*/

                            /* Processor Context Corrupt */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RW_V, default=None*/

                            /* MC3_ADDR register valid */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RW_V, default=None*/

                            /* MC3_MISC register valid */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RW_V, default=None*/

                            /* Error Enabled */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RW_V, default=None*/

                            /* Uncorrectable Error */
    UINT32 Over : 1;

                            /* Bits[62:62], Access Type=RW_V, default=None*/

                            /* Machine Check Overflow */
    UINT32 Val : 1;

                            /* Bits[63:63], Access Type=RW_V, default=None*/

                            /* Status register Valid */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC3_STATUS_REGISTER;

/**
  Machine Check Address Register
**/
#define MSR_MC3_ADDR 0x0000040E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Addr0 : 32;

                            /* Bits[31:0], Access Type=RW_V, default=None*/

                            /* Address of the request that got an error */
    UINT32 Addr1 : 20;

                            /* Bits[51:32], Access Type=RW_V, default=None*/

                            /* Address of the request that got an error */
    UINT32 EnhMcaAvail : 12;

                            /* Bits[63:52], Access Type=RW_V, default=None*/

                            /* Available when Enhnaced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC3_ADDR_REGISTER;

/**
  Machine Check Misc Register
**/
#define MSR_MC3_MISC 0x0000040F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Recaddrlsb : 6;

                            /* Bits[5:0], Access Type=RW_V, default=None*/

                            /* Const: 000101 */
    UINT32 Addrmode : 3;

                            /* Bits[8:6], Access Type=RW_V, default=None*/

                            /* Const: 010 */
    UINT32 Opcode : 7;

                            /* Bits[15:9], Access Type=RW_V, default=None*/

                            /* Opcode */
    UINT32 Sqid : 5;

                            /* Bits[20:16], Access Type=RW_V, default=None*/

                            /* SQID */
    UINT32 Way : 5;

                            /* Bits[25:21], Access Type=RW_V, default=None*/

                            /* Way */
    UINT32 Thread : 1;

                            /* Bits[26:26], Access Type=RW_V, default=None*/

                            /* Thread */
    UINT32 EnhMcaAvail0 : 5;

                            /* Bits[31:27], Access Type=RW_V, default=None*/

                            /* Available when Enhnaced MCA is in use */
    UINT32 EnhMcaAvail1 : 32;

                            /* Bits[63:32], Access Type=RW_V, default=None*/

                            /* Available when Enhnaced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC3_MISC_REGISTER;

/**
  MEE and Ucode/Xucode bank: Control register
**/
#define MSR_MC4_CTL 0x00000410

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Ee00 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               LLC: Uncorrectable tag error flag Enable
                               signaling of Uncorrectable Tag error which
                               includes : Tag/MESI/CV/GFDT
                            */
    UINT32 Ee01 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Flag enables signaling of all errors by MCA
                               message
                            */
    UINT32 Rsvd2 : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC4_CTL_REGISTER;

/**
  MEE and Ucode/Xucode MCA bank: Status register
**/
#define MSR_MC4_STATUS 0x00000411

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Mcacod : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /* MCA error code */
    UINT32 Mscod : 16;

                            /* Bits[31:16], Access Type=RWS_V, default=0x00000000*/

                            /* Model specific error code */
    UINT32 OtherInfo : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* Other info */
    UINT32 CorrectedErrorCount : 15;

                            /* Bits[52:38], Access Type=RWS_V, default=0x00000000*/

                            /* Correctable error count */
    UINT32 ThresholdBasedErrorStatus : 2;

                            /* Bits[54:53], Access Type=RWS_V, default=0x00000000*/

                            /*
                               These bits are used to indicate when the number
                               of corrected errors has exceeded the safe
                               threshold to the point where an uncorrected
                               error has become more likely to happen.
                            */
    UINT32 Ar : 1;

                            /* Bits[55:55], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 S : 1;

                            /* Bits[56:56], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RWS_V, default=0x00000000*/

                            /* PCC: Processor context corrupt */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RWS_V, default=0x00000000*/

                            /*
                               ADDRV: The address held in the MC4_ADDR_MSR is
                               valid
                            */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RWS_V, default=0x00000000*/

                            /*
                               MISCV: The value held in the MC4_MISC_MSR is
                               valid
                            */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RWS_V, default=0x00000000*/

                            /*
                               EN: Error enabled flag Indicates (when set) that
                               signaling of the machine check exception for
                               this error was enabled by an associated flag bit
                               of the Mci_CTL register.
                            */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RWS_V, default=0x00000000*/

                            /*
                               UC: Error uncorrected flag Indicates (when set)
                               that the processor did not correct the error
                               condition. When clear, this flag indicates that
                               the processor was able to correct the error
                               condition.
                            */
    UINT32 Over : 1;

                            /* Bits[62:62], Access Type=RWS_V, default=0x00000000*/

                            /* OVER: Machine check overflow */
    UINT32 Val : 1;

                            /* Bits[63:63], Access Type=RWS_V, default=0x00000000*/

                            /*
                               VAL: Valid flag Indicates (when set) that the
                               information within the Mci_STATUS register is
                               valid. When this flag is set, the processor
                               follows the rules given for the OVER flag in the
                               Mci_STATUS register when overwriting previously
                               valid entries. The processor sets the VAL flag
                               and software is responsible for clearing it.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC4_STATUS_REGISTER;
/**
  MEE and Ucode/Xucode bank: Address register
**/
#define MSR_MC4_ADDR 0x00000412

/**
  MMEE and Ucode/Xucode bank: Misc register
**/
#define MSR_MC4_MISC 0x00000413

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 RecoverableAddressLsb : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /* Points to where the supported LSB in MCi_Addr */
    UINT32 AddressMode : 3;

                            /* Bits[8:6], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Address Mode = 010 since we always have only PA
                               in the SNB LLC
                            */
    UINT32 ModelSpecifcInfoEntry : 10;

                            /* Bits[18:9], Access Type=RWS_V, default=0x00000000*/

                            /*
                               The cache/TLC entry associated with the looged
                               error
                            */
    UINT32 ModelSpecifcInfoBank : 3;

                            /* Bits[21:19], Access Type=RWS_V, default=0x00000000*/

                            /* The cache bank associated with the looged error */
    UINT32 ModelSpecifcInfoSource : 1;

                            /* Bits[22:22], Access Type=RWS_V, default=0x00000000*/

                            /*
                               The ECC source of failure (Cache ECC failure =
                               0, TLC ECC failure = 1)
                            */
    UINT32 ModelSpecifcInfo0 : 9;

                            /* Bits[31:23], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 ModelSpecifcInfo1 : 32;

                            /* Bits[63:32], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC4_MISC_REGISTER;

/**
  MEE1 bank: Control register
**/
#define MSR_MC5_CTL 0x00000414

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Ee00 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* MC bank enable bit */
    UINT32 Ee01 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Flag enables signaling of all errors by MCA
                               message
                            */
    UINT32 Rsvd2 : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC5_CTL_REGISTER;

/**
  MEE1 bank: Status register
**/
#define MSR_MC5_STATUS 0x00000415

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Mcacod : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /* MCA error code */
    UINT32 Mscod : 16;

                            /* Bits[31:16], Access Type=RWS_V, default=0x00000000*/

                            /* Model specific error code */
    UINT32 OtherInfo : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* Other info */
    UINT32 CorrectedErrorCount : 15;

                            /* Bits[52:38], Access Type=RWS_V, default=0x00000000*/

                            /* Correctable error count */
    UINT32 ThresholdBasedErrorStatus : 2;

                            /* Bits[54:53], Access Type=RWS_V, default=0x00000000*/

                            /*
                               These bits are used to indicate when the number
                               of corrected errors has exceeded the safe
                               threshold to the point where an uncorrected
                               error has become more likely to happen.
                            */
    UINT32 Ar : 1;

                            /* Bits[55:55], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 S : 1;

                            /* Bits[56:56], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RWS_V, default=0x00000000*/

                            /* PCC: Processor context corrupt */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RWS_V, default=0x00000000*/

                            /*
                               ADDRV: The address held in the MC5_ADDR_MSR is
                               valid
                            */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RWS_V, default=0x00000000*/

                            /*
                               MISCV: The value held in the MC5_MISC_MSR is
                               valid
                            */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RWS_V, default=0x00000000*/

                            /*
                               EN: Error enabled flag Indicates (when set) that
                               signaling of the machine check exception for
                               this error was enabled by an associated flag bit
                               of the Mci_CTL register.
                            */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RWS_V, default=0x00000000*/

                            /*
                               UC: Error uncorrected flag Indicates (when set)
                               that the processor did not correct the error
                               condition. When clear, this flag indicates that
                               the processor was able to correct the error
                               condition.
                            */
    UINT32 Over : 1;

                            /* Bits[62:62], Access Type=RWS_V, default=0x00000000*/

                            /* OVER: Machine check overflow */
    UINT32 Val : 1;

                            /* Bits[63:63], Access Type=RWS_V, default=0x00000000*/

                            /*
                               VAL: Valid flag Indicates (when set) that the
                               information within the Mci_STATUS register is
                               valid. When this flag is set, the processor
                               follows the rules given for the OVER flag in the
                               Mci_STATUS register when overwriting previously
                               valid entries. The processor sets the VAL flag
                               and software is responsible for clearing it.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC5_STATUS_REGISTER;
/**
  MEE1 bank: Address register
**/
#define MSR_MC5_ADDR 0x00000416

/**
  MMEE1 bank: Misc register
**/
#define MSR_MC5_MISC 0x00000417

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 RecoverableAddressLsb : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /* Points to where the supported LSB in MCi_Addr */
    UINT32 AddressMode : 3;

                            /* Bits[8:6], Access Type=RWS_V, default=0x00000000*/

                            /* Address Mode */
    UINT32 ModelSpecifcInfoEntry : 10;

                            /* Bits[18:9], Access Type=RWS_V, default=0x00000000*/

                            /*
                               The cache/TLC entry associated with the looged
                               error
                            */
    UINT32 ModelSpecifcInfoBank : 3;

                            /* Bits[21:19], Access Type=RWS_V, default=0x00000000*/

                            /* The cache bank associated with the looged error */
    UINT32 ModelSpecifcInfoSource : 1;

                            /* Bits[22:22], Access Type=RWS_V, default=0x00000000*/

                            /*
                               The ECC source of failure (Cache ECC failure =
                               0, TLC ECC failure = 1)
                            */
    UINT32 ModelSpecifcInfo0 : 9;

                            /* Bits[31:23], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 ModelSpecifcInfo1 : 32;

                            /* Bits[63:32], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC5_MISC_REGISTER;

/**
  Machine Check Architecture Control MSR.
**/
#define MSR_MC6_CTL 0x00000418

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 EnPcuHwError : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* Enable bit for MC signaling */
    UINT32 EnPcuUcError : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000001*/

                            /* Enable bit for MC signaling */
    UINT32 EnPcuFwError : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /* Enable bit for MC signaling */
    UINT32 EnVcuHwError : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /* Enable bit for MC signaling */
    UINT32 EnVcuUcError : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /* Enable bit for MC signaling */
    UINT32 EnVcuFwError : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /* Enable bit for MC signaling */
    UINT32 Rsvd6 : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC6_CTL_REGISTER;

/**
  Machine Check Architecture Status MSR.
**/
#define MSR_MC6_STATUS 0x00000419

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Mccod : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /* Machine Check Error Code. */
    UINT32 MsecUc : 4;

                            /* Bits[19:16], Access Type=RWS_V, default=0x00000000*/

                            /* Model specific error code bits 19:16 */
    UINT32 MsecHw : 4;

                            /* Bits[23:20], Access Type=RWS_V, default=0x00000000*/

                            /* Model specific error code bits 23:20. */
    UINT32 MsecFw : 8;

                            /* Bits[31:24], Access Type=RWS_V, default=0x00000000*/

                            /* Model specific error code bits 31:24. */
    UINT32 EnhMcaAvail0 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 CorrErrCount : 15;

                            /* Bits[52:38], Access Type=RWS_V, default=0x00000000*/

                            /* Correctable error count. */
    UINT32 Correrrorstatusind : 2;

                            /* Bits[54:53], Access Type=RWS_V, default=0x00000000*/

                            /*
                               These bits are used to indicate when the number
                               of corrected errors has exceeded the safe
                               threshold to the point where an uncorrected
                               error has become more likely to happen. Table 3
                               shows the encoding of these bits.
                            */
    UINT32 EnhMcaAvail1 : 2;

                            /* Bits[56:55], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RWS_V, default=0x00000000*/

                            /* PCC: Processor context corrupt */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RWS_V, default=0x00000000*/

                            /*
                               ADDRV: The address held in the MC6_ADDR_MSR is
                               valid
                            */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RWS_V, default=0x00000000*/

                            /*
                               MISCV: The value held in the MC6_MISC_MSR is
                               valid
                            */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Enable bit - Whether the logged error was
                               enabled or not.
                            */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Indicates (when set) that the processor did not
                               correct the error condition. 0b Correctable
                               Error 1b Uncorrectable Error
                            */
    UINT32 Ovr : 1;

                            /* Bits[62:62], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Another MCA error was detected when VALID was
                               already set to 1b. In other words, a second MCA
                               error was detected while an earlier error was
                               already logged.
                            */
    UINT32 Valid : 1;

                            /* Bits[63:63], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Indicates that this register contains a valid
                               MCA error that was detected.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC6_STATUS_REGISTER;
/**
  PCU Machine Check Architecture Address MSR.
**/
#define MSR_MC6_ADDR 0x0000041A

/**
  PCU Machine Check Architecture Misc MSR.
**/
#define MSR_MC6_MISC 0x0000041B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ErrorAddress : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               This field contains the address of uncorrected
                               error seen in the PCU/VCU. It is updated on an
                               uncorrectable error. To maintain e-MCA support
                               this register must be writable.
                            */
    UINT32 CorrectedErrorCount : 8;

                            /* Bits[23:16], Access Type=RWS_V, default=0x00000000*/

                            /* Corrected Error Count */
    UINT32 EnhMcaAvail0 : 8;

                            /* Bits[31:24], Access Type=RWS_V, default=0x00000000*/

                            /* Available when enhanced MCA is in use */
    UINT32 EnhMcaAvail1 : 32;

                            /* Bits[63:32], Access Type=RWS_V, default=0x00000000*/

                            /* Available when enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC6_MISC_REGISTER;

/**
  CBO and LLC MCA bank: Control register
**/
#define MSR_MC7_CTL 0x0000041C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LlcTagError : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 LlcDataError : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadErrWbToMmio : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadErrIaAccessToGsm : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadCorrupingErrOther : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadNonCorrupingErrOther : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 MultipleTagMatchErr : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 IdiJitterError : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC7_CTL_REGISTER;

/**
  LLC MCA bank: Status register
**/
#define MSR_MC7_STATUS 0x0000041D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 McaErrorCode : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /* MCA error code */
    UINT32 ModelSpecificErrorCode : 16;

                            /* Bits[31:16], Access Type=RWS_V, default=0x00000000*/

                            /* Model specific error code */
    UINT32 EnhMcaAvail0 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 CorrErrCount : 15;

                            /* Bits[52:38], Access Type=RWS_V, default=0x00000000*/

                            /* Correctable error count */
    UINT32 Correrrorstatusind : 2;

                            /* Bits[54:53], Access Type=RWS_V, default=0x00000001*/

                            /*
                               These bits are used to indicate when the number
                               of corrected errors has exceeded the safe
                               threshold to the point where an uncorrected
                               error has become more likely to happen. Table 3
                               shows the encoding of these bits.
                            */
    UINT32 EnhMcaAvail1 : 2;

                            /* Bits[56:55], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RWS_V, default=0x00000000*/

                            /* PCC: Processor context corrupt */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RWS_V, default=0x00000000*/

                            /*
                               ADDRV: The address held in the MC6_ADDR_MSR is
                               valid
                            */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RWS_V, default=0x00000000*/

                            /*
                               MISCV: The value held in the MC6_MISC_MSR is
                               valid
                            */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RWS_V, default=0x00000000*/

                            /*
                               EN: Error enabled flag Indicates (when set) that
                               signaling of the machine check exception for
                               this error was enabled by an associated flag bit
                               of the Mci_CTL register.
                            */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RWS_V, default=0x00000000*/

                            /*
                               UC: Error uncorrected flag Indicates (when set)
                               that the processor did not correct the error
                               condition. When clear, this flag indicates that
                               the processor was able to correct the error
                               condition.
                            */
    UINT32 Over : 1;

                            /* Bits[62:62], Access Type=RWS_V, default=0x00000000*/

                            /* OVER: Machine check overflow */
    UINT32 Val : 1;

                            /* Bits[63:63], Access Type=RWS_V, default=0x00000000*/

                            /*
                               VAL: Valid flag Indicates (when set) that the
                               information within the Mci_STATUS register is
                               valid. When this flag is set, the processor
                               follows the rules given for the OVER flag in the
                               Mci_STATUS register when overwriting previously
                               valid entries. The processor sets the VAL flag
                               and software is responsible for clearing it.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC7_STATUS_REGISTER;

/**
  LLC MCA bank: Address register
**/
#define MSR_MC7_ADDR 0x0000041E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 AddressOffset : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Bits [5:0] of the Machine Check logged address.
                               Enchanced MCA feature made this field writable
                            */
    UINT32 Address0 : 26;

                            /* Bits[31:6], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 Address1 : 7;

                            /* Bits[38:32], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 EnhMcaAvail : 25;

                            /* Bits[63:39], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC7_ADDR_REGISTER;

/**
  LLC MCA bank: Misc register
**/
#define MSR_MC7_MISC 0x0000041F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LsbAddr : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Points to where the supported LSB in MCi_Addr in
                               SNB LLC cases it's always 6 (=000110)
                            */
    UINT32 Addrmode : 3;

                            /* Bits[8:6], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Address Mode = 010 since we always have only PA
                               in the SNB LLC
                            */
    UINT32 EnhMcaAvail0 : 5;

                            /* Bits[13:9], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Way : 4;

                            /* Bits[17:14], Access Type=RWS_V, default=0x00000000*/

                            /* The way associated with the logged error */
    UINT32 EnhMcaAvail1 : 6;

                            /* Bits[23:18], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Threadid : 1;

                            /* Bits[24:24], Access Type=RWS_V, default=0x00000000*/

                            /* Thread ID associated with the logged error */
    UINT32 Coreid : 4;

                            /* Bits[28:25], Access Type=RWS_V, default=0x00000000*/

                            /* Core ID associated with the logged error */
    UINT32 Opcode0 : 3;

                            /* Bits[31:29], Access Type=RWS_V, default=0x00000000*/

                            /* pipe command associated with the error */
    UINT32 Opcode1 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* pipe command associated with the error */
    UINT32 Torid : 6;

                            /* Bits[43:38], Access Type=RWS_V, default=0x00000000*/

                            /* Entry in the TOR */
    UINT32 EnhMcaAvail2 : 20;

                            /* Bits[63:44], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC7_MISC_REGISTER;

/**
  CBO and LLC MCA bank: Control register
**/
#define MSR_MC8_CTL 0x00000420

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LlcTagError : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 LlcDataError : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadErrWbToMmio : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadErrIaAccessToGsm : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadCorrupingErrOther : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadNonCorrupingErrOther : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 MultipleTagMatchErr : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 IdiJitterError : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC8_CTL_REGISTER;

/**
  LLC MCA bank: Status register
**/
#define MSR_MC8_STATUS 0x00000421

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 McaErrorCode : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /* MCA error code */
    UINT32 ModelSpecificErrorCode : 16;

                            /* Bits[31:16], Access Type=RWS_V, default=0x00000000*/

                            /* Model specific error code */
    UINT32 EnhMcaAvail0 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 CorrErrCount : 15;

                            /* Bits[52:38], Access Type=RWS_V, default=0x00000000*/

                            /* Correctable error count */
    UINT32 Correrrorstatusind : 2;

                            /* Bits[54:53], Access Type=RWS_V, default=0x00000001*/

                            /*
                               These bits are used to indicate when the number
                               of corrected errors has exceeded the safe
                               threshold to the point where an uncorrected
                               error has become more likely to happen. Table 3
                               shows the encoding of these bits.
                            */
    UINT32 EnhMcaAvail1 : 2;

                            /* Bits[56:55], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RWS_V, default=0x00000000*/

                            /* PCC: Processor context corrupt */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RWS_V, default=0x00000000*/

                            /*
                               ADDRV: The address held in the MC6_ADDR_MSR is
                               valid
                            */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RWS_V, default=0x00000000*/

                            /*
                               MISCV: The value held in the MC6_MISC_MSR is
                               valid
                            */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RWS_V, default=0x00000000*/

                            /*
                               EN: Error enabled flag Indicates (when set) that
                               signaling of the machine check exception for
                               this error was enabled by an associated flag bit
                               of the Mci_CTL register.
                            */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RWS_V, default=0x00000000*/

                            /*
                               UC: Error uncorrected flag Indicates (when set)
                               that the processor did not correct the error
                               condition. When clear, this flag indicates that
                               the processor was able to correct the error
                               condition.
                            */
    UINT32 Over : 1;

                            /* Bits[62:62], Access Type=RWS_V, default=0x00000000*/

                            /* OVER: Machine check overflow */
    UINT32 Val : 1;

                            /* Bits[63:63], Access Type=RWS_V, default=0x00000000*/

                            /*
                               VAL: Valid flag Indicates (when set) that the
                               information within the Mci_STATUS register is
                               valid. When this flag is set, the processor
                               follows the rules given for the OVER flag in the
                               Mci_STATUS register when overwriting previously
                               valid entries. The processor sets the VAL flag
                               and software is responsible for clearing it.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC8_STATUS_REGISTER;

/**
  LLC MCA bank: Address register
**/
#define MSR_MC8_ADDR 0x00000422

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 AddressOffset : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Bits [5:0] of the Machine Check logged address.
                               Enchanced MCA feature made this field writable
                            */
    UINT32 Address0 : 26;

                            /* Bits[31:6], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 Address1 : 7;

                            /* Bits[38:32], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 EnhMcaAvail : 25;

                            /* Bits[63:39], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC8_ADDR_REGISTER;

/**
  LLC MCA bank: Misc register
**/
#define MSR_MC8_MISC 0x00000423

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LsbAddr : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Points to where the supported LSB in MCi_Addr in
                               SNB LLC cases it's always 6 (=000110)
                            */
    UINT32 Addrmode : 3;

                            /* Bits[8:6], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Address Mode = 010 since we always have only PA
                               in the SNB LLC
                            */
    UINT32 EnhMcaAvail0 : 5;

                            /* Bits[13:9], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Way : 4;

                            /* Bits[17:14], Access Type=RWS_V, default=0x00000000*/

                            /* The way associated with the logged error */
    UINT32 EnhMcaAvail1 : 6;

                            /* Bits[23:18], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Threadid : 1;

                            /* Bits[24:24], Access Type=RWS_V, default=0x00000000*/

                            /* Thread ID associated with the logged error */
    UINT32 Coreid : 4;

                            /* Bits[28:25], Access Type=RWS_V, default=0x00000000*/

                            /* Core ID associated with the logged error */
    UINT32 Opcode0 : 3;

                            /* Bits[31:29], Access Type=RWS_V, default=0x00000000*/

                            /* pipe command associated with the error */
    UINT32 Opcode1 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* pipe command associated with the error */
    UINT32 Torid : 6;

                            /* Bits[43:38], Access Type=RWS_V, default=0x00000000*/

                            /* Entry in the TOR */
    UINT32 EnhMcaAvail2 : 20;

                            /* Bits[63:44], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC8_MISC_REGISTER;

/**
  CBO and LLC MCA bank: Control register
**/
#define MSR_MC9_CTL 0x00000424

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LlcTagError : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 LlcDataError : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadErrWbToMmio : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadErrIaAccessToGsm : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadCorrupingErrOther : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadNonCorrupingErrOther : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 MultipleTagMatchErr : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 IdiJitterError : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC9_CTL_REGISTER;

/**
  LLC MCA bank: Status register
**/
#define MSR_MC9_STATUS 0x00000425

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 McaErrorCode : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /* MCA error code */
    UINT32 ModelSpecificErrorCode : 16;

                            /* Bits[31:16], Access Type=RWS_V, default=0x00000000*/

                            /* Model specific error code */
    UINT32 EnhMcaAvail0 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 CorrErrCount : 15;

                            /* Bits[52:38], Access Type=RWS_V, default=0x00000000*/

                            /* Correctable error count */
    UINT32 Correrrorstatusind : 2;

                            /* Bits[54:53], Access Type=RWS_V, default=0x00000001*/

                            /*
                               These bits are used to indicate when the number
                               of corrected errors has exceeded the safe
                               threshold to the point where an uncorrected
                               error has become more likely to happen. Table 3
                               shows the encoding of these bits.
                            */
    UINT32 EnhMcaAvail1 : 2;

                            /* Bits[56:55], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RWS_V, default=0x00000000*/

                            /* PCC: Processor context corrupt */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RWS_V, default=0x00000000*/

                            /*
                               ADDRV: The address held in the MC6_ADDR_MSR is
                               valid
                            */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RWS_V, default=0x00000000*/

                            /*
                               MISCV: The value held in the MC6_MISC_MSR is
                               valid
                            */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RWS_V, default=0x00000000*/

                            /*
                               EN: Error enabled flag Indicates (when set) that
                               signaling of the machine check exception for
                               this error was enabled by an associated flag bit
                               of the Mci_CTL register.
                            */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RWS_V, default=0x00000000*/

                            /*
                               UC: Error uncorrected flag Indicates (when set)
                               that the processor did not correct the error
                               condition. When clear, this flag indicates that
                               the processor was able to correct the error
                               condition.
                            */
    UINT32 Over : 1;

                            /* Bits[62:62], Access Type=RWS_V, default=0x00000000*/

                            /* OVER: Machine check overflow */
    UINT32 Val : 1;

                            /* Bits[63:63], Access Type=RWS_V, default=0x00000000*/

                            /*
                               VAL: Valid flag Indicates (when set) that the
                               information within the Mci_STATUS register is
                               valid. When this flag is set, the processor
                               follows the rules given for the OVER flag in the
                               Mci_STATUS register when overwriting previously
                               valid entries. The processor sets the VAL flag
                               and software is responsible for clearing it.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC9_STATUS_REGISTER;

/**
  LLC MCA bank: Address register
**/
#define MSR_MC9_ADDR 0x00000426

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 AddressOffset : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Bits [5:0] of the Machine Check logged address.
                               Enchanced MCA feature made this field writable
                            */
    UINT32 Address0 : 26;

                            /* Bits[31:6], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 Address1 : 7;

                            /* Bits[38:32], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 EnhMcaAvail : 25;

                            /* Bits[63:39], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC9_ADDR_REGISTER;

/**
  LLC MCA bank: Misc register
**/
#define MSR_MC9_MISC 0x00000427

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LsbAddr : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Points to where the supported LSB in MCi_Addr in
                               SNB LLC cases it's always 6 (=000110)
                            */
    UINT32 Addrmode : 3;

                            /* Bits[8:6], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Address Mode = 010 since we always have only PA
                               in the SNB LLC
                            */
    UINT32 EnhMcaAvail0 : 5;

                            /* Bits[13:9], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Way : 4;

                            /* Bits[17:14], Access Type=RWS_V, default=0x00000000*/

                            /* The way associated with the logged error */
    UINT32 EnhMcaAvail1 : 6;

                            /* Bits[23:18], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Threadid : 1;

                            /* Bits[24:24], Access Type=RWS_V, default=0x00000000*/

                            /* Thread ID associated with the logged error */
    UINT32 Coreid : 4;

                            /* Bits[28:25], Access Type=RWS_V, default=0x00000000*/

                            /* Core ID associated with the logged error */
    UINT32 Opcode0 : 3;

                            /* Bits[31:29], Access Type=RWS_V, default=0x00000000*/

                            /* pipe command associated with the error */
    UINT32 Opcode1 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* pipe command associated with the error */
    UINT32 Torid : 6;

                            /* Bits[43:38], Access Type=RWS_V, default=0x00000000*/

                            /* Entry in the TOR */
    UINT32 EnhMcaAvail2 : 20;

                            /* Bits[63:44], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC9_MISC_REGISTER;

/**
  CBO and LLC MCA bank: Control register
**/
#define MSR_MC10_CTL 0x00000428

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LlcTagError : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 LlcDataError : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadErrWbToMmio : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadErrIaAccessToGsm : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadCorrupingErrOther : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadNonCorrupingErrOther : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 MultipleTagMatchErr : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 IdiJitterError : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC10_CTL_REGISTER;

/**
  LLC MCA bank: Status register
**/
#define MSR_MC10_STATUS 0x00000429

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 McaErrorCode : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /* MCA error code */
    UINT32 ModelSpecificErrorCode : 16;

                            /* Bits[31:16], Access Type=RWS_V, default=0x00000000*/

                            /* Model specific error code */
    UINT32 EnhMcaAvail0 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 CorrErrCount : 15;

                            /* Bits[52:38], Access Type=RWS_V, default=0x00000000*/

                            /* Correctable error count */
    UINT32 Correrrorstatusind : 2;

                            /* Bits[54:53], Access Type=RWS_V, default=0x00000001*/

                            /*
                               These bits are used to indicate when the number
                               of corrected errors has exceeded the safe
                               threshold to the point where an uncorrected
                               error has become more likely to happen. Table 3
                               shows the encoding of these bits.
                            */
    UINT32 EnhMcaAvail1 : 2;

                            /* Bits[56:55], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RWS_V, default=0x00000000*/

                            /* PCC: Processor context corrupt */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RWS_V, default=0x00000000*/

                            /*
                               ADDRV: The address held in the MC6_ADDR_MSR is
                               valid
                            */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RWS_V, default=0x00000000*/

                            /*
                               MISCV: The value held in the MC6_MISC_MSR is
                               valid
                            */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RWS_V, default=0x00000000*/

                            /*
                               EN: Error enabled flag Indicates (when set) that
                               signaling of the machine check exception for
                               this error was enabled by an associated flag bit
                               of the Mci_CTL register.
                            */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RWS_V, default=0x00000000*/

                            /*
                               UC: Error uncorrected flag Indicates (when set)
                               that the processor did not correct the error
                               condition. When clear, this flag indicates that
                               the processor was able to correct the error
                               condition.
                            */
    UINT32 Over : 1;

                            /* Bits[62:62], Access Type=RWS_V, default=0x00000000*/

                            /* OVER: Machine check overflow */
    UINT32 Val : 1;

                            /* Bits[63:63], Access Type=RWS_V, default=0x00000000*/

                            /*
                               VAL: Valid flag Indicates (when set) that the
                               information within the Mci_STATUS register is
                               valid. When this flag is set, the processor
                               follows the rules given for the OVER flag in the
                               Mci_STATUS register when overwriting previously
                               valid entries. The processor sets the VAL flag
                               and software is responsible for clearing it.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC10_STATUS_REGISTER;

/**
  LLC MCA bank: Address register
**/
#define MSR_MC10_ADDR 0x0000042A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 AddressOffset : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Bits [5:0] of the Machine Check logged address.
                               Enchanced MCA feature made this field writable
                            */
    UINT32 Address0 : 26;

                            /* Bits[31:6], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 Address1 : 7;

                            /* Bits[38:32], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 EnhMcaAvail : 25;

                            /* Bits[63:39], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC10_ADDR_REGISTER;

/**
  LLC MCA bank: Misc register
**/
#define MSR_MC10_MISC 0x0000042B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LsbAddr : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Points to where the supported LSB in MCi_Addr in
                               SNB LLC cases it's always 6 (=000110)
                            */
    UINT32 Addrmode : 3;

                            /* Bits[8:6], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Address Mode = 010 since we always have only PA
                               in the SNB LLC
                            */
    UINT32 EnhMcaAvail0 : 5;

                            /* Bits[13:9], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Way : 4;

                            /* Bits[17:14], Access Type=RWS_V, default=0x00000000*/

                            /* The way associated with the logged error */
    UINT32 EnhMcaAvail1 : 6;

                            /* Bits[23:18], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Threadid : 1;

                            /* Bits[24:24], Access Type=RWS_V, default=0x00000000*/

                            /* Thread ID associated with the logged error */
    UINT32 Coreid : 4;

                            /* Bits[28:25], Access Type=RWS_V, default=0x00000000*/

                            /* Core ID associated with the logged error */
    UINT32 Opcode0 : 3;

                            /* Bits[31:29], Access Type=RWS_V, default=0x00000000*/

                            /* pipe command associated with the error */
    UINT32 Opcode1 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* pipe command associated with the error */
    UINT32 Torid : 6;

                            /* Bits[43:38], Access Type=RWS_V, default=0x00000000*/

                            /* Entry in the TOR */
    UINT32 EnhMcaAvail2 : 20;

                            /* Bits[63:44], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC10_MISC_REGISTER;

/**
  CBO and LLC MCA bank: Control register
**/
#define MSR_MC11_CTL 0x0000042C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LlcTagError : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 LlcDataError : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadErrWbToMmio : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadErrIaAccessToGsm : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadCorrupingErrOther : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadNonCorrupingErrOther : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 MultipleTagMatchErr : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 IdiJitterError : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC11_CTL_REGISTER;

/**
  LLC MCA bank: Status register
**/
#define MSR_MC11_STATUS 0x0000042D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 McaErrorCode : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /* MCA error code */
    UINT32 ModelSpecificErrorCode : 16;

                            /* Bits[31:16], Access Type=RWS_V, default=0x00000000*/

                            /* Model specific error code */
    UINT32 EnhMcaAvail0 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 CorrErrCount : 15;

                            /* Bits[52:38], Access Type=RWS_V, default=0x00000000*/

                            /* Correctable error count */
    UINT32 Correrrorstatusind : 2;

                            /* Bits[54:53], Access Type=RWS_V, default=0x00000001*/

                            /*
                               These bits are used to indicate when the number
                               of corrected errors has exceeded the safe
                               threshold to the point where an uncorrected
                               error has become more likely to happen. Table 3
                               shows the encoding of these bits.
                            */
    UINT32 EnhMcaAvail1 : 2;

                            /* Bits[56:55], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RWS_V, default=0x00000000*/

                            /* PCC: Processor context corrupt */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RWS_V, default=0x00000000*/

                            /*
                               ADDRV: The address held in the MC6_ADDR_MSR is
                               valid
                            */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RWS_V, default=0x00000000*/

                            /*
                               MISCV: The value held in the MC6_MISC_MSR is
                               valid
                            */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RWS_V, default=0x00000000*/

                            /*
                               EN: Error enabled flag Indicates (when set) that
                               signaling of the machine check exception for
                               this error was enabled by an associated flag bit
                               of the Mci_CTL register.
                            */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RWS_V, default=0x00000000*/

                            /*
                               UC: Error uncorrected flag Indicates (when set)
                               that the processor did not correct the error
                               condition. When clear, this flag indicates that
                               the processor was able to correct the error
                               condition.
                            */
    UINT32 Over : 1;

                            /* Bits[62:62], Access Type=RWS_V, default=0x00000000*/

                            /* OVER: Machine check overflow */
    UINT32 Val : 1;

                            /* Bits[63:63], Access Type=RWS_V, default=0x00000000*/

                            /*
                               VAL: Valid flag Indicates (when set) that the
                               information within the Mci_STATUS register is
                               valid. When this flag is set, the processor
                               follows the rules given for the OVER flag in the
                               Mci_STATUS register when overwriting previously
                               valid entries. The processor sets the VAL flag
                               and software is responsible for clearing it.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC11_STATUS_REGISTER;

/**
  LLC MCA bank: Address register
**/
#define MSR_MC11_ADDR 0x0000042E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 AddressOffset : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Bits [5:0] of the Machine Check logged address.
                               Enchanced MCA feature made this field writable
                            */
    UINT32 Address0 : 26;

                            /* Bits[31:6], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 Address1 : 7;

                            /* Bits[38:32], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 EnhMcaAvail : 25;

                            /* Bits[63:39], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC11_ADDR_REGISTER;

/**
  LLC MCA bank: Misc register
**/
#define MSR_MC11_MISC 0x0000042F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LsbAddr : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Points to where the supported LSB in MCi_Addr in
                               SNB LLC cases it's always 6 (=000110)
                            */
    UINT32 Addrmode : 3;

                            /* Bits[8:6], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Address Mode = 010 since we always have only PA
                               in the SNB LLC
                            */
    UINT32 EnhMcaAvail0 : 5;

                            /* Bits[13:9], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Way : 4;

                            /* Bits[17:14], Access Type=RWS_V, default=0x00000000*/

                            /* The way associated with the logged error */
    UINT32 EnhMcaAvail1 : 6;

                            /* Bits[23:18], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Threadid : 1;

                            /* Bits[24:24], Access Type=RWS_V, default=0x00000000*/

                            /* Thread ID associated with the logged error */
    UINT32 Coreid : 4;

                            /* Bits[28:25], Access Type=RWS_V, default=0x00000000*/

                            /* Core ID associated with the logged error */
    UINT32 Opcode0 : 3;

                            /* Bits[31:29], Access Type=RWS_V, default=0x00000000*/

                            /* pipe command associated with the error */
    UINT32 Opcode1 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* pipe command associated with the error */
    UINT32 Torid : 6;

                            /* Bits[43:38], Access Type=RWS_V, default=0x00000000*/

                            /* Entry in the TOR */
    UINT32 EnhMcaAvail2 : 20;

                            /* Bits[63:44], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC11_MISC_REGISTER;

/**
  CBO and LLC MCA bank: Control register
**/
#define MSR_MC12_CTL 0x00000430

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LlcTagError : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 LlcDataError : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadErrWbToMmio : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadErrIaAccessToGsm : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadCorrupingErrOther : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadNonCorrupingErrOther : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 MultipleTagMatchErr : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 IdiJitterError : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC12_CTL_REGISTER;

/**
  LLC MCA bank: Status register
**/
#define MSR_MC12_STATUS 0x00000431

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 McaErrorCode : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /* MCA error code */
    UINT32 ModelSpecificErrorCode : 16;

                            /* Bits[31:16], Access Type=RWS_V, default=0x00000000*/

                            /* Model specific error code */
    UINT32 EnhMcaAvail0 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 CorrErrCount : 15;

                            /* Bits[52:38], Access Type=RWS_V, default=0x00000000*/

                            /* Correctable error count */
    UINT32 Correrrorstatusind : 2;

                            /* Bits[54:53], Access Type=RWS_V, default=0x00000001*/

                            /*
                               These bits are used to indicate when the number
                               of corrected errors has exceeded the safe
                               threshold to the point where an uncorrected
                               error has become more likely to happen. Table 3
                               shows the encoding of these bits.
                            */
    UINT32 EnhMcaAvail1 : 2;

                            /* Bits[56:55], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RWS_V, default=0x00000000*/

                            /* PCC: Processor context corrupt */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RWS_V, default=0x00000000*/

                            /*
                               ADDRV: The address held in the MC6_ADDR_MSR is
                               valid
                            */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RWS_V, default=0x00000000*/

                            /*
                               MISCV: The value held in the MC6_MISC_MSR is
                               valid
                            */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RWS_V, default=0x00000000*/

                            /*
                               EN: Error enabled flag Indicates (when set) that
                               signaling of the machine check exception for
                               this error was enabled by an associated flag bit
                               of the Mci_CTL register.
                            */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RWS_V, default=0x00000000*/

                            /*
                               UC: Error uncorrected flag Indicates (when set)
                               that the processor did not correct the error
                               condition. When clear, this flag indicates that
                               the processor was able to correct the error
                               condition.
                            */
    UINT32 Over : 1;

                            /* Bits[62:62], Access Type=RWS_V, default=0x00000000*/

                            /* OVER: Machine check overflow */
    UINT32 Val : 1;

                            /* Bits[63:63], Access Type=RWS_V, default=0x00000000*/

                            /*
                               VAL: Valid flag Indicates (when set) that the
                               information within the Mci_STATUS register is
                               valid. When this flag is set, the processor
                               follows the rules given for the OVER flag in the
                               Mci_STATUS register when overwriting previously
                               valid entries. The processor sets the VAL flag
                               and software is responsible for clearing it.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC12_STATUS_REGISTER;

/**
  LLC MCA bank: Address register
**/
#define MSR_MC12_ADDR 0x00000432

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 AddressOffset : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Bits [5:0] of the Machine Check logged address.
                               Enchanced MCA feature made this field writable
                            */
    UINT32 Address0 : 26;

                            /* Bits[31:6], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 Address1 : 7;

                            /* Bits[38:32], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 EnhMcaAvail : 25;

                            /* Bits[63:39], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC12_ADDR_REGISTER;

/**
  LLC MCA bank: Misc register
**/
#define MSR_MC12_MISC 0x00000433

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LsbAddr : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Points to where the supported LSB in MCi_Addr in
                               SNB LLC cases it's always 6 (=000110)
                            */
    UINT32 Addrmode : 3;

                            /* Bits[8:6], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Address Mode = 010 since we always have only PA
                               in the SNB LLC
                            */
    UINT32 EnhMcaAvail0 : 5;

                            /* Bits[13:9], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Way : 4;

                            /* Bits[17:14], Access Type=RWS_V, default=0x00000000*/

                            /* The way associated with the logged error */
    UINT32 EnhMcaAvail1 : 6;

                            /* Bits[23:18], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Threadid : 1;

                            /* Bits[24:24], Access Type=RWS_V, default=0x00000000*/

                            /* Thread ID associated with the logged error */
    UINT32 Coreid : 4;

                            /* Bits[28:25], Access Type=RWS_V, default=0x00000000*/

                            /* Core ID associated with the logged error */
    UINT32 Opcode0 : 3;

                            /* Bits[31:29], Access Type=RWS_V, default=0x00000000*/

                            /* pipe command associated with the error */
    UINT32 Opcode1 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* pipe command associated with the error */
    UINT32 Torid : 6;

                            /* Bits[43:38], Access Type=RWS_V, default=0x00000000*/

                            /* Entry in the TOR */
    UINT32 EnhMcaAvail2 : 20;

                            /* Bits[63:44], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC12_MISC_REGISTER;

/**
  CBO and LLC MCA bank: Control register
**/
#define MSR_MC13_CTL 0x00000434

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LlcTagError : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 LlcDataError : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadErrWbToMmio : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadErrIaAccessToGsm : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadCorrupingErrOther : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadNonCorrupingErrOther : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 MultipleTagMatchErr : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 IdiJitterError : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC13_CTL_REGISTER;

/**
  LLC MCA bank: Status register
**/
#define MSR_MC13_STATUS 0x00000435

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 McaErrorCode : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /* MCA error code */
    UINT32 ModelSpecificErrorCode : 16;

                            /* Bits[31:16], Access Type=RWS_V, default=0x00000000*/

                            /* Model specific error code */
    UINT32 EnhMcaAvail0 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 CorrErrCount : 15;

                            /* Bits[52:38], Access Type=RWS_V, default=0x00000000*/

                            /* Correctable error count */
    UINT32 Correrrorstatusind : 2;

                            /* Bits[54:53], Access Type=RWS_V, default=0x00000001*/

                            /*
                               These bits are used to indicate when the number
                               of corrected errors has exceeded the safe
                               threshold to the point where an uncorrected
                               error has become more likely to happen. Table 3
                               shows the encoding of these bits.
                            */
    UINT32 EnhMcaAvail1 : 2;

                            /* Bits[56:55], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RWS_V, default=0x00000000*/

                            /* PCC: Processor context corrupt */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RWS_V, default=0x00000000*/

                            /*
                               ADDRV: The address held in the MC6_ADDR_MSR is
                               valid
                            */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RWS_V, default=0x00000000*/

                            /*
                               MISCV: The value held in the MC6_MISC_MSR is
                               valid
                            */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RWS_V, default=0x00000000*/

                            /*
                               EN: Error enabled flag Indicates (when set) that
                               signaling of the machine check exception for
                               this error was enabled by an associated flag bit
                               of the Mci_CTL register.
                            */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RWS_V, default=0x00000000*/

                            /*
                               UC: Error uncorrected flag Indicates (when set)
                               that the processor did not correct the error
                               condition. When clear, this flag indicates that
                               the processor was able to correct the error
                               condition.
                            */
    UINT32 Over : 1;

                            /* Bits[62:62], Access Type=RWS_V, default=0x00000000*/

                            /* OVER: Machine check overflow */
    UINT32 Val : 1;

                            /* Bits[63:63], Access Type=RWS_V, default=0x00000000*/

                            /*
                               VAL: Valid flag Indicates (when set) that the
                               information within the Mci_STATUS register is
                               valid. When this flag is set, the processor
                               follows the rules given for the OVER flag in the
                               Mci_STATUS register when overwriting previously
                               valid entries. The processor sets the VAL flag
                               and software is responsible for clearing it.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC13_STATUS_REGISTER;

/**
  LLC MCA bank: Address register
**/
#define MSR_MC13_ADDR 0x00000436

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 AddressOffset : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Bits [5:0] of the Machine Check logged address.
                               Enchanced MCA feature made this field writable
                            */
    UINT32 Address0 : 26;

                            /* Bits[31:6], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 Address1 : 7;

                            /* Bits[38:32], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 EnhMcaAvail : 25;

                            /* Bits[63:39], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC13_ADDR_REGISTER;

/**
  LLC MCA bank: Misc register
**/
#define MSR_MC13_MISC 0x00000437

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LsbAddr : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Points to where the supported LSB in MCi_Addr in
                               SNB LLC cases it's always 6 (=000110)
                            */
    UINT32 Addrmode : 3;

                            /* Bits[8:6], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Address Mode = 010 since we always have only PA
                               in the SNB LLC
                            */
    UINT32 EnhMcaAvail0 : 5;

                            /* Bits[13:9], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Way : 4;

                            /* Bits[17:14], Access Type=RWS_V, default=0x00000000*/

                            /* The way associated with the logged error */
    UINT32 EnhMcaAvail1 : 6;

                            /* Bits[23:18], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Threadid : 1;

                            /* Bits[24:24], Access Type=RWS_V, default=0x00000000*/

                            /* Thread ID associated with the logged error */
    UINT32 Coreid : 4;

                            /* Bits[28:25], Access Type=RWS_V, default=0x00000000*/

                            /* Core ID associated with the logged error */
    UINT32 Opcode0 : 3;

                            /* Bits[31:29], Access Type=RWS_V, default=0x00000000*/

                            /* pipe command associated with the error */
    UINT32 Opcode1 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* pipe command associated with the error */
    UINT32 Torid : 6;

                            /* Bits[43:38], Access Type=RWS_V, default=0x00000000*/

                            /* Entry in the TOR */
    UINT32 EnhMcaAvail2 : 20;

                            /* Bits[63:44], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC13_MISC_REGISTER;

/**
  CBO and LLC MCA bank: Control register
**/
#define MSR_MC14_CTL 0x00000438

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LlcTagError : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 LlcDataError : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadErrWbToMmio : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadErrIaAccessToGsm : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadCorrupingErrOther : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadNonCorrupingErrOther : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 MultipleTagMatchErr : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 IdiJitterError : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC14_CTL_REGISTER;

/**
  LLC MCA bank: Status register
**/
#define MSR_MC14_STATUS 0x00000439

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 McaErrorCode : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /* MCA error code */
    UINT32 ModelSpecificErrorCode : 16;

                            /* Bits[31:16], Access Type=RWS_V, default=0x00000000*/

                            /* Model specific error code */
    UINT32 EnhMcaAvail0 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 CorrErrCount : 15;

                            /* Bits[52:38], Access Type=RWS_V, default=0x00000000*/

                            /* Correctable error count */
    UINT32 Correrrorstatusind : 2;

                            /* Bits[54:53], Access Type=RWS_V, default=0x00000001*/

                            /*
                               These bits are used to indicate when the number
                               of corrected errors has exceeded the safe
                               threshold to the point where an uncorrected
                               error has become more likely to happen. Table 3
                               shows the encoding of these bits.
                            */
    UINT32 EnhMcaAvail1 : 2;

                            /* Bits[56:55], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RWS_V, default=0x00000000*/

                            /* PCC: Processor context corrupt */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RWS_V, default=0x00000000*/

                            /*
                               ADDRV: The address held in the MC6_ADDR_MSR is
                               valid
                            */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RWS_V, default=0x00000000*/

                            /*
                               MISCV: The value held in the MC6_MISC_MSR is
                               valid
                            */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RWS_V, default=0x00000000*/

                            /*
                               EN: Error enabled flag Indicates (when set) that
                               signaling of the machine check exception for
                               this error was enabled by an associated flag bit
                               of the Mci_CTL register.
                            */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RWS_V, default=0x00000000*/

                            /*
                               UC: Error uncorrected flag Indicates (when set)
                               that the processor did not correct the error
                               condition. When clear, this flag indicates that
                               the processor was able to correct the error
                               condition.
                            */
    UINT32 Over : 1;

                            /* Bits[62:62], Access Type=RWS_V, default=0x00000000*/

                            /* OVER: Machine check overflow */
    UINT32 Val : 1;

                            /* Bits[63:63], Access Type=RWS_V, default=0x00000000*/

                            /*
                               VAL: Valid flag Indicates (when set) that the
                               information within the Mci_STATUS register is
                               valid. When this flag is set, the processor
                               follows the rules given for the OVER flag in the
                               Mci_STATUS register when overwriting previously
                               valid entries. The processor sets the VAL flag
                               and software is responsible for clearing it.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC14_STATUS_REGISTER;

/**
  LLC MCA bank: Address register
**/
#define MSR_MC14_ADDR 0x0000043A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 AddressOffset : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Bits [5:0] of the Machine Check logged address.
                               Enchanced MCA feature made this field writable
                            */
    UINT32 Address0 : 26;

                            /* Bits[31:6], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 Address1 : 7;

                            /* Bits[38:32], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 EnhMcaAvail : 25;

                            /* Bits[63:39], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC14_ADDR_REGISTER;

/**
  LLC MCA bank: Misc register
**/
#define MSR_MC14_MISC 0x0000043B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LsbAddr : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Points to where the supported LSB in MCi_Addr in
                               SNB LLC cases it's always 6 (=000110)
                            */
    UINT32 Addrmode : 3;

                            /* Bits[8:6], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Address Mode = 010 since we always have only PA
                               in the SNB LLC
                            */
    UINT32 EnhMcaAvail0 : 5;

                            /* Bits[13:9], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Way : 4;

                            /* Bits[17:14], Access Type=RWS_V, default=0x00000000*/

                            /* The way associated with the logged error */
    UINT32 EnhMcaAvail1 : 6;

                            /* Bits[23:18], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Threadid : 1;

                            /* Bits[24:24], Access Type=RWS_V, default=0x00000000*/

                            /* Thread ID associated with the logged error */
    UINT32 Coreid : 4;

                            /* Bits[28:25], Access Type=RWS_V, default=0x00000000*/

                            /* Core ID associated with the logged error */
    UINT32 Opcode0 : 3;

                            /* Bits[31:29], Access Type=RWS_V, default=0x00000000*/

                            /* pipe command associated with the error */
    UINT32 Opcode1 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* pipe command associated with the error */
    UINT32 Torid : 6;

                            /* Bits[43:38], Access Type=RWS_V, default=0x00000000*/

                            /* Entry in the TOR */
    UINT32 EnhMcaAvail2 : 20;

                            /* Bits[63:44], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC14_MISC_REGISTER;

/**
  CBO and LLC MCA bank: Control register
**/
#define MSR_MC15_CTL 0x0000043C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LlcTagError : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 LlcDataError : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadErrWbToMmio : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadErrIaAccessToGsm : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadCorrupingErrOther : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadNonCorrupingErrOther : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 MultipleTagMatchErr : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 IdiJitterError : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC15_CTL_REGISTER;

/**
  LLC MCA bank: Status register
**/
#define MSR_MC15_STATUS 0x0000043D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 McaErrorCode : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /* MCA error code */
    UINT32 ModelSpecificErrorCode : 16;

                            /* Bits[31:16], Access Type=RWS_V, default=0x00000000*/

                            /* Model specific error code */
    UINT32 EnhMcaAvail0 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 CorrErrCount : 15;

                            /* Bits[52:38], Access Type=RWS_V, default=0x00000000*/

                            /* Correctable error count */
    UINT32 Correrrorstatusind : 2;

                            /* Bits[54:53], Access Type=RWS_V, default=0x00000001*/

                            /*
                               These bits are used to indicate when the number
                               of corrected errors has exceeded the safe
                               threshold to the point where an uncorrected
                               error has become more likely to happen. Table 3
                               shows the encoding of these bits.
                            */
    UINT32 EnhMcaAvail1 : 2;

                            /* Bits[56:55], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RWS_V, default=0x00000000*/

                            /* PCC: Processor context corrupt */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RWS_V, default=0x00000000*/

                            /*
                               ADDRV: The address held in the MC6_ADDR_MSR is
                               valid
                            */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RWS_V, default=0x00000000*/

                            /*
                               MISCV: The value held in the MC6_MISC_MSR is
                               valid
                            */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RWS_V, default=0x00000000*/

                            /*
                               EN: Error enabled flag Indicates (when set) that
                               signaling of the machine check exception for
                               this error was enabled by an associated flag bit
                               of the Mci_CTL register.
                            */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RWS_V, default=0x00000000*/

                            /*
                               UC: Error uncorrected flag Indicates (when set)
                               that the processor did not correct the error
                               condition. When clear, this flag indicates that
                               the processor was able to correct the error
                               condition.
                            */
    UINT32 Over : 1;

                            /* Bits[62:62], Access Type=RWS_V, default=0x00000000*/

                            /* OVER: Machine check overflow */
    UINT32 Val : 1;

                            /* Bits[63:63], Access Type=RWS_V, default=0x00000000*/

                            /*
                               VAL: Valid flag Indicates (when set) that the
                               information within the Mci_STATUS register is
                               valid. When this flag is set, the processor
                               follows the rules given for the OVER flag in the
                               Mci_STATUS register when overwriting previously
                               valid entries. The processor sets the VAL flag
                               and software is responsible for clearing it.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC15_STATUS_REGISTER;

/**
  LLC MCA bank: Address register
**/
#define MSR_MC15_ADDR 0x0000043E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 AddressOffset : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Bits [5:0] of the Machine Check logged address.
                               Enchanced MCA feature made this field writable
                            */
    UINT32 Address0 : 26;

                            /* Bits[31:6], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 Address1 : 7;

                            /* Bits[38:32], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 EnhMcaAvail : 25;

                            /* Bits[63:39], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC15_ADDR_REGISTER;

/**
  LLC MCA bank: Misc register
**/
#define MSR_MC15_MISC 0x0000043F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LsbAddr : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Points to where the supported LSB in MCi_Addr in
                               SNB LLC cases it's always 6 (=000110)
                            */
    UINT32 Addrmode : 3;

                            /* Bits[8:6], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Address Mode = 010 since we always have only PA
                               in the SNB LLC
                            */
    UINT32 EnhMcaAvail0 : 5;

                            /* Bits[13:9], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Way : 4;

                            /* Bits[17:14], Access Type=RWS_V, default=0x00000000*/

                            /* The way associated with the logged error */
    UINT32 EnhMcaAvail1 : 6;

                            /* Bits[23:18], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Threadid : 1;

                            /* Bits[24:24], Access Type=RWS_V, default=0x00000000*/

                            /* Thread ID associated with the logged error */
    UINT32 Coreid : 4;

                            /* Bits[28:25], Access Type=RWS_V, default=0x00000000*/

                            /* Core ID associated with the logged error */
    UINT32 Opcode0 : 3;

                            /* Bits[31:29], Access Type=RWS_V, default=0x00000000*/

                            /* pipe command associated with the error */
    UINT32 Opcode1 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* pipe command associated with the error */
    UINT32 Torid : 6;

                            /* Bits[43:38], Access Type=RWS_V, default=0x00000000*/

                            /* Entry in the TOR */
    UINT32 EnhMcaAvail2 : 20;

                            /* Bits[63:44], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC15_MISC_REGISTER;

/**
  CBO and LLC MCA bank: Control register
**/
#define MSR_MC16_CTL 0x00000440

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LlcTagError : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 LlcDataError : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadErrWbToMmio : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadErrIaAccessToGsm : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadCorrupingErrOther : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadNonCorrupingErrOther : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 MultipleTagMatchErr : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 IdiJitterError : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC16_CTL_REGISTER;

/**
  LLC MCA bank: Status register
**/
#define MSR_MC16_STATUS 0x00000441

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 McaErrorCode : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /* MCA error code */
    UINT32 ModelSpecificErrorCode : 16;

                            /* Bits[31:16], Access Type=RWS_V, default=0x00000000*/

                            /* Model specific error code */
    UINT32 EnhMcaAvail0 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 CorrErrCount : 15;

                            /* Bits[52:38], Access Type=RWS_V, default=0x00000000*/

                            /* Correctable error count */
    UINT32 Correrrorstatusind : 2;

                            /* Bits[54:53], Access Type=RWS_V, default=0x00000001*/

                            /*
                               These bits are used to indicate when the number
                               of corrected errors has exceeded the safe
                               threshold to the point where an uncorrected
                               error has become more likely to happen. Table 3
                               shows the encoding of these bits.
                            */
    UINT32 EnhMcaAvail1 : 2;

                            /* Bits[56:55], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RWS_V, default=0x00000000*/

                            /* PCC: Processor context corrupt */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RWS_V, default=0x00000000*/

                            /*
                               ADDRV: The address held in the MC6_ADDR_MSR is
                               valid
                            */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RWS_V, default=0x00000000*/

                            /*
                               MISCV: The value held in the MC6_MISC_MSR is
                               valid
                            */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RWS_V, default=0x00000000*/

                            /*
                               EN: Error enabled flag Indicates (when set) that
                               signaling of the machine check exception for
                               this error was enabled by an associated flag bit
                               of the Mci_CTL register.
                            */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RWS_V, default=0x00000000*/

                            /*
                               UC: Error uncorrected flag Indicates (when set)
                               that the processor did not correct the error
                               condition. When clear, this flag indicates that
                               the processor was able to correct the error
                               condition.
                            */
    UINT32 Over : 1;

                            /* Bits[62:62], Access Type=RWS_V, default=0x00000000*/

                            /* OVER: Machine check overflow */
    UINT32 Val : 1;

                            /* Bits[63:63], Access Type=RWS_V, default=0x00000000*/

                            /*
                               VAL: Valid flag Indicates (when set) that the
                               information within the Mci_STATUS register is
                               valid. When this flag is set, the processor
                               follows the rules given for the OVER flag in the
                               Mci_STATUS register when overwriting previously
                               valid entries. The processor sets the VAL flag
                               and software is responsible for clearing it.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC16_STATUS_REGISTER;

/**
  LLC MCA bank: Address register
**/
#define MSR_MC16_ADDR 0x00000442

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 AddressOffset : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Bits [5:0] of the Machine Check logged address.
                               Enchanced MCA feature made this field writable
                            */
    UINT32 Address0 : 26;

                            /* Bits[31:6], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 Address1 : 7;

                            /* Bits[38:32], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 EnhMcaAvail : 25;

                            /* Bits[63:39], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC16_ADDR_REGISTER;

/**
  LLC MCA bank: Misc register
**/
#define MSR_MC16_MISC 0x00000443

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LsbAddr : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Points to where the supported LSB in MCi_Addr in
                               SNB LLC cases it's always 6 (=000110)
                            */
    UINT32 Addrmode : 3;

                            /* Bits[8:6], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Address Mode = 010 since we always have only PA
                               in the SNB LLC
                            */
    UINT32 EnhMcaAvail0 : 5;

                            /* Bits[13:9], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Way : 4;

                            /* Bits[17:14], Access Type=RWS_V, default=0x00000000*/

                            /* The way associated with the logged error */
    UINT32 EnhMcaAvail1 : 6;

                            /* Bits[23:18], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Threadid : 1;

                            /* Bits[24:24], Access Type=RWS_V, default=0x00000000*/

                            /* Thread ID associated with the logged error */
    UINT32 Coreid : 4;

                            /* Bits[28:25], Access Type=RWS_V, default=0x00000000*/

                            /* Core ID associated with the logged error */
    UINT32 Opcode0 : 3;

                            /* Bits[31:29], Access Type=RWS_V, default=0x00000000*/

                            /* pipe command associated with the error */
    UINT32 Opcode1 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* pipe command associated with the error */
    UINT32 Torid : 6;

                            /* Bits[43:38], Access Type=RWS_V, default=0x00000000*/

                            /* Entry in the TOR */
    UINT32 EnhMcaAvail2 : 20;

                            /* Bits[63:44], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC16_MISC_REGISTER;

/**
  CBO and LLC MCA bank: Control register
**/
#define MSR_MC17_CTL 0x00000444

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LlcTagError : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 LlcDataError : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadErrWbToMmio : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadErrIaAccessToGsm : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadCorrupingErrOther : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadNonCorrupingErrOther : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 MultipleTagMatchErr : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 IdiJitterError : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC17_CTL_REGISTER;

/**
  LLC MCA bank: Status register
**/
#define MSR_MC17_STATUS 0x00000445

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 McaErrorCode : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /* MCA error code */
    UINT32 ModelSpecificErrorCode : 16;

                            /* Bits[31:16], Access Type=RWS_V, default=0x00000000*/

                            /* Model specific error code */
    UINT32 EnhMcaAvail0 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 CorrErrCount : 15;

                            /* Bits[52:38], Access Type=RWS_V, default=0x00000000*/

                            /* Correctable error count */
    UINT32 Correrrorstatusind : 2;

                            /* Bits[54:53], Access Type=RWS_V, default=0x00000001*/

                            /*
                               These bits are used to indicate when the number
                               of corrected errors has exceeded the safe
                               threshold to the point where an uncorrected
                               error has become more likely to happen. Table 3
                               shows the encoding of these bits.
                            */
    UINT32 EnhMcaAvail1 : 2;

                            /* Bits[56:55], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RWS_V, default=0x00000000*/

                            /* PCC: Processor context corrupt */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RWS_V, default=0x00000000*/

                            /*
                               ADDRV: The address held in the MC6_ADDR_MSR is
                               valid
                            */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RWS_V, default=0x00000000*/

                            /*
                               MISCV: The value held in the MC6_MISC_MSR is
                               valid
                            */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RWS_V, default=0x00000000*/

                            /*
                               EN: Error enabled flag Indicates (when set) that
                               signaling of the machine check exception for
                               this error was enabled by an associated flag bit
                               of the Mci_CTL register.
                            */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RWS_V, default=0x00000000*/

                            /*
                               UC: Error uncorrected flag Indicates (when set)
                               that the processor did not correct the error
                               condition. When clear, this flag indicates that
                               the processor was able to correct the error
                               condition.
                            */
    UINT32 Over : 1;

                            /* Bits[62:62], Access Type=RWS_V, default=0x00000000*/

                            /* OVER: Machine check overflow */
    UINT32 Val : 1;

                            /* Bits[63:63], Access Type=RWS_V, default=0x00000000*/

                            /*
                               VAL: Valid flag Indicates (when set) that the
                               information within the Mci_STATUS register is
                               valid. When this flag is set, the processor
                               follows the rules given for the OVER flag in the
                               Mci_STATUS register when overwriting previously
                               valid entries. The processor sets the VAL flag
                               and software is responsible for clearing it.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC17_STATUS_REGISTER;

/**
  LLC MCA bank: Address register
**/
#define MSR_MC17_ADDR 0x00000446

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 AddressOffset : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Bits [5:0] of the Machine Check logged address.
                               Enchanced MCA feature made this field writable
                            */
    UINT32 Address0 : 26;

                            /* Bits[31:6], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 Address1 : 7;

                            /* Bits[38:32], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 EnhMcaAvail : 25;

                            /* Bits[63:39], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC17_ADDR_REGISTER;

/**
  LLC MCA bank: Misc register
**/
#define MSR_MC17_MISC 0x00000447

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LsbAddr : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Points to where the supported LSB in MCi_Addr in
                               SNB LLC cases it's always 6 (=000110)
                            */
    UINT32 Addrmode : 3;

                            /* Bits[8:6], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Address Mode = 010 since we always have only PA
                               in the SNB LLC
                            */
    UINT32 EnhMcaAvail0 : 5;

                            /* Bits[13:9], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Way : 4;

                            /* Bits[17:14], Access Type=RWS_V, default=0x00000000*/

                            /* The way associated with the logged error */
    UINT32 EnhMcaAvail1 : 6;

                            /* Bits[23:18], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Threadid : 1;

                            /* Bits[24:24], Access Type=RWS_V, default=0x00000000*/

                            /* Thread ID associated with the logged error */
    UINT32 Coreid : 4;

                            /* Bits[28:25], Access Type=RWS_V, default=0x00000000*/

                            /* Core ID associated with the logged error */
    UINT32 Opcode0 : 3;

                            /* Bits[31:29], Access Type=RWS_V, default=0x00000000*/

                            /* pipe command associated with the error */
    UINT32 Opcode1 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* pipe command associated with the error */
    UINT32 Torid : 6;

                            /* Bits[43:38], Access Type=RWS_V, default=0x00000000*/

                            /* Entry in the TOR */
    UINT32 EnhMcaAvail2 : 20;

                            /* Bits[63:44], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC17_MISC_REGISTER;

/**
  CBO and LLC MCA bank: Control register
**/
#define MSR_MC18_CTL 0x00000448

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LlcTagError : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 LlcDataError : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadErrWbToMmio : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadErrIaAccessToGsm : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadCorrupingErrOther : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SadNonCorrupingErrOther : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 MultipleTagMatchErr : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 IdiJitterError : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC18_CTL_REGISTER;

/**
  LLC MCA bank: Status register
**/
#define MSR_MC18_STATUS 0x00000449

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 McaErrorCode : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /* MCA error code */
    UINT32 ModelSpecificErrorCode : 16;

                            /* Bits[31:16], Access Type=RWS_V, default=0x00000000*/

                            /* Model specific error code */
    UINT32 EnhMcaAvail0 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 CorrErrCount : 15;

                            /* Bits[52:38], Access Type=RWS_V, default=0x00000000*/

                            /* Correctable error count */
    UINT32 Correrrorstatusind : 2;

                            /* Bits[54:53], Access Type=RWS_V, default=0x00000001*/

                            /*
                               These bits are used to indicate when the number
                               of corrected errors has exceeded the safe
                               threshold to the point where an uncorrected
                               error has become more likely to happen. Table 3
                               shows the encoding of these bits.
                            */
    UINT32 EnhMcaAvail1 : 2;

                            /* Bits[56:55], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RWS_V, default=0x00000000*/

                            /* PCC: Processor context corrupt */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RWS_V, default=0x00000000*/

                            /*
                               ADDRV: The address held in the MC6_ADDR_MSR is
                               valid
                            */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RWS_V, default=0x00000000*/

                            /*
                               MISCV: The value held in the MC6_MISC_MSR is
                               valid
                            */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RWS_V, default=0x00000000*/

                            /*
                               EN: Error enabled flag Indicates (when set) that
                               signaling of the machine check exception for
                               this error was enabled by an associated flag bit
                               of the Mci_CTL register.
                            */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RWS_V, default=0x00000000*/

                            /*
                               UC: Error uncorrected flag Indicates (when set)
                               that the processor did not correct the error
                               condition. When clear, this flag indicates that
                               the processor was able to correct the error
                               condition.
                            */
    UINT32 Over : 1;

                            /* Bits[62:62], Access Type=RWS_V, default=0x00000000*/

                            /* OVER: Machine check overflow */
    UINT32 Val : 1;

                            /* Bits[63:63], Access Type=RWS_V, default=0x00000000*/

                            /*
                               VAL: Valid flag Indicates (when set) that the
                               information within the Mci_STATUS register is
                               valid. When this flag is set, the processor
                               follows the rules given for the OVER flag in the
                               Mci_STATUS register when overwriting previously
                               valid entries. The processor sets the VAL flag
                               and software is responsible for clearing it.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC18_STATUS_REGISTER;

/**
  LLC MCA bank: Address register
**/
#define MSR_MC18_ADDR 0x0000044A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 AddressOffset : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Bits [5:0] of the Machine Check logged address.
                               Enchanced MCA feature made this field writable
                            */
    UINT32 Address0 : 26;

                            /* Bits[31:6], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 Address1 : 7;

                            /* Bits[38:32], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 EnhMcaAvail : 25;

                            /* Bits[63:39], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC18_ADDR_REGISTER;

/**
  LLC MCA bank: Misc register
**/
#define MSR_MC18_MISC 0x0000044B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LsbAddr : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Points to where the supported LSB in MCi_Addr in
                               SNB LLC cases it's always 6 (=000110)
                            */
    UINT32 Addrmode : 3;

                            /* Bits[8:6], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Address Mode = 010 since we always have only PA
                               in the SNB LLC
                            */
    UINT32 EnhMcaAvail0 : 5;

                            /* Bits[13:9], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Way : 4;

                            /* Bits[17:14], Access Type=RWS_V, default=0x00000000*/

                            /* The way associated with the logged error */
    UINT32 EnhMcaAvail1 : 6;

                            /* Bits[23:18], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */
    UINT32 Threadid : 1;

                            /* Bits[24:24], Access Type=RWS_V, default=0x00000000*/

                            /* Thread ID associated with the logged error */
    UINT32 Coreid : 4;

                            /* Bits[28:25], Access Type=RWS_V, default=0x00000000*/

                            /* Core ID associated with the logged error */
    UINT32 Opcode0 : 3;

                            /* Bits[31:29], Access Type=RWS_V, default=0x00000000*/

                            /* pipe command associated with the error */
    UINT32 Opcode1 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /* pipe command associated with the error */
    UINT32 Torid : 6;

                            /* Bits[43:38], Access Type=RWS_V, default=0x00000000*/

                            /* Entry in the TOR */
    UINT32 EnhMcaAvail2 : 20;

                            /* Bits[63:44], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC18_MISC_REGISTER;

/**
  EDRAM 0 Machine Check Architecture Control MSR.
**/
#define MSR_MC19_CTL 0x0000044C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 EdcDataUncorrError : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 AddrCmdParity : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 EtagUncorrError : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 EtagMultiMatchError : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 EtrogUnknownError : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SaveRestoreUncorrError : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 Rsvd6 : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC19_CTL_REGISTER;

/**
  EDRAM 0 MCA bank: Status Register
**/
#define MSR_MC19_STATUS 0x0000044D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Mcacod : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /* MCA error code */
    UINT32 Mscod : 16;

                            /* Bits[31:16], Access Type=RWS_V, default=0x00000000*/

                            /* Model specific error code */
    UINT32 EnhMcaAvail0 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Available when Enhanced MCA is in use - unused
                               in etrog
                            */
    UINT32 CorrectedErrorCount : 15;

                            /* Bits[52:38], Access Type=RWS_V, default=0x00000000*/

                            /* Correctable error count */
    UINT32 ThresholdBasedErrorStatus : 2;

                            /* Bits[54:53], Access Type=RWS_V, default=0x00000001*/

                            /*
                               These bits are used to indicate when the number
                               of corrected errors has exceeded the safe
                               threshold to the point where an uncorrected
                               error has become more likely to happen.
                            */
    UINT32 Ar : 2;

                            /* Bits[56:55], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Available when Enhanced MCA is in use, unused in
                               etrog
                            */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RWS_V, default=0x00000000*/

                            /* PCC: Processor context corrupt */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RWS_V, default=0x00000000*/

                            /*
                               ADDRV: The address held in the MCi_ADDR_MSR is
                               valid
                            */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RWS_V, default=0x00000000*/

                            /*
                               MISCV: The value held in the MCi_MISC_MSR is
                               valid
                            */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RWS_V, default=0x00000000*/

                            /*
                               EN: Error enabled flag Indicates (when set) that
                               signaling of the machine check exception for
                               this error was enabled by an associated flag bit
                               of the Mci_CTL register.
                            */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RWS_V, default=0x00000000*/

                            /*
                               UC: Error uncorrected flag Indicates (when set)
                               that the processor did not correct the error
                               condition. When clear, this flag indicates that
                               the processor was able to correct the error
                               condition.
                            */
    UINT32 Over : 1;

                            /* Bits[62:62], Access Type=RWS_V, default=0x00000000*/

                            /* OVER: Machine check overflow */
    UINT32 Val : 1;

                            /* Bits[63:63], Access Type=RWS_V, default=0x00000000*/

                            /*
                               VAL: Valid flag Indicates (when set) that the
                               information within the Mci_STATUS register is
                               valid. When this flag is set, the processor
                               follows the rules given for the OVER flag in the
                               Mci_STATUS register when overwriting previously
                               valid entries. The processor sets the VAL flag
                               and software is responsible for clearing it.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC19_STATUS_REGISTER;

/**
  EDRAM 0 MCA bank: Address register
**/
#define MSR_MC19_ADDR 0x0000044E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 AddressOffset : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Bits [5:0] of the Machine Check logged address.
                               Enchanced MCA feature made this field writable
                            */
    UINT32 Address0 : 26;

                            /* Bits[31:6], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 Address1 : 7;

                            /* Bits[38:32], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 EnhMcaAvail : 25;

                            /* Bits[63:39], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC19_ADDR_REGISTER;

/**
  EDRAM 1 Machine Check Architecture Control MSR.
**/
#define MSR_MC20_CTL 0x00000450

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 EdcDataUncorrError : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 AddrCmdParity : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 EtagUncorrError : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 EtagMultiMatchError : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 EtrogUnknownError : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 SaveRestoreUncorrError : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 Rsvd6 : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC20_CTL_REGISTER;

/**
  EDRAM 1 MCA bank: Status Register
**/
#define MSR_MC20_STATUS 0x00000451

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Mcacod : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /* MCA error code */
    UINT32 Mscod : 16;

                            /* Bits[31:16], Access Type=RWS_V, default=0x00000000*/

                            /* Model specific error code */
    UINT32 EnhMcaAvail0 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Available when Enhanced MCA is in use - unused
                               in etrog
                            */
    UINT32 CorrectedErrorCount : 15;

                            /* Bits[52:38], Access Type=RWS_V, default=0x00000000*/

                            /* Correctable error count */
    UINT32 ThresholdBasedErrorStatus : 2;

                            /* Bits[54:53], Access Type=RWS_V, default=0x00000001*/

                            /*
                               These bits are used to indicate when the number
                               of corrected errors has exceeded the safe
                               threshold to the point where an uncorrected
                               error has become more likely to happen.
                            */
    UINT32 Ar : 2;

                            /* Bits[56:55], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Available when Enhanced MCA is in use, unused in
                               etrog
                            */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RWS_V, default=0x00000000*/

                            /* PCC: Processor context corrupt */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RWS_V, default=0x00000000*/

                            /*
                               ADDRV: The address held in the MCi_ADDR_MSR is
                               valid
                            */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RWS_V, default=0x00000000*/

                            /*
                               MISCV: The value held in the MCi_MISC_MSR is
                               valid
                            */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RWS_V, default=0x00000000*/

                            /*
                               EN: Error enabled flag Indicates (when set) that
                               signaling of the machine check exception for
                               this error was enabled by an associated flag bit
                               of the Mci_CTL register.
                            */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RWS_V, default=0x00000000*/

                            /*
                               UC: Error uncorrected flag Indicates (when set)
                               that the processor did not correct the error
                               condition. When clear, this flag indicates that
                               the processor was able to correct the error
                               condition.
                            */
    UINT32 Over : 1;

                            /* Bits[62:62], Access Type=RWS_V, default=0x00000000*/

                            /* OVER: Machine check overflow */
    UINT32 Val : 1;

                            /* Bits[63:63], Access Type=RWS_V, default=0x00000000*/

                            /*
                               VAL: Valid flag Indicates (when set) that the
                               information within the Mci_STATUS register is
                               valid. When this flag is set, the processor
                               follows the rules given for the OVER flag in the
                               Mci_STATUS register when overwriting previously
                               valid entries. The processor sets the VAL flag
                               and software is responsible for clearing it.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC20_STATUS_REGISTER;

/**
  EDRAM 1 MCA bank: Address register
**/
#define MSR_MC20_ADDR 0x00000452

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 AddressOffset : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Bits [5:0] of the Machine Check logged address.
                               Enchanced MCA feature made this field writable
                            */
    UINT32 Address0 : 26;

                            /* Bits[31:6], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 Address1 : 7;

                            /* Bits[38:32], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 EnhMcaAvail : 25;

                            /* Bits[63:39], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC20_ADDR_REGISTER;

/**
  2LM MCA bank: Control register
**/
#define MSR_MC21_CTL 0x00000454

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 EnUncorrectableTcMultipleTagMatchError : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 EnUncorrectableTcTagError : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 EnUncorrectableSdtCorruptionError : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 EnUncorrectableAddressRangeError : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 EnCorrectableTcTagError : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 Rsvd5 : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC21_CTL_REGISTER;

/**
  2LM MCA bank: Status register
**/
#define MSR_MC21_STATUS 0x00000455

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Mcacod : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /* MCA error code */
    UINT32 Mscod : 16;

                            /* Bits[31:16], Access Type=RWS_V, default=0x00000000*/

                            /* Model specific error code */
    UINT32 EnhMcaAvail0 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Available when Enhanced MCA is in use - unused
                               in 2LMc
                            */
    UINT32 CorrectedErrorCount : 15;

                            /* Bits[52:38], Access Type=RWS_V, default=0x00000000*/

                            /* Correctable error count */
    UINT32 ThresholdBasedErrorStatus : 2;

                            /* Bits[54:53], Access Type=RWS_V, default=0x00000001*/

                            /*
                               These bits are used to indicate when the number
                               of corrected errors has exceeded the safe
                               threshold to the point where an uncorrected
                               error has become more likely to happen.
                            */
    UINT32 Ar : 2;

                            /* Bits[56:55], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Available when Enhanced MCA is in use, unused in
                               2LMc
                            */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RWS_V, default=0x00000000*/

                            /* PCC: Processor context corrupt */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RWS_V, default=0x00000000*/

                            /*
                               ADDRV: The address held in the MCi_ADDR_MSR is
                               valid
                            */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RWS_V, default=0x00000000*/

                            /*
                               MISCV: The value held in the MCi_MISC_MSR is
                               valid
                            */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RWS_V, default=0x00000000*/

                            /*
                               EN: Error enabled flag Indicates (when set) that
                               signaling of the machine check exception for
                               this error was enabled by an associated flag bit
                               of the Mci_CTL register.
                            */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RWS_V, default=0x00000000*/

                            /*
                               UC: Error uncorrected flag Indicates (when set)
                               that the processor did not correct the error
                               condition. When clear, this flag indicates that
                               the processor was able to correct the error
                               condition.
                            */
    UINT32 Over : 1;

                            /* Bits[62:62], Access Type=RWS_V, default=0x00000000*/

                            /* OVER: Machine check overflow */
    UINT32 Val : 1;

                            /* Bits[63:63], Access Type=RWS_V, default=0x00000000*/

                            /*
                               VAL: Valid flag Indicates (when set) that the
                               information within the Mci_STATUS register is
                               valid. When this flag is set, the processor
                               follows the rules given for the OVER flag in the
                               Mci_STATUS register when overwriting previously
                               valid entries. The processor sets the VAL flag
                               and software is responsible for clearing it.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC21_STATUS_REGISTER;

/**
  2LM MCA bank: Address register
**/
#define MSR_MC21_ADDR 0x00000456

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 AddressOffset : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Bits [5:0] of the Machine Check logged address.
                               Enchanced MCA feature made this field writable
                            */
    UINT32 Address0 : 26;

                            /* Bits[31:6], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 Address1 : 7;

                            /* Bits[38:32], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 EnhMcaAvail : 25;

                            /* Bits[63:39], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC21_ADDR_REGISTER;

/**
  2LM MCA bank: Miscellaneous register
**/
#define MSR_MC21_MISC 0x00000457

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ErrorAddress : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               This field contains the address of uncorrected
                               error seen in the 2LMC/MLMc. It is updated on an
                               uncorrectable error. To maintain e-MCA support
                               this register must be writable.
                            */
    UINT32 CorrectedErrorCount : 8;

                            /* Bits[23:16], Access Type=RWS_V, default=0x00000000*/

                            /* Corrected Error Count */
    UINT32 EnhMcaAvail0 : 8;

                            /* Bits[31:24], Access Type=RWS_V, default=0x00000000*/

                            /* Available when enhanced MCA is in use */
    UINT32 EnhMcaAvail1 : 32;

                            /* Bits[63:32], Access Type=RWS_V, default=0x00000000*/

                            /* Available when enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC21_MISC_REGISTER;

/**
  2LM MCA bank: Control register
**/
#define MSR_MC22_CTL 0x00000458

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 EnUncorrectableTcMultipleTagMatchError : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 EnUncorrectableTcTagError : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 EnUncorrectableSdtCorruptionError : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 EnUncorrectableAddressRangeError : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 EnCorrectableTcTagError : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 Rsvd5 : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC22_CTL_REGISTER;

/**
  2LM MCA bank: Status register
**/
#define MSR_MC22_STATUS 0x00000459

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Mcacod : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /* MCA error code */
    UINT32 Mscod : 16;

                            /* Bits[31:16], Access Type=RWS_V, default=0x00000000*/

                            /* Model specific error code */
    UINT32 EnhMcaAvail0 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Available when Enhanced MCA is in use - unused
                               in 2LMc
                            */
    UINT32 CorrectedErrorCount : 15;

                            /* Bits[52:38], Access Type=RWS_V, default=0x00000000*/

                            /* Correctable error count */
    UINT32 ThresholdBasedErrorStatus : 2;

                            /* Bits[54:53], Access Type=RWS_V, default=0x00000001*/

                            /*
                               These bits are used to indicate when the number
                               of corrected errors has exceeded the safe
                               threshold to the point where an uncorrected
                               error has become more likely to happen.
                            */
    UINT32 Ar : 2;

                            /* Bits[56:55], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Available when Enhanced MCA is in use, unused in
                               2LMc
                            */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RWS_V, default=0x00000000*/

                            /* PCC: Processor context corrupt */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RWS_V, default=0x00000000*/

                            /*
                               ADDRV: The address held in the MCi_ADDR_MSR is
                               valid
                            */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RWS_V, default=0x00000000*/

                            /*
                               MISCV: The value held in the MCi_MISC_MSR is
                               valid
                            */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RWS_V, default=0x00000000*/

                            /*
                               EN: Error enabled flag Indicates (when set) that
                               signaling of the machine check exception for
                               this error was enabled by an associated flag bit
                               of the Mci_CTL register.
                            */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RWS_V, default=0x00000000*/

                            /*
                               UC: Error uncorrected flag Indicates (when set)
                               that the processor did not correct the error
                               condition. When clear, this flag indicates that
                               the processor was able to correct the error
                               condition.
                            */
    UINT32 Over : 1;

                            /* Bits[62:62], Access Type=RWS_V, default=0x00000000*/

                            /* OVER: Machine check overflow */
    UINT32 Val : 1;

                            /* Bits[63:63], Access Type=RWS_V, default=0x00000000*/

                            /*
                               VAL: Valid flag Indicates (when set) that the
                               information within the Mci_STATUS register is
                               valid. When this flag is set, the processor
                               follows the rules given for the OVER flag in the
                               Mci_STATUS register when overwriting previously
                               valid entries. The processor sets the VAL flag
                               and software is responsible for clearing it.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC22_STATUS_REGISTER;

/**
  2LM MCA bank: Address register
**/
#define MSR_MC22_ADDR 0x0000045A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 AddressOffset : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Bits [5:0] of the Machine Check logged address.
                               Enchanced MCA feature made this field writable
                            */
    UINT32 Address0 : 26;

                            /* Bits[31:6], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 Address1 : 7;

                            /* Bits[38:32], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 EnhMcaAvail : 25;

                            /* Bits[63:39], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC22_ADDR_REGISTER;

/**
  2LM MCA bank: Miscellaneous register
**/
#define MSR_MC22_MISC 0x0000045B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ErrorAddress : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               This field contains the address of uncorrected
                               error seen in the 2LMC/MLMc. It is updated on an
                               uncorrectable error. To maintain e-MCA support
                               this register must be writable.
                            */
    UINT32 CorrectedErrorCount : 8;

                            /* Bits[23:16], Access Type=RWS_V, default=0x00000000*/

                            /* Corrected Error Count */
    UINT32 EnhMcaAvail0 : 8;

                            /* Bits[31:24], Access Type=RWS_V, default=0x00000000*/

                            /* Available when enhanced MCA is in use */
    UINT32 EnhMcaAvail1 : 32;

                            /* Bits[63:32], Access Type=RWS_V, default=0x00000000*/

                            /* Available when enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC22_MISC_REGISTER;

/**
  2LM MCA bank: Control register
**/
#define MSR_MC23_CTL 0x0000045C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 EnUncorrectableTcMultipleTagMatchError : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 EnUncorrectableTcTagError : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 EnUncorrectableSdtCorruptionError : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 EnUncorrectableAddressRangeError : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 EnCorrectableTcTagError : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 Rsvd5 : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC23_CTL_REGISTER;

/**
  2LM MCA bank: Status register
**/
#define MSR_MC23_STATUS 0x0000045D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Mcacod : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /* MCA error code */
    UINT32 Mscod : 16;

                            /* Bits[31:16], Access Type=RWS_V, default=0x00000000*/

                            /* Model specific error code */
    UINT32 EnhMcaAvail0 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Available when Enhanced MCA is in use - unused
                               in 2LMc
                            */
    UINT32 CorrectedErrorCount : 15;

                            /* Bits[52:38], Access Type=RWS_V, default=0x00000000*/

                            /* Correctable error count */
    UINT32 ThresholdBasedErrorStatus : 2;

                            /* Bits[54:53], Access Type=RWS_V, default=0x00000001*/

                            /*
                               These bits are used to indicate when the number
                               of corrected errors has exceeded the safe
                               threshold to the point where an uncorrected
                               error has become more likely to happen.
                            */
    UINT32 Ar : 2;

                            /* Bits[56:55], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Available when Enhanced MCA is in use, unused in
                               2LMc
                            */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RWS_V, default=0x00000000*/

                            /* PCC: Processor context corrupt */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RWS_V, default=0x00000000*/

                            /*
                               ADDRV: The address held in the MCi_ADDR_MSR is
                               valid
                            */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RWS_V, default=0x00000000*/

                            /*
                               MISCV: The value held in the MCi_MISC_MSR is
                               valid
                            */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RWS_V, default=0x00000000*/

                            /*
                               EN: Error enabled flag Indicates (when set) that
                               signaling of the machine check exception for
                               this error was enabled by an associated flag bit
                               of the Mci_CTL register.
                            */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RWS_V, default=0x00000000*/

                            /*
                               UC: Error uncorrected flag Indicates (when set)
                               that the processor did not correct the error
                               condition. When clear, this flag indicates that
                               the processor was able to correct the error
                               condition.
                            */
    UINT32 Over : 1;

                            /* Bits[62:62], Access Type=RWS_V, default=0x00000000*/

                            /* OVER: Machine check overflow */
    UINT32 Val : 1;

                            /* Bits[63:63], Access Type=RWS_V, default=0x00000000*/

                            /*
                               VAL: Valid flag Indicates (when set) that the
                               information within the Mci_STATUS register is
                               valid. When this flag is set, the processor
                               follows the rules given for the OVER flag in the
                               Mci_STATUS register when overwriting previously
                               valid entries. The processor sets the VAL flag
                               and software is responsible for clearing it.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC23_STATUS_REGISTER;

/**
  2LM MCA bank: Address register
**/
#define MSR_MC23_ADDR 0x0000045E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 AddressOffset : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Bits [5:0] of the Machine Check logged address.
                               Enchanced MCA feature made this field writable
                            */
    UINT32 Address0 : 26;

                            /* Bits[31:6], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 Address1 : 7;

                            /* Bits[38:32], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 EnhMcaAvail : 25;

                            /* Bits[63:39], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC23_ADDR_REGISTER;

/**
  2LM MCA bank: Miscellaneous register
**/
#define MSR_MC23_MISC 0x0000045F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ErrorAddress : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               This field contains the address of uncorrected
                               error seen in the 2LMC/MLMc. It is updated on an
                               uncorrectable error. To maintain e-MCA support
                               this register must be writable.
                            */
    UINT32 CorrectedErrorCount : 8;

                            /* Bits[23:16], Access Type=RWS_V, default=0x00000000*/

                            /* Corrected Error Count */
    UINT32 EnhMcaAvail0 : 8;

                            /* Bits[31:24], Access Type=RWS_V, default=0x00000000*/

                            /* Available when enhanced MCA is in use */
    UINT32 EnhMcaAvail1 : 32;

                            /* Bits[63:32], Access Type=RWS_V, default=0x00000000*/

                            /* Available when enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC23_MISC_REGISTER;

/**
  2LM MCA bank: Control register
**/
#define MSR_MC24_CTL 0x00000460

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 EnUncorrectableTcMultipleTagMatchError : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 EnUncorrectableTcTagError : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 EnUncorrectableSdtCorruptionError : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 EnUncorrectableAddressRangeError : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 EnCorrectableTcTagError : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Enable bit for MC signaling */
    UINT32 Rsvd5 : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC24_CTL_REGISTER;

/**
  2LM MCA bank: Status register
**/
#define MSR_MC24_STATUS 0x00000461

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Mcacod : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /* MCA error code */
    UINT32 Mscod : 16;

                            /* Bits[31:16], Access Type=RWS_V, default=0x00000000*/

                            /* Model specific error code */
    UINT32 EnhMcaAvail0 : 6;

                            /* Bits[37:32], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Available when Enhanced MCA is in use - unused
                               in 2LMc
                            */
    UINT32 CorrectedErrorCount : 15;

                            /* Bits[52:38], Access Type=RWS_V, default=0x00000000*/

                            /* Correctable error count */
    UINT32 ThresholdBasedErrorStatus : 2;

                            /* Bits[54:53], Access Type=RWS_V, default=0x00000001*/

                            /*
                               These bits are used to indicate when the number
                               of corrected errors has exceeded the safe
                               threshold to the point where an uncorrected
                               error has become more likely to happen.
                            */
    UINT32 Ar : 2;

                            /* Bits[56:55], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Available when Enhanced MCA is in use, unused in
                               2LMc
                            */
    UINT32 Pcc : 1;

                            /* Bits[57:57], Access Type=RWS_V, default=0x00000000*/

                            /* PCC: Processor context corrupt */
    UINT32 Addrv : 1;

                            /* Bits[58:58], Access Type=RWS_V, default=0x00000000*/

                            /*
                               ADDRV: The address held in the MCi_ADDR_MSR is
                               valid
                            */
    UINT32 Miscv : 1;

                            /* Bits[59:59], Access Type=RWS_V, default=0x00000000*/

                            /*
                               MISCV: The value held in the MCi_MISC_MSR is
                               valid
                            */
    UINT32 En : 1;

                            /* Bits[60:60], Access Type=RWS_V, default=0x00000000*/

                            /*
                               EN: Error enabled flag Indicates (when set) that
                               signaling of the machine check exception for
                               this error was enabled by an associated flag bit
                               of the Mci_CTL register.
                            */
    UINT32 Uc : 1;

                            /* Bits[61:61], Access Type=RWS_V, default=0x00000000*/

                            /*
                               UC: Error uncorrected flag Indicates (when set)
                               that the processor did not correct the error
                               condition. When clear, this flag indicates that
                               the processor was able to correct the error
                               condition.
                            */
    UINT32 Over : 1;

                            /* Bits[62:62], Access Type=RWS_V, default=0x00000000*/

                            /* OVER: Machine check overflow */
    UINT32 Val : 1;

                            /* Bits[63:63], Access Type=RWS_V, default=0x00000000*/

                            /*
                               VAL: Valid flag Indicates (when set) that the
                               information within the Mci_STATUS register is
                               valid. When this flag is set, the processor
                               follows the rules given for the OVER flag in the
                               Mci_STATUS register when overwriting previously
                               valid entries. The processor sets the VAL flag
                               and software is responsible for clearing it.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC24_STATUS_REGISTER;

/**
  2LM MCA bank: Address register
**/
#define MSR_MC24_ADDR 0x00000462

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 AddressOffset : 6;

                            /* Bits[5:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               Bits [5:0] of the Machine Check logged address.
                               Enchanced MCA feature made this field writable
                            */
    UINT32 Address0 : 26;

                            /* Bits[31:6], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 Address1 : 7;

                            /* Bits[38:32], Access Type=RWS_V, default=0x00000000*/

                            /* address of the logged error */
    UINT32 EnhMcaAvail : 25;

                            /* Bits[63:39], Access Type=RWS_V, default=0x00000000*/

                            /* Available when Enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC24_ADDR_REGISTER;

/**
  2LM MCA bank: Miscellaneous register
**/
#define MSR_MC24_MISC 0x00000463

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ErrorAddress : 16;

                            /* Bits[15:0], Access Type=RWS_V, default=0x00000000*/

                            /*
                               This field contains the address of uncorrected
                               error seen in the 2LMC/MLMc. It is updated on an
                               uncorrectable error. To maintain e-MCA support
                               this register must be writable.
                            */
    UINT32 CorrectedErrorCount : 8;

                            /* Bits[23:16], Access Type=RWS_V, default=0x00000000*/

                            /* Corrected Error Count */
    UINT32 EnhMcaAvail0 : 8;

                            /* Bits[31:24], Access Type=RWS_V, default=0x00000000*/

                            /* Available when enhanced MCA is in use */
    UINT32 EnhMcaAvail1 : 32;

                            /* Bits[63:32], Access Type=RWS_V, default=0x00000000*/

                            /* Available when enhanced MCA is in use */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MC24_MISC_REGISTER;
/**
  VMX Capability MSR. IA32_VMX_BASIC info.
**/
#define MSR_VMX_MSR_0 0x00000480
/**
  VMX Capability MSR. allowed setting for PIN BASED VMCS execution control
**/
#define MSR_VMX_MSR_1 0x00000481
/**
  VMX Capability MSR.allowed setting for Processor-based VM-execution controls
**/
#define MSR_VMX_MSR_2 0x00000482
/**
  VMX Capability MSR.allowed setting for VMEXIT controls
**/
#define MSR_VMX_MSR_3 0x00000483
/**
  VMX Capability MSR.allowed setting for VMENTRY controls
**/
#define MSR_VMX_MSR_4 0x00000484
/**
  VMX Capability MSR.info on preemption timer, STM and more.
**/
#define MSR_VMX_MSR_5 0x00000485
/**
  VMX Capability MSR.Allowed 0-settings for CR0 in VMX mode.
**/
#define MSR_VMX_MSR_6 0x00000486
/**
  VMX Capability MSR.Allowed 1-settings for CR0 in VMX mode.
**/
#define MSR_VMX_MSR_7 0x00000487
/**
  VMX Capability MSR.Allowed 0-settings for CR4 in VMX mode.
**/
#define MSR_VMX_MSR_8 0x00000488
/**
  VMX Capability MSR.Allowed 1-settings for CR4 in VMX mode.
**/
#define MSR_VMX_MSR_9 0x00000489
/**
  VMX Capability MSR.VMCS encoding info.
**/
#define MSR_VMX_MSR_10 0x0000048A
/**
  VMX Capability MSR.allowed setting for SECONDARY PROCESSOR-BASED CTLS.
**/
#define MSR_VMX_MSR_11 0x0000048B
/**
  VMX Capability MSR.EPT CAPABILITIES.
**/
#define MSR_VMX_MSR_12 0x0000048C
/**
  VMX Capability MSR. allowed setting for PIN BASED VMCS execution control
**/
#define MSR_VMX_TRUE_PINBASED_CTLS 0x0000048D
/**
  Capability Reporting Register of Primary Processor-based VM-execution Flex Controls
**/
#define MSR_VMX_TRUE_PROCBASED_CTLS 0x0000048E
/**
  Capability Reporting Register of VM-exit Flex Controls
**/
#define MSR_VMX_TRUE_EXIT_CTLS 0x0000048F
/**
  Capability Reporting Register of VM-entry Flex Controls
**/
#define MSR_VMX_TRUE_ENTRY_CTLS 0x00000490
/**
  VMX Capability MSR.
**/
#define MSR_VMX_VMFUNC_CTLS 0x00000491
/**
  Allowed 1-settings of tertiary processor-based VM-execution controls.
**/
#define MSR_VMX_PROCBASED_CTLS3 0x00000492

/**
  Full Width Writable IA32_PMC0 Alias
**/
#define MSR_EMON_FULL_CTR0 0x000004C1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Counter */
    UINT32 Rsvd32 : 16;

                            /* Bits[47:32], Access Type=RW, default=0x00000000*/

                            /* Counter */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_FULL_CTR0_REGISTER;

/**
  Full Width Writable IA32_PMC1 Alias
**/
#define MSR_EMON_FULL_CTR1 0x000004C2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Counter */
    UINT32 Rsvd32 : 16;

                            /* Bits[47:32], Access Type=RW, default=0x00000000*/

                            /* Counter */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_FULL_CTR1_REGISTER;

/**
  Full Width Writable IA32_PMC2 Alias
**/
#define MSR_EMON_FULL_CTR2 0x000004C3

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Counter */
    UINT32 Rsvd32 : 16;

                            /* Bits[47:32], Access Type=RW, default=0x00000000*/

                            /* Counter */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_FULL_CTR2_REGISTER;

/**
  Full Width Writable IA32_PMC3 Alias
**/
#define MSR_EMON_FULL_CTR3 0x000004C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Counter */
    UINT32 Rsvd32 : 16;

                            /* Bits[47:32], Access Type=RW, default=0x00000000*/

                            /* Counter */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_FULL_CTR3_REGISTER;

/**
  Full Width Writable IA32_PMC4 Alias
**/
#define MSR_EMON_FULL_CTR4 0x000004C5

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Counter */
    UINT32 Rsvd32 : 16;

                            /* Bits[47:32], Access Type=RW, default=0x00000000*/

                            /* Counter */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_FULL_CTR4_REGISTER;

/**
  Full Width Writable IA32_PMC5 Alias
**/
#define MSR_EMON_FULL_CTR5 0x000004C6

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Counter */
    UINT32 Rsvd32 : 16;

                            /* Bits[47:32], Access Type=RW, default=0x00000000*/

                            /* Counter */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_FULL_CTR5_REGISTER;

/**
  Full Width Writable IA32_PMC6 Alias
**/
#define MSR_EMON_FULL_CTR6 0x000004C7

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Counter */
    UINT32 Rsvd32 : 16;

                            /* Bits[47:32], Access Type=RW, default=0x00000000*/

                            /* Counter */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_FULL_CTR6_REGISTER;

/**
  Full Width Writable IA32_PMC7 Alias
**/
#define MSR_EMON_FULL_CTR7 0x000004C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Counter */
    UINT32 Rsvd32 : 16;

                            /* Bits[47:32], Access Type=RW, default=0x00000000*/

                            /* Counter */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EMON_FULL_CTR7_REGISTER;

/**
  Pointer to current RTIT Table of Physical Addresses (ToPA)
**/
#define MSR_RTIT_OUTPUT_BASE 0x00000560

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 7;

                            /* Bits[6:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 BasePhysAddrLsb : 5;

                            /* Bits[11:7], Access Type=RW, default=None*/

                            /*
                               RTIT output base physical address (low bits).
                               How this address is used depends on the value of
                               IA32_RTIT_CTL.ToPA: 0: This is the LSB of base
                               physical address of a single, contiguous
                               physical output region. This could be mapped to
                               DRAM or to MMIO, depending on the value. 1: In
                               ToPA mode this field is reserved - ERROR is not
                               aligned to 4KB
                            */
    UINT32 BasePhysAddrMsb0 : 20;

                            /* Bits[31:12], Access Type=RW, default=None*/

                            /*
                               RTIT output base physical address (high bits).
                               How this address is used depends on the value of
                               IA32_RTIT_CTL.ToPA: 0: This is the MSB of base
                               physical address of a single, contiguous
                               physical output region. This could be mapped to
                               DRAM or to MMIO, depending on the value. 1: This
                               is the MSB of base physical address of the first
                               ToPA table. In this case, the address must be 4K
                               aligned, and thus bits 11:7 checked for ERROR
                            */
    UINT32 BasePhysAddrMsb1 : 20;

                            /* Bits[51:32], Access Type=RW, default=None*/

                            /*
                               RTIT output base physical address (high bits).
                               How this address is used depends on the value of
                               IA32_RTIT_CTL.ToPA: 0: This is the MSB of base
                               physical address of a single, contiguous
                               physical output region. This could be mapped to
                               DRAM or to MMIO, depending on the value. 1: This
                               is the MSB of base physical address of the first
                               ToPA table. In this case, the address must be 4K
                               aligned, and thus bits 11:7 checked for ERROR
                            */
    UINT32 Rsvd52 : 12;

                            /* Bits[63:52], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_RTIT_OUTPUT_BASE_REGISTER;

/**
  Holds pointers to the current output region (page) in the current ToPA
**/
#define MSR_RTIT_OUTPUT_MASK_PTRS 0x00000561

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Lower : 7;

                            /* Bits[6:0], Access Type=RW, default=None*/

                            /*
                               Reserved - always '1 to enable using this MSR as
                               a mask
                            */
    UINT32 MaskTableOffset : 25;

                            /* Bits[31:7], Access Type=RW, default=None*/

                            /*
                               [27:3] - Index into the current RTIT Table entry
                               (8B granularity)
                            */
    UINT32 OutputOffset : 32;

                            /* Bits[63:32], Access Type=RW, default=None*/

                            /*
                               [31:0] - Index into the current RTIT output
                               region (1B granularity)
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_RTIT_OUTPUT_MASK_PTRS_REGISTER;

/**
  RTIT Control MSR
**/
#define MSR_RTIT_CTL 0x00000570

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 TraceEn : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable RTIT Trace on thread */
    UINT32 CycEn : 1;

                            /* Bits[1:1], Access Type=RW, default=None*/

                            /* Enable Cycle Pkt */
    UINT32 Os : 1;

                            /* Bits[2:2], Access Type=RW, default=None*/

                            /*
                               When set, only branches in ring 0 (OS mode) will
                               get tracked for RTIT. When both CPL_EQ_0 and
                               CPL_NEQ_0 bits are set, all branches will get
                               tracked for RTIT.
                            */
    UINT32 Usr : 1;

                            /* Bits[3:3], Access Type=RW, default=None*/

                            /*
                               When set, only branches in non-ring 0 (User
                               mode) will get tracked for RTIT. When both
                               CPL_EQ_0 and CPL_NEQ_0 bits are set, all
                               branches will get tracked for RTIT.
                            */
    UINT32 Rsvd4 : 3;

                            /* Bits[6:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Cr3FilterEn : 1;

                            /* Bits[7:7], Access Type=RW, default=None*/

                            /*
                               Enable CR3 filtering, such that tracing is
                               enabled only if CR3 matches the value in
                               IA32_RTIT_CR3
                            */
    UINT32 TopaEn : 1;

                            /* Bits[8:8], Access Type=RW, default=None*/

                            /*
                               When set causes the RTIT output to be in memory
                               via ToPA, else to a single address range
                            */
    UINT32 MtcEn : 1;

                            /* Bits[9:9], Access Type=RW, default=None*/

                            /* Enable mini time stamp counter */
    UINT32 TscEn : 1;

                            /* Bits[10:10], Access Type=RW, default=None*/

                            /*
                               Enable TSC packets, on RTIT enable as well as on
                               bus ratio changes (with CBR). Implies that CBR
                               is also enabled.
                            */
    UINT32 DisableRetComp : 1;

                            /* Bits[11:11], Access Type=RW, default=None*/

                            /* When set, RETURN compression will be disabled */
    UINT32 Rsvd12 : 1;

                            /* Bits[12:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 BranchEn : 1;

                            /* Bits[13:13], Access Type=RW, default=None*/

                            /*
                               0: Disable COFI-based packets; 1: Enable COFI-
                               based packets (FUP, TIP, TNT, PIP, CSB);
                            */
    UINT32 MtcFreq : 4;

                            /* Bits[17:14], Access Type=RW, default=None*/

                            /*
                               Defines MTC Frequency, which is based on the HW
                               Crystal Clock (CTC). MTC will be sent each time
                               the selected CTC bit toggles. Encodings: 0:
                               CTC[0] 4: CTC[4] 8: CTC[8] 12: CTC[12] 1: CTC[1]
                               5: CTC[5] 9: CTC[9] 13: CTC[13] 2: CTC[2] 6:
                               CTC[6] 10: CTC[10] 14: CTC[14] 3: CTC[3] 7:
                               CTC[7] 11: CTC[11] 15: CTC[15]
                            */
    UINT32 Rsvd18 : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CycThresh : 4;

                            /* Bits[22:19], Access Type=RW, default=None*/

                            /*
                               CYC packet threshold, see 4.53.4 for details.
                               CYC packets will be sent with the first eligible
                               packet after N cycles have passed since the last
                               CYC packet. If CycThresh is 0 then N=0,
                               otherwise N is defined as 2(CycThresh-1).
                               Encodings: 0: 0 4: 8 8: 128 12: 2048 1: 1 5: 16
                               9: 256 13: 4096 2: 2 6: 32 10: 512 14: 8192 3: 4
                               7: 64 11: 1024 15: 16384
                            */
    UINT32 Rsvd23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 PsbFreq : 4;

                            /* Bits[27:24], Access Type=RW, default=None*/

                            /*
                               Indicates the frequency of PSB packets. PSB
                               packet frequency is based on the number of RTIT
                               packet bytes output, so this field allows the
                               user to determine the increment of
                               IA32_IA32_RTIT_STATUS. PacketByteCnt that should
                               cause a PSB to be generated. Note that PSB
                               insertion is not precise, but the average output
                               bytes per PSB should approximate the SW selected
                               period. The average PSB period, in RTIT output
                               bytes, can be expressed as 2(PSBFreq+11).
                               Encodings: 0: 2K 4: 32K 8: 512K 12: 8M 1: 4K 5:
                               64K 9: 1M 13: 16M 2: 8K 6: 128K 10: 2M 14: 32M
                               3: 16K 7: 256K 11: 4M 15: 64M
                            */
    UINT32 Rsvd28 : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Addr0Cfg : 4;

                            /* Bits[35:32], Access Type=RW, default=None*/

                            /*
                               0x0 No address filtering 0x1 Enable tracing when
                               IP in [A..B] range 0x2 Stop tracing when IP in
                               [A..B] range 0x3..0xf Reserved
                            */
    UINT32 Addr1Cfg : 4;

                            /* Bits[39:36], Access Type=RW, default=None*/

                            /*
                               0x0 No address filtering 0x1 Enable tracing when
                               IP in [A..B] range 0x2 Stop tracing when IP in
                               [A..B] range 0x3..0xf Reserved
                            */
    UINT32 Rsvd40 : 16;

                            /* Bits[55:40], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 InjectPsbPmiOnEnable : 1;

                            /* Bits[56:56], Access Type=RW, default=None*/

                            /*
                               Enable preservation of PMI/PSB over
                               xsaves/xrstors. Note: #GP on SNC. May be enabled
                               by patch later. See ticket 1405267396
                            */
    UINT32 Rsvd57 : 7;

                            /* Bits[63:57], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_RTIT_CTL_REGISTER;

/**
  RTIT status MSR
**/
#define MSR_RTIT_STATUS 0x00000571

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 FilterEn : 1;

                            /* Bits[0:0], Access Type=RW_V, default=None*/

                            /*
                               Indicates that the LIP is within the filter
                               region. In SKL it compares the current EIP to
                               the EIP calculated when the LIP value was
                               programmed in IA32_RTIT_LIP0/2. FILTER_EN is not
                               writeable, but is re-evaluated by HW on TRACE
                               enabling
                            */
    UINT32 ContextEn : 1;

                            /* Bits[1:1], Access Type=RW_V, default=None*/

                            /*
                               Indicates that the processor is within a
                               traceable context (USR mode, OS mode, VMX mode,
                               SMM mode, SE etc)
                            */
    UINT32 TriggerEn : 1;

                            /* Bits[2:2], Access Type=RO_V, default=None*/

                            /*
                               Indicates that RTIT Tracing is enabled
                               (IA32_RTIT_CTL bits TRACE_EN and TRACE_ACTIVE
                               are set)
                            */
    UINT32 Rsvd3 : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Error : 1;

                            /* Bits[4:4], Access Type=RW_V, default=None*/

                            /*
                               Indicates that RTIT has hit an Error, such as
                               ToPA accessing a protected memory range
                            */
    UINT32 Stopped : 1;

                            /* Bits[5:5], Access Type=RW_V, default=None*/

                            /*
                               Indicates that RTIT Tracing Stopped when
                               branching into either a REGION_TRACE_STOP or a
                               TOPA_STOP
                            */
    UINT32 PendPsb : 1;

                            /* Bits[6:6], Access Type=RW_V, default=None*/

                            /*
                               Indicates that ucode/sw has pending PSB that
                               hasn't yet handled. Note: #GP on SNC. May be
                               enabled by patch later. See ticket 1405267396
                            */
    UINT32 PendTopaPmi : 1;

                            /* Bits[7:7], Access Type=RW_V, default=None*/

                            /*
                               Indicates that ucode/sw has pending PMI that
                               hasn't yet handled. Note: #GP on SNC. May be
                               enabled by patch later. See ticket 1405267396
                            */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 PacketByteCount : 17;

                            /* Bits[48:32], Access Type=RW_V, default=None*/

                            /*
                               Indicates how many Bytes have been traced so
                               far. This field is not cleared on TRACE_EN:=0.
                               Granularity of this counter in SKL is 8 Bytes.
                               Lower 3bits can change only if written by the
                               user as non-zeros
                            */
    UINT32 Rsvd49 : 15;

                            /* Bits[63:49], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_RTIT_STATUS_REGISTER;

/**
  CR3 value used for RTIT filtering
**/
#define MSR_RTIT_CR3_MATCH 0x00000572

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 5;

                            /* Bits[4:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 FilterValue0 : 27;

                            /* Bits[31:5], Access Type=RW, default=None*/

                            /*
                               Bits [63:5] to be compared against next updated
                               CR3 physical address. The unsupported MSBs, e.g.
                               [63:46] should be programmed to '0, else there
                               will not be a match.
                            */
    UINT32 FilterValue1 : 32;

                            /* Bits[63:32], Access Type=RW, default=None*/

                            /*
                               Bits [63:5] to be compared against next updated
                               CR3 physical address. The unsupported MSBs, e.g.
                               [63:46] should be programmed to '0, else there
                               will not be a match.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_RTIT_CR3_MATCH_REGISTER;

/**
  MSR used to support EIP filtering
**/
#define MSR_RTIT_ADDR0_A 0x00000580

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Addr0 : 32;

                            /* Bits[31:0], Access Type=RW, default=None*/

                            /*
                               Low end of LIP filter ADDR0. SKL transfers the
                               EIP to ROB
                            */
    UINT32 Addr1 : 25;

                            /* Bits[56:32], Access Type=RW, default=None*/

                            /*
                               Low end of LIP filter ADDR0. SKL transfers the
                               EIP to ROB
                            */
    UINT32 Rsvd57 : 7;

                            /* Bits[63:57], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_RTIT_ADDR0_A_REGISTER;

/**
  MSR used to support EIP filtering
**/
#define MSR_RTIT_ADDR0_B 0x00000581

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Addr0 : 32;

                            /* Bits[31:0], Access Type=RW, default=None*/

                            /*
                               High end of LIP filter ADDR0. SKL transfers the
                               EIP to ROB
                            */
    UINT32 Addr1 : 25;

                            /* Bits[56:32], Access Type=RW, default=None*/

                            /*
                               High end of LIP filter ADDR0. SKL transfers the
                               EIP to ROB
                            */
    UINT32 Rsvd57 : 7;

                            /* Bits[63:57], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_RTIT_ADDR0_B_REGISTER;

/**
  MSR used to support EIP filtering
**/
#define MSR_RTIT_ADDR1_A 0x00000582

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Addr0 : 32;

                            /* Bits[31:0], Access Type=RW, default=None*/

                            /*
                               Low end of LIP filter ADDR1. SKL transfers the
                               EIP to ROB
                            */
    UINT32 Addr1 : 25;

                            /* Bits[56:32], Access Type=RW, default=None*/

                            /*
                               Low end of LIP filter ADDR1. SKL transfers the
                               EIP to ROB
                            */
    UINT32 Rsvd57 : 7;

                            /* Bits[63:57], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_RTIT_ADDR1_A_REGISTER;

/**
  MSR used to support EIP filtering
**/
#define MSR_RTIT_ADDR1_B 0x00000583

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Addr0 : 32;

                            /* Bits[31:0], Access Type=RW, default=None*/

                            /*
                               High end of LIP filter ADDR1. SKL transfers the
                               EIP to ROB
                            */
    UINT32 Addr1 : 25;

                            /* Bits[56:32], Access Type=RW, default=None*/

                            /*
                               High end of LIP filter ADDR1. SKL transfers the
                               EIP to ROB
                            */
    UINT32 Rsvd57 : 7;

                            /* Bits[63:57], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_RTIT_ADDR1_B_REGISTER;
/**
  Points to the linear address of the first byte of the DS buffer management area, which is used to manage the BTS and PEBS buffers.
**/
#define MSR_DS_AREA 0x00000600
/**
  productivity cycles counter (HWP)
**/
#define MSR_MLC_PCNT_COUNTER 0x0000064E

/**
  Fast wrmsr/rdmsr control MSR. Each bit refers to one MSR and indicate if this MSR
  is set to use the FAST_UNCORE_MSRS mode. Currently , bit 0 refers to IA32_HWP_REQUEST , while bit 1 refers to THREAD_INFO MSR
**/
#define MSR_FAST_UNCORE_MSRS_CTL 0x00000657

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 FastAccessEnable : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /* indicates, for each msr, if we're in fast_mode */
    UINT32 Rsvd4 : 28;

                            /* Bits[31:4], Access Type=RW, default=0x00000000*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_FAST_UNCORE_MSRS_CTL_REGISTER;

/**
  CET user configuration MSR
**/
#define MSR_U_CET 0x000006A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ShStkEn : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable shadow stacks */
    UINT32 WrShstkEn : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable WRSS instruction */
    UINT32 EndbrEn : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enable indirect call/jmp tracking */
    UINT32 LegiwEn : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Enable legacy interworking for indirect
                               calls/jumps tracking
                            */
    UINT32 NoTrackEn : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Enables use of 3E byte as "no-track" prefix for
                               indirect calls and jumps
                            */
    UINT32 SuppressDis : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               disables suppression of CET indirect branch
                               tracking on legacy interwork (each violation
                               requires re-check via bitmap)
                            */
    UINT32 Rsvd6 : 4;

                            /* Bits[9:6], Access Type=RW, default=0x00000000*/

                            /* reserved */
    UINT32 Suppress : 1;

                            /* Bits[10:10], Access Type=RW_V, default=0x00000000*/

                            /* Suppresses state of ENDBRANCH tracker4 */
    UINT32 Tracker : 1;

                            /* Bits[11:11], Access Type=RW_V, default=0x00000000*/

                            /*
                               State of ENDBRANCH tracker [0: idle, 1: wait for
                               endbranch]
                            */
    UINT32 EbLegBitmapBase0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* Linear address of bitmap of legacy code pages */
    UINT32 EbLegBitmapBase1 : 32;

                            /* Bits[63:32], Access Type=RW, default=0x00000000*/

                            /* Linear address of bitmap of legacy code pages */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_U_CET_REGISTER;

/**
  CET supervisor configuration MSR
**/
#define MSR_S_CET 0x000006A2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ShStkEn : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable shadow stacks */
    UINT32 WrShstkEn : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable WRSS instruction */
    UINT32 EndbrEn : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enable indirect call/jmp tracking */
    UINT32 LegiwEn : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Enable legacy interworking for indirect
                               calls/jumps tracking
                            */
    UINT32 NoTrackEn : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Enables use of 3E byte as "no-track" prefix for
                               indirect calls and jumps
                            */
    UINT32 SuppressDis : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               disables suppression of CET indirect branch
                               tracking on legacy interwork (each violation
                               requires re-check via bitmap)
                            */
    UINT32 Rsvd6 : 4;

                            /* Bits[9:6], Access Type=RW, default=0x00000000*/

                            /* reserved */
    UINT32 Suppress : 1;

                            /* Bits[10:10], Access Type=RW_V, default=0x00000000*/

                            /* Suppresses state of ENDBRANCH tracker4 */
    UINT32 Tracker : 1;

                            /* Bits[11:11], Access Type=RW_V, default=0x00000000*/

                            /*
                               State of ENDBRANCH tracker [0: idle, 1: wait for
                               endbranch]
                            */
    UINT32 EbLegBitmapBase0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* Linear address of bitmap of legacy code pages */
    UINT32 EbLegBitmapBase1 : 32;

                            /* Bits[63:32], Access Type=RW, default=0x00000000*/

                            /* Linear address of bitmap of legacy code pages */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_S_CET_REGISTER;
/**
  CET register to hold CPL0s shadow stack pointer on transition to CPL3
**/
#define MSR_PL0_SSP 0x000006A4
/**
  CET register to hold CPL1s shadow stack pointer on transition to CPL3
**/
#define MSR_PL1_SSP 0x000006A5
/**
  CET register to hold CPL2s shadow stack pointer on transition to CPL3
**/
#define MSR_PL2_SSP 0x000006A6
/**
  CET register to hold CPL3s shadow stack pointer on transition to CPL &lt; 3
**/
#define MSR_PL3_SSP 0x000006A7
/**
  linear address of the table of pointers to shadow stacks to be switched to when initiating a stack switch in 64 bit mode through IST mechanism. Must be machine canonical when written in 64 bit mode. On parts that do not support 64 bit mode, this MSR is not present.
**/
#define MSR_INTERRUPT_SSP_TABLE 0x000006A8

/**
  Interface to allow software to determine what is causing resolved frequency to be clamped
  below the requested frequency. Status bits are updated by pcode through the io interface IO_GT_PERF_LIMIT,
  log bits are set by hw on a status bit edge dected and cleared by a SW write of '0'.
**/
#define MSR_GT_PERF_LIMIT_REASONS 0x000006B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Prochot : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               PROCHOT# Status, RO, When set by PCODE indicates
                               that PROCHOT# has cause GT frequency clipping
                            */
    UINT32 Thermal : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Thermal Status, RO, When set by PCODE indicates
                               that Thermal event has cause GT frequency
                               clipping
                            */
    UINT32 SpareGt2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 SpareGt3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 RsrLimit : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 Ratl : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Running average thermal limit Status, R0, When
                               set by PCODE indicates that Running average
                               thermal limit has cause GT frequency clipping
                            */
    UINT32 VrThermalert : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Hot VR (any processor VR) Status, RO, When set
                               by PCODE indicates that Hot VR (any processor
                               VR) has cause GT frequency clipping
                            */
    UINT32 VrTdc : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               VR TDC (Thermal design current) Status, RO, When
                               set by PCODE indicates that VR TDC (Thermal
                               design current has cause GT frequency clipping
                            */
    UINT32 Other : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Other (IccMax, PL4, etc) Status, RO, When set by
                               PCODE indicates that other has cause reason GT
                               frequency clipping
                            */
    UINT32 SpareGt9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 PbmPl1 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               PBM PL1 (pkg, platform), RO, When set by PCODE
                               indicates that PBM PL1 (package or platform PL1)
                               has cause GT frequency clipping
                            */
    UINT32 PbmPl2 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               PBM PL2, PL3 (pkg, platform) Status, RO, When
                               set by PCODE indicates that PBM PL2 or
                               PL3(package or platform PL2 or PL3) has cause GT
                               frequency clipping
                            */
    UINT32 InefficientOperation : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Inefficient operation Status, RO, The current GT
                               Frequency lower than the DCC target Frequency
                            */
    UINT32 SpareGt13 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 SpareGt14 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 SpareGt15 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 ProchotLog : 1;

                            /* Bits[16:16], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               PROCHOT# Log, RW, When set by PCODE indicates
                               that PROCHOT# has cause GT frequency clipping.
                               Software should write to this bit to clear the
                               status in this bit
                            */
    UINT32 ThermalLog : 1;

                            /* Bits[17:17], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Thermal Log, RW, When set by PCODE indicates
                               that Thermal event has cause GT frequency
                               clipping. Software should write to this bit to
                               clear the status in this bit
                            */
    UINT32 SpareGtLog2 : 1;

                            /* Bits[18:18], Access Type=RW0C_FW, default=0x00000000*/

                            /* Reserved */
    UINT32 SpareGt3Log : 1;

                            /* Bits[19:19], Access Type=RW0C_FW, default=0x00000000*/

                            /* Reserved */
    UINT32 RsrLimitLog : 1;

                            /* Bits[20:20], Access Type=RW0C_FW, default=0x00000000*/

                            /* Reserved */
    UINT32 RatlLog : 1;

                            /* Bits[21:21], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Running average thermal limit Log, RW, When set
                               by PCODE indicates that Running average thermal
                               limit has cause GT frequency clipping. Software
                               should write to this bit to clear the status in
                               this bit
                            */
    UINT32 VrThermalertLog : 1;

                            /* Bits[22:22], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Hot VR (any processor VR) Log, RW, When set by
                               PCODE indicates that Hot VR (any processor VR)
                               has cause GT frequency clipping. Software should
                               write to this bit to clear the status in this
                               bit
                            */
    UINT32 VrTdcLog : 1;

                            /* Bits[23:23], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               VR TDC (Thermal design current) Log, RW, When
                               set by PCODE indicates that VR TDC (Thermal
                               design current has cause GT frequency clipping.
                               Software should write to this bit to clear the
                               status in this bit
                            */
    UINT32 OtherLog : 1;

                            /* Bits[24:24], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Other (IccMax, PL4, etc) Log, RW, When set by
                               PCODE indicates that other has cause reason GT
                               frequency clipping. Software should write to
                               this bit to clear the status in this bit
                            */
    UINT32 SpareGt9Log : 1;

                            /* Bits[25:25], Access Type=RW0C_FW, default=0x00000000*/

                            /* Reserved */
    UINT32 PbmPl1Log : 1;

                            /* Bits[26:26], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               PBM PL1 (pkg, platform) Log, RW, When set by
                               PCODE indicates that PBM PL1 (package or
                               platform PL1) has cause GT frequency clipping.
                               Software should write to this bit to clear the
                               status in this bit
                            */
    UINT32 PbmPl2Log : 1;

                            /* Bits[27:27], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               PBM PL2, PL3 (pkg, platform) Log, RW, When set
                               by PCODE indicates that PBM PL2 or PL3(package
                               or platform PL2 or PL3) has cause GT frequency
                               clipping. Software should write to this bit to
                               clear the status in this bit
                            */
    UINT32 InefficientOperationLog : 1;

                            /* Bits[28:28], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Inefficient operation Log, RW, The current GT
                               Frequency lower than the DCC target Frequency.
                               Software should write to this bit to clear the
                               status in this bit
                            */
    UINT32 SpareGt13Log : 1;

                            /* Bits[29:29], Access Type=RW0C_FW, default=0x00000000*/

                            /* Reserved */
    UINT32 SpareGt14Log : 1;

                            /* Bits[30:30], Access Type=RW0C_FW, default=0x00000000*/

                            /* Reserved */
    UINT32 SpareGt15Log : 1;

                            /* Bits[31:31], Access Type=RW0C_FW, default=0x00000000*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_GT_PERF_LIMIT_REASONS_REGISTER;

/**
  Interface to allow software to determine what is causing resolved frequency to be clamped
  below the requested frequency. Status bits are updated by pcode through the io interface IO_CLR_PERF_LIMIT,
  log bits are set by hw on a status bit edge dected and cleared by a SW write of '0'.
**/
#define MSR_CLR_PERF_LIMIT_REASONS 0x000006B1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Prochot : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               PROCHOT# Status, RO, When set by PCODE indicates
                               that PROCHOT# has cause RING frequency clipping
                            */
    UINT32 Thermal : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Thermal Status, RO, When set by PCODE indicates
                               that Thermal event has cause RING frequency
                               clipping
                            */
    UINT32 SpareClr2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 SpareClr3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 Rsr : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 Ratl : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Running average thermal limit Status, R0, When
                               set by PCODE indicates that Running average
                               thermal limit has cause RING frequency clipping
                            */
    UINT32 VrThermalert : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Hot VR (any processor VR) Status, RO, When set
                               by PCODE indicates that Hot VR (any processor
                               VR) has cause RING frequency clipping
                            */
    UINT32 VrTdc : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               VR TDC (Thermal design current) Status, RO, When
                               set by PCODE indicates that VR TDC (Thermal
                               design current has cause RING frequency clipping
                            */
    UINT32 Other : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Other (IccMax, PL4, etc) Status, RO, When set by
                               PCODE indicates that other has cause reason RING
                               frequency clipping
                            */
    UINT32 SpareClr9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 PbmPl1 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               PBM PL1 (pkg, platform), RO, When set by PCODE
                               indicates that PBM PL1 (package or platform PL1)
                               has cause RING frequency clipping
                            */
    UINT32 PbmPl2 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               PBM PL2, PL3 (pkg, platform) Status, RO, When
                               set by PCODE indicates that PBM PL2 or
                               PL3(package or platform PL2 or PL3) has cause
                               RING frequency clipping
                            */
    UINT32 SpareClr12 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 SpareClr13 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 SpareClr14 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 SpareClr15 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 ProchotLog : 1;

                            /* Bits[16:16], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               PROCHOT# Log, RW, When set by PCODE indicates
                               that PROCHOT# has cause RING frequency clipping.
                               Software should write to this bit to clear the
                               status in this bit
                            */
    UINT32 ThermalLog : 1;

                            /* Bits[17:17], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Thermal Log, RW, When set by PCODE indicates
                               that Thermal event has cause RING frequency
                               clipping. Software should write to this bit to
                               clear the status in this bit
                            */
    UINT32 SpareClr2Log : 1;

                            /* Bits[18:18], Access Type=RW0C_FW, default=0x00000000*/

                            /* Reserved */
    UINT32 SpareClr3Log : 1;

                            /* Bits[19:19], Access Type=RW0C_FW, default=0x00000000*/

                            /* Reserved */
    UINT32 RsrLog : 1;

                            /* Bits[20:20], Access Type=RW0C_FW, default=0x00000000*/

                            /* Reserved */
    UINT32 RatlLog : 1;

                            /* Bits[21:21], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Running average thermal limit Log, RW, When set
                               by PCODE indicates that Running average thermal
                               limit has cause RING frequency clipping.
                               Software should write to this bit to clear the
                               status in this bit
                            */
    UINT32 VrThermalertLog : 1;

                            /* Bits[22:22], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Hot VR (any processor VR) Log, RW, When set by
                               PCODE indicates that Hot VR (any processor VR)
                               has cause RING frequency clipping. Software
                               should write to this bit to clear the status in
                               this bit
                            */
    UINT32 VrTdcLog : 1;

                            /* Bits[23:23], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               VR TDC (Thermal design current) Log, RW, When
                               set by PCODE indicates that VR TDC (Thermal
                               design current has cause RING frequency
                               clipping. Software should write to this bit to
                               clear the status in this bit
                            */
    UINT32 OtherLog : 1;

                            /* Bits[24:24], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Other (IccMax, PL4, etc) Log, RW, When set by
                               PCODE indicates that other has cause reason RING
                               frequency clipping. Software should write to
                               this bit to clear the status in this bit
                            */
    UINT32 SpareClr9Log : 1;

                            /* Bits[25:25], Access Type=RW0C_FW, default=0x00000000*/

                            /* Reserved */
    UINT32 PbmPl1Log : 1;

                            /* Bits[26:26], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               PBM PL1 (pkg, platform) Log, RW, When set by
                               PCODE indicates that PBM PL1 (package or
                               platform PL1) has cause RING frequency clipping.
                               Software should write to this bit to clear the
                               status in this bit
                            */
    UINT32 PbmPl2Log : 1;

                            /* Bits[27:27], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               PBM PL2, PL3 (pkg, platform) Log, RW, When set
                               by PCODE indicates that PBM PL2 or PL3(package
                               or platform PL2 or PL3) has cause RING frequency
                               clipping. Software should write to this bit to
                               clear the status in this bit
                            */
    UINT32 SpareClr12Log : 1;

                            /* Bits[28:28], Access Type=RW0C_FW, default=0x00000000*/

                            /* Reserved */
    UINT32 SpareClr13Log : 1;

                            /* Bits[29:29], Access Type=RW0C_FW, default=0x00000000*/

                            /* Reserved */
    UINT32 SpareClr14Log : 1;

                            /* Bits[30:30], Access Type=RW0C_FW, default=0x00000000*/

                            /* Reserved */
    UINT32 SpareClr15Log : 1;

                            /* Bits[31:31], Access Type=RW0C_FW, default=0x00000000*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_CLR_PERF_LIMIT_REASONS_REGISTER;
/**
  TSC Target of Local APIC's TSC Deadline Mode
**/
#define MSR_TSC_DEADLINE 0x000006E0

/**
  Used to convey the OSs performance request {Min, Max. Desired, tc} to apply across threads if requested.
**/
#define MSR_HWP_REQUEST_PKG 0x00000772

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 MinimumPerformance : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000001*/

                            /*
                               Limits the instantaneous minimum performance
                               while in C0. HW will observe this constraint
                               unless and until an overriding condition occurs
                               (e.g. thermal). Set to zero (0) if SW does not
                               care (no limit). Default = zero (0).
                            */
    UINT32 MaximumPerformance : 8;

                            /* Bits[15:8], Access Type=RW, default=0x000000FF*/

                            /*
                               Limits the instantaneous maximum performance
                               level while in C0. Set to all ones (1s) if SW
                               does not care (no limit). Default = all ones
                               (1s).
                            */
    UINT32 DesiredPerformance : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /*
                               When set to zero, HW autonomous selection
                               determines the performance target. When set to a
                               non-zero value, in the range of Lowest
                               Performance to Highest Performance
                               (IA32_HWP_CAPABILITIES), conveys an explicit
                               performance request hint to the hardware;
                               effectively disabling HW Autonomous selection.
                               The Desired Performance input is non-
                               constraining in terms of Performance and Energy
                               Efficiency optimizations, which are
                               independently controlled.
                            */
    UINT32 EnergyPerformancePreference : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000080*/

                            /* tbd */
    UINT32 ActivityWindow : 10;

                            /* Bits[41:32], Access Type=RW, default=0x00000000*/

                            /*
                               Used as a hint to the H/W. The OS writes a time
                               value (10^3-bit exp * 7-bit mantissa in 1sec
                               units: 1us to 1270 sec) to this field to
                               indicate a moving workload history observation
                               window. Combined with the Energy Performance
                               Preference input, influences the rate of
                               performance increase / decrease. This field only
                               has meaning when Desired Performance = 0. 41:39
                               Exponent 38:32 Mantissa
                            */
    UINT32 ReservedForEco42 : 6;

                            /* Bits[47:42], Access Type=RW, default=0x00000000*/

                            /* Reserved_for_eco */
    UINT32 Reserved48 : 16;

                            /* Bits[63:48], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_HWP_REQUEST_PKG_REGISTER;

/**

**/
#define MSR_PECI_HWP_REQUEST_INFO 0x00000775

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 MinimumPerformance : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               RO (default = 0x0), Used by OS to read the
                               latest value of PECI minimum performance input
                            */
    UINT32 MaximumPerformance : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /*
                               RO (default = 0x0), Used by OS to read the
                               latest value of PECI maximum performance input
                            */
    UINT32 Rsvd16 : 8;

                            /* Bits[23:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 EnergyPerformancePreference : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /*
                               RO (default = 0x0), Used by OS to read the
                               latest value of PECI Energy Performance
                               Preference input
                            */
    UINT32 Rsvd32 : 28;

                            /* Bits[59:32], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 EppPeciOverride : 1;

                            /* Bits[60:60], Access Type=RO, default=0x00000000*/

                            /*
                               RO (default = 0x0). Indicates whether PECI if
                               currently overriding the Energy Performance
                               Preference input. If set(1), PECI is overriding
                               the Energy Performance Preference input. If
                               clear(0), OS has control over Energy Performance
                               Preference input
                            */
    UINT32 Rsvd61 : 1;

                            /* Bits[61:61], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MaxPeciOverride : 1;

                            /* Bits[62:62], Access Type=RO, default=0x00000000*/

                            /*
                               RO (default = 0x0).Indicates whether PECI is
                               currently overriding the Maximum Performance
                               input. If set(1), PECI is overriding the Maximum
                               Performance input. If clear(0), OS has control
                               over Maximum Performance input
                            */
    UINT32 MinPeciOverride : 1;

                            /* Bits[63:63], Access Type=RO, default=0x00000000*/

                            /*
                               RO (default = 0x0).Indicates whether PECI is
                               currently overriding the Minimum Performance
                               input. If set(1), PECI is overriding the Minimum
                               Performance input. If clear(0), OS has control
                               over Minimum Performance input
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PECI_HWP_REQUEST_INFO_REGISTER;

/**
  This is a new PerfMon register is used for future user-invisible controls to be shadowed in all PerfMon units.
**/
#define MSR_PERMON_EVNTSEL_AUX 0x00000966

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 BlkCnt : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               manageable by x/ucode. Suppress PMON/ROP
                               counting in SE
                            */
    UINT32 BlkRtitTrc : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               manageable by x/ucode. used to suppress
                               ContextEn on opt-out enclave entries
                            */
    UINT32 BlkRop : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               manageable by x/ucode. used to suppress ROP
                               counters on opt-out enclave entries
                            */
    UINT32 Rsvd3 : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PERMON_EVNTSEL_AUX_REGISTER;

/**
  Read only register for encryption policies supported by TME
**/
#define MSR_TME_CAPABILITY 0x00000981

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 AesXts : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000001*/

                            /* Enumerate support for AES-XTS */
    UINT32 Rsvd1 : 31;

                            /* Bits[31:1], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 MkTmeMaxKeyidBits : 4;

                            /* Bits[35:32], Access Type=RO, default=0x00000000*/

                            /*
                               Number of bits which can be allocated for usage
                               as key identifiers for multi-key memory
                               encryption. Zero if MK-TME is not supported
                            */
    UINT32 MkTmeMaxKeys : 15;

                            /* Bits[50:36], Access Type=RO, default=0x00000000*/

                            /*
                               Indicates the maximum number of keys which are
                               available for usage and may be a non-power of 2.
                               This maximum value of this field will be
                               (2^MK_TME_MAX_KEYID_BITS)-1. Zero if MK-TME is
                               not supported
                            */
    UINT32 Rsvd51 : 13;

                            /* Bits[63:51], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_TME_CAPABILITY_REGISTER;

/**
  Rd/Wr register to generate or restore Keys for TME
**/
#define MSR_TME_ACTIVATE 0x00000982

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Lock : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Will be set upon successful WRMSR; written
                               valuei gnored Lock for MSRs: IA32_TME_ACTIVATE
                               IA32_TME_EXCLUDE_MASK IA32_TME_EXCLUDE_BASE
                            */
    UINT32 TmeEnable : 1;

                            /* Bits[1:1], Access Type=RW_L, default=0x00000000*/

                            /*
                               Set to enable TME Encryption using DRNG
                               operation
                            */
    UINT32 KeySelect : 1;

                            /* Bits[2:2], Access Type=RW_L, default=0x00000000*/

                            /*
                               0 - Create a new ke (expected cold/warm boot) 1
                               - Restore the key from storage (Expected when
                               resume from standby)
                            */
    UINT32 SaveKeyForStandby : 1;

                            /* Bits[3:3], Access Type=RW_L, default=0x00000000*/

                            /*
                               Save key into storage to be used when resume
                               from standby
                            */
    UINT32 TmePolicy : 4;

                            /* Bits[7:4], Access Type=RW_L, default=0x00000000*/

                            /* 0000 = AES-XTS Other Values are reserved */
    UINT32 SgxTemEnable : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /* Enables TME for SGX-TEM. */
    UINT32 Rsvd9 : 23;

                            /* Bits[31:9], Access Type=RO, default=0x00000000*/

                            /* Reserved Bits; Must be 0 */
    UINT32 MkTmeKeyidBits : 4;

                            /* Bits[35:32], Access Type=RW_L, default=0x00000000*/

                            /*
                               The number of key identifier bits to allocate to
                               MK-TME usage. Similar to enumeration, this is an
                               encoded value. Writing a value greater than
                               MK_TME_MAX_KEYID_BITS will result in #GP.
                               Writing a non-zero value to this field will #GP
                               if bit 1 (TME Enable) is not set to '1, as TME
                               must be enabled to use MK-TME.
                            */
    UINT32 Rsvd36 : 12;

                            /* Bits[47:36], Access Type=RO, default=0x00000000*/

                            /* Reserved Bits; Must be 0 */
    UINT32 MkTmeCryptoAlgs : 16;

                            /* Bits[63:48], Access Type=RW_L, default=0x00000000*/

                            /*
                               Bitmask for BIOS to set which encryption
                               algorithms are allowed for MK-TME, would be
                               later enforced by the key loading ISA ('1 =
                               allowed). #GP if corresponding algorithm bits
                               are zero in TME Capability MSR
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_TME_ACTIVATE_REGISTER;

/**
  Register for debug purposes to exclude a memory range from encryption by TME
**/
#define MSR_TME_EXCLUDE_MASK 0x00000983

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 11;

                            /* Bits[10:0], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 Enable : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, then IA32_TME_EXCLUDE_BASE and
                               IA32_TME_EXCLUDE_MASK are used to define an
                               exclusion region for TME.
                            */
    UINT32 Tmemask0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /*
                               This field indicates the bits that must match
                               TMEEBASE in order to qualify as a TME exclusion
                               memory range access. This cannot exceed
                               MAX_PHYS_ADDR
                            */
    UINT32 Tmemask1 : 32;

                            /* Bits[63:32], Access Type=RW, default=0x00000000*/

                            /*
                               This field indicates the bits that must match
                               TMEEBASE in order to qualify as a TME exclusion
                               memory range access. This cannot exceed
                               MAX_PHYS_ADDR
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_TME_EXCLUDE_MASK_REGISTER;

/**
  Register for debug purposes to exclude a memory range from encryption by TME
**/
#define MSR_TME_EXCLUDE_BASE 0x00000984

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 12;

                            /* Bits[11:0], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 Tmebase0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /*
                               Base physical address to be excluded for TME
                               encryption. This cannot exceed MAX_PHYS_ADDR
                            */
    UINT32 Tmebase1 : 32;

                            /* Bits[63:32], Access Type=RW, default=0x00000000*/

                            /*
                               Base physical address to be excluded for TME
                               encryption. This cannot exceed MAX_PHYS_ADDR
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_TME_EXCLUDE_BASE_REGISTER;

/**
  FSCP To hold keylocker information
**/
#define MSR_COPY_STATUS 0x00000990

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 CopyStatus : 1;

                            /* Bits[0:0], Access Type=RW, default=None*/

                            /* Bit to hold COPY_STATUS bit for the MSR */
    UINT32 HandlePtr : 1;

                            /* Bits[1:1], Access Type=RW, default=None*/

                            /* Bit to hold Handle lookup */
    UINT32 Rsvd2 : 2;

                            /* Bits[3:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 HandleValidMask : 2;

                            /* Bits[5:4], Access Type=RW, default=None*/

                            /*
                               Mask to indicate whether the handles are valid
                               or not. Two bits currently for 2 handles
                            */
    UINT32 Rsvd6 : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_COPY_STATUS_REGISTER;

/**
  Status of IWKey Backup in PCH
**/
#define MSR_IWKEYBACKUP_STATUS 0x00000991

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 BackupRestoreValid : 1;

                            /* Bits[0:0], Access Type=RO_V, default=0x00000000*/

                            /*
                               Indication of a successful backup or restore of
                               IWKEY
                            */
    UINT32 BackupKeyCorrupted : 1;

                            /* Bits[1:1], Access Type=RO_V, default=0x00000000*/

                            /*
                               Indicates if ICV/Tag Check passes or fails for
                               IWKey
                            */
    UINT32 BackupKeyStorageRwError : 1;

                            /* Bits[2:2], Access Type=RO_V, default=0x00000000*/

                            /*
                               Indicates if there was a read/write error
                               to/from SPI Flash
                            */
    UINT32 IwkeybackupConsumed : 1;

                            /* Bits[3:3], Access Type=RO_V, default=0x00000000*/

                            /*
                               Indicates if the CSE has consume the IWKeybackup
                               written by Core
                            */
    UINT32 Rsvd4 : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_IWKEYBACKUP_STATUS_REGISTER;

/**
  This register implements Debug Interface (Privacy) MSR
**/
#define MSR_PRIVACY_CONTROL 0x00000C80

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               This bit enables or disables a variety of debug
                               features as defined in the ECO. All consumers of
                               this bit are FW agents.
                            */
    UINT32 Rsvd1 : 29;

                            /* Bits[29:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Lock : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               This bit locks the enable bit. Once set the
                               enable bit should not be changed. The lock is
                               enforced in ucode.
                            */
    UINT32 DebugHasOccurred : 1;

                            /* Bits[31:31], Access Type=RO_V, default=0x00000000*/

                            /*
                               This bit reflects a sticky flop that is the OR
                               of itself and bit 0 (enable bit) of this
                               register. Hardware also allows a sideband write
                               of 1 to this bit. PCODE/VCODE will assert this
                               bit on warm reset if a sticky debug patch is
                               detected
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PRIVACY_CONTROL_REGISTER;

/**
  MLC Class Of Service Configuration Register
**/
#define MSR_L2_QOS_CFG 0x00000C82

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 CdpEn : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               CDP (code data prioritization) enable control.
                               '0 indicates no CDP, '1 indicates CDP is
                               enabled.
                            */
    UINT32 Rsvd1 : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_L2_QOS_CFG_REGISTER;

/**
  Class Of Service Configuration Register
**/
#define MSR_CLOS_CONFIG 0x00000C8F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ClosValue : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /* The Class Of Service of the Thread */
    UINT32 Rsvd5 : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_CLOS_CONFIG_REGISTER;

/**
  available ways vectors for class of service of IA core
**/
#define MSR_COS_WAYS_0 0x00000C90

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Iacosways : 24;

                            /* Bits[23:0], Access Type=RWS, default=0x00FFFFFF*/

                            /*
                               available ways vectors for class of service of
                               IA core
                            */
    UINT32 Rsvd24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_COS_WAYS_0_REGISTER;

/**
  available ways vectors for class of service of IA core
**/
#define MSR_COS_WAYS_1 0x00000C91

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Iacosways : 24;

                            /* Bits[23:0], Access Type=RWS, default=0x0000000F*/

                            /*
                               available ways vectors for class of service of
                               IA core
                            */
    UINT32 Rsvd24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_COS_WAYS_1_REGISTER;

/**
  available ways vectors for class of service of IA core
**/
#define MSR_COS_WAYS_2 0x00000C92

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Iacosways : 24;

                            /* Bits[23:0], Access Type=RWS, default=0x000003FF*/

                            /*
                               available ways vectors for class of service of
                               IA core
                            */
    UINT32 Rsvd24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_COS_WAYS_2_REGISTER;

/**
  available ways vectors for class of service of IA core
**/
#define MSR_COS_WAYS_3 0x00000C93

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Iacosways : 24;

                            /* Bits[23:0], Access Type=RWS, default=0x00000FFF*/

                            /*
                               available ways vectors for class of service of
                               IA core
                            */
    UINT32 Rsvd24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_COS_WAYS_3_REGISTER;

/**
  Spare for WLC
**/
#define MSR_L2_QOS_MASK_0_0 0x00000D10

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 WayMask : 20;

                            /* Bits[19:0], Access Type=RW, default=0x000FFFFF*/

                            /*
                               Way Allocation Mask per way in MLC. '1 in bit
                               indicates allocation to the way is allowed. '0
                               indicates allocation to the way is not allowed.
                               restrictions: Only contiguous 1s are allowed. We
                               had this formula: ucode_reset:IF
                               (FUSE_FORCE512KMLC) THEN 0x000FF ELSE (IF
                               (FUSE_FORCE768KMLC) THEN 0x00FFF ELSE (IF
                               (FUSE_FORCE1MBMLC) THEN 0x0FFFF ELSE 0xFFFFF)),
                               but we are ok to have 20 bits set if fuse says
                               512k b/c the ways don't exist anyway (no one can
                               write)... upon first WRMSR, we?ll #GP check for
                               bits set above # of ways based upon bug
                               1407940993.
                            */
    UINT32 Rsvd20 : 12;

                            /* Bits[31:20], Access Type=RW, default=None*/

                            /* RSVD restrictions: None */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_L2_QOS_MASK_0_0_REGISTER;

/**
  Spare for WLC
**/
#define MSR_L2_QOS_MASK_1_1 0x00000D11

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 WayMask : 20;

                            /* Bits[19:0], Access Type=RW, default=0x000FFFFF*/

                            /*
                               Way Allocation Mask per way in MLC. '1 in bit
                               indicates allocation to the way is allowed. '0
                               indicates allocation to the way is not allowed.
                               restrictions: Only contiguous 1s are allowed. We
                               had this formula: ucode_reset:IF
                               (FUSE_FORCE512KMLC) THEN 0x000FF ELSE (IF
                               (FUSE_FORCE768KMLC) THEN 0x00FFF ELSE (IF
                               (FUSE_FORCE1MBMLC) THEN 0x0FFFF ELSE 0xFFFFF)),
                               but we are ok to have 20 bits set if fuse says
                               512k b/c the ways don't exist anyway (no one can
                               write)... upon first WRMSR, we?ll #GP check for
                               bits set above # of ways based upon bug
                               1407940993.
                            */
    UINT32 Rsvd20 : 12;

                            /* Bits[31:20], Access Type=RW, default=None*/

                            /* RSVD restrictions: None */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_L2_QOS_MASK_1_1_REGISTER;

/**
  Spare for WLC
**/
#define MSR_L2_QOS_MASK_2_2 0x00000D12

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 WayMask : 20;

                            /* Bits[19:0], Access Type=RW, default=0x000FFFFF*/

                            /*
                               Way Allocation Mask per way in MLC. '1 in bit
                               indicates allocation to the way is allowed. '0
                               indicates allocation to the way is not allowed.
                               restrictions: Only contiguous 1s are allowed. We
                               had this formula: ucode_reset:IF
                               (FUSE_FORCE512KMLC) THEN 0x000FF ELSE (IF
                               (FUSE_FORCE768KMLC) THEN 0x00FFF ELSE (IF
                               (FUSE_FORCE1MBMLC) THEN 0x0FFFF ELSE 0xFFFFF)),
                               but we are ok to have 20 bits set if fuse says
                               512k b/c the ways don't exist anyway (no one can
                               write)... upon first WRMSR, we?ll #GP check for
                               bits set above # of ways based upon bug
                               1407940993.
                            */
    UINT32 Rsvd20 : 12;

                            /* Bits[31:20], Access Type=RW, default=None*/

                            /* RSVD restrictions: None */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_L2_QOS_MASK_2_2_REGISTER;

/**
  Spare for WLC
**/
#define MSR_L2_QOS_MASK_3_3 0x00000D13

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 WayMask : 20;

                            /* Bits[19:0], Access Type=RW, default=0x000FFFFF*/

                            /*
                               Way Allocation Mask per way in MLC. '1 in bit
                               indicates allocation to the way is allowed. '0
                               indicates allocation to the way is not allowed.
                               restrictions: Only contiguous 1s are allowed. We
                               had this formula: ucode_reset:IF
                               (FUSE_FORCE512KMLC) THEN 0x000FF ELSE (IF
                               (FUSE_FORCE768KMLC) THEN 0x00FFF ELSE (IF
                               (FUSE_FORCE1MBMLC) THEN 0x0FFFF ELSE 0xFFFFF)),
                               but we are ok to have 20 bits set if fuse says
                               512k b/c the ways don't exist anyway (no one can
                               write)... upon first WRMSR, we?ll #GP check for
                               bits set above # of ways based upon bug
                               1407940993.
                            */
    UINT32 Rsvd20 : 12;

                            /* Bits[31:20], Access Type=RW, default=None*/

                            /* RSVD restrictions: None */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_L2_QOS_MASK_3_3_REGISTER;

/**
  Spare for WLC
**/
#define MSR_L2_QOS_MASK_4_4 0x00000D14

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 WayMask : 20;

                            /* Bits[19:0], Access Type=RW, default=0x000FFFFF*/

                            /*
                               Way Allocation Mask per way in MLC. '1 in bit
                               indicates allocation to the way is allowed. '0
                               indicates allocation to the way is not allowed.
                               restrictions: Only contiguous 1s are allowed. We
                               had this formula: ucode_reset:IF
                               (FUSE_FORCE512KMLC) THEN 0x000FF ELSE (IF
                               (FUSE_FORCE768KMLC) THEN 0x00FFF ELSE (IF
                               (FUSE_FORCE1MBMLC) THEN 0x0FFFF ELSE 0xFFFFF)),
                               but we are ok to have 20 bits set if fuse says
                               512k b/c the ways don't exist anyway (no one can
                               write)... upon first WRMSR, we?ll #GP check for
                               bits set above # of ways based upon bug
                               1407940993.
                            */
    UINT32 Rsvd20 : 12;

                            /* Bits[31:20], Access Type=RW, default=None*/

                            /* RSVD restrictions: None */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_L2_QOS_MASK_4_4_REGISTER;

/**
  Spare for WLC
**/
#define MSR_L2_QOS_MASK_5_5 0x00000D15

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 WayMask : 20;

                            /* Bits[19:0], Access Type=RW, default=0x000FFFFF*/

                            /*
                               Way Allocation Mask per way in MLC. '1 in bit
                               indicates allocation to the way is allowed. '0
                               indicates allocation to the way is not allowed.
                               restrictions: Only contiguous 1s are allowed. We
                               had this formula: ucode_reset:IF
                               (FUSE_FORCE512KMLC) THEN 0x000FF ELSE (IF
                               (FUSE_FORCE768KMLC) THEN 0x00FFF ELSE (IF
                               (FUSE_FORCE1MBMLC) THEN 0x0FFFF ELSE 0xFFFFF)),
                               but we are ok to have 20 bits set if fuse says
                               512k b/c the ways don't exist anyway (no one can
                               write)... upon first WRMSR, we?ll #GP check for
                               bits set above # of ways based upon bug
                               1407940993.
                            */
    UINT32 Rsvd20 : 12;

                            /* Bits[31:20], Access Type=RW, default=None*/

                            /* RSVD restrictions: None */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_L2_QOS_MASK_5_5_REGISTER;

/**
  Spare for WLC
**/
#define MSR_L2_QOS_MASK_6_6 0x00000D16

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 WayMask : 20;

                            /* Bits[19:0], Access Type=RW, default=0x000FFFFF*/

                            /*
                               Way Allocation Mask per way in MLC. '1 in bit
                               indicates allocation to the way is allowed. '0
                               indicates allocation to the way is not allowed.
                               restrictions: Only contiguous 1s are allowed. We
                               had this formula: ucode_reset:IF
                               (FUSE_FORCE512KMLC) THEN 0x000FF ELSE (IF
                               (FUSE_FORCE768KMLC) THEN 0x00FFF ELSE (IF
                               (FUSE_FORCE1MBMLC) THEN 0x0FFFF ELSE 0xFFFFF)),
                               but we are ok to have 20 bits set if fuse says
                               512k b/c the ways don't exist anyway (no one can
                               write)... upon first WRMSR, we?ll #GP check for
                               bits set above # of ways based upon bug
                               1407940993.
                            */
    UINT32 Rsvd20 : 12;

                            /* Bits[31:20], Access Type=RW, default=None*/

                            /* RSVD restrictions: None */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_L2_QOS_MASK_6_6_REGISTER;

/**
  Spare for WLC
**/
#define MSR_L2_QOS_MASK_7_7 0x00000D17

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 WayMask : 20;

                            /* Bits[19:0], Access Type=RW, default=0x000FFFFF*/

                            /*
                               Way Allocation Mask per way in MLC. '1 in bit
                               indicates allocation to the way is allowed. '0
                               indicates allocation to the way is not allowed.
                               restrictions: Only contiguous 1s are allowed. We
                               had this formula: ucode_reset:IF
                               (FUSE_FORCE512KMLC) THEN 0x000FF ELSE (IF
                               (FUSE_FORCE768KMLC) THEN 0x00FFF ELSE (IF
                               (FUSE_FORCE1MBMLC) THEN 0x0FFFF ELSE 0xFFFFF)),
                               but we are ok to have 20 bits set if fuse says
                               512k b/c the ways don't exist anyway (no one can
                               write)... upon first WRMSR, we?ll #GP check for
                               bits set above # of ways based upon bug
                               1407940993.
                            */
    UINT32 Rsvd20 : 12;

                            /* Bits[31:20], Access Type=RW, default=None*/

                            /* RSVD restrictions: None */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_L2_QOS_MASK_7_7_REGISTER;
/**
  BNDCFGS MSR.
**/
#define MSR_BNDCFGS 0x00000D90

/**
  Initiatate copy of IWKEY from Core to PCH for Keylocker
**/
#define MSR_COPY_LOCAL_TO_PLATFORM 0x00000D91

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 CopyIwkeyToPltfrmPersStorage : 1;

                            /* Bits[0:0], Access Type=WO, default=0x00000000*/

                            /*
                               Command bit to initiate copy IWKEY from Core to
                               PCH
                            */
    UINT32 Rsvd1 : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_COPY_LOCAL_TO_PLATFORM_REGISTER;

/**
  Initiatate copy of IWKEY from PCH to Core for Keylocker
**/
#define MSR_COPY_PLATFORM_TO_LOCAL 0x00000D92

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 CopyIwkeyFromPltfrmPersStorage : 1;

                            /* Bits[0:0], Access Type=WO, default=0x00000000*/

                            /*
                               Command bit to initiate copy IWKEY from PCH to
                               Core
                            */
    UINT32 Rsvd1 : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_COPY_PLATFORM_TO_LOCAL_REGISTER;

/**
  PASID MSR programmed by OS for ENQ* instruction (AiA)
**/
#define MSR_PASID 0x00000D93

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 PasidValue : 20;

                            /* Bits[19:0], Access Type=RW, default=0x00000000*/

                            /*
                               Specifies the Process Address Space Identifier
                               (PASID) value for the currently running thread
                            */
    UINT32 Rsvd20 : 11;

                            /* Bits[30:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 PasidValid : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               If set, bits 19:0 of this MSR contains a valid
                               PASID Value
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PASID_REGISTER;

/**
  Enables options to control the  SDC operation.
**/
#define MSR_PKG_SDC_CTL 0x00000DB0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 SdcPackageEnable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Set this bit to enable SDC, Clearing this bit
                               disables SDC and wakes up cores that were force
                               to idle by SDC. Default=0x0
                            */
    UINT32 SdcWasOnceEnabled : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               uCode sets this sticky indication on the first
                               successful RD/WRMSR to any SDC MSR
                            */
    UINT32 Rsvd2 : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PKG_SDC_CTL_REGISTER;

/**
  This is a read/write MSR
**/
#define MSR_PM_CTL1 0x00000DB1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 SdcAllowed : 1;

                            /* Bits[0:0], Access Type=RW, default=None*/

                            /*
                               Set this bit to enable SDC, Clearing this bit
                               disables SDC option to forced this thread only
                               into idle state. (default=1)
                            */
    UINT32 Rsvd1 : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PM_CTL1_REGISTER;

/**
  MPX extension for determine MAWAU
**/
#define MSR_MPX_LAX 0x00001000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Mawa9 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               When set, enables MAWA9 MPX extensions in user
                               mode (when CR4.LA57 is set). If cleared (or
                               CR4.LA57 is cleared), then MPX will operate on
                               48-bit addresses
                            */
    UINT32 Rsvd1 : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MPX_LAX_REGISTER;

/**
  The physical address of the Hardware Scheduler Information table
**/
#define MSR_HW_FB_PTR 0x000017D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Valid : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, indicates a valid pointer is
                               programmed into the MSR
                            */
    UINT32 Rsvd1 : 11;

                            /* Bits[11:1], Access Type=RW, default=0x00000000*/

                            /* Writing non zero values causes #GP */
    UINT32 Addr0 : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /*
                               Physical address of the page frame of the first
                               page of this table
                            */
    UINT32 Addr1 : 32;

                            /* Bits[63:32], Access Type=RW, default=0x00000000*/

                            /*
                               Physical address of the page frame of the first
                               page of this table
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_HW_FB_PTR_REGISTER;

/**
  The operating system enables the hardware guided scheduler interface using new package scoped MSR
**/
#define MSR_HW_FB_CONFIG 0x000017D1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               when set to 1 enables HE guided scheduler
                               interface
                            */
    UINT32 Rsvd1 : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_HW_FB_CONFIG_REGISTER;
/**
  available ways vectors for class of service of LLC snoop filter
  " seq has 16 instances in addresses range 0x1C90 to 0x1C9F"
**/
#define MSR_SF_QOS_MASK_0_0 0x00001890
/**
  available ways vectors for class of service of LLC snoop filter
  " seq has 16 instances in addresses range 0x1C90 to 0x1C9F"
**/
#define MSR_SF_QOS_MASK_1_1 0x00001891
/**
  available ways vectors for class of service of LLC snoop filter
  " seq has 16 instances in addresses range 0x1C90 to 0x1C9F"
**/
#define MSR_SF_QOS_MASK_2_2 0x00001892
/**
  available ways vectors for class of service of LLC snoop filter
  " seq has 16 instances in addresses range 0x1C90 to 0x1C9F"
**/
#define MSR_SF_QOS_MASK_3_3 0x00001893
/**
  available ways vectors for class of service of LLC snoop filter
  " seq has 16 instances in addresses range 0x1C90 to 0x1C9F"
**/
#define MSR_SF_QOS_MASK_4_4 0x00001894
/**
  available ways vectors for class of service of LLC snoop filter
  " seq has 16 instances in addresses range 0x1C90 to 0x1C9F"
**/
#define MSR_SF_QOS_MASK_5_5 0x00001895
/**
  available ways vectors for class of service of LLC snoop filter
  " seq has 16 instances in addresses range 0x1C90 to 0x1C9F"
**/
#define MSR_SF_QOS_MASK_6_6 0x00001896
/**
  available ways vectors for class of service of LLC snoop filter
  " seq has 16 instances in addresses range 0x1C90 to 0x1C9F"
**/
#define MSR_SF_QOS_MASK_7_7 0x00001897
/**
  available ways vectors for class of service of LLC snoop filter
  " seq has 16 instances in addresses range 0x1C90 to 0x1C9F"
**/
#define MSR_SF_QOS_MASK_8_8 0x00001898
/**
  available ways vectors for class of service of LLC snoop filter
  " seq has 16 instances in addresses range 0x1C90 to 0x1C9F"
**/
#define MSR_SF_QOS_MASK_9_9 0x00001899
/**
  available ways vectors for class of service of LLC snoop filter
  " seq has 16 instances in addresses range 0x1C90 to 0x1C9F"
**/
#define MSR_SF_QOS_MASK_10_10 0x0000189A
/**
  available ways vectors for class of service of LLC snoop filter
  " seq has 16 instances in addresses range 0x1C90 to 0x1C9F"
**/
#define MSR_SF_QOS_MASK_11_11 0x0000189B
/**
  available ways vectors for class of service of LLC snoop filter
  " seq has 16 instances in addresses range 0x1C90 to 0x1C9F"
**/
#define MSR_SF_QOS_MASK_12_12 0x0000189C
/**
  available ways vectors for class of service of LLC snoop filter
  " seq has 16 instances in addresses range 0x1C90 to 0x1C9F"
**/
#define MSR_SF_QOS_MASK_13_13 0x0000189D
/**
  available ways vectors for class of service of LLC snoop filter
  " seq has 16 instances in addresses range 0x1C90 to 0x1C9F"
**/
#define MSR_SF_QOS_MASK_14_14 0x0000189E
/**
  available ways vectors for class of service of LLC snoop filter
  " seq has 16 instances in addresses range 0x1C90 to 0x1C9F"
**/
#define MSR_SF_QOS_MASK_15_15 0x0000189F

/**
  Global register for x86 EFER
**/
#define MSR_IA32_CR_EFER 0xC0000080

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Sce : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* System Call Enable Bit */
    UINT32 Rsvd1 : 7;

                            /* Bits[7:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Lme : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /* Long Mode Enabled */
    UINT32 Rsvd9 : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Lma : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Long Mode Active */
    UINT32 Nxe : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Enable No-Execute Mode */
    UINT32 Rsvd12 : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_IA32_CR_EFER_REGISTER;

/**
  SYSCALL/SYSRET selectors and legacy SYSCALL target EIP
**/
#define MSR_STAR 0xC0000081

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 SyscallTargetEip : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Legacy SYSCALL target EIP */
    UINT32 SyscallCsSs : 16;

                            /* Bits[47:32], Access Type=RW, default=0x00000000*/

                            /* SYSCALL CS/SS Selector */
    UINT32 SysretCsSs : 16;

                            /* Bits[63:48], Access Type=RW, default=0x00000000*/

                            /* SYSRET CS/SS Selector */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_STAR_REGISTER;
/**
  64-bit-mode SYSCALL target RIP
**/
#define MSR_LSTAR 0xC0000082
/**
  MSR for compatibility-mode SYSCALL target RIP
**/
#define MSR_CSTAR 0xC0000083
/**
  SYSCALL flag mask
**/
#define MSR_SFMASK 0xC0000084
/**
  FS base MSR
**/
#define MSR_FS_BASE 0xC0000100
/**
  GS base MSR
**/
#define MSR_GS_BASE 0xC0000101
/**
  Swap Target of BASE Address of GS.
**/
#define MSR_KERNEL_GSBASE 0xC0000102
/**
  Time-stamp counter auxilliary field
**/
#define MSR_TSC_AUX 0xC0000103
#endif /* _ARCHMSR_h */
