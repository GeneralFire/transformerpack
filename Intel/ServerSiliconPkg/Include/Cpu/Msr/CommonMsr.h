
/** @file
  CommonMsr.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 - 2020 Intel Corporation. <BR>
  
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
  
  This file contains Silicon register definitions.
  
  This is a generated file; please do not modify it directly.
  
**/

#ifndef _COMMONMSR_h
#define _COMMONMSR_h
#include <Base.h>
/**

**/
#define MSR_ACM_CPU_KEY_HASH_0 0x00000020
/**

**/
#define MSR_ACM_CPU_KEY_HASH_1 0x00000021
/**

**/
#define MSR_ACM_CPU_KEY_HASH_2 0x00000022
/**

**/
#define MSR_ACM_CPU_KEY_HASH_3 0x00000023
/**
  MSR read by Sandra and HotCPU test. Added to prevent these benchmarks from crashing. See NHM-B bug 2425276
**/
#define MSR_POWERON 0x0000002A

/**
  MSR used to control APIC TPR Update messages from the processor's Local APIC.
**/
#define MSR_PIC_MSG_CONTROL 0x0000002E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 10;

                            /* Bits[9:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 TprMsgOff : 1;

                            /* Bits[10:10], Access Type=RW, default=None*/

                            /*
                               When set to 1 (which is the default at reset),
                               no APIC messages (TPR updates) will be sent.
                               When set to 0, APIC messages (TPR updates) will
                               be sent. Writes from 1 to 0 are allowed, but
                               writes of 1 are ignored. BIOS must write 0 to
                               this bit for each thread in each package before
                               any Local APIC mode changes occur. Note: If BIOS
                               does not clear this bit, OS initialization of
                               interrupts will fail.
                            */
    UINT32 Rsvd11 : 21;

                            /* Bits[31:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PIC_MSG_CONTROL_REGISTER;
/**
  Keeps a running count of SMI events
**/
#define MSR_SMI_COUNT 0x00000034

/**
  This is a Read Only MSR that is shared in the processor package and used to
  determine the current count of enabled Cores and Threads.
**/
#define MSR_CORE_THREAD_COUNT 0x00000035

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Threadcount : 16;

                            /* Bits[15:0], Access Type=RO_V, default=None*/

                            /*
                               The Thread Count reflects the enabled threads
                               based on the factory-configured thread count and
                               the value of the RESOLVED_CORES_MASK register
                               for Server processors or the PCH Soft Reset Data
                               register for Client processors at reset time.
                            */
    UINT32 Corecount : 16;

                            /* Bits[31:16], Access Type=RO_V, default=None*/

                            /*
                               The Core Count reflects the enabled cores based
                               on the factory-configured core count and the
                               value of the RESOLVED_CORES_MASK register for
                               Server processors or the PCH Soft Reset Data
                               register for Client processors at reset time.
                            */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_CORE_THREAD_COUNT_REGISTER;

/**
  MSR for PPIN feature
**/
#define MSR_PPIN_CTL 0x0000004E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Lock : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Bit indicates if PPIN_CTL is locked 0 - Not
                               locked (SW free to write to PPIN_CTL) 1 - Locked
                               (SW can no longer write to PPIN_CTL).
                            */
    UINT32 Enable : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Indication to SW about presence of PPIN 0 - PPIN
                               not present. Read of PPIN MSR would #GP 1 - PPIN
                               present. Read of PPIN MSR by SW allowed.
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

} MSR_PPIN_CTL_REGISTER;
/**
  PPIN value
**/
#define MSR_PPIN 0x0000004F

/**
  MSR that is used to initiate quiesce and unquiesce of CSI links
**/
#define MSR_QUIESCE_CTL1 0x00000050

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Quiesce : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* 1 = quiesce initiated */
    UINT32 Unquiesce : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* 1 = unquiesce initiated */
    UINT32 CboEvictionsDrain : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               This bit will be updated in RDMSR flow Value 1 =
                               Victim pending bit in uCR is set Value 0 =
                               Victim pending bit in uCR is not set
                            */
    UINT32 Rsvd3 : 4;

                            /* Bits[6:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 QuiesceCapability : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               1 = quiesce feature enabled; 0 = quiesce feature
                               not enabled. This is a read only bit that is
                               updated in RDMSR flow.
                            */
    UINT32 RsvdPsmi8 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /* Reserved for PSMI handshake */
    UINT32 RsvdDca16 : 6;

                            /* Bits[21:16], Access Type=RW, default=0x00000000*/

                            /* Reserved for storing DCA info */
    UINT32 DisableitlbSpecfillSave : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Used to save/restore
                               IFU_CR_DEBUG_MODE[DISABLEITLB_SPECFILL]. See HSX
                               bug 233740.
                            */
    UINT32 Rsvd23 : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_QUIESCE_CTL1_REGISTER;

/**
  MSR that is used for PSMI handshake before initiating a RAS quiesce
**/
#define MSR_QUIESCE_CTL2 0x00000051

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 SignalHandshake : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* 1 = signal a psmi event as a handshake */
    UINT32 Rsvd1 : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_QUIESCE_CTL2_REGISTER;

/**
  Cloaking error Virtual MSR in core Creg PLA
**/
#define MSR_CLOAKING 0x00000052

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 CerrRdStatusInSmmOnly : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               When 0 (default), legacy rdmsr behavior is
                               maintained for MCi_STATUS registers. When set to
                               1, a rdmsr to any MCi_STATUS register will
                               return 0 while a corrected error is logged in
                               the register unless the processor is in SMM mode
                            */
    UINT32 UcnaRdStatusInSmmOnly : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               When 0 (default), legacy rdmsr and wrmsr
                               behavior is maintained for register. When set to
                               1, a rdmsr to any MCi_STATUS register will
                               return 0 while a UCNA error is logged in the
                               register unless the processor is in SMM mode.
                            */
    UINT32 DualSignalSmi : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Enable SMI when MCA is set. This feature is part
                               of Enhanced MCA Gen 1 that enables the silicon
                               to issue SMI to the local cores along with
                               MCError. The silicon manages timing of SMI/MCE
                               so that the SMI handler is invoked early in
                               Int18 process.
                            */
    UINT32 Rsvd3 : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_CLOAKING_REGISTER;

/**
  Spare Virtual MSRs in uncore Creg PLA
**/
#define MSR_THD_LOGICAL_ID 0x00000053

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LogicalId : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Core and Thread ID */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_THD_LOGICAL_ID_REGISTER;

/**
  This MSR clars smi
**/
#define MSR_SMM_CLEAR_SMI 0x00000057

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ClearSmi : 1;

                            /* Bits[0:0], Access Type=RW, default=None*/

                            /*
                               This field when set clears the SMI bit in
                               ML3_CR_PIC_EVENT_STATUS register
                            */
    UINT32 MsmiToMceEn : 1;

                            /* Bits[1:1], Access Type=RW, default=None*/

                            /*
                               Write of 1 will enable MSMI to MCE morphing,
                               Writes of 0 will disable MSMI to MCE morphing
                            */
    UINT32 Rsvd2 : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SMM_CLEAR_SMI_REGISTER;

/**
  This MSR reads/writes bit 0 in smm_cfg_options fscp
**/
#define MSR_SMM_CFG_OPTIONS 0x00000058

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 CfgOption : 1;

                            /* Bits[0:0], Access Type=RW, default=None*/

                            /*
                               This field updates bit 0 in
                               FSCP_CR_SMM_CFG_OPTIONS_ADDR
                            */
    UINT32 Rsvd1 : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SMM_CFG_OPTIONS_REGISTER;

#if 0 // Duplicate with ArchMsr.h.
/**
  This register provides support for the Firmware-First-Mode thermal interrup feature.
**/
#define MSR_THERM_EVENT_FFM 0x0000005E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 EnableFfm : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Can be set by BIOS in SMM only. When set,
                               instead of delivering a thermal interrupt to
                               core PCU FW must send an SMI.
                            */
    UINT32 FfmSmiSignaled : 1;

                            /* Bits[1:1], Access Type=RW0C, default=0x00000000*/

                            /*
                               BIOS in SMM mode is responsible for checking
                               this bit to determine if the origin of the SMI
                               is from a thermal event or not. When set, the
                               interrupt is thermal related and BIOS must read
                               the thermal status registers to determine which
                               cores to send an IPI to. PCU FW will read this
                               bit before sending a thermal related SMI. If the
                               bit is set it will not deliver another
                               interrupt. BIOS must clear this bit when it is
                               ready to receive another interrupt.
                            */
    UINT32 Rsvd2 : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_THERM_EVENT_FFM_REGISTER;
#endif
/**
  This MSR reads/writes CBO_CR_CBO_COH_CONFIG[0]. SKX bug 305692
**/
#define MSR_MCA_ON_NONNEM_CACHABLEMMIO_EN 0x00000061

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 McaNonnemCacheableMmioEn : 1;

                            /* Bits[0:0], Access Type=RW, default=None*/

                            /*
                               This bit when set enables MCA on nonnem
                               cacheable MMIO
                            */
    UINT32 Rsvd1 : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MCA_ON_NONNEM_CACHABLEMMIO_EN_REGISTER;
/**
  Not really a register. Probe mode MSR used to move data from/to PDR
**/
#define MSR_PROBE_TO_FROM_PDR 0x00000070
/**
  Not really a register. Probe mode MSR used for copying EDX, EAX, EFLAGS, ST0 and the SMMEM bit to/from the scratch pad
**/
#define MSR_PROBE_GP_REG 0x00000071
/**
  Not really a register. Probe mode MSR used for copying SS, CS, ES, LDTR, and GDTR to the scratch pad for probe mode.
**/
#define MSR_PROBE_SEG_REG_1 0x00000072
/**
  Not really a register. Probe mode MSR added in SKL i1350201 that on RD/WRMSR does the following operations:
  R16/R17 to/from CTAP_CR_PROBE_MODE_DR/FSCP_CR_REGISTER_1
  BND_STATUS to/from FSCP_CR_REGISTER_2
  BNDCFG3_BASE_FIELD to/from FSCP_CR_REGISTER_3
**/
#define MSR_PROBE_PL_REG 0x00000073
/**
  Not really a register. Probe mode MSR used for copying XMM0 and MXCSR to the scratch pad for probe mode.
**/
#define MSR_PROBE_MMX2_REG 0x00000074
/**
  Not really a register. Probe mode MSR used for copying shadow copies of ES, DS, FS, and GS to the scratch pad for probe mode.
**/
#define MSR_PROBE_SEG_REG_3 0x00000075
/**
  Not really a register. Probe mode MSR Register used for issuing Special Cycles to the LT chipset.
**/
#define MSR_PROBE_LT_SPC_CYC 0x00000076

/**
  Not really a register. Probe mode MSR used for changing CR0.PG to the value specified in bit 31 of EAX. It allows ITP to transition into and back out of Pseudo-64-bit mode (Long Mode, but with paging turned off).
**/
#define MSR_PROBE_CHANGE_PG 0x00000077

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 31;

                            /* Bits[30:0], Access Type=RO_V, default=None*/

                            /* Junk */
    UINT32 Cr0Pg : 1;

                            /* Bits[31:31], Access Type=RO_V, default=None*/

                            /* Copied to CR0.PG */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PROBE_CHANGE_PG_REGISTER;

/**
  Allows Probe Mode ITP software to access relevant information located in the SECS and EPCM that is not accessible using EDBGRD instruction
**/
#define MSR_SE_RDINFO_CMD 0x0000007D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Command : 12;

                            /* Bits[11:0], Access Type=RW, default=None*/

                            /* Command Code */
    UINT32 EpcPage0 : 20;

                            /* Bits[31:12], Access Type=RW, default=None*/

                            /*
                               Bits [63:12] of the platform physical address of
                               the EPC page.
                            */
    UINT32 EpcPage1 : 32;

                            /* Bits[63:32], Access Type=RW, default=None*/

                            /*
                               Bits [63:12] of the platform physical address of
                               the EPC page.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SE_RDINFO_CMD_REGISTER;
/**
  TBD
**/
#define MSR_EINIT_FAIL_CTR 0x0000007E

/**
  New MSR for NPK address used by AET messages
**/
#define MSR_NPK_STH_ACPIBAR_BASE 0x00000080

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Lock : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Lock bit - if set then this MSR cannot be re-
                               written anymore. Lock bit has to be set in order
                               for the AET packets to be directed to NPK MMIO.
                            */
    UINT32 Rsvd1 : 17;

                            /* Bits[17:1], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 Address0 : 14;

                            /* Bits[31:18], Access Type=RW, default=0x00000000*/

                            /*
                               ACPIBAR_BASE_ADDRESS - AET target address in NPK
                               MMIO space.
                            */
    UINT32 Address1 : 32;

                            /* Bits[63:32], Access Type=RW, default=0x00000000*/

                            /*
                               ACPIBAR_BASE_ADDRESS - AET target address in NPK
                               MMIO space.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_NPK_STH_ACPIBAR_BASE_REGISTER;
/**
  New Xucode R/W MSRs for LEP (Launch Enclave Provider) - CNL feature 1206939047
**/
#define MSR_SGXLEPUBKEYHASH0 0x0000008C
/**
  New Xucode R/W MSRs for LEP (Launch Enclave Provider) - CNL feature 1206939047
**/
#define MSR_SGXLEPUBKEYHASH1 0x0000008D
/**
  New Xucode R/W MSRs for LEP (Launch Enclave Provider) - CNL feature 1206939047
**/
#define MSR_SGXLEPUBKEYHASH2 0x0000008E
/**
  New Xucode R/W MSRs for LEP (Launch Enclave Provider) - CNL feature 1206939047
**/
#define MSR_SGXLEPUBKEYHASH3 0x0000008F
/**
  emulated by xucode --> cause pppe exit, described in SNC HSD 1304290485 - HCTIO/SoftCE: SoftCE ISA support
  and in
  SNC HSD 1304428988 - HCTIO/SoftCE: uArch support for TIO_DEBUG_STATUS MSRs
**/
#define MSR_TIO_DEBUG_STATUS 0x00000090
/**
  emulated by xucode --> cause pppe exit, described in SNC HSD 1304290485 - HCTIO/SoftCE: SoftCE ISA support
**/
#define MSR_TIO_UNWRAP 0x00000091

/**
  Used to enable or disable cache QoS
**/
#define MSR_ENABLE_DISABLE_CACHE_QOS 0x00000095

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LlcQosEn : 1;

                            /* Bits[0:0], Access Type=RW, default=None*/

                            /* Used to enable or disable cache QoS */
    UINT32 Rsvd1 : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_ENABLE_DISABLE_CACHE_QOS_REGISTER;

/**
  SMM Monitor Control MSR
**/
#define MSR_VMX_MSEG_BASE 0x0000009B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Valid : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* MSEG MSR Valid bit */
    UINT32 Reserved1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 Vmxoffctrl : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Controls SMI unmasking during VMXOFF. */
    UINT32 Reserved3 : 9;

                            /* Bits[11:3], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 MsegBase : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* Address of Page Frame of MSEG base */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_VMX_MSEG_BASE_REGISTER;

/**
  SMM Protected Mode Base MSR
**/
#define MSR_SMM_PROT_MODE_BASE 0x0000009D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Protected Mode SMM Enable */
    UINT32 Rsvd1 : 11;

                            /* Bits[11:1], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 PhysicalAddr : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* The physical address of the SMMSEG Table */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SMM_PROT_MODE_BASE_REGISTER;
/**
  This is a read only of SMM Base register
**/
#define MSR_SMBASE_ADDR 0x0000009E
/**
  SoftSKU data MSR
**/
#define MSR_SSKU_DATA 0x000000D0

/**
  SoftSKU cmd MSR
**/
#define MSR_SSKU_CMD 0x000000D1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Cmd : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /* different cmds for ssku */
    UINT32 Rsvd16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SSKU_CMD_REGISTER;

/**
  C-State Configuration register
**/
#define MSR_CLOCK_CST_CONFIG_CONTROL 0x000000E2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 MaxPkgCState : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               Specifies the lowest C-state for the package.
                               This feature does not limit the processor core
                               into specific C-state.
                            */
    UINT32 Rsvd4 : 6;

                            /* Bits[9:4], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 IoMwaitRedirection : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               When set, will map IO_read instructions sent to
                               IO registers PMG_IO_BASE_ADDRBASE+offset to
                               MWAIT(offset)
                            */
    UINT32 Rsvd11 : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 Lock : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               When set, locks bits 15-0 of this register for
                               further writes, until the next reset occurs.
                            */
    UINT32 AccEnable : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               BDX new. Autonomous C-state control enable. When
                               set, enables conversion of HALT instructions to
                               MWAIT(C6). MWAIT(C1) is also converted into a
                               MWAIT(C6) when enabled. This bit can be set only
                               when ACC is fuse enabled (SERVER_UNCORE_FUSES).
                               This bit is implemented in the FSCP (not part of
                               the PCU register)
                            */
    UINT32 Rsvd17 : 8;

                            /* Bits[24:17], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 C3StateAutoDemotionEnable : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               When set, processor will conditionally demote
                               C6/C7 requests to C3 based on uncore auto-demote
                               information
                            */
    UINT32 C1StateAutoDemotionEnable : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               When set, processor will conditionally demote
                               C3/C6/C7 requests to C1 based on uncore auto-
                               demote information
                            */
    UINT32 Enc3undemotion : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Enable Un-Demotion from Demoted C3 */
    UINT32 Enc1undemotion : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /* Enable Un-Demotion from Demoted C1 */
    UINT32 Enpkgcautodemotion : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Enable Package Cstate autodemotion. It enables
                               use of history of past pkg cstate depth and
                               residence, as a factor in determining cstate
                               depth.
                            */
    UINT32 Enpkgcundemotion : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Enable Package Cstate undemotion. Its the enable
                               to consider cases where demotion was the
                               incorrect desicion in determining cstate depth.
                            */
    UINT32 TimedMwaitEnable : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* When set, enables Timed MWAIT feature. */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_CLOCK_CST_CONFIG_CONTROL_REGISTER;

/**
  Power Management I/O base address for I/O trapping to C-states.
**/
#define MSR_PMG_IO_CAPTURE_BASE 0x000000E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Lvl2b : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               Base address of the LVL_2 register visible to
                               software. If IO MWAIT Redirection is enabled
                               using IO MWAIT Redirection Enable
                               (PMG_CST_CONFIG_CONTROL MSR bit 10), reads to
                               this address will be consumed by the power
                               management logic and decoded to MWAIT
                               instructions. When IO port address redirection
                               is enabled, this is the IO port address reported
                               to the OS/software.
                            */
    UINT32 CstRange : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               The IO-port block size in which IO-redirection
                               will be executed (0-7). Should be programmed
                               based on the number of LVLx registers existing
                               in the chipset
                            */
    UINT32 Rsvd19 : 13;

                            /* Bits[31:19], Access Type=RW, default=0x00000000*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PMG_IO_CAPTURE_BASE_REGISTER;

/**
  Not really a register. Probe mode MSR used to modify CPL.
**/
#define MSR_PROBE_CPL 0x00000101

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Cpl : 2;

                            /* Bits[1:0], Access Type=RO_V, default=None*/

                            /* CPLk */
    UINT32 Rsvd2 : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PROBE_CPL_REGISTER;

/**
  Platform Firmware Control MSR.
**/
#define MSR_PLAT_FRMW_PROT_CTRL 0x00000110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 PfatLock : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Locks the PFAT configuration */
    UINT32 PfatEnable : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enables PFAT */
    UINT32 PfatDp : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               When set this bit indicates that a second socket
                               exists. Applicable to dual socket systems only.
                            */
    UINT32 Reserved3 : 29;

                            /* Bits[31:3], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 Reserved32 : 32;

                            /* Bits[63:32], Access Type=RW, default=0x00000000*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PLAT_FRMW_PROT_CTRL_REGISTER;
/**
  Platform Firmware Hash Value 063:000.
**/
#define MSR_PLAT_FRMW_PROT_HASH_0 0x00000111
/**
  Platform Firmware Hash Value 127:064.
**/
#define MSR_PLAT_FRMW_PROT_HASH_1 0x00000112
/**
  Platform Firmware Hash Value 191:128.
**/
#define MSR_PLAT_FRMW_PROT_HASH_2 0x00000113
/**
  Platform Firmware Hash Value 255:192.
**/
#define MSR_PLAT_FRMW_PROT_HASH_3 0x00000114
/**
  This registers contains the 64-bit physical address of the start of the PFAT DIRECTORY.
**/
#define MSR_PLAT_FRMW_PROT_TRIG_PARAM 0x00000115
/**
  This is a Write-Only command MSR. A write triggers the command. A Rd will GP Fault.
**/
#define MSR_PLAT_FRMW_PROT_TRIGGER 0x00000116
/**
  MSR to enable PFAT support on embedded controller discrete flash.
**/
#define MSR_PFAT_EC_PASSWORD 0x00000117

/**
  This RO MSR will be used to read the Ubox CPUBUSNO/CPUBUSNO1/CPUBUSNO_VALID CSRs
**/
#define MSR_CPU_BUSNUMBER 0x00000128

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Cpubusno0 : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* Bus Number 0 */
    UINT32 Cpubusno1 : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /* Bus Number 1 */
    UINT32 Cpubusno2 : 8;

                            /* Bits[23:16], Access Type=RO, default=0x00000000*/

                            /* Bus Number 2 */
    UINT32 Cpubusno3 : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /* Bus Number 3 */
    UINT32 Cpubusno4 : 8;

                            /* Bits[39:32], Access Type=RO, default=0x00000000*/

                            /* Bus Number 4 */
    UINT32 Cpubusno5 : 8;

                            /* Bits[47:40], Access Type=RO, default=0x00000000*/

                            /* Bus Number 5 */
    UINT32 Rsvd48 : 15;

                            /* Bits[62:48], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Valid : 1;

                            /* Bits[63:63], Access Type=RO, default=0x00000000*/

                            /*
                               Set when the CSR is written with valid bus
                               numbers
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_CPU_BUSNUMBER_REGISTER;
/**
  Anchor Cove MSR for reading boot policy information from the PCH.
  8 new RDMSR range (addr 0x130-0x137), which read 64b each.
  Base addr is 0xFED58000 and reads are consecutive.
**/
#define MSR_ANC_BOOT_POLICY_0 0x00000130
/**
  Anchor Cove MSR for reading boot policy information from the PCH.
  8 new RDMSR range (addr 0x130-0x137), which read 64b each.
  Base addr is 0xFED58000 and reads are consecutive.
**/
#define MSR_ANC_BOOT_POLICY_1 0x00000131
/**
  Anchor Cove MSR for reading boot policy information from the PCH.
  8 new RDMSR range (addr 0x130-0x137), which read 64b each.
  Base addr is 0xFED58000 and reads are consecutive.
**/
#define MSR_ANC_BOOT_POLICY_2 0x00000132
/**
  Anchor Cove MSR for reading boot policy information from the PCH.
  8 new RDMSR range (addr 0x130-0x137), which read 64b each.
  Base addr is 0xFED58000 and reads are consecutive.
**/
#define MSR_ANC_BOOT_POLICY_3 0x00000133
/**
  Anchor Cove MSR for reading boot policy information from the PCH.
  8 new RDMSR range (addr 0x130-0x137), which read 64b each.
  Base addr is 0xFED58000 and reads are consecutive.
**/
#define MSR_ANC_BOOT_POLICY_4 0x00000134
/**
  Anchor Cove MSR for reading boot policy information from the PCH.
  8 new RDMSR range (addr 0x130-0x137), which read 64b each.
  Base addr is 0xFED58000 and reads are consecutive.
**/
#define MSR_ANC_BOOT_POLICY_5 0x00000135
/**
  Anchor Cove MSR for reading boot policy information from the PCH.
  8 new RDMSR range (addr 0x130-0x137), which read 64b each.
  Base addr is 0xFED58000 and reads are consecutive.
**/
#define MSR_ANC_BOOT_POLICY_6 0x00000136
/**
  Anchor Cove MSR for reading boot policy information from the PCH.
  8 new RDMSR range (addr 0x130-0x137), which read 64b each.
  Base addr is 0xFED58000 and reads are consecutive.
**/
#define MSR_ANC_BOOT_POLICY_7 0x00000137

/**
  Anchor Cove MSR to indicate that the initial boot block is complete, and to re-enable events.
  Write-only and accessible via the ACM.
**/
#define MSR_ANC_FORCE_BOOT_POLICY_ENFORCED 0x00000138

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Data : 32;

                            /* Bits[31:0], Access Type=RW, default=None*/

                            /* Force Boot Policy Enforced */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_ANC_FORCE_BOOT_POLICY_ENFORCED_REGISTER;

/**
  Anchor Cove MSR to tell PCH that the ACM has successfully launched and to stop the deadman timer countdown.
  Write-only and accessible via the ACM.
**/
#define MSR_ANC_INITIAL_BOOT_BLOCK_COMPLETE 0x00000139

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Done : 1;

                            /* Bits[0:0], Access Type=RW, default=None*/

                            /*
                               Bit indicates that ACM has successfully
                               launched. Stop deadman timer counter
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

} MSR_ANC_INITIAL_BOOT_BLOCK_COMPLETE_REGISTER;

/**
  AES Feature configuration.  This MSR is visible only if one of the following holds:
  CPUID.(EAX=01h):ECX[25]=1, which indicates the processor was manufactured with AES-NI enabled.
**/
#define MSR_FEATURE_CONFIG 0x0000013C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Lock : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Once this bit is set, writes to this register
                               will not be allowed
                            */
    UINT32 AesDisable : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This bit disables Advanced Encryption Standard
                               feature on this processor core. To disable AES,
                               BIOS will write '11 to this MSR on every core.
                            */
    UINT32 Reserved2 : 30;

                            /* Bits[31:2], Access Type=RW, default=0x00000000*/

                            /* Reserved bits */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_FEATURE_CONFIG_REGISTER;
/**
  Spare core MSRs supported by the cregpla. Ucode (instead of the PLA) will signal #GP on access. WSM eco 653232
**/
#define MSR_CORE_SPECIFIC_SPARE_0 0x0000013D
/**
  Spare core MSRs supported by the cregpla. Ucode (instead of the PLA) will signal #GP on access. WSM eco 653232
**/
#define MSR_CORE_SPECIFIC_SPARE_1 0x0000013E
/**
  Spare core MSRs supported by the cregpla. Ucode (instead of the PLA) will signal #GP on access. WSM eco 653232
**/
#define MSR_CORE_SPECIFIC_SPARE_2 0x0000013F

/**
  Miscellaneous enables for thread specific features
  FSCP Crunch - 1 : 32 Used only 1 bit out of 32
**/
#define MSR_MISC_THD_FEATURE_ENABLES 0x00000140

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 CpuidGpOnCplGt0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Causes CPUID to #GP if CPL greater than 0 and
                               not in SMM.
                            */
    UINT32 Reserved1 : 31;

                            /* Bits[31:1], Access Type=RW, default=0x00000000*/

                            /* SCP Register Reserved Bits */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MISC_THD_FEATURE_ENABLES_REGISTER;
/**
  Spare thread MSRs supported by the cregpla. Ucode (instead of the PLA) will signal #GP on access. WSM eco 653232
**/
#define MSR_THREAD_SPECIFIC_SPARE_0 0x00000142
/**
  Spare thread MSRs supported by the cregpla. Ucode (instead of the PLA) will signal #GP on access. WSM eco 653232
**/
#define MSR_THREAD_SPECIFIC_SPARE_1 0x00000143
/**
  Spare thread MSRs supported by the cregpla. Ucode (instead of the PLA) will signal #GP on access. WSM eco 653232
**/
#define MSR_THREAD_SPECIFIC_SPARE_2 0x00000144
/**
  Spare thread MSRs supported by the cregpla. Ucode (instead of the PLA) will signal #GP on access. WSM eco 653232
**/
#define MSR_THREAD_SPECIFIC_SPARE_3 0x00000145
/**
  Spare thread MSRs supported by the cregpla. Ucode (instead of the PLA) will signal #GP on access. WSM eco 653232
**/
#define MSR_THREAD_SPECIFIC_SPARE_4 0x00000146
/**
  Spare thread MSRs supported by the cregpla. Ucode (instead of the PLA) will signal #GP on access. WSM eco 653232
**/
#define MSR_THREAD_SPECIFIC_SPARE_5 0x00000147
/**
  Spare thread MSRs supported by the cregpla. Ucode (instead of the PLA) will signal #GP on access. WSM eco 653232
**/
#define MSR_THREAD_SPECIFIC_SPARE_6 0x00000148
/**
  Spare thread MSRs supported by the cregpla. Ucode (instead of the PLA) will signal #GP on access. WSM eco 653232
**/
#define MSR_THREAD_SPECIFIC_SPARE_7 0x00000149
/**
  Spare thread MSRs supported by the cregpla. Ucode (instead of the PLA) will signal #GP on access. WSM eco 653232
**/
#define MSR_THREAD_SPECIFIC_SPARE_8 0x0000014A
/**
  Spare thread MSRs supported by the cregpla. Ucode (instead of the PLA) will signal #GP on access. WSM eco 653232
**/
#define MSR_THREAD_SPECIFIC_SPARE_9 0x0000014B
/**
  Spare thread MSRs supported by the cregpla. Ucode (instead of the PLA) will signal #GP on access. WSM eco 653232
**/
#define MSR_THREAD_SPECIFIC_SPARE_10 0x0000014C
/**
  Spare thread MSRs supported by the cregpla. Ucode (instead of the PLA) will signal #GP on access. WSM eco 653232
**/
#define MSR_THREAD_SPECIFIC_SPARE_11 0x0000014D
/**
  Spare thread MSRs supported by the cregpla. Ucode (instead of the PLA) will signal #GP on access. WSM eco 653232
**/
#define MSR_THREAD_SPECIFIC_SPARE_12 0x0000014E
/**
  Spare thread MSRs supported by the cregpla. Ucode (instead of the PLA) will signal #GP on access. WSM eco 653232
**/
#define MSR_THREAD_SPECIFIC_SPARE_13 0x0000014F

/**
  MSR to manage consolidated clock ratio-based overclocking and voltage override.
**/
#define MSR_OVERCLOCKING_MAILBOX 0x00000150

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Data : 32;

                            /* Bits[31:0], Access Type=RW, default=None*/

                            /*
                               Overclocking read/write data. Software will
                               write this control to manipulate the
                               voltage/frequency curve and maximum overclocking
                               ratio (frequency) supported for a particular
                               domain. 7 :0 Max OC Ratio 19 :8 Voltage Target,
                               U12.2.10V 20 :20 Voltage target mode.
                               1=override, 0=PCU Adaptive 31 :21 Voltage
                               offset, S11.0.10V, +/-500mV
                            */
    UINT32 Command : 8;

                            /* Bits[39:32], Access Type=RW, default=None*/

                            /*
                               Overclocking Mailbox Command: 0x10 (read), 0x11
                               (write). Commands in 0x1 to 0xf range are read-
                               only capabilities. Commands in 0x10 and up are
                               read/write.
                            */
    UINT32 Param1 : 8;

                            /* Bits[47:40], Access Type=RW, default=None*/

                            /*
                               Parameter 1 contains the Domain ID (IA=0, GT=1,
                               CLR=2, Uncore=3)
                            */
    UINT32 Param2 : 8;

                            /* Bits[55:48], Access Type=RW, default=None*/

                            /*
                               Parameter 2 is currently not implemented or
                               supported in Haswell hardware. Reserved for
                               furture use, and should only be programmed to 0.
                               If set, COMMAND will be interpreted as 'Illegal
                               OC Mailbox command' SNC: This is now supported :
                               b1304634937
                            */
    UINT32 Rsvd56 : 7;

                            /* Bits[62:56], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 RunBusy : 1;

                            /* Bits[63:63], Access Type=RW, default=None*/

                            /*
                               Writes without Run/Busy indicator set are
                               dropped on the floor
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_OVERCLOCKING_MAILBOX_REGISTER;

/**
  SNC_RANGE1_BASE register decription
**/
#define MSR_SNC_RANGE0_BASE 0x00000153

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Range : 16;

                            /* Bits[15:0], Access Type=RW_V, default=0x00000000*/

                            /* Range1 Base Addr */
    UINT32 Rsvd16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SNC_RANGE0_BASE_REGISTER;

/**
  SNC_RANGE2_BASE register decription
**/
#define MSR_SNC_RANGE1_BASE 0x00000154

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Range : 16;

                            /* Bits[15:0], Access Type=RW_V, default=0x00000000*/

                            /* Range2 Base Addr */
    UINT32 Rsvd16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SNC_RANGE1_BASE_REGISTER;

/**
  SNC_RANGE3_BASE register decription
**/
#define MSR_SNC_RANGE2_BASE 0x00000155

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Range : 16;

                            /* Bits[15:0], Access Type=RW_V, default=0x00000000*/

                            /* Range 3 Base Addr */
    UINT32 Rsvd16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SNC_RANGE2_BASE_REGISTER;

/**
  SNC_RANGE4_BASE register decription
**/
#define MSR_SNC_RANGE3_BASE 0x00000156

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Range : 16;

                            /* Bits[15:0], Access Type=RW_V, default=0x00000000*/

                            /* Range4 Base Addr */
    UINT32 Rsvd16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SNC_RANGE3_BASE_REGISTER;

/**
  SNC_RANGE5_BASE register decription
**/
#define MSR_SNC_RANGE4_BASE 0x00000157

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Range : 16;

                            /* Bits[15:0], Access Type=RW_V, default=0x00000000*/

                            /* Range 5 Base Addr */
    UINT32 Rsvd16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SNC_RANGE4_BASE_REGISTER;
/**
  Probe mode MSR, used also in cache flush and segmentation
  C6NoSave Candidate
**/
#define MSR_REGISTER_0 0x00000160
/**
  Probe mode MSR, used also in PDM flows
  C6NoSave Candidate
**/
#define MSR_REGISTER_1 0x00000161
/**
  Probe mode MSR, used also in PDM flows
  C6NoSave Candidate
**/
#define MSR_REGISTER_2 0x00000162
/**
  Probe mode MSR, used also in page a/d assist and vmx flows
  C6NoSave Candidate: actually, we'll leave this in the C6 save loop because it's also an MSR
**/
#define MSR_REGISTER_3 0x00000163
/**
  Probe mode MSR, used also in PDM and paging flows
  C6NoSave Candidate
**/
#define MSR_REGISTER_4 0x00000164
/**
  Probe mode MSR, used also in PDM and paging flows
  C6NoSave Candidate
**/
#define MSR_REGISTER_5 0x00000165
/**
  Probe mode MSR
  C6NoSave Candidate
**/
#define MSR_REGISTER_6 0x00000166
/**
  Probe mode MSR
  C6NoSave Candidate
**/
#define MSR_REGISTER_7 0x00000167
/**
  Probe mode MSR
  C6NoSave Candidate
**/
#define MSR_REGISTER_8 0x00000168
/**
  Probe mode MSR
  C6NoSave Candidate
**/
#define MSR_REGISTER_9 0x00000169
/**
  Probe mode MSR
  C6NoSave Candidate
**/
#define MSR_REGISTER_10 0x0000016A
/**
  Probe mode MSR
  C6NoSave Candidate
**/
#define MSR_REGISTER_11 0x0000016B
/**
  Probe mode MSR
  C6NoSave Candidate
**/
#define MSR_REGISTER_12 0x0000016C
/**
  Probe mode MSR
  C6NoSave Candidate
**/
#define MSR_REGISTER_13 0x0000016D
/**
  SCP used by Task Switch and SMI code to save RIP
  Probe mode MSR, used also in PDM flows
**/
#define MSR_TASK_SWITCH_OR_SMI_RIP 0x0000016E
/**
  Probe mode MSR, used also in task switch
  C6NoSave Candidate
**/
#define MSR_TASK_SWITCH_OR_PROBE_MODE_MISC 0x0000016F

/**
  SMM revision identifier . REVIEW: Register could be removed if RDMSR and SMM ucode use constants
**/
#define MSR_P6_CR_SMREVID 0x00000170

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Id : 16;

                            /* Bits[15:0], Access Type=RW, default=None*/

                            /* SMM Revision identifier */
    UINT32 IoInstrRestart : 1;

                            /* Bits[16:16], Access Type=RW, default=None*/

                            /*
                               If set, IO instruction restart is supported. NHM
                               will set this bit
                            */
    UINT32 SmbaseRelocation : 1;

                            /* Bits[17:17], Access Type=RW, default=None*/

                            /*
                               If set, SMRAM base address relocation is
                               supported. NHM will set this bit
                            */
    UINT32 Rsvd18 : 14;

                            /* Bits[31:18], Access Type=RW, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_P6_CR_SMREVID_REGISTER;
/**
  SMM Base register
**/
#define MSR_P6_CR_SMBASE 0x00000171
/**
  Used to save PPPE exit reason for probe mode redirection
  C6NoSave Candidate
**/
#define MSR_PROBE_MODE_PPPE_REDIRECTION 0x00000172

/**
  Probe mode data register result
**/
#define MSR_PROBE_MODE_DR 0x00000173

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 PrdLo : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Probe data register result for lower 32 bits */
    UINT32 PrdHi : 32;

                            /* Bits[63:32], Access Type=RW, default=0x00000000*/

                            /* Probe data register result for upper 32 bits */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PROBE_MODE_DR_REGISTER;

/**
  Register that contains the poison and viral bit valid / invalid state.
**/
#define MSR_MCG_CONTAIN 0x00000178

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Pe : 1;

                            /* Bits[0:0], Access Type=RW_V, default=None*/

                            /*
                               Poison Enable - when set to '1' then the
                               Enhanced Containment Mode feature should be
                               enabled , when '0' then the behavior should be
                               legacy behavior.
                            */
    UINT32 EnableViral : 1;

                            /* Bits[1:1], Access Type=RW_V, default=0x00000000*/

                            /*
                               Viral Enable - when set to '1' then the Viral
                               Support should be enabled , when '0' (default)
                               then the behavior should be legacy behavior.
                            */
    UINT32 Rsvd2 : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MCG_CONTAIN_REGISTER;

/**
  Enhanced MCA Capabilities register
**/
#define MSR_SMM_MCA_CAP 0x0000017D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 BankSupport : 32;

                            /* Bits[31:0], Access Type=RO_V, default=None*/

                            /*
                               One bit per bank. If bit is set, then the
                               corresponding bank supports enhanced MCA.
                            */
    UINT32 Rsvd32 : 22;

                            /* Bits[53:32], Access Type=RO_V, default=None*/

                            /* Reserved */
    UINT32 SmmProtMode : 1;

                            /* Bits[54:54], Access Type=RO_V, default=None*/

                            /* Set if SMM Protected Mode is supported */
    UINT32 ErrorSpoofing : 1;

                            /* Bits[55:55], Access Type=RO_V, default=None*/

                            /*
                               Set if Error Spoofing and Enhanced MCA are
                               supported
                            */
    UINT32 TargetedSmi : 1;

                            /* Bits[56:56], Access Type=RO_V, default=None*/

                            /*
                               Set to 1 if the thread supports targeted SMI and
                               the SMM_ENABLE register (MSR 4E1h) is supported.
                            */
    UINT32 SmmCpuSvrstr : 1;

                            /* Bits[57:57], Access Type=RO_V, default=None*/

                            /*
                               Set to 1 if the SAVE/RESTORE feature is
                               supported and the SMM_FEATURE_CONTROL register
                               (MSR 4E0h) is supported.
                            */
    UINT32 SmmCodeAccessChk : 1;

                            /* Bits[58:58], Access Type=RO_V, default=None*/

                            /*
                               Set to 1 if the SMM code access check feature is
                               supported and the SMM_FEATURE_CONTROL register
                               (MSR 4E0h) is supported.
                            */
    UINT32 LongFlowIndication : 1;

                            /* Bits[59:59], Access Type=RO_V, default=None*/

                            /* Set to indicate SMM long flow indicator support */
    UINT32 SmmRsvd : 4;

                            /* Bits[63:60], Access Type=RO_V, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SMM_MCA_CAP_REGISTER;

/**
  This 'flexible boot' register is written by BIOS in order to modify the maximum non-turbo ratio on the next reset.
**/
#define MSR_FLEX_RATIO 0x00000194

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 OcExtraVoltage : 8;

                            /* Bits[7:0], Access Type=RWS, default=0x00000000*/

                            /*
                               Extra voltage to be used for Over Clocking. The
                               voltage is defined in units of 1/256 Volts.
                            */
    UINT32 FlexRatio : 8;

                            /* Bits[15:8], Access Type=RWS, default=0x00000000*/

                            /* Desired Flex ratio. */
    UINT32 Enable : 1;

                            /* Bits[16:16], Access Type=RWS, default=0x00000000*/

                            /* Flex Ratio Enabled */
    UINT32 OcBins : 3;

                            /* Bits[19:17], Access Type=RW_L, default=0x00000000*/

                            /*
                               If overclocking is enabled, this field contains
                               how many bits of over-clocking are supported.
                               The encoding is: 0h No overclocking 1h...6h Max
                               1..6 bins of overclocking is supported 7h
                               Unlimited If overclocking is not enabled then
                               this field is meaningless and should be 0.
                            */
    UINT32 OcLock : 1;

                            /* Bits[20:20], Access Type=RW1S, default=0x00000000*/

                            /*
                               If set, all writes to overclocking limits are
                               disabled. This field may only be set by
                               software, and it is cleared by hardware only on
                               reset.
                            */
    UINT32 Rsvd21 : 11;

                            /* Bits[31:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_FLEX_RATIO_REGISTER;
/**
  MSR always return zero - funtionality not exist anymore
**/
#define MSR_MSR_THERM2_CTL 0x0000019D

/**
  Temperature margin in PECI temperature counts from the thermal profile specification.  Platform fan control SW is expected to read therm_margin value to control fan or blower speed.
**/
#define MSR_PACKAGE_THERM_MARGIN 0x000001A1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ThermMargin : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00007F00*/

                            /*
                               Temperature margin in PECI temperature counts
                               from the thermal profile specification.
                               THERM_MARGIN is in 2's complement format (8.8
                               format where MSB equals 1 Sign bit + 7 bits of
                               integer temperature value and the LSB equals 8
                               precison bits of temperature value). A value of
                               zero indicates the hottest CPU die temperature
                               is on the thermal profile line. A negative value
                               indicates gap to the thermal profile that
                               platform SW should increase cooling capacity. A
                               sustained negative value should be avoided as it
                               may impact part reliability.
                            */
    UINT32 Rsvd16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PACKAGE_THERM_MARGIN_REGISTER;

/**
  MCP Thermal reporting.
  This CSR is a mirror of MSR (1a3h) MCP_THERMAL_REPORT_1
**/
#define MSR_MCP_THERMAL_REPORT_1 0x000001A3

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 MarginToThrottle : 16;

                            /* Bits[15:0], Access Type=RO_V, default=0x00000000*/

                            /*
                               Margin to Throttle represents the nomalized
                               temperature margin until throttling engages on
                               any component. It reports the minimum of the
                               temperature margins on different dies. Each
                               margin represents the current sensor temperature
                               relative to the temperature when the hotspot on
                               the die is at its Tj,max. It is reported as a
                               negative number in signed, filtered 10.6 fixed-
                               point format. Note: This is also reported via
                               PECS as the PCS index 2 function.
                            */
    UINT32 MarginToTcontrol : 16;

                            /* Bits[31:16], Access Type=RO_V, default=0x00000000*/

                            /*
                               Margin to TCONTROL reports the minimum thermal
                               margin relative to the thermal spec based on KNL
                               DTS with TCONTROL, This field is relevant only
                               for components with TCONTROL specified for
                               system fan speed control - to ensure long term
                               reliability. Margin to Tcontrol is reported in
                               signed, filtered 10.6 fixed-point format. If a
                               negative value is reported, representing a gap
                               to TCONTROL spec, the system must increase fan
                               speed. If a positive value is reported,
                               representing a margin to TCONTROL spec, the
                               system can decrease fan speed. Note: This is
                               also reported via PECS as the PCS index 10
                               function.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MCP_THERMAL_REPORT_1_REGISTER;

/**
  MCP Thermal reporting.
  This CSR is a mirror of MSR (1A4h) MCP_THERMAL_REPORT_2.
**/
#define MSR_MCP_THERMAL_REPORT_2 0x000001A5

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 PackageAbsoluteMaxTemperature : 16;

                            /* Bits[15:0], Access Type=RO_V, default=0x00000000*/

                            /*
                               Package Absolute Max Temperature reports the
                               hottest absolute temperature of any component in
                               the multi-chip package. This field is reported
                               in signed, absolute, filtered 10.6 fixed-point
                               format. Note: This is also reported via PECS as
                               PCS index 9 command with parameter 0x80.
                            */
    UINT32 Rsvd16 : 16;

                            /* Bits[31:16], Access Type=RO_V, default=0x00000000*/

                            /* Reserved for future use. */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MCP_THERMAL_REPORT_2_REGISTER;

/**
  MSR for thread specific defeature
**/
#define MSR_THREAD_SW_DEFEATURE 0x000001A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 XinitDisable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* When this bit set: READ_XINIT_AS_NON_INIT */
    UINT32 XmodifiedDisable : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* When this bit set: READ_XMODIFIED_AS_MODIFIED */
    UINT32 Rsvd2 : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_THREAD_SW_DEFEATURE_REGISTER;

/**
  Legacy Miscellaneous Power Management Features.
**/
#define MSR_MISC_PWR_MGMT 0x000001AA

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 SinglePctlEn : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Single_PCTL: When set (1), will cause PST
                               commands (writes to IA32_PERF_CTL) from any
                               cores to set the P-state target (no
                               coordination). This mean that the last thread
                               that writes the P-state request, is the dominant
                               one. If not set this mean that coordination
                               between all the thread requests will be
                               initiated and the resolved result will dominant.
                               Default = 0 (disabled).
                            */
    UINT32 Rsvd1 : 5;

                            /* Bits[5:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 EnableHwp : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 EnableHwpInterrupt : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 EnableOutOfBandAutonomous : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 EnableSdcOob : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 EnableSdc : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 SdcOobCapable : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Read-Only bit in which RDMSR reflects the
                               ENABLE_SDC_OOB fuse value for BIOS to know
                               whether it can ENABLE_SDC_OOB
                            */
    UINT32 EnableHwpEpp : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 Lock : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Set this bit will lock this MSR until the next
                               reset
                            */
    UINT32 Rsvd14 : 4;

                            /* Bits[17:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 PlatPStateCtrl : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Adding for Dell request to add
                               platform_p_stat_ctrl bit 18 to 0x1aa msr
                            */
    UINT32 Rsvd19 : 3;

                            /* Bits[21:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 LockThermInt : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Lock_Therm_Int - Ties thermal interrupts from
                               all cores. If set, then a thermal interrupt on
                               one core is routed to all cores. A value = 0
                               indicates not active, and a value = 1 indicates
                               active.
                            */
    UINT32 Rsvd23 : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MISC_PWR_MGMT_REGISTER;

/**
  This register defines the active core ranges for each frequency point
  NUMCORE[0:7] must be populated in ascending order.
  NUMCORE[i+1] must be greater than NUMCORE[i]
  Entries with NUMCORE[i] == 0 will be ignored
  The last valid entry must have NUMCORE >= the number of cores in the SKU
  If any of the rules above are broken, we will silently reject the configuration.
**/
#define MSR_TURBO_RATIO_LIMIT_CORES 0x000001AE

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Numcore0 : 8;

                            /* Bits[7:0], Access Type=RW_V, default=0x00000000*/

                            /*
                               defines the active core ranges for each
                               frequency point.
                            */
    UINT32 Numcore1 : 8;

                            /* Bits[15:8], Access Type=RW_V, default=0x00000000*/

                            /*
                               defines the active core ranges for each
                               frequency point.
                            */
    UINT32 Numcore2 : 8;

                            /* Bits[23:16], Access Type=RW_V, default=0x00000000*/

                            /*
                               defines the active core ranges for each
                               frequency point.
                            */
    UINT32 Numcore3 : 8;

                            /* Bits[31:24], Access Type=RW_V, default=0x00000000*/

                            /*
                               defines the active core ranges for each
                               frequency point.
                            */
    UINT32 Numcore4 : 8;

                            /* Bits[39:32], Access Type=RW_V, default=0x00000000*/

                            /*
                               defines the active core ranges for each
                               frequency point.
                            */
    UINT32 Numcore5 : 8;

                            /* Bits[47:40], Access Type=RW_V, default=0x00000000*/

                            /*
                               defines the active core ranges for each
                               frequency point.
                            */
    UINT32 Numcore6 : 8;

                            /* Bits[55:48], Access Type=RW_V, default=0x00000000*/

                            /*
                               defines the active core ranges for each
                               frequency point.
                            */
    UINT32 Numcore7 : 8;

                            /* Bits[63:56], Access Type=RW_V, default=0x00000000*/

                            /*
                               defines the active core ranges for each
                               frequency point.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_TURBO_RATIO_LIMIT_CORES_REGISTER;

/**
  This MSR return the status of debug resources (Used to be in MISC_ENABLES MSR).
**/
#define MSR_DEBUG_RESOURCE_STATUS 0x000001C6

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 DebugDrxAvailable : 1;

                            /* Bits[0:0], Access Type=RO_V, default=None*/

                            /*
                               Read-only bit. Inverted
                               P6_CR_ICECTLPMR[PRESERVE_DEBUG_RESOURCES]
                            */
    UINT32 DebugAvailable : 1;

                            /* Bits[1:1], Access Type=RO_V, default=None*/

                            /*
                               Read-only bit. Inverted
                               P6_CR_ICECTLPMR[PRESERVE_LBR_RESOURCES]
                            */
    UINT32 Rsvd2 : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_DEBUG_RESOURCE_STATUS_REGISTER;

/**
  Probe mode control and status register. 16 bit. (Mrm roctlregc/roctlregd/icectlpmrSM306L)
**/
#define MSR_ICECTLPMR 0x000001DF

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Ir : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* ICECTLIR: Interrupt Redirection */
    UINT32 Urdmsr : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* ICECTLURDMSR: User Mode Access to Performance */
    UINT32 Pmen : 1;

                            /* Bits[2:2], Access Type=RO_V, default=0x00000000*/

                            /* ICECTLPMEN: ProbeMode Enabled */
    UINT32 PirStt : 1;

                            /* Bits[3:3], Access Type=RO_V, default=0x00000000*/

                            /* ICECTLPIR: Submit Probe Instruction Reg status */
    UINT32 PmIs : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* ICECTLPM_IS: Probe Mode Instruction Status */
    UINT32 PmMce : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* ICECTLPM_MCE: Machine Check Enabled */
    UINT32 ShutdownBit : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Reserved. Not used in PM */
    UINT32 HaltBit : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Reserved. Not used in PM */
    UINT32 PmInitBit : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /* ICECTL_PM_INIT_BIT_POS: Enter Probe Mode on INIT */
    UINT32 PmMcEnBit : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* ICECTL_PM_MC_EN_BIT_POS: Enter PM on enabled MC */
    UINT32 PmSmmEnterBit : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* ICECTL_PM_SMM_ENTER_BIT_POS */
    UINT32 PmSmmExitBit : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* ICECTL_PM_SMM_EXIT_BIT_POS */
    UINT32 Res1Low : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /* ICECTLRES1_LOW: Reserved */
    UINT32 Res1High : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /* ICECTLRES1_HIGH: Reserved */
    UINT32 PreserveDebugResources : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* ICECTLPMR_PRESERVE_DEBUG_RESOURCES: */
    UINT32 PreserveLbrResources : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               ICECTLPMR_PRESERVE_LBR_RESOURCES: PM S/W may
                               program the LBRs to trace the DUT. Before PM
                               exit, the PM S/W should reserve LBRs for itself
                               by setting ICECTLPMR.Preserve_LBR_Resources.
                               This will ensure that any other software running
                               on the CPU cannot use the LBRs. Setting this bit
                               will also effectively preserve RTIT resources.
                            */
    UINT32 CsiOffTapDrainOn : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Enable the tap and turn off the csi link packet:
                               Ynh
                            */
    UINT32 RemainProbemode : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /* Remain in Probe mode */
    UINT32 PmHleBit : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               ICECTL_PM_HLE_BIT_POS: Enter Probe Mode on HLE
                               Aborts
                            */
    UINT32 PmHlepBit : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               ICECTL_PM_HLEP_BIT_POS: Enter Probe Mode on HLE+
                               Aborts
                            */
    UINT32 ClrPir : 1;

                            /* Bits[20:20], Access Type=WO, default=0x00000000*/

                            /* ICECTLPIR: Clear PIR bit state */
    UINT32 EnclaveInterruption : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               ICECTLPIR: ENCLAVE INTERRUPTION bit to be set in
                               ICECTLPMR once probe mode got signaled inside
                               enclave
                            */
    UINT32 PreserveRtitResources : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               ICECTLPMR_PRESERVE_RTIT_RESOURCES: PM S/W may
                               program RTIT to trace the DUT. Before PM exit,
                               the PM S/W should reserve RTIT for itself by
                               setting ICECTLPMR.Preserve_RTIT_Resources. This
                               will ensure that any other software running on
                               the CPU cannot use RTIT. Setting this bit will
                               also effectively preserve LBR/DEBUG resources.
                            */
    UINT32 Rsvd23 : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_ICECTLPMR_REGISTER;

/**
  This RO MSR will be used to report the fuse_cpu_type bits from PCU Package Config CSR
**/
#define MSR_CPU_TYPE 0x000001E1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 FuseCpuType : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /* CPU Type Indication */
    UINT32 Rsvd4 : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_CPU_TYPE_REGISTER;

/**
  This MSR holds the capability of error spoofing. Bit 1 for unlock MCA/ CMCI generation capability (allows event injection), bit 0 to unlock writing to MC banks
**/
#define MSR_DEBUG_ERR_INJ_CTL 0x000001E3

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 McbwE : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Bit indicates if writing to MC banks is allowed
                               0 - Writing to MC banks Not allowed 1 - Writing
                               to MC banks allowed
                            */
    UINT32 McaCmciSe : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Bit indicates if error spoofing is allowed 0 -
                               Error spoofing not allowed 1 - Error spoofing
                               allowed(MCA/CMCI)
                            */
    UINT32 Rsvd2 : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_DEBUG_ERR_INJ_CTL_REGISTER;

/**
  This MSR will be used to trigger MCA(bit0)/CMCI(bit1)
**/
#define MSR_DEBUG_ERR_INJ_CTL2 0x000001E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 McaG : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Bit indicates if MC banks is triggered 0 - Not
                               triggered 1 - Triggered
                            */
    UINT32 CmciG : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Bit indicates if CMCI is triggered 0 - Not
                               triggered 1 - Triggered
                            */
    UINT32 Rsvd2 : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_DEBUG_ERR_INJ_CTL2_REGISTER;

/**
  VLW capability MSR that reports information about the VLW messages that are supported
**/
#define MSR_VLW_CAPABILITY 0x000001F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Ignne : 1;

                            /* Bits[0:0], Access Type=RO_V, default=None*/

                            /*
                               '0 since IGNNE VLW message is not supported on
                               NHM
                            */
    UINT32 A20m : 1;

                            /* Bits[1:1], Access Type=RO_V, default=None*/

                            /* '1 means A20M message is supported */
    UINT32 Intr : 1;

                            /* Bits[2:2], Access Type=RO_V, default=None*/

                            /* '1 means INTR message is supported */
    UINT32 Rsvd3 : 1;

                            /* Bits[3:3], Access Type=RO_V, default=None*/

                            /* Reserved */
    UINT32 Smi : 1;

                            /* Bits[4:4], Access Type=RO_V, default=None*/

                            /* '1 means SMI message is supported */
    UINT32 Init : 1;

                            /* Bits[5:5], Access Type=RO_V, default=None*/

                            /* '1 means INIT message is supported */
    UINT32 Nmi : 1;

                            /* Bits[6:6], Access Type=RO_V, default=None*/

                            /* '1 means NMI message is supported */
    UINT32 Rsvd7 : 25;

                            /* Bits[31:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_VLW_CAPABILITY_REGISTER;

/**
  SMM Range Register - Physical Base Control Register
**/
#define MSR_SMRR_BASE 0x000001F2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Memtype : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* SMRR BASE MemType */
    UINT32 Rsvd3 : 9;

                            /* Bits[11:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Base : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* SMRR BASE Physical Base Address */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SMRR_BASE_REGISTER;

/**
  SMM Range Register Base - Physical Mask Control Register
**/
#define MSR_SMRR_MASK 0x000001F3

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 9;

                            /* Bits[8:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Assist : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               If set, SMRR is in assist-mode - else in page
                               abort-mode (defeature only)
                            */
    UINT32 L : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Lock bit for the SMRR */
    UINT32 Vld : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* SMM Mask Valid Bit */
    UINT32 Mask : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* SMM MASK MTRR Mask */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SMRR_MASK_REGISTER;

/**
  SMM Range Register #2 - Physical Base Control Register
**/
#define MSR_SMRR2_BASE 0x000001F6

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Memtype : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* SMRR BASE MemType */
    UINT32 Rsvd3 : 9;

                            /* Bits[11:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Base : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* SMRR BASE Physical Base Address */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SMRR2_BASE_REGISTER;

/**
  SMM Range Register Base #2 - Physical Mask Control Register
**/
#define MSR_SMRR2_MASK 0x000001F7

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 9;

                            /* Bits[8:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Assist : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               If set, SMRR is in assist-mode - else in page
                               abort-mode (defeature only)
                            */
    UINT32 L : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Lock bit for the SMRR2 */
    UINT32 Vld : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* SMM Mask Valid Bit */
    UINT32 Mask : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /* SMM MASK MTRR Mask */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SMRR2_MASK_REGISTER;
/**
  Special Chipset Usage MSR
**/
#define MSR_SPCL_CHIPSET_USAGE 0x000001FE

/**
  Scratch Pad register P6_CR_ICECTLPMR_2
  Bit descriptions are derived from review of Merom ucode - May be incomplete (RajeshP)
**/
#define MSR_ICECTLPMR_2 0x0000025B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 RsvdB0 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* SCP Register Value */
    UINT32 C6Exit : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /* Redirect to probe mode at the end of C6 exit */
    UINT32 Getsec : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* Redirect to probe mode at the start of GETSEC */
    UINT32 PreCram : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Redirect to probe mode just before loading LT
                               CRAM
                            */
    UINT32 AcmodeEnter : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Redirect to probe mode just before launching an
                               AC-module
                            */
    UINT32 AcmodeExit : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Redirect to probe mode after exiting an AC-
                               module (end of EXITAC)
                            */
    UINT32 SexitEnd : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /* Redirect to probe mode at the end of SEXIT */
    UINT32 Enclu : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* Redirect to probe mode in ENCLU instruction */
    UINT32 Rsvd15 : 7;

                            /* Bits[21:15], Access Type=RW, default=0x00000000*/

                            /* SCP Register Value */
    UINT32 Int1 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /* Redirect to probe mode in INT1 instruction */
    UINT32 StepInto : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               For SNB i366110: ITP Step Into Interrupts and
                               Exceptions
                            */
    UINT32 Shutdown : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /* SCP Register Value */
    UINT32 Rsvd25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /* SCP Register Value */
    UINT32 Vmexit : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /* SCP Register Value */
    UINT32 Rsvd27 : 2;

                            /* Bits[28:27], Access Type=RW, default=0x00000000*/

                            /* SCP Register Value */
    UINT32 Vmentry : 3;

                            /* Bits[31:29], Access Type=RW, default=0x00000000*/

                            /* SCP Register Value */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_ICECTLPMR_2_REGISTER;

/**
  No-Evict Mode MSR.
**/
#define MSR_NEM 0x000002E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Setup : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS sets this when it is ready to fill the NEM
                               region, and then executes the requests to fill
                               the NEM
                            */
    UINT32 Run : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS sets this bit when it's done filling the
                               NEM region. At this point, the cache is locked.
                            */
    UINT32 Rsvd2 : 30;

                            /* Bits[31:2], Access Type=RW, default=0x00000000*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_NEM_REGISTER;
/**
  Used by Intel TXT enabled BIOS to unlock memory after checking that the TPM is valid.
**/
#define MSR_LT_UNLOCK_MEMORY 0x000002E6

/**
  This MSR is used to lock processor core and uncore BAR register space.
**/
#define MSR_LTCTRLSTS 0x000002E7

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Configlock : 1;

                            /* Bits[0:0], Access Type=RW1S, default=0x00000000*/

                            /* This bit is used for indicating 'LT Config Lock' */
    UINT32 Memlockcpu : 1;

                            /* Bits[1:1], Access Type=RO_V, default=0x00000001*/

                            /*
                               This bit is implemented in the IMPH version of
                               the register only (as RO-VFW), the NCU holds a
                               shadow of the bit. This bit is used for
                               indicating 'LT Lock of CPU access to Memory'' to
                               read operations. The bit is cleared by a write
                               of 1 to the MemLockCPUClr bit. 0 - Memory access
                               is enabled 1 - Memory is Locked
                            */
    UINT32 Memlockdev : 1;

                            /* Bits[2:2], Access Type=RO_V, default=0x00000001*/

                            /*
                               This bit is implemented in the IMPH version of
                               the register only (as RO-VFW), the NCU holds a
                               shadow of the bit. This bit is used for
                               indicating 'LT Lock of Device access to Memory''
                               to read operations. The bit is cleared by a
                               write of 1 to the MemLockDevClr bit. 0 - Memory
                               access is enabled 1 - Memory is Locked
                            */
    UINT32 SmmlockRsvd : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /* Reserved for implemention in JKT */
    UINT32 ScheckPass : 1;

                            /* Bits[4:4], Access Type=RW1S, default=0x00000000*/

                            /*
                               This bit is set once by SCHECK once it passes.
                               it is cleared by the SCHECK_PASS_clr bit.
                            */
    UINT32 Poison : 1;

                            /* Bits[5:5], Access Type=RW1S, default=0x00000000*/

                            /*
                               ucode will set this bit whenever it sends out a
                               poison cycle. cleared by PoisonClr bit.
                            */
    UINT32 Private : 1;

                            /* Bits[6:6], Access Type=RW1S, default=0x00000000*/

                            /*
                               This bit is copied in the NCU and Cbo. This bit
                               is cleared by the PrivateClr bit. Read : 0 -
                               Private flag is cleared 1 - Private flag is set
                               Write : 0 - ignored 1 -Set the Private flag
                            */
    UINT32 Inacm : 1;

                            /* Bits[7:7], Access Type=RW1S, default=0x00000000*/

                            /*
                               This bit is copied in the NCU IMPH and Cbo. This
                               bit is cleared by the respective bit in
                               NcuLTCtrClr register. Read : 0 - In ACM flag is
                               cleared 1 - In ACM flag is set Write : 0 -
                               ignored 1 -Set the InACM flag
                            */
    UINT32 Loc3guarddis : 1;

                            /* Bits[8:8], Access Type=RW1S, default=0x00000000*/

                            /*
                               This bit is implemented in the Cbo and has a
                               shadow in NCU. This bit is cleared by writint a
                               1 to the respective bit in NcuLTCtrClr register.
                               Read : 0 - Locality 3 Guard is Enabled 1 -
                               Locality 3 Guard is Disable Write : 0 - ignored
                               1 -Set the Loc3Guard
                            */
    UINT32 Hskinprog : 1;

                            /* Bits[9:9], Access Type=RW1S, default=0x00000000*/

                            /*
                               Indicates that the core is in the process of
                               SENTER or SEXIT handshake. This is a sticky bit
                               used by uCode as part of LT flow. No HW action
                               in NCU. This bit is cleared by writint a 1 to
                               the respective bit in NcuLTCtrClr register. Read
                               : 0 - Hand Shake not in progress 1 - Hand Shake
                               in progress Write : 0 - ignored 1 -Set the
                               HskInProg
                            */
    UINT32 Faterrdursacm : 1;

                            /* Bits[10:10], Access Type=RW1S, default=0x00000000*/

                            /*
                               FATAL_ERROR_DURING_SACM Indicates that an error
                               occurred during Startup ACM processing that
                               caused an LT-shutdown. This register is
                               preserved across warm reset, so reset uCode can
                               check this bit and skip Startup ACM processing
                               if it is set to prevent an infinite loop of
                               resets This is a sticky bit used by uCode as
                               part of LT flow. No HW action in NCU. This bit
                               is cleared by writint a 1 to the respective bit
                               in NcuLTCtrClr register. Read : 0 - No Error 1 -
                               Fatal Error During SACM Write : 0 - ignored 1
                               -Set the FatErrDurSACM
                            */
    UINT32 IlpMsegValid : 1;

                            /* Bits[11:11], Access Type=RW1S, default=0x00000000*/

                            /*
                               This is a sticky bit used by uCode as part of LT
                               flow. It is used to stash the ILP's copy of
                               FSCP_CR_VMX_MSEG_BASE[VALID]. It is used by
                               UCODE for consistency check to verify that all
                               threads select the same type of SMM handling. No
                               HW action in NCU. This bit is cleared by writing
                               a 1 to the respective bit in NcuLTCtrClr
                               register. Read : 0 - Bit is at cleared state 1 -
                               Bit is set state Write : 0 - ignored 1 -Set the
                               ILP_MSEG_VALID
                            */
    UINT32 PostSenter : 1;

                            /* Bits[12:12], Access Type=RW1S, default=0x00000000*/

                            /*
                               This is a sticky bit used by uCode as part of LT
                               flow. The POST_SENTER bit will be set between
                               SENTER and SEXIT. It is used to detect
                               spontaneous reset during secure mode. The
                               POST_SENTER bit must survive warm reset. It will
                               be checked along with the existing AC-mode check
                               in reset UCODE. No HW action in NCU. This bit is
                               cleared by writing a 1 to the respective bit in
                               NcuLTCtrClr register. Read : 0 - Bit is at
                               cleared state 1 - Bit is set state Write : 0 -
                               ignored 1 -Set the POST_SENTER bit
                            */
    UINT32 LtScp3Rsrv : 1;

                            /* Bits[13:13], Access Type=RW1S, default=0x00000000*/

                            /*
                               This is a sticky bit used by uCode as part of LT
                               flow - currently as spare bit. No HW action in
                               NCU. This bit is cleared by writint a 1 to the
                               respective bit in NcuLTCtrClr register. Read : 0
                               - Bit is at cleared state 1 - Bit is set state
                               Write : 0 - ignored 1 -Set the LT_scp3_rsrv
                            */
    UINT32 Rsvd14 : 2;

                            /* Bits[15:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Configunloc : 1;

                            /* Bits[16:16], Access Type=WO, default=0x00000000*/

                            /*
                               Writing a value of 0 to this bit has no effect.
                               Writing a value of 1 to this bit will clear the
                               ConfigLock flag. NOTE: The HW an assume that
                               only SW in ACM mode will write to this bit a
                               value of 1.
                            */
    UINT32 Memlockcpuclr : 1;

                            /* Bits[17:17], Access Type=WO, default=0x00000000*/

                            /*
                               Writing 1 to this bit clears MemLockCPU bit in
                               this register. Writing 0 to this bit has no
                               effect.
                            */
    UINT32 Memlockdevclr : 1;

                            /* Bits[18:18], Access Type=WO, default=0x00000000*/

                            /*
                               Writing 1 to this bit clears MemLockDev bit in
                               this register. Writing 0 to this bit has no
                               effect.
                            */
    UINT32 SmmlockclrReserved : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /* Reserved for use by JKT with SMMLock bit. */
    UINT32 ScheckPassClr : 1;

                            /* Bits[20:20], Access Type=WO, default=0x00000000*/

                            /*
                               0 : Ignored 1 : Clear the SCHECK_PASS bit in
                               this register.
                            */
    UINT32 Poisonclr : 1;

                            /* Bits[21:21], Access Type=WO, default=0x00000000*/

                            /*
                               0 : Ignored 1 : Clear the Poison bit in this
                               register.
                            */
    UINT32 Privateclr : 1;

                            /* Bits[22:22], Access Type=WO, default=0x00000000*/

                            /*
                               0 : Ignored 1 : Clear the Private bit in this
                               register.
                            */
    UINT32 Inacmclr : 1;

                            /* Bits[23:23], Access Type=WO, default=0x00000000*/

                            /* 0 : ignored 1 : Clears the InACM flag */
    UINT32 Loc3guarddisclear : 1;

                            /* Bits[24:24], Access Type=WO, default=0x00000000*/

                            /*
                               Write : If Loc3GuardDis=0 (including the result
                               of this write) - Ignored. else 0 - ignored 1 -
                               Clear the Loc3GuardDis flag
                            */
    UINT32 Hskinprogclr : 1;

                            /* Bits[25:25], Access Type=WO, default=0x00000000*/

                            /*
                               Write : If HskInProgs=0 (including the result of
                               this write) - Ignored. else 0 - ignored 1 -
                               Clear the HskInProg flag
                            */
    UINT32 Faterrdursacmclr : 1;

                            /* Bits[26:26], Access Type=WO, default=0x00000000*/

                            /*
                               Write : If FatErrDurSACM=0 (including the result
                               of this write) - Ignored. else 0 - ignored 1 -
                               Clear the FatErrDurSACM flag
                            */
    UINT32 IlpMsegValidClr : 1;

                            /* Bits[27:27], Access Type=WO, default=0x00000000*/

                            /*
                               Write : If ILP_MSEG_VALID=0 (including the
                               result of this write) - Ignored. else 0 -
                               ignored 1 - Clear the ILP_MSEG_VALID flag
                            */
    UINT32 PostSenterClr : 1;

                            /* Bits[28:28], Access Type=WO, default=0x00000000*/

                            /*
                               Write : If POST_SENTER=0 (including the result
                               of this write) - Ignored. else 0 - ignored 1 -
                               Clear the POST_SENTER flag
                            */
    UINT32 LtScp3RsrvClr : 1;

                            /* Bits[29:29], Access Type=WO, default=0x00000000*/

                            /*
                               Write : If LT_scp3_rsrv=0 (including the result
                               of this write) - Ignored. else 0 - ignored 1 -
                               Clear the LT_scp3_rsrv flag
                            */
    UINT32 Ltpmonctrclr : 1;

                            /* Bits[30:30], Access Type=WO, default=0x00000000*/

                            /*
                               This bit is copied in the NCU and Cbo. This bit
                               is used for LT uCode control over the PMON
                               counters. The NCU copy of the register must be
                               cleared last to prevent race conditions with
                               clear operations of other module's counters. 0b
                               No action 1b Clear all PMON control registers
                               (including Fixed counter control)
                            */
    UINT32 Ltpmoncntclr : 1;

                            /* Bits[31:31], Access Type=WO, default=0x00000000*/

                            /*
                               This bit is copied in the NCU and Cbo. This bit
                               is used for LT uCode control over the PMON
                               counters. 0b No Action 1b Clear all PMON Counter
                               registers and Unit overflow status registers
                               (including Fixed Counter and Global Overflow
                               Status).
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_LTCTRLSTS_REGISTER;

/**
  IVB i1026245 - PerfMon Compound Counter enhancement
**/
#define MSR_COMPOUND_CTR_CTRL 0x00000306

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 FlopCtr0Inc : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Flopped counter 0 inc */
    UINT32 FlopCtr1Inc : 3;

                            /* Bits[5:3], Access Type=RW, default=0x00000000*/

                            /* Flopped counter 1 inc */
    UINT32 FlopCtr2Inc : 3;

                            /* Bits[8:6], Access Type=RW, default=0x00000000*/

                            /* Flopped counter 2 inc */
    UINT32 FlopCtr3Inc : 3;

                            /* Bits[11:9], Access Type=RW, default=0x00000000*/

                            /* Flopped counter 3 inc */
    UINT32 Flop0 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /* Flopped signal 0 */
    UINT32 Flop1 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /* Flopped signal 1 */
    UINT32 Flop2 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* Flopped signal 2 */
    UINT32 Flop3 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* Flopped signal 3 */
    UINT32 OrFlop0123 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /* OR of Flopped signals 0,1,2,3 */
    UINT32 OrFlop012 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /* OR of Flopped signals 0,1,2 */
    UINT32 OrFlop01 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /* OR of Flopped signals 0,1 */
    UINT32 OrFlop23 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /* OR of Flopped signals 2,3 */
    UINT32 Pmi : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               APIC interrupt enable. When set, the processor
                               generates an exception through its local APIC on
                               counter overflow for this counter's thread. This
                               bit gets AND-ed with the overflow of the counter
                               and Not the GlobalStatus overflow of the
                               counter. This means that the GlobalStatus
                               Overflow flag cannot cause a PMI.
                            */
    UINT32 MythrAllthr : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               The 8 general counters have been divided into
                               two banks T0 and T1 for each thread. When this
                               bit is set to zero the counters in each bank
                               counts its own predominant thread type. If set
                               to 1 the counters counts events for both
                               threads. Note, a counter can never be programmed
                               to only count the none predominant thread.
                            */
    UINT32 Enable : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               This field is the local enable for PerfMon
                               Counter. This bit must be asserted in order for
                               the PerfMon counter to begin counting the events
                               selected by the 'event select', 'unit mask'.
                               This bit gets AND-ed with the respective global
                               enable bit in register IA32_CR_PERF_GLOBAL_CTRL
                               to enable the counter to count.
                            */
    UINT32 Invert : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               This field controls the counting behavior of the
                               compound counter. When set to '1' the compound
                               counter will count cycles that the compound
                               event did not occur, When set to '0' the
                               compound counter will count cycles that the
                               compound event did occur.
                            */
    UINT32 EdgeDetect : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               When this bit is set to 0 no edge detection is
                               performed. We set to 1 edge detection is enabled
                               and detects when an event has crossed the
                               threshold value. For example if the threshold is
                               set to 2 and the current count is 0 and then on
                               the next cycle the current count is 2 or greater
                               an edge detection will ocurr which signals the
                               incrementer to increment by one. The if on the
                               next cycle The count coming is 2 or greater edge
                               detection will not fire. Now lets say five cycle
                               later the value drops to below two and then a
                               cycle later rises to two or greater an edge
                               detect will fire. Edge detection only signals
                               the counter to increment by 1. Note -> If edge
                               detection is set to 1 and the threshold is set
                               to zero edge detection is meaningless and
                               becomes disabled. This is because you can never
                               dip below a value of 0.
                            */
    UINT32 Rsvd25 : 7;

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 29;

                            /* Bits[60:32], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 UbrkptCtr0 : 1;

                            /* Bits[61:61], Access Type=RW, default=0x00000000*/

                            /* Override counter 0 MicroBreakPoint */
    UINT32 Forceubrkpt : 1;

                            /* Bits[62:62], Access Type=RW, default=0x00000000*/

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

} MSR_COMPOUND_CTR_CTRL_REGISTER;

/**
  IVB i1026245 - PerfMon Compound Counter enhancement
**/
#define MSR_COMPOUND_PERF_CTR 0x00000307

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Lower : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Lower Half of COMPOUND PERF CTR */
    UINT32 Upper : 16;

                            /* Bits[47:32], Access Type=RW, default=0x00000000*/

                            /* Upper Half of COMPOUND PERF CTR */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_COMPOUND_PERF_CTR_REGISTER;

/**
  Threshold value for the load latency
**/
#define MSR_PEBS_LD_LAT_THRESHOLD 0x000003F6

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Thrshold : 16;

                            /* Bits[15:0], Access Type=RW, default=0x0000FFFF*/

                            /* Threshold for the Latency value */
    UINT32 Rsvd16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PEBS_LD_LAT_THRESHOLD_REGISTER;
/**
  Time spent in the Core C-State.  It is given in units compatible to P1 clock frequency (Guaranteed / Maximum Core Non-Turbo Frequency). CNL NEW: Moved from uncore to core
**/
#define MSR_CORE_C6_RESIDENCY_COUNTER 0x000003FD

/**
  new SGX_DEBUG_MODE MSR to allow non-Intel debugger to enter debug mode equal to DEBUG_UNLOCK_MODE functionality
**/
#define MSR_SGX_DEBUG_MODE 0x00000503

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 NpkRequest : 1;

                            /* Bits[0:0], Access Type=RW, default=None*/

                            /* npk request */
    UINT32 DebugUnlockMode : 1;

                            /* Bits[1:1], Access Type=RW, default=None*/

                            /*
                               READ ONLY bit: copy of
                               EPTAP_CR_SGX_PPPE_DBG.DEBUG_MODE
                            */
    UINT32 Rsvd2 : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SGX_DEBUG_MODE_REGISTER;
/**
  Scratch Pad register PROBE_MODE_4
  C6NoSave Candidate
**/
#define MSR_PROBE_MODE_VMEXIT_REDIRECTION_REASON 0x00000504
/**
  VMX Redirection info used for Probe Mode
**/
#define MSR_PROBE_MODE_VMX_REDIRECTION_INFO 0x00000505
/**
  Not really a register. Probe mode MSR used to flush all TLBs.
**/
#define MSR_PROBE_TLB_FLUSH 0x00000508
/**
  Not really a register. Probe mode MSR used to read/write VMX pointers.
**/
#define MSR_PROBE_VMX_POINTER 0x00000509

/**
  Programmed in Probe mode, used in PDM.
  Need to save in C6
**/
#define MSR_AET_MISC_ENABLE 0x00000511

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 HwInt : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* HW Interrupt trace enable */
    UINT32 Iret : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* IRET trace enable */
    UINT32 Exception : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Exception trace enable */
    UINT32 Msr : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* MSR trace enable */
    UINT32 PowerEvent : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 Io : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* IO trace enable */
    UINT32 Se : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Secure Enclaves entry and exit trace enable */
    UINT32 Wbinvd : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* WBINVD trace enable */
    UINT32 Rsvd8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 CodeBreakpoint : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* Code breakpoint trace enable */
    UINT32 DataBreakpoint : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Data breakpoint trace enable */
    UINT32 Rsvd11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 Btm : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /* BTM trace enable */
    UINT32 Smi : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /* SMI trace enable */
    UINT32 Mwait : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* Monitor/Mwait trace enable */
    UINT32 Rsvd15 : 13;

                            /* Bits[27:15], Access Type=RW, default=None*/

                            /* Reserved */
    UINT32 LipDisable : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /* disable LIP for the pdm packet. */
    UINT32 RicDisable : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* disnable RIC for the pdm packet. */
    UINT32 Rsvd30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_AET_MISC_ENABLE_REGISTER;

/**
  AET (PDM) Event Control Register
**/
#define MSR_AET_EVENT_CTRL 0x00000512

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 HwInt : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* HW Interrupt trace enable */
    UINT32 Iret : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* IRET trace enable */
    UINT32 Exception : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Exception trace enable */
    UINT32 Msr : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* MSR trace enable */
    UINT32 PowerEvent : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 Io : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* IO trace enable */
    UINT32 Se : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Secure Enclaves entry and exit trace enable */
    UINT32 Wbinvd : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* WBINVD trace enable */
    UINT32 Rsvd8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 CodeBreakpoint : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* Code breakpoint trace enable */
    UINT32 DataBreakpoint : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Data breakpoint trace enable */
    UINT32 Rsvd11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 Btm : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /* BTM trace enable */
    UINT32 Smi : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /* SMI trace enable */
    UINT32 Mwait : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* Monitor/Mwait trace enable */
    UINT32 Rsvd15 : 15;

                            /* Bits[29:15], Access Type=RW, default=None*/

                            /* Reserved */
    UINT32 Npk : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* NPK output enable */
    UINT32 PdmMasterEnable : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* Mater enable for PDM. */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_AET_EVENT_CTRL_REGISTER;
/**
  PPPE Redirection info used for Probe Mode
**/
#define MSR_PROBE_MODE_PPPE_REDIRECTION_INFO 0x00000513
/**
  This define is used to reserve some MSR addresses for probe mode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_PROBE_MODE_RESERVED_1 0x00000514
/**
  This define is used to reserve some MSR addresses for probe mode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_PROBE_MODE_RESERVED_2 0x00000515
/**
  This define is used to reserve some MSR addresses for probe mode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_PROBE_MODE_RESERVED_3 0x00000516
/**
  This define is used to reserve some MSR addresses for probe mode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_PROBE_MODE_RESERVED_4 0x00000517
/**
  This define is used to reserve some MSR addresses for probe mode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_PROBE_MODE_RESERVED_5 0x00000518
/**
  This define is used to reserve some MSR addresses for probe mode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_PROBE_MODE_RESERVED_6 0x00000519
/**
  This define is used to reserve some MSR addresses for probe mode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_PROBE_MODE_RESERVED_7 0x0000051A
/**
  This define is used to reserve some MSR addresses for probe mode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_PROBE_MODE_RESERVED_8 0x0000051B
/**
  This define is used to reserve some MSR addresses for probe mode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_PROBE_MODE_RESERVED_9 0x0000051C
/**
  This define is used to reserve some MSR addresses for probe mode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_PROBE_MODE_RESERVED_10 0x0000051D
/**
  This define is used to reserve some MSR addresses for probe mode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_PROBE_MODE_RESERVED_11 0x0000051E
/**
  This define is used to reserve some MSR addresses for probe mode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_PROBE_MODE_RESERVED_12 0x0000051F
/**
  Scratch Pad register ICECTLPMR_PRIOR_STATE
  Used only by probe mode.
  C6NoSave candidate
**/
#define MSR_ICECTLPMR_PRIOR_STATE 0x00000520

/**
  Defines units for calculating SKU power and timing parameters.
**/
#define MSR_PACKAGE_POWER_SKU_UNIT 0x00000606

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 PwrUnit : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000003*/

                            /*
                               Power Units used for power control registers.
                               The actual unit value is calculated by 1 W /
                               Power(2,PWR_UNIT). The default value of 0011b
                               corresponds to 1/8 W.
                            */
    UINT32 Rsvd4 : 4;

                            /* Bits[7:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 EnergyUnit : 5;

                            /* Bits[12:8], Access Type=RW, default=0x0000000E*/

                            /*
                               Energy Units used for power control registers.
                               The actual unit value is calculated by 1 J /
                               Power(2,ENERGY_UNIT). The default value of 14
                               corresponds to Ux.14 number.
                            */
    UINT32 Rsvd13 : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 TimeUnit : 4;

                            /* Bits[19:16], Access Type=RW, default=0x0000000A*/

                            /*
                               Time Units used for power control registers. The
                               actual unit value is calculated by 1 s /
                               Power(2,TIME_UNIT). The default value of Ah
                               corresponds to 976 usec.
                            */
    UINT32 Rsvd20 : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PACKAGE_POWER_SKU_UNIT_REGISTER;

/**
  Control and Status register for the BIOS-to-PCODE mailbox.  This mailbox is implemented as a means for accessing statistics and implementing PCODE patches.
  This register is used in conjunction with BIOS_MAILBOX_DATA.
  THIS REGISTER IS DUPLICATED IN THE PCU I/O SPACE, XML CHANGES MUST BE MADE IN BOTH PLACES.
**/
#define MSR_BIOS_MAILBOX_INTERFACE 0x00000607

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Command : 8;

                            /* Bits[7:0], Access Type=RW_V, default=0x00000000*/

                            /*
                               This field contains the SW request command or
                               the PCODE response code, depending on the
                               setting of RUN_BUSY.
                            */
    UINT32 Addr : 21;

                            /* Bits[28:8], Access Type=RW_V, default=0x00000000*/

                            /*
                               This field contains the address associated with
                               specific commands.
                            */
    UINT32 Rsvd29 : 2;

                            /* Bits[30:29], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 RunBusy : 1;

                            /* Bits[31:31], Access Type=RW1S, default=0x00000000*/

                            /*
                               SW may write to the two mailbox registers only
                               when RUN_BUSY is cleared (0b). Setting RUN_BUSY
                               to 1b will create a Fast Path event. After
                               setting this bit, SW will poll this bit until it
                               is cleared. PCODE will clear RUN_BUSY after
                               updating the mailbox registers with the result
                               and error code.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_BIOS_MAILBOX_INTERFACE_REGISTER;
/**
  Data register for the BIOS-to-PCODE mailbox.  This mailbox is implemented as a means for accessing statistics and implementing PCODE patches.
  This register is used in conjunction with BIOS_MAILBOX_INTERFACE.
  THIS REGISTER IS DUPLICATED IN THE PCU I/O SPACE, XML CHANGES MUST BE MADE IN BOTH PLACES.
**/
#define MSR_BIOS_MAILBOX_DATA 0x00000608

/**
  This MSR reads/writes THREAD_P_REQ[P_STATE_OFFSET]. HSW ECO 4400429
**/
#define MSR_PST_CONFIG_CONTROL 0x00000609

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 PStateOffset : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /* P-state offset of Thread-P-Req */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PST_CONFIG_CONTROL_REGISTER;

/**
  The coordinated Package-Additive (above the core) Interrupt Response Time is used for BIOS runtime control.  This setting affects the selected package state.
**/
#define MSR_C_STATE_LATENCY_CONTROL_0 0x0000060A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Value : 10;

                            /* Bits[9:0], Access Type=RW, default=0x00000000*/

                            /*
                               The Interrupt Response Time Limit is given in
                               units defined in the Multipler field of this
                               register.
                            */
    UINT32 Multiplier : 3;

                            /* Bits[12:10], Access Type=RW, default=0x00000000*/

                            /*
                               This field indicates the unit of measurement
                               that is defined for the Value field in this
                               register.
                            */
    UINT32 Rsvd13 : 2;

                            /* Bits[14:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Valid : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               This field qualifies the validity of the Value
                               field in this register.
                            */
    UINT32 Rsvd16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_C_STATE_LATENCY_CONTROL_0_REGISTER;

/**
  The coordinated Package-Additive (above the core) Interrupt Response Time is used for BIOS runtime control.  This setting affects the selected package state.
**/
#define MSR_C_STATE_LATENCY_CONTROL_1 0x0000060B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Value : 10;

                            /* Bits[9:0], Access Type=RW, default=0x00000000*/

                            /*
                               The Interrupt Response Time Limit is given in
                               units defined in the Multipler field of this
                               register.
                            */
    UINT32 Multiplier : 3;

                            /* Bits[12:10], Access Type=RW, default=0x00000000*/

                            /*
                               This field indicates the unit of measurement
                               that is defined for the Value field in this
                               register.
                            */
    UINT32 Rsvd13 : 2;

                            /* Bits[14:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Valid : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               This field qualifies the validity of the Value
                               field in this register.
                            */
    UINT32 Rsvd16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_C_STATE_LATENCY_CONTROL_1_REGISTER;

/**
  The coordinated Package-Additive (above the core) Interrupt Response Time is used for BIOS runtime control.  This setting affects the selected package state.
**/
#define MSR_C_STATE_LATENCY_CONTROL_2 0x0000060C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Value : 10;

                            /* Bits[9:0], Access Type=RW, default=0x00000000*/

                            /*
                               The Interrupt Response Time Limit is given in
                               units defined in the Multipler field of this
                               register.
                            */
    UINT32 Multiplier : 3;

                            /* Bits[12:10], Access Type=RW, default=0x00000000*/

                            /*
                               This field indicates the unit of measurement
                               that is defined for the Value field in this
                               register.
                            */
    UINT32 Rsvd13 : 2;

                            /* Bits[14:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Valid : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               This field qualifies the validity of the Value
                               field in this register.
                            */
    UINT32 Rsvd16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_C_STATE_LATENCY_CONTROL_2_REGISTER;

/**
  This register allows platform BIOS to limit the power consumption of the processor to the specified values.
**/
#define MSR_PACKAGE_POWER_LIMIT 0x00000610

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 PkgPwrLim1 : 15;

                            /* Bits[14:0], Access Type=RW_L, default=0x00000118*/

                            /* This field indicates the power limitation #1. */
    UINT32 PkgPwrLim1En : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000001*/

                            /* Package Power Limit 1 is always enabled */
    UINT32 PkgClmpLim1 : 1;

                            /* Bits[16:16], Access Type=RW_L, default=0x00000000*/

                            /*
                               Package Clamping limitation #1 - Allow going
                               below P1. 0b PBM is limited between P1 and P0.
                               1b PBM can go below P1.
                            */
    UINT32 PkgPwrLim1Time : 7;

                            /* Bits[23:17], Access Type=RW_L, default=0x0000000A*/

                            /*
                               x = PKG_PWR_LIM_1_TIME[23:22] y =
                               PKG_PWR_LIM_1_TIME[21:17] The timing interval
                               window is Floating Point number given by 1.x *
                               power(2,y). The unit of measurement is defined
                               in PACKAGE_POWER_SKU_UNIT_MSR[TIME_UNIT]. The
                               maximal time window is bounded by
                               PACKAGE_POWER_SKU_MSR[PKG_MAX_WIN]. The minimum
                               time window is 1 unit of measurement (as defined
                               above).
                            */
    UINT32 Rsvd24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 PkgPwrLim2 : 15;

                            /* Bits[46:32], Access Type=RW_L, default=0x00000000*/

                            /*
                               This field indicates the power limitation #2.
                               The unit of measurement is defined in
                               PACKAGE_POWER_SKU_UNIT_MSR[PWR_UNIT].
                            */
    UINT32 PkgPwrLim2En : 1;

                            /* Bits[47:47], Access Type=RW_L, default=0x00000000*/

                            /*
                               This bit enables/disables PKG_PWR_LIM_2. 0b
                               Package Power Limit 2 is Disabled 1b Package
                               Power Limit 2 is Enabled
                            */
    UINT32 PkgClmpLim2 : 1;

                            /* Bits[48:48], Access Type=RW_L, default=0x00000000*/

                            /*
                               Package Clamping limitation #2 - Allow going
                               below P1. 0b PBM is limited between P1 and P0.
                               1b PBM can go below P1.
                            */
    UINT32 PkgPwrLim2Time : 7;

                            /* Bits[55:49], Access Type=RW_L, default=0x00000000*/

                            /*
                               x = PKG_PWR_LIM_2_TIME[55:54] y =
                               PKG_PWR_LIM_2_TIME[53:49] The timing interval
                               window is Floating Point number given by 1.x *
                               power(2,y). The unit of measurement is defined
                               in PACKAGE_POWER_SKU_UNIT_MSR[TIME_UNIT]. The
                               maximal time window is bounded by
                               PACKAGE_POWER_SKU_MSR[PKG_MAX_WIN]. The minimum
                               time window is 1 unit of measurement (as defined
                               above).
                            */
    UINT32 Rsvd56 : 7;

                            /* Bits[62:56], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 PkgPwrLimLock : 1;

                            /* Bits[63:63], Access Type=RW_KL, default=0x00000000*/

                            /*
                               When set, all settings in this register are
                               locked and are treated as Read Only. This bit
                               will typically set by BIOS during boot time or
                               resume from Sx.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PACKAGE_POWER_LIMIT_REGISTER;
/**
  Package energy consumed by the entire CPU (including IA, GT and uncore).  The counter will wrap around and continue counting when it reaches its limit.
  The energy status is reported in units which are defined in PACKAGE_POWER_SKU_UNIT_MSR[ENERGY_UNIT].
  The data is updated by PCODE and is Read Only for all SW.
  THIS REGISTER IS DUPLICATED IN THE PCU I/O SPACE, XML CHANGES MUST BE MADE IN BOTH PLACES.
**/
#define MSR_PACKAGE_ENERGY_STATUS 0x00000611

/**
  >Package energy consumed by the entire CPU (including IA, GT and uncore).  The counter will wrap around and continue counting when it reaches its limit.
  The energy status is reported in units which are defined in PACKAGE_POWER_SKU_UNIT_MSR[ENERGY_UNIT].
  The data is updated by PCODE and is Read Only for all SW
**/
#define MSR_PACKAGE_ENERGY_TIME_STATUS 0x00000612

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 TotalEnergyConsumed : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Total amount of energy consumed since last
                               reset. This is a monotonic increment counter
                               with auto wrap back to zero after overflow. Unit
                               is 61uJ
                            */
    UINT32 TotalTimeElapsed : 32;

                            /* Bits[63:32], Access Type=RW, default=0x00000000*/

                            /*
                               Total time elapsed when the energy was last
                               updated. This is a monotonic increment counter
                               with auto wrap back to zero after overflow. Unit
                               is 10ns. SW calculates the delta change from
                               previous readings in order to determine the
                               power consumed. Power = delta_energy /
                               delta_time
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PACKAGE_ENERGY_TIME_STATUS_REGISTER;
/**
  Package RAPL Performance Status Register. This register provides information on the performance impact of the RAPL power limit and indicates the duration for processor went below the requested P-state due to package power constraint.
**/
#define MSR_PACKAGE_RAPL_PERF_STATUS 0x00000613

/**
  Defines allowed SKU power and timing parameters.
  PCODE will update the contents of this register.
**/
#define MSR_PACKAGE_POWER_SKU 0x00000614

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 PkgTdp : 15;

                            /* Bits[14:0], Access Type=RW, default=0x00000118*/

                            /*
                               The TDP package power setting allowed for this
                               part. The units for this value are defined in
                               PACKAGE_POWER_SKU_MSR[PWR_UNIT].
                            */
    UINT32 Rsvd15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 PkgMinPwr : 15;

                            /* Bits[30:16], Access Type=RWS, default=0x00000060*/

                            /*
                               The minimal package power setting allowed for
                               this part. Lower values will be clamped to this
                               value. The minimum setting is typical (not
                               guaranteed). The units for this value are
                               defined in PACKAGE_POWER_SKU_MSR[PWR_UNIT].
                            */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 PkgMaxPwr : 15;

                            /* Bits[46:32], Access Type=RWS, default=0x00000240*/

                            /*
                               The maximal package power setting allowed for
                               the SKU. Higher values will be clamped to this
                               value. The maximum setting is typical (not
                               guaranteed). The units for this value are
                               defined in PACKAGE_POWER_SKU_MSR[PWR_UNIT].
                            */
    UINT32 Rsvd47 : 1;

                            /* Bits[47:47], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 PkgMaxWin : 7;

                            /* Bits[54:48], Access Type=RWS, default=0x00000012*/

                            /*
                               The maximal time window allowed for the SKU.
                               Higher values will be clamped to this value. x =
                               PKG_MAX_WIN[54:53] y = PKG_MAX_WIN[52:48] The
                               timing interval window is Floating Point number
                               given by 1.x * power(2,y). The unit of
                               measurement is defined in
                               PACKAGE_POWER_SKU_UNIT_MSR[TIME_UNIT].
                            */
    UINT32 Rsvd55 : 9;

                            /* Bits[63:55], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PACKAGE_POWER_SKU_REGISTER;
/**
  Accumulates the energy consumed by the DIMMs (summed across all channels).
**/
#define MSR_DDR_ENERGY_STATUS 0x00000619
/**
  Memory RAPL performance excursion counter.  This register can report the performance impact of power limiting.
**/
#define MSR_DDR_RAPL_PERF_STATUS 0x0000061B

/**

**/
#define MSR_DRAM_POWER_INFO 0x0000061C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 DramTdp : 15;

                            /* Bits[14:0], Access Type=RO_V, default=0x00000118*/

                            /*
                               The Spec power allowed for DRAM. The TDP setting
                               is typical (not guaranteed). The units for this
                               value are defined in
                               DRAM_POWER_INFO_UNIT_MSR[PWR_UNIT].
                            */
    UINT32 Rsvd15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 DramMinPwr : 15;

                            /* Bits[30:16], Access Type=RO_V, default=0x00000078*/

                            /*
                               The minimal power setting allowed for DRAM.
                               Lower values will be clamped to this value. The
                               minimum setting is typical (not guaranteed). The
                               units for this value are defined in
                               DRAM_POWER_INFO_UNIT_MSR[PWR_UNIT].
                            */
    UINT32 Rsvd31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 DramMaxPwr : 15;

                            /* Bits[46:32], Access Type=RO_V, default=0x00000258*/

                            /*
                               The maximal power setting allowed for DRAM.
                               Higher values will be clamped to this value. The
                               maximum setting is typical (not guaranteed). The
                               units for this value are defined in
                               DRAM_POWER_INFO_UNIT_MSR[PWR_UNIT].
                            */
    UINT32 Rsvd47 : 1;

                            /* Bits[47:47], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 DramMaxWin : 7;

                            /* Bits[54:48], Access Type=RO_V, default=0x00000028*/

                            /*
                               The maximal time window allowed for the DRAM.
                               Higher values will be clamped to this value. x =
                               PKG_MAX_WIN[54:53] y = PKG_MAX_WIN[52:48] The
                               timing interval window is Floating Point number
                               given by 1.x * power(2,y). The unit of
                               measurement is defined in
                               DRAM_POWER_INFO_UNIT_MSR[TIME_UNIT].
                            */
    UINT32 Rsvd55 : 8;

                            /* Bits[62:55], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Lock : 1;

                            /* Bits[63:63], Access Type=RO_V, default=0x00000000*/

                            /* Lock bit to lock the Register */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_DRAM_POWER_INFO_REGISTER;

/**
  Time spent with LLC flushed.  It is given in units compatible to P1 clock frequency (Guaranteed / Maximum Core Non-Turbo Frequency).
**/
#define MSR_LLC_FLUSHED_RESIDENCY_COUNTER 0x0000061D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Data0 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Counter Value */
    UINT32 Data1 : 28;

                            /* Bits[59:32], Access Type=RW, default=0x00000000*/

                            /* Counter Value */
    UINT32 Rsvd60 : 4;

                            /* Bits[63:60], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_LLC_FLUSHED_RESIDENCY_COUNTER_REGISTER;

/**
  Min/Max Ratio Limits for Uncore
**/
#define MSR_UNCORE_RATIO_LIMIT 0x00000620

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 MaxClrRatio : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 Rsvd7 : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MinClrRatio : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 Rsvd15 : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_UNCORE_RATIO_LIMIT_REGISTER;

/**
  Gives SW ability to trigger a PMReq through Pcode to put the chipset and other devices
  off of the chipset into a low power state.
**/
#define MSR_PCH_EA_CONTROL 0x00000622

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 EaValue : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               0x0: Tell PCH that cores are not executing 0x1:
                               Tell PCH that cores are executing/Disable
                               Feature
                            */
    UINT32 Rsvd1 : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PCH_EA_CONTROL_REGISTER;

/**
  Reflect the value of control when PCH has responded to request from PCH_EA_CONTROL.
**/
#define MSR_PCH_EA_STATUS 0x00000623

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 EaStatus : 1;

                            /* Bits[0:0], Access Type=RW_L, default=0x00000001*/

                            /*
                               Reflects back the value of control once PCH has
                               responded to control request.
                            */
    UINT32 Rsvd1 : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PCH_EA_STATUS_REGISTER;

/**
  This register is used by BIOS/OS/Integrated Graphics Driver/CPM Driver to limit the power budget of the Primary Power Plane.
  The overall package turbo power limitation is controlled by PACKAGE_RAPL_LIMIT.
**/
#define MSR_PRIMARY_PLANE_TURBO_POWER_LIMIT 0x00000638

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 IaPpPwrLim : 15;

                            /* Bits[14:0], Access Type=RW_L, default=0x00000000*/

                            /*
                               This is the power limitation on the IA cores
                               power plane. The unit of measurement is defined
                               in PACKAGE_POWER_SKU_UNIT_MSR[PWR_UNIT].
                            */
    UINT32 PwrLimCtrlEn : 1;

                            /* Bits[15:15], Access Type=RW_L, default=0x00000000*/

                            /*
                               This bit must be set in order to limit the power
                               of the IA cores power plane. 0b IA cores power
                               plane power limitation is disabled 1b IA cores
                               power plane power limitation is enabled
                            */
    UINT32 PpClampLim : 1;

                            /* Bits[16:16], Access Type=RW_L, default=0x00000000*/

                            /*
                               Power Plane Clamping limitation - Allow going
                               below P1. 0b PBM is limited between P1 and P0.
                               1b PBM can go below P1.
                            */
    UINT32 CtrlTimeWin : 7;

                            /* Bits[23:17], Access Type=RW_L, default=0x00000000*/

                            /*
                               x = CTRL_TIME_WIN[23:22] y =
                               CTRL_TIME_WIN[21:17] The timing interval window
                               is Floating Point number given by 1.x *
                               power(2,y). The unit of measurement is defined
                               in PACKAGE_POWER_SKU_UNIT_MSR[TIME_UNIT]. The
                               maximal time window is bounded by
                               PACKAGE_POWER_SKU_MSR[PKG_MAX_WIN]. The minimum
                               time window is 1 unit of measurement (as defined
                               above).
                            */
    UINT32 Rsvd24 : 7;

                            /* Bits[30:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 PpPwrLimLock : 1;

                            /* Bits[31:31], Access Type=RW_KL, default=0x00000000*/

                            /*
                               When set, all settings in this register are
                               locked and are treated as Read Only.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PRIMARY_PLANE_TURBO_POWER_LIMIT_REGISTER;
/**
  Reports total energy consumed.  The counter will wrap around and continue counting when it reaches its limit.
  The energy status is reported in units which are defined in PACKAGE_POWER_SKU_UNIT_MSR[ENERGY_UNIT].
  SW will read this value and subtract the difference from last value read. The value of this register is updated every 1mSec.
  THIS REGISTER IS DUPLICATED IN THE PCU I/O SPACE, XMl CHANGES MUST BE MADE IN BOTH PLACES.
**/
#define MSR_PRIMARY_PLANE_ENERGY_STATUS 0x00000639

/**
  This register is used to indicate the Nominal Configurable TDP ratio available for this
  specific sku. System BIOS must use this value while building the _PSS table if the feature is enabled.
**/
#define MSR_CONFIG_TDP_NOMINAL 0x00000648

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 TdpRatio : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               Nominal TDP level ratio to be used for this
                               specific processor (in units of 100 MHz). Note:
                               A value of 0 in this field indicates
                               invalid/undefined TDP point
                            */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_CONFIG_TDP_NOMINAL_REGISTER;

/**
  Rd/Wr register to allow platform SW to select TDP point and set lock
**/
#define MSR_CONFIG_TDP_CONTROL 0x0000064B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 TdpLevel : 2;

                            /* Bits[1:0], Access Type=RWS_L, default=0x00000000*/

                            /*
                               Config TDP level selected 0 = nominal TDP level
                               (default) 1 = Level from CONFIG_TDP_LEVEL_1 2 =
                               Level from CONFIG_TDP_LEVEL_2 3 = reserved
                            */
    UINT32 Rsvd2 : 29;

                            /* Bits[30:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ConfigTdpLock : 1;

                            /* Bits[31:31], Access Type=RWS_KL, default=0x00000000*/

                            /*
                               Config TDP level select lock 0 - unlocked. 1 -
                               locked till next reset.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_CONFIG_TDP_CONTROL_REGISTER;

/**
  Read/write register to allow MSR/MMIO access to ACPI P-state notify (PCS 33).
**/
#define MSR_TURBO_ACTIVATION_RATIO 0x0000064C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 MaxNonTurboRatio : 8;

                            /* Bits[7:0], Access Type=RWS_L, default=0x00000000*/

                            /*
                               CPU will treat any P-state request above this
                               ratio as a request for max turbo 0 is special
                               encoding which disables the feature.
                            */
    UINT32 Rsvd8 : 23;

                            /* Bits[30:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 TurboActivationRatioLock : 1;

                            /* Bits[31:31], Access Type=RWS_KL, default=0x00000000*/

                            /*
                               Lock this MSR until next reset 0 - unlocked 1 -
                               locked
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_TURBO_ACTIVATION_RATIO_REGISTER;

#if 0 // Duplicate with ArchMsr.h.
/**
  Interface to allow software to determine what is causing resolved frequency to be clamped
  below the requested frequency. Status bits are updated by pcode through the io interface IO_IA_PERF_LIMIT,
  log bits are set by hw on a status bit edge dected and cleared by a SW write of '0'.
**/
#define MSR_IA_PERF_LIMIT_REASONS 0x0000064F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Prochot : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               PROCHOT# Status, RO, When set by PCODE indicates
                               that PROCHOT# has cause IA frequency clipping
                            */
    UINT32 Thermal : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Thermal Status, RO, When set by PCODE indicates
                               that Thermal event has cause IA frequency
                               clipping
                            */
    UINT32 SpareIa2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 SpareIa3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 RsrLimit : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Reliability stress Restrictor Status, RO, When
                               set by PCODE indicates that Reliability stress
                               restrictor has cause IA frequency clipping
                            */
    UINT32 Ratl : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Running average thermal limit Status, R0, When
                               set by PCODE indicates that Running average
                               thermal limit has cause IA frequency clipping
                            */
    UINT32 VrThermalert : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Hot VR (any processor VR) Status, RO, When set
                               by PCODE indicates that Hot VR (any processor
                               VR) has cause IA frequency clipping
                            */
    UINT32 VrTdc : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               VR TDC (Thermal design current) Status, RO, When
                               set by PCODE indicates that VR TDC (Thermal
                               design current has cause IA frequency clipping
                            */
    UINT32 Other : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Other (IccMax, PL4, etc) Status, RO, When set by
                               PCODE indicates that other has cause reason IA
                               frequency clipping
                            */
    UINT32 SpareIa9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 PbmPl1 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               PBM PL1 (pkg, platform), RO, When set by PCODE
                               indicates that PBM PL1 (package or platform PL1)
                               has cause IA frequency clipping
                            */
    UINT32 PbmPl2 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               PBM PL2, PL3 (pkg, platform) Status, RO, When
                               set by PCODE indicates that PBM PL2 or
                               PL3(package or platform PL2 or PL3) has cause IA
                               frequency clipping
                            */
    UINT32 MaxTurboLimit : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Max turbo limit Status, RO, When set by PCODE
                               indicates that Max turbo limit has cause IA
                               frequency clipping
                            */
    UINT32 TurboAtten : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Turbo attenuation (multi core turbo) Status, RO,
                               When set by PCODE indicates that Turbo
                               attenuation (multi core turbo) has cause IA
                               frequency clipping
                            */
    UINT32 SpareIa14 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 SpareIa15 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 ProchotLog : 1;

                            /* Bits[16:16], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               PROCHOT# Log, RW, When set by PCODE indicates
                               that PROCHOT# has cause IA frequency clipping.
                               Software should write to this bit to clear the
                               status in this bit
                            */
    UINT32 ThermalLog : 1;

                            /* Bits[17:17], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Thermal Log, RW, When set by PCODE indicates
                               that Thermal event has cause IA frequency
                               clipping. Software should write to this bit to
                               clear the status in this bit
                            */
    UINT32 SpareIa2Log : 1;

                            /* Bits[18:18], Access Type=RW0C_FW, default=0x00000000*/

                            /* Reserved */
    UINT32 SpareIa3Log : 1;

                            /* Bits[19:19], Access Type=RW0C_FW, default=0x00000000*/

                            /* Reserved */
    UINT32 RsrLimitLog : 1;

                            /* Bits[20:20], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Reliability stress restrictor Log, RW, When set
                               by PCODE indicates that Reliability stress
                               restrictor has cause IA frequency clipping.
                               Software should write to this bit to clear the
                               status in this bit
                            */
    UINT32 RatlLog : 1;

                            /* Bits[21:21], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Running average thermal limit Log, RW, When set
                               by PCODE indicates that Running average thermal
                               limit has cause IA frequency clipping. Software
                               should write to this bit to clear the status in
                               this bit
                            */
    UINT32 VrThermalertLog : 1;

                            /* Bits[22:22], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Hot VR (any processor VR) Log, RW, When set by
                               PCODE indicates that Hot VR (any processor VR)
                               has cause IA frequency clipping. Software should
                               write to this bit to clear the status in this
                               bit
                            */
    UINT32 VrTdcLog : 1;

                            /* Bits[23:23], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               VR TDC (Thermal design current) Log, RW, When
                               set by PCODE indicates that VR TDC (Thermal
                               design current has cause IA frequency clipping.
                               Software should write to this bit to clear the
                               status in this bit
                            */
    UINT32 OtherLog : 1;

                            /* Bits[24:24], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Other (IccMax, PL4, etc) Log, RW, When set by
                               PCODE indicates that other has cause reason IA
                               frequency clipping. Software should write to
                               this bit to clear the status in this bit
                            */
    UINT32 SpareIa9Log : 1;

                            /* Bits[25:25], Access Type=RW0C_FW, default=0x00000000*/

                            /* Reserved */
    UINT32 PbmPl1Log : 1;

                            /* Bits[26:26], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               PBM PL1 (pkg, platform) Log, RW, When set by
                               PCODE indicates that PBM PL1 (package or
                               platform PL1) has cause IA frequency clipping.
                               Software should write to this bit to clear the
                               status in this bit
                            */
    UINT32 PbmPl2Log : 1;

                            /* Bits[27:27], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               PBM PL2, PL3 (pkg, platform) Log, RW, When set
                               by PCODE indicates that PBM PL2 or PL3(package
                               or platform PL2 or PL3) has cause IA frequency
                               clipping. Software should write to this bit to
                               clear the status in this bit
                            */
    UINT32 MaxTurboLimitLog : 1;

                            /* Bits[28:28], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Max turbo limit Log, RW, When set by PCODE
                               indicates that Max turbo limit has cause IA
                               frequency clipping. Software should write to
                               this bit to clear the status in this bit
                            */
    UINT32 TurboAttenLog : 1;

                            /* Bits[29:29], Access Type=RW0C_FW, default=0x00000000*/

                            /*
                               Turbo attenuation (multi core turbo) Log, RW,
                               When set by PCODE indicates that Turbo
                               attenuation (multi core turbo) has cause IA
                               frequency clipping. Software should write to
                               this bit to clear the status in this bit
                            */
    UINT32 SpareIa14Log : 1;

                            /* Bits[30:30], Access Type=RW0C_FW, default=0x00000000*/

                            /* Reserved */
    UINT32 SpareIa15Log : 1;

                            /* Bits[31:31], Access Type=RW0C_FW, default=0x00000000*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_IA_PERF_LIMIT_REASONS_REGISTER;
#endif
/**
  Core SDC Residency counter
**/
#define MSR_CORE_SDC_RESIDENCY 0x00000653
/**
  Core C1 Residency counter
**/
#define MSR_CORE_C1_RESIDENCY 0x00000660
/**
  Spare Virtual MSRs in uncore Creg PLA
**/
#define MSR_RESIDENCY_CTR_SELECT 0x0000071B
/**
  Spare Virtual MSRs in uncore Creg PLA
**/
#define MSR_RESIDENCY_CTR_CONFIG 0x0000071C
/**
  Spare Virtual MSRs in uncore Creg PLA
**/
#define MSR_RESIDENCY_CTR 0x0000071D

/**
  This MSR will write the ERRINJLCK bit of IIO, QPI, CBO, MC
**/
#define MSR_ERR_INJ_LCK_UNLOCK_CTL 0x00000790

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ErrInjLckUnlock : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Bit indicates if error injection is enabled 0 -
                               unlocked 1 - locked (not enabled). note that
                               this field actually not used by ucode
                            */
    UINT32 Rsvd1 : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_ERR_INJ_LCK_UNLOCK_CTL_REGISTER;
/**
  Spare Virtual MSRs in uncore Creg PLA
**/
#define MSR_UNCORE_SPARE_1 0x00000792

/**
  Extended MCi Status Pointer
**/
#define MSR_EXTENDED_MCG_PTR 0x00000793

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 PhysMemPtr0 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Physical memory pointer. Points to the memory
                               (DRAM or MMIO) containing Extended_MCi_Status
                               structure.
                            */
    UINT32 PhysMemPtr1 : 30;

                            /* Bits[61:32], Access Type=RW, default=0x00000000*/

                            /*
                               Physical memory pointer. Points to the memory
                               (DRAM or MMIO) containing Extended_MCi_Status
                               structure.
                            */
    UINT32 Overflow : 1;

                            /* Bits[62:62], Access Type=RW, default=0x00000000*/

                            /* Overflow */
    UINT32 Valid : 1;

                            /* Bits[63:63], Access Type=RW, default=0x00000000*/

                            /* Valid */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_EXTENDED_MCG_PTR_REGISTER;
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_0 0x00000800
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_1 0x00000801

/**
  X2APIC Local APIC ID register
**/
#define MSR_PIC_EXTENDED_LOCAL_APIC_ID 0x00000802

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Tid : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Thread Id */
    UINT32 Cid : 3;

                            /* Bits[3:1], Access Type=RW, default=0x00000000*/

                            /* Core Id */
    UINT32 HighApicId : 11;

                            /* Bits[14:4], Access Type=RW, default=0x00000000*/

                            /* HIGH_APIC Id */
    UINT32 Rsvd15 : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PIC_EXTENDED_LOCAL_APIC_ID_REGISTER;

/**
  Local APIC Version register
**/
#define MSR_PIC_LOCAL_UNIT_VERSION 0x00000803

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 VersionId : 8;

                            /* Bits[7:0], Access Type=RO_V, default=0x00000015*/

                            /* APIC Version ID */
    UINT32 Rsvd8 : 8;

                            /* Bits[15:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 MaxLvtEntry : 8;

                            /* Bits[23:16], Access Type=RO_V, default=0x00000006*/

                            /* Index of highest LVT Entry */
    UINT32 DirectedEoiSupport : 1;

                            /* Bits[24:24], Access Type=RO_V, default=0x00000001*/

                            /*
                               If 1, this part contains support for Directed
                               EOI (EOI Broadcast Disable)
                            */
    UINT32 Rsvd25 : 7;

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PIC_LOCAL_UNIT_VERSION_REGISTER;
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_2 0x00000804
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_3 0x00000805
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_4 0x00000806
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_5 0x00000807

/**
  APIC Task Priority Register
**/
#define MSR_PIC_TASK_PRIORITY 0x00000808

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 TprValue : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Task Priority Register */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PIC_TASK_PRIORITY_REGISTER;
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_6 0x00000809

/**
  APIC Processor Priority Register
**/
#define MSR_PIC_PROCESSOR_PRIORITY 0x0000080A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 PprValue : 8;

                            /* Bits[7:0], Access Type=RO_V, default=0x00000000*/

                            /* Processor Priority Register */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PIC_PROCESSOR_PRIORITY_REGISTER;
/**
  APIC EOI Register
**/
#define MSR_PIC_EOI 0x0000080B
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_7 0x0000080C

/**
  X2APIC Logical Destination register
**/
#define MSR_PIC_EXTENDED_LOGICAL_DESTINATION 0x0000080D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ExtendedCidTid : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /* Logical ID 15:0 */
    UINT32 HighApicId : 11;

                            /* Bits[26:16], Access Type=RW, default=0x00000000*/

                            /* Logical ID 26:16 */
    UINT32 Rsvd27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PIC_EXTENDED_LOGICAL_DESTINATION_REGISTER;
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_8 0x0000080E

/**
  Spurious Interrupt Vector register
**/
#define MSR_PIC_SPURIOUS_INTERRUPT 0x0000080F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 SpuriousVector : 8;

                            /* Bits[7:0], Access Type=RW, default=0x000000FF*/

                            /* Spurious Interrupt Vector */
    UINT32 SoftwareEnable : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /* Software Enable Bit */
    UINT32 Rsvd9 : 3;

                            /* Bits[11:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Directedeoien : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /* EOI Suppression Bit */
    UINT32 Rsvd13 : 19;

                            /* Bits[31:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PIC_SPURIOUS_INTERRUPT_REGISTER;
/**
  APIC In-Service register bits
**/
#define MSR_PIC_ISR_DWORD_0 0x00000810
/**
  Register decription
**/
#define MSR_PIC_ISR_DWORD_1 0x00000811
/**
  Register decription
**/
#define MSR_PIC_ISR_DWORD_2 0x00000812
/**
  Register decription
**/
#define MSR_PIC_ISR_DWORD_3 0x00000813
/**
  Register decription
**/
#define MSR_PIC_ISR_DWORD_4 0x00000814
/**
  Register decription
**/
#define MSR_PIC_ISR_DWORD_5 0x00000815
/**
  Register decription
**/
#define MSR_PIC_ISR_DWORD_6 0x00000816
/**
  Register decription
**/
#define MSR_PIC_ISR_DWORD_7 0x00000817
/**
  APIC Trigger Mode register bits
**/
#define MSR_PIC_TMR_DWORD_0 0x00000818
/**
  Register decription
**/
#define MSR_PIC_TMR_DWORD_1 0x00000819
/**
  Register decription
**/
#define MSR_PIC_TMR_DWORD_2 0x0000081A
/**
  Register decription
**/
#define MSR_PIC_TMR_DWORD_3 0x0000081B
/**
  Register decription
**/
#define MSR_PIC_TMR_DWORD_4 0x0000081C
/**
  Register decription
**/
#define MSR_PIC_TMR_DWORD_5 0x0000081D
/**
  Register decription
**/
#define MSR_PIC_TMR_DWORD_6 0x0000081E
/**
  Register decription
**/
#define MSR_PIC_TMR_DWORD_7 0x0000081F
/**
  APIC Interrupt Request register bits
**/
#define MSR_PIC_IRR_DWORD_0 0x00000820
/**
  Register decription
**/
#define MSR_PIC_IRR_DWORD_1 0x00000821
/**
  Register decription
**/
#define MSR_PIC_IRR_DWORD_2 0x00000822
/**
  Register decription
**/
#define MSR_PIC_IRR_DWORD_3 0x00000823
/**
  Register decription
**/
#define MSR_PIC_IRR_DWORD_4 0x00000824
/**
  Register decription
**/
#define MSR_PIC_IRR_DWORD_5 0x00000825
/**
  Register decription
**/
#define MSR_PIC_IRR_DWORD_6 0x00000826
/**
  Register decription
**/
#define MSR_PIC_IRR_DWORD_7 0x00000827

/**
  APIC Error Status register
**/
#define MSR_PIC_ERROR_STATUS 0x00000828

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 4;

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 RedirectableIpi : 1;

                            /* Bits[4:4], Access Type=RO_V, default=0x00000000*/

                            /*
                               IPI with illegal Low Priority delivery mode
                               attempted from ICR write
                            */
    UINT32 SendIllegalVector : 1;

                            /* Bits[5:5], Access Type=RO_V, default=0x00000000*/

                            /* IPI with illegal vector attempted from ICR write */
    UINT32 RcvIllegalVector : 1;

                            /* Bits[6:6], Access Type=RO_V, default=0x00000000*/

                            /* Interrupt received with an illegal vector */
    UINT32 IllegalRegAddr : 1;

                            /* Bits[7:7], Access Type=RO_V, default=0x00000000*/

                            /*
                               APIC register access detected with an illegal
                               address
                            */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PIC_ERROR_STATUS_REGISTER;
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_9 0x00000829
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_10 0x0000082A
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_11 0x0000082B
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_12 0x0000082C
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_13 0x0000082D
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_14 0x0000082E

/**
  LVT Entry for Correctable MCA Interrupt
**/
#define MSR_PIC_LVT_CMCI 0x0000082F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Vector : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* PIC Correctable MCA Interrupt Vector */
    UINT32 DeliveryMode : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /* PIC Correctable MCA Interrupt Delivery Mode */
    UINT32 Rsvd11 : 1;

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 DeliveryStatus : 1;

                            /* Bits[12:12], Access Type=RO_V, default=0x00000000*/

                            /* PIC Correctable MCA Interrupt Delivery Status */
    UINT32 Rsvd13 : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Mask : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               PIC Correctable MCA Interrupt Mask Bit -
                               writeable only when APIC is SW Enabled
                            */
    UINT32 Rsvd17 : 15;

                            /* Bits[31:17], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PIC_LVT_CMCI_REGISTER;

/**
  APIC Interrupt Command register
**/
#define MSR_PIC_INTERRUPT_COMMAND 0x00000830

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Vector : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Interrupt Vector for outgoing IPI */
    UINT32 DeliveryMode : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /* Delivery Mode for outgoing IPI */
    UINT32 DestinationMode : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* Destination Mode for outgoing IPI */
    UINT32 DeliveryStatus : 1;

                            /* Bits[12:12], Access Type=RO_V, default=0x00000000*/

                            /* Delivery Status for outgoing IPI */
    UINT32 Rsvd13 : 1;

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Level : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* Level Trigger for outgoing IPI */
    UINT32 TriggerMode : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* Trigger Mode for outgoing IPI */
    UINT32 Rsvd16 : 2;

                            /* Bits[17:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 DestShorthand : 2;

                            /* Bits[19:18], Access Type=RW, default=0x00000000*/

                            /* Destination Shorthand for outgoing IPI */
    UINT32 Rsvd20 : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Destination : 27;

                            /* Bits[58:32], Access Type=RW, default=0x00000000*/

                            /* Destination for outgoing IPI */
    UINT32 Rsvd59 : 5;

                            /* Bits[63:59], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PIC_INTERRUPT_COMMAND_REGISTER;
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_15 0x00000831

/**
  LVT Entry for the PIC Timer
**/
#define MSR_PIC_LVT_TIMER 0x00000832

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Vector : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* PIC Timer Interrupt Vector */
    UINT32 Rsvd8 : 4;

                            /* Bits[11:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 DeliveryStatus : 1;

                            /* Bits[12:12], Access Type=RO_V, default=0x00000000*/

                            /* PIC Timer Interrupt Delivery Status */
    UINT32 Rsvd13 : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Mask : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               PIC Timer Interrupt Mask Bit - writeable only
                               when APIC is SW Enabled
                            */
    UINT32 TimerMode : 2;

                            /* Bits[18:17], Access Type=RW, default=0x00000000*/

                            /*
                               timer mode- single (00) periodic (01)
                               tsc_deadline (10) reserved (11)
                            */
    UINT32 Rsvd19 : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PIC_LVT_TIMER_REGISTER;

/**
  LVT Entry for the Thermal Interrupt
**/
#define MSR_PIC_LVT_THERM 0x00000833

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Vector : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* PIC Thermal Interrupt Vector */
    UINT32 DeliveryMode : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /* PIC Thermal Interrupt Delivery Mode */
    UINT32 Rsvd11 : 1;

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 DeliveryStatus : 1;

                            /* Bits[12:12], Access Type=RO_V, default=0x00000000*/

                            /* PIC Thermal Interrupt Delivery Status */
    UINT32 Rsvd13 : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Mask : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               PIC Thermal Interrupt Mask Bit - writeable only
                               when APIC is SW Enabled
                            */
    UINT32 Rsvd17 : 15;

                            /* Bits[31:17], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PIC_LVT_THERM_REGISTER;

/**
  LVT Entry for PerfMon Interrupt
**/
#define MSR_PIC_LVT_PERF 0x00000834

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Vector : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* PIC PerfMon Interrupt Vector */
    UINT32 DeliveryMode : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /* PIC PerfMon Interrupt Delivery Mode */
    UINT32 Rsvd11 : 1;

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 DeliveryStatus : 1;

                            /* Bits[12:12], Access Type=RO_V, default=0x00000000*/

                            /* PIC PerfMon Interrupt Delivery Status */
    UINT32 Rsvd13 : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Mask : 1;

                            /* Bits[16:16], Access Type=RW_V, default=0x00000001*/

                            /*
                               PIC PerfMon Interrupt Mask Bit - writeable only
                               when APIC is SW Enabled
                            */
    UINT32 Rsvd17 : 15;

                            /* Bits[31:17], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PIC_LVT_PERF_REGISTER;

/**
  LVT Entry for Lint Pin 0 Interrupt
**/
#define MSR_PIC_LVT_LINT0 0x00000835

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Vector : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* PIC Lint Pin 0 Interrupt Vector */
    UINT32 DeliveryMode : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /* PIC Lint Pin 0 Interrupt Delivery Mode */
    UINT32 Rsvd11 : 1;

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 DeliveryStatus : 1;

                            /* Bits[12:12], Access Type=RO_V, default=0x00000000*/

                            /* PIC Lint Pin 0 Interrupt Delivery Status */
    UINT32 Polarity : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /* PIC Lint Pin 0 Polarity */
    UINT32 Rirr : 1;

                            /* Bits[14:14], Access Type=RO_V, default=0x00000000*/

                            /* PIC Lint Pin 0 Remote IRR Bit */
    UINT32 TriggerMode : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* PIC Lint Pin 0 Trigger Mode */
    UINT32 Mask : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               PIC Lint Pin 0 Interrupt Mask Bit - writeable
                               only when APIC is SW Enabled
                            */
    UINT32 Rsvd17 : 15;

                            /* Bits[31:17], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PIC_LVT_LINT0_REGISTER;

/**
  LVT Entry for Lint Pin 1 Interrupt
**/
#define MSR_PIC_LVT_LINT1 0x00000836

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Vector : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* PIC Lint Pin 1 Interrupt Vector */
    UINT32 DeliveryMode : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /* PIC Lint Pin 1 Interrupt Delivery Mode */
    UINT32 Rsvd11 : 1;

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 DeliveryStatus : 1;

                            /* Bits[12:12], Access Type=RO_V, default=0x00000000*/

                            /* PIC Lint Pin 1 Interrupt Delivery Status */
    UINT32 Polarity : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /* PIC Lint Pin 1 Polarity */
    UINT32 Rirr : 1;

                            /* Bits[14:14], Access Type=RO_V, default=0x00000000*/

                            /* PIC Lint Pin 1 Interrupt Remote IRR */
    UINT32 TriggerMode : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* PIC Lint Pin 1 Trigger Mode */
    UINT32 Mask : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               PIC Lint Pin 1 Interrupt Mask Bit - writeable
                               only when APIC is SW Enabled
                            */
    UINT32 Rsvd17 : 15;

                            /* Bits[31:17], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PIC_LVT_LINT1_REGISTER;

/**
  LVT Entry for Error Interrupt
**/
#define MSR_PIC_LVT_ERROR 0x00000837

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Vector : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* PIC Error Interrupt Vector */
    UINT32 Rsvd8 : 4;

                            /* Bits[11:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 DeliveryStatus : 1;

                            /* Bits[12:12], Access Type=RO_V, default=0x00000000*/

                            /* PIC Error Interrupt Delivery Status */
    UINT32 Rsvd13 : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Mask : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               PIC Error Interrupt Mask Bit - writeable only
                               when APIC is SW Enabled
                            */
    UINT32 Rsvd17 : 15;

                            /* Bits[31:17], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PIC_LVT_ERROR_REGISTER;
/**
  APIC Initial Count register
**/
#define MSR_PIC_TIMER_INITIAL_COUNT_REG 0x00000838
/**
  APIC Current Count register
**/
#define MSR_PIC_TIMER_CURRENT_COUNT_REG 0x00000839
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_16 0x0000083A
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_17 0x0000083B
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_18 0x0000083C
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_19 0x0000083D

/**
  APIC Divide Configuration register
**/
#define MSR_PIC_TIMER_DIVIDE_CONFIG_REG 0x0000083E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 DivideSel : 4;

                            /* Bits[3:0], Access Type=RW_V, default=0x00000000*/

                            /* APIC LVT Interrupt Timer Counter Config */
    UINT32 Rsvd4 : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PIC_TIMER_DIVIDE_CONFIG_REG_REGISTER;

/**
  x2APIC Self IPI register
**/
#define MSR_SELF_IPI 0x0000083F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Vector : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Self IPI Vector */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SELF_IPI_REGISTER;
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_20 0x00000840
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_21 0x00000841
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_22 0x00000842
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_23 0x00000843
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_24 0x00000844
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_25 0x00000845
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_26 0x00000846
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_27 0x00000847
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_28 0x00000848
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_29 0x00000849
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_30 0x0000084A
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_31 0x0000084B
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_32 0x0000084C
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_33 0x0000084D
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_34 0x0000084E
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_35 0x0000084F
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_36 0x00000850
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_37 0x00000851
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_38 0x00000852
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_39 0x00000853
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_40 0x00000854
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_41 0x00000855
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_42 0x00000856
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_43 0x00000857
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_44 0x00000858
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_45 0x00000859
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_46 0x0000085A
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_47 0x0000085B
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_48 0x0000085C
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_49 0x0000085D
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_50 0x0000085E
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_51 0x0000085F
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_52 0x00000860
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_53 0x00000861
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_54 0x00000862
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_55 0x00000863
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_56 0x00000864
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_57 0x00000865
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_58 0x00000866
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_59 0x00000867
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_60 0x00000868
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_61 0x00000869
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_62 0x0000086A
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_63 0x0000086B
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_64 0x0000086C
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_65 0x0000086D
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_66 0x0000086E
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_67 0x0000086F
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_68 0x00000870
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_69 0x00000871
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_70 0x00000872
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_71 0x00000873
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_72 0x00000874
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_73 0x00000875
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_74 0x00000876
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_75 0x00000877
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_76 0x00000878
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_77 0x00000879
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_78 0x0000087A
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_79 0x0000087B
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_80 0x0000087C
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_81 0x0000087D
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_82 0x0000087E
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_83 0x0000087F
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_84 0x00000880
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_85 0x00000881
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_86 0x00000882
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_87 0x00000883
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_88 0x00000884
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_89 0x00000885
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_90 0x00000886
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_91 0x00000887
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_92 0x00000888
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_93 0x00000889
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_94 0x0000088A
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_95 0x0000088B
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_96 0x0000088C
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_97 0x0000088D
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_98 0x0000088E
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_99 0x0000088F
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_100 0x00000890
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_101 0x00000891
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_102 0x00000892
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_103 0x00000893
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_104 0x00000894
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_105 0x00000895
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_106 0x00000896
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_107 0x00000897
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_108 0x00000898
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_109 0x00000899
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_110 0x0000089A
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_111 0x0000089B
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_112 0x0000089C
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_113 0x0000089D
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_114 0x0000089E
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_115 0x0000089F
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_116 0x000008A0
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_117 0x000008A1
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_118 0x000008A2
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_119 0x000008A3
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_120 0x000008A4
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_121 0x000008A5
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_122 0x000008A6
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_123 0x000008A7
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_124 0x000008A8
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_125 0x000008A9
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_126 0x000008AA
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_127 0x000008AB
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_128 0x000008AC
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_129 0x000008AD
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_130 0x000008AE
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_131 0x000008AF
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_132 0x000008B0
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_133 0x000008B1
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_134 0x000008B2
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_135 0x000008B3
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_136 0x000008B4
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_137 0x000008B5
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_138 0x000008B6
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_139 0x000008B7
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_140 0x000008B8
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_141 0x000008B9
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_142 0x000008BA
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_143 0x000008BB
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_144 0x000008BC
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_145 0x000008BD
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_146 0x000008BE
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_147 0x000008BF
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_148 0x000008C0
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_149 0x000008C1
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_150 0x000008C2
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_151 0x000008C3
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_152 0x000008C4
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_153 0x000008C5
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_154 0x000008C6
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_155 0x000008C7
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_156 0x000008C8
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_157 0x000008C9
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_158 0x000008CA
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_159 0x000008CB
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_160 0x000008CC
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_161 0x000008CD
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_162 0x000008CE
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_163 0x000008CF
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_164 0x000008D0
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_165 0x000008D1
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_166 0x000008D2
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_167 0x000008D3
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_168 0x000008D4
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_169 0x000008D5
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_170 0x000008D6
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_171 0x000008D7
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_172 0x000008D8
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_173 0x000008D9
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_174 0x000008DA
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_175 0x000008DB
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_176 0x000008DC
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_177 0x000008DD
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_178 0x000008DE
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_179 0x000008DF
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_180 0x000008E0
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_181 0x000008E1
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_182 0x000008E2
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_183 0x000008E3
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_184 0x000008E4
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_185 0x000008E5
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_186 0x000008E6
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_187 0x000008E7
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_188 0x000008E8
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_189 0x000008E9
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_190 0x000008EA
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_191 0x000008EB
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_192 0x000008EC
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_193 0x000008ED
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_194 0x000008EE
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_195 0x000008EF
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_196 0x000008F0
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_197 0x000008F1
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_198 0x000008F2
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_199 0x000008F3
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_200 0x000008F4
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_201 0x000008F5
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_202 0x000008F6
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_203 0x000008F7
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_204 0x000008F8
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_205 0x000008F9
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_206 0x000008FA
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_207 0x000008FB
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_208 0x000008FC
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_209 0x000008FD
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_210 0x000008FE
/**
  Virtual APIC MSRs that are modeled by ucode
**/
#define MSR_VIRTUAL_APIC_211 0x000008FF
/**
  Contains Exit Qual used on SMI I/O exits
  C6NoSave Candidate
**/
#define MSR_VMX_IO_EXIT_QUAL 0x00000901

/**
  Millicode MSR used to read/write certain fields of the
  Macroalias register.
  WRMSR will ignore the undefined bits (63:18, 14:13, 10, 6:2).
  WRMSR will not signal fault on any combination of bits, it is
  softwares responsiblity to make sure that values programmed
  this register are consistent and correct
  RDMSR will always return 0 in bits 63:18, 14:13, 10:9, 6:2.
  RDMSR will return the last value successfully updated by WRMSR
**/
#define MSR_XU_MACROINSTRUCTION_ALIAS 0x00000903

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Scale : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               Scale: 00 -> scale 1, 01 -> scale 2, 10 -> scale
                               4, 11 -> scale 8
                            */
    UINT32 Rsvd2 : 5;

                            /* Bits[6:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Asize : 3;

                            /* Bits[9:7], Access Type=RW, default=0x00000000*/

                            /*
                               Address size: 000 -> 16-bit, 001 -> 32 bit, 010
                               -> 64-bit, 011-111 -> Reserved
                            */
    UINT32 Rsvd10 : 1;

                            /* Bits[10:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Osize : 2;

                            /* Bits[12:11], Access Type=RW, default=0x00000000*/

                            /*
                               Operand size: 00 -> DSZ16, 01 -> DSZ32, 10 ->
                               DSZ64, 11 -> Reserved
                            */
    UINT32 Rsvd13 : 2;

                            /* Bits[14:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Lseg : 3;

                            /* Bits[17:15], Access Type=RW, default=0x00000000*/

                            /*
                               Logical Segment: 000-> ES, 001-> CS, 010-> SS,
                               011-> DS, 100-> FS, 101-> GS, 110-111 ->
                               Reserved
                            */
    UINT32 Rsvd18 : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_XU_MACROINSTRUCTION_ALIAS_REGISTER;

/**
  Millicode MSR used to update the POST_VMXON and CURRENT_WP_IS_SHADOW_VMCS mode-based branch bits
  WRMSR will ignore the undefined bits
  RDMSR will always return 0 in undefined bits
**/
#define MSR_XU_VMX_CONTROL 0x00000904

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 PostVmxon : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Indicates whether the machine is in a post-VMXON
                               state
                            */
    UINT32 Rsvd2 : 4;

                            /* Bits[5:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 CurrentWpIsShadowVmcs : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Indicates whether the current working pointer is
                               a shadow VMCS
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

} MSR_XU_VMX_CONTROL_REGISTER;
/**
  Pause-Loop Exiting: First value
**/
#define MSR_VMX_PLE_FIRST 0x00000905
/**
  Pause-Loop Exiting: Last value
**/
#define MSR_VMX_PLE_LAST 0x00000906
/**
  Saved RSI value from IO SMI, used by parallel VMExit and SMI
  C6NoSave Candidate
**/
#define MSR_IO_SMI_BACKUP_RSI 0x00000909
/**
  Saved RCX value from IO SMI, used by parallel VMExit and SMI
  C6NoSave Candidate
**/
#define MSR_IO_SMI_BACKUP_RCX 0x0000090A
/**
  Saved RDI value from IO SMI, used by parallel VMExit and SMI
  C6NoSave Candidate
**/
#define MSR_IO_SMI_BACKUP_RDI 0x0000090B

/**
  Holds VPPR bits [7:4]
  FSCP Crunch:  4/32 Used only 4 bits out of 32
**/
#define MSR_VMX_VPPR 0x0000090C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Vppr74 : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /* VPPR bits [7:4] */
    UINT32 Reserved4 : 28;

                            /* Bits[31:4], Access Type=RW, default=0x00000000*/

                            /* New FSCPs added on HSW */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_VMX_VPPR_REGISTER;
/**
  Write only MSR for Xucode to write PDM data out to the GDXC
**/
#define MSR_PDM_GDXC_MSG 0x0000090D
/**
  MSR used to access the CAM Filter Table
**/
#define MSR_XU_TIO_CF_TABLE 0x00000913
/**
  VMX AVRR Remap Addr used in RR assist
**/
#define MSR_VMX_AVRR_REMAP 0x00000915
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_1 0x00000917
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_2 0x00000918
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_3 0x00000919
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_4 0x0000091A
/**
  PPPE Exception Handling Entrypoint
**/
#define MSR_EM_EXCEPTIONS_HANDLING_EIP 0x0000091B
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_5 0x0000091C
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_6 0x0000091D
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_7 0x0000091E
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_8 0x0000091F

/**
  Write only MSR for Xucode to modify the PDM header for GDXC messages
**/
#define MSR_PDM_HEADER 0x00000922

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 PacketType : 8;

                            /* Bits[7:0], Access Type=WO, default=0x00000000*/

                            /* GDXC Packet type */
    UINT32 PacketLength : 16;

                            /* Bits[23:8], Access Type=WO, default=0x00000000*/

                            /* GDXC Packet length */
    UINT32 Rsvd24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PDM_HEADER_REGISTER;

/**
  Timer control bits for various MLC/APIC Timers
**/
#define MSR_MLC_PWR_MGMT_TIMER_CTRL 0x00000923

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ApicTimerFreeze : 1;

                            /* Bits[0:0], Access Type=RO_V, default=0x00000000*/

                            /*
                               Set to disable APIC Timer InitCnt and CfgCnt
                               APIC Register updates. Does not directly freeze
                               the APIC Timer. Supposed to freeze CTC in probe-
                               mode, but bit is hardwired to 0 since SNB, not
                               used.
                            */
    UINT32 VtTimerFreeze : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Set to freeze VT timer in MLC */
    UINT32 AmcntFreeze : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Set to freeze ACNT/MCNT Timers in MLC */
    UINT32 McntEnable : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Set to enable MCNT Timer in MLC when the thread
                               is asleep. This would be set for conditions like
                               T-state throttling and P-state transitions where
                               MCNT is supposed to continue to count even while
                               the thread is asleep.
                            */
    UINT32 Rsvd4 : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MLC_PWR_MGMT_TIMER_CTRL_REGISTER;
/**
  Scratch space exposed via MSR in PPPE
**/
#define MSR_PPPE_DOORBELL_EXIT_BITMAP 0x00000924
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_9 0x00000928
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_10 0x00000929
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_11 0x0000092A
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_12 0x0000092B
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_13 0x0000092C
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_14 0x0000092D
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_15 0x0000092E

/**
  Register decription
**/
#define MSR_PIC_LT_DOORBELL 0x0000092F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 SenterEvent : 1;

                            /* Bits[0:0], Access Type=RW1S, default=0x00000000*/

                            /* Needs to be updated */
    UINT32 SenterContinueEvent : 1;

                            /* Bits[1:1], Access Type=RW1S, default=0x00000000*/

                            /* Needs to be updated */
    UINT32 SexitEvent : 1;

                            /* Bits[2:2], Access Type=RW1S, default=0x00000000*/

                            /* Needs to be updated */
    UINT32 SexitContinueEvent : 1;

                            /* Bits[3:3], Access Type=RW1S, default=0x00000000*/

                            /* Needs to be updated */
    UINT32 WakeupEvent : 1;

                            /* Bits[4:4], Access Type=RW1S, default=0x00000000*/

                            /* Needs to be updated */
    UINT32 Pppe : 3;

                            /* Bits[7:5], Access Type=RW1S, default=0x00000000*/

                            /* LT_DOORBELL[8:5] is reserved for PPPE. */
    UINT32 HsxReservedPfatEnter : 1;

                            /* Bits[8:8], Access Type=RW1S, default=0x00000000*/

                            /*
                               LT_DOORBELL[8] is used by Haswell Server as the
                               multi-socket PFAT ENTER DOORBELL.
                            */
    UINT32 UcodeRsvd : 2;

                            /* Bits[10:9], Access Type=RW1S, default=0x00000000*/

                            /* LT_DOORBELL[10:9] is reserved for ucode usage. */
    UINT32 PfatWakeup : 1;

                            /* Bits[11:11], Access Type=RW1S, default=0x00000000*/

                            /*
                               LT_DOORBELL[11] is used as the PFAT WAKEUP
                               DOORBELL.
                            */
    UINT32 RsvdExtLtdb12 : 5;

                            /* Bits[16:12], Access Type=RW1S, default=0x00000000*/

                            /* Needs to be updated */
    UINT32 NonWbLoadSeen : 1;

                            /* Bits[17:17], Access Type=RW1S, default=0x00000000*/

                            /* Needs to be updated */
    UINT32 MlcFillOccurred : 1;

                            /* Bits[18:18], Access Type=RW1S, default=0x00000000*/

                            /* Needs to be updated */
    UINT32 MlcEvictionOccurred : 1;

                            /* Bits[19:19], Access Type=RW1S, default=0x00000000*/

                            /* Needs to be updated */
    UINT32 HiddenLakeInterrupt : 1;

                            /* Bits[20:20], Access Type=RW1S, default=0x00000000*/

                            /* Hidden Lake Interrupt from NCU via serial bus */
    UINT32 Rsvd21 : 7;

                            /* Bits[27:21], Access Type=RW1S, default=0x00000000*/

                            /* Rsvd */
    UINT32 Xucode : 4;

                            /* Bits[31:28], Access Type=RW1S, default=0x00000000*/

                            /* Used by millicode */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PIC_LT_DOORBELL_REGISTER;
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_16 0x00000930
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_17 0x00000931
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_18 0x00000932

/**
  PPPE will have read/write access to the FSCP_CR_VMCS_CACHE_BASE register for implementing the VMCS cache (i387450).
  Can be cut to 32-bits if not used by PPPE
**/
#define MSR_VMCS_CACHE_BASE 0x00000933

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Address : 32;

                            /* Bits[31:0], Access Type=RW, default=None*/

                            /* Address of the VMCS cache for this thread */
    UINT32 Reserved32 : 32;

                            /* Bits[63:32], Access Type=RW, default=0x00000000*/

                            /* Unused */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_VMCS_CACHE_BASE_REGISTER;
/**
  Scratch Pad register BNS_CR_PSMBASE
**/
#define MSR_PSMI_BASE 0x00000934
/**
  VMX timer current count
**/
#define MSR_MLC_VT_TIMER_COUNT 0x00000937

/**
  VMX timer control
**/
#define MSR_MLC_VT_TIMER_CONTROL 0x00000938

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 TimerEnable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Set to enable VT timer */
    UINT32 Rsvd1 : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_MLC_VT_TIMER_CONTROL_REGISTER;

/**
  SMM IO MISC INFO Register. 32 bit. (Mrm roctlregc/roapes/roiomiscinfoSM306L)
**/
#define MSR_IOMISCINFO 0x00000939

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Smi : 1;

                            /* Bits[0:0], Access Type=RW_V, default=0x00000000*/

                            /* IO_SMI_POS */
    UINT32 IoLength : 3;

                            /* Bits[3:1], Access Type=RW, default=0x00000000*/

                            /* IO_LENGTH_MSB_POS:IO_LENGTH_LSB_POS */
    UINT32 IoType : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /* IO_TYPE_MSB_POS:IO_TYPE_LSB_POS */
    UINT32 Rsvd8 : 8;

                            /* Bits[15:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 IoPort : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* IO_PORT_MSB_POS:IO_PORT_LSB_POS */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_IOMISCINFO_REGISTER;
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_19 0x0000093B
/**
  Memory address accessed by the last IO instruction.
  Used to communicate between io and SMI/PPPE.
  C6NoSave candidate
**/
#define MSR_IO_MEM_ADDR 0x0000093E
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_21 0x00000944
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_22 0x00000945
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_23 0x00000946
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_24 0x00000947
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_25 0x00000948
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_26 0x00000949
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_27 0x0000094A
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_28 0x0000094B
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_29 0x0000094C
/**
  This define is used to reserve some MSR addresses for XuCode. Ucode would have a dedicated handler for these MSRs
**/
#define MSR_XU_RESERVED_30 0x0000094D

/**

**/
#define MSR_PKG_SE_SVN 0x00000952

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 PrResetSvn : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /* Reflection of PR_RESET_SVN for XuCode access */
    UINT32 RLastPatchSvn0 : 24;

                            /* Bits[31:8], Access Type=RW, default=None*/

                            /*
                               Reflection of R_LAST_PATCH_SVN[63:8] for XuCode
                               access
                            */
    UINT32 RLastPatchSvn1 : 32;

                            /* Bits[63:32], Access Type=RW, default=None*/

                            /*
                               Reflection of R_LAST_PATCH_SVN[63:8] for XuCode
                               access
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PKG_SE_SVN_REGISTER;

/**

**/
#define MSR_PPPE_TAP_STATUS 0x0000096B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 DebugUnlockMode : 1;

                            /* Bits[0:0], Access Type=RW, default=None*/

                            /* debug unlock mode */
    UINT32 Rsrvd1 : 31;

                            /* Bits[31:1], Access Type=RW, default=None*/

                            /* reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PPPE_TAP_STATUS_REGISTER;

/**
  Various controls
**/
#define MSR_IO_BW_P_LIMIT_OVERRIDE 0x00000A00

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 FloorOffset : 6;

                            /* Bits[5:0], Access Type=RW_V, default=0x00000000*/

                            /*
                               This value specifies the number of bins below P1
                               at which the freq floor will be set to.
                               Frequency Floor = P1 - 100 * OVRD_VALUE By
                               default: Freq floor wil lbe set to P1.
                            */
    UINT32 Rsvd6 : 2;

                            /* Bits[7:6], Access Type=RW_V, default=0x00000000*/

                            /*  */
    UINT32 FloorEnable : 1;

                            /* Bits[8:8], Access Type=RW_V, default=0x00000000*/

                            /* Overrides the Frequency Floor */
    UINT32 Rsvd9 : 23;

                            /* Bits[31:9], Access Type=RW_V, default=0x00000000*/

                            /*  */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_IO_BW_P_LIMIT_OVERRIDE_REGISTER;

/**
  This register effectively governs all major power saving engines and hueristics on the die.
**/
#define MSR_ENERGY_PERF_BIAS_CONFIG 0x00000A01

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 WorkldConfig : 3;

                            /* Bits[2:0], Access Type=RW_V, default=0x00000000*/

                            /*
                               This allows optimization for the workload
                               characterization. The three options for
                               selection: 000 - UMA - default 001 -NUMA
                               optimized - use NUMA table for Intel UPI 010 -
                               I/O sensitive - use IO BW P limit table 011 -
                               NUMA and I/O - use both Intel UPI and IO BW P
                               Lmit table This field indicates which table to
                               use.
                            */
    UINT32 AltEnergyPerfBias : 4;

                            /* Bits[6:3], Access Type=RW_V, default=0x00000000*/

                            /*  */
    UINT32 Rsvd7 : 5;

                            /* Bits[11:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 P0TotalTimeThresholdHigh : 6;

                            /* Bits[17:12], Access Type=RW_V, default=0x00000000*/

                            /*
                               The HW switching mechanism ENABLES the
                               performance setting (0) when the total P0 time
                               is greater than this threshhold.
                            */
    UINT32 P0TotalTimeThresholdLow : 6;

                            /* Bits[23:18], Access Type=RW_V, default=0x00000000*/

                            /*
                               The HW switching mechanism DISABLES the
                               performance setting (0) when the total P0 time
                               is less than this threshhold. It hen uses the
                               over rides when specified, otherwise it uses the
                               socket_IA32_Performance_Power_Bias.
                            */
    UINT32 AvgTimeWindow : 8;

                            /* Bits[31:24], Access Type=RW_V, default=0x00000000*/

                            /*
                               This field is used to control the decay constant
                               of the EWMA filter that is applied to core C0
                               and P0 time. The 8-bit value is distributed into
                               a 4-bit mantissa (M = Bits[7:4]) and 4-bit
                               exponent (E = Bits[3:0]). The formula for
                               computing the decay constant is 1-alpha =
                               (M+17)/pow(2,E+5). The filter is then applied as
                               follows: Y[n] = alpha*Y[n-1] + (1-alpha)*X[n],
                               where Y[n] is the output, X[n] is the input, and
                               n is the time step (in milliseconds).
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_ENERGY_PERF_BIAS_CONFIG_REGISTER;

/**
  This register is intended for OS usage. It enables the OS
  to write an LTR value if they choose to participate in the LTR process.
  This register includes parameters that PCODE will use to override information
  received from PCI Express via LTR messages.
  Pcode will choose this value, or PCIE_LTR_OVRD, or actual LTR values from IIO
  depending on selection function in POWER_CTL uCR
  PCODE will sample this register at slow loop.
  This register needs to be exposed as MSR - details TBD
**/
#define MSR_SW_LTR_OVRD 0x00000A02

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Nstl : 10;

                            /* Bits[9:0], Access Type=RW, default=0x00000000*/

                            /*
                               Latency requirement for Non-Snoop requests. This
                               value is multiplied by the MULTIPLIER field to
                               yield a time value, yielding an expressible
                               range from 1ns to 34,326.183,936 ns. Setting
                               this field and the MULTIPLIER to all 0s
                               indicates that the device will be impacted by
                               any delay and that the best possible service is
                               requested.
                            */
    UINT32 Multiplier : 3;

                            /* Bits[12:10], Access Type=RW, default=0x00000000*/

                            /*
                               This field indicates the scale that the NSTL
                               value is multipled by to yield a time value.
                            */
    UINT32 Rsvd13 : 1;

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ForceNl : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               When this bit is set, PCODE will choose the non-
                               snoop latency requirement from this register,
                               regardless of the LTR messages that are recieved
                               by any of the PCI Express controllers. When this
                               bit is clear, PCODE will choose the non-snoop
                               latency requirement as the minimum value taken
                               between this register and each of the LTR
                               messages that were received by the PCI Express
                               controllers with the Requirement bit set to 1b.
                            */
    UINT32 NlV : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               When this bit is set to 0b, PCODE will ignore
                               the Non- Snoop Latency override value.
                            */
    UINT32 Sxl : 10;

                            /* Bits[25:16], Access Type=RW, default=0x00000000*/

                            /*
                               Latency requirement for Snoop requests. This
                               value is multiplied by the SXL_MULTIPLIER field
                               to yield a time value, yielding an expressible
                               range from 1ns to 34,326.183,936 ns. Setting
                               this field and the SXL_MULTIPLIER to all 0s
                               indicates that the device will be impacted by
                               any delay and that the best possible service is
                               requested.
                            */
    UINT32 Sxlm : 3;

                            /* Bits[28:26], Access Type=RW, default=0x00000000*/

                            /*
                               This field indicates the scale that the SXL
                               value is multipled by to yield a time value.
                            */
    UINT32 Rsvd29 : 1;

                            /* Bits[29:29], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ForceSxl : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               When this bit is set, PCODE will choose the
                               snoop latency requirement from this register,
                               regardless of the LTR messages that are recieved
                               by any of the PCI Express controllers. When this
                               bit is clear, PCODE will choose the snoop
                               latency requirement as the minimum value taken
                               between this register and each of the LTR
                               messages that were received by the PCI Express
                               controllers with the Requirement bit set to 1b.
                            */
    UINT32 SxlV : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               When this bit is set to 0b, PCODE will ignore
                               the Snoop Latency override value.
                            */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SW_LTR_OVRD_REGISTER;

/**
  Available only in SMM. SMRAM copy of CR0 saved by SMI
**/
#define MSR_SMRAM_CR0 0x00000C00

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Pe : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Protection Enable */
    UINT32 Mp : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Monitor Coprocessor */
    UINT32 Em : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Emulation */
    UINT32 Ts : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Task Switched */
    UINT32 Et : 1;

                            /* Bits[4:4], Access Type=RO_V, default=None*/

                            /*
                               Extension Type (hardware for this bit always
                               reads as '1...no actual state here)
                            */
    UINT32 Ne : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Numeric Error */
    UINT32 Rsvd6 : 10;

                            /* Bits[15:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Wp : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /* Write Protect */
    UINT32 Rsvd17 : 1;

                            /* Bits[17:17], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Am : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /* Alignment Mask */
    UINT32 Rsvd19 : 10;

                            /* Bits[28:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Nw : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Not Write-through - During reset ucode the reset
                               thread will write this value to a '1 for itself,
                               and a '0 for the other thread
                            */
    UINT32 Cd : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Cache Disable - During reset ucode the reset
                               thread will write this value to a '1 for itself,
                               and a '0 for the other thread
                            */
    UINT32 Pg : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* Paging */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SMRAM_CR0_REGISTER;

/**
  Available only in SMM. SMRAM copy of EFLAGS saved by SMI
**/
#define MSR_SMRAM_EFLAGS 0x00000C02

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Cf : 1;

                            /* Bits[0:0], Access Type=RW, default=None*/

                            /* Carry */
    UINT32 Rsvd1 : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Pf : 1;

                            /* Bits[2:2], Access Type=RW, default=None*/

                            /* Parity */
    UINT32 Rsvd3 : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Af : 1;

                            /* Bits[4:4], Access Type=RW, default=None*/

                            /* AuxCarry */
    UINT32 Rsvd5 : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Zf : 1;

                            /* Bits[6:6], Access Type=RW, default=None*/

                            /* Zero */
    UINT32 Sf : 1;

                            /* Bits[7:7], Access Type=RW, default=None*/

                            /* Stack Fault */
    UINT32 Tf : 1;

                            /* Bits[8:8], Access Type=RW, default=None*/

                            /* Trap */
    UINT32 InterruptFlag : 1;

                            /* Bits[9:9], Access Type=RW, default=None*/

                            /* Interrupt Enable */
    UINT32 Df : 1;

                            /* Bits[10:10], Access Type=RW, default=None*/

                            /* Double-fault Exception */
    UINT32 Of : 1;

                            /* Bits[11:11], Access Type=RW, default=None*/

                            /* Overflow */
    UINT32 Iopl : 2;

                            /* Bits[13:12], Access Type=RW, default=None*/

                            /* I/O privilege level */
    UINT32 Nt : 1;

                            /* Bits[14:14], Access Type=RW, default=None*/

                            /* Nested Task */
    UINT32 Rsvd15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rf : 1;

                            /* Bits[16:16], Access Type=RW, default=None*/

                            /* Resume */
    UINT32 Vm : 1;

                            /* Bits[17:17], Access Type=RW, default=None*/

                            /* Virtual-8086 Mode */
    UINT32 Ac : 1;

                            /* Bits[18:18], Access Type=RW, default=None*/

                            /* Alignment Check */
    UINT32 Vif : 1;

                            /* Bits[19:19], Access Type=RW, default=None*/

                            /* Virtual Interrupt */
    UINT32 Vip : 1;

                            /* Bits[20:20], Access Type=RW, default=None*/

                            /* Virtual Interrupt Pending */
    UINT32 Id : 1;

                            /* Bits[21:21], Access Type=RW, default=None*/

                            /* Identification */
    UINT32 Rsvd22 : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SMRAM_EFLAGS_REGISTER;

/**
  Available only in SMM. SMRAM copy of EFER saved by SMI
**/
#define MSR_SMRAM_EFER 0x00000C03

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Sce : 1;

                            /* Bits[0:0], Access Type=RW, default=None*/

                            /* System Call Enable Bit */
    UINT32 Rsvd1 : 7;

                            /* Bits[7:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Lme : 1;

                            /* Bits[8:8], Access Type=RW, default=None*/

                            /* Long Mode Enabled */
    UINT32 Rsvd9 : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Lma : 1;

                            /* Bits[10:10], Access Type=RW, default=None*/

                            /* Long Mode Active */
    UINT32 Nxe : 1;

                            /* Bits[11:11], Access Type=RW, default=None*/

                            /* Enable No-Execute Mode */
    UINT32 Rsvd12 : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SMRAM_EFER_REGISTER;
/**
  Available only in SMM. SMRAM copy of RIP saved by SMI, introduced in HSW
**/
#define MSR_SMRAM_RIP 0x00000C04

/**
  Available only in SMM. SMRAM copy of DR6 saved by SMI
**/
#define MSR_SMRAM_DR6 0x00000C05

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 B0 : 1;

                            /* Bits[0:0], Access Type=RW, default=None*/

                            /* Breakpoint condition detected bit */
    UINT32 B1 : 1;

                            /* Bits[1:1], Access Type=RW, default=None*/

                            /* Breakpoint condition detected bit */
    UINT32 B2 : 1;

                            /* Bits[2:2], Access Type=RW, default=None*/

                            /* Breakpoint condition detected bit */
    UINT32 B3 : 1;

                            /* Bits[3:3], Access Type=RW, default=None*/

                            /* Breakpoint condition detected bit */
    UINT32 MustBeOneLow : 8;

                            /* Bits[11:4], Access Type=RW, default=None*/

                            /* Bits which are architecturally defined to be 1 */
    UINT32 MustBeZero : 1;

                            /* Bits[12:12], Access Type=RW, default=None*/

                            /* Bits which are architecturally defined to be 0 */
    UINT32 Bd : 1;

                            /* Bits[13:13], Access Type=RW, default=None*/

                            /*
                               Indicates that the next instruction in the
                               instruction stream will access one of the debug
                               registers when a debug fault handler is reached.
                            */
    UINT32 Bs : 1;

                            /* Bits[14:14], Access Type=RW, default=None*/

                            /*
                               The debug exception was triggered by a single-
                               step trap.
                            */
    UINT32 Bt : 1;

                            /* Bits[15:15], Access Type=RW, default=None*/

                            /*
                               The debug exception was triggered by a TSS T-bit
                               trap.
                            */
    UINT32 MustBeOneHigh : 16;

                            /* Bits[31:16], Access Type=RW, default=None*/

                            /* Bits which are architecturally defined to be 1. */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SMRAM_DR6_REGISTER;

/**
  Available only in SMM. SMRAM copy of DR7 saved by SMI
**/
#define MSR_SMRAM_DR7 0x00000C06

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 L0 : 1;

                            /* Bits[0:0], Access Type=RW, default=None*/

                            /* Local breakpoint enable for DR0 */
    UINT32 G0 : 1;

                            /* Bits[1:1], Access Type=RW, default=None*/

                            /* Global breakpoint enable for DR0 */
    UINT32 L1 : 1;

                            /* Bits[2:2], Access Type=RW, default=None*/

                            /* Local breakpoint enable for DR1 */
    UINT32 G1 : 1;

                            /* Bits[3:3], Access Type=RW, default=None*/

                            /* Global breakpoint enable for DR1 */
    UINT32 L2 : 1;

                            /* Bits[4:4], Access Type=RW, default=None*/

                            /* Local breakpoint enable for DR2 */
    UINT32 G2 : 1;

                            /* Bits[5:5], Access Type=RW, default=None*/

                            /* Global breakpoint enable for DR2 */
    UINT32 L3 : 1;

                            /* Bits[6:6], Access Type=RW, default=None*/

                            /* Local breakpoint enable for DR3 */
    UINT32 G3 : 1;

                            /* Bits[7:7], Access Type=RW, default=None*/

                            /* Global breakpoint enable for DR3 */
    UINT32 Le : 1;

                            /* Bits[8:8], Access Type=RW, default=None*/

                            /* Local exact breakpoint enable */
    UINT32 Ge : 1;

                            /* Bits[9:9], Access Type=RW, default=None*/

                            /* Global exact breakpoint enable */
    UINT32 Rsvd10 : 3;

                            /* Bits[12:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Gd : 1;

                            /* Bits[13:13], Access Type=RW, default=None*/

                            /*
                               Enables the debug register protection condition
                               that is flagged by BD of DR6
                            */
    UINT32 Rsvd14 : 2;

                            /* Bits[15:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rw0 : 2;

                            /* Bits[17:16], Access Type=RW, default=None*/

                            /* Breakpoint condition for DR0 */
    UINT32 Len0 : 2;

                            /* Bits[19:18], Access Type=RW, default=None*/

                            /* Size of memory breakpoint location for DR0 */
    UINT32 Rw1 : 2;

                            /* Bits[21:20], Access Type=RW, default=None*/

                            /* Breakpoint condition for DR1 */
    UINT32 Len1 : 2;

                            /* Bits[23:22], Access Type=RW, default=None*/

                            /* Size of memory breakpoint location for DR1 */
    UINT32 Rw2 : 2;

                            /* Bits[25:24], Access Type=RW, default=None*/

                            /* Breakpoint condition for DR2 */
    UINT32 Len2 : 2;

                            /* Bits[27:26], Access Type=RW, default=None*/

                            /* Size of memory breakpoint location for DR2 */
    UINT32 Rw3 : 2;

                            /* Bits[29:28], Access Type=RW, default=None*/

                            /* Breakpoint condition for DR3 */
    UINT32 Len3 : 2;

                            /* Bits[31:30], Access Type=RW, default=None*/

                            /* Size of memory breakpoint location for DR3 */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SMRAM_DR7_REGISTER;

/**
  Available only in SMM. SMRAM copy of TR Sel and LDTR Sel saved by SMI
**/
#define MSR_SMRAM_TR_LDTR 0x00000C07

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 LdtrSel : 16;

                            /* Bits[15:0], Access Type=RW, default=None*/

                            /* LDTR Selector */
    UINT32 Rsvd16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 TrSel : 16;

                            /* Bits[47:32], Access Type=RW, default=None*/

                            /* TR Selector */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SMRAM_TR_LDTR_REGISTER;

/**
  Available only in SMM. SMRAM copy of GS and FS saved by SMI
**/
#define MSR_SMRAM_GS_FS 0x00000C08

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Fs : 16;

                            /* Bits[15:0], Access Type=RW, default=None*/

                            /* FS */
    UINT32 Rsvd16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Gs : 16;

                            /* Bits[47:32], Access Type=RW, default=None*/

                            /* GS */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SMRAM_GS_FS_REGISTER;

/**
  Available only in SMM. SMRAM copy of DS and SS saved by SMI
**/
#define MSR_SMRAM_DS_SS 0x00000C09

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Ss : 16;

                            /* Bits[15:0], Access Type=RW, default=None*/

                            /* FS */
    UINT32 Rsvd16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Ds : 16;

                            /* Bits[47:32], Access Type=RW, default=None*/

                            /* GS */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SMRAM_DS_SS_REGISTER;

/**
  Available only in SMM. SMRAM copy of CS and ES saved by SMI
**/
#define MSR_SMRAM_CS_ES 0x00000C0A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Es : 16;

                            /* Bits[15:0], Access Type=RW, default=None*/

                            /* FS */
    UINT32 Rsvd16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Cs : 16;

                            /* Bits[47:32], Access Type=RW, default=None*/

                            /* GS */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SMRAM_CS_ES_REGISTER;

/**
  Available only in SMM. SMRAM copy of IOMISCINFO saved by SMI
**/
#define MSR_SMRAM_IOMISCINFO 0x00000C0B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Iomiscinfo : 32;

                            /* Bits[31:0], Access Type=RW, default=None*/

                            /* Copy of ROB IOMISCINFO */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SMRAM_IOMISCINFO_REGISTER;
/**
  Available only in SMM. SMRAM copy of IO_MEM_ADDR saved by SMI
**/
#define MSR_SMRAM_IO_MEM_ADDR 0x00000C0C
/**
  Available only in SMM. SMRAM copy of RDI saved by SMI
**/
#define MSR_SMRAM_RDI 0x00000C0D
/**
  Available only in SMM. SMRAM copy of RSI saved by SMI
**/
#define MSR_SMRAM_RSI 0x00000C0E
/**
  Available only in SMM. SMRAM copy of RBP saved by SMI
**/
#define MSR_SMRAM_RBP 0x00000C0F
/**
  Available only in SMM. SMRAM copy of RSP saved by SMI
**/
#define MSR_SMRAM_RSP 0x00000C10
/**
  Available only in SMM. SMRAM copy of RBX saved by SMI
**/
#define MSR_SMRAM_RBX 0x00000C11
/**
  Available only in SMM. SMRAM copy of RDX saved by SMI
**/
#define MSR_SMRAM_RDX 0x00000C12
/**
  Available only in SMM. SMRAM copy of RCX saved by SMI
**/
#define MSR_SMRAM_RCX 0x00000C13
/**
  Available only in SMM. SMRAM copy of RAX saved by SMI
**/
#define MSR_SMRAM_RAX 0x00000C14
/**
  Available only in SMM. SMRAM copy of R8 saved by SMI
**/
#define MSR_SMRAM_R8 0x00000C15
/**
  Available only in SMM. SMRAM copy of R9 saved by SMI
**/
#define MSR_SMRAM_R9 0x00000C16
/**
  Available only in SMM. SMRAM copy of R10 saved by SMI
**/
#define MSR_SMRAM_R10 0x00000C17
/**
  Available only in SMM. SMRAM copy of R11 saved by SMI
**/
#define MSR_SMRAM_R11 0x00000C18
/**
  Available only in SMM. SMRAM copy of R12 saved by SMI
**/
#define MSR_SMRAM_R12 0x00000C19
/**
  Available only in SMM. SMRAM copy of R13 saved by SMI
**/
#define MSR_SMRAM_R13 0x00000C1A
/**
  Available only in SMM. SMRAM copy of R14 saved by SMI
**/
#define MSR_SMRAM_R14 0x00000C1B
/**
  Available only in SMM. SMRAM copy of RDI saved by SMI
**/
#define MSR_SMRAM_R15 0x00000C1C

/**
  Available only in SMM. SMRAM copy of EVENT_CTL, HLT flag and IO flag saved by SMI
**/
#define MSR_SMRAM_EVENT_CTL_HLT_IO 0x00000C1F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 IoFlag : 16;

                            /* Bits[15:0], Access Type=RW, default=None*/

                            /* Copy of IO Flag */
    UINT32 HltFlag : 16;

                            /* Bits[31:16], Access Type=RW, default=None*/

                            /* Copy of HLT Flag */
    UINT32 EventCtl : 16;

                            /* Bits[47:32], Access Type=RW, default=None*/

                            /* Copy of EVENT_CTL */
    UINT32 Rsvd48 : 16;

                            /* Bits[63:48], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SMRAM_EVENT_CTL_HLT_IO_REGISTER;

/**
  Available only in SMM. SMRAM copy of SMBASE saved by SMI
**/
#define MSR_SMRAM_SMBASE 0x00000C20

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Smbase : 32;

                            /* Bits[31:0], Access Type=RW, default=None*/

                            /* Copy of SMBASE */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SMRAM_SMBASE_REGISTER;

/**
  Available only in SMM. SMRAM copy of SMM_REVID saved by SMI
**/
#define MSR_SMRAM_SMM_REVID 0x00000C21

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 SmmRevid : 32;

                            /* Bits[31:0], Access Type=RW, default=None*/

                            /* Copy of SMM_REVID */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SMRAM_SMM_REVID_REGISTER;

/**
  Available only in SMM. SMRAM copy of IEDBASE saved by SMI
**/
#define MSR_SMRAM_IEDBASE 0x00000C22

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Rsvd0 : 32;

                            /* Bits[31:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Iedbase : 32;

                            /* Bits[63:32], Access Type=RW, default=None*/

                            /* Copy of IEDBASE */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SMRAM_IEDBASE_REGISTER;

/**
  Available only in SMM. SMRAM copy of EPTP_ENABLE saved by SMI
**/
#define MSR_SMRAM_EPTP_ENABLE 0x00000C23

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 EptpEnable : 1;

                            /* Bits[0:0], Access Type=RW, default=None*/

                            /* Copy of EPTP_ENABLE */
    UINT32 EnclaveInterruption : 1;

                            /* Bits[1:1], Access Type=RW, default=None*/

                            /* Copy of ENCLAVE_INTERRUPTION */
    UINT32 Rsvd2 : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rsvd32 : 32;

                            /* Bits[63:32], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_SMRAM_EPTP_ENABLE_REGISTER;
/**
  Available only in SMM. SMRAM copy of EPTP saved by SMI
**/
#define MSR_SMRAM_EPTP 0x00000C24
/**
  Available only in SMM. SMRAM copy of BNDCFGS saved by SMI
**/
#define MSR_SMRAM_BNDCFGS 0x00000C25
/**
  Available only in SMM. SMRAM copy of LDTR_BASE saved by SMI
**/
#define MSR_SMRAM_LDTR_BASE 0x00000C2C
/**
  Available only in SMM. SMRAM copy of IDTR_BASE saved by SMI
**/
#define MSR_SMRAM_IDTR_BASE 0x00000C2D
/**
  Available only in SMM. SMRAM copy of IDTR_BASE saved by SMI
**/
#define MSR_SMRAM_GDTR_BASE 0x00000C2E
/**
  Available only in SMM. SMRAM copy of IO_RSI saved by SMI
**/
#define MSR_SMRAM_IO_RSI 0x00000C40
/**
  Available only in SMM. SMRAM copy of IO_RCX saved by SMI
**/
#define MSR_SMRAM_IO_RCX 0x00000C41
/**
  Available only in SMM. SMRAM copy of IO_RIP saved by SMI
**/
#define MSR_SMRAM_IO_RIP 0x00000C42
/**
  Available only in SMM. SMRAM copy of IO_RDI saved by SMI
**/
#define MSR_SMRAM_IO_RDI 0x00000C43

/**
  CDP enable
**/
#define MSR_PQOS_CDP_ENABLE 0x00000C81

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Cdp : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* enable */
    UINT32 Rsvd1 : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_PQOS_CDP_ENABLE_REGISTER;

/**
  MSR that is used to receive query id from software for which data will be looked up by another MSR
**/
#define MSR_QOSEVTSEL 0x00000C8D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Evtid : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               0x01 for LLC occupancy monitoring, 0x00 for no
                               monitoring, remaining for future extensibility
                            */
    UINT32 Rsvd8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 Rmid : 10;

                            /* Bits[41:32], Access Type=RW, default=0x00000000*/

                            /* 1 = unquiesce initiated */
    UINT32 Rsvd42 : 22;

                            /* Bits[63:42], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_QOSEVTSEL_REGISTER;

/**
  MSR that is used to convey the counter value for the RMID queried by the QOSEVTSEL MSR.  This is a Read only MSR.
**/
#define MSR_QMC 0x00000C8E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 MonitoringData0 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Data for corresponding RMID is populated in this */
    UINT32 MonitoringData1 : 30;

                            /* Bits[61:32], Access Type=RW, default=0x00000000*/

                            /* Data for corresponding RMID is populated in this */
    UINT32 Availability : 1;

                            /* Bits[62:62], Access Type=RW, default=0x00000000*/

                            /* 1 = if data available, 0 if data not available */
    UINT32 Error : 1;

                            /* Bits[63:63], Access Type=RW, default=0x00000000*/

                            /* 1 = Error, 0 = no error */

  } Bits;

  UINT32 Uint32;
  UINT64 Uint64;

} MSR_QMC_REGISTER;
/**
  Accumulates value of THREAD_TSC_SAMPLE_PRE_OPD_ENTER on every OPD_EXIT when transitioning back to C0
**/
#define MSR_THREAD_STALL_COUNTER 0x00000DB2
#endif /* _COMMONMSR_h */
